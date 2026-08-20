# muls_cast

> **命名空间** `asc::te` · **头文件** `tensor_api/experimental/reg/composite_compute.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/experimental/reg/composite_compute.h"`。

将 `float` 输入与标量逐元素相乘，再将结果转换为 `half`。对第 `i` 个有效的
`float` 输入，计算结果写入返回寄存器的第 `2 * i` 个 `half` 位置：

$$
dst_{2i} = cast\_to\_half(src_i \times scalar)
$$

底层 `asc_muls` 使用 `PART_EVEN` 布局，返回值并不是普通的连续 `half` 数据。

## 3. 函数原型

```cpp
namespace asc::te {

__simd_callee__ inline decltype(auto) muls_cast(
    const reg_tensor<float>& src,
    const float& scalar);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | `reg_tensor<float>` 输入，同时提供执行 mask。 |
| `scalar` | 输入 | `float` 标量乘数。 |

**支持的数据类型**

| `src` | `scalar` | 返回值 |
| --- | --- | --- |
| `reg_tensor<float>` | `float` | `reg_tensor<half>`，有效结果位于偶数 `half` 位置 |

## 5. 返回值说明

返回 `reg_tensor<half>`：

- `reg`：第 `i` 个有效 `float` 输入的结果写入第 `2 * i` 个 `half` 位置，即偶数位置；
  奇数 `half` 位置不保存本接口的计算结果。
- `mask`：等于 `src.mask`，描述 `float` 输入 lane 及其对应偶数 `half` 结果是否有效。
  后续操作必须按 EVEN 布局解释该 mask，不能将其当作连续 `half` lane 的 mask。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用；底层调用 `asc_muls`。非 AIV 分支不执行矢量计算，
  不应使用其返回结果。
- `src.mask` 必须在调用前初始化，通常使用 `load(...).with_mask(m)`。`m` 可由
  `update_mask<float>(remain)`、`make_mask<pattern, float>()` 或 `all_mask<float>()` 生成；
  未初始化的 mask 内容不确定。
- 3510 架构的单个矢量寄存器宽度为 256 字节，可容纳 64 个 `float` 或 128 个 `half`。
  64 个 `float` 输入的转换结果依次位于 `half` 位置 0、2、...、126。
- 底层 `asc_muls` 对执行 mask 无效的结果位置置零。奇数 `half` 位置不属于本接口的结果，
  不应读取或存储为有效数据。
- 不能使用普通的 `local_tensor<half>::store` 将返回寄存器直接存成连续 `half` 数据。
  可使用 `asc_half2float(..., ASC_POSITION_EVEN)` 将偶数位置结果转换回连续 `float`，或使用
  经过验证的 EVEN 布局压缩方案后再存储。
- `src` 不是 `reg_tensor<float>` 或 `scalar` 不是 `float` 时，找不到匹配的 `muls_cast` 重载，
  编译失败。
- `float` 乘积转换为 `half` 时采用 ROUND 舍入；超出 `half` 表示范围时产生 `inf`，
  绝对值过小时可能下溢为 `0`。

## 7. 调用示例

以下示例先调用 `muls_cast`，再按 EVEN 布局将 `half` 结果转换回连续 `float`，最后写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/composite_compute_tensor_api/)。

```cpp
#include "c_api/asc_simd.h"
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/composite_compute.h"

__simd_vf__ inline void muls_cast_example(__ubuf__ float* src, __ubuf__ float* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto half_result = asc::te::muls_cast(src_reg, 1.5f);
    asc::te::reg_tensor<float> converted;
    asc_half2float(converted.reg, half_result.reg, half_result.mask, ASC_POSITION_EVEN);
    converted.mask = half_result.mask;
    output.store(asc::te::make_coord(0, 0), converted);
}
```
