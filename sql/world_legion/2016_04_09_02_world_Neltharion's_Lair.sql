update creature set spawndist = 20 where map = 1458 and spawndist > 20;

/* ----------------------
    Rokmora  /  Рокмора
   ---------------------- */

update creature set spawntimesecs = 86400 where id = 91003;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_rokmora', flags_extra = 1 where entry = 91003;
update creature_template_wdb set classification = 3 where entry = 91003;
update creature_template set minlevel = 98, maxlevel = 110, faction = 16, ScriptName = 'npc_rokmora_blightshard_skitter' where entry = 97720;

delete from areatrigger_actions where entry in (4953);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4953, 0, 1, 0, 4096, 192800, 0, 0, 0, 0, 0, 0, 0, 'Rokmora - Choking Dust Apply'),
(4953, 1, 42, 1, 4096, 192800, 0, 0, 0, 0, 0, 0, 0, 'Rokmora - Choking Dust Remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (200247);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 200247, 0, 0, 31, 0, 3, 97720, 0, 0, 0, '', NULL);

delete from spell_script_names where spell_id in (193245);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(193245, 'spell_rokmora_gain_energy');

delete from spell_linked_spell where spell_trigger = 188114;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(188114, 200247, 0, 'Shatter skitter kill');

delete from spell_dummy_trigger where spell_id in (209718);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(209718, 188169, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Razor Shards');

/* -----------------------------------------
    Ularogg Cragshaper  /  Уларогг Скаломол
   -----------------------------------------*/

delete from creature where id = 100818;
update creature set spawntimesecs = 86400 where id = 91004;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_ularogg_cragshaper', flags_extra = 1 where entry = 91004;
update creature_template_wdb set classification = 3 where entry = 91004;
update creature_template set minlevel = 98, maxlevel = 110, faction = 16, ScriptName = 'npc_ularogg_bellowing_idols' where entry = 98081;
update creature_template set minlevel = 98, maxlevel = 110, faction = 16, ScriptName = 'npc_ularogg_bellowing_idols' where entry = 100818;

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (198619,198631);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 198619, 0, 0, 31, 0, 3, 100818, 0, 0, 0, '', NULL),
(13, 4, 198631, 0, 0, 31, 0, 3, 100818, 0, 0, 0, '', NULL);

delete from spell_script_names where spell_id in (183213);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(183213, 'spell_barrel_ride_plr_move');

delete from spell_dummy_trigger where spell_id in (198443,193270);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(198443, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Strike of the Mountain'),
(193270, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Falling Debris');

delete from spell_target_position where id in (198509,198564,198565);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(198509, 1458, 2838.15, 1667.87, -40.8246, 0),
(198564, 1458, 2842.439, 1660.349, -40.82954, 3.647),
(198565, 1458, 2834.184, 1677.193, -40.82466, 3.612);

delete from spell_linked_spell where spell_trigger in (198564);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(198564, 198815, 0, 'Stance of the Mountain - Conversation');

delete from waypoint_data where id in (9100400);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `speed`, `action`, `action_chance`, `entry`, `wpguid`) VALUES 
(9100400, 1, 2820.56, 1325.45, -4.54633, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 2, 2802.25, 1316.7, -4.29819, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 3, 2795.72, 1301.3, -4.29819, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 4, 2778.16, 1288.85, -4.32394, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 5, 2768.9, 1267.93, -4.29753, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 6, 2744.28, 1259.7, -4.78079, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 7, 2735.45, 1246.16, -4.78259, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 8, 2717.75, 1251.4, -4.78259, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 9, 2695.62, 1251.58, -4.78259, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 10, 2677.29, 1259.6, -4.78155, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 11, 2660.6, 1268.23, -4.78155, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 12, 2655.97, 1281.47, -4.78209, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 13, 2642.57, 1294.07, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 14, 2635.1, 1313.17, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 15, 2625.24, 1329.53, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 16, 2622.05, 1348.13, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 17, 2614.58, 1355.12, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 18, 2605.5, 1377.67, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 19, 2595.58, 1381.25, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 20, 2575.87, 1396.24, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 21, 2556.38, 1397.96, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 22, 2545.27, 1408.56, -4.78263, 0, 0, 1, 25, 0, 100, 0, 0),
(9100400, 23, 2549.71, 1449.33, -51, 0, 0, 1, 25, 335, 100, 0, 0);

delete from waypoint_scripts where id = 335;
INSERT INTO `waypoint_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`, `guid`) VALUES 
(335, 0, 14, 183213, 0, 0, 0, 0, 0, 0, 0);

/* ----------------------
    Naraxas  /  Нараксас
   ----------------------*/

delete from creature where id = 101075;
update creature set spawntimesecs = 86400 where id = 91005;
update creature_template set inhabittype = 7, mechanic_immune_mask = 617299967, ScriptName = 'boss_naraxas', flags_extra = 1 where entry = 91005;
update creature_template_wdb set classification = 3 where entry = 91005;
update creature_template set ScriptName = 'npc_naraxas_wormspeaker_devout' where entry = 101075;

delete from areatrigger_actions where entry in (4499,6735);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4499, 0, 1, 0, 4096, 188494, 0, 0, 0, 0, 0, 0, 0, 'Rancid Maw Apply'),
(4499, 1, 42, 1, 4096, 188494, 0, 0, 0, 0, 0, 0, 0, 'Rancid Maw Remove'),
(4499, 2, 1, 0, 262145, 205609, 0, -205609, 0, 0, 0, 0, 101075, 'Apply Root Npc 101075'),
(6735, 0, 1, 0, 4096, 210166, 0, 0, 0, 0, 0, 0, 0, 'Toxic Wretch Apply'),
(6735, 1, 42, 1, 4096, 210166, 0, 0, 0, 0, 0, 0, 0, 'Toxic Wretch Remove');

delete from spell_dummy_trigger where spell_id in (205549);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(205549, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Rancid Maw');

/* ----------------------
    Naraxas  /  Нараксас
   ----------------------*/

update creature set spawntimesecs = 86400 where id = 91007;
update creature_template set inhabittype = 7, mechanic_immune_mask = 617299967, ScriptName = 'boss_dargrul_the_underking', flags_extra = 1 where entry = 91007;
update creature_template_wdb set classification = 3 where entry = 91007;
update creature_template set ScriptName = 'npc_dargrul_molten_charskin' where entry = 101476;
update creature_template set ScriptName = 'npc_dargrul_crystal_wall', flags_extra = 128 where entry = 101593;

update areatrigger_data set radius = 10, radiusTarget = 10, height = 10, heightTarget = 10, polygon = 0 where entry = 5635;

delete from areatrigger_actions where entry in (5635);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5635, 0, 1, 0, 262144, 200672, 0, -200672, 0, 0, 0, 0, 101476, 'Apply Stun Npc 101476');

delete from areatrigger_polygon where entry in (5635);
INSERT INTO `areatrigger_polygon` (`entry`, `spellId`, `type`, `id`, `x`, `y`) VALUES 
(5635, 200367, 1, 0, -30, 6),
(5635, 200367, 1, 1, -30, -8),
(5635, 200367, 1, 2, 0, -7),
(5635, 200367, 1, 3, 0, 5);

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (209947);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 209947, 0, 0, 31, 0, 3, 101593, 0, 0, 0, '', NULL);

delete from spell_script_names where spell_id in (201444,209920,200404);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(201444, 'spell_dargrul_gain_energy'),
(209920, 'spell_dargrul_magma_breaker'),
(200404, 'spell_dargrul_magma_wave_filter');

delete from spell_dummy_trigger where spell_id in (209926,200551);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(209926, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Magma Breaker'),
(200551, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Crystal Spikes');

delete from spell_linked_spell where spell_trigger = 200404;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(200404, 209947, 0, 'Cast Magma Wave');


