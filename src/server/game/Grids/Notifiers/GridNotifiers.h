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

#ifndef TRINITY_GRIDNOTIFIERS_H
#define TRINITY_GRIDNOTIFIERS_H

#include "ObjectGridLoader.h"
#include "UpdateData.h"
#include <iostream>

#include "Corpse.h"
#include "Object.h"
#include "AreaTrigger.h"
#include "Conversation.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Player.h"
#include "Unit.h"
#include "CreatureAI.h"
#include "Spell.h"

class Player;
//class Map;

namespace Trinity
{
    struct VisibleNotifier
    {
        Player &i_player;
        UpdateData i_data;
        std::set<Unit*> i_visibleNow;
        GuidUnorderedSet vis_guids;

        VisibleNotifier(Player &player) : i_player(player), i_data(player.GetMapId()), vis_guids(player.m_clientGUIDs) {}
        template<class T> void Visit(GridRefManager<T> &m);
        void Visit(PlayerMapType &);
        void SendToSelf(void);
    };

    struct VisibleChangesNotifier
    {
        WorldObject &i_object;

        explicit VisibleChangesNotifier(WorldObject &object) : i_object(object) {}
        template<class T> void Visit(GridRefManager<T> &) {}
        void Visit(PlayerMapType &);
        void Visit(CreatureMapType &);
        void Visit(DynamicObjectMapType &);
        void Visit(AreaTriggerMapType &);
        void Visit(ConversationMapType &);
    };

    struct PlayerRelocationNotifier : public VisibleNotifier
    {
        PlayerRelocationNotifier(Player &player) : VisibleNotifier(player) {}

        template<class T> void Visit(GridRefManager<T> &m) { VisibleNotifier::Visit(m); }
        void Visit(CreatureMapType &);
        void Visit(PlayerMapType &);
    };

    struct CreatureRelocationNotifier
    {
        Creature &i_creature;
        CreatureRelocationNotifier(Creature &c) : i_creature(c) {}
        template<class T> void Visit(GridRefManager<T> &) {}
        void Visit(CreatureMapType &);
        void Visit(PlayerMapType &);
    };

    struct DelayedUnitRelocation
    {
        Map &i_map;
        Cell &cell;
        CellCoord &p;
        const float i_radius;
        DelayedUnitRelocation(Cell &c, CellCoord &pair, Map &map, float radius) :
            i_map(map), cell(c), p(pair), i_radius(radius) {}
        template<class T> void Visit(GridRefManager<T> &) {}
        void Visit(CreatureMapType &);
        void Visit(PlayerMapType   &);
    };

    struct AIRelocationNotifier
    {
        Unit &i_unit;
        bool isCreature;
        explicit AIRelocationNotifier(Unit &unit) : i_unit(unit), isCreature(unit.GetTypeId() == TYPEID_UNIT)  {}
        template<class T> void Visit(GridRefManager<T> &) {}
        void Visit(CreatureMapType &);
    };

    struct GridUpdater
    {
        GridType &i_grid;
        uint32 i_timeDiff;
        GridUpdater(GridType &grid, uint32 diff) : i_grid(grid), i_timeDiff(diff) {}

        template<class T> void updateObjects(GridRefManager<T> &m)
        {
            for (typename GridRefManager<T>::iterator iter = m.begin(); iter != m.end(); ++iter)
                iter->getSource()->Update(i_timeDiff);
        }

        void Visit(PlayerMapType &m) { updateObjects<Player>(m); }
        void Visit(CreatureMapType &m){ updateObjects<Creature>(m); }
        void Visit(GameObjectMapType &m) { updateObjects<GameObject>(m); }
        void Visit(DynamicObjectMapType &m) { updateObjects<DynamicObject>(m); }
        void Visit(CorpseMapType &m) { updateObjects<Corpse>(m); }
        void Visit(AreaTriggerMapType &m) { updateObjects<AreaTrigger>(m); }
        void Visit(ConversationMapType &m) { updateObjects<Conversation>(m); }
    };

    struct MessageDistDeliverer
    {
        WorldObject* i_source;
        WorldPacket const* i_message;
        uint32 i_phaseMask;
        float i_distSq;
        uint32 team;
        Player const* skipped_receiver;
        MessageDistDeliverer(WorldObject* src, WorldPacket const* msg, float dist, bool own_team_only = false, Player const* skipped = NULL)
            : i_source(src), i_message(msg), i_phaseMask(src->GetPhaseMask()), i_distSq(dist * dist)
            , team((own_team_only && src->GetTypeId() == TYPEID_PLAYER) ? ((Player*)src)->GetTeam() : 0)
            , skipped_receiver(skipped)
        {
        }
        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(DynamicObjectMapType &m);
        template<class SKIP> void Visit(GridRefManager<SKIP> &) {}

        void SendPacket(Player* player)
        {
            // never send packet to self
            if (player == i_source || (team && player->GetTeam() != team) || skipped_receiver == player)
                return;

            if (!player->HaveAtClient(i_source))
                return;

            if (WorldSession* session = player->GetSession())
                session->SendPacket(i_message);
        }
    };

    struct ChatMessageDistDeliverer
    {
        WorldObject* i_source;
        uint32 i_phaseMask;
        float i_distSq;
        uint32 team;
        Player const* skipped_receiver;
        ChatData& i_c;
        WorldPacket* i_normalMessage;
        WorldPacket* i_codedMessage;
        WorldPacket* i_emptyMessage;
        ChatMessageDistDeliverer(WorldObject* src, ChatData& c, float dist, bool own_team_only = false, Player const* skipped = NULL)
            : i_source(src), i_phaseMask(src->GetPhaseMask()), i_distSq(dist * dist),
            team((own_team_only && src->GetTypeId() == TYPEID_PLAYER) ? ((Player*)src)->GetTeam() : 0),
            skipped_receiver(skipped), i_c(c), i_normalMessage(NULL), i_codedMessage(NULL), i_emptyMessage(NULL)
        {
        }

        ~ChatMessageDistDeliverer()
        {
            delete i_normalMessage;
            delete i_codedMessage;
            delete i_emptyMessage;
        }

        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(DynamicObjectMapType &m);
        template<class SKIP> void Visit(GridRefManager<SKIP> &) {}

        void SendPacket(Player* player);
    };

    struct ObjectUpdater
    {
        uint32 i_timeDiff;
        explicit ObjectUpdater(const uint32 diff) : i_timeDiff(diff) {}
        template<class T> void Visit(GridRefManager<T> &m);
        void Visit(PlayerMapType &) {}
        void Visit(CorpseMapType &) {}
        void Visit(CreatureMapType &);
    };

    // SEARCHERS & LIST SEARCHERS & WORKERS

    // WorldObject searchers & workers

    template<class Check>
    struct WorldObjectSearcher
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        WorldObject* &i_object;
        Check &i_check;

        WorldObjectSearcher(WorldObject const* searcher, WorldObject* & result, Check& check, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            : i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);
        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &m);
        void Visit(DynamicObjectMapType &m);
        void Visit(AreaTriggerMapType &m);
        void Visit(ConversationMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Check>
    struct WorldObjectLastSearcher
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        WorldObject* &i_object;
        Check &i_check;

        WorldObjectLastSearcher(WorldObject const* searcher, WorldObject* & result, Check& check, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            :  i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);
        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &m);
        void Visit(DynamicObjectMapType &m);
        void Visit(AreaTriggerMapType &m);
        void Visit(ConversationMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Check>
    struct WorldObjectListSearcher
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        std::list<WorldObject*> &i_objects;
        Check& i_check;

        WorldObjectListSearcher(WorldObject const* searcher, std::list<WorldObject*> &objects, Check & check, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            : i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &m);
        void Visit(GameObjectMapType &m);
        void Visit(DynamicObjectMapType &m);
        void Visit(AreaTriggerMapType &m);
        void Visit(ConversationMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Do>
    struct WorldObjectWorker
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        Do const& i_do;

        WorldObjectWorker(WorldObject const* searcher, Do const& _do, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            : i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_do(_do) {}

        void Visit(GameObjectMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_GAMEOBJECT))
                return;
            for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        void Visit(PlayerMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_PLAYER))
                return;
            for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }
        void Visit(CreatureMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CREATURE))
                return;
            for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        void Visit(CorpseMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CORPSE))
                return;
            for (CorpseMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        void Visit(DynamicObjectMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_DYNAMICOBJECT))
                return;
            for (DynamicObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        void Visit(AreaTriggerMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_AREATRIGGER))
                return;
            for (AreaTriggerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        void Visit(ConversationMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CONVERSATION))
                return;
            for (ConversationMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // Gameobject searchers

    template<class Check>
    struct GameObjectSearcher
    {
        uint32 i_phaseMask;
        GameObject* &i_object;
        Check &i_check;

        GameObjectSearcher(WorldObject const* searcher, GameObject* & result, Check& check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // Last accepted by Check GO if any (Check can change requirements at each call)
    template<class Check>
    struct GameObjectLastSearcher
    {
        uint32 i_phaseMask;
        GameObject* &i_object;
        Check& i_check;

        GameObjectLastSearcher(WorldObject const* searcher, GameObject* & result, Check& check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Check>
    struct GameObjectListSearcher
    {
        uint32 i_phaseMask;
        std::list<GameObject*> &i_objects;
        Check& i_check;

        GameObjectListSearcher(WorldObject const* searcher, std::list<GameObject*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(GameObjectMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Functor>
    struct GameObjectWorker
    {
        GameObjectWorker(WorldObject const* searcher, Functor& func)
            : _func(func), _phaseMask(searcher->GetPhaseMask()) {}

        void Visit(GameObjectMapType& m)
        {
            for (GameObjectMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(_phaseMask))
                    _func(itr->getSource());
        }

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}

    private:
        Functor& _func;
        uint32 _phaseMask;
    };

    // Unit searchers

    // First accepted by Check Unit if any
    template<class Check>
    struct UnitSearcher
    {
        uint32 i_phaseMask;
        Unit* &i_object;
        Check & i_check;

        UnitSearcher(WorldObject const* searcher, Unit* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);
        void Visit(PlayerMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // Last accepted by Check Unit if any (Check can change requirements at each call)
    template<class Check>
    struct UnitLastSearcher
    {
        uint32 i_phaseMask;
        Unit* &i_object;
        Check & i_check;

        UnitLastSearcher(WorldObject const* searcher, Unit* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);
        void Visit(PlayerMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // All accepted by Check units if any
    template<class Check>
    struct UnitListSearcher
    {
        uint32 i_phaseMask;
        std::list<Unit*> &i_objects;
        Check& i_check;

        UnitListSearcher(WorldObject const* searcher, std::list<Unit*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // All accepted by Check areatriggers if any
    template<class Check>
    struct AreaTriggerListSearcher
    {
        uint32 i_phaseMask;
        std::list<AreaTrigger*> &i_objects;
        Check& i_check;

        AreaTriggerListSearcher(WorldObject const* searcher, std::list<AreaTrigger*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(AreaTriggerMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // Creature searchers

    template<class Check>
    struct CreatureSearcher
    {
        uint32 i_phaseMask;
        Creature* &i_object;
        Check & i_check;

        CreatureSearcher(WorldObject const* searcher, Creature* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // Last accepted by Check Creature if any (Check can change requirements at each call)
    template<class Check>
    struct CreatureLastSearcher
    {
        uint32 i_phaseMask;
        Creature* &i_object;
        Check & i_check;

        CreatureLastSearcher(WorldObject const* searcher, Creature* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Check>
    struct CreatureListSearcher
    {
        uint32 i_phaseMask;
        std::list<Creature*> &i_objects;
        Check& i_check;

        CreatureListSearcher(WorldObject const* searcher, std::list<Creature*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(CreatureMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Do>
    struct CreatureWorker
    {
        uint32 i_phaseMask;
        Do& i_do;

        CreatureWorker(WorldObject const* searcher, Do& _do)
            : i_phaseMask(searcher->GetPhaseMask()), i_do(_do) {}

        void Visit(CreatureMapType &m)
        {
            for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // Player searchers

    template<class Check>
    struct PlayerSearcher
    {
        uint32 i_phaseMask;
        Player* &i_object;
        Check & i_check;

        PlayerSearcher(WorldObject const* searcher, Player* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(PlayerMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Check>
    struct PlayerListSearcher
    {
        uint32 i_phaseMask;
        std::list<Player*> &i_objects;
        Check& i_check;

        PlayerListSearcher(WorldObject const* searcher, std::list<Player*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(PlayerMapType &m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Check>
    struct PlayerLastSearcher
    {
        uint32 i_phaseMask;
        Player* &i_object;
        Check& i_check;

        PlayerLastSearcher(WorldObject const* searcher, Player*& result, Check& check) : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check)
        {
        }

        void Visit(PlayerMapType& m);

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Do>
    struct PlayerWorker
    {
        uint32 i_phaseMask;
        Do& i_do;

        PlayerWorker(WorldObject const* searcher, Do& _do)
            : i_phaseMask(searcher->GetPhaseMask()), i_do(_do) {}

        void Visit(PlayerMapType &m)
        {
            for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_phaseMask))
                    i_do(itr->getSource());
        }

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    template<class Do>
    struct PlayerDistWorker
    {
        WorldObject const* i_searcher;
        float i_dist;
        Do& i_do;

        PlayerDistWorker(WorldObject const* searcher, float _dist, Do& _do)
            : i_searcher(searcher), i_dist(_dist), i_do(_do) {}

        void Visit(PlayerMapType &m)
        {
            for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
                if (itr->getSource()->InSamePhase(i_searcher) && itr->getSource()->IsWithinDist(i_searcher, i_dist))
                    i_do(itr->getSource());
        }

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    // CHECKS && DO classes

    // WorldObject check classes

    class AnyDeadUnitObjectInRangeCheck
    {
        public:
            AnyDeadUnitObjectInRangeCheck(Unit* searchObj, float range) : i_searchObj(searchObj), i_range(range) {}
            bool operator()(Player* u);
            bool operator()(Corpse* u);
            bool operator()(Creature* u);
            template<class NOT_INTERESTED> bool operator()(NOT_INTERESTED*) { return false; }
        protected:
            Unit const* const i_searchObj;
            float i_range;
    };

    class AnyDeadUnitSpellTargetInRangeCheck : public AnyDeadUnitObjectInRangeCheck
    {
        public:
            AnyDeadUnitSpellTargetInRangeCheck(Unit* searchObj, float range, SpellInfo const* spellInfo, SpellTargetCheckTypes check)
                : AnyDeadUnitObjectInRangeCheck(searchObj, range), i_spellInfo(spellInfo), i_check(searchObj, searchObj, spellInfo, check, NULL)
            {}
            bool operator()(Player* u);
            bool operator()(Corpse* u);
            bool operator()(Creature* u);
            template<class NOT_INTERESTED> bool operator()(NOT_INTERESTED*) { return false; }
        protected:
            SpellInfo const* i_spellInfo;
            WorldObjectSpellTargetCheck i_check;
    };

    // WorldObject do classes

    class RespawnDo
    {
        public:
            RespawnDo() {}
            void operator()(Creature* u) const { u->Respawn(false, 2); }
            void operator()(GameObject* u) const { u->Respawn(); }
            void operator()(WorldObject*) const {}
            void operator()(Corpse*) const {}
    };

    // GameObject checks

    class GameObjectFocusCheck
    {
        public:
            GameObjectFocusCheck(Unit const* unit, uint32 focusId) : i_unit(unit), i_focusId(focusId) {}
            bool operator()(GameObject* go) const
            {
                // Time to hack: Anvil of the Thunder Forges
                if (i_focusId == 1813)
                {
                    if (go->GetGOInfo()->type != GAMEOBJECT_TYPE_GOOBER || go->GetGOInfo()->displayId != 10982)
                        return false;

                    float dist = go->GetGOInfo()->size * 2;

                    return go->IsWithinDistInMap(i_unit, dist);
                }

                if (go->GetGOInfo()->type != GAMEOBJECT_TYPE_SPELL_FOCUS)
                    return false;

                if (go->GetGOInfo()->spellFocus.spellFocusType != i_focusId)
                    return false;

                float dist = (float)((go->GetGOInfo()->spellFocus.radius));

                return go->IsWithinDistInMap(i_unit, dist);
            }
        private:
            Unit const* i_unit;
            uint32 i_focusId;
    };

    // Find the nearest Fishing hole and return true only if source object is in range of hole
    class NearestGameObjectFishingHole
    {
        public:
            NearestGameObjectFishingHole(WorldObject const& obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(GameObject* go)
            {
                if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_FISHINGHOLE && go->isSpawned() && i_obj.IsWithinDistInMap(go, i_range) && i_obj.IsWithinDistInMap(go, (float)go->GetGOInfo()->fishingHole.radius))
                {
                    i_range = i_obj.GetDistance(go);
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            float  i_range;

            // prevent clone
            NearestGameObjectFishingHole(NearestGameObjectFishingHole const&);
    };

    class NearestGameObjectCheck
    {
        public:
            NearestGameObjectCheck(WorldObject const& obj) : i_obj(obj), i_range(999) {}
            bool operator()(GameObject* go)
            {
                if (i_obj.IsWithinDistInMap(go, i_range))
                {
                    i_range = i_obj.GetDistance(go);        // use found GO range as new range limit for next check
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            float i_range;

            // prevent clone this object
            NearestGameObjectCheck(NearestGameObjectCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with GameobjectLastSearcher to find nearest GO)
    class NearestGameObjectEntryInObjectRangeCheck
    {
        public:
            NearestGameObjectEntryInObjectRangeCheck(WorldObject const& obj, uint32 entry, float range) : i_obj(obj), i_entry(entry), i_range(range) {}
            bool operator()(GameObject* go)
            {
                if (go->GetEntry() == i_entry && i_obj.IsWithinDistInMap(go, i_range))
                {
                    i_range = i_obj.GetDistance(go);        // use found GO range as new range limit for next check
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            uint32 i_entry;
            float  i_range;

            // prevent clone this object
            NearestGameObjectEntryInObjectRangeCheck(NearestGameObjectEntryInObjectRangeCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with GameobjectLastSearcher to find nearest GO with a certain type)
    class NearestGameObjectTypeInObjectRangeCheck
    {
    public:
        NearestGameObjectTypeInObjectRangeCheck(WorldObject const& obj, GameobjectTypes type, float range) : i_obj(obj), i_type(type), i_range(range) {}
        bool operator()(GameObject* go)
        {
            if (go->GetGoType() == i_type && i_obj.IsWithinDistInMap(go, i_range))
            {
                i_range = i_obj.GetDistance(go);        // use found GO range as new range limit for next check
                return true;
            }
            return false;
        }
        float GetLastRange() const { return i_range; }
    private:
        WorldObject const& i_obj;
        GameobjectTypes i_type;
        float  i_range;

        // prevent clone this object
        NearestGameObjectTypeInObjectRangeCheck(NearestGameObjectTypeInObjectRangeCheck const&);

    };

    class GameObjectWithDbGUIDCheck
    {
        public:
            GameObjectWithDbGUIDCheck(WorldObject const& obj, ObjectGuid::LowType db_guid) : i_obj(obj), i_db_guid(db_guid) {}
            bool operator()(GameObject const* go) const
            {
                return go->GetDBTableGUIDLow() == i_db_guid;
            }
        private:
            WorldObject const& i_obj;
            ObjectGuid::LowType i_db_guid;
    };

    // Unit checks

    class MostHPMissingInRange
    {
        public:
            MostHPMissingInRange(Unit const* obj, float range, uint32 hp) : i_obj(obj), i_range(range), i_hp(hp) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && u->isInCombat() && !i_obj->IsHostileTo(u) && i_obj->IsWithinDistInMap(u, i_range) && u->GetMaxHealth() - u->GetHealth() > i_hp)
                {
                    i_hp = u->GetMaxHealth() - u->GetHealth();
                    return true;
                }
                return false;
            }
        private:
            Unit const* i_obj;
            float i_range;
            uint32 i_hp;
    };

    class FriendlyCCedInRange
    {
        public:
            FriendlyCCedInRange(Unit const* obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && u->isInCombat() && !i_obj->IsHostileTo(u) && i_obj->IsWithinDistInMap(u, i_range) &&
                    (u->isFeared() || u->isCharmed() || u->isFrozen() || u->HasUnitState(UNIT_STATE_STUNNED) || u->HasUnitState(UNIT_STATE_CONFUSED)))
                {
                    return true;
                }
                return false;
            }
        private:
            Unit const* i_obj;
            float i_range;
    };

    class FriendlyMissingBuffInRange
    {
        public:
            FriendlyMissingBuffInRange(Unit const* obj, float range, uint32 spellid) : i_obj(obj), i_range(range), i_spell(spellid) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && u->isInCombat() && !i_obj->IsHostileTo(u) && i_obj->IsWithinDistInMap(u, i_range) &&
                    !(u->HasAura(i_spell)))
                {
                    return true;
                }
                return false;
            }
        private:
            Unit const* i_obj;
            float i_range;
            uint32 i_spell;
    };

    class AnyUnfriendlyUnitInObjectRangeCheck
    {
        public:
            AnyUnfriendlyUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && i_obj->IsWithinDistInMap(u, i_range) && !i_funit->IsFriendlyTo(u))
                    return true;
                else
                    return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
    };

    class AnyUnfriendlyNoTotemUnitInObjectRangeCheck
    {
        public:
            AnyUnfriendlyNoTotemUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (!u->isAlive())
                    return false;

                if (u->GetCreatureType() == CREATURE_TYPE_NON_COMBAT_PET)
                    return false;

                if (u->GetTypeId() == TYPEID_UNIT && ((Creature*)u)->isTotem())
                    return false;

                if (!u->isTargetableForAttack(false))
                    return false;

                return i_obj->IsWithinDistInMap(u, i_range) && !i_funit->IsFriendlyTo(u);
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
    };

    class AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck
    {
        public:
            AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck(Unit const* funit, float range)
                : i_funit(funit), i_range(range) {}

            bool operator()(const Unit* u)
            {
                return u->isAlive()
                    && i_funit->IsWithinDistInMap(u, i_range)
                    && !i_funit->IsFriendlyTo(u)
                    && i_funit->IsValidAttackTarget(u)
                    && u->GetCreatureType() != CREATURE_TYPE_CRITTER
                    && i_funit->canSeeOrDetect(u);
            }
        private:
            Unit const* i_funit;
            float i_range;
    };

    class CreatureWithDbGUIDCheck
    {
        public:
            CreatureWithDbGUIDCheck(WorldObject const* obj, ObjectGuid::LowType const& lowguid) : i_obj(obj), i_lowguid(lowguid) {}
            bool operator()(Creature* u)
            {
                return u->GetDBTableGUIDLow() == i_lowguid;
            }
        private:
            WorldObject const* i_obj;
            ObjectGuid::LowType i_lowguid;
    };

    class AnyFriendlyUnitInObjectRangeCheck
    {
        public:
            AnyFriendlyUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && i_obj->IsWithinDistInMap(u, i_range) && i_funit->IsFriendlyTo(u))
                    return true;
                else
                    return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
    };

    class AnyUnitHavingBuffInObjectRangeCheck
    {
        public:
            AnyUnitHavingBuffInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range, uint32 spellid, bool isfriendly)
                : i_obj(obj), i_funit(funit), i_range(range), i_spellid(spellid), i_friendly(isfriendly) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && i_obj->IsWithinDistInMap(u, i_range) && i_funit->IsFriendlyTo(u) == i_friendly && u->HasAura(i_spellid, i_obj->GetGUID()))
                    return true;
                else
                    return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
            uint32 i_spellid;
            bool i_friendly;
    };

    class AnyUnitHavingAuraTypeInObjectRangeCheck
    {
        public:
            AnyUnitHavingAuraTypeInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range, AuraType type, bool isfriendly)
                : i_obj(obj), i_funit(funit), i_range(range), i_type(type), i_friendly(isfriendly) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && i_obj->IsWithinDistInMap(u, i_range) && i_funit->IsFriendlyTo(u) == i_friendly && u->HasAuraTypeWithCaster(i_type, i_obj->GetGUID()))
                    return true;
                else
                    return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
            AuraType i_type;
            bool i_friendly;
    };

    class AnyGroupedUnitInObjectRangeCheck
    {
        public:
            AnyGroupedUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range, bool raid) : _source(obj), _refUnit(funit), _range(range), _raid(raid) {}
            bool operator()(Unit* u)
            {
                if (_raid)
                {
                    if (!_refUnit->IsInRaidWith(u))
                        return false;
                }
                else if (!_refUnit->IsInPartyWith(u))
                    return false;

                return !_refUnit->IsHostileTo(u) && u->isAlive() && _source->IsWithinDistInMap(u, _range);
            }

        private:
            WorldObject const* _source;
            Unit const* _refUnit;
            float _range;
            bool _raid;
    };

    class AnyUnitInObjectRangeCheck
    {
        public:
            AnyUnitInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isAlive() && i_obj->IsWithinDistInMap(u, i_range) && !u->isTotem())
                    return true;

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;
    };

    class AnyAreaTriggerInObjectRangeCheck
    {
        public:
            AnyAreaTriggerInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(AreaTrigger* at)
            {
                if (i_obj->IsWithinDistInMap(at, i_range))
                    return true;

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;
    };

    class AreaTriggerWithEntryInObjectRangeCheck
    {
        public:
            AreaTriggerWithEntryInObjectRangeCheck(WorldObject const* obj, uint32 entry, ObjectGuid casterGuid, float range) : i_obj(obj), i_entry(entry), i_casterGuid(casterGuid), i_range(range) {}
            bool operator()(AreaTrigger* at)
            {
                if (i_obj->IsWithinDistInMap(at, i_range) && i_entry == at->GetEntry() && (i_casterGuid.IsEmpty() || i_casterGuid == at->GetCasterGUID()))
                    return true;

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;
            uint32 i_entry;
            ObjectGuid i_casterGuid;
    };

    // Success at unit in range, range update for next check (this can be use with UnitLastSearcher to find nearest unit)
    class NearestAttackableUnitInObjectRangeCheck
    {
        public:
            NearestAttackableUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isTargetableForAttack() && i_obj->IsWithinDistInMap(u, i_range) &&
                    !i_funit->IsFriendlyTo(u) && i_funit->canSeeOrDetect(u))
                {
                    i_range = i_obj->GetDistance(u);        // use found unit range as new range limit for next check
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;

            // prevent clone this object
            NearestAttackableUnitInObjectRangeCheck(NearestAttackableUnitInObjectRangeCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with UnitLastSearcher to find nearest unit)
    class NearestAttackableNoCCUnitInObjectRangeCheck
    {
        public:
            NearestAttackableNoCCUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isTargetableForAttack() && i_obj->IsWithinDistInMap(u, i_range) &&
                    !i_funit->IsFriendlyTo(u) && !u->HasCrowdControlAura() && !u->HasAuraType(SPELL_AURA_MOD_CONFUSE))
                {
                    i_range = i_obj->GetDistance(u);        // use found unit range as new range limit for next check
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;

            // prevent clone this object
            NearestAttackableNoCCUnitInObjectRangeCheck(NearestAttackableNoCCUnitInObjectRangeCheck const&);
    };

    class AnyAoETargetUnitInObjectRangeCheck
    {
        public:
            AnyAoETargetUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range)
                : i_obj(obj), i_funit(funit), _spellInfo(NULL), i_range(range)
            {
                Unit const* check = i_funit;
                Unit const* owner = i_funit->GetOwner();
                if (owner)
                    check = owner;
                i_targetForPlayer = (check->GetTypeId() == TYPEID_PLAYER);
                if (i_obj->GetTypeId() == TYPEID_DYNAMICOBJECT)
                    _spellInfo = sSpellMgr->GetSpellInfo(((DynamicObject*)i_obj)->GetSpellId());
            }
            bool operator()(Unit* u)
            {
                // Check contains checks for: live, non-selectable, non-attackable flags, flight check and GM check, ignore totems
                if (u->GetTypeId() == TYPEID_UNIT && ((Creature*)u)->isTotem())
                    return false;

                if (i_funit->_IsValidAttackTarget(u, _spellInfo,i_obj->GetTypeId() == TYPEID_DYNAMICOBJECT ? i_obj : NULL) && i_obj->IsWithinDistInMap(u, i_range))
                    return true;

                return false;
            }
        private:
            bool i_targetForPlayer;
            WorldObject const* i_obj;
            Unit const* i_funit;
            SpellInfo const* _spellInfo;
            float i_range;
    };

    // do attack at call of help to friendly crearture
    class CallOfHelpCreatureInRangeDo
    {
        public:
            CallOfHelpCreatureInRangeDo(Unit* funit, Unit* enemy, float range)
                : i_funit(funit), i_enemy(enemy), i_range(range)
            {}
            void operator()(Creature* u)
            {
                if (u == i_funit)
                    return;

                if (!u->CanAssistTo(i_funit, i_enemy, false))
                    return;

                // too far
                if (!u->IsWithinDistInMap(i_funit, i_range))
                    return;

                // only if see assisted creature's enemy
                if (!u->IsWithinLOSInMap(i_enemy))
                    return;

                if (u->AI())
                    u->AI()->AttackStart(i_enemy);
            }
        private:
            Unit* const i_funit;
            Unit* const i_enemy;
            float i_range;
    };

    // Creature checks
    class NearestHostileUnitCheck
    {
        public:
            explicit NearestHostileUnitCheck(Creature const* creature, float dist = 0) : me(creature)
            {
                m_range = (dist == 0 ? 9999 : dist);
            }
            bool operator()(Unit* u)
            {
                if (!me->IsWithinDistInMap(u, m_range))
                    return false;

                if (!me->IsValidAttackTarget(u))
                    return false;

                m_range = me->GetDistance(u);   // use found unit range as new range limit for next check
                return true;
            }

    private:
            Creature const* me;
            float m_range;
            NearestHostileUnitCheck(NearestHostileUnitCheck const&);
    };

    class NearestHostileNoCCUnitCheck
    {
        public:
            explicit NearestHostileNoCCUnitCheck(Creature const* creature, float dist = 0) : me(creature)
            {
                m_range = (dist == 0 ? 9999 : dist);
            }
            bool operator()(Unit* u)
            {
                if (!me->IsWithinDistInMap(u, m_range))
                    return false;

                if (!me->canSeeOrDetect(u))
                    return false;

                if (u->HasCrowdControlAura())
                    return false;

                if (u->HasAuraType(SPELL_AURA_MOD_CONFUSE))
                    return false;

                m_range = me->GetDistance(u);   // use found unit range as new range limit for next check
                return true;
            }

        private:
            Creature const *me;
            float m_range;
            NearestHostileNoCCUnitCheck(NearestHostileNoCCUnitCheck const&);
    };

    class NearestHostileUnitInAttackDistanceCheck
    {
        public:
            explicit NearestHostileUnitInAttackDistanceCheck(Creature const* creature, float dist = 0) : me(creature)
            {
                m_range = (dist == 0 ? 9999 : dist);
                m_force = (dist == 0 ? false : true);
            }
            bool operator()(Unit* u)
            {
                if (!me->IsWithinDistInMap(u, m_range))
                    return false;

                if (!me->canSeeOrDetect(u))
                    return false;

                if (m_force)
                {
                    if (!me->IsValidAttackTarget(u))
                        return false;
                }
                else if (!me->canStartAttack(u, false))
                    return false;

                m_range = me->GetDistance(u);   // use found unit range as new range limit for next check
                return true;
            }
            float GetLastRange() const { return m_range; }
        private:
            Creature const* me;
            float m_range;
            bool m_force;
            NearestHostileUnitInAttackDistanceCheck(NearestHostileUnitInAttackDistanceCheck const&);
    };

    class AnyAssistCreatureInRangeCheck
    {
        public:
            AnyAssistCreatureInRangeCheck(Unit* funit, Unit* enemy, float range)
                : i_funit(funit), i_enemy(enemy), i_range(range)
            {
            }
            bool operator()(Creature* u)
            {
                if (u == i_funit)
                    return false;

                if (!u->CanAssistTo(i_funit, i_enemy))
                    return false;

                // too far
                if (!i_funit->IsWithinDistInMap(u, i_range))
                    return false;

                // only if see assisted creature
                if (!i_funit->IsWithinLOSInMap(u))
                    return false;

                return true;
            }
        private:
            Unit* const i_funit;
            Unit* const i_enemy;
            float i_range;
    };

    class NearestAssistCreatureInCreatureRangeCheck
    {
        public:
            NearestAssistCreatureInCreatureRangeCheck(Creature* obj, Unit* enemy, float range)
                : i_obj(obj), i_enemy(enemy), i_range(range) {}

            bool operator()(Creature* u)
            {
                if (u == i_obj)
                    return false;
                if (!u->CanAssistTo(i_obj, i_enemy))
                    return false;

                if (!i_obj->IsWithinDistInMap(u, i_range))
                    return false;

                if (!i_obj->IsWithinLOSInMap(u))
                    return false;

                i_range = i_obj->GetDistance(u);            // use found unit range as new range limit for next check
                return true;
            }
            float GetLastRange() const { return i_range; }
        private:
            Creature* const i_obj;
            Unit* const i_enemy;
            float  i_range;

            // prevent clone this object
            NearestAssistCreatureInCreatureRangeCheck(NearestAssistCreatureInCreatureRangeCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with CreatureLastSearcher to find nearest creature)
    class NearestCreatureEntryWithLiveStateInObjectRangeCheck
    {
        public:
            NearestCreatureEntryWithLiveStateInObjectRangeCheck(WorldObject const& obj, uint32 entry, bool alive, float range)
                : i_obj(obj), i_entry(entry), i_alive(alive), i_range(range) {}

            bool operator()(Creature* u)
            {
                if (u->GetEntry() == i_entry && u->isAlive() == i_alive && i_obj.IsWithinDistInMap(u, i_range))
                {
                    i_range = i_obj.GetDistance(u);         // use found unit range as new range limit for next check
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            uint32 i_entry;
            bool   i_alive;
            float  i_range;

            // prevent clone this object
            NearestCreatureEntryWithLiveStateInObjectRangeCheck(NearestCreatureEntryWithLiveStateInObjectRangeCheck const&);
    };

    class AnyPlayerInObjectRangeCheck
    {
        public:
            AnyPlayerInObjectRangeCheck(WorldObject const* obj, float range, bool reqAlive = true) : _obj(obj), _range(range), _reqAlive(reqAlive) {}
            bool operator()(Player* u)
            {
                if (_reqAlive && !u->isAlive())
                    return false;

                if (!_obj->IsWithinDistInMap(u, _range))
                    return false;

                return true;
            }

        private:
            WorldObject const* _obj;
            float _range;
            bool _reqAlive;
    };

    class NearestPlayerInObjectRangeCheck
    {
        public:
            NearestPlayerInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range)
            {
            }

            bool operator()(Player* u)
            {
                if (u->isAlive() && i_obj->IsWithinDistInMap(u, i_range))
                {
                    i_range = i_obj->GetDistance(u);
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;

            NearestPlayerInObjectRangeCheck(NearestPlayerInObjectRangeCheck const&);
    };

    class NearestPlayerNotGMInObjectRangeCheck
    {
        public:
            NearestPlayerNotGMInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range)
            {
            }

            bool operator()(Player* u)
            {
                if (!u->isGameMaster() && u->isAlive() && i_obj->IsWithinDistInMap(u, i_range))
                {
                    i_range = i_obj->GetDistance(u);
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;

            NearestPlayerNotGMInObjectRangeCheck(NearestPlayerNotGMInObjectRangeCheck const&);
    };

    class AllFriendlyCreaturesInGrid
    {
    public:
        AllFriendlyCreaturesInGrid(Unit const* obj) : unit(obj) {}
        bool operator() (Unit* u)
        {
            if (u->isAlive() && u->IsVisible() && u->IsFriendlyTo(unit))
                return true;

            return false;
        }
    private:
        Unit const* unit;
    };

    class AllGameObjectsWithEntryInRange
    {
    public:
        AllGameObjectsWithEntryInRange(const WorldObject* object, uint32 entry, float maxRange) : m_pObject(object), m_uiEntry(entry), m_fRange(maxRange) {}
        bool operator() (GameObject* go)
        {
            if (go->GetEntry() == m_uiEntry && m_pObject->IsWithinDist(go, m_fRange, false))
                return true;

            return false;
        }
    private:
        const WorldObject* m_pObject;
        uint32 m_uiEntry;
        float m_fRange;
    };

    class AllCreaturesOfEntryInRange
    {
        public:
            AllCreaturesOfEntryInRange(const WorldObject* object, uint32 entry, float maxRange) : m_pObject(object), m_uiEntry(entry), m_fRange(maxRange) {}
            bool operator() (Unit* unit)
            {
                if (unit->GetEntry() == m_uiEntry && m_pObject->IsWithinDist(unit, m_fRange, false))
                    return true;

                return false;
            }

        private:
            const WorldObject* m_pObject;
            uint32 m_uiEntry;
            float m_fRange;
    };

    class AllAreaTriggeresOfEntryInRange
    {
        public:
            AllAreaTriggeresOfEntryInRange(const WorldObject* object, uint32 entry, float maxRange) : m_pObject(object), m_uiEntry(entry), m_fRange(maxRange) {}
            bool operator() (AreaTrigger* at)
            {
                if (at->GetEntry() == m_uiEntry && m_pObject->IsWithinDist(at, m_fRange, false))
                    return true;

                return false;
            }

        private:
            const WorldObject* m_pObject;
            uint32 m_uiEntry;
            float m_fRange;
    };

    class AllAliveCreaturesOfEntryInRange
    {
        public:
            AllAliveCreaturesOfEntryInRange(const WorldObject* object, uint32 entry, float maxRange) : m_pObject(object), m_uiEntry(entry), m_fRange(maxRange) {}
            bool operator() (Unit* unit)
            {
                if (unit->GetEntry() == m_uiEntry && m_pObject->IsWithinDist(unit, m_fRange, false) && unit->isAlive())
                    return true;

                return false;
            }

        private:
            const WorldObject* m_pObject;
            uint32 m_uiEntry;
            float m_fRange;
    };

    class SearchCorpseCreatureCheck
    {
        public:
            SearchCorpseCreatureCheck(const WorldObject* object, float range) : m_pObject(object), i_range(range), m_owner(NULL)
            {
                m_owner = const_cast<Player*>(m_pObject->ToPlayer());
                if (!m_owner)
                {
                    if (const Creature *c = m_pObject->ToCreature())
                        if (const Unit* owner = c->GetOwner())
                            m_owner = const_cast<Player*>(owner->ToPlayer());
                }
            }
            bool operator()(Creature* u)
            {
                if(!m_owner)
                    return false;

                if (u->getDeathState() != CORPSE || !m_owner->isAllowedToLoot(u) || !u->HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE))
                    return false;

                return m_pObject->IsWithinDistInMap(u, i_range);
            }
        private:
            Player* m_owner;
            const WorldObject* m_pObject;
            float i_range;
    };

    class PlayerAtMinimumRangeAway
    {
    public:
        PlayerAtMinimumRangeAway(Unit const* unit, float fMinRange) : unit(unit), fRange(fMinRange) {}
        bool operator() (Player* player)
        {
            //No threat list check, must be done explicit if expected to be in combat with creature
            if (!player->isGameMaster() && player->isAlive() && !unit->IsWithinDist(player, fRange, false))
                return true;

            return false;
        }

    private:
        Unit const* unit;
        float fRange;
    };

    class GameObjectInRangeCheck
    {
    public:
        GameObjectInRangeCheck(float _x, float _y, float _z, float _range, uint32 _entry = 0) :
          x(_x), y(_y), z(_z), range(_range), entry(_entry) {}
        bool operator() (GameObject* go)
        {
            if (!entry || (go->GetGOInfo() && go->GetGOInfo()->entry == entry))
                return go->IsInRange(x, y, z, range);
            else return false;
        }
    private:
        float x, y, z, range;
        uint32 entry;
    };

    class AllWorldObjectsInRange
    {
    public:
        AllWorldObjectsInRange(const WorldObject* object, float maxRange) : m_pObject(object), m_fRange(maxRange) {}
        bool operator() (WorldObject* go)
        {
            return m_pObject->IsWithinDist(go, m_fRange, false) && m_pObject->InSamePhase(go);
        }
    private:
        const WorldObject* m_pObject;
        float m_fRange;
    };

    class ObjectTypeIdCheck
    {
        public:
            ObjectTypeIdCheck(TypeID typeId, bool equals) : _typeId(typeId), _equals(equals) {}
            bool operator()(WorldObject* object)
            {
                return (object->GetTypeId() == _typeId) == _equals;
            }

        private:
            TypeID _typeId;
            bool _equals;
    };

    class ObjectGUIDCheck
    {
        public:
            ObjectGUIDCheck(ObjectGuid GUID) : _GUID(GUID) {}
            bool operator()(WorldObject* object)
            {
                return object->GetGUID() == _GUID;
            }

        private:
            ObjectGuid _GUID;
    };

    class UnitAuraCheck
    {
        public:
            UnitAuraCheck(bool present, uint32 spellId, ObjectGuid casterGUID = ObjectGuid::Empty) : _present(present), _spellId(spellId), _casterGUID(casterGUID) {}
            bool operator()(Unit* unit) const
            {
                return unit->HasAura(_spellId, _casterGUID) == _present;
            }

            bool operator()(WorldObject* object) const
            {
                return object->ToUnit() && object->ToUnit()->HasAura(_spellId, _casterGUID) == _present;
            }

        private:
            bool _present;
            uint32 _spellId;
            ObjectGuid _casterGUID;
    };

    class UnitAuraTypeCheck
    {
        public:
            UnitAuraTypeCheck(bool present, AuraType type) : _present(present), _type(type) {}
            bool operator()(Unit* unit) const
            {
                return unit->HasAuraType(_type) == _present;
            }

            bool operator()(WorldObject* object) const
            {
                return object->ToUnit() && object->ToUnit()->HasAuraType(_type) == _present;
            }

        private:
            bool _present;
            AuraType _type;
    };

    class MailBoxMasterCheck
    {
        public:
            MailBoxMasterCheck(Player* plr) : _plr(plr) {}
            bool operator()(GameObject* u)
            {
                if (!_plr->IsInWorld())
                    return false;

                if (_plr->isInFlight())
                    return false;

                if (!u->IsWithinDistInMap(_plr, INTERACTION_DISTANCE))
                    return false;

                return true;
            }

        private:
            Player* _plr;
    };

    // Player checks and do

    // Prepare using Builder localized packets with caching and send to player
    template<class Builder>
    class LocalizedPacketDo
    {
        public:
            explicit LocalizedPacketDo(Builder& builder) : i_builder(builder) { }

            ~LocalizedPacketDo()
            {
                for (size_t i = 0; i < i_data_cache.size(); ++i)
                    delete i_data_cache[i];
            }
            void operator()(Player* p);

        private:
            Builder& i_builder;
            std::vector<WorldPacket*> i_data_cache;         // 0 = default, i => i-1 locale index
    };

    template<class Builder>
    class LocalizedPacketDoNew
    {
        public:
            explicit LocalizedPacketDoNew(Builder& builder) : i_builder(builder) { }

            ~LocalizedPacketDoNew()
            {
                for (size_t i = 0; i < i_data_cache.size(); ++i)
                    delete i_data_cache[i];
            }
            void operator()(Player* p);

        private:
            Builder& i_builder;
            std::vector<WorldPackets::Packet*> i_data_cache; // 0 = default, i => i-1 locale index
    };

    // Prepare using Builder localized packets with caching and send to player
    template<class Builder>
    class LocalizedPacketListDo
    {
        public:
            typedef std::vector<WorldPacket*> WorldPacketList;
            explicit LocalizedPacketListDo(Builder& builder) : i_builder(builder) {}

            ~LocalizedPacketListDo()
            {
                for (size_t i = 0; i < i_data_cache.size(); ++i)
                    for (size_t j = 0; j < i_data_cache[i].size(); ++j)
                        delete i_data_cache[i][j];
            }

            void operator()(Player* p);

        private:
            Builder& i_builder;
            std::vector<WorldPacketList> i_data_cache; // 0 = default, i => i-1 locale index
    };

    template<class Builder>
    class LocalizedPacketListDoNew
    {
        public:
            typedef std::vector<WorldPackets::Packet*> WorldPacketList;
            explicit LocalizedPacketListDoNew(Builder& builder) : i_builder(builder) { }

            ~LocalizedPacketListDoNew()
            {
                for (size_t i = 0; i < i_data_cache.size(); ++i)
                    for (size_t j = 0; j < i_data_cache[i].size(); ++j)
                        delete i_data_cache[i][j];
            }

            void operator()(Player* p);

        private:
            Builder& i_builder;
            std::vector<WorldPacketList> i_data_cache; // 0 = default, i => i-1 locale index
    };
    class UnitHealthState
    {
        public:
            UnitHealthState(bool sortlow) : _sortlow(sortlow) {}
            bool operator()(Unit* unitA, Unit* unitB) const
            {
                return (unitA->GetHealthPct() < unitB->GetHealthPct()) == _sortlow;
            }

            bool operator()(WorldObject* objectA, WorldObject* objectB) const
            {
                return objectA->ToUnit() && objectB->ToUnit() && (objectA->ToUnit()->GetHealthPct() < objectB->ToUnit()->GetHealthPct()) == _sortlow;
            }

        private:
            bool _sortlow;
    };

    class UnitDistanceCheck
    {
        public:
            UnitDistanceCheck(bool checkin, Unit* caster, float dist) : _checkin(checkin), _caster(caster), _dist(dist) {}
            bool operator()(Unit* unit) const
            {
                return (_caster->GetExactDist2d(unit) > _dist) == _checkin;
            }

            bool operator()(WorldObject* object) const
            {
                return (_caster->GetExactDist2d(object) > _dist) == _checkin;
            }

        private:
            bool _checkin;
            Unit* _caster;
            float _dist;
    };

    class UnitTypeCheck
    {
        public:
            UnitTypeCheck(bool checkin, uint32 typeMask) : _checkin(checkin), _typeMask(typeMask) {}
            bool operator()(Unit* unit) const
            {
                return bool(_typeMask & (1 << unit->GetTypeId())) == _checkin;
            }

            bool operator()(WorldObject* object) const
            {
                return bool(_typeMask & (1 << object->GetTypeId())) == _checkin;
            }

        private:
            bool _checkin;
            uint32 _typeMask;
    };

    class UnitSortDistance
    {
        public:
            UnitSortDistance(bool sortlow, Unit* caster) : _sortlow(sortlow), _caster(caster) {}
            bool operator()(Unit* unitA, Unit* unitB) const
            {
                return (_caster->GetExactDist2d(unitA) < _caster->GetExactDist2d(unitB)) == _sortlow;
            }

            bool operator()(WorldObject* objectA, WorldObject* objectB) const
            {
                return (_caster->GetExactDist2d(objectA) < _caster->GetExactDist2d(objectB)) == _sortlow;
            }

        private:
            bool _sortlow;
            Unit* _caster;
    };

    class UnitFriendlyCheck
    {
        public:
            UnitFriendlyCheck(bool present, Unit* caster) : _present(present), _caster(caster) {}
            bool operator() (Unit* unit)
            {
                return unit->IsFriendlyTo(_caster) == _present;
            }
            bool operator() (WorldObject* object)
            {
                return object->ToUnit() && object->ToUnit()->IsFriendlyTo(_caster) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
    };

    class UnitRaidCheck
    {
        public:
            UnitRaidCheck(bool present, Unit* caster) : _present(present), _caster(caster) {}
            bool operator() (Unit* unit)
            {
                return unit->IsInRaidWith(_caster) == _present;
            }
            bool operator() (WorldObject* object)
            {
                return object->ToUnit() && object->ToUnit()->IsInRaidWith(_caster) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
    };

    class UnitPartyCheck
    {
        public:
            UnitPartyCheck(bool present, Unit* caster) : _present(present), _caster(caster) {}
            bool operator() (Unit* unit)
            {
                return unit->IsInPartyWith(_caster) == _present;
            }
            bool operator() (WorldObject* object)
            {
                return object->ToUnit() && object->ToUnit()->IsInPartyWith(_caster) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
    };

    class UnitCheckInLos
    {
        public:
            UnitCheckInLos(bool present, Unit* caster) : _present(present), _caster(caster) {}
            bool operator() (WorldObject* object)
            {
                return object->IsWithinLOSInMap(_caster) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
    };

    class UnitCheckInBetween
    {
        public:
            UnitCheckInBetween(bool present, Unit* caster, Unit* target, float size) : _present(present), _caster(caster), _target(target), _size(size) {}
            bool operator() (WorldObject* object)
            {
                return object->IsInBetween(_caster, _target, _size) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
            Unit* _target;
            float _size;
    };

    class UnitCheckInBetweenShift
    {
        public:
            UnitCheckInBetweenShift(bool present, Unit* caster, Unit* target, float size, float shift, float angleShift)
            : _present(present), _caster(caster), _target(target), _size(size), _shift(shift), _angleShift(angleShift) {}

            bool operator() (WorldObject* object)
            {
                return object->IsInBetweenShift(_caster, _target, _size, _shift, _angleShift) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
            Unit* _target;
            float _size;
            float _shift;
            float _angleShift;
    };

    class UnitCheckCCAura
    {
        public:
            UnitCheckCCAura(bool present, Unit* caster) : _present(present), _caster(caster) {}
            bool operator() (WorldObject* object)
            {
                return object->ToUnit() && object->ToUnit()->HasCrowdControlAura(_caster) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
    };

    class UnitAuraAndCheck
    {
        public:
            UnitAuraAndCheck(int32 aura1, int32 aura2 = 0, int32 aura3 = 0, ObjectGuid casterGUID = ObjectGuid::Empty) : _aura1(aura1), _aura2(aura2), _aura3(aura3), _casterGUID(casterGUID) {}
            bool operator()(Unit* unit) const
            {
                if(_aura1 < 0 && unit->HasAura(abs(_aura1), _casterGUID))
                    return true;
                else if(_aura1 > 0 && !unit->HasAura(_aura1, _casterGUID))
                    return true;

                if(_aura2 < 0 && unit->HasAura(abs(_aura2), _casterGUID))
                    return true;
                else if(_aura2 > 0 && !unit->HasAura(_aura2, _casterGUID))
                    return true;

                if(_aura3 < 0 && unit->HasAura(abs(_aura3), _casterGUID))
                    return true;
                else if(_aura3 > 0 && !unit->HasAura(_aura3, _casterGUID))
                    return true;

                return false;
            }

            bool operator()(WorldObject* object) const
            {
                if(Unit* unit = object->ToUnit())
                {
                    if(_aura1 < 0 && unit->HasAura(abs(_aura1), _casterGUID))
                        return true;
                    else if(_aura1 > 0 && !unit->HasAura(_aura1, _casterGUID))
                        return true;

                    if(_aura2 < 0 && unit->HasAura(abs(_aura2), _casterGUID))
                        return true;
                    else if(_aura2 > 0 && !unit->HasAura(_aura2, _casterGUID))
                        return true;

                    if(_aura3 < 0 && unit->HasAura(abs(_aura3), _casterGUID))
                        return true;
                    else if(_aura3 > 0 && !unit->HasAura(_aura3, _casterGUID))
                        return true;

                    return false;
                }
                else
                    return true;
            }

        private:
            int32 _aura1;
            int32 _aura2;
            int32 _aura3;
            ObjectGuid _casterGUID;
    };

    class UnitAuraOrCheck
    {
        public:
            UnitAuraOrCheck(int32 aura1, int32 aura2, int32 aura3, ObjectGuid casterGUID = ObjectGuid::Empty) : _aura1(aura1), _aura2(aura2), _aura3(aura3), _casterGUID(casterGUID) {}
            bool operator()(Unit* unit) const
            {
                if(_aura1 < 0 && !unit->HasAura(abs(_aura1), _casterGUID))
                    return false;
                else if(_aura1 > 0 && unit->HasAura(_aura1, _casterGUID))
                    return false;

                if(_aura2 < 0 && !unit->HasAura(abs(_aura2), _casterGUID))
                    return false;
                else if(_aura2 > 0 && unit->HasAura(_aura2, _casterGUID))
                    return false;

                if(_aura3 < 0 && !unit->HasAura(abs(_aura3), _casterGUID))
                    return false;
                else if(_aura3 > 0 && unit->HasAura(_aura3, _casterGUID))
                    return false;

                return true;
            }

            bool operator()(WorldObject* object) const
            {
                if(Unit* unit = object->ToUnit())
                {
                    if(_aura1 < 0 && !unit->HasAura(abs(_aura1), _casterGUID))
                        return false;
                    else if(_aura1 > 0 && unit->HasAura(_aura1, _casterGUID))
                        return false;

                    if(_aura2 < 0 && !unit->HasAura(abs(_aura2), _casterGUID))
                        return false;
                    else if(_aura2 > 0 && unit->HasAura(_aura2, _casterGUID))
                        return false;

                    if(_aura3 < 0 && !unit->HasAura(abs(_aura3), _casterGUID))
                        return false;
                    else if(_aura3 > 0 && unit->HasAura(_aura3, _casterGUID))
                        return false;

                    return true;
                }
                else
                    return true;
            }

        private:
            int32 _aura1;
            int32 _aura2;
            int32 _aura3;
            ObjectGuid _casterGUID;
    };

    class UnitEntryCheck
    {
        public:
            UnitEntryCheck (int32 entry1, int32 entry2, int32 entry3) : _entry1(entry1), _entry2(entry2), _entry3(entry3) {}
            bool operator()(WorldObject* object) const
            {
                if(_entry1 > 0)
                {
                    if(_entry1 > 0 && object->GetEntry() == _entry1)
                        return false;
                    if(_entry2 > 0 && object->GetEntry() == _entry2)
                        return false;
                    if(_entry3 > 0 && object->GetEntry() == _entry3)
                        return false;

                    return true;
                }
                if(_entry1 < 0)
                {
                    if(_entry1 < 0 && object->GetEntry() == abs(_entry1))
                        return true;
                    if(_entry2 < 0 && object->GetEntry() == abs(_entry2))
                        return true;
                    if(_entry3 < 0 && object->GetEntry() == abs(_entry3))
                        return true;

                    return false;
                }

                return true;
            }

        private:
            int32 _entry1;
            int32 _entry2;
            int32 _entry3;
    };

    class UnitAttackableCheck
    {
        public:
            UnitAttackableCheck(bool present, Unit* caster) : _present(present), _caster(caster) {}
            bool operator() (Unit* unit)
            {
                return _caster->IsValidAttackTarget(unit) == _present;
            }
            bool operator() (WorldObject* object)
            {
                return object->ToUnit() && _caster->IsValidAttackTarget(object->ToUnit()) == _present;
            }
        private:
            bool _present;
            Unit* _caster;
    };
}
#endif
