/* delete from areatrigger_teleport where id in ();
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(****, 'Maw of Souls - Enter', 1466, 3219.28, 1824.71, 230.29, 3.17),
(****, 'Maw of Souls - Exit', 0, 0, 0, 0, 0); 
*/

delete from world_visible_distance where type = 2 and id = 7811;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(2, 7811, 200, 'The Maw of Souls - fly ship');

/* ----------------------------------------------------
    Ymiron, the Fallen King  /  Имирон, падший король
   ---------------------------------------------------- */

update creature set spawntimesecs = 86400 where id = 96756;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_ymiron_the_fallen_king', flags_extra = 1 where entry = 96756;
update creature_template_wdb set classification = 3 where entry = 96756;
update creature_template set unit_flags = 0, unit_flags2 = 0 where entry = 102375;
update gameobject_template set flags = 0 where entry = 247041;

delete from spell_script_names where spell_id in (167922,193460,191560);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(167922, 'spell_ymiron_power_regen'),
(193460, 'spell_ymiron_bane'),
(191560, 'spell_ymiron_helheim_teleport');

delete from areatrigger_data where entry in (5016,5838);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(5016, 193465, 0, 0, 2.5, 2.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ymiron - Bane'),
(5838, 193465, 0, 0, 2.5, 2.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ymiron - Bane');

delete from areatrigger_actions where entry in (5016,5838);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5016, 0, 1, 0, 4096, 193513, 0, 0, 0, 0, 0, 0, 0, 'Ymiron - Bane'),
(5838, 0, 1, 0, 4096, 193513, 0, 0, 0, 0, 0, 0, 0, 'Ymiron - Bane');

/* ------------------------
    Harbaron  /  Харбарон
   ------------------------ */

delete from creature where id in (98761,98989,100839);
update creature set spawntimesecs = 86400 where id = 96754;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_harbaron', flags_extra = 1 where entry = 96754;
update creature_template_wdb set classification = 3 where entry = 96754;

update creature_template set mechanic_immune_mask = 617299967, flags_extra = 128 where entry = 98989;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_harbaron_scythe', flags_extra = 128 where entry = 100839;
update creature_template set mechanic_immune_mask = 617297663, ScriptName = 'npc_harbaron_shackled_servitor' where entry = 98693;
update creature_template set mechanic_immune_mask = 617297663, ScriptName = 'npc_harbaron_soul_fragment' where entry = 98761;

delete from gameobject where id in (246947);
INSERT INTO `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `AiID`, `state`, `isActive`) VALUES 
(246947, 1492, 7812, 7811, 262, 1, '', 2959.17, 705.19, 553.055, 4.47893, 0, 0, 1, -0.0000000437114, 7200, 255, 0, 1, 0),
(246947, 1492, 7812, 7811, 262, 1, '', 2908.5, 705.914, 553.038, 1.73135, 0, 0, 0.761532, 0.648127, 300, 0, 0, 1, 0);

delete from spell_dummy_trigger where spell_id in (205330,194325);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(205330, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Cosmic Scythe'),
(194325, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Fragment');

delete from spell_target_filter where spellid in (205330);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(205330, 15, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 'Cosmic Scythe');

delete from npc_spellclick_spells where npc_entry = 100839;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(100839, 46598, 1, 0);

delete from spell_linked_spell where spell_trigger in (194327);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(194327, 198551, 0, 'Fragment Stun');

delete from spell_aura_trigger where spell_id in (194344,199837);
INSERT INTO `spell_aura_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `slot`, `comment`) VALUES 
(194344, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Harbaron - Fragment'),
(199837, 204502, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Shade of Xavius - Drain Essence');

/* ------------------
    Helya  /  Хелия
   ------------------ */

delete from creature where id in 
(98363,99800,99803,102306,97099,99447,98973,97200,97365,99033,98919,
101504,100360,100362,100354,101814,99801,100188,101740,100359,100361,100353);

update creature set spawntimesecs = 86400 where id = 96759;
update creature_template set inhabitType = 7, mechanic_immune_mask = 617299967, ScriptName = 'boss_helya', flags_extra = 1 where entry = 96759;
update creature_template_wdb set classification = 3 where entry = 96759;
update creature_template set ScriptName = 'npc_helya_tentacle' where entry in (98363,99801,100354,100360,100362,100188);
update creature_template set ScriptName = 'npc_helya_tentacle_veh' where entry in (97099);
update creature_template set flags_extra = 536871040 where entry in (97099,99803,99800,100361,100359,100353);
update creature_template_addon set auras = '' where entry in (96759,100188);

delete from gameobject where id in (246842,246036);
INSERT INTO `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `AiID`, `state`, `isActive`) VALUES 
(246842, 1492, 7812, 7811, 262, 1, '', 2927.35, 928.695, 511.52, 3.12509, 0, 0, 0.999966, 0.00825308, 300, 0, 0, 1, 0),
(246036, 1492, 7812, 7811, 262, 1, '', 2933.32, 933.939, 512.264, 4.72246, 0, 0, 0.703536, -0.71066, -86400, 0, 0, 1, 0);

/* delete from areatrigger_data where entry in (4048); -- Нет данных
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(4048, 194603, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Helya - Soulless Scream'),
(5070, 195167, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Helya - Swirling Pool'),
*/

delete from areatrigger_actions where entry in (4048,5070);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4048, 0, 1, 0, 4096, 194841, 0, 0, 0, 0, 0, 0, 0, 'Helya - Soulless Scream'),
(5070, 0, 1, 0, 4096, 195309, 0, 0, 0, 0, 0, 0, 0, 'Helya - Swirling Pool');

delete from spell_dummy_trigger where spell_id in (197517);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(197517, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Helya - Taint of the Sea');

delete from spell_target_filter where spellId in (197517,197805);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(197517, 15, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 'Helya - Taint of the Sea'),
(197805, 15, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 'Helya - Torrent');

delete from spell_target_position where id = 197653;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(197653, 1492, 2947.05, 914.22, 513, 0);

delete from spell_linked_spell where spell_trigger in (-197262);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-197262, 197264, 0, 'Taint of the Sea');

delete from npc_spellclick_spells where npc_entry in (99800,99803,100353,100359,100361);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(99800, 46598, 1, 0),
(99803, 46598, 1, 0),
(100353, 46598, 1, 0),
(100359, 46598, 1, 0),
(100361, 46598, 1, 0);

delete from vehicle_template_accessory where EntryOrAura in (99800,100353,100359,100361);
INSERT INTO `vehicle_template_accessory` (`EntryOrAura`, `accessory_entry`, `seat_id`, `offsetX`, `offsetY`, `offsetZ`, `offsetO`, `minion`, `description`, `summontype`, `summontimer`) VALUES 
(99800, 98363, 0, 0.001, 0, 0, 0, 1, '99800 - 98363', 8, 0),
(100353, 100354, 0, 0.001, 0, 0, 0, 1, '100353 - 100354', 8, 0),
(100359, 100360, 0, 0.001, 0, 0, 0, 1, '100359 - 100360', 8, 0),
(100361, 100362, 0, 0.001, 0, 0, 0, 1, '100361 - 100362', 8, 0);