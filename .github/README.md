# Mod: Recruit a friend

[English](README.md) | [Español](README-ES.md)


## Important note
Remember, that inside the SQL folder, inside world, you have a series of texts, which are going to be added inside `firelands_string`. The id, which is inside the file, depends on the number of modules you have, so before executing it, check them, so as not to delete anything that may be used in other modules.

## What is the module for?

The module allows players to recruit a friend within the game and get the benefits of the game.

The linking is at the account level

And it does not give additional rewards, only the original ones. Which are:

- Enhanced mission experience
- Enhanced experience by eliminating npc
- Give a level to a friend, for each level obtained.
- Invoke a friend, once every 60 minutes.

I hope it serves you and you can enjoy it.

Remember that within the worldserver.conf file, you have some variables, which you can modify to extend the benefits beyond level 60 (which is the default value).

```
#
#    RecruitAFriend.MaxLevel
#        Description: Highest level up to which a character can benefit from the Recruit-A-Friend
#                     experience multiplier.
#        Default:     80

RecruitAFriend.MaxLevel = 80

#
#    RecruitAFriend.MaxDifference
#        Description: Highest level difference between linked Recruiter and Friend benefit from
#                     the Recruit-A-Friend experience multiplier.
#        Default:     4

RecruitAFriend.MaxDifference = 4
```

## Available commands

.raf add <characterName>. Ex: .raf add Seobryn
.raf reset. Ex. .raf reset
.raf view. Ex. .raf view

## Credits

- Nefertum (Module author for AzerothCore).
- Stevej / @Pangolp (Module author for AzerothCore).
- Seobryn / @Seobryn (Module Port author for Firelands Cata).