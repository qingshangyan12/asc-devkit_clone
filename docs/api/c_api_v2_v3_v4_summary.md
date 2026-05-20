# C API 接口版本号 `_v2` / `_v3` / `_v4` 分类汇总

> 统计范围：`include/c_api/` 下所有 `.h` 头文件中的接口声明。
> 语义参考：`docs/api/c_api/` 下各接口文档说明。

---

## 总览

共涉及 **6** 个头文件、**3** 种语义模式：

| 模式 | 适用模块 | `_v2` 含义 | `_v3` / `_v4` 含义 |
|------|---------|-----------|-------------------|
| **象限/索引选择器** | `reg_convert.h` / `reg_vector.h` 中的类型转换 API | 读/写源操作数的第 1 个四分区（或下半块） | 第 2 / 第 3 个四分区 |
| **数据搬入搬出变体** | `reg_load.h` / `reg_store.h` 中的 load/store API | 支持 int32_t / iter_reg 偏移地址的版本 | B16/B32 类型的搬入变体 |
| **功能性升级版本** | `cube_datamove.h` / `atomic.h` / `reg_vector.h` | 功能增强、新增参数或不同硬件的行为版本 | — |

---

## 一、模式一：象限/索引选择器（类型转换 API）

> **语义**：当源操作数与目的操作数的位宽比为 **4:1**（或 **1:4**）时，源操作数被等分为 **4 个四分区**（索引 0~3）。无后缀版本操作索引 0，`_v2` 操作索引 1，`_v3` 操作索引 2，`_v4` 操作索引 3。
> 当位宽比为 **2:1**（或 **1:2**）时，仅有 `_v2`，表示下半块或奇数索引元素。

### 1.1 窄→宽转换（1:4 或 1:2，读源操作数的分区）

| API 基名 | 位宽比 | v2 | v3 | v4 | 说明 |
|----------|-------|:--:|:--:|:--:|------|
| `asc_uint82uint32` | 1:4 (8→32) | ✓ | ✓ | ✓ | 读 uint8 源的第 i 个四分区，转换为 uint32 |
| `asc_uint82uint16` | 1:2 (8→16) | ✓ | — | — | 读 uint8 源的下半块，转换为 uint16 |
| `asc_uint162uint32` | 1:2 (16→32) | ✓ | — | — | 读 uint16 源的下半块，转换为 uint32 |
| `asc_int82half` | 1:2 (8→16) | ✓ | — | — | 读 int8 源的奇数索引元素，转换为 half |
| `asc_int82int32` | 1:4 (8→32) | ✓ | ✓ | ✓ | 读 int8 源的第 i 个四分区，转换为 int32 |
| `asc_int82int16` | 1:2 (8→16) | ✓ | — | — | 读 int8 源的下半块，转换为 int16 |
| `asc_int4x22bfloat16` | 1:4 (4→16) | ✓ | ✓ | ✓ | 读 int4x2 源的第 i 个四分区，转换为 bf16 |
| `asc_int4x22int16` | 1:4 (4→16) | ✓ | ✓ | ✓ | 读 int4x2 源的第 i 个四分区，转换为 int16 |
| `asc_int4x22half` | 1:4 (4→16) | ✓ | ✓ | ✓ | 读 int4x2 源的第 i 个四分区，转换为 half |
| `asc_int162int32` | 1:2 (16→32) | ✓ | — | — | 读 int16 源的下半块，转换为 int32 |
| `asc_int162float` | 1:2 (16→32) | ✓ | — | — | 读 int16 源的下半块，转换为 float |
| `asc_int322int64` | 1:2 (32→64) | ✓ | — | — | 读 int32 源的下半块，转换为 int64 |
| `asc_e4m32float` | 1:4 (8→32) | ✓ | ✓ | ✓ | 读 fp8_e4m3fn 源的第 i 个四分区，转换为 float |
| `asc_e5m22float` | 1:4 (8→32) | ✓ | ✓ | ✓ | 读 fp8_e5m2 源的第 i 个四分区，转换为 float |
| `asc_e1m2x22bfloat16` | 1:4 (4→16) | ✓ | ✓ | ✓ | 读 fp4x2_e1m2 源的第 i 个四分区，转换为 bf16 |
| `asc_e2m1x22bfloat16` | 1:4 (4→16) | ✓ | ✓ | ✓ | 读 fp4x2_e2m1 源的第 i 个四分区，转换为 bf16 |
| `asc_hif82float` | 1:4 (8→32) | ✓ | ✓ | ✓ | 读 hifloat8 源的第 i 个四分区，转换为 float |
| `asc_half2int32` | 1:2 (16→32) | ✓ | — | — | 读 half 源的下半块，转换为 int32（多舍入模式） |
| `asc_bfloat162int32` | 1:2 (16→32) | ✓ | — | — | 读 bf16 源的下半块，转换为 int32（多舍入模式） |
| `asc_bfloat162float` | 1:2 (16→32) | ✓ | — | — | 读 bf16 源的下半块，转换为 float |
| `asc_int162uint32` | 1:2 (16→32) | ✓ | — | — | 读 int16 源的下半块，转换为 uint32 |
| `asc_half2float` | 1:2 (16→32) | ✓ | — | — | 读 half 源的下半块，转换为 float |
| `asc_hif82half` | 1:2 (8→16) | ✓ | — | — | 读 hifloat8 源的下半块，转换为 half |

### 1.2 宽→窄转换（4:1 或 2:1，写目标操作数的分区）

| API 基名 | 位宽比 | v2 | v3 | v4 | 说明 |
|----------|-------|:--:|:--:|:--:|------|
| `asc_uint322uint8` | 4:1 (32→8) | ✓ | ✓ | ✓ | 将 uint32 源转换后写入 uint8 dst 的第 i 个四分区 |
| `asc_int322uint8` | 4:1 (32→8) | ✓ | ✓ | ✓ | 将 int32 源转换后写入 uint8 dst 的第 i 个四分区 |
| `asc_float2e4m3` | 4:1 (32→8) | ✓ | ✓ | ✓ | 将 float 源转换后写入 fp8_e4m3fn dst 的第 i 个四分区（rn/+sat） |
| `asc_float2e5m2` | 4:1 (32→8) | ✓ | ✓ | ✓ | 将 float 源转换后写入 fp8_e5m2 dst 的第 i 个四分区（rn/+sat） |
| `asc_float2hif8` | 4:1 (32→8) | ✓ | ✓ | ✓ | 将 float 源转换后写入 hifloat8 dst 的第 i 个四分区（rh/rna +sat） |
| `asc_half2int4x2` | 2:1 (16→4×2) | ✓ | ✓ | ✓ | 将 half 源转换后写入 int4x2 dst 的第 i 个四分区（rd +sat） |
| `asc_bfloat162e2m1x2` | 4:1 (16→4×2) | ✓ | ✓ | ✓ | 将 bf16 源转换后写入 fp4x2_e2m1 dst 的第 i 个四分区（5 舍入模式） |
| `asc_bfloat162e1m2x2` | 4:1 (16→4×2) | ✓ | ✓ | ✓ | 将 bf16 源转换后写入 fp4x2_e1m2 dst 的第 i 个四分区（5 舍入模式） |
| `asc_half2int8` | 2:1 (16→8) | ✓ | — | — | 将 half 源转换后写入 int8 dst 的下半块（5 舍入模式 + sat） |
| `asc_half2uint8` | 2:1 (16→8) | ✓ | — | — | 将 half 源转换后写入 uint8 dst 的下半块（5 舍入模式） |
| `asc_float2int16` | 2:1 (32→16) | ✓ | — | — | 将 float 源转换后写入 int16 dst 的下半块（5 舍入模式 + sat） |
| `asc_float2bfloat16` | 2:1 (32→16) | ✓ | — | — | 将 float 源转换后写入 bf16 dst 的下半块（5 舍入模式 + sat） |
| `asc_float2half` | 2:1 (32→16) | ✓ | — | — | 将 float 源转换后写入 half dst 的下半块（6 舍入模式 + sat） |
| `asc_float2int64` | 1:2 (32→64) | ✓ | — | — | 将 float 源转换后写入 int64 dst 的下半块（5 舍入模式 + sat） |
| `asc_half2hif8` | 2:1 (16→8) | ✓ | — | — | 将 half 源转换后写入 hifloat8 dst 的下半块（rh/rna + sat） |
| `asc_int162uint8` | 2:1 (16→8) | ✓ | — | — | 将 int16 源转换后写入 uint8 dst 的下半块（+ sat） |
| `asc_int322uint16` | 2:1 (32→16) | ✓ | — | — | 将 int32 源转换后写入 uint16 dst 的下半块（+ sat） |
| `asc_int322int16` | 2:1 (32→16) | ✓ | — | — | 将 int32 源转换后写入 int16 dst 的下半块（+ sat） |
| `asc_uint322int16` | 2:1 (32→16) | ✓ | — | — | 将 uint32 源转换后写入 int16 dst 的下半块（+ sat） |
| `asc_uint322uint16` | 2:1 (32→16) | ✓ | — | — | 将 uint32 源转换后写入 uint16 dst 的下半块（+ sat） |
| `asc_int642int32` | 2:1 (64→32) | ✓ | — | — | 将 int64 源转换后写入 int32 dst 的下半块（+ sat） |
| `asc_uint162uint8` | 2:1 (16→8) | ✓ | — | — | 将 uint16 源转换后写入 uint8 dst 的下半块（+ sat） |

---

## 二、模式二：数据搬入/搬出地址变体（Reg Load/Store）

> **语义**：`asc_loadalign` 和 `asc_storealign` 系列 API，`_v1` 到 `_v6` 表示不同的**偏移地址管理模式**和**数据类型支持范围**。在头文件中体现为 `_v2` 和 `_v3` 后缀。

### 2.1 `asc_loadalign` 系列（UB → 矢量寄存器）

| API 名 | 偏移地址类型 | 支持数据类型 | 说明 |
|--------|------------|-------------|------|
| `asc_loadalign_unpack_v2` | 无 / `int32_t` / `iter_reg` / postupdate | B8 类型（int8/uint8/fp8 系/fp4 系/int4x2） | 解打包搬入，`_v2` 支持 int32_t/iter_reg 偏移 |
| `asc_loadalign_brc_v2` | 无 / `int32_t` / `iter_reg` / postupdate | 全部支持类型（B4~B32） | 广播搬入，`_v2` 支持 B8~B32 类型 |
| `asc_loadalign_brc_v3` | 无 / `int32_t` / `iter_reg` / postupdate | B16/B32 类型（int16/uint16/half/bf16/int32/uint32/float） | 广播搬入，`_v3` 仅支持 B16/B32 |

### 2.2 `asc_storealign` 系列（矢量寄存器 → UB）

| API 名 | 偏移地址类型 | 支持数据类型 | 说明 |
|--------|------------|-------------|------|
| `asc_storealign_pack_v2` | 无 / `int32_t` / `iter_reg` / postupdate | B32 类型（int32/uint32/float） | 打包搬出，`_v2` 支持 int32_t/iter_reg 偏移及 postupdate |

---

## 三、模式三：功能性升级版本

> **语义**：`_v2` 表示与 v1 行为不同的新版本接口，通常有额外的参数、不同的硬件支持或新增的副作用。

### 3.1 `asc_copy_l12fb_v2`（cube_datamove.h）
- **位置**：`include/c_api/cube_datamove/cube_datamove.h:1887-1892`
- **v1（原版）**：`asc_copy_l12fb` 执行 L1→Fixpipe Buffer 的简单连续搬运
- **v2**：新增高维切分搬运能力，支持 `n_burst`/`len_burst`/`src_stride`/`dst_stride` 参数；同时提供 `asc_copy_l12fb_v2_sync` 异步版本

### 3.2 `asc_set_store_atomic_config_v2`（atomic.h）
- **位置**：`include/c_api/atomic/atomic.h:79`
- **v1**：适用于 NPU_ARCH_2201（Atlas A3/A2 系列），不同的 type/op 编码
- **v2**：适用于 NPU_ARCH_3510（Ascend 950PR/950DT），扩展了原子操作类型编码

### 3.3 `asc_squeeze_v2`（reg_vector.h）
- **位置**：`include/c_api/reg_compute/reg_vector.h:461-485`
- **v1（原版）**：将源向量根据掩码压缩成更短的向量
- **v2**：额外将有效数据长度写入 AR 寄存器，供 `asc_storeunalign_postupdate` 接口使用。支持 13 种数据类型重载

### 3.4 `asc_pack_v2`（reg_vector.h）
- **位置**：`include/c_api/reg_compute/reg_vector.h:1022-1030`
- **v1（原版）**：将宽类型元素的低位部分写入 dst 的**下半块**
- **v2**：将宽类型元素的低位部分写入 dst 的**上半块**。支持 uint16→uint8、int16→uint8、uint32→uint16、int32→uint16、bool→bool

### 3.5 `asc_muls_v2`（reg_vector.h）
- **位置**：`include/c_api/reg_compute/reg_vector.h:759`
- **说明**：矢量与标量乘法后舍入为 half 的变体，将 `vector_float` 与 `float` 标量相乘后按 round 模式转换为 `vector_half`

### 3.6 `asc_exp_sub_v2`（reg_vector.h）
- **位置**：`include/c_api/reg_compute/reg_vector.h:1037-1039`
- **说明**：计算 `e^{(src0 - src1)}` 的变体，支持 half/half 和 float/float 两种输入组合

### 3.7 `asc_load3d_v2_config`（cube_datamove.h 结构体 + 文档）
- **位置**：在 `asc_set_l13d_rpt` 和 `asc_set_l13d_size` 等接口中使用
- **说明**：Load3Dv2 的 repeat 参数配置结构体，包含 `rpt_stride`/`rpt_time`/`rpt_mode` 等字段，相比 v1 扩展了方向迭代控制能力

### 3.8 `asc_set_l13d_rpt`（cube_datamove.h 相关接口）
- **说明**：配合 `asc_load3d_v2_config` 使用，设置 Load3Dv2 的 repeat 参数

---

## 附录 A：按头文件分布的 v2/v3/v4 接口统计

| 头文件 | v2 计数 | v3 计数 | v4 计数 |
|--------|:------:|:------:|:------:|
| `reg_convert.h` | 63 | 27 | 19 |
| `reg_load.h` | ~120 | ~28 | 0 |
| `reg_store.h` | 12 | 0 | 0 |
| `reg_vector.h` | 37 | 2 | 2 |
| `cube_datamove.h` | 3 | 0 | 0 |
| `atomic.h` | 1 | 0 | 0 |
| **合计** | **~236** | **~57** | **~21** |

> 注：reg_load.h/reg_store.h 中每个 v2 函数对应多种数据类型重载（模板展开），上述为声明行数。

## 附录 B：v2/v3/v4 语义速查

| 后缀 | 典型含义 |
|------|---------|
| `_v2` | ① 类型转换的第 2 个四分区/下半块；② reg load/store 的 int32_t/iter_reg 偏移模式；③ 功能性增强版本 |
| `_v3` | ① 类型转换的第 3 个四分区（仅 1:4/4:1 转换）；② loadalign_brc 的 B16/B32 变体 |
| `_v4` | 类型转换的第 4 个四分区（仅 1:4/4:1 转换） |

---

## 附录 C：重命名方案

> 目标：消除 `_v2`/`_v3`/`_v4` 中的语义模糊性，将版本号替换为描述性后缀，同时保持全小写+下划线风格并控制函数名长度。

### C.1 模式一（象限选择器）：`_segN` / `_lo`

> **核心语义**：Version number = 在源/目的向量中选择第 N 个分区（1:4 位宽比时为四分区，1:2 位宽比时为下半块）。使用硬件文档常用的 0-based 索引，`segN` 中的 N 即分区索引。

| 原后缀 | 新后缀 | 含义 | 适用位宽比 |
|--------|--------|------|-----------|
| (无后缀) | `_seg0` | 操作第 0 个分区 | 1:4 / 4:1 |
| `_v2` | `_seg1` | 操作第 1 个分区 | 1:4 / 4:1 |
| `_v3` | `_seg2` | 操作第 2 个分区 | 1:4 / 4:1 |
| `_v4` | `_seg3` | 操作第 3 个分区 | 1:4 / 4:1 |
| (无后缀) | `_hi` | 操作上半块（高位） | 1:2 / 2:1 |
| `_v2` | `_lo` | 操作下半块（低位） | 1:2 / 2:1 |

**重命名示例**：

```
asc_uint82uint32       → asc_uint82uint32_seg0   (读 uint8 源的分区 0，转为 uint32)
asc_uint82uint32_v2    → asc_uint82uint32_seg1   (读 uint8 源的分区 1，转为 uint32)
asc_uint82uint32_v3    → asc_uint82uint32_seg2   (读 uint8 源的分区 2，转为 uint32)
asc_uint82uint32_v4    → asc_uint82uint32_seg3   (读 uint8 源的分区 3，转为 uint32)

asc_float2e4m3_rn      → asc_float2e4m3_rn_hi   (float 转换后写入 fp8 dst 的上半块)
asc_float2e4m3_rn_v2   → asc_float2e4m3_rn_seg1  (float 转换后写入 fp8 dst 的分区 1)
asc_float2e4m3_rn_sat  → asc_float2e4m3_rn_sat_hi (带饱和，写入上半块)
asc_float2e4m3_rn_sat_v2 → asc_float2e4m3_rn_sat_seg1

asc_half2int4x2_rd_v2  → asc_half2int4x2_rd_seg1 (half→int4 舍入到分区 1)
asc_half2int4x2_rd_v3  → asc_half2int4x2_rd_seg2
asc_half2int4x2_rd_v4  → asc_half2int4x2_rd_seg3

asc_half2int8_rn       → asc_half2int8_rn_hi     (half→int8，写入上半块)
asc_half2int8_rn_v2    → asc_half2int8_rn_lo     (写入下半块)

asc_int82half          → asc_int82half_hi        (读 int8 上半块，转为 half)
asc_int82half_v2       → asc_int82half_lo        (读 int8 下半块，转为 half)
```

> **设计说明**：`seg`（segment）为 3 字母，`hi`/`lo` 为 2 字母，均短于原 `_v2`/`_v3`/`_v4`。统一使用 0-based 索引和 `hi`/`lo` 对，消除版本号歧义。

---

### C.2 模式二（Reg Load/Store）：删除冗余版本号

> **核心语义**：`asc_loadalign` / `asc_storealign` 系列中 `_v2`/`_v3` 并不表示"第二/第三版"功能，而是对数据类型覆盖范围的区分编码。不同版本之间通过 C++ 函数重载（不同 `vector_xxx_t` 类型参数）即可自然区分，无需在函数名中编码。

**重命名规则**：将版本号后缀删除，利用已有的模式前缀（`unpack`/`brc`/`pack`）体现语义。

| 原名 | 新名 | 说明 |
|------|------|------|
| `asc_loadalign_unpack_v2` | `asc_loadalign_unpack` | 解打包搬入；新名去除冗余 v2 |
| `asc_loadalign_brc_v2` | `asc_loadalign_brc` | 广播搬入（全类型 B8/B16/B32） |
| `asc_loadalign_brc_v3` | `asc_loadalign_brc` | 广播搬入（B16/B32 仅部分重载，可与 v2 重载合并） |
| `asc_storealign_pack_v2` | `asc_storealign_pack` | 打包搬出；新名去除冗余 v2 |
| `asc_loadalign_unpack_postupdate_v2` | `asc_loadalign_unpack_post` | postupdate 的 `post` 前缀已足够区分 |
| `asc_loadalign_brc_postupdate_v2` | `asc_loadalign_brc_post` | 同上 |
| `asc_loadalign_brc_postupdate_v3` | `asc_loadalign_brc_post` | 同上 |
| `asc_storealign_pack_postupdate_v2` | `asc_storealign_pack_post` | 同上 |

**重命名示例**：

```
asc_loadalign_brc_v2(dst, src)      → asc_loadalign_brc(dst, src)
asc_loadalign_brc_v3(dst, src)      → asc_loadalign_brc(dst, src)       // 靠 vector_float vs vector_half 类型重载区分
asc_loadalign_brc_v2(dst, src, off) → asc_loadalign_brc(dst, src, off)
asc_loadalign_brc_v3(dst, src, off) → asc_loadalign_brc(dst, src, off)

asc_storealign_pack_v2(dst, src, mask) → asc_storealign_pack(dst, src, mask)
```

> **设计说明**：此方案将命名空间从 "操作名 + 模式 + 版本号" 简化为 "操作名 + 模式"，版本号信息由参数类型（重载）自然承载。对 C 调用者而言，需将 `v2`/`v3` 的判别逻辑改为依据 `vector_xxx_t` 类型选择对应重载。

---

### C.3 模式三（功能性升级）：语义化后缀替换版本号

> **核心语义**：每个 `_v2` 都有独立的升级原因（功能扩展、硬件适配或新增副作用），需要逐一替换为描述该差异的简短后缀。

| 原名 | 新名 | 后缀语义 | 命名理由 |
|------|------|---------|---------|
| `asc_copy_l12fb_v2` | `asc_copy_l12fb_tile` | 分块（tiled）搬运 | v2 相比 v1 新增 `n_burst`/`len_burst`/stride 参数，实现分块搬运 |
| `asc_copy_l12fb_v2_sync` | `asc_copy_l12fb_tile_sync` | 同上 + 异步 | 对应的异步版本 |
| `asc_set_store_atomic_config_v1` | `asc_set_store_atomic_config` | 基础版 | v1 作为无后缀的默认名 |
| `asc_set_store_atomic_config_v2` | `asc_set_store_atomic_config_950` | NPU 950 专用 | 语义完全不同的编码表，按目标架构命名最清晰 |
| `asc_squeeze_v2` | `asc_squeeze_ar` | 附加 AR 寄存器写入 | v2 在压缩之外额外将有效长度写入 AR 寄存器 |
| `asc_pack` | `asc_pack_lo` | 写入 dst 低半块 | 原 pack 写入低半块，命名显式化 |
| `asc_pack_v2` | `asc_pack_hi` | 写入 dst 高半块 | v2 写入高半块，与 lo 形成对称对 |
| `asc_exp_sub` | `asc_exp_sub` | （不变） | 无 v1 竞争，移除无意义的默认 v0 |
| `asc_exp_sub_v2` | `asc_exp_sub` | 合并 | v2 只是多类型重载，无版本差异 |
| `asc_muls` | `asc_muls_to_half` | float→half 的乘标量 | 原 muls 语义不明确，新名同时体现操作和输出类型 |
| `asc_muls_v2` | `asc_muls_to_half` | 同上 | 与 v1 功能相同仅舍入模式不同，可合并 |

> **设计说明**：功能性升级的接口名称差异较大，无法用统一规则替换。原则是：后缀用 ≤8 字符的英文缩写描述新增功能（如 `_tile`、`_ar`、`_hi`/`_lo`、`_950`），确保开发者看到函数名即能理解其与基础版本的区别。

---

### C.4 全量重命名对照速查表

#### C.4.1 类型转换系列（部分代表）

| 原名 | 新名 |
|------|------|
| `asc_uint82uint32` | `asc_uint82uint32_seg0` |
| `asc_uint82uint32_v2` | `asc_uint82uint32_seg1` |
| `asc_uint82uint32_v3` | `asc_uint82uint32_seg2` |
| `asc_uint82uint32_v4` | `asc_uint82uint32_seg3` |
| `asc_int82half` | `asc_int82half_hi` |
| `asc_int82half_v2` | `asc_int82half_lo` |
| `asc_int4x22half` | `asc_int4x22half_seg0` |
| `asc_int4x22half_v2` | `asc_int4x22half_seg1` |
| `asc_int4x22half_v3` | `asc_int4x22half_seg2` |
| `asc_int4x22half_v4` | `asc_int4x22half_seg3` |
| `asc_half2float` | `asc_half2float_hi` |
| `asc_half2float_v2` | `asc_half2float_lo` |
| `asc_float2e4m3_rn` | `asc_float2e4m3_rn_hi` |
| `asc_float2e4m3_rn_v2` | `asc_float2e4m3_rn_seg1` |
| `asc_float2e4m3_rn_sat_v2` | `asc_float2e4m3_rn_sat_seg1` |
| `asc_float2e4m3_rn_v3` | `asc_float2e4m3_rn_seg2` |
| `asc_float2e4m3_rn_v4` | `asc_float2e4m3_rn_seg3` |
| `asc_bfloat162e1m2x2_rd_v2` | `asc_bfloat162e1m2x2_rd_seg1` |
| `asc_bfloat162e1m2x2_rd_v3` | `asc_bfloat162e1m2x2_rd_seg2` |
| `asc_bfloat162e1m2x2_rd_v4` | `asc_bfloat162e1m2x2_rd_seg3` |

#### C.4.2 Reg Load/Store 系列

| 原名 | 新名 |
|------|------|
| `asc_loadalign_unpack_v2` | `asc_loadalign_unpack` |
| `asc_loadalign_brc_v2` | `asc_loadalign_brc` |
| `asc_loadalign_brc_v3` | `asc_loadalign_brc` |
| `asc_loadalign_unpack_postupdate_v2` | `asc_loadalign_unpack_post` |
| `asc_loadalign_brc_postupdate_v2` | `asc_loadalign_brc_post` |
| `asc_loadalign_brc_postupdate_v3` | `asc_loadalign_brc_post` |
| `asc_storealign_pack_v2` | `asc_storealign_pack` |
| `asc_storealign_pack_postupdate_v2` | `asc_storealign_pack_post` |

#### C.4.3 功能性升级系列

| 原名 | 新名 |
|------|------|
| `asc_copy_l12fb_v2` | `asc_copy_l12fb_tile` |
| `asc_copy_l12fb_v2_sync` | `asc_copy_l12fb_tile_sync` |
| `asc_set_store_atomic_config_v1` | `asc_set_store_atomic_config` |
| `asc_set_store_atomic_config_v2` | `asc_set_store_atomic_config_950` |
| `asc_squeeze_v2` | `asc_squeeze_ar` |
| `asc_pack` | `asc_pack_lo` |
| `asc_pack_v2` | `asc_pack_hi` |
| `asc_muls` | `asc_muls_to_half` |
| `asc_muls_v2` | `asc_muls_to_half` |
| `asc_exp_sub` | `asc_exp_sub` |
| `asc_exp_sub_v2` | `asc_exp_sub` |

---

### C.5 方案总结

| 原模式 | 新规则 | 关键后缀 | 命名示例 |
|--------|--------|---------|---------|
| 模式一（象限选择） | `_seg{N}` / `_hi` / `_lo` | `seg0~seg3`, `hi`, `lo` | `asc_int82half_lo` |
| 模式二（Load/Store） | 去除版本号，靠类型重载区分 | （无额外后缀） | `asc_loadalign_brc` |
| 模式三（功能升级） | 逐一替换为语义后缀 | `tile`, `ar`, `hi`/`lo`, `950`, `to_half` | `asc_pack_hi` |

**原则**：
1. 全小写 + 下划线风格
2. 新增后缀长度控制在 2~8 字符（`hi` 最短 2 字符，`to_half` 最长 7 字符，均 ≤ 原 `_v2`/`_v3`/`_v4`）
3. 从函数名即可直接理解参数选择（segN=第N分区, hi/lo=高位/低位, ar=写AR寄存器）
4. 对单次仅选半块的 2:1/1:2 转换，用 `_hi`/`_lo` 二维对代替 `_v2`；对四分区用 `_seg0`~`_seg3`
