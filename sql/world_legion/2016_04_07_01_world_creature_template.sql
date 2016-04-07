ALTER TABLE `creature_template`   
  ADD COLUMN `ScaleLevelMin` TINYINT(3) UNSIGNED DEFAULT 0  NOT NULL AFTER `maxlevel`,
  ADD COLUMN `ScaleLevelMax` TINYINT(3) DEFAULT 0  NOT NULL AFTER `ScaleLevelMin`,
  ADD COLUMN `ScaleLevelDelta` TINYINT(3) DEFAULT 0  NOT NULL AFTER `ScaleLevelMax`,
  ADD COLUMN `ScaleLevelDuration` INT(10) UNSIGNED DEFAULT 0  NOT NULL AFTER `ScaleLevelDelta`,
  ADD COLUMN `ControllerID` INT(10) DEFAULT 0  NOT NULL AFTER `MeleeID`;

ALTER TABLE `creature_template`   
  CHANGE `ScaleLevelMin` `ScaleLevelMin` TINYINT(3) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `ScaleLevelMax` `ScaleLevelMax` TINYINT(3) UNSIGNED DEFAULT 0  NOT NULL,
  CHANGE `ScaleLevelDelta` `ScaleLevelDelta` TINYINT(3) UNSIGNED DEFAULT 0  NOT NULL;
