/*
    http://uwow.biz
    Dungeon : Violet Hold Legion 100-110
    Encounter: Mindflayer Kaahrj
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "violet_hold_legion.h"

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

class boss_mindflayer_kaahrj : public CreatureScript
{
public:
    boss_mindflayer_kaahrj() : CreatureScript("boss_mindflayer_kaahrj") { }

    struct boss_mindflayer_kaahrjAI : public BossAI
    {
        boss_mindflayer_kaahrjAI(Creature* creature) : BossAI(creature, DATA_KAAHRJ) {}

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
        return new boss_mindflayer_kaahrjAI (creature);
    }
};

void AddSC_boss_mindflayer_kaahrj()
{
    new boss_mindflayer_kaahrj();
}