ALTER TABLE `quest_template`   
  ADD COLUMN `UnkInt` INT(10) UNSIGNED DEFAULT 0  NOT NULL AFTER `RewardKillHonor`,
  ADD COLUMN `UnkFloat` FLOAT DEFAULT 0  NOT NULL AFTER `UnkInt`,
  ADD COLUMN `UnkInt2` INT(10) DEFAULT 0  NOT NULL AFTER `UnkFloat`,
  ADD COLUMN `UnkInt3` INT(10) DEFAULT 0  NOT NULL AFTER `AllowableRaces`;

ALTER TABLE `quest_template`   
  CHANGE `RevardXPMultiplier` `RewardXPMultiplier` FLOAT DEFAULT 1  NOT NULL;
