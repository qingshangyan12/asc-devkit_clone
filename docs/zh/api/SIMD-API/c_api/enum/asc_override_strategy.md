# asc_override_strategy

asc_override_strategy用于表示饱和控制策略，在调用[asc_set_saturation_strategy](../sys_var/asc_set_saturation_strategy.md)和[asc_get_saturation_strategy](../sys_var/asc_get_saturation_strategy.md)时使用。该策略对应CTRL[60]，并影响CAST类全局饱和配置是否生效。

## 枚举类具体定义

```cpp
enum class asc_override_strategy : uint8_t {
    USE_API = 0,    // CTRL[60]=0：单指令设置饱和
    USE_GLOBAL = 1  // CTRL[60]=1：全局设置饱和
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 对应CTRL比特位 | 说明 |
| ------ | ----------- | ----------- |
| USE_API | CTRL[60]=0 | 按单指令/API路径配置饱和。此时全局CAST饱和位（CTRL[59]）不生效。 |
| USE_GLOBAL | CTRL[60]=1 | 使用全局CTRL饱和配置。此时通过[asc_set_saturation_flag](../sys_var/asc_set_saturation_flag.md)配置的`CAST`模式才会在精度转换中生效。 |
