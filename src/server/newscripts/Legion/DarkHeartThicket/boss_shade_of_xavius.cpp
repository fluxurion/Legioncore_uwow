/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
    Encounter: Shade of Xavius
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "darkheart_thicket.h"

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

class boss_shade_of_xavius : public CreatureScript
{
public:
    boss_shade_of_xavius() : CreatureScript("boss_shade_of_xavius") { }

    struct boss_shade_of_xaviusAI : public BossAI
    {
        boss_shade_of_xaviusAI(Creature* creature) : BossAI(creature, DATA_XAVIUS) {}

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
        return new boss_shade_of_xaviusAI (creature);
    }
};

void AddSC_boss_shade_of_xavius()
{
    new boss_shade_of_xavius();
}