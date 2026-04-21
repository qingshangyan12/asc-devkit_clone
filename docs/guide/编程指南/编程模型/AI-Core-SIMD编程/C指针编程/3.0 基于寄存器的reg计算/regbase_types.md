# Regbase API 寄存器类型详解

## 1. 矢量数据寄存器

### 1.1 概述

矢量数据寄存器用于存储矢量数据，位宽为VL（Vector Length）= 256B。可存储 VL/sizeof(T) 个元素。

### 1.2 类型分类

**按位宽分类**：

| 位宽 | 数据类型 |
|-----|---------|
| b8 | `vector_int8_t`, `vector_uint8_t`, `vector_fp8_e4m3fn_t`, `vector_fp8_e5m2_t`, `vector_fp8_e8m0_t`, `vector_hifloat8_t`, `vector_int4x2_t`, `vector_fp4x2_e1m2_t`, `vector_fp4x2_e2m1_t` |
| b16 | `vector_int16_t`, `vector_uint16_t`, `vector_half`, `vector_bfloat16_t` |
| b32 | `vector_int32_t`, `vector_uint32_t`, `vector_float` |
| b64 | `vector_int64_t`, `vector_uint64_t` |

### 1.3 元素数量

| 数据类型 | sizeof(T) | 元素数量 |
|---------|-----------|---------|
| vector_int8_t | 1B | 256 |
| vector_half | 2B | 128 |
| vector_int16_t | 2B | 128 |
| vector_float | 4B | 64 |
| vector_int32_t | 4B | 64 |
| vector_int64_t | 8B | 32 |

### 1.4 特殊类型说明

**打包类型**（两元素打包为1字节）：
- `vector_int4x2_t`
- `vector_fp4x2_e1m2_t`
- `vector_fp4x2_e2m1_t`

**FP8类型**：
- `vector_fp8_e4m3fn_t`：E4M3格式
- `vector_fp8_e5m2_t`：E5M2格式
- `vector_fp8_e8m0_t`：E8M0格式
- `vector_hifloat8_t`：HiFloat8格式

### 1.5 定义示例

```cpp
vector_half dst;        // half类型矢量寄存器，128个元素
vector_float src;       // float类型矢量寄存器，64个元素
vector_int8_t data;     // int8类型矢量寄存器，256个元素
vector_bfloat16_t bf16; // bfloat16类型矢量寄存器，128个元素
```

### 1.6 使用示例

```cpp
vector_half dst;
half index = 0;
asc_arange(dst, index);  // 生成递增序列 [0, 1, 2, ...]
```

---

## 2. 掩码寄存器

### 2.1 概述

掩码寄存器类型为`vector_bool`，用于控制矢量计算中哪些元素参与计算。

**位宽**：VL / 8 = 256B / 8 = 32字节

**位排列**：每个bit对应一个元素，1表示参与计算，0表示不参与。

### 2.2 示意图

```
┌─────────────────────────────────────────┐
│ bit0 bit1 bit2 ... bit127 (half类型)    │
│  ↑    ↑    ↑         ↑                   │
│ 元素0 元素1 元素2 ... 元素127           │
└─────────────────────────────────────────┘
```

### 2.3 定义与创建

```cpp
vector_bool mask;

// 创建全True掩码
mask = asc_create_mask_b16(PAT_ALL);

// 根据元素数量创建掩码
uint32_t length = 64;
mask = asc_update_mask_b16(length);
```

### 2.4 使用示例

```cpp
vector_half src0, src1, dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);

asc_add(dst, src0, src1, mask);  // 所有元素参与加法
```

---

## 3. 地址寄存器

### 3.1 概述

地址寄存器类型为`iter_reg`，用于存储地址偏移量。在循环中，iter_reg会根据设置的步长自动自增。

### 3.2 初始化

```cpp
iter_reg addr_reg;
addr_reg = asc_create_iter_reg_b8(one_repeat_size);   // 8位数据步长
addr_reg = asc_create_iter_reg_b16(one_repeat_size);  // 16位数据步长
addr_reg = asc_create_iter_reg_b32(one_repeat_size);  // 32位数据步长
```

### 3.3 使用流程

```cpp
iter_reg addr_reg;
addr_reg = asc_create_iter_reg_b16(128);  // 每次偏移128个half元素

asc_loadalign(src, src_addr, addr_reg);   // addr_reg自动自增
asc_loadalign(src, src_addr, addr_reg);   // 下一次加载偏移128
```

### 3.4 完整示例

```cpp
constexpr uint32_t one_repeat_size = 128;  // half类型一次处理128元素
uint32_t total_length = 512;
uint32_t repeat_time = total_length / one_repeat_size;

__simd_vf__ inline void loop_example(__ubuf__ half* dst_addr, __ubuf__ half* src_addr)
{
    vector_half src, dst;
    vector_bool mask;
    iter_reg addr_reg;
    
    for (uint16_t i = 0; i < repeat_time; ++i) {
        addr_reg = asc_create_iter_reg_b16(one_repeat_size);
        mask = asc_create_mask_b16(PAT_ALL);
        
        asc_loadalign(src, src_addr, addr_reg);
        asc_add(dst, src, src, mask);
        asc_storealign(dst_addr, dst, addr_reg, mask);
    }
}
```

---

## 4. 非对齐寄存器

### 4.1 概述

非对齐寄存器用于处理起始地址未按32字节对齐的数据搬运：

- `vector_load_unalign`：非对齐加载缓冲区
- `vector_store_unalign`：非对齐存储缓冲区

### 4.2 使用流程

**加载流程**：
1. 初始化：`asc_loadunalign_pre`
2. 加载：`asc_loadunalign`

**存储流程**：
1. 存储：`asc_storeunalign`
2. 尾块处理：`asc_storeunalign_post`

### 4.3 完整示例

```cpp
constexpr uint32_t one_repeat_size = 256;  // int8一次处理256元素
uint32_t total_length = 255;
uint32_t repeat_time = (total_length + one_repeat_size - 1) / one_repeat_size;

__simd_vf__ inline void unalign_example(__ubuf__ int8_t* dst_addr, __ubuf__ int8_t* src_addr)
{
    vector_int8_t src, dst;
    vector_load_unalign ureg0;
    vector_store_unalign ureg1;
    vector_bool mask;
    
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b8(total_length);
        
        // 非对齐加载
        asc_loadunalign_pre(ureg0, src_addr + i * one_repeat_size);
        asc_loadunalign(src, ureg0, src_addr + i * one_repeat_size);
        
        // 计算
        asc_neg(dst, src, mask);
        
        // 非对齐存储
        asc_storeunalign(dst_addr + i * one_repeat_size, ureg1, dst);
        asc_storeunalign_post(dst_addr + i * one_repeat_size, ureg1, 0);
    }
}
```

---

## 5. 寄存器约束说明

### 5.1 矢量数据寄存器

- 寄存器数量有限，避免过多同时使用的寄存器
- 在循环中复用寄存器，减少寄存器占用

### 5.2 掩码寄存器

- 每次计算需要提供掩码参数
- 掩码创建时需指定数据位宽（b8/b16/b32）

### 5.3 地址寄存器

- 每层循环使用一个iter_reg
- iter_reg初始化时需指定步长

### 5.4 非对齐寄存器

- 加载前必须先调用`asc_loadunalign_pre`初始化
- 存储后必须调用`asc_storeunalign_post`处理尾块

---

## 6. 类型与接口匹配

| 数据类型 | 创建掩码接口 | 创建地址寄存器接口 |
|---------|-------------|-------------------|
| int8_t/uint8_t | `asc_create_mask_b8` | `asc_create_iter_reg_b8` |
| half/int16_t | `asc_create_mask_b16` | `asc_create_iter_reg_b16` |
| float/int32_t | `asc_create_mask_b32` | `asc_create_iter_reg_b32` |

---

**文档版本**: 1.0  
**适用产品**: Ascend 950PR/Ascend 950DT