/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#ifndef AuctionHousePackets_h__
#define AuctionHousePackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "ItemPackets.h"
#include "MailPackets.h"

struct AuctionEntry;

namespace WorldPackets
{
    namespace AuctionHouse
    {
        struct AuctionItem
        {
            struct AuctionItemEnchant
            {
                AuctionItemEnchant(int32 id, uint32 expiration, int32 charges, uint8 slot) : ID(id), Expiration(expiration), Charges(charges), Slot(slot) { }
                int32 ID = 0;
                uint32 Expiration = 0;
                int32 Charges = 0;
                uint8 Slot = 0;
            };

            struct UnkData
            {
                Item::ItemInstance Item;
                uint8 UnkByte = 0;
            };
            
            std::vector<AuctionItemEnchant> Enchantments;
            std::vector<UnkData> UnkDatas;
            Item::ItemInstance Item;
            ObjectGuid Owner;
            ObjectGuid ItemGuid;
            ObjectGuid OwnerAccountID;
            ObjectGuid Bidder;
            uint64 MinBid = 0;
            uint64 MinIncrement = 0;
            uint64 BuyoutPrice = 0;
            uint64 BidAmount = 0;
            uint32 EndTime = 0;
            uint32 Flags = 0;
            int32 Count = 0;
            int32 Charges = 0;
            int32 AuctionItemID = 0;
            int32 DurationLeft = 0;
            uint8 DeleteReason = 0;
            bool CensorServerSideInfo = false;
            bool CensorBidInfo = false;
        };

        struct AuctionOwnerNotification
        {
            void Initialize(::AuctionEntry const* auction, ::Item const* item);

            int32 AuctionItemID = 0;
            uint64 BidAmount = 0;
            Item::ItemInstance Item;
        };

        struct AuctionBidderNotification
        {
            void Initialize(::AuctionEntry const* auction, ::Item const* item);

            int32 AuctionItemID = 0;
            ObjectGuid Bidder;
            Item::ItemInstance Item;
        };

        class AuctionHelloRequest final : public ClientPacket
        {
        public:
            AuctionHelloRequest(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_HELLO_REQUEST, std::move(packet)) { }

            void Read() override;
            ObjectGuid Guid;
        };

        class AuctionHelloResponse final : public ServerPacket
        {
        public:
            AuctionHelloResponse() : ServerPacket(SMSG_AUCTION_HELLO_RESPONSE, 1 + 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
            bool OpenForBusiness = true;
        };

        class AuctionCommandResult final : public ServerPacket
        {
        public:
            AuctionCommandResult() : ServerPacket(SMSG_AUCTION_COMMAND_RESULT, 32 + 16 + 16) { }

            void InitializeAuction(::AuctionEntry* auction);

            WorldPacket const* Write() override;
            
            ObjectGuid Guid;
            uint64 MinIncrement = 0;
            uint64 Money = 0;
            uint32 AuctionItemID = 0;
            uint32 Command = 0;
            int32 ErrorCode = 0;
            int32 BagResult = 0;
        };

        class AuctionSellItem final : public ClientPacket
        {
        public:
            struct AuctionItemForSale
            {
                ObjectGuid Guid;
                uint32 UseCount = 0;
            };

            AuctionSellItem(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_SELL_ITEM, std::move(packet)) { }

            void Read() override;

            uint64 BuyoutPrice = 0;
            ObjectGuid Auctioneer;
            uint64 MinBid = 0;
            uint32 RunTime = 0;
            std::vector<AuctionItemForSale> Items;
        };

        class AuctionPlaceBid final : public ClientPacket
        {
        public:
            AuctionPlaceBid(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_PLACE_BID, std::move(packet)) { }

            void Read() override;

            ObjectGuid Auctioneer;
            uint64 BidAmount = 0;
            int32 AuctionItemID = 0;
        };

        class AuctionListBidderItems final : public ClientPacket
        {
        public:
            AuctionListBidderItems(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_LIST_BIDDER_ITEMS, std::move(packet)) { }

            void Read() override;

            uint32 Offset = 0;
            std::vector<uint32> AuctionItemIDs;
            ObjectGuid Auctioneer;
        };

        class AuctionRemoveItem final : public ClientPacket
        {
        public:
            AuctionRemoveItem(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_REMOVE_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid Auctioneer;
            int32 AuctionItemID = 0;
        };

        class AuctionReplicateItems final : public ClientPacket
        {
        public:
            AuctionReplicateItems(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_REPLICATE_ITEMS, std::move(packet)) { }

            void Read() override;

            ObjectGuid Auctioneer;
            int32 Count = 0;
            int32 ChangeNumberGlobal = 0;
            int32 ChangeNumberCursor = 0;
            int32 ChangeNumberTombstone = 0;
        };

        class AuctionListPendingSales final : public ClientPacket
        {
        public:
            AuctionListPendingSales(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_LIST_PENDING_SALES, std::move(packet)) { }

            void Read() override { }
        };

        class AuctionListItemsResult final : public ServerPacket
        {
        public:
            AuctionListItemsResult() : ServerPacket(SMSG_AUCTION_LIST_ITEMS_RESULT, 150) { }

            WorldPacket const* Write() override;

            uint32 DesiredDelay = 0;
            std::vector<AuctionItem> Items;
            bool OnlyUsable = true;
            uint32 TotalCount = 0;
        };

        class AuctionListOwnerItemsResult final : public ServerPacket
        {
        public:
            AuctionListOwnerItemsResult() : ServerPacket(SMSG_AUCTION_LIST_OWNER_ITEMS_RESULT, 149) { }

            WorldPacket const* Write() override;

            uint32 DesiredDelay = 0;
            uint32 TotalCount = 0;
            std::vector<AuctionItem> Items;
        };

        class AuctionListBidderItemsResult final : public ServerPacket
        {
        public:
            AuctionListBidderItemsResult() : ServerPacket(SMSG_AUCTION_LIST_BIDDER_ITEMS_RESULT, 149) { }

            WorldPacket const* Write() override;

            uint32 DesiredDelay = 0;
            uint32 TotalCount = 0;
            std::vector<AuctionItem> Items;
        };

        class AuctionListOwnerItems final : public ClientPacket
        {
        public:
            AuctionListOwnerItems(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_LIST_OWNER_ITEMS, std::move(packet)) { }

            void Read() override;

            ObjectGuid Auctioneer;
            uint32 Offset = 0;
        };

        class AuctionListItems final : public ClientPacket
        {
        public:
            AuctionListItems(WorldPacket&& packet) : ClientPacket(CMSG_AUCTION_LIST_ITEMS, std::move(packet)) { }

            void Read() override;
            
            struct Sort
            {
                uint8 UnkByte1 = 0;
                uint8 UnkByte2 = 0;
            };

            std::vector<Sort> DataSort;
            ObjectGuid Auctioneer;
            uint32 Offset = 0;
            int32 ItemClass = 0;
            int32 InvType = 0;
            int32 Quality = 0;
            int32 ItemSubclass = 0;
            std::string Name;
            uint8 MinLevel = 1;
            uint8 SortCount = 0;
            uint8 MaxLevel = 100;
            bool ExactMatch = true;
            bool OnlyUsable = false;
        };

        class AuctionListPendingSalesResult final : public ServerPacket
        {
        public:
            AuctionListPendingSalesResult() : ServerPacket(SMSG_AUCTION_LIST_PENDING_SALES_RESULT, 140) { }

            WorldPacket const* Write() override;

            std::vector<Mail::MailListEntry> Mails;
            int32 TotalNumRecords = 0;
        };

        class AuctionClosedNotification final : public ServerPacket
        {
        public:
            AuctionClosedNotification() : ServerPacket(SMSG_AUCTION_CLOSED_NOTIFICATION, 45) { }

            WorldPacket const* Write() override;

            AuctionOwnerNotification Info;
            float ProceedsMailDelay = 0.0f;
            bool Sold = true;
        };

        class AuctionOwnerBidNotification final : public ServerPacket
        {
        public:
            AuctionOwnerBidNotification() : ServerPacket(SMSG_AUCTION_OWNER_BID_NOTIFICATION, 62) { }

            WorldPacket const* Write() override;

            AuctionOwnerNotification Info;
            ObjectGuid Bidder;
            uint64 MinIncrement = 0;
        };

        class AuctionWonNotification final : public ServerPacket
        {
        public:
            AuctionWonNotification() : ServerPacket(SMSG_AUCTION_WON_NOTIFICATION, 46) { }

            WorldPacket const* Write() override;

            AuctionBidderNotification Info;
        };

        class AuctionOutBidNotification final : public ServerPacket
        {
        public:
            AuctionOutBidNotification() : ServerPacket(SMSG_AUCTION_OUTBID_NOTIFICATION, 62) { }

            WorldPacket const* Write() override;

            AuctionBidderNotification Info;
            uint64 BidAmount = 0;
            uint64 MinIncrement = 0;
        };

        class AuctionReplicateResponse final : public ServerPacket
        {
        public:
            AuctionReplicateResponse() : ServerPacket(SMSG_AUCTION_REPLICATE_RESPONSE, 165) { }

            WorldPacket const* Write() override;

            uint32 ChangeNumberCursor = 0;
            uint32 ChangeNumberGlobal = 0;
            uint32 DesiredDelay = 0;
            uint32 ChangeNumberTombstone = 0;
            uint32 Result = 0;
            std::vector<AuctionItem> Items;
        };
    }
}

#endif // AuctionHousePackets_h__
