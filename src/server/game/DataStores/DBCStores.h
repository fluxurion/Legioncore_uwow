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

#ifndef TRINITY_DBCSTORES_H
#define TRINITY_DBCSTORES_H

#include "Common.h"
#include "DBCStore.h"
#include "DBCStructure.h"
#include "DB2Store.h"
#include "DB2Structure.h"
#include "SharedDefines.h"

#include <list>

typedef std::list<uint32> SimpleFactionsList;
SimpleFactionsList const* GetFactionTeamList(uint32 faction);

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid);

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId);
int32 GetMapFromZone(uint32 zoneId);

uint32 GetExpansionForLevel(uint32 level);

void Zone2MapCoordinates(float &x, float &y, uint32 zone);
void Map2ZoneCoordinates(float &x, float &y, uint32 zone);

uint32 GetLiquidFlags(uint32 liquidType);

template<class T>
class GameTable
{
public:
    GameTable(char const* format) : _storage(format), _gtEntry(nullptr) { }

    void SetGameTableEntry(GameTablesEntry const* gtEntry) { _gtEntry = gtEntry; }

    T const* EvaluateTable(uint32 row, uint32 column) const
    {
        ASSERT(row < _gtEntry->NumRows, "Requested row %u from GameTable %s but there are only %u rows!", row, _gtEntry->Name->Str[0], _gtEntry->NumRows);
        ASSERT(column < _gtEntry->NumColumns, "Requested column %u from GameTable %s but there are only %u columns!", column, _gtEntry->Name->Str[0], _gtEntry->NumColumns);

        return _storage.LookupEntry(_gtEntry->NumRows * column + row);
    }

    inline bool HasEntry(uint32 row, uint32 column) const
    {
        return row < _gtEntry->NumRows && column < _gtEntry->NumColumns;
    }

    char const* GetFormat() const { return _storage.GetFormat(); }
    uint32 GetFieldCount() const { return _storage.GetFieldCount(); }
    bool Load(char const* fileName) { return _storage.Load(fileName, nullptr); }

    uint32 GetTableRowCount() const { return _gtEntry->NumRows; }
    uint32 GetTableColumnCount() const { return _gtEntry->NumColumns; }

private:
    DBCStorage<T> _storage;
    GameTablesEntry const* _gtEntry;
};


extern DBCStorage<BannedAddOnsEntry>            sBannedAddOnsStore;
extern DBCStorage<BattlemasterListEntry>        sBattlemasterListStore;
extern DBCStorage<ChrClassesEntry>              sChrClassesStore;
extern DBCStorage<CreatureModelDataEntry>       sCreatureModelDataStore;
extern DBCStorage<FactionEntry>                 sFactionStore;
extern DBCStorage<FactionTemplateEntry>         sFactionTemplateStore;
extern DBCStorage<GlyphSlotEntry>               sGlyphSlotStore;
extern DBCStorage<LFGDungeonEntry>              sLFGDungeonStore;
extern DBCStorage<LiquidTypeEntry>              sLiquidTypeStore;
extern DBCStorage<MapDifficultyEntry>           sMapDifficultyStore;
extern DBCStorage<SpellItemEnchantmentEntry>    sSpellItemEnchantmentStore;
extern DBCStorage<TalentEntry>                  sTalentStore;
extern DBCStorage<VehicleEntry>                 sVehicleStore;
extern DBCStorage<WMOAreaTableEntry>            sWMOAreaTableStore;
extern DBCStorage<WorldMapAreaEntry>            sWorldMapAreaStore; //-- use Zone2MapCoordinates and Map2ZoneCoordinates
extern DBCStorage<WorldSafeLocsEntry>           sWorldSafeLocsStore;

extern GameTable<GtArmorMitigationByLvlEntry>   sGtArmorMitigationByLvlStore;
extern GameTable<GtBarberShopCostBaseEntry>     sGtBarberShopCostBaseStore;
extern GameTable<GtBattlePetTypeDamageModEntry> sGtBattlePetTypeDamageModStore;
extern GameTable<GtChanceToMeleeCritBaseEntry>  sGtChanceToMeleeCritBaseStore;
extern GameTable<GtChanceToMeleeCritEntry>      sGtChanceToMeleeCritStore;
extern GameTable<GtChanceToSpellCritBaseEntry>  sGtChanceToSpellCritBaseStore;
extern GameTable<GtChanceToSpellCritEntry>      sGtChanceToSpellCritStore;
extern GameTable<GtCombatRatingsEntry>          sGtCombatRatingsStore;
extern GameTable<GtItemSocketCostPerLevelEntry> sGtItemSocketCostPerLevelStore;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStore;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStoreExp1;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStoreExp2;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStoreExp3;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStoreExp4;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStoreExp5;
extern GameTable<GtNpcDamageByClassEntry>       sGtNpcDamageByClassStoreExp6;
extern GameTable<GtNPCManaCostScalerEntry>      sGtNPCManaCostScalerStore;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp1Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp2Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp3Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp4Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp5Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp6Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpStore;
extern GameTable<GtOCTBaseHPByClassEntry>       sGtOCTBaseHPByClassStore;
extern GameTable<GtOCTBaseMPByClassEntry>       sGtOCTBaseMPByClassStore;
extern GameTable<GtOCTHpPerStaminaEntry>        sGtOCTHpPerStaminaStore;
extern GameTable<GtOCTLevelExperienceEntry>     sGtOCTLevelExperienceStore;
extern GameTable<GtRegenMPPerSptEntry>          sGtRegenMPPerSptStore;
extern GameTable<GtSpellScalingEntry>           sGtSpellScalingStore;

void LoadDBCStores(std::string const& dataPath, uint32 defaultLocale);
void LoadGameTables(std::string const& dataPath, uint32 defaultLocale);
void InitDBCCustomStores();
bool IsValidDifficulty(uint32 diff, bool isRaid);

typedef std::vector<TalentEntry const*> TalentsByPosition[MAX_CLASSES][7][3];
extern TalentsByPosition                         sTalentByPos;

#endif
