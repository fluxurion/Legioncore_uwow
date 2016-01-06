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
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "Battlefield.h"
#include "GridNotifiers.h"
#include "BattlefieldMgr.h"

enum DruidSpells
{
    DRUID_INCREASED_MOONFIRE_DURATION       = 38414,
    DRUID_NATURES_SPLENDOR                  = 57865,
    DRUID_SURVIVAL_INSTINCTS                = 50322,
    DRUID_SAVAGE_ROAR                       = 62071,
    SPELL_DRUID_ITEM_T8_BALANCE_RELIC       = 64950,
    SPELL_DRUID_WRATH                       = 5176,
    SPELL_DRUID_STARFIRE                    = 2912,
    SPELL_DRUID_STARSURGE                   = 78674,
    SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE    = 81070,
    SPELL_DRUID_STARSURGE_ENERGIZE          = 86605,
    SPELL_DRUID_SOLAR_ECLIPSE               = 48517,
    SPELL_DRUID_LUNAR_ECLIPSE               = 48518,
    SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE      = 107095,
    SPELL_DRUID_STARFALL                    = 48505,
    SPELL_DRUID_NATURES_GRACE               = 16886,
    SPELL_DRUID_EUPHORIA                    = 81062,
    SPELL_DRUID_PROWL                       = 5215,
    SPELL_DRUID_WEAKENED_ARMOR              = 113746,
    SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN     = 54810,
    SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE    = 124769,
    SPELL_DRUID_CELESTIAL_ALIGNMENT         = 112071,
    SPELL_DRUID_ASTRAL_COMMUNION            = 127663,
    SPELL_DRUID_SUNFIRE                     = 93402,
    SPELL_DRUID_MOONFIRE                    = 8921,
    DRUID_NPC_WILD_MUSHROOM                 = 47649,
    DRUID_SPELL_FUNGAL_GROWTH_SUMMON        = 81283,
    DRUID_SPELL_MUSHROOM_BIRTH_VISUAL       = 94081,
    DRUID_SPELL_WILD_MUSHROOM_DEATH_VISUAL  = 92701,
    DRUID_SPELL_WILD_MUSHROOM_SUICIDE       = 92853,
    DRUID_SPELL_WILD_MUSHROOM_DAMAGE        = 78777,
    SPELL_DRUID_WILD_MUSHROOM_HEAL          = 102792,
    SPELL_DRUID_FAERIE_DECREASE_SPEED       = 102354,
    SPELL_DRUID_SKULL_BASH_MANA_COST        = 82365,
    SPELL_DRUID_SKULL_BASH_INTERUPT         = 93985,
    SPELL_DRUID_SKULL_BASH_CHARGE           = 93983,
    SPELL_DRUID_FORM_CAT_INCREASE_SPEED     = 113636,
    SPELL_DRUID_GLYPH_OF_REGROWTH           = 116218,
    SPELL_DRUID_REGROWTH                    = 8936,
    SPELL_DRUID_MARK_OF_THE_WILD            = 1126,
    SPELL_DRUID_OMEN_OF_CLARITY             = 113043,
    SPELL_DRUID_CLEARCASTING                = 16870,
    SPELL_DRUID_LIFEBLOOM                   = 33763,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL        = 33778,
    SPELL_DRUID_CAT_FORM                    = 768,
    SPELL_DRUID_BEAR_FORM                   = 5487,
    SPELL_DRUID_BEAR_FORM_RAGE_GAIN         = 17057,
    SPELL_DRUID_INFECTED_WOUNDS             = 58180,
    SPELL_DRUID_BEAR_HUG                    = 102795,
    SPELL_DRUID_RIP                         = 1079,
    SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT    = 132402,
    SPELL_DRUID_DASH                        = 1850,
    SPELL_DRUID_BERSERK_BEAR                = 50334,
    SPELL_DRUID_BERSERK_CAT                 = 106951,
    SPELL_DRUID_STAMPEDING_ROAR             = 106898,
    SPELL_DRUID_SOLAR_BEAM                  = 78675,
    SPELL_DRUID_SOLAR_BEAM_SILENCE          = 81261,
    SPELL_DRUID_URSOLS_VORTEX_AREA_TRIGGER  = 102793,
    SPELL_DRUID_URSOLS_VORTEX_SNARE         = 127797,
    SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST     = 118283,
    SPELL_DRUID_NATURES_VIGIL_HEAL          = 124988,
    SPELL_DRUID_NATURES_VIGIL_DAMAGE        = 124991,
    SPELL_DRUID_SHATTERING_BLOW             = 112997,
    WARLOCK_DEMONIC_CIRCLE_SUMMON           = 48018,
    SPELL_DRUID_RAKE                        = 1822,
    SPELL_DRUID_CONSECRATION_DUMMY          = 81298,
    SPELL_DRUID_CONSECRATION_DAMAGE         = 110705,
    SPELL_DRUID_SHOOTING_STARS              = 93400,
    SPELL_DRUID_AQUATIC_FORM                = 1066,
    SPELL_DRUID_FLY_FORM                    = 33943,
    SPELL_DRUID_SWIFT_FLY_FORM              = 40120,
    SPELL_DRUID_STAG_FORM                   = 165961,
};

// Play Death - 110597
class spell_dru_play_death : public SpellScriptLoader
{
    public:
        spell_dru_play_death() : SpellScriptLoader("spell_dru_play_death") { }

        class spell_dru_play_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_play_death_AuraScript);

            int32 health;
            int32 mana;

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                health = GetTarget()->GetHealth();
                mana = GetTarget()->GetPower(POWER_MANA);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (health && mana)
                {
                    GetTarget()->SetHealth(health);
                    GetTarget()->SetPower(POWER_MANA, mana);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_play_death_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_play_death_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_play_death_AuraScript();
        }
};

// Binary predicate for sorting Units based on value of duration of an Aura
class AuraDurationCompareOrderPred
{
    public:
        AuraDurationCompareOrderPred(ObjectGuid caster, uint32 auraId, bool ascending = true) : m_caster(caster), m_aura(auraId), m_ascending(ascending) {}
        bool operator() (const Unit* a, const Unit* b) const
        {
            return m_ascending ? a->GetAura(m_aura, m_caster)->GetDuration() < b->GetAura(m_aura, m_caster)->GetDuration() :
                                    a->GetAura(m_aura, m_caster)->GetDuration() > b->GetAura(m_aura, m_caster)->GetDuration();
        }
    private:
        ObjectGuid m_caster;
        uint32 m_aura;
        const bool m_ascending;
};

// Incarnation: Tree of Life (Shapeshift) - 33891
// Incarnation: King of the Jungle - 102543
// Incarnation: Son of Ursoc - 102558
// Incarnation: Chosen of Elune (Shapeshift) - 102560
class spell_dru_incarnation : public SpellScriptLoader
{
    public:
        spell_dru_incarnation() : SpellScriptLoader("spell_dru_incarnation") { }

        class spell_dru_incarnation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_AuraScript);

            void UpdateModel()
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                ShapeshiftForm form = target->GetShapeshiftForm();
                if (form == FORM_CAT || form == FORM_BEAR || form == FORM_MOONKIN)
                    if (uint32 model = target->GetModelForForm(form))
                        target->SetDisplayId(model);
            }

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                UpdateModel();

                Unit* target = GetTarget();
                if (!target)
                    return;

                switch (GetId())
                {
                    case 33891:     // Incarnation: Tree of Life (Shapeshift)
                        if (!target->HasAura(117679))
                            target->CastSpell(target, 117679, true);
                        break;
                    case 102543:    // Incarnation: King of the Jungle
                        if (!target->HasAura(768))
                            target->CastSpell(target, 768, true);       // activate Cat Form
                        break;
                    case 102558:    // Incarnation: Son of Ursoc
                        if (!target->HasAura(5487))
                            target->CastSpell(target, 5487, true);      // activate Bear Form
                        break;
                    case 102560:    // Incarnation: Chosen of Elune (Shapeshift)
                        if (!target->HasAura(24858))
                            target->CastSpell(target, 24858, true);     // activate Moonkin Form
                        if (target->HasAura(48517) || target->HasAura(48518) || target->HasAura(112071))   // check Eclipse
                            target->CastSpell(target, 122114, true);    // Chosen of Elune
                        break;
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                UpdateModel();
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);

                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_incarnation_AuraScript();
        }
};

// Incarnation (Passive) - 117679
class spell_dru_incarnation_tree_of_life : public SpellScriptLoader
{
    public:
        spell_dru_incarnation_tree_of_life() : SpellScriptLoader("spell_dru_incarnation_tree_of_life") { }

        class spell_dru_incarnation_tree_of_life_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_tree_of_life_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                target->CastSpell(target, 5420, true);  // Incarnation: Tree of Life (Passive)
                target->CastSpell(target, 81097, true); // Incarnation: Tree of Life (Passive)
                target->CastSpell(target, 81098, true); // Incarnation: Tree of Life (Passive)
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                target->RemoveAurasDueToSpell(33891);
                target->RemoveAurasDueToSpell(5420);
                target->RemoveAurasDueToSpell(81097);
                target->RemoveAurasDueToSpell(81098);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_tree_of_life_AuraScript::OnApply, EFFECT_0, SPELL_AURA_IGNORE_CD, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_tree_of_life_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_IGNORE_CD, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_incarnation_tree_of_life_AuraScript();
        }
};

// Moonfire - 8921
// Sunfire - 93402
class spell_dru_moonfire_sunfire : public SpellScriptLoader
{
    public:
        spell_dru_moonfire_sunfire() : SpellScriptLoader("spell_dru_moonfire_sunfire") { }

        class spell_dru_moonfire_sunfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_sunfire_SpellScript);

            void HandleOnHit()
            {
                if (GetSpell()->IsTriggered())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT))
                            _player->CastSpell(target, GetSpellInfo()->Id == SPELL_DRUID_SUNFIRE ? SPELL_DRUID_MOONFIRE : SPELL_DRUID_SUNFIRE, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_moonfire_sunfire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_moonfire_sunfire_SpellScript();
        }
};

// Solar beam - 78675
class spell_dru_solar_beam : public SpellScriptLoader
{
    public:
        spell_dru_solar_beam() : SpellScriptLoader("spell_dru_solar_beam") { }

        class spell_dru_solar_beam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_solar_beam_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_DRUID_SOLAR_BEAM))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_DRUID_SOLAR_BEAM_SILENCE, true, NULL, aurEff, GetCasterGUID());
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_solar_beam_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_solar_beam_AuraScript();
        }
};

// Called by Mangle (bear) - 33878, Mangle (cat) - 33876, Ravage - 6785 and Shred - 5221
// Rip - 1079
class spell_dru_rip_duration : public SpellScriptLoader
{
    public:
        spell_dru_rip_duration() : SpellScriptLoader("spell_dru_rip_duration") { }

        class spell_dru_rip_duration_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rip_duration_SpellScript);

            uint8 addPerc = 0;

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Each time you Shred, Ravage, or Mangle the target while in Cat Form ...
                        if (_player->GetShapeshiftForm() == FORM_CAT)
                        {
                            if (Aura* rip = target->GetAura(SPELL_DRUID_RIP, _player->GetGUID()))
                            {
                                int32 duration = rip->GetDuration();
                                int32 maxDuration = rip->GetMaxDuration();

                                int32 countMin = maxDuration;
                                int32 countMax = sSpellMgr->GetSpellInfo(SPELL_DRUID_RIP)->GetMaxDuration() + 6000;

                                // ... the duration of your Rip on that target is extended by 2 sec, up to a maximum of 6 sec.
                                if ((countMin + 2000) < countMax)
                                {
                                    rip->SetDuration(duration + 2000);
                                    rip->SetMaxDuration(countMin + 2000);
                                }
                                else if (countMin < countMax)
                                {
                                    rip->SetDuration(duration + 2000);
                                    rip->SetMaxDuration(countMax);
                                }
                            }
                        }
                    }
                    if (addPerc)
                        SetHitDamage(GetHitDamage() + int32(CalculatePct(GetHitDamage(), addPerc)));
                }
            }

            SpellCastResult CheckCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* prowl = caster->GetAura(SPELL_DRUID_PROWL))
                        if (AuraEffect const* aurEff = prowl->GetEffect(EFFECT_3))
                            addPerc = aurEff->GetAmount();
                    if (Aura* prowl = caster->GetAura(102547))
                        if (AuraEffect const* aurEff = prowl->GetEffect(EFFECT_3))
                            addPerc = aurEff->GetAmount();
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_rip_duration_SpellScript::CheckCast);
                OnHit += SpellHitFn(spell_dru_rip_duration_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_rip_duration_SpellScript();
        }
};

// Savage Defense - 62606
class spell_dru_savage_defense : public SpellScriptLoader
{
    public:
        spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

        class spell_dru_savage_defense_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_defense_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* aura = caster->GetAura(SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT))
                    {
                        int32 newDur = aura->GetDuration() + 6 * IN_MILLISECONDS;
                        if(newDur > aura->GetMaxDuration())
                            aura->SetMaxDuration(newDur);
                        aura->SetDuration(newDur);
                    }
                    else
                        caster->CastSpell(caster, SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_savage_defense_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_savage_defense_SpellScript();
        }
};

// Bear Form - 5487
class spell_dru_bear_form : public SpellScriptLoader
{
    public:
        spell_dru_bear_form() : SpellScriptLoader("spell_dru_bear_form") { }

        class spell_dru_bear_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_bear_form_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_DRUID_BEAR_FORM_RAGE_GAIN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_bear_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_bear_form_SpellScript();
        }
};

// Ferocious Bite - 22568
class spell_dru_ferocious_bite : public SpellScriptLoader
{
    public:
        spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

        class spell_dru_ferocious_bite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

            uint8 comboPoints;

            bool Validate(SpellInfo const * /*spellEntry*/)
            {
                comboPoints = 0;
                return true;
            }

            void HandleBeforeCast()
            {
                if (GetCaster()->ToPlayer())
                    comboPoints = GetCaster()->ToPlayer()->GetComboPoints(GetSpellInfo()->Id);
            }

            void Damage(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Player* player = caster->ToPlayer();
                if (!player)
                    return;

                Unit* target = GetHitUnit();
                if (target && target->GetHealthPct() < 25.0f)
                    if (Aura* rip = target->GetAura(SPELL_DRUID_RIP, player->GetGUID()))
                        rip->RefreshDuration();

                float perc = 1.0f;
                int32 bpHp = 5;
                int32 damageN = (GetHitDamage() / 5) * comboPoints;
                int32 curValue = caster->GetPower(POWER_ENERGY);

                if(curValue >= 25)
                {
                    perc += 1.0f;
                    bpHp += 5;
                }
                else if(curValue > 0)
                {
                    perc += curValue * 0.04f;
                    bpHp += int32((curValue * 2) / 10);
                }
                if(caster->HasAura(67598)) //Glyph of Ferocious Bite
                    caster->CastCustomSpell(caster, 101024, &bpHp, NULL, NULL, true);

                SetHitDamage(int32(damageN * perc));

                // Soul of the Forest
                if (player->HasAura(114107) && player->GetSpecializationId(player->GetActiveSpec()) == SPEC_DRUID_CAT)
                {
                    // energize
                    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(114113))
                    {
                        int32 bp = info->Effects[EFFECT_0].CalcValue(player) * comboPoints;
                        player->CastCustomSpell(player, info->Id, &bp, NULL, NULL, true);
                    }
                }
                // if target is under 25% of life, also reset rake duration
                if (target && target->GetHealthPct() <= 25.0f)
                    if (Aura* aura = target->GetAura(1822))
                        aura->RefreshDuration();
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_ferocious_bite_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_ferocious_bite_SpellScript::Damage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_ferocious_bite_SpellScript();
        }
};

// Rip - 1079
class spell_dru_rip : public SpellScriptLoader
{
    public:
        spell_dru_rip() : SpellScriptLoader("spell_dru_rip") { }

        class spell_dru_rip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rip_SpellScript);

            uint8 comboPoints;

            bool Validate(SpellInfo const * /*spellEntry*/)
            {
                comboPoints = 0;
                return true;
            }

            void HandleBeforeCast()
            {
                if (GetCaster()->ToPlayer())
                    comboPoints = GetCaster()->ToPlayer()->GetComboPoints(GetSpellInfo()->Id);
            }

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Player* player = caster->ToPlayer();
                if (!player)
                    return;

                //Hack
                if (Aura* cenarius = caster->GetAura(145152))
                    cenarius->DropCharge();

                // Soul of the Forest
                if (player->HasAura(114107) && player->GetSpecializationId(player->GetActiveSpec()) == SPEC_DRUID_CAT)
                {
                    // energize
                    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(114113))
                    {
                        int32 bp = info->Effects[EFFECT_0].CalcValue(player) * comboPoints;
                        player->CastCustomSpell(player, info->Id, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_rip_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dru_rip_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_rip_SpellScript();
        }
};

// Bear Hug - 102795
class spell_dru_bear_hug : public SpellScriptLoader
{
    public:
        spell_dru_bear_hug() : SpellScriptLoader("spell_dru_bear_hug") { }

        class spell_dru_bear_hug_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_bear_hug_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(_player, SPELL_DRUID_BEAR_FORM, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_bear_hug_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_bear_hug_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_bear_hug_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                    amount = caster->CountPctFromMaxHealth(10);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_hug_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_bear_hug_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_bear_hug_SpellScript();
        }
};

// Lifebloom - 33763 : Final heal
class spell_dru_lifebloom : public SpellScriptLoader
{
    public:
        spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

        class spell_dru_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lifebloom_AuraScript);

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                Unit* caster = GetCaster();
                if (!caster)
                    return;

                caster->CastSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, true);
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* target = GetUnitOwner())
                {
                    if (Unit* caster = GetCaster())
                        caster->CastSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                AfterDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lifebloom_AuraScript();
        }
};

// Called by Lifebloom - 33763
// Omen of Clarity - 113043
class spell_dru_omen_of_clarity : public SpellScriptLoader
{
    public:
        spell_dru_omen_of_clarity() : SpellScriptLoader("spell_dru_omen_of_clarity") { }

        class spell_dru_omen_of_clarity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_omen_of_clarity_AuraScript);

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(SPELL_DRUID_OMEN_OF_CLARITY))
                        if (roll_chance_i(4))
                        {
                            if (caster->HasAura(155577)) // Moment of Clarity
                                caster->CastSpell(caster, 155631, true);
                            else
                                caster->CastSpell(caster, SPELL_DRUID_CLEARCASTING, true);
                        }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_omen_of_clarity_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_omen_of_clarity_AuraScript();
        }
};

// Mark of the Wild - 1126
class spell_dru_mark_of_the_wild : public SpellScriptLoader
{
    public:
        spell_dru_mark_of_the_wild() : SpellScriptLoader("spell_dru_mark_of_the_wild") { }

        class spell_dru_mark_of_the_wild_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_mark_of_the_wild_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if (caster && caster->GetTypeId() == TYPEID_PLAYER)
                {
                    caster->AddAura(SPELL_DRUID_MARK_OF_THE_WILD, caster);

                    std::list<Unit*> memberList;
                    Player* plr = caster->ToPlayer();
                    plr->GetPartyMembers(memberList);

                    for (std::list<Unit*>::const_iterator itr = memberList.begin(); itr != memberList.end(); ++itr)
                        caster->AddAura(SPELL_DRUID_MARK_OF_THE_WILD, (*itr));
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_mark_of_the_wild_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_mark_of_the_wild_SpellScript();
        }
};

// Called by Regrowth - 8936
// Glyph of Regrowth - 116218
class spell_dru_glyph_of_regrowth : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_regrowth() : SpellScriptLoader("spell_dru_glyph_of_regrowth") { }

        class spell_dru_glyph_of_regrowth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_regrowth_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                // Increases the critical strike chance of your Regrowth by 40%, but removes the periodic component of the spell.
                if (GetCaster())
                    if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_REGROWTH))
                        GetTarget()->RemoveAura(SPELL_DRUID_REGROWTH, GetCaster()->GetGUID());
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                // Duration automatically refreshes to 6 sec each time Regrowth heals targets at or below 50% health
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        if (target->GetHealthPct() < 50)
                            if (Aura* regrowth = target->GetAura(SPELL_DRUID_REGROWTH, caster->GetGUID()))
                                regrowth->RefreshDuration();
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_regrowth_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_glyph_of_regrowth_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_regrowth_AuraScript();
        }
};

// Cat Form - 768
class spell_dru_cat_form : public SpellScriptLoader
{
    public:
        spell_dru_cat_form() : SpellScriptLoader("spell_dru_cat_form") { }

        class spell_dru_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_cat_form_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (!_player->HasAura(SPELL_DRUID_FORM_CAT_INCREASE_SPEED))
                    {
                        _player->CastSpell(_player, SPELL_DRUID_FORM_CAT_INCREASE_SPEED, true);
                        _player->RemoveMovementImpairingAuras();
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_cat_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_cat_form_SpellScript();
        }

        class spell_dru_cat_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_cat_form_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (Aura* dash = caster->GetAura(SPELL_DRUID_DASH))
                        if (dash->GetEffect(0))
                            if (dash->GetEffect(0)->GetAmount() == 0)
                                dash->GetEffect(0)->SetAmount(70);
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* dash = caster->GetAura(SPELL_DRUID_DASH))
                        dash->GetEffect(0)->SetAmount(0);

                    if (caster->HasAura(SPELL_DRUID_PROWL))
                        caster->RemoveAura(SPELL_DRUID_PROWL);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_cat_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_cat_form_AuraScript();
        }
};

// Skull Bash - 80964
// Skull Bash - 80965
class spell_dru_skull_bash : public SpellScriptLoader
{
    public:
        spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

        class spell_dru_skull_bash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_skull_bash_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, SPELL_DRUID_SKULL_BASH_MANA_COST, true);
                        _player->CastSpell(target, SPELL_DRUID_SKULL_BASH_INTERUPT, true);
                        _player->CastSpell(target, SPELL_DRUID_SKULL_BASH_CHARGE, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_skull_bash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_skull_bash_SpellScript();
        }
};

// Faerie Swarm - 102355
class spell_dru_faerie_swarm : public SpellScriptLoader
{
    public:
        spell_dru_faerie_swarm() : SpellScriptLoader("spell_dru_faerie_swarm") { }

        class spell_dru_faerie_swarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_faerie_swarm_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, SPELL_DRUID_FAERIE_DECREASE_SPEED, true);
                        _player->CastSpell(target, SPELL_DRUID_WEAKENED_ARMOR, true);
                        _player->CastSpell(target, SPELL_DRUID_WEAKENED_ARMOR, true);
                        _player->CastSpell(target, SPELL_DRUID_WEAKENED_ARMOR, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_faerie_swarm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_faerie_swarm_SpellScript();
        }
};

// Wild Mushroom : Detonate - 88751
class spell_dru_wild_mushroom_detonate : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_detonate() : SpellScriptLoader("spell_dru_wild_mushroom_detonate") { }

        class spell_dru_wild_mushroom_detonate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_detonate_SpellScript)

            // Globals variables
            float spellRange;

            bool Load()
            {
                spellRange = GetSpellInfo()->GetMaxRange(true);

                if (!spellRange)
                    return false;

                return true;
            }

            SpellCastResult CheckCast()
            {
                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return SPELL_FAILED_CASTER_DEAD;

                bool inRange = false;

                for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                {
                    if(player->m_SummonSlot[i])
                    {
                        Creature* summon = player->GetMap()->GetCreature(player->m_SummonSlot[i]);
                        if (summon && summon->IsWithinDistInMap(player, spellRange)) // Must have at least one mushroom within 40 yards
                        {
                            inRange = true;
                            break;
                        }
                    }
                }

                if (!inRange)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_TARGET_TOO_FAR);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                    {
                        if(player->m_SummonSlot[i])
                        {
                            Creature* summon = player->GetMap()->GetCreature(player->m_SummonSlot[i]);
                            if (summon && summon->IsWithinDistInMap(player, spellRange)) // Must have at least one mushroom within 40 yards
                            {
                                summon->SetVisible(true);
                                player->CastSpell(summon, DRUID_SPELL_WILD_MUSHROOM_DAMAGE, true);    // Damage
                                player->CastSpell(summon, DRUID_SPELL_FUNGAL_GROWTH_SUMMON, true);    // Fungal Growth
                                summon->CastSpell(summon, DRUID_SPELL_WILD_MUSHROOM_DEATH_VISUAL, true);// Explosion visual
                                summon->CastSpell(summon, DRUID_SPELL_WILD_MUSHROOM_SUICIDE, true);     // Suicide
                                summon->DespawnOrUnsummon(500);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_detonate_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_wild_mushroom_detonate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_detonate_SpellScript();
        }
};

// Wild Mushroom : Bloom - 102791
class spell_dru_wild_mushroom_bloom : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_bloom() : SpellScriptLoader("spell_dru_wild_mushroom_bloom") { }

        class spell_dru_wild_mushroom_bloom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_bloom_SpellScript)

            // Globals variables
            float spellRange;

            bool Load()
            {
                spellRange = GetSpellInfo()->GetMaxRange(true);

                if (!spellRange)
                    return false;

                return true;
            }

            SpellCastResult CheckCast()
            {
                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return SPELL_FAILED_CASTER_DEAD;

                bool inRange = false;

                for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                {
                    if(player->m_SummonSlot[i])
                    {
                        Creature* summon = player->GetMap()->GetCreature(player->m_SummonSlot[i]);
                        if (summon && summon->IsWithinDistInMap(player, spellRange)) // Must have at least one mushroom within 40 yards
                        {
                            inRange = true;
                            break;
                        }
                    }
                }

                if (!inRange)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_TARGET_TOO_FAR);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                    {
                        if(player->m_SummonSlot[i])
                        {
                            Creature* summon = player->GetMap()->GetCreature(player->m_SummonSlot[i]);
                            if (summon && summon->IsWithinDistInMap(player, spellRange)) // Must have at least one mushroom within 40 yards
                            {
                                player->CastSpell(summon, SPELL_DRUID_WILD_MUSHROOM_HEAL, true);

                                if (DynamicObject* dynObj = summon->GetDynObject(81262))
                                    dynObj->SetDuration(0);
                                summon->CastSpell(summon, 116302, true); // Explosion visual and suicide
                                summon->CastSpell(summon, 116305, true); // Explosion visual and suicide
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_bloom_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_wild_mushroom_bloom_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_bloom_SpellScript();
        }
};

// Astral Communion - 127663
class spell_dru_astral_communion : public SpellScriptLoader
{
    public:
        spell_dru_astral_communion() : SpellScriptLoader("spell_dru_astral_communion") { }

        class spell_dru_astral_communion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_astral_communion_AuraScript);

            int32 direction;
            bool Load()
            {
                direction = 1;
                return true;
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                {
                    direction = caster->HasAura(67483) ? 1 : -1;

                    if (Aura* aura = caster->GetAura(145138))
                    {
                        int32 _amount = 100 * direction;
                        caster->CastCustomSpell(caster, 89265, &_amount, NULL, NULL, true);
                        aura->Remove(AURA_REMOVE_BY_DEFAULT);
                        GetAura()->Remove();
                    }
                }
            }

            void OnTick(AuraEffect const* aurEff)
            {
                Player* player = GetTarget()->ToPlayer();
                if (!player)
                    return;

                int32 mod = aurEff->GetAmount() * direction;
                // energize
                player->CastCustomSpell(player, 89265, &mod, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_astral_communion_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_astral_communion_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_astral_communion_AuraScript();
        }
};

// Celestial Alignment - 112071
class spell_dru_celestial_alignment : public SpellScriptLoader
{
    public:
        spell_dru_celestial_alignment() : SpellScriptLoader("spell_dru_celestial_alignment") { }

        class spell_dru_celestial_alignment_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_celestial_alignment_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                Player* player = target->ToPlayer();
                if (!player)
                    return;

                player->SetPower(POWER_LUNAR_POWER, 0);

                player->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
                player->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE);
                player->RemoveAurasDueToSpell(67483);  // markers
                player->RemoveAurasDueToSpell(67484);

                player->CastSpell(player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true);
                player->CastSpell(player, SPELL_DRUID_NATURES_GRACE, true); // Cast Nature's Grace
                player->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                Player* player = target->ToPlayer();
                if (!player)
                    return;

                player->RemoveAura(SPELL_DRUID_NATURES_GRACE);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_celestial_alignment_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_celestial_alignment_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_celestial_alignment_AuraScript();
        }
};

// Frenzied Regeneration - 22842
class spell_dru_frenzied_regeneration : public SpellScriptLoader
{
    public:
        spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

        class spell_dru_frenzied_regeneration_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_frenzied_regeneration_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!_player->HasAura(SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN))
                        {
                            float rageused = _player->GetPower(POWER_RAGE);
                            if (rageused >= 600.0f)
                                rageused = 600.0f;

                            SetHitHeal(int32(GetHitHeal() * float(rageused / 600.0f)));
                            _player->EnergizeBySpell(_player, 22842, -rageused, POWER_RAGE);
                            sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "spell_dru_frenzied_regeneration rageused %i GetHitHeal%i", rageused, GetHitHeal());
                        }
                        else
                        {
                            SetHitHeal(0);
                            _player->CastSpell(_player, SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE, true);
                        }
                    }
                }
            }

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetPower(POWER_RAGE) < 10)
                    return SPELL_FAILED_NOT_READY;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_frenzied_regeneration_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_frenzied_regeneration_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_frenzied_regeneration_SpellScript();
        }
};

// Stampeding Roar - 106898
class spell_dru_stampeding_roar_speed : public SpellScriptLoader
{
    public:
        spell_dru_stampeding_roar_speed() : SpellScriptLoader("spell_dru_stampeding_roar_speed") { }

        class spell_dru_stampeding_roar_speed_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_stampeding_roar_speed_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_DRUID_DASH))
                        if (_player->HasAura(SPELL_DRUID_STAMPEDING_ROAR))
                            _player->RemoveAura(SPELL_DRUID_STAMPEDING_ROAR);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_stampeding_roar_speed_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_stampeding_roar_speed_SpellScript();
        }
};

// Faerie Fire - 770
class spell_dru_faerie_fire : public SpellScriptLoader
{
    public:
        spell_dru_faerie_fire() : SpellScriptLoader("spell_dru_faerie_fire") { }

        class spell_dru_faerie_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_faerie_fire_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, SPELL_DRUID_WEAKENED_ARMOR, true);
                        _player->CastSpell(target, SPELL_DRUID_WEAKENED_ARMOR, true);
                        _player->CastSpell(target, SPELL_DRUID_WEAKENED_ARMOR, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_faerie_fire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_faerie_fire_SpellScript();
        }
};

// Teleport : Moonglade - 18960
class spell_dru_teleport_moonglade : public SpellScriptLoader
{
    public:
        spell_dru_teleport_moonglade() : SpellScriptLoader("spell_dru_teleport_moonglade") { }

        class spell_dru_teleport_moonglade_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_teleport_moonglade_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if(_player->GetDistance(7964.063f, -2491.099f, 487.83f) > 100.0f)
                    {
                        _player->SaveRecallPosition();
                        _player->TeleportTo(1, 7964.063f, -2491.099f, 487.83f, _player->GetOrientation());
                    }
                    else
                        _player->TeleportTo(_player->m_recallMap, _player->m_recallX, _player->m_recallY, _player->m_recallZ, _player->m_recallO);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dru_teleport_moonglade_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_teleport_moonglade_SpellScript();
        }
};

// Growl - 6795, Might of Ursoc - 106922, Stampeding Roar - 106898
class spell_dru_growl : public SpellScriptLoader
{
    public:
        spell_dru_growl() : SpellScriptLoader("spell_dru_growl") { }

        class spell_dru_growl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_growl_SpellScript);

            void HandleOnHit()
            {
                // This spells activate the bear form
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GetSpellInfo()->Id == 106898 && _player->GetShapeshiftForm() != FORM_CAT && _player->GetShapeshiftForm() != FORM_BEAR && !_player->HasAura(114300))
                        _player->CastSpell(_player, SPELL_DRUID_BEAR_FORM, true);
                    else if (GetSpellInfo()->Id != 106898)
                        _player->CastSpell(_player, SPELL_DRUID_BEAR_FORM, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_growl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_growl_SpellScript();
        }
};

// Prowl - 5212, Prowl - 102547 and Dash - 1850
class spell_dru_prowl : public SpellScriptLoader
{
    public:
        spell_dru_prowl() : SpellScriptLoader("spell_dru_prowl") { }

        class spell_dru_prowl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_prowl_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_PROWL))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                // This spell activate the cat form
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_DRUID_CAT_FORM, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_prowl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_prowl_SpellScript();
        }
};

// 5176 - Wrath, 2912 - Starfire and 78674 - Starsurge
class spell_dru_eclipse : public SpellScriptLoader
{
    public:
        spell_dru_eclipse() : SpellScriptLoader("spell_dru_eclipse") { }

        class spell_dru_eclipse_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_eclipse_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DRUID_BALANCE)
                        {
                            if (GetSpell()->IsCritForTarget(target))
                            {
                                if (GetSpellInfo()->Id == SPELL_DRUID_WRATH || GetSpellInfo()->Id == SPELL_DRUID_STARSURGE)
                                {
                                    if (Aura* aura = target->GetAura(SPELL_DRUID_SUNFIRE, _player->GetGUID()))
                                    {
                                        int32 newDur = aura->GetDuration() + 2 * IN_MILLISECONDS;
                                        if (newDur > aura->GetMaxDuration())
                                            newDur = aura->GetMaxDuration();
                                        aura->SetDuration(newDur);
                                    }
                                }
                                if (GetSpellInfo()->Id == SPELL_DRUID_STARFIRE || GetSpellInfo()->Id == SPELL_DRUID_STARSURGE)
                                {
                                    if (Aura* aura = target->GetAura(SPELL_DRUID_MOONFIRE, _player->GetGUID()))
                                    {
                                        int32 newDur = aura->GetDuration() + 2 * IN_MILLISECONDS;
                                        if (newDur > aura->GetMaxDuration())
                                            newDur = aura->GetMaxDuration();
                                        aura->SetDuration(newDur);
                                    }
                                }
                            }
                        }
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DRUID_RESTORATION && GetSpellInfo()->Id == SPELL_DRUID_WRATH)
                        {
                            if (_player->HasAura(108373))
                                SetHitDamage(int32(GetHitDamage() * 1.2f));
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_eclipse_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_eclipse_SpellScript();
        }
};

class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

        class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster()->ToPlayer()->GetGroup())
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
                else
                {
                    targets.remove(GetExplTargetUnit());
                    std::list<Unit*> tempTargets;
                    for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        if ((*itr)->GetTypeId() == TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                            tempTargets.push_back((*itr)->ToUnit());

                    if (tempTargets.empty())
                    {
                        targets.clear();
                        FinishCast(SPELL_FAILED_DONT_REPORT);
                        return;
                    }

                    Unit* target = Trinity::Containers::SelectRandomContainerElement(tempTargets);
                    targets.clear();
                    targets.push_back(target);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_t10_restoration_4p_bonus_SpellScript();
        }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
    public:
        spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

        class spell_dru_swift_flight_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                        amount = 310;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_swift_flight_passive_AuraScript();
        }
};

class spell_dru_starfall_dummy : public SpellScriptLoader
{
    public:
        spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

        class spell_dru_starfall_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if(!GetCaster())
                {
                    targets.clear();
                    return;
                }

                targets.remove_if(Trinity::UnitCheckCCAura(true, GetCaster()));
                if(GetCaster()->HasAura(146655))
                    targets.remove_if(AuraCheck());

                Trinity::Containers::RandomResizeList(targets, 2);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                // Shapeshifting into an animal form or mounting cancels the effect
                if (caster->GetCreatureType() == CREATURE_TYPE_BEAST || caster->IsMounted())
                {
                    if (SpellInfo const* spellInfo = GetTriggeringSpell())
                        caster->RemoveAurasDueToSpell(spellInfo->Id);
                    return;
                }

                // Any effect which causes you to lose control of your character will supress the starfall effect.
                //if (caster->HasUnitState(UNIT_STATE_CONTROLLED))
                    //return;

                caster->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        private:
            class AuraCheck
            {
                public:
                    AuraCheck(){}

                    bool operator()(WorldObject* unit)
                    {
                       return (!unit->ToUnit() || (!unit->ToUnit()->HasAura(8921) && !unit->ToUnit()->HasAura(93402)));
                    }
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_starfall_dummy_SpellScript();
        }
};

class spell_dru_savage_roar : public SpellScriptLoader
{
    public:
        spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

        class spell_dru_savage_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_roar_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetShapeshiftForm() != FORM_CAT)
                    return SPELL_FAILED_ONLY_SHAPESHIFT;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
            }
        };

        class spell_dru_savage_roar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_savage_roar_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(DRUID_SAVAGE_ROAR))
                    return false;
                return true;
            }

            void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->CastSpell(target, DRUID_SAVAGE_ROAR, true, NULL, aurEff, GetCasterGUID());
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(DRUID_SAVAGE_ROAR);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_savage_roar_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_savage_roar_AuraScript();
        }
};

class spell_dru_survival_instincts : public SpellScriptLoader
{
    public:
        spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

        class spell_dru_survival_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(DRUID_SURVIVAL_INSTINCTS))
                    return false;
                return true;
            }

            void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                int32 bp0 = target->CountPctFromMaxHealth(aurEff->GetAmount());
                target->CastCustomSpell(target, DRUID_SURVIVAL_INSTINCTS, &bp0, NULL, NULL, true);
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(DRUID_SURVIVAL_INSTINCTS);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_survival_instincts_AuraScript();
        }
};

class spell_druid_rejuvenation : public SpellScriptLoader
{
    public:
        spell_druid_rejuvenation() : SpellScriptLoader("spell_druid_rejuvenation") { }

        class spell_druid_rejuvenation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_druid_rejuvenation_SpellScript)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit * caster = GetCaster())
                {
                    if (caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    caster->ToPlayer()->KilledMonsterCredit(44175, ObjectGuid::Empty);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_druid_rejuvenation_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        class spell_druid_rejuvenation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_druid_rejuvenation_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if (Aura* aura = caster->GetAura(17076))
                    {
                        aura->SetCustomData(aura->GetCustomData() + 1);
                        if(aura->GetCustomData() >= 3 && !caster->HasAura(96206))
                            caster->CastSpell(caster, 96206, true);
                    }
                }
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if (Aura* aura = caster->GetAura(17076))
                    {
                        aura->SetCustomData(aura->GetCustomData() - 1);
                        if(aura->GetCustomData() < 3 && caster->HasAura(96206))
                            caster->RemoveAurasDueToSpell(96206);
                    }
                }
            }

            void HandleTick(AuraEffect const* /*aurEff*/, int32& amount, Unit* target)
            {
                Unit* caster = GetCaster();
                if(!caster || !target)
                    return;
                Player* _player = caster->ToPlayer();
                if(!_player)
                    return;
                if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DRUID_RESTORATION)
                {
                    Creature* summon = _player->GetMap()->GetCreature(_player->m_SummonSlot[SUMMON_SLOT_TOTEM]);
                    int32 maxhealth = caster->GetMaxHealth();
                    int32 health = int32(target->GetHealth() - target->GetMaxHealth()) + amount;
                    int32 scale = int32(health * 100/maxhealth);
                    if(health > 0 && summon)
                    {
                        if (Aura* aura = summon->GetAura(138616))
                        {
                            if(aura->GetEffect(1)->GetAmount() < (maxhealth * 2))
                            {
                                health += aura->GetEffect(1)->GetAmount();
                                if(health > (maxhealth * 2))
                                    health = maxhealth * 2;
                                aura->GetEffect(0)->SetAmount(int32(health * 100/maxhealth));
                                aura->GetEffect(1)->SetAmount(health);
                            }
                        }
                        else
                            summon->CastCustomSpell(summon, 138616, &scale, &health, NULL, true);
                    }
                }
                if (AuraEffect const* aurEffset = caster->GetAuraEffect(138286, EFFECT_0)) // Item - Druid T15 Restoration 4P Bonus
                {
                    float percent = (aurEffset->GetAmount() / 100.0f) + 1.0f;
                    if (AuraEffect* aurEff = GetAura()->GetEffect(2))
                        aurEff->SetAmount(int32(aurEff->GetAmount() * percent));
                }
            }

            void Register()
            {
                DoEffectChangeTickDamage += AuraEffectChangeTickDamageFn(spell_druid_rejuvenation_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectApply += AuraEffectApplyFn(spell_druid_rejuvenation_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_rejuvenation_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_druid_rejuvenation_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_druid_rejuvenation_SpellScript();
        }
};

class spell_druid_barkskin : public SpellScriptLoader
{
    public:
        spell_druid_barkskin() : SpellScriptLoader("spell_druid_barkskin") { }

        class spell_druid_barkskin_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_druid_barkskin_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                // Glyph of Barkskin
                if (target->HasAura(63057))
                    // Glyph of Amberskin Protection
                    target->CastSpell(target, 63058, true);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->RemoveAurasDueToSpell(63058);
                if (Unit* caster = GetCaster())
                    if(caster->HasAura(144879)) //Item - Druid T16 Guardian 2P Bonus
                    {
                        if (Aura* aura = caster->GetAura(SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT))
                        {
                            int32 newDur = aura->GetDuration() + 6 * IN_MILLISECONDS;
                            if(newDur > aura->GetMaxDuration())
                                aura->SetMaxDuration(newDur);
                            aura->SetDuration(newDur);
                        }
                        else
                            caster->CastSpell(caster, SPELL_DRUID_SAVAGE_DEFENSE_DODGE_PCT, true);
                    }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_druid_barkskin_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_barkskin_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_druid_barkskin_AuraScript();
        }
};

class spell_druid_glyph_of_the_treant : public SpellScriptLoader
{
    public:
        spell_druid_glyph_of_the_treant() : SpellScriptLoader("spell_druid_glyph_of_the_treant") { }

        class spell_druid_glyph_of_the_treant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_druid_glyph_of_the_treant_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    // Treant Form
                    target->learnSpell(114282, false);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    target->removeSpell(114282);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_druid_glyph_of_the_treant_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_glyph_of_the_treant_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_druid_glyph_of_the_treant_AuraScript();
        }
};

// Heart of the Wild - 108288
class spell_druid_heart_of_the_wild : public SpellScriptLoader
{
    public:
        spell_druid_heart_of_the_wild() : SpellScriptLoader("spell_druid_heart_of_the_wild") { }

        class spell_druid_heart_of_the_wild_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_druid_heart_of_the_wild_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* target = GetTarget()->ToPlayer();
                if (!target)
                    return;

                ShapeshiftForm form = target->GetShapeshiftForm();
                uint32 spell = GetSpellInfo()->Id;

                if ((spell == 108291 || spell == 108292 || spell == 108294) && form == FORM_BEAR)
                    target->CastSpell(target, 123738, true);
                if ((spell == 108291 || spell == 108293 || spell == 108294) && form == FORM_CAT)
                    target->CastSpell(target, 123737, true);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* target = GetTarget()->ToPlayer();
                if (!target)
                    return;

                target->RemoveAurasDueToSpell(123737);
                target->RemoveAurasDueToSpell(123738);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_druid_heart_of_the_wild_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_heart_of_the_wild_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_druid_heart_of_the_wild_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_SPELL_HIT_CHANCE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_heart_of_the_wild_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_SPELL_HIT_CHANCE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_druid_heart_of_the_wild_AuraScript();
        }
};

// Eclipse (Solar) - 48517
// Eclipse (Lunar) - 48518
class spell_druid_eclipse_buff : public SpellScriptLoader
{
    public:
        spell_druid_eclipse_buff() : SpellScriptLoader("spell_druid_eclipse_buff") { }

        class spell_druid_eclipse_buff_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_druid_eclipse_buff_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                Player* player = target->ToPlayer();
                if (!player)
                    return;

                if (player->HasAura(102560))                            // check Incarnation
                    player->CastSpell(player, 122114, true);            // Chosen of Elune

                // Eclipse (Lunar) - cast Astral Storm override
                if (GetId() == 48518)
                {
                    player->CastSpell(player, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);
                    // remove Starfall cooldown (source?)
                    if (player->HasSpellCooldown(48505))
                        player->RemoveSpellCooldown(48505, true);
                }

                // Remove Nature's Grace Cooldown
                player->RemoveSpellCooldown(16880, true);

                // solar marker or lunar marker
                uint32 markerSpellAdd =  GetId() == 48517 ? 67484 : 67483;
                uint32 markerSpellRemove = GetId() == 48517 ? 67483 : 67484;
                player->RemoveAurasDueToSpell(markerSpellRemove);
                if (!player->HasAura(markerSpellAdd))
                    player->CastSpell(player, markerSpellAdd, true);

                // Item - Druid T11 Balance 4P Bonus
                if (AuraEffect* aura = player->GetAuraEffect(90163, EFFECT_0))
                {
                    // Astral Alignment
                    if (SpellInfo const* bonus = sSpellMgr->GetSpellInfo(90164))
                    {
                        int32 bp = bonus->Effects[0].CalcValue(player) * 3;
                        player->CastCustomSpell(player, bonus->Id, &bp, NULL, NULL, true, NULL, aura);
                    }
                }

                // search Euphoria, energize mana
                if (player->HasAura(81062))
                    player->CastSpell(player, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, true);

                // cast Nature's Grace
                player->CastSpell(player, 16886, true);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                target->RemoveAurasDueToSpell(122114);

                Player* player = target->ToPlayer();
                if (!player)
                    return;

                player->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_druid_eclipse_buff_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_eclipse_buff_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_druid_eclipse_buff_AuraScript();
        }
};

// Tiger's Fury - 5217
// Berserk - 106951
class spell_druid_berserk_tiger_fury_buff : public SpellScriptLoader
{
    public:
        spell_druid_berserk_tiger_fury_buff() : SpellScriptLoader("spell_druid_berserk_tiger_fury_buff") { }

        class spell_druid_berserk_tiger_fury_buff_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_druid_berserk_tiger_fury_buff_AuraScript);

            void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                // Glyph of Shred (Feral)
                if (target->HasAura(114234))
                    target->CastSpell(target, 114235, true, NULL, aurEff);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                if((GetSpellInfo()->Id == 5217 && !target->HasAura(106951)) || (GetSpellInfo()->Id == 106951 && !target->HasAura(5217)))
                    target->RemoveAurasDueToSpell(114235);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_druid_berserk_tiger_fury_buff_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_druid_berserk_tiger_fury_buff_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);

                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_berserk_tiger_fury_buff_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_druid_berserk_tiger_fury_buff_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_druid_berserk_tiger_fury_buff_AuraScript();
        }
};

// Item - Druid PvP Set Feral 4P Bonus - 131537
class spell_dru_a12_4p_feral_bonus : public SpellScriptLoader
{
    public:
        spell_dru_a12_4p_feral_bonus() : SpellScriptLoader("spell_dru_a12_4p_feral_bonus") { }

        class spell_dru_a12_4p_feral_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_a12_4p_feral_bonus_AuraScript);


            bool Load()
            {
                if (!GetCaster())
                    return false;

                cooldown = 30000;
                return true;
            }

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                if (!GetCaster()->HasAura(81022))
                {
                    if (cooldown <= 0)
                    {
                        GetCaster()->CastSpell(GetCaster(), 81022, true);
                        cooldown = 30000;
                    }
                    else
                    {
                        cooldown -= diff;
                    }
                }

            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_a12_4p_feral_bonus_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
            }

            int32 cooldown;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_a12_4p_feral_bonus_AuraScript();
        }
};

// 106785 - Swipe
class spell_dru_swipe : public SpellScriptLoader
{
    public:
        spell_dru_swipe() : SpellScriptLoader("spell_dru_swipe") { }

        class spell_dru_swipe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swipe_SpellScript);

            void Damage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    GetSpell()->AddEffectTarget(unitTarget->GetGUID());
                    if(unitTarget->HasAuraState(AURA_STATE_BLEEDING))
                        SetHitDamage(GetHitDamage() + int32(CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_1].BasePoints)));
                }
            }

            void HandleAfterCast()
            {
                if(GetSpell()->GetEffectTargets().empty())
                    return;

                if (Unit* caster = GetCaster())
                    caster->ModifyPower(POWER_COMBO_POINTS, 1);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_swipe_SpellScript::Damage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
                AfterCast += SpellCastFn(spell_dru_swipe_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_swipe_SpellScript();
        }
};

// Tooth and Claw - 6807
class spell_dru_tooth_and_claw : public SpellScriptLoader
{
    public:
        spell_dru_tooth_and_claw() : SpellScriptLoader("spell_dru_tooth_and_claw") { }

        class spell_dru_tooth_and_claw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_tooth_and_claw_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if(caster->HasAura(135286))
                        {
                            int32 bp = int32(caster->GetTotalAttackPowerValue(BASE_ATTACK) * 2.4f);
                            if (AuraEffect* effect = caster->GetAuraEffect(135597, 0))
                                effect->SetAmount(effect->GetAmount() + bp);
                            else
                                caster->CastCustomSpell(caster, 135597, &bp, NULL, NULL, true);

                            if (AuraEffect* effect = target->GetAuraEffect(135601, 0))
                                effect->SetAmount(effect->GetAmount() + bp);
                            else
                                caster->CastCustomSpell(target, 135601, &bp, NULL, NULL, true);
                        }

                        if (target->HasAuraWithMechanic((1<<MECHANIC_BLEED)))
                            SetHitDamage(int32(GetHitDamage() * 1.2f));
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_tooth_and_claw_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_tooth_and_claw_SpellScript();
        }
};

// Tooth and Claw - 135597
class spell_dru_spell_dru_tooth_and_claw_absorb : public SpellScriptLoader
{
    public:
        spell_dru_spell_dru_tooth_and_claw_absorb() : SpellScriptLoader("spell_dru_spell_dru_tooth_and_claw_absorb") { }

        class spell_dru_spell_dru_tooth_and_claw_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_spell_dru_tooth_and_claw_absorb_AuraScript);

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if(Unit* target = dmgInfo.GetAttacker())
                {
                    if (Aura* aura = target->GetAura(135601))
                    {
                        GetAura()->Remove();
                        aura->Remove(AURA_REMOVE_BY_DEFAULT);
                    }
                    else
                        absorbAmount = 0;
                }
                else
                    absorbAmount = 0;
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_spell_dru_tooth_and_claw_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_spell_dru_tooth_and_claw_absorb_AuraScript();
        }
};

// Healing Touch - 5185 from Dream of Cenarius
class spell_dru_healing_ouch : public SpellScriptLoader
{
    public:
        spell_dru_healing_ouch() : SpellScriptLoader("spell_dru_healing_ouch") { }

        class spell_dru_healing_ouch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_healing_ouch_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Player* player = caster->ToPlayer();
                if (!player)
                    return;

                if(caster->HasAura(145162))
                {
                    if (player->GetSpecializationId(player->GetActiveSpec()) == SPEC_DRUID_BEAR)
                    {
                        int32 _heal = irand(18460, 21800) + int32(player->GetTotalAttackPowerValue(BASE_ATTACK) * 1.86);
                        SetHitHeal(_heal);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_healing_ouch_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_healing_ouch_SpellScript();
        }
};

// Wild Mushroom - 145205, 147349
class spell_dru_wild_mushroom_heal : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_heal() : SpellScriptLoader("spell_dru_wild_mushroom_heal") { }

        class spell_dru_wild_mushroom_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_heal_SpellScript)

            int32 savePoints;

            bool Load()
            {
                savePoints = 0;
                return true;
            }

            void HandleBeforeCast()
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;
                if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[SUMMON_SLOT_TOTEM]))
                {
                    if (Aura* aura = summon->GetAura(138616))
                        savePoints = aura->GetEffect(1)->GetAmount();
                    if (DynamicObject* dynObj = summon->GetDynObject(81262))
                        dynObj->SetDuration(0);
                }
            }

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;
                if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[SUMMON_SLOT_TOTEM]))
                {
                    if (savePoints != 0)
                    {
                        int32 maxhealth = caster->GetMaxHealth();
                        int32 scale = int32(savePoints * 100/maxhealth);
                        summon->CastCustomSpell(summon, 138616, &scale, &savePoints, NULL, true);
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dru_wild_mushroom_heal_SpellScript::HandleBeforeCast);
                AfterCast += SpellCastFn(spell_dru_wild_mushroom_heal_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_heal_SpellScript();
        }
};

//Genesis - 145518
class spell_dru_genesis : public SpellScriptLoader
{
    public:
        spell_dru_genesis() : SpellScriptLoader("spell_dru_genesis") { }

        class spell_dru_genesis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_genesis_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Aura* aura = target->GetAura(774))
                    {
                        if(aura->GetDuration() < 4000)
                            return;

                        if (AuraEffect* eff = aura->GetEffect(EFFECT_0))
                        {
                            int32 tick = eff->GetTotalTicks() - eff->GetTickNumber();
                            int32 bp = (eff->GetAmount() * tick) / 5;
                            if (Unit* caster = GetCaster())
                                caster->CastCustomSpell(target, 162359, &bp, 0, 0, true);
                        }
                        aura->Remove();
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_genesis_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_genesis_SpellScript();
        }
};

// 16914,106996 - Glyph of Hurricane
class spell_dru_glyph_of_hurricane : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_hurricane() : SpellScriptLoader("spell_dru_glyph_of_hurricane") { }

        class spell_dru_glyph_of_hurricane_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_glyph_of_hurricane_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(54831))
                        amount = -50;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_glyph_of_hurricane_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_glyph_of_hurricane_AuraScript();
        }
};

// 81262 - Efflorescence with Glyph of Efflorescence
class spell_dru_efflorescence_dumy : public SpellScriptLoader
{
    public:
        spell_dru_efflorescence_dumy() : SpellScriptLoader("spell_dru_efflorescence_dumy") { }

        class spell_dru_efflorescence_dumy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_efflorescence_dumy_AuraScript);

            void CalculateMaxDuration(int32 & duration)
            {
                if (Unit* caster = GetCaster())
                    if(Unit* owner = caster->GetAnyOwner())
                        if (owner->HasAura(145529))
                            duration = 300500;
            }

            void Register()
            {
                DoCalcMaxDuration += AuraCalcMaxDurationFn(spell_dru_efflorescence_dumy_AuraScript::CalculateMaxDuration);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_efflorescence_dumy_AuraScript();
        }
};

class spell_druid_dream_of_cenarius: public SpellScriptLoader
{
    public:
        spell_druid_dream_of_cenarius() : SpellScriptLoader("spell_druid_dream_of_cenarius") { }

        class spell_druid_dream_of_cenarius_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_druid_dream_of_cenarius_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                uint32 count = 2;

                for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
                {
                    if ((*itr)->ToUnit() && (*itr)->ToUnit()->GetHealthPct() != 100)
                        ++itr;
                    else
                        targets.erase(itr++);
                }
                if (targets.size() > count)
                    targets.resize(count);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_druid_dream_of_cenarius_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_druid_dream_of_cenarius_SpellScript();
        }
};

// Frenzied Regeneration - 122307
class spell_dru_frenzied_regeneration_t16 : public SpellScriptLoader
{
    public:
        spell_dru_frenzied_regeneration_t16() : SpellScriptLoader("spell_dru_frenzied_regeneration_t16") { }

        class spell_dru_frenzied_regeneration_t16_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_frenzied_regeneration_t16_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (AuraEffect const* aurEff = caster->GetAuraEffect(144879, EFFECT_0)) //Item - Druid T16 Guardian 2P Bonus
                    {
                        int32 rageused = aurEff->GetAmount() * 10;
                        int32 AP = caster->GetTotalAttackPowerValue(BASE_ATTACK);
                        int32 agility = caster->GetStat(STAT_AGILITY) * 2;
                        int32 stamina = int32(caster->GetStat(STAT_STAMINA));
                        int32 a = (AP - agility) * 220 / 100;
                        int32 b = stamina * 250 / 100;

                        int32 healAmount = int32(std::max(a, b));
                        healAmount = rageused * healAmount / 600;

                        SetHitHeal(healAmount);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_frenzied_regeneration_t16_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_frenzied_regeneration_t16_SpellScript();
        }
};

// 144888 - Ursoc's Vigor
class spell_dru_ursocs_vigor : public SpellScriptLoader
{
    public:
        spell_dru_ursocs_vigor() : SpellScriptLoader("spell_dru_ursocs_vigor") { }

        class spell_dru_ursocs_vigor_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursocs_vigor_AuraScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Player* caster = GetCaster()->ToPlayer())
                    if (AuraEffect const* aurEff = caster->GetAuraEffect(144887, EFFECT_0)) // Item - Druid T16 Guardian 4P Bonus
                        amount = int32(caster->GetTotalAttackPowerValue(BASE_ATTACK) * float(aurEff->GetAmount() / 100.0f));
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ursocs_vigor_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ursocs_vigor_AuraScript();
        }
};

// Glyph of One With Nature - 147420
class spell_dru_one_with_nature : public SpellScriptLoader
{
    public:
        spell_dru_one_with_nature() : SpellScriptLoader("spell_dru_one_with_nature") { }

        class spell_dru_one_with_nature_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_one_with_nature_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if (!caster || caster->isInCombat())
                    return;

                Player* player = caster->ToPlayer();
                if (!player)
                    return;

                float X, Y, Z;
                uint32 mapId;
                switch (irand(0,7))
                {
                    case 0:
                        mapId = 870;
                        X = 272.611755f; Y = 1965.844971f; Z = 164.396271f;
                        break;
                    case 1:
                        mapId = 571;
                        X = 6673.821777f; Y = 4872.219727f; Z = -10.785309f;
                        break;
                    case 2:
                        mapId = 1;
                        X = 4990.475098f; Y = 108.657242f; Z = 52.308815f;
                        break;
                    case 3:
                        mapId = 1;
                        X = 7831.314941f; Y = -2479.068604f; Z = 487.088989f;
                        break;
                    case 4:
                        mapId = 870;
                        X = 854.048828f; Y = -1548.728394f; Z = 66.733719f;
                        break;
                    case 5:
                        mapId = 1;
                        X = 3806.464111f; Y = 128.605453f; Z = 9.519085f;
                        break;
                    case 6:
                        mapId = 0;
                        X = 2367.002197f; Y = 1266.341553f; Z = 31.316511f;
                        break;
                    case 7:
                        mapId = 0;
                        X = -12454.017578f; Y = -2722.260742f; Z = 0.786638f;
                        break;
                    default:
                        mapId = 870;
                        X = 272.611755f; Y = 1965.844971f; Z = 164.396271f;
                        break;
                }
                player->TeleportTo(mapId, X, Y, Z, player->GetOrientation());
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_one_with_nature_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_one_with_nature_SpellScript();
        }
};

// 102792 - Wild Mushroom: Bloom
class spell_dru_wild_mushroom_bloom_heal : public SpellScriptLoader
{
    public:
        spell_dru_wild_mushroom_bloom_heal() : SpellScriptLoader("spell_dru_wild_mushroom_bloom_heal") { }

        class spell_dru_wild_mushroom_bloom_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_mushroom_bloom_heal_SpellScript);

            void HandleOnHit()
            {
                int32 _heal = GetHitHeal();
                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
                    {
                        if(player->m_SummonSlot[i])
                        {
                            if (Creature* summon = player->GetMap()->GetCreature(player->m_SummonSlot[i]))
                            {
                                if (Aura* aura = summon->GetAura(138616))
                                    _heal += aura->GetEffect(1)->GetAmount();
                            }
                        }
                    }
                }
                SetHitHeal(int32(_heal / GetSpell()->GetTargetCount()));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_wild_mushroom_bloom_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_wild_mushroom_bloom_heal_SpellScript();
        }
};

// Fortifying brew - 126456
class spell_dru_fortifying_brew : public SpellScriptLoader
{
    public:
        spell_dru_fortifying_brew() : SpellScriptLoader("spell_dru_fortifying_brew") {}

        class spell_dru_fortifying_brew_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_fortifying_brew_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                int32 perc = GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster);;
                amount += caster->CountPctFromCurHealth(perc);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_fortifying_brew_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_fortifying_brew_AuraScript();
        }
};

// Mangle - 93622
class spell_dru_mangle : public SpellScriptLoader
{
    public:
        spell_dru_mangle() : SpellScriptLoader("spell_dru_mangle") { }

        class spell_dru_mangle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_mangle_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->RemoveSpellCooldown(33878, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_mangle_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_mangle_SpellScript();
        }
};

// 34299 - Leader of the Pack
class spell_dru_leader_of_the_pack : public SpellScriptLoader
{
    public:
        spell_dru_leader_of_the_pack() : SpellScriptLoader("spell_dru_leader_of_the_pack") { }

        class spell_dru_leader_of_the_pack_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_leader_of_the_pack_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    SetHitHeal(int32(CalculatePct(caster->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_0].BasePoints)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dru_leader_of_the_pack_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_leader_of_the_pack_SpellScript();
        }
};

// 33745 - Lacerate
class spell_dru_lacerate : public SpellScriptLoader
{
    public:
        spell_dru_lacerate() : SpellScriptLoader("spell_dru_lacerate") { }

        class spell_dru_lacerateAuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lacerateAuraScript);

            void OnStackChange(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();
                if (caster && target)
                    if(GetStackAmount() >= 3 && !target->HasAura(158790))
                        caster->CastSpell(target, 158790, true);
            }

            // function registering
            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_lacerateAuraScript::OnStackChange, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        // function which creates AuraScript
        AuraScript* GetAuraScript() const
        {
            return new spell_dru_lacerateAuraScript();
        }
};

void AddSC_druid_spell_scripts()
{
    new spell_dru_play_death();
    new spell_dru_moonfire_sunfire();
    new spell_dru_solar_beam();
    new spell_dru_rip_duration();
    new spell_dru_savage_defense();
    new spell_dru_bear_form();
    new spell_dru_ferocious_bite();
    new spell_dru_rip();
    new spell_dru_bear_hug();
    new spell_dru_lifebloom();
    new spell_dru_omen_of_clarity();
    new spell_dru_mark_of_the_wild();
    new spell_dru_glyph_of_regrowth();
    new spell_dru_cat_form();
    new spell_dru_skull_bash();
    new spell_dru_faerie_swarm();
    new spell_dru_wild_mushroom_bloom();
    new spell_dru_wild_mushroom_detonate();
    new spell_dru_astral_communion();
    new spell_dru_celestial_alignment();
    new spell_dru_frenzied_regeneration();
    new spell_dru_stampeding_roar_speed();
    new spell_dru_faerie_fire();
    new spell_dru_teleport_moonglade();
    new spell_dru_growl();
    new spell_dru_prowl();
    new spell_dru_eclipse();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_swift_flight_passive();
    new spell_dru_starfall_dummy();
    new spell_dru_savage_roar();
    new spell_dru_survival_instincts();
    new spell_druid_rejuvenation();
    new spell_dru_incarnation();
    new spell_dru_incarnation_tree_of_life();
    new spell_druid_barkskin();
    new spell_druid_glyph_of_the_treant();
    new spell_druid_heart_of_the_wild();
    new spell_druid_eclipse_buff();
    new spell_druid_berserk_tiger_fury_buff();
    new spell_dru_a12_4p_feral_bonus();
    new spell_dru_swipe();
    new spell_dru_tooth_and_claw();
    new spell_dru_spell_dru_tooth_and_claw_absorb();
    new spell_dru_healing_ouch();
    new spell_dru_wild_mushroom_heal();
    new spell_dru_genesis();
    new spell_dru_glyph_of_hurricane();
    new spell_dru_efflorescence_dumy();
    new spell_druid_dream_of_cenarius();
    new spell_dru_frenzied_regeneration_t16();
    new spell_dru_ursocs_vigor();
    new spell_dru_one_with_nature();
    new spell_dru_wild_mushroom_bloom_heal();
    new spell_dru_fortifying_brew();
    new spell_dru_mangle();
    new spell_dru_leader_of_the_pack();
    new spell_dru_lacerate();
}
