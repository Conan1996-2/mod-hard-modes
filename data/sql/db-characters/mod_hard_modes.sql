CREATE TABLE IF NOT EXISTS `mod_hard_modes` (
    `account` int(11) NOT NULL DEFAULT '0' COMMENT 'Account of player.',
    `GUID` int(11) NOT NULL DEFAULT '0' COMMENT 'Character of player.',
    `deaths` int(11) NOT NULL DEFAULT '0' COMMENT 'Number of deaths for the character',
    `reprieves` int(11) NOT NULL DEFAULT '0' COMMENT 'Number of times the character used currency',
    `currency` int(11) NOT NULL DEFAULT '0' COMMENT 'Amount of currency'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
