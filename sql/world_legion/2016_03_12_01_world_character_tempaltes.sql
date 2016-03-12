DROP TABLE IF EXISTS `character_template`;
CREATE TABLE IF NOT EXISTS `character_template` (
  `ID` int(10) unsigned NOT NULL,
  `Name` varchar(70) NOT NULL,
  `Description` varchar(100) NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `character_template_class`;
CREATE TABLE IF NOT EXISTS `character_template_class` (
  `TemplateID` int(10) unsigned NOT NULL,
  `FactionGroup` tinyint(3) unsigned NOT NULL,
  `Class` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`TemplateID`,`FactionGroup`,`class`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `character_template` VALUES ('37', '', '', '100');

INSERT INTO `character_template_class` VALUES ('37', '3', '6');
INSERT INTO `character_template_class` VALUES ('37', '3', '12');
INSERT INTO `character_template_class` VALUES ('37', '3', '11');
INSERT INTO `character_template_class` VALUES ('37', '3', '3');
INSERT INTO `character_template_class` VALUES ('37', '3', '8');
INSERT INTO `character_template_class` VALUES ('37', '3', '10');
INSERT INTO `character_template_class` VALUES ('37', '3', '2');
INSERT INTO `character_template_class` VALUES ('37', '3', '5');
INSERT INTO `character_template_class` VALUES ('37', '3', '4');
INSERT INTO `character_template_class` VALUES ('37', '3', '7');
INSERT INTO `character_template_class` VALUES ('37', '3', '9');
INSERT INTO `character_template_class` VALUES ('37', '3', '1');
INSERT INTO `character_template_class` VALUES ('37', '5', '6');
INSERT INTO `character_template_class` VALUES ('37', '5', '12');
INSERT INTO `character_template_class` VALUES ('37', '5', '11');
INSERT INTO `character_template_class` VALUES ('37', '5', '3');
INSERT INTO `character_template_class` VALUES ('37', '5', '8');
INSERT INTO `character_template_class` VALUES ('37', '5', '10');
INSERT INTO `character_template_class` VALUES ('37', '5', '2');
INSERT INTO `character_template_class` VALUES ('37', '5', '5');
INSERT INTO `character_template_class` VALUES ('37', '5', '4');
INSERT INTO `character_template_class` VALUES ('37', '5', '7');
INSERT INTO `character_template_class` VALUES ('37', '5', '9');
INSERT INTO `character_template_class` VALUES ('37', '5', '1');

