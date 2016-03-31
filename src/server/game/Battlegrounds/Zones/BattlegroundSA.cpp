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

#include "Battleground.h"
#include "BattlegroundSA.h"
#include "Language.h"
#include "Player.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "InstancePackets.h"

BattlegroundSA::BattlegroundSA()
{
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_SA_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SA_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_SA_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SA_HAS_BEGUN;
    BgObjects.resize(BG_SA_MAXOBJ);
    BgCreatures.resize(BG_SA_MAXNPC + BG_SA_MAX_GY);
    TimerEnabled = false;
    UpdateWaitTimer = 0;
    SignaledRoundTwo = false;
    SignaledRoundTwoHalfMin = false;
    InitSecondRound = false;

    //! This is here to prevent an uninitialised variable warning
    //! The warning only occurs when SetUpBattleGround fails though.
    //! In the future this function should be called BEFORE sending initial worldstates.
    memset(&GraveyardStatus, 0, sizeof(GraveyardStatus));
}

BattlegroundSA::~BattlegroundSA()
{
}

void BattlegroundSA::Reset()
{
    TotalTime = 0;
    Attackers = ((urand(0, 1)) ? TEAM_ALLIANCE : TEAM_HORDE);
    for (uint8 i = 0; i <= 5; i++)
        GateStatus[i] = BG_SA_GATE_OK;
    ShipsStarted = false;
    gateDestroyed = false;

    for (int8 i = TEAM_ALLIANCE; i < MAX_TEAMS; ++i)
        _notEvenAScratch[i] = true;

    Status = BG_SA_WARMUP;
}

bool BattlegroundSA::SetupBattleground()
{
    return ResetObjs();
}

void BattlegroundSA::GetTeamStartPosition(uint32 TeamID, Position& pos) const
{
    TeamId idx = GetTeamIndexByTeamId(TeamID);
    if (idx == Attackers)
    {
        if (!ShipsStarted)
        {
            switch (urand(0, 1))
            {
                case 0:
                    pos.m_positionX = 2682.936f;
                    pos.m_positionY = -830.368f;
                    pos.m_positionZ = 15.0f;
                    pos.m_orientation = 2.895f;
                    break;
                case 1:
                default:
                    pos.m_positionX = 2577.003f;
                    pos.m_positionY = 980.261f;
                    pos.m_positionZ = 15.0f;
                    pos.m_orientation = 0.807f;
                    break;
            }
        }
        else
        {
            pos.m_positionX = 1600.381f;
            pos.m_positionY = -106.263f;
            pos.m_positionZ = 8.8745f;
            pos.m_orientation = 3.78f;
        }
    }
    else
    {
        pos.m_positionX = 1209.7f;
        pos.m_positionY = -65.16f;
        pos.m_positionZ = 70.1f;
        pos.m_orientation = 0.0f;
    }
}

bool BattlegroundSA::ResetObjs()
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            SendTransportsRemove(player);

    uint32 atF = BgFactions[Attackers];
    uint32 defF = BgFactions[Attackers ? TEAM_ALLIANCE : TEAM_HORDE];

    for (uint8 i = 0; i <BG_SA_MAXOBJ; i++)
        DelObject(i);

    for (uint8 i = 0; i < BG_SA_MAXNPC; i++)
        DelCreature(i);

    for (uint8 i = BG_SA_MAXNPC; i < BG_SA_MAXNPC + BG_SA_MAX_GY; i++)
        DelCreature(i);

    for (uint8 i = 0; i < 6; i++)
        GateStatus[i] = BG_SA_GATE_OK;

    for (uint8 i = 0; i < BG_SA_BOAT_ONE; i++)
    {
        if (!AddObject(i, BG_SA_ObjEntries[i], BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1], BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3], 0, 0, 0, 0, RESPAWN_ONE_DAY))
            return false;
    }

    for (uint8 i = BG_SA_BOAT_ONE; i < BG_SA_SIGIL_1; i++)
    {
        uint32 boatid=0;
        switch (i)
        {
            case BG_SA_BOAT_ONE:
                boatid= Attackers ? BG_SA_BOAT_ONE_H : BG_SA_BOAT_ONE_A;
                break;
            case BG_SA_BOAT_TWO:
                boatid= Attackers ? BG_SA_BOAT_TWO_H : BG_SA_BOAT_TWO_A;
                break;
        }
        if (!AddObject(i, boatid, BG_SA_ObjSpawnlocs[i][0],
          BG_SA_ObjSpawnlocs[i][1],
          BG_SA_ObjSpawnlocs[i][2]+(Attackers ? -3.750f: 0),
          BG_SA_ObjSpawnlocs[i][3], 0, 0, 1.0f, -0.00000004371139f, RESPAWN_ONE_DAY))
            return false;
    }
    for (uint8 i = BG_SA_SIGIL_1; i < BG_SA_CENTRAL_FLAG; i++)
    {
        if (!AddObject(i, BG_SA_ObjEntries[i],
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY))
        return false;
    }

    // MAD props for Kiper for discovering those values - 4 hours of his work.
    SpawnBGObject(BG_SA_BOAT_ONE, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_SA_BOAT_TWO, RESPAWN_IMMEDIATELY);

    //Cannons and demolishers - NPCs are spawned
    //By capturing GYs.
    for (uint8 i = 0; i < BG_SA_DEMOLISHER_5; i++)
    {
        if (!AddCreature(BG_SA_NpcEntries[i], i, (Attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
              BG_SA_NpcSpawnlocs[i][0], BG_SA_NpcSpawnlocs[i][1],
              BG_SA_NpcSpawnlocs[i][2], BG_SA_NpcSpawnlocs[i][3], 600))
        return false;
    }

    OverrideGunFaction();
    DemolisherStartState(true);

    for (uint8 i = 0; i <= BG_SA_TITAN_RELIC; i++)
    {
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, defF);
    }

    GetBGObject(BG_SA_TITAN_RELIC)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, atF);
    GetBGObject(BG_SA_TITAN_RELIC)->Refresh();

    for (uint8 i = 0; i <= 5; i++)
        GateStatus[i] = BG_SA_GATE_OK;

    TotalTime = 0;
    ShipsStarted = false;

    //Graveyards!
    for (uint8 i = 0;i < BG_SA_MAX_GY; i++)
    {
        WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_SA_GYEntries[i]);
        if (!sg)
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "SOTA: Can't find GY entry %u", BG_SA_GYEntries[i]);
            return false;
        }

        if (i == BG_SA_BEACH_GY)
        {
            GraveyardStatus[i] = Attackers;
            AddSpiritGuide(i + BG_SA_MAXNPC, sg->Loc, Attackers);
        }
        else
        {
            GraveyardStatus[i] = Attackers == TEAM_HORDE ? TEAM_ALLIANCE : TEAM_HORDE;
            if (!AddSpiritGuide(i + BG_SA_MAXNPC, sg->Loc, Attackers == TEAM_HORDE ? TEAM_ALLIANCE : TEAM_HORDE))
                sLog->outError(LOG_FILTER_BATTLEGROUND, "SOTA: couldn't spawn GY: %u", i);
        }
    }

    //GY capture points
    for (uint8 i = BG_SA_CENTRAL_FLAG; i < BG_SA_PORTAL_DEFFENDER_BLUE; i++)
    {
        AddObject(i, (BG_SA_ObjEntries[i] - (Attackers == TEAM_ALLIANCE ? 1:0)),
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, atF);
    }

    for (uint8 i = BG_SA_PORTAL_DEFFENDER_BLUE; i < BG_SA_BOMB; i++)
    {
        AddObject(i, BG_SA_ObjEntries[i],
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, defF);
    }

    for (uint8 i = BG_SA_BOMB; i < BG_SA_MAXOBJ; i++)
    {
        AddObject(i, BG_SA_ObjEntries[BG_SA_BOMB],
            BG_SA_ObjSpawnlocs[i][0], BG_SA_ObjSpawnlocs[i][1],
            BG_SA_ObjSpawnlocs[i][2], BG_SA_ObjSpawnlocs[i][3],
            0, 0, 0, 0, RESPAWN_ONE_DAY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, atF);
    }

    if (InitSecondRound)
    {
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                SendBasicWorldStateUpdate(player);

        WorldPackets::Instance::StartTimer startTimer;
        startTimer.Type = WORLD_TIMER_TYPE_PVP;
        startTimer.TimeRemaining = Minutes(1);
        startTimer.TotalTime = Minutes(1);
        SendPacketToAll(startTimer.Write());
    }

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                SendTransportInit(player);

    TeleportPlayers();
    return true;
}

void BattlegroundSA::StartShips()
{
    if (ShipsStarted)
        return;

    GetBGObject(BG_SA_BOAT_ONE)->SetGoState(GO_STATE_READY);
    GetBGObject(BG_SA_BOAT_TWO)->SetGoState(GO_STATE_READY);

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        {
            if (Player* p = ObjectAccessor::FindPlayer(itr->first))
            {
                UpdateData data(p->GetMapId());
                WorldPacket pkt;
                GetBGObject(i)->BuildValuesUpdateBlockForPlayer(&data, p);
                data.BuildPacket(&pkt);
                p->GetSession()->SendPacket(&pkt);
            }
        }
    }
    ShipsStarted = true;
}

void BattlegroundSA::PostUpdateImpl(uint32 diff)
{
    if (InitSecondRound)
    {
        if (UpdateWaitTimer < diff)
        {
            if (!SignaledRoundTwo)
            {
                SignaledRoundTwo = true;
                InitSecondRound = false;
                SendMessageToAll(LANG_BG_SA_ROUND_TWO_ONE_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            }
        }else
        {
            UpdateWaitTimer -= diff;
            return;
        }
    }
    TotalTime += diff;

    if (Status == BG_SA_WARMUP )
    {
        EndRoundTimer = BG_SA_ROUNDLENGTH;
        if (TotalTime >= BG_SA_WARMUPLENGTH)
        {
            TotalTime = 0;
            ToggleTimer();
            DemolisherStartState(false);
            Status = BG_SA_ROUND_ONE;
            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, (Attackers == TEAM_ALLIANCE)?23748:21702);
            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT2, (Attackers == TEAM_ALLIANCE)?23748:21702);
        }
        if (TotalTime >= BG_SA_BOAT_START)
            StartShips();
        return;
    }
    else if (Status == BG_SA_SECOND_WARMUP)
    {
        if (RoundScores[0].time<BG_SA_ROUNDLENGTH)
            EndRoundTimer = RoundScores[0].time;
        else
            EndRoundTimer = BG_SA_ROUNDLENGTH;

        if (TotalTime >= 60000)
        {
            SendWarningToAll(LANG_BG_SA_HAS_BEGUN);
            TotalTime = 0;
            ToggleTimer();
            DemolisherStartState(false);
            Status = BG_SA_ROUND_TWO;
            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, (Attackers == TEAM_ALLIANCE)?23748:21702);
            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT2, (Attackers == TEAM_ALLIANCE)?23748:21702);
        }
        if (TotalTime >= 30000)
        {
            if (!SignaledRoundTwoHalfMin)
            {
                SignaledRoundTwoHalfMin = true;
                SendMessageToAll(LANG_BG_SA_ROUND_TWO_START_HALF_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            }
        }
        StartShips();
        return;
    }
    else if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (Status == BG_SA_ROUND_ONE)
        {
            if (TotalTime >= BG_SA_ROUNDLENGTH)
            {
                RoundScores[0].winner = Attackers;
                RoundScores[0].time = BG_SA_ROUNDLENGTH;
                TotalTime = 0;
                Status = BG_SA_SECOND_WARMUP;
                Attackers = (Attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                UpdateWaitTimer = 5000;
                SignaledRoundTwo = false;
                SignaledRoundTwoHalfMin = false;
                InitSecondRound = true;
                ToggleTimer();
                ResetObjs();
                return;
            }
        }
        else if (Status == BG_SA_ROUND_TWO)
        {
            if (TotalTime >= EndRoundTimer)
            {
                RoundScores[1].time = BG_SA_ROUNDLENGTH;
                RoundScores[1].winner = (Attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;

                if (RoundScores[0].time == RoundScores[1].time)
                    EndBattleground(0);
                else if (RoundScores[0].time < RoundScores[1].time)
                    EndBattleground(GetTeamByTeamId(RoundScores[0].winner));
                else
                    EndBattleground(GetTeamByTeamId(RoundScores[1].winner));
                return;
            }
        }
        if (Status == BG_SA_ROUND_ONE || Status == BG_SA_ROUND_TWO)
        {
            UpdateDemolisherSpawns();
        }
    }
}

void BattlegroundSA::StartingEventCloseDoors()
{
}

void BattlegroundSA::StartingEventOpenDoors()
{
}

void BattlegroundSA::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    uint32 ally_attacks = uint32(Attackers == TEAM_ALLIANCE ? 1 : 0);
    uint32 horde_attacks = uint32(Attackers == TEAM_HORDE ? 1 : 0);

    packet.Worldstates.emplace_back(WorldStates::BG_SA_ANCIENT_GATEWS, GateStatus[BG_SA_ANCIENT_GATE]);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_YELLOW_GATEWS, GateStatus[BG_SA_YELLOW_GATE]);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_GREEN_GATEWS, GateStatus[BG_SA_GREEN_GATE]);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_BLUE_GATEWS, GateStatus[BG_SA_BLUE_GATE]);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_RED_GATEWS, GateStatus[BG_SA_RED_GATE]);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_PURPLE_GATEWS, GateStatus[BG_SA_PURPLE_GATE]);

    packet.Worldstates.emplace_back(WorldStates::BG_SA_HORDE_ATTACKS, horde_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_ALLY_ATTACKS, ally_attacks);

    //Time will be sent on first update...
    packet.Worldstates.emplace_back(WorldStates::BG_SA_RIGHT_GY_HORDE, GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_LEFT_GY_HORDE, GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_CENTER_GY_HORDE, GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_RIGHT_GY_ALLIANCE, GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_LEFT_GY_ALLIANCE, GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_CENTER_GY_ALLIANCE, GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);

    packet.Worldstates.emplace_back(WorldStates::BG_SA_HORDE_DEFENCE_TOKEN, ally_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_ALLIANCE_DEFENCE_TOKEN, horde_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_LEFT_ATT_TOKEN_HRD, horde_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_RIGHT_ATT_TOKEN_HRD, horde_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_RIGHT_ATT_TOKEN_ALL, ally_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_LEFT_ATT_TOKEN_ALL, ally_attacks);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_BONUS_TIMER, 0);

    packet.Worldstates.emplace_back(WorldStates::BG_SA_ENABLE_TIMER, TimerEnabled ? ((BG_SA_ROUNDLENGTH-TotalTime)/IN_MILLISECONDS) : 0);
    packet.Worldstates.emplace_back(WorldStates::BG_SA_TIMER, TimerEnabled);
}

void BattlegroundSA::SendBasicWorldStateUpdate(Player* player)
{
    uint32 ally_attacks = uint32(Attackers == TEAM_ALLIANCE ? 1 : 0);
    uint32 horde_attacks = uint32(Attackers == TEAM_HORDE ? 1 : 0);

    player->SendUpdateWorldState(WorldStates::BG_SA_RIGHT_GY_HORDE, GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    player->SendUpdateWorldState(WorldStates::BG_SA_LEFT_GY_HORDE, GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    player->SendUpdateWorldState(WorldStates::BG_SA_CENTER_GY_HORDE, GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);

    player->SendUpdateWorldState(WorldStates::BG_SA_RIGHT_GY_ALLIANCE, GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    player->SendUpdateWorldState(WorldStates::BG_SA_LEFT_GY_ALLIANCE, GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    player->SendUpdateWorldState(WorldStates::BG_SA_CENTER_GY_ALLIANCE, GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);

    player->SendUpdateWorldState(WorldStates::BG_SA_ALLY_ATTACKS, ally_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_HORDE_ATTACKS, horde_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_RIGHT_ATT_TOKEN_ALL, ally_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_LEFT_ATT_TOKEN_ALL, ally_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_RIGHT_ATT_TOKEN_HRD, horde_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_LEFT_ATT_TOKEN_HRD, horde_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_HORDE_DEFENCE_TOKEN, ally_attacks);
    player->SendUpdateWorldState(WorldStates::BG_SA_ALLIANCE_DEFENCE_TOKEN, horde_attacks);

    player->SendUpdateWorldState(WorldStates::BG_SA_PURPLE_GATEWS, GateStatus[BG_SA_PURPLE_GATE]);
    player->SendUpdateWorldState(WorldStates::BG_SA_RED_GATEWS, GateStatus[BG_SA_RED_GATE]);
    player->SendUpdateWorldState(WorldStates::BG_SA_BLUE_GATEWS, GateStatus[BG_SA_BLUE_GATE]);
    player->SendUpdateWorldState(WorldStates::BG_SA_GREEN_GATEWS, GateStatus[BG_SA_GREEN_GATE]);
    player->SendUpdateWorldState(WorldStates::BG_SA_YELLOW_GATEWS, GateStatus[BG_SA_YELLOW_GATE]);
    player->SendUpdateWorldState(WorldStates::BG_SA_ANCIENT_GATEWS, GateStatus[BG_SA_ANCIENT_GATE]);
}

void BattlegroundSA::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    PlayerScores[player->GetGUID()] = new BattlegroundSAScore(player->GetGUID(), player->GetBGTeamId());

    if (!ShipsStarted)
    {
        if (player->GetBGTeamId() == Attackers)
            player->CastSpell(player, 12438, true);//Without this player falls before boat loads...
    }

    SendTransportInit(player);
}

void BattlegroundSA::RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/)
{
}

void BattlegroundSA::HandleAreaTrigger(Player* player, uint32 trigger, bool entered)
{
    Battleground::HandleAreaTrigger(player, trigger, entered);
}

void BattlegroundSA::TeleportPlayers()
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
        {
            // should remove spirit of redemption
            if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
                player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

            if (!player->isAlive())
            {
                player->ResurrectPlayer(1.0f);
                player->SpawnCorpseBones();
            }

            player->RemoveAurasWithMechanic(IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK);
            player->ResetAllPowers(true);
            player->CombatStopWithPets(true);

            if (player->GetBGTeamId() == Attackers)
            {
                player->CastSpell(player, 12438, true);     //Without this player falls before boat loads...

                if (urand(0, 1))
                    player->TeleportTo(607, 2682.936f, -830.368f, 50.0f, 2.895f, 0);
                else
                    player->TeleportTo(607, 2577.003f, 980.261f, 50.0f, 0.807f, 0);
            }
            else
                player->TeleportTo(607, 1209.7f, -65.16f, 70.1f, 0.0f, 0);
        }
    }
}

void BattlegroundSA::EventPlayerDamagedGO(Player* /*player*/, GameObject* go, uint32 eventType)
{
    if (!go || !go->GetGOInfo())
        return;

    if (eventType == go->GetGOInfo()->destructibleBuilding.DamagedEvent)
    {
        uint32 i = getGateIdFromDamagedOrDestroyEventId(eventType);
        GateStatus[i] = BG_SA_GATE_DAMAGED;
        WorldStates uws = getWorldStateFromGateId(i);
        if (uws != WorldStates::WS_NONE)
            UpdateWorldState(uws, GateStatus[i]);
    }

    if (eventType == go->GetGOInfo()->destructibleBuilding.DestroyedEvent)
    {
        if (go->GetGOInfo()->destructibleBuilding.DestroyedEvent == BG_SA_EVENT_ANCIENT_GATE_DESTROYED)
            SendWarningToAll(LANG_BG_SA_CHAMBER_BREACHED);
        else
            SendWarningToAll(LANG_BG_SA_WAS_DESTROYED, go->GetGOInfo()->name.c_str());
    }

    if (eventType == go->GetGOInfo()->destructibleBuilding.DamagedEvent)
        SendWarningToAll(LANG_BG_SA_IS_UNDER_ATTACK, go->GetGOInfo()->name.c_str());
}

void BattlegroundSA::HandleKillUnit(Creature* creature, Player* killer)
{
    if (creature->GetEntry() == NPC_DEMOLISHER_SA)
    {
        UpdatePlayerScore(killer, SCORE_DESTROYED_DEMOLISHER, 1);
        _notEvenAScratch[Attackers] = false;
    }
}

/*
  You may ask what the fuck does it do?
  Prevents owner overwriting guns faction with own.
 */
void BattlegroundSA::OverrideGunFaction()
{
    if (!BgCreatures[0])
        return;

    for (uint8 i = BG_SA_GUN_1; i <= BG_SA_GUN_10;i++)
    {
        if (Creature* gun = GetBGCreature(i))
            gun->setFaction(BgFactions[Attackers? TEAM_ALLIANCE : TEAM_HORDE]);
    }

    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_4;i++)
    {
        if (Creature* dem = GetBGCreature(i))
            dem->setFaction(BgFactions[Attackers]);
    }
}

void BattlegroundSA::DemolisherStartState(bool start)
{
    if (!BgCreatures[0])
        return;

    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_4; i++)
    {
        if (Creature* dem = GetBGCreature(i))
        {
            if (start)
                dem->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            else
                dem->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }
    }
}

void BattlegroundSA::DestroyGate(Player* player, GameObject* go)
{
    uint32 i = getGateIdFromDamagedOrDestroyEventId(go->GetGOInfo()->destructibleBuilding.DestroyedEvent);
    if (!GateStatus[i])
        return;

    if (GameObject* g = GetBGObject(i))
    {
        if (g->GetGOValue()->Building.Health == 0)
        {
            GateStatus[i] = BG_SA_GATE_DESTROYED;
            WorldStates uws = getWorldStateFromGateId(i);
            if (uws != WorldStates::WS_NONE)
                UpdateWorldState(uws, GateStatus[i]);
            bool rewardHonor = true;
            gateDestroyed = true;
            switch (i)
            {
                case BG_SA_GREEN_GATE:
                    if (GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
                case BG_SA_BLUE_GATE:
                    if (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
                case BG_SA_RED_GATE:
                    if (GateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
                case BG_SA_PURPLE_GATE:
                    if (GateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
            }

            if (i < 5)
                DelObject(i+9);
            UpdatePlayerScore(player, SCORE_DESTROYED_WALL, 1);
            if (rewardHonor)
                UpdatePlayerScore(player, SCORE_BONUS_HONOR, GetBonusHonorFromKill(1));
        }
    }
}

WorldSafeLocsEntry const* BattlegroundSA::GetClosestGraveYard(Player* player)
{
    uint32 safeloc = 0;
    WorldSafeLocsEntry const* ret;
    WorldSafeLocsEntry const* closest;
    float dist, nearest;
    float x, y, z;

    player->GetPosition(x, y, z);

    if (player->GetBGTeamId() == Attackers)
        safeloc = BG_SA_GYEntries[BG_SA_BEACH_GY];
    else
        safeloc = BG_SA_GYEntries[BG_SA_DEFENDER_LAST_GY];

    closest = sWorldSafeLocsStore.LookupEntry(safeloc);
    nearest = sqrt((closest->Loc.X - x)*(closest->Loc.X - x) + (closest->Loc.Y - y)*(closest->Loc.Y - y) + (closest->Loc.Z - z)*(closest->Loc.Z - z));

    for (uint8 i = BG_SA_RIGHT_CAPTURABLE_GY; i < BG_SA_MAX_GY; i++)
    {
        if (GraveyardStatus[i] != player->GetBGTeamId())
            continue;

        ret = sWorldSafeLocsStore.LookupEntry(BG_SA_GYEntries[i]);
        dist = sqrt((ret->Loc.X - x)*(ret->Loc.X - x) + (ret->Loc.Y - y)*(ret->Loc.Y - y) + (ret->Loc.Z - z)*(ret->Loc.Z - z));
        if (dist < nearest)
        {
            closest = ret;
            nearest = dist;
        }
    }

    return closest;
}

void BattlegroundSA::EventPlayerClickedOnFlag(Player* Source, GameObject* target_obj)
{
    switch (target_obj->GetEntry())
    {
        case 191307:
        case 191308:
            if (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                CaptureGraveyard(BG_SA_LEFT_CAPTURABLE_GY, Source);
            break;
        case 191305:
        case 191306:
            if (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                CaptureGraveyard(BG_SA_RIGHT_CAPTURABLE_GY, Source);
            break;
        case 191310:
        case 191309:
            if ((GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED) && (GateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED))
                CaptureGraveyard(BG_SA_CENTRAL_CAPTURABLE_GY, Source);
            break;
        default:
            return;
    };
}

void BattlegroundSA::CaptureGraveyard(BG_SA_Graveyards i, Player* Source)
{
    if (GraveyardStatus[i] == Attackers)
        return;

    DelCreature(BG_SA_MAXNPC + i);
    GraveyardStatus[i] = Source->GetBGTeamId();
    WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_SA_GYEntries[i]);
    if (!sg)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundSA::CaptureGraveyard: non-existant GY entry: %u", BG_SA_GYEntries[i]);
        return;
    }

    AddSpiritGuide(i + BG_SA_MAXNPC, sg->Loc, GraveyardStatus[i]);
    uint32 npc = 0;
    uint32 flag = 0;

    switch (i)
    {
        case BG_SA_LEFT_CAPTURABLE_GY:
            flag = BG_SA_LEFT_FLAG;
            DelObject(flag);
            AddObject(flag, (BG_SA_ObjEntries[flag] - (Source->GetBGTeamId() == TEAM_ALLIANCE ? 0:1)),
            BG_SA_ObjSpawnlocs[flag][0], BG_SA_ObjSpawnlocs[flag][1],
            BG_SA_ObjSpawnlocs[flag][2], BG_SA_ObjSpawnlocs[flag][3], 0, 0, 0, 0, RESPAWN_ONE_DAY);

            npc = BG_SA_NPC_RIGSPARK;
            AddCreature(BG_SA_NpcEntries[npc], npc, Attackers,
              BG_SA_NpcSpawnlocs[npc][0], BG_SA_NpcSpawnlocs[npc][1],
              BG_SA_NpcSpawnlocs[npc][2], BG_SA_NpcSpawnlocs[npc][3]);
              
            for (uint8 j = BG_SA_DEMOLISHER_7; j <= BG_SA_DEMOLISHER_8; j++)
            {
                AddCreature(BG_SA_NpcEntries[j], j, (Attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
                    BG_SA_NpcSpawnlocs[j][0], BG_SA_NpcSpawnlocs[j][1],
                    BG_SA_NpcSpawnlocs[j][2], BG_SA_NpcSpawnlocs[j][3], 600);
                    
                if (Creature* dem = GetBGCreature(j))
                    dem->setFaction(BgFactions[Attackers]);
            }

            UpdateWorldState(WorldStates::BG_SA_LEFT_GY_ALLIANCE, (GraveyardStatus[i] == TEAM_ALLIANCE? 1:0));
            UpdateWorldState(WorldStates::BG_SA_LEFT_GY_HORDE, (GraveyardStatus[i] == TEAM_ALLIANCE? 0:1));
            if (Source->GetBGTeamId() == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_WEST);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_WEST);
            break;
        case BG_SA_RIGHT_CAPTURABLE_GY:
            flag = BG_SA_RIGHT_FLAG;
            DelObject(flag);
            AddObject(flag, (BG_SA_ObjEntries[flag] - (Source->GetBGTeamId() == TEAM_ALLIANCE ? 0:1)),
              BG_SA_ObjSpawnlocs[flag][0], BG_SA_ObjSpawnlocs[flag][1],
              BG_SA_ObjSpawnlocs[flag][2], BG_SA_ObjSpawnlocs[flag][3], 0, 0, 0, 0, RESPAWN_ONE_DAY);

            npc = BG_SA_NPC_SPARKLIGHT;
            AddCreature(BG_SA_NpcEntries[npc], npc, Attackers,
              BG_SA_NpcSpawnlocs[npc][0], BG_SA_NpcSpawnlocs[npc][1],
              BG_SA_NpcSpawnlocs[npc][2], BG_SA_NpcSpawnlocs[npc][3]);

            UpdateWorldState(WorldStates::BG_SA_RIGHT_GY_ALLIANCE, (GraveyardStatus[i] == TEAM_ALLIANCE? 1:0));
            UpdateWorldState(WorldStates::BG_SA_RIGHT_GY_HORDE, (GraveyardStatus[i] == TEAM_ALLIANCE? 0:1));
            if (Source->GetBGTeamId() == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_EAST);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_EAST);
            break;
        case BG_SA_CENTRAL_CAPTURABLE_GY:
            flag = BG_SA_CENTRAL_FLAG;
            DelObject(flag);
            AddObject(flag, (BG_SA_ObjEntries[flag] - (Source->GetBGTeamId() == TEAM_ALLIANCE ? 0:1)),
              BG_SA_ObjSpawnlocs[flag][0], BG_SA_ObjSpawnlocs[flag][1],
              BG_SA_ObjSpawnlocs[flag][2], BG_SA_ObjSpawnlocs[flag][3], 0, 0, 0, 0, RESPAWN_ONE_DAY);

            UpdateWorldState(WorldStates::BG_SA_CENTER_GY_ALLIANCE, (GraveyardStatus[i] == TEAM_ALLIANCE? 1:0));
            UpdateWorldState(WorldStates::BG_SA_CENTER_GY_HORDE, (GraveyardStatus[i] == TEAM_ALLIANCE? 0:1));
            if (Source->GetBGTeamId() == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_SOUTH);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_SOUTH);
            break;
        default:
            ASSERT(false);
            break;
    };
}

void BattlegroundSA::EventPlayerUsedGO(Player* Source, GameObject* object)
{
    if (object->GetEntry() == BG_SA_ObjEntries[BG_SA_TITAN_RELIC] && GateStatus[BG_SA_ANCIENT_GATE] == BG_SA_GATE_DESTROYED && GateStatus[BG_SA_YELLOW_GATE] == BG_SA_GATE_DESTROYED && (GateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED) && (GateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || GateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED))
    {
        if (Source->GetBGTeamId() == Attackers)
        {
            if (Source->GetBGTeamId() == TEAM_ALLIANCE)
                SendMessageToAll(LANG_BG_SA_ALLIANCE_CAPTURED_RELIC, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            else SendMessageToAll(LANG_BG_SA_HORDE_CAPTURED_RELIC, CHAT_MSG_BG_SYSTEM_NEUTRAL);

            if (Status == BG_SA_ROUND_ONE)
            {
                RoundScores[0].winner = Attackers;
                RoundScores[0].time = TotalTime;
                //Achievement Storm the Beach (1310)
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetBGTeamId() == Attackers)
                            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 65246);
                }

                Attackers = (Attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                Status = BG_SA_SECOND_WARMUP;
                TotalTime = 0;
                ToggleTimer();
                SendWarningToAll(LANG_BG_SA_ROUND_ONE_END);
                UpdateWaitTimer = 5000;
                SignaledRoundTwo = false;
                SignaledRoundTwoHalfMin = false;
                InitSecondRound = true;
                ResetObjs();
            }
            else if (Status == BG_SA_ROUND_TWO)
            {
                RoundScores[1].winner = Attackers;
                RoundScores[1].time = TotalTime;
                ToggleTimer();
                //Achievement Storm the Beach (1310)
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetBGTeamId() == Attackers && RoundScores[1].winner == Attackers)
                            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 65246);
                }

                if (RoundScores[0].time == RoundScores[1].time)
                    EndBattleground(0);
                else if (RoundScores[0].time < RoundScores[1].time)
                    EndBattleground(GetTeamByTeamId(RoundScores[0].winner));
                else
                    EndBattleground(GetTeamByTeamId(RoundScores[1].winner));
            }
        }
    }
}

void BattlegroundSA::ToggleTimer()
{
    TimerEnabled = !TimerEnabled;

    UpdateWorldState(WorldStates::BG_SA_TIMER, TimerEnabled ? (time(nullptr) + 600) : 0);
    UpdateWorldState(WorldStates::BG_SA_ENABLE_TIMER, (TimerEnabled) ? 1 : 0);
}

void BattlegroundSA::EndBattleground(uint32 winner)
{
    //honor reward for winning
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    else if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);

    //complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(2), ALLIANCE);
    RewardHonorToTeam(GetBonusHonorFromKill(2), HORDE);

    Battleground::EndBattleground(winner);
}

void BattlegroundSA::UpdateDemolisherSpawns()
{
    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_8; i++)
    {
        if (!BgCreatures[i].IsEmpty())
        {
            if (Creature* Demolisher = GetBGCreature(i))
            {
                if (Demolisher->isDead())
                {
                    // Demolisher is not in list
                    if (DemoliserRespawnList.find(i) == DemoliserRespawnList.end())
                    {
                        DemoliserRespawnList[i] = getMSTime()+30000;
                    }
                    else
                    {
                        if (DemoliserRespawnList[i] < getMSTime())
                        {
                            Demolisher->Relocate(BG_SA_NpcSpawnlocs[i][0], BG_SA_NpcSpawnlocs[i][1],
                                BG_SA_NpcSpawnlocs[i][2], BG_SA_NpcSpawnlocs[i][3]);

                            Demolisher->Respawn();
                            DemoliserRespawnList.erase(i);
                        }
                    }
                }
            }
        }
    }
}

void BattlegroundSA::SendTransportInit(Player* player)
{
    if (!BgObjects[BG_SA_BOAT_ONE].IsEmpty() ||  !BgObjects[BG_SA_BOAT_TWO].IsEmpty())
    {
        UpdateData transData(player->GetMapId());
        if (!BgObjects[BG_SA_BOAT_ONE].IsEmpty())
            GetBGObject(BG_SA_BOAT_ONE)->BuildCreateUpdateBlockForPlayer(&transData, player);
        if (!BgObjects[BG_SA_BOAT_TWO].IsEmpty())
            GetBGObject(BG_SA_BOAT_TWO)->BuildCreateUpdateBlockForPlayer(&transData, player);
        WorldPacket packet;
        transData.BuildPacket(&packet);
        player->GetSession()->SendPacket(&packet);
    }
}

void BattlegroundSA::SendTransportsRemove(Player* player)
{
    if (!BgObjects[BG_SA_BOAT_ONE].IsEmpty() || !BgObjects[BG_SA_BOAT_TWO].IsEmpty())
    {
        UpdateData transData(player->GetMapId());
        if (!BgObjects[BG_SA_BOAT_ONE].IsEmpty())
            GetBGObject(BG_SA_BOAT_ONE)->BuildOutOfRangeUpdateBlock(&transData);
        if (!BgObjects[BG_SA_BOAT_TWO].IsEmpty())
            GetBGObject(BG_SA_BOAT_TWO)->BuildOutOfRangeUpdateBlock(&transData);
        WorldPacket packet;
        transData.BuildPacket(&packet);
        player->GetSession()->SendPacket(&packet);
    }
}

