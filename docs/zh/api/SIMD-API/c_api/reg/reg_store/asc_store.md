# asc_store

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

## 功能说明

reg计算数据搬运接口，适用于从矢量数据寄存器搬出到UB的场景，不区分是否对齐，在追求极致性能时，对齐场景推荐使用[asc_storealign](asc_storealign.md)接口。

## 函数原型

```cpp
// 不传入count参数，默认搬运VL（Vector Length）长度的元素
__simd_callee__ inline void asc_store(__ubuf__  int8_t* dst, vector_int8_t src)
__simd_callee__ inline void asc_store(__ubuf__  uint8_t* dst, vector_uint8_t src)
__simd_callee__ inline void asc_store(__ubuf__  fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src)
__simd_callee__ inline void asc_store(__ubuf__  fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src)
__simd_callee__ inline void asc_store(__ubuf__  int4b_t* dst, vector_int4x2_t src)
__simd_callee__ inline void asc_store(__ubuf__  fp8_e8m0_t* dst, vector_fp8_e8m0_t src)
__simd_callee__ inline void asc_store(__ubuf__  fp8_e5m2_t* dst, vector_fp8_e5m2_t src)
__simd_callee__ inline void asc_store(__ubuf__  fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src)
__simd_callee__ inline void asc_store(__ubuf__  hifloat8_t* dst, vector_hifloat8_t src)
__simd_callee__ inline void asc_store(__ubuf__  int16_t* dst, vector_int16_t src)
__simd_callee__ inline void asc_store(__ubuf__  uint16_t* dst, vector_uint16_t src)
__simd_callee__ inline void asc_store(__ubuf__  half* dst, vector_half src)
__simd_callee__ inline void asc_store(__ubuf__  bfloat16_t* dst, vector_bfloat16_t src)
__simd_callee__ inline void asc_store(__ubuf__  int32_t* dst, vector_int32_t src)
__simd_callee__ inline void asc_store(__ubuf__  uint32_t* dst, vector_uint32_t src)
__simd_callee__ inline void asc_store(__ubuf__  float* dst, vector_float src)
__simd_callee__ inline void asc_store(__ubuf__  int64_t* dst, vector_int64_t src)

// 传入count参数，搬运count个元素
__simd_callee__ inline void asc_store(__ubuf__  int8_t* dst, vector_int8_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  uint8_t* dst, vector_uint8_t src, uint32_t count)  
__simd_callee__ inline void asc_store(__ubuf__  int16_t* dst, vector_int16_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  uint16_t* dst, vector_uint16_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  int32_t* dst, vector_int32_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  uint32_t* dst, vector_uint32_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  float* dst, vector_float src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  int64_t* dst, vector_int64_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  bfloat16_t* dst, vector_bfloat16_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  fp8_e8m0_t* dst, vector_fp8_e8m0_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  fp8_e5m2_t* dst, vector_fp8_e5m2_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  hifloat8_t* dst, vector_hifloat8_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, uint32_t count)
__simd_callee__ inline void asc_store(__ubuf__  int4b_t* dst, vector_int4x2_t src, uint32_t count)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| count | 输入 | 搬运元素数量。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器，目的操作数为UB地址，UB地址空间外的指针不可作为dst传入。
- dst 起始地址可非 32 字节对齐；本接口按非对齐方式将矢量数据寄存器内容写入 UB，不要求对齐，不会报错。
- **UB容量上限**：UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。dst偏移 + 搬出字节数不可超过实际可用容量。
- 多次本接口调用之间若存在数据依赖（后一次写dst区域与前一次写同区域重叠），需通过同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)保证串行化，防止UB中数据被非预期覆盖。
- count的取值范围为[1, VL/sizeof(dst)]，VL为矢量长度。超出该范围会导致未定义行为。

## 调用示例

```cpp
vector_half src;
__ubuf__ half* dst = (__ubuf__ half*)asc_get_phy_buf_addr(0);
asc_store(dst, src);
```
