/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gate_setting_sun.h"

enum eSpells
{
    // Commander Rimok
    SPELL_VISCOUS_FLUID_SUMMON      = 107078,
    SPELL_VISCOUS_FLUID_DMG_UP      = 107091,
    SPELL_VISCOUS_FLUID_DMG_DOWN    = 107122,
    SPELL_FRENZIED_ASSAULT          = 107120,

    // Add Generator
    SPELL_PERIODIC_SPAWN_SWARMER    = 115052,
    SPELL_PERIODIC_SPAWN_SABOTEUR   = 116621,

    // Saboteur
    SPELL_BOMBARD                   = 120559
};

enum eEvents
{
    EVENT_FRENZIED_ASSAULT  = 1,
    EVENT_VISCOUS_FLUID     = 2
};

class boss_commander_rimok : public CreatureScript
{
    public:
        boss_commander_rimok() : CreatureScript("boss_commander_rimok") {}

        struct boss_commander_rimokAI : public BossAI
        {
            boss_commander_rimokAI(Creature* creature) : BossAI(creature, DATA_RIMOK)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;

            void Reset()
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                events.ScheduleEvent(EVENT_FRENZIED_ASSAULT, urand(5000, 10000));
                events.ScheduleEvent(EVENT_VISCOUS_FLUID,    urand(10000, 15000));
            }

            void JustSummoned(Creature* summoned)
            {
                summons.Summon(summoned);
            }

            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch(events.ExecuteEvent())
                {
                    case EVENT_FRENZIED_ASSAULT:
                        if (me->getVictim())
                            me->CastSpell(me->getVictim(), SPELL_FRENZIED_ASSAULT, false);
                        events.ScheduleEvent(EVENT_FRENZIED_ASSAULT, urand(10000, 15000));
                        break;
                    case EVENT_VISCOUS_FLUID:
                        Position pos;
                        me->GetPosition(&pos);
                        me->SummonCreature(56883, pos, TEMPSUMMON_TIMED_DESPAWN, 30000);
                        events.ScheduleEvent(EVENT_VISCOUS_FLUID, urand(5000, 10000));
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_commander_rimokAI(creature);
        }
};

class npc_krikthik_swarmer : public CreatureScript
{
    public:
        npc_krikthik_swarmer() : CreatureScript("npc_krikthik_swarmer") {}

        struct npc_krikthik_swarmerAI : public ScriptedAI
        {
            npc_krikthik_swarmerAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 attackTimer;

            void Reset()
            {
                attackTimer = 2000;
            }

            void UpdateAI(uint32 diff)
            {
                if (attackTimer)
                {
                    if (attackTimer <= diff)
                    {
                        DoZoneInCombat();
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);

                        attackTimer = 0;
                    }
                    else attackTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_krikthik_swarmerAI(creature);
        }
};

class npc_krikthik_saboteur : public CreatureScript
{
    public:
        npc_krikthik_saboteur() : CreatureScript("npc_krikthik_saboteur") {}

        struct npc_krikthik_saboteurAI : public ScriptedAI
        {
            npc_krikthik_saboteurAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 attackTimer;
            uint32 checkTimer;

            void Reset()
            {
                attackTimer = 2000;
                checkTimer = urand(17500, 22500);
            }

            void UpdateAI(uint32 diff)
            {
                if (attackTimer)
                {
                    if (attackTimer <= diff)
                    {
                        DoZoneInCombat();
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);

                        attackTimer = 0;
                    }
                    else attackTimer -= diff;
                }

                if (checkTimer <= diff)
                {
                    me->CastSpell(me, SPELL_BOMBARD, false);
                    checkTimer = urand(7500, 12500);
                }
                else checkTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_krikthik_saboteurAI(creature);
        }
};

class npc_add_generator : public CreatureScript
{
    public:
        npc_add_generator() : CreatureScript("npc_add_generator") {}

        struct npc_add_generatorAI : public ScriptedAI
        {
            npc_add_generatorAI(Creature* creature) : ScriptedAI(creature), summons(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            SummonList summons;

            void Reset()
            {
                me->RemoveAurasDueToSpell(SPELL_PERIODIC_SPAWN_SWARMER);
                me->RemoveAurasDueToSpell(SPELL_PERIODIC_SPAWN_SABOTEUR);
                summons.DespawnAll();
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case NOT_STARTED:
                    case FAIL:
                        summons.DespawnAll();
                        // no break
                    case DONE:
                        me->RemoveAurasDueToSpell(SPELL_PERIODIC_SPAWN_SWARMER);
                        me->RemoveAurasDueToSpell(SPELL_PERIODIC_SPAWN_SABOTEUR);
                        break;
                    case SPECIAL: // Only one must spawn saboteurs
                        me->CastSpell(me, SPELL_PERIODIC_SPAWN_SABOTEUR, true);
                        break;
                    case IN_PROGRESS:
                        me->CastSpell(me, SPELL_PERIODIC_SPAWN_SWARMER, true);
                        break;
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* summoned)
            {
                summons.Summon(summoned);

                float x = me->GetPositionX();
                float y = me->GetPositionY() - 10;
                float z = me->GetMap()->GetHeight(x, y, 400.0f);
                summoned->GetMotionMaster()->MoveJump(x, y, z, 10, 20);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_add_generatorAI(creature);
        }
};

class npc_viscous_fluid : public CreatureScript
{
    public:
        npc_viscous_fluid() : CreatureScript("npc_viscous_fluid") {}

        struct npc_viscous_fluidAI : public ScriptedAI
        {
            npc_viscous_fluidAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                me->SetReactState(REACT_PASSIVE);
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            }

            InstanceScript* instance;

            void Reset()
            {
                DoCast(me, 107092); //Dummy
                DoCast(me, 107088); 
            }

            void EnterCombat(Unit* who){}

            void EnterEvadeMode(){}

            void UpdateAI(uint32 diff){}
            
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_viscous_fluidAI(creature);
        }
};

class spell_rimok_saboteur_bombard : public SpellScriptLoader
{
    public:
        spell_rimok_saboteur_bombard() :  SpellScriptLoader("spell_rimok_saboteur_bombard") { }

        class spell_rimok_saboteur_bombard_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rimok_saboteur_bombard_AuraScript);

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();

                if (Unit* caster = GetCaster())
                {
                    if (InstanceScript* instance = caster->GetInstanceScript())
                    {
                        Map::PlayerList const &PlayerList = instance->instance->GetPlayers();

                        if (PlayerList.isEmpty())
                            return;

                        Map::PlayerList::const_iterator it = PlayerList.begin();
                        for (uint8 i = 0; i < urand(0, PlayerList.getSize() - 1); ++i, ++it);

                        if (it == PlayerList.end())
                            return;

                        if (Player* player = it->getSource())
                            caster->CastSpell(player, GetSpellInfo()->Effects[0].TriggerSpell, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rimok_saboteur_bombard_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rimok_saboteur_bombard_AuraScript();
        }
};

void AddSC_boss_commander_rimok()
{
    new boss_commander_rimok();
    new npc_krikthik_swarmer();
    new npc_krikthik_saboteur();
    new npc_add_generator();
    new npc_viscous_fluid();
    new spell_rimok_saboteur_bombard();
}
