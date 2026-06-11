# Skyrim-SKSE-Plugin-Automatic-Stocking-Adaptation-for-Heel
Automatic Stocking Adaptation for Heel On/Off Movements

This plugin is **not necessarily incompatible with all mods**. Due to the inconsistency of high-heel foot models across different mods, it can only be compatible with the stocking model sliders under the same mod group. Unless you create a unified foot model yourself and adapt all stocking sliders to this foot model at the same time.



# SKSE Plugin Usage Guide

1.Install the mod with mo2

2.Open the `config.json`  and edit it .path in : `SKSE\Automatic Hosiery Adaptation for High Heel Config`

```json
{
    "highHeelSlideName": "your high slide name",
    "highHeelSlideThreshold": 1,
    "noHighHeelSlideName": "",
    "noHighHeelSlideThreshold": 1,
    "reverse": false,
    "playerOnly": true,
    "stockingSlotList": [38,53]
}
```

| FieldName                  | FieldType | Description                                                  |
| -------------------------- | --------- | ------------------------------------------------------------ |
| `highHeelSlideName`        | `String`  | 1.Stocking slider. <br />2.This program is used to control the weight value of the slider.<br />3.The weight value can only be 0 or 1.Set the slider’s weight value to 0 when the high heels are removed; otherwise, set it to 1. |
| `noHighHeelSlideName`      | `String`  | 1.Stocking slider.<br />2.This program is used to control the weight value of the slider.<br />3.The weight value can only be 0 or 1.Set the slider’s weight value to 1 when the high heels are removed; otherwise, set it to 0.<br />4.It is the inverted version of `highHeelSlideName`.<br />5.Can be empty string |
| `reverse`                  | `boolean` | 1.Used to control whether to reverse the slider state. 2.Example: For the slider named `highHeelSlideName`, the slider weight is 0 when the shoes are worn, and 1 when the shoes are taken off. |
| `stockingSlotList`         | `Array`   | 1.Stocking equipment slot array.<br />2.Multiple entries can be filled in, separated by English commas. The activation status of the sliders is controlled by detecting whether the player has equipped items in the corresponding slots. <br />3.Example: Enter 38. The plugin will then check whether the player has equipped the item in slot 38. If so, the previous stocking sliders will be reset when the player unequips the item in slot 38.<br />See **Additional : supported slot** |
| `playerOnly`               | `boolean` | 1.Control whether it is used only for player<br />2.If set to true, the feature will only take effect for the player. |
| `highHeelSlideThreshold`   | `float`   | 1.A numeric value of float type.<br/>2.Negative numbers are supported.<br/>3.The recommended setting range is [-1, 1]. The value 1 corresponds to 100% in the `bodySlider` slider, while -1 corresponds to -100%.<br />4.Used to control the value of `highHeelSlideName`. |
| `noHighHeelSlideThreshold` | `float`   | 1.A numeric value of float type.<br/>2.Negative numbers are supported.<br/>3.The recommended setting range is [-1, 1]. The value 1 corresponds to 100% in the `bodySlider` slider, while -1 corresponds to -100%.<br />4.Used to control the value of `noHighHeelSlideName`. |







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