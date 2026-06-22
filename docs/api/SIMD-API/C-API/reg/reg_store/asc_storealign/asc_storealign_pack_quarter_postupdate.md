# asc_storealign_pack_quarter_postupdate

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

四分之一压缩模式的Post Update模式，与asc_storealign_pack_quarter的区别在于该接口支持硬件操作的目的操作数地址自动更新。参见[asc_storealign_pack_quarter](asc_storealign_pack_quarter.md)。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__  int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__  float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| offset | 输入 | 地址偏移量。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义.md](../../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
__ubuf__ int32_t* dst_align32b = (__ubuf__ int32_t*)asc_get_phy_buf_addr(0);
vector_int32_t src;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
int32_t offset = 128;
asc_storealign_pack_quarter_postupdate(dst_align32b, src, offset, mask);
```
