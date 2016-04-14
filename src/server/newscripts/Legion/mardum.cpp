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
                delete conversation;
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

//93011
class npc_q40077 : public CreatureScript
{
public:
    npc_q40077() : CreatureScript("npc_q40077") { }


    enum data
    {
        QUEST    = 40077,
        NPC_CONV = 922,
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST)
        {
            //ToDo: how load npc for converstation?
            player->GetMap()->LoadGrid(1170.74f, 3204.71f); //voice

            Conversation* conversation = new Conversation;
            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), NPC_CONV, player, NULL, *player))
                delete conversation;
        }

        return true;
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
            player->KilledMonsterCredit(88872);
            player->CastSpell(player, SCENE, true);
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

//94410
class npc_q40378 : public CreatureScript
{
public:
    npc_q40378() : CreatureScript("npc_q40378") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q40378AI(creature);
    }

    enum data
    {
        QUEST = 40378,
    };

    struct npc_q40378AI : public ScriptedAI
    {
        npc_q40378AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
        }

        void MoveInLineOfSight(Unit* who)
        {
            Player *player = who->ToPlayer();
            if (!player || !me->IsWithinDistInMap(who, 100.0f))
                return;

            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return;

            if (player->GetQuestObjectiveData(QUEST, me->GetEntry()))
                return;

            player->KilledMonsterCredit(me->GetEntry());
            return;
        }
    };
};

class conversation_announcer : public CreatureScript
{
public:
    conversation_announcer() : CreatureScript("conversation_announcer") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new conversation_announcerAI(creature);
    }

    struct conversation_announcerAI : public ScriptedAI
    {
        conversation_announcerAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
        }

        void Reset()
        {
            conversationEntry = 0;
            targetGUID.Clear();
            me->SetReactState(REACT_PASSIVE);
        }

        enum events
        {
            EVENT_1 = 1,
            EVENT_2_ANNOUNCER6 = 2,
            EVENT_CLEAR = 3,
        };

        enum npcs
        {
            NPC_ANNOUNCER_1 = 101748, //583
            NPC_ANNOUNCER_2 = 101781, //1542
        };

        uint32 conversationEntry;
        ObjectGuid targetGUID;
        GuidSet m_player_for_event;
        EventMap events;
        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER || who->IsOnVehicle())
                return;

            GuidSet::iterator itr = m_player_for_event.find(who->GetGUID());
            if (itr != m_player_for_event.end())
                return;

            if (!me->IsWithinDistInMap(who, 60.0f))
                return;

            uint32 eTimer = 1000;

            switch (me->GetEntry())
            {
                case NPC_ANNOUNCER_1:
                    conversationEntry = 583;
                    events.RescheduleEvent(EVENT_1, eTimer);
                    break;
                case NPC_ANNOUNCER_2:
                    conversationEntry = 1542;
                    events.RescheduleEvent(EVENT_2, eTimer);
                    break;
                default:
                    break;
            }

            ASSERT(conversationEntry);
            m_player_for_event.insert(who->GetGUID());
            events.RescheduleEvent(EVENT_CLEAR, 300000);
            targetGUID = who->GetGUID();
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(targetGUID))
                        {
                            Conversation* conversation = new Conversation;
                            if (!conversation->CreateConversation(sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate(), conversationEntry, player, NULL, *player))
                                delete conversation;
                        }
                        break;
                    }
                    case EVENT_2:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(targetGUID))
                        {
                            player->SendSpellScene(conversationEntry, nullptr, true, player);
                        }
                        break;
                    }

                    case EVENT_CLEAR:
                        m_player_for_event.clear();
                        break;
                }
            }
        }
    };
};

//93105
class npc_q39049 : public CreatureScript
{
public:
    npc_q39049() : CreatureScript("npc_q39049") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q39049AI(creature);
    }

    enum data
    {
        QUEST       = 39049,
        CREDIT      = 105946,
        SPELL_LEARN = 195447,
    };

    struct npc_q39049AI : public ScriptedAI
    {
        npc_q39049AI(Creature* creature) : ScriptedAI(creature) {}
        void Reset()
        {
            me->SetReactState(REACT_DEFENSIVE);
        }

        void EnterCombat(Unit* victim)
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_0);
        }

        void JustDied(Unit* attacker)
        {
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2);

            Player *player = attacker->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(QUEST) != QUEST_STATUS_INCOMPLETE)
                return;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL_LEARN, false);
        }

        void UpdateAI(uint32 diff)
        {
            UpdateVictim();
            DoMeleeAttackIfReady();
        }
    };
};

//242989 244916 242987 242990
class go_q38759 : public GameObjectScript
{
public:
    go_q38759() : GameObjectScript("go_q38759") { }

    struct go_q38759AI : public GameObjectAI
    {
        go_q38759AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38759,
        };

        bool GossipHello(Player* player) override
        {
            std::map<uint32, uint32> _data;
            _data[242989] = 94400;
            _data[244916] = 94377;
            _data[242987] = 93117;
            _data[242990] = 93230;

            if (player->GetQuestObjectiveData(QUEST, _data[go->GetEntry()]))
                return true;

            if (Creature *c = player->FindNearestCreature(_data[go->GetEntry()], 100.0f))
            {
                player->KilledMonsterCredit(c->GetEntry());
                sCreatureTextMgr->SendChat(c, TEXT_GENERIC_1, player->GetGUID());
            }

            return true;
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q38759AI(go);
    }
};

//99914
class npc_q40379 : public CreatureScript
{
public:
    npc_q40379() : CreatureScript("npc_q40379") { }

    enum Credit
    {
        QUEST = 40379,
        SPELL = 196724,
    };
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(QUEST, creature->GetEntry()))
            {
            /*
            ClientToServer: CMSG_CLOSE_INTERACTION(0x348A) Length : 15 ConnIdx : 2 Time : 02 / 22 / 2016 13 : 10 : 10.127 Number : 17187
            Guid : Full : 0x202090B92061928000109C00004AF10C; HighType: Creature; Low: 4911372; Map: 1481; Entry: 99914;
            */
                player->CastSpell(creature, SPELL, false);
                player->KilledMonsterCredit(creature->GetEntry());
            }
        }

        return true;
    }
};


class go_q40379 : public GameObjectScript
{
public:
    go_q40379() : GameObjectScript("go_q40379") { }

    struct go_q40379AI : public GameObjectAI
    {
        go_q40379AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST  = 40379,
            SPELL  = 190793,
            CREDIT = 94406,
            CREDIT_REQUARE = 99914,
        };

        bool GossipUse(Player* player)
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT) || !player->GetReqKillOrCastCurrentCount(QUEST, CREDIT_REQUARE))
                return true;
            return false;
        }

        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT) || !player->GetReqKillOrCastCurrentCount(QUEST, CREDIT_REQUARE))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q40379AI(go);
    }
};

//243335
class go_q39050 : public GameObjectScript
{
public:
    go_q39050() : GameObjectScript("go_q39050") { }

    struct go_q39050AI : public GameObjectAI
    {
        go_q39050AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 40379,
            SPELL = 188539,
            CREDIT = 100722,
        };

        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q39050AI(go);
    }
};

//38765
class npc_q38765 : public CreatureScript
{
public:
    npc_q38765() : CreatureScript("npc_q38765") { }

    enum Credit
    {
        QUEST = 38765,
        SPELL = 196731,
    };
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == 1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (player->GetQuestStatus(QUEST) == QUEST_STATUS_INCOMPLETE && !player->GetReqKillOrCastCurrentCount(QUEST, creature->GetEntry()))
            {
                /*
                ClientToServer: CMSG_CLOSE_INTERACTION (0x348A) Length: 15 ConnIdx: 2 Time: 02/22/2016 13:25:19.942 Number: 45189
                Guid: Full: 0x202090B9206192C000109C00004AF10B; HighType: Creature; Low: 4911371; Map: 1481; Entry: 99915;
                */
                sCreatureTextMgr->SendChat(creature, TEXT_GENERIC_1, player->GetGUID());
                player->CastSpell(creature, SPELL, false);
                player->KilledMonsterCredit(creature->GetEntry());
                creature->SendPlaySpellVisualKit(25111, 0);
            }
        }

        return true;
    }
};

//241757
class go_q38765 : public GameObjectScript
{
public:
    go_q38765() : GameObjectScript("go_q38765") { }

    struct go_q38765AI : public GameObjectAI
    {
        go_q38765AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38765,
            SPELL = 190851,
            CREDIT = 94407,
        };

        bool GossipHello(Player* player) override
        {
            if (player->GetReqKillOrCastCurrentCount(QUEST, CREDIT))
                return true;

            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL, false);
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q38765AI(go);
    }
};

//93221 Doom Commander Beliash
class npc_q93221_beliash : public CreatureScript
{
public:
    npc_q93221_beliash() : CreatureScript("npc_q93221_beliash") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q93221_beliashAI(creature);
    }

    struct npc_q93221_beliashAI : public ScriptedAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q93221_beliashAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum data
        {
            QUEST           = 38766,
            SPELL_AT_DEATH  = 210093,
            CREDIT          = 106003,
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
        }

        void DoAction(int32 const /*param*/)
        {

        }

        //! HACK!!! ANTIL FINISH EVENT
        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->ToPlayer())
            {
                damage *= 3;
                return;
            }
            damage /= 2;
        }

        void JustDied(Unit* killer)
        {
            Player *player = killer->ToPlayer();
            if (!player)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_1, player->GetGUID());
            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL_AT_DEATH, false);
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

//96441 Fel Lord Caza
class npc_q39495_caza : public CreatureScript
{
public:
    npc_q39495_caza() : CreatureScript("npc_q39495_caza") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q39495_cazaAI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q39495_cazaAI : public Scripted_NoMovementAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q39495_cazaAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
        }

        enum data
        {
            QUEST = 39495,
            SPELL_AT_DEATH = 210107,
            CREDIT = 106014,
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
        }

        void DoAction(int32 const /*param*/)
        {

        }

        //! HACK!!! ANTIL FINISH EVENT
        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->ToPlayer())
            {
                damage *= 3;
                return;
            }
            damage /= 2;
        }

        void JustDied(Unit* killer)
        {
            Player *player = killer->ToPlayer();
            if (!player)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, player->GetGUID());
            player->KilledMonsterCredit(CREDIT);
            player->CastSpell(player, SPELL_AT_DEATH, false);
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

// 243965 243968 243967
// Q: 38727
class go_q38727 : public GameObjectScript
{
public:
    go_q38727() : GameObjectScript("go_q38727") { }
    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_q38727AI(go);
    }

    struct go_q38727AI : public GameObjectAI
    {
        go_q38727AI(GameObject* go) : GameObjectAI(go)
        {

        }

        enum data
        {
            QUEST = 38727,
            CREDIT = 94407,
        };

        bool GossipHello(Player* player) override
        {
            uint32 credit1 = 0;
            uint32 credit2 = 0;

            switch (go->GetEntry())
            {
                case 243965:
                    credit1 = 93762;
                    credit2 = 96692;
                    break;
                case 243968:
                    credit1 = 96732;
                    credit2 = 96734;
                    break;
                case 243967:
                    credit1 = 96731;
                    credit2 = 96733;
                    break;
            }
            if (player->GetQuestObjectiveData(QUEST, credit1))
                return true;

            player->KilledMonsterCredit(credit1);
            player->KilledMonsterCredit(credit2); //Hack. Special event.
            return false;
        }
    };
};

//103429
class npc_q40222_progres1 : public CreatureScript
{
public:
    npc_q40222_progres1() : CreatureScript("npc_q40222_progres1") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q40222_progres1AI(creature);
    }

    struct npc_q40222_progres1AI : public ScriptedAI
    {
        npc_q40222_progres1AI(Creature* creature) : ScriptedAI(creature)
        {
            SetCanSeeEvenInPassiveMode(true);
        }

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
        }


        enum data
        {
            CREDIT = 103429,
            QUEST = 40222,
        };
        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !me->IsWithinDistInMap(who, 50.0f))
                return;

            if (who->ToPlayer()->GetQuestObjectiveData(QUEST, CREDIT))
                return;

            who->ToPlayer()->KilledMonsterCredit(CREDIT);
        }
    };
};
//98986 Prolifica
class npc_q39495_prolifica : public CreatureScript
{
public:
    npc_q39495_prolifica() : CreatureScript("npc_q39495_prolifica") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q39495_prolificaAI(creature);
    }

    //! Scripted_NoMovementAI HACK!
    struct npc_q39495_prolificaAI : public Scripted_NoMovementAI
    {
        EventMap events;
        ObjectGuid playerGuid;

        npc_q39495_prolificaAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
        }

        enum data
        {
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
        }

        void DoAction(int32 const /*param*/)
        {

        }

        //! HACK!!! ANTIL FINISH EVENT
        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->ToPlayer())
            {
                damage *= 3;
                return;
            }
            damage /= 2;
        }

        void JustDied(Unit* killer)
        {
            Player *player = killer->ToPlayer();
            if (!player)
                return;
            sCreatureTextMgr->SendChat(me, TEXT_GENERIC_2, player->GetGUID());
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
void AddSC_Mardum()
{
    new sceneTrigger_dh_init();
    new go_q40077();
    new npc_q40077();
    new go_q40378();
    new go_q39279();
    new spell_legion_q39279();
    new npc_q40378();
    new conversation_announcer();
    new npc_q39049();
    new go_q38759();
    new npc_q40379();
    new go_q40379();
    new go_q39050();
    new npc_q38765();
    new go_q38765();
    new npc_q93221_beliash();
    new npc_q39495_caza();
    new go_q38727();
    new npc_q40222_progres1();
    new npc_q39495_prolifica();
}