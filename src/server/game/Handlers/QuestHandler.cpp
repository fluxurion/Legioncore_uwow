/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "GossipDef.h"
#include "QuestDef.h"
#include "ObjectAccessor.h"
#include "Group.h"
#include "Battleground.h"
#include "BattlegroundAV.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "QuestPackets.h"

void WorldSession::HandleQuestGiverStatusQuery(WorldPackets::Quest::QuestGiverStatusQuery& packet)
{
    uint32 questStatus = DIALOG_STATUS_NONE;
    uint32 defstatus = DIALOG_STATUS_NONE;

    Object* questgiver = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
    if (!questgiver)
        return;

    switch (questgiver->GetTypeId())
    {
        case TYPEID_UNIT:
        {
            Creature* cr_questgiver=questgiver->ToCreature();
            if (!cr_questgiver->IsHostileTo(_player))       // do not show quest status to enemies
            {
                questStatus = sScriptMgr->GetDialogStatus(_player, cr_questgiver);
                if (questStatus > 6)
                    questStatus = getDialogStatus(_player, cr_questgiver, defstatus);
            }
            break;
        }
        case TYPEID_GAMEOBJECT:
        {
            GameObject* go_questgiver=(GameObject*)questgiver;
            questStatus = sScriptMgr->GetDialogStatus(_player, go_questgiver);
            if (questStatus > 6)
                questStatus = getDialogStatus(_player, go_questgiver, defstatus);
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "QuestGiver called for unexpected type %u", questgiver->GetTypeId());
            break;
    }

    //inform client about status of quest
    _player->PlayerTalkClass->SendQuestGiverStatus(questStatus, packet.QuestGiverGUID);
}

void WorldSession::HandleQuestGiverHello(WorldPackets::Quest::QuestGiverHello& packet)
{
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(packet.QuestGiverGUID, UNIT_NPC_FLAG_NONE);
    if (!creature)
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);
    // Stop the npc if moving
    creature->StopMoving();

    if (sScriptMgr->OnGossipHello(_player, creature))
        return;

    _player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->GossipMenuId, true);
    _player->SendPreparedGossip(creature);

    creature->AI()->sGossipHello(_player);
}

void WorldSession::HandleQuestGiverAcceptQuest(WorldPackets::Quest::QuestGiverAcceptQuest& packet)
{
    Object* object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT|TYPEMASK_ITEM|TYPEMASK_PLAYER);

    if (!object || object == _player)
        return;

    // no or incorrect quest giver
    if (!object || (object->GetTypeId() != TYPEID_PLAYER && !object->hasQuest(packet.QuestID)) ||
        (object->GetTypeId() == TYPEID_PLAYER && object != _player && !object->ToPlayer()->CanShareQuest(packet.QuestID)))
    {
        _player->PlayerTalkClass->SendCloseGossip();
        _player->SetDivider(ObjectGuid::Empty);
        return;
    }

    if (object && object->GetTypeId() == TYPEID_PLAYER && !object->hasQuest(packet.QuestID))
        return;

    // some kind of WPE protection
    if (!_player->CanInteractWithQuestGiver(object))
        return;

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(packet.QuestID))
    {
        // prevent cheating
        if (!GetPlayer()->CanTakeQuest(quest, true))
        {
            _player->PlayerTalkClass->SendCloseGossip();
            _player->SetDivider(ObjectGuid::Empty);
            return;
        }

        if (object && object->GetTypeId() == TYPEID_PLAYER && !quest->HasFlag(QUEST_FLAGS_SHARABLE))
            return;

        if (_player->GetDivider())
        {
            Player* player = ObjectAccessor::FindPlayer(_player->GetDivider());
            if (!player)
            {
                _player->SetDivider(ObjectGuid::Empty);
                return;
            }
            if (player)
            {
                if (!player->CanShareQuest(packet.QuestID))
                {
                    player->SendPushToPartyResponse(_player, QUEST_PARTY_MSG_CANT_TAKE_QUEST);
                    _player->SetDivider(ObjectGuid::Empty);
                    return;
                }
                player->SendPushToPartyResponse(_player, QUEST_PARTY_MSG_ACCEPT_QUEST);
                _player->SetDivider(ObjectGuid::Empty);
            }
        }

        if (_player->CanAddQuest(quest, true))
        {
            _player->AddQuest(quest, object);

            if (quest->HasFlag(QUEST_FLAGS_PARTY_ACCEPT))
            {
                if (Group* group = _player->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* player = itr->getSource();

                        if (!player || player == _player)     // not self
                            continue;

                        if (player->CanTakeQuest(quest, true))
                        {
                            player->SetDivider(_player->GetGUID());

                            //need confirmation that any gossip window will close
                            player->PlayerTalkClass->SendCloseGossip();

                            _player->SendQuestConfirmAccept(quest, player);
                        }
                    }
                }
            }

            if (_player->CanCompleteQuest(packet.QuestID))
                _player->CompleteQuest(packet.QuestID);

            switch (object->GetTypeId())
            {
                case TYPEID_UNIT:
                    sScriptMgr->OnQuestAccept(_player, (object->ToCreature()), quest);
                    (object->ToCreature())->AI()->sQuestAccept(_player, quest);
                    break;
                case TYPEID_ITEM:
                case TYPEID_CONTAINER:
                {
                    sScriptMgr->OnQuestAccept(_player, ((Item*)object), quest);

                    // destroy not required for quest finish quest starting item
                    bool destroyItem = true;
                    for (QuestObjective const& obj : quest->GetObjectives())
                    {
                        if (obj.Type == QUEST_OBJECTIVE_ITEM)
                            if ((obj.ObjectID == ((Item*)object)->GetEntry()) && (((Item*)object)->GetTemplate()->MaxCount > 0))
                        {
                            destroyItem = false;
                            break;
                        }
                    }

                    if (destroyItem)
                        _player->DestroyItem(((Item*)object)->GetBagSlot(), ((Item*)object)->GetSlot(), true);

                    break;
                }
                case TYPEID_GAMEOBJECT:
                    sScriptMgr->OnQuestAccept(_player, ((GameObject*)object), quest);
                    (object->ToGameObject())->AI()->QuestAccept(_player, quest);
                    break;
                default:
                    break;
            }
            _player->PlayerTalkClass->SendCloseGossip();

            if (quest->GetSrcSpell() > 0)
                _player->CastSpell(_player, quest->GetSrcSpell(), true);

            return;
        }
    }

    _player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleQuestGiverQueryQuest(WorldPackets::Quest::QuestGiverQueryQuest& packet)
{
    // Verify that the guid is valid and is a questgiver or involved in the requested quest
    Object* object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_ITEM);
    if (!object || (!object->hasQuest(packet.QuestID) && !object->hasInvolvedQuest(packet.QuestID)))
    {
        _player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(packet.QuestID))
    {
        // not sure here what should happen to quests with QUEST_FLAGS_AUTOCOMPLETE
        // if this breaks them, add && object->GetTypeId() == TYPEID_ITEM to this check
        // item-started quests never have that flag
        if (!_player->CanTakeQuest(quest, true))
            return;

        if (_player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_COMPLETE)
            _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, object->GetGUID(), true);
        else if (_player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_INCOMPLETE)
            _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, object->GetGUID(), _player->CanCompleteQuest(quest->GetQuestId()), true);
        else
        {
            if (quest->IsAutoAccept() && _player->CanAddQuest(quest, true))
            {
                if (Creature* pQuestGiver = ObjectAccessor::GetCreature(*_player, packet.QuestGiverGUID))
                    if (pQuestGiver->IsAIEnabled)
                        sScriptMgr->OnQuestAccept(_player, pQuestGiver, quest);

                _player->AddQuest(quest, object);
                if (_player->CanCompleteQuest(packet.QuestID))
                    _player->CompleteQuest(packet.QuestID);
            }
            _player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, object->GetGUID(), true);
        }
    }
}

void WorldSession::HandleQueryQuestInfo(WorldPackets::Quest::QueryQuestInfo& packet)
{
    if (!_player)
        return;

    _player->PlayerTalkClass->SendQuestQueryResponse(packet.QuestID);
}

void WorldSession::HandleQuestGiverChooseReward(WorldPackets::Quest::QuestGiverChooseReward& packet)
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(packet.QuestID);
    if (!quest)
        return;

    Object* object = _player;
    uint32 reward = 0;

    if (!quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT))
    {
        //TODO: Doing something less dirty
        for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
            if (quest->RewardChoiceItemId[i] == packet.ItemChoiceID)
                reward = i;
        if (reward >= QUEST_REWARD_CHOICES_COUNT)
            return;

        object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(packet.QuestID))
            return;

        // some kind of WPE protection
        if (!_player->CanInteractWithQuestGiver(object))
            return;

    }
    if ((!_player->CanSeeStartQuest(quest) &&  _player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_NONE) ||
        (_player->GetQuestStatus(packet.QuestID) != QUEST_STATUS_COMPLETE && !quest->IsAutoComplete()))
        return;

    if (_player->CanRewardQuest(quest, reward, true, packet.ItemChoiceID))
    {
        _player->RewardQuest(quest, reward, object, true, packet.ItemChoiceID);

        switch (object->GetTypeId())
        { 
        case TYPEID_UNIT:
        case TYPEID_PLAYER:
            {
                //For AutoSubmition was added plr case there as it almost same exclude AI script cases.
                Creature *creatureQGiver = object->ToCreature();
                if (!creatureQGiver || !(sScriptMgr->OnQuestReward(_player, creatureQGiver, quest, reward)))
                {
                    if (creatureQGiver)
                        creatureQGiver->AI()->sQuestReward(_player, quest, reward);
                    // Send next quest
                    if (Quest const* nextQuest = _player->GetNextQuest(packet.QuestGiverGUID, quest))
                    {
                        if (nextQuest->IsAutoAccept() && _player->CanAddQuest(nextQuest, true) && _player->CanTakeQuest(nextQuest, true))
                        {
                            if (creatureQGiver)
                            {
                                sScriptMgr->OnQuestAccept(_player, creatureQGiver, nextQuest);
                                creatureQGiver->AI()->sQuestAccept(_player, nextQuest);
                            }

                            _player->AddQuest(nextQuest, object);
                            if (_player->CanCompleteQuest(nextQuest->GetQuestId()))
                                _player->CompleteQuest(nextQuest->GetQuestId());
                            _player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, packet.QuestGiverGUID, true);
                        }else
                            _player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, packet.QuestGiverGUID, false);
                    }
                }
                break;
            }
        case TYPEID_GAMEOBJECT:
            if (!sScriptMgr->OnQuestReward(_player, ((GameObject*)object), quest, reward))
            {
                // Send next quest
                if (Quest const* nextQuest = _player->GetNextQuest(packet.QuestGiverGUID, quest))
                {
                    if (nextQuest->IsAutoAccept() && _player->CanAddQuest(nextQuest, true) && _player->CanTakeQuest(nextQuest, true))
                    {
                        _player->AddQuest(nextQuest, object);
                        if (_player->CanCompleteQuest(nextQuest->GetQuestId()))
                            _player->CompleteQuest(nextQuest->GetQuestId());
                    }

                    _player->PlayerTalkClass->SendQuestGiverQuestDetails(nextQuest, packet.QuestGiverGUID, true);
                }

                object->ToGameObject()->AI()->QuestReward(_player, quest, reward);
            }
            break;
        default:
            break;
        }
        // As quest complete send available quests. Need when questgiver from next quest chain staying near questtaker
        SendQuestgiverStatusMultipleQuery();

        // AutoTake system
         _player->PrepareAreaQuest( _player->GetAreaId());
    }
    else 
        _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, packet.QuestGiverGUID, true);
}

void WorldSession::HandleQuestGiverRequestReward(WorldPackets::Quest::QuestGiverRequestReward& packet)
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(packet.QuestID);
    if (!quest)
        return;

    if (!quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT))
    {
        Object* object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(packet.QuestID))
            return;

        // some kind of WPE protection
        if (!_player->CanInteractWithQuestGiver(object))
            return;
    }

    if (_player->CanCompleteQuest(packet.QuestID))
        _player->CompleteQuest(packet.QuestID);

    if (_player->GetQuestStatus(packet.QuestID) != QUEST_STATUS_COMPLETE)
        return;

    _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, packet.QuestGiverGUID, true);
}

void WorldSession::HandleQuestgiverCancel(WorldPacket& /*recvData*/)
{
    _player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleQuestLogSwapQuest(WorldPacket& recvData)
{
    uint8 slot1, slot2;
    recvData >> slot1 >> slot2;

    if (slot1 == slot2 || slot1 >= MAX_QUEST_LOG_SIZE || slot2 >= MAX_QUEST_LOG_SIZE)
        return;

    GetPlayer()->SwapQuestSlot(slot1, slot2);
}

void WorldSession::HandleQuestLogRemoveQuest(WorldPackets::Quest::QuestLogRemoveQuest& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.Entry > MAX_QUEST_LOG_SIZE)
        return;

    if (uint32 questId = player->GetQuestSlotQuestId(packet.Entry))
    {
        if (!player->TakeQuestSourceItem(questId, true))
            return;                                     // can't un-equip some items, reject quest cancel

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
        {
            if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
                player->RemoveTimedQuest(questId);
        }

        player->TakeQuestSourceItem(questId, true); // remove quest src item from player
        player->RemoveActiveQuest(questId);
        player->GetAchievementMgr().RemoveTimedAchievement(ACHIEVEMENT_TIMED_TYPE_QUEST, questId);

        sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %u abandoned quest %u", player->GetGUIDLow(), questId);
    }

    _player->SetQuestSlot(packet.Entry, 0);

    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED, 1);
}

void WorldSession::HandleQuestConfirmAccept(WorldPackets::Quest::QuestConfirmAccept& packet)
{
    if (Quest const* quest = sObjectMgr->GetQuestTemplate(packet.QuestID))
    {
        if (!quest->HasFlag(QUEST_FLAGS_PARTY_ACCEPT))
            return;

        Player* player = GetPlayer();
        if (!player)
            return;

        Player* pOriginalPlayer = ObjectAccessor::FindPlayer(player->GetDivider());

        if (!pOriginalPlayer)
            return;

        if (!player->IsInSameRaidWith(pOriginalPlayer))
            return;

        if (player->CanAddQuest(quest, true))
            player->AddQuest(quest, NULL);                // NULL, this prevent DB script from duplicate running

        player->SetDivider(ObjectGuid::Empty);
    }
}

void WorldSession::HandleQuestgiverCompleteQuest(WorldPackets::Quest::QuestGiverCompleteQuest& packet)
{
    //@TODO:Legion
    if (!packet.UnkINT)
    {
        Object* object = ObjectAccessor::GetObjectByTypeMask(*_player, packet.QuestGiverGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
        if (!object || !object->hasInvolvedQuest(packet.QuestID))
            return;

        // some kind of WPE protection
        if (!_player->CanInteractWithQuestGiver(object))
            return;
    }

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(packet.QuestID))
    {
        if (packet.UnkINT && !quest->HasFlag(QUEST_FLAGS_AUTO_SUBMIT))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [playerGuid: %s] tried to complete questId [entry: %u] by auto-submit flag for quest witch not suport it.",
                _player->GetName(), _player->GetGUID().ToString().c_str(), packet.QuestID);
            return;
        }
        if (!_player->CanSeeStartQuest(quest) && _player->GetQuestStatus(packet.QuestID) == QUEST_STATUS_NONE)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [playerGuid: %s] tried to complete questId [entry: %u] without being in possession of the questId!",
                          _player->GetName(), _player->GetGUID().ToString().c_str(), packet.QuestID);
            return;
        }
        // TODO: need a virtual function
        if (_player->InBattleground())
            if (Battleground* bg = _player->GetBattleground())
                if (bg->GetTypeID() == BATTLEGROUND_AV)
                    ((BattlegroundAV*)bg)->HandleQuestComplete(packet.QuestID, _player);

        if (_player->GetQuestStatus(packet.QuestID) != QUEST_STATUS_COMPLETE)
        {
            if (quest->IsRepeatable())
                _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, packet.QuestGiverGUID, _player->CanCompleteRepeatableQuest(quest), false);
            else
                _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, packet.QuestGiverGUID, _player->CanRewardQuest(quest, false), false);
        }
        else
        {
            if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER))                  // some items required
                _player->PlayerTalkClass->SendQuestGiverRequestItems(quest, packet.QuestGiverGUID, _player->CanRewardQuest(quest, false), false);
            else                                                                    // no items required
                _player->PlayerTalkClass->SendQuestGiverOfferReward(quest, packet.QuestGiverGUID, !packet.UnkINT);
        }
    }
}

void WorldSession::HandleQuestgiverQuestAutoLaunch(WorldPacket& /*recvPacket*/) { }

void WorldSession::HandlePushQuestToParty(WorldPacket& recvPacket)
{
    uint32 questId;
    recvPacket >> questId;

    if (_player->GetQuestStatus(questId) == QUEST_STATUS_NONE || _player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
        return;

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
    {
        if (Group* group = _player->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* player = itr->getSource();

                if (!player || player == _player)         // skip self
                    continue;

                _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_SHARING_QUEST);

                if (!player->SatisfyQuestStatus(quest, false))
                {
                    _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_HAVE_QUEST);
                    continue;
                }

                if (player->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE)
                {
                    _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_FINISH_QUEST);
                    continue;
                }

                if (!player->CanTakeQuest(quest, false))
                {
                    _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_CANT_TAKE_QUEST);
                    continue;
                }

                if (!player->SatisfyQuestLog(false))
                {
                    _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_LOG_FULL);
                    continue;
                }

                if (player->GetDivider())
                {
                    _player->SendPushToPartyResponse(player, QUEST_PARTY_MSG_BUSY);
                    continue;
                }

                player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, _player->GetGUID(), true);
                player->SetDivider(_player->GetGUID());
            }
        }
    }
}

void WorldSession::HandleQuestPushResult(WorldPackets::Quest::QuestPushResult& packet)
{
    if (!_player->GetDivider().IsEmpty() && _player->GetDivider() == packet.SenderGUID)
    {
        if (Player* player = ObjectAccessor::FindPlayer(_player->GetDivider()))
        {
            player->SendPushToPartyResponse(_player, static_cast<QuestShareMessages>(packet.Result));
            _player->SetDivider(ObjectGuid::Empty);
        }
    }
}

uint32 WorldSession::getDialogStatus(Player* player, Object* questgiver, uint32 defstatus)
{
    uint32 result = defstatus;

    QuestRelationBounds qr;
    QuestRelationBounds qir;

    switch (questgiver->GetTypeId())
    {
        case TYPEID_GAMEOBJECT:
        {
            qr  = sObjectMgr->GetGOQuestRelationBounds(questgiver->GetEntry());
            qir = sObjectMgr->GetGOQuestInvolvedRelationBounds(questgiver->GetEntry());
            break;
        }
        case TYPEID_UNIT:
        {
            qr  = sObjectMgr->GetCreatureQuestRelationBounds(questgiver->GetEntry());
            qir = sObjectMgr->GetCreatureQuestInvolvedRelationBounds(questgiver->GetEntry());
            break;
        }
        default:
            //its imposible, but check ^)
            sLog->outError(LOG_FILTER_NETWORKIO, "Warning: GetDialogStatus called for unexpected type %u", questgiver->GetTypeId());
            return DIALOG_STATUS_NONE;
    }

    for (QuestRelations::const_iterator i = qir.first; i != qir.second; ++i)
    {
        uint32 result2 = 0;
        uint32 quest_id = i->second;
        Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);
        if (!quest)
            continue;

        ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_SHOW_MARK, quest->GetQuestId());
        if (!sConditionMgr->IsObjectMeetToConditions(player, conditions))
            continue;

        conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_ACCEPT, quest->GetQuestId());
        if (!sConditionMgr->IsObjectMeetToConditions(player, conditions))
            continue;

        QuestStatus status = player->GetQuestStatus(quest_id);
        if ((status == QUEST_STATUS_COMPLETE && !player->GetQuestRewardStatus(quest_id)) ||
            (quest->IsAutoComplete() && player->CanTakeQuest(quest, false)))
        {
            if (quest->IsAutoComplete() && quest->IsRepeatable())
                result2 = DIALOG_STATUS_REWARD_REP;
            else
                result2 = DIALOG_STATUS_REWARD;
        }
        else if (status == QUEST_STATUS_INCOMPLETE)
            result2 = DIALOG_STATUS_INCOMPLETE;

        if (result2 > result)
            result = result2;
    }

    for (QuestRelations::const_iterator i = qr.first; i != qr.second; ++i)
    {
        uint32 result2 = 0;
        uint32 quest_id = i->second;
        Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);
        if (!quest)
            continue;

        ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_SHOW_MARK, quest->GetQuestId());
        if (!sConditionMgr->IsObjectMeetToConditions(player, conditions))
            continue;

        conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_ACCEPT, quest->GetQuestId());
        if (!sConditionMgr->IsObjectMeetToConditions(player, conditions))
            continue;

        QuestStatus status = player->GetQuestStatus(quest_id);
        if (status == QUEST_STATUS_NONE)
        {
            if (player->CanSeeStartQuest(quest))
            {
                if (player->SatisfyQuestLevel(quest, false))
                {
                    if (quest->IsAutoComplete() || (quest->IsRepeatable() && player->IsQuestRewarded(quest_id)))
                        result2 = DIALOG_STATUS_REWARD_REP;
                    else if (player->getLevel() <= ((player->GetQuestLevel(quest) == -1) ? player->getLevel() : player->GetQuestLevel(quest) + sWorld->getIntConfig(CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF)))
                    {
                        if (quest->HasFlag(QUEST_FLAGS_DAILY) || quest->HasFlag(QUEST_FLAGS_WEEKLY))
                            result2 = DIALOG_STATUS_AVAILABLE_REP;
                        else
                            result2 = DIALOG_STATUS_AVAILABLE;
                    }
                    else
                        result2 = DIALOG_STATUS_LOW_LEVEL_AVAILABLE;
                }
                else
                    result2 = DIALOG_STATUS_UNAVAILABLE;
            }
        }

        if (result2 > result)
            result = result2;
    }

    return result;
}

void WorldSession::HandleQuestgiverStatusMultipleQuery(WorldPackets::Quest::QuestGiverStatusMultipleQuery& /*packet*/)
{
    SendQuestgiverStatusMultipleQuery();
}

void WorldSession::SendQuestgiverStatusMultipleQuery()
{
    uint32 questStatus = DIALOG_STATUS_NONE;
    uint32 defstatus = DIALOG_STATUS_NONE;

    WorldPackets::Quest::QuestGiverStatusMultiple response;
    for (auto itr = _player->m_clientGUIDs.begin(); itr != _player->m_clientGUIDs.end(); ++itr)
    {
        if ((*itr).IsCreatureOrPetOrVehicle())
        {
            // need also pet quests case support
            Creature* questgiver = ObjectAccessor::GetCreatureOrPetOrVehicle(*GetPlayer(), *itr);
            if (!questgiver || questgiver->IsHostileTo(_player))
                continue;

            if (!questgiver->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
                continue;

            questStatus = sScriptMgr->GetDialogStatus(_player, questgiver);
            if (questStatus > 6)
                questStatus = getDialogStatus(_player, questgiver, defstatus);

            response.QuestGiver.emplace_back(questgiver->GetGUID(), questStatus);
        }
        else if ((*itr).IsGameObject())
        {
            GameObject* questgiver = GetPlayer()->GetMap()->GetGameObject(*itr);
            if (!questgiver)
                continue;

            if (questgiver->GetGoType() != GAMEOBJECT_TYPE_QUESTGIVER)
                continue;

            questStatus = sScriptMgr->GetDialogStatus(_player, questgiver);
            if (questStatus > 6)
                questStatus = getDialogStatus(_player, questgiver, defstatus);

            response.QuestGiver.emplace_back(questgiver->GetGUID(), questStatus);
        }
    }

    SendPacket(response.Write());
}
