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
}; */

enum Spells
{
    SPELL_ZERO_REGEN            = 118357,
    SPELL_POWER                 = 167922,
    //SPELL_KNEELING_VISUAL     = 197227, //State
    //SPELL_SOUL_SIPHON         = 194665, //HitTarget npc 102375
    SPELL_DARK_SLASH            = 193211,
    SPELL_SCREAMS_OF_DEAD       = 193364,
    SPELL_WINDS_OF_NORTHREND    = 193977,
    SPELL_BANE                  = 193460,
};

enum eEvents
{
    EVENT_DARK_SLASH            = 1,
    EVENT_SCREAMS_OF_DEAD       = 2,
    EVENT_WINDS_OF_NORTHREND    = 3,
    EVENT_BANE                  = 4,
};

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
            me->RemoveAurasDueToSpell(SPELL_POWER);
            DoCast(me, SPELL_ZERO_REGEN, true);
            me->SetMaxPower(POWER_MANA, 100);
            me->SetPower(POWER_MANA, 80);
            if (GameObject* go = instance->instance->GetGameObject(instance->GetGuidData(DATA_YMIRON_GORN)))
                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
        }

        void EnterCombat(Unit* /*who*/) //12:09
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_POWER, true);
            events.ScheduleEvent(EVENT_SCREAMS_OF_DEAD, 6000);      //12:15, 12:38, 13:01
            events.ScheduleEvent(EVENT_WINDS_OF_NORTHREND, 16000);  //12:25, 12:49, 13:19, 13:44
            //events.ScheduleEvent(EVENT_BANE, 22000);                //12:31, 13:23, 14:13 - нужна инфа по АТ
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
            if (GameObject* go = instance->instance->GetGameObject(instance->GetGuidData(DATA_YMIRON_GORN)))
                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_1)
                events.ScheduleEvent(EVENT_DARK_SLASH, 500);
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
                    case EVENT_DARK_SLASH:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_DARK_SLASH);
                        break;
                    case EVENT_SCREAMS_OF_DEAD:
                        DoCast(SPELL_SCREAMS_OF_DEAD);
                        events.ScheduleEvent(EVENT_SCREAMS_OF_DEAD, 23000);
                        break;
                    case EVENT_WINDS_OF_NORTHREND:
                        DoCast(SPELL_WINDS_OF_NORTHREND);
                        events.ScheduleEvent(EVENT_WINDS_OF_NORTHREND, 24000);
                        break;
                    case EVENT_BANE:
                        DoCast(SPELL_BANE);
                        events.ScheduleEvent(EVENT_BANE, 50000);
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

//167922
class spell_ymiron_power_regen : public SpellScriptLoader
{
    public:
        spell_ymiron_power_regen() : SpellScriptLoader("spell_ymiron_power_regen") { }
 
        class spell_ymiron_power_regen_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ymiron_power_regen_AuraScript)

            uint8 power;
            uint8 powerCounter;

            bool Load()
            {
                powerCounter = 0;
                return true;
            }

            void OnPeriodic(AuraEffect const* aurEff)
            {
                if (Creature* ymiron = GetCaster()->ToCreature())
                {
                    power = ymiron->GetPower(POWER_MANA);
                    if (power < 100)
                    {
                        if (powerCounter < 2)
                        {
                            powerCounter++;
                            ymiron->SetPower(POWER_MANA, power + 7);
                        }
                        else
                        {
                            powerCounter = 0;
                            ymiron->SetPower(POWER_MANA, power + 6);
                        }
                    }
                    else
                    {
                        ymiron->SetPower(POWER_MANA, 0);
                        ymiron->AI()->DoAction(ACTION_1); //EVENT_DARK_SLASH
                    }
                }
            }
 
            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ymiron_power_regen_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };
 
        AuraScript* GetAuraScript() const
        {
            return new spell_ymiron_power_regen_AuraScript();
        }
};

//193460
class spell_ymiron_bane : public SpellScriptLoader
{
    public:                                                      
        spell_ymiron_bane() : SpellScriptLoader("spell_ymiron_bane") { }

        class spell_ymiron_bane_AuraScript : public AuraScript 
        {
            PrepareAuraScript(spell_ymiron_bane_AuraScript) 

            void OnPereodic(AuraEffect const* aurEff) 
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Position pos;
                GetCaster()->GetRandomNearPosition(pos, 30.0f);
                if (aurEff->GetTickNumber() == 1)
                {
                    for (int8 i = 0; i < 4; i++)
                        GetCaster()->CastSpell(pos, GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
                }
                else
                    GetCaster()->CastSpell(pos, GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
            }

            void Register() 
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ymiron_bane_AuraScript::OnPereodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ymiron_bane_AuraScript();
        }
};

//191560
class spell_ymiron_helheim_teleport : public SpellScriptLoader
{
    public:
        spell_ymiron_helheim_teleport() : SpellScriptLoader("spell_ymiron_helheim_teleport") { }

        class spell_ymiron_helheim_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ymiron_helheim_teleport_SpellScript);

            void HandleOnHit()
            {
                Player* plr = GetHitUnit()->ToPlayer();
                if (!plr)
                    return;

                plr->TeleportTo(1492, 2931.86f, 870.70f, 515.5f, 4.73f);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_ymiron_helheim_teleport_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ymiron_helheim_teleport_SpellScript();
        }
};

void AddSC_boss_ymiron_the_fallen_king()
{
    new boss_ymiron_the_fallen_king();
    new spell_ymiron_power_regen();
    new spell_ymiron_bane();
    new spell_ymiron_helheim_teleport();
}