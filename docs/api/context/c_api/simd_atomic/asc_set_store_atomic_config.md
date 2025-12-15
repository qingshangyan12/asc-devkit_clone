# asc_set_store_atomic_config

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置原子操作使能位与原子操作类型。

## 函数原型

```c++
// 无需原子操作
__aicore__ inline void asc_set_store_atomic_config_none_sum();
// 配置float类型原子求和操作
__aicore__ inline void asc_set_store_atomic_config_f32_sum();
// 配置half类型原子求和操作
__aicore__ inline void asc_set_store_atomic_config_f16_sum();
// 配置int16_t类型原子求和操作
__aicore__ inline void asc_set_store_atomic_config_s16_sum();
// 配置int32_t类型原子求和操作
__aicore__ inline void asc_set_store_atomic_config_s32_sum();
// 配置int8_t类型原子求和操作
__aicore__ inline void asc_set_store_atomic_config_s8_sum();
// 配置bfloat16_t类型原子求和操作
__aicore__ inline void asc_set_store_atomic_config_bf16_sum();

```

## 参数说明

无

## 返回值说明

无。

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

无