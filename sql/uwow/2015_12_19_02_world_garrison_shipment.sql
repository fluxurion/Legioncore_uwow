UPDATE `creature_template` SET `gossip_menu_id`=16994 WHERE `entry`=81688;
UPDATE `creature_template` SET `gossip_menu_id`=16995 WHERE `entry`=85783;

DELETE FROM `gossip_menu` WHERE `entry`in (16994, 16995);
INSERT IGNORE INTO `gossip_menu` (`entry`, `text_id`) VALUES
(16994, 24741), -- 81688
(16995, 24692); -- 85783


DELETE FROM `gossip_menu_option` WHERE `menu_id` in (16994, 16995);
INSERT IGNORE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `box_coded`, `box_money`, `box_text`, option_id, npc_option_npcflag) VALUES
(16994, 0, 28, 'Сможешь получить руду из этого дренорского камня?', 0, 0, '', 22, 1), -- 81688
(16995, 0, 28, 'Ты можешь прорастить эти дренорские семена?', 0, 0, '', 22, 1); -- 85783


REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(0, 18, 28, 'Я хочу сделать заказ.', 22, 1, 0, 0, 0, 0, '');
UPDATE `gossip_menu_option` SET `option_icon` = 28, option_id = 22 WHERE `option_text` LIKE '%Я хочу сделать заказ%';

--
REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(0, 19, 29, 'Сделай для меня кое-что.', 23, 1, 0, 0, 0, 0, '');