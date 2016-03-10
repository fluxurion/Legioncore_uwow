/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
    Encounter: Advisor Vandros
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_arcway.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
    SAY_EVADE           = ,
}; */

/* enum Spells
{
    SPELL_    = ,
    SPELL_    = ,
    SPELL_    = ,
    SPELL_    = ,
}; */

/* enum eEvents
{
    EVENT_    = 1,
    EVENT_    = 2,
    EVENT_    = 3,
    EVENT_    = 4,
    EVENT_    = 5,
}; */

class boss_advisor_vandros : public CreatureScript
{
public:
    boss_advisor_vandros() : CreatureScript("boss_advisor_vandros") { }

    struct boss_advisor_vandrosAI : public BossAI
    {
        boss_advisor_vandrosAI(Creature* creature) : BossAI(creature, DATA_VANDROS) {}

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
        }

        void EnterEvadeMode()
        {
            //Talk(SAY_EVADE);
            BossAI::EnterEvadeMode();
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
        return new boss_advisor_vandrosAI (creature);
    }
};

void AddSC_boss_advisor_vandros()
{
    new boss_advisor_vandros();
}