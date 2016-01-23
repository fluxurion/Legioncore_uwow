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

#include "ObjectAccessor.h"
#include "ObjectMgr.h"

#include "Player.h"
#include "Creature.h"
#include "GameObject.h"
#include "DynamicObject.h"
#include "Vehicle.h"
#include "WorldPacket.h"
#include "Item.h"
#include "Corpse.h"
#include "GridNotifiers.h"
#include "MapManager.h"
#include "Map.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "Opcodes.h"
#include "ObjectDefines.h"
#include "MapInstanced.h"
#include "World.h"

#include <cmath>

ObjectAccessor::ObjectAccessor()
{
}

ObjectAccessor::~ObjectAccessor()
{
}

WorldObject* ObjectAccessor::GetWorldObject(WorldObject const& p, ObjectGuid guid)
{
    switch (guid.GetHigh())
    {
        case HighGuid::Player:        return GetPlayer(p, guid);
        case HighGuid::Transport:
        case HighGuid::GameObject:    return GetGameObject(p, guid);
        case HighGuid::Vehicle:
        case HighGuid::Creature:          return GetCreature(p, guid);
        case HighGuid::Pet:           return GetPet(p, guid);
        case HighGuid::DynamicObject: return GetDynamicObject(p, guid);
        case HighGuid::AreaTrigger:   return GetAreaTrigger(p, guid);
        case HighGuid::Corpse:        return GetCorpse(p, guid);
        case HighGuid::Conversation:  return GetConversation(p, guid);
        default:                     return NULL;
    }
}

Object* ObjectAccessor::GetObjectByTypeMask(WorldObject const& p, ObjectGuid guid, uint32 typemask)
{
    switch (guid.GetHigh())
    {
        case HighGuid::Item:
            if (typemask & TYPEMASK_ITEM && p.GetTypeId() == TYPEID_PLAYER)
                return ((Player const&)p).GetItemByGuid(guid);
            break;
        case HighGuid::Player:
            if (typemask & TYPEMASK_PLAYER)
                return GetPlayer(p, guid);
            break;
        case HighGuid::Transport:
        case HighGuid::GameObject:
            if (typemask & TYPEMASK_GAMEOBJECT)
                return GetGameObject(p, guid);
            break;
        case HighGuid::Creature:
        case HighGuid::Vehicle:
            if (typemask & TYPEMASK_UNIT)
                return GetCreature(p, guid);
            break;
        case HighGuid::Pet:
            if (typemask & TYPEMASK_UNIT)
                return GetPet(p, guid);
            break;
        case HighGuid::DynamicObject:
            if (typemask & TYPEMASK_DYNAMICOBJECT)
                return GetDynamicObject(p, guid);
            break;
        case HighGuid::AreaTrigger:
            if (typemask & TYPEMASK_AREATRIGGER)
                return GetAreaTrigger(p, guid);
        case HighGuid::Conversation:
            if (typemask & TYPEMASK_CONVERSATION)
                return GetConversation(p, guid);
        case HighGuid::Corpse:
            break;
        default:
            break;
    }

    return NULL;
}

Corpse* ObjectAccessor::GetCorpse(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Corpse*)NULL);
}

GameObject* ObjectAccessor::GetGameObject(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (GameObject*)NULL);
}

DynamicObject* ObjectAccessor::GetDynamicObject(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (DynamicObject*)NULL);
}

AreaTrigger* ObjectAccessor::GetAreaTrigger(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (AreaTrigger*)NULL);
}

Conversation* ObjectAccessor::GetConversation(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Conversation*)NULL);
}

Unit* ObjectAccessor::GetUnit(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Unit*)NULL);
}

Creature* ObjectAccessor::GetCreature(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Creature*)NULL);
}

Pet* ObjectAccessor::GetPet(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Pet*)NULL);
}

Player* ObjectAccessor::GetPlayer(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Player*)NULL);
}

Transport* ObjectAccessor::GetTransport(WorldObject const& u, ObjectGuid guid)
{
    return GetObjectInMap(guid, u.GetMap(), (Transport*)NULL);
}

Creature* ObjectAccessor::GetCreatureOrPetOrVehicle(WorldObject const& u, ObjectGuid guid)
{
    if (guid.IsPet())
        return GetPet(u, guid);

    if (guid.IsCreatureOrVehicle())
        return GetCreature(u, guid);

    return NULL;
}

Pet* ObjectAccessor::FindPet(ObjectGuid const& guid)
{
    return GetObjectInWorld(guid, (Pet*)NULL);
}

Player* ObjectAccessor::FindPlayer(ObjectGuid const& guid, bool checInWorld/*=true*/)
{
    Player* res = GetObjectInWorld(guid, (Player*)NULL);
    if (res && !res->IsInWorld())
        return NULL;
    return res;
}

Unit* ObjectAccessor::FindUnit(ObjectGuid const& guid)
{
    return GetObjectInWorld(guid, (Unit*)NULL);
}

Player* ObjectAccessor::FindPlayerByName(const char* name)
{
    boost::shared_lock<boost::shared_mutex> lock(*HashMapHolder<Player>::GetLock());

    std::string nameStr = name;
    std::transform(nameStr.begin(), nameStr.end(), nameStr.begin(), ::tolower);
    HashMapHolder<Player>::MapType const& m = GetPlayers();
    for (HashMapHolder<Player>::MapType::const_iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        if (!iter->second->IsInWorld())
            continue;
        std::string currentName = iter->second->GetName();
        std::transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);
        if (nameStr.compare(currentName) == 0)
            return iter->second;
    }

    return nullptr;
}

Player* ObjectAccessor::FindConnectedPlayerByName(std::string const& name)
{
    boost::shared_lock<boost::shared_mutex> lock(*HashMapHolder<Player>::GetLock());

    std::string nameStr = name;
    std::transform(nameStr.begin(), nameStr.end(), nameStr.begin(), ::tolower);
    HashMapHolder<Player>::MapType const& m = GetPlayers();
    for (HashMapHolder<Player>::MapType::const_iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        std::string currentName = iter->second->GetName();
        std::transform(currentName.begin(), currentName.end(), currentName.begin(), ::tolower);
        if (nameStr.compare(currentName) == 0)
            return iter->second;
    }

    return nullptr;
}

void ObjectAccessor::SaveAllPlayers()
{
    boost::shared_lock<boost::shared_mutex> lock(*HashMapHolder<Player>::GetLock());

    HashMapHolder<Player>::MapType const& m = GetPlayers();
    for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        itr->second->SaveToDB();
}

Corpse* ObjectAccessor::GetCorpseForPlayerGUID(ObjectGuid guid)
{
    boost::shared_lock<boost::shared_mutex> lock(_corpseLock);

    Player2CorpsesMapType::iterator iter = i_player2corpse.find(guid);
    if (iter == i_player2corpse.end())
        return NULL;

    ASSERT(iter->second->GetType() != CORPSE_BONES);

    return iter->second;
}

void ObjectAccessor::RemoveCorpse(Corpse* corpse)
{
    ASSERT(corpse && corpse->GetType() != CORPSE_BONES);

    boost::upgrade_lock<boost::shared_mutex> lock(_corpseLock);

    /// @todo more works need to be done for corpse and other world object
    if (Map* map = corpse->FindMap())
    {
        corpse->DestroyForNearbyPlayers();
        if (corpse->IsInGrid())
            map->RemoveFromMap(corpse, false);
        else
        {
            corpse->RemoveFromWorld();
            corpse->ResetMap();
        }
    }
    else

        corpse->RemoveFromWorld();

    // Critical section
    {
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

        Player2CorpsesMapType::iterator iter = i_player2corpse.find(corpse->GetOwnerGUID());
        if (iter == i_player2corpse.end()) // TODO: Fix this
            return;

        // build mapid*cellid -> guid_set map
        CellCoord cellCoord = Trinity::ComputeCellCoord(corpse->GetPositionX(), corpse->GetPositionY());
        sObjectMgr->DeleteCorpseCellData(corpse->GetMapId(), cellCoord.GetId(), corpse->GetOwnerGUID());

        i_player2corpse.erase(iter);
    }
}

void ObjectAccessor::AddCorpse(Corpse* corpse)
{
    ASSERT(corpse && corpse->GetType() != CORPSE_BONES);

    // Critical section
    {
        boost::unique_lock<boost::shared_mutex> lock(_corpseLock);

        ASSERT(i_player2corpse.find(corpse->GetOwnerGUID()) == i_player2corpse.end());
        i_player2corpse[corpse->GetOwnerGUID()] = corpse;

        // build mapid*cellid -> guid_set map
        CellCoord cellCoord = Trinity::ComputeCellCoord(corpse->GetPositionX(), corpse->GetPositionY());
        sObjectMgr->AddCorpseCellData(corpse->GetMapId(), cellCoord.GetId(), corpse->GetOwnerGUID(), corpse->GetInstanceId());
    }
}

void ObjectAccessor::AddCorpsesToGrid(GridCoord const& gridpair, GridType& grid, Map* map)
{
    boost::shared_lock<boost::shared_mutex> lock(_corpseLock);

    for (Player2CorpsesMapType::iterator iter = i_player2corpse.begin(); iter != i_player2corpse.end(); ++iter)
    {
        // We need this check otherwise a corpose may be added to a grid twice
        if (iter->second->IsInGrid())
            continue;

        if (iter->second->GetGridCoord() == gridpair)
        {
            // verify, if the corpse in our instance (add only corpses which are)
            if (map->Instanceable())
            {
                if (iter->second->GetInstanceId() == map->GetInstanceId())
                    grid.AddWorldObject(iter->second);
            }
            else
                grid.AddWorldObject(iter->second);
        }
    }
}

Corpse* ObjectAccessor::ConvertCorpseForPlayer(ObjectGuid player_guid, bool insignia /*=false*/)
{
    Corpse* corpse = GetCorpseForPlayerGUID(player_guid);
    if (!corpse)
    {
        //in fact this function is called from several places
        //even when player doesn't have a corpse, not an error
        return NULL;
    }

    sLog->outDebug(LOG_FILTER_GENERAL, "Deleting Corpse and spawned bones.");

    // Map can be NULL
    Map* map = corpse->FindMap();

    // remove corpse from player_guid -> corpse map and from current map
    RemoveCorpse(corpse);

    // remove corpse from DB
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    corpse->DeleteFromDB(trans);
    CharacterDatabase.CommitTransaction(trans);

    Corpse* bones = NULL;
    // create the bones only if the map and the grid is loaded at the corpse's location
    // ignore bones creating option in case insignia

    if (map && (insignia ||
        (map->IsBattlegroundOrArena() ? sWorld->getBoolConfig(CONFIG_DEATH_BONES_BG_OR_ARENA) : sWorld->getBoolConfig(CONFIG_DEATH_BONES_WORLD))) &&
        !map->IsRemovalGrid(corpse->GetPositionX(), corpse->GetPositionY()))
    {
        // Create bones, don't change Corpse
        bones = new Corpse;
        bones->Create(corpse->GetGUIDLow(), map);

        for (uint8 i = OBJECT_FIELD_TYPE + 1; i < CORPSE_END; ++i)                    // don't overwrite guid and object type
            bones->SetUInt32Value(i, corpse->GetUInt32Value(i));

        bones->SetGridCoord(corpse->GetGridCoord());
        // bones->m_time = m_time;                              // don't overwrite time
        // bones->m_type = m_type;                              // don't overwrite type
        bones->Relocate(corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ(), corpse->GetOrientation());
        bones->SetPhaseMask(corpse->GetPhaseMask(), false);

        bones->SetUInt32Value(CORPSE_FIELD_FLAGS, CORPSE_FLAG_UNK2 | CORPSE_FLAG_BONES);
        bones->SetGuidValue(CORPSE_FIELD_OWNER, ObjectGuid::Empty);

        for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        {
            if (corpse->GetUInt32Value(CORPSE_FIELD_ITEMS + i))
                bones->SetUInt32Value(CORPSE_FIELD_ITEMS + i, 0);
        }

        // add bones in grid store if grid loaded where corpse placed
        map->AddToMap(bones);
    }

    // all references to the corpse should be removed at this point
    delete corpse;

    return bones;
}

void ObjectAccessor::RemoveOldCorpses()
{
    time_t now = time(NULL);
    Player2CorpsesMapType::iterator next;
    for (Player2CorpsesMapType::iterator itr = i_player2corpse.begin(); itr != i_player2corpse.end(); itr = next)
    {
        next = itr;
        ++next;

        if (!itr->second->IsExpired(now))
            continue;

        ConvertCorpseForPlayer(itr->first);
    }
}

void ObjectAccessor::Update(uint32 /*diff*/)
{
    UpdateDataMapType update_players;

    while (!i_objects.empty())
    {
        Object* obj = *i_objects.begin();
        ASSERT(obj && obj->IsInWorld());
        i_objects.erase(i_objects.begin());
        obj->BuildUpdate(update_players);
    }

    WorldPacket packet;                                     // here we allocate a std::vector with a size of 0x10000
    for (UpdateDataMapType::iterator iter = update_players.begin(); iter != update_players.end(); ++iter)
    {
        iter->second.BuildPacket(&packet);
        iter->first->GetSession()->SendPacket(&packet);
        packet.clear();                                     // clean the string
    }
}

void ObjectAccessor::UnloadAll()
{
    for (Player2CorpsesMapType::const_iterator itr = i_player2corpse.begin(); itr != i_player2corpse.end(); ++itr)
    {
        itr->second->RemoveFromWorld();
        delete itr->second;
    }
}

/// Define the static members of HashMapHolder

template <class T> std::unordered_map< ObjectGuid, T* > HashMapHolder<T>::_objectMap;
template <class T> boost::shared_mutex HashMapHolder<T>::_lock;

/// Global definitions for the hashmap storage

template class HashMapHolder<Player>;
template class HashMapHolder<Pet>;
template class HashMapHolder<GameObject>;
template class HashMapHolder<DynamicObject>;
template class HashMapHolder<Creature>;
template class HashMapHolder<Corpse>;
template class HashMapHolder<Transport>;
template class HashMapHolder<Conversation>;

template Player* ObjectAccessor::GetObjectInWorld<Player>(uint32 mapid, float x, float y, ObjectGuid guid, Player* /*fake*/);
template Pet* ObjectAccessor::GetObjectInWorld<Pet>(uint32 mapid, float x, float y, ObjectGuid guid, Pet* /*fake*/);
template Creature* ObjectAccessor::GetObjectInWorld<Creature>(uint32 mapid, float x, float y, ObjectGuid guid, Creature* /*fake*/);
template Corpse* ObjectAccessor::GetObjectInWorld<Corpse>(uint32 mapid, float x, float y, ObjectGuid guid, Corpse* /*fake*/);
template GameObject* ObjectAccessor::GetObjectInWorld<GameObject>(uint32 mapid, float x, float y, ObjectGuid guid, GameObject* /*fake*/);
template DynamicObject* ObjectAccessor::GetObjectInWorld<DynamicObject>(uint32 mapid, float x, float y, ObjectGuid guid, DynamicObject* /*fake*/);
template Transport* ObjectAccessor::GetObjectInWorld<Transport>(uint32 mapid, float x, float y, ObjectGuid guid, Transport* /*fake*/);
