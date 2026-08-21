# AICPU AllReduce Sole NHR AicpuReduce 迁移方案

## 迁移前方案

### repository_baseline

- 源仓库：`/Users/wy/gitcode代码/asc-comm/hccl`
  - branch_or_head: `master`
  - commit: `a5101c3161e975df635df46e296182ca6143340b`
  - working_tree: `dirty`
  - uncommitted_files: `.DS_Store`, `src/.DS_Store`（均为未跟踪文件，与本算法无关）
  - tracking_remote: `origin/master`
  - ahead_behind: ahead `0`, behind `14`
- 目标仓库：`/Users/wy/gitcode代码/asc-comm/asc-devkit-allreduce-sole-nhr`
  - branch_or_head: `allreduce-sole-nhr`
  - commit: `6e9de0f8de7496c93d3ed24cb937409d0cb0ecfe`
  - working_tree: `clean`
  - uncommitted_files: `[]`
  - tracking_remote: `upstream/master`
  - ahead_behind: ahead `0`, behind `0`

以上结论分别基于上述两个仓库的当前工作区；源 `hccl` 当前 commit 相对 tracking 远端不是最新，不能把远端尚未同步的改动归因到本次分析。

### 用户输入与代码实际映射

- 用户指定算法：`AicpuAllReduceSoleNHRAicpuReduce`
- `opType`：`HCCL_CMD_ALLREDUCE`
- HCCL 当前注册：
  - executor：`InsV2AllReduceSoleExecutor`
  - TopoMatch：`TopoMatch1D`
  - template：`InsTempAllReduceAicpuReduceNHR`
  - 依据：`hccl/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc:287-288`
- 目标 asc-devkit 当前注册：未找到 `AicpuAllReduceSoleNHRAicpuReduce`，当前只有
  `AicpuAllReduceSoleMeshOneShot` 和 `AicpuAllReduceSoleMeshTwoShot`，依据：
  `asc-devkit-allreduce-sole-nhr/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc:246-252`
- 本任务不是根据 `Ins`/`Aicpu` 前缀推断映射，而是依据源端注册宏和目标端注册表核对出的唯一映射。

### 算法语义与适用边界

- 这是 AllReduce 的 AICPU Sole NHR 算法：先按通信域 rank 切片，执行 NHR gather，随后把本 rank 数据拷贝到 output，再按 rank 顺序做本地归约。
- `CalcRes` 只申请 1 个主线程，并通过 `CalcChannelRequestNhr` 请求 NHR channel，依据：
  `hccl/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc:33-50`。
- `KernelRun` 的关键路径是 `CalcSlice -> PreCopy -> RunGather -> RunReduce`，依据：
  `hccl/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc:59-115`。
- `RunReduce` 会结束并重新开始 batch mode，等待通信线程后执行 `LocalReduce`；这是本算法的 AICPU 本地归约语义，不能改成普通 copy 或跳过等待，依据：
  `hccl/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc:208-232`。
- machine：代码未在该注册项中固定机型，记录为 `unknown`。
- topology：注册使用 `TopoMatch1D`；最终是否匹配由运行时 topology 信息决定，不能仅凭算法名推断具体卡型。
- engine：算法实现属于 AICPU template；实际为 `AICPU`/`AICPU_TS` 由调用侧和 selector 的执行配置决定，注册项本身不固定 engine。

### HCCL selector 选择事实

源 HCCL 的 `SelectAicpuAlgo` 在以下条件会返回 `AicpuAllReduceSoleNHRAicpuReduce`：

- 多层拓扑且数据类型为 `INT64`、`UINT64`、`FP64`，或 reduce op 为 `PROD`，依据：
  `hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:427-435`。
- UBX/非普通 Mesh 分支中的特殊 dtype 或 `PROD`，依据：
  `hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:476-506`。
- PCIe Mix 且底层不是全连接 1D Mesh 时的特殊 dtype 或 `PROD`，依据：
  `hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:554-573`。

目标 selector 当前无条件返回 `AicpuAllReduceSoleMeshOneShot`，依据：
`asc-devkit-allreduce-sole-nhr/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc:15-24`。
因此，当前目标代码不存在该算法的正常自动选择闭环；即使只做 forced-alg 注册，也不能声称 selector 已与 HCCL 一致。

### 依赖闭包

- 新增 AICPU template：`InsTempAllReduceAicpuReduceNHR` 的头文件、实现文件。
- 复用目标已有 `InsV2AllReduceSoleExecutor` 和 `TopoMatch1D`，不新增 executor 家族。
- 依赖 `CalcChannelRequestNhr`、`IsPcieProtocol`、`SendRecvRead/Write`、`LocalCopy`、`LocalReduce`、`AicpuNHRStepInfo` 等公共接口；迁移时按目标仓同类 AICPU template 的 namespace/include/参数接口适配。
- 目标 `template/aicpu/CMakeLists.txt:14-16` 已经列出 `ins_temp_all_reduce_nhr.cc`、`ins_temp_all_reduce_aicpu_reduce_nhr.cc` 等文件，但当前 HEAD tree 中这些文件不存在。这是目标基线已有的悬空 CMake 引用，不归因于本次迁移；迁移时必须先确认是历史残留还是预期迁移接线，不能静默覆盖。

### Host/AICPU 双侧路径

- Host 资源/执行入口：目标 `op_common.cc` 在 `UseCannBridge(param)` 为真时通过 `GetAlgExecViaCann` 查 CANN 注册表，否则查本地 `CollAlgExecRegistryV2`，依据：
  `asc-devkit-allreduce-sole-nhr/impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc:412-425`。
- 目标 `UseCannBridge` 当前对 `AICPU`/`AICPU_TS` 的 `ALLREDUCE` 返回 true，依据：
  `asc-devkit-allreduce-sole-nhr/impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc:142-147`。
- AICPU 侧 kernel launch 会根据 `param.algName` 在本地 `CollAlgExecRegistryV2` 重新查 executor；该路径位于目标 `impl/adv_api/detail/hccl/cc/src/ops/op_common/template/aicpu/kernel_launch.cc` 的 AICPU executor 查表逻辑。
- 计划中必须保证 Host 资源查询和 AICPU `Orchestrate/KernelRun` 使用同一个精确 algName，不能依赖 Host executor 对象跨环境传递。

### asc-devkit 已有能力与缺失能力

- 已有：AllReduce `InsV2AllReduceSoleExecutor`、`TopoMatch1D`、AICPU template 构建链路、OneShot/TwoShot 注册和通用 AllReduce ST 框架。
- 缺失：`AicpuAllReduceSoleNHRAicpuReduce` 注册、`InsTempAllReduceAicpuReduceNHR` 实现文件，以及与 HCCL 选择/Bridge 对应的完整本地闭环。
- 目标 branch 的 `all_reduce` 当前 ST 有通用 `all_reduce_testcase.cc`，未发现该 algName 专属 ST；需要在迁移后决定是通过既有 forced-alg 用例覆盖，还是增加专属用例。

### 计划新增/修改文件

计划只涉及以下范围，阶段二开始前会再次核对：

1. 新增 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.h`
2. 新增 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc`
3. 修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc`：include 和 `REGISTER_EXEC_V2`。
4. 核对并必要时修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/CMakeLists.txt`：只补齐本算法真实存在的 source，不覆盖基线中其他悬空引用。
5. 本次不修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`：后续验证通过 `algConfig` 指定精确 algName，跳过正常 selector；目标自动选择与 HCCL 的差异作为已知范围边界记录。
6. 可能修改 `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc`：移除 AllReduce 的 CANN Bridge 分支；该项会影响目标已有 AllReduce 算法，需用户确认。
7. 必要时补充现有 AllReduce ST 的 forced-alg/特殊 dtype 用例接线；不重构现有测试框架。

### 注册闭环

| 环节 | 当前事实 | 迁移计划 |
|---|---|---|
| HCCL 注册 | `AicpuAllReduceSoleNHRAicpuReduce -> InsV2AllReduceSoleExecutor -> TopoMatch1D -> InsTempAllReduceAicpuReduceNHR` | 作为源语义基线 |
| asc-devkit 注册 | 未注册该 algName | 新增同名 `REGISTER_EXEC_V2` |
| selector | 目标当前固定 OneShot | 是否同步 HCCL 特殊分支待确认 |
| forced-alg | 本地注册缺失，无法命中目标 executor | 注册后支持精确 algName 查表 |
| Host CalcRes | 当前 AllReduce AICPU 走 CANN Bridge 资源 executor | 移除 Bridge 后走本地 registry；会影响既有 AllReduce |
| AICPU Orchestrate/KernelRun | AICPU kernel 按 algName 查本地 registry，但目标注册缺失 | 新增本地 executor 注册后闭环 |

### 构建目标矩阵

- `mc2_client`：AllReduce executor、selector、AICPU template 和 Host Bridge 相关接线。
- `mc2_server`：核对 AICPU kernel launch/registry 是否使用同一 algName；本次不应把 Host 对象传入 Device。
- ST：复用 `tests/api/adv_api/hccl/cc/st/algorithm/all_reduce_testcase.cc`，优先增加 forced-alg 与特殊 dtype/`PROD` 覆盖；当前未发现该算法专属测试。
- UT：本迁移不新增白盒故障注入；如目标仓已有公共 template UT，再按现有模式接入。

### 接口适配与硬检查重点

- HCCL template 的 `KernelRun` 使用 `TemplateResource&`，而目标现有 AICPU template 使用 `const TemplateResource&`；必须先以目标 `InsAlgTemplateBase` 声明为准核对 `override`，不能直接复制旧签名。
- 核对 namespace：源为 `ops_hccl`，目标为 `mc2_ops_hccl`。
- 核对 `DataSlice`、`SendRecvInfo`、`TxRxChannels`、`DATATYPE_SIZE_TABLE`、`AicpuNHRStepInfo` 和 `LocalReduce` 的目标接口。
- 对 `RunReduce` 的等待顺序、rank 索引语义、scratch offset、channel remote memory 和 `isDmaRead_` 分支做源/目标语义对照。
- 冲突解决或接口适配后重新检查头文件声明、`.cc` 定义和调用点，尤其是 `KernelRun` 的 const/ref 签名。

### 共享逻辑影响、风险和待确认项

1. **AllReduce Bridge 共享影响**：目标 `UseCannBridge` 当前覆盖全部 AICPU/AICPU_TS AllReduce。移除 ALLREDUCE 条件后，已有 `AicpuAllReduceSoleMeshOneShot`/`TwoShot` 也会从 CANN executor 切到本地 executor。这是实现“完全脱离 CANN”的必要改动，但属于共享行为变化。
2. **selector 范围边界**：目标 selector 当前所有 AllReduce 都返回 OneShot。本次按用户确认不修改 selector，验证和使用通过 `algConfig` 强制指定 `AicpuAllReduceSoleNHRAicpuReduce`；因此本次不覆盖自动选择与 HCCL 一致性。
3. **源仓未同步**：当前 hccl `master` 落后 `origin/master` 14 个 commit；若用户要求基于最新上游源重新分析，应先更新源仓并重新生成基线。
4. **目标 CMake 基线悬空引用**：目标 CMake 已引用多个当前不存在的 NHR/Chunk 文件；本次不能把这些既有问题与新增文件混为一谈。
5. **默认兜底**：目标已有本地 AllReduce 默认 `AicpuAllReduceSoleMeshOneShot`，因此没有“原语族完全无默认算法”的阻塞；但 selector/Bridge 的共享行为变化仍需批准。

### 用户确认记录

- 已确认迁移算法：`AicpuAllReduceSoleNHRAicpuReduce`
- 已确认：允许移除 AllReduce 的 `UseCannBridge` 分支，使目标已有 AllReduce 也切换到本地 asc-devkit 注册表；该共享行为变化已获用户批准。
- 已确认：本次不迁移 selector；后续通过 `algConfig` 指定 `AicpuAllReduceSoleNHRAicpuReduce`，跳过正常 selector。

状态: `READY_TO_MIGRATE`

## 迁移后实施结果

### 实际修改/新增文件清单

- 新增 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.h`
  - 将 HCCL `InsTempAllReduceAicpuReduceNHR` 迁移到 `mc2_ops_hccl` namespace。
  - 按目标基类将 `KernelRun` 适配为 `const TemplateResource&`。
- 新增 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc`
  - 保留 HCCL 的 NHR gather、AICPU local reduce、slice、channel 和 DMA read/write 语义。
  - 仅增加目标编译所需的 `<algorithm>`、`<stdexcept>` 显式 include。
- 修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc`
  - include 新 template。
  - 新增注册：
    `HCCL_CMD_ALLREDUCE -> AicpuAllReduceSoleNHRAicpuReduce -> InsV2AllReduceSoleExecutor -> TopoMatch1D -> InsTempAllReduceAicpuReduceNHR`。
- 修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/CMakeLists.txt`
  - 将新 template 接入 `mc2_client`。
- 修改 `impl/adv_api/detail/hccl/cc/src/CMakeLists.txt`
  - 将新 template 接入 `mc2_server` 独立 source list；这是 AICPU 侧实际编译所需的根 target 接线。
- 修改 `impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc`
  - `UseCannBridge` 不再对 `HCCL_CMD_ALLREDUCE` 返回 true。
  - AllReduce 的 AICPU/AICPU_TS Host 资源和 executor 查询改走 asc-devkit 本地注册表。
- 未修改 `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`
  - 按用户确认，后续使用精确 `algConfig`，跳过正常 selector。
- 未新增专属 ST/UT 文件
  - 当前目标 AllReduce ST 使用通用 `HcclAllReduce`，未发现现成的精确 algConfig 注入入口；不能把通用 ST 结果宣称为本算法 forced-alg 验证。

### Host/AICPU 构建目标影响

- `mc2_client`：新增 template、executor 注册可见，本地 Host 资源路径可按 algName 查找。
- `mc2_server`：根 `src/CMakeLists.txt` 已加入新 template，AICPU kernel 侧可按 algName 查本地注册表。
- Bridge：AllReduce 不再调用 CANN `GetAlgExecViaCann`；AllToAll/AllToAllV Bridge 行为保持不变。
- selector：保持目标现状，不自动选择本算法。

### 迁移前计划与实际改动差异

- 计划中的 template、executor 注册、Host/AICPU CMake 和 AllReduce Bridge 改动均已完成。
- selector 按用户确认不迁移；算法需要通过 `algConfig=AicpuAllReduceSoleNHRAicpuReduce` 指定。
- ST 未新增，因为当前框架没有直接的精确 algConfig 注入路径；需后续使用适配的 MC2/ST 入口或设备环境验证。

### 常规检查结果

- `git diff --check`：通过。
- 注册名反查：通过；selector 未返回本算法，forced-alg 目标注册存在。
- CMake source list：通过；新 template 同时接入 `mc2_client` 和 `mc2_server`。
- namespace/include：通过；目标实现使用 `mc2_ops_hccl`，新增标准库依赖显式包含。
- `KernelRun` 虚函数签名：通过；目标基类、头文件声明和 `.cc` 定义均为 `const TemplateResource&`。
- Bridge 分流：通过；AllReduce 不再进入 CANN Bridge，AllToAll/AllToAllV 保持原条件。
- 完整 CANN/Linux/交叉编译：未执行，当前迁移环境不具备该构建条件；不能据此宣称编译或设备运行通过。

### 未完成项、遗留风险和 audit 输入

- 当前 `hccl` 源仓 `master` 相对 `origin/master` 落后 14 个 commit；本次源语义基于记录的本地 commit，不包含远端未同步变化。
- 目标 AllReduce 现有 OneShot/TwoShot 也会因 Bridge 改动切换到 asc-devkit 本地路径；这是已获用户批准的共享行为变化，仍需 audit 和运行验证。
- 目标 selector 不会自动选择本算法；验证必须使用精确 algName：`AicpuAllReduceSoleNHRAicpuReduce`。
- 目标当前未提供本算法专属 ST 结果；本次仅完成代码迁移和常规静态检查。

下一步：调用 `hccl-audit-migration` 复审本次迁移。
