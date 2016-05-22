DROP TABLE IF EXISTS `conversation`;
CREATE TABLE `conversation` (
  `guid` bigint(20) unsigned NOT NULL,
  `id` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Conversation Identifier',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `zoneId` int(11) NOT NULL DEFAULT '0',
  `areaId` int(11) NOT NULL DEFAULT '0',
  `spawnMask` int(11) unsigned NOT NULL DEFAULT '1',
  `phaseMask` int(11) unsigned NOT NULL DEFAULT '1',
  `PhaseId` varchar(255) NOT NULL DEFAULT '',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Conversation System';