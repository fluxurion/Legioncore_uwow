update creature_template set AIName = 'SmartAI' where entry in (98370, 98368, 98538, 98366,  98275, 98243, 98280, 101549, 98691, 98792, 98810, 100485);

delete from smart_scripts where entryorguid in (98370, 98368, 98538, 98366, 98275, 98243, 98280, 101549, 98691, 98792, 98810, 100485);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(98370, 0, 0, 0, 0, 0, 100, 0, 3000, 5000, 3000, 5000, 11, 199663, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 4 - cast"),

(98538, 0, 0, 0, 0, 0, 100, 0, 2000, 3000, 2000, 3000, 11, 196916, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 3 - cast"),
(98538, 0, 1, 0, 0, 0, 100, 0, 10000, 11000, 10000, 11000, 11, 196830, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 11 - cast"),

(98366, 0, 0, 0, 0, 0, 100, 0, 20000, 21000, 20000, 21000, 11, 200084, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 21 - cast"),

(98275, 0, 0, 0, 0, 0, 100, 0, 3000, 3100, 3000, 3100, 11, 193633, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 2 - cast"),
(98275, 0, 1, 0, 0, 0, 100, 0, 16000, 17000, 16000, 17000, 11, 200343, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 16 - cast"),

(98243, 0, 0, 0, 0, 0, 100, 0, 14000, 15000, 15000, 16000, 11, 200261, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 16 - cast"),

(98280, 0, 0, 0, 0, 0, 100, 1, 100, 100, 9000, 10000, 11, 200260, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - on pull- cast"),
(98280, 0, 1, 0, 0, 0, 100, 0, 14000, 14000, 14000, 15000, 11, 200248, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 14 - cast"),

(101549, 0, 0, 0, 0, 0, 100, 0, 4000, 4000, 4000, 4000, 11, 200256, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "BR - ever 4 - cast"),

(100485, 0, 0, 0, 0, 0, 100, 0, 21000, 22000, 21000, 22000, 11, 197974, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 4 - cast"),

(98691, 0, 0, 0, 0, 0, 100, 0, 21000, 22000, 21000, 22000, 11, 200291, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 21 - cast"),

(98792, 0, 0, 0, 2, 0, 100, 1, 40, 60, 21000, 22000, 11, 200784, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "BR - on 60% - cast"),

(98810, 0, 0, 0, 0, 0, 100, 0, 19000, 21000, 21000, 22000, 11, 201139, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "BR - ever 21 - cast"),
(98810, 0, 1, 0, 2, 0, 100, 1, 19, 21, 21000, 22000, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "BR - on 21 - cast"),


(98368, 0, 0, 0, 0, 0, 100, 0, 20000, 25000, 20000, 25000, 11, 200105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "BR - after 20 - cast");


update creature_template set ScriptName = 'npc_soul_echoes_outro' where entry in (99857, 99426, 99858);
update creature_template set ScriptName = 'npc_illysanna_commandir' where entry in (98706);
UPDATE `creature` SET `position_z`='96', spawndist = '0' WHERE id in (98706);

delete from spell_dummy_trigger where spell_id in (200343, 200784);
replace INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
('200105', '200099', '4', '0', '0', '0', '0', '0', '0', '1', '0', '0', 'Жертвоприношение души'),
('200343', '200345', '2', '0', '0', '0', '0', '0', '0', '1', '0', '0', 'Залп стрел');

replace into `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `caster`, `target`, `hastype`, `hastalent`, `hastype2`, `hastalent2`, `chance`, `cooldown`, `duration`, `hitmask`, `removeMask`, `targetCountType`, `targetCount`, `actiontype`, `group`, `comment`) VALUES 
('200784', '201061', '1', '0', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '-1', '0', '0', 'Зелье');


delete from areatrigger_actions where entry in (5286);
insert into `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) values
('5286','0','1','0','4096','196829','0','0','0','0','0','0','0','Вращающаяся глефа'),
('5286','1','50','1','4096','196829','0','0','0','0','0','0','0','Вращающаяся глефа');


delete from creature_text where entry in (98542);
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `comment`) VALUES 
('98542', '0', '0', 'Consume! Devour!', '14', '0', '100', '0', '0', '0', '100957', 'Слияние душ to 0'),
('98542', '1', '0', 'The harvest has come!', '14', '0', '100', '0', '0', '0', '100995', 'Слияние душ to Player'),
('98542', '2', '0', 'Leave this meager vessel, and join us...', '14', '0', '100', '0', '0', '0', '100994', 'Слияние душ to Player'),
('98542', '3', '0', 'I feed on your essence...', '14', '0', '100', '0', '0', '0', '100993', 'Слияние душ to 0');

delete from conditions where SourceEntry in (205210, 205211, 205212);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 205212, 0, 0, 31, 0, 3, 103662, 0, 0, 0, '', NULL),
(13, 1, 205211, 0, 0, 31, 0, 3, 103662, 0, 0, 0, '', NULL),
(13, 1, 205210, 0, 0, 31, 0, 3, 103662, 0, 0, 0, '', NULL);

delete from spell_target_filter where spellId in (205210, 205211, 205212);
INSERT INTO`spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
('205212', '38', '4', '-1', '0', '0', '0', '0', '7', '1', '1', '0', '0', '0', 'Луч'),
('205211', '38', '2', '0', '0', '0', '0', '0', '7', '0', '1', '0', '0', '0', 'Луч'),
('205210', '38', '2', '0', '0', '0', '0', '0', '7', '0', '1', '0', '0', '0', 'Луч');

UPDATE `creature_template_wdb` SET `DisplayId1`='11686', `DisplayId2`='0' WHERE (`Entry`='103662');