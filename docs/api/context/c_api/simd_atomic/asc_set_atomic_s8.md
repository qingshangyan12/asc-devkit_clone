# asc_set_atomic_s8

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

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

需要和[asc_set_atomic_add](./asc_set_atomic_add.md)，[asc_set_atomic_max](./asc_set_atomic_max.md)，[asc_set_atomic_min](./asc_set_atomic_min.md)配合使用。使用完成后，建议使用[asc_set_atomic_none](./asc_set_atomic_none.md)清空原子状态，以免影响后续相关指令功能。

## 调用示例

```c++
asc_set_atomic_min();
asc_set_atomic_s8();
asc_set_atomic_none();
```