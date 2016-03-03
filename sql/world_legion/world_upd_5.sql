ALTER TABLE `creature_template_wdb`
CHANGE COLUMN `Name` `Name1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Entry`,
CHANGE COLUMN `NameAlt` `NameAlt1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name1`,
ADD COLUMN `Name2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name1`,
ADD COLUMN `Name3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name2`,
ADD COLUMN `Name4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name3`,
ADD COLUMN `NameAlt2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `NameAlt1`,
ADD COLUMN `NameAlt3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `NameAlt2`,
ADD COLUMN `NameAlt4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `NameAlt3`;

ALTER TABLE `creature_template_wdb_locale`
MODIFY COLUMN `Title`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Locale`,
MODIFY COLUMN `TitleAlt`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Title`,
CHANGE COLUMN `Name` `Name1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `TitleAlt`,
CHANGE COLUMN `NameAlt` `NameAlt1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name1`,
ADD COLUMN `Name2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name1`,
ADD COLUMN `Name3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name2`,
ADD COLUMN `Name4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `Name3`,
ADD COLUMN `NameAlt2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `NameAlt1`,
ADD COLUMN `NameAlt3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `NameAlt2`,
ADD COLUMN `NameAlt4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' AFTER `NameAlt3`;

ALTER TABLE `gossip_menu_option`
ADD COLUMN `OptionBroadcastTextID`  int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `box_text`,
ADD COLUMN `BoxBroadcastTextID`  int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `OptionBroadcastTextID`;

ALTER TABLE `creature_text`
CHANGE COLUMN `entry` `Entry`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 FIRST ,
CHANGE COLUMN `groupid` `GroupID`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `Entry`,
CHANGE COLUMN `id` `ID`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `GroupID`,
CHANGE COLUMN `text` `Text`  longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL AFTER `ID`,
CHANGE COLUMN `type` `Type`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `Text`,
CHANGE COLUMN `language` `Language`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `Type`,
CHANGE COLUMN `probability` `Probability`  float NOT NULL DEFAULT 0 AFTER `Language`,
CHANGE COLUMN `emote` `Emote`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 AFTER `Probability`,
CHANGE COLUMN `duration` `Duration`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 AFTER `Emote`,
CHANGE COLUMN `sound` `Sound`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 AFTER `Duration`,
ADD COLUMN `BroadcastTextID`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 AFTER `Sound`;


