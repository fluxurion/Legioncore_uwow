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
uint32 GetLearnSpell(uint32 trigerSpell);
uint32 GetSpellByTrigger(uint32 trigerSpell);

std::list<uint32> const* GetSpellProcsPerMinuteModList(uint32 PerMinId);

char const* GetPetName(uint32 petfamily, uint32 dbclang);
SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint8 difficulty);
SpellTargetRestrictionsEntry const *GetSpellTargetRestrioctions(uint32 spellid, uint16 difficulty);

int32 GetAreaFlagByAreaID(uint32 area_id);                  // -1 if not found
AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id);
AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag, uint32 map_id);
uint32 GetAreaFlagByMapId(uint32 mapid);

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid);

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId);
int32 GetMapFromZone(uint32 zoneId);

enum ContentLevels
{
    CONTENT_1_60    = 0,
    CONTENT_61_70   = 1,
    CONTENT_71_80   = 2,
    CONTENT_81_85   = 3,
    CONTENT_86_90   = 4,
    MAX_CONTENT
};

uint32 GetMaxLevelForExpansion(uint32 expansion);
uint32 GetExpansionForLevel(uint32 level);

ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId);

void Zone2MapCoordinates(float &x, float &y, uint32 zone);
void Map2ZoneCoordinates(float &x, float &y, uint32 zone);

typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficultyEntry const*>> MapDifficultyMap;
MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapID);
MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty);
MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty);

uint32 GetLiquidFlags(uint32 liquidType);

PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level);
PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id);

template<class T>
class GameTable
{
public:
    GameTable(char const* format) : _storage(format), _gtEntry(nullptr) { }

    void SetGameTableEntry(GameTablesEntry const* gtEntry) { _gtEntry = gtEntry; }

    T const* EvaluateTable(int32 row, int32 column) const
    {
        if (row > (int32)_gtEntry->NumRows || column > (int32)_gtEntry->NumColumns)
        {
            sLog->outU(" >> GameTable::EvaluateTable dbc %s request (row: %i, column: %i) max(row: %u, column: %u)", _gtEntry->Name, row, column, _gtEntry->NumRows, _gtEntry->NumColumns);
            return NULL;
        }
        ASSERT(row < (int32)_gtEntry->NumRows);
        ASSERT(column < (int32)_gtEntry->NumColumns);

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




extern DBCStorage<AreaPOIEntry>                 sAreaPOIStore;
extern DBCStorage<AreaTableEntry>               sAreaStore;
extern DBCStorage<AreaTriggerEntry>             sAreaTriggerStore;
extern DBCStorage<BannedAddOnsEntry>            sBannedAddOnsStore;
extern DBCStorage<BattlemasterListEntry>        sBattlemasterListStore;
extern DBCStorage<CharTitlesEntry>              sCharTitlesStore;
extern DBCStorage<ChatChannelsEntry>            sChatChannelsStore;
extern DBCStorage<ChrClassesEntry>              sChrClassesStore;
extern DBCStorage<ChrSpecializationsEntry>      sChrSpecializationsStore;
extern DBCStorage<CinematicSequencesEntry>      sCinematicSequencesStore;
extern DBCStorage<CreatureFamilyEntry>          sCreatureFamilyStore;
extern DBCStorage<CreatureModelDataEntry>       sCreatureModelDataStore;
extern DBCStorage<DifficultyEntry>              sDifficultyStore;
extern DBCStorage<DungeonEncounterEntry>        sDungeonEncounterStore;
extern DBCStorage<EmotesEntry>                  sEmotesStore;
extern DBCStorage<EmotesTextEntry>              sEmotesTextStore;
extern DBCStorage<FactionEntry>                 sFactionStore;
extern DBCStorage<FactionTemplateEntry>         sFactionTemplateStore;
extern DBCStorage<GemPropertiesEntry>           sGemPropertiesStore;
extern DBCStorage<GlyphPropertiesEntry>         sGlyphPropertiesStore;
extern DBCStorage<GlyphSlotEntry>               sGlyphSlotStore;
extern DBCStorage<ItemSetEntry>                 sItemSetStore;
extern DBCStorage<LFGDungeonEntry>              sLFGDungeonStore;
extern DBCStorage<LiquidTypeEntry>              sLiquidTypeStore;
extern DBCStorage<LockEntry>                    sLockStore;
extern DBCStorage<MapDifficultyEntry>           sMapDifficultyStore; // use GetMapDifficultyData instead
extern DBCStorage<MapEntry>                     sMapStore;
extern DBCStorage<MinorTalentEntry>             sMinorTalentStore;
extern DBCStorage<MovieEntry>                   sMovieStore;
extern DBCStorage<PhaseEntry>                   sPhaseStores;
extern DBCStorage<PowerDisplayEntry>            sPowerDisplayStore;
extern DBCStorage<QuestPOIBlobEntry>            sQuestPOIBlobStore;
extern DBCStorage<SpellEffectEntry>             sSpellEffectStore;
extern DBCStorage<SpellEntry>                   sSpellStore;
extern DBCStorage<SpellItemEnchantmentEntry>    sSpellItemEnchantmentStore;
extern DBCStorage<SpellLevelsEntry>             sSpellLevelsStore;
extern DBCStorage<SpellProcsPerMinuteEntry>     sSpellProcsPerMinuteStore;
extern DBCStorage<SpellProcsPerMinuteModEntry>  sSpellProcsPerMinuteModStore;
extern DBCStorage<SpellRadiusEntry>             sSpellRadiusStore;
extern DBCStorage<SpellRangeEntry>              sSpellRangeStore;
extern DBCStorage<SpellScalingEntry>            sSpellScalingStore;
extern DBCStorage<SpellShapeshiftEntry>         sSpellShapeshiftStore;
extern DBCStorage<SpellShapeshiftFormEntry>     sSpellShapeshiftFormStore;
extern DBCStorage<SpellTargetRestrictionsEntry> sSpellTargetRestrictionsStore;
extern DBCStorage<SummonPropertiesEntry>        sSummonPropertiesStore;
extern DBCStorage<TalentEntry>                  sTalentStore;
extern DBCStorage<VehicleEntry>                 sVehicleStore;
extern DBCStorage<VehicleSeatEntry>             sVehicleSeatStore;
extern DBCStorage<WMOAreaTableEntry>            sWMOAreaTableStore;
extern DBCStorage<WorldMapAreaEntry>            sWorldMapAreaStore; //-- use Zone2MapCoordinates and Map2ZoneCoordinates
extern DBCStorage<WorldSafeLocsEntry>           sWorldSafeLocsStore;

extern GameTable<GtBarberShopCostBaseEntry>     sGtBarberShopCostBaseStore;
extern GameTable<GtArmorMitigationByLvlEntry>   sGtArmorMitigationByLvlStore;
extern GameTable<GtBattlePetTypeDamageModEntry> sGtBattlePetTypeDamageModStore;
extern GameTable<GtChanceToMeleeCritBaseEntry>  sGtChanceToMeleeCritBaseStore;
extern GameTable<GtChanceToMeleeCritEntry>      sGtChanceToMeleeCritStore;
extern GameTable<GtChanceToSpellCritBaseEntry>  sGtChanceToSpellCritBaseStore;
extern GameTable<GtChanceToSpellCritEntry>      sGtChanceToSpellCritStore;
extern GameTable<GtCombatRatingsEntry>          sGtCombatRatingsStore;
extern GameTable<GtItemSocketCostPerLevelEntry> sGtItemSocketCostPerLevelStore;
extern GameTable<GtNPCManaCostScalerEntry>      sGtNPCManaCostScalerStore;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp1Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp2Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp3Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp4Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpExp5Store;
extern GameTable<GtNpcTotalHpEntry>             sGtNpcTotalHpStore;
extern GameTable<GtOCTBaseHPByClassEntry>       sGtOCTBaseHPByClassStore;
extern GameTable<GtOCTBaseMPByClassEntry>       sGtOCTBaseMPByClassStore;
extern GameTable<GtOCTHpPerStaminaEntry>        sGtOCTHpPerStaminaStore;
extern GameTable<GtOCTLevelExperienceEntry>     sGtOCTLevelExperienceStore;
extern GameTable<GtRegenMPPerSptEntry>          sGtRegenMPPerSptStore;
extern GameTable<GtSpellScalingEntry>           sGtSpellScalingStore;

extern MapDifficultyMap                          sMapDifficultyMap;
extern PetFamilySpellsStore                      sPetFamilySpellsStore;
extern SpellCategoryStore                        sSpellCategoryStore;
extern SpellSkillingList                         sSpellSkillingList;

void LoadDBCStores(std::string const& dataPath, uint32 defaultLocale);
void LoadGameTables(std::string const& dataPath, uint32 defaultLocale);
void InitDBCCustomStores();
bool IsValidDifficulty(uint32 diff, bool isRaid);

AreaTableEntry const* FindAreaEntry(uint32 area);
DungeonEncounterEntry const* GetDungeonEncounterByDisplayID(uint32 displayID);
uint32 GetParentZoneOrSelf(uint32 zone);

typedef ChrSpecializationsEntry const* ChrSpecializationByIndexArray[MAX_CLASSES][4];
extern ChrSpecializationByIndexArray             sChrSpecializationByIndexStore;

typedef std::vector<TalentEntry const*> TalentsByPosition[MAX_CLASSES][7][3];
extern TalentsByPosition                         sTalentByPos;

typedef MinorTalentEntry const* MinorTalentByIndexArray[SPEC_MAX_SPEC_INDEX][4];
extern MinorTalentByIndexArray             sMinorTalentByIndexStore;

#endif
