# histograms

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/tensor.h"`。

命名空间：`asc::te::experimental`。

统计`uint8_t`输入值的频率分布或累计分布，并在`dst`初始值上累加结果。

| 配置项 | 取值 | 含义 |
| --- | --- | --- |
| `options.half` | `histogram_half::low` | 统计输入值`[0, 127]`。 |
| `options.half` | `histogram_half::high` | 统计输入值`[128, 255]`。 |
| `options.mode` | `histogram_mode::frequency` | 每个输出元素表示对应输入值出现的次数。 |
| `options.mode` | `histogram_mode::cumulative` | 每个输出元素表示当前值及之前各值的累计次数。 |

## 函数原型

```cpp
template <const histogram_options& options = default_histogram_options>
__simd_callee__ inline reg_tensor<uint16_t> histograms(
    const reg_tensor<uint16_t>& dst, const reg_tensor<uint8_t>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `options` | 模板参数 | 选择统计区间和统计模式。 |
| `dst` | 输入 | `reg_tensor<uint16_t>`类型的统计初值。 |
| `src` | 输入 | `reg_tensor<uint8_t>`类型的待统计数据，`src.mask`标识参与统计的元素。 |

## 返回值说明

返回更新后的`reg_tensor<uint16_t>`。频率模式在`dst`上累加每个输入值的出现次数；累计模式累加不大于对应值的输入数量。结果mask等于`dst.mask`，参与统计的输入由`src.mask`决定。

## 约束说明

- 完整统计`[0, 255]`需要分别选择`low`和`high`，并使用两个目标寄存器保存结果。
- 非默认配置必须先定义具名的`constexpr histogram_options`，再作为模板实参传入。

## 调用示例

```cpp
constexpr asc::te::experimental::histogram_options high_options {
    asc::te::experimental::histogram_half::high,
    asc::te::experimental::histogram_mode::frequency};

auto src = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<uint8_t>());
auto initial = initial_input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<uint16_t>());
auto low = asc::te::experimental::histograms(initial, src);
auto high = asc::te::experimental::histograms<high_options>(initial, src);
```
