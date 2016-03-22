DROP TABLE IF EXISTS `character_honor`;
CREATE TABLE `character_honor` (
  `Guid` bigint(20) NOT NULL DEFAULT '0',
  `CurrentHonorAtLevel` mediumint(10) NOT NULL DEFAULT '0',
  `HonorLevel` smallint(5) NOT NULL DEFAULT '0',
  `PrestigeLevel` smallint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Honor info system';
