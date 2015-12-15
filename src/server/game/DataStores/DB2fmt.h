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

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - uint64, p - field present in sql dbc, a - field absent in sql dbc,
// t - uint16

#ifndef TRINITY_DB2SFRM_H
#define TRINITY_DB2SFRM_H

char const AchievementFormat[] = "nssistttttttbib";
char const AreaGroupFormat[] = "n";
char const AreaGroupMemberFormat[] = "nii";
char const ArmorLocationFormat[] = "fffff";
char const AuctionHouseFormat[] = "stbb";
char const BankBagSlotPricesFormat[] = "i";
char const BarberShopStyleFormat[] = "issfbbbb";
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
char const BroadcastTextFormat[] = "nissiiiiiiiii";
char const CharacterLoadoutItemFormat[]= "nii";
char const CharStartOutfitFormat[] = "iiiiiiiiiiiiiiiiiiiiiiiiibbbbb";
char const ChrClassesXPowerTypesFormat[] = "bb";
char const ChrRacesFormat[] = "iissssssssiiffffffittttttttttttbbbbbbbbb";
char const CreatureDifficultyFormat[] = "niiiiiiiiii";
char const CreatureDisplayInfoExtraFormat[] = "iiiiiiiiiiiibbbbbbbbbbbb";
char const CreatureDisplayInfoFormat[] = "nixifxxxxxxxxxxxxxxix";
char const CreatureTypeFormat[] = "sb";
char const CriteriaFormat[]= "iiitttbbbbb";
char const CriteriaTreeFormat[] = "isttttb";
char const CurrencyTypesFormat[] = "nisssiiiiiis";
char const CurvePointFormat[] = "niiff";
char const DestructibleModelDataFormat[] = "ixxixxxixxxixxxixxxxxxxx";
char const DurabilityCostsFormat[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const DurabilityQualityFormat[] = "f";
char const GameObjectDisplayInfoFormat[] = "iffffffffttttttttttt";
char const GameObjectsFormat[] = "niiffffffffiiiiiiiiiiiis";
char const GameTablesFormat[] = "stb";
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
char const GuildPerkSpellsFormat[] = "ib";
char const HeirloomFormat[] = "niisiiiiiiii";
char const HolidaysFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiisiii";
char const ImportPriceArmorFormat[] = "ffff";
char const ImportPriceQualityFormat[] = "f";
char const ImportPriceShieldFormat[] = "f";
char const ImportPriceWeaponFormat[] = "f";
char const ItemAppearanceFormat[] = "nii";
char const ItemArmorQualityFormat[] = "ffffffft";
char const ItemArmorShieldFormat[] = "ffffffft";
char const ItemArmorTotalFormat[] = "niffff";
char const ItemBonusFormat[] = "niiiii";
char const ItemBonusTreeNodeFormat[] = "niiii";
char const ItemClassFormat[] = "difx";
char const ItemCurrencyCostFormat[] = "in";
char const ItemDamageFormat[] = "nfffffffi";
char const ItemDisenchantLootFormat[] = "niiiiii";
char const ItemEffectFormat[] = "niiiiiiii";
char const ItemExtendedCostFormat[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const ItemFormat[] = "niiiiiiii";
char const ItemLimitCategoryFormat[] = "nxii";
char const ItemModifiedAppearanceFormat[] = "niiiii";
char const ItemPriceBaseFormat[] = "diff";
char const ItemRandomPropertiesFormat[] = "nxiiixxs";
char const ItemRandomSuffixFormat[] = "nsxiiiiiiiiii";
char const ItemSetSpellFormat[] = "niiii";
char const ItemSparseFormat[] = "niiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffffffiiifisssssiiiiiiiiiiiiiiiiiiifiiifiii";
char const ItemSpecFormat[]= "niiiiii";
char const ItemSpecOverrideFormat[]= "nii";
char const ItemToBattlePetSpeciesFormat[] = "ni";
char const ItemUpgradeFormat[] = "niiiii";
char const ItemXBonusTreeFormat[] = "nii";
char const KeyChainFormat[] = "nbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const LanguageWordsFormat[] = "dis";
char const MailTemplateFormat[] = "s";
char const MapChallengeModeFormat[] = "niiiiiiiii";
char const ModifierTreeFormat[]= "niiiixi";
char const MountCapabilityFormat[] = "niiiiiii";
char const MountFormat[] = "niiiisssii";
char const MountTypeFormat[] = "niiiiiiiiiiiiiiiiiiiiiiii";
char const MountTypeXCapabilityFormat[] = "niii";
char const NameGenFormat[] = "dsii";
char const OverrideSpellDataFormat[] = "niiiiiiiiiiii";
char const PhaseGroupFormat[] = "nii";
char const PvpItemFormat[] = "ini";
char const QuestFactionRewardFormat[] = "niiiiiiiiii";
char const QuestPackageItemFormat[] = "niiii";
char const QuestPOIPointFormat[] = "diii";
char const QuestSortFormat[] = "s";
char const QuestV2Format[] = "t";
char const QuestXPFormat[] = "tttttttttt";
char const RandPropPointsStoreFormat[] = "niiiiiiiiiiiiiii";
char const ResearchBranchFormat[] = "nsxixi";
char const ResearchProjectFormat[] = "nssiiiixi";
char const ResearchSiteFormat[] = "niisx";
char const RuleSetItemUpgradeFormat[] = "niii";
char const ScalingStatDistributionFormat[] = "tbb";
char const ScenarioFormat[]= "sbb";
char const ScenarioStepFormat[]= "niiixxiiii";
char const SkillLineAbilityFormat[] = "iiiittttttbb";
char const SkillLineFormat[] = "sssttbbb";
char const SkillRaceClassInfoFormat[] = "diiiiiii";
char const SoundEntriesFormat[] = "nisiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiififfiifffffii";
char const SpecializationSpellsFormat[] = "niistb";
char const SpellAuraOptionsFormat[] = "nxxiiiiii";
char const SpellAuraRestrictionsFormat[] = "niiiiiiii";
char const SpellCastingRequirementsFormat[] = "niiiiii";
char const SpellCastTimeFormat[] = "iit";
char const SpellCategoriesFormat[] = "itttbbbbb";
char const SpellCategoryFormat[] = "sibbb";
char const SpellClassOptionsFormat[] = "niiiiii";
char const SpellCooldownsFormat[] = "iiiib";
char const SpellDurationFormat[] = "iit";
char const SpellEffectFormat[] = "ifiiffiiiiffiiiiifiifttbbbbbbbbb";
char const SpellEffectScalingFormat[] = "fffi";
char const SpellFormat[] = "nssxxiiiiiiiiiiiiiiiiiii";
char const SpellEquippedItemsFormat[] = "iiib";
char const SpellFocusObjectFormat[] = "s";
char const SpellInterruptsFormat[] = "iiiiitb";
char const SpellItemEnchantmentConditionFormat[] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
char const SpellLearnSpellFormat[] = "niii";
char const SpellLevelsFormat[] = "itttb";
char const SpellMiscFormat[] = "niiiiiiiiiiiiiiiiifiiiiif";
char const SpellPowerFormat[] = "niiiiiiiiiffif";
char const SpellProcsPerMinuteFormat[] = "fb";
char const SpellProcsPerMinuteModFormat[] = "ftbb";
char const SpellRadiusFormat[] = "ffff";
char const SpellRangeFormat[] = "ffffssb";
char const SpellReagentsFormat[] = "niiiiiiiiiiiiiiiiii";
char const SpellRuneCostFormat[] = "niiiii";
char const SpellScalingFormat[] = "diiiifiii";
char const SpellShapeshiftFormat[] = "nixixx";
char const SpellShapeshiftFormFormat[] = "sfittttttttttttttbbb";
char const SpellTargetRestrictionsFormat[] = "iffittbb";
char const SpellTotemsFormat[] = "niiii";
char const SpellVisualFormat[] = "niiiiiiiiiiiiiiiiiiffffffiiiii";
char const TaxiNodesFormat[] = "nifffsiiiiiff";
char const TaxiPathFormat[] = "niii";
char const TaxiPathNodeFormat[] = "niiifffiiii";
char const TotemCategoryFormat[] = "isb";
char const ToyFormat[] = "niisi";
char const TransportAnimationFormat[] = "iifffb";
char const TransportRotationFormat[] = "iiffff";
char const UnitPowerBarFormat[] = "niiiiffixxxxxxxxxxxxixxxxxx";
char const WorldMapOverlayFormat[] = "stttttttttttttt";
char const WorldMapTransformsFormat[] = "iiiiiiiiittttb";

#endif
