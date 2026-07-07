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
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件为：`#include "c_api/reg_compute/reg_convert.h"`

将int32_t类型转化为int16_t类型，支持饱和/非饱和模式。

由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个VL大小的数据分为两部分，根据不同接口选取索引0或者索引1。关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式](./rounding_mode.md)。

## 函数原型
```cpp
__simd_callee__ inline void asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_int322int16_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_int322int16_sat_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
```

## 参数说明

**表1**  参数说明


| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |


矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg数据类型定义.md)。

## 数据类型支持


| 输入A类型 | 输入B类型/标量类型 | 输出类型 |
|---|---|---|
| vector_int32_t | 不涉及 | vector_int16_t |


## 返回值说明

无返回值。

## 约束说明

- 饱和模式约束：int32_t转int16_t为窄化转换，支持饱和模式和不饱和模式，需配置ctrl寄存器，ctrl寄存器的详细说明请参见[asc_set_ctrl.md](../../sys_var/asc_set_ctrl.md)。
- 不饱和模式：输入数据截断为目标数据宽度，例如输入int32_t值为32768，输出int16_t值为 -32768。
- 饱和模式：输入数据超出int16_t表示范围时，饱和为int16_t最值，例如输入int32_t值为32768，输出int16_t值为32767；输入负数超出范围时饱和为 -32768。
- 异常和边界值处理：mask未筛选的元素在输出中置零。

## 理论性能


| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int32_t | vector_int16_t | 64 |



- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_int322int16.cpp`


## 调用示例


```cpp
vector_int32_t src;
vector_int16_t dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_int322int16(dst, src, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_int322int16.cpp`

