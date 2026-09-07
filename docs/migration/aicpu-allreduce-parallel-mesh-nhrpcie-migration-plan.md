# AICPU AllReduce Parallel Mesh NHR PCIe 迁移方案

## 迁移前方案

### repository_baseline

以下状态是在读取源码前分别对实际参与迁移的两个仓库采集的；本方案文件创建后，目标仓会因为新增本文件变为 dirty。

```yaml
repository_baseline:
  - path: /Users/wy/gitcode代码/asc-comm/asc-devkit
    branch_or_head: InsAllReduceParallelMesh1DNHRPcie_new
    commit: ab1401308777bd3762ecfbbaa71f2f2e3d2a1d4f
    working_tree: dirty_plan_and_migration
    uncommitted_files: []
    tracking_remote: upstream/master
    ahead_behind: 0/0
  - path: /Users/wy/gitcode代码/asc-comm/hccl
    branch_or_head: master
    commit: dfbba6aa641cac43fa638f1ae4f61441be10a31c
    working_tree: clean
    uncommitted_files: []
    tracking_remote: origin/master
    ahead_behind: 0/0
```

### 用户输入与代码实际映射

- 用户输入：`AicpuAllreduceParallelMeshNHRPcie`，原语为 AllReduce，目标引擎按 AICPU 迁移处理。
- 用户已确认本次只迁移历史 PCIe 专用 `InsAllReduceParallelMesh1DNHRPcie` 语义，并将目标注册名定为 `AicpuAllreduceParallelMeshNHRPcie`（保留用户指定大小写）；不迁移 generic、MultiJetty、Uboe 或 DPU 算法。
- 在当前 `hccl@dfbba6aa641cac43fa638f1ae4f61441be10a31c` 和目标 `asc-devkit@f11078c56e37db905631569a13e1e2de88b4a0db` 中，均没有这个大小写和后缀完全一致的注册名。
- 当前 HCCL 的实际注册名是 `AicpuAllReduceParallelMeshNHR`（注意 `Reduce` 的大小写以及没有 `Pcie` 后缀），位置为：
  `hccl/src/ops/all_reduce/algorithm/executor/ins_v2_all_reduce_parallel_executor.cc:1414-1425`。
  它注册到 `HCCL_CMD_ALLREDUCE`，executor 为四模板 `InsAllReduceParallelExecutor`，TopoMatch 为 `TopoMatchTwoLevel`，模板参数为：
  `InsTempReduceScatterMesh1D`、`InsTempReduceScatterNHR`、`InsTempAllGatherMesh1D`、`InsTempAllGatherNHR`。
- 当前 HCCL selector 在两级 `MESH_1D` 分支（`hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:456-460`）和 PCIe 混合的 `MESH_1D_CLOS` 非全连接分支（同文件 `:560-580`）都可能返回这个实际名称。PCIe 分支的并行/流水切换阈值是 `OMNI_PCIE_AR_DATA_SIZE = 32 MiB`。
- `AicpuAllReduceParallelMeshNHRMultiJetty` 是另一个独立注册项（同 executor 文件 `:1427-1440`），不属于本次候选算法。
- 历史代码中曾出现过 `InsAllReduceParallelMesh1DNHRPcie`，但它不是当前 HCCL master 的注册名。提交 `hccl@f4fa91d1` 的变更显示：旧 PCIe 专用注册被移除，当前 `AicpuAllReduceParallelMeshNHR` 注册统一承接该算法家族，同时将 TopoMatch 从旧的 `TopoMatchPcieMix`/旧分支组合切换为当前注册的 `TopoMatchTwoLevel` 并重新整理 alg attrs；因此这是“注册合并 + 拓扑/属性调整”，不是单纯字符串改名。

阶段一时存在两个解释；现按用户确认选择 PCIe-only 解释：

1. ~~按当前 HCCL 实际名称 `AicpuAllReduceParallelMeshNHR` 迁移~~（不选）；
2. 只迁移历史 PCIe-only `InsAllReduceParallelMesh1DNHRPcie` 语义，采用目标仓现有的 `TopoMatchPcieMix` 兼容范围，并注册为 `AicpuAllreduceParallelMeshNHRPcie`。

该选择已作为阶段二交接输入。

#### 历史 Parallel 注册的当前承接关系

`hccl@f4fa91d1` 的注册和 selector 变更显示，旧的四模板 Parallel 注册并非全部并入同一个名称：

| 历史名称 | 当前承接 | 结论 |
|---|---|---|
| `InsAllReduceParallelMesh1DNHRPcie` | `AicpuAllReduceParallelMeshNHR`（PCIe-mix 非全连接分支） | 合并到 generic，范围 2 是范围 1 的历史 PCIe 子集 |
| `InsAllReduceParallelRSAGUboe` | `AicpuAllReduceParallelMeshNHR`（对称三层 Uboe selector 分支，当前 `:441`） | selector 名称并入 generic，但拓扑/属性语义已随新 TopoMatch 调整 |
| `InsAllReduceParallelRSAGUBX` | `AicpuAllReduceParallelMeshNHRMultiJetty`（当前 selector `:508`） | 未并入 generic，仍是独立注册名/分支 |
| `InsAllReduceParallelRSAGDpu` | `DpuAllReduceSequenceMeshNHR` 或 `DpuAllReducePipeLineMeshNHRNHR`（当前 DPU selector `:702-720`） | 未并入 AICPU generic，属于 DPU 路径，不在本次 AICPU 迁移范围 |

这些历史项曾复用同一个 `InsAllReduceParallelExecutor` 四模板结构，但“复用 executor”不等于“都合并为 `AicpuAllReduceParallelMeshNHR`”。

### 候选算法、选定算法和默认兜底

```yaml
opType: HCCL_CMD_ALLREDUCE
candidate_algorithms:
  - name: AicpuAllreduceParallelMeshNHRPcie
    source_fact: user-confirmed target name for historical InsAllReduceParallelMesh1DNHRPcie
    scope: PCIe-mix Mesh_1D_CLOS non-fully-connected AICPU path only
selected_algorithms:
  - AicpuAllreduceParallelMeshNHRPcie
selected_families:
  - InsAllReduceParallelExecutor (four-template Parallel AllReduce)
default_fallback:
  algName: AicpuAllReduceSoleMeshOneShot
  engine: AICPU/AICPU_TS local path
  machine: unknown; selector is topology-dependent
  topology: existing fully-connected PCIe-mix Mesh_1D_CLOS and small-data local path
  confirmation: existing_local_default
scope_status: READY_TO_MIGRATE
```

目标仓并非完全没有 AllReduce 执行能力：`asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc:251-271` 已注册 `AicpuAllReduceSoleMeshTwoShot`、`AicpuAllReduceSoleMeshOneShot`、`AicpuAllReduceSoleNHRAicpuReduce` 和 `AicpuAllReduceSoleMeshChunkTwoShot`。但它没有本 Parallel executor 的注册；目标 selector 当前只覆盖 `MESH_1D` 和 PCIe-mix **全连接**分支，非全连接 PCIe-mix 返回 `NOT_MATCH`（`asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:41-78`）。这里的默认兜底是目标仓已有本地 AllReduce 能力，不是跨原语借用。

### HCCL 注册、依赖闭包、Host/AICPU 路径

#### HCCL 当前注册和依赖闭包

| 依赖项 | 角色 | HCCL 路径 | 目标仓状态 | 迁移计划（确认后） |
|---|---|---|---|---|
| `InsAllReduceParallelExecutor` | 四模板 AllReduce 编排、资源合并、四阶段流水 | `hccl/src/ops/all_reduce/algorithm/executor/ins_v2_all_reduce_parallel_executor.{h,cc}` | 已新增 | 按目标 `InsCollAlgBase` 和目标 AICPU 接口完成适配 |
| `TopoMatchPcieMix` | 历史 PCIe-only 算法的两层（Mesh/Clos）rank 生成器 | 历史 HCCL 注册及目标实现 | 目标仓已有 | 本次按用户确认使用 `TopoMatchPcieMix`；不迁移 generic 的 `TopoMatchTwoLevel` |
| T0 `InsTempReduceScatterMesh1D` | 框内 RS | `hccl/src/ops/reduce_scatter/algorithm/template/aicpu/ins_temp_reduce_scatter_mesh_1D.{h,cc}` | 同名目标模板已有，namespace/接口不同 | 复用并按目标接口核对 |
| T1 `InsTempReduceScatterNHR` | 框间 RS | `hccl/src/ops/reduce_scatter/algorithm/template/aicpu/ins_temp_reduce_scatter_nhr.{h,cc}` | 同名目标模板已有，namespace/接口不同 | 复用并按目标接口核对 |
| T2 `InsTempAllGatherMesh1D` | 框内 AG | `hccl/src/ops/all_gather/algorithm/template/aicpu/ins_temp_all_gather_mesh_1D.{h,cc}` | 同名目标模板已有，namespace/接口不同 | 复用并按目标接口核对 |
| T3 `InsTempAllGatherNHR` | 框间 AG | `hccl/src/ops/all_gather/algorithm/template/aicpu/ins_temp_all_gather_nhr.{h,cc}` | 同名目标模板已有，namespace/接口不同 | 复用并按目标接口核对 |
| executor CMake | Host 源文件接入 | `hccl/src/ops/all_reduce/algorithm/executor/CMakeLists.txt:11-17` | 目标 `.../all_reduce/executor/CMakeLists.txt:11-17` 已有 sole/sequence，但无 parallel source | 增加目标 executor 接线 |
| template/root CMake | AICPU template 和 mc2_server 接线 | HCCL `src/CMakeLists.txt`、RS/AG template CMake | 目标 `impl/.../cc/src/CMakeLists.txt:154-198` 已接入现有 RS/AG templates，但没有 parallel executor | 增加缺失 executor，核对 template 源是否直接复用 |
| 注册宏 | `opType -> algName -> executor -> TopoMatch -> T0..T3` 闭环 | `REGISTER_EXECUTOR_BY_FOUR_TEMPS`，见 `:1414-1416` | 目标 registry 支持四模板宏（`.../registry/coll_alg_v2_exec_registry.h:98-115`） | 只注册确认后的 AICPU 名称，不带入 CCU/AIV/DPU |

#### Host/AICPU 双侧路径

- HCCL Host 资源流程在 `hccl/src/ops/op_common/op_common.cc` 中按 algName 获取 executor，计算层级信息和 `CalcRes`；AICPU kernel 侧按 `param.algName` 再从本地 registry 获取 executor 并调用 `Orchestrate`。迁移必须保持 Host/AICPU 各自本地查表，不能传递 Host executor 对象。
- 目标 registry 的四模板注册宏会把 `HCCL_CMD_ALLREDUCE`、名称和 `InsAllReduceParallelExecutor<...>` 绑定到 `CollAlgExecRegistryV2`；目标 `op_common.cc:413-426, 839-861` 已明确区分 CANN Bridge 查表和本地 registry 查表。
- 目标当前 `UseCannBridge` 对 AICPU/AICPU_TS 的 ALLREDUCE 返回 true（`asc-devkit/.../cann_host_bridge.cc:142-147`），资源路径和执行路径也将 ALLREDUCE 送入 CANN（`.../op_common.cc:413-419, 844-860`；AICPU decoupler `.../comm_kfc_open_kernel_executor.cc:181-186`）。若要实现完全脱离 CANN 的本地 forced/normal 执行，需修改这些共享 Bridge 分流；这会影响已有 AllReduce 行为，必须单独确认。

### asc-devkit 已有能力和缺失能力

- 已有：AllReduce sole AICPU executor/registration、AICPU RS/AG 四个同名模板、`TopoMatchMultilevel`、`TopoMatchPcieMix`、四模板注册宏、基础 selector/本地 registry、AICPU task launch。
- 缺失：本 Parallel AllReduce executor、当前 HCCL 注册所用的 `TopoMatchTwoLevel`、Parallel algName 的本地注册、PCIe-mix 非全连接分支的本地 selector 命中、完全脱离 CANN 的 ALLREDUCE Bridge 分流。
- 目标 `InsCollAlgBase`（`.../executor_v2_base.h:29-61`）提供 `CalcAlgHierarchyInfo`、`CalcAlgHierarchyInfoV2`、`CalcRes`、`Orchestrate` 和 ABI placeholder，但没有 HCCL 当前 executor 直接使用的具体 `CalcCostCoeff`/`GetAlgNetMeta` 虚函数；迁移时不能把 HCCL 的 override 原样带入。
- 目标 `InsAlgTemplateBase`（`.../template/alg_v2_template_base.h:31-60`）的 `KernelRun` 使用 `const TemplateResource&`，且基类提供 `SetchannelsPerRank`；HCCL 当前四模板头文件的 `KernelRun` 使用非 const `TemplateResource&`。这是必须逐项适配并重新核对声明、定义、调用点的接口差异。
- 目标 namespace 为 `mc2_ops_hccl`，HCCL namespace 为 `ops_hccl`；include 路径和 CMake 目标也不同。

### Parallel 四阶段语义基线

以 HCCL 当前 `InsAllReduceParallelExecutor` 为源语义，确认后应保持以下结构：

1. T0 框内 Mesh1D RS 与 T1 框间 NHR RS 的 part0/part1 并行阶段。
2. T1 框间 RS 与 T0 框内 RS 的互换阶段。
3. T1 框间 AG 与 T2 框内 Mesh1D AG 的互换阶段。
4. T2 框内 AG 与 T3 框间 NHR AG 的尾阶段。

实际源码按四模板的 `RunTemplate*`、BufferType、offset/scratch、零长度跳过、thread/channel/notify 进行逐段核对；不能把复制过程中的 offset 或同步改动当作顺手修复。

当前 HCCL `Orchestrate` 在恢复 channel map 后要求至少两层且两层非空（`hccl/.../ins_v2_all_reduce_parallel_executor.cc:532-565`），并在 AICPU_TS 下对 inter templates 调用 `SetchannelsPerRank`（`:571-579`）。目标模板/基类的 const 和 channel 接口不同，需按目标声明重写调用，不以文件名推断兼容性。

### Host/AICPU 双侧路径和 Bridge 影响

| 路径 | 当前事实 | 迁移影响 |
|---|---|---|
| Host selector | HCCL 将 `AicpuAllReduceParallelMeshNHR` 返回给两级 Mesh 和 PCIe-mix `<32 MiB` 分支；目标 selector 只返回 sole 算法 | 若选择 current generic 语义，必须新增目标 Host selector 分支并确保每个返回名均已注册 |
| forced-alg | 目标执行入口会按 algName 查询 CANN 或本地 registry；当前 ALLREDUCE 因 Bridge 优先走 CANN | 若要 forced 本地执行，需明确修改 Bridge 分流及查表顺序 |
| Host resource | CANN Bridge 下使用 `CalcAlgHierarchyInfoV2/GetAlgoMeta`；本地路径使用 `CalcAlgHierarchyInfo` | 新 executor 需同时满足目标 Host 资源接口和本地 registry 闭环 |
| AICPU execute | decoupler 对 ALLREDUCE 直接调用 CANN launch | 需确认是否移除 ALLREDUCE 的 CANN 执行分支，避免 selector 命中本地但执行仍落 CANN |

### 构建目标矩阵和测试影响

- Host/mc2_client：目标 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/CMakeLists.txt` 和根 `.../cc/src/CMakeLists.txt` 需要接入 parallel executor；selector 只接入 Host。
- Device/mc2_server：目标根 CMake 的 RS/AG AICPU template 源已存在（`.../cc/src/CMakeLists.txt:162-187`），需确认新增 executor 是否由同一 target 编译，不能把目录级 CMake 当作根 target 接线证明。
- HCCL 当前有 Parallel ST：`hccl/test/st/algorithm/testcase/all_reduce_parallel_testcase.cc`，覆盖基础、奇偶/尾片、small/mid/大数据和 asymmetric topology；目标 `asc-devkit` 当前未找到对应 Parallel ST 文件或测试 target 接线。
- 迁移后至少需要在目标 ST 中证明四模板均执行、两层 channel 顺序、尾片/零长度和结果正确；本阶段不修改测试代码。
- HCCL 中的 CCU registrations（同 executor 文件 `:1451-1461`）和任何 AIV/DPU/非 AICPU template 不在本次迁移范围。

### 接口适配清单

- namespace：`ops_hccl` -> `mc2_ops_hccl`。
- include：HCCL `src/ops/...` -> target `impl/adv_api/detail/hccl/cc/src/ops/...`。
- executor base：核对 `CalcAlgHierarchyInfo`、`CalcAlgHierarchyInfoV2`、`CalcRes`、`Orchestrate`；不原样迁移 HCCL 的 CostModel/NetMeta override，除非目标 ABI 明确需要。
- template base：四个 `KernelRun` 的 `TemplateResource` const、`SetchannelsPerRank` 基类行为、`CalcRes/GetRes`、notify/channel 映射逐项核对。
- topology：当前 HCCL `TopoMatchTwoLevel` 在目标缺失；`TopoMatchMultilevel` 与 `TopoMatchPcieMix` 的适用拓扑不同，必须根据用户选择的 generic/PCIe-only 语义确认后再定。
- 编译宏：保留 AICPU 路径的 `AICPU_COMPILE` 边界；不将 `#ifndef AICPU_COMPILE` 下的 CCU include/registration 带入 device。
- C++/ABI：保持目标仓现有 C++ 标准、`ThreadHandle`/`TemplateResource`/`TemplateDataParams` wrapper 参数，不跨仓复制旧 ABI。

### 选择行为约束、未迁移候选和风险

- 当前 HCCL 的数据量条件只作为行为约束记录：标准两级 Mesh 中 `dataSize > AR_AICPU_1D_CROSS_SMALL_DATA_SIZE` 后选择 parallel，超过 sequence threshold 转 sequence；PCIe-mix 非全连接中 `< 32 MiB` 选择 parallel，`>= 32 MiB` 选择 pipeline。
- 不能把 selector 的阈值当作候选算法排序，也不能在目标 Host 侧伪造运行时数据条件。
- `AicpuAllReduceParallelMeshNHRMultiJetty`、`AicpuAllReducePipeLineMeshNHR`、sequence/sole 等未迁移候选必须保持已有分支语义，不能因新增 parallel 注册而改变它们。
- 主要风险：用户输入与当前注册名不一致；target 没有 `TopoMatchTwoLevel`；target 当前 ALLREDUCE Bridge 会绕过本地 executor；target 无 parallel ST；模板/基类 const 和 channel 接口存在差异；修改 selector/Bridge 会影响已有 AllReduce。

### 计划新增/修改文件（用户确认后执行）

预期范围，不代表本阶段已修改：

1. 新增目标 AllReduce Parallel executor 的 `.h/.cc`（从 HCCL 四模板 executor 迁移并做必要接口适配）。
2. 必要时新增或适配目标 topology match glue；是否使用 `TopoMatchMultilevel` 或 `TopoMatchPcieMix` 待确认。
3. 修改目标 AllReduce executor/root CMake 接线，以及必要的 RS/AG template CMake/根 source list。
4. 修改目标 registry 注册确认后的 algName；仅保留 AICPU 注册。
5. 若批准正常选择，修改目标 Host AllReduce selector；若批准完全脱离 CANN，修改 Host/AICPU ALLREDUCE Bridge 分流。
6. 新增/接线目标 Parallel ST，覆盖四模板和两层 channel 语义。
7. 在本文件追加“迁移后实施结果”。

### 共享逻辑影响和待确认项

`approved_existing_behavior_changes` 当前为空。以下任一项都必须先得到用户确认：

1. **算法映射**：本次是否按当前 HCCL 的 `AicpuAllReduceParallelMeshNHR` 迁移，还是只按历史 `InsAllReduceParallelMesh1DNHRPcie` 的 PCIe-only 范围迁移？
2. **selector**：是否允许把目标 `MESH_1D_CLOS + level0PcieMix + 非全连接` 的 `<32 MiB` 分支改为本地 Parallel 算法，并保留 `>=32 MiB` 的 pipeline/其他分支？
3. **Bridge**：是否允许从目标 Host `UseCannBridge`、资源查询和 AICPU decoupler 中移除 ALLREDUCE 的 CANN 路径，使 forced-alg 和正常选择真正走 asc-devkit 本地 registry？若只想迁移 forced-alg，请明确不改 selector/Bridge。
4. **TopoMatch**：generic current 语义需要目标如何承接 HCCL `TopoMatchTwoLevel`；历史 PCIe-only 语义是否确认使用目标已有 `TopoMatchPcieMix`？

### 用户确认记录

```yaml
confirmed_by_user: true
confirmed_scope:
  - historical InsAllReduceParallelMesh1DNHRPcie implementation semantics only
  - target registration name AicpuAllreduceParallelMeshNHRPcie
  - exclude generic AicpuAllReduceParallelMeshNHR, MultiJetty, Uboe and DPU variants
approved_existing_behavior_changes:
  - PCIe-only selector branch may return AicpuAllreduceParallelMeshNHRPcie
  - confirmed PCIe-only ALLREDUCE path may bypass CANN Bridge for local execution
confirmation_notes: "范围控制在 InsAllReduceParallelMesh1DNHRPcie，并改名成 AicpuAllreduceParallelMeshNHRPcie。"
```

状态：`READY_TO_MIGRATE`

```yaml
opType: HCCL_CMD_ALLREDUCE
candidate_algorithms:
  - InsAllReduceParallelMesh1DNHRPcie
selected_algorithms:
  - AicpuAllreduceParallelMeshNHRPcie
selected_families:
  - InsAllReduceParallelExecutor
required_shared_dependencies:
  - four AICPU RS/AG templates
  - topology match adaptation
  - AllReduce executor and registry
  - Host selector (if normal selection is approved)
  - Host/AICPU ALLREDUCE Bridge split (if CANN decoupling is approved)
  - Host/mc2_server CMake
  - Parallel ST
default_fallback:
  algName: AicpuAllReduceSoleMeshOneShot
  engine: AICPU/AICPU_TS
  machine: unknown
  topology: existing local fully-connected Mesh_1D_CLOS/small-data path
  confirmation: existing_local_default
approved_existing_behavior_changes:
  - rename the migrated PCIe-only registration to AicpuAllreduceParallelMeshNHRPcie
  - change only the confirmed PCIe-only selector/Bridge path as required for local execution
scope_status: READY_TO_MIGRATE
plan_document: /Users/wy/gitcode代码/asc-comm/asc-devkit/docs/migration/aicpu-allreduce-parallel-mesh-nhrpcie-migration-plan.md
```

## 迁移后实施结果

### 实施摘要

- 实施时目标仓已从阶段一记录的旧基线快进到 `asc-devkit@0290f560c82a867528b8ecbdb1a20366a6760a64`，源仓保持 `hccl@dfbba6aa641cac43fa638f1ae4f61441be10a31c`。
- 本次只迁移历史 PCIe-only `InsAllReduceParallelMesh1DNHRPcie` 语义，目标注册名落为 `AicpuAllreduceParallelMeshNHRPcie`。
- 未迁移 generic `AicpuAllReduceParallelMeshNHR`、MultiJetty、Uboe、DPU 变体。
- 由于当前目标仓 AllReduce 仍默认经 CANN Bridge，本次把 Bridge 影响收敛到“仅新注册名本地执行”，没有把所有 ALLREDUCE 全局切走。

### 实际修改/新增文件清单

- 新增执行器：
  - `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.h`
  - `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.cc`
- 新增/调整选择与桥接：
  - `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`
  - `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.h`
  - `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc`
  - `impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/execute_selector.cc`
  - `impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_open_kernel_executor.cc`
- 新增/调整资源与配置共享逻辑：
  - `impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc`
  - `impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc`
  - `impl/adv_api/detail/hccl/cc/src/common/alg_env_config.h`
  - `impl/adv_api/detail/hccl/cc/src/common/alg_env_config.cc`
  - `impl/adv_api/detail/hccl/cc/src/ops/op_common/template/template_utils.h`
  - `impl/adv_api/detail/hccl/cc/src/ops/op_common/template/template_utils.cc`
  - `include/adv_api/hccl/internal/hcomm/hccl/hccl_comm.h`
- 新增/调整构建与测试接线：
  - `impl/adv_api/detail/hccl/cc/src/CMakeLists.txt`
  - `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/CMakeLists.txt`

### 每个文件的关键改动

- `ins_v2_all_reduce_parallel_executor.{h,cc}`：以 `TopoMatchPcieMix` 和四模板 `InsTempReduceScatterMesh1D/InsTempReduceScatterNHR/InsTempAllGatherMesh1D/InsTempAllGatherNHR` 迁移历史 Parallel 编排，注册名改为 `AicpuAllreduceParallelMeshNHRPcie`。
- `all_reduce_auto_selector.cc`：在 `MESH_1D_CLOS + level0PcieMix + 非全连接 + 非 special + <32MiB` 分支返回新算法名；大数据和 special 保持既有逻辑，并由 `ExecuteSelector` 走 CANN 回退。
- `cann_host_bridge.{h,cc}`：只对新算法名关闭 ALLREDUCE 的 CANN Bridge；其他 ALLREDUCE 仍保持原有桥接路径。
- `comm_kfc_open_kernel_executor.cc`：AICPU decoupler 侧也只对新算法名跳过 CANN，避免把其他 AllReduce 一起改掉。
- `execute_selector.cc`：先尝试本地 selector；对 PCIe 专用分支在本地 selector 不命中时回退 CANN，避免 32MiB 边界和 special 数据进入未迁移本地执行器。
- `op_common.cc`：在资源准备前回填最终 `param.algName`，确保 forced/local 路径能按新名字查本地 registry；同时接入多维切分比例配置。
- `hccl_alloc_ctx_res.cc`：在资源准备阶段提前填充 `algName`，让本地 registry/CANN 分流可见最终算法名。
- `alg_env_config.{cc,h}`、`template_utils.{cc,h}`、`hccl_comm.h`：补齐多维切分比例的环境变量、comm config 和公式计算共享逻辑。
- 本次按用户要求回退测试接线，没有保留独立 testcase；仅保留业务代码和构建接线。

### Host/AICPU 构建目标影响

- `mc2_client`：新增 parallel executor 源文件接线。
- `mc2_server`：根 CMake 也接入同一 executor 源文件，保证 Host/AICPU 两侧都能编译到。
- ST：本次未新增独立 testcase，也未保留测试侧接线。

### 迁移前计划与实际差异

- 计划里 generic `AicpuAllReduceParallelMeshNHR` 也曾是候选；最终按用户确认只落 PCIe-only `InsAllReduceParallelMesh1DNHRPcie` 语义。
- 计划里曾考虑把 ALLREDUCE Bridge 全局移除；实际只把新算法名从 Bridge 中剥离，保留其他 AllReduce 的 CANN 路径。
- 计划里目标 selector 只会返回本地可执行算法；实际保留了大数据/special 的 CANN 回退，不让未迁移本地执行器接管它们。
- 目标仓没有现成 Parallel ST，所以本次不新增独立 ST testcase；测试相关接线已按用户要求回退。

### 常规检查与构建结果

- 已做静态检查：
  - 反查新注册名 `AicpuAllreduceParallelMeshNHRPcie`
  - 反查旧名 `InsAllReduceParallelMesh1DNHRPcie`
  - 确认 CMake 已接入新 executor 和 ST
  - 确认 Bridge 仅对新算法名生效
  - 其中 ST testcase 和测试侧接线已按用户要求回退，不再作为本次交付内容。
- 未做完整构建/运行：
  - 当前环境没有可直接使用的 `cmake`
  - 也没有 CANN/硬件运行环境可做 ST 设备验证

### 未完成项、遗留风险和 audit 输入

- 这次迁移仍是“代码接线完成，运行未验证”。
- 关键风险是大数据和 special 数据走 CANN 回退的实际行为需要后续设备侧确认。
- 需要交给 `hccl-audit-migration` 继续审查的重点：
  - 新旧注册名是否完全闭环
  - `UseCannBridge` 和 decoupler 是否只影响新算法名
  - 选择器回退是否会误伤既有 AllReduce
  - 共享切分比例逻辑是否和已有算法共用时存在副作用
