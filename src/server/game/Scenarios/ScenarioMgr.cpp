/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#include "DBCStores.h"
#include "Group.h"
#include "ScenarioMgr.h"
#include "LFGMgr.h"
#include "InstanceSaveMgr.h"
#include "WorldSession.h"
#include "ScenarioPackets.h"

ScenarioProgress::ScenarioProgress(uint32 _instanceId, lfg::LFGDungeonData const* _dungeonData)
    : instanceId(_instanceId), dungeonData(_dungeonData),
    m_achievementMgr(this), currentStep(0), currentTree(0), bonusRewarded(false), rewarded(false)
{
    if (dungeonData->dbc->ScenarioID)
        scenarioId = dungeonData->dbc->ScenarioID;
    else
    {
        ScenarioData const* scenarioData = sObjectMgr->GetScenarioOnMap(dungeonData->map);
        if (scenarioData)
            scenarioId = scenarioData->ScenarioID;

        //sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "ScenarioProgress::ScenarioProgress _instanceId %u scenarioId %u dungeonData %u scenarioData %u", _instanceId, scenarioId, dungeonData->map, scenarioData);
    }

    type = ScenarioMgr::GetScenarioType(scenarioId);
    ScenarioSteps const* _steps = sScenarioMgr->GetScenarioSteps(scenarioId);
    ASSERT(_steps);

    steps = *_steps;
    currentTree = GetScenarioCriteriaByStep(currentStep);
    ActiveSteps.push_back(steps[currentStep]->ID);
}

Map* ScenarioProgress::GetMap()
{
    return sMapMgr->FindMap(dungeonData->map, instanceId);
}

bool ScenarioProgress::IsCompleted(bool bonus) const
{
    if (!HasBonusStep())
        return false;

    return currentStep == GetStepCount(bonus);
}

uint8 ScenarioProgress::GetBonusStepCount() const
{
    uint8 count = 0;
    for (ScenarioSteps::const_iterator itr = steps.begin(); itr != steps.end(); ++itr)
        if (itr->second->IsBonusObjective())
            ++count;

    return count;
}

bool ScenarioProgress::HasBonusStep() const
{
    return GetBonusStepCount() > 0;
}

uint8 ScenarioProgress::GetStepCount(bool withBonus) const
{
    if (withBonus)
        return steps.size();

    uint8 count = 0;
    for (ScenarioSteps::const_iterator itr = steps.begin(); itr != steps.end(); ++itr)
        if (!itr->second->IsBonusObjective())
            ++count;

    return count;
}

uint8 ScenarioProgress::UpdateCurrentStep(bool loading)
{
    uint8 oldStep = currentStep;
    for (ScenarioSteps::const_iterator itr = steps.begin(); itr != steps.end(); ++itr)
    {
        //Not check if ctep already complete
        if (currentStep > itr->second->OrderIndex)
            continue;

        CriteriaTreeEntry const* criteriaTree = sCriteriaTreeStore.LookupEntry(itr->second->Criteriatreeid);
        if (!criteriaTree)
            continue;

        if (GetAchievementMgr().IsCompletedScenarioTree(criteriaTree))
        {
            currentStep = itr->second->OrderIndex + 1;
            currentTree = GetScenarioCriteriaByStep(currentStep);
            continue;
        }
    }

    if (currentStep != oldStep && !loading)
    {
        if (currentStep != 0 && currentStep < steps.size())
            ActiveSteps.push_back(steps[currentStep]->ID);

        SendStepUpdate();

        if (IsCompleted(false))
            Reward(false);
        else if (IsCompleted(true))
            Reward(true);
    }

    //sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "UpdateCurrentStep currentStep %u oldStep %u loading %u", currentStep, oldStep, loading);
    return currentStep;
}

uint32 ScenarioProgress::GetScenarioCriteriaByStep(uint8 step)
{
    for (ScenarioSteps::const_iterator itr = steps.begin(); itr != steps.end(); ++itr)
        if (step == itr->second->OrderIndex)
            return itr->second->Criteriatreeid;

    return 0;
}

void ScenarioProgress::Reward(bool bonus)
{
    //sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "ScenarioProgress::Reward bonus %u rewarded %u bonusRewarded %u", bonus, rewarded, bonusRewarded);

    if (bonus && bonusRewarded)
        return;

    if (!bonus && rewarded)
        return;

    if (bonus)
    {
        bonusRewarded = true;

        // no bonus steps for this scenario...
        if (!HasBonusStep())
            return;
    }
    else
        rewarded = true;

    ObjectGuid groupGuid;
    Map* map = GetMap();
    // map not created? bye-bye reward
    if (!map)
        return;

    Map::PlayerList const& players = map->GetPlayers();
    // try to find group guid to refer to
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* player = itr->getSource();
        if (!player || !player->IsInWorld() || player->isGameMaster())
            continue;

        if (groupGuid.IsEmpty())
            groupGuid = player->GetGroup() ? player->GetGroup()->GetGUID() : ObjectGuid::Empty;

        if (!groupGuid.IsEmpty())
            break;
    }

    // should not happen
    if (groupGuid.IsEmpty())
        return;

    uint32 dungeonId = sLFGMgr->GetDungeon(groupGuid);
    // lfg dungeons are rewarded through lfg
    if (dungeonId)
    {
        // lfg dungeon that we are in is not current scenario
        if (dungeonId != dungeonData->id)
            return;

        sLFGMgr->FinishDungeon(groupGuid, dungeonId);
    }
    // we have challenge
    else
    {
        ASSERT(GetType() != SCENARIO_TYPE_NORMAL);

        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* player = itr->getSource();
            if (!player || !player->IsInWorld() || player->isGameMaster())
                continue;

            lfg::LfgReward const* reward = sLFGMgr->GetDungeonReward(dungeonData->id, player->getLevel());
            if (!reward)
                continue;

            // TODO: find challenge reward opcode ?
            bool alreadyCompleted = reward->RewardPlayer(player, dungeonData, dungeonData, bonus);
            lfg::LfgPlayerRewardData data = lfg::LfgPlayerRewardData(dungeonData->Entry(), dungeonData->Entry(), alreadyCompleted, bonus, reward);
            player->GetSession()->SendLfgPlayerReward(data);
        }
    }
}

void ScenarioProgress::SendStepUpdate(Player* player, bool full)
{
    WorldPackets::Scenario::ScenarioState state;

    //@TODO implent this
    //WorldPackets::Scenario::ScenarioState::BonusObjective objective;
    //{
    //    objective.BonusObjectiveID = 0;
    //    objective.ObjectiveComplete = 0;
    //    state.Objectives.push_back(objective);
    //}

    CriteriaProgressMap* progressMap = GetAchievementMgr().GetCriteriaProgressMap(0);

    WorldPackets::Achievement::CriteriaTreeProgress progress;

    state.ScenarioID = GetScenarioId();
    state.CurrentStep = currentStep < steps.size() ? steps[currentStep]->ID : -1;
    state.DifficultyID = 0;
    state.WaveCurrent = 0;
    state.WaveMax = 0;
    state.TimerDuration = 0;
    state.ScenarioComplete = IsCompleted(false);
    state.ActiveSteps = ActiveSteps;

    if (full)
    {
        if (progressMap)
        {
            for (CriteriaProgressMap::const_iterator itr = progressMap->begin(); itr != progressMap->end(); ++itr)
            {
                CriteriaTreeProgress const& treeProgress = itr->second;
                CriteriaTreeEntry const* criteriaTreeEntry = sCriteriaTreeStore.LookupEntry(itr->first);
                if (!criteriaTreeEntry)
                    continue;

                progress.Id = criteriaTreeEntry->CriteriaID;
                progress.Quantity = treeProgress.counter;
                progress.Player = ObjectGuid::Create<HighGuid::Scenario>(0, GetScenarioId(), 1); // whats the fuck ?
                progress.Flags = 0;
                progress.Date = time(nullptr) - treeProgress.date;
                progress.TimeFromStart = time(nullptr) - treeProgress.date;
                progress.TimeFromCreate = time(nullptr) - treeProgress.date;
                state.Progress.push_back(progress);
            }
        }
    }

    if (player)
        player->SendDirectMessage(state.Write());
    else
    {
        if (Map* map = GetMap())
            map->SendToPlayers(state.Write());
    }
}

void ScenarioProgress::SendCriteriaUpdate(CriteriaTreeProgress const* progress)
{
    //sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "ScenarioProgress::SendCriteriaUpdate criteria %u counter %u", progress->criteriaTree->criteria, progress->counter);

    WorldPackets::Scenario::ScenarioProgressUpdate update;

    WorldPackets::Achievement::CriteriaTreeProgress& progressUpdate = update.Progress;
    progressUpdate.Id = progress->criteriaTree->CriteriaID;
    progressUpdate.Quantity = progress->counter;
    progressUpdate.Player = progress->CompletedGUID;
    progressUpdate.Flags = 0;
    progressUpdate.Date = progress->date;
    progressUpdate.TimeFromStart = 0;
    progressUpdate.TimeFromCreate = 0;
    BroadCastPacket(update.Write());
}

void ScenarioProgress::BroadCastPacket(const WorldPacket* data)
{
    Map* map = GetMap();
    if (!map)
        return;

    map->SendToPlayers(data);
}

bool ScenarioProgress::CanUpdateCriteria(uint32 criteriaId, uint32 recursTree /*=0*/) const
{
    auto const& cTreeList = sDB2Manager.GetCriteriaTreeList(recursTree ? recursTree : currentTree);
    if (!cTreeList)
        return false;

    for (std::vector<CriteriaTreeEntry const*>::const_iterator itr = cTreeList->begin(); itr != cTreeList->end(); ++itr)
    {
        if (CriteriaTreeEntry const* criteriaTree = *itr)
        {
            if (criteriaTree->CriteriaID == 0)
            {
                if (CanUpdateCriteria(criteriaId, criteriaTree->ID))
                    return true;
            }
            else if (criteriaTree->ID == criteriaId)
                return true;
        }
    }

    return false;
}

ScenarioMgr::ScenarioMgr() : updateDiff(0)
{
    for (ScenarioStepEntry const* entry : sScenarioStepStore)
    {
        if (!sScenarioStore.LookupEntry(entry->ScenarioID))
            continue;

        m_stepMap[entry->ScenarioID][entry->OrderIndex] = entry;
    }
}

ScenarioMgr::~ScenarioMgr()
{
    for (ScenarioProgressMap::iterator itr = m_scenarioProgressMap.begin(); itr != m_scenarioProgressMap.end(); ++itr)
        delete itr->second;
}

ScenarioProgress* ScenarioMgr::GetScenarioProgress(uint32 instanceId)
{
    ScenarioProgressMap::iterator itr = m_scenarioProgressMap.find(instanceId);
    return itr != m_scenarioProgressMap.end() ? itr->second : nullptr;
}

void ScenarioMgr::AddScenarioProgress(uint32 instanceId, lfg::LFGDungeonData const* dungeonData, bool /*loading*/)
{
    if (m_scenarioProgressMap.find(instanceId) != m_scenarioProgressMap.end())
        return;

    ScenarioProgress* progress = new ScenarioProgress(instanceId, dungeonData);
    m_scenarioProgressMap[instanceId] = progress;
}

void ScenarioMgr::RemoveScenarioProgress(uint32 instanceId)
{
    ScenarioProgressMap::iterator itr = m_scenarioProgressMap.find(instanceId);
    if (itr == m_scenarioProgressMap.end())
        return;

    delete itr->second;
    m_scenarioProgressMap.erase(itr);
}

ScenarioType ScenarioMgr::GetScenarioType(uint32 scenarioId)
{
    ScenarioEntry const* entry = sScenarioStore.LookupEntry(scenarioId);
    ASSERT(entry);

    if (entry->IsProvingGrounds())
        return SCENARIO_TYPE_PROVING_GROUNDS;
    if (entry->IsChallenge())
        return SCENARIO_TYPE_CHALLENGE;

    return SCENARIO_TYPE_NORMAL;
}

void ScenarioMgr::Update(uint32 diff)
{
    updateDiff += diff;
    if (updateDiff < 5 * MINUTE * IN_MILLISECONDS)
        return;

    updateDiff -= 5 * MINUTE * IN_MILLISECONDS;
}

ScenarioSteps const* ScenarioMgr::GetScenarioSteps(uint32 scenarioId)
{
    ScenarioStepsByScenarioMap::const_iterator itr = m_stepMap.find(scenarioId);
    return itr != m_stepMap.end() ? &itr->second : nullptr;
}
