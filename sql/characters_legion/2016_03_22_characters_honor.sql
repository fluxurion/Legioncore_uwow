ALTER TABLE `character_kill`
CHANGE COLUMN `guid` `Guid`  bigint(20) NOT NULL DEFAULT 0 FIRST ,
CHANGE COLUMN `victim_guid` `VictimGuid`  bigint(20) NOT NULL DEFAULT 0 AFTER `Guid`,
CHANGE COLUMN `count` `Count`  tinyint(3) NOT NULL DEFAULT 0 AFTER `VictimGuid`,
ADD COLUMN `PrestigeLevel`  tinyint(3) NOT NULL DEFAULT 0 AFTER `Count`,
ADD COLUMN `CurrentLevelHonor`  smallint(3) NOT NULL DEFAULT 0 AFTER `PrestigeLevel`,
ADD COLUMN `HonorLevel`  smallint(3) NOT NULL DEFAULT 0 AFTER `PrestigeLevel`;

ALTER TABLE character_kill RENAME character_honor;
