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

class boss_oakheart : public CreatureScript
{
public:
    boss_oakheart() : CreatureScript("boss_oakheart") { }

    struct boss_oakheartAI : public BossAI
    {
        boss_oakheartAI(Creature* creature) : BossAI(creature, DATA_OAKHEART) {}

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
        return new boss_oakheartAI (creature);
    }
};

void AddSC_boss_oakheart()
{
    new boss_oakheart();
}