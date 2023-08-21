# Mod: Recruit a friend

[English](README.md) | [Español](README-ES.md)


## Nota importante
Recuerda que dentro del folder SQL, dentro del world, tienes una serie de textos, los cuales se deben insertar dentro de `firelands_string`. Los id que se encuentran en el archivo, dependen del número de modulos que tengas, por tal razón antes de ejecutar, por favor revisar que no esten en uso los ID de los textos usados en este módulo.

## Cual es el propósito de este módulo?

Este módulo permite a los Jugadores reclutar a un amigo dentro del juego, obteniendo todos los beneficios de ello en el juego.

El enlace se realiza a nivel de cuentas y no brinda ningun beneficio adicional, solo brinda beneficios blizzlike, los cuales son:

- Experiencia en misiones mejorada.
- Experiencia mejorada al matar un PNJ.
- Otorga un nivel a tu amigo por cada nivel adquirido.
- Invocar a tu amigo, cada 60 minutos.

Espero que este módulo sea de ayuda y que puedan disfrutarlo.

Recuerda que en el archivo worldserver.conf, tienes algunas variables las cuales puedes modificar para extender los beneficios de nivel (80, el cual es el valor por defecto).

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

## Comandos disponibles

.raf add <characterName>. Ex: .raf add Seobryn
.raf reset. Ex. .raf reset
.raf view. Ex. .raf view

## Creditos
- Nefertum (Autor del módulo para AzerothCore).
- Stevej / @Pangolp (Autor del módulo para AzerothCore).
- Seobryn / @Seobryn (Autor del port para Firelands Cata).