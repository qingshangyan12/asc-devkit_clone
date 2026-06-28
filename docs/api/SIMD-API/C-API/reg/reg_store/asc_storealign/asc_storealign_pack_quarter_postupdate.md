# asc_storealign_pack_quarter_postupdate

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

Reg计算数据搬运存储接口，四分之一粒度压缩搬出模式（pack quarter），使能postupdate自动更新UB目的地址。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst_align32b | 输入/输出 | 目的操作数地址，postupdate后自动偏移 |
| src | 输入 | 源操作数 |
| offset | 输入 | 偏移量 |
| mask | 输入 | 掩码控制 |

## 返回值说明

无

## 调用示例

```cpp
__ubuf__ int32_t* dst_addr;
vector_int32_t src;
int32_t offset = 128;
vector_bool mask;
asc_storealign_pack_quarter_postupdate(dst_addr, src, offset, mask);
```
