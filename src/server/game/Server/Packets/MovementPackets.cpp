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

#include "MovementPackets.h"
#include "MoveSpline.h"
#include "MoveSplineFlag.h"
#include "MovementTypedefs.h"
#include "Unit.h"
#include "PacketUtilities.h"

ByteBuffer& operator<<(ByteBuffer& data, MovementInfo& movementInfo)
{
    bool hasTransportData = !movementInfo.transport.Guid.IsEmpty();
    bool hasFallDirection = movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR);
    bool hasFallData = hasFallDirection || movementInfo.fallTime != 0;

    data << movementInfo.Guid;
    data << movementInfo.MoveIndex;
    data << movementInfo.Pos.PositionXYZOStream();
    data << movementInfo.pitch;
    data << movementInfo.splineElevation;

    data << static_cast<uint32>(movementInfo.RemoveForcesIDs.size());
    uint32 int168 = 0;
    data << int168;

    for (ObjectGuid const& guid : movementInfo.RemoveForcesIDs)
        data << guid;

    data.WriteBits(movementInfo.MoveFlags[0], 30);
    data.WriteBits(movementInfo.MoveFlags[1], 18);

    data.WriteBit(hasTransportData);
    data.WriteBit(hasFallData);
    data.WriteBit(0); // HasSpline
    data.WriteBit(movementInfo.HeightChangeFailed);
    data.WriteBit(movementInfo.RemoteTimeValid);

    data.FlushBits();

    if (hasTransportData)
        data << movementInfo.transport;

    if (hasFallData)
    {
        data << movementInfo.fallTime;
        data << movementInfo.jump.zspeed;

        if (data.WriteBit(hasFallDirection))
        {
            data << movementInfo.jump.sinAngle;
            data << movementInfo.jump.cosAngle;
            data << movementInfo.jump.xyspeed;
        }
    }

    data.FlushBits();

    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, MovementInfo& movementInfo)
{
    data >> movementInfo.Guid;
    data >> movementInfo.MoveIndex;
    data >> movementInfo.Pos.PositionXYZOStream();
    data >> movementInfo.pitch;
    data >> movementInfo.splineElevation;

    uint32 removeMovementForcesCount;
    data >> removeMovementForcesCount;

    uint32 MoveTime;
    data >> MoveTime;

    for (uint32 i = 0; i < removeMovementForcesCount; ++i)
    {
        ObjectGuid guid;
        data >> guid;
    }

    data.ResetBitPos();

    movementInfo.MoveFlags[0] = data.ReadBits(30);
    movementInfo.MoveFlags[1] = data.ReadBits(18);

    bool hasTransport = data.ReadBit();
    bool hasFall = data.ReadBit();
    bool hasSpline = data.ReadBit();

    data.ReadBit(movementInfo.HeightChangeFailed);
    data.ReadBit(movementInfo.RemoteTimeValid);

    if (hasTransport)
        data >> movementInfo.transport;

    if (hasFall)
    {
        data >> movementInfo.fallTime;
        data >> movementInfo.jump.zspeed;

        data.ResetBitPos();

        if (data.ReadBit())
        {
            data >> movementInfo.jump.sinAngle;
            data >> movementInfo.jump.cosAngle;
            data >> movementInfo.jump.xyspeed;
        }
    }

    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, MovementInfo::TransportInfo& transportInfo)
{
    data >> transportInfo.Guid;
    data >> transportInfo.Pos.PositionXYZOStream();
    data >> transportInfo.VehicleSeatIndex;
    data >> transportInfo.MoveTime;

    bool hasPrevTime = data.ReadBit();
    bool hasVehicleId = data.ReadBit();

    if (hasPrevTime)
        data >> transportInfo.PrevMoveTime;

    if (hasVehicleId)
        data >> transportInfo.VehicleRecID;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, MovementInfo::TransportInfo const& transportInfo)
{
    bool hasPrevTime = transportInfo.PrevMoveTime != 0;
    bool hasVehicleId = transportInfo.VehicleRecID != 0;

    data << transportInfo.Guid;
    data << const_cast<MovementInfo::TransportInfo&>(transportInfo).Pos.PositionXYZOStream();
    data << transportInfo.VehicleSeatIndex;
    data << transportInfo.MoveTime;

    data.WriteBit(hasPrevTime);
    data.WriteBit(hasVehicleId);

    data.FlushBits();

    if (hasPrevTime)
        data << transportInfo.PrevMoveTime;

    if (hasVehicleId)
        data << transportInfo.VehicleRecID;

    data.FlushBits();

    return data;
}

void WorldPackets::Movement::ClientPlayerMovement::Read()
{
    _worldPacket >> movementInfo;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MonsterSplineFilterKey const& monsterSplineFilterKey)
{
    data << monsterSplineFilterKey.In;
    data << monsterSplineFilterKey.Out;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MonsterSplineFilter const& monsterSplineFilter)
{
    data << static_cast<uint32>(monsterSplineFilter.FilterKeys.size());
    data << monsterSplineFilter.BaseSpeed;
    data << monsterSplineFilter.StartOffset;
    data << monsterSplineFilter.DistToPrevFilterKey;
    data << monsterSplineFilter.AddedToStart;
    for (WorldPackets::Movement::MonsterSplineFilterKey const& filterKey : monsterSplineFilter.FilterKeys)
        data << filterKey;
    data.WriteBits(monsterSplineFilter.FilterFlags, 2);
    data.FlushBits();

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MovementForce const& movementForce)
{
    data << movementForce.ID;
    data << movementForce.Direction;
    data << movementForce.TransportPosition;
    data << movementForce.TransportID;
    data << movementForce.Magnitude;
    data.WriteBits(movementForce.Type, 2);
    data.FlushBits();

    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Movement::MovementForce& movementForce)
{
    data >> movementForce.ID;
    data >> movementForce.Direction;
    data >> movementForce.TransportPosition;
    data >> movementForce.TransportID;
    data >> movementForce.Magnitude;
    movementForce.Type = data.ReadBits(2);
    data.FlushBits();

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MovementSpline const& movementSpline)
{
    data << movementSpline.Flags;
    data << movementSpline.AnimTier;
    data << movementSpline.TierTransStartTime;
    data << movementSpline.Elapsed;
    data << movementSpline.MoveTime;
    data << movementSpline.JumpGravity;
    data << movementSpline.SpecialTime;
    data << static_cast<int32>(movementSpline.Points.size());
    data << movementSpline.Mode;
    data << movementSpline.VehicleExitVoluntary;
    data << movementSpline.TransportGUID;
    data << movementSpline.VehicleSeat;
    data << static_cast<int32>(movementSpline.PackedDeltas.size());
    for (G3D::Vector3 const& pos : movementSpline.Points)
        data << pos;
    for (G3D::Vector3 const& pos : movementSpline.PackedDeltas)
        data.appendPackXYZ(pos.x, pos.y, pos.z);
    data.WriteBits(movementSpline.Face, 2);
    data.WriteBit(movementSpline.SplineFilter.is_initialized());
    data.WriteBit(false); // unk
    data.FlushBits();

    switch (movementSpline.Face)
    {
        case MONSTER_MOVE_FACING_SPOT:
            data << movementSpline.FaceSpot;
            break;
        case MONSTER_MOVE_FACING_TARGET:
            data << movementSpline.FaceDirection;
            data << movementSpline.FaceGUID;
            break;
        case MONSTER_MOVE_FACING_ANGLE:
            data << movementSpline.FaceDirection;
            break;
    }

    if (movementSpline.SplineFilter)
        data << *movementSpline.SplineFilter;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Movement::MovementMonsterSpline const& movementMonsterSpline)
{
    data << movementMonsterSpline.ID;
    data << movementMonsterSpline.Destination;
    data << movementMonsterSpline.Move;
    data.WriteBit(movementMonsterSpline.CrzTeleport);

    // Unk bits. 0 if monster is moving, 1 or 2 if stopped
    if (movementMonsterSpline.Move.Flags)
        data.WriteBits(0, 2);
    else
        data.WriteBits(2, 2);

    data.FlushBits();

    return data;
}

void WorldPackets::Movement::CommonMovement::WriteCreateObjectSplineDataBlock(::Movement::MoveSpline const& moveSpline, ByteBuffer& data)
{
    data << uint32(moveSpline.GetId());

    if (!moveSpline.isCyclic())
        data << moveSpline.FinalDestination().xyz();
    else
        data << G3D::Vector3::zero();

    if (data.WriteBit(!moveSpline.Finalized()))
    {
        data.FlushBits();

        ::Movement::MoveSplineFlag const& splineFlags = moveSpline.splineflags;

        data << splineFlags.raw();
        data << int32(moveSpline.timePassed());
        data << uint32(moveSpline.Duration());
        data << float(moveSpline.durationModifier);
        data << float(moveSpline.nextDurationModifier);

        uint32 PointsCount = moveSpline.getPath().size();
        data << uint32(PointsCount);
        data.append<G3D::Vector3>(&moveSpline.getPath()[0], PointsCount);

        uint8 face = MONSTER_MOVE_NORMAL;
        if (splineFlags.final_angle)
            face = MONSTER_MOVE_FACING_ANGLE;
        else if (splineFlags.final_target)
            face = MONSTER_MOVE_FACING_TARGET;
        else if (splineFlags.final_point)
            face = MONSTER_MOVE_FACING_SPOT;

        data.WriteBits(face, 2);

        bool HasJumpGravity = data.WriteBit(splineFlags & (::Movement::MoveSplineFlag::Parabolic | ::Movement::MoveSplineFlag::Animation));
        bool HasSpecialTime = data.WriteBit((splineFlags & ::Movement::MoveSplineFlag::Parabolic) && moveSpline.effect_start_time < moveSpline.Duration());
        data.WriteBits(uint8(moveSpline.spline.mode()), 2);
        data.WriteBit(0); // HasSplineFilterKey
        data.WriteBit(0); // byte70
        switch (face)
        {
            case MONSTER_MOVE_FACING_SPOT:
                data << moveSpline.facing.f;
                break;
            case MONSTER_MOVE_FACING_TARGET:
                data << moveSpline.facing.target;
                break;
            case MONSTER_MOVE_FACING_ANGLE:
                data << moveSpline.facing.angle;
                break;
        }

        if (HasJumpGravity)
            data << float(moveSpline.vertical_acceleration);

        if (HasSpecialTime)
            data << uint32(moveSpline.effect_start_time);

        //if (HasSplineFilterKey)
        //{
        //    data << uint32(FilterKeysCount);
        //    for (var i = 0; i < PointsCount; ++i)
        //    {
        //        data << float(In);
        //        data << float(Out);
        //    }

        //    data.WriteBits(FilterFlags, 2);
        //}

        //if (byte70)
        //{
        //    packet.ReadPackedGuid128("unk spline guid", index);
        //    packet.ReadInt32("unk spline int32_1", index);
        //    packet.ReadInt32("unk spline int32_2", index);
        //}
    }
}

void WorldPackets::Movement::MonsterMove::InitializeSplineData(::Movement::MoveSpline const& moveSpline)
{
    SplineData.ID = moveSpline.m_Id;
    WorldPackets::Movement::MovementSpline& movementSpline = SplineData.Move;

    ::Movement::MoveSplineFlag splineFlags = moveSpline.splineflags;
    splineFlags.enter_cycle = moveSpline.isCyclic();
    movementSpline.Flags = uint32(splineFlags & uint32(~::Movement::MoveSplineFlag::Mask_No_Monster_Move));

    switch (splineFlags & ::Movement::MoveSplineFlag::Mask_Final_Facing)
    {
        case ::Movement::MoveSplineFlag::Final_Point:
            movementSpline.Face = MONSTER_MOVE_FACING_SPOT;
            movementSpline.FaceSpot = moveSpline.facing.f;
            break;
        case ::Movement::MoveSplineFlag::Final_Target:
            movementSpline.Face = MONSTER_MOVE_FACING_TARGET;
            movementSpline.FaceGUID = moveSpline.facing.target;
            break;
        case ::Movement::MoveSplineFlag::Final_Angle:
            movementSpline.Face = MONSTER_MOVE_FACING_ANGLE;
            movementSpline.FaceDirection = moveSpline.facing.angle;
            break;
        default:
            movementSpline.Face = MONSTER_MOVE_NORMAL;
            break;
    }

    if (splineFlags.animation)
    {
        movementSpline.AnimTier = splineFlags.getAnimationId();
        movementSpline.TierTransStartTime = moveSpline.effect_start_time;
    }

    movementSpline.MoveTime = moveSpline.Duration();

    if (splineFlags.parabolic)
    {
        movementSpline.JumpGravity = moveSpline.vertical_acceleration;
        movementSpline.SpecialTime = moveSpline.effect_start_time;
    }

    ::Movement::Spline<int32> const& spline = moveSpline.spline;
    std::vector<G3D::Vector3> const& array = spline.getPoints();

    if (splineFlags & ::Movement::MoveSplineFlag::UncompressedPath)
    {
        if (!splineFlags.cyclic)
        {
            uint32 count = spline.getPointCount() - 3;
            for (uint32 i = 0; i < count; ++i)
                movementSpline.Points.push_back(array[i + 2]);
        }
        else
        {
            uint32 count = spline.getPointCount() - 3;
            movementSpline.Points.push_back(array[1]);
            for (uint32 i = 0; i < count; ++i)
                movementSpline.Points.push_back(array[i + 1]);
        }
    }
    else
    {
        uint32 lastIdx = spline.getPointCount() - 3;
        G3D::Vector3 const* realPath = &spline.getPoint(1);

        movementSpline.Points.push_back(realPath[lastIdx]);

        if (lastIdx > 1)
        {
            G3D::Vector3 middle = (realPath[0] + realPath[lastIdx]) / 2.f;

            // first and last points already appended
            for (uint32 i = 1; i < lastIdx; ++i)
                movementSpline.PackedDeltas.push_back(middle - realPath[i]);
        }
    }

    movementSpline.Mode = spline.mode();
}

WorldPacket const* WorldPackets::Movement::MonsterMove::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << Pos;
    _worldPacket << SplineData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveSplineSetSpeed::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << Speed;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveSetSpeed::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << SequenceIndex;
    _worldPacket << Speed;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveUpdateSpeed::Write()
{
    _worldPacket << *movementInfo;
    _worldPacket << Speed;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveSplineSetFlag::Write()
{
    _worldPacket << MoverGUID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveSetFlag::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << SequenceIndex;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveUpdate::Write()
{
    _worldPacket << *movementInfo;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::TransferPending::Write()
{
    _worldPacket << MapID;
    _worldPacket.WriteBit(Ship.is_initialized());
    _worldPacket.WriteBit(TransferSpellID.is_initialized());
    _worldPacket.FlushBits();

    if (Ship)
    {
        _worldPacket << Ship->ID;
        _worldPacket << Ship->OriginMapID;
    }

    if (TransferSpellID)
        _worldPacket << *TransferSpellID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::TransferAborted::Write()
{
    _worldPacket << uint32(MapID);
    _worldPacket << uint8(Arg);
    _worldPacket.WriteBits(TransfertAbort, 5);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::NewWorld::Write()
{
    _worldPacket << int32(MapID);
    _worldPacket << Pos.PositionXYZOStream();
    _worldPacket << uint32(Reason);
    _worldPacket << float(0.0f);
    _worldPacket << float(0.0f);
    _worldPacket << float(0.0f);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveTeleport::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << SequenceIndex;
    _worldPacket << Pos.PositionXYZStream();
    _worldPacket << Facing;

    _worldPacket.WriteBit(TransportGUID.is_initialized());
    _worldPacket.WriteBit(Vehicle.is_initialized());
    _worldPacket.FlushBits();

    if (TransportGUID)
        _worldPacket << *TransportGUID;

    if (Vehicle)
    {
        _worldPacket << Vehicle->VehicleSeatIndex;
        _worldPacket.WriteBit(Vehicle->VehicleExitVoluntary);
        _worldPacket.WriteBit(Vehicle->VehicleExitTeleport);
        _worldPacket.FlushBits();
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveUpdateTeleport::Write()
{
    _worldPacket << *movementInfo;

    _worldPacket << static_cast<int32>(MovementForces.size());
    for (auto& force : MovementForces)
        _worldPacket << force;

    _worldPacket.WriteBit(WalkSpeed.is_initialized());
    _worldPacket.WriteBit(RunSpeed.is_initialized());
    _worldPacket.WriteBit(RunBackSpeed.is_initialized());
    _worldPacket.WriteBit(SwimSpeed.is_initialized());
    _worldPacket.WriteBit(SwimBackSpeed.is_initialized());
    _worldPacket.WriteBit(FlightSpeed.is_initialized());
    _worldPacket.WriteBit(FlightBackSpeed.is_initialized());
    _worldPacket.WriteBit(TurnRate.is_initialized());
    _worldPacket.WriteBit(PitchRate.is_initialized());

    _worldPacket.FlushBits();

    if (WalkSpeed)
        _worldPacket << *WalkSpeed;

    if (RunSpeed)
        _worldPacket << *RunSpeed;

    if (RunBackSpeed)
        _worldPacket << *RunBackSpeed;

    if (SwimSpeed)
        _worldPacket << *SwimSpeed;

    if (SwimBackSpeed)
        _worldPacket << *SwimBackSpeed;

    if (FlightSpeed)
        _worldPacket << *FlightSpeed;

    if (FlightBackSpeed)
        _worldPacket << *FlightBackSpeed;

    if (TurnRate)
        _worldPacket << *TurnRate;

    if (PitchRate)
        _worldPacket << *PitchRate;

    return &_worldPacket;
}

void WorldPackets::Movement::MoveTeleportAck::Read()
{
    _worldPacket >> MoverGUID;
    _worldPacket >> AckIndex;
    _worldPacket >> MoveTime;
}

ByteBuffer& operator>>(ByteBuffer& data, WorldPackets::Movement::MovementAck& ack)
{
    data >> ack.movementInfo;
    data >> ack.AckIndex;

    return data;
}

void WorldPackets::Movement::MovementAckMessage::Read()
{
    _worldPacket >> Ack;
}

void WorldPackets::Movement::MovementSpeedAck::Read()
{
    _worldPacket >> Ack;
    _worldPacket >> Speed;
}

void WorldPackets::Movement::MoveRemoveMovementForceAck::Read()
{
    _worldPacket >> Ack;
    _worldPacket >> TriggerGUID;
}

void WorldPackets::Movement::MoveApplyMovementForceAck::Read()
{
    _worldPacket >> Ack;
    _worldPacket >> MovementForceData;
}

void WorldPackets::Movement::SetActiveMover::Read()
{
    _worldPacket >> ActiveMover;
}

WorldPacket const* WorldPackets::Movement::MoveUpdateKnockBack::Write()
{
    _worldPacket << *movementInfo;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveSetCollisionHeight::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << uint32(SequenceIndex);
    _worldPacket << MsgData.Height;
    _worldPacket << MsgData.Scale;
    _worldPacket << uint32(MountDisplayID);
    _worldPacket.WriteBits(MsgData.Reason, 2);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveUpdateCollisionHeight::Write()
{
    _worldPacket << *movementInfo;
    _worldPacket << float(Height);
    _worldPacket << float(Scale);

    return &_worldPacket;
}

void WorldPackets::Movement::MoveSetCollisionHeightAck::Read()
{
    _worldPacket >> Data;
    _worldPacket >> MsgData.Height;
    _worldPacket >> MountDisplayID;
    MsgData.Reason = UpdateCollisionHeightReason(_worldPacket.ReadBits(2));
}

void WorldPackets::Movement::MoveTimeSkipped::Read()
{
    _worldPacket >> MoverGUID;
    _worldPacket >> TimeSkipped;
}

void WorldPackets::Movement::SummonResponse::Read()
{
    _worldPacket >> SummonerGUID;
    Accept = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Movement::ControlUpdate::Write()
{
    _worldPacket << Guid;
    _worldPacket.WriteBit(On);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Movement::MoveSplineDone::Read()
{
    _worldPacket >> movementInfo;
    _worldPacket >> SplineID;
}

WorldPacket const* WorldPackets::Movement::MoveUpdateRemoveMovementForce::Write()
{
    _worldPacket << *movementInfo;
    _worldPacket << TriggerGUID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveUpdateApplyMovementForce::Write()
{
    _worldPacket << *movementInfo;
    _worldPacket << MovementForceData;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveSetCompoundState::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << static_cast<int32>(Changes.size());
    for (auto& v : Changes)
    {
        _worldPacket << v.MessageID;
        _worldPacket << v.SequenceIndex;

        _worldPacket.WriteBit(v.Speed.is_initialized());
        _worldPacket.WriteBit(v.KnockBack.is_initialized());
        _worldPacket.WriteBit(v.VehicleRecID.is_initialized());
        _worldPacket.WriteBit(v.ColiisionHeight.is_initialized());
        _worldPacket.WriteBit(v.MovementForceData.is_initialized());
        _worldPacket.WriteBit(v.MoverGUID.is_initialized());

        if (v.Speed)
            _worldPacket << *v.Speed;

        if (v.KnockBack)
        {
            _worldPacket << v.KnockBack->HorzSpeed;
            _worldPacket << v.KnockBack->Direction.PositionXYStream();
            _worldPacket << v.KnockBack->InitVertSpeed;
        }

        if (v.VehicleRecID)
            _worldPacket << *v.VehicleRecID;

        if (v.ColiisionHeight)
        {
            _worldPacket << v.ColiisionHeight->Height;
            _worldPacket << v.ColiisionHeight->Scale;
            _worldPacket.WriteBits(v.ColiisionHeight->Reason, 2);
            _worldPacket.FlushBits();
        }

        if (v.MovementForceData)
            _worldPacket << *v.MovementForceData;

        if (v.MoverGUID)
            _worldPacket << *v.MoverGUID;

    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::FlightSplineSync::Write()
{
    _worldPacket << Guid;
    _worldPacket << SplineDist;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::AdjustSplineDuration::Write()
{
    _worldPacket << Unit;
    _worldPacket << Scale;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveKnockBack::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << SequenceIndex;
    _worldPacket << Direction;
    _worldPacket << HorzSpeed;
    _worldPacket << VertSpeed;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveApplyMovementForce::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << SequenceIndex;
    _worldPacket << Force;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Movement::MoveRemoveMovementForce::Write()
{
    _worldPacket << MoverGUID;
    _worldPacket << SequenceIndex;
    _worldPacket << TriggerGUID;

    return &_worldPacket;
}
