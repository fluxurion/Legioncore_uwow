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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "GossipDef.h"
#include "CharacterPackets.h"
#include "QueryPackets.h"
#include "BattlenetAccountMgr.h"
#include "CharacterPackets.h"
#include "QueryPackets.h"
#include "MiscPackets.h"

void WorldSession::SendNameQueryOpcode(ObjectGuid guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);

    WorldPackets::Query::QueryPlayerNameResponse response;
    response.Player = guid;

    if (response.Data.Initialize(guid, player))
        response.Result = RESPONSE_SUCCESS; // name known
    else
        response.Result = RESPONSE_FAILURE; // name unknown

    SendPacket(response.Write());
}


void WorldSession::HandleQueryPlayerName(WorldPackets::Query::QueryPlayerName& packet)
{
    //SendPacket(WorldPackets::Misc::InvalidatePlayer(packet.Player).Write()); fuck fuck fuck, there's the rule for sending this?
    SendNameQueryOpcode(packet.Player);
}

void WorldSession::HandleQueryTime(WorldPackets::Query::QueryTime& /*packet*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPackets::Query::QueryTimeResponse queryTimeResponse;
    queryTimeResponse.CurrentTime = time(nullptr);
    queryTimeResponse.TimeOutRequest = sWorld->GetNextDailyQuestsResetTime() - queryTimeResponse.CurrentTime;
    SendPacket(queryTimeResponse.Write());
}

void WorldSession::HandleCreatureQuery(WorldPackets::Query::QueryCreature& packet)
{
    WorldPackets::Query::QueryCreatureResponse response;
    response.CreatureID = packet.CreatureID;

    if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(packet.CreatureID))
    {
        response.Allow = true;

        WorldPackets::Query::CreatureStats& stats = response.Stats;
        for (uint8 i = 0; i < MAX_CREATURE_NAMES; ++i)
        {
            stats.Name[i] = creatureInfo->Name[i];
            stats.NameAlt[i] = creatureInfo->NameAlt[i];
        }
        stats.Title = creatureInfo->Title;
        stats.TitleAlt = creatureInfo->TitleAlt;

        LocaleConstant localeConstant = GetSessionDbLocaleIndex();
        if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
            if (CreatureLocale const* creatureLocale = sObjectMgr->GetCreatureLocale(packet.CreatureID))
            {
                for (uint8 i = 0; i < MAX_CREATURE_NAMES; ++i)
                {
                    ObjectMgr::GetLocaleString(creatureLocale->Name[i], localeConstant, stats.Name[i]);
                    ObjectMgr::GetLocaleString(creatureLocale->NameAlt[i], localeConstant, stats.NameAlt[i]);
                }
                ObjectMgr::GetLocaleString(creatureLocale->Title, localeConstant, stats.Title);
                ObjectMgr::GetLocaleString(creatureLocale->TitleAlt, localeConstant, stats.TitleAlt);
            }

        stats.CursorName = creatureInfo->CursorName;
        stats.CreatureType = creatureInfo->Type;
        stats.CreatureFamily = creatureInfo->Family;
        stats.Classification = creatureInfo->Classification;
        stats.HpMulti = creatureInfo->HpMulti;
        stats.EnergyMulti = creatureInfo->PowerMulti;
        stats.Leader = creatureInfo->Leader;
        stats.CreatureMovementInfoID = creatureInfo->MovementInfoID;
        stats.RequiredExpansion = creatureInfo->RequiredExpansion;
        for (uint8 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (creatureInfo->QuestItem[i])
                stats.QuestItems.push_back(creatureInfo->QuestItem[i]);
        for (uint8 i = 0; i < MAX_TYPE_FLAGS; ++i)
            stats.Flags[i] = creatureInfo->TypeFlags[i];
        for (uint8 i = 0; i < MAX_KILL_CREDIT; ++i)
            stats.ProxyCreatureID[i] = creatureInfo->KillCredit[i];
        for (uint8 i = 0; i < MAX_CREATURE_MODELS; ++i)
            stats.CreatureDisplayID[i] = creatureInfo->Modelid[i];
    }

    SendPacket(response.Write());
}

void WorldSession::HandleQueryGameObject(WorldPackets::Query::QueryGameObject& packet)
{
    WorldPackets::Query::QueryGameObjectResponse response;

    response.GameObjectID = packet.GameObjectID;

    if (GameObjectTemplate const* gameObjectInfo = sObjectMgr->GetGameObjectTemplate(packet.GameObjectID))
    {
        response.Allow = true;
        WorldPackets::Query::GameObjectStats& stats = response.Stats;

        stats.DisplayID = gameObjectInfo->displayId;
        stats.IconName = gameObjectInfo->IconName;
        stats.Name[0] = gameObjectInfo->name;
        stats.CastBarCaption = gameObjectInfo->castBarCaption;

        for (uint8 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; i++)
            if (gameObjectInfo->questItems[i])
                stats.QuestItems.push_back(gameObjectInfo->questItems[i]);

        for (uint32 i = 0; i < MAX_GAMEOBJECT_DATA; i++)
            stats.Data[i] = gameObjectInfo->raw.data[i];

        stats.Size = gameObjectInfo->size;
        stats.Type = gameObjectInfo->type;
        stats.UnkString = gameObjectInfo->unk1;
        stats.Expansion = 0;

        LocaleConstant localeConstant = GetSessionDbLocaleIndex();
        if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
            if (GameObjectLocale const* gameObjectLocale = sObjectMgr->GetGameObjectLocale(packet.GameObjectID))
            {
                ObjectMgr::GetLocaleString(gameObjectLocale->Name, localeConstant, stats.Name[0]);
                ObjectMgr::GetLocaleString(gameObjectLocale->CastBarCaption, localeConstant, stats.CastBarCaption);
            }
    }
    else
        response.Allow = false;

    SendPacket(response.Write());
}

void WorldSession::HandleQueryCorpseLocation(WorldPackets::Query::QueryCorpseLocationFromClient& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Corpse* corpse = player->GetCorpse();
    if (!corpse)
    {
        WorldPackets::Query::CorpseLocation packet;
        packet.Valid = false;
        SendPacket(packet.Write());
        return;
    }

    uint32 mapID = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpsemapid = mapID;

    if (mapID != player->GetMapId())
    {
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapID))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->CorpseMapID >= 0)
            {
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->CorpseMapID))
                {
                    mapID = corpseMapEntry->CorpseMapID;
                    x = corpseMapEntry->CorpsePos.X;
                    y = corpseMapEntry->CorpsePos.Y;
                    z = entranceMap->GetHeight(player->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }

    WorldPackets::Query::CorpseLocation packet;
    packet.Valid = true;
    packet.MapID = corpsemapid;
    packet.ActualMapID = mapID;
    packet.position = {x, y, z};
    packet.Transport = corpse->GetTransGUID();
    SendPacket(packet.Write());
}

void WorldSession::HandleQueryNPCText(WorldPackets::Query::QueryNPCText& packet)
{
    WorldPackets::Query::QueryNPCTextResponse response;
    response.TextID = packet.TextID;

    if (NpcText const* npcText = sObjectMgr->GetNpcText(packet.TextID))
        for (uint8 i = 0; i < MAX_NPC_TEXT_OPTIONS; ++i)
        {
            response.Probabilities[i] = npcText->Data[i].Probability;
            response.BroadcastTextID[i] = npcText->Data[i].BroadcastTextID;
            if (!response.Allow && npcText->Data[i].BroadcastTextID)
                response.Allow = true;
        }

    SendPacket(response.Write());
}

void WorldSession::HandleQueryPageText(WorldPackets::Query::QueryPageText& packet)
{
    uint32 pageID = packet.PageTextID;

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);

        WorldPackets::Query::QueryPageTextResponse response;
        response.PageTextID = pageID;

        if (!pageText)
        {
            response.Allow = false;
            pageID = 0;
        }
        else
        {
            response.Allow = true;
            response.Info.ID = pageID;
            response.Info.Text = pageText->Text;

            LocaleConstant localeConstant = GetSessionDbLocaleIndex();
            if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, localeConstant, response.Info.Text);

            response.Info.NextPageID = pageText->NextPage;
            pageID = pageText->NextPage;
        }

        SendPacket(response.Write());
    }
}

void WorldSession::HandleQueryCorpseTransport(WorldPackets::Query::QueryCorpseTransport& packet)
{
    WorldPackets::Query::CorpseTransportQuery response;
    if (Corpse* corpse = _player->GetCorpse())
        if (!corpse->GetTransGUID().IsEmpty() && corpse->GetTransGUID() == packet.Transport)
            response.position = corpse->GetTransPosition();

    SendPacket(response.Write());
}

void WorldSession::HandleQuestPOIQuery(WorldPackets::Query::QuestPOIQuery& packet)
{
    if (packet.MissingQuestCount > MAX_QUEST_LOG_SIZE)
        return;

    std::unordered_set<int32> questIds;
    for (int32 i = 0; i < packet.MissingQuestCount; ++i)
        questIds.insert(packet.MissingQuestPOIs[i]);

    WorldPackets::Query::QuestPOIQueryResponse response;

    for (auto itr = questIds.begin(); itr != questIds.end(); ++itr)
    {
        int32 QuestID = *itr;

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(uint32(QuestID));

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk = _player->GetQuestSlotQuestId(questSlot) == uint32(QuestID);

        if (questOk)
        {
            QuestPOIVector const* poiData = sObjectMgr->GetQuestPOIVector(QuestID);
            if (poiData)
            {
                WorldPackets::Query::QuestPOIData questPOIData;

                questPOIData.QuestID = QuestID;

                for (auto data = poiData->begin(); data != poiData->end(); ++data)
                {
                    WorldPackets::Query::QuestPOIBlobData questPOIBlobData;

                    questPOIBlobData.BlobIndex = data->BlobIndex;
                    questPOIBlobData.ObjectiveIndex = data->ObjectiveIndex;
                    questPOIBlobData.QuestObjectiveID = data->QuestObjectiveID;
                    questPOIBlobData.QuestObjectID = data->QuestObjectID;
                    questPOIBlobData.MapID = data->MapID;
                    questPOIBlobData.WorldMapAreaID = data->WorldMapAreaID;
                    questPOIBlobData.Floor = data->Floor;
                    questPOIBlobData.Priority = data->Priority;
                    questPOIBlobData.Flags = data->Flags;
                    questPOIBlobData.WorldEffectID = data->WorldEffectID;
                    questPOIBlobData.PlayerConditionID = data->PlayerConditionID;
                    questPOIBlobData.UnkWoD1 = data->UnkWoD1;

                    for (auto points = data->points.begin(); points != data->points.end(); ++points)
                    {
                        WorldPackets::Query::QuestPOIBlobPoint questPOIBlobPoint;
                        questPOIBlobPoint.X = points->X;
                        questPOIBlobPoint.Y = points->Y;
                        questPOIBlobData.QuestPOIBlobPointStats.push_back(questPOIBlobPoint);
                    }

                    questPOIData.QuestPOIBlobDataStats.push_back(questPOIBlobData);
                }

                response.QuestPOIDataStats.push_back(questPOIData);
            }
        }
    }

    SendPacket(response.Write());
}

void WorldSession::HandleDBQueryBulk(WorldPackets::Query::DBQueryBulk& packet)
{
    DB2StorageBase const* store = sDB2Manager.GetStorage(packet.TableHash);
    if (!store)
    {
        sLog->outError(LOG_FILTER_GENERAL, "DBQueryBulk:: client requested unused db2 storage: %u; can by finded in DB2Hashes", packet.TableHash);
        return;
    }

    for (WorldPackets::Query::DBQueryBulk::DBQueryRecord const& rec : packet.Queries)
    {
        WorldPackets::Query::DBReply response;
        response.TableHash = packet.TableHash;
        response.RecordID = rec.RecordID;

        if (store->HasRecord(rec.RecordID))
        {
            response.Allow = true;
            response.Timestamp = sDB2Manager.GetHotfixDate(rec.RecordID, packet.TableHash);
            store->WriteRecord(rec.RecordID, GetSessionDbcLocale(), response.Data);
        }
        else
            response.Timestamp = time(nullptr);

        SendPacket(response.Write());
    }
}

void WorldSession::HandleItemTextQuery(WorldPackets::Query::ItemTextQuery& packet)
{
    WorldPackets::Query::QueryItemTextResponse response;
    response.Id = packet.Id;

    if (Item* item = _player->GetItemByGuid(packet.Id))
    {
        response.Valid = true;
        response.Item.Text = item->GetText();
    }

    SendPacket(response.Write());
}

void WorldSession::HandleQueryQuestCompletionNPCs(WorldPackets::Query::QueryQuestCompletionNPCs& packet)
{
    WorldPackets::Query::QuestCompletionNPCResponse response;

    for (int32 const& questID : packet.QuestCompletionNPCs)
    {
        if (!sObjectMgr->GetQuestTemplate(questID))
            continue;

        WorldPackets::Query::QuestCompletionNPC questCompletionNPC;
        questCompletionNPC.QuestID = questID;

        auto creatures = sObjectMgr->GetCreatureQuestInvolvedRelationBounds(questID);
        for (auto it = creatures.first; it != creatures.second; ++it)
            questCompletionNPC.NPCs.push_back(it->second);

        auto gos = sObjectMgr->GetGOQuestInvolvedRelationBounds(questID);
        for (auto it = gos.first; it != gos.second; ++it)
            questCompletionNPC.NPCs.push_back(it->second | 0x80000000); // GO mask

        response.QuestCompletionNPCs.push_back(questCompletionNPC);
    }

    SendPacket(response.Write());
}

void WorldSession::HandleQueryRealmName(WorldPackets::Query::QueryRealmName& packet)
{
    WorldPackets::Query::RealmQueryResponse response;
    response.VirtualRealmAddress = packet.RealmID;
    if (packet.RealmID != realm.Id.Realm && packet.RealmID != GetVirtualRealmAddress())  // Cheater ?
    {
        response.LookupState = 1;
        SendPacket(response.Write());
    }

    response.NameInfo.IsLocal = true;
    response.NameInfo.RealmNameActual = sWorld->GetRealmName();
    response.NameInfo.RealmNameNormalized = sWorld->GetTrimmedRealmName();
    SendPacket(response.Write());
}
