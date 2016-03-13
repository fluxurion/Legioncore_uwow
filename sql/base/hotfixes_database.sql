
-- ----------------------------
-- Table structure for `hotfix_data`
-- ----------------------------
DROP TABLE IF EXISTS `hotfix_data`;
CREATE TABLE `hotfix_data` (
  `TableHash` int(10) unsigned NOT NULL DEFAULT '0',
  `RecordID` int(10) NOT NULL,
  `Timestamp` int(10) unsigned NOT NULL DEFAULT '0',
  `Deleted` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`TableHash`,`RecordID`,`Timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `area_table`
--

DROP TABLE IF EXISTS `area_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area_table` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags1` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags2` int(10) unsigned NOT NULL DEFAULT '0',
  `ZoneName` text,
  `AmbientMultiplier` float NOT NULL DEFAULT '0',
  `AreaName` text,
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ParentAreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaBit` smallint(6) NOT NULL DEFAULT '0',
  `AmbienceID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ZoneMusic` smallint(5) unsigned NOT NULL DEFAULT '0',
  `IntroSound` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LiquidTypeID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LiquidTypeID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LiquidTypeID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LiquidTypeID4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UWZoneMusic` smallint(5) unsigned NOT NULL DEFAULT '0',
  `WorldPvPID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PvPCombastWorldStateID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SoundProviderPref` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SoundProviderPrefUnderwater` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ExplorationLevel` tinyint(4) NOT NULL DEFAULT '0',
  `FactionGroupMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MountFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UWIntroMusic` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `WildBattlePetLevelMin` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `WildBattlePetLevelMax` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `WindSettingsID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_table`
--

LOCK TABLES `area_table` WRITE;
/*!40000 ALTER TABLE `area_table` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `area_table_locale`
--

DROP TABLE IF EXISTS `area_table_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area_table_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `ZoneName_lang` text,
  `AreaName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_table_locale`
--

LOCK TABLES `area_table_locale` WRITE;
/*!40000 ALTER TABLE `area_table_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_table_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `area_trigger`
--

DROP TABLE IF EXISTS `area_trigger`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area_trigger` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `PosZ` float NOT NULL DEFAULT '0',
  `Radius` float NOT NULL DEFAULT '0',
  `BoxLength` float NOT NULL DEFAULT '0',
  `BoxWidth` float NOT NULL DEFAULT '0',
  `BoxHeight` float NOT NULL DEFAULT '0',
  `BoxYaw` float NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PhaseID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PhaseGroupID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ShapeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaTriggerActionSetID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PhaseUseFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ShapeType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_trigger`
--

LOCK TABLES `area_trigger` WRITE;
/*!40000 ALTER TABLE `area_trigger` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_trigger` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `achievement`
--

DROP TABLE IF EXISTS `achievement`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
  `RewardPoints` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Amount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `achievement`
--

LOCK TABLES `achievement` WRITE;
/*!40000 ALTER TABLE `achievement` DISABLE KEYS */;
/*!40000 ALTER TABLE `achievement` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `achievement_locale`
--

DROP TABLE IF EXISTS `achievement_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `achievement_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `Reward_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `achievement_locale`
--

LOCK TABLES `achievement_locale` WRITE;
/*!40000 ALTER TABLE `achievement_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `achievement_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `area_group_member`
--

DROP TABLE IF EXISTS `area_group_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area_group_member` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `AreaGroupID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `area_group_member`
--

LOCK TABLES `area_group_member` WRITE;
/*!40000 ALTER TABLE `area_group_member` DISABLE KEYS */;
/*!40000 ALTER TABLE `area_group_member` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auction_house`
--

DROP TABLE IF EXISTS `auction_house`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auction_house` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `FactionID` int(10) unsigned NOT NULL DEFAULT '0',
  `DepositRate` int(10) unsigned NOT NULL DEFAULT '0',
  `ConsignmentRate` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auction_house`
--

LOCK TABLES `auction_house` WRITE;
/*!40000 ALTER TABLE `auction_house` DISABLE KEYS */;
/*!40000 ALTER TABLE `auction_house` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auction_house_locale`
--

DROP TABLE IF EXISTS `auction_house_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auction_house_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auction_house_locale`
--

LOCK TABLES `auction_house_locale` WRITE;
/*!40000 ALTER TABLE `auction_house_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `auction_house_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `armor_location`
--

DROP TABLE IF EXISTS `armor_location`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `armor_location` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Value1` float NOT NULL DEFAULT '0',
  `Value2` float NOT NULL DEFAULT '0',
  `Value3` float NOT NULL DEFAULT '0',
  `Value4` float NOT NULL DEFAULT '0',
  `Value5` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `armor_location`
--

LOCK TABLES `armor_location` WRITE;
/*!40000 ALTER TABLE `armor_location` DISABLE KEYS */;
/*!40000 ALTER TABLE `armor_location` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `banned_addons`
--

DROP TABLE IF EXISTS `banned_addons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banned_addons` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `NameMD51` int(10) unsigned NOT NULL DEFAULT '0',
  `NameMD52` int(10) unsigned NOT NULL DEFAULT '0',
  `NameMD53` int(10) unsigned NOT NULL DEFAULT '0',
  `NameMD54` int(10) unsigned NOT NULL DEFAULT '0',
  `VersionMD51` int(10) unsigned NOT NULL DEFAULT '0',
  `VersionMD52` int(10) unsigned NOT NULL DEFAULT '0',
  `VersionMD53` int(10) unsigned NOT NULL DEFAULT '0',
  `VersionMD54` int(10) unsigned NOT NULL DEFAULT '0',
  `LastModified` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `banned_addons`
--

LOCK TABLES `banned_addons` WRITE;
/*!40000 ALTER TABLE `banned_addons` DISABLE KEYS */;
/*!40000 ALTER TABLE `banned_addons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battlemaster_list`
--

DROP TABLE IF EXISTS `battlemaster_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battlemaster_list` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID1` int(11) NOT NULL DEFAULT '0',
  `MapID2` int(11) NOT NULL DEFAULT '0',
  `MapID3` int(11) NOT NULL DEFAULT '0',
  `MapID4` int(11) NOT NULL DEFAULT '0',
  `MapID5` int(11) NOT NULL DEFAULT '0',
  `MapID6` int(11) NOT NULL DEFAULT '0',
  `MapID7` int(11) NOT NULL DEFAULT '0',
  `MapID8` int(11) NOT NULL DEFAULT '0',
  `MapID9` int(11) NOT NULL DEFAULT '0',
  `MapID10` int(11) NOT NULL DEFAULT '0',
  `MapID11` int(11) NOT NULL DEFAULT '0',
  `MapID12` int(11) NOT NULL DEFAULT '0',
  `MapID13` int(11) NOT NULL DEFAULT '0',
  `MapID14` int(11) NOT NULL DEFAULT '0',
  `MapID15` int(11) NOT NULL DEFAULT '0',
  `MapID16` int(11) NOT NULL DEFAULT '0',
  `InstanceType` int(10) unsigned NOT NULL DEFAULT '0',
  `NameLang` text,
  `MaxGroupSize` int(10) unsigned NOT NULL DEFAULT '0',
  `HolidayWorldState` int(10) unsigned NOT NULL DEFAULT '0',
  `Minlevel` int(10) unsigned NOT NULL DEFAULT '0',
  `Maxlevel` int(10) unsigned NOT NULL DEFAULT '0',
  `RatedPlayers` int(10) unsigned NOT NULL DEFAULT '0',
  `MinPlayers` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxPlayers` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battlemaster_list`
--

LOCK TABLES `battlemaster_list` WRITE;
/*!40000 ALTER TABLE `battlemaster_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `battlemaster_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_classes`
--

DROP TABLE IF EXISTS `chr_classes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chr_classes` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PetNameToken` text,
  `Name` text,
  `NameFemale` text,
  `NameMale` text,
  `Filename` text,
  `CreateScreenFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `SelectScreenFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `IconFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CinematicSequenceID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DefaultSpec` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PowerType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpellClassSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `AttackPowerPerStrength` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `AttackPowerPerAgility` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RangedAttackPowerPerAgility` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LowResScreenFileDataID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LegionUnk` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_classes`
--

LOCK TABLES `chr_classes` WRITE;
/*!40000 ALTER TABLE `chr_classes` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_classes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_classes_locale`
--

DROP TABLE IF EXISTS `chr_classes_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chr_classes_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `PetNameToken_lang` text,
  `Name_lang` text,
  `NameFemale_lang` text,
  `NameMale_lang` text,
  `Filename_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_classes_locale`
--

LOCK TABLES `chr_classes_locale` WRITE;
/*!40000 ALTER TABLE `chr_classes_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_classes_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `char_sections`
--

DROP TABLE IF EXISTS `char_sections`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `char_sections` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID1` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID2` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID3` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GenType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Color` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `char_sections`
--

LOCK TABLES `char_sections` WRITE;
/*!40000 ALTER TABLE `char_sections` DISABLE KEYS */;
/*!40000 ALTER TABLE `char_sections` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_model_data`
--

DROP TABLE IF EXISTS `creature_model_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_model_data` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `ModelScale` float NOT NULL DEFAULT '0',
  `FootprintTextureID` int(10) unsigned NOT NULL DEFAULT '0',
  `FootprintTextureLength` float NOT NULL DEFAULT '0',
  `FootprintTextureWidth` float NOT NULL DEFAULT '0',
  `CollisionWidth` float NOT NULL DEFAULT '0',
  `CollisionHeight` float NOT NULL DEFAULT '0',
  `MountHeight` float NOT NULL DEFAULT '0',
  `GeoBox1` float NOT NULL DEFAULT '0',
  `GeoBox2` float NOT NULL DEFAULT '0',
  `GeoBox3` float NOT NULL DEFAULT '0',
  `GeoBox4` float NOT NULL DEFAULT '0',
  `GeoBox5` float NOT NULL DEFAULT '0',
  `GeoBox6` float NOT NULL DEFAULT '0',
  `WorldEffectScale` float NOT NULL DEFAULT '0',
  `AttachedEffectScale` float NOT NULL DEFAULT '0',
  `MissileCollisionRadius` float NOT NULL DEFAULT '0',
  `MissileCollisionPush` float NOT NULL DEFAULT '0',
  `MissileCollisionRaise` float NOT NULL DEFAULT '0',
  `OverrideLootEffectScale` float NOT NULL DEFAULT '0',
  `OverrideNameScale` float NOT NULL DEFAULT '0',
  `OverrideSelectionRadius` float NOT NULL DEFAULT '0',
  `TamedPetBaseScale` float NOT NULL DEFAULT '0',
  `HoverHeight` float NOT NULL DEFAULT '0',
  `SoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureGeosetDataID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SizeClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FootstepShakeSize` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DeathThudShakeSize` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_model_data`
--

LOCK TABLES `creature_model_data` WRITE;
/*!40000 ALTER TABLE `creature_model_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_model_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `faction`
--

DROP TABLE IF EXISTS `faction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `faction` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseRepRaceMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseRepRaceMask2` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseRepRaceMask3` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseRepRaceMask4` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseRepValue1` int(11) NOT NULL DEFAULT '0',
  `BaseRepValue2` int(11) NOT NULL DEFAULT '0',
  `BaseRepValue3` int(11) NOT NULL DEFAULT '0',
  `BaseRepValue4` int(11) NOT NULL DEFAULT '0',
  `SpilloverRate1` float NOT NULL DEFAULT '0',
  `SpilloverRate2` float NOT NULL DEFAULT '0',
  `Name` text,
  `Desc` text,
  `ReputationListID` smallint(6) NOT NULL DEFAULT '0',
  `BaseRepClassMask1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseRepClassMask2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseRepClassMask3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseRepClassMask4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReputationFlags1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReputationFlags2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReputationFlags3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReputationFlags4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Team` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpilloverRank1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpilloverRank2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Expansion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FriendshipRepID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `faction`
--

LOCK TABLES `faction` WRITE;
/*!40000 ALTER TABLE `faction` DISABLE KEYS */;
/*!40000 ALTER TABLE `faction` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `faction_locale`
--

DROP TABLE IF EXISTS `faction_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `faction_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Desc_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `faction_locale`
--

LOCK TABLES `faction_locale` WRITE;
/*!40000 ALTER TABLE `faction_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `faction_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `faction_template`
--

DROP TABLE IF EXISTS `faction_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `faction_template` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Faction` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Enemies1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Enemies2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Enemies3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Enemies4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Friends1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Friends2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Friends3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Friends4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Mask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FriendMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EnemyMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `faction_template`
--

LOCK TABLES `faction_template` WRITE;
/*!40000 ALTER TABLE `faction_template` DISABLE KEYS */;
/*!40000 ALTER TABLE `faction_template` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `glyph_slot`
--

DROP TABLE IF EXISTS `glyph_slot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `glyph_slot` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `TypeFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `Order` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `glyph_slot`
--

LOCK TABLES `glyph_slot` WRITE;
/*!40000 ALTER TABLE `glyph_slot` DISABLE KEYS */;
/*!40000 ALTER TABLE `glyph_slot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `l_f_g_dungeon`
--

DROP TABLE IF EXISTS `l_f_g_dungeon`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `l_f_g_dungeon` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `TextureFilename` text,
  `Description` text,
  `MaxLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TargetLevelMax` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MapID` smallint(6) NOT NULL DEFAULT '0',
  `RandomID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ScenarioID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LastBossJournalEncounterID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BonusReputationAmount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `TimeWalkerItemLvl` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TargetLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TargetLevelMin` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Faction` tinyint(4) NOT NULL DEFAULT '0',
  `Expansion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GroupID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CountTank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CountHealer` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CountDamage` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinCountTank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinCountHealer` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinCountDamage` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SubType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `l_f_g_dungeon`
--

LOCK TABLES `l_f_g_dungeon` WRITE;
/*!40000 ALTER TABLE `l_f_g_dungeon` DISABLE KEYS */;
/*!40000 ALTER TABLE `l_f_g_dungeon` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `l_f_g_dungeon_locale`
--

DROP TABLE IF EXISTS `l_f_g_dungeon_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `l_f_g_dungeon_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `TextureFilename_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `l_f_g_dungeon_locale`
--

LOCK TABLES `l_f_g_dungeon_locale` WRITE;
/*!40000 ALTER TABLE `l_f_g_dungeon_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `l_f_g_dungeon_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `liquid_type`
--

DROP TABLE IF EXISTS `liquid_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `liquid_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `unk_1` float NOT NULL DEFAULT '0',
  `unk_2` float NOT NULL DEFAULT '0',
  `unk_3` float NOT NULL DEFAULT '0',
  `unk_4` float NOT NULL DEFAULT '0',
  `unk_5` float NOT NULL DEFAULT '0',
  `Texture1` text,
  `Texture2` text,
  `Texture3` text,
  `Texture4` text,
  `Texture5` text,
  `Texture6` text,
  `unk2_` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3_` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4_1` float NOT NULL DEFAULT '0',
  `unk4_2` float NOT NULL DEFAULT '0',
  `unk4_3` float NOT NULL DEFAULT '0',
  `unk4_4` float NOT NULL DEFAULT '0',
  `unk4_5` float NOT NULL DEFAULT '0',
  `unk4_6` float NOT NULL DEFAULT '0',
  `unk4_7` float NOT NULL DEFAULT '0',
  `unk4_8` float NOT NULL DEFAULT '0',
  `unk4_9` float NOT NULL DEFAULT '0',
  `unk4_10` float NOT NULL DEFAULT '0',
  `unk4_11` float NOT NULL DEFAULT '0',
  `unk4_12` float NOT NULL DEFAULT '0',
  `unk4_13` float NOT NULL DEFAULT '0',
  `unk4_14` float NOT NULL DEFAULT '0',
  `unk4_15` float NOT NULL DEFAULT '0',
  `unk4_16` float NOT NULL DEFAULT '0',
  `unk4_17` float NOT NULL DEFAULT '0',
  `unk4_18` float NOT NULL DEFAULT '0',
  `unk5_` int(10) unsigned NOT NULL DEFAULT '0',
  `unk6_` int(10) unsigned NOT NULL DEFAULT '0',
  `unk7_` int(10) unsigned NOT NULL DEFAULT '0',
  `unk8_` int(10) unsigned NOT NULL DEFAULT '0',
  `unk9_` smallint(5) unsigned NOT NULL DEFAULT '0',
  `unk10_` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_6` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_8` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_9` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk11_10` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `liquid_type`
--

LOCK TABLES `liquid_type` WRITE;
/*!40000 ALTER TABLE `liquid_type` DISABLE KEYS */;
/*!40000 ALTER TABLE `liquid_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `liquid_type_locale`
--

DROP TABLE IF EXISTS `liquid_type_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `liquid_type_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Texture1_lang` text,
  `Texture2_lang` text,
  `Texture3_lang` text,
  `Texture4_lang` text,
  `Texture5_lang` text,
  `Texture6_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `liquid_type_locale`
--

LOCK TABLES `liquid_type_locale` WRITE;
/*!40000 ALTER TABLE `liquid_type_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `liquid_type_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `map_difficulty`
--

DROP TABLE IF EXISTS `map_difficulty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map_difficulty` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Message` text,
  `RaidDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LockID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemBonusTreeModID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `map_difficulty`
--

LOCK TABLES `map_difficulty` WRITE;
/*!40000 ALTER TABLE `map_difficulty` DISABLE KEYS */;
/*!40000 ALTER TABLE `map_difficulty` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `map_difficulty_locale`
--

DROP TABLE IF EXISTS `map_difficulty_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map_difficulty_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Message_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `map_difficulty_locale`
--

LOCK TABLES `map_difficulty_locale` WRITE;
/*!40000 ALTER TABLE `map_difficulty_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `map_difficulty_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_item_enchantment`
--

DROP TABLE IF EXISTS `spell_item_enchantment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_item_enchantment` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectSpellID1` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectSpellID2` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectSpellID3` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `EffectScalingPoints1` float NOT NULL DEFAULT '0',
  `EffectScalingPoints2` float NOT NULL DEFAULT '0',
  `EffectScalingPoints3` float NOT NULL DEFAULT '0',
  `UnkLegion` int(10) unsigned NOT NULL DEFAULT '0',
  `EffectPointsMin1` smallint(6) NOT NULL DEFAULT '0',
  `EffectPointsMin2` smallint(6) NOT NULL DEFAULT '0',
  `EffectPointsMin3` smallint(6) NOT NULL DEFAULT '0',
  `ItemVisual` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillRank` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Charges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Effect1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Effect2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Effect3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ScalingClass` tinyint(4) NOT NULL DEFAULT '0',
  `ScalingClassRestricted` tinyint(4) NOT NULL DEFAULT '0',
  `UnkLegion3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_item_enchantment`
--

LOCK TABLES `spell_item_enchantment` WRITE;
/*!40000 ALTER TABLE `spell_item_enchantment` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_item_enchantment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_item_enchantment_locale`
--

DROP TABLE IF EXISTS `spell_item_enchantment_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_item_enchantment_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_item_enchantment_locale`
--

LOCK TABLES `spell_item_enchantment_locale` WRITE;
/*!40000 ALTER TABLE `spell_item_enchantment_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_item_enchantment_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `talent`
--

DROP TABLE IF EXISTS `talent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `talent` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `OverridesSpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `Description` text,
  `SpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Row` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Column` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CategoryMask1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CategoryMask2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `talent`
--

LOCK TABLES `talent` WRITE;
/*!40000 ALTER TABLE `talent` DISABLE KEYS */;
/*!40000 ALTER TABLE `talent` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `talent_locale`
--

DROP TABLE IF EXISTS `talent_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `talent_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `talent_locale`
--

LOCK TABLES `talent_locale` WRITE;
/*!40000 ALTER TABLE `talent_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `talent_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vehicle`
--

DROP TABLE IF EXISTS `vehicle`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vehicle` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `TurnSpeed` float NOT NULL DEFAULT '0',
  `PitchSpeed` float NOT NULL DEFAULT '0',
  `PitchMin` float NOT NULL DEFAULT '0',
  `PitchMax` float NOT NULL DEFAULT '0',
  `MouseLookOffsetPitch` float NOT NULL DEFAULT '0',
  `CameraFadeDistScalarMin` float NOT NULL DEFAULT '0',
  `CameraFadeDistScalarMax` float NOT NULL DEFAULT '0',
  `CameraPitchOffset` float NOT NULL DEFAULT '0',
  `FacingLimitRight` float NOT NULL DEFAULT '0',
  `FacingLimitLeft` float NOT NULL DEFAULT '0',
  `MsslTrgtTurnLingering` float NOT NULL DEFAULT '0',
  `MsslTrgtPitchLingering` float NOT NULL DEFAULT '0',
  `MsslTrgtMouseLingering` float NOT NULL DEFAULT '0',
  `MsslTrgtEndOpacity` float NOT NULL DEFAULT '0',
  `MsslTrgtArcSpeed` float NOT NULL DEFAULT '0',
  `MsslTrgtArcRepeat` float NOT NULL DEFAULT '0',
  `MsslTrgtArcWidth` float NOT NULL DEFAULT '0',
  `MsslTrgtImpactRadius1` float NOT NULL DEFAULT '0',
  `MsslTrgtImpactRadius2` float NOT NULL DEFAULT '0',
  `MsslTrgtArcTexture` text,
  `MsslTrgtImpactTexture` text,
  `MsslTrgtImpactModel1` text,
  `MsslTrgtImpactModel2` text,
  `CameraYawOffset` float NOT NULL DEFAULT '0',
  `MsslTrgtImpactTexRadius` float NOT NULL DEFAULT '0',
  `SeatID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SeatID8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PowerDisplayID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PowerDisplayID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PowerDisplayID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `FlagsB` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UILocomotionType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VehicleUIIndicatorID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vehicle`
--

LOCK TABLES `vehicle` WRITE;
/*!40000 ALTER TABLE `vehicle` DISABLE KEYS */;
/*!40000 ALTER TABLE `vehicle` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vehicle_locale`
--

DROP TABLE IF EXISTS `vehicle_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vehicle_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `MsslTrgtArcTexture_lang` text,
  `MsslTrgtImpactTexture_lang` text,
  `MsslTrgtImpactModel1_lang` text,
  `MsslTrgtImpactModel2_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vehicle_locale`
--

LOCK TABLES `vehicle_locale` WRITE;
/*!40000 ALTER TABLE `vehicle_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `vehicle_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `w_m_o_area_table`
--

DROP TABLE IF EXISTS `w_m_o_area_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `w_m_o_area_table` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `WMOGroupID` int(11) NOT NULL DEFAULT '0',
  `AreaName` text,
  `WMOID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AmbienceID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ZoneMusic` smallint(5) unsigned NOT NULL DEFAULT '0',
  `IntroSound` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaTableID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UWIntroSound` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UWAmbience` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NameSet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SoundProviderPref` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SoundProviderPrefUnderwater` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UWZoneMusic` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `w_m_o_area_table`
--

LOCK TABLES `w_m_o_area_table` WRITE;
/*!40000 ALTER TABLE `w_m_o_area_table` DISABLE KEYS */;
/*!40000 ALTER TABLE `w_m_o_area_table` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `w_m_o_area_table_locale`
--

DROP TABLE IF EXISTS `w_m_o_area_table_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `w_m_o_area_table_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `AreaName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `w_m_o_area_table_locale`
--

LOCK TABLES `w_m_o_area_table_locale` WRITE;
/*!40000 ALTER TABLE `w_m_o_area_table_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `w_m_o_area_table_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_map_area`
--

DROP TABLE IF EXISTS `world_map_area`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_map_area` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `AreaName` text,
  `LocLeft` float NOT NULL DEFAULT '0',
  `LocRight` float NOT NULL DEFAULT '0',
  `LocTop` float NOT NULL DEFAULT '0',
  `LocBottom` float NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AreaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DisplayMapID` smallint(6) NOT NULL DEFAULT '0',
  `DefaultDungeonFloor` smallint(6) NOT NULL DEFAULT '0',
  `ParentWorldMapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LevelRangeMin` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LevelRangeMax` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Unk` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Unk2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_map_area`
--

LOCK TABLES `world_map_area` WRITE;
/*!40000 ALTER TABLE `world_map_area` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_map_area` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_map_area_locale`
--

DROP TABLE IF EXISTS `world_map_area_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_map_area_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `AreaName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_map_area_locale`
--

LOCK TABLES `world_map_area_locale` WRITE;
/*!40000 ALTER TABLE `world_map_area_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_map_area_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_safe_locs`
--

DROP TABLE IF EXISTS `world_safe_locs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_safe_locs` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LocX` float NOT NULL DEFAULT '0',
  `LocY` float NOT NULL DEFAULT '0',
  `LocZ` float NOT NULL DEFAULT '0',
  `LocO` float NOT NULL DEFAULT '0',
  `AreaName` text,
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_safe_locs`
--

LOCK TABLES `world_safe_locs` WRITE;
/*!40000 ALTER TABLE `world_safe_locs` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_safe_locs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_safe_locs_locale`
--

DROP TABLE IF EXISTS `world_safe_locs_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_safe_locs_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `AreaName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_safe_locs_locale`
--

LOCK TABLES `world_safe_locs_locale` WRITE;
/*!40000 ALTER TABLE `world_safe_locs_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_safe_locs_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bank_bag_slot_prices`
--

DROP TABLE IF EXISTS `bank_bag_slot_prices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bank_bag_slot_prices` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Price` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bank_bag_slot_prices`
--

LOCK TABLES `bank_bag_slot_prices` WRITE;
/*!40000 ALTER TABLE `bank_bag_slot_prices` DISABLE KEYS */;
/*!40000 ALTER TABLE `bank_bag_slot_prices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `barber_shop_style`
--

DROP TABLE IF EXISTS `barber_shop_style`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `barber_shop_style` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `CostMultiplier` float NOT NULL DEFAULT '0',
  `Race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `HairID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Tato` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `barber_shop_style`
--

LOCK TABLES `barber_shop_style` WRITE;
/*!40000 ALTER TABLE `barber_shop_style` DISABLE KEYS */;
/*!40000 ALTER TABLE `barber_shop_style` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `barber_shop_style_locale`
--

DROP TABLE IF EXISTS `barber_shop_style_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `barber_shop_style_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `barber_shop_style_locale`
--

LOCK TABLES `barber_shop_style_locale` WRITE;
/*!40000 ALTER TABLE `barber_shop_style_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `barber_shop_style_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `broadcast_text`
--

DROP TABLE IF EXISTS `broadcast_text`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `broadcast_text` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MaleText` text,
  `FemaleText` text,
  `EmoteID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteDelay1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteDelay2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteDelay3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `UnkEmoteID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Language` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `broadcast_text`
--

LOCK TABLES `broadcast_text` WRITE;
/*!40000 ALTER TABLE `broadcast_text` DISABLE KEYS */;
/*!40000 ALTER TABLE `broadcast_text` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `broadcast_text_locale`
--

DROP TABLE IF EXISTS `broadcast_text_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `broadcast_text_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `MaleText_lang` text,
  `FemaleText_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `broadcast_text_locale`
--

LOCK TABLES `broadcast_text_locale` WRITE;
/*!40000 ALTER TABLE `broadcast_text_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `broadcast_text_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `char_titles`
--

DROP TABLE IF EXISTS `char_titles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `char_titles` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `NameMale` text,
  `NameFemale` text,
  `ConditionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaskID` smallint(6) NOT NULL DEFAULT '0',
  `Flags` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `char_titles`
--

LOCK TABLES `char_titles` WRITE;
/*!40000 ALTER TABLE `char_titles` DISABLE KEYS */;
/*!40000 ALTER TABLE `char_titles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `char_titles_locale`
--

DROP TABLE IF EXISTS `char_titles_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `char_titles_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `NameMale_lang` text,
  `NameFemale_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `char_titles_locale`
--

LOCK TABLES `char_titles_locale` WRITE;
/*!40000 ALTER TABLE `char_titles_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `char_titles_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `curve_point`
--

DROP TABLE IF EXISTS `curve_point`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `curve_point` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `CurveID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Index` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `curve_point`
--

LOCK TABLES `curve_point` WRITE;
/*!40000 ALTER TABLE `curve_point` DISABLE KEYS */;
/*!40000 ALTER TABLE `curve_point` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_family`
--

DROP TABLE IF EXISTS `creature_family`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_family` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MinScale` float NOT NULL DEFAULT '0',
  `MaxScale` float NOT NULL DEFAULT '0',
  `Name` text,
  `IconFile` text,
  `SkillLine1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SkillLine2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PetFoodMask` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaxScaleLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinScaleLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PetTalentType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CategoryEnumID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_family`
--

LOCK TABLES `creature_family` WRITE;
/*!40000 ALTER TABLE `creature_family` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_family` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_family_locale`
--

DROP TABLE IF EXISTS `creature_family_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_family_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `IconFile_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_family_locale`
--

LOCK TABLES `creature_family_locale` WRITE;
/*!40000 ALTER TABLE `creature_family_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_family_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `difficulty`
--

DROP TABLE IF EXISTS `difficulty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `difficulty` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `FallbackDifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `InstanceType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OldEnumValue` tinyint(4) NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ToggleDifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GroupSizeHealthCurveID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GroupSizeDmgCurveID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `GroupSizeSpellPointsCurveID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemBonusTreeModID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `difficulty`
--

LOCK TABLES `difficulty` WRITE;
/*!40000 ALTER TABLE `difficulty` DISABLE KEYS */;
/*!40000 ALTER TABLE `difficulty` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `difficulty_locale`
--

DROP TABLE IF EXISTS `difficulty_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `difficulty_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `difficulty_locale`
--

LOCK TABLES `difficulty_locale` WRITE;
/*!40000 ALTER TABLE `difficulty_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `difficulty_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dungeon_encounter`
--

DROP TABLE IF EXISTS `dungeon_encounter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `CreatureDisplayID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpellIconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EncounterID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dungeon_encounter`
--

LOCK TABLES `dungeon_encounter` WRITE;
/*!40000 ALTER TABLE `dungeon_encounter` DISABLE KEYS */;
/*!40000 ALTER TABLE `dungeon_encounter` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dungeon_encounter_locale`
--

DROP TABLE IF EXISTS `dungeon_encounter_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dungeon_encounter_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dungeon_encounter_locale`
--

LOCK TABLES `dungeon_encounter_locale` WRITE;
/*!40000 ALTER TABLE `dungeon_encounter_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `dungeon_encounter_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `emotes_text`
--

DROP TABLE IF EXISTS `emotes_text`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emotes_text` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `TextID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `emotes_text`
--

LOCK TABLES `emotes_text` WRITE;
/*!40000 ALTER TABLE `emotes_text` DISABLE KEYS */;
/*!40000 ALTER TABLE `emotes_text` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `emotes_text_locale`
--

DROP TABLE IF EXISTS `emotes_text_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emotes_text_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `emotes_text_locale`
--

LOCK TABLES `emotes_text_locale` WRITE;
/*!40000 ALTER TABLE `emotes_text_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `emotes_text_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `phase`
--

DROP TABLE IF EXISTS `phase`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `phase` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `phase`
--

LOCK TABLES `phase` WRITE;
/*!40000 ALTER TABLE `phase` DISABLE KEYS */;
/*!40000 ALTER TABLE `phase` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `map`
--

DROP TABLE IF EXISTS `map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Directory` text,
  `Flags1` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags2` int(10) unsigned NOT NULL DEFAULT '0',
  `MinimapIconScale` float NOT NULL DEFAULT '0',
  `CorpsePosX` float NOT NULL DEFAULT '0',
  `CorpsePosY` float NOT NULL DEFAULT '0',
  `RaidOffset` int(10) unsigned NOT NULL DEFAULT '0',
  `MapName` text,
  `MapDescription0` text,
  `MapDescription1` text,
  `AreaTableID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LoadingScreenID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CorpseMapID` smallint(6) NOT NULL DEFAULT '0',
  `TimeOfDayOverride` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ParentMapID` smallint(6) NOT NULL DEFAULT '0',
  `CosmeticParentMapID` smallint(6) NOT NULL DEFAULT '0',
  `InstanceType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MapType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ExpansionID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxPlayers` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TimeOffset` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `map`
--

LOCK TABLES `map` WRITE;
/*!40000 ALTER TABLE `map` DISABLE KEYS */;
/*!40000 ALTER TABLE `map` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `map_locale`
--

DROP TABLE IF EXISTS `map_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `map_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Directory_lang` text,
  `MapName_lang` text,
  `MapDescription0_lang` text,
  `MapDescription1_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `map_locale`
--

LOCK TABLES `map_locale` WRITE;
/*!40000 ALTER TABLE `map_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `map_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `minor_talent`
--

DROP TABLE IF EXISTS `minor_talent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `minor_talent` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpecID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `minor_talent`
--

LOCK TABLES `minor_talent` WRITE;
/*!40000 ALTER TABLE `minor_talent` DISABLE KEYS */;
/*!40000 ALTER TABLE `minor_talent` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `criteria`
--

DROP TABLE IF EXISTS `criteria`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `criteria` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Asset` int(11) NOT NULL DEFAULT '0',
  `StartAsset` int(10) unsigned NOT NULL DEFAULT '0',
  `FailAsset` int(10) unsigned NOT NULL DEFAULT '0',
  `StartTimer` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ModifyTree` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EligibilityWorldStateID` smallint(6) NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StartEvent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FailEvent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EligibilityWorldStateValue` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `criteria`
--

LOCK TABLES `criteria` WRITE;
/*!40000 ALTER TABLE `criteria` DISABLE KEYS */;
/*!40000 ALTER TABLE `criteria` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `criteria_tree`
--

DROP TABLE IF EXISTS `criteria_tree`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `criteria_tree` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Amount` int(10) unsigned NOT NULL DEFAULT '0',
  `Description` text,
  `CriteriaID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Parent` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(6) NOT NULL DEFAULT '0',
  `OrderIndex` smallint(6) NOT NULL DEFAULT '0',
  `Operator` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `criteria_tree`
--

LOCK TABLES `criteria_tree` WRITE;
/*!40000 ALTER TABLE `criteria_tree` DISABLE KEYS */;
/*!40000 ALTER TABLE `criteria_tree` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `criteria_tree_locale`
--

DROP TABLE IF EXISTS `criteria_tree_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `criteria_tree_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `criteria_tree_locale`
--

LOCK TABLES `criteria_tree_locale` WRITE;
/*!40000 ALTER TABLE `criteria_tree_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `criteria_tree_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `character_loadout_item`
--

DROP TABLE IF EXISTS `character_loadout_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_loadout_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LoadOutID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_loadout_item`
--

LOCK TABLES `character_loadout_item` WRITE;
/*!40000 ALTER TABLE `character_loadout_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_loadout_item` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `character_loadout`
--

DROP TABLE IF EXISTS `character_loadout`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_loadout` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Racemask` int(10) unsigned NOT NULL DEFAULT '0',
  `ClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Purpose` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_loadout`
--

LOCK TABLES `character_loadout` WRITE;
/*!40000 ALTER TABLE `character_loadout` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_loadout` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `char_start_outfit`
--

DROP TABLE IF EXISTS `char_start_outfit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `char_start_outfit`
--

LOCK TABLES `char_start_outfit` WRITE;
/*!40000 ALTER TABLE `char_start_outfit` DISABLE KEYS */;
/*!40000 ALTER TABLE `char_start_outfit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_classes_x_power_types`
--

DROP TABLE IF EXISTS `chr_classes_x_power_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chr_classes_x_power_types` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PowerType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_classes_x_power_types`
--

LOCK TABLES `chr_classes_x_power_types` WRITE;
/*!40000 ALTER TABLE `chr_classes_x_power_types` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_classes_x_power_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_races`
--

DROP TABLE IF EXISTS `chr_races`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_races`
--

LOCK TABLES `chr_races` WRITE;
/*!40000 ALTER TABLE `chr_races` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_races` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_races_locale`
--

DROP TABLE IF EXISTS `chr_races_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_races_locale`
--

LOCK TABLES `chr_races_locale` WRITE;
/*!40000 ALTER TABLE `chr_races_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_races_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_display_info`
--

DROP TABLE IF EXISTS `creature_display_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_display_info` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ExtendedDisplayInfoID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureModelScale` float NOT NULL DEFAULT '0',
  `Unknown620` float NOT NULL DEFAULT '0',
  `Unknown7011` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown7012` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown7013` int(10) unsigned NOT NULL DEFAULT '0',
  `PortraitTextureName` text,
  `PortraitCreatureDisplayInfoID` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown0` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown1` int(11) NOT NULL DEFAULT '0',
  `UnkLegion2` float NOT NULL DEFAULT '0',
  `ModelID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NPCSoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Unknown2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Unknown3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Unknown5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureModelAlpha` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SizeClass` tinyint(4) NOT NULL DEFAULT '0',
  `Unknown6` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Unknown7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Gender` tinyint(4) NOT NULL DEFAULT '0',
  `Unknown8` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_display_info`
--

LOCK TABLES `creature_display_info` WRITE;
/*!40000 ALTER TABLE `creature_display_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_display_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_display_info_locale`
--

DROP TABLE IF EXISTS `creature_display_info_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_display_info_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `PortraitTextureName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_display_info_locale`
--

LOCK TABLES `creature_display_info_locale` WRITE;
/*!40000 ALTER TABLE `creature_display_info_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_display_info_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_type`
--

DROP TABLE IF EXISTS `creature_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_type`
--

LOCK TABLES `creature_type` WRITE;
/*!40000 ALTER TABLE `creature_type` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_type_locale`
--

DROP TABLE IF EXISTS `creature_type_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_type_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_type_locale`
--

LOCK TABLES `creature_type_locale` WRITE;
/*!40000 ALTER TABLE `creature_type_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_type_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `destructible_model_data`
--

DROP TABLE IF EXISTS `destructible_model_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destructible_model_data` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `destructible_model_data`
--

LOCK TABLES `destructible_model_data` WRITE;
/*!40000 ALTER TABLE `destructible_model_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `destructible_model_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `durability_costs`
--

DROP TABLE IF EXISTS `durability_costs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `durability_costs` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `durability_costs`
--

LOCK TABLES `durability_costs` WRITE;
/*!40000 ALTER TABLE `durability_costs` DISABLE KEYS */;
/*!40000 ALTER TABLE `durability_costs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `durability_quality`
--

DROP TABLE IF EXISTS `durability_quality`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `durability_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `durability_quality`
--

LOCK TABLES `durability_quality` WRITE;
/*!40000 ALTER TABLE `durability_quality` DISABLE KEYS */;
/*!40000 ALTER TABLE `durability_quality` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gameobject_display_info`
--

DROP TABLE IF EXISTS `gameobject_display_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gameobject_display_info` (
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
  `ObjectEffectPackageID` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gameobject_display_info`
--

LOCK TABLES `gameobject_display_info` WRITE;
/*!40000 ALTER TABLE `gameobject_display_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobject_display_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `import_price_armor`
--

DROP TABLE IF EXISTS `import_price_armor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `import_price_armor` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ClothModifier` float NOT NULL DEFAULT '0',
  `LeatherModifier` float NOT NULL DEFAULT '0',
  `ChainModifier` float NOT NULL DEFAULT '0',
  `PlateModifier` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `import_price_armor`
--

LOCK TABLES `import_price_armor` WRITE;
/*!40000 ALTER TABLE `import_price_armor` DISABLE KEYS */;
/*!40000 ALTER TABLE `import_price_armor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `import_price_quality`
--

DROP TABLE IF EXISTS `import_price_quality`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `import_price_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `import_price_quality`
--

LOCK TABLES `import_price_quality` WRITE;
/*!40000 ALTER TABLE `import_price_quality` DISABLE KEYS */;
/*!40000 ALTER TABLE `import_price_quality` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `import_price_shield`
--

DROP TABLE IF EXISTS `import_price_shield`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `import_price_shield` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `import_price_shield`
--

LOCK TABLES `import_price_shield` WRITE;
/*!40000 ALTER TABLE `import_price_shield` DISABLE KEYS */;
/*!40000 ALTER TABLE `import_price_shield` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `import_price_weapon`
--

DROP TABLE IF EXISTS `import_price_weapon`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `import_price_weapon` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Data` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `import_price_weapon`
--

LOCK TABLES `import_price_weapon` WRITE;
/*!40000 ALTER TABLE `import_price_weapon` DISABLE KEYS */;
/*!40000 ALTER TABLE `import_price_weapon` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_armor_quality`
--

DROP TABLE IF EXISTS `item_armor_quality`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_armor_quality`
--

LOCK TABLES `item_armor_quality` WRITE;
/*!40000 ALTER TABLE `item_armor_quality` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_armor_quality` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_armor_shield`
--

DROP TABLE IF EXISTS `item_armor_shield`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_armor_shield`
--

LOCK TABLES `item_armor_shield` WRITE;
/*!40000 ALTER TABLE `item_armor_shield` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_armor_shield` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_armor_total`
--

DROP TABLE IF EXISTS `item_armor_total`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_armor_total`
--

LOCK TABLES `item_armor_total` WRITE;
/*!40000 ALTER TABLE `item_armor_total` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_armor_total` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_damage`
--

DROP TABLE IF EXISTS `item_damage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_damage`
--

LOCK TABLES `item_damage` WRITE;
/*!40000 ALTER TABLE `item_damage` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_damage` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_class`
--

DROP TABLE IF EXISTS `item_class`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_class` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Class` int(10) unsigned NOT NULL DEFAULT '0',
  `PriceFactor` float NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_class`
--

LOCK TABLES `item_class` WRITE;
/*!40000 ALTER TABLE `item_class` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_class` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_class_locale`
--

DROP TABLE IF EXISTS `item_class_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_class_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_class_locale`
--

LOCK TABLES `item_class_locale` WRITE;
/*!40000 ALTER TABLE `item_class_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_class_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_disenchant_loot`
--

DROP TABLE IF EXISTS `item_disenchant_loot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_disenchant_loot`
--

LOCK TABLES `item_disenchant_loot` WRITE;
/*!40000 ALTER TABLE `item_disenchant_loot` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_disenchant_loot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_limit_category`
--

DROP TABLE IF EXISTS `item_limit_category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_limit_category` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `NameLang` text,
  `Quantity` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_limit_category`
--

LOCK TABLES `item_limit_category` WRITE;
/*!40000 ALTER TABLE `item_limit_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_limit_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_limit_category_locale`
--

DROP TABLE IF EXISTS `item_limit_category_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_limit_category_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `NameLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_limit_category_locale`
--

LOCK TABLES `item_limit_category_locale` WRITE;
/*!40000 ALTER TABLE `item_limit_category_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_limit_category_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_price_base`
--

DROP TABLE IF EXISTS `item_price_base`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_price_base` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Armor` float NOT NULL DEFAULT '0',
  `Weapon` float NOT NULL DEFAULT '0',
  `ItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_price_base`
--

LOCK TABLES `item_price_base` WRITE;
/*!40000 ALTER TABLE `item_price_base` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_price_base` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_random_properties`
--

DROP TABLE IF EXISTS `item_random_properties`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_random_properties` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Enchantment1` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment2` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment3` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment4` int(10) unsigned NOT NULL DEFAULT '0',
  `Enchantment5` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_random_properties`
--

LOCK TABLES `item_random_properties` WRITE;
/*!40000 ALTER TABLE `item_random_properties` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_random_properties` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_random_properties_locale`
--

DROP TABLE IF EXISTS `item_random_properties_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_random_properties_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_random_properties_locale`
--

LOCK TABLES `item_random_properties_locale` WRITE;
/*!40000 ALTER TABLE `item_random_properties_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_random_properties_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_random_suffix`
--

DROP TABLE IF EXISTS `item_random_suffix`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_random_suffix` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_random_suffix`
--

LOCK TABLES `item_random_suffix` WRITE;
/*!40000 ALTER TABLE `item_random_suffix` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_random_suffix` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_random_suffix_locale`
--

DROP TABLE IF EXISTS `item_random_suffix_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_random_suffix_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `IternalName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_random_suffix_locale`
--

LOCK TABLES `item_random_suffix_locale` WRITE;
/*!40000 ALTER TABLE `item_random_suffix_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_random_suffix_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_spec_override`
--

DROP TABLE IF EXISTS `item_spec_override`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_spec_override` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_spec_override`
--

LOCK TABLES `item_spec_override` WRITE;
/*!40000 ALTER TABLE `item_spec_override` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_spec_override` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_spec`
--

DROP TABLE IF EXISTS `item_spec`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_spec` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PrimaryStat` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SecondaryStat` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_spec`
--

LOCK TABLES `item_spec` WRITE;
/*!40000 ALTER TABLE `item_spec` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_spec` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_set_spell`
--

DROP TABLE IF EXISTS `item_set_spell`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_set_spell` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemSetID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ChrSpecID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Threshold` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_set_spell`
--

LOCK TABLES `item_set_spell` WRITE;
/*!40000 ALTER TABLE `item_set_spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_set_spell` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `guild_perk_spells`
--

DROP TABLE IF EXISTS `guild_perk_spells`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `guild_perk_spells` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `guild_perk_spells`
--

LOCK TABLES `guild_perk_spells` WRITE;
/*!40000 ALTER TABLE `guild_perk_spells` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_perk_spells` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `heirloom`
--

DROP TABLE IF EXISTS `heirloom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `heirloom`
--

LOCK TABLES `heirloom` WRITE;
/*!40000 ALTER TABLE `heirloom` DISABLE KEYS */;
/*!40000 ALTER TABLE `heirloom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `heirloom_locale`
--

DROP TABLE IF EXISTS `heirloom_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `heirloom_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `SourceText_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `heirloom_locale`
--

LOCK TABLES `heirloom_locale` WRITE;
/*!40000 ALTER TABLE `heirloom_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `heirloom_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `holidays`
--

DROP TABLE IF EXISTS `holidays`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `holidays`
--

LOCK TABLES `holidays` WRITE;
/*!40000 ALTER TABLE `holidays` DISABLE KEYS */;
/*!40000 ALTER TABLE `holidays` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `holidays_locale`
--

DROP TABLE IF EXISTS `holidays_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `holidays_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `TextureFilename_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `holidays_locale`
--

LOCK TABLES `holidays_locale` WRITE;
/*!40000 ALTER TABLE `holidays_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `holidays_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item`
--

DROP TABLE IF EXISTS `item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item`
--

LOCK TABLES `item` WRITE;
/*!40000 ALTER TABLE `item` DISABLE KEYS */;
/*!40000 ALTER TABLE `item` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_appearance`
--

DROP TABLE IF EXISTS `item_appearance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_appearance` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayID` int(10) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_appearance`
--

LOCK TABLES `item_appearance` WRITE;
/*!40000 ALTER TABLE `item_appearance` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_appearance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_modified_appearance`
--

DROP TABLE IF EXISTS `item_modified_appearance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_modified_appearance` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `AppearanceID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `AppearanceModID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VariationID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_modified_appearance`
--

LOCK TABLES `item_modified_appearance` WRITE;
/*!40000 ALTER TABLE `item_modified_appearance` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_modified_appearance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mail_template`
--

DROP TABLE IF EXISTS `mail_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail_template` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Content` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mail_template`
--

LOCK TABLES `mail_template` WRITE;
/*!40000 ALTER TABLE `mail_template` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail_template` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mail_template_locale`
--

DROP TABLE IF EXISTS `mail_template_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mail_template_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Content_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mail_template_locale`
--

LOCK TABLES `mail_template_locale` WRITE;
/*!40000 ALTER TABLE `mail_template_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `mail_template_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `modifier_tree`
--

DROP TABLE IF EXISTS `modifier_tree`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `modifier_tree` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Asset` int(10) unsigned NOT NULL DEFAULT '0',
  `SecondaryAsset` int(10) unsigned NOT NULL DEFAULT '0',
  `Parent` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Operator` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Amount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `modifier_tree`
--

LOCK TABLES `modifier_tree` WRITE;
/*!40000 ALTER TABLE `modifier_tree` DISABLE KEYS */;
/*!40000 ALTER TABLE `modifier_tree` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mount_capability`
--

DROP TABLE IF EXISTS `mount_capability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mount_capability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpeedModSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredAura` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredRidingSkill` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredArea` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredMap` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Unknown7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mount_capability`
--

LOCK TABLES `mount_capability` WRITE;
/*!40000 ALTER TABLE `mount_capability` DISABLE KEYS */;
/*!40000 ALTER TABLE `mount_capability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mount_type`
--

DROP TABLE IF EXISTS `mount_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mount_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mount_type`
--

LOCK TABLES `mount_type` WRITE;
/*!40000 ALTER TABLE `mount_type` DISABLE KEYS */;
/*!40000 ALTER TABLE `mount_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mount_type_x_capability`
--

DROP TABLE IF EXISTS `mount_type_x_capability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mount_type_x_capability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MountTypeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MountCapabilityID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mount_type_x_capability`
--

LOCK TABLES `mount_type_x_capability` WRITE;
/*!40000 ALTER TABLE `mount_type_x_capability` DISABLE KEYS */;
/*!40000 ALTER TABLE `mount_type_x_capability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `name_gen`
--

DROP TABLE IF EXISTS `name_gen`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `name_gen` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `RaceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `name_gen`
--

LOCK TABLES `name_gen` WRITE;
/*!40000 ALTER TABLE `name_gen` DISABLE KEYS */;
/*!40000 ALTER TABLE `name_gen` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `name_gen_locale`
--

DROP TABLE IF EXISTS `name_gen_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `name_gen_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `name_gen_locale`
--

LOCK TABLES `name_gen_locale` WRITE;
/*!40000 ALTER TABLE `name_gen_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `name_gen_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `names_profanity`
--

DROP TABLE IF EXISTS `names_profanity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `names_profanity` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Language` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `names_profanity`
--

LOCK TABLES `names_profanity` WRITE;
/*!40000 ALTER TABLE `names_profanity` DISABLE KEYS */;
/*!40000 ALTER TABLE `names_profanity` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `names_reserved`
--

DROP TABLE IF EXISTS `names_reserved`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `names_reserved` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `names_reserved`
--

LOCK TABLES `names_reserved` WRITE;
/*!40000 ALTER TABLE `names_reserved` DISABLE KEYS */;
/*!40000 ALTER TABLE `names_reserved` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `names_reserved_locale`
--

DROP TABLE IF EXISTS `names_reserved_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `names_reserved_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `LocaleMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `names_reserved_locale`
--

LOCK TABLES `names_reserved_locale` WRITE;
/*!40000 ALTER TABLE `names_reserved_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `names_reserved_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_faction_rew`
--

DROP TABLE IF EXISTS `quest_faction_rew`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_faction_rew` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_faction_rew`
--

LOCK TABLES `quest_faction_rew` WRITE;
/*!40000 ALTER TABLE `quest_faction_rew` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_faction_rew` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_p_o_i_point`
--

DROP TABLE IF EXISTS `quest_p_o_i_point`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_p_o_i_point` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestPOIBlobID` int(10) unsigned NOT NULL DEFAULT '0',
  `X` smallint(6) NOT NULL DEFAULT '0',
  `Y` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_p_o_i_point`
--

LOCK TABLES `quest_p_o_i_point` WRITE;
/*!40000 ALTER TABLE `quest_p_o_i_point` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_p_o_i_point` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_sort`
--

DROP TABLE IF EXISTS `quest_sort`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_sort` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SortName` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_sort`
--

LOCK TABLES `quest_sort` WRITE;
/*!40000 ALTER TABLE `quest_sort` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_sort` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_sort_locale`
--

DROP TABLE IF EXISTS `quest_sort_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_sort_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `SortName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_sort_locale`
--

LOCK TABLES `quest_sort_locale` WRITE;
/*!40000 ALTER TABLE `quest_sort_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_sort_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_v2`
--

DROP TABLE IF EXISTS `quest_v2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_v2` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `UniqueBitFlag` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_v2`
--

LOCK TABLES `quest_v2` WRITE;
/*!40000 ALTER TABLE `quest_v2` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_v2` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_xp`
--

DROP TABLE IF EXISTS `quest_xp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_xp` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_xp`
--

LOCK TABLES `quest_xp` WRITE;
/*!40000 ALTER TABLE `quest_xp` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_xp` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_line`
--

DROP TABLE IF EXISTS `quest_line`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_line` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LineName` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_line`
--

LOCK TABLES `quest_line` WRITE;
/*!40000 ALTER TABLE `quest_line` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_line` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_line_locale`
--

DROP TABLE IF EXISTS `quest_line_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_line_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `LineName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_line_locale`
--

LOCK TABLES `quest_line_locale` WRITE;
/*!40000 ALTER TABLE `quest_line_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_line_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_line_x_quest`
--

DROP TABLE IF EXISTS `quest_line_x_quest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_line_x_quest` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LineID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Pos` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_line_x_quest`
--

LOCK TABLES `quest_line_x_quest` WRITE;
/*!40000 ALTER TABLE `quest_line_x_quest` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_line_x_quest` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_money_reward`
--

DROP TABLE IF EXISTS `quest_money_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_money_reward` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Money1` int(10) unsigned NOT NULL DEFAULT '0',
  `Money2` int(10) unsigned NOT NULL DEFAULT '0',
  `Money3` int(10) unsigned NOT NULL DEFAULT '0',
  `Money4` int(10) unsigned NOT NULL DEFAULT '0',
  `Money5` int(10) unsigned NOT NULL DEFAULT '0',
  `Money6` int(10) unsigned NOT NULL DEFAULT '0',
  `Money7` int(10) unsigned NOT NULL DEFAULT '0',
  `Money8` int(10) unsigned NOT NULL DEFAULT '0',
  `Money9` int(10) unsigned NOT NULL DEFAULT '0',
  `Money10` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_money_reward`
--

LOCK TABLES `quest_money_reward` WRITE;
/*!40000 ALTER TABLE `quest_money_reward` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_money_reward` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_objective`
--

DROP TABLE IF EXISTS `quest_objective`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_objective` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Amount` int(11) NOT NULL DEFAULT '0',
  `ObjectID` int(11) NOT NULL DEFAULT '0',
  `Description` text,
  `QuestID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `StorageIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UNK` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_objective`
--

LOCK TABLES `quest_objective` WRITE;
/*!40000 ALTER TABLE `quest_objective` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_objective` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_objective_locale`
--

DROP TABLE IF EXISTS `quest_objective_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_objective_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_objective_locale`
--

LOCK TABLES `quest_objective_locale` WRITE;
/*!40000 ALTER TABLE `quest_objective_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_objective_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rand_prop_points`
--

DROP TABLE IF EXISTS `rand_prop_points`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rand_prop_points` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rand_prop_points`
--

LOCK TABLES `rand_prop_points` WRITE;
/*!40000 ALTER TABLE `rand_prop_points` DISABLE KEYS */;
/*!40000 ALTER TABLE `rand_prop_points` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `research_branch`
--

DROP TABLE IF EXISTS `research_branch`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `research_branch` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Texture` text,
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ResearchFieldID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `research_branch`
--

LOCK TABLES `research_branch` WRITE;
/*!40000 ALTER TABLE `research_branch` DISABLE KEYS */;
/*!40000 ALTER TABLE `research_branch` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `research_branch_locale`
--

DROP TABLE IF EXISTS `research_branch_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `research_branch_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Texture_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `research_branch_locale`
--

LOCK TABLES `research_branch_locale` WRITE;
/*!40000 ALTER TABLE `research_branch_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `research_branch_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `research_project`
--

DROP TABLE IF EXISTS `research_project`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `research_project` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `Rare` int(10) unsigned NOT NULL DEFAULT '0',
  `BranchID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `IconName` text,
  `Complexity` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyAmount` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `research_project`
--

LOCK TABLES `research_project` WRITE;
/*!40000 ALTER TABLE `research_project` DISABLE KEYS */;
/*!40000 ALTER TABLE `research_project` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `research_project_locale`
--

DROP TABLE IF EXISTS `research_project_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `research_project_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `IconName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `research_project_locale`
--

LOCK TABLES `research_project_locale` WRITE;
/*!40000 ALTER TABLE `research_project_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `research_project_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `research_site`
--

DROP TABLE IF EXISTS `research_site`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `research_site` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` int(10) unsigned NOT NULL DEFAULT '0',
  `POIid` int(10) unsigned NOT NULL DEFAULT '0',
  `areaName` text,
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `research_site`
--

LOCK TABLES `research_site` WRITE;
/*!40000 ALTER TABLE `research_site` DISABLE KEYS */;
/*!40000 ALTER TABLE `research_site` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `research_site_locale`
--

DROP TABLE IF EXISTS `research_site_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `research_site_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `areaName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `research_site_locale`
--

LOCK TABLES `research_site_locale` WRITE;
/*!40000 ALTER TABLE `research_site_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `research_site_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_bonus`
--

DROP TABLE IF EXISTS `item_bonus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_bonus` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Value1` int(11) NOT NULL DEFAULT '0',
  `Value2` int(11) NOT NULL DEFAULT '0',
  `BonusListID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Index` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_bonus`
--

LOCK TABLES `item_bonus` WRITE;
/*!40000 ALTER TABLE `item_bonus` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_bonus` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_bonus_tree_node`
--

DROP TABLE IF EXISTS `item_bonus_tree_node`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_bonus_tree_node` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SubTreeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BonusListID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BonusTreeModID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_bonus_tree_node`
--

LOCK TABLES `item_bonus_tree_node` WRITE;
/*!40000 ALTER TABLE `item_bonus_tree_node` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_bonus_tree_node` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_currency_cost`
--

DROP TABLE IF EXISTS `item_currency_cost`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_currency_cost` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_currency_cost`
--

LOCK TABLES `item_currency_cost` WRITE;
/*!40000 ALTER TABLE `item_currency_cost` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_currency_cost` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_effect`
--

DROP TABLE IF EXISTS `item_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_effect` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `Cooldown` int(11) NOT NULL DEFAULT '0',
  `CategoryCooldown` int(11) NOT NULL DEFAULT '0',
  `Charges` smallint(6) NOT NULL DEFAULT '0',
  `Category` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpecID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Trigger` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_effect`
--

LOCK TABLES `item_effect` WRITE;
/*!40000 ALTER TABLE `item_effect` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_effect` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_sparse`
--

DROP TABLE IF EXISTS `item_sparse`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_sparse` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkFloat1` float NOT NULL DEFAULT '0',
  `UnkFloat2` float NOT NULL DEFAULT '0',
  `BuyPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `SellPrice` int(10) unsigned NOT NULL DEFAULT '0',
  `AllowableClass` int(11) NOT NULL DEFAULT '0',
  `AllowableRace` int(11) NOT NULL DEFAULT '0',
  `RequiredSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxCount` int(11) NOT NULL DEFAULT '0',
  `Stackable` int(11) NOT NULL DEFAULT '0',
  `ItemStatAllocation` int(11) NOT NULL DEFAULT '0',
  `ItemStatSocketCostMultiplier` float NOT NULL DEFAULT '0',
  `RangedModRange` float NOT NULL DEFAULT '0',
  `Name` text,
  `Name2` text,
  `Name3` text,
  `Name4` text,
  `Description` text,
  `BagFamily` int(10) unsigned NOT NULL DEFAULT '0',
  `ArmorDamageModifier` float NOT NULL DEFAULT '0',
  `Duration` int(10) unsigned NOT NULL DEFAULT '0',
  `StatScalingFactor` float NOT NULL DEFAULT '0',
  `ItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredSkill` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillRank` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredReputationFaction` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemStatValue` smallint(6) NOT NULL DEFAULT '0',
  `ScalingStatDistribution` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Delay` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PageText` smallint(5) unsigned NOT NULL DEFAULT '0',
  `StartQuest` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LockID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RandomProperty` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RandomSuffix` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemSet` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Area` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Map` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SocketBonus` smallint(5) unsigned NOT NULL DEFAULT '0',
  `GemProperties` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemLimitCategory` smallint(5) unsigned NOT NULL DEFAULT '0',
  `HolidayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemNameDescriptionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Quality` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BuyCount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `InventoryType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredLevel` tinyint(4) NOT NULL DEFAULT '0',
  `RequiredHonorRank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredCityRank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredReputationRank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ContainerSlots` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemStatType` tinyint(4) NOT NULL DEFAULT '0',
  `DamageType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Bonding` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LanguageID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PageMaterial` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Material` tinyint(4) NOT NULL DEFAULT '0',
  `Sheath` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TotemCategory` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SocketColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CurrencySubstitutionID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CurrencySubstitutionCount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_sparse`
--

LOCK TABLES `item_sparse` WRITE;
/*!40000 ALTER TABLE `item_sparse` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_sparse` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_sparse_locale`
--

DROP TABLE IF EXISTS `item_sparse_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_sparse_locale`
--

LOCK TABLES `item_sparse_locale` WRITE;
/*!40000 ALTER TABLE `item_sparse_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_sparse_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_extended_cost`
--

DROP TABLE IF EXISTS `item_extended_cost`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_extended_cost` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItem5` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrencyCount5` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredItemCount5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredPersonalArenaRating` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredCurrency5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredArenaSlot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ItemPurchaseGroup` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredFactionId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequirementFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredFactionStanding` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_extended_cost`
--

LOCK TABLES `item_extended_cost` WRITE;
/*!40000 ALTER TABLE `item_extended_cost` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_extended_cost` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_ability`
--

DROP TABLE IF EXISTS `battle_pet_ability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_ability`
--

LOCK TABLES `battle_pet_ability` WRITE;
/*!40000 ALTER TABLE `battle_pet_ability` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_ability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_ability_locale`
--

DROP TABLE IF EXISTS `battle_pet_ability_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_ability_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `name_lang` text,
  `description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_ability_locale`
--

LOCK TABLES `battle_pet_ability_locale` WRITE;
/*!40000 ALTER TABLE `battle_pet_ability_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_ability_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_ability_effect`
--

DROP TABLE IF EXISTS `battle_pet_ability_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_ability_effect`
--

LOCK TABLES `battle_pet_ability_effect` WRITE;
/*!40000 ALTER TABLE `battle_pet_ability_effect` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_ability_effect` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_effect_properties`
--

DROP TABLE IF EXISTS `battle_pet_effect_properties`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_effect_properties`
--

LOCK TABLES `battle_pet_effect_properties` WRITE;
/*!40000 ALTER TABLE `battle_pet_effect_properties` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_effect_properties` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_effect_properties_locale`
--

DROP TABLE IF EXISTS `battle_pet_effect_properties_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_effect_properties_locale`
--

LOCK TABLES `battle_pet_effect_properties_locale` WRITE;
/*!40000 ALTER TABLE `battle_pet_effect_properties_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_effect_properties_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_ability_turn`
--

DROP TABLE IF EXISTS `battle_pet_ability_turn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_ability_turn`
--

LOCK TABLES `battle_pet_ability_turn` WRITE;
/*!40000 ALTER TABLE `battle_pet_ability_turn` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_ability_turn` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_ability_state`
--

DROP TABLE IF EXISTS `battle_pet_ability_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_ability_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `AbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_ability_state`
--

LOCK TABLES `battle_pet_ability_state` WRITE;
/*!40000 ALTER TABLE `battle_pet_ability_state` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_ability_state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_state`
--

DROP TABLE IF EXISTS `battle_pet_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk` int(10) unsigned NOT NULL DEFAULT '0',
  `stateName` text,
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_state`
--

LOCK TABLES `battle_pet_state` WRITE;
/*!40000 ALTER TABLE `battle_pet_state` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_state_locale`
--

DROP TABLE IF EXISTS `battle_pet_state_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_state_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `stateName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_state_locale`
--

LOCK TABLES `battle_pet_state_locale` WRITE;
/*!40000 ALTER TABLE `battle_pet_state_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_state_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_species`
--

DROP TABLE IF EXISTS `battle_pet_species`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_species`
--

LOCK TABLES `battle_pet_species` WRITE;
/*!40000 ALTER TABLE `battle_pet_species` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_species` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_species_locale`
--

DROP TABLE IF EXISTS `battle_pet_species_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_species_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `SourceText_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_species_locale`
--

LOCK TABLES `battle_pet_species_locale` WRITE;
/*!40000 ALTER TABLE `battle_pet_species_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_species_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_species_state`
--

DROP TABLE IF EXISTS `battle_pet_species_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_species_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpeciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `State` int(10) unsigned NOT NULL DEFAULT '0',
  `Value` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_species_state`
--

LOCK TABLES `battle_pet_species_state` WRITE;
/*!40000 ALTER TABLE `battle_pet_species_state` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_species_state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_species_x_ability`
--

DROP TABLE IF EXISTS `battle_pet_species_x_ability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_species_x_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `speciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `abilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredLevel` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_species_x_ability`
--

LOCK TABLES `battle_pet_species_x_ability` WRITE;
/*!40000 ALTER TABLE `battle_pet_species_x_ability` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_species_x_ability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_breed_quality`
--

DROP TABLE IF EXISTS `battle_pet_breed_quality`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_breed_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `quality` int(10) unsigned NOT NULL DEFAULT '0',
  `qualityModifier` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_breed_quality`
--

LOCK TABLES `battle_pet_breed_quality` WRITE;
/*!40000 ALTER TABLE `battle_pet_breed_quality` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_breed_quality` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `battle_pet_breed_state`
--

DROP TABLE IF EXISTS `battle_pet_breed_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `battle_pet_breed_state` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `breedID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateID` int(10) unsigned NOT NULL DEFAULT '0',
  `stateModifier` int(11) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `battle_pet_breed_state`
--

LOCK TABLES `battle_pet_breed_state` WRITE;
/*!40000 ALTER TABLE `battle_pet_breed_state` DISABLE KEYS */;
/*!40000 ALTER TABLE `battle_pet_breed_state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_ability`
--

DROP TABLE IF EXISTS `garr_ability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_ability`
--

LOCK TABLES `garr_ability` WRITE;
/*!40000 ALTER TABLE `garr_ability` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_ability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_ability_locale`
--

DROP TABLE IF EXISTS `garr_ability_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_ability_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_ability_locale`
--

LOCK TABLES `garr_ability_locale` WRITE;
/*!40000 ALTER TABLE `garr_ability_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_ability_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_ability_effect`
--

DROP TABLE IF EXISTS `garr_ability_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_ability_effect`
--

LOCK TABLES `garr_ability_effect` WRITE;
/*!40000 ALTER TABLE `garr_ability_effect` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_ability_effect` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_building`
--

DROP TABLE IF EXISTS `garr_building`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_building`
--

LOCK TABLES `garr_building` WRITE;
/*!40000 ALTER TABLE `garr_building` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_building` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_building_locale`
--

DROP TABLE IF EXISTS `garr_building_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_building_locale`
--

LOCK TABLES `garr_building_locale` WRITE;
/*!40000 ALTER TABLE `garr_building_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_building_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_building_plot_inst`
--

DROP TABLE IF EXISTS `garr_building_plot_inst`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_building_plot_inst`
--

LOCK TABLES `garr_building_plot_inst` WRITE;
/*!40000 ALTER TABLE `garr_building_plot_inst` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_building_plot_inst` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_class_spec`
--

DROP TABLE IF EXISTS `garr_class_spec`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_class_spec`
--

LOCK TABLES `garr_class_spec` WRITE;
/*!40000 ALTER TABLE `garr_class_spec` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_class_spec` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_class_spec_locale`
--

DROP TABLE IF EXISTS `garr_class_spec_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_class_spec_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `NameMale_lang` text,
  `NameFemale_lang` text,
  `NameGenderless_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_class_spec_locale`
--

LOCK TABLES `garr_class_spec_locale` WRITE;
/*!40000 ALTER TABLE `garr_class_spec_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_class_spec_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_follower`
--

DROP TABLE IF EXISTS `garr_follower`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_follower`
--

LOCK TABLES `garr_follower` WRITE;
/*!40000 ALTER TABLE `garr_follower` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_follower` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_follower_locale`
--

DROP TABLE IF EXISTS `garr_follower_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_follower_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `HordeSourceText_lang` text,
  `AllianceSourceText_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_follower_locale`
--

LOCK TABLES `garr_follower_locale` WRITE;
/*!40000 ALTER TABLE `garr_follower_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_follower_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_follower_level_x_p`
--

DROP TABLE IF EXISTS `garr_follower_level_x_p`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_follower_level_x_p` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(10) unsigned NOT NULL DEFAULT '0',
  `NextLevelXP` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_follower_level_x_p`
--

LOCK TABLES `garr_follower_level_x_p` WRITE;
/*!40000 ALTER TABLE `garr_follower_level_x_p` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_follower_level_x_p` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_follower_quality`
--

DROP TABLE IF EXISTS `garr_follower_quality`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_follower_quality` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality` int(10) unsigned NOT NULL DEFAULT '0',
  `NextQualityXP` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_follower_quality`
--

LOCK TABLES `garr_follower_quality` WRITE;
/*!40000 ALTER TABLE `garr_follower_quality` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_follower_quality` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_follower_x_ability`
--

DROP TABLE IF EXISTS `garr_follower_x_ability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_follower_x_ability` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrFollowerID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrAbilityID` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_follower_x_ability`
--

LOCK TABLES `garr_follower_x_ability` WRITE;
/*!40000 ALTER TABLE `garr_follower_x_ability` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_follower_x_ability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_encounter`
--

DROP TABLE IF EXISTS `garr_encounter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_encounter`
--

LOCK TABLES `garr_encounter` WRITE;
/*!40000 ALTER TABLE `garr_encounter` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_encounter` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_encounter_locale`
--

DROP TABLE IF EXISTS `garr_encounter_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_encounter_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_encounter_locale`
--

LOCK TABLES `garr_encounter_locale` WRITE;
/*!40000 ALTER TABLE `garr_encounter_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_encounter_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_encounter_x_mechanic`
--

DROP TABLE IF EXISTS `garr_encounter_x_mechanic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_encounter_x_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrEncounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMechanicID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_encounter_x_mechanic`
--

LOCK TABLES `garr_encounter_x_mechanic` WRITE;
/*!40000 ALTER TABLE `garr_encounter_x_mechanic` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_encounter_x_mechanic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mechanic`
--

DROP TABLE IF EXISTS `garr_mechanic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mechanic` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Mod` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mechanic`
--

LOCK TABLES `garr_mechanic` WRITE;
/*!40000 ALTER TABLE `garr_mechanic` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mechanic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mechanic_type`
--

DROP TABLE IF EXISTS `garr_mechanic_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mechanic_type` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Category` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `SomeFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mechanic_type`
--

LOCK TABLES `garr_mechanic_type` WRITE;
/*!40000 ALTER TABLE `garr_mechanic_type` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mechanic_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mechanic_type_locale`
--

DROP TABLE IF EXISTS `garr_mechanic_type_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mechanic_type_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mechanic_type_locale`
--

LOCK TABLES `garr_mechanic_type_locale` WRITE;
/*!40000 ALTER TABLE `garr_mechanic_type_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mechanic_type_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mission`
--

DROP TABLE IF EXISTS `garr_mission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission`
--

LOCK TABLES `garr_mission` WRITE;
/*!40000 ALTER TABLE `garr_mission` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mission_locale`
--

DROP TABLE IF EXISTS `garr_mission_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `Location_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission_locale`
--

LOCK TABLES `garr_mission_locale` WRITE;
/*!40000 ALTER TABLE `garr_mission_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mission_reward`
--

DROP TABLE IF EXISTS `garr_mission_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission_reward`
--

LOCK TABLES `garr_mission_reward` WRITE;
/*!40000 ALTER TABLE `garr_mission_reward` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission_reward` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_mission_x_encounter`
--

DROP TABLE IF EXISTS `garr_mission_x_encounter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_mission_x_encounter` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrMissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrEncounterID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_mission_x_encounter`
--

LOCK TABLES `garr_mission_x_encounter` WRITE;
/*!40000 ALTER TABLE `garr_mission_x_encounter` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_mission_x_encounter` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_plot`
--

DROP TABLE IF EXISTS `garr_plot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_plot`
--

LOCK TABLES `garr_plot` WRITE;
/*!40000 ALTER TABLE `garr_plot` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_plot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_plot_locale`
--

DROP TABLE IF EXISTS `garr_plot_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_plot_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_plot_locale`
--

LOCK TABLES `garr_plot_locale` WRITE;
/*!40000 ALTER TABLE `garr_plot_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_plot_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_plot_building`
--

DROP TABLE IF EXISTS `garr_plot_building`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_plot_building` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrPlotID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrBuildingID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_plot_building`
--

LOCK TABLES `garr_plot_building` WRITE;
/*!40000 ALTER TABLE `garr_plot_building` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_plot_building` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_plot_instance`
--

DROP TABLE IF EXISTS `garr_plot_instance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_plot_instance` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GarrPlotID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_plot_instance`
--

LOCK TABLES `garr_plot_instance` WRITE;
/*!40000 ALTER TABLE `garr_plot_instance` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_plot_instance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_plot_instance_locale`
--

DROP TABLE IF EXISTS `garr_plot_instance_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `garr_plot_instance_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_plot_instance_locale`
--

LOCK TABLES `garr_plot_instance_locale` WRITE;
/*!40000 ALTER TABLE `garr_plot_instance_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_plot_instance_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_site_level`
--

DROP TABLE IF EXISTS `garr_site_level`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_site_level`
--

LOCK TABLES `garr_site_level` WRITE;
/*!40000 ALTER TABLE `garr_site_level` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_site_level` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `garr_site_level_plot_inst`
--

DROP TABLE IF EXISTS `garr_site_level_plot_inst`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `garr_site_level_plot_inst`
--

LOCK TABLES `garr_site_level_plot_inst` WRITE;
/*!40000 ALTER TABLE `garr_site_level_plot_inst` DISABLE KEYS */;
/*!40000 ALTER TABLE `garr_site_level_plot_inst` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `creature_difficulty`
--

DROP TABLE IF EXISTS `creature_difficulty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `creature_difficulty` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags1` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags2` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags3` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags4` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags5` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Expansion` tinyint(4) NOT NULL DEFAULT '0',
  `MinLevel` tinyint(4) NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `creature_difficulty`
--

LOCK TABLES `creature_difficulty` WRITE;
/*!40000 ALTER TABLE `creature_difficulty` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature_difficulty` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `currency_types`
--

DROP TABLE IF EXISTS `currency_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `currency_types` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `InventoryIcon1` text,
  `InventoryIcon2` text,
  `MaxQty` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxEarnablePerWeek` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Description` text,
  `CategoryID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpellWeight` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpellCategory` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Quality` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `currency_types`
--

LOCK TABLES `currency_types` WRITE;
/*!40000 ALTER TABLE `currency_types` DISABLE KEYS */;
/*!40000 ALTER TABLE `currency_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `currency_types_locale`
--

DROP TABLE IF EXISTS `currency_types_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `currency_types_locale`
--

LOCK TABLES `currency_types_locale` WRITE;
/*!40000 ALTER TABLE `currency_types_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `currency_types_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quest_package_item`
--

DROP TABLE IF EXISTS `quest_package_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quest_package_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestPackageID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ItemCount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `FilterType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quest_package_item`
--

LOCK TABLES `quest_package_item` WRITE;
/*!40000 ALTER TABLE `quest_package_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_package_item` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mount`
--

DROP TABLE IF EXISTS `mount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mount` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Description` text,
  `SourceDescription` text,
  `MountTypeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PlayerConditionId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Source` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mount`
--

LOCK TABLES `mount` WRITE;
/*!40000 ALTER TABLE `mount` DISABLE KEYS */;
/*!40000 ALTER TABLE `mount` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mount_locale`
--

DROP TABLE IF EXISTS `mount_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mount_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Description_lang` text,
  `SourceDescription_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mount_locale`
--

LOCK TABLES `mount_locale` WRITE;
/*!40000 ALTER TABLE `mount_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `mount_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `language_words`
--

DROP TABLE IF EXISTS `language_words`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `language_words` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Word` text,
  `Lang` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `language_words`
--

LOCK TABLES `language_words` WRITE;
/*!40000 ALTER TABLE `language_words` DISABLE KEYS */;
/*!40000 ALTER TABLE `language_words` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `language_words_locale`
--

DROP TABLE IF EXISTS `language_words_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `language_words_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Word_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `language_words_locale`
--

LOCK TABLES `language_words_locale` WRITE;
/*!40000 ALTER TABLE `language_words_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `language_words_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `light`
--

DROP TABLE IF EXISTS `light`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `light` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `PosZ` float NOT NULL DEFAULT '0',
  `FalloffStart` float NOT NULL DEFAULT '0',
  `FalloffEnd` float NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `LightParamsID8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `light`
--

LOCK TABLES `light` WRITE;
/*!40000 ALTER TABLE `light` DISABLE KEYS */;
/*!40000 ALTER TABLE `light` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `key_chain`
--

DROP TABLE IF EXISTS `key_chain`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `key_chain`
--

LOCK TABLES `key_chain` WRITE;
/*!40000 ALTER TABLE `key_chain` DISABLE KEYS */;
/*!40000 ALTER TABLE `key_chain` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `override_spell_data`
--

DROP TABLE IF EXISTS `override_spell_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
  `PlayerActionbarFileDataID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `override_spell_data`
--

LOCK TABLES `override_spell_data` WRITE;
/*!40000 ALTER TABLE `override_spell_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `override_spell_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_aura_restrictions`
--

DROP TABLE IF EXISTS `spell_aura_restrictions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_aura_restrictions`
--

LOCK TABLES `spell_aura_restrictions` WRITE;
/*!40000 ALTER TABLE `spell_aura_restrictions` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_aura_restrictions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_casting_requirements`
--

DROP TABLE IF EXISTS `spell_casting_requirements`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_casting_requirements`
--

LOCK TABLES `spell_casting_requirements` WRITE;
/*!40000 ALTER TABLE `spell_casting_requirements` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_casting_requirements` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_class_options`
--

DROP TABLE IF EXISTS `spell_class_options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_class_options`
--

LOCK TABLES `spell_class_options` WRITE;
/*!40000 ALTER TABLE `spell_class_options` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_class_options` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_learn_spell`
--

DROP TABLE IF EXISTS `spell_learn_spell`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_learn_spell` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `LearnSpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `OverridesSpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_learn_spell`
--

LOCK TABLES `spell_learn_spell` WRITE;
/*!40000 ALTER TABLE `spell_learn_spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_learn_spell` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_misc`
--

DROP TABLE IF EXISTS `spell_misc`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_misc`
--

LOCK TABLES `spell_misc` WRITE;
/*!40000 ALTER TABLE `spell_misc` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_misc` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_misc_difficulty`
--

DROP TABLE IF EXISTS `spell_misc_difficulty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_misc_difficulty` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_misc_difficulty`
--

LOCK TABLES `spell_misc_difficulty` WRITE;
/*!40000 ALTER TABLE `spell_misc_difficulty` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_misc_difficulty` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_power`
--

DROP TABLE IF EXISTS `spell_power`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_power`
--

LOCK TABLES `spell_power` WRITE;
/*!40000 ALTER TABLE `spell_power` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_power` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_reagents`
--

DROP TABLE IF EXISTS `spell_reagents`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_reagents`
--

LOCK TABLES `spell_reagents` WRITE;
/*!40000 ALTER TABLE `spell_reagents` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_reagents` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_reagents_currency`
--

DROP TABLE IF EXISTS `spell_reagents_currency`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_reagents_currency` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrencyID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CurrencyCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_reagents_currency`
--

LOCK TABLES `spell_reagents_currency` WRITE;
/*!40000 ALTER TABLE `spell_reagents_currency` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_reagents_currency` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `toy`
--

DROP TABLE IF EXISTS `toy`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `toy` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Description` text,
  `CategoryFilter` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `toy`
--

LOCK TABLES `toy` WRITE;
/*!40000 ALTER TABLE `toy` DISABLE KEYS */;
/*!40000 ALTER TABLE `toy` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `toy_locale`
--

DROP TABLE IF EXISTS `toy_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `toy_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Description_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `toy_locale`
--

LOCK TABLES `toy_locale` WRITE;
/*!40000 ALTER TABLE `toy_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `toy_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pvp_item`
--

DROP TABLE IF EXISTS `pvp_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pvp_item` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusIlvl` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pvp_item`
--

LOCK TABLES `pvp_item` WRITE;
/*!40000 ALTER TABLE `pvp_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `pvp_item` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_totems`
--

DROP TABLE IF EXISTS `spell_totems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_totems`
--

LOCK TABLES `spell_totems` WRITE;
/*!40000 ALTER TABLE `spell_totems` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_totems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_x_spell_visual`
--

DROP TABLE IF EXISTS `spell_x_spell_visual`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_x_spell_visual`
--

LOCK TABLES `spell_x_spell_visual` WRITE;
/*!40000 ALTER TABLE `spell_x_spell_visual` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_x_spell_visual` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taxi_nodes`
--

DROP TABLE IF EXISTS `taxi_nodes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taxi_nodes`
--

LOCK TABLES `taxi_nodes` WRITE;
/*!40000 ALTER TABLE `taxi_nodes` DISABLE KEYS */;
/*!40000 ALTER TABLE `taxi_nodes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taxi_nodes_locale`
--

DROP TABLE IF EXISTS `taxi_nodes_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `taxi_nodes_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taxi_nodes_locale`
--

LOCK TABLES `taxi_nodes_locale` WRITE;
/*!40000 ALTER TABLE `taxi_nodes_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `taxi_nodes_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taxi_path`
--

DROP TABLE IF EXISTS `taxi_path`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `taxi_path` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `From` int(10) unsigned NOT NULL DEFAULT '0',
  `To` int(10) unsigned NOT NULL DEFAULT '0',
  `Cost` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taxi_path`
--

LOCK TABLES `taxi_path` WRITE;
/*!40000 ALTER TABLE `taxi_path` DISABLE KEYS */;
/*!40000 ALTER TABLE `taxi_path` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taxi_path_node`
--

DROP TABLE IF EXISTS `taxi_path_node`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taxi_path_node`
--

LOCK TABLES `taxi_path_node` WRITE;
/*!40000 ALTER TABLE `taxi_path_node` DISABLE KEYS */;
/*!40000 ALTER TABLE `taxi_path_node` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_upgrade`
--

DROP TABLE IF EXISTS `item_upgrade`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_upgrade`
--

LOCK TABLES `item_upgrade` WRITE;
/*!40000 ALTER TABLE `item_upgrade` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_upgrade` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rule_set_item_upgrade`
--

DROP TABLE IF EXISTS `rule_set_item_upgrade`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rule_set_item_upgrade` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `levelUpd` int(10) unsigned NOT NULL DEFAULT '0',
  `startUpgrade` int(10) unsigned NOT NULL DEFAULT '0',
  `itemEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rule_set_item_upgrade`
--

LOCK TABLES `rule_set_item_upgrade` WRITE;
/*!40000 ALTER TABLE `rule_set_item_upgrade` DISABLE KEYS */;
/*!40000 ALTER TABLE `rule_set_item_upgrade` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gameobjects`
--

DROP TABLE IF EXISTS `gameobjects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gameobjects` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PositionX` float NOT NULL DEFAULT '0',
  `PositionY` float NOT NULL DEFAULT '0',
  `PositionZ` float NOT NULL DEFAULT '0',
  `RotationX` float NOT NULL DEFAULT '0',
  `RotationY` float NOT NULL DEFAULT '0',
  `RotationZ` float NOT NULL DEFAULT '0',
  `RotationO` float NOT NULL DEFAULT '0',
  `Size` float NOT NULL DEFAULT '0',
  `Data1` int(11) NOT NULL DEFAULT '0',
  `Data2` int(11) NOT NULL DEFAULT '0',
  `Data3` int(11) NOT NULL DEFAULT '0',
  `Data4` int(11) NOT NULL DEFAULT '0',
  `Data5` int(11) NOT NULL DEFAULT '0',
  `Data6` int(11) NOT NULL DEFAULT '0',
  `Data7` int(11) NOT NULL DEFAULT '0',
  `Data8` int(11) NOT NULL DEFAULT '0',
  `Name` text,
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DisplayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PhaseID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PhaseUseFlags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PhaseGroupID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gameobjects`
--

LOCK TABLES `gameobjects` WRITE;
/*!40000 ALTER TABLE `gameobjects` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobjects` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gameobjects_locale`
--

DROP TABLE IF EXISTS `gameobjects_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gameobjects_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gameobjects_locale`
--

LOCK TABLES `gameobjects_locale` WRITE;
/*!40000 ALTER TABLE `gameobjects_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `gameobjects_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `game_tables`
--

DROP TABLE IF EXISTS `game_tables`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_tables` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `NumRows` smallint(5) unsigned NOT NULL DEFAULT '0',
  `NumColumns` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `game_tables`
--

LOCK TABLES `game_tables` WRITE;
/*!40000 ALTER TABLE `game_tables` DISABLE KEYS */;
/*!40000 ALTER TABLE `game_tables` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `game_tables_locale`
--

DROP TABLE IF EXISTS `game_tables_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_tables_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `game_tables_locale`
--

LOCK TABLES `game_tables_locale` WRITE;
/*!40000 ALTER TABLE `game_tables_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `game_tables_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `map_challenge_mode`
--

DROP TABLE IF EXISTS `map_challenge_mode`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `map_challenge_mode`
--

LOCK TABLES `map_challenge_mode` WRITE;
/*!40000 ALTER TABLE `map_challenge_mode` DISABLE KEYS */;
/*!40000 ALTER TABLE `map_challenge_mode` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_visual`
--

DROP TABLE IF EXISTS `spell_visual`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_visual`
--

LOCK TABLES `spell_visual` WRITE;
/*!40000 ALTER TABLE `spell_visual` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_visual` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_x_bonus_tree`
--

DROP TABLE IF EXISTS `item_x_bonus_tree`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_x_bonus_tree` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID` int(10) unsigned NOT NULL DEFAULT '0',
  `BonusTreeID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_x_bonus_tree`
--

LOCK TABLES `item_x_bonus_tree` WRITE;
/*!40000 ALTER TABLE `item_x_bonus_tree` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_x_bonus_tree` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_effect`
--

DROP TABLE IF EXISTS `spell_effect`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_effect`
--

LOCK TABLES `spell_effect` WRITE;
/*!40000 ALTER TABLE `spell_effect` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_effect` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_shapeshift_form`
--

DROP TABLE IF EXISTS `spell_shapeshift_form`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
  `PresetSpellID1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID7` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PresetSpellID8` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CreatureType` tinyint(4) NOT NULL DEFAULT '0',
  `MountTypeID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ExitSoundEntriesID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_shapeshift_form`
--

LOCK TABLES `spell_shapeshift_form` WRITE;
/*!40000 ALTER TABLE `spell_shapeshift_form` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_shapeshift_form` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_shapeshift_form_locale`
--

DROP TABLE IF EXISTS `spell_shapeshift_form_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_shapeshift_form_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_shapeshift_form_locale`
--

LOCK TABLES `spell_shapeshift_form_locale` WRITE;
/*!40000 ALTER TABLE `spell_shapeshift_form_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_shapeshift_form_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_shapeshift`
--

DROP TABLE IF EXISTS `spell_shapeshift`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_shapeshift`
--

LOCK TABLES `spell_shapeshift` WRITE;
/*!40000 ALTER TABLE `spell_shapeshift` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_shapeshift` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_target_restrictions`
--

DROP TABLE IF EXISTS `spell_target_restrictions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_target_restrictions`
--

LOCK TABLES `spell_target_restrictions` WRITE;
/*!40000 ALTER TABLE `spell_target_restrictions` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_target_restrictions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell`
--

DROP TABLE IF EXISTS `spell`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `NameSubtext` text,
  `Description` text,
  `AuraDescription` text,
  `SpellMiscDifficultyID` int(10) unsigned NOT NULL DEFAULT '0',
  `DescriptionVariablesID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell`
--

LOCK TABLES `spell` WRITE;
/*!40000 ALTER TABLE `spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_locale`
--

DROP TABLE IF EXISTS `spell_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_locale`
--

LOCK TABLES `spell_locale` WRITE;
/*!40000 ALTER TABLE `spell_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_scaling`
--

DROP TABLE IF EXISTS `spell_scaling`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_scaling` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `ScalesFromItemLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ScalingClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxScalingLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_scaling`
--

LOCK TABLES `spell_scaling` WRITE;
/*!40000 ALTER TABLE `spell_scaling` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_scaling` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_range`
--

DROP TABLE IF EXISTS `spell_range`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_range`
--

LOCK TABLES `spell_range` WRITE;
/*!40000 ALTER TABLE `spell_range` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_range` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_range_locale`
--

DROP TABLE IF EXISTS `spell_range_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_range_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DisplayName_lang` text,
  `DisplayNameShort_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_range_locale`
--

LOCK TABLES `spell_range_locale` WRITE;
/*!40000 ALTER TABLE `spell_range_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_range_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_radius`
--

DROP TABLE IF EXISTS `spell_radius`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_radius` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Radius` float NOT NULL DEFAULT '0',
  `RadiusPerLevel` float NOT NULL DEFAULT '0',
  `RadiusMin` float NOT NULL DEFAULT '0',
  `RadiusMax` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_radius`
--

LOCK TABLES `spell_radius` WRITE;
/*!40000 ALTER TABLE `spell_radius` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_radius` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_procs_per_minute`
--

DROP TABLE IF EXISTS `spell_procs_per_minute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_procs_per_minute` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseProcRate` float NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_procs_per_minute`
--

LOCK TABLES `spell_procs_per_minute` WRITE;
/*!40000 ALTER TABLE `spell_procs_per_minute` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_procs_per_minute` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_procs_per_minute_mod`
--

DROP TABLE IF EXISTS `spell_procs_per_minute_mod`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_procs_per_minute_mod` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `PpmRateMod` float NOT NULL DEFAULT '0',
  `Type` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpecID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SpellProcsPerMinuteID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_procs_per_minute_mod`
--

LOCK TABLES `spell_procs_per_minute_mod` WRITE;
/*!40000 ALTER TABLE `spell_procs_per_minute_mod` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_procs_per_minute_mod` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_levels`
--

DROP TABLE IF EXISTS `spell_levels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_levels` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `BaseLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `SpellLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Unk701` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_levels`
--

LOCK TABLES `spell_levels` WRITE;
/*!40000 ALTER TABLE `spell_levels` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_levels` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_item_enchantment_condition`
--

DROP TABLE IF EXISTS `spell_item_enchantment_condition`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_item_enchantment_condition`
--

LOCK TABLES `spell_item_enchantment_condition` WRITE;
/*!40000 ALTER TABLE `spell_item_enchantment_condition` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_item_enchantment_condition` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_interrupts`
--

DROP TABLE IF EXISTS `spell_interrupts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_interrupts`
--

LOCK TABLES `spell_interrupts` WRITE;
/*!40000 ALTER TABLE `spell_interrupts` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_interrupts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_focus_object`
--

DROP TABLE IF EXISTS `spell_focus_object`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_focus_object` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_focus_object`
--

LOCK TABLES `spell_focus_object` WRITE;
/*!40000 ALTER TABLE `spell_focus_object` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_focus_object` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_focus_object_locale`
--

DROP TABLE IF EXISTS `spell_focus_object_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_focus_object_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_focus_object_locale`
--

LOCK TABLES `spell_focus_object_locale` WRITE;
/*!40000 ALTER TABLE `spell_focus_object_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_focus_object_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_equipped_items`
--

DROP TABLE IF EXISTS `spell_equipped_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_equipped_items` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `EquippedItemInventoryTypeMask` int(11) NOT NULL DEFAULT '0',
  `EquippedItemSubClassMask` int(11) NOT NULL DEFAULT '0',
  `EquippedItemClass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_equipped_items`
--

LOCK TABLES `spell_equipped_items` WRITE;
/*!40000 ALTER TABLE `spell_equipped_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_equipped_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_effect_scaling`
--

DROP TABLE IF EXISTS `spell_effect_scaling`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_effect_scaling` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Coefficient` float NOT NULL DEFAULT '0',
  `Variance` float NOT NULL DEFAULT '0',
  `ResourceCoefficient` float NOT NULL DEFAULT '0',
  `SpellEffectId` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_effect_scaling`
--

LOCK TABLES `spell_effect_scaling` WRITE;
/*!40000 ALTER TABLE `spell_effect_scaling` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_effect_scaling` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_duration`
--

DROP TABLE IF EXISTS `spell_duration`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_duration` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Duration` int(11) NOT NULL DEFAULT '0',
  `MaxDuration` int(11) NOT NULL DEFAULT '0',
  `DurationPerLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_duration`
--

LOCK TABLES `spell_duration` WRITE;
/*!40000 ALTER TABLE `spell_duration` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_duration` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_cooldowns`
--

DROP TABLE IF EXISTS `spell_cooldowns`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_cooldowns`
--

LOCK TABLES `spell_cooldowns` WRITE;
/*!40000 ALTER TABLE `spell_cooldowns` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_cooldowns` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_category`
--

DROP TABLE IF EXISTS `spell_category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_category` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `ChargeRecoveryTime` int(11) NOT NULL DEFAULT '0',
  `UnkLegion` int(11) NOT NULL DEFAULT '0',
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UsesPerWeek` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxCharges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_category`
--

LOCK TABLES `spell_category` WRITE;
/*!40000 ALTER TABLE `spell_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_category_locale`
--

DROP TABLE IF EXISTS `spell_category_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_category_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_category_locale`
--

LOCK TABLES `spell_category_locale` WRITE;
/*!40000 ALTER TABLE `spell_category_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_category_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_categories`
--

DROP TABLE IF EXISTS `spell_categories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_categories`
--

LOCK TABLES `spell_categories` WRITE;
/*!40000 ALTER TABLE `spell_categories` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_categories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_cast_times`
--

DROP TABLE IF EXISTS `spell_cast_times`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_cast_times` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Base` int(11) NOT NULL DEFAULT '0',
  `Minimum` int(11) NOT NULL DEFAULT '0',
  `PerLevel` smallint(6) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_cast_times`
--

LOCK TABLES `spell_cast_times` WRITE;
/*!40000 ALTER TABLE `spell_cast_times` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_cast_times` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spell_aura_options`
--

DROP TABLE IF EXISTS `spell_aura_options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_aura_options`
--

LOCK TABLES `spell_aura_options` WRITE;
/*!40000 ALTER TABLE `spell_aura_options` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_aura_options` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `specialization_spell`
--

DROP TABLE IF EXISTS `specialization_spell`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `specialization_spell`
--

LOCK TABLES `specialization_spell` WRITE;
/*!40000 ALTER TABLE `specialization_spell` DISABLE KEYS */;
/*!40000 ALTER TABLE `specialization_spell` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `specialization_spell_locale`
--

DROP TABLE IF EXISTS `specialization_spell_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `specialization_spell_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DescriptionLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `specialization_spell_locale`
--

LOCK TABLES `specialization_spell_locale` WRITE;
/*!40000 ALTER TABLE `specialization_spell_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `specialization_spell_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skill_race_class_info`
--

DROP TABLE IF EXISTS `skill_race_class_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skill_race_class_info`
--

LOCK TABLES `skill_race_class_info` WRITE;
/*!40000 ALTER TABLE `skill_race_class_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill_race_class_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skill_line`
--

DROP TABLE IF EXISTS `skill_line`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skill_line`
--

LOCK TABLES `skill_line` WRITE;
/*!40000 ALTER TABLE `skill_line` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill_line` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skill_line_locale`
--

DROP TABLE IF EXISTS `skill_line_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skill_line_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DisplayName_lang` text,
  `Description_lang` text,
  `AlternateVerb_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skill_line_locale`
--

LOCK TABLES `skill_line_locale` WRITE;
/*!40000 ALTER TABLE `skill_line_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill_line_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skill_line_ability`
--

DROP TABLE IF EXISTS `skill_line_ability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skill_line_ability`
--

LOCK TABLES `skill_line_ability` WRITE;
/*!40000 ALTER TABLE `skill_line_ability` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill_line_ability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scenario`
--

DROP TABLE IF EXISTS `scenario`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scenario` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scenario`
--

LOCK TABLES `scenario` WRITE;
/*!40000 ALTER TABLE `scenario` DISABLE KEYS */;
/*!40000 ALTER TABLE `scenario` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scenario_locale`
--

DROP TABLE IF EXISTS `scenario_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scenario_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scenario_locale`
--

LOCK TABLES `scenario_locale` WRITE;
/*!40000 ALTER TABLE `scenario_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `scenario_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scenario_step`
--

DROP TABLE IF EXISTS `scenario_step`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scenario_step` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scenario_step`
--

LOCK TABLES `scenario_step` WRITE;
/*!40000 ALTER TABLE `scenario_step` DISABLE KEYS */;
/*!40000 ALTER TABLE `scenario_step` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scenario_step_locale`
--

DROP TABLE IF EXISTS `scenario_step_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scenario_step_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `DescriptionLang_lang` text,
  `TitleLang_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scenario_step_locale`
--

LOCK TABLES `scenario_step_locale` WRITE;
/*!40000 ALTER TABLE `scenario_step_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `scenario_step_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scaling_stat_distribution`
--

DROP TABLE IF EXISTS `scaling_stat_distribution`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scaling_stat_distribution` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemLevelCurveID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scaling_stat_distribution`
--

LOCK TABLES `scaling_stat_distribution` WRITE;
/*!40000 ALTER TABLE `scaling_stat_distribution` DISABLE KEYS */;
/*!40000 ALTER TABLE `scaling_stat_distribution` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `totem_category`
--

DROP TABLE IF EXISTS `totem_category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `totem_category` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `TotemCategoryType` int(10) unsigned NOT NULL DEFAULT '0',
  `TotemCategoryMask` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `totem_category`
--

LOCK TABLES `totem_category` WRITE;
/*!40000 ALTER TABLE `totem_category` DISABLE KEYS */;
/*!40000 ALTER TABLE `totem_category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `totem_category_locale`
--

DROP TABLE IF EXISTS `totem_category_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `totem_category_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `totem_category_locale`
--

LOCK TABLES `totem_category_locale` WRITE;
/*!40000 ALTER TABLE `totem_category_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `totem_category_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_to_battle_pet_species`
--

DROP TABLE IF EXISTS `item_to_battle_pet_species`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_to_battle_pet_species` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `BattlePetSpeciesID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_to_battle_pet_species`
--

LOCK TABLES `item_to_battle_pet_species` WRITE;
/*!40000 ALTER TABLE `item_to_battle_pet_species` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_to_battle_pet_species` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transport_animation`
--

DROP TABLE IF EXISTS `transport_animation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `transport_animation` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `TransportID` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `PosX` float NOT NULL DEFAULT '0',
  `PosY` float NOT NULL DEFAULT '0',
  `PosZ` float NOT NULL DEFAULT '0',
  `SequenceID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transport_animation`
--

LOCK TABLES `transport_animation` WRITE;
/*!40000 ALTER TABLE `transport_animation` DISABLE KEYS */;
/*!40000 ALTER TABLE `transport_animation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unit_power_bar`
--

DROP TABLE IF EXISTS `unit_power_bar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unit_power_bar` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unit_power_bar`
--

LOCK TABLES `unit_power_bar` WRITE;
/*!40000 ALTER TABLE `unit_power_bar` DISABLE KEYS */;
/*!40000 ALTER TABLE `unit_power_bar` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unit_power_bar_locale`
--

DROP TABLE IF EXISTS `unit_power_bar_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unit_power_bar_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Cost_lang` text,
  `OutOfError_lang` text,
  `ToolTip_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unit_power_bar_locale`
--

LOCK TABLES `unit_power_bar_locale` WRITE;
/*!40000 ALTER TABLE `unit_power_bar_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `unit_power_bar_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_map_overlay`
--

DROP TABLE IF EXISTS `world_map_overlay`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_map_overlay` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_map_overlay`
--

LOCK TABLES `world_map_overlay` WRITE;
/*!40000 ALTER TABLE `world_map_overlay` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_map_overlay` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_map_overlay_locale`
--

DROP TABLE IF EXISTS `world_map_overlay_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_map_overlay_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `TextureName_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_map_overlay_locale`
--

LOCK TABLES `world_map_overlay_locale` WRITE;
/*!40000 ALTER TABLE `world_map_overlay_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_map_overlay_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `world_map_transforms`
--

DROP TABLE IF EXISTS `world_map_transforms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_map_transforms` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `world_map_transforms`
--

LOCK TABLES `world_map_transforms` WRITE;
/*!40000 ALTER TABLE `world_map_transforms` DISABLE KEYS */;
/*!40000 ALTER TABLE `world_map_transforms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chat_channels`
--

DROP TABLE IF EXISTS `chat_channels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chat_channels` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Shortcut` text,
  `FactionGroup` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chat_channels`
--

LOCK TABLES `chat_channels` WRITE;
/*!40000 ALTER TABLE `chat_channels` DISABLE KEYS */;
/*!40000 ALTER TABLE `chat_channels` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chat_channels_locale`
--

DROP TABLE IF EXISTS `chat_channels_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chat_channels_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Shortcut_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chat_channels_locale`
--

LOCK TABLES `chat_channels_locale` WRITE;
/*!40000 ALTER TABLE `chat_channels_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `chat_channels_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_specialization`
--

DROP TABLE IF EXISTS `chr_specialization`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chr_specialization` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MasterySpellID1` int(10) unsigned NOT NULL DEFAULT '0',
  `MasterySpellID2` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown2` int(10) unsigned NOT NULL DEFAULT '0',
  `Unknown3` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `Name2` text,
  `Description` text,
  `BackgroundFile` text,
  `SpellIconID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ClassID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `OrderIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PetTalentType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Role` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `PrimaryStatOrder2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_specialization`
--

LOCK TABLES `chr_specialization` WRITE;
/*!40000 ALTER TABLE `chr_specialization` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_specialization` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chr_specialization_locale`
--

DROP TABLE IF EXISTS `chr_specialization_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chr_specialization_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `Name2_lang` text,
  `Description_lang` text,
  `BackgroundFile_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chr_specialization_locale`
--

LOCK TABLES `chr_specialization_locale` WRITE;
/*!40000 ALTER TABLE `chr_specialization_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `chr_specialization_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `emotes`
--

DROP TABLE IF EXISTS `emotes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emotes` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `EmoteSlashCommand` text,
  `EmoteFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellVisualKitID` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion2` int(10) unsigned NOT NULL DEFAULT '0',
  `AnimID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteSoundID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EmoteSpecProc` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EmoteSpecProcParam` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `emotes`
--

LOCK TABLES `emotes` WRITE;
/*!40000 ALTER TABLE `emotes` DISABLE KEYS */;
/*!40000 ALTER TABLE `emotes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `emotes_locale`
--

DROP TABLE IF EXISTS `emotes_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emotes_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `EmoteSlashCommand_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `emotes_locale`
--

LOCK TABLES `emotes_locale` WRITE;
/*!40000 ALTER TABLE `emotes_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `emotes_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gem_properties`
--

DROP TABLE IF EXISTS `gem_properties`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gem_properties` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` smallint(5) unsigned NOT NULL DEFAULT '0',
  `EnchantID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gem_properties`
--

LOCK TABLES `gem_properties` WRITE;
/*!40000 ALTER TABLE `gem_properties` DISABLE KEYS */;
/*!40000 ALTER TABLE `gem_properties` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `glyph_properties`
--

DROP TABLE IF EXISTS `glyph_properties`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `glyph_properties` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `TypeFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellIconID` int(10) unsigned NOT NULL DEFAULT '0',
  `GlyphExclusiveCategoryID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `glyph_properties`
--

LOCK TABLES `glyph_properties` WRITE;
/*!40000 ALTER TABLE `glyph_properties` DISABLE KEYS */;
/*!40000 ALTER TABLE `glyph_properties` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_set`
--

DROP TABLE IF EXISTS `item_set`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_set` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` text,
  `ItemID1` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID2` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID3` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID4` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID5` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID6` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID7` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID8` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID9` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID10` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID11` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID12` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID13` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID14` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID15` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID16` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemID17` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkLegion` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredSkill` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RequiredSkillValue` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_set`
--

LOCK TABLES `item_set` WRITE;
/*!40000 ALTER TABLE `item_set` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_set` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `item_set_locale`
--

DROP TABLE IF EXISTS `item_set_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_set_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `Name_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_set_locale`
--

LOCK TABLES `item_set_locale` WRITE;
/*!40000 ALTER TABLE `item_set_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `item_set_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lock`
--

DROP TABLE IF EXISTS `lock`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lock` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Type1` int(10) unsigned NOT NULL DEFAULT '0',
  `Type2` int(10) unsigned NOT NULL DEFAULT '0',
  `Type3` int(10) unsigned NOT NULL DEFAULT '0',
  `Type4` int(10) unsigned NOT NULL DEFAULT '0',
  `Type5` int(10) unsigned NOT NULL DEFAULT '0',
  `Type6` int(10) unsigned NOT NULL DEFAULT '0',
  `Type7` int(10) unsigned NOT NULL DEFAULT '0',
  `Type8` int(10) unsigned NOT NULL DEFAULT '0',
  `Index1` int(10) unsigned NOT NULL DEFAULT '0',
  `Index2` int(10) unsigned NOT NULL DEFAULT '0',
  `Index3` int(10) unsigned NOT NULL DEFAULT '0',
  `Index4` int(10) unsigned NOT NULL DEFAULT '0',
  `Index5` int(10) unsigned NOT NULL DEFAULT '0',
  `Index6` int(10) unsigned NOT NULL DEFAULT '0',
  `Index7` int(10) unsigned NOT NULL DEFAULT '0',
  `Index8` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill1` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill2` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill3` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill4` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill5` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill6` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill7` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill8` int(10) unsigned NOT NULL DEFAULT '0',
  `Action1` int(10) unsigned NOT NULL DEFAULT '0',
  `Action2` int(10) unsigned NOT NULL DEFAULT '0',
  `Action3` int(10) unsigned NOT NULL DEFAULT '0',
  `Action4` int(10) unsigned NOT NULL DEFAULT '0',
  `Action5` int(10) unsigned NOT NULL DEFAULT '0',
  `Action6` int(10) unsigned NOT NULL DEFAULT '0',
  `Action7` int(10) unsigned NOT NULL DEFAULT '0',
  `Action8` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lock`
--

LOCK TABLES `lock` WRITE;
/*!40000 ALTER TABLE `lock` DISABLE KEYS */;
/*!40000 ALTER TABLE `lock` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `movie`
--

DROP TABLE IF EXISTS `movie`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `movie` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `movie`
--

LOCK TABLES `movie` WRITE;
/*!40000 ALTER TABLE `movie` DISABLE KEYS */;
/*!40000 ALTER TABLE `movie` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `power_display`
--

DROP TABLE IF EXISTS `power_display`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `power_display` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `GlobalStringBaseTag` text,
  `PowerType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Red` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Green` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Blue` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `power_display`
--

LOCK TABLES `power_display` WRITE;
/*!40000 ALTER TABLE `power_display` DISABLE KEYS */;
/*!40000 ALTER TABLE `power_display` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `power_display_locale`
--

DROP TABLE IF EXISTS `power_display_locale`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `power_display_locale` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `GlobalStringBaseTag_lang` text,
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `power_display_locale`
--

LOCK TABLES `power_display_locale` WRITE;
/*!40000 ALTER TABLE `power_display_locale` DISABLE KEYS */;
/*!40000 ALTER TABLE `power_display_locale` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pvp_difficulty`
--

DROP TABLE IF EXISTS `pvp_difficulty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pvp_difficulty` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `MapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BracketID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pvp_difficulty`
--

LOCK TABLES `pvp_difficulty` WRITE;
/*!40000 ALTER TABLE `pvp_difficulty` DISABLE KEYS */;
/*!40000 ALTER TABLE `pvp_difficulty` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `summon_properties`
--

DROP TABLE IF EXISTS `summon_properties`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `summon_properties` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Category` int(10) unsigned NOT NULL DEFAULT '0',
  `Faction` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Slot` int(11) NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `summon_properties`
--

LOCK TABLES `summon_properties` WRITE;
/*!40000 ALTER TABLE `summon_properties` DISABLE KEYS */;
/*!40000 ALTER TABLE `summon_properties` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vehicle_seat`
--

DROP TABLE IF EXISTS `vehicle_seat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vehicle_seat` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `FlagsB` int(10) unsigned NOT NULL DEFAULT '0',
  `FlagsC` int(10) unsigned NOT NULL DEFAULT '0',
  `AttachmentOffsetX` float NOT NULL DEFAULT '0',
  `AttachmentOffsetY` float NOT NULL DEFAULT '0',
  `AttachmentOffsetZ` float NOT NULL DEFAULT '0',
  `EnterPreDelay` float NOT NULL DEFAULT '0',
  `EnterSpeed` float NOT NULL DEFAULT '0',
  `EnterGravity` float NOT NULL DEFAULT '0',
  `EnterMinDuration` float NOT NULL DEFAULT '0',
  `EnterMaxDuration` float NOT NULL DEFAULT '0',
  `EnterMinArcHeight` float NOT NULL DEFAULT '0',
  `EnterMaxArcHeight` float NOT NULL DEFAULT '0',
  `ExitPreDelay` float NOT NULL DEFAULT '0',
  `ExitSpeed` float NOT NULL DEFAULT '0',
  `ExitGravity` float NOT NULL DEFAULT '0',
  `ExitMinDuration` float NOT NULL DEFAULT '0',
  `ExitMaxDuration` float NOT NULL DEFAULT '0',
  `ExitMinArcHeight` float NOT NULL DEFAULT '0',
  `ExitMaxArcHeight` float NOT NULL DEFAULT '0',
  `PassengerYaw` float NOT NULL DEFAULT '0',
  `PassengerPitch` float NOT NULL DEFAULT '0',
  `PassengerRoll` float NOT NULL DEFAULT '0',
  `VehicleEnterAnimDelay` float NOT NULL DEFAULT '0',
  `VehicleExitAnimDelay` float NOT NULL DEFAULT '0',
  `CameraEnteringDelay` float NOT NULL DEFAULT '0',
  `CameraEnteringDuration` float NOT NULL DEFAULT '0',
  `CameraExitingDelay` float NOT NULL DEFAULT '0',
  `CameraExitingDuration` float NOT NULL DEFAULT '0',
  `CameraOffsetX` float NOT NULL DEFAULT '0',
  `CameraOffsetY` float NOT NULL DEFAULT '0',
  `CameraOffsetZ` float NOT NULL DEFAULT '0',
  `CameraPosChaseRate` float NOT NULL DEFAULT '0',
  `CameraFacingChaseRate` float NOT NULL DEFAULT '0',
  `CameraEnteringZoom` float NOT NULL DEFAULT '0',
  `CameraSeatZoomMin` float NOT NULL DEFAULT '0',
  `CameraSeatZoomMax` float NOT NULL DEFAULT '0',
  `UISkinFileDataID` smallint(6) NOT NULL DEFAULT '0',
  `EnterAnimStart` smallint(6) NOT NULL DEFAULT '0',
  `EnterAnimLoop` smallint(6) NOT NULL DEFAULT '0',
  `RideAnimStart` smallint(6) NOT NULL DEFAULT '0',
  `RideAnimLoop` smallint(6) NOT NULL DEFAULT '0',
  `RideUpperAnimStart` smallint(6) NOT NULL DEFAULT '0',
  `RideUpperAnimLoop` smallint(6) NOT NULL DEFAULT '0',
  `ExitAnimStart` smallint(6) NOT NULL DEFAULT '0',
  `ExitAnimLoop` smallint(6) NOT NULL DEFAULT '0',
  `ExitAnimEnd` smallint(6) NOT NULL DEFAULT '0',
  `VehicleEnterAnim` smallint(6) NOT NULL DEFAULT '0',
  `VehicleExitAnim` smallint(6) NOT NULL DEFAULT '0',
  `VehicleRideAnimLoop` smallint(6) NOT NULL DEFAULT '0',
  `EnterUISoundID` smallint(6) NOT NULL DEFAULT '0',
  `ExitUISoundID` smallint(6) NOT NULL DEFAULT '0',
  `EnterAnimKitID` smallint(6) NOT NULL DEFAULT '0',
  `RideAnimKitID` smallint(6) NOT NULL DEFAULT '0',
  `ExitAnimKitID` smallint(6) NOT NULL DEFAULT '0',
  `VehicleEnterAnimBone` smallint(6) NOT NULL DEFAULT '0',
  `VehicleExitAnimBone` smallint(6) NOT NULL DEFAULT '0',
  `VehicleRideAnimLoopBone` smallint(6) NOT NULL DEFAULT '0',
  `CameraModeID` smallint(6) NOT NULL DEFAULT '0',
  `AttachmentID` tinyint(4) NOT NULL DEFAULT '0',
  `PassengerAttachmentID` tinyint(4) NOT NULL DEFAULT '0',
  `VehicleAbilityDisplay` tinyint(4) NOT NULL DEFAULT '0',
  `VehicleEnterAnimKitID` tinyint(4) NOT NULL DEFAULT '0',
  `VehicleRideAnimKitID` tinyint(4) NOT NULL DEFAULT '0',
  `VehicleExitAnimKitID` tinyint(4) NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vehicle_seat`
--

LOCK TABLES `vehicle_seat` WRITE;
/*!40000 ALTER TABLE `vehicle_seat` DISABLE KEYS */;
/*!40000 ALTER TABLE `vehicle_seat` ENABLE KEYS */;
UNLOCK TABLES;

