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
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Unit.h"
#include "ObjectAccessor.h"
#include "Util.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "Battleground.h"
#include "OutdoorPvPMgr.h"
#include "Formulas.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "ScriptMgr.h"
#include "Vehicle.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "WeatherMgr.h"
#include "AreaTrigger.h"
#include "MiscPackets.h"

class Aura;
//
// EFFECT HANDLER NOTES
//
// in aura handler there should be check for modes:
// AURA_EFFECT_HANDLE_REAL set
// AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK set
// AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK set - aura is recalculated or is just applied/removed - need to redo all things related to m_amount
// AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK - logical or of above conditions
// AURA_EFFECT_HANDLE_STAT - set when stats are reapplied
// such checks will speedup trinity change amount/send for client operations
// because for change amount operation packets will not be send
// aura effect handlers shouldn't contain any AuraEffect or Aura object modifications

pAuraEffectHandler AuraEffectHandler[TOTAL_AURAS]=
{
    &AuraEffect::HandleNULL,                                      //  0 SPELL_AURA_NONE
    &AuraEffect::HandleBindSight,                                 //  1 SPELL_AURA_BIND_SIGHT
    &AuraEffect::HandleModPossess,                                //  2 SPELL_AURA_MOD_POSSESS
    &AuraEffect::HandleNoImmediateEffect,                         //  3 SPELL_AURA_PERIODIC_DAMAGE implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleAuraDummy,                                 //  4 SPELL_AURA_DUMMY
    &AuraEffect::HandleModConfuse,                                //  5 SPELL_AURA_MOD_CONFUSE
    &AuraEffect::HandleModCharm,                                  //  6 SPELL_AURA_MOD_CHARM
    &AuraEffect::HandleModFear,                                   //  7 SPELL_AURA_MOD_FEAR
    &AuraEffect::HandleNoImmediateEffect,                         //  8 SPELL_AURA_PERIODIC_HEAL implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleModAttackSpeed,                            //  9 SPELL_AURA_MOD_ATTACKSPEED
    &AuraEffect::HandleModThreat,                                 // 10 SPELL_AURA_MOD_THREAT
    &AuraEffect::HandleModTaunt,                                  // 11 SPELL_AURA_MOD_TAUNT
    &AuraEffect::HandleAuraModStun,                               // 12 SPELL_AURA_MOD_STUN
    &AuraEffect::HandleModDamageDone,                             // 13 SPELL_AURA_MOD_DAMAGE_DONE
    &AuraEffect::HandleNoImmediateEffect,                         // 14 SPELL_AURA_MOD_DAMAGE_TAKEN implemented in Unit::MeleeDamageBonus and Unit::SpellDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         // 15 SPELL_AURA_DAMAGE_SHIELD    implemented in Unit::DoAttackDamage
    &AuraEffect::HandleModStealth,                                // 16 SPELL_AURA_MOD_STEALTH
    &AuraEffect::HandleModStealthDetect,                          // 17 SPELL_AURA_MOD_STEALTH_DETECT
    &AuraEffect::HandleModInvisibility,                           // 18 SPELL_AURA_MOD_INVISIBILITY
    &AuraEffect::HandleModInvisibilityDetect,                     // 19 SPELL_AURA_MOD_INVISIBILITY_DETECT
    &AuraEffect::HandleNoImmediateEffect,                         // 20 SPELL_AURA_OBS_MOD_HEALTH implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         // 21 SPELL_AURA_OBS_MOD_POWER implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleAuraModResistance,                         // 22 SPELL_AURA_MOD_RESISTANCE
    &AuraEffect::HandleNoImmediateEffect,                         // 23 SPELL_AURA_PERIODIC_TRIGGER_SPELL implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         // 24 SPELL_AURA_PERIODIC_ENERGIZE implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleAuraModPacify,                             // 25 SPELL_AURA_MOD_PACIFY
    &AuraEffect::HandleAuraModRoot,                               // 26 SPELL_AURA_MOD_ROOT
    &AuraEffect::HandleAuraModSilence,                            // 27 SPELL_AURA_MOD_SILENCE
    &AuraEffect::HandleNoImmediateEffect,                         // 28 SPELL_AURA_REFLECT_SPELLS        implement in Unit::SpellHitResult
    &AuraEffect::HandleAuraModStat,                               // 29 SPELL_AURA_MOD_STAT
    &AuraEffect::HandleAuraModSkill,                              // 30 SPELL_AURA_MOD_SKILL
    &AuraEffect::HandleAuraModIncreaseSpeed,                      // 31 SPELL_AURA_MOD_INCREASE_SPEED
    &AuraEffect::HandleAuraModIncreaseMountedSpeed,               // 32 SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED
    &AuraEffect::HandleAuraModDecreaseSpeed,                      // 33 SPELL_AURA_MOD_DECREASE_SPEED
    &AuraEffect::HandleAuraModIncreaseHealth,                     // 34 SPELL_AURA_MOD_INCREASE_HEALTH
    &AuraEffect::HandleAuraModIncreaseEnergy,                     // 35 SPELL_AURA_MOD_INCREASE_ENERGY
    &AuraEffect::HandleAuraModShapeshift,                         // 36 SPELL_AURA_MOD_SHAPESHIFT
    &AuraEffect::HandleAuraModEffectImmunity,                     // 37 SPELL_AURA_EFFECT_IMMUNITY
    &AuraEffect::HandleAuraModStateImmunity,                      // 38 SPELL_AURA_STATE_IMMUNITY
    &AuraEffect::HandleAuraModSchoolImmunity,                     // 39 SPELL_AURA_SCHOOL_IMMUNITY
    &AuraEffect::HandleAuraModDmgImmunity,                        // 40 SPELL_AURA_DAMAGE_IMMUNITY
    &AuraEffect::HandleAuraModDispelImmunity,                     // 41 SPELL_AURA_DISPEL_IMMUNITY
    &AuraEffect::HandleNoImmediateEffect,                         // 42 SPELL_AURA_PROC_TRIGGER_SPELL  implemented in Unit::ProcDamageAndSpellFor and Unit::HandleProcTriggerSpell
    &AuraEffect::HandleNoImmediateEffect,                         // 43 SPELL_AURA_PROC_TRIGGER_DAMAGE implemented in Unit::ProcDamageAndSpellFor
    &AuraEffect::HandleAuraTrackCreatures,                        // 44 SPELL_AURA_TRACK_CREATURES
    &AuraEffect::HandleAuraTrackResources,                        // 45 SPELL_AURA_TRACK_RESOURCES
    &AuraEffect::HandleNULL,                                      // 46 SPELL_AURA_46 (used in test spells 54054 and 54058, and spell 48050) (3.0.8a)
    &AuraEffect::HandleAuraModParryPercent,                       // 47 SPELL_AURA_MOD_PARRY_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         // 48 SPELL_AURA_PERIODIC_TRIGGER_SPELL_BY_CLIENT
    &AuraEffect::HandleAuraModDodgePercent,                       // 49 SPELL_AURA_MOD_DODGE_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         // 50 SPELL_AURA_MOD_CRITICAL_HEALING_AMOUNT implemented in Unit::SpellCriticalHealingBonus
    &AuraEffect::HandleAuraModBlockPercent,                       // 51 SPELL_AURA_MOD_BLOCK_PERCENT
    &AuraEffect::HandleAuraModWeaponCritPercent,                  // 52 SPELL_AURA_MOD_WEAPON_CRIT_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         // 53 SPELL_AURA_PERIODIC_LEECH implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleModHitChance,                              // 54 SPELL_AURA_MOD_HIT_CHANCE
    &AuraEffect::HandleModSpellHitChance,                         // 55 SPELL_AURA_MOD_SPELL_HIT_CHANCE
    &AuraEffect::HandleAuraTransform,                             // 56 SPELL_AURA_TRANSFORM
    &AuraEffect::HandleModSpellCritChance,                        // 57 SPELL_AURA_MOD_SPELL_CRIT_CHANCE
    &AuraEffect::HandleAuraModIncreaseSwimSpeed,                  // 58 SPELL_AURA_MOD_INCREASE_SWIM_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         // 59 SPELL_AURA_MOD_DAMAGE_DONE_CREATURE implemented in Unit::MeleeDamageBonus and Unit::SpellDamageBonus
    &AuraEffect::HandleAuraModPacifyAndSilence,                   // 60 SPELL_AURA_MOD_PACIFY_SILENCE
    &AuraEffect::HandleAuraModScale,                              // 61 SPELL_AURA_MOD_SCALE
    &AuraEffect::HandleNoImmediateEffect,                         // 62 SPELL_AURA_PERIODIC_HEALTH_FUNNEL implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleUnused,                                    // 63 unused (4.3.4) old SPELL_AURA_PERIODIC_MANA_FUNNEL
    &AuraEffect::HandleNoImmediateEffect,                         // 64 SPELL_AURA_PERIODIC_MANA_LEECH implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleModCastingSpeed,                           // 65 SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK
    &AuraEffect::HandleFeignDeath,                                // 66 SPELL_AURA_FEIGN_DEATH
    &AuraEffect::HandleAuraModDisarm,                             // 67 SPELL_AURA_MOD_DISARM
    &AuraEffect::HandleAuraModStalked,                            // 68 SPELL_AURA_MOD_STALKED
    &AuraEffect::HandleNoImmediateEffect,                         // 69 SPELL_AURA_SCHOOL_ABSORB implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleUnused,                                    // 70 SPELL_AURA_EXTRA_ATTACKS clientside
    &AuraEffect::HandleModSpellCritChanceShool,                   // 71 SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL
    &AuraEffect::HandleModPowerCostPCT,                           // 72 SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT
    &AuraEffect::HandleModPowerCost,                              // 73 SPELL_AURA_MOD_POWER_COST_SCHOOL
    &AuraEffect::HandleNoImmediateEffect,                         // 74 SPELL_AURA_REFLECT_SPELLS_SCHOOL  implemented in Unit::SpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         // 75 SPELL_AURA_MOD_LANGUAGE
    &AuraEffect::HandleNoImmediateEffect,                         // 76 SPELL_AURA_FAR_SIGHT
    &AuraEffect::HandleModMechanicImmunity,                       // 77 SPELL_AURA_MECHANIC_IMMUNITY
    &AuraEffect::HandleAuraMounted,                               // 78 SPELL_AURA_MOUNTED
    &AuraEffect::HandleModDamagePercentDone,                      // 79 SPELL_AURA_MOD_DAMAGE_PERCENT_DONE
    &AuraEffect::HandleModPercentStat,                            // 80 SPELL_AURA_MOD_PERCENT_STAT
    &AuraEffect::HandleNoImmediateEffect,                         // 81 SPELL_AURA_SPLIT_DAMAGE_PCT implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleWaterBreathing,                            // 82 SPELL_AURA_WATER_BREATHING
    &AuraEffect::HandleModBaseResistance,                         // 83 SPELL_AURA_MOD_BASE_RESISTANCE
    &AuraEffect::HandleNoImmediateEffect,                         // 84 SPELL_AURA_MOD_REGEN implemented in Player::RegenerateHealth
    &AuraEffect::HandleModPowerRegen,                             // 85 SPELL_AURA_MOD_POWER_REGEN implemented in Player::Regenerate
    &AuraEffect::HandleChannelDeathItem,                          // 86 SPELL_AURA_CHANNEL_DEATH_ITEM
    &AuraEffect::HandleNoImmediateEffect,                         // 87 SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN implemented in Unit::MeleeDamageBonus and Unit::SpellDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         // 88 SPELL_AURA_MOD_HEALTH_REGEN_PERCENT implemented in Player::RegenerateHealth
    &AuraEffect::HandleNoImmediateEffect,                         // 89 SPELL_AURA_PERIODIC_DAMAGE_PERCENT
    &AuraEffect::HandleUnused,                                    // 90 unused old SPELL_AURA_MOD_RESIST_CHANCE
    &AuraEffect::HandleNoImmediateEffect,                         // 91 SPELL_AURA_MOD_DETECT_RANGE implemented in Creature::GetAttackDistance
    &AuraEffect::HandlePreventFleeing,                            // 92 SPELL_AURA_PREVENTS_FLEEING
    &AuraEffect::HandleModUnattackable,                           // 93 SPELL_AURA_MOD_UNATTACKABLE
    &AuraEffect::HandleNoImmediateEffect,                         // 94 SPELL_AURA_INTERRUPT_REGEN implemented in Player::Regenerate
    &AuraEffect::HandleAuraGhost,                                 // 95 SPELL_AURA_GHOST
    &AuraEffect::HandleNoImmediateEffect,                         // 96 SPELL_AURA_SPELL_MAGNET implemented in Unit::SelectMagnetTarget
    &AuraEffect::HandleNoImmediateEffect,                         // 97 SPELL_AURA_MANA_SHIELD implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleAuraModSkill,                              // 98 SPELL_AURA_MOD_SKILL_TALENT
    &AuraEffect::HandleAuraModAttackPower,                        // 99 SPELL_AURA_MOD_ATTACK_POWER
    &AuraEffect::HandleUnused,                                    //100 SPELL_AURA_AURAS_VISIBLE obsolete? all player can see all auras now, but still have spells including GM-spell
    &AuraEffect::HandleModResistancePercent,                      //101 SPELL_AURA_MOD_RESISTANCE_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //102 SPELL_AURA_MOD_MELEE_ATTACK_POWER_VERSUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModTotalThreat,                        //103 SPELL_AURA_MOD_TOTAL_THREAT
    &AuraEffect::HandleAuraWaterWalk,                             //104 SPELL_AURA_WATER_WALK
    &AuraEffect::HandleAuraFeatherFall,                           //105 SPELL_AURA_FEATHER_FALL
    &AuraEffect::HandleAuraHover,                                 //106 SPELL_AURA_HOVER
    &AuraEffect::HandleNoImmediateEffect,                         //107 SPELL_AURA_ADD_FLAT_MODIFIER implemented in AuraEffect::CalculateSpellMod()
    &AuraEffect::HandleNoImmediateEffect,                         //108 SPELL_AURA_ADD_PCT_MODIFIER implemented in AuraEffect::CalculateSpellMod()
    &AuraEffect::HandleNoImmediateEffect,                         //109 SPELL_AURA_ADD_TARGET_TRIGGER
    &AuraEffect::HandleModPowerRegenPCT,                          //110 SPELL_AURA_MOD_POWER_REGEN_PERCENT implemented in Player::Regenerate, Creature::Regenerate
    &AuraEffect::HandleNoImmediateEffect,                         //111 SPELL_AURA_ADD_CASTER_HIT_TRIGGER implemented in Unit::SelectMagnetTarget
    &AuraEffect::HandleNoImmediateEffect,                         //112 SPELL_AURA_OVERRIDE_CLASS_SCRIPTS
    &AuraEffect::HandleNoImmediateEffect,                         //113 SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //114 SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //115 SPELL_AURA_MOD_HEALING                 implemented in Unit::SpellBaseHealingBonusTaken
    &AuraEffect::HandleNoImmediateEffect,                         //116 SPELL_AURA_MOD_REGEN_DURING_COMBAT
    &AuraEffect::HandleNoImmediateEffect,                         //117 SPELL_AURA_MOD_MECHANIC_RESISTANCE     implemented in Unit::MagicSpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         //118 SPELL_AURA_MOD_HEALING_PCT             implemented in Unit::SpellHealingBonus
    &AuraEffect::HandleUnused,                                    //119 unused old SPELL_AURA_SHARE_PET_TRACKING
    &AuraEffect::HandleAuraUntrackable,                           //120 SPELL_AURA_UNTRACKABLE
    &AuraEffect::HandleAuraEmpathy,                               //121 SPELL_AURA_EMPATHY
    &AuraEffect::HandleModOffhandDamagePercent,                   //122 SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT
    &AuraEffect::HandleModTargetResistance,                       //123 SPELL_AURA_MOD_TARGET_RESISTANCE
    &AuraEffect::HandleAuraModRangedAttackPower,                  //124 SPELL_AURA_MOD_RANGED_ATTACK_POWER
    &AuraEffect::HandleNoImmediateEffect,                         //125 SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //126 SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //127 SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleModPossessPet,                             //128 SPELL_AURA_MOD_POSSESS_PET
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //129 SPELL_AURA_MOD_SPEED_ALWAYS
    &AuraEffect::HandleAuraModIncreaseMountedSpeed,               //130 SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS
    &AuraEffect::HandleNoImmediateEffect,                         //131 SPELL_AURA_MOD_RANGED_ATTACK_POWER_VERSUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModIncreaseEnergyPercent,              //132 SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT
    &AuraEffect::HandleAuraModIncreaseHealthPercent,              //133 SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT
    &AuraEffect::HandleAuraModRegenInterrupt,                     //134 SPELL_AURA_MOD_MANA_REGEN_INTERRUPT
    &AuraEffect::HandleModHealingDone,                            //135 SPELL_AURA_MOD_HEALING_DONE
    &AuraEffect::HandleNoImmediateEffect,                         //136 SPELL_AURA_MOD_HEALING_DONE_PERCENT   implemented in Unit::SpellHealingBonus
    &AuraEffect::HandleModTotalPercentStat,                       //137 SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE
    &AuraEffect::HandleModMeleeSpeedPct,                          //138 SPELL_AURA_MOD_MELEE_HASTE
    &AuraEffect::HandleForceReaction,                             //139 SPELL_AURA_FORCE_REACTION
    &AuraEffect::HandleAuraModRangedHaste,                        //140 SPELL_AURA_MOD_RANGED_HASTE
    &AuraEffect::HandleUnused,                                    //141 SPELL_AURA_141
    &AuraEffect::HandleAuraModBaseResistancePCT,                  //142 SPELL_AURA_MOD_BASE_RESISTANCE_PCT
    &AuraEffect::HandleAuraModResistanceExclusive,                //143 6.1.2 - 6.2.3 UNUSED        SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE
    &AuraEffect::HandleNoImmediateEffect,                         //144 SPELL_AURA_SAFE_FALL                         implemented in WorldSession::HandleMovementOpcodes
    &AuraEffect::HandleAuraModPetTalentsPoints,                   //145 SPELL_AURA_MOD_PET_TALENT_POINTS
    &AuraEffect::HandleNoImmediateEffect,                         //146 SPELL_AURA_ALLOW_TAME_PET_TYPE
    &AuraEffect::HandleModStateImmunityMask,                      //147 SPELL_AURA_MECHANIC_IMMUNITY_MASK
    &AuraEffect::HandleAuraRetainComboPoints,                     //148 SPELL_AURA_RETAIN_COMBO_POINTS
    &AuraEffect::HandleNoImmediateEffect,                         //149 SPELL_AURA_REDUCE_PUSHBACK
    &AuraEffect::HandleShieldBlockValue,                          //150 SPELL_AURA_MOD_SHIELD_BLOCKVALUE_PCT
    &AuraEffect::HandleAuraTrackStealthed,                        //151 SPELL_AURA_TRACK_STEALTHED
    &AuraEffect::HandleNoImmediateEffect,                         //152 SPELL_AURA_MOD_DETECTED_RANGE implemented in Creature::GetAttackDistance
    &AuraEffect::HandleUnused,                                    //153 Unused old SPELL_AURA_SPLIT_DAMAGE_FLAT
    &AuraEffect::HandleModStealthLevel,                           //154 SPELL_AURA_MOD_STEALTH_LEVEL
    &AuraEffect::HandleNoImmediateEffect,                         //155 SPELL_AURA_MOD_WATER_BREATHING
    &AuraEffect::HandleNoImmediateEffect,                         //156 SPELL_AURA_MOD_REPUTATION_GAIN
    &AuraEffect::HandleUnused,                                    //157 SPELL_AURA_PET_DAMAGE_MULTI
    &AuraEffect::HandleShieldBlockValue,                          //158 SPELL_AURA_MOD_SHIELD_BLOCKVALUE
    &AuraEffect::HandleNoImmediateEffect,                         //159 SPELL_AURA_NO_PVP_CREDIT      only for Honorless Target spell
    &AuraEffect::HandleUnused,                                    //160 Unused old SPELL_AURA_MOD_AOE_AVOIDANCE
    &AuraEffect::HandleNoImmediateEffect,                         //161 SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT
    &AuraEffect::HandleNoImmediateEffect,                         //162 SPELL_AURA_POWER_BURN implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //163 SPELL_AURA_MOD_CRIT_DAMAGE_BONUS
    &AuraEffect::HandleNULL,                                      //164
    &AuraEffect::HandleNoImmediateEffect,                         //165 SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS implemented in Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModAttackPowerPercent,                 //166 SPELL_AURA_MOD_ATTACK_POWER_PCT
    &AuraEffect::HandleAuraModRangedAttackPowerPercent,           //167 SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //168 SPELL_AURA_MOD_DAMAGE_DONE_VERSUS            implemented in Unit::SpellDamageBonus, Unit::MeleeDamageBonus
    &AuraEffect::HandleUnused,                                    //169 Unused (4.3.4) old SPELL_AURA_MOD_CRIT_PERCENT_VERSUS
    &AuraEffect::HandleNULL,                                      //170 SPELL_AURA_DETECT_AMORE       various spells that change visual of units for aura target (clientside?)
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //171 SPELL_AURA_MOD_SPEED_NOT_STACK
    &AuraEffect::HandleAuraModIncreaseMountedSpeed,               //172 SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK
    &AuraEffect::HandleUnused,                                    //173 unused (4.3.4) no spells, old SPELL_AURA_ALLOW_CHAMPION_SPELLS  only for Proclaim Champion spell
    &AuraEffect::HandleModSpellDamagePercentFromStat,             //174 SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT  implemented in Unit::SpellBaseDamageBonus
    &AuraEffect::HandleModSpellHealingPercentFromStat,            //175 SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT implemented in Unit::SpellBaseHealingBonus
    &AuraEffect::HandleSpiritOfRedemption,                        //176 SPELL_AURA_SPIRIT_OF_REDEMPTION   only for Spirit of Redemption spell, die at aura end
    &AuraEffect::HandleCharmConvert,                              //177 SPELL_AURA_AOE_CHARM
    &AuraEffect::HandleUnused,                                    //178 old SPELL_AURA_MOD_DEBUFF_RESISTANCE unused 4.3.4
    &AuraEffect::HandleNoImmediateEffect,                         //179 SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE implemented in Unit::SpellCriticalBonus
    &AuraEffect::HandleNoImmediateEffect,                         //180 SPELL_AURA_MOD_FLAT_SPELL_DAMAGE_VERSUS   implemented in Unit::SpellDamageBonus
    &AuraEffect::HandleUnused,                                    //181 unused old SPELL_AURA_MOD_FLAT_SPELL_CRIT_DAMAGE_VERSUS
    &AuraEffect::HandleAuraModResistenceOfStatPercent,            //182 SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT
    &AuraEffect::HandleNULL,                                      //183 SPELL_AURA_MOD_CRITICAL_THREAT only used in 28746 - miscvalue - spell school
    &AuraEffect::HandleNoImmediateEffect,                         //184 SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE  implemented in Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //185 SPELL_AURA_MOD_ATTACKER_RANGED_HIT_CHANCE implemented in Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //186 SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE  implemented in Unit::MagicSpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         //187 SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE  implemented in Unit::GetUnitCriticalChance
    &AuraEffect::HandleNoImmediateEffect,                         //188 SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_CHANCE implemented in Unit::GetUnitCriticalChance
    &AuraEffect::HandleModRating,                                 //189 SPELL_AURA_MOD_RATING
    &AuraEffect::HandleNoImmediateEffect,                         //190 SPELL_AURA_MOD_FACTION_REPUTATION_GAIN     implemented in Player::CalculateReputationGain
    &AuraEffect::HandleAuraModUseNormalSpeed,                     //191 SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED
    &AuraEffect::HandleModMeleeRangedSpeedPct,                    //192 SPELL_AURA_MOD_MELEE_RANGED_HASTE
    &AuraEffect::HandleModCombatSpeedPct,                         //193 SPELL_AURA_MELEE_SLOW (in fact combat (any type attack) speed pct)
    &AuraEffect::HandleNoImmediateEffect,                         //194 SPELL_AURA_MOD_TARGET_ABSORB_SCHOOL implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleNoImmediateEffect,                         //195 SPELL_AURA_MOD_TARGET_ABILITY_ABSORB_SCHOOL implemented in Unit::CalcAbsorbResist
    &AuraEffect::HandleNULL,                                      //196 SPELL_AURA_MOD_COOLDOWN - flat mod of spell cooldowns
    &AuraEffect::HandleNoImmediateEffect,                         //197 SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE implemented in Unit::SpellCriticalBonus Unit::GetUnitCriticalChance
    &AuraEffect::HandleUnused,                                    //198 unused old SPELL_AURA_MOD_ALL_WEAPON_SKILLS
    &AuraEffect::HandleUnused,                                    //199 unused old SPELL_AURA_MOD_INCREASES_SPELL_PCT_TO_HIT
    &AuraEffect::HandleNoImmediateEffect,                         //200 SPELL_AURA_MOD_XP_PCT implemented in Player::RewardPlayerAndGroupAtKill
    &AuraEffect::HandleAuraAllowFlight,                           //201 SPELL_AURA_FLY                             this aura enable flight mode...
    &AuraEffect::HandleNoImmediateEffect,                         //202 SPELL_AURA_CANNOT_BE_DODGED                implemented in Unit::RollPhysicalOutcomeAgainst
    &AuraEffect::HandleNoImmediateEffect,                         //203 SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_DAMAGE  implemented in Unit::CalculateMeleeDamage and Unit::CalculateSpellDamage
    &AuraEffect::HandleNoImmediateEffect,                         //204 SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_DAMAGE implemented in Unit::CalculateMeleeDamage and Unit::CalculateSpellDamage
    &AuraEffect::HandleNULL,                                      //205 SPELL_AURA_MOD_SCHOOL_CRIT_DMG_TAKEN
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //206 SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //207 SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //208 SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //209 SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //210 SPELL_AURA_MOD_VEHICLE_SPEED_ALWAYS
    &AuraEffect::HandleAuraModIncreaseFlightSpeed,                //211 SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK
    &AuraEffect::HandleUnused,                                    //212 Unused old SPELL_AURA_MOD_RANGED_ATTACK_POWER_OF_STAT_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         //213 SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT implemented in Player::RewardRage
    &AuraEffect::HandleNULL,                                      //214 Tamed Pet Passive
    &AuraEffect::HandleArenaPreparation,                          //215 SPELL_AURA_ARENA_PREPARATION
    &AuraEffect::HandleModCastingSpeed,                           //216 SPELL_AURA_HASTE_SPELLS
    &AuraEffect::HandleModMeleeSpeedPct,                          //217 SPELL_AURA_MOD_MELEE_HASTE_2
    &AuraEffect::HandleAuraModRangedHaste,                        //218 SPELL_AURA_HASTE_RANGED
    &AuraEffect::HandleModManaRegen,                              //219 SPELL_AURA_MOD_MANA_REGEN_FROM_STAT
    &AuraEffect::HandleModRatingFromStat,                         //220 SPELL_AURA_MOD_RATING_FROM_STAT
    &AuraEffect::HandleNULL,                                      //221 SPELL_AURA_MOD_DETAUNT
    &AuraEffect::HandleNoImmediateEffect,                         //222 SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE2
    &AuraEffect::HandleNoImmediateEffect,                         //223 SPELL_AURA_RAID_PROC_FROM_CHARGE
    &AuraEffect::HandleUnused,                                    //224 unused (4.3.4)
    &AuraEffect::HandleNoImmediateEffect,                         //225 SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE
    &AuraEffect::HandleNoImmediateEffect,                         //226 SPELL_AURA_PERIODIC_DUMMY implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //227 SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //228 SPELL_AURA_DETECT_STEALTH stealth detection
    &AuraEffect::HandleNoImmediateEffect,                         //229 SPELL_AURA_MOD_AOE_DAMAGE_AVOIDANCE
    &AuraEffect::HandleAuraModIncreaseHealth,                     //230 SPELL_AURA_MOD_INCREASE_HEALTH_2
    &AuraEffect::HandleNoImmediateEffect,                         //231 SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE
    &AuraEffect::HandleNoImmediateEffect,                         //232 SPELL_AURA_MECHANIC_DURATION_MOD           implement in Unit::CalculateSpellDuration
    &AuraEffect::HandleUnused,                                    //233 set model id to the one of the creature with id GetMiscValue() - clientside
    &AuraEffect::HandleNoImmediateEffect,                         //234 SPELL_AURA_MECHANIC_DURATION_MOD_NOT_STACK implement in Unit::CalculateSpellDuration
    &AuraEffect::HandleNoImmediateEffect,                         //235 SPELL_AURA_MOD_DISPEL_RESIST               implement in Unit::MagicSpellHitResult
    &AuraEffect::HandleAuraControlVehicle,                        //236 SPELL_AURA_CONTROL_VEHICLE
    &AuraEffect::HandleModSpellDamagePercentFromAttackPower,      //237 SPELL_AURA_MOD_SPELL_DAMAGE_OF_ATTACK_POWER  implemented in Unit::SpellBaseDamageBonus
    &AuraEffect::HandleModSpellHealingPercentFromAttackPower,     //238 SPELL_AURA_MOD_SPELL_HEALING_OF_ATTACK_POWER implemented in Unit::SpellBaseHealingBonus
    &AuraEffect::HandleAuraModScale,                              //239 SPELL_AURA_MOD_SCALE_2 only in Noggenfogger Elixir (16595) before 2.3.0 aura 61
    &AuraEffect::HandleNULL,                                      //240 SPELL_AURA_MOD_EXPERTISE - new effect in wod :C
    &AuraEffect::HandleForceMoveForward,                          //241 SPELL_AURA_FORCE_MOVE_FORWARD Forces the caster to move forward
    &AuraEffect::HandleUnused,                                    //242 SPELL_AURA_MOD_SPELL_DAMAGE_FROM_HEALING
    &AuraEffect::HandleAuraModFaction,                            //243 SPELL_AURA_MOD_FACTION
    &AuraEffect::HandleComprehendLanguage,                        //244 SPELL_AURA_COMPREHEND_LANGUAGE
    &AuraEffect::HandleNoImmediateEffect,                         //245 SPELL_AURA_MOD_AURA_DURATION_BY_DISPEL
    &AuraEffect::HandleNoImmediateEffect,                         //246 SPELL_AURA_MOD_AURA_DURATION_BY_DISPEL_NOT_STACK implemented in Spell::EffectApplyAura
    &AuraEffect::HandleAuraCloneCaster,                           //247 SPELL_AURA_CLONE_CASTER
    &AuraEffect::HandleNoImmediateEffect,                         //248 SPELL_AURA_MOD_COMBAT_RESULT_CHANCE         implemented in Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleAuraConvertRune,                           //249 SPELL_AURA_CONVERT_RUNE
    &AuraEffect::HandleAuraModIncreaseHealth,                     //250 SPELL_AURA_MOD_INCREASE_HEALTH_3
    &AuraEffect::HandleNoImmediateEffect,                         //251 SPELL_AURA_MOD_ENEMY_DODGE
    &AuraEffect::HandleModCombatSpeedPct,                         //252 SPELL_AURA_252 Is there any difference between this and SPELL_AURA_MELEE_SLOW ? maybe not stacking mod?
    &AuraEffect::HandleNoImmediateEffect,                         //253 SPELL_AURA_MOD_BLOCK_CRIT_CHANCE  implemented in Unit::isBlockCritical
    &AuraEffect::HandleAuraModDisarm,                             //254 SPELL_AURA_MOD_DISARM_OFFHAND
    &AuraEffect::HandleNoImmediateEffect,                         //255 SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT    implemented in Unit::SpellDamageBonus
    &AuraEffect::HandleNoReagentUseAura,                          //256 SPELL_AURA_NO_REAGENT_USE Use SpellClassMask for spell select
    &AuraEffect::HandleUnused,                                    //257 SPELL_AURA_MOD_TARGET_RESIST_BY_SPELL_CLASS
    &AuraEffect::HandleUnused,                                    //258 SPELL_AURA_258
    &AuraEffect::HandleNoImmediateEffect,                         //259 SPELL_AURA_MOD_HOT_PCT implemented in Unit::SpellHealingBonusTaken
    &AuraEffect::HandleNoImmediateEffect,                         //260 SPELL_AURA_SCREEN_EFFECT (miscvalue = id in ScreenEffect.dbc) not required any code
    &AuraEffect::HandlePhase,                                     //261 SPELL_AURA_PHASE
    &AuraEffect::HandleNoImmediateEffect,                         //262 SPELL_AURA_ABILITY_IGNORE_AURASTATE implemented in spell::cancast
    &AuraEffect::HandleAuraAllowOnlyAbility,                      //263 SPELL_AURA_ALLOW_ONLY_ABILITY player can use only abilities set in SpellClassMask
    &AuraEffect::HandleNULL,                                      //264 
    &AuraEffect::HandleUnused,                                    //265 unused
    &AuraEffect::HandleUnused,                                    //266 unused
    &AuraEffect::HandleNoImmediateEffect,                         //267 SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL         implemented in Unit::IsImmunedToSpellEffect
    &AuraEffect::HandleUnused,                                    //268 SPELL_AURA_268 old SPELL_AURA_MOD_ATTACK_POWER_OF_STAT_PERCENT.
    &AuraEffect::HandleNoImmediateEffect,                         //269 SPELL_AURA_MOD_IGNORE_TARGET_RESIST implemented in Unit::CalcAbsorbResist and CalcArmorReducedDamage
    &AuraEffect::HandleUnused,                                    //270 SPELL_AURA_270 old SPELL_AURA_MOD_ABILITY_IGNORE_TARGET_RESIST
    &AuraEffect::HandleNoImmediateEffect,                         //271 SPELL_AURA_MOD_DAMAGE_FROM_CASTER    implemented in Unit::SpellDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //272 SPELL_AURA_IGNORE_MELEE_RESET
    &AuraEffect::HandleUnused,                                    //273 SPELL_AURA_X_RAY
    &AuraEffect::HandleUnused,                                    //274 SPELL_AURA_274
    &AuraEffect::HandleNoImmediateEffect,                         //275 SPELL_AURA_MOD_IGNORE_SHAPESHIFT Use SpellClassMask for spell select
    &AuraEffect::HandleNoImmediateEffect,                         //276 SPELL_AURA_MOD_DAMAGE_DONE_FOR_MECHANIC
    &AuraEffect::HandleUnused,                                    //277 SPELL_AURA_277 old SPELL_AURA_MOD_MAX_AFFECTED_TARGETS
    &AuraEffect::HandleAuraModDisarm,                             //278 SPELL_AURA_MOD_DISARM_RANGED disarm ranged weapon
    &AuraEffect::HandleAuraInitializeImages,                      //279 SPELL_AURA_INITIALIZE_IMAGES
    &AuraEffect::HandleNoImmediateEffect,                         //280 SPELL_AURA_MOD_ARMOR_PENETRATION_PCT implemented in Unit::CalcArmorReducedDamage
    &AuraEffect::HandleNoImmediateEffect,                         //281 SPELL_AURA_MOD_REPUTATION_GAIN_PCT implemented in Player::RewardHonor
    &AuraEffect::HandleAuraIncreaseBaseHealthPercent,             //282 SPELL_AURA_INCREASE_BASE_HEALTH_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         //283 SPELL_AURA_MOD_HEALING_RECEIVED       implemented in Unit::SpellHealingBonus
    &AuraEffect::HandleAuraLinked,                                //284 SPELL_AURA_LINKED
    &AuraEffect::HandleAuraModAttackPowerOfArmor,                 //285 SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR  implemented in Player::UpdateAttackPowerAndDamage
    &AuraEffect::HandleNoImmediateEffect,                         //286 SPELL_AURA_ABILITY_PERIODIC_CRIT implemented in AuraEffect::PeriodicTick
    &AuraEffect::HandleNoImmediateEffect,                         //287 SPELL_AURA_DEFLECT_SPELLS             implemented in Unit::MagicSpellHitResult and Unit::MeleeSpellHitResult
    &AuraEffect::HandleNoImmediateEffect,                         //288 SPELL_AURA_IGNORE_HIT_DIRECTION  implemented in Unit::MagicSpellHitResult and Unit::MeleeSpellHitResult Unit::RollMeleeOutcomeAgainst
    &AuraEffect::HandleNULL,                                      //289 SPELL_AURA_289
    &AuraEffect::HandleAuraModCritPct,                            //290 SPELL_AURA_MOD_CRIT_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //291 SPELL_AURA_MOD_XP_QUEST_PCT  implemented in Player::RewardQuest
    &AuraEffect::HandleAuraOpenStable,                            //292 SPELL_AURA_OPEN_STABLE
    &AuraEffect::HandleAuraOverrideSpells,                        //293 SPELL_AURA_OVERRIDE_SPELLS
    &AuraEffect::HandleNoImmediateEffect,                         //294 SPELL_AURA_PREVENT_REGENERATE_POWER implemented in Player::Regenerate(Powers power)
    &AuraEffect::HandleUnused,                                    //295 SPELL_AURA_295
    &AuraEffect::HandleAuraSetVehicle,                            //296 SPELL_AURA_SET_VEHICLE_ID sets vehicle on target
    &AuraEffect::HandleNoImmediateEffect,                         //297 SPELL_AURA_BLOCK_SPELL_FAMILY
    &AuraEffect::HandleAuraStrangulate,                           //298 SPELL_AURA_STRANGULATE 70569 - Strangulating, maybe prevents talk or cast
    &AuraEffect::HandleUnused,                                    //299 SPELL_AURA_299
    &AuraEffect::HandleNoImmediateEffect,                         //300 SPELL_AURA_SHARE_DAMAGE_PCT implemented in Unit::DealDamage
    &AuraEffect::HandleNoImmediateEffect,                         //301 SPELL_AURA_SCHOOL_HEAL_ABSORB implemented in Unit::CalcHealAbsorb
    &AuraEffect::HandleUnused,                                    //302 SPELL_AURA_302
    &AuraEffect::HandleNoImmediateEffect,                         //303 SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE implemented in Unit::SpellDamageBonus, Unit::MeleeDamageBonus
    &AuraEffect::HandleAuraModFakeInebriation,                    //304 SPELL_AURA_MOD_DRUNK
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //305 SPELL_AURA_MOD_MINIMUM_SPEED
    &AuraEffect::HandleUnused,                                    //306 SPELL_AURA_306
    &AuraEffect::HandleUnused,                                    //307 SPELL_AURA_HEAL_ABSORB_TEST
    &AuraEffect::HandleNoImmediateEffect,                         //308 SPELL_AURA_MOD_CRIT_CHANCE_FOR_CASTER
    &AuraEffect::HandleAuraModResiliencePct,                      //309 SPELL_AURA_MOD_RESILIENCE_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //310 SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE implemented in Spell::CalculateDamageDone
    &AuraEffect::HandleNULL,                                      //311 SPELL_AURA_311
    &AuraEffect::HandleAnimReplacementSet,                        //312 SPELL_AURA_ANIM_REPLACEMENT_SET
    &AuraEffect::HandleNULL,                                      //313 SPELL_AURA_313
    &AuraEffect::HandlePreventResurrection,                       //314 SPELL_AURA_PREVENT_RESURRECTION
    &AuraEffect::HandleNULL,                                      //315 SPELL_AURA_UNDERWATER_WALKING
    &AuraEffect::HandleUnused,                                    //316 SPELL_AURA_316
    &AuraEffect::HandleAuraModSpellPowerPercent,                  //317 SPELL_AURA_MOD_SPELL_POWER_PCT
    &AuraEffect::HandleAuraMastery,                               //318 SPELL_AURA_MASTERY
    &AuraEffect::HandleModMeleeSpeedPct,                          //319 SPELL_AURA_MOD_MELEE_HASTE_3
    &AuraEffect::HandleAuraModRangedHaste,                        //320 SPELL_AURA_MOD_RANGED_HASTE_3
    &AuraEffect::HandleModNpcFlags2,                              //321 SPELL_AURA_321
    &AuraEffect::HandleNoImmediateEffect,                         //322 SPELL_AURA_INTERFERE_TARGETTING
    &AuraEffect::HandleNULL,                                      //323 SPELL_AURA_MOD_CRITICAL_DAMAGE_FROM_MEELE_SPELLS
    &AuraEffect::HandleUnused,                                    //324 SPELL_AURA_324
    &AuraEffect::HandleUnused,                                    //325 SPELL_AURA_325
    &AuraEffect::HandlePhase,                                     //326 SPELL_AURA_PHASE_2
    &AuraEffect::HandleUnused,                                    //327 SPELL_AURA_FORCE_TO_SEE_ALL_PHASES
    &AuraEffect::HandleNoImmediateEffect,                         //328 SPELL_AURA_PROC_ON_POWER_AMOUNT implemented in Unit::HandleAuraProcOnPowerAmount
    &AuraEffect::HandleNoImmediateEffect,                         //329 SPELL_AURA_MOD_RUNE_REGEN_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         //330 SPELL_AURA_CAST_WHILE_WALKING
    &AuraEffect::HandleAuraForceWeather,                          //331 SPELL_AURA_FORCE_WEATHER
    &AuraEffect::HandleOverrideActionbarSpells,                   //332 SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS implemented in WorldSession::HandleCastSpellOpcode
    &AuraEffect::HandleOverrideActionbarSpells,                   //333 SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2 implemented in WorldSession::HandleCastSpellOpcode
    &AuraEffect::HandleNULL,                                      //334 SPELL_AURA_MOD_BLIND
    &AuraEffect::HandleAuraSeeWhileInvisible,                     //335 SPELL_AURA_SEE_WHILE_INVISIBLE
    &AuraEffect::HandleNoImmediateEffect,                         //336 SPELL_AURA_MOD_FLYING_RESTRICTIONS
    &AuraEffect::HandleNoImmediateEffect,                         //337 SPELL_AURA_MOD_VENDOR_ITEMS_PRICES
    &AuraEffect::HandleNoImmediateEffect,                         //338 SPELL_AURA_MOD_DURABILITY_LOSS
    &AuraEffect::HandleNoImmediateEffect,                         //339 SPELL_AURA_INCREASE_SKILL_GAIN_CHANCE
    &AuraEffect::HandleNoImmediateEffect,                         //340 SPELL_AURA_MOD_RESURRECTED_HEALTH_BY_GUILD_MEMBER
    &AuraEffect::HandleAuraModCategoryCooldown,                   //341 SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN
    &AuraEffect::HandleModMeleeRangedSpeedPct,                    //342 SPELL_AURA_MOD_MELEE_RANGED_HASTE_2
    &AuraEffect::HandleNoImmediateEffect,                         //343 SPELL_AURA_MOD_AUTOATTACK_DAMAGE_TARGET
    &AuraEffect::HandleNoImmediateEffect,                         //344 SPELL_AURA_MOD_AUTOATTACK_DAMAGE
    &AuraEffect::HandleNoImmediateEffect,                         //345 SPELL_AURA_BYPASS_ARMOR_FOR_CASTER
    &AuraEffect::HandleProgressBar,                               //346 SPELL_AURA_ENABLE_ALT_POWER
    &AuraEffect::HandleNoImmediateEffect,                         //347 SPELL_AURA_MOD_SPELL_COOLDOWN_BY_HASTE
    &AuraEffect::HandleNoImmediateEffect,                         //348 SPELL_AURA_AURA_DEPOSIT_BONUS_MONEY_IN_GUILD_BANK_ON_LOOT implemented in WorldSession::HandleLootMoney
    &AuraEffect::HandleNoImmediateEffect,                         //349 SPELL_AURA_MOD_CURRENCY_GAIN implemented in Player::ModifyCurrency (TODO?)
    &AuraEffect::HandleNoImmediateEffect,                         //350 SPELL_AURA_MOD_ITEM_LOOT
    &AuraEffect::HandleNoImmediateEffect,                         //351 SPELL_AURA_MOD_CURRENCY_LOOT
    &AuraEffect::HandleNULL,                                      //352 SPELL_AURA_ALLOW_WORGEN_TRANSFORM
    &AuraEffect::HandleModCamouflage,                             //353 SPELL_AURA_MOD_CAMOUFLAGE
    &AuraEffect::HandleNoImmediateEffect,                         //354 SPELL_AURA_MOD_HEALING_DONE_FROM_PCT_HEALTH
    &AuraEffect::HandleModCastingSpeed,                           //355 SPELL_AURA_MOD_CASTING_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         //356 SPELL_AURA_MOD_DAMAGE_DONE_FROM_PCT_POWER
    &AuraEffect::HandleNULL,                                      //357 SPELL_AURA_ENABLE_BOSS1_UNIT_FRAME
    &AuraEffect::HandleNULL,                                      //358 SPELL_AURA_WORGEN_ALTERED_FORM
    &AuraEffect::HandleNoImmediateEffect,                         //359 SPELL_AURA_MOD_HEALING_DONE2 implemented in Unit::SpellBaseHealingBonusDone
    &AuraEffect::HandleNoImmediateEffect,                         //360 SPELL_AURA_PROC_TRIGGER_SPELL_COPY
    &AuraEffect::HandleNoImmediateEffect,                         //361 SPELL_AURA_PROC_MELEE_TRIGGER_SPELL
    &AuraEffect::HandleUnused,                                    //362 SPELL_AURA_DUMMY2
    &AuraEffect::HandleModNextSpell,                              //363 SPELL_AURA_MOD_NEXT_SPELL
    &AuraEffect::HandleUnused,                                    //364 SPELL_AURA_364
    &AuraEffect::HandleNULL,                                      //365 SPELL_AURA_MAX_FAR_CLIP_PLANE
    &AuraEffect::HandleOverrideSpellPowerByAttackPower,           //366 SPELL_AURA_OVERRIDE_SPELL_POWER_BY_AP_PCT  implemented in Unit::SpellBaseDamageBonus
    &AuraEffect::HandleNoImmediateEffect,                         //367 SPELL_AURA_OVERRIDE_AUTOATTACK
    &AuraEffect::HandleUnused,                                    //368 SPELL_AURA_MOD_DISARM2
    &AuraEffect::HandleNULL,                                      //369 SPELL_AURA_ENABLE_POWER_BAR_TIMER
    &AuraEffect::HandleNULL,                                      //370 SPELL_AURA_SET_FAIR_FAR_CLIP
    &AuraEffect::HandleNULL,                                      //371 SPELL_AURA_371
    &AuraEffect::HandleNULL,                                      //372 SPELL_AURA_372
    &AuraEffect::HandleAuraModIncreaseSpeed,                      //373 SPELL_AURA_INCREASE_MIN_SWIM_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         //374 SPELL_AURA_MOD_FALL_DAMAGE
    &AuraEffect::HandleUnused,                                    //375 SPELL_AURA_375
    &AuraEffect::HandleNoImmediateEffect,                         //376 SPELL_AURA_MOD_CURRENCY_GAIN2
    &AuraEffect::HandleNULL,                                      //377 SPELL_AURA_CAST_WHILE_WALKING2
    &AuraEffect::HandleUnused,                                    //378 SPELL_AURA_378
    &AuraEffect::HandleModManaRegen,                              //379 SPELL_AURA_MOD_BASE_MANA_REGEN_PERCENT
    &AuraEffect::HandleUnused,                                    //380 SPELL_AURA_380
    &AuraEffect::HandleNoImmediateEffect,                         //381 SPELL_AURA_MOD_PET_HEALTH_FROM_OWNER_PCT implemented in Guardian::UpdateMaxHealth
    &AuraEffect::HandleAuraModPetStatsModifier,                   //382 SPELL_AURA_MOD_PET_STATS_MODIFIER
    &AuraEffect::HandleNoImmediateEffect,                         //383 SPELL_AURA_IGNORE_CD implemented in Spell::CheckCast
    &AuraEffect::HandleUnused,                                    //384 SPELL_AURA_384
    &AuraEffect::HandleNoImmediateEffect,                         //385 SPELL_AURA_STRIKE_SELF in Unit::AttackerStateUpdate
    &AuraEffect::HandleNoImmediateEffect,                         //386 SPELL_AURA_MOD_REST_GAINED
    &AuraEffect::HandleNoImmediateEffect,                         //387 SPELL_AURA_MOD_VOID_STORAGE_AND_TRANSMOGRIFY_COST
    &AuraEffect::HandleNoImmediateEffect,                         //388 SPELL_AURA_MOD_FLY_PATH_SPEED
    &AuraEffect::HandleNoImmediateEffect,                         //389 SPELL_AURA_MOD_CAST_TIME_WHILE_MOVING
    &AuraEffect::HandleUnused,                                    //390 SPELL_AURA_390
    &AuraEffect::HandleUnused,                                    //391 SPELL_AURA_391
    &AuraEffect::HandleUnused,                                    //392 SPELL_AURA_392
    &AuraEffect::HandleNoImmediateEffect,                         //393 SPELL_AURA_MOD_DEFLECT_SPELLS_FROM_FRONT
    &AuraEffect::HandleLootBonus,                                 //394 SPELL_AURA_LOOT_BONUS
    &AuraEffect::HandleCreateAreaTrigger,                         //395 SPELL_AURA_CREATE_AREATRIGGER
    &AuraEffect::HandleNoImmediateEffect,                         //396 SPELL_AURA_PROC_ON_POWER_AMOUNT_2 in Unit::VisualForPower
    &AuraEffect::HandleBattlegroundFlag,                          //397 SPELL_AURA_BATTLEGROUND_FLAG
    &AuraEffect::HandleBattlegroundFlag,                          //398 SPELL_AURA_BATTLEGROUND_FLAG_2
    &AuraEffect::HandleUnused,                                    //399 SPELL_AURA_399
    &AuraEffect::HandleAuraModSkill,                              //400 SPELL_AURA_MOD_SKILL_2
    &AuraEffect::HandleNULL,                                      //401 SPELL_AURA_CART_AURA
    &AuraEffect::HandleAuraeEablePowerType,                       //402 SPELL_AURA_ENABLE_POWER_TYPE
    &AuraEffect::HandleAuraModSpellVisual,                        //403 SPELL_AURA_MOD_SPELL_VISUAL
    &AuraEffect::HandleOverrideAttackPowerBySpellPower,           //404 SPELL_AURA_OVERRIDE_AP_BY_SPELL_POWER_PCT
    &AuraEffect::HandleIncreaseModRatingPct,                      //405 SPELL_AURA_MOD_RATING_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //406 SPELL_AURA_OVERRIDE_CLIENT_CONTROLS
    &AuraEffect::HandleModFear,                                   //407 SPELL_AURA_MOD_FEAR_2
    &AuraEffect::HandleNULL,                                      //408 SPELL_AURA_PROC_SPELL_CHARGE removed by spell defined in EffectTriggerSpell
    &AuraEffect::HandleAuraGlide,                                 //409 SPELL_AURA_GLIDE
    &AuraEffect::HandleNULL,                                      //410 SPELL_AURA_MOD_EXTRA_PET_DAMAGE
    &AuraEffect::HandleAuraModCharges,                            //411 SPELL_AURA_MOD_CHARGES
    &AuraEffect::HandleModPowerRegen,                             //412 SPELL_AURA_HASTE_AFFECTS_BASE_MANA_REGEN
    &AuraEffect::HandleAuraModParryPercent,                       //413 SPELL_AURA_MOD_PARRY_PERCENT2
    &AuraEffect::HandleNULL,                                      //414 SPELL_AURA_MOD_DEFLECT_RANGED_ATTACKS
    &AuraEffect::HandleNULL,                                      //415 SPELL_AURA_415
    &AuraEffect::HandleNoImmediateEffect,                         //416 SPELL_AURA_MOD_SPELL_COOLDOWN_BY_MELEE_HASTE implemented in AuraEffect::CalculateSpellMod()
    &AuraEffect::HandleNoImmediateEffect,                         //417 SPELL_AURA_MOD_SPELL_GDC_BY_MELEE_HASTE
    &AuraEffect::HandleAuraModMaxPower,                           //418 SPELL_AURA_MOD_MAX_POWER
    &AuraEffect::HandleAuraModAddEnergyPercent,                   //419 SPELL_AURA_MOD_ADD_ENERGY_PERCENT
    &AuraEffect::HandleNoImmediateEffect,                         //420 SPELL_AURA_MOD_BATTLE_PET_XP_GAIN
    &AuraEffect::HandleNoImmediateEffect,                         //421 SPELL_AURA_MOD_ABSORB_AMOUNT
    &AuraEffect::HandleNULL,                                      //422 SPELL_AURA_MOD_ABSORBTION_PERCENT
    &AuraEffect::HandleNULL,                                      //423 SPELL_AURA_423
    &AuraEffect::HandleNULL,                                      //424 SPELL_AURA_424
    &AuraEffect::HandleUnused,                                    //425 SPELL_AURA_425
    &AuraEffect::HandleNULL,                                      //426 SPELL_AURA_426
    &AuraEffect::HandleNULL,                                      //427 SPELL_AURA_ITEMS_SCALING_AURA
    &AuraEffect::HandleSummonController,                          //428 SPELL_AURA_SUMMON_CONTROLLER
    &AuraEffect::HandleNULL,                                      //429 SPELL_AURA_PET_DAMAGE_DONE_PCT implemented in Unit::SpellBaseDamageBonus Unit::MeleeDamageBonusDone
    &AuraEffect::HandleAuraActivateScene,                         //430 SPELL_AURA_ACTIVATE_SCENE
    &AuraEffect::HandleNULL,                                      //431 SPELL_AURA_CONTESTED_PVP
    &AuraEffect::HandleNULL,                                      //432 SPELL_AURA_ENTER_BATTLEGROUND - seems used for items and player lvl scaling
    &AuraEffect::HandleUnused,                                    //433 SPELL_AURA_433
    &AuraEffect::HandleNULL,                                      //434 SPELL_AURA_434
    &AuraEffect::HandleUnused,                                    //435 SPELL_AURA_435
    &AuraEffect::HandleNULL,                                      //436 SPELL_AURA_MOD_DAMAGE_FROM_LIQUID ?
    &AuraEffect::HandleNULL,                                      //437 SPELL_AURA_437
    &AuraEffect::HandleNULL,                                      //438
    &AuraEffect::HandleUnused,                                    //439
    &AuraEffect::HandleModMultistrikeDamagePct,                   //440 SPELL_AURA_MULTISTRIKE_DAMAGE_PCT
    &AuraEffect::HandleModMultistrikePct,                         //441 SPELL_AURA_MOD_MULTISTRIKE_PCT
    &AuraEffect::HandleModReadinessPct,                           //442 SPELL_AURA_MOD_READINESS_PCT
    &AuraEffect::HandleModLifeStealPct,                           //443 SPELL_AURA_MOD_LIFE_STEAL_PCT
    &AuraEffect::HandleUnused,                                    //444
    &AuraEffect::HandleUnused,                                    //445
    &AuraEffect::HandleNULL,                                      //446
    &AuraEffect::HandleNULL,                                      //447 SPELL_AURA_MOD_EXP_GAIN_FROM_BEAST_KILLINGS
    &AuraEffect::HandleNULL,                                      //448 SPELL_AURA_MOD_CRITICAL_HEAl_GAIN
    &AuraEffect::HandleUnused,                                    //449
    &AuraEffect::HandleNULL,                                      //450 SPELL_AURA_UPGRADE_CHARACTER_SPELL_TIER
    &AuraEffect::HandleNULL,                                      //451 SPELL_AURA_OVERRIDE_PET_SPECS
    &AuraEffect::HandleUnused,                                    //452
    &AuraEffect::HandleAuraModChargeRecoveryMod,                  //453 SPELL_AURA_CHARGE_RECOVERY_MOD
    &AuraEffect::HandleAuraModChargeRecoveryMod,                  //454 SPELL_AURA_CHARGE_RECOVERY_MULTIPLIER
    &AuraEffect::HandleAuraModRoot,                               //455 SPELL_AURA_MOD_ROOTED
    &AuraEffect::HandleNULL,                                      //456 SPELL_AURA_CHARGE_RECOVERY_AFFECTED_BY_HASTE
    &AuraEffect::HandleNULL,                                      //457 SPELL_AURA_CHARGE_RECOVERY_AFFECTED_BY_HASTE_REGEN
    &AuraEffect::HandleNoImmediateEffect,                         //458 SPELL_AURA_IGNORE_DUAL_WIELD_HIT_PENALTY
    &AuraEffect::HandleDisableMovementForce,                      //459 SPELL_AURA_DISABLE_MOVEMENT_FORCE
    &AuraEffect::HandleNULL,                                      //460 SPELL_AURA_RESET_COOLDOWNS_AT_DUEL_START
    &AuraEffect::HandleUnused,                                    //461
    &AuraEffect::HandleNULL,                                      //462
    &AuraEffect::HandleConverCritRatingPctToParryRating,          //463 SPELL_AURA_CONVER_CRIT_RATING_PCT_TO_PARRY_RATING
    &AuraEffect::HandleAttackPowerByBonusArmorPct,                //464 SPELL_AURA_MOD_ATTACK_POWER_BY_BONUS_ARMOR_PCT
    &AuraEffect::HandleNULL,                                      //465 SPELL_AURA_MOD_BONUS_ARMOR
    &AuraEffect::HandleNULL,                                      //466 SPELL_AURA_MOD_BONUS_ARMOR_PCT
    &AuraEffect::HandleModStatBonusPercent,                       //467 SPELL_AURA_MOD_STAT_BONUS_PCT
    &AuraEffect::HandleNoImmediateEffect,                         //468 SPELL_AURA_PROC_ON_HP_BELOW implemented in Unit::ModifyHealth
    &AuraEffect::HandleLootBonus,                                 //469 SPELL_AURA_LOOT_BONUS_NEW
    &AuraEffect::HandleNULL,                                      //470
    &AuraEffect::HandleModVersalityPct,                           //471 SPELL_AURA_MOD_VERSALITY_PCT
    &AuraEffect::HandleNULL,                                      //472 SPELL_AURA_FIXATE
    &AuraEffect::HandleNULL,                                      //473 SPELL_AURA_PREVENT_ITEMS_DURABILITY_LOSS
    &AuraEffect::HandleNULL,                                      //474 SPELL_AURA_MOD_DROP_ITEM_BONUSES_CHANCE
    &AuraEffect::HandleNULL,                                      //475
    &AuraEffect::HandleNoImmediateEffect,                         //476 SPELL_AURA_MOD_CURRENCY_LOOT_FROM_CREATURES_BY_PCT
    &AuraEffect::HandleNULL,                                      //477 SPELL_AURA_DRAENOR_SPELLS_AND_ITEMS_SCALING
    &AuraEffect::HandleNULL,                                      //478
    &AuraEffect::HandleUnused,                                    //479
    &AuraEffect::HandleUnused,                                    //480
    &AuraEffect::HandleNULL,                                      //481 SPELL_AURA_ACTIVETED_DEATH_RUNE
    &AuraEffect::HandleNULL,                                      //482 SPELL_AURA_CAMERA_SELFIE
    &AuraEffect::HandleNULL,                                      //483 SPELL_AURA_TRANSFORM_SUPRESSION_IN_PVP
    &AuraEffect::HandleNULL,                                      //484 SPELL_AURA_484
    &AuraEffect::HandleNULL,                                      //485 SPELL_AURA_485
    &AuraEffect::HandleNULL,                                      //486 SPELL_AURA_486
    &AuraEffect::HandleNULL,                                      //487 SPELL_AURA_487
    &AuraEffect::HandleNULL,                                      //488 SPELL_AURA_488
    &AuraEffect::HandleNULL,                                      //489 SPELL_AURA_489
    &AuraEffect::HandleNULL,                                      //490 SPELL_AURA_490
    &AuraEffect::HandleNULL,                                      //491 SPELL_AURA_491
    &AuraEffect::HandleNULL,                                      //492 SPELL_AURA_492
};

AuraEffect::AuraEffect(Aura* base, uint8 effIndex, int32 *baseAmount, Unit* caster):
m_base(base), m_spellInfo(base->GetSpellInfo()),
m_baseAmount(baseAmount ? *baseAmount : m_spellInfo->GetEffect(effIndex, m_diffMode)->BasePoints),
m_spellmod(NULL), m_periodicTimer(0), m_tickNumber(0), m_effIndex(effIndex),
m_canBeRecalculated(true), m_isPeriodic(false),
m_oldbaseAmount(0), saveTarget(NULL), m_crit_amount(0), m_crit_chance(0.0f),
m_diffMode(caster ? caster->GetSpawnMode() : 0)
{
    m_send_baseAmount = m_baseAmount;
}

AuraEffect::~AuraEffect()
{
    delete m_spellmod;
}

void AuraEffect::GetTargetList(std::list<Unit*> & targetList) const
{
    Aura::ApplicationMap const & targetMap = GetBase()->GetApplicationMap();
    // remove all targets which were not added to new list - they no longer deserve area aura
    for (Aura::ApplicationMap::const_iterator appIter = targetMap.begin(); appIter != targetMap.end(); ++appIter)
    {
        if (appIter->second->HasEffect(GetEffIndex()))
            targetList.push_back(appIter->second->GetTarget());
    }
}

void AuraEffect::GetApplicationList(std::list<AuraApplication*> & applicationList) const
{
    Aura::ApplicationMap const & targetMap = GetBase()->GetApplicationMap();
    for (Aura::ApplicationMap::const_iterator appIter = targetMap.begin(); appIter != targetMap.end(); ++appIter)
    {
        if (appIter->second->HasEffect(GetEffIndex()))
            applicationList.push_back(appIter->second);
    }
}

int32 AuraEffect::CalculateAmount(Unit* caster, int32 &m_aura_amount)
{
    int32 amount;
    Item* castItem = NULL;

    if(caster)
        if (ObjectGuid itemGUID = GetBase()->GetCastItemGUID())
            if (Player* playerCaster = caster->ToPlayer())
                castItem = playerCaster->GetItemByGuid(itemGUID);

    Unit* target = GetBase()->GetUnitOwner();

    // default amount calculation
    m_send_baseAmount = m_spellInfo->GetEffect(m_effIndex, m_diffMode)->CalcValue(caster, &m_baseAmount, GetBase()->GetOwner()->ToUnit(), castItem);
    amount = m_send_baseAmount;

    // check player on BG or arena
    if(caster)
    {
        if (Player* playerCaster = caster->ToPlayer())
        {
            if (playerCaster->InBattleground() || playerCaster->InArena())
            {
                if (GetBase()->InArenaNerf())
                    amount = CalculatePct(amount, 50);
            }
        }
    }

    // check item enchant aura cast
    if (!amount && caster && castItem)
    {
        if (castItem->GetItemSuffixFactor())
        {
            ItemRandomSuffixEntry const* item_rand_suffix = sItemRandomSuffixStore.LookupEntry(abs(castItem->GetItemRandomPropertyId()));
            if (item_rand_suffix)
            {
                for (uint8 k = 0; k < MAX_ITEM_ENCHANTS; k++)
                {
                    SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(item_rand_suffix->Enchantment[k]);
                    if (pEnchant)
                        for (int t = 0; t < MAX_ITEM_ENCHANTMENT_EFFECTS; t++)
                            if (pEnchant->EffectSpellID[t] == m_spellInfo->Id)
                            {
                                amount = uint32((item_rand_suffix->AllocationPct[k] * castItem->GetItemSuffixFactor()) / 10000);
                                break;
                            }

                    if (amount)
                        break;
                }
            }
        }
    }

    float DoneActualBenefit = 0.0f;
    bool CalcStack = bool(m_spellInfo->AuraOptions.CumulativeAura) && !(m_spellInfo->AuraOptions.ProcTypeMask & (PROC_FLAG_DONE_SPELL_MAGIC_DMG_POS_NEG)) && m_spellInfo->Id != 122355;

    if (caster && caster->GetTypeId() == TYPEID_PLAYER && (m_spellInfo->HasAttribute(SPELL_ATTR8_MASTERY_SPECIALIZATION)))
    {
        m_canBeRecalculated = false;
        amount += int32(ceil(caster->GetFloatValue(PLAYER_FIELD_MASTERY) * m_spellInfo->GetEffect(m_effIndex, m_diffMode)->BonusCoefficient));
    }

    GetBase()->CallScriptEffectBeforeCalcAmountHandlers(const_cast<AuraEffect const*>(this), amount, m_canBeRecalculated);

    // custom amount calculations go here
    switch (GetAuraType())
    {
        // crowd control auras
        //case SPELL_AURA_MOD_CONFUSE:
        case SPELL_AURA_MOD_FEAR:
        case SPELL_AURA_MOD_FEAR_2:
        //case SPELL_AURA_MOD_STUN:
        case SPELL_AURA_MOD_ROOT:
        case SPELL_AURA_MOD_ROOTED:
        case SPELL_AURA_TRANSFORM:
            m_canBeRecalculated = false;
            if (!m_spellInfo->AuraOptions.ProcTypeMask)
                break;
            amount = int32(target->CountPctFromMaxHealth(10));
            if (caster)
            {
                // Glyphs increasing damage cap
                Unit::AuraEffectList const& overrideClassScripts = caster->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                for (Unit::AuraEffectList::const_iterator itr = overrideClassScripts.begin(); itr != overrideClassScripts.end(); ++itr)
                {
                    if ((*itr)->IsAffectingSpell(m_spellInfo))
                    {
                        // Glyph of Frost nova and similar auras
                        if ((*itr)->GetMiscValue() == 7801)
                        {
                            AddPct(amount, (*itr)->GetAmount());
                            break;
                        }
                    }
                }
            }
            break;
        case SPELL_AURA_MOD_SPEED_ALWAYS:
        {
            switch (m_spellInfo->Id)
            {
                case 11327:  // Vanish
                case 1784:   // Stealth
                case 115191: // Stealth
                case 5215:   // Prowl
                {
                    if (Aura* aura = caster->GetAura(21009)) // Elusiveness
                        if (AuraEffect* eff = aura->GetEffect(EFFECT_0))
                            amount = eff->GetAmount();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_SCHOOL_ABSORB:
        {
            m_canBeRecalculated = false;

            if (!caster)
                break;

            switch (m_spellInfo->Id)
            {
                case 48707: // Anti-Magic Shell
                {
                    amount = caster->CountPctFromMaxHealth(m_spellInfo->Effects[EFFECT_1].BasePoints);
                    break;
                }
                case 108416: // Sacrificial Pact
                {
                    Unit* spelltarget = caster->GetGuardianPet() ? caster->GetGuardianPet(): caster;
                    int32 sacrifiedHealth = spelltarget->CountPctFromCurHealth(m_spellInfo->Effects[EFFECT_1].BasePoints);
                    spelltarget->ModifyHealth(-sacrifiedHealth);
                    amount = CalculatePct(sacrifiedHealth, m_spellInfo->Effects[EFFECT_0].BasePoints);
                    break;
                }
                default:
                    break;
            }

            amount = caster->CalcAbsorb(target, m_spellInfo, amount);

            break;
        }
//         case SPELL_AURA_MOD_ABSORB_AMOUNT:
//         {
//             if (!caster)
//                 break;
// 
//             switch (m_spellInfo->Id)
//             {
//                 default:
//                     break;
//             }
//             break;
//         }
        case SPELL_AURA_MOD_HEALING_PCT:
        {
            if (!caster)
                break;

            switch (m_spellInfo->Id)
            {
                case 114232: // Sanctified Wrath
                {
                    if (Player* paladin = caster->ToPlayer())
                    {
                        if (paladin->GetSpecializationId() != SPEC_PALADIN_PROTECTION)
                            amount = 0;
                    }
                    break;
                }
                case 73651: // Recuperate
                {
                    if (!caster->HasAura(146625))
                        amount = 0;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_ADD_FLAT_MODIFIER:
        {
            if (!caster)
                break;

            switch (m_spellInfo->Id)
            {
                case 1178:   // Bear Form Passive
                {
                    if (caster->HasAura(114237))
                        amount = 15000;
                    break;
                }
                case 114232: // Sanctified Wrath
                {
                    if (Player* paladin = caster->ToPlayer())
                    {
                        if (paladin->GetSpecializationId() != SPEC_PALADIN_HOLY)
                            amount = 0;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_ADD_PCT_MODIFIER:
        {
            if (!caster)
                break;

            switch (m_spellInfo->Id)
            {
                case 32216: // Impending Victory
                {
                    if (caster && caster->ToPlayer()->HasSpellCooldown(103840))
                        caster->ToPlayer()->RemoveSpellCooldown(103840, true);
                }
                case 137639: // Storm, Earth and Fire
                {
                    if (GetBase()->GetStackAmount() == 2)
                    {
                        SpellInfo const* _spellinf = sSpellMgr->GetSpellInfo(138228);
                        amount = -100 + _spellinf->Effects[EFFECT_1].BasePoints;
                        CalcStack = false;
                    }
                    break;
                }
                case 145958: // Readiness - Death Knight Blood
                case 145992: // Readiness - Warrior Protection
                case 145962: // Readiness - Druid Guardian
                case 145976: // Readiness - Paladin Protection
                case 145967: // Readiness - Monk Brewmaster
                {
                    if (caster->ToPlayer()->getLevel() < 91)
                    {
                        if (Aura * aura = caster->GetAura(146025))
                        {
                            if (AuraEffect* eff = aura->GetEffect(EFFECT_0))
                                amount = 100.0f / float((eff->GetAmount() + 100.0f) / 100.0f) - 100.0f;
                        }
                    }
                    else
                        amount = 0;
                    break;
                }
                case 145978: // Readiness - Paladin Retribution
                case 145990: // Readiness - Warrior Arms
                case 145991: // Readiness - Warrior Fury
                case 145959: // Readiness - Death Knight Frost
                case 145960: // Readiness - Death Knight Unholy
                {
                    if (caster->ToPlayer()->getLevel() < 91)
                    {
                        if (Aura * aura = caster->GetAura(145955))
                        {
                            if (AuraEffect* eff = aura->GetEffect(EFFECT_0))
                                amount = 100.0f / float((eff->GetAmount() + 100.0f) / 100.0f) - 100.0f;
                        }
                    }
                    else
                        amount = 0;
                    break;
                }
                case 145983: // Readiness - Rogue Assassination
                case 145984: // Readiness - Rogue Combat
                case 145985: // Readiness - Rogue Subtlety
                case 145964: // Readiness - Hunter Beast Mastery
                case 145965: // Readiness - Hunter Marksmanship
                case 145966: // Readiness - Hunter Survival
                case 145969: // Readiness - Monk Windwalker
                case 145961: // Readiness - Druid Feral
                case 145986: // Readiness - Shaman Enhancement
                {
                    if (caster->ToPlayer()->getLevel() < 91)
                    {
                        if (Aura * aura = caster->GetAura(146019))
                        {
                            if (AuraEffect* eff = aura->GetEffect(EFFECT_0))
                                amount = 100.0f / float((eff->GetAmount() + 100.0f) / 100.0f) - 100.0f;
                        }
                    }
                    else
                        amount = 0;
                    break;
                }
                case 114232: // Sanctified Wrath
                {
                    if (Player* paladin = caster->ToPlayer())
                    {
                        switch (paladin->GetSpecializationId())
                        {
                            case SPEC_PALADIN_HOLY:        if (m_effIndex != 0) amount = 0; break;
                            case SPEC_PALADIN_PROTECTION:  if (m_effIndex != 1) amount = 0; break;
                            case SPEC_PALADIN_RETRIBUTION: if (m_effIndex != 2) amount = 0; break;
                            default: amount = 0; break;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_DUMMY:
        {
            if (!caster)
                break;

            switch (GetId())
            {
                case 77219: // Mastery: Master Demonologist
                {
                    if (m_effIndex != EFFECT_0)
                        break;

                    if (Aura* aura = caster->GetAura(114168))
                    {
                        aura->GetEffect(EFFECT_1)->SetCanBeRecalculated(true);
                        aura->GetEffect(EFFECT_1)->RecalculateAmount();
                        aura->SetNeedClientUpdateForTargets();;
                    }
                    break;
                }
                case 76657: // Mastery: Master of Beasts
                {
                    if (Guardian* pet = caster->ToPlayer()->GetPet())
                    {
                        if (Aura* aur = pet->GetAura(8875))
                        {
                            aur->GetEffect(EFFECT_0)->SetCanBeRecalculated(true);
                            aur->GetEffect(EFFECT_1)->SetCanBeRecalculated(true);
                            aur->RecalculateAmountOfEffects();
                        }
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_PERIODIC_DUMMY:
        {
            switch (m_spellInfo->Id)
            {
                case 146198: // Essence of Yu'lon
                {
                    SpellInfo const* _info = sSpellMgr->GetSpellInfo(148008);
                    amount = caster->GetSpellPowerDamage(_info->GetSchoolMask()) * _info->Effects[EFFECT_0].BonusCoefficient / GetTotalTicks(); 
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_MOD_DECREASE_SPEED:
        {
            if (!target)
                break;

            switch (GetId())
            {
                case 119450:
                {
                    if (target->isPet())
                    amount = 0;
                }
            }
        }
        case SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN:
        {
            if (!caster)
                break;

            switch (m_spellInfo->Id)
            {
                case 114168: // Dark Apotheosis
                {
                    if (m_effIndex != EFFECT_1)
                        break;

                    if (Player* plr = caster->ToPlayer())
                    {
                        float mastery = plr->GetFloatValue(PLAYER_FIELD_MASTERY);
                        amount = -(RoundingFloatValue(float(m_spellInfo->Effects[EFFECT_5].BasePoints) + (mastery / (mastery + float(m_spellInfo->Effects[EFFECT_0].BasePoints))) * 100.0f));
                        
                    }
                    break;
                }
                case SPELL_BG_HORDE_FLAG:
                case SPELL_BG_ALLIANCE_FLAG:
                {
                    if (Player* plr = caster->ToPlayer())
                    {
                        if (plr->isInTankSpec())
                            amount = (m_spellInfo->Effects[EFFECT_1].BasePoints + 30);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_MOD_SPELL_GDC_BY_MELEE_HASTE:
        case SPELL_AURA_MOD_SPELL_COOLDOWN_BY_MELEE_HASTE:
        {
            switch (GetId())
            {
                case 25956: // Sanctity of Battle
                {
                    if (Player* paladin = caster->ToPlayer())
                    {
                        amount  = -100;
                        amount += paladin->GetFloatValue(UNIT_FIELD_MOD_HASTE) * 100.0f;

                        if (amount > 0) amount = 0;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_PERIODIC_DAMAGE:
        {
            if (!caster || !target)
                break;

            switch (GetId())
            {
                case 109076: // Incendiary Fireworks Launcher
                {
                    amount = irand(6000, 9000);
                    break;
                }
                case 50536: // Unholy Blight damage over time effect
                {
                    m_canBeRecalculated = false;
                    // we're getting total damage on aura apply, change it to be damage per tick
                    amount = int32((float)amount / GetTotalTicks());
                    break;
                }
                default:
                    break;
            }

            amount = caster->SpellDamageBonusDone(target, m_spellInfo, amount, DOT, (SpellEffIndex) GetEffIndex());

            switch (m_spellInfo->Id)
            {
                case 1943: // Rupture
                {
                    if (caster->GetTypeId() != TYPEID_PLAYER)
                        break;

                    amount *= caster->ToPlayer()->GetComboPoints(1943);
                    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::CalculateAmount amount %i m_send_baseAmount %i", amount, m_send_baseAmount, caster->ToPlayer()->GetComboPoints(1943));
                    break;
                }
                case 1079: // Rip
                {
                    if (caster->GetTypeId() != TYPEID_PLAYER)
                        break;

                    amount *= caster->ToPlayer()->GetComboPoints(1079);
                    break;
                }
                case 53301: // Explosive Shot
                {
                    amount = GetBase()->m_damage_amount;
                    break;
                }
                case 32409:  // Shadow Word: Death (DoT)
                {
                    if (Aura* aura = caster->GetAura(105843))
                        if (AuraEffect* eff = aura->GetEffect(EFFECT_1))
                            AddPct(amount, -(eff->GetAmount()));
                    break;
                }
                case 114923: // Nether Tempest
                case 44457:  // Living Bomb
                {
                    if (target->GetTypeId() == TYPEID_PLAYER)
                        amount = CalculatePct(amount, 70);
                    break;
                }
                case 129197: // Insanity
                {
                    if (Aura* aur = target->GetAura(2944, caster->GetGUID()))
                    {
                        int32 addBonusPct = aur->GetEffect(EFFECT_2)->GetAmount();
                        AddPct(amount, addBonusPct);
                    }
                    break;
                }
                default:
                    break;
            }

            caster->isSpellCrit(target, m_spellInfo, m_spellInfo->GetSchoolMask(), BASE_ATTACK, m_crit_chance);
            break;
        }
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
        {
            if(target && caster)
            {
                amount = uint32(target->CountPctFromMaxHealth(amount));
                caster->isSpellCrit(target, m_spellInfo, m_spellInfo->GetSchoolMask(), BASE_ATTACK, m_crit_chance);
            }
            break;
        }
        case SPELL_AURA_PERIODIC_LEECH:
        {
            if(target && caster)
            {
                amount = caster->SpellDamageBonusDone(target, m_spellInfo, amount, DOT, (SpellEffIndex) GetEffIndex());
                caster->isSpellCrit(target, m_spellInfo, m_spellInfo->GetSchoolMask(), BASE_ATTACK, m_crit_chance);
            }
            break;
        }
        case SPELL_AURA_PERIODIC_HEAL:
        {
            if (!caster || !target)
                break;

            switch (GetId())
            {
                case 73651: // Recuperate
                {
                    int32 bp = m_spellInfo->Effects[0].BasePoints;
                    
                    if (caster->HasAura(56806))
                        bp += 1;
                    
                    if (caster->HasAura(61249))
                        bp += 1;
                    
                    amount = CalculatePct(caster->GetMaxHealth(), bp);
                    break;
                }
                case 28880:  // Warrior     - Gift of the Naaru
                case 59542:  // Paladin     - Gift of the Naaru
                case 59543:  // Hunter      - Gift of the Naaru
                case 59544:  // Priest      - Gift of the Naaru
                case 59545:  // Deathknight - Gift of the Naaru
                case 59547:  // Shaman      - Gift of the Naaru
                case 59548:  // Mage        - Gift of the Naaru
                case 121093: // Monk        - Gift of the Naaru
                {
                    if(GetTotalTicks())
                        amount = CalculatePct(caster->GetMaxHealth(), m_spellInfo->Effects[1].BasePoints) / GetTotalTicks();
                    break;
                }
                default:
                    break;
            }
            amount = caster->SpellHealingBonusDone(target, m_spellInfo, amount, DOT, (SpellEffIndex) GetEffIndex());

            switch (m_spellInfo->Id)
            {
                // Spirit Mend
                case 90361:
                    amount += int32((caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.6f) * 1.0504f);
                    break;
                case 156322: // Eternal Flame
                {
                    if (target == caster)
                    {
                        int32 pct = 50;
                        if (Aura* Bastion_of_Glory = caster->GetAura(114637))
                        {
                            if (AuraEffect* eff = Bastion_of_Glory->GetEffect(EFFECT_0))
                                pct += eff->GetAmount();
                        }
                        AddPct(amount, pct);
                    }
                    break;
                }
                default:
                    break;
            }

            caster->isSpellCrit(target, m_spellInfo, m_spellInfo->GetSchoolMask(), BASE_ATTACK, m_crit_chance);
            break;
        }
        case SPELL_AURA_MOD_THREAT:
        {
            uint8 level_diff = 0;
            float multiplier = 0.0f;
            switch (GetId())
            {
                // Arcane Shroud
                case 26400:
                    level_diff = target->getLevel() - 60;
                    multiplier = 2;
                    break;
                // The Eye of Diminution
                case 28862:
                    level_diff = target->getLevel() - 60;
                    multiplier = 1;
                    break;
            }
            if (level_diff > 0)
                amount += int32(multiplier * level_diff);
            break;
        }
        case SPELL_AURA_HASTE_SPELLS:
        {
            switch (m_spellInfo->Id)
            {                
                case 5760:   // Mind-numbing Poison  
                case 31589:  // Slow
                case 50274:
                case 73975:  // Necrotic Strike             
                case 58604:  // Lava Breath
                case 90315:  // Tailspin
                case 109466: // Curse of Enfeeblement
                case 109468: // Curse of Enfeeblement (Soulburn)
                case 116198: // Enfeeblement Aura (Metamorphosis)
                case 126406: // Trample
                {
                    if (target && target->GetTypeId() == TYPEID_PLAYER)
                        amount /= 5;
                    break;
                }
                case 25810:
                {
                    if (target && target->GetTypeId() == TYPEID_PLAYER)
                        amount /= 3;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_MOD_INCREASE_HEALTH:
            // Vampiric Blood
            if (GetId() == 55233)
                amount = target->CountPctFromMaxHealth(amount);
            break;
        case SPELL_AURA_MOD_STAT:
        {
            switch (m_spellInfo->Id)
            {
                case 142530: // Bloody Dancing Steel
                case 120032: // Dancing Steel
                {
                    int32 str = caster->GetStat(STAT_STRENGTH);
                    int32 agi = caster->GetStat(STAT_AGILITY);
                    
                    switch (m_effIndex)
                    {
                        case EFFECT_0: if (str > agi) amount = 0; break;
                        case EFFECT_1: if (str < agi) amount = 0; break;
                    }
                    break;
                }
                case 108300: // Killer Instinct
                {
                    if (caster)
                    {
                        amount = caster->GetStat(STAT_INTELLECT);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_MOD_INCREASE_HEALTH_2:
        case SPELL_AURA_MOD_INCREASE_HEALTH_3:
        {
            switch (m_spellInfo->Id)
            {
                case 132413: // Shadow Bulwark
                case 12975:  // Last Stand
                case 106922: // Might of Ursoc
                {
                    amount = CalculatePct(caster->GetMaxHealth(), amount);
                    break;
                }
                case 137211: // Tremendous Fortitude
                {
                    amount = CalculatePct(caster->GetMaxHealth(), amount);
                    if (amount > 18000)
                        amount = 18000;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_MOD_INCREASE_SPEED:
            // Dash - do not set speed if not in cat form
            if (GetSpellInfo()->ClassOptions.SpellClassSet == SPELLFAMILY_DRUID && GetSpellInfo()->ClassOptions.SpellClassMask[2] & 0x00000008)
                amount = target->GetShapeshiftForm() == FORM_CAT ? amount : 0;
            break;
        case SPELL_AURA_MOD_DAMAGE_PERCENT_DONE:
        case SPELL_AURA_MOD_HEALING_DONE_PERCENT:
        {
            if (!caster)
                break;

            switch (m_spellInfo->Id)
            {
                case 112947: // Nerve Strike
                {
                    if (m_effIndex == EFFECT_0)
                        if (target->GetTypeId() == TYPEID_PLAYER)
                            amount = CalculatePct(amount, 50);
                    break;
                }
                case 138130: // Storm, Earth and Fire (clone)
                {
                    if (m_effIndex != EFFECT_1)
                        break;

                    if (Unit* owner = caster->GetOwner())
                        if (Aura* ownerAura = owner->GetAura(137639))
                        {
                            SpellInfo const* _spellinf = sSpellMgr->GetSpellInfo(138228);
                            switch (ownerAura->GetStackAmount())
                            {
                                case 1: amount = -100 + _spellinf->Effects[EFFECT_0].BasePoints; break;
                                case 2: amount = -100 + _spellinf->Effects[EFFECT_1].BasePoints; break;
                            }
                        }
                    break;
                }
                case 137639: // Storm, Earth and Fire
                {
                    if (GetBase()->GetStackAmount() == 2)
                    {
                        SpellInfo const* _spellinf = sSpellMgr->GetSpellInfo(138228);
                        amount = -100 + _spellinf->Effects[EFFECT_1].BasePoints;
                        CalcStack = false;
                    }
                    break;
                }
                case 116740: // Tigereye Brew
                {
                    if (Aura * aura = caster->GetAura(125195))
                    {
                        uint8 stuck   = aura->GetStackAmount();
                        uint8 residue = stuck;

                        if (stuck >= 10)
                        {
                            stuck = 10;
                            if(caster->HasAura(145022)) //Item - Monk T16 Windwalker 4P Bonus
                                caster->CastSpell(caster, 145024, true);
                        }

                        residue -= stuck;

                        if (m_effIndex == EFFECT_1)
                        {
                            if (residue != 0)
                            {
                                aura->SetStackAmount(residue);

                                if (residue < 10)
                                    caster->RemoveAura(137591);
                            }
                            else caster->RemoveAura(125195);
                        }
                        amount *= stuck;

                        if (caster->HasAura(138315)) //Item - Monk T15 Windwalker 4P Bonus
                            amount += int32(0.5f * stuck);
                    }

                    if (m_effIndex == EFFECT_1)
                        for (uint8 i = 13; i < 16; ++i)
                            if (caster->m_SummonSlot[i])
                                if (Creature* crt = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                                    if (!crt->IsDespawn())
                                        crt->CastCustomSpell(crt, m_spellInfo->Id, &amount, &amount, NULL, true);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELL_AURA_STRANGULATE:
        {
            // Hack. OO Galakras: Anti-Air Cannon
            if (Aura* aura = target->GetAura(147514))
                if (aura->GetStackAmount() < 2)
                    aura->SetStackAmount(aura->GetStackAmount() + 1);
            break;
        }
        case SPELL_AURA_LOOT_BONUS:
        {
            if (target && target->GetMap())
                amount = target->GetMap()->GetDifficultyID();
            break;
        }
        case SPELL_AURA_MOUNTED:
        {
            uint32 mountType = uint32(GetMiscValueB());
            if (MountEntry const* mountEntry = sDB2Manager.GetMount(GetId()))
                mountType = mountEntry->MountTypeID;

            if (MountCapabilityEntry const* mountCapability = GetBase()->GetUnitOwner()->GetMountCapability(mountType))
            {
                amount = mountCapability->ID;
                m_canBeRecalculated = false;
            }
            break;
        }
        default:
            break;
    }

    // Mixology - Effect value mod
    if (caster && caster->GetTypeId() == TYPEID_PLAYER)
    {
        if (/*GetSpellInfo()->SpellFamilyName == SPELLFAMILY_POTION && */(
            sSpellMgr->IsSpellMemberOfSpellGroup(GetId(), SPELL_GROUP_ELIXIR_BATTLE) ||
            sSpellMgr->IsSpellMemberOfSpellGroup(GetId(), SPELL_GROUP_ELIXIR_GUARDIAN)))
        {
            if (caster->HasAura(53042) && caster->HasSpell(GetSpellInfo()->Effects[0].TriggerSpell))
            {
                switch (GetId())
                {
                    case 53749: // Guru's Elixir
                        amount += 8;
                        break;
                    case 11328: // Elixir of Agility
                        amount += 10;
                        break;
                    case 28497: // Elixir of Mighty Agility
                    case 53747: // Elixir of Spirit
                    case 54212: // Flask of Pure Mojo
                    case 60340: // Elixir of Accuracy
                    case 60341: // Elixir of Deadly Strikes
                    case 60343: // Elixir of Mighty Defense
                    case 60344: // Elixir of Expertise
                    case 60345: // Elixir of Armor Piercing
                    case 60346: // Elixir of Lightning Speed
                    case 60347: // Elixir of Mighty Thoughts
                        amount += 20;
                        break;
                    case 53752: // Lesser Flask of Toughness
                    case 62380: // Lesser Flask of Resistance
                        amount += 40;
                        break;
                    case 53755: // Flask of the Frost Wyrm
                        amount += 47;
                        break;
                    case 53760: // Flask of Endless Rage
                        amount += 64;
                        break;
                    case 53751: // Elixir of Mighty Fortitude
                        amount += 200;
                        break;
                    case 53763: // Elixir of Protection
                        amount += 280;
                        break;
                    case 53758: // Flask of Stoneblood
                        amount += 320;
                        break;
                    // Cataclysm
                    case 79469: // Flask of Steelskin
                    case 79470: // Flask of the Draconic Mind
                    case 79471: // Flask of the Winds
                    case 79472: // Flask of Titanic Strength
                    case 94160: // Flask of Flowing Water
                        amount += 80;
                        break;
                    case 79635: // Flask of the Master
                    case 79632: // Elixir of Mighty Speed
                    case 79481: // Elixir of Impossible Accuracy
                    case 79477: // Elixir of the Cobra
                    case 79474: // Elixir of the Naga
                    case 79468: // Ghost Elixir
                        amount += 40;
                        break;
                    case 79480: // Elixir of Deep Earth
                        amount += 345;
                        break;
                    case 79631: // Prismatic Elixir
                        amount += 45;
                        break;
                    // Pandaria
                    case 105689: // Flask of Spring Blossoms
                    case 105691: // Flask of the Warm Sun
                    case 105696: // Flask of Winter's Bite
                        amount += 320;
                        break;
                    case 105694: // Flask of the Earth
                    case 105693: // Flask of Falling Leaves
                        amount += 480;
                        break;
                }
            }
        }
    }
    
    if (DoneActualBenefit != 0.0f)
    {
        DoneActualBenefit *= caster->CalculateLevelPenalty(GetSpellInfo());
        amount += (int32)DoneActualBenefit;
    }

    GetBase()->CallScriptEffectCalcAmountHandlers(const_cast<AuraEffect const*>(this), amount, m_canBeRecalculated);

    CalculateFromDummyAmount(caster, target, amount);

    if (CalcStack)
        amount *= GetBase()->GetStackAmount();

    //Disable same auras on arena or BG(RBG)
    if(caster)
    {
        if (Player* plr = caster->ToPlayer())
        {
            if ((plr->InBattleground() || plr->InArena()) && m_spellInfo->HasAttribute(SPELL_ATTR8_NOT_IN_BG_OR_ARENA))
                amount = 0;
            if ((plr->InRBG() || plr->InArena()) && m_spellInfo->HasAttribute(SPELL_ATTR4_NOT_USABLE_IN_ARENA_OR_RATED_BG))
                amount = 0;
        }
    }

    switch (GetAuraType())
    {
        // Set crit amount for aura
        case SPELL_AURA_PERIODIC_HEAL:
        {
            if(caster)
                m_crit_amount = caster->SpellCriticalHealingBonus(m_spellInfo, amount, target);
            else
                m_crit_amount = amount * 2;
            break;
        }
        case SPELL_AURA_PERIODIC_LEECH:
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
        {
            if(caster)
                m_crit_amount = caster->SpellCriticalDamageBonus(m_spellInfo, amount, target);
            else
                m_crit_amount = amount * 2;
            break;
        }
        default:
            m_crit_amount = amount * 2;
            break;
    }

    switch (GetAuraType())
    {
        // Set amount for aura
        case SPELL_AURA_OBS_MOD_HEALTH:
        case SPELL_AURA_PERIODIC_HEAL:
        case SPELL_AURA_PERIODIC_LEECH:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
        case SPELL_AURA_PERIODIC_DAMAGE:
        {
            if(m_spellInfo->HasAttribute(SPELL_ATTR10_STACK_DAMAGE_OR_HEAL) && GetBase()->GetMaxDuration() != -1 && caster && GetTotalTicks())
            {
                uint32 oldBp = target->GetRemainingPeriodicAmount(caster->GetGUID(), m_spellInfo->Id, GetAuraType());
                uint32 getTicks = GetTotalTicks();

                amount *= getTicks;

                if (oldBp)
                    getTicks++;

                if (amount)
                    amount /= getTicks;

                amount += oldBp;

                m_crit_amount = amount * 2;
            }

            m_aura_amount = amount;
            break;
        }
        default:
            break;
    }

    return amount;
}

void AuraEffect::CalculateFromDummyAmount(Unit* caster, Unit* target, int32 &amount)
{
    if(!caster)
        return;

    if (std::vector<SpellAuraDummy> const* spellAuraDummy = sSpellMgr->GetSpellAuraDummy(GetId()))
    {
        for (std::vector<SpellAuraDummy>::const_iterator itr = spellAuraDummy->begin(); itr != spellAuraDummy->end(); ++itr)
        {
            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::CalculateFromDummyAmount start GetId %i, amount %i, mask %i type %u option %u caster %u GetCaster %u",
            GetId(), amount, itr->effectmask, itr->type, itr->option, caster->GetGUIDLow(), GetCaster()->GetGUIDLow());

            if(itr->type != SPELL_DUMMY_DEFAULT)
                continue;

            if (!(itr->effectmask & (1<<m_effIndex)))
                continue;

            Unit* _caster = caster;
            Unit* _targetAura = caster;
            bool check = false;

            if(itr->caster == 1 && target) //get caster as target
                _caster = target;
            if(itr->caster == 2 && _caster->ToPlayer()) //get target pet
            {
                if (Pet* pet = _caster->ToPlayer()->GetPet())
                    _caster = (Unit*)pet;
            }
            if(itr->caster == 3) //get target owner
            {
                if (Unit* owner = _caster->GetOwner())
                    _caster = owner;
            }

            if(itr->targetaura == 1) //get caster aura
                _targetAura = GetCaster();
            if(itr->targetaura == 2 && target) //get target aura
                _targetAura = target;

            if(!_targetAura)
                _targetAura = caster;

            switch (itr->option)
            {
                case SPELL_DUMMY_ENABLE: //0
                {
                    if(itr->aura > 0 && !_targetAura->HasAura(itr->aura))
                        continue;
                    if(itr->aura < 0 && _targetAura->HasAura(abs(itr->aura)))
                        continue;

                    if(itr->spellDummyId > 0 && !_caster->HasAura(itr->spellDummyId, caster->GetGUID()))
                    {
                        amount = 0;
                        check = true;
                    }
                    if(itr->spellDummyId < 0 && _caster->HasAura(abs(itr->spellDummyId), caster->GetGUID()))
                    {
                        amount = 0;
                        check = true;
                    }
                    break;
                }
                case SPELL_DUMMY_ADD_PERC: //1
                {
                    if(itr->aura > 0 && !_targetAura->HasAura(itr->aura))
                        continue;
                    if(itr->aura < 0 && _targetAura->HasAura(abs(itr->aura)))
                        continue;

                    if(itr->spellDummyId > 0 && _caster->HasAura(itr->spellDummyId, caster->GetGUID()))
                    {
                        if(SpellInfo const* dummyInfo = sSpellMgr->GetSpellInfo(itr->spellDummyId))
                        {
                            float bp = itr->custombp;
                            if(!bp)
                                bp = dummyInfo->Effects[itr->effectDummy].BasePoints;
                            amount += CalculatePct(amount, bp);
                            check = true;
                        }
                    }
                    if(itr->spellDummyId < 0 && _caster->HasAura(abs(itr->spellDummyId), caster->GetGUID()))
                    {
                        if(SpellInfo const* dummyInfo = sSpellMgr->GetSpellInfo(abs(itr->spellDummyId)))
                        {
                            float bp = itr->custombp;
                            if(!bp)
                                bp = dummyInfo->Effects[itr->effectDummy].BasePoints;
                            amount -= CalculatePct(amount, bp);
                            check = true;
                        }
                    }
                    break;
                }
                case SPELL_DUMMY_ADD_VALUE: //2
                {
                    if(itr->aura > 0 && !_targetAura->HasAura(itr->aura))
                        continue;
                    if(itr->aura < 0 && _targetAura->HasAura(abs(itr->aura)))
                        continue;

                    if(itr->spellDummyId > 0 && _caster->HasAura(itr->spellDummyId, caster->GetGUID()))
                    {
                        if(SpellInfo const* dummyInfo = sSpellMgr->GetSpellInfo(itr->spellDummyId))
                        {
                            float bp = itr->custombp;
                            if(!bp)
                                bp = dummyInfo->Effects[itr->effectDummy].BasePoints;
                            amount += bp;
                            check = true;
                        }
                    }
                    if(itr->spellDummyId < 0 && _caster->HasAura(abs(itr->spellDummyId), caster->GetGUID()))
                    {
                        if(SpellInfo const* dummyInfo = sSpellMgr->GetSpellInfo(abs(itr->spellDummyId)))
                        {
                            float bp = itr->custombp;
                            if(!bp)
                                bp = dummyInfo->Effects[itr->effectDummy].BasePoints;
                            amount -= bp;
                            check = true;
                        }
                    }
                    break;
                }
                case SPELL_DUMMY_ADD_PERC_BP: //8
                {
                    if(itr->aura > 0 && !_targetAura->HasAura(itr->aura))
                        continue;
                    if(itr->aura < 0 && _targetAura->HasAura(abs(itr->aura)))
                        continue;

                    if(itr->spellDummyId > 0 && _caster->HasAura(itr->spellDummyId, caster->GetGUID()))
                    {
                        if(SpellInfo const* dummyInfo = sSpellMgr->GetSpellInfo(itr->spellDummyId))
                        {
                            float bp = itr->custombp;
                            if(!bp)
                                bp = dummyInfo->Effects[itr->effectDummy].BasePoints;

                            bp /= 100.0f;
                            amount += CalculatePct(amount, bp);
                            check = true;
                        }
                    }
                    if(itr->spellDummyId < 0 && _caster->HasAura(abs(itr->spellDummyId), caster->GetGUID()))
                    {
                        if(SpellInfo const* dummyInfo = sSpellMgr->GetSpellInfo(abs(itr->spellDummyId)))
                        {
                            float bp = itr->custombp;
                            if(!bp)
                                bp = dummyInfo->Effects[itr->effectDummy].BasePoints;

                            bp /= 100.0f;
                            amount -= CalculatePct(amount, bp);
                            check = true;
                        }
                    }
                    break;
                }
            }
            if(check && itr->removeAura)
                _caster->RemoveAurasDueToSpell(itr->removeAura);
        }
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::CalculateFromDummyAmount end GetId %i, amount %i m_effIndex %u", GetId(), amount, m_effIndex);
    }
}

void AuraEffect::CalculatePeriodic(Unit* caster, bool resetPeriodicTimer /*= true*/, bool load /*= false*/)
{
    m_period = m_spellInfo->GetEffect(m_effIndex, m_diffMode)->ApplyAuraPeriod;
                        
    // prepare periodics
    switch (GetAuraType())
    {
        case SPELL_AURA_OBS_MOD_POWER:
            // 3 spells have no amplitude set
            if (!m_period)
                m_period = 1 * IN_MILLISECONDS;
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_HEAL:
        case SPELL_AURA_OBS_MOD_HEALTH:
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
        case SPELL_AURA_PERIODIC_ENERGIZE:
        case SPELL_AURA_PERIODIC_LEECH:
        case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
        case SPELL_AURA_PERIODIC_MANA_LEECH:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
        case SPELL_AURA_POWER_BURN:
        case SPELL_AURA_PERIODIC_DUMMY:
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
            if(GetBase()->GetMaxDuration())
                m_isPeriodic = true;
            else
                m_isPeriodic = false;
            break;
        default:
            break;
    }

    GetBase()->CallScriptEffectCalcPeriodicHandlers(const_cast<AuraEffect const*>(this), m_isPeriodic, m_period);

    if (!m_isPeriodic)
        return;

    Player* modOwner = caster ? caster->GetSpellModOwner() : NULL;

    // Apply casting time mods
    if (m_period)
    {
        // Apply periodic time mod
        if (modOwner)
            modOwner->ApplySpellMod(GetId(), SPELLMOD_ACTIVATION_TIME, m_period);

        if (caster)
        {
            // Haste modifies periodic time of channeled spells
            if (m_spellInfo->IsChanneled())
            {
                if (m_spellInfo->HasAttribute(SPELL_ATTR5_HASTE_AFFECT_TICK_AND_CASTTIME))
                    caster->ModSpellCastTime(m_spellInfo, m_period);
            }
            else
            {
                if (m_spellInfo->HasAttribute(SPELL_ATTR5_HASTE_AFFECT_TICK_AND_CASTTIME))
                    m_period = int32(m_period * caster->GetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE));

                if (m_spellInfo->HasAttribute(SPELL_ATTR8_HASTE_AFFECT_DURATION_RECOVERY))
                {
                    int32 _duration = GetBase()->GetMaxDuration() * caster->GetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE);
                    GetBase()->SetMaxDuration(_duration);
                    GetBase()->SetDuration(_duration);
                }
            }
            if (m_spellInfo->HasAttribute(SPELL_ATTR8_HASTE_AFFECT_DURATION))
                m_period = int32(m_period * caster->GetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE));
        }

        //! If duration nod defined should we change duration? this remove aura.
        if (!resetPeriodicTimer && !load && !(m_spellInfo->HasAttribute(SPELL_ATTR5_HASTE_AFFECT_TICK_AND_CASTTIME)) && !(m_spellInfo->HasAttribute(SPELL_ATTR8_HASTE_AFFECT_DURATION_RECOVERY)) && !(m_spellInfo->HasAttribute(SPELL_ATTR8_HASTE_AFFECT_DURATION)) && m_spellInfo->Duration.Duration != -1 /* && GetBase()->GetMaxDuration() >= 0*/)
        {
            int32 dotduration = GetBase()->GetMaxDuration() + m_periodicTimer;
            GetBase()->SetMaxDuration(dotduration);
            GetBase()->SetDuration(dotduration);
        }
    }

    if (load) // aura loaded from db
    {
        m_tickNumber = m_period ? GetBase()->GetDuration() / m_period : 0;
        m_periodicTimer = m_period ? GetBase()->GetDuration() % m_period : 0;
        if (m_spellInfo->HasAttribute(SPELL_ATTR5_START_PERIODIC_AT_APPLY))
            ++m_tickNumber;
    }
    else // aura just created or reapplied
    {
        m_tickNumber = 0;
        // reset periodic timer on aura create or on reapply when aura isn't dot
        // possibly we should not reset periodic timers only when aura is triggered by proc
        // or maybe there's a spell attribute somewhere
        if (resetPeriodicTimer)
        {
            m_periodicTimer = 0;
            // Start periodic on next tick or at aura apply
            if (m_period && !(m_spellInfo->HasAttribute(SPELL_ATTR5_START_PERIODIC_AT_APPLY)))
                m_periodicTimer += m_period;
        }
    }
}

void AuraEffect::CalculateSpellMod()
{
    switch (GetAuraType())
    {
        case SPELL_AURA_ADD_FLAT_MODIFIER:
        case SPELL_AURA_ADD_PCT_MODIFIER:
        {
            if (!m_spellmod)
            {
                m_spellmod = new SpellModifier(GetBase());
                m_spellmod->op = SpellModOp(GetMiscValue());
                ASSERT(m_spellmod->op < MAX_SPELLMOD);

                m_spellmod->type = SpellModType(GetAuraType());    // SpellModType value == spell aura types
                m_spellmod->spellId = GetId();
                m_spellmod->mask = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->SpellClassMask;
                m_spellmod->charges = GetBase()->GetCharges();
            }
            m_spellmod->value = GetAmount();
            break;
        }
        case SPELL_AURA_MOD_SPELL_COOLDOWN_BY_MELEE_HASTE:
        case SPELL_AURA_MOD_SPELL_GDC_BY_MELEE_HASTE:
        {
            /*if (!m_spellmod)
            {
                m_spellmod = new SpellModifier(GetBase());
                m_spellmod->op = SpellModOp(GetMiscValue());
                ASSERT(m_spellmod->op < MAX_SPELLMOD);

                m_spellmod->type = SPELLMOD_PCT;    // SpellModType value == spell aura types
                m_spellmod->spellId = GetId();
                m_spellmod->mask = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->SpellClassMask;
                m_spellmod->charges = GetBase()->GetCharges();
            }
            m_spellmod->value = GetAmount();
            break;*/
        }
        default:
            break;
    }
    GetBase()->CallScriptEffectCalcSpellModHandlers(const_cast<AuraEffect const*>(this), m_spellmod);
}

void AuraEffect::ChangeAmount(int32 newAmount, bool mark, bool onStackOrReapply)
{
    // Reapply if amount change
    uint8 handleMask = 0;
    if (newAmount != GetAmount())
        handleMask |= AURA_EFFECT_HANDLE_CHANGE_AMOUNT;
    if (onStackOrReapply)
        handleMask |= AURA_EFFECT_HANDLE_REAPPLY;

    if (!handleMask)
        return;

    std::list<AuraApplication*> effectApplications;
    GetApplicationList(effectApplications);

    for (std::list<AuraApplication*>::const_iterator apptItr = effectApplications.begin(); apptItr != effectApplications.end(); ++apptItr)
        if ((*apptItr)->HasEffect(GetEffIndex()))
            HandleEffect(*apptItr, handleMask, false);

    if (handleMask & AURA_EFFECT_HANDLE_CHANGE_AMOUNT)
    {
        if (!mark)
        {
            m_amount = newAmount;
            GetBase()->UpdateConcatenateAura(GetCaster(), newAmount, m_effIndex);
        }
        else
            SetAmount(newAmount);
        CalculateSpellMod();
    }

    if (handleMask & AURA_EFFECT_HANDLE_REAPPLY)
        CalculateSpellMod();

    for (std::list<AuraApplication*>::const_iterator apptItr = effectApplications.begin(); apptItr != effectApplications.end(); ++apptItr)
        if ((*apptItr)->HasEffect(GetEffIndex()))
            HandleEffect(*apptItr, handleMask, true);
}

void AuraEffect::HandleEffect(AuraApplication * aurApp, uint8 mode, bool apply)
{
    if(!GetBase())
        return;

    // check if call is correct, we really don't want using bitmasks here (with 1 exception)
    ASSERT(mode == AURA_EFFECT_HANDLE_REAL
        || mode == AURA_EFFECT_HANDLE_SEND_FOR_CLIENT
        || mode == AURA_EFFECT_HANDLE_CHANGE_AMOUNT
        || mode == AURA_EFFECT_HANDLE_STAT
        || mode == AURA_EFFECT_HANDLE_SKILL
        || mode == AURA_EFFECT_HANDLE_REAPPLY
        || mode == (AURA_EFFECT_HANDLE_CHANGE_AMOUNT | AURA_EFFECT_HANDLE_REAPPLY));

    // register/unregister effect in lists in case of real AuraEffect apply/remove
    // registration/unregistration is done always before real effect handling (some effect handlers code is depending on this)
    if (mode & AURA_EFFECT_HANDLE_REAL)
        aurApp->GetTarget()->_RegisterAuraEffect(this, apply);

    // real aura apply/remove, handle modifier
    if (mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_REAPPLY))
        ApplySpellMod(aurApp->GetTarget(), apply);

    // call scripts helping/replacing effect handlers
    bool prevented = false;
    if (apply)
        prevented = GetBase()->CallScriptEffectApplyHandlers(const_cast<AuraEffect const*>(this), const_cast<AuraApplication const*>(aurApp), (AuraEffectHandleModes)mode);
    else
        prevented = GetBase()->CallScriptEffectRemoveHandlers(const_cast<AuraEffect const*>(this), const_cast<AuraApplication const*>(aurApp), (AuraEffectHandleModes)mode);

    // check if script events have removed the aura or if default effect prevention was requested
    if ((apply && aurApp->GetRemoveMode()) || prevented)
        return;

    if (GetAuraType() >= TOTAL_AURAS)
        return;

    (*this.*AuraEffectHandler [GetAuraType()])(const_cast<AuraApplication const*>(aurApp), mode, apply);

    // check if script events have removed the aura or if default effect prevention was requested
    if (apply && aurApp->GetRemoveMode())
        return;

    // call scripts triggering additional events after apply/remove
    if (apply)
        GetBase()->CallScriptAfterEffectApplyHandlers(const_cast<AuraEffect const*>(this), const_cast<AuraApplication const*>(aurApp), (AuraEffectHandleModes)mode);
    else
        GetBase()->CallScriptAfterEffectRemoveHandlers(const_cast<AuraEffect const*>(this), const_cast<AuraApplication const*>(aurApp), (AuraEffectHandleModes)mode);
}

void AuraEffect::HandleEffect(Unit* target, uint8 mode, bool apply)
{
    AuraApplication* aurApp = GetBase()->GetApplicationOfTarget(target->GetGUID());
    ASSERT(aurApp);
    HandleEffect(aurApp, mode, apply);
}

void AuraEffect::ApplySpellMod(Unit* target, bool apply)
{
    if (!m_spellmod || target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->AddSpellMod(m_spellmod, apply);

    // reapply some passive spells after add/remove related spellmods
    // Warning: it is a dead loop if 2 auras each other amount-shouldn't happen
    switch (GetMiscValue())
    {
        case SPELLMOD_ALL_EFFECTS:
        case SPELLMOD_EFFECT1:
        case SPELLMOD_EFFECT2:
        case SPELLMOD_EFFECT3:
        case SPELLMOD_EFFECT4:
        case SPELLMOD_EFFECT5:
        {
            ObjectGuid guid = target->GetGUID();
            // Auras with charges do not mod amount of passive auras
            if (GetBase()->IsUsingCharges())
                return;

            Unit::AuraApplicationMap & auras = target->GetAppliedAuras();
            for (Unit::AuraApplicationMap::iterator iter = auras.begin(); iter != auras.end(); ++iter)
            {
                Aura* aura = iter->second->GetBase();
                // only passive and permament auras-active auras should have amount set on spellcast and not be affected
                // if aura is casted by others, it will not be affected
                if ((aura->IsPassive() || aura->IsPermanent()) && aura->GetCasterGUID() == guid && aura->GetSpellInfo()->IsAffectedBySpellMod(m_spellmod))
                {
                    if (GetMiscValue() == SPELLMOD_ALL_EFFECTS)
                    {
                        for (uint8 i = 0; i<MAX_SPELL_EFFECTS; ++i)
                        {
                            if (AuraEffect* aurEff = aura->GetEffect(i))
                                aurEff->RecalculateAmount();
                        }
                    }
                    else if (GetMiscValue() == SPELLMOD_EFFECT1)
                    {
                       if (AuraEffect* aurEff = aura->GetEffect(0))
                            aurEff->RecalculateAmount();
                    }
                    else if (GetMiscValue() == SPELLMOD_EFFECT2)
                    {
                       if (AuraEffect* aurEff = aura->GetEffect(1))
                            aurEff->RecalculateAmount();
                    }
                    else if (GetMiscValue() == SPELLMOD_EFFECT3)
                    {
                       if (AuraEffect* aurEff = aura->GetEffect(2))
                            aurEff->RecalculateAmount();
                    }
                    else if (GetMiscValue() == SPELLMOD_EFFECT4)
                    {
                       if (AuraEffect* aurEff = aura->GetEffect(3))
                            aurEff->RecalculateAmount();
                    }
                    else if (GetMiscValue() == SPELLMOD_EFFECT5)
                    {
                       if (AuraEffect* aurEff = aura->GetEffect(4))
                            aurEff->RecalculateAmount();
                    }
                }
            }
        }
        case SPELLMOD_CASTING_TIME:
        {
            if (apply)
                if (GetAmount() == -100)
                    if (Player* plr = target->ToPlayer())
                    {
                        for (PlayerSpellMap::const_iterator itr = plr->GetSpellMap().begin(); itr != plr->GetSpellMap().end(); ++itr)
                        {
                            if (itr->second->state == PLAYERSPELL_REMOVED)
                                continue;

                            uint32 spellId = itr->first;
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

                            if (!spellInfo)
                                continue;

                            if (spellInfo->IsAffectedBySpellMod(m_spellmod))
                                plr->RemoveSpellCooldown(spellInfo->Id, true);
                        }
                    }
            break;
        }
        default:
            break;
    }
}

void AuraEffect::Update(uint32 diff, Unit* caster)
{
    if(!GetBase())
        return;

    GetBase()->CallScriptEffectUpdateHandlers(diff, this);

    if (m_isPeriodic && (GetBase()->GetDuration() >=0 || GetBase()->IsPassive() || GetBase()->IsPermanent()))
    {
        if (m_periodicTimer > int32(diff))
            m_periodicTimer -= diff;
        else // tick also at m_periodicTimer == 0 to prevent lost last tick in case max m_duration == (max m_periodicTimer)*N
        {
            ++m_tickNumber;

            // update before tick (aura can be removed in TriggerSpell or PeriodicTick calls)
            m_periodicTimer += m_period - diff;
            UpdatePeriodic(caster);

            std::list<AuraApplication*> effectApplications;
            GetApplicationList(effectApplications);
            // tick on targets of effects
            for (std::list<AuraApplication*>::const_iterator apptItr = effectApplications.begin(); apptItr != effectApplications.end(); ++apptItr)
                if ((*apptItr)->HasEffect(GetEffIndex()))
                    PeriodicTick(*apptItr, caster, (SpellEffIndex) GetEffIndex());
        }
    }
}

void AuraEffect::UpdatePeriodic(Unit* caster)
{
    switch (GetAuraType())
    {
        case SPELL_AURA_PERIODIC_DUMMY:
            switch (GetSpellInfo()->ClassOptions.SpellClassSet)
            {
                case SPELLFAMILY_GENERIC:
                    switch (GetId())
                    {
                        case 58549: // Tenacity
                        case 59911: // Tenacity (vehicle)
                           GetBase()->RefreshDuration();
                           break;
                        case 66823: case 67618: case 67619: case 67620: // Paralytic Toxin
                            // Get 0 effect aura
                            if (AuraEffect* slow = GetBase()->GetEffect(0))
                            {
                                int32 newAmount = slow->GetAmount() - 10;
                                if (newAmount < -100)
                                    newAmount = -100;
                                slow->ChangeAmount(newAmount);
                            }
                            break;
                        case 167421:    //Khadgar's Watch
                            if (!caster->HasAura(167410))
                                caster->CastSpell(caster, 167410, true, NULL, this);
                            break;
                        default:
                            break;
                    }
                    break;
                case SPELLFAMILY_DEATHKNIGHT:
                    // Chains of Ice
                    if (GetSpellInfo()->ClassOptions.SpellClassMask[1] & 0x00004000)
                    {
                        // Get 0 effect aura
                        if (AuraEffect* slow = GetBase()->GetEffect(0))
                        {
                            int32 newAmount = slow->GetAmount() + GetAmount();
                            if (newAmount > 0)
                                newAmount = 0;
                            slow->ChangeAmount(newAmount);
                        }
                        return;
                    }
                    break;
                case SPELLFAMILY_DRUID:
                {
                    if (GetId() == 110621) // Mirror Image
                        m_isPeriodic = false;
                    break;
                }
                default:
                    break;
           }
       default:
           break;
    }
    GetBase()->CallScriptEffectUpdatePeriodicHandlers(this);
}

bool AuraEffect::IsAffectingSpell(SpellInfo const* spell) const
{
    if (!spell)
        return false;
    // Check family name
    if (spell->ClassOptions.SpellClassSet != m_spellInfo->ClassOptions.SpellClassSet)
        return false;

    // Check EffectClassMask
    if (m_spellInfo->GetEffect(m_effIndex, m_diffMode)->SpellClassMask & spell->ClassOptions.SpellClassMask)
        return true;

    return false;
}

void AuraEffect::SendTickImmune(Unit* target, Unit* caster) const
{
    if (caster)
        caster->SendSpellDamageImmune(target, m_spellInfo->Id, true);
}

void AuraEffect::PeriodicTick(AuraApplication * aurApp, Unit* caster, SpellEffIndex effIndex) const
{
    bool prevented = GetBase()->CallScriptEffectPeriodicHandlers(this, aurApp);
    if (prevented)
        return;

    Unit* target = aurApp->GetTarget();

    AuraSpellTrigger(target, caster, effIndex);

    switch (GetAuraType())
    {
        case SPELL_AURA_PERIODIC_DUMMY:
            HandlePeriodicDummyAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL:
            HandlePeriodicTriggerSpellAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE:
            HandlePeriodicTriggerSpellWithValueAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
            HandlePeriodicDamageAurasTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_LEECH:
            HandlePeriodicHealthLeechAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
            HandlePeriodicHealthFunnelAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_HEAL:
        case SPELL_AURA_OBS_MOD_HEALTH:
            HandlePeriodicHealAurasTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_MANA_LEECH:
            HandlePeriodicManaLeechAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_OBS_MOD_POWER:
            HandleObsModPowerAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_PERIODIC_ENERGIZE:
            HandlePeriodicEnergizeAuraTick(target, caster, effIndex);
            break;
        case SPELL_AURA_POWER_BURN:
            HandlePeriodicPowerBurnAuraTick(target, caster, effIndex);
            break;
        default:
            break;
    }
}

void AuraEffect::HandleProc(AuraApplication* aurApp, ProcEventInfo& eventInfo, SpellEffIndex effIndex)
{
    bool prevented = GetBase()->CallScriptEffectProcHandlers(this, const_cast<AuraApplication const*>(aurApp), eventInfo);
    if (prevented)
        return;

    switch (GetAuraType())
    {
        case SPELL_AURA_PROC_TRIGGER_SPELL:
            HandleProcTriggerSpellAuraProc(aurApp, eventInfo, effIndex);
            break;
        case SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE:
            HandleProcTriggerSpellWithValueAuraProc(aurApp, eventInfo, effIndex);
            break;
        case SPELL_AURA_PROC_TRIGGER_DAMAGE:
            HandleProcTriggerDamageAuraProc(aurApp, eventInfo, effIndex);
            break;
        case SPELL_AURA_RAID_PROC_FROM_CHARGE:
            HandleRaidProcFromChargeAuraProc(aurApp, eventInfo, effIndex);
            break;
        case SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE:
        case SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE2:
            HandleRaidProcFromChargeWithValueAuraProc(aurApp, eventInfo, effIndex);
            break;
        default:
            break;
    }
}

void AuraEffect::CleanupTriggeredSpells(Unit* target)
{
    uint32 tSpellId = m_spellInfo->GetEffect(GetEffIndex(), m_diffMode)->TriggerSpell;
    if (!tSpellId)
        return;

    SpellInfo const* tProto = sSpellMgr->GetSpellInfo(tSpellId);
    if (!tProto)
        return;

    if (tProto->GetDuration() != -1)
        return;

    // needed for spell 43680, maybe others
    // TODO: is there a spell flag, which can solve this in a more sophisticated way?
    if (m_spellInfo->GetEffect(GetEffIndex(), m_diffMode)->ApplyAuraName == SPELL_AURA_PERIODIC_TRIGGER_SPELL)
    {
        if (uint32(m_spellInfo->GetDuration()) == m_spellInfo->GetEffect(GetEffIndex(), m_diffMode)->ApplyAuraPeriod)
            return;

        if (tProto->AuraOptions.CumulativeAura)
            if (GetTotalTicks() < (int32)tProto->AuraOptions.CumulativeAura)
                return;
    }

    target->RemoveAurasDueToSpell(tSpellId, GetCasterGUID());
}

void AuraEffect::HandleShapeshiftBoosts(Unit* target, bool apply) const
{
    std::vector<uint32> spellId;

    switch (GetMiscValue())
    {
        case FORM_CAT:
            spellId.push_back(3025);     // Wild Charge
            spellId.push_back(48629);    // Swipe, Mangle, Thrash
            spellId.push_back(106840);   // Skull Bash, Stampeding Roar, Berserk

            if (target->HasAura(108299)) // Killer Instinct
                spellId.push_back(108300);
            break;
        case FORM_TRAVEL:
            spellId.push_back(5419);
            break;
        case FORM_AQUA:
            spellId.push_back(5421);
            break;
        case FORM_BEAR:
            spellId.push_back(1178);
            spellId.push_back(21178);  // Swipe, Wild Charge
            spellId.push_back(106829); // Mangle, Thrash, Skull Bash
            spellId.push_back(106899); // Stampeding Roar, Berserk

            if (target->HasAura(108299)) // Killer Instinct
                spellId.push_back(108300);
            break;
        case FORM_MOONKIN:
            spellId.push_back(24905);
            spellId.push_back(24907);
            // Glyph of the Start
            if (!apply || target->HasAura(114301))
                spellId.push_back(114302);  // Astral Form
            break;
        case FORM_FLIGHT:
            spellId.push_back(33948);
            spellId.push_back(34764);
            break;
        case FORM_FLIGHT_EPIC:
            spellId.push_back(40122);
            spellId.push_back(40121);
            break;
        case FORM_METAMORPHOSIS:
            spellId.push_back(103965);
            break;
        case FORM_SPIRITOFREDEMPTION:
            spellId.push_back(27792);
            spellId.push_back(27795);                               // must be second, this important at aura remove to prevent to early iterator invalidation.
            break;
        case FORM_SHADOW:
            spellId.push_back(49868);
            break;
        default:
            break;
    }

    if (apply)
    {
        if (Player* plrTarget = target->ToPlayer())
        {
            // Remove cooldown of spells triggered on stance change - they may share cooldown with stance spell
            for (std::vector<uint32>::iterator itr = spellId.begin(); itr != spellId.end(); ++itr)
            {
                plrTarget->RemoveSpellCooldown(*itr);
                plrTarget->CastSpell(target, *itr, true, NULL, this);
            }

            PlayerSpellMap const& sp_list = plrTarget->GetSpellMap();
            for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
            {
                if (itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled)
                    continue;

                bool hasAnalogy = false;

                for (std::vector<uint32>::iterator i = spellId.begin(); i != spellId.end(); ++i)
                    if (itr->first == (*i))
                    {
                        hasAnalogy = true;
                        break;
                    }

                if (hasAnalogy)
                    continue;

                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
                if (!spellInfo || !(spellInfo->Misc.Attributes[0] & (SPELL_ATTR0_PASSIVE | SPELL_ATTR0_HIDDEN_CLIENTSIDE)))
                    continue;

                if (spellInfo->Stances & (1<<(GetMiscValue()-1)))
                    target->CastSpell(target, itr->first, true, NULL, this);
            }

            // Also do it for Glyphs
            for (uint32 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
            {
                if (uint32 glyphId = plrTarget->GetGlyph(plrTarget->GetSpecIndex(), i))
                {
                    if (GlyphPropertiesEntry const* glyph = sGlyphPropertiesStore.LookupEntry(glyphId))
                    {
                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(glyph->SpellId);
                        if (!spellInfo || !(spellInfo->Misc.Attributes[0] & (SPELL_ATTR0_PASSIVE | SPELL_ATTR0_HIDDEN_CLIENTSIDE)))
                            continue;

                        if (spellInfo->Stances & (1<<(GetMiscValue()-1)))
                            target->CastSpell(target, glyph->SpellId, true, NULL, this);
                    }
                }
            }

            // Leader of the Pack
            if (plrTarget->HasSpell(17007))
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(24932);
                if (spellInfo && spellInfo->Stances & (1<<(GetMiscValue()-1)))
                    target->CastSpell(target, 24932, true, NULL, this);
            }

            // Improved Barkskin - apply/remove armor bonus due to shapeshift
            if (plrTarget->HasSpell(63410) || target->ToPlayer()->HasSpell(63411))
            {
                target->RemoveAurasDueToSpell(66530);
                if (GetMiscValue() == FORM_TRAVEL || GetMiscValue() == FORM_NONE) // "while in Travel Form or while not shapeshifted"
                    target->CastSpell(target, 66530, true);
            }

            switch (GetMiscValue())
            {
                case FORM_CAT:
                    // Savage Roar
                    if (target->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_DRUID, 0, 0x10000000, 0))
                        target->CastSpell(target, 62071, true);
                    break;
            }
        }
    }
    else
    {
        for (std::vector<uint32>::iterator itr = spellId.begin(); itr != spellId.end(); ++itr)
            target->RemoveAurasDueToSpell(*itr);

        const Unit::AuraEffectList& shapeshifts = target->GetAuraEffectsByType(SPELL_AURA_MOD_SHAPESHIFT);
        AuraEffect* newAura = NULL;
        // Iterate through all the shapeshift auras that the target has, if there is another aura with SPELL_AURA_MOD_SHAPESHIFT, then this aura is being removed due to that one being applied
        for (Unit::AuraEffectList::const_iterator itr = shapeshifts.begin(); itr != shapeshifts.end(); ++itr)
        {
            if ((*itr) != this)
            {
                newAura = *itr;
                break;
            }
        }
        Unit::AuraApplicationMap& tAuras = target->GetAppliedAuras();
        for (Unit::AuraApplicationMap::iterator itr = tAuras.begin(); itr != tAuras.end();)
        {
            // Use the new aura to see on what stance the target will be
            uint32 newStance = (1<<((newAura ? newAura->GetMiscValue() : 0)-1));

            // If the stances are not compatible with the spell, remove it
            if (itr->second->GetBase()->IsRemovedOnShapeLost(target) && !(itr->second->GetBase()->GetSpellInfo()->Stances & newStance))
                target->RemoveAura(itr);
            else
                ++itr;
        }
    }

    // Heart of the Wild
    if (apply && target->GetTypeId() == TYPEID_PLAYER)
    {
        Player* player = target->ToPlayer();
        if (player->HasSpell(108288))
        {
            ShapeshiftForm form = ShapeshiftForm(GetMiscValue());

            // check Heart of the Wild spec-specific buffs
            bool checkers[] = { target->HasAura(108291), target->HasAura(108292), target->HasAura(108293), target->HasAura(108294) };
            if ((checkers[0] || checkers[1] || checkers[3]) && form == FORM_BEAR)
                target->CastSpell(target, 123738, true);
            if ((checkers[0] || checkers[2] || checkers[3]) && form == FORM_CAT)
                target->CastSpell(target, 123737, true);
        }
    }
    else
    {
        target->RemoveAurasDueToSpell(123737);
        target->RemoveAurasDueToSpell(123738);
    }
}

/*********************************************************/
/***               AURA EFFECT HANDLERS                ***/
/*********************************************************/

/**************************************/
/***       VISIBILITY & PHASES      ***/
/**************************************/

void AuraEffect::HandleModInvisibilityDetect(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    InvisibilityType type = InvisibilityType(GetMiscValue());

    if (apply)
    {
        target->m_invisibilityDetect.AddFlag(type);
        target->m_invisibilityDetect.AddValue(type, GetAmount());
    }
    else
    {
        if (!target->HasAuraType(SPELL_AURA_MOD_INVISIBILITY_DETECT))
            target->m_invisibilityDetect.DelFlag(type);

        target->m_invisibilityDetect.AddValue(type, -GetAmount());
        //then exit from visibility try to link posible off-line thread state for some atackers npc.
        target->getHostileRefManager().UpdateVisibility();
    }

    // call functions which may have additional effects after chainging state of unit
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleModInvisibility(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    InvisibilityType type = InvisibilityType(GetMiscValue());

    if (apply)
    {
        // apply glow vision
        //! spell 164042 used for Q34445. Is glow apply on alll or not?
        if (target->GetTypeId() == TYPEID_PLAYER && m_spellInfo->Id != 164042 && m_spellInfo->Id  != 165052)
           target->SetByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_INVISIBILITY_GLOW); // TODO : Check PLAYER_FIELD_AURA_VISION

        target->m_invisibility.AddFlag(type);
        target->m_invisibility.AddValue(type, GetAmount());
    }
    else
    {
        if (!target->HasAuraType(SPELL_AURA_MOD_INVISIBILITY))
        {
            // if not have different invisibility auras.
            // remove glow vision
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->RemoveByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_INVISIBILITY_GLOW); // TODO : Check PLAYER_FIELD_AURA_VISION

            target->m_invisibility.DelFlag(type);
        }
        else
        {
            bool found = false;
            Unit::AuraEffectList const& invisAuras = target->GetAuraEffectsByType(SPELL_AURA_MOD_INVISIBILITY);
            for (Unit::AuraEffectList::const_iterator i = invisAuras.begin(); i != invisAuras.end(); ++i)
            {
                if (GetMiscValue() == (*i)->GetMiscValue())
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                target->m_invisibility.DelFlag(type);
        }

        target->m_invisibility.AddValue(type, -GetAmount());
    }

    // call functions which may have additional effects after chainging state of unit
    if (apply && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        // drop flag at invisibiliy in bg
        target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);
    }
    target->UpdateObjectVisibility();
}

//TODO: Finish this aura
void AuraEffect::HandleModCamouflage(AuraApplication const *aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        //
    }
    else if (!(target->isCamouflaged()))
    {
        if (Unit* pet = GetCaster()->GetGuardianPet())
            pet->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
    }
}

void AuraEffect::HandleModStealthDetect(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    StealthType type = StealthType(GetMiscValue());

    if (apply)
    {
        target->m_stealthDetect.AddFlag(type);
        target->m_stealthDetect.AddValue(type, GetAmount());
    }
    else
    {
        if (!target->HasAuraType(SPELL_AURA_MOD_STEALTH_DETECT))
            target->m_stealthDetect.DelFlag(type);

        target->m_stealthDetect.AddValue(type, -GetAmount());
    }

    // call functions which may have additional effects after chainging state of unit
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleModStealth(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    StealthType type = StealthType(GetMiscValue());

    if (apply)
    {
        target->m_stealth.AddFlag(type);
        target->m_stealth.AddValue(type, GetAmount());

        target->SetStandVisFlags(UNIT_STAND_FLAGS_CREEP);
        if (target->GetTypeId() == TYPEID_PLAYER)
            target->SetByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_STEALTH); // TODO : Check PLAYER_FIELD_AURA_VISION
    }
    else
    {
        target->m_stealth.AddValue(type, -GetAmount());

        if (!target->HasAuraType(SPELL_AURA_MOD_STEALTH)) // if last SPELL_AURA_MOD_STEALTH
        {
            target->m_stealth.DelFlag(type);

            target->RemoveStandVisFlags(UNIT_STAND_FLAGS_CREEP);
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->RemoveByteFlag(PLAYER_FIELD_BYTES2, PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION, PLAYER_FIELD_BYTE2_STEALTH); // TODO : Check PLAYER_FIELD_AURA_VISION
        }
    }

    // call functions which may have additional effects after chainging state of unit
    if (apply && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        // drop flag at stealth in bg
        target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);
    }
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleModStealthLevel(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    StealthType type = StealthType(GetMiscValue());

    if (apply)
        target->m_stealth.AddValue(type, GetAmount());
    else
        target->m_stealth.AddValue(type, -GetAmount());

    // call functions which may have additional effects after chainging state of unit
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleSpiritOfRedemption(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // prepare spirit state
    if (apply)
    {
        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            // disable breath/etc timers
            target->ToPlayer()->StopMirrorTimers();

            // set stand state (expected in this form)
            if (!target->IsStandState())
                target->SetStandState(UNIT_STAND_STATE_STAND);
        }

        target->SetHealth(1);
    }
    // die at aura end
    else if (target->isAlive())
        // call functions which may have additional effects after chainging state of unit
        target->setDeathState(JUST_DIED);

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    else
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void AuraEffect::HandleAuraGhost(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
    {
        target->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST);
        target->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
        target->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
    }
    else
    {
        if (target->HasAuraType(SPELL_AURA_GHOST))
            return;

        target->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST);
        target->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
        target->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);
    }
}

void AuraEffect::HandlePhase(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();


    if (Player* player = target->ToPlayer())
    {
        if (apply)	
            player->GetPhaseMgr().RegisterPhasingAuraEffect(this);	
        else	
            player->GetPhaseMgr().UnRegisterPhasingAuraEffect(this);
    }
    else
    {
        uint32 newPhase = 0;
        Unit::AuraEffectList const& phases = target->GetAuraEffectsByType(SPELL_AURA_PHASE);	
        if (!phases.empty())	
            for (Unit::AuraEffectList::const_iterator itr = phases.begin(); itr != phases.end(); ++itr)	
                newPhase |= (*itr)->GetMiscValue();

        Unit::AuraEffectList const& phases2 = target->GetAuraEffectsByType(SPELL_AURA_PHASE_2);	
        if (!phases2.empty())	
            for (Unit::AuraEffectList::const_iterator itr = phases2.begin(); itr != phases2.end(); ++itr)	
                newPhase |= (*itr)->GetMiscValue();

        if (!newPhase)
        {
            newPhase = PHASEMASK_NORMAL;
            if (Creature* creature = target->ToCreature())
                if (CreatureData const* data = sObjectMgr->GetCreatureData(creature->GetDBTableGUIDLow()))
                    newPhase = data->phaseMask;
        }

        target->SetPhaseMask(newPhase, false);
    }

    // call functions which may have additional effects after chainging state of unit
    // phase auras normally not expected at BG but anyway better check
    if (apply && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        // drop flag at invisibiliy in bg
        target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);
    }

    // need triggering visibility update base at phase update of not GM invisible (other GMs anyway see in any phases)
    if (target->IsVisible())
        target->UpdateObjectVisibility();
}

/**********************/
/***   UNIT MODEL   ***/
/**********************/

void AuraEffect::HandleAuraModShapeshift(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    uint32 modelid = 0;
    Powers PowerType = POWER_MANA;
    ShapeshiftForm form = ShapeshiftForm(GetMiscValue());
    
    // Remove aura from invisible model
    target->RemoveAurasByType(SPELL_AURA_CLONE_CASTER);

    switch (form)
    {
        case FORM_FIERCE_TIGER:
        case FORM_STURDY_OX:
        case FORM_CAT:                                      // 0x01
        case FORM_GHOUL:                                    // 0x07
            PowerType = POWER_ENERGY;
            break;

        case FORM_BEAR:                                     // 0x05
        case FORM_BATTLESTANCE:                             // 0x11
        case FORM_DEFENSIVESTANCE:                          // 0x12
        case FORM_BERSERKERSTANCE:                          // 0x13
        case FORM_GLADIATORSTANCE:                          // 0x21
            PowerType = POWER_RAGE;
            break;

        case FORM_TREE:                                     // 0x02
        case FORM_TRAVEL:                                   // 0x03
        case FORM_AQUA:                                     // 0x04
        case FORM_AMBIENT:                                  // 0x06
        case FORM_THARONJA_SKELETON:                        // 0x0A
        case FORM_TEST_OF_STRENGTH:                         // 0x0B
        case FORM_BLB_PLAYER:                               // 0x0C
        case FORM_SHADOW_DANCE:                             // 0x0D
        case FORM_CREATUREBEAR:                             // 0x0E
        case FORM_CREATURECAT:                              // 0x0F
        case FORM_GHOSTWOLF:                                // 0x10
            break;
        case FORM_SPIRITED_CRANE:                           // 0x09
        case FORM_WISE_SERPENT:                             // 0x14
            PowerType = POWER_MANA;
            break;
        case FORM_ZOMBIE:                                   // 0x15
        case FORM_METAMORPHOSIS:                            // 0x16
        case FORM_UNDEAD:                                   // 0x19
        case FORM_MASTER_ANGLER:                            // 0x1A
        case FORM_FLIGHT_EPIC:                              // 0x1B
        case FORM_SHADOW:                                   // 0x1C
        case FORM_FLIGHT:                                   // 0x1D
        case FORM_STEALTH:                                  // 0x1E
        case FORM_MOONKIN:                                  // 0x1F
        case FORM_SPIRITOFREDEMPTION:                       // 0x20
            break;
        default:
            sLog->outError(LOG_FILTER_SPELLS_AURAS, "Auras: Unknown Shapeshift Type: %u", GetMiscValue());
    }

    modelid = target->GetModelForForm(form);

    if (apply)
    {
        // remove polymorph before changing display id to keep new display id
        switch (form)
        {
            case FORM_CAT:
            case FORM_TREE:
            case FORM_TRAVEL:
            case FORM_AQUA:
            case FORM_BEAR:
            case FORM_FLIGHT_EPIC:
            case FORM_FLIGHT:
            case FORM_MOONKIN:
            {
                // remove movement affects
                uint32 mechanicMask = (1 << MECHANIC_SNARE);
                if (target->HasAura(96429) || form == FORM_MOONKIN)
                    mechanicMask |= (1 << MECHANIC_ROOT);

                target->RemoveAurasWithMechanic(mechanicMask);

                // and polymorphic affects
                if (target->IsPolymorphed())
                    target->RemoveAurasDueToSpell(target->getTransForm());
                break;
            }
            default:
               break;
        }

        // remove other shapeshift before applying a new one
        target->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT, ObjectGuid::Empty, GetBase());

        // stop handling the effect if it was removed by linked event
        if (aurApp->GetRemoveMode())
            return;

        if (modelid > 0)
            target->SetDisplayId(modelid);

        if (PowerType != POWER_MANA)
        {
            int32 oldPower = target->GetPower(PowerType);
            // reset power to default values only at power change
            if (target->getPowerType() != PowerType)
                target->setPowerType(PowerType);
        }

        // stop handling the effect if it was removed by linked event
        if (aurApp->GetRemoveMode())
            return;

        target->SetShapeshiftForm(form);
    }
    else
    {
        // reset model id if no other auras present
        // may happen when aura is applied on linked event on aura removal
        if (!target->HasAuraType(SPELL_AURA_MOD_SHAPESHIFT))
        {
            target->SetShapeshiftForm(FORM_NONE);
            if (target->getClass() == CLASS_DRUID)
            {
                target->setPowerType(POWER_MANA);
                // Remove movement impairing effects also when shifting out
                target->RemoveMovementImpairingAuras();
            }
        }

        if (modelid > 0)
            target->RestoreDisplayId();

        switch (form)
        {
            // Nordrassil Harness - bonus
            case FORM_BEAR:
            case FORM_CAT:
                if (AuraEffect* dummy = target->GetAuraEffect(37315, 0))
                    target->CastSpell(target, 37316, true, NULL, dummy);
                break;
            // Nordrassil Regalia - bonus
            case FORM_MOONKIN:
                if (AuraEffect* dummy = target->GetAuraEffect(37324, 0))
                    target->CastSpell(target, 37325, true, NULL, dummy);
                break;
            default:
                break;
        }
    }

    // adding/removing linked auras
    // add/remove the shapeshift aura's boosts
    HandleShapeshiftBoosts(target, apply);

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->InitDataForForm();

    if (target->getClass() == CLASS_DRUID)
    {
        // Dash
        if (AuraEffect* aurEff = target->GetAuraEffect(SPELL_AURA_MOD_INCREASE_SPEED, SPELLFAMILY_DRUID, 0, 0, 0x8))
            aurEff->RecalculateAmount();

        // Disarm handling
        // If druid shifts while being disarmed we need to deal with that since forms aren't affected by disarm
        // and also HandleAuraModDisarm is not triggered
        if (!target->CanUseAttackType(BASE_ATTACK))
        {
            if (Item* pItem = target->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
            {
                target->ToPlayer()->_ApplyWeaponDamage(EQUIPMENT_SLOT_MAINHAND, pItem, apply);
            }
        }
    }

    // stop handling the effect if it was removed by linked event
    if (apply && aurApp->GetRemoveMode())
        return;

    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        SpellShapeshiftFormEntry const* shapeInfo = sSpellShapeshiftFormStore.LookupEntry(form);
        // Learn spells for shapeshift form - no need to send action bars or add spells to spellbook
        for (uint8 i = 0; i < MAX_SHAPESHIFT_SPELLS; ++i)
        {
            if (!shapeInfo->PresetSpellID[i])
                continue;

            if (apply)
                target->ToPlayer()->AddTemporarySpell(shapeInfo->PresetSpellID[i]);
            else
                target->ToPlayer()->RemoveTemporarySpell(shapeInfo->PresetSpellID[i]);
        }
    }
}

void AuraEffect::HandleAuraTransform(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->RemoveAurasByType(SPELL_AURA_CLONE_CASTER);

        // update active transform spell only when transform or shapeshift not set or not overwriting negative by positive case
        SpellInfo const* transformSpellInfo = sSpellMgr->GetSpellInfo(target->getTransForm());
        if (!transformSpellInfo || !GetSpellInfo()->IsPositive() || transformSpellInfo->IsPositive())
        //if (!target->GetModelForForm(target->GetShapeshiftForm()) || !GetSpellInfo()->IsPositive())
        {
            target->setTransForm(GetId());

            // special case (spell specific functionality)
            if (GetMiscValue() == 0)
            {
                switch (GetId())
                {
                    // Orb of Deception
                    case 16739:
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return;

                        switch (target->getRace())
                        {
                            // Blood Elf
                            case RACE_BLOODELF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 17829 : 17830);
                                break;
                            // Orc
                            case RACE_ORC:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10139 : 10140);
                                break;
                            // Troll
                            case RACE_TROLL:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10135 : 10134);
                                break;
                            // Tauren
                            case RACE_TAUREN:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10136 : 10147);
                                break;
                            // Undead
                            case RACE_UNDEAD_PLAYER:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10146 : 10145);
                                break;
                            // Draenei
                            case RACE_DRAENEI:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 17827 : 17828);
                                break;
                            // Dwarf
                            case RACE_DWARF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10141 : 10142);
                                break;
                            // Gnome
                            case RACE_GNOME:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10148 : 10149);
                                break;
                            // Human
                            case RACE_HUMAN:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10137 : 10138);
                                break;
                            // Night Elf
                            case RACE_NIGHTELF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 10143 : 10144);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    // Murloc costume
                    case 42365:
                        target->SetDisplayId(21723);
                        break;
                    // Dread Corsair
                    case 50517:
                    // Corsair Costume
                    case 51926:
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return;

                        switch (target->getRace())
                        {
                            // Blood Elf
                            case RACE_BLOODELF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25032 : 25043);
                                break;
                            // Orc
                            case RACE_ORC:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25039 : 25050);
                                break;
                            // Troll
                            case RACE_TROLL:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25041 : 25052);
                                break;
                            // Tauren
                            case RACE_TAUREN:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25040 : 25051);
                                break;
                            // Undead
                            case RACE_UNDEAD_PLAYER:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25042 : 25053);
                                break;
                            // Draenei
                            case RACE_DRAENEI:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25033 : 25044);
                                break;
                            // Dwarf
                            case RACE_DWARF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25034 : 25045);
                                break;
                            // Gnome
                            case RACE_GNOME:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25035 : 25046);
                                break;
                            // Human
                            case RACE_HUMAN:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25037 : 25048);
                                break;
                            // Night Elf
                            case RACE_NIGHTELF:
                                target->SetDisplayId(target->getGender() == GENDER_MALE ? 25038 : 25049);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    // Pygmy Oil
                    case 53806:
                        target->SetDisplayId(14973);
                        break;
                    // Honor the Dead
                    case 65386:
                    case 65495:
                        target->SetDisplayId(target->getGender() == GENDER_MALE ? 29203 : 29204);
                        break;
                    // Darkspear Pride
                    case 75532:
                        target->SetDisplayId(target->getGender() == GENDER_MALE ? 31737 : 31738);
                        break;
                    // Gnomeregan Pride
                    case 75531:
                        target->SetDisplayId(target->getGender() == GENDER_MALE ? 31654 : 31655);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(GetMiscValue());
                if (!ci)
                {
                    target->SetDisplayId(16358);              // pig pink ^_^
                    sLog->outError(LOG_FILTER_SPELLS_AURAS, "Auras: unknown creature id = %d (only need its modelid) From Spell Aura Transform in Spell ID = %d", GetMiscValue(), GetId());
                }
                else
                {
                    uint32 model_id = 0;

                    if (uint32 modelid = ci->GetRandomValidModelId())
                        model_id = modelid;                     // Will use the default model here

                    // Polymorph (sheep)
                    if (GetSpellInfo()->ClassOptions.SpellClassSet == SPELLFAMILY_MAGE && GetSpellInfo()->Misc.SpellIconID == 82 && GetSpellInfo()->GetSpellVisual(DIFFICULTY_NONE) == 12978)
                        if (Unit* caster = GetCaster())
                        {
                            if (caster->HasAura(52648))         // Glyph of the Penguin
                                model_id = 26452;
                            else if (caster->HasAura(57924))         // Glyph of a porcupine
                                model_id = 40119;
                            else if (caster->HasAura(57927))         // Glyph of a monkey
                                model_id = 29963;
                            else if (caster->HasAura(58136))         // Glyph of a polar bear
                                model_id = 23948;
                        }

                    // Polymorph (Hex)
                    if (GetSpellInfo()->ClassOptions.SpellClassSet == SPELLFAMILY_SHAMAN && GetSpellInfo()->Misc.SpellIconID == 3058 && GetSpellInfo()->GetSpellVisual(DIFFICULTY_NONE) == 12780)
                        if (Unit* caster = GetCaster())
                        {
                            if (caster->HasAura(147785))         // Glyph of the Compy
                                model_id = 47428;
                        }

                    // Prevent visual bug with mirrors image
                    if (target->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE))
                        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);

                    target->SetDisplayId(model_id);

                    // Dragonmaw Illusion (set mount model also)
                    if (GetId() == 42016 && target->GetMountID() && !target->GetAuraEffectsByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED).empty())
                        target->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, 16314);
                }
            }
        }

        // polymorph case
        if ((mode & AURA_EFFECT_HANDLE_REAL) && target->GetTypeId() == TYPEID_PLAYER && target->IsPolymorphed())
        {
            // for players, start regeneration after 1s (in polymorph fast regeneration case)
            // only if caster is Player (after patch 2.4.2)
            if (GetCasterGUID().IsPlayer())
                target->ToPlayer()->setRegenTimerCount(1*IN_MILLISECONDS);

            //dismount polymorphed target (after patch 2.4.2)
            if (target->IsMounted())
                target->RemoveAurasByType(SPELL_AURA_MOUNTED);
            //remove fly aura Zen Flight
            if (target->HasAura(125883))
                target->RemoveAurasDueToSpell(125883, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);
        }
    }
    else
    {
        // HandleEffect(this, AURA_EFFECT_HANDLE_SEND_FOR_CLIENT, true) will reapply it if need
        if (target->getTransForm() == GetId())
            target->setTransForm(0);

        target->RestoreDisplayId();

        // Dragonmaw Illusion (restore mount model)
        if (GetId() == 42016 && target->GetMountID() == 16314)
        {
            if (!target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).empty())
            {
                uint32 cr_id = target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetMiscValue();
                if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(cr_id))
                {
                    uint32 team = 0;
                    if (target->GetTypeId() == TYPEID_PLAYER)
                        team = target->ToPlayer()->GetTeam();

                    uint32 displayID = sObjectMgr->ChooseDisplayId(team, ci);
                    sObjectMgr->GetCreatureModelRandomGender(&displayID);

                    target->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, displayID);
                }
            }
        }
    }
}

void AuraEffect::HandleAuraModScale(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if(GetAmount() > 0)
        target->ApplyPercentModFloatValue(OBJECT_FIELD_SCALE, (float)GetAmount(), apply);
    else
        target->ApplyPercentModFloatValue(OBJECT_FIELD_SCALE, -(float)GetAmount(), !apply);
}

void AuraEffect::HandleAuraCloneCaster(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        Unit* caster = GetCaster();
        if (!caster || caster == target)
            return;

        // Remove aura from invisible model
        target->RemoveAurasByType(SPELL_AURA_TRANSFORM);

        // What must be cloned? at least display and scale
        target->SetDisplayId(caster->GetDisplayId());
        //target->SetObjectScale(caster->GetFloatValue(OBJECT_FIELD_SCALE)); // we need retail info about how scaling is handled (aura maybe?)
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
    }
    else
    {
        target->SetDisplayId(target->GetNativeDisplayId());
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
    }
}

void AuraEffect::HandleAuraInitializeImages(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        Unit* caster = GetCaster();
        if (!caster || caster == target)
            return;

        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
    }
    else
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
}

/************************/
/***      FIGHT       ***/
/************************/

void AuraEffect::HandleFeignDeath(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        UnitList targets;
        Trinity::AnyUnfriendlyUnitInObjectRangeCheck u_check(target, target, target->GetMap()->GetVisibilityRange());
        Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(target, targets, u_check);
        target->VisitNearbyObject(target->GetMap()->GetVisibilityRange(), searcher);
        for (UnitList::iterator iter = targets.begin(); iter != targets.end(); ++iter)
        {
            if (!(*iter)->HasUnitState(UNIT_STATE_CASTING))
                continue;

            for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_MAX_SPELL; i++)
            {
                if ((*iter)->GetCurrentSpell(i)
                && (*iter)->GetCurrentSpell(i)->m_targets.GetUnitTargetGUID() == target->GetGUID())
                {
                    (*iter)->InterruptSpell(CurrentSpellTypes(i), false);
                }
            }
        }
        target->CombatStop();
        target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);

        // prevent interrupt message
        if (GetCasterGUID() == target->GetGUID() && target->GetCurrentSpell(CURRENT_GENERIC_SPELL))
            target->FinishSpell(CURRENT_GENERIC_SPELL, false);
        target->InterruptNonMeleeSpells(true);
        target->getHostileRefManager().deleteReferences();

        // stop handling the effect if it was removed by linked event
        if (aurApp->GetRemoveMode())
            return;

        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
        target->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        target->AddUnitState(UNIT_STATE_DIED);

        if (Creature* creature = target->ToCreature())
        {
            creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            creature->SetReactState(REACT_PASSIVE);
            creature->RemoveAurasAllDots();
        }
    }
    else
    {
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
        target->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
        target->ClearUnitState(UNIT_STATE_DIED);

        if (Creature* creature = target->ToCreature())
        {
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            creature->InitializeReactState();
        }

        Powers power = target->getPowerType();
        target->SetPower(power, target->GetPower(power));
    }
}

void AuraEffect::HandleModUnattackable(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
    if (!apply && target->HasAuraType(SPELL_AURA_MOD_UNATTACKABLE))
        return;

    target->ApplyModFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE, apply);

    // call functions which may have additional effects after chainging state of unit
    if (apply && (mode & AURA_EFFECT_HANDLE_REAL))
    {
        target->CombatStop();
        target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);
    }
}

void AuraEffect::HandleAuraModDisarm(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    AuraType type = GetAuraType();

    //Prevent handling aura twice
    if ((apply) ? target->GetAuraEffectsByType(type).size() > 1 : target->HasAuraType(type))
        return;

    uint32 field, flag, slot;
    WeaponAttackType attType;
    switch (type)
    {
    case SPELL_AURA_MOD_DISARM:
        field=UNIT_FIELD_FLAGS;
        flag=UNIT_FLAG_DISARMED;
        slot=EQUIPMENT_SLOT_MAINHAND;
        attType=BASE_ATTACK;
        break;
    case SPELL_AURA_MOD_DISARM_OFFHAND:
        field=UNIT_FIELD_FLAGS_2;
        flag=UNIT_FLAG2_DISARM_OFFHAND;
        slot=EQUIPMENT_SLOT_OFFHAND;
        attType=OFF_ATTACK;
        break;
    case SPELL_AURA_MOD_DISARM_RANGED:
        /*field=UNIT_FIELD_FLAGS_2;
        flag=UNIT_FLAG2_DISARM_RANGED;
        slot=EQUIPMENT_SLOT_MAINHAND;
        attType=RANGED_ATTACK;
        break*/
    default:
        return;
    }

    // if disarm aura is to be removed, remove the flag first to reapply damage/aura mods
    if (!apply)
        target->RemoveFlag(field, flag);

    // Handle damage modification, shapeshifted druids are not affected
    if (target->GetTypeId() == TYPEID_PLAYER && !target->IsInFeralForm())
    {
        if (Item* pItem = target->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        {
            uint8 attacktype = Player::GetAttackBySlot(slot);

            if (attacktype < MAX_ATTACK)
            {
                target->ToPlayer()->_ApplyWeaponDamage(slot, pItem, !apply);
//                target->ToPlayer()->_ApplyWeaponDependentAuraMods(pItem, WeaponAttackType(attacktype), !apply);
            }
        }
    }

    // if disarm effects should be applied, wait to set flag until damage mods are unapplied
    if (apply)
        target->SetFlag(field, flag);

    if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->GetCurrentEquipmentId())
        target->UpdateDamagePhysical(attType);

    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_DISARM, apply);
}

void AuraEffect::HandleAuraModSilence(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);

        // call functions which may have additional effects after chainging state of unit
        // Stop cast only spells vs PreventionType == SPELL_PREVENTION_TYPE_SILENCE
        for (uint32 i = CURRENT_MELEE_SPELL; i < CURRENT_MAX_SPELL; ++i)
            if (Spell* spell = target->GetCurrentSpell(CurrentSpellTypes(i)))
                if (spell->m_spellInfo->Categories.PreventionType == SPELL_PREVENTION_TYPE_SILENCE)
                {
                    // Stop spells on prepare or casting state
                    target->InterruptSpell(CurrentSpellTypes(i), false);
                    if(GetBase() && GetCaster() && GetCaster()->HasAura(58618)) // Glyph of Strangulate
                        GetBase()->SetDuration(GetBase()->GetDuration() + 2000);
                }
    }
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_SILENCE) || target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;

        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
    }

    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_SILENCE, apply);
}

void AuraEffect::HandleAuraModPacify(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    uint32 _flags = UNIT_FLAG_PACIFIED;;
    uint32 _flags3 = 0;;
    if(m_spellInfo->Id == 51514) // Hack for Hex, sniff send both flags
        _flags |= UNIT_FLAG_SILENCED;

    if (apply)
    {
        if (m_spellInfo->Id == 1022) // Hand of Protection
        {
            target->RemoveAura(46924); // Warrior - Bladestorm
            target->RemoveAura(51690); // Rogue - Killing Spree
        }
        target->SetFlag(UNIT_FIELD_FLAGS, _flags);
    }
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_PACIFY) || target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, _flags);
    }
    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_PACIFY, apply);
}

void AuraEffect::HandleModNpcFlags2(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    uint32 _flags = UNIT_FLAG2_UNK7;

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS, _flags);
    else
        target->RemoveFlag(UNIT_FIELD_FLAGS, _flags);
}

void AuraEffect::HandleAuraModPacifyAndSilence(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // Vengeance of the Blue Flight (TODO: REMOVE THIS!)
    if (m_spellInfo->Id == 45839)
    {
        if (apply)
            target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        else
            target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;
    }

    if (apply)
    {
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
        target->CastStop();
    }
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(SPELL_AURA_MOD_PACIFY) || target->HasAuraType(SPELL_AURA_MOD_PACIFY_SILENCE))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_SILENCED);
    }

    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_PACIFYSILENCE, apply);
}

void AuraEffect::HandleAuraAllowOnlyAbility(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        if (apply)
            target->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_ALLOW_ONLY_ABILITY);
        else
        {
            // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
            if (target->HasAuraType(SPELL_AURA_ALLOW_ONLY_ABILITY))
                return;
            target->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_ALLOW_ONLY_ABILITY);
        }
    }
}

/****************************/
/***      TRACKING        ***/
/****************************/

void AuraEffect::HandleAuraTrackCreatures(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    uint32 mask = 1 << (GetMiscValue() - 1);
    // Track Humanoids
    if (GetId() == 5225)
        // Glyph of the Predator
        if (!apply && target->HasAuraTypeWithMiscvalue(SPELL_AURA_TRACK_CREATURES, GetMiscValue()) || target->HasAura(114280))
            mask |= 1 << (CREATURE_TYPE_BEAST - 1);

    target->ApplyModFlag(PLAYER_FIELD_TRACK_CREATURE_MASK, mask, apply);
}

void AuraEffect::HandleAuraTrackResources(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->SetUInt32Value(PLAYER_FIELD_TRACK_RESOURCE_MASK, (apply) ? ((uint32)1)<<(GetMiscValue()-1): 0);
}

void AuraEffect::HandleAuraTrackStealthed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (!(apply))
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }
    target->ApplyModFlag(PLAYER_FIELD_LOCAL_FLAGS, PLAYER_FIELD_BYTE_TRACK_STEALTHED, apply);
}

void AuraEffect::HandleAuraModStalked(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // used by spells: Hunter's Mark, Mind Vision, Syndicate Tracker (MURP) DND
    if (apply)
        target->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_TRACK_UNIT);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (!target->HasAuraType(GetAuraType()))
            target->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_TRACK_UNIT);
    }

    // call functions which may have additional effects after chainging state of unit
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleAuraUntrackable(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetStandVisFlags(UNIT_STAND_FLAGS_UNTRACKABLE);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveStandVisFlags(UNIT_STAND_FLAGS_UNTRACKABLE);
    }
}

/****************************/
/***  SKILLS & TALENTS    ***/
/****************************/

void AuraEffect::HandleAuraModPetTalentsPoints(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;
}

void AuraEffect::HandleAuraModSkill(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_SKILL)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    uint32 prot = GetMiscValue();
    int32 points = GetAmount();


    if (prot == SKILL_DEFENSE)
        return;

    target->ModifySkillBonus(prot, (apply ? points : -points), GetAuraType() == SPELL_AURA_MOD_SKILL_TALENT);
}

/****************************/
/***       MOVEMENT       ***/
/****************************/

void AuraEffect::HandleAuraMounted(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        uint32 creatureEntry = GetMiscValue();
        uint32 displayId = 0;
        uint32 vehicleId = 0;

        if (MountEntry const* mountEntry = sDB2Manager.GetMount(GetId()))
        {
            displayId = mountEntry->DisplayID;
            // TODO: CREATE TABLE mount_vehicle (mountId, vehicleCreatureId) for future mounts that are vehicles (new mounts no longer have proper data in MiscValue)
            //if (MountVehicle const* mountVehicle = sObjectMgr->GetMountVehicle(mountEntry->Id))
            //    creatureEntry = mountVehicle->VehicleCreatureId;
        }

        // Festive Holiday Mount
        if (target->HasAura(62061))
        {
            if (GetBase()->HasEffectType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                creatureEntry = 24906;
            else
                creatureEntry = 15665;
        }

        if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(creatureEntry))
        {
            vehicleId = ci->VehicleId;

            uint32 team = 0;
            if (target->GetTypeId() == TYPEID_PLAYER)
                team = target->ToPlayer()->GetTeam();

            if (!displayId || vehicleId)
            {
                displayId = ObjectMgr::ChooseDisplayId(team, ci);
                sObjectMgr->GetCreatureModelRandomGender(&displayId);
            }


            //some spell has one aura of mount and one of vehicle
            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (GetSpellInfo()->GetEffect(i, m_diffMode)->Effect == SPELL_EFFECT_SUMMON
                    && GetSpellInfo()->GetEffect(i, m_diffMode)->MiscValue == GetMiscValue())
                    displayId = 0;
        }

        target->Mount(displayId, vehicleId, GetMiscValue());
    }
    else
    {
        target->Dismount();
        //some mounts like Headless Horseman's Mount or broom stick are skill based spell
        // need to remove ALL arura related to mounts, this will stop client crash with broom stick
        // and never endless flying after using Headless Horseman's Mount
        if (mode & AURA_EFFECT_HANDLE_REAL)
            target->RemoveAurasByType(SPELL_AURA_MOUNTED);
    }
    target->UpdateSpeed(MOVE_RUN, true);
    target->UpdateMount();
}

void AuraEffect::HandleAuraAllowFlight(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()) || target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
            return;
    }

    //! Not entirely sure if this should be sent for creatures as well, but I don't think so.
    if (!apply)
    {
        target->m_movementInfo.SetFallTime(0);
        target->RemoveUnitMovementFlag(MOVEMENTFLAG_MASK_MOVING_FLY);
        target->AddUnitMovementFlag(MOVEMENTFLAG_FALLING);
    }else
        target->RemoveUnitMovementFlag(MOVEMENTFLAG_FALLING);

    target->SetCanFly(apply);

    Player* player = target->ToPlayer();
    if (!player)
        player = target->m_movedPlayer;

    if (player)
        target->m_anti_FlightTime = 1000;

    //! We still need to initiate a server-side MoveFall here,
    //! which requires MSG_MOVE_FALL_LAND on landing.
    if (target->GetTypeId() == TYPEID_UNIT)
         target->GetMotionMaster()->MoveFall();
}

void AuraEffect::HandleAuraWaterWalk(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    target->SetWaterWalking(apply);
}

void AuraEffect::HandleAuraFeatherFall(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    target->SetFeatherFall(apply);

    // start fall from current height
    if (!apply && target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->SetFallInformation(0, target->GetPositionZ());
}

void AuraEffect::HandleAuraGlide(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    if (apply)
        target->AddExtraUnitMovementFlag(MOVEMENTFLAG2_0x1000);
    else
        target->RemoveExtraUnitMovementFlag(MOVEMENTFLAG2_0x1000);

    target->SendMoveflag2_0x1000_Update(apply);
}

void AuraEffect::HandleAuraHover(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    target->SetHover(apply);    //! Sets movementflags
}

void AuraEffect::HandleWaterBreathing(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    // update timers in client
    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateMirrorTimers();
}

void AuraEffect::HandleForceMoveForward(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FORCE_MOVEMENT);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FORCE_MOVEMENT);
    }
}

/****************************/
/***        THREAT        ***/
/****************************/

void AuraEffect::HandleModThreat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    for (int8 i = 0; i < MAX_SPELL_SCHOOL; ++i)
        if (GetMiscValue() & (1 << i))
            ApplyPercentModFloatVar(target->m_threatModifier[i], float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModTotalThreat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->isAlive() || target->GetTypeId() != TYPEID_PLAYER)
        return;

    Unit* caster = GetCaster();
    if (caster && caster->isAlive())
        target->getHostileRefManager().addTempThreat((float)GetAmount(), apply);
}

void AuraEffect::HandleModTaunt(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->isAlive() || !target->CanHaveThreatList())
        return;

    Unit* caster = GetCaster();
    if (!caster || !caster->isAlive())
        return;

    if (apply)
        target->TauntApply(caster);
    else
    {
        // When taunt aura fades out, mob will switch to previous target if current has less than 1.1 * secondthreat
        target->TauntFadeOut(caster);
    }
}

/*****************************/
/***        CONTROL        ***/
/*****************************/

void AuraEffect::HandleModConfuse(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if(apply)
        target->SetTimeForSpline(GetBase()->GetDuration());
    else
        target->SetTimeForSpline(0);

    target->SetControlled(apply, UNIT_STATE_CONFUSED);
    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_CONFUSE, apply);
}

void AuraEffect::HandleModFear(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if(apply)
        target->SetTimeForSpline(GetBase()->GetDuration());
    else
        target->SetTimeForSpline(0);

    target->SetControlled(apply, UNIT_STATE_FLEEING);
    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_FEAR, apply);
}

void AuraEffect::HandleAuraModStun(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->SetControlled(apply, UNIT_STATE_STUNNED);
    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_STUN, apply);
}

void AuraEffect::HandleAuraModRoot(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    // Earthgrab totem - Immunity
    if (apply && target->HasAura(116946))
        return;

    target->SetControlled(apply, UNIT_STATE_ROOT);
    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_ROOT, apply);
}

void AuraEffect::HandlePreventFleeing(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->HasAuraType(SPELL_AURA_MOD_FEAR))
        target->SetControlled(!(apply), UNIT_STATE_FLEEING);
}

/***************************/
/***        CHARM        ***/
/***************************/

void AuraEffect::HandleModPossess(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    // no support for posession AI yet
    if (caster && caster->GetTypeId() == TYPEID_UNIT)
    {
        HandleModCharm(aurApp, mode, apply);
        return;
    }

    if (apply)
        target->SetCharmedBy(caster, CHARM_TYPE_POSSESS, aurApp);
    else
        target->RemoveCharmedBy(caster);

    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_POSSESS, apply);
}

// only one spell has this aura
void AuraEffect::HandleModPossessPet(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* caster = GetCaster();
    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
        return;

    //seems it may happen that when removing it is no longer owner's pet
    //if (caster->ToPlayer()->GetPet() != target)
    //    return;

    Unit* target = aurApp->GetTarget();
    if (target->GetTypeId() != TYPEID_UNIT || !target->ToCreature()->isPet())
        return;

    Pet* pet = target->ToPet();

    if (apply)
    {
        if (caster->ToPlayer()->GetPet() != pet)
            return;

        pet->SetCharmedBy(caster, CHARM_TYPE_POSSESS, aurApp);
    }
    else
    {
        pet->RemoveCharmedBy(caster);

        if (!pet->IsWithinDistInMap(caster, pet->GetMap()->GetVisibilityRange()))
            pet->Remove();
        else
        {
            // Reinitialize the pet bar and make the pet come back to the owner
            caster->ToPlayer()->PetSpellInitialize();
            if (!pet->getVictim())
            {
                pet->GetMotionMaster()->MoveFollow(caster, PET_FOLLOW_DIST, pet->GetFollowAngle());
            }
        }
    }
}

void AuraEffect::HandleModCharm(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (apply)
        target->SetCharmedBy(caster, CHARM_TYPE_CHARM, aurApp);
    else
        target->RemoveCharmedBy(caster);

    if(GetBase()->GetDuration() > 0)
        target->SendLossOfControl(GetCaster(), GetId(), GetBase()->GetDuration(), GetBase()->GetDuration(), GetSpellInfo()->GetEffectMechanic(GetEffIndex()), SPELL_SCHOOL_MASK_NONE, LOC_CHARM, apply);
}

void AuraEffect::HandleCharmConvert(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (apply)
        target->SetCharmedBy(caster, CHARM_TYPE_CONVERT, aurApp);
    else
        target->RemoveCharmedBy(caster);
}

/**
 * Such auras are applied from a caster(=player) to a vehicle.
 * This has been verified using spell #49256
 */
void AuraEffect::HandleAuraControlVehicle(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->IsVehicle())
        return;

    Unit* caster = GetCaster();

    if (!caster || caster == target)
        return;

    if (apply)
    {
        // Currently spells that have base points  0 and DieSides 0 = "0/0" exception are pushed to -1,
        // however the idea of 0/0 is to ingore flag VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT and -1 checks for it,
        // so this break such spells or most of them.
        // Current formula about m_amount: effect base points + dieside - 1
        // TO DO: Reasearch more about 0/0 and fix it.
        caster->_EnterVehicle(target->GetVehicleKit(), m_amount - 1, aurApp);
    }
    else
    {
        // Remove pending passengers before exiting vehicle - might cause an Uninstall
        target->GetVehicleKit()->RemovePendingEventsForPassenger(caster);

        if (GetId() == 53111) // Devour Humanoid
        {
            target->Kill(caster);
            if (caster->GetTypeId() == TYPEID_UNIT)
                caster->ToCreature()->RemoveCorpse();
        }

        if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT))
            caster->_ExitVehicle();
        else
            target->GetVehicleKit()->RemovePassenger(caster);  // Only remove passenger from vehicle without launching exit movement or despawning the vehicle

        // some SPELL_AURA_CONTROL_VEHICLE auras have a dummy effect on the player - remove them
        caster->RemoveAurasDueToSpell(GetId());
    }
}

/*********************************************************/
/***                  MODIFY SPEED                     ***/
/*********************************************************/
void AuraEffect::HandleAuraModIncreaseSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (GetAuraType() == SPELL_AURA_INCREASE_MIN_SWIM_SPEED)
    {
        target->UpdateSpeed(MOVE_SWIM, true);
        return;
    }

    target->UpdateSpeed(MOVE_RUN, true);

    if (GetAuraType() == SPELL_AURA_MOD_MINIMUM_SPEED)
    {
        target->UpdateSpeed(MOVE_RUN_BACK, true);
        target->UpdateSpeed(MOVE_FLIGHT, true);
    }

    if (Player *player = target->ToPlayer())
        player->SendMovementSetCollisionHeight(player->GetCollisionHeight(true));
}

void AuraEffect::HandleAuraModIncreaseMountedSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    HandleAuraModIncreaseSpeed(aurApp, mode, apply);
}

void AuraEffect::HandleAuraModIncreaseFlightSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();
    if (mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK)
        target->UpdateSpeed(MOVE_FLIGHT, true);

    //! Update ability to fly
    if (GetAuraType() == SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK && (apply || (!target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !target->HasAuraType(SPELL_AURA_FLY))))
        {
            if (target->ToPlayer())
                target->ToPlayer()->SendMovementSetCanTransitionBetweenSwimAndFly(apply);

            target->SetCanFly(apply);

            if (!apply && !target->IsLevitating() && target->GetTypeId() == TYPEID_UNIT)
                target->GetMotionMaster()->MoveFall();
        }

        //! Someone should clean up these hacks and remove it from this function. It doesn't even belong here.
        if (mode & AURA_EFFECT_HANDLE_REAL)
        {
            //Players on flying mounts must be immune to polymorph
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);

            // Dragonmaw Illusion (overwrite mount model, mounted aura already applied)
            if (apply && target->HasAuraEffect(42016, 0) && target->GetMountID())
                target->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, 16314);
        }
    }
}

void AuraEffect::HandleAuraModIncreaseSwimSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_SWIM, true);
}

void AuraEffect::HandleAuraModDecreaseSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_RUN, true);
    target->UpdateSpeed(MOVE_SWIM, true);
    target->UpdateSpeed(MOVE_FLIGHT, true);
    target->UpdateSpeed(MOVE_RUN_BACK, true);
    target->UpdateSpeed(MOVE_SWIM_BACK, true);
    target->UpdateSpeed(MOVE_FLIGHT_BACK, true);
}

void AuraEffect::HandleAuraModUseNormalSpeed(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateSpeed(MOVE_RUN,  true);
    target->UpdateSpeed(MOVE_SWIM, true);
    target->UpdateSpeed(MOVE_FLIGHT,  true);
}

/*********************************************************/
/***                     IMMUNITY                      ***/
/*********************************************************/

void AuraEffect::HandleModStateImmunityMask(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    std::list <AuraType> aura_immunity_list;
    uint32 mechanic_immunity_list = 0;
    int32 miscVal = GetMiscValue();

    switch (miscVal)
    {
        case 96:
        case 1615:
        {
            if (GetAmount())
            {
                mechanic_immunity_list = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                    | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                    | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                    | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                    | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_ROOT, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FEAR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SLEEP, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_CHARM, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SAPPED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_HORROR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_TURN, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
                aura_immunity_list.push_back(SPELL_AURA_MOD_ROOT);
                aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
                aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);
            }
            break;
        }
        case 679:
        case 1921:
        {
            if (GetId() == 57742 || GetId() == 115018)
            {
                mechanic_immunity_list = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                    | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                    | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                    | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                    | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_ROOT, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FEAR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SLEEP, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_CHARM, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SAPPED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_HORROR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_TURN, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
                aura_immunity_list.push_back(SPELL_AURA_MOD_ROOT);
                aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
                aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);
            }
            break;
        }
        case 1557:
        {
            if (GetId() == 64187)
            {
                mechanic_immunity_list = (1 << MECHANIC_STUN);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
            }
            else
            {
                mechanic_immunity_list = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                    | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                    | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                    | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                    | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_ROOT, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FEAR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SLEEP, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_CHARM, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SAPPED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_HORROR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_TURN, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
                aura_immunity_list.push_back(SPELL_AURA_MOD_ROOT);
                aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
                aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);
            }
            break;
        }
        case 1614:
        case 1694:
        {
            target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, apply);
            aura_immunity_list.push_back(SPELL_AURA_MOD_TAUNT);
            break;
        }
        case 1630:
        {
            if (!GetAmount())
            {
                target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_TAUNT);
            }
            else
            {
                mechanic_immunity_list = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                    | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                    | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                    | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                    | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_ROOT, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FEAR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SLEEP, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_CHARM, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SAPPED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_HORROR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_TURN, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
                aura_immunity_list.push_back(SPELL_AURA_MOD_ROOT);
                aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
                aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);
            }
            break;
        }
        case 477:
        case 1733:
        {
            if (!GetAmount())
            {
                mechanic_immunity_list = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT)
                    | (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                    | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                    | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                    | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN)
                    | (1 << MECHANIC_DISARM) | (1 << MECHANIC_BANISH);

                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_ROOT, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FEAR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SLEEP, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_CHARM, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SAPPED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_HORROR, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_TURN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISARM, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_BANISH, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
                aura_immunity_list.push_back(SPELL_AURA_MOD_ROOT);
                aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
                aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DISARM);
            }
            break;
        }
        case 878:
        {
            if (GetAmount() == 1)
            {
                mechanic_immunity_list = (1 << MECHANIC_SNARE) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_DISORIENTED) | (1 << MECHANIC_FREEZE);

                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
            }
            break;
        }
        case 1887:
        {
            // Frost Pillar
            if (GetId() == 51271)
            {
                target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, apply);
                target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, apply);
                miscVal = 0;

                // Glyph of Frost Pillar
                if (target->HasAura(58635))
                {
                    mechanic_immunity_list = (1 << MECHANIC_FEAR) | (1 << MECHANIC_STUN)
                    | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM)
                    | (1 << MECHANIC_SAPPED) | (1 << MECHANIC_HORROR)
                    | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_DISORIENTED)
                    | (1 << MECHANIC_FREEZE) | (1 << MECHANIC_TURN);

                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FEAR, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SLEEP, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_CHARM, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SAPPED, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_HORROR, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_FREEZE, apply);
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_TURN, apply);
                    aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
                    aura_immunity_list.push_back(SPELL_AURA_TRANSFORM);
                    aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
                    aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);

                    // Apply Root !
                    if (apply)
                        target->CastSpell(target, 90259, true);
                    else
                        target->RemoveAura(90259);
                }
            }

            break;
        }
        default:
            break;
    }

    if (aura_immunity_list.empty())
    {
            if (miscVal & (1<<10))
                aura_immunity_list.push_back(SPELL_AURA_MOD_STUN);
            if (miscVal & (1<<1))
                aura_immunity_list.push_back(SPELL_AURA_TRANSFORM);

            // These flag can be recognized wrong:
            if (miscVal & (1<<6))
                aura_immunity_list.push_back(SPELL_AURA_MOD_DECREASE_SPEED);
            if (miscVal & (1<<0))
                aura_immunity_list.push_back(SPELL_AURA_MOD_ROOT);
            if (miscVal & (1<<2))
                aura_immunity_list.push_back(SPELL_AURA_MOD_CONFUSE);
            if (miscVal & (1<<9))
                aura_immunity_list.push_back(SPELL_AURA_MOD_FEAR);
            if (miscVal & (1<<7))
                aura_immunity_list.push_back(SPELL_AURA_MOD_DISARM);
    }

    // apply immunities
    for (std::list <AuraType>::iterator iter = aura_immunity_list.begin(); iter != aura_immunity_list.end(); ++iter)
        target->ApplySpellImmune(GetId(), IMMUNITY_STATE, *iter, apply);

    if (apply && GetSpellInfo()->HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
    {
        target->RemoveAurasWithMechanic(mechanic_immunity_list, AURA_REMOVE_BY_DEFAULT, GetId());
        for (std::list <AuraType>::iterator iter = aura_immunity_list.begin(); iter != aura_immunity_list.end(); ++iter)
            target->RemoveAurasByType(*iter);
    }
}

void AuraEffect::HandleModMechanicImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    uint32 mechanic;

    switch (GetId())
    {
        case 42292: // PvP trinket
        case 59752: // Every Man for Himself
        case 70029: // The Beast Within
            mechanic = IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;
            // Actually we should apply immunities here, too, but the aura has only 100 ms duration, so there is practically no point
            break;
        case 54508: // Demonic Empowerment
            mechanic = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT);
            target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_SNARE, apply);
            target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_ROOT, apply);
            target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_STUN, apply);
            break;
        default:
            if (GetMiscValue() < 1)
                return;
            mechanic = 1 << GetMiscValue();
            target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, GetMiscValue(), apply);
            break;
    }

    if (apply && GetSpellInfo()->HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
        target->RemoveAurasWithMechanic(mechanic, AURA_REMOVE_BY_DEFAULT, GetId());
}

void AuraEffect::HandleAuraModEffectImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

   target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, GetMiscValue(), apply);
}

void AuraEffect::HandleAuraModStateImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->ApplySpellImmune(GetId(), IMMUNITY_STATE, GetMiscValue(), apply);

    if (apply && GetSpellInfo()->HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY))
        target->RemoveAurasByType(AuraType(GetMiscValue()), ObjectGuid::Empty, GetBase());
}

void AuraEffect::HandleAuraModSchoolImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->ApplySpellImmune(GetId(), IMMUNITY_SCHOOL, GetMiscValue(), (apply));

    if (GetSpellInfo()->Categories.Mechanic == MECHANIC_BANISH)
    {
        if (apply)
            target->AddUnitState(UNIT_STATE_ISOLATED);
        else
        {
            bool banishFound = false;
            Unit::AuraEffectList const& banishAuras = target->GetAuraEffectsByType(GetAuraType());
            for (Unit::AuraEffectList::const_iterator i = banishAuras.begin(); i != banishAuras.end(); ++i)
                if ((*i)->GetSpellInfo()->Categories.Mechanic == MECHANIC_BANISH)
                {
                    banishFound = true;
                    break;
                }
            if (!banishFound)
                target->ClearUnitState(UNIT_STATE_ISOLATED);
        }
    }

    uint32 count = 0;
    if (apply && GetMiscValue() == SPELL_SCHOOL_MASK_NORMAL)
        count += target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);

    // remove all flag auras (they are positive, but they must be removed when you are immune)
    if (GetSpellInfo()->HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY)
        && GetSpellInfo()->HasAttribute(SPELL_ATTR2_DAMAGE_REDUCED_SHIELD))
        count += target->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);

    // TODO: optimalize this cycle - use RemoveAurasWithInterruptFlags call or something else
    if ((apply)
        && GetSpellInfo()->HasAttribute(SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY)
        && GetSpellInfo()->IsPositive())                       //Only positive immunity removes auras
    {
        uint32 school_mask = GetMiscValue();
        Unit::AuraApplicationMap& Auras = target->GetAppliedAuras();
        for (Unit::AuraApplicationMap::iterator iter = Auras.begin(); iter != Auras.end();)
        {
            SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
            if ((spell->GetSchoolMask() & school_mask)//Check for school mask
                && GetSpellInfo()->CanDispelAura(spell)
                && !iter->second->IsPositive()          //Don't remove positive spells
                && spell->Id != GetId())               //Don't remove self
            {
                target->RemoveAura(iter);
                count++;
            }
            else
                ++iter;
        }
    }

    if(Spell* modSpell = target->FindCurrentSpellBySpellId(GetId()))
        modSpell->m_count_dispeling += count;
}

void AuraEffect::HandleAuraModDmgImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->ApplySpellImmune(GetId(), IMMUNITY_DAMAGE, GetMiscValue(), apply);
}

void AuraEffect::HandleAuraModDispelImmunity(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    target->ApplySpellDispelImmunity(m_spellInfo, DispelType(GetMiscValue()), (apply));
}

/*********************************************************/
/***                  MODIFY STATS                     ***/
/*********************************************************/

/********************************/
/***        RESISTANCE        ***/
/********************************/

void AuraEffect::HandleAuraModResistanceExclusive(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    for (int8 x = SPELL_SCHOOL_NORMAL; x < MAX_SPELL_SCHOOL; x++)
    {
        if (GetMiscValue() & int32(1<<x))
        {
            int32 amount = target->GetMaxPositiveAuraModifierByMiscMask(SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE, 1<<x, this);
            if (amount < GetAmount())
            {
                float value = float(GetAmount() - amount);
                target->HandleStatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), BASE_VALUE, value, apply);
                if (target->GetTypeId() == TYPEID_PLAYER)
                    target->ApplyResistanceBuffModsMod(SpellSchools(x), aurApp->IsPositive(), value, apply);
            }
        }
    }
}

void AuraEffect::HandleAuraModResistance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    for (int8 x = SPELL_SCHOOL_NORMAL; x < MAX_SPELL_SCHOOL; x++)
    {
        if (GetMiscValue() & int32(1<<x))
        {
            target->HandleStatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), TOTAL_VALUE, float(GetAmount()), apply);
            if (target->GetTypeId() == TYPEID_PLAYER || target->ToCreature()->isPet())
                target->ApplyResistanceBuffModsMod(SpellSchools(x), GetAmount() > 0, (float)GetAmount(), apply);
        }
    }
}

void AuraEffect::HandleAuraModBaseResistancePCT(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // only players have base stats
    if (target->GetTypeId() != TYPEID_PLAYER)
    {
        //pets only have base armor
        if (target->ToCreature()->isPet() && (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL))
            target->HandleStatModifier(UNIT_MOD_ARMOR, BASE_PCT, float(GetAmount()), apply);
    }
    else
    {
        for (int8 x = SPELL_SCHOOL_NORMAL; x < MAX_SPELL_SCHOOL; x++)
        {
            if (GetMiscValue() & int32(1<<x))
            {
                switch (GetMiscValue())
                {
                    case 1:
                        target->UpdateArmor();
                        break; 
                    default:
                        target->HandleStatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + x), BASE_PCT, float(GetAmount()), apply);
                        break;
                }
            }
        }
    }
}

void AuraEffect::HandleModResistancePercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    for (int8 i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; i++)
    {
        if (GetMiscValue() & int32(1<<i))
        {
            switch (GetMiscValue())
            {
                case 1:
                   target->UpdateArmor();
                   break; 
                default:
                {
                    target->HandleStatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + i), TOTAL_PCT, float(GetAmount()), apply);
                    if (target->GetTypeId() == TYPEID_PLAYER || target->ToCreature()->isPet())
                    {
                        target->ApplyResistanceBuffModsPercentMod(SpellSchools(i), true, (float)GetAmount(), apply);
                        target->ApplyResistanceBuffModsPercentMod(SpellSchools(i), false, (float)GetAmount(), apply);
                    }
                    break;
                }
            }
        }
    }
}

void AuraEffect::HandleModBaseResistance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // only players have base stats
    if (target->GetTypeId() != TYPEID_PLAYER)
    {
        //only pets have base stats
        if (target->ToCreature()->isPet() && (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL))
            target->HandleStatModifier(UNIT_MOD_ARMOR, TOTAL_VALUE, float(GetAmount()), apply);
    }
    else
    {
        for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; i++)
            if (GetMiscValue() & (1<<i))
                target->HandleStatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + i), TOTAL_VALUE, float(GetAmount()), apply);
    }
}

void AuraEffect::HandleModTargetResistance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // applied to damage as HandleNoImmediateEffect in Unit::CalcAbsorbResist and Unit::CalcArmorReducedDamage

    // show armor penetration
    if (target->GetTypeId() == TYPEID_PLAYER && (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL))
        target->ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE, GetAmount(), apply);

    // show as spell penetration only full spell penetration bonuses (all resistances except armor and holy
    if (target->GetTypeId() == TYPEID_PLAYER && (GetMiscValue() & SPELL_SCHOOL_MASK_SPELL) == SPELL_SCHOOL_MASK_SPELL)
        target->ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, GetAmount(), apply);
}

/********************************/
/***           STAT           ***/
/********************************/

void AuraEffect::HandleAuraModStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (GetMiscValue() < -2 || GetMiscValue() > 4)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "WARNING: Spell %u effect %u has an unsupported misc value (%i) for SPELL_AURA_MOD_STAT ", GetId(), GetEffIndex(), GetMiscValue());
        return;
    }

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; i++)
    {
        // -1 or -2 is all stats (misc < -2 checked in function beginning)
        if (GetMiscValue() < 0 || GetMiscValue() == i)
        {
            //target->ApplyStatMod(Stats(i), m_amount, apply);
            target->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float(GetAmount()), apply);
            if (target->GetTypeId() == TYPEID_PLAYER || target->ToCreature()->isPet())
                target->ApplyStatBuffMod(Stats(i), (float)GetAmount(), apply);
        }
    }
}

void AuraEffect::HandleModPercentStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (GetMiscValue() < -1 || GetMiscValue() > 4)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "WARNING: Misc Value for SPELL_AURA_MOD_PERCENT_STAT not valid");
        return;
    }

    // only players have base stats
    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        if (GetMiscValue() == i || GetMiscValue() == -1)
            target->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), BASE_PCT, float(m_amount), apply);
    }
}

void AuraEffect::HandleModSpellDamagePercentFromStat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Magic damage modifiers implemented in Unit::SpellDamageBonus
    // This information for client side use only
    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModSpellHealingPercentFromStat(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleAuraModSpellPowerPercent(AuraApplication const * aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit *target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModSpellDamagePercentFromAttackPower(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Magic damage modifiers implemented in Unit::SpellDamageBonus
    // This information for client side use only
    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModSpellHealingPercentFromAttackPower(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Recalculate bonus
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModHealingDone(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;
    // implemented in Unit::SpellHealingBonus
    // this information is for client side only
    target->ToPlayer()->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleModTotalPercentStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // save current health state
    float healthPct = target->GetHealthPct();

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; i++)
    {
        if (GetMiscValueB() & 1 << i || !GetMiscValueB()) // 0 is also used for all stats
        {
            target->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(GetAmount()), apply);
            if (target->GetTypeId() == TYPEID_PLAYER || target->ToCreature()->isPet())
                target->ApplyStatPercentBuffMod(Stats(i), float(GetAmount()), apply);
        }
    }

    // recalculate current HP/MP after applying aura modifications (only for spells with SPELL_ATTR0_UNK4 0x00000010 flag)
    // this check is total bullshit i think
    if (target->isAlive())
    {
        if (GetMiscValueB() & 1 << STAT_STAMINA && (m_spellInfo->HasAttribute(SPELL_ATTR0_ABILITY)))
            target->SetHealth(std::max<uint32>(uint32(healthPct * target->GetMaxHealth() * 0.01f), 1));
    }
}

void AuraEffect::HandleAuraModResistenceOfStatPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (GetMiscValue() != SPELL_SCHOOL_MASK_NORMAL)
    {
        // support required adding replace UpdateArmor by loop by UpdateResistence at intellect update
        // and include in UpdateResistence same code as in UpdateArmor for aura mod apply.
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "Aura SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT(182) does not work for non-armor type resistances!");
        return;
    }

    // Recalculate Armor
    target->UpdateArmor();
}

/********************************/
/***      HEAL & ENERGIZE     ***/
/********************************/
void AuraEffect::HandleModPowerRegen(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    switch (GetMiscValue())
    {
        case POWER_MANA:
            target->UpdateManaRegen();
            break;
        default:
            break;
    }
}

void AuraEffect::HandleModPowerRegenPCT(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    switch (GetMiscValue())
    {
        case POWER_MANA:
            target->UpdateManaRegen();
            break;
        case POWER_RUNES:
            if (target->GetTypeId() == TYPEID_PLAYER)
                target->ToPlayer()->SetNeedToUpdateRunesRegen();
            break;
        default:
            target->UpdatePowerRegen(GetMiscValue());
            break;
    }

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleModPowerRegenPCT mode %i, apply %i, GetAmount %i", mode, apply, GetAmount());
}

void AuraEffect::HandleModManaRegen(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateManaRegen();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    //Note: an increase in regen does NOT cause threat.
    target->ToPlayer()->SetNeedToUpdateRunesRegen();
}

void AuraEffect::HandleAuraModIncreaseHealth(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    int32 getHealth = target->GetHealth();
    int32 getMaxHealth = target->GetMaxHealth();
    int32 amount = GetAmount();

    if(target->isPet())
    {
        if(target != GetCaster())
            return;
    }

    if (apply)
    {
        target->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_VALUE, float(amount), apply);
        target->ModifyHealth(amount);
    }
    else
    {
        float pct = 0;
        if (!(m_spellInfo->HasAttribute(SPELL_ATTR0_DONT_AFFECT_SHEATH_STATE)))
        {
            if (getHealth > amount)
                target->ModifyHealth(-amount);
            else
                target->SetHealth(1);
        }
        else if (m_spellInfo->HasAttribute(SPELL_ATTR3_NO_DONE_BONUS))
            pct = float(getMaxHealth) / float(getHealth);

        target->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_VALUE, float(amount), apply);

        if (pct)
        {
            target->SetHealth(target->GetMaxHealth() / pct);
        }
    }
}

void AuraEffect::HandleAuraModIncreaseMaxHealth(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if(target->isPet())
    {
        if(target != GetCaster())
            return;
    }

    uint32 oldhealth = target->GetHealth();
    double healthPercentage = (double)oldhealth / (double)target->GetMaxHealth();

    target->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_VALUE, float(GetAmount()), apply);

    // refresh percentage
    if (oldhealth > 0)
    {
        uint32 newhealth = uint32(ceil((double)target->GetMaxHealth() * healthPercentage));
        if (newhealth == 0)
            newhealth = 1;

        target->SetHealth(newhealth);
    }
}

void AuraEffect::HandleAuraModIncreaseEnergy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    Powers powerType = Powers(GetMiscValue());
    // do not check power type, we can always modify the maximum
    // as the client will not see any difference
    // also, placing conditions that may change during the aura duration
    // inside effect handlers is not a good idea
    //if (int32(powerType) != GetMiscValue())
    //    return;

    UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + powerType);

    target->HandleStatModifier(unitMod, TOTAL_VALUE, float(GetAmount()), apply);
    target->UpdateMaxPower(powerType);
}

void AuraEffect::HandleAuraModIncreaseEnergyPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    Powers powerType = Powers(GetMiscValue());
    // do not check power type, we can always modify the maximum
    // as the client will not see any difference
    // also, placing conditions that may change during the aura duration
    // inside effect handlers is not a good idea
    //if (int32(powerType) != GetMiscValue())
    //    return;

    UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + powerType);
    float amount = float(GetAmount());

    if (apply)
    {
        target->HandleStatModifier(unitMod, TOTAL_PCT, amount, apply);
        target->ModifyPowerPct(powerType, amount, apply);
    }
    else
    {
        target->ModifyPowerPct(powerType, amount, apply);
        target->HandleStatModifier(unitMod, TOTAL_PCT, amount, apply);
    }
}

void AuraEffect::HandleAuraModMaxPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    if (target)
        target->HandleStatModifier(UnitMods(UNIT_MOD_POWER_START + Powers(GetMiscValue())), TOTAL_VALUE, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModAddEnergyPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    if (target)
        target->HandleStatModifier(UnitMods(UNIT_MOD_POWER_START + Powers(GetMiscValue())), TOTAL_PCT, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModIncreaseHealthPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if(target->isPet())
    {
        if(target != GetCaster())
            return;
    }
    // Unit will keep hp% after MaxHealth being modified if unit is alive.
    float percent = target->GetHealthPct();

    target->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, float(GetAmount()), apply);

    if (target->isAlive())
        target->SetHealth(target->CountPctFromMaxHealth(int32(percent)));
}

void AuraEffect::HandleAuraModPetStatsModifier(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    if(Unit* target = aurApp->GetTarget())
    {
        Guardian* pet = NULL;
        if (Player* _player = target->ToPlayer())
            pet = _player->GetGuardianPet();

        if(!pet)
            pet = target->ToPet();

        if (pet)
        {
            if(GetMiscValue() == PETSPELLMOD_ARMOR)
                pet->UpdateArmor();
            if(GetMiscValue() == PETSPELLMOD_MAX_HP)
            {
                if(apply)
                    pet->ModifyHealth(GetAmount());
                pet->UpdateMaxHealth();
            }
        }
    }
}

void AuraEffect::HandleAuraIncreaseBaseHealthPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if(target->isPet())
    {
        if(target != GetCaster())
            return;
    }

    target->HandleStatModifier(UNIT_MOD_HEALTH, BASE_PCT, float(GetAmount()), apply);
}

/********************************/
/***          FIGHT           ***/
/********************************/

void AuraEffect::HandleAuraModParryPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateParryPercentage();
}

void AuraEffect::HandleAuraModDodgePercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateDodgePercentage();
}

void AuraEffect::HandleAuraModBlockPercent(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    target->ToPlayer()->UpdateBlockPercentage();
}

void AuraEffect::HandleAuraModRegenInterrupt(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    HandleModManaRegen(aurApp, mode, apply);
}

void AuraEffect::HandleAuraModWeaponCritPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

//     for (int i = 0; i < MAX_ATTACK; ++i)
//         if (Item* pItem = target->ToPlayer()->GetWeaponForAttack(WeaponAttackType(i), true))
//             target->ToPlayer()->_ApplyWeaponDependentAuraCritMod(pItem, WeaponAttackType(i), this, apply);

    // mods must be applied base at equipped weapon class and subclass comparison
    // with spell->EquippedItemClass and  EquippedItemSubClassMask and EquippedItemInventoryTypeMask
    // GetMiscValue() comparison with item generated damage types

    if (GetSpellInfo()->EquippedItemClass == -1)
    {
        target->ToPlayer()->HandleBaseModValue(CRIT_PERCENTAGE,         FLAT_MOD, float (GetAmount()), apply);
        target->ToPlayer()->HandleBaseModValue(OFFHAND_CRIT_PERCENTAGE, FLAT_MOD, float (GetAmount()), apply);
        target->ToPlayer()->HandleBaseModValue(RANGED_CRIT_PERCENTAGE,  FLAT_MOD, float (GetAmount()), apply);
    }
    else
    {
        // done in Player::_ApplyWeaponDependentAuraMods
    }
}

void AuraEffect::HandleModHitChance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        target->ToPlayer()->UpdateMeleeHitChances();
        target->ToPlayer()->UpdateRangedHitChances();
    }
    else
    {
        target->m_modMeleeHitChance += (apply) ? GetAmount() : (-GetAmount());
        target->m_modRangedHitChance += (apply) ? GetAmount() : (-GetAmount());
    }
}

void AuraEffect::HandleModSpellHitChance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateSpellHitChances();
    else
        target->m_modSpellHitChance += (apply) ? GetAmount(): (-GetAmount());
}

void AuraEffect::HandleModSpellCritChance(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateAllSpellCritChances();
    else
        target->m_baseSpellCritChance += (apply) ? GetAmount():-GetAmount();
}

void AuraEffect::HandleModSpellCritChanceShool(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (int school = SPELL_SCHOOL_NORMAL; school < MAX_SPELL_SCHOOL; ++school)
        if (GetMiscValue() & (1<<school))
            target->ToPlayer()->UpdateSpellCritChance(school);
}

void AuraEffect::HandleAuraModCritPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
    {
        target->m_baseSpellCritChance += (apply) ? GetAmount():-GetAmount();
        return;
    }

    target->ToPlayer()->HandleBaseModValue(CRIT_PERCENTAGE,         FLAT_MOD, float (GetAmount()), apply);
    target->ToPlayer()->HandleBaseModValue(OFFHAND_CRIT_PERCENTAGE, FLAT_MOD, float (GetAmount()), apply);
    target->ToPlayer()->HandleBaseModValue(RANGED_CRIT_PERCENTAGE,  FLAT_MOD, float (GetAmount()), apply);

    // included in Player::UpdateSpellCritChance calculation
    target->ToPlayer()->UpdateAllSpellCritChances();
}

void AuraEffect::HandleAuraModResiliencePct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* _player = target->ToPlayer();

    float baseValue = _player->GetFloatValue(PLAYER_FIELD_MOD_RESILIENCE_PERCENT);

    if (apply)
        baseValue += GetAmount() / 100.0f;
    else
        baseValue -= GetAmount() / 100.0f;

    _player->SetFloatValue(PLAYER_FIELD_MOD_RESILIENCE_PERCENT, baseValue);
}

/********************************/
/***         ATTACK SPEED     ***/
/********************************/

void AuraEffect::HandleModCastingSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (Player* plr = target->ToPlayer())
    {
        plr->SetNeedUpdateHastMod();
        plr->SetNeedUpdateCastHastMods();
    }
    else
    {
        target->UpdateHastMod();
        target->UpdateCastHastMods();
    }
}

void AuraEffect::HandleModMeleeRangedSpeedPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    //! ToDo: Haste auras with the same handler _CAN'T_ stack together
    Unit* target = aurApp->GetTarget();

    if (Player* plr = target->ToPlayer())
    {
        plr->SetNeedUpdateMeleeHastMod();
        plr->SetNeedUpdateRangeHastMod();
        plr->SetNeedUpdateCastHastMods();
    }
    else
    {
        target->UpdateMeleeHastMod();
        target->UpdateRangeHastMod();
        target->UpdateCastHastMods();
    }
}

void AuraEffect::HandleModCombatSpeedPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    
    if (Player* plr = target->ToPlayer())
    {
        plr->SetNeedUpdateMeleeHastMod();
        plr->SetNeedUpdateRangeHastMod();
        plr->SetNeedUpdateHastMod();
        plr->SetNeedUpdateCastHastMods();
    }
    else
    {
        target->UpdateHastMod();
        target->UpdateMeleeHastMod();
        target->UpdateRangeHastMod();
        target->UpdateCastHastMods();
    }
}

void AuraEffect::HandleModAttackSpeed(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    float hastMod = target->GetFloatValue(UNIT_FIELD_MOD_HASTE);
    ApplyPercentModFloatVar(hastMod, (float)GetAmount(), !apply);

    target->CalcAttackTimePercentMod(BASE_ATTACK, hastMod);
    target->UpdateDamagePhysical(BASE_ATTACK);
}

void AuraEffect::HandleModMeleeSpeedPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    //! ToDo: Haste auras with the same handler _CAN'T_ stack together
    Unit* target = aurApp->GetTarget();

    int32 value = GetAmount();

    if (Player* plr = target->ToPlayer())
    {
        plr->SetNeedUpdateMeleeHastMod();
    }
    else
    {
        target->UpdateMeleeHastMod();
    }
}

void AuraEffect::HandleAuraModRangedHaste(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    //! ToDo: Haste auras with the same handler _CAN'T_ stack together
    Unit* target = aurApp->GetTarget();

    if (Player* plr = target->ToPlayer())
    {
        plr->SetNeedUpdateRangeHastMod();
    }
    else
    {
        target->UpdateRangeHastMod();
    }
}

/********************************/
/***       COMBAT RATING      ***/
/********************************/

void AuraEffect::HandleModRating(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (uint32 rating = 0; rating < MAX_COMBAT_RATING; ++rating)
        if (GetMiscValue() & (1 << rating))
            target->ToPlayer()->UpdateRating(CombatRating(rating));
}

void AuraEffect::HandleModRatingFromStat(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Just recalculate ratings
    for (uint32 rating = 0; rating < MAX_COMBAT_RATING; ++rating)
        if (GetMiscValue() & (1 << rating))
            target->ToPlayer()->ApplyRatingMod(CombatRating(rating), 0, apply);
}

void AuraEffect::HandleConverCritRatingPctToParryRating(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // Just recalculate ratings
    target->ToPlayer()->UpdateParryPercentage();
}
/********************************/
/***        ATTACK POWER      ***/
/********************************/

void AuraEffect::HandleAuraModAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateAttackPowerAndDamage();
    //target->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModRangedAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if ((target->getClassMask() & CLASSMASK_WAND_USERS) != 0)
        return;

    target->UpdateAttackPowerAndDamage(true);
    //target->HandleStatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, float(GetAmount()), apply);
}

void AuraEffect::HandleAuraModAttackPowerPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // Don't apply Markmanship aura twice on pet
    if (GetCaster() && GetCaster()->ToPlayer() && aurApp->GetBase()->GetId() == 19506)
        if (Pet* pet = GetCaster()->ToPlayer()->GetPet())
            if (target->GetGUID() == pet->GetGUID())
                return;

    if (target->ToPlayer())
        target->ToPlayer()->UpdateAttackPowerAndDamage(false);
    else if (Pet* pet = target->ToPet())
        pet->UpdateAttackPowerAndDamage(false);
}

void AuraEffect::HandleAuraModRangedAttackPowerPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    if ((target->getClassMask() & CLASSMASK_WAND_USERS) != 0)
        return;

    if (target->ToPlayer())
        target->ToPlayer()->UpdateAttackPowerAndDamage(true);
}

void AuraEffect::HandleAuraModAttackPowerOfArmor(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // Recalculate bonus
    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateAttackPowerAndDamage(false);
}

void AuraEffect::HandleOverrideAttackPowerBySpellPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // Recalculate bonus
    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateAttackPowerAndDamage(false);
}

void AuraEffect::HandleOverrideSpellPowerByAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

   target->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleIncreaseModRatingPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target)
        return;

    if (Player* player = target->ToPlayer())
        for (uint8 cr = 0; cr < MAX_COMBAT_RATING; ++cr)
            if (GetMiscValue() & (1 << cr))
                player->RecalculateRating(CombatRating(cr));
}

void AuraEffect::HandleModStatBonusPercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    int32 miscValue = GetMiscValue();

    if (miscValue < -1 || miscValue > 4)
        return;

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        if (miscValue == i || miscValue == -1)
        {
            target->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), BASE_PCT_EXCLUDE_CREATE, float(m_amount), apply);
            target->ApplyStatPercentBuffMod(Stats(i), float(m_amount), apply);
        }
    }
}

void AuraEffect::HandleAttackPowerByBonusArmorPct(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // Recalculate bonus
    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->UpdateAttackPowerAndDamage(false);
}

/********************************/
/***        DAMAGE BONUS      ***/
/********************************/
void AuraEffect::HandleModDamageDone(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    // apply item specific bonuses for already equipped weapon
//     if (target->GetTypeId() == TYPEID_PLAYER)
//     {
//         for (int i = 0; i < MAX_ATTACK; ++i)
//             if (Item* pItem = target->ToPlayer()->GetWeaponForAttack(WeaponAttackType(i), true))
//                 target->ToPlayer()->_ApplyWeaponDependentAuraDamageMod(pItem, WeaponAttackType(i), this, apply);
//     }

    // GetMiscValue() is bitmask of spell schools
    // 1 (0-bit) - normal school damage (SPELL_SCHOOL_MASK_NORMAL)
    // 126 - full bitmask all magic damages (SPELL_SCHOOL_MASK_MAGIC) including wands
    // 127 - full bitmask any damages
    //
    // mods must be applied base at equipped weapon class and subclass comparison
    // with spell->EquippedItemClass and  EquippedItemSubClassMask and EquippedItemInventoryTypeMask
    // GetMiscValue() comparison with item generated damage types

    if ((GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL) != 0)
    {
        // apply generic physical damage bonuses including wand case
        if (GetSpellInfo()->EquippedItemClass == -1 || target->GetTypeId() != TYPEID_PLAYER)
        {
            target->HandleStatModifier(UNIT_MOD_DAMAGE_MAINHAND, TOTAL_VALUE, float(GetAmount()), apply);
            target->HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_VALUE, float(GetAmount()), apply);
            target->HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_VALUE, float(GetAmount()), apply);

            if (target->GetTypeId() == TYPEID_PLAYER)
            {
                if (GetAmount() > 0)
                    target->ApplyModUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS, GetAmount(), apply);
                else
                    target->ApplyModUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG, GetAmount(), apply);
            }
        }
        else
        {
            // done in Player::_ApplyWeaponDependentAuraMods
        }
    }

    // Skip non magic case for speedup
    if ((GetMiscValue() & SPELL_SCHOOL_MASK_MAGIC) == 0)
        return;

    if (GetSpellInfo()->EquippedItemClass != -1 || (GetSpellInfo()->EquippedItemInventoryTypeMask != 0 && GetSpellInfo()->EquippedItemInventoryTypeMask != -1))
    {
        // wand magic case (skip generic to all item spell bonuses)
        // done in Player::_ApplyWeaponDependentAuraMods

        // Skip item specific requirements for not wand magic damage
        return;
    }

    // Magic damage modifiers implemented in Unit::SpellDamageBonus
    // This information for client side use only
    if (target->GetTypeId() == TYPEID_PLAYER)
    {
        target->ToPlayer()->ApplySpellPowerBonus(GetAmount(), apply);

        if (Guardian* pet = target->ToPlayer()->GetGuardianPet())
            pet->UpdateAttackPowerAndDamage();
    }
}

void AuraEffect::HandleModDamagePercentDone(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target)
        return;

    float amount = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);

    if (GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
    {
        for (int i = 0; i < MAX_ATTACK; ++i)
            target->UpdateDamagePhysical(WeaponAttackType(i));

        if (target->GetTypeId() == TYPEID_PLAYER)
            target->ToPlayer()->SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PERCENT, amount);
    }
}

void AuraEffect::HandleModOffhandDamagePercent(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    target->UpdateDamagePhysical(OFF_ATTACK);
}

void AuraEffect::HandleShieldBlockValue(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Unit* target = aurApp->GetTarget();

    BaseModType modType = FLAT_MOD;
    if (GetAuraType() == SPELL_AURA_MOD_SHIELD_BLOCKVALUE_PCT)
        modType = PCT_MOD;

    if (target->GetTypeId() == TYPEID_PLAYER)
        target->ToPlayer()->HandleBaseModValue(SHIELD_BLOCK_VALUE, modType, float(GetAmount()), apply);
}

/********************************/
/***        POWER COST        ***/
/********************************/

void AuraEffect::HandleModPowerCostPCT(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    float amount = CalculatePct(1.0f, GetAmount());
    for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
        if (GetMiscValue() & (1 << i))
            target->ApplyModSignedFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + i, amount, apply);

    // Preparation
    // This allows changind spec while in battleground
    if (GetId() == SPELL_BG_PREPARATION)
        target->ModifyAuraState(AURA_STATE_UNKNOWN20, apply);
}

void AuraEffect::HandleModPowerCost(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
        if (GetMiscValue() & (1<<i))
            target->ApplyModInt32Value(UNIT_FIELD_POWER_COST_MODIFIER+i, GetAmount(), apply);
}

void AuraEffect::HandleArenaPreparation(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION);
    }

    // Preparation
    // This allows changind spec while in battleground
    if (apply || !target->HasAuraType(SPELL_AURA_ARENA_PREPARATION))
        target->ModifyAuraState(AURA_STATE_UNKNOWN20, apply);
}

void AuraEffect::HandleNoReagentUseAura(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    flag128 mask;
    Unit::AuraEffectList const& noReagent = target->GetAuraEffectsByType(SPELL_AURA_NO_REAGENT_USE);
        for (Unit::AuraEffectList::const_iterator i = noReagent.begin(); i != noReagent.end(); ++i)
            mask |= (*i)->m_spellInfo->Effects[(*i)->m_effIndex].SpellClassMask;

    target->SetUInt32Value(PLAYER_FIELD_NO_REAGENT_COST_MASK  , mask[0]);
    target->SetUInt32Value(PLAYER_FIELD_NO_REAGENT_COST_MASK+1, mask[1]);
    target->SetUInt32Value(PLAYER_FIELD_NO_REAGENT_COST_MASK+2, mask[2]);
    target->SetUInt32Value(PLAYER_FIELD_NO_REAGENT_COST_MASK+3, mask[3]);
}

void AuraEffect::HandleAuraRetainComboPoints(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    // combo points was added in SPELL_EFFECT_ADD_COMBO_POINTS handler
    // remove only if aura expire by time (in case combo points amount change aura removed without combo points lost)
    //if (!(apply) && GetBase()->GetDuration() == 0 && target->ToPlayer()->GetComboTarget())
    //    if (Unit* unit = ObjectAccessor::GetUnit(*target, target->ToPlayer()->GetComboTarget()))
    //        target->ToPlayer()->AddComboPoints(unit, -GetAmount());

    //! Now no check target?
    if (!(apply) && GetBase()->GetDuration() == 0)
        target->ToPlayer()->AddComboPoints(NULL, -GetAmount());
}

/*********************************************************/
/***                    OTHERS                         ***/
/*********************************************************/

void AuraEffect::HandleAuraDummy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_REAPPLY)))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (mode & (AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY))
    {
        // AT APPLY
        if (apply)
        {
            switch (GetId())
            {
                case 154010:
                    caster->CastSpell(caster, GetAmount(), true);
                    break;
                case 1515:                                      // Tame beast
                    // FIX_ME: this is 2.0.12 threat effect replaced in 2.1.x by dummy aura, must be checked for correctness
                    if (caster && target->CanHaveThreatList())
                        target->AddThreat(caster, 10.0f);
                    break;
                case 13139:                                     // net-o-matic
                    // root to self part of (root_target->charge->root_self sequence
                    if (caster)
                        caster->CastSpell(caster, 13138, true, NULL, this);
                    break;
                case 37096:                                     // Blood Elf Illusion
                {
                    if (caster)
                    {
                        switch (caster->getGender())
                        {
                            case GENDER_FEMALE:
                                caster->CastSpell(target, 37095, true, NULL, this); // Blood Elf Disguise
                                break;
                            case GENDER_MALE:
                                caster->CastSpell(target, 37093, true, NULL, this);
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                }
                case 39850:                                     // Rocket Blast
                    if (roll_chance_i(20))                       // backfire stun
                        target->CastSpell(target, 51581, true, NULL, this);
                    break;
                case 43873:                                     // Headless Horseman Laugh
                    target->PlayDistanceSound(11965);
                    break;
                case 46354:                                     // Blood Elf Illusion
                    if (caster)
                    {
                        switch (caster->getGender())
                        {
                            case GENDER_FEMALE:
                                caster->CastSpell(target, 46356, true, NULL, this);
                                break;
                            case GENDER_MALE:
                                caster->CastSpell(target, 46355, true, NULL, this);
                                break;
                        }
                    }
                    break;
                case 46361:                                     // Reinforced Net
                    if (caster)
                        target->GetMotionMaster()->MoveFall();
                    break;
                case 71563:
                    {
                        Aura* newAura = target->AddAura(71564, target);
                        if (newAura != NULL)
                            newAura->SetStackAmount(newAura->GetSpellInfo()->AuraOptions.CumulativeAura);
                    }
                    break;
                case 59628: // Tricks of the Trade
                    if (caster && caster->GetMisdirectionTarget())
                        target->SetReducedThreatPercent(100, caster->GetMisdirectionTarget()->GetGUID());
                    break;
            }
        }
        // AT REMOVE
        else
        {
            if ((GetSpellInfo()->IsQuestTame()) && caster && caster->isAlive() && target->isAlive())
            {
                uint32 finalSpelId = 0;
                switch (GetId())
                {
                    case 19548: finalSpelId = 19597; break;
                    case 19674: finalSpelId = 19677; break;
                    case 19687: finalSpelId = 19676; break;
                    case 19688: finalSpelId = 19678; break;
                    case 19689: finalSpelId = 19679; break;
                    case 19692: finalSpelId = 19680; break;
                    case 19693: finalSpelId = 19684; break;
                    case 19694: finalSpelId = 19681; break;
                    case 19696: finalSpelId = 19682; break;
                    case 19697: finalSpelId = 19683; break;
                    case 19699: finalSpelId = 19685; break;
                    case 19700: finalSpelId = 19686; break;
                    case 30646: finalSpelId = 30647; break;
                    case 30653: finalSpelId = 30648; break;
                    case 30654: finalSpelId = 30652; break;
                    case 30099: finalSpelId = 30100; break;
                    case 30102: finalSpelId = 30103; break;
                    case 30105: finalSpelId = 30104; break;
                }

                if (finalSpelId)
                    caster->CastSpell(target, finalSpelId, true, NULL, this);
            }

            switch (m_spellInfo->ClassOptions.SpellClassSet)
            {
                case SPELLFAMILY_GENERIC:
                    switch (GetId())
                    {
                        case 161883: //Incinerating Breath
                            if (Creature* caster = target->ToCreature())
                                caster->SetReactState(REACT_AGGRESSIVE);
                            break;
                        case SPELL_BG_WAITING_FOR_RESURRECT:
                            // Waiting to resurrect spell cancel, we must remove player from resurrect queue
                            if (target->GetTypeId() == TYPEID_PLAYER)
                            {
                                if (Battleground* bg = target->ToPlayer()->GetBattleground())
                                    bg->RemovePlayerFromResurrectQueue(target->GetGUID());
                                if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(target->GetZoneId()))
                                    bf->RemovePlayerFromResurrectQueue(target->GetGUID());
                            }
                            break;
                        case 36730:                                     // Flame Strike
                        {
                            target->CastSpell(target, 36731, true, NULL, this);
                            break;
                        }
                        case 44191:                                     // Flame Strike
                        {
                            if (target->GetMap()->IsDungeon())
                            {
                                uint32 spellId = target->GetMap()->IsHeroic() ? 46163 : 44190;

                                target->CastSpell(target, spellId, true, NULL, this);
                            }
                            break;
                        }
                        case 42783: // Wrath of the Astromancer
                            target->CastSpell(target, GetAmount(), true, NULL, this);
                            break;
                        case 46308: // Burning Winds casted only at creatures at spawn
                            target->CastSpell(target, 47287, true, NULL, this);
                            break;
                        case 52172:  // Coyote Spirit Despawn Aura
                        case 60244:  // Blood Parrot Despawn Aura
                            target->CastSpell((Unit*)NULL, GetAmount(), true, NULL, this);
                            break;
                        case 58600: // Restricted Flight Area
                        case 83100: // Restricted Flight Area
                        case 91604: // Restricted Flight Area
                            if (aurApp->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                                target->CastSpell(target, 58601, true);
                            break;
                    }
                    break;
                case SPELLFAMILY_DEATHKNIGHT:
                    // Summon Gargoyle (Dismiss Gargoyle at remove)
                    if (GetId() == 61777)
                        target->CastSpell(target, GetAmount(), true);
                    break;
				case SPELLFAMILY_ROGUE:
					//  Tricks of the trade
                    switch (GetId())
                    {
                        case 59628: //Tricks of the trade buff on rogue (6sec duration)
                            target->SetReducedThreatPercent(0, ObjectGuid::Empty);
                            break;
                        case 57934: //Tricks of the trade buff on rogue (30sec duration)
                            if (aurApp->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE || !caster->GetMisdirectionTarget())
                                target->SetReducedThreatPercent(0, ObjectGuid::Empty);
                            else
                                target->SetReducedThreatPercent(0,caster->GetMisdirectionTarget()->GetGUID());
                            break;
                    }
                    break;
				default:
					break;
			}
		}
    }

    // AT APPLY & REMOVE

    switch (m_spellInfo->ClassOptions.SpellClassSet)
    {
        case SPELLFAMILY_GENERIC:
        {
            if (!(mode & AURA_EFFECT_HANDLE_REAL))
                break;
            switch (GetId())
            {
                // Permanent Feign Death
                case 96733: //Permanent Feign Death (Stun)
                // Use it every time when we have port spawn
                //DELETE FROM creature_template_addon WHERE entry in(SELECT id FROM `creature` WHERE guid in(SELECT guid FROM `creature_addon` WHERE `auras` LIKE '%29266%'));
                //UPDATE creature_template SET `unit_flags` = `unit_flags` & ~(256 | 512 | 262144 | 536870912) where entry in(SELECT id FROM `creature` WHERE guid in(SELECT guid FROM `creature_addon` WHERE `auras` LIKE '%29266%'));
                case 29266:
                    if (!target)
                        break;
                    
                    if (apply)
                    {
                        target->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, 0x64);
                        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29 | UNIT_FLAG_UNK_15 | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    }
                    else
                    {
                        target->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, 0);
                        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29 | UNIT_FLAG_UNK_15 | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                    }
                    break;
                // Recently Bandaged
                case 11196:
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, GetMiscValue(), apply);
                    break;
                // Unstable Power
                case 24658:
                {
                    uint32 spellId = 24659;
                    if (apply && caster)
                    {
                        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellId);

                        for (uint32 i = 0; i < spell->AuraOptions.CumulativeAura; ++i)
                            caster->CastSpell(target, spell->Id, true, NULL, NULL, GetCasterGUID());
                        break;
                    }
                    target->RemoveAurasDueToSpell(spellId);
                    break;
                }
                // Restless Strength
                case 24661:
                {
                    uint32 spellId = 24662;
                    if (apply && caster)
                    {
                        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellId);
                        for (uint32 i = 0; i < spell->AuraOptions.CumulativeAura; ++i)
                            caster->CastSpell(target, spell->Id, true, NULL, NULL, GetCasterGUID());
                        break;
                    }
                    target->RemoveAurasDueToSpell(spellId);
                    break;
                }
                // Tag Murloc
                case 30877:
                {
                    // Tag/untag Blacksilt Scout
                    target->SetEntry(apply ? 17654 : 17326);
                    break;
                }
                case 57819: // Argent Champion
                case 57820: // Ebon Champion
                case 57821: // Champion of the Kirin Tor
                case 57822: // Wyrmrest Champion
                case 93337: // Champion of Ramkahen
                case 93339: // Champion of the Earthen Ring
                case 93341: // Champion of the Guardians of Hyjal
                case 93347: // Champion of Therazane
                case 93368: // Champion of the Wildhammer Clan
                case 94158: // Champion of the Dragonmaw Clan
                case 93795: // Stormwind Champion
                case 93805: // Ironforge Champion
                case 93806: // Darnassus Champion
                case 93811: // Exodar Champion
                case 93816: // Gilneas Champion
                case 93821: // Gnomeregan Champion
                case 93825: // Orgrimmar Champion
                case 93827: // Darkspear Champion
                case 93828: // Silvermoon Champion
                case 93830: // Bilgewater Champion
                case 94462: // Undercity Champion
                case 94463: // Thunder Bluff Champion
                case 126434: // Tushui
                case 126436: // Huojin
                {
                    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                        break;

                    uint32 FactionID = 0;
                    uint32 DungeonLevel = 0;

                    if (apply)
                    {
                        switch (m_spellInfo->Id)
                        {
                            case 57819: FactionID = 1106; break; // Argent Crusade
                            case 57820: FactionID = 1098; break; // Knights of the Ebon Blade
                            case 57821: FactionID = 1090; break; // Kirin Tor
                            case 57822: FactionID = 1091; break; // The Wyrmrest Accord
                            // Alliance factions
                            case 93795: FactionID = 72;   DungeonLevel = 0;  break; // Stormwind
                            case 93805: FactionID = 47;   DungeonLevel = 0;  break; // Ironforge
                            case 93806: FactionID = 69;   DungeonLevel = 0;  break; // Darnassus
                            case 93811: FactionID = 930;  DungeonLevel = 0;  break; // Exodar
                            case 93816: FactionID = 1134; DungeonLevel = 0;  break; // Gilneas
                            case 93821: FactionID = 54;   DungeonLevel = 0;  break; // Gnomeregan
                            case 126434: FactionID = 1353; DungeonLevel = 0; break; // Tushui Pandaren
                            // Horde factions
                            case 93825: FactionID = 76;   DungeonLevel = 0;  break; // Orgrimmar
                            case 93827: FactionID = 530;  DungeonLevel = 0;  break; // Darkspear Trolls
                            case 93828: FactionID = 911;  DungeonLevel = 0;  break; // Silvermoon
                            case 93830: FactionID = 1133; DungeonLevel = 0;  break; // Bilgewater Cartel
                            case 94462: FactionID = 68;   DungeonLevel = 0;  break; // Undercity
                            case 94463: FactionID = 81;   DungeonLevel = 0;  break; // Thunder Bluff
                            case 126436: FactionID = 1352; DungeonLevel = 0; break; // Huojin Pandaren
                            // Cataclysm factions
                            case 93337: FactionID = 1173; DungeonLevel = 83; break; // Ramkahen
                            case 93339: FactionID = 1135; DungeonLevel = 83; break; // The Earthen Ring
                            case 93341: FactionID = 1158; DungeonLevel = 83; break; // Guardians of Hyjal
                            case 93347: FactionID = 1171; DungeonLevel = 83; break; // Therazane
                            case 93368: FactionID = 1174; DungeonLevel = 83; break; // Wildhammer Clan
                            case 94158: FactionID = 1172; DungeonLevel = 83; break; // Dragonmaw Clan
                        }
                    }
                    caster->ToPlayer()->SetChampioningFaction(FactionID, DungeonLevel);
                    break;
                }
                // LK Intro VO (1)
                case 58204:
                    if (target->GetTypeId() == TYPEID_PLAYER)
                    {
                        // Play part 1
                        if (apply)
                            target->PlayDirectSound(14970, target->ToPlayer());
                        // continue in 58205
                        else
                            target->CastSpell(target, 58205, true);
                    }
                    break;
                // LK Intro VO (2)
                case 58205:
                    if (target->GetTypeId() == TYPEID_PLAYER)
                    {
                        // Play part 2
                        if (apply)
                            target->PlayDirectSound(14971, target->ToPlayer());
                        // Play part 3
                        else
                            target->PlayDirectSound(14972, target->ToPlayer());
                    }
                    break;
                case 62061: // Festive Holiday Mount
                    if (target->HasAuraType(SPELL_AURA_MOUNTED))
                    {
                        uint32 creatureEntry = 0;
                        if (apply)
                        {
                            if (target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                                creatureEntry = 24906;
                            else
                                creatureEntry = 15665;
                        }
                        else
                            creatureEntry = target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetMiscValue();

                        if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creatureEntry))
                        {
                            uint32 team = 0;
                            if (target->GetTypeId() == TYPEID_PLAYER)
                                team = target->ToPlayer()->GetTeam();

                            uint32 displayID = sObjectMgr->ChooseDisplayId(team, creatureInfo);
                            sObjectMgr->GetCreatureModelRandomGender(&displayID);

                            target->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, displayID);
                        }
                    }
                    break;
            }

            break;
        }
        case SPELLFAMILY_MAGE:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
                //break;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            //if (!(mode & AURA_EFFECT_HANDLE_REAL))
                //break;
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
                break;

            switch (GetId())
            {
                case 52610:                                 // Savage Roar
                case 127538:                                // Savage Roar
                {
                    if (apply)
                    {
                        target->RemoveAurasDueToSpell(GetId() == 52610 ? 127538 : 52610);
                        if (target->GetShapeshiftForm() == FORM_CAT)
                            target->CastSpell(target, 62071, true, NULL, NULL, GetCasterGUID());
                    }
                    else
                        target->RemoveAurasDueToSpell(62071);
                    break;
                }
                case 61336:                                 // Survival Instincts
                {
                    if (!(mode & AURA_EFFECT_HANDLE_REAL))
                        break;

                    if (apply)
                    {
                        if (!target->IsInFeralForm())
                            break;

                        target->CastSpell(target, 50322, true);
                    }
                    else
                        target->RemoveAurasDueToSpell(50322);
                    break;
                }
            }
            // Predatory Strikes
            if (target->GetTypeId() == TYPEID_PLAYER && GetSpellInfo()->Misc.SpellIconID == 1563)
            {
                target->ToPlayer()->UpdateAttackPowerAndDamage();
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            if (!(mode & AURA_EFFECT_HANDLE_REAL))
                break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            if (!(mode & AURA_EFFECT_HANDLE_REAL))
                break;
            // Improved Frost Presence
            if (m_spellInfo->Misc.SpellIconID == 2636)
            {
                if (apply)
                {
                    if (!target->HasAura(48266) && !target->HasAura(63611))
                        target->CastSpell(target, 63611, true);
                }
                else
                    target->RemoveAurasDueToSpell(63611);
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            if (!(mode & AURA_EFFECT_HANDLE_REAL))
                break;

            switch (GetId())
            {
                // Smoke Bomb
                case 76577:
                {
                    if (apply)
                    {
                        if (Aura* aur = caster->AddAura(88611, target))
                        {
                            aur->SetMaxDuration(GetBase()->GetDuration());
                            aur->SetDuration(GetBase()->GetDuration());
                        }
                    }
                    else
                        target->RemoveAura(88611);
                    break;
                }
            }

            break;
        }
		case SPELLFAMILY_WARLOCK:
			{
				switch(GetId())
				{
					// Demonic Pact
				case 47236:
					caster->CastSpell(caster, 53646, true);
					break;
				}
				break;   
			}
        }
    }
}

void AuraEffect::HandleChannelDeathItem(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (apply || aurApp->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
        return;

    Unit* caster = GetCaster();

    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* plCaster = caster->ToPlayer();

    // Item amount
    if (GetAmount() <= 0)
        return;

    if (GetSpellInfo()->GetEffect(m_effIndex, m_diffMode)->ItemType == 0)
        return;

    //Adding items
    uint32 noSpaceForCount = 0;
    uint32 count = m_amount;

    ItemPosCountVec dest;
    InventoryResult msg = plCaster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, GetSpellInfo()->GetEffect(m_effIndex, m_diffMode)->ItemType, count, &noSpaceForCount);
    if (msg != EQUIP_ERR_OK)
    {
        count-=noSpaceForCount;
        plCaster->SendEquipError(msg, NULL, NULL, GetSpellInfo()->GetEffect(m_effIndex, m_diffMode)->ItemType);
        if (count == 0)
            return;
    }

    Item* newitem = plCaster->StoreNewItem(dest, GetSpellInfo()->GetEffect(m_effIndex, m_diffMode)->ItemType, true);
    if (!newitem)
    {
        plCaster->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }
    plCaster->SendNewItem(newitem, count, true, true);
}

void AuraEffect::HandleBindSight(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
        return;

    caster->ToPlayer()->SetViewpoint(target, apply);
}

void AuraEffect::HandleForceReaction(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = (Player*)target;

    uint32 faction_id = GetMiscValue();
    ReputationRank faction_rank = ReputationRank(m_amount);

    player->GetReputationMgr().ApplyForceReaction(faction_id, faction_rank, apply);
    player->GetReputationMgr().SendForceReactions();

    // stop fighting if at apply forced rank friendly or at remove real rank friendly
    if ((apply && faction_rank >= REP_FRIENDLY) || (!apply && player->GetReputationRank(faction_id) >= REP_FRIENDLY))
        player->StopAttackFaction(faction_id);
}

void AuraEffect::HandleAuraEmpathy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_UNIT)
        return;

    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(target->GetEntry());
    if (ci && ci->Type == CREATURE_TYPE_BEAST)
        target->ApplyModUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO, apply);
}

void AuraEffect::HandleAuraModFaction(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->setFaction(GetMiscValue());
        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            //target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            target->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP);
            target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_IGNORE_REPUTATION);
        }
    }
    else
    {
        target->RestoreFaction();
        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            //target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            target->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP);
            target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_IGNORE_REPUTATION);
        }
    }
}

void AuraEffect::HandleComprehendLanguage(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_COMPREHEND_LANG);
    else
    {
        if (target->HasAuraType(GetAuraType()))
            return;

        target->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_COMPREHEND_LANG);
    }
}

void AuraEffect::HandleAuraConvertRune(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = (Player*)target;

    if (player->getClass() != CLASS_DEATH_KNIGHT)
        return;

    // convert number of runes specified in aura amount of rune type in miscvalue to runetype in miscvalueb
    if (apply)
    {
        player->ConvertRune(m_amount - 1, RuneType(GetMiscValueB()));
        player->SetBlockRuneConvert(m_amount - 1, true);
    }
    else
    {
        player->ConvertRune(m_amount - 1, player->GetBaseRune(m_amount - 1));
        player->SetBlockRuneConvert(m_amount - 1, false);
    }
}

void AuraEffect::HandleAuraLinked(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    Unit* target = aurApp->GetTarget();

    uint32 triggeredSpellId = m_spellInfo->GetEffect(m_effIndex, m_diffMode)->TriggerSpell;
    SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggeredSpellId);
    if (!triggeredSpellInfo)
        return;

    if (mode & AURA_EFFECT_HANDLE_REAL)
    {
        if (apply)
        {
            Unit* caster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, target->GetMap()->GetDifficultyID()) ? GetCaster() : target;

            if (!caster)
                return;
            // If amount avalible cast with basepoints (Crypt Fever for example)
            if (GetAmount())
                caster->CastCustomSpell(target, triggeredSpellId, &m_amount, NULL, NULL, true, NULL, this);
            else
                caster->CastSpell(target, triggeredSpellId, true, NULL, this);
        }
        else
        {
            ObjectGuid casterGUID = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, target->GetMap()->GetDifficultyID()) ? GetCasterGUID() : target->GetGUID();
            target->RemoveAura(triggeredSpellId, casterGUID, 9, aurApp->GetRemoveMode());
        }
    }
    else if (mode & AURA_EFFECT_HANDLE_REAPPLY && apply)
    {
        ObjectGuid casterGUID = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, target->GetMap()->GetDifficultyID()) ? GetCasterGUID() : target->GetGUID();
        // change the stack amount to be equal to stack amount of our aura
        Aura* triggeredAura = target->GetAura(triggeredSpellId, casterGUID);
        if (triggeredAura != NULL)
            triggeredAura->ModStackAmount(GetBase()->GetStackAmount() - triggeredAura->GetStackAmount());
    }
}

void AuraEffect::HandleAuraOpenStable(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TYPEID_PLAYER || !target->IsInWorld())
        return;

    if (apply)
        target->ToPlayer()->GetSession()->SendStablePet(target->GetGUID());

     // client auto close stable dialog at !apply aura
}

void AuraEffect::HandleAuraModFakeInebriation(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->m_invisibilityDetect.AddFlag(INVISIBILITY_DRUNK);
        target->m_invisibilityDetect.AddValue(INVISIBILITY_DRUNK, GetAmount());

        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            int32 oldval = target->ToPlayer()->GetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION);
            target->ToPlayer()->SetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION, oldval + GetAmount());
        }
    }
    else
    {
        bool removeDetect = !target->HasAuraType(SPELL_AURA_MOD_FAKE_INEBRIATE);

        target->m_invisibilityDetect.AddValue(INVISIBILITY_DRUNK, -GetAmount());

        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            int32 oldval = target->ToPlayer()->GetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION);
            target->ToPlayer()->SetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION, oldval - GetAmount());

            if (removeDetect)
                removeDetect = !target->ToPlayer()->GetDrunkValue();
        }

        if (removeDetect)
            target->m_invisibilityDetect.DelFlag(INVISIBILITY_DRUNK);
    }

    // call functions which may have additional effects after chainging state of unit
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleAuraOverrideSpells(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target || !target->IsInWorld())
        return;

    uint32 overrideId = uint32(GetMiscValue());

    if (apply)
    {
        target->SetUInt16Value(PLAYER_FIELD_BYTES2, PLAYER_BYTES_2_OVERRIDE_SPELLS_UINT16_OFFSET, overrideId);
        if (OverrideSpellDataEntry const* overrideSpells = sOverrideSpellDataStore.LookupEntry(overrideId))
            for (uint8 i = 0; i < MAX_OVERRIDE_SPELL; ++i)
                if (uint32 spellId = overrideSpells->SpellID[i])
                    target->AddTemporarySpell(spellId);
    }
    else
    {
        target->SetUInt16Value(PLAYER_FIELD_BYTES2, PLAYER_BYTES_2_OVERRIDE_SPELLS_UINT16_OFFSET, 0);
        if (OverrideSpellDataEntry const* overrideSpells = sOverrideSpellDataStore.LookupEntry(overrideId))
            for (uint8 i = 0; i < MAX_OVERRIDE_SPELL; ++i)
                if (uint32 spellId = overrideSpells->SpellID[i])
                    target->RemoveTemporarySpell(spellId);
    }
}

void AuraEffect::HandleAuraSetVehicle(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target)
        return;

    uint32 vehicleId = GetMiscValue();
    if (apply)
    {
        if (!target->CreateVehicleKit(vehicleId, 0, GetId()))
            return;
    }
    else if (target->GetVehicleKit())
        target->RemoveVehicleKit();

    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
    {
        target->SendSetVehicleRecId(vehicleId);
        if (Vehicle * veh = target->GetVehicleKit())
            veh->Reset();
        else 
            return;
    }

    if (apply)
        target->ToPlayer()->SendOnCancelExpectedVehicleRideAura();
}

void AuraEffect::HandlePreventResurrection(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (aurApp->GetTarget()->GetTypeId() != TYPEID_PLAYER)
        return;

    if (apply)
        aurApp->GetTarget()->RemoveFlag(PLAYER_FIELD_LOCAL_FLAGS, PLAYER_FIELD_BYTE_RELEASE_TIMER);
    else if (!aurApp->GetTarget()->GetBaseMap()->Instanceable())
        aurApp->GetTarget()->SetFlag(PLAYER_FIELD_LOCAL_FLAGS, PLAYER_FIELD_BYTE_RELEASE_TIMER);
}

bool AuraEffect::AuraSpellTrigger(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::AuraSpellTrigger: Spell %u in Effect %d", GetId(), effIndex);

    if (std::vector<SpellDummyTrigger> const* spellTrigger = sSpellMgr->GetSpellAuraTrigger(m_spellInfo->Id))
    {
        bool check = false;
        Unit* triggerTarget = target;
        Unit* triggerCaster = caster;
        int32 basepoints0 = m_amount;

        for (std::vector<SpellDummyTrigger>::const_iterator itr = spellTrigger->begin(); itr != spellTrigger->end(); ++itr)
        {
            if (!(itr->effectmask & (1 << effIndex)))
                continue;

            //if(target)
            {
                if (itr->target)
                    triggerTarget = (target ? target : caster)->GetUnitForLinkedSpell(caster, target, itr->target);

                if(!triggerTarget)
                {
                    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::AuraSpellTrigger: not exist triggerTarget");
                    check = true;
                    continue;
                }
                if(itr->targetaura > 0 && !triggerTarget->HasAura(itr->targetaura))
                {
                    check = true;
                    continue;
                }
                else if(itr->targetaura < 0 && triggerTarget->HasAura(abs(itr->targetaura)))
                {
                    check = true;
                    continue;
                }
            }

            if(caster)
            {
                if (itr->caster)
                    triggerCaster = caster->GetUnitForLinkedSpell(caster, target, itr->caster);
                if(itr->aura > 0 && !caster->HasAura(itr->aura))
                {
                    check = true;
                    continue;
                }
                else if(itr->aura < 0 && caster->HasAura(abs(itr->aura)))
                {
                    check = true;
                    continue;
                }
            }

            if(!triggerCaster)
            {
                //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::AuraSpellTrigger: not exist triggerCaster");
                check = true;
                continue;
            }

            if(itr->chance != 0)
            {
                if(!roll_chance_i(itr->chance))
                    continue;
            }

            int32 bp0 = int32(itr->bp0);
            int32 bp1 = int32(itr->bp1);
            int32 bp2 = int32(itr->bp2);
            int32 spell_trigger = m_amount;

            if (itr->spell_trigger != 0)
                spell_trigger = abs(itr->spell_trigger);

            switch (itr->option)
            {
                case AURA_TRIGGER: //0
                {
                    triggerCaster->CastSpell(triggerTarget, spell_trigger, true);
                    check = true;
                }
                break;
                case AURA_TRIGGER_BP: //1
                {
                    if (itr->spell_trigger < 0)
                        basepoints0 *= -1;

                    triggerCaster->CastCustomSpell(triggerTarget, spell_trigger, &basepoints0, &basepoints0, &basepoints0, true, NULL, this);
                    check = true;
                }
                break;
                case AURA_TRIGGER_BP_CUSTOM: //2
                {
                    triggerCaster->CastCustomSpell(triggerTarget, spell_trigger, &bp0, &bp1, &bp2, true, NULL, this);
                    check = true;
                }
                break;
                case AURA_TRIGGER_CHECK_COMBAT: //3
                {
                    if (itr->spell_trigger < 0 && !caster->isInCombat())
                        triggerCaster->CastSpell(triggerTarget, spell_trigger, true);
                    else if (itr->spell_trigger > 0 && caster->isInCombat())
                        triggerCaster->CastSpell(triggerTarget, spell_trigger, true);
                    check = true;
                }
                break;
                case AURA_TRIGGER_DEST: //4
                {
                    triggerCaster->CastSpell(triggerTarget->GetPositionX(), triggerTarget->GetPositionY(), triggerTarget->GetPositionZ(), spell_trigger, true, NULL, this);
                    check = true;
                }
                break;
                case AURA_TRIGGER_DYNOBJECT: //5
                {
                    std::list<DynamicObject*> list;
                    triggerCaster->GetDynObjectList(list, GetId());
                    if(!list.empty())
                    {
                        Unit* owner = triggerCaster->GetAnyOwner();
                        for (std::list<DynamicObject*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            if(DynamicObject* dynObj = (*itr))
                                triggerCaster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), spell_trigger, true, NULL, this, owner ? owner->GetGUID() : ObjectGuid::Empty);
                        }
                    }
                    check = true;
                }
                break;
                case AURA_TRIGGER_FROM_SUMMON_SLOT: //6
                {
                    if(itr->slot < MAX_SUMMON_SLOT)
                        if(triggerCaster->m_SummonSlot[itr->slot])
                            if(Creature* summon = triggerCaster->GetMap()->GetCreature(triggerCaster->m_SummonSlot[itr->slot]))
                                summon->CastSpell(summon, spell_trigger, true, NULL, this, triggerCaster->GetGUID());
                    check = true;
                }
                break;
                case AURA_TRIGGER_AREATRIGGER: //7
                {
                    std::list<AreaTrigger*> list;
                    triggerCaster->GetAreaObjectList(list, GetId());
                    if(!list.empty())
                    {
                        Unit* owner = triggerCaster->GetAnyOwner();
                        for (std::list<AreaTrigger*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            if(AreaTrigger* areaObj = (*itr))
                                triggerCaster->CastSpell(areaObj->GetPositionX(), areaObj->GetPositionY(), areaObj->GetPositionZ(), spell_trigger, true, NULL, this, owner ? owner->GetGUID() : ObjectGuid::Empty);
                        }
                    }
                    check = true;
                }
                break;
                case AAURA_TRIGGER_FROM_SUMMON_SLOT_DEST: //8
                {
                    if(itr->slot < MAX_SUMMON_SLOT)
                        if(triggerCaster->m_SummonSlot[itr->slot])
                            if(Creature* summon = triggerCaster->GetMap()->GetCreature(triggerCaster->m_SummonSlot[itr->slot]))
                                summon->CastSpell(summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), spell_trigger, true, NULL, this, triggerCaster->GetGUID());
                    check = true;
                }
                break;
                case AURA_TRIGGER_FROM_SUMMON_DEST: //9
                {
                    GuidList* summonList = triggerCaster->GetSummonList(itr->slot);
                    for (GuidList::const_iterator iter = summonList->begin(); iter != summonList->end(); ++iter)
                    {
                        if(Creature* summon = ObjectAccessor::GetCreature(*triggerCaster, (*iter)))
                            summon->CastSpell(summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), spell_trigger, true, NULL, this, triggerCaster->GetGUID());
                    }
                    check = true;
                }
                break;
                case AURA_TRIGGER_AREATRIGGER_CAST: //10
                {
                    std::list<AreaTrigger*> list;
                    triggerCaster->GetAreaObjectList(list, GetId());
                    if(!list.empty())
                    {
                        for (std::list<AreaTrigger*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            if(AreaTrigger* areaObj = (*itr))
                                areaObj->CastAction();
                        }
                    }
                    check = true;
                }
                break;
            }
        }
        if (check)
            return true;
    }
    return false;
}

void AuraEffect::HandlePeriodicDummyAuraTick(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
	if (GetId() == 102522)
    {
        switch(rand()%4)
        {
            case 1:
                caster->CastSpell(caster, 109090);
                break;
            case 2:
                caster->CastSpell(caster, 109105);
                break;
        }
        
        if(GetBase()->GetDuration() < 1000)
            caster->CastSpell(caster, 109107);
    }

    uint32 trigger_spell_id = m_spellInfo->GetEffect(m_effIndex, m_diffMode)->TriggerSpell;

    switch (GetSpellInfo()->ClassOptions.SpellClassSet)
    {
        case SPELLFAMILY_GENERIC:
            switch (GetId())
            {
                case 199837: // Shade of Xavius - Drain Essence
                {
                    if (caster)
                        caster->CastSpell(caster, 204502, true);
                    break;
                }
                case 200050: // Shade of Xavius - Apocolyptic Nightmare
                {
                    if (caster)
                    {
                        Position pos;
                        caster->GetRandomNearPosition(pos, 30.0f);
                        caster->CastSpell(pos, 200067, true);
                    }
                    break;
                }
                case 146310: // Restless Agility
                case 146317: // Restless Spirit
                {
                    if (Aura* aur = GetBase())
                    {
                        if (aur->GetStackAmount() > 1)
                            aur->SetStackAmount(aur->GetStackAmount() - 1);
                    }
                    break;
                }
                case 25281: // Turkey Marker
                {
                    if (Aura* aur = GetBase())
                    {
                        if (aur->GetStackAmount() >= 15)
                        {
                            target->CastSpell(target, 25285, true);
                            target->RemoveAura(25281);
                        }
                    }
                    break;
                }
                case 118694: // Spirit Bond
                {
                    if (caster->GetOwner() && caster == target && (!caster->IsFullHealth() || !caster->GetOwner()->IsFullHealth()))
                        trigger_spell_id = 149254;
                    break;
                }
                case 66149: // Bullet Controller Periodic - 10 Man
                {
                    if (!caster)
                        break;

                    caster->CastCustomSpell(66152, SPELLVALUE_MAX_TARGETS, urand(1, 6), target, true);
                    caster->CastCustomSpell(66153, SPELLVALUE_MAX_TARGETS, urand(1, 6), target, true);
                    break;
                }
                case 62399: // Overload Circuit
                    if (target->GetMap()->IsDungeon() && int(target->GetAppliedAuras().count(62399)) >= (target->GetMap()->IsHeroic() ? 4 : 2))
                    {
                         target->CastSpell(target, 62475, true); // System Shutdown
                         if (Unit* veh = target->GetVehicleBase())
                             veh->CastSpell(target, 62475, true);
                    }
                    break;
                case 64821: // Fuse Armor (Razorscale)
                    if (GetBase()->GetStackAmount() == GetSpellInfo()->AuraOptions.CumulativeAura)
                    {
                        target->CastSpell(target, 64774, true, NULL, NULL, GetCasterGUID());
                        target->RemoveAura(64821);
                    }
                    break;
            }
            break;
        case SPELLFAMILY_DRUID:
        {
            switch (GetSpellInfo()->Id)
            {
                case 145108: // Ysera's Gift
                {
                    uint32 triggerSpell = caster->IsFullHealth() ? 145110: 145109;
                    int32 heal = CalculatePct(caster->GetMaxHealth(), m_amount);

                    caster->CastCustomSpell(target, triggerSpell, &heal, 0, 0, true);
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            switch (GetSpellInfo()->Id)
            {
                // Master of Subtlety
                case 31666:
                    if (!target->HasAuraType(SPELL_AURA_MOD_STEALTH))
                        target->RemoveAurasDueToSpell(31665);
                    break;
            }
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            switch (GetSpellInfo()->Id)
            {
                // Camouflage
                case 80326:
                {
                    if (!caster || (caster->isMoving() && !caster->HasAura(119449) && !caster->isPet()) || caster->HasAura(80325))
                        return;

                    if (caster->HasAura(119449) || (caster->GetOwner() && caster->GetOwner()->HasAura(119449)))
                        caster->CastSpell(caster, 119450, true);
                    else
                        caster->CastSpell(caster, 80325, true);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        // Custom MoP Script
        case SPELLFAMILY_MONK:
        {
            if (!caster)
                break;

            switch (GetId())
            {
                case 138130: // Storm, Earth and Fire (clone)
                {
                    if (Creature* crt = caster->ToCreature())
                        if (CreatureAI* ai = crt->AI())
                            ai->RecalcStats();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            switch (GetId())
            {
                // Death's Advance
                case 96268:
                {
                    if (caster->ToPlayer()->GetRuneCooldown(RUNE_UNHOLY * 2) && caster->ToPlayer()->GetRuneCooldown(RUNE_UNHOLY * 2 + 1))
                        GetBase()->RefreshDuration();
                    else
                        caster->RemoveAurasDueToSpell(96268);
                    break;
                }
                // Death and Decay
                case 43265:
                {
                    trigger_spell_id = 52212;
                    break;
                }
                case 50034: // Blood Rites
                case 56835: // Reaping
                {
                    if (Player* plr = caster->ToPlayer())
                    {
                        if (Aura* aura = GetBase())
                            if (AuraEffect* eff = aura->GetEffect(m_effIndex))
                            {
                                if (plr->isInCombat())
                                    eff->SetAmount(10);
                                else
                                {
                                    if (GetAmount() != 0)
                                        eff->SetAmount(0);
                                    else
                                        plr->RestoreAllBaseRunes();
                                }
                            }
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    if(caster && trigger_spell_id)
    {
        std::list<DynamicObject*> list;
        caster->GetDynObjectList(list, GetId());
        if(!list.empty())
        {
            Unit* owner = caster->GetAnyOwner();
            for (std::list<DynamicObject*>::iterator itr = list.begin(); itr != list.end(); ++itr)
            {
                if(DynamicObject* dynObj = (*itr))
                    caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), trigger_spell_id, true, NULL, this, owner ? owner->GetGUID() : ObjectGuid::Empty);
            }
        }
        else if(target)
        {
            SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(trigger_spell_id);
            if(!triggeredSpellInfo)
                return;

            SpellCastResult checkResult = triggeredSpellInfo->CheckExplicitTarget(caster, target);
            if (checkResult == SPELL_CAST_OK)
                caster->CastSpell(target, trigger_spell_id, true, NULL, this);
            else if (Unit* _target = (caster->ToPlayer() ? caster->ToPlayer()->GetSelectedUnit() : caster->getVictim()))
            {
                SpellCastResult checkResult = triggeredSpellInfo->CheckExplicitTarget(caster, _target);
                if (checkResult == SPELL_CAST_OK)
                    caster->CastSpell(_target, trigger_spell_id, true, NULL, this);
                else
                    caster->CastSpell(caster, trigger_spell_id, true, NULL, this);
            }
        }
    }
}

void AuraEffect::HandlePeriodicTriggerSpellAuraTick(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
    // generic casting code with custom spells and target/caster customs
    uint32 triggerSpellId = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->TriggerSpell;

    SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId);
    SpellInfo const* auraSpellInfo = GetSpellInfo();
    uint32 auraId = auraSpellInfo->Id;

    // specific code for cases with no trigger spell provided in field
    if (triggeredSpellInfo == NULL)
    {
        switch (auraSpellInfo->ClassOptions.SpellClassSet)
        {
            case SPELLFAMILY_GENERIC:
            {
                switch (auraId)
                {
                    // Cannon Barrage, Skulloc
                    case 168129:
                    {
                        float x = frand(6752, 6857);
                        float y = frand(-1015, -953);

                        caster->CastSpell(x, y, caster->GetPositionZ(), 168384, true);
                        caster->CastSpell(x, y, caster->GetPositionZ(), 168385, true);
                        return;
                    }
                    // Earth's Vengeance, Morchok, Dragon Soul
                    case 103176:
                    {
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            Position pos;
                            caster->GetNearPosition(pos, 40.0f, frand(0.0f, 2 * M_PI));
                            if (caster->GetDistance(pos) >= 20.0f)
                                caster->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 103177, true);
                        }
                        return;
                    }
                    // Cluster Cluck: Remote Control Fireworks Visual
                    case 74177:
                        target->PlayDistanceSound(6820);
                        return;
                    // resistible Pool Pony: Naga Hatchling Proximity Control - Master
                    case 71917:
                        target->PlayDistanceSound(3437);
                        return;
                    // Thaumaturgy Channel
                    case 9712:
                        triggerSpellId = 21029;
                        break;
                    // Brood Affliction: Bronze
                    case 23170:
                        triggerSpellId = 23171;
                        break;
                    // Party G.R.E.N.A.D.E
                    case 51510:
                        triggerSpellId = 51508;
                        break;
                    // Restoration
                    case 24379:
                    case 23493:
                    {
                        if (caster)
                        {
                            int32 heal = caster->CountPctFromMaxHealth(10);
                            caster->HealBySpell(target, auraSpellInfo, heal);

                            if (int32 mana = caster->GetMaxPower(POWER_MANA))
                            {
                                mana /= 10;
                                caster->EnergizeBySpell(caster, 23493, mana, POWER_MANA);
                            }
                        }
                        return;
                    }
                    // Nitrous Boost
                    case 27746:
                        if (caster && target->GetPower(POWER_MANA) >= 10)
                        {
                            target->ModifyPower(POWER_MANA, -10, true);
                            target->SendEnergizeSpellLog(caster, 27746, 10, POWER_MANA);
                        }
                        else
                            target->RemoveAurasDueToSpell(27746);
                        return;
                    // Frost Blast
                    case 27808:
                        if (caster)
                            caster->CastCustomSpell(29879, SPELLVALUE_BASE_POINT0, int32(target->CountPctFromMaxHealth(21)), target, true, NULL, this);
                        return;
                    // Inoculate Nestlewood Owlkin
                    case 29528:
                        if (target->GetTypeId() != TYPEID_UNIT) // prevent error reports in case ignored player target
                            return;
                        break;
                    // Feed Captured Animal
                    case 29917:
                        triggerSpellId = 29916;
                        break;
                    // Extract Gas
                    case 30427:
                    {
                        // move loot to player inventory and despawn target
                        if (caster && caster->GetTypeId() == TYPEID_PLAYER &&
                                target->GetTypeId() == TYPEID_UNIT &&
                                target->ToCreature()->GetCreatureTemplate()->Type == CREATURE_TYPE_GAS_CLOUD)
                        {
                            Player* player = caster->ToPlayer();
                            Creature* creature = target->ToCreature();
                            // missing lootid has been reported on startup - just return
                            if (!creature->GetCreatureTemplate()->SkinLootId)
                                return;

                            player->AutoStoreLoot(creature->GetCreatureTemplate()->SkinLootId, LootTemplates_Skinning, 0, true);

                            creature->DespawnOrUnsummon();
                        }
                        return;
                    }
                    // Quake
                    case 30576:
                        triggerSpellId = 30571;
                        break;
                    // Doom
                    // TODO: effect trigger spell may be independant on spell targets, and executed in spell finish phase
                    // so instakill will be naturally done before trigger spell
                    case 31347:
                    {
                        target->CastSpell(target, 31350, true, NULL, this);
                        target->Kill(target);
                        return;
                    }
                    // Spellcloth
                    case 31373:
                    {
                        // Summon Elemental after create item
                        target->SummonCreature(17870, 0, 0, 0, target->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 0);
                        return;
                    }
                    // Flame Quills
                    case 34229:
                    {
                        // cast 24 spells 34269-34289, 34314-34316
                        for (uint32 spell_id = 34269; spell_id != 34290; ++spell_id)
                            target->CastSpell(target, spell_id, true, NULL, this);
                        for (uint32 spell_id = 34314; spell_id != 34317; ++spell_id)
                            target->CastSpell(target, spell_id, true, NULL, this);
                        return;
                    }
                    // Remote Toy
                    case 37027:
                        triggerSpellId = 37029;
                        break;
                    // Eye of Grillok
                    case 38495:
                        triggerSpellId = 38530;
                        break;
                    // Absorb Eye of Grillok (Zezzak's Shard)
                    case 38554:
                    {
                        if (!caster || target->GetTypeId() != TYPEID_UNIT)
                            return;

                        caster->CastSpell(caster, 38495, true, NULL, this);

                        Creature* creatureTarget = target->ToCreature();

                        creatureTarget->DespawnOrUnsummon();
                        return;
                    }
                    // Tear of Azzinoth Summon Channel - it's not really supposed to do anything, and this only prevents the console spam
                    case 39857:
                        triggerSpellId = 39856;
                        break;
                    // Personalized Weather
                    case 46736:
                        triggerSpellId = 46737;
                        break;
                    //Sha pool (Immerseus)
                    case 143462: 
                        {
                            if (caster && caster->ToCreature())
                            {
                                switch (caster->GetEntry())
                                {
                                case 71544: //npc_sha_pool
                                    triggerSpellId = 143297; //Sha splash
                                    break;
                                case 71543: //boss_immerseus
                                    triggerSpellId = 143460; //Sha pool dmg
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                        break;
                }
                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                switch (auraId)
                {
                    // Totemic Mastery (Skyshatter Regalia (Shaman Tier 6) - bonus)
                    case 38443:
                    {
                        bool all = true;
                        for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                        {
                            if (!target->m_SummonSlot[i])
                            {
                                all = false;
                                break;
                            }
                        }

                        if (all)
                            target->CastSpell(target, 38437, true, NULL, this);
                        else
                            target->RemoveAurasDueToSpell(38437);
                        return;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    else
    {
        // Spell exist but require custom code
        switch (auraId)
        {
            case 51769: // Emblazon Runeblade
                if (caster)
                    caster->CastSpell(caster, triggerSpellId, false);
                return;
            // Hour of Twilight, Ultraxion, Dragon Soul
            case 106371:
                if (caster)
                    if (Creature* pUltraxion = caster->ToCreature())
                        if (Unit* pTarget = pUltraxion->AI()->SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 0.0f, true))
                            pUltraxion->CastSpell(pTarget, 105925, true);
                        return;
            case 113656: // Fists of Fury
            {
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    return;
                break;
            }
            case 146194: // Flurry of Xuen
            {
                switch (effIndex)
                {
                    case EFFECT_0:
                    {
                        if (caster->getClass() == CLASS_HUNTER) 
                            return;
                        break;
                    }
                    case EFFECT_1:
                    {
                        if (caster->getClass() != CLASS_HUNTER) 
                            return;

                        if (Unit* _target = caster->getVictim())
                            if (caster->HasInArc(static_cast<float>(M_PI), _target))
                                caster->CastSpell(_target, triggerSpellId, true);
                        return;
                    }
                }
                break;
            }
            case 107851: // Focused Assault, Hagara, Dragon Soul
            {    
                if (caster)
                    if (Creature* pHagara = caster->ToCreature())
                        if (Unit* pTarget = pHagara->getVictim())
                        {
                            if (!pHagara->GetMap()->IsHeroic())
                                if (!pHagara->isInFront(pTarget) || !pHagara->IsWithinMeleeRange(pTarget))
                                {
                                    GetBase()->Remove();
                                    return;
                                }

                            pHagara->CastSpell(pHagara->getVictim(), 107850, true);
                        }
                return;
            }
            case 105285: // Target, Hagara, Dragon Soul
            {    
                if (caster)
                    caster->CastSpell(target, triggerSpellId, true);
                return;
            }
            // The Biggest Egg Ever: Mechachicken's Rocket Barrage Aura Effect
            case 71416:
                // prock 71419 with this action.
                target->PlayDistanceSound(23829);
                target->HandleEmoteCommand(403);
                break;
            case 114889: // Stone Bulwark Totem
            {
                if (Unit * shaman = caster->GetOwner())
                {
                    int32 SPD    = shaman->GetSpellPowerDamage();
                    int32 amount = SPD * 0.875f * (GetTickNumber() != 1 ? 1: 4);
                    caster->CastCustomSpell(shaman, triggerSpellId, &amount, NULL, NULL, true);
                }
                return;
            }
            // Pursuing Spikes (Anub'arak)
            case 65920:
            case 65922:
            case 65923:
            {
                Unit* permafrostCaster = NULL;
                Aura* permafrostAura = target->GetAura(66193);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67855);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67856);
                if (!permafrostAura)
                    permafrostAura = target->GetAura(67857);

                if (permafrostAura)
                    permafrostCaster = permafrostAura->GetCaster();

                if (permafrostCaster)
                {
                    if (Creature* permafrostCasterCreature = permafrostCaster->ToCreature())
                        permafrostCasterCreature->DespawnOrUnsummon(3000);

                    target->CastSpell(target, 66181, false);
                    target->RemoveAllAuras();
                    if (Creature* targetCreature = target->ToCreature())
                        targetCreature->DisappearAndDie();
                }
                break;
            }
            // Mana Tide
            case 16191:
                target->CastCustomSpell(target, triggerSpellId, &m_amount, NULL, NULL, true, NULL, this);
                return;
            // Negative Energy Periodic
            case 46284:
                target->CastCustomSpell(triggerSpellId, SPELLVALUE_MAX_TARGETS, m_tickNumber / 10 + 1, NULL, true, NULL, this);
                return;
            // Poison (Grobbulus)
            case 28158:
            case 54362:
            // Slime Pool (Dreadscale & Acidmaw)
            case 66882:
                target->CastCustomSpell(triggerSpellId, SPELLVALUE_RADIUS_MOD, (int32)((((float)m_tickNumber / 60) * 0.9f + 0.1f) * 10000 * 2 / 3), NULL, true, NULL, this);
                return;
            // Slime Spray - temporary here until preventing default effect works again
            // added on 9.10.2010
            case 69508:
            {
                if (caster)
                    caster->CastSpell(target, triggerSpellId, true, NULL, NULL, caster->GetGUID());
                return;
            }
            case 24745: // Summon Templar, Trigger
            case 24747: // Summon Templar Fire, Trigger
            case 24757: // Summon Templar Air, Trigger
            case 24759: // Summon Templar Earth, Trigger
            case 24761: // Summon Templar Water, Trigger
            case 24762: // Summon Duke, Trigger
            case 24766: // Summon Duke Fire, Trigger
            case 24769: // Summon Duke Air, Trigger
            case 24771: // Summon Duke Earth, Trigger
            case 24773: // Summon Duke Water, Trigger
            case 24785: // Summon Royal, Trigger
            case 24787: // Summon Royal Fire, Trigger
            case 24791: // Summon Royal Air, Trigger
            case 24792: // Summon Royal Earth, Trigger
            case 24793: // Summon Royal Water, Trigger
            case 68281: // Goblin quest: KTC Snapflash
            {
                // All this spells trigger a spell that requires reagents; if the
                // triggered spell is cast as "triggered", reagents are not consumed
                if (caster)
                    caster->CastSpell(target, triggerSpellId, false);
                return;
            }
            case 106768:
                return;
        }
    }

    // Reget trigger spell proto
    triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId);

    if (triggeredSpellInfo)
    {
        if (Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, target->GetMap()->GetDifficultyID()) ? caster : target)
        {
            std::list<DynamicObject*> list;
            triggerCaster->GetDynObjectList(list, GetId());
            if (!list.empty())
            {
                Unit* owner = caster->GetAnyOwner();
                for (std::list<DynamicObject*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                {
                    if (DynamicObject* dynObj = (*itr))
                        caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), triggerSpellId, true, NULL, this, owner ? owner->GetGUID() : ObjectGuid::Empty);
                }
            }
            else if (target)
            {
                SpellCastResult checkResult = triggeredSpellInfo->CheckExplicitTarget(triggerCaster, target);
                if (checkResult == SPELL_CAST_OK)
                    triggerCaster->CastSpell(target, triggeredSpellInfo, true, NULL, this);
                else if (Unit* _target = (triggerCaster->ToPlayer() ? triggerCaster->ToPlayer()->GetSelectedUnit() : triggerCaster->getVictim()))
                {
                    SpellCastResult checkResult = triggeredSpellInfo->CheckExplicitTarget(triggerCaster, _target);
                    if (checkResult == SPELL_CAST_OK)
                        triggerCaster->CastSpell(_target, triggeredSpellInfo, true, NULL, this);
                    else
                        triggerCaster->CastSpell(triggerCaster, triggeredSpellInfo, true, NULL, this);
                }
            }
        }
    }
    else
    {
        Creature* c = target->ToCreature();
        if (!c || !caster || !sScriptMgr->OnDummyEffect(caster, GetId(), SpellEffIndex(GetEffIndex()), target->ToCreature()) ||
            !c->AI()->sOnDummyEffect(caster, GetId(), SpellEffIndex(GetEffIndex())))
            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandlePeriodicTriggerSpellAuraTick: Spell %u has non-existent spell %u in EffectTriggered[%d] and is therefor not triggered.", GetId(), triggerSpellId, GetEffIndex());
    }
}

void AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick(Unit* target, Unit* caster, SpellEffIndex /*effIndex*/) const
{
    uint32 triggerSpellId = GetSpellInfo()->GetEffect(m_effIndex, m_diffMode)->TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        if (Unit* triggerCaster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, target->GetMap()->GetDifficultyID()) ? caster : target)
        {
            int32 basepoints0 = GetAmount();
            GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), basepoints0, target);
            triggerCaster->CastCustomSpell(target, triggerSpellId, &basepoints0, 0, 0, true, 0, this);
            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick: Spell %u Trigger %u", GetId(), triggeredSpellInfo->Id);
        }
    }
    else
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS,"AuraEffect::HandlePeriodicTriggerSpellWithValueAuraTick: Spell %u has non-existent spell %u in EffectTriggered[%d] and is therefor not triggered.", GetId(), triggerSpellId, GetEffIndex());
}

void AuraEffect::HandlePeriodicDamageAurasTick(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
    if (!caster || !target->isAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    // Consecrate ticks can miss and will not show up in the combat log
    if (GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false, 1 << GetEffIndex()) != SPELL_MISS_NONE)
        return;

    // some auras remove at specific health level or more
    if (GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
    {
        switch (GetSpellInfo()->Id)
        {
            case 43093: case 31956: case 38801:  // Grievous Wound
            case 35321: case 38363: case 39215:  // Gushing Wound
            case 115767:                         // Warrior: Deep Wounds
            {
                if (target->IsFullHealth())
                {
                    target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    return;
                }
                break;
            }
            case 38772: // Grievous Wound
            {
                uint32 percent = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);
                if (!target->HealthBelowPct(percent))
                {
                    target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    return;
                }
                break;
            }
        }
    }

    uint32 absorb = 0;
    uint32 resist = 0;
    CleanDamage cleanDamage = CleanDamage(0, 0, BASE_ATTACK, MELEE_HIT_NORMAL);

    // ignore non positive values (can be result apply spellmods to aura damage
    uint32 damage = std::max(GetAmount(), 0);
    uint32 damageBeforeHit = 0;

    bool crit = roll_chance_f(GetCritChance());
    if (crit)
        damage = GetCritAmount();

    if (GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE)
    {
        if (GetSpellInfo()->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA)
        {
            float critChance = 0.0f;
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), damage, DOT, effIndex, GetBase()->GetStackAmount());
            caster->isSpellCrit(target, GetSpellInfo(), GetSpellInfo()->GetSchoolMask(), BASE_ATTACK, critChance);
            crit = roll_chance_f(critChance);
            if (crit)
                damage = caster->SpellCriticalDamageBonus(GetSpellInfo(), damage, target);
        }
        if (!(GetSpellInfo()->HasAttribute(SPELL_ATTR9_UNK28)))
            damageBeforeHit = target->SpellDamageBonusForDamageBeforeHit(caster, GetSpellInfo(), damage);

        switch (GetSpellInfo()->Id)
        {
            case 980: // Agony
            {
                if (Aura* agony = GetBase())
                    damage = agony->CalcAgonyTickDamage(damage);
                break;
            }
            case 103103: // Malefic Grasp
            {
                int32 afflictionDamage = 0;
                if (caster->HasAura(157077) && target->HealthBelowPct(20)) // Improved Drain Soul
                    damage *= 2;

                // Every tick, Malefic Grasp deals instantly 50% of tick-damage for each affliction effects on the target
                // Corruption ...
                if (Aura* corruption = target->GetAura(146739, caster->GetGUID()))
                {
                    afflictionDamage = corruption->GetEffect(0)->GetAmount();

                    if (Aura* Malefic_Grasp = GetBase())
                        if (AuraEffect* MGEff2 = Malefic_Grasp->GetEffect(EFFECT_2))
                            afflictionDamage = CalculatePct(afflictionDamage, MGEff2->GetAmount());

                    caster->CastCustomSpell(target, 131740, &afflictionDamage, NULL, NULL, true);
                }
                // Unstable Affliction ...
                if (Aura* unstableAffliction = target->GetAura(30108, caster->GetGUID()))
                {
                    afflictionDamage = unstableAffliction->GetEffect(0)->GetAmount();

                    if (Aura* Malefic_Grasp = GetBase())
                        if (AuraEffect* MGEff2 = Malefic_Grasp->GetEffect(EFFECT_2))
                            afflictionDamage = CalculatePct(afflictionDamage, MGEff2->GetAmount());

                    caster->CastCustomSpell(target, 131736, &afflictionDamage, NULL, NULL, true);
                }
                // Agony ...
                if (Aura* agony = target->GetAura(980, caster->GetGUID()))
                {
                    afflictionDamage = agony->GetEffect(0)->GetAmount();

                    if (Aura* Malefic_Grasp = GetBase())
                        if (AuraEffect* MGEff2 = Malefic_Grasp->GetEffect(EFFECT_2))
                            afflictionDamage = CalculatePct(afflictionDamage, MGEff2->GetAmount());

                    caster->CastCustomSpell(target, 131737, &afflictionDamage, NULL, NULL, true);
                    agony->CalcAgonyTickDamage();
                }
                break;
            }
            default:
                break;
        }
        
        if (GetSpellInfo()->ClassOptions.SpellClassSet == SPELLFAMILY_GENERIC)
        {
            switch (GetId())
            {
                case 70911: // Unbound Plague
                    damage *= uint32(pow(1.25f, int32(m_tickNumber)));
                    break;
                default:
                    break;
            }
        }

        damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage);
    }
    else if (GetSpellInfo()->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA)
    {
        float critChance = 0.0f;
        damage = uint32(target->CountPctFromMaxHealth(damage));
        caster->isSpellCrit(target, GetSpellInfo(), GetSpellInfo()->GetSchoolMask(), BASE_ATTACK, critChance);
        crit = roll_chance_f(critChance);
        if (crit)
            damage = caster->SpellCriticalDamageBonus(GetSpellInfo(), damage, target);;
    }

    // If Doom critical tick, a Wild Imp will appear to fight with the Warlock
    if (m_spellInfo->Id == 603 && crit)
        caster->CastSpell(caster, 104317, true);

    int32 dmg = damage;

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), dmg, target);

    if (!(GetSpellInfo()->HasAttribute(SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS)) && m_spellInfo->Id != 110914)
        caster->ApplyResilience(target, &dmg, crit);

    if (target->getClass() == CLASS_MONK)
        dmg = target->CalcStaggerDamage(dmg, m_spellInfo->GetSchoolMask(), m_spellInfo);

    caster->CalcAbsorbResist(target, GetSpellInfo()->GetSchoolMask(), DOT, dmg, &absorb, &resist, GetSpellInfo());

    damage = caster->InterceptionOfDamage(target, dmg, DOT, m_spellInfo->GetSchoolMask(), m_spellInfo);

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: %u (TypeId: %u) attacked %u (TypeId: %u) for %u dmg inflicted by %u abs is %u",
        GetCasterGUID().GetCounter(), GetCasterGUID().GetHigh(), target->GetGUIDLow(), target->GetTypeId(), damage, GetId(), absorb);

    caster->DealDamageMods(target, damage, &absorb);

    DamageInfo dmgInfoProc = DamageInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo() ? SpellSchoolMask(GetSpellInfo()->Misc.SchoolMask) : SPELL_SCHOOL_MASK_NORMAL, SPELL_DIRECT_DAMAGE, damageBeforeHit);
    dmgInfoProc.AbsorbDamage(absorb);
    dmgInfoProc.ResistDamage(resist);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim   = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = (crit ? PROC_EX_CRITICAL_HIT : PROC_EX_NORMAL_HIT) | PROC_EX_INTERNAL_DOT;
    damage = (damage <= absorb+resist) ? 0 : (damage-absorb-resist);
    if (damage)
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;

    if (absorb && !damage)
        procEx |= PROC_EX_ABSORB;

    if (damage > 0)
    {
        if (GetSpellInfo()->Effects[m_effIndex].IsTargetingArea() || GetSpellInfo()->Effects[m_effIndex].Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA)
        {
            resist += damage;
            damage = int32(float(damage) * target->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_AOE_DAMAGE_AVOIDANCE, GetSpellInfo()->GetSchoolMask()));

            if (GetCaster() && GetCaster()->GetTypeId() == TYPEID_UNIT)
                damage = int32(float(damage) * target->GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE, GetSpellInfo()->GetSchoolMask()));

            resist -= damage;
        }
        if(GetBase()->m_aura_amount && damage && GetTotalTicks())
        {
            int32 auraDamage = int32(damage / GetTotalTicks());
            if(GetBase()->m_aura_amount > auraDamage)
                GetBase()->m_aura_amount -= auraDamage;
            else
                GetBase()->m_aura_amount = 0;
        }
    }

    int32 overkill = damage - target->GetHealth();
    if (overkill < 0)
        overkill = 0;

    SpellPeriodicAuraLogInfo pInfo(this, damage, overkill, absorb, resist, 0.0f, crit);
    target->SendPeriodicAuraLog(&pInfo);

    caster->ProcDamageAndSpell(target, procAttacker, procVictim, procEx, &dmgInfoProc, BASE_ATTACK, GetSpellInfo());

    caster->DealDamage(target, damage, &cleanDamage, DOT, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), true);
}

void AuraEffect::HandlePeriodicHealthLeechAuraTick(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
    if (!caster || !target->isAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    if (GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false, 1 << GetEffIndex()) != SPELL_MISS_NONE)
        return;

    uint32 absorb = 0;
    uint32 resist = 0;
    CleanDamage cleanDamage = CleanDamage(0, 0, BASE_ATTACK, MELEE_HIT_NORMAL);

    uint32 damage = std::max(GetAmount(), 0);
    uint32 damageBeforeHit = target->SpellDamageBonusForDamageBeforeHit(caster, GetSpellInfo(), damage);
    damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), damage);

    bool crit = roll_chance_f(GetCritChance());
    if (crit)
        damage = GetCritAmount();

    int32 dmg = damage;

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), dmg, target);

    caster->ApplyResilience(target, &dmg, crit);

    if (target->getClass() == CLASS_MONK)
        dmg = target->CalcStaggerDamage(dmg, m_spellInfo->GetSchoolMask(), m_spellInfo);

    damage = caster->InterceptionOfDamage(target, dmg, DOT, m_spellInfo->GetSchoolMask(), m_spellInfo);
    caster->CalcAbsorbResist(target, GetSpellInfo()->GetSchoolMask(), DOT, damage, &absorb, &resist, m_spellInfo);

    if (target->GetHealth() < damage)
        damage = uint32(target->GetHealth());

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: %u (TypeId: %u) health leech of %u (TypeId: %u) for %u dmg inflicted by %u abs is %u",
        GetCasterGUID().GetCounter(), GetCasterGUID().GetHigh(), target->GetGUIDLow(), target->GetTypeId(), damage, GetId(), absorb);

    caster->SendSpellNonMeleeDamageLog(target, GetId(), damage, GetSpellInfo()->GetSchoolMask(), absorb, resist, false, 0, crit);

    DamageInfo dmgInfoProc = DamageInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo() ? SpellSchoolMask(GetSpellInfo()->Misc.SchoolMask) : SPELL_SCHOOL_MASK_NORMAL, SPELL_DIRECT_DAMAGE, damageBeforeHit);
    dmgInfoProc.AbsorbDamage(absorb);
    dmgInfoProc.ResistDamage(resist);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim   = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = (crit ? PROC_EX_CRITICAL_HIT : PROC_EX_NORMAL_HIT) | PROC_EX_INTERNAL_DOT;
    damage = (damage <= absorb+resist) ? 0 : (damage-absorb-resist);

    if (damage)
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;
    if (caster->isAlive())
        caster->ProcDamageAndSpell(target, procAttacker, procVictim, procEx, &dmgInfoProc, BASE_ATTACK, GetSpellInfo());
    int32 new_damage = caster->DealDamage(target, damage, &cleanDamage, DOT, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), false);
    if (caster->isAlive())
    {
        float gainMultiplier = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->CalcValueMultiplier(caster);

        uint32 heal = uint32(caster->SpellHealingBonusDone(caster, GetSpellInfo(), uint32(new_damage * gainMultiplier), DOT, effIndex, GetBase()->GetStackAmount()));
        heal = uint32(caster->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT, effIndex, GetBase()->GetStackAmount()));

        int32 gain = caster->HealBySpell(caster, GetSpellInfo(), heal);
        caster->getHostileRefManager().threatAssist(caster, gain * 0.5f, GetSpellInfo());
    }
    if(GetBase()->m_aura_amount && damage && GetTotalTicks())
    {
        int32 auraDamage = int32(damage / GetTotalTicks());
        if(GetBase()->m_aura_amount > auraDamage)
            GetBase()->m_aura_amount -= auraDamage;
        else
            GetBase()->m_aura_amount = 0;
    }
}

void AuraEffect::HandlePeriodicHealthFunnelAuraTick(Unit* target, Unit* caster, SpellEffIndex /*effIndex*/) const
{
    if (!caster || !caster->isAlive() || !target->isAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    int32 damage = std::max(GetAmount(), 0);
    // do not kill health donator
    if ((int32)caster->GetHealth() < damage)
        damage = caster->GetHealth() - 1;
    if (!damage)
        return;

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), damage, target);

    caster->ModifyHealth(-(int32)damage);
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: donator %u target %u damage %u.", caster->GetEntry(), target->GetEntry(), damage);

    float gainMultiplier = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->CalcValueMultiplier(caster);

    damage = int32(damage * gainMultiplier);

    caster->HealBySpell(target, GetSpellInfo(), damage);
}

void AuraEffect::HandlePeriodicHealAurasTick(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
    if (!caster || !target->isAlive())
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // heal for caster damage (must be alive)
    if (target != caster && GetSpellInfo()->HasAttribute(SPELL_ATTR2_HEALTH_FUNNEL) && !caster->isAlive())
        return;

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->IsFullHealth())
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 damage = std::max(m_amount, 0);

    bool crit = roll_chance_f(GetCritChance());
    if (crit)
        damage = GetCritAmount();

    if (GetAuraType() == SPELL_AURA_OBS_MOD_HEALTH)
    {
        // Taken mods
        float TakenTotalMod = 1.0f;

        // Tenacity increase healing % taken
       if (AuraEffect const* Tenacity = target->GetAuraEffect(58549, 0))
            AddPct(TakenTotalMod, Tenacity->GetAmount());

        // Healing taken percent
       float ModHealingPct = target->GetTotalAuraMultiplier(SPELL_AURA_MOD_HEALING_PCT);
       if (ModHealingPct != 1.0f)
           TakenTotalMod *= ModHealingPct;

        TakenTotalMod = caster->CalcVersalityBonusDone(target, TakenTotalMod);
        TakenTotalMod = std::max(TakenTotalMod, 0.0f);

        damage = uint32(target->CountPctFromMaxHealth(damage));
        damage = uint32(damage * TakenTotalMod);
        
        switch (GetSpellInfo()->Id)
        {
            case 136:
            {
                if(target->isPet())
                    if (Unit* owner = target->GetOwner())
                    {
                        if(owner->HasAura(19573))
                           owner->CastSpell(target, 24406, true);
                    }
                break;
            }
            case 114635:
            {
                // Mastery: Emberstorm
                if (AuraEffect const* aurEff = caster->GetAuraEffect(77220, EFFECT_0))
                    AddPct(damage, aurEff->GetAmount());
                break;
            }
            default:
                break;
        }
    }
    else
    {
        // Wild Growth = amount + (6 - 2*doneTicks) * ticks* amount / 100
        if (m_spellInfo->ClassOptions.SpellClassSet == SPELLFAMILY_DRUID && m_spellInfo->Misc.SpellIconID == 2864)
        {
            float totalTicks = GetTotalTicks();
            uint32 tickNumber = GetTickNumber();
            int32 pct = (100 + totalTicks - 1) - (2 * (tickNumber - 1));
            int32 initDamage = damage;
            damage = CalculatePct(damage, pct);
            
            if (AuraEffect* aurEff = caster->GetAuraEffect(70658, 0)) // Item - Druid T10 Restoration 2P Bonus
            {
                int32 addition = damage - CalculatePct(initDamage, (100 + totalTicks) - (2 * tickNumber));
                damage += CalculatePct(addition, aurEff->GetAmount());
            }
        }

        if (GetSpellInfo()->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA)
        {
            float critChance = 0.0f;
            damage = caster->SpellHealingBonusDone(target, GetSpellInfo(), damage, DOT, effIndex, GetBase()->GetStackAmount());
            caster->isSpellCrit(target, GetSpellInfo(), GetSpellInfo()->GetSchoolMask(), BASE_ATTACK, critChance);
            crit = roll_chance_f(critChance);
            if (crit)
                damage = caster->SpellCriticalHealingBonus(GetSpellInfo(), damage, target);;
        }
        if (GetSpellInfo()->GetEffectMechanicMask(effIndex) & (1 << MECHANIC_BANDAGE))
        {
            if (AuraEffect* dummy = caster->GetAuraEffect(58369, 0)) // Glyph of Bloody Healing
            {
                if(caster->HasMyAura(115767)) // Deep Wounds
                    damage += CalculatePct(damage, dummy->GetAmount());
            }
        }
        damage = target->SpellHealingBonusTaken(caster, GetSpellInfo(), damage, DOT, effIndex, GetBase()->GetStackAmount());
    }

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: %u (TypeId: %u) heal of %u (TypeId: %u) for %u health inflicted by %u",
        GetCasterGUID().GetCounter(), GetCasterGUID().GetHigh(), target->GetGUIDLow(), target->GetTypeId(), damage, GetId());

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), damage, target);

    uint32 absorb = 0;
    uint32 heal = uint32(damage);
    caster->CalcHealAbsorb(target, GetSpellInfo(), heal, absorb);
    int32 gain = caster->DealHeal(target, heal, GetSpellInfo());

    if(GetBase()->m_aura_amount && heal && GetTotalTicks())
    {
        int32 auraDamage = int32(heal / GetTotalTicks());
        if(GetBase()->m_aura_amount > auraDamage)
            GetBase()->m_aura_amount -= auraDamage;
        else
            GetBase()->m_aura_amount = 0;
    }

    DamageInfo dmgInfoProc = DamageInfo(caster, target, damage, GetSpellInfo(), GetSpellInfo() ? SpellSchoolMask(GetSpellInfo()->Misc.SchoolMask) : SPELL_SCHOOL_MASK_NORMAL, SPELL_DIRECT_DAMAGE, damage);
    dmgInfoProc.AbsorbDamage(absorb);

    SpellPeriodicAuraLogInfo pInfo(this, heal, heal - gain, absorb, 0, 0.0f, crit);
    target->SendPeriodicAuraLog(&pInfo);

    target->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, GetSpellInfo());

    bool haveCastItem = GetBase()->GetCastItemGUID();

    // Health Funnel
    // damage caster for heal amount
    if (target != caster && GetSpellInfo()->Id == 755)
    {
        uint32 funnelDamage = caster->CountPctFromMaxHealth(2);
        caster->DealDamage(caster, funnelDamage, NULL, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim   = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx = (crit ? PROC_EX_CRITICAL_HIT : PROC_EX_NORMAL_HIT) | PROC_EX_INTERNAL_HOT;
    // ignore item heals
    if (!haveCastItem)
        caster->ProcDamageAndSpell(target, procAttacker, procVictim, procEx, &dmgInfoProc, BASE_ATTACK, GetSpellInfo());
}

void AuraEffect::HandlePeriodicManaLeechAuraTick(Unit* target, Unit* caster, SpellEffIndex /*effIndex*/) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!caster || !caster->isAlive() || !target->isAlive() || target->getPowerType() != powerType)
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    if (GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA &&
        caster->SpellHitResult(target, GetSpellInfo(), false, 1 << GetEffIndex()) != SPELL_MISS_NONE)
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 drainAmount = std::max(m_amount, 0);

    SpellPowerEntry power;
    if (!GetSpellInfo()->GetSpellPowerByCasterPower(GetCaster(), power))
        power = GetSpellInfo()->GetPowerInfo(0);

    // Special case: draining x% of mana (up to a maximum of 2*x% of the caster's maximum mana)
    // It's mana percent cost spells, m_amount is percent drain from target
    if (power.PowerCostPercentage)
    {
        // max value
        int32 maxmana = CalculatePct(caster->GetMaxPower(powerType), drainAmount * 2.0f);
        ApplyPct(drainAmount, target->GetMaxPower(powerType));
        if (drainAmount > maxmana)
            drainAmount = maxmana;
    }

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), drainAmount, target);

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: %u (TypeId: %u) power leech of %u (TypeId: %u) for %u dmg inflicted by %u",
        GetCasterGUID().GetCounter(), GetCasterGUID().GetHigh(), target->GetGUIDLow(), target->GetTypeId(), drainAmount, GetId());

    int32 drainedAmount = -target->ModifyPower(powerType, -drainAmount, true);

    float gainMultiplier = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->CalcValueMultiplier(caster);

    SpellPeriodicAuraLogInfo pInfo(this, drainedAmount, 0, 0, 0, gainMultiplier, false);
    target->SendPeriodicAuraLog(&pInfo);

    int32 gainAmount = int32(drainedAmount * gainMultiplier);
    int32 gainedAmount = 0;
    if (gainAmount)
    {
        gainedAmount = caster->ModifyPower(powerType, gainAmount);
        target->AddThreat(caster, float(gainedAmount) * 0.5f, GetSpellInfo()->GetSchoolMask(), GetSpellInfo());
    }

    // Drain Mana
    if (m_spellInfo->ClassOptions.SpellClassSet == SPELLFAMILY_WARLOCK && m_spellInfo->ClassOptions.SpellClassMask[0] & 0x00000010)
    {
        int32 manaFeedVal = 0;
        if (AuraEffect const* aurEff = GetBase()->GetEffect(1))
            manaFeedVal = aurEff->GetAmount();
        // Mana Feed - Drain Mana
        if (manaFeedVal > 0)
        {
            int32 feedAmount = CalculatePct(gainedAmount, manaFeedVal);
            caster->CastCustomSpell(caster, 32554, &feedAmount, NULL, NULL, true, NULL, this);
        }
    }
}

void AuraEffect::HandleObsModPowerAuraTick(Unit* target, Unit* caster, SpellEffIndex /*effIndex*/) const
{
    Powers powerType;
    if (GetMiscValue() == POWER_ALL)
        powerType = target->getPowerType();
    else
        powerType = Powers(GetMiscValue());

    if (!target->isAlive() || !target->GetMaxPower(powerType))
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->GetPower(powerType) == target->GetMaxPower(powerType))
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 amount = std::max(m_amount, 0) * target->GetMaxPower(powerType) /100;

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), amount, target);

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: %u (TypeId: %u) energize %u (TypeId: %u) for %u dmg inflicted by %u",
        GetCasterGUID().GetCounter(), GetCasterGUID().GetHigh(), target->GetGUIDLow(), target->GetTypeId(), amount, GetId());

    SpellPeriodicAuraLogInfo pInfo(this, amount, 0, 0, 0, 0.0f, false);
    target->SendPeriodicAuraLog(&pInfo);

    int32 gain = target->ModifyPower(powerType, amount);

    if (caster)
        target->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, GetSpellInfo());
}

void AuraEffect::HandlePeriodicEnergizeAuraTick(Unit* target, Unit* caster, SpellEffIndex /*effIndex*/) const
{
    Powers powerType = Powers(GetMiscValue());

    if (target->GetTypeId() == TYPEID_PLAYER && target->getPowerType() != powerType && !(m_spellInfo->HasAttribute(SPELL_ATTR7_CAN_RESTORE_SECONDARY_POWER)))
        return;

    if (!target->isAlive() || !target->GetMaxPower(powerType))
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED))
    {
        SendTickImmune(target, caster);
        return;
    }

    // don't regen when permanent aura target has full power
    if (GetBase()->IsPermanent() && target->GetPower(powerType) == target->GetMaxPower(powerType))
        return;

    // ignore negative values (can be result apply spellmods to aura damage
    int32 amount = std::max(m_amount, 0);

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), amount, target);

    SpellPeriodicAuraLogInfo pInfo(this, amount, 0, 0, 0, 0.0f, false);
    target->SendPeriodicAuraLog(&pInfo);

    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "PeriodicTick: %u (TypeId: %u) energize %u (TypeId: %u) for %u dmg inflicted by %u",
        GetCasterGUID().GetCounter(), GetCasterGUID().GetHigh(), target->GetGUIDLow(), target->GetTypeId(), amount, GetId());

    int32 gain = target->ModifyPower(powerType, amount);

    if (caster)
        target->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, GetSpellInfo());
}

void AuraEffect::HandlePeriodicPowerBurnAuraTick(Unit* target, Unit* caster, SpellEffIndex effIndex) const
{
    Powers powerType = Powers(GetMiscValue());

    if (!caster || !target->isAlive() || target->getPowerType() != powerType)
        return;

    if (target->HasUnitState(UNIT_STATE_ISOLATED) || target->IsImmunedToDamage(GetSpellInfo()))
    {
        SendTickImmune(target, caster);
        return;
    }

    // ignore negative values (can be result apply spellmods to aura damage
    int32 damage = std::max(m_amount, 0);

    GetBase()->CallScriptEffectChangeTickDamageHandlers(const_cast<AuraEffect const*>(this), damage, target);

    uint32 gain = uint32(-target->ModifyPower(powerType, -damage));

    float dmgMultiplier = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->CalcValueMultiplier(caster);

    SpellInfo const* spellProto = GetSpellInfo();
    SpellNonMeleeDamage damageInfo(caster, target, spellProto->Id, spellProto->Misc.SchoolMask);
    // no SpellDamageBonus for burn mana
    caster->CalculateSpellDamageTaken(&damageInfo, int32(gain * dmgMultiplier), spellProto, (1 << effIndex));

    caster->DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);

    caster->SendSpellNonMeleeDamageLog(&damageInfo);

    DamageInfo dmgInfoProc = DamageInfo(damageInfo, spellProto);

    // Set trigger flag
    uint32 procAttacker = PROC_FLAG_DONE_PERIODIC;
    uint32 procVictim   = PROC_FLAG_TAKEN_PERIODIC;
    uint32 procEx       = createProcExtendMask(&damageInfo, SPELL_MISS_NONE) | PROC_EX_INTERNAL_DOT;
    if (damageInfo.damage)
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;

    caster->ProcDamageAndSpell(damageInfo.target, procAttacker, procVictim, procEx, &dmgInfoProc, BASE_ATTACK, spellProto);

    caster->DealSpellDamage(&damageInfo, true);
}

void AuraEffect::HandleProcTriggerSpellAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo, SpellEffIndex /*effIndex*/)
{
    Unit* triggerCaster = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();

    uint32 triggerSpellId = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleProcTriggerSpellAuraProc: Triggering spell %u from aura %u proc", triggeredSpellInfo->Id, GetId());
        triggerCaster->CastSpell(triggerTarget, triggeredSpellInfo, true, NULL, this);
    }
    else
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS,"AuraEffect::HandleProcTriggerSpellAuraProc: Could not trigger spell %u from aura %u proc, because the spell does not have an entry in Spell.dbc.", triggerSpellId, GetId());
}

void AuraEffect::HandleProcTriggerSpellWithValueAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo, SpellEffIndex /*effIndex*/)
{
    Unit* triggerCaster = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();

    uint32 triggerSpellId = GetSpellInfo()->GetEffect(m_effIndex, m_diffMode)->TriggerSpell;
    if (SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggerSpellId))
    {
        int32 basepoints0 = GetAmount();
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleProcTriggerSpellWithValueAuraProc: Triggering spell %u with value %d from aura %u proc", triggeredSpellInfo->Id, basepoints0, GetId());
        triggerCaster->CastCustomSpell(triggerTarget, triggerSpellId, &basepoints0, NULL, NULL, true, NULL, this);
    }
    else
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS,"AuraEffect::HandleProcTriggerSpellWithValueAuraProc: Could not trigger spell %u from aura %u proc, because the spell does not have an entry in Spell.dbc.", triggerSpellId, GetId());
}

void AuraEffect::HandleProcTriggerDamageAuraProc(AuraApplication* aurApp, ProcEventInfo& eventInfo, SpellEffIndex effIndex)
{
    Unit* target = aurApp->GetTarget();
    Unit* triggerTarget = eventInfo.GetProcTarget();
    SpellNonMeleeDamage damageInfo(target, triggerTarget, GetId(), GetSpellInfo()->Misc.SchoolMask);
    uint32 damage = target->SpellDamageBonusDone(triggerTarget, GetSpellInfo(), GetAmount(), SPELL_DIRECT_DAMAGE, effIndex);
    damage = triggerTarget->SpellDamageBonusTaken(target, GetSpellInfo(), damage);
    target->CalculateSpellDamageTaken(&damageInfo, damage, GetSpellInfo(), (1 << effIndex));
    target->DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);
    target->SendSpellNonMeleeDamageLog(&damageInfo);
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleProcTriggerDamageAuraProc: Triggering %u spell damage from aura %u proc", damage, GetId());
    target->DealSpellDamage(&damageInfo, true);
}

void AuraEffect::HandleRaidProcFromChargeAuraProc(AuraApplication* aurApp, ProcEventInfo& /*eventInfo*/, SpellEffIndex /*effIndex*/)
{
    Unit* target = aurApp->GetTarget();

    uint32 triggerSpellId;
    switch (GetId())
    {
        case 57949:            // Shiver
            triggerSpellId = 57952;
            //animationSpellId = 57951; dummy effects for jump spell have unknown use (see also 41637)
            break;
        case 59978:            // Shiver
            triggerSpellId = 59979;
            break;
        case 43593:            // Cold Stare
            triggerSpellId = 43594;
            break;
        default:
            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleRaidProcFromChargeAuraProc: received not handled spell: %u", GetId());
            return;
    }

    int32 jumps = GetBase()->GetCharges();

    // current aura expire on proc finish
    GetBase()->SetCharges(0);
    GetBase()->SetUsingCharges(true);

    // next target selection
    if (jumps > 0)
    {
        if (Unit* caster = GetCaster())
        {
            float radius = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->CalcRadius(caster);

            if (Unit* triggerTarget = target->GetNextRandomRaidMemberOrPet(radius))
            {
                target->CastSpell(triggerTarget, GetSpellInfo(), true, NULL, this, GetCasterGUID());
                Aura* aura = triggerTarget->GetAura(GetId(), GetCasterGUID());
                if (aura != NULL)
                    aura->SetCharges(jumps);
            }
        }
    }

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleRaidProcFromChargeAuraProc: Triggering spell %u from aura %u proc", triggerSpellId, GetId());
    target->CastSpell(target, triggerSpellId, true, NULL, this, GetCasterGUID());
}


void AuraEffect::HandleRaidProcFromChargeWithValueAuraProc(AuraApplication* aurApp, ProcEventInfo& /*eventInfo*/, SpellEffIndex /*effIndex*/)
{
    Unit* target = aurApp->GetTarget();

    // Currently only Prayer of Mending
    if (!(GetSpellInfo()->ClassOptions.SpellClassSet == SPELLFAMILY_PRIEST && GetSpellInfo()->ClassOptions.SpellClassMask[1] & 0x20))
    {
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleRaidProcFromChargeWithValueAuraProc: received not handled spell: %u", GetId());
        return;
    }
    uint32 triggerSpellId = 33110;

    int32 value = GetAmount();

    int32 jumps = GetBase()->GetCharges();

    // current aura expire on proc finish
    GetBase()->SetCharges(0);
    GetBase()->SetUsingCharges(true);

    // next target selection
    if (jumps > 0)
    {
        if (Unit* caster = GetCaster())
        {
            float radius = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->CalcRadius(caster);

            if (Unit* triggerTarget = target->GetNextRandomRaidMemberOrPet(radius))
            {
                target->CastCustomSpell(triggerTarget, GetId(), &value, NULL, NULL, true, NULL, this, GetCasterGUID());
                Aura* aura = triggerTarget->GetAura(GetId(), GetCasterGUID());
                if (aura != NULL)
                    aura->SetCharges(jumps);
            }
        }
    }

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AuraEffect::HandleRaidProcFromChargeWithValueAuraProc: Triggering spell %u from aura %u proc", triggerSpellId, GetId());
    target->CastCustomSpell(target, triggerSpellId, &value, NULL, NULL, false, NULL, this, GetCasterGUID());
}

void AuraEffect::HandleAuraForceWeather(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target)
        return;

    if (apply)
        target->GetSession()->SendPacket(WorldPackets::Misc::Weather(WeatherState(GetMiscValue()), 1.0f).Write());
    else
    {
        // send weather for current zone
        if (Weather* weather = WeatherMgr::FindWeather(target->GetZoneId()))
            weather->SendWeatherUpdateToPlayer(target);
        else
        {
            if (!WeatherMgr::AddWeather(target->GetZoneId()))
            {
                // send fine weather packet to remove old weather
                WeatherMgr::SendFineWeatherUpdateToPlayer(target);
            }
        }
    }
}

void AuraEffect::HandleProgressBar(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target)
        return;

    if (!apply)
    {
        target->SetMaxPower(POWER_ALTERNATE_POWER, 0);
        target->SetPower(POWER_ALTERNATE_POWER, 0);
        return;
    }

    UnitPowerBarEntry const * entry = sUnitPowerBarStore.LookupEntry(GetMiscValue());
    if (!entry)
        return;

    target->SetMaxPower(POWER_ALTERNATE_POWER, entry->MaxPower);
    target->SetPower(POWER_ALTERNATE_POWER, entry->StartPower);
}

void AuraEffect::HandleAuraStrangulate(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target)
        return;

    // Asphyxiate
    if (m_spellInfo->Id == 108194)
    {
        int32 newZ = 1;
        target->SetControlled(apply, UNIT_STATE_STUNNED);

        if (apply)
            target->UpdateHeight(target->GetPositionZ() + newZ);
        else
            target->UpdateHeight(target->GetPositionZ() - newZ);
    }
}

void AuraEffect::HandleOverrideActionbarSpells(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    if (!apply)
    {
        Unit::AuraEffectList swaps = target->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
        Unit::AuraEffectList const& swaps2 = target->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
        if (!swaps2.empty())
            swaps.insert(swaps.end(), swaps2.begin(), swaps2.end());

        SpellInfo const* thisInfo = GetSpellInfo();

        // update auras that replace same spells that unapplying aura do
        for (Unit::AuraEffectList::const_iterator itr = swaps.begin(); itr != swaps.end(); ++itr)
        {
            SpellInfo const* otherInfo = (*itr)->GetSpellInfo();
            if (thisInfo->ClassOptions.SpellClassSet == otherInfo->ClassOptions.SpellClassSet)
            {
                bool updated = false;
                for (int i = 0; i < MAX_SPELL_EFFECTS; ++i)
                {
                    for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
                    {
                        if (thisInfo->Effects[i].SpellClassMask & otherInfo->Effects[j].SpellClassMask)
                        {
                            updated = true;
                            (*itr)->GetBase()->SetNeedClientUpdateForTargets();
                            break;
                        }
                    }
                    if (updated)
                        break;
                }
            }
        }
    }
}

void AuraEffect::HandleAuraModCategoryCooldown(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target || target->GetSession()->PlayerLoading())
        return;

    target->SendCategoryCooldownMods();
}

void AuraEffect::HandleAuraSeeWhileInvisible(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateObjectVisibility();
}

void AuraEffect::HandleAuraMastery(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateMasteryAuras();
}

void AuraEffect::HandleAuraModCharges(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->RecalculateSpellCategoryCharges(GetMiscValue());
}

void AuraEffect::HandleAuraModChargeRecoveryMod(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->RecalculateSpellCategoryRegenTime(GetMiscValue());
    target->SendSpellChargeData();
}

void AuraEffect::HandleBattlegroundFlag(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    // when removing flag aura, handle flag drop
    if (!apply && target)
    {
        if (target->InBattleground())
        {
            if (Battleground* bg = target->GetBattleground())
                bg->EventPlayerDroppedFlag(target);
        }
        else
            sOutdoorPvPMgr->HandleDropFlag(target, GetSpellInfo()->Id);
    }
}

void AuraEffect::HandleCreateAreaTrigger(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    //Use custom summon at spell_norushen_residual_corruption id 145074
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    uint32 triggerEntry = GetMiscValue();
    if (!triggerEntry || !target || !GetCaster() || GetCaster()->GetMap() != target->GetMap())
        return;

    // when removing flag aura, handle flag drop
    if (apply)
    {
        Position pos;
        target->GetPosition(&pos);

        AreaTrigger* areaTrigger = new AreaTrigger;
        if (!areaTrigger->CreateAreaTrigger(sObjectMgr->GetGenerator<HighGuid::AreaTrigger>()->Generate(), triggerEntry, GetCaster(), GetSpellInfo(), pos, pos, NULL, target->GetGUID()))
        {
            delete areaTrigger;
            return;
        }

        GetBase()->SetSpellAreaTrigger(areaTrigger->GetGUID());
    }
    else if(!apply)
    {
        ObjectGuid areaTriggerGuid = GetBase()->GetSpellAreaTrigger();
        if (!areaTriggerGuid.IsEmpty())
            if(AreaTrigger* areaTrigger = ObjectAccessor::GetAreaTrigger(*GetCaster(), areaTriggerGuid))
                areaTrigger->SetDuration(0);
    }
}

void AuraEffect::HandleAuraModSpellVisual(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target)
        return;

    return;

    if(apply)
    {
        Unit::AuraEffectList const& auras = target->GetAuraEffectsByType(GetAuraType());
        uint8 count = (auras.size() + 1) * 2;

        for (int i = 0; i <= count;)
        {
            uint32 current = target->GetDynamicPassiveSpells(i + 1);
            if(!current || current == m_spellInfo->Id)
            {
                target->SetDynamicPassiveSpells(0, i);
                target->SetDynamicPassiveSpells(m_spellInfo->Id, i + 1);
                break;
            }
            i += 2;
        }
    }
    else
    {
        Unit::AuraEffectList const& auras = target->GetAuraEffectsByType(GetAuraType());
        uint8 count = auras.size() * 2;

        for (int i = 0; i <= count;)
        {
            if(target->GetDynamicPassiveSpells(i + 1) == m_spellInfo->Id)
            {
                target->SetDynamicPassiveSpells(m_spellInfo->Id, i);
                target->SetDynamicPassiveSpells(0, i + 1);
                break;
            }
            i += 2;
        }
    }
}

void AuraEffect::HandleAuraActivateScene(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target || target->GetTypeId() != TYPEID_PLAYER)
        return;

    Position pos;
    target->GetPosition(&pos);

    target->ToPlayer()->SendSpellScene(GetMiscValue(), m_spellInfo, apply, &pos);
}


void AuraEffect::HandleAuraeEablePowerType(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target)
        return;

    if(apply)
        target->SetUInt32Value(UNIT_FIELD_OVERRIDE_DISPLAY_POWER_ID, GetMiscValue());
    else
        target->SetUInt32Value(UNIT_FIELD_OVERRIDE_DISPLAY_POWER_ID, 0);
}

void AuraEffect::HandleLootBonus(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target || !target->GetMap())
        return;

    //m_amount = target->GetMap()->GetDifficulty();
}

void AuraEffect::HandleSummonController(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* caster = GetCaster();
    if (!caster)
        return;

    uint32 summonSpellID = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->TriggerSpell;

    if (apply)
        caster->CastSpell(caster, summonSpellID, true);
    else
        caster->RemoveAllMinionsByFilter(summonSpellID, 1); // remove by spellID
}

void AuraEffect::HandleModNextSpell(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (target->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = target->ToPlayer();
    if (!player)
        return;

    uint32 triggeredSpellId = GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->TriggerSpell;
    if (apply)
        player->AddTemporarySpell(triggeredSpellId);
    else
        player->RemoveTemporarySpell(triggeredSpellId);
}

void AuraEffect::HandleModMultistrikePct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateMultistrike();
}

void AuraEffect::HandleModMultistrikeDamagePct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateMultistrikeDamage();
}

void AuraEffect::HandleModReadinessPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateReadiness();
}

void AuraEffect::HandleModLifeStealPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateLifesteal();
}

void AuraEffect::HandleModVersalityPct(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateVersality();
}

void AuraEffect::HandleDisableMovementForce(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!target)
        return;

    ObjectGuid forceGuid = target->GetForceGUID();
    if(!forceGuid.IsEmpty())
        if(WorldObject* at = ObjectAccessor::GetWorldObject(*target, forceGuid))
            target->SendMovementForce(at, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false);
}

void AuraEffect::HandleAnimReplacementSet(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (target)
        target->SetAIAnimKitId(GetSpellInfo()->GetEffect(GetEffIndex(), m_diffMode)->MiscValue);   
}
