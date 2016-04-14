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

#ifndef TRINITY_SCENARIOMGR_H
#define TRINITY_SCENARIOMGR_H

#include "Common.h"
#include "DatabaseEnv.h"
#include "AchievementMgr.h"

class ScenarioProgress;
struct ScenarioStepEntry;

typedef std::map<uint8, ScenarioStepEntry const*> ScenarioSteps;

namespace lfg{
struct LFGDungeonData;
}

enum ScenarioType
{
    SCENARIO_TYPE_NORMAL            = 0,
    SCENARIO_TYPE_CHALLENGE         = 1,
    SCENARIO_TYPE_PROVING_GROUNDS   = 2,
};

class ScenarioProgress
{
public:
    ScenarioProgress(uint32 _instanceId, lfg::LFGDungeonData const* _dungeonData);

    uint32 GetInstanceId() const { return instanceId; }
    Map* GetMap();
    ScenarioType GetType() const { return type; }
    uint32 GetScenarioId() const { return scenarioId; }
    uint32 GetCurrentStep() const { return currentStep; }

    bool IsCompleted(bool bonus) const;
    uint8 GetBonusStepCount() const;
    bool HasBonusStep() const;
    uint8 GetStepCount(bool withBonus) const;
    uint8 UpdateCurrentStep(bool loading);
    void Reward(bool bonus);

    AchievementMgr<ScenarioProgress>& GetAchievementMgr() { return m_achievementMgr; }
    AchievementMgr<ScenarioProgress> const& GetAchievementMgr() const { return m_achievementMgr; }

    void SendStepUpdate(Player* player = NULL, bool full = false);
    void SendCriteriaUpdate(CriteriaTreeProgress const* progress);
    void BroadCastPacket(const WorldPacket* data);
    uint32 GetScenarioCriteriaByStep(uint8 step);

    bool CanUpdateCriteria(uint32 criteriaTreeId, uint32 recursTree = 0) const;

protected:
    uint32 instanceId;
    uint32 scenarioId;
    lfg::LFGDungeonData const* dungeonData;
    AchievementMgr<ScenarioProgress> m_achievementMgr;

    uint8 currentStep;
    uint32 currentTree;
    ScenarioSteps steps;
    std::vector<uint32> ActiveSteps;

    ScenarioType type;

    bool rewarded;
    bool bonusRewarded;
};

typedef std::unordered_map<uint32 /*instance_id*/, ScenarioProgress*> ScenarioProgressMap;
typedef std::unordered_map<uint32, ScenarioSteps> ScenarioStepsByScenarioMap;

class ScenarioMgr
{
public:
    ScenarioMgr();
    ~ScenarioMgr();

    static ScenarioMgr* instance()
    {
        static ScenarioMgr instance;
        return &instance;
    }


    void Update(uint32 diff);

    static ScenarioType GetScenarioType(uint32 scenarioId);

    void AddScenarioProgress(uint32 instanceId, lfg::LFGDungeonData const* _dungeonData, bool loading);
    void RemoveScenarioProgress(uint32 instanceId);
    ScenarioProgress* GetScenarioProgress(uint32 instanceId);

    ScenarioSteps const* GetScenarioSteps(uint32 scenarioId);

private:
    ScenarioProgressMap m_scenarioProgressMap;

    ScenarioStepsByScenarioMap m_stepMap;

    uint32 updateDiff;

};

#define sScenarioMgr ScenarioMgr::instance()

#endif
