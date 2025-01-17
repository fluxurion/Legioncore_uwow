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

#include "nexus.h"

enum Spells
{
    //Spells
    SPELL_FROZEN_PRISON                           = 47854,
    SPELL_TAIL_SWEEP                              = 50155,
    SPELL_CRYSTAL_CHAINS                          = 50997,
    SPELL_ENRAGE                                  = 8599,
    SPELL_CRYSTALFIRE_BREATH                      = 48096,
    H_SPELL_CRYSTALFIRE_BREATH                    = 57091,
    SPELL_CRYSTALIZE                              = 48179,
    SPELL_INTENSE_COLD                            = 48094,
    SPELL_INTENSE_COLD_TRIGGERED                  = 48095
};

enum Yells
{
    //Yell
    SAY_AGGRO                                     = -1576040,
    SAY_SLAY                                      = -1576041,
    SAY_ENRAGE                                    = -1576042,
    SAY_DEATH                                     = -1576043,
    SAY_CRYSTAL_NOVA                              = -1576044
};

enum Misc
{
    DATA_INTENSE_COLD                             = 1,
    DATA_CONTAINMENT_SPHERES                      = 3,
};

class boss_keristrasza : public CreatureScript
{
public:
    boss_keristrasza() : CreatureScript("boss_keristrasza") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_keristraszaAI (creature);
    }

    struct boss_keristraszaAI : public ScriptedAI
    {
        boss_keristraszaAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        GuidList intenseColdList;
        ObjectGuid auiContainmentSphereGUIDs[DATA_CONTAINMENT_SPHERES];
        uint32 uiCrystalfireBreathTimer;
        uint32 uiCrystalChainsCrystalizeTimer;
        uint32 uiTailSweepTimer;
        bool intenseCold;
        bool bEnrage;

        void Reset()
        {
            uiCrystalfireBreathTimer = 14*IN_MILLISECONDS;
            uiCrystalChainsCrystalizeTimer = DUNGEON_MODE(30*IN_MILLISECONDS, 11*IN_MILLISECONDS);
            uiTailSweepTimer = 5*IN_MILLISECONDS;
            bEnrage = false;

            intenseCold = true;
            intenseColdList.clear();

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

            RemovePrison(CheckContainmentSpheres());

            if (instance)
                instance->SetData(DATA_KERISTRASZA_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoCastAOE(SPELL_INTENSE_COLD);

            if (instance)
                instance->SetData(DATA_KERISTRASZA_EVENT, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
                instance->SetData(DATA_KERISTRASZA_EVENT, DONE);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            DoScriptText(SAY_SLAY, me);
        }

        bool CheckContainmentSpheres(bool remove_prison = false)
        {
            if (!instance)
                return false;

            auiContainmentSphereGUIDs[0] = instance->GetGuidData(ANOMALUS_CONTAINMET_SPHERE);
            auiContainmentSphereGUIDs[1] = instance->GetGuidData(ORMOROKS_CONTAINMET_SPHERE);
            auiContainmentSphereGUIDs[2] = instance->GetGuidData(TELESTRAS_CONTAINMET_SPHERE);

            GameObject* ContainmentSpheres[DATA_CONTAINMENT_SPHERES];

            for (uint8 i = 0; i < DATA_CONTAINMENT_SPHERES; ++i)
            {
                ContainmentSpheres[i] = instance->instance->GetGameObject(auiContainmentSphereGUIDs[i]);
                if (!ContainmentSpheres[i])
                    return false;
                if (ContainmentSpheres[i]->GetGoState() != GO_STATE_ACTIVE)
                    return false;
            }
            if (remove_prison)
                RemovePrison(true);
            return true;
        }

        void RemovePrison(bool remove)
        {
            if (remove)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (me->HasAura(SPELL_FROZEN_PRISON))
                    me->RemoveAurasDueToSpell(SPELL_FROZEN_PRISON);
            }
            else
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoCast(me, SPELL_FROZEN_PRISON, false);
            }
        }

        void SetGUID(ObjectGuid const& guid, int32 id/* = 0 */)
        {
            if (id == DATA_INTENSE_COLD)
                intenseColdList.push_back(guid);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!bEnrage && HealthBelowPct(25))
            {
                DoScriptText(SAY_ENRAGE, me);
                DoCast(me, SPELL_ENRAGE);
                bEnrage = true;
            }

            if (uiCrystalfireBreathTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CRYSTALFIRE_BREATH);
                uiCrystalfireBreathTimer = 14*IN_MILLISECONDS;
            } else uiCrystalfireBreathTimer -= diff;

            if (uiTailSweepTimer <= diff)
            {
                DoCast(me, SPELL_TAIL_SWEEP);
                uiTailSweepTimer = 5*IN_MILLISECONDS;
            } else uiTailSweepTimer -= diff;

            if (uiCrystalChainsCrystalizeTimer <= diff)
            {
                DoScriptText(SAY_CRYSTAL_NOVA, me);
                if (IsHeroic())
                    DoCast(me, SPELL_CRYSTALIZE);
                else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_CRYSTAL_CHAINS);
                uiCrystalChainsCrystalizeTimer = DUNGEON_MODE(30*IN_MILLISECONDS, 11*IN_MILLISECONDS);
            } else uiCrystalChainsCrystalizeTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

class containment_sphere : public GameObjectScript
{
public:
    containment_sphere() : GameObjectScript("containment_sphere") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go)
    {
        InstanceScript* instance = go->GetInstanceScript();

        Creature* pKeristrasza = Unit::GetCreature(*go, instance ? instance->GetGuidData(DATA_KERISTRASZA) : ObjectGuid::Empty);
        if (pKeristrasza && pKeristrasza->isAlive())
        {
            // maybe these are hacks :(
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            go->SetGoState(GO_STATE_ACTIVE);

            CAST_AI(boss_keristrasza::boss_keristraszaAI, pKeristrasza->AI())->CheckContainmentSpheres(true);
        }
        return true;
    }

};

class spell_intense_cold : public SpellScriptLoader
{
    public:
        spell_intense_cold() : SpellScriptLoader("spell_intense_cold") { }

        class spell_intense_cold_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_intense_cold_AuraScript);

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                if (aurEff->GetBase()->GetStackAmount() < 2)
                    return;
                Unit* caster = GetCaster();
                //TODO: the caster should be boss but not the player
                if (!caster || !caster->GetAI())
                    return;
                caster->GetAI()->SetGUID(GetTarget()->GetGUID(), DATA_INTENSE_COLD);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_intense_cold_AuraScript::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_intense_cold_AuraScript();
        }
};

class achievement_intense_cold : public AchievementCriteriaScript
{
    public:
        achievement_intense_cold() : AchievementCriteriaScript("achievement_intense_cold")
        {
        }

        bool OnCheck(Player* player, Unit* target)
        {
            if (!target)
                return false;

            GuidList intenseColdList = CAST_AI(boss_keristrasza::boss_keristraszaAI, target->ToCreature()->AI())->intenseColdList;
            if (!intenseColdList.empty())
                for (GuidList::iterator itr = intenseColdList.begin(); itr != intenseColdList.end(); ++itr)
                    if (player->GetGUID() == *itr)
                        return false;

            return true;
        }
};

void AddSC_boss_keristrasza()
{
    new boss_keristrasza();
    new containment_sphere();
    new achievement_intense_cold();
    new spell_intense_cold();
}
