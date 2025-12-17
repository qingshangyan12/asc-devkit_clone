# asc_set_atomic_s8

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设定int8_t类型的原子操作。

## 函数原型

```c++
__aicore__ inline int64_t asc_set_atomic_s8()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

需要和asc_set_atomic_add，asc_set_atomic_max，asc_set_atomic_min配合使用。使用完成后，建议使用asc_set_atomic_none清空原子状态，以免影响后续相关指令功能。

## 调用示例

```c++
asc_set_atomic_min();
asc_set_atomic_s8();
asc_set_atomic_none();
```
