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


