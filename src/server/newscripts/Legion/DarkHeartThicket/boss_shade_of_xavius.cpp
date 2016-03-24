/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
    Encounter: Shade of Xavius
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "darkheart_thicket.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_DEATH           = 1,
    SAY_END             = 0,
};

enum Spells
{
    SPELL_SHADE_VISUAL              = 195106,
    SPELL_FESTERING_RIP             = 200182,
    SPELL_NIGHTMARE_BOLT            = 204808,
    SPELL_FEED_ON_THE_WEAK          = 200238,
    SPELL_INDUCED_PARANOIA          = 200359,
    //Final Phase 50%HP
    SPELL_PHASE_CHANGE_CONVERSATION = 199857,
    SPELL_APOCOLYPTIC_NIGHTMARE     = 200050,
    SPELL_DRAIN_ESSENCE             = 199837,
    //Cage
    SPELL_NIGHTMARE_SHIELD          = 204511,
    SPELL_NIGHTMARE_BINDINGS        = 199752,
    //Malfurion
    SPELL_NATURE_RECOVERY           = 204680,
    SPELL_ESCAPES_CONVERSATION      = 199912,
    SPELL_NATURE_DOMINANCE          = 199922,
};

enum eEvents
{
    EVENT_FESTERING_RIP         = 1,
    EVENT_NIGHTMARE_BOLT        = 2,
    EVENT_FEED_ON_THE_WEAK      = 3,
    EVENT_INDUCED_PARANOIA      = 4,
    EVENT_FINAL_PHASE           = 5,
};

class boss_shade_of_xavius : public CreatureScript
{
public:
    boss_shade_of_xavius() : CreatureScript("boss_shade_of_xavius") { }

    struct boss_shade_of_xaviusAI : public BossAI
    {
        boss_shade_of_xaviusAI(Creature* creature) : BossAI(creature, DATA_XAVIUS), summons(me) {}

        SummonList summons;
        bool finalPhase;

        void Reset()
        {
            events.Reset();
            _Reset();
            summons.DespawnAll();
            if (Creature* malfurion = instance->instance->GetCreature(instance->GetGuidData(NPC_MALFURION_STORMRAGE)))
                malfurion->DespawnOrUnsummon();
            finalPhase = false;
            DoCast(me, SPELL_SHADE_VISUAL, true);
            me->SummonCreature(NPC_NIGHTMARE_BINDINGS, 2705.76f, 1324.61f, 128.42f);
        }

        void EnterCombat(Unit* /*who*/) //49:53
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
            events.ScheduleEvent(EVENT_FESTERING_RIP,    3000);  //49:56, 50:13
            events.ScheduleEvent(EVENT_NIGHTMARE_BOLT,   9000);  //50:02, 50:19
            events.ScheduleEvent(EVENT_FEED_ON_THE_WEAK, 14000); //50:07, 51:01, 51:30, 52:00
            events.ScheduleEvent(EVENT_INDUCED_PARANOIA, 21000); //50:14, 51:00, 51:21, 51:42
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
            summons.DespawnAll();
            if (Creature* malfurion = instance->instance->GetCreature(instance->GetGuidData(NPC_MALFURION_STORMRAGE)))
            {
                malfurion->AI()->Talk(SAY_END);
                malfurion->InterruptNonMeleeSpells(false, SPELL_NATURE_DOMINANCE);
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            
            if (summon->GetEntry() == NPC_NIGHTMARE_BINDINGS)
            {
                summon->SetReactState(REACT_PASSIVE);
                summon->CastSpell(summon, SPELL_NIGHTMARE_SHIELD, true);
                summon->CastSpell(summon, SPELL_NIGHTMARE_BINDINGS, true);
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetEntry() == NPC_NIGHTMARE_BINDINGS)
            {
                if (Creature* malfurion = instance->instance->GetCreature(instance->GetGuidData(NPC_MALFURION_STORMRAGE)))
                    malfurion->AI()->DoAction(ACTION_1);

                me->InterruptNonMeleeSpells(false, SPELL_DRAIN_ESSENCE);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (HealthBelowPct(51) && !finalPhase)
            {
                finalPhase = true;
                events.ScheduleEvent(EVENT_FINAL_PHASE, 1000);
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
                    case EVENT_FESTERING_RIP:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_FESTERING_RIP);
                        events.ScheduleEvent(EVENT_FESTERING_RIP, 17000);
                        break;
                    case EVENT_NIGHTMARE_BOLT:
                        DoCast(SPELL_NIGHTMARE_BOLT);
                        events.ScheduleEvent(EVENT_NIGHTMARE_BOLT, 17000);
                        break;
                    case EVENT_FEED_ON_THE_WEAK:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            DoCast(pTarget, SPELL_FEED_ON_THE_WEAK);
                        events.ScheduleEvent(EVENT_FEED_ON_THE_WEAK, 30000);
                        break;
                    case EVENT_INDUCED_PARANOIA:
                        DoCast(SPELL_INDUCED_PARANOIA);
                        events.ScheduleEvent(EVENT_INDUCED_PARANOIA, 21000);
                        break;
                    case EVENT_FINAL_PHASE:
                        DoCast(me, SPELL_PHASE_CHANGE_CONVERSATION, true);
                        DoCast(me, SPELL_APOCOLYPTIC_NIGHTMARE, true);
                        DoCast(SPELL_DRAIN_ESSENCE);
                        if (Creature* cage = instance->instance->GetCreature(instance->GetGuidData(NPC_NIGHTMARE_BINDINGS)))
                        {
                            cage->RemoveAurasDueToSpell(SPELL_NIGHTMARE_SHIELD);
                            cage->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                        }
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_shade_of_xaviusAI (creature);
    }
};

//100652
class npc_xavius_malfurion_stormrage : public CreatureScript
{
public:
    npc_xavius_malfurion_stormrage() : CreatureScript("npc_xavius_malfurion_stormrage") { }

    struct npc_xavius_malfurion_stormrageAI : public ScriptedAI
    {
        npc_xavius_malfurion_stormrageAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = me->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {}

        void DoAction(int32 const action)
        {
            if (action == ACTION_1)
                if (instance->GetBossState(DATA_XAVIUS) == IN_PROGRESS)
                    me->GetMotionMaster()->MoveJump(2713.19f, 1329.85f, 128.36f, 10.0f, 10.0f);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != EFFECT_MOTION_TYPE)
                return;

            DoCast(me, SPELL_NATURE_RECOVERY, true);
            events.ScheduleEvent(EVENT_1, 20000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(me, SPELL_ESCAPES_CONVERSATION, true);
                        DoCast(SPELL_NATURE_DOMINANCE);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_xavius_malfurion_stormrageAI(creature);
    }
};

void AddSC_boss_shade_of_xavius()
{
    new boss_shade_of_xavius();
    new npc_xavius_malfurion_stormrage();
}