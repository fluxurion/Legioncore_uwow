ALTER TABLE `creature_template_wdb`
MODIFY COLUMN `Name1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Entry`,
MODIFY COLUMN `Name2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name1`,
MODIFY COLUMN `Name3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name2`,
MODIFY COLUMN `Name4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name3`,
MODIFY COLUMN `NameAlt1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name4`,
MODIFY COLUMN `NameAlt2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt1`,
MODIFY COLUMN `NameAlt3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt2`,
MODIFY COLUMN `NameAlt4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt3`,
MODIFY COLUMN `Title`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt4`,
MODIFY COLUMN `TitleAlt`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Title`,
MODIFY COLUMN `CursorName`  varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `TitleAlt`;

update creature_template_wdb set NameAlt1 = '' where NameAlt1 = '0';
update creature_template_wdb set Name1 = '' where Name1 = '0';
update creature_template_wdb set Title = '' where Title = '0';
update creature_template_wdb set TitleAlt = '' where TitleAlt = '0';
update creature_template_wdb set CursorName = '' where CursorName = '0';
