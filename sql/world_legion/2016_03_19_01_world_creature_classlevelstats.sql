ALTER TABLE `creature_classlevelstats`   
  DROP COLUMN `damage_base`, 
  DROP COLUMN `damage_exp1`, 
  DROP COLUMN `damage_exp2`, 
  DROP COLUMN `damage_exp3`, 
  DROP COLUMN `damage_exp4`, 
  DROP COLUMN `damage_exp5`,
  DROP COLUMN `comment`;


insert ignore into `creature_classlevelstats` (select `minlevel`,`unit_class`,1,1,506,308 from creature_template where unit_class = 1);
INSERT IGNORE INTO `creature_classlevelstats` (SELECT `minlevel`,`unit_class`,1,1,1254,154 FROM creature_template WHERE unit_class = 2);
INSERT IGNORE INTO `creature_classlevelstats` (SELECT `minlevel`,`unit_class`,1,1,506,308 FROM creature_template WHERE unit_class = 4);
INSERT IGNORE INTO `creature_classlevelstats` (SELECT `minlevel`,`unit_class`,1,1,374,110 FROM creature_template WHERE unit_class = 8);
