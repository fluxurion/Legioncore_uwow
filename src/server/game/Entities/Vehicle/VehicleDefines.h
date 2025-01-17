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

#ifndef __TRINITY_VEHICLEDEFINES_H
#define __TRINITY_VEHICLEDEFINES_H

struct VehicleSeatEntry;

enum VehicleFlags
{
    VEHICLE_FLAG_NO_STRAFE                       = 0x00000001,           // Sets MOVEFLAG2_NO_STRAFE
    VEHICLE_FLAG_NO_JUMPING                      = 0x00000002,           // Sets MOVEFLAG2_NO_JUMPING
    VEHICLE_FLAG_FULLSPEEDTURNING                = 0x00000004,           // Sets MOVEFLAG2_FULLSPEEDTURNING
    VEHICLE_FLAG_ALLOW_PITCHING                  = 0x00000010,           // Sets MOVEFLAG2_ALLOW_PITCHING
    VEHICLE_FLAG_FULLSPEEDPITCHING               = 0x00000020,           // Sets MOVEFLAG2_FULLSPEEDPITCHING
    VEHICLE_FLAG_CUSTOM_PITCH                    = 0x00000040,           // If set use pitchMin and pitchMax from DBC, otherwise pitchMin = -pi/2, pitchMax = pi/2
    VEHICLE_FLAG_ADJUST_AIM_ANGLE                = 0x00000400,           // Lua_IsVehicleAimAngleAdjustable
    VEHICLE_FLAG_ADJUST_AIM_POWER                = 0x00000800,           // Lua_IsVehicleAimPowerAdjustable
};

enum VehicleSpells
{
    VEHICLE_SPELL_RIDE_HARDCODED                 = 46598,
    VEHICLE_SPELL_PARACHUTE                      = 45472
};

struct VehicleSeat
{
    explicit VehicleSeat(VehicleSeatEntry const* seatInfo) : SeatInfo(seatInfo), Passenger(ObjectGuid::Empty) {}
    VehicleSeatEntry const* SeatInfo;
    ObjectGuid Passenger;
};

struct VehicleAccessory
{
    VehicleAccessory(uint32 entry, int8 seatId, bool isMinion, uint8 summonType, uint32 summonTime, Position pos) :
        AccessoryEntry(entry), IsMinion(isMinion), SummonTime(summonTime), SeatId(seatId), SummonedType(summonType), Pos(pos) { }

    uint32 AccessoryEntry;
    bool IsMinion;
    uint32 SummonTime;
    int8 SeatId;
    uint8 SummonedType;
    Position Pos;
};

typedef std::vector<VehicleAccessory> VehicleAccessoryList;
typedef std::map<ObjectGuid::LowType, VehicleAccessoryList> VehicleAccessoryContainer;
typedef std::map<uint32, VehicleAccessoryList> VehicleAccessoryTemplateContainer;
typedef std::map<int8, VehicleSeat> SeatMap;

class TransportBase
{
    public:
        /// This method transforms supplied transport offsets into global coordinates
        virtual void CalculatePassengerPosition(float& x, float& y, float& z, float& o) = 0;

        /// This method transforms supplied global coordinates into local offsets
        virtual void CalculatePassengerOffset(float& x, float& y, float& z, float& o) = 0;
};
#endif
