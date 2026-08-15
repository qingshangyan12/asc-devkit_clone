# 逻辑与位运算

本目录介绍 `asc::te` 命名空间下基于 `reg_tensor<T>` 的逻辑、位运算和移位接口。

## 头文件

```cpp
#include "tensor_api/experimental/reg/logical_compute.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [operator!](LogicalNot.md) | 对普通元素执行逻辑非，对 `reg_tensor<bool>` 执行布尔取反。 |
| [operator&](BitAnd.md) | 逐元素按位与。 |
| [operator\|](BitOr.md) | 逐元素按位或。 |
| [operator^](BitXor.md) | 逐元素按位异或。 |
| [operator<<](ShiftLeft.md) | 逐元素左移。 |
| [operator>>](ShiftRight.md) | 逐元素右移。 |

## 通用说明

- 普通 `reg_tensor<T>` 的结果 mask 跟随参与计算的输入寄存器。
- `reg_tensor<bool>` 表示 predicate 数据，本身不再携带独立 mask 成员。
- 类型和操作数组合不受支持时会在编译期报错。
