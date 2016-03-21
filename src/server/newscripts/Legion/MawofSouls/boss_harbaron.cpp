/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
    Encounter: Harbaron
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_COSMIC_SCYTHE         = 205330,
    SPELL_COSMIC_SCYTHE_VIS     = 198580,
    SPELL_COSMIC_SCYTHE_VIS_2   = 194667,
    SPELL_COSMIC_SCYTHE_DMG     = 194218,
    SPELL_SUM_SHACKLED_SERVITOR = 194231,
    SPELL_FRAGMENT              = 194325,

    //Summons
    SPELL_SHACKLED_SERVITOR     = 194259,
    SPELL_VOID_SNAP             = 194266,
};

enum eEvents
{
    EVENT_COSMIC_SCYTHE         = 1,
    EVENT_SUM_SHACKLED_SERVITOR = 2,
    EVENT_FRAGMENT              = 3,
};

class boss_harbaron : public CreatureScript
{
public:
    boss_harbaron() : CreatureScript("boss_harbaron") { }

    struct boss_harbaronAI : public BossAI
    {
        boss_harbaronAI(Creature* creature) : BossAI(creature, DATA_HARBARON) {}

        uint8 scytheRange;
        uint32 sumScytheTimer;
        bool summonScythe;
        float angle;

        void Reset()
        {
            events.Reset();
            _Reset();
            summonScythe = false;
            sumScytheTimer = 100;
            std::list<Creature*> fragment;
            GetCreatureListWithEntryInGrid(fragment, me, NPC_SOUL_FRAGMENT, 100.0f);
            for (std::list<Creature*>::iterator itr = fragment.begin(); itr != fragment.end(); ++itr)
                (*itr)->DespawnOrUnsummon();
        }

        void EnterCombat(Unit* /*who*/) //21:57
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            //events.ScheduleEvent(EVENT_COSMIC_SCYTHE, 4000);         //22:01, 22:09, 22:24, 22:32
            //events.ScheduleEvent(EVENT_SUM_SHACKLED_SERVITOR, 7000); //22:04, 22:27, 22:56, 23:27
            events.ScheduleEvent(EVENT_FRAGMENT, 1800);             //22:15, 22:48, 23:18
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

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_COSMIC_SCYTHE)
            {
                summonScythe = true;
                scytheRange = 0;
                angle = me->GetRelativeAngle(target);
            }

            if (spell->Id == SPELL_FRAGMENT)
                for (uint8 i = 0; i < 2; i++)
                    target->SummonCreature(NPC_SOUL_FRAGMENT, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 1);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (summonScythe)
            {
                if (sumScytheTimer <= diff)
                {
                    Position pos;
                    me->GetNearPosition(pos, 0.0f + scytheRange, angle);
                    me->SummonCreature(NPC_COSMIC_SCYTHE_2, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() + 5);
                    scytheRange += 4;
                    sumScytheTimer = 100;
                    if (scytheRange >= 30)
                        summonScythe = false;
                }
                else sumScytheTimer -=diff;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_COSMIC_SCYTHE:
                        DoCast(SPELL_COSMIC_SCYTHE);
                        events.ScheduleEvent(EVENT_COSMIC_SCYTHE, 8000);
                        break;
                    case EVENT_SUM_SHACKLED_SERVITOR:
                    {
                        //Talk();
                        Position pos;
                        me->GetNearPosition(pos, 20.0f, float(urand(0, 5)));
                        me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), EVENT_SUM_SHACKLED_SERVITOR);
                        events.ScheduleEvent(EVENT_SUM_SHACKLED_SERVITOR, 23000);
                        break;
                    }
                    case EVENT_FRAGMENT:
                        //Talk();
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(pTarget, SPELL_FRAGMENT);
                        events.ScheduleEvent(EVENT_FRAGMENT, 18000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_harbaronAI (creature);
    }
};

//100839
class npc_harbaron_scythe : public CreatureScript
{
public:
    npc_harbaron_scythe() : CreatureScript("npc_harbaron_scythe") { }

    struct npc_harbaron_scytheAI : public ScriptedAI
    {
        npc_harbaron_scytheAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset()
        {
            DoCast(me, SPELL_COSMIC_SCYTHE_VIS, true);
            events.ScheduleEvent(EVENT_1, 2000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        me->RemoveAurasDueToSpell(SPELL_COSMIC_SCYTHE_VIS);
                        DoCast(me, SPELL_COSMIC_SCYTHE_VIS_2, true);
                        me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 5, SPELL_COSMIC_SCYTHE_DMG);
                        me->DespawnOrUnsummon(1000);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_harbaron_scytheAI(creature);
    }
};

//98693
class npc_harbaron_shackled_servitor : public CreatureScript
{
public:
    npc_harbaron_shackled_servitor() : CreatureScript("npc_harbaron_shackled_servitor") { }

    struct npc_harbaron_shackled_servitorAI : public ScriptedAI
    {
        npc_harbaron_shackled_servitorAI(Creature* creature) : ScriptedAI(creature) 
        {
            SetCombatMovement(false);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_SHACKLED_SERVITOR, true);
            DoZoneInCombat(me, 100.0f);
        }

        void EnterCombat(Unit* /*who*/) //22:06
        {
            events.ScheduleEvent(EVENT_1, 2000); //22:08, 22:15, 22:21
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
                        DoCast(SPELL_VOID_SNAP);
                        events.ScheduleEvent(EVENT_1, 6000);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_harbaron_shackled_servitorAI(creature);
    }
};

//98761
class npc_harbaron_soul_fragment : public CreatureScript
{
public:
    npc_harbaron_soul_fragment() : CreatureScript("npc_harbaron_soul_fragment") { }

    struct npc_harbaron_soul_fragmentAI : public ScriptedAI
    {
        npc_harbaron_soul_fragmentAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            summoner->CastSpell(me, 194345, true); //Clone Caster
            DoCast(summoner, 194344, true); //Dmg tick summoner
            DoCast(me, 194381, true); //Scale
            me->GetMotionMaster()->MoveRandom(15.0f);
        }

        void JustDied(Unit* /*killer*/)
        {
            if (me->ToTempSummon())
                if (Unit* summoner = me->ToTempSummon()->GetSummoner())
                {
                    if (Aura* aura = summoner->GetAura(194344, me->GetGUID()))
                        aura->Remove();

                    if (!summoner->HasAura(194344))
                        summoner->RemoveAurasDueToSpell(194327);
                }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_harbaron_soul_fragmentAI(creature);
    }
};

void AddSC_boss_harbaron()
{
    new boss_harbaron();
    new npc_harbaron_scythe();
    new npc_harbaron_shackled_servitor();
    new npc_harbaron_soul_fragment();
}