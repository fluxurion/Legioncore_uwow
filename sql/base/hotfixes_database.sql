--
-- Table structure for table `achievement`
--

DROP TABLE IF EXISTS `achievement`;
CREATE TABLE `achievement` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Reward` text,
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ParentAchievement` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CategoryID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderInCategory` smallint(5) unsigned NOT NULL DEFAULT '0',
  `IconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RefAchievement` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CriteriaTree` smallint(5) unsigned NOT NULL DEFAULT '0',
  `FactionFlag` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RewardPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `Amount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `achievement_locale`
--

DROP TABLE IF EXISTS `achievement_locale`;
CREATE TABLE `achievement_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `Reward_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `area_group`
--

DROP TABLE IF EXISTS `area_group`;
CREATE TABLE `area_group` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `area_group_member`
--

DROP TABLE IF EXISTS `area_group_member`;
CREATE TABLE `area_group_member` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `AreaGroupID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `auction_house`
--

DROP TABLE IF EXISTS `auction_house`;
CREATE TABLE `auction_house` (
  `Name` text,
  `FactionID` int(10) unsigned NOT NULL DEFAULT '0',
  `DepositRate` int(10) unsigned NOT NULL DEFAULT '0',
  `ConsignmentRate` int(10) unsigned NOT NULL DEFAULT '0',
  `houseId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `auction_house_locale`
--

DROP TABLE IF EXISTS `auction_house_locale`;
CREATE TABLE `auction_house_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `armor_location`
--

DROP TABLE IF EXISTS `armor_location`;
CREATE TABLE `armor_location` (
  `Value1` float NOT NULL DEFAULT '0',
  `Value2` float NOT NULL DEFAULT '0',
  `Value3` float NOT NULL DEFAULT '0',
  `Value4` float NOT NULL DEFAULT '0',
  `Value5` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Value[5]`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `bank_bag_slot_prices`
--

DROP TABLE IF EXISTS `bank_bag_slot_prices`;
CREATE TABLE `bank_bag_slot_prices` (
  `price` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`price`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `barber_shop_style`
--

DROP TABLE IF EXISTS `barber_shop_style`;
CREATE TABLE `barber_shop_style` (
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `CostMultiplier` float NOT NULL DEFAULT '0',
  `Race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `HairID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Tato` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `barber_shop_style_locale`
--

DROP TABLE IF EXISTS `barber_shop_style_locale`;
CREATE TABLE `barber_shop_style_locale` (
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Type`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `broadcast_text`
--

DROP TABLE IF EXISTS `broadcast_text`;
CREATE TABLE `broadcast_text` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Language` int(11) NOT NULL DEFAULT '0',
  `MaleText` text,
  `FemaleText` text,
  `EmoteID1` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteID2` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteID3` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteDelay1` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteDelay2` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteDelay3` int(10) unsigned NOT NULL DEFAULT '0',
  `SoundID` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkEmoteID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `broadcast_text_locale`
--

DROP TABLE IF EXISTS `broadcast_text_locale`;
CREATE TABLE `broadcast_text_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `MaleText_lang` text,
  `FemaleText_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `curve_point`
--

DROP TABLE IF EXISTS `curve_point`;
CREATE TABLE `curve_point` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurveID` int(10) unsigned NOT NULL DEFAULT '0',
  `Index` int(10) unsigned NOT NULL DEFAULT '0',
  `X` float NOT NULL DEFAULT '0',
  `Y` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `criteria`
--

DROP TABLE IF EXISTS `criteria`;
CREATE TABLE `criteria` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `SkillID` int(10) unsigned NOT NULL DEFAULT '0',
  `AchievementID` int(10) unsigned NOT NULL DEFAULT '0',
  `ZoneID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` int(10) unsigned NOT NULL DEFAULT '0',
  `ManLimit` int(10) unsigned NOT NULL DEFAULT '0',
  `RaidSize` int(10) unsigned NOT NULL DEFAULT '0',
  `EnviromentalDamageType` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ObjectiveID` int(10) unsigned NOT NULL DEFAULT '0',
  `AreaID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `TeamType` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemSlot` int(10) unsigned NOT NULL DEFAULT '0',
  `RollValue` int(10) unsigned NOT NULL DEFAULT '0',
  `ClassID` int(10) unsigned NOT NULL DEFAULT '0',
  `RaceID` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteID` int(10) unsigned NOT NULL DEFAULT '0',
  `GoEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `ScriptDataVal` int(10) unsigned NOT NULL DEFAULT '0',
  `SkillLine` int(10) unsigned NOT NULL DEFAULT '0',
  `AddPet` int(10) unsigned NOT NULL DEFAULT '0',
  `LootType` int(10) unsigned NOT NULL DEFAULT '0',
  `LevelUP` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrBuildingID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrisonLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `CharShipmentContainerID` int(10) unsigned NOT NULL DEFAULT '0',
  `Asset` ERROR TYPE },
  `StartAsset` int(10) unsigned NOT NULL DEFAULT '0',
  `FailAsset` int(10) unsigned NOT NULL DEFAULT '0',
  `StartTimer` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ModifyTree` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StartEvent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FailEvent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EligibilityWorldStateID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EligibilityWorldStateValue` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `criteria_tree`
--

DROP TABLE IF EXISTS `criteria_tree`;
CREATE TABLE `criteria_tree` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Amount` int(10) unsigned NOT NULL DEFAULT '0',
  `DescriptionLang` text,
  `CriteriaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Parent` smallint(5) unsigned NOT NULL DEFAULT '0',
  `flags2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UNK` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `criteria_tree_locale`
--

DROP TABLE IF EXISTS `criteria_tree_locale`;
CREATE TABLE `criteria_tree_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DescriptionLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `character_loadout_item`
--

DROP TABLE IF EXISTS `character_loadout_item`;
CREATE TABLE `character_loadout_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LoadOutID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `char_start_outfit`
--

DROP TABLE IF EXISTS `char_start_outfit`;
CREATE TABLE `char_start_outfit` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID1` int(11) NOT NULL DEFAULT '0',
  `ItemID2` int(11) NOT NULL DEFAULT '0',
  `ItemID3` int(11) NOT NULL DEFAULT '0',
  `ItemID4` int(11) NOT NULL DEFAULT '0',
  `ItemID5` int(11) NOT NULL DEFAULT '0',
  `ItemID6` int(11) NOT NULL DEFAULT '0',
  `ItemID7` int(11) NOT NULL DEFAULT '0',
  `ItemID8` int(11) NOT NULL DEFAULT '0',
  `ItemID9` int(11) NOT NULL DEFAULT '0',
  `ItemID10` int(11) NOT NULL DEFAULT '0',
  `ItemID11` int(11) NOT NULL DEFAULT '0',
  `ItemID12` int(11) NOT NULL DEFAULT '0',
  `ItemID13` int(11) NOT NULL DEFAULT '0',
  `ItemID14` int(11) NOT NULL DEFAULT '0',
  `ItemID15` int(11) NOT NULL DEFAULT '0',
  `ItemID16` int(11) NOT NULL DEFAULT '0',
  `ItemID17` int(11) NOT NULL DEFAULT '0',
  `ItemID18` int(11) NOT NULL DEFAULT '0',
  `ItemID19` int(11) NOT NULL DEFAULT '0',
  `ItemID20` int(11) NOT NULL DEFAULT '0',
  `ItemID21` int(11) NOT NULL DEFAULT '0',
  `ItemID22` int(11) NOT NULL DEFAULT '0',
  `ItemID23` int(11) NOT NULL DEFAULT '0',
  `ItemID24` int(11) NOT NULL DEFAULT '0',
  `PetDisplayID` int(11) NOT NULL DEFAULT '0',
  `RaceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GenderID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OutfitID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PetFamilyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `chr_classes_x_power_types`
--

DROP TABLE IF EXISTS `chr_classes_x_power_types`;
CREATE TABLE `chr_classes_x_power_types` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PowerID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `chr_races`
--

DROP TABLE IF EXISTS `chr_races`;
CREATE TABLE `chr_races` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `ClientPrefix` text,
  `ClientFileString` text,
  `Name` text,
  `NameFemale` text,
  `NameMale` text,
  `FacialHairCustomization1` text,
  `FacialHairCustomization2` text,
  `HairCustomization` text,
  `CreateScreenFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `SelectScreenFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `MaleCustomizeOffset1` float NOT NULL DEFAULT '0',
  `MaleCustomizeOffset2` float NOT NULL DEFAULT '0',
  `MaleCustomizeOffset3` float NOT NULL DEFAULT '0',
  `FemaleCustomizeOffset1` float NOT NULL DEFAULT '0',
  `FemaleCustomizeOffset2` float NOT NULL DEFAULT '0',
  `FemaleCustomizeOffset3` float NOT NULL DEFAULT '0',
  `LowResScreenFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ExplorationSoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaleDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `FemaleDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ResSicknessSpellID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SplashSoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CinematicSequenceID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UAMaleCreatureSoundDataID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UAFemaleCreatureSoundDataID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HighResMaleDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HighResFemaleDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Unk` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseLanguage` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CreatureType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TeamID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RaceRelated` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnalteredVisualRaceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CharComponentTexLayoutHiResID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DefaultClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `NeutralRaceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CharComponentTextureLayoutID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `chr_races_locale`
--

DROP TABLE IF EXISTS `chr_races_locale`;
CREATE TABLE `chr_races_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `ClientPrefix_lang` text,
  `ClientFileString_lang` text,
  `Name_lang` text,
  `NameFemale_lang` text,
  `NameMale_lang` text,
  `FacialHairCustomization1_lang` text,
  `FacialHairCustomization2_lang` text,
  `HairCustomization_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `creature_display_info_extra`
--

DROP TABLE IF EXISTS `creature_display_info_extra`;
CREATE TABLE `creature_display_info_extra` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay1` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay2` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay3` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay4` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay5` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay6` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay7` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay8` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay9` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCItemDisplay10` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayRaceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DisplaySexID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SkinID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FaceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `HairStyleID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `HairColorID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FacialHairID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Tato1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Tato2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Tato3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `creature_display_info`
--

DROP TABLE IF EXISTS `creature_display_info`;
CREATE TABLE `creature_display_info` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ModelID` int(10) unsigned NOT NULL DEFAULT '0',
  `SoundID` int(10) unsigned NOT NULL DEFAULT '0',
  `ExtendedDisplayInfoID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureModelScale` float NOT NULL DEFAULT '0',
  `CreatureModelAlpha` int(10) unsigned NOT NULL DEFAULT '0',
  `TextureVariation1` text,
  `TextureVariation2` text,
  `TextureVariation3` text,
  `PortraitTextureName` text,
  `PortraitCreatureDisplayInfoID` int(10) unsigned NOT NULL DEFAULT '0',
  `SizeClass` int(10) unsigned NOT NULL DEFAULT '0',
  `BloodID` int(10) unsigned NOT NULL DEFAULT '0',
  `NPCSoundID` int(10) unsigned NOT NULL DEFAULT '0',
  `ParticleColorID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureGeosetData` int(10) unsigned NOT NULL DEFAULT '0',
  `ObjectEffectPackageID` int(10) unsigned NOT NULL DEFAULT '0',
  `AnimReplacementSetID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Gender` int(11) NOT NULL DEFAULT '0',
  `StateSpellVisualKitID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `creature_display_info_locale`
--

DROP TABLE IF EXISTS `creature_display_info_locale`;
CREATE TABLE `creature_display_info_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `TextureVariation1_lang` text,
  `TextureVariation2_lang` text,
  `TextureVariation3_lang` text,
  `PortraitTextureName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `creature_type`
--

DROP TABLE IF EXISTS `creature_type`;
CREATE TABLE `creature_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `creature_type_locale`
--

DROP TABLE IF EXISTS `creature_type_locale`;
CREATE TABLE `creature_type_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `destructible_model_data`
--

DROP TABLE IF EXISTS `destructible_model_data`;
CREATE TABLE `destructible_model_data` (
  `StateDamagedDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `StateDestroyedDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `StateRebuildingDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `StateSmokeDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HealEffectSpeed` smallint(5) unsigned NOT NULL DEFAULT '0',
  `StateDamagedImpactEffectDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateDamagedAmbientDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateDamagedNameSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateDestroyedDestructionDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateDestroyedImpactEffectDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateDestroyedAmbientDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateDestroyedNameSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateRebuildingDestructionDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateRebuildingImpactEffectDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateRebuildingAmbientDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateRebuildingNameSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateSmokeInitDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateSmokeAmbientDoodadSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StateSmokeNameSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EjectDirection` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DoNotHighlight` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `HealEffect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`StateDamagedDisplayID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `durability_costs`
--

DROP TABLE IF EXISTS `durability_costs`;
CREATE TABLE `durability_costs` (
  `WeaponSubstructCost1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost9` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost10` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost11` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost12` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost13` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost14` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost15` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost16` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost17` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost18` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost19` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost20` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WeaponSubstructCost21` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ArmorSubstructCost8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`WeaponSubstructCost[21]`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `durability_quality`
--

DROP TABLE IF EXISTS `durability_quality`;
CREATE TABLE `durability_quality` (
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Data`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `game_object_display_info`
--

DROP TABLE IF EXISTS `game_object_display_info`;
CREATE TABLE `game_object_display_info` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `GeoBoxMinX` float NOT NULL DEFAULT '0',
  `GeoBoxMinY` float NOT NULL DEFAULT '0',
  `GeoBoxMinZ` float NOT NULL DEFAULT '0',
  `GeoBoxMaxX` float NOT NULL DEFAULT '0',
  `GeoBoxMaxY` float NOT NULL DEFAULT '0',
  `GeoBoxMaxZ` float NOT NULL DEFAULT '0',
  `OverrideLootEffectScale` float NOT NULL DEFAULT '0',
  `OverrideNameScale` float NOT NULL DEFAULT '0',
  `Sound1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound9` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Sound10` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ObjectEffectPackageID` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `import_price_armor`
--

DROP TABLE IF EXISTS `import_price_armor`;
CREATE TABLE `import_price_armor` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ClothModifier` float NOT NULL DEFAULT '0',
  `LeatherModifier` float NOT NULL DEFAULT '0',
  `ChainModifier` float NOT NULL DEFAULT '0',
  `PlateModifier` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `import_price_quality`
--

DROP TABLE IF EXISTS `import_price_quality`;
CREATE TABLE `import_price_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `import_price_shield`
--

DROP TABLE IF EXISTS `import_price_shield`;
CREATE TABLE `import_price_shield` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `import_price_weapon`
--

DROP TABLE IF EXISTS `import_price_weapon`;
CREATE TABLE `import_price_weapon` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_armor_quality`
--

DROP TABLE IF EXISTS `item_armor_quality`;
CREATE TABLE `item_armor_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `QualityMod1` float NOT NULL DEFAULT '0',
  `QualityMod2` float NOT NULL DEFAULT '0',
  `QualityMod3` float NOT NULL DEFAULT '0',
  `QualityMod4` float NOT NULL DEFAULT '0',
  `QualityMod5` float NOT NULL DEFAULT '0',
  `QualityMod6` float NOT NULL DEFAULT '0',
  `QualityMod7` float NOT NULL DEFAULT '0',
  `ItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_armor_shield`
--

DROP TABLE IF EXISTS `item_armor_shield`;
CREATE TABLE `item_armor_shield` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `QualityMod1` float NOT NULL DEFAULT '0',
  `QualityMod2` float NOT NULL DEFAULT '0',
  `QualityMod3` float NOT NULL DEFAULT '0',
  `QualityMod4` float NOT NULL DEFAULT '0',
  `QualityMod5` float NOT NULL DEFAULT '0',
  `QualityMod6` float NOT NULL DEFAULT '0',
  `QualityMod7` float NOT NULL DEFAULT '0',
  `ItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_armor_total`
--

DROP TABLE IF EXISTS `item_armor_total`;
CREATE TABLE `item_armor_total` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Value1` float NOT NULL DEFAULT '0',
  `Value2` float NOT NULL DEFAULT '0',
  `Value3` float NOT NULL DEFAULT '0',
  `Value4` float NOT NULL DEFAULT '0',
  `ItemLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_damage`
--

DROP TABLE IF EXISTS `item_damage`;
CREATE TABLE `item_damage` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality1` float NOT NULL DEFAULT '0',
  `Quality2` float NOT NULL DEFAULT '0',
  `Quality3` float NOT NULL DEFAULT '0',
  `Quality4` float NOT NULL DEFAULT '0',
  `Quality5` float NOT NULL DEFAULT '0',
  `Quality6` float NOT NULL DEFAULT '0',
  `Quality7` float NOT NULL DEFAULT '0',
  `ItemLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_class`
--

DROP TABLE IF EXISTS `item_class`;
CREATE TABLE `item_class` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Class` int(10) unsigned NOT NULL DEFAULT '0',
  `PriceFactor` float NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_class_locale`
--

DROP TABLE IF EXISTS `item_class_locale`;
CREATE TABLE `item_class_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_disenchant_loot`
--

DROP TABLE IF EXISTS `item_disenchant_loot`;
CREATE TABLE `item_disenchant_loot` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MinLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SkillRequired` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemSubClass` tinyint(4) NOT NULL DEFAULT '0',
  `Quality` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_limit_category`
--

DROP TABLE IF EXISTS `item_limit_category`;
CREATE TABLE `item_limit_category` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `NameLang` text,
  `Quantity` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_limit_category_locale`
--

DROP TABLE IF EXISTS `item_limit_category_locale`;
CREATE TABLE `item_limit_category_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `NameLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_price_base`
--

DROP TABLE IF EXISTS `item_price_base`;
CREATE TABLE `item_price_base` (
  `Armor` float NOT NULL DEFAULT '0',
  `Weapon` float NOT NULL DEFAULT '0',
  `ItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Armor`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_random_properties`
--

DROP TABLE IF EXISTS `item_random_properties`;
CREATE TABLE `item_random_properties` (
  `Name` text,
  `Enchantment1` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment2` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment3` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment4` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment5` int(10) unsigned NOT NULL DEFAULT '0',
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_random_properties_locale`
--

DROP TABLE IF EXISTS `item_random_properties_locale`;
CREATE TABLE `item_random_properties_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_random_suffix`
--

DROP TABLE IF EXISTS `item_random_suffix`;
CREATE TABLE `item_random_suffix` (
  `Name` text,
  `IternalName` text,
  `Enchantment1` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment2` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment3` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment4` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment5` int(10) unsigned NOT NULL DEFAULT '0',
  `AllocationPct1` int(10) unsigned NOT NULL DEFAULT '0',
  `AllocationPct2` int(10) unsigned NOT NULL DEFAULT '0',
  `AllocationPct3` int(10) unsigned NOT NULL DEFAULT '0',
  `AllocationPct4` int(10) unsigned NOT NULL DEFAULT '0',
  `AllocationPct5` int(10) unsigned NOT NULL DEFAULT '0',
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_random_suffix_locale`
--

DROP TABLE IF EXISTS `item_random_suffix_locale`;
CREATE TABLE `item_random_suffix_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `IternalName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_spec_override`
--

DROP TABLE IF EXISTS `item_spec_override`;
CREATE TABLE `item_spec_override` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_spec`
--

DROP TABLE IF EXISTS `item_spec`;
CREATE TABLE `item_spec` (
  `SpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PrimaryStat` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SecondaryStat` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpecID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_set_spell`
--

DROP TABLE IF EXISTS `item_set_spell`;
CREATE TABLE `item_set_spell` (
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemSetID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ChrSpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Threshold` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `guild_perk_spells`
--

DROP TABLE IF EXISTS `guild_perk_spells`;
CREATE TABLE `guild_perk_spells` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `heirloom`
--

DROP TABLE IF EXISTS `heirloom`;
CREATE TABLE `heirloom` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `SourceText` text,
  `Source` int(10) unsigned NOT NULL DEFAULT '0',
  `OldItem1` int(10) unsigned NOT NULL DEFAULT '0',
  `OldItem2` int(10) unsigned NOT NULL DEFAULT '0',
  `NextDifficultyItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `UpgradeItemID1` int(10) unsigned NOT NULL DEFAULT '0',
  `UpgradeItemID2` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemBonusListID1` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemBonusListID2` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `heirloom_locale`
--

DROP TABLE IF EXISTS `heirloom_locale`;
CREATE TABLE `heirloom_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `SourceText_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `holidays`
--

DROP TABLE IF EXISTS `holidays`;
CREATE TABLE `holidays` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration1` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration2` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration3` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration4` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration5` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration6` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration7` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration8` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration9` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration10` int(10) unsigned NOT NULL DEFAULT '0',
  `Date1` int(10) unsigned NOT NULL DEFAULT '0',
  `Date2` int(10) unsigned NOT NULL DEFAULT '0',
  `Date3` int(10) unsigned NOT NULL DEFAULT '0',
  `Date4` int(10) unsigned NOT NULL DEFAULT '0',
  `Date5` int(10) unsigned NOT NULL DEFAULT '0',
  `Date6` int(10) unsigned NOT NULL DEFAULT '0',
  `Date7` int(10) unsigned NOT NULL DEFAULT '0',
  `Date8` int(10) unsigned NOT NULL DEFAULT '0',
  `Date9` int(10) unsigned NOT NULL DEFAULT '0',
  `Date10` int(10) unsigned NOT NULL DEFAULT '0',
  `Date11` int(10) unsigned NOT NULL DEFAULT '0',
  `Date12` int(10) unsigned NOT NULL DEFAULT '0',
  `Date13` int(10) unsigned NOT NULL DEFAULT '0',
  `Date14` int(10) unsigned NOT NULL DEFAULT '0',
  `Date15` int(10) unsigned NOT NULL DEFAULT '0',
  `Date16` int(10) unsigned NOT NULL DEFAULT '0',
  `Region` int(10) unsigned NOT NULL DEFAULT '0',
  `Looping` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags1` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags2` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags3` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags4` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags5` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags6` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags7` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags8` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags9` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFlags10` int(10) unsigned NOT NULL DEFAULT '0',
  `HolidayNameID` int(10) unsigned NOT NULL DEFAULT '0',
  `HolidayDescriptionID` int(10) unsigned NOT NULL DEFAULT '0',
  `TextureFilename` text,
  `Priority` int(10) unsigned NOT NULL DEFAULT '0',
  `CalendarFilterType` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `holidays_locale`
--

DROP TABLE IF EXISTS `holidays_locale`;
CREATE TABLE `holidays_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `TextureFilename_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item`
--

DROP TABLE IF EXISTS `item`;
CREATE TABLE `item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `Class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SubClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SoundOverrideSubclass` tinyint(4) NOT NULL DEFAULT '0',
  `Material` tinyint(4) NOT NULL DEFAULT '0',
  `InventoryType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Sheath` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GroupSoundsID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_appearance`
--

DROP TABLE IF EXISTS `item_appearance`;
CREATE TABLE `item_appearance` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayID` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mail_template`
--

DROP TABLE IF EXISTS `mail_template`;
CREATE TABLE `mail_template` (
  `Content` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Content`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mail_template_locale`
--

DROP TABLE IF EXISTS `mail_template_locale`;
CREATE TABLE `mail_template_locale` (
  `Content` text,
  `locale` varchar(4) NOT NULL,
  `Content_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Content`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `modifier_tree`
--

DROP TABLE IF EXISTS `modifier_tree`;
CREATE TABLE `modifier_tree` (
  `Asset` int(10) unsigned NOT NULL DEFAULT '0',
  `SecondaryAsset` int(10) unsigned NOT NULL DEFAULT '0',
  `Parent` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Operator` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Amount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Asset`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mount_capability`
--

DROP TABLE IF EXISTS `mount_capability`;
CREATE TABLE `mount_capability` (
  `Unknown0` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqSpellKnownID` int(10) unsigned NOT NULL DEFAULT '0',
  `ModSpellAuraID` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqRidingSkill` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqAreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqMapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Unknown7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Unknown0`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mount_type`
--

DROP TABLE IF EXISTS `mount_type`;
CREATE TABLE `mount_type` (
  `MountTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`MountTypeID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mount_type_x_capability`
--

DROP TABLE IF EXISTS `mount_type_x_capability`;
CREATE TABLE `mount_type_x_capability` (
  `MountTypeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MountCapabilityID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`MountTypeID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `name_gen`
--

DROP TABLE IF EXISTS `name_gen`;
CREATE TABLE `name_gen` (
  `Name` text,
  `RaceID` int(10) unsigned NOT NULL DEFAULT '0',
  `Gender` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `name_gen_locale`
--

DROP TABLE IF EXISTS `name_gen_locale`;
CREATE TABLE `name_gen_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_faction_rew`
--

DROP TABLE IF EXISTS `quest_faction_rew`;
CREATE TABLE `quest_faction_rew` (
  `Difficulty1` int(11) NOT NULL DEFAULT '0',
  `Difficulty2` int(11) NOT NULL DEFAULT '0',
  `Difficulty3` int(11) NOT NULL DEFAULT '0',
  `Difficulty4` int(11) NOT NULL DEFAULT '0',
  `Difficulty5` int(11) NOT NULL DEFAULT '0',
  `Difficulty6` int(11) NOT NULL DEFAULT '0',
  `Difficulty7` int(11) NOT NULL DEFAULT '0',
  `Difficulty8` int(11) NOT NULL DEFAULT '0',
  `Difficulty9` int(11) NOT NULL DEFAULT '0',
  `Difficulty10` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Difficulty[10]`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_p_o_i_point`
--

DROP TABLE IF EXISTS `quest_p_o_i_point`;
CREATE TABLE `quest_p_o_i_point` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestPOIBlobID` int(10) unsigned NOT NULL DEFAULT '0',
  `X` smallint(6) NOT NULL DEFAULT '0',
  `Y` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_sort`
--

DROP TABLE IF EXISTS `quest_sort`;
CREATE TABLE `quest_sort` (
  `SortName` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`SortName`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_sort_locale`
--

DROP TABLE IF EXISTS `quest_sort_locale`;
CREATE TABLE `quest_sort_locale` (
  `SortName` text,
  `locale` varchar(4) NOT NULL,
  `SortName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`SortName`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_v2`
--

DROP TABLE IF EXISTS `quest_v2`;
CREATE TABLE `quest_v2` (
  `UniqueBitFlag` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`UniqueBitFlag`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_x_p`
--

DROP TABLE IF EXISTS `quest_x_p`;
CREATE TABLE `quest_x_p` (
  `Difficulty1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty9` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Difficulty10` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Difficulty[10]`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `rand_prop_points`
--

DROP TABLE IF EXISTS `rand_prop_points`;
CREATE TABLE `rand_prop_points` (
  `EpicPropertiesPoints1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EpicPropertiesPoints2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EpicPropertiesPoints3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EpicPropertiesPoints4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EpicPropertiesPoints5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RarePropertiesPoints1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RarePropertiesPoints2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RarePropertiesPoints3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RarePropertiesPoints4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RarePropertiesPoints5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UncommonPropertiesPoints1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UncommonPropertiesPoints2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UncommonPropertiesPoints3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UncommonPropertiesPoints4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UncommonPropertiesPoints5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`EpicPropertiesPoints[5]`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `research_branch`
--

DROP TABLE IF EXISTS `research_branch`;
CREATE TABLE `research_branch` (
  `Name` text,
  `Texture` text,
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ResearchFieldID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `research_branch_locale`
--

DROP TABLE IF EXISTS `research_branch_locale`;
CREATE TABLE `research_branch_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Texture_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `research_project`
--

DROP TABLE IF EXISTS `research_project`;
CREATE TABLE `research_project` (
  `Name` text,
  `Description` text,
  `rare` int(10) unsigned NOT NULL DEFAULT '0',
  `branchId` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `Complexity` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyAmount` int(10) unsigned NOT NULL DEFAULT '0',
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `branchId` ERROR TYPE return,
  `!=` ERROR TYPE branchId,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `research_project_locale`
--

DROP TABLE IF EXISTS `research_project_locale`;
CREATE TABLE `research_project_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `research_site`
--

DROP TABLE IF EXISTS `research_site`;
CREATE TABLE `research_site` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `POIid` int(10) unsigned NOT NULL DEFAULT '0',
  `areaName` text,
  `ID` ERROR TYPE return,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `!=` ERROR TYPE ID,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `research_site_locale`
--

DROP TABLE IF EXISTS `research_site_locale`;
CREATE TABLE `research_site_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `areaName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_bonus`
--

DROP TABLE IF EXISTS `item_bonus`;
CREATE TABLE `item_bonus` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusListID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Value1` int(11) NOT NULL DEFAULT '0',
  `Value2` int(11) NOT NULL DEFAULT '0',
  `Index` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_bonus_tree_node`
--

DROP TABLE IF EXISTS `item_bonus_tree_node`;
CREATE TABLE `item_bonus_tree_node` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeModID` int(10) unsigned NOT NULL DEFAULT '0',
  `SubTreeID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusListID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_currency_cost`
--

DROP TABLE IF EXISTS `item_currency_cost`;
CREATE TABLE `item_currency_cost` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_effect`
--

DROP TABLE IF EXISTS `item_effect`;
CREATE TABLE `item_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `Trigger` int(10) unsigned NOT NULL DEFAULT '0',
  `Charges` int(10) unsigned NOT NULL DEFAULT '0',
  `Cooldown` int(11) NOT NULL DEFAULT '0',
  `Category` int(10) unsigned NOT NULL DEFAULT '0',
  `CategoryCooldown` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_modified_appearance`
--

DROP TABLE IF EXISTS `item_modified_appearance`;
CREATE TABLE `item_modified_appearance` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `AppearanceModID` int(10) unsigned NOT NULL DEFAULT '0',
  `AppearanceID` int(10) unsigned NOT NULL DEFAULT '0',
  `IconFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `Index` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_sparse`
--

DROP TABLE IF EXISTS `item_sparse`;
CREATE TABLE `item_sparse` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk1` float NOT NULL DEFAULT '0',
  `Unk2` float NOT NULL DEFAULT '0',
  `BuyCount` int(10) unsigned NOT NULL DEFAULT '0',
  `BuyPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `SellPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `InventoryType` int(10) unsigned NOT NULL DEFAULT '0',
  `AllowableClass` int(11) NOT NULL DEFAULT '0',
  `AllowableRace` int(11) NOT NULL DEFAULT '0',
  `ItemLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredLevel` int(11) NOT NULL DEFAULT '0',
  `RequiredSkill` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillRank` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredHonorRank` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCityRank` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredReputationFaction` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredReputationRank` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxCount` int(10) unsigned NOT NULL DEFAULT '0',
  `Stackable` int(10) unsigned NOT NULL DEFAULT '0',
  `ContainerSlots` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemStatType` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue` int(11) NOT NULL DEFAULT '0',
  `ItemStatAllocation` int(11) NOT NULL DEFAULT '0',
  `ItemStatSocketCostMultiplier` float NOT NULL DEFAULT '0',
  `ScalingStatDistribution` int(10) unsigned NOT NULL DEFAULT '0',
  `DamageType` int(10) unsigned NOT NULL DEFAULT '0',
  `Delay` int(10) unsigned NOT NULL DEFAULT '0',
  `RangedModRange` float NOT NULL DEFAULT '0',
  `Bonding` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Name2` text,
  `Name3` text,
  `Name4` text,
  `Description` text,
  `PageText` int(10) unsigned NOT NULL DEFAULT '0',
  `LanguageID` int(10) unsigned NOT NULL DEFAULT '0',
  `PageMaterial` int(10) unsigned NOT NULL DEFAULT '0',
  `StartQuest` int(10) unsigned NOT NULL DEFAULT '0',
  `LockID` int(10) unsigned NOT NULL DEFAULT '0',
  `Material` int(11) NOT NULL DEFAULT '0',
  `Sheath` int(10) unsigned NOT NULL DEFAULT '0',
  `RandomProperty` int(10) unsigned NOT NULL DEFAULT '0',
  `RandomSuffix` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemSet` int(10) unsigned NOT NULL DEFAULT '0',
  `Area` int(10) unsigned NOT NULL DEFAULT '0',
  `Map` int(10) unsigned NOT NULL DEFAULT '0',
  `BagFamily` int(10) unsigned NOT NULL DEFAULT '0',
  `TotemCategory` int(10) unsigned NOT NULL DEFAULT '0',
  `SocketColor` int(10) unsigned NOT NULL DEFAULT '0',
  `SocketBonus` int(10) unsigned NOT NULL DEFAULT '0',
  `GemProperties` int(10) unsigned NOT NULL DEFAULT '0',
  `ArmorDamageModifier` float NOT NULL DEFAULT '0',
  `Duration` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemLimitCategory` int(10) unsigned NOT NULL DEFAULT '0',
  `HolidayID` int(10) unsigned NOT NULL DEFAULT '0',
  `StatScalingFactor` float NOT NULL DEFAULT '0',
  `CurrencySubstitutionID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencySubstitutionCount` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemNameDescriptionID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_sparse_locale`
--

DROP TABLE IF EXISTS `item_sparse_locale`;
CREATE TABLE `item_sparse_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Name2_lang` text,
  `Name3_lang` text,
  `Name4_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_extended_cost`
--

DROP TABLE IF EXISTS `item_extended_cost`;
CREATE TABLE `item_extended_cost` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredArenaSlot` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem5` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount5` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredPersonalArenaRating` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemPurchaseGroup` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency5` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount5` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredFactionId` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredFactionStanding` int(10) unsigned NOT NULL DEFAULT '0',
  `RequirementFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredAchievement` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `0` ERROR TYPE return,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_ability`
--

DROP TABLE IF EXISTS `battle_pet_ability`;
CREATE TABLE `battle_pet_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `fileDataEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `turnCooldown` int(10) unsigned NOT NULL DEFAULT '0',
  `auraAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `auraDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `name` text,
  `description` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_ability_locale`
--

DROP TABLE IF EXISTS `battle_pet_ability_locale`;
CREATE TABLE `battle_pet_ability_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `name_lang` text,
  `description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_ability_effect`
--

DROP TABLE IF EXISTS `battle_pet_ability_effect`;
CREATE TABLE `battle_pet_ability_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `TurnEntryID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `AuraId_unk` int(10) unsigned NOT NULL DEFAULT '0',
  `propertiesID` int(10) unsigned NOT NULL DEFAULT '0',
  `effectIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyValues1` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyValues2` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyValues3` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyValues4` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyValues5` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyValues6` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_effect_properties`
--

DROP TABLE IF EXISTS `battle_pet_effect_properties`;
CREATE TABLE `battle_pet_effect_properties` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `someFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `propertyDescs1` text,
  `propertyDescs2` text,
  `propertyDescs3` text,
  `propertyDescs4` text,
  `propertyDescs5` text,
  `propertyDescs6` text,
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `unk5` int(10) unsigned NOT NULL DEFAULT '0',
  `unk6` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_effect_properties_locale`
--

DROP TABLE IF EXISTS `battle_pet_effect_properties_locale`;
CREATE TABLE `battle_pet_effect_properties_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `propertyDescs1_lang` text,
  `propertyDescs2_lang` text,
  `propertyDescs3_lang` text,
  `propertyDescs4_lang` text,
  `propertyDescs5_lang` text,
  `propertyDescs6_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_ability_turn`
--

DROP TABLE IF EXISTS `battle_pet_ability_turn`;
CREATE TABLE `battle_pet_ability_turn` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `AbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateID` int(10) unsigned NOT NULL DEFAULT '0',
  `turnIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `procIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_ability_state`
--

DROP TABLE IF EXISTS `battle_pet_ability_state`;
CREATE TABLE `battle_pet_ability_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `AbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_state`
--

DROP TABLE IF EXISTS `battle_pet_state`;
CREATE TABLE `battle_pet_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `stateName` text,
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_state_locale`
--

DROP TABLE IF EXISTS `battle_pet_state_locale`;
CREATE TABLE `battle_pet_state_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `stateName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_species`
--

DROP TABLE IF EXISTS `battle_pet_species`;
CREATE TABLE `battle_pet_species` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `IconFileID` int(10) unsigned NOT NULL DEFAULT '0',
  `spellId` int(10) unsigned NOT NULL DEFAULT '0',
  `petType` int(10) unsigned NOT NULL DEFAULT '0',
  `source` int(11) NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `SourceText` text,
  `Description` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_species_locale`
--

DROP TABLE IF EXISTS `battle_pet_species_locale`;
CREATE TABLE `battle_pet_species_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `SourceText_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_species_state`
--

DROP TABLE IF EXISTS `battle_pet_species_state`;
CREATE TABLE `battle_pet_species_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpeciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `State` int(10) unsigned NOT NULL DEFAULT '0',
  `Value` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_species_x_ability`
--

DROP TABLE IF EXISTS `battle_pet_species_x_ability`;
CREATE TABLE `battle_pet_species_x_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `speciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `abilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_breed_quality`
--

DROP TABLE IF EXISTS `battle_pet_breed_quality`;
CREATE TABLE `battle_pet_breed_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `quality` int(10) unsigned NOT NULL DEFAULT '0',
  `qualityModifier` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `battle_pet_breed_state`
--

DROP TABLE IF EXISTS `battle_pet_breed_state`;
CREATE TABLE `battle_pet_breed_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `breedID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateModifier` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_ability`
--

DROP TABLE IF EXISTS `garr_ability`;
CREATE TABLE `garr_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `IconFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `OtherFactionGarrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrAbilityCategoryID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_ability_locale`
--

DROP TABLE IF EXISTS `garr_ability_locale`;
CREATE TABLE `garr_ability_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_ability_effect`
--

DROP TABLE IF EXISTS `garr_ability_effect`;
CREATE TABLE `garr_ability_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `Amount` float NOT NULL DEFAULT '0',
  `Amount2` float NOT NULL DEFAULT '0',
  `Amount3` float NOT NULL DEFAULT '0',
  `Unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk5` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_building`
--

DROP TABLE IF EXISTS `garr_building`;
CREATE TABLE `garr_building` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeGameObjectID` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceGameObjectID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(10) unsigned NOT NULL DEFAULT '0',
  `NameAlliance` text,
  `NameHorde` text,
  `Description` text,
  `Tooltip` text,
  `BuildDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `CostCurrencyID` int(10) unsigned NOT NULL DEFAULT '0',
  `CostCurrencyAmount` int(11) NOT NULL DEFAULT '0',
  `HordeTexPrefixKitID` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceTexPrefixKitID` int(10) unsigned NOT NULL DEFAULT '0',
  `IconFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusAmount` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceActivationScenePackageID` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeActivationScenePackageID` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxShipments` int(10) unsigned NOT NULL DEFAULT '0',
  `FollowerRequiredGarrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `FollowerGarrAbilityEffectID` int(10) unsigned NOT NULL DEFAULT '0',
  `CostMoney` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_building_locale`
--

DROP TABLE IF EXISTS `garr_building_locale`;
CREATE TABLE `garr_building_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `NameAlliance_lang` text,
  `NameHorde_lang` text,
  `Description_lang` text,
  `Tooltip_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_building_plot_inst`
--

DROP TABLE IF EXISTS `garr_building_plot_inst`;
CREATE TABLE `garr_building_plot_inst` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrBuildingID` int(10) unsigned NOT NULL DEFAULT '0',
  `UiTextureAtlasMemberID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrSiteLevelPlotInstID` int(10) unsigned NOT NULL DEFAULT '0',
  `LandmarkOffsetX` float NOT NULL DEFAULT '0',
  `LandmarkOffsetY` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_class_spec`
--

DROP TABLE IF EXISTS `garr_class_spec`;
CREATE TABLE `garr_class_spec` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `NameMale` text,
  `NameFemale` text,
  `NameGenderless` text,
  `ClassAtlasID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrFollItemSetID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_class_spec_locale`
--

DROP TABLE IF EXISTS `garr_class_spec_locale`;
CREATE TABLE `garr_class_spec_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `NameMale_lang` text,
  `NameFemale_lang` text,
  `NameGenderless_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_follower`
--

DROP TABLE IF EXISTS `garr_follower`;
CREATE TABLE `garr_follower` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeCreatureID` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceCreatureID` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeUiAnimRaceInfoID` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceUiAnimRaceInfoID` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeGarrClassSpecID` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceGarrClassSpecID` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeGarrFollItemSetID` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceGarrFollItemSetID` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemLevelWeapon` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemLevelArmor` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown1` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeSourceText` text,
  `AllianceSourceText` text,
  `Unknown2` int(11) NOT NULL DEFAULT '0',
  `Unknown3` int(11) NOT NULL DEFAULT '0',
  `HordePortraitIconID` int(10) unsigned NOT NULL DEFAULT '0',
  `AlliancePortraitIconID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_follower_locale`
--

DROP TABLE IF EXISTS `garr_follower_locale`;
CREATE TABLE `garr_follower_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `HordeSourceText_lang` text,
  `AllianceSourceText_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_follower_level_x_p`
--

DROP TABLE IF EXISTS `garr_follower_level_x_p`;
CREATE TABLE `garr_follower_level_x_p` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(10) unsigned NOT NULL DEFAULT '0',
  `NextLevelXP` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_follower_quality`
--

DROP TABLE IF EXISTS `garr_follower_quality`;
CREATE TABLE `garr_follower_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality` int(10) unsigned NOT NULL DEFAULT '0',
  `NextQualityXP` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_follower_x_ability`
--

DROP TABLE IF EXISTS `garr_follower_x_ability`;
CREATE TABLE `garr_follower_x_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrFollowerID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_encounter`
--

DROP TABLE IF EXISTS `garr_encounter`;
CREATE TABLE `garr_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Mod` float NOT NULL DEFAULT '0',
  `Mod1` float NOT NULL DEFAULT '0',
  `SomeFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_encounter_locale`
--

DROP TABLE IF EXISTS `garr_encounter_locale`;
CREATE TABLE `garr_encounter_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_encounter_x_mechanic`
--

DROP TABLE IF EXISTS `garr_encounter_x_mechanic`;
CREATE TABLE `garr_encounter_x_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrEncounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mechanic`
--

DROP TABLE IF EXISTS `garr_mechanic`;
CREATE TABLE `garr_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Mod` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mechanic_type`
--

DROP TABLE IF EXISTS `garr_mechanic_type`;
CREATE TABLE `garr_mechanic_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Category` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `SomeFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mechanic_type_locale`
--

DROP TABLE IF EXISTS `garr_mechanic_type_locale`;
CREATE TABLE `garr_mechanic_type_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mission`
--

DROP TABLE IF EXISTS `garr_mission`;
CREATE TABLE `garr_mission` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqFollowersItemLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqFollowersCount` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `MissionDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `OfferDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMissionTypeID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `Location` text,
  `CriteriaID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqResourcesCount` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseXP` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseChance` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mission_locale`
--

DROP TABLE IF EXISTS `garr_mission_locale`;
CREATE TABLE `garr_mission_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `Location_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mission_reward`
--

DROP TABLE IF EXISTS `garr_mission_reward`;
CREATE TABLE `garr_mission_reward` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `RewardXP` int(10) unsigned NOT NULL DEFAULT '0',
  `RewardItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemAmount` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyValue` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_mission_x_encounter`
--

DROP TABLE IF EXISTS `garr_mission_x_encounter`;
CREATE TABLE `garr_mission_x_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrEncounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_plot`
--

DROP TABLE IF EXISTS `garr_plot`;
CREATE TABLE `garr_plot` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrPlotUICategoryID` int(10) unsigned NOT NULL DEFAULT '0',
  `PlotType` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `MinCount` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxCount` int(10) unsigned NOT NULL DEFAULT '0',
  `AllianceConstructionGameObjectID` int(10) unsigned NOT NULL DEFAULT '0',
  `HordeConstructionGameObjectID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_plot_locale`
--

DROP TABLE IF EXISTS `garr_plot_locale`;
CREATE TABLE `garr_plot_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_plot_building`
--

DROP TABLE IF EXISTS `garr_plot_building`;
CREATE TABLE `garr_plot_building` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrPlotID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrBuildingID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_plot_instance`
--

DROP TABLE IF EXISTS `garr_plot_instance`;
CREATE TABLE `garr_plot_instance` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrPlotID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_plot_instance_locale`
--

DROP TABLE IF EXISTS `garr_plot_instance_locale`;
CREATE TABLE `garr_plot_instance_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_site_level`
--

DROP TABLE IF EXISTS `garr_site_level`;
CREATE TABLE `garr_site_level` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `SiteID` int(10) unsigned NOT NULL DEFAULT '0',
  `UITextureKitID` int(10) unsigned NOT NULL DEFAULT '0',
  `TownHallX` float NOT NULL DEFAULT '0',
  `TownHallY` float NOT NULL DEFAULT '0',
  `MovieID` int(10) unsigned NOT NULL DEFAULT '0',
  `Level2` int(10) unsigned NOT NULL DEFAULT '0',
  `UpgradeResourceCost` int(11) NOT NULL DEFAULT '0',
  `UpgradeMoneyCost` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `garr_site_level_plot_inst`
--

DROP TABLE IF EXISTS `garr_site_level_plot_inst`;
CREATE TABLE `garr_site_level_plot_inst` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrSiteLevelID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrPlotInstanceID` int(10) unsigned NOT NULL DEFAULT '0',
  `LandmarkX` float NOT NULL DEFAULT '0',
  `LandmarkY` float NOT NULL DEFAULT '0',
  `Unknown` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `creature_difficulty`
--

DROP TABLE IF EXISTS `creature_difficulty`;
CREATE TABLE `creature_difficulty` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags1` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags2` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags3` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags4` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags5` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Expansion` tinyint(4) NOT NULL DEFAULT '0',
  `MinLevel` tinyint(4) NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `currency_types`
--

DROP TABLE IF EXISTS `currency_types`;
CREATE TABLE `currency_types` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CategoryID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `InventoryIcon1` text,
  `InventoryIcon2` text,
  `SpellWeight` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellCategory` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxQty` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxEarnablePerWeek` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality` int(10) unsigned NOT NULL DEFAULT '0',
  `Description` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `currency_types_locale`
--

DROP TABLE IF EXISTS `currency_types_locale`;
CREATE TABLE `currency_types_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `InventoryIcon1_lang` text,
  `InventoryIcon2_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `quest_package_item`
--

DROP TABLE IF EXISTS `quest_package_item`;
CREATE TABLE `quest_package_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestPackageID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemCount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FilterType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mount`
--

DROP TABLE IF EXISTS `mount`;
CREATE TABLE `mount` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `MountTypeId` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayId` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `SourceDescription` text,
  `Source` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `PlayerConditionId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `mount_locale`
--

DROP TABLE IF EXISTS `mount_locale`;
CREATE TABLE `mount_locale` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `SourceDescription_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `language_words`
--

DROP TABLE IF EXISTS `language_words`;
CREATE TABLE `language_words` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `langId` int(10) unsigned NOT NULL DEFAULT '0',
  `word` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `language_words_locale`
--

DROP TABLE IF EXISTS `language_words_locale`;
CREATE TABLE `language_words_locale` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `word_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `key_chain`
--

DROP TABLE IF EXISTS `key_chain`;
CREATE TABLE `key_chain` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `Key1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key6` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key8` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key9` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key10` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key11` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key12` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key13` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key14` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key15` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key16` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key17` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key18` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key19` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key20` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key21` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key22` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key23` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key24` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key25` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key26` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key27` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key28` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key29` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key30` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key31` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Key32` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `override_spell_data`
--

DROP TABLE IF EXISTS `override_spell_data`;
CREATE TABLE `override_spell_data` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID1` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID2` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID3` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID4` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID5` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID6` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID7` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID8` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID9` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID10` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `PlayerActionbarFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `phase_group`
--

DROP TABLE IF EXISTS `phase_group`;
CREATE TABLE `phase_group` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PhaseID` int(10) unsigned NOT NULL DEFAULT '0',
  `PhaseGroupID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `sound_entries`
--

DROP TABLE IF EXISTS `sound_entries`;
CREATE TABLE `sound_entries` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SoundType` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `FileDataID1` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID2` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID3` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID4` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID5` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID6` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID7` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID8` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID9` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID10` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID11` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID12` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID13` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID14` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID15` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID16` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID17` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID18` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID19` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID20` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq1` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq2` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq3` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq4` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq5` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq6` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq7` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq8` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq9` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq10` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq11` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq12` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq13` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq14` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq15` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq16` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq17` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq18` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq19` int(10) unsigned NOT NULL DEFAULT '0',
  `Freq20` int(10) unsigned NOT NULL DEFAULT '0',
  `VolumeFloat` float NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `MinDistance` float NOT NULL DEFAULT '0',
  `DistanceCutoff` float NOT NULL DEFAULT '0',
  `EAXDef` int(10) unsigned NOT NULL DEFAULT '0',
  `SoundEntriesAdvancedID` int(10) unsigned NOT NULL DEFAULT '0',
  `VolumeVariationPlus` float NOT NULL DEFAULT '0',
  `VolumeVariationMinus` float NOT NULL DEFAULT '0',
  `PitchVariationPlus` float NOT NULL DEFAULT '0',
  `PitchVariationMinus` float NOT NULL DEFAULT '0',
  `PitchAdjust` float NOT NULL DEFAULT '0',
  `DialogType` int(10) unsigned NOT NULL DEFAULT '0',
  `BusOverwriteID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `sound_entries_locale`
--

DROP TABLE IF EXISTS `sound_entries_locale`;
CREATE TABLE `sound_entries_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_aura_restrictions`
--

DROP TABLE IF EXISTS `spell_aura_restrictions`;
CREATE TABLE `spell_aura_restrictions` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `CasterAuraSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `TargetAuraSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `ExcludeCasterAuraSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `ExcludeTargetAuraSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CasterAuraState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TargetAuraState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ExcludeCasterAuraState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ExcludeTargetAuraState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_casting_requirements`
--

DROP TABLE IF EXISTS `spell_casting_requirements`;
CREATE TABLE `spell_casting_requirements` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `MinFactionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredAreasID` smallint(6) NOT NULL DEFAULT '0',
  `RequiresSpellFocus` smallint(5) unsigned NOT NULL DEFAULT '0',
  `FacingCasterFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinReputation` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredAuraVision` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_class_options`
--

DROP TABLE IF EXISTS `spell_class_options`;
CREATE TABLE `spell_class_options` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellClassMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellClassMask2` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellClassMask3` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellClassMask4` int(10) unsigned NOT NULL DEFAULT '0',
  `ModalNextSpell` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpellClassSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_learn_spell`
--

DROP TABLE IF EXISTS `spell_learn_spell`;
CREATE TABLE `spell_learn_spell` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LearnSpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `OverridesSpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_misc`
--

DROP TABLE IF EXISTS `spell_misc`;
CREATE TABLE `spell_misc` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes1` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes2` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes3` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes4` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes5` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes6` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes7` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes8` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes9` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes10` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes11` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes12` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes13` int(10) unsigned NOT NULL DEFAULT '0',
  `Attributes14` int(10) unsigned NOT NULL DEFAULT '0',
  `Speed` float NOT NULL DEFAULT '0',
  `MultistrikeSpeedMod` float NOT NULL DEFAULT '0',
  `CastingTimeIndex` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DurationIndex` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RangeIndex` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpellIconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ActiveIconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SchoolMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_power`
--

DROP TABLE IF EXISTS `spell_power`;
CREATE TABLE `spell_power` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `PowerCost` int(10) unsigned NOT NULL DEFAULT '0',
  `PowerCostPercentage` float NOT NULL DEFAULT '0',
  `PowerCostPercentagePerSecond` float NOT NULL DEFAULT '0',
  `RequiredAura` int(10) unsigned NOT NULL DEFAULT '0',
  `HealthCostPercentage` float NOT NULL DEFAULT '0',
  `PowerCostPerSecond` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ManaCostAdditional` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PowerDisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UnitPowerBarID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PowerIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PowerType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PowerCostPerLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_reagents`
--

DROP TABLE IF EXISTS `spell_reagents`;
CREATE TABLE `spell_reagents` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `Reagent1` int(11) NOT NULL DEFAULT '0',
  `Reagent2` int(11) NOT NULL DEFAULT '0',
  `Reagent3` int(11) NOT NULL DEFAULT '0',
  `Reagent4` int(11) NOT NULL DEFAULT '0',
  `Reagent5` int(11) NOT NULL DEFAULT '0',
  `Reagent6` int(11) NOT NULL DEFAULT '0',
  `Reagent7` int(11) NOT NULL DEFAULT '0',
  `Reagent8` int(11) NOT NULL DEFAULT '0',
  `ReagentCount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReagentCount8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_reagents_currency`
--

DROP TABLE IF EXISTS `spell_reagents_currency`;
CREATE TABLE `spell_reagents_currency` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CurrencyCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_rune_cost`
--

DROP TABLE IF EXISTS `spell_rune_cost`;
CREATE TABLE `spell_rune_cost` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `RuneCost1` int(10) unsigned NOT NULL DEFAULT '0',
  `RuneCost2` int(10) unsigned NOT NULL DEFAULT '0',
  `RuneCost3` int(10) unsigned NOT NULL DEFAULT '0',
  `RuneCost4` int(10) unsigned NOT NULL DEFAULT '0',
  `RunePowerGain` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `toy`
--

DROP TABLE IF EXISTS `toy`;
CREATE TABLE `toy` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Description` text,
  `CategoryFilter` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `toy_locale`
--

DROP TABLE IF EXISTS `toy_locale`;
CREATE TABLE `toy_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `pvp_item`
--

DROP TABLE IF EXISTS `pvp_item`;
CREATE TABLE `pvp_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusIlvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_totems`
--

DROP TABLE IF EXISTS `spell_totems`;
CREATE TABLE `spell_totems` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `TotemCategory1` int(10) unsigned NOT NULL DEFAULT '0',
  `TotemCategory2` int(10) unsigned NOT NULL DEFAULT '0',
  `Totem1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Totem2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_x_spell_visual`
--

DROP TABLE IF EXISTS `spell_x_spell_visual`;
CREATE TABLE `spell_x_spell_visual` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `TravelSpeed` float NOT NULL DEFAULT '0',
  `SpellVisualID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpellVisualID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PlayerConditionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `taxi_nodes`
--

DROP TABLE IF EXISTS `taxi_nodes`;
CREATE TABLE `taxi_nodes` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `PosZ` float NOT NULL DEFAULT '0',
  `Name` text,
  `MountCreatureID` int(10) unsigned NOT NULL DEFAULT '0',
  `ConditionID` int(10) unsigned NOT NULL DEFAULT '0',
  `LearnableIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `MapOffset` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `taxi_nodes_locale`
--

DROP TABLE IF EXISTS `taxi_nodes_locale`;
CREATE TABLE `taxi_nodes_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `taxi_path`
--

DROP TABLE IF EXISTS `taxi_path`;
CREATE TABLE `taxi_path` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `From` int(10) unsigned NOT NULL DEFAULT '0',
  `To` int(10) unsigned NOT NULL DEFAULT '0',
  `Cost` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `taxi_path_node`
--

DROP TABLE IF EXISTS `taxi_path_node`;
CREATE TABLE `taxi_path_node` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PathID` int(10) unsigned NOT NULL DEFAULT '0',
  `NodeIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `LocX` float NOT NULL DEFAULT '0',
  `LocY` float NOT NULL DEFAULT '0',
  `LocZ` float NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Delay` int(10) unsigned NOT NULL DEFAULT '0',
  `ArrivalEventID` int(10) unsigned NOT NULL DEFAULT '0',
  `DepartureEventID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_upgrade`
--

DROP TABLE IF EXISTS `item_upgrade`;
CREATE TABLE `item_upgrade` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `itemUpgradePathId` int(10) unsigned NOT NULL DEFAULT '0',
  `levelBonus` int(10) unsigned NOT NULL DEFAULT '0',
  `prevUpgradeId` int(10) unsigned NOT NULL DEFAULT '0',
  `currencyReqId` int(10) unsigned NOT NULL DEFAULT '0',
  `currencyReqAmt` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `rule_set_item_upgrade`
--

DROP TABLE IF EXISTS `rule_set_item_upgrade`;
CREATE TABLE `rule_set_item_upgrade` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `levelUpd` int(10) unsigned NOT NULL DEFAULT '0',
  `startUpgrade` int(10) unsigned NOT NULL DEFAULT '0',
  `itemEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `game_objects`
--

DROP TABLE IF EXISTS `game_objects`;
CREATE TABLE `game_objects` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayID` int(10) unsigned NOT NULL DEFAULT '0',
  `PositionX` float NOT NULL DEFAULT '0',
  `PositionY` float NOT NULL DEFAULT '0',
  `PositionZ` float NOT NULL DEFAULT '0',
  `RotationX` float NOT NULL DEFAULT '0',
  `RotationY` float NOT NULL DEFAULT '0',
  `RotationZ` float NOT NULL DEFAULT '0',
  `RotationW` float NOT NULL DEFAULT '0',
  `Size` float NOT NULL DEFAULT '0',
  `PhaseUseFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `PhaseID` int(10) unsigned NOT NULL DEFAULT '0',
  `PhaseGroupID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Data1` int(10) unsigned NOT NULL DEFAULT '0',
  `Data2` int(10) unsigned NOT NULL DEFAULT '0',
  `Data3` int(10) unsigned NOT NULL DEFAULT '0',
  `Data4` int(10) unsigned NOT NULL DEFAULT '0',
  `Data5` int(10) unsigned NOT NULL DEFAULT '0',
  `Data6` int(10) unsigned NOT NULL DEFAULT '0',
  `Data7` int(10) unsigned NOT NULL DEFAULT '0',
  `Data8` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `game_objects_locale`
--

DROP TABLE IF EXISTS `game_objects_locale`;
CREATE TABLE `game_objects_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `game_tables`
--

DROP TABLE IF EXISTS `game_tables`;
CREATE TABLE `game_tables` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `NumRows` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NumColumns` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `game_tables_locale`
--

DROP TABLE IF EXISTS `game_tables_locale`;
CREATE TABLE `game_tables_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `map_challenge_mode`
--

DROP TABLE IF EXISTS `map_challenge_mode`;
CREATE TABLE `map_challenge_mode` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `map` int(10) unsigned NOT NULL DEFAULT '0',
  `unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `season` int(10) unsigned NOT NULL DEFAULT '0',
  `bronze` int(10) unsigned NOT NULL DEFAULT '0',
  `silver` int(10) unsigned NOT NULL DEFAULT '0',
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_visual`
--

DROP TABLE IF EXISTS `spell_visual`;
CREATE TABLE `spell_visual` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  `unk5` int(10) unsigned NOT NULL DEFAULT '0',
  `unk6` int(10) unsigned NOT NULL DEFAULT '0',
  `unk7` int(10) unsigned NOT NULL DEFAULT '0',
  `unk8` int(10) unsigned NOT NULL DEFAULT '0',
  `unk9` int(10) unsigned NOT NULL DEFAULT '0',
  `unk10` int(10) unsigned NOT NULL DEFAULT '0',
  `unk11` int(10) unsigned NOT NULL DEFAULT '0',
  `unk12` int(10) unsigned NOT NULL DEFAULT '0',
  `unk13` int(10) unsigned NOT NULL DEFAULT '0',
  `unk14` int(10) unsigned NOT NULL DEFAULT '0',
  `unk15` int(10) unsigned NOT NULL DEFAULT '0',
  `unk16` int(10) unsigned NOT NULL DEFAULT '0',
  `unk17` int(10) unsigned NOT NULL DEFAULT '0',
  `unk18` int(10) unsigned NOT NULL DEFAULT '0',
  `unk19` float NOT NULL DEFAULT '0',
  `unk20` float NOT NULL DEFAULT '0',
  `unk21` float NOT NULL DEFAULT '0',
  `unk22` float NOT NULL DEFAULT '0',
  `unk23` float NOT NULL DEFAULT '0',
  `unk24` float NOT NULL DEFAULT '0',
  `hostileId` int(10) unsigned NOT NULL DEFAULT '0',
  `unk25` int(10) unsigned NOT NULL DEFAULT '0',
  `unk26` int(10) unsigned NOT NULL DEFAULT '0',
  `unk27` int(10) unsigned NOT NULL DEFAULT '0',
  `unk28` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_x_bonus_tree`
--

DROP TABLE IF EXISTS `item_x_bonus_tree`;
CREATE TABLE `item_x_bonus_tree` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_effect`
--

DROP TABLE IF EXISTS `spell_effect`;
CREATE TABLE `spell_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectAmplitude` float NOT NULL DEFAULT '0',
  `EffectAuraPeriod` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectBasePoints` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectBonusCoefficient` float NOT NULL DEFAULT '0',
  `EffectChainAmplitude` float NOT NULL DEFAULT '0',
  `EffectDieSides` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectItemType` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectMiscValue1` int(11) NOT NULL DEFAULT '0',
  `EffectMiscValue2` int(11) NOT NULL DEFAULT '0',
  `EffectPointsPerResource` float NOT NULL DEFAULT '0',
  `EffectRealPointsPerLevel` float NOT NULL DEFAULT '0',
  `EffectSpellClassMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectSpellClassMask2` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectSpellClassMask3` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectSpellClassMask4` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectTriggerSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectPosFacing` float NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectAttributes` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusCoefficientFromAP` float NOT NULL DEFAULT '0',
  `EffectAura` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EffectChainTargets` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(4) NOT NULL DEFAULT '0',
  `Effect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EffectMechanic` tinyint(4) NOT NULL DEFAULT '0',
  `EffectRadiusIndex1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EffectRadiusIndex2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ImplicitTarget1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ImplicitTarget2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EffectIndex` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_shapeshift_form`
--

DROP TABLE IF EXISTS `spell_shapeshift_form`;
CREATE TABLE `spell_shapeshift_form` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `UnkLegion` float NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `AttackIconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CombatRoundTime` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureDisplayID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureDisplayID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureDisplayID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureDisplayID4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureType` tinyint(4) NOT NULL DEFAULT '0',
  `MountTypeID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ExitSoundEntriesID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_shapeshift_form_locale`
--

DROP TABLE IF EXISTS `spell_shapeshift_form_locale`;
CREATE TABLE `spell_shapeshift_form_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_shapeshift`
--

DROP TABLE IF EXISTS `spell_shapeshift`;
CREATE TABLE `spell_shapeshift` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ShapeshiftExclude1` int(10) unsigned NOT NULL DEFAULT '0',
  `ShapeshiftExclude2` int(10) unsigned NOT NULL DEFAULT '0',
  `ShapeshiftMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `ShapeshiftMask2` int(10) unsigned NOT NULL DEFAULT '0',
  `StanceBarOrder` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_target_restrictions`
--

DROP TABLE IF EXISTS `spell_target_restrictions`;
CREATE TABLE `spell_target_restrictions` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ConeAngle` float NOT NULL DEFAULT '0',
  `Width` float NOT NULL DEFAULT '0',
  `Targets` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxTargetLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TargetCreatureType` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxAffectedTargets` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell`
--

DROP TABLE IF EXISTS `spell`;
CREATE TABLE `spell` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `NameSubtext` text,
  `Description` text,
  `AuraDescription` text,
  `SpellMiscDifficultyID` int(10) unsigned NOT NULL DEFAULT '0',
  `DescriptionVariablesID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_locale`
--

DROP TABLE IF EXISTS `spell_locale`;
CREATE TABLE `spell_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `NameSubtext_lang` text,
  `Description_lang` text,
  `AuraDescription_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_scaling`
--

DROP TABLE IF EXISTS `spell_scaling`;
CREATE TABLE `spell_scaling` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ScalesFromItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ScalingClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxScalingLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_range`
--

DROP TABLE IF EXISTS `spell_range`;
CREATE TABLE `spell_range` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MinRangeHostile` float NOT NULL DEFAULT '0',
  `MinRangeFriend` float NOT NULL DEFAULT '0',
  `MaxRangeHostile` float NOT NULL DEFAULT '0',
  `MaxRangeFriend` float NOT NULL DEFAULT '0',
  `DisplayName` text,
  `DisplayNameShort` text,
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_range_locale`
--

DROP TABLE IF EXISTS `spell_range_locale`;
CREATE TABLE `spell_range_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DisplayName_lang` text,
  `DisplayNameShort_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_radius`
--

DROP TABLE IF EXISTS `spell_radius`;
CREATE TABLE `spell_radius` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Radius` float NOT NULL DEFAULT '0',
  `RadiusPerLevel` float NOT NULL DEFAULT '0',
  `RadiusMin` float NOT NULL DEFAULT '0',
  `RadiusMax` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_procs_per_minute`
--

DROP TABLE IF EXISTS `spell_procs_per_minute`;
CREATE TABLE `spell_procs_per_minute` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseProcRate` float NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_procs_per_minute_mod`
--

DROP TABLE IF EXISTS `spell_procs_per_minute_mod`;
CREATE TABLE `spell_procs_per_minute_mod` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PpmRateMod` float NOT NULL DEFAULT '0',
  `Type` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpecID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpellProcsPerMinuteID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_levels`
--

DROP TABLE IF EXISTS `spell_levels`;
CREATE TABLE `spell_levels` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpellLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_item_enchantment_condition`
--

DROP TABLE IF EXISTS `spell_item_enchantment_condition`;
CREATE TABLE `spell_item_enchantment_condition` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Color1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Color2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Color3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Color4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Color5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LtOperandType1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LtOperandType2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LtOperandType3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LtOperandType4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LtOperandType5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Comparator1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Comparator2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Comparator3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Comparator4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Comparator5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CompareColor1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CompareColor2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CompareColor3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CompareColor4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CompareColor5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Value1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Value2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Value3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Value4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Value5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Logic1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Logic2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Logic3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Logic4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Logic5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_interrupts`
--

DROP TABLE IF EXISTS `spell_interrupts`;
CREATE TABLE `spell_interrupts` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `AuraInterruptFlags1` int(10) unsigned NOT NULL DEFAULT '0',
  `AuraInterruptFlags2` int(10) unsigned NOT NULL DEFAULT '0',
  `ChannelInterruptFlags1` int(10) unsigned NOT NULL DEFAULT '0',
  `ChannelInterruptFlags2` int(10) unsigned NOT NULL DEFAULT '0',
  `InterruptFlags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_focus_object`
--

DROP TABLE IF EXISTS `spell_focus_object`;
CREATE TABLE `spell_focus_object` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_focus_object_locale`
--

DROP TABLE IF EXISTS `spell_focus_object_locale`;
CREATE TABLE `spell_focus_object_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_equipped_items`
--

DROP TABLE IF EXISTS `spell_equipped_items`;
CREATE TABLE `spell_equipped_items` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `EquippedItemInventoryTypeMask` int(11) NOT NULL DEFAULT '0',
  `EquippedItemSubClassMask` int(11) NOT NULL DEFAULT '0',
  `EquippedItemClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_effect_scaling`
--

DROP TABLE IF EXISTS `spell_effect_scaling`;
CREATE TABLE `spell_effect_scaling` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Coefficient` float NOT NULL DEFAULT '0',
  `Variance` float NOT NULL DEFAULT '0',
  `ResourceCoefficient` float NOT NULL DEFAULT '0',
  `SpellEffectId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_duration`
--

DROP TABLE IF EXISTS `spell_duration`;
CREATE TABLE `spell_duration` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration` int(11) NOT NULL DEFAULT '0',
  `MaxDuration` int(11) NOT NULL DEFAULT '0',
  `DurationPerLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_cooldowns`
--

DROP TABLE IF EXISTS `spell_cooldowns`;
CREATE TABLE `spell_cooldowns` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `CategoryRecoveryTime` int(10) unsigned NOT NULL DEFAULT '0',
  `RecoveryTime` int(10) unsigned NOT NULL DEFAULT '0',
  `StartRecoveryTime` int(10) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_category`
--

DROP TABLE IF EXISTS `spell_category`;
CREATE TABLE `spell_category` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `ChargeRecoveryTime` int(11) NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UsesPerWeek` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxCharges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_category_locale`
--

DROP TABLE IF EXISTS `spell_category_locale`;
CREATE TABLE `spell_category_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_categories`
--

DROP TABLE IF EXISTS `spell_categories`;
CREATE TABLE `spell_categories` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `Category` smallint(5) unsigned NOT NULL DEFAULT '0',
  `StartRecoveryCategory` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ChargeCategory` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DefenseType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DispelType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Mechanic` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PreventionType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_cast_times`
--

DROP TABLE IF EXISTS `spell_cast_times`;
CREATE TABLE `spell_cast_times` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Base` int(11) NOT NULL DEFAULT '0',
  `Minimum` int(11) NOT NULL DEFAULT '0',
  `PerLevel` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `spell_aura_options`
--

DROP TABLE IF EXISTS `spell_aura_options`;
CREATE TABLE `spell_aura_options` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ProcCharges` int(10) unsigned NOT NULL DEFAULT '0',
  `ProcTypeMask` int(10) unsigned NOT NULL DEFAULT '0',
  `ProcCategoryRecovery` int(10) unsigned NOT NULL DEFAULT '0',
  `CumulativeAura` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ProcChance` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpellProcsPerMinuteID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `specialization_spell`
--

DROP TABLE IF EXISTS `specialization_spell`;
CREATE TABLE `specialization_spell` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `OverridesSpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `DescriptionLang` text,
  `SpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `specialization_spell_locale`
--

DROP TABLE IF EXISTS `specialization_spell_locale`;
CREATE TABLE `specialization_spell_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DescriptionLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `skill_race_class_info`
--

DROP TABLE IF EXISTS `skill_race_class_info`;
CREATE TABLE `skill_race_class_info` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `RaceMask` int(11) NOT NULL DEFAULT '0',
  `SkillID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ClassMask` smallint(6) NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SkillTierID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Availability` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `skill_line`
--

DROP TABLE IF EXISTS `skill_line`;
CREATE TABLE `skill_line` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayName` text,
  `Description` text,
  `AlternateVerb` text,
  `SpellIconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CategoryID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CanLink` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ParentSkillLineID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `skill_line_locale`
--

DROP TABLE IF EXISTS `skill_line_locale`;
CREATE TABLE `skill_line_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DisplayName_lang` text,
  `Description_lang` text,
  `AlternateVerb_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `skill_line_ability`
--

DROP TABLE IF EXISTS `skill_line_ability`;
CREATE TABLE `skill_line_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `RaceMask` int(10) unsigned NOT NULL DEFAULT '0',
  `ClassMask` int(10) unsigned NOT NULL DEFAULT '0',
  `SupercedesSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `SkillLine` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinSkillLineRank` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TrivialSkillLineRankHigh` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TrivialSkillLineRankLow` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UniqueBit` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TradeSkillCategoryID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AquireMethod` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `NumSkillUps` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `scenario`
--

DROP TABLE IF EXISTS `scenario`;
CREATE TABLE `scenario` (
  `Name` text,
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `scenario_locale`
--

DROP TABLE IF EXISTS `scenario_locale`;
CREATE TABLE `scenario_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `scenario_step`
--

DROP TABLE IF EXISTS `scenario_step`;
CREATE TABLE `scenario_step` (
  `DescriptionLang` text,
  `TitleLang` text,
  `Criteriatreeid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ScenarioID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Supersedes` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewardQuestID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RelatedStep` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`DescriptionLang`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `scenario_step_locale`
--

DROP TABLE IF EXISTS `scenario_step_locale`;
CREATE TABLE `scenario_step_locale` (
  `DescriptionLang` text,
  `locale` varchar(4) NOT NULL,
  `DescriptionLang_lang` text,
  `TitleLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`DescriptionLang`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `scaling_stat_distribution`
--

DROP TABLE IF EXISTS `scaling_stat_distribution`;
CREATE TABLE `scaling_stat_distribution` (
  `ItemLevelCurveID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ItemLevelCurveID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `totem_category`
--

DROP TABLE IF EXISTS `totem_category`;
CREATE TABLE `totem_category` (
  `Name` text,
  `TotemCategoryType` int(10) unsigned NOT NULL DEFAULT '0',
  `TotemCategoryMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `totem_category_locale`
--

DROP TABLE IF EXISTS `totem_category_locale`;
CREATE TABLE `totem_category_locale` (
  `Name` text,
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Name`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `item_to_battle_pet_species`
--

DROP TABLE IF EXISTS `item_to_battle_pet_species`;
CREATE TABLE `item_to_battle_pet_species` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetSpeciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `transport_animation`
--

DROP TABLE IF EXISTS `transport_animation`;
CREATE TABLE `transport_animation` (
  `TransportID` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `PosZ` float NOT NULL DEFAULT '0',
  `SequenceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`TransportID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `transport_rotation`
--

DROP TABLE IF EXISTS `transport_rotation`;
CREATE TABLE `transport_rotation` (
  `TransportID` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `RotX` float NOT NULL DEFAULT '0',
  `RotY` float NOT NULL DEFAULT '0',
  `RotZ` float NOT NULL DEFAULT '0',
  `RotO` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`TransportID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `unit_power_bar`
--

DROP TABLE IF EXISTS `unit_power_bar`;
CREATE TABLE `unit_power_bar` (
  `MaxPower` int(10) unsigned NOT NULL DEFAULT '0',
  `RegenerationPeace` float NOT NULL DEFAULT '0',
  `RegenerationCombat` float NOT NULL DEFAULT '0',
  `FileDataID1` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID2` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID3` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID4` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID5` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID6` int(10) unsigned NOT NULL DEFAULT '0',
  `Color1` int(10) unsigned NOT NULL DEFAULT '0',
  `Color2` int(10) unsigned NOT NULL DEFAULT '0',
  `Color3` int(10) unsigned NOT NULL DEFAULT '0',
  `Color4` int(10) unsigned NOT NULL DEFAULT '0',
  `Color5` int(10) unsigned NOT NULL DEFAULT '0',
  `Color6` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Cost` text,
  `OutOfError` text,
  `ToolTip` text,
  `StartInset` float NOT NULL DEFAULT '0',
  `EndInset` float NOT NULL DEFAULT '0',
  `StartPower` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinPower` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CenterPower` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BarType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`MaxPower`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `unit_power_bar_locale`
--

DROP TABLE IF EXISTS `unit_power_bar_locale`;
CREATE TABLE `unit_power_bar_locale` (
  `MaxPower` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Cost_lang` text,
  `OutOfError_lang` text,
  `ToolTip_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`MaxPower`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `world_map_overlay`
--

DROP TABLE IF EXISTS `world_map_overlay`;
CREATE TABLE `world_map_overlay` (
  `TextureName` text,
  `MapAreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TextureWidth` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TextureHeight` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OffsetX` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OffsetY` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HitRectTop` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HitRectLeft` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HitRectBottom` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HitRectRight` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PlayerConditionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`TextureName`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `world_map_overlay_locale`
--

DROP TABLE IF EXISTS `world_map_overlay_locale`;
CREATE TABLE `world_map_overlay_locale` (
  `TextureName` text,
  `locale` varchar(4) NOT NULL,
  `TextureName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`TextureName`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `world_map_transforms`
--

DROP TABLE IF EXISTS `world_map_transforms`;
CREATE TABLE `world_map_transforms` (
  `RegionMinX` float NOT NULL DEFAULT '0',
  `RegionMinY` float NOT NULL DEFAULT '0',
  `RegionMinZ` float NOT NULL DEFAULT '0',
  `RegionMaxX` float NOT NULL DEFAULT '0',
  `RegionMaxY` float NOT NULL DEFAULT '0',
  `RegionMaxZ` float NOT NULL DEFAULT '0',
  `RegionOffsetX` float NOT NULL DEFAULT '0',
  `RegionOffsetY` float NOT NULL DEFAULT '0',
  `RegionScale` float NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NewMapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NewDungeonMapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NewAreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`RegionMin`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
