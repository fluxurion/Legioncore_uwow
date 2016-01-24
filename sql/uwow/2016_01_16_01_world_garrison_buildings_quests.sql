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

