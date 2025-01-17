/*
 * Copyright (C) 2012-2016 Uwow <https://uwow.biz/>
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

#ifndef Trinity_game_Position_h__
#define Trinity_game_Position_h__

#include "Common.h"

#include <G3D/Vector3.h>

class ByteBuffer;

struct Position
{
    struct PositionXYStreamer
    {
        explicit PositionXYStreamer(Position& pos) : Pos(&pos) { }
        Position* Pos;
    };

    struct PositionXYZStreamer
    {
        explicit PositionXYZStreamer(Position& pos) : m_pos(&pos) { }
        Position* m_pos;
    };

    struct PositionXYZOStreamer
    {
        explicit PositionXYZOStreamer(Position& pos) : m_pos(&pos) { }
        Position* m_pos;
    };

    float m_positionX;
    float m_positionY;
    float m_positionZ;
// Better to limit access to m_orientation field, but this will be hard to achieve with many scripts using array initialization for this structure
//private:
    float m_orientation;
//public:

    void operator += (Position pos) {m_positionX += pos.m_positionX; m_positionY += pos.m_positionY; m_positionZ += pos.m_positionZ; m_orientation += pos.m_orientation; }
    
    void Clear() { m_positionX = 0.0f; m_positionY = 0.0f; m_positionZ = 0.0f; SetOrientation(0.0f); }
    
    void Relocate(float x, float y) { m_positionX = x; m_positionY = y;}
    void Relocate(float x, float y, float z) { m_positionX = x; m_positionY = y; m_positionZ = z; }
    void Relocate(float x, float y, float z, float orientation) {  m_positionX = x; m_positionY = y; m_positionZ = z; SetOrientation(orientation); }
    
    void Relocate(Position const& pos) {  m_positionX = pos.m_positionX; m_positionY = pos.m_positionY; m_positionZ = pos.m_positionZ; SetOrientation(pos.m_orientation); }
    void Relocate(Position const* pos) { m_positionX = pos->m_positionX; m_positionY = pos->m_positionY; m_positionZ = pos->m_positionZ; SetOrientation(pos->m_orientation); }

    void SetPosition(DBCPosition2D pos) { m_positionX = pos.X; m_positionY = pos.Y; }
    void SetPosition(DBCPosition3D pos) { m_positionX = pos.X; m_positionY = pos.Y; m_positionZ = pos.Z; }
    void SetPosition(DBCPosition4D pos) { m_positionX = pos.X; m_positionY = pos.Y; m_positionZ = pos.Z; SetOrientation(pos.O); }

    void RelocateOffset(Position const& offset);
    void SetOrientation(float orientation) { m_orientation = NormalizeOrientation(orientation); }

    float GetPositionX() const { return m_positionX; }
    float GetPositionY() const { return m_positionY; }
    float GetPositionZ() const { return m_positionZ; }
    float GetOrientation() const { return m_orientation; }

    Position GetPosition() const {  return *this; }
    void GetPosition(float &x, float &y) const { x = m_positionX; y = m_positionY; }
    void GetPosition(float &x, float &y, float &z) const { x = m_positionX; y = m_positionY; z = m_positionZ; }
    void GetPosition(float &x, float &y, float &z, float &o) const { x = m_positionX; y = m_positionY; z = m_positionZ; o = m_orientation; }
    void GetPosition(Position* pos) const { if (pos) pos->Relocate(m_positionX, m_positionY, m_positionZ, m_orientation); }
    void PositionToVector(G3D::Vector3& pos) const {  pos.x = m_positionX; pos.y = m_positionY; pos.z = m_positionZ; }
    Position VectorToPosition(G3D::Vector3 pos) {  return Position{pos.x, pos.y, pos.z}; }

    Position::PositionXYStreamer PositionXYStream() { return PositionXYStreamer(*this); }
    Position::PositionXYZStreamer PositionXYZStream() { return PositionXYZStreamer(*this); }
    Position::PositionXYZOStreamer PositionXYZOStream() { return PositionXYZOStreamer(*this); }

    bool IsPositionValid() const;

    float GetExactDist2dSq(float x, float y) const { float dx = m_positionX - x; float dy = m_positionY - y; return dx*dx + dy*dy; }
    float GetExactDist2d(const float x, const float y) const { return sqrt(GetExactDist2dSq(x, y)); }
    float GetExactDist2dSq(Position const* pos) const { float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; return dx*dx + dy*dy; }
    float GetExactDist2d(Position const* pos) const { return sqrt(GetExactDist2dSq(pos)); }
    float GetExactDistSq(float x, float y, float z) const { float dz = m_positionZ - z; return GetExactDist2dSq(x, y) + dz*dz; }
    float GetExactDist(float x, float y, float z) const {  return sqrt(GetExactDistSq(x, y, z)); }
    float GetExactDistSq(Position const* pos) const { float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; float dz = m_positionZ - pos->m_positionZ; return dx*dx + dy*dy + dz*dz; }
    float GetExactDist(Position const* pos) const { return sqrt(GetExactDistSq(pos)); }

    void GetPositionOffsetTo(Position const& endPos, Position & retOffset) const;

    float GetAngle(Position const* pos) const;
    float GetAngle(float x, float y) const;
    float GetRelativeAngle(Position const* pos) const { return GetAngle(pos) - m_orientation; }
    float GetRelativeAngle(float x, float y) const { return GetAngle(x, y) - m_orientation; }
    void GetSinCos(float x, float y, float &vsin, float &vcos) const;
    bool IsInDegreesRange(float x, float y, float degresA, float degresB, bool relative = false) const;
    float GetDegreesAngel(float x, float y, bool relative = false) const;

    Position GetRandPointBetween(Position const& B) const;
    void SimplePosXYRelocationByAngle(Position &pos, float dist, float angle, bool relative = false) const;
    void SimplePosXYRelocationByAngle(float &x, float &y, float &z, float dist, float angle, bool relative = false) const;

    bool IsInDist2d(float x, float y, float dist) const { return GetExactDist2dSq(x, y) < dist * dist; }
    bool IsInDist2d(Position const* pos, float dist) const { return GetExactDist2dSq(pos) < dist * dist; }
    bool IsInDist(float x, float y, float z, float dist) const { return GetExactDistSq(x, y, z) < dist * dist; }
    bool IsInDist(Position const* pos, float dist) const { return GetExactDistSq(pos) < dist * dist; }
    bool HasInArc(float arcangle, Position const* pos) const;
    bool HasInLine(WorldObject const* target, float width) const;
    std::string ToString() const;

    bool IsLinesCross(Position const &pos11, Position const &pos12, Position const &pos21, Position const &pos22) const;

    // modulos a radian orientation to the range of 0..2PI
    static float NormalizeOrientation(float o)
    {
        // fmod only supports positive numbers. Thus we have
        // to emulate negative numbers
        if (o < 0)
        {
            float mod = o *-1;
            mod = fmod(mod, 2.0f * static_cast<float>(M_PI));
            mod = -mod + 2.0f * static_cast<float>(M_PI);
            return mod;
        }
        return fmod(o, 2.0f * static_cast<float>(M_PI));
    }

    static float NormalizePitch(float o)
    {
        if (o > -M_PI && o < M_PI)
            return o;

        o = NormalizeOrientation(o + M_PI) - M_PI;
        return o;
    }
};

#define MAPID_INVALID 0xFFFFFFFF

class WorldLocation : public Position
{
public:
    explicit WorldLocation(uint32 _mapid = MAPID_INVALID, float _x = 0, float _y = 0, float _z = 0, float _o = 0) : m_mapId(_mapid) { Relocate(_x, _y, _z, _o); }
    explicit WorldLocation(uint32 _mapid, Position pos) : m_mapId(_mapid) { Relocate(pos); }

    WorldLocation(WorldLocation const& loc) { WorldRelocate(loc); }

    void WorldRelocate(WorldLocation const& loc)
    {
        m_mapId = loc.GetMapId();
        Relocate(loc);
    }

    uint32 GetMapId() const { return m_mapId; }

    void WorldRelocate(uint32 _mapId = MAPID_INVALID, float _x = 0.f, float _y = 0.f, float _z = 0.f, float _o = 0.f)
    {
        m_mapId = _mapId;
        Relocate(_x, _y, _z, _o);
    }

    WorldLocation GetWorldLocation() const { return *this; }

    uint32 m_mapId;
};

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);

#endif // Trinity_game_Position_h__
