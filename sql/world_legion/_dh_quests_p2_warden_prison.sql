/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- ----------------
--   P A S E
-- ----------------
REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `PreloadMapID`, `VisibleMapID`, `flags`, `comment`) VALUES 
('7814', '100', '0', '5982 5966 5965 5964 5409 5407 5401 5309 5158 5129 5121', '0', '0', '16', 'Legion. Global.'); --


-- Quest chaine fixes.
REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('38669', '0', '0', '0'),
('38672', '38669', '0', '0'),
-- ToDo
('38723', '99999', '0', '0');


-- Q: 38669
-- Q: 38672