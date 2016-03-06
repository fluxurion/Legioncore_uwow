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

#include "Position.h"
#include "ByteBuffer.h"
#include "GridDefines.h"

#include <G3D/g3dmath.h>

bool Position::HasInLine(WorldObject const* target, float width) const
{
    if (!HasInArc(M_PI, target))
        return false;
    width += target->GetObjectSize();
    float angle = GetRelativeAngle(target);
    return fabs(sin(angle)) * GetExactDist2d(target->GetPositionX(), target->GetPositionY()) < width;
}

bool Position::IsInDegreesRange(float x, float y, float degresA, float degresB, bool relative/* = false*/) const
{
    float angel = GetDegreesAngel(x, y, relative);
    return angel >= degresA && angel <= degresB;
}

float Position::GetDegreesAngel(float x, float y, bool relative) const
{
    float angel = relative ? GetRelativeAngle(x, y) : GetAngle(x, y);
    return NormalizeOrientation(angel) * M_RAD;
}

Position Position::GetRandPointBetween(Position const& B) const
{
    float Lambda = urand(0.0f, 100.0f) / 100.0f;
    float X = (B.GetPositionX() + Lambda * GetPositionX()) / (1 + Lambda);
    float Y = (B.GetPositionY() + Lambda * GetPositionY()) / (1 + Lambda);
    float Z = (B.GetPositionZ() + Lambda * GetPositionZ()) / (1 + Lambda); //Z should be updated by Vmap

    Position result;
    result.Relocate(X, Y, Z);
    return result;
}

void Position::SimplePosXYRelocationByAngle(Position &pos, float dist, float angle, bool relative) const
{
    if(!relative)
        angle += GetOrientation();

    pos.m_positionX = m_positionX + dist * std::cos(angle);
    pos.m_positionY = m_positionY + dist * std::sin(angle);
    pos.m_positionZ = m_positionZ;

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(pos.m_positionX, pos.m_positionY))
    {
        pos.Relocate(this);
        sLog->outFatal(LOG_FILTER_GENERAL, "Position::SimplePosXYRelocationByAngle invalid coordinates X: %f and Y: %f were passed!", pos.m_positionX, pos.m_positionY);
        return;
    }

    Trinity::NormalizeMapCoord(pos.m_positionX);
    Trinity::NormalizeMapCoord(pos.m_positionY);
    pos.SetOrientation(GetOrientation());
}

void Position::SimplePosXYRelocationByAngle(float &x, float &y, float &z, float dist, float angle, bool relative) const
{
    if(!relative)
        angle += GetOrientation();

    x = m_positionX + dist * std::cos(angle);
    y = m_positionY + dist * std::sin(angle);
    z = m_positionZ;

    // Prevent invalid coordinates here, position is unchanged
    if (!Trinity::IsValidMapCoord(x, y))
    {
        x = m_positionX;
        y = m_positionY;
        z = m_positionZ;
        sLog->outFatal(LOG_FILTER_GENERAL, "Position::SimplePosXYRelocationByAngle invalid coordinates X: %f and Y: %f were passed!", x, y);
        return;
    }

    Trinity::NormalizeMapCoord(x);
    Trinity::NormalizeMapCoord(y);
}

bool Position::IsLinesCross(Position const &pos11, Position const &pos12, Position const &pos21, Position const &pos22) const
{
    //Line 1
    G3D::Vector2 p11(pos11.GetPositionX(), pos11.GetPositionY());
    G3D::Vector2 p12(pos12.GetPositionX(), pos12.GetPositionY());
    LineSegment2D line1 = LineSegment2D::fromTwoPoints(p11, p12);
    //Line 2
    G3D::Vector2 p21(pos21.GetPositionX(), pos21.GetPositionY());
    G3D::Vector2 p22(pos22.GetPositionX(), pos22.GetPositionY());
    LineSegment2D line2 = LineSegment2D::fromTwoPoints(p21, p22);

    //check line
    if(line1.intersection(line2) == G3D::Vector2::inf())
        return false;
    else
        return true;
}

void Position::RelocateOffset(Position const& offset)
{
    m_positionX = GetPositionX() + (offset.GetPositionX() * std::cos(GetOrientation()) + offset.GetPositionY() * std::sin(GetOrientation() + M_PI));
    m_positionY = GetPositionY() + (offset.GetPositionY() * std::cos(GetOrientation()) + offset.GetPositionX() * std::sin(GetOrientation()));
    m_positionZ = GetPositionZ() + offset.GetPositionZ();
    SetOrientation(GetOrientation() + offset.GetOrientation());
}

void Position::GetPositionOffsetTo(Position const& endPos, Position & retOffset) const
{
    float dx = endPos.GetPositionX() - GetPositionX();
    float dy = endPos.GetPositionY() - GetPositionY();

    retOffset.m_positionX = dx * std::cos(GetOrientation()) + dy * std::sin(GetOrientation());
    retOffset.m_positionY = dy * std::cos(GetOrientation()) - dx * std::sin(GetOrientation());
    retOffset.m_positionZ = endPos.GetPositionZ() - GetPositionZ();
    retOffset.SetOrientation(endPos.GetOrientation() - GetOrientation());
}

float Position::GetAngle(Position const* obj) const
{
    if (!obj)
        return 0;

    return GetAngle(obj->GetPositionX(), obj->GetPositionY());
}

// Return angle in range 0..2*pi
float Position::GetAngle(const float x, const float y) const
{
    float dx = x - GetPositionX();
    float dy = y - GetPositionY();

    float ang = atan2(dy, dx);
    ang = (ang >= 0) ? ang : 2 * M_PI + ang;
    return ang;
}

void Position::GetSinCos(const float x, const float y, float &vsin, float &vcos) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;

    if (fabs(dx) < 0.001f && fabs(dy) < 0.001f)
    {
        float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
        vcos = std::cos(angle);
        vsin = std::sin(angle);
    }
    else
    {
        float dist = sqrt((dx*dx) + (dy*dy));
        vcos = dx / dist;
        vsin = dy / dist;
    }
}

bool Position::HasInArc(float arc, Position const* obj) const
{
    // always have self in arc
    if (obj == this)
        return true;

    // move arc to range 0.. 2*pi
    arc = NormalizeOrientation(arc);

    float angle = GetAngle(obj);
    angle -= m_orientation;

    // move angle to range -pi ... +pi
    angle = NormalizeOrientation(angle);
    if (angle > M_PI)
        angle -= 2.0f*M_PI;

    float lborder = -1 * (arc/2.0f);                        // in range -pi..0
    float rborder = (arc/2.0f);                             // in range 0..pi
    return ((angle >= lborder) && (angle <= rborder));
}

bool Position::IsPositionValid() const
{
    return Trinity::IsValidMapCoord(m_positionX, m_positionY, m_positionZ, m_orientation);
}

std::string Position::ToString() const
{
    std::stringstream sstr;
    sstr << "X: " << m_positionX << " Y: " << m_positionY << " Z: " << m_positionZ << " O: " << m_orientation;
    return sstr.str();
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYStreamer const& streamer)
{
    buf << streamer.Pos->GetPositionX();
    buf << streamer.Pos->GetPositionY();

    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYStreamer const& streamer)
{
    float x, y;
    buf >> x >> y;
    streamer.Pos->Relocate(x, y);

    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    buf << streamer.m_pos->GetPositionX();
    buf << streamer.m_pos->GetPositionY();
    buf << streamer.m_pos->GetPositionZ();

    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    buf >> x >> y >> z;
    streamer.m_pos->Relocate(x, y, z);

    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    buf << streamer.m_pos->GetPositionX();
    buf << streamer.m_pos->GetPositionY();
    buf << streamer.m_pos->GetPositionZ();
    buf << streamer.m_pos->GetOrientation();

    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    float x, y, z, o;
    buf >> x >> y >> z >> o;
    streamer.m_pos->Relocate(x, y, z, o);

    return buf;
}
