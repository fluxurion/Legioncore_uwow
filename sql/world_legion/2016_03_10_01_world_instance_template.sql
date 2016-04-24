delete from instance_template where map in (1456,1458,1466,1477,1492,1493,1501,1516,1544);
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`, `bonusChance`) VALUES
(1456, 0, 'instance_eye_of_azshara', 0, 0),
(1458, 0, 'instance_neltharions_lair', 0, 0),
(1466, 0, 'instance_darkheart_thicket', 0, 0),
(1477, 0, 'instance_halls_of_valor', 0, 0),
(1492, 0, 'instance_maw_of_souls', 0, 0),
(1493, 0, 'instance_vault_of_the_wardens', 0, 0),
(1501, 0, 'instance_black_rook_hold_dungeon', 0, 0),
(1516, 0, 'instance_the_arcway', 0, 0),
(1544, 0, 'instance_violet_hold_legion', 0, 0);