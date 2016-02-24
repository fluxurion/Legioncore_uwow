/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#ifndef TRINITY_DB2STORES_H
#define TRINITY_DB2STORES_H

#include "DB2Store.h"
#include "DB2Structure.h"
#include "SharedDefines.h"

#include <boost/regex.hpp>

extern DB2Storage<AchievementEntry>                 sAchievementStore;
extern DB2Storage<AreaGroupMemberEntry>             sAreaGroupMemberStore;
extern DB2Storage<AreaTableEntry>                   sAreaTableStore;
extern DB2Storage<AreaTriggerEntry>                 sAreaTriggerStore;
extern DB2Storage<ArmorLocationEntry>               sArmorLocationStore;
extern DB2Storage<AuctionHouseEntry>                sAuctionHouseStore;
extern DB2Storage<BankBagSlotPricesEntry>           sBankBagSlotPricesStore;
extern DB2Storage<BannedAddonsEntry>                sBannedAddOnsStore;
extern DB2Storage<BarberShopStyleEntry>             sBarberShopStyleStore;
extern DB2Storage<BattlemasterListEntry>            sBattlemasterListStore;
extern DB2Storage<BattlePetAbilityEffectEntry>      sBattlePetAbilityEffectStore;
extern DB2Storage<BattlePetAbilityEntry>            sBattlePetAbilityStore;
extern DB2Storage<BattlePetAbilityStateEntry>       sBattlePetAbilityStateStore;
extern DB2Storage<BattlePetAbilityTurnEntry>        sBattlePetAbilityTurnStore;
extern DB2Storage<BattlePetBreedQualityEntry>       sBattlePetBreedQualityStore;
extern DB2Storage<BattlePetBreedStateEntry>         sBattlePetBreedStateStore;
extern DB2Storage<BattlePetEffectPropertiesEntry>   sBattlePetEffectPropertiesStore;
extern DB2Storage<BattlePetSpeciesEntry>            sBattlePetSpeciesStore;
extern DB2Storage<BattlePetSpeciesStateEntry>       sBattlePetSpeciesStateStore;
extern DB2Storage<BattlePetSpeciesXAbilityEntry>    sBattlePetSpeciesXAbilityStore;
extern DB2Storage<BattlePetStateEntry>              sBattlePetStateStore;
extern DB2Storage<BroadcastTextEntry>               sBroadcastTextStore;
extern DB2Storage<CharSectionsEntry>                sCharSectionsStore;
extern DB2Storage<CharStartOutfitEntry>             sCharStartOutfitStore;
extern DB2Storage<CharTitlesEntry>                  sCharTitlesStore;
extern DB2Storage<ChatChannelsEntry>                sChatChannelsStore;
extern DB2Storage<ChrClassesEntry>                  sChrClassesStore;
extern DB2Storage<ChrClassesXPowerTypesEntry>       sChrClassesXPowerTypesStore;
extern DB2Storage<ChrRacesEntry>                    sChrRacesStore;
extern DB2Storage<ChrSpecializationEntry>           sChrSpecializationStore;
extern DB2Storage<CreatureDisplayInfoEntry>         sCreatureDisplayInfoStore;
extern DB2Storage<CreatureFamilyEntry>              sCreatureFamilyStore;
extern DB2Storage<CreatureModelDataEntry>           sCreatureModelDataStore;
extern DB2Storage<CreatureTypeEntry>                sCreatureTypeStore;
extern DB2Storage<CriteriaEntry>                    sCriteriaStore;
extern DB2Storage<CriteriaTreeEntry>                sCriteriaTreeStore;
extern DB2Storage<CurrencyTypesEntry>               sCurrencyTypesStore;
extern DB2Storage<DestructibleModelDataEntry>       sDestructibleModelDataStore;
extern DB2Storage<DifficultyEntry>                  sDifficultyStore;
extern DB2Storage<DungeonEncounterEntry>            sDungeonEncounterStore;
extern DB2Storage<DurabilityCostsEntry>             sDurabilityCostsStore;
extern DB2Storage<DurabilityQualityEntry>           sDurabilityQualityStore;
extern DB2Storage<EmotesEntry>                      sEmotesStore;
extern DB2Storage<EmotesTextEntry>                  sEmotesTextStore;
extern DB2Storage<FactionEntry>                     sFactionStore;
extern DB2Storage<FactionTemplateEntry>             sFactionTemplateStore;
extern DB2Storage<GameObjectDisplayInfoEntry>       sGameObjectDisplayInfoStore;
extern DB2Storage<GameObjectsEntry>                 sGameObjectsStore;
extern DB2Storage<GameTablesEntry>                  sGameTablesStore;
extern DB2Storage<GarrAbilityEntry>                 sGarrAbilityStore;
extern DB2Storage<GarrBuildingEntry>                sGarrBuildingStore;
extern DB2Storage<GarrBuildingPlotInstEntry>        sGarrBuildingPlotInstStore;
extern DB2Storage<GarrClassSpecEntry>               sGarrClassSpecStore;
extern DB2Storage<GarrEncounterEntry>               sGarrEncounterStore;
extern DB2Storage<GarrEncounterXMechanicEntry>      sGarrEncounterXMechanicStore;
extern DB2Storage<GarrFollowerEntry>                sGarrFollowerStore;
extern DB2Storage<GarrFollowerLevelXPEntry>         sGarrFollowerLevelXPStore;
extern DB2Storage<GarrFollowerQualityEntry>         sGarrFollowerQualityStore;
extern DB2Storage<GarrFollowerXAbilityEntry>        sGarrFollowerXAbilityStore;
extern DB2Storage<GarrMechanicEntry>                sGarrMechanicStore;
extern DB2Storage<GarrMechanicTypeEntry>            sGarrMechanicTypeStore;
extern DB2Storage<GarrMissionEntry>                 sGarrMissionStore;
extern DB2Storage<GarrMissionRewardEntry>           sGarrMissionRewardStore;
extern DB2Storage<GarrMissionXEncounterEntry>       sGarrMissionXEncounterStore;
extern DB2Storage<GarrPlotBuildingEntry>            sGarrPlotBuildingStore;
extern DB2Storage<GarrPlotEntry>                    sGarrPlotStore;
extern DB2Storage<GarrPlotInstanceEntry>            sGarrPlotInstanceStore;
extern DB2Storage<GarrSiteLevelEntry>               sGarrSiteLevelStore;
extern DB2Storage<GarrSiteLevelPlotInstEntry>       sGarrSiteLevelPlotInstStore;
extern DB2Storage<GemPropertiesEntry>               sGemPropertiesStore;
extern DB2Storage<GlyphPropertiesEntry>             sGlyphPropertiesStore;
extern DB2Storage<GlyphSlotEntry>                   sGlyphSlotStore;
extern DB2Storage<GuildPerkSpellsEntry>             sGuildPerkSpellsStore;
extern DB2Storage<HeirloomEntry>                    sHeirloomStore;
extern DB2Storage<HolidaysEntry>                    sHolidaysStore;
extern DB2Storage<ImportPriceArmorEntry>            sImportPriceArmorStore;
extern DB2Storage<ImportPriceQualityEntry>          sImportPriceQualityStore;
extern DB2Storage<ImportPriceShieldEntry>           sImportPriceShieldStore;
extern DB2Storage<ImportPriceWeaponEntry>           sImportPriceWeaponStore;
extern DB2Storage<ItemAppearanceEntry>              sItemAppearanceStore;
extern DB2Storage<ItemArmorQualityEntry>            sItemArmorQualityStore;
extern DB2Storage<ItemArmorShieldEntry>             sItemArmorShieldStore;
extern DB2Storage<ItemArmorTotalEntry>              sItemArmorTotalStore;
extern DB2Storage<ItemClassEntry>                   sItemClassStore;
extern DB2Storage<ItemCurrencyCostEntry>            sItemCurrencyCostStore;
extern DB2Storage<ItemDamageEntry>                  sItemDamageAmmoStore;
extern DB2Storage<ItemDamageEntry>                  sItemDamageOneHandCasterStore;
extern DB2Storage<ItemDamageEntry>                  sItemDamageOneHandStore;
extern DB2Storage<ItemDamageEntry>                  sItemDamageTwoHandCasterStore;
extern DB2Storage<ItemDamageEntry>                  sItemDamageTwoHandStore;
extern DB2Storage<ItemDisenchantLootEntry>          sItemDisenchantLootStore;
extern DB2Storage<ItemEffectEntry>                  sItemEffectStore;
extern DB2Storage<ItemEntry>                        sItemStore;
extern DB2Storage<ItemExtendedCostEntry>            sItemExtendedCostStore;
extern DB2Storage<ItemLimitCategoryEntry>           sItemLimitCategoryStore;
extern DB2Storage<ItemPriceBaseEntry>               sItemPriceBaseStore;
extern DB2Storage<ItemRandomPropertiesEntry>        sItemRandomPropertiesStore;
extern DB2Storage<ItemRandomSuffixEntry>            sItemRandomSuffixStore;
extern DB2Storage<ItemSetEntry>                     sItemSetStore;
extern DB2Storage<ItemSparseEntry>                  sItemSparseStore;
extern DB2Storage<ItemSpecEntry>                    sItemSpecStore;
extern DB2Storage<ItemSpecOverrideEntry>            sItemSpecOverrideStore;
extern DB2Storage<ItemToBattlePetSpeciesEntry>      sItemToBattlePetSpeciesStore;
extern DB2Storage<ItemUpgradeEntry>                 sItemUpgradeStore;
extern DB2Storage<KeyChainEntry>                    sKeyChainStore;
extern DB2Storage<LFGDungeonEntry>                  sLFGDungeonStore;
extern DB2Storage<LiquidTypeEntry>                  sLiquidTypeStore;
extern DB2Storage<LockEntry>                        sLockStore;
extern DB2Storage<MailTemplateEntry>                sMailTemplateStore;
extern DB2Storage<MapChallengeModeEntry>            sMapChallengeModeStore;
extern DB2Storage<MapDifficultyEntry>               sMapDifficultyStore;
extern DB2Storage<MapEntry>                         sMapStore;
extern DB2Storage<MinorTalentEntry>                 sMinorTalentStore;
extern DB2Storage<ModifierTreeEntry>                sModifierTreeStore;
extern DB2Storage<MountCapabilityEntry>             sMountCapabilityStore;
extern DB2Storage<MountTypeEntry>                   sMountTypeStore;
extern DB2Storage<MovieEntry>                       sMovieStore;
extern DB2Storage<OverrideSpellDataEntry>           sOverrideSpellDataStore;
extern DB2Storage<PhaseEntry>                       sPhaseStores;
extern DB2Storage<PowerDisplayEntry>                sPowerDisplayStore;
extern DB2Storage<PvpItemEntry>                     sPvpItemStore;
extern DB2Storage<QuestFactionRewEntry>             sQuestFactionRewardStore;
extern DB2Storage<QuestLineEntry>                   sQuestLineStore;
extern DB2Storage<QuestLineXQuestEntry>             sQuestLineXQuestStore;
extern DB2Storage<QuestMoneyRewardEntry>            sQuestMoneyRewardStore;
extern DB2Storage<QuestObjectiveEntry>              sQuestObjectiveStore;
extern DB2Storage<QuestSortEntry>                   sQuestSortStore;
extern DB2Storage<QuestXPEntry>                     sQuestXPStore;
extern DB2Storage<RandPropPointsEntry>              sRandPropPointsStore;
extern DB2Storage<ResearchBranchEntry>              sResearchBranchStore;
extern DB2Storage<ResearchProjectEntry>             sResearchProjectStore;
extern DB2Storage<RuleSetItemUpgradeEntry>          sRuleSetItemUpgradeEntryStore;
extern DB2Storage<ScalingStatDistributionEntry>     sScalingStatDistributionStore;
extern DB2Storage<ScenarioEntry>                    sScenarioStore;
extern DB2Storage<ScenarioStepEntry>                sScenarioStepStore;
extern DB2Storage<SkillLineAbilityEntry>            sSkillLineAbilityStore;
extern DB2Storage<SkillLineEntry>                   sSkillLineStore;
extern DB2Storage<SkillRaceClassInfoEntry>          sSkillRaceClassInfoStore;
extern DB2Storage<SoundEntriesEntry>                sSoundEntriesStore;
extern DB2Storage<SpecializationSpellEntry>         sSpecializationSpellStore;
extern DB2Storage<SpellAuraOptionsEntry>            sSpellAuraOptionsStore;
extern DB2Storage<SpellAuraRestrictionsEntry>       sSpellAuraRestrictionsStore;
extern DB2Storage<SpellCastingRequirementsEntry>    sSpellCastingRequirementsStore;
extern DB2Storage<SpellCastTimesEntry>              sSpellCastTimesStore;
extern DB2Storage<SpellCategoriesEntry>             sSpellCategoriesStore;
extern DB2Storage<SpellCategoryEntry>               sSpellCategoryStores;
extern DB2Storage<SpellClassOptionsEntry>           sSpellClassOptionsStore;
extern DB2Storage<SpellCooldownsEntry>              sSpellCooldownsStore;
extern DB2Storage<SpellDurationEntry>               sSpellDurationStore;
extern DB2Storage<SpellEffectScalingEntry>          sSpellEffectScalingStore;
extern DB2Storage<SpellEntry>                       sSpellStore;
extern DB2Storage<SpellEquippedItemsEntry>          sSpellEquippedItemsStore;
extern DB2Storage<SpellFocusObjectEntry>            sSpellFocusObjectStore;
extern DB2Storage<SpellInterruptsEntry>             sSpellInterruptsStore;
extern DB2Storage<SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore;
extern DB2Storage<SpellItemEnchantmentEntry>        sSpellItemEnchantmentStore;
extern DB2Storage<SpellLearnSpellEntry>             sSpellLearnSpellStore;
extern DB2Storage<SpellLevelsEntry>                 sSpellLevelsStore;
extern DB2Storage<SpellMiscDifficultyEntry>         sSpellMiscDifficultyStore;
extern DB2Storage<SpellMiscEntry>                   sSpellMiscStore;
extern DB2Storage<SpellPowerEntry>                  sSpellPowerStore;
extern DB2Storage<SpellProcsPerMinuteEntry>         sSpellProcsPerMinuteStore;
extern DB2Storage<SpellProcsPerMinuteModEntry>      sSpellProcsPerMinuteModStore;
extern DB2Storage<SpellRadiusEntry>                 sSpellRadiusStore;
extern DB2Storage<SpellRangeEntry>                  sSpellRangeStore;
extern DB2Storage<SpellReagentsCurrencyEntry>       sSpellReagentsCurrencyStore;
extern DB2Storage<SpellReagentsEntry>               sSpellReagentsStore;
extern DB2Storage<SpellScalingEntry>                sSpellScalingStore;
extern DB2Storage<SpellShapeshiftEntry>             sSpellShapeshiftStore;
extern DB2Storage<SpellShapeshiftFormEntry>         sSpellShapeshiftFormStore;
extern DB2Storage<SpellTargetRestrictionsEntry>     sSpellTargetRestrictionsStore;
extern DB2Storage<SpellTotemsEntry>                 sSpellTotemsStore;
extern DB2Storage<SpellVisualEntry>                 sSpellVisualStore;
extern DB2Storage<SpellXSpellVisualEntry>           sSpellXSpellVisualStore;
extern DB2Storage<SummonPropertiesEntry>            sSummonPropertiesStore;
extern DB2Storage<TalentEntry>                      sTalentStore;
extern DB2Storage<TaxiNodesEntry>                   sTaxiNodesStore;
extern DB2Storage<TaxiPathEntry>                    sTaxiPathStore;
extern DB2Storage<UnitPowerBarEntry>                sUnitPowerBarStore;
extern DB2Storage<VehicleEntry>                     sVehicleStore;
extern DB2Storage<VehicleSeatEntry>                 sVehicleSeatStore;
extern DB2Storage<WMOAreaTableEntry>                sWMOAreaTableStore;
extern DB2Storage<WorldMapAreaEntry>                sWorldMapAreaStore;
extern DB2Storage<WorldMapOverlayEntry>             sWorldMapOverlayStore;
extern DB2Storage<WorldMapTransformsEntry>          sWorldMapTransformsStore;
extern DB2Storage<WorldSafeLocsEntry>               sWorldSafeLocsStore;

extern TaxiMask                                     sTaxiNodesMask;
extern TaxiMask                                     sOldContinentsNodesMask;
extern TaxiMask                                     sHordeTaxiNodesMask;
extern TaxiMask                                     sAllianceTaxiNodesMask;
extern TaxiPathSetBySource                          sTaxiPathSetBySource;
extern TaxiPathNodesByPath                          sTaxiPathNodesByPath;

struct ResearchPOIPoint
{
    ResearchPOIPoint(int32 _x = 0, int32 _y = 0) : x(_x), y(_y) { }
    int32 x, y;
};

typedef std::vector<ResearchPOIPoint> ResearchPOIPointVector;

struct DigSitePosition
{
    DigSitePosition(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) { }
    float x, y;
};

typedef std::vector<DigSitePosition> DigSitePositionVector;

struct ResearchSiteData
{
    ResearchSiteData() : zone(0), level(0xFF), branch_id(0), entry(nullptr) { }

    ResearchSiteEntry const* entry;
    uint16 zone;
    uint8 level;
    uint8 branch_id;
    ResearchPOIPointVector points;
    DigSitePositionVector digSites;
};

void LoadDB2Stores(std::string const& dataPath);

struct HotfixNotify
{
    uint32 TableHash;
    uint32 Timestamp;
    uint32 Entry;
};

typedef std::vector<HotfixNotify> HotfixData;

struct SpellEffect
{
    SpellEffect()
    {
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; i++)
            effects[i] = nullptr;
    }

    SpellEffectEntry const* effects[MAX_SPELL_EFFECTS];
};

struct WMOAreaTableTripple
{
    WMOAreaTableTripple(int32 r, int32 a, int32 g) : groupId(g), rootId(r), adtId(a) { }

    bool operator <(const WMOAreaTableTripple& b) const { return memcmp(this, &b, sizeof(WMOAreaTableTripple)) < 0; }

    // ordered by entropy; that way memcmp will have a minimal medium runtime
    int32 groupId;
    int32 rootId;
    int32 adtId;
};

#define DEFINE_DB2_SET_COMPARATOR(structure) \
    struct structure ## Comparator : public std::binary_function<structure const*, structure const*, bool> \
    { \
        bool operator()(structure const* left, structure const* right) const { return Compare(left, right); } \
        static bool Compare(structure const* left, structure const* right); \
    };

class DB2Manager
{
public:
    DEFINE_DB2_SET_COMPARATOR(MountTypeXCapabilityEntry);
    DEFINE_DB2_SET_COMPARATOR(ChrClassesXPowerTypesEntry);

    typedef std::map<uint32 /*hash*/, DB2StorageBase*> StorageMap;

    typedef std::map<uint32 /*curveID*/, std::map<uint32/*index*/, CurvePointEntry const*, std::greater<uint32>>> HeirloomCurvesContainer;
    typedef std::vector<ItemBonusEntry const*> ItemBonusContainer;
    typedef std::vector<ItemXBonusTreeEntry const*> ItemXBonusTreeContainer;
    typedef std::unordered_map<uint32 /*bonusListId*/, ItemBonusContainer> ItemBonusListContainer;
    typedef std::unordered_map<uint32 /*itemId*/, ItemXBonusTreeContainer> ItemToBonusTreeContainer;
    typedef std::unordered_map<uint32 /*itemId | appearanceMod << 24*/, uint32> ItemDisplayIdContainer;
    typedef std::unordered_map<uint32, std::set<ItemBonusTreeNodeEntry const*>> ItemBonusTreeContainer;
    typedef std::unordered_map<uint32, std::vector<ItemSpecOverrideEntry const*>> ItemSpecOverridesContainer;
    typedef std::unordered_map<uint32, MountEntry const*> MountContainer;
    typedef std::set<MountTypeXCapabilityEntry const*, MountTypeXCapabilityEntryComparator> MountTypeXCapabilitySet;
    typedef std::unordered_map<uint32, MountTypeXCapabilitySet> MountCapabilitiesByTypeContainer;
    typedef std::map<uint32 /*word length*/, StringVector> LanguageWordsContainer;
    typedef std::unordered_map<uint32 /*areaGroupId*/, std::vector<uint32/*areaId*/>> AreaGroupMemberContainer;
    typedef std::unordered_map<uint32, MapChallengeModeEntry const*> MapChallengeModeEntryContainer;
    typedef std::unordered_map<uint32, ItemUpgradeData> ItemUpgradeDataContainer;
    typedef std::unordered_map<uint32, std::vector<QuestPackageItemEntry const*>> QuestPackageItemContainer;
    typedef std::vector<uint32> ToyItemIdsContainer;
    typedef std::unordered_map<uint32, HeirloomEntry const*> HeirloomItemsContainer;
    typedef std::map<uint32, BattlePetSpeciesEntry const*> BattlePetSpeciesBySpellIdContainer;
    typedef std::multimap<uint32, std::pair<uint32, uint8>> BattlePetTurnByAbilityIdContainer;
    typedef std::multimap<uint32, BattlePetSpeciesXAbilityEntry const*> BattlePetXAbilityEntryBySpecIdContainer;
    typedef std::map<uint32, uint8> BattlePetQualityMultiplierContainer;
    typedef std::map<uint32, BattlePetAbilityEffectEntry const*> BattlePetEffectEntryByTurnIdContainer;
    typedef std::unordered_map<BattlePetState /*state*/, int32 /*value*/, std::hash<std::underlying_type<BattlePetState>::type> > BattlePetStateMapContainer;
    typedef std::unordered_map<uint16 /*BreedID*/, BattlePetStateMapContainer> BattlePetBreedStatesContainer;
    typedef std::unordered_map<uint32 /*SpeciesID*/, BattlePetStateMapContainer> BattlePetSpeciesStatesContainer;
    typedef std::unordered_map<uint32 /*level*/, uint32 /*nextLevelXP*/> GarrFollowerLevelXPContainer;
    typedef std::unordered_map<uint32 /*quality*/, uint32 /*nextQualityXP*/> GarrFollowerQualityXPContainer;
    typedef std::unordered_map<uint32 /*missionID*/, GarrMissionRewardEntry const*> GarrMissionRewardByMissionIDContainer;
    typedef std::unordered_map<uint32, AchievementEntry const*> AchievementParentContainer;
    typedef std::map<uint32 /*LoadOutID*/, std::vector<uint32>> CharacterLoadoutItemContainer;
    typedef std::unordered_map<uint32 /*Parent*/, std::vector<CriteriaTreeEntry const*>> CriteriaTreeContainer;
    typedef std::vector<ItemSetSpellEntry const*> ItemSetSpells;
    typedef std::unordered_map<uint32, ItemSetSpells> ItemSetSpellsContainer;
    typedef std::unordered_map<uint32, std::list<uint32>> ItemSpecsContainer;
    typedef std::unordered_map<uint32, std::vector<ModifierTreeEntry const*>> ModifierTreeContainer;
    typedef std::unordered_map<uint32, std::array<std::vector<NameGenEntry const*>, 2>> NameGenContainer;
    typedef std::array<std::vector<boost::regex>, MAX_LOCALES + 1> NameValidationRegexContainer;
    typedef std::set<ResearchProjectEntry const*> ResearchProjectContainer;
    typedef std::map<uint32 /*site_id*/, ResearchSiteData> ResearchSiteDataMap;
    typedef std::unordered_map<uint32 /*frame*/, TransportAnimationEntry const*> TransportAnimationEntryMap;
    typedef std::unordered_map<uint32, TransportAnimationEntryMap> TransportAnimationsByEntryContainer;
    typedef std::unordered_multimap<uint32, SkillRaceClassInfoEntry const*> SkillRaceClassInfoContainer;
    typedef std::pair<SkillRaceClassInfoContainer::iterator, SkillRaceClassInfoContainer::iterator> SkillRaceClassInfoBounds;
    typedef std::unordered_map<uint32, std::vector<SpecializationSpellEntry const*>> SpecializationSpellsBySpecContainer;
    typedef std::unordered_map<uint32, std::list<uint32>> SpellProcsPerMinuteModEntryListContainer;
    typedef std::unordered_map<uint32, std::set<SpellTargetRestrictionsEntry const*>> SpellRestrictionDiffContainer;
    typedef std::unordered_map<uint32, uint32> RevertLearnSpellContainer;
    typedef std::unordered_map<uint32, uint32> ReversTriggerSpellContainer;
    typedef std::unordered_map<uint16, SpellEffectEntry const*> SpellEffectsMap;
    struct SpellEffectDiff { SpellEffectsMap effects; };
    typedef std::unordered_map<uint32, SpellEffectDiff> SpellEffectDiffContainer;
    typedef std::unordered_map<uint32, SpellEffect> SpellEffectContainer;
    typedef std::set<uint32> SpellCategorySet;
    typedef std::unordered_map<uint32, SpellCategorySet> SpellCategoryContainer;
    typedef ChrSpecializationEntry const* ChrSpecializationByIndexContainer[MAX_CLASSES][4];
    typedef std::set<uint32> PetFamilySpellsSet;
    typedef std::unordered_map<uint32, PetFamilySpellsSet > PetFamilySpellsContainer;
    typedef std::map<uint32, AreaTableEntry const*> AreaEntryContainer;
    typedef std::map<uint32, DungeonEncounterEntry const*> DungeonEncounterByDisplayIDContainer;
    typedef MinorTalentEntry const* MinorTalentByIndexContainer[SPEC_MAX_SPEC_INDEX][4];
    typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficultyEntry const*>> MapDifficultyContainer;
    typedef std::unordered_map<uint32, uint32> SpellMiscDifficultyContainer;
    typedef std::unordered_map<uint16, uint16> QuestLineXQuestContainer;
    typedef std::multimap<uint32, CharSectionsEntry const*> CharSectionsContainer;
    typedef std::list<uint32> SimpleFactionsList;
    typedef std::map<uint32, SimpleFactionsList> FactionTeamContainer;
    typedef std::map<WMOAreaTableTripple, WMOAreaTableEntry const*> WMOAreaInfoByTrippleContainer;
    typedef std::vector<TalentEntry const*> TalentsByPositionContainer[MAX_CLASSES][7][3];

    static DB2Manager& Instance()
    {
        static DB2Manager instance;
        return instance;
    }

    void LoadStores(std::string const& dataPath, uint32 defaultLocale);
    void InitDB2CustomStores();
    DB2StorageBase const* GetStorage(uint32 type) const;

    void LoadHotfixData();
    HotfixData const* GetHotfixData() const { return &_hotfixData; }
    time_t GetHotfixDate(uint32 entry, uint32 type) const;

    std::vector<uint32> GetAreasForGroup(uint32 areaGroupId);
    std::list<uint32> GetGameObjectsList();
    ItemUpgradeData const* GetItemUpgradeData(uint32 itemEntry);
    uint32 GetHeirloomItemLevel(uint32 curveId, uint32 level) const;
    uint32 GetItemDisplayId(uint32 itemId, uint32 appearanceModId) const;
    ItemBonusContainer const* GetItemBonusList(uint32 bonusListId) const;
    LanguageWordsContainer const* GetLanguageWordMap(uint32 landID);
    StringVector const* GetLanguageWordsBySize(uint32 landID, uint32 size);
    std::vector<QuestPackageItemEntry const*> const* GetQuestPackageItems(uint32 questPackageID) const;
    MountEntry const* GetMount(uint32 spellId) const;
    MountEntry const* GetMountById(uint32 id) const;
    MountTypeXCapabilitySet const* GetMountCapabilities(uint32 mountType) const;
    std::set<uint32> GetItemBonusTree(uint32 itemId, uint32 itemBonusTreeMod) const;
    std::set<uint32> GetFindBonusTree(uint32 BonusTreeID, uint32 itemBonusTreeMod) const;
    HeirloomEntry const* GetHeirloomByItemId(uint32 itemId) const;
    HeirloomEntry const* GetHeirloomByOldItem(uint32 itemId) const;
    uint8 GetBattlePetSpeciesBySpellID(uint32 entry) const;
    float CalcBattlePetQualityMuliplier(uint8 quality, uint8 level);
    uint32 GetBattlePetTurnByAbility(uint32 abilityID, uint8 turnIndex = 1);
    uint32 GetBattlePetXAbilityEntryBySpec(uint32 speciesID, uint32 customAbility, uint8 rank);
    BattlePetSpeciesEntry const* GetBattlePetSpeciesEntry(uint32 creatureEntry);
    uint32 GetXPForNextFollowerLevel(uint32 level);
    uint32 GetXPForNextFollowerQuality(uint32 quality);
    GarrMissionRewardEntry const* GetMissionRewardByRecID(uint32 missionRecID);
    static char const* GetBroadcastTextValue(BroadcastTextEntry const* broadcastText, LocaleConstant locale = DEFAULT_LOCALE, uint8 gender = GENDER_MALE, bool forceGender = false);
    AchievementEntry const* GetsAchievementByTreeList(uint32 criteriaTree);
    std::vector<uint32> GetItemLoadOutItemsByClassID(uint32 classID);
    std::vector<CriteriaTreeEntry const*> const* GetCriteriaTreeList(uint32 parent);
    std::list<uint32> GetItemSpecsList(uint32 itemID);
    std::vector<ModifierTreeEntry const*> const* GetModifierTreeList(uint32 parent);
    std::string GetRandomCharacterName(uint8 race, uint8 gender, LocaleConstant locale) const;
    ResponseCodes ValidateName(std::string const& name, LocaleConstant locale) const;
    uint32 GetQuestUniqueBitFlag(uint32 questID);
    ResearchSiteEntry const* GetResearchSiteEntryById(uint32 id);
    void DeterminaAlternateMapPosition(uint32 mapId, float x, float y, float z, uint32* newMapId = nullptr, DBCPosition2D* newPos = nullptr);
    bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId);
    SkillRaceClassInfoEntry const* GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_);
    std::vector<SpecializationSpellEntry const*> const* GetSpecializationSpells(uint32 specId);
    std::list<uint32> const* GetSpellProcsPerMinuteModList(uint32 procID);
    SpellTargetRestrictionsEntry const* GetSpellTargetRestrioctions(uint32 spellid, uint16 difficulty);
    uint32 GetLearnSpell(uint32 trigerSpell);
    uint32 GetSpellByTrigger(uint32 trigerSpell);
    SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint8 difficulty);
    std::set<uint32> const* GetSpellCategory(uint32 category);
    std::vector<ItemSpecOverrideEntry const*> const* GetItemSpecOverrides(uint32 itemId) const;
    PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapID, uint32 level);
    PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapID, BattlegroundBracketId id);
    ChrSpecializationEntry const* GetChrSpecializationByID(uint8 classID, uint32 ID);
    PetFamilySpellsSet const* GetPetFamilySpells(uint32 family);
    uint32 GetPowerIndexByClass(uint32 powerType, uint32 classId) const;
    AreaTableEntry const* FindAreaEntry(uint32 area);
    uint32 GetParentZoneOrSelf(uint32 zone);
    char const* GetPetName(uint32 petfamily);
    MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty);
    MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapID);
    DungeonEncounterEntry const* GetDungeonEncounterByDisplayID(uint32 displayID);
    MinorTalentEntry const* GetMinorTalentBySpecAndPerkID(uint32 specID, uint32 perkID);
    MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty);
    uint32 GetSpellMisc(uint32 spellID, Difficulty difficultyID = DIFFICULTY_NONE);
    uint16 GetQuestIDbyLineAndPos(uint8 lineID, uint8 pos);
    QuestLineXQuestEntry const* GetQuestLineXQuestData(uint16 questID);
    CharSectionsEntry const* GetCharSectionEntry(uint8 race, CharSectionType genType, uint8 gender, uint8 type, uint8 color);
    SimpleFactionsList const* GetFactionTeamList(uint32 faction);
    WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid);
    uint32 GetLiquidFlags(uint32 liquidType);

    MapChallengeModeEntryContainer _mapChallengeModeEntrybyMap; // @TODO: move this to private and make special getters
    BattlePetBreedStatesContainer _battlePetBreedStates;
    BattlePetSpeciesStatesContainer _battlePetSpeciesStates;
    ItemSetSpellsContainer _itemSetSpells;
    ResearchProjectContainer _researchProjectContainer;
    ResearchSiteDataMap _researchSiteDataMap;
    TransportAnimationsByEntryContainer _transportAnimationsByEntry;
    std::set<uint32> sScenarioCriteriaTreeStore;
    MapDifficultyContainer _mapDifficulty;
    SpellCategoryContainer _spellCategory;
    TalentsByPositionContainer _talentByPos;
private:
    StorageMap _stores;
    HotfixData _hotfixData;

    HeirloomCurvesContainer _heirloomCurvePoints;
    ItemBonusListContainer _itemBonusLists;
    ItemBonusTreeContainer _itemBonusTrees;
    ItemDisplayIdContainer _itemDisplayIDs;
    ItemToBonusTreeContainer _itemToBonusTree;
    ItemSpecOverridesContainer _itemSpecOverrides;
    MountContainer _mountsBySpellId;
    MountCapabilitiesByTypeContainer _mountCapabilitiesByType;
    std::list<uint32> _gameObjectsList;
    std::map<uint32 /*landID*/, LanguageWordsContainer> _languageWordsMap;
    AreaGroupMemberContainer _areaGroupMembers;
    ItemUpgradeDataContainer _itemUpgradeDataMap;
    QuestPackageItemContainer _questPackages;
    ToyItemIdsContainer _toys;
    HeirloomItemsContainer _heirlooms;
    BattlePetQualityMultiplierContainer _battlePetQualityMultiplierId;
    BattlePetTurnByAbilityIdContainer _battlePetTurnByAbilityId;
    BattlePetXAbilityEntryBySpecIdContainer _battlePetXAbilityEntryBySpecId;
    BattlePetEffectEntryByTurnIdContainer _battlePetEffectEntryByTurnId;
    BattlePetSpeciesBySpellIdContainer _battlePetSpeciesBySpellId;
    GarrFollowerLevelXPContainer _garrFollowerLevelXP;
    GarrFollowerQualityXPContainer _garrFollowerQualityXP;
    GarrMissionRewardByMissionIDContainer _garrMissionRewardByMissionID;
    AchievementParentContainer _achievementParentList;
    CharacterLoadoutItemContainer _characterLoadoutItem;
    CriteriaTreeContainer _criteriaTree;
    ItemSpecsContainer _itemSpec;
    ModifierTreeContainer _modifierTree;
    NameGenContainer _nameGenData;
    NameValidationRegexContainer _nameValidators;
    SkillRaceClassInfoContainer _skillRaceClassInfoBySkill;
    SpecializationSpellsBySpecContainer _specializationSpellsBySpec;
    SpellProcsPerMinuteModEntryListContainer _spellProcsPerMinuteModEntryList;
    SpellRestrictionDiffContainer _spellRestrictionDiff;
    RevertLearnSpellContainer _revertLearnSpell;
    ReversTriggerSpellContainer _reversTriggerSpellList;
    SpellEffectDiffContainer _spellEffectDiff;
    SpellEffectContainer _spellEffectMap;
    ChrSpecializationByIndexContainer _chrSpecializationByIndex;
    PetFamilySpellsContainer _petFamilySpells;
    uint32 _powersByClass[MAX_CLASSES][MAX_POWERS];
    AreaEntryContainer _areaEntry;
    DungeonEncounterByDisplayIDContainer _dungeonEncounterByDisplayID;
    MinorTalentByIndexContainer _minorTalentByIndexStore;
    SpellMiscDifficultyContainer _spellMiscDifficulty;
    QuestLineXQuestContainer _questLineXQuest;
    CharSectionsContainer _charSection;
    FactionTeamContainer _factionTeam;
    WMOAreaInfoByTrippleContainer _WMOAreaInfoByTripple;
};

#define sDB2Manager DB2Manager::Instance()

#endif