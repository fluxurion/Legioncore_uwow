/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
    Encounter: Fenryr
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_UNNERVING_HOWL        = 196543,
    SPELL_RAVENOUS_LEAP         = 197558,
    SPELL_RAVENOUS_LEAP_MARK    = 197549,
    SPELL_RAVENOUS_LEAP_T_1     = 197560, //Поиск цели
    SPELL_RAVENOUS_LEAP_JUMP    = 196495,
    SPELL_RAVENOUS_LEAP_DOT     = 196497,
    SPELL_RAVENOUS_LEAP_AOE     = 196496,
    SPELL_CLAW_FRENZY           = 196512,
    SPELL_SCENT_OF_BLOOD        = 198800,

    //Cosmetic
    SPELL_STEALTH               = 196567,
    SPELL_LICKING_WOUNDS        = 200561,
    SPELL_BLOOD_SPLAT           = 200553,

    //Conversation
    SPELL_ODYN_HUNTING_GROUND   = 202153,
};

enum eEvents
{
    EVENT_UNNERVING_HOWL        = 1,
    EVENT_RAVENOUS_LEAP         = 2,
    EVENT_CLAW_FRENZY           = 3,
    EVENT_SCENT_OF_BLOOD        = 4,

    //Other
    EVENT_LOW_HP_MOVE_LAIR      = 5,
};

Position const oneTrashPos[12] =
{
    {3006.39f, 2980.55f, 618.48f, 2.26f}, //npc 95674
    {3075.17f, 3008.41f, 609.65f, 5.10f}, //go 246272
    {3088.34f, 2989.26f, 614.69f, 5.63f},
    {3016.72f, 3009.90f, 609.72f, 0.35f},
    {3036.83f, 3021.59f, 607.72f, 0.31f},
    {3066.19f, 3016.52f, 609.92f, 5.52f},
    {2999.78f, 2996.95f, 615.46f, 1.04f},
    {3048.63f, 3021.88f, 607.67f, 0.19f},
    {3078.62f, 2995.89f, 612.31f, 5.37f},
    {3029.34f, 3013.41f, 608.78f, 0.58f},
    {3058.14f, 3023.87f, 607.94f, 6.06f},
    {3009.28f, 3003.14f, 612.59f, 0.64f}
};

Position const secondTrashPos[13] =
{
    {3154.69f, 3166.24f, 593.60f, 3.29f}, //npc 95674
    {3130.42f, 3104.78f, 597.30f, 4.24f}, //go 246272
    {3088.16f, 2989.61f, 614.72f, 5.46f},
    {3078.62f, 2996.61f, 612.20f, 5.23f},
    {3076.25f, 3009.70f, 609.88f, 4.60f},
    {3075.56f, 3029.13f, 606.97f, 4.30f},
    {3093.55f, 3041.78f, 605.68f, 3.84f},
    {3098.51f, 3055.33f, 603.27f, 4.06f},
    {3109.60f, 3063.75f, 604.57f, 4.21f},
    {3113.69f, 3076.36f, 602.11f, 4.15f},
    {3126.08f, 3089.76f, 600.51f, 4.22f},
    {3140.12f, 3117.60f, 597.83f, 4.38f},
    {3142.36f, 3131.53f, 596.98f, 4.36f}
};

class boss_fenryr : public CreatureScript
{
public:
    boss_fenryr() : CreatureScript("boss_fenryr") { }

    struct boss_fenryrAI : public BossAI
    {
        boss_fenryrAI(Creature* creature) : BossAI(creature, DATA_FENRYR) 
        {
            if (instance->GetData(DATA_FENRYR_EVENT) != DONE)
            {
                me->SetVisible(false);
                me->SetReactState(REACT_PASSIVE);
            }
        }

        uint8 jumpCount;

        void Reset()
        {
            events.Reset();
            _Reset();
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RAVENOUS_LEAP_DOT);
            me->setRegeneratingHealth(false);
            me->SetHealth(me->CountPctFromMaxHealth(60));
        }

        void EnterCombat(Unit* /*who*/) //43:41
        {
            _EnterCombat();
            me->SetReactState(REACT_AGGRESSIVE);
            events.ScheduleEvent(EVENT_UNNERVING_HOWL, 3000); //43:44, 44:23, 45:02, 45:38
            events.ScheduleEvent(EVENT_RAVENOUS_LEAP, 28000); //44:09, 44:49, 45:26
            events.ScheduleEvent(EVENT_CLAW_FRENZY, 40000);   //44:20, 44:47, 45:00, 45:36, 45:44
            events.ScheduleEvent(EVENT_SCENT_OF_BLOOD, 9000); //43:50, 44:28, 45:07, 45:47
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (id == SPELL_RAVENOUS_LEAP_JUMP)
                {
                    me->GetMotionMaster()->Clear(false);
                    DoCast(me, SPELL_RAVENOUS_LEAP_T_1, true);
                }

            if (type == POINT_MOTION_TYPE)
                if (id == 1)
                    me->DespawnOrUnsummon();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_RAVENOUS_LEAP_T_1 && jumpCount < 2)
            {
                target->RemoveAurasDueToSpell(SPELL_RAVENOUS_LEAP_MARK);
                DoCast(target, SPELL_RAVENOUS_LEAP_JUMP, true);
                jumpCount++;
            }

            if (spell->Id == SPELL_RAVENOUS_LEAP_AOE)
                DoCast(target, SPELL_RAVENOUS_LEAP_DOT, true);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                if (me->getVictim() && me->getVictim()->HasAura(196838))
                {
                    DoMeleeAttackIfReady();
                    return;
                }
                return;
            }

            if (me->GetDistance(me->GetHomePosition()) >= 60.0f)
            {
                EnterEvadeMode();
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_UNNERVING_HOWL:
                        //Talk();
                        DoCast(SPELL_UNNERVING_HOWL);
                        events.ScheduleEvent(EVENT_UNNERVING_HOWL, 36000);
                        break;
                    case EVENT_RAVENOUS_LEAP:
                        jumpCount = 0;
                        DoCast(me, SPELL_RAVENOUS_LEAP_MARK, true);
                        DoCast(SPELL_RAVENOUS_LEAP);
                        events.ScheduleEvent(EVENT_RAVENOUS_LEAP, 36000);
                        break;
                    case EVENT_CLAW_FRENZY:
                        DoCast(SPELL_CLAW_FRENZY);
                        events.ScheduleEvent(EVENT_CLAW_FRENZY, urand(10, 20) * IN_MILLISECONDS);
                        break;
                    case EVENT_SCENT_OF_BLOOD:
                        DoCast(me, SPELL_SCENT_OF_BLOOD, true);
                        events.ScheduleEvent(EVENT_SCENT_OF_BLOOD, 38000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_fenryrAI (creature);
    }
};

//95674
class npc_fenryr : public CreatureScript
{
public:
    npc_fenryr() : CreatureScript("npc_fenryr") {}

    struct npc_fenryrAI : public ScriptedAI
    {
        npc_fenryrAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = me->GetInstanceScript();
            SummonTraces();
            loseFight = false;
        }

        InstanceScript* instance;
        EventMap events;
        bool loseFight;
        uint8 jumpCount;

        void Reset()
        {
            events.Reset();
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RAVENOUS_LEAP_DOT);
        }

        void EnterCombat(Unit* /*who*/) //39:47
        {
            DoZoneInCombat();
            events.ScheduleEvent(EVENT_UNNERVING_HOWL, 7000); //39:54, 40:26, 40:58
            events.ScheduleEvent(EVENT_RAVENOUS_LEAP, 12000); //39:59, 40:33, 41:07
            events.ScheduleEvent(EVENT_CLAW_FRENZY, 20000);   //40:10, 40:20, 40:31, 40:44, 40:54
        }

        void SummonTraces()
        {
            if (me->GetDistance(oneTrashPos[0]) < me->GetDistance(secondTrashPos[0]))
            {
                for (uint8 i = 1; i < 12; i++)
                    me->SummonGameObject(GO_TRACES_OF_FENRIR, 
                    oneTrashPos[i].GetPositionX(), 
                    oneTrashPos[i].GetPositionY(), 
                    oneTrashPos[i].GetPositionZ(), 
                    oneTrashPos[i].GetOrientation(), 0, 0, 0, 0, 1000);
            }
            else
            {
                for (uint8 i = 1; i < 13; i++)
                    me->SummonGameObject(GO_TRACES_OF_FENRIR, 
                    secondTrashPos[i].GetPositionX(), 
                    secondTrashPos[i].GetPositionY(), 
                    secondTrashPos[i].GetPositionZ(), 
                    secondTrashPos[i].GetOrientation(), 0, 0, 0, 0, 1000);
            }

            std::list<Creature*> trashList;
            GetCreatureListWithEntryInGrid(trashList, me, 96609, 30.0f);
            GetCreatureListWithEntryInGrid(trashList, me, 96611, 30.0f);
            GetCreatureListWithEntryInGrid(trashList, me, 96677, 30.0f);
            GetCreatureListWithEntryInGrid(trashList, me, 103801, 30.0f);
            GetCreatureListWithEntryInGrid(trashList, me, 96640, 30.0f);
            GetCreatureListWithEntryInGrid(trashList, me, 96934, 30.0f);
            GetCreatureListWithEntryInGrid(trashList, me, 99804, 30.0f);
            for (std::list<Creature*>::iterator itr = trashList.begin(); itr != trashList.end(); ++itr)
                if ((*itr))
                    (*itr)->CastSpell((*itr), 29266, true); //Fake Death
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(61) && !loseFight)
            {
                loseFight = true;
                events.Reset();
                DoStopAttack();
                events.ScheduleEvent(EVENT_LOW_HP_MOVE_LAIR, 500);
            }

            if (loseFight)
                damage = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (id == SPELL_RAVENOUS_LEAP_JUMP)
                {
                    me->GetMotionMaster()->Clear(false);
                    DoCast(me, SPELL_RAVENOUS_LEAP_T_1, true);
                }

            if (type == POINT_MOTION_TYPE)
                if (id == 1)
                    me->DespawnOrUnsummon();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_RAVENOUS_LEAP_T_1 && jumpCount < 2)
            {
                target->RemoveAurasDueToSpell(SPELL_RAVENOUS_LEAP_MARK);
                DoCast(target, SPELL_RAVENOUS_LEAP_JUMP, true);
                jumpCount++;
            }

            if (spell->Id == SPELL_RAVENOUS_LEAP_AOE)
                DoCast(target, SPELL_RAVENOUS_LEAP_DOT, true);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (!loseFight)
                if (me->GetDistance(me->GetHomePosition()) >= 50.0f)
                {
                    EnterEvadeMode();
                    return;
                }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_UNNERVING_HOWL:
                        //Talk();
                        DoCast(SPELL_UNNERVING_HOWL);
                        events.ScheduleEvent(EVENT_UNNERVING_HOWL, 32000);
                        break;
                    case EVENT_RAVENOUS_LEAP:
                        jumpCount = 0;
                        DoCast(me, SPELL_RAVENOUS_LEAP_MARK, true);
                        DoCast(SPELL_RAVENOUS_LEAP);
                        events.ScheduleEvent(EVENT_RAVENOUS_LEAP, 34000);
                        break;
                    case EVENT_CLAW_FRENZY:
                        DoCast(SPELL_CLAW_FRENZY);
                        events.ScheduleEvent(EVENT_CLAW_FRENZY, 10000);
                        break;
                    case EVENT_LOW_HP_MOVE_LAIR:
                    {
                        DoCast(me, SPELL_LICKING_WOUNDS, true);
                        DoCast(me, SPELL_BLOOD_SPLAT, true);
                        DoCast(me, SPELL_STEALTH, true);
                        if (instance->GetData(DATA_FENRYR_EVENT) != DONE)
                            instance->SetData(DATA_FENRYR_EVENT, DONE);

                        if (me->GetDistance(oneTrashPos[0]) < me->GetDistance(secondTrashPos[0]))
                            me->GetMotionMaster()->MovePoint(1, 2991.90f, 3071.94f, 605.72f);
                        else
                            me->GetMotionMaster()->MovePoint(1, 3100.44f, 3216.60f, 593.64f);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_fenryrAI(creature);
    }
};

//198800
class spell_fenryr_scent_of_blood_filter : public SpellScriptLoader
{
public:
    spell_fenryr_scent_of_blood_filter() : SpellScriptLoader("spell_fenryr_scent_of_blood_filter") { }

    class spell_fenryr_scent_of_blood_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fenryr_scent_of_blood_filter_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster() || targets.empty())
                return;

            if (targets.size() > 1)
            {
                if (GetCaster()->getVictim())
                    targets.remove(GetCaster()->getVictim());

                Trinity::Containers::RandomResizeList(targets, 1);
            } 
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fenryr_scent_of_blood_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_fenryr_scent_of_blood_filter_SpellScript();
    }
};

//196838
class spell_fenryr_scent_of_blood_fixate : public SpellScriptLoader
{
public:
    spell_fenryr_scent_of_blood_fixate() : SpellScriptLoader("spell_fenryr_scent_of_blood_fixate") { }

    class spell_fenryr_scent_of_blood_fixate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fenryr_scent_of_blood_fixate_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* fenryr = GetCaster()->ToCreature())
            {
                fenryr->SetReactState(REACT_PASSIVE);
                fenryr->AttackStop();
                fenryr->CastSpell(fenryr, 196801, true); //aura + %dmg
                fenryr->ClearUnitState(UNIT_STATE_CASTING);
                if (GetTarget())
                    fenryr->AI()->AttackStart(GetTarget());
            }
        }

        void OnRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* fenryr = GetCaster()->ToCreature())
            {
                fenryr->RemoveAurasDueToSpell(196828);
                fenryr->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_fenryr_scent_of_blood_fixate_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_fenryr_scent_of_blood_fixate_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_fenryr_scent_of_blood_fixate_AuraScript();
    }
};

void AddSC_boss_fenryr()
{
    new boss_fenryr();
    new npc_fenryr();
    new spell_fenryr_scent_of_blood_filter();
    new spell_fenryr_scent_of_blood_fixate();
}