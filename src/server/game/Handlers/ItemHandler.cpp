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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Item.h"
#include "UpdateData.h"
#include "ObjectAccessor.h"
#include "SpellInfo.h"
#include "ItemPackets.h"
#include "BankPackets.h"
#include "SpellPackets.h"
#include "BattlePetMgr.h"

void WorldSession::HandleSplitItemOpcode(WorldPackets::Item::SplitItem& splitItem)
{
    if (!splitItem.Inv.Items.empty())
        return;

    uint16 src = ((splitItem.FromPackSlot << 8) | splitItem.FromSlot);
    uint16 dst = ((splitItem.ToPackSlot << 8) | splitItem.ToSlot);

    if (src == dst)
        return;

    if (!splitItem.Quantity)
        return;

    if (!_player->IsValidPos(splitItem.FromPackSlot, splitItem.FromSlot, true))
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
        return;
    }

    if (!_player->IsValidPos(splitItem.ToPackSlot, splitItem.ToSlot, false))
    {
        _player->SendEquipError(EQUIP_ERR_WRONG_SLOT, nullptr, nullptr);
        return;
    }

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleSplitItemOpcode; srcbag %u, srcslot %u, dstbag %u, dstslot %u, src %u, dst %u, count %u", splitItem.FromPackSlot, splitItem.FromSlot, splitItem.ToPackSlot, splitItem.ToSlot, src, dst, splitItem.Quantity);
    }

    _player->SplitItem(src, dst, splitItem.Quantity);
}

void WorldSession::HandleSwapInvItemOpcode(WorldPackets::Item::SwapInvItem& swapInvItem)
{
    if (swapInvItem.Inv.Items.size() != 2)
        return;

    if (swapInvItem.Slot1 == swapInvItem.Slot2)
        return;

    if (!_player->IsValidPos(INVENTORY_SLOT_BAG_0, swapInvItem.Slot1, true))
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
        return;
    }

    if (!_player->IsValidPos(INVENTORY_SLOT_BAG_0, swapInvItem.Slot2, true))
    {
        _player->SendEquipError(EQUIP_ERR_WRONG_SLOT, nullptr, nullptr);
        return;
    }

    if (_player->IsBankPos(INVENTORY_SLOT_BAG_0, swapInvItem.Slot1) && !CanUseBank())
        return;

    if (_player->IsBankPos(INVENTORY_SLOT_BAG_0, swapInvItem.Slot2) && !CanUseBank())
        return;

    uint16 src = ((INVENTORY_SLOT_BAG_0 << 8) | swapInvItem.Slot1);
    uint16 dst = ((INVENTORY_SLOT_BAG_0 << 8) | swapInvItem.Slot2);

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleSwapInvItemOpcode; src %u, dst %u, srcslot %u, dstslot %u size %u", src, dst, swapInvItem.Slot1, swapInvItem.Slot2, swapInvItem.Inv.Items.size());
    }

    _player->SwapItem(src, dst);
}

void WorldSession::HandleAutoEquipItemSlotOpcode(WorldPackets::Item::AutoEquipItemSlot& autoEquipItemSlot)
{
    if (autoEquipItemSlot.Inv.Items.size() != 1 || !Player::IsEquipmentPos(INVENTORY_SLOT_BAG_0, autoEquipItemSlot.ItemDstSlot))
        return;

    Item* item = _player->GetItemByGuid(autoEquipItemSlot.Item);
    uint16 dstPos = autoEquipItemSlot.ItemDstSlot | (INVENTORY_SLOT_BAG_0 << 8);
    uint16 srcPos = autoEquipItemSlot.Inv.Items[0].Slot | (uint32(autoEquipItemSlot.Inv.Items[0].ContainerSlot) << 8);
    
    if (!item || item->GetPos() != srcPos || srcPos == dstPos)
        return;

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        sLog->outDebug(LOG_FILTER_DUPE, "---WorldSession::HandleAutoEquipItemSlotOpcode dstpos %u entry %u", dstPos, item->GetEntry());
    }

    _player->SwapItem(srcPos, dstPos);
}

void WorldSession::HandleSwapItem(WorldPackets::Item::SwapItem& swapItem)
{
    if (swapItem.Inv.Items.size() != 2)
        return;

    uint16 src = ((swapItem.ContainerSlotA << 8) | swapItem.SlotA);
    uint16 dst = ((swapItem.ContainerSlotB << 8) | swapItem.SlotB);
    if (src == dst)
        return;

    if (!_player->IsValidPos(swapItem.ContainerSlotA, swapItem.SlotA, true))
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr);
        return;
    }

    if (!_player->IsValidPos(swapItem.ContainerSlotB, swapItem.SlotB, true))
    {
        _player->SendEquipError(EQUIP_ERR_WRONG_SLOT, nullptr);
        return;
    }

    if (_player->IsBankPos(swapItem.ContainerSlotA, swapItem.SlotA) && !CanUseBank())
        return;

    if (_player->IsBankPos(swapItem.ContainerSlotB, swapItem.SlotB) && !CanUseBank())
        return;

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleSwapItem; src %u dst %u", src, dst);
    }

    _player->SwapItem(src, dst);
}

void WorldSession::HandleAutoEquipItem(WorldPackets::Item::AutoEquipItem& autoEquipItem)
{
    if (autoEquipItem.Inv.Items.size() != 1)
        return;

    Item* pSrcItem  = _player->GetItemByPos(autoEquipItem.PackSlot, autoEquipItem.Slot);
    if (!pSrcItem)
        return;

    uint16 dest;
    InventoryResult msg = _player->CanEquipItem(NULL_SLOT, dest, pSrcItem, !pSrcItem->IsBag());
    if (msg != EQUIP_ERR_OK)
    {
        _player->SendEquipError(msg, pSrcItem);
        return;
    }

    if(pSrcItem->GetEntry() == 38186)
        sLog->outDebug(LOG_FILTER_EFIR, "HandleAutoEquipItem - item %u; inv = %u playerGUID %u, itemGUID %u srcbag %u srcslot %u",
        pSrcItem->GetEntry(), autoEquipItem.Inv.Items.size(), _player->GetGUID(), pSrcItem->GetGUID(), autoEquipItem.PackSlot, autoEquipItem.Slot);

    uint16 src = pSrcItem->GetPos();
    if (dest == src)                                           // prevent equip in same slot, only at cheat
        return;

    Item* pDstItem = _player->GetItemByPos(dest);
    if (!pDstItem)                                         // empty slot, simple case
    {
        _player->RemoveItem(autoEquipItem.PackSlot, autoEquipItem.Slot, true);
        _player->EquipItem(dest, pSrcItem, true);
        _player->AutoUnequipOffhandIfNeed();
    }
    else                                                    // have currently equipped item, not simple case
    {
        uint8 dstbag = pDstItem->GetBagSlot();
        uint8 dstslot = pDstItem->GetSlot();

        if(pDstItem->GetEntry() == 38186)
            sLog->outDebug(LOG_FILTER_EFIR, "HandleAutoEquipItem - item %u; size = %u playerGUID %u, itemGUID %u dstbag %u dstslot %u",
            pDstItem->GetEntry(), autoEquipItem.Inv.Items.size(), _player->GetGUID(), pDstItem->GetGUID(), autoEquipItem.PackSlot, autoEquipItem.Slot);

        msg = _player->CanUnequipItem(dest, !pSrcItem->IsBag());
        if (msg != EQUIP_ERR_OK)
        {
            _player->SendEquipError(msg, pDstItem);
            return;
        }

        // check dest->src move possibility
        ItemPosCountVec sSrc;
        uint16 eSrc = 0;
        if (_player->IsInventoryPos(src))
        {
            msg = _player->CanStoreItem(autoEquipItem.PackSlot, autoEquipItem.Slot, sSrc, pDstItem, true);
            if (msg != EQUIP_ERR_OK)
                msg = _player->CanStoreItem(autoEquipItem.PackSlot, NULL_SLOT, sSrc, pDstItem, true);
            if (msg != EQUIP_ERR_OK)
                msg = _player->CanStoreItem(NULL_BAG, NULL_SLOT, sSrc, pDstItem, true);
        }
        else if (_player->IsBankPos(src))
        {
            msg = _player->CanBankItem(autoEquipItem.PackSlot, autoEquipItem.Slot, sSrc, pDstItem, true);
            if (msg != EQUIP_ERR_OK)
                msg = _player->CanBankItem(autoEquipItem.PackSlot, NULL_SLOT, sSrc, pDstItem, true);
            if (msg != EQUIP_ERR_OK)
                msg = _player->CanBankItem(NULL_BAG, NULL_SLOT, sSrc, pDstItem, true);
        }
        else if (_player->IsEquipmentPos(src))
        {
            msg = _player->CanEquipItem(autoEquipItem.Slot, eSrc, pDstItem, true);
            if (msg == EQUIP_ERR_OK)
                msg = _player->CanUnequipItem(eSrc, true);
        }

        if (msg != EQUIP_ERR_OK)
        {
            _player->SendEquipError(msg, pDstItem, pSrcItem);
            return;
        }

        _player->RemoveItem(dstbag, dstslot, false);
        _player->RemoveItem(autoEquipItem.PackSlot, autoEquipItem.Slot, false);
        _player->EquipItem(dest, pSrcItem, true);

        if (_player->IsInventoryPos(src))
            _player->StoreItem(sSrc, pDstItem, true);
        else if (_player->IsBankPos(src))
            _player->BankItem(sSrc, pDstItem, true);
        else if (_player->IsEquipmentPos(src))
            _player->EquipItem(eSrc, pDstItem, true);

        _player->AutoUnequipOffhandIfNeed();
    }
}

void WorldSession::HandleDestroyItemOpcode(WorldPackets::Item::DestroyItem& destroyItem)
{
    uint16 pos = (destroyItem.ContainerId << 8) | destroyItem.SlotNum;

    if (_player->IsEquipmentPos(pos) || _player->IsBagPos(pos))
    {
        InventoryResult msg = _player->CanUnequipItem(pos, false);
        if (msg != EQUIP_ERR_OK)
        {
            _player->SendEquipError(msg, _player->GetItemByPos(pos), nullptr);
            return;
        }
    }

    Item* item  = _player->GetItemByPos(destroyItem.ContainerId, destroyItem.SlotNum);
    if (!item)
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr);
        return;
    }

    if (item->GetTemplate()->Flags & ITEM_PROTO_FLAG_INDESTRUCTIBLE)
    {
        _player->SendEquipError(EQUIP_ERR_DROP_BOUND_ITEM, nullptr);
        return;
    }
    
    if (TradeData* tradeData = _player->GetTradeData())
    {
        if (tradeData->GetTradeSlotForItem(item->GetGUID()) != TRADE_SLOT_INVALID)
        {
            _player->SendEquipError(EQUIP_ERR_OBJECT_IS_BUSY, nullptr);
            return;
        }
    }

    if (destroyItem.Count)
    {
        uint32 i_count = destroyItem.Count;
        _player->DestroyItemCount(item, i_count, true);
    }
    else
        _player->DestroyItem(destroyItem.ContainerId, destroyItem.SlotNum, true);
}

void WorldSession::HandleReadItem(WorldPackets::Item::ReadItem& packet)
{
    Item* item = _player->GetItemByPos(packet.PackSlot, packet.Slot);
    if (item && item->GetTemplate()->PageText)
    {
        InventoryResult msg = _player->CanUseItem(item);
        if (msg == EQUIP_ERR_OK)
            SendPacket(WorldPackets::Item::ReadItemResultOk(item->GetGUID()).Write());
        else
        {
            WorldPackets::Item::ReadItemResultFailed failed;
            failed.Item = item->GetGUID();
            failed.Delay = 2;
            failed.Subcode = WorldPackets::Item::ITEM_FAILURE_UNK_1;
            SendPacket(failed.Write());
            _player->SendEquipError(msg, item, nullptr);
        }
    }
    else
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
}

void WorldSession::HandleSellItemOpcode(WorldPackets::Item::SellItem& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.ItemGUID.IsEmpty())
        return;

    Creature* creature = player->GetNPCIfCanInteractWith(packet.VendorGUID, UNIT_NPC_FLAG_VENDOR);
    if (!creature)
    {
        player->SendSellError(SELL_ERR_VENDOR_HATES_YOU, nullptr, packet.ItemGUID);
        return;
    }

    if (sObjectMgr->IsPlayerInLogList(player))
        sObjectMgr->DumpDupeConstant(player);

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Item* item = player->GetItemByGuid(packet.ItemGUID);
    if (item)
    {
        if (player->GetGUID() != item->GetOwnerGUID())
        {
            player->SendSellError(SELL_ERR_CANT_SELL_ITEM, creature, packet.ItemGUID);
            return;
        }

        if (item->IsNotEmptyBag())
        {
            player->SendSellError(SELL_ERR_CANT_SELL_ITEM, creature, packet.ItemGUID);
            return;
        }

        if (player->GetLootGUID() == item->GetGUID())
        {
            player->SendSellError(SELL_ERR_CANT_SELL_ITEM, creature, packet.ItemGUID);
            return;
        }

        if (item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_REFUNDABLE))
        {
            player->RefundItem(item);
            return;
        }

        if (item->GetEntry() == 38186)
            sLog->outDebug(LOG_FILTER_EFIR, "HandleBuyItemInSlotOpcode item %u; count = %u playerGUID %u vendorguid %u", item->GetEntry(), packet.Amount, player->GetGUID(), creature->GetGUID());

        if (packet.Amount == 0)
            packet.Amount = item->GetCount();
        else
        {
            if (packet.Amount > item->GetCount())
            {
                player->SendSellError(SELL_ERR_CANT_SELL_ITEM, creature, packet.ItemGUID);
                return;
            }
        }

        ItemTemplate const* itemTemplate = item->GetTemplate();
        if (itemTemplate)
        {
            if (itemTemplate->SellPrice > 0)
            {
                if (packet.Amount < item->GetCount())
                {
                    Item* newItem = item->CloneItem(packet.Amount, player);
                    if (!newItem)
                    {
                        player->SendSellError(SELL_ERR_CANT_SELL_ITEM, creature, packet.ItemGUID);
                        return;
                    }
                    
                    if (item->GetEntry() == 38186)
                        sLog->outDebug(LOG_FILTER_EFIR, "HandleSellItemOpcode - CloneItem of item %u; count = %u playerGUID %u, itemGUID %u", item->GetEntry(), packet.Amount, player->GetGUID(), item->GetGUID());

                    item->SetCount(item->GetCount() - packet.Amount);
                    player->ItemRemovedQuestCheck(item->GetEntry(), packet.Amount);
                    if (player->IsInWorld())
                        item->SendUpdateToPlayer(player);
                    item->SetState(ITEM_CHANGED, player);

                    player->AddItemToBuyBackSlot(newItem);
                    if (player->IsInWorld())
                        newItem->SendUpdateToPlayer(player);
                }
                else
                {
                    player->ItemRemovedQuestCheck(item->GetEntry(), item->GetCount());
                    player->RemoveItem(item->GetBagSlot(), item->GetSlot(), true);
                    item->RemoveFromUpdateQueueOf(player);
                    player->AddItemToBuyBackSlot(item);
                }

                uint32 money = itemTemplate->SellPrice * packet.Amount;
                player->ModifyMoney(money);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS, money);
                player->SendSellError(SELL_ERR_OK, creature, packet.ItemGUID);
            }
            else
                player->SendSellError(SELL_ERR_CANT_SELL_ITEM, creature, packet.ItemGUID);

            return;
        }
    }

    player->SendSellError(SELL_ERR_CANT_FIND_ITEM, creature, packet.ItemGUID);

    return;
}

void WorldSession::HandleBuybackItem(WorldPackets::Item::BuyBackItem& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* creature = player->GetNPCIfCanInteractWith(packet.VendorGUID, UNIT_NPC_FLAG_VENDOR);
    if (!creature)
    {
        player->SendSellError(SELL_ERR_VENDOR_HATES_YOU, nullptr, ObjectGuid::Empty);
        return;
    }

    if (sObjectMgr->IsPlayerInLogList(player))
        sObjectMgr->DumpDupeConstant(player);

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Item* item = player->GetItemFromBuyBackSlot(packet.Slot);
    if (item)
    {
        uint32 price = player->GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE + packet.Slot - BUYBACK_SLOT_START);
        if (!player->HasEnoughMoney(uint64(price)))
        {
            player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, item->GetEntry());
            return;
        }

        if (item->GetEntry() == 38186)
            sLog->outDebug(LOG_FILTER_EFIR, "HandleBuyItemInSlotOpcode item %u; count = %u playerGUID %u vendorguid %u", item->GetEntry(), item->GetCount(), player->GetGUID(), creature->GetGUID());
    
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, item, false);
        if (msg == EQUIP_ERR_OK)
        {
            player->ModifyMoney(-(int32)price);
            player->RemoveItemFromBuyBackSlot(packet.Slot, false);
            player->ItemAddedQuestCheck(item->GetEntry(), item->GetCount());
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM, item->GetEntry(), item->GetCount());
            player->StoreItem(dest, item, true);
        }
        else
            player->SendEquipError(msg, item, nullptr);

        return;
    }
    else
        player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, creature);
}

void WorldSession::HandleBuyItemInSlotOpcode(WorldPacket & recvData)
{
    ObjectGuid vendorguid, bagguid;
    uint32 item, slot, count;
    uint8 bagslot;

    recvData >> vendorguid >> item  >> slot >> bagguid >> bagslot >> count;

    // client expects count starting at 1, and we send vendorslot+1 to client already
    if (slot > 0)
        --slot;
    else
        return;                                             // cheating

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleBuyItemInSlotOpcode item %u; count = %u playerGUID %u vendorguid %u", item, count, _player->GetGUID(), vendorguid);
    }

    if(item == 38186)
        sLog->outDebug(LOG_FILTER_EFIR, "HandleBuyItemInSlotOpcode item %u; count = %u playerGUID %u vendorguid %u", item, count, _player->GetGUID(), vendorguid);

    uint8 bag = NULL_BAG;                                   // init for case invalid bagGUID
    Item* bagItem = NULL;
    // find bag slot by bag guid
    if (bagguid == _player->GetGUID())
        bag = INVENTORY_SLOT_BAG_0;
    else
        bagItem = _player->GetItemByGuid(bagguid);

    if (bagItem && bagItem->IsBag())
        bag = bagItem->GetSlot();

    // bag not found, cheating?
    if (bag == NULL_BAG)
        return;

    GetPlayer()->BuyItemFromVendorSlot(vendorguid, slot, item, count, bag, bagslot);
}

void WorldSession::HandleBuyItemOpcode(WorldPackets::Item::BuyItem& packet)
{
    if (packet.Slot > 0)
        --packet.Slot;
    else
        return;

    if (sObjectMgr->IsPlayerInLogList(GetPlayer()))
    {
        sObjectMgr->DumpDupeConstant(GetPlayer());
        sLog->outDebug(LOG_FILTER_DUPE, "---HandleBuyItemOpcode; vendor: %llu; item: %u; count: %u;", packet.VendorGUID.GetGUIDLow(), packet.Item.ItemID, packet.Quantity);
    }

    if (packet.Item.ItemID == 38186)
        sLog->outDebug(LOG_FILTER_EFIR, "HandleBuyItemOpcode item %u; count = %u playerGUID %u", packet.Item.ItemID, packet.Quantity, _player->GetGUID());

    if (packet.ItemType == ITEM_VENDOR_TYPE_ITEM)
    {
        uint8 bag = NULL_BAG;

        if (!packet.ContainerGUID)
            bag = INVENTORY_SLOT_BAG_0;
        else
        {
            Item* bagItem = _player->GetItemByGuid(packet.ContainerGUID);
            if (bagItem && bagItem->IsBag())
                bag = bagItem->GetSlot();
        }

        GetPlayer()->BuyItemFromVendorSlot(packet.VendorGUID, packet.Slot, packet.Item.ItemID, packet.Quantity, bag, packet.BagSlot);
    }
    else if (packet.ItemType == ITEM_VENDOR_TYPE_CURRENCY)
        GetPlayer()->BuyCurrencyFromVendorSlot(packet.VendorGUID, packet.Slot, packet.Item.ItemID, packet.Quantity);
    else
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: received wrong itemType (%u) in HandleBuyItemOpcode", packet.ItemType);
}

void WorldSession::HandleAutoStoreBagItem(WorldPackets::Item::AutoStoreBagItem& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!packet.Inv.Items.empty())
        return;

    Item* item = player->GetItemByPos(packet.ContainerSlotA, packet.SlotA);
    if (!item)
        return;

    if (!player->IsValidPos(packet.ContainerSlotB, NULL_SLOT, false))
    {
        player->SendEquipError(EQUIP_ERR_WRONG_SLOT, nullptr, nullptr);
        return;
    }

    uint16 src = item->GetPos();
    if (player->IsEquipmentPos (src) || player->IsBagPos (src))
    {
        InventoryResult msg = player->CanUnequipItem(src, !player->IsBagPos (src));
        if (msg != EQUIP_ERR_OK)
        {
            player->SendEquipError(msg, item, nullptr);
            return;
        }
    }

    ItemPosCountVec dest;
    InventoryResult msg = player->CanStoreItem(packet.ContainerSlotB, NULL_SLOT, dest, item, false);
    if (msg != EQUIP_ERR_OK)
    {
        player->SendEquipError(msg, item, nullptr);
        return;
    }

    if (dest.size() == 1 && dest[0].pos == src)
    {
        player->SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, item, nullptr);
        return;
    }

    if (item->GetEntry() == 38186)
        sLog->outDebug(LOG_FILTER_EFIR, "HandleAutoStoreBagItem - item %u; count = %u playerGUID %u, itemGUID %u dstbag %u srcslot %u srcbag %u",
        item->GetEntry(), packet.Inv.Items.size(), player->GetGUID(), item->GetGUID(), packet.ContainerSlotB, packet.SlotA, packet.ContainerSlotA);

    player->RemoveItem(packet.ContainerSlotA, packet.SlotA, true);
    player->StoreItem(dest, item, true);
}

void WorldSession::HandleBuyBankSlot(WorldPackets::Bank::BuyBankSlot& /*packet*/)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    uint32 slot = player->GetBankBagSlotCount();

    ++slot;

    BankBagSlotPricesEntry const* slotEntry = sBankBagSlotPricesStore.LookupEntry(slot);
    if (!slotEntry)
        return;

    uint32 price = slotEntry->Price;

    if (!player->HasEnoughMoney(uint64(price)))
        return;

    player->SetBankBagSlotCount(slot);
    player->ModifyMoney(-int64(price));
    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT);
}

void WorldSession::HandleAutoBankItem(WorldPackets::Bank::AutoBankItem& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Item* item = player->GetItemByPos(packet.Bag, packet.Slot);
    if (!item)
        return;

    ItemPosCountVec dest;
    InventoryResult msg = player->CanBankItem(NULL_BAG, NULL_SLOT, dest, item, false);
    if (msg != EQUIP_ERR_OK)
    {
        player->SendEquipError(msg, item, nullptr);
        return;
    }

    if (dest.size() == 1 && dest[0].pos == item->GetPos())
    {
        player->SendEquipError(EQUIP_ERR_CANT_SWAP, item, nullptr);
        return;
    }

    player->RemoveItem(packet.Bag, packet.Slot, true);
    player->ItemRemovedQuestCheck(item->GetEntry(), item->GetCount());
    player->BankItem(dest, item, true);
}

void WorldSession::HandleAutoStoreBankItem(WorldPackets::Bank::AutoStoreBankItem& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Item* item = player->GetItemByPos(packet.Bag, packet.Slot);
    if (!item)
        return;

    ItemPosCountVec dest;

    if (player->IsBankPos(packet.Bag, packet.Slot))
    {
        
        InventoryResult msg = player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, item, false);
        if (msg != EQUIP_ERR_OK)
        {
            player->SendEquipError(msg, item, nullptr);
            return;
        }

        player->RemoveItem(packet.Bag, packet.Slot, true);
        player->StoreItem(dest, item, true);
        player->ItemAddedQuestCheck(item->GetEntry(), item->GetCount());
    }
    else
    {
        InventoryResult msg = player->CanBankItem(NULL_BAG, NULL_SLOT, dest, item, false);
        if (msg != EQUIP_ERR_OK)
        {
            player->SendEquipError(msg, item, nullptr);
            return;
        }

        player->RemoveItem(packet.Bag, packet.Slot, true);
        player->BankItem(dest, item, true);
    }
}

void WorldSession::HandleWrapItem(WorldPackets::Item::WrapItem& packet)
{
    if (packet.Inv.Items.size() != 2)
        return;

    Item* gift = _player->GetItemByPos(packet.Inv.Items[0].ContainerSlot, packet.Inv.Items[0].Slot);
    if (!gift)
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, gift, nullptr);
        return;
    }

    if (!(gift->GetTemplate()->Flags & ITEM_PROTO_FLAG_WRAPPER)) // cheating: non-wrapper wrapper
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, gift, nullptr);
        return;
    }

    Item* item = _player->GetItemByPos(packet.Inv.Items[1].ContainerSlot, packet.Inv.Items[1].Slot);
    if (!item)
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, item, nullptr);
        return;
    }

    if (item == gift)
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_WRAPPED, item, nullptr);
        return;
    }

    if (item->IsEquipped())
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_EQUIPPED, item, nullptr);
        return;
    }

    if (!item->GetGuidValue(ITEM_FIELD_GIFT_CREATOR).IsEmpty())
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_WRAPPED, item, nullptr);
        return;
    }

    if (item->IsBag())
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_BAGS, item, nullptr);
        return;
    }

    if (item->IsSoulBound())
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_BOUND, item, nullptr);
        return;
    }

    if (item->GetMaxStackCount() != 1)
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_STACKABLE, item, nullptr);
        return;
    }

    if (item->GetTemplate()->MaxCount > 0)
    {
        _player->SendEquipError(EQUIP_ERR_CANT_WRAP_UNIQUE, item, nullptr);
        return;
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_GIFT);
    stmt->setUInt64(0, item->GetOwnerGUID().GetCounter());
    stmt->setUInt64(1, item->GetGUIDLow());
    stmt->setUInt32(2, item->GetEntry());
    stmt->setUInt32(3, item->GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));
    trans->Append(stmt);

    item->SetEntry(gift->GetEntry());

    switch (item->GetEntry())
    {
        case 5042:  item->SetEntry(5043); break;
        case 5048:  item->SetEntry(5044); break;
        case 17303: item->SetEntry(17302); break;
        case 17304: item->SetEntry(17305); break;
        case 17307: item->SetEntry(17308); break;
        case 21830: item->SetEntry(21831); break;
    }
    item->SetGuidValue(ITEM_FIELD_GIFT_CREATOR, _player->GetGUID());
    item->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED);
    item->SetState(ITEM_CHANGED, _player);

    if (item->GetState() == ITEM_NEW)                          // save new item, to have alway for `character_gifts` record in `item_instance`
    {
        // after save it will be impossible to remove the item from the queue
        item->RemoveFromUpdateQueueOf(_player);
        item->SaveToDB(trans);                                   // item gave inventory record unchanged and can be save standalone
    }
    CharacterDatabase.CommitTransaction(trans);

    _player->DestroyItem(gift->GetBagSlot(), gift->GetSlot(), true);
}

void WorldSession::HandleSocketGems(WorldPackets::Item::SocketGems& packet)
{
    if (packet.ItemGuid.IsEmpty())
        return;


    if ((!packet.GemItem[0].IsEmpty() && (packet.GemItem[0] == packet.GemItem[1] || packet.GemItem[0] == packet.GemItem[2])) ||
        (!packet.GemItem[1].IsEmpty() && (packet.GemItem[1] == packet.GemItem[2])))
        return;

    Item* itemTarget = _player->GetItemByGuid(packet.ItemGuid);
    if (!itemTarget)
        return;

    ItemTemplate const* itemProto = itemTarget->GetTemplate();
    if (!itemProto)
        return;

    //this slot is excepted when applying / removing meta gem bonus
    uint8 slot = itemTarget->IsEquipped() ? itemTarget->GetSlot() : uint8(NULL_SLOT);

    Item* Gems[MAX_GEM_SOCKETS];
    for (uint8 i = 0; i < MAX_GEM_SOCKETS; ++i)
        Gems[i] = !packet.GemItem[i].IsEmpty() ? _player->GetItemByGuid(packet.GemItem[i]) : NULL;

    GemPropertiesEntry const* GemProps[MAX_GEM_SOCKETS];
    for (uint8 i = 0; i < MAX_GEM_SOCKETS; ++i)                //get geminfo from dbc storage
        GemProps[i] = (Gems[i]) ? sGemPropertiesStore.LookupEntry(Gems[i]->GetTemplate()->GemProperties) : NULL;

    for (uint8 i = 0; i < MAX_GEM_SOCKETS; ++i)                //check for hack maybe
    {
        if (!GemProps[i])
            continue;

        // tried to put gem in socket where no socket exists (take care about prismatic sockets)
        if (!itemProto->Socket[i].Color)
        {
            // no prismatic socket
            if (!itemTarget->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT))
                return;

            // not first not-colored (not normaly used) socket
            if (i != 0 && !itemProto->Socket[i - 1].Color && (i + 1 >= MAX_GEM_SOCKETS || itemProto->Socket[i + 1].Color))
                return;

            // ok, this is first not colored socket for item with prismatic socket
        }

        // tried to put non-meta gem in meta socket
        if (itemProto->Socket[i].Color == SOCKET_COLOR_META && GemProps[i]->Type != SOCKET_COLOR_META)
            return;

        // tried to put meta gem in non-meta socket
        if (itemProto->Socket[i].Color != SOCKET_COLOR_META && GemProps[i]->Type == SOCKET_COLOR_META)
            return;

        // tried to put non-cogwheel gem in cogwheel socket
        if (itemProto->Socket[i].Color == SOCKET_COLOR_COGWHEEL && GemProps[i]->Type != SOCKET_COLOR_COGWHEEL)
            return;

        // tried to put cogwheel gem in non-cogwheel socket
        if (itemProto->Socket[i].Color != SOCKET_COLOR_COGWHEEL && GemProps[i]->Type == SOCKET_COLOR_COGWHEEL)
            return;

        // tried to put non-sha gem in sha socket
        if (itemProto->Socket[i].Color == SOCKET_COLOR_SHA && GemProps[i]->Type != SOCKET_COLOR_SHA)
            return;

        // tried to put sha gem in non-sha socket
        if (itemProto->Socket[i].Color != SOCKET_COLOR_SHA && GemProps[i]->Type == SOCKET_COLOR_SHA)
            return;
    }

    uint32 GemEnchants[MAX_GEM_SOCKETS];
    uint32 OldEnchants[MAX_GEM_SOCKETS];
    for (uint8 i = 0; i < MAX_GEM_SOCKETS; ++i)                //get new and old enchantments
    {
        GemEnchants[i] = (GemProps[i]) ? GemProps[i]->EnchantID : 0;
        OldEnchants[i] = itemTarget->GetEnchantmentId(EnchantmentSlot(SOCK_ENCHANTMENT_SLOT + i));
    }

    // check unique-equipped conditions
    for (uint8 i = 0; i < MAX_GEM_SOCKETS; ++i)
    {
        if (!Gems[i])
            continue;

        // continue check for case when attempt add 2 similar unique equipped gems in one item.
        ItemTemplate const* iGemProto = Gems[i]->GetTemplate();

        // unique item (for new and already placed bit removed enchantments
        if (iGemProto->Flags & ITEM_PROTO_FLAG_UNIQUE_EQUIPPED)
        {
            for (int j = 0; j < MAX_GEM_SOCKETS; ++j)
            {
                if (i == j)                                    // skip self
                    continue;

                if (Gems[j])
                {
                    if (iGemProto->ItemId == Gems[j]->GetEntry())
                    {
                        _player->SendEquipError(EQUIP_ERR_ITEM_UNIQUE_EQUIPPABLE_SOCKETED, itemTarget, NULL);
                        return;
                    }
                }
                /*else if (OldEnchants[j])
                {
                    if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(OldEnchants[j]))
                        if (iGemProto->ItemId == enchantEntry->SRCItemID)
                        {
                            _player->SendEquipError(EQUIP_ERR_ITEM_UNIQUE_EQUIPPABLE_SOCKETED, itemTarget, NULL);
                            return;
                        }
                }*/
            }
        }

        // unique limit type item
        int32 limit_newcount = 0;
        if (iGemProto->ItemLimitCategory)
        {
            if (ItemLimitCategoryEntry const* limitEntry = sItemLimitCategoryStore.LookupEntry(iGemProto->ItemLimitCategory))
            {
                // NOTE: limitEntry->mode is not checked because if item has limit then it is applied in equip case
                for (uint8 j = 0; j < MAX_GEM_SOCKETS; ++j)
                {
                    if (Gems[j])
                    {
                        // new gem
                        if (iGemProto->ItemLimitCategory == Gems[j]->GetTemplate()->ItemLimitCategory)
                            ++limit_newcount;
                    }
                    /*else if (OldEnchants[j])
                    {
                        // existing gem
                        if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(OldEnchants[j]))
                            if (ItemTemplate const* jProto = sObjectMgr->GetItemTemplate(enchantEntry->SRCItemID))
                                if (iGemProto->ItemLimitCategory == jProto->ItemLimitCategory)
                                    ++limit_newcount;
                    }*/
                }

                if (limit_newcount > 0 && uint32(limit_newcount) > limitEntry->Quantity)
                {
                    _player->SendEquipError(EQUIP_ERR_ITEM_UNIQUE_EQUIPPABLE_SOCKETED, itemTarget, NULL);
                    return;
                }
            }
        }

        // for equipped item check all equipment for duplicate equipped gems
        if (itemTarget->IsEquipped())
        {
            if (InventoryResult res = _player->CanEquipUniqueItem(Gems[i], slot, std::max(limit_newcount, 0)))
            {
                _player->SendEquipError(res, itemTarget, NULL);
                return;
            }
        }
    }

    bool SocketBonusActivated = itemTarget->GemsFitSockets();    //save state of socketbonus
    _player->ToggleMetaGemsActive(slot, false);             //turn off all metagems (except for the target item)

    //if a meta gem is being equipped, all information has to be written to the item before testing if the conditions for the gem are met

    //remove ALL enchants
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + MAX_GEM_SOCKETS; ++enchant_slot)
        _player->ApplyEnchantment(itemTarget, EnchantmentSlot(enchant_slot), false);

    for (uint8 i = 0; i < MAX_GEM_SOCKETS; ++i)
    {
        if (GemEnchants[i])
        {
            uint32 gemCount = 1;
            itemTarget->SetEnchantment(EnchantmentSlot(SOCK_ENCHANTMENT_SLOT + i), GemEnchants[i], 0, 0);
            if (Item* guidItem = _player->GetItemByGuid(packet.GemItem[i]))
                _player->DestroyItemCount(guidItem, gemCount, true);
        }
    }

    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + MAX_GEM_SOCKETS; ++enchant_slot)
        _player->ApplyEnchantment(itemTarget, EnchantmentSlot(enchant_slot), true);

    bool SocketBonusToBeActivated = itemTarget->GemsFitSockets();//current socketbonus state
    if (SocketBonusActivated ^ SocketBonusToBeActivated)     //if there was a change...
    {
        _player->ApplyEnchantment(itemTarget, BONUS_ENCHANTMENT_SLOT, false);
        itemTarget->SetEnchantment(BONUS_ENCHANTMENT_SLOT, (SocketBonusToBeActivated ? itemTarget->GetTemplate()->socketBonus : 0), 0, 0);
        _player->ApplyEnchantment(itemTarget, BONUS_ENCHANTMENT_SLOT, true);
        //it is not displayed, client has an inbuilt system to determine if the bonus is activated
    }

    _player->ToggleMetaGemsActive(slot, true);              //turn on all metagems (except for target item)

    _player->RemoveTradeableItem(itemTarget);
    itemTarget->ClearSoulboundTradeable(_player);           // clear tradeable flag
}

void WorldSession::HandleCancelTempEnchantmentOpcode(WorldPackets::Item::CancelTempEnchantment& packet)
{
    if (!Player::IsEquipmentPos(INVENTORY_SLOT_BAG_0, packet.Slot))
        return;

    Item* item = GetPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, packet.Slot);

    if (!item)
        return;

    if (!item->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
        return;

    GetPlayer()->ApplyEnchantment(item, TEMP_ENCHANTMENT_SLOT, false);
    item->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);
}

void WorldSession::HandleGetItemPurchaseData(WorldPackets::Item::ItemRefundInfo& packet)
{
    Item* item = _player->GetItemByGuid(packet.ItemGUID);
    if (!item)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Item refund: item not found!");
        return;
    }

    GetPlayer()->SendRefundInfo(item);
}

void WorldSession::HandleItemPurchaseRefund(WorldPackets::Item::ItemPurchaseRefund& packet)
{
    Item* item = _player->GetItemByGuid(packet.ItemGUID);
    if (!item)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Item refund: item not found!");
        return;
    }

    GetPlayer()->RefundItem(item);
}

void WorldSession::HandleTransmogrifyItems(WorldPackets::Item::TransmogrifyItems& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (!player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_TRANSMOGRIFIER))
        return;

    int64 cost = 0;
    std::unordered_map<Item*, Item*> transmogItems;
    std::unordered_map<Item*, std::pair<VoidStorageItem*, BonusData>> transmogVoidItems;
    std::vector<Item*> resetAppearanceItems;

    for (auto const& transmogItem : packet.Items)
    {
        if (transmogItem.Slot >= EQUIPMENT_SLOT_END)
            return;

        Item* itemTransmogrified = player->GetItemByPos(INVENTORY_SLOT_BAG_0, transmogItem.Slot);
        if (!itemTransmogrified)
            return;

        if (player->CanUseItem(itemTransmogrified->GetTemplate()) != EQUIP_ERR_OK)
            return;

        WorldPackets::Item::ItemInstance itemInstance;
        BonusData const* bonus = nullptr;
        if (transmogItem.SrcItemGUID)
        {
            Item* itemTransmogrifier = player->GetItemByGuid(*transmogItem.SrcItemGUID);
            if (!itemTransmogrifier)
                return;

            if (player->CanUseItem(itemTransmogrifier->GetTemplate()) != EQUIP_ERR_OK)
                return;

            itemInstance.Initialize(itemTransmogrifier);
            bonus = itemTransmogrifier->GetBonus();
            transmogItems[itemTransmogrified] = itemTransmogrifier;
        }
        else if (transmogItem.SrcVoidItemGUID)
        {
            uint8 slot;
            VoidStorageItem* itemTransmogrifier = player->GetVoidStorageItem(transmogItem.SrcVoidItemGUID->GetCounter(), slot);
            if (!itemTransmogrifier)
                return;

            ItemTemplate const * transmogrifierTemplate = sObjectMgr->GetItemTemplate(itemTransmogrifier->ItemEntry);
            if (player->CanUseItem(transmogrifierTemplate) != EQUIP_ERR_OK)
                return;

            itemInstance.Initialize(itemTransmogrifier);
            std::pair<VoidStorageItem*, BonusData>& transmogData = transmogVoidItems[itemTransmogrified];
            transmogData.first = itemTransmogrifier;
            transmogData.second.Initialize(itemInstance);
            bonus = &transmogData.second;
        }
        else
        {
            resetAppearanceItems.push_back(itemTransmogrified);
            continue;
        }

        if (itemInstance != transmogItem.Item)
            return;

        if (!Item::CanTransmogrifyItemWithItem(itemTransmogrified, transmogItem.Item, bonus))
            return;

        cost += itemTransmogrified->GetSpecialPrice();
        cost = int64(cost * player->GetTotalAuraMultiplier(SPELL_AURA_MOD_VOID_STORAGE_AND_TRANSMOGRIFY_COST));
    }

    if (cost)
    {
        if (!player->HasEnoughMoney(cost))
            return;

        player->ModifyMoney(-cost);
    }

    for (auto& transmogPair : transmogItems)
    {
        Item* transmogrified = transmogPair.first;
        Item* transmogrifier = transmogPair.second;

        transmogrified->SetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID, transmogrifier->GetEntry());
        transmogrified->SetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD, transmogrifier->GetAppearanceModId());
        player->SetVisibleItemSlot(transmogrified->GetSlot(), transmogrified);

        transmogrified->SetNotRefundable(player);
        transmogrified->ClearSoulboundTradeable(player);

        transmogrifier->SetNotRefundable(player);
        transmogrifier->ClearSoulboundTradeable(player);

        if (transmogrifier->GetTemplate()->Bonding == BIND_WHEN_EQUIPED || transmogrifier->GetTemplate()->Bonding == BIND_WHEN_USE)
            transmogrifier->SetBinding(true);
    }

    for (auto& transmogVoirPair : transmogVoidItems)
    {
        Item* transmogrified = transmogVoirPair.first;
        VoidStorageItem* transmogrifier = transmogVoirPair.second.first;
        BonusData& bonus = transmogVoirPair.second.second;

        transmogrified->SetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID, transmogrifier->ItemEntry);
        transmogrified->SetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD, bonus.AppearanceModID);
        player->SetVisibleItemSlot(transmogrified->GetSlot(), transmogrified);

        transmogrified->SetNotRefundable(player);
        transmogrified->ClearSoulboundTradeable(player);
    }

    for (Item* item : resetAppearanceItems)
    {
        item->SetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID, 0);
        item->SetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD, 0);
        player->SetVisibleItemSlot(item->GetSlot(), item);
    }
}

void WorldSession::HandleOpenItem(WorldPackets::Spells::OpenItem& packet)
{
    Player* player = GetPlayer();

    if (player->m_mover != player)
        return;

    Item* item = player->GetItemByPos(packet.Slot, packet.PackSlot);
    if (!item/* || item->GetGUID() != itemGUID*/)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
        return;
    }

    ItemTemplate const* proto = item->GetTemplate();
    if (!proto)
    {
        player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, item, nullptr);
        return;
    }

    if (!(proto->Flags & ITEM_PROTO_FLAG_OPENABLE) && !item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
    {
        player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, nullptr);
        sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [guid: %u] tried to open item [guid: %u, entry: %u] which is not openable!",
                player->GetName(), player->GetGUIDLow(), item->GetGUIDLow(), proto->ItemId);
        return;
    }

    uint32 lockId = proto->LockID;
    if (lockId)
    {
        LockEntry const* lockInfo = sLockStore.LookupEntry(lockId);

        if (!lockInfo)
        {
            player->SendEquipError(EQUIP_ERR_ITEM_LOCKED, item, nullptr);
            return;
        }

        if (item->IsLocked())
        {
            player->SendEquipError(EQUIP_ERR_ITEM_LOCKED, item, nullptr);
            return;
        }
    }

    if (item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GIFT_BY_ITEM);

        stmt->setUInt64(0, item->GetGUIDLow());

        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            uint32 entry = fields[0].GetUInt32();
            uint32 flags = fields[1].GetUInt32();

            item->SetGuidValue(ITEM_FIELD_GIFT_CREATOR, ObjectGuid::Empty);
            item->SetEntry(entry);
            item->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, flags);
            item->SetState(ITEM_CHANGED, player);
        }
        else
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Wrapped item %u don't have record in character_gifts table and will deleted", item->GetGUIDLow());
            player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
            return;
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);

        stmt->setUInt64(0, item->GetGUIDLow());

        CharacterDatabase.Execute(stmt);
    }
    else
        player->SendLoot(item->GetGUID(), LOOT_CORPSE);
}

void WorldSession::HandleUpgradeItem(WorldPackets::Item::UpgradeItem& packet)
{
    Player* player = GetPlayer();
    if (!player->GetNPCIfCanInteractWith(packet.ItemMaster, UNIT_NPC_FLAG_NONE, UNIT_NPC_FLAG2_UPGRADE_MASTER))
        return;

    Item* item = player->GetItemByGuid(packet.ItemGUID);
    if (!item)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeItem - Can't find item (GUID: %u).", packet.ItemGUID.GetGUIDLow());
        return;
    }

    ItemUpgradeData const* upgradeData = sDB2Manager.GetItemUpgradeData(item->GetEntry());
    if (!upgradeData)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeItem - Can't find item (GUID: %u).", packet.ItemGUID.GetGUIDLow());
        return;
    }

    ItemUpgradeEntry const* newUpgrade = nullptr;
    for (ItemUpgradeEntry const* upgradeEntry : upgradeData->upgrade)
        if (upgradeEntry->id == packet.UpgradeID)
        {
            newUpgrade = upgradeEntry;
            break;
        }

    if (!newUpgrade)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeItem - Can't find upgrade id %u for item %u (GUID: %u).",
            packet.UpgradeID, item->GetEntry(), packet.ItemGUID.GetGUIDLow());
        return;
    }

    if (item->GetUpgradeId() != newUpgrade->prevUpgradeId)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeItem - Previous item upgrade id mismatch: %u should be %u. Item id %u (GUID: %u).",
            item->GetUpgradeId(), newUpgrade->prevUpgradeId, item->GetEntry(), packet.ItemGUID.GetGUIDLow());
        return;
    }


    if (uint32 reqCur = newUpgrade->currencyReqId)
    {
        if (!player->HasCurrency(reqCur, newUpgrade->currencyReqAmt))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeItem - insufficient currency: upgrade id %u Item id %u (GUID: %u).",
                packet.UpgradeID, item->GetEntry(), packet.ItemGUID.GetGUIDLow());
            player->SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, nullptr, nullptr);
            return;
        }

        player->ModifyCurrency(reqCur, -int32(newUpgrade->currencyReqAmt));
    }

    if (item->IsEquipped())
        player->_ApplyItemMods(item, item->GetSlot(), false);

    item->SetUpgradeId(packet.UpgradeID);
    //item->SetLevel(item->GetTemplate()->ItemLevel + newUpgrade->levelBonus);
    item->SetState(ITEM_CHANGED, player);

    if (item->IsEquipped())
        player->_ApplyItemMods(item, item->GetSlot(), true);
}

bool WorldSession::CanUseBank(ObjectGuid bankerGUID) const
{
    // bankerGUID parameter is optional, set to 0 by default.
    if (bankerGUID.IsEmpty())
        bankerGUID = m_currentBankerGUID;

    bool isUsingBankCommand = (bankerGUID == GetPlayer()->GetGUID() && bankerGUID == m_currentBankerGUID);

    if (!isUsingBankCommand)
    {
        Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(bankerGUID, UNIT_NPC_FLAG_BANKER);
        if (!creature)
            return false;
    }

    return true;
}

void WorldSession::HandleRepairItem(WorldPackets::Item::RepairItem& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Creature* unit = player->GetNPCIfCanInteractWith(packet.NpcGUID, UNIT_NPC_FLAG_REPAIR);
    if (!unit)
        return;

    if (player->HasUnitState(UNIT_STATE_DIED))
        player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    float discountMod = player->GetReputationPriceDiscount(unit);

    if (packet.ItemGUID)
    {
        Item* item = player->GetItemByGuid(packet.ItemGUID);
        if (item)
            player->DurabilityRepair(item->GetPos(), true, discountMod, packet.UseGuildBank);
    }
    else
        player->DurabilityRepairAll(true, discountMod, packet.UseGuildBank);
}

void WorldSession::HandleSortBags(WorldPackets::Item::SortBags& /*packet*/)
{
    GetPlayer()->SortBags();
}

void WorldSession::HandleUseCritterItem(WorldPackets::Item::UseCritterItem& useCritterItem)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Item* item = player->GetItemByGuid(useCritterItem.ItemGuid);
    if (!item)
        return;

    //< ItemToBattlePetSpecies.db2 was dropped from client
    /*ItemToBattlePetSpeciesEntry const* itemToBattlePetSpecies = sItemToBattlePetSpeciesStore.LookupEntry(item->GetEntry());
    if (!itemToBattlePetSpecies)
        return;

    BattlePetSpeciesEntry const* battlePetSpecies = sBattlePetSpeciesStore.LookupEntry(itemToBattlePetSpecies->BattlePetSpeciesID);
    if (!battlePetSpecies)
        return;

    BattlePetMgr* mgr = player->GetBattlePetMgr();
    if (mgr)
    {
        mgr->AddPet(battlePetSpecies->ID, mgr->GetRandomBreedID(battlePetSpecies->ID), mgr->GetRandomQuailty());
        player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
    }*/
}
