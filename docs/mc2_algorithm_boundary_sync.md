# MC2 AICPU 跨轮入口同步优化（默认关闭）

## 支持范围

本版本只裁剪 A5 本地 `AicpuAllGatherSoleNHR` 的安全入口：
支持有效 K 条通信 Thread 和 K 条后拷贝 Thread、同组 Thread 映射及 Notify 布局一致、
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
多个通信 Thread 时，非主通信 Thread 的入口仍保留：main 收齐上一轮全部后拷贝完成后，
才放行下一轮通信和 scratch 写入。只有后拷贝 Thread 的外层入口可以省略，
其内部 copy-ready 等待保留。因此不会发生快通信 Thread 覆盖慢后拷贝 Thread 正在读取的 scratch。
这不是删除所有 slave 的入口，也不推广到其他模板。

只省第 2～N 轮 executor 的首次入口；内部 payload loop 和 NHR step 均不改。
每轮最终出口改用独立槽位；中间 payload loop 继续用原索引。

## 资源、参数和缓存

- tiling reserved2 前 20 字节放置 magic/version/size 扩展；结构大小不变。
- OpParam 有效数据后附加私有扩展，加载后剥离；不修改 OpParam 或资源序列化布局。
- 参数及资源 tag 使用 `mb2` 纳入配置；开启功能要求匹配的新 client/server。
- Notify 基址 B 为算法最大 Notify 数 + 原 Host/Device 槽位。
  Host 按申请 Thread 数 A 保守预留 `N*(A-1)` 个额外槽位；
  Device 对有效 T 条 Thread 使用 `B+turn*(T-1)+slaveIndex` 的独立出口槽位。
- 上限 64；容量不足整组保持原同步。原同步 Notify 索引不变。
- task-cache 标识纳入首次入口是否省略、实际出口槽位；miss/hit/bypass 使用相同策略。
- 轮次槽位会增加缓存任务图数量，需同时验收命中率、展开耗时和通信耗时。

## 回退和失败

未知路径、不均匀 Channel、Thread 映射不一致、容量不足或动态地址别名均保留入口。
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

## 当前主线适配与回退诊断

本功能按实际执行布局支持 `AicpuAllGatherSoleNHR`，不按申请资源数组大小猜测执行线程数。
保留现有 isPod 条件：非 POD 的普通 SoleNHR 仍使用模板单 Channel 默认值，
即使申请4条 Thread，也可能只有前2条参与执行；不会为启用优化而改变 Channel 选择。
POD 分支要求所有 peer 的有效 Channel 数相同、分区权重一致且为 UB 协议，K=M，T=2K。
本次只开放 K=1/2；更多 Channel 保守回退，不把双 Channel 验证外推。
主线的 `AicpuAllGatherSoleNHRMultiLink` 即使复用同一 NHR 模板也不在白名单中；
不会仅放宽算法名称匹配来启用 MultiLink。
CCU、其他 AllGather、ReduceScatter 和 CANN bridge AllReduce 均保持原任务图。

DEBUG 级别可检索 `[MC2_BOUNDARY_SYNC][BEGIN]`、`[FALLBACK]` 和 `[SUMMARY]`。
每组每种回退原因最多记录一次，后续 Prepare 地址变化导致的首次回退也会记录，
不输出逐任务日志。常见原因：

| 原因 | 含义 |
| --- | --- |
| `unsupported_mode` / `single_turn` | 未接入的模式或只有一轮 |
| `unsupported_algorithm` | 整组包含不支持的引擎或算法，包括 MultiLink |
| `channel_layout` / `uneven_channel_layout` | 空资源或各 peer Channel 数不一致 |
| `channel_protocol_or_partition` | 无效/重复 Channel、非 UB 或各 peer 分区权重不一致 |
| `unsupported_channel_count` / `channel_partition_overflow` | 超过两条有效 Channel 或权重求和溢出 |
| `thread_layout` | 有效线程不足、句柄为空/重复或组内映射不同 |
| `notify_budget_or_layout` / `notify_budget` | Notify 资源不足或组内布局不一致 |
| `zero_count` / `invalid_range_or_stride` | 当前轮无数据或地址范围计算不满足条件 |
| `buffer_alias_or_invalid_span` | 用户 buffer 与彼此或 scratch 重叠，或范围无效 |
| `unsupported_algorithm_or_mode` | 本轮参数属于未接入的算法、对称内存或非 OPBASE 模式 |

配置默认关闭。benchmark 的 `-n` 不等于同一 Kernel 的 `totalTurns`；
须先构造同一 Kernel 的多轮 Prepare/repeat，并确认 BEGIN 的 `active_threads`、
`communication_threads` 和 SUMMARY 的 `removed_entry_pairs`、`removed_notify_tasks`。
对 K 条后拷贝线程，N 轮理想减少 `2 × K × (N − 1)` 个入口 Notify 任务；
内部后拷贝通知、Channel 握手、payload-loop 同步和每轮出口均不减少。
每轮出口槽位对应不同缓存任务图，需同时记录首次捕获、缓存命中率与占用，不能承诺固定微秒收益。

依赖模拟直接执行真实 Sole executor（不再手写其 loop 策略），覆盖完整 loop 加尾 loop、
非连续 stride、开关关闭/开启、BatchTransfer 开启/关闭和快慢线程调度；
运行时准入测试检查别名、零长度、Notify 容量和 MultiLink 回退，并验证复用策略对象不残留裁剪标志。
通过真实 task-cache runner 加 Host 缓存模型，对 miss、hit、容量满 bypass 的任务类型、
地址、长度、Notify 槽位及每线程顺序逐项比较，再执行快慢线程模拟验证结果。
BatchTransfer 能力在进程内不可改变，因此开关覆盖使用两个独立测试进程；不修改生产能力缓存。
这些仍是 Host 队列模拟，不等于真实 HCOMM SQE/WQE 地址重定位或 NPU 验收。

扩展测试采用16逻辑rank，覆盖申请4条但执行2条、真正双Channel/4条执行线程、
非等量3:1分区、尾loop、四类dtype，以及保留通信slave入口后的快慢后拷贝调度。
例如 B=4、4 turn、4 Thread 需要16个Notify，而不是旧实现的8个。
超过64槽位时整组回退；未改变 tiling wire、OpParam 或资源序列化布局。
