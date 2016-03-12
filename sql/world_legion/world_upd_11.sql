insert ignore into `creature_template` (entry) select Entry from creature_template_wdb;

UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=31, `npcflag`=0, `npcflag2`=0, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (101647, 101648, 94655);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2843, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99419, 99423, 98456, 98457, 98459, 98460, 96499, 99919, 101317, 101397, 98157, 98158, 98160, 98165, 101597);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=190, `npcflag`=3, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=92718;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2843, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=295424, `unit_flags2`=33556480, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (100543, 100545, 100548, 100549);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2643, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=1.6, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=33088, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99451;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97225, 92782, 101505);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97228;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=69208064, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=4, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101704;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (98354, 99916, 96654, 98228, 98290);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2843, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33554432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97244, 98712, 97959, 97962);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=92776;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=60, `MaxLevel`=60, `faction`=35, `npcflag`=32769, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=72676;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97265, 97273, 99631, 102393);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=3761, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99501;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97267;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97274;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=110, `MaxLevel`=110, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=3, `speed_walk`=4, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33570880, `unit_flags2`=1073743872, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96159;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=35653632, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101748;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (95046, 102714);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33024, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=9809, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=95048;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=95049;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (101760, 103429);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=2, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97297;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97303;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33587456, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101781;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2805, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (100665, 97051);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2805, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100667;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2843, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (101787, 101788, 101789, 101790, 96930, 96931, 94704, 94705);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=4760, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=102905;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=32768, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=102906;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=102907;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (102908, 96228);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=102909;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=1, `BaseAttackTime`=1500, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97330, 97333);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=0.888, `BaseAttackTime`=1500, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97334;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2805, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97346, 97090);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96230;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96231;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100703;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2792, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=35653632, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98482;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33587200, `unit_flags2`=0, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (100717, 100061, 101332, 100257, 101446, 101585);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=35653632, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98483;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=16384, `unit_flags2`=35653632, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98484;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=35653632, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98486;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.031746, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (96252, 93707);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1.2, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97370;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.031746, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96253;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=2.182543, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100724;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=90, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=37750784, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98497;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=14, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=67160064, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=2100, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97382;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=96276;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (96277, 93716);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (96278, 97034);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=0.888, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=4318, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96279;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96280;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99632, 92985, 100035, 100036, 100037, 100038, 100039, 100040, 100041, 100042, 100043, 100044, 102391, 103581, 103582, 103583, 103584, 103586, 103587, 103588);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=190, `npcflag`=0, `npcflag2`=0, `speed_run`=0.8571429, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33280, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99650, 99656);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=3, `MinLevel`=85, `MaxLevel`=85, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (33765, 23837, 24021, 100469);
UPDATE `creature_template` SET `gossip_menu_id`=18424, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (92980, 96665, 96666);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33344, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=92984;
UPDATE `creature_template` SET `gossip_menu_id`=18426, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=92986;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2500, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33600, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=92990;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.428571, `speed_walk`=1.2, `BaseAttackTime`=1538, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97459;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=0.888, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=95226;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=111, `MaxLevel`=111, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=1000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99709, 99710, 99732);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=2, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (93011, 98711, 100982);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=14, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=18432, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=2100, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101947;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (98611, 97592, 97604);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=320, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=6761, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=105316;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=111, `MaxLevel`=111, `faction`=35, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=1000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99731;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99735, 98618, 98621);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (98622, 97594, 97712);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=16384, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96400;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=16384, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96402;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=99759;
UPDATE `creature_template` SET `gossip_menu_id`=18936, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=0, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96420;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2843, `npcflag`=0, `npcflag2`=0, `speed_run`=0.5714286, `speed_walk`=0.5, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536903680, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99787;
UPDATE `creature_template` SET `gossip_menu_id`=19175, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96436;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96441;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93105;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=4, `MinLevel`=90, `MaxLevel`=90, `faction`=35, `npcflag`=32768, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=65183;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (93112, 94654);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2792, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93115;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=32768, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (93117, 93230, 94377, 94400);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (96473, 97014, 102724, 102726);
UPDATE `creature_template` SET `gossip_menu_id`=18435, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93127;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (98713, 98714, 97964, 97965);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97597;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97598, 97603);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=4760, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97599;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97600;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.031746, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97601;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=14, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97602;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96493;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=1000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96494;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96500;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96501;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96502;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96503;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.031746, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96504;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=67143680, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97624, 101320);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=0.2142857, `speed_walk`=0.6, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97629, 97634);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33600, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97632;
UPDATE `creature_template` SET `gossip_menu_id`=18832, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97643;
UPDATE `creature_template` SET `gossip_menu_id`=19012, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97644;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=1000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97676;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33280, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96561;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96562;
UPDATE `creature_template` SET `gossip_menu_id`=19015, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=1, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=0, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99914;
UPDATE `creature_template` SET `gossip_menu_id`=19133, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=1, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=34816, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99915;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.031746, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96564;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=95447;
UPDATE `creature_template` SET `gossip_menu_id`=19017, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=1, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99917;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=95449;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33600, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99918, 98292);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=95450;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=8409, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93221;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1.111112, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=5, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97706;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99962, 99963, 99964, 99965, 99966, 99967, 99968, 100065, 100066, 100296, 100298, 100299);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=4227072, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99969;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=67143680, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93280;
UPDATE `creature_template` SET `gossip_menu_id`=18899, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2131968, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=94410;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2805, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96645;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=4225, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96650;
UPDATE `creature_template` SET `gossip_menu_id`=18823, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=1, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=32768, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96652;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97770;
UPDATE `creature_template` SET `gossip_menu_id`=18776, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=1, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96653;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97771;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=536871680, `unit_flags2`=2049, `unit_flags3`=0, `dynamicflags`=32, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97772;
UPDATE `creature_template` SET `gossip_menu_id`=18935, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96655;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=100, `faction`=2804, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96656;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=190, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=18432, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96659;
UPDATE `creature_template` SET `gossip_menu_id`=18434, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=35, `npcflag`=1, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=94435;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33536, `unit_flags2`=4194304, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=7.5, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=98905;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=2.182543, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96672;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33536, `unit_flags2`=4194304, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98907;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2805, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96673;
UPDATE `creature_template` SET `gossip_menu_id`=18831, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96675;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=112, `MaxLevel`=112, `faction`=2245, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=1, `BaseAttackTime`=1500, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33088, `unit_flags2`=2048, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=10.5, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96680;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=112, `MaxLevel`=112, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=0.777776, `BaseAttackTime`=1500, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33600, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96681;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=4, `unit_flags`=32832, `unit_flags2`=2097152, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=4082, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96682;
UPDATE `creature_template` SET `gossip_menu_id`=18830, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=4225, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97806;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=3, `MinLevel`=85, `MaxLevel`=85, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=1073743872, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=37490;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=110, `MaxLevel`=110, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=3, `speed_walk`=4, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=16448, `unit_flags2`=1073743872, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=94492;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (103432, 100243);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=1140918272, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=4453, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96731;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=71370752, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=4454, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96732;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=90, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98986;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33555200, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (97881, 96767, 96768, 93764);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=2, `speed_walk`=1, `BaseAttackTime`=1500, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96783;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=512, `unit_flags2`=1174407168, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100161;
UPDATE `creature_template` SET `gossip_menu_id`=18937, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99045;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=3, `MinLevel`=85, `MaxLevel`=85, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100168;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=99, `faction`=7, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101288;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2643, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=33024, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96847;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=0.2142857, `speed_walk`=0.6, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555216, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (101318, 101321);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97971;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=110, `MaxLevel`=110, `faction`=2007, `npcflag`=0, `npcflag2`=0, `speed_run`=7.142857, `speed_walk`=20, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=768, `unit_flags2`=1073743872, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=97978;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=94651;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=1786, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100244;
UPDATE `creature_template` SET `gossip_menu_id`=17260, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=35, `npcflag`=1, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=90247;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=128, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (103655, 103658);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=94744;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=16, `npcflag`=0, `npcflag2`=0, `speed_run`=1.428571, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (100333, 100334);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=8.571428, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=1073743872, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry` IN (99218, 99351);
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=0.6, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=96997;
UPDATE `creature_template` SET `gossip_menu_id`=19544, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=1, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33600, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=105945;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32768, `unit_flags2`=33556480, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=97044;
UPDATE `creature_template` SET `gossip_menu_id`=18447, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=35, `npcflag`=1, `npcflag2`=0, `speed_run`=0.9920629, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=8, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=947, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93693;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=35, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101518;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=2805, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=0, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97053;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=16448, `unit_flags2`=4196352, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97057;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2500, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=1444, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97058;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1.111112, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2097152, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=12.5, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97059;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2500, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97069;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=100, `MaxLevel`=100, `faction`=35, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=32768, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97070;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=1107298304, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|7 WHERE `entry`=98191;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=111, `MaxLevel`=111, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=1000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=256, `unit_flags2`=67110912, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=101548;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=3, `MinLevel`=85, `MaxLevel`=85, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33555200, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=59113;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=9180, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98227;
UPDATE `creature_template` SET `gossip_menu_id`=18438, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2131968, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93759;
UPDATE `creature_template` SET `gossip_menu_id`=18864, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=2838, `npcflag`=3, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33536, `unit_flags2`=2048, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=98229;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=1140918272, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=4443, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93762;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=0, `MinLevel`=1, `MaxLevel`=1, `faction`=35, `npcflag`=0, `npcflag2`=0, `speed_run`=8.571428, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=4196352, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=99352;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=98, `MaxLevel`=98, `faction`=14, `npcflag`=16777216, `npcflag2`=0, `speed_run`=1, `speed_walk`=0.2, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=768, `unit_flags2`=18432, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=2100, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=97142;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=768, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100504;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=2, `unit_flags`=32832, `unit_flags2`=2099200, `unit_flags3`=0, `dynamicflags`=0, `VehicleId`=4497, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=93802;
UPDATE `creature_template` SET `gossip_menu_id`=0, `exp`=5, `MinLevel`=99, `MaxLevel`=99, `faction`=954, `npcflag`=0, `npcflag2`=0, `speed_run`=1.142857, `speed_walk`=1, `BaseAttackTime`=2000, `RangeAttackTime`=2000, `unit_class`=1, `unit_flags`=33554432, `unit_flags2`=1107298304, `unit_flags3`=1, `dynamicflags`=0, `VehicleId`=0, `HoverHeight`=1, `WorldEffectID`=0, `AiID`=0, `MovementIDKit`=0, `MeleeID`=0, `InhabitType`=`InhabitType`|3 WHERE `entry`=100510;

--
INSERT IGNORE INTO creature_model_info (`DisplayID`, `BoundingRadius`, `CombatReach`, `DisplayID_Other_Gender`) VALUES
(65834, 0.4284973, 1.5, 0), -- 65834
(68476, 1.026441, 1.5, 0), -- 68476
(64174, 2.25, 10.875, 0), -- 64174
(67229, 1.5, 7.5, 0), -- 67229
(51861, 0.5, 1, 0), -- 51861
(46710, 0.5, 1, 0), -- 46710
(64560, 0.9397697, 1.5, 0), -- 64560
(63247, 1.483476, 1.5, 0), -- 63247
(67052, 11.04073, 10, 0), -- 67052
(63524, 1.131085, 1.5, 0), -- 63524
(66806, 4.793067, 6, 0), -- 66806
(38615, 0.6, 1.2, 0), -- 38615
(63165, 1.5, 5, 0), -- 63165
(62572, 4.857084, 3.75, 0), -- 62572
(66774, 0.6064333, 1.8, 0), -- 66774
(66773, 0.5558971, 1.65, 0), -- 66773
(64442, 0.8338457, 2.475, 0), -- 64442
(62374, 0.75, 1.5, 0), -- 62374
(63527, 2.142, 1.5, 0), -- 63527
(20041, 0.3, 1.5, 0), -- 20041
(64233, 0.505361, 1.5, 0), -- 64233
(5233, 0.5, 1.5, 0), -- 5233
(65304, 4.763835, 9, 0), -- 65304
(64823, 3.572876, 6.75, 0), -- 64823
(65307, 3.062802, 1.125, 0), -- 65307
(69320, 0.5685312, 1.6875, 0), -- 69320
(69321, 0.5495801, 1.63125, 0), -- 69321
(69323, 1.547241, 1.5, 0), -- 69323
(69322, 0.511678, 1.51875, 0), -- 69322
(1418, 0.3828198, 1, 0), -- 1418
(64443, 0.7833096, 2.325, 0), -- 64443
(63985, 0.4668, 1.8, 0), -- 63985
(68154, 1.96, 3.2, 0), -- 68154
(20047, 0.6, 3, 0), -- 20047
(64441, 0.7327735, 2.175, 0), -- 64441
(66483, 0.19, 1.9, 0), -- 66483
(20628, 0.9076827, 1.5, 0), -- 20628
(62511, 0.8829392, 1.5, 0), -- 62511
(2177, 0.1, 1, 0), -- 2177
(68307, 0.15, 1.5, 0), -- 68307
(66613, 0.6640207, 0.54, 0), -- 66613
(67049, 0.3366, 1.65, 0), -- 67049
(62700, 6.716882, 6, 0), -- 62700
(64806, 3.885667, 3, 0), -- 64806
(65646, 8.743244, 8, 0), -- 65646
(67232, 0.4213, 1.65, 0), -- 67232
(16946, 1, 2, 0), -- 16946
(64804, 3.885667, 3, 0), -- 64804
(65485, 1, 2, 0), -- 65485
(65036, 0.875, 1.75, 0), -- 65036
(58049, 3.5, 10.5, 0), -- 58049
(65394, 0.389, 1.5, 0), -- 65394
(65392, 0.383, 1.5, 0), -- 65392
(64447, 0.306, 1.5, 0), -- 64447
(63194, 0.579843, 1.5, 0), -- 63194
(64808, 0.579843, 1.5, 0), -- 64808
(10812, 0.579843, 1.5, 0), -- 10812
(67343, 2.858301, 5.4, 0), -- 67343
(64446, 0.383, 1.5, 0), -- 64446
(64444, 0.383, 1.5, 0), -- 64444
(65447, 2.858301, 5.4, 0), -- 65447
(65921, 2.213402, 1.8, 0), -- 65921
(65028, 0.575, 1.15, 0), -- 65028
(65494, 7.145753, 13.5, 0), -- 65494
(65196, 1, 10, 0), -- 65196
(66800, 0.3320104, 0.27, 0), -- 66800
(62259, 0.389, 1.5, 0), -- 62259
(66977, 2.5, 3.75, 0), -- 66977
(67413, 1.276538, 3, 0), -- 67413
(65925, 2.677129, 1.5, 0), -- 65925
(65037, 0.575, 1.15, 0), -- 65037
(23767, 0.39, 1, 0), -- 23767
(66865, 1, 1.5, 0), -- 66865
(32803, 0.375, 1.125, 0), -- 32803
(38795, 0.78, 2, 0), -- 38795
(68169, 2.576035, 7, 0), -- 68169
(67019, 0.306, 1.5, 0), -- 67019
(67176, 0.40215, 1.575, 0), -- 67176
(65922, 2.213402, 1.8, 0), -- 65922
(64766, 0.45, 2.25, 0), -- 64766
(9129, 1.452642, 1.5, 0), -- 9129
(63967, 1.075449, 2.5, 0), -- 63967
(64416, 1.15, 1.5, 0), -- 64416
(67884, 1.15, 1.5, 0), -- 67884
(61903, 1.5, 1.5, 0), -- 61903
(61734, 0.306, 1.5, 0), -- 61734
(64417, 1.2, 1.5, 0), -- 64417
(67885, 1.2, 1.5, 0), -- 67885
(67883, 1.1, 1.5, 0), -- 67883
(69439, 0.4213, 1.65, 0), -- 69439
(64013, 0.45, 2.25, 0), -- 64013
(64780, 0.345, 1.15, 0), -- 64780
(64781, 0.375, 1.25, 0), -- 64781
(64779, 0.345, 1.15, 0), -- 64779
(61907, 0.389, 1.5, 0), -- 61907
(9018, 1.452642, 1.5, 0), -- 9018
(64767, 0.6, 3, 0), -- 64767
(64762, 1.75, 1.75, 0), -- 64762
(62750, 1.103674, 1.875, 0), -- 62750
(60790, 2.5, 2.5, 0), -- 60790
(62779, 1.103674, 1.875, 0), -- 62779
(67886, 1.75, 1.75, 0), -- 67886
(67887, 0.6, 3, 0), -- 67887
(67888, 0.45, 2.25, 0), -- 67888
(16269, 0.6, 2, 0), -- 16269
(63144, 0.75, 2.5, 0), -- 63144
(63270, 0.6, 2, 0), -- 63270
(39810, 0.5, 1, 0), -- 39810
(65935, 4.015694, 2.25, 0), -- 65935
(65308, 0.9, 3, 0), -- 65308
(64743, 1.2, 1.8, 0), -- 64743
(64742, 1.2, 1.8, 0), -- 64742
(67769, 1.2, 1.8, 0), -- 67769
(64741, 1.2, 1.8, 0), -- 64741
(66009, 3.877664, 1.5, 0), -- 66009
(63754, 2.4, 8, 0), -- 63754
(68182, 1.131085, 1.5, 0), -- 68182
(64414, 1.1, 1.5, 0), -- 64414
(21999, 0.005, 0.01, 0), -- 21999
(60090, 0.383, 1.5, 0), -- 60090
(61910, 0.383, 1.5, 0), -- 61910
(61906, 0.306, 1.5, 0), -- 61906
(61909, 0.383, 1.5, 0), -- 61909
(67022, 3.151948, 4, 0), -- 67022
(61912, 0.383, 1.5, 0), -- 61912
(61908, 0.306, 1.5, 0), -- 61908
(61732, 0.389, 1.5, 0), -- 61732
(60550, 0.3213, 1.575, 0), -- 60550
(62552, 2.91425, 2.25, 0), -- 62552
(60087, 0.389, 1.5, 0), -- 60087
(61911, 0.383, 1.5, 0), -- 61911
(16982, 0.45, 1.5, 0), -- 16982
(13069, 0.005, 0.01, 0), -- 13069
(63526, 1.131085, 1.5, 0), -- 63526
(62534, 1.210535, 1.5, 0), -- 62534
(65115, 1, 8, 0), -- 65115
(67575, 1.365371, 1.5, 0), -- 67575
(64029, 28.49417, 32, 0), -- 64029
(66377, 21.81045, 22.5, 0), -- 66377
(65683, 1, 1.5, 0), -- 65683
(11686, 0.5, 1, 0), -- 11686
(59610, 0.3785744, 1, 0), -- 59610
(65204, 4.083735, 1.5, 0), -- 65204
(22769, 0.175, 0.35, 0), -- 22769
(66213, 0.4426804, 0.36, 0), -- 66213
(66210, 0.4426804, 0.36, 0), -- 66210
(65648, 8.743244, 8, 0), -- 65648
(62593, 0.386562, 1.5, 0), -- 62593
(66015, 2.858301, 5.4, 0), -- 66015
(61698, 0.4596, 1.8, 0), -- 61698
(66159, 0.3213, 1.575, 0), -- 66159
(63986, 0.3366, 1.65, 0), -- 63986
(66396, 0.4213, 1.65, 0), -- 66396
(60791, 0.389, 1.5, 0), -- 60791
(60079, 0.4213, 1.65, 0), -- 60079
(64294, 0.15, 0.25, 0), -- 64294
(63969, 1.075449, 2.5, 0), -- 63969
(62513, 0.8829392, 1.5, 0), -- 62513
(65203, 4.083735, 1.5, 0), -- 65203
(65202, 4.083735, 1.5, 0); -- 65202

--
INSERT IGNORE INTO `creature_equip_template` (`CreatureID`, `ItemID1`, `ItemID2`, `ItemID3`) VALUES
(99419, 128361, 0, 128369), -- 99419
(92718, 32425, 0, 0), -- 92718
(99423, 128361, 0, 128369), -- 99423
(100543, 128358, 0, 128372), -- 100543
(100545, 128358, 0, 128372), -- 100545
(100548, 128358, 0, 128372), -- 100548
(100549, 128358, 0, 128372), -- 100549
(99451, 128528, 0, 128529), -- 99451
(97225, 133519, 0, 133519), -- 97225
(98354, 128359, 0, 128371), -- 98354
(97244, 128359, 0, 128371), -- 97244
(92776, 119458, 0, 0), -- 92776
(97265, 128359, 0, 128371), -- 97265
(97267, 128360, 0, 128370), -- 97267
(97273, 128359, 0, 128371), -- 97273
(97274, 128360, 0, 128370), -- 97274
(95046, 126793, 0, 0), -- 95046
(97297, 128359, 0, 128371), -- 97297
(97303, 128359, 0, 128371), -- 97303
(100665, 127543, 0, 127543), -- 100665
(100667, 126269, 0, 0), -- 100667
(101787, 128358, 0, 128372), -- 101787
(102905, 29685, 0, 0), -- 102905
(101788, 128358, 0, 128372), -- 101788
(102906, 31669, 0, 31669), -- 102906
(101789, 128358, 0, 128372), -- 101789
(102907, 17383, 0, 0), -- 102907
(101790, 128358, 0, 128372), -- 101790
(102909, 14535, 0, 0), -- 102909
(98456, 122430, 0, 0), -- 98456
(98457, 128359, 0, 128371), -- 98457
(98459, 128359, 0, 128371), -- 98459
(98460, 128359, 0, 128371), -- 98460
(97346, 127986, 0, 127986), -- 97346
(96230, 28965, 0, 0), -- 96230
(96231, 14535, 0, 0), -- 96231
(98484, 128521, 0, 0), -- 98484
(98486, 127651, 0, 0), -- 98486
(96253, 28133, 0, 28133), -- 96253
(100724, 128359, 0, 128371), -- 100724
(96277, 126276, 0, 0), -- 96277
(99631, 128359, 0, 128371), -- 99631
(99632, 128360, 0, 128370), -- 99632
(92980, 128359, 0, 128371), -- 92980
(92984, 128359, 0, 128371), -- 92984
(92985, 128360, 0, 128370), -- 92985
(92986, 128360, 0, 128370), -- 92986
(97459, 17383, 0, 0), -- 97459
(93011, 128359, 0, 128371), -- 93011
(98611, 50043, 0, 0), -- 98611
(98622, 128521, 0, 0), -- 98622
(96400, 128521, 0, 0), -- 96400
(96402, 128519, 0, 0), -- 96402
(96420, 128359, 0, 128371), -- 96420
(99787, 128358, 0, 128372), -- 99787
(96436, 128360, 0, 128370), -- 96436
(96441, 124529, 0, 0), -- 96441
(93112, 136788, 0, 0), -- 93112
(96473, 126793, 0, 0), -- 96473
(97592, 136788, 0, 0), -- 97592
(98711, 128359, 0, 128371), -- 98711
(98712, 128359, 0, 128371), -- 98712
(93127, 128359, 0, 128371), -- 93127
(98713, 128359, 0, 128371), -- 98713
(98714, 128359, 0, 128371), -- 98714
(97597, 126793, 0, 0), -- 97597
(97598, 124360, 0, 0), -- 97598
(97599, 128358, 0, 128372), -- 97599
(97600, 128358, 0, 128372), -- 97600
(97601, 28133, 0, 28133), -- 97601
(97603, 32743, 0, 13504), -- 97603
(97604, 126276, 0, 0), -- 97604
(96493, 124360, 0, 0), -- 96493
(96494, 124360, 0, 0), -- 96494
(96499, 128360, 0, 128370), -- 96499
(96500, 17383, 0, 0), -- 96500
(96501, 29685, 0, 0), -- 96501
(96502, 28965, 0, 0), -- 96502
(96504, 28133, 0, 28133), -- 96504
(100982, 128369, 0, 128369), -- 100982
(97643, 128359, 0, 128371), -- 97643
(97644, 128359, 0, 128371), -- 97644
(97676, 120477, 0, 0), -- 97676
(96561, 14535, 0, 0), -- 96561
(96562, 31669, 0, 31669), -- 96562
(99915, 128361, 0, 128369), -- 99915
(95447, 17383, 0, 0), -- 95447
(99916, 128361, 0, 128369), -- 99916
(99917, 128361, 0, 128369), -- 99917
(95449, 29685, 0, 0), -- 95449
(99918, 128361, 0, 128369), -- 99918
(95450, 31669, 0, 31669), -- 95450
(99919, 128361, 0, 128369), -- 99919
(99962, 128359, 0, 128371), -- 99962
(99963, 128359, 0, 128371), -- 99963
(99964, 128360, 0, 128370), -- 99964
(99965, 128358, 0, 128372), -- 99965
(99966, 128359, 0, 128371), -- 99966
(99967, 128361, 0, 128369), -- 99967
(99968, 128359, 0, 128371), -- 99968
(99969, 122430, 0, 0), -- 99969
(94410, 122430, 0, 0), -- 94410
(96645, 32425, 0, 0), -- 96645
(96650, 128361, 0, 128369), -- 96650
(96652, 128361, 0, 128369), -- 96652
(97770, 14535, 0, 0), -- 97770
(96653, 128358, 0, 128372), -- 96653
(97771, 28965, 0, 0), -- 97771
(96654, 128361, 0, 128369), -- 96654
(96655, 122430, 0, 0), -- 96655
(96656, 128359, 0, 128371), -- 96656
(96665, 128359, 0, 128371), -- 96665
(96666, 128359, 0, 128371), -- 96666
(96672, 128359, 0, 128371), -- 96672
(96675, 122430, 0, 0), -- 96675
(96682, 110449, 0, 110449), -- 96682
(100035, 122430, 0, 0), -- 100035
(100036, 128361, 0, 128369), -- 100036
(100037, 128359, 0, 128371), -- 100037
(100038, 128359, 0, 128371), -- 100038
(100039, 128359, 0, 128371), -- 100039
(100040, 128360, 0, 128370), -- 100040
(97806, 128361, 0, 128369), -- 97806
(100041, 128359, 0, 128371), -- 100041
(100042, 128358, 0, 128372), -- 100042
(100043, 128359, 0, 128371), -- 100043
(100044, 128361, 0, 128369), -- 100044
(100065, 128359, 0, 128371), -- 100065
(100066, 128361, 0, 128369), -- 100066
(103432, 126276, 0, 0), -- 103432
(97881, 14535, 0, 0), -- 97881
(96783, 132981, 0, 0), -- 96783
(102391, 128360, 0, 128370), -- 102391
(102393, 128359, 0, 128371), -- 102393
(99045, 128359, 0, 128371), -- 99045
(97959, 128360, 0, 128370), -- 97959
(97962, 122430, 0, 0), -- 97962
(97964, 128360, 0, 128370), -- 97964
(96847, 97129, 0, 95791), -- 96847
(97965, 122430, 0, 0), -- 97965
(101317, 128360, 0, 128370), -- 101317
(97971, 128360, 0, 128370), -- 97971
(97978, 28067, 0, 0), -- 97978
(103581, 128361, 0, 128369), -- 103581
(103582, 128361, 0, 128369), -- 103582
(103583, 128359, 0, 128371), -- 103583
(103584, 128358, 0, 128372), -- 103584
(103586, 128361, 0, 128369), -- 103586
(103587, 128358, 0, 128372), -- 103587
(103588, 128361, 0, 128369), -- 103588
(94654, 50043, 0, 0), -- 94654
(101397, 128359, 0, 128371), -- 101397
(96930, 128358, 0, 128372), -- 96930
(96931, 128358, 0, 128372), -- 96931
(94704, 128358, 0, 128372), -- 94704
(94705, 128358, 0, 128372), -- 94705
(100296, 128359, 0, 128371), -- 100296
(100298, 128358, 0, 128372), -- 100298
(100299, 128359, 0, 128371), -- 100299
(90247, 17383, 0, 0), -- 90247
(105945, 128361, 0, 128369), -- 105945
(97014, 126793, 0, 0), -- 97014
(97034, 127651, 0, 127651), -- 97034
(98157, 128359, 0, 128371), -- 98157
(98158, 128359, 0, 128371), -- 98158
(98160, 128361, 0, 128369), -- 98160
(93693, 120477, 0, 0), -- 93693
(98165, 128359, 0, 128371), -- 98165
(97051, 127986, 0, 128321), -- 97051
(97057, 124447, 0, 0), -- 97057
(97058, 52011, 0, 0), -- 97058
(93716, 32743, 0, 13504), -- 93716
(97069, 124026, 0, 0), -- 97069
(97070, 128360, 0, 128370), -- 97070
(97090, 41889, 0, 0), -- 97090
(98227, 122430, 0, 0), -- 98227
(93759, 128360, 0, 128370), -- 93759
(98228, 128360, 0, 128370), -- 98228
(98229, 128359, 0, 128371), -- 98229
(102714, 126793, 0, 0), -- 102714
(101597, 128358, 0, 128372), -- 101597
(102724, 126793, 0, 0), -- 102724
(102726, 126793, 0, 0), -- 102726
(98290, 128359, 0, 128371), -- 98290
(98292, 128359, 0, 128371); -- 98292

