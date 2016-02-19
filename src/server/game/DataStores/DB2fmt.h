/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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

// x - skip<uint32>,        X - skip<uint8>,    s - char*,  f - float,  i - uint32,     b - uint8,  d - index (not included)
// n - index (included),    l - uint64,         t - uint16

#ifndef TRINITY_DB2SFRM_H
#define TRINITY_DB2SFRM_H

char const AchievementFormat[] = "nssistttttttbib";
char const AreaGroupMemberFormat[] = "nii";
char const AreaTableFormat[] = "niisfstttttttttttttbbbbbbbbb";
char const AreaTriggerFormat[] = "nifffiiifffffiiii";
char const ArmorLocationFormat[] = "nfffff";
char const AuctionHouseFormat[] = "nsiii";
char const BankBagSlotPricesFormat[] = "ni";
char const BarberShopStyleFormat[] = "nissfbbbb";
char const BattlePetAbilityEffectFormat[] = "niiiiiiiiiii";
char const BattlePetAbilityFormat[] = "niiiiiss";
char const BattlePetAbilityStateFormat[] = "niii";
char const BattlePetAbilityTurnFormat[] = "niiiii";
char const BattlePetBreedQualityFormat[] = "nif";
char const BattlePetBreedStateFormat[] = "niii";
char const BattlePetEffectPropertiesFormat[] = "nissssssiiiiii";
char const BattlePetSpeciesFormat[] = "niiiiiiss";
char const BattlePetSpeciesStateFormat[] = "niii";
char const BattlePetSpeciesXAbilityFormat[] = "niiii";
char const BattlePetStateFormat[] = "nisi";
char const BroadcastTextFormat[] = "nssttttttttbb";
char const CharacterLoadoutItemFormat[]= "nit";
char const CharStartOutfitFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiibbbbb";
char const CharTitlesFormat[] = "nxssix";
char const ChatChannelsEntryFormat[] = "nissb";
char const ChrClassesXPowerTypesFormat[] = "ibb";
char const ChrRacesFormat[] = "nissssssssiiffffffittttttttttttbbbbbbbbb";
char const ChrSpecializationFormat[] = "nsiiiiiiiisssii";
char const CreatureDifficultyFormat[] = "niiiiiitbbb";
char const CreatureDisplayInfoFormat[] = "niffiiisiiittttttbbbbbb";
char const CreatureFamilyFormat[] = "nffsstttbbbb";
char const CreatureTypeFormat[] = "nsb";
char const CriteriaFormat[]= "niiitttbbbbb";
char const CriteriaTreeFormat[] = "nisttttb";
char const CurrencyTypesFormat[] = "nisssiiiiiis";
char const CurvePointFormat[] = "niiff";
char const DestructibleModelDataFormat[] = "ntttttbbbbbbbbbbbbbbbbb";
char const DifficultyFormat[] = "niiiiiiiiiisii";
char const DungeonEncounterFormat[] = "niixisiix";
char const DurabilityCostsFormat[] = "nttttttttttttttttttttttttttttt";
char const DurabilityQualityFormat[] = "nf";
char const EmotesEntryFormat[] = "nsiittbb";
char const EmotesTextFormat[] = "nxixxxxxxxxxxxxxxxx";
char const GameObjectDisplayInfoFormat[] = "nifffffffft";
char const GameObjectsFormat[] = "niiffffffffiiiiiiiiiiiis";
char const GameTablesFormat[] = "nstb";
char const GarrAbilityEffectFormat[] = "niiiiifffii";
char const GarrAbilityFormat[] = "nissiii";
char const GarrBuildingFormat[] = "niiiiissssiiiiiiiiiiiiii";
char const GarrBuildingPlotInstFormat[] = "niiiff";
char const GarrClassSpecFormat[] = "nsssii";
char const GarrEncounterFormat[] = "nisffi";
char const GarrEncounterXMechanicFormat[] = "nii";
char const GarrFollowerFormat[] = "niiiiiiiiiiiiiissiiii";
char const GarrFollowerLevelXPFormat[] = "niii";
char const GarrFollowerQualityFormat[] = "niii";
char const GarrFollowerXAbilityFormat[] = "niii";
char const GarrMechanicFormat[] = "nif";
char const GarrMechanicTypeFormat[] = "nissi";
char const GarrMissionFormat[] = "niiiiiiiiisssiiiiii";
char const GarrMissionRewardFormat[] = "niiiiiiiii";
char const GarrMissionXEncounterFormat[] = "nii";
char const GarrPlotBuildingFormat[] = "nii";
char const GarrPlotFormat[] = "niiisiiii";
char const GarrPlotInstanceFormat[] = "nis";
char const GarrSiteLevelFormat[] = "niiiiffiiii";
char const GarrSiteLevelPlotInstFormat[] = "niiffi";
char const GemPropertiesFormat[] = "nttbbb";
char const GlyphPropertiesFormat[] = "niiii";
char const GuildPerkSpellsFormat[] = "nib";
char const HeirloomFormat[] = "niisiiiiiiii";
char const HolidaysFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiisiii";
char const ImportPriceArmorFormat[] = "nffff";
char const ImportPriceQualityFormat[] = "nf";
char const ImportPriceShieldFormat[] = "nf";
char const ImportPriceWeaponFormat[] = "nf";
char const ItemAppearanceFormat[] = "inib";
char const ItemArmorQualityFormat[] = "nffffffft";
char const ItemArmorShieldFormat[] = "nffffffft";
char const ItemArmorTotalFormat[] = "niffff";
char const ItemBonusFormat[] = "niiiii";
char const ItemBonusTreeNodeFormat[] = "niiii";
char const ItemClassFormat[] = "nifs";
char const ItemCurrencyCostFormat[] = "in";
char const ItemDamageFormat[] = "nfffffffi";
char const ItemDisenchantLootFormat[] = "ntttbbb";
char const ItemEffectFormat[] = "niiiittbbb";
char const ItemExtendedCostFormat[] = "niiiiiiiiiiitttttttttttbbbbb";
char const ItemFormat[] = "nibbbbbbb";
char const ItemLimitCategoryFormat[] = "nsbb";
char const ItemModifiedAppearanceFormat[] = "nitbb";
char const ItemPriceBaseFormat[] = "nfft";
char const ItemRandomPropertiesFormat[] = "nsiiiii";
char const ItemRandomSuffixFormat[] = "nssiiiiiiiiii";
char const ItemSetFormat[] = "nsiiiiiiiiiiiiiiiiitb";
char const ItemSetSpellFormat[] = "nittb";
char const ItemSparseFormat[] = "niiiffiiiiiiiiiiiiiiiiifffffffffffsssssififtttttttttttttttttttttttttttttbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const ItemSpecFormat[]= "ntbbbbb";
char const ItemSpecOverrideFormat[]= "nit";
char const ItemToBattlePetSpeciesFormat[] = "ni";
char const ItemUpgradeFormat[] = "niiiii";
char const ItemXBonusTreeFormat[] = "nii";
char const KeyChainFormat[] = "nbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const LanguageWordsFormat[] = "nsb";
char const LockFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const MailTemplateFormat[] = "ns";
char const MapChallengeModeFormat[] = "niiiiiiiii";
char const MapFormat[] = "nsiiiisissififfiiiiiii";
char const MinorTalentFormat[] = "niii";
char const ModifierTreeFormat[]= "niitbbbb";
char const MountCapabilityFormat[] = "niiitttbb";
char const MountFormat[] = "niissstttb";
char const MountTypeFormat[] = "ni";
char const MountTypeXCapabilityFormat[] = "nttb";
char const MovieFormat[] = "niibb";
char const NameGenFormat[] = "nsbb";
char const NamesProfanityFormat[] = "nSb";
char const NamesReservedFormat[] = "nS";
char const NamesReservedLocaleFormat[] = "nSb";
char const OverrideSpellDataFormat[] = "niiiiiiiiiiib";
char const PhaseFormat[] = "ni";
char const PowerDisplayFormat[]= "nsbbbb";
char const PvPDifficultyFormat[] = "ntbbb";
char const PvpItemFormat[] = "nib";
char const QuestFactionRewardFormat[] = "niiiiiiiiii";
char const QuestPackageItemFormat[] = "nitbb";
char const QuestPOIPointFormat[] = "nitt";
char const QuestSortFormat[] = "ns";
char const QuestV2Format[] = "nt";
char const QuestXPFormat[] = "ntttttttttt";
char const QuestLineFormat[] = "ns";
char const QuestLineXQuestFormat[] = "ntbb";
char const QuestMoneyRewardFormat[] = "niiiiiiiiii";
char const QuestObjectiveFormat[] = "niistbbbb";
char const RandPropPointsStoreFormat[] = "nttttttttttttttt";
char const ResearchBranchFormat[] = "nssitb";
char const ResearchProjectFormat[] = "nssiiiixi";
char const ResearchSiteFormat[] = "niisx";
char const RuleSetItemUpgradeFormat[] = "niii";
char const ScalingStatDistributionFormat[] = "ntbb";
char const ScenarioFormat[]= "nsbbb";
char const ScenarioStepFormat[]= "nssttttbbb";
char const SkillLineAbilityFormat[] = "niiiittttttbb";
char const SkillLineFormat[] = "nsssttbbb";
char const SkillRaceClassInfoFormat[] = "nittttbb";
char const SoundEntriesFormat[] = "nisiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiififfiifffffii";
char const SpecializationSpellsFormat[] = "niistb";
char const SpellAuraOptionsFormat[] = "iniiitbbb";
char const SpellAuraRestrictionsFormat[] = "iniiiibbbbb";
char const SpellCastingRequirementsFormat[] = "intttbbb";
char const SpellCastTimeFormat[] = "niit";
char const SpellCategoriesFormat[] = "intttbbbbb";
char const SpellCategoryFormat[] = "nsibbb";
char const SpellClassOptionsFormat[] = "iniiiitb";
char const SpellCooldownsFormat[] = "iniiib";
char const SpellDurationFormat[] = "niit";
char const SpellEffectFormat[] = "ifiiffiiiiffiiiiifiifttbbbbbbbb";
char const SpellEffectScalingFormat[] = "nfffi";
char const SpellEquippedItemsFormat[] = "iniib";
char const SpellFocusObjectFormat[] = "ns";
char const SpellFormat[] = "nssssii";
char const SpellInterruptsFormat[] = "iniiiitb";
char const SpellItemEnchantmentConditionFormat[] = "nbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const SpellLearnSpellFormat[] = "niii";
char const SpellLevelsFormat[] = "intttbb";
char const SpellMiscFormat[] = "niiiiiiiiiiiiiifftttttb";
char const SpellMiscDifficultyFormat[] = "inb";
char const SpellPowerFormat[] = "niiffifttttbbb";
char const SpellProcsPerMinuteFormat[] = "nfb";
char const SpellProcsPerMinuteModFormat[] = "nftbb";
char const SpellRadiusFormat[] = "nffff";
char const SpellRangeFormat[] = "nffffssb";
char const SpellReagentsCurrencyFormat[] = "intt";
char const SpellReagentsFormat[] = "iniiiiiiiitttttttt";
char const SpellScalingFormat[] = "intbb";
char const SpellShapeshiftFormat[] = "iniiiib";
char const SpellShapeshiftFormFormat[] = "nsfittttttttttttttbbb";
char const SpellTargetRestrictionsFormat[] = "niffittbb";
char const SpellTotemsFormat[] = "iniibb";
char const SpellVisualFormat[] = "niiiiiiiiiiiiiiiiiiffffffiiiii";
char const SpellXSpellVisualFormat[] = "inftttbb";
char const SummonPropertiesFormat[] = "niiiii";
char const TaxiNodesFormat[] = "nifffsiiiiiff";
char const TaxiPathFormat[] = "niii";
char const TaxiPathNodeFormat[] = "niiifffiiii";
char const TotemCategoryFormat[] = "nsib";
char const ToyFormat[] = "niisi";
char const TransportAnimationFormat[] = "niifffb";
char const UnitPowerBarFormat[] = "niffiiiiiiiiiiiissssffttbbb";
char const VehicleSeatFormat[] = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiffffffffffffiiiiiiiii";
char const WorldMapOverlayFormat[] = "nstttttttttttttt";
char const WorldMapTransformsFormat[] = "nfffffffffttttb";

#endif

