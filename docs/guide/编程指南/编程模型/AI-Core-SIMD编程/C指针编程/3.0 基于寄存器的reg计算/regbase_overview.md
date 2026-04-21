# Regbase API 编程概述

## 1. Regbase API 简介

Regbase API是AscendC C API的另一类编程风格，基于矢量数据寄存器进行显式操作。数据以寄存器类型传递，需要手动执行数据加载和存储操作，适合Ascend 950PR/Ascend 950DT产品。

### 1.1 核心特点

- **寄存器编程**：数据以矢量数据寄存器类型（如`vector_half`）表示
- **显式搬运**：需手动调用`asc_loadalign`加载数据，`asc_storealign`存储数据
- **局部掩码**：Mask作为参数传递，每次计算可使用不同掩码
- **精细控制**：支持非对齐搬运、交织/解交织等高级特性

### 1.2 适用产品

| 产品 | 支持状态 |
|-----|---------|
| Ascend 950PR | ✓ |
| Ascend 950DT | ✓ |

---

## 2. 与Membase API对比

| 特性 | Membase | Regbase |
|-----|---------|---------|
| 数据表示 | `__ubuf__ half*` 指针 | `vector_half` 寄存器 |
| 数据加载 | 计算时自动读取 | 显式 `asc_loadalign` |
| 数据存储 | 计算后自动写入 | 显式 `asc_storealign` |
| 掩码控制 | 全局Mask寄存器 | 参数传递局部掩码 |
| 计算粒度 | 支持高维切分迭代 | 单次VL长度计算 |
| 非对齐支持 | 有专门接口 | 完整非对齐流程 |
| 交织操作 | 不支持 | 支持DEINTLV/INTLV |
| 适用产品 | Atlas A3/A2 | Ascend 950 |

---

## 3. 关键字说明

| 关键字 | 描述 |
|-------|------|
| `__simd_callee__` | 函数调用约定，表示SIMD callee函数 |
| `__simd_vf__` | 函数调用约定，表示SIMD VF函数 |
| `__ubuf__` | Unified Buffer地址空间修饰符 |

---

## 4. 寄存器类型体系

### 4.1 矢量数据寄存器

矢量数据寄存器用于存储矢量数据，位宽为VL（Vector Length）= 256B。

| 位宽 | 数据类型 |
|-----|---------|
| b8 | `vector_int8_t`, `vector_uint8_t`, `vector_fp8_e4m3fn_t`, `vector_fp8_e5m2_t`, `vector_hifloat8_t`, `vector_int4x2_t`, `vector_fp4x2_e1m2_t`, `vector_fp4x2_e2m1_t` |
| b16 | `vector_int16_t`, `vector_uint16_t`, `vector_half`, `vector_bfloat16_t` |
| b32 | `vector_int32_t`, `vector_uint32_t`, `vector_float` |
| b64 | `vector_int64_t`, `vector_uint64_t` |

**元素数量计算**：
```
元素数量 = VL / sizeof(T) = 256B / sizeof(T)
```

| 类型 | 元素数量 |
|-----|---------|
| vector_int8_t | 256 |
| vector_half | 128 |
| vector_float | 64 |
| vector_int64_t | 32 |

### 4.2 掩码寄存器

掩码寄存器类型为`vector_bool`，用于控制矢量计算中参与计算的元素。

**位宽**：VL / 8 = 256B / 8 = 32字节

### 4.3 地址寄存器

地址寄存器类型为`iter_reg`，用于存储地址偏移量，在循环中自动自增。

### 4.4 非对齐寄存器

- `vector_load_unalign`：非对齐数据加载缓冲区
- `vector_store_unalign`：非对齐数据存储缓冲区

---

## 5. API分类

### 5.1 数据加载API

| API | 功能 |
|-----|------|
| `asc_load` | 从UB加载VL量数据到寄存器 |
| `asc_loadalign` | 对齐加载，支持多种模式 |
| `asc_loadunalign` | 非对齐加载 |
| `asc_gather` | 按索引收集元素 |
| `asc_gather_datablock` | 按DataBlock收集 |

### 5.2 数据存储API

| API | 功能 |
|-----|------|
| `asc_store` | 从寄存器存储到UB |
| `asc_storealign` | 对齐存储，支持多种模式 |
| `asc_storeunalign` | 非对齐存储 |

### 5.3 矢量计算API

| API类型 | 接口示例 |
|--------|---------|
| 算术运算 | `asc_add`, `asc_sub`, `asc_mul`, `asc_div` |
| 数学函数 | `asc_exp`, `asc_ln`, `asc_sqrt` |
| 激活函数 | `asc_relu`, `asc_leakyrelu` |
| 比较运算 | `asc_eq`, `asc_gt`, `asc_lt` |
| 归约运算 | `asc_reduce_sum`, `asc_reduce_max` |
| 类型转换 | `asc_half2float`, `asc_float2int32` |

### 5.4 掩码操作API

| API | 功能 |
|-----|------|
| `asc_create_mask` | 创建掩码寄存器 |
| `asc_update_mask` | 更新掩码寄存器 |
| `asc_get_mask_spr` | 从SPR读取Mask |

---

## 6. 基本编程流程

### 6.1 标准流程

```cpp
__simd_vf__ inline void compute_demo(__ubuf__ half* dst_addr, __ubuf__ half* src_addr)
{
    // 1. 定义寄存器
    vector_half src, dst;
    vector_bool mask;
    iter_reg addr_reg;
    
    // 2. 初始化地址寄存器
    addr_reg = asc_create_iter_reg_b16(128);  // 每次偏移128个元素
    
    // 3. 创建掩码
    mask = asc_create_mask_b16(PAT_ALL);
    
    // 4. 加载数据
    asc_loadalign(src, src_addr, addr_reg);
    
    // 5. 执行计算
    asc_add(dst, src, src, mask);
    
    // 6. 存储结果
    asc_storealign(dst_addr, dst, addr_reg, mask);
}
```

### 6.2 循环处理

```cpp
__simd_vf__ inline void loop_compute(__ubuf__ half* dst_addr, __ubuf__ half* src_addr,
                                      uint32_t total_length)
{
    constexpr uint32_t one_repeat_size = 128;  // VL / sizeof(half)
    uint32_t repeat_time = total_length / one_repeat_size;
    
    vector_half src, dst;
    vector_bool mask;
    iter_reg addr_reg;
    
    for (uint32_t i = 0; i < repeat_time; ++i) {
        addr_reg = asc_create_iter_reg_b16(one_repeat_size);
        mask = asc_create_mask_b16(PAT_ALL);
        
        asc_loadalign(src, src_addr, addr_reg);
        asc_add(dst, src, src, mask);
        asc_storealign(dst_addr, dst, addr_reg, mask);
    }
}
```

---

## 7. 最简示例

```cpp
#include "c_api/asc_simd.h"

__ubuf__ half src_buffer[128];
__ubuf__ half dst_buffer[128];

__simd_vf__ inline void simple_add()
{
    vector_half src, dst;
    vector_bool mask;
    
    mask = asc_create_mask_b16(PAT_ALL);
    
    asc_loadalign(src, src_buffer);
    asc_add(dst, src, src, mask);
    asc_storealign(dst_buffer, dst, mask);
}
```

---

## 8. 特殊特性

### 8.1 非对齐数据处理

```cpp
vector_load_unalign ureg0;
vector_store_unalign ureg1;

asc_loadunalign_pre(ureg0, src_addr);
asc_loadunalign(src, ureg0, src_addr);
asc_neg(dst, src, mask);
asc_storeunalign(dst_addr, ureg1, dst);
asc_storeunalign_post(dst_addr, ureg1, 0);
```

### 8.2 交织/解交织

```cpp
asc_loadalign_deintlv(dst0, dst1, src_addr);  // 解交织加载
asc_intlv(dst0_addr, dst1_addr, src0, src1);  // 交织存储
```

---

**文档版本**: 1.0  
**适用产品**: Ascend 950PR/Ascend 950DT