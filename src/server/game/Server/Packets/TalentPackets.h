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

#ifndef TalentPackets_h__
#define TalentPackets_h__

#include "Packet.h"
#include "Player.h"

namespace WorldPackets
{
    namespace Talent
    {
        struct TalentGroupInfo
        {
            uint32 SpecID;
            std::vector<uint16> TalentIDs;
            std::vector<uint16> PvPTalentIDs;
        };

        struct TalentInfoUpdate
        {
            std::vector<TalentGroupInfo> TalentGroups;
            uint32 ActiveSpecID = 0;
            uint8 ActiveGroup = 0;
        };

        class UpdateTalentData final : public ServerPacket
        {
        public:
            UpdateTalentData() : ServerPacket(SMSG_UPDATE_TALENT_DATA, 2 + 4 + 4 + 4 + 12) { }

            WorldPacket const* Write() override;

            TalentInfoUpdate Info;
        };

        class SetSpecialization final : public ClientPacket
        {
        public:
            SetSpecialization(WorldPacket&& packet) : ClientPacket(CMSG_SET_SPECIALIZATION, std::move(packet)) { }

            void Read() override;

            uint32 SpecGroupIndex = 0;
        };

        class LearnTalent final : public ClientPacket
        {
        public:
            LearnTalent(WorldPacket&& packet) : ClientPacket(std::move(packet))
            {
                ASSERT(packet.GetOpcode() == CMSG_LEARN_TALENTS);
            }

            void Read() override;
            std::vector<uint16> Talents;
        };

        class LearnPvpTalents final : public ClientPacket
        {
        public:
            LearnPvpTalents(WorldPacket&& packet) : ClientPacket(CMSG_LEARN_PVP_TALENTS, std::move(packet)) { }

            void Read() override;

            Array<uint16, MAX_PVP_TALENTS> TalentIDs;
        };
    }
}

#endif // TalentPackets_h__
