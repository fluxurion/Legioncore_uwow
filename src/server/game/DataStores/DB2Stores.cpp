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

#include "DB2Stores.h"
#include "Common.h"
#include "Containers.h"
#include "DBCStores.h"
#include "DB2fmt.h"
#include "Log.h"
#include "World.h"

DB2Storage<AchievementEntry>                sAchievementStore("Achievement.db2", Achievementfmt, HOTFIX_SEL_ACHIEVEMENT);

DB2Storage<AreaGroupEntry>                  sAreaGroupStore("AreaGroup.db2", AreaGroupFormat, HOTFIX_SEL_AREA_GROUP);
DB2Storage<AreaGroupMemberEntry>            sAreaGroupMemberStore("AreaGroupMember.db2", AreaGroupMemberFormat, HOTFIX_SEL_AREA_GROUP_MEMBER);
DB2Storage<BattlePetAbilityEffectEntry>     sBattlePetAbilityEffectStore("BattlePetAbilityEffect.db2", BattlePetAbilityEffectFormat, HOTFIX_SEL_BATTLE_PET_ABILITY_EFFECT);
DB2Storage<BattlePetAbilityEntry>           sBattlePetAbilityStore("BattlePetAbility.db2", BattlePetAbilityFormat, HOTFIX_SEL_BATTLE_PET_ABILITY);
DB2Storage<BattlePetAbilityStateEntry>      sBattlePetAbilityStateStore("BattlePetAbilityState.db2", BattlePetAbilityStateFormat, HOTFIX_SEL_BATTLE_PET_ABILITY_STATE);
DB2Storage<BattlePetAbilityTurnEntry>       sBattlePetAbilityTurnStore("BattlePetAbilityTurn.db2", BattlePetAbilityTurnFormat, HOTFIX_SEL_BATTLE_PET_ABILITY_TURN);
DB2Storage<BattlePetBreedQualityEntry>      sBattlePetBreedQualityStore("BattlePetBreedQuality.db2", BattlePetBreedQualityFormat, HOTFIX_SEL_BATTLE_PET_BREED_QUALITY);
DB2Storage<BattlePetBreedStateEntry>        sBattlePetBreedStateStore("BattlePetBreedState.db2", BattlePetBreedStateFormat, HOTFIX_SEL_BATTLE_PET_BREED_STATE);
DB2Storage<BattlePetEffectPropertiesEntry>  sBattlePetEffectPropertiesStore("BattlePetEffectProperties.db2", BattlePetEffectPropertiesFormat, HOTFIX_SEL_BATTLE_PET_EFFECT_PROPERTIES);
DB2Storage<BattlePetSpeciesEntry>           sBattlePetSpeciesStore("BattlePetSpecies.db2", BattlePetSpeciesFormat, HOTFIX_SEL_BATTLE_PET_SPECIES);
DB2Storage<BattlePetSpeciesStateEntry>      sBattlePetSpeciesStateStore("BattlePetSpeciesState.db2", BattlePetSpeciesStateFormat, HOTFIX_SEL_BATTLE_PET_SPECIES_STATE);
DB2Storage<BattlePetSpeciesXAbilityEntry>   sBattlePetSpeciesXAbilityStore("BattlePetSpeciesXAbility.db2", BattlePetSpeciesXAbilityFormat, HOTFIX_SEL_BATTLE_PET_SPECIES_X_ABILITY);
DB2Storage<BattlePetStateEntry>             sBattlePetStateStore("BattlePetState.db2", BattlePetStateFormat, HOTFIX_SEL_BATTLE_PET_STATE);
DB2Storage<BroadcastTextEntry>              sBroadcastTextStore("BroadcastText.db2", BroadcastTextFormat, HOTFIX_SEL_BROADCAST_TEXT);
DB2Storage<CreatureDifficultyEntry>         sCreatureDifficultyStore("CreatureDifficulty.db2", CreatureDifficultyFormat, HOTFIX_SEL_CREATURE_DIFFICULTY);
DB2Storage<CurrencyTypesEntry>              sCurrencyTypesStore("CurrencyTypes.db2", CurrencyTypesFormat, HOTFIX_SEL_CURRENCY_TYPES);
DB2Storage<CurvePointEntry>                 sCurvePointStore("CurvePoint.db2", CurvePointFormat, HOTFIX_SEL_CURVE_POINT);
DB2Storage<GameObjectsEntry>                sGameObjectsStore("GameObjects.db2", GameObjectsFormat, HOTFIX_SEL_GAME_OBJECTS);
DB2Storage<GameTablesEntry>                 sGameTablesStore("GameTables.db2", GameTablesFmt, HOTFIX_SEL_GAME_TABLES);
DB2Storage<GarrAbilityEntry>                sGarrAbilityStore("GarrAbility.db2", GarrAbilityFormat, HOTFIX_SEL_GARR_ABILITY);
DB2Storage<GarrAbilityEffectEntry>          sGarrAbilityEffectStore("GarrAbilityEffect.db2", GarrAbilityEffectFormat, HOTFIX_SEL_GARR_ABILITY_EFFECT);
DB2Storage<GarrBuildingEntry>               sGarrBuildingStore("GarrBuilding.db2", GarrBuildingFormat, HOTFIX_SEL_GARR_BUILDING);
DB2Storage<GarrBuildingPlotInstEntry>       sGarrBuildingPlotInstStore("GarrBuildingPlotInst.db2", GarrBuildingPlotInstFormat, HOTFIX_SEL_GARR_BUILDING_PLOT_INST);
DB2Storage<GarrClassSpecEntry>              sGarrClassSpecStore("GarrClassSpec.db2", GarrClassSpecFormat, HOTFIX_SEL_GARR_CLASS_SPEC);
DB2Storage<GarrFollowerEntry>               sGarrFollowerStore("GarrFollower.db2", GarrFollowerFormat, HOTFIX_SEL_GARR_FOLLOWER);
DB2Storage<GarrFollowerLevelXPEntry>        sGarrFollowerLevelXPStore("GarrFollowerLevelXP.db2", GarrFollowerLevelXPFormat, HOTFIX_SEL_GARR_FOLLOWER_LEVEL_XP);
DB2Storage<GarrFollowerQualityEntry>        sGarrFollowerQualityStore("GarrFollowerQuality.db2", GarrFollowerQualityFormat, HOTFIX_SEL_GARR_FOLLOWER_QUALITY);
DB2Storage<GarrFollowerXAbilityEntry>       sGarrFollowerXAbilityStore("GarrFollowerXAbility.db2", GarrFollowerXAbilityFormat, HOTFIX_SEL_GARR_FOLLOWER_X_ABILITY);
DB2Storage<GarrEncounterEntry>              sGarrEncounterStore("GarrEncounter.db2", GarrEncounterFormat, HOTFIX_SEL_GARR_ENCOUNTER);
DB2Storage<GarrEncounterXMechanicEntry>     sGarrEncounterXMechanicStore("GarrEncounterXMechanic.db2", GarrEncounterXMechanicFormat, HOTFIX_SEL_GARR_ENCOUNTER_X_MECHANIC);
DB2Storage<GarrMechanicEntry>               sGarrMechanicStore("GarrMechanic.db2", GarrMechanicFormat, HOTFIX_SEL_GARR_MECHANIC);
DB2Storage<GarrMechanicTypeEntry>           sGarrMechanicTypeStore("GarrMechanicType.db2", GarrMechanicTypeFormat, HOTFIX_SEL_GARR_MECHANIC_TYPE);
DB2Storage<GarrMissionEntry>                sGarrMissionStore("GarrMission.db2", GarrMissionFormat, HOTFIX_SEL_GARR_MISSION);
DB2Storage<GarrMissionRewardEntry>          sGarrMissionRewardStore("GarrMissionReward.db2", GarrMissionRewardFormat, HOTFIX_SEL_GARR_MISSION_REWARD);
DB2Storage<GarrMissionXEncounterEntry>      sGarrMissionXEncounterStore("GarrMissionXEncounter.db2", GarrMissionXEncounterFormat, HOTFIX_SEL_GARR_MISSION_X_ENCOUNTER);
DB2Storage<GarrPlotBuildingEntry>           sGarrPlotBuildingStore("GarrPlotBuilding.db2", GarrPlotBuildingFormat, HOTFIX_SEL_GARR_PLOT_BUILDING);
DB2Storage<GarrPlotEntry>                   sGarrPlotStore("GarrPlot.db2", GarrPlotFormat, HOTFIX_SEL_GARR_PLOT);
DB2Storage<GarrPlotInstanceEntry>           sGarrPlotInstanceStore("GarrPlotInstance.db2", GarrPlotInstanceFormat, HOTFIX_SEL_GARR_PLOT_INSTANCE);
DB2Storage<GarrSiteLevelEntry>              sGarrSiteLevelStore("GarrSiteLevel.db2", GarrSiteLevelFormat, HOTFIX_SEL_GARR_SITE_LEVEL);
DB2Storage<GarrSiteLevelPlotInstEntry>      sGarrSiteLevelPlotInstStore("GarrSiteLevelPlotInst.db2", GarrSiteLevelPlotInstFormat, HOTFIX_SEL_GARR_SITE_LEVEL_PLOT_INST);
DB2Storage<HeirloomEntry>                   sHeirloomStore("Heirloom.db2", HeirloomFormat, HOTFIX_SEL_HEIRLOOM);
DB2Storage<HolidaysEntry>                   sHolidaysStore("Holidays.db2", HolidaysFormat, HOTFIX_SEL_HOLIDAYS);
DB2Storage<ItemAppearanceEntry>             sItemAppearanceStore("ItemAppearance.db2", ItemAppearanceFormat, HOTFIX_SEL_ITEM_APPEARANCE);
DB2Storage<ItemBonusEntry>                  sItemBonusStore("ItemBonus.db2", ItemBonusFormat, HOTFIX_SEL_ITEM_BONUS);
DB2Storage<ItemBonusTreeNodeEntry>          sItemBonusTreeNodeStore("ItemBonusTreeNode.db2", ItemBonusTreeNodeFormat, HOTFIX_SEL_ITEM_BONUS_TREE_NODE);
DB2Storage<ItemCurrencyCostEntry>           sItemCurrencyCostStore("ItemCurrencyCost.db2", ItemCurrencyCostFormat, HOTFIX_SEL_ITEM_CURRENCY_COST);
DB2Storage<ItemEffectEntry>                 sItemEffectStore("ItemEffect.db2", ItemEffectFormat, HOTFIX_SEL_ITEM_EFFECT);
DB2Storage<ItemEntry>                       sItemStore("Item.db2", ItemFormat, HOTFIX_SEL_ITEM);
DB2Storage<ItemExtendedCostEntry>           sItemExtendedCostStore("ItemExtendedCost.db2", ItemExtendedCostFormat, HOTFIX_SEL_ITEM_EXTENDED_COST);
DB2Storage<ItemModifiedAppearanceEntry>     sItemModifiedAppearanceStore("ItemModifiedAppearance.db2", ItemModifiedAppearanceFormat, HOTFIX_SEL_ITEM_MODIFIED_APPEARANCE);
DB2Storage<ItemSparseEntry>                 sItemSparseStore("Item-sparse.db2", ItemSparseFormat, HOTFIX_SEL_ITEM_SPARSE);
DB2Storage<ItemToBattlePetSpeciesEntry>     sItemToBattlePetSpeciesStore("ItemToBattlePetSpecies.db2", ItemToBattlePetSpeciesFormat, HOTFIX_SEL_ITEM_TO_BATTLE_PET_SPECIES);
DB2Storage<ItemUpgradeEntry>                sItemUpgradeStore("ItemUpgrade.db2", ItemUpgradeFormat, HOTFIX_SEL_ITEM_UPGRADE);
DB2Storage<ItemXBonusTreeEntry>             sItemXBonusTreeStore("ItemXBonusTree.db2", ItemXBonusTreeFormat, HOTFIX_SEL_ITEM_X_BONUS_TREE);
DB2Storage<KeyChainEntry>                   sKeyChainStore("KeyChain.db2", KeyChainFormat, HOTFIX_SEL_KEY_CHAIN);
DB2Storage<LanguageWordsEntry>              sLanguageWordsStore("LanguageWords.db2", LanguageWordsFormat, HOTFIX_SEL_LANGUAGE_WORDS);
DB2Storage<MapChallengeModeEntry>           sMapChallengeModeStore("MapChallengeMode.db2", MapChallengeModeFormat, HOTFIX_SEL_MAP_CHALLENGE_MODE);
//DB2Storage<MountCapabilityEntry>            sMountCapabilityStore("MountCapability.db2", MountCapabilityFormat, HOTFIX_SEL_MOUNT_CAPABILITY);
DB2Storage<MountEntry>                      sMountStore("Mount.db2", MountFormat, HOTFIX_SEL_MOUNT);
//DB2Storage<MountTypeXCapabilityEntry>       sMountTypeXCapabilityStore("MountTypeXCapability.db2", MountTypeXCapabilityFormat, HOTFIX_SEL_MOUNT_TYPE_X_CAPABILITY);
DB2Storage<OverrideSpellDataEntry>          sOverrideSpellDataStore("OverrideSpellData.db2", OverrideSpellDataFormat, HOTFIX_SEL_OVERRIDE_SPELL_DATA);
DB2Storage<PhaseGroupEntry>                 sPhaseGroupStore("PhaseXPhaseGroup.db2", PhaseGroupFormat, HOTFIX_SEL_PHASE_GROUP);
DB2Storage<PvpItemEntry>                    sPvpItemStore("PvpItem.db2", PvpItemFormat, HOTFIX_SEL_PVP_ITEM);
DB2Storage<QuestPackageItemEntry>           sQuestPackageItemStore("QuestPackageItem.db2", QuestPackageItemFormat, HOTFIX_SEL_QUEST_PACKAGE_ITEM);
DB2Storage<RuleSetItemUpgradeEntry>         sRuleSetItemUpgradeEntryStore("RulesetItemUpgrade.db2", RuleSetItemUpgradeFormat, HOTFIX_SEL_RULE_SET_ITEM_UPGRADE);
DB2Storage<SoundEntriesEntry>               sSoundEntriesStore("SoundEntries.db2", SoundEntriesFormat, HOTFIX_SEL_SOUND_ENTRIES);
DB2Storage<SpellAuraRestrictionsEntry>      sSpellAuraRestrictionsStore("SpellAuraRestrictions.db2", SpellAuraRestrictionsFormat, HOTFIX_SEL_SPELL_AURA_RESTRICTIONS);
DB2Storage<SpellCastingRequirementsEntry>   sSpellCastingRequirementsStore("SpellCastingRequirements.db2", SpellCastingRequirementsFormat, HOTFIX_SEL_SPELL_CASTING_REQUIREMENTS);
DB2Storage<SpellClassOptionsEntry>          sSpellClassOptionsStore("SpellClassOptions.db2", SpellClassOptionsFormat, HOTFIX_SEL_SPELL_CLASS_OPTIONS);
DB2Storage<SpellLearnSpellEntry>            sSpellLearnSpellStore("SpellLearnSpell.db2", SpellLearnSpellFormat, HOTFIX_SEL_SPELL_LEARN_SPELL);
DB2Storage<SpellMiscEntry>                  sSpellMiscStore("SpellMisc.db2", SpellMiscFormat, HOTFIX_SEL_SPELL_MISC);
DB2Storage<SpellPowerEntry>                 sSpellPowerStore("SpellPower.db2", SpellPowerFormat, HOTFIX_SEL_SPELL_POWER);
DB2Storage<SpellReagentsEntry>              sSpellReagentsStore("SpellReagents.db2", SpellReagentsFormat, HOTFIX_SEL_SPELL_REAGENTS);
DB2Storage<SpellRuneCostEntry>              sSpellRuneCostStore("SpellRuneCost.db2", SpellRuneCostFormat, HOTFIX_SEL_SPELL_RUNE_COST);
DB2Storage<SpellTotemsEntry>                sSpellTotemsStore("SpellTotems.db2", SpellTotemsFormat, HOTFIX_SEL_SPELL_TOTEMS);
DB2Storage<SpellVisualEntry>                sSpellVisualStore("SpellVisual.db2", SpellVisualFormat, HOTFIX_SEL_SPELL_VISUAL);
DB2Storage<TaxiNodesEntry>                  sTaxiNodesStore("TaxiNodes.db2", TaxiNodesFormat, HOTFIX_SEL_TAXI_NODES);
DB2Storage<TaxiPathEntry>                   sTaxiPathStore("TaxiPath.db2", TaxiPathFormat, HOTFIX_SEL_TAXI_PATH);
DB2Storage<TaxiPathNodeEntry>               sTaxiPathNodeStore("TaxiPathNode.db2", TaxiPathNodeFormat, HOTFIX_SEL_TAXI_PATH_NODE);
DB2Storage<ToyEntry>                        sToyStore("Toy.db2", ToyFormat, HOTFIX_SEL_TOY);

TaxiMask                                    sTaxiNodesMask;
TaxiMask                                    sOldContinentsNodesMask;
TaxiMask                                    sHordeTaxiNodesMask;
TaxiMask                                    sAllianceTaxiNodesMask;
TaxiPathSetBySource                         sTaxiPathSetBySource;
TaxiPathNodesByPath                         sTaxiPathNodesByPath;

uint32 DB2FilesCount = 0;
typedef std::list<std::string> DB2StoreProblemList;

static bool LoadDB2_assert_print(uint32 fsize, uint32 rsize, std::string const& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDB2(uint32& availableDb2Locales, DB2StoreProblemList& errlist, DB2Manager::StorageMap& stores, DB2Storage<T>* storage, std::string const& db2Path, uint32 defaultLocale)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage->GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage->GetFormat()), sizeof(T), storage->GetFileName().c_str()));

    ++DB2FilesCount;

    if (storage->Load(db2Path + localeNames[defaultLocale] + '/', defaultLocale))
    {
        storage->LoadFromDB();

        for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (availableDb2Locales & (1 << i))
                if (!storage->LoadStringsFrom((db2Path + localeNames[i] + '/'), i))
                    availableDb2Locales &= ~(1 << i);             // mark as not available for speedup next checks

            storage->LoadStringsFromDB(i);
        }
    }
    else
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE* f = fopen((db2Path + storage->GetFileName()).c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << storage->GetFileName() << " exists, and has " << storage->GetFieldCount() << " field(s) (expected " << strlen(storage->GetFormat())
                << "). Extracted file might be from wrong client version.";
            std::string buf = stream.str();
            errlist.push_back(buf);
            fclose(f);
        }
        else
            errlist.push_back(storage->GetFileName());
    }

    stores[storage->GetHash()] = storage;
}

void DB2Manager::LoadStores(std::string const& dataPath, uint32 defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string db2Path = dataPath + "dbc/";

    DB2StoreProblemList bad_db2_files;
    uint32 availableDb2Locales = 0xFF;

#define LOAD_DB2(store) LoadDB2(availableDb2Locales, bad_db2_files, _stores, &store, db2Path, defaultLocale)

    LOAD_DB2(sAchievementStore);
    //LOAD_DB2(sAreaGroupMemberStore);
    //LOAD_DB2(sAreaGroupStore);
    //LOAD_DB2(sArmorLocationStore,               "ArmorLocation.dbc");
    //LOAD_DB2(sAuctionHouseStore,                "AuctionHouse.dbc");
    //LOAD_DB2(sBankBagSlotPricesStore,           "BankBagSlotPrices.dbc");
    //LOAD_DB2(sBarberShopStyleStore,             "BarberShopStyle.dbc");
    //LOAD_DB2(sBattlePetAbilityEffectStore);
    //LOAD_DB2(sBattlePetAbilityStateStore);
    //LOAD_DB2(sBattlePetAbilityStore);
    //LOAD_DB2(sBattlePetAbilityTurnStore);
    //LOAD_DB2(sBattlePetBreedQualityStore);
    //LOAD_DB2(sBattlePetBreedStateStore);
    //LOAD_DB2(sBattlePetEffectPropertiesStore);
    //LOAD_DB2(sBattlePetSpeciesStateStore);
    //LOAD_DB2(sBattlePetSpeciesStore);
    //LOAD_DB2(sBattlePetSpeciesXAbilityStore);
    //LOAD_DB2(sBattlePetStateStore);
    //LOAD_DB2(sBroadcastTextStore);
    //LOAD_DB2(sCharacterLoadoutItemStore,        "CharacterLoadoutItem.dbc");
    //LOAD_DB2(sCharStartOutfitStore,             "CharStartOutfit.dbc");
    //LOAD_DB2(sChrPowerTypesStore,               "ChrClassesXPowerTypes.dbc");
    //LOAD_DB2(sChrRacesStore,                    "ChrRaces.dbc");
    //LOAD_DB2(sCreatureDifficultyStore);
    //LOAD_DB2(sCreatureDisplayInfoExtraStore,    "CreatureDisplayInfoExtra.dbc");
    //LOAD_DB2(sCreatureDisplayInfoStore,         "CreatureDisplayInfo.dbc");
    //LOAD_DB2(sCreatureSpellDataStore,           "CreatureSpellData.dbc");
    //LOAD_DB2(sCreatureTypeStore,                "CreatureType.dbc");
    //LOAD_DB2(sCriteriaStore,                    "Criteria.dbc");
    //LOAD_DB2(sCriteriaTreeStore,                "CriteriaTree.dbc");
    //LOAD_DB2(sCurrencyTypesStore);
    //LOAD_DB2(sCurvePointStore);
    //LOAD_DB2(sDestructibleModelDataStore,       "DestructibleModelData.dbc");
    //LOAD_DB2(sDurabilityCostsStore,             "DurabilityCosts.dbc");
    //LOAD_DB2(sDurabilityQualityStore,           "DurabilityQuality.dbc");
    //LOAD_DB2(sGameObjectDisplayInfoStore,       "GameObjectDisplayInfo.dbc");
    //LOAD_DB2(sGameObjectsStore);
    LOAD_DB2(sGameTablesStore);
    //LOAD_DB2(sGarrAbilityEffectStore);
    //LOAD_DB2(sGarrAbilityStore);
    //LOAD_DB2(sGarrBuildingPlotInstStore);
    //LOAD_DB2(sGarrBuildingStore);
    //LOAD_DB2(sGarrClassSpecStore);
    //LOAD_DB2(sGarrEncounterStore);
    //LOAD_DB2(sGarrEncounterXMechanicStore);
    //LOAD_DB2(sGarrFollowerLevelXPStore);
    //LOAD_DB2(sGarrFollowerQualityStore);
    //LOAD_DB2(sGarrFollowerStore);
    //LOAD_DB2(sGarrFollowerXAbilityStore);
    //LOAD_DB2(sGarrMechanicStore);
    //LOAD_DB2(sGarrMechanicTypeStore);
    //LOAD_DB2(sGarrMissionRewardStore);
    //LOAD_DB2(sGarrMissionStore);
    //LOAD_DB2(sGarrMissionXEncounterStore);
    //LOAD_DB2(sGarrPlotBuildingStore);
    //LOAD_DB2(sGarrPlotInstanceStore);
    //LOAD_DB2(sGarrPlotStore);
    //LOAD_DB2(sGarrSiteLevelPlotInstStore);
    //LOAD_DB2(sGarrSiteLevelStore);
    //LOAD_DB2(sGlyphSlotStore,                   "GlyphSlot.dbc");
    //LOAD_DB2(sGuildPerkSpellsStore,             "GuildPerkSpells.dbc");
    //LOAD_DB2(sHeirloomStore);
    //LOAD_DB2(sHolidaysStore);
    //LOAD_DB2(sImportPriceArmorStore,            "ImportPriceArmor.dbc");
    //LOAD_DB2(sImportPriceQualityStore,          "ImportPriceQuality.dbc");
    //LOAD_DB2(sImportPriceShieldStore,           "ImportPriceShield.dbc");
    //LOAD_DB2(sImportPriceWeaponStore,           "ImportPriceWeapon.dbc");
    //LOAD_DB2(sItemAppearanceStore);
    //LOAD_DB2(sItemArmorQualityStore,            "ItemArmorQuality.dbc");
    //LOAD_DB2(sItemArmorShieldStore,             "ItemArmorShield.dbc");
    //LOAD_DB2(sItemArmorTotalStore,              "ItemArmorTotal.dbc");
    //LOAD_DB2(sItemBagFamilyStore,               "ItemBagFamily.dbc");
    //LOAD_DB2(sItemBonusStore);
    //LOAD_DB2(sItemBonusTreeNodeStore);
    //LOAD_DB2(sItemClassStore,                   "ItemClass.dbc");
    //LOAD_DB2(sItemCurrencyCostStore);
    //LOAD_DB2(sItemDamageAmmoStore,              "ItemDamageAmmo.dbc");
    //LOAD_DB2(sItemDamageOneHandCasterStore,     "ItemDamageOneHandCaster.dbc");
    //LOAD_DB2(sItemDamageOneHandStore,           "ItemDamageOneHand.dbc");
    //LOAD_DB2(sItemDamageRangedStore,            "ItemDamageRanged.dbc");
    //LOAD_DB2(sItemDamageThrownStore,            "ItemDamageThrown.dbc");
    //LOAD_DB2(sItemDamageTwoHandCasterStore,     "ItemDamageTwoHandCaster.dbc");
    //LOAD_DB2(sItemDamageTwoHandStore,           "ItemDamageTwoHand.dbc");
    //LOAD_DB2(sItemDamageWandStore,              "ItemDamageWand.dbc");
    //LOAD_DB2(sItemDisenchantLootStore,          "ItemDisenchantLoot.dbc");
    //LOAD_DB2(sItemEffectStore);
    //LOAD_DB2(sItemExtendedCostStore);
    //LOAD_DB2(sItemLimitCategoryStore,           "ItemLimitCategory.dbc");
    //LOAD_DB2(sItemModifiedAppearanceStore);
    //LOAD_DB2(sItemPriceBaseStore,               "ItemPriceBase.dbc");
    //LOAD_DB2(sItemRandomPropertiesStore,        "ItemRandomProperties.dbc");
    //LOAD_DB2(sItemRandomSuffixStore,            "ItemRandomSuffix.dbc");
    //LOAD_DB2(sItemSetSpellStore,                "ItemSetSpell.dbc");
    //LOAD_DB2(sItemSparseStore);
    //LOAD_DB2(sItemSpecOverrideStore,            "ItemSpecOverride.dbc");
    //LOAD_DB2(sItemSpecStore,                    "ItemSpec.dbc");
    //LOAD_DB2(sItemStore);
    //LOAD_DB2(sItemToBattlePetSpeciesStore);
    //LOAD_DB2(sItemUpgradeStore);
    //LOAD_DB2(sItemXBonusTreeStore);
    //LOAD_DB2(sKeyChainStore);
    //LOAD_DB2(sLanguageWordsStore);
    //LOAD_DB2(sMailTemplateStore,                "MailTemplate.dbc");
    //LOAD_DB2(sMapChallengeModeStore);
    //LOAD_DB2(sModifierTreeStore,                "ModifierTree.dbc");
    //LOAD_DB2(sMountCapabilityStore);
    //LOAD_DB2(sMountCapabilityStore,             "MountCapability.dbc");
    //LOAD_DB2(sMountStore);
    //LOAD_DB2(sMountTypeStore,                   "MountType.dbc");
    //LOAD_DB2(sMountTypeXCapabilityStore);
    //LOAD_DB2(sNameGenStore,                     "NameGen.dbc");
    //LOAD_DB2(sOverrideSpellDataStore);
    //LOAD_DB2(sPhaseGroupStore);
    //LOAD_DB2(sPvpItemStore);
    //LOAD_DB2(sQuestFactionRewardStore,          "QuestFactionReward.dbc");
    //LOAD_DB2(sQuestPackageItemStore);
    //LOAD_DB2(sQuestPOIPointStore,               "QuestPOIPoint.dbc");
    //LOAD_DB2(sQuestSortStore,                   "QuestSort.dbc");
    //LOAD_DB2(sQuestV2Store,                     "QuestV2.dbc");
    //LOAD_DB2(sQuestXPStore,                     "QuestXP.dbc");
    //LOAD_DB2(sRandomPropertiesPointsStore,      "RandPropPoints.dbc");
    //LOAD_DB2(sResearchBranchStore,              "ResearchBranch.dbc");
    //LOAD_DB2(sResearchProjectStore,             "ResearchProject.dbc");
    //LOAD_DB2(sResearchSiteStore,                "ResearchSite.dbc");
    //LOAD_DB2(sRuleSetItemUpgradeEntryStore);
    //LOAD_DB2(sScalingStatDistributionStore,     "ScalingStatDistribution.dbc");
    //LOAD_DB2(sScenarioStepStore,                "ScenarioStep.dbc");
    //LOAD_DB2(sScenarioStore,                    "Scenario.dbc");
    //LOAD_DB2(sSkillLineAbilityStore,            "SkillLineAbility.dbc");
    //LOAD_DB2(sSkillLineStore,                   "SkillLine.dbc");
    //LOAD_DB2(sSkillRaceClassInfoStore,          "SkillRaceClassInfo.dbc");
    //LOAD_DB2(sSoundEntriesStore);
    //LOAD_DB2(sSpecializationSpellStore,         "SpecializationSpells.dbc");
    //LOAD_DB2(sSpellAuraOptionsStore,            "SpellAuraOptions.dbc");
    //LOAD_DB2(sSpellAuraRestrictionsStore);
    //LOAD_DB2(sSpellCastingRequirementsStore);
    //LOAD_DB2(sSpellCastTimesStore,              "SpellCastTimes.dbc");
    //LOAD_DB2(sSpellCategoriesStore,             "SpellCategories.dbc");
    //LOAD_DB2(sSpellCategoryStores,              "SpellCategory.dbc");
    //LOAD_DB2(sSpellClassOptionsStore);
    //LOAD_DB2(sSpellCooldownsStore,              "SpellCooldowns.dbc");
    //LOAD_DB2(sSpellDurationStore,               "SpellDuration.dbc");
    //LOAD_DB2(sSpellEffectScalingStore,          "SpellEffectScaling.dbc");
    //LOAD_DB2(sSpellEffectStore,                 "SpellEffect.dbc");
    //LOAD_DB2(sSpellEquippedItemsStore,          "SpellEquippedItems.dbc");
    //LOAD_DB2(sSpellFocusObjectStore,            "SpellFocusObject.dbc");
    //LOAD_DB2(sSpellInterruptsStore,             "SpellInterrupts.dbc");
    //LOAD_DB2(sSpellItemEnchantmentConditionStore, "SpellItemEnchantmentCondition.dbc");
    //LOAD_DB2(sSpellLearnSpellStore);
    //LOAD_DB2(sSpellLevelsStore,                 "SpellLevels.dbc");
    //LOAD_DB2(sSpellMiscStore);
    //LOAD_DB2(sSpellPowerStore);
    //LOAD_DB2(sSpellProcsPerMinuteModStore,      "SpellProcsPerMinuteMod.dbc");
    //LOAD_DB2(sSpellProcsPerMinuteStore,         "SpellProcsPerMinute.dbc");
    //LOAD_DB2(sSpellRadiusStore,                 "SpellRadius.dbc");
    //LOAD_DB2(sSpellRangeStore,                  "SpellRange.dbc");
    //LOAD_DB2(sSpellReagentsStore);
    //LOAD_DB2(sSpellRuneCostStore);
    //LOAD_DB2(sSpellScalingStore,                "SpellScaling.dbc");
    //LOAD_DB2(sSpellShapeshiftFormStore,         "SpellShapeshiftForm.dbc");
    //LOAD_DB2(sSpellShapeshiftStore,             "SpellShapeshift.dbc");
    //LOAD_DB2(sSpellStore,                       "Spell.dbc");
    //LOAD_DB2(sSpellTargetRestrictionsStore,     "SpellTargetRestrictions.dbc");
    //LOAD_DB2(sSpellTotemsStore);
    //LOAD_DB2(sSpellVisualStore);
    //LOAD_DB2(sTaxiNodesStore);
    //LOAD_DB2(sTaxiPathNodeStore);
    //LOAD_DB2(sTaxiPathStore);
    //LOAD_DB2(sTotemCategoryStore,               "TotemCategory.dbc");
    //LOAD_DB2(sToyStore);
    //LOAD_DB2(sTransportAnimationStore,          "TransportAnimation.dbc");
    //LOAD_DB2(sTransportRotationStore,           "TransportRotation.dbc");
    //LOAD_DB2(sUnitPowerBarStore,                "UnitPowerBar.dbc");
    //LOAD_DB2(sWorldMapOverlayStore,             "WorldMapOverlay.dbc");
    //LOAD_DB2(sWorldMapTransformsStore,          "WorldMapTransforms.dbc");

#undef LOAD_DB2

    InitDB2CustomStores();

    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdbc/%s/", DB2FilesCount, dataPath.c_str(), localeNames[defaultLocale]);
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount, str.c_str());
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DB2 data stores in %u ms", DB2FilesCount, GetMSTimeDiffToNow(oldMSTime));
}

void DB2Manager::InitDB2CustomStores()
{
    for (BattlePetSpeciesEntry const* entry : sBattlePetSpeciesStore)
        _battlePetSpeciesBySpellId[entry->CreatureEntry] = entry;

    for (BattlePetSpeciesStateEntry const* entry : sBattlePetSpeciesStateStore)
        _battlePetSpeciesStates[entry->SpeciesID][BattlePetState(entry->State)] = entry->Value;

    for (BattlePetBreedStateEntry const* entry : sBattlePetBreedStateStore)
        _battlePetBreedStates[entry->breedID][BattlePetState(entry->stateID)] = entry->stateModifier;

    for (BattlePetBreedQualityEntry const* entry : sBattlePetBreedQualityStore)
        _battlePetQualityMultiplierId[entry->quality] = entry->qualityModifier;

    for (BattlePetAbilityTurnEntry const* entry : sBattlePetAbilityTurnStore)
        _battlePetTurnByAbilityId.insert(BattlePetTurnByAbilityIdMap::value_type(entry->AbilityID, std::make_pair(entry->ID, entry->turnIndex)));

    for (BattlePetAbilityEffectEntry const* entry : sBattlePetAbilityEffectStore)
        _battlePetEffectEntryByTurnId[entry->TurnEntryID] = entry;

    for (BattlePetSpeciesXAbilityEntry const* entry : sBattlePetSpeciesXAbilityStore)
        _battlePetXAbilityEntryBySpecId.insert(BattlePetXAbilityEntryBySpecIdMap::value_type(entry->speciesID, entry));

    for (GarrFollowerLevelXPEntry* const entry : sGarrFollowerLevelXPStore)
        _garrFollowerLevelXP[entry->Level] = entry->NextLevelXP;

    for (GarrFollowerQualityEntry* const entry : sGarrFollowerQualityStore)
        _garrFollowerQualityXP[entry->Quality] = entry->NextQualityXP;

    for (GarrMissionRewardEntry const* entry : sGarrMissionRewardStore)
        _garrMissionRewardByMissionID[entry->MissionID] = entry;

    for (LanguageWordsEntry const* entry : sLanguageWordsStore)
        sLanguageWordsMapStore[entry->langId][strlen(entry->word[DEFAULT_LOCALE].Str[DEFAULT_LOCALE])].push_back(entry->word[DEFAULT_LOCALE].Str[DEFAULT_LOCALE]);

    for (ToyEntry const* toy : sToyStore)
        _toys.push_back(toy->ItemID);

    for (HeirloomEntry const* heirloom : sHeirloomStore)
        _heirlooms[heirloom->ItemID] = heirloom;

    for (AreaGroupMemberEntry const* areaGroupMember : sAreaGroupMemberStore)
        _areaGroupMembers[areaGroupMember->AreaGroupID].push_back(areaGroupMember->AreaID);

    for (ItemBonusEntry const* bonus : sItemBonusStore)
        _itemBonusLists[bonus->BonusListID].push_back(bonus);

    for (ItemBonusTreeNodeEntry const* bonusTreeNode : sItemBonusTreeNodeStore)
        _itemBonusTrees[bonusTreeNode->BonusTreeID].insert(bonusTreeNode);

    for (ItemModifiedAppearanceEntry const* appearanceMod : sItemModifiedAppearanceStore)
        if (ItemAppearanceEntry const* appearance = sItemAppearanceStore.LookupEntry(appearanceMod->AppearanceID))
            _itemDisplayIDs[appearanceMod->ItemID | (appearanceMod->AppearanceModID << 24)] = appearance->DisplayID;

    {
        std::set<uint32> scalingCurves;
        for (ScalingStatDistributionEntry const* ssd : sScalingStatDistributionStore)
            scalingCurves.insert(ssd->ItemLevelCurveID);

        for (CurvePointEntry const* curvePoint : sCurvePointStore)
            if (scalingCurves.count(curvePoint->CurveID))
                _heirloomCurvePoints[curvePoint->CurveID][curvePoint->Index] = curvePoint;
    }

    for (MountEntry const* mount : sMountStore)
        _mountsBySpellId[mount->SpellId] = mount;

    //for (MountTypeXCapabilityEntry const* mount : sMountTypeXCapabilityStore)
        //_mountCapabilitiesByType[mount->MountTypeID].insert(mount);

    for (MapChallengeModeEntry const* entry : sMapChallengeModeStore)
        _mapChallengeModeEntrybyMap[entry->map] = entry;

    for (ItemXBonusTreeEntry const* itemBonusTreeAssignment : sItemXBonusTreeStore)
        _itemToBonusTree[itemBonusTreeAssignment->ItemID].push_back(itemBonusTreeAssignment);

    for (QuestPackageItemEntry const* questPackageItem : sQuestPackageItemStore)
        _questPackages[questPackageItem->QuestPackageID].push_back(questPackageItem);

    for (GameObjectsEntry const* store : sGameObjectsStore)
        sGameObjectsList.push_back(store->ID);

    for (RuleSetItemUpgradeEntry const* rsiu : sRuleSetItemUpgradeEntryStore)
    {
        ItemUpgradeDataMap::iterator itr = _itemUpgradeDataMap.find(rsiu->itemEntry);
        if (itr != _itemUpgradeDataMap.end())
            continue;

        ItemUpgradeData& data = _itemUpgradeDataMap[rsiu->itemEntry];

        uint32 offs = 0;
        uint32 prevUpd = 0;
        for (uint32 j = 0; j < sItemUpgradeStore.GetNumRows(); ++j)
        {
            ItemUpgradeEntry const* ue = sItemUpgradeStore.LookupEntry(j);
            if (!ue)
                continue;

            if (!prevUpd)
            {
                if (ue->id == rsiu->startUpgrade)
                {
                    prevUpd = ue->id;
                    data.upgrade[offs++] = ue;
                    j = 0;
                }
            }
            else if (ue->prevUpgradeId == prevUpd)
            {
                prevUpd = ue->id;
                data.upgrade[offs++] = ue;
                j = 0;
            }
        }
    }


    for (TaxiPathEntry const* entry : sTaxiPathStore)
        sTaxiPathSetBySource[entry->From][entry->To] = TaxiPathBySourceAndDestination(entry->ID, entry->Cost);

    uint32 pathCount = sTaxiPathStore.GetNumRows();

    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           // 0 and some other indexes not used
    for (TaxiPathNodeEntry const* entry : sTaxiPathNodeStore)
        if (pathLength[entry->PathID] < entry->NodeIndex + 1)
            pathLength[entry->PathID] = entry->NodeIndex + 1;

    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for (size_t i = 0; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);

    // fill data
    for (TaxiPathNodeEntry const* entry : sTaxiPathNodeStore)
        sTaxiPathNodesByPath[entry->PathID][entry->NodeIndex] = entry;

    // Initialize global taxinodes mask
    // include existed nodes that have at least single not spell base (scripted) path
    {
        if (sTaxiNodesStore.GetNumRows())
        {
            ASSERT(TaxiMaskSize >= ((sTaxiNodesStore.GetNumRows() - 1) / 8) + 1,
                "TaxiMaskSize is not large enough to contain all taxi nodes! (current value %d, required %d)",
                TaxiMaskSize, (((sTaxiNodesStore.GetNumRows() - 1) / 8) + 1));
        }

        sTaxiNodesMask.fill(0);
        sOldContinentsNodesMask.fill(0);
        sHordeTaxiNodesMask.fill(0);
        sAllianceTaxiNodesMask.fill(0);
        for (TaxiNodesEntry const* node : sTaxiNodesStore)
        {
            if (!(node->Flags & (TAXI_NODE_FLAG_ALLIANCE | TAXI_NODE_FLAG_HORDE)))
                continue;

            // valid taxi network node
            uint8  field = (uint8)((node->ID - 1) / 8);
            uint32 submask = 1 << ((node->ID - 1) % 8);

            sTaxiNodesMask[field] |= submask;
            if (node->Flags & TAXI_NODE_FLAG_HORDE)
                sHordeTaxiNodesMask[field] |= submask;
            if (node->Flags & TAXI_NODE_FLAG_ALLIANCE)
                sAllianceTaxiNodesMask[field] |= submask;

            uint32 nodeMap;
            DeterminaAlternateMapPosition(node->MapID, node->Pos.X, node->Pos.Y, node->Pos.Z, &nodeMap);
            if (nodeMap < 2)
                sOldContinentsNodesMask[field] |= submask;
        }
    }

    for (AchievementEntry const* store : sAchievementStore)
        if (store->CriteriaTree)
            _achievementParentList[store->CriteriaTree] = store;
}

DB2StorageBase const* DB2Manager::GetStorage(uint32 type) const
{
    StorageMap::const_iterator itr = _stores.find(type);
    if (itr != _stores.end())
        return itr->second;

    return nullptr;
}

void DB2Manager::LoadHotfixData()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = HotfixDatabase.Query("SELECT TableHash, RecordID, `Timestamp`, Deleted FROM hotfix_data");

    if (!result)
        return;

    uint32 count = 0;

    _hotfixData.reserve(result->GetRowCount());

    do
    {
        Field* fields = result->Fetch();

        HotfixNotify info;
        info.TableHash = fields[0].GetUInt32();
        info.Entry = fields[1].GetUInt32();
        info.Timestamp = fields[2].GetUInt32();
        _hotfixData.push_back(info);

        if (fields[3].GetBool())
        {
            auto itr = _stores.find(info.TableHash);
            if (itr != _stores.end())
                itr->second->EraseRecord(info.Entry);
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u hotfix info entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

time_t DB2Manager::GetHotfixDate(uint32 entry, uint32 type) const
{
    time_t ret = 0;
    for (HotfixNotify const& hotfix : _hotfixData)
        if (hotfix.Entry == entry && hotfix.TableHash == type)
            if (time_t(hotfix.Timestamp) > ret)
                ret = time_t(hotfix.Timestamp);

    return ret ? ret : time(nullptr);
}

std::vector<uint32> DB2Manager::GetAreasForGroup(uint32 areaGroupId)
{
    auto itr = _areaGroupMembers.find(areaGroupId);
    if (itr != _areaGroupMembers.end())
        return itr->second;

    return std::vector<uint32>();
}

std::list<uint32> DB2Manager::GetGameObjectsList()
{
    return sGameObjectsList;
}

ItemUpgradeData const* DB2Manager::GetItemUpgradeData(uint32 itemEntry)
{
    ItemUpgradeDataMap::iterator itr = _itemUpgradeDataMap.find(itemEntry);
    if (itr == _itemUpgradeDataMap.end())
        return nullptr;

    return &itr->second;
}

uint32 DB2Manager::GetHeirloomItemLevel(uint32 curveId, uint32 level) const
{
    // Assuming linear item level scaling for heirlooms
    auto itr = _heirloomCurvePoints.find(curveId);
    if (itr == _heirloomCurvePoints.end())
        return 0;

    auto it2 = itr->second.begin(); // Highest scaling point
    if (level >= it2->second->X)
        return it2->second->Y;

    auto previousItr = it2++;
    for (; it2 != itr->second.end(); ++it2, ++previousItr)
        if (level >= it2->second->X)
            return uint32((previousItr->second->Y - it2->second->Y) / (previousItr->second->X - it2->second->X) * (float(level) - it2->second->X) + it2->second->Y);

    return uint32(previousItr->second->Y);  // Lowest scaling point
}

uint32 DB2Manager::GetItemDisplayId(uint32 itemId, uint32 appearanceModId) const
{
    if (!itemId)
        return 0;

    auto itr = _itemDisplayIDs.find(itemId | (appearanceModId << 24));
    if (itr != _itemDisplayIDs.end())
        return itr->second;

    // Fall back to unmodified appearance
    if (appearanceModId)
    {
        itr = _itemDisplayIDs.find(itemId);
        if (itr != _itemDisplayIDs.end())
            return itr->second;
    }

    return 0;
}

DB2Manager::ItemBonusList const* DB2Manager::GetItemBonusList(uint32 bonusListId) const
{
    auto itr = _itemBonusLists.find(bonusListId);
    if (itr != _itemBonusLists.end())
        return &itr->second;

    return nullptr;
}

DB2Manager::LanguageWordsMap const* DB2Manager::GetLanguageWordMap(uint32 langID)
{
    std::map<uint32, LanguageWordsMap>::const_iterator itr = sLanguageWordsMapStore.find(langID);
    return itr != sLanguageWordsMapStore.end() ? &itr->second : nullptr;
}

StringVector const* DB2Manager::GetLanguageWordsBySize(uint32 langID, uint32 size)
{
    LanguageWordsMap const* wordMap = GetLanguageWordMap(langID);
    if (!wordMap)
        return nullptr;

    std::map<uint32, StringVector>::const_iterator itr = wordMap->find(size);
    return itr != wordMap->end() ? &itr->second : nullptr;
}

std::vector<QuestPackageItemEntry const*> const* DB2Manager::GetQuestPackageItems(uint32 questPackageID) const
{
    auto itr = _questPackages.find(questPackageID);
    if (itr != _questPackages.end())
        return &itr->second;

    return nullptr;
}

MountEntry const* DB2Manager::GetMount(uint32 spellId) const
{
    auto itr = _mountsBySpellId.find(spellId);
    if (itr != _mountsBySpellId.end())
        return itr->second;

    return nullptr;
}

MountEntry const* DB2Manager::GetMountById(uint32 id) const
{
    return sMountStore.LookupEntry(id);
}

std::set<uint32> DB2Manager::GetItemBonusTree(uint32 itemId, uint32 itemBonusTreeMod) const
{
    std::set<uint32> bonusListIDs;
    auto itemIdRange = _itemToBonusTree.find(itemId);
    if (itemIdRange == _itemToBonusTree.end())
        return bonusListIDs;

    for (auto itemTreeItr : itemIdRange->second)
    {
        std::set<uint32> listBonus = GetFindBonusTree((*itemTreeItr).BonusTreeID, itemBonusTreeMod);
        bonusListIDs.insert(listBonus.begin(), listBonus.end());
    }

    return bonusListIDs;
}

std::set<uint32> DB2Manager::GetFindBonusTree(uint32 BonusTreeID, uint32 itemBonusTreeMod) const
{
    std::set<uint32> bonusListIDs;

    auto treeItr = _itemBonusTrees.find(BonusTreeID);
    if (treeItr == _itemBonusTrees.end())
        return bonusListIDs;

    for (auto bonusTreeNode : treeItr->second)
    {
        if (bonusTreeNode->BonusTreeModID == itemBonusTreeMod)
        {
            if (bonusTreeNode->BonusListID)
                bonusListIDs.insert(bonusTreeNode->BonusListID);
            else if (bonusTreeNode->SubTreeID)
            {
                std::set<uint32> listBonus = GetFindBonusTree(bonusTreeNode->SubTreeID, itemBonusTreeMod);
                bonusListIDs.insert(listBonus.begin(), listBonus.end());
            }
        }
    }

    return bonusListIDs;
}

HeirloomEntry const* DB2Manager::GetHeirloomByItemId(uint32 itemId) const
{
    auto itr = _heirlooms.find(itemId);
    if (itr != _heirlooms.end())
        return itr->second;

    return nullptr;
}

HeirloomEntry const* DB2Manager::GetHeirloomByOldItem(uint32 itemId) const
{
    for (HeirloomEntry const* heirloom : sHeirloomStore)
    {
        if (heirloom->OldItem[0] == itemId || heirloom->OldItem[1] == itemId)
            return heirloom;
    }

    return nullptr;
}

//bool DB2Manager::MountTypeXCapabilityEntryComparator::Compare(MountTypeXCapabilityEntry const* left, MountTypeXCapabilityEntry const* right)
//{
//    if (left->MountTypeID == right->MountTypeID)
//        return left->OrderIndex < right->OrderIndex;
//    return left->ID < right->ID;
//}

//DB2Manager::MountTypeXCapabilitySet const* DB2Manager::GetMountCapabilities(uint32 mountType) const
//{
//    auto itr = _mountCapabilitiesByType.find(mountType);
//    if (itr != _mountCapabilitiesByType.end())
//        return &itr->second;
//
//    return nullptr;
//}

BattlePetSpeciesEntry const* DB2Manager::GetBattlePetSpeciesEntry(uint32 creatureEntry)
{
    auto const it = _battlePetSpeciesBySpellId.find(creatureEntry);
    if (it != _battlePetSpeciesBySpellId.end())
        return it->second;

    return nullptr;
}

uint8 DB2Manager::GetBattlePetSpeciesBySpellID(uint32 entry) const
{
    auto const it = _battlePetSpeciesBySpellId.find(entry);
    if (it != _battlePetSpeciesBySpellId.end())
        return it->second->petType;

    return 1;
}

float DB2Manager::CalcBattlePetQualityMuliplier(uint8 quality, uint8 level)
{
    auto itr = _battlePetQualityMultiplierId.find(quality);
    if (itr != _battlePetQualityMultiplierId.end())
        return level * itr->second;

    return 0.0f;
}

uint32 DB2Manager::GetBattlePetTurnByAbility(uint32 abilityID, uint8 turnIndex /*= 1*/)
{
    auto bounds = _battlePetTurnByAbilityId.equal_range(abilityID);
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
    {
        uint8 _turnIndex = std::get<1>(itr->second);
        if (_turnIndex == turnIndex)
        {
            uint32 turnID = std::get<0>(itr->second);
            auto itr = _battlePetEffectEntryByTurnId.find(turnID);
            if (itr != _battlePetEffectEntryByTurnId.end())
                return itr->second->ID;
        }
    }

    return 0;
}

uint32 DB2Manager::GetBattlePetXAbilityEntryBySpec(uint32 speciesID, uint32 customAbility, uint8 rank)
{
    auto bounds = _battlePetXAbilityEntryBySpecId.equal_range(speciesID);
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
    {
        BattlePetSpeciesXAbilityEntry const* entry = itr->second;
        if (!entry)
            continue;

        if (entry->rank == rank && customAbility == entry->abilityID && entry->requiredLevel >= 10)
            return entry->abilityID;
    }

    return 0;
}

uint32 DB2Manager::GetXPForNextFollowerLevel(uint32 level)
{
    auto const it = _garrFollowerLevelXP.find(level);
    if (it != _garrFollowerLevelXP.end())
        return it->second;

    return 0;
}

uint32 DB2Manager::GetXPForNextFollowerQuality(uint32 quality)
{
    auto const it = _garrFollowerQualityXP.find(quality);
    if (it != _garrFollowerQualityXP.end())
        return it->second;

    return 0;
}

GarrMissionRewardEntry const* DB2Manager::GetMissionRewardByRecID(uint32 missionRecID)
{
    auto const it = _garrMissionRewardByMissionID.find(missionRecID);
    if (it != _garrMissionRewardByMissionID.end())
        return it->second;

    return nullptr;
}

char const* DB2Manager::GetBroadcastTextValue(BroadcastTextEntry const* broadcastText, LocaleConstant locale /*= DEFAULT_LOCALE*/, uint8 gender /*= GENDER_MALE*/, bool forceGender /*= false*/)
{
    if (gender == GENDER_FEMALE && (forceGender || broadcastText->FemaleText->Str[DEFAULT_LOCALE][0] != '\0'))
    {
        if (broadcastText->FemaleText->Str[locale][0] != '\0')
            return broadcastText->FemaleText->Str[locale];

        return broadcastText->FemaleText->Str[DEFAULT_LOCALE];
    }

    if (broadcastText->MaleText->Str[locale][0] != '\0')
        return broadcastText->MaleText->Str[locale];

    return broadcastText->MaleText->Str[DEFAULT_LOCALE];
}

AchievementEntry const* DB2Manager::GetsAchievementByTreeList(uint32 criteriaTree)
{
    auto const& v = _achievementParentList.find(criteriaTree);
    if (v != _achievementParentList.end())
        return v->second;

    return nullptr;
}
