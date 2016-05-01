#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "Battleground.h"
#include "MapManager.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"

//245728
const uint32 q38690[8] = {97614, 97622, 97618, 97616, 97615, 97620, 97617, 97619};

class go_q38690 : public GameObjectScript
{
public:
    go_q38690() : GameObjectScript("go_q38690") { }

    struct go_q38690AI : public GameObjectAI
    {
        go_q38690AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38690,
        };

        bool GossipHello(Player* player) override
        {
            for (int32 i = 0; i < sizeof(q38690); ++i)
            {
                if (!player->GetReqKillOrCastCurrentCount(QUEST, q38690[i]))
                {
                    player->KilledMonsterCredit(q38690[i]);
                    return true;
                }
            }

            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q38690AI(go);
    }
};

//! 92782 92776
class npc_q38689 : public CreatureScript
{
public:
    npc_q38689() : CreatureScript("npc_q38689") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q38689AI(creature);
    }

    struct npc_q38689AI : public ScriptedAI
    {

        npc_q38689AI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            QUEST = 38689,
            SPELL_CREDIT = 133511,
            SPELL_CREDIT2 = 133509,
        };

        void Reset()
        {
            if (me->getVictim() || me->IsInEvadeMode())
                return;

            std::list<Unit*> list;
            me->GetAttackableUnitListInRange(list, 70.0f);
            for (auto enemy : list)
            {
                if (enemy->ToPlayer())
                    continue;
                me->AI()->AttackStart(enemy);
                break;
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) override {};
        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            damage /= 10;
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            damage = 10000;
        }

        void JustDied(Unit* /*killer*/)
        {
            std::list<Player*> lList;
            me->GetPlayerListInGrid(lList, 200.0f);
            for (auto player : lList)
            {
                if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
                {
                    player->CastSpell(player, SPELL_CREDIT, true);
                    player->CastSpell(player, SPELL_CREDIT2, true);
                }
            }
        }

    };
};

//! 187864
class spell_q38723 : public SpellScriptLoader
{
public:
    spell_q38723() : SpellScriptLoader("spell_q38723") { }

    class spell_q38723_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_q38723_AuraScript);


        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();

            if (!target || !target->ToPlayer())
                return;
            
            target->ToPlayer()->TeleportTo(1468, 4084.27f, -298.11f, -282.07f, 3.118031f);
            target->ToPlayer()->KilledMonsterCredit(99303);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_q38723_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ACTIVATE_SCENE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_q38723_AuraScript();
    }
};

//! 92990, 97632
class npc_q38723 : public CreatureScript
{
public:
    npc_q38723() : CreatureScript("npc_q38723") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q38723AI(creature);
    }

    struct npc_q38723AI : public ScriptedAI
    {

        npc_q38723AI(Creature* creature) : ScriptedAI(creature)
        {
            creature->SetCorpseDelay(30);
            creature->SetRespawnDelay(15);
        }

        enum data
        {
            QUEST = 38723,
            SPELL_CREDIT = 210461,
            SPELL_CREDIT2 = 195327,
        };

        void Reset()
        {
            if (me->getVictim() || me->IsInEvadeMode())
                return;

            std::list<Unit*> list;
            me->GetAttackableUnitListInRange(list, 70.0f);
            for (auto enemy : list)
            {
                if (enemy->ToPlayer())
                    continue;
                me->AI()->AttackStart(enemy);
                break;
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) override {};
        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType /*damageType*/) override
        {
            damage /= 10;
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            damage = 10000;
        }

        void JustDied(Unit* /*killer*/)
        {
            std::list<Player*> lList;
            me->GetPlayerListInGrid(lList, 200.0f);
            for (auto player : lList)
            {
                if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE)
                {
                    me->CastSpell(player, SPELL_CREDIT, true);
                    player->KilledMonsterCredit(106241);

                    
                    player->CastSpell(player, SPELL_CREDIT2, true);
                }
            }
        }

    };
};
void AddSC_warden_prison()
{
    new go_q38690();
    new npc_q38689();
    new spell_q38723();
    new npc_q38723();
}