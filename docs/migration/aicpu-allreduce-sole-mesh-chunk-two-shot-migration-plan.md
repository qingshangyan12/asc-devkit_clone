# AicpuAllReduceSoleMeshChunkTwoShot 迁移方案

## 迁移前方案

### repository_baseline

结论分别基于以下本地仓库和工作区；未提交文件不归因于当前 commit。

| 仓库 | 分支/HEAD | commit | 工作区 | tracking 同步状态 |
| --- | --- | --- | --- | --- |
| `/Users/wy/gitcode代码/asc-comm/hccl` | `master` | `a5101c3161e975df635df46e296182ca6143340b` | dirty：`.DS_Store`、`src/.DS_Store` 两个未跟踪文件 | `origin/master` 比本地领先 14 个提交，本地落后 14 个提交 |
| `/Users/wy/gitcode代码/asc-comm/asc-devkit-all_reduce_mesh_chunk_two_shot` | `all_reduce_mesh_chunk_two_shot` | `634529fdbc4e2c04912bf7c24781fff297b79dcb` | clean | `master`：0 ahead / 0 behind |

HCCL 的算法代码使用已拉取的 `origin/master@f18105c608510bf691ccac2743ab2fd2de6c0bda` 作为源基线；该版本包含
`ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.*` 最近的 cost model 更新。目标迁移分支基于
`asc-devkit/master@634529fdb`。

### 用户输入与代码实际映射

- 用户输入：`AicpuAllReduceSoleMeshChunkTwoShot`。
- opType：`HCCL_CMD_ALLREDUCE`。
- HCCL 当前注册（`src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc:310-312`）：
  `AicpuAllReduceSoleMeshChunkTwoShot -> InsV2AllReduceSoleExecutor -> TopoMatch1D -> InsTempAllReduceMesh1DTwoShotMeshChunk`。
- HCCL selector 当前在 `MESH_1D` 大数据分支以及 `MESH_1D_CLOS` 的 PCIe-mix、内层 1D mesh 分支返回该 algName
  （`src/ops/all_reduce/selector/all_reduce_auto_selector.cc:543-550,562-569`）。
- 这是已有 `InsV2AllReduceSoleExecutor` executor 家族中的新 AICPU template，不是新的原语族。

### 候选算法、选定算法和默认兜底

- candidate_algorithms：`AicpuAllReduceSoleMeshChunkTwoShot`。
- selected_algorithms：`AicpuAllReduceSoleMeshChunkTwoShot`（用户已确认迁移）。
- selected_families：`InsV2AllReduceSoleExecutor + TopoMatch1D + InsTempAllReduceMesh1DTwoShotMeshChunk`。
- 当前目标仓已有同 opType 的本地注册：
  `AicpuAllReduceSoleMeshOneShot`、`AicpuAllReduceSoleMeshTwoShot`。
- default_fallback：
  - algName：`AicpuAllReduceSoleMeshOneShot`（目标仓当前 `AllReduceAutoSelector::SelectAicpuAlgo` 固定返回值）；
  - engine：`COMM_ENGINE_AICPU/AICPU_TS`，以实际 tiling 为准；
  - machine：`DEV_TYPE_950`（当前目标 ST 的仿真初始化使用该机型）；
  - topology：目标现有 `TopoMatch1D` 支持的 1D mesh 形态；
  - confirmation：`existing_local_default`。

### HCCL 注册、依赖闭包、Host/AICPU 路径

#### HCCL 注册和实现闭包

需要迁移或适配的直接依赖：

1. `src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.h`。
2. `src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc`。
3. `InsAlgTemplateBase`、`alg_data_trans_wrapper.h`、`SendRecvBatchWriteReduce`、`SendRecvRead`、
   `PreSyncInterThreads/PostSyncInterThreads` 等公共 AICPU template API；目标仓已有对应公共实现。
4. `InsV2AllReduceSoleExecutor` 的已有 `TopoMatch1D`、channel map、thread/notify 和 loop 调度。
5. HCCL `origin/master` 新增的 `CalcCostCoeff` 需要单独适配：目标 executor 基类目前只有
   `CalcCostCoeffAbiPlaceholder`，目标仓未找到 HCCL `CostModelManager` 的完整实现，不能直接复制后假设可编译。

#### Host 路径

当前目标代码事实：

- `ExecuteSelector::Run` 在 `execute_selector.cc:27-33` 对 AICPU AllReduce 先调用 `SelectViaCann`。
- `HcclExecOp` 在 `op_common.cc:412-419` 按 `UseCannBridge(param)` 选择 CANN 或本地 executor。
- `GetOpParamResCtx` 在 `hccl_alloc_ctx_res.cc:751-781` 同样按 `UseCannBridge` 查询 executor。
- `UseCannBridge` 在 `cann_host_bridge.cc:142-147` 对 AICPU/AICPU_TS 的整个 `HCCL_CMD_ALLREDUCE` 返回 true。

因此，新 algName 即使注册到目标本地表，当前 Host 选择、资源预检和执行准备仍可能查 CANN 注册表。仅新增 template
和 `REGISTER_EXEC_V2` 不足以实现完全脱离 CANN。

#### AICPU 路径

当前目标 `comm_kfc_open_kernel_executor.cc:179-201` 对 AllReduce 统一进入 `LaunchViaCann`，不会执行本地
`CollAlgExecRegistryV2::Instance().GetAlgExec(...)->Orchestrate(...)`。迁移必须把该 algName 的执行分流到本地注册表，
并保持 Host 传递的 `algName` 与 AICPU 侧重新查表一致。

### asc-devkit 已有能力和缺失能力

已有：

- `InsV2AllReduceSoleExecutor` 及其 `Orchestrate/CalcRes`。
- `TopoMatch1D`。
- `ins_temp_all_reduce_mesh_1D_one_shot.*` 和 `ins_temp_all_reduce_mesh_1D_two_shot.*`。
- `mc2_client` 的 all_reduce executor/template 目录接线。
- AllReduce ST 基础框架和 `all_reduce_testcase.cc`，其中已有 MeshChunk 用例名称，但当前目标代码没有 MeshChunk
  template 和注册实现，因此该用例目前不能证明目标算法已接入。

缺失或不一致：

- MeshChunk template 的 `.h/.cc`。
- `AicpuAllReduceSoleMeshChunkTwoShot` 的本地注册。
- `mc2_server` 根 source list 对 MeshChunk template 的接线。
- 目标本地 selector 的 MeshChunk 分支；当前 selector 固定返回 OneShot。
- AllReduce Host/AICPU Bridge 的局部分流方案。
- HCCL 最新 `CalcCostCoeff` 的目标接口适配。

### 选择行为约束

HCCL `SelectMeshAlgoAicpu` 的相关行为：

- 特殊 dtype（INT64/UINT64/FP64）或 `PROD` 不选择 MeshChunk。
- `MESH_1D`：非特殊类型且 `dataSize * (8 / rankSize)^2 > 32 MiB` 时选择 MeshChunk；两级网络且超过 sequence
  阈值时会转 Concurrent，不属于本次选定算法。
- `MESH_1D_CLOS` 且 PCIe-mix、内层全连接为 1D mesh：非特殊类型且同一大数据条件时选择 MeshChunk。
- 其他 topology/selector 分支不能让本次迁移新增的 algName 被错误返回。

目标当前 selector 固定返回 OneShot。计划只增加代码事实明确的 MeshChunk 分支，并保留其他未迁移候选的目标既有兜底，
避免把尚未迁移的 HCCL algName 引入目标本地 selector。

### 计划新增/修改文件

计划文件清单，待用户确认后才执行：

| 文件 | 操作 | 目的 |
| --- | --- | --- |
| `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.h` | 新增 | MeshChunk template 声明，按目标基类适配签名 |
| `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc` | 新增 | ReduceScatter + AllGather 实现；适配目标公共 wrapper |
| `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc` | 修改 | include template 并新增精确注册 |
| `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/CMakeLists.txt` | 修改 | 接入 `mc2_client` template 源文件 |
| `impl/adv_api/detail/hccl/cc/src/CMakeLists.txt` | 修改 | 接入 `mc2_server` 的 template 源文件 |
| `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc` | 修改 | 增加有代码依据的 MeshChunk 选择分支；保留未迁移候选兜底 |
| `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc` | 待确认后修改 | 让本地已迁移 algName 不走 CANN selector/resource/exec |
| `impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_open_kernel_executor.cc` | 待确认后修改 | AICPU 侧按本地注册表执行该 algName |
| `tests/api/adv_api/hccl/cc/st/algorithm/testcase/all_reduce_testcase.cc` | 视现有用例结果决定 | 保留已有 MeshChunk 用例，必要时补注册/forced-alg 命中断言 |

### 注册闭环

| 环节 | HCCL 当前 | asc-devkit 当前 | 迁移后计划 |
| --- | --- | --- | --- |
| 注册 | `AicpuAllReduceSoleMeshChunkTwoShot` | 缺失 | 在 `InsV2AllReduceSoleExecutor` 中注册同名算法 |
| selector | HCCL 大数据 Mesh1D/PCIe-mix Mesh1D 分支返回该名 | AICPU selector 固定返回 OneShot | 增加有条件的 MeshChunk 分支，其他分支不返回悬空名 |
| Host `CalcRes` | HCCL 通过注册表构造 sole executor | AllReduce 当前 `UseCannBridge=true`，查 CANN | 经确认的局部分流后使用 devkit 本地注册表 |
| AICPU `Orchestrate/KernelRun` | HCCL AICPU executor/template | AllReduce 当前统一 `LaunchViaCann` | 按 algName 使用本地注册表，调用 `Orchestrate -> KernelRun` |
| CMake | HCCL aicpu source list | template 已接入目录，但缺 MeshChunk | 补 `mc2_client` 和 `mc2_server` 两侧 source list |

### 接口适配清单

- namespace：源 `ops_hccl`，目标 `mc2_ops_hccl`。
- `KernelRun`：目标基类要求使用 `const TemplateResource&`；源实现中的非 const 签名必须按目标基类适配，不能沿用源文件签名猜测。
- include：按目标已有 OneShot/TwoShot template 的 include 和目录使用。
- 数据类型大小表、`const`、引用/指针、返回值、编译宏和 C++14 设置按目标实现适配。
- `CalcCostCoeff`：源实现依赖 `CostModelManager`；目标只有 ABI placeholder，暂列为接口差异，未确认前不复制或伪造。
- `TopoMatch1D`、`ChannelInfo`、`TemplateDataParams`、`TemplateResource` 和 notify/channel API 使用目标定义。

### 构建目标矩阵

| 目标 | 预期接线 |
| --- | --- |
| Host / `mc2_client` | executor 注册、template 源文件、selector、Bridge/本地查表路径 |
| Device / `mc2_server` | 根 `src/CMakeLists.txt` 的 executor/template source list 和 AICPU 编译宏 |
| ST | 现有 `all_reduce_testcase.cc` 的 MeshChunk 用例；必要时补 algName 命中检查 |
| UT | 当前没有发现该算法专属 UT；不新增白盒测试，除非迁移接口适配需要覆盖 |

### 共享逻辑影响、风险和待确认项

1. **AllReduce Bridge 影响（必须确认）**：当前 `UseCannBridge` 是按 opType 的全量开关；要让本算法完全脱离
   CANN，必须调整 AllReduce 的 selector/resource/execute/AICPU dispatch 链路。若整体关闭 AllReduce Bridge，
   目标当前只有 OneShot/TwoShot 的本地 selector 不能覆盖 HCCL 的全部候选算法，可能改变既有 AllReduce 行为。
   计划采用“只让已迁移且本地已注册的 algName 走 devkit，其余保持原 CANN/兜底”的策略；但这需要先解决 selector
   尚未产生 algName 时的分流时机，不能直接修改一个布尔条件后假设闭环成立。
2. **selector 影响（必须确认）**：增加 MeshChunk 分支会改变大数据 AllReduce 的本地选择；非目标分支继续保持当前
   OneShot 兜底，不引入 Concurrent/NHR/Parallel 等未迁移 algName。
3. **最新源版本差异**：HCCL `origin/master` 新增 `CalcCostCoeff`，目标没有完整 cost model API；本次先保持算法执行
   语义，是否迁移 cost model 需要根据目标 ABI 和调用点决定，不能自行删改成伪实现。
4. **测试边界**：现有 ST 用例包含 MeshChunk 名称，但在注册和 Bridge 完整接线前不能视为通过；本地没有完整
   CANN/Linux/交叉编译环境时只做常规静态检查，不能宣称构建或设备验证通过。

### 用户确认记录

- 迁移范围确认：用户回复“继续”，确认迁移 `AicpuAllReduceSoleMeshChunkTwoShot`。
- AllReduce 已迁移 algName 的 Host/AICPU 局部分流：已批准；只对本地精确注册的 MeshChunk algName 生效。
- selector 选择 MeshChunk、其他未迁移候选继续原 CANN/兜底：已批准，保持未迁移算法行为。
- `CalcCostCoeff`：未迁移；目标没有 HCCL `CostModelManager` 对应实现，本次保持执行语义，不添加伪实现。

## scope_status

`READY_TO_MIGRATE`

## plan_document

`docs/migration/aicpu-allreduce-sole-mesh-chunk-two-shot-migration-plan.md`

## 迁移后实施结果

### 实际修改/新增文件清单

新增：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.h`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc`

修改：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/CMakeLists.txt`
- `impl/adv_api/detail/hccl/cc/src/CMakeLists.txt`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`
- `impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/auto_selector_base.h`
- `impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/execute_selector.cc`
- `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.h`
- `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc`
- `impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc`
- `impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc`
- `impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_open_kernel_executor.cc`

### 每个文件的迁移内容

- 新 template：迁移 MeshChunk 的 PreCopy、ReduceScatter、AllGather、切片和 notify 逻辑；namespace 从
  `ops_hccl` 适配为 `mc2_ops_hccl`。
- 新 template：`KernelRun` 按目标 `InsAlgTemplateBase` 使用 `const TemplateResource&`，与目标基类声明、定义和调用点一致。
- sole executor：注册
  `HCCL_CMD_ALLREDUCE -> AicpuAllReduceSoleMeshChunkTwoShot -> InsV2AllReduceSoleExecutor -> TopoMatch1D -> InsTempAllReduceMesh1DTwoShotMeshChunk`。
- CMake：分别接入 `mc2_client` template 和 `mc2_server` template；已有 all_reduce 子目录链路不改动。
- selector：在目标本地 AICPU selector 中增加与 HCCL 当前代码一致的 Mesh1D、PCIe-mix Mesh1D 大数据命中条件；特殊 dtype、PROD、Concurrent 条件和非 Mesh1D 分支不返回该 algName。
- selector 分流：AllReduce 仍保留 CANN selector 作为未迁移算法兜底；只有本地 selector 明确命中新 algName 时才提前返回本地结果。
- Bridge：增加精确 algName 判断；MeshChunk 的 Host executor 查询、资源查询和 AICPU 执行不再调用 CANN 注册表；其他 opType/algName 保持原 Bridge 行为。
- 资源复用：将新 algName 映射到现有 `InsAllReduceMeshClass` 复用类别。

### Host/AICPU 构建目标影响

- `mc2_client`：编译新的 template、executor 注册、selector 和 Host 分流代码。
- `mc2_server`：编译新的 template，并在 AICPU 入口按 algName 使用本地 `CollAlgExecRegistryV2`。
- ST：现有 `all_reduce_testcase.cc` 已包含 MeshChunk 数据边界、rank 和数据量用例；本次未新增重复用例。
- UT：未发现该算法专属 UT，本次未新增白盒测试。

### 迁移前计划与实际改动差异

- 计划中的直接模板、executor、CMake、selector、Bridge 和 AICPU 分流均已完成。
- `CalcCostCoeff` 未迁移：HCCL `origin/master` 的该函数依赖目标不存在的 `CostModelManager`；目标当前只提供 ABI
  placeholder，复制实现会引入未知依赖，因此保持目标现有接口和执行语义。
- 未修改 HCCL 源仓库，也未修改目标工作区原有未提交文件；目标原本只有迁移文档未跟踪改动。

### 测试、常规检查和可选构建结果

- `git diff --check`：通过（已跟踪改动）。
- template namespace/签名检查：通过；头文件、实现和目标基类均为 `KernelRun(..., const TemplateResource&)`。
- 注册反查：通过；selector 返回的 MeshChunk algName 与 executor 注册名完全一致。
- CMake source list 检查：通过；`mc2_client` 和 `mc2_server` 均包含新 template。
- Host 查表检查：通过；正常选择、forced-alg 资源预检和执行阶段对该 algName 使用本地注册表。
- AICPU 查表检查：通过；该 algName 进入本地 `CollAlgExecRegistryV2`，其他 AllReduce 算法仍进入 CANN Bridge。
- 完整 CANN/Linux/交叉编译：未执行；当前环境不具备该迁移阶段所需完整工具链，不能宣称构建通过。
- 设备 ST/性能验证：未执行；需要后续验证环境和对应拓扑。

### 未完成项、遗留风险和 audit 输入

- 本次实现保留 CANN selector 作为未迁移 AllReduce 候选兜底；因此目标算法自身的资源和执行已脱离 CANN，未迁移候选仍依赖 CANN，这是为保护 devkit 既有行为做的局部分流。
- HCCL 最新 `CalcCostCoeff` 未同步，若目标后续引入 cost model 选择，需要单独评估接口和行为。
- 当前只完成常规静态检查，未替代真实构建和设备验证。
- 建议审查重点：局部 Bridge 分流是否覆盖正常 selector、forced-alg、资源预检和 AICPU 重新查表；MeshChunk 的非均匀切片、offset、channel/notify 和 0-size slice 行为；未迁移 AllReduce 兜底是否保持。

状态：迁移代码和常规静态检查完成，等待 `hccl-audit-migration` 复审。
