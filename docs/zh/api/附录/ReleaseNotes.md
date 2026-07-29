# Ascend C SIMD C-API Release Notes

> **版本**：master（基于 CANN 9.1.0 基线）
> **适用产品**：Ascend 950PR / Ascend 950DT
> **接口范围**：SIMD C-API reg 矢量寄存器计算接口
> **基线对比版本**：CANN 9.1.0

---

## 1. 新增特性

### 1.1 新增数据搬运接口

本版本新增 8 个数据加载（Load）系列接口，支持广播（Broadcast）和解包（Unpack）模式，覆盖全数据类型（int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float/int64/uint64 以及 fp8/hifloat8/fp4/int4 等压缩数据类型）：

| 接口 | 说明 | 新增原型数 |
| --- | --- | --- |
| `asc_loadalign_brc_datablock` | 按 DataBlock 粒度广播加载对齐数据 | 48 |
| `asc_loadalign_brc_datablock_postupdate` | 上述接口的地址自更新版本 | 16 |
| `asc_loadalign_brc_elem` | 按 Element 粒度广播加载对齐数据 | 48 |
| `asc_loadalign_brc_elem_postupdate` | 上述接口的地址自更新版本 | 16 |
| `asc_loadalign_brc_elem2datablock` | Element 到 DataBlock 粒度广播加载 | 21 |
| `asc_loadalign_brc_elem2datablock_postupdate` | 上述接口的地址自更新版本 | 7 |
| `asc_loadalign_unpack4` | 4 倍解包加载（int4x2/fp4x2 专用） | 27 |
| `asc_loadalign_unpack4_postupdate` | 上述接口的地址自更新版本 | 9 |

### 1.2 新增数据存储接口

| 接口 | 说明 | 新增原型数 |
| --- | --- | --- |
| `asc_storealign_pack_quarter` | 四分之一打包存储（int32/uint32/float 专用） | 9 |
| `asc_storealign_pack_quarter_postupdate` | 上述接口的地址自更新版本 | 3 |

### 1.3 新增数据压缩接口

| 接口 | 说明 | 新增原型数 |
| --- | --- | --- |
| `asc_pack_to_high` | 将源操作数的高 8 位打包写入目的操作数 | 3 |
| `asc_pack_to_low` | 将源操作数的低 8 位打包写入目的操作数 | 3 |

### 1.4 新增比较选择接口

| 接口 | 说明 | 新增原型数 |
| --- | --- | --- |
| `asc_squeeze_with_status` | 带 Squeeze 状态输出的数据压缩选择 | 13 |

### 1.5 新增地址寄存器接口

| 接口 | 说明 | 新增原型数 |
| --- | --- | --- |
| `asc_update_addr_reg_b8` | 初始化/更新 b8 位宽地址偏移寄存器（支持 1~4 维） | 4 |
| `asc_update_addr_reg_b16` | 初始化/更新 b16 位宽地址偏移寄存器（支持 1~4 维） | 4 |
| `asc_update_addr_reg_b32` | 初始化/更新 b32 位宽地址偏移寄存器（支持 1~4 维） | 4 |

### 1.6 新增数据类型支持

在 42 个已有接口上新增了数据类型重载，主要变更如下：

**算术计算类**

- `asc_add` / `asc_sub` / `asc_addc` / `asc_subc`：新增 `bfloat16_t` 类型支持；新增带进位/借位的 `int32_t` / `uint32_t` 重载
- `asc_add_scalar`：新增 `bfloat16_t` 标量加法支持
- `asc_div`：新增 `int16_t` / `uint16_t` / `int32_t` / `uint32_t` 整数除法支持
- `asc_max` / `asc_min` / `asc_mul` / `asc_mull` / `asc_max_scalar` / `asc_min_scalar`：新增 `bfloat16_t` 类型支持

**比较选择类**

- `asc_select`：新增 `bfloat16_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`hifloat8_t`、`uint16_t`、`uint32_t` 类型支持

**逻辑计算类**

- `asc_and`：新增 `bfloat16_t` 类型支持
- `asc_shiftleft` / `asc_shiftright` 及其 `_scalar` 变体：新增 `uint16_t` / `uint32_t` / `bfloat16_t` / `int64_t` 等类型支持

**数据搬运类**

- `asc_loadalign` / `asc_loadalign_deintlv` / `asc_loadalign_unpack` / `asc_loadalign_downsample` / `asc_loadalign_upsample` / `asc_loadalign_postupdate`：全面新增 `addr_reg offset` 参数重载及 `bfloat16_t` / `fp8` / `hifloat8` / `fp4` / `int4` 等新数据类型支持
- `asc_loadunalign` / `asc_loadunalign_pre` / `asc_loadunalign_postupdate`：新增 `int4b_t` / `fp4x2` 类型支持及 `addr_reg&` 参数重载
- `asc_gather` / `asc_gather_datablock`：新增 `bfloat16_t` / `fp8` / `hifloat8` / `int4` 类型及 `vector_uint16_t index` 重载
- `asc_scatter`：新增 `addr_reg offset` 参数重载及 `int4b_t` / `fp4x2` 类型支持
- `asc_storealign` / `asc_storealign_1st` / `asc_storealign_intlv` / `asc_storealign_pack` / `asc_storealign_postupdate` / `asc_storealign_pack_postupdate` / `asc_storealign_1st_postupdate`：全面新增 `addr_reg offset` 参数重载及 `int4b_t` / `fp4x2` 类型支持
- `asc_storeunalign` / `asc_storeunalign_post` / `asc_storeunalign_postupdate` / `asc_storeunalign_post_postupdate`：新增 `addr_reg& count` 参数重载及 `int4b_t` / `int4x2_t` 类型支持

**数据重排类**

- `asc_deintlv` / `asc_intlv`：新增 `int8_t` / `uint8_t` / `int16_t` / `uint16_t` / `hifloat8_t` / `fp8` / `bfloat16_t` / `int4b_t` 等类型支持

---

## 2. Bug 修复

### 2.1 文档与头文件一致性问题修复

- **CANN 9.1.0 遗留问题**：9.1.0 版本中存在 95 个文档原型与头文件声明类型不匹配的问题（主要集中在 `asc_loadalign_deintlv_postupdate`、`asc_loadalign_postupdate`、`asc_loadalign_v4` 等接口），本版本已对头文件声明进行对齐修正
- **头文件缺失声明修复**：9.1.0 版本中 1 个文档原型在头文件中缺失声明的问题已修复
- **master 版本残留问题**：当前 master 版本仍有 910 个文档原型与头文件存在类型命名差异（主要为 `addr_reg` vs `iter_reg`、`vector_half` vs `vector_f16` 等类型名规范化变更引起），将在后续版本持续对齐

### 2.2 类型命名规范化

- 修复了 `vector_f16` 类型名，统一规范为 `vector_half`
- 修复了 `vector_f4e1m2x2` / `vector_f4e2m1x2` 类型名，统一规范为 `vector_fp4x2_e1m2_t` / `vector_fp4x2_e2m1_t`

---

## 3. 增强

### 3.1 地址寄存器（addr_reg）机制增强

- 引入全新的 `addr_reg` 地址偏移寄存器类型，替代原有 `iter_reg`，支持在数据搬运接口中使用地址寄存器进行多维循环地址偏移
- `addr_reg` 支持最多 4 层循环轴，通过 `asc_update_addr_reg_b8/b16/b32` 初始化，地址计算公式为 `offset = Σ(index_i × stride_i)`
- 数量上限为 8 个，编译器自动复用生命周期结束的 `addr_reg` 寄存器
- 涉及增强的接口包括：`asc_loadalign`、`asc_loadalign_deintlv`、`asc_loadalign_unpack`、`asc_loadalign_downsample`、`asc_loadalign_upsample`、`asc_storealign`、`asc_storealign_1st`、`asc_storealign_intlv`、`asc_storealign_pack`、`asc_scatter`、`asc_loadunalign_pre`、`asc_loadunalign_postupdate`、`asc_storeunalign_post`、`asc_storeunalign_postupdate` 等

### 3.2 PostUpdate 地址自更新模式增强

- 在原有 `int32_t offset` 和 `iter_reg offset` 参数基础上，新增 `addr_reg offset` 和 `addr_reg& count` 参数的 PostUpdate 重载
- 涉及接口：`asc_loadalign_brc_datablock_postupdate`、`asc_loadalign_brc_elem_postupdate`、`asc_loadalign_brc_elem2datablock_postupdate`、`asc_loadalign_unpack4_postupdate`、`asc_storealign_pack_quarter_postupdate` 等

### 3.3 新数据类型全链路支持

本版本在算术计算、比较选择、逻辑运算、数据搬运等全链路接口上新增了对以下数据类型的支持：

| 数据类型 | 说明 | 涉及接口数 |
| --- | --- | --- |
| `bfloat16_t` | BF16 浮点 | 15+ |
| `fp8_e4m3fn_t` | FP8 E4M3 格式 | 10+ |
| `fp8_e5m2_t` | FP8 E5M2 格式 | 10+ |
| `fp8_e8m0_t` | FP8 E8M0 格式 | 8+ |
| `hifloat8_t` | HiFloat8 格式 | 8+ |
| `fp4x2_e1m2_t` | FP4 E1M2 打包格式 | 8+ |
| `fp4x2_e2m1_t` | FP4 E2M1 打包格式 | 8+ |
| `int4x2_t` / `int4b_t` | INT4 打包格式 | 8+ |
| `int64_t` / `uint64_t` | 64 位整数 | 5+ |

### 3.4 接口废弃（Deprecation）

本版本对以下 43 个旧版接口/重载标记为 **deprecated**，建议使用新版本替代：

**数据类型转换类（30 个）**

- `asc_bfloat162e1m2x2_rn_v2/v3/v4`、`asc_bfloat162e1m2x2_rna_v2/v3/v4`、`asc_bfloat162e1m2x2_rd_v2/v3/v4`、`asc_bfloat162e1m2x2_ru_v2/v3/v4`、`asc_bfloat162e1m2x2_rz_v2/v3/v4`
- `asc_bfloat162e2m1x2_rn_v2/v3/v4`、`asc_bfloat162e2m1x2_rna_v2/v3/v4`、`asc_bfloat162e2m1x2_rd_v2/v3/v4`、`asc_bfloat162e2m1x2_ru_v2/v3/v4`、`asc_bfloat162e2m1x2_rz_v2/v3/v4`
- **替代方案**：使用统一命名的 `asc_bfloat162e1m2x2_rn` / `asc_bfloat162e2m1x2_rn` 等基础版本

**数据搬运类（9 个）**

- `asc_loadalign_brc` / `asc_loadalign_brc_v2` / `asc_loadalign_brc_v3`
- `asc_loadalign_brc_postupdate` / `asc_loadalign_brc_postupdate_v2` / `asc_loadalign_brc_postupdate_v3`
- `asc_loadalign_unpack_v2` / `asc_loadalign_unpack_postupdate_v2`
- **替代方案**：使用 `asc_loadalign_brc_datablock` / `asc_loadalign_brc_elem` 等新接口

**数据存储类（2 个）**

- `asc_storealign_pack_v2` / `asc_storealign_pack_postupdate_v2`
- **替代方案**：使用 `asc_storealign_pack` / `asc_storealign_pack_postupdate` 基础版本

**迭代寄存器类（3 个）**

- `asc_create_iter_reg_b8` / `asc_create_iter_reg_b16` / `asc_create_iter_reg_b32`
- **替代方案**：使用 `asc_update_addr_reg_b8` / `asc_update_addr_reg_b16` / `asc_update_addr_reg_b32`

### 3.5 向后兼容性

- **零删除**：本版本未删除任何 CANN 9.1.0 中已有的接口函数，完全向后兼容
- **类型名迁移**：`vector_f16` → `vector_half`、`vector_f4e1m2x2` → `vector_fp4x2_e1m2_t` 等类型名变更属于命名规范化，底层语义不变

---

## 附录：变更统计

| 变更类型 | 数量 |
| --- | --- |
| 新增接口函数 | 16 |
| 新增数据类型重载 | 42 个接口，共 633 个新原型 |
| 废弃接口/重载 | 43 |
| 删除接口 | 0 |
| 文档原型总数 | 2746（9.1.0 为 2113） |
| 头文件声明总数 | 2352（9.1.0 为 2113） |
