# GetAlgResAICPU 资源申请分析

## 1. 功能概述

`GetAlgResAICPU` 的主要职责如下：

1. 根据算法生成的 `AlgResourceRequest` 申请 AICPU 执行资源。
2. 将申请结果填入 Host 侧 `AlgResourceCtxSerializable`。
3. 序列化资源上下文并复制到 Device context。
4. 返回 Device 侧资源上下文地址 `resCtxSequence` 和大小 `ctxSize`。
5. 在增量建链模式下复用已有资源或补充缺失 channel。

代码位置：

- `impl/adv_api/detail/hccl/cc/src/ops/op_common/op_common.cc:859`

整体调用关系：

```text
HcclGetAlgRes
  -> executor->CalcAlgHierarchyInfo
  -> executor->CalcRes
      -> InsTempAllReduceMesh1DTwoShot::CalcRes
      -> 生成 AlgResourceRequest
  -> GetAlgResAICPU
      -> HcclAllocAlgResourceAICPU
          -> 获取 CCL buffer
          -> HcclGetThread
          -> HcclGetChannel
      -> HcclMemcpyCtxHostToDevice
```

## 2. TwoShot 声明的资源需求

资源需求由标准 TwoShot template 的 `CalcRes` 生成，而不是由 `GetAlgResAICPU` 自行决定。

代码位置：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.cc`

对于 rank 数为 `N` 的通信域，资源请求如下：

| 请求项 | 数量 |
|---|---:|
| 从线程 | `N - 1` |
| 总执行线程 | `N` |
| 主线程 notify | `N - 1` |
| 每个从线程 notify | 1 |
| Mesh1D channel | 每个远端 rank 一条 |
| scratch multiple | 2 |

对应代码：

```cpp
resourceRequest.slaveThreadNum = templateRankSize_ - 1;
resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, 1);
resourceRequest.notifyNumOnMainThread = resourceRequest.slaveThreadNum;
```

Channel 请求通过以下接口生成：

```cpp
CalcChannelRequestMesh1D(...)
```

## 3. Host 侧资源上下文初始化

首次创建资源时，`GetAlgResAICPU` 先保存通信域、拓扑和算法分层信息：

```cpp
resCtxHost->commInfoPtr = static_cast<void*>(comm);
resCtxHost->topoInfo = *topoInfo;
resCtxHost->algHierarchyInfo = algHierarchyInfo;
```

这些信息用于 Device 侧：

- 恢复当前 user rank 和 rank size。
- 恢复 Mesh1D rank 列表。
- 构造 `InsTempAllReduceMesh1DTwoShot`。
- 通过 `GetAlgRank` 确定本 rank 对应的数据切片。
- 将 channel 按通信层和远端 rank 恢复为运行时 channel map。

这部分是执行元数据，不是独立硬件资源。

## 4. CCL Buffer

### 4.1 获取方式

`HcclAllocAlgResourceAICPU` 从通信域获取 HCCL buffer：

```cpp
HcclGetHcclBuffer(comm, &cclBufferAddr, &cclBufferSize);
```

并保存为：

```cpp
resCtxHost->cclMem = HcclMem{
    HCCL_MEM_TYPE_DEVICE,
    cclBufferAddr,
    cclBufferSize
};
```

这里通常是获取通信域已经管理的 CCL buffer，不是按照当前 AllReduce count 单独申请用户输入输出内存。

### 4.2 在 TwoShot 中的用途

CCL buffer 是标准 TwoShot 的中转和 scratch 空间。

ReduceScatter 阶段：

```text
远端 rank 0 的目标切片 -> CCL buffer 第 0 片
远端 rank 1 的目标切片 -> CCL buffer 第 1 片
...
远端 rank N-1 的目标切片 -> CCL buffer 第 N-1 片
```

数据收集完成后，第 1 到第 `N-1` 片按顺序归约到第 0 片。AllGather 阶段再从第 0 片发送本 rank 的归约结果。

### 4.3 scratch multiple 为 2 的作用

标准 TwoShot 的 `CalcScratchMultiple` 返回 2。executor 据此限制：

```text
maxDataSizePerLoop <= cclMem.size / 2
```

这样可以为非均匀切片和多 rank 接收布局保留足够空间。超过单轮容量的数据由 executor 分多个 loop 执行，不切换到 MeshChunk。

## 5. AICPU Thread

### 5.1 执行线程

`HcclGetThread` 按以下规则计算线程数量：

```cpp
threadNum = resRequest.slaveThreadNum + 1;
```

标准 TwoShot 最终申请 `N` 个 AICPU thread：

```cpp
HcclThreadAcquire(
    comm,
    COMM_ENGINE_AICPU_TS,
    threadNum,
    maxNotifyNum + 1,
    threads.data());
```

线程用途：

- `threads[0]` 是主线程，并执行本地顺序 Reduce。
- `threads[1..N-1]` 是从线程，用于执行各远端 rank 的收发操作。
- TwoShot 按 rank/切片索引使用对应线程，展开 `LocalCopy`、Send、Recv 和 SendRecv 操作。

ReduceScatter 使用的主要操作包括：

- `LocalCopy`
- `SendBatchWrite`
- `RecvWrite`
- `SendRecvBatchWrite`
- `LocalReduce`

AllGather 使用的主要操作包括：

- `LocalCopy`
- `SendRead`
- `RecvBatchRead`
- `SendRecvBatchRead`

### 5.2 展开线程

除算法执行线程外，还会申请一条 CPU 展开线程：

```cpp
HcclThreadAcquire(
    comm,
    COMM_ENGINE_CPU,
    1,
    0,
    &resCtxHost->unfoldThread);
```

展开线程用于 AICPU 算法展开和调度，与执行通信操作的 thread 列表分开。其 handle 保存在独立的 `unfoldAlgTag` context 中，供后续运行阶段恢复。

## 6. Notify

TwoShot template 声明：

```text
主线程 notify 数量 = N - 1
每个从线程 notify 数量 = 1
```

`HcclGetThread` 计算所有线程请求中的最大 notify 数：

```cpp
maxNotifyNum = max(
    notifyNumOnMainThread,
    notifyNumPerThread[i]);
```

实际申请 thread 时使用：

```text
maxNotifyNum + 1
```

额外的 1 个 notify 用于 Host/Device 或 AICore/AICPU 同步。

### 6.1 算法内部用途

ReduceScatter 和 AllGather 均使用以下同步流程：

```text
PreSync
  -> 主线程通知所有从线程开始

通信操作

PostSync
  -> 所有从线程通知主线程完成
```

对应接口：

```cpp
PreSyncInterThreads(...)
PostSyncInterThreads(...)
```

Notify 用于保证：

- 所有线程在正确时机进入通信阶段。
- 主线程等待全部远端收发完成后再执行本地 Reduce。
- AllGather 完成前不能结束当前 executor loop。

主线程 handle 和 notify 数量还会通过 `SaveMainThreadInfo` 保存到 CPU_TS context，供运行阶段恢复。

## 7. Mesh1D Channel

### 7.1 申请过程

`HcclGetChannel` 按通信层遍历 `resRequest.channels`：

```text
按 level 遍历 channel request
  -> 区分 DEVICE endpoint
  -> 区分 HOST endpoint
  -> HcclGetChannelImpl
      -> HcclChannelAcquire
```

标准 Mesh1D TwoShot 通常为每个远端 rank 申请一条 device channel，本 rank 不需要对自身建立 channel。

### 7.2 ChannelInfo 内容

每条 channel 保存：

- `remoteRank`
- 通信协议
- endpoint 类型
- channel handle
- channel notify 数量
- `remoteCclMem`
- 图模式下的 remote input/output buffer

远端 CCL buffer 通过以下接口获取：

```cpp
HcclChannelGetHcclBuffer(
    comm,
    channelHandle,
    &remoteCclBufferAddr,
    &remoteCclBufferSize);
```

并保存为：

```cpp
channel.remoteCclMem = HcclMem{
    HCCL_MEM_TYPE_DEVICE,
    remoteCclBufferAddr,
    remoteCclBufferSize
};
```

### 7.3 在 TwoShot 中的用途

ReduceScatter：

```text
本地 input -> 通过 channel 写入远端 CCL buffer
远端 input -> 通过 channel 传输到本地 CCL buffer
```

AllGather：

```text
本地归约结果 -> 通过 channel 传输给远端 rank
远端归约结果 -> 通过 channel 写入本地 output
```

TwoShot 通过以下方式取得每个远端 rank 的首条 channel：

```cpp
channels.at(remoteRank).at(0);
```

因此每个远端 rank 至少需要一条有效 channel。

## 8. 图模式 Buffer 注册

当：

```cpp
param.opMode == OpMode::OFFLOAD
```

Channel 创建前还会调用 `RegGraphModeBuffers` 注册 input/output buffer，并为远端获取：

```text
remoteInputGraphMode
remoteOutputGraphMode
```

这些资源用于图模式下访问远端用户输入输出内存。对于当前 MC2 AICPU opbase 路径，标准 TwoShot 主要使用 `remoteCclMem`；图模式相关资源只在 `OFFLOAD` 条件下申请。

## 9. Device 资源上下文

Host 侧资源申请完成后，资源保存在 `AlgResourceCtxSerializable` 中，主要字段包括：

```text
topoInfo
algHierarchyInfo
cclMem
threads
unfoldThread
notifyNumOnMainThread
notifyNumPerThread
channels
commInfoPtr
batch transfer capability
```

随后由 `HcclMemcpyCtxHostToDevice` 执行：

```cpp
std::vector<char> seq = resCtxHost->Serialize();

HcclEngineCtxCreate(
    comm,
    param.algTag,
    COMM_ENGINE_AICPU_TS,
    size,
    &ctx);

HcclEngineCtxCopy(
    comm,
    COMM_ENGINE_AICPU_TS,
    param.algTag,
    seq.data(),
    size,
    0);
```

这一步会创建 Device context，并将 Host 侧资源描述复制进去。AICPU server 在运行时从该 context 恢复 CCL buffer、thread、channel、notify 和拓扑信息，再调用 executor 的 `Orchestrate`。

日志中的：

```text
ctxSize[589]
resCtxSequence[0x...]
```

分别表示：

- `ctxSize`：序列化资源上下文大小。
- `resCtxSequence`：Device 侧资源上下文地址。

## 10. 缓存复用和增量建链

`GetAlgResAICPU` 根据 `increCreateChannelFlag` 和 `g_hostCtx` 判断资源处理方式。

### 10.1 首次创建

满足以下任一条件：

```text
非增量建链
或 g_hostCtx 中不存在当前 algTag
```

执行完整流程：

```text
申请 thread/channel
-> 保存 CCL buffer 描述
-> 创建 Device context
-> 复制资源上下文
```

### 10.2 完全复用

如果缓存中已经包含全部所需 channel：

```cpp
HcclEngineCtxGet(...)
```

直接返回已有 Device context，不重复申请 thread 和 channel。

### 10.3 增量建链

如果缓存资源缺少部分 channel：

```text
CompReqChannelWithExistChannel
-> HcclGetChannel 申请缺失 channel
-> HcclEngineCtxDestroy 销毁旧 Device context
-> HcclMemcpyCtxHostToDevice 重新序列化并复制 context
```

## 11. 不属于 GetAlgResAICPU 的资源

以下资源不由 `GetAlgResAICPU` 直接申请：

- 用户 input/output buffer：由融合算子提供。
- MC2 workspace：后续由 `HcclAllocOpResCtx` 申请。
- `OpResCtx` 本身：由 `HcclAllocOpResCtx` 封装和复制。
- AllReduce 运行时真实 count：来自 AICore 发送的 `HcclMsg`。
- AllReduce 输出内存：使用用户传入的 `recvBuf`。

## 12. 资源汇总

| 资源 | 数量或来源 | 作用 |
|---|---|---|
| CCL buffer | 通信域已有 buffer | ReduceScatter 接收、顺序 Reduce 和 AllGather 发送使用的 scratch |
| AICPU threads | `rankSize` | 并行执行本地复制和各远端 rank 的收发任务 |
| 展开线程 | 1 条 CPU thread | AICPU 算法展开和调度 |
| Notify | 主线程请求 `N-1`，每个从线程请求 1，实际额外增加同步 notify | 主从线程及 Host/Device 同步 |
| Mesh1D channels | 每个远端 rank 一条 | 跨 rank 数据传输 |
| Remote CCL memory | 每条 channel 一份 | 访问远端 ReduceScatter/AllGather 中转空间 |
| 图模式内存句柄 | 仅 `OFFLOAD` | 访问远端用户 input/output |
| Device resource context | 一份序列化 context | Device 侧恢复算法所需的全部资源描述 |

## 13. 日志含义

当出现以下日志：

```text
[asc][AlgoResource][GetAlgResAICPU] success,
opType[...],
algTag[...InsAllReduceMesh1DTwoShot...],
ctxSize[...],
resCtxSequence[...]
```

可以确认：

- 标准 TwoShot 需要的资源已经成功创建、复用或完成增量补充。
- 资源上下文已经复制到 Device，或者成功取得已有 Device context。
- `ctxSize` 和 `resCtxSequence` 分别给出了资源上下文大小和地址。

该日志只表示资源准备成功，不表示 TwoShot 已经实际运行。是否进入算法执行仍需检查：

```text
[InsV2AllReduceSoleExecutor][Orchestrate] Orchestrate Start.
[InsTempAllReduceMesh1DTwoShot] KernelRun Start.
[InsTempAllReduceMesh1DTwoShot] KernelRun finished.
[MC2_OPEN_DIAG][LoopAfterKernel] ... ret 0
[MC2_OPEN_DIAG][LoopAfterPost] ... ret 0
```
