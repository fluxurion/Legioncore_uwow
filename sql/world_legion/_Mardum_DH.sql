-- Misc
UPDATE `creature_template` SET `ScriptName` = 'conversation_announcer' WHERE `entry` in (101748);

--
DELETE FROM `spell_scene` WHERE `SceneScriptPackageID` in (1487, 1451, 1460, 1468, 1470, 1512, 1469, 1479, 1423, 1480, 1493);
INSERT INTO `spell_scene` (`SceneScriptPackageID`, `MiscValue`, `PlaybackFlags`, MonsterCredit, trigerSpell, ScriptName, comment) VALUES
(1487, 1106, 20, 0, 0, 'sceneTrigger_dh_init', 'spell 193525 DH_INIT'), -- 1487 - 1106 crash on 701
(1493, 1116, 27, 0, 0, '', 'spell 191677'), -- 1493 - 1116
(1451, 1053, 20, 0, 0, '', 'spell 189261'), -- 1451 - 1053
(1460, 1061, 26, 0, 0, '', ''), -- 1460 - 1061
(1468, 1077, 20, 0, 0, '', ''), -- 1468 - 1077
(1470, 1079, 20, 0, 0, '', ''), -- 1470 - 1079
(1512, 1142, 20, 0, 0, '', ''), -- 1512 - 1142
(1469, 1078, 20, 0, 0, '', ''), -- 1469 - 1078
(1479, 1094, 20, 0, 0, '', ''), -- 1479 - 1094
(1423, 1016, 26, 0, 0, '', ''), -- 1423 - 1016
(1480, 1095, 20, 0, 0, '', ''); -- 1480 - 1095

DELETE FROM `spell_area` WHERE area in (7705);
REPLACE INTO `spell_area` (`area`, `spell`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
('7705', '179665', '0', '40077', '0', '0', '2', '1', '10', '64'), -- disable all mounts.
('7705', '193525', '0', '40077', '0', '0', '2', '1', '10', '74'), -- 1106
('7705', '191677', '0', '40077', '0', '0', '2', '0', '10', '64'), -- 1116
('7705', '191466', '40378', '0', '0', '0', '2', '0', '66', '64'); -- 1116

-- 179665 193525

-- Quest chaine fixes.
REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('40378', '40077', '0', '0'),
('39279', '40077', '0', '0'),
('39049', '40378', '0', '0'),
('38759', '40378', '0', '0'),
('40379', '40378', '0', '0');

-- Q: 40077
UPDATE `quest_objectives` SET `ObjectID` = '244898' WHERE `quest_objectives`.`ID` = 280292 AND `QuestID` = 40077;
REPLACE INTO `gameobject_quest_visual` (`goID`, `questID`, `incomplete_state_spell_visual`, `incomplete_state_world_effect`, `complete_state_spell_visual`, `complete_state_world_effect`, `Comment`)
 VALUES ('244898', '40077', '37794', '0', '8743', '0', 'Legion. Q: 40077');
UPDATE `gameobject_template` SET `ScriptName` = 'go_q40077' WHERE `gameobject_template`.`entry` = 244898;
UPDATE `creature_template` SET `ScriptName` = 'npc_q40077' WHERE `creature_template`.`entry` = 93011;

-- Q: 40378
UPDATE `gameobject_template` SET `ScriptName` = 'go_q40378' WHERE `gameobject_template`.`entry` = 241751;
REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) VALUES
 ('200255', '200175', '0', '5', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '-1', '0', '0', '');
UPDATE `creature_template` SET `ScriptName` = 'npc_q40378' WHERE `creature_template`.`entry` = 94410;

-- Q: 39279 Area.
REPLACE INTO `area_queststart` (`id`, `quest`) VALUES ('7741', '39279');
UPDATE `gameobject_template` SET `ScriptName` = 'go_q39279' WHERE `gameobject_template`.`entry` in (244439, 244440, 244441, 243873);
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('191827', 'spell_legion_q39279');

-- Q: 39049
REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(93105, 0, 0, 'Я вижу все твои тайны...', 12, 0, 100, 0, 0, 0, 'Инквизитор Зловеш to Player'),
(93105, 1, 0, 'Довольно! Сейчас мой инфернал сокрушит тебя. ', 14, 0, 100, 0, 0, 0, 'Инквизитор Зловеш to 0'),
(93105, 2, 0, 'А-а-а... Мои глаза!', 14, 0, 100, 0, 0, 0, 'Инквизитор Зловеш to Player');
UPDATE `creature_template` SET `ScriptName` = 'npc_q39049' WHERE `entry` = 93105;

-- Q: 38759
-- Q: 40379