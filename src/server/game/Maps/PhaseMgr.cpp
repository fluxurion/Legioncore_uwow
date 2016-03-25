/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "PhaseMgr.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "ConditionMgr.h"


//////////////////////////////////////////////////////////////////
// Updating

PhaseMgr::PhaseMgr(Player* _player) : player(_player), phaseData(_player), _UpdateFlags(0)
{
    _PhaseDefinitionStore = sObjectMgr->GetPhaseDefinitionStore();
    _SpellPhaseStore = sObjectMgr->GetSpellPhaseStore();
}

void PhaseMgr::Update()
{
    if (IsUpdateInProgress())
        return;

    if (_UpdateFlags & PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED)
        phaseData.SendPhaseshiftToPlayer();

    if (_UpdateFlags & PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED)
        phaseData.SendPhaseMaskToPlayer();

    _UpdateFlags = 0;
}

void PhaseMgr::RemoveUpdateFlag(PhaseUpdateFlag updateFlag)
{
    _UpdateFlags &= ~updateFlag;

    if (updateFlag == PHASE_UPDATE_FLAG_ZONE_UPDATE ||
        updateFlag == PHASE_UPDATE_FLAG_AREA_UPDATE)
    {
        // Update zone changes
        if (phaseData.HasActiveDefinitions())
        {
            phaseData.ResetDefinitions();
            _UpdateFlags |= (PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED | PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED);
        }

        if (_PhaseDefinitionStore->find(player->GetZoneId()) != _PhaseDefinitionStore->end())
            Recalculate();
    }

    Update();
}

/////////////////////////////////////////////////////////////////
// Notifier

void PhaseMgr::NotifyConditionChanged(PhaseUpdateData const& updateData)
{
    if (NeedsPhaseUpdateWithData(updateData))
    {
        Recalculate();
        Update();
    }
}

//////////////////////////////////////////////////////////////////
// Phasing Definitions

void PhaseMgr::Recalculate()
{
    if (phaseData.HasActiveDefinitions())
    {
        phaseData.ResetDefinitions();
        _UpdateFlags |= (PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED | PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED);
    }

    PhaseDefinitionStore::const_iterator itr = _PhaseDefinitionStore->find(player->GetZoneId());
    if (itr != _PhaseDefinitionStore->end())
        for (PhaseDefinitionContainer::const_iterator phase = itr->second.begin(); phase != itr->second.end(); ++phase)
            if (CheckDefinition(&(*phase)))
            {
                phaseData.AddPhaseDefinition(&(*phase));

                sLog->outDebug(LOG_FILTER_PLAYER_LOADING, "PhaseMgr::recalculete enable id: %u zone %u: ", phase->entry, phase->zoneId);

                if (phase->phasemask)
                    _UpdateFlags |= PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED;

                if (phase->phaseId.size() || phase->terrainswapmap || phase->wmAreaId)
                    _UpdateFlags |= PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED;

                if (phase->IsLastDefinition())
                    break;
            }

    std::set<uint32> phaseIds;
    for (std::list<PhaseDefinition const*>::const_iterator itr = phaseData.activePhaseDefinitions.begin(); itr != phaseData.activePhaseDefinitions.end(); ++itr)
    {
        for (auto phaseID : (*itr)->phaseId)
            phaseIds.insert(phaseID);
    }

    for (PhaseInfoContainer::const_iterator itr = phaseData.spellPhaseInfo.begin(); itr != phaseData.spellPhaseInfo.end(); ++itr)
        if (itr->second.phaseId)
            phaseIds.insert(itr->second.phaseId);

    player->SetPhaseId(phaseIds, true);
}

inline bool PhaseMgr::CheckDefinition(PhaseDefinition const* phaseDefinition)
{
    ConditionList const conditions = sConditionMgr->GetConditionsForPhaseDefinition(phaseDefinition->zoneId, phaseDefinition->entry);
    if (conditions.empty())
        return true;

    return sConditionMgr->IsObjectMeetToConditions(player, conditions);
}

bool PhaseMgr::NeedsPhaseUpdateWithData(PhaseUpdateData const updateData) const
{
    PhaseDefinitionStore::const_iterator itr = _PhaseDefinitionStore->find(player->GetZoneId());
    if (itr != _PhaseDefinitionStore->end())
    {
        for (PhaseDefinitionContainer::const_iterator phase = itr->second.begin(); phase != itr->second.end(); ++phase)
        {
            ConditionList conditionList = sConditionMgr->GetConditionsForPhaseDefinition(phase->zoneId, phase->entry);
            for (ConditionList::const_iterator condition = conditionList.begin(); condition != conditionList.end(); ++condition)
                if (updateData.IsConditionRelated(*condition))
                    return true;
        }
    }

    if (player->GetPhases().size())
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////
// Auras

void PhaseMgr::RegisterPhasingAuraEffect(AuraEffect const* auraEffect)
{
    PhaseInfo phaseInfo;

    if (auraEffect->GetMiscValue())
    {
        _UpdateFlags |= PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED;
        phaseInfo.phasemask = auraEffect->GetMiscValue();
    }

    phaseInfo.phaseId = auraEffect->GetMiscValueB();
    phaseData.AddAuraInfo(auraEffect->GetId(), phaseInfo);

    if (phaseInfo.NeedsClientSideUpdate())
    {
        _UpdateFlags |= PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED;

        if (phaseInfo.phaseId)
            Recalculate();
    }

    Update();
}

void PhaseMgr::UnRegisterPhasingAuraEffect(AuraEffect const* auraEffect)
{
    _UpdateFlags |= phaseData.RemoveAuraInfo(auraEffect->GetId());

    if (auraEffect->GetMiscValueB())
        Recalculate();

    Update();
}

void PhaseMgr::RegisterPhasingAura(uint32 spellId, Unit* target)
{
    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "PhaseMgr::RegisterPhasingAura: spellId %u", spellId);

    SpellPhaseStore::const_iterator itr = _SpellPhaseStore->find(spellId);
    if (itr == _SpellPhaseStore->end())
        return;

    PhaseInfo phaseInfo;

    if (itr->second.phasemask)
    {
        _UpdateFlags |= PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED;
        phaseInfo.phasemask = itr->second.phasemask;
    }

    if (itr->second.terrainswapmap)
        phaseInfo.terrainswapmap = itr->second.terrainswapmap;

    if (itr->second.phaseId)
        phaseInfo.phaseId = itr->second.phaseId;

    if (phaseInfo.NeedsClientSideUpdate())
        _UpdateFlags |= PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED;

    phaseData.AddAuraInfo(spellId, phaseInfo);

    Update();
    if (target->IsVisible())
        target->UpdateObjectVisibility();
}

void PhaseMgr::UnRegisterPhasingAura(uint32 spellId, Unit* target)
{
    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "PhaseMgr::RegisterPhasingAura: spellId %u", spellId);

    SpellPhaseStore::const_iterator itr = _SpellPhaseStore->find(spellId);
    if (itr == _SpellPhaseStore->end())
        return;

    _UpdateFlags |= phaseData.RemoveAuraInfo(spellId);

    Update();
    if (target->IsVisible())
        target->UpdateObjectVisibility();
}

//////////////////////////////////////////////////////////////////
// Commands

void PhaseMgr::SendDebugReportToPlayer(Player* const debugger)
{
    ChatHandler(debugger).PSendSysMessage(LANG_PHASING_REPORT_STATUS, player->GetName(), player->GetZoneId(), player->getLevel(), player->GetTeamId(), _UpdateFlags);

    PhaseDefinitionStore::const_iterator itr = _PhaseDefinitionStore->find(player->GetZoneId());
    if (itr == _PhaseDefinitionStore->end())
        ChatHandler(debugger).PSendSysMessage(LANG_PHASING_NO_DEFINITIONS, player->GetZoneId());
    else
    {
        for (PhaseDefinitionContainer::const_iterator phase = itr->second.begin(); phase != itr->second.end(); ++phase)
        {
            if (CheckDefinition(&(*phase)))
                ChatHandler(debugger).PSendSysMessage(LANG_PHASING_SUCCESS, phase->entry, phase->IsNegatingPhasemask() ? "negated Phase" : "Phase", phase->phasemask);
            else
                ChatHandler(debugger).PSendSysMessage(LANG_PHASING_FAILED, phase->phasemask, phase->entry, phase->zoneId);

            if (phase->IsLastDefinition())
            {
                ChatHandler(debugger).PSendSysMessage(LANG_PHASING_LAST_PHASE, phase->phasemask, phase->entry, phase->zoneId);
                break;
            }
        }
    }

    ChatHandler(debugger).PSendSysMessage(LANG_PHASING_LIST, phaseData._PhasemaskThroughDefinitions, phaseData._PhasemaskThroughAuras, phaseData._CustomPhasemask);

    ChatHandler(debugger).PSendSysMessage(LANG_PHASING_PHASEMASK, phaseData.GetPhaseMaskForSpawn(), player->GetPhaseMask());
}

void PhaseMgr::SetCustomPhase(uint32 const phaseMask)
{
    phaseData._CustomPhasemask = phaseMask;

    _UpdateFlags |= PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED;

    Update();
}

//////////////////////////////////////////////////////////////////
// Phase Data

uint32 PhaseData::GetCurrentPhasemask() const
{
    if (player->isGameMaster())
        return PHASEMASK_ANYWHERE;

    if (_CustomPhasemask)
        return _CustomPhasemask;

    return GetPhaseMaskForSpawn();
}

inline uint32 PhaseData::GetPhaseMaskForSpawn() const
{
    uint32 const phase = (_PhasemaskThroughDefinitions | _PhasemaskThroughAuras);
    return (phase ? phase : PHASEMASK_NORMAL);
}

void PhaseData::SendPhaseMaskToPlayer()
{
    // Server side update
    uint32 const phasemask = GetCurrentPhasemask();
    if (player->GetPhaseMask() == phasemask)
        return;

    player->SetPhaseMask(phasemask, false);

    if (player->IsVisible())
        player->UpdateObjectVisibility();
}

void PhaseData::SendPhaseshiftToPlayer()
{
    // Client side update
    std::set<uint32> phaseIds;
    std::set<uint32> terrainswaps;
    std::set<uint32> WorldMapAreaIds;
    uint32 flags = 0;

    for (PhaseInfoContainer::const_iterator itr = spellPhaseInfo.begin(); itr != spellPhaseInfo.end(); ++itr)
    {
        if (itr->second.terrainswapmap)
            terrainswaps.insert(itr->second.terrainswapmap);

        if (itr->second.phaseId)
            phaseIds.insert(itr->second.phaseId);
    }

    // Phase Definitions
    for (std::list<PhaseDefinition const*>::const_iterator itr = activePhaseDefinitions.begin(); itr != activePhaseDefinitions.end(); ++itr)
    {
        for (auto phaseID : (*itr)->phaseId)
            phaseIds.insert(phaseID);

        if ((*itr)->terrainswapmap)
            terrainswaps.insert((*itr)->terrainswapmap);

        if ((*itr)->wmAreaId)
            WorldMapAreaIds.insert((*itr)->wmAreaId);

        if ((*itr)->flags)
            flags = (*itr)->flags;
    }

    player->GetSession()->SendSetPhaseShift(phaseIds, terrainswaps, WorldMapAreaIds, std::set<uint32>(), flags);

    uint32 zoneid, areaid;
    player->GetZoneAndAreaId(zoneid, areaid);

    WorldPackets::WorldState::InitWorldStates packet;
    packet.MapID = player->GetMapId();
    packet.AreaID = areaid;
    packet.SubareaID = zoneid;
    player->GetSession()->SendPacket(packet.Write());
}

void PhaseData::AddPhaseDefinition(PhaseDefinition const* phaseDefinition)
{
    if (phaseDefinition->IsOverwritingExistingPhases())
    {
        activePhaseDefinitions.clear();
        _PhasemaskThroughDefinitions = phaseDefinition->phasemask;
    }
    else
    {
        if (phaseDefinition->IsNegatingPhasemask())
            _PhasemaskThroughDefinitions &= ~phaseDefinition->phasemask;
        else
            _PhasemaskThroughDefinitions |= phaseDefinition->phasemask;
    }

    activePhaseDefinitions.push_back(phaseDefinition);
}

void PhaseData::AddAuraInfo(uint32 const spellId, PhaseInfo phaseInfo)
{
    if (phaseInfo.phasemask)
        _PhasemaskThroughAuras |= phaseInfo.phasemask;
    spellPhaseInfo[spellId] = phaseInfo;
}

uint32 PhaseData::RemoveAuraInfo(uint32 const spellId)
{
    PhaseInfoContainer::const_iterator rAura = spellPhaseInfo.find(spellId);
    if (rAura != spellPhaseInfo.end())
    {
        uint32 updateflag = 0;

        if (rAura->second.NeedsClientSideUpdate())
            updateflag |= PHASE_UPDATE_FLAG_CLIENTSIDE_CHANGED;

        if (rAura->second.NeedsServerSideUpdate())
        {
            _PhasemaskThroughAuras = 0;

            updateflag |= PHASE_UPDATE_FLAG_SERVERSIDE_CHANGED;

            spellPhaseInfo.erase(rAura);
            for (PhaseInfoContainer::const_iterator itr = spellPhaseInfo.begin(); itr != spellPhaseInfo.end(); ++itr)
                _PhasemaskThroughAuras |= itr->second.phasemask;
        }

        return updateflag;
    }
    else
        return 0;
}

//////////////////////////////////////////////////////////////////
// Phase Update Data

void PhaseUpdateData::AddQuestUpdate(uint32 const questId)
{
    AddConditionType(CONDITION_QUESTREWARDED);
    AddConditionType(CONDITION_QUESTTAKEN);
    AddConditionType(CONDITION_QUEST_COMPLETE);
    AddConditionType(CONDITION_QUEST_NONE);

    _questId = questId;
}

bool PhaseUpdateData::IsConditionRelated(Condition const* condition) const
{
    switch (condition->ConditionType)
    {
        case CONDITION_QUESTREWARDED:
        case CONDITION_QUESTTAKEN:
        case CONDITION_QUEST_COMPLETE:
        case CONDITION_QUEST_NONE:
            return condition->ConditionValue1 == _questId && ((1 << condition->ConditionType) & _conditionTypeFlags);
        default:
            return (1 << condition->ConditionType) & _conditionTypeFlags;
    }
}

bool PhaseMgr::IsConditionTypeSupported(ConditionTypes const conditionType)
{
    switch (conditionType)
    {
        case CONDITION_AURA:
        case CONDITION_QUESTREWARDED:
        case CONDITION_QUESTTAKEN:
        case CONDITION_QUEST_COMPLETE:
        case CONDITION_QUEST_NONE:
        case CONDITION_TEAM:
        case CONDITION_CLASS:
        case CONDITION_RACE:
        case CONDITION_INSTANCE_INFO:
        case CONDITION_LEVEL:
        case CONDITION_AREA_EXPLORED:
        case CONDITION_SCENE_SEEN:
        case CONDITION_SCENE_TRIGER_EVENT:
        case CONDITION_QUEST_OBJECTIVE_DONE:
        case CONDITION_AREAID:
        case CONDITION_ZONEID:
            return true;
        default:
            return false;
    }
}

std::string PhaseMgr::GetPhaseIdString()
{
    std::ostringstream ss;
    for (std::list<PhaseDefinition const*>::const_iterator itr = phaseData.activePhaseDefinitions.begin(); itr != phaseData.activePhaseDefinitions.end(); ++itr)
    {
        ss << "Phase Zone: " << (*itr)->zoneId << " ID: " << (*itr)->entry << " PhaseIDs: ";
        for (auto phaseID : (*itr)->phaseId)
            ss << phaseID << " ";
        ss << "\n";
    }

    ss << "Spell PhaseIDs: ";
    for (PhaseInfoContainer::const_iterator itr = phaseData.spellPhaseInfo.begin(); itr != phaseData.spellPhaseInfo.end(); ++itr)
    {
        if (itr->second.phaseId)
            ss << itr->second.phaseId << " ";
    }
    return ss.str().c_str();
}