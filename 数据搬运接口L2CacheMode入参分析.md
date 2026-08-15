# 数据搬运接口 L2 Cache Mode 入参分析

> 分析范围：`docs/zh/api/SIMD-API/c_api/cube_data_move` 与 `docs/zh/api/SIMD-API/c_api/vector_data_move` 两个目录下，所有「从 GM 搬出」与「搬入 GM」的搬运接口。

## 一、背景说明

搬运接口中控制 L2 Cache 管理策略的入参，在不同接口中命名与类型略有差异，语义等价：

| 参数名 | 类型 | 取值含义 |
|--------|------|----------|
| `l2_cache_ctl` | `uint8_t` | 0=正常模式、1=LAST模式、2=PERSISTENT模式、4=DISABLE模式 |
| `l2_cache_mode` | `uint8_t` 或枚举类型 | 语义等价，控制 L2 Cache 管理策略 |
| `cache_mode` | `uint8_t`（已废弃） | 语义等价 |

**判定标准**：函数原型参数列表中包含上述任一参数，即视为「有 L2 Cache Mode 入参」；否则视为「无」。

## 二、完整清单

### （一）从 GM 搬出（gm2xxx）

| 接口名 | 架构 | 是否有 L2 Cache 入参 | 参数名 |
|--------|------|---------------------|--------|
| `asc_copy_gm2l0a` | — | ❌ 无 | — |
| `asc_copy_gm2l0b` | — | ❌ 无 | — |
| `asc_copy_gm2l1` | 3510（填充模式） | ❌ 无 | — |
| `asc_copy_gm2l1` | 3510（2D搬运模式） | ✅ 有 | `l2_cache_ctl` |
| `asc_copy_gm2l1` | 2201 | ❌ 无 | — |
| `asc_copy_gm2l1_align` | — | ✅ 有 | `l2_cache_ctl` |
| `asc_copy_gm2l1_dn2nz` | — | ✅ 有 | `l2_cache_ctl` |
| `asc_copy_gm2l1_nd2nz` | 3510 | ✅ 有 | `l2_cache_ctl` |
| `asc_copy_gm2l1_nd2nz` | 2201 | ❌ 无 | — |
| `asc_copy_gm2ub` | 3510 / 2201 | ❌ 无 | — |
| `asc_copy_gm2ub_align` | 3510 | ✅ 有 | `l2_cache_mode` |
| `asc_copy_gm2ub_align` | 2201 | ❌ 无 | — |
| `asc_ndim_copy_gm2ub` | — | ✅ 有 | `l2_cache_mode` / `cache_mode` |

### （二）搬入 GM（xxx2gm）

| 接口名 | 架构 | 是否有 L2 Cache 入参 | 参数名 |
|--------|------|---------------------|--------|
| `asc_copy_l0c2gm` | 3510 | ✅ 有 | `l2_cache_mode` |
| `asc_copy_l0c2gm` | 2201 | ❌ 无 | — |
| `asc_copy_l12gm` | — | ❌ 无 | — |
| `asc_copy_ub2gm` | 3510 / 2201 | ❌ 无 | — |
| `asc_copy_ub2gm_align` | 3510 | ✅ 有 | `l2_cache_mode` |
| `asc_copy_ub2gm_align` | 2201 | ❌ 无 | — |

## 三、例外接口清单（无 L2 Cache Mode 入参）

以下接口的搬运函数原型中**没有**任何形式的 L2 Cache 管理策略入参，属于例外：

### （一）cube_data_move 目录

| # | 接口 | 说明 | 文档 |
|---|------|------|------|
| 1 | `asc_copy_gm2l0a` | 从 GM 搬出到 L0A，无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l0a.md) |
| 2 | `asc_copy_gm2l0b` | 从 GM 搬出到 L0B，无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l0b.md) |
| 3 | `asc_copy_gm2l1`（2201 架构） | 2201 架构无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md) |
| 4 | `asc_copy_gm2l1`（3510 填充模式） | 3510 架构「填充模式」无 L2 Cache 入参（「2D搬运模式」有） | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md) |
| 5 | `asc_copy_gm2l1_nd2nz`（2201 架构） | 2201 架构无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_2201.md) |
| 6 | `asc_copy_l0c2gm`（2201 架构） | 2201 架构搬入 GM，无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_2201.md) |
| 7 | `asc_copy_l12gm` | 从 L1 搬入 GM，无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/cube_data_move/asc_copy_l12gm.md) |

### （二）vector_data_move 目录

| # | 接口 | 说明 | 文档 |
|---|------|------|------|
| 8 | `asc_copy_gm2ub`（3510 / 2201） | 两个架构均无 L2 Cache 入参 | [3510 跳转](docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md) / [2201 跳转](docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_gm2ub/asc_copy_gm2ub_arch_2201.md) |
| 9 | `asc_copy_gm2ub_align`（2201 架构） | 2201 架构无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_2201.md) |
| 10 | `asc_copy_ub2gm`（3510 / 2201） | 两个架构均无 L2 Cache 入参 | [3510 跳转](docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md) / [2201 跳转](docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_ub2gm/asc_copy_ub2gm_arch_2201.md) |
| 11 | `asc_copy_ub2gm_align`（2201 架构） | 2201 架构无 L2 Cache 入参 | [跳转](docs/zh/api/SIMD-API/c_api/vector_data_move/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_2201.md) |

## 四、结论与补充说明

### 核心结论

1. **并非所有「从 GM 搬出 / 搬入 GM」的搬运接口都有 L2 Cache Mode 入参**，存在明显例外。

2. **例外集中在两类**：
   - **L0 级搬运**：`asc_copy_gm2l0a`、`asc_copy_gm2l0b`（直接搬到 L0A/L0B 计算缓存，不经过 L2 Cache，故无该入参）。
   - **老架构（2201 = Atlas A2/A3）**：`asc_copy_gm2l1`、`asc_copy_gm2l1_nd2nz`、`asc_copy_l0c2gm`、`asc_copy_gm2ub_align`、`asc_copy_ub2gm_align` 这几个接口的 2201 架构文档均无 L2 Cache 入参，而 3510（Ascend 950）架构文档有——说明该参数是 950 系列新增。

3. **`asc_copy_gm2ub`、`asc_copy_ub2gm`、`asc_copy_l12gm`**：无论哪个架构都无 L2 Cache 入参。

### 参数命名与类型差异

- 严格命名为 `l2_cache_ctl` 的接口：`asc_copy_gm2l1_align`、`asc_copy_gm2l1_dn2nz`、`asc_copy_gm2l1_nd2nz`（3510）、`asc_copy_gm2l1`（3510 2D模式）。
- 命名为 `l2_cache_mode` 的接口：`asc_copy_l0c2gm`（3510）、`asc_copy_gm2ub_align`（3510）、`asc_copy_ub2gm_align`（3510）。
- 命名为 `cache_mode` 的接口：`asc_ndim_copy_gm2ub`（废弃版本）。
- 枚举类型入参：`asc_copy_gm2ub_align` 使用 `asc_load_l2_cache_mode`，`asc_copy_ub2gm_align` 使用 `asc_store_l2_cache_mode`。

### 说明

- 顶层 `.md`（如 `asc_copy_gm2l1/asc_copy_gm2l1.md`）多为分架构索引文件，真正的函数原型在各 `*_arch_3510.md` / `*_arch_2201.md` 中，本分析以分架构文档为准。
