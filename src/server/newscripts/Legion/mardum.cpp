#include "NewScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "CreatureTextMgr.h"
#include "Battleground.h"
#include "MapManager.h"
#include "ScriptMgr.h"

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

void AddSC_Mardum()
{
    new sceneTrigger_dh_init();
}