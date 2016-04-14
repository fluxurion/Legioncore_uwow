/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
    Encounter: The Amalgam of Souls
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_rook_hold_dungeon.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_SWIRLING_SCYTHE       = 195254,
    SPELL_SOUL_ECHOES           = 194966,
    SPELL_REAP_SOUL             = 194956,

    SPELL_SUMMON_MINIBOSS_A     = 196619, //Суммонятся что бы открыть двери, после смерти босса
    SPELL_SUMMON_MINIBOSS_B     = 196620,
    SPELL_SUMMON_MINIBOSS_C     = 196646,

    SPELL_SOUL_ECHOES_CLONE_PLR = 194981,
    SPELL_SOUL_ECHOES_DMG       = 194960,
};

enum eEvents
{
    EVENT_SWIRLING_SCYTHE       = 1,
    EVENT_SOUL_ECHOES           = 2,
    EVENT_REAP_SOUL             = 3,
};

enum eAnim //SMSG_PLAY_ONE_SHOT_ANIM_KIT
{
    STALKER_ANIM_1 = 9038,
    STALKER_ANIM_2 = 9039,
    STALKER_ANIM_3 = 9176
};

uint32 AnimRand[3] =
{
    STALKER_ANIM_1,
    STALKER_ANIM_2,
    STALKER_ANIM_3
};

class boss_the_amalgam_of_souls : public CreatureScript
{
public:
    boss_the_amalgam_of_souls() : CreatureScript("boss_the_amalgam_of_souls") { }

    struct boss_the_amalgam_of_soulsAI : public BossAI
    {
        boss_the_amalgam_of_soulsAI(Creature* creature) : BossAI(creature, DATA_AMALGAM) {}

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) //58:07
        {
            //Talk(SAY_AGGRO); //Consume! Devour!
            _EnterCombat();

            events.ScheduleEvent(EVENT_SWIRLING_SCYTHE, 8000); //58:15, 58:36, 58:59
            events.ScheduleEvent(EVENT_SOUL_ECHOES, 14000); //58:22, 58:50, 59:18, 59:45
            events.ScheduleEvent(EVENT_REAP_SOUL, 20000); //58:27, 58:40, 58:54, 59:07
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
                    case EVENT_SWIRLING_SCYTHE:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(pTarget, SPELL_SWIRLING_SCYTHE);
                        events.ScheduleEvent(EVENT_SWIRLING_SCYTHE, 20000);
                        break;
                    case EVENT_SOUL_ECHOES:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(pTarget, SPELL_SOUL_ECHOES);
                        events.ScheduleEvent(EVENT_SOUL_ECHOES, 28000);
                        break;
                    case EVENT_REAP_SOUL:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_REAP_SOUL);
                        events.ScheduleEvent(EVENT_REAP_SOUL, 13000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_the_amalgam_of_soulsAI (creature);
    }
};

//99090
class npc_soul_echoes_stalker : public CreatureScript
{
public:
    npc_soul_echoes_stalker() : CreatureScript("npc_soul_echoes_stalker") {}

    struct npc_soul_echoes_stalkerAI : public ScriptedAI
    {
        npc_soul_echoes_stalkerAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            summoner->CastSpell(me, SPELL_SOUL_ECHOES_CLONE_PLR, true);
            me->PlayOneShotAnimKit(AnimRand[urand(STALKER_ANIM_1, STALKER_ANIM_3)]);
            events.ScheduleEvent(EVENT_1, 4000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_1)
                    DoCast(me, SPELL_SOUL_ECHOES_DMG, true);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_soul_echoes_stalkerAI(creature);
    }
};

void AddSC_boss_the_amalgam_of_souls()
{
    new boss_the_amalgam_of_souls();
    new npc_soul_echoes_stalker();
}