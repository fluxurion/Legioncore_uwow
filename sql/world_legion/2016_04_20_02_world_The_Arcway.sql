delete from creature where id = 73400 and map = 1516;

/* -------------------
    Ivanyr  /  Иванир
   ------------------- */

update creature set spawntimesecs = 86400 where id = 98203;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_ivanyr', flags_extra = 1 where entry = 98203;
update creature_template_wdb set classification = 3 where entry = 98203;
update creature_template set mechanic_immune_mask = 617299967, flags_extra = 2 where entry = 98734;

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (202679,196392);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 202679, 0, 0, 31, 0, 3, 98734, 0, 0, 0, '', NULL),
(13, 1, 196392, 0, 0, 31, 0, 3, 98734, 0, 0, 0, '', NULL);

delete from spell_visual where spellId = 196392;
INSERT INTO `spell_visual` (`spellId`, `SpellVisualID`, `TravelSpeed`, `MissReason`, `ReflectStatus`, `SpeedAsTime`, `type`, `temp`, `HasPosition`, `comment`) VALUES 
(196392, 51100, 1, 0, 0, 1, 0, 0, 0, 'Overcharge Mana');

delete from spell_script_names where spell_id in (196392);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(196392, 'spell_ivanyr_overcharge_mana');

delete from spell_linked_spell where spell_trigger in (-196562);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-196562, 196570, 0, 'Volatile Magic');

delete from spell_dummy_trigger where spell_id in (196357,196562);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(196357, 0, 9, 0, 0, 0, 0, 0, 0, 2, 0, 0, 'Arcane Blast - Add Mana Cost'),
(196562, 0, 9, 0, 0, 0, 0, 0, 0, 2, 0, 0, 'Volatile Magic - Add Mana Cost');

/* --------------------------
    Corstilax  /  Корстилакс
   -------------------------- */

delete from creature where id in (99628,99446);
update creature set spawntimesecs = 86400 where id = 98205;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_corstilax', flags_extra = 1 where entry = 98205;
update creature_template_wdb set classification = 3 where entry = 98205;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_corstilax_suppression_protocol', flags_extra = 128 where entry = 99628;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_corstilax_quarantine' where entry = 99560;
update creature_template set inhabitType = 7, flags_extra = 128 where entry = 99446;
delete from npc_spellclick_spells where npc_entry = 99560;

delete from areatrigger_actions where entry in (5186);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5186, 0, 1, 0, 4096, 195362, 1, 0, 0, 0, 0, 0, 0, 'Nightwell Energy');

delete from conditions where SourceTypeOrReferenceId = 13 and SourceEntry in (196070,195820);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 196070, 0, 0, 31, 0, 3, 99628, 0, 0, 0, '', NULL),
(13, 3, 195820, 0, 0, 31, 0, 3, 99560, 0, 0, 0, '', NULL);

delete from spell_area where spell = 195362;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `classmask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
(195362, 7855, 0, 0, 0, 0, 0, 2, 0, 64, 11);

delete from spell_dummy_trigger where spell_id in (195791);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(195791, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Quarantine');

delete from spell_linked_spell where spell_trigger in (-195826);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `removeMask`, `comment`) VALUES 
(-195826, 195846, 0, 16, 'Quarantine');

delete from spell_target_filter where spellId in (196047);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(196047, 15, 1, -195804, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 'Suppression Protocol');

/* ----------------------------------
    General Xakal  /  Генерал Ксакал
   ---------------------------------- */

update creature set spawntimesecs = 86400 where id = 98206;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'boss_general_xakal', flags_extra = 1 where entry = 98206;
update creature_template_wdb set classification = 3 where entry = 98206;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_xakal_fissure', flags_extra = 128 where entry = 100342;
update creature_template set ScriptName = 'npc_xakal_dread_felbat' where entry = 100393;

delete from areatrigger_actions where entry in (5355,6940);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5355, 0, 1, 0, 4096, 197579, 0, 0, 0, 0, 0, 0, 0, 'Fel Fissure'),
(6940, 0, 1, 0, 4096, 212071, 0, 0, 0, 0, 0, 0, 0, 'Shadow Slash');

delete from spell_script_names where spell_id in (197786);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(197786, 'spell_xakal_bombardment');

/* -----------------------
    Nal'tira  /  Нал'тира
   ----------------------- */

update creature set position_z = 575, spawntimesecs = 86400, spawndist = 0, movementtype = 0 where id = 98207;
update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_naltira', flags_extra = 1 where entry = 98207;
update creature_template_wdb set classification = 3 where entry = 98207;

delete from areatrigger_actions where entry in (5605);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5605, 0, 1, 0, 4096, 200040, 0, 0, 0, 0, 0, 0, 0, 'Nether Venom Apply'),
(5605, 1, 42, 1, 4096, 200040, 0, 0, 0, 0, 0, 0, 0, 'Nether Venom Remove');

delete from spell_target_filter where spellId in (199809);
INSERT INTO `spell_target_filter` (`spellId`, `targetId`, `option`, `param1`, `param2`, `param3`, `aura`, `chance`, `effectMask`, `resizeType`, `count`, `maxcount`, `addcount`, `addcaster`, `comments`) VALUES 
(199809, 15, 4, -1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 'Blink Strikes');

delete from spell_dummy_trigger where spell_id in (199809,199956);
INSERT INTO `spell_dummy_trigger` (`spell_id`, `spell_trigger`, `option`, `target`, `caster`, `targetaura`, `bp0`, `bp1`, `bp2`, `effectmask`, `aura`, `chance`, `comment`) VALUES 
(199809, 199810, 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Blink Strikes'),
(199956, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'Nether Venom');

delete from spell_script_names where spell_id in (200227,200251);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(200227, 'spell_naltira_tangled_web'),
(200251, 'spell_naltira_tangled_web_check_dist');

/* --------------------------------------
    Advisor Vandros  /  Советник Вандрос
   -------------------------------------- */

delete from creature where id = 98208;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`) VALUES 
(98208, 1516, 7855, 7855, 6, 1, '0', 0, 0, 3369.45, 4805.58, 587.88, 3.13789, 86400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

update creature_template set unit_flags = 0, mechanic_immune_mask = 617299967, ScriptName = 'boss_advisor_vandros', flags_extra = 1 where entry = 98208;
update creature_template_wdb set classification = 3 where entry = 98208;
update creature_template set mechanic_immune_mask = 617299967, ScriptName = 'npc_vandros_chrono_shard' where entry in (102849);

delete from areatrigger_actions where entry in (5903,5879);
INSERT INTO `areatrigger_actions` (`entry`, `id`, `moment`, `actionType`, `targetFlags`, `spellId`, `maxCharges`, `aura`, `hasspell`, `chargeRecoveryTime`, `scale`, `hitMaxCount`, `amount`, `comment`) VALUES 
(5903, 0, 1, 0, 4096, 203139, 0, 0, 0, 0, 0, 0, 0, 'Force Bomb Apply'),
(5903, 1, 42, 1, 4096, 203139, 0, 0, 0, 0, 0, 0, 0, 'Force Bomb Remove'),
(5879, 0, 40, 0, 4096, 203087, 0, 0, 0, 0, 0, 0, 0, 'Force Bomb');

delete from spell_target_position where id = 203883;

delete from spell_script_names where spell_id in (203883,203914);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(203883, 'spell_vandros_teleport_plr'),
(203914, 'spell_vandros_banish_in_time_timer');






