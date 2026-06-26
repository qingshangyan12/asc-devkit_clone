# Finalize

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas 训练系列产品：不支持
<!-- end id6 -->

## 功能说明

通知服务端后续无通信任务，执行结束后退出。

## 函数原型

```
template <bool sync = true>
__aicore__ inline void Finalize()
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| sync | 输入 | 是否需要等待服务端的通信完成。bool类型，参数取值如下：<br>true：默认值，表示客户端将检测并等待最后一个通信任务完成。<br>false：表示客户端发送Finalize通知后，不等待服务端通信任务完成即退出。该取值仅跳过客户端侧的阻塞等待，不表示已下发的通信任务已完成，也不保证返回时对端已收到[BatchWrite](BatchWrite.md)数据。<!-- npu="A3" id7 --><br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，该参数支持默认值true，仅在通信任务为[BatchWrite](BatchWrite.md)时，支持取值为false。<!-- end id7 --><!-- npu="910b" id8 --><br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，该参数仅支持默认值true。<!-- end id8 --> |

## 返回值说明

无

## 约束说明

-   调用本接口前确保已调用过[InitV2](InitV2.md)和[SetCcTilingV2](SetCcTilingV2.md)接口。
-   本接口在AIC核或者AIV核上调用必须与对应的Prepare接口的调用核保持一致。
-   sync为false时，客户端发送Finalize通知后不会等待服务端通信任务完成，不能将Finalize\<false\>()返回作为[BatchWrite](BatchWrite.md)数据已到达对端或对端Global Memory数据已可用的依据。
-   调用Finalize\<false\>()前，必须确保需要保证完成的通信任务已同步完成，否则客户端直接退出后，无法保证数据传输完成。多队列[BatchWrite](BatchWrite.md)场景需先对所有队列调用[QueueBarrier](QueueBarrier.md)接口，等待队列上的BatchWrite任务完成后再调用Finalize\<false\>()（参见[BatchWrite](BatchWrite.md)的"多个队列的点对点通信"调用示例）；单handle场景需先调用[Wait](Wait-98.md)接口等待对应任务完成。
-   Finalize为终止接口：无论sync取值为true或false，调用后服务端开始退出，当前Hccl对象不可再次用于通信。如需继续通信，需重新创建Hccl对象并调用[InitV2](InitV2.md)接口初始化。

## 调用示例

```
REGISTER_TILING_DEFAULT(ReduceScatterCustomTilingData); // ReduceScatterCustomTilingData为对应算子头文件定义的结构体
GET_TILING_DATA_WITH_STRUCT(ReduceScatterCustomTilingData, tilingData, tilingGM);
Hccl hccl;
GM_ADDR contextGM = AscendC::GetHcclContext<0>(); // AscendC自定义算子kernel中，通过此方式获取HCCL context
hccl.InitV2(contextGM, &tilingData);
hccl.SetCcTilingV2(offsetof(ReduceScatterCustomTilingData, mc2CcTiling));
if (AscendC::g_coreType == AIC) {
    HcclHandle handleId =
        hccl.ReduceScatter(sendBuf, recvBuf, 100, HcclDataType::HCCL_DATA_TYPE_INT8, HcclReduceOp::HCCL_REDUCE_SUM, 10);
    hccl.Commit(handleId); // 通知服务端可以执行上述的ReduceScatter任务
    for (uint8_t i = 0; i < 10; i++) {
        hccl.Wait(handleId); // 阻塞接口，需等待上述ReduceScatter任务执行完毕
    }
    hccl.Finalize<true>(); // 后续无其他通信任务，通知服务端执行上述ReduceScatter任务之后即可以退出
}
```

sync取值为false时，需先完成必要的通信同步；多队列BatchWrite场景的QueueBarrier前置同步示例请参见[BatchWrite](BatchWrite.md)的"多个队列的点对点通信"调用示例。
