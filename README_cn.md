# Skyrim-SKSE-Plugin-Automatic-Stocking-Adaptation-for-Heel
Automatic Stocking Adaptation for Heel On/Off Movements



# Quick Start

Download the [plugin bootstrap](https://github.com/gottyduke/PluginTutorialCN/blob/master/docs/setup/Setup.md) and build the Bootstrap then copy this source code to `path: SKSEPlugins/Plugins`



# SKSE Plugin Usage Guide

The code must add in plugin path `SKSE/Automatic Hosiery Adaptation for High Heel Config` and filename with `config.json`.

Example path: `F:\The Elder Scrolls V Skyrim - Special Edition\mo2\mods\SKSE\SKSE\Automatic Hosiery Adaptation for High Heel Config`

```json
{
    "highHeelSlideName": "Fana188HighHeel",
    "noHighHeelSlideName": "",
    "reverse": false,
    "stockingSlotList": [38]
}
```

| FieldName           | FieldType | Description                                                  |
| ------------------- | --------- | ------------------------------------------------------------ |
| highHeelSlideName   | String    | Stocking slider. This program is used to control the weight value of the slider. The weight value can only be 0 or 1.Set the slider’s weight value to 0 when the high heels are removed; otherwise, set it to 1. |
| noHighHeelSlideName | String    | Stocking slider. This program is used to control the weight value of the slider. The weight value can only be 0 or 1.Set the slider’s weight value to 1 when the high heels are removed; otherwise, set it to 0.<br />It is the inverted version of **highHeelSlideName**.<br />Can be empty string |
| reverse             | boolean   | Used to control whether to invert the slider.                |
| stockingSlotList    | Array     | Hosiery equipment slot. It checks whether the corresponding slot is equipped on the player to control whether the slider is activated. See **Additional : supported slot** |

**Additional : supported slot**

| Slot Num | Name                                          |
| -------- | --------------------------------------------- |
| 1        | BP_HEAD                                       |
| 2        | BP_HEAD2                                      |
| 3        | BP_LEFTARM                                    |
| 4        | BP_LEFTARM2                                   |
| 5        | BP_RIGHTARM                                   |
| 6        | BP_RIGHTARM2                                  |
| 7        | BP_LEFTLEG                                    |
| 8        | BP_LEFTLEG2                                   |
| 9        | BP_LEFTLEG3                                   |
| 10       | BP_RIGHTLEG                                   |
| 11       | BP_RIGHTLEG2                                  |
| 12       | BP_RIGHTLEG3                                  |
| 13       | BP_BRAIN                                      |
| 30       | SBP_30_HEAD                                   |
| 31       | SBP_31_HAIR                                   |
| 32       | SBP_32_BODY                                   |
| 33       | SBP_33_HANDS                                  |
| 34       | SBP_34_FOREARMS                               |
| 35       | SBP_35_AMULET                                 |
| 36       | SBP_36_RING                                   |
| 37       | SBP_37_FEET                                   |
| 38       | SBP_38_CALVES                                 |
| 39       | SBP_39_SHIELD                                 |
| 40       | SBP_40_TAIL                                   |
| 41       | SBP_41_LONGHAIR                               |
| 42       | SBP_42_CIRCLET                                |
| 43       | SBP_43_EARS                                   |
| 44       | SBP_44_DRAGON_BLOODHEAD_OR_MOD_MOUTH          |
| 45       | SBP_45_DRAGON_BLOODWINGL_OR_MOD_NECK          |
| 46       | SBP_46_DRAGON_BLOODWINGR_OR_MOD_CHEST_PRIMARY |
| 47       | SBP_47_DRAGON_BLOODTAIL_OR_MOD_BACK           |
| 48       | SBP_48_MOD_MISC1                              |
| 49       | SBP_49_MOD_PELVIS_PRIMARY                     |
| 50       | SBP_50_DECAPITATEDHEAD                        |
| 51       | SBP_51_DECAPITATE                             |
| 52       | SBP_52_MOD_PELVIS_SECONDARY                   |
| 53       | SBP_53_MOD_LEG_RIGHT                          |
| 54       | SBP_54_MOD_LEG_LEFT                           |
| 55       | SBP_55_MOD_FACE_JEWELRY                       |
| 56       | SBP_56_MOD_CHEST_SECONDARY                    |
| 57       | SBP_57_MOD_SHOULDER                           |
| 58       | SBP_58_MOD_ARM_LEFT                           |
| 59       | SBP_59_MOD_ARM_RIGHT                          |
| 60       | SBP_60_MOD_MISC2                              |
| 61       | SBP_61_FX01                                   |