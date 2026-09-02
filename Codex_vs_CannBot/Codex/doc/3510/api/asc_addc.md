# asc_addc

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

`asc_addc`是RegBase整数加法接口。接口根据`mask`，将矢量数据寄存器`src0`、`src1`和掩码寄存器`carry_src`中的输入进位逐元素相加，将低32位写入`dst`，并将最高位进位写入掩码寄存器`carry`。接口适合实现大整数多字加法等需要显式传递进位的场景。

硬件按32位无符号位模式完成计算。对于第$i$个有效元素，计算公式为：

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, src1_i\} + \{32'b0, carry\_src_i\}
$$

## 函数原型

[适用版本：950]

对`uint32_t`元素执行带输入、输出进位的加法。

```cpp
__simd_callee__ inline void asc_addc(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0,
    vector_uint32_t src1, vector_bool carry_src, vector_bool mask)
```

[适用版本：950]

对`int32_t`元素按32位位模式执行带输入、输出进位的加法。

```cpp
__simd_callee__ inline void asc_addc(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0,
    vector_int32_t src1, vector_bool carry_src, vector_bool mask)
```

## 参数说明

本接口没有模板参数。

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|-----------|------|
| `carry` | 输出 | 输出进位，类型为掩码寄存器`vector_bool`。每个32位元素的进位写入对应4 bit组的最低位。寄存器参数不涉及`TPosition`或地址对齐。 |
| `dst` | 输出 | 加法结果，类型为`vector_uint32_t`或`vector_int32_t`。保存逐元素计算结果的低32位。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src0` | 输入 | 第一源操作数，类型与`dst`一致。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src1` | 输入 | 第二源操作数，类型与`dst`一致。寄存器参数不涉及`TPosition`或地址对齐。 |
| `carry_src` | 输入 | 输入进位，类型为掩码寄存器`vector_bool`。对应进位位为1时为当前元素加1。 |
| `mask` | 输入 | 有效元素掩码，类型为`vector_bool`。对应位置为1的元素参与计算，为0的元素不参与计算。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|-------------|---------------------|----------|
| `vector_int32_t` | `vector_int32_t` | `vector_int32_t`和`vector_bool` |
| `vector_uint32_t` | `vector_uint32_t` | `vector_uint32_t`和`vector_bool` |

## 约束说明

- 本接口只能在Vector Function中调用，并且仅在AIV上生效；非AIV调用不执行计算。
- `src0`、`src1`和`dst`必须使用同一种32位整数矢量寄存器类型。
- `mask`和`carry_src`必须在调用前赋值。未初始化的掩码寄存器内容不确定。
- 硬件输出完整的32位低位结果和进位，不受`ctrl`寄存器饱和模式影响。
- 接口封装Vector流水上的`vaddcs`指令，不占用UB或L1临时空间，也不分配eventID。

## 关键特性说明

### 进位链

`carry_src`提供当前32位分段的输入进位，`carry`提供下一分段所需的输出进位。实现多字整数加法时，应在调用之间按数据分段顺序传递进位，且不能把普通元素掩码直接当作全1输入进位使用。

## 理论性能

本接口的理论性能请参考[C-API Reg矢量计算接口理论性能汇总](../../../../../docs/zh/api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)。实际性能还受指令发射方式和上下游数据搬运影响。

## 调用示例

下面的Vector Function对64个`uint32_t`元素执行带进位加法。示例将有效元素的输入进位置1，并将结果和进位分别写回Unified Buffer（UB）。

```cpp
#include "c_api/asc_simd.h"

constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void addc_vf(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* carry_out,
    __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1)
{
    vector_uint32_t dst_reg;
    vector_uint32_t src0_reg;
    vector_uint32_t src1_reg;
    vector_bool carry;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    vector_bool carry_src = mask;

    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    asc_addc(carry, dst_reg, src0_reg, src1_reg, carry_src, mask);
    asc_storealign(dst, dst_reg, mask);
    asc_storealign(carry_out, carry);
}
```

该调用形式由本仓库的3510单元测试`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_addc.cpp`覆盖。当前环境未提供`bisheng`可执行文件，因此本次未执行离线编译。

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|---------------|----------|
| `asc_add` | 执行普通逐元素加法；部分重载可以输出进位，但不接收`carry_src`。 | 不需要级联输入进位时使用`asc_add`。 |
| `asc_subc` | 执行带借位输入、输出的减法。 | 实现多字减法时使用`asc_subc`。 |
