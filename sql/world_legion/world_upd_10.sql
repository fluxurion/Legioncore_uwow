ALTER TABLE `page_text`
CHANGE COLUMN `entry` `ID`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 FIRST ,
CHANGE COLUMN `text` `Text`  longtext CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `ID`,
CHANGE COLUMN `next_page` `NextPageID`  mediumint(8) UNSIGNED NOT NULL DEFAULT 0 AFTER `Text`,
CHANGE COLUMN `WDBVerified` `VerifiedBuild`  smallint(6) NULL DEFAULT 1 AFTER `NextPageID`;
