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
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "TicketMgr.h"
#include "TicketPackets.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleComplaint(WorldPackets::Ticket::Complaint& packet)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_COMPLAINTS);
    stmt->setUInt64(0, sObjectMgr->GenerateReportComplaintID());
    stmt->setUInt64(1, packet.Offender.PlayerGuid.GetCounter());
    stmt->setUInt8(2, packet.ComplaintType);
    stmt->setUInt32(3, packet.MailID); 
    stmt->setUInt32(4, packet.Offender.TimeSinceOffence);
    stmt->setString(5, packet.Chat.MessageLog);
    CharacterDatabase.Execute(stmt);

    WorldPackets::Ticket::ComplaintResult result;
    result.ComplaintType = packet.ComplaintType;
    result.Result = 0;
    SendPacket(result.Write());
}

void WorldSession::HandleSupportTicketSubmitBug(WorldPackets::Ticket::SupportTicketSubmitBug& packet)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);
    stmt->setUInt64(0, sObjectMgr->GenerateSupportTicketSubmitBugID());
    stmt->setUInt64(1, _player->GetGUID().GetCounter());
    stmt->setString(2, packet.Note);
    stmt->setUInt32(3, packet.Header.MapID);
    stmt->setFloat(4, packet.Header.Pos.GetPositionX());
    stmt->setFloat(5, packet.Header.Pos.GetPositionY());
    stmt->setFloat(6, packet.Header.Pos.GetPositionZ());
    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleGMTicketGetSystemStatus(WorldPackets::Ticket::GMTicketGetSystemStatus& /*packet*/)
{
    // Note: This only disables the ticket UI at client side and is not fully reliable
    WorldPacket data(SMSG_GM_TICKET_SYSTEM_STATUS, 4);
    data << uint32(sTicketMgr->GetStatus() ? GMTICKET_QUEUE_STATUS_ENABLED : GMTICKET_QUEUE_STATUS_DISABLED);
    SendPacket(&data);
}

