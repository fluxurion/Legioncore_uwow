#include"the_vortex_pinnacle.h"

enum ScriptTexts
{
    SAY_AGGRO    = 0,
    SAY_KILL    = 1,
    SAY_SPELL    = 2,
    SAY_DEATH    = 3,
};

enum Spells
{
    SPELL_CHAIN_LIGHTNING                    = 87622,
    SPELL_CHAIN_LIGHTNING_H                    = 93993,
    SPELL_UNSTABLE_GROUNDING_FIELD            = 86911,
    SPELL_GROUNDING_FIELD_VISUAL_BEAMS        = 87517,

    SPELL_SUPREMACY_OF_THE_STORM            = 86930,
    
    SPELL_SUPREMACY_OF_THE_STORM_DUMMY        = 86715,
    SPELL_SUPREMACY_OF_THE_STORM_DMG        = 87553,
    SPELL_SUPREMACY_OF_THE_STORM_DMG_H        = 93994,

    SPELL_SUPREMACY_OF_THE_STORM_DUMMY_1    = 87521,
    SPELL_SUPREMACY_OF_THE_STORM_SUM        = 87518,
    SPELL_SUPREMACY_OF_THE_STORM_TELE        = 87328,

    SPELL_STATIC_CLING                        = 87618,
};

enum Events
{
    EVENT_FIELD                = 1,
    EVENT_FIELD_1            = 2,
    EVENT_FIELD_2            = 3,
    EVENT_FIELD_3            = 4,
    EVENT_FIELD_4            = 5,
    EVENT_FIELD_5            = 6,
    EVENT_FIELD_6            = 7,
    EVENT_CHAIN_LIGHTNING    = 8,
    EVENT_STATIC_CLING        = 9,
    EVENT_SUMMON_STAR        = 10,
};

enum Adds
{
    NPC_UNSTABLE_GROUNDING_FIELD    = 46492,
    NPC_SKYFALL_STAR                = 52019,
    NPC_STORM_TARGET                = 46387,
    NPC_GROUNDING_FIELD                = 47000,
};

const Position fieldPos[4]    =
{
    {-644.20f, 489.00f, 646.63f, 0.0f},
    {-638.38f, 480.68f, 646.63f, 0.0f},
    {-635.43f, 492.11f, 646.63f, 0.0f},
    {-639.23f, 488.13f, 656.63f, 0.0f},
};

const Position starPos[6] = 
{
    {-583.77f, 516.56f, 649.51f, 5.65f},
    {-591.65f, 476.39f, 649.19f, 4.39f},
    {-617.69f, 544.79f, 650.12f, 0.11f},
    {-652.62f, 532.48f, 649.03f, 1.53f},
    {-618.65f, 463.05f, 650.63f, 0.0f},
    {-649.24f, 474.11f, 649.63f, 0.0f},
};

class boss_asaad : public CreatureScript
{
    public:
        boss_asaad() : CreatureScript("boss_asaad") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return GetAIForInstance <boss_asaadAI>(pCreature, VPScriptName);
        }
        struct boss_asaadAI : public BossAI
        {
            boss_asaadAI(Creature* pCreature) : BossAI(pCreature, DATA_ASAAD)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
            }

            bool bField;
            Creature* _field1;
            Creature* _field2;
            Creature* _field3;

            void Reset()
            {
                _Reset();
                me->SetCanFly(false);
                bField = false;
            }
    
            void EnterCombat(Unit* /*pWho*/)
            {
                events.ScheduleEvent(EVENT_SUMMON_STAR, urand(10000, 20000));
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(5000, 10000));
                events.ScheduleEvent(EVENT_FIELD, 45000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_STATIC_CLING, urand(12000, 18000));
                Talk(SAY_AGGRO);
                bField = false;
                DoZoneInCombat();
                instance->SetBossState(DATA_ASAAD, IN_PROGRESS);
            }    
            void KilledUnit(Unit* killer)
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* pWho)
            {
                _JustDied();
                me->SetCanFly(false);
                Talk(SAY_DEATH);
            }
            

            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) && !bField)
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_CHAIN_LIGHTNING);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(15000, 20000));
                            break;
                        case EVENT_SUMMON_STAR:
                            me->SummonCreature(NPC_SKYFALL_STAR, starPos[urand(0, 5)]);
                            events.ScheduleEvent(EVENT_SUMMON_STAR, urand(24000, 30000));
                            break;
                        case EVENT_STATIC_CLING:
                            DoCast(SPELL_STATIC_CLING);
                            events.ScheduleEvent(EVENT_STATIC_CLING, urand(20000, 23000));
                            break;
                        case EVENT_FIELD:
                            bField = true;
                            events.CancelEvent(EVENT_CHAIN_LIGHTNING);
                            events.CancelEvent(EVENT_STATIC_CLING);
                            events.CancelEvent(EVENT_SUMMON_STAR);
                            me->InterruptNonMeleeSpells(false);
                            Talk(SAY_SPELL);
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            if (_field1 = me->SummonCreature(NPC_UNSTABLE_GROUNDING_FIELD, fieldPos[0]))
                                DoCast(_field1, SPELL_GROUNDING_FIELD_VISUAL_BEAMS);
                            events.ScheduleEvent(EVENT_FIELD_4, 20000);
                            events.ScheduleEvent(EVENT_FIELD_1, 6000); 
                            break;
                        case EVENT_FIELD_1:
                            if (_field2 = me->SummonCreature(NPC_UNSTABLE_GROUNDING_FIELD, fieldPos[1]))
                                if (_field1)
                                    _field1->CastSpell(_field2, SPELL_GROUNDING_FIELD_VISUAL_BEAMS, true);
                            events.ScheduleEvent(EVENT_FIELD_2, 6000);
                            break;
                        case EVENT_FIELD_2:
                            if (_field3 = me->SummonCreature(NPC_UNSTABLE_GROUNDING_FIELD, fieldPos[2]))
                                if (_field2)
                                    _field2->CastSpell(_field3, SPELL_GROUNDING_FIELD_VISUAL_BEAMS, true);
                            events.ScheduleEvent(EVENT_FIELD_3, 6000);
                            break;
                        case EVENT_FIELD_3:
                            if (_field3 && _field1)
                                _field3->CastSpell(_field1, SPELL_GROUNDING_FIELD_VISUAL_BEAMS, true);
                            break;
                        case EVENT_FIELD_4:
                            me->SetCanFly(true);
                            me->SetDisableGravity(true);
                            me->NearTeleportTo(
                                fieldPos[3].GetPositionX(),
                                fieldPos[3].GetPositionY(),
                                fieldPos[3].GetPositionZ(),
                                0.0f);
                            events.ScheduleEvent(EVENT_FIELD_5, 800);
                            break;
                        case EVENT_FIELD_5:
                            DoCast(SPELL_SUPREMACY_OF_THE_STORM);
                            events.ScheduleEvent(EVENT_FIELD_6, 6000);
                            break;
                        case EVENT_FIELD_6:
                            if (_field1)
                                _field1->DespawnOrUnsummon();
                            if (_field2)
                                _field2->DespawnOrUnsummon();
                            if (_field3)
                                _field3->DespawnOrUnsummon();
                            bField = false;
                            me->SetCanFly(false);
                            me->SetDisableGravity(false);;
                            me->SetReactState(REACT_AGGRESSIVE);
                            AttackStart(me->getVictim());
                            events.ScheduleEvent(EVENT_FIELD, 45000);
                            events.ScheduleEvent(EVENT_SUMMON_STAR, urand(10000, 20000));
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(5000, 10000));
                            if (IsHeroic())
                                events.ScheduleEvent(EVENT_STATIC_CLING, urand(12000, 18000));
                            break;
                        }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_unstable_grounding_field : public CreatureScript
{
    public:
        npc_unstable_grounding_field() : CreatureScript("npc_unstable_grounding_field") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_unstable_grounding_fieldAI(pCreature);
        }
        struct npc_unstable_grounding_fieldAI : public Scripted_NoMovementAI
        {
            npc_unstable_grounding_fieldAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
            }

            void UpdateAI(uint32 diff)
            {
            }
     };
};

class spell_asaad_supremacy_of_the_storm : public SpellScriptLoader
{
    public:
        spell_asaad_supremacy_of_the_storm() : SpellScriptLoader("spell_asaad_supremacy_of_the_storm") { }


        class spell_asaad_supremacy_of_the_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_asaad_supremacy_of_the_storm_SpellScript);


            void HandleScript()
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (GetCaster()->GetDistance2d(GetHitUnit()) < 5.0f)
                    SetHitDamage(0);
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_asaad_supremacy_of_the_storm_SpellScript::HandleScript/*, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE*/);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_asaad_supremacy_of_the_storm_SpellScript();
        }
};

class spell_unstable_grounding_field : public SpellScriptLoader
{
    public:
        spell_unstable_grounding_field() : SpellScriptLoader("spell_unstable_grounding_field") { }

        class spell_unstable_grounding_field_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_unstable_grounding_field_SpellScript);

            void FilterTargets1(std::list<WorldObject*>& unitList)
            {
                /*for (std::list<WorldObject*>::iterator itr = unitList.begin() ; itr != unitList.end();)
                {
                    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "spell_unstable_grounding_field TargetA name %s, entry %u", (*itr)->GetName().c_str(), (*itr)->GetEntry());
                    itr++;
                }*/
            }

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                /*for (std::list<WorldObject*>::iterator itr = unitList.begin() ; itr != unitList.end();)
                {
                    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "spell_unstable_grounding_field TargetB name %s, entry %u", (*itr)->GetName().c_str(), (*itr)->GetEntry());
                    itr++;
                }*/
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_unstable_grounding_field_SpellScript::FilterTargets1, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
                //OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_unstable_grounding_field_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_unstable_grounding_field_SpellScript();
        }
};

void AddSC_boss_asaad()
{
    new boss_asaad();
    new npc_unstable_grounding_field();
    new spell_asaad_supremacy_of_the_storm();
    new spell_unstable_grounding_field();
}