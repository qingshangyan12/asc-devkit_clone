# RFC：将 Ascend C C-API（capi）适配到 M510 系列（npu_arch_5102）

- **状态**：草案（待评审）
- **范围**：`impl/c_api/`（SIMD C-API，即 capi）对 M510 芯片系列的适配，复用 3510 代码
- **作者**：asc-devkit 架构评估
- **关联**：basic_api `dav_m510`（已支持）、编译工程 `mc62cm12a.py`、capi `npu_arch_3510` / `npu_arch_9201`

---

## 1. 背景与问题

capi（`impl/c_api/`）当前仅通过编译期宏 `__NPU_ARCH__` 支持两类架构：

| 架构 | capi 目录 | `__NPU_ARCH__` | 备注 |
|------|-----------|----------------|------|
| Ascend950PR / A2/A3/A5（dav3510） | `reg_base_impl/npu_arch_3510/` | `3510` | cube/vector **分离**，独立 scalar 单元（c310） |
| Ascend910B/950（c220） | `reg_base_impl/npu_arch_9201/` | `9201` | MIX 架构 |

**M510 系列当前不被 capi 支持**。但 M510 的 basic_api 早已落地（`impl/basic_api/dav_m510/`、`impl/basic_api/reg_compute/dav_m510/`），编译工程也已识别 M510（`tools/build/asc_op_compile_base/asc_op_compiler/config/mc62cm12a.py`，`chip_version="m510"`，`PRE_COMPILE_KERNEL_MIX_COUPLING`，`__NPU_ARCH__ == 5102`）。即：**3510 与 M510 的架构差异，已经在编译工程与 basic_api 中完整体现**。

已知架构差异（用户给定 + 代码实证）：

- **3510**：cube/vector 分离；独立 scalar 单元（c310，见 `impl/basic_api/dav_3510/kernel_tpipe_impl_c310*.h`）；拥有专属 `expdiff`（Vexpdif）fp16 指令。
- **M510**：cube/vector **耦合**（`PRE_COMPILE_KERNEL_MIX_COUPLING`，核心宏 `__DAV_M200__` / `__DAV_M200_VEC__`）；scalar 单元走通用路径（`dav_m510/` 下无 tpipe/scalar 专属文件）。
- **vector 侧差距很小**：基础_api 中 M510 的 vector impl 多数可直接复用 3510；唯一明确的 vector 差异是 M510 缺乏 `expdiff`/`VexpdifFloat` 这条 fp16 专用指令（`impl/simt_api/cpp/dav_3510/kernel_simt_common_impl.h` 与 capi 3510 测试 `tests/api/c_api/npu_arch_3510/vector_compute/test_return_vconv.cpp` 实证）。
- **cube 侧差距很大**：`impl/basic_api/dav_m510/kernel_operator_mm_impl.h` 已对 M510 将多条 cube 路径桩化/报错（`LoadData2DL12L0ACal` 直接 `ASSERT(false)`；GM→L0A、L1→L0A transpose、InitL0A 等路径返回"not supported on current device"）。

**问题**：能否基于 3510 的 capi 代码，搞出一套适配 M510 的 capi？如果可行，产出 RFC，并为该芯片 capi 准备一套测试用例。

---

## 2. 可行性结论

**结论：可行，且复用率高。建议推进。**

支撑理由（均来自本仓库代码实证）：

1. **分发机制完全一致**：capi 的架构选择是 100% 编译期行为，靠 `__NPU_ARCH__` 宏 + 通用头内的 `#if / #elif / #include` 链，与已落地的 3510 / 9201 机制完全相同。新增 M510 分支是机械操作，无需改 CMake 编译逻辑（`impl/c_api/CMakeLists.txt` 仅 `install(DIRECTORY)`，新增文件自动纳入）。
2. **`memory_base_impl/` 完全架构无关**：25 个文件，已确认零 `__NPU_ARCH__` 引用 → **零改动**。
3. **大量 reg/vector/scalar 类实现可直接复用 3510**：`npu_arch_3510/` 下 20 个文件中的 vector / reg / scalar / sys_var / sync / spr / ub_scatter / loadalign / storealign 等，与 M510 行为一致，复制后基本原样保留即可。
4. **唯一明确的 vector 差异（expdiff）范围可控**：仅 `asc_exp_sub*` 系列依赖 Vexpdif 专用指令，可集中决策（见 §4.5）。
5. **cube 差异已有 basic_api 基准**：M510 不支持的 cube 路径在 `dav_m510/kernel_operator_mm_impl.h` 中已有明确 ASSERT/报错实现，可作为 capi `npu_arch_5102/cube_*` 的裁剪依据。

**主要风险**（详见 §6，均可在 Phase 0/1 化解）：通用层"总是编译"的 additive-only 机制无法用 arch 文件"覆盖"已存在的通用函数体；测试实际运行依赖外部 `tikicpulib::ascendm510` CPU 模型库（当前不存在）。

---

## 3. 架构分发机制（现状，代码实证）

**通用头示例** `impl/c_api/reg_base_impl/cube_compute_intf_impl.h`：

```c
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/npu_arch_3510/cube_compute_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/npu_arch_9201/cube_compute_intf_impl.h"
#endif
```

**公共门控** `include/c_api/asc_simd.h:26`：

```c
#if !defined(__NPU_ARCH__) || (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201))
#include "c_api/asc_reg_vector.h"
#endif
```

要点：

- `__NPU_ARCH__` 由 Ascend C 编译器（ccec）按目标 `--cce-aicore-arch` 预定义，仓库只"消费"，不"定义"。
- 分发是 **宏 + `#if/#elif/#include`**，不是 include 路径优先级、不是转发头、也不是运行时分发。
- **additive-only 约束**：arch 文件是"增量包含"（通常新增不同名的函数 / 新重载），**不能重新定义**通用头里已经编译的函数体。若某通用函数需要对 M510 改变行为，必须（a）让通用函数体自身按 `__NPU_ARCH__` 分支，或（b）改为不同名/新重载。

---

## 4. 复用策略（与 3510 的关系）

### 4.1 分层

| 层 | 路径 | M510 处理 |
|----|------|-----------|
| 架构无关实现 | `impl/c_api/memory_base_impl/`（25 文件） | **不改** |
| 通用 reg_base 头（23 个无 arch override 的，如 `reg_compare`、`reg_mask`、`scalar_bit`、`vector_sort` 等） | `impl/c_api/reg_base_impl/*.h` | 审计后，将 `5102` 加入已有 `__NPU_ARCH__` 分支；行为一致的无需改动；行为需不同的，在该头内按 `#elif 5102` 分支或下沉到 arch 文件 |
| 3510 架构专属实现 | `impl/c_api/reg_base_impl/npu_arch_3510/`（20 文件） | 新建 `npu_arch_5102/`，按"复用 / 裁剪 / 删除"三分类处理（见 §4.2） |
| 公共门控 | `include/c_api/asc_simd.h`、`asc_reg_vector.h` 等 | 条件中加入 `|| (__NPU_ARCH__ == 5102)` |

### 4.2 `npu_arch_3510/` 20 个文件的复用分类

| 分类 | 文件 | M510 处理 |
|------|------|-----------|
| **直接复用**（行为一致） | `vector_compute_composite_intf_impl.h`、`vector_datamove_intf_impl.h`、`reg_convert_intf_impl.h`、`reg_fused_intf_impl.h`、`reg_logic_intf_impl.h`、`reg_permute_sel_intf_impl.h`、`reg_addr_reg_intf_impl.h`、`scalar_atomic_intf_impl.h`、`spr_intf_impl.h`、`sync_intf_impl.h`、`sys_var_intf_impl.h`、`ub_scatter_intf_impl.h`、`loadalign_intf_impl.h`、`storealign_intf_impl.h`、`storeunalign_intf_impl.h` | 复制到 `npu_arch_5102/`，验证指令集一致后原样保留 |
| **需裁剪/改造**（cube 耦合差异） | `cube_compute_intf_impl.h`、`cube_compute_composite_intf_impl.h`、`cube_datamove_intf_impl.h`、`cube_datamove_composite_intf_impl.h`、`datamove_atomic_intf_impl.h` | 保留 M510 支持的共享算子（`asc_mmad`/`mad`、`fixpipe`、`l0c2gm` 等）；**删除或桩化** M510 不支持路径（GM→L0A、L1→L0A transpose、InitL0A、v1 LoadData），以 `impl/basic_api/dav_m510/kernel_operator_mm_impl.h` 的 ASSERT/报错为基准 |
| **3510 专属、首期不纳入** | `cube_compute_intf_impl.h` 中的 `asc_mmad_mx*` 系列（fp4 MX） | M510 是否支持待确认（见 §6），首期不在复用范围 |

> 可选优化（二期）：将 vector_* / reg_* 的共享实现抽到 `npu_arch_common/` 或保留在通用层，避免 3510 与 5102 双份维护。首期以"复制复用"换取落地速度。

### 4.3 通用头审计（Phase 0 必做）

23 个无 arch override 的通用头中，部分可能内含 `3510/9201` 专属 `#if` 而遗漏 M510。需逐文件核对并补 `#elif __NPU_ARCH__ == 5102` 分支，避免 M510 编译时落到错误/空实现。重点排查：`cube_compute_intf_impl.h`、`cube_datamove_intf_impl.h`、`vector_datamove_intf_impl.h` 等含 arch 分发的头。

### 4.4 公共门控扩展

```c
// include/c_api/asc_simd.h
#if !defined(__NPU_ARCH__) || (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 5102))
#include "c_api/asc_reg_vector.h"
#endif
```

### 4.5 expdiff / VexpdifFloat 决策（vector 侧唯一差异）

M510 无 `expdiff`（Vexpdif）fp16 专用指令。三个候选策略：

1. **不暴露**：M510 不提供 `asc_exp_sub*` 系列（最省事，但功能缺失）。
2. **软件/通用 Exp 组合模拟**：用通用 `Exp` 组合实现语义等价（需与架构团队确认精度/语义是否可接受）。
3. **编译期报错 + 清晰提示**：保留 API 名，M510 下用 `static_assert`/删除声明，让调用方编译期即知不支持。

**建议**：首期采用策略 1 或 3（明确"不支持"优于静默错误），并在文档中标注；后续若确有需求再评估策略 2。该决策直接影响 `vector_compute/test_return_vconv.cpp` 中 2 个 expdiff 用例在 M510 的归属（见测试用例 README）。

### 4.6 命名约定

basic_api 用 `dav_m510`（字符串名），capi 用 `npu_arch_<id>`（数字名，如 `npu_arch_3510`/`npu_arch_9201`）。**建议 capi 侧沿用 `npu_arch_5102`**（与现有数字约定一致），对外文档统一称"M510（npu_arch_5102）"。

---

## 5. 构建与测试接入

- **impl 侧**：`impl/c_api/CMakeLists.txt` 仅 install，新增 `npu_arch_5102/` 被 `install(DIRECTORY)` 自动纳入，**无需改动 CMake**。
- **测试侧**：`tests/api/c_api/CMakeLists.txt` 需新增产品类型（patch 见 §7）。关键：
  - `ascendm510_AIC` / `ascendm510_AIV`，对应编译定义 `__NPU_ARCH__=5102;__DAV_M200__;__DAV_M200_VEC__;__DAV_C310__`（耦合架构，cube/vector 仍由 `__DAV_CUBE__`/`__DAV_VEC__` 区分，沿用现有 AIC/AIV 双目标）。
  - 依赖 `tikicpulib::ascendm510`（外部 CPU 模型库，需先就绪）。
- **测试目录**：`tests/api/c_api/npu_arch_5102/`，结构与 `npu_arch_3510/` 对齐（`atomic`、`cube_compute`、`cube_datamove`、`cache_ctrl`、`misc`、`scalar_compute`、`sync`、`sys_var`、`vector_compute`、`vector_datamove`、`reg_compute`）。
- 测试风格与本仓库一致：gtest + mockcpp（`MOCKER`/`MOCKER_CPP` 桩底层指令并断言被调用；`g_coreType` 切 AIC/AIV；`static_assert` 校验返回类型）。大多数 M510 用例是 **3510 用例的直接克隆**（证明复用），差异用例单独标注。

---

## 6. 风险与待确认项（Open Questions）

- [ ] 编译器是否对 M510 预定义 `__NPU_ARCH__ == 5102`（以及 5101/5161/5163/5165 兄弟族）？需编译器团队确认宏值。
- [ ] `tikicpulib::ascendm510` CPU 模型库就绪时间（**阻塞测试实际运行**）。
- [ ] cube 耦合下，3510 中哪些 `asc_*` 在 M510 必须删除/桩化（以 `dav_m510` 为基准核对全量清单，产出最终裁剪表）。
- [ ] expdiff 策略（§4.5）最终拍板。
- [ ] 3510 专属 fp4 MX（`asc_mmad_mx*`）M510 支持范围。
- [ ] 命名：`npu_arch_5102` vs `npu_arch_m510`（建议前者）。
- [ ] adv_api（`impl/adv_api/detail/.../*_3510_impl.h`）是否需同步 M510 适配（**本 RFC 范围外**，作为后续）。

---

## 7. 实施计划（分阶段）

- **Phase 0 审计**：① 列出 23 个通用头中需补 `5102` 分支的文件；② 完成 §4.2 复用/裁剪清单；③ 与编译器团队确认 `__NPU_ARCH__` 值。
- **Phase 1 骨架**：新增 `reg_base_impl/npu_arch_5102/`；复制 3510 的 vector/reg/scalar 文件（直接复用）；扩展公共门控与通用头分支。
- **Phase 2 cube**：基于 `dav_m510/kernel_operator_mm_impl.h` 实现 `npu_arch_5102/cube_*`，删除/桩化不支持路径。
- **Phase 3 expdiff**：按 §4.5 决策落地。
- **Phase 4 测试**：落地 `npu_arch_5102/` 测试（克隆 3510 共享用例 + M510 差异用例），接入 CMake（依赖 tikicpulib）。
- **Phase 5 文档**：capi 文档补充 M510 支持与差异说明。

### 7.1 测试 CMake 接入 patch（落 Phase 4，依赖外部库就绪）

```cmake
# tests/api/c_api/CMakeLists.txt —— 在 ASCENDC_UT_PRODUCT_TYPE_LIST 追加：
set(ASCENDC_UT_PRODUCT_TYPE_LIST
    ...
    ascendm510_AIC
    ascendm510_AIV
)

# 在已有 GLOB 块后追加：
file(GLOB ASCENDC_UT_ascendm510_AIC_CASE_SRC
    npu_arch_5102/cube_compute/*.cpp
    npu_arch_5102/cube_datamove/*.cpp
    npu_arch_5102/sys_var/test_asc_get_pipe_idle_slot_count.cpp
)
file(GLOB ASCENDC_UT_ascendm510_AIV_CASE_SRC
    npu_arch_5102/atomic/*.cpp
    npu_arch_5102/cache_ctrl/*.cpp
    npu_arch_5102/misc/*.cpp
    npu_arch_5102/scalar_compute/*.cpp
    npu_arch_5102/sync/*.cpp
    npu_arch_5102/sys_var/*.cpp
    npu_arch_5102/vector_compute/*.cpp
    npu_arch_5102/vector_datamove/*.cpp
)

# 在 foreach 的 target_compile_definitions 中追加：
$<$<STREQUAL:${product_type},ascendm510_AIC>:__NPU_ARCH__=5102;__DAV_M200__;__DAV_M200_VEC__;__DAV_CUBE__>
$<$<STREQUAL:${product_type},ascendm510_AIV>:__NPU_ARCH__=5102;__DAV_M200__;__DAV_M200_VEC__;__DAV_VEC__>

# 在 target_link_libraries 中追加：
$<$<STREQUAL:${product_type},ascendm510_AIC>:tikicpulib::ascendm510>
$<$<STREQUAL:${product_type},ascendm510_AIV>:tikicpulib::ascendm510>
```

---

## 8. 附录：测试矩阵（npu_arch_5102 与 3510 对应）

完整矩阵见 `tests/api/c_api/npu_arch_5102/README.md`。状态说明：

- **Reuse**：直接克隆 3510 用例（行为一致，证明复用）。
- **Adapt**：需按 M510 差异改写（主要是 cube 耦合路径、expdiff 策略）。
- **Drop**：3510 专属、M510 首期不提供（如 fp4 MX）。

| 类别 | 3510 用例示例 | M510 状态 |
|------|---------------|-----------|
| atomic | `test_set_atomic_add_int16.cpp` 等 | Reuse |
| sync | `test_asc_sync_block_wait.cpp` 等 | Reuse |
| sys_var | `test_get_block_idx.cpp` 等 | Reuse |
| scalar_compute | `test_asc_popc.cpp`、`test_asc_atomic_*.cpp` 等 | Reuse |
| cache_ctrl | `test_dci.cpp`、`test_icache_preload.cpp` 等 | Reuse（需核对指令） |
| cube_compute | `test_mmad.cpp`、`test_mmad_bias.cpp` | Reuse（共享 mmad 子集） |
| cube_compute | `test_mmad_mx.cpp`、`test_mmad_mx_bias.cpp`、`test_mmad_mx_sync.cpp` | Drop（fp4 MX，首期不支持） |
| cube_datamove | `test_asc_copy_gm2l0a.cpp`、`test_copy_l12l0a_mx.cpp` 等 | Adapt（GM→L0A / L1→L0A transpose / InitL0A 等路径 M510 不支持） |
| vector_compute | `test_return_vconv.cpp`（vconv 重载） | Reuse |
| vector_compute | `test_return_vconv.cpp`（expdiff 两个用例） | Adapt/Drop（按 §4.5） |
| vector_datamove | `test_*` | Reuse |
| misc | `test_asc_init.cpp` 等 | Reuse |
| reg_compute | `reg_*` 系列 | Reuse（reg 类实现共享） |
