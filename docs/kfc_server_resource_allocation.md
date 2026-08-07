# MC2 KFC Server 硬件资源申请过程

## 1. 总览

MC2 算子通过 KFC Server 框架调用 CCU 通信算法。整个硬件资源申请分为三个阶段：

```
① 硬件资源预分配（hcomm 初始化时，一次性）
   硬件 caps 寄存器 → CcuResBatchAllocator 预分配全部 block 资源 → CcuResPack 按 CCU_SCHED 模板领取资源池

② KFC Server kernel 注册（MC2 资源分配时，一次性）
   HcommCcuKernelRegister → 执行 CcuKfcServerKernel 函数体构建指令序列 → 子 kernel 中 AllocGoResource 等创建 CCU 资源对象 → 资源需求累加到 CcuKernel::res_

③ 资源检查与搬移（注册函数体执行完后）
   GetResourceRequest 汇总 res_ → CheckResIfAvailable 对比 resPack 持有量 → 够则 LoadRes 搬移，不够则 CCU_E_UNAVAIL
```

---

## 2. 阶段一：硬件资源预分配

### 2.1 硬件能力查询

`hcomm/.../ccu_res_specs.cc:99-155`

通过 `HccpRaTlvCcuCustomChannel(devLogicId, CCU_U_OP_GET_BASIC_INFO)` 查询硬件 caps 寄存器，解析出每个 IO Die 的硬件资源上限：

| 资源 | caps 字段 | bit 位 | 含义 |
|------|---------|:---:|------|
| `msNum` | `cap2 >> 16` | 16 | MS buffer 总数 |
| `ckeNum` | `cap2 & 0xFFFF` | 16 | CKE 总数 |
| `loopEngineNum` | `cap0 >> 24`（V1）/ `cap0 >> 20`（V2） | 8/12 | Loop engine 总数 |
| `xnNum` | `cap1 >> 16` | 16 | XN 寄存器总数 |
| `missionNum` | `cap0 >> 16` | 8/4 | Mission 总数 |

`CCU_MAX_IODIE_NUM = 2`（`hcomm/.../ccu_common.h:18`），每个 Die 独立查询。

### 2.2 Block 资源策略

`hcomm/.../ccu_res_specs.h:64-69`

```cpp
struct CcuBlockResStrategy {
    uint32_t loopNum{16};      // Loop engine block 粒度
    uint32_t ckeNum{16};       // CKE block 粒度
    uint32_t msNum{128};       // MS buffer block 粒度
    uint32_t missionNum{2};
};
```

block 资源以固定粒度分配：MS 每块 128 个，CKE 每块 16 个，Loop 每块 16 个。

### 2.3 预分配全部 block 资源

`hcomm/.../ccu_res_batch_allocator.cc:124-183` — `PreAllocBlockRes`

对每个 enabled Die：
1. `GetPreAllocatedMaxBlockNum`（行 98-122）：`blockNum.msNum = hardwareMsNum / 128`，`blockNum.ckeNum = min(max(loop,ms), ckeNum/16)`
2. `ccuComponent.AllocRes(dieId, resType, reqNum, true, ...)`（行 155）：从硬件驱动分配全部资源到 batch allocator
3. 按 blockSize 切分为 `BlockInfo` 条目存入 `resBlocks_[dieId]`（行 167-177）

### 2.4 CCU_SCHED 资源池模板

`hcomm/.../ccu_dev_mgr_imp.cc:167-193` — `ConfigCcuResReqCcuSched`

```cpp
constexpr u32 CCU_SCHED_DEFAULT_BLOCK_LOOP_ENGINE_REQ = 16;   // 1 个 Loop block
constexpr u32 CCU_SCHED_DEFAULT_BLOCK_MS_REQ          = 128;  // 1 个 MS block
constexpr u32 CCU_SCHED_DEFAULT_CKE_REQ               = 32;   // 32 个 regular CKE
constexpr u32 CCU_SCHED_DEFAULT_BLOCK_CKE_REQ         = 16;   // 1 个 CKE block
constexpr u32 CCU_SCHED_DEFAULT_CONTINUOUS_XN_REQ    = 400;  // 400 个 XN
constexpr u32 CCU_SCHED_DEFAULT_GSA_REQ              = 400;
constexpr u32 CCU_SCHED_DEFAULT_MISSIONREQ_REQ       = 2;
```

`CcuAllocResHandleByInsType`（行 196-253）按 enabled Die 逐个配置 `CcuResReq`，调 `CcuDevMgrImp::AllocResHandle`。

### 2.5 CcuResPack 领取资源池

`hcomm/.../ccu_instance.cc:44-65` — `CcuInstance::Init`：创建 `CcuResPack`，调 `resPack_->Init()`。

`hcomm/.../ccu_res_pack.cc:32-52` — `CcuResPack::Init`：调 `CcuAllocResHandleByInsType` → `CcuResBatchAllocator::AllocResHandle` → `HandleBlockRes`。

`HandleBlockRes`（`ccu_res_batch_allocator.cc:268-306`）：对 MS，`blockNum = 1 + (128-1)/128 = 1`，标记 1 个 block 已分配，产生 1 个 `ResInfo{startId, num=128}` 存入 `resRepoPtr->blockMs[dieId]`。

**resPack 持有的资源（每个 enabled Die）**：

| 资源类型 | 数量 | 来源 |
|---------|:---:|------|
| block MS | 128 | 1 个 ResInfo{num=128} |
| block CKE | 16 | 1 个 ResInfo{num=16} |
| block Loop engine | 16 | 1 个 ResInfo{num=16} |
| regular CKE | 32 | 32 个单 CKE |
| continuous XN | 400 | 400 个单 XN |
| mission | 2 | 2 个 mission |

---

## 3. 阶段二：KFC Server kernel 注册

### 3.1 注册入口

`asc-devkit/.../op_common.cc:1439-1502`

```
HcclGetCcuKernel → RegisterCcuKernels → RegisterCcuKernelGroup → RegisterSingleCcuKernel
  → HcommCcuKernelRegister(insHandle, dieId, "CcuKfcServer", CcuKfcServerKernel, ...)
```

### 3.2 Register 执行 kernel 函数体

`hcomm/.../ccu_kernel_mgr.cc:105-159` — `CcuKernelMgr::Register`

1. 创建 `currKernel_ = std::make_unique<CcuKernel>()`（行 123）——**整个 KFC Server 注册只有一个 CcuKernel 对象**
2. 执行 `ccuKernelFunc(ccuKernelArg)`（行 140）——调 `CcuKfcServerKernel(arg)`
3. **函数体执行过程中**，所有 `ccu::Event`/`ccu::CcuBuffer`/`ccu::Array` 的构造都通过 `CcuKernelMgr::GetCurrentKernel()` 获取同一个 `currKernel_`，把资源需求累加到 `currKernel_->res_`
4. `FlushClosablePendingIfs()`（行 143）关闭未闭合的 CCU_IF
5. `AllocRes(resPack)`（行 147）检查资源

### 3.3 CcuKfcServerKernel 函数体执行

`asc-devkit/.../ccu_kernel_kfc_server.cc:231-249`

```cpp
CcuResult CcuKfcServerKernel(CcuKernelArg arg) {
    KfcServerContext ctx;
    ParseKernelArg(ctx, kernelArg);
    InitResource(ctx);
    LoadArgs(ctx);
    CompArgs(ctx);
    CCU_IF(ctx.missionIndex == 0) {
        KfcServerGenCircularQueue(ctx);       // 主分支
    }
    CCU_ELSE {
        SlaveKfcServerGenCircularQueue(ctx);  // 从分支
    }
}
```

### 3.4 CCU_IF/CCU_ELSE 注册时两个分支都执行

`hcomm/include/ccu/ccu_control_flow_macro.h:54-91`

`CCU_IF` 展开为 C++ `for` 循环，`CcuIfBegin` 总是返回 `CCU_SUCCESS`，循环体**总是执行一次**。`CCU_ELSE` 同理。

`CcuKernel::IfBegin`（`ccu_kernel.cc:1114-1158`）：生成条件跳转指令追加到 rep 队列，但**不阻止 C++ 代码继续执行 if 体**。

**关键结论**：注册时 `CCU_IF(ctx.missionIndex == 0)` 和 `CCU_ELSE` 的函数体**都会执行**。`ctx.missionIndex` 是运行时值（从 HBM 参数加载），不影响注册时的 C++ 执行流。

### 3.5 DispatchKfcSubKernel 中的子 kernel 调用

`asc-devkit/.../ccu_kernel_kfc_server.cc:120-141`

```cpp
static void DispatchKfcSubKernel(ccu::Array<ccu::Variable>& param, KfcServerContext& ctx) {
    ccu::Variable opType = ctx.arg->opParam.opType;
    CCU_IF(opType == HCCL_CMD_ALLGATHER) {
        CcuKfcAllGatherMesh1DMem2MemKernel(...);     // AllGather 子 kernel
    }
    CCU_IF(opType == HCCL_CMD_REDUCE_SCATTER) {
        CcuReduceScatterMesh1DMem2MemKernel(...);     // ReduceScatter 子 kernel
    }
}
```

这是**两个独立的 `CCU_IF`**（不是 if/else），注册时**两个子 kernel 都执行**。

### 3.6 注册时的完整执行路径

```
CcuKfcServerKernel (一次调用)
  ├─ CCU_IF(missionIndex==0): KfcServerGenCircularQueue     ← 主分支执行
  │   └─ CCU_WHILE → CCU_IF(param[0]!=INVALID):
  │       └─ DispatchKfcSubKernel:
  │           ├─ CCU_IF(ALLGATHER): CcuKfcAllGatherKernel      ← 第1次执行 AG
  │           └─ CCU_IF(REDUCE_SCATTER): CcuReduceScatterKernel ← 第1次执行 RS
  │
  └─ CCU_ELSE: SlaveKfcServerGenCircularQueue                ← 从分支执行
      └─ CCU_WHILE → CCU_IF(param[0]!=INVALID):
          └─ DispatchKfcSubKernel:
              ├─ CCU_IF(ALLGATHER): CcuKfcAllGatherKernel      ← 第2次执行 AG
              └─ CCU_IF(REDUCE_SCATTER): CcuReduceScatterKernel← 第2次执行 RS
```

**子 kernel 执行次数**：
- AllGather：**2 次**（主+从，无 static 保护）
- ReduceScatter：第 1 次正常执行，第 2 次 `s_reduceScatterKernelExecuted=true` 跳过 → **1 次**

---

## 4. 子 kernel 中的资源创建

### 4.1 资源创建 API 对应关系

| C++ 对象 | 构造调用的 API | hcomm 方法 | 累加到 `res_` 字段 | resReq 字段 |
|---------|--------------|-----------|-----------------|------------|
| `ccu::Event()` | `CcuEventAlloc` | `CreateResAssist(res_.completedEvent)` | `completedEvent[dieId]` +1 | `ckeReq` +1 |
| `ccu::CcuBuffer()` | `CcuBufferAlloc` | `CreateResAssist(res_.ccubufs)` | `ccubufs[dieId]` +1 | `msReq` +1 |
| `ccu::Array<ccu::Event>(N)` | `CcuBlockEventAlloc(N)` | `CreateBlockResAssist(N, res_.blockCompletedEvent)` | `blockCompletedEvent[dieId]` +N | `blockCkeReq` +N |
| `ccu::Array<ccu::CcuBuffer>(N)` | `CcuBlockBufferAlloc(N)` | `CreateBlockResAssist(N, res_.blockCcubufs)` | `blockCcubufs[dieId]` +N | `blockMsReq` +N |

所有创建都通过 `CcuKernelMgr::GetCurrentKernel()` 获取同一个 `currKernel_`，累加到同一个 `res_`。

### 4.2 ReduceScatter 资源创建

`asc-devkit/.../ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc`

| 位置 | 代码 | 创建的资源 | 累加量 |
|------|------|---------|-------|
| `InitResource` 行 99-100 | `ctx.events.resize(RS_UNROLL_NUM * eventNum)` = 16 | 16 个 `ccu::Event` | `ckeReq` +16 |
| `CreateReduceLoop` 行 233 | `AllocGoResource(..., LOOP_NUM_16=16)` | | |
| → `AllocGoResource` 行 36 | `res.completedEvent = ccu::Array<ccu::Event>(16)` | 16 个 block CKE | `blockCkeReq` +16 |
| → `AllocGoResource` 行 39 | `res.ccuBuf = ccu::Array<ccu::CcuBuffer>(16*8=128)` | 128 个 block MS | `blockMsReq` +128 |

ReduceScatter 只执行 1 次（static 保护），总贡献：`ckeReq +16, blockCkeReq +16, blockMsReq +128`。

### 4.3 AllGather 资源创建

AllGather 无 static 保护，执行 2 次。

| 位置 | 代码 | 单次创建量 | 2 次累加 |
|------|------|---------|-------|
| `InitResource` | `ctx.events.resize(N)` | N 个 `ccu::Event` | `ckeReq` +2N |
| `GroupCopy` → `AllocGoResource(8, 8)` | `Array<Event>(8)` + `Array<CcuBuffer>(8*8=64)` | `blockCkeReq` +8, `blockMsReq` +64 | `blockCkeReq` +16, `blockMsReq` +128 |

### 4.4 资源累加汇总

| 步骤 | 来源 | blockMsReq | blockCkeReq | ckeReq |
|------|------|:---:|:---:|:---:|
| 1. 主分支 AG | AllGather 第 1 次 | +64 | +8 | +N |
| 2. 主分支 RS | ReduceScatter（static 通过） | +128 | +16 | +16 |
| 3. 从分支 AG | AllGather 第 2 次 | +64 | +8 | +N |
| 4. 从分支 RS | ReduceScatter（static 跳过） | +0 | +0 | +0 |
| **合计** | | **256** | **32** | **16+2N** |

---

## 5. 阶段三：资源检查与搬移

### 5.1 GetResourceRequest

`hcomm/.../ccu_kernel.cc:322-350`

```cpp
req.blockMsReq[dieId] = res_.blockCcubufs[dieId].size();         // = 256
req.blockCkeReq[dieId] = res_.blockCompletedEvent[dieId].size(); // = 32
req.ckeReq[dieId] = res_.completedEvent[dieId].size() + res_.localNotify[dieId].size();
```

### 5.2 GetResNumFromResPack

`hcomm/.../ccu_kernel_mgr.cc:187-208`

读取 resPack 当前持有的资源总量：

```cpp
totalRes.blockMsReq[i] += GetResTotalNum(tmpResRepository.blockMs[i]);  // = 128
totalRes.blockCkeReq[i] += GetResTotalNum(tmpResRepository.blockCke[i]); // = 16
```

### 5.3 CheckResIfAvailable

`hcomm/.../ccu_kernel_mgr.cc:215-253`

```cpp
needRes.blockMsReq = GetReqResNum(reqRes, totalRes);  // = GetReqResNum(256, 128) = 128
```

`GetReqResNum`（行 211-214）：`return (reqRes > totalRes) ? (reqRes - totalRes) : 0`

| 资源 | kernel 需求 | resPack 持有 | 差额 | 结果 |
|------|:---:|:---:|:---:|------|
| block MS | 256 | 128 | 128 | 不够 |
| block CKE | 32 | 16 | 16 | 不够 |
| regular CKE | 16+2N | 32 | 0（如果 16+2N <= 32） | 够 |
| block Loop | 16 | 16 | 0 | 够 |

`needRes.blockMsReq[1] = 128 != 0` -> 返回 false -> `CCU_E_UNAVAIL`（行 357-359）。

### 5.4 报错日志解读

```
ccu_kernel_mgr.cc:242: dieId[1] not enough, blockMsReq[128] blockCkeReq[16]
ccuKernel.cc:329: blockMs[256] ckeReq[18] blockCke[32]
```

- `dieId[1] not enough, blockMsReq[128]`：差额 = 256 - 128 = 128
- `blockMs[256]`：kernel 总需求 = 256（RS 128 + AG 128）
- `ckeReq[18]`：regular CKE 需求 = 16（RS events）+ 2（AG events，N=1 时）

---

## 6. 根因分析

### 6.1 为什么 128 变成 256

```
resPack 持有：128 block MS（SCHED 池容量）
kernel 需求：256 block MS
              +-- ReduceScatter: 128（1 次，static 保护）
              +-- AllGather:     128（2 次，无 static 保护）
                  +-- 主分支: 64
                  +-- 从分支: 64
```

### 6.2 根本原因

| 原因 | 说明 |
|------|------|
| `CCU_IF`/`CCU_ELSE` 注册时都执行 | 宏展开为 C++ for 循环，`CcuIfBegin` 总返回 CCU_SUCCESS，两个分支函数体都执行 |
| `DispatchKfcSubKernel` 中两个 `CCU_IF` 都执行 | AllGather 和 ReduceScatter 是独立 `CCU_IF`（非 if/else），注册时两个子 kernel 都执行 |
| AllGather 无 static 保护 | 每次调用都创建 `ccu::Array<ccu::CcuBuffer>(64)` -> 64 block MS，2 次共 128 |
| 所有资源累加到同一个 `currKernel_->res_` | `CcuKernelMgr::GetCurrentKernel()` 总返回同一个 KFC Server 的 `currKernel_` |

### 6.3 KFC Server 框架问题（非算法问题）

- hccl 中每个算子 kernel **独立注册**，只调一次 `AllocGoResource`，128 MS fit SCHED 池
- asc-devkit KFC Server 把所有算子 dispatch **打包在一个 kernel 里注册**，主从分支各调一次子 kernel，资源翻倍
- 这是 **KFC Server 框架的设计缺陷**，不应由子算法用 static hack 规避

### 6.4 当前 static hack 的局限性

| 场景 | static hack 是否有效 | 问题 |
|------|:---:|------|
| ReduceScatter | 有效 | RS 只执行 1 次 |
| AllGather | 无效 | AG 无 static 保护，执行 2 次 |
| `missionNum > 1`（未来多 Die） | 从分支无指令序列 | static 跳过后从分支不构建指令，运行时从分支找不到指令 |

---

## 7. 彻底解决方案

| 方案 | 改动 | 主从共享指令序列 | missionNum>1 可用 | 需改 KFC Server |
|------|------|:---:|:---:|:---:|
| **A. KFC Server 层预分配** | `CcuKfcServerKernel` 入口调一次 `AllocGoResource`，子 kernel 共享 `moRes` | 是 | 是 | 是 |
| **B. 所有子 kernel 加 static** | AllGather/AlltoAll 等也加 `s_xxxKernelExecuted` | 否 | 否 | 否 |
| **C. 扩大 SCHED 池** | `CCU_SCHED_DEFAULT_BLOCK_MS_REQ` 128->256 | 否（各分配各的） | 是 | 否（改 hcomm） |
| **D. 从分支不展开子 kernel 指令** | KFC Server 的 `CCU_ELSE` 不调 `DispatchKfcSubKernel` | 否 | 否 | 是 |

方案 A 最彻底但改动最大；方案 C 最简单但治标不治本（未来更多算子会再次翻倍）。需要和 KFC Server 团队讨论确定方案。
