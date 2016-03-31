/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
    Encounter: Hymdall
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
    SPELL_HORN_OF_VALOR         = 191284,
    SPELL_DANCING_BLADE         = 193235,
    SPELL_DANCING_BLADE_AT      = 193214,
    SPELL_BLOODLETTING_SWEEP    = 193092,
    //Drake
    SPELL_STORM_BREATH_VIS      = 188404,
    SPELL_STORM_BREATH_AT       = 192959,
};

enum eEvents
{
    EVENT_HORN_OF_VALOR         = 1,
    EVENT_SUM_S_DRAKE           = 2,
    EVENT_DANCING_BLADE         = 3,
    EVENT_BLOODLETTING_SWEEP    = 4,
};

Position const drakePos[6] =
{
    {3566.12f, 675.0f,  625.44f, 4.67f},
    {3566.12f, 383.00f, 625.44f, 1.60f},
    {3534.45f, 675.02f, 625.44f, 4.67f},
    {3534.45f, 383.00f, 625.44f, 1.60f},
    {3502.45f, 675.04f, 625.44f, 4.67f},
    {3502.45f, 383.00f, 625.44f, 1.60f}
};

class boss_hymdall : public CreatureScript
{
public:
    boss_hymdall() : CreatureScript("boss_hymdall") { }

    struct boss_hymdallAI : public BossAI
    {
        boss_hymdallAI(Creature* creature) : BossAI(creature, DATA_HYMDALL) {}

        uint8 drakeCount;
        uint8 randDrakePos;

        void Reset()
        {
            events.Reset();
            _Reset();
            drakeCount = 0;
            randDrakePos = 0;
        }

        void EnterCombat(Unit* /*who*/) //12:47
        {
            //Talk(SAY_AGGRO); //I will be the judge now.
            _EnterCombat();
            events.ScheduleEvent(EVENT_HORN_OF_VALOR, 6000);        //12:53, 13:24, 14:04
            events.ScheduleEvent(EVENT_DANCING_BLADE, 15000);       //13:02, 13:13, 13:34, 13:45
            events.ScheduleEvent(EVENT_BLOODLETTING_SWEEP, 20000);  //13:07, 13:39, 14:01, 14:24
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_DANCING_BLADE)
            {
                summon->SetReactState(REACT_PASSIVE);
                summon->CastSpell(summon, SPELL_DANCING_BLADE_AT, true);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_HORN_OF_VALOR:
                    {
                        //Talk();
                        DoCast(SPELL_HORN_OF_VALOR);
                        if (drakeCount < 3)
                            drakeCount++;
                        randDrakePos = urand(0, 5);
                        for (uint8 i = 0; i < drakeCount; i++)
                            events.ScheduleEvent(EVENT_SUM_S_DRAKE, 5 * (i + 1) * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_HORN_OF_VALOR, 30000);
                        break;
                    }
                    case EVENT_SUM_S_DRAKE:
                        me->SummonCreature(NPC_STORM_DRAKE, drakePos[randDrakePos]);
                        randDrakePos += 2;
                        if (randDrakePos > 5)
                            randDrakePos = 0;
                        break;
                    case EVENT_DANCING_BLADE:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                            DoCast(pTarget, SPELL_DANCING_BLADE);
                        events.ScheduleEvent(EVENT_DANCING_BLADE, 11000);
                        break;
                    case EVENT_BLOODLETTING_SWEEP:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_BLOODLETTING_SWEEP, true);
                        events.ScheduleEvent(EVENT_BLOODLETTING_SWEEP, 26000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_hymdallAI (creature);
    }
};

//97788
class npc_hymdall_storm_drake : public CreatureScript
{
public:
    npc_hymdall_storm_drake() : CreatureScript("npc_hymdall_storm_drake") { }

    struct npc_hymdall_storm_drakeAI : public ScriptedAI
    {
        npc_hymdall_storm_drakeAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetCanFly(true);
            me->SetSpeed(MOVE_FLIGHT, 2.0f);
        }

        EventMap events;

        void Reset(){}

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_1, 3000);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            events.Reset();
            me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                    {
                        DoCast(SPELL_STORM_BREATH_VIS);
                        Position pos;
                        me->GetNearPosition(pos, 100.0f, 0.0f);
                        me->GetMotionMaster()->MovePoint(1, pos);
                        events.ScheduleEvent(EVENT_2, 1000);
                        break;
                    }
                    case EVENT_2:
                        if (Creature* staticField = me->FindNearestCreature(NPC_STATIC_FIELD, 15.0f))
                            if (!staticField->HasAura(SPELL_STORM_BREATH_AT))
                                staticField->CastSpell(staticField, SPELL_STORM_BREATH_AT, true);
                        events.ScheduleEvent(EVENT_2, 200);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_hymdall_storm_drakeAI(creature);
    }
};

void AddSC_boss_hymdall()
{
    new boss_hymdall();
    new npc_hymdall_storm_drake();
}