/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "AuthenticationPackets.h"
#include "BattlePayPackets.h"
#include "ClientConfigPackets.h"
#include "CharacterPackets.h"
#include "Opcodes.h"
#include "SharedDefines.h"
#include "SystemPackets.h"
#include "TokenPackets.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::SendAuthResponse(uint8 code, bool hasAccountData, bool queued, uint32 queuePos)
{
    //! WTF?
    WorldPacket data(SMSG_DANCE_STUDIO_CREATE_RESULT, 9);
    data << uint8(128);           //onebit
    data << uint32(795877);
    data << uint32(10848087);
    data << uint32(1084761);
    data << uint32(4665878);
    SendPacket(&data);

    WorldPackets::Auth::AuthResponse response;
    response.Result = code;

    if (queued)
    {
        response.WaitInfo = boost::in_place();
        response.WaitInfo->WaitCount = queuePos;
    }

    if (code == AUTH_OK)
    {
        response.SuccessInfo = boost::in_place();

        response.SuccessInfo->AccountExpansionLevel = Expansion();
        response.SuccessInfo->ActiveExpansionLevel = Expansion();
        response.SuccessInfo->VirtualRealmAddress = GetVirtualRealmAddress();
        response.SuccessInfo->CurrencyID = CURRENCY_RUB;
        response.SuccessInfo->TimeRemain = 0;

        std::string realmName = sObjectMgr->GetRealmName(realm.Id.Realm);
        response.SuccessInfo->VirtualRealms.emplace_back(GetVirtualRealmAddress(), true, false, realmName, realmName);
        response.SuccessInfo->AvailableClasses = &sObjectMgr->GetClassExpansionRequirements();
        response.SuccessInfo->AvailableRaces = &sObjectMgr->GetRaceExpansionRequirements();
        response.SuccessInfo->IsExpansionTrial = false;
        response.SuccessInfo->IsVeteranTrial = false;
    }

    SendPacket(response.Write());
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPackets::ClientConfig::ClientCacheVersion cache;
    cache.CacheVersion = version;
    SendPacket(cache.Write());
}

void WorldSession::SendDisplayPromo(int32 promo)
{
    SendPacket(WorldPackets::ClientConfig::DisplayPromotion(promo).Write());
}

void WorldSession::SendFeatureSystemStatusGlueScreen()
{
    WorldPackets::System::FeatureSystemStatusGlueScreen features;
    features.TokenPollTimeSeconds = 300;
    features.TokenRedeemIndex = 0;
    features.BpayStoreAvailable = true;
    features.BpayStoreDisabledByParentalControls = false;
    features.CharUndeleteEnabled = HasAuthFlag(AT_AUTH_FLAG_RESTORE_DELETED_CHARACTER);
    features.BpayStoreEnabled = sWorld->getBoolConfig(CONFIG_FEATURE_SYSTEM_BPAY_STORE_ENABLED);
    features.CommerceSystemEnabled = true;
    features.Unk14 = true;
    features.WillKickFromWorld = false;
    SendPacket(features.Write());
}

void WorldSession::HandleGetUndeleteCharacterCooldownStatus(WorldPackets::Character::GetUndeleteCharacterCooldownStatus& /*packet*/)
{
    WorldPackets::Character::UndeleteCooldownStatusResponse response;
    response.OnCooldown = false;
    response.MaxCooldown = 0;
    response.CurrentCooldown = 0;
    SendPacket(response.Write());
}

void WorldSession::HandleWowTokenMarketPrice(WorldPackets::Token::RequestWowTokenMarketPrice& packet)
{
    WorldPackets::Token::WowTokenMarketPriceResponse response;
    response.CurrentMarketPrice = 60000 * GOLD;
    response.Result = TOKEN_RESULT_ERROR_DISABLED;
    response.UnkInt = packet.UnkInt;
    response.UnkInt2 = 14400;
    SendPacket(response.Write());
}

void WorldSession::HandleUpdateListedAuctionableTokens(WorldPackets::Token::UpdateListedAuctionableTokens& packet)
{
    WorldPackets::Token::UpdateListedAuctionableTokensResponse response;
    response.UnkInt = packet.UnkInt;
    response.Result = TOKEN_RESULT_ERROR_DISABLED;
    response.AuctionableTokenAuctionableList.resize(0);
    for (uint8 v : {0})
    {
        WorldPackets::Token::UpdateListedAuctionableTokensResponse::AuctionableTokenAuctionable token;
        token.BuyoutPrice = 60000 * GOLD;
        token.DistributionID = 0;
        token.DateCreated = 0;
        token.Owner = 0;
        token.EndTime = 0;
        response.AuctionableTokenAuctionableList.push_back(token);
    }
    SendPacket(response.Write());
}
