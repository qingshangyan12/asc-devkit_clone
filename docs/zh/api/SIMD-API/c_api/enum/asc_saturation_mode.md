# asc_saturation_mode

asc_saturation_mode用于表示饱和控制所作用的模式类型，在调用[asc_set_saturation_flag](../sys_var/asc_set_saturation_flag.md)和[asc_get_saturation_flag](../sys_var/asc_get_saturation_flag.md)时使用。

## 枚举类具体定义

```cpp
enum class asc_saturation_mode : uint8_t {
    FLOAT = 48,
    FLOAT8 = 50,
    INT = 53,
    CAST = 59
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 对应CTRL比特位 | 说明 |
| ------ | ----------- | ----------- |
| FLOAT | CTRL[48] | 浮点计算/部分浮点精度转换场景的饱和控制。开启饱和时CTRL[48]=0，关闭时为1。 |
| FLOAT8 | CTRL[50] | 浮点8相关场景的饱和控制。开启饱和时CTRL[50]=0，关闭时为1。 |
| INT | CTRL[53] | 整数计算溢出时的饱和/截断控制。开启饱和时CTRL[53]=1，关闭（截断）时为0。 |
| CAST | CTRL[59] | 浮点转整数或整数转整数时的精度转换饱和控制。开启饱和时CTRL[59]=0，关闭（截断）时为1。仅在饱和策略为`USE_GLOBAL`时生效。 |
