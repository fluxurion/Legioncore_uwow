--
DELETE FROM `spell_scene` WHERE `SceneScriptPackageID` in (1487, 1451, 1460, 1468, 1470, 1512, 1469, 1479, 1423, 1480, 1493);
INSERT INTO `spell_scene` (`SceneScriptPackageID`, `MiscValue`, `PlaybackFlags`, MonsterCredit, trigerSpell, ScriptName, comment) VALUES
(1487, 1106, 20, 0, 0, 'sceneTrigger_dh_init', 'spell 193525 DH_INIT'), -- 1487 - 1106 crash on 701
(1451, 1053, 20, 0, 0, '', ''), -- 1451 - 1053
(1460, 1061, 26, 0, 0, '', ''), -- 1460 - 1061
(1468, 1077, 20, 0, 0, '', ''), -- 1468 - 1077
(1470, 1079, 20, 0, 0, '', ''), -- 1470 - 1079
(1512, 1142, 20, 0, 0, '', ''), -- 1512 - 1142
(1469, 1078, 20, 0, 0, '', ''), -- 1469 - 1078
(1479, 1094, 20, 0, 0, '', ''), -- 1479 - 1094
(1423, 1016, 26, 0, 0, '', ''), -- 1423 - 1016
(1480, 1095, 20, 0, 0, '', ''), -- 1480 - 1095
(1493, 1116, 27, 0, 0, '', 'spell 191677'); -- 1493 - 1116

DELETE FROM `spell_area` WHERE area in (7705);
REPLACE INTO `spell_area` (`area`, `spell`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('7705', '193525', '0', '40077', '0', '0', '2', '1', '10', '64');

-- 179665 193525

-- Q: 40077