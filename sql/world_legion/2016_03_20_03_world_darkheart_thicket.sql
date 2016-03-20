replace INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
('102851', '0', '0', '0', '4097', '0', '203257');

update creature_template set flags_extra = '128' where entry in (102851, 100404);

delete from creature_text where entry in (96512);
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `comment`) VALUES
(96512, 0, 0, 'Defilers... I can smell the Nightmare in your blood. Be gone from these woods or suffer nature''s wrath!', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to Player'),
(96512, 1, 0, 'Defilers... so it is you who have befouled these woods with the Nightmare.', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to 0'),
(96512, 2, 0, 'Andu-falah-dor!', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to 0'),
(96512, 2, 1, 'Be purified in the light of the stars!', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to 0'),
(96512, 3, 0, 'I am the vessel of nature''s fury!', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to 0'),
(96512, 4, 0, 'By tooth and claw be purged!', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to 0'),
(96512, 5, 0, 'I will bleed the Nightmare from you!', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to 0'),
(96512, 6, 0, 'The Nightmare... clouds my thoughts no more...', 14, 0, 100, 0, 0, 0, 0, 'Arch-Druid Glaidalis to Player');