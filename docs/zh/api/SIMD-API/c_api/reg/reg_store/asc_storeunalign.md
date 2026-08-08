# asc_storeunalign

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

reg计算数据搬运接口，适用于从矢量数据寄存器连续非32B对齐的起始地址连续搬出到UB的场景。

该接口使用uint32_t作为存储偏移量，不改变源操作数在UB上的地址，每次循环的调用都需要用户手动更新目的操作数在UB上的地址。

## 函数原型

```cpp
__simd_callee__ inline void asc_storeunalign(__ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count)
```

## 参数说明

**表 1**  参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入/输出 | 非对齐寄存器，用于保存非对齐数据，长度32B。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。 |
| count | 输入 | 存储搬运的个数。 |

矢量数据寄存器和非对齐寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器与`vector_store_unalign`临时寄存器，目的操作数为UB地址，UB地址空间外的指针不可作为dst传入。
- dst起始地址可非32字节对齐；本接口按非对齐方式将拼接结果写入UB，不要求对齐，不会报错。
- 调用该接口之后，需要调用[asc_storeunalign_post](asc_storeunalign_post.md)处理非对齐搬出的尾块。
- src0按引用传入，本接口调用后其内容被更新为本次搬出尾部数据，供后续拼接继续使用。
- UB容量上限：UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。dst偏移后不可超过实际可用容量，否则会报错。
- count不可大于矢量数据寄存器容纳的最大元素个数VL/sizeof(dtype)；超出寄存器范围的元素无有效数据，写入dst的内容不确定，建议取[0, VL/sizeof(dtype)]范围内有效值。
- count×sizeof(dtype)字节为实际写入数据量，与dst起始地址相加后不可越过UB可用容量上限，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

```cpp
// dst地址为8，非32B对齐，占用的地址为UB[8:520]。
__ubuf__ uint32_t* dst = (__ubuf__ uint32_t*)asc_get_phy_buf_addr(8);
vector_store_unalign ureg;
vector_uint32_t src;
uint32_t count = 64;
uint32_t repeat = 2;
for (uint32_t i = 0; i < repeat; i++) {
    // 其他reg操作

    // 第一次：src[0:62] 写入dst[0:62]（即UB[8:256]）；同时src[62:64] 写入ureg[0:2]
    // 第二次：ureg[0:2] + src[0:62] 写入dst[62:126]（即UB[256:512]）；同时src[62:64] 写入ureg[0:2]
    asc_storeunalign(dst + i*count, ureg, src, count);
}
// dst结束地址为520，即dst + repeat * count * sizeof(uint32_t)，所以offset要配置为repeat * count
int32_t offset = repeat * count;
// ureg[0:2] 写入dst[126:128]（即UB[512:520]）
asc_storeunalign_post(dst, ureg, offset);
```
