/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
    Encounter: Helya
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

/* enum Says
{
    SAY_INTRO           = ,
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

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
};

enum eEvents
{
    EVENT_INTRO                 = 1,
    EVENT_TAINT_SEA             = 2,
    EVENT_TORRENT               = 3,
    EVENT_SUM_PIERCING_TENTACLE = 4,
    EVENT_SUBMERGED_START       = 5,
    EVENT_SUBMERGED_END         = 6,
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

        void Reset()
        {
            events.Reset();
            _Reset();
            tentacleDiedCound = 0;
            idxSumm = 0;
            healthSwitch = 0;
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
            //Talk(SAY_AGGRO); //You have come far enough. Time to die.
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
            me->NearTeleportTo(2933.5f, 1055.94f, 512.463f, 4.79f); //HomePos
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
                //Talk(SAY_INTRO); //You ALL will regret trespassing in my realm.
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
                    break;
                default:
                    break;
            }
            
            if (healthSwitch < 2)
            {
                me->SetHealth(me->GetHealth() - me->CountPctFromMaxHealth(3));
                healthSwitch++;
            }
            else
            {
                me->SetHealth(me->GetHealth() - me->CountPctFromMaxHealth(4));
                healthSwitch = 0;
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
                //Talk(); //Do you think you have won? You have merely survived the storm... The seas are unstoppable.
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
                        events.ScheduleEvent(EVENT_SUBMERGED_END, 10000);
                        break;
                    case EVENT_SUBMERGED_END:
                        me->NearTeleportTo(2933.5f, 961.79f, 510.38f, 4.71f);
                        me->RemoveAllAuras();
                        me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_PREVENT_SELECT_NPC);
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

void AddSC_boss_helya()
{
    new boss_helya();
    new npc_helya_tentacle();
    new npc_helya_tentacle_veh();
}