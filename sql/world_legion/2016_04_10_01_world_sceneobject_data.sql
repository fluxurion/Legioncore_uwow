CREATE TABLE `sceneobject_data`(  
  `Entry` INT(10) NOT NULL DEFAULT 0,
  `PackageID` INT(10) NOT NULL DEFAULT 0,
  `RndSeedVal` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  `SceneType` INT(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`Entry`, `PackageID`)
) ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;

ALTER TABLE `sceneobject_data`   
  ADD COLUMN `CreatureID` INT(10) DEFAULT 0  NOT NULL AFTER `Entry`, 
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`Entry`, `CreatureID`, `PackageID`);
