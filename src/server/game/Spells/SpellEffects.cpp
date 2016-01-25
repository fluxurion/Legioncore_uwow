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
#include "AreaTrigger.h"
#include "Battleground.h"
#include "BattlegroundDG.h"
#include "BattlegroundEY.h"
#include "BattlegroundMgr.h"
#include "BattlegroundTP.h"
#include "BattlegroundWS.h"
#include "CellImpl.h"
#include "CollectionMgr.h"
#include "Common.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "DynamicObject.h"
#include "Formulas.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Garrison.h"
#include "GossipDef.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "InstanceScript.h"
#include "Language.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvPMgr.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "SocialMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "Unit.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "Util.h"
#include "Vehicle.h"
#include "VMapFactory.h"
#include "World.h"
#include "WorldPacket.h"

#include "CombatLogPackets.h"
#include "DuelPackets.h"
#include "MiscPackets.h"

pEffect SpellEffects[TOTAL_SPELL_EFFECTS] =
{
    &Spell::EffectNULL,                                     //  0
    &Spell::EffectInstaKill,                                //  1 SPELL_EFFECT_INSTAKILL
    &Spell::EffectSchoolDMG,                                //  2 SPELL_EFFECT_SCHOOL_DAMAGE
    &Spell::EffectDummy,                                    //  3 SPELL_EFFECT_DUMMY
    &Spell::EffectUnused,                                   //  4 SPELL_EFFECT_PORTAL_TELEPORT          unused
    &Spell::EffectTeleportUnits,                            //  5 SPELL_EFFECT_TELEPORT_UNITS
    &Spell::EffectApplyAura,                                //  6 SPELL_EFFECT_APPLY_AURA
    &Spell::EffectEnvironmentalDMG,                         //  7 SPELL_EFFECT_ENVIRONMENTAL_DAMAGE
    &Spell::EffectPowerDrain,                               //  8 SPELL_EFFECT_POWER_DRAIN
    &Spell::EffectHealthLeech,                              //  9 SPELL_EFFECT_HEALTH_LEECH
    &Spell::EffectHeal,                                     // 10 SPELL_EFFECT_HEAL
    &Spell::EffectBind,                                     // 11 SPELL_EFFECT_BIND
    &Spell::EffectNULL,                                     // 12 SPELL_EFFECT_PORTAL
    &Spell::EffectUnused,                                   // 13 SPELL_EFFECT_RITUAL_BASE              unused
    &Spell::EffectUnused,                                   // 14 SPELL_EFFECT_RITUAL_SPECIALIZE        unused
    &Spell::EffectUnused,                                   // 15 SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL   unused
    &Spell::EffectQuestComplete,                            // 16 SPELL_EFFECT_QUEST_COMPLETE
    &Spell::EffectWeaponDmg,                                // 17 SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
    &Spell::EffectResurrect,                                // 18 SPELL_EFFECT_RESURRECT
    &Spell::EffectAddExtraAttacks,                          // 19 SPELL_EFFECT_ADD_EXTRA_ATTACKS
    &Spell::EffectUnused,                                   // 20 SPELL_EFFECT_DODGE                    one spell: Dodge
    &Spell::EffectUnused,                                   // 21 SPELL_EFFECT_EVADE                    one spell: Evade (DND)
    &Spell::EffectParry,                                    // 22 SPELL_EFFECT_PARRY
    &Spell::EffectBlock,                                    // 23 SPELL_EFFECT_BLOCK                    one spell: Block
    &Spell::EffectCreateItem,                               // 24 SPELL_EFFECT_CREATE_ITEM
    &Spell::EffectUnused,                                   // 25 SPELL_EFFECT_WEAPON
    &Spell::EffectUnused,                                   // 26 SPELL_EFFECT_DEFENSE                  one spell: Defense
    &Spell::EffectPersistentAA,                             // 27 SPELL_EFFECT_PERSISTENT_AREA_AURA
    &Spell::EffectSummonType,                               // 28 SPELL_EFFECT_SUMMON
    &Spell::EffectLeap,                                     // 29 SPELL_EFFECT_LEAP
    &Spell::EffectEnergize,                                 // 30 SPELL_EFFECT_ENERGIZE
    &Spell::EffectWeaponDmg,                                // 31 SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
    &Spell::EffectTriggerMissileSpell,                      // 32 SPELL_EFFECT_TRIGGER_MISSILE
    &Spell::EffectOpenLock,                                 // 33 SPELL_EFFECT_OPEN_LOCK
    &Spell::EffectSummonChangeItem,                         // 34 SPELL_EFFECT_SUMMON_CHANGE_ITEM
    &Spell::EffectApplyAreaAura,                            // 35 SPELL_EFFECT_APPLY_AREA_AURA_PARTY
    &Spell::EffectLearnSpell,                               // 36 SPELL_EFFECT_LEARN_SPELL
    &Spell::EffectUnused,                                   // 37 SPELL_EFFECT_SPELL_DEFENSE            one spell: SPELLDEFENSE (DND)
    &Spell::EffectDispel,                                   // 38 SPELL_EFFECT_DISPEL
    &Spell::EffectUnused,                                   // 39 SPELL_EFFECT_LANGUAGE
    &Spell::EffectDualWield,                                // 40 SPELL_EFFECT_DUAL_WIELD
    &Spell::EffectJump,                                     // 41 SPELL_EFFECT_JUMP
    &Spell::EffectJumpDest,                                 // 42 SPELL_EFFECT_JUMP_DEST
    &Spell::EffectTeleUnitsFaceCaster,                      // 43 SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER
    &Spell::EffectLearnSkill,                               // 44 SPELL_EFFECT_SKILL_STEP
    &Spell::EffectPlayMovie,                                // 45 SPELL_EFFECT_PLAY_MOVIE
    &Spell::EffectUnused,                                   // 46 SPELL_EFFECT_SPAWN clientside, unit appears as if it was just spawned
    &Spell::EffectTradeSkill,                               // 47 SPELL_EFFECT_TRADE_SKILL
    &Spell::EffectUnused,                                   // 48 SPELL_EFFECT_STEALTH                  one spell: Base Stealth
    &Spell::EffectUnused,                                   // 49 SPELL_EFFECT_DETECT                   one spell: Detect
    &Spell::EffectTransmitted,                              // 50 SPELL_EFFECT_TRANS_DOOR
    &Spell::EffectUnused,                                   // 51 SPELL_EFFECT_FORCE_CRITICAL_HIT       unused
    &Spell::EffectUnused,                                   // 52 SPELL_EFFECT_GUARANTEE_HIT            unused
    &Spell::EffectEnchantItemPerm,                          // 53 SPELL_EFFECT_ENCHANT_ITEM
    &Spell::EffectEnchantItemTmp,                           // 54 SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY
    &Spell::EffectTameCreature,                             // 55 SPELL_EFFECT_TAMECREATURE
    &Spell::EffectSummonPet,                                // 56 SPELL_EFFECT_SUMMON_PET
    &Spell::EffectLearnPetSpell,                            // 57 SPELL_EFFECT_LEARN_PET_SPELL
    &Spell::EffectWeaponDmg,                                // 58 SPELL_EFFECT_WEAPON_DAMAGE
    &Spell::EffectCreateRandomItem,                         // 59 SPELL_EFFECT_CREATE_RANDOM_ITEM       create item base at spell specific loot
    &Spell::EffectProficiency,                              // 60 SPELL_EFFECT_PROFICIENCY
    &Spell::EffectSendEvent,                                // 61 SPELL_EFFECT_SEND_EVENT
    &Spell::EffectPowerBurn,                                // 62 SPELL_EFFECT_POWER_BURN
    &Spell::EffectThreat,                                   // 63 SPELL_EFFECT_THREAT
    &Spell::EffectTriggerSpell,                             // 64 SPELL_EFFECT_TRIGGER_SPELL
    &Spell::EffectApplyAreaAura,                            // 65 SPELL_EFFECT_APPLY_AREA_AURA_RAID
    &Spell::EffectRechargeManaGem,                          // 66 SPELL_EFFECT_CREATE_MANA_GEM          (possibly recharge it, misc - is item ID)
    &Spell::EffectHealMaxHealth,                            // 67 SPELL_EFFECT_HEAL_MAX_HEALTH
    &Spell::EffectInterruptCast,                            // 68 SPELL_EFFECT_INTERRUPT_CAST
    &Spell::EffectDistract,                                 // 69 SPELL_EFFECT_DISTRACT
    &Spell::EffectPull,                                     // 70 SPELL_EFFECT_PULL                     one spell: Distract Move
    &Spell::EffectPickPocket,                               // 71 SPELL_EFFECT_PICKPOCKET
    &Spell::EffectAddFarsight,                              // 72 SPELL_EFFECT_ADD_FARSIGHT
    &Spell::EffectUntrainTalents,                           // 73 SPELL_EFFECT_UNTRAIN_TALENTS
    &Spell::EffectApplyGlyph,                               // 74 SPELL_EFFECT_APPLY_GLYPH
    &Spell::EffectHealMechanical,                           // 75 SPELL_EFFECT_HEAL_MECHANICAL          one spell: Mechanical Patch Kit
    &Spell::EffectSummonObjectWild,                         // 76 SPELL_EFFECT_SUMMON_OBJECT_WILD
    &Spell::EffectScriptEffect,                             // 77 SPELL_EFFECT_SCRIPT_EFFECT
    &Spell::EffectUnused,                                   // 78 SPELL_EFFECT_ATTACK
    &Spell::EffectSanctuary,                                // 79 SPELL_EFFECT_SANCTUARY
    &Spell::EffectAddComboPoints,                           // 80 SPELL_EFFECT_ADD_COMBO_POINTS
    &Spell::EffectUnused,                                   // 81 SPELL_EFFECT_CREATE_HOUSE             one spell: Create House (TEST)
    &Spell::EffectNULL,                                     // 82 SPELL_EFFECT_BIND_SIGHT
    &Spell::EffectDuel,                                     // 83 SPELL_EFFECT_DUEL
    &Spell::EffectStuck,                                    // 84 SPELL_EFFECT_STUCK
    &Spell::EffectSummonPlayer,                             // 85 SPELL_EFFECT_SUMMON_PLAYER
    &Spell::EffectActivateObject,                           // 86 SPELL_EFFECT_ACTIVATE_OBJECT
    &Spell::EffectGameObjectDamage,                         // 87 SPELL_EFFECT_GAMEOBJECT_DAMAGE
    &Spell::EffectGameObjectRepair,                         // 88 SPELL_EFFECT_GAMEOBJECT_REPAIR
    &Spell::EffectGameObjectSetDestructionState,            // 89 SPELL_EFFECT_GAMEOBJECT_SET_DESTRUCTION_STATE
    &Spell::EffectKillCreditPersonal,                       // 90 SPELL_EFFECT_KILL_CREDIT              Kill credit but only for single person
    &Spell::EffectUnused,                                   // 91 SPELL_EFFECT_THREAT_ALL               one spell: zzOLDBrainwash
    &Spell::EffectEnchantHeldItem,                          // 92 SPELL_EFFECT_ENCHANT_HELD_ITEM
    &Spell::EffectForceDeselect,                            // 93 SPELL_EFFECT_FORCE_DESELECT
    &Spell::EffectSelfResurrect,                            // 94 SPELL_EFFECT_SELF_RESURRECT
    &Spell::EffectSkinning,                                 // 95 SPELL_EFFECT_SKINNING
    &Spell::EffectCharge,                                   // 96 SPELL_EFFECT_CHARGE
    &Spell::EffectCastButtons,                              // 97 SPELL_EFFECT_CAST_BUTTON (totem bar since 3.2.2a)
    &Spell::EffectKnockBack,                                // 98 SPELL_EFFECT_KNOCK_BACK
    &Spell::EffectDisEnchant,                               // 99 SPELL_EFFECT_DISENCHANT
    &Spell::EffectInebriate,                                //100 SPELL_EFFECT_INEBRIATE
    &Spell::EffectFeedPet,                                  //101 SPELL_EFFECT_FEED_PET
    &Spell::EffectDismissPet,                               //102 SPELL_EFFECT_DISMISS_PET
    &Spell::EffectReputation,                               //103 SPELL_EFFECT_GIVE_REPUTATION
    &Spell::EffectSummonObject,                             //104 SPELL_EFFECT_SUMMON_OBJECT_SLOT
    &Spell::EffectSurvey,                                   //105 SPELL_EFFECT_SURVEY
    &Spell::EffectSummonRaidMarker,                         //106 SPELL_EFFECT_SUMMON_RAID_MARKER
    &Spell::EffectNULL,                                     //107 SPELL_EFFECT_LOOT_CORPSE
    &Spell::EffectDispelMechanic,                           //108 SPELL_EFFECT_DISPEL_MECHANIC
    &Spell::EffectSummonDeadPet,                            //109 SPELL_EFFECT_SUMMON_DEAD_PET
    &Spell::EffectDestroyAllTotems,                         //110 SPELL_EFFECT_DESTROY_ALL_TOTEMS
    &Spell::EffectDurabilityDamage,                         //111 SPELL_EFFECT_DURABILITY_DAMAGE
    &Spell::EffectUnused,                                   //112 SPELL_EFFECT_112
    &Spell::EffectResurrectNew,                             //113 SPELL_EFFECT_RESURRECT_NEW
    &Spell::EffectTaunt,                                    //114 SPELL_EFFECT_ATTACK_ME
    &Spell::EffectDurabilityDamagePCT,                      //115 SPELL_EFFECT_DURABILITY_DAMAGE_PCT
    &Spell::EffectSkinPlayerCorpse,                         //116 SPELL_EFFECT_SKIN_PLAYER_CORPSE       one spell: Remove Insignia, bg usage, required special corpse flags...
    &Spell::EffectSpiritHeal,                               //117 SPELL_EFFECT_SPIRIT_HEAL              one spell: Spirit Heal
    &Spell::EffectSkill,                                    //118 SPELL_EFFECT_SKILL                    professions and more
    &Spell::EffectApplyAreaAura,                            //119 SPELL_EFFECT_APPLY_AREA_AURA_PET
    &Spell::EffectUnused,                                   //120 SPELL_EFFECT_TELEPORT_GRAVEYARD       one spell: Graveyard Teleport Test
    &Spell::EffectWeaponDmg,                                //121 SPELL_EFFECT_NORMALIZED_WEAPON_DMG
    &Spell::EffectUnused,                                   //122 SPELL_EFFECT_122                      unused
    &Spell::EffectSendTaxi,                                 //123 SPELL_EFFECT_SEND_TAXI                taxi/flight related (misc value is taxi path id)
    &Spell::EffectPullTowards,                              //124 SPELL_EFFECT_PULL_TOWARDS
    &Spell::EffectModifyThreatPercent,                      //125 SPELL_EFFECT_MODIFY_THREAT_PERCENT
    &Spell::EffectStealBeneficialBuff,                      //126 SPELL_EFFECT_STEAL_BENEFICIAL_BUFF    spell steal effect?
    &Spell::EffectProspecting,                              //127 SPELL_EFFECT_PROSPECTING              Prospecting spell
    &Spell::EffectApplyAreaAura,                            //128 SPELL_EFFECT_APPLY_AREA_AURA_FRIEND
    &Spell::EffectApplyAreaAura,                            //129 SPELL_EFFECT_APPLY_AREA_AURA_ENEMY
    &Spell::EffectRedirectThreat,                           //130 SPELL_EFFECT_REDIRECT_THREAT
    &Spell::EffectPlaySound,                                //131 SPELL_EFFECT_PLAY_SOUND               sound id in misc value (SoundEntries.dbc)
    &Spell::EffectPlayMusic,                                //132 SPELL_EFFECT_PLAY_MUSIC               sound id in misc value (SoundEntries.dbc)
    &Spell::EffectUnlearnSpecialization,                    //133 SPELL_EFFECT_UNLEARN_SPECIALIZATION   unlearn profession specialization
    &Spell::EffectKillCredit,                               //134 SPELL_EFFECT_KILL_CREDIT              misc value is creature entry
    &Spell::EffectNULL,                                     //135 SPELL_EFFECT_CALL_PET
    &Spell::EffectHealPct,                                  //136 SPELL_EFFECT_HEAL_PCT
    &Spell::EffectEnergizePct,                              //137 SPELL_EFFECT_ENERGIZE_PCT
    &Spell::EffectLeapBack,                                 //138 SPELL_EFFECT_LEAP_BACK                Leap back
    &Spell::EffectQuestClear,                               //139 SPELL_EFFECT_CLEAR_QUEST              Reset quest status (miscValue - quest ID)
    &Spell::EffectForceCast,                                //140 SPELL_EFFECT_FORCE_CAST
    &Spell::EffectForceCast,                                //141 SPELL_EFFECT_FORCE_CAST_WITH_VALUE
    &Spell::EffectTriggerSpell,                             //142 SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE
    &Spell::EffectApplyAreaAura,                            //143 SPELL_EFFECT_APPLY_AREA_AURA_OWNER
    &Spell::EffectKnockBack,                                //144 SPELL_EFFECT_KNOCK_BACK_DEST
    &Spell::EffectPullTowards,                              //145 SPELL_EFFECT_PULL_TOWARDS_DEST                      Black Hole Effect
    &Spell::EffectActivateRune,                             //146 SPELL_EFFECT_ACTIVATE_RUNE
    &Spell::EffectQuestFail,                                //147 SPELL_EFFECT_QUEST_FAIL               quest fail
    &Spell::EffectTriggerMissileSpell,                      //148 SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE
    &Spell::EffectChargeDest,                               //149 SPELL_EFFECT_CHARGE_DEST
    &Spell::EffectQuestStart,                               //150 SPELL_EFFECT_QUEST_START
    &Spell::EffectTriggerRitualOfSummoning,                 //151 SPELL_EFFECT_TRIGGER_SPELL_2
    &Spell::EffectSummonRaFFriend,                          //152 SPELL_EFFECT_SUMMON_RAF_FRIEND        summon Refer-a-Friend
    &Spell::EffectCreateTamedPet,                           //153 SPELL_EFFECT_CREATE_TAMED_PET         misc value is creature entry
    &Spell::EffectDiscoverTaxi,                             //154 SPELL_EFFECT_DISCOVER_TAXI
    &Spell::EffectTitanGrip,                                //155 SPELL_EFFECT_TITAN_GRIP Allows you to equip two-handed axes, maces and swords in one hand, but you attack $49152s1% slower than normal.
    &Spell::EffectEnchantItemPrismatic,                     //156 SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC
    &Spell::EffectCreateItem2,                              //157 SPELL_EFFECT_CREATE_ITEM_2            create item or create item template and replace by some randon spell loot item
    &Spell::EffectMilling,                                  //158 SPELL_EFFECT_MILLING                  milling
    &Spell::EffectRenamePet,                                //159 SPELL_EFFECT_ALLOW_RENAME_PET         allow rename pet once again
    &Spell::EffectTriggerSpell,                             //160 SPELL_EFFECT_TRIGGER_SPELL_4
    &Spell::EffectSpecCount,                                //161 SPELL_EFFECT_TALENT_SPEC_COUNT        second talent spec (learn/revert)
    &Spell::EffectActivateSpec,                             //162 SPELL_EFFECT_TALENT_SPEC_SELECT       activate primary/secondary spec
    &Spell::EffectUnused,                                   //163 SPELL_EFFECT_163  unused
    &Spell::EffectRemoveAura,                               //164 SPELL_EFFECT_REMOVE_AURA
    &Spell::EffectDamageFromMaxHealthPCT,                   //165 SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT
    &Spell::EffectGiveCurrency,                             //166 SPELL_EFFECT_GIVE_CURRENCY
    &Spell::EffectUpdatePlayerPhase,                        //167 SPELL_EFFECT_UPDATE_PLAYER_PHASE
    &Spell::EffectNULL,                                     //168 SPELL_EFFECT_168
    &Spell::EffectDestroyItem,                              //169 SPELL_EFFECT_DESTROY_ITEM
    &Spell::EffectNULL,                                     //170 SPELL_EFFECT_UPDATE_ZONE_AURAS_AND_PHASES
    &Spell::EffectSummonObject,                             //171 SPELL_EFFECT_OBJECT_WITH_PERSONAL_VISIBILITY
    &Spell::EffectResurrectWithAura,                        //172 SPELL_EFFECT_RESURRECT_WITH_AURA
    &Spell::EffectBuyGuilkBankTab,                          //173 SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB
    &Spell::EffectApplyAreaAura,                            //174 SPELL_EFFECT_APPLY_AURA_ON_PET_OR_SELF
    &Spell::EffectUnused,                                   //175 SPELL_EFFECT_175
    &Spell::EffectSanctuary,                                //176 SPELL_EFFECT_SANCTUARY_2
    &Spell::EffectDespawnDynamicObject,                     //177 SPELL_EFFECT_DESPAWN_DYNOBJECT
    &Spell::EffectUnused,                                   //178 SPELL_EFFECT_178 unused
    &Spell::EffectCreateAreaTrigger,                        //179 SPELL_EFFECT_CREATE_AREATRIGGER
    &Spell::EffectUnused,                                   //180 SPELL_EFFECT_UPDATE_AREATRIGGER
    &Spell::EffectUnlearnTalent,                            //181 SPELL_EFFECT_UNLEARN_TALENT
    &Spell::EffectDespawnAreatrigger,                       //182 SPELL_EFFECT_DESPAWN_AREATRIGGER
    &Spell::EffectNULL,                                     //183 SPELL_EFFECT_183
    &Spell::EffectNULL,                                     //184 SPELL_EFFECT_REPUTATION_REWARD
    &Spell::SendScene,                                      //185 SPELL_EFFECT_ACTIVATE_SCENE4
    &Spell::SendScene,                                      //186 SPELL_EFFECT_ACTIVATE_SCENE5
    &Spell::EffectRandomizeDigsites,                        //187 SPELL_EFFECT_RANDOMIZE_DIGSITES
    &Spell::EffectNULL,                                     //188 SPELL_EFFECT_STAMPEDE
    &Spell::EffectBonusLoot,                                //189 SPELL_EFFECT_LOOT_BONUS
    &Spell::EffectJoinOrLeavePlayerParty,                   //190 SPELL_EFFECT_JOIN_LEAVE_PLAYER_PARTY
    &Spell::EffectTeleportToDigsite,                        //191 SPELL_EFFECT_TELEPORT_TO_DIGSITE
    &Spell::EffectUncageBattlePet,                          //192 SPELL_EFFECT_UNCAGE_BATTLE_PET
    &Spell::EffectNULL,                                     //193 SPELL_EFFECT_193
    &Spell::EffectNULL,                                     //194 SPELL_EFFECT_194
    &Spell::SendScene,                                      //195 SPELL_EFFECT_ACTIVATE_SCENE
    &Spell::SendScene,                                      //196 SPELL_EFFECT_ACTIVATE_SCENE2
    &Spell::SendScene,                                      //197 SPELL_EFFECT_ACTIVATE_SCENE6
    &Spell::SendScene,                                      //198 SPELL_EFFECT_ACTIVATE_SCENE3 send package
    &Spell::EffectNULL,                                     //199 SPELL_EFFECT_199
    &Spell::EffectHealBattlePetPct,                         //200 SPELL_EFFECT_HEAL_BATTLEPET_PCT
    &Spell::EffectUnlockPetBattles,                         //201 SPELL_EFFECT_UNLOCK_PET_BATTLES
    &Spell::EffectNULL,                                     //202 SPELL_EFFECT_APPLY_AURA_WITH_VALUE
    &Spell::EffectRemoveAura,                               //203 SPELL_EFFECT_REMOVE_AURA_2 Based on 144863 -> This spell remove auras. 145052 possible trigger spell.
    &Spell::EffectNULL,                                     //204 SPELL_EFFECT_UPGRADE_BATTLE_PET
    &Spell::EffectNULL,                                     //205 SPELL_EFFECT_LAUNCH_QUEST_CHOICE
    &Spell::EffectCreateItem3,                              //206 SPELL_EFFECT_CREATE_ITEM_3
    &Spell::EffectNULL,                                     //207 SPELL_EFFECT_LAUNCH_QUEST_TASK
    &Spell::EffectNULL,                                     //208 SPELL_EFFECT_REPUTATION_SET
    &Spell::EffectNULL,                                     //209 SPELL_EFFECT_209
    &Spell::EffectLearnGarrisonBuilding,                    //210 SPELL_EFFECT_LEARN_GARRISON_BUILDING
    &Spell::EffectNULL,                                     //211 SPELL_EFFECT_LEARN_GARRISON_SPECIALIZATION
    &Spell::EffectNULL,                                     //212 SPELL_EFFECT_212
    &Spell::EffectJumpDest,                                 //213 SPELL_EFFECT_JUMP_DEST2
    &Spell::EffectCreateGarrison,                           //214 SPELL_EFFECT_CREATE_GARRISON
    &Spell::EffectNULL,                                     //215 SPELL_EFFECT_UPGRADE_CHARACTER_SPELLS
    &Spell::EffectNULL,                                     //216 SPELL_EFFECT_CREATE_SHIPMENT
    &Spell::EffectNULL,                                     //217 SPELL_EFFECT_UPGRADE_GARRISON
    &Spell::EffectNULL,                                     //218 SPELL_EFFECT_218
    &Spell::EffectSummonConversation,                       //219 SPELL_EFFECT_SUMMON_CONVERSATION
    &Spell::EffectAddGarrisonFollower,                      //220 SPELL_EFFECT_ADD_GARRISON_FOLLOWER
    &Spell::EffectAddGarrisonMission,                       //221 SPELL_EFFECT_ADD_GARRISON_MISSION
    &Spell::EffectCreateHeirloomItem,                       //222 SPELL_EFFECT_CREATE_HEIRLOOM_ITEM
    &Spell::EffectNULL,                                     //223 SPELL_EFFECT_CHANGE_ITEM_BONUSES
    &Spell::EffectActivateGarrisonBuilding,                 //224 SPELL_EFFECT_ACTIVATE_GARRISON_BUILDING
    &Spell::EffectNULL,                                     //225 SPELL_EFFECT_GRANT_BATTLEPET_LEVEL
    &Spell::EffectNULL,                                     //226 SPELL_EFFECT_226
    &Spell::EffectTeleportUnits,                            //227 SPELL_EFFECT_TELEPORT
    &Spell::EffectNULL,                                     //228 SPELL_EFFECT_228
    &Spell::EffectNULL,                                     //229 SPELL_EFFECT_SET_FOLLOWER_QUALITY
    &Spell::EffectIncreaseFollowerItemLevel,                //230 SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL
    &Spell::EffectNULL,                                     //231 SPELL_EFFECT_INCREASE_FOLLOWER_EXPERIENCE
    &Spell::EffectRemovePhase,                              //232 SPELL_EFFECT_REMOVE_PHASE
    &Spell::EffectReTrainFollower,                          //233 SPELL_EFFECT_RETRAIN_FOLLOWER
    &Spell::EffectNULL,                                     //234 SPELL_EFFECT_234
    &Spell::EffectNULL,                                     //235 SPELL_EFFECT_235
    &Spell::EffectGieveExperience,                          //236 SPELL_EFFECT_GIVE_EXPERIENCE
    &Spell::EffectNULL,                                     //237 SPELL_EFFECT_GIVE_RESTED_EXPERIENCE_BONUS
    &Spell::EffectNULL,                                     //238 SPELL_EFFECT_INCREASE_SKILL
    &Spell::EffectNULL,                                     //239 SPELL_EFFECT_END_GARRISON_BUILDING_CONSTRUCTION
    &Spell::EffectNULL,                                     //240 SPELL_EFFECT_240
    &Spell::EffectNULL,                                     //241 SPELL_EFFECT_241
    &Spell::EffectNULL,                                     //242 SPELL_EFFECT_242
    &Spell::EffectNULL,                                     //243 SPELL_EFFECT_APPLY_ENCHANT_ILLUSION
    &Spell::EffectNULL,                                     //244 SPELL_EFFECT_LEARN_FOLLOWER_ABILITY
    &Spell::EffectUpgradeHeirloom,                          //245 SPELL_EFFECT_UPGRADE_HEIRLOOM
    &Spell::EffectNULL,                                     //246 SPELL_EFFECT_FINISH_GARRISON_MISSION
    &Spell::EffectAddGarrisonMission,                       //247 SPELL_EFFECT_ADD_GARRISON_MISSION2
    &Spell::EffectNULL,                                     //248 SPELL_EFFECT_FINISH_SHIPMENT
    &Spell::EffectNULL,                                     //249 SPELL_EFFECT_249
    &Spell::EffectNULL,                                     //250 SPELL_EFFECT_TAKE_SCREENSHOT
};

void Spell::EffectNULL(SpellEffIndex /*effIndex*/)
{
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WORLD: Spell Effect DUMMY");
}

void Spell::EffectUnused(SpellEffIndex /*effIndex*/)
{
    // NOT USED BY ANY SPELL OR USELESS OR IMPLEMENTED IN DIFFERENT WAY IN TRINITY
}

void Spell::EffectResurrectNew(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->isAlive())
        return;

    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if (!unitTarget->IsInWorld())
        return;

    Player* target = unitTarget->ToPlayer();

    if (target->IsRessurectRequested())       // already have one active request
        return;

    uint32 health = damage;
    uint32 mana = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    ExecuteLogEffectResurrect(effIndex, target);
    target->SetResurrectRequestData(m_caster, health, mana, 0);
    SendResurrectRequest(target);
}

void Spell::EffectInstaKill(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (m_spellInfo->Id == 108503)
    {
        if (!unitTarget->GetHealth() || !unitTarget->isAlive())
        {
            unitTarget->ToPet()->Remove();
            return;
        }

        if (Pet* pet = unitTarget->ToPet())
            pet->CastPetAuras(false, m_spellInfo->Id);
    }

    if (unitTarget->HasAura(SPELL_BG_PREPARATION) || unitTarget->HasAura(SPELL_ARENA_PREPARATION))
        return;

    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
        if (unitTarget->ToPlayer()->GetCommandStatus(CHEAT_GOD))
            return;

    if (m_caster == unitTarget)                              // prevent interrupt message
        finish();

    WorldPackets::CombatLog::SpellInstakillLog data;
    data.Target = unitTarget->GetGUID();
    data.Caster = m_caster->GetGUID();
    data.SpellID = m_spellInfo->Id;
    m_caster->SendMessageToSet(data.Write(), true);

    m_caster->DealDamage(unitTarget, unitTarget->GetHealth(), NULL, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
}

void Spell::EffectEnvironmentalDMG(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive())
        return;

    uint32 absorb = 0;
    uint32 resist = 0;

    m_caster->CalcAbsorbResist(unitTarget, m_spellInfo->GetSchoolMask(), SPELL_DIRECT_DAMAGE, damage, &absorb, &resist, m_spellInfo);

    m_caster->SendSpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, damage, m_spellInfo->GetSchoolMask(), absorb, resist, false, 0, false);
    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
        unitTarget->ToPlayer()->EnvironmentalDamage(DAMAGE_FIRE, damage);
}

void Spell::EffectSchoolDMG(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "EffectSchoolDMG %i, m_diffMode %i, effIndex %i, spellId %u, damage %i", m_damage, m_diffMode, effIndex, m_spellInfo->Id, damage);

    if (unitTarget && unitTarget->isAlive())
    {
        switch (m_spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                switch (m_spellInfo->Id)                     // better way to check unknown
                {
                    case 105408: // Burning Blood dmg, Madness of Deathwing, Dragon Soul
                        if (m_triggeredByAuraSpell)
                        {
                            if (Aura* const aur = m_caster->GetAura(m_triggeredByAuraSpell->Id))
                                damage *= aur->GetStackAmount();
                        }
                        break;
                    // Resonating Crystal dmg, Morchok, Dragon Soul
                    case 103545:
                        if (!unitTarget)
                            break;

                        if (unitTarget->HasAura(103534))
                            damage *= 1.5f;
                        else if (unitTarget->HasAura(103536))
                            damage *= 0.7f;
                        else if (unitTarget->HasAura(103541))
                            damage *= 0.3f;

                        unitTarget->RemoveAura(103534);
                        unitTarget->RemoveAura(103536);
                        unitTarget->RemoveAura(103541);
                        break;
                    // Stomp, Morchok, Dragon Soul
                    case 103414:
                    {
                        if (!unitTarget)
                            break;

                        if (Creature* pMorchok = m_caster->ToCreature())
                        {
                            if ((pMorchok->GetEntry() == 57773) || pMorchok->AI()->GetData(3))
                                damage /= 2;

                            if ((unitTarget->GetGUID() == pMorchok->AI()->GetGUID(1)) ||
                                (unitTarget->GetGUID() == pMorchok->AI()->GetGUID(2)))
                                damage *= 2;
                        }
                        break;
                    }
                    // percent from health with min
                    case 25599:                             // Thundercrash
                    {
                        damage = unitTarget->GetHealth() / 2;
                        if (damage < 200)
                            damage = 200;
                        break;
                    }
                    // arcane charge. must only affect demons (also undead?)
                    case 45072:
                    {
                        if (unitTarget->GetCreatureType() != CREATURE_TYPE_DEMON
                            && unitTarget->GetCreatureType() != CREATURE_TYPE_UNDEAD)
                            return;
                        break;
                    }
                    // Gargoyle Strike
                    case 51963:
                    {
                        // about +4 base spell dmg per level
                        damage = (m_caster->getLevel() - 60) * 4 + 60;
                        break;
                    }
                }
                break;
            }
            case SPELLFAMILY_WARLOCK:
            {
                // Incinerate Rank 1 & 2
                if ((m_spellInfo->SpellFamilyFlags[1] & 0x000040) && m_spellInfo->Misc.SpellIconID == 2128)
                {
                    // Incinerate does more dmg (dmg/6) if the target have Immolate debuff.
                    // Check aura state for speed but aura state set not only for Immolate spell
                    if (unitTarget->HasAuraState(AURA_STATE_CONFLAGRATE))
                    {
                        if (unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_WARLOCK, 0x4, 0, 0))
                            damage += damage / 6;
                    }
                }
                break;
            }
            case SPELLFAMILY_DRUID:
            {
                switch (m_spellInfo->Id)
                {
                    case 102355: // Faerie Swarm
                    case 770:    // Faerie Fire
                    {
                        // Deals damage only if casted from bear form
                        if (m_caster->GetShapeshiftForm() != FORM_BEAR)
                            return;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case SPELLFAMILY_ROGUE:
            {
                // Envenom
                if (m_spellInfo->Id == 32645)
                {
                    if (Player* player = m_caster->ToPlayer())
                    {
                        uint8 combo = player->GetComboPoints(m_spellInfo->Id);

                        float ap = player->GetTotalAttackPowerValue(BASE_ATTACK);

                        if (combo)
                        {
                            damage += int32(0.306f * combo * ap + damage * combo); //!TODO 0.417 on 6.2.2a

                            // Eviscerate and Envenom Bonus Damage (item set effect)
                            if (m_caster->HasAura(37169))
                                damage += combo * 40;
                        }
                    }
                }
                // Eviscerate
                else if (m_spellInfo->Id == 2098)
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (uint32 combo = ((Player*)m_caster)->GetComboPoints(m_spellInfo->Id))
                        {
                            float ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK);
                            damage += int32(ap * combo * 0.577f); //!TODO 0.559f on 6.2.2a

                            // Eviscerate and Envenom Bonus Damage (item set effect)
                            if (m_caster->HasAura(37169))
                                damage += combo * 40;

                            if (AuraEffect* aurEff = unitTarget->GetAuraEffect(84617, 2, m_caster->GetGUID()))
                                AddPct(damage, aurEff->GetAmount());
                        }
                    }
                }
                // Crimson Tempest
                else if (m_spellInfo->Id == 121411)
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (uint32 combo = ((Player*)m_caster)->GetComboPoints(m_spellInfo->Id))
                            damage += int32(float(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)) * combo * 0.09f);
                    }
                }
                // Deadly Throw
                else if (m_spellInfo->Id == 26679)
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (uint32 combo = m_caster->ToPlayer()->GetComboPoints(m_spellInfo->Id))
                        {
                            damage += int32(float(m_caster->GetTotalAttackPowerValue(BASE_ATTACK)) * combo * 0.178f);
                            if (combo >= 3)
                                m_caster->CastSpell(unitTarget, 137576, true);
                        }
                    }
                }
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                switch (m_spellInfo->Id)
                {
                    // Claw, Bite, Smack
                    case 49966:
                    case 16827:
                    case 17253:
                    {
                        if (Unit* hunter = m_caster->GetOwner())
                        {
                            damage += int32(hunter->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.333f);

                            // Deals 100% more damage and costs 100% more Focus when your pet has 50 or more Focus.
                            if (m_caster->GetPower(POWER_FOCUS) >= 50)
                            {
                                damage *= 2;
                                m_caster->ModifyPower(POWER_FOCUS, -25, true);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }

                // Gore
                if (m_spellInfo->Misc.SpellIconID == 1578)
                {
                    if (m_caster->HasAura(57627))           // Charge 6 sec post-affect
                        damage *= 2;
                }
                break;
            }
            case SPELLFAMILY_MAGE:
            {
                switch (m_spellInfo->Id)
                {
                    case 42208: // Blizzard
                        m_caster->CastSpell(unitTarget, 12486, true);
                        break;
                    case 44461: // Living Bomb
                    {
                        SpellInfo const* Living_Bomb_Dot = sSpellMgr->GetSpellInfo(44457);
                        if (!Living_Bomb_Dot)
                            return;

                        float cof = Living_Bomb_Dot->Effects[EFFECT_2].BasePoints;
                        int32 SPD = m_caster->GetSpellPowerDamage(m_spellInfo->GetSchoolMask()) * Living_Bomb_Dot->Effects[EFFECT_0].BonusCoefficient;
                        int32 eff1val = Living_Bomb_Dot->Effects[EFFECT_0].CalcValue(m_caster);
                        uint8 totalticks = Living_Bomb_Dot->GetMaxDuration() / Living_Bomb_Dot->Effects[EFFECT_0].ApplyAuraPeriod;
                        eff1val += SPD;
                        eff1val *= totalticks;

                        damage = (cof / 100.0f) * eff1val;
                        break;
                    }
                    case 31707: // Waterbolt
                    {
                        if (Unit* owner = m_caster->GetOwner())
                            if (AuraEffect const* aurEff = owner->GetAuraEffect(76613, EFFECT_0))
                                AddPct(damage, aurEff->GetAmount());
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                switch (m_spellInfo->Id)
                {
                    // Ancient Fury
                    case 86704:
                    {
                        if (m_caster->GetTypeId() == TYPEID_PLAYER)
                        {
                            if (Aura* aura = m_caster->GetAura(86700))
                            {
                                uint8 stacks = aura->GetStackAmount();
                                damage = stacks * (damage + 0.1f * m_caster->GetSpellPowerDamage(m_spellInfo->GetSchoolMask()));
                                damage = m_caster->SpellDamageBonusDone(unitTarget, m_spellInfo, damage, SPELL_DIRECT_DAMAGE, effIndex);
                                uint32 count = 0;
                                for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                                    ++count;
                                damage /= count;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
        }

        if (m_originalCaster)
        {
            uint32 bonus = m_originalCaster->SpellDamageBonusDone(unitTarget, m_spellInfo, (uint32)damage, SPELL_DIRECT_DAMAGE, effIndex);
            damage = bonus + uint32(bonus * variance);
        }

        m_damage += damage;

        // Meteor like spells (divided damage to targets)
        if (m_spellInfo->AttributesCu & SPELL_ATTR0_CU_SHARE_DAMAGE)
        {
            uint32 count = 0;
            for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                if (ihit->effectMask & (1 << effIndex))
                    ++count;

            m_damage /= count;                    // divide to all targets
        }

        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "EffectSchoolDMG end %i, m_diffMode %i, effIndex %i, spellId %u, damage %i", m_damage, m_diffMode, effIndex, m_spellInfo->Id, damage);

        switch (m_spellInfo->Id)
        {
            case 51505:  // Lava Burst
            {
                if (m_caster->HasAura(138144)) // Item - Shaman T15 Elemental 4P Bonus
                    if (Player* player = m_caster->ToPlayer())
                        player->ModifySpellCooldown(114049, -1000);

                if (Aura* aura = unitTarget->GetAura(8050, m_caster->GetGUID()))
                    if (AuraEffect* eff = aura->GetEffect(EFFECT_2))
                        AddPct(m_damage, eff->GetAmount());
                break;
            }
            case 77451:  // Lava Burst (Mastery)
            {
                if (Aura* aura = unitTarget->GetAura(8050, m_caster->GetGUID()))
                    if (AuraEffect* eff = aura->GetEffect(EFFECT_2))
                        AddPct(m_damage, eff->GetAmount());
                break;
            }
            case 129176: // Shadow Word: Death (Glyph)
            {
                if (unitTarget->GetHealthPct() < 20)
                    m_damage *= 4;
                break;
            }
            case 32379:  // Shadow Word: Death
            {
                if (effIndex == EFFECT_0)
                    m_damage = 0;
                break;
            }
            case 113092: // Frost Bomb
            {
                if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    m_damage = CalculatePct(m_damage, 60);
                break;
            }
            case 114954: // Nether Tempest
            case 44461:  // Living Bomb
            {
                if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    m_damage = CalculatePct(m_damage, 70);
                break;
            }
            case 124468: // Mind Flay (Mastery)
            {
                if (m_caster->HasAura(139139))
                {
                    if (Aura* aur = unitTarget->GetAura(2944, m_caster->GetGUID()))
                    {
                        int32 addBonusPct = aur->GetEffect(EFFECT_2)->GetAmount();
                        AddPct(m_damage, addBonusPct);
                    }
                }
                break;
            }
            case 116858: // Chaos Bolt - nerf res
                if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    m_damage -= int32(m_damage * 0.25f);
                break;
        }
    }
}

bool Spell::SpellDummyTriggered(SpellEffIndex effIndex)
{
    if (std::vector<SpellDummyTrigger> const* spellTrigger = sSpellMgr->GetSpellDummyTrigger(m_spellInfo->Id))
    {
        bool check = false;
        Unit* triggerTarget = unitTarget;
        Unit* triggerCaster = m_caster;
        Unit* targetAura = m_caster;
        int32 basepoints0 = damage;
        uint32 cooldown_spell_id = 0;

        for (std::vector<SpellDummyTrigger>::const_iterator itr = spellTrigger->begin(); itr != spellTrigger->end(); ++itr)
        {
            #ifdef WIN32
            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectDummy %u, 1<<effIndex %u, itr->effectmask %u, option %u, spell_trigger %i, target %u (%u ==> %u)", m_spellInfo->Id, 1<<effIndex, itr->effectmask, itr->option, itr->spell_trigger, itr->target, triggerTarget ? triggerTarget->GetGUIDLow() : 0, triggerCaster ? triggerCaster->GetGUIDLow() : 0);
            #endif

            if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH && itr->option != DUMMY_TRIGGER_CAST_DEST && itr->option != DUMMY_TRIGGER_CAST_OR_REMOVE)
                continue;

            if (!(itr->effectmask & (1 << effIndex)))
                continue;

            #ifdef WIN32
            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectDummy2: %u, 1<<effIndex %u, itr->effectmask %u, option %u, spell_trigger %i, target %u (%u ==> %u)", m_spellInfo->Id, 1<<effIndex, itr->effectmask, itr->option, itr->spell_trigger, itr->target, triggerTarget ? triggerTarget->GetGUIDLow(): 0, triggerCaster ? triggerCaster->GetGUIDLow(): 0);
            #endif

            if (itr->target)
                triggerTarget = (m_originalCaster ? m_originalCaster : m_caster)->GetUnitForLinkedSpell(m_caster, unitTarget, itr->target);

            if (itr->caster)
                triggerCaster = (m_originalCaster ? m_originalCaster : m_caster)->GetUnitForLinkedSpell(m_caster, unitTarget, itr->caster);

            if (itr->targetaura)
                targetAura = (m_originalCaster ? m_originalCaster : m_caster)->GetUnitForLinkedSpell(m_caster, unitTarget, itr->targetaura);

            cooldown_spell_id = abs(itr->spell_trigger);
            if (triggerCaster && triggerCaster->ToPlayer())
                if (triggerCaster->ToPlayer()->HasSpellCooldown(cooldown_spell_id) && itr->option != DUMMY_TRIGGER_COOLDOWN)
                    return true;
            if (triggerCaster && triggerCaster->ToCreature())
                if (triggerCaster->ToCreature()->HasSpellCooldown(cooldown_spell_id) && itr->option != DUMMY_TRIGGER_COOLDOWN)
                    return true;

            int32 bp0 = int32(itr->bp0);
            int32 bp1 = int32(itr->bp1);
            int32 bp2 = int32(itr->bp2);
            int32 spell_trigger = damage;

            if (itr->spell_trigger != 0)
                spell_trigger = abs(itr->spell_trigger);

            if(triggerCaster && !triggerCaster->IsInWorld())
                return false;
            if(triggerTarget && !triggerTarget->IsInWorld())
                return false;

            if(targetAura)
            {
                if (itr->aura > 0)
                {
                    if (!targetAura->HasAura(abs(itr->aura)))
                    {
                        check = true;
                        continue;
                    }
                }
                else if (itr->aura < 0)
                {
                    if (targetAura->HasAura(abs(itr->aura)))
                    {
                        check = true;
                        continue;
                    }
                }
            }

            switch (itr->option)
            {
                case DUMMY_TRIGGER_BP: //0
                {
                    if(!triggerCaster || !triggerTarget)
                        break;
                    if (itr->spell_trigger < 0)
                        basepoints0 *= -1;

                    triggerCaster->CastCustomSpell(triggerTarget, spell_trigger, &basepoints0, &basepoints0, &basepoints0, true, m_CastItem, NULL, m_originalCasterGUID);
                    if (itr->target == 6)
                    {
                        if (Guardian* pet = triggerCaster->GetGuardianPet())
                            triggerCaster->CastCustomSpell(pet, spell_trigger, &basepoints0, &bp1, &bp2, true);
                    }
                    check = true;
                }
                break;
                case DUMMY_TRIGGER_BP_CUSTOM: //1
                {
                    if(!triggerCaster || !triggerTarget)
                        break;
                    triggerCaster->CastCustomSpell(triggerTarget, spell_trigger, &bp0, &bp1, &bp2, true, m_CastItem, NULL, m_originalCasterGUID);
                    if (itr->target == 6)
                    {
                        if (Guardian* pet = triggerCaster->GetGuardianPet())
                            triggerCaster->CastCustomSpell(pet, spell_trigger, &basepoints0, &bp1, &bp2, true);
                    }
                    check = true;
                }
                break;
                case DUMMY_TRIGGER_COOLDOWN: //2
                {
                    if(!triggerTarget)
                        break;
                    if (Player* player = triggerTarget->ToPlayer())
                    {
                        uint32 spellid = abs(spell_trigger);
                        if (itr->bp0 == 0.0f)
                            player->RemoveSpellCooldown(spellid, true);
                        else
                        {
                            int32 delay = itr->bp0;
                            if (delay > -1 * IN_MILLISECONDS)
                            {
                                if (roll_chance_i(50))
                                    player->ModifySpellCooldown(spellid, -1 * IN_MILLISECONDS);
                            }
                            else
                                player->ModifySpellCooldown(spellid, delay);
                        }
                    }
                    check = true;
                }
                break;
                case DUMMY_TRIGGER_CHECK_PROCK: //3
                {
                    if(!triggerCaster || !triggerTarget)
                        break;
                    if (triggerCaster->HasAura(itr->aura))
                    {
                        if (spell_trigger > 0)
                            triggerCaster->CastSpell(triggerTarget, spell_trigger, true);
                        else
                            triggerCaster->RemoveAura(spell_trigger);
                    }

                    check = true;
                }
                break;
                case DUMMY_TRIGGER_DUMMY: //4
                {
                    if(!triggerCaster || !triggerTarget)
                        break;
                    triggerCaster->CastSpell(triggerTarget, spell_trigger, false);
                    check = true;
                }
                break;
                case DUMMY_TRIGGER_CAST_DEST: //5
                {
                    if(!triggerCaster)
                        break;
                    if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_trigger))
                    {
                        SpellCastTargets targets;
                        if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
                            targets.SetUnitTarget(unitTarget);
                        else
                            targets.SetDst(m_targets);
                        CustomSpellValues values;
                        triggerCaster->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK, NULL, NULL, m_originalCasterGUID);
                    }
                    check = true;
                }
                break;
                case DUMMY_TRIGGER_CAST_OR_REMOVE: // 6
                {
                    m_caster->CastSpell(unitTarget, spell_trigger, true);
                    check = true;
                    break;
                }
                case DUMMY_TRIGGER_DAM_MAXHEALTH: //7
                {
                    if(!triggerCaster)
                        break;

                    int32 percent = basepoints0;
                    if (bp0)
                        percent += bp0;
                    if (bp1)
                        percent /= bp1;
                    if (bp2)
                        percent *= bp2;

                    basepoints0 = CalculatePct(triggerTarget->GetMaxHealth(), percent);

                    triggerCaster->CastCustomSpell(triggerTarget, spell_trigger, &basepoints0, &bp1, &bp2, true, m_CastItem, NULL, m_originalCasterGUID);
                    if (itr->target == 6)
                    {
                        if (Guardian* pet = triggerCaster->GetGuardianPet())
                            triggerCaster->CastCustomSpell(pet, spell_trigger, &basepoints0, &bp1, &bp2, true);
                    }
                    check = true;
                }
                break;
                case DUMMY_TRIGGER_COPY_AURA: // 8
                {
                    if (itr->aura && triggerTarget && triggerCaster)
                    {
                        if (Aura* aura = triggerCaster->GetAura(itr->aura))
                        {
                            Aura* copyAura = m_caster->AddAura(itr->aura, triggerTarget);
                            if(!copyAura)
                                break;
                            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                            {
                                AuraEffect* aurEff = aura->GetEffect(i);
                                AuraEffect* copyAurEff = copyAura->GetEffect(i);
                                if (aurEff && copyAurEff)
                                    copyAurEff->SetAmount(aurEff->GetAmount());
                            }
                            copyAura->SetStackAmount(aura->GetStackAmount());
                            copyAura->SetMaxDuration(aura->GetMaxDuration());
                            copyAura->SetDuration(aura->GetDuration());
                            copyAura->SetCharges(aura->GetCharges());
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

void Spell::EffectDummy(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    #ifdef WIN32
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "EffectDummy end %i, m_diffMode %i, effIndex %i, spellId %u, damage %i, effectHandleMode %u, GetExplicitTargetMask %u", m_damage, m_diffMode, effIndex, m_spellInfo->Id, damage, effectHandleMode, m_spellInfo->GetExplicitTargetMask());
    #endif

    uint32 spell_id = 0;
    int32 bp = 0;
    bool triggered = true;

    if (SpellDummyTriggered(effIndex))
        return;

    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget && !gameObjTarget && !itemTarget)
        return;

    // Fishing dummy
    if (m_spellInfo->Id == 131474)
    {
        if (Item* fishPole = m_caster->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
        {
            if (fishPole->GetTemplate()->Class == ITEM_CLASS_WEAPON && fishPole->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
                m_caster->CastSpell(m_caster, 131490, true);
            else
                m_caster->CastSpell(m_caster, 131476, true);
        }
    }

    // selection by spell family
    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (m_spellInfo->Id)
            {
                case 51858:     //Q: Death Comes From On High ID 12641
                    if (unitTarget &&
                        (unitTarget->GetEntry() == 28543 ||
                        unitTarget->GetEntry() == 28542 ||
                        unitTarget->GetEntry() == 28525 ||
                        unitTarget->GetEntry() == 28544))
                        if (Player *plr = m_caster->GetCharmerOrOwnerPlayerOrPlayerItself())
                            plr->KilledMonsterCredit(unitTarget->GetEntry(), unitTarget->GetGUID());
                    break;
                case 78640: // Deepstone Oil
                {
                    if (m_caster->HasAuraType(SPELL_AURA_MOUNTED))
                        m_caster->CastSpell(m_caster, 78639, true);
                    else
                        m_caster->CastSpell(m_caster, 78627, true);
                    break;
                }
                case 145111: // Moonfang's Curse
                {
                    if (roll_chance_i(30))
                        m_caster->CastSpell(m_caster, 145112, true); // Free Your Mind
                    break;
                }
                case 92679: // Flask of Battle
                {
                    uint32 spellid = 0;
                    uint32 Agi = m_caster->GetStat(STAT_AGILITY);
                    uint32 Str = m_caster->GetStat(STAT_STRENGTH);
                    uint32 Int = m_caster->GetStat(STAT_INTELLECT);
                    uint32 Spi = m_caster->GetStat(STAT_SPIRIT);

                    if (Player* pPlayer = m_caster->ToPlayer())
                    {
                        if (pPlayer->isInTankSpec())
                            pPlayer->CastSpell(pPlayer, 92729, true);
                        else
                        {
                            if (Agi > Str && Agi > Int && Agi > Spi) spellid = 92725;
                            else if (Str > Agi && Str > Int && Str > Spi) spellid = 92731;
                            else if (Int > Str && Int > Agi && Int > Spi) spellid = 92730;
                            else if (Spi > Str && Spi > Agi && Spi > Int) spellid = 94160;
                        }
                        pPlayer->CastCustomSpell(pPlayer, spellid, &damage, 0, 0, true);
                    }
                    break;
                }
                case 126734: // Synapse Springs (Mod II)
                {
                    uint32 spellid = 0;
                    uint32 Agi = m_caster->GetStat(STAT_AGILITY);
                    uint32 Str = m_caster->GetStat(STAT_STRENGTH);
                    uint32 Int = m_caster->GetStat(STAT_INTELLECT);

                    if (Agi > Str && Agi > Int) spellid = 96228;
                    else if (Str > Agi && Str > Int) spellid = 96229;
                    else if (Int > Str && Int > Agi) spellid = 96230;

                    m_caster->CastCustomSpell(m_caster, spellid, &damage, 0, 0, true);
                    break;
                }
                case 105617: // Alchemist's Flask
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 Agi = m_caster->GetStat(STAT_AGILITY);
                    uint32 Int = m_caster->GetStat(STAT_INTELLECT);
                    uint32 Str = m_caster->GetStat(STAT_STRENGTH);

                    if (Agi > Int && Agi > Str)
                        m_caster->CastCustomSpell(m_caster, 79639, &damage, 0, 0, true, m_CastItem);
                    else if (Int > Agi && Int > Str)
                        m_caster->CastCustomSpell(m_caster, 79640, &damage, 0, 0, true, m_CastItem);
                    else if (Str > Agi && Str > Int)
                        m_caster->CastCustomSpell(m_caster, 79638, &damage, 0, 0, true, m_CastItem);
                    break;
                }
                case 47484: // Ghoul: Huddle
                {
                    if (!unitTarget)
                        return;

                    // Dark Transformation - Replace spell
                    if (m_caster->HasAura(63560))
                        m_caster->CastSpell(unitTarget, 91837, true); // Putrid Bulwark
                    else
                        m_caster->CastSpell(unitTarget, 91838, true); // Huddle
                    break;
                }
                case 47482: // Ghoul: Leap
                {
                    if (!unitTarget)
                        return;

                    // Dark Transformation - Replace spell
                    if (m_caster->HasAura(63560))
                        m_caster->CastSpell(unitTarget, 91802, true); // Shambling Rush
                    else
                        m_caster->CastSpell(unitTarget, 91809, true); // Leap
                    break;
                }
                case 6203:  // Soulstone
                {
                    if (!unitTarget->isAlive())
                        unitTarget->CastSpell(unitTarget, 3026, true); // Self resurrect
                    break;
                }
                case 45206: // Copy Off-hand Weapon
                case 69892:
                {
                    unitTarget->CastSpell(m_caster, damage, true);
                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                        break;

                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (Item* offItem = m_caster->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                            unitTarget->SetVirtualItem(1, offItem->GetEntry());
                    }
                    else
                        unitTarget->SetVirtualItem(1, m_caster->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID + 1));
                    break;
                }
                case 41055: // Copy Mainhand Weapon
                case 63416:
                case 69891:
                {
                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                        break;

                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (Item* mainItem = m_caster->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                            unitTarget->SetVirtualItem(0, mainItem->GetEntry());
                    }
                    else
                        unitTarget->SetVirtualItem(0, m_caster->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID));
                    break;
                }
                case 97383: // Capturing
                {
                    m_caster->CastSpell(unitTarget, 97388);
                    break;
                }
                case 97388: // Capturing
                {
                    if (Player* player = m_caster->ToPlayer())
                    {
                        Battleground* bg = player->GetBattleground();
                        if (bg && (bg->GetTypeID(true) == BATTLEGROUND_DG || bg->GetTypeID() == BATTLEGROUND_DG))
                            ((BattlegroundDG*)bg)->HandlePointCapturing(player, unitTarget->ToCreature());
                    }

                    break;
                }
                case 126755:    // Wormhole: Pandaria
                {
                    if (!unitTarget)
                        return;

                    Player* player = unitTarget->ToPlayer();
                    if (!player)
                        return;

                    switch (urand(0, 8))
                    {
                        case 0:     // Dread Wastes, Zan'vess
                            player->TeleportTo(870, -1513.342896f, 4575.043945f, 367.025208f, 1.921501f);
                            break;
                        case 1:     // Dread Wastes, Rikkitun Village
                            player->TeleportTo(870, 635.419983f, 4150.577637f, 210.107971f, 3.444401f);
                            break;
                        case 2:     // The Jade Forest, Emperor's Omen
                            player->TeleportTo(870, 2387.586914f, -2107.073486f, 230.486252f, 5.69462f);
                            break;
                        case 3:     // Krasarang Wilds, Narsong Spires
                            player->TeleportTo(870, -1493.026123f, -380.700806f, 119.941902f, 1.411952f);
                            break;
                        case 4:     // Kun-Lai Summit, Firebough Nook
                            player->TeleportTo(870, 2087.523682f, 2113.091309f, 443.190643f, 5.433147f);
                            break;
                        case 5:     // Kun-Lai Summit, Isle of Reckoning
                            player->TeleportTo(870, 5052.794434f, 193.100662f, 2.693901f, 4.899077f);
                            break;
                        case 6:     // Townlong Steppes, Sra'vess
                            player->TeleportTo(870, 3074.227539f, 6119.812500f, 54.291294f, 0.226609f);
                            break;
                        case 7:     // Vale of Eternal Blossoms, Whitepetal Lake
                            player->TeleportTo(870, 1208.797974f, 1376.896851f, 363.663788f, 5.034821f);
                            break;
                        case 8:     // Valley of the Four Winds, The Heartland
                            player->TeleportTo(870, 125.107628f, 1024.812378f, 194.217041f, 3.868679f);
                            break;
                        default:
                            break;
                    }
                    return;
                }
                case 148565:    // Spectral Grog
                {
                    if (!unitTarget)
                        return;

                    Player* player = unitTarget->ToPlayer();
                    if (!player)
                        return;

                    player->CastSpell(player, player->getGender() == GENDER_MALE ? 148564 : 148563, true);
                    return;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
            switch (m_spellInfo->Id)
            {
                case 85673:  // Word of Glory
                {
                    spell_id = 130551;
                    break;
                }
                case 136494: // Word of Glory
                {
                    spell_id = 130551;
                    if (!unitTarget->IsFriendlyTo(m_caster))
                        spell_id = 130552;
                    break;
                }
                case 20473: // Holy Shock
                {
                    uint32 spellid = unitTarget->IsFriendlyTo(m_caster) ? 25914 : 25912;
                    m_caster->CastSpell(unitTarget, spellid, true);
                    break;
                }
                case 31789:                                 // Righteous Defense (step 1)
                {
                    // Clear targets for eff 1
                    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                        ihit->effectMask &= ~(1 << 1);

                    // not empty (checked), copy
                    Unit::AttackerSet attackers = unitTarget->getAttackers();

                    // remove invalid attackers
                    for (Unit::AttackerSet::iterator aItr = attackers.begin(); aItr != attackers.end();)
                        if (!(*aItr)->IsValidAttackTarget(m_caster))
                            attackers.erase(aItr++);
                        else
                            ++aItr;

                    // selected from list 3
                    uint32 maxTargets = std::min<uint32>(3, attackers.size());
                    for (uint32 i = 0; i < maxTargets; ++i)
                    {
                        Unit* attacker = Trinity::Containers::SelectRandomContainerElement(attackers);
                        AddUnitTarget(attacker, 1 << 1);
                        attackers.erase(attacker);
                    }

                    // now let next effect cast spell at each target.
                    return;
                }
                default:
                    break;
            }
            break;
        case SPELLFAMILY_WARRIOR:
        {
            switch (m_spellInfo->Id)
            {
                case 100: // Charge
                {
                    m_caster->EnergizeBySpell(m_caster, m_spellInfo->Id, damage, POWER_RAGE);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
            // Death Coil
            if (m_spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_DK_DEATH_COIL)
            {
                if (m_caster->IsFriendlyTo(unitTarget))
                {
                    if (unitTarget != m_caster && m_caster->HasAura(63333) && unitTarget->GetCreatureType() != CREATURE_TYPE_UNDEAD) // Glyph of Death Coil
                    {
                        int32 bp = damage + m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.8f;
                        m_caster->CastCustomSpell(unitTarget, 115635, &bp, NULL, NULL, false);
                    }
                    else
                    {
                        int32 bp = (damage + m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.8f) * 5.0f;
                        m_caster->CastCustomSpell(unitTarget, 47633, &bp, NULL, NULL, false);
                    }
                }
                else
                {
                    int32 bp = damage + m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.8f;
                    m_caster->CastCustomSpell(unitTarget, 47632, &bp, NULL, NULL, false);
                }
                return;
            }
    }

    switch (m_spellInfo->Id)
    {
        case 115921: // Legacy of the Emperor
        {
            spell_id = m_spellInfo->Effects[effIndex].BasePoints;

            if (unitTarget->IsInPartyWith(m_caster))
                spell_id = 117666;
            break;
        }
        case 145640: // Chi Brew
        {
            uint32 spellid = 115867;
            uint8  effect = 0;

            if      (m_caster->HasSpell(113656))  spellid = 125195;
            else if (m_caster->HasSpell(121253)) {spellid = 128939; effect = 1;}

            if (effect != effIndex)
                break;

            if (spellid == 125195)
            {
                for (uint32 i = 0; i < uint32(damage); ++i)
                    m_caster->CastSpell(m_caster, spellid, true);
            }
            else
            {
                if (!m_caster->HasAura(spellid)) damage -= 1;

                m_caster->SetAuraStack(spellid, m_caster, damage);
            }
            break;
        }
        // Lunar Invitation
        case 26373:
            if(m_caster->ToPlayer())
                m_caster->ToPlayer()->TeleportTo(1, 7581.144531f, -2211.47900f, 473.639771f, 0, 0);
        break;
        case 120165: //Conflagrate
        {
            UnitList friends;
            Trinity::AnyFriendlyUnitInObjectRangeCheck u_check(m_caster, m_caster, 5.0f);
            Trinity::UnitListSearcher<Trinity::AnyFriendlyUnitInObjectRangeCheck> searcher(m_caster, friends, u_check);
            m_caster->VisitNearbyObject(5.0f, searcher);

            for (UnitList::const_iterator unit = friends.begin(); unit != friends.end(); ++unit)
            {
                if (m_caster->GetGUID() == (*unit)->GetGUID())
                    continue;
                GetOriginalCaster()->CastSpell(*unit, 120160, true);
                GetOriginalCaster()->CastSpell(*unit, 120201, true);
            }

            break;
        }
        case 107045: //Jade Fire
            m_caster->CastSpell(unitTarget, 107098, false);
            break;
        case 106299: //Summon Living Air
        {
            TempSummon* enne = m_caster->SummonCreature(54631, m_caster->GetPositionX() + rand() % 5, m_caster->GetPositionY() + 2 + rand() % 5, m_caster->GetPositionZ() + 1, 3.3f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
            enne->AddThreat(m_caster, 2000.f);
            break;
        }
        case 120202: // Gate of the Setting Sun | Boss 3 | Bombard
            spell_id = GetSpellInfo()->Effects[0].BasePoints;
            break;
    }

    //spells triggered by dummy effect should not miss
    if (spell_id)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id);

        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SPELLS_AURAS, "EffectDummy of spell %u: triggering unknown spell id %i\n", m_spellInfo->Id, spell_id);
            return;
        }

        SpellCastTargets targets;
        targets.SetUnitTarget(unitTarget);
        Spell* spell = new Spell(m_caster, spellInfo, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, m_originalCasterGUID, true);
        if (bp) spell->SetSpellValue(SPELLVALUE_BASE_POINT0, bp);
        spell->prepare(&targets);
    }

    // normal DB scripted effect
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell ScriptStart spellid %u in EffectDummy(%u) caster %s target %s", m_spellInfo->Id, effIndex, m_caster ? m_caster->ToString().c_str() : "", unitTarget ? unitTarget->ToString().c_str() : "");
    m_caster->GetMap()->ScriptsStart(sSpellScripts, uint32(m_spellInfo->Id | (effIndex << 24)), m_caster, unitTarget);

    // Script based implementation. Must be used only for not good for implementation in core spell effects
    // So called only for not proccessed cases
    if (gameObjTarget)
        sScriptMgr->OnDummyEffect(m_caster, m_spellInfo->Id, effIndex, gameObjTarget);
    else if (unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnDummyEffect(m_caster, m_spellInfo->Id, effIndex, unitTarget->ToCreature());
    else if (itemTarget)
        sScriptMgr->OnDummyEffect(m_caster, m_spellInfo->Id, effIndex, itemTarget);
}

void Spell::EffectTriggerSpell(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;

    // todo: move those to spell scripts
    if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
    {
        // special cases
        switch (triggered_spell_id)
        {
            case 153795: //Skyreach: Rukhran - Pierce Armor
            {
                if (unitTarget->HasAura(77535) || unitTarget->HasAura(112048) || unitTarget->HasAura(115308)
                || unitTarget->HasAura(132402) || unitTarget->HasAura(132403))
                {
                    return;
                }
                break;
            }
            // Demonic Fury (not exist)
            case 104330:
            {
                if (Unit* owner = m_caster->GetAnyOwner())
                    m_caster->EnergizeBySpell(owner, m_spellInfo->Id, damage, POWER_OBSOLETE2);
                return;
            }
            // Vanish (not exist)
            case 18461:
            {
                unitTarget->RemoveMovementImpairingAuras();
                unitTarget->RemoveAurasByType(SPELL_AURA_MOD_STALKED);
                unitTarget->CastSpell(unitTarget, 11327, true);
                unitTarget->CombatStop();
                return;
            }
            // Brittle Armor - (need add max stack of 24575 Brittle Armor)
            case 29284:
            {
                // Brittle Armor
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(24575);
                if (!spell)
                    return;

                for (uint32 j = 0; j < spell->AuraOptions.CumulativeAura; ++j)
                    m_caster->CastSpell(unitTarget, spell->Id, true);
                return;
            }
        }
    }

    switch (triggered_spell_id)
    {
        case 73603: //Kaja'Cola Gives You IDEAS! (TM): Summon Assistant Greely
        case 73607:
            return;
        default:
            break;
    }

    // normal case
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);
    if (!spellInfo)
    {
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectTriggerSpell spell %u tried to trigger unknown spell %u", m_spellInfo->Id, triggered_spell_id);
        return;
    }

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectTriggerSpell spell %u tried to trigger spell %u effectHandleMode %u Need %u TargetMask %u", m_spellInfo->Id, triggered_spell_id, effectHandleMode, spellInfo->NeedsToBeTriggeredByCaster(), spellInfo->GetExplicitTargetMask());

    SpellCastTargets targets;
    if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
    {
        if (!spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, m_caster->GetMap()->GetDifficultyID()))
            return;
        targets.SetUnitTarget(unitTarget);
    }
    else //if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH)
    {
        if (spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, m_caster->GetMap()->GetDifficultyID()) && (m_spellInfo->GetEffect(effIndex, m_diffMode)->GetProvidedTargetMask() & TARGET_FLAG_UNIT_MASK))
            return;

        if (spellInfo->GetExplicitTargetMask() & TARGET_FLAG_DEST_LOCATION && targets.HasDst())
        {
            if (m_targets.HasDst())
                targets.SetDst(m_targets);
            else
                targets.SetDst(*m_caster);
        }

        targets.SetUnitTarget(m_caster);
    }

    CustomSpellValues values;
    // set basepoints for trigger with value effect
    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE)
    {
        // maybe need to set value only when basepoints == 0?
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
    }

    // Remove spell cooldown (not category) if spell triggering spell with cooldown and same category
    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_spellInfo->CategoryRecoveryTime && spellInfo->CategoryRecoveryTime
        && m_spellInfo->Categories.Category == spellInfo->Categories.Category)
        m_caster->ToPlayer()->RemoveSpellCooldown(spellInfo->Id);

    // Hack. Lana'thel Vampiric Bite
    if (m_spellInfo->Id == 71726 || m_spellInfo->Id == 70946)
    {
        unitTarget->CastSpell(unitTarget, 70867, false);
        return;
    }

    switch (triggered_spell_id)
    {
        case 41637: // Prayer of Mending
            values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
            values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
            values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
        default:
            break;
    }

    // original caster guid only for GO cast
    m_caster->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK, NULL, NULL, m_originalCasterGUID);
}

void Spell::EffectTriggerMissileSpell(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;

    switch (m_spellInfo->Id)
    {
        case 146364: // OO: Throw Bomb
        {
            if (Aura * aura = m_caster->GetAura(145987))
            {
                uint8 stack = aura->GetStackAmount();
                if (stack > 1)
                {
                    --stack;
                    aura->SetStackAmount(stack);
                }
                else
                {
                    aura->Remove();
                    m_caster->RemoveAura(146364);
                }
            }
            triggered_spell_id = 146365;
            break;
        }
        default:
            break;
    }

    // normal case
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);
    if (!spellInfo)
    {
        SpellDummyTriggered(effIndex);
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectTriggerMissileSpell spell %u tried to trigger unknown spell %u effectHandleMode %u", m_spellInfo->Id, triggered_spell_id, effectHandleMode);
        return;
    }

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectTriggerMissileSpell spell %u trigger %u effectHandleMode %i unitTarget %i TargetMask %i TargetCount %i",
    //m_spellInfo->Id, triggered_spell_id, effectHandleMode, unitTarget ? unitTarget->GetGUIDLow() : 0, m_spellInfo->GetEffect(effIndex, m_diffMode)->GetProvidedTargetMask(), GetTargetCount());

    SpellCastTargets targets;
    if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
    {
        if (!spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, m_caster->GetMap()->GetDifficultyID()))
            return;
        targets.SetUnitTarget(unitTarget);
    }
    else //if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT)
    {
        if (spellInfo->NeedsToBeTriggeredByCaster(m_spellInfo, m_caster->GetMap()->GetDifficultyID()))
            return;

        if (spellInfo->GetExplicitTargetMask() & TARGET_FLAG_DEST_LOCATION)
            targets.SetDst(m_targets);
        else
            targets.SetUnitTarget(m_caster);
    }

    CustomSpellValues values;
    // set basepoints for trigger with value effect
    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_TRIGGER_MISSILE_SPELL_WITH_VALUE)
    {
        // maybe need to set value only when basepoints == 0?
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
    }

    // Remove spell cooldown (not category) if spell triggering spell with cooldown and same category
    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_spellInfo->CategoryRecoveryTime && spellInfo->CategoryRecoveryTime
        && m_spellInfo->Categories.Category == spellInfo->Categories.Category)
        m_caster->ToPlayer()->RemoveSpellCooldown(spellInfo->Id);

    // original caster guid only for GO cast
    m_caster->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK, NULL, NULL, m_originalCasterGUID);
}

void Spell::EffectForceCast(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;

    // normal case
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell::EffectForceCast of spell %u: triggering unknown spell id %i", m_spellInfo->Id, triggered_spell_id);
        return;
    }

    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_FORCE_CAST && damage)
    {
        switch (m_spellInfo->Id)
        {
            case 52588: // Skeletal Gryphon Escape
            case 48598: // Ride Flamebringer Cue
                unitTarget->RemoveAura(damage);
                break;
            case 52463: // Hide In Mine Car
            case 52349: // Overtake
                unitTarget->CastCustomSpell(unitTarget, spellInfo->Id, &damage, NULL, NULL, true, NULL, NULL, m_originalCasterGUID);
                return;
        }
    }

    CustomSpellValues values;
    // set basepoints for trigger with value effect
    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_FORCE_CAST_WITH_VALUE)
    {
        // maybe need to set value only when basepoints == 0?
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, damage);
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, damage);
    }

    SpellCastTargets targets;
    targets.SetUnitTarget(m_caster);

    unitTarget->CastSpell(targets, spellInfo, &values, TRIGGERED_FULL_MASK);
}

void Spell::EffectTriggerRitualOfSummoning(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "EffectTriggerRitualOfSummoning of spell %u: triggering unknown spell id %i", m_spellInfo->Id, triggered_spell_id);
        return;
    }

    finish();

    m_caster->CastSpell((Unit*)NULL, spellInfo, false);
}

void Spell::EffectJump(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    if (m_caster->isInFlight())
        return;

    if (!unitTarget || m_caster == unitTarget)
        return;

    DelayCastEvent *delayCast = NULL;
    //Perfome trigger spell at jumping.
    if (uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell)
        delayCast = new DelayCastEvent(ObjectGuid::Empty, unitTarget->GetGUID(), triggered_spell_id);

    //Fix creature to move back old pos
    if (m_caster->ToCreature())
        m_caster->GetMotionMaster()->Clear(false);

    float x, y, z;
    unitTarget->GetContactPoint(m_caster, x, y, z, CONTACT_DISTANCE);


    float speedXY, speedZ;
    float distance = m_caster->GetExactDist(x, y, z);
    CalculateJumpSpeeds(effIndex, distance, speedXY, speedZ);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "EffectJump start xyz %f %f %f caster %u target %u damage %i distance %f",
    x, y, z, m_caster->GetGUIDLow(), unitTarget->GetGUIDLow(), damage, distance);

    m_caster->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ, 0, 0.0f, delayCast, unitTarget);
}

void Spell::EffectJumpDest(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    if (m_caster->isInFlight())
        return;

    if (!m_targets.HasDst())
        return;

    DelayCastEvent *delayCast = NULL;
    //Perfome trigger spell at jumping.
    if (uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell)
    {
        Unit* pTarget = m_targets.GetUnitTarget();
        delayCast = new DelayCastEvent(ObjectGuid::Empty, pTarget ? pTarget->GetGUID() : ObjectGuid::Empty, triggered_spell_id);
    }

    // Init dest coordinates
    float x, y, z, o = 0.0f;
    destTarget->GetPosition(x, y, z);

    if (m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_DEST_TARGET_BACK)
    {
        Unit* pTarget = NULL;
        if (m_targets.GetUnitTarget() && m_targets.GetUnitTarget() != m_caster)
            pTarget = m_targets.GetUnitTarget();
        else if (m_caster->getVictim())
            pTarget = m_caster->getVictim();
        else if (m_caster->GetTypeId() == TYPEID_PLAYER)
            pTarget = ObjectAccessor::GetUnit(*m_caster, m_caster->ToPlayer()->GetSelection());

        o = pTarget ? pTarget->GetOrientation() : m_caster->GetOrientation();
    }

    if (Player* plr = m_caster->ToPlayer())
        plr->SetFallInformation(0, z);


    float speedXY, speedZ;
    float distance = m_caster->GetExactDist(x, y, z);
    CalculateJumpSpeeds(effIndex, distance, speedXY, speedZ);

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "EffectJumpDest start xyz %f %f %f o %f distance %f distance2d %f", x, y, z, o, distance, distance2d);

    // Death Grip and Wild Charge (no form)
    m_caster->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ, m_spellInfo->Id, o, delayCast);
}

void Spell::CalculateJumpSpeeds(uint8 i, float dist, float & speedXY, float & speedZ)
{
    if (m_spellInfo->GetEffect(i, m_diffMode)->MiscValue)
        speedZ = float(m_spellInfo->GetEffect(i, m_diffMode)->MiscValue) / 10;
    else if (m_spellInfo->GetEffect(i, m_diffMode)->MiscValueB)
        speedZ = float(m_spellInfo->GetEffect(i, m_diffMode)->MiscValueB) / 10;
    else
        speedZ = 10.0f;

    speedXY = dist * 10.0f / speedZ;

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "CalculateJumpSpeeds speedZ %f dist %f speedXY %f", speedZ, dist, speedXY);

    if (m_spellInfo->Id == 49575)
        speedXY = 38;
}

void Spell::EffectTeleportUnits(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->isInFlight())
        return;

    // Pre effects
    uint8 uiMaxSafeLevel = 0;
    switch (m_spellInfo->Id)
    {
        case 48129:  // Scroll of Recall
            uiMaxSafeLevel = 40;
        case 60320:  // Scroll of Recall II
            if (!uiMaxSafeLevel)
                uiMaxSafeLevel = 70;
        case 60321:  // Scroll of Recal III
            if (!uiMaxSafeLevel)
                uiMaxSafeLevel = 80;

            if (unitTarget->getLevel() > uiMaxSafeLevel)
            {
                unitTarget->AddAura(60444, unitTarget); //Apply Lost! Aura

                // ALLIANCE from 60323 to 60330 - HORDE from 60328 to 60335

                uint32 spellId = 60323;
                if (m_caster->ToPlayer()->GetTeam() == HORDE)
                    spellId += 5;
                spellId += urand(0, 7);
                m_caster->CastSpell(m_caster, spellId, true);
                return;
            }
            break;
        case 148705: // Teleport Banishment(Ordos Palace)
        {
            if (Player* caster = m_caster->ToPlayer())
            {
                if (caster->isGameMaster())
                    return;

                if (caster->GetQuestStatus(33104) == QUEST_STATUS_REWARDED || caster->HasAchieved(8325))
                    return;
            }
            break;
        }
    }

    // If not exist data for dest location - return
    if (!m_targets.HasDst())
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell::EffectTeleportUnits - does not have destination for spell ID %u\n", m_spellInfo->Id);
        return;
    }

    // Init dest coordinates
    uint32 mapid = destTarget->GetMapId();
    if (mapid == MAPID_INVALID)
        mapid = unitTarget->GetMapId();
    float x, y, z, orientation;
    destTarget->GetPosition(x, y, z, orientation);
    if (!orientation && m_targets.GetUnitTarget())
        orientation = m_targets.GetUnitTarget()->GetOrientation();
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectTeleportUnits - teleport unit to %u %f %f %f %f targetGuid %u\n", mapid, x, y, z, orientation, unitTarget->GetGUIDLow());

    if (mapid == unitTarget->GetMapId())
        unitTarget->NearTeleportTo(x, y, z, orientation, unitTarget == m_caster);
    else if (unitTarget->GetTypeId() == TYPEID_PLAYER)
        unitTarget->ToPlayer()->TeleportTo(mapid, x, y, z, orientation, unitTarget == m_caster ? TELE_TO_SPELL : 0, m_spellInfo->Id);

    // post effects for TARGET_DEST_DB
    switch (m_spellInfo->Id)
    {
        case 66550: // Teleport outside (Isle of Conquest)
        case 66551: // Teleport inside (Isle of Conquest)
        {
            if (Creature* teleportTarget = m_caster->FindNearestCreature((m_spellInfo->Id == 66550 ? 23472 : 22515), 35.0f, true))
            {
                float x, y, z, o;
                teleportTarget->GetPosition(x, y, z, o);

                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    m_caster->ToPlayer()->TeleportTo(628, x, y, z, o);
            }
            break;
        }
        case 54643: // Telepot inside (Strand of the Ancients)
        {
            if (Creature* teleportTarget = m_caster->FindNearestCreature(23472, 35.0f, true))
            {
                float x, y, z, o;
                teleportTarget->GetPosition(x, y, z, o);

                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    m_caster->ToPlayer()->TeleportTo(teleportTarget->GetMapId(), x, y, z, o);
            }
            break;
        }
        // Dimensional Ripper - Everlook
        case 23442:
        {
            int32 r = irand(0, 119);
            if (r >= 70)                                  // 7/12 success
            {
                if (r < 100)                              // 4/12 evil twin
                    m_caster->CastSpell(m_caster, 23445, true);
                else                                        // 1/12 fire
                    m_caster->CastSpell(m_caster, 23449, true);
            }
            return;
        }
        // Ultra safe Transporter: Toshley's Station
        case 36941:
        {
            if (roll_chance_i(50))                        // 50% success
            {
                int32 rand_eff = urand(1, 7);
                switch (rand_eff)
                {
                    case 1:
                        // soul split - evil
                        m_caster->CastSpell(m_caster, 36900, true);
                        break;
                    case 2:
                        // soul split - good
                        m_caster->CastSpell(m_caster, 36901, true);
                        break;
                    case 3:
                        // Increase the size
                        m_caster->CastSpell(m_caster, 36895, true);
                        break;
                    case 4:
                        // Decrease the size
                        m_caster->CastSpell(m_caster, 36893, true);
                        break;
                    case 5:
                        // Transform
                    {
                        if (m_caster->ToPlayer()->GetTeam() == ALLIANCE)
                            m_caster->CastSpell(m_caster, 36897, true);
                        else
                            m_caster->CastSpell(m_caster, 36899, true);
                        break;
                    }
                    case 6:
                        // chicken
                        m_caster->CastSpell(m_caster, 36940, true);
                        break;
                    case 7:
                        // evil twin
                        m_caster->CastSpell(m_caster, 23445, true);
                        break;
                }
            }
            return;
        }
        // Dimensional Ripper - Area 52
        case 36890:
        {
            if (roll_chance_i(50))                        // 50% success
            {
                int32 rand_eff = urand(1, 4);
                switch (rand_eff)
                {
                    case 1:
                        // soul split - evil
                        m_caster->CastSpell(m_caster, 36900, true);
                        break;
                    case 2:
                        // soul split - good
                        m_caster->CastSpell(m_caster, 36901, true);
                        break;
                    case 3:
                        // Increase the size
                        m_caster->CastSpell(m_caster, 36895, true);
                        break;
                    case 4:
                        // Transform
                    {
                        if (m_caster->ToPlayer()->GetTeam() == ALLIANCE)
                            m_caster->CastSpell(m_caster, 36897, true);
                        else
                            m_caster->CastSpell(m_caster, 36899, true);
                        break;
                    }
                }
            }
            return;
        }
    }
    // Glyph of Rapid Teleportation 
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_MAGE && m_spellInfo->SpellFamilyFlags[0] & 0x80000000 && m_caster->HasAura(89749))
        m_caster->CastSpell(m_caster, 46989, true);
}

void Spell::EffectApplyAura(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!m_spellAura || !unitTarget)
        return;

    switch (m_spellAura->GetId())
    {
        case 38177:
            if (unitTarget->GetEntry() != 21387)
                return;
            break;
        case 91836:
            m_caster->RemoveAurasDueToSpell(91832);
            break;
        case 51755: // Camouflage
        {
            if (m_caster->isInCombat())
                m_spellAura->SetDuration(6000);
            break;
        }
        case 42292:
        {
            if (m_caster->ToPlayer() && m_caster->ToPlayer()->GetTeam())
            {
                int32 visual = m_caster->ToPlayer()->GetTeam() == ALLIANCE ? 97403 : 97404;
                m_caster->CastSpell(m_caster, visual, true);
            }
            break;
        }
    }

    ASSERT(unitTarget == m_spellAura->GetOwner());
    m_spellAura->_ApplyEffectForTargets(effIndex);
    if (_triggeredCastFlags & TRIGGERED_CASTED_BY_AREATRIGGER)
        m_spellAura->SetFromAreaTrigger(true);
}

void Spell::EffectApplyAreaAura(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!m_spellAura || !unitTarget)
        return;
    ASSERT(unitTarget == m_spellAura->GetOwner());
    m_spellAura->_ApplyEffectForTargets(effIndex);
    if (_triggeredCastFlags & TRIGGERED_CASTED_BY_AREATRIGGER)
        m_spellAura->SetFromAreaTrigger(true);
}

void Spell::EffectUnlearnSpecialization(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();
    uint32 spellToUnlearn = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;

    player->removeSpell(spellToUnlearn);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell: Player %u has unlearned spell %u from NpcGUID: %u", player->GetGUIDLow(), spellToUnlearn, m_caster->GetGUIDLow());
}

void Spell::EffectPowerDrain(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue < 0 || m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue >= int8(MAX_POWERS))
        return;

    Powers powerType = Powers(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);

    if (!unitTarget || !unitTarget->isAlive() || unitTarget->getPowerType() != powerType || damage < 0)
        return;

    // add spell damage bonus
    uint32 bonus = m_caster->SpellDamageBonusDone(unitTarget, m_spellInfo, (uint32)damage, SPELL_DIRECT_DAMAGE, effIndex);
    damage = bonus + uint32(bonus * variance);

    int32 newDamage = -(unitTarget->ModifyPower(powerType, -damage));

    float gainMultiplier = 0.0f;

    // Don't restore from self drain
    if (m_caster != unitTarget)
    {
        gainMultiplier = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValueMultiplier(m_originalCaster, this);
        int32 gain = int32(newDamage* gainMultiplier);
        m_caster->EnergizeBySpell(m_caster, m_spellInfo->Id, gain, powerType);
    }
    ExecuteLogEffectTakeTargetPower(effIndex, unitTarget, powerType, newDamage, gainMultiplier);
}

void Spell::EffectSendEvent(SpellEffIndex effIndex)
{
    // we do not handle a flag dropping or clicking on flag in battleground by sendevent system
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET
        && effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    WorldObject* target = NULL;

    // call events for object target if present
    if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
    {
        if (unitTarget)
            target = unitTarget;
        else if (gameObjTarget)
            target = gameObjTarget;
    }
    else // if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT)
    {
        // let's prevent executing effect handler twice in case when spell effect is capable of targeting an object
        // this check was requested by scripters, but it has some downsides:
        // now it's impossible to script (using sEventScripts) a cast which misses all targets
        // or to have an ability to script the moment spell hits dest (in a case when there are object targets present)
        if (m_spellInfo->GetEffect(effIndex, m_diffMode)->GetProvidedTargetMask() & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_GAMEOBJECT_MASK))
            return;
        // some spells have no target entries in dbc and they use focus target
        if (focusObject)
            target = focusObject;
        // TODO: there should be a possibility to pass dest target to event script
    }

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell ScriptStart %u for spellid %u in EffectSendEvent ", m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, m_spellInfo->Id);

    if (ZoneScript* zoneScript = m_caster->GetZoneScript())
        zoneScript->ProcessEvent(target, m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
    else if (InstanceScript* instanceScript = m_caster->GetInstanceScript())    // needed in case Player is the caster
        instanceScript->ProcessEvent(target, m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);

    m_caster->GetMap()->ScriptsStart(sEventScripts, m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, m_caster, target);

    if (Player* player = m_caster->ToPlayer())
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT, m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, 1);
}

void Spell::EffectPowerBurn(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue < 0 || m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue >= int8(MAX_POWERS))
        return;

    Powers powerType = Powers(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);

    if (!unitTarget || !unitTarget->isAlive() || unitTarget->getPowerType() != powerType || damage < 0)
        return;

    // burn x% of target's mana, up to maximum of 2x% of caster's mana (Mana Burn)
    if (m_spellInfo->Id == 8129)
    {
        int32 maxDamage = int32(CalculatePct(m_caster->GetMaxPower(powerType), damage * 2));
        damage = int32(CalculatePct(unitTarget->GetMaxPower(powerType), damage));
        damage = std::min(damage, maxDamage);
    }

    int32 newDamage = -(unitTarget->ModifyPower(powerType, -damage, true));

    // NO - Not a typo - EffectPowerBurn uses effect value multiplier - not effect damage multiplier
    float dmgMultiplier = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValueMultiplier(m_originalCaster, this);

    // add log data before multiplication (need power amount, not damage)
    ExecuteLogEffectTakeTargetPower(effIndex, unitTarget, powerType, newDamage, 0.0f);

    newDamage = int32(newDamage* dmgMultiplier);

    m_damage += newDamage;
}

void Spell::EffectHeal(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    if (unitTarget && unitTarget->isAlive() && damage >= 0)
    {
        // Try to get original caster
        Unit* caster = m_originalCasterGUID ? m_originalCaster : m_caster;

        // Skip if m_originalCaster not available
        if (!caster)
            return;

        if (m_spellInfo->AttributesCu & SPELL_ATTR0_CU_SHARE_DAMAGE)
        {
            uint32 count = 0;
            for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                if (ihit->effectMask & (1 << effIndex))
                    ++count;

            damage /= count;                    // divide to all targets
        }

        int32 addhealth = damage;

        switch (m_spellInfo->Id)
        {
            case 115072: // Expel Harm
            case 147489: // Expel Harm
            {
                SpellInfo const* _triggerInfo = sSpellMgr->GetSpellInfo(115129);
                int32 bp = CalculatePct(addhealth, _triggerInfo->Effects[1].BasePoints);
                m_caster->CastCustomSpell(unitTarget, 115129, &bp, NULL, NULL, true);
                break;
            }
            case 52042: // Glyph of Healing Stream Totem
            {
                if (m_caster->ToTotem()->GetOwner()->HasAura(55456))
                {
                    m_caster->CastSpell(unitTarget, 119523, true);
                }
                break;
            }
            // Spirit Mend
            case 90361:
            {
                uint32 ap = m_caster->GetTotalAttackPowerValue(BASE_ATTACK) * 2;
                uint32 spd = m_caster->SpellDamageBonusDone(m_caster, m_spellInfo, damage, SPELL_DIRECT_DAMAGE, effIndex);
                addhealth += int32((ap * 0.35f) * 0.5f) + spd;
                break;
            }
            case 45064: // Vessel of the Naaru (Vial of the Sunwell trinket)
            {
                int damageAmount = 0; // Amount of heal - depends from stacked Holy Energy
                if (AuraEffect const* aurEff = m_caster->GetAuraEffect(45062, 0))
                {
                    damageAmount += aurEff->GetAmount();
                    m_caster->RemoveAurasDueToSpell(45062);
                }
                addhealth += damageAmount;
                break;
            }
            case 67489: // Runic Healing Injector (heal increased by 25% for engineers - 3.2.0 patch change)
            {
                if (Player* player = m_caster->ToPlayer())
                    if (player->HasSkill(SKILL_ENGINEERING))
                        AddPct(addhealth, 25);
                break;
            }
            default:
                break;
        }
        // Death Pact - return pct of max health to caster
        if (m_spellInfo->SpellFamilyName == SPELLFAMILY_DEATHKNIGHT && m_spellInfo->SpellFamilyFlags[0] & 0x00080000)
            addhealth = caster->SpellHealingBonusDone(unitTarget, m_spellInfo, int32(caster->CountPctFromMaxHealth(damage)), HEAL, effIndex);
        else
        {
            addhealth = caster->SpellHealingBonusDone(unitTarget, m_spellInfo, addhealth, HEAL, effIndex);
            uint32 bonus = caster->SpellHealingBonusDone(unitTarget, m_spellInfo, addhealth, HEAL, effIndex);
            damage = bonus + uint32(bonus * variance);
        }

        addhealth = unitTarget->SpellHealingBonusTaken(caster, m_spellInfo, addhealth, HEAL, effIndex);

        // Mastery DeathKnight
        if (m_spellInfo->Id == 45470 && m_caster->HasAura(48263))
            if (Aura* aur = m_caster->GetAura(77513))
            {
                int32 bp0 = int32(addhealth * float(aur->GetEffect(0)->GetAmount() / 100.0f));
                if (Aura* aurShield = m_caster->GetAura(77535))
                    bp0 += aurShield->GetEffect(0)->GetAmount();

                if (bp0 > int32(m_caster->GetMaxHealth()))
                    bp0 = int32(m_caster->GetMaxHealth());

                m_caster->CastCustomSpell(m_caster, 77535, &bp0, NULL, NULL, true);
            }

        switch (m_spellInfo->Id)
        {
            case 148009: // Spirit of Chi-Ji
            {
                addhealth /= m_UniqueTargetInfo.size();
                break;
            }
            case 130551: // Word of Glory
            case 114163: // Eternal Flame
            {
                if (unitTarget == caster)
                    if (Aura* Bastion_of_Glory = caster->GetAura(114637))
                        if (AuraEffect* eff = Bastion_of_Glory->GetEffect(EFFECT_0))
                            AddPct(addhealth, eff->GetAmount());
                break;
            }
            case 89653: // Drain Life
            {
                if (Aura * aura = caster->GetAura(108371))
                {
                    AddPct(addhealth, aura->GetSpellInfo()->Effects[1].BasePoints);
                }
                break;
            }
            case 19750: // Selfless Healer - Increases heal of Flash of Light if it heals an other player than you
            {
                if (Aura* selflessHealer = caster->GetAura(114250))
                {
                    int32 perc = 0;

                    Player* player = caster->ToPlayer();
                    if (!player)
                        break;

                    if (player->GetSpecializationId(player->GetActiveSpec()) != SPEC_PALADIN_HOLY)
                    {
                        if (AuraEffect* eff = selflessHealer->GetEffect(EFFECT_1))
                            perc = eff->GetAmount();
                    }
                    else if (player->GetSpecializationId(player->GetActiveSpec()) == SPEC_PALADIN_HOLY)
                    {
                        if (AuraEffect* eff = selflessHealer->GetEffect(EFFECT_3))
                            perc = eff->GetAmount();
                    }

                    if (perc && unitTarget->GetGUID() != caster->GetGUID())
                        AddPct(addhealth, perc);
                    else if (unitTarget->GetGUID() == caster->GetGUID())
                    {
                        if (Aura* bastion = caster->GetAura(114637))
                            if (AuraEffect* bastionEff = bastion->GetEffect(EFFECT_2))
                            {
                                AddPct(addhealth, bastionEff->GetAmount());
                                bastion->Remove();
                            }
                    }
                }
                break;
            }
            default:
                break;
        }

        // Remove Grievous bite if fully healed
        if (unitTarget->HasAura(48920) && (unitTarget->GetHealth() + addhealth >= unitTarget->GetMaxHealth()))
            unitTarget->RemoveAura(48920);

        // Chakra : Serenity - 81208
        if (m_caster && addhealth && m_caster->HasAura(81208) && m_spellInfo->Effects[0].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY) // Single heal target
            if (Aura* renew = unitTarget->GetAura(139, m_caster->GetGUID()))
                renew->RefreshDuration();

        // Mogu'Shan Vault
        if (caster->HasAura(116161) || unitTarget->HasAura(116161)) // SPELL_CROSSED_OVER
        {
            // http://fr.wowhead.com/spell=117549#english-comments
            // uint32 targetSpec = unitTarget->ToPlayer()->GetSpecializationId(unitTarget->ToPlayer()->GetActiveSpec());
            uint32 innervationId = 0;

            if (unitTarget == caster)
            {
                int32 bp1 = addhealth / 2;
                int32 bp2 = 15;

                caster->CastCustomSpell(unitTarget, 117543, &bp1, &bp2, NULL, NULL, NULL, NULL, true); // Mana regen bonus
            }
            else
            {
                int32 bp1 = 10;
                int32 bp2 = 15;
                int32 bp3 = 20;
                int32 bp4 = 25;
                int32 bp5 = 30;
                int32 bp6 = 35;

                caster->CastCustomSpell(unitTarget, 117549, &bp1, &bp2, &bp3, &bp4, &bp5, &bp6, true);
            }

            if (unitTarget->GetHealth() + addhealth >= unitTarget->GetMaxHealth())
                unitTarget->CastSpell(unitTarget, 120717, true);  // Revitalized Spirit
        }

        m_damage -= addhealth;
        //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectHeal m_damage %i addhealth %i damage %i", m_damage, addhealth, damage);
    }
}

void Spell::EffectHealPct(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive() || damage < 0)
        return;

    // Skip if m_originalCaster not available
    if (!m_originalCaster)
        return;

    uint32 divider = 1;

    if (m_spellInfo->HasAttribute(SPELL_ATTR11_UNK4))
    {
        bool resetHeal = true;
        Unit::AuraEffectList const& mTotalAuraList = m_caster->GetAuraEffectsByType(SPELL_AURA_DUMMY);
        for (Unit::AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
            if ((*i)->GetMiscValue() == 11)
                if ((*i)->GetAmount() == m_spellInfo->Id)
                {
                    (*i)->SetAmount(NULL);
                    resetHeal = false;
                }

        if (resetHeal)
            damage = 0;
    }

    switch (m_spellInfo->Id)
    {
        case 63106: // Siphon Life
            divider = 1000;
            break;
        case 53353: // Chimera Shot - Heal
            if (m_caster->HasAura(119447)) // Glyph of Chimera Shot
                damage += 2;
            break;
        case 114635:  // Ember Tap
        {
            // Mastery: Emberstorm
            if (AuraEffect const* aurEff = m_caster->GetAuraEffect(77220, EFFECT_0))
                AddPct(damage, aurEff->GetAmount());
            break;
        }
        case 118340:// Impending Victory - Heal
            // Victorious State causes your next Impending Victory to heal for 20% of your maximum health.
            if (m_caster->HasAura(32216))
            {
                damage = 20;
                m_caster->RemoveAurasDueToSpell(32216);
            }
            if (m_caster->HasAura(138279))
            {
                damage *= 2;
                m_caster->RemoveAurasDueToSpell(138279);
            }
            break;
        case 118779: // Victory Rush
            if (m_caster->HasAura(58382)) // Glyph of Victory Rush
            {
                damage += 10;
            }
            if (m_caster->HasAura(138279))
            {
                //damage *= 2;
                m_caster->RemoveAurasDueToSpell(138279);
            }
            break;
        default:
            break;
    }

    uint32 heal = unitTarget->CountPctFromMaxHealth(damage);

    switch (m_spellInfo->Id)
    {
        case 149254: // Spirit Bond
            heal = m_originalCaster->CalcVersalityBonusDone(unitTarget, heal);
        default:
            break;
    }

    if (damage)
    {
        heal = m_originalCaster->SpellHealingBonusDone(unitTarget, m_spellInfo, heal, HEAL, effIndex);
        heal = unitTarget->SpellHealingBonusTaken(m_originalCaster, m_spellInfo, heal, HEAL, effIndex);
    }

    heal /= divider;
    m_healing += heal;
}

void Spell::EffectHealMechanical(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive() || damage < 0)
        return;

    // Skip if m_originalCaster not available
    if (!m_originalCaster)
        return;

    uint32 heal = m_originalCaster->SpellHealingBonusDone(unitTarget, m_spellInfo, uint32(damage), HEAL, effIndex);
    heal += uint32(heal * variance);

    m_healing += unitTarget->SpellHealingBonusTaken(m_originalCaster, m_spellInfo, heal, HEAL, effIndex);
}

void Spell::EffectHealthLeech(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive() || damage < 0)
        return;

    uint32 bonus = m_caster->SpellDamageBonusDone(unitTarget, m_spellInfo, (uint32)damage, SPELL_DIRECT_DAMAGE, effIndex);
    damage = bonus + uint32(bonus * variance);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "HealthLeech :%i", damage);

    float healMultiplier = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValueMultiplier(m_originalCaster, this);

    m_damage += damage;
    // get max possible damage, don't count overkill for heal
    uint32 healthGain = uint32(-unitTarget->GetHealthGain(-damage) * healMultiplier);

    if (m_caster->isAlive())
    {
        healthGain = m_caster->SpellHealingBonusDone(m_caster, m_spellInfo, healthGain, HEAL, effIndex);
        healthGain = m_caster->SpellHealingBonusTaken(m_caster, m_spellInfo, healthGain, HEAL, effIndex);

        m_caster->HealBySpell(m_caster, m_spellInfo, uint32(healthGain));
    }
}

void Spell::DoCreateItem(uint32 /*i*/, uint32 itemtype, std::vector<int32> const& bonusListIDs)
{
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();

    uint32 newitemid = itemtype;
    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(newitemid);
    if (!pProto)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    // bg reward have some special in code work
    uint32 bgType = 0;
    switch (m_spellInfo->Id)
    {
        case SPELL_AV_MARK_WINNER:
        case SPELL_AV_MARK_LOSER:
            bgType = BATTLEGROUND_AV;
            break;
        case SPELL_WS_MARK_WINNER:
        case SPELL_WS_MARK_LOSER:
            bgType = BATTLEGROUND_WS;
            break;
        case SPELL_AB_MARK_WINNER:
        case SPELL_AB_MARK_LOSER:
            bgType = BATTLEGROUND_AB;
            break;
        default:
            break;
    }

    uint32 num_to_add = damage;

    if (num_to_add < 1)
        num_to_add = 1;
    if (num_to_add > pProto->GetMaxStackSize())
        num_to_add = pProto->GetMaxStackSize();

    // init items_count to 1, since 1 item will be created regardless of specialization
    int items_count = 1;
    // the chance to create additional items
    float additionalCreateChance = 0.0f;
    // the maximum number of created additional items
    uint8 additionalMaxNum = 0;
    // get the chance and maximum number for creating extra items
    if (canCreateExtraItems(player, m_spellInfo->Id, additionalCreateChance, additionalMaxNum))
    {
        // roll with this chance till we roll not to create or we create the max num
        while (roll_chance_f(additionalCreateChance) && items_count <= additionalMaxNum)
            ++items_count;
    }

    // really will be created more items
    num_to_add *= items_count;

    // can the player store the new item?
    ItemPosCountVec dest;
    uint32 no_space = 0;
    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, newitemid, num_to_add, &no_space);
    if (msg != EQUIP_ERR_OK)
    {
        // convert to possible store amount
        if (msg == EQUIP_ERR_INV_FULL || msg == EQUIP_ERR_ITEM_MAX_COUNT)
            num_to_add -= no_space;
        else
        {
            // if not created by another reason from full inventory or unique items amount limitation
            player->SendEquipError(msg, NULL, NULL, newitemid);
            return;
        }
    }

    if (num_to_add)
    {
        // create the new item and store it
        Item* pItem = player->StoreNewItem(dest, newitemid, true, Item::GenerateItemRandomPropertyId(newitemid), GuidSet(), bonusListIDs);

        if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
        {
            if (pProto->Quality > ITEM_QUALITY_EPIC || (pProto->Quality == ITEM_QUALITY_EPIC && pProto->ItemLevel >= MinNewsItemLevel[CURRENT_EXPANSION]))
                if (!pProto->IsNotAppearInGuildNews())
                    guild->AddGuildNews(GUILD_NEWS_ITEM_CRAFTED, player->GetGUID(), 0, pProto->ItemId);

            guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD, pProto->ItemId, num_to_add, 0, NULL, player);
        }

        // was it successful? return error if not
        if (!pItem)
        {
            player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
            return;
        }

        if (pItem->GetEntry() == 38186)
            sLog->outDebug(LOG_FILTER_EFIR, "DoCreateItem - StoreNewItem of item %u; 1 = %u playerGUID %u, itemGUID %u spellId %u", pItem->GetEntry(), 1, player->GetGUID(), pItem->GetGUID(), m_spellInfo->Id);

        // set the "Crafted by ..." property of the item
        if (pItem->GetTemplate()->Class != ITEM_CLASS_CONSUMABLE && pItem->GetTemplate()->Class != ITEM_CLASS_QUEST && newitemid != 6265 && newitemid != 6948)
            pItem->SetUInt32Value(ITEM_FIELD_CREATOR, player->GetGUIDLow());

        // send info to the client
        player->SendNewItem(pItem, num_to_add, true, bgType == 0);

        // we succeeded in creating at least one item, so a level up is possible
        if (bgType == 0)
            player->UpdateCraftSkill(m_spellInfo->Id);
    }

/*
    // for battleground marks send by mail if not add all expected
    if (no_space > 0 && bgType)
    {
        if (Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BattlegroundTypeId(bgType)))
            bg->SendRewardMarkByMail(player, newitemid, no_space);
    }
*/
}

void Spell::EffectCreateItem(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_spellInfo->IsArchaeologyCraftingSpell())
        if (!m_caster->ToPlayer()->SolveResearchProject(m_spellInfo->Id, m_targets))
            return;

    DoCreateItem(effIndex, m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType);
    ExecuteLogEffectCreateItem(effIndex, m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType);
}

void Spell::EffectDestroyItem(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    Player* player = unitTarget->ToPlayer();
    if (!player)
        return;

    player->DestroyItemCount(m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType, 1, true);
    //ExecuteLogEffectTradeSkillItem(effIndex, m_spellInfo->GetEffect(effIndex, m_diffMode).ItemType);
}

void Spell::EffectCreateItem2(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_spellInfo->IsArchaeologyCraftingSpell())
        if (!m_caster->ToPlayer()->SolveResearchProject(m_spellInfo->Id, m_targets))
            return;

    Player* player = unitTarget->ToPlayer();

    uint32 item_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType;

    if (item_id)
        DoCreateItem(effIndex, item_id);

    // special case: fake item replaced by generate using spell_loot_template
    if (m_spellInfo->IsLootCrafting())
    {
        if (item_id)
        {
            if (!player->HasItemCount(item_id))
                return;

            // create some random items
            if(player->AutoStoreLoot(m_spellInfo->Id, LootTemplates_Spell, m_CastItem ? m_CastItem->GetTemplate()->ItemLevel : 0))
            // remove reagent
                player->DestroyItemCount(item_id, 1, true);
        }
        else
        {
            player->AutoStoreLoot(m_spellInfo->Id, LootTemplates_Spell, m_CastItem ? m_CastItem->GetTemplate()->ItemLevel : 0);    // create some random items
            player->UpdateCraftSkill(m_spellInfo->Id);
        }
    }

    ExecuteLogEffectCreateItem(effIndex, m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType);
}

void Spell::EffectCreateItem3(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    Item* item = m_targets.GetItemTarget();
    if (!item)
        return;

    Player* player = m_caster->ToPlayer();
    if (item->GetEntry() && player)
    {
        if (m_spellInfo->IsLootCrafting())
        {
            if (player->AutoStoreLoot(item->GetTemplate()->Effects[0]->SpellID, LootTemplates_Spell, 535))
                player->DestroyItemCount(item->GetEntry(), 1, true);
        }
    }
}

void Spell::EffectCreateRandomItem(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    Player* player = unitTarget->ToPlayer();

    uint32 item_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType;

    if (item_id)
        DoCreateItem(effIndex, item_id);

    // create some random items
    player->AutoStoreLoot(m_spellInfo->Id, LootTemplates_Spell);
    ExecuteLogEffectCreateItem(effIndex, m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType);
}

void Spell::EffectPersistentAA(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (!m_spellAura)
    {
        Unit* caster = m_caster->GetEntry() == WORLD_TRIGGER ? m_originalCaster : m_caster;
        float radius = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius(caster);

        // Caster not in world, might be spell triggered from aura removal
        if (!caster->IsInWorld())
            return;
        DynamicObject* dynObj = new DynamicObject(false);
        if (!dynObj->CreateDynamicObject(sObjectMgr->GetGenerator<HighGuid::DynamicObject>()->Generate(), caster, m_spellInfo->Id, *destTarget, radius, DYNAMIC_OBJECT_AREA_SPELL))
        {
            delete dynObj;
            return;
        }

        SetSpellDynamicObject(dynObj->GetGUID());
        Aura* aura = Aura::TryCreate(m_spellInfo, MAX_EFFECT_MASK, dynObj, caster, &m_spellValue->EffectBasePoints[0]);
        if (aura != NULL)
        {
            m_spellAura = aura;
            m_spellAura->_RegisterForTargets();
        }
        else
            return;
    }

    ASSERT(m_spellAura->GetDynobjOwner());
    m_spellAura->_ApplyEffectForTargets(effIndex);
    if (_triggeredCastFlags & TRIGGERED_CASTED_BY_AREATRIGGER)
        m_spellAura->SetFromAreaTrigger(true);
}

void Spell::EffectEnergize(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;
    if (!unitTarget->isAlive())
        return;

    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue < 0 || m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue >= int8(MAX_POWERS))
        return;

    Powers power = Powers(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);

    if (unitTarget->GetTypeId() == TYPEID_PLAYER && unitTarget->getPowerType() != power && !(m_spellInfo->HasAttribute(SPELL_ATTR7_CAN_RESTORE_SECONDARY_POWER)))
        return;

    if (unitTarget->GetMaxPower(power) == 0)
        return;

    if (power == POWER_COMBO_POINTS) //Hack for used old CP system
    {
        EffectAddComboPoints(effIndex);
        return;
    }

    // Some level depends spells
    int level_multiplier = 0;
    int level_diff = 0;
    switch (m_spellInfo->Id)
    {
        case 23922:  // Shield Slam
        {
            m_caster->RemoveAura(50227);
            break;
        }
        case 1454: // Life Tap
        {
            damage = CalculatePct(m_caster->GetMaxHealth(), damage);
            break;
        }
        case 9512:                                          // Restore Energy
            level_diff = m_caster->getLevel() - 40;
            level_multiplier = 2;
            break;
        case 24571:                                         // Blood Fury
            level_diff = m_caster->getLevel() - 60;
            level_multiplier = 10;
            break;
        case 24532:                                         // Burst of Energy
            level_diff = m_caster->getLevel() - 60;
            level_multiplier = 4;
            break;
        case 33878:                                         // Mangle
            if (Player* player = m_caster->ToPlayer())
            {
                // check Soul of the Forest for guardian druids
                if (player->GetSpecializationId(player->GetActiveSpec()) == SPEC_DRUID_BEAR)
                    if (AuraEffect const* aura = player->GetAuraEffect(114107, EFFECT_2))
                        damage += aura->GetAmount() * 10;
            }
            break;
        case 67490:                                         // Runic Mana Injector (mana gain increased by 25% for engineers - 3.2.0 patch change)
        {
            if (Player* player = m_caster->ToPlayer())
                if (player->HasSkill(SKILL_ENGINEERING))
                    AddPct(damage, 25);
            break;
        }
        default:
            break;
    }

    if (level_diff > 0)
        damage -= level_multiplier * level_diff;

    // now alter power used as mana too as it's some kind of category not power.
    //if (damage < 0 && power != POWER_LUNAR_POWER && power != POWER_ALTERNATE_POWER)
    //    return;

    // Do not energize when in Celestial Alignment
    //if (power == POWER_LUNAR_POWER && m_caster->HasAura(112071))
        //return;

    if (power == POWER_RAGE && m_caster->HasAura(138222) && unitTarget->HasAura(5229)) // Item - Druid T15 Guardian 4P Bonus
        damage *= 1.5;

    if (unitTarget->HasAura(143594)) //Berserker Stance - General Nazgrim
        damage *= 2;

    if (unitTarget->HasAura(143411)) //Acceleration - Thok Bloodthirsty
    {
        uint8 stack = unitTarget->GetAura(143411)->GetStackAmount();
        switch (stack)
        {
            case 1:
                damage += 1;
                break;
            case 2:
                damage += 4;
                break;
            case 3:
                damage += 11;
                break;
            case 4:
                damage += 14;
                break;
            case 5:
            case 6:
            case 7:
                damage += 21;
                break;
            case 8:
                damage += 46;
                break;
            default:
                damage += 46;
                break;
        }
    }

    if (unitTarget->GetMaxPower(power) == 0)
        return;

    m_addptype = power;
    m_addpower = damage;
    m_caster->EnergizeBySpell(unitTarget, m_spellInfo->Id, damage, power);

    // Mad Alchemist's Potion
    if (m_spellInfo->Id == 45051)
    {
        // find elixirs on target
        bool guardianFound = false;
        bool battleFound = false;
        Unit::AuraApplicationMap& Auras = unitTarget->GetAppliedAuras();
        for (Unit::AuraApplicationMap::iterator itr = Auras.begin(); itr != Auras.end(); ++itr)
        {
            uint32 spell_id = itr->second->GetBase()->GetId();
            if (!guardianFound)
                if (sSpellMgr->IsSpellMemberOfSpellGroup(spell_id, SPELL_GROUP_ELIXIR_GUARDIAN))
                    guardianFound = true;
            if (!battleFound)
                if (sSpellMgr->IsSpellMemberOfSpellGroup(spell_id, SPELL_GROUP_ELIXIR_BATTLE))
                    battleFound = true;
            if (battleFound && guardianFound)
                break;
        }

        // get all available elixirs by mask and spell level
        std::set<uint32> avalibleElixirs;
        if (!guardianFound)
            sSpellMgr->GetSetOfSpellsInSpellGroup(SPELL_GROUP_ELIXIR_GUARDIAN, avalibleElixirs);
        if (!battleFound)
            sSpellMgr->GetSetOfSpellsInSpellGroup(SPELL_GROUP_ELIXIR_BATTLE, avalibleElixirs);
        for (std::set<uint32>::iterator itr = avalibleElixirs.begin(); itr != avalibleElixirs.end();)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(*itr);
            if (spellInfo->SpellLevel < m_spellInfo->SpellLevel || spellInfo->SpellLevel > unitTarget->getLevel())
                avalibleElixirs.erase(itr++);
            else if (sSpellMgr->IsSpellMemberOfSpellGroup(*itr, SPELL_GROUP_ELIXIR_SHATTRATH))
                avalibleElixirs.erase(itr++);
            else if (sSpellMgr->IsSpellMemberOfSpellGroup(*itr, SPELL_GROUP_ELIXIR_UNSTABLE))
                avalibleElixirs.erase(itr++);
            else
                ++itr;
        }

        if (!avalibleElixirs.empty())
        {
            // cast random elixir on target
            m_caster->CastSpell(unitTarget, Trinity::Containers::SelectRandomContainerElement(avalibleElixirs), true, m_CastItem);
        }
    }
}

void Spell::EffectEnergizePct(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;
    if (!unitTarget->isAlive())
        return;

    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue < 0 || m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue >= int8(MAX_POWERS))
        return;

    Powers power = Powers(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);

    if (unitTarget->GetTypeId() == TYPEID_PLAYER && unitTarget->GetMaxPower(power) == 0 && !(m_spellInfo->HasAttribute(SPELL_ATTR7_CAN_RESTORE_SECONDARY_POWER)))
        return;

    uint32 maxPower = unitTarget->GetMaxPower(power);
    if (maxPower == 0)
        return;

    uint32 gain = CalculatePct(maxPower, damage);

    if (m_spellInfo->Id == 123051) // hack for Mana Leech 0.75% bp cannot now is float
        gain /= 100;

    m_addptype = power;
    m_addpower = gain;
    m_caster->EnergizeBySpell(unitTarget, m_spellInfo->Id, gain, power);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectEnergizePct Id %i damage %i power %i gain %i", m_spellInfo->Id, damage, power, gain);
}

void Spell::SendLoot(ObjectGuid const& guid, LootType loottype)
{
    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    if (gameObjTarget)
    {
        // Players shouldn't be able to loot gameobjects that are currently despawned
        if (!gameObjTarget->isSpawned() && !player->isGameMaster())
        {
            sLog->outError(LOG_FILTER_SPELLS_AURAS, "Possible hacking attempt: Player %s [guid: %u] tried to loot a gameobject [entry: %u id: %u] which is on respawn time without being in GM mode!",
                           player->GetName(), player->GetGUIDLow(), gameObjTarget->GetEntry(), gameObjTarget->GetGUIDLow());
            return;
        }
        // special case, already has GossipHello inside so return and avoid calling twice
        if (gameObjTarget->GetGoType() == GAMEOBJECT_TYPE_GOOBER)
        {
            gameObjTarget->Use(m_caster);
            return;
        }

        if (sScriptMgr->OnGossipHello(player, gameObjTarget))
            return;

        if (gameObjTarget->AI()->GossipHello(player))
            return;

        switch (gameObjTarget->GetGoType())
        {
            case GAMEOBJECT_TYPE_DOOR:
            case GAMEOBJECT_TYPE_BUTTON:
                gameObjTarget->UseDoorOrButton(0, false, player);
                player->GetMap()->ScriptsStart(sGameObjectScripts, gameObjTarget->GetDBTableGUIDLow(), player, gameObjTarget);
                return;

            case GAMEOBJECT_TYPE_QUESTGIVER:
                player->PrepareGossipMenu(gameObjTarget, gameObjTarget->GetGOInfo()->questgiver.gossipID);
                player->SendPreparedGossip(gameObjTarget);
                return;

            case GAMEOBJECT_TYPE_SPELL_FOCUS:
                // triggering linked GO
                if (uint32 trapEntry = gameObjTarget->GetGOInfo()->spellFocus.linkedTrap)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry, m_caster);
                return;

            case GAMEOBJECT_TYPE_CHEST:
                // TODO: possible must be moved to loot release (in different from linked triggering)
                if (gameObjTarget->GetGOInfo()->chest.triggeredEvent)
                {
                    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Chest ScriptStart id %u for GO %u", gameObjTarget->GetGOInfo()->chest.triggeredEvent, gameObjTarget->GetDBTableGUIDLow());
                    player->GetMap()->ScriptsStart(sEventScripts, gameObjTarget->GetGOInfo()->chest.triggeredEvent, player, gameObjTarget);
                }

                // triggering linked GO
                if (uint32 trapEntry = gameObjTarget->GetGOInfo()->chest.linkedTrap)
                    gameObjTarget->TriggeringLinkedGameObject(trapEntry, m_caster);

                // Don't return, let loots been taken
            default:
                break;
        }
    }

    // Send loot
    player->SendLoot(guid, loottype);
}

void Spell::EffectOpenLock(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
    {
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WORLD: Open Lock - No Player Caster!");
        return;
    }

    Player* player = m_caster->ToPlayer();

    uint32 lockId = 0;
    ObjectGuid guid;

    // Get lockId
    if (gameObjTarget)
    {
        GameObjectTemplate const* goInfo = gameObjTarget->GetGOInfo();
        // Arathi Basin banner opening. // TODO: Verify correctness of this check
        if ((goInfo->type == GAMEOBJECT_TYPE_BUTTON && goInfo->button.noDamageImmune) ||
            ///< this type replaced with GAMEOBJECT_TYPE_CAPTURE_POINT for bgs - here just for support old systems
            (goInfo->type == GAMEOBJECT_TYPE_GOOBER && goInfo->goober.requireLOS) ||
            (goInfo->type == GAMEOBJECT_TYPE_CAPTURE_POINT))
        {
            //CanUseBattlegroundObject() already called in CheckCast()
            // in battleground check
            if (Battleground* bg = player->GetBattleground())
            {
                bg->EventPlayerClickedOnFlag(player, gameObjTarget);
                return;
            }
        }
        else if (goInfo->type == GAMEOBJECT_TYPE_FLAGSTAND)
        {
            //CanUseBattlegroundObject() already called in CheckCast()
            // in battleground check
            if (Battleground* bg = player->GetBattleground())
            {
                if (bg->GetTypeID(true) == BATTLEGROUND_EY || bg->GetTypeID(true) == BATTLEGROUND_EY_RATED)
                    bg->EventPlayerClickedOnFlag(player, gameObjTarget);
                return;
            }
        }
        else if (m_spellInfo->Id == 1842 && gameObjTarget->GetGOInfo()->type == GAMEOBJECT_TYPE_TRAP && gameObjTarget->GetOwner())
        {
            gameObjTarget->SetLootState(GO_JUST_DEACTIVATED);
            return;
        }
        // TODO: Add script for spell 41920 - Filling, becouse server it freze when use this spell
        // handle outdoor pvp object opening, return true if go was registered for handling
        // these objects must have been spawned by outdoorpvp!
        else if (gameObjTarget->GetGOInfo()->type == GAMEOBJECT_TYPE_GOOBER && sOutdoorPvPMgr->HandleOpenGo(player, gameObjTarget->GetGUID()))
            return;
        lockId = goInfo->GetLockId();
        guid = gameObjTarget->GetGUID();
    }
    else if (itemTarget)
    {
        lockId = itemTarget->GetTemplate()->LockID;
        guid = itemTarget->GetGUID();
    }
    else
    {
        sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WORLD: Open Lock - No GameObject/Item Target!");
        return;
    }

    SkillType skillId = SKILL_NONE;
    int32 reqSkillValue = 0;
    int32 skillValue;

    SpellCastResult res = CanOpenLock(effIndex, lockId, skillId, reqSkillValue, skillValue);
    if (res != SPELL_CAST_OK && lockId != 2127)
    {
        SendCastResult(res);
        return;
    }

    if (gameObjTarget)
        SendLoot(guid, LOOT_SKINNING);
    else if (itemTarget)
        itemTarget->SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_UNLOCKED);

    // not allow use skill grow at item base open
    if (!m_CastItem && skillId != SKILL_NONE)
    {
        // update skill if really known
        if (uint32 pureSkillValue = player->GetPureSkillValue(skillId))
        {
            if (gameObjTarget)
            {
                reqSkillValue = gameObjTarget->GetGOInfo()->chest.trivialSkillLow - 50;

                // Allow one skill-up until respawned
                if (!gameObjTarget->IsInSkillupList(player->GetGUID()) &&
                    player->UpdateGatherSkill(skillId, pureSkillValue, reqSkillValue))
                {
                    gameObjTarget->AddToSkillupList(player->GetGUID());

                    // Update player XP
                    // Patch 4.0.1 (2010-10-12): Gathering herbs and Mining will give XP
                    if (skillId == SKILL_MINING || skillId == SKILL_HERBALISM)
                        player->GiveGatheringXP();
                }
            }
            else if (itemTarget)
            {
                // Do one skill-up
                player->UpdateGatherSkill(skillId, pureSkillValue, reqSkillValue);
            }
        }
    }

    ExecuteLogEffectOpenLock(effIndex, gameObjTarget ? (Object*)gameObjTarget : (Object*)itemTarget);
}

void Spell::EffectSummonChangeItem(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = m_caster->ToPlayer();

    // applied only to using item
    if (!m_CastItem)
        return;

    // ... only to item in own inventory/bank/equip_slot
    if (m_CastItem->GetOwnerGUID() != player->GetGUID())
        return;

    uint32 newitemid = m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType;
    if (!newitemid)
        return;

    uint16 pos = m_CastItem->GetPos();

    Item* pNewItem = Item::CreateItem(newitemid, 1, player);
    if (!pNewItem)
        return;

    if (pNewItem->GetEntry() == 38186)
        sLog->outDebug(LOG_FILTER_EFIR, "EffectSummonChangeItem - CreateItem of item %u; 1 = %u playerGUID %u, itemGUID %u m_CastItem %i", pNewItem->GetEntry(), 1, player->GetGUID(), pNewItem->GetGUID(), m_CastItem->GetEntry());

    for (uint8 j = PERM_ENCHANTMENT_SLOT; j <= TEMP_ENCHANTMENT_SLOT; ++j)
        if (m_CastItem->GetEnchantmentId(EnchantmentSlot(j)))
            pNewItem->SetEnchantment(EnchantmentSlot(j), m_CastItem->GetEnchantmentId(EnchantmentSlot(j)), m_CastItem->GetEnchantmentDuration(EnchantmentSlot(j)), m_CastItem->GetEnchantmentCharges(EnchantmentSlot(j)));

    if (m_CastItem->GetUInt32Value(ITEM_FIELD_DURABILITY) < m_CastItem->GetUInt32Value(ITEM_FIELD_MAX_DURABILITY))
    {
        double lossPercent = 1 - m_CastItem->GetUInt32Value(ITEM_FIELD_DURABILITY) / double(m_CastItem->GetUInt32Value(ITEM_FIELD_MAX_DURABILITY));
        player->DurabilityLoss(pNewItem, lossPercent);
    }

    if (player->IsInventoryPos(pos))
    {
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), dest, pNewItem, true);
        if (msg == EQUIP_ERR_OK)
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if (m_CastItem == m_targets.GetItemTarget())
                m_targets.SetItemTarget(NULL);

            m_CastItem = NULL;
            m_castItemGUID.Clear();
            m_castItemEntry = 0;

            player->StoreItem(dest, pNewItem, true);
            return;
        }
    }
    else if (player->IsBankPos(pos))
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanBankItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), dest, pNewItem, true);
        if (msg == EQUIP_ERR_OK)
        {
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), true);

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if (m_CastItem == m_targets.GetItemTarget())
                m_targets.SetItemTarget(NULL);

            m_CastItem = NULL;
            m_castItemGUID.Clear();
            m_castItemEntry = 0;

            player->BankItem(dest, pNewItem, true);
            return;
        }
    }
    else if (player->IsEquipmentPos(pos))
    {
        uint16 dest;

        player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), true);

        uint8 msg = player->CanEquipItem(m_CastItem->GetSlot(), dest, pNewItem, true);

        if (msg == EQUIP_ERR_OK || msg == EQUIP_ERR_CLIENT_LOCKED_OUT)
        {
            if (msg == EQUIP_ERR_CLIENT_LOCKED_OUT) dest = EQUIPMENT_SLOT_MAINHAND;

            // prevent crash at access and unexpected charges counting with item update queue corrupt
            if (m_CastItem == m_targets.GetItemTarget())
                m_targets.SetItemTarget(NULL);

            m_CastItem = NULL;
            m_castItemGUID.Clear();
            m_castItemEntry = 0;

            player->EquipItem(dest, pNewItem, true);
            player->AutoUnequipOffhandIfNeed();
            return;
        }
    }

    // fail
    delete pNewItem;
}

void Spell::EffectProficiency(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    Player* p_target = m_caster->ToPlayer();

    uint32 subClassMask = m_spellInfo->EquippedItemSubClassMask;
    if (m_spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON && !(p_target->GetWeaponProficiency() & subClassMask))
    {
        p_target->AddWeaponProficiency(subClassMask);
        p_target->SendProficiency(ITEM_CLASS_WEAPON, p_target->GetWeaponProficiency());
    }
    if (m_spellInfo->EquippedItemClass == ITEM_CLASS_ARMOR && !(p_target->GetArmorProficiency() & subClassMask))
    {
        p_target->AddArmorProficiency(subClassMask);
        p_target->SendProficiency(ITEM_CLASS_ARMOR, p_target->GetArmorProficiency());
    }
}

void Spell::EffectSummonType(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    uint32 entry = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    if (!entry)
        return;

    SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValueB);
    if (!properties)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "EffectSummonType: Unhandled summon type %u", m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValueB);
        return;
    }

    if (!m_originalCaster)
        return;

    int32 duration = m_spellInfo->GetDuration();
    if (Player* modOwner = m_originalCaster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_DURATION, duration);

    TempSummon* summon = NULL;

    // determine how many units should be summoned
    uint32 numSummons;

    // some spells need to summon many units, for those spells number of summons is stored in effect value
    // however so far noone found a generic check to find all of those (there's no related data in summonproperties.dbc
    // and in spell attributes, possibly we need to add a table for those)
    // so here's a list of MiscValueB values, which is currently most generic check
    switch (properties->ID)
    {
        case 64:
        case 61:
        case 1101:
        case 66:
        case 648:
        case 2301:
        case 1061:
        case 1261:
        case 629:
        case 181:
        case 715:
        case 1562:
        case 833:
        case 1161:
        case 3245:
            numSummons = (damage > 0) ? damage : 1;
            break;
        default:
            numSummons = 1;
            break;
    }

    switch (properties->Category)
    {
        case SUMMON_CATEGORY_WILD:
        case SUMMON_CATEGORY_ALLY:
        case SUMMON_CATEGORY_UNK:
        {
            switch (properties->Type)
            {
                case SUMMON_TYPE_PET:
                case SUMMON_TYPE_GUARDIAN:
                case SUMMON_TYPE_GUARDIAN2:
                case SUMMON_TYPE_MINION:
                    SummonGuardian(effIndex, entry, properties, numSummons);
                    break;
                // Summons a vehicle, but doesn't force anyone to enter it (see SUMMON_CATEGORY_VEHICLE)
                case SUMMON_TYPE_VEHICLE:
                case SUMMON_TYPE_VEHICLE2:
                case SUMMON_TYPE_GATE:
                case SUMMON_TYPE_STATUE:
                    summon = m_caster->GetMap()->SummonCreature(entry, *destTarget, properties, duration, m_originalCaster, m_targets.GetUnitTargetGUID(), m_spellInfo->Id);
                    break;
                case SUMMON_TYPE_TOTEM:
                case SUMMON_TYPE_BANNER:
                {
                    summon = m_caster->GetMap()->SummonCreature(entry, *destTarget, properties, duration, m_originalCaster, m_targets.GetUnitTargetGUID(), m_spellInfo->Id);
                    if (!summon || !summon->isTotem())
                        return;

                    switch (m_spellInfo->Id)
                    {
                        case 114192: // Mocking Banner
                        {
                            uint32 perc = 30;
                            damage = m_caster->CountPctFromMaxHealth(perc);
                            break;
                        }
                        case 108280: // Healing Tide Totem
                        case 108270: // Stone Bulwark Totem
                        {
                            uint32 perc = 10;
                            if (m_caster->HasAura(63298))
                                perc = 15;

                            damage = m_caster->CountPctFromMaxHealth(perc);
                            break;
                        }
                        default:
                        {
                            if (m_caster->HasAura(63298))
                                damage += m_caster->CountPctFromMaxHealth(5);
                            break;
                        }
                    }

                    if (damage)                                            // if not spell info, DB values used
                    {
                        summon->SetMaxHealth(damage);
                        summon->SetHealth(damage);
                    }
                    break;
                }
                case SUMMON_TYPE_MINIPET:
                {
                    summon = m_caster->GetMap()->SummonCreature(entry, *destTarget, properties, duration, m_originalCaster, m_targets.GetUnitTargetGUID(), m_spellInfo->Id);

                    if (!summon || !summon->HasUnitTypeMask(UNIT_MASK_MINION))
                        return;

                    summon->SelectLevel(summon->GetCreatureTemplate());       // some summoned creaters have different from 1 DB data for level/hp
                    summon->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, summon->GetCreatureTemplate()->npcflag);

                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

                    // handle unique pets with the only creatureEntry and existing summonSpellID
                    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_spellInfo->Id != SPELL_SUMMON_BATTLE_PET && sDB2Manager.GetBattlePetSpeciesBySpellID(summon->GetEntry()))
                    {
                        ObjectGuid::LowType battlePetGUIDlow = m_caster->ToPlayer()->GetBattlePetMgr()->GetPetGUIDBySpell(m_spellInfo->Id);
                        if (battlePetGUIDlow)
                        {
                            if (auto petInfo = m_caster->ToPlayer()->GetBattlePetMgr()->GetPet(ObjectGuid::Create<HighGuid::BattlePet>(battlePetGUIDlow)))
                            {
                                m_caster->SetGuidValue(PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID, ObjectGuid::Create<HighGuid::BattlePet>(battlePetGUIDlow));
                                summon->SetGuidValue(UNIT_FIELD_BATTLE_PET_COMPANION_GUID, ObjectGuid::Create<HighGuid::BattlePet>(battlePetGUIDlow));
                                summon->SetUInt64Value(UNIT_FIELD_BATTLE_PET_DBID, battlePetGUIDlow);
                                if (petInfo->JournalInfo.CustomName != "")
                                    summon->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, time(nullptr));
                                m_caster->SetUInt32Value(PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY, petInfo->JournalInfo.BreedQuality);
                                summon->SetUInt32Value(UNIT_FIELD_WILD_BATTLE_PET_LEVEL, petInfo->JournalInfo.Level);
                                summon->SetHealth(petInfo->JournalInfo.Health);
                                summon->SetMaxHealth(petInfo->JournalInfo.MaxHealth);
                            }
                        }
                    }

                    summon->AI()->EnterEvadeMode();
                    break;
                }
                default:
                {
                    if (properties->Flags & 512 || properties->ID == 2921)
                    {
                        SummonGuardian(effIndex, entry, properties, numSummons);
                        break;
                    }
                    float radius = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius();

                    TempSummonType summonType = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN;

                    for (uint32 count = 0; count < numSummons; ++count)
                    {
                        Position pos;
                        if (count == 0)
                            pos = *destTarget;
                        else
                            // randomize position for multiple summons
                            m_caster->GetRandomPoint(*destTarget, radius, pos);

                        summon = m_originalCaster->SummonCreature(entry, *destTarget, m_originalTargetGUID ? m_originalTargetGUID : m_targets.GetUnitTargetGUID(), summonType, duration, m_spellInfo->Id, properties);
                        if (!summon)
                            continue;

                        switch (properties->ID)
                        {
                            case 3347: // Orphelins
                            {
                                if (int32 slot = properties->Slot)
                                {
                                    if (m_caster->m_SummonSlot[slot] && m_caster->m_SummonSlot[slot] != summon->GetGUID())
                                    {
                                        Creature* oldSummon = m_caster->GetMap()->GetCreature(m_caster->m_SummonSlot[slot]);
                                        if (oldSummon && oldSummon->isSummon())
                                            oldSummon->ToTempSummon()->UnSummon();
                                    }
                                    m_caster->m_SummonSlot[slot] = summon->GetGUID();
                                }
                            }
                            break;
                            default:
                                break;
                        }

                        if (properties->Category == SUMMON_CATEGORY_ALLY)
                        {
                            summon->SetOwnerGUID(m_originalCaster->GetGUID());
                            summon->SetCreatorGUID(m_originalCaster->GetGUID());
                            summon->setFaction(m_originalCaster->getFaction());
                        }

                        // Explosive Decoy and Explosive Decoy 2.0
                        if (m_spellInfo->Id == 54359 || m_spellInfo->Id == 62405)
                        {
                            summon->SetMaxHealth(damage);
                            summon->SetHealth(damage);
                            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        }

                        // Wild Mushroom : Plague
                        if (summon && m_spellInfo->Id == 113516)
                            m_originalCaster->CastSpell(m_originalCaster, 113517, true); // Wild Mushroom : Plague (periodic dummy)

                        ExecuteLogEffectSummonObject(effIndex, summon);
                    }
                    return;
                }
            }//switch
            break;
        }
        case SUMMON_CATEGORY_PET:
            SummonGuardian(effIndex, entry, properties, numSummons);
            break;
        case SUMMON_CATEGORY_PUPPET:
            summon = m_caster->GetMap()->SummonCreature(entry, *destTarget, properties, duration, m_originalCaster, m_targets.GetUnitTargetGUID(), m_spellInfo->Id);
            break;
        case SUMMON_CATEGORY_VEHICLE:
        {
            // Summoning spells (usually triggered by npc_spellclick) that spawn a vehicle and that cause the clicker
            // to cast a ride vehicle spell on the summoned unit.
            float x, y, z;
            m_caster->GetClosePoint(x, y, z, DEFAULT_WORLD_OBJECT_SIZE);
            summon = m_originalCaster->GetMap()->SummonCreature(entry, *destTarget, properties, duration, m_caster, m_targets.GetUnitTargetGUID(), m_spellInfo->Id);
            if (!summon || !summon->IsVehicle())
                return;

            // The spell that this effect will trigger. It has SPELL_AURA_CONTROL_VEHICLE
            uint32 spellId = VEHICLE_SPELL_RIDE_HARDCODED;
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValue());
            if (spellInfo && spellInfo->HasAura(SPELL_AURA_CONTROL_VEHICLE))
                spellId = spellInfo->Id;

            // Hard coded enter vehicle spell
            m_originalCaster->CastSpell(summon, spellId, true);

            uint32 faction = properties->Faction;
            if (!faction)
                faction = m_originalCaster->getFaction();

            summon->setFaction(faction);
            break;
        }
    }

    if (summon)
    {
        summon->SetCreatorGUID(m_originalCaster->GetGUID());
        ExecuteLogEffectSummonObject(effIndex, summon);
    }
}

void Spell::EffectLearnSpell(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
    {
        if (unitTarget->ToPet())
            EffectLearnPetSpell(effIndex);
        return;
    }

    Player* player = unitTarget->ToPlayer();

    uint32 spellToLearn = (m_spellInfo->Id == 483 || m_spellInfo->Id == 55884) ? damage : m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;
    player->learnSpell(spellToLearn, false);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell: Player %u has learned spell %u from NpcGUID=%u", player->GetGUIDLow(), spellToLearn, m_caster->GetGUIDLow());
}

typedef std::list< std::pair<uint32, ObjectGuid> > DispelList;
void Spell::EffectDispel(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    // Create dispel mask by dispel type
    uint32 dispel_type = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    uint32 dispelMask = SpellInfo::GetDispelMask(DispelType(dispel_type));
    bool LastDispelEff = false;

    // Epuration can dispell Magic with Sacred Cleansing
    if (m_spellInfo->Id == 4987 && m_caster->HasAura(53551))
        dispelMask = DISPEL_ALL_MASK;
    // Nature's Cure can dispell all Magic, Curse and poison
    else if (m_spellInfo->Id == 88423)
        dispelMask = ((1 << DISPEL_MAGIC) | (1 << DISPEL_CURSE) | (1 << DISPEL_POISON));

    DispelChargesList dispel_list;
    DispelChargesList predicted_dispel_list;
    unitTarget->GetDispellableAuraList(m_caster, dispelMask, dispel_list);

    if (!hasPredictedDispel)
    {
        uint32 allEffectDispelMask = m_spellInfo->GetSimilarEffectsMiscValueMask(SPELL_EFFECT_DISPEL, m_caster);
        unitTarget->GetDispellableAuraList(m_caster, allEffectDispelMask, predicted_dispel_list);

        if (!predicted_dispel_list.empty())
            hasPredictedDispel++;

        hasPredictedDispel++;
    }

    for (uint32 eff = effIndex + 1; eff < MAX_SPELL_EFFECTS; ++eff)
    {
        if (m_spellInfo->Effects[eff].Effect != 0)
        {
            if (m_spellInfo->Effects[eff].IsEffect(SPELL_EFFECT_DISPEL))
                break;
            else
                continue;
        }
        else LastDispelEff = true; break;
    }

    if (LastDispelEff && hasPredictedDispel == 1)
        if (Player* player = m_caster->ToPlayer())
            if (m_spellInfo->RecoveryTime <= 8000 && m_spellInfo->IsPositive())
                player->RemoveSpellCooldown(m_spellInfo->Id, true);

    if (dispel_list.empty())
        return;

    Unit::AuraEffectList const& mTotalAuraList = m_caster->GetAuraEffectsByType(SPELL_AURA_DUMMY);
    for (Unit::AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
        if ((*i)->GetMiscValue() == 11 && (*i)->GetSpellInfo()->Misc.SpellIconID == m_spellInfo->Misc.SpellIconID)
            (*i)->SetAmount(m_spellInfo->Id);

    // Ok if exist some buffs for dispel try dispel it
    DispelChargesList success_list;

    std::list<uint32> spellFails;
    // dispel N = damage buffs (or while exist buffs for dispel)
    for (int32 count = 0; count < damage && !dispel_list.empty();)
    {
        // Random select buff for dispel
        DispelChargesList::iterator itr = dispel_list.begin();
        std::advance(itr, urand(0, dispel_list.size() - 1));

        int32 chance = itr->first->CalcDispelChance(unitTarget, !unitTarget->IsFriendlyTo(m_caster));
        // 2.4.3 Patch Notes: "Dispel effects will no longer attempt to remove effects that have 100% dispel resistance."
        if (!chance)
        {
            dispel_list.erase(itr);
            continue;
        }
        else
        {
            if (roll_chance_i(chance))
            {
                success_list.push_back(std::make_pair(itr->first, 1));
                --itr->second;
                if (itr->second <= 0)
                    dispel_list.erase(itr);
            }
            else
                spellFails.push_back(itr->first->GetId());               // Spell Id
            ++count;
        }
    }

    if (!spellFails.empty())
        m_caster->SendDispelFailed(unitTarget->GetGUID(), m_spellInfo->Id, spellFails);

    if (success_list.empty())
        return;

    // Remove Curse and Glyph of Remove Curse
    if (m_spellInfo->Id == 475 && m_caster->HasAura(115700))
        m_caster->CastSpell(m_caster, 115701, true);

    std::list<uint32> spellSuccess;

    for (DispelChargesList::iterator itr = success_list.begin(); itr != success_list.end(); ++itr)
    {
        spellSuccess.push_back(itr->first->GetId());
        unitTarget->RemoveAurasDueToSpellByDispel(itr->first->GetId(), m_spellInfo->Id, itr->first->GetCasterGUID(), m_caster, itr->second);
    }

    m_count_dispeling += spellSuccess.size();
    m_caster->SendDispelLog(unitTarget->GetGUID(), m_spellInfo->Id, spellSuccess, false, false);

    // On success dispel
    // Devour Magic
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_PET_ABILITY && m_spellInfo->Categories.Category == SPELLCATEGORY_DEVOUR_MAGIC)
    {
        m_caster->CastSpell(m_caster, 19658, true);
        // Glyph of Felhunter
        if (Unit* owner = m_caster->GetOwner())
            if (owner->GetAura(56249))
                m_caster->CastSpell(owner, 19658, true);
    }
}

void Spell::EffectDualWield(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    unitTarget->SetCanDualWield(true);
    if (unitTarget->GetTypeId() == TYPEID_UNIT)
        unitTarget->ToCreature()->UpdateDamagePhysical(OFF_ATTACK);
}

void Spell::EffectPull(SpellEffIndex effIndex)
{
    // TODO: create a proper pull towards distract spell center for distract
    EffectNULL(effIndex);
}

void Spell::EffectDistract(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    // Check for possible target
    if (!unitTarget || unitTarget->isInCombat())
        return;

    // target must be OK to do this
    if (unitTarget->HasUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_STUNNED | UNIT_STATE_FLEEING))
        return;

    unitTarget->SetFacingTo(unitTarget->GetAngle(destTarget));
    unitTarget->ClearUnitState(UNIT_STATE_MOVING);

    if (unitTarget->GetTypeId() == TYPEID_UNIT)
        unitTarget->GetMotionMaster()->MoveDistract(damage * IN_MILLISECONDS);
}

void Spell::EffectPickPocket(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    // victim must be creature and attackable
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT/*|| m_caster->IsFriendlyTo(unitTarget)*/)
        return;

    // victim have to be alive and humanoid or undead
    if (unitTarget->isAlive() && (unitTarget->GetCreatureTypeMask() &CREATURE_TYPEMASK_HUMANOID_OR_UNDEAD) != 0)
        m_caster->ToPlayer()->SendLoot(unitTarget->GetGUID(), LOOT_PICKPOCKETING);
}

void Spell::EffectAddFarsight(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    float radius = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius();
    int32 duration = m_spellInfo->GetDuration();
    // Caster not in world, might be spell triggered from aura removal
    if (!m_caster->IsInWorld())
        return;

    DynamicObject* dynObj = new DynamicObject(true);
    if (!dynObj->CreateDynamicObject(sObjectMgr->GetGenerator<HighGuid::DynamicObject>()->Generate(), m_caster, m_spellInfo->Id, *destTarget, radius, DYNAMIC_OBJECT_FARSIGHT_FOCUS))
    {
        delete dynObj;
        return;
    }

    SetSpellDynamicObject(dynObj->GetGUID());
    dynObj->SetDuration(duration);
    dynObj->SetCasterViewpoint();
}

void Spell::EffectUntrainTalents(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || m_caster->GetTypeId() == TYPEID_PLAYER)
        return;

    ObjectGuid guid = m_caster->GetGUID();
    if (!guid.IsEmpty()) // the trainer is the caster
        unitTarget->ToPlayer()->SendTalentWipeConfirm(guid, RESPEC_TYPE_TALENTS);
}

void Spell::EffectTeleUnitsFaceCaster(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (unitTarget->isInFlight())
        return;

    float dis = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius(m_caster);

    //float fx, fy, fz;
    //m_caster->GetClosePoint(fx, fy, fz, unitTarget->GetObjectSize(), dis);
    Position pos;
    m_caster->GetNearPosition(pos, m_caster->GetObjectSize(), m_caster->GetAngle(unitTarget));

    // Earthen Vortex, Morchok, Dragon Soul
    // Prevent dropping into textures
    switch (m_spellInfo->Id)
    {
        case 103821:
            pos.m_positionX += 8.0f;
            break;
        default:
            break;
    }

    unitTarget->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), -m_caster->GetOrientation(), unitTarget == m_caster);
}

void Spell::EffectLearnSkill(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if (damage < 0)
        return;

    uint32 skillid = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    SkillRaceClassInfoEntry const* rcEntry = sDB2Manager.GetSkillRaceClassInfo(skillid, unitTarget->getRace(), unitTarget->getClass());
    if (!rcEntry)
        return;

    SkillTiersEntry const* tier = sObjectMgr->GetSkillTier(rcEntry->SkillTierID);
    if (!tier)
        return;

    uint16 skillval = unitTarget->ToPlayer()->GetPureSkillValue(skillid);
    unitTarget->ToPlayer()->SetSkill(skillid, m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValue(), std::max<uint16>(skillval, 1), tier->Value[damage - 1]);

    // Archaeology
    if (skillid == SKILL_ARCHAEOLOGY && !skillval && sWorld->getBoolConfig(CONFIG_ARCHAEOLOGY_ENABLED))
    {
        unitTarget->ToPlayer()->GenerateResearchSites();
        unitTarget->ToPlayer()->GenerateResearchProjects();
    }
}

void Spell::EffectPlayMovie(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    uint32 movieId = GetSpellInfo()->GetEffect(effIndex, m_diffMode)->MiscValue;
    if (!sMovieStore.LookupEntry(movieId))
        return;

    unitTarget->ToPlayer()->SendMovieStart(movieId);
}

void Spell::EffectTradeSkill(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    // uint32 skillid =  m_spellInfo->GetEffect(i, m_diffMode).MiscValue;
    // uint16 skillmax = unitTarget->ToPlayer()->(skillid);
    // m_caster->ToPlayer()->SetSkill(skillid, skillval?skillval:1, skillmax+75);
}

void Spell::EffectEnchantItemPerm(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!itemTarget)
        return;

    Player* p_caster = (Player*)m_caster;

    // Handle vellums
    if (itemTarget->IsVellum())
    {
        // destroy one vellum from stack
        uint32 count = 1;
        p_caster->DestroyItemCount(itemTarget, count, true);
        unitTarget = p_caster;
        // and add a scroll
        DoCreateItem(effIndex, m_spellInfo->GetEffect(effIndex, m_diffMode)->ItemType);
        itemTarget = NULL;
        m_targets.SetItemTarget(NULL);
    }
    else
    {
        // do not increase skill if vellum used
        if (!(m_CastItem && m_CastItem->GetTemplate()->Flags & ITEM_PROTO_FLAG_TRIGGERED_CAST))
            p_caster->UpdateCraftSkill(m_spellInfo->Id);

        uint32 enchant_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
        if (!enchant_id)
            return;

        SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!pEnchant)
            return;

        // item can be in trade slot and have owner diff. from caster
        Player* item_owner = itemTarget->GetOwner();
        if (!item_owner)
            return;

        if (item_owner != p_caster && !AccountMgr::IsPlayerAccount(p_caster->GetSession()->GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
        {
            sLog->outCommand(p_caster->GetSession()->GetAccountId(), "GM %s (Account: %u) enchanting(perm): %s (Entry: %d) for player: %s (Account: %u)",
                             p_caster->GetName(), p_caster->GetSession()->GetAccountId(),
                             itemTarget->GetTemplate()->Name1.c_str(), itemTarget->GetEntry(),
                             item_owner->GetName(), item_owner->GetSession()->GetAccountId());
        }

        EnchantmentSlot slot = pEnchant->RequiredSkillID == SKILL_ENGINEERING ? USE_ENCHANTMENT_SLOT : PERM_ENCHANTMENT_SLOT;

        // remove old enchanting before applying new if equipped
        item_owner->ApplyEnchantment(itemTarget, slot, false);

        itemTarget->SetEnchantment(slot, enchant_id, 0, 0, m_caster->GetGUID());

        // add new enchanting if equipped
        item_owner->ApplyEnchantment(itemTarget, slot, true);

        item_owner->RemoveTradeableItem(itemTarget);
        itemTarget->ClearSoulboundTradeable(item_owner);
    }
}

void Spell::EffectEnchantItemPrismatic(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!itemTarget)
        return;

    Player* p_caster = (Player*)m_caster;

    uint32 enchant_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    if (!enchant_id)
        return;

    SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if (!pEnchant)
        return;

    // support only enchantings with add socket in this slot
    {
        bool add_socket = false;
        for (uint8 i = 0; i < MAX_ITEM_ENCHANTMENT_EFFECTS; ++i)
        {
            if (pEnchant->Effect[i] == ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET)
            {
                add_socket = true;
                break;
            }
        }
        if (!add_socket)
        {
            sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell::EffectEnchantItemPrismatic: attempt apply enchant spell %u with SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC (%u) but without ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET (%u), not suppoted yet.",
                           m_spellInfo->Id, SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC, ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET);
            return;
        }
    }

    // item can be in trade slot and have owner diff. from caster
    Player* item_owner = itemTarget->GetOwner();
    if (!item_owner)
        return;

    if (item_owner != p_caster && !AccountMgr::IsPlayerAccount(p_caster->GetSession()->GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
    {
        sLog->outCommand(p_caster->GetSession()->GetAccountId(), "GM %s (Account: %u) enchanting(perm): %s (Entry: %d) for player: %s (Account: %u)",
                         p_caster->GetName(), p_caster->GetSession()->GetAccountId(),
                         itemTarget->GetTemplate()->Name1.c_str(), itemTarget->GetEntry(),
                         item_owner->GetName(), item_owner->GetSession()->GetAccountId());
    }

    // remove old enchanting before applying new if equipped
    item_owner->ApplyEnchantment(itemTarget, PRISMATIC_ENCHANTMENT_SLOT, false);

    itemTarget->SetEnchantment(PRISMATIC_ENCHANTMENT_SLOT, enchant_id, 0, 0, m_caster->GetGUID());

    // add new enchanting if equipped
    item_owner->ApplyEnchantment(itemTarget, PRISMATIC_ENCHANTMENT_SLOT, true);

    item_owner->RemoveTradeableItem(itemTarget);
    itemTarget->ClearSoulboundTradeable(item_owner);
}

void Spell::EffectEnchantItemTmp(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;

    if (!itemTarget)
        return;

    uint32 enchant_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    if (!enchant_id)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell %u Effect %u (SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY) have 0 as enchanting id", m_spellInfo->Id, effIndex);
        return;
    }

    SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
    if (!pEnchant)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell %u Effect %u (SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY) have not existed enchanting id %u ", m_spellInfo->Id, effIndex, enchant_id);
        return;
    }

    // select enchantment duration
    uint32 duration;

    // other cases with this SpellVisual already selected
    if (m_spellInfo->GetSpellVisual(DIFFICULTY_NONE) == 215)
        duration = 1800;                                    // 30 mins
    // some fishing pole bonuses except Glow Worm which lasts full hour
    else if (m_spellInfo->GetSpellVisual(DIFFICULTY_NONE) == 563 && m_spellInfo->Id != 64401)
        duration = 600;                                     // 10 mins
    else if (m_spellInfo->Id == 29702)
        duration = 300;                                     // 5 mins
    else if (m_spellInfo->Id == 37360)
        duration = 300;                                     // 5 mins
    // default case
    else
        duration = 3600;                                    // 1 hour

    // item can be in trade slot and have owner diff. from caster
    Player* item_owner = itemTarget->GetOwner();
    if (!item_owner)
        return;

    if (item_owner != p_caster && !AccountMgr::IsPlayerAccount(p_caster->GetSession()->GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
    {
        sLog->outCommand(p_caster->GetSession()->GetAccountId(), "GM %s (Account: %u) enchanting(temp): %s (Entry: %d) for player: %s (Account: %u)",
                         p_caster->GetName(), p_caster->GetSession()->GetAccountId(),
                         itemTarget->GetTemplate()->Name1.c_str(), itemTarget->GetEntry(),
                         item_owner->GetName(), item_owner->GetSession()->GetAccountId());
    }

    // remove old enchanting before applying new if equipped
    item_owner->ApplyEnchantment(itemTarget, TEMP_ENCHANTMENT_SLOT, false);

    itemTarget->SetEnchantment(TEMP_ENCHANTMENT_SLOT, enchant_id, duration * 1000, 0, m_caster->GetGUID());

    // add new enchanting if equipped
    item_owner->ApplyEnchantment(itemTarget, TEMP_ENCHANTMENT_SLOT, true);
}

void Spell::EffectTameCreature(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetPetGUID())
        return;

    if (!unitTarget)
        return;

    if (unitTarget->GetTypeId() != TYPEID_UNIT)
        return;

    Creature* creatureTarget = unitTarget->ToCreature();

    if (creatureTarget->isPet())
        return;

    if (m_caster->getClass() != CLASS_HUNTER)
        return;

    PetSlot slot = m_caster->ToPlayer()->getSlotForNewPet();

    // If we have a full list we shoulden't be able to create a new one.
    if (slot == PET_SLOT_FULL_LIST)
    {
        m_caster->ToPlayer()->SendPetTameResult(PET_TAME_ERROR_TOO_MANY_PETS);
        return;
    }

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->m_currentSummonedSlot = slot;

    // cast finish successfully
    //SendChannelUpdate(0);
    finish();

    Pet* pet = m_caster->CreateTamedPetFrom(creatureTarget, m_spellInfo->Id);
    if (!pet)                                               // in very specific state like near world end/etc.
        return;

    // "kill" original creature
    creatureTarget->DespawnOrUnsummon();

    uint8 level = m_caster->getLevel();

    // prepare visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);

    // add to world
    pet->GetMap()->AddToMap(pet->ToCreature());
    pet->SetSlot(slot);

    // visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

    // caster have pet now
    m_caster->SetMinion(pet, true);

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        m_caster->ToPlayer()->m_currentSummonedSlot = slot;
        pet->SetSlot(slot);
        pet->SavePetToDB();
        m_caster->ToPlayer()->PetSpellInitialize();
        m_caster->ToPlayer()->GetSession()->SendStablePet(ObjectGuid::Empty);
    }
}

void Spell::EffectSummonPet(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    Player* owner = NULL;
    if (m_originalCaster)
    {
        owner = m_originalCaster->ToPlayer();
        if (!owner && m_originalCaster->ToCreature()->isTotem())
            owner = m_originalCaster->GetCharmerOrOwnerPlayerOrPlayerItself();
    }

    uint32 petentry = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    if (!owner)
    {
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(67);
        if (properties)
            SummonGuardian(effIndex, petentry, properties, 1);
        return;
    }

    Pet* OldSummon = owner->GetPet();

    // if pet requested type already exist
    if (OldSummon)
    {
        if (petentry == 0)
        {
            // pet in corpse state can't be summoned
            if (OldSummon->isDead())
                return;

            ASSERT(OldSummon->GetMap() == owner->GetMap());

            float px, py, pz;
            owner->GetClosePoint(px, py, pz, OldSummon->GetObjectSize());
            OldSummon->NearTeleportTo(px, py, pz, OldSummon->GetOrientation());

            if (owner->GetTypeId() == TYPEID_PLAYER && OldSummon->isControlled())
                owner->ToPlayer()->PetSpellInitialize();

            return;
        }

        if (owner->GetTypeId() == TYPEID_PLAYER)
            owner->ToPlayer()->RemovePet(OldSummon);
        else
            return;
    }

    PetSlot slot = PetSlot(m_spellInfo->GetEffect(effIndex, m_diffMode)->BasePoints);
    owner->m_currentSummonedSlot = slot;

    Position pos;
    owner->GetFirstCollisionPosition(pos, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);

    Pet* pet = owner->SummonPet(petentry, pos.m_positionX, pos.m_positionY, pos.m_positionZ, owner->GetOrientation(), SUMMON_PET, 0, m_spellInfo->Id);
    if (!pet)
        return;

    if (m_caster->GetTypeId() == TYPEID_UNIT)
    {
        if (m_caster->ToCreature()->isTotem())
            pet->SetReactState(REACT_AGGRESSIVE);
        else
            pet->SetReactState(REACT_DEFENSIVE);
    }

    // generate new name for summon pet
    if (petentry)
    {
        std::string new_name = sObjectMgr->GeneratePetName(petentry);
        if (!new_name.empty())
            pet->SetName(new_name);
    }
    pet->SetSlot(slot);

    ExecuteLogEffectSummonObject(effIndex, pet);
}

void Spell::EffectLearnPetSpell(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (unitTarget->ToPlayer())
    {
        EffectLearnSpell(effIndex);
        return;
    }
    Pet* pet = unitTarget->ToPet();
    if (!pet)
        return;

    SpellInfo const* learn_spellproto = sSpellMgr->GetSpellInfo(m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell);
    if (!learn_spellproto)
        return;

    pet->learnSpell(learn_spellproto->Id);
    pet->SavePetToDB();
    if (Player* player = pet->GetOwner()->ToPlayer())
        player->PetSpellInitialize();
}

void Spell::EffectTaunt(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    switch (m_spellInfo->Id)
    {
        case 51399:     // Death Grip
        case 49560:     // Death Grip
        {
            // Glyph of Tranquil Grip
            if (m_caster->HasAura(131554))
                return;
        }
        break;
    }

    // this effect use before aura Taunt apply for prevent taunt already attacking target
    // for spell as marked "non effective at already attacking target"
    if (!unitTarget || !unitTarget->CanHaveThreatList()
        || unitTarget->getVictim() == m_caster)
    {
        SendCastResult(SPELL_FAILED_DONT_REPORT);
        return;
    }

    // Also use this effect to set the taunter's threat to the taunted creature's highest value
    if (unitTarget->getThreatManager().getCurrentVictim())
    {
        float myThreat = unitTarget->getThreatManager().getThreat(m_caster);
        float itsThreat = unitTarget->getThreatManager().getCurrentVictim()->getThreat();
        if (itsThreat > myThreat)
            unitTarget->getThreatManager().addThreat(m_caster, itsThreat - myThreat);
    }

    //Set aggro victim to caster
    if (!unitTarget->getThreatManager().getOnlineContainer().empty())
        if (HostileReference* forcedVictim = unitTarget->getThreatManager().getOnlineContainer().getReferenceByTarget(m_caster))
            unitTarget->getThreatManager().setCurrentVictim(forcedVictim);

    if (unitTarget->ToCreature()->IsAIEnabled && !unitTarget->ToCreature()->HasReactState(REACT_PASSIVE))
        unitTarget->ToCreature()->AI()->AttackStart(m_caster);
}

void Spell::EffectWeaponDmg(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive())
        return;

    // multiple weapon dmg effect workaround
    // execute only the last weapon damage
    // and handle all effects at once
    for (uint32 j = effIndex + 1; j < MAX_SPELL_EFFECTS; ++j)
    {
        switch (m_spellInfo->Effects[j].Effect)
        {
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
            case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
            case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                if ((m_spellInfo->Effects[effIndex].TargetA.GetTarget() == m_spellInfo->Effects[j].TargetA.GetTarget() || m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_NONE || m_spellInfo->Effects[j].TargetA.GetTarget() == TARGET_NONE)
                    && (m_spellInfo->Effects[effIndex].TargetB.GetTarget() == m_spellInfo->Effects[j].TargetB.GetTarget() || m_spellInfo->Effects[effIndex].TargetB.GetTarget() == TARGET_NONE || m_spellInfo->Effects[j].TargetB.GetTarget() == TARGET_NONE))
                    return;     // we must calculate only at last weapon effect
                break;
        }
    }

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell EffectWeaponDmg spellid %u in Effect(%u) m_damage %i, damage %i", m_spellInfo->Id, effIndex, m_damage, damage);

    // some spell specific modifiers
    float totalDamagePercentMod = 1.0f;                    // applied to final bonus+weapon damage
    int32 fixed_bonus = 0;
    int32 spell_bonus = 0;                                  // bonus specific for spell
    bool calcAllEffects = true;

    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (m_spellInfo->Id)
            {
                case 69055:     // Saber Lash
                {
                    uint32 count = 0;
                    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                        if (ihit->effectMask & (1 << effIndex))
                            ++count;

                    totalDamagePercentMod /= count;
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            // Heroic Strike - 154%, Cleave - 114.8% more damage with one-hand weapon
            if (m_spellInfo->Id == 78 || m_spellInfo->Id == 845)
            {
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    if (Item* item = m_caster->ToPlayer()->GetWeaponForAttack(m_attackType, true))
                        if (item->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_SWORD ||
                            item->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
                            item->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
                            item->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
                            item->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_FIST_WEAPON)
                            totalDamagePercentMod *= 1.4f;
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Ambush & Hemorrhage- 40% more damage with daggers
            if (m_spellInfo->Id == 8676 || m_spellInfo->Id == 16511)
            {
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    if (Item* item = m_caster->ToPlayer()->GetWeaponForAttack(m_attackType, true))
                        if (item->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER)
                            totalDamagePercentMod *= 1.4f;
            }
            // Fan of Knives
            else if (m_spellInfo->Id == 51723)
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    break;

                //if (m_caster->ToPlayer()->GetComboTarget() == unitTarget->GetGUID())
                m_caster->ToPlayer()->AddComboPoints(unitTarget, 1);

                // Fan of Knives - Vile Poisons
                if (AuraEffect* aur = m_caster->GetDummyAuraEffect(SPELLFAMILY_ROGUE, 857, 2))
                {
                    if (roll_chance_i(aur->GetAmount()))
                    {
                        for (uint8 i = BASE_ATTACK; i < MAX_ATTACK; ++i)
                            m_caster->ToPlayer()->CastItemCombatSpell(unitTarget, WeaponAttackType(i), PROC_FLAG_TAKEN_DAMAGE, PROC_EX_NORMAL_HIT);
                    }
                }
            }
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            // Skyshatter Harness item set bonus
            // Stormstrike
            if (AuraEffect* aurEff = m_caster->IsScriptOverriden(m_spellInfo, 5634))
                m_caster->CastSpell(m_caster, 38430, true, NULL, aurEff);
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            // Mangle (Cat): CP
            if (m_spellInfo->SpellFamilyFlags[1] & 0x400)
            {
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    m_caster->ToPlayer()->AddComboPoints(unitTarget, 1, this);
            }
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            switch (m_spellInfo->Id)
            {
                case 109259:
                {
                    calcAllEffects = false;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            // Obliterate (12.5% more damage per disease)
            if (m_spellInfo->SpellFamilyFlags[1] & 0x20000)
            {
                float bonusPct = m_spellInfo->GetEffect(EFFECT_2, m_diffMode)->CalcValue(m_caster) * unitTarget->GetDiseasesByCaster(m_caster->GetGUID(), false) / 2.0f;
                // Death Knight T8 Melee 4P Bonus
                if (AuraEffect const* aurEff = m_caster->GetAuraEffect(64736, EFFECT_0))
                    AddPct(bonusPct, aurEff->GetAmount());
                AddPct(totalDamagePercentMod, bonusPct);
                break;
            }
            // Heart Strike
            if (m_spellInfo->SpellFamilyFlags[0] & 0x1000000)
            {
                float bonusPct = m_spellInfo->GetEffect(EFFECT_2, m_diffMode)->CalcValue(m_caster) * unitTarget->GetDiseasesByCaster(m_caster->GetGUID());
                // Death Knight T8 Melee 4P Bonus
                if (AuraEffect const* aurEff = m_caster->GetAuraEffect(64736, EFFECT_0))
                    AddPct(bonusPct, aurEff->GetAmount());

                AddPct(totalDamagePercentMod, bonusPct);
                break;
            }
            break;
        }
    }

    bool normalized = false;
    float weaponDamagePercentMod = 0.0f;
    for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
    {
        switch (m_spellInfo->Effects[j].Effect)
        {
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                fixed_bonus += CalculateDamage(j, unitTarget);
                break;
            case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                fixed_bonus += CalculateDamage(j, unitTarget);
                normalized = true;
                break;
            case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                if (!calcAllEffects && effIndex == j + 1 || calcAllEffects)
                    weaponDamagePercentMod += m_spellInfo->GetEffect(j, m_diffMode)->CalcValue(m_caster) / 100.0f;
                    //weaponDamagePercentMod += CalculateDamage(j, unitTarget) / 100.0f;
                break;
            default:
                break;                                      // not weapon damage effect, just skip
        }
    }

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell EffectWeaponDmg spellid %u in Effect(%u) fixed_bonus %i, spell_bonus %i weaponDamagePercentMod %f", m_spellInfo->Id, effIndex, fixed_bonus, spell_bonus, weaponDamagePercentMod);

    // apply to non-weapon bonus weapon total pct effect, weapon total flat effect included in weapon damage
    if (fixed_bonus || spell_bonus)
    {
        UnitMods unitMod;
        switch (m_attackType)
        {
            default:
            case BASE_ATTACK:   unitMod = UNIT_MOD_DAMAGE_MAINHAND; break;
            case OFF_ATTACK:    unitMod = UNIT_MOD_DAMAGE_OFFHAND;  break;
            case RANGED_ATTACK: unitMod = UNIT_MOD_DAMAGE_RANGED;   break;
        }

        float weapon_total_pct = 1.0f;
        if (m_spellInfo->Misc.SchoolMask & SPELL_SCHOOL_MASK_NORMAL)
            weapon_total_pct = m_caster->GetModifierValue(unitMod, TOTAL_PCT);

        if (fixed_bonus)
            fixed_bonus = int32(fixed_bonus * weapon_total_pct);
        if (spell_bonus)
            spell_bonus = int32(spell_bonus * weapon_total_pct);
    }

    int32 weaponDamage = m_caster->CalculateDamage(m_attackType, normalized, true);
    bool  calculateWPD = true;

    // Sequence is important
    for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
    {
        // We assume that a spell have at most one fixed_bonus
        // and at most one weaponDamagePercentMod
        switch (m_spellInfo->Effects[j].Effect)
        {
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
            case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                weaponDamage += fixed_bonus;
                break;
            case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
            {
                if (calculateWPD)
                {
                    weaponDamage = int32(weaponDamage* weaponDamagePercentMod);
                    calculateWPD = false;
                }
            }
            default:
                break;                                      // not weapon damage effect, just skip
        }
    }

    if (spell_bonus)
        weaponDamage += spell_bonus;

    if (totalDamagePercentMod != 1.0f)
        weaponDamage = int32(weaponDamage* totalDamagePercentMod);

    // prevent negative damage
    uint32 eff_damage(std::max(weaponDamage, 0));

    // Add melee damage bonuses (also check for negative)
    uint32 _damage = m_caster->MeleeDamageBonusDone(unitTarget, eff_damage, m_attackType, m_spellInfo, (1 << effIndex));

    m_damage += _damage;

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell EffectWeaponDmg spellid %u in Effect(%u) weaponDamage %i, _damage %i m_damage %i effectHandleMode %i", m_spellInfo->Id, effIndex, weaponDamage, _damage, m_damage, effectHandleMode);

    switch (m_spellInfo->Id)
    {
        case 5221: // Shred
        {
            if (unitTarget->HasAuraWithMechanic((1 << MECHANIC_BLEED)))
            {
                AddPct(m_damage, 20);
            }
            break;
        }
        case 25504: // Windfury Weapon MH
        case 33750: // Windfury Weapon OH
        {
            AddPct(m_damage, 50); // Hotfix 24th September MOP (5.4): Windfury Now deals 50% additional damage.
            break;
        }
        case 60103:        // Lava Lash
        {
            if (m_caster->GetTypeId() != TYPEID_PLAYER)
                break;

            //Bonus 4P shaman
            if (m_caster->HasAura(131554))
            {
                if (Item* offItem = m_caster->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                {
                    // Flametongue
                    if (offItem->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 2)
                        AddPct(m_damage, 40);
                }
            }
            break;
        }
    }
}

void Spell::EffectThreat(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive() || !m_caster->isAlive())
        return;

    if (!unitTarget->CanHaveThreatList())
        return;

    unitTarget->AddThreat(m_caster, float(damage));
}

void Spell::EffectHealMaxHealth(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive())
        return;

    int32 addhealth = 0;

    // damage == 0 - heal for caster max health
    if (damage == 0)
        addhealth = m_caster->GetMaxHealth();
    else
        addhealth = unitTarget->GetMaxHealth() - unitTarget->GetHealth();

    addhealth = m_caster->SpellHealingBonusDone(m_caster, m_spellInfo, addhealth, HEAL, effIndex);
    addhealth = unitTarget->SpellHealingBonusTaken(m_caster, m_spellInfo, addhealth, HEAL, effIndex);

    m_healing += addhealth;
}

void Spell::EffectInterruptCast(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive())
        return;

    // TODO: not all spells that used this effect apply cooldown at school spells
    // also exist case: apply cooldown to interrupted cast only and to all spells
    // there is no CURRENT_AUTOREPEAT_SPELL spells that can be interrupted
    for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_AUTOREPEAT_SPELL; ++i)
    {
        if (Spell* spell = unitTarget->GetCurrentSpell(CurrentSpellTypes(i)))
        {
            SpellInfo const* curSpellInfo = spell->m_spellInfo;
            // check if we can interrupt spell
            if ((spell->getState() == SPELL_STATE_CASTING
                || (spell->getState() == SPELL_STATE_PREPARING && spell->GetCastTime() > 0.0f))
                && (curSpellInfo->Categories.PreventionType == SPELL_PREVENTION_TYPE_SILENCE || curSpellInfo->Categories.PreventionType > SPELL_PREVENTION_TYPE_PACIFY && curSpellInfo->Categories.PreventionType != SPELL_PREVENTION_TYPE_UNK4)
                && ((i == CURRENT_GENERIC_SPELL && curSpellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_INTERRUPT)
                || (i == CURRENT_CHANNELED_SPELL && curSpellInfo->ChannelInterruptFlags & CHANNEL_INTERRUPT_FLAG_INTERRUPT)))
            {
                if (m_originalCaster)
                {
                    int32 duration = m_spellInfo->GetDuration();
                    unitTarget->ProhibitSpellSchool(curSpellInfo->GetSchoolMask(), unitTarget->ModSpellDuration(m_spellInfo, unitTarget, duration, false, 1 << effIndex, m_originalCaster));
                }

                if (Creature* creature = unitTarget->ToCreature())
                    if (creature->IsAIEnabled)
                        creature->AI()->OnInterruptCast(m_caster, m_spellInfo->Id, curSpellInfo->Id, curSpellInfo->GetSchoolMask());

                ExecuteLogEffectInterruptCast(effIndex, unitTarget, curSpellInfo->Id);
                unitTarget->InterruptSpell(CurrentSpellTypes(i), false);
                unitTarget->SendLossOfControl(m_caster, m_spellInfo->Id, m_spellInfo->GetDuration(), m_spellInfo->GetDuration(), m_spellInfo->GetEffectMechanic(effIndex), curSpellInfo->GetSchoolMask(), LOC_SCHOOL_INTERRUPT, true);

                switch (m_spellInfo->Id)
                {
                    case 6552: // Pummel
                        if (m_caster->HasAura(58372)) // Glyph of Rude Interruption
                            m_caster->CastSpell(m_caster, 86663, true);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void Spell::EffectSummonObjectWild(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    uint32 gameobject_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    GameObject* pGameObj = new GameObject;

    WorldObject* target = focusObject;
    if (!target)
        target = m_caster;

    float x, y, z;
    if (m_targets.HasDst())
        destTarget->GetPosition(x, y, z);
    else
        m_caster->GetClosePoint(x, y, z, DEFAULT_WORLD_OBJECT_SIZE);

    Map* map = target->GetMap();

    if (!pGameObj->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), gameobject_id, map,
        m_caster->GetPhaseMask(), x, y, z, target->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, GO_STATE_READY))
    {
        delete pGameObj;
        return;
    }

    int32 duration = m_spellInfo->GetDuration();

    pGameObj->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);

    ExecuteLogEffectSummonObject(effIndex, pGameObj);

    // Wild object not have owner and check clickable by players
    map->AddToMap(pGameObj);

    if (pGameObj->GetGoType() == GAMEOBJECT_TYPE_FLAGDROP && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        Player* player = m_caster->ToPlayer();
        Battleground* bg = player->GetBattleground();

        switch (pGameObj->GetMapId())
        {
            case 489:                                       //WS
            {
                if (bg && bg->GetTypeID(true) == BATTLEGROUND_WS && bg->GetStatus() == STATUS_IN_PROGRESS)
                {
                    uint32 team = ALLIANCE;

                    if (player->GetTeam() == team)
                        team = HORDE;

                    ((BattlegroundWS*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID(), team);
                }
                break;
            }
            case 566:                                       //EY
            {
                if (bg && (bg->GetTypeID(true) == BATTLEGROUND_EY || bg->GetTypeID(true) == BATTLEGROUND_EY_RATED) && bg->GetStatus() == STATUS_IN_PROGRESS)
                {
                    ((BattlegroundEY*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID());
                }
                break;
            }
            case 726:                                       //TP
            {
                if (bg && bg->GetTypeID(true) == BATTLEGROUND_TP && bg->GetStatus() == STATUS_IN_PROGRESS)
                {
                    uint32 team = TEAM_ALLIANCE;

                    if (player->GetTeamId() == team)
                        team = TEAM_HORDE;

                    ((BattlegroundTP*)bg)->SetDroppedFlagGUID(pGameObj->GetGUID(), team);
                }
                break;
            }
        }
    }

    if (uint32 linkedEntry = pGameObj->GetGOInfo()->GetLinkedGameObjectEntry())
    {
        GameObject* linkedGO = new GameObject;
        if (linkedGO->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), linkedEntry, map,
            m_caster->GetPhaseMask(), x, y, z, target->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, GO_STATE_READY))
        {
            linkedGO->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);
            linkedGO->SetSpellId(m_spellInfo->Id);

            ExecuteLogEffectSummonObject(effIndex, linkedGO);

            // Wild object not have owner and check clickable by players
            map->AddToMap(linkedGO);
        }
        else
        {
            delete linkedGO;
            linkedGO = NULL;
            return;
        }
    }
}

void Spell::EffectScriptEffect(SpellEffIndex effIndex)
{
    if (SpellDummyTriggered(effIndex))
        return;

    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    // TODO: we must implement hunter pet summon at login there (spell 6962)

    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            // process discovery spells
            if (m_spellInfo->Categories.Mechanic == MECHANIC_DISCOVERY || m_spellInfo->IsExplicitDiscovery())
            {
                if (Player* player = m_caster->ToPlayer())
                {
                    // learn random explicit discovery recipe (if any)
                    if (uint32 discoveredSpell = GetExplicitDiscoverySpell(m_spellInfo->Id, player))
                    {
                        player->learnSpell(discoveredSpell, false);
                        return;
                    }
                }
            }

            switch (m_spellInfo->Id)
            {
                case 143483: //Paragons Purpose heal, Paragons of the Klaxxi[SO]
                {
                    if (!unitTarget)
                        return;

                    unitTarget->CastSpell(unitTarget, 143482, true); //Paragons Purpose buff dmg
                    return;
                }
                // Seafood Magnifique Feast
                case 87806:
                {
                    if (!unitTarget)
                        return;

                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        float stat = 0.0f;
                        uint32 spellId = 0;

                        if (unitTarget->GetStat(STAT_STRENGTH) > stat) { spellId = 87584; stat = unitTarget->GetStat(STAT_STRENGTH); }
                        if (unitTarget->GetStat(STAT_AGILITY)  > stat) { spellId = 87586; stat = unitTarget->GetStat(STAT_AGILITY); }
                        if (unitTarget->GetStat(STAT_INTELLECT)  > stat) { spellId = 87587; stat = unitTarget->GetStat(STAT_INTELLECT); }
                        if (unitTarget->GetStat(STAT_SPIRIT)  > stat) { spellId = 87588; }
                        
                        if (spellId)
                            unitTarget->CastSpell(unitTarget, spellId, true);
                    }
                    return;
                }
                // Banquet of the Grill
                case 126532:
                {
                    if (!unitTarget)
                        return;

                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        float stat = 0.0f;
                        uint32 spellId = 0;

                        if (unitTarget->GetStat(STAT_STRENGTH) > stat) { spellId = 104263; stat = unitTarget->GetStat(STAT_STRENGTH); }
                        if (unitTarget->GetStat(STAT_AGILITY)  > stat) { spellId = 104286; stat = unitTarget->GetStat(STAT_AGILITY); }
                        if (unitTarget->GetStat(STAT_INTELLECT)  > stat) { spellId = 104266; stat = unitTarget->GetStat(STAT_INTELLECT); }
                        if (unitTarget->GetStat(STAT_SPIRIT)  > stat) { spellId = 104291; }
                        
                        if (spellId)
                            unitTarget->CastSpell(unitTarget, spellId, true);
                    }
                    return;
                }
                // Great Pandaren Banquet
                case 104924:
                {
                    if (!unitTarget)
                        return;

                    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        float stat = 0.0f;
                        uint32 spellId = 0;

                        if (unitTarget->GetStat(STAT_STRENGTH) > stat) { spellId = 104284; stat = unitTarget->GetStat(STAT_STRENGTH); }
                        if (unitTarget->GetStat(STAT_AGILITY)  > stat) { spellId = 104287; stat = unitTarget->GetStat(STAT_AGILITY); }
                        if (unitTarget->GetStat(STAT_INTELLECT)  > stat) { spellId = 104290; stat = unitTarget->GetStat(STAT_INTELLECT); }
                        if (unitTarget->GetStat(STAT_SPIRIT)  > stat) { spellId = 104292; }
                        
                        if (spellId)
                            unitTarget->CastSpell(unitTarget, spellId, true);
                    }
                    return;
                }
                case 104126:    //Mop.Quest.Monkey Wisdom
                {
                    if (Player *player = unitTarget->ToPlayer())
                    {
                        const uint32 text[6] = {2000009994, 2000009995, 2000009996, 2000009997, 2000009998, 2000009999};
                        LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
                        std::string texta(sObjectMgr->GetTrinityString(text[urand(0, 5)], loc_idx));
                        WorldPacket data(SMSG_CHAT, 200);
                        player->BuildPlayerChat(&data, CHAT_MSG_RAID_BOSS_WHISPER, texta, LANG_UNIVERSAL);
                        player->GetSession()->SendPacket(&data);
                    }
                    return;
                }
                // Glyph of Scourge Strike
                case 69961:
                {
                    Unit::AuraEffectList const &mPeriodic = unitTarget->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
                    for (Unit::AuraEffectList::const_iterator i = mPeriodic.begin(); i != mPeriodic.end(); ++i)
                    {
                        AuraEffect const* aurEff = *i;
                        SpellInfo const* spellInfo = aurEff->GetSpellInfo();
                        // search our Blood Plague and Frost Fever on target
                        if (spellInfo->SpellFamilyName == SPELLFAMILY_DEATHKNIGHT && spellInfo->SpellFamilyFlags[2] & 0x2 &&
                            aurEff->GetCasterGUID() == m_caster->GetGUID())
                        {
                            uint32 countMin = aurEff->GetBase()->GetMaxDuration();
                            uint32 countMax = spellInfo->GetMaxDuration();

                            // this Glyph
                            countMax += 9000;
                            // talent Epidemic
                            if (AuraEffect const* epidemic = m_caster->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_DEATHKNIGHT, 234, EFFECT_0))
                                countMax += epidemic->GetAmount();

                            if (countMin < countMax)
                            {
                                aurEff->GetBase()->SetDuration(aurEff->GetBase()->GetDuration() + 3000);
                                aurEff->GetBase()->SetMaxDuration(countMin + 3000);
                            }
                        }
                    }
                    return;
                }
                case 45204: // Clone Me!
                    unitTarget->CastSpell(m_caster, damage, true);
                    break;
                case 55693:                                 // Remove Collapsing Cave Aura
                    if (!unitTarget)
                        return;
                    unitTarget->RemoveAurasDueToSpell(m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValue());
                    break;
                // PX-238 Winter Wondervolt TRAP
                case 26275:
                {
                    uint32 spells[4] = {26272, 26157, 26273, 26274};

                    // check presence
                    for (uint8 j = 0; j < 4; ++j)
                        if (unitTarget->HasAuraEffect(spells[j], 0))
                            return;

                    // select spell
                    uint32 iTmpSpellId = spells[urand(0, 3)];

                    // cast
                    unitTarget->CastSpell(unitTarget, iTmpSpellId, true);
                    return;
                }
                // Bending Shinbone
                case 8856:
                {
                    if (!itemTarget && m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 spell_id = roll_chance_i(20) ? 8854 : 8855;

                    m_caster->CastSpell(m_caster, spell_id, true, NULL);
                    return;
                }
                // Brittle Armor - need remove one 24575 Brittle Armor aura
                case 24590:
                    unitTarget->RemoveAuraFromStack(24575);
                    return;
                // Mercurial Shield - need remove one 26464 Mercurial Shield aura
                case 26465:
                    unitTarget->RemoveAuraFromStack(26464);
                    return;
                // Shadow Flame (All script effects, not just end ones to prevent player from dodging the last triggered spell)
                case 22539:
                case 22972:
                case 22975:
                case 22976:
                case 22977:
                case 22978:
                case 22979:
                case 22980:
                case 22981:
                case 22982:
                case 22983:
                case 22984:
                case 22985:
                {
                    if (!unitTarget || !unitTarget->isAlive())
                        return;

                    // Onyxia Scale Cloak
                    if (unitTarget->HasAura(22683))
                        return;

                    // Shadow Flame
                    m_caster->CastSpell(unitTarget, 22682, true);
                    return;
                }
                case 17512: // Piccolo of the Flaming Fire
                case 51508: // Party G.R.E.N.A.D.E.
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                        return;
                    unitTarget->HandleEmoteCommand(EMOTE_ONESHOT_DANCE);
                    return;
                }
                // Decimate
                case 28374:
                case 54426:
                    if (unitTarget)
                    {
                        int32 damage = int32(unitTarget->GetHealth()) - int32(unitTarget->CountPctFromMaxHealth(5));
                        if (damage > 0)
                            m_caster->CastCustomSpell(28375, SPELLVALUE_BASE_POINT0, damage, unitTarget);
                    }
                    return;
                // Mirren's Drinking Hat
                case 29830:
                {
                    uint32 item = 0;
                    switch (urand(1, 6))
                    {
                        case 1:
                        case 2:
                        case 3:
                            item = 23584; break;            // Loch Modan Lager
                        case 4:
                        case 5:
                            item = 23585; break;            // Stouthammer Lite
                        case 6:
                            item = 23586; break;            // Aerie Peak Pale Ale
                    }
                    if (item)
                        DoCreateItem(effIndex, item);
                    break;
                }
                // Plant Warmaul Ogre Banner
                case 32307:
                    if (Player* caster = m_caster->ToPlayer())
                    {
                        caster->RewardPlayerAndGroupAtEvent(18388, unitTarget);
                        if (Creature* target = unitTarget->ToCreature())
                        {
                            target->setDeathState(CORPSE);
                            target->RemoveCorpse();
                        }
                    }
                    break;
                // Mug Transformation
                case 41931:
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint8 bag = 19;
                    uint8 slot = 0;
                    Item* item = NULL;

                    while (bag) // 256 = 0 due to var type
                    {
                        item = m_caster->ToPlayer()->GetItemByPos(bag, slot);
                        if (item && item->GetEntry() == 38587)
                            break;

                        ++slot;
                        if (slot == 39)
                        {
                            slot = 0;
                            ++bag;
                        }
                    }
                    if (bag)
                    {
                        if (m_caster->ToPlayer()->GetItemByPos(bag, slot)->GetCount() == 1) m_caster->ToPlayer()->RemoveItem(bag, slot, true);
                        else m_caster->ToPlayer()->GetItemByPos(bag, slot)->SetCount(m_caster->ToPlayer()->GetItemByPos(bag, slot)->GetCount() - 1);
                        // Spell 42518 (Braufest - Gratisprobe des Braufest herstellen)
                        m_caster->CastSpell(m_caster, 42518, true);
                        return;
                    }
                    break;
                }
                // Brutallus - Burn
                case 45141:
                case 45151:
                {
                    //Workaround for Range ... should be global for every ScriptEffect
                    float radius = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius();
                    if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER && unitTarget->GetDistance(m_caster) >= radius && !unitTarget->HasAura(46394) && unitTarget != m_caster)
                        unitTarget->CastSpell(unitTarget, 46394, true);

                    break;
                }
                // Goblin Weather Machine
                case 46203:
                {
                    if (!unitTarget)
                        return;

                    uint32 spellId = 0;
                    switch (rand() % 4)
                    {
                        case 0: spellId = 46740; break;
                        case 1: spellId = 46739; break;
                        case 2: spellId = 46738; break;
                        case 3: spellId = 46736; break;
                    }
                    unitTarget->CastSpell(unitTarget, spellId, true);
                    break;
                }
                // 5, 000 Gold
                case 46642:
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                        return;

                    unitTarget->ToPlayer()->ModifyMoney(5000 * GOLD);

                    break;
                }
                // Roll Dice - Decahedral Dwarven Dice
                case 47770:
                {
                    char buf[128];
                    const char *gender = "his";
                    if (m_caster->getGender() > 0)
                        gender = "her";
                    sprintf(buf, "%s rubs %s [Decahedral Dwarven Dice] between %s hands and rolls. One %u and one %u.", m_caster->GetName(), gender, gender, urand(1, 10), urand(1, 10));
                    m_caster->MonsterTextEmote(buf, ObjectGuid::Empty);
                    break;
                }
                // Roll 'dem Bones - Worn Troll Dice
                case 47776:
                {
                    char buf[128];
                    const char *gender = "his";
                    if (m_caster->getGender() > 0)
                        gender = "her";
                    sprintf(buf, "%s causually tosses %s [Worn Troll Dice]. One %u and one %u.", m_caster->GetName(), gender, urand(1, 6), urand(1, 6));
                    m_caster->MonsterTextEmote(buf, ObjectGuid::Empty);
                    break;
                }
                // Death Knight Initiate Visual
                case 51519:
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT)
                        return;

                    uint32 iTmpSpellId = 0;
                    switch (unitTarget->GetDisplayId())
                    {
                        case 25369: iTmpSpellId = 51552; break; // bloodelf female
                        case 25373: iTmpSpellId = 51551; break; // bloodelf male
                        case 25363: iTmpSpellId = 51542; break; // draenei female
                        case 25357: iTmpSpellId = 51541; break; // draenei male
                        case 25361: iTmpSpellId = 51537; break; // dwarf female
                        case 25356: iTmpSpellId = 51538; break; // dwarf male
                        case 25372: iTmpSpellId = 51550; break; // forsaken female
                        case 25367: iTmpSpellId = 51549; break; // forsaken male
                        case 25362: iTmpSpellId = 51540; break; // gnome female
                        case 25359: iTmpSpellId = 51539; break; // gnome male
                        case 25355: iTmpSpellId = 51534; break; // human female
                        case 25354: iTmpSpellId = 51520; break; // human male
                        case 25360: iTmpSpellId = 51536; break; // nightelf female
                        case 25358: iTmpSpellId = 51535; break; // nightelf male
                        case 25368: iTmpSpellId = 51544; break; // orc female
                        case 25364: iTmpSpellId = 51543; break; // orc male
                        case 25371: iTmpSpellId = 51548; break; // tauren female
                        case 25366: iTmpSpellId = 51547; break; // tauren male
                        case 25370: iTmpSpellId = 51545; break; // troll female
                        case 25365: iTmpSpellId = 51546; break; // troll male
                        default: return;
                    }

                    unitTarget->CastSpell(unitTarget, iTmpSpellId, true);
                    Creature* npc = unitTarget->ToCreature();
                    npc->LoadEquipment();
                    return;
                }
                // Emblazon Runeblade
                case 51770:
                {
                    if (!m_originalCaster)
                        return;

                    m_originalCaster->CastSpell(m_originalCaster, damage, false);
                    break;
                }
                // Deathbolt from Thalgran Blightbringer
                // reflected by Freya's Ward
                // Retribution by Sevenfold Retribution
                case 51854:
                {
                    if (!unitTarget)
                        return;
                    if (unitTarget->HasAura(51845))
                        unitTarget->CastSpell(m_caster, 51856, true);
                    else
                        m_caster->CastSpell(unitTarget, 51855, true);
                    break;
                }
                // Summon Ghouls On Scarlet Crusade
                case 51904:
                {
                    if (!m_targets.HasDst())
                        return;

                    float x, y, z;
                    float radius = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius();
                    for (uint8 i = 0; i < 15; ++i)
                    {
                        m_caster->GetRandomPoint(*destTarget, radius, x, y, z);
                        m_caster->CastSpell(x, y, z, 54522, true);
                    }
                    break;
                }
                case 52173: // Coyote Spirit Despawn
                case 60243: // Blood Parrot Despawn
                    if (unitTarget->GetTypeId() == TYPEID_UNIT && unitTarget->ToCreature()->isSummon())
                        unitTarget->ToTempSummon()->UnSummon();
                    return;
                case 52479: // Gift of the Harvester
                    if (unitTarget && m_originalCaster)
                        m_originalCaster->CastSpell(unitTarget, urand(0, 1) ? damage : 52505, true);
                    return;
                case 53110: // Devour Humanoid
                    if (unitTarget)
                        unitTarget->CastSpell(m_caster, damage, true);
                    return;
                case 57347: // Retrieving (Wintergrasp RP-GG pickup spell)
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT || m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    unitTarget->ToCreature()->DespawnOrUnsummon();

                    return;
                }
                case 57349: // Drop RP-GG (Wintergrasp RP-GG at death drop spell)
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    // Delete item from inventory at death
                    m_caster->ToPlayer()->DestroyItemCount(damage, 5, true);

                    return;
                }
                case 58418:                                 // Portal to Orgrimmar
                case 58420:                                 // Portal to Stormwind
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER || effIndex != 0)
                        return;

                    uint32 spellID = m_spellInfo->GetEffect(EFFECT_0, m_diffMode)->CalcValue();
                    uint32 questID = m_spellInfo->GetEffect(EFFECT_1, m_diffMode)->CalcValue();

                    if (unitTarget->ToPlayer()->GetQuestStatus(questID) == QUEST_STATUS_COMPLETE)
                        unitTarget->CastSpell(unitTarget, spellID, true);

                    return;
                }
                case 58941:                                 // Rock Shards
                    if (unitTarget && m_originalCaster)
                    {
                        for (uint32 i = 0; i < 3; ++i)
                        {
                            m_originalCaster->CastSpell(unitTarget, 58689, true);
                            m_originalCaster->CastSpell(unitTarget, 58692, true);
                        }
                        if (((InstanceMap*)m_originalCaster->GetMap())->GetDifficultyID() == DIFFICULTY_NORMAL)
                        {
                            m_originalCaster->CastSpell(unitTarget, 58695, true);
                            m_originalCaster->CastSpell(unitTarget, 58696, true);
                        }
                        else
                        {
                            m_originalCaster->CastSpell(unitTarget, 60883, true);
                            m_originalCaster->CastSpell(unitTarget, 60884, true);
                        }
                    }
                    return;
                case 58983: // Big Blizzard Bear
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                        return;

                    // Prevent stacking of mounts and client crashes upon dismounting
                    unitTarget->RemoveAurasByType(SPELL_AURA_MOUNTED);

                    // Triggered spell id dependent on riding skill
                    if (uint16 skillval = unitTarget->ToPlayer()->GetSkillValue(SKILL_RIDING))
                    {
                        if (skillval >= 150)
                            unitTarget->CastSpell(unitTarget, 58999, true);
                        else
                            unitTarget->CastSpell(unitTarget, 58997, true);
                    }
                    return;
                }
                case 63845: // Create Lance
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    if (m_caster->ToPlayer()->GetTeam() == ALLIANCE)
                        m_caster->CastSpell(m_caster, 63914, true);
                    else
                        m_caster->CastSpell(m_caster, 63919, true);
                    return;
                }
                case 59317:                                 // Teleporting
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
                        return;

                    // return from top
                    if (unitTarget->ToPlayer()->GetAreaId() == 4637)
                        unitTarget->CastSpell(unitTarget, 59316, true);
                    // teleport atop
                    else
                        unitTarget->CastSpell(unitTarget, 59314, true);

                    return;
                case 143626:                                // Celestial Cloth and Its Uses
                case 143644:                                // Hardened Magnificent Hide and Its Uses
                case 143646:                                // Balanced Trillium Ingot and Its Uses
                {
                    Player* player = m_caster->ToPlayer();
                    if (!player)
                        return;

                    uint32 spellToRecipe[][3] = {
                        {143626, 143011, 146925},
                        {143644, 142976, 146923},
                        {143646, 143255, 146921},
                        {0, 0, 0}
                    };

                    uint32 learnSpell = 0;
                    uint32 acceleratedSpell = 0;
                    for (uint32 i = 0; spellToRecipe[i][0] != 0; ++i)
                        if (spellToRecipe[i][0] == m_spellInfo->Id)
                        {
                            learnSpell = spellToRecipe[i][1];
                            acceleratedSpell = spellToRecipe[i][2];
                            break;
                        }

                    if (!learnSpell || player->HasSpell(learnSpell))
                        return;

                    player->learnSpell(learnSpell, false);
                    player->learnSpell(acceleratedSpell, false);

                    // learn random explicit discovery recipe (if any)
                    if (uint32 discoveredSpell = GetExplicitDiscoverySpell(learnSpell, player))
                        player->learnSpell(discoveredSpell, false);
                    return;
                }
                case 62482: // Grab Crate
                {
                    if (unitTarget)
                    {
                        if (Unit* seat = m_caster->GetVehicleBase())
                        {
                            if (Unit* parent = seat->GetVehicleBase())
                            {
                                // TODO: a hack, range = 11, should after some time cast, otherwise too far
                                m_caster->CastSpell(parent, 62496, true);
                                unitTarget->CastSpell(parent, m_spellInfo->GetEffect(EFFECT_0, m_diffMode)->CalcValue());
                            }
                        }
                    }
                    return;
                }
                case 66545: //Summon Memory
                {
                    uint8 uiRandom = urand(0, 25);
                    uint32 uiSpells[26] = {66704, 66705, 66706, 66707, 66709, 66710, 66711, 66712, 66713, 66714, 66715, 66708, 66708, 66691, 66692, 66694, 66695, 66696, 66697, 66698, 66699, 66700, 66701, 66702, 66703, 66543};

                    m_caster->CastSpell(m_caster, uiSpells[uiRandom], true);
                    break;
                }
                case 45668:                                 // Ultra-Advanced Proto-Typical Shortening Blaster
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT)
                        return;

                    if (roll_chance_i(50))                  // chance unknown, using 50
                        return;

                    static uint32 const spellPlayer[5] =
                    {
                        45674,                            // Bigger!
                        45675,                            // Shrunk
                        45678,                            // Yellow
                        45682,                            // Ghost
                        45684                             // Polymorph
                    };

                    static uint32 const spellTarget[5] =
                    {
                        45673,                            // Bigger!
                        45672,                            // Shrunk
                        45677,                            // Yellow
                        45681,                            // Ghost
                        45683                             // Polymorph
                    };

                    m_caster->CastSpell(m_caster, spellPlayer[urand(0, 4)], true);
                    unitTarget->CastSpell(unitTarget, spellTarget[urand(0, 4)], true);
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_POTION:
        {
            switch (m_spellInfo->Id)
            {
                // Netherbloom
                case 28702:
                {
                    if (!unitTarget)
                        return;
                    // 25% chance of casting a random buff
                    if (roll_chance_i(75))
                        return;

                    // triggered spells are 28703 to 28707
                    // Note: some sources say, that there was the possibility of
                    //       receiving a debuff. However, this seems to be removed by a patch.
                    const uint32 spellid = 28703;

                    // don't overwrite an existing aura
                    for (uint8 i = 0; i < 5; ++i)
                        if (unitTarget->HasAura(spellid + i))
                            return;
                    unitTarget->CastSpell(unitTarget, spellid + urand(0, 4), true);
                    break;
                }

                // Nightmare Vine
                case 28720:
                {
                    if (!unitTarget)
                        return;
                    // 25% chance of casting Nightmare Pollen
                    if (roll_chance_i(75))
                        return;
                    unitTarget->CastSpell(unitTarget, 28721, true);
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            // Shattering Throw
            if (m_spellInfo->SpellFamilyFlags[1] & 0x00400000)
            {
                if (!unitTarget)
                    return;
                // remove shields, will still display immune to damage part
                unitTarget->RemoveAurasWithMechanic(1 << MECHANIC_MAGICAL_IMMUNITY, AURA_REMOVE_BY_ENEMY_SPELL);
                return;
            }
            break;
        }
    }

    // normal DB scripted effect
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell ScriptStart spellid %u in EffectScriptEffect(%u)", m_spellInfo->Id, effIndex);
    m_caster->GetMap()->ScriptsStart(sSpellScripts, uint32(m_spellInfo->Id | (effIndex << 24)), m_caster, unitTarget);
}

void Spell::EffectSanctuary(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    unitTarget->getHostileRefManager().UpdateVisibility();

    Unit::AttackerSet const& attackers = unitTarget->getAttackers();
    for (Unit::AttackerSet::const_iterator itr = attackers.begin(); itr != attackers.end();)
    {
        if (!(*itr)->canSeeOrDetect(unitTarget))
            (*(itr++))->AttackStop();
        else
            ++itr;
    }

    //unitTarget->m_lastSanctuaryTime = getMSTime();
}

void Spell::EffectAddComboPoints(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (!m_caster->m_movedPlayer)
        return;

    if (m_spellInfo->Id == 1752 && unitTarget->HasAura(84617, m_caster->GetGUID()) && roll_chance_i(20)) //Debuff Revealing Strike add CP Sinister Strike
    {
        if (m_caster->HasAura(114015) && m_caster->ToPlayer() && m_caster->ToPlayer()->GetComboPoints(m_spellInfo->Id) >= 4)
            m_caster->CastSpell(m_caster, 115189, true);
        else
            damage += 1;
    }
    if (damage <= 0)
        return;

    if (m_spellInfo->IsTargetingArea())
    {
        if (Unit* target = m_caster->m_movedPlayer->GetSelectedUnit())
        {
            if (unitTarget->GetGUID() != target->GetGUID())
                return;
        }
        else
            return;
    }

    if (/*m_spellInfo->Id == 139546 || */m_spellInfo->Id == 144859) //Add CP after use old CP
        m_caster->m_movedPlayer->SaveAddComboPoints(damage);
    else
        m_caster->m_movedPlayer->AddComboPoints(unitTarget, damage, this);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectAddComboPoints damage %i, Id %i", damage, m_spellInfo->Id);
}

void Spell::EffectDuel(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || m_caster->GetTypeId() != TYPEID_PLAYER || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* caster = m_caster->ToPlayer();
    Player* target = unitTarget->ToPlayer();

    // caster or target already have requested duel
    if (caster->duel || target->duel || !target->GetSocial() || target->GetSocial()->HasIgnore(caster->GetGUID()))
        return;

    // Players can only fight a duel in zones with this flag
    AreaTableEntry const* casterAreaEntry = GetAreaEntryByAreaID(caster->GetAreaId());
    if (casterAreaEntry && !(casterAreaEntry->Flags[0] & AREA_FLAG_ALLOW_DUELS))
    {
        SendCastResult(SPELL_FAILED_NO_DUELING);            // Dueling isn't allowed here
        return;
    }

    AreaTableEntry const* targetAreaEntry = GetAreaEntryByAreaID(target->GetAreaId());
    if (targetAreaEntry && !(targetAreaEntry->Flags[0] & AREA_FLAG_ALLOW_DUELS))
    {
        SendCastResult(SPELL_FAILED_NO_DUELING);            // Dueling isn't allowed here
        return;
    }

    //CREATE DUEL FLAG OBJECT
    GameObject* pGameObj = new GameObject;

    uint32 gameobject_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    Map* map = m_caster->GetMap();
    if (!pGameObj->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), gameobject_id,
        map, m_caster->GetPhaseMask(),
        m_caster->GetPositionX() + (unitTarget->GetPositionX() - m_caster->GetPositionX()) / 2,
        m_caster->GetPositionY() + (unitTarget->GetPositionY() - m_caster->GetPositionY()) / 2,
        m_caster->GetPositionZ(),
        m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY))
    {
        delete pGameObj;
        return;
    }

    pGameObj->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, m_caster->getFaction());
    pGameObj->SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, m_caster->getLevel() + 1);
    int32 duration = m_spellInfo->GetDuration();
    pGameObj->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);

    ExecuteLogEffectSummonObject(effIndex, pGameObj);

    m_caster->AddGameObject(pGameObj);
    map->AddToMap(pGameObj);

    WorldPackets::Duel::DuelRequested duelRequested;
    duelRequested.ArbiterGUID = pGameObj->GetGUID();
    duelRequested.RequestedByGUID = caster->GetGUID();
    duelRequested.RequestedByWowAccount = caster->GetSession()->GetBattlenetAccountGUID();

    WorldPacket const* worldPacket = duelRequested.Write();
    caster->GetSession()->SendPacket(worldPacket);
    target->GetSession()->SendPacket(worldPacket);

    // create duel-info
    DuelInfo* duel = new DuelInfo;
    duel->initiator = caster;
    duel->opponent = target;
    duel->startTime = 0;
    duel->startTimer = 0;
    duel->isMounted = (GetSpellInfo()->Id == 62875); // Mounted Duel
    caster->duel = duel;

    DuelInfo* duel2 = new DuelInfo;
    duel2->initiator = caster;
    duel2->opponent = caster;
    duel2->startTime = 0;
    duel2->startTimer = 0;
    duel2->isMounted = (GetSpellInfo()->Id == 62875); // Mounted Duel
    target->duel = duel2;

    caster->SetGuidValue(PLAYER_FIELD_DUEL_ARBITER, pGameObj->GetGUID());
    target->SetGuidValue(PLAYER_FIELD_DUEL_ARBITER, pGameObj->GetGUID());

    sScriptMgr->OnPlayerDuelRequest(target, caster);
}

void Spell::EffectStuck(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    if (!sWorld->getBoolConfig(CONFIG_CAST_UNSTUCK))
        return;

    Player* target = (Player*)m_caster;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell Effect: Stuck");
    sLog->outInfo(LOG_FILTER_SPELLS_AURAS, "Player %s (guid %u) used auto-unstuck future at map %u (%f, %f, %f)", target->GetName(), target->GetGUIDLow(), m_caster->GetMapId(), m_caster->GetPositionX(), target->GetPositionY(), target->GetPositionZ());

    if (target->isInFlight())
        return;

    target->TeleportTo(target->GetStartPosition(), TELE_TO_SPELL);
    // homebind location is loaded always
    // target->TeleportTo(target->m_homebindMapId, target->m_homebindX, target->m_homebindY, target->m_homebindZ, target->GetOrientation(), (m_caster == m_caster ? TELE_TO_SPELL : 0));

    // Stuck spell trigger Hearthstone cooldown
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(8690);
    if (!spellInfo)
        return;
    Spell spell(target, spellInfo, TRIGGERED_FULL_MASK);
    spell.SendSpellCooldown();
}

void Spell::EffectSummonPlayer(SpellEffIndex /*effIndex*/)
{
    // workaround - this effect should not use target map
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    // Evil Twin (ignore player summon, but hide this for summoner)
    if (unitTarget->HasAura(23445))
        return;

    float x, y, z;
    m_caster->GetPosition(x, y, z);

    unitTarget->ToPlayer()->SetSummonPoint(m_caster->GetMapId(), x, y, z);

    WorldPackets::Misc::SummonRequest request;
    request.SummonerGUID = m_caster->GetGUID();
    request.SummonerVirtualRealmAddress = GetVirtualRealmAddress();
    request.AreaID = m_caster->GetZoneId();
    request.ConfirmSummon = false; //@TODO
    unitTarget->ToPlayer()->GetSession()->SendPacket(request.Write());
}

void Spell::EffectActivateObject(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!gameObjTarget || m_spellInfo->Id == gameObjTarget->GetGOInfo()->GetSpell())
        return;

    ScriptInfo activateCommand;
    activateCommand.command = SCRIPT_COMMAND_ACTIVATE_OBJECT;

    // int32 unk = m_spellInfo->GetEffect(effIndex, m_diffMode).MiscValue; // This is set for EffectActivateObject spells; needs research

    switch (m_spellInfo->Id)
    {
        case 144229: //spoils_of_pandaria open box
            return;
        case 105847: // Seal Armor Breach (left), Spine of Deathwing, Dragon Soul
        case 105848: // Seal Armor Breach (right), Spine of Deathwing, Dragon Soul
        case 105363: // Breach Armor (left), Spine of Deathwing, Dragon Soul
        case 105385: // Breach Armor (right), Spine of Deathwing, Dragon Soul
        case 105366: // Plate Fly Off (left), Spine of Deathwing, Dragon Soul
        case 105384: // Plate Fly Off (right), Spine of Deathwing, Dragon Soul
            if (gameObjTarget->GetEntry() == 209623 || gameObjTarget->GetEntry() == 209631 || gameObjTarget->GetEntry() == 209632)
            {
                // Send anim kit
                gameObjTarget->SendGameObjectActivateAnimKit(m_spellInfo->Effects[0].MiscValueB, false);
                return;
            }
            break;
    }

    gameObjTarget->GetMap()->ScriptCommandStart(activateCommand, 0, m_caster, gameObjTarget);
}

void Spell::EffectApplyGlyph(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER || m_misc.Raw.Data[0] >= MAX_GLYPH_SLOT_INDEX)
        return;

    Player* player = (Player*)m_caster;

    // glyph sockets level requirement
    uint8 minLevel = 0;
    switch (m_misc.Raw.Data[0])
    {
        case 0:
        case 1:
        case 6: minLevel = 25; break;
        case 2:
        case 3:
        case 7: minLevel = 50; break;
        case 4:
        case 5:
        case 8: minLevel = 75; break;
    }

    if (minLevel && m_caster->getLevel() < minLevel)
    {
        SendCastResult(SPELL_FAILED_GLYPH_SOCKET_LOCKED);
        return;
    }

    // apply new one
    if (uint32 glyph = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue)
    {
        if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyph))
        {
            if (GlyphSlotEntry const* gs = sGlyphSlotStore.LookupEntry(player->GetGlyphSlot(m_misc.Raw.Data[0])))
            {
                if (gp->TypeFlags != gs->TypeFlags)
                {
                    SendCastResult(SPELL_FAILED_INVALID_GLYPH);
                    return;                                 // glyph slot mismatch
                }
            }

            // remove old glyph
            if (uint32 oldglyph = player->GetGlyph(player->GetActiveSpec(), m_misc.Raw.Data[0]))
            {
                if (GlyphPropertiesEntry const* old_gp = sGlyphPropertiesStore.LookupEntry(oldglyph))
                {
                    player->removeSpell(old_gp->SpellId);
                    //player->RemoveAurasDueToSpell(old_gp->SpellId);
                    player->SetGlyph(m_misc.Raw.Data[0], 0);
                }
            }

            player->learnSpell(gp->SpellId, false);
            //player->CastSpell(m_caster, gp->SpellId, true);
            player->SetGlyph(m_misc.Raw.Data[0], glyph);
            player->SendTalentsInfoData(false);
        }
    }
    else
    {
        // remove old glyph
        if (uint32 oldglyph = player->GetGlyph(player->GetActiveSpec(), m_misc.Raw.Data[0]))
        {
            if (GlyphPropertiesEntry const* old_gp = sGlyphPropertiesStore.LookupEntry(oldglyph))
            {
                player->removeSpell(old_gp->SpellId);
                //player->RemoveAurasDueToSpell(old_gp->SpellId);
                player->SetGlyph(m_misc.Raw.Data[0], 0);
                player->SendTalentsInfoData(false);
            }
        }
    }
}

void Spell::EffectEnchantHeldItem(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    // this is only item spell effect applied to main-hand weapon of target player (players in area)
    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* item_owner = (Player*)unitTarget;
    Item* item = item_owner->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

    if (!item)
        return;

    // must be equipped
    if (!item->IsEquipped())
        return;

    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue)
    {
        uint32 enchant_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
        int32 duration = m_spellInfo->GetDuration();          //Try duration index first ..
        if (!duration)
            duration = damage;//+1;            //Base points after ..
        if (!duration)
            duration = 10;                                  //10 seconds for enchants which don't have listed duration

        SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!pEnchant)
            return;

        // Always go to temp enchantment slot
        EnchantmentSlot slot = TEMP_ENCHANTMENT_SLOT;

        // Enchantment will not be applied if a different one already exists
        if (item->GetEnchantmentId(slot) && item->GetEnchantmentId(slot) != enchant_id)
            return;

        // Apply the temporary enchantment
        item->SetEnchantment(slot, enchant_id, duration*IN_MILLISECONDS, 0, m_caster->GetGUID());
        item_owner->ApplyEnchantment(item, slot, true);
    }
}

void Spell::EffectDisEnchant(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!itemTarget || !itemTarget->GetTemplate()->DisenchantID)
        return;

    if (Player* caster = m_caster->ToPlayer())
    {
        caster->UpdateCraftSkill(m_spellInfo->Id);
        caster->SendLoot(itemTarget->GetGUID(), LOOT_DISENCHANTING);
    }

    // item will be removed at disenchanting end
}

void Spell::EffectInebriate(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();
    uint8 currentDrunk = player->GetDrunkValue();
    uint8 drunkMod = damage;
    if (currentDrunk + drunkMod > 100)
    {
        currentDrunk = 100;
        if (rand_chance() < 25.0f)
            player->CastSpell(player, 67468, false);    // Drunken Vomit
    }
    else
        currentDrunk += drunkMod;

    player->SetDrunkValue(currentDrunk, m_CastItem ? m_CastItem->GetEntry() : 0);
}

void Spell::EffectFeedPet(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    Item* foodItem = itemTarget;
    if (!foodItem)
        return;

    Pet* pet = player->GetPet();
    if (!pet)
        return;

    if (!pet->isAlive())
        return;

    ExecuteLogEffectDestroyItem(effIndex, foodItem->GetEntry());

    uint32 count = 1;
    player->DestroyItemCount(foodItem, count, true);
    // TODO: fix crash when a spell has two effects, both pointed at the same item target

    m_caster->CastSpell(pet, m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell, true);
}

void Spell::EffectDismissPet(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isPet())
        return;

    Pet* pet = unitTarget->ToPet();

    ExecuteLogEffectUnsummonObject(effIndex, pet);
    if (Player* player = pet->GetOwner()->ToPlayer())
    {
        player->RemovePet(pet);
        player->m_currentPetNumber = 0;
    }
}

void Spell::EffectSummonObject(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (!m_caster)
        return;

    uint8 slot = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValueB;
    if (slot >= MAX_GAMEOBJECT_SLOT)
        return;

    uint32 go_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    float x, y, z, o;

    ObjectGuid guid = m_caster->m_ObjectSlot[slot];
    if (!guid.IsEmpty())
    {
        if (GameObject* obj = m_caster->GetMap()->GetGameObject(guid))
        {
            // Recast case - null spell id to make auras not be removed on object remove from world
            if (m_spellInfo->Id == obj->GetSpellId())
                obj->SetSpellId(0);
            m_caster->RemoveGameObject(obj, true);
        }
        m_caster->m_ObjectSlot[slot].Clear();
    }

    GameObject* pGameObj = new GameObject;

    // If dest location if present
    if (m_targets.HasDst())
        destTarget->GetPosition(x, y, z, o);
    // Summon in random point all other units if location present
    else
    {
        m_caster->GetClosePoint(x, y, z, DEFAULT_WORLD_OBJECT_SIZE);
        o = m_caster->GetOrientation();
    }

    Map* map = m_caster->GetMap();
    if (!pGameObj->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), go_id, map,
        m_caster->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY))
    {
        delete pGameObj;
        return;
    }

    //pGameObj->SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, m_caster->getLevel());
    int32 duration = m_spellInfo->GetDuration();
    pGameObj->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);
    m_caster->AddGameObject(pGameObj);

    // object only for SPELL_EFFECT_OBJECT_WITH_PERSONAL_VISIBILITY
    if (m_currentExecutedEffect == SPELL_EFFECT_OBJECT_WITH_PERSONAL_VISIBILITY && m_caster->GetTypeId() == TYPEID_PLAYER)
        pGameObj->AddPlayerInPersonnalVisibilityList(m_caster->GetGUID());

    ExecuteLogEffectSummonObject(effIndex, pGameObj);

    map->AddToMap(pGameObj);

    m_caster->m_ObjectSlot[slot] = pGameObj->GetGUID();
}

void Spell::EffectSurvey(SpellEffIndex effIndex)
{
    if (!sWorld->getBoolConfig(CONFIG_ARCHAEOLOGY_ENABLED))
        return;

    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (!m_caster)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    uint8 slot = 4;
    uint32 go_id;

    float x, y, z, o;
    x = m_caster->GetPositionX();
    y = m_caster->GetPositionY();
    z = m_caster->GetPositionZ();
    o = m_caster->GetOrientation();

    int32 duration;
    if (!player->OnSurvey(go_id, x, y, z, o))
        duration = 10000;
    else
        duration = 60000;

    if (!go_id)
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell::EffectSurvey: no go id for x: %f y: %f z: %f map: %u",
                       m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), m_caster->GetMapId());
        return;
    }

    ObjectGuid const& guid = m_caster->m_ObjectSlot[slot];
    if (!guid.IsEmpty())
    {
        if (GameObject* obj = m_caster->GetMap()->GetGameObject(guid))
        {
            // Recast case - null spell id to make auras not be removed on object remove from world
            if (m_spellInfo->Id == obj->GetSpellId())
                obj->SetSpellId(0);
            m_caster->RemoveGameObject(obj, true);
        }
        m_caster->m_ObjectSlot[slot].Clear();
    }

    GameObject* pGameObj = new GameObject;

    Map* map = m_caster->GetMap();
    if (!pGameObj->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), go_id, map,
        m_caster->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY))
    {
        delete pGameObj;
        return;
    }

    pGameObj->AddPlayerInPersonnalVisibilityList(player->GetGUID());

    pGameObj->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);
    pGameObj->SetSpellId(m_spellInfo->Id);
    m_caster->AddGameObject(pGameObj);

    map->AddToMap(pGameObj);

    m_caster->m_ObjectSlot[slot] = pGameObj->GetGUID();
}

void Spell::EffectResurrect(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;
    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if (unitTarget->isAlive())
        return;
    if (!unitTarget->IsInWorld())
        return;

    switch (m_spellInfo->Id)
    {
        // Defibrillate (Goblin Jumper Cables) have 33% chance on success
        case 8342:
            if (roll_chance_i(67))
            {
                m_caster->CastSpell(m_caster, 8338, true, m_CastItem);
                return;
            }
            break;
        // Defibrillate (Goblin Jumper Cables XL) have 50% chance on success
        case 22999:
            if (roll_chance_i(50))
            {
                m_caster->CastSpell(m_caster, 23055, true, m_CastItem);
                return;
            }
            break;
        // Defibrillate (Gnomish Army Knife) have 67% chance on success_list
        case 54732:
            if (roll_chance_i(33))
            {
                return;
            }
            break;
        default:
            break;
    }

    Player* target = unitTarget->ToPlayer();

    if (target->IsRessurectRequested())       // already have one active request
        return;

    if (m_spellInfo->HasAttribute(SPELL_ATTR8_BATTLE_RESURRECTION))
        if (InstanceScript* instance = target->GetInstanceScript())
            instance->SetResurectSpell();

    int32 hpPerc = m_spellInfo->Effects[EFFECT_1].CalcValue(m_caster);
    if (!hpPerc)
        hpPerc = damage;

    uint32 health = target->CountPctFromMaxHealth(hpPerc);
    uint32 mana = CalculatePct(target->GetMaxPower(POWER_MANA), damage);

    ExecuteLogEffectResurrect(effIndex, target);

    target->SetResurrectRequestData(m_caster, health, mana, 0);
    SendResurrectRequest(target);

    m_caster->CastSpell(unitTarget, 160029, true);
}

void Spell::EffectAddExtraAttacks(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->isAlive() || !unitTarget->getVictim())
        return;

    if (unitTarget->m_extraAttacks)
        return;

    unitTarget->m_extraAttacks = damage;

    ExecuteLogEffectExtraAttacks(effIndex, unitTarget, damage);
}

void Spell::EffectParry(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->SetCanParry(true);
}

void Spell::EffectBlock(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->SetCanBlock(true);
}

void Spell::EffectLeap(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->isInFlight())
        return;

    if (!m_targets.HasDst())
        return;

    Position pos = *m_targets.GetDstPos();
    unitTarget->AddUnitState(UNIT_STATE_JUMPING);

    //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "EffectLeap If %i, X %f, Y %f, Z %f", m_spellInfo->Id, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());

    unitTarget->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), unitTarget == m_caster);
}

void Spell::EffectReputation(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();

    int32 rep_change = damage;
    uint32 faction_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id);
    if (!factionEntry)
        return;

    if (RepRewardRate const* repData = sObjectMgr->GetRepRewardRate(faction_id))
        rep_change = int32((float)rep_change * repData->spell_rate);

    // Bonus from spells that increase reputation gain
    float bonus = rep_change * player->GetTotalAuraModifier(SPELL_AURA_MOD_REPUTATION_GAIN) / 100.0f; // 10%
    rep_change += (int32)bonus;

    player->GetReputationMgr().ModifyReputation(factionEntry, rep_change);
}

void Spell::EffectQuestComplete(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    Player* player = unitTarget->ToPlayer();

    uint32 questId = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    if (questId)
    {
        Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
        if (!quest)
            return;

        uint16 logSlot = player->FindQuestSlot(questId);
        if (logSlot < MAX_QUEST_LOG_SIZE)
            player->AreaExploredOrEventHappens(questId);
        else if (player->CanTakeQuest(quest, false))    // never rewarded before
            player->CompleteQuest(questId);             // quest not in log - for internal use
    }
}

void Spell::EffectForceDeselect(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    WorldPackets::Spells::ClearTarget clearTarget;
    clearTarget.Guid = m_caster->GetGUID();
    m_caster->SendMessageToSet(clearTarget.Write(), true);
}

void Spell::EffectSelfResurrect(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (!m_caster || m_caster->isAlive())
        return;
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!m_caster->IsInWorld())
        return;

    uint32 health = 0;
    uint32 mana = 0;

    int32 hpPerc = m_spellInfo->Effects[EFFECT_1].CalcValue(m_caster);
    if (!hpPerc)
        hpPerc = damage;

    // flat case
    if (damage < 0)
    {
        health = uint32(-damage);
        mana = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    }
    // percent case
    else
    {
        health = m_caster->CountPctFromMaxHealth(hpPerc);
        if (m_caster->GetMaxPower(POWER_MANA) > 0)
            mana = CalculatePct(m_caster->GetMaxPower(POWER_MANA), damage);
    }

    Player* player = m_caster->ToPlayer();
    player->ResurrectPlayer(0.0f);

    player->SetHealth(health);
    player->SetPower(POWER_MANA, mana);
    player->SetPower(POWER_RAGE, 0);
    player->SetPower(POWER_ENERGY, player->GetMaxPower(POWER_ENERGY));
    player->SetPower(POWER_FOCUS, 0);

    player->SpawnCorpseBones();
    player->RemoveAurasDueToSpell(160029);
}

void Spell::EffectSkinning(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (unitTarget->GetTypeId() != TYPEID_UNIT)
        return;
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Creature* creature = unitTarget->ToCreature();
    int32 targetLevel = creature->getLevel();

    uint32 skill = creature->GetCreatureTemplate()->GetRequiredLootSkill();

    m_caster->ToPlayer()->SendLoot(creature->GetGUID(), LOOT_SKINNING);
    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

    int32 reqValue = targetLevel < 10 ? 0 : targetLevel < 20 ? (targetLevel - 10) * 10 : targetLevel * 5;
    if (targetLevel > 80)
        reqValue = targetLevel * 6;

    int32 skillValue = m_caster->ToPlayer()->GetPureSkillValue(skill);

    // Double chances for elites
    m_caster->ToPlayer()->UpdateGatherSkill(skill, skillValue, reqValue, creature->isElite() ? 2 : 1);
}

void Spell::EffectCharge(SpellEffIndex effIndex)
{
    if (effectHandleMode == SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
    {
        if (!unitTarget)
            return;

        uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;
        float angle = unitTarget->GetRelativeAngle(m_caster);
        Position pos;

        unitTarget->GetContactPoint(m_caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
        unitTarget->GetFirstCollisionPosition(pos, unitTarget->GetObjectSize(), angle);

        if (!m_caster->GetMotionMaster()->SpellMoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ + unitTarget->GetObjectSize(), SPEED_CHARGE, EVENT_CHARGE, triggered_spell_id))
            return;

        switch (m_spellInfo->Id)
        {
            case 100: // Charge
            {
                uint32 stunspell = m_caster->HasAura(103828) ? 7922 : 105771;
                m_caster->CastSpell(unitTarget, stunspell, true);
                break;
            }
            default:
                break;
        }
    }

    if (effectHandleMode == SPELL_EFFECT_HANDLE_HIT_TARGET)
    {
        if (!unitTarget)
            return;

        // not all charge effects used in negative spells
        if (!m_spellInfo->IsPositive() && m_caster->GetTypeId() == TYPEID_PLAYER)
            m_caster->Attack(unitTarget, true);
    }
}

void Spell::EffectChargeDest(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    if (m_targets.HasDst())
    {
        Position pos;
        destTarget->GetPosition(&pos);
        float angle = m_caster->GetRelativeAngle(pos.GetPositionX(), pos.GetPositionY());
        float dist = m_caster->GetDistance(pos);
        m_caster->GetFirstCollisionPosition(pos, dist, angle);

        // Racer Slam Hit Destination
        if (m_spellInfo->Id == 49302)
        {
            if (urand(0, 100) < 80)
            {
                m_caster->CastSpell(m_caster, 49336, false);
                m_caster->CastSpell((Unit*)NULL, 49444, false); // achievement counter
            }
        }

        uint32 triggered_spell_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell;

        if (!m_caster->GetMotionMaster()->SpellMoveCharge(pos.m_positionX, pos.m_positionY, pos.m_positionZ, SPEED_CHARGE, EVENT_CHARGE, triggered_spell_id))
            return;
    }
}

void Spell::EffectKnockBack(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (Creature* creatureTarget = unitTarget->ToCreature())
        if (creatureTarget->isWorldBoss() || creatureTarget->IsDungeonBoss())
            return;

    // Spells with SPELL_EFFECT_KNOCK_BACK(like Thunderstorm) can't knoback target if target has ROOT/STUN
    if (unitTarget->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
        return;

    // Thunderstorm
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_SHAMAN && m_spellInfo->SpellFamilyFlags[1] & 0x00002000)
    {
        // Glyph of Thunderstorm
        if (m_caster->HasAura(62132))
            return;
    }

    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
        if (unitTarget->ToPlayer()->GetKnockBackTime())
            return;


    // Hack. Instantly interrupt non melee spells being casted
    if (Spell* spell = unitTarget->GetCurrentSpell(CURRENT_GENERIC_SPELL))
    {
        if (unitTarget->IsNonMeleeSpellCasted(true) &&
            (spell->m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_MOVEMENT) &&
            !unitTarget->HasAuraType(SPELL_AURA_CAST_WHILE_WALKING))
        {
            unitTarget->InterruptNonMeleeSpells(true);
        }
    }

    float ratio = 0.1f;
    float speedxy = float(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue) * ratio;
    float speedz = float(damage) * ratio;
    /*if (fabs(speedxy < 0.1f) && speedz < 0.1f)
        return;*/

    float x, y;
    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_KNOCK_BACK_DEST)
    {
        if (m_targets.HasDst())
            destTarget->GetPosition(x, y);
        else
            return;
    }
    else //if (m_spellInfo->GetEffect(i, m_diffMode).Effect == SPELL_EFFECT_KNOCK_BACK)
    {
        m_caster->GetPosition(x, y);
    }

    // Forward Thrust
    if (m_spellInfo->Id == 126408)
        unitTarget->JumpTo(-speedxy, speedz);
    else
        unitTarget->KnockbackFrom(x, y, speedxy, speedz);

    if (unitTarget->GetTypeId() == TYPEID_PLAYER)
        unitTarget->ToPlayer()->SetKnockBackTime(getMSTime());
}

void Spell::EffectLeapBack(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH_TARGET)
        return;

    if (!unitTarget)
        return;

    float speedxy = float(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue) / 10;
    float speedz = float(damage / 10);

    // Fix Glyph of Disengage
    if (m_caster->HasAura(56844))
    {
        speedxy *= 1.5f;
        speedz = float(75 / 10);
    }

    //1891: Disengage and Wild Charge (moonkin form)
    m_caster->JumpTo(speedxy, speedz, m_spellInfo->Misc.SpellIconID != 1891 && m_spellInfo->Id != 102383);
}

void Spell::EffectQuestClear(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    Player* player = unitTarget->ToPlayer();

    uint32 quest_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);

    if (!quest)
        return;

    // Player has never done this quest
    if (player->GetQuestStatus(quest_id) == QUEST_STATUS_NONE)
        return;

    // remove all quest entries for 'entry' from quest log
    for (uint8 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
    {
        uint32 logQuest = player->GetQuestSlotQuestId(slot);
        if (logQuest == quest_id)
        {
            player->SetQuestSlot(slot, 0);

            // we ignore unequippable quest items in this case, it's still be equipped
            player->TakeQuestSourceItem(logQuest, false);
        }
    }

    player->RemoveActiveQuest(quest_id);
    player->RemoveRewardedQuest(quest_id);
}

void Spell::EffectSendTaxi(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->ActivateTaxiPathTo(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, m_spellInfo->Id);
}

void Spell::EffectPullTowards(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    float speedZ = (float)(m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValue() / 10);
    float speedXY = (float)(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue / 10);
    Position pos;
    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->Effect == SPELL_EFFECT_PULL_TOWARDS_DEST)
    {
        if (m_targets.HasDst())
            pos.Relocate(*destTarget);
        else
            return;
    }
    else //if (m_spellInfo->GetEffect(i, m_diffMode).Effect == SPELL_EFFECT_PULL_TOWARDS)
    {
        pos.Relocate(m_caster);
    }

    unitTarget->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), speedXY, speedZ);
}

void Spell::EffectDispelMechanic(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    uint32 mechanic = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    uint32 allEffectDispelMask = m_spellInfo->GetSimilarEffectsMiscValueMask(SPELL_EFFECT_DISPEL_MECHANIC, m_caster);

    std::queue < std::pair < uint32, ObjectGuid > > dispel_list;

    Unit::AuraMap const& auras = unitTarget->GetOwnedAuras();
    for (Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
    {
        Aura* aura = itr->second;
        if (!aura->GetApplicationOfTarget(unitTarget->GetGUID()))
            continue;
        if (roll_chance_i(aura->CalcDispelChance(unitTarget, !unitTarget->IsFriendlyTo(m_caster))))
        {
            if (SpellInfo const* auraSpellInfo = aura->GetSpellInfo())
                if (uint32 mechanicMask = auraSpellInfo->GetAllEffectsMechanicMask())
                {
                    if ((mechanicMask & allEffectDispelMask) && hasPredictedDispel != 2)
                        hasPredictedDispel = 1;

                    if (mechanicMask & (1 << mechanic))
                        dispel_list.push(std::make_pair(aura->GetId(), aura->GetCasterGUID()));
                }
        }
    }

    for (; dispel_list.size(); dispel_list.pop())
    {
        unitTarget->RemoveAura(dispel_list.front().first, dispel_list.front().second, 0, AURA_REMOVE_BY_ENEMY_SPELL);
    }

    if (hasPredictedDispel == 1)
    {
        Unit::AuraEffectList const& mTotalAuraList = m_caster->GetAuraEffectsByType(SPELL_AURA_DUMMY);
        for (Unit::AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
            if ((*i)->GetMiscValue() == 11 && (*i)->GetSpellInfo()->Misc.SpellIconID == m_spellInfo->Misc.SpellIconID)
                (*i)->SetAmount(m_spellInfo->Id);

        hasPredictedDispel++; // 2 is lock
    }
}

void Spell::EffectSummonDeadPet(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    Pet* pet = player->GetPet();
    if (!pet)
        return;

    if (pet->isAlive())
        return;

    if (damage < 0)
        return;

    float x, y, z;
    player->GetPosition(x, y, z);

    player->GetMap()->CreatureRelocation(pet, x, y, z, player->GetOrientation());

    pet->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_NONE);
    pet->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
    pet->setDeathState(ALIVE);
    pet->ClearUnitState(uint32(UNIT_STATE_ALL_STATE));
    pet->SetHealth(pet->CountPctFromMaxHealth(damage));

    //pet->AIM_Initialize();
    //player->PetSpellInitialize();
    pet->SavePetToDB();
}

void Spell::EffectDestroyAllTotems(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    int32 mana = 0;
    for (uint8 slot = SUMMON_SLOT_TOTEM; slot < MAX_TOTEM_SLOT; ++slot)
    {
        if (!m_caster->m_SummonSlot[slot])
            continue;

        Creature* totem = m_caster->GetMap()->GetCreature(m_caster->m_SummonSlot[slot]);
        if (totem && totem->isTotem())
        {
            uint32 spell_id = totem->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL);
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id);
            if (spellInfo)
                mana += CalculatePct(spellInfo->CalcPowerCost(m_caster, spellInfo->GetSchoolMask()), damage);
            totem->ToTotem()->UnSummon();
        }
    }
    if(!mana)
        return;

    m_caster->CastCustomSpell(m_caster, 39104, &mana, NULL, NULL, true);
}

void Spell::EffectDurabilityDamage(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 slot = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    // FIXME: some spells effects have value -1/-2
    // Possibly its mean -1 all player equipped items and -2 all items
    if (slot < 0)
    {
        unitTarget->ToPlayer()->DurabilityPointsLossAll(damage, (slot < -1));
        ExecuteLogEffectDurabilityDamage(effIndex, unitTarget, -1, -1);
        return;
    }

    // invalid slot value
    if (slot >= INVENTORY_SLOT_BAG_END)
        return;

    if (Item* item = unitTarget->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
    {
        unitTarget->ToPlayer()->DurabilityPointsLoss(item, damage);
        ExecuteLogEffectDurabilityDamage(effIndex, unitTarget, item->GetEntry(), slot);
    }
}

void Spell::EffectDurabilityDamagePCT(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 slot = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    // FIXME: some spells effects have value -1/-2
    // Possibly its mean -1 all player equipped items and -2 all items
    if (slot < 0)
    {
        unitTarget->ToPlayer()->DurabilityLossAll(float(damage) / 100.0f, (slot < -1), false);
        return;
    }

    // invalid slot value
    if (slot >= INVENTORY_SLOT_BAG_END)
        return;

    if (damage <= 0)
        return;

    if (Item* item = unitTarget->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
        unitTarget->ToPlayer()->DurabilityLoss(item, float(damage) / 100.0f);
}

void Spell::EffectModifyThreatPercent(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    unitTarget->getThreatManager().modifyThreatPercent(m_caster, damage);
}

void Spell::EffectTransmitted(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    uint32 name_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(name_id);

    if (!goinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u) not exist and not created at spell (ID: %u) cast", name_id, m_spellInfo->Id);
        return;
    }

    float fx, fy, fz;

    if (m_targets.HasDst())
        destTarget->GetPosition(fx, fy, fz);
    //FIXME: this can be better check for most objects but still hack
    else if (m_spellInfo->GetEffect(effIndex, m_diffMode)->HasRadius() && m_spellInfo->Misc.Speed == 0)
    {
        float dis = m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcRadius(m_originalCaster);
        m_caster->GetClosePoint(fx, fy, fz, DEFAULT_WORLD_OBJECT_SIZE, dis);
    }
    else
    {
        //GO is always friendly to it's creator, get range for friends
        float min_dis = m_spellInfo->GetMinRange(true);
        float max_dis = m_spellInfo->GetMaxRange(true);
        float dis = (float)rand_norm() * (max_dis - min_dis) + min_dis;

        m_caster->GetClosePoint(fx, fy, fz, DEFAULT_WORLD_OBJECT_SIZE, dis);
    }

    Map* cMap = m_caster->GetMap();
    if (goinfo->type == GAMEOBJECT_TYPE_FISHINGNODE || goinfo->type == GAMEOBJECT_TYPE_FISHINGHOLE)
    {
        LiquidData liqData;
        if (!cMap->IsInWater(fx, fy, fz + 1.f/* -0.5f */, &liqData))             // Hack to prevent fishing bobber from failing to land on fishing hole
        { // but this is not proper, we really need to ignore not materialized objects
            SendCastResult(SPELL_FAILED_NOT_HERE);
            SendChannelUpdate(0);
            return;
        }

        // replace by water level in this case
        //fz = cMap->GetWaterLevel(fx, fy);
        fz = liqData.level;
    }
    // if gameobject is summoning object, it should be spawned right on caster's position
    else if (goinfo->type == GAMEOBJECT_TYPE_RITUAL)
    {
        m_caster->GetPosition(fx, fy, fz);
    }

    GameObject* pGameObj = new GameObject;

    if (!pGameObj->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), name_id, cMap,
        m_caster->GetPhaseMask(), fx, fy, fz, m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, GO_STATE_READY))
    {
        delete pGameObj;
        return;
    }

    int32 duration = m_spellInfo->GetDuration();

    switch (goinfo->type)
    {
        case GAMEOBJECT_TYPE_FISHINGNODE:
        {
            m_caster->SetGuidValue(UNIT_FIELD_CHANNEL_OBJECT, pGameObj->GetGUID());
            m_caster->AddGameObject(pGameObj);              // will removed at spell cancel

            // end time of range when possible catch fish (FISHING_BOBBER_READY_TIME..GetDuration(m_spellInfo))
            // start time == fish-FISHING_BOBBER_READY_TIME (0..GetDuration(m_spellInfo)-FISHING_BOBBER_READY_TIME)
            int32 lastSec = 0;
            switch (urand(0, 3))
            {
                case 0: lastSec = 3; break;
                case 1: lastSec = 7; break;
                case 2: lastSec = 13; break;
                case 3: lastSec = 17; break;
            }

            duration = duration - lastSec*IN_MILLISECONDS + FISHING_BOBBER_READY_TIME*IN_MILLISECONDS;
            break;
        }
        case GAMEOBJECT_TYPE_RITUAL:
        {
            if (m_caster->GetTypeId() == TYPEID_PLAYER)
            {
                pGameObj->AddUniqueUse(m_caster->ToPlayer());
                m_caster->AddGameObject(pGameObj);      // will be removed at spell cancel
            }
            break;
        }
        case GAMEOBJECT_TYPE_DUEL_ARBITER: // 52991
            m_caster->AddGameObject(pGameObj);
            break;
        case GAMEOBJECT_TYPE_FISHINGHOLE:
        case GAMEOBJECT_TYPE_CHEST:
        default:
            break;
    }

    pGameObj->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);

    pGameObj->SetOwnerGUID(m_caster->GetGUID());

    //pGameObj->SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, m_caster->getLevel());
    pGameObj->SetSpellId(m_spellInfo->Id);

    ExecuteLogEffectSummonObject(effIndex, pGameObj);

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "AddObject at SpellEfects.cpp EffectTransmitted");
    //m_caster->AddGameObject(pGameObj);
    //m_ObjToDel.push_back(pGameObj);

    cMap->AddToMap(pGameObj);

    // Glyph of Soulwell, Create Soulwell - 29893
    if (m_spellInfo->Id == 29893 && m_caster->HasAura(58094))
        m_caster->CastSpell(fx, fy, fz, 34145, true);

    if (uint32 linkedEntry = pGameObj->GetGOInfo()->GetLinkedGameObjectEntry())
    {
        GameObject* linkedGO = new GameObject;
        if (linkedGO->Create(sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate(), linkedEntry, cMap,
            m_caster->GetPhaseMask(), fx, fy, fz, m_caster->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 100, GO_STATE_READY))
        {
            linkedGO->SetRespawnTime(duration > 0 ? duration / IN_MILLISECONDS : 0);
            //linkedGO->SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, m_caster->getLevel());
            linkedGO->SetSpellId(m_spellInfo->Id);
            linkedGO->SetOwnerGUID(m_caster->GetGUID());

            ExecuteLogEffectSummonObject(effIndex, linkedGO);

            linkedGO->GetMap()->AddToMap(linkedGO);
        }
        else
        {
            delete linkedGO;
            linkedGO = NULL;
            return;
        }
    }
}

void Spell::EffectProspecting(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if (!itemTarget || !(itemTarget->GetTemplate()->Flags & ITEM_PROTO_FLAG_PROSPECTABLE))
        return;

    if (itemTarget->GetCount() < 5)
        return;

    if (sWorld->getBoolConfig(CONFIG_SKILL_PROSPECTING))
    {
        uint32 SkillValue = p_caster->GetPureSkillValue(SKILL_JEWELCRAFTING);
        uint32 reqSkillValue = itemTarget->GetTemplate()->RequiredSkillRank;
        p_caster->UpdateGatherSkill(SKILL_JEWELCRAFTING, SkillValue, reqSkillValue);
    }

    m_caster->ToPlayer()->SendLoot(itemTarget->GetGUID(), LOOT_PROSPECTING);
}

void Spell::EffectMilling(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = (Player*)m_caster;
    if (!itemTarget || !(itemTarget->GetTemplate()->Flags & ITEM_PROTO_FLAG_MILLABLE))
        return;

    if (itemTarget->GetCount() < 5)
        return;

    if (sWorld->getBoolConfig(CONFIG_SKILL_MILLING))
    {
        uint32 SkillValue = p_caster->GetPureSkillValue(SKILL_INSCRIPTION);
        uint32 reqSkillValue = itemTarget->GetTemplate()->RequiredSkillRank;
        p_caster->UpdateGatherSkill(SKILL_INSCRIPTION, SkillValue, reqSkillValue);
    }

    m_caster->ToPlayer()->SendLoot(itemTarget->GetGUID(), LOOT_MILLING);
}

void Spell::EffectSkill(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WORLD: SkillEFFECT");
}

/* There is currently no need for this effect. We handle it in Battleground.cpp
   If we would handle the resurrection here, the spiritguide would instantly disappear as the
   player revives, and so we wouldn't see the spirit heal visual effect on the npc.
   This is why we use a half sec delay between the visual effect and the resurrection itself */
void Spell::EffectSpiritHeal(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    /*
    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    if (!unitTarget->IsInWorld())
        return;

    //m_spellInfo->GetEffect(i, m_diffMode).BasePoints; == 99 (percent?)
    //unitTarget->ToPlayer()->setResurrect(m_caster->GetGUID(), unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(), unitTarget->GetMaxHealth(), unitTarget->GetMaxPower(POWER_MANA));
    unitTarget->ToPlayer()->ResurrectPlayer(1.0f);
    unitTarget->ToPlayer()->SpawnCorpseBones();
    */
}

// remove insignia spell effect
void Spell::EffectSkinPlayerCorpse(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Effect: SkinPlayerCorpse");
    if ((m_caster->GetTypeId() != TYPEID_PLAYER) || (unitTarget->GetTypeId() != TYPEID_PLAYER) || (unitTarget->isAlive()))
        return;

    unitTarget->ToPlayer()->RemovedInsignia((Player*)m_caster);
}

void Spell::EffectStealBeneficialBuff(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Effect: StealBeneficialBuff");

    if (!unitTarget || unitTarget == m_caster)                 // can't steal from self
        return;

    DispelChargesList steal_list;

    // Create dispel mask by dispel type
    uint32 dispelMask = SpellInfo::GetDispelMask(DispelType(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue));
    Unit::AuraMap const& auras = unitTarget->GetOwnedAuras();
    for (Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
    {
        Aura* aura = itr->second;
        AuraApplication * aurApp = aura->GetApplicationOfTarget(unitTarget->GetGUID());
        if (!aurApp)
            continue;

        if ((aura->GetSpellInfo()->GetDispelMask()) & dispelMask)
        {
            // Need check for passive? this
            if (!aurApp->IsPositive() || aura->IsPassive() || aura->GetSpellInfo()->HasAttribute(SPELL_ATTR4_NOT_STEALABLE))
                continue;

            // The charges / stack amounts don't count towards the total number of auras that can be dispelled.
            // Ie: A dispel on a target with 5 stacks of Winters Chill and a Polymorph has 1 / (1 + 1) -> 50% chance to dispell
            // Polymorph instead of 1 / (5 + 1) -> 16%.
            bool dispel_charges = (aura->GetSpellInfo()->HasAttribute(SPELL_ATTR7_DISPEL_CHARGES)) != 0;
            uint8 charges = dispel_charges ? aura->GetCharges() : aura->GetStackAmount();
            if (charges > 0)
                steal_list.push_back(std::make_pair(aura, charges));
        }
    }

    if (steal_list.empty())
        return;

    // Ok if exist some buffs for dispel try dispel it
    DispelList success_list;

    std::list<uint32> spellFails;
    // dispel N = damage buffs (or while exist buffs for dispel)
    for (int32 count = 0; count < damage && !steal_list.empty();)
    {
        // Random select buff for dispel
        DispelChargesList::iterator itr = steal_list.begin();
        std::advance(itr, urand(0, steal_list.size() - 1));

        int32 chance = itr->first->CalcDispelChance(unitTarget, !unitTarget->IsFriendlyTo(m_caster));
        // 2.4.3 Patch Notes: "Dispel effects will no longer attempt to remove effects that have 100% dispel resistance."
        if (!chance)
        {
            steal_list.erase(itr);
            continue;
        }
        else
        {
            if (roll_chance_i(chance))
            {
                success_list.push_back(std::make_pair(itr->first->GetId(), itr->first->GetCasterGUID()));
                --itr->second;
                if (itr->second <= 0)
                    steal_list.erase(itr);
            }
            else
                spellFails.push_back(itr->first->GetId());              // Spell Id
            ++count;
        }
    }

    if (!spellFails.empty())
        m_caster->SendDispelFailed(unitTarget->GetGUID(), m_spellInfo->Id, spellFails);

    if (success_list.empty())
        return;

    // Spellsteal and Glyph of Spellsteal
    if (m_spellInfo->Id == 30449 && m_caster->HasAura(115713))
        m_caster->CastSpell(m_caster, 115714, true);

    // On success dispel
    // Devour Magic
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_PET_ABILITY && m_spellInfo->Categories.Category == SPELLCATEGORY_DEVOUR_MAGIC)
    {
        m_caster->CastSpell(m_caster, 19658, true);
        // Glyph of Felhunter
        if (Unit* owner = m_caster->GetAnyOwner())
            if (owner->GetAura(56249))
                m_caster->CastSpell(owner, 19658, true);
    }

    std::list<uint32> spellSuccess;
    for (DispelList::iterator itr = success_list.begin(); itr != success_list.end(); ++itr)
    {
        spellSuccess.push_back(itr->first);          // Spell Id
        unitTarget->RemoveAurasDueToSpellBySteal(itr->first, itr->second, m_caster);
    }

    m_caster->SendDispelLog(unitTarget->GetGUID(), m_spellInfo->Id, spellSuccess, false, true);
}

void Spell::EffectKillCreditPersonal(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->KilledMonsterCredit(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, ObjectGuid::Empty);
}

void Spell::EffectKillCredit(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    int32 creatureEntry = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    if (!creatureEntry)
    {
        if (m_spellInfo->Id == 42793) // Burn Body
            creatureEntry = 24008; // Fallen Combatant
    }

    if (creatureEntry)
        unitTarget->ToPlayer()->RewardPlayerAndGroupAtEvent(creatureEntry, unitTarget);
}

void Spell::EffectQuestFail(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->FailQuest(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
}

void Spell::EffectQuestStart(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();
    if (Quest const* qInfo = sObjectMgr->GetQuestTemplate(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue))
    {
        if (player->CanTakeQuest(qInfo, false) && player->CanAddQuest(qInfo, false))
        {
            player->AddQuest(qInfo, NULL);
        }
    }
}

void Spell::EffectActivateRune(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_LAUNCH)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = m_caster->ToPlayer();

    if (player->getClass() != CLASS_DEATH_KNIGHT)
        return;

    // needed later
    m_runesState = m_caster->ToPlayer()->GetRunesState();

    uint32 count = damage;
    if (count == 0) count = 1;
    for (uint32 j = 0; j < MAX_RUNES && count > 0; ++j)
    {
        if (player->GetRuneCooldown(j) && player->GetCurrentRune(j) == RuneType(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue))
        {
            player->SetRuneCooldown(j, 0);
            --count;
        }
    }

    // Empower rune weapon
    if (m_spellInfo->Id == 47568)
    {
        // Need to do this just once
        if (effIndex != 0)
            return;

        for (uint32 i = 0; i < MAX_RUNES; ++i)
        {
            if (player->GetRuneCooldown(i) && (player->GetCurrentRune(i) == RUNE_FROST || player->GetCurrentRune(i) == RUNE_DEATH))
                player->SetRuneCooldown(i, 0);
        }
    }
}

void Spell::EffectCreateTamedPet(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER || unitTarget->GetPetGUID() || unitTarget->getClass() != CLASS_HUNTER)
        return;

    PetSlot slot = m_caster->ToPlayer()->getSlotForNewPet();
    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->m_currentSummonedSlot = slot;

    uint32 creatureEntry = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    Pet* pet = unitTarget->CreateTamedPetFrom(creatureEntry, m_spellInfo->Id);
    if (!pet)
        return;

    // add to world
    pet->GetMap()->AddToMap(pet->ToCreature());
    pet->SetSlot(slot);

    unitTarget->SetMinion(pet, true);

    pet->SavePetToDB();
    unitTarget->ToPlayer()->PetSpellInitialize();

    unitTarget->ToPlayer()->GetSession()->SendStablePet(ObjectGuid::Empty);
}

void Spell::EffectDiscoverTaxi(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;
    uint32 nodeid = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    if (sTaxiNodesStore.LookupEntry(nodeid))
        unitTarget->ToPlayer()->GetSession()->SendDiscoverNewTaxiNode(nodeid);
}

void Spell::EffectTitanGrip(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->SetCanTitanGrip(true);
}

void Spell::EffectRedirectThreat(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (unitTarget)
        m_caster->SetReducedThreatPercent((uint32)damage, unitTarget->GetGUID());
}

void Spell::EffectGameObjectDamage(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!gameObjTarget)
        return;

    Unit* caster = m_originalCaster;
    if (!caster)
        return;

    FactionTemplateEntry const* casterFaction = caster->getFactionTemplateEntry();
    FactionTemplateEntry const* targetFaction = sFactionTemplateStore.LookupEntry(gameObjTarget->GetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE));
    // Do not allow to damage GO's of friendly factions (ie: Wintergrasp Walls/Ulduar Storm Beacons)
    if ((casterFaction && targetFaction && !casterFaction->IsFriendlyTo(*targetFaction)) || !targetFaction)
        gameObjTarget->ModifyHealth(-damage, caster, GetSpellInfo()->Id);
}

void Spell::EffectGameObjectRepair(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!gameObjTarget)
        return;

    gameObjTarget->ModifyHealth(damage, m_caster);
}

void Spell::EffectGameObjectSetDestructionState(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!gameObjTarget || !m_originalCaster)
        return;

    Player* player = m_originalCaster->GetCharmerOrOwnerPlayerOrPlayerItself();
    gameObjTarget->SetDestructibleState(GameObjectDestructibleState(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue), player, true);
}

void Spell::SummonGuardian(uint32 i, uint32 entry, SummonPropertiesEntry const* properties, uint32 numGuardians)
{
    Unit* caster = m_originalCaster;
    if (!caster)
        return;

    if (caster->isTotem())
        caster = caster->ToTotem()->GetOwner();

    // in another case summon new
    uint8 level = caster->getLevel();

    // level of pet summoned using engineering item based at engineering skill level
    if (m_CastItem && caster->GetTypeId() == TYPEID_PLAYER)
        if (ItemTemplate const* proto = m_CastItem->GetTemplate())
            if (proto->RequiredSkill == SKILL_ENGINEERING)
                if (uint16 skill202 = caster->ToPlayer()->GetSkillValue(SKILL_ENGINEERING))
                    level = skill202 / 5;

    float radius = 5.0f;
    int32 duration = m_spellInfo->GetDuration();
    if (!numGuardians)
        numGuardians = 1;

    if (Player* modOwner = m_originalCaster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_DURATION, duration);

    //TempSummonType summonType = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN;
    Map* map = caster->GetMap();

    for (uint32 count = 0; count < numGuardians; ++count)
    {
        Position pos;
        if (count == 0)
            pos = *destTarget;
        else
            // randomize position for multiple summons
            m_caster->GetRandomPoint(*destTarget, radius, pos);

        TempSummon* summon = map->SummonCreature(entry, pos, properties, duration, caster, m_targets.GetUnitTargetGUID(), m_spellInfo->Id);
        if (!summon)
            return;
        if (summon->HasUnitTypeMask(UNIT_MASK_GUARDIAN))
        {
            for (uint8 i = 0; i < summon->GetPetCastSpellSize(); ++i)
            {
                uint32 spellId = summon->GetPetCastSpellOnPos(i);
                if (SpellInfo const* sInfo = sSpellMgr->GetSpellInfo(spellId))
                {
                    if (sInfo->GetMaxRange(false) >= 30.0f && sInfo->GetMaxRange(false) > summon->GetAttackDist() && (sInfo->AttributesCu & SPELL_ATTR0_CU_DIRECT_DAMAGE) && !sInfo->IsTargetingAreaCast())
                    {
                        PetStats const* pStats = sObjectMgr->GetPetStats(entry);
                        if (!pStats)
                            summon->SetCasterPet(true);
                        if (!sInfo->IsPositive())
                            summon->SetAttackDist(sInfo->GetMaxRange(false));
                    }
                }
            }
        }

        if (properties && properties->Category == SUMMON_CATEGORY_ALLY)
            summon->setFaction(caster->getFaction());

        if (summon->GetEntry() == 27893)
        {
            if (uint32 weapon = m_caster->GetUInt32Value(PLAYER_FIELD_VISIBLE_ITEMS + EQUIPMENT_SLOT_MAINHAND * 2))
            {
                summon->SetDisplayId(11686);
                summon->SetVirtualItem(0, weapon);
            }
            else
                summon->SetDisplayId(1126);
        }

        // guardians should follow owner.
        if (caster && !summon->HasUnitState(UNIT_STATE_FOLLOW)) // no charm info and no victim
            if (summon->GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE) // Prevent rewrite movement from scripts
                summon->GetMotionMaster()->MoveFollow(caster, PET_FOLLOW_DIST, summon->GetFollowAngle());

        ExecuteLogEffectSummonObject(i, summon);
    }
}

void Spell::EffectRenamePet(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT ||
        !unitTarget->ToCreature()->isPet() || ((Pet*)unitTarget)->getPetType() != HUNTER_PET)
        return;

    unitTarget->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);
}

void Spell::EffectPlayMusic(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    uint32 soundKitID = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;

    if (!sSoundEntriesStore.LookupEntry(soundKitID))
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "EffectPlayMusic: SoundKitID: %u not exist in spell %u.", soundKitID, m_spellInfo->Id);
        return;
    }

    unitTarget->ToPlayer()->SendMusic(soundKitID);
}

void Spell::EffectSpecCount(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->UpdateSpecCount(damage);
}

void Spell::EffectActivateSpec(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->ActivateSpec(damage - 1);  // damage is 1 or 2, spec is 0 or 1
}

void Spell::EffectPlaySound(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    switch (m_spellInfo->Id)
    {
        case 91604: // Restricted Flight Area
        case 58600: // Restricted Flight Area
            unitTarget->ToPlayer()->GetSession()->SendNotification(LANG_ZONE_NOFLYZONE);
            unitTarget->PlayDirectSound(9417); // Fel Reaver sound
            break;
    }

    uint32 soundId = m_spellInfo->Effects[effIndex].MiscValue;

    if (!sSoundEntriesStore.LookupEntry(soundId))
    {
        sLog->outError(LOG_FILTER_SPELLS_AURAS, "EffectPlayerNotification: Sound (Id: %u) not exist in spell %u.", soundId, m_spellInfo->Id);
        return;
    }

    unitTarget->ToPlayer()->SendSound(soundId, ObjectGuid::Empty);
}

void Spell::EffectRemoveAura(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;
    // there may be need of specifying casterguid of removed auras
    unitTarget->RemoveAurasDueToSpell(m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell);
}

void Spell::EffectDamageFromMaxHealthPCT(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    if (m_spellInfo->Id == 125372)
        damage = damage / 100;

    m_damage += unitTarget->CountPctFromMaxHealth(damage);
}

void Spell::EffectGiveCurrency(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->ModifyCurrency(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, damage);
}

void Spell::EffectCastButtons(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* p_caster = m_caster->ToPlayer();
    uint32 button_id = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue + 132;
    uint32 n_buttons = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValueB;

    for (; n_buttons; --n_buttons, ++button_id)
    {
        ActionButton const* ab = p_caster->GetActionButton(button_id);
        if (!ab || ab->GetType() != ACTION_BUTTON_SPELL)
            continue;

        //! Action button data is unverified when it's set so it can be "hacked"
        //! to contain invalid spells, so filter here.
        uint32 spell_id = ab->GetAction();
        if (!spell_id)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id);
        if (!spellInfo)
            continue;

        if (!p_caster->HasSpell(spell_id) || p_caster->HasSpellCooldown(spell_id))
            continue;

        if (!(spellInfo->HasAttribute(SPELL_ATTR7_SUMMON_TOTEM)))
            continue;

        int32 cost = spellInfo->CalcPowerCost(m_caster, spellInfo->GetSchoolMask());
        if (m_caster->GetPower(POWER_MANA) < cost)
            continue;

        TriggerCastFlags triggerFlags = TriggerCastFlags(TRIGGERED_IGNORE_GCD | TRIGGERED_IGNORE_CAST_IN_PROGRESS | TRIGGERED_CAST_DIRECTLY);
        m_caster->CastSpell(m_caster, spell_id, triggerFlags);
    }
}

void Spell::EffectRechargeManaGem(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    uint32 item_id = m_spellInfo->GetEffect(EFFECT_0, m_diffMode)->ItemType;

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item_id);
    if (!pProto)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    if (Item* pItem = player->GetItemByEntry(item_id))
    {
        for (auto const& v : pProto->Effects)
            pItem->SetSpellCharges(v->ID, v->Charges);

        pItem->SetState(ITEM_CHANGED, player);
    }
}

void Spell::EffectBind(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();

    uint32 area_id;
    WorldLocation loc;
    if (m_spellInfo->GetEffect(effIndex, m_diffMode)->TargetA.GetTarget() == TARGET_DEST_DB || m_spellInfo->GetEffect(effIndex, m_diffMode)->TargetB.GetTarget() == TARGET_DEST_DB)
    {
        SpellTargetPosition const* st = sSpellMgr->GetSpellTargetPosition(m_spellInfo->Id);
        if (!st)
        {
            sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell::EffectBind - unknown teleport coordinates for spell ID %u", m_spellInfo->Id);
            return;
        }

        loc.m_mapId = st->target_mapId;
        loc.m_positionX = st->target_X;
        loc.m_positionY = st->target_Y;
        loc.m_positionZ = st->target_Z;
        loc.SetOrientation(st->target_Orientation);
        area_id = player->GetAreaId();
    }
    else
    {
        player->GetPosition(&loc);
        loc.m_mapId = player->GetMapId();
        area_id = player->GetAreaId();
    }

    player->SetHomebind(loc, area_id);
    player->SendBindPointUpdate();

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "New homebind X      : %f", loc.m_positionX);
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "New homebind Y      : %f", loc.m_positionY);
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "New homebind Z      : %f", loc.m_positionZ);
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "New homebind MapId  : %u", loc.m_mapId);
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "New homebind AreaId : %u", area_id);

    player->SendDirectMessage(WorldPackets::Misc::PlayerBound(player->GetGUID(), area_id).Write());
}

void Spell::EffectSummonRaFFriend(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER || !unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    m_caster->CastSpell(unitTarget, m_spellInfo->GetEffect(effIndex, m_diffMode)->TriggerSpell, true);
}

void Spell::EffectUnlearnTalent(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    TalentEntry const* talent = sTalentStore.LookupEntry(m_misc.Raw.Data[0]);
    if (!talent)
        return;

    Player* player = m_caster ? m_caster->ToPlayer() : nullptr;
    if (!player)
        return;

    player->RemoveTalent(talent);
    player->SendTalentsInfoData(false);
}

void Spell::EffectDespawnAreatrigger(SpellEffIndex effIndex)
{
    if (!m_caster)
        return;

    m_caster->RemoveAllAreaObjects();
}

void Spell::EffectDespawnDynamicObject(SpellEffIndex effIndex)
{
    if (!m_caster)
        return;

    m_caster->RemoveAllDynObjects();
}

void Spell::EffectCreateAreaTrigger(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    Position pos;
    if (!m_targets.HasDst())
        GetCaster()->GetPosition(&pos);
    else
        destTarget->GetPosition(&pos);

    Position posMove;
    destAtTarget.GetPosition(&posMove);

    // trigger entry/miscvalue relation is currently unknown, for now use MiscValue as trigger entry
    uint32 triggerEntry = GetSpellInfo()->Effects[effIndex].MiscValue;
    if (!triggerEntry)
        return;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectCreateAreaTrigger pos (%f %f %f) posMove(%f %f %f) HasDst %i", pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), posMove.GetPositionX(), posMove.GetPositionY(), posMove.GetPositionZ(), m_targets.HasDst());

    AreaTrigger * areaTrigger = new AreaTrigger;
    if (!areaTrigger->CreateAreaTrigger(sObjectMgr->GetGenerator<HighGuid::AreaTrigger>()->Generate(), triggerEntry, GetCaster(), GetSpellInfo(), pos, posMove, this))
        delete areaTrigger;
}

void Spell::EffectBuyGuilkBankTab(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    uint32 guildId = m_caster->ToPlayer()->GetGuildId();
    Guild* guild = sGuildMgr->GetGuildById(guildId);

    if (!guild)
        return;

    if (guild->GetLeaderGUID() != m_caster->GetGUID())
        return;

    guild->HandleSpellEffectBuyBankTab(m_caster->ToPlayer()->GetSession(), damage - 1);
}

void Spell::EffectResurrectWithAura(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || !unitTarget->IsInWorld())
        return;

    Player* target = unitTarget->ToPlayer();
    if (!target)
        return;
    if (unitTarget->isAlive())
        return;

    if (target->IsRessurectRequested())       // already have one active request
        return;

    uint32 health = target->CountPctFromMaxHealth(damage);
    uint32 mana = CalculatePct(target->GetMaxPower(POWER_MANA), damage);
    uint32 resurrectAura = 0;
    if (sSpellMgr->GetSpellInfo(GetSpellInfo()->GetEffect(effIndex, m_diffMode)->TriggerSpell))
        resurrectAura = GetSpellInfo()->GetEffect(effIndex, m_diffMode)->TriggerSpell;

    if (resurrectAura && target->HasAura(resurrectAura))
        return;

    ExecuteLogEffectResurrect(effIndex, target);
    target->SetResurrectRequestData(m_caster, health, mana, resurrectAura);
    SendResurrectRequest(target);
}

void Spell::EffectSummonRaidMarker(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player || !m_targets.HasDst())
        return;

    Group* group = player->GetGroup();
    if (!group || (group->isRaidGroup() && !group->IsLeader(player->GetGUID()) && !group->IsAssistant(player->GetGUID())))
        return;

    float x, y, z;
    destTarget->GetPosition(x, y, z);

    group->AddRaidMarker(damage, player->GetMapId(), x, y, z);
}

void Spell::EffectRandomizeDigsites(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!sWorld->getBoolConfig(CONFIG_ARCHAEOLOGY_ENABLED))
        return;

    Player* player = m_caster->ToPlayer();
    if (!player || !player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    player->RandomizeSitesInMap(m_spellInfo->GetEffect(effIndex)->MiscValue, damage);
}

void Spell::EffectTeleportToDigsite(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!sWorld->getBoolConfig(CONFIG_ARCHAEOLOGY_ENABLED))
        return;

    Player* player = m_caster->ToPlayer();
    if (!player || !player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    player->TeleportToDigsiteInMap(player->GetMapId());
}

void Spell::EffectUncageBattlePet(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!m_CastItem || m_CastItem->GetEntry() != ITEM_BATTLE_PET_CAGE_ID)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    uint32 speciesID = m_CastItem->GetModifier(ITEM_MODIFIER_BATTLE_PET_SPECIES_ID);
    uint32 tempData = m_CastItem->GetModifier(ITEM_MODIFIER_BATTLE_PET_BREED_DATA);

    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesID);
    if (!speciesEntry)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr)
        return;

    if (battlePetMgr->GetPetCount(speciesID) >= MAX_ACTIVE_BATTLE_PETS)
    {
        battlePetMgr->SendError(BATTLEPETRESULT_CANT_HAVE_MORE_PETS_OF_THAT_TYPE, speciesEntry->CreatureEntry);
        SendCastResult(SPELL_FAILED_CANT_ADD_BATTLE_PET);
        return;
    }

    // learn pet spell, hack, TODO: fix it
    if (!player->HasSpell(speciesEntry->spellId))
        player->learnSpell(speciesEntry->spellId, false);

    battlePetMgr->AddPet(speciesID, tempData & 0xFF, tempData >> 24, m_CastItem->GetModifier(ITEM_MODIFIER_BATTLE_PET_LEVEL));
    player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), true);

    // prevent crash at access to deleted m_targets.GetItemTarget
    if (m_CastItem == m_targets.GetItemTarget())
        m_targets.SetItemTarget(nullptr);

    m_CastItem = nullptr;
}

void Spell::EffectUnlockPetBattles(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    if (!player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_PET_BATTLES_UNLOCKED))
    {
        player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_PET_BATTLES_UNLOCKED);
        player->GetBattlePetMgr()->UnlockSlot(0);
    }
}

void Spell::EffectHealBattlePetPct(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (battlePetMgr)
        battlePetMgr->HealBattlePetsPct(m_spellInfo->GetEffect(effIndex, m_diffMode)->CalcValue(m_caster));
}

//! Based on SPELL_EFFECT_ACTIVATE_SCENE3 spell 117790
void Spell::SendScene(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET ||
        m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Position pos;
    if (m_targets.HasDst())
        pos = *m_targets.GetDstPos();
    else
        m_caster->GetPosition(&pos);

    m_caster->ToPlayer()->SendSpellScene(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue, m_spellInfo, true, &pos);
}

void Spell::EffectBonusLoot(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();

    uint32 lootId = m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue;
    uint8 lootType = TYPE_CREATURE;
    if (!lootId)
        return;

    uint32 spellCooldownId = m_spellInfo->Id;
    PersonalLootData const* plData = sObjectMgr->GetPersonalLootDataBySpell(m_spellInfo->Id);
    if (plData)
    {
        if (m_spellInfo->Id != plData->lootspellId)
            spellCooldownId = plData->bonusspellId;
        if (plData->cooldownid)
        {
            lootId = plData->cooldownid;
            lootType = plData->cooldowntype;
        }
        else
            lootId = plData->entry;
    }

    //sLog->outDebug(LOG_FILTER_LOOT, "Spell::EffectBonusLoot spellCooldownId %i", spellCooldownId);

    uint32 difficulty = 0;
    if (unitTarget == m_caster)
    {
        if (Aura* aura = unitTarget->GetAura(spellCooldownId))
        {
            if (AuraEffect* eff = aura->GetEffect(EFFECT_0))
                difficulty = eff->GetAmount();
            else
            {
                aura->Remove();
                return;
            }
            aura->Remove();
        }
        else
            return;
    }

    if (player->IsPlayerLootCooldown(spellCooldownId, TYPE_SPELL, difficulty))
        return;

    //Loot for boos
    if (plData)
    {
        if (m_spellInfo->Id == plData->lootspellId)
        {
            player->AddPlayerLootCooldown(plData->lootspellId, TYPE_SPELL, true, difficulty);
            return;
        }
        //sLog->outDebug(LOG_FILTER_LOOT, "Spell::EffectBonusLoot lootId %i lootType %i", lootId, lootType);

        Loot* loot = &player->personalLoot;
        loot->clear();
        loot->personal = true;
        loot->isBoss = true;
        loot->spawnMode = difficulty;
        loot->bonusLoot = unitTarget == m_caster;
        if (plData)
            loot->chance = plData->chance;
        switch (lootType)
        {
            case TYPE_GO:
                loot->FillLoot(lootId, LootTemplates_Gameobject, player, true);
                break;
            case TYPE_CREATURE:
                loot->FillLoot(lootId, LootTemplates_Creature, player, true);
                break;
        }
        loot->AutoStoreItems();
        if (loot->gold)
        {
            //sLog->outDebug(LOG_FILTER_LOOT, "Spell::EffectBonusLoot gold %i bonusLoot %i", loot->gold, loot->bonusLoot);
            player->ModifyMoney(loot->gold);
            player->SendDisplayToast(0, 7, 0/*loot->bonusLoot*/, loot->gold, 3);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, loot->gold);
            loot->gold = 0;
        }
        if (IsTriggered())
            TakeReagents();
        player->AddPlayerLootCooldown(spellCooldownId, TYPE_SPELL, true, difficulty);
    }
    else //Other loot for item and any
    {
        Loot* loot = &player->personalLoot;
        loot->clear();
        loot->personal = true;
        loot->FillLoot(lootId, LootTemplates_Bonus, player, true);
        loot->AutoStoreItems();
        uint32 count = 1;

        //sLog->outDebug(LOG_FILTER_LOOT, "Spell::EffectBonusLoot Other loot for item and any lootId %i", lootId);
        if (m_CastItem)
            player->DestroyItemCount(m_CastItem, count, true);
    }
}

void Spell::EffectUpdatePlayerPhase(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* player = unitTarget->ToPlayer();
    // force recalculate & update.
    player->GetPhaseMgr().RemoveUpdateFlag(PHASE_UPDATE_FLAG_ZONE_UPDATE);
}

void Spell::EffectJoinOrLeavePlayerParty(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    //if (!unitTarget || !m_caster)
    //    return;


    //Group* group = m_caster->GetTargetUnit()->ToPlayer()->GetGroup();
    //Creature* creature = m_caster->ToCreature();
    //if (!group)
    //    return;

    //if (m_spellInfo->GetEffect(effIndex, m_diffMode).MiscValue == 1)
    //    group->AddCreatureMember(creature);
    //else
    //    group->RemoveCreatureMember(creature->GetGUID());
}

void Spell::EffectSummonConversation(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    Position pos;
    if (!m_targets.HasDst())
        GetCaster()->GetPosition(&pos);
    else
        destTarget->GetPosition(&pos);

    // trigger entry/miscvalue relation is currently unknown, for now use MiscValue as trigger entry
    uint32 triggerEntry = GetSpellInfo()->Effects[effIndex].MiscValue;
    if (!triggerEntry)
        return;

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell::EffectSummonConversation pos (%f %f %f) HasDst %i", pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), m_targets.HasDst());

    Conversation* conversation = new Conversation;
    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), triggerEntry, GetCaster(), GetSpellInfo(), pos))
        delete conversation;
}

void Spell::EffectLearnGarrisonBuilding(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    Player* player = unitTarget->ToPlayer();
    if (!player)
        return;

    if (Garrison* garrison = player->GetGarrison())
        if (garrison->LearnBlueprint(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue) && m_CastItem)
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), true);
}

void Spell::EffectCreateGarrison(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->CreateGarrison(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
}

void Spell::EffectAddGarrisonFollower(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    Player* player = unitTarget->ToPlayer();
    if (!player)
        return;

    if (Garrison* garrison = player->GetGarrison())
    {
        garrison->AddFollower(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
        if (m_CastItem)
            player->DestroyItem(m_CastItem->GetBagSlot(), m_CastItem->GetSlot(), false/*STOP! NO true*/);
    }
}

void Spell::EffectAddGarrisonMission(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget)
        return;

    Player* player = unitTarget->ToPlayer();
    if (!player)
        return;

    if (Garrison* garrison = player->GetGarrison())
        garrison->AddMission(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
}

void Spell::EffectActivateGarrisonBuilding(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget || unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    if (Garrison* garrison = unitTarget->ToPlayer()->GetGarrison())
        garrison->ActivateBuilding(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
}

void Spell::EffectCreateHeirloomItem(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    CollectionMgr* collectionMgr = m_caster->ToPlayer()->GetCollectionMgr();
    if (!collectionMgr)
        return;

    std::vector<int32> bonusList;
    bonusList.push_back(collectionMgr->GetHeirloomBonus(m_misc.Raw.Data[0]));

    DoCreateItem(effIndex, m_misc.Raw.Data[0], bonusList);
    ExecuteLogEffectCreateItem(effIndex, m_misc.Raw.Data[0]);
}

void Spell::EffectUpgradeHeirloom(SpellEffIndex /*effIndex*/)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT)
        return;

    CollectionMgr* collectionMgr = m_caster->ToPlayer()->GetCollectionMgr();
    if (collectionMgr)
        collectionMgr->UpgradeHeirloom(m_misc.Raw.Data[0], m_castItemEntry);
}

void Spell::EffectGieveExperience(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    uint32 xp = CalculatePct(unitTarget->ToPlayer()->GetUInt32Value(PLAYER_FIELD_NEXT_LEVEL_XP), m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValueB);
    if (xp)
        unitTarget->ToPlayer()->GiveXP(xp, unitTarget);
}

void Spell::EffectRemovePhase(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (unitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    unitTarget->ToPlayer()->RemovePhase(m_spellInfo->GetEffect(effIndex, m_diffMode)->MiscValue);
}

void Spell::EffectIncreaseFollowerItemLevel(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    Garrison* garrison = player->GetGarrison();
    if (!garrison)
        return;

    if (Garrison::Follower* follower = garrison->GetFollowerByID(m_misc.Raw.Data[0]))
        follower->IncreaseFollowerItemLevel(m_spellInfo, player);
}

void Spell::EffectReTrainFollower(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    Player* player = m_caster->ToPlayer();
    if (!player)
        return;

    Garrison* garrison = player->GetGarrison();
    if (garrison)
        garrison->ReTrainFollower(m_spellInfo, m_misc.Raw.Data[0]);
}
