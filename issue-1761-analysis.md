# Issue 1761 分析报告

## 1. 问题概述

Issue 1761 反馈：`group_barrier` 示例在 8 个 AIV 核执行时，所有核在 `SyncAll()` 前无条件调用：

```cpp
AscendC::InitOutput<int32_t>(dstGlobal, totalLength, 0);
```

`mssanitizer racecheck` 检测到：

- 28 条 GM WAW hazard；
- 24 条 GM RAW hazard；
- WAW 地址为输出缓冲区；
- RAW 地址为 `GroupBarrier` 的 `barWorkspace`。

## 2. 代码核查结果

当前代码位于 [group_barrier.asc:L42](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:42)：

```cpp
AscendC::GroupBarrier<AscendC::PipeMode::MTE3_MODE> barA(
    barWorkspace, ARRIVE_NUM, WAIT_NUM);
AscendC::InitOutput<int32_t>(dstGlobal, totalLength, 0);
AscendC::SyncAll();
```

kernel 启动 8 个 block，而 `InitOutput` 没有按 `GetBlockIdx()` 分工，因此 8 个 AIV 都向同一输出地址写入 0。8 个核两两组合数为：

```text
C(8, 2) = 28
```

因此，Issue 中关于 28 条 WAW 的描述与代码结构完全一致，问题属实。

当前通常不会表现为功能错误，是因为所有核写入的值都是相同的 0，属于幂等写入。但这仍是未建立同步关系前的多核同址写，存在以下风险：

1. racecheck 会报告真实的数据竞争；
2. 如果未来初始化值发生变化，最终结果可能依赖执行时序；
3. 该写法容易掩盖初始化阶段的同步设计问题。

## 3. RAW hazard 分析

`GroupBarrier::Wait()` 的实现位于 [kernel_operator_group_barrier_impl.h:L90](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/impl/basic_api/dav_3510/core_mng/roc/kernel_operator_group_barrier_impl.h:90)。Waiter 会通过 `dcci(... CACHELINE_OUT)` 刷新缓存，并持续轮询：

```cpp
while (BarrierInfoAddr->head < expectedArriveNum) {
    dcci(...);
}
```

Arriver 则通过原子累加更新 barrier 计数。该模式本身就是 GroupBarrier 的跨 AIV 软件同步机制：

- Arriver 原子更新计数；
- Waiter 通过缓存失效和轮询等待计数满足条件；
- 计数未满足时的读取属于预期轮询行为。

因此，24 条 RAW 更可能是工具对软件同步轮询的保守报告，不等价于 GroupBarrier 实现存在功能性错误。不过，仍建议在目标 SoC 和当前 mssanitizer 版本上补充确认；如果工具支持，应增加软件同步/轮询场景的误报抑制或规则说明。

## 4. 建议修复方案

将输出初始化限定为一个 block 执行，并使用现有 `SyncAll()` 建立初始化完成后的同步关系：

```cpp
auto id = AscendC::GetBlockIdx();

if (id == 0) {
    AscendC::InitOutput<int32_t>(dstGlobal, totalLength, 0);
}

AscendC::SyncAll();

if (id < ARRIVE_NUM) {
    ...
} else if (id < ARRIVE_NUM + WAIT_NUM) {
    ...
}
```

该方案可以消除 28 条 WAW，不改变原有 GroupBarrier 语义，并保证两个 Arriver 执行原子累加前输出已经完成初始化，同时保持 6 个 Waiter 最终读取结果为 `24`。

## 5. 验证建议

1. 重新编译 `group_barrier` 示例；
2. 运行普通功能验证，确认 6 个 Waiter 均输出 `24`；
3. 重新执行 `mssanitizer racecheck`，确认 28 条 WAW 消失；
4. 单独记录剩余 RAW 报告，并确认其仅来自 `barWorkspace` 的 GroupBarrier 轮询；
5. 在 `dav-2201`、`dav-3510` 等支持架构上进行回归。

本次分析未执行 NPU/mssanitizer 实机验证，结论基于 Issue 日志、当前仓库代码及 GroupBarrier 实现核查。

## 6. 结论

Issue 1761 **部分属实**：

- **28 条 WAW：确认属实，应修复示例代码。**
- **24 条 RAW：属于 GroupBarrier 软件同步轮询引起的工具报告，当前没有证据表明是功能性缺陷，但应补充工具侧确认或误报说明。**

Issue 原文：[GitCode Issue 1761](https://gitcode.com/cann/asc-devkit/issues/1761)

## 7. Issue 回复（200 字以内）

感谢您提交Issue1761。我们已确认group_barrier示例中所有AIV在同步前重复执行InitOutput，产生28条WAW竞争。修复时将仅由block0初始化输出，再通过SyncAll()同步。24条RAW来自GroupBarrier的原子更新与轮询读取，属于预期的软件同步行为，我们会继续确认工具表现。同时，我们将排查样例仓中同类的同步前多核同址写问题，并根据结果一并修复或补充说明。
