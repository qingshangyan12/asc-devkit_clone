# Regbase API 数据加载与存储

## 1. 数据加载概述

Regbase API需要显式执行数据加载操作，将数据从UB搬运到矢量数据寄存器。

### 1.1 加载接口分类

| API | 功能 | 对齐要求 |
|-----|------|---------|
| `asc_load` | 加载VL量数据 | 支持非32B对齐 |
| `asc_loadalign` | 对齐加载（多种模式） | 32B对齐 |
| `asc_loadunalign` | 非对齐加载 | 非对齐地址 |
| `asc_gather` | 按元素索引收集 | - |
| `asc_gather_datablock` | 按DataBlock收集 | - |

---

## 2. 对齐数据加载

### 2.1 asc_load

```cpp
__simd_callee__ inline void asc_load(vector_half& dst, __ubuf__ half* src);
```

**功能**：从UB搬运VL量数据到矢量数据寄存器。

**特点**：
- 源地址可不按32字节对齐
- 搬运量为VL（256字节）
- 连续搬运时需手动更新src地址

**支持的类型**：
- `vector_int8_t`, `vector_uint8_t`
- `vector_int16_t`, `vector_uint16_t`
- `vector_half`, `vector_bfloat16_t`
- `vector_int32_t`, `vector_uint32_t`, `vector_float`
- `vector_int64_t`, `vector_uint64_t`
- FP8类型、FP4类型等

**示例**：
```cpp
vector_half dst;
__ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
asc_load(dst, src);
```

### 2.2 asc_loadalign

对齐加载支持多种搬入模式。

#### NORM模式（正常搬入）

```cpp
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src);
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, iter_reg offset);
```

**功能**：从UB连续对齐搬入数据。

#### BRC模式（广播搬入）

```cpp
__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src);
__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, iter_reg offset);
```

**功能**：广播搬入，将第一个元素广播到整个寄存器。

#### UP2X模式（上采样搬入）

```cpp
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src);
```

**功能**：上采样搬入，每个元素扩展为两个元素。

#### DOWN2X模式（下采样搬入）

```cpp
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src);
```

**功能**：下采样搬入，相邻两个元素合并为一个元素。

#### UNPACK模式（解包搬入）

```cpp
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src);
```

**功能**：解包搬入，将压缩数据解包。

#### DEINTLV模式（解交织搬入）

```cpp
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1,
                                                   __ubuf__ half* src);
```

**功能**：解交织搬入，将交织数据分离到两个寄存器。

**示意图**：
```
输入(交织): [a0 b0 a1 b1 a2 b2 ...]
输出dst0:   [a0 a1 a2 ...]
输出dst1:   [b0 b1 b2 ...]
```

---

## 3. 非对齐数据加载

### 3.1 使用场景

源地址未按32字节对齐时使用。

### 3.2 加载流程

1. 初始化：`asc_loadunalign_pre`
2. 加载：`asc_loadunalign`

### 3.3 接口详解

#### asc_loadunalign_pre

```cpp
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& ureg, __ubuf__ int8_t* src);
```

**功能**：非对齐加载前的初始化。

#### asc_loadunalign

```cpp
__simd_callee__ inline void asc_loadunalign(vector_int8_t& dst, vector_load_unalign& ureg,
                                             __ubuf__ int8_t* src);
```

**功能**：非对齐加载源数据到寄存器。

### 3.4 示例

```cpp
vector_int8_t src;
vector_load_unalign ureg0;

asc_loadunalign_pre(ureg0, src_addr);
asc_loadunalign(src, ureg0, src_addr);
```

---

## 4. 按索引收集

### 4.1 asc_gather

```cpp
__simd_callee__ inline void asc_gather(vector_half& dst, __ubuf__ half* src,
                                        vector_uint32_t index, vector_bool mask);
```

**功能**：根据索引位置将源操作数按元素收集到目的操作数。

**参数**：
| 参数 | 说明 |
|-----|------|
| dst | 目的寄存器 |
| src | 源地址 |
| index | 索引数组（每个元素指定收集位置） |
| mask | 掩码寄存器 |

### 4.2 asc_gather_datablock

```cpp
__simd_callee__ inline void asc_gather_datablock(vector_half& dst, __ubuf__ half* src,
                                                  vector_uint32_t index, vector_bool mask);
```

**功能**：按DataBlock粒度收集数据。

**示例**：
```cpp
vector_half dst;
vector_uint32_t index;
vector_bool mask = asc_create_mask_b16(PAT_ALL);

asc_gather(dst, src_addr, index, mask);
```

---

## 5. 数据存储概述

### 5.1 存储接口分类

| API | 功能 | 对齐要求 |
|-----|------|---------|
| `asc_store` | 存储到UB | 不区分对齐 |
| `asc_storealign` | 对齐存储（多种模式） | 32B对齐 |
| `asc_storeunalign` | 非对齐存储 | 非对齐地址 |

---

## 6. 对齐数据存储

### 6.1 asc_store

```cpp
__simd_callee__ inline void asc_store(__ubuf__ half* dst, vector_half src);
```

**功能**：从寄存器存储到UB，不区分是否对齐。

**注意**：追求极致性能时，应尽量避免使用该接口，优先使用`asc_storealign`。

### 6.2 asc_storealign

对齐存储支持多种搬出模式。

#### NORM模式

```cpp
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src);
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, iter_reg offset);
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, iter_reg offset, vector_bool mask);
```

**功能**：对齐存储，从寄存器存储到UB。

#### PACK模式

```cpp
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int8_t* dst, vector_int32_t src);
```

**功能**：打包存储，将数据打包后存储。

#### INTLV模式（交织存储）

```cpp
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ half* dst0, __ubuf__ half* dst1,
                                                  vector_half src0, vector_half src1);
```

**功能**：交织存储，将两个寄存器的数据交织存储。

**示意图**：
```
输入src0: [a0 a1 a2 ...]
输入src1: [b0 b1 b2 ...]
输出:     [a0 b0 a1 b1 a2 b2 ...]
```

---

## 7. 非对齐数据存储

### 7.1 使用场景

目的地址未按32字节对齐时使用。

### 7.2 存储流程

1. 存储：`asc_storeunalign`
2. 尾块处理：`asc_storeunalign_post`

### 7.3 接口详解

#### asc_storeunalign

```cpp
__simd_callee__ inline void asc_storeunalign(__ubuf__ int8_t* dst, vector_store_unalign& ureg,
                                             vector_int8_t src, uint32_t count);
```

**功能**：非对齐存储数据到UB。

#### asc_storeunalign_post

```cpp
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign& ureg,
                                                  uint32_t tail_count);
```

**功能**：处理非对齐存储的尾块。

### 7.4 示例

```cpp
vector_int8_t dst;
vector_store_unalign ureg1;

asc_storeunalign(dst_addr, ureg1, dst, one_repeat_size);
asc_storeunalign_post(dst_addr, ureg1, 0);
```

---

## 8. 加载存储完整示例

### 8.1 对齐加载存储循环

```cpp
constexpr uint32_t one_repeat_size = 128;  // VL / sizeof(half)
uint32_t total_length = 512;
uint32_t repeat_time = total_length / one_repeat_size;

__simd_vf__ inline void aligned_loop(__ubuf__ half* dst_addr, __ubuf__ half* src_addr)
{
    vector_half src0, src1, dst;
    vector_bool mask;
    iter_reg addr_reg;
    
    for (uint16_t i = 0; i < repeat_time; ++i) {
        addr_reg = asc_create_iter_reg_b16(one_repeat_size);
        mask = asc_create_mask_b16(PAT_ALL);
        
        asc_loadalign(src0, src_addr, addr_reg);
        asc_loadalign(src1, src_addr + 64, addr_reg);
        asc_add(dst, src0, src1, mask);
        asc_storealign(dst_addr, dst, addr_reg, mask);
    }
}
```

### 8.2 非对齐加载存储循环

```cpp
constexpr uint32_t one_repeat_size = 256;  // VL / sizeof(int8_t)
uint32_t total_length = 255;
uint32_t repeat_time = (total_length + one_repeat_size - 1) / one_repeat_size;

__simd_vf__ inline void unaligned_loop(__ubuf__ int8_t* dst_addr, __ubuf__ int8_t* src_addr,
                                        uint32_t count)
{
    vector_int8_t src, dst;
    vector_load_unalign ureg0;
    vector_store_unalign ureg1;
    vector_bool mask;
    
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b8(count);
        
        asc_loadunalign_pre(ureg0, src_addr + i * one_repeat_size);
        asc_loadunalign(src, ureg0, src_addr + i * one_repeat_size);
        
        asc_neg(dst, src, mask);
        
        asc_storeunalign(dst_addr + i * one_repeat_size, ureg1, dst, one_repeat_size);
        asc_storeunalign_post(dst_addr + i * one_repeat_size, ureg1, 0);
    }
}
```

### 8.3 解交织加载示例

```cpp
__simd_vf__ inline void deinterleave_example(__ubuf__ half* dst0_addr, __ubuf__ half* dst1_addr,
                                              __ubuf__ half* src_addr)
{
    vector_half dst0, dst1;
    
    asc_loadalign_deintlv(dst0, dst1, src_addr);
    
    asc_storealign(dst0_addr, dst0);
    asc_storealign(dst1_addr, dst1);
}
```

---

## 9. 模式选择指南

| 场景 | 推荐模式 |
|-----|---------|
| 连续数据 | NORM模式 |
| 广播数据 | BRC模式 |
| 上采样 | UP2X模式 |
| 下采样 | DOWN2X模式 |
| 解包 | UNPACK模式 |
| 交织数据分离 | DEINTLV模式 |
| 非对齐地址 | 非对齐接口 |

---

**文档版本**: 1.0  
**适用产品**: Ascend 950PR/Ascend 950DT