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




