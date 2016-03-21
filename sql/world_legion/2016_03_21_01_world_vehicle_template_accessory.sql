ALTER TABLE `vehicle_template_accessory`   
  ADD COLUMN `offsetX` FLOAT DEFAULT 0  NOT NULL AFTER `seat_id`,
  ADD COLUMN `offsetY` FLOAT DEFAULT 0  NOT NULL AFTER `offsetX`,
  ADD COLUMN `offsetZ` FLOAT DEFAULT 0  NOT NULL AFTER `offsetY`,
  ADD COLUMN `offsetO` FLOAT DEFAULT 0  NOT NULL AFTER `offsetZ`;
