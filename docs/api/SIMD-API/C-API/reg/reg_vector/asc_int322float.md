# asc_int322float

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

头文件路径：`"c_api/reg_compute/reg_convert.h"`。

$$
dst_i = (float)src_i
$$

关于舍入模式的详细说明，请参见 [舍入模式](./rounding_mode.md)。

## 函数原型

```cpp
// RINT舍入模式
__simd_callee__ inline void asc_int322float_rn(vector_float& dst, vector_int32_t src, vector_bool mask)
// ROUND舍入模式
__simd_callee__ inline void asc_int322float_rna(vector_float& dst, vector_int32_t src, vector_bool mask)
// FLOOR舍入模式
__simd_callee__ inline void asc_int322float_rd(vector_float& dst, vector_int32_t src, vector_bool mask)
// CEIL舍入模式
__simd_callee__ inline void asc_int322float_ru(vector_float& dst, vector_int32_t src, vector_bool mask)
// TRUNC舍入模式
__simd_callee__ inline void asc_int322float_rz(vector_float& dst, vector_int32_t src, vector_bool mask)
```

## 参数说明

| 参数名    | 输入/输出 | 描述                |
| :------ | :----- | :----------------- |
| dst    | 输出    | 目的操作数（矢量数据寄存器）。            |
| src    | 输入    | 源操作数（矢量数据寄存器）。             |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。

## 数据类型支持


| 输入类型 | 输出类型 |
|---|---|
| int32_t | float |


## 返回值说明

无返回值。
## 流水类型

PIPE_V

## 约束说明

- 当int32_t的值超出float可精确表示的整数范围（绝对值大于2^24）时，按函数后缀指定的舍入模式进行舍入。
- mask未筛选的元素在输出中置零。

## 理论性能


| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| int32_t | float | 32 |

## 调用示例

```cpp
vector_int32_t src;
vector_float dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_int322float_rn(dst, src, mask);
```
