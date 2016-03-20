/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
    Encounter: Arch-druid glaidalis
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "darkheart_thicket.h"

 enum Says
{
    SAY_EVENT_BOSS         = 0,
    SAY_AGGRO              = 1,
    SAY_NIGHTFALL          = 2,
    SAY_RAMPAGE            = 3,
    SAY_GREIEVOUS          = 4,
    SAY_20                 = 5,
    SAY_DEATH              = 6,
    //event
    SAY_EVENT              = 0,
};

enum Spells
{
    SPELL_PRE_GRIEVOUS_LEAP     = 197709, // Используется что бы убить треш. Пред-эвент спелл.
    SPELL_GRIEVOUS_LEAP         = 196346,
    SPELL_GRIEVOUS_LEAP_RETURN  = 198095,
    SPELL_GRIEVOUS_LEAP_DOT     = 196376,
    SPELL_PRIMAL_RAMPAGE_MORPH  = 198360,
    SPELL_PRIMAL_RAMPAGE_KNOCK  = 198376,
    SPELL_PRIMAL_RAMPAGE_CHARGE = 198379,
    SPELL_NIGHTFALL             = 198400,
    //pre-event
    SPELL_VISUAL                = 197642,
};

enum eEvents
{
    EVENT_PRE_GRIEVOUS_LEAP     = 1, // Для пред эвента.
    EVENT_GRIEVOUS_LEAP         = 2,
    EVENT_PRIMAL_RAMPAGE        = 3,
    EVENT_NIGHTFALL             = 4,
};

class boss_arch_druid_glaidalis : public CreatureScript
{
public:
    boss_arch_druid_glaidalis() : CreatureScript("boss_arch_druid_glaidalis") { }

    struct boss_arch_druid_glaidalisAI : public BossAI
    {
        boss_arch_druid_glaidalisAI(Creature* creature) : BossAI(creature, DATA_GLAIDALIS) 
        {
           _introDone = false;
           introEvent = false;
           _say_20 = false;
           instance->HandleGameObject(instance->GetGuidData(GO_GLAIDALIS_INVIS_DOOR), true);
        }

        void Reset()
        {
            events.Reset();
            _Reset();
            _say_20 = false;
        }

        void EnterCombat(Unit* /*who*/) //35:36
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
            events.ScheduleEvent(EVENT_GRIEVOUS_LEAP, 6000);    //35:42
            events.ScheduleEvent(EVENT_PRIMAL_RAMPAGE, 14000);  //35:50
            events.ScheduleEvent(EVENT_NIGHTFALL, 20000);       //35:56
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
            instance->DoRemoveAurasDueToSpellOnPlayers(198408);  //from nightfall dot
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
            instance->DoRemoveAurasDueToSpellOnPlayers(198408); //from nightfall dot
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (me->HasAura(196348) && me->getVictim())
                    DoCast(me->getVictim(), SPELL_GRIEVOUS_LEAP_RETURN);
        }


        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_GRIEVOUS_LEAP_RETURN)
                DoCast(target, SPELL_GRIEVOUS_LEAP_DOT, true);
        }

        void UpdateAI(uint32 diff)
        {
           
            if (!UpdateVictim() && introEvent)
                return;
             
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
             
            rand = urand(0, 1);
            
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                   case EVENT_PRE_GRIEVOUS_LEAP:
                      if (Creature* target = me->FindNearestCreature(NPC_DRUIDIC_PRESERVER, 200.0f, true))
                       {
                           me->CastSpell(target, SPELL_PRE_GRIEVOUS_LEAP);
                           me->Kill(target);
                           events.ScheduleEvent(EVENT_PRE_GRIEVOUS_LEAP, 700);
                       } 
                       else
                       {
                           BossAI::EnterEvadeMode(); //на начальное место
                           if (Creature* trigger = me->FindNearestCreature(102851, 200.0f, true))   //trigger aura
                              trigger->RemoveAurasDueToSpell(203257);
                           instance->HandleGameObject(instance->GetGuidData(GO_GLAIDALIS_INVIS_DOOR), true);
                           introEvent = true;
                       }
                       break;
                    case EVENT_GRIEVOUS_LEAP:
                        if (rand == 1) 
                            Talk(SAY_GREIEVOUS);  // не каждый раз
                        DoCast(SPELL_GRIEVOUS_LEAP);
                        events.ScheduleEvent(EVENT_GRIEVOUS_LEAP, 16000);
                        break;
                    case EVENT_PRIMAL_RAMPAGE:                        
                        if (rand == 1) 
                            Talk(SAY_RAMPAGE);  // не каждый раз
                        DoCast(me, SPELL_PRIMAL_RAMPAGE_MORPH, true);
                        if (Unit* pTaget = me->getVictim())
                        {
                            DoCast(pTaget, SPELL_PRIMAL_RAMPAGE_KNOCK, true);
                            DoCast(pTaget, SPELL_PRIMAL_RAMPAGE_CHARGE);
                        }
                        events.ScheduleEvent(EVENT_PRIMAL_RAMPAGE, 16000);
                        break;
                    case EVENT_NIGHTFALL:
                        Talk(SAY_NIGHTFALL);
                        DoCast(SPELL_NIGHTFALL);
                        events.ScheduleEvent(EVENT_NIGHTFALL, 16000);
                        break;
                }
            }
            
              if (!_say_20 && me->GetHealth()*100 / me->GetMaxHealth() <= 20)
              {
                Talk(SAY_20);
                _say_20 = true;
              }
            DoMeleeAttackIfReady();
        }
        

         void MoveInLineOfSight(Unit* who)
         {  
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
            
             if (!_introDone && me->IsWithinDistInMap(who, 48.0f))
             {
                 _introDone = true;
                 instance->HandleGameObject(instance->GetGuidData(GO_GLAIDALIS_INVIS_DOOR), false);
                 std::list<Creature*> list;
                 list.clear();
                 me->GetCreatureListWithEntryInGrid(list, NPC_DRUIDIC_PRESERVER, 150.0f);
                 if (!list.empty())
                 {
                    Talk(SAY_EVENT_BOSS);
                    for (std::list<Creature*>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                    {
                      if (Creature* target = (*itr)->FindNearestCreature(100404, 50.0f, true))  //trigger
                        (*itr)->CastSpell(target, SPELL_VISUAL); 
                    }
                    
                     std::list<Creature*>::const_iterator itr = list.begin();
                     std::advance(itr, urand(0, list.size() - 1));
                     (*itr)->AI()->Talk(SAY_EVENT);
                  }
                 events.ScheduleEvent(EVENT_PRE_GRIEVOUS_LEAP, 3000);
             }
         }
         
     private:
      bool _introDone;
      bool _say_20;
      bool introEvent;
      uint32 rand;
    };
    

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_arch_druid_glaidalisAI (creature);
    }
};

//196346
class spell_glaidalis_grievous_leap : public SpellScriptLoader
{
public:
    spell_glaidalis_grievous_leap() : SpellScriptLoader("spell_glaidalis_grievous_leap") { }

    class spell_glaidalis_grievous_leap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_glaidalis_grievous_leap_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Creature* glaidalis = GetCaster()->ToCreature();
            if (targets.empty() || !glaidalis)
                return;

            targets.clear();

            if (Unit* target = glaidalis->AI()->SelectTarget(SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                targets.push_back(target);
        }

        void HandleOnHit()
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), 196348, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_glaidalis_grievous_leap_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_glaidalis_grievous_leap_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_glaidalis_grievous_leap_SpellScript();
    }
};

//196376
class spell_glaidalis_grievous_tear : public SpellScriptLoader
{
public:
    spell_glaidalis_grievous_tear() : SpellScriptLoader("spell_glaidalis_grievous_tear") { }

    class spell_glaidalis_grievous_tear_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_glaidalis_grievous_tear_AuraScript)

        void OnPeriodic(AuraEffect const* aurEff)
        {
            if (!GetTarget())
                return;

            if (GetTarget()->HealthAbovePct(GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                GetTarget()->RemoveAurasDueToSpell(aurEff->GetId());
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_glaidalis_grievous_tear_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_glaidalis_grievous_tear_AuraScript();
    }
};

void AddSC_boss_arch_druid_glaidalis()
{
    new boss_arch_druid_glaidalis();
    new spell_glaidalis_grievous_leap();
    new spell_glaidalis_grievous_tear();
}