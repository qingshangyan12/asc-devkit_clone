# AICPU AllReduce Parallel Mesh1D NHR PCIe Migration Plan

## 迁移前方案

### repository_baseline

- source repository: `/Users/wy/gitcode代码/asc-comm/hccl`
  - branch_or_head: `master`
  - commit: `a5101c3161e975df635df46e296182ca6143340b`
  - working_tree: dirty
  - uncommitted_files: `.DS_Store`, `src/.DS_Store` (untracked only; not used as source evidence)
  - tracking_remote: `origin/master`
  - ahead_behind: `0/0`
- target repository: `/Users/wy/gitcode代码/asc-comm/asc-devkit`
  - branch_or_head: `all_reduce_parallel_new`
  - commit: `dd293a1c6b873d149bf289bb2b8098dadf9f5cbd`
  - working_tree: clean
  - uncommitted_files: none
  - tracking_remote: `upstream/master`
  - ahead_behind: `0/0`

All conclusions in this document are based on the repositories, branches and commits above. The two
untracked `.DS_Store` files in `hccl` are not treated as source changes.

### 用户输入与代码实际映射

用户输入：`AicpuAllReduceParallelMeshNHPice`。

当前 HCCL 注册表中没有这个精确名称。代码实际对应的唯一 AICPU 注册项是：

```text
opType: HCCL_CMD_ALLREDUCE
algName: InsAllReduceParallelMesh1DNHRPcie
executor: InsAllReduceParallelExecutor
TopoMatch: TopoMatchPcieMix
templates:
  InsTempReduceScatterMesh1D
  InsTempReduceScatterNHR
  InsTempAllGatherMesh1D
  InsTempAllGatherNHR
```

代码依据：

- `hccl/src/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.cc:1299-1301`
- `hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:567-573`

本方案暂按上述实际注册名迁移，不根据 `Aicpu`、`Ins` 或历史命名推断新的 algName。

### opType、engine、machine、topology

- opType: `HCCL_CMD_ALLREDUCE`
- engine: HCCL selector 进入 AICPU 分支；具体运行 engine 以调用侧 `OpParam.engine` 为准，当前代码未在
  注册项本身固定单一 engine，运行时值记为 `source-defined/unknown`
- machine: 由 `TopoMatchPcieMix` 和目标拓扑信息共同决定，具体芯片型号未在当前注册项中固定，记为 `unknown`
- topology: PCIe mix 拓扑；HCCL selector 的分支为 `MESH_1D_CLOS`、`level0PcieMix`、底层网络不是全连接
  `1DMESH`，并且 `dataSize < OMNI_PCIE_AR_DATA_SIZE`

### 候选算法、选定算法和默认兜底

当前 HCCL AllReduce AICPU selector 在 PCIe mix 分支中选择：

```text
dataSize < OMNI_PCIE_AR_DATA_SIZE:
    InsAllReduceParallelMesh1DNHRPcie
otherwise:
    AicpuAllReducePipeLinePcie
```

本次选定迁移：

```text
selected_algorithms:
  - InsAllReduceParallelMesh1DNHRPcie
selected_families:
  - InsAllReduceParallelExecutor
```

目标仓库当前 AllReduce selector 固定返回：

```text
AicpuAllReduceSoleMeshOneShot
```

该算法是目标仓已有的本地默认兜底。目标仓当前没有 `InsAllReduceParallelMesh1DNHRPcie` 的可执行注册。

### HCCL 注册、依赖闭包、Host/AICPU 路径

HCCL Parallel executor 是四模板 executor，不是只新增一个 `.cc` 文件。迁移闭包包括：

1. `ins_v2_all_reduce_parallel_executor.h/.cc`
2. AllReduce executor CMake 接线
3. AllGather AICPU templates：`InsTempAllGatherMesh1D`、`InsTempAllGatherNHR`
4. ReduceScatter AICPU templates：`InsTempReduceScatterMesh1D`、`InsTempReduceScatterNHR`
5. AllReduce selector 中的 PCIe-mix 选择分支
6. AllReduce 的 Host/AICPU 本地注册和按 algName 查表路径
7. AllReduce Bridge 排除逻辑
8. 必要的 AllReduce Parallel ST/CMake 接线

目标仓已存在第 3、4 项对应 template 及其独立 CMake 接线，但尚未存在 AllReduce Parallel executor。
这些公共 templates 不能重复迁移或重构，只需确认它们能被新 executor 在 `mc2_client` 目标中复用。

### Host/AICPU 双侧路径

HCCL 当前 executor：

- Host `CalcRes`：计算四个 template 的资源、线程、channel、notify 和 workspace
- AICPU `Orchestrate`：读取 `AlgResourceCtxSerializable`，恢复 channel map，实例化四个 template，生成通信指令
- Host/AICPU 两侧均通过 `algName` 查表，不能跨环境传递 Host executor 对象

目标仓现有 AllReduce executor 只注册 `InsV2AllReduceSoleExecutor`，selector 也只返回 OneShot。迁移后需要让
`InsAllReduceParallelMesh1DNHRPcie` 在 Host 资源计算和 AICPU Orchestrate 两侧都能命中同一个本地注册契约。

### 构建目标矩阵

| 目标 | 当前状态 | 迁移计划 |
| --- | --- | --- |
| `mc2_client` Host/resource | 目标已有 AllReduce executor、selector、四个公共 template 的部分链路 | 增加 Parallel executor；本次不改 selector |
| `mc2_server` Device/AICPU | 目标已有 AICPU KFC 框架和 AllReduce 执行入口 | 确认 executor/template 在 AICPU 编译宏下可用，不引入 CCU/AIV/DPU 路径 |
| AllReduce ST | 目标分支当前未发现该 Parallel 算法对应的完整本地 ST | 按现有 AllReduce/Parallel ST 结构补最小必要接线 |
| UT | 目标仓已有公共 selector/executor 测试结构 | 仅补目标算法需要的注册/选择覆盖，不扩大无关范围 |

### 接口适配清单

迁移前必须逐项对照目标仓现有基类和同类 Parallel executor：

- namespace：HCCL `ops_hccl` 到目标仓 `mc2_ops_hccl`
- include：目标仓 include 根和内部头文件路径
- `KernelRun`、`CalcRes`、`Orchestrate` 的完整签名，尤其是 `const TemplateResource&`
- `REGISTER_EXECUTOR_BY_FOUR_TEMPS` 宏及目标仓注册表类型
- `TopoMatchPcieMix` 在目标仓的可用性
- `TemplateDataParams`、`TemplateResource`、`AlgResourceCtxSerializable` 类型差异
- 编译宏：`AICPU_COMPILE`、`MC2_CLIENT_ENABLE_CCU` 等
- `mc2_client`、`mc2_server` 的 source list 和 include 闭包

### 选择行为约束

HCCL 当前行为约束必须保留：

- 仅在 PCIe mix 对应拓扑分支考虑该算法
- 目标底层网络不是全连接 `1DMESH`
- `dataSize < OMNI_PCIE_AR_DATA_SIZE` 时选择 Parallel PCIe 算法
- 更大数据量分支选择 `AicpuAllReducePipeLinePcie`
- 其他拓扑和特殊数据类型/规约类型继续按当前 selector 分支处理
- forced-alg 必须只能命中目标仓已有的真实本地注册

### 共享逻辑影响、风险和待确认项

已确认本次暂不接入 selector 选择逻辑：

- 不修改目标 `AllReduceAutoSelector::SelectAicpuAlgo` 的现有默认返回值。
- 不把 HCCL 的 PCIe-mix 数据量/拓扑分支复制到目标 selector。
- 本阶段只建立 `InsAllReduceParallelMesh1DNHRPcie` 的本地注册和 forced-alg 执行闭环。
- 该算法不会因为正常 selector 自动命中；正常选择仍保持目标仓当前行为。

如果后续需要让正常 selector 命中该算法，再单独进行 upstream selector 同步评估。

已确认并允许的共享逻辑影响：

目标仓 `AllReduceAutoSelector::SelectAicpuAlgo` 当前无条件返回 `AicpuAllReduceSoleMeshOneShot`。接入 HCCL 的
PCIe-mix 选择分支后，在满足 PCIe mix 条件的 AllReduce 场景中，默认算法可能从 OneShot 改为
`InsAllReduceParallelMesh1DNHRPcie`；这是目标仓已有 `HCCL_CMD_ALLREDUCE` 选择行为的变化，不是纯新增注册。

目标仓原先在 Host `UseCannBridge` 和 AICPU `comm_kfc_open_kernel_executor.cc` 中都将
`HCCL_CMD_ALLREDUCE` 分流到 CANN。本次按用户确认，两侧均移除 AllReduce 分支；AllReduce 的 OneShot、Parallel
以及后续本地算法都统一走 devkit 本地 selector/注册表/执行路径。AllToAll 和 AllToAllV 的 CANN Bridge 行为保持不变。

### 状态

```text
scope_status: NEEDS_USER_CONFIRMATION
selected_algorithms: [InsAllReduceParallelMesh1DNHRPcie]
default_fallback:
  algName: AicpuAllReduceSoleMeshOneShot
  engine: AICPU path
  machine: unknown
  topology: target-local default
  confirmation: existing_local_default
approved_existing_behavior_changes:
  - selector: keep existing target selector behavior; do not add normal selection for this algorithm
  - bridge: remove HCCL_CMD_ALLREDUCE from Host and AICPU CANN Bridge conditions
```

## 用户确认记录

- 用户确认：本次不修改 selector 自动选择逻辑，保持目标仓当前 AllReduce 默认行为。
- 用户确认：允许将 `HCCL_CMD_ALLREDUCE` 从 CANN Bridge 排除，使 forced-alg 能进入本地 AICPU executor。

## 迁移后实施结果

### 实际修改/新增文件

- 新增 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.h`
  - 迁入四模板 `InsAllReduceParallelExecutor` 声明。
  - namespace 适配为目标仓 `mc2_ops_hccl`。
  - 保留 `remoteRankToChannelInfo_`、四模板资源准备、数据切分和指令生成状态。
- 新增 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.cc`
  - 复用目标仓已有 ReduceScatter Mesh1D/NHR 与 AllGather Mesh1D/NHR AICPU templates。
  - 注册 `HCCL_CMD_ALLREDUCE -> InsAllReduceParallelMesh1DNHRPcie -> InsAllReduceParallelExecutor`
    和 `TopoMatchPcieMix`，四模板顺序与 HCCL 当前注册一致。
  - 同步当前 HCCL 的空层级信息和非 PCIe CLOS 层级数量检查，避免资源计算阶段越界。
  - 去除历史 `CANN_VERSION_NUM` 注册门控，使该本地 AICPU 注册不依赖 CANN 版本宏。
- 修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/CMakeLists.txt`
  - 将新 executor 加入 `mc2_client`。
- 修改 `impl/adv_api/detail/hccl/cc/src/CMakeLists.txt`
  - 将新 executor 加入 `mc2_server` device source list。
- 修改 `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc`
  - `UseCannBridge` 不再对 `HCCL_CMD_ALLREDUCE` 返回 true。
- 修改 `impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_open_kernel_executor.cc`
  - AICPU 侧 `useCannBridge` 不再对 `HCCL_CMD_ALLREDUCE` 分流。

### 注册、选择和执行闭环

- 本次未修改 AllReduce selector；目标仓正常选择仍保持现有 `AicpuAllReduceSoleMeshOneShot` 行为。
- forced-alg 传入 `InsAllReduceParallelMesh1DNHRPcie` 时，Host `GetAlgExec` 可从本地注册表构造 Parallel executor，
  `CalcRes` 复用四个公共 AICPU template 计算资源。
- AICPU 侧收到相同 algName 后重新从本地注册表查表，进入 `Orchestrate`，恢复 channel map 并调用四个 template 的
  `KernelRun`；不依赖 Host executor 对象跨环境传递。
- Host 和 AICPU 两侧均已排除 AllReduce CANN Bridge；AllToAll/AllToAllV Bridge 行为未改。

### 计划与实际改动差异

- 按用户确认，未迁移 HCCL selector 的 PCIe mix 数据量/拓扑选择分支，因此正常 selector 暂时不会自动命中该算法。
- 未新增 template；四个依赖 template 在目标仓已存在并由现有 CMake 接入。
- 方案中原计划的 ST/UT 接线未新增：当前任务只建立 forced-alg 的实现闭环，完整运行验证另行处理。
- 为了完成完全脱离 CANN 的 AllReduce 执行闭环，实际同时修改了 Host Bridge 和 AICPU Bridge 两处条件。

### 静态检查、构建和验证结果

- `git diff --check`：通过；新增 executor 文件的尾随空格检查：通过。
- 注册检查：通过，注册名、opType、TopoMatch 和四模板均与 HCCL 当前 Pcie 注册项一致。
- CMake 检查：通过，`mc2_client` 和 `mc2_server` 两个 source list 均已接入 executor。
- 尝试构建：`bash build.sh -j2 --build-type=Debug`；失败于构建前置检查，当前环境未设置 CANN 安装目录，
  需要通过 `-p/--cann_path` 提供有效依赖。
- 尝试最小语法检查：`clang++ -fsyntax-only ... ins_v2_all_reduce_parallel_executor.cc`；失败于环境缺少外部
  头文件 `securec.h`，未能完成 translation unit 编译。
- 未执行设备 ST/性能验证，也不能据此宣布算法运行正确。

### 未完成项、遗留风险和 audit/verify 输入

- 当前 selector 不会自动选择 `InsAllReduceParallelMesh1DNHRPcie`；需要 forced-alg 或后续单独迁移 selector。
- 需要在具备 CANN/Ascend 编译依赖的环境重新构建 `mc2_client` 与 `mc2_server`，重点确认新增 executor 的
  `override`、template 类型实例化和链接注册。
- 需要后续调用 `hccl-audit-migration` 复审 CANN 脱离、注册闭环、共享 Bridge 影响和 HCCL 语义一致性；再用验证
  skill 做 2 卡功能及目标规模性能验证。

### Audit 修复记录

- 根据本次 audit 的 P1 问题，在 `Orchestrate` 入口补充 `resCtx.algHierarchyInfo.infos.empty()` 检查，
  与当前 HCCL 实现保持一致；空层级信息时返回 `HCCL_E_PARA`，避免 AICPU 侧访问 `infos[0]`/`infos[1]` 越界。
- 本次只修复该运行时保护，不修改 selector、ST 或其他迁移逻辑。
- 修复后需要重新调用 `hccl-audit-migration` 复审；ST 接线缺口仍保留为未处理项。

状态：已完成 audit 指定的第一个问题修复，构建和设备验证待补齐；等待重新 audit。
