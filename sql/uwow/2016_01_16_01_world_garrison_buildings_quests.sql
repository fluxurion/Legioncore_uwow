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
