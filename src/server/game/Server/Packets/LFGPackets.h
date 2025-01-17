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

#ifndef LFGPackets_h__
#define LFGPackets_h__

#include "Packet.h"
#include "G3D/Vector3.h"
#include "Object.h"
#include "Unit.h"
#include "LFG.h"
#include "LFGMgr.h"
#include "PacketUtilities.h"
#include "QuestDef.h"
#include "Player.h"

namespace WorldPackets
{
    namespace LFG
    {
        struct RideTicket
        {
            ObjectGuid RequesterGuid;
            int32 Id = 0;
            int32 Type = 0;
            uint32 Time = 0;
        };

        struct BlackList
        {
            void Initialize(::lfg::LfgLockMap const& lock, ObjectGuid const& = ObjectGuid::Empty);

            struct BlackListInfo
            {
                uint32 Slot = 0;
                uint32 Reason = 0;
                uint32 SubReason1 = 0;
                uint32 SubReason2 = 0;
            };

            Optional<ObjectGuid> PlayerGuid;
            std::vector<BlackListInfo> blackListData;
        };

        struct LFGListBlacklist
        {
            uint32 ActivityID = 0;
            uint32 Reason = 0;
        };

        struct BootInfo
        {
            bool VoteInProgress = false;
            bool VotePassed = false;
            bool MyVoteCompleted = false;
            bool MyVote = false;
            ObjectGuid Target;
            uint32 TotalVotes = 0;
            uint32 BootVotes = 0;
            uint32 TimeLeft = 0;
            uint32 VotesNeeded = 0;
            std::string Reason;
        };

        struct ListJoinRequest
        {
            uint32 ActivityID = 0;
            float RequiredItemLevel = 0.0f;
            std::string Name;
            std::string Comment;
            std::string VoiceChat;
        };

        class ListGetStatus final : public ClientPacket //@TODO
        {
        public:
            ListGetStatus(WorldPacket&& packet) : ClientPacket(CMSG_LFG_LIST_GET_STATUS, std::move(packet)) { }

            void Read() override { }
        };

        class RequestLfgListBlackList final : public ClientPacket //@TODO
        {
        public:
            RequestLfgListBlackList(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_LFG_LIST_BLACKLIST, std::move(packet)) { }

            void Read() override { }
        };

        struct ShortageReward
        {
            void Initialize(::Quest const* quest = nullptr, ::Quest const* bonusQuest = nullptr, ::Player* player = nullptr);

            struct PlayerQuestRewardItem
            {
                uint32 ItemID = 0;
                uint32 Quantity = 0;
            };

            struct PlayerQuestRewardCurrency
            {
                uint32 CurrencyID = 0;
                uint32 Quantity = 0;
            };

            uint32 Mask = 0;
            uint32 RewardMoney = 0;
            uint32 RewardXP = 0;
            Optional<uint32> RewardSpellID;
            std::vector<PlayerQuestRewardItem> Item;
            std::vector<PlayerQuestRewardCurrency> Currency;
            std::vector<PlayerQuestRewardCurrency> BonusCurrency;
        };

        struct PlayerDungeonInfo
        {
            uint32 Slot = 0;
            bool FirstReward = false;
            bool ShortageEligible = false;

            uint32 CompletionQuantity = 0;
            uint32 CompletionLimit = 0;
            uint32 CompletionCurrencyID = 0;
            uint32 SpecificQuantity = 0;
            uint32 SpecificLimit = 0;
            uint32 OverallQuantity = 0;
            uint32 OverallLimit = 0;
            uint32 PurseWeeklyQuantity = 0;
            uint32 PurseWeeklyLimit = 0;
            uint32 PurseQuantity = 0;
            uint32 PurseLimit = 0;
            uint32 Quantity = 0;
            uint32 CompletedMask = 0;

            std::vector<ShortageReward> ShortageRewards;
            ShortageReward Reward;
        };

        class PlayerInfo final : public ServerPacket
        {
        public:
            PlayerInfo() : ServerPacket(SMSG_LFG_PLAYER_INFO, 5 + 4) { }

            WorldPacket const* Write() override;

            std::vector<PlayerDungeonInfo> Dungeon;
            BlackList BlackListMap;
        };

        class JoinResult final : public ServerPacket
        {
        public:
            JoinResult() : ServerPacket(SMSG_LFG_JOIN_RESULT, 4 + 28 + 1 + 1) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            lfg::LfgJoinResult Result = lfg::LFG_JOIN_OK;
            lfg::LfgRoleCheckState ResultDetail = lfg::LFG_ROLECHECK_DEFAULT;
            std::vector<BlackList> blackList;
        };

        class QueueStatusUpdate final : public ServerPacket
        {
        public:
            QueueStatusUpdate() : ServerPacket(SMSG_LFG_UPDATE_STATUS, 28 + 1 + 1 + 1 + 4 + 4 + 4 + 5 + 2) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            GuidVector SuspendedPlayers;
            std::vector<uint32> Slots;
            uint32 RequestedRoles = 0;
            uint8 SubType = 0;
            uint8 Reason = 0;
            bool IsParty = false;
            bool NotifyUI = false;
            bool Joined = false;
            bool LfgJoined = false;
            bool Queued = false;
        };

        class LockInfoRequest final : public ClientPacket
        {
        public:
            LockInfoRequest(WorldPacket&& packet) : ClientPacket(CMSG_DF_GET_SYSTEM_INFO, std::move(packet)) { }

            void Read() override;

            bool Player = 0;
            uint8 PartyIndex = 0;
        };

        class QueueStatus final : public ServerPacket
        {
        public:
            QueueStatus() : ServerPacket(SMSG_LFG_QUEUE_STATUS, 28 + 4 + 4 + 4 + 4 + 1 + 4) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            uint32 Slot = 0;
            uint32 AvgWaitTime = 0;
            uint32 QueuedTime = 0;
            uint32 AvgWaitTimeByRole[3] = { };
            uint8 LastNeeded[3] = { };
            uint32 AvgWaitTimeMe = 0;
        };

        class ProposalUpdate final : public ServerPacket
        {
        public:
            ProposalUpdate() : ServerPacket(SMSG_LFG_PROPOSAL_UPDATE, 28 + 8 + 4 + 4 + 1 + 4 + 1 + 1 + 4) { }

            WorldPacket const* Write() override;

            struct ProposalUpdatePlayer
            {
                uint32 Roles = 0;
                bool Me = false;
                bool SameParty = false;
                bool MyParty = false;
                bool Responded = false;
                bool Accepted = false;
            };

            RideTicket Ticket;
            uint64 InstanceID = 0;
            uint32 ProposalID = 0;
            uint32 Slot = 0;
            uint8 State = 0;
            uint32 CompletedMask = 0;
            bool ValidCompletedMask = false;
            bool ProposalSilent = false;
            std::vector<ProposalUpdatePlayer> Players;
        };

        class PlayerReward final : public ServerPacket
        {
        public:
            PlayerReward() : ServerPacket(SMSG_LFG_PLAYER_REWARD, 4 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            struct PlayerRewards
            {
                uint32 RewardItem = 0;
                uint32 RewardItemQuantity = 0;
                uint32 BonusCurrency = 0;
                bool IsCurrency = false;
            };

            std::vector<PlayerRewards> Players;
            uint32 ActualSlot = 0;
            uint32 QueuedSlot = 0;
            uint32 RewardMoney = 0;
            uint32 AddedXP = 0;
        };

        class DFJoin final : public ClientPacket
        {
        public:
            DFJoin(WorldPacket&& packet) : ClientPacket(CMSG_DF_JOIN, std::move(packet)) { }

            void Read() override;

            bool QueueAsGroup = 0;
            bool UnkBit = 0;
            uint8 PartyIndex = 0;
            uint32 Roles = 0;
            lfg::LfgDungeonSet Slot;
        };

        class RoleCheckUpdate final : public ServerPacket
        {
        public:
            RoleCheckUpdate() : ServerPacket(SMSG_LFG_ROLE_CHECK_UPDATE, 4 + 4 + 1 + 1 + 8 + 1 + 1 + 4) { }

            WorldPacket const* Write() override;

            struct CheckUpdateMember
            {
                ObjectGuid Guid;
                uint32 RolesDesired = 0;
                uint8 Level = 0;
                bool RoleCheckComplete = false;
            };

            std::vector<CheckUpdateMember> Members;
            std::vector<uint32> JoinSlots;
            uint8 PartyIndex = 0;
            uint8 RoleCheckStatus = 0;
            uint64 BgQueueID = 0;
            uint32 ActivityID = 0;
            bool IsBeginning = false;
            bool ShowRoleCheck = false;
        };

        class RoleChosen final : public ServerPacket
        {
        public:
            RoleChosen() : ServerPacket(SMSG_ROLE_CHOSEN, 16 + 4 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid Player;
            uint32 RoleMask = 0;
            bool Accepted = false;
        };

        class PartyInfo final : public ServerPacket
        {
        public:
            PartyInfo() : ServerPacket(SMSG_LFG_PARTY_INFO, 4) { }

            WorldPacket const* Write() override;

            std::vector<BlackList> Player;
        };

        class BootPlayer final : public ServerPacket
        {
        public:
            BootPlayer() : ServerPacket(SMSG_LFG_BOOT_PLAYER, 2 + 1 + 1 + 1 + 16 + 4 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            BootInfo Info;
        };

        class DFBootPlayerVote final : public ClientPacket
        {
        public:
            DFBootPlayerVote(WorldPacket&& packet) : ClientPacket(CMSG_DF_BOOT_PLAYER_VOTE, std::move(packet)) { }

            void Read() override;

            bool Vote = false;
        };

        struct ProposalResponse
        {
            RideTicket Ticket;
            uint64 InstanceID = 0;
            uint32 ProposalID = 0;
            bool Accepted = false;
        };

        class DFProposalResponse final : public ClientPacket
        {
        public:
            DFProposalResponse(WorldPacket&& packet) : ClientPacket(CMSG_DF_PROPOSAL_RESPONSE, std::move(packet)) { }

            void Read() override;

            ProposalResponse Data;
        };

        class ListUpdateBlacklist final : public ServerPacket
        {
        public:
            ListUpdateBlacklist() : ServerPacket(SMSG_LFG_LIST_UPDATE_BLACKLIST, 4) { }

            WorldPacket const* Write() override;

            std::vector<LFGListBlacklist> Blacklist;
        };

        class ListUpdateStatus final : public ServerPacket
        {
        public:
            ListUpdateStatus() : ServerPacket(SMSG_LFG_LIST_UPDATE_STATUS, 28 + 1 + 1 + 4 + 4 + 2 + 2 + 2) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            ListJoinRequest Request;
            uint8 Reason = 0;
            bool Listed = false;
        };

        class TeleportDenied final : public ServerPacket
        {
        public:
            TeleportDenied(uint8 reason) : ServerPacket(SMSG_LFG_TELEPORT_DENIED, 1), Reason(reason) { }

            WorldPacket const* Write() override;

            uint8 Reason = 0;
        };

        class ListInviteResponse final : public ClientPacket //@TODO
        {
        public:
            ListInviteResponse(WorldPacket&& packet) : ClientPacket(CMSG_LFG_LIST_INVITE_RESPONSE, std::move(packet)) { }

            void Read() override;

            RideTicket Ticket;
            bool Accept = false;
        };

        class DFSetComment final : public ClientPacket
        {
        public:
            DFSetComment(WorldPacket&& packet) : ClientPacket(CMSG_DF_SET_COMMENT, std::move(packet)) { }

            void Read() override;

            RideTicket Ticket;
            std::string Comment;
        };

        class DFTeleport final : public ClientPacket
        {
        public:
            DFTeleport(WorldPacket&& packet) : ClientPacket(CMSG_DF_TELEPORT, std::move(packet)) { }

            void Read() override;

            bool TeleportOut = false;
        };

        class DFSetRoles final : public ClientPacket
        {
        public:
            DFSetRoles(WorldPacket&& packet) : ClientPacket(CMSG_DF_SET_ROLES, std::move(packet)) { }

            void Read() override;

            uint32 RolesDesired = 0;
            uint8 PartyIndex = 0;
        };

        class DFLeave final : public ClientPacket
        {
        public:
            DFLeave(WorldPacket&& packet) : ClientPacket(CMSG_DF_LEAVE, std::move(packet)) { }

            void Read() override;

            RideTicket Ticket;
        };

        class ListJoin final : public ClientPacket //@TODO
        {
        public:
            ListJoin(WorldPacket&& packet) : ClientPacket(CMSG_LFG_LIST_JOIN, std::move(packet)) { }

            void Read() override;

            ListJoinRequest Request;
        };

        class ListLeave final : public ClientPacket //@TODO
        {
        public:
            ListLeave(WorldPacket&& packet) : ClientPacket(CMSG_LFG_LIST_LEAVE, std::move(packet)) { }

            void Read() override;

            RideTicket Ticket;
        };

        class BonusFactionID final : public ClientPacket
        {
        public:
            BonusFactionID(WorldPacket&& packet) : ClientPacket(CMSG_SET_LFG_BONUS_FACTION_ID, std::move(packet)) { }

            void Read() override;

            uint32 FactionID = 0;
        };

        //< CMSG_DF_GET_JOIN_STATUS
        class NullCmsg final : public ClientPacket
        {
        public:
            NullCmsg(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override { }
        };

        //< SMSG_LFG_DISABLED
        class NullSmsg final : public ServerPacket
        {
        public:
            NullSmsg(OpcodeServer opcode) : ServerPacket(opcode, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class DFSearchJoin final : public ClientPacket //@TODO
        {
        public:
            DFSearchJoin(WorldPacket&& packet) : ClientPacket(CMSG_DF_SEARCH_JOIN, std::move(packet)) { }

            void Read() override;

            int32 Slot = 0;
        };

        class LFGListJoinResult final : public ServerPacket
        {
        public:
            LFGListJoinResult() : ServerPacket(SMSG_LFG_LIST_JOIN_RESULT, 28 + 1 + 1) { }

            WorldPacket const* Write() override;

            RideTicket Ticket;
            uint8 ResultDetail = 0;
            uint8 Result = 0;
        };

        class DFSearchLeave final : public ClientPacket //@TODO
        {
        public:
            DFSearchLeave(WorldPacket&& packet) : ClientPacket(CMSG_DF_SEARCH_LEAVE, std::move(packet)) { }

            void Read() override;

            int32 Slot = 0;
        };

        class LFGOfferContinue final : public ServerPacket
        {
        public:
            LFGOfferContinue(uint32 slot) : ServerPacket(SMSG_LFG_OFFER_CONTINUE, 4), Slot(slot) { }

            WorldPacket const* Write() override;

            uint32 Slot = 0;
        };
    }
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::LFG::RideTicket& ticket);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::RideTicket const& ticket);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::BlackList const& blackList);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::LFGListBlacklist const& blackList);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::LFG::LFGListBlacklist& blackList);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::BootInfo const& boot);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::ListJoinRequest const& join);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::LFG::ListJoinRequest& join);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::LFG::ShortageReward const& reward);


#endif // LFGPackets_h__
