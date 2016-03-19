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

/*
 * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warr_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum WarriorSpells
{
    WARRIOR_SPELL_LAST_STAND_TRIGGERED          = 12976,
    WARRIOR_SPELL_VICTORY_RUSH_DAMAGE           = 34428,
    WARRIOR_SPELL_VICTORY_RUSH_HEAL             = 118779,
    WARRIOR_SPELL_VICTORIOUS_STATE              = 32216,
    WARRIOR_SPELL_BLOODTHIRST                   = 23881,
    WARRIOR_SPELL_BLOODTHIRST_HEAL              = 117313,
    WARRIOR_SPELL_DEEP_WOUNDS                   = 115767,
    WARRIOR_SPELL_WEAKENED_BLOWS                = 115798,
    WARRIOR_SPELL_SHOCKWAVE_STUN                = 132168,
    WARRIOR_SPELL_HEROIC_LEAP_DAMAGE            = 52174,
    WARRIOR_SPELL_RALLYING_CRY		            = 97463,
    WARRIOR_SPELL_GLYPH_OF_MORTAL_STRIKE        = 58368,
    WARRIOR_SPELL_SWORD_AND_BOARD               = 50227,
    WARRIOR_SPELL_SHIELD_SLAM                   = 23922,
    WARRIOR_SPELL_ALLOW_RAGING_BLOW             = 131116,
    WARRIOR_SPELL_MOCKING_BANNER_TAUNT          = 114198,
    WARRIOR_NPC_MOCKING_BANNER                  = 59390,
    WARRIOR_SPELL_COLOSSUS_SMASH                = 86346,
    WARRIOR_SPELL_MORTAL_STRIKE_AURA            = 12294,
    WARRIOR_SPELL_DRAGON_ROAR_KNOCK_BACK        = 118895,
    WARRIOR_SPELL_MEAT_CLEAVER_PROC             = 85739,
    WARRIOR_SPELL_PHYSICAL_VULNERABILITY        = 81326,
    WARRIOR_SPELL_STORM_BOLT_STUN               = 132169,
    WARRIOR_SPELL_SHIELD_BLOCKC_TRIGGERED       = 132404,
    WARRIOR_SPELL_GLYPH_OF_GAG_ORDER            = 58357,
    WARRIOR_SPELL_SILENCED_GAG_ORDER            = 18498,
    WARRIOR_SPELL_T16_DPS_2P_BONUS              = 144436,
    WARRIOR_SPELL_REFLECTION_SHIELD_EQUIPED     = 146120,
    WARRIOR_SPELL_REFLECTION_SHIELD_HORDE       = 146122,
    WARRIOR_SPELL_REFLECTION_SHIELD_ALLIANCE    = 147923,
    WARRIOR_SPELL_SHIELDWALL_SHIELD_EQUIPED     = 146128,
    WARRIOR_SPELL_SHIELDWALL_SHIELD_HORDE       = 146127,
    WARRIOR_SPELL_SHIELDWALL_SHIELD_ALLIANCE    = 147925,
    WARRIOR_SPELL_SPELL_REFLECTION              = 23920,
    WARRIOR_SPELL_SHIELDWALL                    = 871,
};

// Shield Barrier - 112048, 174926
class spell_warr_shield_barrier : public SpellScriptLoader
{
    public:
        spell_warr_shield_barrier() : SpellScriptLoader("spell_warr_shield_barrier") { }

        class spell_warr_shield_barrier_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shield_barrier_SpellScript);

            // Power should be taken after CalculateAmount call or need create method for correct send ammount.
            void TakePower(Powers p, int32 &amount)
            {
                amount = 0;
            }

            void Register()
            {
                OnTakePower += TakePowertFn(spell_warr_shield_barrier_SpellScript::TakePower);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shield_barrier_SpellScript();
        }

        class spell_warr_shield_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_shield_barrier_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Player* plr = GetCaster()->ToPlayer())
                {
                    int32 rage = int32(plr->GetPower(POWER_RAGE) / 10);
                    int32 AP = int32(plr->GetTotalAttackPowerValue(BASE_ATTACK));
                    float versality = (plr->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_TAKEN) + plr->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE)) / 10;

                    amount += int32(AP * 1.4 * (1 + versality)) * (std::min(40, rage)) / 20;

                    plr->ModifyPower(POWER_RAGE, -(std::min(40, rage) * 10), true);

                    amount = plr->CalcAbsorb(plr, GetSpellInfo(), amount);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_shield_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_shield_barrier_AuraScript();
        }
};

//Arms Execute - 163201
class spell_warr_execute : public SpellScriptLoader
{
public:
    spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

    class spell_warr_execute_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_execute_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                int32 rage = int32(caster->GetPower(POWER_RAGE) / 10);

                if (rage >= 20)
                    SetHitDamage(GetHitDamage() * 3);
            }
        }

        void HandleEnergy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                int32 rage = int32(caster->GetPower(POWER_RAGE) / 10);

                if (rage < 20)
                    SetEffectValue(0);
                else
                    SetEffectValue(GetSpellInfo()->Effects[EFFECT_2].BasePoints * 10);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_execute_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(spell_warr_execute_SpellScript::HandleEnergy, EFFECT_2, SPELL_EFFECT_ENERGIZE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_execute_SpellScript();
    }
};

// Shield Block - 2565
class spell_warr_shield_block : public SpellScriptLoader
{
    public:
        spell_warr_shield_block() : SpellScriptLoader("spell_warr_shield_block") { }

        class spell_warr_shield_block_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shield_block_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* aura = caster->GetAura(WARRIOR_SPELL_SHIELD_BLOCKC_TRIGGERED))
                        aura->SetDuration(aura->GetDuration() + 6 * IN_MILLISECONDS);
                    else
                        caster->CastSpell(caster, WARRIOR_SPELL_SHIELD_BLOCKC_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_shield_block_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shield_block_SpellScript();
        }
};

// Storm Bolt - 107570
class spell_warr_storm_bolt : public SpellScriptLoader
{
    public:
        spell_warr_storm_bolt() : SpellScriptLoader("spell_warr_storm_bolt") { }

        class spell_warr_storm_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_storm_bolt_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (unitTarget->IsImmunedToSpellEffect(sSpellMgr->GetSpellInfo(WARRIOR_SPELL_STORM_BOLT_STUN), 0))
                            SetHitDamage(GetHitDamage() * 4);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_storm_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_storm_bolt_SpellScript();
        }
};

// Colossus Smash - 86346
class spell_warr_colossus_smash : public SpellScriptLoader
{
    public:
        spell_warr_colossus_smash() : SpellScriptLoader("spell_warr_colossus_smash") { }

        class spell_warr_colossus_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_colossus_smash_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, WARRIOR_SPELL_PHYSICAL_VULNERABILITY, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_colossus_smash_SpellScript::HandleOnHit);
            }
        };

        class spell_warr_colossus_smash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_colossus_smash_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                    if(Player* player = caster->ToPlayer())
                        if (player->GetSelectedPlayer())
                            amount /= 2;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_colossus_smash_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_BYPASS_ARMOR_FOR_CASTER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_colossus_smash_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_colossus_smash_SpellScript();
        }
};

// Called by Raging Blow - 85288
// Meat Cleaver - 85739
class spell_warr_meat_cleaver : public SpellScriptLoader
{
    public:
        spell_warr_meat_cleaver() : SpellScriptLoader("spell_warr_meat_cleaver") { }

        class spell_warr_meat_cleaver_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_meat_cleaver_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(WARRIOR_SPELL_MEAT_CLEAVER_PROC))
                        _player->RemoveAura(WARRIOR_SPELL_MEAT_CLEAVER_PROC);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_meat_cleaver_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_meat_cleaver_SpellScript();
        }
};

// Staggering Shout - 107566
class spell_warr_staggering_shout : public SpellScriptLoader
{
    public:
        spell_warr_staggering_shout() : SpellScriptLoader("spell_warr_staggering_shout") { }

        class spell_warr_staggering_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_staggering_shout_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(Trinity::UnitAuraTypeCheck(false, SPELL_AURA_MOD_DECREASE_SPEED));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_staggering_shout_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_staggering_shout_SpellScript();
        }
};

// Sudden Death - 52437
class spell_warr_sudden_death : public SpellScriptLoader
{
    public:
        spell_warr_sudden_death() : SpellScriptLoader("spell_warr_sudden_death") { }

        class spell_warr_sudden_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sudden_death_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasSpellCooldown(WARRIOR_SPELL_COLOSSUS_SMASH))
                        _player->RemoveSpellCooldown(WARRIOR_SPELL_COLOSSUS_SMASH, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sudden_death_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sudden_death_SpellScript();
        }
};

// Mocking Banner - 114192
class spell_warr_mocking_banner : public SpellScriptLoader
{
    public:
        spell_warr_mocking_banner() : SpellScriptLoader("spell_warr_mocking_banner") { }

        class spell_warr_mocking_banner_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_mocking_banner_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Player* player = GetTarget()->ToPlayer())
                {
                    std::list<Creature*> bannerList;
                    std::list<Creature*> tempList;

                    GetTarget()->GetCreatureListWithEntryInGrid(tempList, WARRIOR_NPC_MOCKING_BANNER, 30.0f);

                    bannerList = tempList;

                    // Remove other players banners
                    for (std::list<Creature*>::const_iterator itr = tempList.begin(); itr != tempList.end(); ++itr)
                    {
                        Unit* owner = (*itr)->GetOwner();
                        if (owner && owner == player && (*itr)->isSummon())
                            continue;

                        bannerList.remove(*itr);
                    }

                    for (std::list<Creature*>::const_iterator itr = bannerList.begin(); itr != bannerList.end(); ++itr)
                        player->CastSpell(*itr, WARRIOR_SPELL_MOCKING_BANNER_TAUNT, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_mocking_banner_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_mocking_banner_AuraScript();
        }
};

// Called by the proc of Enrage - 12880
// Raging Blow (allow to use it) - 131116
class spell_warr_raging_blow : public SpellScriptLoader
{
    public:
        spell_warr_raging_blow() : SpellScriptLoader("spell_warr_raging_blow") { }

        class spell_warr_raging_blow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->GetSpecializationId() == SPEC_WARRIOR_FURY && _player->getLevel() >= 30)
                        _player->CastSpell(_player, WARRIOR_SPELL_ALLOW_RAGING_BLOW, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_raging_blow_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_SpellScript();
        }
};

// Called by Devastate - 20243
// Sword and Board - 46953
class spell_warr_sword_and_board : public SpellScriptLoader
{
    public:
        spell_warr_sword_and_board() : SpellScriptLoader("spell_warr_sword_and_board") { }

        class spell_warr_sword_and_board_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sword_and_board_SpellScript);

            void HandleOnHit()
            {
                // Fix Sword and Board
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (roll_chance_i(30))
                        {
                            _player->CastSpell(_player, WARRIOR_SPELL_SWORD_AND_BOARD, true);
                            _player->RemoveSpellCooldown(WARRIOR_SPELL_SHIELD_SLAM, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sword_and_board_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sword_and_board_SpellScript();
        }
};

// Mortal strike - 12294
class spell_warr_mortal_strike : public SpellScriptLoader
{
    public:
        spell_warr_mortal_strike() : SpellScriptLoader("spell_warr_mortal_strike") { }

        class spell_warr_mortal_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_mortal_strike_SpellScript);

            void HandleOnHit()
            {
                // Fix Apply Mortal strike buff on player only if he has the correct glyph
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(WARRIOR_SPELL_MORTAL_STRIKE_AURA))
                            if (!_player->HasAura(WARRIOR_SPELL_GLYPH_OF_MORTAL_STRIKE))
                                _player->RemoveAura(WARRIOR_SPELL_MORTAL_STRIKE_AURA);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_mortal_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_mortal_strike_SpellScript();
        }
};

// Heroic leap - 6544
class spell_warr_heroic_leap : public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap() : SpellScriptLoader("spell_warr_heroic_leap") { }

        class spell_warr_heroic_leap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_SpellScript);

            SpellCastResult CheckElevation()
            {
                Unit* caster = GetCaster();

                WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());

                if (dest->GetPositionZ() > caster->GetPositionZ() + 5.0f)
                    return SPELL_FAILED_NOPATH;
                else if (caster->HasAuraType(SPELL_AURA_MOD_ROOT))
                    return SPELL_FAILED_ROOTED;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warr_heroic_leap_SpellScript::CheckElevation);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_SpellScript();
        }
};

// Heroic Leap (damage) - 52174
class spell_warr_heroic_leap_damage : public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap_damage() : SpellScriptLoader("spell_warr_heroic_leap_damage") { }

        class spell_warr_heroic_leap_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_damage_SpellScript);

            void HandleOnHit()
            {
                if(Unit* caster = GetCaster())
                    SetHitDamage(int32(caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.5f));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_heroic_leap_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_damage_SpellScript();
        }
};

// Shockwave - 46968
class spell_warr_shockwave : public SpellScriptLoader
{
    public:
        spell_warr_shockwave() : SpellScriptLoader("spell_warr_shockwave") { }

        class spell_warr_shockwave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shockwave_SpellScript);

            int32 delay;
            bool Load()
            {
                delay = 0;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.size() >= GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                    delay = -GetSpellInfo()->Effects[EFFECT_3].BasePoints * IN_MILLISECONDS;
            }


            void HandleAfterCast()
            {
                if(!delay)
                    return;
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->ModifySpellCooldown(GetSpellInfo()->Id, delay);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warr_shockwave_SpellScript::HandleAfterCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_shockwave_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shockwave_SpellScript();
        }
};

// Bloodthirst - 23881
class spell_warr_bloodthirst : public SpellScriptLoader
{
    public:
        spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

        class spell_warr_bloodthirst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_BLOODTHIRST))
                    return false;
                return true;
            }
            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (GetHitDamage())
                            _player->CastSpell(_player, WARRIOR_SPELL_BLOODTHIRST_HEAL, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_bloodthirst_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_bloodthirst_SpellScript();
        }
};

// Victory Rush - 34428
class spell_warr_victory_rush : public SpellScriptLoader
{
    public:
        spell_warr_victory_rush() : SpellScriptLoader("spell_warr_victory_rush") { }

        class spell_warr_victory_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_victory_rush_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_VICTORY_RUSH_DAMAGE))
                    return false;
                return true;
            }
            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(_player, WARRIOR_SPELL_VICTORY_RUSH_HEAL, true);
                        if (_player->HasAura(WARRIOR_SPELL_VICTORIOUS_STATE))
                            _player->RemoveAura(WARRIOR_SPELL_VICTORIOUS_STATE);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_victory_rush_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_victory_rush_SpellScript();
        }
};

class spell_warr_last_stand : public SpellScriptLoader
{
    public:
        spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

        class spell_warr_last_stand_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_last_stand_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_LAST_STAND_TRIGGERED))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(30));
                    caster->CastCustomSpell(caster, WARRIOR_SPELL_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Last Stand
                OnEffectHit += SpellEffectFn(spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_last_stand_SpellScript();
        }
};

// Thunder Clap - 6343
class spell_warr_thunder_clap : public SpellScriptLoader
{
    public:
        spell_warr_thunder_clap() : SpellScriptLoader("spell_warr_thunder_clap") { }

        class spell_warr_thunder_clap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_thunder_clap_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, WARRIOR_SPELL_WEAKENED_BLOWS, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_thunder_clap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_thunder_clap_SpellScript();
        }
};

// Called By Thunder Clap - 6343, Devastate - 20243
class spell_warr_deep_wounds : public SpellScriptLoader
{
    public:
        spell_warr_deep_wounds() : SpellScriptLoader("spell_warr_deep_wounds") { }

        class spell_warr_deep_wounds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_deep_wounds_SpellScript);

            void HandleAfterHit()
            {
                if (Player* plr = GetCaster()->ToPlayer())
                {
                    if (plr->GetSpecializationId() == SPEC_WARRIOR_PROTECTION)
                        if (Unit* target = GetHitUnit())
                            plr->CastSpell(target, WARRIOR_SPELL_DEEP_WOUNDS, true);
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_warr_deep_wounds_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_deep_wounds_SpellScript();
        }
};

// Glyph of Die by the Sword - 58386
class spell_war_glyph_of_die_by_the_sword : public SpellScriptLoader
{
public:
    spell_war_glyph_of_die_by_the_sword() : SpellScriptLoader("spell_war_glyph_of_die_by_the_sword") {}

    class spell_war_glyph_of_die_by_the_sword_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_war_glyph_of_die_by_the_sword_SpellScript);

        void HandleOnHit()
        {            
            Unit* pCaster = GetCaster();
            if (!pCaster)
                return;
                        
            if (pCaster->HasAura(58386))
            {
                if (GetSpellInfo()->Id == 7384)
                {
                    if (Aura* aura = pCaster->GetAura(118038))
                    {
                        int32 duration = aura->GetDuration();
                        aura->SetDuration(duration+1000);
                    }
                }
                if (GetSpellInfo()->Id == 100130)
                {
                    if (Aura* aura = pCaster->GetAura(118038))
                    {
                        int32 duration = aura->GetDuration();
                        aura->SetDuration(duration+500);
                    }
                }
            }
        }

    void Register()
        {
            OnHit += SpellHitFn(spell_war_glyph_of_die_by_the_sword_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_war_glyph_of_die_by_the_sword_SpellScript();
    }
};

// Glyph of Gag Order - 58357
class spell_glyph_of_gag_order : public SpellScriptLoader
{
public:
    spell_glyph_of_gag_order() : SpellScriptLoader("spell_glyph_of_gag_order") {}

    class spell_glyph_of_gag_order_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_glyph_of_gag_order_SpellScript);

        void HandleOnHit()
        {            
            Unit* player = GetCaster();
            Unit* target = GetHitUnit();
            if (!player || target->GetTypeId() == TYPEID_PLAYER)
                return;

            if (player->HasAura(WARRIOR_SPELL_GLYPH_OF_GAG_ORDER))
            {
                player->CastSpell(target, WARRIOR_SPELL_SILENCED_GAG_ORDER, true);
            }
        }

    void Register()
        {
            OnHit += SpellHitFn(spell_glyph_of_gag_order_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_glyph_of_gag_order_SpellScript();
    }
};

// Warrior T16 DPS 2P Bonus - 144436
class spell_warr_t16_dps_2p : public SpellScriptLoader
{
    public:
        spell_warr_t16_dps_2p() : SpellScriptLoader("spell_warr_t16_dps_2p") { }

        class spell_warr_t16_dps_2p_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_t16_dps_2p_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(WARRIOR_SPELL_COLOSSUS_SMASH))
                        {
                            if (_player->HasAura(WARRIOR_SPELL_T16_DPS_2P_BONUS))
                                _player->EnergizeBySpell(_player, GetSpellInfo()->Id, 50, POWER_RAGE);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_t16_dps_2p_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_t16_dps_2p_SpellScript();
        }
};

// Raging Blow! - 131116, use spell - 96103 on remove aura
class spell_warr_raging_blow_remove : public SpellScriptLoader
{
    public:
        spell_warr_raging_blow_remove() : SpellScriptLoader("spell_warr_raging_blow_remove") { }

        class spell_warr_raging_blow_remove_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_remove_SpellScript);

            ObjectGuid tar;
            
            void HandleOnCast()
            {                
                Unit* target = GetExplTargetUnit();

                if (!target)
                    return;

                tar = target->GetGUID();
                
                if (GetCaster())
                {
                    if (Aura* bloodCharges = GetCaster()->GetAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW))
                    {
                        if (bloodCharges->GetStackAmount() > 1)
                            bloodCharges->SetStackAmount(bloodCharges->GetStackAmount() - 1);
                        else
                            GetCaster()->RemoveAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW);
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_warr_raging_blow_remove_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_remove_SpellScript();
        }
};

// Spell Reflection - 23920, Shield Wall - 871. Visual shield
class spell_warr_shield_visual : public SpellScriptLoader
{
    public:
        spell_warr_shield_visual() : SpellScriptLoader("spell_warr_shield_visual") { }

        class spell_warr_shield_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shield_visual_SpellScript);
            
            void HandleOnCast()
            {                
                Player* caster = GetCaster()->ToPlayer();
                
                if (GetSpellInfo()->Id == WARRIOR_SPELL_SPELL_REFLECTION)
                {
                        if (!caster->GetShield())
                        {
                            if (caster->GetTeam() == HORDE)
                                    caster->CastSpell(caster, WARRIOR_SPELL_REFLECTION_SHIELD_HORDE, true);
                                else
                                    caster->CastSpell(caster, WARRIOR_SPELL_REFLECTION_SHIELD_ALLIANCE, true);
                        }
                        else
                            caster->CastSpell(caster, WARRIOR_SPELL_REFLECTION_SHIELD_EQUIPED, true);
                }
                if (GetSpellInfo()->Id == WARRIOR_SPELL_SHIELDWALL)
                {
                        if (!caster->GetShield())
                        {
                            if (caster->GetTeam() == HORDE)
                                    caster->CastSpell(caster, WARRIOR_SPELL_SHIELDWALL_SHIELD_HORDE, true);
                                else
                                    caster->CastSpell(caster, WARRIOR_SPELL_SHIELDWALL_SHIELD_ALLIANCE, true);
                        }
                        else
                            caster->CastSpell(caster, WARRIOR_SPELL_SHIELDWALL_SHIELD_EQUIPED, true);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_warr_shield_visual_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shield_visual_SpellScript();
        }
};

// Slam AOE - 146361
class spell_war_slam_aoe : public SpellScriptLoader
{
    public:
        spell_war_slam_aoe() : SpellScriptLoader("spell_war_slam_aoe") { }

        class spell_war_slam_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_war_slam_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if(Unit* target = _player->GetSelectedUnit())
                        targets.remove(target);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_war_slam_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_war_slam_aoe_SpellScript();
        }
};

// Intervene - 3411
class spell_war_intervene : public SpellScriptLoader
{
    public:
        spell_war_intervene() : SpellScriptLoader("spell_war_intervene") { }

        class spell_war_intervene_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_war_intervene_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.sort(CheckHealthState());
                if (targets.size() > 1)
                    targets.resize(1);
            }

            SpellCastResult CheckCast()
            {
                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return SPELL_FAILED_BAD_TARGETS;

                Unit* target = _player->GetSelectedUnit();
                if (!target)
                    return SPELL_FAILED_BAD_TARGETS;

                if (!_player->IsFriendlyTo(target) || (!_player->IsInPartyWith(target) && !_player->IsInRaidWith(target)))
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_war_intervene_SpellScript::CheckCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_war_intervene_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }

        private:
            class CheckHealthState
            {
                public:
                    CheckHealthState() { }

                    bool operator() (WorldObject* a, WorldObject* b) const
                    {
                        Unit* unita = a->ToUnit();
                        Unit* unitb = b->ToUnit();
                        if(!unita)
                            return true;
                        if(!unitb)
                            return false;
                        return unita->GetHealthPct() < unitb->GetHealthPct();
                    }
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_war_intervene_SpellScript();
        }
};

// Warrior Charge Drop Fire Periodic - 126661
class spell_warr_charge_drop_fire : public SpellScriptLoader
{
    public:
        spell_warr_charge_drop_fire() : SpellScriptLoader("spell_warr_charge_drop_fire") { }

        class spell_warr_charge_drop_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_charge_drop_fire_AuraScript);

            Position savePos;
            void OnTick(AuraEffect const* aurEff)
            {
                if(Unit* caster = GetCaster())
                {
                    float distance = caster->GetDistance(savePos);
                    float angle = caster->GetAngle(&savePos);
                    if (uint32 count = uint32(distance))
                    {
                        for(uint32 j = 1; j < count + 1; ++j)
                        {
                            uint32 distanceNext = j;
                            float destx = caster->GetPositionX() + distanceNext * std::cos(angle);
                            float desty = caster->GetPositionY() + distanceNext * std::sin(angle);
                            savePos.Relocate(destx, desty, caster->GetPositionZ());
                            caster->SendSpellCreateVisual(GetSpellInfo(), &savePos);
                        }
                    }
                }
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if(Unit* caster = GetCaster())
                    savePos.Relocate(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_charge_drop_fire_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_charge_drop_fire_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_charge_drop_fire_AuraScript();
        }
};

// Dragon Roar - 118000
class spell_warr_dragon_roar : public SpellScriptLoader
{
    public:
        spell_warr_dragon_roar() : SpellScriptLoader("spell_warr_dragon_roar") { }

        class spell_warr_dragon_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_dragon_roar_SpellScript);

            void HandleOnHit()
            {
                uint32 count = GetSpell()->GetTargetCount();
                int32 damage = GetHitDamage();
                if(count >= 10)
                    damage = damage * 5 / count;
                else if(count >= 5)
                    damage = damage * 0.5f;
                else if(count >= 4)
                    damage = damage * 0.55f;
                else if(count >= 3)
                    damage = damage * 0.65f;
                else if(count >= 2)
                    damage = damage * 0.75f;
                SetHitDamage(damage);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_dragon_roar_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_dragon_roar_SpellScript();
        }
};

// Defensive Stance - 7376
class spell_warr_defensive_stance : public SpellScriptLoader
{
public:
    spell_warr_defensive_stance() : SpellScriptLoader("spell_warr_defensive_stance") { }

    class spell_warr_defensive_stance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_defensive_stance_AuraScript);

        uint32 timer;

        bool Load()
        {
            if (!GetCaster())
                return false;

            timer = 0;
            return true;
        }

        void OnUpdate(uint32 diff, AuraEffect* aurEff)
        {
            timer += diff;
            if (timer >= 3000)
            {
                timer = 0;
                if (Unit* caster = GetCaster())
                    if (caster->isInCombat())
                        caster->ModifyPower(POWER_RAGE, 10);
            }
        }

        void Register()
        {
            OnEffectUpdate += AuraEffectUpdateFn(spell_warr_defensive_stance_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_warr_defensive_stance_AuraScript();
    }
};

// Ravager - 152277
class spell_warr_ravager : public SpellScriptLoader
{
    public:
        spell_warr_ravager() : SpellScriptLoader("spell_warr_ravager") { }

        class spell_warr_ravager_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_ravager_AuraScript);

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->m_SummonSlot[13])
                    {
                        if (Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[13]))
                            caster->CastSpell(summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), 156287, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_ravager_AuraScript::HandlePeriodicTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_ravager_AuraScript();
        }
};

void AddSC_warrior_spell_scripts()
{
    new spell_warr_shield_barrier();
    new spell_warr_shield_block();
    new spell_warr_storm_bolt();
    new spell_warr_colossus_smash();
    new spell_warr_meat_cleaver();
    new spell_warr_staggering_shout();
    new spell_warr_sudden_death();
    new spell_warr_mocking_banner();
    new spell_warr_raging_blow();
    new spell_warr_sword_and_board();
    new spell_warr_mortal_strike();
    new spell_warr_heroic_leap_damage();
    new spell_warr_heroic_leap();
    new spell_warr_shockwave();
    new spell_warr_bloodthirst();
    new spell_warr_victory_rush();
    new spell_warr_last_stand();
    new spell_warr_thunder_clap();
    new spell_warr_deep_wounds();
    new spell_war_glyph_of_die_by_the_sword();
    new spell_glyph_of_gag_order();
    new spell_warr_t16_dps_2p();
    new spell_warr_raging_blow_remove();
    new spell_warr_shield_visual();
    new spell_war_slam_aoe();
    new spell_war_intervene();
    new spell_warr_charge_drop_fire();
    new spell_warr_dragon_roar();
    new spell_warr_defensive_stance();
    new spell_warr_ravager();
    new spell_warr_execute();
}
