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

| 层级 | 正向同步 | 反向同步 |
|------|----------|----------|
| GM→L1 | `SetFlag<MTE2_MTE1>` / `WaitFlag<MTE2_MTE1>` | `WaitFlag<MTE1_MTE2>` / `SetFlag<MTE1_MTE2>` |
| L1→L0 | `SetFlag<MTE1_M>` / `WaitFlag<MTE1_M>` | `WaitFlag<M_MTE1>` / `SetFlag<M_MTE1>` |

### 3.3 CopyIn中的缓存命中判断

```cpp
__aicore__ inline void CopyInA(AscendC::LocalTensor<half> a1Local, uint32_t kIndex, uint32_t mIndex)
{
    const uint32_t kModStepKa = kIndex % stepKa;
    if (kModStepKa == 0) {
        // 缓存未命中：执行大包搬运
        WaitFlag<MTE1_MTE2>(mte2DBFlagA);  // 等待L0消费完
        DataCopy(a1Local, aGM[...], nd2nzA1Params);
        SetFlag<MTE2_MTE1>(0);              // 通知DataLoad数据就绪
        mte2DBFlagA = 1 - mte2DBFlagA;      // 切换Ping/Pong
    }
    // else: 缓存命中，跳过搬运
}
```

### 3.4 DataLoad中的K偏移计算

```cpp
// 修改前（简化版）
uint32_t srcAddr = kModStepKa * baseK * baseM;  // A矩阵L1偏移
```

### 3.5 L1缓存释放

修改前的基础API **没有显式的L1缓存释放机制**。`SetFlag<MTE1_MTE2>` 在每次DataLoad后都执行，而不是仅在最 后一个K块完成时执行。

---

## 四、差异对比

### 4.1 核心差异总览

| 维度 | 高阶API | 基础API（修改前） |
|------|---------|-------------------|
| **K循环结构** | 内外双层循环（outerIdx + innerIdx），以 minStepK 为粒度 | 简单线性循环（kIndex: 0 → kLoopCount） |
| **同步模型** | TQue（EnQue/DeQue/AllocTensor/FreeTensor） | 静态tensor（SetFlag/WaitFlag） |
| **L1缓存命中** | `Hit(posL1, bufferPos)` 显式判断 | `kIndex % stepKa == 0` 隐式判断 |
| **L1缓存释放** | `ClearL1BufferCache()` 在外层循环边界判断 | 无显式释放，MTE1_MTE2在每次循环SetFlag |
| **K偏移计算** | `kAxisL1Offset = (kInnerIdx % stepKa) * baseK` | 相同逻辑 `kModStepKa * baseK * baseM` |
| **外层K索引** | `GetOuterKaIdx() = outerIdx / kaStepFactor` | 无概念，直接用 kIndex |
| **A/B独立切换** | kaStepFactor/kbStepFactor 控制A/B不同步切换 | mte2DBFlagA/mte2DBFlagB 独立管理，但线性循环中缺乏外层语义 |

### 4.2 循环结构的根本差异

高阶API的双层循环天然表达了 **A/B矩阵L1 buffer异步切换** 的语义：

```
stepKa=8, stepKb=4, minStepK=4, kaStepFactor=2, kbStepFactor=1

外层循环0: innerStartIdx=0
  → A: outerKaIdx=0 (触发CopyInA), B: outerKbIdx=0 (触发CopyInB)
  内层: kIndex=0,1,2,3

外层循环1: innerStartIdx=4
  → A: outerKaIdx=0 (不触发CopyInA, 缓存命中), B: outerKbIdx=1 (触发CopyInB)
  内层: kIndex=4,5,6,7

外层循环2: innerStartIdx=8
  → A: outerKaIdx=1 (触发CopyInA), B: outerKbIdx=2 (触发CopyInB)
  内层: kIndex=8,9,10,11

外层循环3: innerStartIdx=12
  → A: outerKaIdx=1 (不触发CopyInA, 缓存命中), B: outerKbIdx=3 (触发CopyInB)
  内层: kIndex=12,13,14,15
```

**关键观察**：在外层循环1中，A矩阵缓存命中（outerKaIdx仍为0），B矩阵缓存未命中（outerKbIdx从0变为1）。这天然表达了"stepKa=8的A矩阵每2个外层循环才需要搬运一次，而stepKb=4的B矩阵每个外层循环都需要搬运"。

而基础API的线性循环 `kIndex=0,1,...,127`，虽然通过 `kIndex % stepKa == 0` 也能判断缓存命中，但缺乏外层语义，无法正确表达 **A/B在不同时间点切换L1 Ping/Pong buffer** 的协调关系。

### 4.3 L1缓存释放差异

高阶API的 `ClearL1BufferCache` 在外层循环边界判断是否需要释放：

```
outerIdx=1结束时: nextOuterKaIdx=0 (与curKaOuterIdx=0相同) → A不释放
                  nextOuterKbIdx=1 (与curKbOuterIdx=0不同) → B释放

outerIdx=2结束时: nextOuterKaIdx=1 (与curKaOuterIdx=0不同) → A释放
                  nextOuterKbIdx=2 (与curKbOuterIdx=1不同) → B释放
```

基础API中，MTE1_MTE2 的 SetFlag 机制需要在DataLoad完成时通知CopyIn可以覆盖buffer。修改前每个循环都执行 SetFlag，但更精确的做法是**仅在当前K块是某个buffer的最后一个K块时才释放**，避免过早释放导致数据被覆盖。

### 4.4 同步模型差异

| 高阶API (TQue) | 基础API (SetFlag/WaitFlag) | 说明 |
|-----------------|---------------------------|------|
| `AllocTensor(bufferPos)` | N/A（静态tensor预分配） | 基础API在Process()开头一次性分配所有buffer |
| `EnQue(tensor)` | `SetFlag<MTE2_MTE1>` | 通知DataLoad数据已就绪 |
| `DeQue()` | `WaitFlag<MTE2_MTE1>` | 等待DataCopy完成 |
| `Hit(posL1, bufferPos)` | `kIndex % stepKa == 0` | 判断是否需要搬运 |
| `FreeTensor(bufferPos)` | `SetFlag<MTE1_MTE2>` | 通知CopyIn可以覆盖buffer |

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

**原因**：明确区分A矩阵（flag 0,1）和B矩阵（flag 2,3）的MTE1_MTE2同步标志，与CopyInB中 `WaitFlag<MTE1_MTE2>(mte2DBFlagB + 2)` 和 DataLoadB后 `SetFlag<MTE1_MTE2>(currMte2IdxB + 2)` 的用法一致。

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

### 5.3 修改3：L1缓存释放优化

**修改前**：每个K循环的DataLoad后都执行 SetFlag<MTE1_MTE2>，不论当前K块是否是buffer的最后一个消费者。

**修改后**：参考高阶API的FreeTensor语义，仅在当前K块是某个L1 buffer的**最后一个K块**时才释放：

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

---

## 六、修改原因详解

### 6.1 为什么要引入双层K循环

**问题**：线性循环 `kIndex=0,1,...,127` 无法自然表达 stepKa≠stepKb 时 A/B 矩阵 L1 buffer 的异步切换节奏。

以 stepKa=8, stepKb=4 为例：

| kIndex范围 | A矩阵L1状态 | B矩阵L1状态 |
|-----------|-------------|-------------|
| 0-3 | 第1次CopyIn（Ping） | 第1次CopyIn（Ping） |
| 4-7 | **缓存命中**（Ping） | 第2次CopyIn（Pong） |
| 8-11 | 第2次CopyIn（Pong） | 第3次CopyIn（Ping） |
| 12-15 | **缓存命中**（Pong） | 第4次CopyIn（Pong） |

线性循环中，kIndex=4时A缓存命中但B需要搬运，kIndex=8时两者都需要搬运。这种不同的节奏在双层循环中自然体现：

- **外层循环0** (innerStartIdx=0)：A=miss, B=miss → 两者都CopyIn
- **外层循环1** (innerStartIdx=4)：A=hit(outerKaIdx仍=0), B=miss(outerKbIdx变为1) → 仅B CopyIn
- **外层循环2** (innerStartIdx=8)：A=miss(outerKaIdx变为1), B=miss → 两者都CopyIn
- **外层循环3** (innerStartIdx=12)：A=hit(outerKaIdx仍=1), B=miss → 仅B CopyIn

双层循环的 `kaStepFactor` 和 `kbStepFactor` 天然编码了这个节奏。

### 6.2 为什么要优化L1缓存释放

**问题**：修改前每个K循环的DataLoad后都执行 `SetFlag<MTE1_MTE2>`，这意味着CopyIn在下一个 `kIndex % stepKa == 0` 时 WaitFlag 会立即通过，可能导致 **L1 buffer中的数据尚未被所有K块消费完就被覆盖**。

例如，当 stepKa=8 时，A矩阵的一个L1 buffer被 kIndex=0~7 共8个K块共享。如果 kIndex=0 的 DataLoad 完成后就 SetFlag，那么 kIndex=8 的 CopyIn 会 WaitFlag 通过后立即写入同一个buffer，而此时 kIndex=1~7 的 DataLoad 尚未从这个buffer读取数据。

**修改后**：仅在 `(kIndex + 1) % stepKa == 0` 时 SetFlag，即最后一个K块消费完毕后才释放buffer。这对应高阶API中 `ClearL1BufferCache` 仅在 `nextOuterKaIdx != curKaOuterIdx` 时调用 `FreeTensor` 的语义。

### 6.3 为什么保持静态tensor模型

高阶API使用 TQue（EnQue/DeQue）模型，而基础API使用静态tensor（SetFlag/WaitFlag）模型。两者的映射关系：

| 高阶API | 基础API等价 | 说明 |
|---------|------------|------|
| `AllocTensor(bufferPos)` | `LocalTensor<T>(pos, offset, size)` | 静态tensor预分配，无需动态分配 |
| `EnQue(tensor)` | `SetFlag<MTE2_MTE1>` | 通知下游数据已就绪 |
| `DeQue()` | `WaitFlag<MTE2_MTE1>` | 等待上游数据就绪 |
| `Hit(posL1, bufferPos)` | `kIndex % stepKa == 0` | 缓存命中判断 |
| `FreeTensor(bufferPos)` | `SetFlag<MTE1_MTE2>` | 释放buffer，允许覆盖 |

基础API的静态tensor模型在功能上完全等价于TQue，只是同步接口不同。修改时保持静态tensor模型，仅调整循环结构和缓存释放时机。

---

## 七、修改后的执行流程（stepKa=8, stepKb=4）

### 7.1 外层循环0 (innerStartIdx=0)

| kIndex | kModStepKa | kModStepKb | CopyInA | CopyInB | DataLoadA SetFlag | DataLoadB SetFlag |
|--------|-----------|-----------|---------|---------|-------------------|-------------------|
| 0 | 0 (miss) | 0 (miss) | ✓ Ping | ✓ Ping | ✗ (0+1)%8≠0 | ✗ (0+1)%4≠0 |
| 1 | 1 | 1 | ✗ | ✗ | ✗ (1+1)%8≠0 | ✗ (1+1)%4≠0 |
| 2 | 2 | 2 | ✗ | ✗ | ✗ (2+1)%8≠0 | ✗ (2+1)%4≠0 |
| 3 | 3 | 3 | ✗ | ✗ | ✗ (3+1)%8≠0 | ✓ (3+1)%4==0 → SetFlag B |

### 7.2 外层循环1 (innerStartIdx=4)

| kIndex | kModStepKa | kModStepKb | CopyInA | CopyInB | DataLoadA SetFlag | DataLoadB SetFlag |
|--------|-----------|-----------|---------|---------|-------------------|-------------------|
| 4 | 4 | 0 (miss) | ✗ | ✓ Pong | ✗ (4+1)%8≠0 | ✗ (4+1)%4≠0 |
| 5 | 5 | 1 | ✗ | ✗ | ✗ (5+1)%8≠0 | ✗ (5+1)%4≠0 |
| 6 | 6 | 2 | ✗ | ✗ | ✗ (6+1)%8≠0 | ✗ (6+1)%4≠0 |
| 7 | 7 | 3 | ✗ | ✗ | ✓ (7+1)%8==0 → SetFlag A | ✓ (7+1)%4==0 → SetFlag B |

### 7.3 外层循环2 (innerStartIdx=8)

| kIndex | kModStepKa | kModStepKb | CopyInA | CopyInB | DataLoadA SetFlag | DataLoadB SetFlag |
|--------|-----------|-----------|---------|---------|-------------------|-------------------|
| 8 | 0 (miss) | 0 (miss) | ✓ Pong | ✓ Ping | ✗ (8+1)%8≠0 | ✗ (8+1)%4≠0 |
| 9 | 1 | 1 | ✗ | ✗ | ✗ (9+1)%8≠0 | ✗ (9+1)%4≠0 |
| 10 | 2 | 2 | ✗ | ✗ | ✗ (10+1)%8≠0 | ✗ (10+1)%4≠0 |
| 11 | 3 | 3 | ✗ | ✗ | ✗ (11+1)%8≠0 | ✓ (11+1)%4==0 → SetFlag B |

### 7.4 关键观察

1. **A矩阵每8个K块才释放一次L1 buffer**（kIndex=7, 15, 23...），对应 stepKa=8
2. **B矩阵每4个K块释放一次L1 buffer**（kIndex=3, 7, 11...），对应 stepKb=4
3. **CopyInA每8个K块触发一次**（kIndex=0, 8, 16...），且在释放后才能写入
4. **CopyInB每4个K块触发一次**（kIndex=0, 4, 8...），且在释放后才能写入

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

本次修改的核心思想是**参考高阶API的K轴双层循环结构，使基础API的MDL实现能正确协调 stepKa≠stepKb 时 A/B 矩阵的 L1 缓存节奏**。关键修改点：

1. **循环重构**：线性K循环 → 内外双层K循环（以 minStepK 为粒度）
2. **缓存释放优化**：每次循环释放 → 仅在最后一个K块消费完毕时释放
3. **保持同步模型**：坚持使用 SetFlag/WaitFlag，不引入 EnQue/DeQue

这些修改使基础API的MDL实现语义与高阶API对齐，同时保持了静态tensor模型的简洁性。
