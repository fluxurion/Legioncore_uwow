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

#include "InstanceScript.h"
#include "DatabaseEnv.h"
#include "Map.h"
#include "Player.h"
#include "GameObject.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Log.h"
#include "LFGMgr.h"
#include "ChallengeMgr.h"
#include "Group.h"
#include "ScenarioMgr.h"
#include "InstanceSaveMgr.h"
#include "Packets/InstancePackets.h"

#define CHALLENGE_START 5

enum events
{
    EVENT_START_CHALLENGE = 1,
    EVENT_SAVE_CHALLENGE  = 2,
    EVENT_CHALLENGE_STOP  = 3,
    EVENT_CONTINUE_CHALLENGE
};

void InstanceScript::SaveToDB()
{
    std::string data = GetSaveData();
    if (data.empty())
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_INSTANCE_DATA);
    stmt->setUInt32(0, GetCompletedEncounterMask());
    stmt->setUInt32(1, GetChallengeProgresTime());
    stmt->setString(2, data);
    stmt->setUInt32(3, instance->GetInstanceId());
    CharacterDatabase.Execute(stmt);

    if(InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(instance->GetInstanceId()))
        save->SetCompletedEncountersMask(GetCompletedEncounterMask());
    if(InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(instance->GetInstanceId()))
        save->SetCompletedEncountersMask(GetCompletedEncounterMask());
}

void InstanceScript::HandleGameObject(ObjectGuid GUID, bool open, GameObject* go)
{
    if (!go)
        go = instance->GetGameObject(GUID);
    if (go)
        go->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
    else
        sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript: HandleGameObject failed");
}

bool InstanceScript::IsEncounterInProgress() const
{
    for (std::vector<BossInfo>::const_iterator itr = bosses.begin(); itr != bosses.end(); ++itr)
        if (itr->state == IN_PROGRESS)
            return true;

    const_cast<InstanceScript*>(this)->ResurectCount = 0;
    return false;
}

void InstanceScript::LoadMinionData(const MinionData* data)
{
    while (data->entry)
    {
        if (data->bossId < bosses.size())
            minions.insert(std::make_pair(data->entry, MinionInfo(&bosses[data->bossId])));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadMinionData: " UI64FMTD " minions loaded.", uint64(minions.size()));
}

void InstanceScript::LoadDoorData(const DoorData* data)
{
    while (data->entry)
    {
        if (data->bossId < bosses.size())
            doors.insert(std::make_pair(data->entry, DoorInfo(&bosses[data->bossId], data->type, BoundaryType(data->boundary))));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadDoorData: " UI64FMTD " doors loaded.", uint64(doors.size()));
}

void InstanceScript::LoadDoorDataBase(std::vector<DoorData> const* data)
{
    for (std::vector<DoorData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
    {
        if (itr->bossId < bosses.size())
            doors.insert(std::make_pair(itr->entry, DoorInfo(&bosses[itr->bossId], itr->type, BoundaryType(itr->boundary))));
        sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadDoorDataBase data->entry %u, data->bossId %u, bosses.size() %u, data->type %u, data->boundary %u", itr->entry, itr->bossId, bosses.size(), itr->type, itr->boundary);
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadDoorDataBase: " UI64FMTD " doors loaded.", uint64(doors.size()));
}

void InstanceScript::UpdateMinionState(Creature* minion, EncounterState state)
{
    switch (state)
    {
        case NOT_STARTED:
            if (!minion->isAlive())
                minion->Respawn();
            else if (minion->isInCombat())
                minion->AI()->EnterEvadeMode();
            break;
        case IN_PROGRESS:
            if (!minion->isAlive())
                minion->Respawn();
            else if (!minion->getVictim())
                minion->AI()->DoZoneInCombat();
            break;
        default:
            break;
    }
}

void InstanceScript::UpdateDoorState(GameObject* door)
{
    if(!this || !door)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());
    if (lower == upper)
        return;

    bool open = true;
    for (DoorInfoMap::iterator itr = lower; itr != upper && open; ++itr)
    {
        switch (itr->second.type)
        {
            case DOOR_TYPE_ROOM:
                open = (itr->second.bossInfo->state != IN_PROGRESS);
                break;
            case DOOR_TYPE_PASSAGE:
                open = (itr->second.bossInfo->state == DONE);
                break;
            case DOOR_TYPE_SPAWN_HOLE:
                open = (itr->second.bossInfo->state == IN_PROGRESS);
                break;
            default:
                break;
        }
    }

    door->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
}

void InstanceScript::AddDoor(GameObject* door, bool add)
{
    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());
    if (lower == upper)
        return;

    for (DoorInfoMap::iterator itr = lower; itr != upper; ++itr)
    {
        DoorInfo const& data = itr->second;

        if (add)
        {
            door->setActive(true);
            data.bossInfo->door[data.type].insert(door);
            switch (data.boundary)
            {
                default:
                case BOUNDARY_NONE:
                    break;
                case BOUNDARY_N:
                case BOUNDARY_S:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX();
                    break;
                case BOUNDARY_E:
                case BOUNDARY_W:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionY();
                    break;
                case BOUNDARY_NW:
                case BOUNDARY_SE:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() + door->GetPositionY();
                    break;
                case BOUNDARY_NE:
                case BOUNDARY_SW:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() - door->GetPositionY();
                    break;
            }
        }
        else
            data.bossInfo->door[data.type].erase(door);
    }

    if (add)
        UpdateDoorState(door);
}

void InstanceScript::AddMinion(Creature* minion, bool add)
{
    MinionInfoMap::iterator itr = minions.find(minion->GetEntry());
    if (itr == minions.end())
        return;

    if (add)
        itr->second.bossInfo->minion.insert(minion);
    else
        itr->second.bossInfo->minion.erase(minion);
}

bool InstanceScript::SetBossState(uint32 id, EncounterState state)
{
    if (id < bosses.size())
    {
        BossInfo* bossInfo = &bosses[id];
        if (bossInfo->state == TO_BE_DECIDED) // loading
        {
            bossInfo->state = state;
            //sLog->outError(LOG_FILTER_GENERAL, "Inialize boss %u state as %u.", id, (uint32)state);
            return false;
        }
        else
        {
            if (bossInfo->state == state)
                return false;

            if (state == DONE)
                for (MinionSet::iterator i = bossInfo->minion.begin(); i != bossInfo->minion.end(); ++i)
                    if ((*i)->isWorldBoss() && (*i)->isAlive())
                        return false;

            ResurectCount = 0;
            bossInfo->state = state;
            SaveToDB();
        }

        for (uint32 type = 0; type < MAX_DOOR_TYPES; ++type)
            for (DoorSet::iterator i = bossInfo->door[type].begin(); i != bossInfo->door[type].end(); ++i)
                UpdateDoorState(*i);

        for (MinionSet::iterator i = bossInfo->minion.begin(); i != bossInfo->minion.end(); ++i)
            UpdateMinionState(*i, state);

        return true;
    }
    return false;
}

std::string InstanceScript::LoadBossState(const char * data)
{
    if (!data)
        return NULL;
    std::istringstream loadStream(data);
    uint32 buff;
    uint32 bossId = 0;
    for (std::vector<BossInfo>::iterator i = bosses.begin(); i != bosses.end(); ++i, ++bossId)
    {
        loadStream >> buff;
        if (buff < TO_BE_DECIDED)
            SetBossState(bossId, (EncounterState)buff);
    }
    return loadStream.str();
}

std::string InstanceScript::GetBossSaveData()
{
    std::ostringstream saveStream;
    for (std::vector<BossInfo>::iterator i = bosses.begin(); i != bosses.end(); ++i)
        saveStream << (uint32)i->state << ' ';
    return saveStream.str();
}

void InstanceScript::DoUseDoorOrButton(ObjectGuid uiGuid, uint32 uiWithRestoreTime, bool bUseAlternativeState)
{
    if (uiGuid.IsEmpty())
        return;

    GameObject* go = instance->GetGameObject(uiGuid);

    if (go)
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR || go->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (go->getLootState() == GO_READY)
                go->UseDoorOrButton(uiWithRestoreTime, bUseAlternativeState);
            else if (go->getLootState() == GO_ACTIVATED)
                go->ResetDoorOrButton();
        }
        else
            sLog->outError(LOG_FILTER_GENERAL, "SD2: Script call DoUseDoorOrButton, but gameobject entry %u is type %u.", go->GetEntry(), go->GetGoType());
    }
}

void InstanceScript::DoRespawnGameObject(ObjectGuid uiGuid, uint32 uiTimeToDespawn)
{
    if (GameObject* go = instance->GetGameObject(uiGuid))
    {
        //not expect any of these should ever be handled
        if (go->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE || go->GetGoType() == GAMEOBJECT_TYPE_DOOR ||
            go->GetGoType() == GAMEOBJECT_TYPE_BUTTON || go->GetGoType() == GAMEOBJECT_TYPE_TRAP)
            return;

        if (go->isSpawned())
            return;

        go->SetRespawnTime(uiTimeToDespawn);
    }
}

void InstanceScript::DoUpdateWorldState(WorldStates variableID, uint32 value)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            if (Player* player = itr->getSource())
                player->SendUpdateWorldState(variableID, value);
    }
    else
        sLog->outDebug(LOG_FILTER_TSCR, "DoUpdateWorldState attempt send data but no players in map.");
}

// Send Notify to all players in instance
void InstanceScript::DoSendNotifyToInstance(char const* format, ...)
{
    InstanceMap::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        va_list ap;
        va_start(ap, format);
        char buff[1024];
        vsnprintf(buff, 1024, format, ap);
        va_end(ap);
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            if (Player* player = i->getSource())
                if (WorldSession* session = player->GetSession())
                    session->SendNotification("%s", buff);
    }
}

// Reset Achievement Criteria for all players in instance
void InstanceScript::DoResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 /*= 0*/, uint64 miscValue2 /*= 0*/, bool evenIfCriteriaComplete /*= false*/)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().ResetAchievementCriteria(type, miscValue1, miscValue2, evenIfCriteriaComplete);
}

// Complete Achievement for all players in instance
void InstanceScript::DoCompleteAchievement(uint32 achievement)
{
    AchievementEntry const* pAE = sAchievementStore.LookupEntry(achievement);
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!pAE)
        return;

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player *pPlayer = i->getSource())
                pPlayer->CompletedAchievement(pAE);
}

// Update Achievement Criteria for all players in instance
void InstanceScript::DoUpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, uint32 miscValue3 /*= 0*/, Unit* unit /*= NULL*/)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3, unit);
}

// Start timed achievement for all players in instance
void InstanceScript::DoStartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().StartTimedAchievement(type, entry);
}

// Stop timed achievement for all players in instance
void InstanceScript::DoStopTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().RemoveTimedAchievement(type, entry);
}

// Remove Auras due to Spell on all players in instance
void InstanceScript::DoRemoveAurasDueToSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    if (!PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            if (Player* player = itr->getSource())
            {
                player->RemoveAurasDueToSpell(spell);
                if (Pet* pet = player->GetPet())
                    pet->RemoveAurasDueToSpell(spell);
            }
        }
    }
}

// Remove aura from stack on all players in instance
void InstanceScript::DoRemoveAuraFromStackOnPlayers(uint32 spell, ObjectGuid const& casterGUID, AuraRemoveMode mode, uint32 num)
{
    Map::PlayerList const& plrList = instance->GetPlayers();
    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator itr = plrList.begin(); itr != plrList.end(); ++itr)
            if (Player* pPlayer = itr->getSource())
                pPlayer->RemoveAuraFromStack(spell, casterGUID, mode, num);
}

void InstanceScript::DoNearTeleportPlayers(const Position pos, bool casting /*=false*/)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), casting);
}

void InstanceScript::DoStartMovie(uint32 movieId)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->SendMovieStart(movieId);

}

// Cast spell on all players in instance
void InstanceScript::DoCastSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->CastSpell(player, spell, true);
}

void InstanceScript::DoSetAlternatePowerOnPlayers(int32 value)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->SetPower(POWER_ALTERNATE_POWER, value);
}

// Add aura on all players in instance
void InstanceScript::DoAddAuraOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->AddAura(spell, player);
}

bool InstanceScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/ /*= NULL*/, uint32 /*miscvalue1*/ /*= 0*/)
{
    sLog->outError(LOG_FILTER_GENERAL, "Achievement system call InstanceScript::CheckAchievementCriteriaMeet but instance script for map %u not have implementation for achievement criteria %u",
        instance->GetId(), criteria_id);
    return false;
}

void InstanceScript::SendEncounterUnit(uint32 type, Unit* unit /*= NULL*/, uint8 param1 /*= 0*/, uint8 param2 /*= 0*/)
{
    switch (type)
    {
        case ENCOUNTER_FRAME_INSTANCE_START:
        {
            if (!unit)
                return;

            if (DungeonEncounterEntry const* dungeon = GetDungeonEncounterByDisplayID(unit->GetNativeDisplayId()))
            {
                WorldPackets::Instance::EncounterStart start;
                start.EncounterID = dungeon->id;
                start.DifficultyID = unit->GetSpawnMode();
                start.GroupSize = instance->GetMaxPlayer();
                instance->SendToPlayers(start.Write());
            }

            WorldPackets::Instance::InstanceEncounterStart eStart;
            eStart.InCombatResCount = ResurectCount;
            eStart.MaxInCombatResCount = instance->Is25ManRaid() ? 3 : 1;
            eStart.CombatResChargeRecovery = 216000;
            eStart.NextCombatResChargeTime = 216000;
            instance->SendToPlayers(eStart.Write());
            break;
        }
        case ENCOUNTER_FRAME_INSTANCE_END:
        {
            if (!unit)
                return;

            if (DungeonEncounterEntry const* dungeon = GetDungeonEncounterByDisplayID(unit->GetNativeDisplayId()))
            {
                WorldPackets::Instance::EncounterEnd end;
                end.EncounterID = dungeon->id;
                end.DifficultyID = unit->GetSpawnMode();
                end.GroupSize = instance->GetMaxPlayer();
                end.Success = true;
                instance->SendToPlayers(end.Write());
            }

            instance->SendToPlayers(WorldPackets::Instance::NullSmsg(SMSG_INSTANCE_ENCOUNTER_END).Write());
            break;
        }
        case ENCOUNTER_FRAME_ENGAGE:
        {
            if (!unit)
                return;

            WorldPackets::Instance::InstanceEncounterEngageUnit engageUnit;
            engageUnit.Unit = unit->GetGUID();
            engageUnit.TargetFramePriority = param1;
            instance->SendToPlayers(engageUnit.Write());
            break;
        }
        case ENCOUNTER_FRAME_DISENGAGE:
        {
            if (!unit)
                return;

            WorldPackets::Instance::InstanceEncounterDisengageUnit disengageUnit;
            disengageUnit.Unit = unit->GetGUID();
            instance->SendToPlayers(disengageUnit.Write());
            break;
        }
        case ENCOUNTER_FRAME_UPDATE_PRIORITY:
        {
            if (!unit)
                return;

            WorldPackets::Instance::InstanceEncounterChangePriority changePriority;
            changePriority.Unit = unit->GetGUID();
            changePriority.TargetFramePriority = param1;
            instance->SendToPlayers(changePriority.Write());
            break;
        }
        /*case ENCOUNTER_FRAME_ADD_TIMER:
        case ENCOUNTER_FRAME_ENABLE_OBJECTIVE:
        case ENCOUNTER_FRAME_DISABLE_OBJECTIVE:
        case ENCOUNTER_FRAME_SET_COMBAT_RES_LIMIT:
            data << uint8(param1);
            break;
        case ENCOUNTER_FRAME_UPDATE_OBJECTIVE:
            data << uint8(param1);
            data << uint8(param2);
            break;
        case ENCOUNTER_FRAME_UNK7:*/
        default:
            break;
    }
}

bool InstanceScript::IsWipe() const
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();

    if (PlayerList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
    {
        Player* player = Itr->getSource();

        if (!player)
            continue;

        if (player->isAlive() && !player->isGameMaster())
            return false;
    }

    return true;
}

//void InstanceScript::UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* /*source*/)
/*{
    Difficulty diff = instance->GetDifficultyID();
    if (challenge_timer)
        diff = DIFFICULTY_HEROIC;

    sLog->outDebug(LOG_FILTER_LFG, "UpdateEncounterState: Instance %s (instanceId %u) diff %u. creditEntry: %u, type %u", instance->GetMapName(), instance->GetInstanceId(), diff, creditEntry, type);

    DungeonEncounterList const* encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), diff);
    if (!encounters)
        return;

    uint32 dungeonId = 0;
    uint32 fullEncounterIndex = 0;

    for (DungeonEncounterList::const_iterator itr = encounters->begin(); itr != encounters->end(); ++itr)
    {
        DungeonEncounter const* encounter = *itr;
        if (encounter->creditType == type && encounter->creditEntry == creditEntry)
        {
            completedEncounters |= 1 << encounter->dbcEntry->encounterIndex;
            if (encounter->lastEncounterDungeon)
            {
                dungeonId = encounter->lastEncounterDungeon;
                sLog->outDebug(LOG_FILTER_LFG, "UpdateEncounterState: Instance %s (instanceId %u) completed encounter %s. Credit Dungeon: %u", instance->GetMapName(), instance->GetInstanceId(), encounter->dbcEntry->encounterName, dungeonId);
                // no break need check all encounters.
                //break;
            }
        }
        fullEncounterIndex |= 1 << encounter->dbcEntry->encounterIndex;
    }

    if (dungeonId && (fullEncounterIndex == completedEncounters || instance->GetDifficultyID() != DIFFICULTY_CHALLENGE))
    // TODO: challenges should be rewarded in scenario mgr
    //if (dungeonId && !challenge_timer)
    {
        Map::PlayerList const& players = instance->GetPlayers();
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
        {
            if (Player* player = i->getSource())
                if (Group* grp = player->GetGroup())
                    if (grp->isLFGGroup())
                    {
                        sLog->outDebug(LOG_FILTER_LFG, "UpdateEncounterState: grp->GetGUID %u. Credit Dungeon: %u", grp->GetGUID(), dungeonId);
                        sLFGMgr->FinishDungeon(grp->GetGUID(), dungeonId);
                        break;
                    }
        }
    }
    if (dungeonId)
    {
        // Challenge reward
        // TODO: move this to scenario mgr
        if (uint32 time = GetChallengeProgresTime())
        {
            auto itr = sDB2Manager._mapChallengeModeEntrybyMap.find(instance->GetId());
            if (itr != sDB2Manager._mapChallengeModeEntrybyMap.end())
            {
                ChallengeMode medal = CHALLENGE_MEDAL_NONE;
                MapChallengeModeEntry const* mode = itr->second;

                // Calculate reward medal
                if (mode->gold > time)
                    medal = CHALLENGE_MEDAL_GOLD;
                else if (mode->silver > time)
                    medal = CHALLENGE_MEDAL_SILVER;
                else if (mode->bronze > time)
                    medal = CHALLENGE_MEDAL_BRONZE;
                else
                    return;

                sChallengeMgr->GroupReward(instance, getMSTime() - challenge_timer, medal);

                _events.ScheduleEvent(EVENT_CHALLENGE_STOP, 1000);
            }
        }
    }
}*/

void InstanceScript::UpdatePhasing()
{
    PhaseUpdateData phaseUdateData;
    phaseUdateData.AddConditionType(CONDITION_INSTANCE_INFO);

    Map::PlayerList const& players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->getSource())
            player->GetPhaseMgr().NotifyConditionChanged(phaseUdateData);
}

void InstanceScript::BroadcastPacket(WorldPacket const* data) const
{
    Map::PlayerList const& players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->getSource())
            player->GetSession()->SendPacket(data);
}

void InstanceScript::Update(uint32 diff)
{
    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_START_CHALLENGE:
            {
                challenge_timer = getMSTime();

                WorldPacket data(SMSG_INSTANCE_ENCOUNTER_TIMER_START, 8);
                data << uint32(WORLD_TIMER_TYPE_CHALLENGE_MODE);
                data << uint32(0);                                                          //time elapsed in sec
                BroadcastPacket(&data);

                // save challenge progress every min
                _events.ScheduleEvent(EVENT_SAVE_CHALLENGE, 60000);

                // Now spawn from heroic & hide block
                instance->SetSpawnModeBy(DIFFICULTY_HEROIC);
                break;
            }
            case EVENT_CONTINUE_CHALLENGE:
            {
                WorldPacket data(SMSG_INSTANCE_ENCOUNTER_TIMER_START, 8);
                data << uint32(WORLD_TIMER_TYPE_CHALLENGE_MODE);
                data << uint32((getMSTime() - challenge_timer)/IN_MILLISECONDS);         //time elapsed in sec
                BroadcastPacket(&data);

                // save challenge progress every min
                _events.ScheduleEvent(EVENT_SAVE_CHALLENGE, 60000);

                // Now spawn from heroic & hide block
                instance->SetSpawnModeBy(DIFFICULTY_HEROIC);
                break;
            }
            case EVENT_SAVE_CHALLENGE:
            {
                SaveToDB();
                _events.ScheduleEvent(EVENT_SAVE_CHALLENGE, 60000);
                break;
            }
            case EVENT_CHALLENGE_STOP:
            {
                _events.CancelEvent(EVENT_SAVE_CHALLENGE);

                WorldPacket data(SMSG_WORLD_STATE_TIMER_STOP, 5);
                data.WriteBit(1);
                data.FlushBits();
                data << uint32(WORLD_TIMER_TYPE_CHALLENGE_MODE);
                BroadcastPacket(&data);
                break;
            }
            default:
                break;
        }
    }
}

uint32 InstanceScript::GetChallengeProgresTime()
{
    if (!challenge_timer)
        return 0;

    return (getMSTime() - challenge_timer)/IN_MILLISECONDS;
}

void InstanceScript::SetChallengeProgresInSec(uint32 timer)
{
    if (!timer || challenge_timer)
        return;

    challenge_timer = getMSTime() - (timer*IN_MILLISECONDS);

    // start save progress event
    _events.ScheduleEvent(EVENT_SAVE_CHALLENGE, 60000);

    // Now spawn from heroic & hide block
    instance->SetSpawnModeBy(DIFFICULTY_HEROIC);
}

void InstanceScript::StartChallenge()
{
    if (instance->IsRaid() || !instance->isChallenge() || instance->GetSpawnMode() == DIFFICULTY_HEROIC)
        return;

    // Check if dungeon support challenge
    if (sDB2Manager._mapChallengeModeEntrybyMap.find(instance->GetId()) == sDB2Manager._mapChallengeModeEntrybyMap.end())
        return;

    // Set Timer For Start challenge
    _events.ScheduleEvent(EVENT_START_CHALLENGE, CHALLENGE_START * IN_MILLISECONDS);

    WorldPackets::Instance::StartTimer startTimer;
    startTimer.Type = WORLD_TIMER_TYPE_CHALLENGE_MODE;
    startTimer.TimeRemaining = Seconds(CHALLENGE_START);
    startTimer.TotalTime = Seconds(CHALLENGE_START);
    BroadcastPacket(startTimer.Write());
}

void InstanceScript::StopChallenge()
{
     _events.ScheduleEvent(EVENT_CHALLENGE_STOP, 1000);
}

void InstanceScript::FillInitialWorldTimers(WorldPacket& data)
{
    if (challenge_timer && instance->GetSpawnMode() == DIFFICULTY_HEROIC)
    {
        data << uint32(WORLD_TIMER_TYPE_CHALLENGE_MODE);
        data << uint32((getMSTime() - challenge_timer)/IN_MILLISECONDS);    //time elapsed in sec
    }
}