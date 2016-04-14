/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
    Encounter: Smashspite the Hateful
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
    SPELL_ZERO_REGEN            = 72242,
    SPELL_BRUTALITY_PROC        = 198114,
    SPELL_EARTHSHAKING_STOMP    = 198073,
    SPELL_BRUTAL_HAYMAKER       = 198245,

    SPELL_FEL_VOMIT             = 198446,
};

enum eEvents
{
    EVENT_EARTHSHAKING_STOMP    = 1,
    EVENT_SUM_FEL_BAT           = 2,
};

class boss_smashspite_the_hateful : public CreatureScript
{
public:
    boss_smashspite_the_hateful() : CreatureScript("boss_smashspite_the_hateful") { }

    struct boss_smashspite_the_hatefulAI : public BossAI
    {
        boss_smashspite_the_hatefulAI(Creature* creature) : BossAI(creature, DATA_SMASHSPITE) {}

        ObjectGuid felbatContainer[10];

        void Reset()
        {
            _Reset();
            DoCast(me, SPELL_ZERO_REGEN, true);
            DoCast(me, SPELL_BRUTALITY_PROC, true);
            me->SetPower(POWER_ENERGY, 0);
        }

        void EnterCombat(Unit* /*who*/) //16:43
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_EARTHSHAKING_STOMP, 12000); //16:55, 17:12, 17:29
            events.ScheduleEvent(EVENT_SUM_FEL_BAT, );
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
                    case EVENT_EARTHSHAKING_STOMP:
                        DoCast(SPELL_EARTHSHAKING_STOMP);
                        events.ScheduleEvent(EVENT_EARTHSHAKING_STOMP, 17000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_smashspite_the_hatefulAI (creature);
    }
};

//198114
class spell_smashpite_brutality_proc : public SpellScriptLoader
{
public:
    spell_smashpite_brutality_proc() : SpellScriptLoader("spell_smashpite_brutality_proc") { }

    class spell_smashpite_brutality_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_smashpite_brutality_proc_AuraScript);

        uint8 PowerCount = 0;

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster || caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            PowerCount = caster->GetPower(POWER_ENERGY);
sLog->outError(LOG_FILTER_SERVER_LOADING, ">>>>>>>");
            if (PowerCount < 100)
            {
                if (PowerCount < 32 || PowerCount == 92)
                {
                    caster->SetPower(POWER_ENERGY, PowerCount + 8);
                }
                else
                    caster->SetPower(POWER_ENERGY, PowerCount + 5);
            }
            else
            {
                if (caster->getVictim())
                    caster->CastSpell(caster->getVictim(), SPELL_BRUTAL_HAYMAKER);
            }
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_smashpite_brutality_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_smashpite_brutality_proc_AuraScript();
    }
};

void AddSC_boss_smashspite_the_hateful()
{
    new boss_smashspite_the_hateful();
    new spell_smashpite_brutality_proc();
}