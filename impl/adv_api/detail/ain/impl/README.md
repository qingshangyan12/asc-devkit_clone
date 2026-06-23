# AIN介绍

## 功能说明

AIN（Accelerator Initiated Networking）是Ascend C提供的Device侧主动通信接口，用于支持算子Kernel在AI Core侧主动发起跨Rank的数据访问和通知操作。

AIN面向需要在Kernel执行过程中编排点对点通信的场景。用户在Host侧完成通信资源、对称内存窗口和Signal窗口的创建后，可在Device侧通过AIN接口访问这些资源，实现远端写、远端读、远端通知、本地Signal读取和等待等操作。

AIN不负责创建底层通信资源。当前实现基于HCOMM Channel完成数据传输，其他Host侧初始化流程负责准备通信域、Rank信息、Channel以及对称内存窗口，接口使用请参考[HCOMM](https://gitcode.com/cann/hcomm/blob/master/docs/zh/api_ref/comm_mgr_c/README.md)。

## 通信模型

通信双方预先注册可访问的Global Memory区域，Device侧接口通过Window Index和Offset定位实际访问地址。Signal窗口同样基于HCOMM对称内存窗口实现，用于存储和管理跨Rank的Signal通知数据。

**表 1**  AIN主要通信能力

| 能力 | 接口 | 说明 |
| --- | --- | --- |
| 远端写 | `put` | 将本Rank本地Window中的数据写入目标Peer的远端Window。 |
| 远端读 | `get` | 从目标Peer的远端Window读取数据到本Rank本地Window。 |
| 远端通知 | `signal` | 对目标Peer的Indexed Signal发起通知。 |
| 本地读取Signal | `readSignal` | 读取本Rank本地Signal值。 |
| 本地等待Signal | `waitSignal` | 轮询本Rank本地Signal，直到Signal值满足条件。 |
| 完成等待 | `flush` | 提交并等待当前AIN对象记录的延迟通信操作完成。 |


## 约束说明

- 使用AIN接口前，Host侧必须完成通信资源、HCOMM Channel、对称内存窗口的创建。
- `devComm`必须为有效Device侧通信资源地址。空指针仅可用于编译覆盖，不能执行通信接口。
- `put`、`get`和`signal`依赖有效UB临时工作区。
- 当前`team`参数和`contextIndex`参数为预留参数，现有实现尚未基于这些参数选择不同通信资源。
- 当前`AinMemoryOrder`参数为预留参数，现有实现尚未基于不同Memory Order生成不同分支。

