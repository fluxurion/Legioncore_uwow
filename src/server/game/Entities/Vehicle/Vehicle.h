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

#ifndef __TRINITY_VEHICLE_H
#define __TRINITY_VEHICLE_H

#include "ObjectDefines.h"
#include "VehicleDefines.h"
#include "Unit.h"

struct VehicleEntry;
class Unit;
class VehicleJoinEvent;

class Vehicle : public TransportBase
{
    protected:
        friend bool Unit::CreateVehicleKit(uint32 id, uint32 creatureEntry, uint32 RecAura, bool);
        Vehicle(Unit* unit, VehicleEntry const* vehInfo, uint32 creatureEntry, uint32 recAura);

        friend void Unit::RemoveVehicleKit(bool);
        ~Vehicle();

    public:
        void Install();
        void Uninstall(bool uninstallBeforeDelete = false);
        void Reset(bool evading = false);
        void InstallAllAccessories(bool evading);
        void ApplyAllImmunities();
        void InstallAccessory(VehicleAccessory const* accessory);   //! May be called from scripts

        Unit* GetBase() const { return _me; }
        VehicleEntry const* GetVehicleInfo() const { return _vehicleInfo; }
        uint32 GetCreatureEntry() const { return _creatureEntry; }

        bool HasEmptySeat(int8 seatId) const;
        Unit* GetPassenger(int8 seatId) const;
        SeatMap::const_iterator GetNextEmptySeat(int8 seatId, bool next) const;
        uint8 GetAvailableSeatCount() const;
        uint32 GetRecAura() const { return _recAura; }
        bool CheckCustomCanEnter();
        bool AddPassenger(Unit* passenger, int8 seatId = -1);
        void EjectPassenger(Unit* passenger, Unit* controller);
        void RemovePassenger(Unit* passenger);
        void RelocatePassengers();
        void RemoveAllPassengers();
        bool IsVehicleInUse() { return Seats.begin() != Seats.end(); }

        inline bool ArePassengersSpawnedByAI() const { return _passengersSpawnedByAI; }
        void SetPassengersSpawnedByAI(bool passengersSpawnedByAI) { _passengersSpawnedByAI = passengersSpawnedByAI; }

        inline bool CanBeCastedByPassengers() const { return _canBeCastedByPassengers; }
        void SetCanBeCastedByPassengers(bool canBeCastedByPassengers) { _canBeCastedByPassengers = canBeCastedByPassengers; }

        void SetLastShootPos(Position const& pos) { _lastShootPos.Relocate(pos); }
        Position GetLastShootPos() { return _lastShootPos; }

        SeatMap Seats;                                      ///< The collection of all seats on the vehicle. Including vacant ones.

        VehicleSeatEntry const* GetSeatForPassenger(Unit const* passenger) const;

        void RemovePendingEventsForPassenger(Unit* passenger);

    protected:
        friend class VehicleJoinEvent;
        uint32 UsableSeatNum;                               ///< Number of seats that match VehicleSeatEntry::UsableByPlayer, used for proper display flags

    private:
        enum Status
        {
            STATUS_NONE,
            STATUS_INSTALLED,
            STATUS_UNINSTALLING,
        };

        SeatMap::iterator GetSeatIteratorForPassenger(Unit* passenger);
        void InitMovementInfoForBase();

        /// This method transforms supplied transport offsets into global coordinates
        void CalculatePassengerPosition(float& x, float& y, float& z, float& o);

        /// This method transforms supplied global coordinates into local offsets
        void CalculatePassengerOffset(float& x, float& y, float& z, float& o);

        void RemovePendingEvent(VehicleJoinEvent* e);
        void AddPendingEvent(VehicleJoinEvent* e);
        void RemovePendingEventsForSeat(int8 seatId);

    private:
        Unit* _me;                                          ///< The underlying unit with the vehicle kit. Can be player or creature.
        VehicleEntry const* _vehicleInfo;                   ///< DBC data for vehicle
        GuidSet vehiclePlayers;
        uint32 _creatureEntry;                              ///< Can be different than the entry of _me in case of players
        Status _status;                                     ///< Internal variable for sanity checks
        Position _lastShootPos;

        uint32 _recAura;                                    ///< aura 296 SPELL_AURA_SET_VEHICLE_ID create vehicle from players.
        bool _isBeingDismissed;
        bool _passengersSpawnedByAI;
        bool _canBeCastedByPassengers;

        typedef std::list<VehicleJoinEvent*> PendingJoinEventContainer;
        PendingJoinEventContainer _pendingJoinEvents;       ///< Collection of delayed join events for prospective passengers

        std::mutex _lock;
};

class VehicleJoinEvent : public BasicEvent
{
    friend class Vehicle;
    protected:
        VehicleJoinEvent(Vehicle* v, Unit* u) : Target(v), Passenger(u), Seat(Target->Seats.end()), targetGuid(Target->GetBase() ? Target->GetBase()->GetGUID() : ObjectGuid::Empty)
        {
            if (Target->GetBase())
                ptr = Target->GetBase()->get_ptr();
        }
        ~VehicleJoinEvent();
        bool Execute(uint64, uint32);
        void Abort(uint64);

        C_PTR  ptr;
        Vehicle* Target;
        Unit* Passenger;
        SeatMap::iterator Seat;
        ObjectGuid targetGuid;
};

#endif
