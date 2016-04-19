/*
    http://uwow.biz
    Dungeon : Neltharions Lair 100-110
    Encounter: Ularogg Cragshaper
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "neltharions_lair.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_STANCE_EMOTE    = 1,
    SAY_DEATH           = 2,
};

enum Spells
{
    SPELL_SUNDER                    = 198496,
    SPELL_SUNDER_CALL               = 198823, //Conversation
    SPELL_STRIKE_MOUNTAIN           = 198428,
    SPELL_MOUNTAIN_CALL             = 198825, //Conversation
    SPELL_BELLOW_DEEPS_1            = 193375,
    SPELL_BELLOW_DEEPS_2            = 193376,
    SPELL_BELLOW_DEEPS_CALL         = 198824,
    //SPELL_CALL                    = 198826, //Conversation
    SPELL_STANCE_MOUNTAIN_JUMP      = 198509,
    SPELL_STANCE_MOUNTAIN_SUM       = 198565,
    SPELL_STANCE_MOUNTAIN_MORPH     = 198510,
    SPELL_STANCE_MOUNTAIN_TICK      = 198617,
    SPELL_FALLING_DEBRIS_ULAROGG    = 198719, //boss
    SPELL_STANCE_MOUNTAIN_END       = 198631,

    SPELL_FALLING_DEBRIS            = 193267, //npc 98081
    SPELL_FALLING_DEBRIS_2          = 198717, //npc 100818
    SPELL_STANCE_MOUNTAIN_MOVE      = 198616,
};

enum eEvents
{
    EVENT_SUNDER                = 1,
    EVENT_STRIKE_MOUNTAIN       = 2,
    EVENT_BELLOW_DEEPS_1        = 3,
    EVENT_BELLOW_DEEPS_2        = 4,
    EVENT_STANCE_MOUNTAIN_1     = 5,
    EVENT_STANCE_MOUNTAIN_2     = 6,
    EVENT_STANCE_MOUNTAIN_3     = 7,
    EVENT_STANCE_MOUNTAIN_4     = 8,
};

class boss_ularogg_cragshaper : public CreatureScript
{
public:
    boss_ularogg_cragshaper() : CreatureScript("boss_ularogg_cragshaper") { }

    struct boss_ularogg_cragshaperAI : public BossAI
    {
        boss_ularogg_cragshaperAI(Creature* creature) : BossAI(creature, DATA_ULAROGG) {}

        uint8 healthPct;
        ObjectGuid stanceGUID;
        bool lastStance;
        bool phaseMountain;

        void Reset()
        {
            _Reset();
            healthPct = 60;
            phaseMountain = false;
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveAllAuras();
        }

        void EnterCombat(Unit* /*who*/) //05:40
        {
            Talk(SAY_AGGRO); //Pay attention, Navarogg. I want you to see your heroes die.
            _EnterCombat();
            DefaultEvent();
        }

        void DefaultEvent()
        {
            events.ScheduleEvent(EVENT_SUNDER, 8000);           //05:48
            events.ScheduleEvent(EVENT_STRIKE_MOUNTAIN, 16000); //05:56
            events.ScheduleEvent(EVENT_BELLOW_DEEPS_1, 20000);  //06:01
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(healthPct))
            {
                healthPct -= 30;
                events.ScheduleEvent(EVENT_STANCE_MOUNTAIN_1, 500);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (id == SPELL_STANCE_MOUNTAIN_JUMP)
                    events.ScheduleEvent(EVENT_STANCE_MOUNTAIN_2, 2000);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == 198619)
            {
                Position pos;
                me->GetRandomNearPosition(pos, 30.0f);
                target->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_STANCE_MOUNTAIN_MOVE, true);
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetGUID() == stanceGUID)
            {
                me->RemoveAllAuras();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
                me->SetReactState(REACT_AGGRESSIVE);
                DoCast(me, SPELL_STANCE_MOUNTAIN_END, true);
                DefaultEvent();
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_BELLOWING_IDOL_2 && lastStance)
            {
                lastStance = false;
                stanceGUID.Clear();
                stanceGUID = summon->GetGUID();
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
                    case EVENT_SUNDER:
                        if (me->getVictim())
                        {
                            DoCast(me, SPELL_SUNDER_CALL, true);
                            DoCast(me->getVictim(), SPELL_SUNDER);
                        }
                        events.ScheduleEvent(EVENT_SUNDER, 10000);
                        break;
                    case EVENT_STRIKE_MOUNTAIN:
                        DoCast(SPELL_STRIKE_MOUNTAIN);        
                        events.ScheduleEvent(EVENT_STRIKE_MOUNTAIN, 16000);
                        break;
                    case EVENT_BELLOW_DEEPS_1:
                        DoCast(SPELL_BELLOW_DEEPS_1);
                        events.ScheduleEvent(EVENT_BELLOW_DEEPS_1, 32000);
                        events.ScheduleEvent(EVENT_BELLOW_DEEPS_2, 3000);
                        break;
                    case EVENT_BELLOW_DEEPS_2:
                        DoCast(me, SPELL_BELLOW_DEEPS_CALL, true);
                        DoCast(SPELL_BELLOW_DEEPS_2);
                        break;
                    case EVENT_STANCE_MOUNTAIN_1:
                        events.Reset();
                        DoStopAttack();
                        DoCast(SPELL_STANCE_MOUNTAIN_JUMP);
                        Talk(SAY_STANCE_EMOTE);
                        break;
                    case EVENT_STANCE_MOUNTAIN_2:
                        DoCast(SPELL_STANCE_MOUNTAIN_SUM);
                        events.ScheduleEvent(EVENT_STANCE_MOUNTAIN_3, 3000);
                        break;
                    case EVENT_STANCE_MOUNTAIN_3:
                        me->RemoveAllAuras();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
                        DoCast(SPELL_STANCE_MOUNTAIN_MORPH);
                        lastStance = true;
                        me->SummonCreature(NPC_BELLOWING_IDOL_2, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->AddAura(SPELL_STANCE_MOUNTAIN_TICK, me);
                        events.ScheduleEvent(EVENT_STANCE_MOUNTAIN_4, 10000);
                        break;
                    case EVENT_STANCE_MOUNTAIN_4:
                    {
                        EntryCheckPredicate pred(NPC_BELLOWING_IDOL_2);
                        summons.DoAction(1, pred);
                        me->AddAura(SPELL_FALLING_DEBRIS_ULAROGG, me);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ularogg_cragshaperAI (creature);
    }
};

//98081, 100818
class npc_ularogg_bellowing_idols : public CreatureScript
{
public:
    npc_ularogg_bellowing_idols() : CreatureScript("npc_ularogg_bellowing_idols") {}

    struct npc_ularogg_bellowing_idolsAI : public ScriptedAI
    {
        npc_ularogg_bellowing_idolsAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            if (me->GetEntry() == NPC_BELLOWING_IDOL)
                DoCast(me, SPELL_FALLING_DEBRIS, true);
            else
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
                DoCast(me, 198569, true); //Visual Spawn
            }
        }

        void DoAction(int32 const action)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
            DoCast(me, SPELL_FALLING_DEBRIS_2, true);
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ularogg_bellowing_idolsAI(creature);
    }
};

//183213
class spell_barrel_ride_plr_move : public SpellScriptLoader
{
    public:
        spell_barrel_ride_plr_move() : SpellScriptLoader("spell_barrel_ride_plr_move") { }

        class spell_barrel_ride_plr_move_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_barrel_ride_plr_move_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget())
                    return;

                if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                {
                    GetTarget()->GetMotionMaster()->MovePath(9100400, false);
                    GetTarget()->ToPlayer()->SetClientControl(GetTarget(), 0);
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget())
                    return;

                if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                    GetTarget()->ToPlayer()->SetClientControl(GetTarget(), 1);
            }
            
            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_barrel_ride_plr_move_AuraScript::OnApply, EFFECT_2, SPELL_AURA_321, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_barrel_ride_plr_move_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_321, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_barrel_ride_plr_move_AuraScript();
        }
};


//trash 102228
class npc_stonedark_slave : public CreatureScript
{
public:
    npc_stonedark_slave() : CreatureScript("npc_stonedark_slave") {}

    struct npc_stonedark_slaveAI : public ScriptedAI
    {
        npc_stonedark_slaveAI(Creature* creature) : ScriptedAI(creature) 
        {
           _endBarrel = false;
           _intro = false;
        }
 
        bool _endBarrel;
        bool _intro;
        
        void MoveInLineOfSight(Unit* who)
        {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!_endBarrel && me->IsWithinDistInMap(who, 90.0f))
             {
                // who->CastSpell(who, 209531, true);  пока офф до 7.0.3, крашит
                _endBarrel = true;
             }            
             if (!_intro && me->IsWithinDistInMap(who, 40.0f))
             {
                who->CastSpell(who, 209536, true);
                _intro = true;
             }
        }
 
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stonedark_slaveAI(creature);
    }
};

// trash 183088
class spell_avalanche : public SpellScriptLoader
{
    public:
        spell_avalanche() : SpellScriptLoader("spell_avalanche") { }

        class spell_avalanche_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_avalanche_SpellScript);
            
            void HandleScript(SpellEffIndex effIndex)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;
                std::list<Player*> targets;
                GetPlayerListInGrid(targets, caster, 40);
                Trinity::Containers::RandomResizeList(targets, 2);
                for (std::list<Creature*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                  if (!targets.empty())
                    caster->CastSpell((*itr), 183095, true);                 
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_avalanche_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_avalanche_SpellScript();
        }
};

void AddSC_boss_ularogg_cragshaper()
{
    new boss_ularogg_cragshaper();
    new npc_ularogg_bellowing_idols();
    new spell_barrel_ride_plr_move();
    new npc_stonedark_slave();
    new spell_avalanche();
}