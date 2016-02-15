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
 * Scripts for spells with SPELLFAMILY_MONK and SPELLFAMILY_GENERIC spells used by monk players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_monk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum MonkSpells
{
    SPELL_MONK_LEGACY_OF_THE_EMPEROR            = 117667,
    SPELL_MONK_FORTIFYING_BREW                  = 120954,
    SPELL_MONK_PROVOKE                          = 116189,
    SPELL_MONK_PROVOKE_AOE                      = 118635,
    SPELL_MONK_BLACKOUT_KICK_DOT                = 128531,
    SPELL_MONK_BLACKOUT_KICK_HEAL               = 128591,
    SPELL_MONK_SHUFFLE                          = 115307,
    SPELL_MONK_ZEN_PILGRIMAGE                   = 126892,
    SPELL_MONK_ZEN_PILGRIMAGE_RETURN            = 126895,
    SPELL_MONK_DISABLE_ROOT                     = 116706,
    SPELL_MONK_DISABLE                          = 116095,
    SPELL_MONK_SOOTHING_MIST_VISUAL             = 117899,
    SPELL_MONK_SOOTHING_MIST_ENERGIZE           = 116335,
    SPELL_MONK_BREATH_OF_FIRE_DOT               = 123725,
    SPELL_MONK_BREATH_OF_FIRE_CONFUSED          = 123393,
    SPELL_MONK_ELUSIVE_BREW_STACKS              = 128939,
    SPELL_MONK_ELUSIVE_BREW                     = 115308,
    SPELL_MONK_KEG_SMASH_VISUAL                 = 123662,
    SPELL_MONK_KEG_SMASH_ENERGIZE               = 127796,
    SPELL_MONK_WEAKENED_BLOWS                   = 115798,
    SPELL_MONK_DIZZYING_HAZE                    = 116330,
    SPELL_MONK_CLASH_CHARGE_SELF                = 122235,
    SPELL_MONK_CLASH_CHARGE_TARGET              = 122252,
    SPELL_MONK_LIGHT_STAGGER                    = 124275,
    SPELL_MONK_MODERATE_STAGGER                 = 124274,
    SPELL_MONK_HEAVY_STAGGER                    = 124273,
    SPELL_MONK_ROLL                             = 109132,
    SPELL_MONK_ROLL_TRIGGER                     = 107427,
    SPELL_MONK_CHI_TORPEDO_HEAL                 = 124040,
    SPELL_MONK_CHI_TORPEDO_DAMAGE               = 117993,
    SPELL_MONK_FLYING_SERPENT_KICK              = 101545,
    SPELL_MONK_FLYING_SERPENT_KICK_NEW          = 115057,
    SPELL_MONK_FLYING_SERPENT_KICK_AOE          = 123586,
    SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE        = 116709,
    SPELL_MONK_CHI_BURST_DAMAGE                 = 130651,
    SPELL_MONK_CHI_BURST_HEAL                   = 130654,
    SPELL_MONK_ZEN_SPHERE_DAMAGE                = 124098,
    SPELL_MONK_ZEN_SPHERE_HEAL                  = 124081,
    SPELL_MONK_ZEN_SPHERE_DETONATE_HEAL         = 124101,
    SPELL_MONK_ZEN_SPHERE_DETONATE_DAMAGE       = 125033,
    SPELL_MONK_RENEWING_MIST_HOT                = 119611,
    SPELL_MONK_RENEWING_MIST_JUMP_AURA          = 119607,
    SPELL_MONK_GLYPH_OF_RENEWING_MIST           = 123334,
    SPELL_MONK_SURGING_MIST_HEAL                = 116995,
    SPELL_MONK_ENVELOPING_MIST_HEAL             = 132120,
    SPELL_MONK_PLUS_ONE_MANA_TEA                = 123760,
    SPELL_MONK_MANA_TEA_STACKS                  = 115867,
    SPELL_MONK_MANA_TEA_REGEN                   = 115294,
    SPELL_MONK_SPINNING_CRANE_KICK_HEAL         = 117640,
    MONK_NPC_JADE_SERPENT_STATUE                = 60849,
    SPELL_MONK_UPLIFT_ALLOWING_CAST             = 123757,
    SPELL_MONK_THUNDER_FOCUS_TEA                = 116680,
    SPELL_MONK_PATH_OF_BLOSSOM_AREATRIGGER      = 122035,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_DAMAGE     = 123408,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_MISSILE    = 118852,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_ROOT       = 123407,
    SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE   = 124280,
    SPELL_MONK_JADE_LIGHTNING_ENERGIZE          = 123333,
    SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP        = 117962,
    SPELL_MONK_POWER_STRIKES_TALENT             = 121817,
    SPELL_MONK_POWER_STRIKES_DUMMY              = 129914,
    SPELL_MONK_CREATE_CHI_SPHERE                = 121286,
    SPELL_MONK_GLYPH_OF_ZEN_FLIGHT              = 125893,
    SPELL_MONK_ZEN_FLIGHT                       = 125883,
    SPELL_MONK_BEAR_HUG                         = 127361,
    ITEM_MONK_T14_TANK_4P                       = 123159,
    MONK_NPC_BLACK_OX_STATUE                    = 61146,
    SPELL_MONK_GUARD                            = 115295,
};

class spell_monk_storm_earth_and_fire : public SpellScriptLoader
{
    public:
        spell_monk_storm_earth_and_fire() : SpellScriptLoader("spell_monk_storm_earth_and_fire") { }

        class spell_monk_storm_earth_and_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_storm_earth_and_fire_SpellScript);

            bool castspell;
            bool addMainAura;

            void CheckTarget(bool fullstack, Unit* caster, Unit* target)
            {
                std::list<Unit*> removeAllClones;
                for (uint8 i = 13; i < 16; ++i)
                    if (caster->m_SummonSlot[i])
                        if (Creature* crt = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                            if (!crt->IsDespawn())
                                {
                                    if (crt->getVictim() != target)
                                    {
                                        if (!fullstack)
                                            return;

                                        removeAllClones.push_back(crt);
                                        continue;
                                    }
                                    else
                                    {
                                        if (CreatureAI* ai = crt->AI())
                                            ai->ComonOnHome();

                                        castspell = false;
                                        return;
                                    }
                                }

                if (!removeAllClones.empty())
                    for(std::list<Unit*>::const_iterator i = removeAllClones.begin(); i != removeAllClones.end(); ++i)
                        if (Creature* crt = (*i)->ToCreature())
                            if (CreatureAI* ai = crt->AI())
                                ai->ComonOnHome();

                addMainAura = false;
            }

            SpellCastResult HandleCheckCast()
            {
                if (Unit* caster = GetCaster())
                {
                    Unit* target = GetExplTargetUnit();

                    if (!target)
                        return SPELL_FAILED_DONT_REPORT;

                    std::vector<uint32> id;
                    uint32 visualId = 0;
                    uint32 thirdId = 0;
                    bool addVisual = true;
                    addMainAura = true;
                    castspell = true;
                    id.push_back(138121);
                    id.push_back(138122);
                    id.push_back(138123);
                    std::random_shuffle(id.begin(), id.end());

                    if (Aura* aura = caster->GetAura(137639))
                        CheckTarget(aura->GetStackAmount() > 1, caster, target);

                    if (!castspell)
                        return SPELL_FAILED_DONT_REPORT;

                    if (!addMainAura)
                        return SPELL_CAST_OK;

                    for (std::vector<uint32>::iterator iter = id.begin(); iter != id.end(); ++iter)
                    {
                        bool getVisual = false;

                        switch (*iter)
                        {
                            case 138121: visualId = 138083; break;
                            case 138122: visualId = 138080; break;
                            case 138123: visualId = 138081; break;
                        }

                        if (caster->HasAura(visualId))
                        {
                            getVisual = true;
                            addVisual = false;
                        }

                        for (uint8 i = 13; i < 16; ++i)
                            if (caster->m_SummonSlot[i])
                                if (Creature* crt = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                                    if (!crt->IsDespawn())
                                    {
                                        if (getVisual)
                                            break;

                                        SpellInfo const* _spellinfo = sSpellMgr->GetSpellInfo(*iter);

                                        if (crt->GetEntry() == _spellinfo->Effects[EFFECT_0].MiscValue)
                                        {
                                            getVisual = true;
                                            addVisual = false;
                                            break;
                                        }
                                    }

                        if (getVisual)
                            continue;
                        else if (thirdId && !getVisual)
                            break;

                        thirdId = *iter;
                    }

                    if (addVisual)
                        caster->AddAura(visualId, caster);

                    if (thirdId)
                        caster->CastSpell(target, thirdId, true);
                }
                return SPELL_CAST_OK;
            }

            void HandleTargetSelect(WorldObject*& target)
            {
                if (!addMainAura)
                    target = NULL;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_storm_earth_and_fire_SpellScript::HandleCheckCast);
                OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_storm_earth_and_fire_SpellScript::HandleTargetSelect, EFFECT_1, TARGET_UNIT_CASTER);
                OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_storm_earth_and_fire_SpellScript::HandleTargetSelect, EFFECT_2, TARGET_UNIT_CASTER);
                OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_storm_earth_and_fire_SpellScript::HandleTargetSelect, EFFECT_3, TARGET_UNIT_CASTER);
                OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_storm_earth_and_fire_SpellScript::HandleTargetSelect, EFFECT_4, TARGET_UNIT_CASTER);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_storm_earth_and_fire_SpellScript();
        }

        class spell_monk_storm_earth_and_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_storm_earth_and_fire_AuraScript);

            void RemoveEff(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->RemoveAura(138083);
                    caster->RemoveAura(138080);
                    caster->RemoveAura(138081);

                    std::list<Creature*> ControllUnit;
                    for (uint8 i = 13; i < 16; ++i)
                        if (caster->m_SummonSlot[i])
                            if (Creature* crt = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                                if (!crt->IsDespawn())
                                    ControllUnit.push_back(crt);

                    for (std::list<Creature*>::const_iterator i = ControllUnit.begin(); i != ControllUnit.end(); ++i)
                        if (CreatureAI* crtAI = (*i)->AI())
                            crtAI->ComonOnHome();
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_storm_earth_and_fire_AuraScript::RemoveEff, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_storm_earth_and_fire_AuraScript();
        }
};

class spell_monk_storm_earth_and_fire_clone_visual : public SpellScriptLoader
{
public:
    spell_monk_storm_earth_and_fire_clone_visual() : SpellScriptLoader("spell_monk_storm_earth_and_fire_clone_visual") { }

    class spell_monk_storm_earth_and_fire_clone_visualAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_storm_earth_and_fire_clone_visualAuraScript);

        void ApplyEff(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->GetTypeId() == TYPEID_PLAYER)
                    if (!caster->m_SummonSlot[13] && !caster->m_SummonSlot[14] && !caster->m_SummonSlot[15])
                        caster->RemoveAura(137639);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_monk_storm_earth_and_fire_clone_visualAuraScript::ApplyEff, EFFECT_2, SPELL_AURA_MOD_SPELL_VISUAL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_storm_earth_and_fire_clone_visualAuraScript();
    }
};

class spell_monk_clone_cast : public SpellScriptLoader
{
public:
    spell_monk_clone_cast() : SpellScriptLoader("spell_monk_clone_cast") { }

    class spell_monk_clone_cast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_clone_cast_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if(Unit* caster = GetCaster())
                if (caster->HasSpell(139598))
                    caster->CastSpell(caster, 139597, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_clone_cast_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_clone_cast_SpellScript();
    }
};

class spell_monk_fists_of_fury_stun : public SpellScriptLoader
{
public:
    spell_monk_fists_of_fury_stun() : SpellScriptLoader("spell_monk_fists_of_fury_stun") { }

    class spell_monk_fists_of_fury_stun_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_fists_of_fury_stun_SpellScript);

        void HandleObjectAreaTargetSelect(std::list<WorldObject*>& targets)
        {
            std::list<Unit*> unitTargets;
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                if (Unit * target = (*itr)->ToUnit())
                    unitTargets.push_back(target);

            for (std::list<Unit*>::const_iterator itr = unitTargets.begin(); itr != unitTargets.end(); ++itr)
                if (((*itr))->HasAura(120086))
                    targets.remove(*itr);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_fists_of_fury_stun_SpellScript::HandleObjectAreaTargetSelect, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_fists_of_fury_stun_SpellScript();
    }
};

// Diffuse Magic - 122783
class spell_monk_diffuse_magic : public SpellScriptLoader
{
    public:
        spell_monk_diffuse_magic() : SpellScriptLoader("spell_monk_diffuse_magic") { }

        class spell_monk_diffuse_magic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_diffuse_magic_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    Unit::AuraApplicationMap AuraList = caster->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = AuraList.begin(); iter != AuraList.end(); ++iter)
                    {
                        Aura* aura = iter->second->GetBase();
                        if (!aura)
                            continue;

                        Unit* target = aura->GetCaster();
                        if (!target || target->GetGUID() == caster->GetGUID())
                            continue;

                        if (!target->IsWithinDist(caster, 40.0f))
                            continue;

                        if (aura->GetSpellInfo()->IsPositive())
                            continue;

                        if (!(aura->GetSpellInfo()->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                            continue;

                        bool isBoss = false;

                        if (Creature* crt = target->ToCreature())
                            if (crt->isWorldBoss() || crt->IsDungeonBoss())
                                isBoss = true;

                        if (!isBoss)
                        {
                            caster->AddAura(aura->GetSpellInfo()->Id, target);

                            if (Aura* targetAura = target->GetAura(aura->GetSpellInfo()->Id, caster->GetGUID()))
                            {
                                targetAura->SetMaxDuration(aura->GetDuration());
                                targetAura->SetDuration(aura->GetDuration());
                                targetAura->SetStackAmount(aura->GetStackAmount());

                                for (int i = 0; i < MAX_SPELL_EFFECTS; ++i)
                                    if (AuraEffect* targetEff = targetAura->GetEffect(i))
                                        if (AuraEffect* Eff = aura->GetEffect(i))
                                        {
                                            targetEff->SetAmount(Eff->GetAmount());
                                            targetEff->SetCritAmount(Eff->GetCritAmount());
                                            targetEff->SetPeriodicTimer(Eff->GetPeriodicTimer());
                                        }
                            }
                        }

                        caster->RemoveAura(aura->GetSpellInfo()->Id, target->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_diffuse_magic_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_diffuse_magic_SpellScript();
        }
};

// Guard - 115295
class spell_monk_guard : public SpellScriptLoader
{
    public:
        spell_monk_guard() : SpellScriptLoader("spell_monk_guard") { }

        class spell_monk_guard_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_guard_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _plr = GetCaster()->ToPlayer())
                {
                    amount += int32((_plr->GetTotalAttackPowerValue(BASE_ATTACK) * 18));
                    AddPct(amount, _plr->GetFloatValue(PLAYER_FIELD_VERSATILITY));

                    if (_plr->HasAura(ITEM_MONK_T14_TANK_4P))
                        amount = int32(amount * 1.2f);

                    if (_plr->HasAura(123401)) // Glyph of Guard
                        amount = int32(amount * 1.1f);
                }
                // For Black Ox Statue
                else if (GetCaster()->GetOwner())
                {
                    if (Player* _plr = GetCaster()->GetOwner()->ToPlayer())
                    {
                        amount += int32((_plr->GetTotalAttackPowerValue(BASE_ATTACK) * 18));
                        AddPct(amount, _plr->GetFloatValue(PLAYER_FIELD_VERSATILITY));

                        if (_plr->HasAura(ITEM_MONK_T14_TANK_4P))
                            amount = int32(amount * 1.2f);

                        if (_plr->HasAura(123401)) // Glyph of Guard
                            amount = int32(amount * 1.1f);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_guard_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_guard_AuraScript();
        }
};

// Bear Hug - 127361
class spell_monk_bear_hug : public SpellScriptLoader
{
    public:
        spell_monk_bear_hug() : SpellScriptLoader("spell_monk_bear_hug") { }

        class spell_monk_bear_hug_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_bear_hug_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                    amount = caster->CountPctFromMaxHealth(2);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_bear_hug_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_bear_hug_AuraScript();
        }
};

// Zen Flight - 125883
class spell_monk_zen_flight_check : public SpellScriptLoader
{
    public:
        spell_monk_zen_flight_check() : SpellScriptLoader("spell_monk_zen_flight_check") { }

        class spell_monk_zen_flight_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_zen_flight_check_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetMap()->IsBattlegroundOrArena())
                        return SPELL_FAILED_NOT_IN_BATTLEGROUND;

                    // In Kalimdor and Eastern Kingdom
                    if (!_player->HasSpell(90267) && (_player->GetMapId() == 1 || _player->GetMapId() == 0))
                        return SPELL_FAILED_NOT_HERE;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_zen_flight_check_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_zen_flight_check_SpellScript();
        }
};

// Glyph of Zen Flight - 125893
class spell_monk_glyph_of_zen_flight : public SpellScriptLoader
{
    public:
        spell_monk_glyph_of_zen_flight() : SpellScriptLoader("spell_monk_glyph_of_zen_flight") { }

        class spell_monk_glyph_of_zen_flight_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_glyph_of_zen_flight_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(SPELL_MONK_ZEN_FLIGHT, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(SPELL_MONK_ZEN_FLIGHT))
                        _player->removeSpell(SPELL_MONK_ZEN_FLIGHT, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_glyph_of_zen_flight_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_glyph_of_zen_flight_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }

        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_glyph_of_zen_flight_AuraScript();
        }
};

// Called by Jab - 100780
// Power Strikes - 121817
class spell_monk_power_strikes : public SpellScriptLoader
{
    public:
        spell_monk_power_strikes() : SpellScriptLoader("spell_monk_power_strikes") { }

        class spell_monk_power_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_power_strikes_SpellScript)

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(SPELL_MONK_POWER_STRIKES_DUMMY))
                    {
                        if (caster->GetPower(POWER_CHI) < caster->GetMaxPower(POWER_CHI))
                            caster->CastSpell(caster, 121283, true);
                        else
                            caster->CastSpell(caster, SPELL_MONK_CREATE_CHI_SPHERE, true);

                        caster->RemoveAura(SPELL_MONK_POWER_STRIKES_DUMMY);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_power_strikes_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_power_strikes_SpellScript();
        }
};

// Crackling Jade Lightning - 117952
class spell_monk_crackling_jade_lightning : public SpellScriptLoader
{
    public:
        spell_monk_crackling_jade_lightning() : SpellScriptLoader("spell_monk_crackling_jade_lightning") { }

        class spell_monk_crackling_jade_lightning_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_crackling_jade_lightning_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                    if (roll_chance_i(25))
                        caster->CastSpell(caster, SPELL_MONK_JADE_LIGHTNING_ENERGIZE, true);
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GetTarget()->HasAura(aurEff->GetSpellInfo()->Id, _player->GetGUID()))
                    {
                        if (!_player->HasSpellCooldown(SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP))
                        {
                            _player->CastSpell(GetTarget(), SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP, true);
                            _player->AddSpellCooldown(SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP, 0, getPreciseTime() + 8.0);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_crackling_jade_lightning_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectProc += AuraEffectProcFn(spell_monk_crackling_jade_lightning_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_crackling_jade_lightning_AuraScript();
        }
};

// Touch of Karma - 122470
class spell_monk_touch_of_karma : public SpellScriptLoader
{
    public:
        spell_monk_touch_of_karma() : SpellScriptLoader("spell_monk_touch_of_karma") { }

        class spell_monk_touch_of_karma_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_touch_of_karma_AuraScript);

            ObjectGuid eff1Target;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->m_SpecialTarget = GetUnitOwner()->GetGUID();
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->RemoveAura(122470);
                    caster->m_SpecialTarget.Clear();
                }
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (GetCaster())
                    amount = CalculatePct(GetCaster()->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_2].BasePoints);
            }

            void AfterAbsorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& /*absorbAmount*/)
            {
                if (Unit* caster = dmgInfo.GetVictim())
                {
                    if(!caster->m_SpecialTarget)
                        return;
                    if (Unit* target = ObjectAccessor::GetUnit(*caster, caster->m_SpecialTarget))
                    {
                        int32 bp = int32(dmgInfo.GetAbsorb() / 6);

                        if (bp)
                            caster->CastCustomSpell(target, SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_touch_of_karma_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_monk_touch_of_karma_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_touch_of_karma_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_monk_touch_of_karma_AuraScript::AfterAbsorb, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_touch_of_karma_AuraScript();
        }
};

// Called by Uplift - 116670
// Thunder Focus Tea - 116680
class spell_monk_thunder_focus_tea : public SpellScriptLoader
{
    public:
        spell_monk_thunder_focus_tea() : SpellScriptLoader("spell_monk_thunder_focus_tea") { }

        class spell_monk_thunder_focus_tea_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_thunder_focus_tea_SpellScript)

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (caster->HasAura(116680))
                            if (Aura* aura = target->GetAura(SPELL_MONK_RENEWING_MIST_HOT, caster->GetGUID()))
                                aura->RefreshDuration();
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAura(SPELL_MONK_THUNDER_FOCUS_TEA);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_thunder_focus_tea_SpellScript::HandleAfterCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_thunder_focus_tea_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_thunder_focus_tea_SpellScript();
        }
};

// Mana Tea - 115294
class spell_monk_mana_tea : public SpellScriptLoader
{
    public:
        spell_monk_mana_tea() : SpellScriptLoader("spell_monk_mana_tea") { }

        class spell_monk_mana_tea_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_mana_tea_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (GetCaster())
                {
                    // remove one charge per tick instead of remove aura on cast
                    // "Cancelling the channel will not waste stacks"
                    if (Aura* manaTea = GetCaster()->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                    {
                        if (manaTea->GetStackAmount() > 1)
                            manaTea->SetStackAmount(manaTea->GetStackAmount() - 1);
                        else
                            GetCaster()->RemoveAura(SPELL_MONK_MANA_TEA_STACKS);
                    }
                }
            }

            void CalculateMaxDuration(int32& duration)
            {
                if (Unit* caster = GetCaster())
                    if (Aura* manaTeaStacks = caster->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                        duration = 500 * manaTeaStacks->GetStackAmount();
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_mana_tea_AuraScript::OnTick, EFFECT_0, SPELL_AURA_OBS_MOD_POWER);
                DoCalcMaxDuration += AuraCalcMaxDurationFn(spell_monk_mana_tea_AuraScript::CalculateMaxDuration);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_mana_tea_AuraScript();
        }
};

// Brewing : Mana Tea - 123766
class spell_monk_mana_tea_stacks : public SpellScriptLoader
{
    public:
        spell_monk_mana_tea_stacks() : SpellScriptLoader("spell_monk_mana_tea_stacks") { }

        class spell_monk_mana_tea_stacks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_mana_tea_stacks_AuraScript);

            uint32 chiConsumed;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                chiConsumed = 0;
            }

            void SetData(uint32 type, uint32 cost)
            {
                chiConsumed += cost;
                if (chiConsumed >= 4)
                {
                    chiConsumed -= 4;

                    if (Unit* caster = GetCaster())
                    {
                        float critChance = caster->ToPlayer()->GetFloatValue(PLAYER_FIELD_CRIT_PERCENTAGE);
                        bool crit = roll_chance_f(critChance);
                        caster->CastSpell(caster, SPELL_MONK_MANA_TEA_STACKS, true);
                        if(crit)
                            caster->CastSpell(caster, SPELL_MONK_MANA_TEA_STACKS, true);
                        caster->CastSpell(caster, SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                    }
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_monk_mana_tea_stacks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_mana_tea_stacks_AuraScript();
        }
};

// Enveloping Mist - 124682
class spell_monk_enveloping_mist : public SpellScriptLoader
{
    public:
        spell_monk_enveloping_mist() : SpellScriptLoader("spell_monk_enveloping_mist") { }

        class spell_monk_enveloping_mist_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_enveloping_mist_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetExplTargetUnit())
                        _player->CastSpell(target, SPELL_MONK_ENVELOPING_MIST_HEAL, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_enveloping_mist_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_enveloping_mist_SpellScript();
        }
};

// Surging Mist - 116694
class spell_monk_surging_mist : public SpellScriptLoader
{
    public:
        spell_monk_surging_mist() : SpellScriptLoader("spell_monk_surging_mist") { }

        class spell_monk_surging_mist_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_surging_mist_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetExplTargetUnit())
                        _player->CastSpell(target, SPELL_MONK_SURGING_MIST_HEAL, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_surging_mist_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_surging_mist_SpellScript();
        }
};

// Zen Sphere - 124081
class spell_monk_zen_sphere : public SpellScriptLoader
{
    public:
        spell_monk_zen_sphere() : SpellScriptLoader("spell_monk_zen_sphere") { }

        class spell_monk_zen_sphere_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_zen_sphere_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();
                if(caster && target)
                    caster->CastSpell(target, SPELL_MONK_ZEN_SPHERE_DAMAGE, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();
                if(caster && target)
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE || removeMode == AURA_REMOVE_BY_CANCEL)
                    {
                        caster->CastSpell(target, SPELL_MONK_ZEN_SPHERE_DETONATE_HEAL, true);
                        caster->CastSpell(target, SPELL_MONK_ZEN_SPHERE_DETONATE_DAMAGE, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_zen_sphere_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_zen_sphere_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_zen_sphere_AuraScript();
        }
};

// Chi Burst - 123986
class spell_monk_chi_burst : public SpellScriptLoader
{
    public:
        spell_monk_chi_burst() : SpellScriptLoader("spell_monk_chi_burst") { }

        class spell_monk_chi_burst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_burst_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> tempUnitMap;
                        _player->GetAttackableUnitListInRange(tempUnitMap, _player->GetDistance(target));

                        // Chi Burst will always heal the Monk
                        _player->CastSpell(_player, SPELL_MONK_CHI_BURST_HEAL, true);

                        if (_player->IsValidAttackTarget(target))
                            _player->CastSpell(target, SPELL_MONK_CHI_BURST_DAMAGE, true);
                        else
                            _player->CastSpell(target, SPELL_MONK_CHI_BURST_HEAL, true);

                        for (std::list<Unit*>::const_iterator itr = tempUnitMap.begin(); itr != tempUnitMap.end(); ++itr)
                        {
                            if ((*itr)->GetGUID() == _player->GetGUID())
                                continue;

                            if (!(*itr)->IsInBetween(_player, target, 1.0f))
                                continue;

                            uint32 spell = _player->IsValidAttackTarget(*itr) ? SPELL_MONK_CHI_BURST_DAMAGE : SPELL_MONK_CHI_BURST_HEAL;
                            _player->CastSpell(*itr, spell, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_chi_burst_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_burst_SpellScript();
        }
};

// Energizing Brew - 115288
class spell_monk_energizing_brew : public SpellScriptLoader
{
    public:
        spell_monk_energizing_brew() : SpellScriptLoader("spell_monk_energizing_brew") { }

        class spell_monk_energizing_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_energizing_brew_SpellScript);

            SpellCastResult CheckFight()
            {
                if (!GetCaster()->isInCombat())
                    return SPELL_FAILED_CASTER_AURASTATE;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_energizing_brew_SpellScript::CheckFight);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_energizing_brew_SpellScript();
        }
};

// Spear Hand Strike - 116705
class spell_monk_spear_hand_strike : public SpellScriptLoader
{
    public:
        spell_monk_spear_hand_strike() : SpellScriptLoader("spell_monk_spear_hand_strike") { }

        class spell_monk_spear_hand_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_spear_hand_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->isInFront(_player))
                        {
                            _player->CastSpell(target, SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE, true);
                            _player->AddSpellCooldown(116705, 0, getPreciseTime() + 18.0);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_spear_hand_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_spear_hand_strike_SpellScript();
        }
};

// Flying Serpent Kick - 115057
class spell_monk_flying_serpent_kick : public SpellScriptLoader
{
    public:
        spell_monk_flying_serpent_kick() : SpellScriptLoader("spell_monk_flying_serpent_kick") { }

        class spell_monk_flying_serpent_kick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_flying_serpent_kick_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_FLYING_SERPENT_KICK_NEW))
                    return false;
                return true;
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(SPELL_MONK_FLYING_SERPENT_KICK))
                        caster->RemoveAura(SPELL_MONK_FLYING_SERPENT_KICK);

                    caster->CastSpell(caster, SPELL_MONK_FLYING_SERPENT_KICK_AOE, true);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_flying_serpent_kick_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_flying_serpent_kick_SpellScript();
        }
};

// Chi Torpedo - 115008 or Chi Torpedo (3 charges) - 121828
class spell_monk_chi_torpedo : public SpellScriptLoader
{
    public:
        spell_monk_chi_torpedo() : SpellScriptLoader("spell_monk_chi_torpedo") { }

        class spell_monk_chi_torpedo_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_torpedo_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        std::list<Unit*> tempUnitMap;
                        _player->GetAttackableUnitListInRange(tempUnitMap, 20.0f);

                        for (std::list<Unit*>::const_iterator itr = tempUnitMap.begin(); itr != tempUnitMap.end(); ++itr)
                        {
                            if (!(_player)->isInFront((*itr), M_PI / 3) && (*itr)->GetGUID() != _player->GetGUID())
                                continue;

                            uint32 spell = _player->IsValidAttackTarget(*itr) ? SPELL_MONK_CHI_TORPEDO_DAMAGE : SPELL_MONK_CHI_TORPEDO_HEAL;
                            _player->CastSpell(*itr, spell, true);
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_chi_torpedo_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_torpedo_SpellScript();
        }
};

// Purifying Brew - 119582
class spell_monk_purifying_brew : public SpellScriptLoader
{
    public:
        spell_monk_purifying_brew() : SpellScriptLoader("spell_monk_purifying_brew") { }

        class spell_monk_purifying_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_purifying_brew_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(145055))  // Item - Monk T16 Brewmaster 4P Bonus
                        caster->CastSpell(caster, 145056, true);
                    caster->RemoveAura(124255);
                    caster->RemoveAura(SPELL_MONK_MODERATE_STAGGER);
                    caster->RemoveAura(SPELL_MONK_LIGHT_STAGGER);
                    caster->RemoveAura(SPELL_MONK_HEAVY_STAGGER);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_purifying_brew_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_purifying_brew_SpellScript();
        }
};

// Clash - 122057 and Clash - 126449
class spell_monk_clash : public SpellScriptLoader
{
    public:
        spell_monk_clash() : SpellScriptLoader("spell_monk_clash") { }

        class spell_monk_clash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_clash_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        float distane = caster->GetDistance(target) / 2;
                        Position position;
                        caster->GetNearPosition(position, distane, caster->GetRelativeAngle(target->GetPositionX(), target->GetPositionY()));
                        target->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), SPELL_MONK_CLASH_CHARGE_TARGET, true);
                        caster->CastSpell(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), SPELL_MONK_CLASH_CHARGE_SELF, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_clash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_clash_SpellScript();
        }
};

// Keg Smash - 121253
class spell_monk_keg_smash : public SpellScriptLoader
{
    public:
        spell_monk_keg_smash() : SpellScriptLoader("spell_monk_keg_smash") { }

        class spell_monk_keg_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_keg_smash_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            _player->CastSpell(target, SPELL_MONK_KEG_SMASH_VISUAL, true);
                            _player->CastSpell(target, SPELL_MONK_WEAKENED_BLOWS, true);
                            _player->CastSpell(_player, SPELL_MONK_KEG_SMASH_ENERGIZE, true);
                            // Prevent to receive 2 CHI more than once time per cast
                            _player->AddSpellCooldown(SPELL_MONK_KEG_SMASH_ENERGIZE, 0, getPreciseTime() + 1.0);
                            _player->CastSpell(target, SPELL_MONK_DIZZYING_HAZE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_keg_smash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_keg_smash_SpellScript();
        }
};

// Elusive Brew - 115308
class spell_monk_elusive_brew : public SpellScriptLoader
{
    public:
        spell_monk_elusive_brew() : SpellScriptLoader("spell_monk_elusive_brew") { }

        class spell_monk_elusive_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_elusive_brew_SpellScript);

            void HandleOnHit()
            {
                int32 stackAmount = 0;

                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (Aura* brewStacks = _player->GetAura(SPELL_MONK_ELUSIVE_BREW_STACKS))
                            stackAmount = brewStacks->GetStackAmount();

                        if(Aura* elusiveBrew = _player->AddAura(SPELL_MONK_ELUSIVE_BREW, _player))
                        {
                            int32 maxDuration = elusiveBrew->GetMaxDuration();
                            int32 newDuration = stackAmount * 1000;
                            elusiveBrew->SetDuration(newDuration);

                            if (newDuration > maxDuration)
                                elusiveBrew->SetMaxDuration(newDuration);

                            _player->RemoveAura(SPELL_MONK_ELUSIVE_BREW_STACKS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_elusive_brew_SpellScript::HandleOnHit);
            }
        };

        class spell_monk_elusive_brew_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_elusive_brew_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if(caster->HasAura(138231)) // Item - Monk T15 Brewmaster 2P Bonus
                    {
                        if(Aura* staggering = caster->AddAura(138233, caster))
                        {
                            int32 newDuration = aurEff->GetBase()->GetMaxDuration();
                            staggering->SetDuration(newDuration);

                            if (newDuration > staggering->GetMaxDuration())
                                staggering->SetMaxDuration(newDuration);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_elusive_brew_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DODGE_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_elusive_brew_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_elusive_brew_SpellScript();
        }
};

// Breath of Fire - 115181
class spell_monk_breath_of_fire : public SpellScriptLoader
{
    public:
        spell_monk_breath_of_fire() : SpellScriptLoader("spell_monk_breath_of_fire") { }

        class spell_monk_breath_of_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_breath_of_fire_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            // if Dizzying Haze is on the target, they will burn for an additionnal damage over 8s
                            if (target->HasAura(SPELL_MONK_DIZZYING_HAZE) || caster->HasAura(157362))
                            {
                                _player->CastSpell(target, SPELL_MONK_BREATH_OF_FIRE_DOT, true);

                                // Glyph of Breath of Fire
                                if (_player->HasAura(123394))
                                    _player->CastSpell(target, SPELL_MONK_BREATH_OF_FIRE_CONFUSED, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_monk_breath_of_fire_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_breath_of_fire_SpellScript();
        }
};

// Soothing Mist - 115175
class spell_monk_soothing_mist : public SpellScriptLoader
{
    public:
        spell_monk_soothing_mist() : SpellScriptLoader("spell_monk_soothing_mist") { }

        class spell_monk_soothing_mist_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_soothing_mist_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                Unit* caster = GetCaster();
                if (!caster || !target)
                    return;

                caster->CastSpell(target, SPELL_MONK_SOOTHING_MIST_VISUAL, true);

                for (int i = SUMMON_SLOT_TOTEM; i < SUMMON_SLOT_TOTEM + 1; ++i)
                {
                    if(caster->m_SummonSlot[i])
                    {
                        if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                            if(summon->GetEntry() == 60849 && summon->GetExactDist2d(target) < 40.0f)
                            {
                                if(caster->IsInRaidWith(target))
                                {
                                    target->CastSpell(target, 125955, true);
                                    summon->CastSpell(target, 125950, true);
                                }
                                else if(caster->GetExactDist2d(summon) < 40.0f)
                                {
                                    caster->CastSpell(caster, 125955, true);
                                    summon->CastSpell(caster, 125950, true);
                                }
                            }
                    }
                }
            }

            void HandleEffectPeriodic(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        // 25% to give 1 chi per tick
                        if (roll_chance_i(25))
                            caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST_ENERGIZE, true);

                        caster->SendSpellCreateVisual(GetSpellInfo(), target, target, 1, 24208);
                    }
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();
                if (!caster || !target)
                    return;
                target->RemoveAura(SPELL_MONK_SOOTHING_MIST_VISUAL);
                if(caster->IsInRaidWith(target))
                {
                    target->RemoveAura(125955);
                    target->RemoveAura(125950);
                }
                else
                {
                    caster->RemoveAura(125955);
                    caster->RemoveAura(125950);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_monk_soothing_mist_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_soothing_mist_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_monk_soothing_mist_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_soothing_mist_AuraScript();
        }
};

// Zen Pilgrimage - 126892 and Zen Pilgrimage : Return - 126895
class spell_monk_zen_pilgrimage : public SpellScriptLoader
{
    public:
        spell_monk_zen_pilgrimage() : SpellScriptLoader("spell_monk_zen_pilgrimage") { }

        class spell_monk_zen_pilgrimage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_zen_pilgrimage_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ZEN_PILGRIMAGE) || !sSpellMgr->GetSpellInfo(SPELL_MONK_ZEN_PILGRIMAGE_RETURN))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (GetSpellInfo()->Id == SPELL_MONK_ZEN_PILGRIMAGE)
                        {
                            _player->SaveRecallPosition();
                            _player->TeleportTo(870, 3818.55f, 1793.18f, 950.35f, _player->GetOrientation());
                        }
                        else if (GetSpellInfo()->Id == SPELL_MONK_ZEN_PILGRIMAGE_RETURN)
                        {
                            _player->TeleportTo(_player->m_recallLoc);
                            _player->RemoveAura(126896);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_zen_pilgrimage_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_zen_pilgrimage_SpellScript();
        }
};

// Blackout Kick - 100784
class spell_monk_blackout_kick : public SpellScriptLoader
{
    public:
        spell_monk_blackout_kick() : SpellScriptLoader("spell_monk_blackout_kick") { }

        class spell_monk_blackout_kick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_blackout_kick_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (caster->HasAura(128595))
                        {
                            uint32 triggered_spell_id = 128531;
                            Unit* originalCaster = caster->GetOwner() ? caster->GetOwner(): caster;
                            int32 damsges = GetHitAbsorb() + GetHitDamage() + GetHitResist() + GetHitBlocked();
                            int32 basepoints0 = CalculatePct(damsges, GetSpellInfo()->Effects[EFFECT_1].BasePoints);

                            if (!originalCaster->HasAura(132005) && !target->isInBack(caster))
                                triggered_spell_id = 128591;

                            if (triggered_spell_id == 128531)
                                basepoints0 /= 4;

                            caster->CastCustomSpell(target, triggered_spell_id, &basepoints0, NULL, NULL, true);
                        }
                    }
                }
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit() != GetExplTargetUnit())
                    SetHitDamage(int32(GetHitDamage() * 0.5f));
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    // Brewmaster : Training - you gain Shuffle, increasing parry chance and stagger amount by 20%
                    if (caster->GetTypeId() == TYPEID_PLAYER && caster->ToPlayer()->GetSpecializationId(caster->ToPlayer()->GetActiveSpec()) == SPEC_MONK_BREWMASTER)
                    {
                        if (Aura* aura = caster->GetAura(SPELL_MONK_SHUFFLE))
                        {
                            int32 _duration = int32(aura->GetDuration() + 6000);
                            aura->SetDuration(_duration);
                            if (_duration > aura->GetMaxDuration())
                                aura->SetMaxDuration(_duration);
                        }
                        else
                            caster->CastSpell(caster, SPELL_MONK_SHUFFLE, true);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_blackout_kick_SpellScript::HandleAfterCast);
                AfterHit += SpellHitFn(spell_monk_blackout_kick_SpellScript::HandleAfterHit);
                OnEffectHitTarget += SpellEffectFn(spell_monk_blackout_kick_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_blackout_kick_SpellScript();
        }
};

// Provoke - 115546
class spell_monk_provoke : public SpellScriptLoader
{
    public:
        spell_monk_provoke() : SpellScriptLoader("spell_monk_provoke") { }

        class spell_monk_provoke_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_provoke_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* target = GetExplTargetUnit();
                if (!target)
                    return SPELL_FAILED_NO_VALID_TARGETS;
                else if (target->GetTypeId() == TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;
                else if (!target->IsWithinLOSInMap(GetCaster()))
                    return SPELL_FAILED_LINE_OF_SIGHT;
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (target && caster)
                {
                    if (caster->getClass() == CLASS_MONK && caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (target->GetEntry() == 61146)
                            caster->CastSpell(target, SPELL_MONK_PROVOKE_AOE, true, NULL, NULL, target->GetGUID());
                        else
                            caster->CastSpell(target, SPELL_MONK_PROVOKE, true);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_provoke_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_provoke_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_provoke_SpellScript();
        }
};

// Fortifying brew - 120954
class spell_monk_fortifying_brew : public SpellScriptLoader
{
    public:
        spell_monk_fortifying_brew() : SpellScriptLoader("spell_monk_fortifying_brew") {}

        class spell_monk_fortifying_brew_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_fortifying_brew_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                int32 perc = 20;
                if (Aura* aura = caster->GetAura(124997))
                    perc -= aura->GetEffect(EFFECT_1)->GetAmount();

                amount += caster->CountPctFromCurHealth(perc);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_fortifying_brew_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_fortifying_brew_AuraScript();
        }
};

// Roll - 109132 or Roll (3 charges) - 121827
class spell_monk_roll : public SpellScriptLoader
{
    public:
        spell_monk_roll() : SpellScriptLoader("spell_monk_roll") { }

        class spell_monk_roll_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_roll_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL))
                    return false;
                return true;
            }

            void HandleBeforeCast()
            {
                Aura* aur = GetCaster()->AddAura(SPELL_MONK_ROLL_TRIGGER, GetCaster());
                if (!aur)
                    return;

                AuraApplication* app =  aur->GetApplicationOfTarget(GetCaster()->GetGUID());
                if (!app)
                    return;

                app->ClientUpdate();
            }

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                caster->CastSpell(caster, SPELL_MONK_ROLL_TRIGGER, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_monk_roll_SpellScript::HandleBeforeCast);
                AfterCast += SpellCastFn(spell_monk_roll_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_roll_SpellScript();
        }
};

// 115636 - Mastery : Bottled Fury
class spell_mastery_bottled_fury : public SpellScriptLoader
{
public:
    spell_mastery_bottled_fury() : SpellScriptLoader("spell_mastery_bottled_fury") { }

    class spell_mastery_bottled_fury_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mastery_bottled_fury_SpellScript);

        void HandleAfterHit()
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* WW_mastery = caster->GetAura(115636))
                {
                    int32 amount = WW_mastery->GetEffect(EFFECT_0)->GetAmount();

                    if (roll_chance_i(amount))
                        caster->AddAura(125195, caster);
                }
            }
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_mastery_bottled_fury_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mastery_bottled_fury_SpellScript();
    }
};

class spell_monk_remove_zen_flight : public SpellScriptLoader
{
    public:
        spell_monk_remove_zen_flight() : SpellScriptLoader("spell_monk_remove_zen_flight") { }

        class spell_monk_remove_zen_flight_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_remove_zen_flight_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                Player* player = target->ToPlayer();
                if (!player)
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

                if (removeMode != AURA_REMOVE_BY_CANCEL)
                {
                    player->CastSpell(player, 54649, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_monk_remove_zen_flight_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_remove_zen_flight_AuraScript();
        }
};

// Spinning Crane Kick - 107270, 148187
class spell_monk_spinning_crane_kick : public SpellScriptLoader
{
    public:
        spell_monk_spinning_crane_kick() : SpellScriptLoader("spell_monk_spinning_crane_kick") { }

        class spell_monk_spinning_crane_kick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_spinning_crane_kick_SpellScript)

            void HandleAfterCast()
            {
                if(Unit* caster = GetCaster())
                {
                    Player* plr = caster->ToPlayer();
                    if(plr && GetSpell()->GetTargetCount() >= 3)
                    {
                        if (caster->HasSpell(139598))
                        {
                            if(!plr->HasSpellCooldown(139597))
                            {
                                caster->CastSpell(caster, 139597, true);
                                plr->AddSpellCooldown(139597, 0, getPreciseTime() + 6.0);
                            }
                        }

                        if(!plr->HasSpellCooldown(129881))
                        {
                            caster->CastSpell(caster, 129881, true);
                            plr->AddSpellCooldown(129881, 0, getPreciseTime() + 6.0);
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_spinning_crane_kick_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_spinning_crane_kick_SpellScript();
        }
};

// Transcendence - 101643
class spell_monk_transcendence : public SpellScriptLoader
{
    public:
        spell_monk_transcendence() : SpellScriptLoader("spell_monk_transcendence") { }

        class spell_monk_transcendence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_transcendence_SpellScript);

            void HandleBeforeCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if(caster->m_SummonSlot[17])
                    {
                        if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[17]))
                            summon->DespawnOrUnsummon(500);
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_monk_transcendence_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_transcendence_SpellScript();
        }
};

// Transcendence: Transfer - 119996
class spell_monk_transcendence_transfer : public SpellScriptLoader
{
    public:
        spell_monk_transcendence_transfer() : SpellScriptLoader("spell_monk_transcendence_transfer") { }

        class spell_monk_transcendence_transfer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_transcendence_transfer_SpellScript);

            SpellCastResult CheckDist()
            {
                if (Unit* caster = GetCaster())
                    if (caster->m_SummonSlot[17])
                        if (Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[17]))
                            if (summon->IsWithinDistInMap(caster, 40.0f))
                                return SPELL_CAST_OK;

                return SPELL_FAILED_OUT_OF_RANGE;
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if(caster->m_SummonSlot[17])
                    {
                        if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[17]))
                        {
                            float x, y, z, o;
                            summon->GetPosition(x, y, z, o);
                            summon->NearTeleportTo(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), caster->GetOrientation());
                            caster->NearTeleportTo(x, y, z, o);
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_transcendence_transfer_SpellScript::HandleOnCast);
                OnCheckCast += SpellCheckCastFn(spell_monk_transcendence_transfer_SpellScript::CheckDist);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_transcendence_transfer_SpellScript();
        }
};

// Charging Ox Wave - 125084
class spell_monk_charging_ox_wave : public SpellScriptLoader
{
    public:
        spell_monk_charging_ox_wave() : SpellScriptLoader("spell_monk_charging_ox_wave") { }

        class spell_monk_charging_ox_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_charging_ox_wave_SpellScript);

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    WorldLocation location = *GetExplTargetDest();
                    Position position;
                    caster->GetNearPosition(position, 30.0f, 0.0f);
                    location.Relocate(position);
                    SetExplTargetDest(location);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_charging_ox_wave_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_charging_ox_wave_SpellScript();
        }
};

// spell 119611 Renewing Mist
class spell_monk_renewing_mist : public SpellScriptLoader
{
public:
    spell_monk_renewing_mist() : SpellScriptLoader("spell_monk_renewing_mist") { }

    class spell_monk_renewing_mistAuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_renewing_mistAuraScript);

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if(Aura* aura = GetAura())
                aura->SetStackAmount(aurEff->GetAmount());
        }

        void OnTick(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* Uplift = caster->GetAura(123757))
                    Uplift->RefreshTimers();

                if(GetAura()->GetStackAmount() > 1)
                {
                    int32 setstack = GetAura()->GetStackAmount() - 1;
                    caster->CastCustomSpell(caster, SPELL_MONK_RENEWING_MIST_JUMP_AURA, NULL, &setstack, NULL, true, NULL, aurEff, caster->GetGUID());
                    GetAura()->SetStackAmount(1);
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_monk_renewing_mistAuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_renewing_mistAuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_renewing_mistAuraScript();
    }
};

//Renewing Mist selector - 119607
class spell_monk_renewing_mist_selector : public SpellScriptLoader
{
    public:
        spell_monk_renewing_mist_selector() : SpellScriptLoader("spell_monk_renewing_mist_selector") { }

        class spell_monk_renewing_mist_selector_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_renewing_mist_selector_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(AuraCheck());
                targets.remove_if(DistanceCheck(GetCaster(), 20.0f));
                targets.sort(CheckHealthState());
                if (targets.size() > 1)
                    targets.resize(1);

                if (targets.empty())
                    targets.push_back(GetCaster());
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                SpellValue const* spellValue = GetSpellValue();
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if(!caster || !spellValue || !target)
                    return;

                int32 bp1 = spellValue->EffectBasePoints[1];
                caster->CastSpell(target, 119647, true);
                caster->CastCustomSpell(target, SPELL_MONK_RENEWING_MIST_HOT, NULL, &bp1, NULL, true, NULL, NULL, caster->GetGUID());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_renewing_mist_selector_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_renewing_mist_selector_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
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
            class AuraCheck
            {
                public:
                    AuraCheck(){}

                    bool operator()(WorldObject* unit)
                    {
                       return (!unit->ToUnit() || unit->ToUnit()->HasAura(119611));
                    }
            };
            class DistanceCheck
            {
                public:
                    DistanceCheck(Unit* caster, float dist) : _caster(caster), _dist(dist) {}

                    bool operator()(WorldObject* unit)
                    {
                        return _caster->GetExactDist2d(unit) > _dist;
                    }

                private:
                    Unit* _caster;
                    float _dist;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_renewing_mist_selector_SpellScript();
        }
};

//Renewing Mist start - 115151
class spell_monk_renewing_mist_start : public SpellScriptLoader
{
    public:
        spell_monk_renewing_mist_start() : SpellScriptLoader("spell_monk_renewing_mist_start") { }

        class spell_monk_renewing_mist_start_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_renewing_mist_start_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if(!caster || !target)
                    return;

                int32 bp1 = 3;
                if (Aura* aura = caster->GetAura(116680))
                {
                    bp1 +=2;
                    aura->Remove();
                }
                caster->CastCustomSpell(target, SPELL_MONK_RENEWING_MIST_HOT, NULL, &bp1, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_renewing_mist_start_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_HEAL);
            }
        private:
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_renewing_mist_start_SpellScript();
        }
};

//Healing Sphere despawn - 135914, 135920
class spell_monk_healing_sphere_despawn : public SpellScriptLoader
{
    public:
        spell_monk_healing_sphere_despawn() : SpellScriptLoader("spell_monk_healing_sphere_despawn") { }

        class spell_monk_healing_sphere_despawn_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_healing_sphere_despawn_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(DistanceCheck(GetCaster(), 6.0f));
                targets.sort(CheckHealthState());
                if (targets.size() > 1)
                    targets.resize(1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_healing_sphere_despawn_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
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
            class DistanceCheck
            {
                public:
                    DistanceCheck(Unit* caster, float dist) : _caster(caster), _dist(dist) {}

                    bool operator()(WorldObject* unit)
                    {
                        return _caster->GetExactDist2d(unit) > _dist;
                    }

                private:
                    Unit* _caster;
                    float _dist;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_healing_sphere_despawn_SpellScript();
        }
};

//Eminence - 126890, 117895
class spell_monk_eminence : public SpellScriptLoader
{
    public:
        spell_monk_eminence() : SpellScriptLoader("spell_monk_eminence") { }

        class spell_monk_eminence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_eminence_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(DistanceCheck(GetCaster(), 20.0f));
                targets.sort(CheckHealthState());
                if (targets.size() > 1)
                    targets.resize(1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_eminence_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
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
            class DistanceCheck
            {
                public:
                    DistanceCheck(Unit* caster, float dist) : _caster(caster), _dist(dist) {}

                    bool operator()(WorldObject* unit)
                    {
                        return _caster->GetExactDist2d(unit) > _dist;
                    }

                private:
                    Unit* _caster;
                    float _dist;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_eminence_SpellScript();
        }
};

//Guard from Sanctuary of the Ox - 118605
class spell_monk_guard_ox : public SpellScriptLoader
{
    public:
        spell_monk_guard_ox() : SpellScriptLoader("spell_monk_guard_ox") { }

        class spell_monk_guard_ox_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_guard_ox_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetCaster());
                if (Unit* owner = GetCaster()->GetOwner())
                    targets.remove(owner);
                targets.remove_if(AuraCheck());
                targets.remove_if(DistanceCheck(GetCaster(), 40.0f));
                if (targets.size() > 1)
                    targets.resize(1);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if(!caster || !target)
                    return;

                caster->CastSpell(target, 118627, true);
                if (Unit* owner = caster->GetOwner())
                    owner->CastSpell(target, 118604, true);
                else
                    caster->CastSpell(target, 118604, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_guard_ox_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_monk_guard_ox_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }

        private:
            class AuraCheck
            {
                public:
                    AuraCheck(){}

                    bool operator()(WorldObject* unit)
                    {
                       return (!unit->ToUnit() || unit->ToUnit()->HasAura(118604));
                    }
            };
            class DistanceCheck
            {
                public:
                    DistanceCheck(Unit* caster, float dist) : _caster(caster), _dist(dist) {}

                    bool operator()(WorldObject* unit)
                    {
                        return _caster->GetExactDist2d(unit) > _dist;
                    }

                private:
                    Unit* _caster;
                    float _dist;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_guard_ox_SpellScript();
        }
};

// Touch of Death - 115080
class spell_monk_touch_of_death : public SpellScriptLoader
{
    public:
        spell_monk_touch_of_death() : SpellScriptLoader("spell_monk_touch_of_death") { }

        class spell_monk_touch_of_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_touch_of_death_SpellScript);

            SpellCastResult CheckTarget()
            {
                Unit* caster = GetCaster();
                Unit* target = GetExplTargetUnit();

                if(!caster || !target)
                    return SPELL_FAILED_BAD_TARGETS;

                if (Creature* unit = target->ToCreature())
                    if (unit->IsDungeonBoss() && target->GetHealthPct() > 10)
                        return SPELL_FAILED_BAD_TARGETS;

                if (target->GetHealth() > caster->GetMaxHealth())
                    return SPELL_FAILED_BAD_TARGETS;

                if (target->GetTypeId() == TYPEID_PLAYER || target->isPet())
                    if (target->GetHealthPct() > 10)
                        return SPELL_FAILED_BAD_TARGETS;

                if (target->GetTypeId() == TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                if (Unit* owner = target->GetAnyOwner())
                    if (owner->GetTypeId() == TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    int32 damage = caster->GetMaxHealth();
                    SetHitDamage(damage);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_touch_of_death_SpellScript::CheckTarget);
                OnEffectHitTarget += SpellEffectFn(spell_monk_touch_of_death_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_touch_of_death_SpellScript();
        }
};

// Dampen Harm - 122278
class spell_monk_dampen_harm : public SpellScriptLoader
{
    public:
        spell_monk_dampen_harm() : SpellScriptLoader("spell_monk_dampen_harm") { }

        class spell_monk_dampen_harm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_dampen_harm_AuraScript);

            uint32 bpHealh;

            bool Load()
            {
                if (Unit* caster = GetCaster())
                    bpHealh = uint32(caster->GetMaxHealth() * float(GetSpellInfo()->Effects[EFFECT_0].BasePoints / 100.0f));
                else
                    bpHealh = 0;
                return true;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if ((int32)dmgInfo.GetDamage() >= bpHealh)
                {
                    absorbAmount = int32(dmgInfo.GetDamage() * 0.5f);
                    aurEff->GetBase()->ModCharges(-1);
                }
                else
                    absorbAmount = 0;
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_dampen_harm_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_dampen_harm_AuraScript();
        }
};

// Leer of the Ox - 115543
class spell_monk_leer_of_the_ox : public SpellScriptLoader
{
    public:
        spell_monk_leer_of_the_ox() : SpellScriptLoader("spell_monk_leer_of_the_ox") { }

        class spell_monk_leer_of_the_ox_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_leer_of_the_ox_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* target = GetExplTargetUnit();
                Unit* caster = GetCaster();
                if (!target || !caster)
                    return SPELL_FAILED_NO_VALID_TARGETS;
                else if (target->GetTypeId() == TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;
                else if (!target->IsWithinLOSInMap(GetCaster()))
                    return SPELL_FAILED_LINE_OF_SIGHT;
                else
                {
                    for (int i = SUMMON_SLOT_TOTEM; i < SUMMON_SLOT_TOTEM + 1; ++i)
                    {
                        if(caster->m_SummonSlot[i])
                        {
                            if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                                if(summon->GetEntry() == 61146 && summon->GetExactDist2d(target) > 40.0f)
                                    return SPELL_FAILED_LINE_OF_SIGHT;
                        }
                    }
                }
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (target && caster)
                {
                    for (int i = SUMMON_SLOT_TOTEM; i < SUMMON_SLOT_TOTEM + 1; ++i)
                    {
                        if(caster->m_SummonSlot[i])
                        {
                            if(Creature* summon = caster->GetMap()->GetCreature(caster->m_SummonSlot[i]))
                                if(summon->GetEntry() == 61146)
                                {
                                    summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                                    summon->CastSpell(target, 118585, true);
                                }
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_leer_of_the_ox_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_leer_of_the_ox_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_leer_of_the_ox_SpellScript();
        }
};

// Clash - 122242
class spell_monk_clash_stun : public SpellScriptLoader
{
    public:
        spell_monk_clash_stun() : SpellScriptLoader("spell_monk_clash_stun") { }

        class spell_monk_clash_stun_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_clash_stun_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if(GetCaster() && GetCaster()->getVictim())
                    targets.push_back(GetCaster()->getVictim());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_clash_stun_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_clash_stun_SpellScript();
        }
};

// Mana Tea Glyph - 123761
class spell_monk_mana_tea_glyph : public SpellScriptLoader
{
    public:
        spell_monk_mana_tea_glyph() : SpellScriptLoader("spell_monk_mana_tea_glyph") { }

        class spell_monk_mana_tea_glyph_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_mana_tea_glyph_SpellScript);

            void HandleEnergy(SpellEffIndex /*effIndex*/)
            {
                int32 setDamage = 0;
                if (Unit* caster = GetCaster())
                    if (Aura* aura = caster->GetAura(115867))
                    {
                        int32 stack = aura->GetStackAmount();
                        if(stack > 1)
                            stack = 2;
                        else
                            stack = 1;
                        setDamage = GetSpellInfo()->Effects[EFFECT_0].BasePoints * stack;
                        aura->ModStackAmount(-stack);
                    }

                SetEffectValue(setDamage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_mana_tea_glyph_SpellScript::HandleEnergy, EFFECT_0, SPELL_EFFECT_ENERGIZE_PCT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_mana_tea_glyph_SpellScript();
        }
};

// Glyph of Paralysis - 115078
class spell_monk_glyph_of_paralysis : public SpellScriptLoader
{
    public:
        spell_monk_glyph_of_paralysis() : SpellScriptLoader("spell_monk_glyph_of_paralysis") { }

        class spell_monk_glyph_of_paralysis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_glyph_of_paralysis_SpellScript);

            void HandleBeforeCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (caster->HasAura(125755))
                        {
                            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                            target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
                        }
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_monk_glyph_of_paralysis_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_glyph_of_paralysis_SpellScript();
        }
};

//Glyph of Surging Mist - 123273
class spell_monk_glyph_of_surging_mist : public SpellScriptLoader
{
    public:
        spell_monk_glyph_of_surging_mist() : SpellScriptLoader("spell_monk_glyph_of_surging_mist") { }

        class spell_monk_glyph_of_surging_mist_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_glyph_of_surging_mist_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(HpCheck());
                targets.sort(CheckHealthState());
                if (targets.size() > 1)
                    targets.resize(1);

                if (targets.empty())
                    targets.push_back(GetCaster());
            }

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_MONK_SURGING_MIST_HEAL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_glyph_of_surging_mist_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_glyph_of_surging_mist_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
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
            class HpCheck
            {
                public:
                    HpCheck(){}

                    bool operator()(WorldObject* unit)
                    {
                       return (!unit->ToUnit() || unit->ToUnit()->IsFullHealth());
                    }
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_glyph_of_surging_mist_SpellScript();
        }
};

// Glyph of Targeted Expulsion - 147489
class spell_monk_glyph_of_targeted_expulsion : public SpellScriptLoader
{
    public:
        spell_monk_glyph_of_targeted_expulsion() : SpellScriptLoader("spell_monk_glyph_of_targeted_expulsion") { }

        class spell_monk_glyph_of_targeted_expulsion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_glyph_of_targeted_expulsion_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (target && caster)
                {
                    if(target != caster)
                        SetHitHeal(int32(GetHitHeal() / 2));
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_glyph_of_targeted_expulsion_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_glyph_of_targeted_expulsion_SpellScript();
        }
};

//Chi Wave - 132466
class spell_monk_chi_wave_filter : public SpellScriptLoader
{
    public:
        spell_monk_chi_wave_filter() : SpellScriptLoader("spell_monk_chi_wave_filter") { }

        class spell_monk_chi_wave_filter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_filter_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetCaster());
                targets.remove_if(OptionCheck(GetCaster()));
                if (!GetCaster()->IsFriendlyTo(GetOriginalCaster()))
                {
                    targets.remove_if(FriendlyToOriginalCaster(GetOriginalCaster(), GetSpellInfo()));
                    targets.sort(CheckHealthState());
                    if (targets.size() > 1)
                        targets.resize(1);
                }
                else
                {
                    targets.remove_if(OptionCheck(GetOriginalCaster()));
                    targets.sort(CheckNearbyVictim(GetCaster()));
                    if (targets.size() > 1)
                        targets.resize(1);
                }
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 bp1 = GetEffectValue();

                if(bp1 >= 7)
                    return;

                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                Unit* origCaster = GetOriginalCaster();
                if (target && caster && origCaster)
                {
                    bp1++;
                    if (target->IsFriendlyTo(origCaster))
                        caster->CastCustomSpell(target, 132464, NULL, &bp1, NULL, true, NULL, NULL, origCaster->GetGUID());
                    else
                        caster->CastCustomSpell(target, 132467, NULL, &bp1, NULL, true, NULL, NULL, origCaster->GetGUID());
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_wave_filter_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_wave_filter_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        private:
            class OptionCheck
            {
                public:
                    OptionCheck(Unit* caster) : _caster(caster) {}

                    Unit* _caster;

                    bool operator()(WorldObject* unit)
                    {
                        if(!unit->ToUnit())
                            return true;
                        if(!_caster->IsValidAttackTarget(unit->ToUnit()))
                            return true;
                        return false;
                    }
            };
            class FriendlyToOriginalCaster
            {
            public:
                FriendlyToOriginalCaster(Unit* caster, SpellInfo const* spellInfo) : _caster(caster), _spellInfo(spellInfo){}

                Unit* _caster;
                SpellInfo const* _spellInfo;

                bool operator()(WorldObject* unit)
                {
                    Unit* victim = unit->ToUnit();
                    if (!victim)
                        return true;
                    if (!_caster->IsFriendlyTo(victim) || !_caster->_IsValidAssistTarget(victim, _spellInfo))
                        return true;
                    return false;
                }
            };
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
            class CheckNearbyVictim
            {
            public:
                CheckNearbyVictim(Unit* caster) : _caster(caster) { }

                Unit* _caster;

                bool operator() (WorldObject* a, WorldObject* b) const
                {
                    Unit* unita = a->ToUnit();
                    Unit* unitb = b->ToUnit();
                    if (!unita)
                        return true;
                    if (!unitb)
                        return false;

                    Position posA;
                    Position posB;
                    unita->GetPosition(&posA);
                    unitb->GetPosition(&posB);
                    float distA = _caster->GetDistance(posA);
                    float distB = _caster->GetDistance(posB);

                    return distA < distB;
                }
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_wave_filter_SpellScript();
        }
};

// Chi Wave - 132464, 132467
class spell_monk_chi_wave_dummy : public SpellScriptLoader
{
    public:
        spell_monk_chi_wave_dummy() : SpellScriptLoader("spell_monk_chi_wave_dummy") { }

        class spell_monk_chi_wave_dummy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_chi_wave_dummy_AuraScript);

            void ApplyDummy(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetTarget();
                if (target && caster)
                {
                    int32 bp1 = aurEff->GetAmount();
                    target->CastCustomSpell(target, 132466, NULL, &bp1, NULL, true, NULL, NULL, GetCasterGUID());
                    if (target->IsFriendlyTo(caster))
                        caster->CastCustomSpell(target, 132463, NULL, &bp1, NULL, true, NULL, NULL, GetCasterGUID());
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_chi_wave_dummy_AuraScript::ApplyDummy, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_chi_wave_dummy_AuraScript();
        }
};

// Chi Wave - 115098
class spell_monk_chi_wave : public SpellScriptLoader
{
    public:
        spell_monk_chi_wave() : SpellScriptLoader("spell_monk_chi_wave") { }

        class spell_monk_chi_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_SpellScript);

            void HandleBeforeCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        uint32 spellId = 0;
                        int32 bp = 1;

                        if (target->IsFriendlyTo(caster))
                        {
                            spellId = 132464;

                            if (!caster->_IsValidAssistTarget(target, GetSpellInfo()))
                                target = caster;
                        }
                        else
                        {
                            if (!caster->IsValidAttackTarget(target))
                            {
                                spellId = 132464;
                                target = caster;
                            }
                            else
                                spellId = 132467;
                        }

                        caster->CastCustomSpell(target, spellId, NULL, &bp, NULL, true, NULL, NULL, caster->GetGUID());
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_monk_chi_wave_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_wave_SpellScript();
        }
};

// Disable - 116095
class spell_monk_disable : public SpellScriptLoader
{
    public:
        spell_monk_disable() : SpellScriptLoader("spell_monk_disable") { }

        class spell_monk_disable_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_disable_AuraScript);

            void CalculateMaxDuration(int32& duration)
            {
                if (Unit* caster = GetCaster())
                    if(Unit* target = GetUnitOwner())
                    {
                        if(target->ToPlayer())
                            duration = 8000;
                        else
                            duration = 15000;
                    }
            }

            void Register()
            {
                DoCalcMaxDuration += AuraCalcMaxDurationFn(spell_monk_disable_AuraScript::CalculateMaxDuration);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_disable_AuraScript();
        }
};

// Purified Healing - 145056
class spell_monk_purified_healing : public SpellScriptLoader
{
    public:
        spell_monk_purified_healing() : SpellScriptLoader("spell_monk_purified_healing") { }

        class spell_monk_purified_healing_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_purified_healing_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* bonusAura = caster->GetAura(145055)) // Item - Monk T16 Brewmaster 4P Bonus
                    {
                        int32 perc = bonusAura->GetEffect(EFFECT_0)->GetAmount();
                        int32 _amount = 0;
                        if (Aura* staggerAura = caster->GetAura(124275))
                            if(AuraEffect* eff = staggerAura->GetEffect(EFFECT_1))
                                _amount += eff->GetAmount();
                        if (Aura* staggerAura = caster->GetAura(124274))
                            if(AuraEffect* eff = staggerAura->GetEffect(EFFECT_1))
                                _amount += eff->GetAmount();
                        if (Aura* staggerAura = caster->GetAura(124273))
                            if(AuraEffect* eff = staggerAura->GetEffect(EFFECT_1))
                                _amount += eff->GetAmount();
                        if(_amount)
                            SetHitHeal(CalculatePct(_amount, perc));
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_purified_healing_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_purified_healing_SpellScript();
        }
};

// Hurricane Strike - 152175
class spell_monk_hurricane_strike : public SpellScriptLoader
{
    public:
        spell_monk_hurricane_strike() : SpellScriptLoader("spell_monk_hurricane_strike") { }

        class spell_monk_hurricane_strike_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_hurricane_strike_AuraScript);

            uint32 update = 0;

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 158221, true);
            }

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                update += diff;

                if (update >= 140)
                {
                    if (Unit* caster = GetCaster())
                        caster->CastSpell(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 158221, true);
                    update = 0;
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_hurricane_strike_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ALLOW_ONLY_ABILITY, AURA_EFFECT_HANDLE_REAL);
                OnEffectUpdate += AuraEffectUpdateFn(spell_monk_hurricane_strike_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_ALLOW_ONLY_ABILITY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_hurricane_strike_AuraScript();
        }
};

// Breath of the Serpent - 157535
class spell_monk_breath_of_the_serpent : public SpellScriptLoader
{
    public:
        spell_monk_breath_of_the_serpent() : SpellScriptLoader("spell_monk_breath_of_the_serpent") { }

        class spell_monk_breath_of_the_serpent_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_breath_of_the_serpent_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (Unit* caster = GetCaster())
                {
                    GuidList* summonList = caster->GetSummonList(60849);
                    for (GuidList::const_iterator iter = summonList->begin(); iter != summonList->end(); ++iter)
                    {
                        if(Creature* summon = ObjectAccessor::GetCreature(*caster, (*iter)))
                            if(summon->GetExactDist2d(caster) <= 20.0f)
                                return SPELL_CAST_OK;
                    }
                    if(!summonList->empty())
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_STATUE_OUT_OF_RANGE);
                    else
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_NO_STATUE_SUMMONED);
                }
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    GuidList* summonList = caster->GetSummonList(60849);
                    for (GuidList::const_iterator iter = summonList->begin(); iter != summonList->end(); ++iter)
                    {
                        if(Creature* summon = ObjectAccessor::GetCreature(*caster, (*iter)))
                            if(summon->GetExactDist2d(caster) <= 20.0f)
                            {
                                caster->CastSpell(summon, 157636, true);
                                return;
                            }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_breath_of_the_serpent_SpellScript::CheckTarget);
                OnEffectHitTarget += SpellEffectFn(spell_monk_breath_of_the_serpent_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_breath_of_the_serpent_SpellScript();
        }
};

// Chi Explosion - 157676
class spell_monk_chi_explosion_brewmaster : public SpellScriptLoader
{
    public:
        spell_monk_chi_explosion_brewmaster() : SpellScriptLoader("spell_monk_chi_explosion_brewmaster") { }

        class spell_monk_chi_explosion_brewmaster_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_explosion_brewmaster_SpellScript);

            int32 chiCount = 0;
            int32 stagger = 0;
            int32 targetCount = 0;

            void HandleDamage1(SpellEffIndex effIndex)
            {
                SetHitDamage(GetHitDamage() * chiCount / 2);

                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                if (chiCount >= 4 && caster->HasAura(124255))
                    caster->CastSpell(target, 157679, true);
            }

            void HandleDamage2(SpellEffIndex effIndex)
            {
                if (!targetCount)
                    return;

                if(stagger)
                    SetHitDamage(int32(((GetHitDamage() / 2) + stagger) / targetCount));
                else
                    SetHitDamage(int32((GetHitDamage() / 2) / targetCount));
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    chiCount = caster->GetPower(POWER_CHI);
                    if (chiCount > 4)
                        chiCount = 4;

                    if (chiCount > 1)
                        caster->ModifyPower(POWER_CHI, -(chiCount - 1), true);

                    int32 duration = 0;
                    switch (chiCount)
                    {
                        case 4:
                            duration += 2000;
                            if (AuraEffect* effect = caster->GetAuraEffect(124255, EFFECT_1))
                                stagger = effect->GetAmount() / 2;
                        case 3:
                            duration += 2000;
                            caster->RemoveAura(124255);
                            caster->RemoveAura(SPELL_MONK_MODERATE_STAGGER);
                            caster->RemoveAura(SPELL_MONK_LIGHT_STAGGER);
                            caster->RemoveAura(SPELL_MONK_HEAVY_STAGGER);
                        case 2:
                            duration += 6000;
                    }
                    if (duration)
                    {
                        if (Aura* aura = caster->GetAura(SPELL_MONK_SHUFFLE))
                        {
                            int32 _duration = int32(aura->GetDuration() + duration);
                            aura->SetDuration(_duration);
                            if (_duration > aura->GetMaxDuration())
                                aura->SetMaxDuration(_duration);
                        }
                        else
                        {
                            if(Aura* aura = caster->AddAura(SPELL_MONK_SHUFFLE, caster))
                            {
                                aura->SetDuration(duration);
                                aura->SetMaxDuration(duration);
                            }
                        }
                    }
                }
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* caster = GetCaster())
                {
                    if(caster->GetPower(POWER_CHI) < 4 || !caster->HasAura(124255))
                        targets.clear();
                    else if(Unit* target = GetExplTargetUnit())
                        targets.remove(target);
                    targetCount = targets.size();
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_chi_explosion_brewmaster_SpellScript::HandleOnCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_brewmaster_SpellScript::HandleDamage1, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_brewmaster_SpellScript::HandleDamage2, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_explosion_brewmaster_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_explosion_brewmaster_SpellScript();
        }
};

// Chi Explosion - 157675
class spell_monk_chi_explosion_mistweaver : public SpellScriptLoader
{
    public:
        spell_monk_chi_explosion_mistweaver() : SpellScriptLoader("spell_monk_chi_explosion_mistweaver") { }

        class spell_monk_chi_explosion_mistweaver_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_explosion_mistweaver_SpellScript);

            int32 chiCount = 0;

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                caster->CastCustomSpell(target, 182078, &chiCount, &chiCount, NULL, true);
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    chiCount = caster->GetPower(POWER_CHI);
                    if (chiCount > 4)
                        chiCount = 4;

                    if (chiCount > 1)
                        caster->ModifyPower(POWER_CHI, -(chiCount - 1), true);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_chi_explosion_mistweaver_SpellScript::HandleOnCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_mistweaver_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_explosion_mistweaver_SpellScript();
        }
};

// Chi Explosion - 182078
class spell_monk_chi_explosion_mistweaver_heal : public SpellScriptLoader
{
    public:
        spell_monk_chi_explosion_mistweaver_heal() : SpellScriptLoader("spell_monk_chi_explosion_mistweaver_heal") { }

        class spell_monk_chi_explosion_mistweaver_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_explosion_mistweaver_heal_SpellScript);

            int32 baseHeal = 0;
            int32 targetCount = 0;
            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                SpellValue const* spellValue = GetSpellValue();
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target || !spellValue)
                    return;

                int32 bp0 = spellValue->EffectBasePoints[0];
                baseHeal = GetHitHeal() * bp0;
                int32 healPeriodic = int32(baseHeal / 12); // 50% for 6 tick
                switch (bp0)
                {
                    case 4:
                        caster->CastSpell(target, 157682, true);
                        caster->CastSpell(target, 157683, true);
                        caster->CastSpell(target, 157684, true);
                        caster->CastSpell(target, 157685, true);
                        caster->CastSpell(target, 157686, true);
                        caster->CastSpell(target, 157687, true);
                        caster->CastSpell(target, 157688, true);
                        caster->CastSpell(target, 157689, true);
                    case 3:
                    case 2:
                        caster->CastCustomSpell(target, 157681, &healPeriodic, NULL, NULL, true);
                }
                SetHitHeal(baseHeal);
            }

            void HandleHealAoe(SpellEffIndex /*effIndex*/)
            {
                SpellValue const* spellValue = GetSpellValue();
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target || !spellValue || !targetCount)
                    return;

                int32 heal = int32(baseHeal / targetCount);
                SetHitHeal(heal);
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if(SpellValue const* spellValue = GetSpellValue())
                {
                    if (spellValue->EffectBasePoints[1] < 3)
                        targets.clear();
                    else if(Unit* target = GetExplTargetUnit())
                        targets.remove(target);
                    targetCount = targets.size();
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_mistweaver_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_mistweaver_heal_SpellScript::HandleHealAoe, EFFECT_1, SPELL_EFFECT_HEAL);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_explosion_mistweaver_heal_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_explosion_mistweaver_heal_SpellScript();
        }
};

// Chi Explosion - 159620
class spell_monk_chi_explosion_mistweaver_talent : public SpellScriptLoader
{
    public:
        spell_monk_chi_explosion_mistweaver_talent() : SpellScriptLoader("spell_monk_chi_explosion_mistweaver_talent") { }

        class spell_monk_chi_explosion_mistweaver_talent_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_explosion_mistweaver_talent_SpellScript);

            int32 chiCount = 0;
            int32 targetCount = 0;

            void HandleDamage(SpellEffIndex effIndex)
            {
                SetHitDamage(GetHitDamage() * chiCount);
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if(chiCount < 2)
                {
                    SetHitHeal(0);
                    return;
                }
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                switch (chiCount)
                {
                    case 4:
                        caster->CastSpell(target, 157682, true);
                        caster->CastSpell(target, 157683, true);
                        caster->CastSpell(target, 157684, true);
                        caster->CastSpell(target, 157685, true);
                        caster->CastSpell(target, 157686, true);
                        caster->CastSpell(target, 157687, true);
                        caster->CastSpell(target, 157688, true);
                        caster->CastSpell(target, 157689, true);
                }
                SetHitHeal(GetHitHeal() * chiCount);
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    chiCount = caster->GetPower(POWER_CHI);
                    if (chiCount > 4)
                        chiCount = 4;

                    if (chiCount > 1)
                        caster->ModifyPower(POWER_CHI, -(chiCount - 1), true);
                }
            }

            void HandleHealAoe(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target || !targetCount)
                    return;

                SetHitHeal(int32(GetHitHeal() / targetCount));
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* caster = GetCaster())
                {
                    if(caster->GetPower(POWER_CHI) < 3)
                        targets.clear();
                    targets.remove(caster);
                    targetCount = targets.size();
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_chi_explosion_mistweaver_talent_SpellScript::HandleOnCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_mistweaver_talent_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_mistweaver_talent_SpellScript::HandleHeal, EFFECT_1, SPELL_EFFECT_HEAL);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_mistweaver_talent_SpellScript::HandleHealAoe, EFFECT_2, SPELL_EFFECT_HEAL);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_explosion_mistweaver_talent_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_explosion_mistweaver_talent_SpellScript();
        }
};

// Chi Explosion - 152174
class spell_monk_chi_explosion_windwalker : public SpellScriptLoader
{
    public:
        spell_monk_chi_explosion_windwalker() : SpellScriptLoader("spell_monk_chi_explosion_windwalker") { }

        class spell_monk_chi_explosion_windwalker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_explosion_windwalker_SpellScript);

            int32 chiCount = 0;
            int32 targetCount = 0;

            void HandleDamage(SpellEffIndex effIndex)
            {
                SetHitDamage(GetHitDamage() * chiCount / 2);

                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                if (chiCount > 1)
                {
                    int32 bp = int32(GetHitDamage() / 12);
                    caster->CastCustomSpell(target, 157680, &bp, NULL, NULL, true);
                }
            }

            void HandleDamageAoe(SpellEffIndex effIndex)
            {
                if (!targetCount)
                    return;
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();
                if (!caster || !target)
                    return;

                int32 bp = int32(GetHitDamage() / 6);
                caster->CastCustomSpell(target, 157680, &bp, NULL, NULL, true);

                SetHitDamage(int32(GetHitDamage() / targetCount));
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    chiCount = caster->GetPower(POWER_CHI);
                    if (chiCount > 4)
                        chiCount = 4;

                    if (chiCount > 1)
                        caster->ModifyPower(POWER_CHI, -(chiCount - 1), true);

                    int32 duration = 0;
                    switch (chiCount)
                    {
                        case 4:
                        case 3:
                            caster->CastSpell(caster, 125195, true);
                    }
                }
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* caster = GetCaster())
                {
                    if(caster->GetPower(POWER_CHI) < 4)
                        targets.clear();
                    else if(Unit* target = GetExplTargetUnit())
                        targets.remove(target);
                    targetCount = targets.size();
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_chi_explosion_windwalker_SpellScript::HandleOnCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_windwalker_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_explosion_windwalker_SpellScript::HandleDamageAoe, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_explosion_windwalker_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_explosion_windwalker_SpellScript();
        }
};

// Detonate Chi - 115460
class spell_monk_detonate_chi : public SpellScriptLoader
{
    public:
        spell_monk_detonate_chi() : SpellScriptLoader("spell_monk_detonate_chi") { }

        class spell_monk_detonate_chi_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_detonate_chi_SpellScript);

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<AreaTrigger*> list;
                    caster->GetAreaObjectList(list, 119031);
                    if(!list.empty())
                    {
                        for (std::list<AreaTrigger*>::iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            if(AreaTrigger* areaObj = (*itr))
                                if(areaObj->GetDuration() > 500)
                                    areaObj->SetDuration(100);
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_detonate_chi_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_detonate_chi_SpellScript();
        }
};

// Fists of Fury - 117418
class spell_monk_fists_of_fury : public SpellScriptLoader
{
    public:
        spell_monk_fists_of_fury() : SpellScriptLoader("spell_monk_fists_of_fury") { }

        class spell_monk_fists_of_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_fists_of_fury_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                    if (GetHitUnit() != caster->getVictim() && GetSpell()->GetTargetCount() > 1)
                        SetHitDamage(int32(GetHitDamage() / (GetSpell()->GetTargetCount() - 1)));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_fists_of_fury_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_fists_of_fury_SpellScript();
        }
};

void AddSC_monk_spell_scripts()
{
    new spell_monk_clone_cast();
    new spell_monk_storm_earth_and_fire_clone_visual();
    new spell_monk_storm_earth_and_fire();
    new spell_monk_fists_of_fury_stun();
    new spell_monk_renewing_mist();
    new spell_monk_diffuse_magic();
    new spell_monk_guard();
    new spell_monk_bear_hug();
    new spell_monk_zen_flight_check();
    new spell_monk_glyph_of_zen_flight();
    new spell_monk_power_strikes();
    new spell_monk_crackling_jade_lightning();
    new spell_monk_touch_of_karma();
    new spell_monk_thunder_focus_tea();
    new spell_monk_mana_tea();
    new spell_monk_mana_tea_stacks();
    new spell_monk_enveloping_mist();
    new spell_monk_surging_mist();
    new spell_monk_zen_sphere();
    new spell_monk_chi_burst();
    new spell_monk_energizing_brew();
    new spell_monk_spear_hand_strike();
    new spell_monk_flying_serpent_kick();
    new spell_monk_chi_torpedo();
    new spell_monk_purifying_brew();
    new spell_monk_clash();
    new spell_monk_keg_smash();
    new spell_monk_elusive_brew();
    new spell_monk_breath_of_fire();
    new spell_monk_soothing_mist();
    new spell_monk_zen_pilgrimage();
    new spell_monk_blackout_kick();
    new spell_monk_fortifying_brew();
    new spell_monk_provoke();
    new spell_monk_roll();
    new spell_mastery_bottled_fury();
    new spell_monk_remove_zen_flight();
    new spell_monk_spinning_crane_kick();
    new spell_monk_transcendence();
    new spell_monk_transcendence_transfer();
    new spell_monk_charging_ox_wave();
    new spell_monk_renewing_mist_selector();
    new spell_monk_renewing_mist_start();
    new spell_monk_healing_sphere_despawn();
    new spell_monk_eminence();
    new spell_monk_guard_ox();
    new spell_monk_touch_of_death();
    new spell_monk_dampen_harm();
    new spell_monk_leer_of_the_ox();
    new spell_monk_clash_stun();
    new spell_monk_mana_tea_glyph();
    new spell_monk_glyph_of_paralysis();
    new spell_monk_glyph_of_surging_mist();
    new spell_monk_glyph_of_targeted_expulsion();
    new spell_monk_chi_wave_filter();
    new spell_monk_chi_wave();
    new spell_monk_chi_wave_dummy();
    new spell_monk_disable();
    new spell_monk_purified_healing();
    new spell_monk_hurricane_strike();
    new spell_monk_breath_of_the_serpent();
    new spell_monk_chi_explosion_brewmaster();
    new spell_monk_chi_explosion_mistweaver();
    new spell_monk_chi_explosion_mistweaver_heal();
    new spell_monk_chi_explosion_mistweaver_talent();
    new spell_monk_chi_explosion_windwalker();
    new spell_monk_detonate_chi();
    new spell_monk_fists_of_fury();
}
