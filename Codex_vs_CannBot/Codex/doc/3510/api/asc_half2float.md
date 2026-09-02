# asc_half2float

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持
- Kirin X90：不支持
- Kirin 9030：不支持

## 功能说明

头文件为：

```cpp
#include "c_api/asc_simd.h"
```

`asc_half2float`是RegBase类型转换接口。接口将`vector_half`中由`src_pos`选中的偶数或奇数索引元素转换为`vector_float`。由于源元素位宽为16 bit、目的元素位宽为32 bit，一个源寄存器需要按奇偶位置分两次转换。掩码未选中的输出元素置0。

$$
dst_i = \begin{cases}
float(src_{2i}), & src\_pos=ASC\_POSITION\_EVEN \\
float(src_{2i+1}), & src\_pos=ASC\_POSITION\_ODD
\end{cases}
$$

## 函数原型

[适用版本：950]

选择偶数索引元素，通过返回值输出结果。

```cpp
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

选择奇数索引元素，通过返回值输出结果。

```cpp
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

[适用版本：950]

选择偶数索引元素，通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

选择奇数索引元素，通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

### 废弃原型

> **已废弃**：不带`src_pos`的原型固定选择偶数索引。请改用带`ASC_POSITION_EVEN`的原型，显式表达源数据位置。

[适用版本：950]

```cpp
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask)
```

## 参数说明

本接口没有模板参数。

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|-----------|------|
| `dst` | 输出 | `vector_float`结果寄存器，仅引用输出原型包含该参数。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src` | 输入 | 待转换的`vector_half`源寄存器。寄存器参数不涉及`TPosition`或地址对齐。 |
| `mask` | 输入 | 有效输出元素掩码。对应位置为1的元素参与转换，为0的输出元素置0。 |
| `src_pos` | 输入 | 编译期位置常量。`ASC_POSITION_EVEN`选择偶数索引，`ASC_POSITION_ODD`选择奇数索引。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|-------------|---------------------|----------|
| `vector_half` | 不涉及 | `vector_float` |

## 返回值说明

| 返回值类型 | 含义 |
|-----------|------|
| `vector_float` | 保存转换结果，可直接传给后续寄存器计算接口。 |

引用输出原型的返回值类型为`void`，结果写入`dst`。

## 约束说明

- 本接口只能在Vector Function中调用，并且仅在AIV上生效。
- `src_pos`只能使用`ASC_POSITION_EVEN`或`ASC_POSITION_ODD`对应的编译期常量类型。
- `mask`按32位输出元素设置，并且必须在调用前赋值。
- 完整转换一个`vector_half`寄存器时，需要分别调用偶数和奇数位置原型，并把两组结果存放到不同目的区域。
- 接口封装Vector流水上的`vcvt`零填充模式，不占用UB或L1临时空间，也不分配eventID。

## 关键特性说明

### 奇偶位置选择

`ASC_POSITION_EVEN`读取`src[0]`、`src[2]`等偶数位置，`ASC_POSITION_ODD`读取`src[1]`、`src[3]`等奇数位置。位置参数是编译期常量，不能在运行时动态切换。

## 理论性能

本接口的理论性能请参考[C-API Reg矢量计算接口理论性能汇总](../../../../../docs/zh/api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)。

## 调用示例

下面的Vector Function将源寄存器的偶数、奇数索引分别转换后写入两个`float`目的区域。

```cpp
#include "c_api/asc_simd.h"

__simd_vf__ inline void half2float_vf(
    __ubuf__ float* even_dst, __ubuf__ float* odd_dst, __ubuf__ half* src)
{
    vector_half src_reg;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    asc_loadalign(src_reg, src);

    vector_float even_reg = asc_half2float(src_reg, mask, ASC_POSITION_EVEN);
    vector_float odd_reg = asc_half2float(src_reg, mask, ASC_POSITION_ODD);
    asc_storealign(even_dst, even_reg, mask);
    asc_storealign(odd_dst, odd_reg, mask);
}
```

这些调用形式由本仓库的3510单元测试`test_asc_half2float.cpp`和`test_return_vconv.cpp`覆盖。当前环境未提供`bisheng`可执行文件，因此本次未执行离线编译。

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|---------------|----------|
| `asc_float2half_rn` | 将`float`转换为`half`，方向相反且包含舍入模式。 | 需要缩窄为`half`时使用。 |
| `asc_exp_sub_half2float` | 在转换后立即执行融合指数差。 | 目标计算是指数差时使用融合接口。 |
