/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
    Encounter: Helya
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

enum Says
{
    SAY_INTRO           = 0,
    SAY_AGGRO           = 1,
    SAY_TAINT_SEA       = 2,
    SAY_97              = 3,
    SAY_94              = 4,
    SAY_90              = 5,
    SAY_87              = 6,
    SAY_PHASE2          = 7,
    SAY_TENTACLE        = 8,
    SAY_DEATH           = 9,
};

enum Spells
{
    //Intro
    SPELL_SOULLESS_SCREAM       = 194603,

    SPELL_INTERFERE_TARGETTING  = 197710,
    SPELL_TAINT_OF_THE_SEA      = 197517,
    SPELL_TORRENT               = 197805,
    //Phase 2
    SPELL_SUBMERGED             = 196947, //32:34
    SPELL_TRANSFORM             = 197734, //32:44 - Self hit
    //SMSG_MOVE_UPDATE_TELEPORT 32:44 - 2933.5 Y: 961.792 Z: 510.38 O: 4.712389
    SPELL_SMASH                 = 197689, //Force cast all players
    //Tentacle
    SPELL_GRASPING_STUN_LEFT    = 198180,
    SPELL_GRASPING_STUN_RIGHT   = 196450,
    SPELL_PIERCING_TENTACLE     = 197112,
    SPELL_SWIRLING_POOL_AT      = 195167,
    SPELL_SWIRLING_POOL_DMG     = 195309,
    SPELL_SWIRLING_POOL_JUMP    = 194839,
    
    //trash
    SPELL_GIVE_NO               = 196885,
    SPELL_DEBILITATING          = 195293,
    SPELL_BIND                  = 195279,
};

enum eEvents
{
    EVENT_INTRO                 = 1,
    EVENT_TAINT_SEA             = 2,
    EVENT_TORRENT               = 3,
    EVENT_SUM_PIERCING_TENTACLE = 4,
    EVENT_SUBMERGED_START       = 5,
    EVENT_SUBMERGED_END         = 6,
    
    //trash
    EVENT_SUMMON_1              = 7,
    EVENT_SUMMON_2              = 8,
    EVENT_GIVE_NO               = 9,
    EVENT_DEBILITATING          = 10,
    EVENT_BIND                  = 11,
};

enum eVisualKits
{
    VISUAL_KIT_1 = 60920, //30:14 - visual spawn
    VISUAL_KIT_2 = 62877, //30:22 - start intro
    VISUAL_KIT_3 = 60222, //30:28
};

Position const tentaclePos[15] =
{
//1 (100% -> 90%)
    {2933.28f, 960.37f, 512.42f, 4.71f}, //NPC_DESTRUCTOR_TENTACLE
    {2942.16f, 967.02f, 513.79f, 4.08f}, //NPC_GRASPING_TENTACLE_1
    {2925.54f, 965.84f, 512.26f, 5.65f}, //NPC_GRASPING_TENTACLE_2
//2 (90% -> 87%)
    {2919.81f, 959.98f, 513.21f, 5.85f}, //NPC_GRASPING_TENTACLE_3
//3 (87% -> 83%)
    {2933.28f, 960.37f, 512.42f, 4.71f}, //NPC_DESTRUCTOR_TENTACLE
    {2933.40f, 941.15f, 512.42f, 4.71f},
    {2948.52f, 921.45f, 512.42f, 4.71f},
//4 (83% -> 80%)
    {2946.55f, 959.81f, 513.23f, 3.58f}, //NPC_GRASPING_TENTACLE_4
//Swirling Pool VEH - 97099
    {2939.53f, 928.46f, 512.41f, 1.57f},
    {2922.20f, 937.07f, 512.41f, 1.57f},
    {2927.44f, 928.84f, 512.41f, 1.57f},
    {2948.52f, 921.45f, 512.41f, 1.57f},
    {2933.40f, 941.15f, 512.41f, 1.57f},
    {2943.52f, 935.80f, 512.41f, 1.57f},
    {2918.25f, 921.30f, 512.41f, 1.57f},
};

// skyal
Position const addsPos[2] =
{
    {2942.57f, 886.99f, 537.76f, 4.48f},
    {2924.55f, 887.20f, 537.65f, 5.26f},
};
uint32 swirlingGO[8]
{
    GO_SWIRLING_POOL_1,
    GO_SWIRLING_POOL_2,
    GO_SWIRLING_POOL_3,
    GO_SWIRLING_POOL_4,
    GO_SWIRLING_POOL_5,
    GO_SWIRLING_POOL_6,
    GO_SWIRLING_POOL_7,
    GO_SWIRLING_POOL_8, 
};

class boss_helya : public CreatureScript
{
public:
    boss_helya() : CreatureScript("boss_helya") { }

    struct boss_helyaAI : public BossAI
    {
        boss_helyaAI(Creature* creature) : BossAI(creature, DATA_HELYA)
        {
            SetCombatMovement(false);
            introSpawn = false;
            introEvent = false;
        }

        bool introSpawn, introEvent, encounterComplete;
        uint8 tentacleDiedCound;
        uint8 idxSumm;
        uint8 healthSwitch;
        uint8 healthSay;

        void Reset()
        {
            events.Reset();
            _Reset();
            tentacleDiedCound = 0;
            idxSumm = 0;
            healthSwitch = 0;
            healthSay = healthSwitch + 3;
            encounterComplete = false;
            UpdateShipState(STATE_REBUILDING);

            if (introEvent)
                SummonTentacle(idxSumm);

            for (uint8 i = 8; i < 15; i++)
                me->SummonCreature(NPC_SWIRLING_POOL_VEH, tentaclePos[i]);

            for (uint8 i = 0; i < 8; i++)
                if (GameObject* go = me->FindNearestGameObject(swirlingGO[i], 200.0f))
                    go->SetGoState(GO_STATE_READY);
        }

        void EnterCombat(Unit* /*who*/) //30:46
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
            events.ScheduleEvent(EVENT_TAINT_SEA, 0);  //30:46, 30:58, 31:12, 31:27
            events.ScheduleEvent(EVENT_TORRENT, 3000); //30:49, 30:53, 31:00, 31:04
            events.ScheduleEvent(EVENT_SUM_PIERCING_TENTACLE, 8000);

            std::list<Creature*> tentacle;
            GetCreatureListWithEntryInGrid(tentacle, me, 99801, 200.0f); //DESTRUCTOR_TENTACLE
            GetCreatureListWithEntryInGrid(tentacle, me, 98363, 200.0f); //NPC_GRASPING_TENTACLE_1
            GetCreatureListWithEntryInGrid(tentacle, me, 100362, 200.0f); //NPC_GRASPING_TENTACLE_2
            for (std::list<Creature*>::iterator itr = tentacle.begin(); itr != tentacle.end(); ++itr)
                if (!(*itr)->isInCombat())
                    (*itr)->AI()->DoZoneInCombat((*itr), 150.0f);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
            DoTeleportTo(2933.5f, 1055.94f, 512.463f, 4.79f); //HomePos
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (who->ToPlayer()->isGameMaster())
                return;

            if (me->GetDistance(who) < 140.0f && !introSpawn) //30:14
            {
                introSpawn = true;
                me->SendPlaySpellVisualKit(VISUAL_KIT_1, 0);
            }

            if (me->GetDistance(who) < 110.0f && !introEvent) //30:22
            {
                Talk(SAY_INTRO); //You ALL will regret trespassing in my realm.
                me->SendPlaySpellVisualKit(VISUAL_KIT_2, 0);
                events.ScheduleEvent(EVENT_INTRO, 5000);
            }
        }

        void SummonTentacle(uint8 idPos)
        {
            switch (idPos)
            {
                case 0:
                    me->SummonCreature(NPC_DESTRUCTOR_TENTACLE, tentaclePos[0]);
                    me->SummonCreature(NPC_GRASPING_TENTACLE_1, tentaclePos[1]);
                    me->SummonCreature(NPC_GRASPING_TENTACLE_2, tentaclePos[2]);
                    break;
                case 1:
                    me->SummonCreature(NPC_GRASPING_TENTACLE_3, tentaclePos[3]);
                    break;
                case 2:
                    me->SummonCreature(NPC_DESTRUCTOR_TENTACLE, tentaclePos[urand(4,6)]);
                    me->SummonCreature(NPC_GRASPING_TENTACLE_4, tentaclePos[7]);
                    break;
                default:
                    return;
            }
            idxSumm++;
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            switch (summon->GetEntry())
            {
                case NPC_DESTRUCTOR_TENTACLE:
                case NPC_GRASPING_TENTACLE_1:
                case NPC_GRASPING_TENTACLE_2:
                case NPC_GRASPING_TENTACLE_3:
                case NPC_GRASPING_TENTACLE_4:
                    tentacleDiedCound++;
                    break;
                default:
                    return;
            }

            switch (tentacleDiedCound)
            {
                case 3:              
                case 4:              
                case 5:
                    SummonTentacle(idxSumm);
                    break;
                case 6:
                    events.ScheduleEvent(EVENT_SUBMERGED_START, 1000);
                    Talk(SAY_87);
                    break;
                default:
                    break;
            }
            
            if (healthSwitch < 2)
            {
                me->SetHealth(me->GetHealth() - me->CountPctFromMaxHealth(3));
                healthSwitch++;
                healthSay = healthSwitch + 3;
                Talk(healthSay);
            }
            else
            {
                me->SetHealth(me->GetHealth() - me->CountPctFromMaxHealth(4));
                healthSwitch = 0;
                healthSay = healthSwitch + 3;
                Talk(healthSay);
            }
        }

        void UpdateShipState(uint8 state)
        {
            if (GameObject* pGo = instance->instance->GetGameObject(instance->GetGuidData(DATA_SHIP)))
            {
                if (state == STATE_DESTROY)
                {
                    pGo->SetDisplayId(31853);
                    pGo->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED, nullptr, true);
                }
                else if (state == STATE_REBUILDING)
                {
                    pGo->SetDisplayId(31852);
                    pGo->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, nullptr, true);
                }

                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* plr = itr->getSource())
                        pGo->SendUpdateToPlayer(plr);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(71) && !encounterComplete)
            {
                encounterComplete = true;
                events.Reset();
                DoStopAttack();
                Talk(SAY_DEATH);
                instance->SetBossState(DATA_HELYA, DONE);
                me->DespawnOrUnsummon();
            }            
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && introEvent)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_INTRO:
                    {
                        if (introEvent)
                            break;
                        introEvent = true;
                        me->SendPlaySpellVisualKit(VISUAL_KIT_3, 0);
                        DoCast(me, SPELL_SOULLESS_SCREAM, true);
                        DoCast(me, SPELL_INTERFERE_TARGETTING, true);
                        SummonTentacle(idxSumm);
                        break;
                    }
                    case EVENT_TAINT_SEA:
                        DoCast(SPELL_TAINT_OF_THE_SEA);
                        events.ScheduleEvent(EVENT_TAINT_SEA, 12000);
                        Talk(SAY_TAINT_SEA);
                        break;
                    case EVENT_TORRENT:
                        DoCast(SPELL_TORRENT);
                        events.ScheduleEvent(EVENT_TORRENT, 5000);
                        break;
                    case EVENT_SUM_PIERCING_TENTACLE:
                    {
                        std::list<Creature*> tentacle;
                        GetCreatureListWithEntryInGrid(tentacle, me, NPC_SWIRLING_POOL_VEH, 200.0f);
                        Trinity::Containers::RandomResizeList(tentacle, 1);
                        for (std::list<Creature*>::iterator itr = tentacle.begin(); itr != tentacle.end(); ++itr)
                            if (!tentacle.empty())
                                (*itr)->AI()->DoAction(true);
                        events.ScheduleEvent(EVENT_SUM_PIERCING_TENTACLE, 8000);
                        break;
                    }
                    case EVENT_SUBMERGED_START:
                        me->InterruptNonMeleeSpells(false);
                        DoCast(me, SPELL_SUBMERGED, true);
                        Talk(SAY_PHASE2);
                        events.ScheduleEvent(EVENT_SUBMERGED_END, 10000);
                        break;
                    case EVENT_SUBMERGED_END:
                        me->RemoveAllAuras();
                        DoTeleportTo(2933.5f, 961.79f, 512.38f, 4.71f);
                        me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_PREVENT_SELECT_NPC);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                        DoCast(me, SPELL_TRANSFORM, true);
                        DoCast(me, SPELL_SMASH, true);
                        UpdateShipState(STATE_DESTROY);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_helyaAI (creature);
    }
};

//98363, 99801, 100354, 100360, 100362, 100188
class npc_helya_tentacle : public CreatureScript
{
public:
    npc_helya_tentacle() : CreatureScript("npc_helya_tentacle") { }

    struct npc_helya_tentacleAI : public ScriptedAI
    {
        npc_helya_tentacleAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = me->GetInstanceScript();
            me->SetReactState(REACT_DEFENSIVE);
        }

        InstanceScript* instance;
        EventMap events;
        bool find;

        void Reset() {}

        void EnterCombat(Unit* /*who*/) //30:46
        {
            if (instance && instance->GetBossState(DATA_HELYA) != IN_PROGRESS)
                if (Creature* helya = instance->instance->GetCreature(instance->GetGuidData(DATA_HELYA)))
                    helya->SetInCombatWithZone();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (me->ToTempSummon())
                if (Unit* summoner = me->ToTempSummon()->GetSummoner())
                    summoner->Kill(summoner);
        }
        
         void CheckPlrInBattleZone()
           {
               find = false;
               const Map::PlayerList &PlayerList = me->GetMap()->GetPlayers();
               if (PlayerList.isEmpty())
                   return;
               for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
               {
                   if (Player* plr = i->getSource())
                   {
                       if (!plr->isAlive() || plr->isGameMaster())
                           return;
    
                       if (plr->GetPositionX() > (me->GetPositionX()-5) && plr->GetPositionX() < (me->GetPositionX()+5) &&
                           plr->GetPositionY() > (me->GetPositionY()-5) && plr->GetPositionY() < (me->GetPositionY()+5))
                       {
                           find = true;
                       } 
                   }
               }
               if (!find)
               {
                   events.ScheduleEvent(EVENT_6, 1000);
               }
           }
           
        void IsSummonedBy(Unit* summoner)
        {
            switch (me->GetEntry())
            {
                case 100360: // NPC_GRASPING_TENTACLE_3
                case 100362: // NPC_GRASPING_TENTACLE_2
                    events.ScheduleEvent(EVENT_1, 1000);
                    break;
                case 98363:  // NPC_GRASPING_TENTACLE_1
                case 100354: // NPC_GRASPING_TENTACLE_4
                    events.ScheduleEvent(EVENT_2, 1000);
                    break;
                case 100188:
                    events.ScheduleEvent(EVENT_3, 1000);
                    return;
                case 99801:
                    events.ScheduleEvent(EVENT_5, 2000);
                    events.ScheduleEvent(EVENT_7, 1000);
                    Talk(0);
                    break;
                default:
                    break;
            }
            if (instance->GetBossState(DATA_HELYA) == IN_PROGRESS)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                DoZoneInCombat(me, 100.0f);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && me->isInCombat())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(me, SPELL_GRASPING_STUN_LEFT, true);
                        break;
                    case EVENT_2:
                        DoCast(me, SPELL_GRASPING_STUN_RIGHT, true);
                        break;
                    case EVENT_3:
                        me->CastSpell(me, SPELL_PIERCING_TENTACLE, TriggerCastFlags(TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE));
                        events.ScheduleEvent(EVENT_4, 4000);
                        break;
                    case EVENT_4:
                        me->SetDisplayId(11686);
                        break;
                    case EVENT_5:
                        if (Unit* target = me->FindNearestPlayer(5))
                           me->CastSpell(target, 185539, TriggerCastFlags(TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE));
                        events.ScheduleEvent(EVENT_5, 6000);
                        break;
                    case EVENT_6:
                        if (Unit* target = me->FindNearestPlayer(50))
                           me->CastSpell(target, 196534, TriggerCastFlags(TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE));
                           Talk(1);                        
                        break;
                    case EVENT_7:
                        CheckPlrInBattleZone();                    
                        events.ScheduleEvent(EVENT_7, 1000);
                        break;

                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_helya_tentacleAI(creature);
    }
};

//97099
class npc_helya_tentacle_veh : public CreatureScript
{
public:
    npc_helya_tentacle_veh() : CreatureScript("npc_helya_tentacle_veh") { }

    struct npc_helya_tentacle_vehAI : public ScriptedAI
    {
        npc_helya_tentacle_vehAI(Creature* creature) : ScriptedAI(creature), summons(me) 
        {
            me->SetReactState(REACT_PASSIVE);
            createAT = false;
        }

        SummonList summons;
        bool createAT;

        void Reset(){}

        void DoAction(int32 const action)
        {
            if (!createAT)
            {
                createAT = true;
                DoCast(me, SPELL_SWIRLING_POOL_AT, true);
                for (uint8 i = 0; i < 8; i++)
                    if (GameObject* go = me->FindNearestGameObject(swirlingGO[i], 1.0f))
                        go->SetGoState(GO_STATE_ACTIVE);
            }
            float x = me->GetPositionX();
            float y = me->GetPositionY();
            float z = me->GetPositionZ();
            me->SummonCreature(NPC_PIERCING_TENTACLE, x, y, z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 6 * IN_MILLISECONDS);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            summon->SetReactState(REACT_PASSIVE);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_SWIRLING_POOL_DMG)
            {
                Position pos;
                me->GetNearPosition(pos, 5.0f, frand(0.0f, 5.5f));
                target->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 5, SPELL_SWIRLING_POOL_JUMP, true);
            }
        }

        void UpdateAI(uint32 diff){}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_helya_tentacle_vehAI(creature);
    }
};

// trash 199589
class spell_whirpool_of_souls : public SpellScriptLoader
{
    public:                                                      
        spell_whirpool_of_souls() : SpellScriptLoader("spell_whirpool_of_souls") { }

        class spell_whirpool_of_souls_AuraScript : public AuraScript 
        {
            PrepareAuraScript(spell_whirpool_of_souls_AuraScript) 

            void OnPereodic(AuraEffect const* aurEff) 
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                caster->CastSpell(caster, 199519);
                caster->CastSpell(caster->SelectNearbyTarget(), 199519);
                
            }

            void Register() 
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_whirpool_of_souls_AuraScript::OnPereodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_whirpool_of_souls_AuraScript();
        }
};

//200208
class spell_brackwater : public SpellScriptLoader
{
    public:
        spell_brackwater() : SpellScriptLoader("spell_brackwater") { }

        class spell_brackwater_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_brackwater_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                if (Creature* pTarget = target->FindNearestCreature(98919, 40.0f, true))
                  {
                      pTarget->CastSpell(target, 201397, true);
                  }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_brackwater_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_brackwater_AuraScript();
        }
};

//99307
class npc_skyal : public CreatureScript
{
    public:
        npc_skyal() : CreatureScript("npc_skyal") { }

        struct npc_skyalAI : public ScriptedAI
        {
            npc_skyalAI(Creature* creature) : ScriptedAI(creature) 
            {
               _introDone = false;
            }
            
            
         void MoveInLineOfSight(Unit* who)
         {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!_introDone && me->IsWithinDistInMap(who, 148.0f))
             {
                _introDone = true;
                Talk(0);
             }
         }
          
        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SUMMON_1, 1000);
            events.ScheduleEvent(EVENT_SUMMON_2, 10000);
            events.ScheduleEvent(EVENT_GIVE_NO, 7000);
            events.ScheduleEvent(EVENT_DEBILITATING, 13000);
            events.ScheduleEvent(EVENT_BIND, 17000);
        }
        
        void JustDied(Unit* /*killer*/)
        {
            Talk(2);
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
                   case EVENT_SUMMON_1:
                        me->SummonCreature(98973, addsPos[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 4000);
                        me->SummonCreature(98973, addsPos[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 4000);
                        Talk(1);
                        events.ScheduleEvent(EVENT_SUMMON_1, 18000);
                        break;
                   case EVENT_SUMMON_2:
                        me->SummonCreature(99447, addsPos[0], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 4000);
                        me->SummonCreature(99447, addsPos[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 4000);
                        Talk(1);
                        events.ScheduleEvent(EVENT_SUMMON_2, 18000);
                        break;
                   case EVENT_GIVE_NO:
                        if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            me->CastSpell(target, SPELL_GIVE_NO);
                        events.ScheduleEvent(EVENT_GIVE_NO, 13000);
                        break;
                   case EVENT_DEBILITATING:
                        DoCast(SPELL_DEBILITATING);
                        events.ScheduleEvent(EVENT_DEBILITATING, 13000);
                        break;
                   case EVENT_BIND:
                        DoCast(SPELL_BIND);
                        events.ScheduleEvent(EVENT_BIND, 13000);
                        break;
                }
                
            }
            
            DoMeleeAttackIfReady();
        }
                    
            
            private:
               bool _introDone;
               EventMap events;
        };
         CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_skyalAI (creature);
        }
};

void AddSC_boss_helya()
{
    new boss_helya();
    new npc_helya_tentacle();
    new npc_helya_tentacle_veh();
    new spell_whirpool_of_souls();
    new spell_brackwater();
    new npc_skyal();
}