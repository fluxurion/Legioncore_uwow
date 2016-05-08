delete from world_visible_distance where type = 0 and id = 1493;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(0, 1493, 150, 'Dungeon: Vault of the Wardens');

/* -----------------------------------------
    Tirathon Saltheril  /  Тиратон Салтерил
   ----------------------------------------- */

update creature set spawntimesecs = 86400 where id = 95885;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_tirathon_saltheril', flags_extra = 1 where entry = 95885;
update creature_template_wdb set classification = 3 where entry = 95885;

delete from areatrigger_actions where entry in (4854);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4854, 0, 1, 0, 4096, 191853, 0, 0, 0, 0, 0, 0, 0, 'Furious Flames Apply'),
(4854, 1, 42, 1, 4096, 191853, 0, 0, 0, 0, 0, 0, 0, 'Furious Flames Remove');

delete from spell_target_position where id in (199132);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(199132, 1493, 4284.97, -451.275, 259.655, 3.15);

delete from spell_dummy_trigger where spell_id in (191765);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(191765, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Swoop');

/* -------------------------------------------------
    Inquisitor Tormentorum  /  Инквизитор Истязарий
   ------------------------------------------------- */
delete from creature where id in (99704,99644,99645,99675,99676,99655,99657,99651,99678);
update gameobject set state = 1 where id in (246506, 246507, 246508, 246509, 246510, 246511, 246512, 246513, 246514, 246515, 246516, 246517);
update creature set spawntimesecs = 86400 where id = 96015;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_inquisitor_tormentorum', flags_extra = 1 where entry = 96015;
update creature_template_wdb set classification = 3 where entry = 96015;
update creature_template set faction = 16, minlevel = 110, maxlevel = 110, scalelevelmin = 110, scalelevelmax = 110, exp = 6 where entry in (99678,99651,99657,99655);

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (206472,196992);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 206472, 0, 0, 31, 0, 3, 104341, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 0, 31, 0, 5, 246506, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 1, 31, 0, 5, 246507, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 2, 31, 0, 5, 246508, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 3, 31, 0, 5, 246509, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 4, 31, 0, 5, 246510, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 5, 31, 0, 5, 246511, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 6, 31, 0, 5, 246512, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 7, 31, 0, 5, 246513, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 8, 31, 0, 5, 246514, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 9, 31, 0, 5, 246515, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 10, 31, 0, 5, 246516, 0, 0, 0, '', NULL),
(13, 1, 196992, 0, 11, 31, 0, 5, 246517, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (206472);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(206472, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Torment');

delete from spell_linked_spell where spell_trigger in (-200905);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `removeMask`, `comment`) VALUES 
(-200905, 200904, 0, 16, 'Sapped Soul');

delete from spell_script_names where spell_id in (200904,196992);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(200904, 'spell_tormentorum_sap_soul'),
(196992, 'spell_tormentorum_open_prisons');

delete from spell_proc_event where entry in (200904);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`, `effectmask`) VALUES 
(200904, 0, 0, 0, 0, 0, 0, 87056, 0, 0, 0, 0, 7);

/* ----------------------
    Ash'Golm  /  Вул'Кан
   ---------------------- */
delete from creature where id in (99240);
update creature set spawntimesecs = 86400 where id = 95886;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_ashgolm', flags_extra = 1 where entry = 95886;
update creature_template_wdb set classification = 3 where entry = 95886;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_ashgolm_ember' where entry = 99233;
update creature_template set ScriptName = 'npc_ashgolm_countermeasures' where entry = 99240;
update creature_template set inhabitType = 7 where entry = 99277;
update areatrigger_data set isMoving = 0 where entry in (4919,4915); -- нужный тип передвижения не реализован

delete from areatrigger_actions where entry in (4919);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `onDespawn`, `comment`) VALUES 
(4919, 0, 1, 0, 4096, 192519, 0, 0, 0, 0, 0, 0, 0, 1, 'Lava Wreath'),
(4919, 1, 1, 0, 262144, 202169, 0, 0, 0, 0, 0, 0, 99233, 1, 'Fired Up');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (195213,195227,201523,192517);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 195213, 0, 0, 31, 0, 3, 99277, 0, 0, 0, '', NULL),
(13, 2, 195227, 0, 0, 31, 0, 3, 99233, 0, 0, 0, '', NULL),
(13, 1, 195227, 0, 0, 31, 0, 3, 95886, 0, 0, 0, '', NULL),
(13, 64, 201523, 0, 0, 31, 0, 4, 0, 0, 0, 0, '', NULL),
(13, 256, 192517, 0, 0, 31, 0, 4, 0, 0, 0, 0, '', NULL),
(13, 32, 192517, 0, 0, 31, 0, 5, 247010, 0, 0, 0, '', NULL);

delete from spell_script_names where spell_id in (200354);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(200354, 'spell_ashgolm_frozen');

delete from spell_dummy_trigger where spell_id in (192622);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(192622, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Volcano');

delete from creature_summon_groups where summonerId in (95886);
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(95886, 0, 0, 0, 99240, 4244.63, -416.621, 106.63, 4.44357, 0, 0, 0, 8, 0),
(95886, 1, 0, 0, 99240, 4203.36, -451.139, 106.63, 0, 0, 0, 0, 8, 0),
(95886, 2, 0, 0, 99240, 4244.52, -486.082, 106.63, 1.82022, 0, 0, 0, 8, 0);

delete from spell_linked_spell where spell_trigger in (-201523, -192517);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-201523, 195188, 0, 'Freeze Room trash'),
(-192517, 193209, 0, 'Freeze Room boss');

/* ----------------------
    Glazer  /  Смотрящий
   ---------------------- */

delete from creature where id in (102310,98082);
update creature set spawntimesecs = 86400 where id = 95887;
update creature_template set mechanic_immune_mask = 617299967, inhabittype = 7, ScriptName = 'boss_glazer', flags_extra = 1 where entry = 95887;
update creature_template_wdb set classification = 3 where entry = 95887;
update creature_template set inhabittype = 7, ScriptName = 'npc_glazer_beam', flags_extra = 128 where entry = 102310;
update creature_template set ScriptName = 'npc_glazer_lens' where entry = 98082;
update creature_template set inhabittype = 7, flags_extra = 128 where entry = 98118;

delete from areatrigger_actions where entry in (5130,5139);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `onDespawn`, `comment`) VALUES 
(5130, 0, 1, 0, 4096, 194880, 1, 0, 0, 0, 0, 0, 0, 0, 'Glazer - Pulse'),
(5139, 0, 1, 0, 4096, 194945, 0, 0, 0, 0, 0, 0, 0, 0, 'Glazer - Lingering Gaze'),
(5139, 1, 42, 1, 4096, 194945, 0, 0, 0, 0, 0, 0, 0, 0, 'Glazer - Lingering Gaze');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (194289,194333);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 3, 194289, 0, 0, 31, 0, 3, 102310, 0, 0, 0, '', NULL),
(13, 1, 194333, 0, 0, 31, 0, 3, 95887, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (194941);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(194941, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Lingering Gaze');

delete from spell_aura_trigger where spell_id in (201042,193373);
INSERT INTO `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) VALUES 
(201042, 194463, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Glazer - Beam boss'),
(193373, 194468, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Glazer - Beam trash');

delete from spell_target_position where id in (194149);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(194149, 1493, 4450.97, -673.27, 118.24, 1.57);

delete from spell_script_names where spell_id in (194463, 194468,195032);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(194463, 'spell_glazer_beam'),
(194468, 'spell_glazer_beam'),
(195032, 'spell_glazer_radiation');

delete from creature_model_info where DisplayID in (66531);
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`, `hostileId`) VALUES 
(66531, 0, 0, 0, 0);

/* ------------------------------------------------
    Cordana Felsong  /  Кордана Оскверненная Песнь
   ------------------------------------------------ */

delete from creature where id in (95888,100525);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`) VALUES 
(95888, 1493, 7787, 7787, 262, 1, '0', 0, 0,  4020.46, -297.3, -281.377, 0, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_cordana', flags_extra = 1 where entry = 95888;
update creature_template_wdb set classification = 3 where entry = 95888;

delete from areatrigger_actions where entry in (7101,5354);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `onDespawn`, `comment`) VALUES 
(7101, 0, 1, 0, 4096, 213395, 0, 0, 0, 0, 0, 0, 0, 0, 'Cordana - Deepening Shadows Apply'),
(7101, 1, 42, 1, 4096, 213395, 0, 0, 0, 0, 0, 0, 0, 0, 'Cordana - Deepening Shadows Apply'),
(5354, 0, 1, 0, 4096, 197541, 1, 0, 0, 0, 0, 0, 0, 0, 'Cordana - Detonation');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (201359,197823);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 201359, 0, 0, 31, 0, 3, 100525, 0, 0, 0, '', NULL),
(13, 4, 197823, 0, 0, 31, 0, 3, 100525, 0, 0, 0, '', NULL),
(13, 8, 197823, 0, 0, 31, 0, 3, 104293, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (201359,213576,203416);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(201359, 197897, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Elune\'s Light'),
(201359, 204294, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Elune\'s Light'),
(213576, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Deepening Shadows'),
(203416, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Shadowstep');






delete from creature_summon_groups where summonerId in (95888);
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(95888, 0, 0, 0, 100525, 4022.88, -269.684, -281.029, 5.49572, 0, 0, 0, 8, 0),
(95888, 1, 0, 0, 100525, 4482.64, -334.073, -240.317, 2.41779, 0, 0, 0, 8, 0),
(95888, 2, 0, 0, 100525, 4078.44, -269.568, -281.029, 3.9366, 0, 0, 0, 8, 0),
(95888, 3, 0, 0, 100525, 4022.93, -325.17, -281.029, 0.810119, 0, 0, 0, 8, 0),
(95888, 4, 0, 0, 100525, 4078.45, -325.234, -281.274, 2.35448, 0, 0, 0, 8, 0);


