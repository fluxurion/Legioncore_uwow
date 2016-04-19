/*
    http://uwow.biz
    Dungeon : Neltharions Lair 100-110
    Encounter: Rokmora
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "neltharions_lair.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_RAZOR           = 1,
    SAY_DEATH           = 2,
}; 

enum Spells
{
    SPELL_GAIN_ENERGY           = 193245,
    SPELL_BRITTLE               = 187714,
    SPELL_INTRO_ULAROGG         = 209390, //Boss 01 Intro Ularogg Cast
    SPELL_INTRO_EMERGE          = 209394, //Boss 01 Intro Emerge

    SPELL_SHATTER_START_CALL_1  = 198122, //Conversation Shatter
    SPELL_SHATTER_END_CALL_1    = 198135, //Conversation Shatter
    SPELL_SHATTER_END_CALL_2    = 198136, //Conversation Shatter
    SPELL_SHATTER               = 188114,
    SPELL_RAZOR_SHARDS_CALL     = 198125, //Conversation Razor Shards
    SPELL_RAZOR_SHARDS          = 209718,

    SPELL_CHOKING_DUST_AT       = 192799,
};

enum eEvents
{
    EVENT_RAZOR_SHARDS          = 1,
    EVENT_DEAD_CONVERSATION     = 2,
};

class boss_rokmora : public CreatureScript
{
public:
    boss_rokmora() : CreatureScript("boss_rokmora") { }

    struct boss_rokmoraAI : public BossAI
    {
        boss_rokmoraAI(Creature* creature) : BossAI(creature, DATA_ROKMORA) 
        {
              introDone = false;
        }

        
        bool introDone;
        
        void Reset()
        {
            _Reset();

            DoCast(me, SPELL_BRITTLE, true);
            me->RemoveAurasDueToSpell(SPELL_GAIN_ENERGY);
            me->SetMaxPower(POWER_MANA, 100);
            me->SetPower(POWER_MANA, 0);        
        }

        void EnterCombat(Unit* /*who*/) //53:15
        {
            Talk(SAY_AGGRO); //Rok SMASH!
            _EnterCombat();
            DoCast(me, SPELL_GAIN_ENERGY, true);

            events.ScheduleEvent(EVENT_RAZOR_SHARDS, 30000); //53:44, 54:09, 54:35
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
            events.ScheduleEvent(EVENT_DEAD_CONVERSATION, 3000);
        }

        void MoveInLineOfSight(Unit* who)
        {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!introDone && me->IsWithinDistInMap(who, 40.0f))
             {
                DoCast(209374);
                introDone = true;
             }
        }
        
        void UpdateAI(uint32 diff)
        {
            if (!introDone)
               if (Creature* ularogg = me->FindNearestCreature(105300, 50.0f, true)
               {
                  ularogg->CastSpell(me, SPELL_INTRO_ULAROGG);
                  me->CastSpell(me, SPELL_INTRO_EMERGE);
               }
               
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RAZOR_SHARDS:
                        DoCast(me, SPELL_RAZOR_SHARDS_CALL, true);
                        DoCast(SPELL_RAZOR_SHARDS);
                        Talk(SAY_RAZOR);
                        events.ScheduleEvent(EVENT_RAZOR_SHARDS, 26000);
                        break;
                    case EVENT_DEAD_CONVERSATION:
                        //DoCast(199392); пока офф, крашит
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_rokmoraAI (creature);
    }
};

//97720
class npc_rokmora_blightshard_skitter : public CreatureScript
{
public:
    npc_rokmora_blightshard_skitter() : CreatureScript("npc_rokmora_blightshard_skitter") {}

    struct npc_rokmora_blightshard_skitterAI : public ScriptedAI
    {
        npc_rokmora_blightshard_skitterAI(Creature* creature) : ScriptedAI(creature) 
        {
            fakeDied = false;
        }

        bool fakeDied;

        void Reset() {}

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (damage >= me->GetHealth())
            {
                if (!fakeDied)
                {
                    fakeDied = true;
                    DoStopAttack();
                    DoCast(me, SPELL_CHOKING_DUST_AT, true);
                    DoCast(me, 29266, true); //Fake Death
                }
                damage = 0;
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_rokmora_blightshard_skitterAI(creature);
    }
};

//193245
class spell_rokmora_gain_energy : public SpellScriptLoader
{
public:
    spell_rokmora_gain_energy() : SpellScriptLoader("spell_rokmora_gain_energy") { }

    class spell_rokmora_gain_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rokmora_gain_energy_AuraScript);

        bool checkTalk = false;

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (caster->GetPower(POWER_MANA) == 0 && checkTalk)
            {
                checkTalk = false;
                uint8 rand = urand(0, 1);
                caster->CastSpell(caster, rand ? SPELL_SHATTER_END_CALL_1 : SPELL_SHATTER_END_CALL_2, true);
            }

            if (caster->GetPower(POWER_MANA) >= 100)
            {
                caster->CastSpell(caster, SPELL_SHATTER_START_CALL_1, true);
                caster->CastSpell(caster, SPELL_SHATTER);
                checkTalk = true;
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rokmora_gain_energy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_rokmora_gain_energy_AuraScript();
    }
};

// trash 183433
class spell_submerge : public SpellScriptLoader
{
    public:
        spell_submerge() : SpellScriptLoader("spell_submerge") { }

        class spell_submerge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_submerge_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                std::list<Player*> targets;
                GetPlayerListInGrid(targets, target, 40);
                Trinity::Containers::RandomResizeList(targets, 1);
                for (std::list<Creature*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                  if (!targets.empty())
                    target->CastSpell((*itr), 183430, true);
                    target->CastSpell((*itr), 183438, false);
                    
                    target->AI()SetVisible(true);          
            }
            
            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
               if (Unit* target = GetTarget())
                  target->AI()->SetVisible(false);      
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_submerge_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_submerge_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_submerge_AuraScript();
        }
};

void AddSC_boss_rokmora()
{
    new boss_rokmora();
    new npc_rokmora_blightshard_skitter();
    new spell_rokmora_gain_energy();
    // trash
    new spell_submerge();
}