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

/*
[1] byte70: False
[1] [0] Waypoint: X: 4436.342 Y: -285.1404 Z: -245.9517
[1] [1] Waypoint: X: 4436.776 Y: -284.2395 Z: -245.9517
[1] [2] Waypoint: X: 4446.447 Y: -295.8958 Z: -235.3393
[1] [3] Waypoint: X: 4456.686 Y: -311.0035 Z: -207.6675
[1] [4] Waypoint: X: 4460.651 Y: -321.5399 Z: -187.7354
[1] [5] Waypoint: X: 4454.374 Y: -328.5417 Z: -165.5679
[1] [6] Waypoint: X: 4447.924 Y: -327.4219 Z: -153.9652
[1] [7] Waypoint: X: 4448.483 Y: -319.5451 Z: -144.8872
[1] [8] Waypoint: X: 4454.757 Y: -318.3906 Z: -127.1212
[1] [9] Waypoint: X: 4455.591 Y: -325.2778 Z: -107.7209
[1] [10] Waypoint: X: 4449.187 Y: -328.9375 Z: -85.20784
[1] [11] Waypoint: X: 4444.792 Y: -325.2083 Z: -66.88081
[1] [12] Waypoint: X: 4446.588 Y: -317.9913 Z: -27.77394
[1] [13] Waypoint: X: 4451.853 Y: -316.2778 Z: -6.795706
[1] [14] Waypoint: X: 4454.854 Y: -320.9028 Z: 38.56396
[1] [15] Waypoint: X: 4454.283 Y: -324.1094 Z: 68.72004
[1] [16] Waypoint: X: 4451.352 Y: -326.1875 Z: 91.97843
[1] [17] Waypoint: X: 4451.143 Y: -327.8229 Z: 136.2271
[1] [18] Waypoint: X: 4451.238 Y: -351.0729 Z: 129.3609
[1] [19] Waypoint: X: 4451.238 Y: -351.0729 Z: 129.3609
[1] RecID: 4372
[1] InitialRawFacing: 1.121713
*/
class npc_q39682 : public CreatureScript
{
public:
    npc_q39682() : CreatureScript("npc_q39682") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q39682AI(creature);
    }

    struct npc_q39682AI : public npc_escortAI
    {
        npc_q39682AI(Creature* creature) : npc_escortAI(creature) {}

        bool PlayerOn;
        void Reset()
        {
            PlayerOn = false;
        }

        void OnCharmed(bool /*apply*/)
        {
        }


        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
        {
            if (!apply || who->GetTypeId() != TYPEID_PLAYER)
                return;

            PlayerOn = true;
            Start(false, true, who->GetGUID());
        }

        void WaypointReached(uint32 i)
        {
            switch (i)
            {
                case 20:    // for NPC_BASTIA_2
                    if (Player* player = GetPlayerForEscort())
                    {
                        
                        player->KilledMonsterCredit(96659);
                        SetEscortPaused(true);
                        player->ExitVehicle();
                    }
                    break;
            }
        }

        void UpdateAI(uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if (PlayerOn)
            {
                if (Player* player = GetPlayerForEscort())
                    player->SetClientControl(me, 0);
                PlayerOn = false;
            }
        }
    };
};

//Q: 39685
class npc_q39685 : public CreatureScript
{
public:
    npc_q39685() : CreatureScript("npc_q39685") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q39685AI(creature);
    }

    enum data
    {
        SPELL_ID = 0,
        SPELL_AURA = 195189,
    };
    struct npc_q39685AI : public ScriptedAI
    {
        npc_q39685AI(Creature* creature) : ScriptedAI(creature) {}

        uint32 cooldown;

        void Reset()
        {
            cooldown = 0;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, 51861);
            me->AddAura(SPELL_AURA, me);
            
        }

        void OnSpellClick(Unit* Clicker)
        {
            if (cooldown)
                return;

            Clicker->ToPlayer()->KilledMonsterCredit(me->GetEntry());
            //Clicker->CastSpell(me, SPELL_ID, false);
                

            cooldown = 5000;
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void EnterCombat(Unit* /*who*/)
        {
            return;
        }

        void UpdateAI(uint32 diff)
        {
            if (cooldown)
            {
                if (cooldown <= diff)
                {
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    cooldown = 0;
                }
                else
                    cooldown -= diff;
            }
        }
    };
};

//! 96682
class npc_q39683 : public CreatureScript
{
public:
    npc_q39683() : CreatureScript("npc_q39683") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q39683AI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q39683AI : public Scripted_NoMovementAI
    {
        EventMap events;
        ObjectGuid playerGuid;
        int8 lowStep;
        npc_q39683AI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            lowStep = 1;
        }

        enum data
        {
            REWARD_SPELL = 210486,
            REWARD_SPELL2 = 195440,
            CREDIT = 106254,            

            EVENT_1 = 1,
            EVENT_2,
            EVENT_3,
            EVENT_4,
            EVENT_5,
            EVENT_6,
        };

        void EnterCombat(Unit* victim) override
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0, victim->GetGUID());

            Player *player = victim->ToPlayer();
            if (!player)
                return;

            lowStep = 1;

            float health_pct = 100.0f;
            if (player->GetQuestStatus(39684) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(39684) ==  QUEST_STATUS_REWARDED)
            {
                ++lowStep;
                health_pct -= 40.0f;
            }
            if (player->GetQuestStatus(39685) == QUEST_STATUS_COMPLETE || player->GetQuestStatus(39685) == QUEST_STATUS_COMPLETE)
            {
                ++lowStep;
                health_pct -= 40.0f;
            }

            me->SetHealth(float(me->GetMaxHealth() / 100.0f)*health_pct);
        }

        void DoAction(int32 const /*param*/)
        {

        }

        //! HACK!!! ANTIL FINISH EVENT
        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            damage *= lowStep;
        }

        void JustDied(Unit* killer)
        {
            Player *player = killer->ToPlayer();
            if (!player)
                return;

            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_3, player->GetGUID());
            player->CastSpell(player, REWARD_SPELL, true);
            player->CastSpell(player, REWARD_SPELL2, true);
            player->KilledMonsterCredit(CREDIT);
        }
        void UpdateAI(uint32 diff)
        {
            UpdateVictim();

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {

            }
            DoMeleeAttackIfReady();
        }
    };
};

//! 96675
class npc_dh_questgiver_96675 : public CreatureScript
{
public:
    npc_dh_questgiver_96675() : CreatureScript("npc_dh_questgiver_96675") {}
    enum data
    {
        QUEST_03 = 39686,
        GO_ELEVATOR = 244644,
        CREDIT = 96814,
    };
    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_03) // La lecon du parchemin brulant
        {
            creature->GetMap()->LoadGrid(4450.857f, -451.7679f); //voice

            std::list<GameObject*> lList;
            creature->GetGameObjectListWithEntryInGrid(lList, GO_ELEVATOR, 500.0f);
            if (lList.empty())
            {
                creature->MonsterSay("SCRIPT::npc_dh_questgiver_96675 no elevator", LANG_UNIVERSAL, player->GetGUID());
                return true;
            }
            //creature->MonsterSay("SCRIPT::npc_dh_questgiver_96675 OK", LANG_UNIVERSAL, player->GetGUID());

            GameObject* elevator = *lList.begin();
            player->AddToExtraLook(elevator);

            UpdateData transData(player->GetMapId());
            elevator->BuildCreateUpdateBlockForPlayer(&transData, player);
            player->KilledMonsterCredit(CREDIT);
        }

        return true;
    }
};

void AddSC_warden_prison()
{
    new go_q38690();
    new npc_q38689();
    new spell_q38723();
    new npc_q38723();
    new npc_q39682();
    new npc_q39685();
    new npc_q39683();
    new npc_dh_questgiver_96675();
}