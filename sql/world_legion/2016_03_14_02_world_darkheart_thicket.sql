/* delete from areatrigger_teleport where id in ();
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(****, 'DarkHeart Thicket - Enter', 1466, 3219.28, 1824.71, 230.29, 3.17),
(****, 'DarkHeart Thicket - Exit', 0, 0, 0, 0, 0); 
*/

/* ----------------------------------------------------
    Arch-Druid Glaidalis  /  Верховный друид Глайдалис
   ---------------------------------------------------- */

delete from creature where id in (96512);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES 
(96512, 1466, 7673, 7673, 262, 1, '0', 0, 0, 2888.09, 1965.66, 189.605, 0.0851218, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_arch_druid_glaidalis', flags_extra = 1 where entry = 96512;
update creature_template_wdb set typeflags = typeflags | 4, Classification = 3, displayid1 = 64927 where entry = 96512;

update creature_template_addon set auras = '' where entry = 96512;

delete from spell_script_names where spell_id in (196346,196376);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(196346, 'spell_glaidalis_grievous_leap'),
(196376, 'spell_glaidalis_grievous_tear');

delete from areatrigger_data where entry in (5443,5445);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(5443, 198379, 10152, 51556, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Glaidalis - Primal Rampage'),
(5445, 198402, 10154, 51560, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Glaidalis - Nightfall');

delete from areatrigger_actions where entry in (5443,5445);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5443, 0, 1, 0, 4096, 198386, 0, 0, 0, 0, 0, 0, 0, 'Glaidalis - Primal Rampage'),
(5445, 0, 1, 0, 4096, 198408, 0, 0, 0, 0, 0, 0, 0, 'Glaidalis - Nightfall Apply'),
(5445, 1, 42, 1, 4096, 198408, 0, 0, 0, 0, 0, 0, 0, 'Glaidalis - Nightfall Remove');

delete from spell_dummy_trigger where spell_id in (198400);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(198400, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Glaidalis - Nightfall');

delete from spell_target_filter where spellId in (198400);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(198400, 15, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 'Glaidalis - Nightfall');

/* -----------------------
    Oakheart  /  Дубосерд
   ----------------------- */

update creature set spawntimesecs = 86400 where id = 103344;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_oakheart', flags_extra = 1 where entry = 103344;
update creature_template_wdb set typeflags = typeflags | 4, Classification = 3 where entry = 103344;
update creature_template_addon set auras = '' where entry = 103344;

delete from creature_template where entry in (100991);
INSERT INTO `creature_template` (`entry`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Mana_mod_extra`, `Armor_mod`, `RegenHealth`, `mechanic_immune_mask`, `personalloot`, `VignetteId`, `flags_extra`, `WorldEffectID`, `AiID`, `MovementIDKit`, `MeleeID`, `ScriptName`) VALUES 
(100991, 0, 98, 110, 0, 16, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 34816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 617299967, 0, 0, 0, 0, 0, 0, 0, 'npc_oakheart_strangling_roots');

delete from areatrigger_data where entry in (6037);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(6037, 199067, 10725, 51718, 2.25, 2.25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Oakheart - Strangling Roots');

delete from areatrigger_actions where entry in (6037);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(6037, 0, 1, 0, 4096, 199063, 0, 0, 0, 0, 0, 0, 0, 'Oakheart - Strangling Roots Apply'),
(6037, 1, 42, 1, 4096, 199063, 0, 0, 0, 0, 0, 0, 0, 'Oakheart - Strangling Roots Remove');

delete from spell_dummy_trigger where spell_id in (204574);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(204574, 199051, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Oakheart - Strangling Roots');

delete from spell_target_filter where spellId in (204574);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(204574, 15, 9, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Oakheart - Strangling Roots');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (204613);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 204613, 0, 0, 31, 0, 3, 103344, 0, 0, 0, '', NULL);

/* -----------------------
    Dresaron  /  Дресарон - нет снифа.
   ----------------------- */

update creature set spawntimesecs = 86400 where id = 99200;
update creature_template set inhabitType = 7, mechanic_immune_mask = 617299967, ScriptName = 'boss_dresaron', flags_extra = 1 where entry = 99200;
update creature_template_wdb set typeflags = typeflags | 4, Classification = 3 where entry = 99200;

/* ---------------------------------
    Shade of Xavius  /  Тень Ксавия
   --------------------------------- */

delete from creature where id in (101329,100652);
update creature set spawntimesecs = 86400 where id = 99192;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_shade_of_xavius', flags_extra = 1 where entry = 99192;
update creature_template_wdb set typeflags = typeflags | 4, Classification = 3 where entry = 99192;
update creature_template set mechanic_immune_mask = 617299967 where entry = 101329;
update creature_template_wdb set displayId1 = 11686 where entry = 101329;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_xavius_malfurion_stormrage' where entry = 100652;

delete from spell_dummy_trigger where spell_id in (204808,200359);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(204808, 200185, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Nightmare Bolt'),
(200359, 200289, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Induced Paranoia');

delete from spell_target_filter where spellId in (204808,200359);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(204808, 15, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 'Nightmare Bolt'),
(200359, 15, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 'Induced Paranoia');

delete from areatrigger_data where entry in (5631);
INSERT INTO `areatrigger_data` (`entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`, `isMoving`, `moveType`, `waitTime`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`, `comment`) VALUES 
(5631, 200289, 10343, 52046, 6.5, 6.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Induced Paranoia');

delete from areatrigger_actions where entry in (5631);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5631, 0, 1, 0, 49160, 200329, 0, 0, 0, 0, 0, 0, 0, 'Induced Paranoia');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (199837);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 199837, 0, 0, 31, 0, 3, 100652, 0, 0, 0, '', NULL);

delete from npc_spellclick_spells where npc_entry = 101329;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(101329, 46598, 1, 0);

delete from vehicle_template_accessory where EntryOrAura = 101329;
INSERT INTO `vehicle_template_accessory` (`EntryOrAura`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES 
(101329, 100652, 0, 0, 'Nightmare Bindings', 6, 30000);


