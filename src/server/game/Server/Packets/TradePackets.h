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

#ifndef TradePackets_h__
#define TradePackets_h__

#include "Packet.h"
#include "ItemPackets.h"

namespace WorldPackets
{
    namespace Trade
    {
        class AcceptTrade final : public ClientPacket
        {
        public:
            AcceptTrade(WorldPacket&& packet) : ClientPacket(CMSG_ACCEPT_TRADE, std::move(packet)) { }
            
            void Read() override { }
        };

        class ClearTradeItem final : public ClientPacket
        {
        public:
            ClearTradeItem(WorldPacket&& packet) : ClientPacket(CMSG_CLEAR_TRADE_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 TradeSlot = 0;
        };

        class InitiateTrade final : public ClientPacket
        {
        public:
            InitiateTrade(WorldPacket&& packet) : ClientPacket(CMSG_INITIATE_TRADE, std::move(packet)) { }

            void Read() override;

            ObjectGuid Guid;
        };

        class SetTradeCurrency final : public ClientPacket
        {
        public:
            SetTradeCurrency(WorldPacket&& packet) : ClientPacket(CMSG_SET_TRADE_CURRENCY, std::move(packet)) { }

            void Read() override;

            uint32 Type = 0;
            uint32 Quantity = 0;
        };

        class SetTradeGold final : public ClientPacket
        {
        public:
            SetTradeGold(WorldPacket&& packet) : ClientPacket(CMSG_SET_TRADE_GOLD, std::move(packet)) { }

            void Read() override;

            uint64 Coinage = 0;
        };

        //< CMSG_BEGIN_TRADE
        //< CMSG_BUSY_TRADE,
        //< CMSG_CANCEL_TRADE
        //< CMSG_IGNORE_TRADE
        //< CMSG_UNACCEPT_TRADE
        class NullCmsg final : public ClientPacket
        {
        public:
            NullCmsg(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override { }
        };

        class SetTradeItem final : public ClientPacket
        {
        public:
            SetTradeItem(WorldPacket&& packet) : ClientPacket(CMSG_SET_TRADE_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 TradeSlot = 0;
            uint8 PackSlot = 0;
            uint8 ItemSlotInPack = 0;
        };

        class TradeStatus final : public ServerPacket
        {
        public:
            TradeStatus() : ServerPacket(SMSG_TRADE_STATUS, 1 + 1 + 16 + 4 + 4 + 1 + 4 + 4 + 4 + 1) { }

            WorldPacket const* Write() override;

            ::TradeStatus Status = TRADE_STATUS_INITIATED;
            uint8 TradeSlot = 0;
            ObjectGuid PartnerAccount;
            ObjectGuid Partner;
            int32 CurrencyType = 0;
            int32 CurrencyQuantity = 0;
            bool FailureForYou = false;
            int32 BagResult = 0;
            uint32 ItemID = 0;
            uint32 ID = 0;
            bool PartnerIsSameBnetAccount = false;
        };

        class TradeUpdated final : public ServerPacket
        {
        public:
            struct UnwrappedTradeItem
            {
                struct UnkData
                {
                    WorldPackets::Item::ItemInstance Item;
                    uint8 UnkByte = 0;
                };

                std::vector<UnkData> UnkDatas;
                int32 EnchantID = 0;
                int32 OnUseEnchantmentID = 0;
                ObjectGuid Creator;
                int32 Charges = 0;
                bool Lock = false;
                uint32 MaxDurability = 0;
                uint32 Durability = 0;
            };

            struct TradeItem
            {
                uint8 Slot = 0;
                int32 StackCount = 0;
                ObjectGuid GiftCreator;
                Optional<UnwrappedTradeItem> Unwrapped;
                WorldPackets::Item::ItemInstance Item;
            };

            TradeUpdated() : ServerPacket(SMSG_TRADE_UPDATED, 8 + 4 + 1 + 4 + 7 * sizeof(UnwrappedTradeItem) + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 Gold = 0;
            uint32 CurrentStateIndex = 0;
            uint8 WhichPlayer = 0;
            uint32 ClientStateIndex = 0;
            std::vector<TradeItem> Items;
            int32 CurrencyType = 0;
            uint32 ID = 0;
            int32 ProposedEnchantment = 0;
            int32 CurrencyQuantity = 0;
        };
    }
}

#endif // TradePackets_h__
