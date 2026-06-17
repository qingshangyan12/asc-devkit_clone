# asc_storeunalign

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

reg计算数据搬运接口，适用于从矢量数据寄存器或MaskReg连续非32B对齐的起始地址连续搬出到UB的场景。

- 矢量数据寄存器搬出：使用`count`指定每次搬运的元素个数，不改变目的操作数在UB上的地址，每次循环需要用户手动更新`dst`。
- MaskReg搬出：通过`mask`（`vector_bool`）传入MaskReg，无`count`参数；`dst`为引用类型，每次调用后由硬件自动后更新目的地址。

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
__simd_callee__ inline void asc_storeunalign(__ubuf__ int4b_t*& dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count)
__simd_callee__ inline void asc_storeunalign(__ubuf__ uint16_t*& dst, vector_store_unalign& src0,
    vector_bool mask)
__simd_callee__ inline void asc_storeunalign(__ubuf__ uint32_t*& dst, vector_store_unalign& src0,
    vector_bool mask)
__simd_callee__ inline void asc_storeunalign(__ubuf__ uint8_t*& dst, vector_store_unalign& src0,
    vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入/输出 | 非对齐寄存器，用于保存非对齐数据，长度32B。 |
| src1 | 输入 | 源操作数（矢量数据寄存器），仅矢量数据寄存器搬出接口需要。 |
| mask | 输入 | 源操作数（掩码寄存器），仅MaskReg搬出接口需要。 |
| count | 输入 | 存储搬运的个数，仅矢量数据寄存器搬出接口需要。 |

矢量数据寄存器和非对齐寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 该接口中的dst不需要32B对齐。
- 矢量数据寄存器搬出（带`count`参数）后，需要调用[asc_storeunalign_post](./asc_storeunalign_post.md)处理非对齐搬出的尾块。
- MaskReg搬出（`mask`参数）后，同样需要调用[asc_storeunalign_post](./asc_storeunalign_post.md)处理尾块；尾块处理只需在循环外调用一次。post接口仅传入`ureg`与`dst`，不需要也不支持`vector_bool`类型参数。

## 调用示例

### 矢量数据寄存器连续非对齐搬出

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

### MaskReg连续非对齐搬出（pstu）

```cpp
// dst起始地址为8（非32B对齐）。UB上以uint16_t存储时，每次将32B MaskReg压缩为16B写出。
__ubuf__ uint16_t* dst = (__ubuf__ uint16_t*)asc_get_phy_buf_addr(8);
vector_store_unalign ureg;
vector_bool mask;
uint32_t remain_count = 128;
uint32_t repeat = 2;
for (uint32_t i = 0; i < repeat; i++) {
    // 准备本次待搬出的mask
    mask = asc_update_mask_b16(remain_count);
    // 每次调用写出一个MaskReg；跨32B边界的尾块缓存在ureg中
    asc_storeunalign(dst, ureg, mask);
}
// 循环结束后调用一次post，将ureg中剩余数据刷至UB；mask_dst已自增至结束位置，offset为0
asc_storeunalign_post(dst, ureg, 0);
```
