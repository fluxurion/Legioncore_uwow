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

#include "Containers.h"
#include "DatabaseEnv.h"
#include "GameObject.h"
#include "Garrison.h"
#include "GarrisonMgr.h"
#include "ObjectDefines.h"
#include "ObjectMgr.h"
#include "World.h"

uint32 GarrisonMgr::getFirstMap(uint32 map)
{
    switch (map)
    {
        case 1152:
        case 1330:
        case 1153:
            return 1152;
        case 1158:
        case 1331:
        case 1159:
            return 1158;
    }
    return 0;
}

void GarrisonMgr::Initialize()
{
    for (GarrSiteLevelPlotInstEntry const* plotInstance : sGarrSiteLevelPlotInstStore)
        _garrisonPlotInstBySiteLevel[plotInstance->GarrSiteLevelID].push_back(plotInstance);

    for (GameObjectsEntry const* gameObject : sGameObjectsStore)
        if (gameObject->Type == GAMEOBJECT_TYPE_GARRISON_PLOT)
            _garrisonPlots[gameObject->MapID][gameObject->Data[0]] = gameObject;

    for (GarrPlotBuildingEntry const* plotBuilding : sGarrPlotBuildingStore)
        _garrisonBuildingsByPlot[plotBuilding->GarrPlotID].insert(plotBuilding->GarrBuildingID);

    for (GarrBuildingPlotInstEntry const* buildingPlotInst : sGarrBuildingPlotInstStore)
        _garrisonBuildingPlotInstances[MAKE_PAIR64(buildingPlotInst->GarrBuildingID, buildingPlotInst->GarrSiteLevelPlotInstID)] = buildingPlotInst->ID;

    for (GarrBuildingEntry const* building : sGarrBuildingStore)
        _garrisonBuildingsByType[building->Type].push_back(building);

    for (GarrFollowerXAbilityEntry const* followerAbility : sGarrFollowerXAbilityStore)
    {
        if (GarrAbilityEntry const* ability = sGarrAbilityStore.LookupEntry(followerAbility->GarrAbilityID))
        {
            if (!(ability->Flags & GARRISON_ABILITY_CANNOT_ROLL) && ability->Flags & GARRISON_ABILITY_FLAG_TRAIT)
                _garrisonFollowerRandomTraits.insert(ability);

            if (followerAbility->FactionIndex < 2)
            {
                if (ability->Flags & GARRISON_ABILITY_FLAG_TRAIT)
                    _garrisonFollowerAbilities[followerAbility->FactionIndex][followerAbility->GarrFollowerID].Traits.insert(ability);
                else
                    _garrisonFollowerAbilities[followerAbility->FactionIndex][followerAbility->GarrFollowerID].Counters.insert(ability);
            }
        }
    }

    InitializeDbIdSequences();
    LoadPlotFinalizeGOInfo();
    LoadFollowerClassSpecAbilities();
    LoadBuildingSpawnNPC();
    LoadBuildingSpawnGo();
    LoadMissionLine();
    LoadShipment();
}

GarrSiteLevelEntry const* GarrisonMgr::GetGarrSiteLevelEntry(uint32 garrSiteId, uint32 level) const
{
    for (GarrSiteLevelEntry const* garrSiteLevel : sGarrSiteLevelStore)
        if (garrSiteLevel->SiteID == garrSiteId && garrSiteLevel->Level == level)
            return garrSiteLevel;

    return nullptr;
}

std::vector<GarrSiteLevelPlotInstEntry const*> const* GarrisonMgr::GetGarrPlotInstForSiteLevel(uint32 garrSiteLevelId) const
{
    auto itr = _garrisonPlotInstBySiteLevel.find(garrSiteLevelId);
    if (itr != _garrisonPlotInstBySiteLevel.end())
        return &itr->second;

    return nullptr;
}

GameObjectsEntry const* GarrisonMgr::GetPlotGameObject(uint32 mapId, uint32 garrPlotInstanceId) const
{
    auto mapItr = _garrisonPlots.find(mapId);
    if (mapItr != _garrisonPlots.end())
    {
        auto plotItr = mapItr->second.find(garrPlotInstanceId);
        if (plotItr != mapItr->second.end())
            return plotItr->second;
    }

    return nullptr;
}

bool GarrisonMgr::IsPlotMatchingBuilding(uint32 garrPlotId, uint32 garrBuildingId) const
{
    auto plotItr = _garrisonBuildingsByPlot.find(garrPlotId);
    if (plotItr != _garrisonBuildingsByPlot.end())
        return plotItr->second.count(garrBuildingId) > 0;

    return false;
}

uint32 GarrisonMgr::GetGarrBuildingPlotInst(uint32 garrBuildingId, uint32 garrSiteLevelPlotInstId) const
{
    auto itr = _garrisonBuildingPlotInstances.find(MAKE_PAIR64(garrBuildingId, garrSiteLevelPlotInstId));
    if (itr != _garrisonBuildingPlotInstances.end())
        return itr->second;

    return 0;
}

GarrBuildingEntry const* GarrisonMgr::GetPreviousLevelBuilding(uint32 buildingType, uint32 currentLevel) const
{
    auto itr = _garrisonBuildingsByType.find(buildingType);
    if (itr != _garrisonBuildingsByType.end())
        for (GarrBuildingEntry const* building : itr->second)
            if (building->Level == currentLevel - 1)
                return building;

    return nullptr;
}

FinalizeGarrisonPlotGOInfo const* GarrisonMgr::GetPlotFinalizeGOInfo(uint32 garrPlotInstanceID) const
{
    auto itr = _finalizePlotGOInfo.find(garrPlotInstanceID);
    if (itr != _finalizePlotGOInfo.end())
        return &itr->second;

    return nullptr;
}

uint64 GarrisonMgr::GenerateFollowerDbId()
{
    if (_followerDbIdGenerator >= std::numeric_limits<uint64>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Garrison follower db id overflow! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }

    return _followerDbIdGenerator++;
}

uint64 GarrisonMgr::GenerateMissionDbId()
{
    if (_missionDbIdGenerator >= std::numeric_limits<uint64>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Garrison mission db id overflow! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }

    return _missionDbIdGenerator++;
}

uint64 GarrisonMgr::GenerateShipmentDbId()
{
    if (_shipmentDbIdGenerator >= std::numeric_limits<uint64>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Garrison shipment db id overflow! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }

    return _shipmentDbIdGenerator++;
}

uint32 const AbilitiesForQuality[][2] =
{
    // Counters, Traits
    {0, 0},
    {1, 0},
    {1, 1},   // Uncommon
    {1, 2},   // Rare
    {2, 3},   // Epic
    {2, 3}    // Legendary
};

std::list<GarrAbilityEntry const*> GarrisonMgr::RollFollowerAbilities(GarrFollowerEntry const* follower, uint32 quality, uint32 faction, bool initial) const
{
    ASSERT(faction < 2);

    bool hasForcedExclusiveTrait = false;
    std::list<GarrAbilityEntry const*> result;
    int32 slots[2] = {AbilitiesForQuality[quality][0], AbilitiesForQuality[quality][1]};

    GarrAbilities const* abilities = nullptr;
    auto itr = _garrisonFollowerAbilities[faction].find(follower->ID);
    if (itr != _garrisonFollowerAbilities[faction].end())
        abilities = &itr->second;

    std::list<GarrAbilityEntry const*> abilityList, forcedAbilities, traitList, forcedTraits;
    if (abilities)
    {
        for (GarrAbilityEntry const* ability : abilities->Counters)
        {
            if (ability->Flags & GARRISON_ABILITY_HORDE_ONLY && faction != GARRISON_FACTION_INDEX_HORDE)
                continue;
            else if (ability->Flags & GARRISON_ABILITY_ALLIANCE_ONLY && faction != GARRISON_FACTION_INDEX_ALLIANCE)
                continue;

            if (ability->Flags & GARRISON_ABILITY_FLAG_CANNOT_REMOVE)
                forcedAbilities.push_back(ability);
            else
                abilityList.push_back(ability);
        }

        for (GarrAbilityEntry const* ability : abilities->Traits)
        {
            if (ability->Flags & GARRISON_ABILITY_HORDE_ONLY && faction != GARRISON_FACTION_INDEX_HORDE)
                continue;
            else if (ability->Flags & GARRISON_ABILITY_ALLIANCE_ONLY && faction != GARRISON_FACTION_INDEX_ALLIANCE)
                continue;

            if (ability->Flags & GARRISON_ABILITY_FLAG_CANNOT_REMOVE)
                forcedTraits.push_back(ability);
            else
                traitList.push_back(ability);
        }
    }

    Trinity::Containers::RandomResizeList(abilityList, std::max<int32>(0, slots[0] - forcedAbilities.size()));
    Trinity::Containers::RandomResizeList(traitList, std::max<int32>(0, slots[1] - forcedTraits.size()));

    // Add abilities specified in GarrFollowerXAbility.db2 before generic classspec ones on follower creation
    if (initial)
    {
        forcedAbilities.splice(forcedAbilities.end(), abilityList);
        forcedTraits.splice(forcedTraits.end(), traitList);
    }

    forcedAbilities.sort();
    abilityList.sort();
    forcedTraits.sort();
    traitList.sort();

    // check if we have a trait from exclusive category
    for (GarrAbilityEntry const* ability : forcedTraits)
    {
        if (ability->Flags & GARRISON_ABILITY_FLAG_EXCLUSIVE)
        {
            hasForcedExclusiveTrait = true;
            break;
        }
    }

    if (slots[0] > forcedAbilities.size() + abilityList.size())
    {
        std::list<GarrAbilityEntry const*> classSpecAbilities = GetClassSpecAbilities(follower, faction);
        std::list<GarrAbilityEntry const*> classSpecAbilitiesTemp, classSpecAbilitiesTemp2;
        classSpecAbilitiesTemp2.swap(abilityList);
        std::set_difference(classSpecAbilities.begin(), classSpecAbilities.end(), forcedAbilities.begin(), forcedAbilities.end(), std::back_inserter(classSpecAbilitiesTemp));
        std::set_union(classSpecAbilitiesTemp.begin(), classSpecAbilitiesTemp.end(), classSpecAbilitiesTemp2.begin(), classSpecAbilitiesTemp2.end(), std::back_inserter(abilityList));

        Trinity::Containers::RandomResizeList(abilityList, std::max<int32>(0, slots[0] - forcedAbilities.size()));
    }

    if (slots[1] > forcedTraits.size() + traitList.size())
    {
        std::list<GarrAbilityEntry const*> genericTraits, genericTraitsTemp;
        for (GarrAbilityEntry const* ability : _garrisonFollowerRandomTraits)
        {
            if (ability->Flags & GARRISON_ABILITY_HORDE_ONLY && faction != GARRISON_FACTION_INDEX_HORDE)
                continue;
            else if (ability->Flags & GARRISON_ABILITY_ALLIANCE_ONLY && faction != GARRISON_FACTION_INDEX_ALLIANCE)
                continue;

            // forced exclusive trait exists, skip other ones entirely
            if (hasForcedExclusiveTrait && ability->Flags & GARRISON_ABILITY_FLAG_EXCLUSIVE)
                continue;

            genericTraitsTemp.push_back(ability);
        }

        std::set_difference(genericTraitsTemp.begin(), genericTraitsTemp.end(), forcedTraits.begin(), forcedTraits.end(), std::back_inserter(genericTraits));
        genericTraits.splice(genericTraits.begin(), traitList);
        // "split" the list into two parts [nonexclusive, exclusive] to make selection later easier
        genericTraits.sort([](GarrAbilityEntry const* a1, GarrAbilityEntry const* a2)
        {
            uint32 e1 = a1->Flags & GARRISON_ABILITY_FLAG_EXCLUSIVE;
            uint32 e2 = a2->Flags & GARRISON_ABILITY_FLAG_EXCLUSIVE;
            if (e1 != e2)
                return e1 < e2;

            return a1->ID < a2->ID;
        });
        genericTraits.unique();

        size_t firstExclusive = 0, total = genericTraits.size();
        for (auto itr = genericTraits.begin(); itr != genericTraits.end(); ++itr, ++firstExclusive)
            if ((*itr)->Flags & GARRISON_ABILITY_FLAG_EXCLUSIVE)
                break;

        while (traitList.size() < std::max<int32>(0, slots[1] - forcedTraits.size()) && total)
        {
            auto itr = genericTraits.begin();
            std::advance(itr, urand(0, total-- - 1));
            if ((*itr)->Flags & GARRISON_ABILITY_FLAG_EXCLUSIVE)
                total = firstExclusive; // selected exclusive trait - no other can be selected now
            else
                --firstExclusive;

            traitList.push_back(*itr);
            genericTraits.erase(itr);
        }
    }

    result.splice(result.end(), forcedAbilities);
    result.splice(result.end(), abilityList);
    result.splice(result.end(), forcedTraits);
    result.splice(result.end(), traitList);

    return result;
}

std::list<GarrAbilityEntry const*> GarrisonMgr::GetClassSpecAbilities(GarrFollowerEntry const* follower, uint32 faction) const
{
    std::list<GarrAbilityEntry const*> abilities;
    uint32 classSpecId;
    switch (faction)
    {
        case GARRISON_FACTION_INDEX_HORDE:
            classSpecId = follower->HordeGarrClassSpecID;
            break;
        case GARRISON_FACTION_INDEX_ALLIANCE:
            classSpecId = follower->AllianceGarrClassSpecID;
            break;
        default:
            return abilities;
    }

    if (!sGarrClassSpecStore.LookupEntry(classSpecId))
        return abilities;

    auto itr = _garrisonFollowerClassSpecAbilities.find(classSpecId);
    if (itr != _garrisonFollowerClassSpecAbilities.end())
        abilities = itr->second;

    return abilities;
}

void GarrisonMgr::InitializeDbIdSequences()
{
    if (QueryResult result = CharacterDatabase.Query("SELECT MAX(dbId) FROM character_garrison_followers"))
        _followerDbIdGenerator = (*result)[0].GetUInt64() + 1;

    if (QueryResult result = CharacterDatabase.Query("SELECT MAX(dbId) FROM character_garrison_missions"))
        _missionDbIdGenerator = (*result)[0].GetUInt64() + 1;

    if (QueryResult result = CharacterDatabase.Query("SELECT MAX(dbId) FROM character_garrison_shipment"))
        _shipmentDbIdGenerator = (*result)[0].GetUInt64() + 1;
}

void GarrisonMgr::LoadPlotFinalizeGOInfo()
{
    //                                                                0                  1       2       3       4       5
    QueryResult result = WorldDatabase.Query("SELECT garrPlotInstanceId, hordeGameObjectId, hordeX, hordeY, hordeZ, hordeO, "
    //                      6          7          8         9         10                 11
        "allianceGameObjectId, allianceX, allianceY, allianceZ, allianceO FROM garrison_plot_finalize_info");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison follower class spec abilities. DB table `garrison_plot_finalize_info` is empty.");
        return;
    }

    uint32 msTime = getMSTime();
    do
    {
        Field* fields = result->Fetch();
        uint32 garrPlotInstanceId = fields[0].GetUInt32();
        uint32 hordeGameObjectId = fields[1].GetUInt32();
        uint32 allianceGameObjectId = fields[6].GetUInt32();
        
        if (!sGarrPlotInstanceStore.LookupEntry(garrPlotInstanceId))
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrPlotInstance.db2 entry %u was referenced in `garrison_plot_finalize_info`.", garrPlotInstanceId);
            continue;
        }

        GameObjectTemplate const* goTemplate = sObjectMgr->GetGameObjectTemplate(hordeGameObjectId);
        if (!goTemplate)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing gameobject_template entry %u was referenced in `garrison_plot_finalize_info`.`hordeGameObjectId` for garrPlotInstanceId %u goID %u.",
                hordeGameObjectId, garrPlotInstanceId, hordeGameObjectId);
            continue;
        }

        if (goTemplate->type != GAMEOBJECT_TYPE_GOOBER)
        {
            sLog->outError(LOG_FILTER_SQL, "Invalid gameobject type %u (entry %u) was referenced in `garrison_plot_finalize_info`.`hordeGameObjectId` for garrPlotInstanceId %u.",
                goTemplate->type, hordeGameObjectId, garrPlotInstanceId);
            continue;
        }

        goTemplate = sObjectMgr->GetGameObjectTemplate(allianceGameObjectId);
        if (!goTemplate)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing gameobject_template entry %u was referenced in `garrison_plot_finalize_info`.`allianceGameObjectId` for garrPlotInstanceId %u goID %u.",
                allianceGameObjectId, garrPlotInstanceId, allianceGameObjectId);
            continue;
        }

        if (goTemplate->type != GAMEOBJECT_TYPE_GOOBER)
        {
            sLog->outError(LOG_FILTER_SQL, "Invalid gameobject type %u (entry %u) was referenced in `garrison_plot_finalize_info`.`allianceGameObjectId` for garrPlotInstanceId %u.",
                goTemplate->type, allianceGameObjectId, garrPlotInstanceId);
            continue;
        }
        

        FinalizeGarrisonPlotGOInfo& info = _finalizePlotGOInfo[garrPlotInstanceId];
        info.FactionInfo[GARRISON_FACTION_INDEX_HORDE].GameObjectId = hordeGameObjectId;
        info.FactionInfo[GARRISON_FACTION_INDEX_HORDE].Pos.Relocate(fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat(), fields[5].GetFloat());

        info.FactionInfo[GARRISON_FACTION_INDEX_ALLIANCE].GameObjectId = allianceGameObjectId;
        info.FactionInfo[GARRISON_FACTION_INDEX_ALLIANCE].Pos.Relocate(fields[7].GetFloat(), fields[8].GetFloat(), fields[9].GetFloat(), fields[10].GetFloat());
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison plot finalize entries in %u.", uint32(_finalizePlotGOInfo.size()), GetMSTimeDiffToNow(msTime));
}

void GarrisonMgr::LoadFollowerClassSpecAbilities()
{
    QueryResult result = WorldDatabase.Query("SELECT classSpecId, abilityId FROM garrison_follower_class_spec_abilities");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison follower class spec abilities. DB table `garrison_follower_class_spec_abilities` is empty.");
        return;
    }

    uint32 msTime = getMSTime();
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 classSpecId = fields[0].GetUInt32();
        uint32 abilityId = fields[1].GetUInt32();

        if (!sGarrClassSpecStore.LookupEntry(classSpecId))
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrClassSpec.db2 entry %u was referenced in `garrison_follower_class_spec_abilities` by row (%u, %u).", classSpecId, classSpecId, abilityId);
            continue;
        }

        GarrAbilityEntry const* ability = sGarrAbilityStore.LookupEntry(abilityId);
        if (!ability)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrAbility.db2 entry %u was referenced in `garrison_follower_class_spec_abilities` by row (%u, %u).", abilityId, classSpecId, abilityId);
            continue;
        }

        _garrisonFollowerClassSpecAbilities[classSpecId].push_back(ability);
        ++count;

    }
    while (result->NextRow());

    for (auto& pair : _garrisonFollowerClassSpecAbilities)
        pair.second.sort();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison follower class spec abilities in %u.", count, GetMSTimeDiffToNow(msTime));
}

void GarrisonMgr::LoadBuildingSpawnNPC()
{
    //                                                  0       1      2   3    4              5           6            7
    QueryResult result = WorldDatabase.Query("SELECT plotID, BuildID, id, map, position_x, position_y, position_z, orientation, building FROM garrison_building_creature");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison building creatures. DB table `garrison_building_creature` is empty.");
        return;
    }

    uint32 msTime = getMSTime();
    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        uint32 garrPlotInstanceId = fields[index++].GetUInt32();
        uint32 BuildID = fields[index++].GetUInt32();
        uint32 entry = fields[index++].GetUInt32();

        CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(entry);
        if (!cInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `garrison_building_creature` has creature with non existing creature entry %u, skipped.", entry);
            continue;
        }

        if (!sGarrPlotInstanceStore.LookupEntry(garrPlotInstanceId))
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrPlotInstance.db2 entry %u was referenced in `garrison_building_creature`.", garrPlotInstanceId);
            continue;
        }

        //! BuildID = 0 - empty build spawn.
        if (BuildID && !sGarrBuildingStore.LookupEntry(BuildID))
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrBuilding.db2 entry %u was referenced in `garrison_building_creature`.", BuildID);
            continue;
        }

        CreatureData data;
        data.id = entry;
        uint32 map = fields[index++].GetUInt16();
        data.mapid = getFirstMap(map);
        data.posX = fields[index++].GetFloat();
        data.posY = fields[index++].GetFloat();
        data.posZ = fields[index++].GetFloat();
        data.orientation = fields[index++].GetFloat();
        data.building = fields[index++].GetBool();
        data.dbData = false;
        _buildSpawnNpc[BuildID][garrPlotInstanceId].push_back(data);

        if (!data.mapid)
            sLog->outError(LOG_FILTER_SQL, "Not supported map %u in `garrison_building_creature`.", map);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison building creatures in %u.", count, GetMSTimeDiffToNow(msTime));
}

void GarrisonMgr::LoadBuildingSpawnGo()
{
    //                                                  0       1      2   3    4              5           6            7           8           9       10          11          12
    QueryResult result = WorldDatabase.Query("SELECT plotID, BuildID, id, map, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, building FROM garrison_building_gameobject");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison building go. DB table `garrison_building_gameobject` is empty.");
        return;
    }

    uint32 msTime = getMSTime();
    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        uint32 garrPlotInstanceId = fields[index++].GetUInt32();
        uint32 BuildID = fields[index++].GetUInt32();
        uint32 entry = fields[index++].GetUInt32();
        auto templ = sObjectMgr->GetGameObjectTemplate(entry);
        if (!templ)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `garrison_building_gameobject` has go with non existing go entry %u, skipped.", entry);
            continue;
        }

        if (templ->type == GAMEOBJECT_TYPE_GARRISON_BUILDING || templ->type == GAMEOBJECT_TYPE_GARRISON_PLOT)
        {
            if (entry  != 239085)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `garrison_building_gameobject` has go (%u) with no allowed type %u, skipped.", templ->type, entry);
                continue;
            }
        }

        if (!sGarrPlotInstanceStore.LookupEntry(garrPlotInstanceId))
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrPlotInstance.db2 entry %u was referenced in `garrison_building_gameobject`.", garrPlotInstanceId);
            continue;
        }

        //! BuildID = 0 - empty build spawn.
        if (BuildID && !sGarrBuildingStore.LookupEntry(BuildID))
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing GarrBuilding.db2 entry %u was referenced in `garrison_building_gameobject`.", BuildID);
            continue;
        }

        GameObjectData data;
        data.id = entry;
        uint32 map = fields[index++].GetUInt16();
        data.mapid = getFirstMap(map);
        data.posX = fields[index++].GetFloat();
        data.posY = fields[index++].GetFloat();
        data.posZ = fields[index++].GetFloat();
        data.orientation = fields[index++].GetFloat();
        data.rotation0 = fields[index++].GetFloat();
        data.rotation1 = fields[index++].GetFloat();
        data.rotation2 = fields[index++].GetFloat();
        data.rotation3 = fields[index++].GetFloat();
        data.building = fields[index++].GetBool();
        data.dbData = false;
        _buildSpawnGo[BuildID][garrPlotInstanceId].push_back(data);

        if (!data.mapid)
            sLog->outError(LOG_FILTER_SQL, "Not supported map %u in `garrison_building_gameobject`.", map);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison building go in %u.", count, GetMSTimeDiffToNow(msTime));
}

void GarrisonMgr::LoadMissionLine()
{
    //                                                  0    1          2                   3
    QueryResult result = WorldDatabase.Query("SELECT ID, NextMission, ReqGarrFollowerID, IsRandom FROM garrison_mission_line");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison mission lines. DB table `garrison_mission_line` is empty.");
        return;
    }

    uint32 msTime = getMSTime();
    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        uint32 missionID = fields[index++].GetUInt32();
        uint32 NextMission = fields[index++].GetUInt32();
        uint32 ReqGarrFollowerID = fields[index++].GetUInt32();
        bool IsRandom = fields[index++].GetBool();

        GarrMissionEntry const* missionEntry = sGarrMissionStore.LookupEntry(missionID);
        if (!missionEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing sGarrMissionStore missionID %u was referenced in `garrison_mission_line`.", missionID);
            continue;
        }

        GarrMissionEntry const* NextMissionEntry = sGarrMissionStore.LookupEntry(NextMission);

        if (NextMission && !NextMissionEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing sGarrMissionStore missionID %u was referenced in `garrison_mission_line`.", NextMission);
            continue;
        }

        GarrFollowerEntry const* followerEntry = sGarrFollowerStore.LookupEntry(ReqGarrFollowerID);
        if (ReqGarrFollowerID && !followerEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing sGarrFollowerStore ReqGarrFollowerID %u was referenced in `garrison_mission_line`.", ReqGarrFollowerID);
            continue;
        }

        GarrMissionLine &data = _MissionLineStore[missionID];
        data.MissionID = missionEntry;
        data.NextMission = NextMissionEntry;
        data.Reqfollower = followerEntry;
        data.isRandom = IsRandom;

        if (NextMissionEntry)
            _nextMission[missionID] = NextMissionEntry;

        if (followerEntry)
            _nextMissionByFollower[ReqGarrFollowerID] = missionEntry;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison mission lines in %u.", count, GetMSTimeDiffToNow(msTime));
}

void GarrisonMgr::LoadShipment()
{
    //                                                  0            1         2         3
    QueryResult result = WorldDatabase.Query("SELECT SiteID, ContainerID, NpcEntry, ShipmentID FROM garrison_shipment");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison shipment. DB table `garrison_shipment` is empty.");
        return;
    }

    uint32 msTime = getMSTime();
    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        GarrShipment data;
        data.SiteID = fields[index++].GetUInt32();
        data.ContainerID = fields[index++].GetUInt32();
        data.NpcEntry = fields[index++].GetUInt32();
        data.ShipmentID = fields[index++].GetUInt32();

        if (data.SiteID && data.SiteID != SITE_ID_GARRISON_ALLIANCE && data.SiteID != SITE_ID_GARRISON_HORDE)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `garrison_shipment` has non-existen SiteID %u, skipped.", data.SiteID);
            continue;
        }

        CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(data.NpcEntry);
        if (!cInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `garrison_shipment` has creature with non existing creature entry %u, skipped.", data.NpcEntry);
            continue;
        }

        CharShipmentEntry const* shipmentEntry = sCharShipmentStore.LookupEntry(data.ShipmentID);
        if (!shipmentEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing CharShipment.db2 entry %u was referenced in `garrison_shipment`.", data.ShipmentID);
            continue;
        }

        CharShipmentConteiner const* shipmentConteinerEntry = sCharShipmentContainerStore.LookupEntry(data.ContainerID);
        if (!shipmentConteinerEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Non-existing CharShipmentContainer.db2 entry %u was referenced in `garrison_shipment`.", data.ContainerID);
            continue;
        }
        data.cEntry = shipmentConteinerEntry;
        data.data = shipmentEntry;

        const_cast<CreatureTemplate*>(cInfo)->npcflag2 |= UNIT_NPC_FLAG2_SHIPMENT_ORDER;
        const_cast<CreatureTemplate*>(cInfo)->npcflag |= UNIT_NPC_FLAG_GOSSIP;
        const_cast<CreatureTemplate*>(cInfo)->IconName = "workorders";

        shipment[SHIPMENT_GET_BY_NPC].insert(shipmentStoreMap::value_type(data.NpcEntry, data));
        shipment[SHIPMENT_GET_BY_CONTEINER_ID].insert(shipmentStoreMap::value_type(data.ContainerID, data));

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison_shipment in %u.", count, GetMSTimeDiffToNow(msTime));
}

GarrShipment const* GarrisonMgr::GetGarrShipment(uint32 entry, ShipmentGetType type) const
{
    std::map<uint8, shipmentStoreMap>::const_iterator i = shipment.find(type);
    if (i == shipment.end())
        return NULL;

    shipmentStoreMap::const_iterator itr = i->second.find(entry);
    if (itr == i->second.end())
        return NULL;

    return &itr->second;
}

GarrMissionEntry const* GarrisonMgr::GetNextMissionInQuestLine(uint32 missionID)
{
    auto data = _nextMission.find(missionID);
    if (data != _nextMission.end())
        return data->second;
    return NULL;
}

GarrMissionEntry const* GarrisonMgr::GetMissionAtFollowerTaking(uint32 followerID)
{
    auto data = _nextMissionByFollower.find(followerID);
    if (data != _nextMissionByFollower.end())
        return data->second;
    return NULL;
}

std::list<GameObjectData> const* GarrisonMgr::GetGoSpawnBuilding(uint32 plotID, uint32 build) const
{
    auto b = _buildSpawnGo.find(build);
    if (b != _buildSpawnGo.end())
    {
        auto p = b->second.find(plotID);
        if (p != b->second.end())
        {
            return &p->second;
        }
    }
    return NULL;
}

std::list<CreatureData> const* GarrisonMgr::GetNpcSpawnBuilding(uint32 plotID, uint32 build) const
{
    auto b = _buildSpawnNpc.find(build);
    if (b != _buildSpawnNpc.end())
    {
        auto p = b->second.find(plotID);
        if (p != b->second.end())
        {
            return &p->second;
        }
    }
    return NULL;
}

uint32 GarrisonMgr::GetShipmentID(GarrShipment const* shipment)
{
    if (shipment->cEntry->BuildingType != GARR_BTYPE_TRADING_POST)
        return shipment->ShipmentID;

    if (time(nullptr) > _randShipment[shipment->ContainerID].Timer)
    {
        uint32 count = sDB2Manager._charShipmentConteiner.count(shipment->data->ShipmentConteinerID);
        uint32 idx = urand(1, count);
        uint32 i = 1;
        DB2Manager::ShipmentConteinerMapBounds bounds = sDB2Manager.GetShipmentConteinerBounds(shipment->data->ShipmentConteinerID);
        for (DB2Manager::ShipmentConteinerMap::const_iterator sh_idx = bounds.first; sh_idx != bounds.second; ++sh_idx)
        {
            if (i == idx)
            {
                _randShipment[shipment->ContainerID].ShipmentID = sh_idx->second->ID;
                break;
            }
            i++;
        }

        _randShipment[shipment->ContainerID].Timer = time(nullptr) + DAY;
    }

    return _randShipment[shipment->ContainerID].ShipmentID;
}