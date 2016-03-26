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

void AddSC_Mardum()
{
    new sceneTrigger_dh_init();
    new go_q40077();
}