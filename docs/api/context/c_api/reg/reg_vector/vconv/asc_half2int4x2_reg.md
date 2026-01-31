# asc_bfloat162float

## 产品支持情况

|产品|是否支持|
| :------------ | :------------: |
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

将regtensor的half类型数据转为int4x2类型，无舍入模式。

## 函数原型

- 前n个数据计算
```cpp
__simd_callee__ inline void asc_half2int4x2_rd(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rd_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rd_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rd_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rn(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rn_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rn_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rn_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rna(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rna_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rna_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rna_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ro(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ro_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ro_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ro_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ru(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ru_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ru_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_ru_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rz(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rz_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rz_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_half2int4x2_rz_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|dst|输出|目的操作数。类型为[矢量数据寄存器](../../reg数据类型定义.md)。|
|src|输入|源操作数。类型为[矢量数据寄存器](../../reg数据类型定义.md)。|
|mask|输入|参与计算的元素掩码。|

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的需要32字节对齐。

## 调用示例

```cpp
constexpr uint32_t total_length = 128;
__ubuf__ bfloat16_t src[total_length];
vector_half src = asc_load(src, dstub, mask);
vector_bfloat16_t dst;
asc_half2bfloat16_rd(dst, src, mask);
```