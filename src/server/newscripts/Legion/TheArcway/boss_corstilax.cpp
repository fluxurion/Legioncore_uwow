/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
    Encounter: Corstilax
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_arcway.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_SUPPRESSION_PROTOCOL  = 196047,
    SPELL_S_PROTOCOL_FIXATE     = 196068,
    SPELL_S_PROTOCOL_DMG        = 196070,
    SPELL_QUARANTINE            = 195791,
    SPELL_QUARANTINE_STUN       = 195804,
    SPELL_CLEANSING_FORCE       = 196115,
    SPELL_CLEANSING_FORCE_AT    = 196088,

    SPELL_ENERGY_BURST_VISUAL   = 195516,
    SPELL_ENERGY_BURST_AT       = 195523,
    SPELL_NIGHTWELL_ENERGY      = 195362,
};

enum eEvents
{
    EVENT_PROTOCOL              = 1,
    EVENT_QUARANTINE            = 2,
    EVENT_CLEANSING_FORCE       = 3,
    EVENT_ENERGY_BURST          = 4,
};

Position const centrPos = {3147.14f, 4887.08f, 617.62f};

class boss_corstilax : public CreatureScript
{
public:
    boss_corstilax() : CreatureScript("boss_corstilax") {}

    struct boss_corstilaxAI : public BossAI
    {
        boss_corstilaxAI(Creature* creature) : BossAI(creature, DATA_CORSTILAX) {}

        void Reset()
        {
            _Reset();
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NIGHTWELL_ENERGY);
        }

        void EnterCombat(Unit* /*who*/) //19:38
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_PROTOCOL, 5000);     //19:43
            events.ScheduleEvent(EVENT_QUARANTINE, 20000);  //19:59
            //events.ScheduleEvent(EVENT_CLEANSING_FORCE, 28000); //20:09 - Нет инфы по АТ
            events.ScheduleEvent(EVENT_ENERGY_BURST, 18000); //19:56
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NIGHTWELL_ENERGY);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_SUPPRESSION_PROTOCOL:
                {
                    float x = target->GetPositionX();
                    float y = target->GetPositionY();
                    float z = target->GetPositionZ();
                    if (Creature* protocol = me->SummonCreature(NPC_SUPPRESSION_PROTOCOL, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 12000))
                    {
                        protocol->CastSpell(target, SPELL_S_PROTOCOL_FIXATE, true);
                        if (target->GetTypeId() != TYPEID_PLAYER)
                        protocol->AI()->Talk(0, target->ToPlayer()->GetGUID());
                    }
                    DoCast(SPELL_S_PROTOCOL_DMG);
                    break;
                }
                case SPELL_S_PROTOCOL_DMG:
                    me->AddUnitState(UNIT_STATE_CASTING);
                    break;
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
                    case EVENT_PROTOCOL:
                        DoCast(SPELL_SUPPRESSION_PROTOCOL);
                        events.ScheduleEvent(EVENT_PROTOCOL, 36000);
                        break;
                    case EVENT_QUARANTINE:
                        DoCast(SPELL_QUARANTINE);
                        events.ScheduleEvent(EVENT_QUARANTINE, 32000);
                        break;
                    case EVENT_CLEANSING_FORCE:
                        DoCast(me, SPELL_CLEANSING_FORCE_AT, true);
                        DoCast(SPELL_CLEANSING_FORCE);
                        events.ScheduleEvent(EVENT_CLEANSING_FORCE, 32000);
                        break;
                    case EVENT_ENERGY_BURST:
                        if (Creature* pipe = me->SummonCreature(NPC_PIPE_STALKER, 3101.87f, 4908.10f, 622.39f, 5.86f, TEMPSUMMON_TIMED_DESPAWN, 10000))
                        {
                            pipe->CastSpell(pipe, SPELL_ENERGY_BURST_VISUAL, true);
                            for(uint8 i = 0; i < 10; i++)
                            {
                                float angle = (float)rand_norm() * static_cast<float>(2 * M_PI);
                                float dist = 35.0f * (float)rand_norm();
                                float x = centrPos.GetPositionX() + dist * std::cos(angle);
                                float y = centrPos.GetPositionY() + dist * std::sin(angle);
                                Trinity::NormalizeMapCoord(x);
                                Trinity::NormalizeMapCoord(y);
                                pipe->CastSpell(x, y, 617.63f, SPELL_ENERGY_BURST_AT, true);
                            }
                        }
                        events.ScheduleEvent(EVENT_ENERGY_BURST, 32000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_corstilaxAI (creature);
    }
};

//99628
class npc_corstilax_suppression_protocol : public CreatureScript
{
public:
    npc_corstilax_suppression_protocol() : CreatureScript("npc_corstilax_suppression_protocol") {}

    struct npc_corstilax_suppression_protocolAI : public ScriptedAI
    {
        npc_corstilax_suppression_protocolAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        Unit* plrTarget;

        void Reset() {}

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_S_PROTOCOL_FIXATE)
            {
                plrTarget = target;
                events.ScheduleEvent(EVENT_1, 500);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            damage = 0;
        }

        void UpdateAI(uint32 diff) 
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_1)
                {
                    me->GetMotionMaster()->MovePoint(1, plrTarget->GetPositionX(), plrTarget->GetPositionY(), plrTarget->GetPositionZ());
                    events.ScheduleEvent(EVENT_1, 1000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_corstilax_suppression_protocolAI(creature);
    }
};

//99560
class npc_corstilax_quarantine : public CreatureScript
{
public:
    npc_corstilax_quarantine() : CreatureScript("npc_corstilax_quarantine") {}

    struct npc_corstilax_quarantineAI : public ScriptedAI
    {
        npc_corstilax_quarantineAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            me->setFaction(35);
        }

        Unit* plrTarget;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            if (summoner->GetTypeId() != TYPEID_PLAYER)
                return;

            plrTarget = summoner;
            DoCast(summoner, 195820, true); //Ride Veh
            Talk(0, summoner->ToPlayer()->GetGUID());
        }

        void OnSpellClick(Unit* clicker)
        {
            clicker->CastSpell(me, 195826);
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == 195846)
            {
                plrTarget->RemoveAurasDueToSpell(SPELL_QUARANTINE_STUN);
                me->DespawnOrUnsummon();
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_corstilax_quarantineAI(creature);
    }
};

void AddSC_boss_corstilax()
{
    new boss_corstilax();
    new npc_corstilax_suppression_protocol();
    new npc_corstilax_quarantine();
}