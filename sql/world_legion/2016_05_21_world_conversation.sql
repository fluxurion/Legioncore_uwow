--
-- Структура таблицы `conversation`
--

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

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `conversation`
--
ALTER TABLE `conversation`
  ADD PRIMARY KEY (`guid`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `conversation`
--
ALTER TABLE `conversation`
  MODIFY `guid` bigint(20) unsigned NOT NULL AUTO_INCREMENT;
  
SET @CONV_GUID = 1;
DELETE FROM conversation WHERE `guid` BETWEEN @CONV_GUID+0 AND @CONV_GUID+32;
INSERT IGNORE INTO conversation (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`) VALUES
-- (@CONV_GUID+0, 705, 1481, 0, 0, 1, NULL, '', 1180.64, 3284.96, 70.34794, 0), -- 705 (Area: 7705)
-- (@CONV_GUID+1, 922, 1481, 0, 0, 1, NULL, '', 1180.111, 3203.256, 51.44204, 0), -- 922 (Area: 7705)
-- (@CONV_GUID+2, 581, 1481, 0, 0, 1, NULL, '', 861.621, 2902.978, -24.526, 0), -- 581 (Area: 7741)
-- (@CONV_GUID+3, 558, 1481, 0, 0, 1, NULL, '', 935.8586, 2837.984, 1.290445, 0), -- 558 (Area: 7741)
(@CONV_GUID+4, 583, 1481, 0, 0, 1, NULL, '', 751.1985, 2457.983, -62.45122, 0), -- 583 (Area: 7740)
(@CONV_GUID+5, 735, 1481, 0, 0, 1, NULL, '', 1058.397, 2576.308, -37.35653, 0), -- 735 (Area: 7740)
(@CONV_GUID+6, 531, 1481, 0, 0, 1, NULL, '', 1561.098, 2623.586, 30.56277, 0), -- 531 (Area: 7742)
(@CONV_GUID+7, 747, 1481, 0, 0, 1, NULL, '', 1429.753, 2362.509, 61.22379, 0), -- 747 (Area: 7742)
(@CONV_GUID+8, 486, 1481, 0, 0, 1, NULL, '', 1455.89, 1764.582, 54.43871, 0), -- 486 (Area: 7712)
(@CONV_GUID+9, 569, 1481, 0, 0, 1, NULL, '', 1395.266, 1694.818, 55.31528, 0), -- 569 (Area: 7712)
(@CONV_GUID+10, 567, 1481, 0, 0, 1, NULL, '', 1148.444, 1255.214, 119.6508, 0), -- 567 (Area: 7821)
(@CONV_GUID+11, 573, 1481, 0, 0, 1, NULL, '', 1197.303, 1298.53, 99.46679, 0), -- 573 (Area: 7821)
(@CONV_GUID+12, 571, 1481, 0, 0, 1, NULL, '', 1239.783, 1224.877, 91.67918, 0), -- 571 (Area: 7821)
(@CONV_GUID+13, 567, 1481, 0, 0, 1, NULL, '', 1152.775, 1253.411, 120.0668, 0), -- 567 (Area: 7821)
(@CONV_GUID+14, 567, 1481, 0, 0, 1, NULL, '', 1148.444, 1255.214, 119.6508, 0), -- 567 (Area: 7821)
(@CONV_GUID+15, 585, 1481, 0, 0, 1, NULL, '', 1868.532, 1144.586, 79.74224, 0), -- 585 (Area: 7912)
(@CONV_GUID+16, 585, 1481, 0, 0, 1, NULL, '', 1875.826, 1145.406, 80.50592, 0), -- 585 (Area: 7912)
(@CONV_GUID+17, 585, 1481, 0, 0, 1, NULL, '', 1874.49, 1137.696, 80.93039, 0), -- 585 (Area: 7912)
(@CONV_GUID+18, 570, 1481, 0, 0, 1, NULL, '', 1818.793, 1511.559, 82.93542, 0), -- 570 (Area: 7747)
(@CONV_GUID+19, 555, 1481, 0, 0, 1, NULL, '', 1467.47, 1412.78, 243.96, 0), -- 555 (Area: 7749)
(@CONV_GUID+20, 540, 1481, 0, 0, 1, NULL, '', 1523.513, 1411.794, 243.9906, 0), -- 540 (Area: 7749)
(@CONV_GUID+21, 492, 1468, 0, 0, 1, NULL, '', 4325.94, -620.21, -281.41, 0), -- 492 (Area: 7873)
(@CONV_GUID+22, 506, 1468, 0, 0, 1, NULL, '', 4330.473, -589.0867, -281.9222, 0), -- 506 (Area: 7873)
(@CONV_GUID+23, 1181, 1468, 0, 0, 1, NULL, '', 4326.771, -410.8968, -281.7924, 0), -- 1181 (Area: 7872)
(@CONV_GUID+24, 527, 1468, 0, 0, 1, NULL, '', 4259.488, -301.5368, -284.4094, 0), -- 527 (Area: 7871)
(@CONV_GUID+25, 528, 1468, 0, 0, 1, NULL, '', 4036.731, -316.2841, -280.9993, 0), -- 528 (Area: 7819)
(@CONV_GUID+26, 529, 1468, 0, 0, 1, NULL, '', 4036.731, -316.2841, -280.9993, 0), -- 529 (Area: 7819)
(@CONV_GUID+27, 530, 1468, 0, 0, 1, NULL, '', 4059.872, -290.9392, -281.1898, 0), -- 530 (Area: 7819)
(@CONV_GUID+28, 1183, 1468, 0, 0, 1, NULL, '', 4460.598, -321.8893, -182.9056, 0), -- 1183 (Area: 7871)
(@CONV_GUID+29, 833, 1468, 0, 0, 1, NULL, '', 4454.906, -394.2793, 126.0001, 0), -- 833 (Area: 7866)
(@CONV_GUID+30, 1184, 1468, 0, 0, 1, NULL, '', 4460.886, -451.486, 254.1279, 0), -- 1184 (Area: 7866)
(@CONV_GUID+31, 536, 1468, 0, 0, 1, NULL, '', 4285.958, -453.686, 259.5194, 0), -- 536 (Area: 7865)
(@CONV_GUID+32, 538, 1468, 0, 0, 1, NULL, '', 4161.787, -855.9694, 291.6914, 0); -- 538 (Area: 0)