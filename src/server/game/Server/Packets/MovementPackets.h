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

#ifndef MovementPackets_h__
#define MovementPackets_h__

#include "Packet.h"
#include "Object.h"

#include <G3D/Vector3.h>

namespace Movement
{
    class MoveSpline;
}

namespace WorldPackets
{
    namespace Movement
    {
        class ClientPlayerMovement final : public ClientPacket
        {
        public:
            ClientPlayerMovement(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            MovementInfo movementInfo;
        };

        class MoveUpdate final : public ServerPacket
        {
        public:
            MoveUpdate() : ServerPacket(SMSG_MOVE_UPDATE) { }

            WorldPacket const* Write() override;

            MovementInfo* movementInfo = nullptr;
        };

        struct MonsterSplineFilterKey
        {
            int16 Idx = 0;
            int16 Speed = 0;
        };

        struct MonsterSplineFilter
        {
            std::vector<MonsterSplineFilterKey> FilterKeys;
            uint8 FilterFlags = 0;
            float BaseSpeed = 0.0f;
            int16 StartOffset = 0;
            float DistToPrevFilterKey = 0.0f;
            int16 AddedToStart = 0;
        };

        struct MovementSpline
        {
            uint32 Flags = 0;
            uint8 Face = 0;
            uint8 AnimTier = 0;
            uint32 TierTransStartTime = 0;
            uint32 Elapsed = 0;
            uint32 MoveTime = 0;
            float JumpGravity = 0.0f;
            uint32 SpecialTime = 0;
            std::vector<G3D::Vector3> Points;
            uint8 Mode = 0;
            uint8 VehicleExitVoluntary = 0;
            ObjectGuid TransportGUID;
            uint8 VehicleSeat = 255;
            std::vector<G3D::Vector3> PackedDeltas;
            Optional<MonsterSplineFilter> SplineFilter;
            float FaceDirection = 0.0f;
            ObjectGuid FaceGUID;
            G3D::Vector3 FaceSpot;
        };

        struct MovementMonsterSpline
        {
            uint32 ID = 0;
            G3D::Vector3 Destination;
            bool CrzTeleport = false;
            MovementSpline Move;
        };

        class CommonMovement
        {
        public:
            static void WriteCreateObjectSplineDataBlock(::Movement::MoveSpline const& moveSpline, ByteBuffer& data);
        };

        class MonsterMove final : public ServerPacket
        {
        public:
            MonsterMove() : ServerPacket(SMSG_ON_MONSTER_MOVE) { }

            void InitializeSplineData(::Movement::MoveSpline const& moveSpline);

            WorldPacket const* Write() override;

            MovementMonsterSpline SplineData;
            ObjectGuid MoverGUID;
            G3D::Vector3 Pos;
        };

        class MoveSplineSetSpeed : public ServerPacket
        {
        public:
            MoveSplineSetSpeed(OpcodeServer opcode) : ServerPacket(opcode, 12) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            float Speed = 1.0f;
        };

        class MoveSetSpeed : public ServerPacket
        {
        public:
            MoveSetSpeed(OpcodeServer opcode) : ServerPacket(opcode) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
            float Speed = 1.0f;
        };

        class MoveUpdateSpeed : public ServerPacket
        {
        public:
            MoveUpdateSpeed(OpcodeServer opcode) : ServerPacket(opcode) { }

            WorldPacket const* Write() override;

            MovementInfo* movementInfo = nullptr;
            float Speed = 1.0f;
        };

        // SMSG_MOVE_SET_ACTIVE_MOVER
        // SMSG_MOVE_SPLINE_ROOT
        // SMSG_MOVE_SPLINE_UNROOT
        // SMSG_MOVE_SPLINE_DISABLE_GRAVITY
        // SMSG_MOVE_SPLINE_ENABLE_GRAVITY
        // SMSG_MOVE_SPLINE_DISABLE_COLLISION
        // SMSG_MOVE_SPLINE_ENABLE_COLLISION
        // SMSG_MOVE_SPLINE_SET_FEATHER_FALL
        // SMSG_MOVE_SPLINE_SET_NORMAL_FALL
        // SMSG_MOVE_SPLINE_SET_HOVER
        // SMSG_MOVE_SPLINE_UNSET_HOVER
        // SMSG_MOVE_SPLINE_SET_WATER_WALK
        // SMSG_MOVE_SPLINE_START_SWIM
        // SMSG_MOVE_SPLINE_STOP_SWIM
        // SMSG_MOVE_SPLINE_SET_RUN_MODE
        // SMSG_MOVE_SPLINE_SET_WALK_MODE
        // SMSG_MOVE_SPLINE_SET_FLYING
        // SMSG_MOVE_SPLINE_UNSET_FLYING
        // SMSG_MOVE_SPLINE_SET_LAND_WALK
        class MoveSplineSetFlag final : public ServerPacket
        {
        public:
            MoveSplineSetFlag(OpcodeServer opcode) : ServerPacket(opcode, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
        };

        // SMSG_MOVE_DISABLE_GRAVITY
        // SMSG_MOVE_DISABLE_TRANSITION_BETWEEN_SWIM_AND_FLY
        // SMSG_MOVE_ENABLE_GRAVITY
        // SMSG_MOVE_ENABLE_TRANSITION_BETWEEN_SWIM_AND_FLY
        // SMSG_MOVE_ROOT
        // SMSG_MOVE_SET_CAN_FLY
        // SMSG_MOVE_SET_FEATHER_FALL
        // SMSG_MOVE_SET_HOVERING
        // SMSG_MOVE_SET_IGNORE_MOVEMENT_FORCES
        // SMSG_MOVE_SET_LAND_WALK
        // SMSG_MOVE_SET_NORMAL_FALL
        // SMSG_MOVE_SET_WATER_WALK
        // SMSG_MOVE_UNROOT
        // SMSG_MOVE_UNSET_CAN_FLY
        // SMSG_MOVE_UNSET_IGNORE_MOVEMENT_FORCES
        // SMSG_MOVE_ENABLE_COLLISION
        // SMSG_MOVE_DISABLE_COLLISION
        // SMSG_MOVE_SET_CAN_TURN_WHILE_FALLING
        // SMSG_MOVE_UNSET_HOVERING
        class MoveSetFlag final : public ServerPacket
        {
        public:
            MoveSetFlag(OpcodeServer opcode) : ServerPacket(opcode, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 SequenceIndex = 0;
        };

        class TransferPending final : public ServerPacket
        {
        public:
            struct ShipTransferPending
            {
                uint32 ID = 0;
                int32 OriginMapID = -1;
            };

            TransferPending() : ServerPacket(SMSG_TRANSFER_PENDING, 16) { }

            WorldPacket const* Write() override;

            int32 MapID = -1;
            Optional<ShipTransferPending> Ship;
            Optional<int32> TransferSpellID;
        };

        class TransferAborted final : public ServerPacket
        {
        public:
            TransferAborted() : ServerPacket(SMSG_TRANSFER_ABORTED, 4 + 1 + 4) { }

            WorldPacket const* Write() override;

            uint32 TransfertAbort = 0;
            uint8 Arg = 0;
            uint32 MapID = 0;
        };

        class NewWorld final : public ServerPacket
        {
        public:
            NewWorld() : ServerPacket(SMSG_NEW_WORLD, 24) { }

            WorldPacket const* Write() override;

            int32 MapID = 0;
            uint32 Reason = 0;
            Position Pos;
        };

        class WorldPortAck final : public ClientPacket
        {
        public:
            WorldPortAck(WorldPacket&& packet) : ClientPacket(CMSG_WORLD_PORT_RESPONSE, std::move(packet)) { }

            void Read() override { }
        };

        struct VehicleTeleport
        {
            uint8 VehicleSeatIndex = 0;
            bool VehicleExitVoluntary = false;
            bool VehicleExitTeleport = false;
        };

        class MoveTeleport final : public ServerPacket
        {
        public:
            MoveTeleport() : ServerPacket(SMSG_MOVE_TELEPORT, 12 + 4 + 16 + 16 + 4) { }

            WorldPacket const* Write() override;

            Position Pos;
            Optional<VehicleTeleport> Vehicle;
            uint32 SequenceIndex = 0;
            ObjectGuid MoverGUID;
            Optional<ObjectGuid> TransportGUID;
            float Facing = 0.0f;
        };

        struct MovementForce
        {
            ObjectGuid ID;
            Position Direction;
            uint32 TransportID = 0;
            float Magnitude = 0;
            uint8 Type = 0;
        };

        class MoveUpdateTeleport final : public ServerPacket
        {
        public:
            MoveUpdateTeleport() : ServerPacket(SMSG_MOVE_UPDATE_TELEPORT) { }

            WorldPacket const* Write() override;

            MovementInfo* movementInfo = nullptr;
            std::vector<MovementForce> MovementForces;
            Optional<float> SwimBackSpeed;
            Optional<float> FlightSpeed;
            Optional<float> SwimSpeed;
            Optional<float> WalkSpeed;
            Optional<float> TurnRate;
            Optional<float> RunSpeed;
            Optional<float> FlightBackSpeed;
            Optional<float> RunBackSpeed;
            Optional<float> PitchRate;
        };

        class MoveTeleportAck final : public ClientPacket
        {
        public:
            MoveTeleportAck(WorldPacket&& packet) : ClientPacket(CMSG_MOVE_TELEPORT_ACK, std::move(packet)) { }

            void Read() override;

            ObjectGuid MoverGUID;
            int32 AckIndex = 0;
            int32 MoveTime = 0;
        };

        struct MovementAck
        {
            MovementInfo movementInfo;
            int32 AckIndex = 0;
        };

        class MovementAckMessage final : public ClientPacket
        {
        public:
            MovementAckMessage(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            MovementAck Ack;
        };

        class MovementSpeedAck final : public ClientPacket
        {
        public:
            MovementSpeedAck(WorldPacket&& packet) : ClientPacket(std::move(packet)) { }

            void Read() override;

            MovementAck Ack;
            float Speed = 0.0f;
        };

        class SetActiveMover final : public ClientPacket
        {
        public:
            SetActiveMover(WorldPacket&& packet) : ClientPacket(CMSG_SET_ACTIVE_MOVER, std::move(packet)) { }

            void Read() override;

            ObjectGuid ActiveMover;
        };

        class MoveUpdateKnockBack final : public ServerPacket
        {
        public:
            MoveUpdateKnockBack() : ServerPacket(SMSG_MOVE_UPDATE_KNOCK_BACK) { }

            WorldPacket const* Write() override;

            MovementInfo* movementInfo = nullptr;
        };

        enum UpdateCollisionHeightReason : uint8
        {
            UPDATE_COLLISION_HEIGHT_SCALE = 0,
            UPDATE_COLLISION_HEIGHT_MOUNT = 1,
            UPDATE_COLLISION_HEIGHT_FORCE = 2
        };

        struct CollisionHeightData
        {
            float Height = 0.0f;
            float Scale = 0.0f;
            UpdateCollisionHeightReason Reason = UPDATE_COLLISION_HEIGHT_MOUNT;
        };

        class MoveSetCollisionHeight final : public ServerPacket
        {
        public:
            MoveSetCollisionHeight() : ServerPacket(SMSG_MOVE_SET_COLLISION_HEIGHT, 4 + 16 + 4 + 1 + 4 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid MoverGUID;
            uint32 MountDisplayID = 0;
            uint32 SequenceIndex = 0;
            CollisionHeightData MsgData;
        };

        class MoveUpdateCollisionHeight final : public ServerPacket
        {
        public:
            MoveUpdateCollisionHeight() : ServerPacket(SMSG_MOVE_UPDATE_COLLISION_HEIGHT) { }

            WorldPacket const* Write() override;

            MovementInfo* movementInfo = nullptr;
            float Scale = 1.0f;
            float Height = 1.0f;
        };

        class MoveSetCollisionHeightAck final : public ClientPacket
        {
        public:
            MoveSetCollisionHeightAck(WorldPacket&& packet) : ClientPacket(CMSG_MOVE_SET_COLLISION_HEIGHT_ACK, std::move(packet)) { }

            void Read() override;

            MovementAck Data;
            CollisionHeightData MsgData;
            uint32 MountDisplayID = 0;
        };

        class MoveTimeSkipped final : public ClientPacket
        {
        public:
            MoveTimeSkipped(WorldPacket&& packet) : ClientPacket(CMSG_MOVE_TIME_SKIPPED, std::move(packet)) { }

            void Read() override;

            ObjectGuid MoverGUID;
            uint32 TimeSkipped = 0;
        };

        class SummonResponse final : public ClientPacket
        {
        public:
            SummonResponse(WorldPacket&& packet) : ClientPacket(CMSG_SUMMON_RESPONSE, std::move(packet)) { }

            void Read() override;

            bool Accept = false;
            ObjectGuid SummonerGUID;
        };

        class ControlUpdate final : public ServerPacket
        {
        public:
            ControlUpdate() : ServerPacket(SMSG_CONTROL_UPDATE, 16 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
            bool On = false;
        };

        class MoveSplineDone final : public ClientPacket
        {
        public:
            MoveSplineDone(WorldPacket&& packet) : ClientPacket(CMSG_MOVE_SPLINE_DONE, std::move(packet)) { }
            
            void Read() override;
            
            MovementInfo movementInfo;
            int32 SplineID;
        };

        class MoveUpdateRemoveMovementForce final : public ServerPacket
        {
        public:
            MoveUpdateRemoveMovementForce() : ServerPacket(SMSG_MOVE_UPDATE_REMOVE_MOVEMENT_FORCE) { }

            WorldPacket const* Write() override;
            
            MovementInfo* movementInfo = nullptr;
            ObjectGuid TriggerGUID;
        };

        class MoveUpdateApplyMovementForce final : public ServerPacket
        {
        public:
            MoveUpdateApplyMovementForce() : ServerPacket(SMSG_MOVE_UPDATE_APPLY_MOVEMENT_FORCE) { }

            WorldPacket const* Write() override;
            
            MovementInfo* movementInfo = nullptr;
            MovementForce MovementForceData;
        };

        struct KnockBackData
        {
            float HorzSpeed = 0.0f;
            Position Direction;
            float InitVertSpeed = 0.0f;
        };

        struct MoveStateChange
        {
            int16 MessageID = 0;
            uint32 SequenceIndex = 0;
            Optional<float> Speed;
            Optional<uint32> VehicleRecID;
            Optional<MovementForce> MovementForceData;
            Optional<ObjectGuid> MoverGUID;
            Optional<KnockBackData> KnockBack;
            Optional<CollisionHeightData> ColiisionHeight;
        };

        class MoveSetCompoundState final : public ServerPacket
        {
        public:
            MoveSetCompoundState() : ServerPacket(SMSG_MOVE_SET_COMPOUND_STATE, 16 + 4) { }

            WorldPacket const* Write() override;
            
            ObjectGuid MoverGUID;
            std::vector<MoveStateChange> Changes;
        };

        class FlightSplineSync final : public ServerPacket
        {
        public:
            FlightSplineSync() : ServerPacket(SMSG_FLIGHT_SPLINE_SYNC, 16 + 4) { }

            WorldPacket const* Write() override;
            
            ObjectGuid Guid;
            float SplineDist = 0.0f;
        };

        class AdjustSplineDuration final : public ServerPacket
        {
        public:
            AdjustSplineDuration() : ServerPacket(SMSG_ADJUST_SPLINE_DURATION, 16 + 4) { }

            WorldPacket const* Write() override;
            
            ObjectGuid Unit;
            float Scale = 0.0f;
        };

        class MoveKnockBack final : public ServerPacket
        {
        public:
            MoveKnockBack() : ServerPacket(SMSG_MOVE_KNOCK_BACK, 16 + 4 + 12) { }

            WorldPacket const* Write() override;
            
            ObjectGuid MoverGUID;
            Position Direction;
            int32 SequenceIndex = 0;
            float HorzSpeed = 0.0f;
            float VertSpeed = 0.0f;
        };
    }
}

ByteBuffer& operator>>(ByteBuffer& data, MovementInfo& movementInfo);
ByteBuffer& operator<<(ByteBuffer& data, MovementInfo& movementInfo);

ByteBuffer& operator>>(ByteBuffer& data, MovementInfo::TransportInfo& transportInfo);
ByteBuffer& operator<<(ByteBuffer& data, MovementInfo::TransportInfo const& transportInfo);

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MonsterSplineFilterKey const& monsterSplineFilterKey);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MonsterSplineFilter const& monsterSplineFilter);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MovementSpline const& movementSpline);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MovementMonsterSpline const& movementMonsterSpline);
ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Movement::MovementAck& movementAck);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MovementForce const& movementForce);

#endif // MovementPackets_h__
