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
#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "AuctionHouseMgr.h"
#include "Item.h"
#include "Language.h"
#include "Log.h"
#include "ItemPackets.h"
#include "AuctionHousePackets.h"

enum eAuctionHouse
{
    AH_MINIMUM_DEPOSIT = 100,
};

AuctionHouseMgr::AuctionHouseMgr()
{ }

AuctionHouseMgr::~AuctionHouseMgr()
{
    for (ItemMap::iterator itr = mAitems.begin(); itr != mAitems.end(); ++itr)
        delete itr->second;
}

AuctionHouseObject* AuctionHouseMgr::GetAuctionsMap(uint32 factionTemplateId)
{
    if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
        return &mNeutralAuctions;

    // team have linked auction houses
    FactionTemplateEntry const* u_entry = sFactionTemplateStore.LookupEntry(factionTemplateId);
    if (!u_entry)
        return &mNeutralAuctions;
    else if (u_entry->Mask & FACTION_MASK_ALLIANCE)
        return &mAllianceAuctions;
    else if (u_entry->Mask & FACTION_MASK_HORDE)
        return &mHordeAuctions;
    else
        return &mNeutralAuctions;
}

uint32 AuctionHouseMgr::GetAuctionDeposit(AuctionHouseEntry const* entry, uint32 time, Item* pItem, uint32 count)
{
    uint32 MSV = pItem->GetTemplate()->SellPrice;

    if (MSV <= 0)
        return AH_MINIMUM_DEPOSIT;

    float multiplier = CalculatePct(float(entry->DepositRate), 3);
    uint32 timeHr = (((time / 60) / 60) / 12);
    uint32 deposit = uint32(((multiplier * MSV * count / 3) * timeHr * 3) * sWorld->getRate(RATE_AUCTION_DEPOSIT));

    sLog->outDebug(LOG_FILTER_AUCTIONHOUSE, "MSV:        %u", MSV);
    sLog->outDebug(LOG_FILTER_AUCTIONHOUSE, "Items:      %u", count);
    sLog->outDebug(LOG_FILTER_AUCTIONHOUSE, "Multiplier: %f", multiplier);
    sLog->outDebug(LOG_FILTER_AUCTIONHOUSE, "Deposit:    %u", deposit);

    if (deposit < AH_MINIMUM_DEPOSIT)
        return AH_MINIMUM_DEPOSIT;
    else
        return deposit;
}

//does not clear ram
void AuctionHouseMgr::SendAuctionWonMail(AuctionEntry* auction, SQLTransaction& trans)
{
    Item* pItem = GetAItem(auction->itemGUIDLow);
    if (!pItem)
        return;

    uint32 bidder_accId = 0;
    ObjectGuid bidderGuid = ObjectGuid::Create<HighGuid::Player>(auction->bidder);
    Player* bidder = ObjectAccessor::FindPlayer(bidderGuid);
    // data for gm.log
    if (sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
    {
        uint32 bidder_security = 0;
        std::string bidder_name;
        if (bidder)
        {
            bidder_accId = bidder->GetSession()->GetAccountId();
            bidder_security = bidder->GetSession()->GetSecurity();
            bidder_name = bidder->GetName();
        }
        else
        {
            bidder_accId = ObjectMgr::GetPlayerAccountIdByGUID(bidderGuid);
            bidder_security = AccountMgr::GetSecurity(bidder_accId, realm.Id.Realm);

            if (!AccountMgr::IsPlayerAccount(bidder_security)) // not do redundant DB requests
            {
                if (!ObjectMgr::GetPlayerNameByGUID(bidderGuid, bidder_name))
                    bidder_name = sObjectMgr->GetTrinityStringForDBCLocale(LANG_UNKNOWN);
            }
        }
        if (!AccountMgr::IsPlayerAccount(bidder_security))
        {
            std::string owner_name;
            ObjectGuid ownerGuid = ObjectGuid::Create<HighGuid::Player>(auction->owner);
            if (!ObjectMgr::GetPlayerNameByGUID(ownerGuid, owner_name))
                owner_name = sObjectMgr->GetTrinityStringForDBCLocale(LANG_UNKNOWN);

            uint32 owner_accid = ObjectMgr::GetPlayerAccountIdByGUID(ownerGuid);

            sLog->outCommand(bidder_accId, "GM %s (Account: %u) won item in auction: %s (Entry: %u Count: %u) and pay money: %u. Original owner %s (Account: %u)",
                bidder_name.c_str(), bidder_accId, pItem->GetTemplate()->Name1.c_str(), pItem->GetEntry(), pItem->GetCount(), auction->bid, owner_name.c_str(), owner_accid);
        }
    }

    // receiver exist
    if (bidder || bidder_accId)
    {
        // set owner to bidder (to prevent delete item with sender char deleting)
        // owner in `data` will set at mail receive and item extracting
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_OWNER);
        stmt->setUInt64(0, auction->bidder);
        stmt->setUInt64(1, pItem->GetGUIDLow());
        trans->Append(stmt);

        if (bidder)
        {
            bidder->GetSession()->SendAuctionWonNotification(auction, pItem);
            // FIXME: for offline player need also
            bidder->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS, 1);
        }

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_WON), AuctionEntry::BuildAuctionMailBody(auction->owner, auction->bid, auction->buyout, 0, 0))
            .AddItem(pItem)
            .SendMailTo(trans, MailReceiver(bidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
    }
}

void AuctionHouseMgr::SendAuctionSalePendingMail(AuctionEntry* auction, SQLTransaction& trans)
{
    ObjectGuid owner_guid = ObjectGuid::Create<HighGuid::Player>(auction->owner);
    Player* owner = ObjectAccessor::FindPlayer(owner_guid);
    uint32 owner_accId = ObjectMgr::GetPlayerAccountIdByGUID(owner_guid);
    // owner exist (online or offline)
    if (owner || owner_accId)
        MailDraft(auction->BuildAuctionMailSubject(AUCTION_SALE_PENDING), AuctionEntry::BuildAuctionMailBody(auction->bidder, auction->bid, auction->buyout, auction->deposit, auction->GetAuctionCut()))
            .SendMailTo(trans, MailReceiver(owner, auction->owner), auction, MAIL_CHECK_MASK_COPIED);
}

//call this method to send mail to auction owner, when auction is successful, it does not clear ram
void AuctionHouseMgr::SendAuctionSuccessfulMail(AuctionEntry* auction, SQLTransaction& trans)
{
    ObjectGuid owner_guid = ObjectGuid::Create<HighGuid::Player>(auction->owner);
    Player* owner = ObjectAccessor::FindPlayer(owner_guid);
    uint32 owner_accId = ObjectMgr::GetPlayerAccountIdByGUID(owner_guid);
    Item* item = GetAItem(auction->itemGUIDLow);
    if (item && (owner || owner_accId))
    {
        uint64 profit = auction->bid + auction->deposit - auction->GetAuctionCut();

        //FIXME: what do if owner offline
        if (owner)
        {
            owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS, profit);
            owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD, auction->bid);
            owner->GetSession()->SendAuctionClosedNotification(auction, (float)sWorld->getIntConfig(CONFIG_MAIL_DELIVERY_DELAY), true, item);
        }

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_SUCCESSFUL), AuctionEntry::BuildAuctionMailBody(auction->bidder, auction->bid, auction->buyout, auction->deposit, auction->GetAuctionCut()))
            .AddMoney(profit)
            .SendMailTo(trans, MailReceiver(owner, auction->owner), auction, MAIL_CHECK_MASK_COPIED, sWorld->getIntConfig(CONFIG_MAIL_DELIVERY_DELAY));
    }
}

//does not clear ram
void AuctionHouseMgr::SendAuctionExpiredMail(AuctionEntry* auction, SQLTransaction& trans)
{
    //return an item in auction to its owner by mail
    Item* pItem = GetAItem(auction->itemGUIDLow);
    if (!pItem)
        return;

    ObjectGuid owner_guid = ObjectGuid::Create<HighGuid::Player>(auction->owner);
    Player* owner = ObjectAccessor::FindPlayer(owner_guid);
    uint32 owner_accId = ObjectMgr::GetPlayerAccountIdByGUID(owner_guid);
    // owner exist
    if (owner || owner_accId)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_OWNER);
        stmt->setUInt64(0, auction->owner);
        stmt->setUInt64(1, pItem->GetGUIDLow());
        trans->Append(stmt);

        if (owner)
            owner->GetSession()->SendAuctionClosedNotification(auction, 0.0f, false, pItem);

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_EXPIRED), AuctionEntry::BuildAuctionMailBody(0, 0, auction->buyout, auction->deposit, 0))
            .AddItem(pItem)
            .SendMailTo(trans, MailReceiver(owner, auction->owner), auction, MAIL_CHECK_MASK_COPIED, 0);
    }
}

//this function sends mail to old bidder
void AuctionHouseMgr::SendAuctionOutbiddedMail(AuctionEntry* auction, uint64 const& newPrice, Player* newBidder, SQLTransaction& trans)
{
    ObjectGuid oldBidder_guid = ObjectGuid::Create<HighGuid::Player>(auction->bidder);
    Player* oldBidder = ObjectAccessor::FindPlayer(oldBidder_guid);
    Item* item = GetAItem(auction->itemGUIDLow);
    uint32 oldBidder_accId = 0;
    if (!oldBidder)
        oldBidder_accId = ObjectMgr::GetPlayerAccountIdByGUID(oldBidder_guid);

    if (oldBidder || oldBidder_accId)
    {
        if (oldBidder && item)
            oldBidder->GetSession()->SendAuctionOutBidNotification(auction, item);

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_OUTBIDDED), AuctionEntry::BuildAuctionMailBody(auction->owner, auction->bid, auction->buyout, auction->deposit, auction->GetAuctionCut()))
            .AddMoney(auction->bid)
            .SendMailTo(trans, MailReceiver(oldBidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
    }
}

//this function sends mail, when auction is cancelled to old bidder
void AuctionHouseMgr::SendAuctionCancelledToBidderMail(AuctionEntry* auction, SQLTransaction& trans, Item* item)
{
    ObjectGuid bidder_guid = ObjectGuid::Create<HighGuid::Player>(auction->bidder);
    Player* bidder = ObjectAccessor::FindPlayer(bidder_guid);

    uint32 bidder_accId = 0;
    if (!bidder)
        bidder_accId = ObjectMgr::GetPlayerAccountIdByGUID(bidder_guid);

    if (bidder)
    {
        bidder->GetSession()->SendAuctionOutBidNotification(auction, item);
        //bidder->GetSession()->SendAuctionRemovedNotification(auction->Id, auction->itemEntry, item->GetItemRandomPropertyId());
    }

    // bidder exist
    if (bidder || bidder_accId)
        MailDraft(auction->BuildAuctionMailSubject(AUCTION_CANCELLED_TO_BIDDER), AuctionEntry::BuildAuctionMailBody(auction->owner, auction->bid, auction->buyout, auction->deposit, 0))
            .AddMoney(auction->bid)
            .SendMailTo(trans, MailReceiver(bidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
}

void AuctionHouseMgr::LoadAuctionItems()
{
    uint32 oldMSTime = getMSTime();

    // data needs to be at first place for Item::LoadFromDB
    //          0           1               2           3       4       5       6           7                   8           9           10      11              12          13              14              15          16
    //SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, durability, playedTime, text, transmogrification, upgradeId, enchantIllusion, bonusListIDs, itemguid, itemEntry
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_AUCTION_ITEMS);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 auction items. DB table `auctionhouse` or `item_instance` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ObjectGuid::LowType item_guid = fields[15].GetUInt64();
        uint32 itemEntry    = fields[16].GetUInt32();

        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemEntry);
        if (!proto)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AuctionHouseMgr::LoadAuctionItems: Unknown item (GUID: " UI64FMTD " id: #%u) in auction, skipped.", item_guid, itemEntry);
            continue;
        }

        Item* item = NewItemOrBag(proto);
        if (!item->LoadFromDB(item_guid, ObjectGuid::Empty, fields, itemEntry))
        {
            delete item;
            continue;
        }
        AddAItem(item);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u auction items in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void AuctionHouseMgr::LoadAuctions()
{
    uint32 oldMSTime = getMSTime();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_AUCTIONS);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 auctions. DB table `auctionhouse` is empty.");

        return;
    }

    uint32 count = 0;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    do
    {
        Field* fields = result->Fetch();

        AuctionEntry* aItem = new AuctionEntry();
        if (!aItem->LoadFromDB(fields))
        {
            aItem->DeleteFromDB(trans);
            delete aItem;
            continue;
        }

        GetAuctionsMap(aItem->factionTemplateId)->AddAuction(aItem);
        count++;
    } while (result->NextRow());

    CharacterDatabase.CommitTransaction(trans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u auctions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void AuctionHouseMgr::AddAItem(Item* it)
{
    ASSERT(it);
    ASSERT(mAitems.find(it->GetGUIDLow()) == mAitems.end());
    mAitems[it->GetGUIDLow()] = it;
}

bool AuctionHouseMgr::RemoveAItem(ObjectGuid::LowType const& id)
{
    ItemMap::iterator i = mAitems.find(id);
    if (i == mAitems.end())
        return false;

    mAitems.erase(i);
    return true;
}

void AuctionHouseMgr::Update()
{
    mHordeAuctions.Update();
    mAllianceAuctions.Update();
    mNeutralAuctions.Update();
}

AuctionHouseEntry const* AuctionHouseMgr::GetAuctionHouseEntry(uint32 factionTemplateId)
{
    uint32 houseid = 7; // goblin auction house

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
    {
        //FIXME: found way for proper auctionhouse selection by another way
        // AuctionHouse.dbc have faction field with _player_ factions associated with auction house races.
        // but no easy way convert creature faction to player race faction for specific city
        switch (factionTemplateId)
        {
            case   12: houseid = 1; break; // human
            case   29: houseid = 6; break; // orc, and generic for horde
            case   55: houseid = 2; break; // dwarf, and generic for alliance
            case   68: houseid = 4; break; // undead
            case   80: houseid = 3; break; // n-elf
            case  104: houseid = 5; break; // trolls
            case  120: houseid = 7; break; // booty bay, neutral
            case  474: houseid = 7; break; // gadgetzan, neutral
            case  855: houseid = 7; break; // everlook, neutral
            case 1604: houseid = 6; break; // b-elfs,
            default:                       // for unknown case
            {
                FactionTemplateEntry const* u_entry = sFactionTemplateStore.LookupEntry(factionTemplateId);
                if (!u_entry)
                    houseid = 7; // goblin auction house
                else if (u_entry->Mask & FACTION_MASK_ALLIANCE)
                    houseid = 1; // human auction house
                else if (u_entry->Mask & FACTION_MASK_HORDE)
                    houseid = 6; // orc auction house
                else
                    houseid = 7; // goblin auction house
                break;
            }
        }
    }

    return sAuctionHouseStore.LookupEntry(houseid);
}

void AuctionHouseObject::AddAuction(AuctionEntry* auction)
{
    ASSERT(auction);

    AuctionsMap[auction->Id] = auction;
    sScriptMgr->OnAuctionAdd(this, auction);
}

bool AuctionHouseObject::RemoveAuction(AuctionEntry* auction, uint32 /*itemEntry*/)
{
    bool wasInMap = AuctionsMap.erase(auction->Id) ? true : false;

    sScriptMgr->OnAuctionRemove(this, auction);

    // we need to delete the entry, it is not referenced any more
    delete auction;
    return wasInMap;
}

void AuctionHouseObject::Update()
{
    time_t curTime = sWorld->GetGameTime();
    ///- Handle expired auctions

    // If storage is empty, no need to update. next == NULL in this case.
    if (AuctionsMap.empty())
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_AUCTION_BY_TIME);
    stmt->setUInt32(0, (uint32)curTime + 60);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    do
    {
        // from auctionhousehandler.cpp, creates auction pointer & player pointer
        AuctionEntry* auction = GetAuction(result->Fetch()->GetUInt32());

        if (!auction)
            continue;

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        ///- Either cancel the auction if there was no bidder
        if (auction->bidder == 0)
        {
            sAuctionMgr->SendAuctionExpiredMail(auction, trans);
            sScriptMgr->OnAuctionExpire(this, auction);
        }
        ///- Or perform the transaction
        else
        {
            //we should send an "item sold" message if the seller is online
            //we send the item to the winner
            //we send the money to the seller
            sAuctionMgr->SendAuctionSuccessfulMail(auction, trans);
            sAuctionMgr->SendAuctionWonMail(auction, trans);
            sScriptMgr->OnAuctionSuccessful(this, auction);
        }

        uint32 itemEntry = auction->itemEntry;

        ///- In any case clear the auction
        auction->DeleteFromDB(trans);
        CharacterDatabase.CommitTransaction(trans);

        RemoveAuction(auction, itemEntry);
        sAuctionMgr->RemoveAItem(auction->itemGUIDLow);
    }
    while (result->NextRow());
}

void AuctionHouseObject::BuildListBidderItems(WorldPackets::AuctionHouse::AuctionListBidderItemsResult& packet, Player* player, uint32& totalcount)
{
    for (AuctionEntryMap::const_iterator itr = AuctionsMap.begin(); itr != AuctionsMap.end(); ++itr)
    {
        AuctionEntry* Aentry = itr->second;
        if (Aentry && Aentry->bidder == player->GetGUIDLow())
        {
            itr->second->BuildAuctionInfo(packet.Items, false);
            ++totalcount;
        }
    }
}

void AuctionHouseObject::BuildListOwnerItems(WorldPackets::AuctionHouse::AuctionListOwnerItemsResult& packet, Player* player, uint32& totalcount)
{
    for (AuctionEntryMap::const_iterator itr = AuctionsMap.begin(); itr != AuctionsMap.end(); ++itr)
    {
        AuctionEntry* Aentry = itr->second;
        if (Aentry && Aentry->owner == player->GetGUIDLow())
        {
            Aentry->BuildAuctionInfo(packet.Items, false);
            ++totalcount;
        }
    }
}

void AuctionHouseObject::BuildListAuctionItems(WorldPackets::AuctionHouse::AuctionListItemsResult& packet, Player* player,
    std::wstring const& wsearchedname, uint32 listfrom, uint8 levelmin, uint8 levelmax, uint8 usable,
    uint32 inventoryType, uint32 itemClass, uint32 itemSubClass, uint32 quality, uint32& totalcount)
{
    LocaleConstant localeConstant = player->GetSession()->GetSessionDbcLocale();

    for (AuctionEntryMap::const_iterator itr = AuctionsMap.begin(); itr != AuctionsMap.end(); ++itr)
    {
        AuctionEntry* Aentry = itr->second;
        Item* item = sAuctionMgr->GetAItem(Aentry->itemGUIDLow);
        if (!item)
            continue;

        ItemTemplate const* proto = item->GetTemplate();

        if (itemClass != -1 && proto->Class != itemClass)
            continue;

        if (itemSubClass != -1 && proto->SubClass != itemSubClass)
            continue;

        if (inventoryType != -1 && proto->GetInventoryType() != inventoryType)
            continue;

        if (quality != -1 && proto->Quality != quality)
            continue;

        if (levelmin != 0 && (proto->RequiredLevel < levelmin || (levelmax != 0 && proto->RequiredLevel > levelmax)))
            continue;

        if (usable != 0 && player->CanUseItem(item) != EQUIP_ERR_OK)
            continue;

        // Allow search by suffix (ie: of the Monkey) or partial name (ie: Monkey)
        // No need to do any of this if no search term was entered
        if (!wsearchedname.empty())
        {
            std::string name = proto->Name1;
            if (name.empty())
                continue;

            // local name
            if (localeConstant != LOCALE_enUS && localeConstant != LOCALE_none)
                if (ItemLocale const* itemLocale = sObjectMgr->GetItemLocale(proto->ItemId))
                    ObjectMgr::GetLocaleString(itemLocale->Name, localeConstant, name);

            // DO NOT use GetItemEnchantMod(proto->RandomProperty) as it may return a result
            //  that matches the search but it may not equal item->GetItemRandomPropertyId()
            //  used in BuildAuctionInfo() which then causes wrong items to be listed
            int32 propRefID = item->GetItemRandomPropertyId();

            if (propRefID)
            {
                const char* suffix = nullptr;
                if (propRefID < 0)
                {
                    if (ItemRandomSuffixEntry const* itemRandSuffix = sItemRandomSuffixStore.LookupEntry(-propRefID))
                        suffix = itemRandSuffix->Name->Str[localeConstant];
                }
                else if (ItemRandomPropertiesEntry const* itemRandProp = sItemRandomPropertiesStore.LookupEntry(propRefID))
                        suffix = itemRandProp->Name->Str[localeConstant];

                // dbc local name
                if (suffix)
                {
                    name += ' ';
                    name += suffix;
                }
            }

            // Perform the search (with or without suffix)
            if (!Utf8FitTo(name, wsearchedname))
                continue;
        }

        // Add the item if no search term or if entered search term was found
        if (packet.Items.size() < 50 && totalcount >= listfrom)
            Aentry->BuildAuctionInfo(packet.Items, true);

        ++totalcount;
    }
}

void AuctionEntry::BuildAuctionInfo(std::vector<WorldPackets::AuctionHouse::AuctionItem>& items, bool listAuctionItems) const
{
    Item* item = sAuctionMgr->GetAItem(itemGUIDLow);
    if (!item)
        return;

    WorldPackets::AuctionHouse::AuctionItem auctionItem;

    auctionItem.AuctionItemID = Id;
    auctionItem.Item.Initialize(item);
    auctionItem.BuyoutPrice = buyout;
    auctionItem.CensorBidInfo = false;
    auctionItem.CensorServerSideInfo = listAuctionItems;
    auctionItem.Charges = item->GetSpellCharges();
    auctionItem.Count = item->GetCount();
    auctionItem.DeleteReason = 0; // Always 0 ?
    auctionItem.DurationLeft = (expire_time - time(nullptr)) * IN_MILLISECONDS;
    auctionItem.EndTime = expire_time;
    auctionItem.Flags = 0; // todo
    auctionItem.ItemGuid = item->GetGUID();
    auctionItem.MinBid = startbid;
    auctionItem.Owner = ObjectGuid::Create<HighGuid::Player>(owner);
    auctionItem.OwnerAccountID = ObjectGuid::Create<HighGuid::WowAccount>(ObjectMgr::GetPlayerAccountIdByGUID(auctionItem.Owner));
    if (bidder)
    {
        auctionItem.MinIncrement = GetAuctionOutBid();
        auctionItem.Bidder = ObjectGuid::Create<HighGuid::Player>(bidder);
        auctionItem.BidAmount = bid;
    }

    for (uint8 i = 0; i < MAX_INSPECTED_ENCHANTMENT_SLOT; i++)
        if (uint32 enchantmentID = item->GetEnchantmentId(EnchantmentSlot(i)))
            auctionItem.Enchantments.emplace_back(enchantmentID, item->GetEnchantmentDuration(EnchantmentSlot(i)), item->GetEnchantmentCharges(EnchantmentSlot(i)), i);

    items.emplace_back(auctionItem);
}


uint64 AuctionEntry::GetAuctionCut() const
{
    int64 cut = int64(CalculatePct(bid, auctionHouseEntry->ConsignmentRate) * sWorld->getRate(RATE_AUCTION_CUT));
    return cut > 0 ? cut : 0;
}

/// the sum of outbid is (1% from current bid)*5, if bid is very small, it is 1c
uint64 AuctionEntry::GetAuctionOutBid() const
{
    uint64 outbid = CalculatePct(bid, 5);
    return outbid ? outbid : 1;
}

void AuctionEntry::DeleteFromDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_AUCTION);
    stmt->setUInt32(0, Id);
    trans->Append(stmt);
}

void AuctionEntry::SaveToDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_AUCTION);
    stmt->setUInt32(0, Id);
    stmt->setUInt64(1, auctioneer);
    stmt->setUInt64(2, itemGUIDLow);
    stmt->setUInt64(3, owner);
    stmt->setInt64 (4, int64(buyout));
    stmt->setUInt64(5, int64(expire_time));
    stmt->setUInt64(6, bidder);
    stmt->setInt64 (7, int64(bid));
    stmt->setInt64 (8, int64(startbid));
    stmt->setInt64 (9, int64(deposit));
    trans->Append(stmt);
}

bool AuctionEntry::LoadFromDB(Field* fields)
{
    uint8 index = 0;

    Id          = fields[index++].GetUInt32();
    auctioneer  = fields[index++].GetUInt64();
    itemGUIDLow = fields[index++].GetUInt64();
    itemEntry   = fields[index++].GetUInt32();
    itemCount   = fields[index++].GetUInt32();
    owner       = fields[index++].GetUInt64();
    buyout      = fields[index++].GetUInt64();
    expire_time = fields[index++].GetUInt32();
    bidder      = fields[index++].GetUInt64();
    bid         = fields[index++].GetUInt64();
    startbid    = fields[index++].GetUInt64();
    deposit     = fields[index++].GetUInt64();

    CreatureData const* auctioneerData = sObjectMgr->GetCreatureData(auctioneer);
    if (!auctioneerData)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has not a existing auctioneer (GUID : %u)", Id, auctioneer);
        return false;
    }

    CreatureTemplate const* auctioneerInfo = sObjectMgr->GetCreatureTemplate(auctioneerData->id);
    if (!auctioneerInfo)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has not a existing auctioneer (GUID : %u Entry: %u)", Id, auctioneer, auctioneerData->id);
        return false;
    }

    factionTemplateId = auctioneerInfo->faction;
    auctionHouseEntry = AuctionHouseMgr::GetAuctionHouseEntry(factionTemplateId);
    if (!auctionHouseEntry)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has auctioneer (GUID : %u Entry: %u) with wrong faction %u", Id, auctioneer, auctioneerData->id, factionTemplateId);
        return false;
    }

    // check if sold item exists for guid
    // and itemEntry in fact (GetAItem will fail if problematic in result check in AuctionHouseMgr::LoadAuctionItems)
    if (!sAuctionMgr->GetAItem(itemGUIDLow))
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has not a existing item : %u", Id, itemGUIDLow);
        return false;
    }
    return true;
}

void AuctionHouseMgr::DeleteExpiredAuctionsAtStartup()
{
    // Deletes expired auctions. Should be called at server start before loading auctions.

    // DO NOT USE after auctions are already loaded since this deletes from the DB
    //  and assumes the auctions HAVE NOT been loaded into a list or AuctionEntryMap yet

    uint32 oldMSTime = getMSTime();
    uint32 expirecount = 0;
    time_t curTime = sWorld->GetGameTime();

    // Query the DB to see if there are any expired auctions
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_AUCTIONS);
    stmt->setUInt32(0, (uint32)curTime + 60);
    PreparedQueryResult expAuctions = CharacterDatabase.Query(stmt);

    if (!expAuctions)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> No expired auctions to delete");

        return;
    }

    do
    {
        Field* fields = expAuctions->Fetch();

        AuctionEntry* auction = new AuctionEntry();

        // Can't use LoadFromDB() because it assumes the auction map is loaded
        if (!auction->LoadFromFieldList(fields))
        {
            // For some reason the record in the DB is broken (possibly corrupt
            //  faction info). Delete the object and move on.
            delete auction;
            continue;
        }

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        if (auction->bidder == 0)
        {
            // Cancel the auction, there was no bidder
            sAuctionMgr->SendAuctionExpiredMail(auction, trans);
        }
        else
        {
            // Send the item to the winner and money to seller
            sAuctionMgr->SendAuctionSuccessfulMail(auction, trans);
            sAuctionMgr->SendAuctionWonMail(auction, trans);
        }

        // Call the appropriate AuctionHouseObject script
        //  ** Do we need to do this while core is still loading? **
        sScriptMgr->OnAuctionExpire(GetAuctionsMap(auction->factionTemplateId), auction);

        // Delete the auction from the DB
        auction->DeleteFromDB(trans);
        CharacterDatabase.CommitTransaction(trans);

        // Release memory
        delete auction;
        ++expirecount;

    } while (expAuctions->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Deleted %u expired auctions in %u ms", expirecount, GetMSTimeDiffToNow(oldMSTime));


}

bool AuctionEntry::LoadFromFieldList(Field* fields)
{
    // Loads an AuctionEntry item from a field list. Unlike "LoadFromDB()", this one
    //  does not require the AuctionEntryMap to have been loaded with items. It simply
    //  acts as a wrapper to fill out an AuctionEntry struct from a field list

    uint8 index = 0;

    Id          = fields[index++].GetUInt32();
    auctioneer  = fields[index++].GetUInt64();
    itemGUIDLow = fields[index++].GetUInt64();
    itemEntry   = fields[index++].GetUInt32();
    itemCount   = fields[index++].GetUInt32();
    owner       = fields[index++].GetUInt64();
    buyout      = fields[index++].GetUInt64();
    expire_time = fields[index++].GetUInt32();
    bidder      = fields[index++].GetUInt64();
    bid         = fields[index++].GetUInt64();
    startbid    = fields[index++].GetUInt64();
    deposit     = fields[index++].GetUInt64();

    CreatureData const* auctioneerData = sObjectMgr->GetCreatureData(auctioneer);
    if (!auctioneerData)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::LoadFromFieldList() - Auction %u has not a existing auctioneer (GUID : %u)", Id, auctioneer);
        return false;
    }

    CreatureTemplate const* auctioneerInfo = sObjectMgr->GetCreatureTemplate(auctioneerData->id);
    if (!auctioneerInfo)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::LoadFromFieldList() - Auction %u has not a existing auctioneer (GUID : %u Entry: %u)", Id, auctioneer, auctioneerData->id);
        return false;
    }

    factionTemplateId = auctioneerInfo->faction;
    auctionHouseEntry = AuctionHouseMgr::GetAuctionHouseEntry(factionTemplateId);

    if (!auctionHouseEntry)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::LoadFromFieldList() - Auction %u has auctioneer (GUID : %u Entry: %u) with wrong faction %u", Id, auctioneer, auctioneerData->id, factionTemplateId);
        return false;
    }

    return true;
}

std::string AuctionEntry::BuildAuctionMailSubject(MailAuctionAnswers response) const
{
    std::ostringstream strm;
    strm << itemEntry << ":0:" << response << ':' << Id << ':' << itemCount;
    return strm.str();
}

std::string AuctionEntry::BuildAuctionMailBody(uint32 lowGuid, uint64 bid, uint64 buyout, uint64 deposit, uint64 cut)
{
    std::ostringstream strm;
    strm.width(16);
    strm << std::right << std::hex << ObjectGuid::Create<HighGuid::Player>(lowGuid);   // HighGuid::Player always present, even for empty guids
    strm << std::dec << ':' << bid << ':' << buyout;
    strm << ':' << deposit << ':' << cut;
    return strm.str();
}
