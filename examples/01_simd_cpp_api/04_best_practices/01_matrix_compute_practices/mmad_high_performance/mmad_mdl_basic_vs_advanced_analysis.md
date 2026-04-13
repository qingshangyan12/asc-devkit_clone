# MMAD MDL 基础API vs 高阶API 实现对比与修改分析

## 一、背景

在矩阵乘法 (MMAD) 的 MDL (Multi-Data Loading) 模式中，K轴采用不对称的步进参数 `stepKa` 和 `stepKb`（如 stepKa=8, stepKb=4），使得 A/B 矩阵在 L1 中的缓存粒度不同。本文档对比高阶API和基础API的MDL实现差异，并说明修改原因。

---

## 二、高阶API MDL 实现

### 2.1 整体架构

高阶API的MDL实现由以下核心模块组成：

| 模块 | 源文件 | 职责 |
|------|--------|------|
| `KLoopMDLBase` | `k_loop_mdl_base.h` | K轴双层循环迭代器（外层/内层） |
| `CopyCubeInMDLBase` | `copy_cube_in_mdl_base.h` | L1数据加载（Hit/AllocTensor/EnQue/DeQue） |
| `CubeInBuffer(DoubleBuffer)` | `cube_in_buffer_double_buffer.h` | L1双缓冲管理（Ping/Pong缓存命中判断与释放） |
| `MatmulMDLSchedulerBase` | `scheduler_mdl_base.h` | MDL调度器（CopyIn/Split/Compute/ClearL1Cache） |

### 2.2 K轴双层循环结构（KLoopMDLBase）

高阶API最核心的设计是**以 `minStepK` 为粒度的内外双层K循环**：

```cpp
// 参数推导
minStepK_ = stepKa > stepKb ? stepKb : stepKa;       // minStepK = min(stepKa, stepKb) = 4
kaStepFactor_ = stepKa / minStepK_;                    // kaStepFactor = 8/4 = 2
kbStepFactor_ = stepKb / minStepK_;                    // kbStepFactor = 4/4 = 1
outerKaIter_ = Ceil(singleCoreK, baseK * stepKa);      // A矩阵外层循环次数
outerKbIter_ = Ceil(singleCoreK, baseK * stepKb);      // B矩阵外层循环次数
outIter_ = max(outerKaIter_, outerKbIter_);             // 外层总循环次数
```

**循环结构**：

```
for outerIdx in [0, outIter_):
    innerStartIdx_ = outerIdx * minStepK_          // 当前外层循环的内层起始索引
    curKaOuterIdx = innerStartIdx_ / stepKa         // A矩阵当前外层K索引
    curKbOuterIdx = innerStartIdx_ / stepKb         // B矩阵当前外层K索引
    tileShapeA = stepKa * baseK (或 tailStepKa_)    // A的L1 tile大小
    tileShapeB = stepKb * baseK (或 tailStepKb_)    // B的L1 tile大小
    innerIter_ = min(tileShapeA, tileShapeB) / baseBlockSize  // 内层循环次数

    for innerIdx in [innerStartIdx_, innerStartIdx_ + innerIter_):
        // CopyIn -> Split -> Compute
```

**关键方法**：

| 方法 | 功能 |
|------|------|
| `GetOuterKaIdx()` | 返回 `outerIdx / kaStepFactor`，用于CopyIn中确定A的buffer位置 |
| `GetOuterKbIdx()` | 返回 `outerIdx / kbStepFactor`，用于CopyIn中确定B的buffer位置 |
| `GetNextOuterKaIdx()` | 返回 `(outerIdx + 1) / kaStepFactor`，用于ClearL1Cache判断A是否需要释放 |
| `GetNextOuterKbIdx()` | 返回 `(outerIdx + 1) / kbStepFactor`，用于ClearL1Cache判断B是否需要释放 |

**stepKa=8, stepKb=4 的实例化**：

```
minStepK = 4, kaStepFactor = 2, kbStepFactor = 1

outerIdx=0: innerStartIdx=0,  curKaOuterIdx=0, curKbOuterIdx=0
outerIdx=1: innerStartIdx=4,  curKaOuterIdx=0, curKbOuterIdx=1
outerIdx=2: innerStartIdx=8,  curKaOuterIdx=1, curKbOuterIdx=2
outerIdx=3: innerStartIdx=12, curKaOuterIdx=1, curKbOuterIdx=3
...

可见：A矩阵每2个外层循环才切换一次L1 buffer，B矩阵每个外层循环都切换。
```

### 2.3 L1缓存命中与释放（CopyCubeInMDLBase + CubeInBuffer）

高阶API使用 **TQue（EnQue/DeQue）** 模型管理L1双缓冲：

```cpp
// CopyCubeInMDLBase::LoadData
template <typename ScheduleContext = int>
__aicore__ inline LocalTensor<TransT> LoadData(
    int32_t curRow, int32_t curCol, int32_t tileHeight, int32_t tileWidth, const ScheduleContext& context = {})
{
    LocalTensor<TransT> l1;
    int32_t posL1 = BASE_MODULE::GetIterIndex(curRow, curCol);
    int32_t bufferPos = MATMUL_MODULE(CopyCubeInParams)->GetBufferPos();
    if (MATMUL_MODULE(CubeInBuffer)->Hit(posL1, bufferPos)) {
        // 缓存命中：直接返回已有buffer，不重复搬运
        l1 = MATMUL_MODULE(CubeInBuffer)->GetBuffer(posL1, bufferPos);
    } else {
        // 缓存未命中：分配buffer，执行DataCopy，EnQue/DeQue同步
        l1 = MATMUL_MODULE(CubeInBuffer)->AllocTensor(bufferPos);
        MATMUL_MODULE(DataCopyUtils)->CopyTileToCube(l1, curRow, curCol, tileHeight, tileWidth);
        MATMUL_MODULE(CubeInBuffer)->EnQue(l1);
        MATMUL_MODULE(CubeInBuffer)->DeQue();
    }
    return l1;
}
```

**Hit/AllocTensor/FreeTensor 的buffer管理**：

| 操作 | 说明 |
|------|------|
| `Hit(posL1, bufferPos)` | 判断L1中是否已有该位置的数据。当 `iterIndex != 0 || IsBufferCaching(bufferPos & cacheFactor_)` 时命中 |
| `AllocTensor(bufferPos)` | 从TQue分配一个tensor，设置缓存标记 |
| `EnQue(tensor)` | 入队，触发MTE2→MTE1的硬件同步 |
| `DeQue()` | 出队，等待MTE2搬运完成 |
| `FreeTensor(bufferPos)` | 释放buffer，清除缓存标记。在ClearL1Cache时调用 |

### 2.4 L1缓存清除（MatmulMDLSchedulerBase::ClearL1BufferCache）

```cpp
__aicore__ inline void ClearL1BufferCache(int32_t& curKaOuterIdx, int32_t& curKbOuterIdx)
{
    // A矩阵：下一个outerKaIdx与当前不同，且A不是全量加载时，释放A1
    int32_t curKaIdx = MATMUL_MODULE(KLoop)->GetNextOuterKaIdx();
    if (curKaIdx != curKaOuterIdx && !MATMUL_MODULE(KLoop)->IsAKL1FullLoad()) {
        MATMUL_MODULE(CopyCubeInA)->ClearLoadData();  // 调用 FreeTensor
        curKaOuterIdx = curKaIdx;
    }
    // B矩阵同理
    int32_t curKbIdx = MATMUL_MODULE(KLoop)->GetNextOuterKbIdx();
    if (curKbIdx != curKbOuterIdx && !MATMUL_MODULE(KLoop)->IsBKL1FullLoad()) {
        MATMUL_MODULE(CopyCubeInB)->ClearLoadData();
        curKbOuterIdx = curKbIdx;
    }
}
```

**核心思想**：外层循环推进时，通过 `GetNextOuterKaIdx()` 和 `GetNextOuterKbIdx()` 判断下一个外层K索引是否与当前不同。如果不同，说明当前L1 buffer中的数据已全部消费完毕，可以释放。

### 2.5 Split参数更新（MatmulMDLSchedulerBase::UpdateSplitParams）

```cpp
__aicore__ inline void UpdateSplitParams(SplitParams& aL0Params, SplitParams& bL0Params)
{
    int32_t kInnerIdx = MATMUL_MODULE(KLoop)->GetInnerStartIdx();
    // A矩阵的L1 K轴偏移 = (kInnerIdx % stepKa) * baseK
    aL0Params.kAxisL1Offset = (kInnerIdx - kInnerIdx / tilingStepKa * tilingStepKa) * tilingBaseK;
    // B矩阵的L1 K轴偏移 = (kInnerIdx % stepKb) * baseK
    bL0Params.kAxisL1Offset = (kInnerIdx - kInnerIdx / tilingStepKb * tilingStepKb) * tilingBaseK;
}
```

**核心思想**：DataLoad从L1搬运到L0时，需要计算在L1大包中的偏移。该偏移为 `kInnerIdx % stepK * baseK`，即当前内层K索引在其所属的stepK大包内的相对偏移。

---

## 三、基础API MDL 实现（修改前）

### 3.1 循环结构

修改前的基础API使用**简单的线性K循环**：

```cpp
for (uint32_t kIndex = 0; kIndex < kLoopCount; kIndex++) {
    CopyInA(a1Local, kIndex, mIndex);
    CopyInB(b1Local, kIndex, nIndex);
    DataLoadA(a1LocalTmp, a2Local, mIndex, kIndex);
    DataLoadB(b1LocalTmp, b2Local, nIndex, kIndex);
    Compute(cLocal, a2Local, b2Local, kIndex, mIndex, nIndex);
}
```

### 3.2 同步模型

基础API使用**静态tensor + SetFlag/WaitFlag**同步模型：

| 层级 | 正向同步（当前循环内） | 反向同步（跨循环依赖） |
|------|------------------------|------------------------|
| GM→L1 | `SetFlag<MTE2_MTE1>` → `WaitFlag<MTE2_MTE1>` | `WaitFlag<MTE1_MTE2>` → `SetFlag<MTE1_MTE2>` |
| L1→L0 | `SetFlag<MTE1_M>` → `WaitFlag<MTE1_M>` | `WaitFlag<M_MTE1>` → `SetFlag<M_MTE1>` |

### 3.3 修改前的同步流程

修改前的同步流程如下（以A矩阵为例，B矩阵类似）：

```
CopyInA (kIndex % stepKa == 0时执行):
    WaitFlag<MTE1_MTE2>(mte2DBFlagA)   ← 等待上一个buffer被消费完
    DataCopy(GM → A1 Ping/Pong)
    SetFlag<MTE2_MTE1>(0)              ← 通知DataLoadA数据就绪
    mte2DBFlagA = 1 - mte2DBFlagA

DataLoadA (每次循环执行):
    if (kIndex % stepKa == 0)
        WaitFlag<MTE2_MTE1>(0)         ← 等待CopyInA完成
    LoadData(A1 → A2)
    // ← 没有SetFlag<MTE1_MTE2>！

Compute (每次循环执行):
    SetFlag<MTE1_M>(mte1DBFlag)
    WaitFlag<MTE1_M>(mte1DBFlag)
    Mmad(...)
    SetFlag<M_MTE1>(mte1DBFlag)        ← 通知下一轮DataLoad
    mte1DBFlag = 1 - mte1DBFlag
```

### 3.4 修改前存在的同步缺陷

**关键问题：DataLoadA/DataLoadB 之后缺少 `SetFlag<MTE1_MTE2>`**

`MTE1_MTE2` 是反向同步事件，用于防止 MTE2（CopyIn）覆盖 MTE1（DataLoad）尚未消费完的 L1 buffer。完整的同步对应该为：

```
生产端（CopyIn）:  WaitFlag<MTE1_MTE2>  ← 等待消费端释放
消费端（DataLoad）: SetFlag<MTE1_MTE2>  ← 消费完毕后释放
```

修改前，只有 `InitSyncFlags` 中初始设置了 `SetFlag<MTE1_MTE2>(0/1/2/3)`，DataLoad 之后没有任何 `SetFlag<MTE1_MTE2>`。这意味着：

- **第一次 CopyInA**（kIndex=0）：`WaitFlag<MTE1_MTE2>(0)` 消耗了 InitSyncFlags 的初始 flag，正常通过
- **第二次 CopyInA**（kIndex=8）：`WaitFlag<MTE1_MTE2>(1)` 会**永久阻塞**，因为没有任何地方再次 `SetFlag<MTE1_MTE2>(1)`

> **注意**：在实际硬件上，由于 SetFlag/WaitFlag 是基于 event ID 的信号量机制，InitSyncFlags 设置的 flag 在第一次 WaitFlag 时被消耗后就不再有效。后续如果没有对应的 SetFlag，WaitFlag 就会一直等待。因此在 stepKa > 1 的场景下，修改前的代码**实际上无法正确运行**。

---

## 四、差异对比

### 4.1 核心差异总览

| 维度 | 高阶API | 基础API（修改前） |
|------|---------|-------------------|
| **K循环结构** | 内外双层循环（outerIdx + innerIdx），以 minStepK 为粒度 | 简单线性循环（kIndex: 0 → kLoopCount） |
| **同步模型** | TQue（EnQue/DeQue/AllocTensor/FreeTensor） | 静态tensor（SetFlag/WaitFlag） |
| **L1缓存命中** | `Hit(posL1, bufferPos)` 显式判断 | `kIndex % stepKa == 0` 隐式判断 |
| **L1缓存释放** | `FreeTensor` 在 ClearL1BufferCache 中调用 | **缺失**：DataLoad 后没有 SetFlag<MTE1_MTE2> |
| **K偏移计算** | `kAxisL1Offset = (kInnerIdx % stepKa) * baseK` | 相同逻辑 `kModStepKa * baseK * baseM` |
| **A/B独立切换** | kaStepFactor/kbStepFactor 控制A/B不同步切换 | mte2DBFlagA/mte2DBFlagB 独立管理 |

### 4.2 循环结构差异分析

**重要说明：双层循环与线性循环在功能上是等价的**。

线性循环 `for (kIndex = 0; kIndex < kLoopCount; kIndex++)` 与双层循环 `kIndex = outerKIdx * minStepK + innerKOffset` 产生的 kIndex 序列完全相同，判断条件也完全相同（`kIndex % stepKa == 0`）。

双层循环的真正价值在于：

1. **语义清晰**：`outerKaIdx` 和 `outerKbIdx` 显式表达了A/B矩阵在外层循环维度的独立进度，这在高阶API中用于 ClearL1BufferCache 的判断逻辑
2. **扩展性**：双层循环为未来添加 Preload（预取下一个L1 tile）等优化提供了自然的插入点
3. **与高阶API对齐**：保持代码结构一致，便于理解和维护

但在当前代码中，这些 `outerKaIdx`/`outerKbOuterIdx` 变量虽然被计算但未被直接使用（缓存命中仍通过 `kIndex % stepKa == 0` 判断），所以**双层循环目前仅是代码风格/语义上的改进，不影响执行逻辑**。

### 4.3 L1缓存释放差异（核心差异）

这是**修改前后的真正区别**。

高阶API中，`FreeTensor` 在 `ClearL1BufferCache` 中被调用，语义是"当前L1 buffer的数据已全部消费完毕，释放buffer允许下一次 CopyIn 覆盖"。对应到基础API的静态tensor模型，等价操作是在 DataLoad 消费完最后一个K块后执行 `SetFlag<MTE1_MTE2>`。

| 高阶API | 基础API等价 | 说明 |
|---------|------------|------|
| `AllocTensor(bufferPos)` | `LocalTensor<T>(pos, offset, size)` | 静态tensor预分配 |
| `EnQue(tensor)` | `SetFlag<MTE2_MTE1>` | 通知下游数据就绪 |
| `DeQue()` | `WaitFlag<MTE2_MTE1>` | 等待上游数据就绪 |
| `Hit(posL1, bufferPos)` | `kIndex % stepKa == 0` | 缓存命中判断 |
| `FreeTensor(bufferPos)` | `SetFlag<MTE1_MTE2>` | **释放buffer，允许覆盖** ← 修改前缺失 |

### 4.4 同步模型差异

高阶API使用 TQue（EnQue/DeQue），基础API使用静态tensor（SetFlag/WaitFlag）。两者在功能上等价，区别仅在于接口形式：

- TQue 是队列抽象，EnQue/DeQue 隐含了硬件事件同步
- SetFlag/WaitFlag 是底层原语，需要开发者手动管理 flag ID 和配对关系

基础API选择 SetFlag/WaitFlag 是因为静态 tensor 不支持 TQue 接口。

---

## 五、修改内容

### 5.1 修改1：InitSyncFlags 注释更新

**修改前**：
```cpp
__aicore__ inline void InitSyncFlags()
{
    // 初始化反向同步标志，防止第一次循环死锁
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(0);
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(1);
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(2);
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(3);
    AscendC::SetFlag<AscendC::HardEvent::M_MTE1>(0);
    AscendC::SetFlag<AscendC::HardEvent::M_MTE1>(1);
}
```

**修改后**：
```cpp
__aicore__ inline void InitSyncFlags()
{
    // 初始化反向同步标志，防止第一次循环死锁
    // A矩阵L1双缓冲使用flag 0,1
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(0);
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(1);
    // B矩阵L1双缓冲使用flag 2,3
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(2);
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(3);
    // L0双缓冲使用flag 0,1
    AscendC::SetFlag<AscendC::HardEvent::M_MTE1>(0);
    AscendC::SetFlag<AscendC::HardEvent::M_MTE1>(1);
}
```

**原因**：明确区分A矩阵（flag 0,1）和B矩阵（flag 2,3）的 MTE1_MTE2 同步标志，与 CopyInB 中 `WaitFlag<MTE1_MTE2>(mte2DBFlagB + 2)` 和 DataLoadB 后 `SetFlag<MTE1_MTE2>(currMte2IdxB + 2)` 的用法一致。

### 5.2 修改2：Process() 引入内外双层K循环

**修改前**：
```cpp
for (uint32_t kIndex = 0; kIndex < kLoopCount; kIndex++) {
    uint32_t kModStepKa = kIndex % stepKa;
    uint32_t kModStepKb = kIndex % stepKb;
    // ... CopyIn, DataLoad, Compute
}
```

**修改后**：
```cpp
// 参考高阶API KLoopMDLBase: 以 minStepK 为粒度的内外双层K循环
constexpr uint32_t minStepK = (stepKa < stepKb) ? stepKa : stepKb;
constexpr uint32_t kaStepFactor = stepKa / minStepK;
constexpr uint32_t kbStepFactor = stepKb / minStepK;
constexpr uint32_t outerKaIter = CeilDivision(singleCoreK, baseK * stepKa);
constexpr uint32_t outerKbIter = CeilDivision(singleCoreK, baseK * stepKb);
constexpr uint32_t outerKIter = (outerKaIter > outerKbIter) ? outerKaIter : outerKbIter;

for (uint32_t outerKIdx = 0; outerKIdx < outerKIter; outerKIdx++) {
    uint32_t innerStartIdx = outerKIdx * minStepK;
    uint32_t curKaOuterIdx = innerStartIdx / stepKa;
    uint32_t curKbOuterIdx = innerStartIdx / stepKb;
    uint32_t tileShapeA = stepKa * baseK;
    uint32_t tileShapeB = stepKb * baseK;
    constexpr uint32_t innerKIter = minStepK;

    for (uint32_t innerKOffset = 0; innerKOffset < innerKIter; innerKOffset++) {
        uint32_t kIndex = innerStartIdx + innerKOffset;
        if (kIndex >= kLoopCount) break;
        // ... CopyIn, DataLoad, Compute（同步逻辑不变）
    }
}
```

**注意**：双层循环产生的 kIndex 序列与线性循环完全相同，判断条件也相同。这个修改是**语义和代码结构上的改进**，不是功能修正。其价值在于与高阶API的 KLoopMDLBase 结构对齐，便于理解和维护。

### 5.3 修改3：补全缺失的 SetFlag<MTE1_MTE2>（核心修改）

**修改前**：DataLoadA/DataLoadB 之后**没有** `SetFlag<MTE1_MTE2>`，导致 CopyInA/CopyInB 中的 `WaitFlag<MTE1_MTE2>` 在第二次触发时永久阻塞。

**修改后**：参考高阶API的 `FreeTensor` 语义，在 DataLoad 消费完当前K块后，**仅在该K块是L1 buffer的最后一个消费者时**才释放：

```cpp
DataLoadA(a1LocalTmp, a2Local, mIndex, kIndex);
// A矩阵: buffer被stepKa个K块共享，仅在最后一个K块完成DataLoad后释放
if ((kIndex + 1) % stepKa == 0)
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(currMte2IdxA);

DataLoadB(b1LocalTmp, b2Local, nIndex, kIndex);
// B矩阵: buffer被stepKb个K块共享，仅在最后一个K块完成DataLoad后释放
if ((kIndex + 1) % stepKb == 0)
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(currMte2IdxB + 2);
```

**为什么只在最后一个K块才 SetFlag**：

一个 L1 buffer 被 stepKa 个 K 块共享（例如 stepKa=8 时，kIndex=0~7 共享 A1 Ping）。只有当最后一个 K 块（kIndex=7）的 DataLoad 完成后，该 buffer 中的数据才全部被消费完毕，此时才能安全释放，允许 CopyIn 覆盖。

如果每个 K 块都 SetFlag，会导致：
1. 过早释放：kIndex=0 的 DataLoad 后就释放，但 kIndex=1~7 还没从这个 buffer 读取
2. flag 计数错误：多次 SetFlag 同一个 flag ID，而 CopyIn 只 WaitFlag 一次，可能导致后续 WaitFlag 立即通过

---

## 六、修改原因详解

### 6.1 为什么要补全 SetFlag<MTE1_MTE2>

**修改前代码存在同步死锁缺陷**。

`MTE1_MTE2` 是反向同步事件，保证"消费完才能覆盖"。完整的同步链路为：

```
CopyInA:  WaitFlag<MTE1_MTE2>(mte2DBFlagA)  ← 我要写这个buffer，等你说消费完了
DataLoadA: SetFlag<MTE1_MTE2>(currMte2IdxA) ← 我消费完了，你可以写了
```

修改前只有 WaitFlag 没有 SetFlag（除 InitSyncFlags 的初始值），第二次 CopyInA 会因 WaitFlag 无法通过而永久阻塞。

**这等价于高阶API中缺少 FreeTensor**：如果高阶API不调用 FreeTensor，AllocTensor 会因 TQue 满而阻塞，效果相同。

### 6.2 为什么 SetFlag 要加条件判断

条件 `(kIndex + 1) % stepKa == 0` 的含义是"当前K块是该L1 buffer的最后一个消费者"。

这对应高阶API中 `ClearL1BufferCache` 的判断逻辑：
```cpp
int32_t curKaIdx = MATMUL_MODULE(KLoop)->GetNextOuterKaIdx();
if (curKaIdx != curKaOuterIdx) {
    MATMUL_MODULE(CopyCubeInA)->ClearLoadData();  // FreeTensor
}
```

高阶API在外层循环边界判断"下一个outerKaIdx与当前不同"时释放，等价于基础API中"当前K块是stepKa对齐的最后一个"时释放。

对于 stepKa=8：
- 高阶API：outerIdx=1结束时，nextOuterKaIdx = (1+1)/2 = 1 ≠ curKaOuterIdx=0，所以释放
- 基础API：kIndex=7时，(7+1)%8==0，所以释放

两者在时机上完全一致。

### 6.3 双层循环的意义

诚然，双层循环在当前代码中**不改变功能**。它的意义在于：

1. **语义对齐**：与高阶API KLoopMDLBase 的结构一致，便于代码审查者理解 A/B 异步切换的节奏
2. **扩展性**：为 Preload 等优化提供自然的插入点（外层循环末尾是发起预取的位置）
3. **辅助变量**：`curKaOuterIdx`/`curKbOuterIdx` 等变量虽然当前未直接使用，但为未来扩展（如尾部处理、动态 tileShape）预留了接口

### 6.4 为什么保持静态tensor模型

高阶API使用 TQue（EnQue/DeQue）模型，而基础API使用静态tensor（SetFlag/WaitFlag）模型。两者的映射关系：

| 高阶API | 基础API等价 | 说明 |
|---------|------------|------|
| `AllocTensor(bufferPos)` | `LocalTensor<T>(pos, offset, size)` | 静态tensor预分配，无需动态分配 |
| `EnQue(tensor)` | `SetFlag<MTE2_MTE1>` | 通知下游数据已就绪 |
| `DeQue()` | `WaitFlag<MTE2_MTE1>` | 等待上游数据就绪 |
| `Hit(posL1, bufferPos)` | `kIndex % stepKa == 0` | 缓存命中判断 |
| `FreeTensor(bufferPos)` | `SetFlag<MTE1_MTE2>` | 释放buffer，允许覆盖 |

基础API的静态tensor模型在功能上完全等价于TQue，只是同步接口不同。修改时保持静态tensor模型，仅补全了缺失的 `SetFlag<MTE1_MTE2>`。

---

## 七、修改后的执行流程（stepKa=8, stepKb=4）

### 7.1 外层循环0 (innerStartIdx=0)

| kIndex | kModStepKa | kModStepKb | CopyInA | CopyInB | DataLoadA SetFlag | DataLoadB SetFlag |
|--------|-----------|-----------|---------|---------|-------------------|-------------------|
| 0 | 0 (miss) | 0 (miss) | ✓ Ping | ✓ Ping | ✗ (0+1)%8≠0 | ✗ (0+1)%4≠0 |
| 1 | 1 | 1 | ✗ | ✗ | ✗ (1+1)%8≠0 | ✗ (1+1)%4≠0 |
| 2 | 2 | 2 | ✗ | ✗ | ✗ (2+1)%8≠0 | ✗ (2+1)%4≠0 |
| 3 | 3 | 3 | ✗ | ✗ | ✗ (3+1)%8≠0 | ✓ (3+1)%4==0 → SetFlag B(2) |

### 7.2 外层循环1 (innerStartIdx=4)

| kIndex | kModStepKa | kModStepKb | CopyInA | CopyInB | DataLoadA SetFlag | DataLoadB SetFlag |
|--------|-----------|-----------|---------|---------|-------------------|-------------------|
| 4 | 4 | 0 (miss) | ✗ | ✓ Pong | ✗ (4+1)%8≠0 | ✗ (4+1)%4≠0 |
| 5 | 5 | 1 | ✗ | ✗ | ✗ (5+1)%8≠0 | ✗ (5+1)%4≠0 |
| 6 | 6 | 2 | ✗ | ✗ | ✗ (6+1)%8≠0 | ✗ (6+1)%4≠0 |
| 7 | 7 | 3 | ✗ | ✗ | ✓ (7+1)%8==0 → SetFlag A(0) | ✓ (7+1)%4==0 → SetFlag B(3) |

### 7.3 外层循环2 (innerStartIdx=8)

| kIndex | kModStepKa | kModStepKb | CopyInA | CopyInB | DataLoadA SetFlag | DataLoadB SetFlag |
|--------|-----------|-----------|---------|---------|-------------------|-------------------|
| 8 | 0 (miss) | 0 (miss) | ✓ Pong | ✓ Ping | ✗ (8+1)%8≠0 | ✗ (8+1)%4≠0 |
| 9 | 1 | 1 | ✗ | ✗ | ✗ (9+1)%8≠0 | ✗ (9+1)%4≠0 |
| 10 | 2 | 2 | ✗ | ✗ | ✗ (10+1)%8≠0 | ✗ (10+1)%4≠0 |
| 11 | 3 | 3 | ✗ | ✗ | ✗ (11+1)%8≠0 | ✓ (11+1)%4==0 → SetFlag B(2) |

### 7.4 同步链路验证

以 A 矩阵为例，验证 SetFlag/WaitFlag 的完整配对：

```
InitSyncFlags: SetFlag<MTE1_MTE2>(0), SetFlag<MTE1_MTE2>(1)

kIndex=0, CopyInA: WaitFlag<MTE1_MTE2>(0) ← 消耗Init的flag(0)，通过
kIndex=7, DataLoadA: SetFlag<MTE1_MTE2>(0) ← 补充flag(0)
kIndex=8, CopyInA: WaitFlag<MTE1_MTE2>(1) ← 消耗Init的flag(1)，通过
kIndex=15, DataLoadA: SetFlag<MTE1_MTE2>(1) ← 补充flag(1)
kIndex=16, CopyInA: WaitFlag<MTE1_MTE2>(0) ← 消耗kIndex=7补充的flag(0)，通过
...
```

每次 CopyInA 的 WaitFlag 都有对应的 SetFlag 提供信号，不会死锁。

---

## 八、未实现的优化（未来工作）

| 优化项 | 高阶API实现 | 基础API可行性 |
|--------|------------|--------------|
| **Preload预取** | `AsyncLoadData` + `AwaitLoadData`，提前搬运下一个L1 tile | 可实现：在当前外层循环末尾发起下一次CopyIn |
| **CopyOut双缓冲** | L0C Ping/Pong交替写回，隐藏写回延迟 | 可实现：增加CO1 Ping/Pong和FIX_M同步 |
| **尾部处理** | `tailStepKa_`, `tailStepKb_`, `innerTailK_` 精确处理不整除情况 | 可实现：在外层循环末尾动态调整tileShape和innerIter |
| **L1缓存显式清除** | `ClearL1BufferCache` 在外层循环边界释放buffer | 静态tensor模型中通过同步保证已消费，无需额外操作 |

---

## 九、总结

### 9.1 修改的核心：补全缺失的 SetFlag<MTE1_MTE2>

修改前代码存在**同步死锁缺陷**：DataLoad 消费完 L1 buffer 后没有 `SetFlag<MTE1_MTE2>`，导致 CopyIn 中的 `WaitFlag<MTE1_MTE2>` 在第二次触发时永久阻塞。

修改后参考高阶API的 `FreeTensor` 语义，在 DataLoad 消费完当前K块后，**仅在该K块是L1 buffer的最后一个消费者时**（`(kIndex+1) % stepKa == 0`）执行 `SetFlag<MTE1_MTE2>`，补全了同步链路。

### 9.2 双层循环：语义改进，非功能修正

双层循环与线性循环在功能上完全等价（kIndex 序列和判断条件相同），其价值在于：
- 与高阶API KLoopMDLBase 结构对齐，提升代码可读性
- 为 Preload 等扩展优化提供自然的插入点

### 9.3 修改汇总

| 修改 | 类型 | 影响 |
|------|------|------|
| InitSyncFlags 注释更新 | 注释 | 无功能影响 |
| 双层K循环 | 代码重构 | 功能等价，语义改进 |
| **补全 SetFlag<MTE1_MTE2>** | **Bug修复** | **修复同步死锁，使代码可正确运行** |
