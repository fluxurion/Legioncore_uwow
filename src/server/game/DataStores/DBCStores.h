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
#include "DB2Structure.h"
#include "DB2Store.h"
#include "SharedDefines.h"

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
        if (row > _gtEntry->NumRows || column > _gtEntry->NumColumns)
        {
            sLog->outError(LOG_FILTER_UNITS, "GameTable::EvaluateTable Requested wrong row %u or column %u in GameTable: %s", _gtEntry->NumRows, _gtEntry->NumColumns, _gtEntry->Name->Str[LOCALE_enUS]);
            return nullptr;
        }

        return _storage.LookupEntry(_gtEntry->NumRows * column + row);
    }

    inline bool HasEntry(uint32 row, uint32 column) const {  return row < _gtEntry->NumRows && column < _gtEntry->NumColumns; }

    char const* GetFormat() const { return _storage.GetFormat(); }
    uint32 GetFieldCount() const { return _storage.GetFieldCount(); }
    bool Load(char const* fileName) { return _storage.Load(fileName, nullptr); }

    uint32 GetTableRowCount() const { return _gtEntry->NumRows; }
    uint32 GetTableColumnCount() const { return _gtEntry->NumColumns; }

private:
    DBCStorage<T> _storage;
    GameTablesEntry const* _gtEntry;
};

extern GameTable<GameTableEntry>    sGtArmorMitigationByLvlStore;
extern GameTable<GameTableEntry>    sGtArtifactLevelXPStore;
extern GameTable<GameTableEntry>    sGtBarberShopCostBaseStore;
extern GameTable<GameTableEntry>    sGtBattlePetTypeDamageModStore;
extern GameTable<GameTableEntry>    sGtChanceToMeleeCritBaseStore;
extern GameTable<GameTableEntry>    sGtChanceToMeleeCritStore;
extern GameTable<GameTableEntry>    sGtChanceToSpellCritBaseStore;
extern GameTable<GameTableEntry>    sGtChanceToSpellCritStore;
extern GameTable<GameTableEntry>    sGtCombatRatingsMultByILvlStore;
extern GameTable<GameTableEntry>    sGtCombatRatingsStore;
extern GameTable<GameTableEntry>    sGtHonorLevelStore;
extern GameTable<GameTableEntry>    sGtItemSocketCostPerLevelStore;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStore;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStoreExp1;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStoreExp2;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStoreExp3;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStoreExp4;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStoreExp5;
extern GameTable<GameTableEntry>    sGtNpcDamageByClassStoreExp6;
extern GameTable<GameTableEntry>    sGtNPCManaCostScalerStore;
extern GameTable<GameTableEntry>    sGtNpcTotalHpExp1Store;
extern GameTable<GameTableEntry>    sGtNpcTotalHpExp2Store;
extern GameTable<GameTableEntry>    sGtNpcTotalHpExp3Store;
extern GameTable<GameTableEntry>    sGtNpcTotalHpExp4Store;
extern GameTable<GameTableEntry>    sGtNpcTotalHpExp5Store;
extern GameTable<GameTableEntry>    sGtNpcTotalHpExp6Store;
extern GameTable<GameTableEntry>    sGtNpcTotalHpStore;
extern GameTable<GameTableEntry>    sGtOCTBaseHPByClassStore;
extern GameTable<GameTableEntry>    sGtOCTBaseMPByClassStore;
extern GameTable<GameTableEntry>    sGtOCTHpPerStaminaStore;
extern GameTable<GameTableEntry>    sGtOCTLevelExperienceStore;
extern GameTable<GameTableEntry>    sGtRegenMPPerSptStore;
extern GameTable<GameTableEntry>    sGtSpellScalingStore;

void LoadGameTables(std::string const& dataPath, uint32 defaultLocale);
bool IsValidDifficulty(uint32 diff, bool isRaid);

#endif
