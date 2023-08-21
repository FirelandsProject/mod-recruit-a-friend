DELETE FROM `command` WHERE `name` IN ('raf add', 'raf info', 'raf reset');

INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('raf add', 195, 'Syntax: .raf add #characterName The command allows you to recruit the #characterName. It does not use the #, only the name of the character.'),
('raf info', 195, 'Syntax: .raf info The command show the current recruited account.'),
('raf reset', 195, 'Syntax: .raf reset Allows to reset the recruit, allowing the player to re-recruit after disconnecting.');
