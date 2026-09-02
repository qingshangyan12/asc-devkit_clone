# asc_prelu

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

`asc_prelu`是RegBase PReLU（Parametric Rectified Linear Unit）接口。接口按照`mask`对`src0`逐元素计算，负半轴斜率由`alpha`的对应元素给出；结果可以写入引用参数`dst`，也可以通过返回值取得。掩码未选中的输出元素置0。

$$
dst_i = \begin{cases}
src0_i, & src0_i > 0 \\
src0_i \times alpha_i, & src0_i \le 0
\end{cases}
$$

## 函数原型

[适用版本：950]

通过引用参数输出`float`计算结果。

```cpp
__simd_callee__ inline void asc_prelu(
    vector_float& dst, vector_float src0, vector_float alpha, vector_bool mask)
```

[适用版本：950]

通过引用参数输出`half`计算结果。

```cpp
__simd_callee__ inline void asc_prelu(
    vector_half& dst, vector_half src0, vector_half alpha, vector_bool mask)
```

[适用版本：950]

通过返回值输出`float`计算结果。

```cpp
__simd_callee__ inline vector_float asc_prelu(
    vector_float src0, vector_float alpha, vector_bool mask)
```

[适用版本：950]

通过返回值输出`half`计算结果。

```cpp
__simd_callee__ inline vector_half asc_prelu(
    vector_half src0, vector_half alpha, vector_bool mask)
```

## 参数说明

本接口没有模板参数。

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|-----------|------|
| `dst` | 输出 | PReLU结果矢量寄存器，仅引用输出原型包含该参数。类型必须与`src0`和`alpha`一致。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src0` | 输入 | 待计算的`vector_half`或`vector_float`源操作数。寄存器参数不涉及`TPosition`或地址对齐。 |
| `alpha` | 输入 | 逐元素负半轴斜率，类型与`src0`一致。寄存器参数不涉及`TPosition`或地址对齐。 |
| `mask` | 输入 | 有效元素掩码。对应位置为1的元素参与计算，为0的输出元素置0。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|-------------|---------------------|----------|
| `vector_half` | `vector_half` | `vector_half` |
| `vector_float` | `vector_float` | `vector_float` |

## 返回值说明

| 返回值类型 | 含义 |
|-----------|------|
| `vector_half` | `half`输入的PReLU结果，可直接传给后续寄存器计算接口。 |
| `vector_float` | `float`输入的PReLU结果，可直接传给后续寄存器计算接口。 |

引用输出原型的返回值类型为`void`，结果写入`dst`。

## 约束说明

- 本接口只能在Vector Function中调用，并且仅在AIV上生效；非AIV引用输出原型不执行计算。
- `dst`、`src0`和`alpha`的数据类型必须一致。
- `mask`必须在调用前赋值；未初始化的掩码寄存器内容不确定。
- `src0`为负零时按负半轴处理，乘法结果遵循硬件浮点运算规则。
- 接口封装Vector流水上的`vprelu`指令，不占用UB或L1临时空间，也不分配eventID。

## 理论性能

本接口的理论性能请参考[C-API Reg矢量计算接口理论性能汇总](../../../../../docs/zh/api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)。

## 调用示例

下面的Vector Function读取两个`float`矢量，以逐元素`alpha`计算PReLU并把结果写回UB。

```cpp
#include "c_api/asc_simd.h"

__simd_vf__ inline void prelu_vf(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ float* alpha_ptr)
{
    vector_float src_reg;
    vector_float alpha_reg;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);

    asc_loadalign(src_reg, src);
    asc_loadalign(alpha_reg, alpha_ptr);
    vector_float dst_reg = asc_prelu(src_reg, alpha_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
```

该调用形式由本仓库的3510单元测试`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_prelu.cpp`覆盖。当前环境未提供`bisheng`可执行文件，因此本次未执行离线编译。

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|---------------|----------|
| `asc_relu` | 负半轴固定输出0，不接收斜率矢量。 | 不需要可学习负半轴斜率时使用`asc_relu`。 |
| `asc_leakyrelu` | 负半轴使用一个标量斜率。 | 所有元素共用同一斜率时使用`asc_leakyrelu`。 |

