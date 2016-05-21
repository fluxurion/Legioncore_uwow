/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
    Encounter: Ivanyr
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_arcway.h"

// TO-DO: Скиллы почти у всего треша, когда будет 703

enum Says
{
    SAY_AGGRO           = 1,
    SAY_MAGIC           = 2,
    SAY_DEATH           = 3,
}; 

enum Spells
{
    SPELL_SPAWN_VISUAL          = 202679, //Hit npc 98734
    SPELL_ARCANE_BLAST          = 196357,
    SPELL_VOLATILE_MAGIC        = 196562,
    SPELL_OVERCHARGE_MANA       = 196392,
    SPELL_OVERCHARGE            = 196396,
    SPELL_NETHER_LINK           = 196804,
    SPELL_WITHERING_CONSUMPTION = 196549,
    SPELL_CONSUME_ESSENCE       = 196877,
};

enum eEvents
{
    EVENT_VOLATILE_MAGIC        = 1,
    EVENT_OVERCHARGE_MANA       = 2,
    EVENT_NETHER_LINK           = 3,
    EVENT_WITHERING_CONSUMPTION = 4,
    EVENT_CONSUME_ESSENCE       = 5,
};

class boss_ivanyr : public CreatureScript
{
public:
    boss_ivanyr() : CreatureScript("boss_ivanyr") {}

    struct boss_ivanyrAI : public BossAI
    {
        boss_ivanyrAI(Creature* creature) : BossAI(creature, DATA_IVANYR) 
        {
            DoCast(me, SPELL_SPAWN_VISUAL, true);
            introDone = false;
        }

        bool phaseLowMana;
        bool introDone;

        void Reset()
        {
            _Reset();
            phaseLowMana = false;
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveAurasDueToSpell(SPELL_WITHERING_CONSUMPTION);
            me->RemoveAurasDueToSpell(SPELL_OVERCHARGE);
        }

        void EnterCombat(Unit* /*who*/) //08:22
        {
            Talk(SAY_AGGRO); //Stay back! It's mine!
            _EnterCombat();

            events.ScheduleEvent(EVENT_VOLATILE_MAGIC, 8000); //08:31
            events.ScheduleEvent(EVENT_OVERCHARGE_MANA, 16000); //08:41
            events.ScheduleEvent(EVENT_NETHER_LINK, 56000); //09:18
            events.ScheduleEvent(EVENT_WITHERING_CONSUMPTION, 2000);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
        }
        void MoveInLineOfSight(Unit* who)
        {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!introDone && me->IsWithinDistInMap(who, 130.0f))
             {
                introDone = true;
                Talk(0);
             }
        }             

        void JustReachedHome()
        {
            DoCast(me, SPELL_SPAWN_VISUAL, true);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                DoCast(me, SPELL_OVERCHARGE_MANA, true);
                me->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 60.0f)
            {
                EnterEvadeMode();
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_VOLATILE_MAGIC:
                        DoCast(SPELL_VOLATILE_MAGIC);
                        Talk(SAY_MAGIC);
                        events.ScheduleEvent(EVENT_VOLATILE_MAGIC, 24000);
                        break;
                    case EVENT_OVERCHARGE_MANA:
                        DoStopAttack();
                        me->GetMotionMaster()->MovePoint(1, me->GetHomePosition());
                        events.ScheduleEvent(EVENT_OVERCHARGE_MANA, 20000);
                        break;
                    case EVENT_NETHER_LINK:
                        DoCast(SPELL_NETHER_LINK);
                        events.ScheduleEvent(EVENT_NETHER_LINK, 56000);
                        break;
                    case EVENT_WITHERING_CONSUMPTION:
                        if (me->GetPower(POWER_MANA) > 0)
                            events.ScheduleEvent(EVENT_NETHER_LINK, 2000);
                        else
                        {
                            events.Reset();
                            DoCast(SPELL_WITHERING_CONSUMPTION);
                            events.ScheduleEvent(EVENT_CONSUME_ESSENCE, 8000);
                        }
                        break;
                    case EVENT_CONSUME_ESSENCE:
                        DoCast(SPELL_CONSUME_ESSENCE);
                        events.ScheduleEvent(EVENT_CONSUME_ESSENCE, 16000);
                        break;
                }
            }
            if (me->HasAura(SPELL_WITHERING_CONSUMPTION))
                DoMeleeAttackIfReady();
            else
                DoSpellAttackIfReady(SPELL_ARCANE_BLAST);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ivanyrAI (creature);
    }
};

//196392
class spell_ivanyr_overcharge_mana : public SpellScriptLoader
{
public:
    spell_ivanyr_overcharge_mana() : SpellScriptLoader("spell_ivanyr_overcharge_mana") { }

    class spell_ivanyr_overcharge_mana_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ivanyr_overcharge_mana_AuraScript);

        uint16 tickTimer = 1000;

        void OnTick(AuraEffect const* aurEff)
        {
            if (!GetCaster() || !GetTarget())
                return;

            GetTarget()->SendSpellCreateVisual(GetSpellInfo(), nullptr, GetCaster());
            
            if (tickTimer <= 0)
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_OVERCHARGE, true);
                tickTimer = 1000;
            }
            else
                tickTimer -= 100;
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ivanyr_overcharge_mana_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ivanyr_overcharge_mana_AuraScript();
    }
};

void AddSC_boss_ivanyr()
{
    new boss_ivanyr();
    new spell_ivanyr_overcharge_mana();
}