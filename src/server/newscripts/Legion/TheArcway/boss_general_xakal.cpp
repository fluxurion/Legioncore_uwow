/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
    Encounter: General Xakal
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_arcway.h"

 enum Says
{
    SAY_AGGRO           = 1,
    SAY_FEL             = 2,
    SAY_SUM             = 3,
    SAY_WICKED          = 4,
    SAY_WICKED_TEXT     = 5,
    SAY_DEATH           = 6,
}; 

//TO-DO: Весь треш на 703+
enum Spells
{
    SPELL_FEL_FISSURE       = 197776,
    SPELL_FEL_FISSURE_SUM   = 197765,
    SPELL_FEL_FISSURE_AT    = 197573, //197579 dmg
    SPELL_SHADOW_SLASH      = 212028,
    SPELL_SHADOW_SLASH_AT   = 212030,
    SPELL_WICKED_SLAM       = 197810,

    //Felbat
    SPELL_BOMBARDMENT_AURA  = 197786,
    SPELL_BOMBARDMENT_DMG   = 197787,
};

enum eEvents
{
    EVENT_FEL_FISSURE       = 1,
    EVENT_SHADOW_SLASH      = 2,
    EVENT_WICKED_SLAM       = 3,
    EVENT_SUM_FELBAT        = 4,
};

class boss_general_xakal : public CreatureScript
{
public:
    boss_general_xakal() : CreatureScript("boss_general_xakal") {}

    struct boss_general_xakalAI : public BossAI
    {
        boss_general_xakalAI(Creature* creature) : BossAI(creature, DATA_XAKAL) {}

        void Reset()
        {
            _Reset();

            std::list<GameObject*> fissur;
            me->GetGameObjectListWithEntryInGrid(fissur, GO_XAKAL_FISSURE, 100);
            for (std::list<GameObject*>::iterator itr = fissur.begin(); itr != fissur.end(); ++itr)
                (*itr)->Delete();
        }

        void EnterCombat(Unit* /*who*/) //37:50
        {
            Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_FEL_FISSURE, 6000); //37:56
            events.ScheduleEvent(EVENT_SHADOW_SLASH, 13000); //38:03
            events.ScheduleEvent(EVENT_WICKED_SLAM, 40000); //38:30
            events.ScheduleEvent(EVENT_SUM_FELBAT, 11000); //38:01, 38:23
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_FEL_FISSURE:
                    DoCast(target, SPELL_FEL_FISSURE_SUM, true);
                    break;
                case SPELL_SHADOW_SLASH:
                    DoCast(target, SPELL_SHADOW_SLASH_AT);
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
                    case EVENT_FEL_FISSURE:
                        DoCast(SPELL_FEL_FISSURE);
                        Talk(SAY_FEL);
                        events.ScheduleEvent(EVENT_FEL_FISSURE, 14000);
                        break;
                    case EVENT_SHADOW_SLASH:
                        DoCast(SPELL_SHADOW_SLASH);
                        events.ScheduleEvent(EVENT_SHADOW_SLASH, 14000);
                        break;
                    case EVENT_WICKED_SLAM:
                        Talk(SAY_WICKED);
                        Talk(SAY_WICKED_TEXT);
                        DoCast(SPELL_WICKED_SLAM);
                        events.ScheduleEvent(EVENT_WICKED_SLAM, 40000);
                        break;
                    case EVENT_SUM_FELBAT:
                    {
                        Talk(SAY_SUM);
                        Position pos;
                        me->GetRandomNearPosition(pos, 30.0f);
                        me->SummonCreature(NPC_DREAD_FELBAT, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 20.0f);
                        events.ScheduleEvent(EVENT_SUM_FELBAT, 22000);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_general_xakalAI (creature);
    }
};

//100342
class npc_xakal_fissure : public CreatureScript
{
public:
    npc_xakal_fissure() : CreatureScript("npc_xakal_fissure") {}

    struct npc_xakal_fissureAI : public ScriptedAI
    {
        npc_xakal_fissureAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            SetCombatMovement(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        }

        EventMap events;
        bool active;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            active = false;
            events.ScheduleEvent(EVENT_1, 3000);
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (me->GetDistance(who) < 3.0f && !active)
            {
                active = true;
                DoCast(SPELL_FEL_FISSURE_AT);
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        active = true;
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_xakal_fissureAI(creature);
    }
};

//100393
class npc_xakal_dread_felbat : public CreatureScript
{
public:
    npc_xakal_dread_felbat() : CreatureScript("npc_xakal_dread_felbat") {}

    struct npc_xakal_dread_felbatAI : public ScriptedAI
    {
        npc_xakal_dread_felbatAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_BOMBARDMENT_AURA, true);
            events.ScheduleEvent(EVENT_1, 7000);
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
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoZoneInCombat(me, 100.0f);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_xakal_dread_felbatAI(creature);
    }
};

//197786
class spell_xakal_bombardment : public SpellScriptLoader
{
public:
    spell_xakal_bombardment() : SpellScriptLoader("spell_xakal_bombardment") { }

    class spell_xakal_bombardment_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_xakal_bombardment_AuraScript)

        void OnPeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->GetAnyOwner())
                if (Creature* summoner = caster->GetAnyOwner()->ToCreature())
                {
                    Position pos;
                    summoner->GetRandomNearPosition(pos, 30.0f);
                    caster->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_BOMBARDMENT_DMG, true);
                }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_xakal_bombardment_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_xakal_bombardment_AuraScript();
    }
};

void AddSC_boss_general_xakal()
{
    new boss_general_xakal();
    new npc_xakal_fissure();
    new npc_xakal_dread_felbat();
    new spell_xakal_bombardment();
}