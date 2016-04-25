/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
    Encounter: Serpentrix
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "eye_of_azshara.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_TOXIC_WOUND           = 191855,
    SPELL_POISON_SPIT           = 192050,
    SPELL_POISON_SPIT_TARG      = 191839,
    SPELL_POISON_SPIT_S1        = 191841, //Speed: 2.00
    SPELL_POISON_SPIT_S2        = 191843, //Speed: 3.00
    SPELL_POISON_SPIT_S3        = 191845, //Speed: 0.75
    SPELL_SUBMERGE              = 191873,
    SPELL_SUM_HYDRA_SPAWN       = 192010,
    SPELL_RAMPAGE               = 191848,
    SPELL_RAMPAGE_TARG          = 191850,
    SPELL_HYDRA_HEAD            = 202680,

    SPELL_BLAZING_NOVA          = 192003,
};

enum eEvents
{
    EVENT_TOXIC_WOUND           = 1,
    EVENT_POISON_SPIT           = 2,
    EVENT_SUBMERGE              = 3,
    EVENT_SUM_HYDRA             = 4,
    EVENT_RAMPAGE               = 5,
};

uint32 poisonSpells[3] =
{
    SPELL_POISON_SPIT_S1,
    SPELL_POISON_SPIT_S2,
    SPELL_POISON_SPIT_S3
};

Position const tpPos[6] =
{
    {-3256.36f, 4370.39f, 0.37f},
    {-3294.20f, 4460.52f, -0.6f},
    {-3304.17f, 4405.53f, 0.08f},
    {-3193.61f, 4435.89f, -0.7f},
    {-3199.40f, 4384.95f, 0.16f},
    {-3246.71f, 4479.65f, 0.26f}
};

class boss_serpentrix : public CreatureScript
{
public:
    boss_serpentrix() : CreatureScript("boss_serpentrix") {}

    struct boss_serpentrixAI : public BossAI
    {
        boss_serpentrixAI(Creature* creature) : BossAI(creature, DATA_SERPENTRIX) 
        {
            SetCombatMovement(false);
        }

        int8 healthPct;
        uint8 sumGidrCount;
        uint16 checkVictimTimer;

        void Reset()
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(191855);
            _Reset();
            DespawnAllAT();
            healthPct = 67;
            checkVictimTimer = 2000;
        }

        void EnterCombat(Unit* /*who*/) //41:42
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_TOXIC_WOUND, 6000);  //41:48, 42:13
            events.ScheduleEvent(EVENT_POISON_SPIT, 11000); //41:53, 42:02
        }

        void EnterEvadeMode()
        {
            me->NearTeleportTo(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY(), 
                               me->GetHomePosition().GetPositionZ(), me->GetHomePosition().GetOrientation());

            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_POISON_SPIT_TARG:
                case SPELL_RAMPAGE_TARG:
                    DoCast(target, poisonSpells[urand(0, 2)], true); //Нужно переделать каст спелла по ветру, а не рандому.
                    break;
                case SPELL_SUBMERGE:
                    events.ScheduleEvent(EVENT_SUM_HYDRA, 500);
                    break;
                case SPELL_SUM_HYDRA_SPAWN:
                    me->SummonCreature(NPC_BLAZING_HYDRA_SPAWN, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                    break;
                default:
                    break;
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(healthPct))
            {
                healthPct -= 34;
                events.ScheduleEvent(EVENT_SUBMERGE, 0);
            }
        }

        void DoAction(int32 const action)
        {
            if (sumGidrCount < 2)
            {
                sumGidrCount++;
            }
            else
            {
                sumGidrCount = 0;
                DoCast(me, SPELL_HYDRA_HEAD, true);
            }
        }

        void DespawnAllAT()
        {
            std::list<AreaTrigger*> atlist;
            me->GetAreaTriggersWithEntryInRange(atlist, 4856, me->GetGUID(), 100.0f);
            if (!atlist.empty())
                for (std::list<AreaTrigger*>::const_iterator itr = atlist.begin(); itr != atlist.end(); itr++)
                    (*itr)->RemoveFromWorld();
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (checkVictimTimer <= diff)
            {
                if (me->getVictim())
                {
                    if (!me->IsWithinMeleeRange(me->getVictim()))
                    {
                        if (!me->HasAura(SPELL_RAMPAGE))
                            events.ScheduleEvent(EVENT_RAMPAGE, 500);
                    }
                    else
                        me->InterruptNonMeleeSpells(false, SPELL_RAMPAGE);
                }
                checkVictimTimer = 2000;
            }
            else checkVictimTimer -= diff;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TOXIC_WOUND:
                        DoCast(SPELL_TOXIC_WOUND);
                        events.ScheduleEvent(EVENT_TOXIC_WOUND, 25000);
                        break;
                    case EVENT_POISON_SPIT:
                        DoCast(SPELL_POISON_SPIT);
                        events.ScheduleEvent(EVENT_POISON_SPIT, 9000);
                        break;
                    case EVENT_SUBMERGE:
                        DoCast(SPELL_SUBMERGE);
                        break;
                    case EVENT_SUM_HYDRA:
                        DoCast(SPELL_SUM_HYDRA_SPAWN);
                        break;
                    case EVENT_RAMPAGE:
                        DoCast(SPELL_RAMPAGE);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_serpentrixAI (creature);
    }
};

//97259
class npc_serpentrix_blazing_hydra : public CreatureScript
{
public:
    npc_serpentrix_blazing_hydra() : CreatureScript("npc_serpentrix_blazing_hydra") {}

    struct npc_serpentrix_blazing_hydraAI : public ScriptedAI
    {
        npc_serpentrix_blazing_hydraAI(Creature* creature) : ScriptedAI(creature) 
        {
            SetCombatMovement(false);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            if (summoner->isInCombat())
                summoner->ToCreature()->AI()->DoAction(true);
            else
                me->DespawnOrUnsummon();

            DoZoneInCombat(me, 100.0f);
            events.ScheduleEvent(EVENT_1, 3000);
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
                    case EVENT_1:
                        DoCast(SPELL_HYDRA_HEAD);
                        events.ScheduleEvent(EVENT_1, 2000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_serpentrix_blazing_hydraAI(creature);
    }
};

//191873
class spell_serpentrix_submerge_teleport : public SpellScriptLoader
{
public:
    spell_serpentrix_submerge_teleport() : SpellScriptLoader("spell_serpentrix_submerge_teleport") { }

    class spell_serpentrix_submerge_teleport_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_serpentrix_submerge_teleport_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Unit* caster = GetCaster();
            if (!caster)
                return;

            uint8 rand = urand(0, 5);
            if (caster->GetDistance(tpPos[rand]) < 10.0f)
                rand -= rand;

            caster->NearTeleportTo(tpPos[rand].GetPositionX(), tpPos[rand].GetPositionY(), tpPos[rand].GetPositionZ(), tpPos[rand].GetOrientation());
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_serpentrix_submerge_teleport_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_TELEPORT_L);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_serpentrix_submerge_teleport_SpellScript();
    }
};

void AddSC_boss_serpentrix()
{
    new boss_serpentrix();
    new npc_serpentrix_blazing_hydra();
    new spell_serpentrix_submerge_teleport();
}