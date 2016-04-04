ALTER TABLE `spell_area`   
  ADD COLUMN `classmask` INT(11) DEFAULT 0  NOT NULL AFTER `racemask`, 
  DROP PRIMARY KEY,
  ADD PRIMARY KEY (`spell`, `area`, `quest_start`, `aura_spell`, `racemask`, `classmask`, `gender`);

insert into `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `classmask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) values('208643','7877','0','0','0','0','-4','2','0','64','11');
insert into `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) values('208649','1220','4495.8','4849.9','662','1.338668');
