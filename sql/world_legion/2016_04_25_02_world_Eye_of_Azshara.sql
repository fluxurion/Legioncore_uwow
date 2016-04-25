delete from world_visible_distance where type = 0 and id = 1456;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(0, 1456, 200, 'Eye of Azshara');

delete from areatrigger_force where CustomEntry = 9561;
INSERT INTO `areatrigger_force` (`CustomEntry`, `AuraID`, `MayBeAuraID`, `windType`, `windSpeed`, `windX`, `windY`, `windZ`, `centerX`, `centerY`, `centerZ`) VALUES 
(9561, 191797, 191797, 0, 2, -0.0000000874228, 2, 0, -3486.26, 4386.87, -3.58042);

/* ---------------------------------------
    Warlord Parjesh  /  Полководец Паржеш
   --------------------------------------- */

update creature set spawntimesecs = 86400 where id = 91784;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_warlord_parjesh', flags_extra = 1 where entry = 91784;
update creature_template_wdb set classification = 3 where entry = 91784;

delete from areatrigger_data where entry in (4863,4888);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `Distance`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(4863, 191900, 9580, 49704, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(4888, 191900, 9604, 49704, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

delete from areatrigger_actions where entry in (4863,4876);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4863, 0, 1, 0, 4096, 191919, 0, 0, 0, 0, 0, 0, 0, 'Crashing Wave'),
(4876, 0, 1, 0, 4096, 192053, 0, 0, 0, 0, 0, 0, 0, 'Quicksand Apply'),
(4876, 1, 42, 1, 4096, 192053, 0, 0, 0, 0, 0, 0, 0, 'Quicksand Remove');

delete from spell_dummy_trigger where spell_id in (191927);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(191927, 191946, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Impaling Spear - betwen cast'),
(191927, 192094, 6, 0, 0, 0, 0, 0, 0, 2, 0, 0, 'Impaling Spear - Fixate');

delete from spell_target_filter where spellid = 191946;
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(191946, 130, 4, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 'Impaling Spear');

/* -----------------------------------------
    Lady Hatecoil  /  Леди Кольцо Ненависти
   ----------------------------------------- */
delete from creature where id = 97853;
update creature set position_z = 0.1, spawntimesecs = 86400 where id = 91789;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_lady_hatecoil', flags_extra = 1 where entry = 91789;
update creature_template_wdb set classification = 3 where entry = 91789;
update creature_template set ScriptName = 'npc_hatecoil_sand_dune', flags_extra = 128 where entry = 97853;

delete from creature_model_info where DisplayID = 64055;
INSERT INTO `creature_model_info` (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`, `hostileId`) VALUES 
(64055, 0, 0, 0, 0);

delete from creature_summon_groups where summonerId = 91789;
INSERT INTO `creature_summon_groups` (`summonerId`, `id`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `count`, `actionType`, `distance`, `summonType`, `summonTime`) VALUES 
(91789, 0, 0, 0, 97853, -3471.26, 4568.86, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 1, 0, 0, 97853, -3478.83, 4596.36, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 2, 0, 0, 97853, -3426.3, 4560.19, -0.354614,  0, 0, 0, 0, 8, 0),
(91789, 3, 0, 0, 97853, -3452.93, 4569.12, -0.354621, 0, 0, 0, 0, 8, 0),
(91789, 4, 0, 0, 97853, -3415.41, 4570.52, -0.354614, 0, 0, 0, 0, 8, 0),
(91789, 5, 0, 0, 97853, -3464.79, 4614.29, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 6, 0, 0, 97853, -3463.75, 4596.38, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 7, 0, 0, 97853, -3450.3, 4579.59, -0.354631,  0, 0, 0, 0, 8, 0),
(91789, 8, 0, 0, 97853, -3440.32, 4553.86, -0.354614, 0, 0, 0, 0, 8, 0),
(91789, 9, 0, 0, 97853, -3425.11, 4604.11, -0.354628, 0, 0, 0, 0, 8, 0),
(91789, 10, 0, 0, 97853, -3447.14, 4603.75, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 11, 0, 0, 97853, -3408.97, 4608.92, -0.354607, 0, 0, 0, 0, 8, 0),
(91789, 12, 0, 0, 97853, -3465.96, 4582.65, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 13, 0, 0, 97853, -3443.34, 4590.65, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 14, 0, 0, 97853, -3420.62, 4616.91, -0.354624, 0, 0, 0, 0, 8, 0),
(91789, 15, 0, 0, 97853, -3440.59, 4620.17, -0.354631, 0, 0, 0, 0, 8, 0),
(91789, 16, 0, 0, 97853, -3425.97, 4586.57, -0.354627, 0, 0, 0, 0, 8, 0),
(91789, 17, 0, 0, 97853, -3402.37, 4589.11, -0.354608, 0, 0, 0, 0, 8, 0),
(91789, 18, 0, 0, 97853, -3462.64, 4552.49, -0.354621, 0, 0, 0, 0, 8, 0);

delete from areatrigger_actions where entry in (4972);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4972, 0, 1, 0, 4096, 193066, 0, -193066, 0, 0, 0, 0, 0, 'Sand Dune Apply'),
(4972, 1, 42, 1, 4096, 193066, 0, 0, 0, 0, 0, 0, 0, 'Sand Dune Remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (193720,193625);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 193720, 0, 0, 31, 0, 3, 98293, 0, 0, 0, '', NULL),
(13, 1, 193625, 0, 0, 31, 0, 3, 97853, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (197324,193682);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(197324, 197326, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Crackling Thunder'),
(193682, 193683, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Beckon Storm');

delete from spell_script_names where spell_id in (197324, 193698);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(197324, 'spell_crackling_thunder_filter'),
(193698, 'spell_hatecoil_curse_of_the_witch');

delete from spell_target_filter where spellid in (193597);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(193597, 15, 1, 193066, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 'Static Nova');

/* --------------------------------------
    King Deepbeard  /  Король Волнобород
   -------------------------------------- */

update creature set spawntimesecs = 86400 where id = 91797;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_king_deepbeard', flags_extra = 1 where entry = 91797;
update creature_template_wdb set classification = 3 where entry = 91797;
update creature_template set ScriptName = 'npc_deepbeard_quake', flags_extra = 128 where entry = 97916;
update creature_template set ScriptName = 'npc_deepbeard_call_the_seas', flags_extra = 128 where entry = 97844;

delete from areatrigger_actions where entry in (4968);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4968, 0, 1, 0, 4096, 193055, 0, 0, 0, 0, 0, 0, 0, 'Call the Seas Apply'),
(4968, 1, 42, 1, 4096, 193055, 0, 0, 0, 0, 0, 0, 0, 'Call the Seas Remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (193051);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 193051, 0, 0, 31, 0, 3, 97844, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (193093);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(193093, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Ground Slam');

delete from spell_script_names where spell_id in (193018);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(193018, 'spell_deepbeard_gaseous_explosion');

/* -----------------------
    Serpentrix  /  Змеикс
   ----------------------- */
delete from creature where id in (97259);
update creature set spawntimesecs = 86400 where id = 91808;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_serpentrix', flags_extra = 1 where entry = 91808;
update creature_template_wdb set classification = 3 where entry = 91808;
update creature_template set flags_extra = 128 where entry = 97263;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_serpentrix_blazing_hydra' where entry = 97259;

update areatrigger_data set updatedelay = 1000 where entry = 4856;

delete from areatrigger_actions where entry in (4856);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4856, 0, 4, 0, 9, 191858, 0, 0, 0, 0, 0, 0, 0, 'Toxic Puddle');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (192010,202680);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 192010, 0, 0, 31, 0, 3, 97263, 0, 0, 0, '', NULL),
(13, 1, 202680, 0, 0, 31, 0, 3, 97259, 0, 0, 0, '', NULL);

delete from spell_target_filter where spellId in (191855,192010);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(191855, 15, 4, -1, 0, 0, 0, 0, 3, 1, 1, 0, 0, 0, 'Toxic Wound'),
(192010, 7, 2, -1, 10, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Summon Hydra Spawn - dist'),
(192010, 7, 4, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 'Summon Hydra Spawn - sort');

delete from spell_dummy_trigger where spell_id in (192050);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(192050, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Poison Spit');

delete from spell_script_names where spell_id in (191873);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(191873, 'spell_serpentrix_submerge_teleport');

/* -----------------------------------
    Wrath of Azshara  /  Гнев Азшары
   ----------------------------------- */

delete from creature where id in (98173,100248,100249,100250);
update creature set spawntimesecs = 86400 where id = 96028;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_wrath_of_azshara', flags_extra = 1 where entry = 96028;
update creature_template_wdb set classification = 3 where entry = 96028;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'npc_wrath_of_azshara_nagas' where entry in (98173,100248,100249,100250);
update creature_template set ScriptName = 'npc_wrath_of_azshara_mystic_tornado', flags_extra = 128 where entry = 97673;
update creature_template set ScriptName = 'npc_wrath_of_azshara_arcane_bomb', flags_extra = 128 where entry = 97691;
update creature_template set AIName = 'SmartAI' where entry = 97713;

delete from smart_scripts where entryorguid = 97713;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(97713, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'React Passive'),
(97713, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 11, 192732, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Link Cast'),
(97713, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 11, 192737, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Link Cast'),
(97713, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 192736, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Link Cast');

delete from areatrigger_actions where entry in (4923,4930);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(4923, 0, 1, 0, 4096, 192633, 0, 0, 0, 0, 0, 0, 0, 'Surging Waters'),
(4930, 0, 1, 0, 4096, 192675, 0, 0, 0, 0, 0, 0, 0, 'Mystic Tornado');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (192793,193196);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 192793, 0, 0, 31, 0, 3, 97739, 0, 0, 0, '', NULL),
(13, 1, 193196, 0, 0, 31, 0, 3, 96028, 0, 0, 0, '', NULL);

delete from spell_dummy_trigger where spell_id in (192680,192620,192705);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(192680, 192681, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Mystic Tornado'),
(192620, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Massive Deluge'),
(192705, 192706, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Arcane Bomb');

delete from spell_target_filter where spellId in (192680,192620);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(192680, 15, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 'Mystic Tornado'),
(192620, 15, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 'Massive Deluge');








