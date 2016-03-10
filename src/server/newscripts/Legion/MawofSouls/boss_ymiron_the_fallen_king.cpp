/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
    Encounter: Ymiron the fallen king
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

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

class boss_ymiron_the_fallen_king : public CreatureScript
{
public:
    boss_ymiron_the_fallen_king() : CreatureScript("boss_ymiron_the_fallen_king") { }

    struct boss_ymiron_the_fallen_kingAI : public BossAI
    {
        boss_ymiron_the_fallen_kingAI(Creature* creature) : BossAI(creature, DATA_YMIRON) {}

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
        return new boss_ymiron_the_fallen_kingAI (creature);
    }
};

void AddSC_boss_ymiron_the_fallen_king()
{
    new boss_ymiron_the_fallen_king();
}