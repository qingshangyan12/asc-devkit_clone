# muls

## 产品支持情况

- Ascend 950PR/Ascend 950DT：不支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素计算寄存器与标量的乘积。Tensor API 使用 `operator*` 表达 `muls`，支持寄存器与标量、
标量与寄存器两种操作数顺序：

$$
dst_i = src_i \times scalar
$$

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator*(const T& a, const U& b);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a`、`b` | 输入 | 一个操作数为 `reg_tensor`，另一个操作数为可转换为寄存器元素类型的标量。 |

## 数据类型

寄存器元素类型和转换后的标量类型支持 `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。

## 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor<T>`。每个有效元素为寄存器对应元素与标量的乘积；
结果 mask 与寄存器操作数的 mask 一致。

## 约束说明

- 标量不能转换为寄存器元素类型时，在标量转换处编译失败。
- 寄存器元素类型为 `bfloat16_t` 时，编译失败，并提示 `scalar operator* does not support this element type`。
- 整型乘积超出元素类型表示范围时采用非饱和截断，仅保留目标元素类型位宽内的结果。

## 调用示例

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"

__simd_vf__ inline void muls_example(__ubuf__ float* src, __ubuf__ float* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto product = src_reg * 2.0f;
    output.store(asc::te::make_coord(0, 0), product);
}
```
