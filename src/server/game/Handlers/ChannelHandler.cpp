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
 
#include <cctype>

#include "ChannelMgr.h"
#include "ChannelPackets.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WordFilterMgr.h"
#include "WorldSession.h"

void WorldSession::HandleJoinChannel(WorldPackets::Channel::JoinChannel& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (packet.ChatChannelId)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(packet.ChatChannelId);
        if (!channel)
            return;

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(player->GetZoneId());
        if (!zone || !player->CanJoinConstantChannelInZone(channel, zone))
            return;
    }

    if (packet.ChannelName.empty())
        return;

    if (isdigit(packet.ChannelName[0]))
        return;

    if (ChannelMgr* cMgr = channelMgr(player->GetTeam()))
    {
        cMgr->team = player->GetTeam();
        if (Channel* channel = cMgr->GetJoinChannel(packet.ChannelName, packet.ChatChannelId))
            channel->JoinChannel(player, packet.Password, true);
    }
}

void WorldSession::HandleLeaveChannel(WorldPackets::Channel::LeaveChannel& packet)
{
    Player* player = GetPlayer();
    if (packet.ChannelName.empty() || !player)
        return;

    if (ChannelMgr* cMgr = channelMgr(player->GetTeam()))
    {
        if (Channel* channel = cMgr->GetChannel(packet.ChannelName, player))
            channel->LeaveChannel(player, true, true);

        cMgr->LeftChannel(packet.ChannelName);
    }
}

template<void(Channel::*CommandFunction)(Player const*)>
void WorldSession::HandleChannelCommand(WorldPackets::Channel::ChannelPlayerCommand& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    if (ChannelMgr* cMgr = channelMgr(player->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(packet.ChannelName, player))
            (channel->*CommandFunction)(player);
}

template<void(Channel::*CommandFunction)(Player const*, std::string const&)>
void WorldSession::HandleChannelPlayerCommand(WorldPackets::Channel::ChannelPlayerCommand& packet)
{
    Player* player = GetPlayer();
    if (packet.Name.length() >= MAX_CHANNEL_NAME_STR || !player)
        return;

    if (!normalizePlayerName(packet.Name))
        return;

    if (ChannelMgr* cMgr = channelMgr(player->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(packet.ChannelName, player))
            (channel->*CommandFunction)(player, packet.Name);
}

template<>
void WorldSession::HandleChannelPlayerCommand<&Channel::Password>(WorldPackets::Channel::ChannelPlayerCommand& packet)
{
    Player* player = GetPlayer();
    if (packet.Name.length() > MAX_CHANNEL_PASS_STR || !player)
        return;

    if (ChannelMgr* cMgr = channelMgr(player->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(packet.ChannelName, player))
            channel->Password(player, packet.Name);
}

template void WorldSession::HandleChannelCommand<&Channel::Announce>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::Ban>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelCommand<&Channel::DeclineInvite>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::Invite>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::Kick>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelCommand<&Channel::List>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::SetModerator>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::SetMute>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelCommand<&Channel::SendWhoOwner>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::SetOwner>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::SilenceAll>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::SilenceVoice>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::UnBan>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::UnsetModerator>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::UnsetMute>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::UnsilenceAll>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelPlayerCommand<&Channel::UnsilenceVoice>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelCommand<&Channel::DeVoice>(WorldPackets::Channel::ChannelPlayerCommand&);
template void WorldSession::HandleChannelCommand<&Channel::Voice>(WorldPackets::Channel::ChannelPlayerCommand&);
