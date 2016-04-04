/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Misc
UPDATE `creature_template` SET `ScriptName` = 'conversation_announcer' WHERE `entry` in (101748);

--
DELETE FROM `spell_scene` WHERE `SceneScriptPackageID` in (1487, 1451, 1460, 1468, 1470, 1512, 1469, 1479, 1423, 1480, 1493);
INSERT INTO `spell_scene` (`SceneScriptPackageID`, `MiscValue`, `PlaybackFlags`, MonsterCredit, trigerSpell, ScriptName, comment) VALUES
(1487, 1106, 20, 0, 0, 'sceneTrigger_dh_init', 'spell 193525 DH_INIT'), -- 1487 - 1106 crash on 701
(1493, 1116, 27, 0, 0, '', 'spell 191677'), -- 1493 - 1116
(1451, 1053, 20, 0, 0, '', 'spell 189261'), -- 1451 - 1053
(1460, 1061, 26, 0, 0, '', 'spell 189767'), -- 1460 - 1061
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
('7705', '191466', '40378', '0', '0', '0', '2', '0', '66', '64'), -- 1116
('7705', '190793', '40379', '0', '0', '0', '2', '0', '10', '64'); -- 1077

-- Quest chaine fixes.
REPLACE INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`) VALUES 
('40378', '40077', '0', '0'),
('39279', '40077', '0', '0'),
('39049', '40378', '0', '-39050'),
('38759', '40378', '0', '-39050'),
('40379', '40378', '0', '-39050'),
('39050', '40379', '0', ''),
('38765', '39050', '0', '0'),
('38766', '39050', '0', '0');


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
DELETE FROM `creature_text` WHERE `Entry` = 93105;
REPLACE INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `comment`) VALUES
(93105, 0, 0, 'I can see your secrets...', 12, 0, 100, 0, 0, 55222, 94930, 'Инквизитор Зловеш to Player'),
(93105, 1, 0, 'Enough! My colossal infernal will crush you.', 14, 0, 100, 0, 0, 55224, 99554, 'Инквизитор Зловеш to 0'),
(93105, 2, 0, 'My eyes... My eyes!', 14, 0, 100, 0, 0, 55223, 96010, 'Инквизитор Зловеш to Player');
REPLACE INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(93105, 0, 0, 'Я вижу все твои тайны...'),
(93105, 1, 0, 'Довольно! Сейчас мой инфернал сокрушит тебя. '),
(93105, 2, 0, 'А-а-а... Мои глаза!');
UPDATE `creature_template` SET `ScriptName` = 'npc_q39049' WHERE `entry` = 93105;

-- Q: 38759
UPDATE `gameobject_template` SET `ScriptName` = 'go_q38759' WHERE `gameobject_template`.`entry` in (242989, 244916, 242987, 242990);
REPLACE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `shared`) VALUES 
('95226', '128227', '-100', '0', '0', '1', '1', '0');

REPLACE INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `comment`) VALUES
(93117, 0, 0, 'The pain!', 12, 0, 100, 0, 0, 55226, 96415, 'Изаль Белая Луна to Player'),
(93117, 1, 0, 'A slight setback. I assure you it will not happen again.', 12, 0, 100, 274, 0, 55227, 96644, 'Изаль Белая Луна to Player'),
(93230, 0, 0, 'So weak... hard to contain... my energy.', 12, 0, 100, 0, 0, 55310, 96407, 'Маннетрель Темная Звезда to Player'),
(93230, 1, 0, 'I almost lost the fight against the fel energy inside me.', 12, 0, 100, 6, 0, 55311, 96202, 'Маннетрель Темная Звезда to Player'),
(94377, 0, 0, 'I... will not... give in!', 14, 0, 100, 0, 0, 55090, 96405, 'Циана Ночная Глефа to Player'),
(94377, 1, 0, 'I will never be taken prisoner again. Never!', 12, 0, 100, 5, 0, 55089, 95081, 'Циана Ночная Глефа to Player'),
(94400, 0, 0, 'I will destroy all of you.', 12, 0, 100, 0, 0, 55052, 96411, 'Белат Клинок Рассвета to Player'),
(94400, 1, 0, 'I will kill every last demon in my way.', 12, 0, 100, 5, 0, 55053, 96643, 'Белат Клинок Рассвета to Player');

REPLACE INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(94400, 0, 0, 'Я вас всех уничтожу.'),
(94400, 1, 0, 'Я убью всех демонов на своем пути.'),
(94377, 0, 0, 'Нет... я… не... сдамся!'),
(94377, 1, 0, 'Я больше никогда не окажусь в клетке!'),
(93117, 0, 0, 'Больно!'),
(93117, 1, 0, 'Досадное недоразумение. Этого больше не повторится.'),
(93230, 0, 0, 'Я слабею... не могу... сдержать... силу...'),
(93230, 1, 0, 'Еще немного, и я бы поддался энергии Скверны.');

-- Q: 40379
UPDATE `gameobject_template` SET `ScriptName` = 'go_q40379' WHERE `gameobject_template`.`entry` = 241756;
UPDATE `creature_template` SET `ScriptName` = 'npc_q40379' WHERE `creature_template`.`entry` = 99914;

-- Q: 39050
UPDATE `quest_template_addon` SET `SourceSpellID` = '201114' WHERE `quest_template_addon`.`ID` = 39050;
UPDATE `gameobject_template` SET `ScriptName` = 'go_q39050' WHERE `gameobject_template`.`entry` = 243335;

-- Q: 38765
-- Q: 38766
