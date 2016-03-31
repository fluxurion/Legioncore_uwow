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

#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ObjectMgr.h"
#include "SpellAuras.h"
#include "SpellAuraDefines.h"
#include "SharedDefines.h"
#include "DBCStores.h"
#include "World.h"
#include "Chat.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "MapManager.h"
#include "BattlegroundIC.h"
#include "BattlefieldWG.h"
#include "BattlefieldMgr.h"

bool IsPrimaryProfessionSkill(uint32 skill)
{
    SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(skill);
    if (!pSkill)
        return false;

    if (pSkill->CategoryID != SKILL_CATEGORY_PROFESSION)
        return false;

    return true;
}

bool IsWeaponSkill(uint32 skill)
{
    SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(skill);
    if (!pSkill)
        return false;

    if (pSkill->CategoryID != SKILL_CATEGORY_WEAPON)
        return false;

    return true;
}

bool IsPartOfSkillLine(uint32 skillId, uint32 spellId)
{
    SkillLineAbilityMapBounds skillBounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
    for (SkillLineAbilityMap::const_iterator itr = skillBounds.first; itr != skillBounds.second; ++itr)
        if (itr->second->SkillLine == skillId)
            return true;

    return false;
}

DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellInfo const* spellproto, bool triggered)
{
    if (!spellproto || spellproto->IsPositive())
        return DIMINISHING_NONE;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellproto->Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
            return DIMINISHING_TAUNT;
    }

    // Explicit Diminishing Groups
    switch (spellproto->ClassOptions.SpellClassSet)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (spellproto->Id)
            {
                case 143301:
                case 125500:
                case 112955:
                case 162652:
                    return DIMINISHING_NONE;
            }
            // Pet charge effects (Infernal Awakening, Demon Charge)
            if (spellproto->GetSpellVisual(DIFFICULTY_NONE) == 2816 && spellproto->Misc.SpellIconID == 15)
                return DIMINISHING_CONTROLLED_STUN;
            // Gnaw
            else if (spellproto->Id == 47481)
                return DIMINISHING_CONTROLLED_STUN;
            break;
        }
        // Event spells
        case SPELLFAMILY_UNK1:
            return DIMINISHING_NONE;
        case SPELLFAMILY_MAGE:
        {
            // Frostbite
            if (spellproto->ClassOptions.SpellClassMask[1] & 0x80000000)
                return DIMINISHING_ROOT;
            // Shattered Barrier
            else if (spellproto->GetSpellVisual(DIFFICULTY_NONE) == 12297)
                return DIMINISHING_ROOT;
            // Deep Freeze
            else if (spellproto->Misc.SpellIconID == 2939 && spellproto->GetSpellVisual(DIFFICULTY_NONE) == 9963)
                return DIMINISHING_CONTROLLED_STUN;
            // Frost Nova / Freeze (Water Elemental)
            else if (spellproto->Misc.SpellIconID == 193)
                return DIMINISHING_CONTROLLED_ROOT;
            // Dragon's Breath
            else if (spellproto->ClassOptions.SpellClassMask[0] & 0x800000)
                return DIMINISHING_DRAGONS_BREATH;
            // Ring of Frost
            else if (spellproto->Id == 82691)
                return DIMINISHING_DISORIENT;
            // Slow, Frostbolt
            else if (spellproto->Id == 31589 || spellproto->Id == 116)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            if (spellproto->Id == 114404)
                return DIMINISHING_NONE;
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            // Hamstring and Piercing Howl - limit duration to 10s in PvP
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x2 || spellproto->Id == 12323 || spellproto->Id == 137637)
                return DIMINISHING_LIMITONLY;
            // Charge Stun (own diminishing)
            else if (spellproto->ClassOptions.SpellClassMask[0] & 0x01000000)
                return DIMINISHING_CHARGE;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Curses/etc
            if ((spellproto->ClassOptions.SpellClassMask[0] & 0x80000000) || (spellproto->ClassOptions.SpellClassMask[1] & 0x200))
                return DIMINISHING_LIMITONLY;
            // Seduction
            else if (spellproto->Id == 6358 || spellproto->Id == 132412)
                return DIMINISHING_FEAR;
            // Mesmerize
            else if (spellproto->Id == 115268)
                return DIMINISHING_FEAR;
            else if (spellproto->Id == 87204 || spellproto->Id == 31117)
                return DIMINISHING_NONE;
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            // Entangling Roots
            // Nature's Grasp
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x00000200)
                return DIMINISHING_CONTROLLED_ROOT;

            switch (spellproto->Id)
            {
                case 770:    // Faerie Fire
                case 58180:  // Infected Wounds
                case 102355: // Faerie Swarm
                case 102354: // Faerie Swarm (Slow)
                    return DIMINISHING_LIMITONLY;
                case 45334: // Immobilized
                    return DIMINISHING_ROOT;
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Gouge
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x8)
                return DIMINISHING_DISORIENT;
            // Blind
            else if (spellproto->ClassOptions.SpellClassMask[0] & 0x1000000)
                return DIMINISHING_FEAR;
            // Paralytic Poison
             else if (spellproto->ClassOptions.SpellClassMask[2] & 0x00000001)
                 return DIMINISHING_OPENING_STUN;
            // Crippling poison - Limit to 10 seconds in PvP (No ClassOptions.SpellClassMask)
            else if (spellproto->Misc.SpellIconID == 163)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Hunter's Mark
            if ((spellproto->ClassOptions.SpellClassMask[0] & 0x400) && spellproto->Misc.SpellIconID == 538)
                return DIMINISHING_LIMITONLY;
            // Scatter Shot (own diminishing)
            else if ((spellproto->ClassOptions.SpellClassMask[0] & 0x40000) && spellproto->Misc.SpellIconID == 132)
                return DIMINISHING_SCATTER_SHOT;
            // Entrapment (own diminishing)
            else if (spellproto->GetSpellVisual(DIFFICULTY_NONE) == 7484 && spellproto->Misc.SpellIconID == 20)
                return DIMINISHING_ENTRAPMENT;
            // Wyvern Sting mechanic is MECHANIC_SLEEP but the diminishing is DIMINISHING_DISORIENT
            else if ((spellproto->ClassOptions.SpellClassMask[1] & 0x1000) && spellproto->Misc.SpellIconID == 1721)
                return DIMINISHING_DISORIENT;
            // Freezing Arrow
            else if (spellproto->ClassOptions.SpellClassMask[0] & 0x8)
                return DIMINISHING_DISORIENT;
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Judgement of Justice - limit duration to 10s in PvP
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x100000)
                return DIMINISHING_LIMITONLY;
            // Turn Evil
            else if (spellproto->Id == 10326 || spellproto->Id == 145067)
                return DIMINISHING_FEAR;
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            // Hungering Cold (no flags)
            if (spellproto->Misc.SpellIconID == 2797)
                return DIMINISHING_DISORIENT;
            // Mark of Blood
            else if ((spellproto->ClassOptions.SpellClassMask[0] & 0x10000000) && spellproto->Misc.SpellIconID == 2285)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_MONK:
        {
            switch (spellproto->Id)
            {
                case 123393: return DIMINISHING_DISORIENT;       break; // Breath of Fire
                case 120086: return DIMINISHING_CONTROLLED_STUN; break; // Fists of Fury
                case 116095: return DIMINISHING_LIMITONLY;       break; // Disable
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    // Lastly - Set diminishing depending on mechanic
    uint32 mechanic = spellproto->GetAllEffectsMechanicMask();
    if (mechanic & (1 << MECHANIC_CHARM))
        return DIMINISHING_MIND_CONTROL;
    if (mechanic & (1 << MECHANIC_SILENCE) && spellproto->Id != 81261)  // haxx: Solar Beam diminishing with itself
        return DIMINISHING_SILENCE;
    if (mechanic & (1 << MECHANIC_SLEEP))
        return DIMINISHING_SLEEP;
    if (mechanic & ((1 << MECHANIC_SAPPED) | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_SHACKLE)))
        return DIMINISHING_DISORIENT;
    // Mechanic Knockout, except Blast Wave
    if (mechanic & (1 << MECHANIC_INCAPACITATE) && spellproto->Misc.SpellIconID != 292)
        return DIMINISHING_DISORIENT;
    if (mechanic & (1 << MECHANIC_DISARM))
        return DIMINISHING_DISARM;
    if (mechanic & (1 << MECHANIC_FEAR))
        return DIMINISHING_FEAR;
    if (mechanic & (1 << MECHANIC_STUN))
        return triggered ? DIMINISHING_STUN : DIMINISHING_CONTROLLED_STUN;
    if (mechanic & (1 << MECHANIC_BANISH))
        return DIMINISHING_BANISH;
    if (mechanic & (1 << MECHANIC_ROOT))
        return triggered ? DIMINISHING_ROOT : DIMINISHING_CONTROLLED_ROOT;
    if (mechanic & (1 << MECHANIC_HORROR))
        return DIMINISHING_HORROR;

    return DIMINISHING_NONE;
}

DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_STUN:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_CHARGE:
            return DRTYPE_ALL;
        case DIMINISHING_LIMITONLY:
        case DIMINISHING_NONE:
            return DRTYPE_NONE;
        default:
            return DRTYPE_PLAYER;
    }
}

DiminishingLevels GetDiminishingReturnsMaxLevel(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
            return DIMINISHING_LEVEL_TAUNT_IMMUNE;
        default:
            return DIMINISHING_LEVEL_IMMUNE;
    }
}

int32 GetDiminishingReturnsLimitDuration(DiminishingGroup group, SpellInfo const* spellproto)
{
    if (!IsDiminishingReturnsGroupDurationLimited(group))
        return 0;

    // Explicit diminishing duration
    switch (spellproto->ClassOptions.SpellClassSet)
    {
        case SPELLFAMILY_DRUID:
        {
            // Faerie Fire and Faerie Swarm - limit to 20 seconds in PvP (5.4)
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x400 || spellproto->Id == 102355)
                return 20 * IN_MILLISECONDS;
            if (spellproto->Id == 33786)
                return 6 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Wyvern Sting
            if (spellproto->ClassOptions.SpellClassMask[1] & 0x1000)
                return 6 * IN_MILLISECONDS;
            // Wyvern Sting
            if (spellproto->Id == 117526)
                return 3 * IN_MILLISECONDS;
            // Hunter's Mark
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x400)
                return 20 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Repentance - limit to 6 seconds in PvP
            if (spellproto->ClassOptions.SpellClassMask[0] & 0x4)
                return 6 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Banish - limit to 6 seconds in PvP
            if (spellproto->ClassOptions.SpellClassMask[1] & 0x8000000)
                return 6 * IN_MILLISECONDS;
            // Curse of Tongues - limit to 12 seconds in PvP
            else if (spellproto->ClassOptions.SpellClassMask[2] & 0x800)
                return 12 * IN_MILLISECONDS;
            // Curse of Elements - limit to 120 seconds in PvP
            else if (spellproto->ClassOptions.SpellClassMask[1] & 0x200)
                return 120 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_MONK:
        {
            switch (spellproto->Id)
            {
                case 115078: // Paralysis
                case 116706: // Disable (Root)
                    return 4 * IN_MILLISECONDS;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    return 8 * IN_MILLISECONDS;
}

bool IsDiminishingReturnsGroupDurationLimited(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_BANISH:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_CONTROLLED_ROOT:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_DISARM:
        case DIMINISHING_DISORIENT:
        case DIMINISHING_ENTRAPMENT:
        case DIMINISHING_FEAR:
        case DIMINISHING_HORROR:
        case DIMINISHING_MIND_CONTROL:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_ROOT:
        case DIMINISHING_STUN:
        case DIMINISHING_SLEEP:
        case DIMINISHING_LIMITONLY:
            return true;
        default:
            return false;
    }
}

SpellMgr::SpellMgr()
{
}

SpellMgr::~SpellMgr()
{
    UnloadSpellInfoStore();
}

/// Some checks for spells, to prevent adding deprecated/broken spells for trainers, spell book, etc
bool SpellMgr::IsSpellValid(SpellInfo const* spellInfo, Player* player, bool msg)
{
    // not exist
    if (!spellInfo)
        return false;

    bool need_check_reagents = false;

    // check effects
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (spellInfo->Effects[i].Effect)
        {
        case 0:
            continue;

            // craft spell for crafting non-existed item (break client recipes list show)
        case SPELL_EFFECT_CREATE_ITEM:
        case SPELL_EFFECT_CREATE_ITEM_2:
            {
                if (spellInfo->Effects[i].ItemType == 0)
                {
                    // skip auto-loot crafting spells, its not need explicit item info (but have special fake items sometime)
                    if (!spellInfo->IsLootCrafting())
                    {
                        if (msg)
                        {
                            if (player)
                                ChatHandler(player).PSendSysMessage("Craft spell %u not have create item entry.", spellInfo->Id);
                            else
                                sLog->outError(LOG_FILTER_SQL, "Craft spell %u not have create item entry.", spellInfo->Id);
                        }
                        return false;
                    }

                }
                // also possible IsLootCrafting case but fake item must exist anyway
                else if (!sObjectMgr->GetItemTemplate(spellInfo->Effects[i].ItemType))
                {
                    if (msg)
                    {
                        if (player)
                            ChatHandler(player).PSendSysMessage("Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Effects[i].ItemType);
                        else
                            sLog->outError(LOG_FILTER_SQL, "Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Effects[i].ItemType);
                    }
                    return false;
                }

                need_check_reagents = true;
                break;
            }
        case SPELL_EFFECT_LEARN_SPELL:
            {
                SpellInfo const* spellInfo2 = sSpellMgr->GetSpellInfo(spellInfo->Effects[i].TriggerSpell);
                if (!IsSpellValid(spellInfo2, player, msg))
                {
                    if (msg)
                    {
                        if (player)
                            ChatHandler(player).PSendSysMessage("Spell %u learn to broken spell %u, and then...", spellInfo->Id, spellInfo->Effects[i].TriggerSpell);
                        else
                            sLog->outError(LOG_FILTER_SQL, "Spell %u learn to invalid spell %u, and then...", spellInfo->Id, spellInfo->Effects[i].TriggerSpell);
                    }
                    return false;
                }
                break;
            }
        }
    }

    if (need_check_reagents)
    {
        for (uint8 j = 0; j < MAX_SPELL_REAGENTS; ++j)
        {
            if (spellInfo->Reagents.Reagent[j] > 0 && !sObjectMgr->GetItemTemplate(spellInfo->Reagents.Reagent[j]))
            {
                if (msg)
                {
                    if (player)
                        ChatHandler(player).PSendSysMessage("Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagents.Reagent[j]);
                    else
                        sLog->outError(LOG_FILTER_SQL, "Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagents.Reagent[j]);
                }
                return false;
            }
        }
    }

    return true;
}

bool SpellMgr::IsSpellForbidden(uint32 spellid)
{
    std::list<uint32>::iterator Itr;

    for (Itr = mForbiddenSpells.begin(); Itr != mForbiddenSpells.end(); ++Itr)
        if ((*Itr) == spellid)
            return true;

    return false;
}

SpellChainNode const* SpellMgr::GetSpellChainNode(uint32 spell_id) const
{
    SpellChainMap::const_iterator itr = mSpellChains.find(spell_id);
    if (itr == mSpellChains.end())
        return NULL;

    return &itr->second;
}

uint32 SpellMgr::GetFirstSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->first->Id;

    return spell_id;
}

uint32 SpellMgr::GetLastSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->last->Id;

    return spell_id;
}

uint32 SpellMgr::GetNextSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        if (node->next)
            return node->next->Id;

    return 0;
}

uint32 SpellMgr::GetPrevSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        if (node->prev)
            return node->prev->Id;

    return 0;
}

uint8 SpellMgr::GetSpellRank(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->rank;

    return 0;
}

uint32 SpellMgr::GetSpellWithRank(uint32 spell_id, uint32 rank, bool strict) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
    {
        if (rank != node->rank)
            return GetSpellWithRank(node->rank < rank ? node->next->Id : node->prev->Id, rank, strict);
    }
    else if (strict && rank > 1)
        return 0;
    return spell_id;
}

SpellRequiredMapBounds SpellMgr::GetSpellsRequiredForSpellBounds(uint32 spell_id) const
{
    return SpellRequiredMapBounds(mSpellReq.lower_bound(spell_id), mSpellReq.upper_bound(spell_id));
}

SpellsRequiringSpellMapBounds SpellMgr::GetSpellsRequiringSpellBounds(uint32 spell_id) const
{
    return SpellsRequiringSpellMapBounds(mSpellsReqSpell.lower_bound(spell_id), mSpellsReqSpell.upper_bound(spell_id));
}

bool SpellMgr::IsSpellRequiringSpell(uint32 spellid, uint32 req_spellid) const
{
    SpellsRequiringSpellMapBounds spellsRequiringSpell = GetSpellsRequiringSpellBounds(req_spellid);
    for (SpellsRequiringSpellMap::const_iterator itr = spellsRequiringSpell.first; itr != spellsRequiringSpell.second; ++itr)
    {
        if (itr->second == spellid)
            return true;
    }
    return false;
}

const SpellsRequiringSpellMap SpellMgr::GetSpellsRequiringSpell()
{
    return this->mSpellsReqSpell;
}

uint32 SpellMgr::GetSpellRequired(uint32 spell_id) const
{
    SpellRequiredMap::const_iterator itr = mSpellReq.find(spell_id);

    if (itr == mSpellReq.end())
        return 0;

    return itr->second;
}

SpellLearnSkillNode const* SpellMgr::GetSpellLearnSkill(uint32 spell_id) const
{
    SpellLearnSkillMap::const_iterator itr = mSpellLearnSkills.find(spell_id);
    if (itr != mSpellLearnSkills.end())
        return &itr->second;
    else
        return NULL;
}

SpellLearnSpellMapBounds SpellMgr::GetSpellLearnSpellMapBounds(uint32 spell_id) const
{
    return SpellLearnSpellMapBounds(mSpellLearnSpells.lower_bound(spell_id), mSpellLearnSpells.upper_bound(spell_id));
}

bool SpellMgr::IsSpellLearnSpell(uint32 spell_id) const
{
    return mSpellLearnSpells.find(spell_id) != mSpellLearnSpells.end();
}

bool SpellMgr::IsSpellLearnToSpell(uint32 spell_id1, uint32 spell_id2) const
{
    SpellLearnSpellMapBounds bounds = GetSpellLearnSpellMapBounds(spell_id1);
    for (SpellLearnSpellMap::const_iterator i = bounds.first; i != bounds.second; ++i)
        if (i->second.spell == spell_id2)
            return true;
    return false;
}

SpellTargetPosition const* SpellMgr::GetSpellTargetPosition(uint32 spell_id) const
{
    SpellTargetPositionMap::const_iterator itr = mSpellTargetPositions.find(spell_id);
    if (itr != mSpellTargetPositions.end())
        return &itr->second;
    return NULL;
}

SpellSpellGroupMapBounds SpellMgr::GetSpellSpellGroupMapBounds(uint32 spell_id) const
{
    spell_id = GetFirstSpellInChain(spell_id);
    return SpellSpellGroupMapBounds(mSpellSpellGroup.lower_bound(spell_id), mSpellSpellGroup.upper_bound(spell_id));
}

uint32 SpellMgr::IsSpellMemberOfSpellGroup(uint32 spellid, SpellGroup groupid) const
{
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellid);
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        if (itr->second == groupid)
            return true;
    }
    return false;
}

SpellGroupSpellMapBounds SpellMgr::GetSpellGroupSpellMapBounds(SpellGroup group_id) const
{
    return SpellGroupSpellMapBounds(mSpellGroupSpell.lower_bound(group_id), mSpellGroupSpell.upper_bound(group_id));
}

void SpellMgr::GetSetOfSpellsInSpellGroup(SpellGroup group_id, std::set<uint32>& foundSpells) const
{
    std::set<SpellGroup> usedGroups;
    GetSetOfSpellsInSpellGroup(group_id, foundSpells, usedGroups);
}

void SpellMgr::GetSetOfSpellsInSpellGroup(SpellGroup group_id, std::set<uint32>& foundSpells, std::set<SpellGroup>& usedGroups) const
{
    if (usedGroups.find(group_id) != usedGroups.end())
        return;
    usedGroups.insert(group_id);

    SpellGroupSpellMapBounds groupSpell = GetSpellGroupSpellMapBounds(group_id);
    for (SpellGroupSpellMap::const_iterator itr = groupSpell.first; itr != groupSpell.second; ++itr)
    {
        if (itr->second < 0)
        {
            SpellGroup currGroup = (SpellGroup)abs(itr->second);
            GetSetOfSpellsInSpellGroup(currGroup, foundSpells, usedGroups);
        }
        else
        {
            foundSpells.insert(itr->second);
        }
    }
}

bool SpellMgr::AddSameEffectStackRuleSpellGroups(SpellInfo const* spellInfo, AuraEffect* eff, std::multimap<SpellGroup, AuraEffect*>& groups) const
{
    uint32 spellId = spellInfo->GetFirstRankSpell()->Id;
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellId);
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        SpellGroup group = itr->second;
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(group);
        if (found != mSpellGroupStack.end())
        {
            if (found->second == SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT)
            {
                if (groups.find(group) == groups.end())
                    groups.insert(std::multimap<SpellGroup, AuraEffect*>::value_type(group, eff));
                else
                {
                    for (std::multimap<SpellGroup, AuraEffect*>::iterator iter = groups.lower_bound(group); iter != groups.upper_bound(group);)
                    {
                        AuraEffect* iterEff = iter->second;
                        if (abs(iterEff->GetAmount()) <= abs(eff->GetAmount()))
                        {
                            groups.erase(iter);
                            groups.insert(std::multimap<SpellGroup, AuraEffect*>::value_type(group, eff));
                        }
                        break;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool SpellMgr::AddSameEffectStackRuleSpellGroups(SpellInfo const* spellInfo, int32 amount, std::map<SpellGroup, int32>& groups) const
{
    uint32 spellId = spellInfo->GetFirstRankSpell()->Id;
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellId);
    // Find group with SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT if it belongs to one
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        SpellGroup group = itr->second;
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(group);
        if (found != mSpellGroupStack.end())
        {
            if (found->second == SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT)
            {
                // Put the highest amount in the map
                if (groups.find(group) == groups.end())
                    groups[group] = amount;
                else
                {
                    int32 curr_amount = groups[group];
                    // Take absolute value because this also counts for the highest negative aura
                    if (abs(curr_amount) < abs(amount))
                        groups[group] = amount;
                }
                // return because a spell should be in only one SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT group
                return true;
            }
        }
    }
    // Not in a SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT group, so return false
    return false;
}

SpellGroupStackRule SpellMgr::CheckSpellGroupStackRules(SpellInfo const* spellInfo1, SpellInfo const* spellInfo2) const
{
    uint32 spellid_1 = spellInfo1->GetFirstRankSpell()->Id;
    uint32 spellid_2 = spellInfo2->GetFirstRankSpell()->Id;
    if (spellid_1 == spellid_2)
        return SPELL_GROUP_STACK_RULE_DEFAULT;
    // find SpellGroups which are common for both spells
    SpellSpellGroupMapBounds spellGroup1 = GetSpellSpellGroupMapBounds(spellid_1);
    std::set<SpellGroup> groups;
    for (SpellSpellGroupMap::const_iterator itr = spellGroup1.first; itr != spellGroup1.second; ++itr)
    {
        if (IsSpellMemberOfSpellGroup(spellid_2, itr->second))
        {
            bool add = true;
            SpellGroupSpellMapBounds groupSpell = GetSpellGroupSpellMapBounds(itr->second);
            for (SpellGroupSpellMap::const_iterator itr2 = groupSpell.first; itr2 != groupSpell.second; ++itr2)
            {
                if (itr2->second < 0)
                {
                    SpellGroup currGroup = (SpellGroup)abs(itr2->second);
                    if (IsSpellMemberOfSpellGroup(spellid_1, currGroup) && IsSpellMemberOfSpellGroup(spellid_2, currGroup))
                    {
                        add = false;
                        break;
                    }
                }
            }
            if (add)
                groups.insert(itr->second);
        }
    }

    SpellGroupStackRule rule = SPELL_GROUP_STACK_RULE_DEFAULT;

    for (std::set<SpellGroup>::iterator itr = groups.begin(); itr!= groups.end(); ++itr)
    {
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(*itr);
        if (found != mSpellGroupStack.end())
            rule = found->second;
        if (rule)
            break;
    }
    return rule;
}

const std::vector<SpellProcEventEntry>* SpellMgr::GetSpellProcEvent(uint32 spellId) const
{
    SpellProcEventMap::const_iterator itr = mSpellProcEventMap.find(spellId);
    if (itr != mSpellProcEventMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::IsSpellProcEventCanTriggeredBy(SpellProcEventEntry const* spellProcEvent, uint32 EventProcFlag, SpellInfo const* procSpell, uint32 procFlags, uint32 procExtra, bool active)
{
    // No extra req need
    uint32 procEvent_procEx = PROC_EX_NONE;

    // check prockFlags for condition
    if ((procFlags & EventProcFlag) == 0)
        return false;

    /// Quick Check - If PROC_FLAG_TAKEN_DAMAGE is set for aura and procSpell dealt damage, proc no matter what kind of spell that deals the damage.
    if (procFlags & PROC_FLAG_TAKEN_DAMAGE && EventProcFlag & PROC_FLAG_TAKEN_DAMAGE)
        return true;

    if (procFlags & PROC_FLAG_DONE_PERIODIC && EventProcFlag & PROC_FLAG_DONE_PERIODIC)
    {
        if (procExtra & PROC_EX_INTERNAL_HOT)
        {
            if (!(EventProcFlag & (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS)))
                return false;
        }
        /// Aura must have negative or neutral(PROC_FLAG_DONE_PERIODIC only) procflags for a DOT to proc
        else if (EventProcFlag != PROC_FLAG_DONE_PERIODIC)
            if (!(EventProcFlag & (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG | PROC_FLAG_DONE_TRAP_ACTIVATION)))
                return false;
    }

    if (procFlags & PROC_FLAG_TAKEN_PERIODIC && EventProcFlag & PROC_FLAG_TAKEN_PERIODIC)
    {
        if (procExtra & PROC_EX_INTERNAL_HOT)
        {
            /// No aura that only has PROC_FLAG_TAKEN_PERIODIC can proc from a HOT.
            if (EventProcFlag == PROC_FLAG_TAKEN_PERIODIC)
                return false;
            /// Aura must have positive procflags for a HOT to proc
            if (!(EventProcFlag & (PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS)))
                return false;
        }
        /// Aura must have negative or neutral(PROC_FLAG_TAKEN_PERIODIC only) procflags for a DOT to proc
        else if (EventProcFlag != PROC_FLAG_TAKEN_PERIODIC)
            if (!(EventProcFlag & (PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG | PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_NEG)))
                return false;
    }

    // Trap casts are active by default
    if (procFlags & PROC_FLAG_DONE_TRAP_ACTIVATION)
        active = true;

    // Always trigger for this
    if (procFlags & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    if (spellProcEvent)     // Exist event data
    {
        // Store extra req
        procEvent_procEx = spellProcEvent->procEx;

        // For melee triggers
        if (procSpell == NULL)
        {
            // Check (if set) for school (melee attack have Normal school)
            if (spellProcEvent->schoolMask && (spellProcEvent->schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0)
                return false;
        }
        else // For spells need check school/spell family/family mask
        {
            // Check (if set) for school
            if (spellProcEvent->schoolMask && (spellProcEvent->schoolMask & procSpell->Misc.SchoolMask) == 0)
                return false;

            // Check (if set) for spellFamilyName
            if (spellProcEvent->spellFamilyName && (spellProcEvent->spellFamilyName != procSpell->ClassOptions.SpellClassSet))
                return false;

            // spellFamilyName is Ok need check for spellFamilyMask if present
            if (spellProcEvent->spellFamilyMask)
            {
                if (!(spellProcEvent->spellFamilyMask & procSpell->ClassOptions.SpellClassMask))
                    return false;

                // Some spells are not considered as active even with have spellfamilyflags
                if (!(procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL))
                    active = true;
            }
        }
    }

    // Check for extra req (if none) and hit/crit
    if (procEvent_procEx == PROC_EX_NONE)
    {
        // No extra req, so can trigger only for hit/crit - spell has to be active
        if ((procExtra & (PROC_EX_NORMAL_HIT | PROC_EX_CRITICAL_HIT))/* && active*/)
            return true;
    }
    else // Passive spells hits here only if resist/reflect/immune/evade
    {
        if (procExtra & AURA_SPELL_PROC_EX_MASK)
        {
            // if spell marked as procing only from not active spells
            if (active && procEvent_procEx & PROC_EX_NOT_ACTIVE_SPELL)
                return false;
            // if spell marked as procing only from active spells
            if (!active && procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL)
                return false;
            // Exist req for PROC_EX_EX_TRIGGER_ALWAYS
            if (procEvent_procEx & PROC_EX_EX_TRIGGER_ALWAYS)
                return true;
            // PROC_EX_NOT_ACTIVE_SPELL and PROC_EX_ONLY_ACTIVE_SPELL flags handle: if passed checks before
            if ((procExtra & (PROC_EX_NORMAL_HIT | PROC_EX_CRITICAL_HIT)) && ((procEvent_procEx & (AURA_SPELL_PROC_EX_MASK)) == 0))
                return true;
        }
        // Check Extra Requirement like (hit/crit/miss/resist/parry/dodge/block/immune/reflect/absorb and other)
        if (procEvent_procEx & procExtra)
            return true;
    }
    return false;
}

SpellProcEntry const* SpellMgr::GetSpellProcEntry(uint32 spellId) const
{
    SpellProcMap::const_iterator itr = mSpellProcMap.find(spellId);
    if (itr != mSpellProcMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::CanSpellTriggerProcOnEvent(SpellProcEntry const& procEntry, ProcEventInfo& eventInfo)
{
    // proc type doesn't match
    if (!(eventInfo.GetTypeMask() & procEntry.typeMask))
        return false;

    // check XP or honor target requirement
    if (procEntry.attributesMask & PROC_ATTR_REQ_EXP_OR_HONOR)
        if (Player* actor = eventInfo.GetActor()->ToPlayer())
            if (eventInfo.GetActionTarget() && !actor->isHonorOrXPTarget(eventInfo.GetActionTarget()))
                return false;

    // always trigger for these types
    if (eventInfo.GetTypeMask() & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    // check school mask (if set) for other trigger types
    if (procEntry.schoolMask && !(eventInfo.GetSchoolMask() & procEntry.schoolMask))
        return false;

    // check spell family name/flags (if set) for spells
    if (eventInfo.GetTypeMask() & (PERIODIC_PROC_FLAG_MASK | SPELL_PROC_FLAG_MASK | PROC_FLAG_DONE_TRAP_ACTIVATION))
    {
        if (procEntry.spellFamilyName && (procEntry.spellFamilyName != eventInfo.GetSpellInfo()->ClassOptions.SpellClassSet))
            return false;

        if (procEntry.spellFamilyMask && !(procEntry.spellFamilyMask & eventInfo.GetSpellInfo()->ClassOptions.SpellClassMask))
            return false;
    }

    // check spell type mask (if set)
    if (eventInfo.GetTypeMask() & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK))
    {
        if (procEntry.spellTypeMask && !(eventInfo.GetSpellTypeMask() & procEntry.spellTypeMask))
            return false;
    }

    // check spell phase mask
    if (eventInfo.GetTypeMask() & REQ_SPELL_PHASE_PROC_FLAG_MASK)
    {
        if (!(eventInfo.GetSpellPhaseMask() & procEntry.spellPhaseMask))
            return false;
    }

    // check hit mask (on taken hit or on done hit, but not on spell cast phase)
    if ((eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK) || ((eventInfo.GetTypeMask() & DONE_HIT_PROC_FLAG_MASK) && !(eventInfo.GetSpellPhaseMask() & PROC_SPELL_PHASE_CAST)))
    {
        uint32 hitMask = procEntry.hitMask;
        // get default values if hit mask not set
        if (!hitMask)
        {
            // for taken procs allow normal + critical hits by default
            if (eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL;
            // for done procs allow normal + critical + absorbs by default
            else
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB;
        }
        if (!(eventInfo.GetHitMask() & hitMask))
            return false;
    }

    return true;
}

SpellBonusEntry const* SpellMgr::GetSpellBonusData(uint32 spellId) const
{
    // Lookup data
    SpellBonusMap::const_iterator itr = mSpellBonusMap.find(spellId);
    if (itr != mSpellBonusMap.end())
        return &itr->second;
    // Not found, try lookup for 1 spell rank if exist
    if (uint32 rank_1 = GetFirstSpellInChain(spellId))
    {
        SpellBonusMap::const_iterator itr2 = mSpellBonusMap.find(rank_1);
        if (itr2 != mSpellBonusMap.end())
            return &itr2->second;
    }
    return NULL;
}

SpellThreatEntry const* SpellMgr::GetSpellThreatEntry(uint32 spellID) const
{
    SpellThreatMap::const_iterator itr = mSpellThreatMap.find(spellID);
    if (itr != mSpellThreatMap.end())
        return &itr->second;
    else
    {
        uint32 firstSpell = GetFirstSpellInChain(spellID);
        itr = mSpellThreatMap.find(firstSpell);
        if (itr != mSpellThreatMap.end())
            return &itr->second;
    }
    return NULL;
}

SkillLineAbilityMapBounds SpellMgr::GetSkillLineAbilityMapBounds(uint32 spell_id) const
{
    return SkillLineAbilityMapBounds(mSkillLineAbilityMap.lower_bound(spell_id), mSkillLineAbilityMap.upper_bound(spell_id));
}

const std::vector<PetAura>* SpellMgr::GetPetAura(int32 entry) const
{
    SpellPetAuraMap::const_iterator itr = mSpellPetAuraMap.find(entry);
    return itr != mSpellPetAuraMap.end() ? &(itr->second) : NULL;
}

SpellEnchantProcEntry const* SpellMgr::GetSpellEnchantProcEvent(uint32 enchId) const
{
    SpellEnchantProcEventMap::const_iterator itr = mSpellEnchantProcEventMap.find(enchId);
    if (itr != mSpellEnchantProcEventMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::IsArenaAllowedEnchancment(uint32 ench_id) const
{
    return mEnchantCustomAttr[ench_id];
}

const std::vector<SpellLinked>* SpellMgr::GetSpellLinked(int32 spell_id) const
{
    SpellLinkedMap::const_iterator itr = mSpellLinkedMap.find(spell_id);
    return itr != mSpellLinkedMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellTalentLinked>* SpellMgr::GetSpelltalentLinked(int32 spell_id) const
{
    SpellTalentLinkedMap::const_iterator itr = mSpellTalentLinkedMap.find(spell_id);
    return itr != mSpellTalentLinkedMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellConcatenateAura>* SpellMgr::GetSpellConcatenateApply(int32 spell_id) const
{
    SpellConcatenateApplyMap::const_iterator itr = mSpellConcatenateApplyMap.find(spell_id);
    return itr != mSpellConcatenateApplyMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellConcatenateAura>* SpellMgr::GetSpellConcatenateUpdate(int32 spell_id) const
{
    SpellConcatenateUpdateMap::const_iterator itr = mSpellConcatenateUpdateMap.find(spell_id);
    return itr != mSpellConcatenateUpdateMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellVisual>* SpellMgr::GetPlaySpellVisualData(int32 spell_id) const
{
    PlaySpellVisualMap::const_iterator itr = mSpellVisualMap.find(spell_id);
    return itr != mSpellVisualMap.end() ? &(itr->second) : NULL;
}

const SpellVisualPlayOrphan* SpellMgr::GetSpellVisualPlayOrphan(int32 spell_id) const
{
    SpellVisualPlayOrphanMap::const_iterator itr = mSpellVisualPlayOrphanMap.find(spell_id);
    return itr != mSpellVisualPlayOrphanMap.end() ? &(itr->second) : NULL;
}

const SpellScene* SpellMgr::GetSpellScene(int32 miscValue) const
{
    SpellSceneMap::const_iterator itr = mSpellSceneMap.find(miscValue);
    return itr != mSpellSceneMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellPendingCast>* SpellMgr::GetSpellPendingCast(int32 spellID) const
{
    auto itr = mSpellPendingCastMap.find(spellID);
    if (itr != mSpellPendingCastMap.end())
        return &itr->second;

    return nullptr;
}

const uint32 SpellMgr::GetMountListId(uint32 spell_id, uint32 teamid) const
{
    uint32 faction = 0; //both
    if(teamid == TEAM_ALLIANCE)
        faction = 1;
    if(teamid == TEAM_HORDE)
        faction = 2;

    SpellMountListMap::const_iterator itr = mSpellMountListMap.find(spell_id);
    if (itr != mSpellMountListMap.end())
    {
        SpellMountList* _mountList =  itr->second;
        if(_mountList->side == 0 || _mountList->side == faction)
            return spell_id;
        if(_mountList->side != faction)
            return _mountList->spellIdS;
    }
    else
        return spell_id;

    return 0;
}

const std::vector<SpellPrcoCheck>* SpellMgr::GetSpellPrcoCheck(int32 spell_id) const
{
    SpellPrcoCheckMap::const_iterator itr = mSpellPrcoCheckMap.find(spell_id);
    return itr != mSpellPrcoCheckMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellTriggered>* SpellMgr::GetSpellTriggered(int32 spell_id) const
{
    SpellTriggeredMap::const_iterator itr = mSpellTriggeredMap.find(spell_id);
    return itr != mSpellTriggeredMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellDummyTrigger>* SpellMgr::GetSpellDummyTrigger(int32 spell_id) const
{
    SpellDummyTriggerMap::const_iterator itr = mSpellDummyTriggerMap.find(spell_id);
    return itr != mSpellDummyTriggerMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellDummyTrigger>* SpellMgr::GetSpellAuraTrigger(int32 spell_id) const
{
    SpellAuraTriggerMap::const_iterator itr = mSpellAuraTriggerMap.find(spell_id);
    return itr != mSpellAuraTriggerMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellAuraDummy>* SpellMgr::GetSpellAuraDummy(int32 spell_id) const
{
    SpellAuraDummyMap::const_iterator itr = mSpellAuraDummyMap.find(spell_id);
    return itr != mSpellAuraDummyMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellTargetFilter>* SpellMgr::GetSpellTargetFilter(int32 spell_id) const
{
    SpellTargetFilterMap::const_iterator itr = mSpellTargetFilterMap.find(spell_id);
    return itr != mSpellTargetFilterMap.end() ? &(itr->second) : NULL;
}

const std::vector<SpellCheckCast>* SpellMgr::GetSpellCheckCast(int32 spell_id) const
{
    SpellCheckCastMap::const_iterator itr = mSpellCheckCastMap.find(spell_id);
    return itr != mSpellCheckCastMap.end() ? &(itr->second) : NULL;
}

PetLevelupSpellSet const* SpellMgr::GetPetLevelupSpellList(uint32 petFamily) const
{
    PetLevelupSpellMap::const_iterator itr = mPetLevelupSpellMap.find(petFamily);
    if (itr != mPetLevelupSpellMap.end())
        return &itr->second;
    else
        return NULL;
}

PetDefaultSpellsEntry const* SpellMgr::GetPetDefaultSpellsEntry(int32 id) const
{
    PetDefaultSpellsMap::const_iterator itr = mPetDefaultSpellsMap.find(id);
    if (itr != mPetDefaultSpellsMap.end())
        return &itr->second;
    return NULL;
}

SpellAreaMapBounds SpellMgr::GetSpellAreaMapBounds(uint32 spell_id) const
{
    return mSpellAreaMap.equal_range(spell_id);
}

SpellAreaForQuestMapBounds SpellMgr::GetSpellAreaForQuestMapBounds(uint32 quest_id) const
{
    return mSpellAreaForQuestMap.equal_range(quest_id);
}

SpellAreaForQuestMapBounds SpellMgr::GetSpellAreaForQuestEndMapBounds(uint32 quest_id) const
{
    return mSpellAreaForQuestEndMap.equal_range(quest_id);
}

SpellAreaForAuraMapBounds SpellMgr::GetSpellAreaForAuraMapBounds(uint32 spell_id) const
{
    return mSpellAreaForAuraMap.equal_range(spell_id);
}

SpellAreaForAreaMapBounds SpellMgr::GetSpellAreaForAreaMapBounds(uint32 area_id) const
{
    return mSpellAreaForAreaMap.equal_range(area_id);
}

bool SpellArea::IsFitToRequirements(Player const* player, uint32 newZone, uint32 newArea) const
{
    if (gender != GENDER_NONE)                   // not in expected gender
        if (!player || gender != player->getGender())
            return false;

    if (raceMask)                                // not in expected race
        if (!player || !(raceMask & player->getRaceMask()))
            return false;

    if (areaId > 0)                                  // not in expected zone
        if (newZone != areaId && newArea != areaId)
            return false;

    if (areaId < 0 && player)
        if(player->GetMapId() != abs(areaId))
            return false;

    if (questStart)                              // not in expected required quest state
        if (!player || ((questStartStatus & (1 << player->GetQuestStatus(questStart))) == 0))
            return false;

    if (questEnd)                                // not in expected forbidden quest state
        if (!player || (questEndStatus & (1 << player->GetQuestStatus(questEnd))))
            return false;

    if (auraSpell)                               // not have expected aura
        if (!player || (auraSpell > 0 && !player->HasAura(auraSpell)) || (auraSpell < 0 && player->HasAura(-auraSpell)))
            return false;

    // Extra conditions -- leaving the possibility add extra conditions...
    switch (spellId)
    {
        case 58600: // No fly Zone - Dalaran
        {
            if (!player)
                return false;

            AreaTableEntry const* pArea = sAreaTableStore.LookupEntry(player->GetAreaId());
            if (!(pArea && pArea->Flags[0] & AREA_FLAG_NO_FLY_ZONE))
                return false;

            if (!player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !player->HasAuraType(SPELL_AURA_FLY))
                return false;

            break;
        }
        case 91604: // No fly Zone - Wintergrasp
        {
            if (!player)
                return false;

            //Battlefield* Bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());
            if (/*!Bf || Bf->CanFlyIn() || */(!player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !player->HasAuraType(SPELL_AURA_FLY)))
                return false;
            break;
        }
        case 68719: // Oil Refinery - Isle of Conquest.
        case 68720: // Quarry - Isle of Conquest.
        {
            if (!player || player->GetBattlegroundTypeId() != BATTLEGROUND_IC || !player->GetBattleground())
                return false;

            uint8 nodeType = spellId == 68719 ? NODE_TYPE_REFINERY : NODE_TYPE_QUARRY;
            uint8 nodeState = player->GetTeamId() == TEAM_ALLIANCE ? NODE_STATE_CONTROLLED_A : NODE_STATE_CONTROLLED_H;

            BattlegroundIC* pIC = static_cast<BattlegroundIC*>(player->GetBattleground());
            if (pIC->GetNodeState(nodeType) == nodeState)
                return true;

            return false;
        }
        case 56618: // Horde Controls Factory Phase Shift
        case 56617: // Alliance Controls Factory Phase Shift
        {
            if (!player)
                return false;

            Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());

            if (!bf || bf->GetTypeId() != BATTLEFIELD_WG)
                return false;

            // team that controls the workshop in the specified area
            uint32 team = bf->GetData(newArea);

            if (team == TEAM_HORDE)
                return spellId == 56618;
            else if (team == TEAM_ALLIANCE)
                return spellId == 56617;
            break;
        }
        case 57940: // Essence of Wintergrasp - Northrend
        case 58045: // Essence of Wintergrasp - Wintergrasp
        {
            if (!player)
                return false;

            if (Battlefield* battlefieldWG = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
                return battlefieldWG->IsEnabled() && (player->GetTeamId() == battlefieldWG->GetDefenderTeam()) && !battlefieldWG->IsWarTime();
            break;
        }
        case 74411: // Battleground - Dampening
        {
            if (!player)
                return false;

            if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
                return bf->IsWarTime();
            break;
        }
    }

    return true;
}

void SpellMgr::LoadSpellRanks()
{
    uint32 oldMSTime = getMSTime();

    // cleanup core data before reload - remove reference to ChainNode from SpellInfo
    for (SpellChainMap::iterator itr = mSpellChains.begin(); itr != mSpellChains.end(); ++itr)
    {
        mSpellInfoMap[itr->first]->ChainEntry = NULL;
    }
    mSpellChains.clear();
    //                                                     0             1      2
    QueryResult result = WorldDatabase.Query("SELECT first_spell_id, spell_id, rank from spell_ranks ORDER BY first_spell_id, rank");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell rank records. DB table `spell_ranks` is empty.");

        return;
    }

    uint32 count = 0;
    bool finished = false;

    do
    {
        // spellid, rank
        std::list < std::pair < int32, int32 > > rankChain;
        int32 currentSpell = -1;
        int32 lastSpell = -1;

        // fill one chain
        while (currentSpell == lastSpell && !finished)
        {
            Field* fields = result->Fetch();

            currentSpell = fields[0].GetUInt32();
            if (lastSpell == -1)
                lastSpell = currentSpell;
            uint32 spell_id = fields[1].GetUInt32();
            uint32 rank = fields[2].GetUInt8();

            // don't drop the row if we're moving to the next rank
            if (currentSpell == lastSpell)
            {
                rankChain.push_back(std::make_pair(spell_id, rank));
                if (!result->NextRow())
                    finished = true;
            }
            else
                break;
        }
        // check if chain is made with valid first spell
        SpellInfo const* first = GetSpellInfo(lastSpell);
        if (!first)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell rank identifier(first_spell_id) %u listed in `spell_ranks` does not exist!", lastSpell);
            continue;
        }
        // check if chain is long enough
        if (rankChain.size() < 2)
        {
            sLog->outError(LOG_FILTER_SQL, "There is only 1 spell rank for identifier(first_spell_id) %u in `spell_ranks`, entry is not needed!", lastSpell);
            continue;
        }
        int32 curRank = 0;
        bool valid = true;
        // check spells in chain
        for (std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin(); itr!= rankChain.end(); ++itr)
        {
            SpellInfo const* spell = GetSpellInfo(itr->first);
            if (!spell)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u (rank %u) listed in `spell_ranks` for chain %u does not exist!", itr->first, itr->second, lastSpell);
                valid = false;
                break;
            }
            ++curRank;
            if (itr->second != curRank)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u (rank %u) listed in `spell_ranks` for chain %u does not have proper rank value(should be %u)!", itr->first, itr->second, lastSpell, curRank);
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;
        int32 prevRank = 0;
        // insert the chain
        std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin();
        do
        {
            ++count;
            int32 addedSpell = itr->first;
            mSpellChains[addedSpell].first = GetSpellInfo(lastSpell);
            mSpellChains[addedSpell].last = GetSpellInfo(rankChain.back().first);
            mSpellChains[addedSpell].rank = itr->second;
            mSpellChains[addedSpell].prev = GetSpellInfo(prevRank);
            mSpellInfoMap[addedSpell]->ChainEntry = &mSpellChains[addedSpell];
            prevRank = addedSpell;
            ++itr;
            if (itr == rankChain.end())
            {
                mSpellChains[addedSpell].next = NULL;
                break;
            }
            else
                mSpellChains[addedSpell].next = GetSpellInfo(itr->first);
        }
        while (true);
    } while (!finished);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell rank records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellRequired()
{
    uint32 oldMSTime = getMSTime();

    mSpellsReqSpell.clear();                                   // need for reload case
    mSpellReq.clear();                                         // need for reload case

    //                                                   0        1
    QueryResult result = WorldDatabase.Query("SELECT spell_id, req_spell from spell_required");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell required records. DB table `spell_required` is empty.");

        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();
        uint32 spell_req = fields[1].GetUInt32();

        // check if chain is made with valid first spell
        SpellInfo const* spell = GetSpellInfo(spell_id);
        if (!spell)
        {
            sLog->outError(LOG_FILTER_SQL, "spell_id %u in `spell_required` table is not found in dbcs, skipped", spell_id);
            continue;
        }

        SpellInfo const* req_spell = GetSpellInfo(spell_req);
        if (!req_spell)
        {
            sLog->outError(LOG_FILTER_SQL, "req_spell %u in `spell_required` table is not found in dbcs, skipped", spell_req);
            continue;
        }

        if (GetFirstSpellInChain(spell_id) == GetFirstSpellInChain(spell_req))
        {
            sLog->outError(LOG_FILTER_SQL, "req_spell %u and spell_id %u in `spell_required` table are ranks of the same spell, entry not needed, skipped", spell_req, spell_id);
            continue;
        }

        if (IsSpellRequiringSpell(spell_id, spell_req))
        {
            sLog->outError(LOG_FILTER_SQL, "duplicated entry of req_spell %u and spell_id %u in `spell_required`, skipped", spell_req, spell_id);
            continue;
        }

        mSpellReq.insert (std::pair<uint32, uint32>(spell_id, spell_req));
        mSpellsReqSpell.insert (std::pair<uint32, uint32>(spell_req, spell_id));
        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell required records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellLearnSkills()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSkills.clear();                              // need for reload case

    // search auto-learned skills and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);

        if (!entry)
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_SKILL)
            {
                SpellLearnSkillNode dbc_node;
                dbc_node.skill = entry->Effects[i].MiscValue;
                dbc_node.step  = entry->Effects[i].CalcValue();
                if (dbc_node.skill != SKILL_RIDING)
                    dbc_node.value = 1;
                else
                    dbc_node.value = dbc_node.step * 75;
                dbc_node.maxvalue = dbc_node.step * 75;
                mSpellLearnSkills[spell] = dbc_node;
                ++dbc_count;
                break;
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Spell Learn Skills from DBC in %u ms", dbc_count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellLearnSpells()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSpells.clear();                              // need for reload case

    //                                                  0      1        2
    QueryResult result = WorldDatabase.Query("SELECT entry, SpellID, Active FROM spell_learn_spell");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell learn spells. DB table `spell_learn_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();

        SpellLearnSpellNode node;
        node.spell       = fields[1].GetUInt32();
        node.overridesSpell = 0;
        node.active      = fields[2].GetBool();
        node.autoLearned = false;

        if (!GetSpellInfo(spell_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_learn_spell` does not exist", spell_id);
            WorldDatabase.PExecute("DELETE FROM `spell_learn_spell` WHERE entry = %u", spell_id);
            continue;
        }

        if (!GetSpellInfo(node.spell))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_learn_spell` learning not existed spell %u", spell_id, node.spell);
            WorldDatabase.PExecute("DELETE FROM `spell_learn_spell` WHERE SpellID = %u", node.spell);
            continue;
        }

        mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell_id, node));

        ++count;
    } while (result->NextRow());

    // copy state loaded from db
    SpellLearnSpellMap dbSpellLearnSpells = mSpellLearnSpells;

    // search auto-learned spells and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);
        if (!entry)
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL)
            {
                SpellLearnSpellNode dbc_node;
                dbc_node.spell = entry->Effects[i].TriggerSpell;
                dbc_node.active = true;                     // all dbc based learned spells is active (show in spell book or hide by client itself)
                dbc_node.overridesSpell = 0;

                // ignore learning not existed spells (broken/outdated/or generic learnig spell 483
                if (!GetSpellInfo(dbc_node.spell))
                    continue;

                // talent or passive spells or skill-step spells auto-casted and not need dependent learning,
                // pet teaching spells must not be dependent learning (casted)
                // other required explicit dependent learning
                dbc_node.autoLearned = entry->Effects[i].TargetA.GetTarget() == TARGET_UNIT_PET || entry->IsPassive() || entry->HasEffect(SPELL_EFFECT_SKILL_STEP);

                SpellLearnSpellMapBounds db_node_bounds = dbSpellLearnSpells.equal_range(spell);

                bool found = false;
                for (SpellLearnSpellMap::const_iterator itr = db_node_bounds.first; itr != db_node_bounds.second; ++itr)
                {
                    if (itr->second.spell == dbc_node.spell)
                    {
                        sLog->outError(LOG_FILTER_SQL, "Spell %u auto-learn spell %u in spell.dbc then the record in `spell_learn_spell` is redundant, please fix DB.",
                            spell, dbc_node.spell);
                        found = true;
                        break;
                    }
                }

                if (!found)                                  // add new spell-spell pair if not found
                {
                    mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell, dbc_node));
                    ++dbc_count;
                }
            }
        }
    }

    for (SpellLearnSpellEntry const* spellLearnSpell : sSpellLearnSpellStore)
    {
        if (!GetSpellInfo(spellLearnSpell->SpellID))
            continue;

        SpellLearnSpellMapBounds db_node_bounds = dbSpellLearnSpells.equal_range(spellLearnSpell->LearnSpellID);
        bool found = false;
        for (SpellLearnSpellMap::const_iterator itr = db_node_bounds.first; itr != db_node_bounds.second; ++itr)
        {
            if (itr->second.spell == spellLearnSpell->SpellID)
            {
                sLog->outError(LOG_FILTER_SQL, "Found redundant record (entry: %u, SpellID: %u) in `spell_learn_spell`, spell added automatically from SpellLearnSpell.db2", spellLearnSpell->LearnSpellID, spellLearnSpell->SpellID);
                found = true;
                break;
            }
        }

        if (found)
            continue;

        // Check if it is already found in Spell.dbc, ignore silently if yes
        SpellLearnSpellMapBounds dbc_node_bounds = GetSpellLearnSpellMapBounds(spellLearnSpell->LearnSpellID);
        found = false;
        for (SpellLearnSpellMap::const_iterator itr = dbc_node_bounds.first; itr != dbc_node_bounds.second; ++itr)
        {
            if (itr->second.spell == spellLearnSpell->SpellID)
            {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        SpellLearnSpellNode dbcLearnNode;
        dbcLearnNode.spell = spellLearnSpell->SpellID;
        dbcLearnNode.overridesSpell = spellLearnSpell->OverridesSpellID;
        dbcLearnNode.active = true;
        dbcLearnNode.autoLearned = false;

        mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spellLearnSpell->LearnSpellID, dbcLearnNode));
        ++dbc_count;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell learn spells, %u found in Spell.dbc in %u ms", count, dbc_count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellTargetPositions()
{
    uint32 oldMSTime = getMSTime();

    mSpellTargetPositions.clear();                                // need for reload case

    //                                                0      1              2                  3                  4                  5
    QueryResult result = WorldDatabase.Query("SELECT id, target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM spell_target_position");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell target coordinates. DB table `spell_target_position` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 Spell_ID = fields[0].GetUInt32();

        SpellTargetPosition st;

        st.target_mapId       = fields[1].GetUInt16();
        st.target_X           = fields[2].GetFloat();
        st.target_Y           = fields[3].GetFloat();
        st.target_Z           = fields[4].GetFloat();
        st.target_Orientation = fields[5].GetFloat();

        MapEntry const* mapEntry = sMapStore.LookupEntry(st.target_mapId);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (ID:%u) target map (ID: %u) does not exist in `Map.dbc`.", Spell_ID, st.target_mapId);
            continue;
        }

        if (st.target_X==0 && st.target_Y==0 && st.target_Z==0)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (ID:%u) target coordinates not provided.", Spell_ID);
            continue;
        }

        SpellInfo const* spellInfo = GetSpellInfo(Spell_ID);
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (ID:%u) listed in `spell_target_position` does not exist.", Spell_ID);
            continue;
        }

        bool found = false;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (spellInfo->Effects[i].TargetA.GetObjectType() == TARGET_OBJECT_TYPE_DEST || spellInfo->Effects[i].TargetB.GetObjectType() == TARGET_OBJECT_TYPE_DEST
            || spellInfo->Effects[i].TargetA.GetObjectType() == TARGET_OBJECT_TYPE_OBJ_AND_DEST || spellInfo->Effects[i].TargetB.GetObjectType() == TARGET_OBJECT_TYPE_OBJ_AND_DEST)
            {
                // additional requirements
                if (spellInfo->Effects[i].Effect == SPELL_EFFECT_BIND && spellInfo->Effects[i].MiscValue)
                {
                    uint32 area_id = sMapMgr->GetAreaId(st.target_mapId, st.target_X, st.target_Y, st.target_Z);
                    if (area_id != uint32(spellInfo->Effects[i].MiscValue))
                    {
                        sLog->outError(LOG_FILTER_SQL, "Spell (Id: %u) listed in `spell_target_position` expected point to zone %u bit point to zone %u.", Spell_ID, spellInfo->Effects[i].MiscValue, area_id);
                        break;
                    }
                }

                found = true;
                break;
            }
        }
        if (!found && !spellInfo->CastingReq.RequiresSpellFocus)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (Id: %u) listed in `spell_target_position` does not have target with Object Type = TARGET_OBJECT_TYPE_DEST .", Spell_ID);
            continue;
        }

        mSpellTargetPositions[Spell_ID] = st;
        ++count;

    } while (result->NextRow());

    /*
    // Check all spells
    for (uint32 i = 1; i < GetSpellInfoStoreSize; ++i)
    {
    SpellInfo const* spellInfo = GetSpellInfo(i);
    if (!spellInfo)
    continue;

    bool found = false;
    for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
    {
    switch (spellInfo->Effects[j].TargetA)
    {
    case TARGET_DEST_DB:
    found = true;
    break;
    }
    if (found)
    break;
    switch (spellInfo->Effects[j].TargetB)
    {
    case TARGET_DEST_DB:
    found = true;
    break;
    }
    if (found)
    break;
    }
    if (found)
    {
    if (!sSpellMgr->GetSpellTargetPosition(i))
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell (ID: %u) does not have record in `spell_target_position`", i);
    }
    }*/

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell teleport coordinates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellGroups()
{
    uint32 oldMSTime = getMSTime();

    mSpellSpellGroup.clear();                                  // need for reload case
    mSpellGroupSpell.clear();

    //                                                0     1
    QueryResult result = WorldDatabase.Query("SELECT id, spell_id FROM spell_group");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell group definitions. DB table `spell_group` is empty.");
        return;
    }

    std::set<uint32> groups;
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        if (group_id <= SPELL_GROUP_DB_RANGE_MIN && group_id >= SPELL_GROUP_CORE_RANGE_MAX)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellGroup id %u listed in `spell_group` is in core range, but is not defined in core!", group_id);
            continue;
        }
        int32 spell_id = fields[1].GetInt32();

        groups.insert(std::set<uint32>::value_type(group_id));
        mSpellGroupSpell.insert(SpellGroupSpellMap::value_type((SpellGroup)group_id, spell_id));

    } while (result->NextRow());

    for (SpellGroupSpellMap::iterator itr = mSpellGroupSpell.begin(); itr!= mSpellGroupSpell.end();)
    {
        if (itr->second < 0)
        {
            if (groups.find(abs(itr->second)) == groups.end())
            {
                sLog->outError(LOG_FILTER_SQL, "SpellGroup id %u listed in `spell_group` does not exist", abs(itr->second));
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
        else
        {
            SpellInfo const* spellInfo = GetSpellInfo(itr->second);

            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_group` does not exist", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else if (spellInfo->GetRank() > 1)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_group` is not first rank of spell", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
    }

    for (std::set<uint32>::iterator groupItr = groups.begin(); groupItr != groups.end(); ++groupItr)
    {
        std::set<uint32> spells;
        GetSetOfSpellsInSpellGroup(SpellGroup(*groupItr), spells);

        for (std::set<uint32>::iterator spellItr = spells.begin(); spellItr != spells.end(); ++spellItr)
        {
            ++count;
            mSpellSpellGroup.insert(SpellSpellGroupMap::value_type(*spellItr, SpellGroup(*groupItr)));
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell group definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellGroupStackRules()
{
    uint32 oldMSTime = getMSTime();

    mSpellGroupStack.clear();                                  // need for reload case

    //                                                       0         1
    QueryResult result = WorldDatabase.Query("SELECT group_id, stack_rule FROM spell_group_stack_rules");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell group stack rules. DB table `spell_group_stack_rules` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        uint8 stack_rule = fields[1].GetInt8();
        if (stack_rule >= SPELL_GROUP_STACK_RULE_MAX)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellGroupStackRule %u listed in `spell_group_stack_rules` does not exist", stack_rule);
            continue;
        }

        SpellGroupSpellMapBounds spellGroup = GetSpellGroupSpellMapBounds((SpellGroup)group_id);

        if (spellGroup.first == spellGroup.second)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellGroup id %u listed in `spell_group_stack_rules` does not exist", group_id);
            continue;
        }

        mSpellGroupStack[(SpellGroup)group_id] = (SpellGroupStackRule)stack_rule;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell group stack rules in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadForbiddenSpells()
{
    uint32 oldMSTime = getMSTime();

    mForbiddenSpells.clear();

    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT spell_id FROM spell_forbidden");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell group definitions", count);
        return;
    }

    do
    {
        Field *fields = result->Fetch();
    
        mForbiddenSpells.push_back(fields[0].GetUInt32());

        ++count;
    } while (result->NextRow());


    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u forbidden spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}


void SpellMgr::LoadSpellProcEvents()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcEventMap.clear();                             // need for reload case

    //                                               0      1           2                3                 4                 5                 6                 7          8       9        10            11        12
    QueryResult result = WorldDatabase.Query("SELECT entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, SpellFamilyMask3, procFlags, procEx, ppmRate, CustomChance, Cooldown, effectmask FROM spell_proc_event");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell proc event conditions. DB table `spell_proc_event` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 customProc = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        SpellInfo const* spell = GetSpellInfo(entry);
        if (!spell)
        {
            WorldDatabase.PExecute("DELETE FROM spell_proc_event WHERE entry = %u;", entry);
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc_event` does not exist", entry);
            continue;
        }

        SpellProcEventEntry spe;

        spe.schoolMask      = fields[1].GetInt8();
        spe.spellFamilyName = fields[2].GetUInt16();
        spe.spellFamilyMask[0] = fields[3].GetUInt32();
        spe.spellFamilyMask[1] = fields[4].GetUInt32();
        spe.spellFamilyMask[2] = fields[5].GetUInt32();
        spe.spellFamilyMask[3] = fields[6].GetUInt32();
        spe.procFlags       = fields[7].GetUInt32();
        spe.procEx          = fields[8].GetUInt32();
        spe.ppmRate         = fields[9].GetFloat();
        spe.customChance    = fields[10].GetFloat();
        spe.cooldown        = fields[11].GetFloat();
        spe.effectMask        = fields[12].GetUInt32();

        mSpellProcEventMap[entry].push_back(spe);

        if (spell->AuraOptions.ProcTypeMask == 0)
        {
            if (spe.procFlags == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc_event` probally not triggered spell", entry);
                continue;
            }
            customProc++;
        }
        ++count;
    } while (result->NextRow());

    if (customProc)
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u extra and %u custom spell proc event conditions in %u ms",  count, customProc, GetMSTimeDiffToNow(oldMSTime));
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u extra spell proc event conditions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellProcs()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcMap.clear();                             // need for reload case

    //                                               0        1           2                3                 4                 5                 6                 7         8              9               10       11              12             13      14        15       16
    QueryResult result = WorldDatabase.Query("SELECT spellId, schoolMask, spellFamilyName, spellFamilyMask0, spellFamilyMask1, spellFamilyMask2, spellFamilyMask3, typeMask, spellTypeMask, spellPhaseMask, hitMask, attributesMask, ratePerMinute, chance, cooldown, charges, modcharges FROM spell_proc");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell proc conditions and data. DB table `spell_proc` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();

        bool allRanks = false;
        if (spellId <=0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellEntry = GetSpellInfo(spellId);
        if (!spellEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc` does not exist", spellId);
            continue;
        }

        if (allRanks)
        {
            if (GetFirstSpellInChain(spellId) != uint32(spellId))
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc` is not first rank of spell.", fields[0].GetInt32());
                continue;
            }
        }

        SpellProcEntry baseProcEntry;

        baseProcEntry.schoolMask      = fields[1].GetInt8();
        baseProcEntry.spellFamilyName = fields[2].GetUInt16();
        baseProcEntry.spellFamilyMask[0] = fields[3].GetUInt32();
        baseProcEntry.spellFamilyMask[1] = fields[4].GetUInt32();
        baseProcEntry.spellFamilyMask[2] = fields[5].GetUInt32();
        baseProcEntry.spellFamilyMask[3] = fields[6].GetUInt32();
        baseProcEntry.typeMask        = fields[7].GetUInt32();
        baseProcEntry.spellTypeMask   = fields[8].GetUInt32();
        baseProcEntry.spellPhaseMask  = fields[9].GetUInt32();
        baseProcEntry.hitMask         = fields[10].GetUInt32();
        baseProcEntry.attributesMask  = fields[11].GetUInt32();
        baseProcEntry.ratePerMinute   = fields[12].GetFloat();
        baseProcEntry.chance          = fields[13].GetFloat();
        float cooldown                = fields[14].GetFloat();
        baseProcEntry.cooldown        = uint32(cooldown);
        baseProcEntry.charges         = fields[15].GetUInt32();
        baseProcEntry.modcharges      = fields[16].GetUInt32();

        while (true)
        {
            if (mSpellProcMap.find(spellId) != mSpellProcMap.end())
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc` has duplicate entry in the table", spellId);
                break;
            }
            SpellProcEntry procEntry = SpellProcEntry(baseProcEntry);

            // take defaults from dbcs
            if (!procEntry.typeMask)
                procEntry.typeMask = spellEntry->AuraOptions.ProcTypeMask;
            if (!procEntry.charges)
                procEntry.charges = spellEntry->AuraOptions.ProcCharges;
            if (!procEntry.chance && !procEntry.ratePerMinute)
                procEntry.chance = float(spellEntry->AuraOptions.ProcChance);

            // validate data
            if (procEntry.schoolMask & ~SPELL_SCHOOL_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `schoolMask` set: %u", spellId, procEntry.schoolMask);
            if (procEntry.spellFamilyName && (procEntry.spellFamilyName < 3 || procEntry.spellFamilyName > 17 || procEntry.spellFamilyName == 14 || procEntry.spellFamilyName == 16))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `spellFamilyName` set: %u", spellId, procEntry.spellFamilyName);
            if (procEntry.chance < 0)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has negative value in `chance` field", spellId);
                procEntry.chance = 0;
            }
            if (procEntry.ratePerMinute < 0)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has negative value in `ratePerMinute` field", spellId);
                procEntry.ratePerMinute = 0;
            }
            if (cooldown < 0)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has negative value in `cooldown` field", spellId);
                procEntry.cooldown = 0;
            }
            if (procEntry.chance == 0 && procEntry.ratePerMinute == 0)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u doesn't have `chance` and `ratePerMinute` values defined, proc will not be triggered", spellId);
            if (procEntry.charges > 99)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has too big value in `charges` field", spellId);
                procEntry.charges = 99;
            }
            if (!procEntry.typeMask)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u doesn't have `typeMask` value defined, proc will not be triggered", spellId);
            if (procEntry.spellTypeMask & ~PROC_SPELL_PHASE_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `spellTypeMask` set: %u", spellId, procEntry.spellTypeMask);
            if (procEntry.spellTypeMask && !(procEntry.typeMask & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK)))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has `spellTypeMask` value defined, but it won't be used for defined `typeMask` value", spellId);
            if (!procEntry.spellPhaseMask && procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u doesn't have `spellPhaseMask` value defined, but it's required for defined `typeMask` value, proc will not be triggered", spellId);
            if (procEntry.spellPhaseMask & ~PROC_SPELL_PHASE_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `spellPhaseMask` set: %u", spellId, procEntry.spellPhaseMask);
            if (procEntry.spellPhaseMask && !(procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has `spellPhaseMask` value defined, but it won't be used for defined `typeMask` value", spellId);
            if (procEntry.hitMask & ~PROC_HIT_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `hitMask` set: %u", spellId, procEntry.hitMask);
            if (procEntry.hitMask && !(procEntry.typeMask & TAKEN_HIT_PROC_FLAG_MASK || (procEntry.typeMask & DONE_HIT_PROC_FLAG_MASK && (!procEntry.spellPhaseMask || procEntry.spellPhaseMask & (PROC_SPELL_PHASE_HIT | PROC_SPELL_PHASE_FINISH)))))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has `hitMask` value defined, but it won't be used for defined `typeMask` and `spellPhaseMask` values", spellId);

            mSpellProcMap[spellId] = procEntry;

            if (allRanks)
            {
                spellId = GetNextSpellInChain(spellId);
                spellEntry = GetSpellInfo(spellId);
            }
            else
                break;
        }
        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell proc conditions and data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellBonusess()
{
    uint32 oldMSTime = getMSTime();

    mSpellBonusMap.clear();                             // need for reload case

    //                                                0      1             2          3         4                   5           6
    QueryResult result = WorldDatabase.Query("SELECT entry, direct_bonus, dot_bonus, ap_bonus, ap_dot_bonus, damage_bonus, heal_bonus FROM spell_bonus_data");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell bonus data. DB table `spell_bonus_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();

        SpellInfo const* spell = GetSpellInfo(entry);
        if (!spell)
        {
            WorldDatabase.PExecute("DELETE FROM spell_bonus_data WHERE entry = %u;", entry);
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_bonus_data` does not exist", entry);
            continue;
        }

        SpellBonusEntry& sbe = mSpellBonusMap[entry];
        sbe.direct_damage = fields[1].GetFloat();
        sbe.dot_damage    = fields[2].GetFloat();
        sbe.ap_bonus      = fields[3].GetFloat();
        sbe.ap_dot_bonus   = fields[4].GetFloat();
        sbe.damage_bonus   = fields[5].GetFloat();
        sbe.heal_bonus   = fields[6].GetFloat();

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u extra spell bonus data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellThreats()
{
    uint32 oldMSTime = getMSTime();

    mSpellThreatMap.clear();                                // need for reload case

    //                                                0      1        2       3
    QueryResult result = WorldDatabase.Query("SELECT entry, flatMod, pctMod, apPctMod FROM spell_threat");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 aggro generating spells. DB table `spell_threat` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        if (!GetSpellInfo(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_threat` does not exist", entry);
            continue;
        }

        SpellThreatEntry ste;
        ste.flatMod  = fields[1].GetInt32();
        ste.pctMod   = fields[2].GetFloat();
        ste.apPctMod = fields[3].GetFloat();

        mSpellThreatMap[entry] = ste;
        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u SpellThreatEntries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSkillLineAbilityMap()
{
    uint32 oldMSTime = getMSTime();

    mSkillLineAbilityMap.clear();

    uint32 count = 0;

    for (SkillLineAbilityEntry const* SkillInfo : sSkillLineAbilityStore)
    {
        mSkillLineAbilityMap.insert(SkillLineAbilityMap::value_type(SkillInfo->SpellID, SkillInfo));
        ++count;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u SkillLineAbility MultiMap Data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellPetAuras()
{
    uint32 oldMSTime = getMSTime();

    mSpellPetAuraMap.clear();                                  // need for reload case

    //                                                    0          1         2         3            4         5      6     7        8         9              10            11
    QueryResult result = WorldDatabase.Query("SELECT `petEntry`, `spellId`, `option`, `target`, `targetaura`, `bp0`, `bp1`, `bp2`, `aura`, `casteraura`, `createdspell`, `fromspell` FROM `spell_pet_auras`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell pet auras. DB table `spell_pet_auras` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 petEntry = fields[0].GetInt32();
        int32 spellId = fields[1].GetInt32();
        int32 option = fields[2].GetInt32();
        int32 target = fields[3].GetInt32();
        int32 targetaura = fields[4].GetInt32();
        float bp0 = fields[5].GetFloat();
        float bp1 = fields[6].GetFloat();
        float bp2 = fields[7].GetFloat();
        int32 aura = fields[8].GetInt32();
        int32 casteraura = fields[9].GetInt32();
        int32 createdspell = fields[10].GetInt32();
        int32 fromspell = fields[11].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_pet_auras` does not exist", spellId);
            continue;
        }

        PetAura tempPetAura;
        tempPetAura.petEntry = petEntry;
        tempPetAura.spellId = spellId;
        tempPetAura.option = option;
        tempPetAura.target = target;
        tempPetAura.targetaura = targetaura;
        tempPetAura.bp0 = bp0;
        tempPetAura.bp1 = bp1;
        tempPetAura.bp2 = bp2;
        tempPetAura.aura = aura;
        tempPetAura.casteraura = casteraura;
        tempPetAura.createdspell = createdspell;
        tempPetAura.fromspell = fromspell;
        mSpellPetAuraMap[petEntry].push_back(tempPetAura);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell pet auras in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

// Fill custom data about enchancments
void SpellMgr::LoadEnchantCustomAttr()
{
    uint32 oldMSTime = getMSTime();

    uint32 size = sSpellItemEnchantmentStore.GetNumRows();
    mEnchantCustomAttr.resize(size);

    for (uint32 i = 0; i < size; ++i)
        mEnchantCustomAttr[i] = 0;

    uint32 count = 0;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = GetSpellInfo(i);
        if (!spellInfo)
            continue;

        // TODO: find a better check
        if (!(spellInfo->HasAttribute(SPELL_ATTR2_PRESERVE_ENCHANT_IN_ARENA)) || !(spellInfo->HasAttribute(SPELL_ATTR0_NOT_SHAPESHIFT)))
            continue;

        for (uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
            if (spellInfo->Effects[j].Effect == SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY)
            {
                uint32 enchId = spellInfo->Effects[j].MiscValue;
                if (!sSpellItemEnchantmentStore.LookupEntry(enchId))
                    continue;

                mEnchantCustomAttr[enchId] = true;
                ++count;
                break;
            }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u custom enchant attributes in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellEnchantProcData()
{
    uint32 oldMSTime = getMSTime();

    mSpellEnchantProcEventMap.clear();                             // need for reload case

    //                                                  0         1           2         3
    QueryResult result = WorldDatabase.Query("SELECT entry, customChance, PPMChance, procEx FROM spell_enchant_proc_data");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell enchant proc event conditions. DB table `spell_enchant_proc_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 enchantId = fields[0].GetUInt32();
        if (!sSpellItemEnchantmentStore.LookupEntry(enchantId))
        {
            sLog->outError(LOG_FILTER_SQL, "Enchancment %u listed in `spell_enchant_proc_data` does not exist", enchantId);
            continue;
        }

        SpellEnchantProcEntry spe;
        spe.customChance = fields[1].GetUInt32();
        spe.PPMChance = fields[2].GetFloat();
        spe.procEx = fields[3].GetUInt32();
        mSpellEnchantProcEventMap[enchantId] = spe;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u enchant proc data definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellLinked()
{
    uint32 oldMSTime = getMSTime();

    mSpellLinkedMap.clear();    // need for reload case

    //                                                0              1             2      3       4         5          6         7        8       9         10        11          12         13           14             15          16         17
    QueryResult result = WorldDatabase.Query("SELECT spell_trigger, spell_effect, type, caster, target, hastalent, hastalent2, chance, cooldown, hastype, hitmask, removeMask, hastype2, actiontype, targetCount, targetCountType, `group`, `duration` FROM spell_linked_spell");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 linked spells. DB table `spell_linked_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 trigger = fields[0].GetInt32();
        int32 effect = fields[1].GetInt32();
        int32 type = fields[2].GetUInt8();
        int32 caster = fields[3].GetUInt8();
        int32 target = fields[4].GetUInt8();
        int32 hastalent = fields[5].GetInt32();
        int32 hastalent2 = fields[6].GetInt32();
        int32 chance = fields[7].GetInt32();
        int32 cooldown = fields[8].GetUInt8();
        int32 hastype = fields[9].GetUInt8();
        uint32 hitmask = fields[10].GetUInt32();
        int32 removeMask = fields[11].GetInt32();
        int32 hastype2 = fields[12].GetInt32();
        int32 actiontype = fields[13].GetInt32();
        int8 targetCount = fields[14].GetInt32();
        int8 targetCountType = fields[15].GetInt32();
        int8 group = fields[16].GetInt32();
        int32 duration = fields[17].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(trigger));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_linked_spell` does not exist", trigger);
            WorldDatabase.PExecute("DELETE FROM `spell_linked_spell` WHERE spell_trigger = %i", trigger);
            continue;
        }
        spellInfo = GetSpellInfo(abs(effect));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_linked_spell` does not exist", effect);
            WorldDatabase.PExecute("DELETE FROM `spell_linked_spell` WHERE spell_trigger = %i", trigger);
            continue;
        }

        if (type) //we will find a better way when more types are needed
        {
            if (trigger > 0)
                trigger += SPELL_LINKED_MAX_SPELLS * type;
            else
                trigger -= SPELL_LINKED_MAX_SPELLS * type;
        }
        SpellLinked templink;
        templink.effect = effect;
        templink.hastalent = hastalent;
        templink.hastalent2 = hastalent2;
        templink.chance = chance;
        templink.cooldown = cooldown;
        templink.hastype = hastype;
        templink.caster = caster;
        templink.target = target;
        templink.hitmask = hitmask;
        templink.removeMask = removeMask;
        templink.hastype2 = hastype2;
        templink.actiontype = actiontype;
        templink.targetCount = targetCount;
        templink.targetCountType = targetCountType;
        templink.group = group;
        templink.duration = duration;
        mSpellLinkedMap[trigger].push_back(templink);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u linked spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadTalentSpellLinked()
{
    uint32 oldMSTime = getMSTime();

    mSpellTalentLinkedMap.clear();    // need for reload case

    //                                                  0        1        2       3       4
    QueryResult result = WorldDatabase.Query("SELECT spellid, spelllink, type, target, caster FROM spell_talent_linked_spell");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 linked talent spells. DB table `spell_talent_linked_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 talent = fields[0].GetInt32();
        int32 triger = fields[1].GetInt32();
        int32 type   = fields[2].GetUInt8();
        int32 target = fields[3].GetUInt8();
        int32 caster = fields[4].GetUInt8();

        SpellInfo const* spellInfo = GetSpellInfo(abs(talent));
        if (!spellInfo && talent)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_talent_linked_spell` does not exist", talent);
            WorldDatabase.PExecute("DELETE FROM `spell_talent_linked_spell` WHERE spellid = %i", talent);
            continue;
        }
        spellInfo = GetSpellInfo(abs(triger));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_talent_linked_spell` does not exist", triger);
            WorldDatabase.PExecute("DELETE FROM `spell_talent_linked_spell` WHERE spelllink = %i", triger);
            continue;
        }

        /*if (type) //we will find a better way when more types are needed
        {
            if (talent > 0)
                talent += SPELL_LINKED_MAX_SPELLS * type;
            else
                talent -= SPELL_LINKED_MAX_SPELLS * type;
        }*/
        SpellTalentLinked templink;
        templink.talent = talent;
        templink.triger = triger;
        templink.type   = type;
        templink.target = target;
        templink.caster = caster;
        mSpellTalentLinkedMap[talent].push_back(templink);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u linked talent spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellConcatenateAura()
{
    uint32 oldMSTime = getMSTime();

    mSpellConcatenateApplyMap.clear();    // need for reload case
    mSpellConcatenateUpdateMap.clear();    // need for reload case

    //                                                   0            1           2            3          4         5         6
    QueryResult result = WorldDatabase.Query("SELECT `spellid`, `effectSpell`, `auraId`, `effectAura`, `caster`, `target`, `option` FROM spell_concatenate_aura");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 concatenate auraspells. DB table `spell_concatenate_aura` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spellid = fields[0].GetInt32();
        int32 effectSpell = fields[1].GetUInt8();
        int32 auraId   = fields[2].GetInt32();
        int32 effectAura = fields[3].GetUInt8();
        int8 caster = fields[4].GetUInt8();
        int8 target = fields[5].GetUInt8();
        uint32 option = fields[6].GetUInt8();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellid));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_concatenate_aura` does not exist", spellid);
            continue;
        }
        spellInfo = GetSpellInfo(abs(auraId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_concatenate_aura` does not exist", auraId);
            continue;
        }

        SpellConcatenateAura tempAura;
        tempAura.spellid = spellid;
        tempAura.effectSpell = effectSpell;
        tempAura.auraId   = auraId;
        tempAura.effectAura = effectAura;
        tempAura.caster = caster;
        tempAura.target = target;
        tempAura.option = option;
        mSpellConcatenateUpdateMap[spellid].push_back(tempAura);
        mSpellConcatenateApplyMap[auraId].push_back(tempAura);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u concatenate aura spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellVisual()
{
    uint32 oldMSTime = getMSTime();

    mSpellVisualMap.clear();    // need for reload case
    mSpellVisualPlayOrphanMap.clear();    // need for reload case

    //                                                  0            1            2            3             4            5        6
    QueryResult result = WorldDatabase.Query("SELECT spellId, SpellVisualID, MissReason, ReflectStatus, TravelSpeed, SpeedAsTime, type FROM spell_visual");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 visual spells. DB table `spell_visual` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();
        int32 SpellVisualID = fields[1].GetInt32();
        int32 MissReason   = fields[2].GetUInt16();
        int32 ReflectStatus = fields[3].GetUInt16();
        float TravelSpeed = fields[4].GetFloat();
        bool SpeedAsTime = bool(fields[5].GetUInt8());
        int32 type = fields[6].GetUInt8();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_visual` does not exist", abs(spellId));
            continue;
        }

        SpellVisual templink;
        templink.spellId = spellId;
        templink.SpellVisualID = SpellVisualID;
        templink.MissReason = MissReason;
        templink.ReflectStatus = ReflectStatus;
        templink.TravelSpeed = TravelSpeed;
        templink.SpeedAsTime = SpeedAsTime;
        templink.type = type;
        mSpellVisualMap[spellId].push_back(templink);

        ++count;
    } while (result->NextRow());

    //                                      0            1            2            3          4
    result = WorldDatabase.Query("SELECT spellId, SpellVisualID, TravelSpeed, SpeedAsTime, UnkFloat FROM spell_visual_play_orphan");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 visual spells. DB table `spell_visual_play_orphan` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();
        int32 SpellVisualID = fields[1].GetInt32();
        float TravelSpeed = fields[2].GetFloat();
        bool SpeedAsTime = bool(fields[3].GetUInt8());
        float UnkFloat = fields[4].GetFloat();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_visual_play_orphan` does not exist", abs(spellId));
            continue;
        }

        SpellVisualPlayOrphan templink;
        templink.spellId = spellId;
        templink.SpellVisualID = SpellVisualID;
        templink.TravelSpeed = TravelSpeed;
        templink.SpeedAsTime = SpeedAsTime;
        templink.UnkFloat = UnkFloat;
        mSpellVisualPlayOrphanMap[spellId] = templink;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u visual spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellScene()
{
    uint32 oldMSTime = getMSTime();

    mSpellSceneMap.clear();    // need for reload case

    //                                                        0                1           2            3                4           5
    QueryResult result = WorldDatabase.Query("SELECT SceneScriptPackageID, MiscValue, trigerSpell, MonsterCredit, PlaybackFlags, ScriptName FROM spell_scene");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 visual spells. DB table `spell_scene` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        int8 ind = 0;

        SpellScene templink;
        templink.SceneScriptPackageID = fields[ind++].GetInt32();
        templink.MiscValue = fields[ind++].GetInt32();
        templink.trigerSpell = fields[ind++].GetInt32();
        templink.MonsterCredit = fields[ind++].GetInt32();
        templink.PlaybackFlags = fields[ind++].GetInt32();
        templink.scriptID = sObjectMgr->GetScriptId(fields[ind++].GetString().c_str());

        mSpellSceneMap[templink.MiscValue] = templink;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u visual spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellPendingCast()
{
    uint32 oldMSTime = getMSTime();

    mSpellPendingCastMap.clear();    // need for reload case

    //                                                  0          1          2      3
    QueryResult result = WorldDatabase.Query("SELECT `spell_id`, `pending_id`, `option`, `check` FROM `spell_pending_cast`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 pending spells. DB table `spell_pending_cast` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spell_id = fields[0].GetInt32();
        int32 pending_id = fields[1].GetInt32();
        int8 option   = fields[2].GetUInt8();
        int32 check = fields[3].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spell_id));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "spell_id %u listed in `spell_pending_cast` does not exist", abs(spell_id));
            continue;
        }
        spellInfo = GetSpellInfo(abs(pending_id));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "pending_id %u listed in `spell_pending_cast` does not exist", abs(pending_id));
            continue;
        }


        SpellPendingCast templink;
        templink.spell_id = spell_id;
        templink.pending_id = pending_id;
        templink.option   = option;
        templink.check = check;
        mSpellPendingCastMap[spell_id].push_back(templink);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pending spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadmSpellMountList()
{
    uint32 oldMSTime = getMSTime();

    mSpellMountListMap.clear();    // need for reload case

    //                                                0      1      2       3
    QueryResult result = WorldDatabase.Query("SELECT spell, side, spellS, sideS FROM mount_list");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 linked spells. DB table `mount_list` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spell = fields[0].GetInt32();
        int32 side = fields[1].GetInt32();
        int32 spellS = fields[2].GetUInt32();
        int32 sideS = fields[3].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(spell);
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `mount_list` does not exist", spell);
            continue;
        }
        spellInfo = GetSpellInfo(spellS);
        if (spellS != 0 && !spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `mount_list` does not exist", spellS);
            continue;
        }

        SpellMountList* templist = new SpellMountList;
        templist->spellId = spell;
        templist->side = side;
        templist->spellIdS = spellS;
        templist->sideS = sideS;
        mSpellMountListMap[spell] = templist;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u mount list in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellPrcoCheck()
{
    uint32 oldMSTime = getMSTime();

    mSpellPrcoCheckMap.clear();    // need for reload case

    //                                                0        1       2      3             4         5      6          7           8         9        10       11            12              13          14       15          16           17             18             19
    QueryResult result = WorldDatabase.Query("SELECT entry, entry2, entry3, checkspell, hastalent, chance, target, effectmask, powertype, dmgclass, specId, spellAttr0, targetTypeMask, mechanicMask, fromlevel, perchp, spelltypeMask, combopoints, deathstateMask, hasDuration FROM spell_proc_check");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 proc check spells. DB table `spell_proc_check` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 entry = fields[0].GetInt32();
        int32 entry2 = fields[1].GetInt32();
        int32 entry3 = fields[2].GetInt32();
        int32 checkspell = fields[3].GetInt32();
        int32 hastalent = fields[4].GetInt32();
        int32 chance = fields[5].GetInt32();
        int32 target = fields[6].GetInt32();
        int32 effectmask = fields[7].GetInt32();
        int32 powertype = fields[8].GetInt32();
        int32 dmgclass = fields[9].GetInt32();
        int32 specId = fields[10].GetInt32();
        int32 spellAttr0 = fields[11].GetInt32();
        int32 targetTypeMask = fields[12].GetInt32();
        int32 mechanicMask = fields[13].GetInt32();
        int32 fromlevel = fields[14].GetInt32();
        int32 perchp = fields[15].GetInt32();
        int32 spelltypeMask = fields[16].GetInt32();
        int32 combopoints = fields[17].GetInt32();
        int32 deathstateMask = fields[18].GetInt32();
        int32 hasDuration = fields[19].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(entry));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc_check` does not exist", abs(entry));
            WorldDatabase.PExecute("DELETE FROM `spell_proc_check` WHERE entry = %u", abs(entry));
            continue;
        }

        SpellPrcoCheck templink;
        templink.checkspell = checkspell;
        templink.hastalent = hastalent;
        templink.chance = chance;
        templink.target = target;
        templink.powertype = powertype;
        templink.dmgclass = dmgclass;
        templink.effectmask = effectmask;
        templink.specId = specId;
        templink.spellAttr0 = spellAttr0;
        templink.targetTypeMask = targetTypeMask;
        templink.mechanicMask = mechanicMask;
        templink.fromlevel = fromlevel;
        templink.perchp = perchp;
        templink.spelltypeMask = spelltypeMask;
        templink.combopoints = combopoints;
        templink.deathstateMask = deathstateMask;
        templink.hasDuration = hasDuration;
        mSpellPrcoCheckMap[entry].push_back(templink);
        if(entry2)
            mSpellPrcoCheckMap[entry2].push_back(templink);
        if(entry3)
            mSpellPrcoCheckMap[entry3].push_back(templink);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u proc check spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellTriggered()
{
    uint32 oldMSTime = getMSTime();

    mSpellTriggeredMap.clear();    // need for reload case
    mSpellDummyTriggerMap.clear();    // need for reload case
    mSpellAuraTriggerMap.clear();    // need for reload case
    mSpellAuraDummyMap.clear();    // need for reload case
    mSpellTargetFilterMap.clear();    // need for reload case
    mSpellCheckCastMap.clear();    // need for reload case

    uint32 count = 0;
    //                                                    0           1                    2           3         4          5          6          7      8      9         10         11       12       13         14          15            16            17           18          19           20              21          22
    QueryResult result = WorldDatabase.Query("SELECT `spell_id`, `spell_trigger`, `spell_cooldown`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `group`, `procFlags`, `procEx`, `check_spell_id`, `addptype`, `schoolMask`, `dummyId`, `dummyEffect`, `targetaura2`, `aura2` FROM `spell_trigger`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 triggered spells. DB table `spell_trigger` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spell_id = fields[0].GetInt32();
        int32 spell_trigger = fields[1].GetInt32();
        int32 spell_cooldown = fields[2].GetInt32();
        int32 option = fields[3].GetInt32();
        int32 target = fields[4].GetInt32();
        int32 caster = fields[5].GetInt32();
        int32 targetaura = fields[6].GetInt32();
        float bp0 = fields[7].GetFloat();
        float bp1 = fields[8].GetFloat();
        float bp2 = fields[9].GetFloat();
        int32 effectmask = fields[10].GetInt32();
        int32 aura = fields[11].GetInt32();
        int32 chance = fields[12].GetInt32();
        int32 group = fields[13].GetInt32();
        int32 procFlags = fields[14].GetInt32();
        int32 procEx = fields[15].GetInt32();
        int32 check_spell_id = fields[16].GetInt32();
        int32 addptype = fields[17].GetInt32();
        int32 schoolMask = fields[18].GetInt32();
        int32 dummyId = fields[19].GetInt32();
        int32 dummyEffect = fields[20].GetInt32();
        int32 targetaura2 = fields[21].GetInt32();
        int32 aura2 = fields[22].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spell_id));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell_id %i listed in `spell_trigger` does not exist", spell_id);
            WorldDatabase.PExecute("DELETE FROM `spell_trigger` WHERE spell_id = %i", spell_id);
            continue;
        }
        spellInfo = GetSpellInfo(abs(spell_trigger));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell_trigger %i listed in `spell_trigger` does not exist", spell_trigger);
            WorldDatabase.PExecute("DELETE FROM `spell_trigger` WHERE spell_trigger = %i", spell_trigger);
            continue;
        }
        spellInfo = GetSpellInfo(abs(dummyId));
        if (dummyId && !spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "DummyId %i listed in `spell_trigger` does not exist", dummyId);
            continue;
        }

        SpellTriggered temptrigger;
        temptrigger.spell_id = spell_id;
        temptrigger.spell_trigger = spell_trigger;
        temptrigger.spell_cooldown = spell_cooldown;
        temptrigger.option = option;
        temptrigger.target = target;
        temptrigger.caster = caster;
        temptrigger.targetaura = targetaura;
        temptrigger.bp0 = bp0;
        temptrigger.bp1 = bp1;
        temptrigger.bp2 = bp2;
        temptrigger.effectmask = effectmask;
        temptrigger.aura = aura;
        temptrigger.chance = chance;
        temptrigger.group = group;
        temptrigger.procFlags = procFlags;
        temptrigger.procEx = procEx;
        temptrigger.check_spell_id = check_spell_id;
        temptrigger.addptype = addptype;
        temptrigger.schoolMask = schoolMask;
        temptrigger.dummyId = dummyId;
        temptrigger.dummyEffect = dummyEffect;
        temptrigger.targetaura2 = targetaura2;
        temptrigger.aura2 = aura2;
        mSpellTriggeredMap[spell_id].push_back(temptrigger);

        ++count;
    } while (result->NextRow());


    //                                        0             1             2         3         4          5          6      7      8         9          10       11
    result = WorldDatabase.Query("SELECT `spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance` FROM `spell_dummy_trigger`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 triggered spells. DB table `spell_dummy_trigger` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spell_id = fields[0].GetInt32();
        int32 spell_trigger = fields[1].GetInt32();
        int32 option = fields[2].GetInt32();
        int32 target = fields[3].GetInt32();
        int32 caster = fields[4].GetInt32();
        int32 targetaura = fields[5].GetInt32();
        float bp0 = fields[6].GetFloat();
        float bp1 = fields[7].GetFloat();
        float bp2 = fields[8].GetFloat();
        int32 effectmask = fields[9].GetInt32();
        int32 aura = fields[10].GetInt32();
        int32 chance = fields[11].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spell_id));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_trigger_dummy` does not exist", abs(spell_id));
            //WorldDatabase.PExecute("DELETE FROM `spell_trigger_dummy` WHERE spell_id = %u", abs(spell_id));
            continue;
        }

        SpellDummyTrigger tempDummy;
        tempDummy.spell_id = spell_id;
        tempDummy.spell_trigger = spell_trigger;
        tempDummy.option = option;
        tempDummy.target = target;
        tempDummy.caster = caster;
        tempDummy.targetaura = targetaura;
        tempDummy.bp0 = bp0;
        tempDummy.bp1 = bp1;
        tempDummy.bp2 = bp2;
        tempDummy.effectmask = effectmask;
        tempDummy.aura = aura;
        tempDummy.chance = chance;
        mSpellDummyTriggerMap[spell_id].push_back(tempDummy);

        ++count;
    } while (result->NextRow());


    //                                        0             1             2         3         4          5          6      7      8         9          10       11       12
    result = WorldDatabase.Query("SELECT `spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot` FROM `spell_aura_trigger`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 triggered spells. DB table `spell_aura_trigger` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spell_id = fields[0].GetInt32();
        int32 spell_trigger = fields[1].GetInt32();
        int32 option = fields[2].GetInt32();
        int32 target = fields[3].GetInt32();
        int32 caster = fields[4].GetInt32();
        int32 targetaura = fields[5].GetInt32();
        float bp0 = fields[6].GetFloat();
        float bp1 = fields[7].GetFloat();
        float bp2 = fields[8].GetFloat();
        int32 effectmask = fields[9].GetInt32();
        int32 aura = fields[10].GetInt32();
        int32 chance = fields[11].GetInt32();
        int32 slot = fields[12].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spell_id));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_aura_trigger` does not exist", abs(spell_id));
            //WorldDatabase.PExecute("DELETE FROM `spell_aura_trigger` WHERE spell_id = %u", abs(spell_id));
            continue;
        }

        SpellDummyTrigger tempAura;
        tempAura.spell_id = spell_id;
        tempAura.spell_trigger = spell_trigger;
        tempAura.option = option;
        tempAura.target = target;
        tempAura.caster = caster;
        tempAura.targetaura = targetaura;
        tempAura.bp0 = bp0;
        tempAura.bp1 = bp1;
        tempAura.bp2 = bp2;
        tempAura.effectmask = effectmask;
        tempAura.aura = aura;
        tempAura.chance = chance;
        tempAura.slot = slot;
        mSpellAuraTriggerMap[spell_id].push_back(tempAura);

        ++count;
    } while (result->NextRow());

    //                                        0             1          2         3         4           5              6             7           8        9          10         11        12           13        14       15
    result = WorldDatabase.Query("SELECT `spellId`, `spellDummyId`, `option`, `target`, `caster`, `targetaura`, `effectmask`, `effectDummy`, `aura`, `chance`, `removeAura`, `attr`, `attrValue`, `custombp`, `type`, `charge` FROM `spell_aura_dummy`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 aura dummy spells. DB table `spell_aura_dummy` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();
        int32 spellDummyId = fields[1].GetInt32();
        int32 option = fields[2].GetInt32();
        int32 target = fields[3].GetInt32();
        int32 caster = fields[4].GetInt32();
        int32 targetaura = fields[5].GetInt32();
        int32 effectmask = fields[6].GetInt32();
        int32 effectDummy = fields[7].GetInt32();
        int32 aura = fields[8].GetInt32();
        int32 chance = fields[9].GetInt32();
        int32 removeAura = fields[10].GetInt32();
        int32 attr = fields[11].GetInt32();
        int32 attrValue = fields[12].GetInt32();
        float custombp = fields[13].GetFloat();
        int32 type = fields[14].GetInt32();
        int32 charge = fields[15].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_aura_dummy` does not exist", spellId);
            WorldDatabase.PExecute("DELETE FROM `spell_aura_dummy` WHERE spellId = %i", spellId);
            continue;
        }

        SpellAuraDummy tempdummy;
        tempdummy.spellId = spellId;
        tempdummy.spellDummyId = spellDummyId;
        tempdummy.option = option;
        tempdummy.target = target;
        tempdummy.caster = caster;
        tempdummy.targetaura = targetaura;
        tempdummy.effectmask = effectmask;
        tempdummy.effectDummy = effectDummy;
        tempdummy.aura = aura;
        tempdummy.removeAura = removeAura;
        tempdummy.chance = chance;
        tempdummy.attr = attr;
        tempdummy.attrValue = attrValue;
        tempdummy.custombp = custombp;
        tempdummy.type = type;
        tempdummy.charge = charge;
        mSpellAuraDummyMap[spellId].push_back(tempdummy);

        ++count;
    } while (result->NextRow());

    //                                       0          1          2        3        4           5              6          7         8           9           10          11        12        13
    result = WorldDatabase.Query("SELECT `spellId`, `targetId`, `option`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `param1`, `param2`, `param3` FROM `spell_target_filter`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 aura dummy spells. DB table `spell_target_filter` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();
        int32 targetId = fields[1].GetInt32();
        int32 option = fields[2].GetInt32();
        int32 aura = fields[3].GetInt32();
        int32 chance = fields[4].GetInt32();
        int32 effectMask = fields[5].GetInt32();
        int32 resizeType = fields[6].GetInt32();
        int32 count = fields[7].GetInt32();
        int32 maxcount = fields[8].GetInt32();
        int32 addcount = fields[9].GetInt32();
        int32 addcaster = fields[10].GetInt32();
        float param1 = fields[11].GetFloat();
        float param2 = fields[12].GetFloat();
        float param3 = fields[13].GetFloat();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_target_filter` does not exist", spellId);
            WorldDatabase.PExecute("DELETE FROM `spell_target_filter` WHERE spellId = %i", spellId);
            continue;
        }

        SpellTargetFilter tempfilter;
        tempfilter.spellId = spellId;
        tempfilter.targetId = targetId;
        tempfilter.option = option;
        tempfilter.aura = aura;
        tempfilter.chance = chance;
        tempfilter.effectMask = effectMask;
        tempfilter.resizeType = resizeType;
        tempfilter.count = count;
        tempfilter.maxcount = maxcount;
        tempfilter.addcount = addcount;
        tempfilter.addcaster = addcaster;
        tempfilter.param1 = param1;
        tempfilter.param2 = param2;
        tempfilter.param3 = param3;
        mSpellTargetFilterMap[spellId].push_back(tempfilter);

        ++count;
    } while (result->NextRow());

    //                                       0        1         2             3            4         5           6           7            8            9           10        11        12
    result = WorldDatabase.Query("SELECT `spellId`, `type`, `errorId`, `customErrorId`, `caster`, `target`, `checkType`, `dataType`, `checkType2`, `dataType2`, `param1`, `param2`, `param3` FROM `spell_check_cast`");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 aura dummy spells. DB table `spell_check_cast` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        int32 spellId = fields[0].GetInt32();

        SpellInfo const* spellInfo = GetSpellInfo(abs(spellId));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %i listed in `spell_check_cast` does not exist", spellId);
            WorldDatabase.PExecute("DELETE FROM `spell_check_cast` WHERE spellId = %i", spellId);
            continue;
        }

        SpellCheckCast checkCast;
        checkCast.spellId = spellId;
        checkCast.type = fields[1].GetInt32();
        checkCast.errorId = fields[2].GetInt32();
        checkCast.customErrorId = fields[3].GetInt32();
        checkCast.caster = fields[4].GetInt32();
        checkCast.target = fields[5].GetInt32();
        checkCast.checkType = fields[6].GetInt32();
        checkCast.dataType = fields[7].GetInt32();
        checkCast.checkType2 = fields[8].GetInt32();
        checkCast.dataType2 = fields[9].GetInt32();
        checkCast.param1 = fields[10].GetInt32();
        checkCast.param2 = fields[11].GetInt32();
        checkCast.param3 = fields[12].GetInt32();
        mSpellCheckCastMap[spellId].push_back(checkCast);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u triggered spell in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadPetLevelupSpellMap()
{
     uint32 oldMSTime = getMSTime();

    mPetLevelupSpellMap.clear();                                   // need for reload case

    uint32 count = 0;
    uint32 family_count = 0;

    for (CreatureFamilyEntry const* creatureFamily : sCreatureFamilyStore)
    {
        for (uint8 j = 0; j < 2; ++j)
        {
            if (!creatureFamily->SkillLine[j])
                continue;

            for (SkillLineAbilityEntry const* skillLine : sSkillLineAbilityStore)
            {
                //if (skillLine->skillId != creatureFamily->SkillLine[0] &&
                //    (!creatureFamily->SkillLine[1] || skillLine->skillId != creatureFamily->SkillLine[1]))
                //    continue;

                if (skillLine->SkillLine != creatureFamily->SkillLine[j])
                    continue;

                if (skillLine->AquireMethod != SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN)
                    continue;

                SpellInfo const* spell = GetSpellInfo(skillLine->SpellID);
                if (!spell) // not exist or triggered or talent
                    continue;

                if (!spell->SpellLevel)
                    continue;

                PetLevelupSpellSet& spellSet = mPetLevelupSpellMap[creatureFamily->ID];
                if (spellSet.empty())
                    ++family_count;

                spellSet.insert(PetLevelupSpellSet::value_type(spell->SpellLevel, spell->Id));
                ++count;
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pet levelup and default spells for %u families in %u ms", count, family_count, GetMSTimeDiffToNow(oldMSTime));
}

bool IsCCSpell(SpellInfo const *spellInfo, uint8 EffMask, bool nodamage)
{
    for(uint8 effIndex = 0; effIndex < MAX_SPELL_EFFECTS; ++effIndex)
    {
        if (EffMask && !(EffMask & (1<<effIndex)))
            continue;

        if (nodamage)
        switch(spellInfo->Effects[effIndex].Effect)
        {
                case SPELL_EFFECT_HEALTH_LEECH:
                case SPELL_EFFECT_SCHOOL_DAMAGE:
                    return false;
        }
        switch(spellInfo->Effects[effIndex].ApplyAuraName)
        {
            case SPELL_AURA_MOD_CONFUSE:
            case SPELL_AURA_MOD_FEAR:
            case SPELL_AURA_MOD_FEAR_2:
            case SPELL_AURA_MOD_STUN:
            case SPELL_AURA_MOD_ROOT:
            case SPELL_AURA_MOD_ROOTED:
            case SPELL_AURA_MOD_SILENCE:
            case SPELL_AURA_TRANSFORM:
            case SPELL_AURA_MOD_DISARM:
            case SPELL_AURA_MOD_POSSESS:
                if(!spellInfo->IsPositiveEffect(effIndex))
                    return true;
                break;
            default: break;
        }
    }
    return false;
}

bool LoadPetDefaultSpells_helper(CreatureTemplate const* cInfo, PetDefaultSpellsEntry& petDefSpells)
{
    // skip empty list;
    bool have_spell = false;
    for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if (petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }
    if (!have_spell)
        return false;

    // remove duplicates with levelupSpells if any
    if (PetLevelupSpellSet const* levelupSpells = cInfo->Family ? sSpellMgr->GetPetLevelupSpellList(cInfo->Family) : NULL)
    {
        for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
        {
            if (!petDefSpells.spellid[j])
                continue;

            for (PetLevelupSpellSet::const_iterator itr = levelupSpells->begin(); itr != levelupSpells->end(); ++itr)
            {
                if (itr->second == petDefSpells.spellid[j])
                {
                    petDefSpells.spellid[j] = 0;
                    break;
                }
            }
        }
    }

    // skip empty list;
    have_spell = false;
    for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if (petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }

    return have_spell;
}

void SpellMgr::LoadPetDefaultSpells()
{
    uint32 oldMSTime = getMSTime();

    mPetDefaultSpellsMap.clear();

    uint32 countCreature = 0;
    uint32 countData = 0;

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {
        if (!itr->second.PetSpellDataId)
            continue;

        int32 petSpellsId = -int32(itr->second.PetSpellDataId);
        PetDefaultSpellsEntry petDefSpells;
        for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
            petDefSpells.spellid[j] = itr->second.spells[j];

        if (LoadPetDefaultSpells_helper(&itr->second, petDefSpells))
        {
            mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
            ++countData;
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded addition spells for %u pet spell data entries in %u ms", countData, GetMSTimeDiffToNow(oldMSTime));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading summonable creature templates...");
    oldMSTime = getMSTime();

    // different summon spells
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellEntry = GetSpellInfo(i);
        if (!spellEntry)
            continue;

        for (uint8 k = 0; k < MAX_SPELL_EFFECTS; ++k)
        {
            if (spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON || spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON_PET)
            {
                uint32 creature_id = spellEntry->Effects[k].MiscValue;
                CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature_id);
                if (!cInfo)
                    continue;

                // already loaded
                if (cInfo->PetSpellDataId)
                    continue;

                // for creature without PetSpellDataId get default pet spells from creature_template
                int32 petSpellsId = cInfo->Entry;
                if (mPetDefaultSpellsMap.find(cInfo->Entry) != mPetDefaultSpellsMap.end())
                    continue;

                PetDefaultSpellsEntry petDefSpells;
                for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
                    petDefSpells.spellid[j] = cInfo->spells[j];

                if (LoadPetDefaultSpells_helper(cInfo, petDefSpells))
                {
                    mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
                    ++countCreature;
                }
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u summonable creature templates in %u ms", countCreature, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellAreas()
{
    uint32 oldMSTime = getMSTime();

    mSpellAreaMap.clear();                                  // need for reload case
    mSpellAreaForQuestMap.clear();
    mSpellAreaForActiveQuestMap.clear();
    mSpellAreaForQuestEndMap.clear();
    mSpellAreaForAuraMap.clear();

    //                                                  0     1         2              3               4                 5          6          7       8         9
    QueryResult result = WorldDatabase.Query("SELECT spell, area, quest_start, quest_start_status, quest_end_status, quest_end, aura_spell, racemask, gender, autocast FROM spell_area");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell area requirements. DB table `spell_area` is empty.");

        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        SpellArea spellArea;
        spellArea.spellId             = spell;
        spellArea.areaId              = fields[1].GetInt32();
        spellArea.questStart          = fields[2].GetUInt32();
        spellArea.questStartStatus    = fields[3].GetUInt32();
        spellArea.questEndStatus      = fields[4].GetUInt32();
        spellArea.questEnd            = fields[5].GetUInt32();
        spellArea.auraSpell           = fields[6].GetInt32();
        spellArea.raceMask            = fields[7].GetUInt32();
        spellArea.gender              = Gender(fields[8].GetUInt8());
        spellArea.autocast            = fields[9].GetBool();

        if (SpellInfo const* spellInfo = GetSpellInfo(spell))
        {
            if (spellArea.autocast)
                const_cast<SpellInfo*>(spellInfo)->Misc.Attributes[0] |= SPELL_ATTR0_CANT_CANCEL;
        }
        else
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` does not exist", spell);
            continue;
        }

        {
            bool ok = true;
            SpellAreaMapBounds sa_bounds = GetSpellAreaMapBounds(spellArea.spellId);
            for (SpellAreaMap::const_iterator itr = sa_bounds.first; itr != sa_bounds.second; ++itr)
            {
                if (spellArea.spellId != itr->second.spellId)
                    continue;
                if (spellArea.areaId != itr->second.areaId)
                    continue;
                if (spellArea.questStart != itr->second.questStart)
                    continue;
                if (spellArea.auraSpell != itr->second.auraSpell)
                    continue;
                if ((spellArea.raceMask & itr->second.raceMask) == 0)
                    continue;
                if (spellArea.gender != itr->second.gender)
                    continue;

                // duplicate by requirements
                ok =false;
                break;
            }

            if (!ok)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` already listed with similar requirements.", spell);
                continue;
            }
        }

        if (spellArea.areaId > 0 && !sAreaTableStore.LookupEntry(spellArea.areaId))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong area (%u) requirement", spell, spellArea.areaId);
            continue;
        }

        if (spellArea.areaId < 0 && !sMapStore.LookupEntry(abs(spellArea.areaId)))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong mapid (%u) requirement", spell, abs(spellArea.areaId));
            continue;
        }

        if (spellArea.questStart && !sObjectMgr->GetQuestTemplate(spellArea.questStart))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong start quest (%u) requirement", spell, spellArea.questStart);
            continue;
        }

        if (spellArea.questEnd)
        {
            if (!sObjectMgr->GetQuestTemplate(spellArea.questEnd))
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong end quest (%u) requirement", spell, spellArea.questEnd);
                continue;
            }
        }

        if (spellArea.auraSpell)
        {
            SpellInfo const* spellInfo = GetSpellInfo(abs(spellArea.auraSpell));
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong aura spell (%u) requirement", spell, abs(spellArea.auraSpell));
                continue;
            }

            if (uint32(abs(spellArea.auraSpell)) == spellArea.spellId)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have aura spell (%u) requirement for itself", spell, abs(spellArea.auraSpell));
                continue;
            }

            // not allow autocast chains by auraSpell field (but allow use as alternative if not present)
            if (spellArea.autocast && spellArea.auraSpell > 0)
            {
                bool chain = false;
                SpellAreaForAuraMapBounds saBound = GetSpellAreaForAuraMapBounds(spellArea.spellId);
                for (SpellAreaForAuraMap::const_iterator itr = saBound.first; itr != saBound.second; ++itr)
                {
                    if (itr->second->autocast && itr->second->auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if (chain)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }

                SpellAreaMapBounds saBound2 = GetSpellAreaMapBounds(spellArea.auraSpell);
                for (SpellAreaMap::const_iterator itr2 = saBound2.first; itr2 != saBound2.second; ++itr2)
                {
                    if (itr2->second.autocast && itr2->second.auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if (chain)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }
            }
        }

        if (spellArea.raceMask && (spellArea.raceMask & RACEMASK_ALL_PLAYABLE) == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong race mask (%u) requirement", spell, spellArea.raceMask);
            continue;
        }

        if (spellArea.gender != GENDER_NONE && spellArea.gender != GENDER_FEMALE && spellArea.gender != GENDER_MALE)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong gender (%u) requirement", spell, spellArea.gender);
            continue;
        }

        SpellArea const* sa = &mSpellAreaMap.insert(SpellAreaMap::value_type(spell, spellArea))->second;

        // for search by current zone/subzone at zone/subzone change
        if (spellArea.areaId)
            mSpellAreaForAreaMap.insert(SpellAreaForAreaMap::value_type(spellArea.areaId, sa));

        // for search at quest start/reward
        if (spellArea.questStart)
            mSpellAreaForQuestMap.insert(SpellAreaForQuestMap::value_type(spellArea.questStart, sa));

        // for search at quest start/reward
        if (spellArea.questEnd)
            mSpellAreaForQuestEndMap.insert(SpellAreaForQuestMap::value_type(spellArea.questEnd, sa));

        // for search at aura apply
        if (spellArea.auraSpell)
            mSpellAreaForAuraMap.insert(SpellAreaForAuraMap::value_type(abs(spellArea.auraSpell), sa));

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell area requirements in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellInfoStore()
{
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading SpellInfo store...");

    uint32 oldMSTime = getMSTime();

    std::unordered_map<uint32, SpellVisualMap> visualsBySpell;

    UnloadSpellInfoStore();
    mSpellInfoMap.resize(sSpellStore.GetNumRows(), nullptr);

    for (SpellXSpellVisualEntry const* visual : sSpellXSpellVisualStore)
        visualsBySpell[visual->SpellID][visual->DifficultyID].push_back(visual);

    for (SpellEntry const* spellEntry : sSpellStore)
        mSpellInfoMap[spellEntry->ID] = new SpellInfo(spellEntry, std::move(visualsBySpell[spellEntry->ID]));

    for (SpellPowerEntry const* spellPower : sSpellPowerStore)
    {
        SpellInfo* spell = mSpellInfoMap[spellPower->SpellID];
        if (!spell)
            continue;

        spell->Power.PowerType = spellPower->PowerType;
        spell->Power.PowerCost = spellPower->PowerCost;
        spell->Power.PowerCostPercentage = spellPower->PowerCostPercentage;
        spell->Power.PowerCostPerSecond = spellPower->PowerCostPerSecond;
        spell->Power.PowerCostPercentagePerSecond = spellPower->PowerCostPercentagePerSecond;
        spell->Power.RequiredAura = spellPower->RequiredAura;
        spell->Power.HealthCostPercentage = spellPower->HealthCostPercentage;

        if (!spell->AddPowerData(spellPower))
            sLog->outInfo(LOG_FILTER_WORLDSERVER, "Spell - %u has more powers > 4.", spell->Id);
    }

    for (TalentEntry const* talentInfo : sTalentStore)
        if (SpellInfo* spellEntry = mSpellInfoMap[talentInfo->SpellID])
            spellEntry->talentId = talentInfo->ID;

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded SpellInfo store in %u seconds", GetMSTimeDiffToNow(oldMSTime) / 1000);
}

void SpellMgr::UnloadSpellInfoStore()
{
    for (size_t i = 0; i < mSpellInfoMap.size(); ++i)
        if (mSpellInfoMap[i])
            delete mSpellInfoMap[i];

    mSpellInfoMap.clear();
}

void SpellMgr::UnloadSpellInfoImplicitTargetConditionLists()
{
    for (size_t i = 0; i < mSpellInfoMap.size(); ++i)
        if (mSpellInfoMap[i])
            mSpellInfoMap[i]->_UnloadImplicitTargetConditionLists();
}

void SpellMgr::LoadSpellCustomAttr()
{

    uint32 oldMSTime = getMSTime();

    SpellInfo* spellInfo = NULL;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        {
            spellInfo = mSpellInfoMap[i];
            if (!spellInfo)
                continue;

            for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
            {
                switch (spellInfo->Effects[j].ApplyAuraName)
                {
                    //case SPELL_AURA_MOD_POSSESS:
                    //case SPELL_AURA_MOD_CONFUSE:
                    //case SPELL_AURA_MOD_CHARM:
                    //case SPELL_AURA_AOE_CHARM:
                    //case SPELL_AURA_MOD_FEAR:
                    //case SPELL_AURA_MOD_FEAR_2:
                    case SPELL_AURA_MOD_STUN:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                        break;
                    case SPELL_AURA_PERIODIC_HEAL:
                    case SPELL_AURA_PERIODIC_DAMAGE:
                    case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                    case SPELL_AURA_PERIODIC_LEECH:
                    case SPELL_AURA_PERIODIC_MANA_LEECH:
                    case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                    case SPELL_AURA_PERIODIC_ENERGIZE:
                    case SPELL_AURA_OBS_MOD_HEALTH:
                    case SPELL_AURA_OBS_MOD_POWER:
                    case SPELL_AURA_POWER_BURN:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                        break;
                    case SPELL_AURA_PROC_MELEE_TRIGGER_SPELL:
                        spellInfo->AuraOptions.ProcChance = 100;
                        spellInfo->AuraOptions.ProcTypeMask = PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS | PROC_FLAG_DONE_MELEE_AUTO_ATTACK;
                        break;
                }

                switch (spellInfo->Effects[j].Effect)
                {
                    case SPELL_EFFECT_SCHOOL_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                    case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                    case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                    case SPELL_EFFECT_HEAL:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_DIRECT_DAMAGE;
                        break;
                    case SPELL_EFFECT_POWER_DRAIN:
                    case SPELL_EFFECT_POWER_BURN:
                    case SPELL_EFFECT_HEAL_MAX_HEALTH:
                    case SPELL_EFFECT_HEALTH_LEECH:
                    case SPELL_EFFECT_HEAL_PCT:
                    case SPELL_EFFECT_ENERGIZE_PCT:
                    case SPELL_EFFECT_ENERGIZE:
                    case SPELL_EFFECT_HEAL_MECHANICAL:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                        break;
                    case SPELL_EFFECT_CHARGE:
                    case SPELL_EFFECT_CHARGE_DEST:
                    case SPELL_EFFECT_JUMP:
                    case SPELL_EFFECT_JUMP_DEST:
                    case SPELL_EFFECT_LEAP_BACK:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_CHARGE;
                        break;
                    case SPELL_EFFECT_PICKPOCKET:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_PICKPOCKET;
                        break;
                    case SPELL_EFFECT_ENCHANT_ITEM:
                    case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
                    case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
                    case SPELL_EFFECT_ENCHANT_HELD_ITEM:
                    {
                        // only enchanting profession enchantments procs can stack
                        if (IsPartOfSkillLine(SKILL_ENCHANTING, i))
                        {
                            uint32 enchantId = spellInfo->Effects[j].MiscValue;
                            SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchantId);
                            for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                            {
                                if (enchant->Effect[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                                    continue;

                                SpellInfo* procInfo = (SpellInfo*)GetSpellInfo(enchant->EffectSpellID[s]);
                                if (!procInfo)
                                    continue;

                                // if proced directly from enchantment, not via proc aura
                                // NOTE: Enchant Weapon - Blade Ward also has proc aura spell and is proced directly
                                // however its not expected to stack so this check is good
                                if (procInfo->HasAura(SPELL_AURA_PROC_TRIGGER_SPELL))
                                    continue;

                                procInfo->AttributesCu |= SPELL_ATTR0_CU_ENCHANT_PROC;
                            }
                        }
                        //break;
                    }
                    case SPELL_EFFECT_APPLY_GLYPH:
                        if(sWorld->getBoolConfig(CONFIG_FUN_OPTION_ENABLED))
                        {
                            spellInfo->CastTimes.Minimum = 0;
                            spellInfo->CastTimes.Base = 0;
                        }
                        break;
                    case SPELL_EFFECT_CREATE_ITEM:
                    case SPELL_EFFECT_CREATE_ITEM_2:
                        mSpellCreateItemList.push_back(i);
                        break;
                }
            }

            if (!spellInfo->_IsPositiveEffect(EFFECT_0, false))
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;

            if (!spellInfo->_IsPositiveEffect(EFFECT_1, false))
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF1;

            if (!spellInfo->_IsPositiveEffect(EFFECT_2, false))
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF2;

            if (!spellInfo->_IsPositiveEffect(EFFECT_3, false))
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF3;

            if (!spellInfo->_IsPositiveEffect(EFFECT_4, false))
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF4;

            if (spellInfo->HasEffect(SPELL_EFFECT_APPLY_AREA_AURA_ENEMY))
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_POSITIVE_FOR_CASTER;

            if (spellInfo->GetSpellVisual(DIFFICULTY_NONE) == 3879)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_BACK;

            switch (spellInfo->Id)
            {
                case 45182:  // Cheat Death
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                    break;
                case 83381: // Kill Command
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                    break;
                case 81751: // Atonement
                    spellInfo->Misc.Attributes[2] &= ~SPELL_ATTR2_CANT_CRIT;
                    break;
                case 147303: // Arena Deserter DR (Test Spell)
                    spellInfo->Misc.Attributes[1] |= SPELL_ATTR1_DONT_DISPLAY_IN_AURA_BAR;
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_PASSIVE;
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_HIDE_IN_COMBAT_LOG;
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(42);
                    break;
                case 125761: // Arena Deserter (Test Spell)
                    spellInfo->AttributesCu  = SPELL_ATTR0_CU_NEGATIVE_EFF0;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(3);
                    spellInfo->Misc.Attributes[0] = 0x2D800100;
                    spellInfo->Misc.Attributes[1] = 0x000000A8;
                    spellInfo->Misc.Attributes[2] = 0x00084005;
                    spellInfo->Misc.Attributes[3] = 0x10100000;
                    spellInfo->Misc.Attributes[4] = 0x00000004;
                    spellInfo->Misc.Attributes[5] = 0x00060008;
                    spellInfo->Misc.Attributes[6] = 0x00001000;
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    break;
                case 25956: // Sanctity of Battle
                    spellInfo->Effects[EFFECT_1].MiscValue = SPELLMOD_GLOBAL_COOLDOWN;
                    break;
                case 145591: // Sha Cloud
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_CANT_USED_IN_COMBAT;
                    spellInfo->Misc.Attributes[4] &= ~SPELL_ATTR4_TRIGGERED;
                    break;
                case 137049: // Hotfix Passive 
                    spellInfo->Effects[EFFECT_5].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_5].BasePoints = 100;
                    spellInfo->Effects[EFFECT_5].TargetA = TARGET_UNIT_CASTER;
                    spellInfo->Effects[EFFECT_5].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                    spellInfo->Effects[EFFECT_5].MiscValue = SPELLMOD_DOT;
                    spellInfo->Effects[EFFECT_5].SpellClassMask[1] |= 16;
                    break;
                case 101546: // Spinning Crane Kick
                    spellInfo->Misc.Attributes[0] &= ~SPELL_ATTR0_ABILITY;
                    break;
                case 119032: // Spectral Guise
                    spellInfo->Misc.Attributes[4] |= SPELL_ATTR4_UNK19;
                    break;
                case 10326:  // Turn Evil
                case 145067: // Turn Evil
                    spellInfo->Categories.Mechanic = MECHANIC_FEAR;
                    break;
                case 122470: // Touch of Karma
                    spellInfo->Misc.Attributes[6] &= ~SPELL_ATTR6_NOT_LIMIT_ABSORB;
                    break;
                case 137594: // Fortitude Trigger
                case 137592: // Haste Trigger
                case 137595: // Lightning Strike Charges Trigger
                    spellInfo->Misc.Attributes[4] |= SPELL_ATTR4_NOT_USABLE_IN_ARENA_OR_RATED_BG;
                    break;
                case 115176: // Zen Meditation
                    spellInfo->ChannelInterruptFlags = 0;
                    break;
                case 146025: // Readiness (Prot)
                case 145955: // Readiness (DD Plate)
                case 146019: // Readiness (Other DD)
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_DEATH_PERSISTENT;
                    break;
                case 116706: // Disable (Root)
                    spellInfo->AuraOptions.CumulativeAura = 0;
                    spellInfo->AuraOptions.ProcTypeMask = 0;
                    break;
                case 116023: // Sparring
                case 111397: // Blood Horror
                case 115191: // Stealth
                    spellInfo->AuraInterruptFlags = 0;
                    break;
                case 121471: // Shadow Blades
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_OVERRIDE_AUTOATTACK;
                    break;
                case 148008: // Essence of Yu'lon
                    spellInfo->Misc.Attributes[6] &= ~SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS;
                    break;
                case 51640: // Taunt Flag Targeting
                case 107223: // Sunfire Rays
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    break;
                case 77215:  // Mastery: Potent Afflictions
                    spellInfo->Effects[EFFECT_2].SpellClassMask[1] &= ~2048;
                    break;
                case 138121: // Storm, Earth and Fire
                case 138122: // Storm, Earth and Fire
                case 138123: // Storm, Earth and Fire
                    spellInfo->Effects[EFFECT_0].BasePoints = 2;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    break;
                case 130121: // Item - Scotty's Lucky Coin
                case 26364:  // Lightning Shield
                    spellInfo->Misc.Attributes[4] &= ~SPELL_ATTR4_TRIGGERED;
                    break;
                case 137619: // Marked for Death
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                    spellInfo->Misc.Attributes[3] &= ~SPELL_ATTR3_DEATH_PERSISTENT;
                    break;
                case 53503: // Sword of Light
                    spellInfo->Effects[2].SpellClassMask[2] |= 2097152;
                    break;
                case 146343: // Avoidance
                    spellInfo->Effects[EFFECT_0].MiscValue = SPELL_SCHOOL_MASK_ALL;
                    break;
                case 146051: // Amplification
                    spellInfo->Effects[EFFECT_0].Scaling.Coefficient = 0.00177f;
                    spellInfo->Effects[2].Scaling.Coefficient = 0.00177f;
                    break;
                case 146202: // Wrath
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_HIDE_DURATION;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39);
                    break;
                case 108371: // Harvest Life
                    spellInfo->Effects[EFFECT_0].MiscValue = SPELLMOD_DOT;
                    break;
                case 74434: // Soulburn
                    spellInfo->Effects[EFFECT_1].SpellClassMask[0] |= 33024;
                    spellInfo->AuraOptions.ProcCharges = 1;
                    break;
                case 81269: // Efflorescence
                    spellInfo->Effects[EFFECT_0].Scaling.Coefficient = 1.5309f;
                    spellInfo->Misc.Attributes[2] |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                    break;
                case 132464: // Chi Wave (Pos)
                case 121093: // Monk - Gift of the Naaru
                    spellInfo->ClassOptions.SpellClassSet = SPELLFAMILY_MONK;
                    break;
                case 145640: // Chi Brew
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CASTER;
                    break;
                case 58095: // Glyph of Mystic Shout
                case 146631: // Glyph of Hemorrhaging Veins
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED;
                    break;
                case 124271: // Sanguinary Vein
                case 116000:
                case 103785: // Black Blood of the Earth dmg
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                    break;
                case 108366: // Soul Leech
                case 143597: //Generate rage energize
                case 114714:// Grilled Plainshawk Leg
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                    break;
                case 108942: // Phantasm
                    spellInfo->Misc.Attributes[3] &= ~SPELL_ATTR3_DISABLE_PROC;
                    break;
                case 1943:  // Rupture
                case 2818:  // Deadly Poison
                case 703:   // Garrote
                case 89775: // Hemo
                    spellInfo->Misc.Attributes[4] |= SPELL_ATTR4_DAMAGE_DOESNT_BREAK_AURAS;
                    break;
                case 83968: // Mass Resurrection
                case 6203:  // Soulstone
                case 2641:  // Dismiss Pet
                    spellInfo->Misc.Attributes[2] |= SPELL_ATTR2_CAN_TARGET_DEAD;
                    break;
                case 125883: // Zen Flight
                    spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_MELEE_ATTACK;
                    spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_DIRECT_DAMAGE;
                    break;
                case 31665: // Master of subtlety
                    spellInfo->Effects[EFFECT_0].BasePoints = 10;
                    break;
                case 115834: // Shroud of Concealment
                    spellInfo->Misc.Attributes[1] |= SPELL_ATTR1_CANT_TARGET_IN_COMBAT;
                    break;
                case 1850:   // Dash
                case 113636: // Cat Form (Passive)
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_SPEED;
                    break;
                case 9005: // Pounce
                    spellInfo->Categories.Mechanic = MECHANIC_STUN;
                    break;
                case 58372: // Glyph of Rude Interruption
                    spellInfo->AuraOptions.ProcChance = 0;
                    break;
                case 115460: // Healing Sphere
                    spellInfo->Categories.PreventionType = 1;
                    //spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_DEST_AREA_ENTRY;
                    break;
                case 16213: // Purification
                    spellInfo->Effects[EFFECT_0].SpellClassMask[0] &= ~8192;
                    spellInfo->Effects[EFFECT_0].SpellClassMask[3] &= ~16384;
                    spellInfo->Effects[EFFECT_0].SpellClassMask[2] |= 64;
                    break;
                case 379: // Earth Shield
                    spellInfo->Scaling.Class = 11;
                    spellInfo->Effects[EFFECT_0].Scaling.Coefficient = 1.862f;
                    break;
                case 115450: // Detox
                    spellInfo->Effects[EFFECT_2].BasePoints = 0;
                    break;
                case 65148:  // Sacred Shield
                case 113092: // Frost Bomb
                case 18153:  // Kodo Kombobulator
                case 145110:  // Ysera's Gift
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                    break;
                case 79136: // Venomous Wound
                case 127802: // Touch of the Grave
                case 145718:  // Gusting Bomb
                    spellInfo->Misc.Speed = 25.0f;
                    break;
                case 2094: // Blind
                case 108446: // Soul Link
                    spellInfo->Misc.Attributes[3] &= ~SPELL_ATTR3_CANT_TRIGGER_PROC;
                    break;
                case 123154: // Fists of Fury Visual Target
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36);
                    break;
                case 53:    // Backstab
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_CASTER_NOT_FRONT_TARGET;
                    break;
                case 21987: // Lash of Pain
                case 58563: // Assassinate Restless Lookout
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET;
                    break;
                case 124487: // Zen Focus
                case 122013: // Glyph of Incite (Protection)
                    spellInfo->Misc.Attributes[3] = SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED;
                    break;
                case 127424:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 1776: // Gouge
                case 12540:
                case 13579:
                case 24698:
                case 28456:
                case 29425:
                case 34940:
                case 36862:
                case 38863:
                case 52743: // Head Smack
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER;
                    break;
                // Shado-Pan Dragon Gun
                case 120751:
                case 120876:
                case 120964:
                case 124347:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                    break;
                case 26029: // Dark Glare
                case 37433: // Spout
                case 43140: // Flame Breath
                case 43215: // Flame Breath
                case 70461: // Coldflame Trap
                case 72133: // Pain and Suffering
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_LINE;
                    break;
                case 119072: // Holy Wrath
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                    break;
                case 24340: // Meteor
                case 26558: // Meteor
                case 28884: // Meteor
                case 36837: // Meteor
                case 38903: // Meteor
                case 41276: // Meteor
                case 57467: // Meteor
                case 26789: // Shard of the Fallen Star
                case 31436: // Malevolent Cleave
                case 35181: // Dive Bomb
                case 40810: // Saber Lash
                case 43267: // Saber Lash
                case 43268: // Saber Lash
                case 42384: // Brutal Swipe
                case 45150: // Meteor Slash
                case 64688: // Sonic Screech
                case 72373: // Shared Suffering
                case 71904: // Chaos Bane
                case 70492: // Ooze Eruption
                case 121129: // Daybreak
                case 102792: // Wild Mushroom: Bloom
                case 145944: // Sha Smash
                case 106375: // Unstable Twilight
                case 107439: // Twilight Barrage
                case 106401: // Twilight Onslaught
                case 103414: // Stomp
				case 136216: // Caustic Gas
                case 117418: // Fists of Fury (damage)
                case 114083: // Ascendance
                case 135703: // Static shock tr ef dmg
                case 98474: // Flame Scythe
                case 81280: // Blood Burst
                case 142890: //Blood Rage Dmg
                case 143962: //Inferno Strike
                case 157333: //Soothing Winds
                case 157503: //Cloudburst
                case 155152: //Prismatic Crystal
                case 153564: //Meteor
                case 153596: //Comet Storm
                    // ONLY SPELLS WITH EFFECT_SCHOOL_DAMAGE or SPELL_EFFECT_HEAL
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                    break;
                case 18500: // Wing Buffet
                case 33086: // Wild Bite
                case 49749: // Piercing Blow
                case 52890: // Penetrating Strike
                case 53454: // Impale
                case 59446: // Impale
                case 62383: // Shatter
                case 64777: // Machine Gun
                case 65239: // Machine Gun
                case 65919: // Impale
                case 74439: // Machine Gun
                case 63278: // Mark of the Faceless (General Vezax)
                case 62544: // Thrust (Argent Tournament)
                case 62709: // Counterattack! (Argent Tournament)
                case 62626: // Break-Shield (Argent Tournament, Player)
                case 64590: // Break-Shield (Argent Tournament, Player)
                case 64342: // Break-Shield (Argent Tournament, NPC)
                case 64686: // Break-Shield (Argent Tournament, NPC)
                case 65147: // Break-Shield (Argent Tournament, NPC)
                case 68504: // Break-Shield (Argent Tournament, NPC)
                case 62874: // Charge (Argent Tournament, Player)
                case 68498: // Charge (Argent Tournament, Player)
                case 64591: // Charge (Argent Tournament, Player)
                case 63003: // Charge (Argent Tournament, NPC)
                case 63010: // Charge (Argent Tournament, NPC)
                case 68321: // Charge (Argent Tournament, NPC)
                case 72255: // Mark of the Fallen Champion (Deathbringer Saurfang)
                case 118000: // Dragon Roar
                case 107029: // Impale Aspect
                case 106548:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                case 64422:  // Sonic Screech (Auriaya)
                case 122994: // Unseen Strike
                case 117921: // Massive Attacks
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                case 72293: // Mark of the Fallen Champion (Deathbringer Saurfang)
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                    break;
                case 21847: // Snowman
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF1;
                    break;
                case 82691: // Ring of Frost
                    spellInfo->Misc.Attributes[1] |= SPELL_ATTR1_CANT_BE_REFLECTED;
                    spellInfo->Misc.Attributes[5] &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                    break;
                case 76577: // Smoke Bomb
                    spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_DUMMY;
                    break;
                case 130616:// Glyph of Fear
                    spellInfo->Misc.Attributes[3] &= ~SPELL_ATTR3_IGNORE_HIT_RESULT;
                    break;
                case 118699:// Fear Effect
                    spellInfo->Misc.Attributes[3] &= ~SPELL_ATTR3_IGNORE_HIT_RESULT;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_FEAR;
                    break;
                case 124991:// Nature's Vigil (Damage)
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_NO_DONE_BONUS;
                    spellInfo->Misc.Attributes[6] &= ~SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 124988:// Nature's Vigil (Heal)
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ALLY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 114942:// Healing Tide
                    spellInfo->Misc.Attributes[1] &= ~SPELL_ATTR1_CANT_TARGET_SELF;
                    spellInfo->ClassOptions.SpellClassMask[0] = 0x00002000;
                    break;
                case 116943:// Earthgrab
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 94339: // Fungal Area
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(18); // 20s
                    break;
                case 81282: // Fungal Growth
                case 23691: // Berzerker Rage Effect
                    spellInfo->Effects[EFFECT_0].BasePoints = 100;
                    break;
                case 974:   // Earth Shield
                    spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_MOD_HEALING_RECEIVED;
                    break;
                case 86529: // Mail Specialization (Shaman)
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    break;
                case 61999: // Raise Ally
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ALLY;
                    break;
                case 121118: // Dire Beast summons
                case 122802:
                case 122804:
                case 122806:
                case 122807:
                case 122809:
                case 122811:
                case 126213:
                case 126214:
                case 126215:
                case 126216:
                case 132764:
                case 106853: // Fists of Fury
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 1459:  // Arcane Illumination
                case 109773:// Dark Intent
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                    break;
                case 61316: // Dalaran Illumination
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                    spellInfo->Effects[EFFECT_2].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                    break;
                case 86674: // Ancient Healer
                    spellInfo->AuraOptions.ProcCharges = 5;
                    break;
                case 86657: // Ancient Guardian
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                    spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                    break;
                case 5782:  // Fear
                    spellInfo->Categories.Mechanic = 0;
                    spellInfo->Effects[EFFECT_0].Mechanic = MECHANIC_NONE;
                    break;
                case 45204: // Mirror Image - Clone Me!
                    spellInfo->Misc.Attributes[6] |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                    spellInfo->Misc.Attributes[2] |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                    break;
                case 41055: // Copy Weapon Spells
                case 45206:
                case 63416:
                case 69891:
                case 69892:
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                    spellInfo->Categories.Mechanic = 0;
                    break;
                case 116694:// Surging Mists
                case 117952:// Crackling Jade Lightning
                case 116: // Frost Bolt
                    spellInfo->Categories.PreventionType = SPELL_PREVENTION_TYPE_SILENCE;
                    break;
                case 108201:// Desecrated Ground
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_USABLE_WHILE_CONFUSED;
                    break;
                case 61336: // Survival Instincts
                    spellInfo->Effects[EFFECT_0].BasePoints = -50;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                    break;
                case 137573: // Burst of Speed (IMMUNITY)
                case 1160:   // Demoralizing Shout
                case 1966:   // Feint
                case 50256:  // Demoralizing Roar
                    spellInfo->Misc.Attributes[1] |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                    break;
                case 84745: // Shallow Insight
                case 84746: // Moderate Insight
                case 84747: // Deep Insight
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
                    spellInfo->Effects[EFFECT_0].MiscValue = SPELL_SCHOOL_MASK_ALL;
                    break;
                case 44457: // Living Bomb
                case 124081: // Zen Sphere
                    spellInfo->Misc.Attributes[5] &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                    break;
                case 44461: // Living Bomb
                    spellInfo->CustomMaxAffectedTargets = 3; //used if empty on dbc SpellTargetRestrictionsEntry
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                    break;
                case 114205:// Demoralizing Banner
                case 140130: // Summon Intro Scene (hack scene)
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(8); // 15s
                    break;
                case 324:    // Lightning Shield
                case 50227:  // Sword and Board
                case 113901: // Demonic Gateway
                case 131116: // Raging Blow!
                case 136050: // Malformed Blood
                    spellInfo->AuraOptions.ProcCharges = 0;
                    break;
                case 90259: // Glyph of Frost Pillar (Root Aura)
                    spellInfo->Effects[EFFECT_0].MiscValue = 0;
                    spellInfo->Effects[EFFECT_0].MiscValueB = 0;
                    break;
                case 6770:   // Sap
                case 6346:   // Fear Ward
                case 16870:  // Clearcasting
                case 81292:  // Glyph of Mind Spike
                case 132158: // Nature's Swiftness
                case 143333: // Water Strider Water Walking
                case 48108:  // Hot Streak
                case 57761:  // Brain Freeze
                case 34936:  // Backlash
                case 124430: // Divine Insight (Shadow)
                case 93400:  // Shooting Stars
                case 144569:  // Bastion of Power
                case 144871:  // Sage Mender
                case 144595:  // Divine Crusader
                case 157174:  // Elemental Fusion
                case 88819:  // Daybreak
                case 157717:  // Enhanced Basic Attacks
                case 157644:  // Enhanced Pyrotechnics
                case 79684:  // Arcane Missiles
                case 81662:  // Evangelism
                case 123267:  // Divine Insight
                case 152117:  // Words of Mending
                case 155363:  // Words of Mending
                    spellInfo->AuraOptions.ProcCharges = 1;
                    break;
                case 89485:  // Inner Focus
                    spellInfo->AuraOptions.ProcChance = 100;
                    spellInfo->AuraOptions.ProcCharges = 1;
                    spellInfo->AuraOptions.ProcTypeMask = PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;
                    spellInfo->Effects[EFFECT_1].SpellClassMask = spellInfo->Effects[EFFECT_0].SpellClassMask;
                    break;
                case 6358:  // Seduce (succubus)
                case 115268: // Mesmerize (succubus)
                    spellInfo->ClassOptions.SpellClassSet = SPELLFAMILY_WARLOCK;
                    break;
                case 131740: // Corruption (Malefic Grasp)
                case 131736: // Unstable Affliction (Malefic Grasp)
                case 132566: // Seed of Corruption (Malefic Grasp)
                case 131737: // Agony (Malefic Grasp)
                case 85288:  // Raging Blow
                case 114908: // Spirit Shell
                case 47753:  // Divine Aegis
                case 77535:  // Mastery: Blood Shield
                case 86273:  // Mastery: Illuminated Healing
                case 83077:  // Improved Serpent Sting
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_NO_DONE_BONUS;
                    break;
                case 44544: // Fingers of Frost
                    // affect Ice Lance
                    spellInfo->Effects[EFFECT_0].SpellClassMask[0] |= 0x20000;
                    break;
                case 117993:// Chi Torpedo : Heal
                case 124040:// Chi Torpedo : Damage
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(9);
                    break;
                case 80325: // Camouflage
                case 119450:// Glyph of Camouflage
                case 30451: // Arcane Blast
                case 138334: //Fatal strike
                case 124845: //Calamity
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    break;
                case 115295: //Guard
                    spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_MOD_HEALING_DONE_PERCENT;
                    spellInfo->Effects[EFFECT_1].BasePoints = 30;
                    break;
                case 126451: //Clash - Impact
                case 129428: //Stone Guards - Dummy Searcher(cobalt mine)
                case 154462: //Ritual of Bones
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(29);
                    break;
                case 121253:// Keg Smash
                    spellInfo->CustomMaxAffectedTargets = 3;      //used if empty on dbc SpellTargetRestrictionsEntry
                    break;
                case 115308: // Elusive Brew
                case 122300: // Psyfiend Visual
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1);
                    break;
                case 126892:// Zen Pilgrimage
                case 126895:// Zen Pilgrimage : Return
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                    break;
                case 130320:// Rising Sun Kick - Monks abilities deal 10% more damage
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);
                    break;
                case 107270:// Spinning Crane Kick - Radius
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);
                    break;
                case 127626:// Devouring plague - Heal
                    spellInfo->Effects[EFFECT_0].BasePoints = 1;
                    break;
                //case 106909:
                //    {
                //        SpellInfo const* spell = sSpellMgr->GetSpellInfo(113379);
                //        if (!spell)
                //            break;
                //        spellInfo->DurationEntry = spell->DurationEntry;
                //    }
                //    break;
                //case 113315:
                //    {
                //        SpellInfo const* spell = sSpellMgr->GetSpellInfo(113379);
                //        if (!spell)
                //            break;
                //        spellInfo->DurationEntry = spell->DurationEntry;
                //    }
                //    break;
                case 106736:
                case 106113:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                    break;
                case 119922: //Shockwave
                case 119929:
                case 119930:
                case 119931:
                case 119932:
                case 119933:
                    spellInfo->Misc.Speed = 5.0f;
                    break;
                case 106112:
                    {
                        const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(22);
                        if (!radius)
                            break;

                        spellInfo->Effects[EFFECT_0].RadiusEntry = radius; //200yards.
                    }
                    break;
                case 106847:
                case 106588: // Expose Weakness
                case 106600:
                case 106613:
                case 106624:
                case 104855: // Overpacked Firework
                case 145212: //Unleashed Anger dmg
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                    break;
                    // Wise Mari Wash Away
                case 106334:
                case 99508: //Bloated Frog
                    spellInfo->Misc.Attributes[3] &= ~SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                    break;
                case 120552:
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(16);
                    break;
                case 119684:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                    spellInfo->Effects[EFFECT_1].TargetB = 0;
                    break;
                case 112060:
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 118685:
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(5);
                    break;
                 // Malygos Enrage
                case 60670:
                    spellInfo->Effects[EFFECT_1].TriggerSpell = 0;
                    spellInfo->Effects[EFFECT_2].TriggerSpell = 0;
                    break;
                case 114746:
                    spellInfo->Effects[EFFECT_2].TargetA = TARGET_UNIT_TARGET_ALLY;
                    spellInfo->Effects[EFFECT_2].TargetB = 0;
                    break;
                // Add Server-Side dummy spell for Fishing
                // TODO : Add more generic system to load server-side spell
                //case 7733:
                //case 7734:
                //case 18249:
                //case 54083:
                //case 54084:
                //case 51293:
                //case 88869:
                //case 110412:
                //{
                //    if(SpellEntry const* spellEntry = sSpellStore.LookupEntry(131474))
                //    {
                //        SpellInfo* fishingDummy = new SpellInfo(spellEntry);
                //        fishingDummy->Id = spellInfo->Effects[EFFECT_0].TriggerSpell;
                //        mSpellInfoMap[spellInfo->Effects[EFFECT_0].TriggerSpell] = fishingDummy;
                //    }
                //    break;
                //}
                // Siege of the Niuzoa temple
                case 119941: //Puddle Void Zone
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(22);
                    break;
                case 124290: //Blade Rush Dmg trigger(sword)
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(26);
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    break;
                case 124312: //Blade Rush Charge
                    spellInfo->Effects[EFFECT_0].TargetA = 25;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 119875: //Templest
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(48);
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(48);
                    break;
                // Stormstout brewery
                case 112944: //Carrot Breath - set 4 second becaus rotate not work
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(35);
                    break;
                //Scholomance
                case 114062: //Gravity Flux
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                    break;
                case 113996: //Bone Armor
                case 116606: //Troll Rush
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    break;
                case 111628: //Shadow blaze dmg
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(44);
                    break;
                // Mogu'shan Vault
                // Feng
                case 116364: //Arcane Velocity
                case 116018: //Epicenter
                case 116157: //Lightning fists
                case 116374: //Lightning fists (trigger dmg)
                case 136324: //Rising Anger
                    spellInfo->AuraRestrictions.CasterAuraSpell = 0;
                    break;
                case 116417: //Arcane Resonance
                    spellInfo->Effects[EFFECT_0].TargetA = 6;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = 6;
                    spellInfo->Effects[EFFECT_1].TargetB = 0;
                    break;
                case 116040: //Epicenter(trigger dmg)
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(48);//60 yards
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(48);
                    spellInfo->Effects[EFFECT_2].RadiusEntry = sSpellRadiusStore.LookupEntry(48);
                    break;
                case 116365: //Arcane Velocrity (trigger dmg)
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(48);//60 yards
                    break;
                case 116434: //Arcane Resonance(trigger dmg)
                    spellInfo->Effects[EFFECT_0].TargetB = 30;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                    break;
                //Elegon
                case 129724: //Energy tendrols (trigger spell - grip)
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_JUMP;
                    break;
                //Will of the Imperator
                case 116782:
                case 116803: //Titan Gase (trigger spell)
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(41); //150yards
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(41); //150yards
                    break;
                case 118327: //Titan Gase (trigger spell)
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(41); //150yards
                    break;
                case 116550: //Emergizing Smash
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); //10yards
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(13); //10yards
                    spellInfo->Effects[EFFECT_2].RadiusEntry = sSpellRadiusStore.LookupEntry(13); //10yards
                    break;
                case 116161:
                    spellInfo->Effects[EFFECT_1].MiscValue = 2; // Set Phase to 2
                    spellInfo->Effects[EFFECT_3].Effect    = 0; // No need to summon
                    break;
                case 116272:
                    spellInfo->Effects[EFFECT_0].MiscValue = 2; // Set Phase to 2
                    break;
                case 118303:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    break;
                case 15850: // Chilled
                case 16927: // Chilled
                case 20005: // Chilled
                    spellInfo->Categories.Mechanic = MECHANIC_SNARE;
                    break;
                case 128405: // Narrow Escape
                    spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_TAKE_DAMAGE;
                    break;
                //Heart of Fear
                //Garalon
                case 122835: //Pheromones
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0; 
                    spellInfo->Effects[3].TriggerSpell = 0;
                    break;
                case 123120: //Pheromones trail tr ef
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(44); //0.5yard
                    break;
                //
                //Lord Meljarak
                case 122193: //Heal
                    spellInfo->AuraRestrictions.TargetAuraSpell = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = 25;
                    break;
                case 122147: //Heal trigger
                    spellInfo->AuraRestrictions.TargetAuraSpell = 0;
                    spellInfo->Effects[EFFECT_0].TargetA = 25;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                //
                //Unsok
                case 122408: //Massive Stomp
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(18);//15 yards
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(18);
                    break;
                case 121995: //Amber Scalpel trigger spell
                    spellInfo->AuraRestrictions.TargetAuraSpell = 0;
                    spellInfo->Effects[EFFECT_0].TargetA = 25;
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(15);//3yards
                    break;
                case 122532: //Explose
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);//8yards
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    break;
                //
                //Empress Shekzeer
                case 123788: //Cry of terror
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = 1;
                    break;
                case 123735: //Dread screetch
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = 1;
                    spellInfo->Effects[EFFECT_1].TargetB = 0;
                    break;
                case 123743: //Dread screetch trigger spell
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(8);//5yards
                    break;
                case 66289: // Glaive
                case 67439: // Boulder
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(10);//30yards
                    break;
                //Terrace of Endless Spring
                //Protectors of Endless
                case 117052: //Sha Corruption
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    break;
                //Tsulong
                case 122767: //Dread Shadows
                case 144776: //Ground Pound
                case 154294: //Power Conduit
                    spellInfo->Effects[EFFECT_1].TriggerSpell = 0;
                    break;
                case 122789: //SunBeam trigger aura
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(26);//4yards
                    break;
                case 122855: //Sun Breath
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENTRY;
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENTRY;
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    break;
                //Lei Shi
                case 123121: //Spray
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(7);//2yards
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(7);//2yards
                    break;
                //Sha of Fear
                case 119495: //Eerie skull trigger spell
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(7);//2yards
                    break;
                case 119086: //Penetrating bolt trigger spell
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(26);//4yards
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(26);//4yards
                    break;
                //Throne of Thunder
                //Jinrokh
                case 137162: //Static burst
                case 144115: //Flame Coating
                case 119610: //Bitter thoughts
                case 139901: //Stormcloud tr ef - dmg
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                    break;
                case 137261: //Lightning storm tr ef - dmg
                case 140819: //Lightning storm tr ef = dummy
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    break;
                //Minibosses
                case 139900: //Stormcloud
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                //Horridon
                case 136740: //Double swipe tr ef
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_BACK;
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(4);
                    break;
                case 136769: //Horridon charge
                case 88764:  // Rolling Thunder
                case 144278: //Generate rage
                case 143462: //Sha pool
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    break;
                case 136991: //Bitting cold tr ef 
                case 144766: //Detonation sequence
                case 136345: //Stormcloud tr ef
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                    break;
                //Kazrajin
                case 137122: //Reckless charge (point dmg)
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->Effects[EFFECT_1].TargetA = 22;
                    spellInfo->Effects[EFFECT_1].TargetB = 15;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(8);//5yards
                    break;
                //Tortos
                case 134920: //Quake stomp
                    spellInfo->Effects[2].TriggerSpell = 0;
                    break;
                case 134011: //Spinning shell dmg
                    spellInfo->Effects[EFFECT_0].TargetA = 6;
                    spellInfo->Effects[EFFECT_1].TargetA = 6;
                    spellInfo->Effects[2].TargetA = 6;
                    break;
                case 133946: //Furios stone breath tr ef dmg(nerf)
                    spellInfo->Effects[EFFECT_0].BasePoints = 34124;
                    break;
                case 135101: //Drain the weak tr ef dmg
                    spellInfo->Effects[EFFECT_0].TargetA = 6;
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(2);
                    break;
                //Megaera
                case 139822: //Cinders dot
                    spellInfo->Effects[EFFECT_0].TargetA = 6;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = 6;
                    spellInfo->Effects[EFFECT_1].TargetB = 0;
                    break;
                case 139836: //Cinders void zone dmg
                    spellInfo->Effects[EFFECT_1].TargetB = 15;
                    break;
                //Primordius
                case 136220: //Acidic explosion tr ef dmg
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(8);
                    break;
                //Dark Animus
                case 138569: //Explosive slam
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->Effects[EFFECT_1].TargetB = 15;
                    break;
                //Iron Qon
                case 134926: //Throw spear tr ef
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    spellInfo->Effects[EFFECT_1].MiscValue = 0;
                    spellInfo->Effects[EFFECT_1].MiscValueB = 0; 
                    break;
                //Twin Consorts
                case 137341: //Beast of nightmares target aura
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_1].TargetA = 1;
                    spellInfo->Effects[2].TargetA = 1;
                    spellInfo->Effects[3].TargetA = 1;
                    spellInfo->Effects[4].TargetA = 1;
                    break;
                case 137405: //Tears of Sun
                    spellInfo->AuraRestrictions.ExcludeTargetAuraSpell = 0;
                    break;
                case 137419: //Ice Comet tr ef
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    spellInfo->Effects[EFFECT_1].MiscValue = 0;
                    spellInfo->Effects[EFFECT_1].MiscValueB = 0; 
                    break;
                //Lei Shen
                case 134912: //Decapitate base aura
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(3); //20yards
                    break;
                case 135695: //Static shock base aura
                    spellInfo->Effects[EFFECT_0].TargetA = 6;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                //Ra Den
                case 138321: //Material of creation
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 138329: //Unleashed anime
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->Effects[EFFECT_1].TargetA = 1;
                    spellInfo->Effects[EFFECT_1].TargetB = 0;
                    break;
                //Siege of Orgrimmar
                //Immerseus
                case 143461: //Sha pool dummy
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                    break;
                case 143297: //Sha splash
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(65); // 1.5s
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(7); //2yards
                    break;
                case 130063: //Sha splash Dummy
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(21);
                    break;
                case 145377: //Erupting water
                case 143524: //Purified residue
                    spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                    break;
                case 113762: //Swirl
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 125925;
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_PASSIVE;
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_HIDDEN_CLIENTSIDE;
                    break;
                case 143412: //Swirl dmg
                    spellInfo->Effects[EFFECT_0].TargetA = 1;
                    spellInfo->Effects[EFFECT_1].TargetA = 1;
                    spellInfo->Effects[EFFECT_0].Effect = 2;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = 0;
                    break;
                case 125925: //Swirlr tr ef (Cone Searcher)
                    spellInfo->Effects[EFFECT_0].BasePoints = 0;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_110;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(13); //200yards
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(22); //200yards
                    break;
                case 143574: //Swelling corruption
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 143579;
                    break;
                case 143579: // Immerseus: Sha Corruption
                case 103534: // Resonating Crystal: Danger
                case 103536: // Resonating Crystal: Warning
                case 103541: // Resonating Crystal: Safe
                case 105479: // Corruption: Searing Plasma
                case 144774: // Sha of Pride: Reaching Attack
                case 100941: // CATA: Ragnaros: Dreadflame
                case 146703: // Amalgam of Corruption: Bottomless Pit
                case 148310: // Bombard Stun
                case 148311: // Bombard Stun
                case 82881:  // Mortality
                case 136992: //Bitting cold
                case 140023: //Ring of Peace
                case 134916: //Decapitate tr ef
                case 146325: //Cutter Laser Visual Target
                case 144918: //Cutter Laser Dmg
                case 159226: //Solar Storm
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                    break;
                case 144396: //Vengeful Strikes. WTF. SPELL_AURA_MOD_POSSESS_PET
                    spellInfo->Effects[0].Effect = 0;
                    spellInfo->Effects[0].ApplyAuraName = 0;
                    break;
                case 143730: //Dark Meditation
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 143546;
                    break;
                case 143840: //Mark of Anguish. WTF. SPELL_AURA_MOD_POSSESS_PET
                    spellInfo->Effects[3].Effect = 0;
                    spellInfo->Effects[3].ApplyAuraName = 0;
                    break;
                //Norushen
                case 144514:    //Lingering Corruption
                    spellInfo->CustomMaxAffectedTargets = 1; //used if empty on dbc SpellTargetRestrictionsEntry
                    spellInfo->Misc.SchoolMask |= SPELL_SCHOOL_MASK_NORMAL;
                    break;
                case 147082:    //Burst of Anger
                    spellInfo->AuraRestrictions.TargetAuraSpell = 144421;
                    break;
                case 145214:    //Unleashed Anger
                    spellInfo->AuraRestrictions.CasterAuraSpell = 0;
                    spellInfo->Effects[2].TargetA = 25;
                    break;
                case 145573:    //Blind Hatred Dummy
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].ApplyAuraPeriod = 500;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 145227;
                    break;
                case 145227:    //Blind Hatred Dmg
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->Effects[EFFECT_1].TargetB = 15;
                    break;
                case 145735:    //Icy Fear Dmg
                    spellInfo->AuraRestrictions.TargetAuraSpell = 0;
                    break;
                case 144421:    //Corruption
                    spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_APPLY_AURA;
                    break;
                case 144482:    //Tear Reality
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_104;
                    break;
                case 145073:    //Residual Corruption
                    spellInfo->AuraRestrictions.TargetAuraSpell = 0;
                case 144628:    //Titanic Smash
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                    spellInfo->Effects[2].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                    break;
                //Iron Juggernaut
                case 144218: //Borer Drill Dmg
                    spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                    break;
                case 144458: //Scatter Laser
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    spellInfo->Effects[EFFECT_0].TargetB = NULL;
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    spellInfo->Effects[EFFECT_1].TargetB = NULL;
                    break;
                case 144555: //Mortar Barrage
                case 143848: //Essence of yshaarj
                    spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                    spellInfo->Effects[0].TargetB = 0;
                    break;
                //Korkron Dark Shaman
                case 144215: //Froststorm strike 
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(13); //200yards
                    break;
                case 144334: //Iron Tomb dmg
                    spellInfo->Effects[0].TargetA = TARGET_DEST_CASTER_FRONT;
                    break;
                case 144331: //Iron Prison
                case 142906: //Ancient Miasma Dmg
                case 29604:  // Jom Gabbar
                    spellInfo->Misc.Attributes[6] |= SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS;
                    break;
                //General Nazgrim
                case 143872: //Ravager Summon
                    spellInfo->Effects[1].TargetA = TARGET_DEST_TARGET_ENEMY;
                    break;
                case 143716: //Heroic Shockwave
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CASTER;
                    spellInfo->Effects[EFFECT_1].TriggerSpell = 143715; //Summon
                    break;
                case 143420: // Ironstorm
                case 105847: // Seal Armor Breach
                case 105848:
                    spellInfo->InterruptFlags = 0;
                    break;
                case 143882: //Hunter's Mark
                    spellInfo->AuraRestrictions.ExcludeTargetAuraSpell = 0;
                    break;
                //Spoils of Pandaria
                case 146257: //Path of Blossoms Dmg
                case 146289: //Mass Paralyses
                    spellInfo->Effects[0].TargetA = 22;
                    spellInfo->Effects[0].TargetB = 15;
                    break;
                case 146189: //Eminence
                case 146068: //Blade of the hundred steps (tank buff)
                    spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
                    spellInfo->Effects[0].TriggerSpell = 0;
                    break;
                case 148515: //Shadow Volley Dummy
                    spellInfo->Effects[1].TargetA = 22;
                    spellInfo->Effects[1].TargetB = 15;
                    break;
                case 148582: //Jade Tempest AT
                    spellInfo->Effects[2].Effect = 0;
                    spellInfo->Effects[3].Effect = 0;
                    spellInfo->Effects[4].Effect = 0;
                    break;
                case 148583: //Jade Tempest Dmg
                    spellInfo->Effects[0].BasePoints = 105000;
                    break;
                //Thok Bloodthirsty
                case 147607: //Cannon Ball AT Dmg
                    spellInfo->Effects[0].TargetA = 25;
                    spellInfo->Effects[0].TargetB = 0;
                    spellInfo->Effects[1].TargetA = 25;
                    spellInfo->Effects[1].TargetB = 0;
                    break;
                case 147906: //Cannon Ball Dest Dmg
                    spellInfo->Effects[4].RadiusEntry = sSpellRadiusStore.LookupEntry(14);//8yards
                    break;
                case 143428: //Tail lash
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_BACK;
                    break;  
                case 143430: //Clump Check
                    spellInfo->AuraRestrictions.TargetAuraSpell = 0;
                    spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(22); //200yards
                    spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                    spellInfo->Effects[0].TargetA = 22;
                    spellInfo->Effects[0].TargetB = 15;
                    break;
                case 143445: //Fixate
                    spellInfo->Effects[0].TargetA = 25;
                    spellInfo->Effects[0].TargetB = 0;
                    spellInfo->Effects[1].TargetA = 25;
                    spellInfo->Effects[1].TargetB = 0;
                    break;
                //Paragons of the Klaxxi
                case 146982: //Enrage
                    spellInfo->Effects[0].TargetA = 1;
                    spellInfo->Effects[0].TargetB = 0;
                    spellInfo->Effects[1].TargetA = 1;
                    spellInfo->Effects[1].TargetB = 0;
                    spellInfo->Effects[2].TargetA = 1;
                    spellInfo->Effects[2].TargetB = 0;
                    spellInfo->Effects[3].TargetA = 1;
                    spellInfo->Effects[3].TargetB = 0;
                    spellInfo->Effects[4].TargetA = 1;
                    spellInfo->Effects[4].TargetB = 0;
                    break;
 
                    
                //World Bosses
                //Sha of Anger
                case 119487: //Anger
                    spellInfo->Effects[EFFECT_0].TargetA = 22;
                    spellInfo->Effects[EFFECT_0].TargetB = 15;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(30);
                    spellInfo->Effects[EFFECT_1].TargetA = 22;
                    spellInfo->Effects[EFFECT_1].TargetB = 15;
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(30);
                    break;
                case 119489: //Unleashed Wrath
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(10);
                    break;
                // Galion
                case 121787: //Stomp
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(12);
                    spellInfo->Effects[EFFECT_2].RadiusEntry = sSpellRadiusStore.LookupEntry(12);
                    break;
                case 121600: //Barrage Dmg
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                    spellInfo->Effects[EFFECT_2].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                    break;
                //Nalak
                case 136340: //Stormcloud
                case 136343: //Static Shield tr ef dmg
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                    spellInfo->Effects[EFFECT_0].TargetB = 0;
                    break;
                case 136341: //Static shield
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    spellInfo->Effects[EFFECT_1].TriggerSpell = 0;
                    break;
                // Dalaran arena knockback
                case 61698:
                    spellInfo->Misc.Attributes[0] = 536871296;
                    spellInfo->Misc.Attributes[1] = 269058048;
                    spellInfo->Misc.Attributes[2] = 67108868;
                    spellInfo->Misc.Attributes[3] = 268894272;
                    spellInfo->Misc.Attributes[4] = 2048;
                    spellInfo->Misc.Attributes[6] = 1024;
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(1);
                    spellInfo->EquippedItemClass = -1;
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                    spellInfo->Effects[EFFECT_0].TargetA = SpellImplicitTargetInfo(25);
                    break;
                case 130649: // Life Spirit
                    spellInfo->Effects[EFFECT_0]. BasePoints = 10000;
                    break;
                case 130650: // Water Spirit
                    spellInfo->Effects[EFFECT_0]. BasePoints = 5000;
                    break;
                case 105709: // Master Mana Potion
                    spellInfo->Effects[EFFECT_0]. BasePoints = 30000;   
                    break;
                case 126349:
                case 126413:
                case 126549:
                case 126550:
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(26); // 240 seconds
                    break; 
                case 11327: //Vanish
                    spellInfo->Misc.Attributes[4] |= SPELL_ATTR4_TRIGGERED;
                    break;
                case 8177: //totem
                    spellInfo->Cooldowns.RecoveryTime = 25000;
                    spellInfo->Cooldowns.CategoryRecoveryTime = 0;
                    break;
                case 45284: //Lightning Bolt
                    spellInfo->ClassOptions.SpellClassMask[0] = 0x00000001;
                    spellInfo->ClassOptions.SpellClassMask[2] = 0;
                    break;
                case 117679:    // Incarnation (Passive)
                    spellInfo->Misc.Attributes[0] &= ~SPELL_ATTR0_CANT_CANCEL;
                    break;
                case 55442: //Glyph of Capacitor Totem
                    spellInfo->Effects[EFFECT_0].SpellClassMask[0] = 0x00008000;
                    break;
                case 116186:    // Glyph of Prowl
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
                    break;
                case 132402:    // Savage Defense
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(32); // 6s
                    break;
                case 76724: // Offering
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(3); // 60s
                    break;
                case 122557: // The Challenger's Ring: Summon Snow Blossom Fighter
                case 122746: // The Challenger's Ring: Yalia Sagewhisper
                case 62772: // Summon Gorat's Spirit
                case 62814: // Summon Elendilad
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(5); // 300s
                    break;
                case 37062: // To Catch A Thistlehead
                case 139999: // Mason's Folly Finale Event (hack scene)
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(4); //120s
                    break;
                case 112965:    // Fingers of Frost
                    for (int i = 0; i < 3; ++i)
                    {
                        spellInfo->Effects[i].Effect = SPELL_EFFECT_APPLY_AURA;
                        spellInfo->Effects[i].ApplyAuraName = SPELL_AURA_DUMMY;
                        spellInfo->Effects[i].TargetA = TARGET_UNIT_CASTER;
                    }
                    break;
                case 122137: // Summon Ghosts from Urns - Summons
                    spellInfo->Effects[EFFECT_0].BasePoints = 5;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(25); // 180s
                    break;
                case 74793:// Summoning Ritual
                case 134210: // Scenario: Pursuing the Black Harvest - Memory of the Reliquary
                    spellInfo->CastingReq.RequiredAreasID = -1;
                    break;
                case 75478:// Summon Charbringer
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_CASTER_RANDOM;
                    break;
                case 61784: // Feast On Turkey
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 61842;
                    spellInfo->AuraOptions.ProcChance = 100;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_MASTER;
                    break;
                case 61786: // Feast On Sweet Potatoes
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 61844;
                    spellInfo->AuraOptions.ProcChance = 100;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_MASTER;
                    break;
                case 61788: // Feast On Stuffing
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 61843;
                    spellInfo->AuraOptions.ProcChance = 100;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_MASTER;
                    break;
                case 61787: // Feast On Pie
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 61845;
                    spellInfo->AuraOptions.ProcChance = 100;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_MASTER;
                    break;
                case 61785: // Feast On Cranberries
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 61841;
                    spellInfo->AuraOptions.ProcChance = 100;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_MASTER;
                    break;
                case 112897: // Battle Ring
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(27); // 3s
                    break;
                case 20711:
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    spellInfo->Effects[EFFECT_0].BasePoints = 1;
                    spellInfo->Effects[EFFECT_0].MiscValue = 0;
                    break;
                case 119914: //Felstorm
                case 119915: //Felstorm
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    break;
                case 70781: // Light's Hammer Teleport
                case 70856: // Oratory of the Damned Teleport
                case 70857: // Rampart of Skulls Teleport
                case 70858: // Deathbringer's Rise Teleport
                case 70859: // Upper Spire Teleport
                case 70860: // Frozen Throne Teleport
                case 70861: // Sindragosa's Lair Teleport
                case 108786:// Summon Stack of Reeds
                case 108808:// Mop: quest
                case 108830:// Mop: quest
                case 108827:// Mop: quest
                case 104450:// Mop: quest
                case 108845:// Mop: quest
                case 108847:// Mop: quest
                case 108857:// Mop: quest
                case 108858:// Mop: quest
                case 109335:// Mop: quest
                case 105002:// Mop: quest
                case 117497:// Mop: quest
                case 117597:// Mop: quest
                case 69971: //q.24502
                case 69976:
                case 69977:
                case 69978:
                case 69979:
                case 69980:
                case 69981:
                case 69982:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DB;
                    break;
                case 115435:// Mop: quest
                    spellInfo->Effects[EFFECT_0].MiscValueB = 1;
                    break;
                case 84964:  // Rayne's Seed
                case 101847: // Shoe Baby
                case 65203:  // Throw Oil
                case 50493: // D.I.S.C.O.
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_SRC_AREA_ENTRY;
                    break;
                case 66795: // Gather Lumber
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_MASTER;
                    break;
                case 3411: // Intervene
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_RAID;
                    break;
                case 51490:  // Thunderstorm
                case 30823:  // Shamanistic Rage
                case 498:    // Divine Protection
                case 137562: // Nimble Brew
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                    break;
                case 131086: // Bladestorm (Protection buff) DND
                    spellInfo->Effects[EFFECT_0].BasePoints = 50;
                    break;
                case 20066: // Repentance
                    spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                    break;
                case 23035: // Battle Standard (Horde)
                case 23034: // Battle Standard (Alliance)
                    spellInfo->Effects[EFFECT_0].MiscValueB = 3291;  //SUMMON_TYPE_BANNER
                    break;
                case 33619: // Reflective Shield
                    spellInfo->Misc.Attributes[6] |= SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS;
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_NO_DONE_BONUS;
                    break;
                // Thrall Lighting in goblin
                case 68441:
                case 68440:
                    spellInfo->Effects[EFFECT_0].ChainTargets = 60;
                    break;
                case 71091: // Goblin. Lost Isles. It's a Town-In-A-Box
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(63);
                    break;
                case 71919:
                case 71918:
                case 83115:
                case 83116:
                    spellInfo->Effects[EFFECT_0].MiscValueB = 3302;  //SUMMON_TYPE_MINIPET
                    break;
                case 103964: // Touch of Chaos
                case 129347: // Chaos Wave
                    spellInfo->Misc.SchoolMask &= ~SPELL_SCHOOL_MASK_NORMAL;
                    break;
                case 53257: // Cobra Strikes
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_CANT_CANCEL;
                    break;
                case 96117: // Toss Stink Bomb Credit
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_PASSENGER_0;
                    break;
                case 66550: // Teleport outside (Isle of Conquest)
                    spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(1);
                    break;
                case 146557: // Frozen Thoughts
                    spellInfo->Effects[EFFECT_0].SpellClassMask[0] |= 131616;
                    break;
                case 106371: // Hour of Twilight
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 103327;
                    spellInfo->Effects[EFFECT_1].TriggerSpell = 106174;
                    break;
                case 108038: // Harpoon
                    spellInfo->SetRangeIndex(13); // 5000 yards
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                    break;
                case 109176: // Unstable Twilight aoe 2
                    spellInfo->CustomMaxAffectedTargets = 1;
                    break;
                case 103821: // Earthen Vortex
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_STUN;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(48); // 60 yard
                    spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(48);
                    spellInfo->Effects[2].RadiusEntry = sSpellRadiusStore.LookupEntry(48);
                    break;
                case 103178: // Earths Vengeance dmg
                    spellInfo->Duration.Duration = 23 * IN_MILLISECONDS;
                    break;
                case 105241: // Absorb Blood
                case 106444: // Impale
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(26); // 4 yards
                    break;
                // Madness of Deathwing
                case 106663: // Carrying Winds
                case 106668:
                case 106670:
                case 106672:
                case 106674:
                case 106676:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    spellInfo->AuraRestrictions.ExcludeCasterAuraSpell = 0;
                    spellInfo->Duration.Duration = 1 * IN_MILLISECONDS;
                    break;
                // Aspects auras
                // Increase duration up to 10 secs
                case 106028:
                case 106027:
                case 106457:
                case 106464:
                case 106029:
                    spellInfo->Duration.Duration = 10 * IN_MILLISECONDS;
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                    break;
                case 106040: // Spellweaving
                    spellInfo->AuraOptions.ProcChance = 10;
                    spellInfo->Duration.Duration = 10 * IN_MILLISECONDS;
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                    break;
                // Increase amplitude
                case 105825:
                case 105823:
                case 106456:
                case 106463:
                case 106026:
                case 106039:
                    spellInfo->Effects[EFFECT_0].ApplyAuraPeriod = 7000;
                    break;
                case 106527: // Cataclysm screen
                    spellInfo->Duration.Duration = 10 * IN_MILLISECONDS;
                    break;
                case 105369: // Lightning Conduit dmg
                    spellInfo->Duration.Duration = 1 * IN_MILLISECONDS;
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_HIDE_DURATION;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    spellInfo->Misc.Attributes[8] |= SPELL_ATTR8_DONT_RESET_PERIODIC_TIMER;
                    break;
                case 105367: // Lightning Conduit dummy 1
                    spellInfo->Duration.Duration = 1 * IN_MILLISECONDS;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
                    spellInfo->Effects[EFFECT_1].ApplyAuraPeriod = 1000;
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                    break;
                case 105371: // Lightning Conduit dummy 2
                    spellInfo->Duration.Duration = 1 * IN_MILLISECONDS;
                    spellInfo->Misc.Attributes[5] |= SPELL_ATTR5_HIDE_DURATION;
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                    break;
                case 58423: // Relentless Strikes
                    spellInfo->Effects[EFFECT_0].SpellClassMask[1] |= 8;
                    break;
                case 122355: // Molten Core
                    spellInfo->AuraOptions.CumulativeAura = 10;
                    break;
                case 109145: // Demonic Fury
                    spellInfo->Effects[EFFECT_0].BasePoints = 0;
                    break;
                case 77489: // Echo of Light
                case 58068: // Glyph of Soul Consumption
                    spellInfo->Misc.Attributes[2] |= SPELL_ATTR2_CANT_CRIT;
                    break;
                // Zul'Gurub: Jindo The Godbreaker
                case 96689: // Spirit World aura
                    spellInfo->Effects[EFFECT_1].MiscValue = 2;
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                    spellInfo->Misc.Attributes[3] |= SPELL_ATTR3_IGNORE_HIT_RESULT;
                    break;
                case 54355: // Achiev Mine Sweeper
                case 54402:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                    spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CASTER;
                    break;
                case 93957: // Achiev Headed South
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA;
                    spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                    spellInfo->Duration.Duration = -1;
                    break;
                case 77569: // Release Aberrations
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_GAMEOBJECT_SRC_AREA;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(28);
                    break;
                case 77991: // Release All
                    spellInfo->InterruptFlags &= ~SPELL_INTERRUPT_FLAG_INTERRUPT;
                    spellInfo->Effects[EFFECT_0].TargetB = TARGET_GAMEOBJECT_SRC_AREA;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(28);
                    spellInfo->Effects[EFFECT_1].Effect = 0;
                    break;
                case 85222: // Light of Dawn
                    spellInfo->Categories.DefenseType = SPELL_DAMAGE_CLASS_MAGIC;
                    break;
                case 115037: // Ling-Ting's Herbal Journey
                    spellInfo->Effects[EFFECT_0].MiscValue = 3;
                    break;
                case 119377: // Siege Explosive
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_TARGET_RADIUS;
                    spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(10);
                    break;
                case 54724: // Teleport to Hall of Command
                case 54725:
                case 54745:
                case 54746:
                case 54699: // Teleport to Heart of Acherus
                case 54700:
                case 54742:
                case 54744:
                    spellInfo->Misc.Attributes[6] |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                    spellInfo->Misc.Attributes[2] |= SPELL_ATTR2_CAN_TARGET_DEAD;
                    break;
                case 1784:  //Wod:stealth
                    spellInfo->Effects[EFFECT_2].BasePoints = 25;
                    break;
				case 166216: // Wod: Q34429
					//spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(21); // -1s
					break;
                case 163772: // Wod: lost scene id.
                    spellInfo->Effects[EFFECT_0].MiscValue = 801;
                    break;
                //Wod: Q: 34439 Blizz use script for change summon entry. But i did it by this.
                case 161166:
                    spellInfo->Effects[EFFECT_0].MiscValue = 79315;
                    break;
                case 161065:
                    spellInfo->Effects[EFFECT_0].MiscValue = 78430;
                    break;
                case 161167:
                    spellInfo->Effects[EFFECT_0].MiscValue = 79675;
                    break;
                case 158818:
                    spellInfo->Effects[EFFECT_0].MiscValue = 79767;
                    break;
                // Hacks scene
                case 139384: // Summon Wrathion Intro
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(608);
                    break;
                case 140097: // Summon Intro Scene
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(600); //42s
                    break;
                case 140265: // Summon Thunder King Finale
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(23); //90s
                    break;
                case 146397: // Legendary Finale
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(567); //135s
                    break;
                case 89496: // Gnomish Gravity Well
                    spellInfo->Misc.Attributes[4] &= ~SPELL_ATTR4_CAST_ONLY_IN_OUTLAND;
                    break;
                case 100752: // Darkmoon: Using Steam Tonk Controller
                    spellInfo->Effects[0].TargetB = TARGET_DEST_DB;
                    break;
                //< Scenario: Thunder forge
                case 138869: // Forging
                    spellInfo->Effects[EFFECT_1].Amplitude = 655 * 2;
                    break;
                case 109659: // Dig Rat Stew
                    spellInfo->Effects[EFFECT_0].TriggerSpell = 6417;
                    break;
                case 108212: // Burst of Speed
                    spellInfo->Misc.Attributes[1] |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                    //spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(35); // 4s
                    spellInfo->Effects[0].BasePoints = 70;
                    break;
                case 122016:  // Incite
                    spellInfo->AuraOptions.ProcCharges = 3;
                    spellInfo->AuraOptions.CumulativeAura = 0;
                    break;
                case 153238:
                    spellInfo->Effects[EFFECT_0].ApplyAuraPeriod = 2500;
                    break;
                case 169422:  // Q: 34364
                    spellInfo->Effects[EFFECT_0].MiscValue = 604;
                    spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_ACTIVATE_SCENE;
                    break;
                case 163452:
                    spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                    break;
                case 6196:  // Far Sight
                    spellInfo->Misc.Attributes[0] |= SPELL_ATTR0_INDOORS_ONLY;
                    break;
                case 52042: // Healing Tide
                    spellInfo->Misc.Attributes[1] &= ~SPELL_ATTR1_CANT_TARGET_SELF;
                    break;
                case 157154:
                    spellInfo->Effects[EFFECT_0].BasePoints = 0;
                    break;
                case 168082: // Revitalizing Waters
                case 168105: // Rapid Tides
                case 177497: // Bramble Patch
                case 168041: // Briarskin
                case 168375: // Grasping Vine
                case 175997: // Noxious Eruption
                    spellInfo->AuraRestrictions.CasterAuraSpell = 0;
                    break;
                default:
                    break;
            }

            switch (spellInfo->ClassOptions.SpellClassSet)
            {
            case SPELLFAMILY_WARRIOR:
                // Shout
                if (spellInfo->ClassOptions.SpellClassMask[0] & 0x20000 || spellInfo->ClassOptions.SpellClassMask[1] & 0x20)
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                break;
            case SPELLFAMILY_DRUID:
                // Roar
                if (spellInfo->ClassOptions.SpellClassMask[0] & 0x8)
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                break;
            default:
                break;
            }

            // This must be re-done if targets changed since the spellinfo load
            spellInfo->ExplicitTargetMask = spellInfo->_GetExplicitTargetMask();

            switch (spellInfo->Id)
            {
                case 107223:
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                   break;
                case 106736:
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                    break;
                case 106112:
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_DEST_LOCATION;
                    break;
                case 106113:
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                    break;
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded spell custom attributes in %u ms", GetMSTimeDiffToNow(oldMSTime));

    oldMSTime = getMSTime();
    //                                                   0            1             2              3              4              5
    QueryResult result = WorldDatabase.Query("SELECT spell_id, effectradius0, effectradius1, effectradius2, effectradius3, effectradius4 from spell_radius");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell effect radius records. DB table `spell_radius` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();
        uint32 effectradius0 = fields[1].GetUInt32();
        uint32 effectradius1 = fields[2].GetUInt32();
        uint32 effectradius2 = fields[3].GetUInt32();
        uint32 effectradius3 = fields[4].GetUInt32();
        uint32 effectradius4 = fields[5].GetUInt32();

        // check if valid radius
        SpellInfo* spellInfo = mSpellInfoMap[spell_id];
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "spell_id %u in `spell_radius` table is not found in dbcs, skipped", spell_id);
            continue;
        }

        if (effectradius0 && sSpellRadiusStore.LookupEntry(effectradius0))
        {
            spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(effectradius0);
        }

        if (effectradius1 && sSpellRadiusStore.LookupEntry(effectradius1))
        {
            spellInfo->Effects[EFFECT_1].RadiusEntry = sSpellRadiusStore.LookupEntry(effectradius1);
        }

        if (effectradius2 && sSpellRadiusStore.LookupEntry(effectradius2))
        {
            spellInfo->Effects[EFFECT_2].RadiusEntry = sSpellRadiusStore.LookupEntry(effectradius2);
        }

        if (effectradius3 && sSpellRadiusStore.LookupEntry(effectradius3))
        {
            spellInfo->Effects[EFFECT_3].RadiusEntry = sSpellRadiusStore.LookupEntry(effectradius3);
        }

        if (effectradius4 && sSpellRadiusStore.LookupEntry(effectradius4))
        {
            spellInfo->Effects[EFFECT_4].RadiusEntry = sSpellRadiusStore.LookupEntry(effectradius4);
        }

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell effect radius records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadTalentSpellInfo()
{
    for (TalentEntry const* talent : sTalentStore)
        mTalentSpellInfo.insert(talent->SpellID);
}

void SpellMgr::LoadSpellPowerInfo()
{
    mSpellPowerInfo.resize(sSpellStore.GetNumRows());
    for (SpellPowerEntry const* spellPower : sSpellPowerStore)
        mSpellPowerInfo[spellPower->SpellID].push_back(spellPower->ID);
}

SpellPowerEntry const* SpellMgr::GetSpellPowerEntryByIdAndPower(uint32 id, Powers power) const
{
    for (std::list<uint32>::iterator itr = GetSpellPowerList(id).begin(); itr != GetSpellPowerList(id).end(); ++itr)
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(*itr);
        if(!spellPower)
            continue;

        if(spellPower->PowerType == power)
            return spellPower;
    }

    SpellInfo const* spell = sSpellMgr->GetSpellInfo(id);
    return spell->GetPowerInfo(0);
}
