# asc_int322uint16

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
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明
头文件为：`#include "c_api/reg_compute/reg_convert.h"`

将 vector_int32_t 类型转换成 vector_uint16_t 类型，写入目的操作数的上半部分或下半部分，并支持不同的饱和模式。   - asc_int322uint16：不饱和模式，写入目的操作数的上半部分。   - asc_int322uint16_sat：饱和模式，写入目的操作数的上半部分。   - asc_int322uint16_v2：不饱和模式，写入目的操作数的下半部分。   - asc_int322uint16_sat_v2：饱和模式，写入目的操作数的下半部分。 由于源操作数与目的操作数类型位宽比为 2:1，写入数据时需要将一个 VL 大小的数据分为两部分，根据不同接口选取写入目的操作数的上半部分或下半部分。关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式](./rounding_mode.md)。以 asc_int322uint16 接口为例：

## 函数原型
```cpp
// 不饱和模式，数据写入目的操作数的上半部分
```

```cpp
__simd_callee__ inline void asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
// 饱和模式，数据写入目的操作数的上半部分
```

```cpp
__simd_callee__ inline void asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
// 不饱和模式，数据写入目的操作数的下半部分
```

```cpp
__simd_callee__ inline void asc_int322uint16_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
// 饱和模式，数据写入目的操作数的下半部分
```

```cpp
__simd_callee__ inline void asc_int322uint16_sat_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
```

## 参数说明
**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg数据类型定义.md)。

## 数据类型支持
| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| vector_int32_t | 不涉及 | vector_uint16_t |

## 返回值说明
无返回值。

## 约束说明
- 饱和模式约束：int32_t 转 uint16_t 为窄化转换，支持饱和模式和不饱和模式，需配置 ctrl 寄存器，ctrl 寄存器的详细说明请参见[asc_set_ctrl.md](../../sys_var/asc_set_ctrl.md)。
- 不饱和模式：输入数据截断为目标数据宽度，例如输入 int32_t 值为 65536，输出 uint16_t 值为 0。
- 饱和模式：输入数据超出 uint16_t 表示范围时，饱和为 uint16_t 最值；输入负数时饱和为 0。
- 异常和边界值处理：mask 未筛选的元素在输出中置零。

## 理论性能

本接口的理论性能数据请参考附录：
- Vector指令理论性能汇总：详见 [Vector指令理论性能汇总](../../../../附录/Vector指令理论性能汇总.md)

## 调用示例
```cpp
vector_uint16_t dst;
vector_int32_t src;
vector_bool mask;
asc_int322uint16(dst, src, mask);        // 不饱和模式，将src转换成vector_uint16_t类型并写入dst的上半部分
asc_int322uint16_sat(dst, src, mask);    // 饱和模式，将src转换成vector_uint16_t类型并写入dst的上半部分
asc_int322uint16_v2(dst, src, mask);     // 不饱和模式，将src转换成vector_uint16_t类型并写入dst的下半部分
asc_int322uint16_sat_v2(dst, src, mask); // 饱和模式，将src转换成vector_uint16_t类型并写入dst的下半部分
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_vconv_part_instr.cpp`

