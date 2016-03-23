CREATE TABLE `creature_action`(  
  `entry` INT(10) NOT NULL DEFAULT 0,
  `target` INT(10) NOT NULL DEFAULT 0,
  `type` TINYINT(3) NOT NULL DEFAULT 0,
  `spellId` INT(10) NOT NULL DEFAULT 0,
  `action` TINYINT(3) NOT NULL DEFAULT 0,
  PRIMARY KEY (`entry`, `target`, `type`)
);
