/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
    Encounter: God King Skovald
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

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

class boss_god_king_skovald : public CreatureScript
{
public:
    boss_god_king_skovald() : CreatureScript("boss_god_king_skovald") { }

    struct boss_god_king_skovaldAI : public BossAI
    {
        boss_god_king_skovaldAI(Creature* creature) : BossAI(creature, DATA_SKOVALD) {}

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
        return new boss_god_king_skovaldAI (creature);
    }
};

void AddSC_boss_god_king_skovald()
{
    new boss_god_king_skovald();
}