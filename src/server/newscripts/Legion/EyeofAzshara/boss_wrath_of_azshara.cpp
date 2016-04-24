/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
    Encounter: Wrath of Azshara
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "eye_of_azshara.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    //SPELL_    = ,
    //SPELL_    = ,
    //SPELL_    = ,
    //SPELL_    = ,
};

enum eEvents
{
    //EVENT_    = 1,
    //EVENT_    = 2,
    //EVENT_    = 3,
    //EVENT_    = 4,
    //EVENT_    = 5,
};

class boss_wrath_of_azshara : public CreatureScript
{
public:
    boss_wrath_of_azshara() : CreatureScript("boss_wrath_of_azshara") { }

    struct boss_wrath_of_azsharaAI : public BossAI
    {
        boss_wrath_of_azsharaAI(Creature* creature) : BossAI(creature, DATA_WRATH_OF_AZSHARA) {}

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            //events.ScheduleEvent(EVENT_, );
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
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
                    case EVENT_1:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_wrath_of_azsharaAI (creature);
    }
};

void AddSC_boss_wrath_of_azshara()
{
    new boss_wrath_of_azshara();
}