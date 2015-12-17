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
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum DeathKnightSpells
{
    DK_SPELL_RUNIC_POWER_ENERGIZE               = 49088,
    DK_SPELL_ANTI_MAGIC_SHELL_TALENT            = 51052,
    DK_SPELL_CORPSE_EXPLOSION_TRIGGERED         = 43999,
    DK_SPELL_CORPSE_EXPLOSION_VISUAL            = 51270,
    DK_SPELL_GHOUL_EXPLODE                      = 47496,
    DK_SPELL_SCOURGE_STRIKE_TRIGGERED           = 70890,
    DK_SPELL_BLOOD_BOIL_TRIGGERED               = 65658,
    SPELL_DK_ITEM_T8_MELEE_4P_BONUS             = 64736,
    DK_SPELL_MASTERY_DREADBLADE                 = 77515,
    DK_SPELL_BLOOD_PLAGUE                       = 55078,
    DK_SPELL_FROST_FEVER                        = 55095,
    DK_SPELL_MASTER_OF_GHOULS                   = 52143,
    DK_SPELL_GHOUL_AS_GUARDIAN                  = 46585,
    DK_SPELL_GHOUL_AS_PET                       = 52150,
    DK_SPELL_ROILING_BLOOD                      = 108170,
    DK_SPELL_PESTILENCE                         = 50842,
    DK_SPELL_PLAGUE_LEECH                       = 123693,
    DK_SPELL_PERDITION                          = 123981,
    DK_SPELL_SHROUD_OF_PURGATORY                = 116888,
    DK_SPELL_PURGATORY_INSTAKILL                = 123982,
    DK_SPELL_BLOOD_RITES                        = 50034,
    DK_SPELL_DEATH_SIPHON_HEAL                  = 116783,
    DK_SPELL_BLOOD_CHARGE                       = 114851,
    DK_SPELL_PILLAR_OF_FROST                    = 51271,
    DK_SPELL_SOUL_REAPER_HASTE                  = 114868,
    DK_SPELL_SOUL_REAPER_DAMAGE                 = 114867,
    DK_SPELL_REMORSELESS_WINTER_STUN            = 115001,
    DK_SPELL_REMORSELESS_WINTER                 = 115000,
    DK_SPELL_CONVERSION							= 119975,
    DK_SPELL_WEAKENED_BLOWS                     = 115798,
    DK_SPELL_SCARLET_FEVER                      = 81132,
    DK_SPELL_SCENT_OF_BLOOD_AURA                = 50421,
    DK_SPELL_CHAINS_OF_ICE                      = 45524,
    DK_SPELL_EBON_PLAGUEBRINGER                 = 51160,
    DK_SPELL_REAPING                            = 56835,
    DK_SPELL_DESECRATED_GROUND                  = 118009,
    DK_SPELL_DESECRATED_GROUND_IMMUNE           = 115018,
    DK_SPELL_ASPHYXIATE                         = 108194,
    DK_SPELL_DARK_INFUSION_STACKS               = 91342,
    DK_SPELL_DARK_INFUSION_AURA                 = 93426,
    DK_NPC_WILD_MUSHROOM                        = 59172,
};

// Might of Ursoc - 113072
class spell_dk_might_of_ursoc : public SpellScriptLoader
{
    public:
        spell_dk_might_of_ursoc() : SpellScriptLoader("spell_dk_might_of_ursoc") { }

        class spell_dk_might_of_ursoc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_might_of_ursoc_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->GetHealthPct() < 15.0f)
                        caster->SetHealth(caster->CountPctFromMaxHealth(15));
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_might_of_ursoc_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_might_of_ursoc_AuraScript();
        }
};

// Dark transformation - transform pet spell - 63560
class spell_dk_dark_transformation_form : public SpellScriptLoader
{
    public:
        spell_dk_dark_transformation_form() : SpellScriptLoader("spell_dk_dark_transformation_form") { }

        class spell_dk_dark_transformation_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_dark_transformation_form_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* pet = GetHitUnit())
                        if (pet->HasAura(DK_SPELL_DARK_INFUSION_STACKS))
                        {
                            pet->RemoveAura(DK_SPELL_DARK_INFUSION_STACKS);
                            _player->RemoveAura(DK_SPELL_DARK_INFUSION_STACKS);
                        }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_dark_transformation_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_dark_transformation_form_SpellScript();
        }
};

// Called by Strangulate - 47476
// Asphyxiate - 108194
class spell_dk_asphyxiate : public SpellScriptLoader
{
    public:
        spell_dk_asphyxiate() : SpellScriptLoader("spell_dk_asphyxiate") { }

        class spell_dk_asphyxiate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_asphyxiate_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasSpell(DK_SPELL_ASPHYXIATE))
                        {
                            if (target->HasAura(GetSpellInfo()->Id, _player->GetGUID()))
                            {
                                target->RemoveAura(GetSpellInfo()->Id, _player->GetGUID());
                                _player->CastSpell(target, DK_SPELL_ASPHYXIATE, true);
                            }
                        }
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_asphyxiate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_asphyxiate_SpellScript();
        }
};

// Desecrated ground - 118009
class spell_dk_desecrated_ground : public SpellScriptLoader
{
    public:
        spell_dk_desecrated_ground() : SpellScriptLoader("spell_dk_desecrated_ground") { }

        class spell_dk_desecrated_ground_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_desecrated_ground_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if(Unit* caster = GetCaster())
                    if (DynamicObject* dynObj = caster->GetDynObject(DK_SPELL_DESECRATED_GROUND))
                        if (caster->GetDistance(dynObj) <= 8.0f)
                            caster->CastSpell(caster, DK_SPELL_DESECRATED_GROUND_IMMUNE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_desecrated_ground_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_desecrated_ground_AuraScript();
        }
};

// Necrotic Strike - 73975
class spell_dk_necrotic_strike : public SpellScriptLoader
{
    public:
        spell_dk_necrotic_strike() : SpellScriptLoader("spell_dk_necrotic_strike") { }

        class spell_dk_necrotic_strike_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_necrotic_strike_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    amount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), 225);
                    amount += aurEff->GetOldBaseAmount();
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_necrotic_strike_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_necrotic_strike_AuraScript();
        }
};

// Festering Strike - 85948
class spell_dk_festering_strike : public SpellScriptLoader
{
    public:
        spell_dk_festering_strike() : SpellScriptLoader("spell_dk_festering_strike") { }

        class spell_dk_festering_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_festering_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* BP = target->GetAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID()))
                        {
                            uint32 dur = BP->GetDuration() + 6000;
                            BP->SetDuration(dur);
                        }
                        if (Aura* FF = target->GetAura(DK_SPELL_FROST_FEVER, _player->GetGUID()))
                        {
                            uint32 dur = FF->GetDuration() + 6000;
                            FF->SetDuration(dur);
                        }
                        if (Aura* PV = target->GetAura(81326, _player->GetGUID()))
                        {
                            uint32 dur = PV->GetDuration() + 6000;
                            PV->SetDuration(dur);
                        }
                        if (Aura* COI = target->GetAura(DK_SPELL_CHAINS_OF_ICE, _player->GetGUID()))
                        {
                            uint32 dur = COI->GetDuration() + 6000;
                            COI->SetDuration(dur);
                        }
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_festering_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_festering_strike_SpellScript();
        }
};

// Howling Blast - 49184
class spell_dk_howling_blast : public SpellScriptLoader
{
    public:
        spell_dk_howling_blast() : SpellScriptLoader("spell_dk_howling_blast") { }

        class spell_dk_howling_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_howling_blast_SpellScript);

            ObjectGuid tar;

            void HandleBeforeCast()
            {
                Unit* target = GetExplTargetUnit();
                Unit* caster = GetCaster();

                if (!caster || !target)
                    return;

                tar = target->GetGUID();
            }

            void HandleOnHit()
            {
                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();

                if (!caster || !target || !tar)
                    return;

                if (target->GetGUID() != tar)
                    SetHitDamage(GetHitDamage()/2);

                caster->CastSpell(target, DK_SPELL_FROST_FEVER, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dk_howling_blast_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dk_howling_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_howling_blast_SpellScript();
        }
};

// Remorseless Winter - 115000
class spell_dk_remorseless_winter : public SpellScriptLoader
{
    public:
        spell_dk_remorseless_winter() : SpellScriptLoader("spell_dk_remorseless_winter") { }

        class spell_dk_remorseless_winter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_remorseless_winter_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (Aura* remorselessWinter = target->GetAura(DK_SPELL_REMORSELESS_WINTER))
                            if (remorselessWinter->GetStackAmount() == 5 && !target->HasAura(DK_SPELL_REMORSELESS_WINTER_STUN))
                                _player->CastSpell(target, DK_SPELL_REMORSELESS_WINTER_STUN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_remorseless_winter_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_remorseless_winter_SpellScript();
        }
};

// Soul Reaper - 130736 (unholy) or 130735 (frost) or 114866 (blood)
class spell_dk_soul_reaper : public SpellScriptLoader
{
    public:
        spell_dk_soul_reaper() : SpellScriptLoader("spell_dk_soul_reaper") { }

        class spell_dk_soul_reaper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        if (target->GetHealthPct() < 35.0f || (caster->HasAura(138347) && target->GetHealthPct() < 45.0f) || (caster->HasAura(157342) && target->GetHealthPct() < 45.0f))
                            caster->CastSpell(target, DK_SPELL_SOUL_REAPER_DAMAGE, true);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_DEATH && GetDuration() > 1000)
                        caster->CastSpell(caster, DK_SPELL_SOUL_REAPER_HASTE, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_soul_reaper_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectApplyFn(spell_dk_soul_reaper_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_soul_reaper_AuraScript();
        }
};

// Pillar of Frost - 51271
class spell_dk_pillar_of_frost : public SpellScriptLoader
{
    public:
        spell_dk_pillar_of_frost() : SpellScriptLoader("spell_dk_pillar_of_frost") { }

        class spell_dk_pillar_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_pillar_of_frost_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_INCAPACITATE, false);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_INCAPACITATE, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_pillar_of_frost_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_pillar_of_frost_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_pillar_of_frost_AuraScript();
        }
};

// Blood Tap - 45529
class spell_dk_blood_tap : public SpellScriptLoader
{
    public:
        spell_dk_blood_tap() : SpellScriptLoader("spell_dk_blood_tap") { }

        class spell_dk_blood_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_tap_SpellScript);

            SpellCastResult CheckBloodCharges()
            {
                if (GetCaster())
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* bloodCharges = _player->GetAura(DK_SPELL_BLOOD_CHARGE))
                    {
                        if (bloodCharges->GetStackAmount() < 5)
                        {
                            Spell::SendCastResult(_player, GetSpellInfo(), SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_INSUFFICIENT_BLOOD_CHARGES);
                            return SPELL_FAILED_DONT_REPORT;
                        }
                    }
                    else
                    {
                        Spell::SendCastResult(_player, GetSpellInfo(), SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_INSUFFICIENT_BLOOD_CHARGES);
                        return SPELL_FAILED_DONT_REPORT;
                    }


                    int32 cooldown = 0;
                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                        if (_player->GetCurrentRune(i) != RUNE_DEATH && _player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN)
                            cooldown++;

                    if (!cooldown)
                    {
                        Spell::SendCastResult(_player, GetSpellInfo(), SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_NO_DEPLETED_RUNES);
                        return SPELL_FAILED_DONT_REPORT;
                    }
                }

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* bloodCharges = _player->GetAura(DK_SPELL_BLOOD_CHARGE))
                        {
                            int32 newAmount = bloodCharges->GetStackAmount();

                            if ((newAmount - 5) <= 0)
                                _player->RemoveAura(DK_SPELL_BLOOD_CHARGE);
                            else
                                bloodCharges->SetStackAmount(newAmount - 5);
                        }

                        int32 runesRestor = 0;
                        for (int i = 0; i < MAX_RUNES ; i++)
                        {
                            if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN && runesRestor < 1 && _player->GetCurrentRune(i) != RUNE_DEATH)
                            {
                                runesRestor++;
                                _player->SetRuneCooldown(i, 0);
                                _player->SetConvertIn(i, RUNE_DEATH);
                                _player->ConvertRune(i, RUNE_DEATH);
                                _player->AddRunePower(i);
                            }
                        }
                        /*if(runesRestor < 1)
                        {
                            for (int i = 0; i < MAX_RUNES ; i++)
                            {
                                if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN && runesRestor < 1)
                                {
                                    runesRestor++;
                                    _player->SetRuneCooldown(i, 0);
                                    _player->AddRunePower(i);
                                }
                            }
                        }*/
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_blood_tap_SpellScript::CheckBloodCharges);
                OnHit += SpellHitFn(spell_dk_blood_tap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_tap_SpellScript();
        }
};

// Death Siphon - 108196
class spell_dk_death_siphon : public SpellScriptLoader
{
    public:
        spell_dk_death_siphon() : SpellScriptLoader("spell_dk_death_siphon") { }

        class spell_dk_death_siphon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_siphon_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 bp = CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_1].CalcValue(_player));
                        _player->CastCustomSpell(_player, DK_SPELL_DEATH_SIPHON_HEAL, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_dk_death_siphon_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_siphon_SpellScript();
        }
};

// Purgatory - 116888
class spell_dk_purgatory : public SpellScriptLoader
{
    public:
        spell_dk_purgatory() : SpellScriptLoader("spell_dk_purgatory") { }

        class spell_dk_purgatory_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                        _player->CastSpell(_player, DK_SPELL_PURGATORY_INSTAKILL, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_purgatory_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_AuraScript();
        }
};

// Purgatory - 114556
class spell_dk_purgatory_absorb : public SpellScriptLoader
{
    public:
        spell_dk_purgatory_absorb() : SpellScriptLoader("spell_dk_purgatory_absorb") { }

        class spell_dk_purgatory_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_absorb_AuraScript);

            void CalculateAmount(AuraEffect const* /*AuraEffect**/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                // No damage received under Shroud of Purgatory
                if (target->ToPlayer()->HasAura(DK_SPELL_SHROUD_OF_PURGATORY))
                {
                    absorbAmount = dmgInfo.GetDamage();
                    return;
                }

                if (target->ToPlayer()->HasAura(DK_SPELL_PERDITION))
                    return;

                int32 bp = dmgInfo.GetDamage();

                target->CastCustomSpell(target, DK_SPELL_SHROUD_OF_PURGATORY, &bp, NULL, NULL, true);
                target->CastSpell(target, DK_SPELL_PERDITION, true);
                target->SetHealth(1);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_purgatory_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_purgatory_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_absorb_AuraScript();
        }
};

// Plague Leech - 123693
class spell_dk_plague_leech : public SpellScriptLoader
{
    public:
        spell_dk_plague_leech() : SpellScriptLoader("spell_dk_plague_leech") { }

        class spell_dk_plague_leech_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plague_leech_SpellScript);

            RuneType GetRuneBan(Player* plr)
            {
                switch (plr->GetSpecializationId(plr->GetActiveSpec()))
                {
                    case SPEC_DK_BLOOD:
                    case SPEC_DK_FROST:  return RUNE_BLOOD;
                    case SPEC_DK_UNHOLY: return RUNE_UNHOLY;
                    default:
                        return NUM_RUNE_TYPES;
                }
            }

            SpellCastResult CheckRunes()
            {
                int32 runesUsed = 0;
                
                if (GetCaster())
                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        RuneType runesBan = GetRuneBan(_player);

                        for (uint8 i = 0; i < MAX_RUNES; ++i)
                            if (_player->GetBaseRune(i) != runesBan)
                                if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN)
                                    runesUsed++;

                        if (!runesUsed)
                        {
                            Spell::SendCastResult(_player, GetSpellInfo(), SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_NO_DEPLETED_RUNES);
                            return SPELL_FAILED_DONT_REPORT;
                        }

                        if (Unit* target = GetExplTargetUnit())
                        {
                            if (!target->HasAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID()) || !target->HasAura(DK_SPELL_FROST_FEVER, _player->GetGUID()))
                            {
                                Spell::SendCastResult(_player, GetSpellInfo(), SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_FEVER_PLAGUE_MUST_BE_PRESENT);
                                return SPELL_FAILED_DONT_REPORT;
                            }
                        }
                        return SPELL_CAST_OK;
                    }

                return SPELL_FAILED_DONT_REPORT;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        target->RemoveAura(DK_SPELL_FROST_FEVER, _player->GetGUID());
                        target->RemoveAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID());

                        int32 runesRestor = 0;
                        RuneType runesBan = GetRuneBan(_player);

                        for (int i = 0; i < MAX_RUNES ; i++)
                        {
                            if (_player->GetBaseRune(i) == runesBan)
                                continue;

                            if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN && runesRestor < 2)
                            {
                                runesRestor++;
                                _player->SetRuneCooldown(i, 0);
                                _player->AddRunePower(i);
                                if (_player->GetCurrentRune(i) != RUNE_DEATH)
                                {
                                    _player->SetConvertIn(i, RUNE_DEATH);
                                    _player->ConvertRune(i, RUNE_DEATH);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_plague_leech_SpellScript::CheckRunes);
                OnHit += SpellHitFn(spell_dk_plague_leech_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plague_leech_SpellScript();
        }
};

// Outbreak - 77575
class spell_dk_outbreak : public SpellScriptLoader
{
    public:
        spell_dk_outbreak() : SpellScriptLoader("spell_dk_outbreak") { }

        class spell_dk_outbreak_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_outbreak_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, DK_SPELL_BLOOD_PLAGUE, true);
                        _player->CastSpell(target, DK_SPELL_FROST_FEVER, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_outbreak_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_outbreak_SpellScript();
        }
};

// 50462 - Anti-Magic Shell (on raid member)
class spell_dk_anti_magic_shell_raid : public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_raid() : SpellScriptLoader("spell_dk_anti_magic_shell_raid") { }

        class spell_dk_anti_magic_shell_raid_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_raid_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // TODO: this should absorb limited amount of damage, but no info on calculation formula
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_raid_AuraScript();
        }
};

// 48707 - Anti-Magic Shell (on self)
class spell_dk_anti_magic_shell_self : public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_self() : SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

        class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);

            uint32 absorbPct, container;
            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                container = 0;
                return true;
            }

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_RUNIC_POWER_ENERGIZE))
                    return false;
                return true;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                absorbAmount = std::min(CalculatePct(dmgInfo.GetDamage(), absorbPct), uint32(aurEff->GetAmount()));
            }

            void Trigger(AuraEffect* aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
            {
                if (Unit* target = GetTarget()) // AMS generates 2 Runic Power for every percent of maximum health absorbed
                {
                    uint32 RPCap = target->CountPctFromMaxHealth(1) / 2;
                    int32 bp = int32((container + absorbAmount) / RPCap);
                    container = (container + absorbAmount) - (bp * RPCap);
                    bp *= 10;

                    if (bp >= 10)
                        target->CastCustomSpell(target, DK_SPELL_RUNIC_POWER_ENERGIZE, &bp, NULL, NULL, true, NULL, aurEff);
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE && player->HasAura(146648)) // Glyph of Regenerative Magic
                    {
                        double cooldown = player->GetSpellCooldownDelay(GetSpellInfo()->Id);
                        int32 percent = int32(float(aurEff->GetAmount() * 100.0f) / player->GetMaxHealth());
                        percent = percent < 50 ? percent : 50;
                        cooldown = (cooldown * percent) / 100;
                        player->ModifySpellCooldown(GetSpellInfo()->Id, -(cooldown * IN_MILLISECONDS));
                    }
                }
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_anti_magic_shell_self_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_self_AuraScript();
        }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone : public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_zone() : SpellScriptLoader("spell_dk_anti_magic_zone") { }

        class spell_dk_anti_magic_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_zone_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_ANTI_MAGIC_SHELL_TALENT))
                    return false;
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                SpellInfo const* talentSpell = sSpellMgr->GetSpellInfo(DK_SPELL_ANTI_MAGIC_SHELL_TALENT);
                amount = 136800;
                if (Player* player = GetCaster()->ToPlayer())
                     amount += int32(player->GetStat(STAT_STRENGTH) * 4);
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_zone_AuraScript();
        }
};

// 47496 - Explode, Ghoul spell for Corpse Explosion
class spell_dk_ghoul_explode : public SpellScriptLoader
{
    public:
        spell_dk_ghoul_explode() : SpellScriptLoader("spell_dk_ghoul_explode") { }

        class spell_dk_ghoul_explode_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_ghoul_explode_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_CORPSE_EXPLOSION_TRIGGERED))
                    return false;
                return true;
            }

            void Suicide(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    // Corpse Explosion (Suicide)
                    unitTarget->CastSpell(unitTarget, DK_SPELL_CORPSE_EXPLOSION_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_ghoul_explode_SpellScript::Suicide, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_ghoul_explode_SpellScript();
        }
};

// Death Gate - 53822
class spell_dk_death_gate_teleport : public SpellScriptLoader
{
    public:
        spell_dk_death_gate_teleport() : SpellScriptLoader("spell_dk_death_gate_teleport") {}

        class spell_dk_death_gate_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_teleport_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }
                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if(_player->GetZoneId() == 139)
                        _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, 0.0f);
                    else
                        _player->TeleportTo(0, 2355.23f, -5666.4433f, 426.028f, _player->GetOrientation());
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_teleport_SpellScript::CheckClass);
                AfterCast += SpellCastFn(spell_dk_death_gate_teleport_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_teleport_SpellScript();
        }
};

// Death Gate - 52751
class spell_dk_death_gate : public SpellScriptLoader
{
    public:
        spell_dk_death_gate() : SpellScriptLoader("spell_dk_death_gate") {}

        class spell_dk_death_gate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, GetEffectValue(), false);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_SpellScript();
        }
};

// Death Pact - 48743
class spell_dk_death_pact : public SpellScriptLoader
{
    public:
        spell_dk_death_pact() : SpellScriptLoader("spell_dk_death_pact") { }

        class spell_dk_death_pact_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_pact_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    amount = CalculatePct(caster->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                    amount += aurEff->GetOldBaseAmount();
                }
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (SpellInfo const* spellInfo = dmgInfo.GetSpellInfo())
                    if(spellInfo->Id == GetSpellInfo()->Id)
                        absorbAmount = 0;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_pact_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_HEAL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_death_pact_AuraScript::Absorb, EFFECT_1, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_pact_AuraScript();
        }
};

// Blood Boil - 50842
class spell_dk_blood_boil : public SpellScriptLoader
{
    public:
        spell_dk_blood_boil() : SpellScriptLoader("spell_dk_blood_boil") { }

        class spell_dk_blood_boil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_boil_SpellScript);

            int32 duration = 0;
            int32 durationBlood = 0;
            int32 durationFrost = 0;
            int32 maxDuration = 0;
            int32 maxDurationBlood = 0;
            int32 maxDurationFrost = 0;
            int32 stack = 0;
            bool maxDur = false;

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* aura = target->GetAura(DK_SPELL_BLOOD_PLAGUE, caster->GetGUID()))
                        {
                            if (maxDur)
                                aura->SetDuration(aura->GetMaxDuration());
                            else if (aura->GetDuration() < durationBlood)
                                aura->SetDuration(durationBlood);
                        }
                        else if(durationBlood)
                        {
                            if(Aura* newAura = caster->AddAura(DK_SPELL_BLOOD_PLAGUE, target))
                            if (!maxDur)
                            {
                                newAura->SetDuration(durationBlood);
                                newAura->SetMaxDuration(maxDurationBlood);
                            }
                        }
                            

                        if (Aura* aura = target->GetAura(DK_SPELL_FROST_FEVER, caster->GetGUID()))
                        {
                            if (maxDur)
                                aura->SetDuration(aura->GetMaxDuration());
                            else if (aura->GetDuration() < durationFrost)
                                aura->SetDuration(durationFrost);
                        }
                        else if(durationFrost)
                        {
                            if(Aura* newAura = caster->AddAura(DK_SPELL_FROST_FEVER, target))
                            if (!maxDur)
                            {
                                newAura->SetDuration(durationFrost);
                                newAura->SetMaxDuration(maxDurationFrost);
                            }
                        }

                        if (Aura* aura = target->GetAura(155159, caster->GetGUID()))
                        {
                            if (maxDur)
                                aura->SetDuration(aura->GetMaxDuration());
                            else if (aura->GetDuration() < duration)
                                aura->SetDuration(duration);
                            if (aura->GetStackAmount() < stack)
                                aura->SetStackAmount(stack);
                        }
                        else if(duration)
                        {
                            if(Aura* newAura = caster->AddAura(155159, target))
                            {
                                if (!maxDur)
                                {
                                    newAura->SetDuration(duration);
                                    newAura->SetMaxDuration(maxDuration);
                                }
                                newAura->SetStackAmount(stack);
                            }
                        }
                    }
                }
            }

            void HandleOnCast()
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                maxDur = bool(caster->HasAura(49509));

                Unit* targetExpl = GetExplTargetUnit();
                if (!targetExpl && caster->ToPlayer())
                    targetExpl = caster->ToPlayer()->GetSelectedUnit();
                if(targetExpl)
                {
                    if (Aura* aura = targetExpl->GetAura(DK_SPELL_BLOOD_PLAGUE, caster->GetGUID()))
                    {
                        durationBlood = aura->GetDuration();
                        maxDurationBlood = aura->GetMaxDuration();
                    }
                    if (Aura* aura = targetExpl->GetAura(DK_SPELL_FROST_FEVER, caster->GetGUID()))
                    {
                        durationFrost = aura->GetDuration();
                        maxDurationFrost = aura->GetMaxDuration();
                    }
                    if (Aura* aura = targetExpl->GetAura(155159, caster->GetGUID()))
                    {
                        duration = aura->GetDuration();
                        stack = aura->GetStackAmount();
                        maxDuration = aura->GetMaxDuration();
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_blood_boil_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnCast += SpellCastFn(spell_dk_blood_boil_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_boil_SpellScript();
        }
};

// Death Grip - 49560
class spell_dk_death_grip : public SpellScriptLoader
{
    public:
        spell_dk_death_grip() : SpellScriptLoader("spell_dk_death_grip") { }

        class spell_dk_death_grip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_grip_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 damage = GetEffectValue();
                Position const* pos = GetExplTargetDest();
                if (Unit* target = GetHitUnit())
                {
                    if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS)) // Deterrence
                        target->CastSpell(pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), damage, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_grip_SpellScript();
        }
};

// Death Grip - 49576
class spell_dk_death_grip_dummy : public SpellScriptLoader
{
    public:
        spell_dk_death_grip_dummy() : SpellScriptLoader("spell_dk_death_grip_dummy") { }

        class spell_dk_death_grip_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_grip_dummy_SpellScript);

            SpellCastResult CheckCast()
            {
                if(Unit* caster = GetCaster())
                    if (caster->GetUnitMovementFlags() & MOVEMENTFLAG_FALLING)
                        return SPELL_FAILED_ERROR;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                {
                    if(target->IsWithinMeleeRange(caster))
                        caster->CastSpell(target, 51399, false);
                    else
                        caster->CastSpell(target, 49560, false);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_grip_dummy_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_grip_dummy_SpellScript();
        }
};

// Gorefiend's Grasp - 108199
class spell_dk_gorefiends_grasp : public SpellScriptLoader
{
    public:
        spell_dk_gorefiends_grasp() : SpellScriptLoader("spell_dk_gorefiends_grasp") { }

        class spell_dk_gorefiends_grasp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_gorefiends_grasp_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetExplTargetUnit();
                Unit* target = GetHitUnit();
                if(caster && target && caster != target)
                {
                    Position const* pos = GetExplTargetDest();
                    caster->CastSpell(target, 114869, true);
                    target->CastSpell(pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), 146599, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_gorefiends_grasp_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_gorefiends_grasp_SpellScript();
        }
};

// Presence - 48266,48265,48263
class spell_dk_presence : public SpellScriptLoader
{
    public:
        spell_dk_presence() : SpellScriptLoader("spell_dk_presence") { }

        class spell_dk_presence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_presence_SpellScript);

            void HandleOnHit()
            {
                if(Unit* caster = GetCaster())
                {
                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        int32 power = _player->GetPower(POWER_RUNIC_POWER);
                        if(caster->HasAura(58647)) // Glyph of Shifting Presences
                            power -= int32(power * 0.7f);

                        _player->ModifyPower(POWER_RUNIC_POWER, -(power), true);
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_presence_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_presence_SpellScript();
        }
};

// Unholy Frenzy - 49016
class spell_dk_unholy_frenzy : public SpellScriptLoader
{
    public:
        spell_dk_unholy_frenzy() : SpellScriptLoader("spell_dk_unholy_frenzy") { }

        class spell_dk_unholy_frenzy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_unholy_frenzy_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if(caster->HasAura(58616))
                        amount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_unholy_frenzy_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_unholy_frenzy_AuraScript();
        }
};

// Corpse Explosion - 127344
class spell_dk_corpse_explosion : public SpellScriptLoader
{
    public:
        spell_dk_corpse_explosion() : SpellScriptLoader("spell_dk_corpse_explosion") {}

        class spell_dk_corpse_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_corpse_explosion_SpellScript);

            SpellCastResult CheckClass()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if(!target->isDead() || target->GetDisplayId() != target->GetNativeDisplayId())
                        return SPELL_FAILED_TARGET_NOT_DEAD;
                }
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_corpse_explosion_SpellScript::CheckClass);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_corpse_explosion_SpellScript();
        }
};

// Soul Reaper - 114868
class spell_dk_soul_reaper_speed : public SpellScriptLoader
{
    public:
        spell_dk_soul_reaper_speed() : SpellScriptLoader("spell_dk_soul_reaper_speed") { }

        class spell_dk_soul_reaper_speed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_speed_AuraScript);

            void CalculateAmount(AuraEffect const* /*AuraEffect**/, int32& amount, bool& /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if(!caster->HasAura(146645))
                        amount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_soul_reaper_speed_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_SPEED_ALWAYS);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_soul_reaper_speed_AuraScript();
        }
};

// Called by - 55050, 56815, 47541, 114866, 48721
// Item - Death Knight T16 Blood 2P Bonus - 144934
class spell_dk_bone_shield : public SpellScriptLoader
{
    public:
        spell_dk_bone_shield() : SpellScriptLoader("spell_dk_bone_shield") { }

        class spell_dk_bone_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_bone_shield_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Aura* setCast = caster->GetAura(144934))
                        setCast->SetScriptData(0, 1);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_bone_shield_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_bone_shield_SpellScript();
        }
};

// Item - Death Knight T16 Blood 2P Bonus - 144934
class spell_dk_blood_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dk_blood_2p_bonus() : SpellScriptLoader("spell_dk_blood_2p_bonus") { }

        class spell_dk_blood_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_blood_2p_bonus_AuraScript);

            uint32 castCount;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                castCount = 0;
            }

            void SetData(uint32 /*type*/, uint32 count)
            {
                castCount += count;
                if (castCount >= 10)
                {
                    castCount -= 10;

                    if (Unit* caster = GetCaster())
                        caster->CastSpell(caster, 144948, true);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_blood_2p_bonus_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_blood_2p_bonus_AuraScript();
        }
};

// Called by - 49028
// Item - Death Knight T16 Blood 4P Bonus - 144950
class spell_dk_dancing_rune_weapon : public SpellScriptLoader
{
    public:
        spell_dk_dancing_rune_weapon() : SpellScriptLoader("spell_dk_dancing_rune_weapon") { }

        class spell_dk_dancing_rune_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_dancing_rune_weapon_SpellScript);

            void HandleOnCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144950))
                    {
                        for (int i = 0; i < MAX_RUNES ; i++)
                        {
                            if (_player->GetCurrentRune(i) == RUNE_DEATH)
                                continue;

                            if (_player->GetCurrentRune(i) == RUNE_FROST || _player->GetCurrentRune(i) == RUNE_UNHOLY)
                            {
                                _player->SetConvertIn(i, RUNE_DEATH);
                                _player->ConvertRune(i, RUNE_DEATH);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_dk_dancing_rune_weapon_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_dancing_rune_weapon_SpellScript();
        }
};

// Death Shroud - 144901
class spell_dk_death_shroud : public SpellScriptLoader
{
    public:
        spell_dk_death_shroud() : SpellScriptLoader("spell_dk_death_shroud") { }

        class spell_dk_death_shroud_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_shroud_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    uint32 mastery = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_MASTERY);
                    uint32 haste = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_HASTE_MELEE);

                    if(haste < mastery)
                        amount = 0;
                }
            }

            void CalculateAmount1(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    uint32 mastery = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_MASTERY);
                    uint32 haste = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_HASTE_MELEE);

                    if(haste > mastery)
                        amount = 0;
                }
            }

            void CalculateMaxDuration(int32 & duration)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    if (_player->HasSpell(63560))
                        duration = 15000;
                    else if(_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DK_FROST)
                        duration = 4000 + (_player->IsTwoHandUsed() ? 4000 : 2000);
                }
            }

            void Register()
            {
                DoCalcMaxDuration += AuraCalcMaxDurationFn(spell_dk_death_shroud_AuraScript::CalculateMaxDuration);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_shroud_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_shroud_AuraScript::CalculateAmount1, EFFECT_1, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_shroud_AuraScript();
        }
};

//Blood Gorged 81277
class spell_dk_blood_gorged : public SpellScriptLoader
{
public:
    spell_dk_blood_gorged() : SpellScriptLoader("spell_dk_blood_gorged") { }

    class spell_dk_blood_gorged_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_blood_gorged_AuraScript);

        void OnStackChange(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if(!target)
                return;

            if (Unit* owner = target->GetOwner())
            {
                int32 stacks = GetStackAmount() * 10;
                int32 masterPct = int32(100.0f - owner->GetHealthPct());
                AddPct(stacks, masterPct);
                if (roll_chance_i(stacks))
                {
                    int32 damage = GetStackAmount() * target->GetMaxHealth() * GetSpellInfo()->Effects[EFFECT_1].BasePoints / 100.0f;
                    target->CastCustomSpell(target, 81280, &damage, NULL, NULL, true);
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dk_blood_gorged_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_blood_gorged_AuraScript();
    }
};

//Necrotic Plague - 155159
class spell_dk_necrotic_plague : public SpellScriptLoader
{
public:
    spell_dk_necrotic_plague() : SpellScriptLoader("spell_dk_necrotic_plague") { }

    class spell_dk_necrotic_plague_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_necrotic_plague_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if(!caster)
                return;
            Unit* target = GetTarget();
            if(!target)
                return;

            target->CastCustomSpell(target, 155163, NULL, NULL, NULL, true, NULL, aurEff, caster->GetGUID());

            std::list<Unit*> targets;
            Trinity::AnyUnfriendlyUnitInObjectRangeCheck u_check(target, caster, 8.0f);
            Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(target, targets, u_check);
            target->VisitNearbyObject(8.0f, searcher);

            // remove not LoS targets
            for (std::list<Unit*>::iterator tIter = targets.begin(); tIter != targets.end();)
            {
                if (!target->IsWithinLOSInMap(*tIter) || (*tIter)->isTotem() || (*tIter)->HasAura(155159) || !caster->_IsValidAttackTarget((*tIter), GetSpellInfo()))
                    targets.erase(tIter++);
                else
                    ++tIter;
            }

            // no appropriate targets
            if (targets.empty())
                return;

            // select random
            Unit* newTarget = Trinity::Containers::SelectRandomContainerElement(targets);
            target->CastCustomSpell(newTarget, 155163, NULL, NULL, NULL, true, NULL, aurEff, caster->GetGUID());
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_necrotic_plague_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_necrotic_plague_AuraScript();
    }
};

//Necrotic Plague - 155163
class spell_dk_necrotic_plague_cast : public SpellScriptLoader
{
    public:
        spell_dk_necrotic_plague_cast() : SpellScriptLoader("spell_dk_necrotic_plague_cast") { }

        class spell_dk_necrotic_plague_cast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_necrotic_plague_cast_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;
                Unit* target = GetHitUnit();
                if(!target)
                    return;
                Unit* originalCaster = GetOriginalCaster();

                if(!originalCaster || originalCaster == caster)
                {
                    if (Aura* aura = target->GetAura(155159, (originalCaster ? originalCaster : caster)->GetGUID()))
                        aura->ModStackAmount(1);
                    else
                        caster->CastSpell(target, 155159, true);
                }
                else if (originalCaster)
                {
                    if (Aura* aura = caster->GetAura(155159, originalCaster->GetGUID()))
                        if(Aura* newAura = originalCaster->AddAura(155159, target))
                        {
                            newAura->SetDuration(aura->GetDuration());
                            newAura->SetMaxDuration(aura->GetDuration());
                        }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_necrotic_plague_cast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_necrotic_plague_cast_SpellScript();
        }
};

// 152280 - Defile
class spell_dk_defile : public SpellScriptLoader
{
    public:
        spell_dk_defile() : SpellScriptLoader("spell_dk_defile") { }

        class spell_dk_defile_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_defile_AuraScript);

            uint32 absorbPct;
            uint32 tickCount = 0;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_3].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                amount = 0;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                absorbAmount = 0;
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    if(_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DK_BLOOD)
                        absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
                }
            }

            void OnTick(AuraEffect const* aurEff)
            {
                if(Unit* caster = GetCaster())
                {
                    if (AreaTrigger* areaObj = caster->GetAreaObject(GetSpellInfo()->Id))
                    {
                        float scale = areaObj->GetFloatValue(AREATRIGGER_FIELD_BOUNDS_RADIUS_2_D) + 0.025f;
                        areaObj->SetFloatValue(AREATRIGGER_FIELD_BOUNDS_RADIUS_2_D, scale);
                        areaObj->CastAction();
                    }
                    GuidList* summonList = caster->GetSummonList(82521);
                    for (GuidList::const_iterator iter = summonList->begin(); iter != summonList->end(); ++iter)
                    {
                        if(Creature* summon = ObjectAccessor::GetCreature(*caster, (*iter)))
                        {
                            switch (tickCount)
                            {
                                case 0:
                                    summon->SendPlaySpellVisualKit(46617, 0);
                                    break;
                                case 1:
                                    summon->SendPlaySpellVisualKit(46618, 0);
                                    break;
                                case 2:
                                    summon->SendPlaySpellVisualKit(46619, 0);
                                    break;
                                case 3:
                                    summon->SendPlaySpellVisualKit(46629, 0);
                                    break;
                                case 4:
                                    summon->SendPlaySpellVisualKit(46630, 0);
                                    break;
                                case 5:
                                    summon->SendPlaySpellVisualKit(46631, 0);
                                    break;
                                case 6:
                                    summon->SendPlaySpellVisualKit(46632, 0);
                                    break;
                                case 7:
                                    summon->SendPlaySpellVisualKit(46633, 0);
                                    break;
                                case 8:
                                    summon->SendPlaySpellVisualKit(46634, 0);
                                    break;
                                case 9:
                                    summon->SendPlaySpellVisualKit(46635, 0);
                                    break;
                                case 10:
                                    summon->SendPlaySpellVisualKit(46643, 0);
                                    break;
                            }
                        }
                    }
                    tickCount++;
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_defile_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_defile_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_defile_AuraScript::Absorb, EFFECT_3, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_defile_AuraScript();
        }
};

// Breath of Sindragosa - 155166
class spell_dk_breath_of_sindragosa : public SpellScriptLoader
{
    public:
        spell_dk_breath_of_sindragosa() : SpellScriptLoader("spell_dk_breath_of_sindragosa") { }

        class spell_dk_breath_of_sindragosa_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_breath_of_sindragosa_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;
                Aura* aura = caster->GetAura(152279);
                if (!aura)
                    return;
                int32 dam = GetHitDamage();
                ObjectGuid mainTargetGuid = aura->GetEffectTargets().empty() ? ObjectGuid::Empty : aura->GetRndEffectTarget();
                Unit* mainTarget = NULL;
                if (mainTargetGuid)
                    mainTarget = ObjectAccessor::GetUnit(*caster, mainTargetGuid);
                if(!mainTarget)
                {
                    mainTarget = caster->getAttackerForHelper();
                    if (!mainTarget && caster->ToPlayer())
                    {
                        mainTarget = caster->ToPlayer()->GetSelectedUnit();
                        if(mainTarget && !caster->_IsValidAttackTarget(mainTarget, GetSpellInfo()))
                            mainTarget = NULL;
                    }
                    if (!mainTarget)
                        mainTarget = caster->SelectNearbyTarget(caster);
                }
                if(mainTarget && !mainTargetGuid)
                    aura->AddEffectTarget(mainTarget->GetGUID());
                if(target != mainTarget)
                    SetHitDamage(GetHitDamage()/2);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_breath_of_sindragosa_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_breath_of_sindragosa_SpellScript();
        }
};

// Defile Damage - 156000
class spell_dk_defile_damage : public SpellScriptLoader
{
    public:
        spell_dk_defile_damage() : SpellScriptLoader("spell_dk_defile_damage") { }

        class spell_dk_defile_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_defile_damage_SpellScript);

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;
                if (AreaTrigger* areaObj = caster->GetAreaObject(152280))
                    SetHitDamage(int32(GetHitDamage() * areaObj->GetFloatValue(AREATRIGGER_FIELD_BOUNDS_RADIUS_2_D)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_defile_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_defile_damage_SpellScript();
        }
};

// Rune Tap - 171049
class spell_dk_rune_tap : public SpellScriptLoader
{
    public:
        spell_dk_rune_tap() : SpellScriptLoader("spell_dk_rune_tap") { }

        class spell_dk_rune_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_rune_tap_SpellScript);

            int32 duration = 0;

            void HandleBeforeCast()
            {
                if(Unit* caster = GetCaster())
                    if(Aura* aura = caster->GetAura(171049))
                        duration = aura->GetDuration();
            }

            void HandleAfterCast()
            {
                if(Unit* caster = GetCaster())
                    if(Aura* aura = caster->GetAura(171049))
                        aura->SetDuration(aura->GetDuration() + duration);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dk_rune_tap_SpellScript::HandleBeforeCast);
                AfterCast += SpellCastFn(spell_dk_rune_tap_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_rune_tap_SpellScript();
        }
};

// Enhanced Death Coil - 164047
class spell_dk_enhanced_death_coil : public SpellScriptLoader
{
    public:
        spell_dk_enhanced_death_coil() : SpellScriptLoader("spell_dk_enhanced_death_coil") { }

        class spell_dk_enhanced_death_coil_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_enhanced_death_coil_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount += aurEff->GetOldBaseAmount();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_enhanced_death_coil_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_enhanced_death_coil_AuraScript();
        }
};

void AddSC_deathknight_spell_scripts()
{
    new spell_dk_might_of_ursoc();
    new spell_dk_dark_transformation_form();
    new spell_dk_asphyxiate();
    new spell_dk_desecrated_ground();
    new spell_dk_necrotic_strike();
    new spell_dk_festering_strike();
    new spell_dk_howling_blast();
    new spell_dk_remorseless_winter();
    new spell_dk_soul_reaper();
    new spell_dk_pillar_of_frost();
    new spell_dk_blood_tap();
    new spell_dk_death_siphon();
    new spell_dk_purgatory();
    new spell_dk_purgatory_absorb();
    new spell_dk_plague_leech();
    new spell_dk_outbreak();
    new spell_dk_anti_magic_shell_raid();
    new spell_dk_anti_magic_shell_self();
    new spell_dk_anti_magic_zone();
    new spell_dk_ghoul_explode();
    new spell_dk_death_gate_teleport();
    new spell_dk_death_gate();
    new spell_dk_death_pact();
    new spell_dk_blood_boil();
    new spell_dk_death_grip();
    new spell_dk_death_grip_dummy();
    new spell_dk_gorefiends_grasp();
    new spell_dk_presence();
    new spell_dk_unholy_frenzy();
    new spell_dk_corpse_explosion();
    new spell_dk_soul_reaper_speed();
    new spell_dk_bone_shield();
    new spell_dk_blood_2p_bonus();
    new spell_dk_dancing_rune_weapon();
    new spell_dk_death_shroud();
    new spell_dk_blood_gorged();
    new spell_dk_necrotic_plague();
    new spell_dk_necrotic_plague_cast();
    new spell_dk_defile();
    new spell_dk_breath_of_sindragosa();
    new spell_dk_defile_damage();
    new spell_dk_rune_tap();
    new spell_dk_enhanced_death_coil();
}