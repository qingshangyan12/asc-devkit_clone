# CCU 算子迁移方案（参照 hccl → asc-devkit KFC Server）

## 1. MC2 到算子的调用路径与框架接入点

MC2 算子不直接调用算子代码，而是通过 GE 框架到算子。整个过程分两个阶段：**资源分配**（加载图时，一次性）和**算子执行**（每次运行时）。框架在 7 个位置查找算子实现，每个位置对应一个需修改的文件：

```
=== 资源分配阶段（host，一次性）===

MC2 tiling（声明 opType + commEngine + group）
  → GE HcomCreateComResourceMC2 → HcomGetA5AicpuContext
    → dlsym HcclAllocComResourceByTilingA5Mc2 from libmc2_client.so
      → asc-devkit HcclAllocComResourceByTiling
        → CcuSelectAlg (hccl_alloc_ctx_res.cc:832)
          → SelectAlgAndPrepareEngine
            → ExecuteSelector::Run
              → SelectorRegistry::GetSelectorsByOpType(opType)   ← ① Selector: selector->Select() 返回 algName
          → GetAlgExecutorForOp(opType, algName)
            → CollAlgExecRegistryV2::GetAlgExec(opType, algName) ← ② Executor: 按 opType+algName 查找 executor
          → HcclGetAlgRes (op_common.cc:731)
            → executor->CalcRes → CcuTemp<Op>::CalcRes           ← ③ Template: 填 channels（不设 kernelFunc）
            → 检测 isKfc → CcuTempKfcServer::CalcRes（替换为 CcuKfcServerKernel）
            → HcclGetCcuKernel → HcommCcuKernelRegister

=== 算子执行阶段（device + CCU，每次调用）===

device AIV kernel（MC2 算子内）:
  AscendC::Hccl.<Op>(sendBuf, recvBuf, count, ...)
    → CommonPrepareImpl（存 sendBuf/recvBuf/count/opType 到 handleParamGM_）
  AscendC::Hccl.Commit(handleId)
    → CommitMsgInner → CcuPrepareForOp(handleId)        ← ④ Device 分发: 按 opType if-else
      → CcuPrepareFor<Op>M2M（写 xnData_[] 参数到 HBM）   ← ⑤ Device 参数: 参数计算逻辑来自 hccl KernelRun
    → CcuSendMsg（FlushDataCache 写 HBM + 通知 CCU）
  AscendC::Hccl.Wait(handleId)
    → 轮询 waitCKEAddr（CCU 完成后写非零值）
  AscendC::Hccl.Finalize()
    → 写 INVALID_U64 sentinel → KFC Server 退出循环

CCU 侧（KFC Server，资源分配时已注册，持久运行）:
  CcuKfcServerKernel → KfcServerGenCircularQueue（循环）
    → 轮询 HBM commitCKEAddr
    → LoadFuncParamFromMemory（从 HBM xnAddr 读 32 参数）
    → DispatchKfcSubKernel（ctx, param）                 ← ⑥ KFC Dispatch: 按 opType if-else
      → Ccu<Op>Kernel（显式参数）                         ← ⑦ Kernel: 算法实现（参照 hccl kernel）
    → SetTurnEndSig（写 waitCKEAddr 通知 device Wait 返回）
```

| 接入点 | 框架查找方式 | 需修改文件 | hccl 参照 |
|--------|------------|-----------|----------|
| ① Selector | `SelectorRegistry::GetSelectorsByOpType(opType)` 按 priority 遍历 | `<op>_auto_selector.h` + `.cc`（新建） | hccl selector |
| ② Executor | `CollAlgExecRegistryV2::GetAlgExec(opType, algName)` | `ins_v2_<op>_sole_executor.cc`（新建） | hccl executor |
| ③ Template | executor `CalcRes` → `CcuTemp<Op>::CalcRes` | `ccu_temp_<op>_mesh_1D_mem2mem.h` + `.cc`（新建） | hccl template |
| ④ Device 分发 | `CcuPrepareForOp` 按 opType if-else | `hccl_ccu_v0.h`（修改） | 无（asc-devkit 独有） |
| ⑤ Device 参数 | `CcuPrepareFor<Op>M2M` 写 `xnData_[]` | `hccl_ccu_v0_prepare.h`（修改） | hccl `KernelRun`/`CalcSlice` 参数计算 |
| ⑥ KFC Dispatch | `DispatchKfcSubKernel` 按 opType if-else | `ccu_kernel_kfc_server.cc`（修改） | 无（asc-devkit 独有） |
| ⑦ Kernel | `DispatchKfcSubKernel` 调 `Ccu<Op>Kernel(显式参数)` | `ccu_kernel_<op>_mesh1d_mem2mem.h` + `.cc`（新建） | hccl kernel |

> 以下按运行时调用顺序（①→⑦）逐文件说明。

---

## 2. 逐文件迁移方案

### ① Selector `.h` + `.cc`

**路径**：`impl/adv_api/detail/hccl/cc/src/ops/<op>/selector/<op>_auto_selector.h` + `.cc`
**hccl 参照**：`hccl/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`

**框架调用代码**（`execute_selector.cc:36-42`）：
```cpp
selectors = SelectorRegistry::Global()->GetSelectorsByOpType(opParam.opType); // 按 opType 查找
for (auto iter : selectors) {
    if (iter.second->Select(opParam, topoInfo, selectAlgName) == SelectorStatus::MATCH) {
        return HCCL_SUCCESS; // selectAlgName = "CcuAllReduceMesh1DMem2Mem"
    }
}
```

**hccl 实现**（`all_reduce_auto_selector.cc:42-80`，多路径 + 数据量分支）：
```cpp
SelectorStatus AllReduceAutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo,  // 拓扑信息（层级数/mesh类型/rankSize）
    const OpParam &opParam,                        // 算子参数（dataType/reduceType/count）
    const std::map<HcclCMDType, std::vector<HcclAlgoType>> &configAlgMap, // 配置的算法映射（未用）
    std::string &selectAlgName) const              // [out] 选中的算法名
{
    // 校验：topoLevelNums<=1、非INT8、非PROD、非64bit
    // ...
    return SelectMeshAlgo(topoInfo, opParam, selectAlgName);
    // SelectMeshAlgo 内部按数据量/拓扑分支返回多种算法名：
    //   "CcuAllReduceMesh1DMem2Mem" / "CcuAllReduceNHR1D" / "CcuAllReduceSequenceMesh1D" / ...
}
```

**迁移后实现**（参照 `reduce_scatter_auto_selector.cc:20-52`，只保留 CCU_MS 路径 + 硬编码算法名）：
```cpp
SelectorStatus <Op>AutoSelector::SelectCcuMsAlgo(
    const TopoInfoWithNetLayerDetails* topoInfo,  // 拓扑信息（层级数/mesh类型/rankSize）
    const OpParam& opParam,                        // 算子参数（dataType/reduceType/count）
    const std::map<HcclCMDType, std::vector<HcclAlgoType>>& configAlgMap, // 配置的算法映射（未用）
    std::string& selectAlgName) const              // [out] 选中的算法名
{
    // 复用 hccl 校验：topoLevelNums<=1、非INT8、非PROD、非64bit
    // ...
    selectAlgName = "Ccu<Op>Mesh1DMem2Mem"; // 硬编码，不再按数据量分支
    return SelectorStatus::MATCH;
}
// 文件末尾注册（hccl 无此宏，asc-devkit 独有）
REGISTER_SELECTOR_BY_OPTYPE(HcclCMDType::HCCL_CMD_<OP>, 18, <Op>AutoSelector);
// 参数：opType枚举, priority(18=MC2 CCU模式), selector类名
```

**改造说明**：复用 hccl `SelectCcuMsAlgo` 校验逻辑；删除 `SelectCcuScheduleAlgo`/`SelectAicpuAlgo`/`SelectMeshAlgoCcums`；删除 `SelectMeshAlgo` 内部数据量分支（只保留 mem2mem 一种算法）；namespace 改 `mc2_ops_hccl`；新增 `REGISTER_SELECTOR_BY_OPTYPE`。算法名须与 ② 一致。

> ReduceScatter 迁移时保留了 dead code（已知问题 P4-9），新迁移不应重复。

**ReduceScatter 参照**：`reduce_scatter_auto_selector.h` + `.cc:20-52,391` ✅

---

### ② Executor `.cc`

**路径**：`impl/adv_api/detail/hccl/cc/src/ops/<op>/executor/ins_v2_<op>_sole_executor.cc`
**hccl 参照**：`hccl/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc:280`

**框架调用代码**（`hccl_alloc_ctx_res.cc:534`）：
```cpp
// 按 opType + algName 查找 executor
return CollAlgExecRegistryV2::Instance().GetAlgExec(opParam.opType, algName);
// 返回 InsV2AllReduceSoleExecutor 实例，框架调 executor->CalcRes → template CalcRes
```

**hccl 实现**（`ins_v2_all_reduce_sole_executor.cc:280`，直接移植）：
```cpp
namespace ops_hccl {
REGISTER_EXEC_V2(HcclCMDType::HCCL_CMD_ALLREDUCE, CcuAllReduceMesh1DMem2Mem,
                 InsV2AllReduceSoleExecutor, TopoMatch1D, CcuTempAllReduceMeshMem2Mem1D);
// 参数：opType, 算法名(与①一致), executor模板类, topo匹配类, template类(指向③)
}
```

**迁移后实现**（只改 namespace，其余不变）：
```cpp
namespace mc2_ops_hccl { // ops_hccl → mc2_ops_hccl
REGISTER_EXEC_V2(HcclCMDType::HCCL_CMD_<OP>, Ccu<Op>Mesh1DMem2Mem,
                 InsV2<Op>SoleExecutor, TopoMatch1D, CcuTemp<Op>MeshMem2Mem1D);
// 参数：opType, 算法名(与①一致), executor模板类, topo匹配类, template类(指向③)
}
```

**改造说明**：复用 hccl `InsV2<Op>SoleExecutor` 模板类（executor 逻辑通用）；namespace 改 `mc2_ops_hccl`；算法名与 ① 一致；template 类指向 ③。

**ReduceScatter 参照**：`ins_v2_reduce_scatter_sole_executor.cc:238` ✅

---

---

### ③ Template `.h` + `.cc`

**路径**：`impl/adv_api/detail/hccl/cc/src/ops/<op>/template/ccu/ccu_temp_<op>_mesh_1D_mem2mem.h` + `.cc`
**hccl 参照**：`hccl/src/ops/all_reduce/template/ccu/ccu_temp_all_reduce_mesh_1D_mem2mem.h` + `.cc`（239 行）

**框架调用代码**（`op_common.cc:771-802`）：
```cpp
// 框架调 executor->CalcRes → CcuTemp<Op>::CalcRes
calcResRet = executor->CalcRes(comm, param, topoInfo, algHierarchyInfo, resRequest);
// 参数：comm(通信域), param(算子参数), topoInfo(拓扑), algHierarchyInfo(算法层级), resRequest([out]资源请求)
// 检测 isKfc，替换为 CcuKfcServerKernel
if (param.isKfc && param.engine == COMM_ENGINE_CCU) {
    auto ccuKfcExecutor = CollAlgExecRegistryV2::Instance().GetAlgExec(
        HcclCMDType::HCCL_CMD_KFC_SERVER, "CcuKfcServer");
    ccuKfcExecutor->CalcRes(comm, param, topoInfo, algHierarchyInfo, resRequest);
    // 复用同一 resRequest，在已有 ccuKernelInfos 后追加 CcuKfcServerKernel 的 kernelInfo
}
// 框架调 HcclGetCcuKernel → HcommCcuKernelRegister(kernelFunc, kernelArg)
```

**hccl 实现**（`ccu_temp_all_reduce_mesh_1D_mem2mem.cc:70-110,195-230`，CalcRes 设 kernelFunc + KernelRun 直接下发）：
```cpp
HcclResult CcuTempAllReduceMeshMem2Mem1D::CalcRes(...) {
    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, ..., "CcuKernelAllReduceMesh1DMem2Mem");
    kernelInfo.kernelFunc = reinterpret_cast<void*>(CcuAllReduceMeshMem2Mem1DKernel); // 设 kernelFunc
    auto kernelArg = std::make_shared<CcuKernelArgAllReduceMeshMem2Mem1D>();
    kernelArg->rankSize = subCommRanks_[0].size();  // 参数: rankSize
    kernelArg->rankId = mySubCommRank_;              // 参数: rankId
    kernelArg->opParam = param;                      // 参数: opParam(含dataType/reduceType)
    kernelArg->subCommRanks = subCommRanks_;         // 参数: subCommRanks
    kernelInfo.setKernelArg(kernelArg);              // 设 kernelArg
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HCCL_SUCCESS;
}

HcclResult CcuTempAllReduceMeshMem2Mem1D::KernelRun(const OpParam& param,
    const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    // 计算 15 个 taskArgs 参数
    CalcSlice(templateDataParams.sliceSize, sliceInfoVec);
    const uint64_t inputAddr = ...;                    // 参数[0]: inputAddr（发送缓冲基地址+偏移）
    const uint64_t outputAddr = ...;                   // 参数[1]: outputAddr（接收缓冲基地址+偏移）
    const uint64_t token = ...;                        // 参数[2]: token（通知令牌）
    const uint64_t scratchAddr = ...;                  // 参数[3]: scratchAddr（CCU MS 中间结果地址）
    const uint64_t currentRankSliceInputOffset = ...;  // 参数[4]: 本rank输入偏移（inputSliceStride * myRank）
    const uint64_t currentRankSliceOutputOffset = ...; // 参数[5]: 本rank输出偏移（outputSliceStride * myRank）
    const uint64_t normalSliceSize = ...;              // 参数[6]: 均分时每rank的slice大小
    const uint64_t lastSliceSize = ...;                // 参数[7]: 最后一个rank的slice大小（非均分时不同）
    const uint64_t mySliceSize = ...;                  // 参数[8]: 本rank实际slice大小
    const uint64_t sliceOffset = ...;                  // 参数[9]: slice在buffer中的偏移
    const uint64_t isInputOutputEqual = ...;           // 参数[10]: 输入输出地址是否相同（0/1）
    const std::vector<uint64_t> goSize = CalGoSize(normalSliceSize, config, GetCcuVersion()); // 参数[11-14]: goSize[4]（分块参数）
    BuildTaskArgs(inputAddr, outputAddr, token, scratchAddr, ..., goSize, taskArgs);

    // 直接下发
    HcommCcuKernelLaunch(templateResource.threads[0], templateResource.ccuKernels[0],
                         taskArgs.data(), taskArgs.size());
    return HCCL_SUCCESS;
}
```

**迁移后实现**（参照 `ccu_temp_reduce_scatter_mesh_1D_mem2mem.cc:44-69,86-97`，CalcRes 只填 channels + KernelRun 空壳）：
```cpp
HcclResult CcuTemp<Op>MeshMem2Mem1D::CalcRes(...) {
    CcuKernelInfo kernelInfo;
    // 不设 kernelFunc / kernelFuncName / setKernelArg（由 CcuTempKfcServer::CalcRes 替换）
    std::vector<HcclChannelDesc> channelDescs;
    CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs);
    // 参数：comm(通信域), param(算子参数), topoInfo(拓扑), subCommRanks_(子通信域rank列表), channelDescs([out]channel描述)
    kernelInfo.channels = channelDescs;  // 只填 channels
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HCCL_SUCCESS;
}

HcclResult CcuTemp<Op>MeshMem2Mem1D::KernelRun(...) {
    /* KFC Server 统一下发，参数计算移到 ⑤ device 侧，此处不再直接 launch
    CalcSlice(...);  // 删除：参数计算移到 ⑤
    BuildTaskArgs(inputAddr, ..., goSize, taskArgs);  // 删除
    HcommCcuKernelLaunch(...);  // 删除：由 KFC Server 接管
    */
    return HCCL_SUCCESS;
}
```

**改造说明**：
- `.h`：复用 hccl 类声明（继承 `CcuAlgTemplateBase`），namespace 改 `mc2_ops_hccl`
- `.cc` CalcRes：复用 hccl channel 计算逻辑；删除 `kernelFunc`/`kernelFuncName`/`setKernelArg`/`CalcSlice`（KFC 由 `CcuTempKfcServer::CalcRes` 替换为 `CcuKfcServerKernel`；参数计算移到 ⑤）
- `.cc` KernelRun/FastLaunch：改为空壳（`OrchestrateLoop` 仍调用，保留签名注释掉 `HcommCcuKernelLaunch` 调用和参数计算）

**ReduceScatter 参照**：`ccu_temp_reduce_scatter_mesh_1D_mem2mem.h` + `.cc` ✅

---

### ④ Device 侧分发（修改现有）

**路径**：`impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0.h`

**框架调用代码**（`hccl_ccu_v0.h:255-283`，Commit 时触发，需新增分支）：
```cpp
__aicore__ inline void HcclImpl<...>::CcuPrepareForOp(const HcclHandle handleId) {
    if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_ALLGATHER) {
        ccuUsedXnNum_ = 9;
        CcuPrepareForAllGatherM2M(&handleParamGM_[handleId]);
    } else if (... == HcclCMDType::HCCL_CMD_ALLREDUCE) {
        ccuUsedXnNum_ = 15;
        CcuPrepareForAllReduceM2M(&handleParamGM_[handleId]);
    } else if (... == HcclCMDType::HCCL_CMD_REDUCE_SCATTER) {
        ccuUsedXnNum_ = 13;  // 参数: xnData_ 使用的 xn 数量
        CcuPrepareForReduceScatterM2M(&handleParamGM_[handleId]);
    }
    // ← 新增: else if (... == HCCL_CMD_<OP>) { ccuUsedXnNum_ = N; CcuPrepareFor<Op>M2M(...); }
}
```

**迁移后实现**（新增 opType 分支）：
```cpp
} else if (handleParamGM_[handleId].commType.prepareType == HcclCMDType::HCCL_CMD_<OP>) {
    ccuUsedXnNum_ = <N>; // 参数: xnData_ 使用的 xn 数量，与 ⑤ 写入的 xnData_ 数量一致
    CcuPrepareFor<Op>M2M(&handleParamGM_[handleId]);
}
```

**改造说明**：在 `CcuPrepareForOp` if-else 链中新增 opType 分支，指向 ⑤ 的 `CcuPrepareFor<Op>M2M`。`ccuUsedXnNum_` 需与 ⑤ 实际写入的 `xnData_` 数量一致。

**ReduceScatter 参照**：`hccl_ccu_v0.h:277-279` ✅

---

### ⑤ Device 侧参数填充（修改现有）

**路径**：`impl/adv_api/detail/hccl/ccu/hccl_ccu_v0_prepare.h`
**hccl 参照**：hccl template `KernelRun`/`CalcSlice` 中的参数计算逻辑

框架入口：④ 调用 `CcuPrepareFor<Op>M2M`，函数将算子运行时参数写入 `xnData_[]`，后续由 `CcuSendMsg` 写入 HBM 供 KFC Server 读取。

hccl 在 host 侧 `KernelRun` 中构造 taskArgs（inputAddr/outputAddr/scratchAddr/sliceSize/offset/goSize 等）直接传给 kernel。asc-devkit 改为 device AIV kernel 写 `xnData_[]` 到 HBM。因此：

- **新增** `CcuPrepareFor<Op>M2M` 函数，将 hccl `KernelRun`/`CalcSlice` 的参数计算逻辑**改为**写入 `xnData_[]`
- `xnData_[0]` 固定 `GetOpId`（KFC Server sentinel）
- 其余索引与 ⑥ `DispatchKfcSubKernel` 的 `param[]` 一一对应

```cpp
__aicore__ inline void HcclImpl<...>::CcuPrepareFor<Op>M2M(
    __gm__ CommonPrepareParamCcu* commParam)  // MC2算子调用时传入的运行时参数（sendBuf/recvBuf/count/dataType/...）
{
    xnData_[0] = GetOpId(commParam);           // sentinel（KFC Server 用作 mission 标识）
    xnData_[1] = sendBuf + offset;             // → param[1] inputAddr（发送缓冲 + 偏移）
    xnData_[2] = recvBuf + offset;             // → param[2] outputAddr（接收缓冲 + 偏移）
    xnData_[3] = ccuParam_.scratchAddr;        // → param[3] scratch（CCU MS 中间结果地址）
    xnData_[4] = rankSliceOffset;              // → param[4] currentRankSliceInputOffset（本 rank 输入偏移）
    xnData_[5] = rankSliceOutputOffset;        // → param[5] currentRankSliceOutputOffset（本 rank 输出偏移）
    xnData_[6] = normalSliceSize;              // → param[6] normalSliceSize（均分时每 rank 的 slice 大小）
    xnData_[7] = lastSliceSize;                // → param[7] lastSliceSize（最后一个 rank 的 slice 大小，非均分时不同）
    xnData_[8] = mySliceSize;                  // → param[8] mySliceSize（本 rank 实际 slice 大小）
    xnData_[9] = sliceOffset;                  // → param[9] sliceOffset（slice 在 buffer 中的偏移）
    xnData_[10] = isInputOutputEqual;          // → param[10] isInputOutputEqual（输入输出地址是否相同）
    CalcGoSize(..., &xnData_[11]);             // → param[11-14] goSize[0-3]（分块参数，⑤已算，⑥需传，⑦需读）
}
```

> **索引约束**：`xnData_[N]` 写入索引必须与 ⑥ `DispatchKfcSubKernel` 的 `param[HBM_PARAM_IDX_N]` 读取索引完全一致。⑤ 和 ⑥ 必须同步开发。

**ReduceScatter 参照**：`hccl_ccu_v0_prepare.h:222-245` ✅

---

### ⑥ KFC Dispatch（修改现有）

**路径**：`impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc`

框架入口：CCU 侧 `CcuKfcServerKernel` 从 HBM 读 32 参数后调 `DispatchKfcSubKernel(ctx, param)`，函数内按 opType if-else 分发到各算子 kernel。

hccl 由 template `KernelRun` 直接调 `HcommCcuKernelLaunch` 下发算子 kernel，无统一分发层。asc-devkit 需在 `DispatchKfcSubKernel` 中**新增** opType 分支：

- `param[]` 索引与 ⑤ `xnData_[]` 一致
- `channels`/`channelCount`/`rankSize`/`rankId`/`dataType`/`outputType`/`reduceType` 从 `ctx.arg` 获取
- `goSize` ⑤已算写入 `xnData_[9-12]`，⑥需传 `param[9-12]`，⑦需读（参照 hccl `LoadArg`）；`isInputOutputEqual` 不传

```cpp
if (ctx.arg->opParam.opType == HcclCMDType::HCCL_CMD_<OP>) {
    Ccu<Op>MeshMem2Mem1DKernel(
        param[HBM_PARAM_IDX_1],   // inputAddr      ← xnData_[1]（发送缓冲+偏移）
        param[HBM_PARAM_IDX_2],   // outputAddr     ← xnData_[2]（接收缓冲+偏移）
        ctx.token,                // token          ← kfcServerArgs（非HBM，来自资源分配阶段）
        param[HBM_PARAM_IDX_3],   // scratch        ← xnData_[3]（CCU MS 中间结果地址）
        param[HBM_PARAM_IDX_4],   // sliceOffset    ← xnData_[4]（本rank输入偏移）
        // ... 按 xnData_ 布局映射其余 param（索引与 ⑤ 一致）
        ctx.arg->channels,        // channels        ← CcuKernelArgKfcServer（资源分配时填的 channel 描述）
        ctx.arg->channelCount,    // channelCount    ← CcuKernelArgKfcServer
        static_cast<uint32_t>(ctx.arg->rankSize),  // rankSize ← CcuKernelArgKfcServer
        ctx.arg->rankId,          // rankId          ← CcuKernelArgKfcServer
        ctx.arg->opParam.DataDes.dataType,   // dataType    ← CcuKernelArgKfcServer.opParam
        ctx.arg->opParam.DataDes.outputType, // outputType  ← CcuKernelArgKfcServer.opParam
        ctx.arg->opParam.reduceType);        // reduceType  ← CcuKernelArgKfcServer.opParam
    return;
}
```

**ReduceScatter 参照**：`ccu_kernel_kfc_server.cc:249-255` ✅

---

### ⑦ Kernel `.h` + `.cc`

**路径**：`impl/adv_api/detail/hccl/cc/src/ops/<op>/template/ccu/kernel/ccu_kernel_<op>_mesh1d_mem2mem.h` + `.cc`
**hccl 参照**：`hccl/src/ops/all_reduce/template/ccu/kernel/ccu_kernel_all_reduce_mesh1d_mem2mem.h` + `.cc`（665 行）

框架入口：⑥ `DispatchKfcSubKernel` 调 `Ccu<Op>Kernel(显式参数)`。

**`.h`**：
hccl kernel 接收 `CcuKernelArg` 单结构体指针（内含 rankSize/rankId/opParam/subCommRanks）。asc-devkit ⑥ `DispatchKfcSubKernel` 按 opType 分发，不传 `CcuKernelArg`，改为显式参数列表。因此：
- **复用** hccl Context 结构体字段（input/output/token/scratch/events）
- **删除** hccl 的 `CcuKernelArgBase` 子类
- **改为** kernel 签名 `Kernel(CcuKernelArg arg)` → `Kernel(显式参数列表)`，与 ⑥ 调用匹配

```cpp
// hccl 签名（直接下发，CcuKernelArg 单参数）
CcuResult CcuAllReduceMeshMem2Mem1DKernel(CcuKernelArg arg);
// CcuKernelArg 内含：rankSize, rankId, opParam(dataType/outputType/reduceType), subCommRanks, channels, channelCount

// asc-devkit 签名（KFC 分发，显式参数）
CcuResult Ccu<Op>MeshMem2Mem1DKernel(
    ccu::Variable inputAddr,               // ← param[1] 发送缓冲+偏移
    ccu::Variable outputAddr,              // ← param[2] 接收缓冲+偏移
    ccu::Variable token,                   // ← ctx.token 通知令牌
    ccu::Variable scratch,                 // ← param[3] CCU MS 中间结果地址
    ccu::Variable sliceOffset,             // ← param[4] 本rank输入偏移
    ccu::Variable normalSliceSize,         // ← param[6] 均分时每rank的slice大小
    ccu::Variable lastSliceSize,           // ← param[7] 最后一个rank的slice大小
    ccu::Variable mySliceSize,             // ← param[8] 本rank实际slice大小
    // ... 其余按 op 逻辑增减，索引与 ⑤ xnData_ / ⑥ param 一致
    const ChannelHandle channels[],        // ← ctx.arg->channels channel描述数组
    uint32_t channelCount,                 // ← ctx.arg->channelCount channel数量
    uint32_t rankSize,                     // ← ctx.arg->rankSize 通信域rank数
    uint32_t rankId,                       // ← ctx.arg->rankId 本rank ID
    const HcclDataType &dataType,          // ← ctx.arg->opParam.DataDes.dataType 输入数据类型
    const HcclDataType &outputType,        // ← ctx.arg->opParam.DataDes.outputType 输出数据类型
    const HcclReduceOp &reduceType);       // ← ctx.arg->opParam.reduceType 规约操作类型
```

**`.cc`**：
hccl 的 `LoadArgs` 用 `ccu::LoadArg(ctx.xxx, argId++)` 逐个从 taskArgs 加载。asc-devkit 无 taskArgs，参数由 ⑥ 显式传入。因此：
- **复用**（直接移植）：`InitResource`（按 channel 填充变量）、`PreSync`（变量交换）、`PostSync`（后同步）、`BcastLocToRmt`（Write 本地到远端）
- **改为** `ParseKernelArg`/`LoadArgs`：从显式参数直接赋值到 ctx
- **改为** `ReduceLoopGroupV1/V2`：`ccu::Func`+`ccu::Loop`+`ccu::LoopGroup` 硬件循环 → 软件 `for` 循环（参照 ReduceScatter，KFC 避开 LoopGroup 的 block 资源开销）
- **复用** `PairwiseLocalReduce`（rankSize>8 时 log2(n) 两两归约），直接移植

KFC 约束适配（hccl 无此约束）：
- **goSize**：hccl 经 taskArgs 传入 goSize[4]；asc-devkit ⑤ 已调 `CalcGoSize` 写入 `xnData_[9-12]`，但 ⑥ 未传、⑦ 未读。需 ⑥ 传 `param[9-12]`，⑦ 读（参照 hccl `LoadArg`）
- **EventWait**：hccl 用 `BIT_NUM_PER_CKE=16` 分组 + events 数组 → **必须移植**，`events.resize((rankSize+15)/16)`，按 `eventIdx = rankIdx / 16` 分组
- **lastSliceSize**：hccl taskArgs 传 normal 和 last → 确保 ⑤ 正确计算写入 `xnData_`，⑥ 传 `param[6]`/`param[7]`

**ReduceScatter 参照**：`ccu_kernel_reduce_scatter_mesh1d_mem2mem.h` + `.cc`（263 行）✅（有已知问题，见第 4 节）

---

### CMakeLists.txt

将 ①②③⑦ 新建的 `.h`/`.cc` 加入对应 CMakeLists.txt 编译目标，参照 ReduceScatter 目录结构。

---

## 3. ReduceScatter 迁移案例

| 接入点 | 文件 | 状态 | 说明 |
|:---:|------|:---:|------|
| ① | `reduce_scatter_auto_selector.h` + `.cc` | ✅ | 校验 + 硬编码算法名 + `REGISTER_SELECTOR_BY_OPTYPE` |
| ② | `ins_v2_reduce_scatter_sole_executor.cc:238` | ✅ | `REGISTER_EXEC_V2` |
| ③ | `ccu_temp_reduce_scatter_mesh_1D_mem2mem.h` + `.cc` | ✅ | CalcRes 填 channels；KernelRun/FastLaunch 空壳 |
| ④ | `hccl_ccu_v0.h:277-279` | ✅ | `CcuPrepareForOp` 分支 |
| ⑤ | `hccl_ccu_v0_prepare.h:222-245` | ✅ | `CcuPrepareForReduceScatterM2M` |
| ⑥ | `ccu_kernel_kfc_server.cc:249-255` | ✅ | `DispatchKfcSubKernel` 分支 |
| ⑦ | `ccu_kernel_reduce_scatter_mesh1d_mem2mem.h` + `.cc` | ✅ | 13 参数 kernel + 263 行实现 |

---

## 4. 算子升级（hccl 基线演进差异）

asc-devkit 已迁移算子（ReduceScatter）基于 hccl 较早基线完成（263 行），hccl 此后持续演进到 761 行。新算子迁移时应参照 hccl **当前版本**，而非复制 asc-devkit 已迁移算子的简化实现。

### 4.1 特性索引

| # | 特性 | 提交 | 影响定性 | 涉及接入点 |
|:---:|------|----------|---------|-----------|
| 1 | 3 阶段流水线 + RS_UNROLL_NUM + events 数组扩展 + scratchRepeatStride | `72b350cb`/`b6d6bdb4` | 性能+**正确性** | **⑤⑥⑦** |
| 2 | PairwiseLocalReduce（rankSize>8） | `b6d6bdb4` | 性能 | — |
| 3 | goSize 分块 | 早期基线 | **正确性** | **⑤⑥⑦** |
| 4 | lastSliceSize 正确传递 | 早期基线 | **正确性**（边界） | **⑤⑥⑦** |
| 5 | groupcopy bugfix（`LoopGroup` 第 3 参数） | `6ee28963` | **正确性** | — |

> "涉及接入点"仅标注需修改框架对接代码（参数传递链路 ⑤→⑥→⑦）的项；纯 ⑦ kernel 内部实现更新标"—"。
> 以下各项的 hccl 参照文件：`hccl/src/ops/reduce_scatter/template/ccu/kernel/ccu_kernel_reduce_scatter_mesh1d_mem2mem.cc`。

### 4.2 三阶段流水线 + RS_UNROLL_NUM + events 数组 + scratchRepeatStride（⑤⑥⑦）

**动机**：原串行 `DoReduceScatter` 中 Read 阻塞，Wait 期间 CCU 空闲。改为 16 轮批量 ReadNb（非阻塞，event 错开）→ 批量 WaitEvent → 串行 Reduce，使 Read 和 Reduce 重叠（`72b350cb`，描述："跨框中小数据量性能优化"）。这 4 项是同一提交的配套改动：3 阶段流水线是框架，RS_UNROLL_NUM 是批量轮数，events 数组为每轮提供独立 event 组，scratchRepeatStride 是多轮 Read 时 scratch 地址步进量。

**hccl 实现**（行 81、137-340）：
- `DoReduceScatter` 拆为 `DoReduceScatterRead`/`DoReduceScatterWait`/`DoReduceScatterReduce`
- `events.resize(RS_UNROLL_NUM * eventNum)`，`eventNum = (rankSize + 15) / 16`
- `LoadArg(ctx.scratchRepeatStride, cnt++)`，`scratchRepeatStride = rankSize * memSlice`

**改动**：

⑦ `ccu_kernel_<op>_mesh1d_mem2mem.h` + `.cc` — kernel 内部：
- **新增** `DoReduceScatterRead`/`DoReduceScatterWait`/`DoReduceScatterReduce` + `InitReduceScatterAddr`/`ResetReduceScatterAddr`
- **改为** Context 字段：`ccu::Event event` → `std::vector<ccu::Event> events`；**新增** `constVar1`/`readRepeatNum`/`waitRepeatNum`/`flag`
- **改为** `InitResource`：`events.resize(RS_UNROLL_NUM * eventNum)`
- **改为** Read/Wait 中 event 索引：`eventIdx = unrollIdx * numEventsPerIter + rankIdx / 16`，`rankMask = 1 << (rankIdx % 16)`
- **改为** `DoRepeatReduceScatter` 三阶段编排：

```cpp
static CcuResult DoRepeatReduceScatter(Context &ctx) {
    InitReduceScatterAddr(ctx);
    // Phase 1: 批量 ReadNb（16轮，非阻塞）
    for (uint32_t i = 0; i < RS_UNROLL_NUM; i++) {
        CCU_IF(ctx.repeatNum != UINT64_MAX) {
            ctx.repeatNum += ctx.constVar1;  // constVar1=1
            if (i > 0) { /* 步进 scratch/input，用 scratchRepeatStride */ }
            DoReduceScatterRead(ctx, i);     // i=unrollIdx，用于 event 组索引
        }
    }
    // Phase 2: 批量 WaitEvent
    for (uint32_t i = 0; i < RS_UNROLL_NUM; i++) {
        CCU_IF(ctx.waitRepeatNum != UINT64_MAX) { ctx.waitRepeatNum += ctx.constVar1; DoReduceScatterWait(ctx, i); }
    }
    // Phase 3: 串行 Reduce
    ResetReduceScatterAddr(ctx);
    CCU_CHK_RET(DoReduceScatterReduce(ctx));
    return CCU_SUCCESS;
}
```

⑤ `hccl_ccu_v0_prepare.h` — 新增 `scratchRepeatStride` 写入：
```cpp
uint64_t scratchRepeatStride = rankSize * CCU_MEMSLICE_SIZE; // rankSize(通信域rank数) * CCU_MEMSLICE_SIZE(单次MS传输大小)
xnData_[<idx>] = scratchRepeatStride;  // → param[<idx>]
```

⑥ `ccu_kernel_kfc_server.cc` — 新增传参：`param[HBM_PARAM_IDX_<idx>]` // scratchRepeatStride

⑦ `ccu_kernel_<op>_mesh1d_mem2mem.h` — 签名新增：`ccu::Variable scratchRepeatStride` // ← param[<idx>]，每轮 repeat 后 scratch 地址步进量

### 4.3 PairwiseLocalReduce（⑦）

**动机**：rankSize>8 时逐个 LocalReduce 指令数 O(n)，改为每轮两两归约 O(log n)。同时 rankSize>8 时需先将本 rank 数据 LocalCopy 到 scratch 使所有 rank 数据连续（`b6d6bdb4`，性能优化）。

**hccl 实现**（行 192-260）：
```cpp
static CcuResult PairwiseLocalReduce(Context &ctx, ccu::LocalAddr myOutput,
    std::vector<ccu::LocalAddr> &scratchMem, ccu::Variable sliceSize, ...) {
    uint32_t remainPieces = ctx.arg->rankSize;  // rankSize(通信域rank数)
    while (remainPieces > 1) { /* 每轮两两归约 */ remainPieces = (remainPieces + 1) / 2; }
}
```

**改动**（⑦ `.cc`）：
- **新增** `PairwiseLocalReduce` 函数，从 hccl 移植（~70 行）
- **改为** `DoReduceScatter` 中 rankSize 分支：`rankSize <= 8` 用 `ReduceLoopGroup`（可选），`>8` 用 `PairwiseLocalReduce`
- **改为** `DoReduceScatterRead` 中 `rankSize > 8` 时本 rank 用 `LocalCopy` 到 scratch（而非 `EventRecord`）

### 4.4 goSize 分块（⑤⑥⑦）

**动机**：大数据量超 CCU MS 单次操作容量需分块。goSize[4] 控制循环地址偏移/循环次数/并行循环次数/尾块残差。asc-devkit ⑤ 已算但 ⑥ 不传、⑦ 不读。

**hccl**：`KernelRun` 调 `CalGoSize` 算 4 值，经 taskArgs 参数[11-14] 传，`LoadArg` 加载。

**改动**：
⑤ `hccl_ccu_v0_prepare.h` — **已实现**，无需修改：
```cpp
CalcGoSize(sliceSize, loopCount, CCU_MEMSLICE_SIZE, &xnData_[9]);
// xnData_[9]=goSize[0] addrOffset(循环地址偏移)  xnData_[10]=goSize[1] loopParam(循环次数)
// xnData_[11]=goSize[2] parallelParam(并行循环次数)  xnData_[12]=goSize[3] residual(尾块残差大小)
```
⑥ `ccu_kernel_kfc_server.cc` — 新增传 4 个 param：`param[9]`/`param[10]`/`param[11]`/`param[12]`
⑦ `ccu_kernel_<op>_mesh1d_mem2mem.h` — 签名新增 4 参数；`.cc` `LoadArgs` 中赋值到 `ctx.goSize`：
```cpp
ctx.goSize.addrOffset = goSizeAddrOffset;       // ← param[9]
ctx.goSize.loopParam = goSizeLoopParam;          // ← param[10]
ctx.goSize.parallelParam = goSizeParallelParam;  // ← param[11]
ctx.goSize.residual = goSizeResidual;            // ← param[12]
```

**资源消耗**：goSize 分块依赖 LoopGroup 硬件循环引擎 + MS buffer 8 路交错流水线。`ReduceLoopGroup` → `CreateReduceLoop` → `AllocGoResource` 一次性申请 `loopCount * msInterleave = 16 * 8 = 128` 个 block MS buffer（4KB/个）+ 16 个 block CKE + 16 个 block Loop engine。这是旧版（手写 for 循环直接在 HBM 上操作，0 block 资源）到新版（LoopGroup MS 交错流水线）的主要资源代价。

CCU SCHED 资源池每个 IO Die 的 block MS 容量为 128（`CCU_SCHED_DEFAULT_BLOCK_MS_REQ`，`hcomm/.../ccu_dev_mgr_imp.cc:168`），单算子 ReduceScatter 恰好占满。多算子同时使用 LoopGroup 会超出池容量（详见第 5 节遗留问题 #5）。

### 4.5 lastSliceSize 正确传递（⑤⑥⑦）

**动机**：非均分场景（count 不被 rankSize 整除）最后一个 rank 的 slice 大小不同，需区分 normal/last。asc-devkit 当前 `lastSliceSize = sliceSize`（均分假设）。

**hccl**：`KernelRun` 从 `CalcSlice` 取 `sliceInfoVec[rankSize-1][0].size`，经 taskArgs 参数[7] 传，`LoadArg` 加载。

**改动**：
⑤ `hccl_ccu_v0_prepare.h` — 修正计算：
```cpp
uint64_t totalSize = commParam->count * dataSize;  // count(总元素数) * dataSize(单元素大小)
uint64_t normalSliceSize = totalSize / rankSize;    // rankSize(通信域rank数)
uint64_t lastSliceSize = totalSize - normalSliceSize * (rankSize - 1); // 最后一个rank的实际大小
xnData_[6] = normalSliceSize;  // → param[6]
xnData_[7] = lastSliceSize;    // → param[7]
```
⑥ `ccu_kernel_kfc_server.cc` — **已传** `param[7]`，无需修改。
⑦ `ccu_kernel_<op>_mesh1d_mem2mem.cc` — `LoadArgs` 中 `ctx.lastSliceSize = lastSliceSize`，kernel 内按 `rankId == rankSize-1` 时用 `lastSliceSize`。

### 4.6 groupcopy bugfix（⑦，仅选 LoopGroup 时需要）

**动机**：`LoopGroup` 第 3 参数传硬编码 `1`/`loopNum=2` 而非 `ctx.moConfig.loopCount`，导致循环次数错误（`6ee28963`）。

**hccl 修复**（`ccu_kernel_alg_base.cc`，6 处）：
```cpp
// 修复前（asc-devkit 当前）→ 修复后（hccl 当前）：
ccu::LoopGroup group(paraCfg, offsetCfg, 1, grpLoops);                    → ..., ctx.moConfig.loopCount, ...);
ccu::LoopGroup group(goSize.parallelParam, offsetCfg, 2, grpLoops);       → ..., ctx.moConfig.loopCount, ...);
```

**改动**（⑦ `ccu_kernel_alg_base.cc`，6 处：`GroupReduce`/`GroupBroadcast`/`GroupBroadcastWithoutMyRank`/`GroupReduceWithoutMyRank` 各 2 处）：第 3 参数改为 `ctx.moConfig.loopCount`。

> asc-devkit ReduceScatter kernel 未用 LoopGroup 不受影响。新算子如选软件循环替代 LoopGroup 则无需此修复；如选 LoopGroup 则必须同步。

### 4.7 参数差异

hccl 当前 `LoadArgs` 加载 **16 个参数**，asc-devkit 已迁移代码只传 **7 个 HBM param + 6 个 ctx 字段 = 13 个**：

| 参数 | hccl | asc-devkit | 涉及接入点 | 差异 |
|------|------|-----------|-----------|------|
| `scratchRepeatStride` | `LoadArg`（参数[10]） | 硬编码 0 | **⑤⑥⑦** | 缺失，需新增 `xnData_` 索引经 ⑤→⑥ 传入 ⑦ |
| `goSize[0]` addrOffset | `LoadArg`（参数[11]） | ⑤已算写 `xnData_[9]`，⑥不传，⑦不读 | **⑤⑥⑦** | ⑤ 已算，⑥ 需传 `param[9]`，⑦ 需读 |
| `goSize[1]` loopParam | `LoadArg`（参数[12]） | ⑤已算写 `xnData_[10]`，⑥不传，⑦不读 | **⑤⑥⑦** | 同上 |
| `goSize[2]` parallelParam | `LoadArg`（参数[13]） | ⑤已算写 `xnData_[11]`，⑥不传，⑦不读 | **⑤⑥⑦** | 同上 |
| `goSize[3]` residual | `LoadArg`（参数[14]） | ⑤已算写 `xnData_[12]`，⑥不传，⑦不读 | **⑤⑥⑦** | 同上 |

### 4.8 函数对比

| hccl 函数 | 行数 | asc-devkit 对应 | 行数 | 差异 |
|----------|:---:|----------------|:---:|------|
| `DoReduceScatterRead` | ~25 | 无（内联在 `DoReduceScatter`） | — | hccl 独立为批量 Read 阶段 |
| `DoReduceScatterWait` | ~15 | 无 | — | hccl 独立为批量 Wait 阶段 |
| `DoReduceScatterReduce` | ~30 | `DoReduceScatter` | 50 | hccl 用 LoopGroup，asc-devkit 软件循环 |
| `CreateReduceLoopV1` | ~55 | 无 | — | hccl 硬件循环创建 |
| `CreateReduceLoopV2` | ~55 | 无 | — | hccl V2 版硬件循环 |
| `ReduceLoopGroupV1` | ~130 | 无 | — | hccl V1 硬件循环执行 |
| `ReduceLoopGroupV2` | ~130 | 无 | — | hccl V2 硬件循环执行 |
| `PairwiseLocalReduce` | ~70 | 无 | — | hccl log2(n) 两两归约 |

### 4.9 升级建议

- **必须移植**（正确性）：3 阶段流水线项的 events 数组 + scratchRepeatStride、goSize 分块、lastSliceSize
- **应移植**（性能）：3 阶段流水线、PairwiseLocalReduce
- **可选移植**：LoopGroup V1/V2（KFC 默认用软件循环替代；如需用 LoopGroup，必须先同步 groupcopy bugfix）

---

## 5. 遗留问题（ReduceScatter）

以下为 ReduceScatter 迁移过程中引入的工程问题：

### 5.1 工程问题

| # | 问题 | 位置 |
|---|------|------|
| 1 | `hccl_alloc_ctx_res.h:22-27` git merge conflict 未解决（阻塞编译） | `impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.h` |
| 2 | selector dead code 引用已删算法名 | `reduce_scatter_auto_selector.cc:54-389` |
| 3 | `.bak` 文件入库 | `tests/.../hccl_op_common_stub.cc.bak` |
| 4 | test 未覆盖 kernel 实际执行，只测资源分配 | `tests/.../test_ccu_mc2.cpp` |

### 5.2 KFC Server 打包注册导致 block 资源翻倍

**问题**：KFC Server 把所有算子 dispatch 逻辑打包在一个 kernel 里注册。`CCU_IF`/`CCU_ELSE` 宏注册时两个分支都执行（C++ for 循环），子 kernel 被调 2 次（主+从分支各一次），`AllocGoResource` 无去重保护，block MS/CKE 累加翻倍：

```
注册时 CcuKfcServerKernel 执行:
  CCU_IF(missionIndex==0): DispatchKfcSubKernel
    → RS: AllocGoResource → +128 block MS    ← 第1次
    → AG: GroupCopy → AllocGoResource → +64 block MS
  CCU_ELSE: DispatchKfcSubKernel
    → RS: AllocGoResource → +128 block MS    ← 第2次（翻倍）
    → AG: GroupCopy → AllocGoResource → +64 block MS ← 第2次（翻倍）
  合计: 384 block MS，远超 resPack 128
```

`EnsureLoopEnginePool` 有去重保护（pool 已满则跳过），block Loop 不翻倍。但 `ccu::Array<CcuBuffer>` 构造无去重，每次都 `BlockAlloc` 累加到 `CcuKernel::res_`。

**当前临时方案**：删掉 `CCU_ELSE` 从分支（`ccu_kernel_kfc_server.cc`），注册时只走主分支。同时注释掉 AllGather（AG 的 `GroupCopy` 也调 `AllocGoResource` +64 block MS，128+64=192 仍超 128）。

| 配置 | block MS | resPack(128) | 结果 |
|------|:---:|:---:|------|
| 原始（主+从，RS+AG） | 384 | 128 | 不够 |
| 删 CCU_ELSE（主，RS+AG） | 192 | 128 | 不够 |
| 删 CCU_ELSE + 注释 AG（主，仅 RS） | 128 | 128 | 恰好 fit |

**彻底解决方案**（需 KFC Server 框架层面改动）：

| 方案 | 改动 | 主从共享 | missionNum>1 | 需改框架 |
|------|------|:---:|:---:|:---:|
| **A. KFC Server 层预分配** | `CcuKfcServerKernel` 入口调一次 `AllocGoResource`，子 kernel 共享 `moRes` | 是 | 是 | 是 |
| **B. 扩大 SCHED 池** | `CCU_SCHED_DEFAULT_BLOCK_MS_REQ` 128→256+ | 否 | 是 | 否（改 hcomm） |
| **C. 从分支不展开** | 当前临时方案（删 CCU_ELSE） | 否 | 否 | 是 |

方案 A 最彻底；方案 B 最简单但治标不治本（未来更多算子会再次翻倍）；方案 C 是当前临时方案，`missionNum>1` 时从分支无指令序列。

---

## 6. 迁移检查清单

- [ ] ① Selector — 复用 hccl `SelectCcuMsAlgo` 校验 + 删除其他路径 + `REGISTER_SELECTOR_BY_OPTYPE` priority=18
- [ ] ② Executor — `REGISTER_EXEC_V2`（算法名与 ① 一致）
- [ ] ③ Template — CalcRes 只填 channels + KernelRun/FastLaunch 空壳
- [ ] ④ Device 分发 — `CcuPrepareForOp` 新增 opType 分支
- [ ] ⑤ Device 参数 — `CcuPrepareFor<Op>M2M` 写 `xnData_`（索引与 ⑥ 一致）
- [ ] ⑥ KFC Dispatch — `DispatchKfcSubKernel` 新增 opType 分支（param 索引与 ⑤ 一致，kernel 参数与 ⑦ 一致）
- [ ] ⑦ Kernel — Context + 显式参数签名 + 算法移植（参照 hccl 当前版本，含 3 阶段流水线 + events 分组 + goSize[4]读取 + scratchRepeatStride + lastSliceSize + PairwiseLocalReduce）
- [ ] CMakeLists.txt — 新文件加入编译
- [ ] 编译验证：`bash build.sh -t -j8`
- [ ] 测试：端到端数据正确性验证
