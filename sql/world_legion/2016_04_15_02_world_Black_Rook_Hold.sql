delete from world_visible_distance where type = 0 and id = 1501;
INSERT INTO `world_visible_distance` (`type`, `id`, `distance`, `comment`) VALUES 
(0, 1501, 150, 'Black Rook Hold');

/* --------------------------------------
    The Amalgam of Souls  /  Слияние душ
   -------------------------------------- */

update creature set spawntimesecs = 86400 where id = 98542;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_the_amalgam_of_souls', flags_extra = 1 where entry = 98542;
update creature_template_wdb set classification = 3 where entry = 98542;
update creature_template set ScriptName = 'npc_soul_echoes_stalker' where entry = 99090;

delete from areatrigger_actions where entry in (5167);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5167, 0, 1, 0, 4096, 196517, 0, 0, 0, 0, 0, 0, 0, 'Swirling Scythe');

/* ---------------------------------------------------
    Illysanna Ravencrest  /  Иллисанна Гребень Ворона
   --------------------------------------------------- */

delete from creature where id = 100436;
update creature set spawntimesecs = 86400 where id = 98696;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_illysanna_ravencrest', flags_extra = 1 where entry = 98696;
update creature_template_wdb set classification = 3 where entry = 98696;
update creature_template set unit_flags = 33686400, ScriptName = 'npc_illysanna_eye_beam_stalker', flags_extra = 128 where entry = 100436;

delete from areatrigger_actions where entry in (5352,5376);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5352, 0, 1, 0, 4096, 197521, 0, 0, 0, 0, 0, 0, 0, 'Dark Rush - Apply'),
(5352, 1, 42, 1, 4096, 197521, 0, 0, 0, 0, 0, 0, 0, 'Dark Rush - Remove'),
(5376, 0, 1, 0, 4096, 197821, 0, 0, 0, 0, 0, 0, 0, 'Felblazed Ground - Apply'),
(5376, 1, 42, 1, 4096, 197821, 0, 0, 0, 0, 0, 0, 0, 'Felblazed Ground - Remove');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (197674);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 197674, 0, 0, 31, 0, 4, 0, 0, 0, 0, '', NULL);

delete from spell_target_filter where spellId in (197484);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(197484, 16, 1, 197478, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Dark Rush');

delete from spell_script_names where spell_id in (197394,197696);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(197394, 'spell_illysanna_periodic_energize'),
(197696, 'spell_illysanna_eye_beams');

delete from spell_target_position where id = 197622;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(197622, 1501, 3100.95, 7315.51, 105.002, 4.11);

/* --------------------------------------------
    Smashspite the Hateful  /  Хмуродроб Лютый
   -------------------------------------------- */

delete from creature where id = 100759;
update creature set spawntimesecs = 86400 where id = 98949;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_smashspite_the_hateful', flags_extra = 1 where entry = 98949;
update creature_template_wdb set classification = 3 where entry = 98949;
update creature_template set ScriptName = 'npc_smashspite_fel_bat' where entry = 100759;

delete from areatrigger_actions where entry = 5455;
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5455, 0, 1, 0, 4096, 198501, 0, 0, 0, 0, 0, 0, 0, 'Fel Vomit - Apply'),
(5455, 1, 42, 1, 4096, 198501, 0, 0, 0, 0, 0, 0, 0, 'Fel Vomit - Remove');

delete from spell_script_names where spell_id in (198114);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(198114, 'spell_smashpite_brutality_proc');

/* -------------------------------------------------------
    Lord Kurtalos Ravencrest  /  Кур'талос Гребень Ворона
   ------------------------------------------------------- */

delete from creature where id in (98970,98965,99611,101028,101054, 100861);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`) VALUES 
(98970, 1501, 0, 0, 262, 1, '', 0, 0, 3196.04, 7425.02, 270.373, 3.47808, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

delete from creature_template where entry = 99611;
INSERT INTO `creature_template` (`entry`, `gossip_menu_id`, `minlevel`, `maxlevel`, `ScaleLevelMin`, `ScaleLevelMax`, `ScaleLevelDelta`, `ScaleLevelDuration`, `exp`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Mana_mod_extra`, `Armor_mod`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `WorldEffectID`, `ControllerID`, `ScriptName`) VALUES 
(99611, 0, 112, 112, 0, 0, 0, 100, 6, 16, 0, 0, 1, 1.14286, 1.14286, 1, 0, 0, 0, 0, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 617299967, 1, 0, 0, '');

update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_latosius', flags_extra = 1 where entry = 98970;
update creature_template_wdb set classification = 3 where entry = 98970;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_kurtalos_ravencrest' where entry = 98965;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_kurtalos_trigger' where entry in (101054,100861);
update creature_template set mechanic_immune_mask = 617299967, AIName = 'SmartAI', flags_extra = 128 where entry = 100994;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_kurtalos_trigger' where entry = 101028;

delete from smart_scripts where entryorguid in (100994);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(100994, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'React Passive'),
(100994, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 199092, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Summon AT Hypnosis');

delete from areatrigger_actions where entry in (5478,5498);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5478, 0, 1, 0, 4096, 198781, 0, 0, 0, 0, 0, 0, 0, 'latosius - Whirling Blade'),
(5498, 0, 1, 0, 4096, 199141, 0, 0, 0, 0, 0, 0, 0, 'Apply Sleep'),
(5498, 1, 42, 1, 4096, 199097, 0, 0, 0, 0, 0, 0, 0, 'Remove Sleep'),
(5498, 2, 42, 1, 4096, 199141, 0, 0, 0, 0, 0, 0, 0, 'Remove Sleep');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (198961);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 198961, 0, 0, 31, 0, 3, 98965, 0, 0, 0, '', NULL);

delete from spell_script_names where spell_id in (198835);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(198835, 'spell_latosius_random_teleport');

delete from spell_target_position where id = 199058;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(199058, 1501, 3186.7, 7411.99, 270.379, 0);

delete from spell_linked_spell where spell_trigger in (-198961, -199141, 199141);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-198961, 117624, 0, 'Kurtalos Suicide'),
(199141, 199097, 0, 'Remove Sleep'),
(-199141, -199097, 0, 'Remove Sleep');


