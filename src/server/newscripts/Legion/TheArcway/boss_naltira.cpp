/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
    Encounter: Naltira
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_arcway.h"

//TO-DO: pre-event

enum Spells
{
    //SPELL_ARCANE_WEB_BEAM       = 209833, //Arcane Web Beam (Boss)
    SPELL_BLINK_STRIKES_VISUAL  = 199864,
    SPELL_BLINK_STRIKES_SEARCH  = 199809,
    SPELL_BLINK_STRIKES_TP      = 199810,
    SPELL_BLINK_STRIKES_DMG     = 199811,
    SPELL_NETHER_VENOM          = 199956,
    SPELL_TANGLED_WEB           = 200227,
    SPELL_TANGLED_WEB_JUMP      = 200230,
    SPELL_TANGLED_WEB_DMG       = 200284,
    SPELL_TANGLED_BEAM          = 200237,
};

enum eEvents
{
    EVENT_BLINK_STRIKES_1       = 1,
    EVENT_BLINK_STRIKES_2       = 2,
    EVENT_NETHER_VENOM          = 3,
    EVENT_TANGLED_WEB           = 4,
};

class boss_naltira : public CreatureScript
{
public:
    boss_naltira() : CreatureScript("boss_naltira") {}

    struct boss_naltiraAI : public BossAI
    {
        boss_naltiraAI(Creature* creature) : BossAI(creature, DATA_NALTIRA) {}

        bool blinkEvent;
        bool checkWebPlayer;
        Unit* plrWebTarget;

        void Reset()
        {
            _Reset();
            blinkEvent = false;
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* /*who*/) //28:59
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_BLINK_STRIKES_1, 15000); //29:14
            events.ScheduleEvent(EVENT_NETHER_VENOM, 26000); //29:24
            events.ScheduleEvent(EVENT_TANGLED_WEB, 32000); //29:34
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
                case SPELL_BLINK_STRIKES_TP:
                    me->GetMotionMaster()->Clear(false);
                    me->SetFacingToObject(target);
                    events.ScheduleEvent(EVENT_BLINK_STRIKES_2, 500);
                    break;
                case SPELL_TANGLED_WEB_DMG:
                    if (checkWebPlayer)
                    {
                        checkWebPlayer = false;
                        plrWebTarget = target;
                    }
                    else
                        target->CastSpell(plrWebTarget, SPELL_TANGLED_BEAM, true);
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
                    case EVENT_BLINK_STRIKES_1:
                        DoStopAttack();
                        DoCast(me, SPELL_BLINK_STRIKES_SEARCH, true);
                        if (!blinkEvent)
                        {
                            blinkEvent = true;
                            DoCast(me, SPELL_BLINK_STRIKES_VISUAL, true);
                            events.ScheduleEvent(EVENT_BLINK_STRIKES_1, 3000);
                        }
                        else
                        {
                            blinkEvent = false;
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_BLINK_STRIKES_1, 30000);
                        }
                        break;
                    case EVENT_BLINK_STRIKES_2:
                        DoCast(me, SPELL_BLINK_STRIKES_DMG, true);
                        break;
                    case EVENT_NETHER_VENOM:
                        DoCast(SPELL_NETHER_VENOM);
                        events.ScheduleEvent(EVENT_NETHER_VENOM, 30000);
                        break;
                    case EVENT_TANGLED_WEB:
                        plrWebTarget = nullptr;
                        checkWebPlayer = true;
                        DoCast(SPELL_TANGLED_WEB);
                        Talk(0);
                        events.ScheduleEvent(EVENT_TANGLED_WEB, 22000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_naltiraAI (creature);
    }
};

//200227
class spell_naltira_tangled_web : public SpellScriptLoader
{
public:
    spell_naltira_tangled_web() : SpellScriptLoader("spell_naltira_tangled_web") { }

    class spell_naltira_tangled_web_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_naltira_tangled_web_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                if ((*itr)->GetTypeId() != TYPEID_PLAYER)
                    targets.remove((*itr));

            if (targets.size() < 2)
                targets.clear();
        }

        void HandleOnHit()
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetHitUnit()->CastSpell(GetCaster(), SPELL_TANGLED_WEB_JUMP, true);
            GetCaster()->CastSpell(GetHitUnit(), SPELL_TANGLED_WEB_DMG, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_naltira_tangled_web_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_naltira_tangled_web_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_naltira_tangled_web_SpellScript();
    }
};

//200251
class spell_naltira_tangled_web_check_dist : public SpellScriptLoader
{
public:
    spell_naltira_tangled_web_check_dist() : SpellScriptLoader("spell_naltira_tangled_web_check_dist") { }

    class spell_naltira_tangled_web_check_dist_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_naltira_tangled_web_check_dist_SpellScript);

        void HandleOnHit()
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetCaster()->GetDistance(GetHitUnit()) >= 30.0f)
            {
                GetCaster()->RemoveAurasDueToSpell(SPELL_TANGLED_WEB_DMG);
                GetHitUnit()->RemoveAurasDueToSpell(SPELL_TANGLED_WEB_DMG);
                GetHitUnit()->RemoveAurasDueToSpell(SPELL_TANGLED_BEAM);
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_naltira_tangled_web_check_dist_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_naltira_tangled_web_check_dist_SpellScript();
    }
};

void AddSC_boss_naltira()
{
    new boss_naltira();
    new spell_naltira_tangled_web();
    new spell_naltira_tangled_web_check_dist();
}