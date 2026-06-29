# asc_loadalign_brc_elem2datablock

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

对齐数据搬运接口，广播搬运模式（elem to datablock），加载(VL/DataBlock)个b16/b32类型数据，并将每个元素广播到一个DataBlock（32B）中。

## 函数原型

```cpp
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int16_t& dst, __ubuf__ int16_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src);
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int32_t& dst, __ubuf__ int32_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst | 输出 | 目的操作数 |
| src | 输入 | 源操作数地址 |

## 返回值说明

无

## 约束说明

- 源操作数地址需32B对齐
- 输入数据类型为b16/b32

## 调用示例

```cpp
vector_half dst;
__ubuf__ half* src_addr;
asc_loadalign_brc_elem2datablock(dst, src_addr);
```
