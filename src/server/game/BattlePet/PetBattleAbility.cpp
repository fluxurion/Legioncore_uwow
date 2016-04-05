/*
* Copyright (C) 2012-2016 Uwow <https://uwow.biz/>
* Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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

#include "PetBattleAbility.h"

/*std::unordered_map<PetBattleAbilityEffectName, PetBattleAbility::EffectInfo> PetBattleAbility::_effectsInfo
{
    {EFFECT_DO_NOTHING,                         {PETBATTLE_EFFECT_TYPE_INVALID,         PetBattleEffectTarget::NONE,    &PetBattleAbility::EffectUnused}},
    {EFFECT_STANDARD_HEAL,                      {PETBATTLE_EFFECT_TYPE_SET_HEALTH,      PetBattleEffectTarget::CASTER,  &PetBattleAbility::EffectNULL}},
    {EFFECT_STANDARD_DAMAGE,                    {PETBATTLE_EFFECT_TYPE_SET_HEALTH,      PetBattleEffectTarget::CASTER,  &PetBattleAbility::EffectDealDamage}},
    {EFFECT_PET_TRAP,                           {PETBATTLE_EFFECT_TYPE_STATUS_CHANGE,   PetBattleEffectTarget::ENEMY,   &PetBattleAbility::EffectNULL}},
    //{EFFECT_APPLY_AURA_26,                    PetBattleEffectTarget::TARGET_ENEMY}, 
    {EFFECT_INCREASING_DAMAGE,                  {PETBATTLE_EFFECT_TYPE_AURA_APPLY,      PetBattleEffectTarget::CASTER,  &PetBattleAbility::EffectHeal}},
    //{EFFECT_APPLY_AURA_28,                      PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DAMAGE_IF_STATE,               PetBattleEffectTarget::ENEMY}, 
    {EFFECT_SET_STATE,                          {PETBATTLE_EFFECT_TYPE_SET_STATE,       PetBattleEffectTarget::CASTER,   &PetBattleAbility::EffectNULL}},
    //{EFFECT_HEAL_PCT_OF_DAMAGE_DEALT,           PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_HEAL_PCT_OF_DAMAGE_TAKEN,           PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_REMOVE_AURA,                        PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_HEAL_PCT_OF_MAX_HEALTH,             PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_APPLY_AURA_54,                      PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DAMAGE_IF_LESS_HP,             PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_HEAL_PCT_CONSUME_CORPSE,            PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DAMAGE_PCT_OF_MAX_HP,          PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_APPLY_AURA_63,                      PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DAMAGE_WITH_BONUS,             PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DOUBLE_DAMAGE_BELOW_25_PCT,    PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_EQUALIZE_HEALTH,                    PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DAMAGE_PCT_OF_USERS_MAX_HP,    PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_APPLY_OR_CONSUME_AURA_TO_HEAL,      PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_INITIALIZE_PET_BATTLE,              PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_APPLY_OR_CONSUME_AURA_TO_DMG_75,    PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_APPLY_OR_CONSUME_AURA_TO_DMG_76,    PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_APPLY_OR_CONSUME_AURA_TO_DMG_77,    PetBattleEffectTarget::ENEMY}, 
    {EFFECT_STATE_CHANGE,                       {PETBATTLE_EFFECT_TYPE_SET_STATE,       PetBattleEffectTarget::CASTER,   &PetBattleAbility::EffectNULL}},
    {EFFECT_WEATHER,                            {PETBATTLE_EFFECT_TYPE_AURA_APPLY,      PetBattleEffectTarget::WEATHER,  &PetBattleAbility::EffectNULL}},
    //{EFFECT_DEAL_DAMAGE_HIT_CHANCE_CONDITION,   PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_HEAL_WITH_PET_TYPE_CONDITION,       PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_DEAL_DAMAGE_IF_FIRST,               PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_HEAL_WITH_STATE_CONDITION,          PetBattleEffectTarget::ENEMY}, 
    //{EFFECT_INSTA_KILL_CASTER,                  PetBattleEffectTarget::ENEMY}, 
};*/

void PetBattleAbility::EffectNULL()
{ }

void PetBattleAbility::EffectUnused()
{ }

void PetBattleAbility::EffectDealDamage()
{ }

void PetBattleAbility::EffectHeal()
{ }
