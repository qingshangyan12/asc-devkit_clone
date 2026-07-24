# asc_saturation_mode

asc_saturation_mode用于表示不同计算类型的饱和/截断模式，在调用[asc_set_saturation_flag](../sys_var/asc_set_saturation_flag.md)和[asc_get_saturation_flag](../sys_var/asc_get_saturation_flag.md)时使用。

## 枚举类具体定义

```cpp
enum class asc_saturation_mode : uint8_t {
    FLOAT = 0,
    FLOAT8 = 1,
    INT = 2,
    CAST = 3
};
```

## 枚举值详解

|枚举值|饱和模式说明|
|------|-----------|
| FLOAT | 控制half浮点数计算的饱和模式。对应CTRL[48]比特位，0表示饱和模式（INF输出饱和为±MAX，NAN输出饱和为0），1表示非饱和模式（INF/NAN保持原输出）。 |
| FLOAT8 | 控制fp8浮点数精度转换的饱和模式。对应CTRL[50]比特位，0表示饱和模式，1表示非饱和模式。 |
| INT | 控制整数计算的饱和模式。对应CTRL[53]比特位，0表示截断模式（溢出值按目标数据类型位数截断，保留低位，舍弃高位），1表示饱和模式（溢出值饱和到±MAX）。 |
| CAST | 控制浮点转整数或整数转整数时的精度转换饱和模式。对应CTRL[59]比特位，0表示饱和模式（溢出值饱和到±MAX），1表示截断模式（溢出值按目标数据类型位数截断，保留低位，舍弃高位）。 |
