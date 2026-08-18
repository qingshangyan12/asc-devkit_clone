# CcuSchedAllGatherSoleMesh 迁移会话恢复记忆

## 1. 新 Session 恢复顺序

新 Session 开始后，先按以下顺序读取：

1. 本文档：本次 CCU AllGather 迁移的最终状态、验证结果和待办。
2. `docs/cc_algorithm/ccu_sched_all_gather_sole_mesh_migration_plan.md`：完整源码对比、设计和实施结果。
3. `.agent/skills/migrate-hccl-kfc-algorithms/SKILL.md`：继续同步或迁移 HCCL 算法时必须遵循的流程。
4. `docs/cc_algorithm/cc_iter1_session_memory.md`：迭代 1 的历史背景和其他算法状态。
5. `docs/cc_algorithm/mc2_st_test_framework_design.md`：AICPU MC2 E2E ST 与 CCU host ST 机制。

恢复完成后，以用户的新命令为准，不要自动继续第二阶段或修改代码。

## 2. 当前仓库快照

| 项目 | 当前值 |
| --- | --- |
| 记录时间 | 2026-08-17，Asia/Shanghai |
| asc-devkit 路径 | `/mnt/c/data/gitcodes/asc-devkit` |
| asc-devkit 分支 | `cc0806_2` |
| rebase 目标 | `trunk/master`，`1a58781c33d15509b5c1cb45182c29aa8b02e893` |
| rebase 前 HEAD | `d6614fce4586400491218af494da158786ebb32a` |
| rebase 后算法提交 | `3d2effae4 ccu ag fullmesh 1d update 1 step` |
| 最新提交 | 本文档所在的 rebase 冲突修复提交，以 `git log -1` 为准 |
| 远端状态 | 本地历史已重写；`origin/cc0806_2` 仍指向 rebase 前的 `d6614fce4`，尚未推送 |
| 工作树状态 | rebase 冲突修复和本文档提交后应为干净状态 |
| 原实施基线 | `97d5ca108aeed3471820dd6517f29d0cb0a2c789` |
| HCCL 路径 | `/mnt/c/data/gitcodes/hccl` |
| HCCL 分支 | `master`，与 `origin/master` 一致 |
| HCCL HEAD | `a5101c31`；本次仅按 skill 快进检查，没有同步新增 HCCL 代码 |
| CANN | `/home/houyu/Ascend/cann-9.2.0` |
| 目标架构 | `ascend950pr_9599`、`SocVersion=ASCEND950`、`__NPU_ARCH__=3510` |

本次迁移的 26 个源码、测试和方案文档文件已随 11 个本地提交 rebase 到最新 `trunk/master`。旧提交
`d6614fce4` 对应的新提交为 `3d2effae4`；完整新提交序列见本文第 10 节。

## 3. 用户确认的范围

本次目标是从最新 HCCL 同步 Ascend 950 CCU AllGather 算法，并适配 asc-devkit MC2/KFC Server 框架。

算法名称：

- HCCL canonical 名称：`CcuSchedAllGatherSoleMesh`。
- 历史注册名：`CcuAllGatherMesh1DMem2Mem`。
- HCCL 原生 template/kernel 类名仍保留 Mesh1D Mem2Mem 命名，不需要为了表面统一机械改名。

范围内：

- CCU/KFC Server。
- MC2 路径。
- 单层、单 die、`MESH_1D`。
- host 控制面 ST。

明确不包含：

- 双 die。
- UBX/Clos 和 NHR。
- HalfAllToAllV。
- DPU。
- CCU device 仿真结论。
- HCCL 最多 16 repeat 的聚合性能协议改造。

## 4. 已完成实现

### 4.1 名称和注册合同

- selector、executor 注册、tiling allowlist、host/device `AlgorithmType`、algorithm map 和测试全部使用
  `CcuSchedAllGatherSoleMesh`。
- `AlgorithmType` 数值保持为 `1`，没有改变 host/device ABI 数值。
- `impl/` 和 `tests/` 中不再残留旧注册名 `CcuAllGatherMesh1DMem2Mem` 或旧枚举名
  `CcuAllGatherMeshMem2Mem1D`。

关键文件：

```text
impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.h
impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0_def.h
impl/adv_api/tiling/hccl/hccl_tiling.cpp
impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
```

### 4.2 Selector

- `SelectCcuMsAlgo` 不直接选择 schedule 算法，返回 `NOT_MATCH`。
- 复用 `AutoSelectorBase` 已有的 `CCU_MS -> CCU_SCHED` fallback。
- `SelectCcuScheduleAlgo` 只允许 `topoLevelNums == 1`。
- level 0 必须是 `MESH_1D + SINGLE_DIE`，并拒绝 input/output overlap。
- 双 die、多层和 UBX/Clos 不会误选 `CcuSchedAllGatherSoleMesh`。

关键文件：

```text
impl/adv_api/detail/hccl/cc/src/ops/all_gather/selector/all_gather_auto_selector.cc
impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/auto_selector_base.cc
```

### 4.3 KFC 数据面适配

- AllGather 动态 XN 数从 9 增至 10。
- `CcuPrepareForAllGatherM2M` 使用 XN9 传递 input 与本 rank output slice 是否同址：

```cpp
xnData_[9] = (xnData_[1] == xnData_[2] + xnData_[3]) ? 1 : 0;
```

- KFC Server dispatch 将 `param[HBM_PARAM_IDX_9]` 传入 AllGather 子 kernel。
- `CcuKfcAllGatherMesh1DMem2MemKernel` 在 XN9 为 1 时跳过本地 `GroupCopy`。
- AllReduce 复用 AllGather 阶段时显式传入 `isInputOutputEqual = 1`。

关键文件：

```text
impl/adv_api/detail/hccl/ccu/hccl_ccu_v0_prepare.h
impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0.h
impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc
impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc
impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/ccu/kernel/ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.cc
```

### 4.4 与 HCCL 原生实现的保留差异

第一阶段保留 asc-devkit KFC 的逐 mission 协议：MC2 client 按 repeat 拆分任务，子 kernel 一次只处理一个 mission。
没有把 HCCL 原生 `AG_UNROLL_NUM=16` 直接复制进 KFC 子 kernel，否则 client 与 kernel 会重复执行 repeat。

HCCL 最新普通 Mesh template 使用 `CalcChannelRequestMesh1DFullMesh`，本次限定单层、单 die，asc-devkit 继续使用
`CalcChannelRequestMesh1D`。放开多层、跨框或双 die 前，必须完整迁移 full-mesh helper 及其依赖闭包，不能只替换函数名。

## 5. ST 框架和覆盖边界

新增测试：

```text
tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/host/all_gather_mc2_host_testcase.cpp
```

父级 `mc2_ccu` CMake 子目录已正式接入测试构建。

专项 ST 分为两层：

1. 直接编译和调用真实 `AllGatherAutoSelector`，验证 canonical 名称、`CCU_SCHED`、`CCU_MS` fallback、双 die、
   多层和 overlap 拒绝。
2. 从 MC2 tiling 调用 `HcclAllocComResourceByTiling`，验证 rank 2、4、8、16、17 的 KFC Server host 资源、
   `algorithmType=1`、`isKfc`、单 Server 注册和 6 个序列化 Server 参数。

真实性边界：

- 生产 selector 是真实代码。
- MC2 资源构造仍使用 executor 桩，因此不能声称生产 executor/template 已由 ST 执行。
- rank 17 只证明 host 资源构造，不证明 CCU event mask 在设备执行。
- 没有 CCU 仿真器，不能声明 CCU device 数据正确性 E2E 通过。

## 6. 最终验证结果

### 6.1 测试

| 验证项 | 最终结果 |
| --- | --- |
| AICPU HCCL 算法回归 | rebase 后重新构建，`86/86` 通过 |
| MC2 CCU host ST | rebase 后重新构建，`22/22` 通过 |
| AllGather 专项 host ST | `10/10` 通过 |
| `bash build.sh --pkg -j4` | rebase 前通过；rebase 后未重复打包 |

测试二进制：

```text
build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC
build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/mc2_ccu/ascendc_ut_adv_api_hccl_ccu_mc2_host
```

复跑命令：

```bash
cd /mnt/c/data/gitcodes/asc-devkit
source /home/houyu/Ascend/cann/set_env.sh

build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/\
ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC

build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/mc2_ccu/\
ascendc_ut_adv_api_hccl_ccu_mc2_host

build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/mc2_ccu/\
ascendc_ut_adv_api_hccl_ccu_mc2_host \
  --gtest_filter='AllGatherProductionSelectorTest.*:RankBoundaries/AllGatherMc2HostTest.*'
```

本次 rebase 后已使用 `ENABLE_TEST=ON` 重新配置并生成上述两个测试二进制。构建时先恢复了上次打包清理掉的
mockcpp 安装副本；该操作只影响 `build/_CPack_Packages/` 下的生成产物。

### 6.2 覆盖率

报告目录：

```text
build/coverage/ccu_sched_all_gather_sole_mesh/
```

主要文件：

```text
coverage.info
production_coverage.info
html/index.html
html_production/index.html
line_coverage.tsv
test.log
coverage_summary.txt
```

指标：

- 5 个生产文件加 1 个专项测试文件：行 `44.8% (594/1325)`，函数 `56.5% (74/131)`。
- 仅 5 个生产文件：行 `38.2% (452/1183)`，函数 `43.0% (43/100)`。
- `line_coverage.tsv` 共 1326 行，包含表头和 1325 个可执行行的文件、准确行号、命中次数及状态。

不要把测试文件自身 100% 覆盖混同为产品代码覆盖，也不要把 host 覆盖率解释成 CCU device kernel 覆盖率。

### 6.3 安装包和 patch

最终 run 包：

```text
build_out/cann-asc-devkit_9.2.0_linux-x86_64.run
生成时间：2026-08-17 15:09:12 +0800
大小：11735689 bytes
SHA256：8ef023b8c97b5861ee84d5d54e5d4e2124e46e23955540e4584455e782b68763
```

提交前生成的 staged patch 仍保留：

```text
build_out/ccu_sched_all_gather_sole_mesh.patch
大小：71551 bytes
SHA256：085533ea894fee5326a9b62972f357dd3d733b65874a724a66b513af99d22d6b
```

该 patch 对应 rebase 前的 `97d5ca108aee..d6614fce4586`，只作为归档保留。当前代码已 rebase 到新主线，
不要再次应用该 patch，也不要把它当作新提交序列的精确补丁。

构建存在非阻断告警：依赖检查读取到 runtime/metadef 9.1 元数据，但实际使用路径为
`/home/houyu/Ascend/cann-9.2.0`，没有导致编译或打包失败。

`adv_test_two` 中与本次 HCCL 改动无关的 `ascendc_ut_adv_api_tiling_ascend610` 目标在链接时仍有 Conv3D
未定义符号，因此该通用 tiling 目标未通过。本次相关的 HCCL AICPU 和 CCU host 目标均已独立构建并全部通过。

## 7. 真机待验证和后续阶段

以下内容尚未完成，后续不能直接声明通过：

- CCU device kernel 实际执行。
- `Write -> GroupCopy -> WaitEvent` 设备 IR 行为。
- rank 17 尾 event mask。
- XN9 同址场景跳过本地复制的硬件行为。
- count 0、in-place、非对齐大小、repeat/stride 和支持上限的数据正确性。
- 与 HCCL 原生实现的带宽、时延及最多 16 repeat 批处理性能对比。

第二阶段 repeat 聚合若启动，必须同时改 MC2 client/KFC 消息协议、XN/CKE/event 资源、Server cache/协议版本和
子 kernel，不能只在子 kernel 中增加循环。

## 8. 新 Session 快速检查

```bash
cd /mnt/c/data/gitcodes/asc-devkit
git branch --show-current
git rev-parse HEAD
git status --short
git log -1 --oneline

git -C /mnt/c/data/gitcodes/hccl status --short --branch
git -C /mnt/c/data/gitcodes/hccl rev-parse HEAD

sha256sum build_out/cann-asc-devkit_9.2.0_linux-x86_64.run
sha256sum build_out/ccu_sched_all_gather_sole_mesh.patch
```

若用户的新命令要求继续同步 HCCL，先完整读取迁移 skill，并按 skill 要求执行 HCCL `git pull --ff-only`，记录
更新前后 revision 后再比较；不要默认 `a5101c31` 仍是最新 HCCL。

## 9. 不要重复或误操作

- 不要再次应用 `ccu_sched_all_gather_sole_mesh.patch`，代码已 rebase 到新提交序列。
- 不要把 `CcuSchedAllGatherSoleMesh` 注册回 `CCU_MS`；当前通过公共 fallback 进入 `CCU_SCHED`。
- 不要恢复旧注册名 alias，除非出现明确外部 ABI 合同。
- 不要恢复独立 `kfc_server_op.cc/.h` 或 legacy AllGather host op 入口。
- 不要顺带接入双 die、UBX/Clos、NHR、DPU 或 HalfAllToAllV。
- 不要声称 CCU device、数据正确性或性能已经通过 host ST。
- 不要删除 build 下的覆盖率、patch 和 run 包，除非用户明确要求清理。

## 10. 2026-08-17 rebase 记录

执行过程：

1. 将 HCCL `master` 从 `94b7a598` 快进到 `a5101c31`，只记录最新源仓状态，没有扩大本次代码范围。
2. 将 `cc0806_2` 的 11 个提交从旧基线 `77a0b1e1a` rebase 到 `trunk/master` 的 `1a58781c3`。
3. 唯一冲突位于
   `tests/api/adv_api/hccl/cc/st/algorithm/testcase/all_to_all_mesh1d_aicpu_testcase.cc`。
4. 冲突原因是主线把注册检查从 `GetAlgExec()` 改为无副作用的 `IsRegistered()`，分支则调整了本地注册的
   算法集合。最终保留主线 API，同时只检查分支实际迁移的 AllToAllV 和两个 AllReduce 算法。
5. `git range-diff` 显示其余 10 个提交内容等价；第 2 个提交只有上述预期测试 API 差异。

rebase 后的 11 个算法迁移提交：

```text
14a11a496 cc migrate iter1 yhy impl
43f25b47d ubx kfc allgather concurrent
3090443b1 CalcChannelRequestNhrMultiJetty same update
110c5563a concur aicpu sd
ea4cabbd6 fix: adapt MC2 resources after rebase
881ab24da test: run allgather AICPU ST through MC2
f02947e8a add st design
3e31b973f update migrate skill after concurrent and st
9afc4616d sync hccl 0815, impl and test codes updated
3a00ea09e aicpu st test sd update
3d2effae4 ccu ag fullmesh 1d update 1 step
```

验证结果：

- `git diff --check trunk/master..HEAD` 通过。
- HCCL AICPU 算法目标重新构建，`86/86` 通过。
- MC2 CCU host 目标重新构建，`22/22` 通过。
- AllGather 专项包含在 host 结果中，`10/10` 通过。
- `ascendc_ut_adv_api_tiling_ascend610` 因现存 Conv3D 未定义符号失败，与本次冲突文件和 HCCL 目标无关。

## 11. 2026-08-18 Concurrent Mesh + NHR 会话恢复记忆

### 11.1 当前任务和已确认原则

本轮用户要求将HCCL的`CcuSchedAllAGatherConcurMeshNHRMultiLink`（历史名称
`CcuAllGatherConcurrentMesh1DNHRMem`）迁移到asc-devkit，并先后保留两种设计：

- `cc0806_2`：原则X，即“Host优先、双独立队列而非CCU mission间同步”。
- `ccuCurMisSyn`：取消原则X，使用CCU mission间同步的设计。

用户已要求忽略Ascend 610。后续不要为该算法补充Ascend 610适配或将其作为验证目标。

### 11.2 当前仓库快照

| 项目 | 当前值 |
| --- | --- |
| 记录时间 | 2026-08-18，Asia/Shanghai |
| asc-devkit路径 | `/mnt/c/data/gitcodes/asc-devkit` |
| 当前分支 | `ccuCurMisSyn` |
| 当前HEAD | `b86b00f77 add sd files` |
| 当前未提交变更 | `docs/cc_algorithm/cc-concept.md`，本节所记录的会话记忆文件也将随本次记录变更 |
| HCCL路径 | `/mnt/c/data/gitcodes/hccl` |

本会话只进行了源码结构定位和文档更新，未重新运行构建或测试；不能把此前分支的验证结果外推为本轮文档变更后的新验证结果。

### 11.3 两份详细设计文档和PDF

已提交的详细设计文档：

```text
docs/cc_algorithm/ccu_sched_all_gather_concur_mesh_nhr_multi_link_detailed_design_host_dual_queue.md
docs/cc_algorithm/ccu_sched_all_gather_concur_mesh_nhr_multi_link_detailed_design_mission_sync.md
```

两份文档分别描述`cc0806_2`的Host双独立队列设计和`ccuCurMisSyn`的CCU mission同步设计。内容包括：

- 从HCCL同步的代码与为asc-devkit MC2/KFC新增的代码标记；
- selector、executor、template、kernel、资源申请和KFC适配的职责；
- 双kernel机制的简化流程图、泳道图；
- 从MC2 tiling到双kernel运行结束的E2E流程图、泳道图；
- 两分支的核心差异和选择建议。

对应PDF已生成，但位于被`.gitignore`忽略的目录，未随Git提交：

```text
output/pdf/ccu_sched_all_gather_concur_mesh_nhr_multi_link_detailed_design_host_dual_queue.pdf
output/pdf/ccu_sched_all_gather_concur_mesh_nhr_multi_link_detailed_design_mission_sync.pdf
```

### 11.4 本次补充的CCU概念

`docs/cc_algorithm/cc-concept.md`已追加“CCU Kernel参数与资源描述”章节，记录以下源码结论：

1. `AlgResourceRequest::ccuKernelInfos`定义于
   `impl/adv_api/detail/hccl/cc/src/ops/op_common/inc/alg_param.h`，类型为
   `std::vector<CcuKernelInfo>`。
2. 同文件的`CcuKernelInfo`含`resGroup`、`dieId`、`kernelFuncName`、`kernelFunc`、`kernelArg`和
   `channels`；私有`kernelArgSmartPtr`通过`setKernelArg`维持裸指针`kernelArg`的生命周期。
3. `CcuKernelInfo`使用的参数基类是同文件的`CcuKernelArgBase`，其含定长
   `ChannelHandle channels[CCU_MAX_RANK_SIZE]`和`channelCount`。
4. `hcomm::CcuKernelArg`是不同的传统CCU对象模型抽象基类，定义于
   `include/adv_api/hccl/internal/hcomm/pkg_inc/hcomm/ccu/ccu_kernel_arg.h`；它要求派生类实现
   `GetKernelSignature()`，并以`std::vector<ChannelHandle> channels`保存通道。
5. `include/adv_api/hccl/internal/hcomm/ccu/ccu_types.h`中的无命名空间`CcuKernelArg`只是
   `typedef void* CcuKernelArg`。不能把它或`hcomm::CcuKernelArg`与MC2/KFC的`CcuKernelArgBase`混用。
6. Concurrent Mesh + NHR MultiLink executor会按Mesh、NHR顺序合并两项`ccuKernelInfos`，并设置
   `ccuKernelNum = {1, 1}`；KFC适配层据mission分组将两个原始kernel分别转换为KFC server kernel。

### 11.5 下个Session恢复顺序

1. 读取本文第11节，再运行`git branch --show-current`、`git log -1 --oneline`和`git status --short`确认状态。
2. 阅读两份Concurrent算法详细设计文档，明确当前任务应针对Host双队列设计还是mission同步设计。
3. 阅读`docs/cc_algorithm/cc-concept.md`的“CCU Kernel参数与资源描述”章节，避免混淆三种
   `CcuKernelArg`相关类型。
4. 若要继续代码同步或迁移，完整阅读`.agent/skills/migrate-hccl-kfc-algorithms/SKILL.md`，并先检查HCCL
   工作树和revision；不要只按本文中的历史revision盲目继续。
5. 若修改CCU kernel代码，按`.agent/skills/asc-ar-dev/SKILL.md`查询当前devkit API文档并记录证据；本会话未修改
   kernel代码。
