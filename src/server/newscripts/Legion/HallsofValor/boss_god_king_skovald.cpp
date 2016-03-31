/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
    Encounter: God King Skovald
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_ZERO_REGEN            = 78725,
    SPELL_CONVERSATION          = 193975, //Skovald Intro 

    SPELL_FELBLAZE_RUSH         = 193658,
    SPELL_RAGNAROK              = 193826,
    
    //193783, 194214, 193765
    //Odyn
    SPELL_AEGIS_SPAWN           = 193940,
    
    //Aegis
    SPELL_AEGIS_VISUAL_SHIELD   = 193769,
    SPELL_AEGIS_OVERRIDE        = 193783,
};

enum eEvents
{
    EVENT_INTRO_COMPLETE        = 1,
    EVENT_FELBLAZE_RUSH         = 2,
    EVENT_RAGNAROK              = 3,
    //EVENT_    = 4,
    //EVENT_    = 5,
};

Position const kingsPos[4] =
{
    {2387.87f, 506.50f, 749.30f, 0.57f}, //NPC_KING_HALDOR  95843
    {2388.01f, 551.16f, 749.30f, 5.67f}, //NPC_KING_BJORN   97081
    {2409.10f, 490.69f, 749.30f, 1.16f}, //NPC_KING_RANULF  97083
    {2408.56f, 568.34f, 749.30f, 5.11f}, //NPC_KING_TOR     97084
};

uint32 OdynKings[4] =
{
    NPC_KING_HALDOR,
    NPC_KING_BJORN,
    NPC_KING_RANULF,
    NPC_KING_TOR
};

class boss_god_king_skovald : public CreatureScript
{
public:
    boss_god_king_skovald() : CreatureScript("boss_god_king_skovald") {}

    struct boss_god_king_skovaldAI : public BossAI
    {
        boss_god_king_skovaldAI(Creature* creature) : BossAI(creature, DATA_SKOVALD) 
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            me->SetReactState(REACT_PASSIVE);
            me->SetVisible(false);
            SummonOdynKings();
            intro = true;
            kingDiedCount = 0;
        }

        bool intro;
        uint8 kingDiedCount;
        uint8 power, powerCounter;
        uint16 powerTimer;

        void Reset()
        {
            events.Reset();
            _Reset();

            DoCast(me, SPELL_ZERO_REGEN, true);
            me->SetPower(POWER_ENERGY, 80);
            powerTimer = 1000;
            if (!intro)
                if (Creature* odyn = instance->instance->GetCreature(instance->GetGuidData(DATA_ODYN)))
                    odyn->CastSpell(odyn, SPELL_AEGIS_SPAWN, true, nullptr, 0, me->GetGUID());
        }

        void EnterCombat(Unit* /*who*/) //57:56
        {
            //Talk(SAY_AGGRO); //Give up the aegis or die!
            _EnterCombat();
            events.ScheduleEvent(EVENT_FELBLAZE_RUSH, 7000); //58:03, 58:39, 58:50, 59:02, 59:23
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

        void SummonOdynKings()
        {
            for (uint8 i = 0; i < 4; i++)
                me->SummonCreature(OdynKings[i], kingsPos[i]);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (!intro)
                return;

            switch (summon->GetEntry())
            {
                case NPC_KING_HALDOR:
                case NPC_KING_BJORN:
                case NPC_KING_RANULF:
                case NPC_KING_TOR:
                    kingDiedCount++;
                    break;
            }

            if (kingDiedCount == 4)
            {
                me->SetVisible(true);
                me->SetPower(POWER_ENERGY, 80);
                me->GetMotionMaster()->MovePoint(1, 2411.03f, 528.72f, 748.99f);
                if (Creature* odyn = instance->instance->GetCreature(instance->GetGuidData(DATA_ODYN)))
                {
                    //odyn->AI()->Talk();
                    odyn->CastSpell(odyn, SPELL_AEGIS_SPAWN, true, nullptr, 0, me->GetGUID());
                }
                DoCast(me, SPELL_CONVERSATION, true);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
            {
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                events.ScheduleEvent(EVENT_INTRO_COMPLETE, 24000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && !intro)
                return;

            events.Update(diff);

            if (me->isInCombat())
            {
                if (powerTimer <= diff)
                {
                    power = me->GetPower(POWER_ENERGY);
                    if (power < 100)
                    {
                        if (powerCounter < 2)
                        {
                            powerCounter++;
                            me->SetPower(POWER_ENERGY, power + 2);
                        }
                        else
                        {
                            powerCounter = 0;
                            me->SetPower(POWER_ENERGY, power + 1);
                        }
                    }
                    else
                    {
                        events.ScheduleEvent(EVENT_RAGNAROK, 500);
                    }

                    powerTimer = 1000;
                }
                else powerTimer -=diff;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_INTRO_COMPLETE:
                        intro = false;
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                    case EVENT_FELBLAZE_RUSH:
                        DoCast(SPELL_FELBLAZE_RUSH);
                        events.ScheduleEvent(EVENT_FELBLAZE_RUSH, urand(10, 26) * IN_MILLISECONDS);
                        break;
                    case EVENT_RAGNAROK:
                        DoCast(SPELL_RAGNAROK);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_god_king_skovaldAI (creature);
    }
};

//95843, 97081, 97083, 97084
class npc_generic_odyn_kings : public CreatureScript
{
public:
    npc_generic_odyn_kings() : CreatureScript("npc_generic_odyn_kings") {}

    struct npc_generic_odyn_kingsAI : public ScriptedAI
    {
        npc_generic_odyn_kingsAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            //events.ScheduleEvent(EVENT_1, );
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
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_generic_odyn_kingsAI(creature);
    }
};

//98364
class npc_skovald_aegis_of_aggramar : public CreatureScript
{
public:
    npc_skovald_aegis_of_aggramar() : CreatureScript("npc_skovald_aegis_of_aggramar") {}

    struct npc_skovald_aegis_of_aggramarAI : public ScriptedAI
    {
        npc_skovald_aegis_of_aggramarAI(Creature* creature) : ScriptedAI(creature) {}

        bool click;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            click = false;
            DoCast(me, SPELL_AEGIS_VISUAL_SHIELD, true);
        }

        void OnSpellClick(Unit* clicker)
        {
            if (!click)
            {
                click = true;
                clicker->CastSpell(clicker, SPELL_AEGIS_OVERRIDE, true);
                me->DespawnOrUnsummon();
            }
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_skovald_aegis_of_aggramarAI(creature);
    }
};

//193826
class spell_skovald_ragnarok : public SpellScriptLoader
{
    public:
        spell_skovald_ragnarok() : SpellScriptLoader("spell_skovald_ragnarok") { }

        class spell_skovald_ragnarok_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_skovald_ragnarok_AuraScript);

            uint16 m_checkTimer;

            bool Load()
            {
                m_checkTimer = 1500;
                return true;
            }

            void OnUpdate(uint32 diff, AuraEffect* /*aurEff*/)
            {
                if (m_checkTimer <= diff)
                    m_checkTimer = 1500;
                else
                {
                    m_checkTimer -= diff;
                    return;
                }

                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, 193827, true);
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_skovald_ragnarok_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_skovald_ragnarok_AuraScript();
        }
};

void AddSC_boss_god_king_skovald()
{
    new boss_god_king_skovald();
    new npc_generic_odyn_kings();
    new npc_skovald_aegis_of_aggramar();
    new spell_skovald_ragnarok();
}