#include "NewScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "Battleground.h"
#include "MapManager.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"

class sceneTrigger_dh_init : public SceneTriggerScript
{
public:
    sceneTrigger_dh_init() : SceneTriggerScript("sceneTrigger_dh_init")
    {}

    enum data
    {
        NPC_CONV = 705,
        SPELL_CUEILLIDANTH = 191667,
    };

    bool OnTrigger(Player* player, SpellScene const* trigger, std::string type) override
    {
        if (type == "CUEILLIDANTH")
        {
            //player->CastSpell(player, CUEILLIDANTH, false);
            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
            {
                delete conversation;
            }

        }
        return true;
    }
};

// 244898
class go_q40077 : public GameObjectScript
{
public:
    go_q40077() : GameObjectScript("go_q40077") { }

    struct go_q40077AI : public GameObjectAI
    {
        go_q40077AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SCENE = 191677,
        };

        bool GossipHello(Player* player) override
        {
            /*

            ServerToClient: SMSG_PLAY_SCENE (0x2651) Length: 34 ConnIdx: 0 Time: 02/22/2016 12:57:13.116 Number: 11210
            SceneID: 1116
            PlaybackFlags: 27
            SceneInstanceID: 2
            SceneScriptPackageID: 1493
            TransportGUID: Full: 0x0
            Pos: X: 1002.885 Y: 2956.695 Z: -10.55672
            Facing: 4.76053
            */
            player->CastSpell(player, SCENE, true);
            player->KillCreditGO(go->GetEntry(), go->GetGUID());
            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q40077AI(go);
    }
};

// 241751
class go_q40378 : public GameObjectScript
{
public:
    go_q40378() : GameObjectScript("go_q40378") { }

    struct go_q40378AI : public GameObjectAI
    {
        go_q40378AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            SCENE = 189261,
        };

        bool GossipHello(Player* player) override
        {
            /*
            ClientToServer: CMSG_GAME_OBJ_REPORT_USE (0x34DE) Length: 15 ConnIdx: 2 Time: 02/06/2016 22:39:25.012 Number: 16325
            GameObjectGUID: Full: 0x2C2090B920EEB5C00000100001364D15; HighType: GameObject; Low: 20335893; Map: 1481; Entry: 244439;
            */
            player->Dismount();
            player->CastSpell(player, SCENE, true);
            player->KilledMonsterCredit(88872);
            return true;
        }
 
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q40378AI(go);
    }
};


/*
before OBJECT_FIELD_DYNAMIC_FLAGS: 4294902018
after OBJECT_FIELD_DYNAMIC_FLAGS: 2473853184

*/

//244439, 244440, 244441, 243873
class go_q39279 : public GameObjectScript
{
public:
    go_q39279() : GameObjectScript("go_q39279") { }

    struct go_q39279AI : public GameObjectAI
    {
        go_q39279AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 39279,
            SCENE = 189261,
            GO_244439 = 244439,
            GO_244440 = 244440,
            GO_244441 = 244441,
            GO_243873 = 243873,
            NPC_CONV_GO_244439 = 558,
            NPC_CONV_GO_244440 = 583,
        };

        bool GossipUse(Player* player)
        { 
            if (player->GetReqKillOrCastCurrentCount(QUEST, go->GetEntry()))
                return true;
            return false;
        }
        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, go->GetEntry()))
                return true;

            switch (go->GetEntry())
            {
                case GO_244439:
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV_GO_244439, player, NULL, *player))
                        delete conversation;
                    break;
                }
                case GO_244440:
                {
                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV_GO_244440, player, NULL, *player))
                        delete conversation;
                    break;
                }
                case GO_244441:
                case GO_243873:
                    break;
            }

            return false;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q39279AI(go);
    }
};

/*
ClientToServer: CMSG_SPELL_CLICK (0x348C) Length: 16 ConnIdx: 2 Time: 02/06/2016 22:40:30.961 Number: 17163
SpellClickUnitGUID: Full: 0x202090B9205EDD800000100002B64D15; HighType: Creature; Low: 45501717; Map: 1481; Entry: 97142;
TryAutoDismount: True

conv - 581
*/

// Destroying Fel Spreader - 191827
class spell_legion_q39279 : public SpellScriptLoader
{
public:
    spell_legion_q39279() : SpellScriptLoader("spell_legion_q39279") { }

    class spell_legion_q39279_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_legion_q39279_SpellScript);

        enum data
        {
            QUEST = 39279,
            NPC_CONV = 581
        };

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(EFFECT_0);
            if (Unit* caster = GetCaster())
            {
                Player *player = caster->ToPlayer();
                if (!player)
                    return;

                if (Unit * target = GetHitUnit())
                {
                    target->AddToHideList(caster->GetGUID());
                    target->DestroyForPlayer(player, false);
                    player->KilledMonsterCredit(target->GetEntry());

                    Conversation* conversation = new Conversation;
                    if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
                        delete conversation;
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_legion_q39279_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_legion_q39279_SpellScript();
    }
};
void AddSC_Mardum()
{
    new sceneTrigger_dh_init();
    new go_q40077();
    new go_q40378();
    new go_q39279();
    new spell_legion_q39279();
}