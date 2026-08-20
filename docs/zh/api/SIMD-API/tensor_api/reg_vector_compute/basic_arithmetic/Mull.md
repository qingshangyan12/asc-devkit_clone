# mull

> **命名空间** `asc::te` · **头文件** `tensor_api/experimental/reg/basic_arithmetic.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素执行 32 位整数宽乘，并分别得到乘积的低 32 位和高 32 位。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
__simd_callee__ inline decltype(auto) mull(
    reg_tensor<T>& high,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `high` | 输出 | 乘积的高 32 位。 |
| `src0`、`src1` | 输入 | 两个 32 位整数寄存器。 |

**支持的数据类型**

`T` 支持 `int32_t`、`uint32_t`。

## 5. 返回值说明

返回乘积低 32 位，并通过 `high` 返回高 32 位：

- 返回值 `reg`：保存 `src0.reg * src1.reg` 的低 32 位。
- `high.reg`：保存乘积的高 32 位。
- 返回值 `mask` 和 `high.mask`：均等于 `src0.mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_mull`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 底层 C API 将执行 mask 无效位置的乘积低 32 位和高 32 位结果均置零。两个返回对象仍携带
  `src0.mask`，后续计算和存储应继续遵循该 mask。
- `T` 不是 `int32_t` 或 `uint32_t` 时，编译失败，并提示 `mull supports int32_t and uint32_t`。
- `src0`、`src1` 和 `high` 的元素类型不一致时，找不到匹配的 `mull` 重载，编译失败。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/basic_arithmetic_tensor_api/)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"

__simd_vf__ inline void mull_example(__ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1,
    __ubuf__ uint32_t* low_dst, __ubuf__ uint32_t* high_dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input0 = asc::te::make_tensor(asc::te::make_mem_ptr(src0), layout);
    auto input1 = asc::te::make_tensor(asc::te::make_mem_ptr(src1), layout);
    auto low_output = asc::te::make_tensor(asc::te::make_mem_ptr(low_dst), layout);
    auto high_output = asc::te::make_tensor(asc::te::make_mem_ptr(high_dst), layout);
    auto mask = asc::te::all_mask<uint32_t>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    asc::te::reg_tensor<uint32_t> high;
    auto low = asc::te::mull(high, src0_reg, src1_reg);
    low_output.store(asc::te::make_coord(0, 0), low);
    high_output.store(asc::te::make_coord(0, 0), high);
}
```
