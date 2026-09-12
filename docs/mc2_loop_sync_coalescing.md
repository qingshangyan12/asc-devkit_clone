# MC2 ReduceScatter SoleNHR 内部循环同步合并

本功能默认关闭，仅用于 A5 MC2 AICPU_TS 的本地
`AicpuReduceScatterSoleNHR`，且拓扑必须满足 HCCL 同款 `isPod` 多 Channel 条件。
它不是跨 Prepare、跨 repeat turn 或跨 CCoreWait/CCoreRecord 的同步删除。

## 启用

在原有 `Mc2CcTilingConfig` 设置完成后、生成通信 tiling 前调用：

```cpp
if (config.SetEnableLoopSyncCoalescing(1) != 0) {
    // 按算子现有错误处理路径返回。
}
```

只接受 0/1。开启表示允许安全规划，不保证当前 shape 一定合并。
关闭、非 POD、其他算法和引擎保持原同步策略。
本版本同时补齐 SoleNHR 在 POD 上的多 Channel 配置；这项前置对齐与
同步合并是两个独立因素，性能归因时应分别对比。

## 合并范围

一次 executor 可能因为 scratch 容量限制处理多个内部 loop。
仅相邻完整 loop 的 count、切片和 Channel/Thread 所有权一致时，移除前一
loop 的出口 PostSync 和后一 loop 的入口 PreSync。保留段首入口、段尾出口。
较短尾 loop 前仍执行完整边界。

```text
CCoreWait
  PreSync -> full 0 -> full 1 -> full 2 -> PostSync
  PreSync -> short tail -> PostSync
CCoreRecord
```

Channel ACK、DATA_SIGNAL、WriteReduce 融合、NHR step、LocalDataCopy 和
PostLocalCopy 均保留。输入 Rank 间距仍使用完整 count/strideCount，不能使用
当前 loop 的 count 替代。合法非对齐 stride 不增加新限制。

## 准入和回退

规划在任何算法任务和 cache lookup 前完成，检查输入、输出、scratch 的
范围与别名、Thread 唯一性、Channel 归属、各 peer 的分区一致性和尾块。
PCIe、对称内存、OFFLOAD、不完整资源、单 Thread 或单 loop 不合并。
无法证明安全时保留全同步，不在部分任务已经提交后回退重跑。

缓存键包含版本化循环策略和实际边界；每次地址改变都重新检查别名。
miss 捕获、hit 重放、bypass 完整展开使用同一任务图。DEBUG 的
`[MC2_LOOP_SYNC]` 在实际展开时记录准入原因，不逐任务打印；缓存命中不重复打印。

## 兼容边界

`Mc2CcTilingInner` 保持 512 字节，使用 reserved2 的前 8 字节编码。
基础 OpParam 和资源序列化不变；开启时通过独立参数尾部扩展传递，server
拆分扩展后仅向原算法/bridge 传递基础参数。旧格式按关闭处理。
已识别但损坏的扩展报错。启用功能要求 client/server 使用配套版本，
不支持新 client 与旧 server 混装启用。

## 验收

必须先确认多 Channel、内部 loops > 1 且实际出现可合并段。
对比开关两侧 Notify 图、所有输出和 padding、非连续 stride、多 turn、
cache miss/hit/bypass，并制造快慢 Channel。一次合法边界合并减少约 4S 个
本地 Notify 任务（S 为从 Thread 数），不减少 Channel 握手或数据任务。

ST/目标构建不替代 NPU 验证。本功能不承诺实测性能收益；板端正确性、死锁
和性能 A/B 尚需验收，正式性能采集关闭 Profiling。
