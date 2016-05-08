/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
    Encounter: Cordana
    TODO: Босс инвалид, нужно будет доделать на 703+.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "vault_of_the_wardens.h"

/* enum Says
{
    SAY_AGGRO               = ,
    SAY_DEATH               = ,
}; */

enum Spells
{
    SPELL_ELUNES_LIGHT          = 201359,
    SPELL_KNOCKDOWN_KICK        = 197251,
    SPELL_DEEPENING_SHADOWS     = 213576,
    SPELL_DETONATING_MOONGLAIVE = 197513,
    SPELL_SHADOWSTEP            = 203416,
    SPELL_SHADOWSTEP_2          = 204635,

    //Other
    SPELL_ELUNES_LIGHT_VIS_1    = 197897,
    SPELL_ELUNES_LIGHT_VIS_2    = 204294,
    SPELL_ELUNES_LIGHT_AT_1     = 197941, //AT cast 192752 ?
    SPELL_ELUNES_LIGHT_AT_2     = 197910,
    SPELL_ELUNES_LIGHT_CLICK    = 197890,
    SPELL_ELUNES_LIGHT_OVERRIDE = 192656,
    SPELL_ELUNES_LIGHT_SUMMON   = 204491,
    SPELL_STOLEN_LIGHT          = 206567,
};

enum eEvents
{
    EVENT_KNOCKDOWN_KICK        = 1,
    EVENT_DEEPENING_SHADOWS     = 2,
    EVENT_DETONATING_MOONGLAIVE = 3,
    EVENT_SHADOWSTEP            = 4,
};

Position const centrPos = {4049.907f, -297.4254f, -281.4167f};

class boss_cordana : public CreatureScript
{
public:
    boss_cordana() : CreatureScript("boss_cordana") { }

    struct boss_cordanaAI : public BossAI
    {
        boss_cordanaAI(Creature* creature) : BossAI(creature, DATA_CORDANA) 
        {
            intro = false;
            //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
        }

        bool intro;

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) //24:29
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_ELUNES_LIGHT, true);

            events.ScheduleEvent(EVENT_KNOCKDOWN_KICK, 9000); //24:38, 24:59
            events.ScheduleEvent(EVENT_DEEPENING_SHADOWS, 11000); //24:40, 25:12
            //events.ScheduleEvent(EVENT_DETONATING_MOONGLAIVE, 16000); //24:45, 24:52 - Не реализован этот тип передвижения АТ
            //events.ScheduleEvent(EVENT_SHADOWSTEP, 44000); //25:13
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        /* void MoveInLineOfSight(Unit* who) //Включить когда будет работать геодата
        {
            if (who->GetTypeId() != TYPEID_PLAYER || who->ToPlayer()->isGameMaster() || intro)
                return;

            if (me->GetDistance(who) < 80.0f)
            {
                intro = true;
                me->GetMotionMaster()->MovePoint(1, centrPos);
                me->SetHomePosition(centrPos);
            }
        } */

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == 204642) //Shadowstep
                DoCast(me, SPELL_SHADOWSTEP_2, true);
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
                    case EVENT_KNOCKDOWN_KICK:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_KNOCKDOWN_KICK);
                        events.ScheduleEvent(EVENT_KNOCKDOWN_KICK, 21000);
                        break;
                    case EVENT_DEEPENING_SHADOWS:
                        DoCast(SPELL_DEEPENING_SHADOWS);
                        events.ScheduleEvent(EVENT_DEEPENING_SHADOWS, 22000);
                        break;
                    case EVENT_DETONATING_MOONGLAIVE:
                        DoCast(SPELL_DETONATING_MOONGLAIVE);
                        events.ScheduleEvent(EVENT_DETONATING_MOONGLAIVE, 8000);
                        break;
                    case EVENT_SHADOWSTEP:
                        DoCast(SPELL_SHADOWSTEP);
                        events.ScheduleEvent(EVENT_SHADOWSTEP, 60000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_cordanaAI (creature);
    }
};

//100525
class npc_cordana_glowing_sentry : public CreatureScript
{
public:
    npc_cordana_glowing_sentry() : CreatureScript("npc_cordana_glowing_sentry") { }

    struct npc_cordana_glowing_sentryAI : public ScriptedAI
    {
        npc_cordana_glowing_sentryAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            if (me->GetDistance(summoner) > 80.0f)
            {
                DoCast(me, SPELL_ELUNES_LIGHT_VIS_1, true);
                DoCast(me, SPELL_ELUNES_LIGHT_VIS_2, true);
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_cordana_glowing_sentryAI(creature);
    }
};

void AddSC_boss_cordana()
{
    new boss_cordana();
    new npc_cordana_glowing_sentry();
}