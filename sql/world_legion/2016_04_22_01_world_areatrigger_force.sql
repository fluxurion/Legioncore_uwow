CREATE TABLE `areatrigger_force` (
  `CustomEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `AuraID` int(10) unsigned NOT NULL DEFAULT '0',
  `MayBeAuraID` int(10) unsigned NOT NULL DEFAULT '0',
  `windType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `windSpeed` float NOT NULL DEFAULT '0',
  `windX` float NOT NULL DEFAULT '0',
  `windY` float NOT NULL DEFAULT '0',
  `windZ` float NOT NULL DEFAULT '0',
  `centerX` float NOT NULL DEFAULT '0',
  `centerY` float NOT NULL DEFAULT '0',
  `centerZ` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`CustomEntry`,`AuraID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `areatrigger_force` (`CustomEntry`, `MayBeAuraID`, `AuraID`, `windType`, `windSpeed`, `windX`, `windY`, `windZ`, `centerX`, `centerY`, `centerZ`) VALUES
(9561, 191798, 191798, 0, 2, -8.742278E-08, 2, 0, -3486.264, 4386.87, -3.580416); -- 9561
