/*
    http://uwow.biz
    Dungeon : Neltharions Lair 100-110
    Encounter: Dargrul the Underking
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "neltharions_lair.h"

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

class boss_dargrul_the_underking : public CreatureScript
{
public:
    boss_dargrul_the_underking() : CreatureScript("boss_dargrul_the_underking") { }

    struct boss_dargrul_the_underkingAI : public BossAI
    {
        boss_dargrul_the_underkingAI(Creature* creature) : BossAI(creature, DATA_DARGRUL) {}

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
        return new boss_dargrul_the_underkingAI (creature);
    }
};

void AddSC_boss_dargrul_the_underking()
{
    new boss_dargrul_the_underking();
}