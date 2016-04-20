CREATE TABLE `gameobject_action`(  
  `entry` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  `ActionType` TINYINT(3) UNSIGNED NOT NULL DEFAULT 0,
  `SpellID` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  `WorldSafeLocID` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  `Distance` INT(10) UNSIGNED NOT NULL DEFAULT 5,
  `X` FLOAT UNSIGNED NOT NULL DEFAULT 0,
  `Y` FLOAT UNSIGNED NOT NULL DEFAULT 0,
  `Z` FLOAT UNSIGNED NOT NULL DEFAULT 0,
  `O` FLOAT UNSIGNED NOT NULL DEFAULT 0,
  `MapID` INT(10) UNSIGNED DEFAULT 0  NOT NULL,
  `comment` TEXT NOT NULL,
  PRIMARY KEY (`entry`, `ActionType`)
) ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;

insert  into `gameobject_action`(`entry`,`ActionType`,`SpellID`,`WorldSafeLocID`,`Distance`,`X`,`Y`,`Z`,`O`,`MapID`,`comment`) values (244534,0,192293,0,1,0,0,0,0,0,'Port to library in dalaran'),(244560,0,192295,0,1,0,0,0,0,0,'Port from library to dalaran'),(233436,0,0,5352,5,0,0,0,0,0,'To Black Rook Hold'),(232552,0,0,5353,5,0,0,0,0,0,'From Black Rook Hold'),(214885,0,0,5334,5,0,0,0,0,0,'To Darkheart Thicket');

