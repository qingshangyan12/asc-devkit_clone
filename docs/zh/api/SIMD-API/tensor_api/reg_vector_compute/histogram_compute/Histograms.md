# histograms

> **命名空间** `asc::te` · **头文件** `tensor_api/experimental/reg/histogram_compute.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/experimental/reg/histogram_compute.h"`。

统计 `uint8_t` 输入值的频率分布或累计分布，并在传入的 `dst` 初始值上累加结果。

- `low`：统计输入值 `[0, 127]`。
- `high`：统计输入值 `[128, 255]`。
- `frequency`：每个输出元素表示对应输入值出现的次数。
- `cumulative`：每个输出元素表示当前值及之前各值的累计次数。

## 3. 函数原型

```cpp
namespace asc::te {

template <const histogram_options& options = default_histogram_options>
__simd_callee__ inline decltype(auto) histograms(
    const reg_tensor<uint16_t>& dst,
    const reg_tensor<uint8_t>& src);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `options.half` | 模板参数 | 选择 `[0, 127]` 或 `[128, 255]` 统计区间。 |
| `options.mode` | 模板参数 | 选择频率统计或累计统计。 |
| `dst` | 输入 | `uint16_t` 统计初值，计算结果在该值上累加。 |
| `src` | 输入 | 待统计的 `uint8_t` 数据；`src.mask` 指示参与统计的元素。 |

**支持的数据类型**

| `src` | `dst` 和返回值 |
| --- | --- |
| `reg_tensor<uint8_t>` | `reg_tensor<uint16_t>` |

## 5. 返回值说明

返回更新后的 `reg_tensor<uint16_t>`：

- `reg`：`frequency` 模式在 `dst.reg` 上累加各输入值的出现次数；`cumulative` 模式累加不大于当前 bin 对应值的输入数量。
- `mask`：等于 `dst.mask`。该 mask 用于约束后续计算和存储，不参与本次直方图统计；参与统计的输入由 `src.mask` 决定。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_frequency_histogram_bin0/bin1` 或
  `asc_cumulative_histogram_bin0/bin1`。
- `src.mask` 必须在调用前初始化，它决定参与统计的 `uint8_t` 输入 lane；`dst.mask` 不参与底层直方图计算，但会原样传播到返回对象，必须保证其适用于后续存储。
- 该底层 C API 未提供统一的 ZEROING 模式参数；不要依赖执行 mask 无效位置的数据，
  后续计算和存储应以返回对象的 `mask` 为准。
- 完整统计 `[0, 255]` 需要分别选择 `low` 和 `high`，并使用两个目标寄存器保存结果。
- 非默认配置必须先定义具名的 `constexpr histogram_options`，再作为模板实参传入；直接使用临时对象时，
  C++17 编译失败。
- `src.mask` 无效的输入元素不会参与统计。
- `src` 和 `dst` 分别不是 `reg_tensor<uint8_t>` 和 `reg_tensor<uint16_t>` 时，找不到匹配的重载，编译失败。
- 单个计数超过 `uint16_t` 上限 `65535` 时发生非饱和截断，结果按低 16 位保存。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/histogram_compute_tensor_api/histogram_compute_tensor_api.asc)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/histogram_compute.h"

constexpr asc::te::histogram_options high_options {
    asc::te::histogram_half::high, asc::te::histogram_mode::frequency};

__simd_vf__ inline void histograms_example(__ubuf__ uint8_t* src,
    __ubuf__ uint16_t* low_dst, __ubuf__ uint16_t* high_dst)
{
    auto src_layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_256 {});
    auto dst_layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_128 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), src_layout);
    auto low_output = asc::te::make_tensor(asc::te::make_mem_ptr(low_dst), dst_layout);
    auto high_output = asc::te::make_tensor(asc::te::make_mem_ptr(high_dst), dst_layout);
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<uint8_t>());

    asc::te::reg_tensor<uint16_t> initial;
    initial.mask = asc::te::all_mask<uint16_t>().reg;
    asc_duplicate_scalar(initial.reg, static_cast<uint16_t>(0), initial.mask);
    auto low = asc::te::histograms(initial, src_reg);
    auto high = asc::te::histograms<high_options>(initial, src_reg);
    low_output.store(asc::te::make_coord(0, 0), low);
    high_output.store(asc::te::make_coord(0, 0), high);
}
```
