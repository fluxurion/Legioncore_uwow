ALTER TABLE `characters` ADD `playerBytes3` INT(11) NOT NULL DEFAULT '0' AFTER `playerBytes2`;
ALTER TABLE `characters` DROP `gender`;
ALTER TABLE `characters` DROP `drunk`;