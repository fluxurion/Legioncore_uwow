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

#include "QuestDef.h"
#include "GossipDef.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Formulas.h"
#include "QuestPackets.h"
#include "NPCPackets.h"

GossipMenu::GossipMenu()
{
    _menuId = 0;
    _locale = DEFAULT_LOCALE;
}

GossipMenu::~GossipMenu()
{
    ClearMenu();
}

void GossipMenu::AddMenuItem(int32 menuItemId, uint8 icon, std::string const& message, uint32 sender, uint32 action, std::string const& boxMessage, uint32 boxMoney, bool coded /*= false*/)
{
    if(_menuItems.size() > GOSSIP_MAX_MENU_ITEMS)
        return;
    //ASSERT(_menuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    // Find a free new id - script case
    if (menuItemId == -1)
    {
        menuItemId = 0;
        if (!_menuItems.empty())
        {
            for (GossipMenuItemContainer::const_iterator itr = _menuItems.begin(); itr != _menuItems.end(); ++itr)
            {
                if (int32(itr->first) > menuItemId)
                    break;

                menuItemId = itr->first + 1;
            }
        }
    }

    GossipMenuItem& menuItem = _menuItems[menuItemId];

    menuItem.MenuItemIcon    = icon;
    menuItem.Message         = message;
    menuItem.IsCoded         = coded;
    menuItem.Sender          = sender;
    menuItem.OptionType      = action;
    menuItem.BoxMessage      = boxMessage;
    menuItem.BoxMoney        = boxMoney;
}

void GossipMenu::AddMenuItem(uint32 menuId, uint32 menuItemId, uint32 sender, uint32 action)
{
    GossipMenuItemsMapBounds bounds = sObjectMgr->GetGossipMenuItemsMapBounds(menuId);
    if (bounds.first == bounds.second)
        return;

    LocaleConstant localeConstant = GetLocale();
    for (GossipMenuItemsContainer::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
    {
        if (itr->second.OptionIndex != menuItemId)
            continue;

        std::string strOptionText = itr->second.OptionText;
        std::string strBoxText = itr->second.BoxText;

        if (localeConstant != LOCALE_enUS && localeConstant != LOCALE_none)
        {
            if (BroadcastTextEntry const* optionBroadcastText = sBroadcastTextStore.LookupEntry(itr->second.OptionBroadcastTextID))
                strOptionText = DB2Manager::GetBroadcastTextValue(optionBroadcastText, GetLocale());
            else if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
                ObjectMgr::GetLocaleString(gossipMenuLocale->OptionText, GetLocale(), strOptionText);

            if (BroadcastTextEntry const* boxBroadcastText = sBroadcastTextStore.LookupEntry(itr->second.BoxBroadcastTextID))
                strBoxText = DB2Manager::GetBroadcastTextValue(boxBroadcastText, GetLocale());
            else if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
                ObjectMgr::GetLocaleString(gossipMenuLocale->BoxText, GetLocale(), strBoxText);
        }

        AddMenuItem(-1, itr->second.OptionIcon, strOptionText, sender, action, strBoxText, itr->second.BoxMoney, itr->second.BoxCoded);
    }
}

void GossipMenu::AddGossipMenuItemData(uint32 menuItemId, uint32 gossipActionMenuId, uint32 gossipActionPoi)
{
    GossipMenuItemData& itemData = _menuItemData[menuItemId];

    itemData.GossipActionMenuId  = gossipActionMenuId;
    itemData.GossipActionPoi     = gossipActionPoi;
}

uint32 GossipMenu::GetMenuItemSender(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.Sender;
}

uint32 GossipMenu::GetMenuItemAction(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.OptionType;
}

bool GossipMenu::IsMenuItemCoded(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return false;

    return itr->second.IsCoded;
}

void GossipMenu::ClearMenu()
{
    _menuItems.clear();
    _menuItemData.clear();
}

PlayerMenu::PlayerMenu(WorldSession* session) : _session(session)
{
    if (_session)
        _gossipMenu.SetLocale(_session->GetSessionDbLocaleIndex());
}

PlayerMenu::~PlayerMenu()
{
    ClearMenus();
}

void PlayerMenu::ClearMenus()
{
    _gossipMenu.ClearMenu();
    _questMenu.ClearMenu();
}

void PlayerMenu::SendGossipMenu(uint32 titleTextId, ObjectGuid objectGUID) const
{
    WorldPackets::NPC::GossipMessage packet;
    packet.GossipGUID = objectGUID;
    packet.TextID = titleTextId;
    packet.GossipID = _gossipMenu.GetMenuId();

    packet.GossipOptions.resize(_gossipMenu.GetMenuItems().size());
    uint32 count = 0;
    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        WorldPackets::NPC::ClientGossipOptions& opt = packet.GossipOptions[count];
        GossipMenuItem const& item = itr->second;
        opt.ClientOption = itr->first;
        opt.OptionNPC = item.MenuItemIcon;
        opt.OptionFlags = item.IsCoded;     // makes pop up box password
        opt.OptionCost = item.BoxMoney;     // money required to open menu, 2.0.3
        opt.Text = item.Message;            // text for gossip item
        opt.Confirm = item.BoxMessage;      // accept text (related to money) pop up box, 2.0.3
        ++count;
    }

    packet.GossipText.resize(_questMenu.GetMenuItemCount());
    count = 0;
    for (uint8 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& item = _questMenu.GetItem(i);
        uint32 questID = item.QuestId;
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            WorldPackets::NPC::ClientGossipText& text = packet.GossipText[count];
            text.QuestID = questID;
            text.QuestType = item.QuestIcon;
            text.QuestLevel = quest->Level;
            text.QuestFlags[0] = quest->GetFlags();
            text.QuestFlags[1] = quest->FlagsEx;
            text.Repeatable = quest->IsRepeatable();

            std::string title = quest->LogTitle;
            LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
            if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
                if (QuestTemplateLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(localeData->LogTitle, localeConstant, title);

            text.QuestTitle = title;
            ++count;
        }
    }

    // Shrink to the real size
    packet.GossipText.resize(count);

    _session->SendPacket(packet.Write());
}

void PlayerMenu::SendCloseGossip() const
{
    _session->SendPacket(WorldPackets::NPC::GossipComplete().Write());
}

void PlayerMenu::SendPointOfInterest(uint32 poiId) const
{
    PointOfInterest const* poi = sObjectMgr->GetPointOfInterest(poiId);
    if (!poi)
        return;

    std::string name = poi->icon_name;
    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
        if (PointOfInterestLocale const* localeData = sObjectMgr->GetPointOfInterestLocale(poiId))
            ObjectMgr::GetLocaleString(localeData->IconName, localeConstant, name);

    WorldPackets::NPC::GossipPOI packet;
    packet.Flags = poi->flags;
    packet.Pos.m_positionX = poi->x;
    packet.Pos.m_positionY = poi->y;
    packet.Icon = poi->icon;
    packet.Importance = poi->data;
    packet.Name = name;
    _session->SendPacket(packet.Write());
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(16);                                   // can be set for max from most often sizes to speedup push_back and less memory use
}

QuestMenu::~QuestMenu()
{
    ClearMenu();
}

void QuestMenu::AddMenuItem(uint32 QuestId, uint8 Icon)
{
    if (!sObjectMgr->GetQuestTemplate(QuestId))
        return;

    ASSERT(_questMenuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    QuestMenuItem questMenuItem;

    questMenuItem.QuestId        = QuestId;
    questMenuItem.QuestIcon      = Icon;

    _questMenuItems.push_back(questMenuItem);
}

bool QuestMenu::HasItem(uint32 questId) const
{
    for (QuestMenuItemList::const_iterator i = _questMenuItems.begin(); i != _questMenuItems.end(); ++i)
        if (i->QuestId == questId)
            return true;

    return false;
}

void QuestMenu::ClearMenu()
{
    _questMenuItems.clear();
}

void PlayerMenu::SendQuestGiverQuestList(QEmote eEmote, std::string const& Title, ObjectGuid npcGUID)
{
    WorldPackets::Quest::QuestGiverQuestList questList;
    questList.QuestGiverGUID = npcGUID;

    questList.GreetEmoteDelay = eEmote._Delay;
    questList.GreetEmoteType = eEmote._Emote;
    questList.Greeting = Title;

    for (uint32 i = 0; i < _questMenu.GetMenuItemCount(); ++i)
    {
        QuestMenuItem const& questMenuItem = _questMenu.GetItem(i);

        uint32 questID = questMenuItem.QuestId;

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            std::string title = quest->LogTitle;

            LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
            if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
                if (QuestTemplateLocale const* ql = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(ql->LogTitle, localeConstant, title);

            questList.GossipTexts.push_back(WorldPackets::Quest::GossipTextData(questID, questMenuItem.QuestIcon, quest->Level, quest->Flags, quest->FlagsEx, quest->IsRepeatable(), title));
        }
    }

    _session->SendPacket(questList.Write());
}

void PlayerMenu::SendQuestGiverStatus(uint32 questStatus, ObjectGuid npcGUID) const
{
    WorldPackets::Quest::QuestGiverStatus packet;
    packet.QuestGiver.Guid = npcGUID;
    packet.QuestGiver.Status = questStatus;
    _session->SendPacket(packet.Write());
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const* quest, ObjectGuid npcGUID, bool activateAccept, bool isAreaTrigger /*=false*/) const
{
    std::string questLogTitle = quest->LogTitle;
    std::string questDescription = quest->QuestDescription;
    std::string questLogDescription = quest->LogDescription;
    std::string questEndText = quest->AreaDescription;
    std::string portraitGiverText = quest->PortraitGiverText;
    std::string portraitGiverName = quest->PortraitGiverName;
    std::string portraitTurnInText = quest->PortraitTurnInText;
    std::string portraitTurnInName = quest->PortraitTurnInName;

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant != LOCALE_enUS && localeConstant != LOCALE_none)
        if (QuestTemplateLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->LogTitle, localeConstant, questLogTitle);
            ObjectMgr::GetLocaleString(localeData->QuestDescription, localeConstant, questDescription);
            ObjectMgr::GetLocaleString(localeData->LogDescription, localeConstant, questLogDescription);
            ObjectMgr::GetLocaleString(localeData->AreaDescription, localeConstant, questEndText);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverText, localeConstant, portraitGiverText);
            ObjectMgr::GetLocaleString(localeData->PortraitGiverName, localeConstant, portraitGiverName);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInText, localeConstant, portraitTurnInText);
            ObjectMgr::GetLocaleString(localeData->PortraitTurnInName, localeConstant, portraitTurnInName);
        }

    WorldPackets::Quest::QuestGiverQuestDetails packet;
    packet.QuestGiverGUID = npcGUID;
    packet.InformUnit = _session->GetPlayer()->GetDivider();
    packet.QuestID = quest->GetQuestId();
    packet.QuestTitle = questLogTitle;
    packet.LogDescription = questLogDescription;
    packet.DescriptionText = questDescription;
    packet.PortraitGiverText = portraitGiverText;
    packet.PortraitGiverName = portraitGiverName;
    packet.PortraitTurnInText = portraitTurnInText;
    packet.PortraitTurnInName = portraitTurnInName;
    packet.PortraitGiver = quest->QuestGiverPortrait;
    packet.PortraitTurnIn = quest->QuestTurnInPortrait;
    packet.DisplayPopup = activateAccept;

    //ToDo: CHECK IT ON WOD
    //bool isAutoLaunched = true;                             //activateAccept ? 1 : 0
    //ObjectGuid guid2 = isAutoLaunched ? ObjectGuid::Empty : npcGUID;

    //if (isAreaTrigger)
    //{
    //    isAutoLaunched = false;
    //    guid2 = npcGUID;
    //}
    //packet.StartCheat = isAutoLaunched;
    //offer.AutoLaunched = isAreaTrigger;

    packet.QuestFlags[0] = quest->GetFlags();
    packet.QuestFlags[1] = quest->FlagsEx;
    packet.SuggestedPartyMembers = quest->SuggestedPlayers;

    if (quest->SourceSpellID)
        packet.LearnSpells.push_back(quest->SourceSpellID);

    quest->BuildQuestRewards(packet.Rewards, _session->GetPlayer());

    packet.DescEmotes.resize(QUEST_EMOTE_COUNT);
    for (uint32 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        packet.DescEmotes[i].Type = quest->DetailsEmote[i];
        packet.DescEmotes[i].Delay = quest->DetailsEmoteDelay[i];
    }

    QuestObjectives const& objs = quest->GetObjectives();
    packet.Objectives.resize(objs.size());
    for (size_t i = 0; i < objs.size(); ++i)
    {
        packet.Objectives[i].ID = objs[i].ID;
        packet.Objectives[i].ObjectID = objs[i].ObjectID;
        packet.Objectives[i].Amount = objs[i].Amount;
        packet.Objectives[i].Type = objs[i].Type;
    }

    _session->SendPacket(packet.Write());
}

void PlayerMenu::SendQuestQueryResponse(uint32 questId) const
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest)
        return;

    std::string questLogTitle = quest->LogTitle;
    std::string questLogDescription = quest->LogDescription;
    std::string questDescription = quest->QuestDescription;
    std::string areaDescription = quest->AreaDescription;
    std::string questCompletionLog = quest->QuestCompletionLog;
    std::string portraitGiverText = quest->PortraitGiverText;
    std::string portraitGiverName = quest->PortraitGiverName;
    std::string portraitTurnInText = quest->PortraitTurnInText;
    std::string portraitTurnInName = quest->PortraitTurnInName;

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
    {
        if (QuestTemplateLocale const* questTemplateLocale = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(questTemplateLocale->LogTitle, localeConstant, questLogTitle);
            ObjectMgr::GetLocaleString(questTemplateLocale->LogDescription, localeConstant, questLogDescription);
            ObjectMgr::GetLocaleString(questTemplateLocale->QuestDescription, localeConstant, questDescription);
            ObjectMgr::GetLocaleString(questTemplateLocale->AreaDescription, localeConstant, areaDescription);
            ObjectMgr::GetLocaleString(questTemplateLocale->QuestCompletionLog, localeConstant, questCompletionLog);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitGiverText, localeConstant, portraitGiverText);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitGiverName, localeConstant, portraitGiverName);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitTurnInText, localeConstant, portraitTurnInText);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitTurnInName, localeConstant, portraitTurnInName);
        }
    }

    WorldPackets::Quest::QueryQuestInfoResponse packet;

    packet.Allow = true;
    packet.QuestID = quest->GetQuestId();

    packet.Info.QuestID = quest->GetQuestId();
    packet.Info.QuestType = quest->Type;
    packet.Info.QuestLevel = quest->Level;
    packet.Info.QuestPackageID = quest->PackageID;
    packet.Info.QuestMinLevel = quest->MinLevel;
    packet.Info.QuestSortID = quest->QuestSortID;
    packet.Info.QuestInfoID = quest->QuestInfoID;
    packet.Info.SuggestedGroupNum = quest->SuggestedPlayers;
    packet.Info.RewardNextQuest = quest->NextQuestIdChain;
    packet.Info.RewardXPDifficulty = quest->RewardXPDifficulty;
    packet.Info.RewardXPMultiplier = quest->RewardXPMultiplier;
    packet.Info.RewardMoneyMultiplier = quest->RewardMoneyMultiplier;

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
        packet.Info.RewardMoney = quest->RewardMoney;

    packet.Info.RewardMoneyDifficulty = quest->RewardMoneyDifficulty;
    packet.Info.RewardBonusMoney = quest->GetRewMoneyMaxLevel();
    packet.Info.RewardDisplaySpell = quest->RewardDisplaySpell;
    packet.Info.RewardSpell = quest->RewardDisplaySpell;

    packet.Info.RewardHonor = quest->RewardHonor;
    packet.Info.RewardKillHonor = quest->RewardKillHonor;

    packet.Info.StartItem = quest->SourceItemId;
    packet.Info.Flags = quest->GetFlags();
    packet.Info.FlagsEx = quest->FlagsEx;
    packet.Info.RewardTitle = quest->RewardTitleId;
    packet.Info.RewardTalents = quest->RewardTalents;
    packet.Info.RewardArenaPoints = quest->RewardArenaPoints;
    packet.Info.RewardSkillLineID = quest->RewardSkillId;
    packet.Info.RewardNumSkillUps = quest->RewardSkillPoints;
    packet.Info.RewardFactionFlags = quest->RewardFactionFlags;
    packet.Info.PortraitGiver = quest->QuestGiverPortrait;
    packet.Info.PortraitTurnIn = quest->QuestTurnInPortrait;

    if (!quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
    {
        for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        {
            packet.Info.RewardItems[i] = quest->RewardItemId[i];
            packet.Info.RewardAmount[i] = quest->RewardItemCount[i];
        }
        for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        {
            packet.Info.UnfilteredChoiceItems[i].ItemID = quest->RewardChoiceItemId[i];
            packet.Info.UnfilteredChoiceItems[i].Quantity = quest->RewardChoiceItemCount[i];
        }
    }

    for (uint8 i = 0; i < QUEST_REWARD_REPUTATIONS_COUNT; ++i)
    {
        packet.Info.RewardFactionID[i] = quest->RewardFactionId[i];
        packet.Info.RewardFactionValue[i] = quest->RewardFactionValue[i];
        packet.Info.RewardFactionOverride[i] = quest->RewardFactionOverride[i];
    }

    packet.Info.POIContinent = quest->POIContinent;
    packet.Info.POIx = quest->POIx;
    packet.Info.POIy = quest->POIy;
    packet.Info.POIPriority = quest->POIPriority;
    packet.Info.LogTitle = questLogTitle;
    packet.Info.LogDescription = questLogDescription;
    packet.Info.QuestDescription = questDescription;
    packet.Info.AreaDescription = areaDescription;
    packet.Info.QuestCompletionLog = questCompletionLog;
    packet.Info.AllowableRaces = quest->AllowableRaces;

    for (QuestObjective const& questObjective : quest->GetObjectives())
    {
        packet.Info.Objectives.push_back(questObjective);

        if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
            if (QuestObjectivesLocale const* questObjectivesLocale = sObjectMgr->GetQuestObjectivesLocale(questObjective.ID))
                ObjectMgr::GetLocaleString(questObjectivesLocale->Description, localeConstant, packet.Info.Objectives.back().Description);
    }

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        packet.Info.RewardCurrencyID[i] = quest->RewardCurrencyId[i];
        packet.Info.RewardCurrencyQty[i] = quest->RewardCurrencyCount[i];
    }

    packet.Info.PortraitGiverText = portraitGiverText;
    packet.Info.PortraitGiverName = portraitGiverName;
    packet.Info.PortraitTurnInText = portraitTurnInText;
    packet.Info.PortraitTurnInName = portraitTurnInName;

    packet.Info.AcceptedSoundKitID = quest->SoundAccept;
    packet.Info.CompleteSoundKitID = quest->SoundTurnIn;
    packet.Info.AreaGroupID = quest->AreaGroupID;
    packet.Info.TimeAllowed = quest->LimitTime;

    _session->SendPacket(packet.Write());
}

void PlayerMenu::SendQuestGiverOfferReward(Quest const* quest, ObjectGuid npcGUID, bool enableNext) const
{
    Player* player = _session->GetPlayer();

    std::string questTitle = quest->LogTitle;
    std::string questOfferRewardText = quest->OfferRewardText;
    std::string portraitGiverText = quest->PortraitGiverText;
    std::string portraitGiverName = quest->PortraitGiverName;
    std::string portraitTurnInText = quest->PortraitTurnInText;
    std::string portraitTurnInName = quest->PortraitTurnInName;

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
    {
        if (QuestTemplateLocale const* questTemplateLocale = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(questTemplateLocale->LogTitle, localeConstant, questTitle);
            ObjectMgr::GetLocaleString(questTemplateLocale->OfferRewardText, localeConstant, questOfferRewardText);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitGiverText, localeConstant, portraitGiverText);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitGiverName, localeConstant, portraitGiverName);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitTurnInText, localeConstant, portraitTurnInText);
            ObjectMgr::GetLocaleString(questTemplateLocale->PortraitTurnInName, localeConstant, portraitTurnInName);
        }
    }

    WorldPackets::Quest::QuestGiverOfferRewardMessage packet;
    WorldPackets::Quest::QuestGiverOfferReward& offer = packet.QuestData;

    quest->BuildQuestRewards(offer.Rewards, _session->GetPlayer());
    offer.QuestGiverGUID = npcGUID;

    // Is there a better way? what about game objects?
    if (Creature const* creature = sObjectAccessor->GetCreature(*_session->GetPlayer(), npcGUID))
        offer.QuestGiverCreatureID = creature->GetCreatureTemplate()->Entry;

    offer.QuestID = quest->GetQuestId();
    offer.AutoLaunched = enableNext;
    offer.SuggestedPartyMembers = quest->SuggestedPlayers;

    for (uint32 i = 0; i < QUEST_EMOTE_COUNT && quest->OfferRewardEmote[i]; ++i)
        offer.Emotes.push_back(WorldPackets::Quest::QuestDescEmote(quest->OfferRewardEmote[i], quest->OfferRewardEmoteDelay[i]));

    offer.QuestFlags[0] = quest->GetFlags();
    offer.QuestFlags[1] = quest->FlagsEx;

    packet.QuestTitle = questTitle;
    packet.RewardText = questOfferRewardText;
    packet.PortraitTurnIn = quest->QuestTurnInPortrait;
    packet.PortraitGiver = quest->QuestGiverPortrait;
    packet.PortraitGiverText = portraitGiverText;
    packet.PortraitGiverName = portraitGiverName;
    packet.PortraitTurnInText = portraitTurnInText;
    packet.PortraitTurnInName = portraitTurnInName;
    packet.QuestPackageID = quest->PackageID;

    _session->SendPacket(packet.Write());
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const* quest, ObjectGuid npcGUID, bool canComplete, bool closeOnCancel) const
{
    // We can always call to RequestItems, but this packet only goes out if there are actually
    // items.  Otherwise, we'll skip straight to the OfferReward

    std::string questTitle = quest->LogTitle;
    std::string requestItemsText = quest->RequestItemsText;

    LocaleConstant localeConstant = _session->GetSessionDbLocaleIndex();
    if (localeConstant >= LOCALE_enUS && localeConstant != LOCALE_none)
        if (QuestTemplateLocale const* questTemplateLocale = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(questTemplateLocale->LogTitle, localeConstant, questTitle);
            ObjectMgr::GetLocaleString(questTemplateLocale->RequestItemsText, localeConstant, requestItemsText);
        }

    if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER) && canComplete)
    {
        SendQuestGiverOfferReward(quest, npcGUID, true);
        return;
    }

    WorldPackets::Quest::QuestGiverRequestItems packet;
    packet.QuestGiverGUID = npcGUID;

    // Is there a better way? what about game objects?
    if (Creature const* creature = sObjectAccessor->GetCreature(*_session->GetPlayer(), npcGUID))
        packet.QuestGiverCreatureID = creature->GetCreatureTemplate()->Entry;

    packet.QuestID = quest->GetQuestId();

    if (canComplete)
    {
        packet.CompEmoteDelay = quest->EmoteOnCompleteDelay;
        packet.CompEmoteType = quest->EmoteOnComplete;
    }
    else
    {
        packet.CompEmoteDelay = quest->EmoteOnIncompleteDelay;
        packet.CompEmoteType = quest->EmoteOnIncomplete;
    }

    packet.QuestFlags[0] = quest->GetFlags();
    packet.QuestFlags[1] = quest->FlagsEx;
    packet.SuggestPartyMembers = quest->SuggestedPlayers;
    packet.StatusFlags = 0xDF; // Unk, send common value

    packet.MoneyToGet = 0;
    for (QuestObjective const& obj : quest->GetObjectives())
    {
        switch (obj.Type)
        {
            case QUEST_OBJECTIVE_ITEM:
                packet.Collect.push_back(WorldPackets::Quest::QuestObjectiveCollect(obj.ObjectID, obj.Amount));
                break;
            case QUEST_OBJECTIVE_CURRENCY:
                packet.Currency.push_back(WorldPackets::Quest::QuestCurrency(obj.ObjectID, obj.Amount));
                break;
            case QUEST_OBJECTIVE_MONEY:
                packet.MoneyToGet += obj.Amount;
                break;
            default:
                break;
        }
    }

    packet.AutoLaunched = closeOnCancel;
    packet.QuestTitle = questTitle;
    packet.CompletionText = requestItemsText;

    _session->SendPacket(packet.Write());
}
