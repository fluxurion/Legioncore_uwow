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
    if (packet.ChatChannelId)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(packet.ChatChannelId);
        if (!channel)
            return;

        AreaTableEntry const* current_zone = sDB2Manager.GetAreaEntryByAreaID(_player->GetZoneId());
        if (!current_zone)
            return;

        if (!_player->CanJoinConstantChannelInZone(channel, current_zone))
            return;
    }

    if (packet.ChannelName.empty())
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        cMgr->team = _player->GetTeam();
        if (Channel* chn = cMgr->GetJoinChannel(packet.ChannelName, packet.ChatChannelId))
            chn->JoinChannel(_player, packet.Password.c_str(), true);
    }
}

void WorldSession::HandleLeaveChannel(WorldPackets::Channel::LeaveChannel& packet)
{
    if (packet.ChannelName.empty())
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* chn = cMgr->GetChannel(packet.ChannelName, _player))
            chn->LeaveChannel(_player, true, true);

        cMgr->LeftChannel(packet.ChannelName);
    }
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
