DELETE FROM `playercreateinfo` WHERE (`race`=10 AND `class`=12);
DELETE FROM `playercreateinfo` WHERE (`race`=4 AND `class`=12);
INSERT INTO `playercreateinfo` (`race`, `class`, `map`, `position_x`, `position_y`, `position_z`) VALUES
('4', '12', '1481', '1180.64', '3284.96', '70.5241'),
('10', '12', '1481', '1180.64', '3284.96', '70.5241');
INSERT INTO `player_levelstats` VALUES
('4', '12', '98', '10', '10', '10', '10', '10'),
('10', '12', '98', '10', '10', '10', '10', '10');
INSERT INTO `class_expansion_requirement` VALUES ('12', '0');

DROP TABLE IF EXISTS `creature_template_locale`;
CREATE TABLE IF NOT EXISTS `creature_template_locale` (
  `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  `Locale` VARCHAR(4) NOT NULL,
  `Name` TEXT,
  `NameAlt` TEXT,
  `Title` TEXT,
  `TitleAlt` TEXT,
  `VerifiedBuild` SMALLINT(5) DEFAULT '0'
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

ALTER TABLE `creature_template_locale`
  ADD PRIMARY KEY (`ID`,`Locale`);

INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "koKR", `name_loc1`, `subname_loc1`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "frFR", `name_loc2`, `subname_loc2`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "deDE", `name_loc3`, `subname_loc3`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "zhCN", `name_loc4`, `subname_loc4`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "zhTW", `name_loc5`, `subname_loc5`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "esES", `name_loc6`, `subname_loc6`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "esMX", `name_loc7`, `subname_loc7`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "ruRU", `name_loc8`, `subname_loc8`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "ptBR", `name_loc9`, `subname_loc9`  FROM `locales_creature`);
INSERT INTO `creature_template_locale` (`ID`, `Locale`, `Name`, `Title`)
  (SELECT `entry`, "itIT", `name_loc10`, `subname_loc10`  FROM `locales_creature`);

DROP TABLE IF EXISTS `locales_creature`;


-- ----------------------------
-- Table structure for `creature_template_wdb`
-- ----------------------------
DROP TABLE IF EXISTS `creature_template_wdb`;
CREATE TABLE `creature_template_wdb` (
  `Entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Name` char(255) NOT NULL DEFAULT '',
  `NameAlt` char(255) NOT NULL DEFAULT '',
  `Title` char(255) DEFAULT '',
  `TitleAlt` char(255) DEFAULT '',
  `CursorName` char(100) DEFAULT '',
  `TypeFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `TypeFlags2` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Family` mediumint(6) NOT NULL DEFAULT '0',
  `Classification` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `KillCredit1` int(10) unsigned NOT NULL DEFAULT '0',
  `KillCredit2` int(10) unsigned NOT NULL DEFAULT '0',
  `UnkInt` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayId1` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `DisplayId2` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `DisplayId3` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `DisplayId4` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `HpMulti` float NOT NULL DEFAULT '1',
  `PowerMulti` float NOT NULL DEFAULT '1',
  `Leader` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `QuestItem1` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestItem2` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestItem3` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestItem4` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestItem5` int(10) unsigned NOT NULL DEFAULT '0',
  `QuestItem6` int(10) unsigned NOT NULL DEFAULT '0',
  `MovementInfoID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `RequiredExpansion` smallint(2) NOT NULL DEFAULT '0',
  `FlagQuest` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) DEFAULT '1',
  PRIMARY KEY (`Entry`),
  KEY `idx_name` (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Creature System';

INSERT INTO `creature_template_wdb` (Entry, Name, NameAlt, Title, CursorName, Type, TypeFlags, TypeFlags2, RequiredExpansion, Family, Classification, MovementInfoID, HpMulti, PowerMulti, Leader,  KillCredit1, KillCredit2, DisplayId1, DisplayId2, DisplayId3, DisplayId4, QuestItem1, QuestItem2, QuestItem3, QuestItem4, QuestItem5, QuestItem6, VerifiedBuild)
  (SELECT entry, name, femaleName, subname, IconName, type, type_flags, type_flags2, exp_unk, family, rank, movementId, HealthModifier, ManaModifier, RacialLeader, KillCredit1, KillCredit2, modelid1, modelid2, modelid3, modelid4, questItem1, questItem2, questItem3, questItem4, questItem5, questItem6, VerifiedBuild  FROM `creature_template`);
  
ALTER TABLE `creature_template`
DROP COLUMN `name`,
DROP COLUMN `femaleName`,
DROP COLUMN `subname`,
DROP COLUMN `IconName`,
DROP COLUMN `type`,
DROP COLUMN `type_flags`,
DROP COLUMN `type_flags2`,
DROP COLUMN `exp_unk`,
DROP COLUMN `family`,
DROP COLUMN `rank`,
DROP COLUMN `movementId`,
DROP COLUMN `HealthModifier`,
DROP COLUMN `ManaModifier`,
DROP COLUMN `RacialLeader`,
DROP COLUMN `KillCredit1`,
DROP COLUMN `KillCredit2`,
DROP COLUMN `modelid1`,
DROP COLUMN `modelid2`,
DROP COLUMN `modelid3`,
DROP COLUMN `modelid4`,
DROP COLUMN `questItem1`,
DROP COLUMN `questItem2`,
DROP COLUMN `questItem3`,
DROP COLUMN `questItem4`,
DROP COLUMN `questItem5`,
DROP COLUMN `questItem6`,
DROP COLUMN `VerifiedBuild`;

DROP TABLE IF EXISTS `gossip_menu_option_locale`;
CREATE TABLE IF NOT EXISTS `gossip_menu_option_locale` (
  `MenuID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  `ID` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  `Locale` VARCHAR(4) NOT NULL,
  `OptionText` TEXT,
  `BoxText` TEXT,
  `VerifiedBuild` SMALLINT(5) DEFAULT '0'
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

ALTER TABLE `gossip_menu_option_locale`
  ADD PRIMARY KEY (`MenuID`,`ID`, `Locale`);

INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "koKR", `option_text_loc1`, `box_text_loc1`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "frFR", `option_text_loc2`, `box_text_loc2`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "deDE", `option_text_loc3`, `box_text_loc3`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "zhCN", `option_text_loc4`, `box_text_loc4`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "zhTW", `option_text_loc5`, `box_text_loc5`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "esES", `option_text_loc6`, `box_text_loc6`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "esMX", `option_text_loc7`, `box_text_loc7`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "ruRU", `option_text_loc8`, `box_text_loc8`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "ptBR", `option_text_loc9`, `box_text_loc9`  FROM `locales_gossip_menu_option`);
INSERT INTO `gossip_menu_option_locale` (`MenuID`, `ID`, `Locale`, `OptionText`, `BoxText`)
  (SELECT `menu_id`, `id`, "itIT", `option_text_loc10`, `box_text_loc10`  FROM `locales_gossip_menu_option`);

DROP TABLE IF EXISTS `locales_gossip_menu_option`;
