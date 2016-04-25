/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
    Encounter: Wrath of Azshara
    TODO: Не скриптована боевая часть Наг.
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
    //Pre-event spells
    SPELL_SURGING_WATERS        = 192632,
    SPELL_TIDAL_WAVE            = 192940,
    SPELL_DAMAGE_SELF_20PCT     = 193500,
    SPELL_TEMPEST_ATTUNEMENT    = 193491,
    SPELL_STORM_CONDUIT         = 193196,

    SPELL_MYSTIC_TORNADO        = 192680,
    SPELL_MASSIVE_DELUGE        = 192620,
    SPELL_ARCANE_BOMB           = 192705,
    SPELL_ARCANE_BOMB_VEH_SEAT  = 192706,
    SPELL_ARCANE_BOMB_VISUAL    = 192711,
    SPELL_ARCANE_BOMB_DMG       = 192708,
    SPELL_CRY_OF_WRATH          = 192985,
    SPELL_RAGING_STORMS         = 192696,

    SPELL_MYSTIC_TORNADO_AT     = 192673,
    SPELL_LIGHTNING_STRIKES_1   = 192989, //2s tick
    SPELL_LIGHTNING_STRIKES_2   = 192990, //7s tick
};

enum eEvents
{
    EVENT_MYSTIC_TORNADO        = 1,
    EVENT_MASSIVE_DELUGE        = 2,
    EVENT_ARCANE_BOMB           = 3,
    EVENT_CRY_OF_WRATH          = 4,
    EVENT_RAGING_STORMS         = 5,
};

Position const nagPos[4] =
{
    {-3511.70f, 4479.83f, 1.61f, 5.00f},
    {-3474.94f, 4281.46f, 1.89f, 1.65f},
    {-3382.71f, 4354.07f, 1.39f, 2.83f},
    {-3415.23f, 4442.80f, 1.09f, 3.79f},
};

class boss_wrath_of_azshara : public CreatureScript
{
public:
    boss_wrath_of_azshara() : CreatureScript("boss_wrath_of_azshara") {}

    struct boss_wrath_of_azsharaAI : public BossAI
    {
        boss_wrath_of_azsharaAI(Creature* creature) : BossAI(creature, DATA_WRATH_OF_AZSHARA) 
        {
            SetCombatMovement(false);
            me->setRegeneratingHealth(false);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
            SummonNagas();
            nagaDiedCount = 0;
        }

        uint8 nagaDiedCount;
        uint16 checkVictimTimer;
        bool lowHP;

        void Reset()
        {
            _Reset();
            lowHP = false;
            checkVictimTimer = 2000;

            if (!me->HasAura(SPELL_SURGING_WATERS))
                DoCast(me, SPELL_SURGING_WATERS, true);

            if (nagaDiedCount == 4)
                me->SetHealth(me->CountPctFromMaxHealth(21));
        }

        void EnterCombat(Unit* /*who*/) //57:06
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_TIDAL_WAVE, true);

            events.ScheduleEvent(EVENT_MYSTIC_TORNADO, 9000); //57:15, 57:40
            events.ScheduleEvent(EVENT_MASSIVE_DELUGE, 12000); //57:18, 58:09
            events.ScheduleEvent(EVENT_ARCANE_BOMB, 23000); //57:29, 57:59
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(11) && !lowHP)
            {
                lowHP = true;
                events.ScheduleEvent(EVENT_CRY_OF_WRATH, 0);
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ARCANE_BOMB_VEH_SEAT)
                me->SummonCreature(NPC_ARCANE_BOMB, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 17000);
        }

        void SummonNagas()
        {
            me->SummonCreature(NPC_MYSTIC_SSAVEH, nagPos[0]);
            me->SummonCreature(NPC_RITUALIST_LESHA, nagPos[1]);
            me->SummonCreature(NPC_CHANNELER_VARISZ, nagPos[2]);
            me->SummonCreature(NPC_BINDER_ASHIOI, nagPos[3]);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            switch (summon->GetEntry())
            {
                case NPC_MYSTIC_SSAVEH:
                case NPC_RITUALIST_LESHA:
                case NPC_CHANNELER_VARISZ:
                case NPC_BINDER_ASHIOI:
                    nagaDiedCount++;
                    DoCast(me, SPELL_DAMAGE_SELF_20PCT, true);
                    break;
            }
            if (nagaDiedCount == 4)
            {
                me->RemoveAurasDueToSpell(SPELL_STORM_CONDUIT);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
                me->SetReactState(REACT_DEFENSIVE);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (checkVictimTimer <= diff)
            {
                if (me->getVictim())
                    if (!me->IsWithinMeleeRange(me->getVictim()))
                        events.ScheduleEvent(EVENT_RAGING_STORMS, 500);

                checkVictimTimer = 2000;
            }
            else checkVictimTimer -= diff;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MYSTIC_TORNADO:
                        DoCast(me, SPELL_MYSTIC_TORNADO, true);
                        events.ScheduleEvent(EVENT_MYSTIC_TORNADO, 25000);
                        break;
                    case EVENT_MASSIVE_DELUGE:
                        DoCast(SPELL_MASSIVE_DELUGE);
                        events.ScheduleEvent(EVENT_MASSIVE_DELUGE, 48000);
                        break;
                    case EVENT_ARCANE_BOMB:
                        DoCast(SPELL_ARCANE_BOMB);
                        events.ScheduleEvent(EVENT_ARCANE_BOMB, 30000);
                        break;
                    case EVENT_CRY_OF_WRATH:
                        DoCast(SPELL_CRY_OF_WRATH);
                        break;
                    case EVENT_RAGING_STORMS:
                        DoCast(SPELL_RAGING_STORMS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_wrath_of_azsharaAI (creature);
    }
};

//98173, 100248, 100249, 100250
class npc_wrath_of_azshara_nagas : public CreatureScript
{
public:
    npc_wrath_of_azshara_nagas() : CreatureScript("npc_wrath_of_azshara_nagas") {}

    struct npc_wrath_of_azshara_nagasAI : public ScriptedAI
    {
        npc_wrath_of_azshara_nagasAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;
        bool activateConduit;

        void Reset()
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner)
        {
            activateConduit = false;
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
            DoCast(me, SPELL_TEMPEST_ATTUNEMENT, true);
        }

        void DoAction(int32 const action)
        {
            if (activateConduit)
                return;

            activateConduit = true;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
            me->SetReactState(REACT_AGGRESSIVE);
            me->InterruptNonMeleeSpells(false, SPELL_TEMPEST_ATTUNEMENT);
            DoCast(me, SPELL_STORM_CONDUIT, true);
        }

        void JustReachedHome()
        {
            DoCast(me, SPELL_STORM_CONDUIT, true);
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
                        //events.ScheduleEvent(EVENT_1, );
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wrath_of_azshara_nagasAI(creature);
    }
};

//97673
class npc_wrath_of_azshara_mystic_tornado : public CreatureScript
{
public:
    npc_wrath_of_azshara_mystic_tornado() : CreatureScript("npc_wrath_of_azshara_mystic_tornado") {}

    struct npc_wrath_of_azshara_mystic_tornadoAI : public ScriptedAI
    {
        npc_wrath_of_azshara_mystic_tornadoAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        uint16 randMoveTimer;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            if (!summoner->isInCombat())
            {
                me->DespawnOrUnsummon();
                return;
            }

            DoCast(me, SPELL_MYSTIC_TORNADO_AT, true);
            randMoveTimer = 10000;
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            if (randMoveTimer <= diff)
            {
                randMoveTimer = 10000;
                float angle = (float)rand_norm() * static_cast<float>(2 * M_PI);
                float distance = 15 * (float)rand_norm();
                float x = me->GetHomePosition().GetPositionX() + distance * std::cos(angle);
                float y = me->GetHomePosition().GetPositionY() + distance * std::sin(angle);
                float z = me->GetHomePosition().GetPositionZ();
                Trinity::NormalizeMapCoord(x);
                Trinity::NormalizeMapCoord(y);

                me->GetMotionMaster()->MovePoint(1, x, y, z);
            }
            else randMoveTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wrath_of_azshara_mystic_tornadoAI(creature);
    }
};

//97691
class npc_wrath_of_azshara_arcane_bomb : public CreatureScript
{
public:
    npc_wrath_of_azshara_arcane_bomb() : CreatureScript("npc_wrath_of_azshara_arcane_bomb") {}

    struct npc_wrath_of_azshara_arcane_bombAI : public ScriptedAI
    {
        npc_wrath_of_azshara_arcane_bombAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            if (!summoner->isInCombat())
            {
                me->DespawnOrUnsummon();
                return;
            }
            //TODO: Нужно переделать! По идее моб должен садиться на игрока, а визуалка шара, видимо, двигатся за ними.
            DoCast(me, SPELL_ARCANE_BOMB_VISUAL, true);
            DoCast(SPELL_ARCANE_BOMB_DMG);
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wrath_of_azshara_arcane_bombAI(creature);
    }
};

void AddSC_boss_wrath_of_azshara()
{
    new boss_wrath_of_azshara();
    new npc_wrath_of_azshara_nagas();
    new npc_wrath_of_azshara_mystic_tornado();
}