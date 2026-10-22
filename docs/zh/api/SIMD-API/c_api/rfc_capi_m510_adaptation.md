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
- **vector 侧差距（原判断有误，已按 §4.10 修正）**：基础_api 中 M510 的 vector impl 多数可直接复用 3510。早期曾判断「M510 缺 expdiff/fp4/fp8/s4 一整类数据类型专用指令」（`UNPK4_B8`、`vsel`/`vintlv`/`vselr`/`vstar`、`vcvt_s42*`、`vexpdif`），并据此在共享通用头加了大规模「排除仅 5102」守卫。**该判断后被 `impl/basic_api/reg_compute/dav_m510/` 的实证推翻**——这些指令 M510 均实现，仅个别类型组合不同（如 expdiff 仅支持 `Tuple<half,half>`/`Tuple<float,float>`）。详见 **§4.10「前提修正与守卫收缩」**：守卫已从 1114 个收缩至 83 个，非 cube 域分发链已补齐。
- **cube 侧差距很大**：`impl/basic_api/dav_m510/kernel_operator_mm_impl.h` 已对 M510 将多条 cube 路径桩化/报错（`LoadData2DL12L0ACal` 直接 `ASSERT(false)`；GM→L0A、L1→L0A transpose、InitL0A 等路径返回"not supported on current device"）。

**问题**：能否基于 3510 的 capi 代码，搞出一套适配 M510 的 capi？如果可行，产出 RFC，并为该芯片 capi 准备一套测试用例。

---

## 2. 可行性结论

**结论：可行，且复用率高。建议推进。**

支撑理由（均来自本仓库代码实证）：

1. **分发机制完全一致**：capi 的架构选择是 100% 编译期行为，靠 `__NPU_ARCH__` 宏 + 通用头内的 `#if / #elif / #include` 链，与已落地的 3510 / 9201 机制完全相同。新增 M510 分支是机械操作，无需改 CMake 编译逻辑（`impl/c_api/CMakeLists.txt` 仅 `install(DIRECTORY)`，新增文件自动纳入）。
2. **`memory_base_impl/` 基本架构无关**：25 个文件整体无 `__NPU_ARCH__` 分支；其中 `cube_datamove_intf_impl.h` 的 L1→L0 / GM→L0 加载函数按 §4.8 加「排除仅 5102」守卫（其余零改动）。
3. **大量 reg/vector/scalar 类实现可直接复用 3510**：`npu_arch_3510/` 下 20 个文件中的 vector / reg / scalar / sys_var / sync / spr / ub_scatter / loadalign / storealign 等，与 M510 行为一致，复制后基本原样保留即可。
4. **数据类型专用指令（expdiff / fp4 / fp8 / s4）M510 实际支持**：依赖 `vexpdif`、`UNPK4_B8`、`vsel`/`vintlv`、`vcvt_s42*` 等指令的函数，绝大多数定义在**共享通用头**而非 `npu_arch_3510/` 内。早期据「M510 不支持」加过大范围守卫，后经 `dav_m510` basic_api 逐指令核对确认**均支持**（唯一例外：expdiff 的 `Tuple<float,half>` 组合），守卫已按 §4.10 收缩至 83 个。
5. **cube 差异已有 basic_api 基准**：M510 不支持的 cube 路径在 `dav_m510/kernel_operator_mm_impl.h` 中已有明确 ASSERT/报错实现，可作为 capi `npu_arch_5102/cube_*` 的裁剪依据（§4.8 据此保留 79 个守卫）。

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
| **直接复用（薄包含）**（行为一致） | `vector_compute_composite_intf_impl.h`、`vector_datamove_intf_impl.h`、`reg_convert_intf_impl.h`、`reg_fused_intf_impl.h`、`reg_logic_intf_impl.h`、`reg_permute_sel_intf_impl.h`、`reg_addr_reg_intf_impl.h`、`scalar_atomic_intf_impl.h`、`spr_intf_impl.h`、`sync_intf_impl.h`、`sys_var_intf_impl.h`、`ub_scatter_intf_impl.h`、`loadalign_intf_impl.h`、`storealign_intf_impl.h`、`storeunalign_intf_impl.h` | `npu_arch_5102/` 下对应文件**仅一行 `#include` 直接包含 `npu_arch_3510/` 同名文件**（薄包含），不复制函数体。指令集一致的，3510 实现在 5102 下原样可用。 |
| **需裁剪/改造**（cube 耦合差异） | `cube_compute_intf_impl.h`、`cube_compute_composite_intf_impl.h`、`cube_datamove_intf_impl.h`、`cube_datamove_composite_intf_impl.h`、`datamove_atomic_intf_impl.h` | 共享算子（`asc_mmad`/`mad`、`asc_copy_l0c2gm`/`l0c2l1`、`asc_copy_gm2l1*`、`asc_copy_l12l0c`、`asc_copy_l12gm` 等）保留；M510 不支持的 L1→L0 / GM→L0 加载路径（`asc_copy_gm2l0a`/`l0b`、`asc_copy_l12l0a`/`l0b`/`bt`/`fb`、`asc_load_image_to_cbuf` 等）在共享头内按 §4.8 加「排除仅 5102」守卫编译期剔除（对 3510/9201 零影响）；fp4 MX（`asc_mmad_mx*`）按 §4.5 首期 Drop。 |
| **3510 专属、首期不纳入** | `cube_compute_intf_impl.h` 中的 `asc_mmad_mx*` 系列（fp4 MX） | M510 是否支持待确认（见 §6），首期不在复用范围 |

> 可选优化（二期）：将 vector_* / reg_* 的共享实现抽到 `npu_arch_common/` 或保留在通用层，避免 3510 与 5102 双份维护。首期以「薄包含 + 通用头守卫」换取落地速度（薄包含零重复，守卫只在 10 个通用头里出现）。

### 4.3 通用头审计（Phase 0 必做）

23 个无 arch override 的通用头中，部分可能内含 `3510/9201` 专属 `#if` 而遗漏 M510。需逐文件核对并补 `#elif __NPU_ARCH__ == 5102` 分支，避免 M510 编译时落到错误/空实现。重点排查：`cube_compute_intf_impl.h`、`cube_datamove_intf_impl.h`、`vector_datamove_intf_impl.h` 等含 arch 分发的头。

### 4.4 公共门控扩展

```c
// include/c_api/asc_simd.h
#if !defined(__NPU_ARCH__) || (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 5102))
#include "c_api/asc_reg_vector.h"
#endif
```

### 4.5 数据类型专用指令缺口决策（expdiff / fp4 / fp8 / s4）—— **结论已修正，见 §4.10**

> ⚠️ **本节原结论已被推翻**。原判断「M510 缺 expdiff / fp4 / fp8 / s4 一整类数据类型专用指令」与仓库自身的 `impl/basic_api/reg_compute/dav_m510/` 实现矛盾。经逐指令核对，**这些指令 M510 均支持**，仅个别**类型组合**与 3510 不同。因此基于本节落地的大规模「排除仅 5102」守卫（§4.7，1114 个函数）已大部分回退，详见 **§4.10「前提修正与守卫收缩」**。本节保留作为历史记录。

（原文，存档）M510 缺一组数据类型专用指令（expdiff / fp4 / fp8 / s4）。

关键事实（原判断依据：ccec dav-510r2 实测 + 用户确认）：
- 这些指令的 capi 实现直接定义在共享通用头（reg_convert_intf_impl.h、loadalign_intf_impl.h、storealign_intf_impl.h、storeunalign_intf_impl.h、loadunalign_intf_impl.h、loadunalign_composite_intf_impl.h、storeunalign_composite_intf_impl.h、reg_fused_intf_impl.h、reg_permute_sel_intf_impl.h、reg_gather_intf_impl.h），不在 npu_arch_3510/ 里。
- 用户确认 M510 对这些数据类型确实不支持（非缺编译开关），故正确做法是编译期排除，而非模拟。

候选策略：

1. **不暴露（编译期排除）**：用守卫把相关函数从 5102 编出里拿掉。零运行时风险，但 M510 在这些数据类型上的 API 面比 3510 小。
2. **软件/通用指令组合模拟**：用通用指令拼出语义等价（精度/语义需架构团队确认）。首期不做。
3. **编译期报错 + 清晰提示**：保留 API 名，M510 下 static_assert/删声明，让调用方编译期即知不支持。

**原决策**：采用策略 1，统一用「`#if !defined(__NPU_ARCH__) || (__NPU_ARCH__ != 5102)`」守卫包裹这些函数（见 §4.7）。**该决策及其落地范围已按 §4.10 修正。**

### 4.6 命名约定

basic_api 用 `dav_m510`（字符串名），capi 用 `npu_arch_<id>`（数字名，如 `npu_arch_3510`/`npu_arch_9201`）。**建议 capi 侧沿用 `npu_arch_5102`**（与现有数字约定一致），对外文档统一称"M510（npu_arch_5102）"。

### 4.7 排除仅 5102 守卫策略（已在共享通用头落地）

决策 §4.5 的落地方式不是改 `npu_arch_3510/`，而是**在 10 个共享通用头里，对依赖专用指令的函数加编译期守卫**。原因：这些函数直接定义在通用头（对所有 arch 都编译），无法靠 arch 文件覆盖已存在的函数体（additive-only 约束，见 §3）。

守卫写法（每个被排除函数整体包一层）：

```
#if !defined(__NPU_ARCH__) || (__NPU_ARCH__ != 5102)
__simd_callee__ inline void asc_xxx(...) { ... }  // 使用 vexpdif / UNPK4_B8 / vsel / vcvt_s42* 等
#endif  // not M510 (3510-only instruction path, see RFC 4.5)
```

语义：`#if` 条件对 3510 / 9201 恒为真（宏未定义或值不等于 5102）→ 函数照常编出；仅当 `__NPU_ARCH__ == 5102` 时条件为假 → 函数从 5102 编出中消失。因此**对 3510/9201 零行为变化**，已用 ccec 双向验证（见 §7 状态）。

10 个共享通用头与原排除函数数（**其中 1031 个已按 §4.10 回退，仅 83 个保留**）：

| 共享通用头 | 原排除函数数 | 涉及指令 | 现状态（§4.10 后） |
|------|------|------|------|
| reg_convert_intf_impl.h | 475 | vcvt_s42*(s4)、vcvt_f162s4 | 守卫**已全部移除**（指令 M510 支持） |
| loadalign_intf_impl.h | 332 | vlds(..., UNPK4_B8) | 守卫**已全部移除**（M510 的 `CheckLoadDist` 显式支持 `DIST_UNPACK4_B8`） |
| storealign_intf_impl.h | 71 | vsts 等 | 守卫**已全部移除** |
| reg_permute_sel_intf_impl.h | 36 | vintlv 等 | 守卫**已全部移除**（`vintlv` 在 dav_m510 basic_api 中存在） |
| storeunalign_intf_impl.h | 42 | vstus / vstar 等 | 守卫**已全部移除**（`vstar` 在 dav_m510 `DataCopyUnAlignPostImpl` 中对 b8/b16/b32/b64 通用使用） |
| loadunalign_intf_impl.h | 35 | vldus / vldas 等 | 守卫**已全部移除** |
| loadunalign_composite_intf_impl.h | 14 | 复合非对齐加载 | 守卫**已全部移除** |
| storeunalign_composite_intf_impl.h | 14 | 复合非对齐存储 | 守卫**已全部移除** |
| reg_gather_intf_impl.h | 10 | vselr 等 | 守卫**已全部移除**（`vselr` 在 dav_m510 `GatherMaskImpl` 中对 u8/u16/u32 通用使用） |
| reg_fused_intf_impl.h | 6 | vexpdif | 移除 2 个（`asc_exp_sub(float,float)`，M510 支持 `Tuple<float,float>`）；**保留 4 个** `asc_exp_sub_half2float`（`Tuple<float,half>`，M510 不支持） |
| **合计** | **1035** | | **移除 1031，保留 4** |

> ⚠️ 本节的「排除后 M510 API 面比 3510 小」结论**仅对 expdiff 的 half→float 组合成立**；其余 fp4/fp8/s4 路径 M510 实际支持，相关函数已恢复编出（§4.10）。测试矩阵（§8）与测试 README 中相关行已从 Drop 改回 Reuse/Adapt。

注意：`npu_arch_5102/` 的 shim 文件为薄包含 3510 同名文件（见 §4.2）；在补齐顶层分发链的 5102 分支（§4.9 / §4.10）后，这些 shim 已被真正引用，M510 由此获得 3510 的架构专属实现。

### 4.8 cube_datamove「排除仅 5102」守卫（Phase 2，cube 耦合裁剪落地）

Phase 2 将 §4.7 的守卫机制应用到 **cube 数据搬运** 路径。依据 `impl/basic_api/dav_m510/kernel_operator_mm_impl.h`，M510 的 cube 数据搬运不支持 L1→L0A/L0B 的部分路径与 GM→L0A/L0B 直接加载（对应 3510 的 LoadData 路径在 `dav_m510` 中已被桩化为 `ASSERT(false)` / 返回 "not supported on current device"）。注意：`npu_arch_5102/` 的 shim 是薄包含 3510 同名文件，但**顶层分发头（`cube_datamove_intf_impl.h` 等）并无 5102 分支**，所以这些 L0 加载函数实际来自共享的 `memory_base_impl/cube_datamove_intf_impl.h` 与通用 `reg_base_impl/cube_datamove_intf_impl.h`，无法靠 arch 文件覆盖（additive-only 约束，见 §3）——因此同样采用「排除仅 5102」编译期守卫，从 5102 编出中剔除这些函数；对 3510/9201 恒真、零影响。

**被守卫排除的函数族（共 10 族，约 147 个定义）**：

| 函数族 | 含义 | 守卫定义点 |
|------|------|-----------|
| `asc_copy_gm2l0a` | GM→L0A | `memory_base_impl/cube_datamove_intf_impl.h`、`reg_base_impl/cube_datamove_intf_impl.h` |
| `asc_copy_gm2l0b` | GM→L0B | 同上 |
| `asc_copy_l12l0a` | L1→L0A | 同上 |
| `asc_copy_l12l0a_trans` | L1→L0A transpose | 同上 |
| `asc_copy_l12l0b` | L1→L0B | 同上 |
| `asc_copy_l12l0b_sparse` | L1→L0B sparse | 同上 |
| `asc_copy_l12l0b_trans` | L1→L0B transpose | 同上 |
| `asc_copy_l12bt` | L1→BT | 同上 |
| `asc_copy_l12fb` | L1→FB | 同上 |
| `asc_load_image_to_cbuf` | LoadImage 到 cbuf | 同上 |

**守卫落地位置与数量（ccec 双向验证：5102 全量 0 错误、9201 零回归）**：

| 文件 | 守卫数 | 说明 |
|------|--------|------|
| `impl/c_api/memory_base_impl/cube_datamove_intf_impl.h` | 66 | 共享架构无关头；L1→L0 / GM→L0 加载函数加守卫，其余零改动 |
| `impl/c_api/reg_base_impl/cube_datamove_intf_impl.h` | 79 | 通用分发头；同一批 10 族函数加守卫 |
| `impl/c_api/reg_base_impl/npu_arch_9201/cube_datamove_intf_impl.h` | 2 | 9201 专属路径；守卫恒真、保留 |
| `impl/c_api/reg_base_impl/npu_arch_3510/cube_datamove_intf_impl.h` | 0 | 3510 专属头不含这 10 族（定义落在共享/通用头），无需改动 |

**保留（不加守卫，M510 支持的共享 cube 算子）**：`asc_copy_l12l0c`（cbuf→L0C）、`asc_copy_gm2l1*`、`asc_copy_l0c2gm` / `l0c2l1` / `l0c2ub`、`asc_copy_l12gm`，以及 `asc_mmad` / `mad`（位于通用 `cube_compute_intf_impl.h`，M510 支持）。

**fp4 MX（§4.5 Drop）**：`asc_mmad_mx*` 系列已在 `npu_arch_3510/cube_compute_composite_intf_impl.h`（8 个定义）与 `cube_datamove_composite_intf_impl.h`（106 个定义）加同款守卫；`npu_arch_5102/cube_compute_intf_impl.h` 已清空（M510 无 fp4/fp8 MX matmul，见 §4.5）。

### 4.9 顶层分发链补 5102 分支（Phase 2 缺陷修复，实测发现）

**问题（ccec 预处理实测发现）**：仅靠 §4.8 的守卫**不足以让 M510 获得 cube 算子**。公共门控 `include/c_api/cube_compute/cube_compute.h`、`include/c_api/cube_datamove/cube_datamove.h` 把 `5102` 路由到**顶层通用头** `impl/c_api/reg_base_impl/cube_compute_intf_impl.h` / `cube_datamove_intf_impl.h`；但这两个顶层头原有的 `#if/#elif/#include` 链**只有 3510 与 9201 两条分支，缺 5102 分支**，于是 M510 只能拿到顶层头自身定义的通用函数（如 `asc_mmad`、`asc_copy_gm2l1`），而**所有架构专属 cube 算子都取不到**——包括 M510 实际**支持**的 `asc_copy_l0c2gm`（L0C→GM）、`asc_copy_l0c2l1`（L0C→L1）。

ccec `-E` 实测（pp 输出按「定义体」计数，声明不算）：

| 函数 | 修复前 5102 | 修复前 9201 | 修复后 5102 | 修复后 9201 |
|------|-----------|-----------|-----------|-----------|
| `asc_copy_l0c2gm` | **0** | 28 | **28** | 28 |
| `asc_copy_l0c2l1` | **0** | 25 | **27** | 25 |
| `asc_copy_gm2l1` | 48 | 64 | 48 | 64 |
| `asc_mmad` | 36 | 50 | 36 | 50 |

即：修复前 M510 下 `asc_copy_l0c2gm` / `l0c2l1` 只有声明、**无定义**（调用即编译失败），而它们属于 M510 应支持的算子。根因是 `npu_arch_5102/cube_*` shim 未被任何分发链引用（成了死代码）。

**修复（已落地）**：在顶层 cube 分发链补上 5102 分支，让 M510 走薄包含 3510 的 `npu_arch_5102/` shim：

```c
// impl/c_api/reg_base_impl/cube_{compute,datamove}_intf_impl.h
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/npu_arch_3510/cube_datamove_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9201)
#include "impl/c_api/reg_base_impl/npu_arch_9201/cube_datamove_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5102)
#include "impl/c_api/reg_base_impl/npu_arch_5102/cube_datamove_intf_impl.h"
#endif
```

**为什么这样安全**：`npu_arch_3510/cube_datamove_intf_impl.h` 中**不含**那 10 个不支持家族（实测 0 定义，它们落在共享/通用头，已由 §4.8 守卫剔除），因此 cube 域路由 5102 → `npu_arch_5102` → 3510 不会引入 M510 不支持的指令。实测确认：修复后 5102 的 `UNPK4_B8` / `vexpdif`（expdiff）/ `vcvt_s42bf16`（s4）均仍为 **0**，9201 保留全部指令（`vexpdif`×3、`vcvt_s42bf16`×9），双向 0 错误。

### 4.10 前提修正与守卫收缩（Phase 2，推翻 §4.5 原结论）

§4.9 排查过程中发现一个更根本的问题：**§4.5 关于「M510 缺 expdiff / fp4 / fp8 / s4 指令」的前提，与仓库自身的 `dav_m510` basic_api 实现矛盾**。

**核对方法**：`impl/basic_api/reg_compute/kernel_reg_compute_*_intf_impl.h` 明确以 `#elif __NPU_ARCH__ == 5102` 分发到 `impl/basic_api/reg_compute/dav_m510/`，因此该目录是 5102 的**活实现**（非死代码）。逐指令核对结果：

| 指令 | 原判断 | dav_m510 basic_api 实证 | 结论 |
|------|--------|------------------------|------|
| `vexpdif` | M510 缺 | `dav_m510/kernel_reg_compute_vec_fused_impl.h:105` 的 `FusedExpSubImpl` 直接调用 `vexpdif`；且 `#if __NPU_ARCH__ == 5102` 分支把支持类型设为 `Tuple<half,half>`/`Tuple<float,float>`（**扩展**而非限制） | **支持**（类型组合受限） |
| `vstar` | 归为 fp8 专属、缺 | `dav_m510/kernel_reg_compute_datacopy_store_impl.h:524` `DataCopyUnAlignPostImpl` 对 `b8/b16/b32/b64` 通用调用 `vstar`，无 5102 排除守卫 | **支持**（通用指令，非 fp8 专属） |
| `UNPK4_B8` | M510 缺 | `dav_m510/kernel_reg_compute_datacopy_load_impl.h:39` `CheckLoadDist` 的 `SupportEnum` 显式含 `LoadDist::DIST_UNPACK4_B8` | **支持** |
| `vcvt_s42bf16` / `vcvt_s42f16` | M510 缺 | `dav_m510/kernel_reg_compute_vec_vconv_impl.h:75/78` 对 `Tuple<half,int4x2_t>` / `Tuple<bfloat16_t,int4x2_t>` 调用 | **支持** |
| `vcvt_f162s4` | — | `dav_m510/kernel_reg_compute_vec_vconv_impl.h:207` 对 `Tuple<int4x2_t,half>` 调用 | **支持** |
| `vselr` | 归为 fp8 专属、缺 | `dav_m510/kernel_reg_compute_gather_mask_impl.h:94` 对 `u8/u16/u32` 通用调用 | **支持**（通用指令） |
| `vintlv` / `vdintlv` / `vsel` / `vlds` / `vsts` / `vldus` / `vstus` / `vldas` / `vstas` / `vsldb` / `vsqz` / `vsstb` / `vlda` / `vldu` / `vstu` / `vsta` / `vstur` / `vcvt` | 未逐一核对 | 全部在 `dav_m510` basic_api 中出现，文件数与 `dav_3510` **完全一致** | **支持** |

即：**§4.7 守卫区域内引用到的每一个 intrinsic，M510 的 basic_api 都实现了**。整个 `dav_m510` basic_api 中仅有 4 处 `#if __NPU_ARCH__ == 5102` 守卫：1 处是上面 expdiff 的类型扩展，另 3 处是 `SetPcieRDCtrl` / `SetPcieWRCtrl` / `Trap` 的真不支持桩（`kernel_operator_sys_var_impl.h:126/133/136`）。

**修正动作（已落地）**：

1. **补齐 18 个非 cube 顶层分发头的 5102 分支**（§4.9 遗留的缺口）：`vector_datamove`、`reg_convert`、`reg_fused`、`reg_logic`、`reg_permute_sel`、`loadalign`、`storealign`、`storeunalign`、`sync`、`spr`、`sys_var`、`ub_scatter`、`datamove_atomic`、`scalar_atomic`、`reg_addr_reg`、`vector_compute_composite`、`cube_compute_composite`、`cube_datamove_composite`（`reg_arith` / `reg_reduce` 仅 9201 有 arch 专属实现，无 3510 对应，不补）。此前 §4.9 曾判断「补这些分支会把不支持的 s4/expdiff 路径拉进 5102」——该判断同样基于 §4.5 的错误前提，实测（5102 预处理 0 错误、`vexpdif`/`vcvt_s42bf16` 计数不再因补分支而上升）证明不成立。
2. **收缩 §4.7 守卫**：移除 1031 个守卫块，仅保留 83 个——`cube_datamove_intf_impl.h` 的 79 个（§4.8，M510 basic_api 对这些 L0 加载路径确实是 ASSERT/报错桩）与 `reg_fused_intf_impl.h` 的 4 个 `asc_exp_sub_half2float`（`Tuple<float,half>`，M510 不支持）。

**验证（ccec CANN 9.3.0，dav-510r2 / dav-920r2 预处理 `c_api/asc_simd.h`）**：

| 检查项 | M510 (5102) | 9201 |
|------|-----------|------|
| 预处理 | 0 错误 | 0 错误 |
| 恢复的算子（此前 M510 为 0） | `l0c2gm`=28、`l0c2l1`=27、`loadalign_unpack_v2`=27、`int4x22half_v2`=1、`exp_sub(float,float)`=1、`scatter`=27、`copy_gm2ub`=2 | — |
| 保留守卫（M510 必须为 0） | `exp_sub_half2float`=0；cube 10 个 L0 加载家族=0 | `exp_sub_half2float`=4、cube 家族全部保留 |
| fp4 MX Drop | `asc_mmad_mx`=0 | 30 |
| 函数名集合差异（9201→M510） | 仅 4 个：`asc_vf_debug_publish`、`asc_vf_debug_ub2gm`、`get_ring_buf_read_info`、`get_ring_buf_write_info`（VF-debug/ring-buffer 调试设施，`npu_arch_5102/asc_debug_utils_impl.h` 为 105 行存根，3510/9201 各 445 行；属 dump/调试设施，见下方已知缺口） | — |

**已知缺口（未修，需后续）**：
- `impl/utils/debug/npu_arch_5102/asc_debug_utils_impl.h` 仅提供 `asc_entire_dcci_impl` / `asc_debug_get_system_cycle_impl`，缺少 3510/9201 具备的 VF-debug 与 ring-buffer 调试设施（上述 4 个函数）。补齐需 M510 dump 设施的领域知识。
- `asc_copy_l12l0c` / `asc_copy_l12gm` 在 3510/9201/5102 下均只有声明、无定义（定义位于 `memory_base_impl/cube_datamove_intf_impl.h`，而该文件仅被 `__NPU_ARCH__ == 2201` 的公共头分支引用）。这是上游既有状态，**非 M510 回归**，但值得单独反馈。

**对测试矩阵的影响**：原按 §4.5/§4.7 标为 Drop 的 fp4/fp8/s4/expdiff 用例（`reg_convert`、`loadalign`、`storealign`、`storeunalign`、`loadunalign`、`reg_permute_sel`、`reg_gather`、`reg_fused`）**大部分应改回 Reuse**，仅 `reg_fused` 的 expdiff **half→float** 用例仍为 Drop。§8 与测试 README 已同步修订。

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

- [x] __NPU_ARCH__ == 5102 已确认：用户确认 5102 即宏值、编译器认识（--cce-aicore-arch=dav-510r2 配合 -D__NPU_ARCH__=5102）；编译工程 mc62cm12a.py 已注入该宏。注意 ccec 不会自动预定义该宏，须显式 -D 传入（与构建工程一致）。5101/5161/5163/5165 兄弟族沿用同一 npu_arch_5102 分支。
- [ ] tikicpulib::ascendm510 CPU 模型库就绪时间（阻塞测试实际运行）。但已验证：不依赖 tikicpulib，ccec（CANN 9.3.0）即可编译验证 M510 capi 实现，故 impl 工作不受该库阻塞。
- [x] cube 耦合裁剪（Phase 2）：完成并验证。①按 §4.8 在共享/通用 cube_datamove 头对 M510 不支持的 L0 加载函数族加「排除仅 5102」守卫（10 族约 147 个定义）；②按 §4.9 在顶层 cube 分发链补 5102 分支，修复 M510 取不到支持算子（l0c2gm/l0c2l1 0→28/27）的缺陷；fp4 MX（asc_mmad_mx*）按 §4.5 Drop。ccec 双向验证：5102 全量 0 错误、Drop 家族定义数 0、支持算子定义数 >0、不支持指令 0；9201 零回归。
- [x] expdiff / fp4 / fp8 / s4 指令支持性：**原结论已修正（§4.10）**。原判断「M510 缺这些指令」被 `dav_m510` basic_api 实证推翻——`vexpdif` / `vstar` / `DIST_UNPACK4_B8` / `vcvt_s42*` / `vcvt_f162s4` / `vselr` / `vintlv` 等 M510 均实现。唯一确认不支持的是 expdiff 的 `Tuple<float,half>` 类型组合（对应 `asc_exp_sub_half2float`，4 个函数，守卫保留）。§4.7 的 1114 个守卫已收缩为 83 个。
- [x] fp4 MX（asc_mmad_mx*）确认不支持：用户确认 M510 对 fp4 等数据类型确实不支持（非缺开关），故首期 Drop（与 §4.5 一致）。经验证 `asc_mmad_mx` 在 M510 编出中定义数为 0、9201 为 30，Drop 生效。
- [ ] `impl/utils/debug/npu_arch_5102/asc_debug_utils_impl.h` 为 105 行存根，缺 VF-debug / ring-buffer 调试设施（4 个函数，见 §4.10 已知缺口）。需 M510 dump 设施领域知识，留作后续。
- [ ] `asc_copy_l12l0c` / `asc_copy_l12gm` 在 3510/9201/5102 下均只有声明无定义（`memory_base_impl/cube_datamove_intf_impl.h` 仅被 `__NPU_ARCH__ == 2201` 引用）。上游既有问题，非 M510 回归，建议单独反馈。
- [x] 命名已定：npu_arch_5102（与现有数字约定一致），对外称 M510（npu_arch_5102）。
- [ ] adv_api（impl/adv_api/detail/.../*_3510_impl.h）是否需同步 M510 适配（本 RFC 范围外，作为后续）。

---

## 7. 实施计划（分阶段）

**实施状态（截至本 RFC 修订）**：

- Phase 0（审计）：完成。确认分发机制、公共门控、通用头需补 5102 分支。（其中「M510 缺 expdiff/fp4/fp8/s4 指令」一项后被 §4.10 推翻。）
- Phase 1（骨架）+ §4.7（排除守卫）：骨架完成并验证（新增 `npu_arch_5102/` 薄包含 shim、公共门控与通用头分发链加 5102 分支）。**§4.7 的大规模守卫（1035 个函数）后被 §4.10 判定为误伤，已收缩至 83 个。**
- 验证：ccec（CANN 9.3.0，`--cce-aicore-arch=dav-510r2 -D__NPU_ARCH__=5102 -std=c++17`）对 `c_api/asc_simd.h` 预处理 **0 错误**；同法以 `dav-920r2 -D__NPU_ARCH__=9201` 预处理 **零回归**。注：本环境 `-E` 预处理可行但 `-c` 全量编译缺 ccec 内建类型头（`float4_e2m1x2_t` 等），故以「定义体计数 + 指令计数」作为编出证据。
- Phase 2（cube 裁剪 + 分发修复 + 前提修正）：完成并验证。①§4.8：cube 10 族 L0 加载加守卫（约 147 个定义，保留）；②§4.9：顶层 cube 分发链补 5102 分支，修复 M510 取不到支持算子（l0c2gm/l0c2l1 0→28/27）的缺陷；③§4.10：补齐 18 个非 cube 分发头 5102 分支 + 收缩 §4.7 守卫（1031 移除 / 4 保留）；fp4 MX（asc_mmad_mx*）Drop 保留。最终 M510 与 9201 的函数名集合差异仅 4 个 VF-debug 函数。
- Phase 3（expdiff）：原按「M510 无 vexpdif」处理；§4.10 修正后仅需保留 `asc_exp_sub_half2float`（`Tuple<float,half>`）守卫，其余 expdiff 路径恢复。
- Phase 4（测试 + CMake）：阻塞于 tikicpulib::ascendm510；impl 编译验证已用 ccec 完成。
- Phase 5（文档）：进行中（本 RFC 修订 + 测试 README）。
- **后续 follow-up**（非本阶段）：`npu_arch_5102/asc_debug_utils_impl.h` 补 VF-debug/ring-buffer 设施；向上游反馈 `asc_copy_l12l0c`/`l12gm` 声明无定义问题；若 §4.10 的指令支持结论需编译器侧最终确认，建议与编译器团队复核。
- Phase 4（测试 + CMake）：阻塞于 tikicpulib::ascendm510；impl 编译验证已用 ccec 完成。
- Phase 5（文档）：进行中（本 RFC 修订 + 测试 README）。


- **Phase 0 审计**：① 列出 23 个通用头中需补 `5102` 分支的文件；② 完成 §4.2 复用/裁剪清单；③ 与编译器团队确认 `__NPU_ARCH__` 值。
- **Phase 1 骨架**：新增 `reg_base_impl/npu_arch_5102/`；复制 3510 的 vector/reg/scalar 文件（直接复用）；扩展公共门控与通用头分支。
- **Phase 2 cube**：基于 `dav_m510/kernel_operator_mm_impl.h` 裁剪 `npu_arch_5102/cube_*`（删除/守卫不支持路径）并补齐顶层 cube 分发链 5102 分支；**已完成并验证**（见 §4.8 / §4.9 / §7 状态）。
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
- **Adapt**：需按 M510 差异改写（主要是 cube 耦合路径）。
- **Drop**：3510 专属、M510 首期不提供（如 fp4 MX、expdiff 的 half→float 组合）。

> ⚠️ **本表已按 §4.10 修正**：原先按 §4.5/§4.7 标为 Drop 的 fp4/fp8/s4/expdiff 用例，经 `dav_m510` basic_api 逐指令核对确认 M510 **支持**，已改回 Reuse；仅保留两处 Drop。

| 类别 | 3510 用例示例 | M510 状态 |
|------|---------------|-----------|
| atomic | `test_set_atomic_add_int16.cpp` 等 | Reuse |
| sync | `test_asc_sync_block_wait.cpp` 等 | Reuse |
| sys_var | `test_get_block_idx.cpp` 等 | Reuse |
| scalar_compute | `test_asc_popc.cpp`、`test_asc_atomic_*.cpp` 等 | Reuse |
| cache_ctrl | `test_dci.cpp`、`test_icache_preload.cpp` 等 | Reuse（需核对指令） |
| cube_compute | `test_mmad.cpp`、`test_mmad_bias.cpp` | Reuse（共享 mmad 子集） |
| cube_compute | `test_mmad_mx.cpp`、`test_mmad_mx_bias.cpp`、`test_mmad_mx_sync.cpp` | **Drop**（fp4 MX，M510 不支持；实测 M510 编出 0 定义、9201 30 定义） |
| cube_datamove | `test_asc_copy_gm2l1*.cpp`、`test_asc_copy_l0c2gm.cpp`、`test_copy_l0c2l1_instr.cpp`、`test_asc_copy_l0c2ub.cpp`、`test_asc_copy_l12ub.cpp` 等 | Reuse（§4.9 分发修复后 M510 可取得 `l0c2gm`/`l0c2l1` 定义） |
| cube_datamove | `test_asc_copy_gm2l0a.cpp`、`test_asc_copy_l12l0a.cpp`、`test_copy_l12l0a_mx.cpp`、`test_asc_copy_l12l0b.cpp`、`test_copy_l12l0b_trans.cpp`、`test_asc_copy_l12bt.cpp`、`test_asc_copy_l12fb.cpp` 等 | **Drop**（GM→L0A/L0B、L1→L0A/L0B transpose/sparse、L1→BT/FB、LoadImage 到 cbuf；`dav_m510/kernel_operator_mm_impl.h` 对这些路径是 ASSERT/报错桩，见 §4.8） |
| vector_compute | `test_return_vconv.cpp`（vconv 重载，含 fp4/fp8/s4 数据类型） | **Reuse**（§4.10：M510 支持 `vcvt_s42*`/`vcvt_f162s4` 等） |
| vector_compute | `test_return_vconv.cpp`（expdiff `Tuple<float,half>` 用例） | **Drop**（§4.10：M510 的 `FusedExpSubImpl` 仅支持 `Tuple<half,half>`/`Tuple<float,float>`） |
| vector_compute | `test_return_vconv.cpp`（expdiff `Tuple<float,float>`/`Tuple<half,half>` 用例） | **Reuse**（§4.10：`vexpdif` M510 支持） |
| vector_datamove | `test_*` | Reuse |
| misc | `test_asc_init.cpp` 等 | Reuse |
| reg_compute | `reg_*` 系列 | Reuse（§4.10：18 个非 cube 分发头已补 5102 分支） |
| reg_convert（fp4/fp8/s4） | `vcvt_s42*` / `vcvt_f162s4` / `UNPK4_B8` 相关用例 | **Reuse**（§4.10 修正，原 Drop 有误） |
| loadalign / storealign（fp4） | `UNPK4_B8` 解包/打包相关用例 | **Reuse**（§4.10：M510 `CheckLoadDist` 支持 `DIST_UNPACK4_B8`） |
| loadunalign / storeunalign（fp4/fp8） | 非对齐加载/存储相关用例 | **Reuse**（§4.10：`vldus`/`vstus`/`vstar` 等 M510 均支持） |
| reg_fused（expdiff） | `asc_exp_sub_half2float` 相关用例 | **Drop**（§4.10：`Tuple<float,half>` 不支持，守卫保留） |
| reg_fused（expdiff） | `asc_exp_sub(float,float)` 相关用例 | **Reuse**（§4.10：`Tuple<float,float>` 支持，守卫已移除） |
| reg_permute_sel / reg_gather（fp4/fp8） | `vintlv`/`vselr` 相关选择/交织/gather 用例 | **Reuse**（§4.10 修正，原 Drop 有误） |
| debug（VF-debug / ring-buffer） | `asc_vf_debug_publish`、`get_ring_buf_*` 等 | **Drop（已知缺口）**：`npu_arch_5102/asc_debug_utils_impl.h` 为 105 行存根，需后续补齐（§4.10） |
