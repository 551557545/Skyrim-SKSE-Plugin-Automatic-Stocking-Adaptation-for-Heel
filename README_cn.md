# 上古卷轴 5-SKSE 插件 - 高跟鞋丝袜自动适配

高跟鞋穿脱动作的丝袜自动适配功能

这个插件**不一定不适用于所有mod**。因为不同mod的高跟脚模不一致，导致只能适配同模组下的丝袜模型滑块。除非你自己去做一个统一脚模，同时将所有丝袜滑块都适配这个脚模。

# 快速开始

下载[插件基础框架](https://github.com/gottyduke/PluginTutorialCN/blob/master/docs/setup/Setup.md)并编译该框架，然后将本源代码复制到路径 `SKSEPlugins/Plugins` 下。



# SKSE 插件使用指南

必须在以下路径创建配置文件：`SKSE/Automatic Hosiery Adaptation for High Heel Config`，文件名为 `config.json`。

示例路径：`F:\The Elder Scrolls V Skyrim - Special Edition\mo2\mods\modName\SKSE\Automatic Hosiery Adaptation for High Heel Config`

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
| `highHeelSlideName`        | `String`  | 丝袜滑块名称。本程序用于控制该滑块的权重值（权重值仅为 0 或 1）。<br />脱下高跟鞋时将滑块权重设为 0，穿上时设为 1。 |
| `noHighHeelSlideName`      | `String`  | 丝袜滑块名称。本程序用于控制该滑块的权重值（权重值仅为 0 或 1）。脱下高跟鞋时将滑块权重设为 1，穿上时设为 0。是 `highHeelSlideName` 的反向控制版本。 |
| `reverse`                  | `boolean` | 用于控制是否反向切换滑块状态。例子：`highHeelSlideName`穿上鞋子则滑块权重为0，脱下鞋子滑块权重为1 |
| `stockingSlotList`         | `Array`   | 丝袜装备槽位数组，可以填入多个，用英文逗号分开。通过检测玩家是否装备了对应槽位的物品，来控制滑块是否激活。<br />例如：填入38。则会判断玩家是否穿上38号插槽的装备。如果有，则当玩家脱下38号插槽装备则会清空之前的丝袜滑块<br />比如详情请见：**补充说明：支持的装备槽位** |
| `playerOnly`               | `boolean` | 控制该功能是否仅对玩家生效。<br />若设置为`true`，此功能将只作用于玩家。 |
| `highHeelSlideThreshold`   | `float`   | 浮点型数值。<br/>支持负数。<br/>推荐取值范围：[-1, 1]。数值 1 对应体型滑块 `bodySlider` 的 100%，数值 - 1 对应 - 100%。<br/>用于控制 `highHeelSlideName`参数数值。 |
| `noHighHeelSlideThreshold` | `float`   | 浮点型数值。<br/>支持负数。<br/>推荐取值范围：[-1, 1]。数值 1 对应体型滑块 `bodySlider` 的 100%，数值 - 1 对应 - 100%。<br/>用于控制 `noHighHeelSlideName`参数数值。 |

**补充说明：支持的装备槽位**

| 插槽位置 | 描述                                                         |
| -------- | ------------------------------------------------------------ |
| 1        | BP_HEAD（头部）                                              |
| 2        | BP_HEAD2（头部 2）                                           |
| 3        | BP_LEFTARM（左臂）                                           |
| 4        | BP_LEFTARM2（左臂 2）                                        |
| 5        | BP_RIGHTARM（右臂）                                          |
| 6        | BP_RIGHTARM2（右臂 2）                                       |
| 7        | BP_LEFTLEG（左腿）                                           |
| 8        | BP_LEFTLEG2（左腿 2）                                        |
| 9        | BP_LEFTLEG3（左腿 3）                                        |
| 10       | BP_RIGHTLEG（右腿）                                          |
| 11       | BP_RIGHTLEG2（右腿 2）                                       |
| 12       | BP_RIGHTLEG3（右腿 3）                                       |
| 13       | BP_BRAIN（脑部）                                             |
| 30       | SBP_30_HEAD（头部专用槽 30）                                 |
| 31       | SBP_31_HAIR（头发）                                          |
| 32       | SBP_32_BODY（身体）                                          |
| 33       | SBP_33_HANDS（手部）                                         |
| 34       | SBP_34_FOREARMS（前臂）                                      |
| 35       | SBP_35_AMULET（护身符）                                      |
| 36       | SBP_36_RING（戒指）                                          |
| 37       | SBP_37_FEET（脚部）                                          |
| 38       | SBP_38_CALVES（小腿）                                        |
| 39       | SBP_39_SHIELD（盾牌）                                        |
| 40       | SBP_40_TAIL（尾巴）                                          |
| 41       | SBP_41_LONGHAIR（长发）                                      |
| 42       | SBP_42_CIRCLET（头冠）                                       |
| 43       | SBP_43_EARS（耳朵）                                          |
| 44       | SBP_44_DRAGON_BLOODHEAD_OR_MOD_MOUTH（龙血头部 / 模组嘴巴）  |
| 45       | SBP_45_DRAGON_BLOODWINGL_OR_MOD_NECK（龙血左翼 / 模组颈部）  |
| 46       | SBP_46_DRAGON_BLOODWINGR_OR_MOD_CHEST_PRIMARY（龙血右翼 / 模组主胸部） |
| 47       | SBP_47_DRAGON_BLOODTAIL_OR_MOD_BACK（龙血尾巴 / 模组背部）   |
| 48       | SBP_48_MOD_MISC1（模组杂项 1）                               |
| 49       | SBP_49_MOD_PELVIS_PRIMARY（模组主骨盆）                      |
| 50       | SBP_50_DECAPITATEDHEAD（断头效果）                           |
| 51       | SBP_51_DECAPITATE（斩首效果）                                |
| 52       | SBP_52_MOD_PELVIS_SECONDARY（模组次骨盆）                    |
| 53       | SBP_53_MOD_LEG_RIGHT（模组右腿）                             |
| 54       | SBP_54_MOD_LEG_LEFT（模组左腿）                              |
| 55       | SBP_55_MOD_FACE_JEWELRY（模组面部饰品）                      |
| 56       | SBP_56_MOD_CHEST_SECONDARY（模组次胸部）                     |
| 57       | SBP_57_MOD_SHOULDER（模组肩部）                              |
| 58       | SBP_58_MOD_ARM_LEFT（模组左臂）                              |
| 59       | SBP_59_MOD_ARM_RIGHT（模组右臂）                             |
| 60       | SBP_60_MOD_MISC2（模组杂项 2）                               |
| 61       | SBP_61_FX01（特效槽 01）                                     |