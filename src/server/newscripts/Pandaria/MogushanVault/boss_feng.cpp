/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "mogu_shan_vault.h"

enum eSpells
{
    // Shared
    SPELL_SPIRIT_BOLT                   = 118530,
    SPELL_STRENGHT_OF_SPIRIT            = 116363,
    SPELL_DRAW_ESSENCE                  = 121631,

    // Visuals
    SPELL_SPIRIT_FIST                   = 115743,
    SPELL_SPIRIT_SPEAR                  = 115740,
    SPELL_SPIRIT_STAFF                  = 115742,
    SPELL_SPIRIT_SHIELD                 = 115741,

    // Spirit of the Fist
    SPELL_LIGHTNING_LASH                = 131788,
    SPELL_LIGHTNING_FISTS               = 116157,
    SPELL_EPICENTER                     = 116018,

    // Spirit of the Spear
    SPELL_FLAMING_SPEAR                 = 116942,
    SPELL_WILDFIRE_SPARK                = 116784,
    SPELL_DRAW_FLAME                    = 116711,
    SPELL_WILDFIRE_INFUSION             = 116817,

    // Spirit of the Staff 
    SPELL_ARCANE_SHOCK                  = 131790,
    SPELL_ARCANE_VELOCITY               = 116364,
    SPELL_ARCANE_RESONANCE              = 116417,

    // Spirit of the Shield ( Heroic )
    SPELL_SHADOWBURN                    = 17877,
    SPELL_SIPHONING_SHIELD              = 118071,
    SPELL_CHAINS_OF_SHADOW              = 118783,

    // Stolen Essences of Stone
    SPELL_NULLIFICATION_BARRIER         = 115817,
    SPELL_SHROUD_OF_REVERSAL            = 115911,

    // Controler Visual
    SPELL_VISUAL_FIST                   = 105032,
    SPELL_VISUAL_SPEAR                  = 118485,
    SPELL_VISUAL_STAFF                  = 118486,
    SPELL_VISUAL_SHIELD                 = 117303,

    // Inversions Spell
    SPELL_INVERSION                     = 115972,

    SPELL_EPICENTER_INVERSION           = 118300,
    SPELL_LIGHTNING_FISTS_INVERSION     = 118302,
    SPELL_ARCANE_RESONANCE_INVERSION    = 118304,
    SPELL_ARCANE_VELOCITY_INVERSION     = 118305,
    SPELL_WILDFIRE_SPARK_INVERSION      = 118307,
    SPELL_FLAMING_SPEAR_INVERSION       = 118308,
    // Inversion bouclier siphon        = 118471,
    SPELL_SHADOWBURN_INVERSION          = 132296,
    SPELL_LIGHTNING_LASH_INVERSION      = 132297,
    SPELL_ARCANE_SHOCK_INVERSION        = 132298
};

enum eSummon
{
    NPC_LIGHTNING_FISTS         = 60241,
};

enum eEvents
{
    EVENT_DOT_ATTACK            = 1,
    EVENT_RE_ATTACK             = 2,

    EVENT_LIGHTNING_FISTS       = 3,
    EVENT_EPICENTER             = 4,

    EVENT_WILDFIRE_SPARK        = 5,
    EVENT_DRAW_FLAME            = 6,

    EVENT_ARCANE_VELOCITY       = 7,
    EVENT_ARCANE_VELOCITY_END   = 8,
    EVENT_ARCANE_RESONANCE      = 9,
};

enum eFengPhases
{
    PHASE_NONE                  = 0,
    PHASE_FIST                  = 1,
    PHASE_SPEAR                 = 2,
    PHASE_STAFF                 = 3,
    PHASE_SHIELD                = 4
};

Position modPhasePositions[4] =
{
    {4063.26f, 1320.50f, 466.30f, 5.5014f}, // Phase Fist
    {4021.17f, 1320.50f, 466.30f, 3.9306f}, // Phase Spear
    {4021.17f, 1362.80f, 466.30f, 2.0378f}, // Phase Staff
    {4063.26f, 1362.80f, 466.30f, 0.7772f}, // Phase Shield
};

uint32 statueEntryInOrder[4] = {GOB_FIST_STATUE,   GOB_SPEAR_STATUE,   GOB_STAFF_STATUE,   GOB_SHIELD_STATUE};
uint32 controlerVisualId[4]  = {SPELL_VISUAL_FIST, SPELL_VISUAL_SPEAR, SPELL_VISUAL_STAFF, SPELL_VISUAL_SHIELD};
uint32 fengVisualId[4]       = {SPELL_SPIRIT_FIST, SPELL_SPIRIT_SPEAR, SPELL_SPIRIT_STAFF, SPELL_SPIRIT_SHIELD};

#define MAX_INVERSION_SPELLS    9
uint32 inversionMatching[MAX_INVERSION_SPELLS][2] =
{
    {SPELL_EPICENTER,        SPELL_EPICENTER_INVERSION},
    {SPELL_LIGHTNING_FISTS,  SPELL_LIGHTNING_FISTS_INVERSION},
    {SPELL_ARCANE_RESONANCE, SPELL_ARCANE_RESONANCE_INVERSION},
    {SPELL_ARCANE_VELOCITY,  SPELL_ARCANE_VELOCITY_INVERSION},
    {SPELL_WILDFIRE_SPARK,   SPELL_WILDFIRE_SPARK_INVERSION},
    {SPELL_FLAMING_SPEAR,    SPELL_FLAMING_SPEAR_INVERSION},
    {SPELL_SHADOWBURN,       SPELL_SHADOWBURN_INVERSION},
    {SPELL_LIGHTNING_LASH,   SPELL_LIGHTNING_LASH_INVERSION},
    {SPELL_ARCANE_SHOCK,     SPELL_ARCANE_SHOCK_INVERSION}
};

#define MAX_DIST    60

float const minpullpos = 4007.9379f;
float const maxpullpos = 4076.9135f;

class boss_feng : public CreatureScript
{
    public:
        boss_feng() : CreatureScript("boss_feng") {}

        struct boss_fengAI : public BossAI
        {
            boss_fengAI(Creature* creature) : BossAI(creature, DATA_FENG)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            bool phaseone, phasetwo, phasethree;
            uint8 newphase;
            uint8 actualPhase;
            uint32 dotSpellId, checkvictim;
            ObjectGuid targetGuid;

            void Reset()
            {
                _Reset();
                phaseone = false;
                phasetwo = false;
                phasethree = false;
                checkvictim = 0; 
                newphase = 0;
                actualPhase = PHASE_NONE;
                dotSpellId = 0;
                targetGuid.Clear();
                instance->DoRemoveAurasDueToSpellOnPlayers(115811);
                instance->DoRemoveAurasDueToSpellOnPlayers(115972);

                for (uint8 i = 0; i < 4; ++i)
                    me->RemoveAurasDueToSpell(fengVisualId[i]);

                if (GameObject* oldStatue = instance->instance->GetGameObject(instance->GetGuidData(statueEntryInOrder[actualPhase - 1])))
                {
                    oldStatue->SetLootState(GO_READY);
                    oldStatue->UseDoorOrButton();
                }

                if (GameObject* inversionGob = instance->instance->GetGameObject(instance->GetGuidData(GOB_INVERSION)))
                    inversionGob->Respawn();

                if (GameObject* cancelGob = instance->instance->GetGameObject(instance->GetGuidData(GOB_CANCEL)))
                    cancelGob->Respawn();
            }

            void JustReachedHome()
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            bool CheckPullPlayerPos(Unit* who)
            {
                if (!who->ToPlayer() || who->GetPositionX() < minpullpos || who->GetPositionX() > maxpullpos)
                    return false;

                return true;
            }
            
            void EnterCombat(Unit* who)
            {
                if (instance)
                {
                    if (!CheckPullPlayerPos(who))
                    {
                        EnterEvadeMode();
                        return;
                    }
                }
              _EnterCombat();
              checkvictim = 1500;
            }

            void JustDied(Unit* attacker)
            {
                _JustDied();
                instance->DoRemoveAurasDueToSpellOnPlayers(115811);
                instance->DoRemoveAurasDueToSpellOnPlayers(115972);

                if (GameObject* inversionGob = instance->instance->GetGameObject(instance->GetGuidData(GOB_INVERSION)))
                    inversionGob->Delete();

                if (GameObject* cancelGob = instance->instance->GetGameObject(instance->GetGuidData(GOB_CANCEL)))
                    cancelGob->Delete();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id >= 1 && id <= 4)
                    PrepareNewPhase(id);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SPARK)
                    if (Aura* aura = me->GetAura(SPELL_WILDFIRE_INFUSION))
                        aura->ModCharges(1);
                    else
                        me->AddAura(SPELL_WILDFIRE_INFUSION, me);
            }

            void PrepareNewPhase(uint8 newPhase)
            {
                events.Reset();
                events.ScheduleEvent(EVENT_DOT_ATTACK, 15000);
                events.ScheduleEvent(EVENT_RE_ATTACK,  1000);
                me->GetMotionMaster()->Clear();

                if (Creature* controler = me->FindNearestCreature(NPC_PHASE_CONTROLER, 60.0f, true))
                {
                    controler->RemoveAllAuras();
                    controler->DespawnOrUnsummon();
                }

                // Desactivate old statue and enable the new one
                if (GameObject* oldStatue = instance->instance->GetGameObject(instance->GetGuidData(statueEntryInOrder[actualPhase - 1])))
                {
                    oldStatue->SetLootState(GO_READY);
                    oldStatue->UseDoorOrButton();
                }

                if (GameObject* newStatue = instance->instance->GetGameObject(instance->GetGuidData(statueEntryInOrder[newPhase - 1])))
                {
                    newStatue->SetLootState(GO_READY);
                    newStatue->UseDoorOrButton();
                }

                for (uint8 i = 0; i < 4; ++i)
                    me->RemoveAurasDueToSpell(fengVisualId[i]);

                //me->AddAura(fengVisualId[newPhase - 1], me);
                me->CastSpell(me, SPELL_DRAW_ESSENCE, true);

                switch (newPhase)
                {
                    case PHASE_FIST:
                    {
                        dotSpellId = SPELL_LIGHTNING_LASH;
                        events.ScheduleEvent(EVENT_LIGHTNING_FISTS,  20000, PHASE_FIST);
                        events.ScheduleEvent(EVENT_EPICENTER,        35000, PHASE_FIST);
                        break;
                    }
                    case PHASE_SPEAR:
                    {
                        dotSpellId = SPELL_FLAMING_SPEAR;
                        events.ScheduleEvent(EVENT_WILDFIRE_SPARK,   35000, PHASE_SPEAR);
                        events.ScheduleEvent(EVENT_DRAW_FLAME,       40000, PHASE_SPEAR);
                        break;
                    }
                    case PHASE_STAFF:
                    {
                        dotSpellId = SPELL_ARCANE_SHOCK;
                        events.ScheduleEvent(EVENT_ARCANE_VELOCITY,  25000, PHASE_STAFF);
                        events.ScheduleEvent(EVENT_ARCANE_RESONANCE, 10000, PHASE_STAFF);
                        break;
                    }
                    case PHASE_SHIELD:
                    {
                        dotSpellId = SPELL_SHADOWBURN;
                        // Todo
                        break;
                    }
                    default:
                        break;
                }

                actualPhase = newPhase;
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (!instance)
                    return;

                if (HealthBelowPct(95) && !phaseone)
                {
                    phaseone = true;
                    newphase = 1;
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    if (Creature* controler = me->SummonCreature(NPC_PHASE_CONTROLER, modPhasePositions[newphase - 1].GetPositionX(), modPhasePositions[newphase - 1].GetPositionY(), modPhasePositions[newphase - 1].GetPositionZ()))
                        controler->AddAura(controlerVisualId[newphase - 1], controler);
                    me->GetMotionMaster()->MovePoint(newphase, modPhasePositions[newphase - 1].GetPositionX(), modPhasePositions[newphase - 1].GetPositionY(), modPhasePositions[newphase - 1].GetPositionZ());
                }
                else if (HealthBelowPct(63) && !phasetwo)
                {
                    phasetwo = true;
                    newphase = 2;
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    if (Creature* controler = me->SummonCreature(NPC_PHASE_CONTROLER, modPhasePositions[newphase - 1].GetPositionX(), modPhasePositions[newphase - 1].GetPositionY(), modPhasePositions[newphase - 1].GetPositionZ()))
                        controler->AddAura(controlerVisualId[newphase - 1], controler);
                    me->GetMotionMaster()->MovePoint(newphase, modPhasePositions[newphase - 1].GetPositionX(), modPhasePositions[newphase - 1].GetPositionY(), modPhasePositions[newphase - 1].GetPositionZ());
                }
                else if (HealthBelowPct(31) && !phasethree)
                {
                    phasethree = true;
                    newphase = 3;
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    if (Creature* controler = me->SummonCreature(NPC_PHASE_CONTROLER, modPhasePositions[newphase - 1].GetPositionX(), modPhasePositions[newphase - 1].GetPositionY(), modPhasePositions[newphase - 1].GetPositionZ()))
                        controler->AddAura(controlerVisualId[newphase - 1], controler);
                    me->GetMotionMaster()->MovePoint(newphase, modPhasePositions[newphase - 1].GetPositionX(), modPhasePositions[newphase - 1].GetPositionY(), modPhasePositions[newphase - 1].GetPositionZ());
                }
            }

            void JustSummoned(Creature* sum)
            {
                if (sum->GetEntry() == NPC_LIGHTNING_FISTS)
                {
                    if (Unit* pl = me->GetPlayer(*me, targetGuid))
                    {
                        sum->AI()->SetGUID(targetGuid); 
                        targetGuid.Clear();
                    }
                }
            }

            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (checkvictim && instance)
                {
                    if (checkvictim <= diff)
                    {
                        if (me->getVictim())
                        {
                            if (!CheckPullPlayerPos(me->getVictim()))
                            {
                                me->AttackStop();
                                me->SetReactState(REACT_PASSIVE);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                EnterEvadeMode();
                                checkvictim = 0;
                            }
                            else
                                checkvictim = 1500;
                        }
                    }
                    else
                        checkvictim -= diff;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                
                events.Update(diff);

                switch(events.ExecuteEvent())
                {
                    // All Phases
                    case EVENT_DOT_ATTACK:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            DoCast(target, dotSpellId);
                        events.ScheduleEvent(EVENT_DOT_ATTACK, 12500);
                        break;
                     case EVENT_RE_ATTACK:
                         if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                             me->GetMotionMaster()->MoveChase(target);
                         me->SetReactState(REACT_AGGRESSIVE);
                         break;
                     // Fist Phase
                    case EVENT_LIGHTNING_FISTS:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 30.0f, true))
                        {
                            if (targetGuid)
                                targetGuid.Clear();
                            targetGuid = target->GetGUID();
                            DoCast(target, SPELL_LIGHTNING_FISTS);
                        }
                        events.ScheduleEvent(EVENT_LIGHTNING_FISTS, 20000);
                        break;
                    case EVENT_EPICENTER:
                        DoCast(me, SPELL_EPICENTER);
                        events.ScheduleEvent(EVENT_EPICENTER, 30000);
                        break;
                    // Spear Phase
                    case EVENT_WILDFIRE_SPARK:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_WILDFIRE_SPARK);
                        events.ScheduleEvent(EVENT_WILDFIRE_SPARK, urand(25000, 35000));
                        break;
                    case EVENT_DRAW_FLAME: 
                        DoCast(me, SPELL_DRAW_FLAME);
                        events.ScheduleEvent(EVENT_DRAW_FLAME, 60000);
                        break;
                    // Staff Phase
                    case EVENT_ARCANE_VELOCITY:
                        DoCast(me, SPELL_ARCANE_VELOCITY);
                        events.ScheduleEvent(EVENT_ARCANE_VELOCITY, 15000);
                        break;
                    case EVENT_ARCANE_RESONANCE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 30.0f, true ))
                            DoCast(target, SPELL_ARCANE_RESONANCE);
                        events.ScheduleEvent(EVENT_ARCANE_RESONANCE, 20000);
                        break;
                    
                    // Shield Phase : TODO
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_fengAI(creature);
        }
};

enum eLightningFistSpell
{
    SPELL_FIST_BARRIER      = 115856,
    SPELL_FIST_CHARGE       = 116374,
    SPELL_FIST_VISUAL       = 116225,
    SPELL_AURA_SEARCHER     = 129426,
    SPELL_SEARCHER          = 129428
};

class mob_lightning_fist : public CreatureScript
{
    public:
        mob_lightning_fist() : CreatureScript("mob_lightning_fist") {}

        struct mob_lightning_fistAI : public ScriptedAI
        {
            mob_lightning_fistAI(Creature* creature) : ScriptedAI(creature)
            {
                InstanceScript* instance = creature->GetInstanceScript();
                me->SetReactState(REACT_PASSIVE);
            }

            InstanceScript* instance;

            uint32 unsummon;
            
            void Reset()
            {
                unsummon = 6000;
            }

            void SetGUID(ObjectGuid const& plguid, int32 type/* = 0 */)
            {
                if (plguid)
                {
                    if (Unit* target = me->GetPlayer(*me, plguid))
                    {
                        if (target->isAlive())
                        {
                            me->SetFacingToObject(target);
                            me->AddAura(SPELL_AURA_SEARCHER, me);
                            me->AddAura(SPELL_FIST_VISUAL, me);
                            float x = 0, y = 0;
                            GetPositionWithDistInOrientation(me, 100.0f, me->GetOrientation(), x, y);
                            me->GetMotionMaster()->MoveCharge(x, y, me->GetPositionZ(), 24.0f, 1);
                            return;
                        }
                    }
                }
                me->DespawnOrUnsummon();
            }

            void EnterCombat(Unit* who){}

            void EnterEvadeMode(){}

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (target->GetTypeId() == TYPEID_PLAYER && spell->Id == SPELL_SEARCHER)
                    DoCast(target, SPELL_FIST_CHARGE);
            }

            void UpdateAI(uint32 diff)
            {
                if (unsummon)
                {
                    if (unsummon <= diff)
                        me->DespawnOrUnsummon();
                    else
                        unsummon -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lightning_fistAI(creature);
        }
};

class mob_wild_spark : public CreatureScript
{
    public:
        mob_wild_spark() : CreatureScript("mob_wild_spark") {}

        struct mob_wild_sparkAI : public ScriptedAI
        {
            mob_wild_sparkAI(Creature* creature) : ScriptedAI(creature)
            {}

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, 116717, true); // Fire aura
            }
    
            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_DRAW_FLAME)
                    me->GetMotionMaster()->MovePoint(1, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ());
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == 1)
                    if (InstanceScript* instance = me->GetInstanceScript())
                        if (Creature* feng = instance->instance->GetCreature(instance->GetGuidData(NPC_FENG)))
                        {
                            feng->AI()->DoAction(ACTION_SPARK);
                            me->DespawnOrUnsummon();
                        }
            }

            void UpdateAI(uint32 diff)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_wild_sparkAI(creature);
        }
};

// Mogu Epicenter - 116040
class spell_mogu_epicenter : public SpellScriptLoader
{
    public:
        spell_mogu_epicenter() : SpellScriptLoader("spell_mogu_epicenter") { }

        class spell_mogu_epicenter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mogu_epicenter_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;
                
                float distance = caster->GetExactDist2d(target);

                if (distance >= 0 && distance <= 60)
                    SetHitDamage(GetHitDamage() * (1 - (distance / MAX_DIST)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mogu_epicenter_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mogu_epicenter_SpellScript();
        }
};

// Wildfire Spark - 116583
/*class spell_mogu_wildfire_spark : public SpellScriptLoader
{
    public:
        spell_mogu_wildfire_spark() : SpellScriptLoader("spell_mogu_wildfire_spark") { }

        class spell_mogu_wildfire_spark_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mogu_wildfire_spark_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                const uint8 maxSpark = 3;

                Unit* caster = GetCaster();

                if (!caster)
                    return;

                for (uint8 i = 0; i < maxSpark; ++i)
                {
                    float position_x = caster->GetPositionX() + frand(-3.0f, 3.0f);
                    float position_y = caster->GetPositionY() + frand(-3.0f, 3.0f);
                    caster->CastSpell(position_x, position_y, caster->GetPositionZ(), 116586, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_mogu_wildfire_spark_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mogu_wildfire_spark_SpellScript();
        }
};*/

// Wildfire Infusion - 116816
class spell_mogu_wildfire_infusion : public SpellScriptLoader
{
    public:
        spell_mogu_wildfire_infusion() : SpellScriptLoader("spell_mogu_wildfire_infusion") { }

        class spell_mogu_wildfire_infusion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mogu_wildfire_infusion_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    if (Aura* aura = caster->GetAura(SPELL_WILDFIRE_INFUSION))
                        aura->ModCharges(-1);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_mogu_wildfire_infusion_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mogu_wildfire_infusion_SpellScript();
        }
};

// Arcane Velocity - 116365
class spell_mogu_arcane_velocity : public SpellScriptLoader
{
    public:
        spell_mogu_arcane_velocity() : SpellScriptLoader("spell_mogu_arcane_velocity") { }

        class spell_mogu_arcane_velocity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mogu_arcane_velocity_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;
                
                float distance = caster->GetExactDist2d(target);

                if (distance >= 0 && distance <= 60)
                    SetHitDamage(GetHitDamage() * (distance / MAX_DIST));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mogu_arcane_velocity_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mogu_arcane_velocity_SpellScript();
        }
};

// Arcane Resonance - 116434
/*class spell_mogu_arcane_resonance : public SpellScriptLoader
{
    public:
        spell_mogu_arcane_resonance() : SpellScriptLoader("spell_mogu_arcane_resonance") { }

        class spell_mogu_arcane_resonance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mogu_arcane_resonance_SpellScript);

            uint8 targetCount;

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targetCount = targets.size();
            }

            void DealDamage()
            {
                if (targetCount > 25)
                    targetCount = 0;

                SetHitDamage(GetHitDamage() * targetCount);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mogu_arcane_resonance_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnHit                    += SpellHitFn(spell_mogu_arcane_resonance_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mogu_arcane_resonance_SpellScript();
        }
};*/

// Mogu Inversion - 118300 / 118302 / 118304 / 118305 / 118307 / 118308 / 132296 / 132297 / 132298
class spell_mogu_inversion : public SpellScriptLoader
{
    public:
        spell_mogu_inversion() : SpellScriptLoader("spell_mogu_inversion") { }

        class spell_mogu_inversion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mogu_inversion_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget())
                    GetTarget()->RemoveAurasDueToSpell(SPELL_INVERSION);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget())
                    GetTarget()->CastSpell(GetTarget(), SPELL_INVERSION, true);
            }

            void Register()
            {
                OnEffectApply     += AuraEffectApplyFn(spell_mogu_inversion_AuraScript::OnApply,   EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_mogu_inversion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mogu_inversion_AuraScript();
        }
};

void AddSC_boss_feng()
{
    new boss_feng();
    new mob_lightning_fist();
    new mob_wild_spark();
    new spell_mogu_epicenter();
    //new spell_mogu_wildfire_spark();
    new spell_mogu_wildfire_infusion();
    new spell_mogu_arcane_velocity();
    //new spell_mogu_arcane_resonance();
    new spell_mogu_inversion();
}
