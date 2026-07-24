# asc_override_strategy

asc_override_strategy用于表示饱和模式的覆盖策略，在调用[asc_set_saturation_strategy](../sys_var/asc_set_saturation_strategy.md)和[asc_get_saturation_strategy](../sys_var/asc_get_saturation_strategy.md)时使用。

## 枚举类具体定义

```cpp
enum class asc_override_strategy : uint8_t {
    USE_API = 0,
    USE_GLOBAL = 1
};
```

## 枚举值详解

|枚举值|覆盖策略说明|
|------|-----------|
| USE_API | 使用API级饱和配置，即由[asc_set_saturation_flag](../sys_var/asc_set_saturation_flag.md)设置的饱和标志生效。对应CTRL[60]比特位置0。 |
| USE_GLOBAL | 使用全局饱和配置，忽略API级设置。对应CTRL[60]比特位置1。 |
