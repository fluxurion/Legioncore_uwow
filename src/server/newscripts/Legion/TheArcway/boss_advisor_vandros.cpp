/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
    Encounter: Advisor Vandros
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
    SPELL_ACCELERATING_BLAST    = 203176,
    SPELL_CHRONO_SHARDS         = 203254,
    SPELL_FORCE_BOMB            = 202974,
    SPELL_FORCE_BOMB_AT         = 202975,
    SPELL_FORCE_BOMB_AT_2       = 203090, //Разрастающийся взрыв
    //Health - 50% 
    SPELL_BANISH_IN_TIME_STUN   = 203922,
    SPELL_BANISH_IN_TIME_AT     = 203882,
    SPELL_BANISH_IN_TIME_TP     = 203883,
    SPELL_BANISH_IN_TIME_TIMER  = 203914,
    SPELL_BANISH_IN_TIME_REMOV  = 203941,

    SPELL_TIME_SPLIT            = 203833,
    SPELL_PULSE_VISUAL          = 203835, //Chrono Shard Pulse Visual
};

enum eEvents
{
    EVENT_A_BLAST               = 1,
    EVENT_CHRONO_SHARDS         = 2,
    EVENT_FORCE_BOMB            = 3,
    EVENT_BANISH_IN_TIME        = 4,
};

Position const tpPos[4] =
{
    {3134.70f, 4892.10f, 617.64f, 2.86f},
    {3151.23f, 5109.96f, 623.22f, 2.06f},
    {3154.38f, 4661.42f, 574.20f, 2.95f},
    {3327.04f, 4523.76f, 570.80f, 3.25f}
};

class boss_advisor_vandros : public CreatureScript
{
public:
    boss_advisor_vandros() : CreatureScript("boss_advisor_vandros") {}

    struct boss_advisor_vandrosAI : public BossAI
    {
        boss_advisor_vandrosAI(Creature* creature) : BossAI(creature, DATA_VANDROS) 
        {
            me->SetVisible(false);
            me->SetReactState(REACT_PASSIVE);
        }

        bool specialPhase;

        void Reset()
        {
            _Reset();
            specialPhase = false;
        }

        void EnterCombat(Unit* /*who*/) //40:56
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_A_BLAST, 3000); //40:59
            events.ScheduleEvent(EVENT_CHRONO_SHARDS, 6000); //41:02
            events.ScheduleEvent(EVENT_FORCE_BOMB, 16000); //41:13
        }

        void EnterEvadeMode()
        {
            me->SetReactState(REACT_AGGRESSIVE);
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_BANISH_IN_TIME_AT)
            {
                me->NearTeleportTo(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY(), me->GetHomePosition().GetPositionZ(), me->GetHomePosition().GetOrientation());
                me->GetMotionMaster()->Clear(false);

                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->getSource())
                        if (player->isAlive())
                        {
                            player->CastSpell(player, SPELL_BANISH_IN_TIME_TIMER, true);
                            player->CastSpell(player, SPELL_BANISH_IN_TIME_TP, true);
                        }
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_FORCE_BOMB)
                DoCast(target, SPELL_FORCE_BOMB_AT, true);
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->HealthBelowPct(50) && !specialPhase)
            {
                specialPhase = true;
                events.ScheduleEvent(EVENT_BANISH_IN_TIME, 500);
            }

            if (me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                if (me->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->m_spellInfo->Id == SPELL_BANISH_IN_TIME_AT)
                    if (me->IsWithinMeleeRange(attacker))
                    {
                        me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoCast(me, SPELL_BANISH_IN_TIME_REMOV, true);
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
                    case EVENT_A_BLAST:
                        DoCast(SPELL_ACCELERATING_BLAST);
                        events.ScheduleEvent(EVENT_A_BLAST, 6000);
                        break;
                    case EVENT_CHRONO_SHARDS:
                        for(uint8 i = 0; i < 3; i++)
                            DoCast(SPELL_CHRONO_SHARDS);
                        events.ScheduleEvent(EVENT_CHRONO_SHARDS, 6000);
                        break;
                    case EVENT_FORCE_BOMB:
                        DoCast(SPELL_FORCE_BOMB);
                        events.ScheduleEvent(EVENT_FORCE_BOMB, 16000);
                        break;
                    case EVENT_BANISH_IN_TIME:
                        instance->SetData(DATA_RAND_TELEPORT, urand(0,3));
                        DoStopAttack();
                        DoCast(me, SPELL_BANISH_IN_TIME_STUN, true);
                        DoCast(SPELL_BANISH_IN_TIME_AT);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_advisor_vandrosAI (creature);
    }
};

//102849
class npc_vandros_chrono_shard : public CreatureScript
{
public:
    npc_vandros_chrono_shard() : CreatureScript("npc_vandros_chrono_shard") {}

    struct npc_vandros_chrono_shardAI : public ScriptedAI
    {
        npc_vandros_chrono_shardAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_1, 8000);
            DoCast(me, SPELL_PULSE_VISUAL, true);
            Position pos;
            me->GetRandomNearPosition(pos, 10.0f);
            me->GetMotionMaster()->MovePoint(1, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        DoCast(me, SPELL_TIME_SPLIT, true);
                        me->DespawnOrUnsummon(1000);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vandros_chrono_shardAI(creature);
    }
};

//203883
class spell_vandros_teleport_plr : public SpellScriptLoader
{
public:
    spell_vandros_teleport_plr() : SpellScriptLoader("spell_vandros_teleport_plr") { }

    class spell_vandros_teleport_plr_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vandros_teleport_plr_SpellScript);

        uint8 rand;

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            if (InstanceScript* instance = player->GetInstanceScript())
                rand = instance->GetData(DATA_RAND_TELEPORT);

            player->TeleportTo(player->GetMapId(), tpPos[rand].GetPositionX(), tpPos[rand].GetPositionY(), tpPos[rand].GetPositionZ(), tpPos[rand].GetOrientation());
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_vandros_teleport_plr_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_TELEPORT_L);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_vandros_teleport_plr_SpellScript();
    }
};

//203914
class spell_vandros_banish_in_time_timer : public SpellScriptLoader
{
    public:
        spell_vandros_banish_in_time_timer() : SpellScriptLoader("spell_vandros_banish_in_time_timer") { }

        class spell_vandros_banish_in_time_timer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_vandros_banish_in_time_timer_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetTarget())
                    return;

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                    GetTarget()->Kill(GetTarget());
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_vandros_banish_in_time_timer_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_vandros_banish_in_time_timer_AuraScript();
        }
};

void AddSC_boss_advisor_vandros()
{
    new boss_advisor_vandros();
    new npc_vandros_chrono_shard();
    new spell_vandros_teleport_plr();
    new spell_vandros_banish_in_time_timer();
}