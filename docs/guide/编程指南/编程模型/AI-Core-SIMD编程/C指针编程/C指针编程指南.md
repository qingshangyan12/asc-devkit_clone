# 内存管理

本章介绍 AI Core 存储单元架构、内存申请方式、地址对齐要求等，帮助开发者合理规划片上内存资源，实现高性能算子开发。

---

## 1. 芯片物理位置

### 1.1 AI Core存储架构

AI Core包含多级内部存储（LocalMemory）、外部存储（GlobalMemory）和计算单元内部的寄存器，形成完整的存储层次结构。

**表 1**  AI Core存储单元与寄存器层级

| 存储层级 | 存储类型 | 名称 | 地址限定符 | 用途 | 访问单元 |
|---------|---------|------|-----------|------|---------|
| **外部存储** | GlobalMemory | Global Memory | `__gm__` | 存放算子输入/输出数据 | 所有单元通过总线访问 |
| **片上存储** | LocalMemory | L1 Buffer | `__cbuf__` | Cube数据中转区 | MTE1、MTE2、FixPipe |
| | | L0A Buffer | `__ca__` | Cube左矩阵输入 | Cube、MTE1 |
| | | L0B Buffer | `__cb__` | Cube右矩阵输入 | Cube、MTE1 |
| | | L0C Buffer | `__cc__` | Cube输出结果 | Cube、FixPipe |
| | | Unified Buffer | `__ubuf__` | Vector输入/输出 | Vector、MTE2、MTE3 |
| **计算单元内部** | 寄存器 | 矢量数据寄存器 | 无 | Vector计算的工作存储 | Vector计算单元 |
| | | 掩码寄存器 | 无 | 选择参与计算的元素 | Vector计算单元 |
| | | 地址寄存器 | 无 | 存储地址偏移量 | Scalar单元 |

### 1.2 存储层次结构

**图 1**  AI Core存储层次结构

![alt text](SIMD-Reg矢量计算内存层级-1.png)

### 1.3 存储层级特点

**表 2**  存储层级特点对比

| 存储层级 | 位置 | 容量 | 访问速度 | 管理方式 |
|---------|------|------|---------|---------|
| GlobalMemory | AI Core外部 | 大（GB级） | 慢（总线访问） | 通过搬运单元访问 |
| LocalMemory | AI Core内部 | 中（KB~MB级） | 快（片上访问） | 使用地址限定符申请 |
| 寄存器 | 计算单元内部 | 小（256B） | 最快（内部访问） | 定义寄存器变量 |

---

## 2. 内存单元定义修饰符号

### 2.1 地址限定符

C指针编程通过地址限定符描述不同层级内存，编译器根据限定符将数据分配到对应的物理存储单元。

**表 3**  地址限定符说明

| 地址限定符 | 存储单元 | 描述 | 示例 |
|-----------|---------|------|------|
| `__gm__` | Global Memory | 变量位于Global Memory地址空间 | `__gm__ float* data;` |
| `__ubuf__` | Unified Buffer | 变量位于Unified Buffer地址空间 | `__ubuf__ half buffer[256];` |
| `__cbuf__` | L1 Buffer | 变量位于L1 Buffer地址空间 | `__cbuf__ half matrix[4096];` |
| `__ca__` | L0A Buffer | 变量位于L0A Buffer地址空间 | `__ca__ half a_l0a[512];` |
| `__cb__` | L0B Buffer | 变量位于L0B Buffer地址空间 | `__cb__ half b_l0b[512];` |
| `__cc__` | L0C Buffer | 变量位于L0C Buffer地址空间 | `__cc__ float c_l0c[256];` |

---

## 3. 寄存器变量类型

### 3.1 矢量数据寄存器

矢量数据寄存器用于存储矢量数据，位宽为VL（Vector Length = 256B）。

**表 4**  矢量数据寄存器类型

| 位宽 | 数据类型 | 元素数量 | 说明 |
|------|---------|---------|------|
| b8 | `vector_int8_t` / `vector_uint8_t` | 256个 | 8位整型 |
| b16 | `vector_half` / `vector_bfloat16_t` | 128个 | 16位浮点 |
| b32 | `vector_float` / `vector_int32_t` | 64个 | 32位数据 |
| b64 | `vector_int64_t` / `vector_uint64_t` | 32个 | 64位整型 |

**使用示例：**
```cpp
vector_half dst;
half index = 0;
asc_arange(dst, index);
```

### 3.2 掩码寄存器

掩码寄存器用于矢量计算中选择参与计算的元素。

```cpp
vector_bool mask = asc_create_mask_b16(PAT_ALL);  // 创建全True掩码
vector_bool mask = asc_update_mask_b16(length);   // 根据元素数量生成掩码
```

### 3.3 地址寄存器

地址寄存器用于存储地址偏移量，支持循环自增。

```cpp
iter_reg addr_reg = asc_create_iter_reg_b8(offset);
```

---

## 4. LocalMemory与寄存器的关系

### 4.1 核心关系

**LocalMemory是数据存储空间，寄存器是计算单元的工作台。**

数据流动遵循以下流程：
1. **Load**: 从LocalMemory加载数据到寄存器
2. **Compute**: 寄存器数据参与计算，结果存储到寄存器
3. **Store**: 从寄存器写回数据到LocalMemory

**图 2**  LocalMemory与寄存器数据流

![alt text](image.png)

> **说明**：Vector计算数据必须先从UB加载到寄存器，计算完成后写回UB。

### 4.2 Vector计算数据流

**Vector计算单元只能访问寄存器，数据必须先从UB加载到寄存器。**

```cpp
// 1. 申请LocalMemory（UB）
constexpr uint32_t length = 128;
__ubuf__ half src_buffer[length];  // 数据存储在UB
__ubuf__ half dst_buffer[length];  // 结果存储在UB

// 2. Load: LocalMemory → 寄存器
vector_half src_reg;
asc_loadalign(src_reg, src_buffer);

// 3. Compute: 寄存器 → 寄存器
vector_half dst_reg;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_add(dst_reg, src_reg, src_reg, mask);

// 4. Store: 寄存器 → LocalMemory
asc_storealign(dst_buffer, dst_reg, mask);
```

### 4.3 Cube计算数据流

**Cube计算单元只能访问L0寄存器，数据必须先从L1加载到L0A/B。**

```cpp
// 1. 申请LocalMemory（L1、L0A/B/C）
__cbuf__ half a_l1[4096];  // L1中转区
__ca__ half a_l0a[512];    // L0A输入
__cb__ half b_l0b[512];    // L0B输入
__cc__ float c_l0c[256];   // L0C输出

// 2. Load: L1 → L0A/L0B
asc_copy_l12l0a(a_l0a, a_l1, 0, 1, 1, 0);
asc_copy_l12l0b(b_l0b, b_l1, 0, 1, 1, 0);

// 3. Compute: L0A/B → L0C
asc_mmad(c_l0c, a_l0a, b_l0b, 128, 64, 64, 0, false, false, true);

// 4. Store: L0C → GM或L1
asc_copy_l0c2gm(output, c_l0c, size);
```

### 4.4 数据搬运与计算过程

**图 3**  数据搬运与Vector计算过程

![alt text](image-1.png)

> **说明**：
> - **CopyIn**: 数据从Global Memory搬运到Local Memory（Unified Buffer）
> - **Load**: 从UB加载数据到寄存器
> - **Compute**: 寄存器数据参与计算
> - **Store**: 计算结果从寄存器写回UB
> - **CopyOut**: 计算结果从Local Memory搬回Global Memory

### 4.5 关键接口说明

**表 5**  Load/Store接口

| 接口类型 | 接口名称 | 功能 | 示例 |
|---------|---------|------|------|
| Load | `asc_loadalign` | 对齐加载（32B） | `asc_loadalign(reg, buffer);` |
| Load | `asc_loadunalign` | 非对齐加载 | `asc_loadunalign(reg, ureg, buffer);` |
| Compute | `asc_add` / `asc_mul` | 向量计算 | `asc_add(dst, src0, src1, mask);` |
| Store | `asc_storealign` | 对齐存储（32B） | `asc_storealign(buffer, reg, mask);` |
| Store | `asc_storeunalign` | 非对齐存储 | `asc_storeunalign(buffer, ureg, reg, count);` |

---

## 5. LocalMemory的内存申请方式

### 5.1 静态数组申请方式

使用数组方式申请片上内存，数组长度必须为编译期常量。

**语法：**
```cpp
constexpr uint32_t length = <编译期常量>;
__<地址限定符>__ <数据类型> buffer[length];
```

**示例：**
```cpp
// 申请UB内存
constexpr uint32_t ub_size = 256;
__ubuf__ half ub_buffer[ub_size];

// 申请L1内存
constexpr uint32_t l1_size = 4096;
__cbuf__ half l1_buffer[l1_size];

// 申请L0A/B/C内存
constexpr uint32_t l0_size = 512;
__ca__ half a_l0a[l0_size];
__cb__ half b_l0b[l0_size];
__cc__ float c_l0c[l0_size];
```

**优势：**
- 编译器自动保证地址对齐（默认32字节对齐）
- 语法简洁，易于理解
- 无需手动管理地址偏移
- 编译器自动分配，避免地址重叠

**约束：**
- 数组长度必须为编译期常量（`constexpr`）
- 不支持动态数组（长度不能是运行时变量）
- 不支持多维数组和嵌套数组
- 不支持与`asc_get_phy_buf_addr`方式混用

### 5.2 偏移量申请方式

通过偏移量获取物理地址，需手动管理地址分配和对齐。

**语法：**
```cpp
__<地址限定符>__ <数据类型>* ptr = (__<地址限定符>__ <数据类型>*)asc_get_phy_buf_addr(offset);
```

**示例：**
```cpp
constexpr uint32_t buffer0_size = 1024;
constexpr uint32_t buffer1_size = 512;

// 通过偏移量申请UB内存
__ubuf__ half* buffer0 = (__ubuf__ half*)asc_get_phy_buf_addr(0);
__ubuf__ half* buffer1 = (__ubuf__ half*)asc_get_phy_buf_addr(buffer0_size * sizeof(half));
__ubuf__ half* buffer2 = (__ubuf__ half*)asc_get_phy_buf_addr((buffer0_size + buffer1_size) * sizeof(half));
```

**优势：**
- 可以灵活控制内存布局
- 支持动态计算偏移量（偏移量可以是运行时变量）

**约束：**
- 不能与静态数组方式混用，否则可能导致地址重叠
- 需手动确保地址对齐（建议32字节对齐）
- 需手动计算偏移量，避免地址重叠
- 需要开发者自行管理内存空间规划
