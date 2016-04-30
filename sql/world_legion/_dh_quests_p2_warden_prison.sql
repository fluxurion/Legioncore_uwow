/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- ----------------
--   P A S E
-- ----------------

-- 5366 5367   - Q38672
REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES 
('7814', '1', '0', '5366 5367', '0', '0', '16', 'While not rew 38672'),
('7814', '2', '0', '5976', '0', '0', '16', 'rew 38672'),
('7814', '100', '0', '5982 5966 5965 5964 5409 5407 5401 5309 5158 5129 5121 5125 5124', '0', '0', '16', 'Legion. Global.'); --

DELETE FROM `conditions` WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 7814; 
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(23, 7814, 1, 0, 0, 8, 0, 38672, 0, 0, 1, 0, '', 'Draenor. FrostFireRidge. While non rew 38672'),
(23, 7814, 2, 0, 0, 8, 0, 38672, 0, 0, 0, 0, '', 'Draenor. FrostFireRidge. While rew 38672');

-- Quest chaine fixes.
REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('38668', '0', '0', '0'), -- hideen
('38669', '0', '0', '0'),
('38672', '38669', '0', '0'),
-- ToDo
('38723', '99999', '0', '0');


-- Q: 38669 GO 241553


-- Q: 38672 Breaking Out GO 244923 taker. npc 100168 trigger possible. just illusion of door.
UPDATE `gameobject_template` SET `flags` = '0' WHERE `gameobject_template`.`entry` = 244923;

UPDATE `quest_template` SET `StartScript` = '38672' WHERE `quest_template`.`ID` = 38672;
DELETE FROM `quest_start_scripts` WHERE id = 38672;
INSERT INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
('38672', '0', '8', '99326', '0', '0', '0', '0', '0', '0');