REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('36142', '36138', '0', '0'),
('36182', '36142', '0', '0'),

('36192', '36189', '0', '0'),
('36194', '36192', '0', '0'),
('36195', '36194', '0', '0');

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('167962', 'spell_garr_lumberjack');

UPDATE `creature_template` SET `ScriptName` = 'mob_garr_lumberjack' WHERE `creature_template`.`entry` in (83950, 83985);

REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(83950, 0, 0, 'Ну, я пошел.', 12, 0, 100, 0, 0, 6286, 'Дровосек to Player'),
(83985, 0, 0, 'Дело сделано!', 12, 0, 100, 0, 0, 6199, 'Батрак to Player');

REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `content_loc9`, `content_loc10`) VALUES 
('22000', '|cffADFF2Ftimber: +$1i|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '|cffADFF2FДревесина: +$1i|r', NULL, NULL);

-- lumber mill lvl 2 36194 36142
REPLACE INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 36194, 0, 0, 43, 0, 4, 2, 0, 0, 0, '', 'req lumber mill lvl2'),
(19, 0, 36142, 0, 0, 43, 0, 4, 2, 0, 0, 0, '', 'req lumber mill lvl2'),
(19, 0, 36195, 0, 0, 43, 0, 4, 3, 0, 0, 0, '', 'req lumber mill lvl3'),
(19, 0, 36182, 0, 0, 43, 0, 4, 3, 0, 0, 0, '', 'req lumber mill lvl3');

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('167970', 'spell_garr_lumberjack_lvl2');
UPDATE `creature_template` SET `ScriptName` = 'mob_garr_lumberjack2' WHERE `creature_template`.`entry` in (84003, 84004);
DELETE FROM  `creature_template_addon` where entry in (84003, 84004);
UPDATE `creature_template` SET `InhabitType` = '3' WHERE `creature_template`.`entry` in (84003, 84004);
REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(84004, 0, 0, 'Слушай, иди занимайся своими делами, а я буду делать свою работу – валить деревья.', 12, 0, 100, 0, 0, 0, 'Дроворуб Орды to Player'),
(84004, 0, 1, 'Да, представь, это циркулярные пилы. А теперь в сторону! Мне тут дерево спилить надо!', 12, 0, 100, 0, 0, 0, 'Дроворуб Орды to Player'),
(84003, 0, 0, 'А тебе известно, что вероятность выживания боевых дровосеков в диких землях равна лишь двадцати процентам? ', 12, 0, 100, 0, 0, 0, 'Боевой дровосек to Player'),
(84003, 0, 1, '$n, разве тебе не надо еще мир спасать, а? Так что грязную работенку оставь мне!', 12, 0, 100, 0, 0, 0, 'Боевой дровосек to Player');

-- Q: A: 37088 H: 37062
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
('174569', 'spell_q37088_q37062');

REPLACE INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `auras`) VALUES
(87265, 0, 0x0, 0x1, '84186'), -- 87265 - 84186
(87290, 0, 0x0, 0x1, '84186'); -- 87290 - 84186

REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(87290, 0, 0, 'Извини, $n.', 12, 0, 100, 24, 0, 0, 'Динки Деньжатникс to Player'),
(87265, 0, 0, 'Извини, $n.', 12, 0, 100, 24, 0, 42439, 'Криксель Паросвист to Player');

-- Q: H: 37044 A: 37091
INSERT IGNORE INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `shared`) VALUES
(234146, 114983, 100, 1, 0, 1, 1, 0),
(233901, 114244, 100, 1, 0, 1, 1, 0),
(233900, 114246, 100, 1, 0, 1, 1, 0),
(234017, 114744, 100, 1, 0, 1, 1, 0),
(235078, 119158, 100, 1, 0, 1, 1, 0),
(233899, 114633, 100, 1, 0, 1, 1, 0),
(235126, 115530, 100, 1, 0, 1, 1, 0),
(234018, 114925, 100, 1, 0, 1, 1, 0),
(234019, 114924, 100, 1, 0, 1, 1, 0);

--
UPDATE `gameobject_loot_template` SET ChanceOrQuestChance = 0.1 WHERE `entry` in (239066, 239067);
