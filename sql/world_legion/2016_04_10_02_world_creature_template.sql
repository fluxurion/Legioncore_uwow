ALTER TABLE `creature_template`   
  DROP COLUMN `personalloot`, 
  DROP COLUMN `VignetteId`;

ALTER TABLE `creature_template_wdb`   
  CHANGE `UnkInt` `VignetteID` INT(10) UNSIGNED DEFAULT 0  NOT NULL;
