
SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `complaints`
-- ----------------------------
DROP TABLE IF EXISTS `complaints`;
CREATE TABLE `complaints` (
  `PlayerGuid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `ComplaintType` smallint(3) unsigned NOT NULL DEFAULT '0',
  `MailID` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeSinceOffence` int(10) unsigned NOT NULL DEFAULT '0',
  `MessageLog` longtext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- ----------------------------
-- Records of complaints
-- ----------------------------
