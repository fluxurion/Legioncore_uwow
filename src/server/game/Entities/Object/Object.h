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

#ifndef _OBJECT_H
#define _OBJECT_H

#include "Common.h"
#include "UpdateMask.h"
#include "UpdateFields.h"
#include "UpdateData.h"
#include "GridReference.h"
#include "ObjectDefines.h"
#include "GridDefines.h"
#include "Map.h"

#include <boost/unordered_set.hpp>
#include "G3D/Vector3.h"
#include "G3D/LineSegment.h"

using G3D::Vector3;
using G3D::LineSegment2D;

#define CONTACT_DISTANCE            0.5f
#define INTERACTION_DISTANCE        5.0f
#define ATTACK_DISTANCE             5.0f
#define LOOT_DISTANCE               25.0f
#define MAX_VISIBILITY_DISTANCE     SIZE_OF_GRIDS           // max distance for visible objects
#define SIGHT_RANGE_UNIT            50.0f
#define DEFAULT_VISIBILITY_DISTANCE 90.0f                   // default visible distance, 90 yards on continents
#define DEFAULT_VISIBILITY_INSTANCE 170.0f                  // default visible distance in instances, 170 yards
#define DEFAULT_VISIBILITY_BGARENAS 533.0f                  // default visible distance in BG/Arenas, roughly 533 yards

#define DEFAULT_WORLD_OBJECT_SIZE   0.306f                  // player size, also currently used (correctly?) for any non Unit world objects
#define DEFAULT_COMBAT_REACH        1.5f
#define MIN_MELEE_REACH             2.0f
#define NOMINAL_MELEE_RANGE         5.0f
#define MELEE_RANGE                 (NOMINAL_MELEE_RANGE - MIN_MELEE_REACH * 2) //center to center for players
#define MAGIC_RANGE                 30.0f

enum TempSummonType
{
    TEMPSUMMON_TIMED_OR_DEAD_DESPAWN       = 1,             // despawns after a specified time OR when the creature disappears
    TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN     = 2,             // despawns after a specified time OR when the creature dies
    TEMPSUMMON_TIMED_DESPAWN               = 3,             // despawns after a specified time
    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT = 4,             // despawns after a specified time after the creature is out of combat
    TEMPSUMMON_CORPSE_DESPAWN              = 5,             // despawns instantly after death
    TEMPSUMMON_CORPSE_TIMED_DESPAWN        = 6,             // despawns after a specified time after death
    TEMPSUMMON_DEAD_DESPAWN                = 7,             // despawns when the creature disappears
    TEMPSUMMON_MANUAL_DESPAWN              = 8              // despawns when UnSummon() is called
};

enum SummonerType
{
    SUMMONER_TYPE_CREATURE      = 0,
    SUMMONER_TYPE_GAMEOBJECT    = 1,
    SUMMONER_TYPE_MAP           = 2
};

enum PhaseMasks
{
    PHASEMASK_NORMAL   = 0x00000001,
    PHASEMASK_ANYWHERE = 0xFFFFFFFF
};

enum NotifyFlags
{
    NOTIFY_NONE                     = 0x00,
    NOTIFY_AI_RELOCATION            = 0x01,
    NOTIFY_VISIBILITY_CHANGED       = 0x02,
    NOTIFY_ALL                      = 0xFF
};

class WorldPacket;
class UpdateData;
class ByteBuffer;
class WorldSession;
class Creature;
class Player;
class InstanceScript;
class Item;
class GameObject;
class TempSummon;
class Vehicle;
class CreatureAI;
class ZoneScript;
class Unit;
class Transport;

/// Key for storing temp summon data in TempSummonDataContainer
struct TempSummonGroupKey
{
    TempSummonGroupKey(uint32 summonerEntry, SummonerType summonerType, uint8 group)
        : _summonerEntry(summonerEntry), _summonerType(summonerType), _summonGroup(group)
    {
    }

    bool operator<(TempSummonGroupKey const& rhs) const
    {
        return std::tie(_summonerEntry, _summonerType, _summonGroup) <
            std::tie(rhs._summonerEntry, rhs._summonerType, rhs._summonGroup);
    }

private:
    uint32 _summonerEntry;      ///< Summoner's entry
    SummonerType _summonerType; ///< Summoner's type, see SummonerType for available types
    uint8 _summonGroup;         ///< Summon's group id
};

typedef std::unordered_map<Player*, UpdateData> UpdateDataMapType;
typedef std::map<TempSummonGroupKey, GuidList> TempSummonGroupMap;
typedef std::unordered_map<uint32, GuidList> TempSummonMap;

typedef cyber_ptr<Object> C_PTR;
class Object
{
    public:
        virtual ~Object();

        bool IsInWorld() const { return m_inWorld; }

        virtual void AddToWorld();
        virtual void RemoveFromWorld();

        ObjectGuid const& GetGUID() const { return GetGuidValue(OBJECT_FIELD_GUID); }
        uint32 GetGUIDLow() const { return GetGUID().GetGUIDLow(); }
        uint32 GetGUIDHigh() const { return GetGUID().GetGUIDHigh(); }
        PackedGuid const& GetPackGUID() const { return m_PackGUID; }
        uint32 GetEntry() const { return GetUInt32Value(OBJECT_FIELD_ENTRY_ID); }
        void SetEntry(uint32 entry) { SetUInt32Value(OBJECT_FIELD_ENTRY_ID, entry); }
        ObjectGuid const& GetVignetteGUID() const { return vignetteGuid; }

        float GetObjectScale() const { return GetFloatValue(OBJECT_FIELD_SCALE); }
        virtual void SetObjectScale(float scale) { SetFloatValue(OBJECT_FIELD_SCALE, scale); }

        TypeID GetTypeId() const { return m_objectTypeId; }
        bool isType(uint16 mask) const { return (mask & m_objectType) != 0; }

        virtual void BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const;
        void SendUpdateToPlayer(Player* player);

        void BuildValuesUpdateBlockForPlayer(UpdateData* data, Player* target) const;
        void BuildOutOfRangeUpdateBlock(UpdateData* data) const;

        virtual void DestroyForPlayer(Player* target, bool onDeath = false) const;

        int32 GetInt32Value(uint16 index) const
        {
            //ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            if(index < m_valuesCount || PrintIndexError(index , false))
                return m_int32Values[ index ];
            else
                return int32(0);
        }

        uint32 GetUInt32Value(uint16 index) const
        {
            //ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            uint16 tempindex = index;
            if(tempindex < m_valuesCount)
                return m_uint32Values[ tempindex ];
            if(PrintIndexError(tempindex , false))
                return m_uint32Values[ tempindex ];
            else
                return uint32(0);
        }

        uint64 GetUInt64Value(uint16 index) const
        {
            //ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, false));
            if(index + 1 < m_valuesCount || PrintIndexError(index , false))
                return *((uint64*)&(m_uint32Values[ index ]));
            else
                return uint64(0);
        }

        float GetFloatValue(uint16 index) const
        {
            //ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            if(index < m_valuesCount || PrintIndexError(index , false))
                return m_floatValues[ index ];
            else
                return float(0);
        }

        uint8 GetByteValue(uint16 index, uint8 offset) const
        {
            //ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            //ASSERT(offset < 4);
            if((index < m_valuesCount || PrintIndexError(index , false)) && (offset < 4))
                return *(((uint8*)&m_uint32Values[ index ])+offset);
            else
                return uint8(0);
        }

        uint16 GetUInt16Value(uint16 index, uint8 offset) const
        {
            //ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            //ASSERT(offset < 2);
            if((index < m_valuesCount || PrintIndexError(index , false)) && (offset < 2))
                return *(((uint16*)&m_uint32Values[ index ])+offset);
            else
                return uint16(0);
        }

        ObjectGuid const& GetGuidValue(uint16 index) const;

        void SetInt32Value(uint16 index, int32 value);
        void UpdateInt32Value(uint16 index, int32 value);
        void SetUInt32Value(uint16 index, uint32 value);
        void UpdateUInt32Value(uint16 index, uint32 value);
        void SetUInt64Value(uint16 index, uint64 value);
        void SetFloatValue(uint16 index, float value);
        void SetByteValue(uint16 index, uint8 offset, uint8 value);
        void SetUInt16Value(uint16 index, uint8 offset, uint16 value);
        void SetInt16Value(uint16 index, uint8 offset, int16 value) { SetUInt16Value(index, offset, (uint16)value); }
        void SetGuidValue(uint16 index, ObjectGuid const& value);
        void SetStatFloatValue(uint16 index, float value);
        void SetStatInt32Value(uint16 index, int32 value);

        bool AddGuidValue(uint16 index, ObjectGuid const& value);
        bool RemoveGuidValue(uint16 index, ObjectGuid const& value);

        void ApplyModUInt32Value(uint16 index, int32 val, bool apply);
        void ApplyModInt32Value(uint16 index, int32 val, bool apply);
        void ApplyModUInt64Value(uint16 index, int32 val, bool apply);
        void ApplyModPositiveFloatValue(uint16 index, float val, bool apply);
        void ApplyModSignedFloatValue(uint16 index, float val, bool apply);

        void ApplyPercentModFloatValue(uint16 index, float val, bool apply)
        {
            float value = GetFloatValue(index);
            ApplyPercentModFloatVar(value, val, apply);
            SetFloatValue(index, value);
        }

        void SetFlag(uint16 index, uint32 newFlag);
        void RemoveFlag(uint16 index, uint32 oldFlag);

        void ToggleFlag(uint16 index, uint32 flag)
        {
            if (HasFlag(index, flag))
                RemoveFlag(index, flag);
            else
                SetFlag(index, flag);
        }

        bool HasFlag(uint16 index, uint32 flag) const
        {
            if (index >= m_valuesCount && !PrintIndexError(index, false))
                return false;

            return (m_uint32Values[index] & flag) != 0;
        }

        void SetByteFlag(uint16 index, uint8 offset, uint8 newFlag);
        void RemoveByteFlag(uint16 index, uint8 offset, uint8 newFlag);

        void ToggleFlag(uint16 index, uint8 offset, uint8 flag)
        {
            if (HasByteFlag(index, offset, flag))
                RemoveByteFlag(index, offset, flag);
            else
                SetByteFlag(index, offset, flag);
        }

        bool HasByteFlag(uint16 index, uint8 offset, uint8 flag) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            ASSERT(offset < 4);
            return (((uint8*)&m_uint32Values[index])[offset] & flag) != 0;
        }

        void ApplyModFlag(uint16 index, uint32 flag, bool apply)
        {
            if (apply) SetFlag(index, flag); else RemoveFlag(index, flag);
        }

        void SetFlag64(uint16 index, uint64 newFlag)
        {
            uint64 oldval = GetUInt64Value(index);
            uint64 newval = oldval | newFlag;
            SetUInt64Value(index, newval);
        }

        void RemoveFlag64(uint16 index, uint64 oldFlag)
        {
            uint64 oldval = GetUInt64Value(index);
            uint64 newval = oldval & ~oldFlag;
            SetUInt64Value(index, newval);
        }

        void ToggleFlag64(uint16 index, uint64 flag)
        {
            if (HasFlag64(index, flag))
                RemoveFlag64(index, flag);
            else
                SetFlag64(index, flag);
        }

        bool HasFlag64(uint16 index, uint64 flag) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            return (GetUInt64Value(index) & flag) != 0;
        }

        void ApplyModFlag64(uint16 index, uint64 flag, bool apply)
        {
            if (apply) SetFlag64(index, flag); else RemoveFlag64(index, flag);
        }

        void ClearUpdateMask(bool remove);

        uint16 GetValuesCount() const { return m_valuesCount; }
        uint16 GetDynamicValuesCount() const { return _dynamicValuesCount; }

        std::vector<uint32> const& GetDynamicValues(uint16 tab) const;
        void AddDynamicValue(uint16 tab, uint32 value);
        void SetDynamicUInt32Value(uint32 tab, uint16 index, uint32 value);
        void ClearDynamicValue(uint16 tab);
        uint32 GetDynamicUInt32Value(uint32 index, uint16 offset) const
        {
            ASSERT(index < _dynamicValuesCount);
            return _dynamicValues[index][offset];
        }

        virtual bool hasQuest(uint32 /* quest_id */) const { return false; }
        virtual bool hasInvolvedQuest(uint32 /* quest_id */) const { return false; }
        virtual void BuildUpdate(UpdateDataMapType&) {}
        virtual void BuildDynamicValuesUpdate(uint8 updatetype, ByteBuffer *data, Player* target) const;

        virtual uint32 GetVignetteId() const { return 0; }

        void BuildFieldsUpdate(Player*, UpdateDataMapType &) const;

        void SetFieldNotifyFlag(uint16 flag) { _fieldNotifyFlags |= flag; }
        void RemoveFieldNotifyFlag(uint16 flag) { _fieldNotifyFlags &= ~flag; }

        // FG: some hacky helpers
        void ForceValuesUpdateAtIndex(uint32);

        Player* ToPlayer() { if (GetTypeId() == TYPEID_PLAYER) return reinterpret_cast<Player*>(this); else return NULL; }
        Player const* ToPlayer() const { if (GetTypeId() == TYPEID_PLAYER) return reinterpret_cast<Player const*>(this); else return NULL; }

        Creature* ToCreature() { if (GetTypeId() == TYPEID_UNIT) return reinterpret_cast<Creature*>(this); else return NULL; }
        Creature const* ToCreature() const { if (GetTypeId() == TYPEID_UNIT) return reinterpret_cast<Creature const*>(this); else return NULL; }

        Unit* ToUnit() { if (isType(TYPEMASK_UNIT)) return reinterpret_cast<Unit*>(this); else return NULL; }
        Unit const* ToUnit() const { if (isType(TYPEMASK_UNIT)) return reinterpret_cast<Unit const*>(this); else return NULL; }

        GameObject* ToGameObject() { if (GetTypeId() == TYPEID_GAMEOBJECT) return reinterpret_cast<GameObject*>(this); else return NULL; }
        GameObject const* ToGameObject() const { if (GetTypeId() == TYPEID_GAMEOBJECT) return reinterpret_cast<GameObject const*>(this); else return NULL; }

        Corpse* ToCorpse() { if (GetTypeId() == TYPEID_CORPSE) return reinterpret_cast<Corpse*>(this); else return NULL; }
        Corpse const* ToCorpse() const { if (GetTypeId() == TYPEID_CORPSE) return reinterpret_cast<Corpse const*>(this); else return NULL; }

        Item* ToItem() { if (GetTypeId() == TYPEID_ITEM) return reinterpret_cast<Item*>(this); else return NULL; }
        Item const* ToItem() const { if (GetTypeId() == TYPEID_ITEM) return reinterpret_cast<Item const*>(this); else return NULL; }

        DynamicObject* ToDynObject() { if (GetTypeId() == TYPEID_DYNAMICOBJECT) return reinterpret_cast<DynamicObject*>(this); else return NULL; }
        DynamicObject const* ToDynObject() const { if (GetTypeId() == TYPEID_DYNAMICOBJECT) return reinterpret_cast<DynamicObject const*>(this); else return NULL; }

        AreaTrigger* ToAreaTrigger() { if (GetTypeId() == TYPEID_AREATRIGGER) return reinterpret_cast<AreaTrigger*>(this); else return NULL; }
        AreaTrigger const* ToAreaTrigger() const { if (GetTypeId() == TYPEID_AREATRIGGER) return reinterpret_cast<AreaTrigger const*>(this); else return NULL; }

        WorldObject* ToWorldObject() { return reinterpret_cast<WorldObject*>(this); }
        WorldObject const* ToWorldObject() const { return reinterpret_cast<WorldObject const*>(this); }
        //!  Get or Init cyber ptr.
        C_PTR get_ptr();

        TempSummonGroupMap tempSummonGroupList;
        TempSummonMap tempSummonList;
    protected:
        Object();

        void _InitValues();
        void _Create(ObjectGuid const& guid);
        std::string _ConcatFields(uint16 startIndex, uint16 size) const;
        void _LoadIntoDataField(const char* data, uint32 startOffset, uint32 count);

        uint32 GetUpdateFieldData(Player const* target, uint32*& flags) const;
        uint32 GetDynamicUpdateFieldData(Player const* target, uint32*& flags) const;

        void _BuildMovementUpdate(ByteBuffer * data, uint16 flags) const;
        void _BuildValuesUpdate(uint8 updatetype, ByteBuffer *data, Player* target) const;

        uint16 m_objectType;

        TypeID m_objectTypeId;
        uint16 m_updateFlag;

        union
        {
            int32  *m_int32Values;
            uint32 *m_uint32Values;
            float  *m_floatValues;
        };

        bool* _changedFields;

        uint16 m_valuesCount;

        uint16 _fieldNotifyFlags;

        bool m_objectUpdated;

        std::vector<uint32>* _dynamicValues;
        UpdateMask _dynamicChangesMask;
        UpdateMask* _dynamicChangesArrayMask;
        uint16 _dynamicValuesCount;

        ObjectGuid vignetteGuid;
    private:
        C_PTR ptr;
        bool m_inWorld;

        PackedGuid m_PackGUID;

        // for output helpfull error messages from asserts
        bool PrintIndexError(uint32 index, bool set) const;
        Object(const Object&);                              // prevent generation copy constructor
        Object& operator=(Object const&);                   // prevent generation assigment operator
};

struct Position
{
    struct PositionXYStreamer
    {
        explicit PositionXYStreamer(Position& pos) : Pos(&pos) { }
        Position* Pos;
    };

    struct PositionXYZStreamer
    {
        explicit PositionXYZStreamer(Position& pos) : m_pos(&pos) {}
        Position* m_pos;
    };

    struct PositionXYZOStreamer
    {
        explicit PositionXYZOStreamer(Position& pos) : m_pos(&pos) {}
        Position* m_pos;
    };

    float m_positionX;
    float m_positionY;
    float m_positionZ;
// Better to limit access to m_orientation field, but this will be hard to achieve with many scripts using array initialization for this structure
//private:
    float m_orientation;
//public:

    void Relocate(float x, float y) { m_positionX = x; m_positionY = y;}
    void Relocate(float x, float y, float z) { m_positionX = x; m_positionY = y; m_positionZ = z; }
    void Relocate(float x, float y, float z, float orientation) { m_positionX = x; m_positionY = y; m_positionZ = z; SetOrientation(orientation); }
    
    void SetPosition(DBCPosition2D pos) { m_positionX = pos.X; m_positionY = pos.Y;}
    void SetPosition(DBCPosition3D pos) { m_positionX = pos.X; m_positionY = pos.Y; m_positionZ = pos.Z; }
    void SetPosition(DBCPosition4D pos) { m_positionX = pos.X; m_positionY = pos.Y; m_positionZ = pos.Z; SetOrientation(pos.O); }

    void Relocate(Position const& pos) { m_positionX = pos.m_positionX; m_positionY = pos.m_positionY; m_positionZ = pos.m_positionZ; SetOrientation(pos.m_orientation); }
    void Relocate(Position const* pos) { m_positionX = pos->m_positionX; m_positionY = pos->m_positionY; m_positionZ = pos->m_positionZ; SetOrientation(pos->m_orientation); }


    void RelocateOffset(const Position &offset);
    void SetOrientation(float orientation)
    { m_orientation = NormalizeOrientation(orientation); }

    float GetPositionX() const { return m_positionX; }
    float GetPositionY() const { return m_positionY; }
    float GetPositionZ() const { return m_positionZ; }
    float GetOrientation() const { return m_orientation; }

    void GetPosition(float &x, float &y) const
        { x = m_positionX; y = m_positionY; }
    void GetPosition(float &x, float &y, float &z) const
        { x = m_positionX; y = m_positionY; z = m_positionZ; }
    void GetPosition(float &x, float &y, float &z, float &o) const
        { x = m_positionX; y = m_positionY; z = m_positionZ; o = m_orientation; }
    void GetPosition(Position* pos) const
    {
        if (pos)
            pos->Relocate(m_positionX, m_positionY, m_positionZ, m_orientation);
    }
    void PositionToVector(G3D::Vector3& pos) const
        { pos.x = m_positionX; pos.y = m_positionY; pos.z = m_positionZ; }
    void VectorToPosition(G3D::Vector3 pos)
        { m_positionX = pos.x; m_positionY = pos.y; m_positionZ = pos.z; }

    Position::PositionXYStreamer PositionXYStream() { return PositionXYStreamer(*this); }
    Position::PositionXYZStreamer PositionXYZStream() { return PositionXYZStreamer(*this); }
    Position::PositionXYZOStreamer PositionXYZOStream() { return PositionXYZOStreamer(*this); }

    bool IsPositionValid() const;

    float GetExactDist2dSq(float x, float y) const
        { float dx = m_positionX - x; float dy = m_positionY - y; return dx*dx + dy*dy; }
    float GetExactDist2d(const float x, const float y) const
        { return sqrt(GetExactDist2dSq(x, y)); }
    float GetExactDist2dSq(const Position* pos) const
        { float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; return dx*dx + dy*dy; }
    float GetExactDist2d(const Position* pos) const
        { return sqrt(GetExactDist2dSq(pos)); }
    float GetExactDistSq(float x, float y, float z) const
        { float dz = m_positionZ - z; return GetExactDist2dSq(x, y) + dz*dz; }
    float GetExactDist(float x, float y, float z) const
        { return sqrt(GetExactDistSq(x, y, z)); }
    float GetExactDistSq(const Position* pos) const
        { float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; float dz = m_positionZ - pos->m_positionZ; return dx*dx + dy*dy + dz*dz; }
    float GetExactDist(const Position* pos) const
        { return sqrt(GetExactDistSq(pos)); }

    void GetPositionOffsetTo(const Position & endPos, Position & retOffset) const;

    Position GetPosition() const
    {
        return *this;
    }

    float GetAngle(const Position* pos) const;
    float GetAngle(float x, float y) const;
    float GetRelativeAngle(const Position* pos) const
        { return GetAngle(pos) - m_orientation; }
    float GetRelativeAngle(float x, float y) const { return GetAngle(x, y) - m_orientation; }
    void GetSinCos(float x, float y, float &vsin, float &vcos) const;
    bool IsInDegreesRange(float x, float y, float degresA, float degresB, bool relative = false) const;
    float GetDegreesAngel(float x, float y, bool relative = false) const;

    Position GetRandPointBetween(const Position &B) const;
    void SimplePosXYRelocationByAngle(Position &pos, float dist, float angle, bool relative = false) const;
    void SimplePosXYRelocationByAngle(float &x, float &y, float &z, float dist, float angle, bool relative = false) const;

    bool IsInDist2d(float x, float y, float dist) const
        { return GetExactDist2dSq(x, y) < dist * dist; }
    bool IsInDist2d(const Position* pos, float dist) const
        { return GetExactDist2dSq(pos) < dist * dist; }
    bool IsInDist(float x, float y, float z, float dist) const
        { return GetExactDistSq(x, y, z) < dist * dist; }
    bool IsInDist(const Position* pos, float dist) const
        { return GetExactDistSq(pos) < dist * dist; }
    bool HasInArc(float arcangle, const Position* pos) const;
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

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);

struct MovementInfo
{
    // common
    ObjectGuid guid;
    uint32 flags;
    uint16 flags2;
    Position pos;
    uint32 time;

    // transport
    struct TransportInfo
    {
        void Reset()
        {
            guid.Clear();
            pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
            seat = -1;
            time = 0;
            prevTime = 0;
            vehicleId = 0;
            hasTrevTime = false;
            hasVehicleId = false;
        }

        ObjectGuid guid;
        Position pos;
        int8 seat;
        uint32 time;
        bool hasTrevTime;
        uint32 prevTime;
        bool hasVehicleId;
        uint32 vehicleId;
    } transport;

    // swimming/flying
    float pitch;
    bool hasPitch;

    // falling
    uint32 fallTime;

    // jumping
    struct JumpInfo
    {
        void Reset()
        {
            zspeed = sinAngle = cosAngle = xyspeed = 0.0f;
        }

        float zspeed, sinAngle, cosAngle, xyspeed;

    } jump;

    // spline
    bool hasSplineElevation;
    float splineElevation;
    // BitClientData
    bool hasFallData;
    bool hasFallDirection;
    bool hasSpline;
    bool byte95;
    bool byteAC;
    bool hasUnkInt32;
    uint32 unkInt32;
    std::vector<uint32> unkCounter;

    MovementInfo()
    {
        pos.Relocate(0, 0, 0, 0);
        transport.Reset();
        jump.Reset();
        guid.Clear();
        flags = 0;
        flags2 = 0;
        time = fallTime = 0;
        splineElevation = 0;
        hasSplineElevation = false;
        hasPitch = false;
        pitch = 0.0f;
        hasFallData = false;
        hasFallDirection = false;
        hasSpline = false;
        byte95 = false;
        byteAC = false;
        hasUnkInt32 = false;
        unkInt32 = 0;
    }

    uint32 GetMovementFlags() const { return flags; }
    void SetMovementFlags(uint32 flag) { flags = flag; }
    void AddMovementFlag(uint32 flag) { flags |= flag; }
    void RemoveMovementFlag(uint32 flag) { flags &= ~flag; }
    bool HasMovementFlag(uint32 flag) const { return (flags & flag) != 0; }

    uint16 GetExtraMovementFlags() const { return flags2; }
    void SetExtraMovementFlags(uint32 flag) { flags2 = flag; }
    void AddExtraMovementFlag(uint16 flag) { flags2 |= flag; }
    bool HasExtraMovementFlag(uint16 flag) const { return (flags2 & flag) != 0; }

    void SetFallTime(uint32 time) { fallTime = time; }

    void ResetTransport()
    {
        transport.Reset();
    }

    void ResetJump()
    {
        jump.Reset();
    }

    void OutDebug();
};

struct DigSiteInfo
{
    float x, y, z;
    float last_cast_dist;
    uint32 loot_GO_entry;
    uint32 race;
    uint32 pointCount[871];
    uint32 countProject;
    uint32 currentDigest;

    DigSiteInfo()
    {
        x = y = z = 0.0f;
        last_cast_dist = 0;
        loot_GO_entry = 0;
        race = 0;
        countProject = 0;
        currentDigest = 0;
        pointCount[0] = 0;
        pointCount[1] = 0;
        pointCount[530] = 0;
        pointCount[571] = 0;
        pointCount[870] = 0;
    }
};

#define MAPID_INVALID 0xFFFFFFFF

class WorldLocation : public Position
{
public:
    explicit WorldLocation(uint32 _mapid = MAPID_INVALID, float _x = 0, float _y = 0, float _z = 0, float _o = 0)
        : m_mapId(_mapid)
    {
        Relocate(_x, _y, _z, _o);
    }

    WorldLocation(WorldLocation const& loc)
    {
        WorldRelocate(loc);
    }

    void WorldRelocate(WorldLocation const& loc)
    {
        m_mapId = loc.GetMapId();
        Relocate(loc);
    }

    uint32 GetMapId() const
    {
        return m_mapId;
    }

    void WorldRelocate(uint32 _mapId = MAPID_INVALID, float _x = 0.f, float _y = 0.f, float _z = 0.f, float _o = 0.f)
    {
        m_mapId = _mapId;
        Relocate(_x, _y, _z, _o);
    }

    WorldLocation GetWorldLocation() const
    {
        return *this;
    }

    uint32 m_mapId;
};

template<class T>
class GridObject
{
    public:
        bool IsInGrid() const { return _gridRef.isValid(); }
        void AddToGrid(GridRefManager<T>& m) { ASSERT(!IsInGrid()); _gridRef.link(&m, (T*)this); }
        void RemoveFromGrid() { ASSERT(IsInGrid()); _gridRef.unlink(); }
    private:
        GridReference<T> _gridRef;
};

template <class T_VALUES, class T_FLAGS, class FLAG_TYPE, uint8 ARRAY_SIZE>
class FlaggedValuesArray32
{
    public:
        FlaggedValuesArray32()
        {
            memset(&m_values, 0x00, sizeof(T_VALUES) * ARRAY_SIZE);
            m_flags = 0;
        }

        T_FLAGS  GetFlags() const { return m_flags; }
        bool     HasFlag(FLAG_TYPE flag) const { return m_flags & (1 << flag); }
        void     AddFlag(FLAG_TYPE flag) { m_flags |= (1 << flag); }
        void     DelFlag(FLAG_TYPE flag) { m_flags &= ~(1 << flag); }

        T_VALUES GetValue(FLAG_TYPE flag) const { return m_values[flag]; }
        void     SetValue(FLAG_TYPE flag, T_VALUES value) { m_values[flag] = value; }
        void     AddValue(FLAG_TYPE flag, T_VALUES value) { m_values[flag] += value; }

    private:
        T_VALUES m_values[ARRAY_SIZE];
        T_FLAGS m_flags;
};

enum MapObjectCellMoveState
{
    MAP_OBJECT_CELL_MOVE_NONE, //not in move list
    MAP_OBJECT_CELL_MOVE_ACTIVE, //in move list
    MAP_OBJECT_CELL_MOVE_INACTIVE, //in move list but should not move
};

class MapObject
{
    friend class Map; //map for moving creatures
    friend class ObjectGridLoader; //grid loader for loading creatures

    protected:
    MapObject() : _moveState(MAP_OBJECT_CELL_MOVE_NONE), _newPosition() { }

    private:
    Cell _currentCell;
    Cell const& GetCurrentCell() const { return _currentCell;  }

    void SetCurrentCell(Cell const& cell)
    {
        _currentCell = cell;
    }

    MapObjectCellMoveState _moveState;
    Position _newPosition;
    void SetNewCellPosition(float x, float y, float z, float o)
    {
        _moveState = MAP_OBJECT_CELL_MOVE_ACTIVE;
        _newPosition.Relocate(x, y, z, o);
    }
};

class WorldObject : public Object, public WorldLocation
{
    protected:
        explicit WorldObject(bool isWorldObject); //note: here it means if it is in grid object list or world object list
    public:
        virtual ~WorldObject();

        virtual void Update (uint32 /*time_diff*/) { }

        virtual void RemoveFromWorld()
        {
            if (!IsInWorld())
                return;

            DestroyForNearbyPlayers();

            Object::RemoveFromWorld();
        }

        void GetNearPoint2D(float &x, float &y, float distance, float absAngle) const;
        void GetNearPoint(WorldObject const* searcher, float &x, float &y, float &z, float searcher_size, float distance2d, float absAngle) const;
        void GetClosePoint(float &x, float &y, float &z, float size, float distance2d = 0, float angle = 0) const
        {
            // angle calculated from current orientation
            GetNearPoint(NULL, x, y, z, size, distance2d, GetOrientation() + angle);
        }
        
        void MovePosition(Position &pos, float dist, float angle);
        void GetNearPosition(Position &pos, float dist, float angle)
        {
            GetPosition(&pos);
            MovePosition(pos, dist, angle);
        }
        void MovePositionToFirstCollision(Position &pos, float dist, float angle);
        void GetFirstCollisionPosition(Position &pos, float dist, float angle)
        {
            GetPosition(&pos);
            MovePositionToFirstCollision(pos, dist, angle);
        }
        void MovePositionToCollisionBetween(Position &pos, float distMin, float distMax, float angle);
        void GetCollisionPositionBetween(Position &pos, float distMin, float distMax, float angle)
        {
            GetPosition(&pos);
            MovePositionToCollisionBetween(pos, distMin, distMax, angle);
        }
        void GetRandomNearPosition(Position &pos, float radius)
        {
            GetPosition(&pos);
            MovePosition(pos, radius * (float)rand_norm(), (float)rand_norm() * static_cast<float>(2 * M_PI));
        }

        void GetRandomNearDest(Position &pos, Position dest, float radius)
        {
            MovePosition(dest, radius * (float)rand_norm(), (float)rand_norm() * static_cast<float>(2 * M_PI));
        }

        void GetContactPoint(const WorldObject* obj, float &x, float &y, float &z, float distance2d = CONTACT_DISTANCE) const
        {
            // angle to face `obj` to `this` using distance includes size of `obj`
            GetNearPoint(obj, x, y, z, obj->GetObjectSize(), distance2d, GetAngle(obj));
        }

        float GetObjectSize() const
        {
            return (m_valuesCount > UNIT_FIELD_COMBAT_REACH) ? m_floatValues[UNIT_FIELD_COMBAT_REACH] : DEFAULT_WORLD_OBJECT_SIZE;
        }
        void UpdateGroundPositionZ(float x, float y, float &z) const;
        void UpdateAllowedPositionZ(float x, float y, float &z) const;

        void GetRandomPoint(const Position &srcPos, float distance, float &rand_x, float &rand_y, float &rand_z) const;
        void GetRandomPoint(const Position &srcPos, float distance, Position &pos) const
        {
            float x, y, z;
            GetRandomPoint(srcPos, distance, x, y, z);
            pos.Relocate(x, y, z, GetOrientation());
        }

        uint32 GetInstanceId() const { return m_InstanceId; }

        virtual void SetPhaseMask(uint32 newPhaseMask, bool update);
        uint32 GetPhaseMask() const { return m_phaseMask; }
        bool RemovePhase(uint32 removePhaseID) { return m_phaseId.erase(removePhaseID); }
        bool InSamePhase(WorldObject const* obj) const { return InSamePhase(obj->GetPhaseMask()) && InSamePhaseId(obj); }
        bool InSamePhase(uint32 phasemask) const { return (GetPhaseMask() & phasemask) != 0; }

        virtual void SetPhaseId(std::set<uint32> const& newPhaseId, bool update) { m_phaseId = newPhaseId; };
        bool HasPhaseId(uint32 PhaseID) const { return m_phaseId.find(PhaseID) != m_phaseId.end(); }
        bool HasPhaseId() const { return m_phaseId.size(); }
        std::set<uint32> GetPhases() const { return m_phaseId;  }
        bool InSamePhaseId(WorldObject const* obj) const { return obj->IgnorePhaseId() || InSamePhaseId(obj->GetPhases()); }
        bool InSamePhaseId(std::set<uint32> const& phase) const;
        void RebuildTerrainSwaps();
        void RebuildWorldMapAreaSwaps();
        std::set<uint32> const& GetTerrainSwaps() const { return _terrainSwaps; }
        std::set<uint32> const& GetWorldMapAreaSwaps() const { return _worldMapAreaSwaps; }
        int32 GetDBPhase() const { return _dbPhase; }
        bool IsInTerrainSwap(uint32 terrainSwap) const { return _terrainSwaps.find(terrainSwap) != _terrainSwaps.end(); }

        void setIgnorePhaseIdCheck(bool apply)  { m_ignorePhaseIdCheck = apply; }
        bool IgnorePhaseId() const { return m_ignorePhaseIdCheck; }

        uint32 GetZoneId() const;
        uint32 GetPZoneId() const;
        uint32 GetAreaId() const;
        void GetZoneAndAreaId(uint32& zoneid, uint32& areaid) const;

        InstanceScript* GetInstanceScript();

        const char* GetName() const { return m_name.c_str(); }
        void SetName(const std::string& newname) { m_name=newname; }

        virtual const char* GetNameForLocaleIdx(LocaleConstant /*locale_idx*/) const { return GetName(); }

        float GetDistance(const WorldObject* obj) const
        {
            float d = GetExactDist(obj) - GetObjectSize() - obj->GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance(const Position &pos) const
        {
            float d = GetExactDist(&pos) - GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance(float x, float y, float z) const
        {
            float d = GetExactDist(x, y, z) - GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance2d(const WorldObject* obj) const
        {
            float d = GetExactDist2d(obj) - GetObjectSize() - obj->GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance2d(float x, float y) const
        {
            float d = GetExactDist2d(x, y) - GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistanceZ(const WorldObject* obj) const;

        bool IsSelfOrInSameMap(const WorldObject* obj) const
        {
            if (this == obj)
                return true;
            return IsInMap(obj);
        }
        bool IsInMap(const WorldObject* obj) const
        {
            if (obj)
                return IsInWorld() && obj->IsInWorld() && (GetMap() == obj->GetMap());
            return false;
        }
        bool IsWithinDist3d(float x, float y, float z, float dist) const
            { return IsInDist(x, y, z, dist + GetObjectSize()); }
        bool IsWithinDist3d(const Position* pos, float dist) const
            { return IsInDist(pos, dist + GetObjectSize()); }
        bool IsWithinDist2d(float x, float y, float dist) const
            { return IsInDist2d(x, y, dist + GetObjectSize()); }
        bool IsWithinDist2d(const Position* pos, float dist) const
            { return IsInDist2d(pos, dist + GetObjectSize()); }
        // use only if you will sure about placing both object at same map
        bool IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D = true) const
        {
            return obj && _IsWithinDist(obj, dist2compare, is3D);
        }
        bool IsWithinDistInMap(WorldObject const* obj, float dist2compare, bool is3D = true) const
        {
            return obj && IsInMap(obj) && InSamePhase(obj) && InSamePhaseId(obj) && _IsWithinDist(obj, dist2compare, is3D);
        }
        bool IsWithinLOS(float x, float y, float z) const;
        bool IsWithinLOSInMap(const WorldObject* obj) const;
        bool GetDistanceOrder(WorldObject const* obj1, WorldObject const* obj2, bool is3D = true) const;
        bool IsInRange(WorldObject const* obj, float minRange, float maxRange, bool is3D = true) const;
        bool IsInRange2d(float x, float y, float minRange, float maxRange) const;
        bool IsInRange3d(float x, float y, float z, float minRange, float maxRange) const;
        bool isInFront(WorldObject const* target, float arc = M_PI) const;
        bool isInBack(WorldObject const* target, float arc = M_PI) const;

        bool IsInBetweenShift(const Position* obj1, const Position* obj2, float size, float shift, float angleShift) const;
        bool IsInBetween(const Position* obj1, const Position* obj2, float size = 0) const;
        bool IsInBetween(const WorldObject* obj1, float x2, float y2, float size = 0) const;

        virtual void CleanupsBeforeDelete(bool finalCleanup = true);  // used in destructor or explicitly before mass creature delete to remove cross-references to already deleted units

        virtual void SendMessageToSet(WorldPacket const* data, bool self);
        virtual void SendMessageToSetInRange(WorldPacket const* data, float dist, bool self);
        virtual void SendMessageToSet(WorldPacket const* data, Player const* skipped_rcvr);

        virtual uint8 getLevelForTarget(WorldObject const* /*target*/) const { return 1; }

        void MonsterSay(const char* text, uint32 language, ObjectGuid TargetGuid);
        void MonsterYell(const char* text, uint32 language, ObjectGuid TargetGuid);
        void MonsterTextEmote(const char* text, ObjectGuid TargetGuid, bool IsBossEmote = false);
        void MonsterWhisper(const char* text, ObjectGuid receiver, bool IsBossWhisper = false);
        void MonsterSay(int32 textId, uint32 language, ObjectGuid TargetGuid);
        void MonsterYell(int32 textId, uint32 language, ObjectGuid TargetGuid);
        void MonsterTextEmote(int32 textId, ObjectGuid TargetGuid, bool IsBossEmote = false);
        void MonsterWhisper(int32 textId, ObjectGuid receiver, bool IsBossWhisper = false);
        void MonsterYellToZone(int32 textId, uint32 language, ObjectGuid TargetGuid);
        void BuildMonsterChat(WorldPacket* data, uint8 msgtype, char const* text, uint32 language, char const* name, ObjectGuid TargetGuid) const;

        void PlayDistanceSound(uint32 soundID, Player* target = nullptr);
        void PlayDirectSound(uint32 sound_id, Player* target = NULL);

        void SendObjectDeSpawnAnim(ObjectGuid guid);

        virtual void SaveRespawnTime() {}
        void AddObjectToRemoveList();

        float GetGridActivationRange() const;
        float GetVisibilityRange() const;
        float GetSightRange(const WorldObject* target = NULL) const;
        bool canSeeOrDetect(WorldObject const* obj, bool ignoreStealth = false, bool distanceCheck = false) const;

        void SetVisible(bool x);

        FlaggedValuesArray32<int32, uint32, StealthType, TOTAL_STEALTH_TYPES> m_stealth;
        FlaggedValuesArray32<int32, uint32, StealthType, TOTAL_STEALTH_TYPES> m_stealthDetect;

        FlaggedValuesArray32<int32, uint32, InvisibilityType, TOTAL_INVISIBILITY_TYPES> m_invisibility;
        FlaggedValuesArray32<int32, uint32, InvisibilityType, TOTAL_INVISIBILITY_TYPES> m_invisibilityDetect;

        FlaggedValuesArray32<int32, uint32, ServerSideVisibilityType, TOTAL_SERVERSIDE_VISIBILITY_TYPES> m_serverSideVisibility;
        FlaggedValuesArray32<int32, uint32, ServerSideVisibilityType, TOTAL_SERVERSIDE_VISIBILITY_TYPES> m_serverSideVisibilityDetect;

        // Low Level Packets
        void SendPlaySound(uint32 Sound, bool OnlySelf);

        virtual void SetMap(Map* map);
        virtual void ResetMap();
        Map* GetMap() const { if(m_currMap) return m_currMap;else return 0; }
        Map* FindMap() const { return m_currMap; }
        //used to check all object's GetMap() calls when object is not in world!

        //this function should be removed in nearest time...
        Map const* GetBaseMap() const;

        void SetZoneScript();
        ZoneScript* GetZoneScript() const { return m_zoneScript; }

        TempSummon* SummonCreature(uint32 id, const Position &pos, TempSummonType spwtype = TEMPSUMMON_MANUAL_DESPAWN, uint32 despwtime = 0, int32 vehId = 0, ObjectGuid viewerGuid = ObjectGuid::Empty, GuidUnorderedSet* viewersList = NULL) const;
        TempSummon* SummonCreature(uint32 id, const Position &pos, ObjectGuid targetGuid, TempSummonType spwtype, uint32 despwtime, uint32 spellId = 0, SummonPropertiesEntry const* properties = NULL) const;
        TempSummon* SummonCreature(uint32 id, float x, float y, float z, float ang = 0, TempSummonType spwtype = TEMPSUMMON_MANUAL_DESPAWN, uint32 despwtime = 0, ObjectGuid viewerGuid = ObjectGuid::Empty, GuidUnorderedSet* viewersList = NULL)
        {
            if (!x && !y && !z)
            {
                GetClosePoint(x, y, z, GetObjectSize());
                ang = GetOrientation();
            }
            Position pos;
            pos.Relocate(x, y, z, ang);
            return SummonCreature(id, pos, spwtype, despwtime, 0, viewerGuid, viewersList);
        }
        GameObject* SummonGameObject(uint32 entry, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime, ObjectGuid viewerGuid = ObjectGuid::Empty, GuidUnorderedSet* viewersList = NULL);
        Creature*   SummonTrigger(float x, float y, float z, float ang, uint32 dur, CreatureAI* (*GetAI)(Creature*) = NULL);
        void SummonCreatureGroup(uint8 group, std::list<TempSummon*>* list = NULL);
        void SummonCreatureGroupDespawn(uint8 group, std::list<TempSummon*>* list = NULL);

        void GetAttackableUnitListInRange(std::list<Unit*> &list, float fMaxSearchRange) const;
        void GetAreaTriggersWithEntryInRange(std::list<AreaTrigger*>& list, uint32 entry, ObjectGuid casterGuid, float fMaxSearchRange) const;
        Creature*   FindNearestCreature(uint32 entry, float range, bool alive = true) const;
        GameObject* FindNearestGameObject(uint32 entry, float range) const;
        Player*     FindNearestPlayer(float range, bool alive = true);
        GameObject* FindNearestGameObjectOfType(GameobjectTypes type, float range) const;

        void GetGameObjectListWithEntryInGrid(std::list<GameObject*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetCreatureListWithEntryInGrid(std::list<Creature*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetAreaTriggerListWithEntryInGrid(std::list<AreaTrigger*>& atList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetPlayerListInGrid(std::list<Player*>& lList, float fMaxSearchRange) const;
        void GetAliveCreatureListWithEntryInGrid(std::list<Creature*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetCorpseCreatureInGrid(std::list<Creature*>& lList, float fMaxSearchRange) const;

        void GetGameObjectListWithEntryInGridAppend(std::list<GameObject*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetCreatureListWithEntryInGridAppend(std::list<Creature*>& lList, uint32 uiEntry, float fMaxSearchRange) const;

        void DestroyForNearbyPlayers();
        void DestroyVignetteForNearbyPlayers();
        virtual void UpdateObjectVisibility(bool forced = true);
        void BuildUpdate(UpdateDataMapType&);

        bool isActiveObject() const { return m_isActive; }
        void setActive(bool isActiveObject);
        void SetWorldObject(bool apply);
        bool IsPermanentWorldObject() const { return m_isWorldObject; }
        bool IsWorldObject() const;

        template<class NOTIFIER> void VisitNearbyObject(const float &radius, NOTIFIER &notifier, bool loadGrids = false) const { if (IsInWorld()) GetMap()->VisitAll(GetPositionX(), GetPositionY(), radius, notifier, loadGrids); }
        template<class NOTIFIER> void VisitNearbyGridObject(const float &radius, NOTIFIER &notifier, bool loadGrids = false) const { if (IsInWorld()) GetMap()->VisitGrid(GetPositionX(), GetPositionY(), radius, notifier, loadGrids); }
        template<class NOTIFIER> void VisitNearbyWorldObject(const float &radius, NOTIFIER &notifier, bool loadGrids = false) const { if (IsInWorld()) GetMap()->VisitWorld(GetPositionX(), GetPositionY(), radius, notifier, loadGrids); }

        uint32  LastUsedScriptID;

        // Transports
        Transport* GetTransport() const { return m_transport; }
        virtual float GetTransOffsetX() const { return 0; }
        virtual float GetTransOffsetY() const { return 0; }
        virtual float GetTransOffsetZ() const { return 0; }
        virtual float GetTransOffsetO() const { return 0; }
        virtual uint32 GetTransTime()   const { return 0; }
        virtual int8 GetTransSeat()     const { return -1; }
        virtual ObjectGuid GetTransGUID()   const;
        void SetTransport(Transport* t) { m_transport = t; }

        MovementInfo m_movementInfo;

        // Personal visibility system
        bool MustBeVisibleOnlyForSomePlayers() const { return !_visibilityPlayerList.empty(); }
        void GetMustBeVisibleForPlayersList(GuidUnorderedSet& playerList) { playerList = _visibilityPlayerList; }
        void ClearVisibleOnlyForSomePlayers()  { _visibilityPlayerList.clear(); }

        bool IsInPersonnalVisibilityList(ObjectGuid const& guid) const;
        void AddPlayerInPersonnalVisibilityList(ObjectGuid  const& guid) { _visibilityPlayerList.insert(guid); }
        void AddPlayersInPersonnalVisibilityList(GuidUnorderedSet const& viewerList);
        void RemovePlayerFromPersonnalVisibilityList(ObjectGuid  const& guid) { _visibilityPlayerList.erase(guid); }

        bool HideForSomePlayers() const { return !_hideForGuid.empty(); }
        void AddToHideList(ObjectGuid  const& guid) { _hideForGuid.insert(guid); }
        bool ShouldHideFor(ObjectGuid const& guid) const { return _hideForGuid.find(guid) != _hideForGuid.end();  };

        uint16 GetAIAnimKitId() const { return _aiAnimKitId; }
        void SetAIAnimKitId(uint16 animKitId);
        uint16 GetMovementAnimKitId() const { return _movementAnimKitId; }
        void SetMovementAnimKitId(uint16 animKitId);
        uint16 GetMeleeAnimKitId() const { return _meleeAnimKitId; }
        void SetMeleeAnimKitId(uint16 animKitId);
    protected:
        std::string m_name;
        bool m_isActive;
        const bool m_isWorldObject;
        ZoneScript* m_zoneScript;

        // transports
        Transport* m_transport;

        //these functions are used mostly for Relocate() and Corpse/Player specific stuff...
        //use them ONLY in LoadFromDB()/Create() funcs and nowhere else!
        //mapId/instanceId should be set in SetMap() function!
        void SetLocationMapId(uint32 _mapId) { m_mapId = _mapId; }
        void SetLocationInstanceId(uint32 _instanceId) { m_InstanceId = _instanceId; }

        virtual bool IsNeverVisible() const { return !IsInWorld(); }
        virtual bool IsAlwaysVisibleFor(WorldObject const* /*seer*/) const { return false; }
        virtual bool IsInvisibleDueToDespawn() const { return false; }
        //difference from IsAlwaysVisibleFor: 1. after distance check; 2. use owner or charmer as seer
        virtual bool IsAlwaysDetectableFor(WorldObject const* /*seer*/) const { return false; }

    private:
        uint16 _aiAnimKitId = 0;
        uint16 _movementAnimKitId = 0;
        uint16 _meleeAnimKitId = 0;

        Map* m_currMap;                                    //current object's Map location

        //uint32 m_mapId;                                     // object at map with map_id
        uint32 m_InstanceId;                                // in map copy with instance id
        uint32 m_phaseMask;                                 // in area phase state
        std::set<uint32> m_phaseId;                         // special phase. It's new generation phase, when we should check id.
        bool m_ignorePhaseIdCheck;                          // like gm mode.
        std::set<uint32> _terrainSwaps;
        std::set<uint32> _worldMapAreaSwaps;
        int32 _dbPhase;

        GuidUnorderedSet _visibilityPlayerList;
        GuidUnorderedSet _hideForGuid;

        virtual bool _IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D) const;

        bool CanNeverSee(WorldObject const* obj) const { return GetMap() != obj->GetMap() || !InSamePhase(obj); }
        virtual bool CanAlwaysSee(WorldObject const* /*obj*/) const { return false; }
        bool CanDetect(WorldObject const* obj, bool ignoreStealth) const;
        bool CanDetectInvisibilityOf(WorldObject const* obj) const;
        bool CanDetectStealthOf(WorldObject const* obj) const;
};

namespace Trinity
{
    // Binary predicate to sort WorldObjects based on the distance to a reference WorldObject
    class ObjectDistanceOrderPred
    {
        public:
            ObjectDistanceOrderPred(const WorldObject* pRefObj, bool ascending = true) : m_refObj(pRefObj), m_ascending(ascending) {}
            bool operator()(const WorldObject* pLeft, const WorldObject* pRight) const
            {
                return m_ascending ? m_refObj->GetDistanceOrder(pLeft, pRight) : !m_refObj->GetDistanceOrder(pLeft, pRight);
            }
        private:
            const WorldObject* m_refObj;
            const bool m_ascending;
    };

    // Binary predicate to sort WorldObjects based on the distance to a reference WorldObject
    class GuidValueSorterPred
    {
        public:
            GuidValueSorterPred(bool ascending = true) : m_ascending(ascending) {}
            bool operator()(const WorldObject* pLeft, const WorldObject* pRight) const
            {
                if (!pLeft->IsInWorld() || !pRight->IsInWorld())
                    return false;

                return m_ascending ? pLeft->GetGUIDLow() < pRight->GetGUIDLow() : pLeft->GetGUIDLow() > pRight->GetGUIDLow();
            }
        private:
            const bool m_ascending;
    };
}

#endif
