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

#ifndef TRINITY_OBJECTACCESSOR_H
#define TRINITY_OBJECTACCESSOR_H

#include <mutex>
#include <set>
#include <unordered_map>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "Define.h"
#include "GridDefines.h"
#include "UpdateData.h"
#include "Object.h"
#include "Player.h"
#include "Transport.h"


class Creature;
class Corpse;
class Unit;
class GameObject;
class DynamicObject;
class WorldObject;
class Vehicle;
class Map;
class WorldRunnable;
class Transport;

template <class T>
class HashMapHolder
{
    public:

        typedef std::unordered_map<ObjectGuid, T*> MapType;

        static void Insert(T* o)
        {
            boost::unique_lock<boost::shared_mutex> lock(_lock);

            _objectMap[o->GetGUID()] = o;
        }

        static void Remove(T* o)
        {
            boost::unique_lock<boost::shared_mutex> lock(_lock);

            _objectMap.erase(o->GetGUID());
        }

        static T* Find(ObjectGuid guid)
        {
            boost::shared_lock<boost::shared_mutex> lock(_lock);

            typename MapType::iterator itr = _objectMap.find(guid);
            return (itr != _objectMap.end()) ? itr->second : NULL;
        }

        static MapType& GetContainer() { return _objectMap; }

        static boost::shared_mutex* GetLock() { return &_lock; }

    private:

        //Non instanceable only static
        HashMapHolder() {}

        static boost::shared_mutex _lock;
        static MapType _objectMap;
};

class ObjectAccessor
{
    private:
        ObjectAccessor();
        ~ObjectAccessor();
        ObjectAccessor(const ObjectAccessor&);
        ObjectAccessor& operator=(const ObjectAccessor&);

    public:
        // TODO: override these template functions for each holder type and add assertions

        static ObjectAccessor* instance()
        {
            static ObjectAccessor instance;
            return &instance;
        }

        template<class T> static T* GetObjectInOrOutOfWorld(ObjectGuid guid, T* /*typeSpecifier*/)
        {
            return HashMapHolder<T>::Find(guid);
        }

        static Unit* GetObjectInOrOutOfWorld(ObjectGuid guid, Unit* /*typeSpecifier*/)
        {
            if (guid.IsPlayer())
                return (Unit*)GetObjectInOrOutOfWorld(guid, (Player*)NULL);

            if (guid.IsPet())
                return (Unit*)GetObjectInOrOutOfWorld(guid, (Pet*)NULL);

            return (Unit*)GetObjectInOrOutOfWorld(guid, (Creature*)NULL);
        }

        // returns object if is in world
        template<class T> static T* GetObjectInWorld(ObjectGuid guid, T* /*typeSpecifier*/)
        {
            return HashMapHolder<T>::Find(guid);
        }

        // Player may be not in world while in ObjectAccessor
        static Player* GetObjectInWorld(ObjectGuid guid, Player* /*typeSpecifier*/)
        {
            Player* player = HashMapHolder<Player>::Find(guid);
            if (player && player->IsInWorld())
                return player;
            return NULL;
        }

        static Unit* GetObjectInWorld(ObjectGuid guid, Unit* /*typeSpecifier*/)
        {
            if (guid.IsPlayer())
                return (Unit*)GetObjectInWorld(guid, (Player*)NULL);

            if (guid.IsPet())
                return (Unit*)GetObjectInWorld(guid, (Pet*)NULL);

            return (Unit*)GetObjectInWorld(guid, (Creature*)NULL);
        }

        // returns object if is in map
        template<class T> static T* GetObjectInMap(ObjectGuid guid, Map* map, T* /*typeSpecifier*/)
        {
            ASSERT(map);
            if (T * obj = GetObjectInWorld(guid, (T*)NULL))
                if (obj->GetMap() == map)
                    return obj;
            return NULL;
        }

        template<class T> static T* GetObjectInWorld(uint32 mapid, float x, float y, ObjectGuid guid, T* /*fake*/)
        {
            T* obj = HashMapHolder<T>::Find(guid);
            if (!obj || obj->GetMapId() != mapid)
                return NULL;

            CellCoord p = Trinity::ComputeCellCoord(x, y);
            if (!p.IsCoordValid())
            {
                sLog->outError(LOG_FILTER_GENERAL, "ObjectAccessor::GetObjectInWorld: invalid coordinates supplied X:%f Y:%f grid cell [%u:%u]", x, y, p.x_coord, p.y_coord);
                return NULL;
            }

            CellCoord q = Trinity::ComputeCellCoord(obj->GetPositionX(), obj->GetPositionY());
            if (!q.IsCoordValid())
            {
                sLog->outError(LOG_FILTER_GENERAL, "ObjectAccessor::GetObjecInWorld: object (GUID: %u TypeId: %u) has invalid coordinates X:%f Y:%f grid cell [%u:%u]", obj->GetGUIDLow(), obj->GetTypeId(), obj->GetPositionX(), obj->GetPositionY(), q.x_coord, q.y_coord);
                return NULL;
            }

            int32 dx = int32(p.x_coord) - int32(q.x_coord);
            int32 dy = int32(p.y_coord) - int32(q.y_coord);

            if (dx > -2 && dx < 2 && dy > -2 && dy < 2)
                return obj;
            else
                return NULL;
        }

        // these functions return objects only if in map of specified object
        static WorldObject* GetWorldObject(WorldObject const&, ObjectGuid);
        static Object* GetObjectByTypeMask(WorldObject const&, ObjectGuid, uint32 typemask);
        static Corpse* GetCorpse(WorldObject const& u, ObjectGuid guid);
        static GameObject* GetGameObject(WorldObject const& u, ObjectGuid guid);
        static DynamicObject* GetDynamicObject(WorldObject const& u, ObjectGuid guid);
        static AreaTrigger* GetAreaTrigger(WorldObject const& u, ObjectGuid guid);
        static Conversation* GetConversation(WorldObject const& u, ObjectGuid guid);
        static Unit* GetUnit(WorldObject const&, ObjectGuid guid);
        static Creature* GetCreature(WorldObject const& u, ObjectGuid guid);
        static Pet* GetPet(WorldObject const&, ObjectGuid guid);
        static Player* GetPlayer(WorldObject const&, ObjectGuid guid);
        static Creature* GetCreatureOrPetOrVehicle(WorldObject const&, ObjectGuid);
        static Transport* GetTransport(WorldObject const& u, ObjectGuid guid);

        // these functions return objects if found in whole world
        // ACCESS LIKE THAT IS NOT THREAD SAFE
        static Pet* FindPet(ObjectGuid const& g);
        static Player* FindPlayer(ObjectGuid const& g, bool inWorld = true);
        static Unit* FindUnit(ObjectGuid const& g);
        static Player* FindPlayerByName(const char* name);
        static Player* FindConnectedPlayerByName(std::string const& name);

        // when using this, you must use the hashmapholder's lock
        static HashMapHolder<Player>::MapType const& GetPlayers()
        {
            return HashMapHolder<Player>::GetContainer();
        }

        // when using this, you must use the hashmapholder's lock
        static HashMapHolder<Creature>::MapType const& GetCreatures()
        {
            return HashMapHolder<Creature>::GetContainer();
        }

        // when using this, you must use the hashmapholder's lock
        static HashMapHolder<GameObject>::MapType const& GetGameObjects()
        {
            return HashMapHolder<GameObject>::GetContainer();
        }

        template<class T> static void AddObject(T* object)
        {
            HashMapHolder<T>::Insert(object);
        }

        template<class T> static void RemoveObject(T* object)
        {
            HashMapHolder<T>::Remove(object);
        }

        static void SaveAllPlayers();

        //non-static functions
        void AddUpdateObject(Object* obj)
        {
            std::lock_guard<std::mutex> lock(_objectLock);
            i_objects.insert(obj);
        }

        void RemoveUpdateObject(Object* obj)
        {
            std::lock_guard<std::mutex> lock(_objectLock);
            i_objects.erase(obj);
        }

        //Thread safe
        Corpse* GetCorpseForPlayerGUID(ObjectGuid guid);
        void RemoveCorpse(Corpse* corpse);
        void AddCorpse(Corpse* corpse);
        void AddCorpsesToGrid(GridCoord const& gridpair, GridType& grid, Map* map);
        Corpse* ConvertCorpseForPlayer(ObjectGuid player_guid, bool insignia = false);

        //Thread unsafe
        void Update(uint32 diff);
        void RemoveOldCorpses();
        void UnloadAll();

    private:
        static void _buildChangeObjectForPlayer(WorldObject*, UpdateDataMapType&);
        static void _buildPacket(Player*, Object*, UpdateDataMapType&);
        void _update();

        typedef std::unordered_map<ObjectGuid, Corpse*> Player2CorpsesMapType;
        typedef std::unordered_map<Player*, UpdateData>::value_type UpdateDataValueType;

        std::set<Object*> i_objects;
        Player2CorpsesMapType i_player2corpse;

        std::mutex _objectLock;
        boost::shared_mutex _corpseLock;
};

#define sObjectAccessor ObjectAccessor::instance()
#endif
