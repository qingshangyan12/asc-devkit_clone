# asc_exp_sub

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

`asc_exp_sub`是RegBase融合指数差接口。`float`输入使用`asc_exp_sub`，`half`输入使用`asc_exp_sub_half2float`；后者先选择源寄存器的偶数或奇数索引元素，将其扩展为`float`，再完成相减和自然指数计算。结果可以写入引用参数，也可以通过返回值取得。

对于`float`输入，第$i$个有效输出元素为：

$$
dst_i = e^{src0_i-src1_i}
$$

对于`half`输入，位置参数决定读取的源元素：

$$
dst_i = \begin{cases}
e^{float(src0_{2i})-float(src1_{2i})}, & src\_pos=ASC\_POSITION\_EVEN \\
e^{float(src0_{2i+1})-float(src1_{2i+1})}, & src\_pos=ASC\_POSITION\_ODD
\end{cases}
$$

## 函数原型

### float输入

[适用版本：950]

通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_exp_sub(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

[适用版本：950]

通过返回值输出结果。

```cpp
__simd_callee__ inline vector_float asc_exp_sub(
    vector_float src0, vector_float src1, vector_bool mask)
```

### half输入并转换为float

[适用版本：950]

选择偶数索引元素，通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

选择奇数索引元素，通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

[适用版本：950]

选择偶数索引元素，通过返回值输出结果。

```cpp
__simd_callee__ inline vector_float asc_exp_sub_half2float(
    vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

选择奇数索引元素，通过返回值输出结果。

```cpp
__simd_callee__ inline vector_float asc_exp_sub_half2float(
    vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

### 废弃原型

> **已废弃**：以下兼容原型已由公共头文件标记为废弃。`half`输入应改用带显式`src_pos`的`asc_exp_sub_half2float`；`float`输入应使用`asc_exp_sub`。

[适用版本：950]

```cpp
__simd_callee__ inline void asc_exp_sub(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)
```

[适用版本：950]

```cpp
__simd_callee__ inline void asc_exp_sub_v2(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)
```

[适用版本：950]

```cpp
__simd_callee__ inline void asc_exp_sub_v2(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

## 参数说明

本接口没有模板参数。

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|-----------|------|
| `dst` | 输出 | `vector_float`结果寄存器，仅引用输出原型包含该参数。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src0` | 输入 | 被减数寄存器，类型为`vector_float`或`vector_half`。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src1` | 输入 | 减数寄存器，类型必须与`src0`一致。寄存器参数不涉及`TPosition`或地址对齐。 |
| `mask` | 输入 | 有效输出元素掩码。对应位置为1的元素参与计算，为0的输出元素置0。 |
| `src_pos` | 输入 | 编译期位置常量。`ASC_POSITION_EVEN`选择偶数索引元素，`ASC_POSITION_ODD`选择奇数索引元素。仅`asc_exp_sub_half2float`包含该参数。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|-------------|---------------------|----------|
| `vector_half` | `vector_half` | `vector_float` |
| `vector_float` | `vector_float` | `vector_float` |

## 返回值说明

| 返回值类型 | 含义 |
|-----------|------|
| `vector_float` | 保存指数差结果，可直接传给后续寄存器计算接口。 |

引用输出原型的返回值类型为`void`，结果写入`dst`。

## 约束说明

- 本接口只能在Vector Function中调用，并且仅在AIV上生效。
- `src0`和`src1`的数据类型必须一致，输出固定为`vector_float`。
- `mask`必须在调用前赋值；未初始化的掩码寄存器内容不确定。
- `half`输入必须显式传入`ASC_POSITION_EVEN`或`ASC_POSITION_ODD`，以确定每个输出对应的源索引。
- 只有输入为`half`、输出为`float`时存在位宽扩展带来的精度提升。
- 接口封装Vector流水上的`vexpdif`指令，不占用UB或L1临时空间，也不分配eventID。

## 关键特性说明

### 融合计算

接口将减法、指数计算以及可选的`half`到`float`转换融合到一条寄存器计算路径中，避免在中间结果上增加额外的寄存器指令。选择`half`源数据时，应根据实际数据排布选择偶数或奇数位置。

## 理论性能

本接口的理论性能请参考[C-API Reg矢量计算接口理论性能汇总](../../../../../docs/zh/api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)。

## 调用示例

下面的Vector Function分别演示`float`输入和`half`偶数索引输入。两个结果均为`vector_float`。

```cpp
#include "c_api/asc_simd.h"

__simd_vf__ inline void exp_sub_vf(
    __ubuf__ float* float_dst, __ubuf__ float* half_dst,
    __ubuf__ float* float_src0, __ubuf__ float* float_src1,
    __ubuf__ half* half_src0, __ubuf__ half* half_src1)
{
    vector_float f0;
    vector_float f1;
    vector_half h0;
    vector_half h1;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);

    asc_loadalign(f0, float_src0);
    asc_loadalign(f1, float_src1);
    asc_loadalign(h0, half_src0);
    asc_loadalign(h1, half_src1);

    vector_float float_result = asc_exp_sub(f0, f1, mask);
    vector_float half_result = asc_exp_sub_half2float(
        h0, h1, mask, ASC_POSITION_EVEN);
    asc_storealign(float_dst, float_result, mask);
    asc_storealign(half_dst, half_result, mask);
}
```

这些调用形式由本仓库的3510单元测试`test_asc_exp_sub.cpp`和`test_return_vconv.cpp`覆盖。当前环境未提供`bisheng`可执行文件，因此本次未执行离线编译。

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|---------------|----------|
| `asc_exp` | 仅计算单个输入的自然指数。 | 已经得到差值或不需要融合减法时使用`asc_exp`。 |
| `asc_sub` | 仅执行逐元素减法。 | 需要保留差值或采用其他指数处理时使用`asc_sub`。 |
| `asc_half2float` | 仅完成`half`到`float`转换。 | 不需要同时计算指数差时使用`asc_half2float`。 |
