/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
    Encounter: King Deepbeard
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
    SPELL_GROUND_SLAM               = 193093,
    SPELL_GASEOUS_BUBBLES           = 193018,
    SPELL_GASEOUS_BUBBLES_EXPLOSE   = 193047,
    SPELL_QUAKE                     = 193152,
    SPELL_CALL_THE_SEAS             = 193051,
    SPELL_FRENZY                    = 197550,

    SPELL_QUAKE_VISAL               = 193175,
    SPELL_QUAKE_DMG                 = 193159,
};

enum eEvents
{
    EVENT_GROUND_SLAM           = 1,
    EVENT_GASEOUS_BUBBLES       = 2,
    EVENT_QUAKE                 = 3,
    EVENT_CALL_THE_SEAS         = 4,
};

class boss_king_deepbeard : public CreatureScript
{
public:
    boss_king_deepbeard() : CreatureScript("boss_king_deepbeard") {}

    struct boss_king_deepbeardAI : public BossAI
    {
        boss_king_deepbeardAI(Creature* creature) : BossAI(creature, DATA_DEEPBEARD) {}

        bool lowHp;
        bool activeEnergy;
        uint16 changeEnergyTimer;

        void Reset()
        {
            _Reset();
            me->SetPower(POWER_ENERGY, 30);
            changeEnergyTimer = 1000;
            activeEnergy = true;
            lowHp = false;
        }

        void EnterCombat(Unit* /*who*/) //33:25
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_GROUND_SLAM, 7000); //33:32
            events.ScheduleEvent(EVENT_GASEOUS_BUBBLES, 11000); //33:36, 34:09
            events.ScheduleEvent(EVENT_CALL_THE_SEAS, 20000); //33:45
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->HealthBelowPct(31) && !lowHp)
            {
                lowHp = true;
                DoCast(me, SPELL_FRENZY, true);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (activeEnergy)
                if (changeEnergyTimer <= diff)
                {
                    if (me->GetPower(POWER_ENERGY) >= 100)
                    {
                        events.ScheduleEvent(EVENT_QUAKE, 0);
                        activeEnergy = false;
                    }
                    else
                        me->SetPower(POWER_ENERGY, me->GetPower(POWER_ENERGY) + 5);

                    changeEnergyTimer = 1000;
                }
                else changeEnergyTimer -= diff;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GROUND_SLAM:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_GROUND_SLAM);
                        events.ScheduleEvent(EVENT_GROUND_SLAM, 18000);
                        break;
                    case EVENT_GASEOUS_BUBBLES:
                        DoCast(SPELL_GASEOUS_BUBBLES);
                        events.ScheduleEvent(EVENT_GASEOUS_BUBBLES, 33000);
                        break;
                    case EVENT_QUAKE:
                        DoCast(SPELL_QUAKE);
                        activeEnergy = true;
                        break;
                    case EVENT_CALL_THE_SEAS:
                        DoCast(SPELL_CALL_THE_SEAS);
                        events.ScheduleEvent(EVENT_CALL_THE_SEAS, 30000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_king_deepbeardAI (creature);
    }
};

//97916
class npc_deepbeard_quake : public CreatureScript
{
public:
    npc_deepbeard_quake() : CreatureScript("npc_deepbeard_quake") {}

    struct npc_deepbeard_quakeAI : public ScriptedAI
    {
        npc_deepbeard_quakeAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_QUAKE_VISAL, true);
            DoCast(SPELL_QUAKE_DMG);
            events.ScheduleEvent(EVENT_1, 4000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(SPELL_QUAKE_DMG);
                        me->DespawnOrUnsummon(1000);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_deepbeard_quakeAI(creature);
    }
};

//97844
class npc_deepbeard_call_the_seas : public CreatureScript
{
public:
    npc_deepbeard_call_the_seas() : CreatureScript("npc_deepbeard_call_the_seas") {}

    struct npc_deepbeard_call_the_seasAI : public ScriptedAI
    {
        npc_deepbeard_call_the_seasAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        bool moveActive;
        uint16 randomMoveTimer;

        void Reset() 
        {
            moveActive = true;
            randomMoveTimer = 1000;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            moveActive = true;
        }

        void UpdateAI(uint32 diff)
        {
            if (moveActive)
                if (randomMoveTimer <= diff)
                {
                    moveActive = false;
                    randomMoveTimer = 500;
                    float angle = (float)rand_norm() * static_cast<float>(2 * M_PI);
                    float distance = 20 * (float)rand_norm();
                    float x = me->GetHomePosition().GetPositionX() + distance * std::cos(angle);
                    float y = me->GetHomePosition().GetPositionY() + distance * std::sin(angle);
                    float z = me->GetHomePosition().GetPositionZ();
                    Trinity::NormalizeMapCoord(x);
                    Trinity::NormalizeMapCoord(y);
    
                    me->GetMotionMaster()->MovePoint(1, x, y, z);
                }
                else randomMoveTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_deepbeard_call_the_seasAI(creature);
    }
};

//193018
class spell_deepbeard_gaseous_explosion : public SpellScriptLoader
{
    public:
        spell_deepbeard_gaseous_explosion() : SpellScriptLoader("spell_deepbeard_gaseous_explosion") { }

        class spell_deepbeard_gaseous_explosion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_deepbeard_gaseous_explosion_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget())
                    return;

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                    GetTarget()->CastSpell(GetTarget(), SPELL_GASEOUS_BUBBLES_EXPLOSE, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_deepbeard_gaseous_explosion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_deepbeard_gaseous_explosion_AuraScript();
        }
};

void AddSC_boss_king_deepbeard()
{
    new boss_king_deepbeard();
    new npc_deepbeard_quake();
    new npc_deepbeard_call_the_seas();
    new spell_deepbeard_gaseous_explosion();
}