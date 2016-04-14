/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
    Encounter: Illysanna Ravencrest
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_rook_hold_dungeon.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_FURY_POWER_OVERRIDE   = 197367,
    SPELL_REGEN_POWER           = 197394,
    SPELL_BRUTAL_GLAIVE         = 197546,
    SPELL_VENGEFUL_SHEAR        = 197418,
    SPELL_DARK_RUSH_FILTER      = 197478,
    SPELL_DARK_RUSH_CHARGE      = 197484,
    SPELL_PHASE_2_JUMP          = 197622,
    SPELL_EYE_BEAMS             = 197674,
    SPELL_EYE_BEAMS_AURA        = 197696,
    SPELL_EYE_BEAMS_AT          = 197703,
    SPELL_FIXATE_BEAM           = 197687,
};

enum eEvents
{
    EVENT_BRUTAL_GLAIVE         = 1,
    EVENT_VENGEFUL_SHEAR        = 2,
    EVENT_DARK_RUSH_1           = 3,
    EVENT_DARK_RUSH_2           = 4,
    EVENT_SUMMON_ADDS           = 5,
    EVENT_EYE_BEAMS             = 6,
    EVENT_PHASE_1               = 7,
};

Position const summonsPos[2] =
{
    {3105.11f, 7296.94f, 103.28f, 3.14f}, //NPC_RISEN_ARCANIST
    {3083.07f, 7313.35f, 103.28f, 4.92f}  //NPC_SOUL_TORN_VANGUARD
};

class boss_illysanna_ravencrest : public CreatureScript
{
public:
    boss_illysanna_ravencrest() : CreatureScript("boss_illysanna_ravencrest") {}

    struct boss_illysanna_ravencrestAI : public BossAI
    {
        boss_illysanna_ravencrestAI(Creature* creature) : BossAI(creature, DATA_ILLYSANNA) {}

        bool glaiveDubleCast;
        bool phaseTwo;
        int8 PowerCount;

        void Reset()
        {
            _Reset();
            glaiveDubleCast = true;
            me->SetPower(POWER_ENERGY, 50); //DoCast(me, SPELL_FURY_POWER_OVERRIDE, true);
            DoCast(me, SPELL_REGEN_POWER, true);
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            phaseTwo = false;
        }

        void EnterCombat(Unit* /*who*/) //07:35
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            DefaultEvents();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void DefaultEvents()
        {
            events.ScheduleEvent(EVENT_BRUTAL_GLAIVE, 6000);  //07:41, 07:57
            events.ScheduleEvent(EVENT_VENGEFUL_SHEAR, 9000); //07:44, 07:59, 08:11
            events.ScheduleEvent(EVENT_DARK_RUSH_1, 12000);   //07:47, 08:18
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_VENGEFUL_SHEAR:
                    if (!target->IsActiveSaveAbility())
                        DoCast(target, 197429, true); //debuff: mod damage
                    break;
                case SPELL_DARK_RUSH_CHARGE:
                    me->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 50.0f, SPELL_DARK_RUSH_CHARGE);
                    break;
                case SPELL_EYE_BEAMS:
                {
                    float x = target->GetPositionX();
                    float y = target->GetPositionY();
                    float z = target->GetPositionZ();
                    if (Creature* stalker = me->SummonCreature(NPC_EYE_BEAM_STALKER, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 12000))
                        DoCast(stalker, SPELL_EYE_BEAMS_AURA, true);
                    break;
                }
                default:
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != EFFECT_MOTION_TYPE)
                return;

            switch (id)
            {
                case SPELL_DARK_RUSH_CHARGE:
                    events.ScheduleEvent(EVENT_DARK_RUSH_2, 500);
                    break;
                case SPELL_PHASE_2_JUMP:
                    me->GetMotionMaster()->Clear(false);
                    events.ScheduleEvent(EVENT_SUMMON_ADDS, 1000);
                    events.ScheduleEvent(EVENT_EYE_BEAMS, 2000);
                    break;
                case 1:
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    phaseTwo = false;
                    break;
            }
        }

        void DoAction(int32 const action)
        {
            if (!phaseTwo)
            {
                phaseTwo = true;
                events.Reset();
                DoStopAttack();
                me->RemoveAurasDueToSpell(SPELL_REGEN_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoCast(me, SPELL_PHASE_2_JUMP, true);
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (phaseTwo)
                damage = 0;
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
                    case EVENT_BRUTAL_GLAIVE:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true, -SPELL_BRUTAL_GLAIVE))
                            DoCast(pTarget, SPELL_BRUTAL_GLAIVE);
                        if (glaiveDubleCast)
                            glaiveDubleCast = false;
                        else
                            glaiveDubleCast = true;
                        events.ScheduleEvent(EVENT_BRUTAL_GLAIVE, !glaiveDubleCast ? 0 : 16000);
                        break;
                    case EVENT_VENGEFUL_SHEAR:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_VENGEFUL_SHEAR);
                        events.ScheduleEvent(EVENT_VENGEFUL_SHEAR, 12000);
                        break;
                    case EVENT_DARK_RUSH_1:
                        DoCast(SPELL_DARK_RUSH_FILTER);
                        events.ScheduleEvent(EVENT_DARK_RUSH_1, 30000);
                        events.ScheduleEvent(EVENT_DARK_RUSH_2, 3000);
                        break;
                    case EVENT_DARK_RUSH_2:
                        DoCast(SPELL_DARK_RUSH_CHARGE);
                        break;
                    case EVENT_SUMMON_ADDS:
                        me->SummonCreature(NPC_RISEN_ARCANIST, summonsPos[0]); //Heroic?
                        me->SummonCreature(NPC_SOUL_TORN_VANGUARD, summonsPos[1]);
                        break;
                    case EVENT_EYE_BEAMS:
                        PowerCount = me->GetPower(POWER_ENERGY);
                        me->SetPower(POWER_ENERGY, PowerCount - 33);
                        DoCast(SPELL_EYE_BEAMS);
                        if (PowerCount <= 0)
                            events.ScheduleEvent(EVENT_PHASE_1, 14000);
                        else
                            events.ScheduleEvent(EVENT_EYE_BEAMS, 12000);
                        break;
                    case EVENT_PHASE_1:
                        DefaultEvents();
                        DoCast(me, SPELL_FURY_POWER_OVERRIDE, true);
                        me->GetMotionMaster()->MoveJump(3089.76f, 7299.66f, 103.53f, 25, 15, 1); //HomePos
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_illysanna_ravencrestAI (creature);
    }
};

//100436
class npc_illysanna_eye_beam_stalker : public CreatureScript
{
public:
    npc_illysanna_eye_beam_stalker() : CreatureScript("npc_illysanna_eye_beam_stalker") {}

    struct npc_illysanna_eye_beam_stalkerAI : public ScriptedAI
    {
        npc_illysanna_eye_beam_stalkerAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        Unit* plrTarget;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_1, 500);
            events.ScheduleEvent(EVENT_2, 1000);
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
                switch (eventId)
                {
                    case EVENT_1:
                    {
                        if (Creature* summoner = me->GetAnyOwner()->ToCreature())
                            if (Unit* target = summoner->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            {
                                plrTarget = target;
                                me->CastSpell(target, SPELL_FIXATE_BEAM, true);
                            }
                        events.ScheduleEvent(EVENT_1, 6000);
                        break;
                    }
                    case EVENT_2:
                        me->GetMotionMaster()->MovePoint(1, plrTarget->GetPositionX(), plrTarget->GetPositionY(), plrTarget->GetPositionZ());
                        events.ScheduleEvent(EVENT_2, 500);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_illysanna_eye_beam_stalkerAI(creature);
    }
};

//197394
class spell_illysanna_periodic_energize : public SpellScriptLoader
{
public:
    spell_illysanna_periodic_energize() : SpellScriptLoader("spell_illysanna_periodic_energize") { }

    class spell_illysanna_periodic_energize_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_illysanna_periodic_energize_AuraScript);

        uint8 PowerCount = 0;

        void OnTick(AuraEffect const* aurEff)
        {
            Creature* caster = GetCaster()->ToCreature();
            if (!caster || !caster->isInCombat())
                return;

            PowerCount = caster->GetPower(POWER_ENERGY);

            if (PowerCount < 100)
                caster->SetPower(POWER_ENERGY, PowerCount + 1);
            else
                caster->AI()->DoAction(true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_illysanna_periodic_energize_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_illysanna_periodic_energize_AuraScript();
    }
};

//197696
class spell_illysanna_eye_beams : public SpellScriptLoader
{
public:
    spell_illysanna_eye_beams() : SpellScriptLoader("spell_illysanna_eye_beams") { }

    class spell_illysanna_eye_beams_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_illysanna_eye_beams_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* target = GetTarget();
            if (!target)
                return;

            target->CastSpell(target, SPELL_EYE_BEAMS_AT, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_illysanna_eye_beams_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_illysanna_eye_beams_AuraScript();
    }
};

void AddSC_boss_illysanna_ravencrest()
{
    new boss_illysanna_ravencrest();
    new npc_illysanna_eye_beam_stalker();
    new spell_illysanna_periodic_energize();
    new spell_illysanna_eye_beams();
}