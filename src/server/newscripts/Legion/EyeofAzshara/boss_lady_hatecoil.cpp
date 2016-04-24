/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
    Encounter: Lady Hatecoil
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
    SPELL_CRACKLING_THUNDER     = 197324,
    SPELL_CRACKLING_THUNDER_DMG = 197326,
    SPELL_STATIC_NOVA           = 193597,
    SPELL_CURSE_WITCH_1         = 193712, //MaxTargets 1
    SPELL_CURSE_WITCH_2         = 193716, //MaxTargets 3
    SPELL_CURSE_WITCH_3         = 193717, //MaxTargets 5
    SPELL_CURSE_WITCH_AURA      = 193698,
    SPELL_CURSE_WITCH_KNOCK     = 193700,
    SPELL_CURSE_WITCH_KILL_G    = 193720, //Hit npc: 98293
    SPELL_CURSE_WITCH_ROOT      = 194197,
    SPELL_BECKON_STORM          = 193682,
    SPELL_FOCUSED_LIGHTNING     = 193611,

    SPELL_SAND_DUNE_GO          = 193060,
    SPELL_SAND_DUNE_AT          = 193064,
};

enum eEvents
{
    EVENT_CRACKLING_THUNDER     = 1,
    EVENT_STATIC_NOVA           = 2,
    EVENT_CURSE_WITCH           = 3,
    EVENT_BECKON_STORM          = 4,
    EVENT_FOCUSED_LIGHTNING     = 5,
};

uint32 curseSpells[3] =
{
    SPELL_CURSE_WITCH_1,
    SPELL_CURSE_WITCH_2,
    SPELL_CURSE_WITCH_3
};

Position const homePos = {-3443.38f, 4590.50f, 0.0f};

class boss_lady_hatecoil : public CreatureScript
{
public:
    boss_lady_hatecoil() : CreatureScript("boss_lady_hatecoil") {}

    struct boss_lady_hatecoilAI : public BossAI
    {
        boss_lady_hatecoilAI(Creature* creature) : BossAI(creature, DATA_HATECOIL) {}

        void Reset()
        {
            DelSandDune();
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) //51:13
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();

            events.ScheduleEvent(EVENT_CRACKLING_THUNDER, 4000);
            events.ScheduleEvent(EVENT_STATIC_NOVA, 11000);       //51:24
            events.ScheduleEvent(EVENT_CURSE_WITCH, 17000);       //51:30
            events.ScheduleEvent(EVENT_BECKON_STORM, 19000);      //51:32, 52:21
            events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 25000); //51:38, 52:15
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void DelSandDune()
        {
            std::list<GameObject*> duneList;
            me->GetGameObjectListWithEntryInGrid(duneList, GO_SAND_DUNE, 100.0f);
            for (std::list<GameObject*>::iterator itr = duneList.begin(); itr != duneList.end(); ++itr)
                (*itr)->Delete();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            switch (spell->Id)
            {
                case SPELL_CURSE_WITCH_1:
                case SPELL_CURSE_WITCH_2:
                case SPELL_CURSE_WITCH_3:
                    DoCast(target, SPELL_CURSE_WITCH_AURA, true);
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

            if (me->GetDistance(me->GetHomePosition()) >= 50.0f)
            {
                EnterEvadeMode();
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CRACKLING_THUNDER:
                        DoCast(me, SPELL_CRACKLING_THUNDER, true);
                        events.ScheduleEvent(EVENT_CRACKLING_THUNDER, 4000);
                        break;
                    case EVENT_STATIC_NOVA:
                        DoCast(SPELL_STATIC_NOVA);
                        events.ScheduleEvent(EVENT_STATIC_NOVA, 35000);
                        break;
                    case EVENT_CURSE_WITCH:
                        DoCast(curseSpells[urand(0, 2)]);
                        events.ScheduleEvent(EVENT_CURSE_WITCH, 24000);
                        break;
                    case EVENT_BECKON_STORM:
                        DoCast(SPELL_BECKON_STORM);
                        events.ScheduleEvent(EVENT_BECKON_STORM, 47000);
                        break;
                    case EVENT_FOCUSED_LIGHTNING:
                        DoCast(SPELL_FOCUSED_LIGHTNING);
                        events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 36000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_lady_hatecoilAI (creature);
    }
};

//97853
class npc_hatecoil_sand_dune : public CreatureScript
{
public:
    npc_hatecoil_sand_dune() : CreatureScript("npc_hatecoil_sand_dune") {}

    struct npc_hatecoil_sand_duneAI : public ScriptedAI
    {
        npc_hatecoil_sand_duneAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        ObjectGuid DustGUID;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_SAND_DUNE_GO, true);
            DoCast(me, SPELL_SAND_DUNE_AT, true);
        }

        void JustSummonedGO(GameObject* go)
        {
            DustGUID = go->GetGUID();
        }

        void SpellHit(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == 193625)
            {
                me->RemoveAurasDueToSpell(SPELL_SAND_DUNE_AT);

                if (GameObject* go = me->GetMap()->GetGameObject(DustGUID))
                    go->Delete();
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_hatecoil_sand_duneAI(creature);
    }
};

//197324
class spell_crackling_thunder_filter : public SpellScriptLoader
{
public:
    spell_crackling_thunder_filter() : SpellScriptLoader("spell_crackling_thunder_filter") { }

    class spell_crackling_thunder_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_crackling_thunder_filter_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;

            std::list<WorldObject*> tempList;
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                if ((*itr)->GetDistance(homePos) > 45.0f)
                    tempList.push_back((*itr));

            targets.clear();

            if (!tempList.empty())
                for (std::list<WorldObject*>::const_iterator itr = tempList.begin(); itr != tempList.end(); ++itr)
                    targets.push_back((*itr));
        }

        void HandleOnHit()
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), SPELL_CRACKLING_THUNDER_DMG, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_crackling_thunder_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_crackling_thunder_filter_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_crackling_thunder_filter_SpellScript();
    }
};

//193698
class spell_hatecoil_curse_of_the_witch : public SpellScriptLoader
{
    public:
        spell_hatecoil_curse_of_the_witch() : SpellScriptLoader("spell_hatecoil_curse_of_the_witch") { }

        class spell_hatecoil_curse_of_the_witch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hatecoil_curse_of_the_witch_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster() || !GetTarget())
                    return;

                GetTarget()->CastSpell(GetTarget(), SPELL_CURSE_WITCH_KNOCK, true);
                GetTarget()->CastSpell(GetTarget(), SPELL_CURSE_WITCH_KILL_G, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_hatecoil_curse_of_the_witch_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hatecoil_curse_of_the_witch_AuraScript();
        }
};

void AddSC_boss_lady_hatecoil()
{
    new boss_lady_hatecoil();
    new npc_hatecoil_sand_dune();
    new spell_crackling_thunder_filter();
    new spell_hatecoil_curse_of_the_witch();
}