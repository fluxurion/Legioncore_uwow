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

#include "DB2Stores.h"
#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DBCfmt.h"
#include "ItemTemplate.h"
#include <iostream>
#include <fstream>

#include <map>

typedef std::map<uint16, uint32> AreaFlagByAreaID;
typedef std::map<uint32, uint32> AreaFlagByMapID;
typedef std::map<uint32, AreaTableEntry const* > AreaEntryMap;
typedef std::map<uint32, DungeonEncounterEntry const*> DungeonEncounterByDisplayID;

struct WMOAreaTableTripple
{
    WMOAreaTableTripple(int32 r, int32 a, int32 g) : groupId(g), rootId(r), adtId(a)
    { }

    bool operator <(const WMOAreaTableTripple& b) const
    {
        return memcmp(this, &b, sizeof(WMOAreaTableTripple)) < 0;
    }

    // ordered by entropy; that way memcmp will have a minimal medium runtime
    int32 groupId;
    int32 rootId;
    int32 adtId;
};


typedef std::map<uint32, SimpleFactionsList> FactionTeamMap;
typedef std::map<WMOAreaTableTripple, WMOAreaTableEntry const*> WMOAreaInfoByTripple;

static AreaEntryMap sAreaEntry;
static AreaFlagByAreaID sAreaFlagByAreaID;
static AreaFlagByMapID sAreaFlagByMapID;
static FactionTeamMap sFactionTeamMap;
static WMOAreaInfoByTripple sWMOAreaInfoByTripple;
static DungeonEncounterByDisplayID sDungeonEncounterByDisplayID;

MapDifficultyMap                            sMapDifficultyMap;
TalentsByPosition                           sTalentByPos;
MinorTalentByIndexArray                     sMinorTalentByIndexStore;

DBCStorage<AreaPOIEntry>                    sAreaPOIStore(AreaPOIEntryfmt);
DBCStorage<AreaTableEntry>                  sAreaStore(AreaTableEntryfmt);
DBCStorage<AreaTriggerEntry>                sAreaTriggerStore(AreaTriggerEntryfmt);
DBCStorage<BannedAddOnsEntry>               sBannedAddOnsStore(BannedAddOnsfmt);
DBCStorage<BattlemasterListEntry>           sBattlemasterListStore(BattlemasterListEntryfmt);
DBCStorage<CharTitlesEntry>                 sCharTitlesStore(CharTitlesEntryfmt);
DBCStorage<ChrClassesEntry>                 sChrClassesStore(ChrClassesEntryfmt);
DBCStorage<CreatureFamilyEntry>             sCreatureFamilyStore(CreatureFamilyfmt);
DBCStorage<CreatureModelDataEntry>          sCreatureModelDataStore(CreatureModelDatafmt);
DBCStorage<DifficultyEntry>                 sDifficultyStore(Difficultyfmt);
DBCStorage<DungeonEncounterEntry>           sDungeonEncounterStore(DungeonEncounterfmt);
DBCStorage<EmotesTextEntry>                 sEmotesTextStore(EmotesTextEntryfmt);
DBCStorage<FactionEntry>                    sFactionStore(FactionEntryfmt);
DBCStorage<FactionTemplateEntry>            sFactionTemplateStore(FactionTemplateEntryfmt);
DBCStorage<GlyphSlotEntry>                  sGlyphSlotStore(GlyphSlotfmt);
DBCStorage<LFGDungeonEntry>                 sLFGDungeonStore(LFGDungeonEntryfmt);
DBCStorage<LiquidTypeEntry>                 sLiquidTypeStore(LiquidTypefmt);
DBCStorage<MapDifficultyEntry>              sMapDifficultyStore(MapDifficultyEntryfmt); // only for loading
DBCStorage<MapEntry>                        sMapStore(MapEntryfmt);
DBCStorage<MinorTalentEntry>                sMinorTalentStore(MinorTalentEntryfmt);
DBCStorage<PhaseEntry>                      sPhaseStores(PhaseEntryfmt);
DBCStorage<QuestPOIBlobEntry>               sQuestPOIBlobStore(QuestPOIBlobfmt);
DBCStorage<SpellItemEnchantmentEntry>       sSpellItemEnchantmentStore(SpellItemEnchantmentfmt);
DBCStorage<StableSlotPricesEntry>           sStableSlotPricesStore(StableSlotPricesfmt);
DBCStorage<TalentEntry>                     sTalentStore(TalentEntryfmt);
DBCStorage<TeamContributionPointsEntry>     sTeamContributionPointsStore(TeamContributionPointsfmt);
DBCStorage<VehicleEntry>                    sVehicleStore(VehicleEntryfmt);
DBCStorage<WMOAreaTableEntry>               sWMOAreaTableStore(WMOAreaTableEntryfmt);
DBCStorage<WorldMapAreaEntry>               sWorldMapAreaStore(WorldMapAreaEntryfmt);
DBCStorage<WorldSafeLocsEntry>              sWorldSafeLocsStore(WorldSafeLocsEntryfmt);

GameTable<GtArmorMitigationByLvlEntry>      sGtArmorMitigationByLvlStore(GameTablefmt);
GameTable<GtBarberShopCostBaseEntry>        sGtBarberShopCostBaseStore(GtBarberShopCostBasefmt);
GameTable<GtBattlePetTypeDamageModEntry>    sGtBattlePetTypeDamageModStore(GtBattlePetTypeDamageModfmt);
GameTable<GtChanceToMeleeCritBaseEntry>     sGtChanceToMeleeCritBaseStore(GtChanceToMeleeCritBasefmt);
GameTable<GtChanceToMeleeCritEntry>         sGtChanceToMeleeCritStore(GtChanceToMeleeCritfmt);
GameTable<GtChanceToSpellCritBaseEntry>     sGtChanceToSpellCritBaseStore(GtChanceToSpellCritBasefmt);
GameTable<GtChanceToSpellCritEntry>         sGtChanceToSpellCritStore(GtChanceToSpellCritfmt);
GameTable<GtCombatRatingsEntry>             sGtCombatRatingsStore(GtCombatRatingsfmt);
GameTable<GtItemSocketCostPerLevelEntry>    sGtItemSocketCostPerLevelStore(GtItemSocketCostPerLevelfmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStore(GameTablefmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStoreExp1(GameTablefmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStoreExp2(GameTablefmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStoreExp3(GameTablefmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStoreExp4(GameTablefmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStoreExp5(GameTablefmt);
GameTable<GtNpcDamageByClassEntry>          sGtNpcDamageByClassStoreExp6(GameTablefmt);
GameTable<GtNPCManaCostScalerEntry>         sGtNPCManaCostScalerStore(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp1Store(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp2Store(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp3Store(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp4Store(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp5Store(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpExp6Store(GameTablefmt);
GameTable<GtNpcTotalHpEntry>                sGtNpcTotalHpStore(GameTablefmt);
GameTable<GtOCTBaseHPByClassEntry>          sGtOCTBaseHPByClassStore(GtOCTBaseHPByClassfmt);
GameTable<GtOCTBaseMPByClassEntry>          sGtOCTBaseMPByClassStore(GtOCTBaseMPByClassfmt);
GameTable<GtOCTHpPerStaminaEntry>           sGtOCTHpPerStaminaStore(GtOCTHpPerStaminafmt);
GameTable<GtOCTLevelExperienceEntry>        sGtOCTLevelExperienceStore(GtOCTLevelExperiencefmt);
GameTable<GtOCTRegenHPEntry>                sGtOCTRegenHPStore(GtOCTRegenHPfmt);
GameTable<GtRegenMPPerSptEntry>             sGtRegenMPPerSptStore(GtRegenMPPerSptfmt);
GameTable<GtSpellScalingEntry>              sGtSpellScalingStore(GtSpellScalingfmt);

typedef std::list<std::string> StoreProblemList;

uint32 DBCFileCount = 0;
uint32 GameTableCount = 0;

static bool LoadDBC_assert_print(uint32 fsize, uint32 rsize, std::string const& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDBC(uint32& availableDbcLocales, StoreProblemList& errors, DBCStorage<T>& storage, std::string const& dbcPath, std::string const& filename, uint32 defaultLocale, std::string const* customFormat = nullptr, std::string const* customIndexName = nullptr)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DBCFileCount;
    std::string dbcFilename = dbcPath + localeNames[defaultLocale] + '/' + filename;
    SqlDbc * sql = nullptr;
    if (customFormat)
        sql = new SqlDbc(&filename, customFormat, customIndexName, storage.GetFormat());

    if (storage.Load(dbcFilename.c_str(), sql))
    {
        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (!(availableDbcLocales & (1 << i)))
                continue;

            std::string localizedName(dbcPath);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str()))
                availableDbcLocales &= ~(1 << i);             // mark as not available for speedup next checks
        }
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << dbcFilename << " exists, and has " << storage.GetFieldCount() << " field(s) (expected " << strlen(storage.GetFormat()) << "). Extracted file might be from wrong client version or a database-update has been forgotten.";
            std::string buf = stream.str();
            errors.push_back(buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }

    delete sql;
}

template<class T>
inline void LoadGameTable(StoreProblemList& errors, std::string const& tableName, GameTable<T>& storage, std::string const& dbcPath, std::string const& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++GameTableCount;
    std::string dbcFilename = dbcPath + filename;

    if (storage.Load(dbcFilename.c_str()))
    {
        bool found = false;
        // Find table definition in GameTables.dbc
        for (GameTablesEntry const* gt : sGameTablesStore)
        {
            if (tableName == gt->Name[DEFAULT_LOCALE].Str[DEFAULT_LOCALE])
            {
                found = true;
                storage.SetGameTableEntry(gt);
                break;
            }
        }

        ASSERT(found, "Game table %s definition not found in GameTables.dbc", tableName.c_str());
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << dbcFilename << " exists, and has " << storage.GetFieldCount() << " field(s) (expected " << strlen(storage.GetFormat()) << "). Extracted file might be from wrong client version or a database-update has been forgotten.";
            std::string buf = stream.str();
            errors.push_back(buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }
}

void LoadDBCStores(std::string const& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath + "dbc/";

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

    LoadDBC(availableDbcLocales, bad_dbc_files, sMapDifficultyStore, dbcPath, "MapDifficulty.dbc", defaultLocale, &CustomMapDifficultyEntryfmt, &CustomMapDifficultyEntryIndex);

#define LOAD_DBC(store, file) LoadDBC(availableDbcLocales, bad_dbc_files, store, dbcPath, file, defaultLocale)

    //LOAD_DBC(sAreaStore,                        "AreaTable.dbc");
    LOAD_DBC(sAreaTriggerStore,                 "AreaTrigger.dbc");
    LOAD_DBC(sBannedAddOnsStore,                "BannedAddOns.dbc");
    LOAD_DBC(sBattlemasterListStore,            "BattlemasterList.dbc");
    LOAD_DBC(sCharTitlesStore,                  "CharTitles.dbc");
    LOAD_DBC(sChrClassesStore,                  "ChrClasses.dbc");
    LOAD_DBC(sCreatureFamilyStore,              "CreatureFamily.dbc");
    //LOAD_DBC(sCreatureModelDataStore,           "CreatureModelData.dbc");
    LOAD_DBC(sDifficultyStore,                  "Difficulty.dbc");
    LOAD_DBC(sDungeonEncounterStore,            "DungeonEncounter.dbc");
    LOAD_DBC(sEmotesTextStore,                  "EmotesText.dbc");
    LOAD_DBC(sFactionStore,                     "Faction.dbc");
    LOAD_DBC(sFactionTemplateStore,             "FactionTemplate.dbc");
    //LOAD_DBC(sLFGDungeonStore,                  "LfgDungeons.dbc");
    LOAD_DBC(sLiquidTypeStore,                  "LiquidType.dbc");
    LOAD_DBC(sMapStore,                         "Map.dbc");
    LOAD_DBC(sMinorTalentStore,                 "MinorTalent.dbc");
    LOAD_DBC(sPhaseStores,                      "Phase.dbc");
    //LOAD_DBC(sSpellItemEnchantmentStore,        "SpellItemEnchantment.dbc");
    LOAD_DBC(sTalentStore,                      "Talent.dbc");
    LOAD_DBC(sVehicleStore,                     "Vehicle.dbc");
    LOAD_DBC(sWMOAreaTableStore,                "WMOAreaTable.dbc");
    LOAD_DBC(sWorldMapAreaStore,                "WorldMapArea.dbc");
    LOAD_DBC(sWorldSafeLocsStore,               "WorldSafeLocs.dbc");

#undef LOAD_DBC

    // error checks
    if (bad_dbc_files.size() >= DBCFileCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Incorrect DataDir value in worldserver.conf or ALL required *.dbc files (%d) not found by path: %sdbc/%s/", DBCFileCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "Some required *.dbc files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), DBCFileCount, str.c_str());
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DBC data stores in %u ms", DBCFileCount, GetMSTimeDiffToNow(oldMSTime));
}

void LoadGameTables(std::string const& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath + "dbc/" + localeNames[defaultLocale] + '/';

    StoreProblemList bad_dbc_files;

#define LOAD_GT(tableName, store, file) LoadGameTable(bad_dbc_files, tableName, store, dbcPath, file)

    LOAD_GT("ArmorMitigationByLvl",    sGtArmorMitigationByLvlStore,    "gtArmorMitigationByLvl.dbc");
    LOAD_GT("BarberShopCostBase",      sGtBarberShopCostBaseStore,      "gtBarberShopCostBase.dbc");
    LOAD_GT("BattlePetTypeDamageMod",  sGtBattlePetTypeDamageModStore,  "gtBattlePetTypeDamageMod.dbc");
    LOAD_GT("ChanceToMeleeCrit",       sGtChanceToMeleeCritStore,       "gtChanceToMeleeCrit.dbc");
    LOAD_GT("ChanceToMeleeCritBase",   sGtChanceToMeleeCritBaseStore,   "gtChanceToMeleeCritBase.dbc");
    LOAD_GT("ChanceToSpellCrit",       sGtChanceToSpellCritStore,       "gtChanceToSpellCrit.dbc");
    LOAD_GT("ChanceToSpellCritBase",   sGtChanceToSpellCritBaseStore,   "gtChanceToSpellCritBase.dbc");
    LOAD_GT("CombatRatings",           sGtCombatRatingsStore,           "gtCombatRatings.dbc");
    LOAD_GT("ItemSocketCostPerLevel",  sGtItemSocketCostPerLevelStore,  "gtItemSocketCostPerLevel.dbc");
    LOAD_GT("NpcDamageByClass",        sGtNpcDamageByClassStore,        "gtNpcDamageByClass.dbc");
    LOAD_GT("NpcDamageByClassExp1",    sGtNpcDamageByClassStoreExp1,    "gtNpcDamageByClassExp1.dbc");
    LOAD_GT("NpcDamageByClassExp2",    sGtNpcDamageByClassStoreExp2,    "gtNpcDamageByClassExp2.dbc");
    LOAD_GT("NpcDamageByClassExp3",    sGtNpcDamageByClassStoreExp3,    "gtNpcDamageByClassExp3.dbc");
    LOAD_GT("NpcDamageByClassExp4",    sGtNpcDamageByClassStoreExp4,    "gtNpcDamageByClassExp4.dbc");
    LOAD_GT("NpcDamageByClassExp5",    sGtNpcDamageByClassStoreExp5,    "gtNpcDamageByClassExp5.dbc");
    LOAD_GT("NpcDamageByClassExp6",    sGtNpcDamageByClassStoreExp6,    "gtNpcDamageByClassExp6.dbc");
    LOAD_GT("NPCManaCostScaler",       sGtNPCManaCostScalerStore,       "gtNPCManaCostScaler.dbc");
    LOAD_GT("NpcTotalHp",              sGtNpcTotalHpStore,              "gtNpcTotalHp.dbc");
    LOAD_GT("NpcTotalHpExp1",          sGtNpcTotalHpExp1Store,          "gtNpcTotalHpExp1.dbc");
    LOAD_GT("NpcTotalHpExp2",          sGtNpcTotalHpExp2Store,          "gtNpcTotalHpExp2.dbc");
    LOAD_GT("NpcTotalHpExp3",          sGtNpcTotalHpExp3Store,          "gtNpcTotalHpExp3.dbc");
    LOAD_GT("NpcTotalHpExp4",          sGtNpcTotalHpExp4Store,          "gtNpcTotalHpExp4.dbc");
    LOAD_GT("NpcTotalHpExp5",          sGtNpcTotalHpExp5Store,          "gtNpcTotalHpExp5.dbc");
    LOAD_GT("NpcTotalHpExp6",          sGtNpcTotalHpExp6Store,          "gtNpcTotalHpExp6.dbc");
    LOAD_GT("OCTBaseHPByClass",        sGtOCTBaseHPByClassStore,        "gtOCTBaseHPByClass.dbc");
    LOAD_GT("OCTBaseMPByClass",        sGtOCTBaseMPByClassStore,        "gtOCTBaseMPByClass.dbc");
    LOAD_GT("OCTHPPerStamina",         sGtOCTHpPerStaminaStore,         "gtOCTHpPerStamina.dbc");
    LOAD_GT("OCTLevelExperience",      sGtOCTLevelExperienceStore,      "gtOCTLevelExperience.dbc");
    LOAD_GT("RegenMPPerSpt",           sGtRegenMPPerSptStore,           "gtRegenMPPerSpt.dbc");
    LOAD_GT("SpellScaling",            sGtSpellScalingStore,            "gtSpellScaling.dbc");

#undef LOAD_GT

    // error checks
    if (bad_dbc_files.size() >= GameTableCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Incorrect DataDir value in worldserver.conf or ALL required *.dbc GameTable files (%d) not found by path: %sdbc/%s/", DBCFileCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "Some required *.dbc GameTable files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), GameTableCount, str.c_str());
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DBC GameTables data stores in %u ms", GameTableCount, GetMSTimeDiffToNow(oldMSTime));
}

void InitDBCCustomStores()
{
    for (AreaTableEntry const* area : sAreaStore)
    {
        sAreaEntry.insert(AreaEntryMap::value_type(area->ID, area));
        sAreaFlagByAreaID.insert(AreaFlagByAreaID::value_type(uint16(area->ID), area->AreaBit));

        if (area->ParentAreaID == 0)
            sAreaFlagByMapID.insert(AreaFlagByMapID::value_type(area->mapid, area->AreaBit));
    }

    for (FactionEntry const* faction : sFactionStore)
    {
        if (faction->team)
        {
            SimpleFactionsList &flist = sFactionTeamMap[faction->team];
            flist.push_back(faction->ID);
        }
    }

    for (MapDifficultyEntry const* entry : sMapDifficultyStore)
    {
        if (!sMapStore.LookupEntry(entry->MapID))
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "DB table `mapdifficulty_dbc` or MapDifficulty.dbc has non-existant map %u.", entry->MapID);
            continue;
        }
        if (entry->DifficultyID && !sDifficultyStore.LookupEntry(entry->DifficultyID))
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "DB table `mapdifficulty_dbc` or MapDifficulty.dbc has non-existant difficulty %u.", entry->DifficultyID);
            continue;
        }
        sMapDifficultyMap[entry->MapID][entry->DifficultyID] = entry;
    }

    for (TalentEntry const* talentInfo : sTalentStore)
        if (talentInfo->classId < MAX_CLASSES && talentInfo->row < 7 && talentInfo->column < 3)
            sTalentByPos[talentInfo->classId][talentInfo->row][talentInfo->column].push_back(talentInfo);

    for (WMOAreaTableEntry const* wmoAreaTableEntry : sWMOAreaTableStore)
        sWMOAreaInfoByTripple.insert(WMOAreaInfoByTripple::value_type(WMOAreaTableTripple(wmoAreaTableEntry->WMOID, wmoAreaTableEntry->NameSet, wmoAreaTableEntry->WMOGroupID), wmoAreaTableEntry));

    for (DungeonEncounterEntry const* store : sDungeonEncounterStore)
        if (store->creatureDisplayID)
            sDungeonEncounterByDisplayID[store->creatureDisplayID] = store;

    memset(sMinorTalentByIndexStore, 0, sizeof(sMinorTalentByIndexStore));
    for (MinorTalentEntry const* minotTal : sMinorTalentStore)
        sMinorTalentByIndexStore[minotTal->SpecID][minotTal->OrderIndex] = minotTal;
}

SimpleFactionsList const* GetFactionTeamList(uint32 faction)
{
    FactionTeamMap::const_iterator itr = sFactionTeamMap.find(faction);
    if (itr != sFactionTeamMap.end())
        return &itr->second;

    return nullptr;
}

char const* GetPetName(uint32 petfamily)
{
    if (!petfamily)
        return nullptr;

    CreatureFamilyEntry const* pet_family = sCreatureFamilyStore.LookupEntry(petfamily);
    if (!pet_family)
        return nullptr;

    return pet_family->Name ? pet_family->Name : nullptr;
}

int32 GetAreaFlagByAreaID(uint32 area_id)
{
    AreaFlagByAreaID::iterator i = sAreaFlagByAreaID.find(area_id);
    if (i == sAreaFlagByAreaID.end())
        return -1;

    return i->second;
}

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid)
{
    WMOAreaInfoByTripple::iterator i = sWMOAreaInfoByTripple.find(WMOAreaTableTripple(rootid, adtid, groupid));
    if (i == sWMOAreaInfoByTripple.end())
        return nullptr;
    return i->second;
}

AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id)
{
    int32 areaflag = GetAreaFlagByAreaID(area_id);
    if (areaflag < 0)
        return nullptr;

    return sAreaStore.LookupEntry(areaflag);
}

AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag, uint32 map_id)
{
    if (area_flag)
        return sAreaStore.LookupEntry(area_flag);

    if (MapEntry const* mapEntry = sMapStore.LookupEntry(map_id))
        return GetAreaEntryByAreaID(mapEntry->linked_zone);

    return nullptr;
}

uint32 GetAreaFlagByMapId(uint32 mapid)
{
    AreaFlagByMapID::iterator i = sAreaFlagByMapID.find(mapid);
    if (i == sAreaFlagByMapID.end())
        return 0;
    else
        return i->second;
}

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId)
{
    if (mapid != 530 && mapid != 571 && mapid != 732)   // speed for most cases
        return mapid;

    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->DisplayMapID >= 0 ? wma->DisplayMapID : wma->MapID;

    return mapid;
}

int32 GetMapFromZone(uint32 zoneId)
{
    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->DisplayMapID >= 0 ? wma->DisplayMapID : wma->MapID;

    return -1;
}

uint32 GetExpansionForLevel(uint32 level)
{
    if (level < 60)
        return EXPANSION_CLASSIC;
    else if (level < 70)
        return EXPANSION_THE_BURNING_CRUSADE;
    else if (level < 80)
        return EXPANSION_WRATH_OF_THE_LICH_KING;
    else if (level < 85)
        return EXPANSION_CATACLYSM;
    else if (level < 90)
        return EXPANSION_MISTS_OF_PANDARIA;
    else if (level < 100)
        return EXPANSION_WARLORDS_OF_DRAENOR;
    else if (level < 110)
        return EXPANSION_LEGION;
    else
        return CURRENT_EXPANSION;
}

void Zone2MapCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    std::swap(x, y);                                         // at client map coords swapped
    x = x*((maEntry->LocBottom - maEntry->LocTop) / 100) + maEntry->LocTop;
    y = y*((maEntry->LocRight - maEntry->LocLeft) / 100) + maEntry->LocLeft;      // client y coord from top to down
}

void Map2ZoneCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if (!maEntry)
        return;

    x = (x - maEntry->LocTop) / ((maEntry->LocBottom - maEntry->LocTop) / 100);
    y = (y - maEntry->LocLeft) / ((maEntry->LocRight - maEntry->LocLeft) / 100);    // client y coord from top to down
    std::swap(x, y);                                         // client have map coords swapped
}

MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapID)
{
    auto itr = sMapDifficultyMap.find(mapID);
    if (itr == sMapDifficultyMap.end())
        return nullptr;

    if (itr->second.empty())
        return nullptr;

    for (auto& p : itr->second)
    {
        DifficultyEntry const* difficulty = sDifficultyStore.LookupEntry(p.first);
        if (!difficulty)
            continue;

        if (difficulty->Flags & DIFFICULTY_FLAG_DEFAULT)
            return p.second;
    }

    return itr->second.begin()->second;
}


MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty)
{
    auto itr = sMapDifficultyMap.find(mapId);
    if (itr == sMapDifficultyMap.end())
        return nullptr;

    auto diffItr = itr->second.find(difficulty);
    if (diffItr == itr->second.end())
        return nullptr;

    return diffItr->second;
}

MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty)
{
    DifficultyEntry const* diffEntry = sDifficultyStore.LookupEntry(difficulty);
    if (!diffEntry)
        return GetDefaultMapDifficulty(mapId);

    uint32 tmpDiff = difficulty;
    MapDifficultyEntry const* mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff));
    while (!mapDiff)
    {
        tmpDiff = diffEntry->FallbackDifficultyID;
        diffEntry = sDifficultyStore.LookupEntry(tmpDiff);
        if (!diffEntry)
            return GetDefaultMapDifficulty(mapId);

        // pull new data
        mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff)); // we are 10 normal or 25 normal
    }

    difficulty = Difficulty(tmpDiff);
    return mapDiff;
}

uint32 GetLiquidFlags(uint32 liquidType)
{
    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquidType))
        return 1 << liq->Type;

    return 0;
}

float GetCurrencyPrecision(uint32 currencyId)
{
    CurrencyTypesEntry const * entry = sCurrencyTypesStore.LookupEntry(currencyId);
    return entry ? entry->GetPrecision() : 1.0f;
}

bool MapEntry::IsDifficultyModeSupported(uint32 difficulty) const
{
    return IsValidDifficulty(difficulty, IsRaid());
}

bool IsValidDifficulty(uint32 diff, bool isRaid)
{
    if (diff == DIFFICULTY_NONE)
        return true;

    switch (diff)
    {
        case DIFFICULTY_NORMAL:
        case DIFFICULTY_HEROIC:
        case DIFFICULTY_N_SCENARIO:
        case DIFFICULTY_HC_SCENARIO:
        case DIFFICULTY_CHALLENGE:
            return !isRaid;
        default:
            break;
    }

    return isRaid;
}

AreaTableEntry const* FindAreaEntry(uint32 area)
{
    auto data = sAreaEntry.find(area);
    if (data == sAreaEntry.end())
        return nullptr;
    return data->second;
}

uint32 GetParentZoneOrSelf(uint32 zone)
{
    AreaTableEntry const* a = FindAreaEntry(zone);
    if (!a)
        return zone;
    return a->ParentAreaID ? a->ParentAreaID : zone;
}

DungeonEncounterEntry const* GetDungeonEncounterByDisplayID(uint32 displayID)
{
    auto data = sDungeonEncounterByDisplayID.find(displayID);
    if (data == sDungeonEncounterByDisplayID.end())
        return nullptr;
    return data->second;
}
