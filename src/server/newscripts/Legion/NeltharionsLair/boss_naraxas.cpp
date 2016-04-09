/*
    http://uwow.biz
    Dungeon : Neltharions Lair 100-110
    Encounter: Naraxas
    TODO: Не сделан эвент "шипастый язык" - нехватает инфы.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "neltharions_lair.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    //SPELL_INTRO_MYSTIC          = 209625, //Boss 03 Intro Mystic Cast - визуально прячется?
    //SPELL_INTRO_EMERGE          = 209641, //Boss 03 Intro Emerge - hit npc 105766
    SPELL_GAIN_ENERGY           = 200086,
    SPELL_PUTRID_SKIES          = 198963,
    SPELL_FRENZY                = 199775,
    SPELL_RANCID_MAW            = 205549,
    SPELL_TOXIC_WRETCH          = 210150,
    SPELL_TOXIC_WRETCH_AT       = 210159,
    SPELL_SPIKED_TONGUE         = 199187,
    SPELL_RAVENOUS              = 199246,

    //Trash
    SPELL_JUMP_VISUAL           = 184483,
    SPELL_FIXATE_PLR            = 209906,
    SPELL_STONE_BOLT            = 209910,
    SPELL_RANCID_MAW_ROOT       = 205609,
};

enum eEvents
{
    EVENT_RANCID_MAW            = 1,
    EVENT_TOXIC_WRETCH          = 2,
    EVENT_SUM_WORMSPEAKER       = 3,
};

Position const speakerPos[2] =
{
    {3045.07f, 1807.39f, -44.13f, 3.54f},
    {3048.89f, 1799.60f, -45.43f, 3.35f}
};

class boss_naraxas : public CreatureScript
{
public:
    boss_naraxas() : CreatureScript("boss_naraxas") {}

    struct boss_naraxasAI : public BossAI
    {
        boss_naraxasAI(Creature* creature) : BossAI(creature, DATA_NARAXAS), summons(me)
        {
            SetCombatMovement(false);
            me->SetMaxPower(POWER_MANA, 100);
        }

        SummonList summons;
        uint8 playerCount, speakerCount;
        uint16 checkMeleeTimer;
        bool berserkActive;

        void Reset()
        {
            summons.DespawnAll();
            events.Reset();
            _Reset();
            me->RemoveAurasDueToSpell(SPELL_GAIN_ENERGY);
            me->RemoveAurasDueToSpell(SPELL_FRENZY);
            me->SetPower(POWER_MANA, 0);
            checkMeleeTimer = 2000;
            berserkActive = false;
            speakerCount = 0;
        }

        void EnterCombat(Unit* /*who*/) //17:03
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_GAIN_ENERGY, true);

            events.ScheduleEvent(EVENT_RANCID_MAW, 8000);    //17:11, 17:29
            events.ScheduleEvent(EVENT_TOXIC_WRETCH, 12000); //17:16
            events.ScheduleEvent(EVENT_SUM_WORMSPEAKER, 7000); //17:10, 17:17, 17:24, 17:30
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (summon->GetEntry() == NPC_WORMSPEAKER_DEVOUT)
                speakerCount++;
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetEntry() == NPC_WORMSPEAKER_DEVOUT)
                speakerCount--;
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(21) && !berserkActive)
            {
                berserkActive = true;
                DoCast(me, SPELL_FRENZY, true);
            }
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_TOXIC_WRETCH)
            {
                Position pos1;
                Position pos2;
                float dist;
                float angle;

                for (uint8 i = 0; i < 6; i++)
                {
                   dist = frand(5, 10);
                   angle = frand(0, M_PI / 6);
                   me->GetNearPosition(pos1, dist, angle);
                   me->GetNearPosition(pos2, dist, -angle);
                   me->CastSpell(pos1.GetPositionX(), pos1.GetPositionY(), pos1.GetPositionZ(), SPELL_TOXIC_WRETCH_AT, true);
                   me->CastSpell(pos2.GetPositionX(), pos2.GetPositionY(), pos2.GetPositionZ(), SPELL_TOXIC_WRETCH_AT, true);
                }
            }
        }

        void SummonWormspeaker(uint8 sumPos)
        {
            playerCount = 0;

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player* player = itr->getSource())
                    if (player->isAlive())
                        playerCount++;

            if (speakerCount < playerCount)
                me->SummonCreature(NPC_WORMSPEAKER_DEVOUT, speakerPos[sumPos]);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (checkMeleeTimer <= diff)
            {
                if (me->getVictim())
                    if (!me->IsWithinMeleeRange(me->getVictim()))
                        DoCast(SPELL_PUTRID_SKIES);

                checkMeleeTimer = 2000;
            }
            else
                checkMeleeTimer -= diff;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RANCID_MAW:
                        DoCast(SPELL_RANCID_MAW);
                        events.ScheduleEvent(EVENT_RANCID_MAW, 18000);
                        break;
                    case EVENT_TOXIC_WRETCH:
                        DoCast(SPELL_TOXIC_WRETCH);
                        events.ScheduleEvent(EVENT_TOXIC_WRETCH, 14000);
                        break;
                    case EVENT_SUM_WORMSPEAKER:
                        //Talk();
                        for (uint8 i = 0; i < 2; i++)
                            SummonWormspeaker(i);
                        events.ScheduleEvent(EVENT_SUM_WORMSPEAKER, 64000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_naraxasAI (creature);
    }
};

//101075
class npc_naraxas_wormspeaker_devout : public CreatureScript
{
public:
    npc_naraxas_wormspeaker_devout() : CreatureScript("npc_naraxas_wormspeaker_devout") {}

    struct npc_naraxas_wormspeaker_devoutAI : public ScriptedAI
    {
        npc_naraxas_wormspeaker_devoutAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_1, 1000);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != EFFECT_MOTION_TYPE)
                return;

            if (id == 0)
            {
                DoZoneInCombat(me, 100.0f);
                events.ScheduleEvent(EVENT_2, 1000);
            }
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_RANCID_MAW_ROOT)
            {
                me->InterruptNonMeleeSpells(false);
                events.ScheduleEvent(EVENT_3, urand(2, 6) * IN_MILLISECONDS);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && me->isInCombat())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                    {
                        DoCast(me, SPELL_JUMP_VISUAL, true);
                        Position pos;
                        float dist = frand(5, 10);
                        me->GetNearPosition(pos, dist, 0.0f);
                        me->GetMotionMaster()->MoveJump(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ() - 10, 15.0f, 15.0f);
                        break;
                    }
                    case EVENT_2:
                        events.ScheduleEvent(EVENT_2, 2000);
                        if (!me->getVictim() || !me->getVictim()->HasAura(SPELL_FIXATE_PLR))
                        {
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true, -SPELL_FIXATE_PLR))
                            {
                                DoStopAttack();
                                AttackStart(pTarget);
                                me->CastSpell(pTarget, SPELL_FIXATE_PLR, true);
                                me->ClearUnitState(UNIT_STATE_CASTING);
                            }
                        }
                        break;
                    case EVENT_3:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(pTarget, SPELL_STONE_BOLT);
                        events.ScheduleEvent(EVENT_3, 8000);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_naraxas_wormspeaker_devoutAI(creature);
    }
};

//200086
class spell_naraxas_gain_energy : public SpellScriptLoader
{
public:
    spell_naraxas_gain_energy() : SpellScriptLoader("spell_naraxas_gain_energy") { }

    class spell_naraxas_gain_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_naraxas_gain_energy_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (caster->GetPower(POWER_MANA) >= 100)
            {
                caster->CastSpell(caster, SPELL_SPIKED_TONGUE);
                caster->CastSpell(caster, SPELL_RAVENOUS, true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_naraxas_gain_energy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_naraxas_gain_energy_AuraScript();
    }
};

void AddSC_boss_naraxas()
{
    new boss_naraxas();
    new npc_naraxas_wormspeaker_devout();
    new spell_naraxas_gain_energy();
}