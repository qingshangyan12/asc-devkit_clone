# absdiff

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

逐元素计算两个输入之差的绝对值：

$$
dst_i = |src0_i - src1_i|
$$

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
__simd_callee__ inline decltype(auto) absdiff(
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，同时提供结果 mask。 |
| `src1` | 输入 | 减数。 |

**支持的数据类型**

`T` 支持 `half`、`float`，输入和返回值类型一致。

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `reg`：有效 lane 保存 `abs(src0.reg - src1.reg)`，无效 lane 置零。
- `mask`：等于 `src0.mask`；`src1.mask` 不参与执行 mask 的选择。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_abs_sub`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 底层 C API 使用 ZEROING 模式：执行 mask 无效的目的 lane 置零。返回对象仍携带相同
  `mask`，后续计算和存储应继续遵循该 mask。
- `T` 不是 `half` 或 `float` 时，编译失败，并提示 `absdiff supports half and float`。
- 两个输入寄存器的元素类型不一致时，找不到匹配的 `absdiff` 重载，编译失败。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/composite_compute_tensor_api/)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/composite_compute.h"

__simd_vf__ inline void absdiff_example(
    __ubuf__ float* src0, __ubuf__ float* src1, __ubuf__ float* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input0 = asc::te::make_tensor(asc::te::make_mem_ptr(src0), layout);
    auto input1 = asc::te::make_tensor(asc::te::make_mem_ptr(src1), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::absdiff(src0_reg, src1_reg);
    output.store(asc::te::make_coord(0, 0), result);
}
```
