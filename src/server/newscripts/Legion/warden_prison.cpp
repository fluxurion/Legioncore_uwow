#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "Battleground.h"
#include "MapManager.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"

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
            if (victim->ToPlayer())
                damage /= 10;
            else
                damage = 0;
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            damage *= 5;
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
            me->Respawn(false, 5000);
        }
    };
};

void AddSC_warden_prison()
{
    new npc_q38689();
}