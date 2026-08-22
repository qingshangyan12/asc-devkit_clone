# 逻辑与位运算

本目录介绍 `asc::te::experimental` 命名空间下基于 `reg_tensor<T>` 的逻辑、位运算和移位接口。

## 头文件

```cpp
#include "tensor_api/tensor.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

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

- 接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
`__simd_vf__` 函数直接或间接调用。
- `reg_tensor<bool>` 与其他 `reg_tensor<T>` 使用同一结构：`reg` 保存 predicate 数据，`mask` 表示
  predicate 数据的有效 lane。
- 二元接口的执行 mask 和结果 mask 跟随第一个 `reg_tensor` 操作数；另一个寄存器操作数的
  `mask` 不参与本次执行 mask 的选择。
