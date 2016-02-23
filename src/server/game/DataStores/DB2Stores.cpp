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

DB2Storage<AchievementEntry>                sAchievementStore("Achievement.db2", AchievementFormat, HOTFIX_SEL_ACHIEVEMENT);
DB2Storage<AreaGroupMemberEntry>            sAreaGroupMemberStore("AreaGroupMember.db2", AreaGroupMemberFormat, HOTFIX_SEL_AREA_GROUP_MEMBER);
DB2Storage<AreaTableEntry>                  sAreaTableStore("AreaTable.db2", AreaTableFormat, HOTFIX_SEL_AREA_TABLE);
DB2Storage<AreaTriggerEntry>                sAreaTriggerStore("AreaTrigger.db2", AreaTriggerFormat, HOTFIX_SEL_AREA_TRIGGER);
DB2Storage<ArmorLocationEntry>              sArmorLocationStore("ArmorLocation.db2", ArmorLocationFormat, HOTFIX_SEL_ARMOR_LOCATION);
DB2Storage<AuctionHouseEntry>               sAuctionHouseStore("AuctionHouse.db2", AuctionHouseFormat, HOTFIX_SEL_AUCTION_HOUSE);
DB2Storage<BankBagSlotPricesEntry>          sBankBagSlotPricesStore("BankBagSlotPrices.db2", BankBagSlotPricesFormat, HOTFIX_SEL_BANK_BAG_SLOT_PRICES);
DB2Storage<BarberShopStyleEntry>            sBarberShopStyleStore("BarberShopStyle.db2", BarberShopStyleFormat, HOTFIX_SEL_BARBER_SHOP_STYLE);
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
DB2Storage<CharacterLoadoutItemEntry>       sCharacterLoadoutItemStore("CharacterLoadoutItem.db2", CharacterLoadoutItemFormat, HOTFIX_SEL_CHARACTER_LOADOUT_ITEM);
DB2Storage<CharStartOutfitEntry>            sCharStartOutfitStore("CharStartOutfit.db2", CharStartOutfitFormat, HOTFIX_SEL_CHAR_START_OUTFIT);
DB2Storage<CharTitlesEntry>                 sCharTitlesStore("CharTitles.db2", CharTitlesFormat, HOTFIX_SEL_CHAR_TITLES);
DB2Storage<ChatChannelsEntry>               sChatChannelsStore("ChatChannels.db2", ChatChannelsEntryFormat, HOTFIX_SEL_CHAT_CHANNELS);
DB2Storage<ChrClassesXPowerTypesEntry>      sChrClassesXPowerTypesStore("ChrClassesXPowerTypes.db2", ChrClassesXPowerTypesFormat, HOTFIX_SEL_CHR_CLASSES_X_POWER_TYPES);
DB2Storage<ChrRacesEntry>                   sChrRacesStore("ChrRaces.db2", ChrRacesFormat, HOTFIX_SEL_CHR_RACES);
DB2Storage<ChrSpecializationEntry>          sChrSpecializationStore("ChrSpecialization.db2", ChrSpecializationFormat, HOTFIX_SEL_CHR_SPECIALIZATION);
DB2Storage<CreatureDifficultyEntry>         sCreatureDifficultyStore("CreatureDifficulty.db2", CreatureDifficultyFormat, HOTFIX_SEL_CREATURE_DIFFICULTY);
DB2Storage<CreatureDisplayInfoEntry>        sCreatureDisplayInfoStore("CreatureDisplayInfo.db2", CreatureDisplayInfoFormat, HOTFIX_SEL_CREATURE_DISPLAY_INFO);
DB2Storage<CreatureFamilyEntry>             sCreatureFamilyStore("CreatureFamily.db2", CreatureFamilyFormat, HOTFIX_SEL_CREATURE_FAMILY);
DB2Storage<CreatureTypeEntry>               sCreatureTypeStore("CreatureType.db2", CreatureTypeFormat, HOTFIX_SEL_CREATURE_TYPE);
DB2Storage<CriteriaEntry>                   sCriteriaStore("Criteria.db2", CriteriaFormat, HOTFIX_SEL_CRITERIA);
DB2Storage<CriteriaTreeEntry>               sCriteriaTreeStore("CriteriaTree.db2", CriteriaTreeFormat, HOTFIX_SEL_CRITERIA_TREE);
DB2Storage<CurrencyTypesEntry>              sCurrencyTypesStore("CurrencyTypes.db2", CurrencyTypesFormat, HOTFIX_SEL_CURRENCY_TYPES);
DB2Storage<CurvePointEntry>                 sCurvePointStore("CurvePoint.db2", CurvePointFormat, HOTFIX_SEL_CURVE_POINT);
DB2Storage<DestructibleModelDataEntry>      sDestructibleModelDataStore("DestructibleModelData.db2", DestructibleModelDataFormat, HOTFIX_SEL_DESTRUCTIBLE_MODEL_DATA);
DB2Storage<DifficultyEntry>                 sDifficultyStore("Difficulty.db2", DifficultyFormat, HOTFIX_SEL_DIFFICULTY);
DB2Storage<DungeonEncounterEntry>           sDungeonEncounterStore("DungeonEncounter.db2", DungeonEncounterFormat, HOTFIX_SEL_DUNGEON_ENCOUNTER);
DB2Storage<DurabilityCostsEntry>            sDurabilityCostsStore("DurabilityCosts.db2", DurabilityCostsFormat, HOTFIX_SEL_DURABILITY_COSTS);
DB2Storage<DurabilityQualityEntry>          sDurabilityQualityStore("DurabilityQuality.db2", DurabilityQualityFormat, HOTFIX_SEL_DURABILITY_QUALITY);
DB2Storage<EmotesEntry>                     sEmotesStore("Emotes.db2", EmotesEntryFormat, HOTFIX_SEL_EMOTES);
DB2Storage<EmotesTextEntry>                 sEmotesTextStore("EmotesText.db2", EmotesTextFormat, HOTFIX_SEL_EMOTES_TEXT);
DB2Storage<GameObjectDisplayInfoEntry>      sGameObjectDisplayInfoStore("GameObjectDisplayInfo.db2", GameObjectDisplayInfoFormat, HOTFIX_SEL_GAMEOBJECT_DISPLAY_INFO);
DB2Storage<GameObjectsEntry>                sGameObjectsStore("GameObjects.db2", GameObjectsFormat, HOTFIX_SEL_GAMEOBJECTS);
DB2Storage<GameTablesEntry>                 sGameTablesStore("GameTables.db2", GameTablesFormat, HOTFIX_SEL_GAME_TABLES);
DB2Storage<GarrAbilityEffectEntry>          sGarrAbilityEffectStore("GarrAbilityEffect.db2", GarrAbilityEffectFormat, HOTFIX_SEL_GARR_ABILITY_EFFECT);
DB2Storage<GarrAbilityEntry>                sGarrAbilityStore("GarrAbility.db2", GarrAbilityFormat, HOTFIX_SEL_GARR_ABILITY);
DB2Storage<GarrBuildingEntry>               sGarrBuildingStore("GarrBuilding.db2", GarrBuildingFormat, HOTFIX_SEL_GARR_BUILDING);
DB2Storage<GarrBuildingPlotInstEntry>       sGarrBuildingPlotInstStore("GarrBuildingPlotInst.db2", GarrBuildingPlotInstFormat, HOTFIX_SEL_GARR_BUILDING_PLOT_INST);
DB2Storage<GarrClassSpecEntry>              sGarrClassSpecStore("GarrClassSpec.db2", GarrClassSpecFormat, HOTFIX_SEL_GARR_CLASS_SPEC);
DB2Storage<GarrEncounterEntry>              sGarrEncounterStore("GarrEncounter.db2", GarrEncounterFormat, HOTFIX_SEL_GARR_ENCOUNTER);
DB2Storage<GarrEncounterXMechanicEntry>     sGarrEncounterXMechanicStore("GarrEncounterXMechanic.db2", GarrEncounterXMechanicFormat, HOTFIX_SEL_GARR_ENCOUNTER_X_MECHANIC);
DB2Storage<GarrFollowerEntry>               sGarrFollowerStore("GarrFollower.db2", GarrFollowerFormat, HOTFIX_SEL_GARR_FOLLOWER);
DB2Storage<GarrFollowerLevelXPEntry>        sGarrFollowerLevelXPStore("GarrFollowerLevelXP.db2", GarrFollowerLevelXPFormat, HOTFIX_SEL_GARR_FOLLOWER_LEVEL_X_P);
DB2Storage<GarrFollowerQualityEntry>        sGarrFollowerQualityStore("GarrFollowerQuality.db2", GarrFollowerQualityFormat, HOTFIX_SEL_GARR_FOLLOWER_QUALITY);
DB2Storage<GarrFollowerXAbilityEntry>       sGarrFollowerXAbilityStore("GarrFollowerXAbility.db2", GarrFollowerXAbilityFormat, HOTFIX_SEL_GARR_FOLLOWER_X_ABILITY);
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
DB2Storage<GemPropertiesEntry>              sGemPropertiesStore("GemProperties.db2", GemPropertiesFormat, HOTFIX_SEL_GEM_PROPERTIES);
DB2Storage<GlyphPropertiesEntry>            sGlyphPropertiesStore("GlyphProperties.db2", GlyphPropertiesFormat, HOTFIX_SEL_GLYPH_PROPERTIES);
DB2Storage<GuildPerkSpellsEntry>            sGuildPerkSpellsStore("GuildPerkSpells.db2", GuildPerkSpellsFormat, HOTFIX_SEL_GUILD_PERK_SPELLS);
DB2Storage<HeirloomEntry>                   sHeirloomStore("Heirloom.db2", HeirloomFormat, HOTFIX_SEL_HEIRLOOM);
DB2Storage<HolidaysEntry>                   sHolidaysStore("Holidays.db2", HolidaysFormat, HOTFIX_SEL_HOLIDAYS);
DB2Storage<ImportPriceArmorEntry>           sImportPriceArmorStore("ImportPriceArmor.db2", ImportPriceArmorFormat, HOTFIX_SEL_IMPORT_PRICE_ARMOR);
DB2Storage<ImportPriceQualityEntry>         sImportPriceQualityStore("ImportPriceQuality.db2", ImportPriceQualityFormat, HOTFIX_SEL_IMPORT_PRICE_QUALITY);
DB2Storage<ImportPriceShieldEntry>          sImportPriceShieldStore("ImportPriceShield.db2", ImportPriceShieldFormat, HOTFIX_SEL_IMPORT_PRICE_SHIELD);
DB2Storage<ImportPriceWeaponEntry>          sImportPriceWeaponStore("ImportPriceWeapon.db2", ImportPriceWeaponFormat, HOTFIX_SEL_IMPORT_PRICE_WEAPON);
DB2Storage<ItemAppearanceEntry>             sItemAppearanceStore("ItemAppearance.db2", ItemAppearanceFormat, HOTFIX_SEL_ITEM_APPEARANCE);
DB2Storage<ItemArmorQualityEntry>           sItemArmorQualityStore("ItemArmorQuality.db2", ItemArmorQualityFormat, HOTFIX_SEL_ITEM_ARMOR_QUALITY);
DB2Storage<ItemArmorShieldEntry>            sItemArmorShieldStore("ItemArmorShield.db2", ItemArmorShieldFormat, HOTFIX_SEL_ITEM_ARMOR_SHIELD);
DB2Storage<ItemArmorTotalEntry>             sItemArmorTotalStore("ItemArmorTotal.db2", ItemArmorTotalFormat, HOTFIX_SEL_ITEM_ARMOR_TOTAL);
DB2Storage<ItemBonusEntry>                  sItemBonusStore("ItemBonus.db2", ItemBonusFormat, HOTFIX_SEL_ITEM_BONUS);
DB2Storage<ItemBonusTreeNodeEntry>          sItemBonusTreeNodeStore("ItemBonusTreeNode.db2", ItemBonusTreeNodeFormat, HOTFIX_SEL_ITEM_BONUS_TREE_NODE);
DB2Storage<ItemClassEntry>                  sItemClassStore("ItemClass.db2", ItemClassFormat, HOTFIX_SEL_ITEM_CLASS);
DB2Storage<ItemCurrencyCostEntry>           sItemCurrencyCostStore("ItemCurrencyCost.db2", ItemCurrencyCostFormat, HOTFIX_SEL_ITEM_CURRENCY_COST);
DB2Storage<ItemDamageEntry>                 sItemDamageAmmoStore("ItemDamageAmmo.db2", ItemDamageFormat, HOTFIX_SEL_ITEM_DAMAGE);
DB2Storage<ItemDamageEntry>                 sItemDamageOneHandCasterStore("ItemDamageOneHandCaster.db2", ItemDamageFormat, HOTFIX_SEL_ITEM_DAMAGE);
DB2Storage<ItemDamageEntry>                 sItemDamageOneHandStore("ItemDamageOneHand.db2", ItemDamageFormat, HOTFIX_SEL_ITEM_DAMAGE);
DB2Storage<ItemDamageEntry>                 sItemDamageTwoHandCasterStore("ItemDamageTwoHandCaster.db2", ItemDamageFormat, HOTFIX_SEL_ITEM_DAMAGE);
DB2Storage<ItemDamageEntry>                 sItemDamageTwoHandStore("ItemDamageTwoHand.db2", ItemDamageFormat, HOTFIX_SEL_ITEM_DAMAGE);
DB2Storage<ItemDisenchantLootEntry>         sItemDisenchantLootStore("ItemDisenchantLoot.db2", ItemDisenchantLootFormat, HOTFIX_SEL_ITEM_DISENCHANT_LOOT);
DB2Storage<ItemEffectEntry>                 sItemEffectStore("ItemEffect.db2", ItemEffectFormat, HOTFIX_SEL_ITEM_EFFECT);
DB2Storage<ItemEntry>                       sItemStore("Item.db2", ItemFormat, HOTFIX_SEL_ITEM);
DB2Storage<ItemExtendedCostEntry>           sItemExtendedCostStore("ItemExtendedCost.db2", ItemExtendedCostFormat, HOTFIX_SEL_ITEM_EXTENDED_COST);
DB2Storage<ItemLimitCategoryEntry>          sItemLimitCategoryStore("ItemLimitCategory.db2", ItemLimitCategoryFormat, HOTFIX_SEL_ITEM_LIMIT_CATEGORY);
DB2Storage<ItemModifiedAppearanceEntry>     sItemModifiedAppearanceStore("ItemModifiedAppearance.db2", ItemModifiedAppearanceFormat, HOTFIX_SEL_ITEM_MODIFIED_APPEARANCE);
DB2Storage<ItemPriceBaseEntry>              sItemPriceBaseStore("ItemPriceBase.db2", ItemPriceBaseFormat, HOTFIX_SEL_ITEM_PRICE_BASE);
DB2Storage<ItemRandomPropertiesEntry>       sItemRandomPropertiesStore("ItemRandomProperties.db2", ItemRandomPropertiesFormat, HOTFIX_SEL_ITEM_RANDOM_PROPERTIES);
DB2Storage<ItemRandomSuffixEntry>           sItemRandomSuffixStore("ItemRandomSuffix.db2", ItemRandomSuffixFormat, HOTFIX_SEL_ITEM_RANDOM_SUFFIX);
DB2Storage<ItemSetEntry>                    sItemSetStore("ItemSet.db2", ItemSetFormat, HOTFIX_SEL_ITEM_SET);
DB2Storage<ItemSetSpellEntry>               sItemSetSpellStore("ItemSetSpell.db2", ItemSetSpellFormat, HOTFIX_SEL_ITEM_SET_SPELL);
DB2Storage<ItemSparseEntry>                 sItemSparseStore("Item-sparse.db2", ItemSparseFormat, HOTFIX_SEL_ITEM_SPARSE);
DB2Storage<ItemSpecEntry>                   sItemSpecStore("ItemSpec.db2", ItemSpecFormat, HOTFIX_SEL_ITEM_SPEC);
DB2Storage<ItemSpecOverrideEntry>           sItemSpecOverrideStore("ItemSpecOverride.db2", ItemSpecOverrideFormat, HOTFIX_SEL_ITEM_SPEC_OVERRIDE);
DB2Storage<ItemToBattlePetSpeciesEntry>     sItemToBattlePetSpeciesStore("ItemToBattlePetSpecies.db2", ItemToBattlePetSpeciesFormat, HOTFIX_SEL_ITEM_TO_BATTLE_PET_SPECIES);
DB2Storage<ItemUpgradeEntry>                sItemUpgradeStore("ItemUpgrade.db2", ItemUpgradeFormat, HOTFIX_SEL_ITEM_UPGRADE);
DB2Storage<ItemXBonusTreeEntry>             sItemXBonusTreeStore("ItemXBonusTree.db2", ItemXBonusTreeFormat, HOTFIX_SEL_ITEM_X_BONUS_TREE);
DB2Storage<KeyChainEntry>                   sKeyChainStore("KeyChain.db2", KeyChainFormat, HOTFIX_SEL_KEY_CHAIN);
DB2Storage<LanguageWordsEntry>              sLanguageWordsStore("LanguageWords.db2", LanguageWordsFormat, HOTFIX_SEL_LANGUAGE_WORDS);
DB2Storage<LockEntry>                       sLockStore("Lock.db2", LockFormat, HOTFIX_SEL_LOCK);
DB2Storage<MailTemplateEntry>               sMailTemplateStore("MailTemplate.db2", MailTemplateFormat, HOTFIX_SEL_MAIL_TEMPLATE);
DB2Storage<MapChallengeModeEntry>           sMapChallengeModeStore("MapChallengeMode.db2", MapChallengeModeFormat, HOTFIX_SEL_MAP_CHALLENGE_MODE);
DB2Storage<MapEntry>                        sMapStore("Map.db2", MapFormat, HOTFIX_SEL_MAP);
DB2Storage<MinorTalentEntry>                sMinorTalentStore("MinorTalent.db2", MinorTalentFormat, HOTFIX_SEL_MINOR_TALENT);
DB2Storage<ModifierTreeEntry>               sModifierTreeStore("ModifierTree.db2", ModifierTreeFormat, HOTFIX_SEL_MODIFIER_TREE);
DB2Storage<MountCapabilityEntry>            sMountCapabilityStore("MountCapability.db2", MountCapabilityFormat, HOTFIX_SEL_MOUNT_CAPABILITY);
DB2Storage<MountEntry>                      sMountStore("Mount.db2", MountFormat, HOTFIX_SEL_MOUNT);
DB2Storage<MountTypeEntry>                  sMountTypeStore("MountType.db2", MountTypeFormat, HOTFIX_SEL_MOUNT_TYPE);
DB2Storage<MountTypeXCapabilityEntry>       sMountTypeXCapabilityStore("MountTypeXCapability.db2", MountTypeXCapabilityFormat, HOTFIX_SEL_MOUNT_TYPE_X_CAPABILITY);
DB2Storage<MovieEntry>                      sMovieStore("Movie.db2", MovieFormat, HOTFIX_SEL_MOVIE);
DB2Storage<NameGenEntry>                    sNameGenStore("NameGen.db2", NameGenFormat, HOTFIX_SEL_NAME_GEN);
DB2Storage<NamesProfanityEntry>             sNamesProfanityStore("NamesProfanity.db2", NamesProfanityFormat, HOTFIX_SEL_NAMES_PROFANITY);
DB2Storage<NamesReservedEntry>              sNamesReservedStore("NamesReserved.db2", NamesReservedFormat, HOTFIX_SEL_NAMES_RESERVED);
DB2Storage<NamesReservedLocaleEntry>        sNamesReservedLocaleStore("NamesReservedLocale.db2", NamesReservedLocaleFormat, HOTFIX_SEL_NAMES_RESERVED_LOCALE);
DB2Storage<OverrideSpellDataEntry>          sOverrideSpellDataStore("OverrideSpellData.db2", OverrideSpellDataFormat, HOTFIX_SEL_OVERRIDE_SPELL_DATA);
DB2Storage<PhaseEntry>                      sPhaseStores("Phase.db2", PhaseFormat, HOTFIX_SEL_PHASE);
DB2Storage<PowerDisplayEntry>               sPowerDisplayStore("PowerDisplay.db2", PowerDisplayFormat, HOTFIX_SEL_POWER_DISPLAY);
DB2Storage<PvPDifficultyEntry>              sPvPDifficultyStore("PvpDifficulty.db2", PvPDifficultyFormat, HOTFIX_SEL_PVP_DIFFICULTY);
DB2Storage<PvpItemEntry>                    sPvpItemStore("PvpItem.db2", PvpItemFormat, HOTFIX_SEL_PVP_ITEM);
DB2Storage<QuestFactionRewEntry>            sQuestFactionRewardStore("QuestFactionReward.db2", QuestFactionRewardFormat, HOTFIX_SEL_QUEST_FACTION_REW);
DB2Storage<QuestPackageItemEntry>           sQuestPackageItemStore("QuestPackageItem.db2", QuestPackageItemFormat, HOTFIX_SEL_QUEST_PACKAGE_ITEM);
DB2Storage<QuestPOIPointEntry>              sQuestPOIPointStore("QuestPOIPoint.db2", QuestPOIPointFormat, HOTFIX_SEL_QUEST_P_O_I_POINT);
DB2Storage<QuestSortEntry>                  sQuestSortStore("QuestSort.db2", QuestSortFormat, HOTFIX_SEL_QUEST_SORT);
DB2Storage<QuestV2Entry>                    sQuestV2Store("QuestV2.db2", QuestV2Format, HOTFIX_SEL_QUEST_V2);
DB2Storage<QuestXPEntry>                    sQuestXPStore("QuestXP.db2", QuestXPFormat, HOTFIX_SEL_QUEST_XP);
DB2Storage<QuestLineEntry>                  sQuestLineStore("QuestLine.db2", QuestLineFormat, HOTFIX_SEL_QUEST_LINE);
DB2Storage<QuestLineXQuestEntry>            sQuestLineXQuestStore("QuestLineXQuest.db2", QuestLineXQuestFormat, HOTFIX_SEL_QUEST_LINE_X_QUEST);
DB2Storage<QuestMoneyRewardEntry>           sQuestMoneyRewardStore("QuestMoneyReward.db2", QuestMoneyRewardFormat, HOTFIX_SEL_QUEST_MONEY_REWARD);
DB2Storage<QuestObjectiveEntry>             sQuestObjectiveStore("QuestObjective.db2", QuestObjectiveFormat, HOTFIX_SEL_QUEST_OBJECTIVE);
DB2Storage<RandPropPointsEntry>             sRandPropPointsStore("RandPropPoints.db2", RandPropPointsStoreFormat, HOTFIX_SEL_RAND_PROP_POINTS);
DB2Storage<ResearchBranchEntry>             sResearchBranchStore("ResearchBranch.db2", ResearchBranchFormat, HOTFIX_SEL_RESEARCH_BRANCH);
DB2Storage<ResearchProjectEntry>            sResearchProjectStore("ResearchProject.db2", ResearchProjectFormat, HOTFIX_SEL_RESEARCH_PROJECT);
DB2Storage<ResearchSiteEntry>               sResearchSiteStore("ResearchSite.db2", ResearchSiteFormat, HOTFIX_SEL_RESEARCH_SITE);
DB2Storage<RuleSetItemUpgradeEntry>         sRuleSetItemUpgradeEntryStore("RulesetItemUpgrade.db2", RuleSetItemUpgradeFormat, HOTFIX_SEL_RULE_SET_ITEM_UPGRADE);
DB2Storage<ScalingStatDistributionEntry>    sScalingStatDistributionStore("ScalingStatDistribution.db2", ScalingStatDistributionFormat, HOTFIX_SEL_SCALING_STAT_DISTRIBUTION);
DB2Storage<ScenarioEntry>                   sScenarioStore("Scenario.db2", ScenarioFormat, HOTFIX_SEL_SCENARIO);
DB2Storage<ScenarioStepEntry>               sScenarioStepStore("ScenarioStep.db2", ScenarioStepFormat, HOTFIX_SEL_SCENARIO_STEP);
DB2Storage<SkillLineAbilityEntry>           sSkillLineAbilityStore("SkillLineAbility.db2", SkillLineAbilityFormat, HOTFIX_SEL_SKILL_LINE_ABILITY);
DB2Storage<SkillLineEntry>                  sSkillLineStore("SkillLine.db2", SkillLineFormat, HOTFIX_SEL_SKILL_LINE);
DB2Storage<SkillRaceClassInfoEntry>         sSkillRaceClassInfoStore("SkillRaceClassInfo.db2", SkillRaceClassInfoFormat, HOTFIX_SEL_SKILL_RACE_CLASS_INFO);
DB2Storage<SoundEntriesEntry>               sSoundEntriesStore("SoundEntries.db2", SoundEntriesFormat, HOTFIX_SEL_SOUND_ENTRIES);
DB2Storage<SpecializationSpellEntry>        sSpecializationSpellStore("SpecializationSpells.db2", SpecializationSpellsFormat, HOTFIX_SEL_SPECIALIZATION_SPELL);
DB2Storage<SpellAuraOptionsEntry>           sSpellAuraOptionsStore("SpellAuraOptions.db2", SpellAuraOptionsFormat, HOTFIX_SEL_SPELL_AURA_OPTIONS);
DB2Storage<SpellAuraRestrictionsEntry>      sSpellAuraRestrictionsStore("SpellAuraRestrictions.db2", SpellAuraRestrictionsFormat, HOTFIX_SEL_SPELL_AURA_RESTRICTIONS);
DB2Storage<SpellCastingRequirementsEntry>   sSpellCastingRequirementsStore("SpellCastingRequirements.db2", SpellCastingRequirementsFormat, HOTFIX_SEL_SPELL_CASTING_REQUIREMENTS);
DB2Storage<SpellCastTimesEntry>             sSpellCastTimesStore("SpellCastTimes.db2", SpellCastTimeFormat, HOTFIX_SEL_SPELL_CAST_TIMES);
DB2Storage<SpellCategoriesEntry>            sSpellCategoriesStore("SpellCategories.db2", SpellCategoriesFormat, HOTFIX_SEL_SPELL_CATEGORIES);
DB2Storage<SpellCategoryEntry>              sSpellCategoryStores("SpellCategory.db2", SpellCategoryFormat, HOTFIX_SEL_SPELL_CATEGORY);
DB2Storage<SpellClassOptionsEntry>          sSpellClassOptionsStore("SpellClassOptions.db2", SpellClassOptionsFormat, HOTFIX_SEL_SPELL_CLASS_OPTIONS);
DB2Storage<SpellCooldownsEntry>             sSpellCooldownsStore("SpellCooldowns.db2", SpellCooldownsFormat, HOTFIX_SEL_SPELL_COOLDOWNS);
DB2Storage<SpellDurationEntry>              sSpellDurationStore("SpellDuration.db2", SpellDurationFormat, HOTFIX_SEL_SPELL_DURATION);
DB2Storage<SpellEffectEntry>                sSpellEffectStore("SpellEffect.db2", SpellEffectFormat, HOTFIX_SEL_SPELL_EFFECT);
DB2Storage<SpellEffectScalingEntry>         sSpellEffectScalingStore("SpellEffectScaling.db2", SpellEffectScalingFormat, HOTFIX_SEL_SPELL_EFFECT_SCALING);
DB2Storage<SpellEntry>                      sSpellStore("Spell.db2", SpellFormat, HOTFIX_SEL_SPELL);
DB2Storage<SpellEquippedItemsEntry>         sSpellEquippedItemsStore("SpellEquippedItems.db2", SpellEquippedItemsFormat, HOTFIX_SEL_SPELL_EQUIPPED_ITEMS);
DB2Storage<SpellFocusObjectEntry>           sSpellFocusObjectStore("SpellFocusObject.db2", SpellFocusObjectFormat, HOTFIX_SEL_SPELL_FOCUS_OBJECT);
DB2Storage<SpellInterruptsEntry>            sSpellInterruptsStore("SpellInterrupts.db2", SpellInterruptsFormat, HOTFIX_SEL_SPELL_INTERRUPTS);
DB2Storage<SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore("SpellItemEnchantmentCondition.db2", SpellItemEnchantmentConditionFormat, HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT_CONDITION);
DB2Storage<SpellLearnSpellEntry>            sSpellLearnSpellStore("SpellLearnSpell.db2", SpellLearnSpellFormat, HOTFIX_SEL_SPELL_LEARN_SPELL);
DB2Storage<SpellLevelsEntry>                sSpellLevelsStore("SpellLevels.db2", SpellLevelsFormat, HOTFIX_SEL_SPELL_LEVELS);
DB2Storage<SpellMiscEntry>                  sSpellMiscStore("SpellMisc.db2", SpellMiscFormat, HOTFIX_SEL_SPELL_MISC);
DB2Storage<SpellMiscDifficultyEntry>        sSpellMiscDifficultyStore("SpellMiscDifficulty.db2", SpellMiscDifficultyFormat, HOTFIX_SEL_SPELL_MISC_DIFFICULTY);
DB2Storage<SpellPowerEntry>                 sSpellPowerStore("SpellPower.db2", SpellPowerFormat, HOTFIX_SEL_SPELL_POWER);
DB2Storage<SpellProcsPerMinuteEntry>        sSpellProcsPerMinuteStore("SpellProcsPerMinute.db2", SpellProcsPerMinuteFormat, HOTFIX_SEL_SPELL_PROCS_PER_MINUTE);
DB2Storage<SpellProcsPerMinuteModEntry>     sSpellProcsPerMinuteModStore("SpellProcsPerMinuteMod.db2", SpellProcsPerMinuteModFormat, HOTFIX_SEL_SPELL_PROCS_PER_MINUTE_MOD);
DB2Storage<SpellRadiusEntry>                sSpellRadiusStore("SpellRadius.db2", SpellRadiusFormat, HOTFIX_SEL_SPELL_RADIUS);
DB2Storage<SpellRangeEntry>                 sSpellRangeStore("SpellRange.db2", SpellRangeFormat, HOTFIX_SEL_SPELL_RANGE);
DB2Storage<SpellReagentsCurrencyEntry>      sSpellReagentsCurrencyStore("SpellReagentsCurrency.db2", SpellReagentsCurrencyFormat, HOTFIX_SEL_SPELL_REAGENTS_CURRENCY);
DB2Storage<SpellReagentsEntry>              sSpellReagentsStore("SpellReagents.db2", SpellReagentsFormat, HOTFIX_SEL_SPELL_REAGENTS);
DB2Storage<SpellScalingEntry>               sSpellScalingStore("SpellScaling.db2", SpellScalingFormat, HOTFIX_SEL_SPELL_SCALING);
DB2Storage<SpellShapeshiftEntry>            sSpellShapeshiftStore("SpellShapeshift.db2", SpellShapeshiftFormat, HOTFIX_SEL_SPELL_SHAPESHIFT);
DB2Storage<SpellShapeshiftFormEntry>        sSpellShapeshiftFormStore("SpellShapeshiftForm.db2", SpellShapeshiftFormFormat, HOTFIX_SEL_SPELL_SHAPESHIFT_FORM);
DB2Storage<SpellTargetRestrictionsEntry>    sSpellTargetRestrictionsStore("SpellTargetRestrictions.db2", SpellTargetRestrictionsFormat, HOTFIX_SEL_SPELL_TARGET_RESTRICTIONS);
DB2Storage<SpellTotemsEntry>                sSpellTotemsStore("SpellTotems.db2", SpellTotemsFormat, HOTFIX_SEL_SPELL_TOTEMS);
DB2Storage<SpellVisualEntry>                sSpellVisualStore("SpellVisual.db2", SpellVisualFormat, HOTFIX_SEL_SPELL_VISUAL);
DB2Storage<SpellXSpellVisualEntry>          sSpellXSpellVisualStore("SpellXSpellVisual.db2", SpellXSpellVisualFormat, HOTFIX_SEL_SPELL_X_SPELL_VISUAL);
DB2Storage<SummonPropertiesEntry>           sSummonPropertiesStore("SummonProperties.db2", SummonPropertiesFormat, HOTFIX_SEL_SUMMON_PROPERTIES);
DB2Storage<TaxiNodesEntry>                  sTaxiNodesStore("TaxiNodes.db2", TaxiNodesFormat, HOTFIX_SEL_TAXI_NODES);
DB2Storage<TaxiPathEntry>                   sTaxiPathStore("TaxiPath.db2", TaxiPathFormat, HOTFIX_SEL_TAXI_PATH);
DB2Storage<TaxiPathNodeEntry>               sTaxiPathNodeStore("TaxiPathNode.db2", TaxiPathNodeFormat, HOTFIX_SEL_TAXI_PATH_NODE);
DB2Storage<TotemCategoryEntry>              sTotemCategoryStore("TotemCategory.db2", TotemCategoryFormat, HOTFIX_SEL_TOTEM_CATEGORY);
DB2Storage<ToyEntry>                        sToyStore("Toy.db2", ToyFormat, HOTFIX_SEL_TOY);
DB2Storage<TransportAnimationEntry>         sTransportAnimationStore("TransportAnimation.db2", TransportAnimationFormat, HOTFIX_SEL_TRANSPORT_ANIMATION);
DB2Storage<UnitPowerBarEntry>               sUnitPowerBarStore("UnitPowerBar.db2", UnitPowerBarFormat, HOTFIX_SEL_UNIT_POWER_BAR);
DB2Storage<VehicleSeatEntry>                sVehicleSeatStore("VehicleSeat.db2", VehicleSeatFormat, HOTFIX_SEL_VEHICLE_SEAT);
DB2Storage<WorldMapOverlayEntry>            sWorldMapOverlayStore("WorldMapOverlay.db2", WorldMapOverlayFormat, HOTFIX_SEL_WORLD_MAP_OVERLAY);
DB2Storage<WorldMapTransformsEntry>         sWorldMapTransformsStore("WorldMapTransforms.db2", WorldMapTransformsFormat, HOTFIX_SEL_WORLD_MAP_TRANSFORMS);

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
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage->GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage->GetFormat()), sizeof(T), storage->GetFileName().c_str()));

    ++DB2FilesCount;

    if (storage->Load(db2Path + localeNames[defaultLocale] + '/', defaultLocale))
    {
        storage->LoadFromDB();

        for (uint8 i = LOCALE_enUS; i < MAX_LOCALES; ++i)
        {
            if (defaultLocale == i || i == LOCALE_none)
                continue;

            if (availableDb2Locales & (1 << i))
                if (!storage->LoadStringsFrom((db2Path + localeNames[i] + '/'), i))
                    availableDb2Locales &= ~(1 << i);             // mark as not available for speedup next checks

            storage->LoadStringsFromDB(i);
        }
    }
    else
    {
        if (FILE* f = fopen((db2Path + storage->GetFileName()).c_str(), "rb"))
        {
            std::ostringstream stream;
            stream << storage->GetFileName() << " exists, and has " << storage->GetFieldCount() << " field(s) (expected " << strlen(storage->GetFormat())
                << "). Extracted file might be from wrong client version.";
            errlist.push_back(std::string(stream.str()));
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

    //LOAD_DB2(sAreaGroupMemberStore);
    //LOAD_DB2(sBarberShopStyleStore);            // 20914
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
    //LOAD_DB2(sDungeonEncounterStore);
    //LOAD_DB2(sGameObjectsStore);
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
    //LOAD_DB2(sGlyphPropertiesStore);          // 20914
    //LOAD_DB2(sGlyphSlotStore,                   "GlyphSlot.db2"); // DEPRECATED
    //LOAD_DB2(sHeirloomStore);
    //LOAD_DB2(sHolidaysStore);
    //LOAD_DB2(sItemToBattlePetSpeciesStore);
    //LOAD_DB2(sItemUpgradeStore);
    //LOAD_DB2(sMapChallengeModeStore);
    //LOAD_DB2(sMountCapabilityStore);            // 20914
    //LOAD_DB2(sMountTypeStore);                  // 20914
    //LOAD_DB2(sResearchProjectStore);          // 20796
    //LOAD_DB2(sResearchSiteStore);             // 20796
    //LOAD_DB2(sRuleSetItemUpgradeEntryStore);
    //LOAD_DB2(sSoundEntriesStore);
    //LOAD_DB2(sSpellItemEnchantmentConditionStore); // 20810 DB2FileLoader::Load 193 - not compatible and empty
    //LOAD_DB2(sSpellVisualStore);
    //LOAD_DB2(sTaxiNodesStore);
    //LOAD_DB2(sTaxiPathNodeStore);
    //LOAD_DB2(sTaxiPathStore);
    //LOAD_DB2(sToyStore);
    //LOAD_DB2(sVehicleSeatStore);                // 20914
    
    LOAD_DB2(sAchievementStore);                // 20914
    LOAD_DB2(sAreaTableStore);                  // 20994
    LOAD_DB2(sAreaTriggerStore);                // 20994
    LOAD_DB2(sArmorLocationStore);              // 20914
    LOAD_DB2(sAuctionHouseStore);               // 20914
    LOAD_DB2(sBankBagSlotPricesStore);          // 20914
    LOAD_DB2(sBroadcastTextStore);              // 20994
    LOAD_DB2(sCharacterLoadoutItemStore);       // 20914
    LOAD_DB2(sCharStartOutfitStore);            // 20914
    LOAD_DB2(sCharTitlesStore);                 // 20994
    LOAD_DB2(sChatChannelsStore);               // 20914
    LOAD_DB2(sChrClassesXPowerTypesStore);      // 20914
    LOAD_DB2(sChrRacesStore);                   // 20914
    LOAD_DB2(sChrSpecializationStore);          // 20994
    LOAD_DB2(sCreatureDifficultyStore);         // 20914
    LOAD_DB2(sCreatureDisplayInfoStore);        // 20914
    LOAD_DB2(sCreatureFamilyStore);             // 20994
    LOAD_DB2(sCreatureTypeStore);               // 20914
    LOAD_DB2(sCriteriaStore);                   // 20914
    LOAD_DB2(sCriteriaTreeStore);               // 20914
    LOAD_DB2(sCurrencyTypesStore);              // 20994
    LOAD_DB2(sCurvePointStore);                 // 20994
    LOAD_DB2(sDestructibleModelDataStore);      // 20914
    LOAD_DB2(sDifficultyStore);                 // 20994
    LOAD_DB2(sDurabilityCostsStore);            // 20914
    LOAD_DB2(sDurabilityQualityStore);          // 20914
    LOAD_DB2(sEmotesStore);                     // 20914
    LOAD_DB2(sEmotesTextStore);                 // 20994
    LOAD_DB2(sGameObjectDisplayInfoStore);      // 20914
    LOAD_DB2(sGameTablesStore);                 // 20914
    LOAD_DB2(sGemPropertiesStore);              // 20914
    LOAD_DB2(sGuildPerkSpellsStore);            // 20914
    LOAD_DB2(sImportPriceArmorStore);           // 20914
    LOAD_DB2(sImportPriceQualityStore);         // 20914
    LOAD_DB2(sImportPriceShieldStore);          // 20914
    LOAD_DB2(sImportPriceWeaponStore);          // 20914
    LOAD_DB2(sItemAppearanceStore);             // 20994
    LOAD_DB2(sItemArmorQualityStore);           // 20914
    LOAD_DB2(sItemArmorShieldStore);            // 20914
    LOAD_DB2(sItemArmorTotalStore);             // 20914
    LOAD_DB2(sItemBonusStore);                  // 20994
    LOAD_DB2(sItemBonusTreeNodeStore);          // 20994
    LOAD_DB2(sItemClassStore);                  // 20914
    LOAD_DB2(sItemCurrencyCostStore);           // 20994
    LOAD_DB2(sItemDamageAmmoStore);             // 20914
    LOAD_DB2(sItemDamageOneHandCasterStore);    // 20914
    LOAD_DB2(sItemDamageOneHandStore);          // 20914
    LOAD_DB2(sItemDamageTwoHandCasterStore);    // 20914
    LOAD_DB2(sItemDamageTwoHandStore);          // 20914
    LOAD_DB2(sItemDisenchantLootStore);         // 20914
    LOAD_DB2(sItemEffectStore);                 // 20914
    LOAD_DB2(sItemExtendedCostStore);           // 20994
    LOAD_DB2(sItemLimitCategoryStore);          // 20914
    LOAD_DB2(sItemModifiedAppearanceStore);     // 20994
    LOAD_DB2(sItemPriceBaseStore);              // 20914
    LOAD_DB2(sItemRandomPropertiesStore);       // 20914
    LOAD_DB2(sItemRandomSuffixStore);           // 20914
    LOAD_DB2(sItemSetSpellStore);               // 20914
    LOAD_DB2(sItemSetStore);                    // 20914
    LOAD_DB2(sItemSparseStore);                 // 20914
    LOAD_DB2(sItemSpecOverrideStore);           // 20914
    LOAD_DB2(sItemSpecStore);                   // 20914
    LOAD_DB2(sItemStore);                       // 20914
    LOAD_DB2(sItemXBonusTreeStore);             // 20994
    LOAD_DB2(sKeyChainStore);                   // 20914
    LOAD_DB2(sLanguageWordsStore);              // 20914
    LOAD_DB2(sLockStore);                       // 20914
    LOAD_DB2(sMailTemplateStore);               // 20914
    LOAD_DB2(sMapStore);                        // 20914
    //LOAD_DB2(sMinorTalentStore);                // 20994
    LOAD_DB2(sModifierTreeStore);               // 20914
    LOAD_DB2(sMountStore);                      // 20994
    LOAD_DB2(sMountTypeXCapabilityStore);       // 20914
    LOAD_DB2(sMovieStore);                      // 20914
    LOAD_DB2(sNameGenStore);                    // 20914
    LOAD_DB2(sNamesProfanityStore);             // 20994
    LOAD_DB2(sNamesReservedLocaleStore);        // 20994
    LOAD_DB2(sNamesReservedStore);              // 20994
    LOAD_DB2(sOverrideSpellDataStore);          // 20994
    LOAD_DB2(sPhaseStores);                     // 20994
    LOAD_DB2(sPowerDisplayStore);               // 20914
    LOAD_DB2(sPvPDifficultyStore);              // 20914
    LOAD_DB2(sPvpItemStore);                    // 20914
    LOAD_DB2(sQuestFactionRewardStore);         // 20914
    LOAD_DB2(sQuestLineStore);                  // 20994
    LOAD_DB2(sQuestLineXQuestStore);            // 20994
    LOAD_DB2(sQuestMoneyRewardStore);           // 20994
    LOAD_DB2(sQuestObjectiveStore);             // 20994
    LOAD_DB2(sQuestPackageItemStore);           // 20914
    LOAD_DB2(sQuestPOIPointStore);              // 20914
    LOAD_DB2(sQuestSortStore);                  // 20914
    LOAD_DB2(sQuestV2Store);                    // 20914
    LOAD_DB2(sQuestXPStore);                    // 20914
    LOAD_DB2(sRandPropPointsStore);             // 20914
    LOAD_DB2(sResearchBranchStore);             // 20914
    LOAD_DB2(sScalingStatDistributionStore);    // 20914
    LOAD_DB2(sScenarioStepStore);               // 20914
    LOAD_DB2(sScenarioStore);                   // 20914
    LOAD_DB2(sSkillLineAbilityStore);           // 20914
    LOAD_DB2(sSkillLineStore);                  // 20914
    LOAD_DB2(sSkillRaceClassInfoStore);         // 20914
    LOAD_DB2(sSpecializationSpellStore);        // 20914
    LOAD_DB2(sSpellAuraOptionsStore);           // 20914
    LOAD_DB2(sSpellAuraRestrictionsStore);      // 20914
    LOAD_DB2(sSpellCastingRequirementsStore);   // 20914
    LOAD_DB2(sSpellCastTimesStore);             // 20914
    LOAD_DB2(sSpellCategoriesStore);            // 20914
    LOAD_DB2(sSpellCategoryStores);             // 20914
    LOAD_DB2(sSpellClassOptionsStore);          // 20914
    LOAD_DB2(sSpellCooldownsStore);             // 20914
    LOAD_DB2(sSpellDurationStore);              // 20914
    LOAD_DB2(sSpellEffectScalingStore);         // 20914
    LOAD_DB2(sSpellEffectStore);                // 20914
    LOAD_DB2(sSpellEquippedItemsStore);         // 20914
    LOAD_DB2(sSpellFocusObjectStore);           // 20914
    LOAD_DB2(sSpellInterruptsStore);            // 20914
    LOAD_DB2(sSpellLearnSpellStore);            // 20994
    LOAD_DB2(sSpellLevelsStore);                // 20914
    LOAD_DB2(sSpellMiscDifficultyStore);        // 20914
    LOAD_DB2(sSpellMiscStore);                  // 20914
    LOAD_DB2(sSpellPowerStore);                 // 20914
    LOAD_DB2(sSpellProcsPerMinuteModStore);     // 20914
    LOAD_DB2(sSpellProcsPerMinuteStore);        // 20914
    LOAD_DB2(sSpellRadiusStore);                // 20914
    LOAD_DB2(sSpellRangeStore);                 // 20914
    LOAD_DB2(sSpellReagentsCurrencyStore);      // 20914
    LOAD_DB2(sSpellReagentsStore);              // 20914
    LOAD_DB2(sSpellScalingStore);               // 20914
    LOAD_DB2(sSpellShapeshiftFormStore);        // 20914
    LOAD_DB2(sSpellShapeshiftStore);            // 20914
    LOAD_DB2(sSpellStore);                      // 20914
    LOAD_DB2(sSpellTargetRestrictionsStore);    // 20914
    LOAD_DB2(sSpellTotemsStore);                // 20914
    LOAD_DB2(sSpellXSpellVisualStore);          // 20914
    LOAD_DB2(sSummonPropertiesStore);           // 20914
    LOAD_DB2(sTotemCategoryStore);              // 20914
    LOAD_DB2(sTransportAnimationStore);         // 20914
    LOAD_DB2(sUnitPowerBarStore);               // 20914
    LOAD_DB2(sWorldMapOverlayStore);            // 20914
    LOAD_DB2(sWorldMapTransformsStore);         // 20914

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
        for (DB2StoreProblemList::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount, str.c_str());
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DB2 data stores in %u ms", DB2FilesCount, GetMSTimeDiffToNow(oldMSTime));
}

void DB2Manager::InitDB2CustomStores()
{
    for (AreaTableEntry const* area : sAreaTableStore)
        _areaEntry.insert(AreaEntryContainer::value_type(area->ID, area));

    for (BattlePetSpeciesEntry const* entry : sBattlePetSpeciesStore)
        _battlePetSpeciesBySpellId[entry->CreatureEntry] = entry;

    for (BattlePetSpeciesStateEntry const* entry : sBattlePetSpeciesStateStore)
        _battlePetSpeciesStates[entry->SpeciesID][BattlePetState(entry->State)] = entry->Value;

    for (BattlePetBreedStateEntry const* entry : sBattlePetBreedStateStore)
        _battlePetBreedStates[entry->breedID][BattlePetState(entry->stateID)] = entry->stateModifier;

    for (BattlePetBreedQualityEntry const* entry : sBattlePetBreedQualityStore)
        _battlePetQualityMultiplierId[entry->quality] = entry->qualityModifier;

    for (BattlePetAbilityTurnEntry const* entry : sBattlePetAbilityTurnStore)
        _battlePetTurnByAbilityId.insert(BattlePetTurnByAbilityIdContainer::value_type(entry->AbilityID, std::make_pair(entry->ID, entry->turnIndex)));

    for (BattlePetAbilityEffectEntry const* entry : sBattlePetAbilityEffectStore)
        _battlePetEffectEntryByTurnId[entry->TurnEntryID] = entry;

    for (BattlePetSpeciesXAbilityEntry const* entry : sBattlePetSpeciesXAbilityStore)
        _battlePetXAbilityEntryBySpecId.insert(BattlePetXAbilityEntryBySpecIdContainer::value_type(entry->speciesID, entry));

    for (GarrFollowerLevelXPEntry* const entry : sGarrFollowerLevelXPStore)
        _garrFollowerLevelXP[entry->Level] = entry->NextLevelXP;

    for (GarrFollowerQualityEntry* const entry : sGarrFollowerQualityStore)
        _garrFollowerQualityXP[entry->Quality] = entry->NextQualityXP;

    for (GarrMissionRewardEntry const* entry : sGarrMissionRewardStore)
        _garrMissionRewardByMissionID[entry->MissionID] = entry;

    for (LanguageWordsEntry const* entry : sLanguageWordsStore)
        _languageWordsMap[entry->Lang][strlen(entry->Word->Str[sObjectMgr->GetDBCLocaleIndex()])].push_back(entry->Word->Str[sObjectMgr->GetDBCLocaleIndex()]);

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
        _mountsBySpellId[mount->SpellID] = mount;

    for (MountTypeXCapabilityEntry const* mount : sMountTypeXCapabilityStore)
        _mountCapabilitiesByType[mount->MountTypeID].insert(mount);

    for (MapChallengeModeEntry const* entry : sMapChallengeModeStore)
        _mapChallengeModeEntrybyMap[entry->map] = entry;

    for (ItemXBonusTreeEntry const* itemBonusTreeAssignment : sItemXBonusTreeStore)
        _itemToBonusTree[itemBonusTreeAssignment->ItemID].push_back(itemBonusTreeAssignment);

    for (QuestPackageItemEntry const* questPackageItem : sQuestPackageItemStore)
        _questPackages[questPackageItem->QuestPackageID].push_back(questPackageItem);

    for (GameObjectsEntry const* store : sGameObjectsStore)
        _gameObjectsList.push_back(store->ID);

    for (RuleSetItemUpgradeEntry const* rsiu : sRuleSetItemUpgradeEntryStore)
    {
        ItemUpgradeDataContainer::iterator itr = _itemUpgradeDataMap.find(rsiu->itemEntry);
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

    for (CharacterLoadoutItemEntry const* LoadOutItem : sCharacterLoadoutItemStore)
        _characterLoadoutItem[LoadOutItem->LoadOutID].push_back(LoadOutItem->ItemID);

    for (CriteriaTreeEntry const* ct : sCriteriaTreeStore)
        if (ct->Parent)
            _criteriaTree[ct->Parent].push_back(ct);

    for (GameObjectDisplayInfoEntry const* info : sGameObjectDisplayInfoStore)
    {
        if (info->GeoBoxMax.X < info->GeoBoxMin.X)
            std::swap(*(float*)(&info->GeoBoxMax.X), *(float*)(&info->GeoBoxMin.X));

        if (info->GeoBoxMax.Y < info->GeoBoxMin.Y)
            std::swap(*(float*)(&info->GeoBoxMax.Y), *(float*)(&info->GeoBoxMin.Y));

        if (info->GeoBoxMax.Z < info->GeoBoxMin.Z)
            std::swap(*(float*)(&info->GeoBoxMax.Z), *(float*)(&info->GeoBoxMin.Z));
    }

    for (ItemSetSpellEntry const* itemSetSpell : sItemSetSpellStore)
        _itemSetSpells[itemSetSpell->ItemSetID].push_back(itemSetSpell);

    for (ItemSpecOverrideEntry const* entry : sItemSpecOverrideStore)
    {
        _itemSpec[entry->ItemID].push_back(entry->SpecID);
        _itemSpecOverrides[entry->ItemID].push_back(entry);
    }

    for (ModifierTreeEntry const* mt : sModifierTreeStore)
        if (mt->Parent)
            _modifierTree[mt->Parent].push_back(mt);

    for (NameGenEntry const* entry : sNameGenStore)
        _nameGenData[entry->RaceID][entry->Gender].push_back(entry);

    for (NamesProfanityEntry const* namesProfanity : sNamesProfanityStore)
    {
        ASSERT(namesProfanity->Language < MAX_LOCALES || namesProfanity->Language == -1);
        if (namesProfanity->Language != -1)
            _nameValidators[namesProfanity->Language].emplace_back(namesProfanity->Name, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
        else
        {
            for (uint32 i = 0; i < MAX_LOCALES; ++i)
            {
                if (i == LOCALE_none)
                    continue;

                _nameValidators[i].emplace_back(namesProfanity->Name, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
            }
        }
    }

    for (NamesReservedEntry const* namesReserved : sNamesReservedStore)
        _nameValidators[MAX_LOCALES].emplace_back(namesReserved->Name, boost::regex::perl | boost::regex::icase | boost::regex::optimize);

    for (NamesReservedLocaleEntry const* namesReserved : sNamesReservedLocaleStore)
    {
        ASSERT(!(namesReserved->LocaleMask & ~((1 << MAX_LOCALES) - 1)));
        for (uint32 i = 0; i < MAX_LOCALES; ++i)
        {
            if (i == LOCALE_none)
                continue;

            if (namesReserved->LocaleMask & (1 << i))
                _nameValidators[i].emplace_back(namesReserved->Name, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
        }
    }

    for (ResearchSiteEntry const* rs : sResearchSiteStore)
    {
        if (!rs->IsValid())
            continue;

        ResearchSiteData& data = _researchSiteDataMap[rs->ID];
        data.entry = rs;
        for (QuestPOIPointEntry const* poi : sQuestPOIPointStore)
            if (poi->QuestPOIBlobID == rs->POIid)
                data.points.push_back(ResearchPOIPoint(poi->X, poi->Y));

        if (data.points.size() == 0)
            sLog->outDebug(LOG_FILTER_SERVER_LOADING, "Research site %u POI %u map %u has 0 POI points in DB2!", rs->ID, rs->POIid, rs->MapID);
    }

    for (ResearchProjectEntry const* rp : sResearchProjectStore)
    {
        if (!rp || !rp->IsVaid())
            continue;

        _researchProjectContainer.insert(rp);
    }

    for (TransportAnimationEntry const* entry : sTransportAnimationStore)
        _transportAnimationsByEntry[entry->TransportID][entry->TimeIndex] = entry;

    for (ScenarioStepEntry const* entry : sScenarioStepStore)
    {
        if (!entry || !entry->Criteriatreeid)
            continue;

        if (!sCriteriaTreeStore.LookupEntry(entry->Criteriatreeid))
            continue;

        sScenarioCriteriaTreeStore.insert(entry->Criteriatreeid);
    }

    for (SkillRaceClassInfoEntry const* entry : sSkillRaceClassInfoStore)
        if (sSkillLineStore.LookupEntry(entry->SkillID))
            _skillRaceClassInfoBySkill.emplace(entry->SkillID, entry);

    for (SpecializationSpellEntry const* specSpells : sSpecializationSpellStore)
        _specializationSpellsBySpec[specSpells->SpecID].push_back(specSpells);

    for (SpellEntry const* v : sSpellStore)
    {
        SpellCategoriesEntry const* spell = sSpellCategoriesStore.LookupEntry(v->ID);
        if (spell && spell->Category)
            _spellCategory[spell->Category].insert(v->ID);
    }

    for (SpellProcsPerMinuteModEntry const* sppm : sSpellProcsPerMinuteModStore)
        _spellProcsPerMinuteModEntryList[sppm->SpellProcsPerMinuteID].push_back(sppm->ID);

    for (SpellTargetRestrictionsEntry const* restriction : sSpellTargetRestrictionsStore)
        _spellRestrictionDiff[restriction->SpellID].insert(restriction);

    for (SpellEffectEntry const* spellEffect : sSpellEffectStore)
    {
        if (spellEffect->EffectIndex > MAX_SPELL_EFFECTS)
            continue;

        if (spellEffect->DifficultyID)
            _spellEffectDiff[spellEffect->SpellID].effects[MAKE_PAIR16(spellEffect->EffectIndex, spellEffect->DifficultyID)] = spellEffect;
        else
            _spellEffectMap[spellEffect->SpellID].effects[spellEffect->EffectIndex] = spellEffect;

        if (spellEffect->Effect == SPELL_EFFECT_LEARN_SPELL)
            _revertLearnSpell[spellEffect->EffectTriggerSpell] = spellEffect->SpellID;

        if (spellEffect->EffectTriggerSpell)
            _reversTriggerSpellList[spellEffect->EffectTriggerSpell] = spellEffect->SpellID;
    }

    memset(_chrSpecializationByIndex, 0, sizeof(_chrSpecializationByIndex));
    for (ChrSpecializationEntry const* chrSpec : sChrSpecializationStore)
        _chrSpecializationByIndex[chrSpec->ClassID][chrSpec->OrderIndex] = chrSpec;

    for (PvPDifficultyEntry const* entry : sPvPDifficultyStore)
    {
        if (entry->BracketID > MAX_BATTLEGROUND_BRACKETS)
            ASSERT(false && "Need update MAX_BATTLEGROUND_BRACKETS by db2 data");
    }

    sChrClassesXPowerTypesStore.Sort(&ChrClassesXPowerTypesEntryComparator::Compare);
    for (uint32 i = 0; i < MAX_CLASSES; ++i)
        for (uint32 j = 0; j < MAX_POWERS; ++j)
            _powersByClass[i][j] = MAX_POWERS;

    for (ChrClassesXPowerTypesEntry const* power : sChrClassesXPowerTypesStore)
    {
        uint32 index = 0;
        for (uint32 j = 0; j < MAX_POWERS; ++j)
            if (_powersByClass[power->ClassID][j] != MAX_POWERS)
                ++index;

        _powersByClass[power->ClassID][power->PowerType] = index;
    }

    for (DungeonEncounterEntry const* store : sDungeonEncounterStore)
        if (store->creatureDisplayID)
            _dungeonEncounterByDisplayID[store->creatureDisplayID] = store;

    memset(_minorTalentByIndexStore, 0, sizeof(_minorTalentByIndexStore));
    for (MinorTalentEntry const* minotTal : sMinorTalentStore)
        _minorTalentByIndexStore[minotTal->SpecID][minotTal->OrderIndex] = minotTal;

    for (MapDifficultyEntry const* entry : sMapDifficultyStore)
        _mapDifficulty[entry->MapID][entry->DifficultyID] = entry;

    for (SpellMiscDifficultyEntry const* entry : sSpellMiscDifficultyStore)
        _spellMiscDifficulty.insert(SpellMiscDifficultyContainer::value_type(entry->SpellID, entry->ID));

    for (SkillLineAbilityEntry const* skillLine : sSkillLineAbilityStore)
    {
        SpellMiscEntry const* spellMisc = sSpellMiscStore.LookupEntry(GetSpellMisc(skillLine->SpellID, DIFFICULTY_NONE));
        if (!spellMisc)
            continue;

        if (spellMisc->Attributes[0] & SPELL_ATTR0_PASSIVE)
        {
            for (CreatureFamilyEntry const* cFamily : sCreatureFamilyStore)
            {
                if (skillLine->SkillLine != cFamily->SkillLine[0] && skillLine->SkillLine != cFamily->SkillLine[1])
                    continue;

                if (skillLine->AquireMethod != SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN)
                    continue;

                _petFamilySpells[cFamily->ID].insert(skillLine->SpellID);
            }
        }
    }

    for (QuestLineXQuestEntry const* entry : sQuestLineXQuestStore)
        _questLineXQuest.insert(QuestLineXQuestContainer::value_type(MAKE_PAIR16(entry->LineID, entry->Pos), entry->QuestID));
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
    return _gameObjectsList;
}

ItemUpgradeData const* DB2Manager::GetItemUpgradeData(uint32 itemEntry)
{
    ItemUpgradeDataContainer::iterator itr = _itemUpgradeDataMap.find(itemEntry);
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
    if (level >= it2->second->Pos.X)
        return it2->second->Pos.Y;

    auto previousItr = it2++;
    for (; it2 != itr->second.end(); ++it2, ++previousItr)
        if (level >= it2->second->Pos.X)
            return uint32((previousItr->second->Pos.Y - it2->second->Pos.Y) / (previousItr->second->Pos.X - it2->second->Pos.X) * (float(level) - it2->second->Pos.X) + it2->second->Pos.Y);

    return uint32(previousItr->second->Pos.Y);  // Lowest scaling point
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

DB2Manager::ItemBonusContainer const* DB2Manager::GetItemBonusList(uint32 bonusListId) const
{
    auto itr = _itemBonusLists.find(bonusListId);
    if (itr != _itemBonusLists.end())
        return &itr->second;

    return nullptr;
}

DB2Manager::LanguageWordsContainer const* DB2Manager::GetLanguageWordMap(uint32 langID)
{
    std::map<uint32, LanguageWordsContainer>::const_iterator itr = _languageWordsMap.find(langID);
    return itr != _languageWordsMap.end() ? &itr->second : nullptr;
}

StringVector const* DB2Manager::GetLanguageWordsBySize(uint32 langID, uint32 size)
{
    LanguageWordsContainer const* wordMap = GetLanguageWordMap(langID);
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
    HeirloomItemsContainer::const_iterator itr = _heirlooms.find(itemId);
    if (itr != _heirlooms.end())
        return itr->second;

    return nullptr;
}

HeirloomEntry const* DB2Manager::GetHeirloomByOldItem(uint32 itemId) const
{
    for (HeirloomEntry const* heirloom : sHeirloomStore)
        if (heirloom->OldItem[0] == itemId || heirloom->OldItem[1] == itemId)
            return heirloom;

    return nullptr;
}

bool DB2Manager::MountTypeXCapabilityEntryComparator::Compare(MountTypeXCapabilityEntry const* left, MountTypeXCapabilityEntry const* right)
{
    if (left->MountTypeID == right->MountTypeID)
        return left->OrderIndex < right->OrderIndex;

    return left->ID < right->ID;
}

DB2Manager::MountTypeXCapabilitySet const* DB2Manager::GetMountCapabilities(uint32 mountType) const
{
    auto itr = _mountCapabilitiesByType.find(mountType);
    if (itr != _mountCapabilitiesByType.end())
        return &itr->second;

    return nullptr;
}

BattlePetSpeciesEntry const* DB2Manager::GetBattlePetSpeciesEntry(uint32 creatureEntry)
{
    BattlePetSpeciesBySpellIdContainer::const_iterator it = _battlePetSpeciesBySpellId.find(creatureEntry);
    if (it != _battlePetSpeciesBySpellId.end())
        return it->second;

    return nullptr;
}

uint8 DB2Manager::GetBattlePetSpeciesBySpellID(uint32 entry) const
{
    BattlePetSpeciesBySpellIdContainer::const_iterator it = _battlePetSpeciesBySpellId.find(entry);
    if (it != _battlePetSpeciesBySpellId.end())
        return it->second->petType;

    return 1;
}

float DB2Manager::CalcBattlePetQualityMuliplier(uint8 quality, uint8 level)
{
    BattlePetQualityMultiplierContainer::const_iterator itr = _battlePetQualityMultiplierId.find(quality);
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
            BattlePetEffectEntryByTurnIdContainer::const_iterator itr = _battlePetEffectEntryByTurnId.find(turnID);
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
    GarrFollowerLevelXPContainer::const_iterator it = _garrFollowerLevelXP.find(level);
    if (it != _garrFollowerLevelXP.end())
        return it->second;

    return 0;
}

uint32 DB2Manager::GetXPForNextFollowerQuality(uint32 quality)
{
    GarrFollowerQualityXPContainer::const_iterator it = _garrFollowerQualityXP.find(quality);
    if (it != _garrFollowerQualityXP.end())
        return it->second;

    return 0;
}

GarrMissionRewardEntry const* DB2Manager::GetMissionRewardByRecID(uint32 missionRecID)
{
    GarrMissionRewardByMissionIDContainer::const_iterator it = _garrMissionRewardByMissionID.find(missionRecID);
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
    AchievementParentContainer::const_iterator v = _achievementParentList.find(criteriaTree);
    if (v != _achievementParentList.end())
        return v->second;

    return nullptr;
}

std::vector<uint32> DB2Manager::GetItemLoadOutItemsByClassID(uint32 classID)
{
    switch (classID)
    {
        case CLASS_WARRIOR: return _characterLoadoutItem[539];
        case CLASS_PALADIN: return _characterLoadoutItem[524];
        case CLASS_HUNTER: return _characterLoadoutItem[533];
        case CLASS_ROGUE: return _characterLoadoutItem[512];
        case CLASS_PRIEST: return _characterLoadoutItem[536];
        case CLASS_DEATH_KNIGHT: return _characterLoadoutItem[515];
        case CLASS_SHAMAN: return _characterLoadoutItem[530];
        case CLASS_MAGE: return _characterLoadoutItem[545];
        case CLASS_WARLOCK: return _characterLoadoutItem[542];
        case CLASS_MONK: return _characterLoadoutItem[527];
        case CLASS_DRUID: return _characterLoadoutItem[549];
        case CLASS_DEMON_HUNTER: return _characterLoadoutItem[512]; // update id
    }

    return _characterLoadoutItem[536];
}

std::vector<CriteriaTreeEntry const*> const* DB2Manager::GetCriteriaTreeList(uint32 parent)
{
    CriteriaTreeContainer::const_iterator itr = _criteriaTree.find(parent);
    if (itr != _criteriaTree.end())
        return &itr->second;

    return nullptr;
}

std::list<uint32> DB2Manager::GetItemSpecsList(uint32 itemID)
{
    return _itemSpec[itemID];
}

std::vector<ModifierTreeEntry const*> const* DB2Manager::GetModifierTreeList(uint32 parent)
{
    ModifierTreeContainer::const_iterator itr = _modifierTree.find(parent);
    if (itr != _modifierTree.end())
        return &itr->second;

    return nullptr;
}

std::string DB2Manager::GetRandomCharacterName(uint8 race, uint8 gender, LocaleConstant locale) const
{
    ASSERT(gender < GENDER_NONE);
    auto ritr = _nameGenData.find(race);
    if (ritr == _nameGenData.end())
        return "";

    if (ritr->second[gender].empty())
        return "";

    LocalizedString* data = Trinity::Containers::SelectRandomContainerElement(ritr->second[gender])->Name;
    if (*data->Str[locale] != '\0')
        return data->Str[locale];

    return data->Str[sWorld->GetDefaultDbcLocale()];
}

ResponseCodes DB2Manager::ValidateName(std::string const& name, LocaleConstant locale) const
{
    for (boost::regex const& regex : _nameValidators[locale])
        if (boost::regex_search(name, regex))
            return CHAR_NAME_PROFANE;

    for (boost::regex const& regex : _nameValidators[MAX_LOCALES])
        if (boost::regex_search(name, regex))
            return CHAR_NAME_RESERVED;

    return CHAR_NAME_SUCCESS;
}

uint32 DB2Manager::GetQuestUniqueBitFlag(uint32 questID)
{
    QuestV2Entry const* v2 = sQuestV2Store.LookupEntry(questID);
    if (!v2)
        return 0;

    return v2->UniqueBitFlag;
}

ResearchSiteEntry const* DB2Manager::GetResearchSiteEntryById(uint32 id)
{
    ResearchSiteDataMap::const_iterator itr = _researchSiteDataMap.find(id);
    if (itr != _researchSiteDataMap.end())
        return itr->second.entry;

    return nullptr;
}

void DB2Manager::DeterminaAlternateMapPosition(uint32 mapId, float x, float y, float z, uint32* newMapId /*= nullptr*/, DBCPosition2D* newPos /*= nullptr*/)
{
    ASSERT(newMapId || newPos);
    WorldMapTransformsEntry const* transformation = nullptr;
    for (WorldMapTransformsEntry const* transform : sWorldMapTransformsStore)
    {
        if (transform->MapID != mapId)
            continue;

        if (transform->RegionMin.X > x || transform->RegionMax.X < x)
            continue;
        if (transform->RegionMin.Y > y || transform->RegionMax.Y < y)
            continue;
        if (transform->RegionMin.Z > z || transform->RegionMax.Z < z)
            continue;

        transformation = transform;
        break;
    }

    if (!transformation)
    {
        if (newMapId)
            *newMapId = mapId;

        if (newPos)
        {
            newPos->X = x;
            newPos->Y = y;
        }
        return;
    }

    if (newMapId)
        *newMapId = transformation->NewMapID;

    if (!newPos)
        return;

    if (transformation->RegionScale > 0.0f && transformation->RegionScale < 1.0f)
    {
        x = (x - transformation->RegionMin.X) * transformation->RegionScale + transformation->RegionMin.X;
        y = (y - transformation->RegionMin.Y) * transformation->RegionScale + transformation->RegionMin.Y;
    }

    newPos->X = x + transformation->RegionOffset.X;
    newPos->Y = y + transformation->RegionOffset.Y;
}

bool DB2Manager::IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId)
{
    if (!requiredTotemCategoryId)
        return true;

    if (!itemTotemCategoryId)
        return false;

    TotemCategoryEntry const* itemEntry = sTotemCategoryStore.LookupEntry(itemTotemCategoryId);
    if (!itemEntry)
        return false;

    TotemCategoryEntry const* reqEntry = sTotemCategoryStore.LookupEntry(requiredTotemCategoryId);
    if (!reqEntry)
        return false;

    if (itemEntry->TotemCategoryType != reqEntry->TotemCategoryType)
        return false;

    return (itemEntry->TotemCategoryMask & reqEntry->TotemCategoryMask) == reqEntry->TotemCategoryMask;
}

SkillRaceClassInfoEntry const* DB2Manager::GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_)
{
    SkillRaceClassInfoBounds bounds = _skillRaceClassInfoBySkill.equal_range(skill);
    for (SkillRaceClassInfoContainer::iterator itr = bounds.first; itr != bounds.second; ++itr)
    {
        if (itr->second->RaceMask && !(itr->second->RaceMask & (1 << (race - 1))))
            continue;
        if (itr->second->ClassMask && !(itr->second->ClassMask & (1 << (class_ - 1))))
            continue;

        return itr->second;
    }

    return nullptr;
}

std::vector<SpecializationSpellEntry const*> const* DB2Manager::GetSpecializationSpells(uint32 specId)
{
    SpecializationSpellsBySpecContainer::const_iterator itr = _specializationSpellsBySpec.find(specId);
    if (itr != _specializationSpellsBySpec.end())
        return &itr->second;

    return nullptr;
}

std::list<uint32> const* DB2Manager::GetSpellProcsPerMinuteModList(uint32 procID)
{
    SpellProcsPerMinuteModEntryListContainer::const_iterator itr = _spellProcsPerMinuteModEntryList.find(procID);
    if (itr != _spellProcsPerMinuteModEntryList.end())
        return &itr->second;

    return nullptr;
}

SpellTargetRestrictionsEntry const* DB2Manager::GetSpellTargetRestrioctions(uint32 spellId, uint16 difficulty)
{
    SpellRestrictionDiffContainer::const_iterator itr = _spellRestrictionDiff.find(spellId);
    if (itr != _spellRestrictionDiff.end())
        for (auto const& v : itr->second)
            if (v->DifficultyID == difficulty)
                return v;

    return nullptr;
}

uint32 DB2Manager::GetLearnSpell(uint32 trigerSpell)
{
    RevertLearnSpellContainer::const_iterator itr = _revertLearnSpell.find(trigerSpell);
    if (itr != _revertLearnSpell.end())
        return itr->second;

    return 0;
}

uint32 DB2Manager::GetSpellByTrigger(uint32 trigerSpell)
{
    ReversTriggerSpellContainer::const_iterator itr = _reversTriggerSpellList.find(trigerSpell);
    if (itr != _reversTriggerSpellList.end())
        return itr->second;

    return 0;
}

SpellEffectEntry const* DB2Manager::GetSpellEffectEntry(uint32 spellId, uint32 effect, uint8 difficulty)
{
    if (spellId == 9262) //hack fix Segmentation fault
        return nullptr;

    if (difficulty)
    {
        uint16 index = MAKE_PAIR16(effect, difficulty);
        SpellEffectDiffContainer::const_iterator itr = _spellEffectDiff.find(spellId);
        if (itr != _spellEffectDiff.end())
        {
            SpellEffectsMap const* effects = &itr->second.effects;
            SpellEffectsMap::const_iterator itrsecond = effects->find(index);
            if (itrsecond != effects->end())
                return itrsecond->second;
        }
    }
    else
    {
        SpellEffectContainer::const_iterator itr = _spellEffectMap.find(spellId);
        if (itr != _spellEffectMap.end())
            if (itr->second.effects[effect])
                return itr->second.effects[effect];
    }

    return nullptr;
}

std::set<uint32> const* DB2Manager::GetSpellCategory(uint32 category)
{
    SpellCategoryContainer::const_iterator v = _spellCategory.find(category);
    if (v != _spellCategory.end())
        return &v->second;

    return nullptr;
}

std::vector<ItemSpecOverrideEntry const*> const* DB2Manager::GetItemSpecOverrides(uint32 itemId) const
{
    auto itr = _itemSpecOverrides.find(itemId);
    if (itr != _itemSpecOverrides.end())
        return &itr->second;

    return nullptr;
}

PvPDifficultyEntry const* DB2Manager::GetBattlegroundBracketByLevel(uint32 mapID, uint32 level)
{
    PvPDifficultyEntry const* maxEntry = nullptr;
    for (PvPDifficultyEntry const* entry : sPvPDifficultyStore)
    {
        if (entry->MapID != mapID || entry->MinLevel > level)
            continue;

        if (entry->MaxLevel >= level)
            return entry;

        if (!maxEntry || maxEntry->MaxLevel < entry->MaxLevel)
            maxEntry = entry;
    }

    return maxEntry;
}

PvPDifficultyEntry const* DB2Manager::GetBattlegroundBracketById(uint32 mapID, BattlegroundBracketId id)
{
    for (PvPDifficultyEntry const* entry : sPvPDifficultyStore)
        if (entry->MapID == mapID && entry->GetBracketId() == id)
            return entry;

    return nullptr;
}

ChrSpecializationEntry const* DB2Manager::GetChrSpecializationByID(uint8 classID, uint32 ID)
{
    return _chrSpecializationByIndex[classID][ID];
}

 DB2Manager::PetFamilySpellsSet const* DB2Manager::GetPetFamilySpells(uint32 family)
 {
    auto itr = _petFamilySpells.find(family);
    if (itr != _petFamilySpells.end())
        return &itr->second;

    return nullptr;
 }

bool DB2Manager::ChrClassesXPowerTypesEntryComparator::Compare(ChrClassesXPowerTypesEntry const* left, ChrClassesXPowerTypesEntry const* right)
{
    if (left->ClassID != right->ClassID)
        return left->ClassID < right->ClassID;

    return left->PowerType < right->PowerType;
}

uint32 DB2Manager::GetPowerIndexByClass(uint32 powerType, uint32 classId) const
{
    return _powersByClass[classId][powerType];
}

AreaTableEntry const* DB2Manager::FindAreaEntry(uint32 area)
{
    auto data = _areaEntry.find(area);
    if (data != _areaEntry.end())
        return data->second;

    return nullptr;
}

uint32 DB2Manager::GetParentZoneOrSelf(uint32 zone)
{
    AreaTableEntry const* area = FindAreaEntry(zone);
    if (!area)
        return zone;

    return area->ParentAreaID ? area->ParentAreaID : zone;
}

char const* DB2Manager::GetPetName(uint32 petfamily)
{
    if (!petfamily)
        return nullptr;

    CreatureFamilyEntry const* family = sCreatureFamilyStore.LookupEntry(petfamily);
    if (family)
        return family->Name->Str[sObjectMgr->GetDBCLocaleIndex()] ? family->Name->Str[sObjectMgr->GetDBCLocaleIndex()] : nullptr;

    return nullptr;
}

MapDifficultyEntry const* DB2Manager::GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty)
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

        mapDiff = GetMapDifficultyData(mapId, Difficulty(tmpDiff)); // we are 10 normal or 25 normal
    }

    difficulty = Difficulty(tmpDiff);
    return mapDiff;
}

MapDifficultyEntry const* DB2Manager::GetDefaultMapDifficulty(uint32 mapID)
{
    auto itr = _mapDifficulty.find(mapID);
    if (itr == _mapDifficulty.end())
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

DungeonEncounterEntry const* DB2Manager::GetDungeonEncounterByDisplayID(uint32 displayID)
{
    auto data = _dungeonEncounterByDisplayID.find(displayID);
    if (data != _dungeonEncounterByDisplayID.end())
        return data->second;

    return nullptr;
}

MinorTalentEntry const* DB2Manager::GetMinorTalentBySpecAndPerkID(uint32 specID, uint32 perkID)
{
    if (MinorTalentEntry const* entry = _minorTalentByIndexStore[specID][perkID])
        return entry;

    return nullptr;
}

MapDifficultyEntry const* DB2Manager::GetMapDifficultyData(uint32 mapId, Difficulty difficulty)
{
    auto itr = _mapDifficulty.find(mapId);
    if (itr == _mapDifficulty.end())
        return nullptr;

    auto diffItr = itr->second.find(difficulty);
    if (diffItr == itr->second.end())
        return nullptr;

    return diffItr->second;
}

uint32 DB2Manager::GetSpellMisc(uint32 spellID, Difficulty /*difficultyID = DIFFICULTY_NONE*/)
{
    auto data = _spellMiscDifficulty.find(spellID);
    if (data != _spellMiscDifficulty.end())
        return data->second;

    return 0;
}


uint16 DB2Manager::GetQuestIDbyLineAndPos(uint8 lineID, uint8 pos)
{
    auto data = _questLineXQuest.find(MAKE_PAIR16(lineID, pos));
    if (data != _questLineXQuest.end())
        return data->second;

    return 0;
}

QuestLineXQuestEntry const* DB2Manager::GetQuestLineXQuestData(uint16 questID)
{
    for (QuestLineXQuestEntry const* entry : sQuestLineXQuestStore)
        if (entry->QuestID == questID)
            return entry;

    return nullptr;
}
