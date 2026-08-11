# AllReduce Mesh1D Mem2Mem 算法迁移流程（HCCL → asc-devkit）

## 1. 整体架构对比

### HCCL 仓执行链路

```
HcclAllReduce (all_reduce_op.cc)
  → AllReduceOutPlace
    → Selector (op_common.cc)
      → DecideHcclOpExpansionMode: 环境变量 HCCL_OP_EXPANSION_MODE 决定 CCU_MS/CCU_SCHED
      → AutoSelectorBase::Select (auto_selector_base.cc)
        → SelectCcuScheduleAlgo → SelectCcuScheduleLevel0AlgoMesh1D
          → 数据量检查: dataSize * (8/rankSize²) > 8MB ? NOT_MATCH : MATCH
          → "CcuAllReduceMesh1DMem2Mem"
    → HcclExecOp
      → executor->Orchestrate
        → OrchestrateLoop (分块循环)
          → dataCount_ = param.DataDes.count (实际值)
          → loopTimes = ceil(dataCount / maxDataCountPerLoop)
          → for each loop:
              algTemplate->KernelRun(param, tempAlgParams, templateAlgRes)
                → CalcSlice / CalGoSize / HcommCcuKernelLaunch (直接 launch)
```

### asc-devkit 仓执行链路

```
HcclAllocComResourceByTiling (hccl_mc2.cc)
  → ObtainCommEngine: ccTiling->commEngine 仅允许 CCU_SCHED 或 AICPU_TS
  → AllocComResourceByTilingCcu
    → CcuSelectAlg (hccl_alloc_ctx_res.cc)
      → SelectCcuAlgorithm
        → InitOpParamByTiling
          → PrepareParamForAllReduce: param.DataDes.count = 0 (硬编码)
          → opParam.engine = OpExecuteConfigToCommEngine(ccTiling->commEngine)
        → SelectAlgAndPrepareEngine
          → PrepareTopoInfoForOp
            → HcclGetOpExpansionMode: 环境变量 ASCEND_ENABLE_CCU_KFC_BRANCH=1 → CCU_SCHED
          → AutoSelectorBase::Select
            → SelectCcuScheduleAlgo → SelectCcuScheduleLevel0Algo
              → 数据量检查: 0 * (8/rankSize) >= 8MB ? NO → MATCH
              → "CcuAllReduceMesh1DMem2Mem"
        → algorithmMap 校验: algName 必须在 map 中
      → FillCcuAlgorithmInfo: opResCtx.isKfc[tilingIndex] = true
      → PrepareCcuAlgorithmResource
        → GetCcuOpParamResCtx
          → AcquireAlgResources
            → executor->CalcRes (InsV2AllReduceSoleExecutor)
              → CcuTempAllReduceMeshMem2Mem1D::CalcRes: 注册 channel + kernelInfo
            → KFC Server CalcRes (CcuTempKfcServer::CalcRes)
              → 继承源 kernel 的 channels 和 args
              → 替换 kernelFunc 为 CcuKfcServerKernel
            → HcclAllocAlgResourceCcu: 获取 cclBuffer/token/threads/channels/kernels
            → 序列化 → aclrtMemcpy H2D → 存入 EngineCtx
      → CopyCcuOpParamToDevice: OpParam H2D 拷贝

执行阶段:
CcuKernelLaunch (hccl_mc2.cc)
  → CopyOpResCtxToHost (D2H)
  → CopyOpParamToHost (D2H)
  → LoadResourceCtx (D2H + DeSerialize)
  → GetLaunchHandles: threads[0] + ccuKernels[0] (KFC Server kernel)
  → HcommCcuKernelLaunch(threadHandle, kernelHandle, kfcServerArgs, argSize)
    → CcuKfcServerKernel 执行
      → 读取 xnData (由 AICORE 侧 CcuPrepareForAllReduceM2M 填充)
      → 生成 CircularQueue → CCU 硬件执行
```

## 2. 各层迁移细节

### 2.1 Selector 层

#### 文件映射

| HCCL 仓 | asc-devkit 仓 |
|---------|--------------|
| `hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc` | `asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc` |
| `hccl/src/ops/op_common/selector/auto_selector_base.cc` | `asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/auto_selector_base.cc` |

#### 关键差异

**执行模式决定方式不同**:

- HCCL: `DecideHcclOpExpansionMode` 读取 `HCCL_OP_EXPANSION_MODE` 环境变量，支持 CCU_MS / CCU_SCHED / AICPU 三种模式
- dev-kit: `HcclGetOpExpansionMode` 读取 `ASCEND_ENABLE_CCU_KFC_BRANCH` 环境变量，仅支持 CCU_SCHED / AICPU 两种模式

**数据量检查公式不同**:

```cpp
// HCCL: SelectCcuScheduleLevel0AlgoMesh1D (line 329)
ratio = DEFAULT_RANK_SIZE / topoInfo->userRankSize / topoInfo->userRankSize;  // 8/N²
if (dataSize * ratio > AR_M2M_1D_MAX_DATA_SIZE) { ... }  // 严格大于

// dev-kit: SelectCcuScheduleLevel0Algo (line 67)
ratio = DEFAULT_RANK_SIZE / topoInfo->userRankSize;  // 8/N
if (dataSize * ratio >= AR_M2M_1D_MAX_DATA_SIZE) { ... }  // 大于等于
```

rankSize=2 时: HCCL ratio=2.0（上限 4MB），dev-kit ratio=4.0（上限 2MB）。

**CCU_MS 的 SelectCcuMsAlgo 差异**:

- HCCL: `SelectCcuMsAlgo` → `SelectMeshAlgo`，按数据量路由到 `CcuAllReduceMesh1DOneShot` / `CcuAllReduceMesh1D` / `CcuAllReduceMesh1DMem2Mem` 等不同算法
- dev-kit: `SelectCcuMsAlgo` 无 `SelectMeshAlgo` 中间层，无条件返回 `CcuAllReduceMesh1DMem2Mem`

#### 迁移要点

1. 算法名 `CcuAllReduceMesh1DMem2Mem` 保持一致，通过 `REGISTER_SELECTOR_BY_OPTYPE` 注册
2. `SelectCcuScheduleAlgo` 的检查条件（topo、dataType、reduceOp、inplace）直接移植
3. 数据量检查公式需根据实际场景调整 ratio 计算
4. CCU_MS 路径可选保留，但需补充数据量路由逻辑

### 2.2 Executor 层

#### 文件映射

| HCCL 仓 | asc-devkit 仓 |
|---------|--------------|
| `hccl/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc` | `asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc` |

#### 注册方式

```cpp
// HCCL
REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_ALLREDUCE, CcuAllReduceMesh1DMem2Mem, InsV2AllReduceSoleExecutor, TopoMatch1D,
    CcuTempAllReduceMeshMem2Mem1D);

// dev-kit (完全相同)
REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_ALLREDUCE, CcuAllReduceMesh1DMem2Mem, InsV2AllReduceSoleExecutor, TopoMatch1D,
    CcuTempAllReduceMeshMem2Mem1D);
```

#### OrchestrateLoop 分块逻辑（两仓一致）

```cpp
dataCount_ = param.DataDes.count;
templateScratchMultiplier = algTemplate->CalcScratchMultiple();  // = rankSize
maxTmpMemSize_ = cclMem.size;
transportBoundDataSize = UB_MAX_DATA_SIZE;  // 256MB
scratchBoundDataSize = maxTmpMemSize_ / templateScratchMultiplier / HCCL_MIN_SLICE_ALIGN * HCCL_MIN_SLICE_ALIGN;
maxDataSizePerLoop = min(transportBoundDataSize, scratchBoundDataSize);
maxDataCountPerLoop = maxDataSizePerLoop / dataTypeSize_;
loopTimes = ceil(dataCount_ / maxDataCountPerLoop);

for (loop = 0; loop < loopTimes; loop++) {
    currDataCount = min(maxDataCountPerLoop, dataCount_ - processedDataCount);
    tempAlgParams.sliceSize = currDataCount * dataTypeSize_;
    algTemplate->KernelRun(param, tempAlgParams, templateAlgRes);
    processedDataCount += currDataCount;
}
```

#### 关键差异

- HCCL: `param.DataDes.count` 为实际值，分块循环正常执行，每轮调用 `KernelRun` 执行一次 CCU launch
- dev-kit: `param.DataDes.count = 0`（`PrepareParamForAllReduce` 硬编码），`loopTimes=0`，分块循环不执行。实际执行依赖 KFC Server 机制

#### 迁移要点

1. Executor 模板类 `InsV2AllReduceSoleExecutor` 整体移植，包括 `CalcAlgHierarchyInfo`、`CalcRes`、`Orchestrate`、`OrchestrateLoop`
2. `OrchestrateLoop` 的分块逻辑保持一致，依赖 `param.DataDes.count` 正确赋值
3. HCCL 仓额外有 `FastLaunchSaveCtx` / `FastLaunch` 快速路径，dev-kit 未移植（KFC Server 替代）

### 2.3 Template 层

#### 文件映射

| HCCL 仓 | asc-devkit 仓 |
|---------|--------------|
| `hccl/src/ops/all_reduce/template/ccu/ccu_temp_all_reduce_mesh_1D_mem2mem.{h,cc}` | `asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/ccu/ccu_temp_all_reduce_mesh_1D_mem2mem.{h,cc}` |

#### 基类差异

- HCCL: `CcuAlgTemplateBase`
- dev-kit: `CcuTempKfcMesh1DMem2MemBase` → `CcuAlgTemplateBase`（增加了 KFC 中间层）

#### CalcRes 差异

**HCCL** (`ccu_temp_all_reduce_mesh_1D_mem2mem.cc:68-115`):
```cpp
CcuTempAllReduceMeshMem2Mem1D::CalcRes(...) {
    resourceRequest.ccuKernelNum.push_back(1);
    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, "CcuKernelAllReduceMesh1DMem2Mem");
    kernelInfo.kernelFunc = reinterpret_cast<void*>(CcuAllReduceMeshMem2Mem1DKernel);  // 直接绑定 kernel
    // 构建 kernelArg
    auto kernelArg = std::make_shared<CcuKernelArgAllReduceMeshMem2Mem1D>();
    kernelArg->rankSize = subCommRanks_[0].size();
    kernelArg->rankId = mySubCommRank_;
    kernelArg->opParam = param;
    kernelArg->subCommRanks = subCommRanks_;
    kernelInfo.setKernelArg(kernelArg);
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
}
```

**dev-kit** (`ccu_temp_all_reduce_mesh_1D_mem2mem.cc:62-93`):
```cpp
CcuTempAllReduceMeshMem2Mem1D::CalcRes(...) {
    resourceRequest.ccuKernelNum.push_back(1);
    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, "CcuKernelAllReduceMesh1DMem2Mem");
    // 不绑定 kernelFunc！由后续 KFC Server CalcRes 替换为 CcuKfcServerKernel
    // 不构建 kernelArg！由 InheritKfcServerKernelArg 继承
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
}
```

#### KernelRun 差异

**HCCL** (`ccu_temp_all_reduce_mesh_1D_mem2mem.cc:207-254`): 完整实现
```cpp
KernelRun(...) {
    CalcSlice(sliceSize, sliceInfoVec);           // 分片计算
    inputAddr = inputPtr + inBuffBaseOff;          // 地址计算
    goSize = CalGoSize(normalSliceSize, config, GetCcuVersion());  // goSize 计算
    BuildTaskArgs(...);                            // 构建 15 个参数
    HcommCcuKernelLaunch(threads[0], ccuKernels[0], taskArgs, argSize);  // 直接 launch
    SaveSubmitInfo(...);                           // 保存用于 FastLaunch
}
```

**dev-kit** (`ccu_temp_all_reduce_mesh_1D_mem2mem.cc:102-110`): 空实现
```cpp
KernelRun(...) {
    (void)param; (void)templateDataParams; (void)templateResource;
    return HCCL_SUCCESS;  // 参数由 AICORE 侧 CcuPrepareForAllReduceM2M 计算
}
```

#### 迁移要点

1. `CalcSlice` 分片逻辑移植（按 `unitAllignSize` 对齐）
2. `CalcRes` 中 channel 构建逻辑移植（`CalcChannelRequestMesh1D` / `CalcChannelRequestMesh1DWithPriorityTopo`）
3. `CalcScratchMultiple` 返回 `templateRankSize_`（scratch = rankSize × sliceSize）
4. `KernelRun` 在 KFC Server 模式下为空，参数计算转移到 AICORE 侧

### 2.4 KFC Server 层（dev-kit 独有）

#### 文件

```
asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/kfc_server/
├── selector/kfc_server_auto_selector.cc    # 注册 "CcuKfcServer"
├── executor/ins_kfc_server_sole_executor.cc # 注册 executor
└── template/ccu/
    ├── ccu_temp_kfc_server.{h,cc}           # CalcRes: 继承源 kernel 属性
    └── kernel/ccu_kernel_kfc_server.{h,cc}  # CcuKfcServerKernel: 生成 CircularQueue
```

#### KFC Server 工作机制

1. **资源准备阶段** (`HcclGetAlgRes` → `CcuTempKfcServer::CalcRes`):
   - 从 `ccuKernelInfos.back()` 继承 channels
   - 通过 `InheritKfcServerKernelArg` 构建 `CcuKernelArgKfcServer`（含 rankSize、rankId、opParam、subCommRanks）
   - 替换 `kernelFunc` 为 `CcuKfcServerKernel`
   - 替换 `ccuKernelInfos.back()` 为 KFC Server kernel

2. **参数准备阶段** (AICORE 侧 `CcuPrepareForAllReduceM2M`):
   - `hccl_ccu_v0.h:258-263`: `CcuPrepareForOp` 根据 `prepareType` 分发到 `CcuPrepareForAllReduceM2M`
   - `hccl_ccu_v0_prepare.h:151-207`: 填充 `xnData_[0..14]`（15 个参数）
   - 参数包括: sendBuf/recvBuf 地址、scratch 地址、sliceOffset、normalSliceSize、lastSliceSize、goSize 等
   - `CalcGoSize` 使用 `CCU_LOOP_COUNT_M2M_RE=16` 和 `CCU_MEMSLICE_SIZE=4096`

3. **执行阶段** (`CcuKernelLaunch` → `HcommCcuKernelLaunch`):
   - `hccl_mc2.cc:486-517`: D2H 拷贝 OpResCtx/OpParam/resCtx，反序列化，获取 threadHandle/kernelHandle
   - `kfcServerArgs` = {xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token}
   - `HcommCcuKernelLaunch(threadHandle, kernelHandle, kfcServerArgs, 6)`
   - `CcuKfcServerKernel`: 读取 xnData → `KfcServerGenCircularQueue` → CCU 硬件执行

### 2.5 Kernel 层

#### 文件映射

| HCCL 仓 | asc-devkit 仓 |
|---------|--------------|
| `hccl/src/ops/all_reduce/template/ccu/kernel/ccu_kernel_all_reduce_mesh1d_mem2mem.{h,cc}` | `asc-devkit/impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/ccu/kernel/ccu_kernel_all_reduce_mesh1d_mem2mem.{h,cc}` |

#### 函数签名差异

```cpp
// HCCL: 通过 CcuKernelArg 封装 + ccu::LoadArg 加载
CcuResult CcuAllReduceMeshMem2Mem1DKernel(CcuKernelArg arg);

// dev-kit: 直接参数传递
CcuResult CcuAllReduceMeshMem2Mem1DKernel(
    ccu::Variable inputAddr, ccu::Variable outputAddr, ...,  // 15 个 ccu::Variable
    const ChannelHandle channels[], uint32_t channelCount, uint32_t rankSize, uint32_t rankId,
    const HcclDataType& dataType, const HcclDataType& outputDataType, const HcclReduceOp& reduceOp);
```

#### Context 结构差异

```cpp
// HCCL: 参数通过 arg 指针访问
struct AllReduceMeshMem2Mem1DContext : CcuKernelCtxBase {
    const CcuKernelArgAllReduceMeshMem2Mem1D* arg;  // 指向 kernelArg
    // ...
};

// dev-kit: 参数直接存在 ctx 中
struct AllReduceMeshMem2Mem1DContext : CcuKernelCtxBase {
    const ChannelHandle* channels{nullptr};
    uint32_t channelCount{0};
    uint32_t rankSize{0};
    uint32_t rankId{0};
    // ... (无 arg 指针)
};
```

#### Kernel 核心逻辑（两仓一致）

```
CcuAllReduceMeshMem2Mem1DKernel:
  1. InitResource: 从 channels 获取 input/output/token 变量
  2. LoadArgs: 加载 15 个参数到 ctx
  3. PreSync: WriteVariableWithNotify + NotifyWait (交换 input/output/token 地址)
  4. DoRepeatAllReduce (if mySliceSize != 0):
     a. ReduceRmtToLoc:
        - 对每个 remote rank: ccu::Read(sliceSize) → 写入 scratch
        - EventWait 等待所有 Read 完成
        - DoLocalReduce: ReduceLoopGroup (Loop 内 LocalCopy + LocalReduce)
     b. BcastLocToRmt:
        - 对每个 remote rank: ccu::Write(sliceSize) → 写入远端 output
        - EventWait 等待所有 Write 完成
  5. PostSync: NotifyRecord + NotifyWait
```

#### CcuVersion 支持

- HCCL: `ReduceLoopGroup` 动态选择 V1/V2，V2 使用 3 个独立 CKE 事件实现流水化
- dev-kit: 仅 V1（`ReduceLoopGroup` 硬编码 "select ReduceLoopGroupV1"），V2 未移植

#### 迁移要点

1. Kernel 核心逻辑（PreSync / ReduceRmtToLoc / BcastLocToRmt / PostSync）整体移植
2. 函数签名从 `CcuKernelArg` 封装改为直接参数传递
3. Context 结构从指针引用改为值存储
4. `ccu::LoadArg` 机制改为 `LoadArgs` 直接赋值
5. `GetCcuVersion()` 动态选择改为硬编码 V1

## 3. 参数传递全链路

### HCCL 仓参数流

```
AllReduceOutPlace
  → param.DataDes.count = count (实际值)
  → Selector → algName
  → HcclExecOp → Orchestrate
    → dataCount_ = param.DataDes.count
    → OrchestrateLoop 分块
      → KernelRun
        → CalcSlice(dataSize) → normalSliceSize, lastSliceSize, mySliceSize
        → CalGoSize(sliceSize, config) → goSize[4]
        → BuildTaskArgs → 15 个 uint64_t
        → HcommCcuKernelLaunch(thread, kernel, taskArgs, 15)
          → CcuAllReduceMeshMem2Mem1DKernel(CcuKernelArg)
            → ccu::LoadArg × 15
```

### dev-kit 仓参数流

```
[资源准备阶段 - Host]
HcclAllocComResourceByTiling
  → PrepareParamForAllReduce
    → param.DataDes.count = 0  ← 硬编码（待修复）
    → param.DataDes.dataType = ccTiling->srcDataType
    → param.reduceType = ccTiling->reduceType
  → SelectAlgAndPrepareEngine → algName = "CcuAllReduceMesh1DMem2Mem"
  → GetCcuOpParamResCtx
    → executor->CalcRes → CcuTempAllReduceMeshMem2Mem1D::CalcRes (注册 channel)
    → CcuTempKfcServer::CalcRes (替换为 KfcServer kernel)
    → HcclAllocAlgResourceCcu (获取 cclBuffer/token/thread/channel/kernel)
    → 序列化 → H2D → EngineCtx
  → CopyCcuOpParamToDevice (OpParam H2D)

[参数准备阶段 - AICORE]
CcuPrepareForAllReduceM2M (hccl_ccu_v0_prepare.h)
  → xnData_[0] = opId
  → xnData_[1] = sendBuf + offset
  → xnData_[2] = recvBuf + offset
  → xnData_[3] = scratchAddr
  → xnData_[4..5] = rankSliceOffset
  → xnData_[6..8] = normalSliceSize / lastSliceSize / mySliceSize
  → xnData_[9] = sliceOffset
  → xnData_[10] = isInputOutputEqual
  → xnData_[11..14] = CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE)
  → CcuSendMsg → 写入 CCU 硬件队列

[执行阶段 - Host]
CcuKernelLaunch (hccl_mc2.cc)
  → D2H: OpResCtx / OpParam / resCtx
  → DeSerialize → threads[0] / ccuKernels[0] (KfcServer)
  → kfcServerArgs = {xnAddr, ckeAddr, dieNum=1, missionNum=1, missionIndex=0, token}
  → HcommCcuKernelLaunch(thread, kernel, kfcServerArgs, 6)
    → CcuKfcServerKernel
      → 读取 xnData → KfcServerGenCircularQueue → CCU 硬件执行
```

## 4. 已知问题与修复方向

### 4.1 count=0 导致分块失效（待修复）

**位置**: `hccl_alloc_ctx_res.cc:425`

```cpp
param.DataDes.count = 0;  // 应从 ccTiling 获取实际值
```

**影响**: `OrchestrateLoop` 中 `loopTimes=0`，分块循环不执行，大数据无法分块传输

**修复方向**: 从 `Mc2CcTilingInner` 或运行时参数获取实际 count

### 4.2 数据量检查失效

**原因**: count=0 → dataSize=0 → 数据量检查永远通过

**影响**: 任意大小数据都会选中 `CcuAllReduceMesh1DMem2Mem`，该算法单次 Read 整个 slice，大数据触发网络超时

**修复方向**: 修复 count 后自然生效，或独立增加数据量上限检查

### 4.3 CcuVersion V2 未移植

**影响**: V2 的 3-CKE 事件流水化机制对大数据性能更优

**修复方向**: 移植 `ReduceLoopGroupV2` 和 `CreateReduceLoopV2`，通过 `GetCcuVersion()` 动态选择

### 4.4 CCU_MS 路径数据量路由缺失

**位置**: dev-kit `SelectCcuMsAlgo` 无 `SelectMeshAlgo` 中间层

**影响**: CCU_MS 模式下无条件选中 `CcuAllReduceMesh1DMem2Mem`

**修复方向**: 补充 `SelectMeshAlgo` 数据量路由，或限制 CCU_MS 仅处理小数据

## 5. 迁移检查清单

- [ ] Selector: 算法名注册一致
- [ ] Selector: 检查条件（topo/dataType/reduceOp/inplace）移植完整
- [ ] Selector: 数据量检查公式适配
- [ ] Executor: `REGISTER_EXEC_V2` 注册一致
- [ ] Executor: `OrchestrateLoop` 分块逻辑一致
- [ ] Template: `CalcRes` channel 构建逻辑移植
- [ ] Template: `CalcScratchMultiple` 返回值一致
- [ ] Template: `KernelRun` 适配 KFC Server 模式
- [ ] Kernel: 核心逻辑（PreSync/ReduceRmtToLoc/BcastLocToRmt/PostSync）移植完整
- [ ] Kernel: 函数签名适配（CcuKernelArg → 直接参数）
- [ ] KFC Server: `InheritKfcServerKernelArg` 正确继承属性
- [ ] AICORE: `CcuPrepareForAllReduceM2M` 参数计算正确
- [ ] 参数传递: `param.DataDes.count` 正确赋值（待修复）
- [ ] CcuVersion: V1/V2 动态选择（V2 待移植）
