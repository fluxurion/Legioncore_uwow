/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
    Encounter: Glazer
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
    SPELL_PULSE             = 194849,
    SPELL_PULSE_AT          = 194853,
    SPELL_LINGERING_GAZE    = 194941,
    SPELL_TELEPORT_CENTR    = 194149,
    SPELL_ENERGIZE          = 193443,
    SPELL_FOCUSED           = 194289,
    SPELL_FOCUSED_AURA      = 194323,
    SPELL_BEAM_CHECK_BW     = 194463,
    SPELL_BEAM_VIS_1        = 194261,
    SPELL_BEAM_VIS_2        = 201042,

    //Lens
    SPELL_ACTIVATE_GO       = 197151,
    SPELL_DEACTIVATE_GO     = 198165,
    SPELL_LENS_SPAWN_VIS    = 203926, //Visual spawn
    SPELL_BEAM_VIS_3        = 193373,
    SPELL_BEAM_VIS_4        = 194333, //Target boss
    SPELL_BEAM_CHECK_PLR_1  = 194523,
    SPELL_BEAM_CHECK_PLR_2  = 194468,
    SPELL_BEAM_DMG          = 202046,
};

enum eEvents
{
    EVENT_PULSE             = 1,
    EVENT_LINGERING_GAZE    = 2,
    EVENT_TELEPORT          = 3,
    EVENT_ENERGIZE          = 4,
    EVENT_FOCUSED           = 5,
};

Position const beamPos[2] = 
{
    {4486.56f, -708.61f, 117.23f},
    {4510.05f, -676.14f, 117.54f}
};

Position const lensPos[3] =
{
    {4451.21f, -638.68f, 117.23f, 0.78f},
    {4433.92f, -655.97f, 117.23f, 2.35f},
    {4485.78f, -673.26f, 117.23f, 5.49f}
};

class boss_glazer : public CreatureScript
{
public:
    boss_glazer() : CreatureScript("boss_glazer") {}

    struct boss_glazerAI : public BossAI
    {
        boss_glazerAI(Creature* creature) : BossAI(creature, DATA_GLAZER) {}

        void Reset()
        {
            _Reset();
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* /*who*/) //17:03
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            //events.ScheduleEvent(EVENT_PULSE, 6000); //17:09, 17:21
            //events.ScheduleEvent(EVENT_LINGERING_GAZE, 12000); //17:15, 18:22, 18:41
            events.ScheduleEvent(EVENT_TELEPORT, 3000); //30s,17:35
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_TELEPORT_CENTR:
                    events.ScheduleEvent(EVENT_ENERGIZE, 500);
                    break;
                case SPELL_ENERGIZE:
                    me->GetMotionMaster()->Clear();
                    DoCast(me, SPELL_FOCUSED_AURA, true);
                    events.ScheduleEvent(EVENT_FOCUSED, 500);
                    break;
                case SPELL_BEAM_VIS_4:
                {
                    me->InterruptNonMeleeSpells(false, SPELL_BEAM_VIS_2);
                    me->RemoveAurasDueToSpell(SPELL_FOCUSED_AURA);
                    me->SetReactState(REACT_AGGRESSIVE);
                    EntryCheckPredicate pred(NPC_LENS);
                    summons.DoAction(true, pred);
                    break;
                }
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_PULSE:
                    //me->CastSpell(me, SPELL_PULSE_AT, true);
                    //me->CastSpell(me, SPELL_PULSE_AT, true);
                    break;
                case SPELL_FOCUSED:
                    DoCast(target, SPELL_BEAM_VIS_2, true);
                    break;
            }
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            summon->CastSpell(summon, SPELL_DEACTIVATE_GO, true);
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
                    case EVENT_PULSE:
                        DoCast(SPELL_PULSE);
                        events.ScheduleEvent(EVENT_PULSE, 12000);
                        break;
                    case EVENT_LINGERING_GAZE:
                        DoCast(SPELL_LINGERING_GAZE);
                        events.ScheduleEvent(EVENT_LINGERING_GAZE, 18000);
                        break;
                    case EVENT_TELEPORT:
                        DoStopAttack();
                        DoCast(SPELL_TELEPORT_CENTR);
                        break;
                    case EVENT_ENERGIZE:
                        DoCast(SPELL_ENERGIZE);
                        break;
                    case EVENT_FOCUSED:
                        for (uint8 i = 0; i < 3; i++)
                            me->SummonCreature(NPC_LENS, lensPos[i]);
                        me->SummonCreature(NPC_GLAZER_BEAM, beamPos[0]);
                        DoCast(SPELL_FOCUSED);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_glazerAI (creature);
    }
};

//102310
class npc_glazer_beam : public CreatureScript
{
public:
    npc_glazer_beam() : CreatureScript("npc_glazer_beam") {}

    struct npc_glazer_beamAI : public ScriptedAI
    {
        npc_glazer_beamAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetDisplayId(11686);
            moveComplete = false;
        }

        bool moveComplete;

        void Reset() {}

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (moveComplete)
                return;

            if (spell->Id == SPELL_FOCUSED)
            {
                moveComplete = true;
                me->GetMotionMaster()->MovePoint(1, beamPos[1]);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (me->GetAnyOwner())
                if (Creature* summoner = me->GetAnyOwner()->ToCreature())
                {
                    summoner->InterruptNonMeleeSpells(false, SPELL_FOCUSED);
                    summoner->InterruptNonMeleeSpells(false, SPELL_BEAM_VIS_2);
                    if (Creature* lens = me->FindNearestCreature(NPC_LENS, 30.0f))
                        summoner->CastSpell(lens, SPELL_BEAM_VIS_2, true);
                }

            me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_glazer_beamAI(creature);
    }
};

//98082
class npc_glazer_lens : public CreatureScript
{
public:
    npc_glazer_lens() : CreatureScript("npc_glazer_lens") {}

    struct npc_glazer_lensAI : public ScriptedAI
    {
        npc_glazer_lensAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetDisplayId(11686);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_1, 500);
            events.ScheduleEvent(EVENT_2, 1000);
            events.ScheduleEvent(EVENT_3, 3000);
        }

        void DoAction(int32 const action)
        {
            me->InterruptNonMeleeSpells(false, SPELL_BEAM_VIS_3);
            me->InterruptNonMeleeSpells(false, SPELL_BEAM_VIS_4);
            me->RemoveAurasDueToSpell(SPELL_LENS_SPAWN_VIS);
            me->DespawnOrUnsummon(2000);
        }

        void OnSpellClick(Unit* clicker)
        {
            float orient = me->GetOrientation() - 1.57f;
            if (orient < 0.0f)
                orient = 5.5f;

            me->SetFacingTo(orient);
        }

        void SearchBeamTarget()
        {
            std::list<Creature*> creatureList;
            me->GetCreatureListWithEntryInGrid(creatureList, NPC_BEAM_TARGET_TRIGGER, 100.0f);
            me->GetCreatureListWithEntryInGrid(creatureList, NPC_LENS, 100.0f);
            me->GetCreatureListWithEntryInGrid(creatureList, NPC_GLAZER, 100.0f);
            creatureList.sort();
            for (auto creature : creatureList)
            {
                float angle = me->GetAngle(creature);
                if (me->GetOrientation() >= (angle - 0.05f) && me->GetOrientation() <= (angle + 0.05f))
                {
                    if (creature->HasAura(SPELL_BEAM_VIS_2) || creature->HasAura(SPELL_BEAM_VIS_3) || creature->HasAura(SPELL_BEAM_VIS_4))
                    {
                        if (creature->GetEntry() == NPC_LENS && me->HasAura(SPELL_BEAM_VIS_3, creature->GetGUID()))
                            me->InterruptNonMeleeSpells(false, SPELL_BEAM_VIS_3);
                        return;
                    }

                    if (creature->GetEntry() != NPC_GLAZER)
                        DoCast(creature, SPELL_BEAM_VIS_3, true);
                    else
                        DoCast(creature, SPELL_BEAM_VIS_4, true);
                    return;
                }
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
                        DoCast(me, SPELL_ACTIVATE_GO, true);
                        break;
                    case EVENT_2:
                        DoCast(me, SPELL_LENS_SPAWN_VIS, true);
                        break;
                    case EVENT_3:
                        if (me->HasAura(SPELL_BEAM_VIS_2) || me->HasAura(SPELL_BEAM_VIS_3) || me->HasAura(SPELL_BEAM_VIS_4))
                            SearchBeamTarget();
                        events.ScheduleEvent(EVENT_3, 1000);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_glazer_lensAI(creature);
    }
};

//194463, 194468
class spell_glazer_beam : public SpellScriptLoader
{
public:
    spell_glazer_beam() : SpellScriptLoader("spell_glazer_beam") { }

    class spell_glazer_beam_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_glazer_beam_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (target->GetTypeId() != TYPEID_PLAYER)
                return;

            caster->CastSpell(target, SPELL_BEAM_DMG, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_glazer_beam_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_glazer_beam_SpellScript();
    }
};

//195032
class spell_glazer_radiation : public SpellScriptLoader
{
public:
    spell_glazer_radiation() :  SpellScriptLoader("spell_glazer_radiation") { }

    class spell_glazer_radiation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_glazer_radiation_SpellScript);

        uint8 stack = 0;
        int32 damage = 0;

        void RecalculateDamage()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Aura* aura = caster->GetAura(195034))
                stack = aura->GetStackAmount();

            damage = CalculatePct(GetHitDamage(), stack);
            SetHitDamage(GetHitDamage() + damage);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_glazer_radiation_SpellScript::RecalculateDamage);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_glazer_radiation_SpellScript();
    }
};

void AddSC_boss_glazer()
{
    new boss_glazer();
    new npc_glazer_beam();
    new npc_glazer_lens();
    new spell_glazer_beam();
    new spell_glazer_radiation();
}