DELETE FROM `playercreateinfo` WHERE (`race`=10 AND `class`=12);
DELETE FROM `playercreateinfo` WHERE (`race`=4 AND `class`=12);
INSERT INTO `playercreateinfo` (`race`, `class`, `map`, `position_x`, `position_y`, `position_z`) VALUES
('4', '12', '1481', '1180.64', '3284.96', '70.5241'),
('10', '12', '1481', '1180.64', '3284.96', '70.5241');

INSERT INTO `player_levelstats` VALUES
('4', '12', '98', '10', '10', '10', '10', '10'),
('10', '12', '98', '10', '10', '10', '10', '10');


INSERT INTO `class_expansion_requirement` VALUES ('12', '0');
