/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
    Encounter: Ymiron the fallen king
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

enum Says
{
    SAY_INTRO        = 0,
    SAY_AGGRO        = 2,
    SAY_SCREAMS      = 3,
    SAY_WINDS        = 4,
    SAY_DARK         = 5,
    SAY_DEATH        = 6,
};

enum Spells
{
    SPELL_ZERO_REGEN            = 118357,
    SPELL_POWER                 = 167922,
    SPELL_KNEELING_VISUAL       = 197227, //State
    SPELL_SOUL_SIPHON           = 194665, //HitTarget npc 102375
    SPELL_DARK_SLASH            = 193211,
    SPELL_SCREAMS_OF_DEAD       = 193364,
    SPELL_WINDS_OF_NORTHREND    = 193977,
    SPELL_BANE                  = 193460,
    
    //trash
    SPELL_STATE                 = 196025,
    SPELL_INSTAKILL             = 117624,
};

enum eEvents
{
    EVENT_DARK_SLASH            = 1,
    EVENT_SCREAMS_OF_DEAD       = 2,
    EVENT_WINDS_OF_NORTHREND    = 3,
    EVENT_BANE                  = 4,
    EVENT_INTRO                 = 5,
   // EVENT_INTRO_2               = 6,
    //trash
    EVENT_TRASH_INTRO           = 7,
    EVENT_TRASH_INTRO_2         = 8,
    EVENT_FRACTURE              = 9,
    EVENT_BARBED                = 10,
    EVENT_SWIRLING              = 11,
};

class boss_ymiron_the_fallen_king : public CreatureScript
{
public:
    boss_ymiron_the_fallen_king() : CreatureScript("boss_ymiron_the_fallen_king") { }

    struct boss_ymiron_the_fallen_kingAI : public BossAI
    {
        boss_ymiron_the_fallen_kingAI(Creature* creature) : BossAI(creature, DATA_YMIRON) 
        {
            me->ApplySpellImmune(0, IMMUNITY_ID, 194665, true);
        }

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
            Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_POWER, true);
            events.ScheduleEvent(EVENT_SCREAMS_OF_DEAD, 6000);      //12:15, 12:38, 13:01
            events.ScheduleEvent(EVENT_WINDS_OF_NORTHREND, 16000);  //12:25, 12:49, 13:19, 13:44
            //events.ScheduleEvent(EVENT_BANE, 22000);                //12:31, 13:23, 14:13 - нужна инфа по АТ
            me->SetWalk(false);
            me->RemoveAurasDueToSpell(SPELL_KNEELING_VISUAL);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
            if (GameObject* go = instance->instance->GetGameObject(instance->GetGuidData(DATA_YMIRON_GORN)))
                go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_1)
                events.ScheduleEvent(EVENT_DARK_SLASH, 500);
            if (action == ACTION_2)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetWalk(true);
                timer = 1000;
                phase = 0;
                _iventIntro = false;
            }
        }

        void UpdateAI(uint32 diff)
        {
             if (!UpdateVictim() && _iventIntro)
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
                         Talk(SAY_DARK);
                        break;
                    case EVENT_SCREAMS_OF_DEAD:
                        DoCast(SPELL_SCREAMS_OF_DEAD);
                        events.ScheduleEvent(EVENT_SCREAMS_OF_DEAD, 23000);
                        Talk(SAY_SCREAMS);
                        break;
                    case EVENT_WINDS_OF_NORTHREND:
                        DoCast(SPELL_WINDS_OF_NORTHREND);
                        events.ScheduleEvent(EVENT_WINDS_OF_NORTHREND, 24000);
                        Talk(SAY_WINDS);
                        break;
                    case EVENT_BANE:
                        DoCast(SPELL_BANE);
                        events.ScheduleEvent(EVENT_BANE, 50000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
         switch (phase) 
         {
             case 0:
                 if (timer < diff) {
                     me->GetMotionMaster()->MoveJump(7406.41f, 7280.58f, 44.26f, 5, 5);
                     phase++;
                     timer = 5000;
                 }
                 else timer -= diff;           
                 break;
                 case 1:
                 if (timer < diff) {
                     me->GetMotionMaster()->MovePoint(0, 7408.16f, 7293.06f, 43.78f);
                     phase++;
                     timer = 5000;
                 }
                 else timer -= diff;           
                 break;
                 case 2:
                 if (timer < diff) {
                     me->GetMotionMaster()->MovePoint(0, 7397.77f, 7297.18f, 43.78f);
                     phase++;
                     timer = 5000;
                 }
                 else timer -= diff;           
                 break;
                 case 3:
                 if (timer < diff) {
                     std::list<Creature*> list;
                     list.clear();
                     me->GetCreatureListWithEntryInGrid(list, 102375, 18.0f);
                     
                        if (!list.empty())
                        {
                            for (std::list<Creature*>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                            {
                                (*itr)->CastSpell((*itr), SPELL_SOUL_SIPHON);
                            }
                        }                    
                     DoCast(SPELL_SOUL_SIPHON);
                     Talk(SAY_INTRO);
                     phase++;
                     timer = 12000;
                 }
                 else timer -= diff;           
                 break;
                 case 4:
                 if (timer < diff) {
                     me->GetMotionMaster()->MovePoint(0, 7396.58f, 7276.02f, 43.79f);
                     Talk(1);
                     std::list<Creature*> list;
                     list.clear();
                     me->GetCreatureListWithEntryInGrid(list, 102375, 18.0f);
                     
                        if (!list.empty())
                        {
                            for (std::list<Creature*>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                            {
                                (*itr)->CastSpell((*itr), SPELL_INSTAKILL);
                            }
                        }                      
                     phase++;
                     timer = 5000;
                 }
                 else timer -= diff;           
                 break;
                 case 5:
                 if (timer < diff) {
                     if (GameObject* GO = me->FindNearestGameObject(247041, 30.0f))
                        me->SetFacingToObject(GO);
                     DoCast(SPELL_KNEELING_VISUAL);
                     phase++;
                     timer = 1000;
                 }
                 else timer -= diff;           
                 break;
                 case 6:
                 if (timer < diff) {
                     me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                     me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                     me->SetWalk(false); 
                     phase++;
                     timer = 5000;
                     _iventIntro = true;
                 }
                 else timer -= diff;           
                 break;                                       
         }
        }
      
        
        private:
         bool _iventIntro;
         uint32 timer;
         int phase;
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

class npc_trigger_mos_start : public CreatureScript
{
    public:
        npc_trigger_mos_start() : CreatureScript("npc_trigger_mos_start") { }

        struct npc_trigger_mos_startAI : public ScriptedAI
        {
            npc_trigger_mos_startAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = creature->GetInstanceScript();
                _introDone = false;
                _pull = false;
                me->SetVisible(false);
            }

         void MoveInLineOfSight(Unit* who)
         {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!_introDone && me->IsWithinDistInMap(who, 45.0f))
             {
                _introDone = true;
                std::list<Creature*> list;
                list.clear();
                me->GetCreatureListWithEntryInGrid(list, NPC_BIRD, 50.0f);
                
                   if (!list.empty())
                   {
                       for (std::list<Creature*>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                       {
                           (*itr)->RemoveAurasDueToSpell(SPELL_STATE);
                           (*itr)->GetMotionMaster()->MovePoint(0, 7257.08f, 7081.54f, 26.41f);
                       }
                   }
             }
             
             if (!_pull && me->IsWithinDistInMap(who, 20.0f))
             {
                  _pull = true;
                 if (Creature* trigger = me->FindNearestCreature(NPC_RUNECARVER, 20.0f, true))
                 {
                     trigger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                     if (Creature* target = me->FindNearestCreature(97043, 30.0f, true))
                         target->AI()->DoAction(ACTION_2);
                     
                     if (Creature* ymiron = me->FindNearestCreature(NPC_YMIRON, 130.0f, true))
                           ymiron->AI()->DoAction(ACTION_2);
                 }
                 if (Creature* trigger = me->FindNearestCreature(NPC_SHIELDMAIDEN, 20.0f, true))
                 {
                     trigger->AI()->Talk(0);
                     trigger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                     if (Creature* target = me->FindNearestCreature(97043, 30.0f, true))
                         target->AI()->DoAction(ACTION_1);;
                 }
             }
             
             ScriptedAI::MoveInLineOfSight(who);
         }
            
            
            
        private:
            bool _introDone;
            bool _pull;
            InstanceScript* _instance;
        };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_trigger_mos_startAI (creature);
    }
};

//97043
class npc_referee : public CreatureScript
{
    public:
        npc_referee() : CreatureScript("npc_referee") { }

        struct npc_refereeAI : public ScriptedAI
        {
            npc_refereeAI(Creature* creature) : ScriptedAI(creature) {}

            
        void DoAction(int32 const action)
        {
            if (action == ACTION_1)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                _events.ScheduleEvent(EVENT_TRASH_INTRO, 3000);
                introEvent = false;
            }
            if (action == ACTION_2)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                _events.ScheduleEvent(EVENT_TRASH_INTRO_2, 1000);
                introEvent = false;
            }
        }
        
        void EnterCombat(Unit* /*who*/) 
        {
           _events.ScheduleEvent(EVENT_FRACTURE, 15000);
           _events.ScheduleEvent(EVENT_BARBED, 9000);
           _events.ScheduleEvent(EVENT_SWIRLING, 17000);
        }
        void UpdateAI(uint32 diff)
        {
             if (!UpdateVictim() && introEvent)
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TRASH_INTRO:
                        me->GetMotionMaster()->MovePoint(0, 7267.00f, 7289.00f, 27.16f);
                        Talk(0);
                        if (GameObject* GO = me->FindNearestGameObject(245064, 20.0f))
                          GO->SetGoState(GO_STATE_ACTIVE);
                        Talk(1);
                        if (Unit* target = me->FindNearestPlayer(100.0f))
                        {                           
                           if (Creature* trigger = me->FindNearestCreature(NPC_SHIELDMAIDEN, 30.0f, true))
                           {
                                 trigger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                                 trigger->AI()->AttackStart(target);
                           }
                           me->AI()->AttackStart(target);
                        }
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        introEvent = true;
                    break;
                    case EVENT_TRASH_INTRO_2:
                         me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                         if (GameObject* GO = me->FindNearestGameObject(245062, 20.0f))
                          GO->SetGoState(GO_STATE_ACTIVE);
                       if (Unit* target = me->FindNearestPlayer(100.0f))
                        {   
                            if (Creature* trigger = me->FindNearestCreature(NPC_RUNECARVER, 20.0f, true))
                            {
                               trigger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                               trigger->AI()->AttackStart(target);
                            }
                            me->AI()->AttackStart(target);
                        }
                         me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                         
                         Talk(3);
                        introEvent = true;
                    break;
                   case EVENT_FRACTURE:
                         DoCast(198752);
                         _events.ScheduleEvent(EVENT_FRACTURE, 15000);
                   break;
                   case EVENT_BARBED:
                         DoCast(194674);
                         _events.ScheduleEvent(EVENT_BARBED, 9000);
                   break;                   
                   case EVENT_SWIRLING:
                         DoCast(201567);
                         _events.ScheduleEvent(EVENT_SWIRLING, 17000);
                   break;
                   
                }
            }
            DoMeleeAttackIfReady();
        }
        
        
        
        private:
         EventMap _events;
         InstanceScript* _instance;
         bool introEvent;
     };
         CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_refereeAI (creature);
        }
};



void AddSC_boss_ymiron_the_fallen_king()
{
    new boss_ymiron_the_fallen_king();
    new spell_ymiron_power_regen();
    new spell_ymiron_bane();
    new spell_ymiron_helheim_teleport();
    new npc_trigger_mos_start();
    new npc_referee();
}