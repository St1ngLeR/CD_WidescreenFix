![](https://github.com/user-attachments/assets/23224742-5127-4d1b-831b-7695b6a1dd56)

# Crashday 1.2 - Widescreen Fix
An ASI plugin to improve widescreen support for Crashday (2006).

> [!WARNING]
> Widescreen Fix versions 2.0.0 and later are made especially for 1.2 version, for 1.0 & 1.1 game versions support use [vesrion 1.1.0](https://github.com/St1ngLeR/CD_WidescreenFix/releases/tag/v1.1.0)!

## Install & usage
To make ASI plugin work in the game you need to use [ThirteenAG's Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) and put plugin files in to game directory or folders mentioned in ThirteenAG's loader GitHub page.


This fix works automatically out of the box, but you can change the settings offered inside the `CDWidescreenFix.ini` file:

- `ForceAspectRatio` - This parameter is overrides game's aspect ratio. For automatic recognition based on the game window size use `auto`. To predefine desired aspect ratios set your own values with `X:Y` format (i.e. 4:3, 5:4, 16:9)
- `FixHUDMessagesScale` - This parameter is fixes the scaling of texture-based HUD messages (0 - disable, 1 - enable)
- `FixLightFlaresScale` - This paramater is fixes the scaling of light flares (0 - disable, 1 - enable)
