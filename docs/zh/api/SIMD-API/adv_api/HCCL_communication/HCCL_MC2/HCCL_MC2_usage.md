# HCCL MC2使用说明

> [!NOTE]说明
>本章节描述的Host侧接口用于替代aclnn路径中通过Tiling数据申请通信资源、再下发通信内核的流程。接口同时支持AICPU和CCU两种通信服务端路径，二者共用同一组参数配置和资源上下文接口。

HCCL MC2接口由Host侧参数配置、资源上下文申请和通信服务端启动三部分组成。用户在Host侧配置通信任务参数后，接口内部会完成算法选择、通信资源申请以及设备侧资源上下文的写入。

典型流程如下：

1.  调用[Mc2GetCcArgs](Mc2GetCcArgs.md)创建通信参数对象。

2.  调用配置接口填充通信参数。

    -   [Mc2SetCcCommEngine](Mc2SetCcCommEngine.md)：设置通信引擎。
    -   [Mc2SetCcSrcDataType](Mc2SetCcSrcDataType.md)：设置输入数据类型。
    -   [Mc2SetCcDstDataType](Mc2SetCcDstDataType.md)：设置输出数据类型。
    -   [Mc2SetCcReduceType](Mc2SetCcReduceType.md)：设置Reduce操作类型，仅AllReduce、ReduceScatter等归约类任务需要配置。
    -   [Mc2SetCcAlgConfig](Mc2SetCcAlgConfig.md)：设置通信算法。

3.  调用[Mc2AcquireCcResCtx](Mc2AcquireCcResCtx.md)申请通信资源上下文，获得设备侧资源上下文地址和大小。

4.  调用[Mc2CcKernelLaunch](Mc2CcKernelLaunch.md)启动通信服务端。AICPU路径传入独立于AICore任务的AICPU任务流；CCU路径可传入nullptr。

5.  在AI Core核函数中，将步骤3返回的资源上下文传入[InitV2](../HCCL_Kernel/InitV2.md)，再调用AllGather、AllReduce、AlltoAll等HCCL高阶API下发客户端通信任务。

6.  参数对象不再使用后，调用[Mc2FreeCcArgs](Mc2FreeCcArgs.md)释放。

上述流程中的第3步与第5步必须使用同一个资源上下文。ccResCtx由通信域生命周期管理，用户不需要调用单独的接口释放该资源。

[Mc2GetCcArgs](Mc2GetCcArgs.md)创建的参数对象默认通信引擎为AICPU_TS。若使用CCU路径，必须调用[Mc2SetCcCommEngine](Mc2SetCcCommEngine.md)显式设置通信引擎，推荐设置为CCU_SCHED。

AICPU与CCU路径的主要差异如下：

| 项目 | AICPU路径 | CCU路径 |
| --- | --- | --- |
| commEngine取值 | 2，AICPU_TS | 5，CCU_MS；或6，CCU_SCHED，推荐使用6 |
| 服务端启动stream | 必须传入独立的aclrtStream | 传入nullptr，launch stream由资源上下文内部携带 |
| AI Core侧初始化 | hccl.InitV2(contextGM) | hccl.InitV2(contextGM) |
| HCCL模板参数 | HCCL_SERVER_TYPE_AICPU | HCCL_SERVER_TYPE_CCU |
| 退出机制 | AICore侧调用Finalize后服务端退出 | AICore侧调用Finalize后服务端退出 |
| rank数 | 必须大于1 | 支持单rank和多rank |

以下示例展示AlltoAll任务使用CCU通信路径时的Host侧调用顺序：

```
void* ccArgs = nullptr;
void* ccResCtx = nullptr;
uint32_t ccResCtxSize = 0U;
constexpr uint8_t kCcuSchedEngine = 6U; // CCU_SCHED
const uint8_t ccType = static_cast<uint8_t>(AscendC::HcclCMDType::HCCL_CMD_ALLTOALL);
char algConfig[] = "CcuSchedAllToAllSoleMesh";

Mc2Result ret = Mc2GetCcArgs(&ccArgs);
ret = Mc2SetCcCommEngine(ccArgs, kCcuSchedEngine);
ret = Mc2SetCcSrcDataType(ccArgs, HCCL_DATA_TYPE_FP32);
ret = Mc2SetCcDstDataType(ccArgs, HCCL_DATA_TYPE_FP32);
ret = Mc2SetCcAlgConfig(ccArgs, algConfig);

ret = Mc2AcquireCcResCtx(comm, ccType, ccArgs, &ccResCtx, &ccResCtxSize);
(void)Mc2FreeCcArgs(ccArgs);

ret = Mc2CcKernelLaunch(nullptr, ccResCtx, ccResCtxSize);

// 将ccResCtx传入AI Core核函数，并在核函数内调用InitV2和AlltoAll接口。
all_to_all_kernel<<<1, nullptr, streamAiv>>>(sendBuf, recvBuf, ccResCtx, dataCount);
```

以下示例展示AllGather任务使用AICPU通信路径时的调用顺序。AICPU服务端会阻塞等待AICore客户端消息，因此aicpuStream与streamAiv必须为两条不同的任务流，且建议先同步AICore任务流、再同步AICPU任务流。

```
void* ccArgs = nullptr;
void* ccResCtx = nullptr;
uint32_t ccResCtxSize = 0U;
constexpr uint8_t kAicpuTsEngine = 2U; // AICPU_TS
const uint8_t ccType = static_cast<uint8_t>(AscendC::HcclCMDType::HCCL_CMD_ALLGATHER);

Mc2Result ret = Mc2GetCcArgs(&ccArgs);
ret = Mc2SetCcCommEngine(ccArgs, kAicpuTsEngine);
ret = Mc2SetCcSrcDataType(ccArgs, HCCL_DATA_TYPE_FP32);
ret = Mc2SetCcDstDataType(ccArgs, HCCL_DATA_TYPE_FP32);

ret = Mc2AcquireCcResCtx(comm, ccType, ccArgs, &ccResCtx, &ccResCtxSize);
(void)Mc2FreeCcArgs(ccArgs);

ret = Mc2CcKernelLaunch(aicpuStream, ccResCtx, ccResCtxSize);

all_gather_kernel<<<1, nullptr, streamAiv>>>(sendBuf, recvBuf, ccResCtx, sendCount);
```

AICore核函数中的AICPU客户端示例片段如下：

```
__global__ __vector__ void all_gather_kernel(
    __gm__ void* sendBuf, __gm__ void* recvBuf, __gm__ void* contextGM, uint64_t sendCount)
{
    if (g_coreType != AscendC::AIV) {
        return;
    }
    AscendC::InitSocState();
    AscendC::Hccl<AscendC::HcclServerType::HCCL_SERVER_TYPE_AICPU> hccl;
    hccl.InitV2((GM_ADDR)contextGM, nullptr);
    AscendC::HcclHandle handleId = hccl.AllGather<true>(
        (GM_ADDR)sendBuf, (GM_ADDR)recvBuf, sendCount,
        AscendC::HcclDataType::HCCL_DATA_TYPE_FP32, 0);
    hccl.Wait(handleId);
    AscendC::SyncAll<true>();
    hccl.Finalize();
    AscendC::PipeBarrier<PIPE_ALL>();
}
```
