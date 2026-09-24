# histograms

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

统计`uint8_t`输入值的频率分布或累计分布，并在`dst`初始值上累加结果。

令$h$表示统计区间的起始值：`low`模式下$h=0$，`high`模式下$h=128$。对于输出位置
$i\in[0,127]$，两种统计模式分别为：

$$
\begin{aligned}
result_i^{frequency} &= dst_i + \sum_{j:\,src.mask_j=1}\mathbf{1}(src_j=h+i) \\
result_i^{cumulative} &= dst_i + \sum_{j:\,src.mask_j=1}\mathbf{1}(src_j\le h+i)
\end{aligned}
$$

| 配置项 | 取值 | 含义 |
| --- | --- | --- |
| `options.half` | `histogram_half::low` | 统计输入值`[0, 127]`。 |
| `options.half` | `histogram_half::high` | 统计输入值`[128, 255]`。 |
| `options.mode` | `histogram_mode::frequency` | 每个输出元素表示对应输入值出现的次数。 |
| `options.mode` | `histogram_mode::cumulative` | 每个输出元素表示当前值及之前各值的累计次数。 |

## 函数原型

```cpp
template <const histogram_options& options = default_histogram_options, typename T, typename U>
__simd_callee__ inline reg_tensor<T> histograms(
    reg_tensor<T>& dst, const reg_tensor<U>& src)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| options | 直方图配置，类型为histogram_options。默认统计[0, 127]范围内各数值出现的次数。 |
| T | 输出元素类型。目前支持`uint16_t`。 |
| U | 输入元素类型。目前支持`uint8_t`。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `dst` | 输入/输出 | 统计初值和结果，类型为`reg_tensor<uint16_t>`。接口直接更新`dst.reg`，返回值的`mask`与`dst.mask`相同。 |
| `src` | 输入 | 待统计数据，类型为`reg_tensor<uint8_t>`。`src.mask`用于标记参与统计的元素，对应位置为`1`时参与统计，为`0`时不参与统计。 |

## 数据类型

dst和返回值的元素类型为uint16_t，src的元素类型为uint8_t。

## 返回值说明

返回更新后的`reg_tensor<uint16_t>`。频率模式在`dst`上累加每个输入值的出现次数，累计模式累加不大于对应值的输入数量。返回值的`mask`与`dst.mask`相同。

## 约束说明

- `options.half`只能取`histogram_half::low`或`histogram_half::high`，`options.mode`只能取`histogram_mode::frequency`或`histogram_mode::cumulative`。
- src.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与统计的元素位置错误。
- `src.mask`只用于筛选参与统计的输入元素，不用于筛选返回值中的统计位置。返回值中的所有位置都会在`dst`原值的基础上更新。
- 统计结果在`dst`原有数据的基础上累加。调用前必须初始化`dst.reg`，多次调用时后一次结果累加到前一次结果中。返回值的`mask`继承`dst.mask`，将返回值用于`store`前需要通过`with_mask`设置`dst.mask`。
- `dst`的元素类型为`uint16_t`，单个统计结果的最大值为65535。用户需要保证累加结果不超过该范围，否则会发生溢出。
- 完整统计`[0, 255]`需要分别选择`low`和`high`，并使用两个独立的目标寄存器保存结果。
- 非默认配置必须先定义具名的`constexpr histogram_options`，再作为模板实参传入。

## 调用示例

以下示例分别统计输入数据低半区和高半区的频率分布。

```cpp
#include "tensor_api/experimental/vector_compute.h"

constexpr asc::te::experimental::histogram_options high_options {
    asc::te::experimental::histogram_half::high,
    asc::te::experimental::histogram_mode::frequency};

template <typename SrcTensor, typename InitialTensor, typename OutputTensor>
__simd_vf__ inline void histograms_example(
    SrcTensor input, InitialTensor initial_input, OutputTensor output)
{
    auto src_mask = asc::te::experimental::all_mask<uint8_t>();
    auto dst_mask = asc::te::experimental::all_mask<uint16_t>();
    auto src = asc::te::experimental::load(input, asc::te::make_coord(0)).with_mask(src_mask);
    auto initial_low = asc::te::experimental::load(initial_input, asc::te::make_coord(0)).with_mask(dst_mask);
    auto initial_high = asc::te::experimental::load(initial_input, asc::te::make_coord(0)).with_mask(dst_mask);
    auto low = asc::te::experimental::histograms(initial_low, src);
    auto high = asc::te::experimental::histograms<high_options>(initial_high, src);

    asc::te::experimental::store(output, asc::te::make_coord(0), low);
    asc::te::experimental::store(output, asc::te::make_coord(1), high);
}
```
