# histograms

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/histogram_compute.h"`。

统计 `uint8_t` 输入值的频率分布或累计分布，并在传入的 `dst` 初始值上累加结果。

- `low`：统计输入值 `[0, 127]`。
- `high`：统计输入值 `[128, 255]`。
- `frequency`：每个输出元素表示对应输入值出现的次数。
- `cumulative`：每个输出元素表示当前值及之前各值的累计次数。

## 枚举定义

```cpp
enum class histogram_half {
    low,
    high,
};

enum class histogram_mode {
    frequency,
    cumulative,
};

struct histogram_options {
    histogram_half half = histogram_half::low;
    histogram_mode mode = histogram_mode::frequency;
};

inline constexpr histogram_options default_histogram_options {};
```

## 函数原型

```cpp
template <const histogram_options& options = default_histogram_options>
__simd_callee__ inline decltype(auto) histograms(
    reg_tensor<uint16_t> dst,
    reg_tensor<uint8_t> src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `options.half` | 模板参数 | 选择 `[0, 127]` 或 `[128, 255]` 统计区间。 |
| `options.mode` | 模板参数 | 选择频率统计或累计统计。 |
| `dst` | 输入 | `uint16_t` 统计初值，计算结果在该值上累加。 |
| `src` | 输入 | 待统计的 `uint8_t` 数据；`src.mask` 指示参与统计的元素。 |

## 数据类型

| `src` | `dst` 和返回值 |
| --- | --- |
| `reg_tensor<uint8_t>` | `reg_tensor<uint16_t>` |

## 返回值说明

返回更新后的 `reg_tensor<uint16_t>`。返回对象保留传入 `dst` 的 mask。

## 约束说明

- 完整统计 `[0, 255]` 需要分别选择 `low` 和 `high`，并使用两个目标寄存器保存结果。
- 仓库使用C++17，非默认配置应先定义命名的`constexpr histogram_options`，再作为模板参数传入。
- `src.mask` 无效的输入元素不会参与统计。
- 单个计数使用 `uint16_t` 保存，调用方需要处理累计值超过 65535 的风险。

## 调用示例

```cpp
constexpr asc::te::histogram_options high_options {
    asc::te::histogram_half::high,
    asc::te::histogram_mode::frequency
};

auto low = asc::te::histograms(initial, src);
auto high = asc::te::histograms<high_options>(initial, src);
```
