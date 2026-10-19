# AicpuAllGatherSoleMeshConcur 迁移方案

## 迁移前方案

- 状态: READY_TO_MIGRATE
- 生成时间: 2026-10-12 Asia/Shanghai
- 源仓: /mnt/workspace/allgather_migrate/hccl
- 目标仓: /mnt/workspace/allgather_migrate/asc-devkit_4916
- 目标分支: allgather_migrate

### repository_baseline

| 仓库 | 分支/HEAD | Commit | 工作区 | Tracking 远端 | Ahead/Behind |
|---|---|---|---|---|---|
| /mnt/workspace/allgather_migrate/hccl | master | 95711ee1d33e53ebbd75e46e5dc6c49e29bbccd4 | clean，无未提交文件 | origin/master | 同步（ahead 0 / behind 0） |
| /mnt/workspace/allgather_migrate/asc-devkit_4916 | allgather_migrate | 335f2e4f979697efa974e6640e7bc9deb7af2e12 | clean，无未提交文件 | 无（本地分支未设置 upstream） | N/A |

以上结论均来自 2026-10-12 读取的当前工作区 `git status`、`git branch -vv` 与 `git rev-parse`。目标分支当前提交与本地 master、origin/master 指向同一 commit，但 `allgather_migrate` 分支本身没有 tracking 配置。

### 用户输入与代码实际映射

| 项目 | 用户输入/代码事实 |
|---|---|
| opType | HCCL_CMD_ALLGATHER |
| algName | AicpuAllGatherSoleMeshConcur |
| engine | AICPU。源 selector 在 AICPU 分支返回该 algName；模板中按 algName 区分 AICPU SoleMeshConcur 与其他 Z-axis 使用方 |
| machine | 未显式输入。源注册 attrs 仅约束 topology，未约束 dev type，标记为 unknown |
| topology | 单层 MESH_1D 且两层网场景。源注册 attrs 为 `topo.maxTopoLevelNum = 1; topo.supportLevel0Topos = LEVEL0_TOPO_MESH_1D` |
| executor | InsV2AllGatherSoleExecutor |
| TopoMatch | 源仓 TopoMatchOneLevel；目标仓等价现用实现为 TopoMatch1D |
| template | InsTempAllGatherMesh1D1DZAxisDetour，单模板 |

源仓证据：

- src/ops/all_gather/algorithm/executor/ins_v2_all_gather_sole_executor.cc:388-392 注册 `AicpuAllGatherSoleMeshConcur -> InsV2AllGatherSoleExecutor + TopoMatchOneLevel + InsTempAllGatherMesh1D1DZAxisDetour`，attrs 限定单层 Mesh1D。
- src/ops/all_gather/selector/all_gather_auto_selector.cc:358-366 在单层 Mesh1D 且两层网条件下，超过 1GiB 总量或 2P 大于等于 4MiB 时选择该算法。
- src/ops/all_gather/algorithm/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc:105-174 实现资源计算、线程数、带宽分片和 channel 数更新。

### 候选算法、选定算法和默认兜底

- 候选算法：仅用户指定的 `AicpuAllGatherSoleMeshConcur`。目标仓 AllGather 已有 15 个本地可执行注册，非新原语族，不需要为新 opType 确认默认兜底。
- 选定算法：`AicpuAllGatherSoleMeshConcur`。
- 默认兜底：目标仓已有本地默认 Mesh 实现 `InsAllGatherMesh1D`。selector 修改后，未达到阈值时仍返回该算法，不引入跨原语兜底。
- forced-alg：目标仓 bare-name forced 配置会先查 `CollAlgExecRegistryV2`，再执行 `CheckForcedAlgResource`；缺失注册时拒绝并回退默认 selector。新增注册后 bare-name forced `AicpuAllGatherSoleMeshConcur` 才能闭环。

### HCCL 注册、依赖闭包、Host/AICPU 路径

| 依赖项 | HCCL 路径 | asc-devkit 状态 | 迁移动作 |
|---|---|---|---|
| executor | src/ops/all_gather/algorithm/executor/ins_v2_all_gather_sole_executor.cc | impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc 已存在，已适配 `mc2_ops_hccl`、TopoMatch1D、AlgMetaRegistry | 不新增文件；插入 EXEC 注册与 ALG_META 注册，include Z-axis 模板 |
| TopoMatch | src/ops/op_common/algorithm/topo_match/topo_match_one_level.cc | 目标仓无同名文件；已有 TopoMatch1D 并被同类 AllGather executor 使用 | 复用 TopoMatch1D，不迁移 HCCL 的 TopoMatchOneLevel |
| template | src/ops/all_gather/algorithm/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc/.h | 目标仓已有同名文件，但仍是旧固定 0.5 比例实现；已接 mc2_client 和 mc2_server 构建 | 修改同名模板，仅同步本次算法所需的源实现差异；不新增文件 |
| shared helper | src/ops/op_common/algorithm/template/template_utils.cc/.h | 目标仓 template_utils 只保留 `CalcDataSplitByPortGroupZAxisDetour`，没有源仓 2026-09 新增的 `CalcDataSplitByBandwidthZAxisDetour` | 在目标 template_utils.cc/.h 增加带宽分片 helper 及声明；这是共享文件，详见共享影响评估 |
| channel helper | src/ops/op_common/algorithm/executor/channel/channel.cc/.h | CalcChannelRequestMesh1DLevel0/Level1 已存在并被现有 Z-axis 模板使用；audit 发现目标缺少 HCCL 0c40a5e9 的按 Die 去重和 0c8fa8e7 的缺失网络层提前退出逻辑 | 初始计划复用；两轮 audit 修复后分别同步 ProcessLinkForProtocol 按 Die 去重和缺失层提前退出逻辑 |
| selector | src/ops/all_gather/selector/all_gather_auto_selector.cc | 目标仓已有 selector，单层 Mesh1D 目前固定返回 `InsAllGatherMesh1D` | 同步源分支阈值和 algName；使用目标仓已有的 `IsTwoLevelNetLayer(topoInfo)` 接口 |
| CMake | 源 executor/template CMake 已接线 | 目标 executor/template 目录 CMake 与根 mc2_server source list 均已接入 | 不修改 CMake；template_utils 本身已在目标构建源列表中 |
| Bridge | 源仓无目标仓 Bridge 概念 | `UseCannBridge` 只作用于 ALLTOALLV 和 ALLREDUCE，AllGather 不走 CANN Bridge | 不修改 Bridge |
| 测试 | 源仓未找到该 algName 的专属测试 | tests/api/adv_api/hccl/cc/st/algorithm/testcase/all_gather_aicpu_testcase.cc 已有 AllGather selector/注册/资源测试入口 | 增加注册、selector 阈值和资源计算断言 |

模板语义差异：

- 源实现 CalcRes 会校验 topoInfo 非空、channel 数非零，并检查 `GetRes` 返回值。
- 源实现按当前 peer 的实际 channel 数计算 level1 channel 数，并调用带宽权重分片函数；线程资源仍按最大 channel 数申请。
- 源实现的 `SetchannelsPerRank` 无条件把 level0 channel 数设为 `MESH_CHANNELS_NUM`，level1 数量为总 channel 数减 1。
- 目标仓现有实现保留旧固定 0.5 分片路径和成员，不能直接满足源算法当前语义，需要同步上述实现。
- 源模板的 `CalcCostCoeff` 属于 HCCL cost-model ABI，目标 executor 基类没有该接口；本次目标仓迁移不迁移该函数。这是接口适配差异，不是算法资源/执行语义缺失。

### asc-devkit 已有能力和缺失能力

已有：

- AllGather 本地 selector 与本地 V2 executor 注册表。
- sole executor、TopoMatch1D、基础 Mesh/NHR 模板、channel 请求 helper、Host CalcRes 与 AICPU Orchestrate 链路。
- Z-axis 模板文件已进入 mc2_client 和 mc2_server 构建。
- AllGather 不启用 CANN Bridge。

缺失：

- `AicpuAllGatherSoleMeshConcur` 本地 EXEC/ALG_META 注册。
- selector 在单层 Mesh1D 两层网大数据/2P 场景返回该 algName。
- 源仓带宽权重 Z-axis 分片 helper。
- 现有 Z-axis 模板与源仓当前实现的资源、分片和 channel 数语义同步。
- 该 algName 的本地注册/selector/资源测试断言。
- 外部算法白名单没有该 bare name；是否必须加入取决于是否允许 bare-name forced 直接选择它。若不加入，bare-name forced 仍会在注册表和资源检查中命中，但会被白名单过滤逻辑影响，详见待确认项 1。

### 计划新增/修改文件

计划修改：

1. impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
2. impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.h
3. impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
4. impl/adv_api/detail/hccl/cc/src/ops/all_gather/selector/all_gather_auto_selector.cc
5. impl/adv_api/detail/hccl/cc/src/ops/op_common/template/template_utils.cc
6. impl/adv_api/detail/hccl/cc/src/ops/op_common/template/template_utils.h
7. impl/adv_api/detail/hccl/cc/src/ops/op_common/external_alg/alg_whitelist.h（仅当确认允许外部/bare-name forced 集成）
8. tests/api/adv_api/hccl/cc/st/algorithm/testcase/all_gather_aicpu_testcase.cc
9. tests/api/adv_api/hccl/cc/st/algorithm/testcase/external_alg/external_alg_testcase.cc（仅当修改白名单或注册计数锚）

计划不新增源代码文件，不修改 CMake。原因：目标仓同名 executor/template 文件与构建接线已存在，新增会形成重复类/重复注册风险。

### 注册闭环

HCCL 当前注册：

- `HCCL_CMD_ALLGATHER -> AicpuAllGatherSoleMeshConcur -> InsV2AllGatherSoleExecutor -> TopoMatchOneLevel -> InsTempAllGatherMesh1D1DZAxisDetour`
- attrs：单层 Mesh1D。

asc-devkit 当前注册：

- AllGather V2 本地注册 15 个：InsAllGatherMesh1D、AicpuAllGatherSoleNHR、AicpuAllGatherSoleNHRMultiLink、CcuSchedAllGatherSoleMesh、CcuSchedAllGatherSoleNHR、InsAllGatherParallelMesh1DNHR/UBX/MultiJetty/Pcie、CcuSchedAllGatherParallelMeshNHRMultiLink、AicpuAllGatherSequenceMeshConcurNHR、InsAllGatherMeshNhrDPU、AicpuAllGatherConcurMeshNHR、CcuSchedAllGatherConcurMeshNHRMultiLink、AicpuAllGatherPipeLinePcie。
- 当前没有 `AicpuAllGatherSoleMeshConcur`。

迁移后计划注册：

- 在 sole executor 中插入 `REGISTER_EXEC_V2(HCCL_CMD_ALLGATHER, AicpuAllGatherSoleMeshConcur, InsV2AllGatherSoleExecutor, TopoMatch1D, InsTempAllGatherMesh1D1DZAxisDetour)`。
- 同步插入 `REGISTER_ALG_META(HCCL_CMD_ALLGATHER, AicpuAllGatherSoleMeshConcur, AlgEngine::AICPU, ...)`。外部名需见待确认项 1。
- 目标仓没有 HCCL `REGISTER_ALG_ATTRS` 机制；TopoMatch1D 是目标基线同类 Mesh executor 的现有拓扑匹配方式。计划不改 TopoMatch1D，避免改变既有 Mesh 算法。

selector：

- 当前单层 Mesh1D 固定 `InsAllGatherMesh1D`。
- 计划按源分支：`IsTwoLevelNetLayer(topoInfo) && (dataSize * userRankSize > 1GiB || (userRankSize == 2 && dataSize >= 4MiB))` 时返回 `AicpuAllGatherSoleMeshConcur`，否则保持 `InsAllGatherMesh1D`。
- 目标仓 `IsTwoLevelNetLayer` 签名为 `(const TopoInfoWithNetLayerDetails*)`，不接收 OpParam；RS selector 已按此接口使用，属于目标仓既有接口适配。

forced-alg：

- bare-name forced 配置会先进入 forced 名解析，再由注册表和 `CheckForcedAlgResource` 检查。
- 增加注册后，bare-name forced `AicpuAllGatherSoleMeshConcur` 可在本地注册表闭环；若白名单保持排除，则外部名/bare-name 生产过滤路径不会暴露它，见待确认项 1。

Host 资源查询：

- `HcclGetAlgRes -> executor->CalcAlgHierarchyInfo -> executor->CalcRes -> template->CalcRes`。
- 目标 sole executor 已在 CalcRes 中构造模板并调用模板 CalcRes。

AICPU 执行查询：

- AICPU kernel 入口从 `param.algName` 读取 algName，通过 `CollAlgExecRegistryV2::GetAlgExec` 重新查本地注册表，然后调用 `executor->Orchestrate`。
- 目标 sole executor 的 Orchestrate/OrchestrateLoop 会重新构造模板并调用 `KernelRun`。不传递 Host executor 对象跨环境。

### Host/AICPU 双侧路径

Host 侧：

1. ExecuteSelector 读取本地 AllGather selector。
2. selector 写入目标 algName。
3. HcclGetAlgRes 从本地 V2 registry 取 executor。
4. executor CalcAlgHierarchyInfo 使用 TopoMatch1D。
5. executor CalcRes 构造 InsTempAllGatherMesh1D1DZAxisDetour，并调用模板 CalcRes。
6. 模板 CalcRes 申请 level0+level1 merged channels，计算 slave thread、notify 和 channel 数。

AICPU 侧：

1. kernel 入口按 param.algName 从本地 V2 registry 取 executor。
2. executor Orchestrate 恢复资源上下文。
3. OrchestrateLoop 构造模板，按需调用 SetchannelsPerRank。
4. 模板 KernelRun 执行本地拷贝、线程同步、Z-axis 通道传输。
5. algName 通过 OpParam 字符串传递，不跨环境传递 Host executor 实例。

### 构建目标矩阵

| 目标 | 当前状态 | 迁移影响 |
|---|---|---|
| mc2_client Host | executor、selector、aicpu template、template_utils 均已由目录 CMake 接入 | 新增注册/selector 分支/模板实现自动进入；不新增 CMake |
| mc2_server Device | 根 CMake source list 已包含 sole executor、Z-axis template、template_utils，且定义 AICPU_COMPILE | 新增注册和模板实现自动进入；selector 不在 mc2_server |
| AllGather ST/UT | all_gather_aicpu_testcase.cc 已接入 AIC checker target；external_alg_testcase.cc 独立全量收集 executor/selector/template | 新增断言会进入既有 target；external_alg 目标因 GLOB 自动看到新增注册，但计数锚可能需要同步 |
| external_alg UT | 全量编译所有 executor/selector/template，检查 V2 registry 与 AlgMetaRegistry 满射和数量锚 | 新增 ALG_META 后需要同步 AG 注册计数、总 sidecar 数、候选键/组合计数；是否加入白名单影响生产口径断言 |

### 接口适配清单

- namespace：源 `ops_hccl`，目标 `mc2_ops_hccl`。
- include：源模板额外包含 cost_model.h；目标无对应 cost-model ABI，不迁移 CalcCostCoeff。
- TopoMatch：源 TopoMatchOneLevel，目标 TopoMatch1D，接口签名不同，按目标基类使用。
- template KernelRun：源模板依赖基类已有 KernelRun；目标 Z-axis 文件当前自行声明 KernelRun 及执行细节。迁移时以目标基类和现有执行框架为准，只同步本次算法必需语义。
- GetRes：目标 InsAlgTemplateBase/InsTempAllGatherMesh1D 提供 virtual GetRes；源模板通过继承的 GetRes 生成资源。计划让 Z-axis 模板调用 `CHK_RET(GetRes(...))`，保持源错误传播。
- IsTwoLevelNetLayer：源静态方法接收 topoInfo 与 opParam；目标成员方法只接收 topoInfo。按目标接口适配。
- 日志格式：源最新代码对 size_t 使用 %zu；目标沿用源格式。
- const/引用签名：CalcRes、GetRes、GetThreadNum、CalcDataSplitByPortGroup、SetchannelsPerRank 均保持目标基类声明签名，不改虚函数类型。
- 编译宏：目标 mc2_server 定义 AICPU_COMPILE；AICPU 注册不得放在 CCU 宏内。
- C++ 标准：mc2_client 和 AIC checker 为 C++14；external_alg UT 为 C++17。模板实现需保持 C++14 兼容。
- 类型表：不新增数据类型，继续使用 DATATYPE_SIZE_TABLE。
- wrapper 参数：模板现有 SendRecvRead/Write、DataSlice、ChannelInfo 接口已在目标仓存在，不改签名。

### 选择行为约束

源 selector 语义：

- 仅单层 Mesh1D 分支。
- 必须是两层网。
- 满足任一阈值：
  - `dataSize * userRankSize > 1GiB`
  - `userRankSize == 2 && dataSize >= 4MiB`
- 未满足时返回 `AicpuAllGatherSoleMesh`；目标对应本地默认算法为 `InsAllGatherMesh1D`。
- forced bare-name 可直接指定源 algName。

目标既有行为影响：

- 小数据/非两层网 Mesh1D 仍选择 `InsAllGatherMesh1D`，行为不变。
- 两层网大数据从 `InsAllGatherMesh1D` 变为 `AicpuAllGatherSoleMeshConcur`。这是本次迁移的目的性行为变化，仍限定在该分支。
- 2P 且 >=4MiB 也从默认 Mesh 变为目标算法，符合源语义。
- 未迁移候选算法不在本分支新增；不改变其他拓扑分支。

### 共享逻辑影响、风险和待确认项

共享文件影响：

1. template_utils.cc/.h：新增源仓带宽权重分片 helper。该 helper 只被 AG Z-axis 模板调用，目标仓当前无调用方，因此不影响现有算法；external_alg UT 会编译该公共文件。
2. sole executor：新增 AICPU 注册和 include。既有注册、CCU 宏块和 Orchestrate 行为不变。
3. selector：仅改单层 Mesh1D 两层网阈值分支；其他拓扑和 CCU 分支不变。
4. alg_whitelist.h（可选）：新增 1 个 bare name 会使数组长度从 39 到 40，并影响 external_alg 的生产口径数量断言。
5. all_gather_aicpu_testcase.cc / external_alg_testcase.cc：新增测试断言，不改变生产代码行为。

主要风险：

- 目标 Z-axis 模板当前实现比源仓多出一套执行路径。本次只同步源算法需要的资源/分片/channel 语义，必须防止顺手重构或改变其他模板行为。
- TopoMatch1D 与源 TopoMatchOneLevel 不是逐行等价：目标 TopoMatch1D 在 host 侧生成全 rank 层级，源 TopoMatchOneLevel 依赖物理层和 attrs。由于目标所有同类 Mesh 算法均使用 TopoMatch1D，且 AllGather 已有本地执行闭环，计划按目标既有模式复用。该差异需要 audit 重点复核。
- 目标 external_alg UT 有精确注册数量锚；新增注册后未同步测试锚会导致测试失败，但不影响生产代码。
- 未执行真实 NPU ST，无法在本阶段声明设备运行通过。

待确认项：

1. 是否把 `AicpuAllGatherSoleMeshConcur` 加入 alg_whitelist，从而允许外部名/bare-name 生产过滤路径选择它？
   - 代码事实：bare-name forced 会先通过注册表和资源检查；whitelist 只在 resolver 生产候选过滤路径中使用。若不加入，forced bare-name 路径仍可进入，但生产候选/死键断言不会包含它。
   - 默认建议：加入。理由是该算法将成为本地可执行注册，且 selector 可自动选中；不加入会使本地注册与生产可见集合不一致。但这会修改共享白名单并更新 39 个名称计数锚，需要用户确认。
2. externalName 应使用什么值？
   - 目标现有语义按 executor/layer 命名。该算法是 sole executor + Mesh + 并发 channel，源仓没有 externalName 机制。
   - 候选：`sole[mesh]` 与既有 InsAllGatherMesh1D 和 CCU Mesh 共键，需要规则条件区分；`sole[mesh.multi_channel]` 更能区分并发通道但现有 parser 属性表没有专门表达 Z-axis 两层网大数据。
   - 由于源仓没有 externalName 事实，无法由代码唯一确定。若用户确认加入白名单，需要同时确认 externalName/条件。若用户选择只做自动 selector 与 bare-name forced，则暂不注册 ALG_META 可行，但这会违反目标仓 V2 registry 与 AlgMetaRegistry 满射测试，因此不建议。

### 用户确认记录

已确认：

- 迁移算法：AicpuAllGatherSoleMeshConcur。
- 加入 alg_whitelist。
- ALG_META externalName：sole[mesh.multi_channel]，条件使用 COND_NONE、FLAG_NONE。
- 接受两层网大数据/2P 分支从既有本地 Mesh 默认算法切换到该算法。

### 阶段一交接结构

```text
repository_baseline:
  - path: /mnt/workspace/allgather_migrate/hccl
    branch_or_head: master
    commit: 95711ee1d33e53ebbd75e46e5dc6c49e29bbccd4
    working_tree: clean
    uncommitted_files: []
    tracking_remote: origin/master
    ahead_behind: 0/0
  - path: /mnt/workspace/allgather_migrate/asc-devkit_4916
    branch_or_head: allgather_migrate
    commit: 335f2e4f979697efa974e6640e7bc9deb7af2e12
    working_tree: clean
    uncommitted_files: []
    tracking_remote: none
    ahead_behind: N/A
opType: HCCL_CMD_ALLGATHER
candidate_algorithms:
  - AicpuAllGatherSoleMeshConcur
selected_algorithms:
  - AicpuAllGatherSoleMeshConcur
selected_families:
  - InsV2AllGatherSoleExecutor / InsTempAllGatherMesh1D1DZAxisDetour
required_shared_dependencies:
  - op_common/template/template_utils.cc
  - op_common/template/template_utils.h
  - all_gather/executor/ins_v2_all_gather_sole_executor.cc
  - all_gather/selector/all_gather_auto_selector.cc
default_fallback:
  algName: InsAllGatherMesh1D
  engine: AICPU
  machine: unknown
  topology: single-level MESH_1D
  confirmation: existing_local_default
approved_existing_behavior_changes:
  - AllGather single-level MESH_1D + two-layer network: data total > 1 GiB, or 2 ranks and data >= 4 MiB, select AicpuAllGatherSoleMeshConcur instead of InsAllGatherMesh1D
  - Add AicpuAllGatherSoleMeshConcur to the external-algorithm whitelist and register ALG_META externalName sole[mesh.multi_channel]
scope_status: READY_TO_MIGRATE
plan_document: docs/migration/aicpu-all-gather-sole-mesh-concur-migration-plan.md
```

## 迁移后实施结果

完成时间：2026-10-12 Asia/Shanghai。

### 实际修改文件

生产代码：

1. impl/adv_api/detail/hccl/cc/src/ops/op_common/template/template_utils.cc
2. impl/adv_api/detail/hccl/cc/src/ops/op_common/template/template_utils.h
3. impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
4. impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.h
5. impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
6. impl/adv_api/detail/hccl/cc/src/ops/all_gather/selector/all_gather_auto_selector.cc
7. impl/adv_api/detail/hccl/cc/src/ops/op_common/external_alg/alg_whitelist.h
8. impl/adv_api/detail/hccl/cc/src/ops/op_common/executor/channel/channel.cc
9. impl/adv_api/detail/hccl/cc/src/ops/op_common/executor/channel/channel.h

测试：

1. tests/api/adv_api/hccl/cc/st/algorithm/testcase/all_gather_aicpu_testcase.cc
2. tests/api/adv_api/hccl/cc/st/algorithm/testcase/external_alg/external_alg_testcase.cc

### 生产实现与注册

- template_utils：新增 HCCL 的 CalcDetourSplitCount 和 CalcDataSplitByBandwidthZAxisDetour，按 level0/level1 带宽权重分片，优先 4KiB 对齐，最后一个 channel 吸收剩余数据，并保留数据量、channel 布局和 u32 权重溢出检查。
- Z-axis AllGather 模板：CalcRes 增加 topoInfo 非空检查、channelsPerRank_ 非零检查并传播 GetRes 返回值；size_t 日志改用 %zu；CalcDataSplitByPortGroup 改为按 peer 实际 level1 channel 数调用带宽权重分片；SetchannelsPerRank 无条件设置 level0=MESH_CHANNELS_NUM、level1=总 channel 数-1；删除已无调用的 level0DataRatio_ 成员。目标仓现有 KernelRun/执行路径和 GetRes override 保持不变。
- executor：新增 AicpuAllGatherSoleMeshConcur 的 REGISTER_EXEC_V2，映射为 InsV2AllGatherSoleExecutor + TopoMatch1D + InsTempAllGatherMesh1D1DZAxisDetour；新增 REGISTER_ALG_META，engine=AICPU，externalName=sole[mesh.multi_channel]，COND_NONE、FLAG_NONE、priority=0。
- selector：恢复单层 MESH_1D 分支的两层网阈值。IsTwoLevelNetLayer(topoInfo) 为真且 dataSize*userRankSize>1GiB，或 userRankSize==2 且 dataSize>=4MiB 时选择 AicpuAllGatherSoleMeshConcur；否则保持 InsAllGatherMesh1D。目标仓 IsTwoLevelNetLayer 使用单 topoInfo 参数，这是目标接口适配。
- whitelist：AllGather 白名单加入 AicpuAllGatherSoleMeshConcur，ALG_WHITELIST 从 39 项增至 40 项。
- channel helper：同步 HCCL commit 0c40a5e9 的 ProcessLinkForProtocol 按 Die 去重逻辑。对匹配协议的多条链路，仅在每个 Die 首次出现时创建 channel；查询 Die 信息失败时保持 HCCL 现有降级行为，即不因 Die 查询失败阻断建链。该修复保证 Z-axis level0 每对端 channel 数符合带宽分片 helper 的布局假设。
- channel helper：同步 HCCL commit 0c8fa8e7 的缺失网络层处理逻辑。CheckNetLayerExists 通过输出参数向调用方报告 netLayer 是否存在；CalcChannelRequestMesh1DByLevel 在请求的 netLayer 不存在时直接返回 HCCL_SUCCESS，不再继续调用 HcclRankGraphGetLinks。该语义保护 Z-axis 绕路场景，特别是 1825 网卡拓扑删除 layer1 后的建链请求。
- 未迁移 HCCL CalcCostCoeff：目标 executor/template ABI 没有该 cost-model 接口，本次只保留资源计算与执行语义。
- 已迁移 TopoMatchOneLevel：第三轮 audit 发现复用 TopoMatch1D 与源仓语义不等价后，新增目标 ABI 专属实现并仅将 AicpuAllGatherSoleMeshConcur 注册切换到该匹配器。

### 构建、Bridge 与注册闭环

- 不修改 CMake。sole executor、Z-axis template 和 template_utils 均已在 Host 目录 CMake 与 Device 根 source list 中；selector 仅在 Host/UT selector 构建中，不进入 mc2_server。
- Host HcclGetAlgRes 按本地 V2 registry 获取 executor 并调用 CalcRes；AICPU KernelRun 链路按 param.algName 重新查本地 V2 registry 后调用 Orchestrate，不跨环境传递 executor 对象。
- AllGather 不启用 CANN Bridge，本次未修改 Bridge 文件，也未新增 ALLGATHER Bridge 路径。
- 新增 selector 返回名 AicpuAllGatherSoleMeshConcur 与本地 REGISTER_EXEC_V2 精确匹配；external_alg UT 的 V2 registry 与 AlgMetaRegistry 双向满射断言通过。

### 测试更新与验证

测试内容：

- all_gather_aicpu_testcase.cc：
  - 新增本地 V2 registry 注册断言。
  - 新增单层 MESH_1D + 两层网拓扑夹具。
  - 覆盖 16 rank 总量大于 1GiB、小数据回退 InsAllGatherMesh1D、2 rank 且 FP32 count=1MiB（4MiB）三种 selector 行为。
  - 新增资源计算测试：4 rank 下 6 条 channel、5 个 slave thread、主线程 notify 5、每从线程 notify 1。
  - 新增带宽分片测试：level0/level1 权重 3:3 时各 512 个 FP32 元素，均为 4KiB 对齐。
- external_alg_testcase.cc：
  - AllGather V2 tags 从 15 更新为 16，产品 sidecar 总数从 52 更新为 53。
  - 候选键从 31 更新为 32，engine/op/externalName 组合从 41 更新为 42。
  - 白名单和生产保留行从 39 更新为 40。
  - 加入白名单名称检查，并新增 sole[mesh.multi_channel] 的 resolver golden 与生产口径 large-data 解析断言。
  - 新增 ProcessLinkForProtocol 直接 UT：构造同 Die 两条链路和另一 Die 一条链路，断言最终只保留 2 条 channel，覆盖按 Die 去重行为。
  - 新增 MissingLevel1LayerSkipsLevel1Request：构造只有 layer0 的单机两 rank 模拟拓扑，断言 level0 正常返回 channel，level1 缺失时返回 HCCL_SUCCESS 且 channels 为空。

已执行检查：

- git diff --check：通过，无输出。
- cmake --build build --target ascendc_ut_hccl_cc_external_alg -j 8：按 Die 去重和缺失 layer1 提前退出修复后构建并运行通过，30/30 tests passed。
- cmake --build build --target ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC -j 8：构建并运行通过，82/82 tests passed。
- 目标 CMake 源接线静态检查通过：executor/template/template_utils 同时出现在根 Device source list 与目录 Host CMake；selector 只在 Host selector 构建中。
- include/namespace 检查通过：修改均位于 mc2_ops_hccl，executor 已 include Z-axis 模板头文件。
- override/签名复核通过：CalcRes、GetRes、GetThreadNum、CalcDataSplitByPortGroup、SetchannelsPerRank 的声明和定义均匹配目标基类/父类签名；未改动 KernelRun 虚签名。
- AllGather CANN Bridge 检查通过：未引入 Bridge 路径。

可选全组命令：

- bash build.sh --adv_test_two -j 8：失败于无关目标 ascendc_ut_adv_api_tiling_ascend610。环境 CANN runtime/metadef 为 9.1.0，仓库检查要求 9.2，且源码使用 DAV_5162 而当前 9.1 头文件只有 DAV_5102。该失败发生在与本次迁移无关的 tiling 目标；受影响的 external_alg 与 AllGather AIC 目标单独构建和运行均通过。

### 计划与实际差异

- 计划中的 CMake 修改不需要执行，因为目标文件和公共 template_utils 已全部接入现有构建。
- external_alg_full_testcase.cpp 未发现需要同步的精确数量锚，未做无关修改。
- 新增了迁移前计划未显列出的带宽分片直接 UT，用于固定 4KiB 对齐和权重分片行为。
- 初始实现未同步 channel helper 的按 Die 去重差异；hccl-audit-migration 审查发现该 P1 风险后，用户确认修复。实际修复同步 HCCL commit 0c40a5e9，并补充 direct UT。
- 第二次 hccl-audit-migration 复审发现 channel helper 仍缺少 HCCL commit 0c8fa8e7 的缺失网络层提前退出差异；用户确认修复后，实际同步该提交的 CheckNetLayerExists 输出语义和 level 缺失提前返回行为，并补充 direct UT。
- 第四次 hccl-audit-migration 复审通过后，将目标仓基线 P2（AllGather AIV selector 返回未注册的 AivAllGatherMesh1D）一并修复：HCCL 实际 AIV 注册名为 AivAllGatherSoleMesh，目标仓未迁移该 AIV 算法和模板，因此 AIV 分支返回 NOT_MATCH，避免选择结果落入本地未注册 algName。
- 未执行真实 NPU/device ST；UT 通过仅覆盖 Host/selector/registry/resource 和 stub 环境，不声明设备端性能或数据正确性通过。

### 修改后仓库状态与风险

- 源仓 hccl 保持 clean：master，commit 95711ee1d33e53ebbd75e46e5dc6c49e29bbccd4，tracking origin/master 同步。
- 目标仓 asc-devkit_4916 位于 allgather_migrate，基线 commit 335f2e4f979697efa974e6640e7bc9deb7af2e12；本次 11 个生产/测试文件已修改，迁移文档为新增未跟踪文件。
- 未提交、未推送。
- 主要遗留风险：未迁移 cost-model，目标侧不能通过 HCCL CalcCostCoeff 路径评估该算法成本。未执行 NPU/ST。

### audit 修复记录

- 审查时间：2026-10-12 Asia/Shanghai。
- 审查结论：NEEDS_MIGRATION_REPAIR，P1 问题为共享 channel.cc 未同步 HCCL ProcessLinkForProtocol 按 Die 去重逻辑，可能导致多 Die level0 链路 channel 数超过 Z-axis 分片假设并触发资源计算失败或语义漂移。
- 用户确认：修复。
- 实际修复：
  - channel.cc 同步 HCCL commit 0c40a5e9 的 seenDie 去重实现。
  - channel.h 声明 ProcessLinkForProtocol，供 direct UT 验证共享语义。
  - external_alg_testcase.cc 新增 LinksAreDeduplicatedByDie，覆盖同 Die 去重与异 Die 保留。
- 修复验证：
  - git diff --check 通过。
  - ascendc_ut_hccl_cc_external_alg：当时修复后 29/29 通过。
  - ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC：82/82 通过。
- 修复后仍建议再次运行 hccl-audit-migration 复审。

第二轮复审修复记录：

- 审查时间：2026-10-12 Asia/Shanghai。
- 审查结论：NEEDS_MIGRATION_REPAIR，P1 问题为共享 channel.cc 未同步 HCCL commit 0c8fa8e7 的缺失网络层处理逻辑。目标实现此前在 layer 不存在时仅 warning 后继续执行，仍会为不存在的 level1 调用 HcclRankGraphGetLinks。
- 用户确认：继续修复。
- 实际修复：
  - channel.cc 将 CheckNetLayerExists 增加布尔输出参数 netLayerExists，先置 false，仅当 rank graph layers 中存在请求层时置 true。
  - CalcChannelRequestMesh1DByLevel 在 netLayerExists 为 false 时直接返回 HCCL_SUCCESS，不再执行后续 rank 与链路查询。
  - channel.h 同步 CheckNetLayerExists 声明；AICPU 编译分支中的占位调用保持原有“不执行”语义。
  - external_alg_testcase.cc 新增 MissingLevel1LayerSkipsLevel1Request，覆盖只有 layer0 的拓扑下 level1 请求成功返回且不产生 channel。
- 修复验证：
  - git diff --check 通过。
  - ascendc_ut_hccl_cc_external_alg：30/30 通过。
  - ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC：82/82 通过。
- 修复后仍建议再次运行 hccl-audit-migration 复审。

第三轮复审修复记录：

- 审查时间：2026-10-13 Asia/Shanghai。
- 审查结论：NEEDS_MIGRATION_REPAIR，P1 问题为 AicpuAllGatherSoleMeshConcur 复用 TopoMatch1D，而源仓使用 TopoMatchOneLevel；两者在多物理层、真实 1DMESH 和 hostdpu 场景下语义不等价。
- 用户确认：继续修复。
- 实际修复：
  - 新增 op_common/topo/topo_match_one_level.h/.cc，按目标 TopoMatchBase 接口适配源仓单层匹配语义。
  - 匹配逻辑要求选中物理层 localRanks.size == userRankSize 且 topoType 为 COMM_TOPO_1DMESH；HOSTCPU/HOSTCPU_TS 只接受 HOST 层，非 hostdpu 排除 HOST 层；为 MeshConcur 填充 {Mesh 层, 上层超集层} 的 physicalIdxForAlgoLevels；无满 rank Mesh 层或无上层超集时返回 HCCL_E_NOT_SUPPORT。
  - AicpuAllGatherSoleMeshConcur 注册从 TopoMatch1D 切换到 TopoMatchOneLevel，其他 Mesh 算法继续使用 TopoMatch1D。
  - 新增实现接入 mc2_client 目录 CMake、mc2_server 根 source list、external_alg UT 与 AIC executor/kernel 测试库。
  - external_alg_testcase.cc 新增 5 个生产 TopoMatchOneLevel 用例，覆盖满 rank Mesh + 上层 CLOS 选择、Mesh 小于通信域拒绝、无上层拒绝、非 Mesh 拒绝和 hostdpu HOST 层过滤。
  - 目标 ABI 无 COMM_PROTOCOL_UBG 枚举，本实现不迁移源仓 AIV UBG 过滤分支；本算法为 AICPU 路径，当前注册和 executor 均使用 AICPU_TS 语义。
- 修复验证：
  - git diff --check 通过。
  - ascendc_ut_hccl_cc_external_alg：35/35 通过（原 30 个用例 + 新增 5 个拓扑匹配用例）。
  - ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC：82/82 通过。
- 修复后仍建议再次运行 hccl-audit-migration 复审。

P2 修复记录：

- 修复时间：2026-10-13 Asia/Shanghai。
- 问题：目标仓基线 AllGatherAutoSelector::SelectAivAlgo 返回 AivAllGatherMesh1D，但目标仓没有该 algName 的本地注册。
- 范围确认：HCCL 当前实际 AIV AllGather 注册名为 AivAllGatherSoleMesh，AivAllGatherMesh1D 只是 AIV kernel 类名；目标仓没有 AllGather AIV 模板、AIV 参数存储和 launch 依赖闭包，完整迁移该 AIV 算法超出本次 AICPU 迁移范围。
- 实际修复：AIV 分支不再返回悬空 algName，改为返回 SelectorStatus::NOT_MATCH；AIV_ONLY 保持失败，AIV 可回退路径按 AutoSelectorBase 既有流程处理。
- 测试：all_gather_aicpu_testcase.cc 新增 AIV_ONLY 用例，断言选择失败且不覆盖调用方传入的 algName。
- 修复验证：
  - git diff --check 通过。
  - ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC：83/83 通过（新增 1 个 AIV selector 防线用例）。
  - ascendc_ut_hccl_cc_external_alg：35/35 通过。
- 后续如需启用 AIV AllGather，应单独迁移 HCCL 的 AivAllGatherSoleMesh 及 AIV template/kernel/launch 依赖闭包，而不是恢复 AivAllGatherMesh1D 字符串。

### audit 输入

- 用户确认：迁移 AicpuAllGatherSoleMeshConcur、加入白名单、externalName=sole[mesh.multi_channel]。
- 关键源提交：aab888c9（带宽分片与 4KiB 对齐）、5b634e9e（%zu 日志格式）、0c40a5e9（channel 按 Die 去重）、0c8fa8e7（channel 缺失网络层提前退出）、40c8207c（cost-model，未迁移）。
- 重点审查文件：executor 注册、selector 分支、Z-axis 模板、template_utils、alg_whitelist 与 external_alg 计数锚。

### PR 6596 前置同步记录

- 同步时间：2026-10-13 Asia/Shanghai。
- 同步来源：/mnt/workspace/z00940346/asc-devkit master 分支提交 9af4fc15c934fcdf7afc41f6ff0b08a441223321（add aicpu support 960），对应 https://gitcode.com/cann/asc-devkit/pull/6596。
- 同步目的：引入 AICPU 960/9201 前置支持，包括新增 DevType::DEV_TYPE_960，并将原有 950-only 检查扩展为同时接受 DEV_TYPE_950 或 DEV_TYPE_960。
- 应用结果：PR 全量 diff 共 141 个文件，使用 git apply --3way 干净应用，无冲突；PR 改动与本次 AllGather 迁移修改文件无重叠。
- 内容核对：目标 staged 的 141 个文件清单与源提交一致；按文件核对 PR 增量 hunks 一致。其中 kernel_utils_ceil_oom_que.h 和 kernel_utils_macros.h 的最终文件内容与源提交不同，原因是目标分支基线缺少源仓已有的 OOM 相关实现，3-way 应用只合入 PR 6596 的平台判断增量，未顺手带入源仓基线差异。
- 范围说明：仅同步 PR 6596 对应的 master 提交。源仓后续本地分支 aicpu-960-comm 中的额外 ReduceScatter 改动不属于该 PR，未同步。
- 同步后验证：
  - git diff --check：迁移改动通过。PR 6596 staged diff 报出 impl/utils/sys_macros_impl.h 中 8 处源提交自带的 CRLF 续行空白提示，未额外改写源 PR 内容。
  - ascendc_ut_hccl_cc_external_alg：35/35 通过。
  - ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC：83/83 通过。
  - ascendc_ut_adv_api_ccu_st：131/131 通过。
- 同步后状态：PR 6596 改动经 git apply --3way 进入 index（staged），原 AllGather 迁移改动保持工作区修改，新增迁移文件保持 untracked；仍未提交、未推送。

### upstream master rebase 记录

- Rebase 时间：2026-10-19 Asia/Shanghai。
- upstream 基准：cann/asc-devkit upstream/master，HEAD c9b7d4a1649837fcf9936b094fdf2aa5b580d86a（tensor api vec侧资料搬运补充）。
- 处理方式：先以临时提交 204e539aa 保存当时的迁移与 PR 6596 状态，并创建备份分支 backup/allgather-before-upstream-rebase；将分支基线重置到 upstream/master 后，只重新应用 AllGather 迁移相关差异。
- PR 6596 副本处理：不再保留当时手工同步的 141 个文件副本。upstream/master 已通过 53eb4c709（!6498 Ascend960DT 适配）和 a5e1e7815（!6754 MC2aicpu适配960）合入 960/9201 前置支持，包含 DevType::DEV_TYPE_960、950/960 双检查、topo/KFC/AICPU launch 等改动；保留手工副本会重复应用并产生大量冲突。
- upstream external_alg 机制变化：原 alg_whitelist.h 和 IsAlgAllowed 过滤机制已移除，resolver 改为“全量注册构建候选面”。AicpuAllGatherSoleMeshConcur 通过 REGISTER_ALG_META 注册 externalName=sole[mesh.multi_channel] 后自然进入候选表，无需白名单文件。
- 测试适配：external_alg_testcase.cc 在 upstream 新结构下补齐 channel/sim/topo includes，增加 GoldenAllEngines 的 sole[mesh.multi_channel] 解析断言，并将候选数量锚从 upstream 的 32/42 更新为 33/43。
- Rebase 后验证：
  - git diff --check 通过。
  - ascendc_ut_hccl_cc_external_alg：32/32 通过。
  - ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC：83/83 通过。
  - ascendc_ut_adv_api_ccu_st：131/131 通过。
- Rebase 后状态：allgather_migrate HEAD 为 upstream/master c9b7d4a16，迁移改动保持工作区未提交状态；备份分支 backup/allgather-before-upstream-rebase 保留 rebase 前完整状态。当前环境 CANN runtime/metadef 为 9.1.0，upstream 依赖检查要求 9.2，上述三个目标仍可构建运行；未执行真实 NPU/device ST。
