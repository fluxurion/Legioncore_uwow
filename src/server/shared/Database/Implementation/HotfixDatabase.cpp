/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#include "HotfixDatabase.h"

// Force locale statments to appear exactly in locale declaration order, right after normal data fetch statement
#define PREPARE_LOCALE_STMT(stmtBase, sql, con) \
    static_assert(stmtBase + 1 == stmtBase##_LOCALE, "Invalid prepared statement index for " #stmtBase "_LOCALE"); \
    PrepareStatement(stmtBase##_LOCALE, sql, con);

void HotfixDatabaseConnection::DoPrepareStatements()
{
    return;

    if (!m_reconnecting)
        m_stmts.resize(MAX_HOTFIXDATABASE_STATEMENTS);

    // Achievement.db2
    PrepareStatement(HOTFIX_SEL_ACHIEVEMENT, "SELECT ID, Name, Description, Flags, Reward, MapID, ParentAchievement, CategoryID, OrderInCategory, "
        "IconID, RefAchievement, CriteriaTree, FactionFlag, RewardPoints, Amount FROM achievement ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ACHIEVEMENT, "SELECT ID, Name_lang, Description_lang, Reward_lang FROM achievement_locale WHERE locale = ?", CONNECTION_SYNCH);

    // AreaGroup.db2
    PrepareStatement(HOTFIX_SEL_AREA_GROUP, "SELECT ID FROM area_group ORDER BY ID DESC", CONNECTION_SYNCH);

    // AreaGroupMember.db2
    PrepareStatement(HOTFIX_SEL_AREA_GROUP_MEMBER, "SELECT ID, AreaGroupID, AreaID FROM area_group_member ORDER BY ID DESC", CONNECTION_SYNCH);

    // AuctionHouse.db2
    PrepareStatement(HOTFIX_SEL_AUCTION_HOUSE, "SELECT Name, FactionID, DepositRate, ConsignmentRate, houseId FROM auction_house ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_AUCTION_HOUSE, "SELECT Name, Name_lang FROM auction_house_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ArmorLocation.db2
    PrepareStatement(HOTFIX_SEL_ARMOR_LOCATION, "SELECT Value1, Value2, Value3, Value4, Value5 FROM armor_location ORDER BY Value[5] DESC", CONNECTION_SYNCH);

    // BankBagSlotPrices.db2
    PrepareStatement(HOTFIX_SEL_BANK_BAG_SLOT_PRICES, "SELECT price FROM bank_bag_slot_prices ORDER BY price DESC", CONNECTION_SYNCH);

    // BarberShopStyle.db2
    PrepareStatement(HOTFIX_SEL_BARBER_SHOP_STYLE, "SELECT Type, Name, Description, CostMultiplier, Race, Gender, HairID, Tato FROM barber_shop_style"
        " ORDER BY Type DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BARBER_SHOP_STYLE, "SELECT Type, Name_lang, Description_lang FROM barber_shop_style_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BroadcastText.db2
    PrepareStatement(HOTFIX_SEL_BROADCAST_TEXT, "SELECT ID, Language, MaleText, FemaleText, EmoteID1, EmoteID2, EmoteID3, EmoteDelay1, EmoteDelay2, "
        "EmoteDelay3, SoundID, UnkEmoteID, Type FROM broadcast_text ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BROADCAST_TEXT, "SELECT ID, MaleText_lang, FemaleText_lang FROM broadcast_text_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CurvePoint.db2
    PrepareStatement(HOTFIX_SEL_CURVE_POINT, "SELECT ID, CurveID, `Index`, X, Y FROM curve_point ORDER BY ID DESC", CONNECTION_SYNCH);

    // Criteria.db2
    PrepareStatement(HOTFIX_SEL_CRITERIA, "SELECT CreatureID, MapID, SkillID, AchievementID, ZoneID, CurrencyID, ManLimit, RaidSize, "
        "EnviromentalDamageType, QuestID, SpellID, ObjectiveID, AreaID, ItemID, TeamType, rank, FactionID, ItemSlot, RollValue, ClassID, RaceID, "
        "EmoteID, GoEntry, ScriptDataVal, SkillLine, AddPet, LootType, LevelUP, GarrBuildingID, GarrisonLevel, GarrMissionID, "
        "CharShipmentContainerID, StartAsset, FailAsset, StartTimer, ModifyTree, Flags, Type, StartEvent, FailEvent, EligibilityWorldStateID, "
        "EligibilityWorldStateValue, ID FROM criteria ORDER BY CreatureID DESC", CONNECTION_SYNCH);

    // CriteriaTree.db2
    PrepareStatement(HOTFIX_SEL_CRITERIA_TREE, "SELECT Amount, DescriptionLang, CriteriaID, Parent, flags2, UNK, Flags, ID FROM criteria_tree"
        " ORDER BY Amount DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CRITERIA_TREE, "SELECT Amount, DescriptionLang_lang FROM criteria_tree_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CharacterLoadoutItem.db2
    PrepareStatement(HOTFIX_SEL_CHARACTER_LOADOUT_ITEM, "SELECT LoadOutID, ItemID FROM character_loadout_item ORDER BY LoadOutID DESC", CONNECTION_SYNCH);

    // CharStartOutfit.db2
    PrepareStatement(HOTFIX_SEL_CHAR_START_OUTFIT, "SELECT ItemID1, ItemID2, ItemID3, ItemID4, ItemID5, ItemID6, ItemID7, ItemID8, ItemID9, ItemID10, "
        "ItemID11, ItemID12, ItemID13, ItemID14, ItemID15, ItemID16, ItemID17, ItemID18, ItemID19, ItemID20, ItemID21, ItemID22, ItemID23, ItemID24, "
        "PetDisplayID, RaceID, ClassID, GenderID, OutfitID, PetFamilyID FROM char_start_outfit ORDER BY ItemID[MAX_OUTFIT_ITEMS] DESC", CONNECTION_SYNCH);

    // ChrClassesXPowerTypes.db2
    PrepareStatement(HOTFIX_SEL_CHR_CLASSES_X_POWER_TYPES, "SELECT ClassID, PowerID FROM chr_classes_x_power_types ORDER BY ClassID DESC", CONNECTION_SYNCH);

    // ChrRaces.db2
    PrepareStatement(HOTFIX_SEL_CHR_RACES, "SELECT ID, Flags, ClientPrefix, ClientFileString, Name, NameFemale, NameMale, FacialHairCustomization1, "
        "FacialHairCustomization2, HairCustomization, CreateScreenFileDataID, SelectScreenFileDataID, MaleCustomizeOffset1, MaleCustomizeOffset2, "
        "MaleCustomizeOffset3, FemaleCustomizeOffset1, FemaleCustomizeOffset2, FemaleCustomizeOffset3, LowResScreenFileDataID, FactionID, "
        "ExplorationSoundID, MaleDisplayID, FemaleDisplayID, ResSicknessSpellID, SplashSoundID, CinematicSequenceID, UAMaleCreatureSoundDataID, "
        "UAFemaleCreatureSoundDataID, HighResMaleDisplayID, HighResFemaleDisplayID, Unk, BaseLanguage, CreatureType, TeamID, RaceRelated, "
        "UnalteredVisualRaceID, CharComponentTexLayoutHiResID, DefaultClassID, NeutralRaceID, CharComponentTextureLayoutID FROM chr_races"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHR_RACES, "SELECT ID, ClientPrefix_lang, ClientFileString_lang, Name_lang, NameFemale_lang, NameMale_lang, "
        "FacialHairCustomization1_lang, FacialHairCustomization2_lang, HairCustomization_lang FROM chr_races_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CreatureDisplayInfoExtra.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_DISPLAY_INFO_EXTRA, "SELECT NPCItemDisplay1, NPCItemDisplay2, NPCItemDisplay3, NPCItemDisplay4, "
        "NPCItemDisplay5, NPCItemDisplay6, NPCItemDisplay7, NPCItemDisplay8, NPCItemDisplay9, NPCItemDisplay10, FileDataID, Unk, DisplayRaceID, "
        "DisplaySexID, SkinID, FaceID, HairStyleID, HairColorID, FacialHairID, Tato1, Tato2, Tato3, Flags FROM creature_display_info_extra"
        " ORDER BY NPCItemDisplay[10] DESC", CONNECTION_SYNCH);

    // CreatureDisplayInfo.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_DISPLAY_INFO, "SELECT ID, ModelID, SoundID, ExtendedDisplayInfoID, CreatureModelScale, CreatureModelAlpha, "
        "TextureVariation1, TextureVariation2, TextureVariation3, PortraitTextureName, PortraitCreatureDisplayInfoID, SizeClass, BloodID, NPCSoundID, "
        "ParticleColorID, CreatureGeosetData, ObjectEffectPackageID, AnimReplacementSetID, Flags, Gender, StateSpellVisualKitID"
        " FROM creature_display_info ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CREATURE_DISPLAY_INFO, "SELECT ID, TextureVariation1_lang, TextureVariation2_lang, TextureVariation3_lang, "
        "PortraitTextureName_lang FROM creature_display_info_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CreatureType.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_TYPE, "SELECT Name, Flags FROM creature_type ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CREATURE_TYPE, "SELECT Name, Name_lang FROM creature_type_locale WHERE locale = ?", CONNECTION_SYNCH);

    // DestructibleModelData.db2
    PrepareStatement(HOTFIX_SEL_DESTRUCTIBLE_MODEL_DATA, "SELECT StateDamagedDisplayID, StateDestroyedDisplayID, StateRebuildingDisplayID, "
        "StateSmokeDisplayID, HealEffectSpeed, StateDamagedImpactEffectDoodadSet, StateDamagedAmbientDoodadSet, StateDamagedNameSet, "
        "StateDestroyedDestructionDoodadSet, StateDestroyedImpactEffectDoodadSet, StateDestroyedAmbientDoodadSet, StateDestroyedNameSet, "
        "StateRebuildingDestructionDoodadSet, StateRebuildingImpactEffectDoodadSet, StateRebuildingAmbientDoodadSet, StateRebuildingNameSet, "
        "StateSmokeInitDoodadSet, StateSmokeAmbientDoodadSet, StateSmokeNameSet, EjectDirection, DoNotHighlight, HealEffect"
        " FROM destructible_model_data ORDER BY StateDamagedDisplayID DESC", CONNECTION_SYNCH);

    // DurabilityCosts.db2
    PrepareStatement(HOTFIX_SEL_DURABILITY_COSTS, "SELECT WeaponSubstructCost1, WeaponSubstructCost2, WeaponSubstructCost3, WeaponSubstructCost4, "
        "WeaponSubstructCost5, WeaponSubstructCost6, WeaponSubstructCost7, WeaponSubstructCost8, WeaponSubstructCost9, WeaponSubstructCost10, "
        "WeaponSubstructCost11, WeaponSubstructCost12, WeaponSubstructCost13, WeaponSubstructCost14, WeaponSubstructCost15, WeaponSubstructCost16, "
        "WeaponSubstructCost17, WeaponSubstructCost18, WeaponSubstructCost19, WeaponSubstructCost20, WeaponSubstructCost21, ArmorSubstructCost1, "
        "ArmorSubstructCost2, ArmorSubstructCost3, ArmorSubstructCost4, ArmorSubstructCost5, ArmorSubstructCost6, ArmorSubstructCost7, "
        "ArmorSubstructCost8 FROM durability_costs ORDER BY WeaponSubstructCost[21] DESC", CONNECTION_SYNCH);

    // DurabilityQuality.db2
    PrepareStatement(HOTFIX_SEL_DURABILITY_QUALITY, "SELECT Data FROM durability_quality ORDER BY Data DESC", CONNECTION_SYNCH);

    // GameObjectDisplayInfo.db2
    PrepareStatement(HOTFIX_SEL_GAME_OBJECT_DISPLAY_INFO, "SELECT FileDataID, GeoBoxMinX, GeoBoxMinY, GeoBoxMinZ, GeoBoxMaxX, GeoBoxMaxY, GeoBoxMaxZ, "
        "OverrideLootEffectScale, OverrideNameScale, Sound1, Sound2, Sound3, Sound4, Sound5, Sound6, Sound7, Sound8, Sound9, Sound10, "
        "ObjectEffectPackageID FROM game_object_display_info ORDER BY FileDataID DESC", CONNECTION_SYNCH);

    // ImportPriceArmor.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_ARMOR, "SELECT ClothModifier, LeatherModifier, ChainModifier, PlateModifier FROM import_price_armor"
        " ORDER BY ClothModifier DESC", CONNECTION_SYNCH);

    // ImportPriceQuality.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_QUALITY, "SELECT Data FROM import_price_quality ORDER BY Data DESC", CONNECTION_SYNCH);

    // ImportPriceShield.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_SHIELD, "SELECT Data FROM import_price_shield ORDER BY Data DESC", CONNECTION_SYNCH);

    // ImportPriceWeapon.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_WEAPON, "SELECT Data FROM import_price_weapon ORDER BY Data DESC", CONNECTION_SYNCH);

    // ItemArmorQuality.db2
    PrepareStatement(HOTFIX_SEL_ITEM_ARMOR_QUALITY, "SELECT QualityMod1, QualityMod2, QualityMod3, QualityMod4, QualityMod5, QualityMod6, "
        "QualityMod7, ItemLevel FROM item_armor_quality ORDER BY QualityMod[7] DESC", CONNECTION_SYNCH);

    // ItemArmorShield.db2
    PrepareStatement(HOTFIX_SEL_ITEM_ARMOR_SHIELD, "SELECT QualityMod1, QualityMod2, QualityMod3, QualityMod4, QualityMod5, QualityMod6, QualityMod7, "
        "ItemLevel FROM item_armor_shield ORDER BY QualityMod[7] DESC", CONNECTION_SYNCH);

    // ItemArmorTotal.db2
    PrepareStatement(HOTFIX_SEL_ITEM_ARMOR_TOTAL, "SELECT Value1, Value2, Value3, Value4, ItemLevel FROM item_armor_total ORDER BY Value[4] DESC", CONNECTION_SYNCH);

    // ItemDamage.db2
    PrepareStatement(HOTFIX_SEL_ITEM_DAMAGE, "SELECT Quality1, Quality2, Quality3, Quality4, Quality5, Quality6, Quality7, ItemLevel FROM item_damage"
        " ORDER BY Quality[7] DESC", CONNECTION_SYNCH);

    // ItemClass.db2
    PrepareStatement(HOTFIX_SEL_ITEM_CLASS, "SELECT Class, PriceFactor, Name FROM item_class ORDER BY Class DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_CLASS, "SELECT Class, Name_lang FROM item_class_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemDisenchantLoot.db2
    PrepareStatement(HOTFIX_SEL_ITEM_DISENCHANT_LOOT, "SELECT MinLevel, MaxLevel, SkillRequired, ItemClass, ItemSubClass, Quality, Id"
        " FROM item_disenchant_loot ORDER BY MinLevel DESC", CONNECTION_SYNCH);

    // ItemLimitCategory.db2
    PrepareStatement(HOTFIX_SEL_ITEM_LIMIT_CATEGORY, "SELECT NameLang, Quantity, Flags FROM item_limit_category ORDER BY NameLang DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_LIMIT_CATEGORY, "SELECT NameLang, NameLang_lang FROM item_limit_category_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemPriceBase.db2
    PrepareStatement(HOTFIX_SEL_ITEM_PRICE_BASE, "SELECT Armor, Weapon, ItemLevel FROM item_price_base ORDER BY Armor DESC", CONNECTION_SYNCH);

    // ItemRandomProperties.db2
    PrepareStatement(HOTFIX_SEL_ITEM_RANDOM_PROPERTIES, "SELECT Name, Enchantment1, Enchantment2, Enchantment3, Enchantment4, Enchantment5, ID"
        " FROM item_random_properties ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_RANDOM_PROPERTIES, "SELECT Name, Name_lang FROM item_random_properties_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemRandomSuffix.db2
    PrepareStatement(HOTFIX_SEL_ITEM_RANDOM_SUFFIX, "SELECT Name, IternalName, Enchantment1, Enchantment2, Enchantment3, Enchantment4, Enchantment5, "
        "AllocationPct1, AllocationPct2, AllocationPct3, AllocationPct4, AllocationPct5, ID FROM item_random_suffix ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_RANDOM_SUFFIX, "SELECT Name, Name_lang, IternalName_lang FROM item_random_suffix_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemSpecOverride.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPEC_OVERRIDE, "SELECT ItemID, SpecID FROM item_spec_override ORDER BY ItemID DESC", CONNECTION_SYNCH);

    // ItemSpec.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPEC, "SELECT SpecID, MinLevel, MaxLevel, ItemType, PrimaryStat, SecondaryStat FROM item_spec"
        " ORDER BY SpecID DESC", CONNECTION_SYNCH);

    // ItemSetSpell.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SET_SPELL, "SELECT SpellID, ItemSetID, ChrSpecID, Threshold FROM item_set_spell ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // GuildPerkSpells.db2
    PrepareStatement(HOTFIX_SEL_GUILD_PERK_SPELLS, "SELECT SpellId, Level FROM guild_perk_spells ORDER BY SpellId DESC", CONNECTION_SYNCH);

    // Heirloom.db2
    PrepareStatement(HOTFIX_SEL_HEIRLOOM, "SELECT ID, ItemID, Flags, SourceText, Source, OldItem1, OldItem2, NextDifficultyItemID, UpgradeItemID1, "
        "UpgradeItemID2, ItemBonusListID1, ItemBonusListID2 FROM heirloom ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_HEIRLOOM, "SELECT ID, SourceText_lang FROM heirloom_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Holidays.db2
    PrepareStatement(HOTFIX_SEL_HOLIDAYS, "SELECT ID, Duration1, Duration2, Duration3, Duration4, Duration5, Duration6, Duration7, Duration8, "
        "Duration9, Duration10, Date1, Date2, Date3, Date4, Date5, Date6, Date7, Date8, Date9, Date10, Date11, Date12, Date13, Date14, Date15, "
        "Date16, Region, Looping, CalendarFlags1, CalendarFlags2, CalendarFlags3, CalendarFlags4, CalendarFlags5, CalendarFlags6, CalendarFlags7, "
        "CalendarFlags8, CalendarFlags9, CalendarFlags10, HolidayNameID, HolidayDescriptionID, TextureFilename, Priority, CalendarFilterType, Flags"
        " FROM holidays ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_HOLIDAYS, "SELECT ID, TextureFilename_lang FROM holidays_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Item.db2
    PrepareStatement(HOTFIX_SEL_ITEM, "SELECT ID, FileDataID, Class, SubClass, SoundOverrideSubclass, Material, InventoryType, Sheath, GroupSoundsID"
        " FROM item ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemAppearance.db2
    PrepareStatement(HOTFIX_SEL_ITEM_APPEARANCE, "SELECT ID, DisplayID, FileDataID FROM item_appearance ORDER BY ID DESC", CONNECTION_SYNCH);

    // MailTemplate.db2
    PrepareStatement(HOTFIX_SEL_MAIL_TEMPLATE, "SELECT Content FROM mail_template ORDER BY Content DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_MAIL_TEMPLATE, "SELECT Content, Content_lang FROM mail_template_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ModifierTree.db2
    PrepareStatement(HOTFIX_SEL_MODIFIER_TREE, "SELECT Asset, SecondaryAsset, Parent, Type, UnkLegion, Operator, Amount, ID FROM modifier_tree"
        " ORDER BY Asset DESC", CONNECTION_SYNCH);

    // MountCapability.db2
    PrepareStatement(HOTFIX_SEL_MOUNT_CAPABILITY, "SELECT Unknown0, ReqSpellKnownID, ModSpellAuraID, ReqRidingSkill, ReqAreaID, ReqMapID, Flags, "
        "Unknown7 FROM mount_capability ORDER BY Unknown0 DESC", CONNECTION_SYNCH);

    // MountType.db2
    PrepareStatement(HOTFIX_SEL_MOUNT_TYPE, "SELECT MountTypeID FROM mount_type ORDER BY MountTypeID DESC", CONNECTION_SYNCH);

    // MountTypeXCapability.db2
    PrepareStatement(HOTFIX_SEL_MOUNT_TYPE_X_CAPABILITY, "SELECT MountTypeID, MountCapabilityID, OrderIndex FROM mount_type_x_capability"
        " ORDER BY MountTypeID DESC", CONNECTION_SYNCH);

    // NameGen.db2
    PrepareStatement(HOTFIX_SEL_NAME_GEN, "SELECT Name, RaceID, Gender FROM name_gen ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_NAME_GEN, "SELECT Name, Name_lang FROM name_gen_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestFactionRew.db2
    PrepareStatement(HOTFIX_SEL_QUEST_FACTION_REW, "SELECT Difficulty1, Difficulty2, Difficulty3, Difficulty4, Difficulty5, Difficulty6, Difficulty7, "
        "Difficulty8, Difficulty9, Difficulty10 FROM quest_faction_rew ORDER BY Difficulty[10] DESC", CONNECTION_SYNCH);

    // QuestPOIPoint.db2
    PrepareStatement(HOTFIX_SEL_QUEST_P_O_I_POINT, "SELECT ID, QuestPOIBlobID, X, Y FROM quest_p_o_i_point ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestSort.db2
    PrepareStatement(HOTFIX_SEL_QUEST_SORT, "SELECT SortName FROM quest_sort ORDER BY SortName DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_QUEST_SORT, "SELECT SortName, SortName_lang FROM quest_sort_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestV2.db2
    PrepareStatement(HOTFIX_SEL_QUEST_V2, "SELECT UniqueBitFlag FROM quest_v2 ORDER BY UniqueBitFlag DESC", CONNECTION_SYNCH);

    // QuestXP.db2
    PrepareStatement(HOTFIX_SEL_QUEST_X_P, "SELECT Difficulty1, Difficulty2, Difficulty3, Difficulty4, Difficulty5, Difficulty6, Difficulty7, "
        "Difficulty8, Difficulty9, Difficulty10 FROM quest_x_p ORDER BY Difficulty[10] DESC", CONNECTION_SYNCH);

    // RandPropPoints.db2
    PrepareStatement(HOTFIX_SEL_RAND_PROP_POINTS, "SELECT EpicPropertiesPoints1, EpicPropertiesPoints2, EpicPropertiesPoints3, EpicPropertiesPoints4, "
        "EpicPropertiesPoints5, RarePropertiesPoints1, RarePropertiesPoints2, RarePropertiesPoints3, RarePropertiesPoints4, RarePropertiesPoints5, "
        "UncommonPropertiesPoints1, UncommonPropertiesPoints2, UncommonPropertiesPoints3, UncommonPropertiesPoints4, UncommonPropertiesPoints5"
        " FROM rand_prop_points ORDER BY EpicPropertiesPoints[5] DESC", CONNECTION_SYNCH);

    // ResearchBranch.db2
    PrepareStatement(HOTFIX_SEL_RESEARCH_BRANCH, "SELECT Name, Texture, ItemID, CurrencyID, ResearchFieldID, ID FROM research_branch"
        " ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_RESEARCH_BRANCH, "SELECT Name, Name_lang, Texture_lang FROM research_branch_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ResearchProject.db2
    PrepareStatement(HOTFIX_SEL_RESEARCH_PROJECT, "SELECT Name, Description, rare, branchId, SpellID, Complexity, RequiredCurrencyAmount, ID"
        " FROM research_project ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_RESEARCH_PROJECT, "SELECT Name, Name_lang, Description_lang FROM research_project_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ResearchSite.db2
    PrepareStatement(HOTFIX_SEL_RESEARCH_SITE, "SELECT ID, MapID, POIid, areaName FROM research_site ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_RESEARCH_SITE, "SELECT ID, areaName_lang FROM research_site_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemBonus.db2
    PrepareStatement(HOTFIX_SEL_ITEM_BONUS, "SELECT ID, BonusListID, Type, Value1, Value2, `Index` FROM item_bonus ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemBonusTreeNode.db2
    PrepareStatement(HOTFIX_SEL_ITEM_BONUS_TREE_NODE, "SELECT ID, BonusTreeID, BonusTreeModID, SubTreeID, BonusListID FROM item_bonus_tree_node"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemCurrencyCost.db2
    PrepareStatement(HOTFIX_SEL_ITEM_CURRENCY_COST, "SELECT ID, ItemId FROM item_currency_cost ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemEffect.db2
    PrepareStatement(HOTFIX_SEL_ITEM_EFFECT, "SELECT ID, ItemID, OrderIndex, SpellID, `Trigger`, Charges, Cooldown, Category, CategoryCooldown"
        " FROM item_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemModifiedAppearance.db2
    PrepareStatement(HOTFIX_SEL_ITEM_MODIFIED_APPEARANCE, "SELECT ID, ItemID, AppearanceModID, AppearanceID, IconFileDataID, `Index`"
        " FROM item_modified_appearance ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemSparse.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPARSE, "SELECT ID, Quality, Flags, Unk1, Unk2, BuyCount, BuyPrice, SellPrice, InventoryType, AllowableClass, "
        "AllowableRace, ItemLevel, RequiredLevel, RequiredSkill, RequiredSkillRank, RequiredSpell, RequiredHonorRank, RequiredCityRank, "
        "RequiredReputationFaction, RequiredReputationRank, MaxCount, Stackable, ContainerSlots, ItemStatType, ItemStatValue, ItemStatAllocation, "
        "ItemStatSocketCostMultiplier, ScalingStatDistribution, DamageType, Delay, RangedModRange, Bonding, Name, Name2, Name3, Name4, Description, "
        "PageText, LanguageID, PageMaterial, StartQuest, LockID, Material, Sheath, RandomProperty, RandomSuffix, ItemSet, Area, Map, BagFamily, "
        "TotemCategory, SocketColor, SocketBonus, GemProperties, ArmorDamageModifier, Duration, ItemLimitCategory, HolidayID, StatScalingFactor, "
        "CurrencySubstitutionID, CurrencySubstitutionCount, ItemNameDescriptionID FROM item_sparse ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_SPARSE, "SELECT ID, Name_lang, Name2_lang, Name3_lang, Name4_lang, Description_lang FROM item_sparse_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // ItemExtendedCost.db2
    PrepareStatement(HOTFIX_SEL_ITEM_EXTENDED_COST, "SELECT ID, RequiredArenaSlot, RequiredItem1, RequiredItem2, RequiredItem3, RequiredItem4, "
        "RequiredItem5, RequiredItemCount1, RequiredItemCount2, RequiredItemCount3, RequiredItemCount4, RequiredItemCount5, "
        "RequiredPersonalArenaRating, ItemPurchaseGroup, RequiredCurrency1, RequiredCurrency2, RequiredCurrency3, RequiredCurrency4, "
        "RequiredCurrency5, RequiredCurrencyCount1, RequiredCurrencyCount2, RequiredCurrencyCount3, RequiredCurrencyCount4, RequiredCurrencyCount5, "
        "RequiredFactionId, RequiredFactionStanding, RequirementFlags, RequiredAchievement, RequiredMoney FROM item_extended_cost ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetAbility.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY, "SELECT ID, Type, fileDataEntry, turnCooldown, auraAbilityID, auraDuration, name, description"
        " FROM battle_pet_ability ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_ABILITY, "SELECT ID, name_lang, description_lang FROM battle_pet_ability_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetAbilityEffect.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY_EFFECT, "SELECT ID, TurnEntryID, unk, AuraId_unk, propertiesID, effectIndex, propertyValues1, "
        "propertyValues2, propertyValues3, propertyValues4, propertyValues5, propertyValues6 FROM battle_pet_ability_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetEffectProperties.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_EFFECT_PROPERTIES, "SELECT ID, someFlags, propertyDescs1, propertyDescs2, propertyDescs3, propertyDescs4, "
        "propertyDescs5, propertyDescs6, unk, unk2, unk3, unk4, unk5, unk6 FROM battle_pet_effect_properties ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_EFFECT_PROPERTIES, "SELECT ID, propertyDescs1_lang, propertyDescs2_lang, propertyDescs3_lang, "
        "propertyDescs4_lang, propertyDescs5_lang, propertyDescs6_lang FROM battle_pet_effect_properties_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetAbilityTurn.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY_TURN, "SELECT ID, AbilityID, stateID, turnIndex, unk, procIndex FROM battle_pet_ability_turn"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetAbilityState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY_STATE, "SELECT ID, AbilityID, stateID, unk FROM battle_pet_ability_state ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_STATE, "SELECT ID, unk, stateName, flags FROM battle_pet_state ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_STATE, "SELECT ID, stateName_lang FROM battle_pet_state_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetSpecies.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_SPECIES, "SELECT ID, CreatureEntry, IconFileID, spellId, petType, source, flags, SourceText, Description"
        " FROM battle_pet_species ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_SPECIES, "SELECT ID, SourceText_lang, Description_lang FROM battle_pet_species_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetSpeciesState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_SPECIES_STATE, "SELECT ID, SpeciesID, State, Value FROM battle_pet_species_state ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetSpeciesXAbility.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_SPECIES_X_ABILITY, "SELECT ID, speciesID, abilityID, requiredLevel, rank FROM battle_pet_species_x_ability"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetBreedQuality.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_BREED_QUALITY, "SELECT ID, quality, qualityModifier FROM battle_pet_breed_quality ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetBreedState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_BREED_STATE, "SELECT ID, breedID, stateID, stateModifier FROM battle_pet_breed_state ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrAbility.db2
    PrepareStatement(HOTFIX_SEL_GARR_ABILITY, "SELECT ID, Flags, Name, Description, IconFileDataID, OtherFactionGarrAbilityID, GarrAbilityCategoryID"
        " FROM garr_ability ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_ABILITY, "SELECT ID, Name_lang, Description_lang FROM garr_ability_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrAbilityEffect.db2
    PrepareStatement(HOTFIX_SEL_GARR_ABILITY_EFFECT, "SELECT ID, Flags, GarrAbilityID, Unk1, GarrMechanicTypeID, Unk3, Amount, Amount2, Amount3, "
        "Unk4, Unk5 FROM garr_ability_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrBuilding.db2
    PrepareStatement(HOTFIX_SEL_GARR_BUILDING, "SELECT ID, HordeGameObjectID, AllianceGameObjectID, Unknown, Type, Level, NameAlliance, NameHorde, "
        "Description, Tooltip, BuildDuration, CostCurrencyID, CostCurrencyAmount, HordeTexPrefixKitID, AllianceTexPrefixKitID, IconFileDataID, "
        "BonusAmount, Flags, AllianceActivationScenePackageID, HordeActivationScenePackageID, MaxShipments, FollowerRequiredGarrAbilityID, "
        "FollowerGarrAbilityEffectID, CostMoney FROM garr_building ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_BUILDING, "SELECT ID, NameAlliance_lang, NameHorde_lang, Description_lang, Tooltip_lang"
        " FROM garr_building_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrBuildingPlotInst.db2
    PrepareStatement(HOTFIX_SEL_GARR_BUILDING_PLOT_INST, "SELECT ID, GarrBuildingID, UiTextureAtlasMemberID, GarrSiteLevelPlotInstID, "
        "LandmarkOffsetX, LandmarkOffsetY FROM garr_building_plot_inst ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrClassSpec.db2
    PrepareStatement(HOTFIX_SEL_GARR_CLASS_SPEC, "SELECT ID, NameMale, NameFemale, NameGenderless, ClassAtlasID, GarrFollItemSetID"
        " FROM garr_class_spec ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_CLASS_SPEC, "SELECT ID, NameMale_lang, NameFemale_lang, NameGenderless_lang FROM garr_class_spec_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // GarrFollower.db2
    PrepareStatement(HOTFIX_SEL_GARR_FOLLOWER, "SELECT ID, HordeCreatureID, AllianceCreatureID, HordeUiAnimRaceInfoID, AllianceUiAnimRaceInfoID, "
        "Quality, HordeGarrClassSpecID, AllianceGarrClassSpecID, HordeGarrFollItemSetID, AllianceGarrFollItemSetID, Level, ItemLevelWeapon, "
        "ItemLevelArmor, Unknown1, Flags, HordeSourceText, AllianceSourceText, Unknown2, Unknown3, HordePortraitIconID, AlliancePortraitIconID"
        " FROM garr_follower ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_FOLLOWER, "SELECT ID, HordeSourceText_lang, AllianceSourceText_lang FROM garr_follower_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrFollowerLevelXP.db2
    PrepareStatement(HOTFIX_SEL_GARR_FOLLOWER_LEVEL_X_P, "SELECT ID, Level, NextLevelXP, Unk FROM garr_follower_level_x_p ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrFollowerQuality.db2
    PrepareStatement(HOTFIX_SEL_GARR_FOLLOWER_QUALITY, "SELECT ID, Quality, NextQualityXP, Unk FROM garr_follower_quality ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrFollowerXAbility.db2
    PrepareStatement(HOTFIX_SEL_GARR_FOLLOWER_X_ABILITY, "SELECT ID, GarrFollowerID, GarrAbilityID, FactionIndex FROM garr_follower_x_ability"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrEncounter.db2
    PrepareStatement(HOTFIX_SEL_GARR_ENCOUNTER, "SELECT ID, CreatureEntry, Name, Mod, Mod1, SomeFileDataID FROM garr_encounter ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_ENCOUNTER, "SELECT ID, Name_lang FROM garr_encounter_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrEncounterXMechanic.db2
    PrepareStatement(HOTFIX_SEL_GARR_ENCOUNTER_X_MECHANIC, "SELECT ID, GarrEncounterID, GarrMechanicID FROM garr_encounter_x_mechanic ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrMechanic.db2
    PrepareStatement(HOTFIX_SEL_GARR_MECHANIC, "SELECT ID, Type, Mod FROM garr_mechanic ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrMechanicType.db2
    PrepareStatement(HOTFIX_SEL_GARR_MECHANIC_TYPE, "SELECT ID, Category, Name, Description, SomeFileDataID FROM garr_mechanic_type ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_MECHANIC_TYPE, "SELECT ID, Name_lang, Description_lang FROM garr_mechanic_type_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrMission.db2
    PrepareStatement(HOTFIX_SEL_GARR_MISSION, "SELECT ID, ReqLevel, ReqFollowersItemLevel, GarrMechanicTypeID, ReqFollowersCount, Unk1, "
        "MissionDuration, OfferDuration, Unk2, GarrMissionTypeID, Name, Description, Location, CriteriaID, Unk3, ReqResourcesCount, Unk4, BaseXP, "
        "BaseChance FROM garr_mission ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_MISSION, "SELECT ID, Name_lang, Description_lang, Location_lang FROM garr_mission_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrMissionReward.db2
    PrepareStatement(HOTFIX_SEL_GARR_MISSION_REWARD, "SELECT ID, MissionID, RewardXP, RewardItemID, ItemAmount, CurrencyID, CurrencyValue, Unk2, "
        "Unk3, Unk4 FROM garr_mission_reward ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrMissionXEncounter.db2
    PrepareStatement(HOTFIX_SEL_GARR_MISSION_X_ENCOUNTER, "SELECT ID, GarrMissionID, GarrEncounterID FROM garr_mission_x_encounter ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrPlot.db2
    PrepareStatement(HOTFIX_SEL_GARR_PLOT, "SELECT ID, GarrPlotUICategoryID, PlotType, Flags, Name, MinCount, MaxCount, "
        "AllianceConstructionGameObjectID, HordeConstructionGameObjectID FROM garr_plot ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_PLOT, "SELECT ID, Name_lang FROM garr_plot_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrPlotBuilding.db2
    PrepareStatement(HOTFIX_SEL_GARR_PLOT_BUILDING, "SELECT ID, GarrPlotID, GarrBuildingID FROM garr_plot_building ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrPlotInstance.db2
    PrepareStatement(HOTFIX_SEL_GARR_PLOT_INSTANCE, "SELECT ID, GarrPlotID, Name FROM garr_plot_instance ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_PLOT_INSTANCE, "SELECT ID, Name_lang FROM garr_plot_instance_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrSiteLevel.db2
    PrepareStatement(HOTFIX_SEL_GARR_SITE_LEVEL, "SELECT ID, Level, MapID, SiteID, UITextureKitID, TownHallX, TownHallY, MovieID, Level2, "
        "UpgradeResourceCost, UpgradeMoneyCost FROM garr_site_level ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrSiteLevelPlotInst.db2
    PrepareStatement(HOTFIX_SEL_GARR_SITE_LEVEL_PLOT_INST, "SELECT ID, GarrSiteLevelID, GarrPlotInstanceID, LandmarkX, LandmarkY, Unknown"
        " FROM garr_site_level_plot_inst ORDER BY ID DESC", CONNECTION_SYNCH);

    // CreatureDifficulty.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_DIFFICULTY, "SELECT ID, CreatureID, FactionID, Expansion, MinLevel, MaxLevel, Flags1, Flags2, Flags3, "
        "Flags4, Flags5 FROM creature_difficulty ORDER BY ID DESC", CONNECTION_SYNCH);

    // CurrencyTypes.db2
    PrepareStatement(HOTFIX_SEL_CURRENCY_TYPES, "SELECT ID, CategoryID, Name, InventoryIcon1, InventoryIcon2, SpellWeight, SpellCategory, MaxQty, "
        "MaxEarnablePerWeek, Flags, Quality, Description FROM currency_types ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CURRENCY_TYPES, "SELECT ID, Name_lang, InventoryIcon1_lang, InventoryIcon2_lang, Description_lang"
        " FROM currency_types_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestPackageItem.db2
    PrepareStatement(HOTFIX_SEL_QUEST_PACKAGE_ITEM, "SELECT ID, QuestPackageID, ItemID, ItemCount, FilterType FROM quest_package_item ORDER BY ID DESC", CONNECTION_SYNCH);

    // Mount.db2
    PrepareStatement(HOTFIX_SEL_MOUNT, "SELECT Id, MountTypeId, DisplayId, Flags, Name, Description, SourceDescription, Source, SpellId, "
        "PlayerConditionId FROM mount ORDER BY Id DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_MOUNT, "SELECT Id, Name_lang, Description_lang, SourceDescription_lang FROM mount_locale WHERE locale = ?", CONNECTION_SYNCH);

    // LanguageWords.db2
    PrepareStatement(HOTFIX_SEL_LANGUAGE_WORDS, "SELECT Id, langId, word FROM language_words ORDER BY Id DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_LANGUAGE_WORDS, "SELECT Id, word_lang FROM language_words_locale WHERE locale = ?", CONNECTION_SYNCH);

    // KeyChain.db2
    PrepareStatement(HOTFIX_SEL_KEY_CHAIN, "SELECT Id, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key10, Key11, Key12, Key13, Key14, "
        "Key15, Key16, Key17, Key18, Key19, Key20, Key21, Key22, Key23, Key24, Key25, Key26, Key27, Key28, Key29, Key30, Key31, Key32 FROM key_chain"
        " ORDER BY Id DESC", CONNECTION_SYNCH);

    // OverrideSpellData.db2
    PrepareStatement(HOTFIX_SEL_OVERRIDE_SPELL_DATA, "SELECT ID, SpellID1, SpellID2, SpellID3, SpellID4, SpellID5, SpellID6, SpellID7, SpellID8, "
        "SpellID9, SpellID10, Flags, PlayerActionbarFileDataID FROM override_spell_data ORDER BY ID DESC", CONNECTION_SYNCH);

    // PhaseGroup.db2
    PrepareStatement(HOTFIX_SEL_PHASE_GROUP, "SELECT ID, PhaseID, PhaseGroupID FROM phase_group ORDER BY ID DESC", CONNECTION_SYNCH);

    // SoundEntries.db2
    PrepareStatement(HOTFIX_SEL_SOUND_ENTRIES, "SELECT ID, SoundType, Name, FileDataID1, FileDataID2, FileDataID3, FileDataID4, FileDataID5, "
        "FileDataID6, FileDataID7, FileDataID8, FileDataID9, FileDataID10, FileDataID11, FileDataID12, FileDataID13, FileDataID14, FileDataID15, "
        "FileDataID16, FileDataID17, FileDataID18, FileDataID19, FileDataID20, Freq1, Freq2, Freq3, Freq4, Freq5, Freq6, Freq7, Freq8, Freq9, Freq10, "
        "Freq11, Freq12, Freq13, Freq14, Freq15, Freq16, Freq17, Freq18, Freq19, Freq20, VolumeFloat, Flags, MinDistance, DistanceCutoff, EAXDef, "
        "SoundEntriesAdvancedID, VolumeVariationPlus, VolumeVariationMinus, PitchVariationPlus, PitchVariationMinus, PitchAdjust, DialogType, "
        "BusOverwriteID FROM sound_entries ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SOUND_ENTRIES, "SELECT ID, Name_lang FROM sound_entries_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellAuraRestrictions.db2
    PrepareStatement(HOTFIX_SEL_SPELL_AURA_RESTRICTIONS, "SELECT ID, SpellID, CasterAuraSpell, TargetAuraSpell, ExcludeCasterAuraSpell, "
        "ExcludeTargetAuraSpell, DifficultyID, CasterAuraState, TargetAuraState, ExcludeCasterAuraState, ExcludeTargetAuraState"
        " FROM spell_aura_restrictions ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellCastingRequirements.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CASTING_REQUIREMENTS, "SELECT ID, SpellID, MinFactionID, RequiredAreasID, RequiresSpellFocus, "
        "FacingCasterFlags, MinReputation, RequiredAuraVision FROM spell_casting_requirements ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellClassOptions.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CLASS_OPTIONS, "SELECT ID, SpellID, SpellClassMask1, SpellClassMask2, SpellClassMask3, SpellClassMask4, "
        "ModalNextSpell, SpellClassSet FROM spell_class_options ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellLearnSpell.db2
    PrepareStatement(HOTFIX_SEL_SPELL_LEARN_SPELL, "SELECT ID, LearnSpellID, SpellID, OverridesSpellID FROM spell_learn_spell ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellMisc.db2
    PrepareStatement(HOTFIX_SEL_SPELL_MISC, "SELECT ID, Attributes1, Attributes2, Attributes3, Attributes4, Attributes5, Attributes6, Attributes7, "
        "Attributes8, Attributes9, Attributes10, Attributes11, Attributes12, Attributes13, Attributes14, Speed, MultistrikeSpeedMod, "
        "CastingTimeIndex, DurationIndex, RangeIndex, SpellIconID, ActiveIconID, SchoolMask FROM spell_misc ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellPower.db2
    PrepareStatement(HOTFIX_SEL_SPELL_POWER, "SELECT ID, SpellID, PowerIndex, PowerType, PowerCost, PowerCostPerLevel, PowerCostPerSecond, "
        "ManaCostAdditional, PowerDisplayId, UnitPowerBarID, PowerCostPercentage, PowerCostPercentagePerSecond, RequiredAura, HealthCostPercentage"
        " FROM spell_power ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellReagents.db2
    PrepareStatement(HOTFIX_SEL_SPELL_REAGENTS, "SELECT Id, Reagent1, Reagent2, Reagent3, Reagent4, Reagent5, Reagent6, Reagent7, Reagent8, "
        "ReagentCount1, ReagentCount2, ReagentCount3, ReagentCount4, ReagentCount5, ReagentCount6, ReagentCount7, ReagentCount8, ReagentCurrency, "
        "ReagentCurrencyCount FROM spell_reagents ORDER BY Id DESC", CONNECTION_SYNCH);

    // SpellRuneCost.db2
    PrepareStatement(HOTFIX_SEL_SPELL_RUNE_COST, "SELECT ID, RuneCost1, RuneCost2, RuneCost3, RuneCost4, RunePowerGain FROM spell_rune_cost"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // Toy.db2
    PrepareStatement(HOTFIX_SEL_TOY, "SELECT ID, ItemID, Flags, Description, CategoryFilter FROM toy ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TOY, "SELECT ID, Description_lang FROM toy_locale WHERE locale = ?", CONNECTION_SYNCH);

    // PvpItem.db2
    PrepareStatement(HOTFIX_SEL_PVP_ITEM, "SELECT ID, ItemID, BonusIlvl FROM pvp_item ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellTotems.db2
    PrepareStatement(HOTFIX_SEL_SPELL_TOTEMS, "SELECT ID, TotemCategory1, TotemCategory2, Totem1, Totem2 FROM spell_totems ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellXSpellVisual.db2
    PrepareStatement(HOTFIX_SEL_SPELL_X_SPELL_VISUAL, "SELECT ID, SpellID, TravelSpeed, SpellVisualID1, SpellVisualID2, PlayerConditionID, "
        "DifficultyID, Flags FROM spell_x_spell_visual ORDER BY ID DESC", CONNECTION_SYNCH);

    // TaxiNodes.db2
    PrepareStatement(HOTFIX_SEL_TAXI_NODES, "SELECT ID, MapID, PosX, PosY, PosZ, Name, MountCreatureID, ConditionID, LearnableIndex, Flags, "
        "MapOffset FROM taxi_nodes ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TAXI_NODES, "SELECT ID, Name_lang FROM taxi_nodes_locale WHERE locale = ?", CONNECTION_SYNCH);

    // TaxiPath.db2
    PrepareStatement(HOTFIX_SEL_TAXI_PATH, "SELECT ID, `From`, `To`, Cost FROM taxi_path ORDER BY ID DESC", CONNECTION_SYNCH);

    // TaxiPathNode.db2
    PrepareStatement(HOTFIX_SEL_TAXI_PATH_NODE, "SELECT ID, PathID, NodeIndex, MapID, LocX, LocY, LocZ, Flags, Delay, ArrivalEventID, "
        "DepartureEventID FROM taxi_path_node ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemUpgrade.db2
    PrepareStatement(HOTFIX_SEL_ITEM_UPGRADE, "SELECT id, itemUpgradePathId, levelBonus, prevUpgradeId, currencyReqId, currencyReqAmt"
        " FROM item_upgrade ORDER BY id DESC", CONNECTION_SYNCH);

    // RuleSetItemUpgrade.db2
    PrepareStatement(HOTFIX_SEL_RULE_SET_ITEM_UPGRADE, "SELECT id, levelUpd, startUpgrade, itemEntry FROM rule_set_item_upgrade ORDER BY id DESC", CONNECTION_SYNCH);

    // GameObjects.db2
    PrepareStatement(HOTFIX_SEL_GAME_OBJECTS, "SELECT ID, MapID, DisplayID, PositionX, PositionY, PositionZ, RotationX, RotationY, RotationZ, "
        "RotationW, Size, PhaseUseFlags, PhaseID, PhaseGroupID, Type, Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Name FROM game_objects"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GAME_OBJECTS, "SELECT ID, Name_lang FROM game_objects_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GameTables.db2
    PrepareStatement(HOTFIX_SEL_GAME_TABLES, "SELECT ID, Name, NumRows, NumColumns FROM game_tables ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GAME_TABLES, "SELECT ID, Name_lang FROM game_tables_locale WHERE locale = ?", CONNECTION_SYNCH);

    // MapChallengeMode.db2
    PrepareStatement(HOTFIX_SEL_MAP_CHALLENGE_MODE, "SELECT id, map, unk1, unk2, season, bronze, silver, gold, unk3, unk4 FROM map_challenge_mode"
        " ORDER BY id DESC", CONNECTION_SYNCH);

    // SpellVisual.db2
    PrepareStatement(HOTFIX_SEL_SPELL_VISUAL, "SELECT ID, unk, unk2, unk3, unk4, unk5, unk6, unk7, unk8, unk9, unk10, unk11, unk12, unk13, unk14, "
        "unk15, unk16, unk17, unk18, unk19, unk20, unk21, unk22, unk23, unk24, hostileId, unk25, unk26, unk27, unk28 FROM spell_visual"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemXBonusTree.db2
    PrepareStatement(HOTFIX_SEL_ITEM_X_BONUS_TREE, "SELECT ID, ItemID, BonusTreeID FROM item_x_bonus_tree ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellEffect.db2
    PrepareStatement(HOTFIX_SEL_SPELL_EFFECT, "SELECT ID, DifficultyID, Effect, EffectAmplitude, EffectAura, EffectAuraPeriod, EffectBasePoints, "
        "EffectBonusCoefficient, EffectChainAmplitude, EffectChainTargets, EffectDieSides, EffectItemType, EffectMechanic, EffectMiscValue, "
        "EffectMiscValueB, EffectPointsPerResource, EffectRadiusIndex, EffectRadiusMaxIndex, EffectRealPointsPerLevel, EffectSpellClassMask1, "
        "EffectSpellClassMask2, EffectSpellClassMask3, EffectSpellClassMask4, EffectTriggerSpell, EffectPosFacing, ImplicitTarget1, ImplicitTarget2, "
        "SpellID, EffectIndex, EffectAttributes, BonusCoefficientFromAP FROM spell_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellShapeshiftForm.db2
    PrepareStatement(HOTFIX_SEL_SPELL_SHAPESHIFT_FORM, "SELECT Name, UnkLegion, Flags, AttackIconID, CombatRoundTime, CreatureDisplayID1, "
        "CreatureDisplayID2, CreatureDisplayID3, CreatureDisplayID4, PresetSpellID, CreatureType, MountTypeID, ExitSoundEntriesID, ID"
        " FROM spell_shapeshift_form ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_SHAPESHIFT_FORM, "SELECT Name, Name_lang FROM spell_shapeshift_form_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellShapeshift.db2
    PrepareStatement(HOTFIX_SEL_SPELL_SHAPESHIFT, "SELECT ID, ShapeshiftExclude1, ShapeshiftExclude2, ShapeshiftMask1, ShapeshiftMask2, "
        "StanceBarOrder FROM spell_shapeshift ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellTargetRestrictions.db2
    PrepareStatement(HOTFIX_SEL_SPELL_TARGET_RESTRICTIONS, "SELECT SpellID, ConeAngle, Width, Targets, MaxTargetLevel, TargetCreatureType, "
        "DifficultyID, MaxAffectedTargets FROM spell_target_restrictions ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // Spell.db2
    PrepareStatement(HOTFIX_SEL_SPELL, "SELECT ID, Name, NameSubtext, Description, AuraDescription, SpellMiscDifficultyID, DescriptionVariablesID"
        " FROM spell ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL, "SELECT ID, Name_lang, NameSubtext_lang, Description_lang, AuraDescription_lang FROM spell_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // SpellScaling.db2
    PrepareStatement(HOTFIX_SEL_SPELL_SCALING, "SELECT SpellID, ScalesFromItemLevel, ScalingClass, MaxScalingLevel FROM spell_scaling"
        " ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpellRange.db2
    PrepareStatement(HOTFIX_SEL_SPELL_RANGE, "SELECT MinRangeHostile, MinRangeFriend, MaxRangeHostile, MaxRangeFriend, DisplayName, DisplayNameShort, "
        "Flags, ID FROM spell_range ORDER BY MinRangeHostile DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_RANGE, "SELECT MinRangeHostile, DisplayName_lang, DisplayNameShort_lang FROM spell_range_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // SpellRadius.db2
    PrepareStatement(HOTFIX_SEL_SPELL_RADIUS, "SELECT Radius, RadiusPerLevel, RadiusMin, RadiusMax FROM spell_radius ORDER BY Radius DESC", CONNECTION_SYNCH);

    // SpellProcsPerMinute.db2
    PrepareStatement(HOTFIX_SEL_SPELL_PROCS_PER_MINUTE, "SELECT BaseProcRate, Flags FROM spell_procs_per_minute ORDER BY BaseProcRate DESC", CONNECTION_SYNCH);

    // SpellProcsPerMinuteMod.db2
    PrepareStatement(HOTFIX_SEL_SPELL_PROCS_PER_MINUTE_MOD, "SELECT PpmRateMod, Type, SpecID, SpellProcsPerMinuteID, Id"
        " FROM spell_procs_per_minute_mod ORDER BY PpmRateMod DESC", CONNECTION_SYNCH);

    // SpellLevels.db2
    PrepareStatement(HOTFIX_SEL_SPELL_LEVELS, "SELECT SpellID, BaseLevel, MaxLevel, SpellLevel, DifficultyID FROM spell_levels ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpellItemEnchantmentCondition.db2
    PrepareStatement(HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT_CONDITION, "SELECT Color1, Color2, Color3, Color4, Color5, LtOperandType1, LtOperandType2, "
        "LtOperandType3, LtOperandType4, LtOperandType5, Comparator1, Comparator2, Comparator3, Comparator4, Comparator5, CompareColor1, "
        "CompareColor2, CompareColor3, CompareColor4, CompareColor5, Value1, Value2, Value3, Value4, Value5, Logic1, Logic2, Logic3, Logic4, Logic5"
        " FROM spell_item_enchantment_condition ORDER BY Color[MAX_ITEM_ENCHANTS] DESC", CONNECTION_SYNCH);

    // SpellInterrupts.db2
    PrepareStatement(HOTFIX_SEL_SPELL_INTERRUPTS, "SELECT SpellID, AuraInterruptFlags1, AuraInterruptFlags2, ChannelInterruptFlags1, "
        "ChannelInterruptFlags2, InterruptFlags, DifficultyID FROM spell_interrupts ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpellFocusObject.db2
    PrepareStatement(HOTFIX_SEL_SPELL_FOCUS_OBJECT, "SELECT Name FROM spell_focus_object ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_FOCUS_OBJECT, "SELECT Name, Name_lang FROM spell_focus_object_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellEquippedItems.db2
    PrepareStatement(HOTFIX_SEL_SPELL_EQUIPPED_ITEMS, "SELECT SpellID, EquippedItemInventoryTypeMask, EquippedItemSubClassMask, EquippedItemClass"
        " FROM spell_equipped_items ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpellEffectScaling.db2
    PrepareStatement(HOTFIX_SEL_SPELL_EFFECT_SCALING, "SELECT Coefficient, Variance, ResourceCoefficient, SpellEffectId FROM spell_effect_scaling"
        " ORDER BY Coefficient DESC", CONNECTION_SYNCH);

    // SpellDuration.db2
    PrepareStatement(HOTFIX_SEL_SPELL_DURATION, "SELECT ID, Duration, MaxDuration, DurationPerLevel FROM spell_duration ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellCooldowns.db2
    PrepareStatement(HOTFIX_SEL_SPELL_COOLDOWNS, "SELECT SpellID, CategoryRecoveryTime, RecoveryTime, StartRecoveryTime, DifficultyID"
        " FROM spell_cooldowns ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpellCategory.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CATEGORY, "SELECT ID, Name, ChargeRecoveryTime, Flags, UsesPerWeek, MaxCharges FROM spell_category"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_CATEGORY, "SELECT ID, Name_lang FROM spell_category_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellCategories.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CATEGORIES, "SELECT SpellID, Category, StartRecoveryCategory, ChargeCategory, DifficultyID, DefenseType, "
        "DispelType, Mechanic, PreventionType FROM spell_categories ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpellCastTimes.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CAST_TIMES, "SELECT Base, Minimum, PerLevel FROM spell_cast_times ORDER BY Base DESC", CONNECTION_SYNCH);

    // SpellAuraOptions.db2
    PrepareStatement(HOTFIX_SEL_SPELL_AURA_OPTIONS, "SELECT SpellID, ProcCharges, ProcTypeMask, ProcCategoryRecovery, CumulativeAura, DifficultyID, "
        "ProcChance, SpellProcsPerMinuteID FROM spell_aura_options ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // SpecializationSpell.db2
    PrepareStatement(HOTFIX_SEL_SPECIALIZATION_SPELL, "SELECT ID, SpellID, OverridesSpellID, DescriptionLang, SpecID, UnkLegion"
        " FROM specialization_spell ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPECIALIZATION_SPELL, "SELECT ID, DescriptionLang_lang FROM specialization_spell_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SkillRaceClassInfo.db2
    PrepareStatement(HOTFIX_SEL_SKILL_RACE_CLASS_INFO, "SELECT RaceMask, SkillID, ClassMask, Flags, SkillTierID, Availability, MinLevel"
        " FROM skill_race_class_info ORDER BY RaceMask DESC", CONNECTION_SYNCH);

    // SkillLine.db2
    PrepareStatement(HOTFIX_SEL_SKILL_LINE, "SELECT DisplayName, Description, AlternateVerb, SpellIconID, Flags, CategoryID, CanLink, "
        "ParentSkillLineID, id FROM skill_line ORDER BY DisplayName DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SKILL_LINE, "SELECT DisplayName, DisplayName_lang, Description_lang, AlternateVerb_lang FROM skill_line_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // SkillLineAbility.db2
    PrepareStatement(HOTFIX_SEL_SKILL_LINE_ABILITY, "SELECT SpellID, RaceMask, ClassMask, SupercedesSpell, SkillLine, MinSkillLineRank, "
        "TrivialSkillLineRankHigh, TrivialSkillLineRankLow, UniqueBit, TradeSkillCategoryID, AquireMethod, NumSkillUps FROM skill_line_ability"
        " ORDER BY SpellID DESC", CONNECTION_SYNCH);

    // Scenario.db2
    PrepareStatement(HOTFIX_SEL_SCENARIO, "SELECT Name, Flags, UnkLegion FROM scenario ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SCENARIO, "SELECT Name, Name_lang FROM scenario_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ScenarioStep.db2
    PrepareStatement(HOTFIX_SEL_SCENARIO_STEP, "SELECT DescriptionLang, TitleLang, Criteriatreeid, ScenarioID, Supersedes, RewardQuestID, OrderIndex, "
        "Flags, RelatedStep FROM scenario_step ORDER BY DescriptionLang DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SCENARIO_STEP, "SELECT DescriptionLang, DescriptionLang_lang, TitleLang_lang FROM scenario_step_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // ScalingStatDistribution.db2
    PrepareStatement(HOTFIX_SEL_SCALING_STAT_DISTRIBUTION, "SELECT ItemLevelCurveID, MinLevel, MaxLevel FROM scaling_stat_distribution"
        " ORDER BY ItemLevelCurveID DESC", CONNECTION_SYNCH);

    // TotemCategory.db2
    PrepareStatement(HOTFIX_SEL_TOTEM_CATEGORY, "SELECT Name, TotemCategoryType, TotemCategoryMask FROM totem_category ORDER BY Name DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TOTEM_CATEGORY, "SELECT Name, Name_lang FROM totem_category_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemToBattlePetSpecies.db2
    PrepareStatement(HOTFIX_SEL_ITEM_TO_BATTLE_PET_SPECIES, "SELECT ID, BattlePetSpeciesID FROM item_to_battle_pet_species ORDER BY ID DESC", CONNECTION_SYNCH);

    // TransportAnimation.db2
    PrepareStatement(HOTFIX_SEL_TRANSPORT_ANIMATION, "SELECT TransportID, TimeIndex, PosX, PosY, PosZ, SequenceID FROM transport_animation"
        " ORDER BY TransportID DESC", CONNECTION_SYNCH);

    // TransportRotation.db2
    PrepareStatement(HOTFIX_SEL_TRANSPORT_ROTATION, "SELECT TransportID, TimeIndex, RotX, RotY, RotZ, RotO FROM transport_rotation"
        " ORDER BY TransportID DESC", CONNECTION_SYNCH);

    // UnitPowerBar.db2
    PrepareStatement(HOTFIX_SEL_UNIT_POWER_BAR, "SELECT MaxPower, RegenerationPeace, RegenerationCombat, FileDataID1, FileDataID2, FileDataID3, "
        "FileDataID4, FileDataID5, FileDataID6, Color1, Color2, Color3, Color4, Color5, Color6, Name, Cost, OutOfError, ToolTip, StartInset, "
        "EndInset, StartPower, Flags, MinPower, CenterPower, BarType FROM unit_power_bar ORDER BY MaxPower DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_UNIT_POWER_BAR, "SELECT MaxPower, Name_lang, Cost_lang, OutOfError_lang, ToolTip_lang FROM unit_power_bar_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // WorldMapOverlay.db2
    PrepareStatement(HOTFIX_SEL_WORLD_MAP_OVERLAY, "SELECT TextureName, MapAreaID, AreaID1, AreaID2, AreaID3, AreaID4, TextureWidth, TextureHeight, "
        "OffsetX, OffsetY, HitRectTop, HitRectLeft, HitRectBottom, HitRectRight, PlayerConditionID FROM world_map_overlay ORDER BY TextureName DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_WORLD_MAP_OVERLAY, "SELECT TextureName, TextureName_lang FROM world_map_overlay_locale WHERE locale = ?", CONNECTION_SYNCH);

    // WorldMapTransforms.db2
    PrepareStatement(HOTFIX_SEL_WORLD_MAP_TRANSFORMS, "SELECT RegionMinX, RegionMinY, RegionMinZ, RegionMaxX, RegionMaxY, RegionMaxZ, RegionOffsetX, "
        "RegionOffsetY, RegionScale, MapID, NewMapID, NewDungeonMapID, NewAreaID, Flags FROM world_map_transforms ORDER BY RegionMin DESC", CONNECTION_SYNCH);
}
