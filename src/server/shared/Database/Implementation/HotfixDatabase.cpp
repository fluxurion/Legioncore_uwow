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

#include "HotfixDatabase.h"

// Force locale statments to appear exactly in locale declaration order, right after normal data fetch statement
#define PREPARE_LOCALE_STMT(stmtBase, sql, con) \
    static_assert(stmtBase + 1 == stmtBase##_LOCALE, "Invalid prepared statement index for " #stmtBase "_LOCALE"); \
    PrepareStatement(stmtBase##_LOCALE, sql, con);

void HotfixDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_HOTFIXDATABASE_STATEMENTS);

    // AreaTable.db2
    PrepareStatement(HOTFIX_SEL_AREA_TABLE, "SELECT ID, Flags1, Flags2, ZoneName, AmbientMultiplier, AreaName, MapID, ParentAreaID, AreaBit, "
        "AmbienceID, ZoneMusic, IntroSound, LiquidTypeID1, LiquidTypeID2, LiquidTypeID3, LiquidTypeID4, UWIntroMusic, UWZoneMusic, UWAmbience, "
        "WorldPvPID, WindSettingsID, SoundProviderPref, SoundProviderPrefUnderwater, ExplorationLevel, FactionGroupMask, MountFlags, "
        "WildBattlePetLevelMin, WildBattlePetLevelMax FROM area_table ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_AREA_TABLE, "SELECT ID, ZoneName_lang, AreaName_lang FROM area_table_locale WHERE locale = ?", CONNECTION_SYNCH);

    // AreaTrigger.db2
    PrepareStatement(HOTFIX_SEL_AREA_TRIGGER, "SELECT ID, PosX, PosY, PosZ, Radius, BoxLength, BoxWidth, BoxHeight, BoxYaw, MapID, PhaseID, "
        "PhaseGroupID, ShapeID, AreaTriggerActionSetID, PhaseUseFlags, ShapeType, Flags FROM area_trigger ORDER BY ID DESC", CONNECTION_SYNCH);

    // Achievement.db2
    PrepareStatement(HOTFIX_SEL_ACHIEVEMENT, "SELECT ID, Name, Description, Flags, Reward, MapID, ParentAchievement, CategoryID, OrderInCategory, "
        "IconID, RefAchievement, CriteriaTree, FactionFlag, RewardPoints, Amount FROM achievement ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ACHIEVEMENT, "SELECT ID, Name_lang, Description_lang, Reward_lang FROM achievement_locale WHERE locale = ?", CONNECTION_SYNCH);

    // AreaGroupMember.db2
    PrepareStatement(HOTFIX_SEL_AREA_GROUP_MEMBER, "SELECT ID, AreaGroupID, AreaID FROM area_group_member ORDER BY ID DESC", CONNECTION_SYNCH);

    // AuctionHouse.db2
    PrepareStatement(HOTFIX_SEL_AUCTION_HOUSE, "SELECT ID, Name, FactionID, DepositRate, ConsignmentRate FROM auction_house ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_AUCTION_HOUSE, "SELECT ID, Name_lang FROM auction_house_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ArmorLocation.db2
    PrepareStatement(HOTFIX_SEL_ARMOR_LOCATION, "SELECT ID, Value1, Value2, Value3, Value4, Value5 FROM armor_location ORDER BY ID DESC", CONNECTION_SYNCH);

    // Artifact.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT, "SELECT ID, Name, Unk1, Unk2, Unk3, SpecID, UnkByte1, UnkByte2, UnkByte3 FROM artifact ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ARTIFACT, "SELECT ID, Name_lang FROM artifact_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ArtifactAppearance.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_APPEARANCE, "SELECT ID, Name, Unk1, UnkFloat1, UnkFloat2, Unk2, UnkShot1, UnkByte1, UnkByte2, UnkByte3, UnkByte4, UnkByte5, UnkByte6 FROM artifact_appearance ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ARTIFACT_APPEARANCE, "SELECT ID, Name_lang FROM artifact_appearance_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ArtifactAppearanceSet.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_APPEARANCE_SET, "SELECT ID, Name1, Name2, UnkShot1, UnkShot2, UnkByte1, UnkByte2, UnkByte3 FROM artifact_appearance_set ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ARTIFACT_APPEARANCE_SET, "SELECT ID, Name1_lang, Name2_lang FROM artifact_appearance_set_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ArtifactCategory.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_CATEGORY, "SELECT ID, UnkShot1, UnkShot2 FROM artifact_category ORDER BY ID DESC", CONNECTION_SYNCH);

    // ArtifactPower.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_POWER, "SELECT ID, UnkFloat1, UnkFloat2, UnkShot1, UnkByte1, UnkByte2, UnkByte3 FROM artifact_power ORDER BY ID DESC", CONNECTION_SYNCH);

    // ArtifactPowerLink.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_POWER_LINK, "SELECT ID, UnkShot1, UnkShot2 FROM artifact_power_link ORDER BY ID DESC", CONNECTION_SYNCH);

    // ArtifactPowerRank.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_POWER_RANK, "SELECT ID, Unk1, UnkFloat1, UnkShot1, UnkShot2, UnkByte1 FROM artifact_power_rank ORDER BY ID DESC", CONNECTION_SYNCH);

    // ArtifactQuestXP.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_QUEST_XP, "SELECT ID, UnkShot1, UnkShot2, UnkShot3, UnkShot4, UnkShot5, UnkShot6, UnkShot7, UnkShot8, UnkShot9, UnkShot10 FROM artifact_quest_xp ORDER BY ID DESC", CONNECTION_SYNCH);

    // ArtifactUnlock.db2
    PrepareStatement(HOTFIX_SEL_ARTIFACT_UNLOCK, "SELECT ID, UnkShot1, UnkShot2, UnkByte1, UnkByte2 FROM artifact_unlock ORDER BY ID DESC", CONNECTION_SYNCH);

    // BannedAddons.db2
    PrepareStatement(HOTFIX_SEL_BANNED_ADDONS, "SELECT ID, NameMD51, NameMD52, NameMD53, NameMD54, VersionMD51, VersionMD52, VersionMD53, "
        "VersionMD54, LastModified, Flags FROM banned_addons ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlemasterList.db2
    PrepareStatement(HOTFIX_SEL_BATTLEMASTER_LIST, "SELECT ID, NameLang, IconFileDataID, GametypeLang, MapID1, MapID2, MapID3, MapID4, MapID5, "
        "MapID6, MapID7, MapID8, MapID9, MapID10, MapID11, MapID12, MapID13, MapID14, MapID15, MapID16, HolidayWorldState, CritreriaID, InstanceType, "
        "GroupsAllowed, MaxGroupSize, Minlevel, Maxlevel, RatedPlayers, MinPlayers, MaxPlayers, Flags FROM battlemaster_list ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLEMASTER_LIST, "SELECT ID, NameLang_lang, GametypeLang_lang FROM battlemaster_list_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ChrClasses.db2
    PrepareStatement(HOTFIX_SEL_CHR_CLASSES, "SELECT ID, PetNameToken, Name, NameFemale, NameMale, Filename, CreateScreenFileDataID, "
        "SelectScreenFileDataID, IconFileDataID, Flags, CinematicSequenceID, DefaultSpec, PowerType, SpellClassSet, AttackPowerPerStrength, "
        "AttackPowerPerAgility, RangedAttackPowerPerAgility, LowResScreenFileDataID, LegionUnk FROM chr_classes ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHR_CLASSES, "SELECT ID, PetNameToken_lang, Name_lang, NameFemale_lang, NameMale_lang, Filename_lang"
        " FROM chr_classes_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CharSections.db2
    PrepareStatement(HOTFIX_SEL_CHAR_SECTIONS, "SELECT ID, FileDataID1, FileDataID2, FileDataID3, Flags, Race, Gender, GenType, Type, Color"
        " FROM char_sections ORDER BY ID DESC", CONNECTION_SYNCH);

    // CreatureModelData.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_MODEL_DATA, "SELECT ID, Flags, FileDataID, ModelScale, FootprintTextureID, FootprintTextureLength, "
        "FootprintTextureWidth, CollisionWidth, CollisionHeight, MountHeight, GeoBox1, GeoBox2, GeoBox3, GeoBox4, GeoBox5, GeoBox6, WorldEffectScale, "
        "AttachedEffectScale, MissileCollisionRadius, MissileCollisionPush, MissileCollisionRaise, OverrideLootEffectScale, OverrideNameScale, "
        "OverrideSelectionRadius, TamedPetBaseScale, HoverHeight, SoundID, CreatureGeosetDataID, SizeClass, UnkLegion, FootstepShakeSize, UnkLegion2, "
        "UnkLegion3, UnkLegion4, UnkLegion5, DeathThudShakeSize FROM creature_model_data ORDER BY ID DESC", CONNECTION_SYNCH);

    // ConversationLine.db2
    PrepareStatement(HOTFIX_SEL_CONVERSATION_LINE, "SELECT ID, tesxID, duration, unk, nextID, unk1, unk2, unk3, unk4 FROM conversation_line"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // Faction.db2
    PrepareStatement(HOTFIX_SEL_FACTION, "SELECT ID, BaseRepRaceMask1, BaseRepRaceMask2, BaseRepRaceMask3, BaseRepRaceMask4, BaseRepValue1, "
        "BaseRepValue2, BaseRepValue3, BaseRepValue4, SpilloverRate1, SpilloverRate2, Name, `Desc`, ReputationListID, BaseRepClassMask1, "
        "BaseRepClassMask2, BaseRepClassMask3, BaseRepClassMask4, ReputationFlags1, ReputationFlags2, ReputationFlags3, ReputationFlags4, Team, "
        "SpilloverRank1, SpilloverRank2, Expansion, Flags, FriendshipRepID FROM faction ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_FACTION, "SELECT ID, Name_lang, Desc_lang FROM faction_locale WHERE locale = ?", CONNECTION_SYNCH);

    // FactionTemplate.db2
    PrepareStatement(HOTFIX_SEL_FACTION_TEMPLATE, "SELECT ID, Faction, Flags, Enemies1, Enemies2, Enemies3, Enemies4, Friends1, Friends2, Friends3, "
        "Friends4, Mask, FriendMask, EnemyMask FROM faction_template ORDER BY ID DESC", CONNECTION_SYNCH);

    // GlyphSlot.db2
    PrepareStatement(HOTFIX_SEL_GLYPH_SLOT, "SELECT ID, TypeFlags, `Order` FROM glyph_slot ORDER BY ID DESC", CONNECTION_SYNCH);

    // LFGDungeon.db2
    PrepareStatement(HOTFIX_SEL_L_F_G_DUNGEON, "SELECT ID, Name, Flags, TextureFilename, Description, MaxLevel, TargetLevelMax, MapID, RandomID, "
        "ScenarioID, LastBossJournalEncounterID, BonusReputationAmount, TimeWalkerItemLvl, MinLevel, TargetLevel, TargetLevelMin, DifficultyID, Type, "
        "Faction, Expansion, OrderIndex, GroupID, CountTank, CountHealer, CountDamage, MinCountTank, MinCountHealer, MinCountDamage, SubType, "
        "UnkLegion FROM l_f_g_dungeon ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_L_F_G_DUNGEON, "SELECT ID, Name_lang, TextureFilename_lang, Description_lang FROM l_f_g_dungeon_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // LiquidType.db2
    PrepareStatement(HOTFIX_SEL_LIQUID_TYPE, "SELECT ID, Name, SpellId, unk_1, unk_2, unk_3, unk_4, unk_5, Texture1, Texture2, Texture3, Texture4, "
        "Texture5, Texture6, unk2_, unk3_, unk4_1, unk4_2, unk4_3, unk4_4, unk4_5, unk4_6, unk4_7, unk4_8, unk4_9, unk4_10, unk4_11, unk4_12, "
        "unk4_13, unk4_14, unk4_15, unk4_16, unk4_17, unk4_18, unk5_, unk6_, unk7_, unk8_, unk9_, unk10_, Type, unk11_1, unk11_2, unk11_3, unk11_4, "
        "unk11_5, unk11_6, unk11_7, unk11_8, unk11_9, unk11_10 FROM liquid_type ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_LIQUID_TYPE, "SELECT ID, Name_lang, Texture1_lang, Texture2_lang, Texture3_lang, Texture4_lang, Texture5_lang, "
        "Texture6_lang FROM liquid_type_locale WHERE locale = ?", CONNECTION_SYNCH);

    // MapDifficulty.db2
    PrepareStatement(HOTFIX_SEL_MAP_DIFFICULTY, "SELECT ID, Message, RaidDuration, MapID, DifficultyID, MaxPlayers, LockID, ItemBonusTreeModID"
        " FROM map_difficulty ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_MAP_DIFFICULTY, "SELECT ID, Message_lang FROM map_difficulty_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellItemEnchantment.db2
    PrepareStatement(HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT, "SELECT ID, EffectSpellID1, EffectSpellID2, EffectSpellID3, Name, EffectScalingPoints1, "
        "EffectScalingPoints2, EffectScalingPoints3, UnkLegion, EffectPointsMin1, EffectPointsMin2, EffectPointsMin3, ItemVisual, Flags, "
        "RequiredSkillID, RequiredSkillRank, ItemLevel, Charges, Effect1, Effect2, Effect3, UnkLegion2, MinLevel, MaxLevel, ScalingClass, "
        "ScalingClassRestricted, UnkLegion3, UnkLegion4 FROM spell_item_enchantment ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT, "SELECT ID, Name_lang FROM spell_item_enchantment_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Talent.db2
    PrepareStatement(HOTFIX_SEL_TALENT, "SELECT ID, SpellID, OverridesSpellID, Description, SpecID, `Row`, `Column`, Flags, CategoryMask1, "
        "CategoryMask2, ClassID FROM talent ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TALENT, "SELECT ID, Description_lang FROM talent_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Vehicle.db2
    PrepareStatement(HOTFIX_SEL_VEHICLE, "SELECT ID, Flags, TurnSpeed, PitchSpeed, PitchMin, PitchMax, MouseLookOffsetPitch, CameraFadeDistScalarMin, "
        "CameraFadeDistScalarMax, CameraPitchOffset, FacingLimitRight, FacingLimitLeft, MsslTrgtTurnLingering, MsslTrgtPitchLingering, "
        "MsslTrgtMouseLingering, MsslTrgtEndOpacity, MsslTrgtArcSpeed, MsslTrgtArcRepeat, MsslTrgtArcWidth, MsslTrgtImpactRadius1, "
        "MsslTrgtImpactRadius2, MsslTrgtArcTexture, MsslTrgtImpactTexture, MsslTrgtImpactModel1, MsslTrgtImpactModel2, CameraYawOffset, "
        "MsslTrgtImpactTexRadius, SeatID1, SeatID2, SeatID3, SeatID4, SeatID5, SeatID6, SeatID7, SeatID8, PowerDisplayID1, PowerDisplayID2, "
        "PowerDisplayID3, FlagsB, UILocomotionType, VehicleUIIndicatorID FROM vehicle ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_VEHICLE, "SELECT ID, MsslTrgtArcTexture_lang, MsslTrgtImpactTexture_lang, MsslTrgtImpactModel1_lang, "
        "MsslTrgtImpactModel2_lang FROM vehicle_locale WHERE locale = ?", CONNECTION_SYNCH);

    // WMOAreaTable.db2
    PrepareStatement(HOTFIX_SEL_W_M_O_AREA_TABLE, "SELECT ID, WMOGroupID, AreaName, WMOID, AmbienceID, ZoneMusic, IntroSound, AreaTableID, "
        "UWIntroSound, UWAmbience, NameSet, SoundProviderPref, SoundProviderPrefUnderwater, Flags, UWZoneMusic FROM w_m_o_area_table ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_W_M_O_AREA_TABLE, "SELECT ID, AreaName_lang FROM w_m_o_area_table_locale WHERE locale = ?", CONNECTION_SYNCH);

    // WorldMapArea.db2
    PrepareStatement(HOTFIX_SEL_WORLD_MAP_AREA, "SELECT ID, AreaName, LocLeft, LocRight, LocTop, LocBottom, MapID, AreaID, DisplayMapID, "
        "DefaultDungeonFloor, ParentWorldMapID, Flags, LevelRangeMin, LevelRangeMax, Unk, Unk2 FROM world_map_area ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_WORLD_MAP_AREA, "SELECT ID, AreaName_lang FROM world_map_area_locale WHERE locale = ?", CONNECTION_SYNCH);

    // WorldSafeLocs.db2
    PrepareStatement(HOTFIX_SEL_WORLD_SAFE_LOCS, "SELECT ID, LocX, LocY, LocZ, LocO, AreaName, MapID FROM world_safe_locs ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_WORLD_SAFE_LOCS, "SELECT ID, AreaName_lang FROM world_safe_locs_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BankBagSlotPrices.db2
    PrepareStatement(HOTFIX_SEL_BANK_BAG_SLOT_PRICES, "SELECT ID, Price FROM bank_bag_slot_prices ORDER BY ID DESC", CONNECTION_SYNCH);

    // BarberShopStyle.db2
    PrepareStatement(HOTFIX_SEL_BARBER_SHOP_STYLE, "SELECT ID, Name, Description, CostMultiplier, Type, Race, Gender, HairID FROM barber_shop_style"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BARBER_SHOP_STYLE, "SELECT ID, Name_lang, Description_lang FROM barber_shop_style_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BroadcastText.db2
    PrepareStatement(HOTFIX_SEL_BROADCAST_TEXT, "SELECT ID, MaleText, FemaleText, EmoteID1, EmoteID2, EmoteID3, EmoteDelay1, EmoteDelay2, "
        "EmoteDelay3, SoundID, UnkEmoteID, Language, Type FROM broadcast_text ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BROADCAST_TEXT, "SELECT ID, MaleText_lang, FemaleText_lang FROM broadcast_text_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CharTitles.db2
    PrepareStatement(HOTFIX_SEL_CHAR_TITLES, "SELECT ID, NameMale, NameFemale, ConditionID, MaskID, Flags FROM char_titles ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHAR_TITLES, "SELECT ID, NameMale_lang, NameFemale_lang FROM char_titles_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CurvePoint.db2
    PrepareStatement(HOTFIX_SEL_CURVE_POINT, "SELECT ID, PosX, PosY, CurveID, `Index` FROM curve_point ORDER BY ID DESC", CONNECTION_SYNCH);

    // CreatureFamily.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_FAMILY, "SELECT ID, MinScale, MaxScale, Name, IconFile, SkillLine1, SkillLine2, PetFoodMask, MaxScaleLevel, "
        "MinScaleLevel, PetTalentType, CategoryEnumID FROM creature_family ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CREATURE_FAMILY, "SELECT ID, Name_lang, IconFile_lang FROM creature_family_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Difficulty.db2
    PrepareStatement(HOTFIX_SEL_DIFFICULTY, "SELECT ID, Name, FallbackDifficultyID, InstanceType, MinPlayers, MaxPlayers, OldEnumValue, Flags, "
        "ToggleDifficultyID, GroupSizeHealthCurveID, GroupSizeDmgCurveID, GroupSizeSpellPointsCurveID, ItemBonusTreeModID, OrderIndex FROM difficulty"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_DIFFICULTY, "SELECT ID, Name_lang FROM difficulty_locale WHERE locale = ?", CONNECTION_SYNCH);

    // DungeonEncounter.db2
    PrepareStatement(HOTFIX_SEL_DUNGEON_ENCOUNTER, "SELECT ID, Name, CreatureDisplayID, MapID, OrderID, SpellIconID, DifficultyID, EncounterID, "
        "Flags FROM dungeon_encounter ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_DUNGEON_ENCOUNTER, "SELECT ID, Name_lang FROM dungeon_encounter_locale WHERE locale = ?", CONNECTION_SYNCH);

    // EmotesText.db2
    PrepareStatement(HOTFIX_SEL_EMOTES_TEXT, "SELECT ID, Name, TextID FROM emotes_text ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_EMOTES_TEXT, "SELECT ID, Name_lang FROM emotes_text_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Phase.db2
    PrepareStatement(HOTFIX_SEL_PHASE, "SELECT ID, Flags FROM phase ORDER BY ID DESC", CONNECTION_SYNCH);

    // Map.db2
    PrepareStatement(HOTFIX_SEL_MAP, "SELECT ID, Directory, Flags1, Flags2, MinimapIconScale, CorpsePosX, CorpsePosY, RaidOffset, MapName, "
        "MapDescription0, MapDescription1, AreaTableID, LoadingScreenID, CorpseMapID, TimeOfDayOverride, ParentMapID, CosmeticParentMapID, "
        "InstanceType, MapType, ExpansionID, MaxPlayers, TimeOffset FROM map ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_MAP, "SELECT ID, Directory_lang, MapName_lang, MapDescription0_lang, MapDescription1_lang FROM map_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // Criteria.db2
    PrepareStatement(HOTFIX_SEL_CRITERIA, "SELECT ID, Asset, StartAsset, FailAsset, StartTimer, ModifyTree, EligibilityWorldStateID, Type, "
        "StartEvent, FailEvent, Flags, EligibilityWorldStateValue FROM criteria ORDER BY ID DESC", CONNECTION_SYNCH);

    // CriteriaTree.db2
    PrepareStatement(HOTFIX_SEL_CRITERIA_TREE, "SELECT ID, Amount, Description, CriteriaID, Parent, Operator, OrderIndex, Flags FROM criteria_tree"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CRITERIA_TREE, "SELECT ID, Description_lang FROM criteria_tree_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CharacterLoadoutItem.db2
    PrepareStatement(HOTFIX_SEL_CHARACTER_LOADOUT_ITEM, "SELECT ID, ItemID, LoadOutID FROM character_loadout_item ORDER BY ID DESC", CONNECTION_SYNCH);

    // CharacterLoadout.db2
    PrepareStatement(HOTFIX_SEL_CHARACTER_LOADOUT, "SELECT ID, Racemask, ClassID, Purpose FROM character_loadout ORDER BY ID DESC", CONNECTION_SYNCH);

    // CharStartOutfit.db2
    PrepareStatement(HOTFIX_SEL_CHAR_START_OUTFIT, "SELECT ID, ItemID1, ItemID2, ItemID3, ItemID4, ItemID5, ItemID6, ItemID7, ItemID8, ItemID9, "
        "ItemID10, ItemID11, ItemID12, ItemID13, ItemID14, ItemID15, ItemID16, ItemID17, ItemID18, ItemID19, ItemID20, ItemID21, ItemID22, ItemID23, "
        "ItemID24, PetDisplayID, RaceID, ClassID, GenderID, OutfitID, PetFamilyID FROM char_start_outfit ORDER BY ID DESC", CONNECTION_SYNCH);

    // ChrClassesXPowerTypes.db2
    PrepareStatement(HOTFIX_SEL_CHR_CLASSES_X_POWER_TYPES, "SELECT ID, ClassID, PowerType FROM chr_classes_x_power_types ORDER BY ID DESC", CONNECTION_SYNCH);

    // ChrRaces.db2
    PrepareStatement(HOTFIX_SEL_CHR_RACES, "SELECT ID, Flags, ClientPrefix, ClientFileString, Name, NameFemale, NameMale, FacialHairCustomization1, "
        "FacialHairCustomization2, HairCustomization, CreateScreenFileDataID, SelectScreenFileDataID, MaleCustomizeOffset1, MaleCustomizeOffset2, "
        "MaleCustomizeOffset3, FemaleCustomizeOffset1, FemaleCustomizeOffset2, FemaleCustomizeOffset3, LowResScreenFileDataID, FactionID, "
        "ExplorationSoundID, MaleDisplayID, FemaleDisplayID, ResSicknessSpellID, SplashSoundID, CinematicSequenceID, UAMaleCreatureSoundDataID, "
        "UAFemaleCreatureSoundDataID, HighResMaleDisplayID, HighResFemaleDisplayID, Unk, BaseLanguage, CreatureType, TeamID, RaceRelated, "
        "UnalteredVisualRaceID, CharComponentTexLayoutHiResID, DefaultClassID, NeutralRaceID, CharComponentTextureLayoutID, UnkLegion FROM chr_races"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHR_RACES, "SELECT ID, ClientPrefix_lang, ClientFileString_lang, Name_lang, NameFemale_lang, NameMale_lang, "
        "FacialHairCustomization1_lang, FacialHairCustomization2_lang, HairCustomization_lang FROM chr_races_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CreatureDisplayInfo.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_DISPLAY_INFO, "SELECT ID, ExtendedDisplayInfoID, CreatureModelScale, Unknown620, Unknown7011, Unknown7012, "
        "Unknown7013, PortraitTextureName, PortraitCreatureDisplayInfoID, Unknown0, Unknown1, UnkLegion2, ModelID, SoundID, NPCSoundID, Unknown2, "
        "Unknown3, Unknown5, CreatureModelAlpha, SizeClass, Unknown6, Unknown7, Gender, Unknown8 FROM creature_display_info ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CREATURE_DISPLAY_INFO, "SELECT ID, PortraitTextureName_lang FROM creature_display_info_locale WHERE locale = ?", CONNECTION_SYNCH);

    // CreatureType.db2
    PrepareStatement(HOTFIX_SEL_CREATURE_TYPE, "SELECT ID, Name, Flags FROM creature_type ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CREATURE_TYPE, "SELECT ID, Name_lang FROM creature_type_locale WHERE locale = ?", CONNECTION_SYNCH);

    // DestructibleModelData.db2
    PrepareStatement(HOTFIX_SEL_DESTRUCTIBLE_MODEL_DATA, "SELECT ID, StateDamagedDisplayID, StateDestroyedDisplayID, StateRebuildingDisplayID, "
        "StateSmokeDisplayID, HealEffectSpeed, StateDamagedImpactEffectDoodadSet, StateDamagedAmbientDoodadSet, StateDamagedNameSet, "
        "StateDestroyedDestructionDoodadSet, StateDestroyedImpactEffectDoodadSet, StateDestroyedAmbientDoodadSet, StateDestroyedNameSet, "
        "StateRebuildingDestructionDoodadSet, StateRebuildingImpactEffectDoodadSet, StateRebuildingAmbientDoodadSet, StateRebuildingNameSet, "
        "StateSmokeInitDoodadSet, StateSmokeAmbientDoodadSet, StateSmokeNameSet, EjectDirection, DoNotHighlight, HealEffect"
        " FROM destructible_model_data ORDER BY ID DESC", CONNECTION_SYNCH);

    // DurabilityCosts.db2
    PrepareStatement(HOTFIX_SEL_DURABILITY_COSTS, "SELECT ID, WeaponSubstructCost1, WeaponSubstructCost2, WeaponSubstructCost3, WeaponSubstructCost4, "
        "WeaponSubstructCost5, WeaponSubstructCost6, WeaponSubstructCost7, WeaponSubstructCost8, WeaponSubstructCost9, WeaponSubstructCost10, "
        "WeaponSubstructCost11, WeaponSubstructCost12, WeaponSubstructCost13, WeaponSubstructCost14, WeaponSubstructCost15, WeaponSubstructCost16, "
        "WeaponSubstructCost17, WeaponSubstructCost18, WeaponSubstructCost19, WeaponSubstructCost20, WeaponSubstructCost21, ArmorSubstructCost1, "
        "ArmorSubstructCost2, ArmorSubstructCost3, ArmorSubstructCost4, ArmorSubstructCost5, ArmorSubstructCost6, ArmorSubstructCost7, "
        "ArmorSubstructCost8 FROM durability_costs ORDER BY ID DESC", CONNECTION_SYNCH);

    // DurabilityQuality.db2
    PrepareStatement(HOTFIX_SEL_DURABILITY_QUALITY, "SELECT ID, Data FROM durability_quality ORDER BY ID DESC", CONNECTION_SYNCH);

    // GameobjectDisplayInfo.db2
    PrepareStatement(HOTFIX_SEL_GAMEOBJECT_DISPLAY_INFO, "SELECT ID, FileDataID, GeoBoxMinX, GeoBoxMinY, GeoBoxMinZ, GeoBoxMaxX, GeoBoxMaxY, "
        "GeoBoxMaxZ, OverrideLootEffectScale, OverrideNameScale, ObjectEffectPackageID FROM gameobject_display_info ORDER BY ID DESC", CONNECTION_SYNCH);

    // ImportPriceArmor.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_ARMOR, "SELECT ID, ClothModifier, LeatherModifier, ChainModifier, PlateModifier FROM import_price_armor"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // ImportPriceQuality.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_QUALITY, "SELECT ID, Data FROM import_price_quality ORDER BY ID DESC", CONNECTION_SYNCH);

    // ImportPriceShield.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_SHIELD, "SELECT ID, Data FROM import_price_shield ORDER BY ID DESC", CONNECTION_SYNCH);

    // ImportPriceWeapon.db2
    PrepareStatement(HOTFIX_SEL_IMPORT_PRICE_WEAPON, "SELECT ID, Data FROM import_price_weapon ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemArmorQuality.db2
    PrepareStatement(HOTFIX_SEL_ITEM_ARMOR_QUALITY, "SELECT ID, QualityMod1, QualityMod2, QualityMod3, QualityMod4, QualityMod5, QualityMod6, "
        "QualityMod7, ItemLevel FROM item_armor_quality ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemArmorShield.db2
    PrepareStatement(HOTFIX_SEL_ITEM_ARMOR_SHIELD, "SELECT ID, QualityMod1, QualityMod2, QualityMod3, QualityMod4, QualityMod5, QualityMod6, "
        "QualityMod7, ItemLevel FROM item_armor_shield ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemArmorTotal.db2
    PrepareStatement(HOTFIX_SEL_ITEM_ARMOR_TOTAL, "SELECT ID, Value1, Value2, Value3, Value4, ItemLevel FROM item_armor_total ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemDamage.db2
    PrepareStatement(HOTFIX_SEL_ITEM_DAMAGE, "SELECT ID, Quality1, Quality2, Quality3, Quality4, Quality5, Quality6, Quality7, ItemLevel"
        " FROM item_damage ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemClass.db2
    PrepareStatement(HOTFIX_SEL_ITEM_CLASS, "SELECT ID, Class, PriceFactor, Name FROM item_class ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_CLASS, "SELECT ID, Name_lang FROM item_class_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemDisenchantLoot.db2
    PrepareStatement(HOTFIX_SEL_ITEM_DISENCHANT_LOOT, "SELECT ID, MinLevel, MaxLevel, SkillRequired, ItemClass, ItemSubClass, Quality"
        " FROM item_disenchant_loot ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemLimitCategory.db2
    PrepareStatement(HOTFIX_SEL_ITEM_LIMIT_CATEGORY, "SELECT ID, NameLang, Quantity, Flags FROM item_limit_category ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_LIMIT_CATEGORY, "SELECT ID, NameLang_lang FROM item_limit_category_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemPriceBase.db2
    PrepareStatement(HOTFIX_SEL_ITEM_PRICE_BASE, "SELECT ID, Armor, Weapon, ItemLevel FROM item_price_base ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemRandomProperties.db2
    PrepareStatement(HOTFIX_SEL_ITEM_RANDOM_PROPERTIES, "SELECT ID, Name, Enchantment1, Enchantment2, Enchantment3, Enchantment4, Enchantment5"
        " FROM item_random_properties ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_RANDOM_PROPERTIES, "SELECT ID, Name_lang FROM item_random_properties_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemRandomSuffix.db2
    PrepareStatement(HOTFIX_SEL_ITEM_RANDOM_SUFFIX, "SELECT ID, Name, IternalName, Enchantment1, Enchantment2, Enchantment3, Enchantment4, "
        "Enchantment5, AllocationPct1, AllocationPct2, AllocationPct3, AllocationPct4, AllocationPct5 FROM item_random_suffix ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_RANDOM_SUFFIX, "SELECT ID, Name_lang, IternalName_lang FROM item_random_suffix_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemSpecOverride.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPEC_OVERRIDE, "SELECT ID, ItemID, SpecID FROM item_spec_override ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemSpec.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPEC, "SELECT ID, SpecID, MinLevel, MaxLevel, ItemType, PrimaryStat, SecondaryStat FROM item_spec"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemSetSpell.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SET_SPELL, "SELECT ID, SpellID, ItemSetID, ChrSpecID, Threshold FROM item_set_spell ORDER BY ID DESC", CONNECTION_SYNCH);

    // GuildPerkSpells.db2
    PrepareStatement(HOTFIX_SEL_GUILD_PERK_SPELLS, "SELECT ID, SpellId, Level FROM guild_perk_spells ORDER BY ID DESC", CONNECTION_SYNCH);

    // Heirloom.db2
    PrepareStatement(HOTFIX_SEL_HEIRLOOM, "SELECT ID, ItemID, SourceText, OldItem1, OldItem2, NextDifficultyItemID, UpgradeItemID1, UpgradeItemID2, "
        "ItemBonusListID1, ItemBonusListID2, Flags, Source FROM heirloom ORDER BY ID DESC", CONNECTION_SYNCH);
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
    PrepareStatement(HOTFIX_SEL_ITEM_APPEARANCE, "SELECT ID, DisplayID, FileDataID, UnkLegion, UnkLegion2 FROM item_appearance ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemModifiedAppearance.db2
    PrepareStatement(HOTFIX_SEL_ITEM_MODIFIED_APPEARANCE, "SELECT ID, ItemID, AppearanceID, AppearanceModID, VariationID, UnkLegion"
        " FROM item_modified_appearance ORDER BY ID DESC", CONNECTION_SYNCH);

    // MailTemplate.db2
    PrepareStatement(HOTFIX_SEL_MAIL_TEMPLATE, "SELECT ID, Content FROM mail_template ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_MAIL_TEMPLATE, "SELECT ID, Content_lang FROM mail_template_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ModifierTree.db2
    PrepareStatement(HOTFIX_SEL_MODIFIER_TREE, "SELECT ID, Asset, SecondaryAsset, Parent, Type, UnkLegion, Operator, Amount FROM modifier_tree"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // MountCapability.db2
    PrepareStatement(HOTFIX_SEL_MOUNT_CAPABILITY, "SELECT ID, RequiredSpell, SpeedModSpell, RequiredRidingSkill, RequiredArea, RequiredMap, Flags, "
        "RequiredAura FROM mount_capability ORDER BY ID DESC", CONNECTION_SYNCH);

    // MountType.db2
    PrepareStatement(HOTFIX_SEL_MOUNT_TYPE, "SELECT ID FROM mount_type ORDER BY ID DESC", CONNECTION_SYNCH);

    // MountTypeXCapability.db2
    PrepareStatement(HOTFIX_SEL_MOUNT_TYPE_X_CAPABILITY, "SELECT ID, MountTypeID, MountCapabilityID, OrderIndex FROM mount_type_x_capability"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // NameGen.db2
    PrepareStatement(HOTFIX_SEL_NAME_GEN, "SELECT ID, Name, RaceID, Gender FROM name_gen ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_NAME_GEN, "SELECT ID, Name_lang FROM name_gen_locale WHERE locale = ?", CONNECTION_SYNCH);

    // NamesProfanity.db2
    PrepareStatement(HOTFIX_SEL_NAMES_PROFANITY, "SELECT ID, Name, Language FROM names_profanity ORDER BY ID DESC", CONNECTION_SYNCH);

    // NamesReserved.db2
    PrepareStatement(HOTFIX_SEL_NAMES_RESERVED, "SELECT ID, Name FROM names_reserved ORDER BY ID DESC", CONNECTION_SYNCH);

    // NamesReservedLocale.db2
    PrepareStatement(HOTFIX_SEL_NAMES_RESERVED_LOCALE, "SELECT ID, Name, LocaleMask FROM names_reserved_locale ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestFactionRew.db2
    PrepareStatement(HOTFIX_SEL_QUEST_FACTION_REW, "SELECT ID, Difficulty1, Difficulty2, Difficulty3, Difficulty4, Difficulty5, Difficulty6, "
        "Difficulty7, Difficulty8, Difficulty9, Difficulty10 FROM quest_faction_rew ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestPOIPoint.db2
    PrepareStatement(HOTFIX_SEL_QUEST_P_O_I_POINT, "SELECT ID, QuestPOIBlobID, X, Y FROM quest_p_o_i_point ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestSort.db2
    PrepareStatement(HOTFIX_SEL_QUEST_SORT, "SELECT ID, SortName FROM quest_sort ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_QUEST_SORT, "SELECT ID, SortName_lang FROM quest_sort_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestV2.db2
    PrepareStatement(HOTFIX_SEL_QUEST_V2, "SELECT ID, UniqueBitFlag FROM quest_v2 ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestV2CliTask.db2
    PrepareStatement(HOTFIX_SEL_QUEST_V2_CLI_TASK, "SELECT ID, Flags, Unk, Name, Name1, Unk2, Unk3, Unk4, Unk5, Unk6, "
    "Unk7, Unk8, Unk9, Unk10, Unk11, Unk12, Unk13, Unk14, Unk15, Unk16, Unk17, Unk18, Unk19, Unk20 FROM quest_v2_cli_task ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_QUEST_V2_CLI_TASK, "SELECT ID, Name_lang , Name1_lang FROM quest_v2_cli_task_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestXp.db2
    PrepareStatement(HOTFIX_SEL_QUEST_XP, "SELECT ID, Difficulty1, Difficulty2, Difficulty3, Difficulty4, Difficulty5, Difficulty6, Difficulty7, "
        "Difficulty8, Difficulty9, Difficulty10 FROM quest_xp ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestLine.db2
    PrepareStatement(HOTFIX_SEL_QUEST_LINE, "SELECT ID, LineName FROM quest_line ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_QUEST_LINE, "SELECT ID, LineName_lang FROM quest_line_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestLineXQuest.db2
    PrepareStatement(HOTFIX_SEL_QUEST_LINE_X_QUEST, "SELECT ID, QuestID, LineID, Pos FROM quest_line_x_quest ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestMoneyReward.db2
    PrepareStatement(HOTFIX_SEL_QUEST_MONEY_REWARD, "SELECT ID, Money1, Money2, Money3, Money4, Money5, Money6, Money7, Money8, Money9, Money10"
        " FROM quest_money_reward ORDER BY ID DESC", CONNECTION_SYNCH);

    // QuestObjective.db2
    PrepareStatement(HOTFIX_SEL_QUEST_OBJECTIVE, "SELECT ID, Amount, ObjectID, Description, QuestID, Type, StorageIndex, UNK, Flags"
        " FROM quest_objective ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_QUEST_OBJECTIVE, "SELECT ID, Description_lang FROM quest_objective_locale WHERE locale = ?", CONNECTION_SYNCH);

    // RandPropPoints.db2
    PrepareStatement(HOTFIX_SEL_RAND_PROP_POINTS, "SELECT ID, EpicPropertiesPoints1, EpicPropertiesPoints2, EpicPropertiesPoints3, "
        "EpicPropertiesPoints4, EpicPropertiesPoints5, RarePropertiesPoints1, RarePropertiesPoints2, RarePropertiesPoints3, RarePropertiesPoints4, "
        "RarePropertiesPoints5, UncommonPropertiesPoints1, UncommonPropertiesPoints2, UncommonPropertiesPoints3, UncommonPropertiesPoints4, "
        "UncommonPropertiesPoints5 FROM rand_prop_points ORDER BY ID DESC", CONNECTION_SYNCH);

    // ResearchBranch.db2
    PrepareStatement(HOTFIX_SEL_RESEARCH_BRANCH, "SELECT ID, Name, Texture, ItemID, CurrencyID, ResearchFieldID FROM research_branch ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_RESEARCH_BRANCH, "SELECT ID, Name_lang, Texture_lang FROM research_branch_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ResearchProject.db2
    PrepareStatement(HOTFIX_SEL_RESEARCH_PROJECT, "SELECT ID, Name, Description, SpellID, IconName, BranchID, Rare, Complexity, "
        "RequiredCurrencyAmount FROM research_project ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_RESEARCH_PROJECT, "SELECT ID, Name_lang, Description_lang, IconName_lang FROM research_project_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // ResearchSite.db2
    PrepareStatement(HOTFIX_SEL_RESEARCH_SITE, "SELECT ID, POIid, areaName, MapID, flags FROM research_site ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_RESEARCH_SITE, "SELECT ID, areaName_lang FROM research_site_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ItemBonus.db2
    PrepareStatement(HOTFIX_SEL_ITEM_BONUS, "SELECT ID, Value1, Value2, BonusListID, Type, `Index` FROM item_bonus ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemBonusTreeNode.db2
    PrepareStatement(HOTFIX_SEL_ITEM_BONUS_TREE_NODE, "SELECT ID, BonusTreeID, SubTreeID, BonusListID, BonusTreeModID FROM item_bonus_tree_node"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemCurrencyCost.db2
    PrepareStatement(HOTFIX_SEL_ITEM_CURRENCY_COST, "SELECT ID, ItemId FROM item_currency_cost ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemEffect.db2
    PrepareStatement(HOTFIX_SEL_ITEM_EFFECT, "SELECT ID, ItemID, SpellID, Cooldown, CategoryCooldown, Charges, Category, SpecID, OrderIndex, "
        "`Trigger` FROM item_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemSparse.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SPARSE, "SELECT ID, Flags, UnkFloat1, UnkFloat2, BuyPrice, SellPrice, AllowableClass, AllowableRace, "
        "RequiredSpell, MaxCount, Stackable, ItemStatAllocation, ItemStatSocketCostMultiplier, RangedModRange, Name, Name2, Name3, Name4, "
        "Description, BagFamily, ArmorDamageModifier, Duration, StatScalingFactor, ItemLevel, RequiredSkill, RequiredSkillRank, "
        "RequiredReputationFaction, ItemStatValue, ScalingStatDistribution, Delay, PageText, StartQuest, LockID, RandomProperty, RandomSuffix, "
        "ItemSet, Area, Map, SocketBonus, GemProperties, ItemLimitCategory, HolidayID, ItemNameDescriptionID, Quality, BuyCount, InventoryType, "
        "RequiredLevel, RequiredHonorRank, RequiredCityRank, RequiredReputationRank, ContainerSlots, ItemStatType, DamageType, Bonding, LanguageID, "
        "PageMaterial, Material, Sheath, TotemCategory, SocketColor, CurrencySubstitutionID, CurrencySubstitutionCount, ArtifactID, UnkLegion2"
        " FROM item_sparse ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_SPARSE, "SELECT ID, Name_lang, Name2_lang, Name3_lang, Name4_lang, Description_lang FROM item_sparse_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // ItemExtendedCost.db2
    PrepareStatement(HOTFIX_SEL_ITEM_EXTENDED_COST, "SELECT ID, RequiredItem1, RequiredItem2, RequiredItem3, RequiredItem4, RequiredItem5, "
        "RequiredCurrencyCount1, RequiredCurrencyCount2, RequiredCurrencyCount3, RequiredCurrencyCount4, RequiredCurrencyCount5, RequiredMoney, "
        "RequiredItemCount1, RequiredItemCount2, RequiredItemCount3, RequiredItemCount4, RequiredItemCount5, RequiredPersonalArenaRating, "
        "RequiredCurrency1, RequiredCurrency2, RequiredCurrency3, RequiredCurrency4, RequiredCurrency5, RequiredArenaSlot, ItemPurchaseGroup, "
        "RequiredFactionId, RequirementFlags, RequiredFactionStanding FROM item_extended_cost ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetAbility.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY, "SELECT ID, IconFileID, Name, Description, AuraAbilityID, School, Cooldown, AuraDuration"
        " FROM battle_pet_ability ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_ABILITY, "SELECT ID, Name_lang, Description_lang FROM battle_pet_ability_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetAbilityEffect.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY_EFFECT, "SELECT ID, TurnID, Unk1, AuraID, PropertyValue1, PropertyValue2, PropertyValue3, "
        "PropertyValue4, PropertyValue5, PropertyValue6, EffectPropertiesID, Effect FROM battle_pet_ability_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetEffectProperties.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_EFFECT_PROPERTIES, "SELECT ID, Description1, Description2, Description3, Description4, Description5, "
        "Description6, Flags, IsAura1, IsAura2, IsAura3, IsAura4, IsAura5, IsAura6 FROM battle_pet_effect_properties ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_EFFECT_PROPERTIES, "SELECT ID, Description1_lang, Description2_lang, Description3_lang, "
        "Description4_lang, Description5_lang, Description6_lang FROM battle_pet_effect_properties_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetAbilityTurn.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY_TURN, "SELECT ID, AbilityID, StateID, Turn, HasProcType, ProcType FROM battle_pet_ability_turn"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetAbilityState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_ABILITY_STATE, "SELECT ID, Value, AbilityID, State FROM battle_pet_ability_state ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_STATE, "SELECT ID, stateName, flags, unk FROM battle_pet_state ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_STATE, "SELECT ID, stateName_lang FROM battle_pet_state_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetSpecies.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_SPECIES, "SELECT ID, CreatureEntry, IconFileID, spellId, SourceText, Description, flags, petType, source"
        " FROM battle_pet_species ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_BATTLE_PET_SPECIES, "SELECT ID, SourceText_lang, Description_lang FROM battle_pet_species_locale WHERE locale = ?", CONNECTION_SYNCH);

    // BattlePetSpeciesState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_SPECIES_STATE, "SELECT ID, Value, SpeciesID, State FROM battle_pet_species_state ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetSpeciesXAbility.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_SPECIES_X_ABILITY, "SELECT ID, SpeciesID, AbilityID, Level, Slot FROM battle_pet_species_x_ability"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetBreedQuality.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_BREED_QUALITY, "SELECT ID, Modifier, Quality FROM battle_pet_breed_quality ORDER BY ID DESC", CONNECTION_SYNCH);

    // BattlePetBreedState.db2
    PrepareStatement(HOTFIX_SEL_BATTLE_PET_BREED_STATE, "SELECT ID, stateModifier, breedID, stateID FROM battle_pet_breed_state ORDER BY ID DESC", CONNECTION_SYNCH);

    // CharShipment.db2
    PrepareStatement(HOTFIX_SEL_CHAR_SHIPMENT, "SELECT ID, ShipmentConteinerID, CriteriaID, Flags, TimeForShipment, SpellCreation, "
        "ShipmentResultItemID FROM char_shipment ORDER BY ID DESC", CONNECTION_SYNCH);

    // CharShipmentConteiner.db2
    PrepareStatement(HOTFIX_SEL_CHAR_SHIPMENT_CONTEINER, "SELECT ID, Flags, BuildingType, Name, unt3, unk4, unk5, unk6, unk7, MaxShipments, "
        "Description, enotherID, fractionType FROM char_shipment_conteiner ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHAR_SHIPMENT_CONTEINER, "SELECT ID, Name_lang, Description_lang FROM char_shipment_conteiner_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

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
    PrepareStatement(HOTFIX_SEL_GARR_ENCOUNTER, "SELECT ID, CreatureEntry, Name, `Mod`, Mod1, SomeFileDataID FROM garr_encounter ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GARR_ENCOUNTER, "SELECT ID, Name_lang FROM garr_encounter_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GarrEncounterXMechanic.db2
    PrepareStatement(HOTFIX_SEL_GARR_ENCOUNTER_X_MECHANIC, "SELECT ID, GarrEncounterID, GarrMechanicID FROM garr_encounter_x_mechanic ORDER BY ID DESC", CONNECTION_SYNCH);

    // GarrMechanic.db2
    PrepareStatement(HOTFIX_SEL_GARR_MECHANIC, "SELECT ID, Type, `Mod` FROM garr_mechanic ORDER BY ID DESC", CONNECTION_SYNCH);

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
    PrepareStatement(HOTFIX_SEL_CREATURE_DIFFICULTY, "SELECT ID, CreatureID, Flags1, Flags2, Flags3, Flags4, Flags5, UnkLegion, UnkLegion2, "
        "FactionID, Expansion, MinLevel, MaxLevel FROM creature_difficulty ORDER BY ID DESC", CONNECTION_SYNCH);

    // CurrencyTypes.db2
    PrepareStatement(HOTFIX_SEL_CURRENCY_TYPES, "SELECT ID, Name, InventoryIcon1, InventoryIcon2, MaxQty, MaxEarnablePerWeek, Flags, Description, "
        "CategoryID, SpellWeight, SpellCategory, Quality FROM currency_types ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CURRENCY_TYPES, "SELECT ID, Name_lang, InventoryIcon1_lang, InventoryIcon2_lang, Description_lang"
        " FROM currency_types_locale WHERE locale = ?", CONNECTION_SYNCH);

    // QuestPackageItem.db2
    PrepareStatement(HOTFIX_SEL_QUEST_PACKAGE_ITEM, "SELECT ID, ItemID, QuestPackageID, ItemCount, FilterType FROM quest_package_item ORDER BY ID DESC", CONNECTION_SYNCH);

    // Mount.db2
    PrepareStatement(HOTFIX_SEL_MOUNT, "SELECT ID, SpellID, DisplayID, Name, Description, SourceDescription, MountTypeID, Flags, PlayerConditionId, "
        "Source FROM mount ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_MOUNT, "SELECT ID, Name_lang, Description_lang, SourceDescription_lang FROM mount_locale WHERE locale = ?", CONNECTION_SYNCH);

    // LanguageWords.db2
    PrepareStatement(HOTFIX_SEL_LANGUAGE_WORDS, "SELECT ID, Word, Lang FROM language_words ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_LANGUAGE_WORDS, "SELECT ID, Word_lang FROM language_words_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Light.db2
    PrepareStatement(HOTFIX_SEL_LIGHT, "SELECT ID, PosX, PosY, PosZ, FalloffStart, FalloffEnd, MapID, LightParamsID1, LightParamsID2, LightParamsID3, "
        "LightParamsID4, LightParamsID5, LightParamsID6, LightParamsID7, LightParamsID8 FROM light ORDER BY ID DESC", CONNECTION_SYNCH);

    // KeyChain.db2
    PrepareStatement(HOTFIX_SEL_KEY_CHAIN, "SELECT Id, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key10, Key11, Key12, Key13, Key14, "
        "Key15, Key16, Key17, Key18, Key19, Key20, Key21, Key22, Key23, Key24, Key25, Key26, Key27, Key28, Key29, Key30, Key31, Key32 FROM key_chain"
        " ORDER BY Id DESC", CONNECTION_SYNCH);

    // OverrideSpellData.db2
    PrepareStatement(HOTFIX_SEL_OVERRIDE_SPELL_DATA, "SELECT ID, SpellID1, SpellID2, SpellID3, SpellID4, SpellID5, SpellID6, SpellID7, SpellID8, "
        "SpellID9, SpellID10, PlayerActionbarFileDataID, Flags FROM override_spell_data ORDER BY ID DESC", CONNECTION_SYNCH);

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

    // SpellMiscDifficulty.db2
    PrepareStatement(HOTFIX_SEL_SPELL_MISC_DIFFICULTY, "SELECT ID, SpellID, DifficultyID FROM spell_misc_difficulty ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellPower.db2
    PrepareStatement(HOTFIX_SEL_SPELL_POWER, "SELECT ID, SpellID, PowerCost, PowerCostPercentage, PowerCostPercentagePerSecond, RequiredAura, "
        "HealthCostPercentage, PowerCostPerSecond, ManaCostAdditional, PowerDisplayID, UnitPowerBarID, PowerIndex, PowerType, PowerCostPerLevel"
        " FROM spell_power ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellReagents.db2
    PrepareStatement(HOTFIX_SEL_SPELL_REAGENTS, "SELECT ID, SpellID, Reagent1, Reagent2, Reagent3, Reagent4, Reagent5, Reagent6, Reagent7, Reagent8, "
        "ReagentCount1, ReagentCount2, ReagentCount3, ReagentCount4, ReagentCount5, ReagentCount6, ReagentCount7, ReagentCount8 FROM spell_reagents"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellReagentsCurrency.db2
    PrepareStatement(HOTFIX_SEL_SPELL_REAGENTS_CURRENCY, "SELECT ID, SpellID, CurrencyID, CurrencyCount FROM spell_reagents_currency ORDER BY ID DESC", CONNECTION_SYNCH);

    // Toy.db2
    PrepareStatement(HOTFIX_SEL_TOY, "SELECT ID, ItemID, Description, Flags, CategoryFilter FROM toy ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TOY, "SELECT ID, Description_lang FROM toy_locale WHERE locale = ?", CONNECTION_SYNCH);

    // PvpItem.db2
    PrepareStatement(HOTFIX_SEL_PVP_ITEM, "SELECT ID, ItemID, BonusIlvl FROM pvp_item ORDER BY ID DESC", CONNECTION_SYNCH);

    // PvpTalentUnlock.db2
    PrepareStatement(HOTFIX_SEL_PVP_TALENT_UNLOCK, "SELECT ID, `Row`, `Column`, HonorLevel FROM pvp_talent_unlock ORDER BY ID DESC", CONNECTION_SYNCH);

    // PvpTalent.db2
    PrepareStatement(HOTFIX_SEL_PVP_TALENT, "SELECT ID, `Row`, `Column`, SpellID, Unknown4, ClassID, SpecializationID, Flags, OverrideSpellID, `Desc`"
        " FROM pvp_talent ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_PVP_TALENT, "SELECT ID, Desc_lang FROM pvp_talent_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellTotems.db2
    PrepareStatement(HOTFIX_SEL_SPELL_TOTEMS, "SELECT ID, SpellID, Totem1, Totem2, TotemCategory1, TotemCategory2 FROM spell_totems ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellXSpellVisual.db2
    PrepareStatement(HOTFIX_SEL_SPELL_X_SPELL_VISUAL, "SELECT ID, SpellID, TravelSpeed, SpellVisualID1, SpellVisualID2, PlayerConditionID, "
        "DifficultyID, Flags FROM spell_x_spell_visual ORDER BY ID DESC", CONNECTION_SYNCH);

    // TaxiNodes.db2
    PrepareStatement(HOTFIX_SEL_TAXI_NODES, "SELECT ID, PosX, PosY, PosZ, Name, MountCreatureID1, MountCreatureID2, MapOffset1, MapOffset2, MapID, "
        "ConditionID, LearnableIndex, Flags FROM taxi_nodes ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TAXI_NODES, "SELECT ID, Name_lang FROM taxi_nodes_locale WHERE locale = ?", CONNECTION_SYNCH);

    // TaxiPath.db2
    PrepareStatement(HOTFIX_SEL_TAXI_PATH, "SELECT ID, `From`, `To`, Cost FROM taxi_path ORDER BY ID DESC", CONNECTION_SYNCH);

    // TaxiPathNode.db2
    PrepareStatement(HOTFIX_SEL_TAXI_PATH_NODE, "SELECT ID, LocX, LocY, LocZ, Delay, PathID, MapID, ArrivalEventID, DepartureEventID, NodeIndex, "
        "Flags FROM taxi_path_node ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemUpgrade.db2
    PrepareStatement(HOTFIX_SEL_ITEM_UPGRADE, "SELECT id, currencyReqAmt, prevUpgradeId, currencyReqId, itemUpgradePathId, levelBonus"
        " FROM item_upgrade ORDER BY id DESC", CONNECTION_SYNCH);

    // RuleSetItemUpgrade.db2
    PrepareStatement(HOTFIX_SEL_RULE_SET_ITEM_UPGRADE, "SELECT id, levelUpd, startUpgrade, itemEntry FROM rule_set_item_upgrade ORDER BY id DESC", CONNECTION_SYNCH);

    // Gameobjects.db2
    PrepareStatement(HOTFIX_SEL_GAMEOBJECTS, "SELECT ID, PositionX, PositionY, PositionZ, RotationX, RotationY, RotationZ, RotationO, Size, Data1, "
        "Data2, Data3, Data4, Data5, Data6, Data7, Data8, Name, MapID, DisplayID, PhaseID, PhaseUseFlags, PhaseGroupID, Type FROM gameobjects"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_GAMEOBJECTS, "SELECT ID, Name_lang FROM gameobjects_locale WHERE locale = ?", CONNECTION_SYNCH);

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
    PrepareStatement(HOTFIX_SEL_SPELL_EFFECT, "SELECT ID, EffectAmplitude, EffectAuraPeriod, EffectBasePoints, EffectBonusCoefficient, "
        "EffectChainAmplitude, EffectDieSides, EffectItemType, EffectMiscValue1, EffectMiscValue2, EffectPointsPerResource, EffectRealPointsPerLevel, "
        "EffectSpellClassMask1, EffectSpellClassMask2, EffectSpellClassMask3, EffectSpellClassMask4, EffectTriggerSpell, EffectPosFacing, SpellID, "
        "EffectAttributes, BonusCoefficientFromAP, EffectAura, EffectChainTargets, DifficultyID, Effect, EffectMechanic, EffectRadiusIndex1, "
        "EffectRadiusIndex2, ImplicitTarget1, ImplicitTarget2, EffectIndex FROM spell_effect ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellShapeshiftForm.db2
    PrepareStatement(HOTFIX_SEL_SPELL_SHAPESHIFT_FORM, "SELECT ID, Name, UnkLegion, Flags, AttackIconID, CombatRoundTime, CreatureDisplayID1, "
        "CreatureDisplayID2, CreatureDisplayID3, CreatureDisplayID4, PresetSpellID1, PresetSpellID2, PresetSpellID3, PresetSpellID4, PresetSpellID5, "
        "PresetSpellID6, PresetSpellID7, PresetSpellID8, CreatureType, MountTypeID, ExitSoundEntriesID FROM spell_shapeshift_form ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_SHAPESHIFT_FORM, "SELECT ID, Name_lang FROM spell_shapeshift_form_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellShapeshift.db2
    PrepareStatement(HOTFIX_SEL_SPELL_SHAPESHIFT, "SELECT ID, SpellID, ShapeshiftExclude1, ShapeshiftExclude2, ShapeshiftMask1, ShapeshiftMask2, "
        "StanceBarOrder FROM spell_shapeshift ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellTargetRestrictions.db2
    PrepareStatement(HOTFIX_SEL_SPELL_TARGET_RESTRICTIONS, "SELECT ID, SpellID, ConeAngle, Width, Targets, MaxTargetLevel, TargetCreatureType, "
        "DifficultyID, MaxAffectedTargets FROM spell_target_restrictions ORDER BY ID DESC", CONNECTION_SYNCH);

    // Spell.db2
    PrepareStatement(HOTFIX_SEL_SPELL, "SELECT ID, Name, NameSubtext, Description, AuraDescription, SpellMiscDifficultyID, DescriptionVariablesID"
        " FROM spell ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL, "SELECT ID, Name_lang, NameSubtext_lang, Description_lang, AuraDescription_lang FROM spell_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // SpellScaling.db2
    PrepareStatement(HOTFIX_SEL_SPELL_SCALING, "SELECT ID, SpellID, ScalesFromItemLevel, ScalingClass, MaxScalingLevel FROM spell_scaling"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellRange.db2
    PrepareStatement(HOTFIX_SEL_SPELL_RANGE, "SELECT ID, MinRangeHostile, MinRangeFriend, MaxRangeHostile, MaxRangeFriend, DisplayName, "
        "DisplayNameShort, Flags FROM spell_range ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_RANGE, "SELECT ID, DisplayName_lang, DisplayNameShort_lang FROM spell_range_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellRadius.db2
    PrepareStatement(HOTFIX_SEL_SPELL_RADIUS, "SELECT ID, Radius, RadiusPerLevel, RadiusMin, RadiusMax FROM spell_radius ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellProcsPerMinute.db2
    PrepareStatement(HOTFIX_SEL_SPELL_PROCS_PER_MINUTE, "SELECT ID, BaseProcRate, Flags FROM spell_procs_per_minute ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellProcsPerMinuteMod.db2
    PrepareStatement(HOTFIX_SEL_SPELL_PROCS_PER_MINUTE_MOD, "SELECT ID, PpmRateMod, Type, SpecID, SpellProcsPerMinuteID"
        " FROM spell_procs_per_minute_mod ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellLevels.db2
    PrepareStatement(HOTFIX_SEL_SPELL_LEVELS, "SELECT ID, SpellID, BaseLevel, MaxLevel, SpellLevel, DifficultyID, Unk701 FROM spell_levels"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellItemEnchantmentCondition.db2
    PrepareStatement(HOTFIX_SEL_SPELL_ITEM_ENCHANTMENT_CONDITION, "SELECT ID, Color1, Color2, Color3, Color4, Color5, LtOperandType1, LtOperandType2, "
        "LtOperandType3, LtOperandType4, LtOperandType5, Comparator1, Comparator2, Comparator3, Comparator4, Comparator5, CompareColor1, "
        "CompareColor2, CompareColor3, CompareColor4, CompareColor5, Value1, Value2, Value3, Value4, Value5, Logic1, Logic2, Logic3, Logic4, Logic5"
        " FROM spell_item_enchantment_condition ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellInterrupts.db2
    PrepareStatement(HOTFIX_SEL_SPELL_INTERRUPTS, "SELECT ID, SpellID, AuraInterruptFlags1, AuraInterruptFlags2, ChannelInterruptFlags1, "
        "ChannelInterruptFlags2, InterruptFlags, DifficultyID FROM spell_interrupts ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellFocusObject.db2
    PrepareStatement(HOTFIX_SEL_SPELL_FOCUS_OBJECT, "SELECT ID, Name FROM spell_focus_object ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_FOCUS_OBJECT, "SELECT ID, Name_lang FROM spell_focus_object_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellEquippedItems.db2
    PrepareStatement(HOTFIX_SEL_SPELL_EQUIPPED_ITEMS, "SELECT ID, SpellID, EquippedItemInventoryTypeMask, EquippedItemSubClassMask, "
        "EquippedItemClass FROM spell_equipped_items ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellEffectScaling.db2
    PrepareStatement(HOTFIX_SEL_SPELL_EFFECT_SCALING, "SELECT ID, Coefficient, Variance, OtherCoefficient, SpellEffectId FROM spell_effect_scaling"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellDuration.db2
    PrepareStatement(HOTFIX_SEL_SPELL_DURATION, "SELECT ID, Duration, MaxDuration, DurationPerLevel FROM spell_duration ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellCooldowns.db2
    PrepareStatement(HOTFIX_SEL_SPELL_COOLDOWNS, "SELECT ID, SpellID, CategoryRecoveryTime, RecoveryTime, StartRecoveryTime, DifficultyID"
        " FROM spell_cooldowns ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellCategory.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CATEGORY, "SELECT ID, Name, ChargeRecoveryTime, UnkLegion, Flags, UsesPerWeek, MaxCharges FROM spell_category"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPELL_CATEGORY, "SELECT ID, Name_lang FROM spell_category_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SpellCategories.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CATEGORIES, "SELECT ID, SpellID, Category, StartRecoveryCategory, ChargeCategory, DifficultyID, DefenseType, "
        "DispelType, Mechanic, PreventionType FROM spell_categories ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellCastTimes.db2
    PrepareStatement(HOTFIX_SEL_SPELL_CAST_TIMES, "SELECT ID, Base, Minimum, PerLevel FROM spell_cast_times ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpellAuraOptions.db2
    PrepareStatement(HOTFIX_SEL_SPELL_AURA_OPTIONS, "SELECT ID, SpellID, ProcCharges, ProcTypeMask, ProcCategoryRecovery, CumulativeAura, "
        "DifficultyID, ProcChance, SpellProcsPerMinuteID FROM spell_aura_options ORDER BY ID DESC", CONNECTION_SYNCH);

    // SpecializationSpell.db2
    PrepareStatement(HOTFIX_SEL_SPECIALIZATION_SPELL, "SELECT ID, SpellID, OverridesSpellID, DescriptionLang, SpecID, UnkLegion"
        " FROM specialization_spell ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SPECIALIZATION_SPELL, "SELECT ID, DescriptionLang_lang FROM specialization_spell_locale WHERE locale = ?", CONNECTION_SYNCH);

    // SkillRaceClassInfo.db2
    PrepareStatement(HOTFIX_SEL_SKILL_RACE_CLASS_INFO, "SELECT ID, RaceMask, SkillID, ClassMask, Flags, SkillTierID, Availability, MinLevel"
        " FROM skill_race_class_info ORDER BY ID DESC", CONNECTION_SYNCH);

    // SkillLine.db2
    PrepareStatement(HOTFIX_SEL_SKILL_LINE, "SELECT ID, DisplayName, Description, AlternateVerb, SpellIconID, Flags, CategoryID, CanLink, "
        "ParentSkillLineID FROM skill_line ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SKILL_LINE, "SELECT ID, DisplayName_lang, Description_lang, AlternateVerb_lang FROM skill_line_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // SkillLineAbility.db2
    PrepareStatement(HOTFIX_SEL_SKILL_LINE_ABILITY, "SELECT ID, SpellID, RaceMask, ClassMask, SupercedesSpell, SkillLine, MinSkillLineRank, "
        "TrivialSkillLineRankHigh, TrivialSkillLineRankLow, UniqueBit, TradeSkillCategoryID, AquireMethod, NumSkillUps FROM skill_line_ability"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // Scenario.db2
    PrepareStatement(HOTFIX_SEL_SCENARIO, "SELECT ID, Name, Flags, UnkLegion, UnkLegion2 FROM scenario ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SCENARIO, "SELECT ID, Name_lang FROM scenario_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ScenarioStep.db2
    PrepareStatement(HOTFIX_SEL_SCENARIO_STEP, "SELECT ID, DescriptionLang, TitleLang, Criteriatreeid, ScenarioID, Supersedes, RewardQuestID, "
        "OrderIndex, Flags, RelatedStep FROM scenario_step ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_SCENARIO_STEP, "SELECT ID, DescriptionLang_lang, TitleLang_lang FROM scenario_step_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ScalingStatDistribution.db2
    PrepareStatement(HOTFIX_SEL_SCALING_STAT_DISTRIBUTION, "SELECT ID, ItemLevelCurveID, MinLevel, MaxLevel FROM scaling_stat_distribution"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // TotemCategory.db2
    PrepareStatement(HOTFIX_SEL_TOTEM_CATEGORY, "SELECT ID, Name, TotemCategoryType, TotemCategoryMask FROM totem_category ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_TOTEM_CATEGORY, "SELECT ID, Name_lang FROM totem_category_locale WHERE locale = ?", CONNECTION_SYNCH);

    // TransportAnimation.db2
    PrepareStatement(HOTFIX_SEL_TRANSPORT_ANIMATION, "SELECT ID, TransportID, TimeIndex, PosX, PosY, PosZ, SequenceID FROM transport_animation"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // UnitPowerBar.db2
    PrepareStatement(HOTFIX_SEL_UNIT_POWER_BAR, "SELECT ID, MaxPower, RegenerationPeace, RegenerationCombat, FileDataID1, FileDataID2, FileDataID3, "
        "FileDataID4, FileDataID5, FileDataID6, Color1, Color2, Color3, Color4, Color5, Color6, Name, Cost, OutOfError, ToolTip, StartInset, "
        "EndInset, StartPower, Flags, MinPower, CenterPower, BarType FROM unit_power_bar ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_UNIT_POWER_BAR, "SELECT ID, Name_lang, Cost_lang, OutOfError_lang, ToolTip_lang FROM unit_power_bar_locale"
        " WHERE locale = ?", CONNECTION_SYNCH);

    // WorldMapOverlay.db2
    PrepareStatement(HOTFIX_SEL_WORLD_MAP_OVERLAY, "SELECT ID, TextureName, MapAreaID, AreaID1, AreaID2, AreaID3, AreaID4, TextureWidth, "
        "TextureHeight, OffsetX, OffsetY, HitRectTop, HitRectLeft, HitRectBottom, HitRectRight, PlayerConditionID FROM world_map_overlay"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_WORLD_MAP_OVERLAY, "SELECT ID, TextureName_lang FROM world_map_overlay_locale WHERE locale = ?", CONNECTION_SYNCH);

    // WorldMapTransforms.db2
    PrepareStatement(HOTFIX_SEL_WORLD_MAP_TRANSFORMS, "SELECT ID, RegionMinX, RegionMinY, RegionMinZ, RegionMaxX, RegionMaxY, RegionMaxZ, "
        "RegionOffsetX, RegionOffsetY, RegionScale, MapID, NewMapID, NewDungeonMapID, NewAreaID, Flags FROM world_map_transforms ORDER BY ID DESC", CONNECTION_SYNCH);

    // ChatChannels.db2
    PrepareStatement(HOTFIX_SEL_CHAT_CHANNELS, "SELECT ID, Flags, Name, Shortcut, FactionGroup FROM chat_channels ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHAT_CHANNELS, "SELECT ID, Name_lang, Shortcut_lang FROM chat_channels_locale WHERE locale = ?", CONNECTION_SYNCH);

    // ChrSpecialization.db2
    PrepareStatement(HOTFIX_SEL_CHR_SPECIALIZATION, "SELECT ID, MasterySpellID1, MasterySpellID2, Unknown2, Unknown3, Name, Name2, Description, "
        "BackgroundFile, SpellIconID, ClassID, OrderIndex, PetTalentType, Role, PrimaryStatOrder2 FROM chr_specialization ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_CHR_SPECIALIZATION, "SELECT ID, Name_lang, Name2_lang, Description_lang, BackgroundFile_lang"
        " FROM chr_specialization_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Emotes.db2
    PrepareStatement(HOTFIX_SEL_EMOTES, "SELECT ID, EmoteSlashCommand, EmoteFlags, SpellVisualKitID, UnkLegion, UnkLegion2, AnimID, EmoteSoundID, "
        "EmoteSpecProc, EmoteSpecProcParam FROM emotes ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_EMOTES, "SELECT ID, EmoteSlashCommand_lang FROM emotes_locale WHERE locale = ?", CONNECTION_SYNCH);

    // GemProperties.db2
    PrepareStatement(HOTFIX_SEL_GEM_PROPERTIES, "SELECT ID, UnkLegion, Type, EnchantID FROM gem_properties ORDER BY ID DESC", CONNECTION_SYNCH);

    // GlyphProperties.db2
    PrepareStatement(HOTFIX_SEL_GLYPH_PROPERTIES, "SELECT ID, SpellId, TypeFlags, SpellIconID, GlyphExclusiveCategoryID FROM glyph_properties"
        " ORDER BY ID DESC", CONNECTION_SYNCH);

    // ItemSet.db2
    PrepareStatement(HOTFIX_SEL_ITEM_SET, "SELECT ID, Name, ItemID1, ItemID2, ItemID3, ItemID4, ItemID5, ItemID6, ItemID7, ItemID8, ItemID9, "
        "ItemID10, ItemID11, ItemID12, ItemID13, ItemID14, ItemID15, ItemID16, ItemID17, UnkLegion, RequiredSkill, RequiredSkillValue FROM item_set"
        " ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_ITEM_SET, "SELECT ID, Name_lang FROM item_set_locale WHERE locale = ?", CONNECTION_SYNCH);

    // Lock.db2
    PrepareStatement(HOTFIX_SEL_LOCK, "SELECT ID, Index1, Index2, Index3, Index4, Index5, Index6, Index7, Index8, Skill1, Skill2, Skill3, Skill4, "
        "Skill5, Skill6, Skill7, Skill8, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Action1, Action2, Action3, Action4, Action5, "
        "Action6, Action7, Action8 FROM `lock` ORDER BY ID DESC", CONNECTION_SYNCH);

    // Movie.db2
    PrepareStatement(HOTFIX_SEL_MOVIE, "SELECT ID, unk1, unk2, unk3, unk4 FROM movie ORDER BY ID DESC", CONNECTION_SYNCH);

    // PowerDisplay.db2
    PrepareStatement(HOTFIX_SEL_POWER_DISPLAY, "SELECT ID, GlobalStringBaseTag, PowerType, Red, Green, Blue FROM power_display ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_POWER_DISPLAY, "SELECT ID, GlobalStringBaseTag_lang FROM power_display_locale WHERE locale = ?", CONNECTION_SYNCH);

    // PvpDifficulty.db2
    PrepareStatement(HOTFIX_SEL_PVP_DIFFICULTY, "SELECT ID, MapID, BracketID, MinLevel, MaxLevel FROM pvp_difficulty ORDER BY ID DESC", CONNECTION_SYNCH);

    // SummonProperties.db2
    PrepareStatement(HOTFIX_SEL_SUMMON_PROPERTIES, "SELECT ID, Category, Faction, Type, Slot, Flags FROM summon_properties ORDER BY ID DESC", CONNECTION_SYNCH);

    // VehicleSeat.db2
    PrepareStatement(HOTFIX_SEL_VEHICLE_SEAT, "SELECT ID, Flags, FlagsB, FlagsC, AttachmentOffsetX, AttachmentOffsetY, AttachmentOffsetZ, "
        "EnterPreDelay, EnterSpeed, EnterGravity, EnterMinDuration, EnterMaxDuration, EnterMinArcHeight, EnterMaxArcHeight, ExitPreDelay, ExitSpeed, "
        "ExitGravity, ExitMinDuration, ExitMaxDuration, ExitMinArcHeight, ExitMaxArcHeight, PassengerYaw, PassengerPitch, PassengerRoll, "
        "VehicleEnterAnimDelay, VehicleExitAnimDelay, CameraEnteringDelay, CameraEnteringDuration, CameraExitingDelay, CameraExitingDuration, "
        "CameraOffsetX, CameraOffsetY, CameraOffsetZ, CameraPosChaseRate, CameraFacingChaseRate, CameraEnteringZoom, CameraSeatZoomMin, "
        "CameraSeatZoomMax, UISkinFileDataID, EnterAnimStart, EnterAnimLoop, RideAnimStart, RideAnimLoop, RideUpperAnimStart, RideUpperAnimLoop, "
        "ExitAnimStart, ExitAnimLoop, ExitAnimEnd, VehicleEnterAnim, VehicleExitAnim, VehicleRideAnimLoop, EnterUISoundID, ExitUISoundID, "
        "EnterAnimKitID, RideAnimKitID, ExitAnimKitID, VehicleEnterAnimBone, VehicleExitAnimBone, VehicleRideAnimLoopBone, CameraModeID, "
        "AttachmentID, PassengerAttachmentID, VehicleAbilityDisplay, VehicleEnterAnimKitID, VehicleRideAnimKitID, VehicleExitAnimKitID"
        " FROM vehicle_seat ORDER BY ID DESC", CONNECTION_SYNCH);

    // Vignette.db2
    PrepareStatement(HOTFIX_SEL_VIGNETTE, "SELECT ID, Name, UnkFloat1, UnkFloat2, QuestUnk, QuestID, Unk1, Unk2 FROM vignette ORDER BY ID DESC", CONNECTION_SYNCH);
    PREPARE_LOCALE_STMT(HOTFIX_SEL_VIGNETTE, "SELECT ID, Name_lang FROM vignette_locale WHERE locale = ?", CONNECTION_SYNCH);
}
