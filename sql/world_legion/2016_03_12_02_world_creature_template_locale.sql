ALTER TABLE `creature_template_wdb_locale`
MODIFY COLUMN `Title`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Locale`,
MODIFY COLUMN `TitleAlt`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Title`,
MODIFY COLUMN `Name1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `TitleAlt`,
MODIFY COLUMN `Name2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name1`,
MODIFY COLUMN `Name3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name2`,
MODIFY COLUMN `Name4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name3`,
MODIFY COLUMN `NameAlt1`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `Name4`,
MODIFY COLUMN `NameAlt2`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt1`,
MODIFY COLUMN `NameAlt3`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt2`,
MODIFY COLUMN `NameAlt4`  varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER `NameAlt3`;

update creature_template_wdb_locale set Title = '' where Title = '0';
update creature_template_wdb_locale set TitleAlt = '' where TitleAlt = '0';
update creature_template_wdb_locale set Name1 = '' where Name1 = '0';
update creature_template_wdb_locale set Name2 = '' where Name2 = '0';
update creature_template_wdb_locale set Name3 = '' where Name3 = '0';
update creature_template_wdb_locale set Name4 = '' where Name4 = '0';
update creature_template_wdb_locale set NameAlt1 = '' where NameAlt1 = '0';
update creature_template_wdb_locale set NameAlt2 = '' where NameAlt2 = '0';
update creature_template_wdb_locale set NameAlt3 = '' where NameAlt3 = '0';
update creature_template_wdb_locale set NameAlt4 = '' where NameAlt4 = '0';
