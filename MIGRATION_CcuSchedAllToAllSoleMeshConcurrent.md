# CcuSchedAllToAllSoleMeshConcurrent 算法迁移修改记录

## 概述

将 `CcuSchedAllToAllSoleMeshConcurrent` 算法从 hccl 仓迁移到 asc-devkit 仓，参考 `CcuSchedAllGatherConcurMeshNHRMultiLink` 的注册和分发模式，但 kernel 核心逻辑独立实现。

---

## 一、新建文件（4 个）

### 1. KFC AllToAll Kernel 头文件

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/template/ccu/kernel/ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty.h`

- 声明 `CcuKfcAllToAllMesh1DMultiJettyKernel` 函数
- 参数：inputAddr, outputAddr, tokenInfo, sliceSize, srcStride, srcOffset, dstOffset, goSize[0..3], channels, channelCount, rankSize, rankId

### 2. KFC AllToAll Kernel 实现

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/template/ccu/kernel/ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty.cc`

- `KfcAllToAllContext` 结构体：channels, rankSize, rankId, input/output/token, peerOutput/peerToken, sliceSize, srcStride, srcOffset, dstOffset, goSize, event
- `InitResource`：按 rank 遍历 channels，获取 peerOutput/peerToken
- `PreSync`：向所有 channel 写 output/token 变量并 Notify，然后等待所有 channel 的回应
- `DoAllToAll`：遍历所有非本 rank，计算 src/dst 地址，按 rank mask 写入；本地拷贝走 GroupCopy；最终 EventWait 等待所有 rank 完成
- `PostSync`：所有 channel NotifyRecord + NotifyWait
- **设计决策**：不使用 jetty 分片（`ccu::Variable` 不支持 `/` `*` 运算符），直接按 rank 写完整 `sliceSize`

### 3. KFC AllToAll 模板头文件

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/template/ccu/ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.h`

- `CcuKernelArgKfcAllToAllMesh1DMultiJetty` 结构体：rankSize, rankId, opParam, subCommRanks
- `CcuTempKfcAllToAllMesh1DMultiJetty` 类：继承 `CcuAlgTemplateBase`
- 提供 `CalcRes`、`KernelRun`、`GetRes`、`GetThreadNum`、`CalcScratchMultiple`、`CalcResByChannelDescs`（stub）

### 4. KFC AllToAll 模板实现

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/template/ccu/ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.cc`

- `CalcRes`：调用 `CalcChannelRequestMeshClosMultiJetty` 获取 channel，创建 kernelInfo
- `KernelRun`：计算 inputAddr/outputAddr/token/sliceSize/srcStride/srcOffset/dstOffset/goSize，通过 `FillCachedArgs` 填充 11 个参数（`1ULL` 控制字 + 10 个数据参数），token 不放入 payload
- `GetRes`：slaveThreadNum=0, notifyNumOnMainThread=0
- `CalcScratchMultiple`：返回 0

---

## 二、修改文件

### 5. kfc_server_message.h — 新增 AllToAll 参数索引枚举

**文件**: `impl/adv_api/detail/hccl/common/kfc_server_message.h`

新增 `KfcConcurrentAllToAllParamIndex` 枚举：
```
KFC_CONCURRENT_A2A_OP_ID = 0
KFC_CONCURRENT_A2A_INPUT = 1
KFC_CONCURRENT_A2A_OUTPUT = 2
KFC_CONCURRENT_A2A_SLICE_SIZE = 3
KFC_CONCURRENT_A2A_SRC_STRIDE = 4
KFC_CONCURRENT_A2A_SRC_OFFSET = 5
KFC_CONCURRENT_A2A_DST_OFFSET = 6
KFC_CONCURRENT_A2A_GO_SIZE_0..3 = 7..10
KFC_CONCURRENT_A2A_PARAM_NUM = 11
```

### 6. kfc_server_protocol.h — 新增 AllToAll 算法名和角色

**文件**: `impl/adv_api/detail/hccl/cc/src/common/kfc_server_protocol.h`

- 新增 `KFC_CONCURRENT_ALL_TO_ALL_ALG_NAME[] = "CcuSchedAllToAllSoleMeshConcurrent"`
- `GetKfcServerMissionNum`：增加 AllToAll 算法返回 `KFC_MAX_MISSION_NUM`
- `KfcServerRole` 枚举：新增 `ALL_TO_ALL_MULTI_JETTY = 3`
- `GetKfcServerRole`：AllToAll 算法返回 `ALL_TO_ALL_MULTI_JETTY`

### 7. ccu_kernel_kfc_server.h — KfcServer arg 结构体

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.h`

- `CcuKernelArgKfcServer`：未新增字段（最终设计不需要 `a2aJettyNums`）

### 8. ccu_kernel_kfc_server.cc — KFC server dispatch

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc`

- 新增 include：`ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty.h`、`ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h`
- 恢复 `HBM_PARAM_IDX_11`~`HBM_PARAM_IDX_17` 常量定义
- `DispatchKfcSubKernel`：重写为统一使用 `uint64_t opType` 变量，新增 `ALL_TO_ALL_MULTI_JETTY` 分支，使用 `KFC_CONCURRENT_A2A_*` 常量调用 `CcuKfcAllToAllMesh1DMultiJettyKernel`，token 从 `ctx.token` 注入
- 新增 `static_assert(KFC_CONCURRENT_A2A_PARAM_NUM <= CCU_PARAM_NUM_PER_DIE)`

### 9. ccu_temp_kfc_server.cc — KfcServer 模板

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/ccu_temp_kfc_server.cc`

- 新增 include：`ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.h`
- `InheritKfcServerKernelArg`：新增 `isAlltoAllKfcMultiJetty` 分支，从 `CcuKernelArgKfcAllToAllMesh1DMultiJetty` 继承 rankSize/rankId/opParam/subCommRanks
- `CalcRes`：新增 `isAlltoAllKfcMultiJetty` 声明和 roleMatches 条件；条件判断包含 `isAlltoAllKfcMultiJetty`
- 修复 git 冲突标记（`<<<<<<<`/`=======`/`>>>>>>>`）

### 10. ins_v2_all_to_all_concurrent_executor.h — executor 头文件

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/executor/ins_v2_all_to_all_concurrent_executor.h`

- 新增 `tmp0CcuKernels_`/`tmp1CcuKernels_` 成员变量

### 11. ins_v2_all_to_all_concurrent_executor.cc — executor 实现

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/executor/ins_v2_all_to_all_concurrent_executor.cc`

- 新增 include：`ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.h`（`#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU`）
- `CalcRes`：新增 CCU 分支（`param.engine == CommEngine::COMM_ENGINE_CCU`），调用 `CalcRes` 而非 `CalcResByChannelDescs`，填充 ccuKernelNum/ccuKernelInfos
- `SetTemplateDataParams`：新增 `inBuffBaseOff`/`outBuffBaseOff` 计算：`(splitData.sdispls[0] + processedCount) * dataTypeSize_` / `(splitData.rdispls[0] + processedCount) * dataTypeSize_`，含溢出检查
- `Orchestrate`：新增 CCU 分支设置 ccuKernels 和 threads
- `OrchestrateLoop`：新增 CCU 分支设置 resource 的 ccuKernels
- 新增算法注册：`REGISTER_EXECUTOR_BY_TWO_TEMPS(HCCL_CMD_ALLTOALL, CcuSchedAllToAllSoleMeshConcurrent, InsV2AllToAllConcurrentExecutor, TopoMatchUBX, CcuTempKfcAllToAllMesh1DMultiJetty, CcuTempKfcAllToAllMesh1DMultiJetty)`

### 12. alltoall_auto_selector.cc — selector

**文件**: `impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/selector/alltoall_auto_selector.cc`

- `SelectMesh1DClosAlgo`：并发条件分支返回 `"CcuSchedAllToAllSoleMeshConcurrent"` + `SelectorStatus::MATCH`（原先返回 `"CcuAllToAllMesh1DConcurrent"` + `NOT_MATCH`）

### 13. hccl_alloc_ctx_res.h — algorithmMap 和 AlgorithmType 枚举

**文件**: `impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.h`

- `AlgorithmType` 枚举：新增 `CcuSchedAllToAllSoleMeshConcurrent`
- `algorithmMap`：新增 `{"CcuSchedAllToAllSoleMeshConcurrent", CcuSchedAllToAllSoleMeshConcurrent}`

### 14. hccl_tiling.cpp — tiling 侧算法名集合

**文件**: `impl/adv_api/detail/hccl/tiling/hccl/hccl_tiling.cpp`

- `REGISTERED_CCU_ALGORITHMS` 集合：新增 `"CcuSchedAllToAllSoleMeshConcurrent"`
- 作用：tiling 生成时 `IsCcuAlgorithmRegistered` 查此集合，找到则 version=101（CCU KFC），否则 version=100

### 15. hccl_ccu_v0_def.h — device 侧 AlgorithmType 枚举

**文件**: `impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0_def.h`

- `AlgorithmType` 枚举（`uint8_t`）：新增 `CcuSchedAllToAllSoleMeshConcurrent`

### 16. hccl_ccu_v0.h — device 侧 missionNum 判断

**文件**: `impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0.h`

- `GetKfcMissionNum`：增加 `CcuSchedAllToAllSoleMeshConcurrent` 条件，返回 `KFC_MAX_MISSION_NUM`

### 17. CMakeLists.txt 文件（3 个）

**`impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/template/ccu/kernel/CMakeLists.txt`**：
- 新增 `ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty.cc`

**`impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/template/ccu/CMakeLists.txt`**：
- 新增 `ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.cc`

**`impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/executor/CMakeLists.txt`**：
- 移除 `if(NOT HCCL_CANN_COMPAT_850)` 守卫，`ins_v2_all_to_all_concurrent_executor.cc` 无条件编译

---

## 三、pre-commit clang-format 修复（5 个文件）

以下文件被 clang-format 自动格式化，手动应用了相同修改：

1. `ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.h` — `StringFormat` 行合并
2. `ccu_kernel_kfc_all_to_all_mesh1d_multi_jetty.cc` — `HCCL_ERROR` 参数换行、`numJetty` 三元表达式换行
3. `ccu_temp_kfc_server.cc` — `roleMatches` 条件对齐、`if` 条件缩进
4. `ccu_kernel_kfc_server.cc` — `CcuAlltoAllMesh1DKernel` 参数换行
5. `ins_v2_all_to_all_concurrent_executor.cc` — `REGISTER_EXECUTOR_BY_TWO_TEMPS` 参数换行

---

## 四、编译错误修复

### 错误1：`ccu::Variable` 不支持 `/` `*` 运算符

- **原因**：kernel 中 `ctx.sliceSize / numJetty / A2A_MIN_SLICE_ALIGN * A2A_MIN_SLICE_ALIGN` 使用了 `ccu::Variable` 的除法和乘法
- **修复**：简化 `DoAllToAll`，移除 jetty 分片逻辑，直接按 rank 写完整 `sliceSize`

### 错误2：`ccu_temp_kfc_server.cc` git 冲突标记

- **原因**：文件中残留 `<<<<<<< HEAD` / `=======` / `>>>>>>>` 冲突标记
- **修复**：合并两侧代码，移除冲突标记，保留 `isAllGatherKfc`/`isAllToAllV`/`isAlltoAllKfcMultiJetty` 声明

### 错误3：`ccu_kernel_kfc_server.cc` 缺少声明

- **原因**：AllReduce include 被误删、`HBM_PARAM_IDX_11`~`17` 未定义、`opType` 未声明
- **修复**：恢复 AllReduce include 和常量定义，统一使用 `uint64_t opType`

### 错误4：`CalcResByChannelDescs` 不存在

- **原因**：CCU 模板类没有 `CalcResByChannelDescs` 方法，但 executor 非CCU路径调用它
- **修复**：在 `CcuTempKfcAllToAllMesh1DMultiJetty` 类中添加 stub 方法

### 错误5：producer/server 参数布局错位

- **原因**：producer 端 `FillCachedArgs` 写入 `inputAddr, outputAddr, token, ...`，但 server 端 `param[0]` 是控制字、`param[1]` 是 input、token 从 `ctx.token` 注入
- **修复**：producer 端改为写入 `1ULL, inputAddr, outputAddr, sliceSize, ...`（移除 token），server 端使用 `KFC_CONCURRENT_A2A_*` 常量读取

### 错误6：`inBuffBaseOff`/`outBuffBaseOff` 未设置

- **原因**：`SetTemplateDataParams` 未设置这两个字段，导致两个并发分片和多 loop 都从地址 0 开始读写
- **修复**：在 `SetTemplateDataParams` 中计算 `inBaseOff = (splitData.sdispls[0] + processedCount) * dataTypeSize_`、`outBaseOff = (splitData.rdispls[0] + processedCount) * dataTypeSize_`

### 错误7：算法未注册 warning

- **原因**：`algorithmMap`（`hccl_alloc_ctx_res.h`）缺少 `"CcuSchedAllToAllSoleMeshConcurrent"` 条目
- **修复**：在 `AlgorithmType` 枚举和 `algorithmMap` 中新增条目

### 错误8：tiling version 100 vs 101

- **原因**：`hccl_tiling.cpp` 的 `REGISTERED_CCU_ALGORITHMS` 集合缺少 `"CcuSchedAllToAllSoleMeshConcurrent"`，导致 tiling version 设为 100 而非 101
- **修复**：在集合中新增算法名

### 错误9：device 侧 missionNum 未覆盖 AllToAll

- **原因**：`hccl_ccu_v0.h` 的 `GetKfcMissionNum` 只检查 AllGather concurrent 算法返回双 mission
- **修复**：增加 `CcuSchedAllToAllSoleMeshConcurrent` 条件

### 错误10：device 侧 AlgorithmType 枚举缺失

- **原因**：`hccl_ccu_v0_def.h` 的 `AlgorithmType` 枚举缺少 `CcuSchedAllToAllSoleMeshConcurrent`
- **修复**：在枚举末尾新增

### 错误11：ValidateParam 拒绝 CCU 引擎

- **原因**：`ins_v2_all_to_all_concurrent_executor.cc` 的 `ValidateParam` 第50行 `!IsAicpuEngine(param.engine)` 检查只允许 AICPU 引擎（`COMM_ENGINE_AICPU`/`COMM_ENGINE_AICPU_TS`），engine=5（`COMM_ENGINE_CCU`）被拒绝，返回 `HCCL_E_NOT_SUPPORT`
- **修复**：条件改为 `!IsAicpuEngine(param.engine) && param.engine != CommEngine::COMM_ENGINE_CCU`，放行 CCU 引擎

### 错误12：CalcChannelRequestMeshClosMultiJetty 拒绝 CCU 引擎

- **原因**：`channel.cc:749` 的 `CalcChannelRequestMeshClosMultiJetty` 只允许 AICPU 引擎（`COMM_ENGINE_AICPU`/`COMM_ENGINE_AICPU_TS`），engine=5（`COMM_ENGINE_CCU`）被拒绝
- **修复**：条件增加 `param.engine != CommEngine::COMM_ENGINE_CCU`，放行 CCU 引擎

### 错误13：CCU 路径 mesh 模板使用错误的 channel 计算函数导致 kernel 死锁

- **原因**：CCU 路径两个模板都调用 `CcuTempKfcAllToAllMesh1DMultiJetty::CalcRes`，内部都调用 `CalcChannelRequestMeshClosMultiJetty`。但 mesh 模板应使用 `CalcChannelRequestMesh1DWithPriorityTopo`、clos 模板才用 `CalcChannelRequestMeshClosMultiJetty`。mesh 模板拿到错误 channel 导致 PreSync 死锁
- **修复**：CCU 路径改为在 executor 中分别计算 mesh/clos channel（与非 CCU 路径一致），然后调用 `CalcResByChannelDescs`；将 `CalcResByChannelDescs` 从 stub 改为实际实现（设置 `GetRes`/`ccuKernelNum`/kernelInfo）

### 错误14：CCU mission 1 执行错误

- **现象**：`missionId=1` 处 `CCUM Execute Error(0x09), CCU Mission Task Killed(0x02)`
- **根因**：device 侧 `CcuPrepareForOp` 对 AllToAll 只调用 `CcuPrepareForAllToAll`，写入一组参数。两个 mission（mesh/clos）读同一组参数，处理相同的数据分片，导致地址冲突/执行错误。对比 AllGather 的 `CcuPrepareForConcurrentAllGatherM2M` 在同一个 task entry 中写入 mesh 参数（index 0-14）和 NHR 参数（index 15-29），两个 mission 各取各的
- **修复**：
  1. `kfc_server_message.h`：新增 `KFC_CONCURRENT_A2A_MESH_*`（index 0-10）和 `KFC_CONCURRENT_A2A_CLOS_*`（index 11-20）两套参数索引
  2. `hccl_ccu_v0_prepare.h`：新增 `CcuPrepareForConcurrentAllToAll`，按带宽比例拆分数据，写入 mesh 和 clos 两套参数
  3. `hccl_ccu_v0.h`：`CcuPrepareForOp` 中 AllToAll 分支判断 `GetKfcMissionNum`，KFC 模式调用 `CcuPrepareForConcurrentAllToAll`
  4. `hccl_ccu_v0_def.h`：新增 `CcuPrepareForConcurrentAllToAll` 声明
  5. `kfc_server_protocol.h`：`KfcServerRole` 新增 `ALL_TO_ALL_MESH`/`ALL_TO_ALL_CLOS`，`GetKfcServerRole` 按 missionIndex 返回不同 role
  6. `ccu_kernel_kfc_server.cc`：dispatch 按 role 分别从 `KFC_CONCURRENT_A2A_MESH_*` 和 `KFC_CONCURRENT_A2A_CLOS_*` 读取参数
