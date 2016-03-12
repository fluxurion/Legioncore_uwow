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

#ifndef TicketPackets_h__
#define TicketPackets_h__

#include "Packet.h"
#include "LFGPackets.h"
#include "Position.h"

namespace WorldPackets
{
    namespace Ticket
    {
        class Complaint final : public ClientPacket
        {
        public:
            Complaint(WorldPacket&& packet) : ClientPacket(CMSG_COMPLAINT, std::move(packet)) { }

            void Read() override;

            enum SupportSpamType
            {
                SUPPORT_SPAM_TYPE_MAIL     = 0,
                SUPPORT_SPAM_TYPE_CHAT     = 1,
                SUPPORT_SPAM_TYPE_CALENDAR = 2
            };

            struct ComplaintOffender
            {
                ObjectGuid PlayerGuid;
                uint32 RealmAddress = 0;
                uint32 TimeSinceOffence = 0;
            };

            struct ComplaintChat
            {
                uint32 Command = 0;
                uint32 ChannelID = 0;
                std::string MessageLog;
            };

            ComplaintOffender Offender;
            ComplaintChat Chat;
            ObjectGuid EventGuid;
            ObjectGuid InviteGuid;
            uint32 MailID = 0;
            uint8 ComplaintType = 0;
        };

        class ComplaintResult final : public ServerPacket
        {
        public:
            ComplaintResult() : ServerPacket(SMSG_COMPLAINT_RESULT, 9) { }

            WorldPacket const* Write() override;

            uint32 ComplaintType = 0;
            uint8 Result = 0;
        };

        struct SupportTicketHeader
        {
            Position Pos;
            int32 MapID = 0;
        };

        class SupportTicketSubmitBug final : public ClientPacket
        {
        public:
            SupportTicketSubmitBug(WorldPacket&& packet) : ClientPacket(CMSG_SUPPORT_TICKET_SUBMIT_BUG, std::move(packet)) { }

            void Read() override;

            SupportTicketHeader Header;
            std::string Note;
        };

        class GMTicketGetSystemStatus final : public ClientPacket
        {
        public:
            GMTicketGetSystemStatus(WorldPacket&& packet) : ClientPacket(CMSG_GM_TICKET_GET_SYSTEM_STATUS, std::move(packet)) { }

            void Read() override { }
        };
    }
}

#endif // TicketPackets_h__
