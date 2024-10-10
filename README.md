# FPS Puzzle Game

This project is a FPS/Puzzle game, where you try to complete levels as fast as possible, using two abilties: [Telekinesis](Source/GameplayAbilities/TelekinesisComponent.cpp) and a [Blink](Source/GameplayAbilities/BlinkComponent.cpp).

## Abilities

The abilities are encapsulated inside components, which can be added/removed from the character easily. [Ability Component](Source/GameplayAbilities/AbilityComponent.cpp) features a light version of Unreal's Gameplay Ability System, allowing delegates to be attached to be attached upon entering and leaving an ability. They work hand in hand with the [Player Controller](Source/GameplayAbilities/GameplayAbilitiesPlayerController.cpp) to ensure only one can be used at any time.

## Menus

The game features a main menu and a level selection menu, which are automatically populated with the levels created.

## Data

The game keeps track of the time a player takes to finish a level, as well as if the level was completed without Blink, Telekinesis, or either of the abilities in a [Save Game](Source/GameplayAbilities/GameplayAbilitiesSaveGame.h) file. This provides an extra challenge in trying to find the way to do it.
