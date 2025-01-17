/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
    Encounter: Ashgolm
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "vault_of_the_wardens.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_FROZEN                = 200354,
    SPELL_HEAT_UP               = 200981,
    SPELL_SMOLDERING            = 193209,
    SPELL_VOLCANO               = 192622,
    SPELL_LAVA_WREATH           = 192631,
    SPELL_LAVA_WREATH_VIS       = 193354,
    SPELL_LAVA_WREATH_TRIG      = 193353,
    SPELL_FISSURE               = 192522,
    SPELL_FISSURE_TRIG          = 193355,
    SPELL_SUM_EMBER             = 195170,

    //Ember
    SPELL_EMBER_SMOLDERING      = 195188,
    SPELL_SEAR                  = 195332,
    SPELL_FIRED_UP              = 202169,
    SPELL_DETONATE              = 195187,
    
    //System security
    SPELL_ACTIVATE_SYSTEM_VIS   = 195189,
    SPELL_ACTIVATE_SYSTEM       = 195213,
    SPELL_FREEZE_ROOM           = 195227,
};

enum eEvents
{
    EVENT_VOLCANO               = 1,
    EVENT_LAVA_WREATH           = 2,
    EVENT_FISSURE               = 3,
    EVENT_SUM_EMBER             = 4,
};

Position const centrPos = {4239.14f, -451.332f, 105.947f};

class boss_ashgolm : public CreatureScript
{
public:
    boss_ashgolm() : CreatureScript("boss_ashgolm") {}

    struct boss_ashgolmAI : public BossAI
    {
        boss_ashgolmAI(Creature* creature) : BossAI(creature, DATA_ASHGOLM) {}

        void Reset()
        {
            _Reset();
            me->GetMotionMaster()->MoveTargetedHome();

            if (instance->GetData(DATA_ASHGOLM_SYSTEM) != NOT_STARTED)
                instance->SetData(DATA_ASHGOLM_SYSTEM, NOT_STARTED);

            std::list<GameObject*> volcanoList;
            me->GetGameObjectListWithEntryInGrid(volcanoList, GO_ASHGOLM_VOLCANO, 80.0f);
            for (std::list<GameObject*>::iterator itr = volcanoList.begin(); itr != volcanoList.end(); ++itr)
            {
                (*itr)->Delete();
            }
        }

        void EnterCombat(Unit* /*who*/) //12:02
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(SPELL_HEAT_UP);

            events.ScheduleEvent(EVENT_VOLCANO, 10000);     //12:12, 12:34
            events.ScheduleEvent(EVENT_LAVA_WREATH, 25000); //12:27, 13:22
            events.ScheduleEvent(EVENT_FISSURE, 40000);     //12:42, 13:29
            events.ScheduleEvent(EVENT_SUM_EMBER, 15000);
        }

        void JustReachedHome()
        {
            DoCast(me, SPELL_FROZEN, true);
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_HEAT_UP)
                DoCast(me, SPELL_SMOLDERING, true);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_LAVA_WREATH)
            {
                DoCast(target, SPELL_LAVA_WREATH_VIS, true);
                Position pos;
                for (uint8 i = 0; i < 6; i++)
                {
                    target->GetNearPosition(pos, 5.0f, i * 1.2);
                    me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_LAVA_WREATH_TRIG, true);
                }
            }
            if (spell->Id == SPELL_FISSURE)
            {
                Position pos;
                float angle = centrPos.GetRelativeAngle(target);
                for (uint8 i = 0; i < 5; i++)
                {
                    centrPos.SimplePosXYRelocationByAngle(pos, i * 6, angle);
                    me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_FISSURE_TRIG, true);
                }
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
                    case EVENT_VOLCANO:
                        DoCast(SPELL_VOLCANO);
                        events.ScheduleEvent(EVENT_VOLCANO, 20000);
                        break;
                    case EVENT_LAVA_WREATH:
                        DoCast(SPELL_LAVA_WREATH);
                        events.ScheduleEvent(EVENT_LAVA_WREATH, 55000);
                        break;
                    case EVENT_FISSURE:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_FISSURE);
                        events.ScheduleEvent(EVENT_FISSURE, 46000);
                        break;
                    case EVENT_SUM_EMBER:
                        if (GameObject* volcano = me->FindNearestGameObject(GO_ASHGOLM_VOLCANO, 60.0f))
                            me->CastSpell(volcano->GetPositionX(), volcano->GetPositionY(), volcano->GetPositionZ(), SPELL_SUM_EMBER, true);
                        events.ScheduleEvent(EVENT_SUM_EMBER, 15000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ashgolmAI (creature);
    }
};

//99233
class npc_ashgolm_ember : public CreatureScript
{
public:
    npc_ashgolm_ember() : CreatureScript("npc_ashgolm_ember") {}

    struct npc_ashgolm_emberAI : public ScriptedAI
    {
        npc_ashgolm_emberAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset() {}

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_FIRED_UP)
                events.ScheduleEvent(EVENT_1, 500);
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_EMBER_SMOLDERING, true);
            events.ScheduleEvent(EVENT_2, 2000);
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
                        DoCast(SPELL_DETONATE);
                        break;
                    case EVENT_2:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                            DoCast(pTarget, SPELL_SEAR);
                        events.ScheduleEvent(EVENT_2, 4000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ashgolm_emberAI(creature);
    }
};

//99240
class npc_ashgolm_countermeasures : public CreatureScript
{
public:
    npc_ashgolm_countermeasures() : CreatureScript("npc_ashgolm_countermeasures") {}

    struct npc_ashgolm_countermeasuresAI : public ScriptedAI
    {
        npc_ashgolm_countermeasuresAI(Creature* creature) : ScriptedAI(creature) 
        {
            instance = me->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
        }

        InstanceScript* instance;
        bool click;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_ACTIVATE_SYSTEM_VIS, true);
        }

        void OnSpellClick(Unit* clicker)
        {
            if (instance->GetBossState(DATA_ASHGOLM) != IN_PROGRESS)
                return;

            if (instance->GetData(DATA_ASHGOLM_SYSTEM) != IN_PROGRESS)
                instance->SetData(DATA_ASHGOLM_SYSTEM, IN_PROGRESS);
        }

        void DoAction(int32 const action)
        {
            DoCast(me, SPELL_ACTIVATE_SYSTEM, true);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ACTIVATE_SYSTEM)
                target->CastSpell(target, SPELL_FREEZE_ROOM);
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ashgolm_countermeasuresAI(creature);
    }
};

//200354
class spell_ashgolm_frozen : public SpellScriptLoader
{
public:
    spell_ashgolm_frozen() : SpellScriptLoader("spell_ashgolm_frozen") { }

    class spell_ashgolm_frozen_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ashgolm_frozen_AuraScript);

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* caster = GetCaster()->ToCreature())
                caster->SetReactState(REACT_PASSIVE);
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* caster = GetCaster()->ToCreature())
                caster->SetReactState(REACT_AGGRESSIVE);
        }
        
        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_ashgolm_frozen_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_ashgolm_frozen_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ashgolm_frozen_AuraScript();
    }
};

void AddSC_boss_ashgolm()
{
    new boss_ashgolm();
    new npc_ashgolm_ember();
    new npc_ashgolm_countermeasures();
    new spell_ashgolm_frozen();
}