/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
    Encounter: Warlord Parjesh
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "eye_of_azshara.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_EMPTY_ENERGY              = 202146,
    SPELL_ENERGIZE                  = 202143,
    SPELL_CALL_REINFORCEMENTS_1     = 192072,
    SPELL_CALL_REINFORCEMENTS_2     = 192073,
    SPELL_THROW_SPEAR               = 192131,
    SPELL_CRASHING_WAVE             = 191900,
    SPELL_IMPALING_SPEAR_FILTER     = 191927,
    SPELL_IMPALING_SPEAR_FIXATE     = 192094,
    SPELL_IMPALING_SPEAR            = 191946,
    SPELL_IMPALING_SPEAR_DMG_NPC    = 191975,
    SPELL_IMPALING_SPEAR_DMG_PLR    = 191977,
    SPELL_IMPALING_SPEAR_KNOCK      = 193183,
    SPELL_ENRAGE                    = 197064,
};

enum eEvents
{
    EVENT_CALL_REINFORC     = 1,
    EVENT_THROW_SPEAR       = 2,
    EVENT_CRASHING_WAVE     = 3,
    EVENT_IMPALING_SPEAR    = 4,
};

class boss_warlord_parjesh : public CreatureScript
{
public:
    boss_warlord_parjesh() : CreatureScript("boss_warlord_parjesh") { }

    struct boss_warlord_parjeshAI : public BossAI
    {
        boss_warlord_parjeshAI(Creature* creature) : BossAI(creature, DATA_PARJESH) {}

        bool randSum;
        bool enrage;

        void Reset()
        {
            _Reset();
            me->RemoveAurasDueToSpell(SPELL_ENERGIZE);
            me->RemoveAurasDueToSpell(SPELL_ENRAGE);
            DoCast(me, SPELL_EMPTY_ENERGY, true);
            randSum = false;
            enrage = false;
        }

        void EnterCombat(Unit* /*who*/) //19:40
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_ENERGIZE, true);

            events.ScheduleEvent(EVENT_CALL_REINFORC, 3000); //19:43
            events.ScheduleEvent(EVENT_THROW_SPEAR, 11000); //19:51
            events.ScheduleEvent(EVENT_CRASHING_WAVE, 3000);
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            events.ScheduleEvent(EVENT_IMPALING_SPEAR, 0);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_IMPALING_SPEAR)
            {
                if (target->GetTypeId() != TYPEID_PLAYER)
                    DoCast(target, SPELL_IMPALING_SPEAR_DMG_NPC, true);
                else
                    DoCast(target, SPELL_IMPALING_SPEAR_DMG_PLR, true);

                DoCast(target, SPELL_IMPALING_SPEAR_KNOCK, true);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(31) && !enrage)
            {
                enrage = true;
                DoCast(me, SPELL_ENRAGE, true);
            }
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
                    case EVENT_CALL_REINFORC:
                        if (!randSum)
                        {
                            DoCast(SPELL_CALL_REINFORCEMENTS_1);
                            randSum = true;
                        }
                        else
                        {
                            DoCast(SPELL_CALL_REINFORCEMENTS_2);
                            randSum = false;
                        }
                        events.ScheduleEvent(EVENT_CALL_REINFORC, 20000);
                        break;
                    case EVENT_THROW_SPEAR:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                            DoCast(pTarget, SPELL_THROW_SPEAR);
                        events.ScheduleEvent(EVENT_THROW_SPEAR, 16000);
                        break;
                    case EVENT_CRASHING_WAVE:
                        DoCast(SPELL_CRASHING_WAVE);
                        events.ScheduleEvent(EVENT_CRASHING_WAVE, 3000);
                        break;
                    case EVENT_IMPALING_SPEAR:
                        DoCast(me, SPELL_IMPALING_SPEAR_FILTER, true);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_warlord_parjeshAI (creature);
    }
};

void AddSC_boss_warlord_parjesh()
{
    new boss_warlord_parjesh();
}