# muls_cast

## 产品支持情况

- Ascend 950PR/Ascend 950DT：不支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/experimental/reg/composite_compute.h"`。

将 `float` 输入与标量逐元素相乘，再将结果转换为 `half`：

$$
dst_i = cast\_to\_half(src_i \times scalar)
$$

## 函数原型

```cpp
__simd_callee__ inline decltype(auto) muls_cast(
    const reg_tensor<float>& src,
    const float& scalar);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | `reg_tensor<float>` 输入。 |
| `scalar` | 输入 | `float` 标量乘数。 |

## 数据类型

输入为 `reg_tensor<float>` 和 `float` 标量，返回 `reg_tensor<half>`。

## 返回值说明

返回 `reg_tensor<half>`。每个有效元素先以 `float` 计算 `src * scalar`，再将乘积转换为 `half`；
结果 mask 与 `src.mask` 一致。

## 约束说明

- `src` 不是 `reg_tensor<float>` 或 `scalar` 不是 `float` 时，找不到匹配的 `muls_cast` 重载，编译失败。
- `float` 乘积转换为 `half` 时按底层 `asc_muls` 的舍入规则处理；超出 `half` 表示范围时产生 `inf`，
  绝对值过小时可能下溢为 `0`。

## 调用示例

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/reg/composite_compute.h"

__simd_vf__ inline void muls_cast_example(__ubuf__ float* src, __ubuf__ half* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto result = asc::te::muls_cast(src_reg, 1.5f);
    output.store(asc::te::make_coord(0, 0), result);
}
```
