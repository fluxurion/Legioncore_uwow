/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
    Encounter: Tirathon Saltheril
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "vault_of_the_wardens.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_DARKSTRIKES       = 191941,
    SPELL_DARKSTRIKES_DMG   = 191948,
    SPELL_SWOOP             = 191765,
    SPELL_SWOOP_JUMP        = 191766,
    SPELL_FURIOUS_BLAST     = 191823,

    //Second Phase 50% HP
    SPELL_METAMORPHOSIS     = 202740,
    SPELL_FEL_MORTAR        = 202913,

    //Other
    SPELL_JUMP_DOWN         = 199132,
    SPELL_DUAL_WIELD        = 42459,
};

enum eEvents
{
    EVENT_DARKSTRIKES       = 1,
    EVENT_SWOOP             = 2,
    EVENT_METAMORPHOSIS     = 3,
    EVENT_FEL_MORTAR        = 4,
};

class boss_tirathon_saltheril : public CreatureScript
{
public:
    boss_tirathon_saltheril() : CreatureScript("boss_tirathon_saltheril") {}

    struct boss_tirathon_saltherilAI : public BossAI
    {
        boss_tirathon_saltherilAI(Creature* creature) : BossAI(creature, DATA_SALTHERIL) {}

        bool phaseSecond;

        void Reset()
        {
            _Reset();
            phaseSecond = false;
            DoCast(me, SPELL_DUAL_WIELD, true);
        }

        void EnterCombat(Unit* /*who*/) //00:55
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_DARKSTRIKES, 16000); //01:11
            events.ScheduleEvent(EVENT_SWOOP, 22000); //01:18
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (id == SPELL_SWOOP_JUMP)
                    DoCast(SPELL_FURIOUS_BLAST);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(51) && !phaseSecond)
            {
                phaseSecond = true;
                events.ScheduleEvent(EVENT_METAMORPHOSIS, 0);
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_FEL_MORTAR:
                {
                    Position pos;
                    for (uint8 i = 0; i < 3; i++)
                    {
                        target->GetRandomNearPosition(pos, 5.0f);
                        me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 202920, true); //Fel Mortar
                    }
                    break;
                }
                case SPELL_DARKSTRIKES_DMG:
                    if (!target->IsActiveSaveAbility())
                        DoCast(me, 191999, true); //Absorb shield
                    break;
            }
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
                    case EVENT_DARKSTRIKES:
                        DoCast(SPELL_DARKSTRIKES);
                        events.ScheduleEvent(EVENT_DARKSTRIKES, 60000);
                        break;
                    case EVENT_SWOOP:
                        DoCast(SPELL_SWOOP);
                        events.ScheduleEvent(EVENT_SWOOP, 30000);
                        break;
                    case EVENT_METAMORPHOSIS:
                        DoCast(SPELL_METAMORPHOSIS);
                        events.CancelEvent(EVENT_SWOOP);
                        events.ScheduleEvent(EVENT_FEL_MORTAR, 12000);
                        break;
                    case EVENT_FEL_MORTAR:
                        DoCast(SPELL_FEL_MORTAR);
                        events.ScheduleEvent(EVENT_FEL_MORTAR, 16000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_tirathon_saltherilAI (creature);
    }
};

void AddSC_boss_tirathon_saltheril()
{
    new boss_tirathon_saltheril();
}