# MC2 AICPU 跨轮入口同步优化（默认关闭）

## 支持范围

本版本只裁剪 A5 本地 `AicpuAllGatherSoleNHR` 的安全入口：
实际只有一条通信 Thread 和一条后拷贝 Thread、同组 Thread 映射及 Notify 布局一致、
输入/输出/scratch 无别名，且额外 Notify 容量足够。

RS SoleNHR 按当前实施范围排除，等待后续多 Channel 支持。其他 RS、本地 AllReduce、
CANN bridge AllReduce、AllGather Mesh、Parallel/Sequence 等保留原同步。
`OUTPUT_DEFERRED_GROUP` 可以编码和校验分组，但本版本没有出口裁剪路径，
因此不会产生“中间 Wait 提前完成”的变化。不得将接口存在理解成这些路径已优化。

## 调用契约

```cpp
config.SetAlgorithmBoundarySync(
    AscendC::Mc2AlgorithmBoundarySyncMode::INPUT_READY_GROUP,
    7, 3); // 同一 AICPU Kernel、通信域内 groupId=7，总共 3 个 turn
```

每个 turn 是 CCoreWait 到 CCoreRecord 之间的一次完整 executor，
不是 executor 的 payload loop，也不是业务计算块。
支持一个 Prepare 的 repeat=3，或连续多个 Prepare 的 repeat 总和为 3。
同组 tiling 项必须同模式、同 groupId、同 totalTurns；组内不能插入非成员。
groupId 每个 Kernel 只用一次；不跨 Kernel。不完整组不能 Finalize。

INPUT_READY_GROUP 由调用方承诺整组输入已就绪且允许从线程提前进入后续轮次。
API 不推断业务算子，不放宽每轮结果完成语义。默认 DISABLED。

## 为什么单通信 Thread 的 NHR 可以优化

正常 OPBASE 路径的所有 NHR 通信任务在 main Thread。
后拷贝如果分配到 slave Thread，则前面已有算法内部的
`POST_COPY_NOTIFY_IDX=1` Record/Wait。它在 scratch 生产者之后发信号，
因此 slave 即使提前越过最外层入口，仍不能越过这道内部等待读取 scratch。

没有独立后拷贝时，slave 除了边界通知没有数据访问。
每轮最终 PostSync 始终保留，使 main 在进入下一轮 LocalDataCopy 前已收齐上一轮 slave。
因此 main 的 FIFO 和保留的内部等待共同保护跨轮 scratch 复用。
这项证明不适用于多个通信 Thread，也不直接推广到其他模板。

只省第 2～N 轮 executor 的首次入口；内部 payload loop 和 NHR step 均不改。
每轮最终出口改用独立槽位；中间 payload loop 继续用原索引。

## 资源、参数和缓存

- tiling reserved2 前 20 字节放置 magic/version/size 扩展；结构大小不变。
- OpParam 有效数据后附加私有扩展，加载后剥离；不修改 OpParam 或资源序列化布局。
- 参数及资源 tag 纳入配置；开启功能要求匹配的新 client/server。
- Notify 基址为算法最大 Notify 数 + 原 Host/Device 槽位；单 slave 每轮占一个额外槽位。
- 上限 64；容量不足整组保持原同步。原同步 Notify 索引不变。
- task-cache 标识纳入首次入口是否省略、实际出口槽位；miss/hit/bypass 使用相同策略。
- 轮次槽位会增加缓存任务图数量，需同时验收命中率、展开耗时和通信耗时。

## 回退和失败

未知路径、多 Channel、Thread 映射不一致、容量不足或动态地址别名均保留入口。
本版本不省出口，因此不存在需要延后补齐的出口。
分组配置不一致、重复组、轮数越界、提交失败及不完整 Finalize 返回错误。
已提交任务不重试、不重新展开。资源申请失败不以策略回退掩盖。
不修改 CCore、Channel ACK/DATA_SIGNAL、BatchMode 或 BatchTransfer 协议。

## 验证边界

仓内测试包含 wire 编解码、跨 Prepare/repeat 分组、失败完成检查、缓存策略隔离，
以及拦截实际 NHR primitive 构造多 Rank 队列后按快慢 main/slave 顺序执行。
模型中的普通 Notify 使用置位/等待清除，检测未消费信号覆盖；
检查带 padding 的 AllGather 输出和每轮完成时刻，而非仅检查任务数。

模拟与编译不能替代 NPU 测试。上板需覆盖两个及以上 turn、内部多 payload loop、
主尾块、非连续 stride、不同 dtype、cache miss/hit/bypass、快慢 Rank、
关闭配置的原任务图以及启用后的逐轮 Wait 结果正确性。
正式性能关闭 Profiling 采集，不预先承诺收益。
