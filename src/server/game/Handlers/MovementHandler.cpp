/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AccountMgr.h"
#include "Battleground.h"
#include "Common.h"
#include "Corpse.h"
#include "Garrison.h"
#include "InstanceSaveMgr.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "SpellAuras.h"
#include "Transport.h"
#include "Vehicle.h"
#include "VMapFactory.h"
#include "WaypointMovementGenerator.h"
#include "WorldSession.h"

#include "MovementPackets.h"
#include "InstancePackets.h"
#include "WorldPacket.h"

#define MOVEMENT_PACKET_TIME_DELAY 0

void WorldSession::HandleWorldPortAck(WorldPackets::Movement::WorldPortAck& /*packet*/)
{
    HandleWorldPortAck();
}

void WorldSession::HandleWorldPortAck()
{
    Player* player = GetPlayer();
    if (!player->IsBeingTeleportedFar())
        return;

    bool seamlessTeleport = player->IsBeingTeleportedSeamlessly();
    player->SetSemaphoreTeleportFar(false);

    if (Unit* mover = player->m_mover)
        mover->ClearUnitState(UNIT_STATE_JUMPING);

    WorldLocation const loc = player->GetTeleportDest();
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());
    InstanceTemplate const* mInstance = sObjectMgr->GetInstanceTemplate(loc.GetMapId());

    if (player->m_InstanceValid == false && !mInstance)
        player->m_InstanceValid = true;

    Map* oldMap = player->GetMap();
    if (player->IsInWorld())
        oldMap->RemovePlayerFromMap(player, false);

    Map* newMap = sMapMgr->CreateMap(loc.GetMapId(), player);
    if (!newMap || !newMap->CanEnter(player))
    {
        player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, player->GetOrientation());
        return;
    }
    else
        player->Relocate(&loc);

    player->ResetMap();
    player->SetMap(newMap);

    if (seamlessTeleport)
    {
        if (newMap->IsDungeon())
            player->SendDirectMessage(WorldPackets::Instance::UpdateLastInstance(loc.GetMapId()).Write());

        WorldPackets::Movement::NewWorld packet;
        packet.MapID = loc.GetMapId();
        packet.Pos = loc;
        packet.Reason = WorldPackets::Movement::NewWorldReason::SEAMLESS;
        player->SendDirectMessage(packet.Write());
        player->SendSavedInstances();
    }
    else
        player->SendInitialPacketsBeforeAddToMap();

    if (!player->GetMap()->AddPlayerToMap(player, !seamlessTeleport))
    {
        player->ResetMap();
        player->SetMap(oldMap);
        player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, player->GetOrientation());
        return;
    }

    if (player->InBattleground())
    {
        if (!mEntry->IsBattlegroundOrArena())
        {
            if (Battleground* bg = player->GetBattleground())
                player->LeaveBattleground(false);

            player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            player->SetBGTeam(0);
        }
        else if (Battleground* bg = player->GetBattleground())
        {
            if (player->IsInvitedForBattlegroundInstance(player->GetBattlegroundId()))
                bg->AddPlayer(player);
        }
    }

    if (!seamlessTeleport)
        player->SendInitialPacketsAfterAddToMap();
    else
    {
        player->UpdateVisibilityForPlayer();
        if (Garrison* garrison = player->GetGarrison())
            garrison->SendRemoteInfo();
    }

    if (player->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!player->InBattleground())
        {
            if (!seamlessTeleport)
            {
                FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(player->GetMotionMaster()->top());
                flight->Initialize(*player);
            }
            return;
        }

        player->GetMotionMaster()->MovementExpired();
        player->CleanupAfterTaxiFlight();
    }

    Corpse* corpse = player->GetCorpse();
    if (corpse && corpse->GetType() != CORPSE_BONES && corpse->GetMapId() == player->GetMapId())
    {
        if (mEntry->IsDungeon())
        {
            player->ResurrectPlayer(0.5f, false);
            player->SpawnCorpseBones();
        }
    }

    bool allowMount = !mEntry->IsDungeon() || mEntry->IsBattlegroundOrArena();
    if (mInstance)
    {
        Difficulty diff = player->GetDifficultyID(mEntry);
        if (MapDifficultyEntry const* mapDiff = sDB2Manager.GetMapDifficultyData(mEntry->ID, diff))
            if (mapDiff->RaidDuration)
                if (time_t timeReset = sWorld->getInstanceResetTime(mapDiff->RaidDuration))
                    player->SendInstanceResetWarning(mEntry->ID, diff, uint32(timeReset - time(nullptr)));

        allowMount = mInstance->AllowMount;
    }

    if (!allowMount || (player->GetMapId() == 530 && player->GetZoneId() == 0)) //530 - uwow event map
        player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    uint32 newzone, newarea;
    player->GetZoneAndAreaId(newzone, newarea);
    player->UpdateZone(newzone, newarea);

    if (player->pvpInfo.inHostileArea)
        player->CastSpell(player, 2479, true);
    else if (player->IsPvP() && !player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
        player->UpdatePvP(false, false);

    player->ResummonPetTemporaryUnSummonedIfAny();
    player->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAck(WorldPackets::Movement::MoveTeleportAck& packet)
{
    Player* plMover = _player->m_mover->ToPlayer();

    if (plMover && plMover->IsBeingTeleportedSeamlessly())
    {
        HandleWorldPortAck();
        return;
    }

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if (packet.MoverGUID != plMover->GetGUID())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();
    plMover->UpdatePosition(plMover->GetTeleportDest(), true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    if (old_zone != newzone)
    {
        if (plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);

        else if (plMover->IsPvP() && !plMover->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
            plMover->UpdatePvP(false, false);
    }

    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
    GetPlayer()->ProcessDelayedOperations();

    if (Unit* mover = _player->m_mover)
    {
        mover->m_movementInfo.time = getMSTime();
        mover->m_movementInfo.pos = mover->GetPosition();

        WorldPackets::Movement::MoveUpdate playerMovement;
        playerMovement.movementInfo = &mover->m_movementInfo;
        mover->SendMessageToSet(playerMovement.Write(), mover);

        mover->ClearUnitState(UNIT_STATE_JUMPING);
    }
}

void WorldSession::HandleMovementOpcodes(WorldPackets::Movement::ClientPlayerMovement& packet)
{
    Unit* mover = _player->m_mover;
    if (!mover)
        return;

    Player* plrMover = mover->ToPlayer();
    if (Vehicle const* vehMover = mover->GetVehicleKit())
        if (mover->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
            if (Unit* charmer = mover->GetCharmer())
                if (charmer->GetTypeId() == TYPEID_PLAYER)
                    plrMover = static_cast<Player*>(charmer);

    if (plrMover && plrMover->IsBeingTeleported())
        return;

    GetPlayer()->ValidateMovementInfo(&packet.movementInfo);

    MovementInfo& movementInfo = packet.movementInfo;
    if (movementInfo.guid != mover->GetGUID() || !movementInfo.pos.IsPositionValid())
        return;

    if (plrMover && (plrMover->GetUInt32Value(UNIT_NPC_EMOTESTATE) != 0))
        plrMover->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);

    if (mover->HasAuraType(SPELL_AURA_MOD_POSSESS) || (plrMover && plrMover->HasAuraType(SPELL_AURA_MOD_POSSESS)))
        if (movementInfo.flags & MOVEMENTFLAG_WALKING)
            movementInfo.flags &= ~MOVEMENTFLAG_WALKING;

    if (!movementInfo.transport.guid.IsEmpty())
    {
        if (movementInfo.transport.pos.GetPositionX() > 50.0f || movementInfo.transport.pos.GetPositionY() > 50.0f || movementInfo.transport.pos.GetPositionZ() > 50.0f)
            return;

        if (!Trinity::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.transport.pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.transport.pos.GetPositionY(),
            movementInfo.pos.GetPositionZ() + movementInfo.transport.pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.transport.pos.GetOrientation()))
            return;

        if (plrMover)
        {
            if (!plrMover->GetTransport())
            {
                for (auto const& iter : sMapMgr->m_Transports)
                    if (iter->GetGUID() == movementInfo.transport.guid)
                    {
                        plrMover->m_transport = iter;
                        iter->AddPassenger(plrMover);
                        break;
                    }
            }
            else if (plrMover->GetTransport()->GetGUID() != movementInfo.transport.guid)
            {
                bool foundNewTransport = false;
                plrMover->m_transport->RemovePassenger(plrMover);
                for (auto const& iter : sMapMgr->m_Transports)
                    if (iter->GetGUID() == movementInfo.transport.guid)
                    {
                        foundNewTransport = true;
                        plrMover->m_transport = iter;
                        iter->AddPassenger(plrMover);
                        break;
                    }

                if (!foundNewTransport)
                {
                    plrMover->m_transport = nullptr;
                    plrMover->m_movementInfo.transport.Reset();
                }
            }
        }

        if (!mover->GetTransport() && !mover->GetVehicle())
            if (GameObject* go = mover->GetMap()->GetGameObject(movementInfo.transport.guid))
                if (go->GetGoType() != GAMEOBJECT_TYPE_TRANSPORT)
                    movementInfo.transport.Reset();
    }
    else if (plrMover && plrMover->GetTransport())
        plrMover->m_transport->RemovePassenger(plrMover);
        
    OpcodeClient opcode = packet.GetOpcode();
    if (opcode == CMSG_MOVE_FALL_LAND && plrMover && !plrMover->isInFlight())
        plrMover->HandleFall(movementInfo);

    if (plrMover && ((movementInfo.flags & MOVEMENTFLAG_SWIMMING) != 0) != plrMover->IsInWater())
        plrMover->SetInWater(!plrMover->IsInWater() || plrMover->GetBaseMap()->IsUnderWater(movementInfo.pos));

    if (plrMover && mover)
        if (Vehicle const* veh = mover->GetVehicleKit())
            if (Unit* base = veh->GetBase())
                if (Creature* vehCreature = base->ToCreature())
                    if (!vehCreature->isInAccessiblePlaceFor(vehCreature))
                        plrMover->ExitVehicle();

    if (m_clientTimeDelay == 0)
        m_clientTimeDelay = getMSTime() - movementInfo.time;

    movementInfo.time = movementInfo.time + m_clientTimeDelay + MOVEMENT_PACKET_TIME_DELAY;

    movementInfo.guid = mover->GetGUID();
    mover->m_movementInfo = movementInfo;

    if (Vehicle* vehicle = mover->GetVehicle())
    {
        if (VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(mover))
            if (seat->Flags & VEHICLE_SEAT_FLAG_ALLOW_TURNING)
                if (movementInfo.pos.GetOrientation() != mover->GetOrientation())
                {
                    mover->SetOrientation(movementInfo.pos.GetOrientation());
                    mover->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TURNING);
                }

        return;
    }

    mover->UpdatePosition(movementInfo.pos);

    WorldPackets::Movement::MoveUpdate moveUpdate;
    moveUpdate.movementInfo = &mover->m_movementInfo;
    mover->SendMessageToSet(moveUpdate.Write(), _player);

    if (plrMover)
    {
        if (plrMover->IsSitState() && (movementInfo.HasMovementFlag(MOVEMENTFLAG_MASK_MOVING | MOVEMENTFLAG_MASK_TURNING)))
            plrMover->SetStandState(UNIT_STAND_STATE_STAND);

        plrMover->UpdateFallInformationIfNeed(movementInfo, opcode);

        if (movementInfo.pos.GetPositionZ() < plrMover->GetMap()->GetMinHeight(movementInfo.pos))
            if (!(plrMover->GetBattleground() && plrMover->GetBattleground()->HandlePlayerUnderMap(_player)))
                if (plrMover->isAlive())
                {
                    plrMover->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IS_OUT_OF_BOUNDS);
                    plrMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                    if (!plrMover->isAlive())
                        plrMover->KillPlayer();
                }
    }
}

void WorldSession::HandleForceSpeedChangeAck(WorldPackets::Movement::MovementSpeedAck& packet)
{
    Player* player = GetPlayer();
    player->ValidateMovementInfo(&packet.Ack.movementInfo);

    if (player->GetGUID() != packet.Ack.movementInfo.guid)
        return;

    UnitMoveType move_type;

    OpcodeClient opcode = packet.GetOpcode();
    switch (opcode)
    {
        case CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK:        move_type = MOVE_WALK;        break;
        case CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK:         move_type = MOVE_RUN;         break;
        case CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK:    move_type = MOVE_RUN_BACK;    break;
        case CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK:        move_type = MOVE_SWIM;        break;
        case CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:   move_type = MOVE_SWIM_BACK;   break;
        case CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK:         move_type = MOVE_TURN_RATE;   break;
        case CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK:      move_type = MOVE_FLIGHT;      break;
        case CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK: move_type = MOVE_FLIGHT_BACK; break;
        case CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK:        move_type = MOVE_PITCH_RATE;  break;
        default:
            return;
    }

    if (player->m_forced_speed_changes[move_type] > 0)
    {
        --player->m_forced_speed_changes[move_type];
        if (player->m_forced_speed_changes[move_type] > 0)
            return;
    }

    if (!player->GetTransport() && std::fabs(player->GetSpeed(move_type) - packet.Speed) > 0.01f)
    {
        if (player->GetSpeed(move_type) > packet.Speed)
            player->SetSpeed(move_type, player->GetSpeedRate(move_type), true);
        else
            player->GetSession()->KickPlayer();
    }
}

void WorldSession::HandleMoveKnockBackAck(WorldPackets::Movement::MovementAckMessage& packet)
{
    Player* player = GetPlayer();
    player->ValidateMovementInfo(&packet.Ack.movementInfo);

    if (player->m_mover->GetGUID() != packet.Ack.movementInfo.guid)
        return;

    player->m_movementInfo = packet.Ack.movementInfo;

    WorldPackets::Movement::MoveUpdateKnockBack updateKnockBack;
    updateKnockBack.movementInfo = &player->m_movementInfo;
    player->SendMessageToSet(updateKnockBack.Write(), false);
}

void WorldSession::HandleMovementAckMessage(WorldPackets::Movement::MovementAckMessage& packet)
{
    GetPlayer()->ValidateMovementInfo(&packet.Ack.movementInfo);
}

void WorldSession::HandleSetCollisionHeightAck(WorldPackets::Movement::MoveSetCollisionHeightAck& packet)
{
    GetPlayer()->ValidateMovementInfo(&packet.Data.movementInfo);
}

void WorldSession::HandleSetActiveMover(WorldPackets::Movement::SetActiveMover& packet)
{
    if (Player* player = GetPlayer())
        if (player->IsInWorld())
        {
            if (player->m_mover->GetGUID() != packet.ActiveMover)
                sLog->outError(LOG_FILTER_NETWORKIO, "HandleSetActiveMover: incorrect mover guid: mover is %s  and should be %s",
                packet.ActiveMover.ToString().c_str(), player->m_mover->GetGUID().ToString().c_str());
        }
}

void WorldSession::HandleMoveTimeSkipped(WorldPackets::Movement::MoveTimeSkipped& /*packet*/)
{
}

void WorldSession::HandleMoveSplineDone(WorldPackets::Movement::MoveSplineDone& packet)
{
    MovementInfo movementInfo = packet.movementInfo;
    _player->ValidateMovementInfo(&movementInfo);

    // in taxi flight packet received in 2 case:
    // 1) end taxi path in far (multi-node) flight
    // 2) switch from one map to other in case multim-map taxi path
    // we need process only (1)

    uint32 curDest = GetPlayer()->m_taxi.GetTaxiDestination();
    if (curDest)
    {
        TaxiNodesEntry const* curDestNode = sTaxiNodesStore.LookupEntry(curDest);

        // far teleport case
        if (curDestNode && curDestNode->MapID != GetPlayer()->GetMapId())
        {
            if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
            {
                FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());

                flight->SetCurrentNodeAfterTeleport();
                TaxiPathNodeEntry const* node = flight->GetPath()[flight->GetCurrentNode()];
                flight->SkipCurrentNode();

                GetPlayer()->TeleportTo(curDestNode->MapID, node->Loc.X, node->Loc.Y, node->Loc.Z, GetPlayer()->GetOrientation());
            }
        }

        return;
    }

    if (GetPlayer()->m_taxi.GetPath().size() != 1)
        return;

    GetPlayer()->CleanupAfterTaxiFlight();
    GetPlayer()->SetFallInformation(0, GetPlayer()->GetPositionZ());
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);
}

void WorldSession::HandleMoveRemoveMovementForceAck(WorldPackets::Movement::MoveRemoveMovementForceAck& packet)
{
//    Player* player = GetPlayer();
//    player->ValidateMovementInfo(&packet.Ack.movementInfo);
//    if (player->m_mover->GetGUID() != packet.Ack.movementInfo.MoverGUID)
//        return;
//
//    player->m_movementInfo = packet.Ack.movementInfo;
//    player->m_movementInfo.RemoveForcesIDs.emplace_back(packet.TriggerGUID);
//
//    auto it = player->m_movementInfo.Forces.find(packet.TriggerGUID);
//    if (it != player->m_movementInfo.Forces.end())
//        player->m_movementInfo.Forces.erase(it);
//
//    WorldPackets::Movement::MoveUpdateRemoveMovementForce remove;
//    remove.movementInfo = &player->m_movementInfo;
//    remove.TriggerGUID = packet.TriggerGUID;
//    player->SendMessageToSet(remove.Write(), player);
//}

void WorldSession::HandleMoveApplyMovementForceAck(WorldPackets::Movement::MoveApplyMovementForceAck& packet)
{
    //Player* player = GetPlayer();
    //player->ValidateMovementInfo(&packet.Ack.movementInfo);
    //if (player->m_mover->GetGUID() != packet.Ack.movementInfo.MoverGUID)
    //    return;

    //player->m_movementInfo = packet.Ack.movementInfo;
    //player->m_movementInfo.Forces[packet.MovementForceData.ID] = packet.MovementForceData;

    //WorldPackets::Movement::MoveUpdateApplyMovementForce apply;
    //apply.movementInfo = &player->m_movementInfo;
    //apply.MovementForceData = packet.MovementForceData;
    //player->SendMessageToSet(apply.Write(), player);
}
