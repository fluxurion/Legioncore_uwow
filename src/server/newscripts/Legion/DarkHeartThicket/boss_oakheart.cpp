/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
    Encounter: Oakheart
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "darkheart_thicket.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_CANCEL_DEEP_ROOTS     = 165953, //Remove 165950
    SPELL_SHATTERED_EARTH       = 204666,
    SPELL_STRANGLING_ROOTS      = 204574,
    SPELL_STRANGLING_ROOTS_AT   = 199067,
    SPELL_NIGHTMARE_BREATH      = 204667,
    SPELL_CRUSHING_GRIP         = 204611,
};

enum eEvents
{
    EVENT_SHATTERED_EARTH       = 1,
    EVENT_STRANGLING_ROOTS      = 2,
    EVENT_NIGHTMARE_BREATH      = 3,
    EVENT_CRUSHING_GRIP         = 4,
    EVENT_REMOVE_PASSENGERS     = 5,
};

class boss_oakheart : public CreatureScript
{
public:
    boss_oakheart() : CreatureScript("boss_oakheart") {}

    struct boss_oakheartAI : public BossAI
    {
        boss_oakheartAI(Creature* creature) : BossAI(creature, DATA_OAKHEART) {}

        void Reset()
        {
            events.Reset();
            _Reset();

            if (me->GetVehicleKit())
                me->GetVehicleKit()->RemoveAllPassengers();
        }

        void EnterCombat(Unit* /*who*/) //43:30
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            events.ScheduleEvent(EVENT_SHATTERED_EARTH, 6000);      //43:37, 44:17
            events.ScheduleEvent(EVENT_STRANGLING_ROOTS, 14000);    //43:44, 44:07
            events.ScheduleEvent(EVENT_NIGHTMARE_BREATH, 22000);    //43:52, 44:24
            events.ScheduleEvent(EVENT_CRUSHING_GRIP, 30000);       //44:01
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

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
        {
            if (apply)
            {
                //DoCast(me, 204643, true); //Check Targets
                DoCast(204644); //Timing cast
            }
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == 204644)
            {
                events.ScheduleEvent(EVENT_REMOVE_PASSENGERS, 6000);

                if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                {
                    Unit* pTarget = SelectTarget(SELECT_TARGET_FARTHEST, 0, 0.0f, true);
                    if (pTarget)
                    {
                        passenger->CastSpell(pTarget, 204651);
                        events.CancelEvent(EVENT_REMOVE_PASSENGERS);
                    }
                    else
                        passenger->CastSpell(passenger, 204651);
                }
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
                    case EVENT_SHATTERED_EARTH:
                        DoCast(SPELL_SHATTERED_EARTH);
                        events.ScheduleEvent(EVENT_SHATTERED_EARTH, 40000);
                        break;
                    case EVENT_STRANGLING_ROOTS:
                        DoCast(SPELL_STRANGLING_ROOTS);
                        events.ScheduleEvent(EVENT_STRANGLING_ROOTS, 22000);
                        break;
                    case EVENT_NIGHTMARE_BREATH:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_NIGHTMARE_BREATH);
                        events.ScheduleEvent(EVENT_NIGHTMARE_BREATH, 32000);
                        break;
                    case EVENT_CRUSHING_GRIP:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_CRUSHING_GRIP);
                        events.ScheduleEvent(EVENT_CRUSHING_GRIP, 30000);
                        break;
                    case EVENT_REMOVE_PASSENGERS:
                        if (me->GetVehicleKit())
                            me->GetVehicleKit()->RemoveAllPassengers();
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_oakheartAI (creature);
    }
};

//100991
class npc_oakheart_strangling_roots : public CreatureScript
{
public:
    npc_oakheart_strangling_roots() : CreatureScript("npc_oakheart_strangling_roots") {}

    struct npc_oakheart_strangling_rootsAI : public ScriptedAI
    {
        npc_oakheart_strangling_rootsAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_STRANGLING_ROOTS_AT, true);
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_oakheart_strangling_rootsAI (creature);
    }
};

void AddSC_boss_oakheart()
{
    new boss_oakheart();
    new npc_oakheart_strangling_roots();
}