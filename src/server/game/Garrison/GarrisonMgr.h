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

#ifndef GarrisonMgr_h__
#define GarrisonMgr_h__

#include <unordered_set>

#include "DB2Stores.h"

struct randShipment
{
    uint32 Timer = 0;
    uint32 ShipmentID = 0;
};

struct FinalizeGarrisonPlotGOInfo
{
    struct
    {
        uint32 GameObjectId;
        Position Pos;
    } FactionInfo[2];
};

struct GarrMissionLine
{
    GarrMissionEntry const* MissionID;
    GarrMissionEntry const* NextMission;
    GarrFollowerEntry const* Reqfollower;
    bool isRandom = false;
};

struct GarrAbilities
{
    std::unordered_set<GarrAbilityEntry const*> Counters;
    std::unordered_set<GarrAbilityEntry const*> Traits;
};

struct GarrShipment
{
    uint32 SiteID = 0;
    uint32 ContainerID = 0;
    uint32 NpcEntry = 0;
    uint32 ShipmentID = 0;

    CharShipmentEntry const* data = NULL;
    CharShipmentConteiner const* cEntry = NULL;
};

struct GarrTradeSkill
{
    uint32 spellID;
    uint32 conditionID;
};

typedef std::list<GarrTradeSkill /*tradeskill*/> TradeskillList;
typedef std::unordered_map<uint32 /*npcEntry*/, TradeskillList> NpcTradeskillList;


enum ShipmentGetType : uint8
{
    SHIPMENT_GET_BY_NPC           = 0,
    SHIPMENT_GET_BY_CONTEINER_ID  = 1,
};

enum GarrBuildingType
{
    GARR_BTYPE_NONE         = 0,
    GARR_BTYPE_MINE         = 1,
    GARR_BTYPE_HERB_GARDEN  = 2,
    GARR_BTYPE_BARN         = 3,
    GARR_BTYPE_LUMBER_MILL  = 4,
    GARR_BTYPE_TAVERN       = 5,
    GARR_BTYPE_TRADING_POST = 6,
    GARR_BTYPE_MENAGERIE    = 7,
    GARR_BTYPE_BARRACKS     = 8,
    GARR_BTYPE_WARMILL      = 10,
    GARR_BTYPE_STABLES      = 11,
    GARR_BTYPE_MAGETOWER    = 13,
    GARR_BTYPE_SALVAGE_YARD = 14,
    GARR_BTYPE_STOREHOUSE   = 15,
    GARR_BTYPE_ALCHEMY_LAB  = 16,
    GARR_BTYPE_FORGE        = 17,
    GARR_BTYPE_ENCHANTERS   = 18,
    GARR_BTYPE_ENGINEERING  = 19,
    GARR_BTYPE_SCRIBE       = 20,
    GARR_BTYPE_GEM          = 21,
    GARR_BTYPE_TANNERY      = 22,
    GARR_BTYPE_TAILORING    = 23,
    GARR_BTYPE_FISHING      = 24,
    GARR_BTYPE_GLADIATORS   = 25,
    GARR_BTYPE_WORKSHOP     = 26,
    GARR_BTYPE_MAX
};

class GarrisonMgr
{
public:
    static GarrisonMgr& Instance()
    {
        static GarrisonMgr instance;
        return instance;
    }

    static uint32 getFirstMap(uint32 map);

    void Initialize();

    GarrSiteLevelEntry const* GetGarrSiteLevelEntry(uint32 garrSiteId, uint32 level) const;
    std::vector<GarrSiteLevelPlotInstEntry const*> const* GetGarrPlotInstForSiteLevel(uint32 garrSiteLevelId) const;
    GameObjectsEntry const* GetPlotGameObject(uint32 mapId, uint32 garrPlotInstanceId) const;
    bool IsPlotMatchingBuilding(uint32 garrPlotId, uint32 garrBuildingId) const;
    uint32 GetGarrBuildingPlotInst(uint32 garrBuildingId, uint32 garrSiteLevelPlotInstId) const;
    GarrBuildingEntry const* GetPreviousLevelBuilding(uint32 buildingType, uint32 currentLevel) const;
    FinalizeGarrisonPlotGOInfo const* GetPlotFinalizeGOInfo(uint32 garrPlotInstanceID) const;
    uint64 GenerateFollowerDbId();
    std::list<GarrAbilityEntry const*> RollFollowerAbilities(GarrFollowerEntry const* follower, uint32 quality, uint32 faction, bool initial) const;
    std::list<GarrAbilityEntry const*> GetClassSpecAbilities(GarrFollowerEntry const* follower, uint32 faction) const;
    uint64 GenerateMissionDbId();
    uint64 GenerateShipmentDbId();
    std::list<GameObjectData> const* GetGoSpawnBuilding(uint32 plotID, uint32 build) const;
    std::list<CreatureData> const* GetNpcSpawnBuilding(uint32 plotID, uint32 build) const;

    GarrMissionEntry const* GetNextMissionInQuestLine(uint32 missionID);
    GarrMissionEntry const* GetMissionAtFollowerTaking(uint32 followerID);

    GarrShipment const* GetGarrShipment(uint32 entry, ShipmentGetType type) const;
    uint32 GetShipmentID(GarrShipment const* shipment);
    
private:
    void InitializeDbIdSequences();
    void LoadPlotFinalizeGOInfo();
    void LoadFollowerClassSpecAbilities();
    void LoadBuildingSpawnNPC();
    void LoadBuildingSpawnGo();
    void LoadMissionLine();
    void LoadShipment();
    void LoadTradeSkill();

    std::unordered_map<uint32 /*garrSiteId*/, std::vector<GarrSiteLevelPlotInstEntry const*>> _garrisonPlotInstBySiteLevel;
    std::unordered_map<uint32 /*mapId*/, std::unordered_map<uint32 /*garrPlotId*/, GameObjectsEntry const*>> _garrisonPlots;
    std::unordered_map<uint32 /*garrPlotId*/, std::unordered_set<uint32/*garrBuildingId*/>> _garrisonBuildingsByPlot;
    std::unordered_map<uint64 /*garrBuildingId | garrSiteLevelPlotInstId << 32*/, uint32 /*garrBuildingPlotInstId*/> _garrisonBuildingPlotInstances;
    std::unordered_map<uint32 /*buildingType*/, std::vector<GarrBuildingEntry const*>> _garrisonBuildingsByType;
    std::unordered_map<uint32 /*garrPlotInstanceId*/, FinalizeGarrisonPlotGOInfo> _finalizePlotGOInfo;
    std::unordered_map<uint32 /*garrFollowerId*/, GarrAbilities> _garrisonFollowerAbilities[2];
    std::unordered_map<uint32 /*classSpecId*/, std::list<GarrAbilityEntry const*>> _garrisonFollowerClassSpecAbilities;

    std::unordered_map<uint32 /*BuildID*/, std::unordered_map<uint32 /*garrPlotId*/, std::list<GameObjectData>>> _buildSpawnGo;
    std::unordered_map<uint32 /*BuildID*/, std::unordered_map<uint32 /*garrPlotId*/, std::list<CreatureData>>> _buildSpawnNpc;

    std::unordered_map<uint32 /*MissionID*/, GarrMissionLine /*nextMission*/> _MissionLineStore;
    std::unordered_map<uint32 /*MissionID*/, GarrMissionEntry const* /*nextMission*/> _nextMission;
    std::unordered_map<uint32 /*FollowerID*/, GarrMissionEntry const* /*nextMission*/> _nextMissionByFollower;
    
    NpcTradeskillList _garrNpcTradeSkill;
    
    typedef std::unordered_map<uint32/*entry*/, GarrShipment> shipmentStoreMap;
    std::map<uint8 /*ShipmentGetType*/, shipmentStoreMap> shipment;

    std::set<GarrAbilityEntry const*> _garrisonFollowerRandomTraits;
    std::unordered_map<uint32 /*ShipmentConteinerID*/, randShipment> _randShipment;
    
    uint64 _followerDbIdGenerator = UI64LIT(1);
    uint64 _missionDbIdGenerator = UI64LIT(1);
    uint64 _shipmentDbIdGenerator = UI64LIT(1);
};

#define sGarrisonMgr GarrisonMgr::Instance()

#endif // GarrisonMgr_h__
