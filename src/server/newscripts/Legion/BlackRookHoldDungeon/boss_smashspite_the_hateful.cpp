/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
    Encounter: Smashspite the Hateful
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_rook_hold_dungeon.h"


enum Spells
{
    SPELL_ZERO_REGEN            = 72242,
    SPELL_BRUTALITY_PROC        = 198114,
    SPELL_EARTHSHAKING_STOMP    = 198073,
    SPELL_BRUTAL_HAYMAKER       = 198245,

    SPELL_FEL_VOMIT             = 198446,
    SPELL_FEL_VOMIT_DMG         = 198499,
};

enum eEvents
{
    EVENT_EARTHSHAKING_STOMP    = 1,
    EVENT_FEL_ATTACK            = 2,
};

Position const felPos[10] =
{
    {3377.12f, 7379.05f, 260.46f}, //100759
    {3405.04f, 7360.28f, 250.10f},
    {3410.22f, 7311.27f, 250.10f},
    {3392.56f, 7317.72f, 260.46f},
    {3379.99f, 7295.96f, 268.62f},
    {3388.76f, 7284.25f, 237.00f},
    {3257.77f, 7188.85f, 255.80f},
    {3352.13f, 7259.52f, 251.29f},
    {3206.34f, 7261.78f, 255.80f},
    {3267.14f, 7182.61f, 244.89f},
};

Position const centrPos = {3269.27f, 7289.28f, 231.47f};

class boss_smashspite_the_hateful : public CreatureScript
{
public:
    boss_smashspite_the_hateful() : CreatureScript("boss_smashspite_the_hateful") { }

    struct boss_smashspite_the_hatefulAI : public BossAI
    {
        boss_smashspite_the_hatefulAI(Creature* creature) : BossAI(creature, DATA_SMASHSPITE) {}

        ObjectGuid felbatContainer[10];

        void Reset()
        {
            _Reset();
            DoCast(me, SPELL_ZERO_REGEN, true);
            DoCast(me, SPELL_BRUTALITY_PROC, true);
            me->SetPower(POWER_ENERGY, 0);

            for (uint8 i = 0; i < 10; i++)
            {
                felbatContainer[i].Clear();

                if (Creature* summon = me->SummonCreature(NPC_FEL_BAT, felPos[i]))
                    felbatContainer[i] = summon->GetGUID();
            }
        }

        void EnterCombat(Unit* /*who*/) //16:43
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_EARTHSHAKING_STOMP, 12000); //16:55, 17:12, 17:29
            events.ScheduleEvent(EVENT_FEL_ATTACK, 25000);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
            DoCast(205271);
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
                    case EVENT_EARTHSHAKING_STOMP:
                        DoCast(SPELL_EARTHSHAKING_STOMP);
                        events.ScheduleEvent(EVENT_EARTHSHAKING_STOMP, 17000);
                        break;
                    case EVENT_FEL_ATTACK:
                        if (Creature* fel = me->GetCreature(*me, felbatContainer[urand(0, 9)]))
                            fel->AI()->DoAction(true);
                        events.ScheduleEvent(EVENT_FEL_ATTACK, 25000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_smashspite_the_hatefulAI (creature);
    }
};

//100759
class npc_smashspite_fel_bat : public CreatureScript
{
public:
    npc_smashspite_fel_bat() : CreatureScript("npc_smashspite_fel_bat") {}

    struct npc_smashspite_fel_batAI : public ScriptedAI
    {
        npc_smashspite_fel_batAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        Unit* plrTarget;

        void Reset() {}

        void DoAction(int32 const action)
        {
            plrTarget = nullptr;
            DoCast(SPELL_FEL_VOMIT);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (target->GetTypeId() != TYPEID_PLAYER)
                return;

            if (spell->Id == SPELL_FEL_VOMIT)
            {
                plrTarget = target;
                events.ScheduleEvent(EVENT_1, 6000);
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
                    {
                        Position pos;
                        float x, y, z;
                        float ang1 = centrPos.GetRelativeAngle(me);
                        centrPos.SimplePosXYRelocationByAngle(pos, 30.0f, ang1);
                        float ang2 = pos.GetRelativeAngle(plrTarget);
                        for (uint8 i = 0; i < 25; i++)
                        {
                            pos.SimplePosXYRelocationByAngle(x, y, z, i * 3, ang2);
                            me->CastSpell(x, y, z, SPELL_FEL_VOMIT_DMG, true);
                        }
                        break;
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_smashspite_fel_batAI(creature);
    }
};

//198114
class spell_smashpite_brutality_proc : public SpellScriptLoader
{
public:
    spell_smashpite_brutality_proc() : SpellScriptLoader("spell_smashpite_brutality_proc") { }

    class spell_smashpite_brutality_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_smashpite_brutality_proc_AuraScript);

        uint8 PowerCount = 0;

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster || caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            PowerCount = caster->GetPower(POWER_ENERGY);

            if (PowerCount < 100)
            {
                if (PowerCount < 32 || PowerCount == 92)
                {
                    caster->SetPower(POWER_ENERGY, PowerCount + 8);
                }
                else
                    caster->SetPower(POWER_ENERGY, PowerCount + 5);
            }
            else
            {
                if (caster->getVictim())
                    caster->CastSpell(caster->getVictim(), SPELL_BRUTAL_HAYMAKER);
            }
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_smashpite_brutality_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_smashpite_brutality_proc_AuraScript();
    }
};

void AddSC_boss_smashspite_the_hateful()
{
    new boss_smashspite_the_hateful();
    new npc_smashspite_fel_bat();
    new spell_smashpite_brutality_proc();
}