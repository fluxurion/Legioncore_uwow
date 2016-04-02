delete from world_visible_distance where type = 2 and id in (7672,7983);
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(2, 7672, 200, 'Halls of Valor'),
(2, 7983, 150, 'Halls of Valor - Fenryr Lair');

update creature set spawndist = 15 where spawndist > 15 and map = 1477;

/* -----------------------
    Hymdall  /  Химдалль
   ----------------------- */

update creature set spawntimesecs = 86400 where id = 94960;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_hymdall', flags_extra = 1 where entry = 94960;
update creature_template_wdb set classification = 3 where entry = 94960;
update creature_template set inhabittype = 7, ScriptName = 'npc_hymdall_storm_drake' where entry = 97788;
update creature_template set flags_extra = 130 where entry = 97795;
update creature_template set minlevel = 98, maxlevel = 110, faction = 14, unit_flags = 33587200 where entry = 97960;

delete from areatrigger_actions where entry in (4960,4985);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4960, 0, 1, 0, 4096, 192961, 0, 0, 0, 0, 0, 0, 0, 'Hymdall - Storm Breath'),
(4985, 0, 1, 0, 4096, 193234, 0, 0, 0, 0, 0, 0, 0, 'Hymdall - Dancing Blade - Apply'),
(4985, 1, 42, 1, 4096, 193234, 0, 0, 0, 0, 0, 0, 0, 'Hymdall - Dancing Blade - Remove');

/* ------------------
    Hyrja  /  Хирья
   ------------------ */

delete from creature where id in (97202,97219);
update creature set spawntimesecs = 86400, spawndist = 0, movementType = 0 where id = 95833;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_hyrja', flags_extra = 1 where entry = 95833;
update creature_template_wdb set classification = 3 where entry = 95833;
update creature_template set ScriptName = 'npc_hyrja_defenders' where entry in (97202,97219);
update creature_template set flags_extra = 130 where entry = 101758;

update areatrigger_data set moveType = 6 where entry = 4885;
delete from areatrigger_data where entry in (4865,4866);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(4865, 192152, 9582, 49765, 50, 50, 60, 60, 17.5, 17.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Mystic Empowerment: Holy'),
(4866, 192150, 9583, 49765, 50, 50, 60, 60, 17.5, 17.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Mystic Empowerment: Thunder');

delete from areatrigger_actions where entry in (4865,4866,5987,5667,4885);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4865, 0, 1, 0, 65536, 191924, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Mystic Empowerment: Holy Beam - Apply'),
(4865, 1, 42, 1, 65536, 191924, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Mystic Empowerment: Holy Beam - Remove'),
(4866, 0, 1, 0, 65536, 192008, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Mystic Empowerment: Thunder Beam - Apply'),
(4866, 1, 42, 1, 65536, 192008, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Mystic Empowerment: Thunder Beam - Remove'),
(5987, 0, 1, 0, 4096, 203963, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Eye of the Storm - Apply'),
(5987, 1, 42, 1, 4096, 203963, 0, 0, 0, 0, 0, 0, 0, 'Hyrja - Eye of the Storm - Remove'),
(5667, 0, 1, 0, 4096, 200906, 0, 0, 0, 0, 0, 0, 0, 'Eye of the Storm Apply'),
(5667, 1, 42, 1, 4096, 200906, 0, 0, 0, 0, 0, 0, 0, 'Eye of the Storm Remove'),
(4885, 0, 1, 0, 4096, 192206, 0, 0, 0, 0, 0, 0, 0, 'Sanctify');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (192304);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 192304, 0, 0, 31, 0, 3, 101758, 0, 0, 0, '', NULL);

delete from spell_target_position where id in (191478,203955);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(191478, 1477, 3149.01, 324.14, 655.25, 0.78),
(203955, 1477, 3164.38, 339.224, 655.18, 6.19);

delete from spell_linked_spell where spell_trigger in (-192008,-191924);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-192008, -192132, 0, 'Hyrja - Remove Thunder'),
(-191924, -192133, 0, 'Hyrja - Remove Holy');

delete from spell_script_names where spell_id in (192130,192048,192158);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(192130, 'spell_hyrja_empowerment_tracker'),
(192048, 'spell_hyrja_expel_light'),
(192158, 'spell_hyrja_sanctify');

delete from spell_dummy_trigger where spell_id in (192044,192304);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(192044, 192048, 6, 0, 0, 0, 0, 0, 0, 3, 0, 0, 'Hyrja - Expel Light'),
(192304, 200902, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Hyrja - Eye of the Storm');

/* -------------------
    Fenryr  /  Фенрир
   ------------------- */

delete from creature where guid in (1152810,1152808);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES 
(1152810, 95674, 1477, 7672, 7983, 262, 1, '', 0, 0, 3006.39, 2980.55, 618.48, 2.26, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(1152808, 95674, 1477, 7672, 7983, 262, 1, '', 0, 0, 3154.69, 3166.24, 593.6, 3.29, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

delete from gameobject where id in (246272);

update creature set spawntimesecs = 86400, spawndist = 0, movementType = 0 where id = 99868;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_fenryr', flags_extra = 1 where entry = 99868;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'npc_fenryr', AiID = 0 where entry = 95674;
update creature_template_wdb set classification = 3 where entry = 99868;
update creature_template_wdb set DisplayId1 = 27521 where entry = 96608;
update creature_template_wdb set DisplayId1 = 47641, DisplayId2 = 0 where entry = 101712;
update creature_template_addon set auras = '' where entry in (select id from creature where areaId = 7983);
update creature_template_addon set auras = '200611' where entry = 101712;

delete from pool_template where entry in (60023);
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES 
(60023, 1, 'Halls of Valor - Fenryr');

delete from pool_creature where guid in (1152810,1152808);
INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES 
(1152810, 60023, 0, 'Halls of Valor - Fenryr - Spawn 1'),
(1152808, 60023, 0, 'Halls of Valor - Fenryr - Spawn 2');

delete from spell_area where spell in (196497);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
(196497, 7983, 0, 0, 0, 0, 2, 0, 64, 11);

delete from spell_dummy_trigger where spell_id in (197549,198800);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(197549, 197556, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Fenryr - Ravenous Leap - Mark'),
(198800, 196838, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Fenryr - Scent of Blood');

delete from spell_target_filter where spellId in (197549,197560);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(197549, 15, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 'Fenryr - Ravenous Leap'),
(197560, 15, 1, 197556, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Fenryr - Ravenous Leap - Check Aura');

delete from spell_script_names where spell_id in (198800,196838);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(198800, 'spell_fenryr_scent_of_blood_filter'),
(196838, 'spell_fenryr_scent_of_blood_fixate');

delete from spell_target_position where id in (197575,197577);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(197575, 1477, 3186.41, 2902.43, 638.8, 2.24),
(197577, 1477, 3251.59, 632.13, 635.4, 5.46);

/* -------------------------------------------
    God-King Skovald  /  Король-Бог Сковальд
   ------------------------------------------- */

delete from creature where id in (95843,97081,97083,97084);
update creature set spawntimesecs = 86400 where id = 95675;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_god_king_skovald', flags_extra = 1 where entry = 95675;
update creature_template_wdb set classification = 3 where entry = 95675;
update creature_template set mechanic_immune_mask = 617299967, faction = 35, npcflag = 1, ScriptName = 'npc_generic_odyn_kings' where entry in (95843,97081,97083,97084);
update creature_template set minlevel = 100, maxlevel = 100, npcflag = 16777216, ScriptName = 'npc_skovald_aegis_of_aggramar' where entry = 98364;

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (193765,194112,193988);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 193765, 0, 0, 31, 0, 3, 171306, 0, 0, 0, '', NULL),
(13, 1, 194112, 0, 0, 31, 0, 3, 98364, 0, 0, 0, '', NULL),
(13, 1, 193988, 0, 0, 31, 0, 3, 95675, 0, 0, 0, '', NULL);

delete from areatrigger_actions where entry in (5040, 5031);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5040, 0, 1, 0, 9, 193743, 0, 0, 0, 0, 0, 0, 0, 'Aegis of Aggramar - Apply'),
(5040, 1, 42, 1, 9, 193743, 0, 0, 0, 0, 0, 0, 0, 'Aegis of Aggramar - Remove'),
(5031, 0, 1, 0, 4096, 193702, 0, 0, 0, 0, 0, 0, 0, 'Infernal Flames Apply'),
(5031, 1, 42, 1, 4096, 193702, 0, 0, 0, 0, 0, 0, 0, 'Infernal Flames Remove');

delete from areatrigger_polygon where entry in (5040);
INSERT INTO `areatrigger_polygon` (`entry`, `spellId`, `type`, `id`, `x`, `y`) VALUES
(5040, 193765, 1, 0, 0.375, 4.645813),
(5040, 193765, 1, 1, -3.447754, 6.100647),
(5040, 193765, 1, 2, -189.907, 8.611084),
(5040, 193765, 1, 3, -190.3621, -8.34552),
(5040, 193765, 1, 4, -3.142334, -6.06427),
(5040, 193765, 1, 5, 0.604248, -4.246582),
(5040, 193765, 1, 6, 2.00708, 0.07983398);

delete from spell_dummy_trigger where spell_id in (193658,194222);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(193658, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Skovald - Felblaze Rush'),
(194222, 193705, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Infernal Flames');

delete from spell_target_position where id in (193940);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(193940, 1477, 2405.39, 528.123, 749.212, 0);

delete from spell_script_names where spell_id in (193826,193991,193783,193983);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(193826, 'spell_skovald_ragnarok'),
(193991, 'spell_skovald_drop_aegis'),
(193783, 'spell_skovald_aegis_remove'),
(193983, 'spell_skovald_aegis_remove');

delete from spell_linked_spell where spell_trigger in (-193765);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-193765, -193783, 0, 'Remove Aegis');

/* ---------------
    Odyn  /  Один
   --------------- */

delete from creature where id = 101459;
update creature set spawntimesecs = 86400, spawndist = 0, movementType = 0 where id = 95676;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_odyn', flags_extra = 1 where entry = 95676;
update creature_template_wdb set classification = 3 where entry = 95676;
update creature_template_addon set auras = '' where entry in (95676);
update creature_template set faction = 14, minlevel = 110, maxlevel = 110, AIName = 'SmartAI', flags_extra = 128 where entry = 100575;
update creature_template set flags_extra = 128 where entry = 101459;
update gameobject set spawntimesecs = -86400 where id = 245847;

delete from smart_scripts where entryorguid = 100575;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(100575, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Summon - SetReact - Passive'),
(100575, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 198059, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Link - Cast Spear of Light');

delete from areatrigger_actions where entry in (5691,5413,5398,5400,5401,5403,5404);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5691, 0, 1, 0, 4096, 200988, 0, 0, 0, 0, 0, 0, 0, 'Spear of Light Apply'),
(5691, 1, 42, 1, 4096, 200988, 0, 0, 0, 0, 0, 0, 0, 'Spear of Light Remove'),
(5413, 0, 1, 0, 4096, 198088, 0, 0, 0, 0, 0, 0, 0, 'Shatter Spears Apply'),
(5413, 1, 42, 1, 4096, 198088, 0, 0, 0, 0, 0, 0, 0, 'Shatter Spears Remove'),
(5398, 0, 1, 0, 4096, 197996, 0, 197963, 0, 0, 0, 0, 0, 'Runic Brand - Pure Branded'),
(5400, 0, 1, 0, 4096, 197996, 0, 197964, 0, 0, 0, 0, 0, 'Runic Brand - Red Branded'),
(5401, 0, 1, 0, 4096, 197996, 0, 197965, 0, 0, 0, 0, 0, 'Runic Brand - Yellow Branded'),
(5403, 0, 1, 0, 4096, 197996, 0, 197966, 0, 0, 0, 0, 0, 'Runic Brand - Blue Branded'),
(5404, 0, 1, 0, 4096, 197996, 0, 197967, 0, 0, 0, 0, 0, 'Runic Brand - Green Branded');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (198077);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 198077, 0, 0, 31, 0, 3, 100575, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (198284);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(198284, 198060, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Odyn - Spear of Light');

delete from spell_script_names where spell_id in (201006,197963,197964,197965,197966,197967);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(201006, 'spell_odyn_radiant_tempest'),
(197963, 'spell_odyn_runic_brand'),
(197964, 'spell_odyn_runic_brand'),
(197965, 'spell_odyn_runic_brand'),
(197966, 'spell_odyn_runic_brand'),
(197967, 'spell_odyn_runic_brand');

delete from spell_target_position where id in (197968,197971,197972,197975,197977);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(197968, 1477, 2437.733, 497.3854, 749.4288, 0),
(197971, 1477, 2403.48, 509.241, 749.66, 0),
(197972, 1477, 2403.48, 547.997, 749.716, 0),
(197975, 1477, 2437.67, 559.781, 749.638, 0),
(197977, 1477, 2461.51, 528.667, 749.589, 0);

delete from spell_linked_spell where spell_trigger in (197996);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(197996, -197963, 0, 'Branded - Remove - Runic Brand'),
(197996, -197964, 0, 'Branded - Remove - Runic Brand'),
(197996, -197965, 0, 'Branded - Remove - Runic Brand'),
(197996, -197966, 0, 'Branded - Remove - Runic Brand'),
(197996, -197967, 0, 'Branded - Remove - Runic Brand');

