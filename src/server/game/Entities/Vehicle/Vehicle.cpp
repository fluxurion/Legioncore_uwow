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

#include "Common.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Vehicle.h"
#include "Unit.h"
#include "Util.h"
#include "WorldPacket.h"
#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "ZoneScript.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "MoveSplineInit.h"
#include "EventProcessor.h"
#include "Player.h"
#include "Battleground.h"

Vehicle::Vehicle(Unit* unit, VehicleEntry const* vehInfo, uint32 creatureEntry, uint32 recAura) :
    UsableSeatNum(0), _me(unit), _vehicleInfo(vehInfo), _creatureEntry(creatureEntry), _recAura(recAura),
    _isBeingDismissed(false), _passengersSpawnedByAI(false), _canBeCastedByPassengers(false), _status(STATUS_NONE)
{
    for (uint32 i = 0; i < MAX_VEHICLE_SEATS; ++i)
    {
        if (uint32 seatId = _vehicleInfo->SeatID[i])
            if (VehicleSeatEntry const* veSeat = sVehicleSeatStore.LookupEntry(seatId))
            {
                Seats.insert(std::make_pair(i, VehicleSeat(veSeat)));
                if (veSeat->CanEnterOrExit() || veSeat->IsUsableByOverride())
                    ++UsableSeatNum;
            }
    }

    if (UsableSeatNum)
        _me->SetFlag(UNIT_FIELD_NPC_FLAGS, (_me->GetTypeId() == TYPEID_PLAYER ? UNIT_NPC_FLAG_PLAYER_VEHICLE : UNIT_NPC_FLAG_SPELLCLICK));
    else
        _me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, (_me->GetTypeId() == TYPEID_PLAYER ? UNIT_NPC_FLAG_PLAYER_VEHICLE : UNIT_NPC_FLAG_SPELLCLICK));

    InitMovementInfoForBase();
}

Vehicle::~Vehicle()
{
    /// @Uninstall must be called before this.
    //ASSERT(_status == STATUS_UNINSTALLING);
    //for (SeatMap::const_iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
        //ASSERT(!itr->second.Passenger);
}

/**
 * @fn void Vehicle::Install()
 *
 * @brief Initializes power type for vehicle. Nothing more.
 *
 * @author Machiavelli
 * @date 17-2-2013
 */

void Vehicle::Install()
{
    if (Creature* creature = _me->ToCreature())
    {
        if (PowerDisplayEntry const* powerDisplay = sPowerDisplayStore.LookupEntry(_vehicleInfo->PowerDisplayID[0]))
        {
            _me->setPowerType(Powers(powerDisplay->PowerType));
            _me->SetMaxPower(Powers(powerDisplay->PowerType), _me->GetCreatePowers(Powers(powerDisplay->PowerType), powerDisplay->ID));
            _me->SetPower(Powers(powerDisplay->PowerType), _me->GetPowerForReset(Powers(powerDisplay->PowerType), true, powerDisplay->ID));
        }
        else
        {
            Powers powerType = POWER_ENERGY;
            for (uint32 i = 0; i < MAX_SPELL_CONTROL_BAR; ++i)
            {
                uint32 spellId = i < CREATURE_MAX_SPELLS ? creature->m_temlate_spells[i] : 0;
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
                if (!spellInfo)
                    continue;

                if (!spellInfo->IsPowerActive(0))
                    continue;

                if(SpellPowerEntry const* power = spellInfo->GetPowerInfo(0))
                    powerType = (Powers)power->PowerType;
                break;
            }
            _me->setPowerType(powerType);
            _me->SetMaxPower(powerType, _me->GetCreatePowers(powerType));
            _me->SetPower(powerType, _me->GetPowerForReset(powerType, true));
        }
    }

    _status = STATUS_INSTALLED;
    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnInstall(this);
}

void Vehicle::InstallAllAccessories(bool evading)
{
    if (ArePassengersSpawnedByAI())
        return;

    if (GetBase()->GetTypeId() == TYPEID_PLAYER || !evading)
        RemoveAllPassengers();   // We might have aura's saved in the DB with now invalid casters - remove

    VehicleAccessoryList const* accessories = sObjectMgr->GetVehicleAccessoryList(this);
    if (!accessories)
        return;

    for (VehicleAccessoryList::const_iterator itr = accessories->begin(); itr != accessories->end(); ++itr)
        if (!evading || itr->IsMinion)  // only install minions on evade mode
            InstallAccessory(&(*itr));
}

/**
* @fn void Vehicle::Uninstall()
*
* @brief Removes all passengers and sets status to STATUS_UNINSTALLING.
* 		  No new passengers can be added to the vehicle after this call.
*
* @author Machiavelli
* @date 17-2-2013
*/

void Vehicle::Uninstall(bool uninstallBeforeDelete)
{
    /// @Prevent recursive uninstall call. (Bad script in OnUninstall/OnRemovePassenger/PassengerBoarded hook.)
    if (_status == STATUS_UNINSTALLING && !GetBase()->HasUnitTypeMask(UNIT_MASK_MINION))
    {
        sLog->outError(LOG_FILTER_VEHICLES, "Vehicle GuidLow: %u, Entry: %u attempts to uninstall, but already has STATUS_UNINSTALLING! "
            "Check Uninstall/PassengerBoarded script hooks for errors.", _me->GetGUIDLow(), _me->GetEntry());
        return;
    }
    _status = STATUS_UNINSTALLING;

    if (uninstallBeforeDelete)
        _isBeingDismissed = true;

    sLog->outDebug(LOG_FILTER_VEHICLES, "Vehicle::Uninstall Entry: %u, GuidLow: %u", _creatureEntry, _me->GetGUIDLow());
    RemoveAllPassengers();

    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnUninstall(this);
}

/**
* @fn void Vehicle::Reset(bool evading )
*
* @brief Reapplies immunities and reinstalls accessories. Only has effect for creatures.
*
* @author Machiavelli
* @date 17-2-2013
*
* @param evading true if called from CreatureAI::EnterEvadeMode
*/

void Vehicle::Reset(bool evading /*= false*/)
{
    if (GetBase()->GetTypeId() == TYPEID_PLAYER)
    {
        InstallAllAccessories(evading);
        return;
    }

    if (GetBase()->GetTypeId() != TYPEID_UNIT)
        return;

    sLog->outDebug(LOG_FILTER_VEHICLES, "Vehicle::Reset (Entry: %u, GuidLow: %u, DBGuid: %u)", GetCreatureEntry(), _me->GetGUIDLow(), _me->ToCreature()->GetDBTableGUIDLow());

    ApplyAllImmunities();
    InstallAllAccessories(evading);

    // Set or remove correct flags based on available seats. Will overwrite db data (if wrong).
    if (UsableSeatNum)
        _me->SetFlag(UNIT_FIELD_NPC_FLAGS, (_me->GetTypeId() == TYPEID_PLAYER ? UNIT_NPC_FLAG_PLAYER_VEHICLE : UNIT_NPC_FLAG_SPELLCLICK));
    else
        _me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, (_me->GetTypeId() == TYPEID_PLAYER ? UNIT_NPC_FLAG_PLAYER_VEHICLE : UNIT_NPC_FLAG_SPELLCLICK));

    sScriptMgr->OnReset(this);
}

/**
* @fn void Vehicle::ApplyAllImmunities()
*
* @brief Applies specific immunities that cannot be set in DB.
*
* @author Machiavelli
* @date 17-2-2013
*/

void Vehicle::ApplyAllImmunities()
{
    // This couldn't be done in DB, because some spells have MECHANIC_NONE

    // Vehicles should be immune on Knockback ...
    if (GetVehicleInfo()->ID != 2059)
    {
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
    }

    // Mechanical units & vehicles ( which are not Bosses, they have own immunities in DB ) should be also immune on healing ( exceptions in switch below )
    if (_me->ToCreature() && _me->ToCreature()->GetCreatureTemplate()->Type == CREATURE_TYPE_MECHANICAL && !_me->ToCreature()->isWorldBoss())
    {
        // Heal & dispel ...
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL, true);
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL_PCT, true);
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_DISPEL, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_PERIODIC_HEAL, true);

        // ... Shield & Immunity grant spells ...
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SCHOOL_IMMUNITY, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_UNATTACKABLE, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SCHOOL_ABSORB, true);
        _me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SHIELD, true);
        _me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_MAGICAL_IMMUNITY, true);

        // ... Resistance, Split damage, Change stats ...
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_DAMAGE_SHIELD, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SPLIT_DAMAGE_PCT, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_RESISTANCE, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_STAT, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, true);
    }

    // Different immunities for vehicles goes below
    switch (GetVehicleInfo()->ID)
    {
        // code below prevents a bug with movable cannons
        case 160: // Strand of the Ancients
        case 244: // Wintergrasp
        case 510: // Isle of Conquest
            _me->SetControlled(true, UNIT_STATE_ROOT);
            // why we need to apply this? we can simple add immunities to slow mechanic in DB
            _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DECREASE_SPEED, true);
            break;
        default:
            break;
    }
}

/**
 * @fn void Vehicle::RemoveAllPassengers()
 *
 * @brief Removes all current and pending passengers from the vehicle.
 *
 * @author Machiavelli
 * @date 17-2-2013
 */

void Vehicle::RemoveAllPassengers()
{
    sLog->outDebug(LOG_FILTER_VEHICLES, "Vehicle::RemoveAllPassengers. Entry: %u, GuidLow: %u", _creatureEntry, _me->GetGUIDLow());

    /// Setting to_Abort to true will cause @VehicleJoinEvent::Abort to be executed on next @Unit::UpdateEvents call
    /// This will properly "reset" the pending join process for the passenger.
    {
        /// Update vehicle pointer in every pending join event - Abort may be called after vehicle is deleted
        Vehicle* eventVehicle = _status != STATUS_UNINSTALLING ? this : NULL;

        _lock.lock();
        while (!_pendingJoinEvents.empty())
        {
            VehicleJoinEvent* e = _pendingJoinEvents.front();
            e->to_Abort = true;
            e->Target = eventVehicle;
            _pendingJoinEvents.pop_front();
        }
        _lock.unlock();
    }

    // Passengers always cast an aura with SPELL_AURA_CONTROL_VEHICLE on the vehicle
    // We just remove the aura and the unapply handler will make the target leave the vehicle.
    // We don't need to iterate over Seats
    _me->RemoveAurasByType(SPELL_AURA_CONTROL_VEHICLE);

    // Sometime aura do not work, so we iterate to be sure that every passengers have been removed
    // We need a copy because passenger->_ExitVehicle() may modify the Seats list
    SeatMap tempSeatMap = Seats;
    for (SeatMap::iterator itr = tempSeatMap.begin(); itr != tempSeatMap.end(); ++itr)
    {
        if (itr->second.Passenger)
        {
            if (Unit* passenger = ObjectAccessor::FindUnit(itr->second.Passenger))
                passenger->_ExitVehicle();

            itr->second.Passenger.Clear();
        }
    }
}

/**
 * @fn bool Vehicle::HasEmptySeat(int8 seatId) const
 *
 * @brief Checks if vehicle's seat specified by 'seatId' is empty.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param seatId Identifier for the seat.
 *
 * @return true if empty seat, false if not.
 */

bool Vehicle::HasEmptySeat(int8 seatId) const
{
    SeatMap::const_iterator seat = Seats.find(seatId);
    if (seat == Seats.end())
        return false;
    return !seat->second.Passenger;
}

/**
 * @fn Unit* Vehicle::GetPassenger(int8 seatId) const
 *
 * @brief Gets a passenger on specified seat.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param seatId Seat to look on.
 *
 * @return null if it not found, else pointer to passenger if in world
 */

Unit* Vehicle::GetPassenger(int8 seatId) const
{
    SeatMap::const_iterator seat = Seats.find(seatId);
    if (seat == Seats.end())
        return NULL;

    return ObjectAccessor::GetUnit(*GetBase(), seat->second.Passenger);
}

/**
 * @fn SeatMap::const_iterator Vehicle::GetNextEmptySeat(int8 seatId, bool next) const
 *
 * @brief Gets the next empty seat based on current seat.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param seatId Identifier for the current seat.
 * @param next   true if iterating forward, false means iterating backwards.
 *
 * @return The next empty seat.
 */

SeatMap::const_iterator Vehicle::GetNextEmptySeat(int8 seatId, bool next) const
{
    SeatMap::const_iterator seat = Seats.find(seatId);
    if (seat == Seats.end())
        return seat;

    while (seat->second.Passenger || (!seat->second.SeatInfo->CanEnterOrExit() && !seat->second.SeatInfo->IsUsableByOverride()))
    {
        if (next)
        {
            if (++seat == Seats.end())
                seat = Seats.begin();
        }
        else
        {
            if (seat == Seats.begin())
                seat = Seats.end();
            --seat;
        }

        // Make sure we don't loop indefinetly
        if (seat->first == seatId)
            return Seats.end();
    }

    return seat;
}

/**
 * @fn void Vehicle::InstallAccessory(uint32 entry, int8 seatId, bool minion, uint8 type,
 *     uint32 summonTime)
 *
 * @brief Installs an accessory.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param entry      The NPC entry of accessory.
 * @param seatId     Identifier for the seat to add the accessory to.
 * @param minion     true if accessory considered a 'minion'. Implies that the accessory will despawn when the vehicle despawns.
 *                   Essentially that it has no life without the vehicle. Their fates are bound.
 * @param type       See enum @SummonType.
 * @param summonTime Time after which the minion is despawned in case of a timed despawn @type specified.
 */

void Vehicle::InstallAccessory(VehicleAccessory const* as)
{
    /// @Prevent adding accessories when vehicle is uninstalling. (Bad script in OnUninstall/OnRemovePassenger/PassengerBoarded hook.)
    if (_status == STATUS_UNINSTALLING)
    {
        sLog->outError(LOG_FILTER_VEHICLES, "Vehicle (GuidLow: %u, DB GUID: %u, Entry: %u) attempts to install accessory (Entry: %u) on seat %d with STATUS_UNINSTALLING! "
            "Check Uninstall/PassengerBoarded script hooks for errors.", _me->GetGUIDLow(),
            (_me->GetTypeId() == TYPEID_UNIT ? _me->ToCreature()->GetDBTableGUIDLow() : _me->GetGUIDLow()), GetCreatureEntry(), as->AccessoryEntry, as->SeatId);
        return;
    }

    sLog->outDebug(LOG_FILTER_VEHICLES, "Vehicle (GuidLow: %u, DB Guid: %u, Entry %u): installing accessory (Entry: %u) on seat: %d",
        _me->GetGUIDLow(), (_me->GetTypeId() == TYPEID_UNIT ? _me->ToCreature()->GetDBTableGUIDLow() : _me->GetGUIDLow()), GetCreatureEntry(),
        as->AccessoryEntry, as->SeatId);

    Map* map = _me->FindMap();
    if (!map)
        return;

    // For correct initialization accessory should set owner 
    TempSummon* accessory = map->SummonCreature(as->AccessoryEntry, *_me, NULL, as->SummonTime, _me, ObjectGuid::Empty, 0, GetRecAura() ? 0 : -1);

    //ASSERT(accessory);
    if(!accessory)
        return;

    accessory->SetTempSummonType(TempSummonType(as->SummonedType));
    if (as->IsMinion)
        accessory->AddUnitTypeMask(UNIT_MASK_ACCESSORY);

    accessory->m_movementInfo.transport.Pos = as->Pos;

    // Force enter for force vehicle aura - 296
    if (GetRecAura())
        accessory->EnterVehicle(_me, -1);
    else
    (void)_me->HandleSpellClick(accessory, as->SeatId);

    /// If for some reason adding accessory to vehicle fails it will unsummon in
    /// @VehicleJoinEvent::Abort
}

bool Vehicle::CheckCustomCanEnter()
{
    switch (GetCreatureEntry())
    {
        case 56682: // Keg in Stormstout Brewery
        case 46185: // Sanitron
        case 25460: //Amazing Flying Carpet. VehID 317
        case 33513: //368
        case 33386: //360
        case 63872: //2341
        case 80578: //Q34462
            return true;
    }

    return false;
}

/**
 * @fn bool Vehicle::AddPassenger(Unit* unit, int8 seatId)
 *
 * @brief Attempts to add a passenger to the vehicle on 'seatId'.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param [in,out] The prospective passenger.
 * @param seatId        Identifier for the seat. Value of -1 indicates the next available seat.
 *
 * @return true if it succeeds, false if it fails.
 */

bool Vehicle::AddPassenger(Unit* unit, int8 seatId)
{
    /// @Prevent adding passengers when vehicle is uninstalling. (Bad script in OnUninstall/OnRemovePassenger/PassengerBoarded hook.)
    if (_status == STATUS_UNINSTALLING)
    {
        if (unit)
            sLog->outError(LOG_FILTER_VEHICLES, "Passenger GuidLow: %u, Entry: %u, attempting to board vehicle GuidLow: %u, Entry: %u during uninstall! SeatId: %d",
                unit->GetGUID().GetGUIDLow(), unit->GetEntry(), _me->GetGUID().GetGUIDLow(), _me->GetEntry(), (int32)seatId);
        return false;
    }

    if (!unit)
        return false;

    sLog->outDebug(LOG_FILTER_VEHICLES, "Unit %s scheduling enter vehicle (entry: %u, vehicleId: %u, guid: %u (dbguid: %u) on seat %d",
        unit->GetName(), _me->GetEntry(), _vehicleInfo->ID, _me->GetGUID().GetGUIDLow(),
        (_me->GetTypeId() == TYPEID_UNIT ? _me->ToCreature()->GetGUIDLow() : 0), (int32)seatId);

    // The seat selection code may kick other passengers off the vehicle.
    // While the validity of the following may be arguable, it is possible that when such a passenger
    // exits the vehicle will dismiss. That's why the actual adding the passenger to the vehicle is scheduled
    // asynchronously, so it can be cancelled easily in case the vehicle is uninstalled meanwhile.
    SeatMap::iterator seat;
    VehicleJoinEvent* e = new VehicleJoinEvent(this, unit);
    unit->m_Events.AddEvent(e, unit->m_Events.CalculateTime(0));

    if (seatId < 0) // no specific seat requirement
    {
        for (seat = Seats.begin(); seat != Seats.end(); ++seat)
            if (!seat->second.Passenger && (seat->second.SeatInfo->CanEnterOrExit() || seat->second.SeatInfo->IsUsableByOverride() || CheckCustomCanEnter()))
                break;

        if (seat == Seats.end()) // no available seat
        {
            e->to_Abort = true;
            return false;
        }

        e->Seat = seat;
        AddPendingEvent(e);
    }
    else
    {
        seat = Seats.find(seatId);
        if (seat == Seats.end())
        {
            e->to_Abort = true;
            return false;
        }

        e->Seat = seat;
        AddPendingEvent(e);
        if (seat->second.Passenger)
        {
            Unit* passenger = ObjectAccessor::GetUnit(*GetBase(), seat->second.Passenger);
            //ASSERT(passenger);
            if(passenger)
                passenger->ExitVehicle();
        }

        //ASSERT(!seat->second.Passenger);
        if(seat->second.Passenger)
            return false;
    }
    if (seat->second.SeatInfo->Flags && !(seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_ALLOW_TURNING))
        if (!(_me->ToCreature() && _me->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_VEHICLE_ATTACKABLE_PASSENGERS) &&
              !(unit->ToCreature() && unit->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_VEHICLE_ATTACKABLE_PASSENGERS))
            unit->AddUnitState(UNIT_STATE_ONVEHICLE);

    return true;
}

/**
 * @fn void Vehicle::RemovePassenger(Unit* unit)
 *
 * @brief Removes the passenger from the vehicle.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param [in,out] unit The passenger to remove.
 */

void Vehicle::RemovePassenger(Unit* unit)
{
    if (unit->GetVehicle() != this)
        return;

    SeatMap::iterator seat = GetSeatIteratorForPassenger(unit);
    //ASSERT(seat != Seats.end());
    if (seat == Seats.end())
        return;

    sLog->outDebug(LOG_FILTER_VEHICLES, "Unit %s exit vehicle entry %u id %u dbguid %u seat %d", 
        unit->GetName(), _me->GetEntry(), _vehicleInfo->ID, _me->GetGUIDLow(), (int32)seat->first);

    seat->second.Passenger.Clear();
    if (seat->second.SeatInfo->CanEnterOrExit() && ++UsableSeatNum)
        _me->SetFlag(UNIT_FIELD_NPC_FLAGS, (_me->GetTypeId() == TYPEID_PLAYER ? UNIT_NPC_FLAG_PLAYER_VEHICLE : UNIT_NPC_FLAG_SPELLCLICK));

    unit->ClearUnitState(UNIT_STATE_ONVEHICLE);

    if (_me->GetTypeId() == TYPEID_UNIT && unit->GetTypeId() == TYPEID_PLAYER)
    {
        if (seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_CAN_CONTROL)
            _me->RemoveCharmedBy(unit);
        else if (seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_UNK2)
        {
            unit->ToPlayer()->SetClientControl(unit, true);
            unit->ToPlayer()->SetViewpoint(_me, false);
            unit->ToPlayer()->SetClientControl(_me, false);
        }
    }

    if (_me->IsInWorld())
        unit->m_movementInfo.transport.Reset();

    // only for flyable vehicles
    if (unit->IsFlying())
        _me->CastSpell(unit, VEHICLE_SPELL_PARACHUTE, true);

    if (_me->GetTypeId() == TYPEID_UNIT && _me->ToCreature()->IsAIEnabled)
        _me->ToCreature()->AI()->PassengerBoarded(unit, seat->first, false);

    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnRemovePassenger(this, unit);
}

/**
 * @fn void Vehicle::RelocatePassengers()
 *
 * @brief Relocate passengers. Must be called after m_base::Relocate
 *
 * @author Machiavelli
 * @date 17-2-2013
 */

void Vehicle::RelocatePassengers()
{
    ASSERT(_me->GetMap());

    // not sure that absolute position calculation is correct, it must depend on vehicle pitch angle
    for (SeatMap::const_iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
    {
        if (Unit* passenger = ObjectAccessor::GetUnit(*GetBase(), itr->second.Passenger))
        {
            ASSERT(passenger->IsInWorld());

            float px, py, pz, po;
            passenger->m_movementInfo.transport.Pos.GetPosition(px, py, pz, po);
            CalculatePassengerPosition(px, py, pz, po);
            passenger->UpdatePosition(px, py, pz, po);
            //if (passenger->GetTypeId() == TYPEID_PLAYER)
                //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "RelocatePassengers loc(%f %f %f)", px, py, pz);
        }
    }
}

/**
 * @fn void Vehicle::InitMovementInfoForBase()
 *
 * @brief Sets correct MovementFlags2 based on VehicleFlags from DBC.
 *
 * @author Machiavelli
 * @date 17-2-2013
 */

void Vehicle::InitMovementInfoForBase()
{
    uint32 vehicleFlags = GetVehicleInfo()->Flags;

    if (vehicleFlags & VEHICLE_FLAG_NO_STRAFE)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_NO_STRAFE);
    if (vehicleFlags & VEHICLE_FLAG_NO_JUMPING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_NO_JUMPING);
    if (vehicleFlags & VEHICLE_FLAG_FULLSPEEDTURNING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_FULL_SPEED_TURNING);
    if (vehicleFlags & VEHICLE_FLAG_ALLOW_PITCHING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING);
    if (vehicleFlags & VEHICLE_FLAG_FULLSPEEDPITCHING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_FULL_SPEED_PITCHING);
}

/**
 * @fn VehicleSeatEntry const* Vehicle::GetSeatForPassenger(Unit* passenger)
 *
 * @brief Returns information on the seat of specified passenger, represented by the format in VehicleSeat.dbc
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param [in,out] The passenger for which we check the seat info.
 *
 * @return null if passenger not found on vehicle, else the DBC record for the seat.
 */

VehicleSeatEntry const* Vehicle::GetSeatForPassenger(Unit const* passenger) const
{
    for (SeatMap::const_iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (itr->second.Passenger == passenger->GetGUID())
            return itr->second.SeatInfo;

    return NULL;
}

/**
 * @fn SeatMap::iterator Vehicle::GetSeatIteratorForPassenger(Unit* passenger)
 *
 * @brief Gets seat iterator for specified passenger.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param [in,out] passenger Passenger to look up.
 *
 * @return The seat iterator for specified passenger if it's found on the vehicle. Otherwise Seats.end() (invalid iterator).
 */

SeatMap::iterator Vehicle::GetSeatIteratorForPassenger(Unit* passenger)
{
    SeatMap::iterator itr;
    for (itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (itr->second.Passenger == passenger->GetGUID())
            return itr;

    return Seats.end();
}

/**
 * @fn uint8 Vehicle::GetAvailableSeatCount() const
 *
 * @brief Gets the available seat count.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @return The available seat count.
 */

uint8 Vehicle::GetAvailableSeatCount() const
{
    uint8 ret = 0;
    SeatMap::const_iterator itr;
    for (itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (!itr->second.Passenger && (itr->second.SeatInfo->CanEnterOrExit() || itr->second.SeatInfo->IsUsableByOverride()))
            ++ret;

    return ret;
}

void Vehicle::CalculatePassengerPosition(float& x, float& y, float& z, float& o)
{
    float inx = x, iny = y, inz = z, ino = o;
    o = GetBase()->GetOrientation() + ino;
    x = GetBase()->GetPositionX() + inx * std::cos(GetBase()->GetOrientation()) - iny * std::sin(GetBase()->GetOrientation());
    y = GetBase()->GetPositionY() + iny * std::cos(GetBase()->GetOrientation()) + inx * std::sin(GetBase()->GetOrientation());
    z = GetBase()->GetPositionZ() + inz;
}

void Vehicle::CalculatePassengerOffset(float& x, float& y, float& z, float& o)
{
    o -= GetBase()->GetOrientation();
    z -= GetBase()->GetPositionZ();
    y -= GetBase()->GetPositionY();    // y = searchedY * std::cos(o) + searchedX * std::sin(o)
    x -= GetBase()->GetPositionX();    // x = searchedX * std::cos(o) + searchedY * std::sin(o + pi)
    float inx = x, iny = y;
    y = (iny - inx * tan(GetBase()->GetOrientation())) / (cos(GetBase()->GetOrientation()) + std::sin(GetBase()->GetOrientation()) * tan(GetBase()->GetOrientation()));
    x = (inx + iny * tan(GetBase()->GetOrientation())) / (cos(GetBase()->GetOrientation()) + std::sin(GetBase()->GetOrientation()) * tan(GetBase()->GetOrientation()));
}

/**
 * @fn void Vehicle::RemovePendingEvent(VehicleJoinEvent* e)
 *
 * @brief Removes @VehicleJoinEvent objects from pending join event store.
 *        This method only removes it after it's executed or aborted to prevent leaving
 *        pointers to deleted events.
 *
 * @author Shauren
 * @date 22-2-2013
 *
 * @param [in] e The VehicleJoinEvent* to remove from pending event store.
 */

void Vehicle::RemovePendingEvent(VehicleJoinEvent* /*e*/)
{
    _lock.lock();
    _pendingJoinEvents.clear();
    _lock.unlock();
}

void Vehicle::AddPendingEvent(VehicleJoinEvent* e)
{
    _lock.lock();
    _pendingJoinEvents.push_back(e);
    _lock.unlock();
}

/**
 * @fn void Vehicle::RemovePendingEventsForSeat(uint8 seatId)
 *
 * @brief Removes any pending events for given seatId. Executed when a @VehicleJoinEvent::Execute is called
 *
 * @author Machiavelli
 * @date 23-2-2013
 *
 * @param seatId Identifier for the seat.
 */

void Vehicle::RemovePendingEventsForSeat(int8 seatId)
{
    _lock.lock();
    for (PendingJoinEventContainer::iterator itr = _pendingJoinEvents.begin(); itr != _pendingJoinEvents.end();)
    {
        if ((*itr)->Seat->first == seatId)
        {
            (*itr)->to_Abort = true;
            _pendingJoinEvents.erase(itr++);
        }
        else
            ++itr;
    }
    _lock.unlock();
}

/**
 * @fn void Vehicle::RemovePendingEventsForSeat(uint8 seatId)
 *
 * @brief Removes any pending events for given passenger. Executed when vehicle control aura is removed while adding passenger is in progress
 *
 * @author Shauren
 * @date 13-2-2013
 *
 * @param passenger Unit that is supposed to enter the vehicle.
 */

void Vehicle::RemovePendingEventsForPassenger(Unit* passenger)
{
    _lock.lock();
    for (PendingJoinEventContainer::iterator itr = _pendingJoinEvents.begin(); itr != _pendingJoinEvents.end();)
    {
        if ((*itr)->Passenger == passenger)
        {
            (*itr)->to_Abort = true;
            _pendingJoinEvents.erase(itr++);
        }
        else
            ++itr;
    }
    _lock.unlock();
}

VehicleJoinEvent::~VehicleJoinEvent()
{
    Object *obj = ptr.get();
    if (!obj)
        return;
    Unit *unit = obj->ToUnit();
    if (!unit)
        return;
    Vehicle *Target = unit->GetVehicleKit();
    if (!Target)
        return;

    Target->RemovePendingEvent(this);
}

/**
 * @fn bool VehicleJoinEvent::Execute(uint64, uint32)
 *
 * @brief Actually adds the passenger @Passenger to vehicle @Target.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param parameter1 Unused
 * @param parameter2 Unused.
 *
 * @return true, cannot fail.
 *
 */

bool VehicleJoinEvent::Execute(uint64, uint32)
{
    ASSERT(Passenger->IsInWorld());
    ASSERT(Target && Target->GetBase()->IsInWorld());
    ASSERT(Target->GetRecAura() || Target->GetBase()->HasAuraTypeWithCaster(SPELL_AURA_CONTROL_VEHICLE, Passenger->GetGUID()));

    Target->RemovePendingEventsForSeat(Seat->first);
    Target->RemovePendingEventsForPassenger(Passenger);

    bool newTPos = true;
    if (Passenger->m_movementInfo.transport.Pos.m_positionX != 0.0f || Passenger->m_movementInfo.transport.Pos.m_positionY != 0.0f || Passenger->m_movementInfo.transport.Pos.m_positionZ != 0.0f || Passenger->m_movementInfo.transport.Pos.m_orientation != 0.0f)
        newTPos = false;

    Passenger->m_vehicle = Target;

    Seat->second.Passenger = Passenger->GetGUID();
    if (Seat->second.SeatInfo->CanEnterOrExit())
    {
        //ASSERT(Target->UsableSeatNum);
        if(!Target->UsableSeatNum)
            return false;
        --(Target->UsableSeatNum);
        if (!Target->UsableSeatNum)
        {
            if (Target->GetBase()->GetTypeId() == TYPEID_PLAYER)
                Target->GetBase()->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
            else
                Target->GetBase()->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }
    }

    Passenger->InterruptNonMeleeSpells(false);
    Passenger->RemoveAurasByType(SPELL_AURA_MOUNTED);
    
    VehicleSeatEntry const* veSeat = Seat->second.SeatInfo;
    if (Player* player = Passenger->ToPlayer())
    {
        // drop flag
        if (Battleground* bg = player->GetBattleground())
            bg->EventPlayerDroppedFlag(player);

        player->StopCastingCharm();
        player->StopCastingBindSight();
        player->SendOnCancelExpectedVehicleRideAura();
        if (!(veSeat->FlagsB & VEHICLE_SEAT_FLAG_B_KEEP_PET))
            player->UnsummonPetTemporaryIfAny();
    }

    if (Seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_HIDE_PASSENGER)
        Passenger->AddUnitState(UNIT_STATE_ONVEHICLE);

    if (newTPos)
        Passenger->m_movementInfo.transport.Pos.SetPosition(veSeat->AttachmentOffset);
    Passenger->m_movementInfo.transport.MoveTime = 0; // 1 for player
    Passenger->m_movementInfo.transport.VehicleSeatIndex = Seat->first;
    Passenger->m_movementInfo.transport.Guid = Target->GetBase()->GetGUID();
    Passenger->m_movementInfo.transport.VehicleRecID = Target->GetVehicleInfo()->ID;

    // Hackfix
    switch (veSeat->ID)
    {
        case 10882:
            Passenger->m_movementInfo.transport.Pos.m_positionX = 15.0f;
            Passenger->m_movementInfo.transport.Pos.m_positionY = 0.0f;
            Passenger->m_movementInfo.transport.Pos.m_positionZ = 30.0f;
            break;
        default:
            break;
    }

    //Old Trinity method
    //if (Target->GetBase()->GetTypeId() == TYPEID_UNIT && Passenger->GetTypeId() == TYPEID_PLAYER &&
    //    Seat->second.SeatInfo->m_flags & VEHICLE_SEAT_FLAG_CAN_CONTROL)
    //    ASSERT(Target->GetBase()->SetCharmedBy(Passenger, CHARM_TYPE_VEHICLE))

    if (Target->GetBase()->GetTypeId() == TYPEID_UNIT && Passenger->GetTypeId() == TYPEID_PLAYER)
    {
        if (Seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_CAN_CONTROL
            && !Target->GetBase()->SetCharmedBy(Passenger, CHARM_TYPE_VEHICLE))     // SMSG_CLIENT_CONTROL
        {
            //ASSERT(false);
            return false;
        }
        else if (Seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_UNK2 && Seat->second.SeatInfo->Flags & VEHICLE_SEAT_FLAG_CAN_CONTROL)
        {
            Passenger->Dismount();
            Passenger->ToPlayer()->SetClientControl(Target->GetBase(), 1);
            Passenger->ToPlayer()->SetMover(Target->GetBase());
            Passenger->ToPlayer()->SetViewpoint(Target->GetBase(), true);
        }
    }

    Passenger->SendClearTarget();                            // SMSG_BREAK_TARGET
    Passenger->SetControlled(true, UNIT_STATE_ROOT);         // SMSG_FORCE_ROOT - In some cases we send SMSG_MOVE_SPLINE_ROOT here (for creatures)
    // also adds MOVEMENTFLAG_ROOT
    
    Movement::MoveSplineInit init(*Passenger);
    init.DisableTransportPathTransformations();
    init.MoveTo(veSeat->AttachmentOffset.X, veSeat->AttachmentOffset.Y, veSeat->AttachmentOffset.Z, false, true);
    init.SetFacing(0.0f);
    init.SetTransportEnter();
    init.Launch();

    //not we could install accessory
    if (Creature *c = Passenger->ToCreature())
    {
        if (c->onVehicleAccessoryInit())
        {
            // Before add to map call initialization accasorys if it has.
            if (c->GetVehicleKit())
                c->GetVehicleKit()->Reset();

            // and after initialization we finally could see
            c->SetVehicleAccessoryInit(false);
            Passenger->UpdateObjectVisibility(true);
        }
    }

    if (Target->GetBase()->GetTypeId() == TYPEID_UNIT)
    {
        if (Target->GetBase()->ToCreature()->IsAIEnabled)
            Target->GetBase()->ToCreature()->AI()->PassengerBoarded(Passenger, Seat->first, true);

        sScriptMgr->OnAddPassenger(Target, Passenger, Seat->first);

        // Actually quite a redundant hook. Could just use OnAddPassenger and check for unit typemask inside script.
        if (Passenger->HasUnitTypeMask(UNIT_MASK_ACCESSORY))
            sScriptMgr->OnInstallAccessory(Target, Passenger->ToCreature());
    }
    return true;
}

/**
 * @fn void VehicleJoinEvent::Abort(uint64)
 *
 * @brief Aborts the event. Implies that unit @Passenger will not be boarding vehicle @Target after all.
 *
 * @author Machiavelli
 * @date 17-2-2013
 *
 * @param parameter1 Unused
 */

void VehicleJoinEvent::Abort(uint64)
{
    Object *obj = ptr.get();
    Unit *targetBase = obj ? obj->ToUnit() : NULL; // Faster then ObjectAccessor::GetUnit

    /// Check if the Vehicle was already uninstalled, in which case all auras were removed already
    //if (Target)
    //if (Unit* targetBase = ObjectAccessor::GetUnit(*Passenger, targetGuid))
    if (targetBase)
    {
        //sLog->outDebug(LOG_FILTER_VEHICLES, "Passenger GuidLow: %u, Entry: %u, board on vehicle GuidLow: %u, Entry: %u SeatId: %d cancelled",
            //Passenger->GetGUIDLow(), Passenger->GetEntry(), Target->GetBase()->GetGUIDLow(), Target->GetBase()->GetEntry(), (int32)Seat->first);

        /// @SPELL_AURA_CONTROL_VEHICLE auras can be applied even when the passenger is not (yet) on the vehicle.
        /// When this code is triggered it means that something went wrong in @Vehicle::AddPassenger, and we should remove
        /// the aura manually.
        //if(Unit* targetBase = Target->GetBase())
            //if (targetBase->IsInWorld())
                targetBase->RemoveAurasByType(SPELL_AURA_CONTROL_VEHICLE, Passenger->GetGUID());
    }
    else if (Passenger)
        sLog->outDebug(LOG_FILTER_VEHICLES, "Passenger GuidLow: %u, Entry: %u, board on uninstalled vehicle SeatId: %d cancelled",
        Passenger->GetGUIDLow(), Passenger->GetEntry(), (int32) Seat->first);
    else
        sLog->outError(LOG_FILTER_VEHICLES, " WARNING!!! VehicleJoinEvent Abort with non existen Passanger");

    if (Passenger && Passenger->IsInWorld() && Passenger->HasUnitTypeMask(UNIT_MASK_ACCESSORY))
        Passenger->ToCreature()->DespawnOrUnsummon();
}