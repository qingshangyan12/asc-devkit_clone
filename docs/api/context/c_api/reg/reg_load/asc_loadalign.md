# asc_loadalign

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  </cann-filter>| √ |

## 功能说明

reg计算数据搬运接口，适用于从UB中32B对齐的起始地址连续搬入矢量数据寄存器的场景。

该接口不改变源操作数在UB上的地址，每次循环的调用都需要用户手动更新源操作数在UB上的地址。

## 函数原型

```cpp
// =========asc_loadalign(fp4x2_e2m1/fp4x2_e1m2/u8/s8/f8_e4m3/fp8_e5m2/fp8_e8m0/u16/s16/bf16/half/u32/s32/float/u64/s64)=========
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src_align_32b, iter_reg offset)

// =========BRC_B8(fp4x2_e2m1/fp4x2_e1m2/u8/s8/f8_e4m3/fp8_e5m2/fp8_e8m0)=========
__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)

// =========BRC_B16(u16/s16/bf16/half)=========
__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)

// =========BRC_B32(u32/s32/float)=========
__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src_align_32b, iter_reg offset)

// =========US(B8/B16)=========
__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)

// =========DS(B8/B16)=========
__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)

// DINTLV(B8/B16/B32)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst, __ubuf__ uint32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst, __ubuf__ int32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst, __ubuf__ float* src_align_32b, iter_reg offset)

// unpack(B8/B16/B32)
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src_align_32b, iter_reg offset)

// unpack v2
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)

// brc_v2
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src_align_32b, iter_reg offset)

/// brc_v3(B16/B32)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src_align_32b, iter_reg offset)
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src_align_32b, iter_reg offset)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src_align_32b | 输入 | 源操作数，UB中32B对齐的起始地址。 |
| offset | 输入 | 地址寄存器，每次迭代的偏移量，单位为字节。 |

矢量数据寄存器和非对齐寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 该接口中的src_align_32b需要32B对齐。

## 调用示例

```cpp
__ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
iter_reg offset = 0x10;
asc_loadalign(dst, src, offset);
```