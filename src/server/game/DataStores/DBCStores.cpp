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

GameTable<GameTableEntry>   sGtArmorMitigationByLvlStore(GameTablefmt);
GameTable<GameTableEntry>   sGtArtifactLevelXPStore(GameTablefmt);
GameTable<GameTableEntry>   sGtBarberShopCostBaseStore(GameTablefmt);
GameTable<GameTableEntry>   sGtBattlePetTypeDamageModStore(GameTablefmt);
GameTable<GameTableEntry>   sGtChanceToMeleeCritBaseStore(GameTablefmt);
GameTable<GameTableEntry>   sGtChanceToMeleeCritStore(GameTablefmt);
GameTable<GameTableEntry>   sGtChanceToSpellCritBaseStore(GameTablefmt);
GameTable<GameTableEntry>   sGtChanceToSpellCritStore(GameTablefmt);
GameTable<GameTableEntry>   sGtCombatRatingsMultByILvlStore(GameTablefmt);
GameTable<GameTableEntry>   sGtCombatRatingsStore(GameTablefmt);
GameTable<GameTableEntry>   sGtHonorLevelStore(GameTablefmt);
GameTable<GameTableEntry>   sGtItemSocketCostPerLevelStore(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStore(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStoreExp1(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStoreExp2(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStoreExp3(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStoreExp4(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStoreExp5(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcDamageByClassStoreExp6(GameTablefmt);
GameTable<GameTableEntry>   sGtNPCManaCostScalerStore(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpExp1Store(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpExp2Store(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpExp3Store(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpExp4Store(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpExp5Store(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpExp6Store(GameTablefmt);
GameTable<GameTableEntry>   sGtNpcTotalHpStore(GameTablefmt);
GameTable<GameTableEntry>   sGtOCTBaseHPByClassStore(GameTablefmt);
GameTable<GameTableEntry>   sGtOCTBaseMPByClassStore(GameTablefmt);
GameTable<GameTableEntry>   sGtOCTHpPerStaminaStore(GameTablefmt);
GameTable<GameTableEntry>   sGtOCTLevelExperienceStore(GameTablefmt);
GameTable<GameTableEntry>   sGtOCTRegenHPStore(GameTablefmt);
GameTable<GameTableEntry>   sGtRegenMPPerSptStore(GameTablefmt);
GameTable<GameTableEntry>   sGtSpellScalingStore(GameTablefmt);

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
        for (uint8 i = 0; i < MAX_LOCALES; ++i)
        {
            if (!(availableDbcLocales & (1 << i)) || i == LOCALE_none)
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
        for (GameTablesEntry const* gt : sGameTablesStore)
        {
            for (uint32 l = 0; l < MAX_LOCALES; ++l)
            {
                if (l != LOCALE_none && tableName == gt->Name->Str[l])
                {
                    found = true;
                    storage.SetGameTableEntry(gt);
                    break;
                }
            }

            if (found)
                break;
        }

        ASSERT(found, "Game table %s definition not found in GameTables.db2", tableName.c_str());
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
    LOAD_GT("HonorLevel",              sGtHonorLevelStore,              "gtHonorLevel.dbc");
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
    LOAD_GT("ArtifactLevelXP",         sGtArtifactLevelXPStore,         "gtArtifactLevelXP.dbc");
    LOAD_GT("CombatRatingsMultByILvl", sGtCombatRatingsMultByILvlStore, "gtCombatRatingsMultByILvl.dbc");
    /*
        exist but'til unused

        gtBattlePetXP
        gtChallengeModeDamage
        gtChallengeModeHealth
        gtSandboxScaling

        this was deleted in 21154.... use them from old builds atm

        gtChanceToMeleeCrit
        gtChanceToMeleeCritBase
        gtChanceToSpellCrit
        gtChanceToSpellCritBase
        gtOCTBaseHPByClass
        gtRegenMPPerSpt
    */

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
    if (!maEntry)
        return;

    std::swap(x, y);                                         // at client map coords swapped
    x = x*((maEntry->LocBottom - maEntry->LocTop) / 100) + maEntry->LocTop;
    y = y*((maEntry->LocRight - maEntry->LocLeft) / 100) + maEntry->LocLeft;      // client y coord from top to down
}

void Map2ZoneCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);
    if (!maEntry)
        return;

    x = (x - maEntry->LocTop) / ((maEntry->LocBottom - maEntry->LocTop) / 100);
    y = (y - maEntry->LocLeft) / ((maEntry->LocRight - maEntry->LocLeft) / 100);    // client y coord from top to down
    std::swap(x, y);                                         // client have map coords swapped
}

uint32 GetLiquidFlags(uint32 liquidType)
{
    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquidType))
        return 1 << liq->Type;

    return 0;
}

float GetCurrencyPrecision(uint32 currencyId)
{
    CurrencyTypesEntry const* entry = sCurrencyTypesStore.LookupEntry(currencyId);
    if (!entry)
        return 1.0f;

    return entry->Flags & CURRENCY_FLAG_HAS_PRECISION ? CURRENCY_PRECISION : 1.0f;
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
