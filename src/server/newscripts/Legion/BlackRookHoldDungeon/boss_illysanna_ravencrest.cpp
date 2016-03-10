/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
    Encounter: Illysanna Ravencrest
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_rook_hold_dungeon.h"

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

class boss_illysanna_ravencrest : public CreatureScript
{
public:
    boss_illysanna_ravencrest() : CreatureScript("boss_illysanna_ravencrest") { }

    struct boss_illysanna_ravencrestAI : public BossAI
    {
        boss_illysanna_ravencrestAI(Creature* creature) : BossAI(creature, DATA_ILLYSANNA) {}

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
        return new boss_illysanna_ravencrestAI (creature);
    }
};

void AddSC_boss_illysanna_ravencrest()
{
    new boss_illysanna_ravencrest();
}