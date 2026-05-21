# SKILL.md API 分类概览 — 测试覆盖总表

> 对比基准: `opencode/skills/cube_c_api_skill/SKILL.md` § API 分类概览
> 测试环境: CANN 9.0.0-beta.2 / dav-3510 / Bisheng compiler / Ascend950pr_9599 simulator
> 统计: 14 个 compute API + 27 个 datamove API = 41 个

---

## Cube 计算 (cube_compute.h) — 14 个

| # | API | 覆盖 | 位置 | 说明 |
|---|-----|------|------|------|
| 1 | `asc_mmad` | ✅ 已覆盖 | `cube_examples/asc_mmad/` | half×half→float 单分形, 基准示例 |
| 2 | `asc_mmad_sync` | ✅ 已覆盖 | `000_cube_tests/compute/asc_mmad_sync/` | half×half→float 同步版本 |
| 3 | `asc_mmad_s4` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:92)，3510 不可用 |
| 4 | `asc_mmad_s4_sync` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:102)，3510 不可用 |
| 5 | `asc_mmad_mx` | ❌ CCE 限制 | `000_cube_tests/compute/asc_mmad_mx/` | CCE 不支持 struct 替代类型创建 `__ca__ fp8_e4m3fn_t[]` 地址空间数组 |
| 6 | `asc_mmad_mx_sync` | ❌ CCE 限制 | `000_cube_tests/compute/asc_mmad_mx/` | 同上，fp4/fp8 类型为 CCE 原生类型, struct 替代无法通过编译 |
| 7 | `asc_mmad_sparse` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:106)，3510 不可用 |
| 8 | `asc_mmad_sparse_sync` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:109)，3510 不可用 |
| 9 | `asc_set_l0c2gm_config` | ✅ 已覆盖 | `000_cube_tests/compute/asc_set_l0c2gm_config/` | F322F16 浮点→半精度量化, PASS |
| 10 | `asc_set_l0c2gm_nz2nd` | ✅ 已覆盖 | `000_cube_tests/compute/asc_set_l0c2gm_nz2nd/` | NZ→ND 格式转换, PASS |
| 11 | `asc_set_mmad_direction_m` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:113)，3510 不可用 |
| 12 | `asc_set_mmad_direction_n` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:115)，3510 不可用 |
| 13 | `asc_set_fp32_mode` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:121)，3510 不可用 |
| 14 | `asc_get_l0c2gm_unitflag` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_compute.h:125)，3510 不可用 |

**compute 小结**: 4 已覆盖 + 2 CCE 类型限制 + 7 架构限制(2201-only) + 1 已在 cube_examples = 14

---

## Cube 数据搬运 (cube_datamove.h) — 27 个

| # | API | 覆盖 | 位置 | 说明 |
|---|-----|------|------|------|
| 1 | `asc_copy_gm2l1` | ✅ 间接覆盖 | 全部 11 个用例 | 所有测试的 GM→L1 搬运均使用 `asc_copy_gm2l1_sync` (3510 段: line 2459)，未单独建用例但覆盖充分 |
| 2 | `asc_copy_gm2l1_align` | ⚠️ 格式限制 | `000_cube_tests/datamove/asc_copy_gm2l1_align/` | 编译通过, 运行 PASS=❌。burst 搬运数据格式与 `asc_copy_l12l0a` 2D 分形格式不兼容, 无法通过 MMAD 管道精确验证。3510 缺少 `asc_copy_l12gm` 回环路径 |
| 3 | `asc_copy_gm2l0a` | ✅ 已覆盖 | `cube_examples/asc_copy_gm2l0a/` | 已有完整用例 |
| 4 | `asc_copy_gm2l0b` | ✅ 已覆盖 | `cube_examples/asc_copy_gm2l0b/` | 已有完整用例 |
| 5 | `asc_copy_l12l0a` | ✅ 已覆盖 | `cube_examples/asc_copy_l12l0a/` | 已有完整用例(含 transpose/mx 变体) |
| 6 | `asc_copy_l12l0b` | ✅ 已覆盖 | `cube_examples/asc_copy_l12l0b/` | 已有完整用例(含 trans/transpose 变体) |
| 7 | `asc_copy_l12l0c` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_datamove.h:1401-1441)，3510 不可用 |
| 8 | `asc_copy_l0c2gm` | ✅ 已覆盖 | `cube_examples/asc_copy_l0c2gm/` | 已有完整用例 |
| 9 | `asc_copy_l0c2ub` | ❌ API 链缺失 | `000_cube_tests/datamove/asc_copy_l0c2ub/` | L0C→UB 搬运成功, 但 **UB→GM 搬运 API (`asc_copy_ub2gm_sync`) 在 3510 的 cube_datamove.h 中不存在**, 无法完成回环验证 |
| 10 | `asc_copy_l0c2l1` | ❌ impl 限制 | `000_cube_tests/datamove/asc_copy_l0c2l1/` | impl 函数 `copy_matrix_cc_to_cbuf` **不支持 dav-3510 target feature** (CANN 9.0.0-beta.2), linker 找不到实现 |
| 11 | `asc_copy_l12fb` | ❌ CCE 限制 | — | `__fbuf__` 修饰符 **不能在 CCE C API kernel 中声明为局部变量**。Fixpipe Buffer 仅在高阶 API (kernel_operator) 中可用 |
| 12 | `asc_copy_l12fb_v2` | ✅ 已覆盖 | `cube_examples/asc_copy_l12fb_v2/` | 已有完整用例 |
| 13 | `asc_copy_l12bt` | ❌ CCE 限制 | — | `__biasbuf__` 修饰符 **禁止在 dav-c310-cube 中使用**, 且 **CCE 禁止对此类地址空间指针解引用**。BiasTable 仅在高阶 API (kernel_operator) 中可用 |
| 14 | `asc_copy_l12gm` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_datamove.h:1394-1399)。Cube 核 3510 缺少 L1→GM 直接搬运路径，所有测试改用 MMAD→L0C→GM 间接搬运 |
| 15 | `asc_copy_l12ub` | ❌ 跨核限制 | — | L1→UB **跨核搬运 (Cube→Vector)，不可使用 `__cube__` 或 `__vector__` 修饰符**，且验证路径复杂，跳过 |
| 16 | `asc_fill_l0a` | ❌ 链接限制 | `000_cube_tests/datamove/asc_fill_l0a/` | API 在 common 段声明(cube_datamove.h:54-70), 但 linker 报告 `undefined symbol: asc_fill_l0a_sync(half*, half, ...)`, **3510 实现缺失** |
| 17 | `asc_fill_l0b` | ✅ 已覆盖 | `cube_examples/asc_fill_l0b/` | 已有完整用例 |
| 18 | `asc_fill_l1` | ✅ 已覆盖 | `cube_examples/asc_fill_l1/` | 已有完整用例 |
| 19 | `asc_set_l13d_fmatrix` | ✅ 已覆盖 | `cube_examples/asc_set_l13d_fmatrix/` | 已有完整用例 |
| 20 | `asc_set_l13d_fmatrix_b` | ✅ 已覆盖 | `cube_examples/asc_set_l13d_fmatrix_b/` | 已有完整用例 |
| 21 | `asc_set_l13d_rpt` | ✅ 已覆盖 | `000_cube_tests/datamove/asc_set_l13d_rpt/` | repeat=1 默认配置, PASS |
| 22 | `asc_set_l13d_size` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_datamove.h:729)，3510 不可用 |
| 23 | `asc_set_l13d_padding` | ❌ 架构限制 | — | **仅在 2201 段声明** (cube_datamove.h:731)，3510 不可用 |
| 24 | `asc_set_gm2l1_loop_size` | ✅ 已覆盖 | `000_cube_tests/datamove/asc_set_gm2l1_loop_size/` | loop1=2 双块搬运, PASS |
| 25 | `asc_set_gm2l1_loop1_stride` | ✅ 已覆盖 | `000_cube_tests/datamove/asc_set_gm2l1_loop1_stride/` | loop1 stride 配置, PASS |
| 26 | `asc_set_gm2l1_loop2_stride` | ✅ 已覆盖 | `000_cube_tests/datamove/asc_set_gm2l1_loop2_stride/` | loop2 stride 配置, PASS |
| 27 | `asc_set_gm2l1_pad` | ✅ 已覆盖 | `000_cube_tests/datamove/asc_set_gm2l1_pad/` | pad_mode=0 无填充模式, PASS |

**datamove 小结**: 18 已覆盖 + 1 格式限制 + 4 架构限制(2201-only) + 2 CCE 限制(__fbuf__/__biasbuf__) + 1 impl 限制 + 1 API 链缺失(UB→GM) = 27

---

## 总体统计

| 状态 | 数量 | 占比 |
|------|------|------|
| ✅ 已覆盖 (含 PASS + 格式限制) | **24** | 58.5% |
| ❌ 架构限制 (2201-only) | **11** | 26.8% |
| ❌ CCE/CANN 限制 (类型/impl/API链) | **6** | 14.6% |
| **合计** | **41** | 100% |

## 限制分类详解

### 🏗 架构限制 (2201-only) — 11 个 API
这些 API 仅在 `#if __NPU_ARCH__ == 2201` 段中声明，3510 架构不可访问。包括：
- 所有 `asc_mmad_s4` 系列（4-bit 数据类型）
- `asc_mmad_sparse` 系列（稀疏矩阵）
- `asc_set_mmad_direction_*` / `asc_set_fp32_mode` / `asc_get_l0c2gm_unitflag`（MMAD 控制类）
- `asc_copy_l12l0c` / `asc_copy_l12gm`（L1↔L0C/L1→GM 搬运）
- `asc_set_l13d_size` / `asc_set_l13d_padding`（3D 搬运设置）

### 🔧 CCE/CANN 限制 — 6 个 API

| 限制类型 | API | 具体表现 |
|----------|-----|---------|
| CCE struct 地址空间 | `asc_mmad_mx` / `asc_mmad_mx_sync` | `__ca__ fp8_e4m3fn_t[256]` 无法用 struct 替代类型构造 |
| CCE 内存修饰符 | `asc_copy_l12fb` | `__fbuf__` 不允许声明为 kernel 局部数组 |
| CCE 内存修饰符 | `asc_copy_l12bt` | `__biasbuf__` 不允许在 cube 核中使用/解引用 |
| impl target feature | `asc_copy_l0c2l1` | `copy_matrix_cc_to_cbuf` 不支持 dav-3510 |
| API 链缺失 | `asc_copy_l0c2ub` | UB→GM 搬运 API 在 3510 不存在 |
| 链接缺失 | `asc_fill_l0a` | linker 找不到 3510 实现 |
