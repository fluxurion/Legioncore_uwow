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

#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "GossipDef.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "WorldSession.h"
#include "BigNumber.h"
#include "SHA1.h"
#include "UpdateData.h"
#include "LootMgr.h"
#include "Chat.h"
#include "zlib.h"
#include "ObjectAccessor.h"
#include "Object.h"
#include "Battleground.h"
#include "OutdoorPvP.h"
#include "Pet.h"
#include "SocialMgr.h"
#include "CellImpl.h"
#include "AccountMgr.h"
#include "Vehicle.h"
#include "CreatureAI.h"
#include "DBCEnums.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "InstanceScript.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "AccountMgr.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Bracket.h"
#include "LFGMgr.h"
#include "MiscPackets.h"
#include "CharacterPackets.h"
#include "ItemPackets.h"
#include "InstancePackets.h"
#include "ReputationPackets.h"
#include "AuthenticationPackets.h"

void WorldSession::HandleRepopRequest(WorldPackets::Misc::RepopRequest& packet)
{
    if (GetPlayer()->isAlive() || GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    if (GetPlayer()->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silently return, client should display the error by itself

    // the world update order is sessions, players, creatures
    // the netcode runs in parallel with all of these
    // creatures can kill players
    // so if the server is lagging enough the player can
    // release spirit after he's killed but before he is updated
    if (GetPlayer()->getDeathState() == JUST_DIED)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleRepopRequestOpcode: got request after player %s(%d) was killed and before he was updated", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        GetPlayer()->KillPlayer();
    }

    //this is spirit release confirm?
    GetPlayer()->RemovePet(NULL);
    GetPlayer()->BuildPlayerRepop();
    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleLogoutCancel(WorldPackets::Character::LogoutCancel& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    LogoutRequest(0);

    SendPacket(WorldPackets::Character::LogoutCancelAck().Write());

    if (player->CanFreeMove())
    {
        player->SetRooted(false);
        player->SetStandState(UNIT_STAND_STATE_STAND);
        player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    player->PetSpellInitialize();
}

void WorldSession::HandleTogglePvP(WorldPackets::Misc::TogglePvP& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    bool inPvP = player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP);

    player->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, inPvP ? PLAYER_FLAGS_PVP_TIMER : PLAYER_FLAGS_IN_PVP, inPvP);

    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
    {
        if (!player->IsPvP() || player->pvpInfo.endTimer)
            player->UpdatePvP(true, true);
    }
    else
    {
        if (!player->pvpInfo.inHostileArea && player->IsPvP())
            player->pvpInfo.endTimer = time(nullptr);
    }
}

void WorldSession::HandleSetPvP(WorldPackets::Misc::SetPvP& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, packet.EnablePVP ? PLAYER_FLAGS_IN_PVP: PLAYER_FLAGS_PVP_TIMER, packet.EnablePVP);

    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
    {
        if (!player->IsPvP() || player->pvpInfo.endTimer)
            player->UpdatePvP(true, true);
    }
    else
    {
        if (!player->pvpInfo.inHostileArea && player->IsPvP())
            player->pvpInfo.endTimer = time(nullptr);
    }
}

void WorldSession::HandlePortGraveyard(WorldPackets::Misc::PortGraveyard& /*packet*/)
{
    if (GetPlayer()->isAlive() || !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleSetSelectionOpcode(WorldPackets::Misc::SetSelection& packet)
{
    _player->SetSelection(packet.Selection);
}

void WorldSession::HandleStandStateChangeOpcode(WorldPackets::Misc::StandStateChange& packet)
{
    _player->SetStandState(packet.StandState);
}

void WorldSession::HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string friendNote)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    ObjectGuid friendGuid;
    uint32 friendAccountId;
    uint32 team;
    FriendsResult friendResult;

    friendResult = FRIEND_NOT_FOUND;

    if (result)
    {
        Field* fields = result->Fetch();

        friendGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt32());
        team = Player::TeamForRace(fields[1].GetUInt8());
        friendAccountId = fields[2].GetUInt32();

        if (!AccountMgr::IsPlayerAccount(GetSecurity()) || sWorld->getBoolConfig(CONFIG_ALLOW_GM_FRIEND) || AccountMgr::IsPlayerAccount(AccountMgr::GetSecurity(friendAccountId, realm.Id.Realm)))
        {
            if (!friendGuid.IsEmpty())
            {
                if (friendGuid == player->GetGUID())
                    friendResult = FRIEND_SELF;
                else if (player->GetTeam() != team && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND) && AccountMgr::IsPlayerAccount(GetSecurity()))
                    friendResult = FRIEND_ENEMY;
                else if (player->GetSocial()->HasFriend(friendGuid))
                    friendResult = FRIEND_ALREADY;
                else
                {
                    Player* pFriend = ObjectAccessor::FindPlayer(friendGuid);
                    if (pFriend && pFriend->IsInWorld() && pFriend->IsVisibleGloballyFor(player))
                        friendResult = FRIEND_ADDED_ONLINE;
                    else
                        friendResult = FRIEND_ADDED_OFFLINE;
                    if (!player->GetSocial()->AddToSocialList(friendGuid, SOCIAL_FLAG_FRIEND))
                    {
                        friendResult = FRIEND_LIST_FULL;
                        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s's friend list is full.", player->GetName());
                    }
                }
                player->GetSocial()->SetFriendNote(friendGuid, friendNote);
            }
        }
    }

    sSocialMgr->SendFriendStatus(player, friendResult, friendGuid, false);
}

void WorldSession::HandleReclaimCorpse(WorldPackets::Misc::ReclaimCorpse& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->isAlive() || player->InArena() || !player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    Corpse* corpse = player->GetCorpse();
    if (!corpse)
        return;

    if (time_t(corpse->GetGhostTime() + player->GetCorpseReclaimDelay(corpse->GetType() == CORPSE_RESURRECTABLE_PVP)) > time_t(time(NULL)))
        return;

    if (!corpse->IsWithinDist(player, CORPSE_RECLAIM_RADIUS, true) || !corpse->IsInMap(player))
        return;

    player->ResurrectPlayer(player->InBattleground() ? 1.0f : 0.5f);
    player->SpawnCorpseBones();
}

void WorldSession::HandleResurrectResponse(WorldPackets::Misc::ResurrectResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->isAlive())
        return;

    if (packet.Response != 0)
    {
        player->ClearResurrectRequestData();
        return;
    }

    if (!player->IsRessurectRequestedBy(packet.Resurrecter))
        return;

    player->ResurectUsingRequestData();
}

void WorldSession::HandleAreaTrigger(WorldPackets::Misc::AreaTrigger& packet)
{
    Player* player = GetPlayer();
    if (player->isInFlight())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) in flight, ignore Area Trigger ID:%u",
            player->GetName(), player->GetGUIDLow(), packet.AreaTriggerID);
        return;
    }

    AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(packet.AreaTriggerID);
    if (!atEntry)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) send unknown (by DBC) Area Trigger ID:%u",
            player->GetName(), player->GetGUIDLow(), packet.AreaTriggerID);
        return;
    }

    if (player->GetMapId() != atEntry->MapID)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) too far (trigger map: %u player map: %u), ignore Area Trigger ID: %u",
            player->GetName(), atEntry->MapID, player->GetMapId(), player->GetGUIDLow(), packet.AreaTriggerID);
        return;
    }

    // delta is safe radius
    const float delta = 5.0f;

    if (atEntry->Radius)
    {
        // if we have radius check it
        float dist = player->GetDistance(atEntry->Pos.X, atEntry->Pos.Y, atEntry->Pos.Z);
        if (dist > atEntry->Radius + delta)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) too far (radius: %f distance: %f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUIDLow(), atEntry->Radius, dist, packet.AreaTriggerID);
            return;
        }
    }
    else
    {
        // we have only extent

        // rotate the players position instead of rotating the whole cube, that way we can make a simplified
        // is-in-cube check and we have to calculate only one point instead of 4

        // 2PI = 360°, keep in mind that ingame orientation is counter-clockwise
        double rotation = 2 * M_PI - atEntry->BoxYaw;
        double sinVal = std::sin(rotation);
        double cosVal = std::cos(rotation);

        float playerBoxDistX = player->GetPositionX() - atEntry->Pos.X;
        float playerBoxDistY = player->GetPositionY() - atEntry->Pos.Y;

        float rotPlayerX = float(atEntry->Pos.X + playerBoxDistX * cosVal - playerBoxDistY*sinVal);
        float rotPlayerY = float(atEntry->Pos.Y + playerBoxDistY * cosVal + playerBoxDistX*sinVal);

        // box edges are parallel to coordiante axis, so we can treat every dimension independently :D
        float dz = player->GetPositionZ() - atEntry->Pos.Z;
        float dx = rotPlayerX - atEntry->Pos.X;
        float dy = rotPlayerY - atEntry->Pos.Y;
        if ((fabs(dx) > atEntry->BoxLength / 2 + delta) ||
            (fabs(dy) > atEntry->BoxWidth / 2 + delta) ||
            (fabs(dz) > atEntry->BoxHeight / 2 + delta))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTrigger: Player '%s' (GUID: %u) too far (1/2 box X: %f 1/2 box Y: %f 1/2 box Z: %f rotatedPlayerX: %f rotatedPlayerY: %f dZ:%f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUIDLow(), atEntry->BoxLength / 2, atEntry->BoxWidth / 2, atEntry->BoxHeight / 2, rotPlayerX, rotPlayerY, dz, packet.AreaTriggerID);
            return;
        }
    }

    if (player->isDebugAreaTriggers)
        ChatHandler(player).PSendSysMessage(LANG_DEBUG_AREATRIGGER_REACHED, packet.AreaTriggerID);

    // set for future scrip using.
    player->SetLastAreaTrigger(atEntry);

    if (sScriptMgr->OnAreaTrigger(player, atEntry, packet.Entered))
        return;

    if (player->isAlive())
    {
        if (std::unordered_set<uint32> const* quests = sObjectMgr->GetQuestsForAreaTrigger(packet.AreaTriggerID))
        {
            for (uint32 questId : *quests)
            {
                Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
                if (qInfo && player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                {
                    for (QuestObjective const& obj : qInfo->GetObjectives())
                    {
                        if (obj.Type != QUEST_OBJECTIVE_AREATRIGGER)
                            continue;

                        player->SetQuestObjectiveData(qInfo, &obj, int32(true));
                        break;
                    }

                    player->AreaExploredOrEventHappens(questId);
                }
            }
        }
    }  

    if (sObjectMgr->IsTavernAreaTrigger(packet.AreaTriggerID))
    {
        // set resting flag we are in the inn
        player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
        player->InnEnter(time(NULL), atEntry->MapID, atEntry->Pos.X, atEntry->Pos.Y, atEntry->Pos.Z);
        player->SetRestType(REST_TYPE_IN_TAVERN);

        if (sWorld->IsFFAPvPRealm())
            player->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);

        return;
    }

    if (Battleground* bg = player->GetBattleground())
        bg->HandleAreaTrigger(player, packet.AreaTriggerID, packet.Entered);

    if (OutdoorPvP* pvp = player->GetOutdoorPvP())
        pvp->HandleAreaTrigger(_player, packet.AreaTriggerID, packet.Entered);

    AreaTriggerStruct const* at = sObjectMgr->GetAreaTrigger(packet.AreaTriggerID);
    if (!at)
        return;

    bool teleported = false;
    if (player->GetMapId() != at->target_mapId)
    {
        if (!sMapMgr->CanPlayerEnter(at->target_mapId, player, false))
            return;

        if (Group* group = player->GetGroup())
            if (group->isLFGGroup() && player->GetMap()->IsDungeon())
                teleported = player->TeleportToBGEntryPoint();

        MapEntry const* targetMap = sMapStore.LookupEntry(at->target_mapId);
        if (targetMap->IsDungeon() && player->GetGroup())
        {
            if (uint32 dungeonId = sLFGMgr->GetDungeon(player->GetGroup()->GetGUID(), true))
                if (lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(dungeonId))
                    if (dungeon->map == targetMap->ID)
                        teleported = player->TeleportTo(dungeon->map, dungeon->x, dungeon->y, dungeon->z, dungeon->o, TELE_TO_NOT_LEAVE_TRANSPORT);
        }
    }

    if (!teleported)
        player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation, TELE_TO_NOT_LEAVE_TRANSPORT);
}

void WorldSession::HandleCompleteCinematic(WorldPackets::Misc::CompleteCinematic& /*packet*/) { }

void WorldSession::HandleNextCinematicCamera(WorldPackets::Misc::NextCinematicCamera& /*packet*/) { }

void WorldSession::HandleCompleteMovie(WorldPackets::Misc::CompleteMovie& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    player->setWatchinMovie(false);
    player->SetCanDelayTeleport(false);
}

void WorldSession::HandleFarSight(WorldPackets::Misc::FarSight& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!packet.Enable)
        player->SetSeer(player);
    else
        if (WorldObject* target = player->GetViewpoint())
            player->SetSeer(target);

    player->UpdateVisibilityForPlayer();
}

void WorldSession::HandleTimeSyncResp(WorldPackets::Misc::TimeSyncResponse& packet)
{
    // Prevent crashing server if queue is empty
    if (_player->m_timeSyncQueue.empty())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Received CMSG_TIME_SYNC_RESPONSE from player %s without requesting it (hacker?)", _player->GetName());
        return;
    }

    if (packet.SequenceIndex != _player->m_timeSyncQueue.front())
        sLog->outError(LOG_FILTER_NETWORKIO, "Wrong time sync counter from player %s (cheater?)", _player->GetName());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Time sync received: counter %u, client ticks %u, time since last sync %u", packet.SequenceIndex, packet.ClientTime, packet.ClientTime - _player->m_timeSyncClient);

    uint32 ourTicks = packet.ClientTime + (getMSTime() - _player->m_timeSyncServer);

    // diff should be small
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Our ticks: %u, diff %u, latency %u", ourTicks, ourTicks - packet.ClientTime, GetLatency());

    _player->m_timeSyncClient = packet.ClientTime;
    _player->m_timeSyncQueue.pop();
}

void WorldSession::HandleResetInstances(WorldPackets::Instance::ResetInstances& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (Group* group = player->GetGroup())
    {
        if (group->IsLeader(player->GetGUID()))
            group->ResetInstances(INSTANCE_RESET_ALL, false, false, player);
    }
    else
        player->ResetInstances(INSTANCE_RESET_ALL, false, false);
}

void WorldSession::HandleSetDungeonDifficulty(WorldPackets::Misc::SetDungeonDifficulty& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(packet.DifficultyID);
    if (!difficultyEntry)
        return;

    if (difficultyEntry->InstanceType != MAP_INSTANCE)
        return;

    if (!(difficultyEntry->Flags & DIFFICULTY_FLAG_CAN_SELECT))
        return;

    if (Difficulty(packet.DifficultyID) == player->GetDungeonDifficultyID())
        return;

    Map* map = player->FindMap();
    if (map && map->IsDungeon())
        return;

    Group* group = player->GetGroup();
    if (group)
    {
        if (group->IsLeader(player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->getSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsNonRaidDungeon())
                    return;
            }

            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, false, player);
            group->SetDungeonDifficultyID(Difficulty(packet.DifficultyID));
            player->SendDungeonDifficulty();
            group->SendUpdate();
        }
    }
    else
    {
        player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, false);
        player->SetDungeonDifficultyID(Difficulty(packet.DifficultyID));
        player->SendDungeonDifficulty();
    }
}

void WorldSession::HandleSetRaidDifficulty(WorldPackets::Misc::SetRaidDifficulty& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(packet.DifficultyID);
    if (!difficultyEntry)
        return;

    if (difficultyEntry->InstanceType != MAP_RAID)
        return;

    if (!(difficultyEntry->Flags & DIFFICULTY_FLAG_CAN_SELECT))
        return;

    if (((difficultyEntry->Flags & DIFFICULTY_FLAG_LEGACY) >> 5) != packet.Legacy)
        return;

    Difficulty difficultyID = Difficulty(difficultyEntry->ID);
    if (difficultyID == (packet.Legacy ?  player->GetLegacyRaidDifficultyID() : player->GetRaidDifficultyID()))
        return;

    Map* map = player->FindMap();
    if (map && map->IsDungeon())
        return;

    Group* group = player->GetGroup();
    if (group)
    {
        if (group->IsLeader(player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->getSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsRaid())
                    return;
            }

            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true, packet.Legacy != 0, player);
            if (packet.Legacy)
                group->SetLegacyRaidDifficultyID(difficultyID);
            else
                group->SetRaidDifficultyID(difficultyID);
            player->SendRaidDifficulty(packet.Legacy != 0);
            group->SendUpdate();
        }
    }
    else
    {
        player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true, packet.Legacy != 0);
        if (packet.Legacy)
            player->SetLegacyRaidDifficultyID(difficultyID);
        else
            player->SetRaidDifficultyID(difficultyID);

        player->SendRaidDifficulty(packet.Legacy != 0);
    }
}

void WorldSession::HandleRequestPetInfo(WorldPackets::PetPackets::RequestPetInfo& /*packet*/) { }

void WorldSession::HandleUITimeRequest(WorldPackets::Misc::UITimeRequest& /*request*/)
{
    WorldPackets::Misc::UITime response;
    response.Time = time(NULL);
    SendPacket(response.Write());
}

void WorldSession::SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& visibleMapIDs, std::set<uint32> const& uiWorldMapAreaIDSwaps, std::set<uint32> const& preloadMapIDs, uint32 flags /*= 0x1F*/)
{
    WorldPackets::Misc::PhaseShift phaseShift;
    phaseShift.ClientGUID = _player->GetGUID();
    phaseShift.PersonalGUID = _player->GetGUID();
    phaseShift.PhaseShifts = phaseIds;
    phaseShift.PreloadMapIDs = preloadMapIDs;
    phaseShift.VisibleMapIDs = visibleMapIDs;
    phaseShift.UiWorldMapAreaIDSwaps = uiWorldMapAreaIDSwaps;
    SendPacket(phaseShift.Write());
}

void WorldSession::HandleInstanceLockResponse(WorldPackets::Instance::InstanceLockResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!player->HasPendingBind())
        return;

    if (packet.AcceptLock)
        player->BindToInstance();
    else
        player->RepopAtGraveyard();

    player->SetPendingBind(0, 0);
}

void WorldSession::HandleViolenceLevel(WorldPackets::Misc::ViolenceLevel& /*packet*/) { }

void WorldSession::HandleObjectUpdateFailed(WorldPackets::Misc::ObjectUpdateFailed& packet)
{
    if (_player->GetGUID() == packet.ObjectGUID)
    {
        LogoutPlayer(true);
        return;
    }

    _player->m_clientGUIDs.erase(packet.ObjectGUID);
}

void WorldSession::HandleObjectUpdateRescued(WorldPackets::Misc::ObjectUpdateRescued& packet)
{
    _player->m_clientGUIDs.insert(packet.ObjectGUID);
}

// DestrinyFrame.xml : lua function NeutralPlayerSelectFaction
#define JOIN_THE_ALLIANCE 1
#define JOIN_THE_HORDE    0

void WorldSession::HandleNeutralPlayerSelectFaction(WorldPackets::Character::NeutralPlayerSelectFaction& packet)
{
    if (_player->getRace() != RACE_PANDAREN_NEUTRAL)
        return;

    if (packet.Faction == JOIN_THE_HORDE)
    {
        _player->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_HORDE);
        _player->setFactionForRace(RACE_PANDAREN_HORDE);
        _player->SaveToDB();
        WorldLocation location(1, 1349.72f, -4374.50f, 26.15f, M_PI);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 363);
        _player->learnSpell(669, false); // Language Orcish
        _player->learnSpell(108127, false); // Language Pandaren
    }
    else if (packet.Faction == JOIN_THE_ALLIANCE)
    {
        _player->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_ALLIANCE);
        _player->setFactionForRace(RACE_PANDAREN_ALLIANCE);
        _player->SaveToDB();
        WorldLocation location(0, -9076.77f, 424.74f, 92.42f, M_PI);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 9);
        _player->learnSpell(668, false); // Language Common
        _player->learnSpell(108127, false); // Language Pandaren
    }

    //if (_player->GetQuestStatus(31450) == QUEST_STATUS_INCOMPLETE)
        //_player->KilledMonsterCredit(64594);

    _player->CompleteQuest(31450);
    _player->SendMovieStart(116);
}

void WorldSession::HandleRequestCemeteryList(WorldPackets::Misc::RequestCemeteryList& /*packet*/)
{
    uint32 team = _player->GetTeam();

    std::vector<uint32> graveyardIds;
    auto range = sObjectMgr->GraveYardStore.equal_range(_player->GetZoneId());

    for (auto it = range.first; it != range.second && graveyardIds.size() < 16; ++it) // client max
    {
        if (it->second.team == 0 || it->second.team == team)
            graveyardIds.push_back(it->first);
    }

    if (graveyardIds.empty())
        return;

    WorldPackets::Misc::RequestCemeteryListResponse packet;
    packet.IsGossipTriggered = false;
    packet.CemeteryID.reserve(graveyardIds.size());

    for (uint32 const& id : graveyardIds)
        packet.CemeteryID.push_back(id);

    SendPacket(packet.Write());
}

void WorldSession::SuspendTokenResponse(WorldPackets::Auth::SuspendTokenResponse& packet)
{
    /*
    doesnt seems like we should send it like response... this smsg requesting SuspendTokenResponse
    WorldPackets::Auth::ResumeToken resume;
    resume.Sequence = packet.Sequence;
    resume.Reason = 2;
    SendPacket(resume.Write());*/
}

void WorldSession::HandleForcedReactions(WorldPackets::Reputation::RequestForcedReactions& /*packet*/)
{
    _player->GetReputationMgr().SendForceReactions();
}

void WorldSession::HandleWarGameStart(WorldPacket& recvPacket) { }

void WorldSession::HandleWarGameAccept(WorldPacket& recvPacket) { }

void WorldSession::HandleMountSpecialAnim(WorldPackets::Misc::MountSpecialAnim& /*packet*/)
{
    _player->SendMessageToSet(WorldPackets::Misc::SpecialMountAnim(_player->GetGUID()).Write(), false);
}

void WorldSession::HandleSummonResponse(WorldPackets::Movement::SummonResponse& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!player->isAlive() || player->isInCombat())
        return;

    player->SummonIfPossible(packet.Accept);
}

void WorldSession::HandleRandomRollClient(WorldPackets::Misc::RandomRollClient& packet)
{
    Player* player = GetPlayer();
    if (packet.Min > packet.Max || packet.Max > 10000 || !player)
        return;

    WorldPackets::Misc::RandomRoll randomRoll;
    randomRoll.Min = packet.Min;
    randomRoll.Max = packet.Max;
    randomRoll.Result = urand(packet.Min, packet.Max);
    randomRoll.Roller = player->GetGUID();
    randomRoll.RollerWowAccount = GetBattlenetAccountGUID();
    if (player->GetGroup())
        player->GetGroup()->BroadcastPacket(randomRoll.Write(), false);
    else
        SendPacket(randomRoll.Write());
}

void WorldSession::HandleOpeningCinematic(WorldPackets::Misc::OpeningCinematic& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (player->GetUInt32Value(PLAYER_FIELD_XP))
        return;

    if (ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(player->getClass()))
    {
        if (classEntry->CinematicSequenceID)
            player->SendCinematicStart(classEntry->CinematicSequenceID);
        else if (ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(player->getRace()))
            player->SendCinematicStart(raceEntry->CinematicSequenceID);
    }
}

void WorldSession::HandleWorldTeleport(WorldPackets::Misc::WorldTeleport& packet)
{
    if (GetPlayer()->isInFlight())
        return;

    if (AccountMgr::IsAdminAccount(GetSecurity()))
        GetPlayer()->TeleportTo(packet.MapID, packet.Pos.x, packet.Pos.y, packet.Pos.z, packet.Facing);
    else
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
}

void WorldSession::HandleRequestResearchHistory(WorldPackets::Misc::RequestResearchHistory& /*packet*/)
{
    _player->SendCompletedProjects();
}

void WorldSession::HandleChoiceResponse(WorldPackets::Misc::ChoiceResponse& packet)
{
    std::vector<DisplayChoiceData> const* displayData = sObjectMgr->GetDisplayChoiceData(packet.ChoiceID);
    if (displayData && !displayData->empty())
    {
        for (std::vector<DisplayChoiceData>::const_iterator itr = displayData->begin(); itr != displayData->end(); ++itr)
        {
            if (itr->ResponseID == packet.ResponseID && itr->SpellID)
                _player->CastSpell(_player, itr->SpellID, true);
        }
    }
}

