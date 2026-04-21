# Regbase API 高级特性

## 1. 非对齐数据处理

### 1.1 概述

当源地址或目的地址未按32字节对齐时，需要使用非对齐接口。

### 1.2 非对齐寄存器

- `vector_load_unalign`：非对齐加载缓冲区
- `vector_store_unalign`：非对齐存储缓冲区

### 1.3 加载流程

```cpp
vector_load_unalign ureg0;
vector_int8_t src;

// 1. 初始化
asc_loadunalign_pre(ureg0, src_addr);

// 2. 加载
asc_loadunalign(src, ureg0, src_addr);
```

### 1.4 存储流程

```cpp
vector_store_unalign ureg1;
vector_int8_t dst;

// 1. 存储
asc_storeunalign(dst_addr, ureg1, dst, count);

// 2. 尾块处理
asc_storeunalign_post(dst_addr, ureg1, tail_count);
```

### 1.5 完整示例

```cpp
constexpr uint32_t one_repeat_size = 256;
uint32_t total_length = 255;

__simd_vf__ inline void unalign_example(__ubuf__ int8_t* dst_addr, __ubuf__ int8_t* src_addr)
{
    vector_int8_t src, dst;
    vector_load_unalign ureg0;
    vector_store_unalign ureg1;
    vector_bool mask;
    
    mask = asc_update_mask_b8(total_length);
    
    asc_loadunalign_pre(ureg0, src_addr);
    asc_loadunalign(src, ureg0, src_addr);
    
    asc_neg(dst, src, mask);
    
    asc_storeunalign(dst_addr, ureg1, dst, one_repeat_size);
    asc_storeunalign_post(dst_addr, ureg1, 0);
}
```

---

## 2. 交织/解交织操作

### 2.1 概述

交织操作将两个数据序列交替合并，解交织操作将交织数据分离。

### 2.2 解交织加载

```cpp
asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src);
```

**功能**：从UB加载交织数据，分离到两个寄存器。

**示意图**：
```
输入src: [a0 b0 a1 b1 a2 b2 a3 b3 ...]
输出dst0: [a0 a1 a2 a3 ...]
输出dst1: [b0 b1 b2 b3 ...]
```

### 2.3 交织存储

```cpp
asc_storealign_intlv(__ubuf__ half* dst0, __ubuf__ half* dst1,
                     vector_half src0, vector_half src1);
```

**功能**：将两个寄存器的数据交织存储。

**示意图**：
```
输入src0: [a0 a1 a2 a3 ...]
输入src1: [b0 b1 b2 b3 ...]
输出: [a0 b0 a1 b1 a2 b2 a3 b3 ...]（交替写入dst0和dst1）
```

### 2.4 使用示例

```cpp
vector_half a, b;
vector_half merged_a, merged_b;

// 解交织加载
asc_loadalign_deintlv(a, b, interleaved_src);

// 处理数据
asc_add(merged_a, a, a, mask);
asc_add(merged_b, b, b, mask);

// 交织存储
asc_storealign_intlv(dst0_addr, dst1_addr, merged_a, merged_b);
```

---

## 3. 打包/解包操作

### 3.1 解包加载

```cpp
asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src);
```

**功能**：解包加载，将压缩数据解包后加载。

### 3.2 打包存储

```cpp
asc_storealign_pack(__ubuf__ int8_t* dst, vector_int32_t src);
```

**功能**：打包存储，将32位数据打包为8位后存储。

### 3.3 asc_pack

```cpp
asc_pack(vector_int16_t& dst, vector_int32_t src, PACK_MODE mode, vector_bool mask);
```

**功能**：将源操作数元素选取低8位/16位/32位写入目的操作数。

---

## 4. 上采样/下采样操作

### 4.1 上采样加载

```cpp
asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src);
```

**功能**：上采样加载，每个元素扩展为两个元素。

**示意图**：
```
输入src: [a0 a1 a2 ...]（64个元素）
输出dst: [a0 a0 a1 a1 a2 a2 ...]（128个元素）
```

### 4.2 下采样加载

```cpp
asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src);
```

**功能**：下采样加载，相邻两个元素合并为一个。

**示意图**：
```
输入src: [a0 a1 a2 a3 ...]（128个元素）
输出dst: [a0 a2 a4 ...]（64个元素，取偶数位置）
```

### 4.3 使用示例

```cpp
vector_half dst;

// 上采样加载
asc_loadalign_upsample(dst, src_addr);  // 数据扩展

// 下采样加载
asc_loadalign_downsample(dst, src_addr);  // 数据压缩
```

---

## 5. 广播操作

### 5.1 广播加载

```cpp
asc_loadalign_brc(vector_half& dst, __ubuf__ half* src);
```

**功能**：广播加载，将第一个元素广播到整个寄存器。

**示意图**：
```
输入src: [a0 ...]
输出dst: [a0 a0 a0 a0 ... a0]（128个元素全为a0）
```

---

## 6. 累计/频率统计

### 6.1 累计直方图

```cpp
asc_cumulative_histogram(vector_int32_t& dst, vector_int32_t src, vector_bool mask);
```

**功能**：对直方图数据进行累计统计。

### 6.2 频率直方图

```cpp
asc_frequency_histogram(vector_int32_t& dst, vector_int32_t src, vector_bool mask);
```

**功能**：对直方图数据进行频率统计。

---

## 7. 数据压缩/解压缩

### 7.1 压缩（squeeze）

```cpp
asc_squeeze(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：将src中被mask选择的有效元素依次复制到dst，有效元素在dst上连续排列。

**示意图**：
```
mask: [1 0 1 0 1 1 0 1 ...]
src:  [a0 a1 a2 a3 a4 a5 a6 a7 ...]
dst:  [a0 a2 a4 a5 a7 ...]（有效元素连续排列）
```

### 7.2 解压缩（unsqueeze）

```cpp
asc_unsqueeze(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：根据mask进行解压缩，将生成的数据输出到dst。

---

## 8. 特殊转换

### 8.1 FP4转换

```cpp
asc_bfloat162e1m2x2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);
asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src);
```

### 8.2 FP8转换

```cpp
asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src);
asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src);
```

### 8.3 HiFloat8转换

```cpp
asc_float2hif8(vector_hifloat8_t& dst, vector_float src, vector_bool mask);
asc_hif82half(vector_half& dst, vector_hifloat8_t src);
```

---

## 9. 截断操作

### 9.1 asc_truncate

```cpp
asc_truncate(vector_float& dst, vector_float src, ROUND_MODE mode, vector_bool mask);
```

**功能**：将源操作数的浮点数元素截断到整数位，数据类型保持不变。

---

## 10. 同步操作

### 10.1 流水线同步

```cpp
asc_mem_bar();
```

**功能**：Reg计算宏函数内不同流水线之间的同步指令。

---

## 11. 地址寄存器高级用法

### 11.1 创建地址寄存器

```cpp
iter_reg addr_reg;
addr_reg = asc_create_iter_reg_b8(stride);   // 8位数据步长
addr_reg = asc_create_iter_reg_b16(stride);  // 16位数据步长
addr_reg = asc_create_iter_reg_b32(stride);  // 32位数据步长
```

### 11.2 循环中的使用

```cpp
for (uint16_t i = 0; i < repeat_time; ++i) {
    addr_reg = asc_create_iter_reg_b16(128);  // 每次偏移128个half
    
    asc_loadalign(src, src_addr, addr_reg);   // addr_reg自动自增
    asc_storealign(dst_addr, dst, addr_reg);
}
```

---

**文档版本**: 1.0  
**适用产品**: Ascend 950PR/Ascend 950DT