CREATE TABLE `display_choice`(  
  `ChoiceID` INT(10) NOT NULL DEFAULT 0,
  `ResponseID` INT(10) NOT NULL DEFAULT 0,
  `ChoiceArtFileID` INT(10) NOT NULL DEFAULT 0,
  `HasReward` BOOL NOT NULL DEFAULT 1,
  `Answer` TEXT NOT NULL,
  `Title` TEXT NOT NULL,
  `Description` TEXT NOT NULL,
  `Confirm` TEXT NOT NULL,
  `TitleID` INT(10) NOT NULL DEFAULT 0,
  `PackageID` INT(10) NOT NULL DEFAULT 0,
  `SkillLineID` INT(10) NOT NULL DEFAULT 0,
  `SkillPointCount` INT(10) NOT NULL DEFAULT 0,
  `ArenaPointCount` INT(10) NOT NULL DEFAULT 0,
  `HonorPointCount` INT(10) NOT NULL DEFAULT 0,
  `Money` INT(10) NOT NULL DEFAULT 0,
  `Xp` INT(10) NOT NULL DEFAULT 0,
  `Question` TEXT NOT NULL,
  `SpellID` INT(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`ChoiceID`, `ResponseID`)
) ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;

CREATE TABLE `display_choice_item`(  
  `ResponseID` INT(10) NOT NULL DEFAULT 0,
  `Type` TINYINT(3) NOT NULL DEFAULT 0,
  `Id` INT(10) NOT NULL DEFAULT 0,
  `DisplayID` INT(10) NOT NULL DEFAULT 0,
  `Unk` INT(10) NOT NULL DEFAULT 0,
  `Quantity` INT(10) DEFAULT 0,
  PRIMARY KEY (`ResponseID`, `Type`, `Id`)
) ENGINE=MYISAM CHARSET=utf8 COLLATE=utf8_general_ci;
