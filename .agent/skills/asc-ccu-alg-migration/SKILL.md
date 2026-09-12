---
name: asc-ccu-alg-migration
description: Use when 迁移/移植 CCU 算法（hccl 仓 → asc-devkit 仓）、新增 CcuSched/CcuAlltoAll/CcuAllGather 等算法到 CCU tiling 新路径、注册 REGISTERED_CCU_ALGORITHMS 白名单、或排查迁移后算法不生效/fallback/KFC server 分发缺失问题。
---

# CCU 算法迁移（hccl 仓 → asc-devkit 仓）

## Overview

把 hccl 仓老路径（host 逐 op 直发 CCU mission）的 CCU 算法，迁到 asc-devkit 仓 tiling 新路径
（tiling 白名单 → GE 生成 AIC launcher + KFC server 指令窗口 → `CcuKernelLaunch` 每次下发）。
迁移 = **kernel 搬运适配 + 7 处注册接线**，缺一处则 tiling fallback、执行不命中或 UT 失败。

基准范本（AllToAll 迁移，照此对齐）：
- 迁移合入 `3cd5022114`（feat: AllToAll CCU 算法移植 + ST 测试框架，MR !5425）
- 命名统一 `8b2d6fae33`（unify alltoall ccu algorithm names，MR !5481）

## 迁移分层清单（9 层，按序执行）

| # | 层 | 文件（asc-devkit 仓，`CC=impl/adv_api/detail/hccl`） | 做什么 |
|---|---|---|---|
| 1 | tiling 白名单 | `impl/adv_api/tiling/hccl/hccl_tiling.cpp` | `REGISTERED_CCU_ALGORITHMS` 加新算法名（命中→`INIT_TILING_CCU_NEW_VERSION`） |
| 2 | 算法 ID 枚举 | `$CC/cc/src/common/hccl_alloc_ctx_res.h` + 设备侧镜像 `impl/.../platform_v310/hccl_ccu_v0_def.h` | `enum class AlgorithmType` 加枚举值 + 下方 `algorithmMap` 加 `{"算法名", 枚举}`。编号分段：AllGather=0~、ReduceScatter=50~、AllReduce=100~、AllToAll=150~。**新值必须显式赋值**（查段内当前已用最大值+1，勿依赖隐式接续）——并行迁移若在你前面插入隐式条目会使后续值漂移，你的显式值可能与漂移后的隐式值成同值别名（git auto-merge 无冲突静默合入，AIC 路由误判）；设备侧 `hccl_ccu_v0_def.h` 有独立镜像枚举需同步加同值。**合并 upstream 后必查**：宿主与设备镜像两处自己的值未漂移、未与隐式值撞名 |
| 3 | opType 白名单 | `$CC/cc/src/common/hccl_alloc_ctx_res.cc` | IsXxxOpType 判断加本 `HcclCMDType`；`$CC/common/hccl_inner_def.h` 的 `DATA_TYPE_MAP` 核对本算法 dtype 字节数（AllToAll 迁移曾修正 UINT16 位 8→2） |
| 4 | selector | `$CC/ops/<op>/selector/*_auto_selector.cc` | `SelectCcuScheduleAlgo` 分支接线：占位名 → 真名，`NOT_MATCH` fallback → `MATCH` |
| 5 | executor | `$CC/ops/<op>/executor/ins_v2_*.cc` | `REGISTER_EXEC_V2(HcclCMDType::X, <名>, <Executor>, <TopoMatch>, <Temp>)`，包在 `#ifndef AICPU_COMPILE` 内 |
| 6 | template + kernel | `$CC/ops/<op>/template/ccu/`（ccu_temp_x.cc/.h + kernel/） | 从 hccl 仓搬 + 适配（见下）+ 两级 CMakeLists 接线 |
| 7 | kfc_server 分发 | `$CC/ops/kfc_server/template/ccu/ccu_temp_kfc_server.cc` 与 `kernel/ccu_kernel_kfc_server.cc` | DispatchKfcSubKernel 加 `isXxx` 分支、`HBM_PARAM_IDX_n` 传参、`CCU_IF(opType==X)` 调 kernel；mission 数看 `kfc_server_protocol.h:GetKfcServerMissionNum` |
| 8 | UT | `tests/api/adv_api/hccl/ccu/test_ccu_mc2.cpp`、`tests/api/adv_api/tiling/test_hccl_tiling.cpp` | algorithmMap 断言（size+枚举值）；`opType+算法名+engine → INIT_TILING_CCU_NEW_VERSION` 用例 |
| 9 | ST | `tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/<op>/` | testcase（`TEST_F` + `scenario.expectedAlgName`）+ adapter；复用既有 ccu_sim_world/ccu_checker 框架 |

另按需：`$CC/ccu/hccl_ccu_v0_prepare.h` 加/改 `CcuPrepareForXxx` —— AIC launcher 侧参数翻译
（aclnn 的 `CommonPrepareParamCcu` → xn 信箱 `xnData_[n]`，末尾 `CalcGoSize` 算 GroupCopy 的 GoSize），
仅当参数布局与既有 prepare 函数不同才需要。注意：AllToAll 首版（c1a0bd09c0）曾有 CCU_WHILE 按
256MB 分片 Write（writeLoopCount 倒计数 + NEG_256MB），合入前已删除，最终为单条 Write 全量搬运；
单 slice >256MB 场景未经确认，大流量算法迁移时需实测验证。

## kernel 适配模式（hccl → devkit，从 3cd5022114 diff 提取）

```cpp
// 1. namespace: ops_hccl → mc2_ops_hccl；版权年改 2026
// 2. include "ccu_kernel_alg_base.h"（Group*/CCU 宏基座）
// 3. 参数走 ctx.arg：ctx.channels/rankSize/rankId → arg->channels/rankSize/rankId
//    （kfc_server 分发时从 HBM param[idx] + ctx.arg 取，见 kfc_server kernel 改法）
// 4. 补齐常量：CKE_IDX_n / XN_ID / POST_SYNC_BIT 等 event 位定义
// 5. 搬运形态照 hccl 仓终版原样：单条 ccu::Write(sliceSize) / LocalCopy / GroupCopy(goSize)；
//    分支两路（loadFromMem=true/false）都要覆盖，事件 mask 非 r==rankId 分支 1<<r
```

## 处理逻辑一致性（验收标准）

**迁移 kernel 的处理逻辑必须与 hccl 仓终版完全一致**——"结果对得上"不等于迁移成功；只有逻辑同构，
hccl 侧将来的行为变化/bug 修复才能同步跟进。

- **必须一致**：搬运结构（Write/LocalCopy/GroupCopy 及长度）、事件拓扑（event 数量/mask 语义/wait 顺序）、
  分支覆盖（loadFromMem 两路等）、切片/分块计算、空片防御（EventRecord 补位）；
- **允许不同（适配面，非逻辑）**：namespace/include、参数来源（LoadArg→形参/HBM 槽）、
  CKE 编号/PostSync 位等资源编排、日志；
- **禁止**：以"结果等价"为由简化或重构 hccl 逻辑（如单 event 替换 per-rank eventList、删空片防御、
  合并 wait）——骨架差异在 hccl 侧演进时变成行为差异。

合入前**逐段对照 hccl 源 kernel 审计**：每一处 diff 都要能归入"允许不同"清单；归不进的要么改回，
要么在设计文档立 DDR 说明理由。

## 命名规范（8b2d6fae33）

注册名统一 `CcuSched<Op><Topo>[Variant]`（如 `CcuSchedAllToAllSoleMesh`、`CcuSchedAllGatherConcurMeshNHRMultiLink`）；
**kernel 函数名与 ABI 不改名**。改名 PR 需同步：白名单、selector、executor、kfc_server、
hccl_alloc_ctx_res 映射、ST testcase 期望名 —— 只做字符串替换，不动逻辑。

## 验证清单（合入前）

```bash
# UT（MR !5481 实测命令）
#   ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC  全绿
#   ascendc_ut_adv_api_hccl_ccu_mc2                            全绿
#   ascendc_ut_adv_api_ccu_st                                  全绿
# 白名单生效自检
grep -n "新算法名" impl/adv_api/tiling/hccl/hccl_tiling.cpp
# C/C++ 提交前 clang-format（CI pre-commit 强检）
git diff --name-only upstream/master HEAD | grep -E "\.(cc|h|hpp|cpp)$" | xargs clang-format --style=file -i
# 逻辑一致性审计（合入前必做）：逐段对照 hccl 源 kernel（分支/事件/mask/wait/切片/防御），
# 每处差异必须归入"允许不同"清单（见"处理逻辑一致性"节），否则改回或立 DDR
```

运行期验证：plog 确认 `HcclGetCcuKernel`/kfc_server mission 下发；profiling 出 CcuTask
（需 profSwitch 含 L1 位 `ACL_PROF_TASK_TIME`=0x0002，benchmark 侧 hccl_api_test.cc:786 开关）。
Group\* 原语的 kernel 额外产出 CcuGroupInfo（dataSize 权威来源）。

## 常见坑

| 坑 | 症状 | 规避 |
|---|---|---|
| 白名单漏注册 | tiling fallback 走老路径，KFC server 不加载 | 层 1 先行，UT 层 8 兜底 |
| executor 未包 `#ifndef AICPU_COMPILE` | AICPU 编译目标挂 | 照抄 3cd5022114 写法 |
| CMakeLists 漏接 template/kernel | 链接错或文件孤立 | 两级 CMakeLists 都要加 |
| 超 256MB DMA（存疑） | 大 slice 下发失败/数据错 | 合入版无分片；单 slice >256MB 需实测，出问题回溯 c1a0bd09c0 的 CCU_WHILE 分片方案 |
| 注册名不一致（selector/executor/白名单/ST 四处） | 选不中或 ST 断言失败 | 一处定义、四处 grep 对齐 |
| 枚举值冲突 | algorithmMap UT 挂 | 层 2 分配新值并断言 |
| 自创逻辑/以"结果等价"为由简化重构 | 当前能跑，hccl 侧演进后行为漂移无法同步 | 逐段对照 hccl 终版审计；每处差异归入允许清单或立 DDR |

## 实战档案

**AllToAll Mesh1D 演进链**（迭代 5 版，08-13~08-28）：分支尝试 `8fc6f30d65`（白名单先行）→
`c1a0bd09c0`（首版：迁移+256MB 分片修复+ST 框架）→ `bc36f6c2e3`/`b48f45238f`（迭代）→
`3cd5022114` 合入 master（现名 `CcuSchedAllToAllSoleMesh`，改名 `8b2d6fae33`）。

**下一个迁移：CcuAlltoAllMesh1DMultiJetty → CcuSchedAllToAllMesh1DMultiJetty**（已裁决：To 大写；2026-09 摸底）：
- **状态：已实施（2026-09-11）**，分支 `feat/ccu-alltoall-multijetty-migration`（400+ 测试绿，READY FOR MR）。
  实施要点（后续迁移复用的经验）：kfc_server 实为 **5 接线点**（role 枚举+映射 / InheritKfcServerKernelArg
  static_cast 拷贝 / **CcuTempKfcServer::CalcRes 白名单**——漏了整链死代码 / dispatch 分支 / ST 侧 CMakeLists
  vtable 链接）；`channel.cc:748 CalcChannelRequestMeshClosMultiJetty` 的 engine 闸门曾漏 CCU（迁移算法
  CalcRes 必失败，排查时先查这里）；AIC 侧 `hccl_ccu_v0_def.h` 有独立 AlgorithmType 镜像需同步加（显式值必需）；
  ST 造 MESH_1D_CLOS 用 phyId 标记位 0x80000000（纯网格造不出恰好 2 实例）。
  **并行迁移合并教训（3 轮 upstream merge 实录）**：枚举值两次被撞（MultiJetty 153→156、SLICED role 4→7→9），
  根因是他人插入隐式枚举条目使值漂移、与显式值成同值别名且 auto-merge 无冲突——层 2 规则已更新为
  显式赋值+合并后查漂移；role/枚举类共享常量合并时先读 upstream 注释里的预留约定再选值。
- 源（hccl 仓）：`src/ops/all_to_all_v/template/ccu/ccu_temp_all_to_all_mesh1d_multi_jetty.cc`（209 行）+
  `kernel/ccu_kernel_all_to_all_mesh1d_multi_jetty.cc`（241 行，GroupCopy :192）
- 源注册：selector MESH_1D_CLOS 分支（meshNum==closNum 且 ≤4 rank 且大流量 → Concurrent，否则 MultiJetty）；
  executor `REGISTER_EXEC_V2(HCCL_CMD_ALLTOALL, CcuAlltoAllMesh1DMultiJetty, InsV2AlltoAllVSoleExecutor, TopoMatchUBX1d, CcuTempAllToAllMesh1dMultiJetty)`
- devkit 现状：selector :81 已有占位名 `CcuSchedAllToAllSoleMeshUBX`（NOT_MATCH fallback，迁移时改为本目标名）；
  `TopoMatchUBX1d` 已存在（`$CC/ops/op_common/topo/topo_match_ubx_1d.h`）；
  `KfcServerRole::ALL_TO_ALL_MULTI_JETTY=3` 已预埋（kfc_server_protocol.h，当前映射 Concurrent 名）
- 枚举：`hccl_alloc_ctx_res.h:87 AlgorithmType`，建议 `CcuSchedAllToAllMesh1DMultiJetty = 153`
- 迁移时新增：kfc_server HCCL_CMD_ALLTOALL 已有 Mesh1D 分发，MultiJetty 需按 kernelFuncName 区分分支
- 方案与设计（2026-09-10 已定）：**方案A = NHR 式标量切片，N=1 起步**（参数坍缩 11+2×rankSize → 恒定 13 槽；
  per-rank eventList + jetty 位保持 hccl 事件拓扑），全套独立件（独立枚举段/独立 prepare/独立路由
  algorithmType 判别/独立 role=4），设计文档：
  `growthring/notes/superpowers/specs/2026-09-10-alltoall-multijetty-migration-design.md`（v2）
