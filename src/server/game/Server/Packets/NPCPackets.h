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

#ifndef NPCPackets_h__
#define NPCPackets_h__

#include "Packet.h"
#include "ItemPackets.h"
#include "Creature.h"

namespace WorldPackets
{
    namespace NPC
    {
        // CMSG_BANKER_ACTIVATE
        // CMSG_BINDER_ACTIVATE
        // CMSG_TALK_TO_GOSSIP
        // CMSG_LIST_INVENTORY
        // CMSG_TRAINER_LIST
        class Hello final : public ClientPacket
        {
        public:
            Hello(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            ObjectGuid Unit;
        };

        struct ClientGossipOptions
        {
            int32 ClientOption = 0;
            uint8 OptionNPC = 0;
            uint8 OptionFlags = 0;
            int32 OptionCost = 0;
            std::string Text;
            std::string Confirm;
        };

        struct ClientGossipText
        {
            int32 QuestID = 0;
            int32 QuestType = 0;
            int32 QuestLevel = 0;
            bool Repeatable = false;
            std::string QuestTitle;
            int32 QuestFlags[2] = { };
        };

        class GossipMessage final : public ServerPacket
        {
        public:
            GossipMessage() : ServerPacket(SMSG_GOSSIP_MESSAGE, 4 + 4 + 16 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            std::vector<ClientGossipOptions> GossipOptions;
            int32 FriendshipFactionID = 0;
            ObjectGuid GossipGUID;
            std::vector<ClientGossipText> GossipText;
            int32 TextID = 0;
            int32 GossipID = 0;
        };

        class GossipSelectOption final : public ClientPacket
        {
        public:
            GossipSelectOption(WorldPacket&& packet) : ClientPacket(CMSG_GOSSIP_SELECT_OPTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid GossipUnit;
            int32 GossipIndex = 0;
            int32 GossipID = 0;
            std::string PromotionCode;
        };

        class GossipComplete final : public ServerPacket
        {
        public:
            GossipComplete() : ServerPacket(SMSG_GOSSIP_COMPLETE, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        struct VendorItem
        {
            int32 MuID = 0;
            int32 Type = 0;
            WorldPackets::Item::ItemInstance Item;
            int32 Quantity = -1;
            int32 Price = 0;
            int32 Durability = 0;
            int32 StackCount = 0;
            int32 ExtendedCostID = 0;
            int32 PlayerConditionFailed = 0;
            bool DoNotFilterOnVendor = false;
        };

        class VendorInventory final : public ServerPacket
        {
        public:
            VendorInventory() : ServerPacket(SMSG_VENDOR_INVENTORY, 1 + 4 + 16) { }

            WorldPacket const* Write() override;

            uint8 Reason = 0;
            std::vector<VendorItem> Items;
            ObjectGuid Vendor;
        };

        struct TrainerListSpell
        {
            int32 SpellID = 0;
            int32 MoneyCost = 0;
            int32 ReqSkillLine = 0;
            int32 ReqSkillRank = 0;
            int32 ReqAbility[MAX_TRAINERSPELL_ABILITY_REQS] = { };
            uint8 Usable = 0;
            uint8 ReqLevel = 0;
        };

        class TrainerList final : public ServerPacket
        {
        public:
            TrainerList() : ServerPacket(SMSG_TRAINER_LIST, 2 + 4 + 16 + 4 + 4) { }

            WorldPacket const* Write() override;

            std::string Greeting;
            int32 TrainerType = 0;
            ObjectGuid TrainerGUID;
            int32 TrainerID = 1;
            std::vector<TrainerListSpell> Spells;
        };

        class ShowBank final : public ServerPacket
        {
        public:
            ShowBank() : ServerPacket(SMSG_SHOW_BANK, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class PlayerTabardVendorActivate final : public ServerPacket
        {
        public:
            PlayerTabardVendorActivate() : ServerPacket(SMSG_PLAYER_TABARD_VENDOR_ACTIVATE, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Vendor;
        };

        class SuppressNPCGreetings final : public ServerPacket
        {
        public:
            SuppressNPCGreetings() : ServerPacket(SMSG_SUPPRESS_NPC_GREETINGS, 16 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            bool SuppressNPCGreeting = false;
        };

        class GossipPOI final : public ServerPacket
        {
        public:
            GossipPOI() : ServerPacket(SMSG_GOSSIP_POI, 2 + 2 * 4 + 4 + 4 + 1) { }

            WorldPacket const* Write() override;

            uint32 Flags = 0;
            Position Pos;
            int32 Icon = 0;
            int32 Importance = 0;
            std::string Name;
        };

        class SpiritHealerActivate final : public ClientPacket
        {
        public:
            SpiritHealerActivate(WorldPacket&& packet) : ClientPacket(CMSG_SPIRIT_HEALER_ACTIVATE, std::move(packet)) { }

            void Read() override;

            ObjectGuid Healer;
        };

        class SpiritHealerConfirm final : public ServerPacket
        {
        public:
            SpiritHealerConfirm() : ServerPacket(SMSG_SPIRIT_HEALER_CONFIRM, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Unit;
        };

        class TrainerBuySpell final : public ClientPacket
        {
        public:
            TrainerBuySpell(WorldPacket&& packet) : ClientPacket(CMSG_TRAINER_BUY_SPELL, std::move(packet)) { }

            void Read() override;

            ObjectGuid TrainerGUID;
            int32 TrainerID     = 0;
            int32 SpellID       = 0;
        };

        class TrainerBuyFailed final : public ServerPacket
        {
        public:
            TrainerBuyFailed() : ServerPacket(SMSG_TRAINER_BUY_FAILED, 16 + 4 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid TrainerGUID;
            int32 SpellID = 0;
            int32 TrainerFailedReason = 0;
        };

        class RequestStabledPets final : public ClientPacket
        {
        public:
            RequestStabledPets(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_STABLED_PETS, std::move(packet)) { }
        
            void Read() override;

            ObjectGuid StableMaster;
        };
    }
}

#endif // NPCPackets_h__
