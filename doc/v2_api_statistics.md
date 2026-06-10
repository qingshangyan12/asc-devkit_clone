# V2 接口统计与分类

## 概述

在 `impl/c_api/instr_impl/npu_arch_3510/` 目录下，统计所有带有 `_v2_impl`（及 `_v3_impl`, `_v4_impl`）后缀的实现函数，按功能领域分类归纳。

所有 v2/v3/v4 变体仅在 `npu_arch_3510/` 中存在，`npu_arch_2201/` 仅有 v1 基础版本。

---

## 一、类型转换类 (vconv)

### 1.1 两位变体 — v2 only（对应 `asc_position::EVEN/ODD`）

**特征**: 源/目的操作数位宽比为 **1:2 或 2:1**，256B 向量分为上下两半：
- v1 (基础): 读取/写入上半部分 (EVEN, PART_EVEN, 索引 0)
- v2: 读取/写入下半部分 (ODD, PART_ODD, 索引 1)

共 **28 个基础转换方向**，展开后 **110 个 `_v2_impl` 函数**。

| 转换方向 | 位宽比 | 舍入/饱和变体 | v2 函数数 |
|---|---|---|---|
| `asc_bfloat162float` | 1:2 | - | 1 |
| `asc_bfloat162int32` | 1:2 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_float2bfloat16` | 2:1 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_float2half` | 2:1 | rd/rn/rna/ru/rz/ro × sat/non-sat | 12 |
| `asc_float2int16` | 2:1 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_float2int64` | 1:2 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_half2float` | 1:2 | - | 1 |
| `asc_half2hif8` | 2:1 | rh/rna × sat/non-sat | 4 |
| `asc_half2int32` | 1:2 | rd/rn/rna/ru/rz | 5 |
| `asc_half2int8` | 2:1 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_half2uint8` | 2:1 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_hif82half` | 1:2 | - | 1 |
| `asc_int162float` | 1:2 | - | 1 |
| `asc_int162int32` | 1:2 | - | 1 |
| `asc_int162uint32` | 1:2 | - | 1 |
| `asc_int162uint8` | 2:1 | sat/non-sat | 2 |
| `asc_int322int16` | 2:1 | sat/non-sat | 2 |
| `asc_int322int64` | 1:2 | - | 1 |
| `asc_int322uint16` | 2:1 | sat/non-sat | 2 |
| `asc_int642float` | 2:1 | rd/rn/rna/ru/rz | 5 |
| `asc_int642int32` | 2:1 | sat/non-sat | 2 |
| `asc_int82half` | 1:2 | - | 1 |
| `asc_int82int16` | 1:2 | - | 1 |
| `asc_uint162uint32` | 1:2 | - | 1 |
| `asc_uint162uint8` | 2:1 | sat/non-sat | 2 |
| `asc_uint322int16` | 2:1 | sat/non-sat | 2 |
| `asc_uint82half` | 1:2 | - | 1 |
| `asc_uint82uint16` | 1:2 | - | 1 |

**v2 语义**: 源操作数位宽小于目的时，v2 从源操作数的奇数位/下半部分读取数据；源操作数位宽大于目的时，v2 将结果写入目的操作数的奇数位/下半部分。

---

### 1.2 四位变体 — v2 + v3 + v4（对应 `asc_disperse_quarter::FIRST/SECOND/THIRD/FOURTH`）

**特征**: 源/目的操作数位宽比为 **1:4 或 4:1**，256B 向量分为四个季度：
- v1 (基础): 第一个季度 (FIRST, PART_P0)
- v2: 第二个季度 (SECOND, PART_P1)
- v3: 第三个季度 (THIRD, PART_P2)
- v4: 第四个季度 (FOURTH, PART_P3)

共 **18 个基础转换方向**，每个方向有 v2/v3/v4 共 3 个变体，总计 **42 个 `_v3_impl` + 42 个 `_v4_impl`** 函数。

| 转换方向 | 位宽比 | 舍入/饱和变体 | 每变体数 |
|---|---|---|---|
| `asc_bfloat162e1m2x2` | 4:1 | rd/rn/rna/ru/rz | 5 |
| `asc_bfloat162e2m1x2` | 4:1 | rd/rn/rna/ru/rz | 5 |
| `asc_e1m2x22bfloat16` | 1:4 | - | 1 |
| `asc_e2m1x22bfloat16` | 1:4 | - | 1 |
| `asc_e4m32float` | 1:4 | - | 1 |
| `asc_e5m22float` | 1:4 | - | 1 |
| `asc_float2e4m3` | 4:1 | rn × sat/non-sat | 2 |
| `asc_float2e5m2` | 4:1 | rn × sat/non-sat | 2 |
| `asc_float2hif8` | 4:1 | rh/rna × sat/non-sat | 4 |
| `asc_half2int4x2` | 4:1 | rd/rn/rna/ru/rz × sat/non-sat | 10 |
| `asc_hif82float` | 1:4 | - | 1 |
| `asc_int4x22bfloat16` | 1:4 | - | 1 |
| `asc_int4x22half` | 1:4 | - | 1 |
| `asc_int4x22int16` | 1:4 | - | 1 |
| `asc_int322uint8` | 4:1 | sat/non-sat | 2 |
| `asc_int82int32` | 1:4 | - | 1 |
| `asc_uint322uint8` | 4:1 | sat/non-sat | 2 |
| `asc_uint82uint32` | 1:4 | - | 1 |

**v2/v3/v4 语义**: 与两位变体同理，但分为 4 个季度。

---

## 二、数据重排类

### 2.1 `asc_pack_v2`

| 位宽变化 | 支持类型 | 函数数 | 说明 |
|---|---|---|---|
| 16→8bit | u16, i16 → u8 | 2 | v1 写入低半部分, v2 写入高半部分 |
| 32→16bit | u32, i32 → u16 | 2 | 同上 |
| bool → bool | bool | 1 | 位压缩，v2 写入高半部分 |

**v2 语义**: v1 写入低半部分，v2 写入高半部分。

### 2.2 `asc_squeeze_v2`

覆盖 13 种数据类型。**v2 语义**: v1 仅压缩，v2 额外将有效元素数量写入 AR 寄存器。

---

## 三、数据搬移类

### 3.1 `asc_loadalign` (向量加载)

| 模式 | v2 函数 | 说明 |
|---|---|---|
| UNPACKV2 | `asc_loadalign_unpack_v2`, `asc_loadalign_unpack_postupdate_v2` | 1:4 解压 (quarter→full)，中间补零 |
| BRCV2 | `asc_loadalign_brc_v2`, `asc_loadalign_brc_postupdate_v2` | 广播 32B DataBlock |

### 3.2 `asc_storealign` (向量存储)

| 模式 | v2 函数 | 说明 |
|---|---|---|
| PACKV2 | `asc_storealign_pack_v2`, `asc_storealign_pack_postupdate_v2` | 保留低 8bit，32→8bit 压缩 |

---

## 四、特殊计算类

| 接口 | v2 说明 |
|---|---|
| `asc_muls_v2` | v1 写入偶数位，v2 写入奇数位 |
| `asc_exp_sub_v2` | v1 从偶数位读取，v2 从奇数位读取 |

---

## 五、原子配置类

| 接口 | 说明 |
|---|---|
| `asc_set_store_atomic_config_v2` | v2 版本，扩展支持更多数据类型 |

---

## 六、汇总统计

| 分类 | 基础接口数 | `_v2_impl` | `_v3_impl` | `_v4_impl` |
|---|---|---|---|---|
| 类型转换 (v2 only) | 28 | 110 | 0 | 0 |
| 类型转换 (v2+v3+v4) | 18 | 42 | 42 | 42 |
| 数据重排 | 2 | 18 | 0 | 0 |
| 数据搬移 | 5 | ~32 | 7 | 0 |
| 特殊计算 | 2 | 2 | 0 | 0 |
| 原子配置 | 1 | 1 | 0 | 0 |
| **总计** | **56** | **~205** | **49** | **42** |

---

## 七、`_v2`/`_v3`/`_v4` 语义归纳

| 应用场景 | v1 | v2 | v3/v4 |
|---|---|---|---|
| 类型转换 (位宽比 1:2 或 2:1) | 偶数位 / 上半部分 | 奇数位 / 下半部分 | — |
| 类型转换 (位宽比 1:4 或 4:1) | 第 1 个季度 | 第 2 个季度 | 第 3/4 个季度 |
| asc_pack | 写入低半部分 | 写入高半部分 | — |
| asc_squeeze | 不保存 AR | 保存有效大小到 AR | — |
| asc_loadalign UNPACK | 1:2 解压 | UNPACKV2 = 1:4 解压 | — |
| asc_loadalign BRC | — | BRCV2 = 广播 32B | — |
| asc_storealign PACK | 半位宽压缩 | PACKV2 = 32→8bit | — |
| asc_muls | 写入偶数位 | 写入奇数位 | — |
| asc_exp_sub | 从偶数位读取 | 从奇数位读取 | — |
| asc_set_store_atomic_config | 基础版本 | v2 版本 | — |
