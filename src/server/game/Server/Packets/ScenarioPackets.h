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

#ifndef ScenarioPackets_h__
#define ScenarioPackets_h__

#include "Packet.h"
#include "BattlePetPackets.h"
#include "AchievementPackets.h"
#include "Object.h"

namespace WorldPackets
{
    namespace Scenario
    {
        class QueryScenarioPOI final : public ClientPacket
        {
        public:
            QueryScenarioPOI(WorldPacket&& packet) : ClientPacket(CMSG_QUERY_SCENARIO_POI, std::move(packet)) { }

            void Read() override;

            std::vector<uint32> MissingScenarioPOITreeIDs;
        };

        class ScenarioPOIs final : public ServerPacket
        {
        public:
            ScenarioPOIs() : ServerPacket(SMSG_SCENARIO_PO_IS, 4) { }

            WorldPacket const* Write() override;

            struct POIData
            {
                struct BlobData
                {
                    struct POIPointData
                    {
                        uint32 X = 0;
                        uint32 Y = 0;
                    };

                    uint32 BlobID = 0;
                    uint32 MapID = 0;
                    uint32 WorldMapAreaID = 0;
                    uint32 Floor = 0;
                    uint32 Priority = 0;
                    uint32 Flags = 0;
                    uint32 WorldEffectID = 0;
                    uint32 PlayerConditionID = 0;
                    std::vector<POIPointData> Points;
                };

                uint32 CriteriaTreeID = 0;
                std::vector<BlobData> BlobDatas;
            };

            std::vector<POIData> PoiInfos;
        };

        //< SMSG_SCENARIO_BOOT
        class NullSMsg final : public ServerPacket
        {
        public:
            NullSMsg(OpcodeServer opcode) : ServerPacket(opcode) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class ScenarioProgressUpdate final : public ServerPacket
        {
        public:
            ScenarioProgressUpdate() : ServerPacket(SMSG_SCENARIO_PROGRESS_UPDATE) { }

            WorldPacket const* Write() override;

            WorldPackets::Achievement::CriteriaTreeProgress Progress;
        };

        class ScenarioState final : public ServerPacket
        {
        public:
            ScenarioState() : ServerPacket(SMSG_SCENARIO_STATE, 33) { }

            WorldPacket const* Write() override;

            struct BonusObjective
            {
                uint32 BonusObjectiveID = 0;
                bool ObjectiveComplete = false;
            };

            struct UnkData
            {
                uint32 UnkInt = 0;
                bool UnkBool = false;
            };
            
            std::vector<BonusObjective> Objectives;
            std::vector<WorldPackets::Achievement::CriteriaTreeProgress> Progress;
            std::vector<UnkData> UnkDatas;
            std::vector<uint32> ActiveSteps;
            uint32 ScenarioID = 0;
            int32 CurrentStep = 0;
            uint32 DifficultyID = 0;
            uint32 WaveCurrent = 0;
            uint32 WaveMax = 0;
            uint32 TimerDuration = 0;
            bool ScenarioComplete = false;
        };

        class ScenarioCompleted final : public ServerPacket
        {
        public:
            ScenarioCompleted() : ServerPacket(SMSG_SCENARIO_COMPLETED, 4) { }

            WorldPacket const* Write() override;

            uint32 ScenarioID = 0;
        };
    }
}

#endif // ScenarioPackets_h__
