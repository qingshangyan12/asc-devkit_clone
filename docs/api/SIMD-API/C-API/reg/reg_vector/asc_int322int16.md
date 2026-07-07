# asc_int322int16

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

## 功能说明

- 接口声明文件：`c_api/reg_compute/reg_convert.h`
- 核心功能：将 int32_t 类型转化为 int16_t 类型，支持饱和/非饱和模式。
- 实现原理/公式：由于源操作数与目的操作数类型位宽比为 2:1，写入数据时需要将一个 VL 大小的数据分为两部分，根据不同接口选取索引 0 或者索引 1。关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式](./rounding_mode.md)。

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
// 非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
// 饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
// 非饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_int322int16_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
// 饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_int322int16_sat_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
```

<!-- end id10 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

<!-- end id11 -->

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| vector_int32_t | 不涉及 | vector_int16_t |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，操作数均为矢量数据寄存器。
- 同步资源占用：占用 PIPE_V 流水资源，使用矢量数据寄存器和掩码寄存器。
- 资源占用与参数/数据量关系：资源占用固定，不随参数或数据量变化。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 饱和模式约束：int32_t 转 int16_t 为窄化转换，支持饱和模式和不饱和模式，需配置 ctrl 寄存器，ctrl 寄存器的详细说明请参见[asc_set_ctrl.md](../../sys_var/asc_set_ctrl.md)。
- 不饱和模式：输入数据截断为目标数据宽度，例如输入 int32_t 值为 32768，输出 int16_t 值为 -32768。
- 饱和模式：输入数据超出 int16_t 表示范围时，饱和为 int16_t 最值，例如输入 int32_t 值为 32768，输出 int16_t 值为 32767；输入负数超出范围时饱和为 -32768。
- 异常和边界值处理：mask 未筛选的元素在输出中置零。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int32_t | vector_int16_t | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
vector_int32_t src;
vector_int16_t dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_int322int16(dst, src, mask);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/blob/master/tests/api/c_api/npu_arch_3510/vector_compute/test_asc_int322int16.cpp`

<!-- end id15 -->
