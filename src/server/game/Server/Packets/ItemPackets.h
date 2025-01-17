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

#ifndef ItemPackets_h__
#define ItemPackets_h__

#include "Packet.h"
#include "Item.h"
#include "Group.h"
#include "PacketUtilities.h"

struct VoidStorageItem;

namespace WorldPackets
{
    namespace Item
    {
        class BuyBackItem final : public ClientPacket
        {
        public:
            BuyBackItem(WorldPacket&& packet) : ClientPacket(CMSG_BUY_BACK_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid VendorGUID;
            uint32 Slot = 0;
        };

        class ItemRefundInfo final : public ClientPacket
        {
        public:
            ItemRefundInfo(WorldPacket&& packet) : ClientPacket(CMSG_GET_ITEM_PURCHASE_DATA, std::move(packet)) { }

            void Read() override;

            ObjectGuid ItemGUID;
        };

        class RepairItem final : public ClientPacket
        {
        public:
            RepairItem(WorldPacket&& packet) : ClientPacket(CMSG_REPAIR_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            ObjectGuid ItemGUID;
            bool UseGuildBank = false;
        };

        class SellItem final : public ClientPacket
        {
        public:
            SellItem(WorldPacket&& packet) : ClientPacket(CMSG_SELL_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid VendorGUID;
            ObjectGuid ItemGUID;
            uint32 Amount = 0;
        };

        class ItemTimeUpdate final : public ServerPacket
        {
        public:
            ItemTimeUpdate() : ServerPacket(SMSG_ITEM_TIME_UPDATE, 8 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid ItemGuid;
            uint32 DurationLeft = 0;
        };

        class SetProficiency final : public ServerPacket
        {
        public:
            SetProficiency() : ServerPacket(SMSG_SET_PROFICIENCY, 5) { }

            WorldPacket const* Write() override;

            uint32 ProficiencyMask = 0;
            uint8 ProficiencyClass = 0;
        };

        struct ItemBonusInstanceData
        {
            uint8 Context = 0;
            std::vector<int32> BonusListIDs;

            bool operator==(ItemBonusInstanceData const& r) const;
            bool operator!=(ItemBonusInstanceData const& r) const { return !(*this == r); }
        };

        struct ItemInstance
        {
            void Initialize(::Item const* item);
            void Initialize(::LootItem const* lootItem);
            void Initialize(::VoidStorageItem const* voidItem);
            void Initialize(::Roll const* roll);

            uint32 ItemID = 0;
            uint32 RandomPropertiesSeed = 0;
            uint32 RandomPropertiesID = 0;
            Optional<ItemBonusInstanceData> ItemBonus;
            Optional<CompactArray<int32>> Modifications;

            bool operator==(ItemInstance const& r) const;
            bool operator!=(ItemInstance const& r) const { return !(*this == r); }
        };

        struct InvUpdate
        {
            struct InvItem
            {
                uint8 ContainerSlot = 0;
                uint8 Slot = 0;
            };

            std::vector<InvItem> Items;
        };

        class InventoryChangeFailure final : public ServerPacket
        {
        public:
            InventoryChangeFailure() : ServerPacket(SMSG_INVENTORY_CHANGE_FAILURE, 22) { }

            WorldPacket const* Write() override;

            int8 BagResult = EQUIP_ERR_OK;
            uint8 ContainerBSlot = 0;
            ObjectGuid SrcContainer;
            ObjectGuid DstContainer;
            int32 SrcSlot = 0;
            int32 LimitCategory = 0;
            int32 Level = 0;
            ObjectGuid Item[2];
        };

        class SplitItem final : public ClientPacket
        {
        public:
            SplitItem(WorldPacket&& packet) : ClientPacket(CMSG_SPLIT_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 ToSlot = 0;
            uint8 ToPackSlot = 0;
            uint8 FromPackSlot = 0;
            int32 Quantity = 0;
            InvUpdate Inv;
            uint8 FromSlot = 0;
        };

        class SwapInvItem final : public ClientPacket
        {
        public:
            SwapInvItem(WorldPacket&& packet) : ClientPacket(CMSG_SWAP_INV_ITEM, std::move(packet)) { }

            void Read() override;

            InvUpdate Inv;
            uint8 Slot1 = 0;
            uint8 Slot2 = 0;
        };

        class SwapItem final : public ClientPacket
        {
        public:
            SwapItem(WorldPacket&& packet) : ClientPacket(CMSG_SWAP_ITEM, std::move(packet)) { }

            void Read() override;

            InvUpdate Inv;
            uint8 SlotA = 0;
            uint8 ContainerSlotB = 0;
            uint8 SlotB = 0;
            uint8 ContainerSlotA = 0;
        };

        class AutoEquipItem final : public ClientPacket
        {
        public:
            AutoEquipItem(WorldPacket&& packet) : ClientPacket(CMSG_AUTO_EQUIP_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 Slot = 0;
            InvUpdate Inv;
            uint8 PackSlot = 0;
        };

        class AutoStoreBagItem final : public ClientPacket
        {
        public:
            AutoStoreBagItem(WorldPacket&& packet) : ClientPacket(CMSG_AUTO_STORE_BAG_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 ContainerSlotB = 0;
            InvUpdate Inv;
            uint8 ContainerSlotA = 0;
            uint8 SlotA = 0;
        };

        class DestroyItem final : public ClientPacket
        {
        public:
            DestroyItem(WorldPacket&& packet) : ClientPacket(CMSG_DESTROY_ITEM, std::move(packet)) { }

            void Read() override;

            uint32 Count = 0;
            uint8 SlotNum = 0;
            uint8 ContainerId = 0;
        };

        ByteBuffer& operator>>(ByteBuffer& data, InvUpdate& invUpdate);

        class BuyItem final : public ClientPacket
        {
        public:
            BuyItem(WorldPacket&& packet) : ClientPacket(CMSG_BUY_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid VendorGUID;
            ItemInstance Item;
            uint32 BagSlot = 0u;
            uint32 Slot = 0u;
            ItemVendorType ItemType = ITEM_VENDOR_TYPE_NONE;
            int32 Quantity = 0;
            ObjectGuid ContainerGUID;
        };

        struct TransmogrifyItem
        {
            Optional<ObjectGuid> SrcItemGUID;
            Optional<ObjectGuid> SrcVoidItemGUID;
            ItemInstance Item;
            uint32 Slot = 0;
        };

        class TransmogrifyItems final : public ClientPacket
        {
        public:
            enum
            {
                MAX_TRANSMOGRIFY_ITEMS = 11
            };

            TransmogrifyItems(WorldPacket&& packet) : ClientPacket(CMSG_TRANSMOGRIFY_ITEMS, std::move(packet)) { }

            void Read() override;

            ObjectGuid NpcGUID;
            Array<TransmogrifyItem, MAX_TRANSMOGRIFY_ITEMS> Items;
        };

        struct VoidStorageContentStruct
        {
            ItemInstance Item;
            ObjectGuid Guid;
            ObjectGuid Creator;
            uint32 Slot;
        };

        class ItemPushResult final : public ServerPacket
        {
        public:
            ItemPushResult() : ServerPacket(SMSG_ITEM_PUSH_RESULT, 14 + 32 + 32 + 2 + 1 + 4) { }

            WorldPacket const* Write() override;

            enum class DisplayType : uint16
            {   
                NONE            = 0,
                ENCOUNTER_LOOT  = 1,
                NORMAL          = 2,
                HIDDEN          = 3
            };

            ItemInstance Item;
            ObjectGuid PlayerGUID;
            ObjectGuid ItemGUID;
            uint32 DungeonEncounterID = 0;
            int32 SlotInBag = 0;
            uint32 QuestLogItemID = 0;
            int32 Quantity = 0;
            int32 QuantityInInventory = 0;
            int32 BattlePetBreedID = 0;
            uint32 BattlePetBreedQuality = 0;
            int32 BattlePetSpeciesID = 0;
            int32 BattlePetLevel = 0;
            DisplayType DisplayText = DisplayType::NONE;
            uint8 Slot = 0;
            bool Pushed = false;
            bool Created = false;
            bool IsBonusRoll = false;
            bool IsEncounterLoot = false;
        };

        class BuyFailed final : ServerPacket
        {
        public:
            BuyFailed() : ServerPacket(SMSG_BUY_FAILED, 16 + 4 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid VendorGUID;
            uint32 Muid = 0u;
            BuyResult Reason = BUY_ERR_CANT_FIND_ITEM;
        };

        class SellResponse final : public ServerPacket
        {
        public:
            SellResponse() : ServerPacket(SMSG_SELL_RESPONSE, 16 + 16 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid VendorGUID;
            ObjectGuid ItemGUID;
            SellResult Reason = SELL_ERR_UNK;
        };

        class BuySucceeded final : ServerPacket
        {
        public:
            BuySucceeded() : ServerPacket(SMSG_BUY_SUCCEEDED, 16 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid VendorGUID;
            uint32 Muid = 0u;
            uint32 QuantityBought = 0u;
            int32 NewQuantity = 0;
        };

        class ItemCooldown final : public ServerPacket
        {
        public:
            ItemCooldown() : ServerPacket(SMSG_ITEM_COOLDOWN, 20) { }

            WorldPacket const* Write() override;

            ObjectGuid ItemGuid;
            uint32 SpellID = 0;
        };

        class EnchantmentLog final : public ServerPacket
        {
        public:
            EnchantmentLog() : ServerPacket(SMSG_ENCHANTMENT_LOG, 60) { }

            WorldPacket const* Write() override;

            ObjectGuid Caster;
            ObjectGuid Owner;
            ObjectGuid ItemGUID;
            int32 ItemID = 0;
            int32 EnchantSlot = 0;
            int32 Enchantment = 0;
        };

        class ItemEnchantTimeUpdate final : public ServerPacket
        {
        public:
            ItemEnchantTimeUpdate() : ServerPacket(SMSG_ITEM_ENCHANT_TIME_UPDATE, 40) { }

            WorldPacket const* Write() override;

            ObjectGuid ItemGuid;
            ObjectGuid PlayerGuid;
            uint32 Duration = 0;
            uint32 Slot = 0;
        };

        class UseCritterItem final : public ClientPacket
        {
        public:
            UseCritterItem(WorldPacket&& packet) : ClientPacket(CMSG_USE_CRITTER_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid ItemGuid;
        };

        class ItemExpirePurchaseRefund final : public ServerPacket
        {
        public:
            ItemExpirePurchaseRefund() : ServerPacket(SMSG_ITEM_EXPIRE_PURCHASE_REFUND, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid ItemGUID;
        };

        class ReadItemResultOk final : public ServerPacket
        {
        public:
            ReadItemResultOk(ObjectGuid guid) : ServerPacket(SMSG_READ_ITEM_RESULT_OK, 16), Item(guid) { }

            WorldPacket const* Write() override;

            ObjectGuid Item;
        };

        enum ReadItemFailure : int32
        {
            ITEM_FAILURE_NONE = 0,
            ITEM_FAILURE_UNK_1 = 1,
        };

        class ReadItemResultFailed final : public ServerPacket
        {
        public:
            ReadItemResultFailed() : ServerPacket(SMSG_READ_ITEM_RESULT_FAILED, 16 + 4 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid Item;
            ReadItemFailure Subcode = ITEM_FAILURE_NONE;
            int32 Delay = 0;
        };

        struct ItemPurchaseRefundItem
        {
            int32 ItemID = 0;
            int32 ItemCount = 0;
        };

        struct ItemPurchaseRefundCurrency
        {
            int32 CurrencyID = 0;
            int32 CurrencyCount = 0;
        };

        struct ItemPurchaseContents
        {
            uint32 Money = 0;
            ItemPurchaseRefundItem Items[5] = { };
            ItemPurchaseRefundCurrency Currencies[5] = { };
        };

        class SetItemPurchaseData final : public ServerPacket
        {
        public:
            SetItemPurchaseData() : ServerPacket(SMSG_SET_ITEM_PURCHASE_DATA, 4 + 4 + 4 + 5 * (4 + 4) + 5 * (4 + 4) + 16) { }

            WorldPacket const* Write() override;

            uint32 PurchaseTime = 0;
            uint32 Flags = 0;
            ItemPurchaseContents Contents;
            ObjectGuid ItemGUID;
        };

        class ItemPurchaseRefundResult final : public ServerPacket
        {
        public:
            ItemPurchaseRefundResult() : ServerPacket(SMSG_ITEM_PURCHASE_REFUND_RESULT, 1 + 4 + 5 * (4 + 4) + 5 * (4 + 4) + 16) { }

            WorldPacket const* Write() override;

            uint8 Result = 0;
            ObjectGuid ItemGUID;
            Optional<ItemPurchaseContents> Contents;
        };

        class AutoEquipItemSlot final : public ClientPacket
        {
        public:
            AutoEquipItemSlot(WorldPacket&& packet) : ClientPacket(CMSG_AUTO_EQUIP_ITEM_SLOT, std::move(packet)) { }

            void Read() override;

            ObjectGuid Item;
            InvUpdate Inv;
            uint8 ItemDstSlot = 0;
        };

        class CancelTempEnchantment final : public ClientPacket
        {
        public:
            CancelTempEnchantment(WorldPacket&& packet) : ClientPacket(CMSG_CANCEL_TEMP_ENCHANTMENT, std::move(packet)) { }

            void Read() override;

            int32 Slot = 0;
        };

        class WrapItem final : public ClientPacket
        {
        public:
            WrapItem(WorldPacket&& packet) : ClientPacket(CMSG_WRAP_ITEM, std::move(packet)) { }

            void Read() override;

            InvUpdate Inv;
        };

        class ItemPurchaseRefund final : public ClientPacket
        {
        public:
            ItemPurchaseRefund(WorldPacket&& packet) : ClientPacket(CMSG_ITEM_PURCHASE_REFUND, std::move(packet)) { }

            void Read() override;

            ObjectGuid ItemGUID;
        };

        class UpgradeItem final : public ClientPacket
        {
        public:
            UpgradeItem(WorldPacket&& packet) : ClientPacket(CMSG_UPGRADE_ITEM, std::move(packet)) { }

            void Read() override;

            ObjectGuid ItemMaster;
            ObjectGuid ItemGUID;
            int32 ContainerSlot = 0;
            int32 UpgradeID = 0;
            int32 Slot = 0;
        };

        class ReadItem final : public ClientPacket
        {
        public:
            ReadItem(WorldPacket&& packet) : ClientPacket(CMSG_READ_ITEM, std::move(packet)) { }

            void Read() override;

            uint8 PackSlot = 0;
            uint8 Slot = 0;
        };

        class SortBags final : public ClientPacket
        {
        public:
            SortBags(WorldPacket&& packet) : ClientPacket(CMSG_SORT_BAGS, std::move(packet)) { }

            void Read() override { }
        };

        class SocketGems final : public ClientPacket
        {
        public:
            SocketGems(WorldPacket&& packet) : ClientPacket(CMSG_SOCKET_GEMS, std::move(packet)) { }

            void Read() override;
            
            std::array<ObjectGuid, MAX_GEM_SOCKETS> GemItem;
            ObjectGuid ItemGuid;
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Item::ItemBonusInstanceData const& itemBonusInstanceData);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Item::ItemBonusInstanceData& itemBonusInstanceData);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Item::ItemInstance const& itemInstance);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Item::ItemInstance& itemInstance);

#endif // ItemPackets_h__
