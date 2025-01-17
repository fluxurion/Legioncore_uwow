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

#include "TradePackets.h"

void WorldPackets::Trade::ClearTradeItem::Read()
{
    _worldPacket >> TradeSlot;
}

void WorldPackets::Trade::InitiateTrade::Read()
{
    _worldPacket >> Guid;
}

void WorldPackets::Trade::SetTradeCurrency::Read()
{
    _worldPacket >> Type;
    _worldPacket >> Quantity;
}

void WorldPackets::Trade::SetTradeGold::Read()
{
    _worldPacket >> Coinage;
}

void WorldPackets::Trade::SetTradeItem::Read()
{
    _worldPacket >> TradeSlot;
    _worldPacket >> PackSlot;
    _worldPacket >> ItemSlotInPack;
}

WorldPacket const* WorldPackets::Trade::TradeStatus::Write()
{
    _worldPacket.WriteBit(PartnerIsSameBnetAccount);
    _worldPacket.WriteBits(Status, 5);
    switch (Status)
    {
        case TRADE_STATUS_FAILED:
            _worldPacket.WriteBit(FailureForYou);
            _worldPacket << int32(BagResult);
            _worldPacket << int32(ItemID);
            break;
        case TRADE_STATUS_INITIATED:
            _worldPacket << uint32(ID);
            break;
        case TRADE_STATUS_PROPOSED:
            _worldPacket << Partner;
            _worldPacket << PartnerAccount;
            break;
        case TRADE_STATUS_WRONG_REALM:
        case TRADE_STATUS_NOT_ON_TAPLIST:
            _worldPacket << uint8(TradeSlot);
            break;
        case TRADE_STATUS_NOT_ENOUGH_CURRENCY:
        case TRADE_STATUS_CURRENCY_NOT_TRADABLE:
            _worldPacket << int32(CurrencyType);
            _worldPacket << int32(CurrencyQuantity);
            break;
        default:
            _worldPacket.FlushBits();
            break;
    }

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& buffer, WorldPackets::Trade::TradeUpdated::UnwrappedTradeItem const& unwrappedTradeItem)
{
    buffer << int32(unwrappedTradeItem.EnchantID);
    buffer << int32(unwrappedTradeItem.OnUseEnchantmentID);
    buffer << unwrappedTradeItem.Creator;
    buffer << int32(unwrappedTradeItem.Charges);
    buffer << uint32(unwrappedTradeItem.MaxDurability);
    buffer << uint32(unwrappedTradeItem.Durability);
    buffer.WriteBits(unwrappedTradeItem.UnkDatas.size(), 2);
    buffer.WriteBit(unwrappedTradeItem.Lock);
    buffer.FlushBits();

    for (auto const& v : unwrappedTradeItem.UnkDatas)
    {
        buffer << v.UnkByte;
        buffer << v.Item;
    }

    return buffer;
}

ByteBuffer& operator<<(ByteBuffer& buffer, WorldPackets::Trade::TradeUpdated::TradeItem const& tradeItem)
{
    buffer << uint8(tradeItem.Slot);
    buffer << tradeItem.Item;
    buffer << uint32(tradeItem.StackCount);
    buffer << tradeItem.GiftCreator;
    if (buffer.WriteBit(tradeItem.Unwrapped.is_initialized()))
        buffer << *tradeItem.Unwrapped;

    return buffer;
}

WorldPacket const* WorldPackets::Trade::TradeUpdated::Write()
{
    _worldPacket << uint8(WhichPlayer);
    _worldPacket << uint32(ID);
    _worldPacket << uint32(ClientStateIndex);
    _worldPacket << uint32(CurrentStateIndex);
    _worldPacket << uint64(Gold);
    _worldPacket << int32(CurrencyType);
    _worldPacket << int32(CurrencyQuantity);
    _worldPacket << int32(ProposedEnchantment);
    _worldPacket << static_cast<uint32>(Items.size());

    for (TradeItem const& item : Items)
        _worldPacket << item;

    return &_worldPacket;
}
