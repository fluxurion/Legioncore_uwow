/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
    Encounter: Arch-druid glaidalis
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "darkheart_thicket.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_PRE_GRIEVOUS_LEAP     = 197709, // Используется что бы убить треш. Пред-эвент спелл.
    SPELL_GRIEVOUS_LEAP         = 196346,
    SPELL_GRIEVOUS_LEAP_RETURN  = 198095,
    SPELL_GRIEVOUS_LEAP_DOT     = 196376,
    SPELL_PRIMAL_RAMPAGE_MORPH  = 198360,
    SPELL_PRIMAL_RAMPAGE_KNOCK  = 198376,
    SPELL_PRIMAL_RAMPAGE_CHARGE = 198379,
    SPELL_NIGHTFALL             = 198400,
};

enum eEvents
{
    EVENT_PRE_GRIEVOUS_LEAP     = 1, // Для пред эвента.
    EVENT_GRIEVOUS_LEAP         = 2,
    EVENT_PRIMAL_RAMPAGE        = 3,
    EVENT_NIGHTFALL             = 4,
};

class boss_arch_druid_glaidalis : public CreatureScript
{
public:
    boss_arch_druid_glaidalis() : CreatureScript("boss_arch_druid_glaidalis") { }

    struct boss_arch_druid_glaidalisAI : public BossAI
    {
        boss_arch_druid_glaidalisAI(Creature* creature) : BossAI(creature, DATA_GLAIDALIS) {}

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) //35:36
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            events.ScheduleEvent(EVENT_GRIEVOUS_LEAP, 6000);    //35:42
            events.ScheduleEvent(EVENT_PRIMAL_RAMPAGE, 14000);  //35:50
            events.ScheduleEvent(EVENT_NIGHTFALL, 20000);       //35:56
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (me->HasAura(196348) && me->getVictim())
                    DoCast(me->getVictim(), SPELL_GRIEVOUS_LEAP_RETURN);
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_GRIEVOUS_LEAP_RETURN)
                DoCast(target, SPELL_GRIEVOUS_LEAP_DOT, true);
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
                    case EVENT_GRIEVOUS_LEAP:
                        DoCast(SPELL_GRIEVOUS_LEAP);
                        events.ScheduleEvent(EVENT_GRIEVOUS_LEAP, 16000);
                        break;
                    case EVENT_PRIMAL_RAMPAGE:
                        DoCast(me, SPELL_PRIMAL_RAMPAGE_MORPH, true);
                        if (Unit* pTaget = me->getVictim())
                        {
                            DoCast(pTaget, SPELL_PRIMAL_RAMPAGE_KNOCK, true);
                            DoCast(pTaget, SPELL_PRIMAL_RAMPAGE_CHARGE);
                        }
                        events.ScheduleEvent(EVENT_PRIMAL_RAMPAGE, 16000);
                        break;
                    case EVENT_NIGHTFALL:
                        DoCast(SPELL_NIGHTFALL);
                        events.ScheduleEvent(EVENT_NIGHTFALL, 16000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_arch_druid_glaidalisAI (creature);
    }
};

//196346
class spell_glaidalis_grievous_leap : public SpellScriptLoader
{
public:
    spell_glaidalis_grievous_leap() : SpellScriptLoader("spell_glaidalis_grievous_leap") { }

    class spell_glaidalis_grievous_leap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_glaidalis_grievous_leap_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Creature* glaidalis = GetCaster()->ToCreature();
            if (targets.empty() || !glaidalis)
                return;

            targets.clear();

            if (Unit* target = glaidalis->AI()->SelectTarget(SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                targets.push_back(target);
        }

        void HandleOnHit()
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), 196348, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_glaidalis_grievous_leap_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_glaidalis_grievous_leap_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_glaidalis_grievous_leap_SpellScript();
    }
};

//196376
class spell_glaidalis_grievous_tear : public SpellScriptLoader
{
public:
    spell_glaidalis_grievous_tear() : SpellScriptLoader("spell_glaidalis_grievous_tear") { }

    class spell_glaidalis_grievous_tear_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_glaidalis_grievous_tear_AuraScript)

        void OnPeriodic(AuraEffect const* aurEff)
        {
            if (!GetTarget())
                return;

            if (GetTarget()->HealthAbovePct(GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                GetTarget()->RemoveAurasDueToSpell(aurEff->GetId());
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_glaidalis_grievous_tear_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_glaidalis_grievous_tear_AuraScript();
    }
};

void AddSC_boss_arch_druid_glaidalis()
{
    new boss_arch_druid_glaidalis();
    new spell_glaidalis_grievous_leap();
    new spell_glaidalis_grievous_tear();
}