# AscendC DataCopy 优化手段详解

## 1. 背景与概述

在昇腾AI处理器上，数据搬运（DataCopy）是算子开发中最频繁且最关键的操作之一。数据在Global Memory（GM）与Local Memory（UB）之间搬运的效率，直接影响算子的整体性能。AscendC提供了多种搬运API及优化模式，帮助开发者充分利用硬件带宽、减少指令开销和无效数据搬运。

本指南结合 `optimize_datacopy` 文件夹中的三个优化样例，深入解析三种核心优化手段：

| 样例 | 优化手段 | 核心思想 |
|------|---------|---------|
| `optimize_reduce_invalid_datacopy` | Compact模式减少无效数据搬运 | 非对齐场景下紧凑排列，减少padding浪费 |
| `optimize_datacopy_loop_mode` | Loop模式减少搬运指令条数 | 二维非连续数据一次指令完成搬运 |
| `optimize_datacopy_nddma` | NDDMA多维搬运减少指令条数 | 多维度自由配置stride，一次完成复杂搬运 |

---

## 2. 前置知识：AscendC数据搬运基础

### 2.1 存储层次

昇腾AI处理器的存储层次如下：

```
Global Memory (GM)  ──  HBM，大容量，高延迟
       ↕ DMA搬运
Local Memory (UB)   ──  片上缓存，小容量，低延迟
```

数据在GM与UB之间搬运时，需要满足**32字节对齐**的约束（即一个DataBlock的大小为32字节）。

### 2.2 核心搬运API

| API | 功能 | 特点 |
|-----|------|------|
| `DataCopy` | 基础数据搬运 | 支持32B对齐的连续/定间隔搬运 |
| `DataCopyPad` | 非对齐数据搬运 | 支持Normal/Compact两种搬运模式，支持Padding填充 |
| `DataCopy`（NDDMA） | 多维数据搬运 | 支持自由配置1~5维的stride和padding |

### 2.3 通用优化原则

根据昇腾官方最佳实践：

1. **尽量一次搬运较大的数据块**：单次搬运数据长度16KB以上时，通常能较好地发挥带宽的最佳性能。
2. **GM地址尽量512B对齐**：512B对齐场景的带宽效率远高于32B对齐场景，差距最大可达30%。
3. **高效使用搬运API**：通过配置搬运控制参数实现连续搬运或固定间隔搬运，避免使用for循环。

> 参考文档：[尽量一次搬运较大的数据块](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0013.html)、[GM地址尽量512B对齐](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0014.html)、[高效的使用搬运API](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0015.html)

---

## 3. 优化手段一：Compact模式减少无效数据搬运

### 3.1 问题场景

当需要搬运的数据块大小**不是32字节的整数倍**时，硬件要求搬运到UB的数据必须32B对齐，因此需要在每个数据块末尾补充padding数据使其对齐。

假设需要搬运**3个48字节（12个float）**的数据块。48字节不是32字节的整数倍（48 = 32 + 16），需要补充16字节的padding才能达到64字节（2个DataBlock）的对齐。

### 3.2 Normal模式 vs Compact模式

#### Normal模式（反例）

Normal模式下，**每个数据块独立补齐到32B对齐**：

```
数据块0: [有效数据48B] [padding 16B]  → 64B
数据块1: [有效数据48B] [padding 16B]  → 64B
数据块2: [有效数据48B] [padding 16B]  → 64B
                               总计: 192B（其中有效144B，无效48B）
```

UB中的数据排列：

```
[1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,   ← 数据块0 + padding
 1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0,   ← 数据块1 + padding
 1,1,1,1,1,1,1,1,1,1,1,1, 0,0,0,0]   ← 数据块2 + padding
```

#### Compact模式（正例）

Compact模式下，**所有数据块紧凑排列，仅在整块数据末尾统一补齐一次**：

```
[有效数据48B] [有效数据48B] [有效数据48B] [padding 16B]
                               总计: 160B（其中有效144B，无效16B）
```

UB中的数据排列：

```
[1,1,1,1,1,1,1,1,1,1,1,1,   ← 数据块0
 1,1,1,1,1,1,1,1,1,1,1,1,   ← 数据块1
 1,1,1,1,1,1,1,1,1,1,1,1,   ← 数据块2
 0,0,0,0]                     ← 末尾统一padding
```

**对比：Normal模式搬运192B，Compact模式搬运160B，节省了32B（即17%）的带宽。**

### 3.3 样例代码解析

```cpp
// optimize_reduce_invalid_datacopy 样例 CopyIn函数
__aicore__ inline void CopyIn()
{
    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::Duplicate<T>(xLocal, 0, count);  // 先将UB清零

    AscendC::DataCopyParams dataCopyParams;
    dataCopyParams.blockCount = 3;   // 3个连续数据块
    dataCopyParams.blockLen = 48;    // 每块48字节
    dataCopyParams.srcStride = 0;    // GM上数据块之间无间隔
    dataCopyParams.dstStride = 0;    // UB上数据块之间无间隔

    AscendC::DataCopyPadParams dataCopyPadParams;
    dataCopyPadParams.isPad = 1;         // 需要填充padding
    dataCopyPadParams.leftPadding = 0;   // 左侧无padding
    dataCopyPadParams.rightPadding = 4;  // 右侧padding 4个元素(16字节)
    dataCopyPadParams.paddingValue = 0;  // padding值为0

    // 关键：使用 Compact 模式
    AscendC::DataCopyPad<T, AscendC::PaddingMode::Compact>(
        xLocal, xGm, dataCopyParams, dataCopyPadParams);
    inQueueX.EnQue<T>(xLocal);
}
```

**参数详解：**

| 参数 | 值 | 含义 |
|------|-----|------|
| `blockCount` | 3 | 搬运3个数据块 |
| `blockLen` | 48 | 每块48字节（12个float） |
| `srcStride` | 0 | GM上数据块之间紧密排列 |
| `dstStride` | 0 | UB上数据块之间紧密排列 |
| `isPad` | 1 | 需要填充 |
| `rightPadding` | 4 | 右侧补4个float（16字节），使48+16=64B对齐 |
| `PaddingMode::Compact` | - | 紧凑模式，所有块连续排列，仅在末尾补齐 |

### 3.4 为什么能优化

- **Normal模式**：每个数据块独立补齐到32B对齐，3个块产生3次padding（3 × 16B = 48B无效数据）
- **Compact模式**：多个数据块紧凑排列后统一补齐，仅产生1次padding（16B无效数据）
- **带宽节省**：从搬运192B减少到160B，节省17%的搬运数据量

> 参考文档：[非对齐场景减少无效数据的搬运](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00017.html)、[DataCopyPad API](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00131.html)

---

## 4. 优化手段二：Loop模式减少搬运指令条数

### 4.1 问题场景

当需要搬运**二维非连续数据**时，即数据块在源和目的地址上存在不同间隔的排列，如果使用普通的DataCopyPad接口，需要多次调用搬运指令来处理不同位置的数据块。

假设需要搬运8个48字节的数据块，它们在GM和UB上的排列存在规律性的间隔：

```
GM源数据排列（每行24个float=96B）：
Row0: [块0: 12个float] [间隔12个float] [块1: 12个float] [间隔12个float]
Row1: [块2: 12个float] [间隔12个float] [块3: 12个float] [间隔12个float]
Row2: [块4: 12个float] [间隔12个float] [块5: 12个float] [间隔12个float]
Row3: [块6: 12个float] [间隔12个float] [块7: 12个float] [间隔12个float]

UB目的排列（每行32个float=128B）：
Row0: [块0: 12个float] [间隔20个float] [块1: 12个float] [间隔20个float]
Row1: [块2: 12个float] [间隔20个float] [块3: 12个float] [间隔20个float]
...
```

### 4.2 不使用Loop模式（反例）

需要调用4次DataCopyPad指令：

```cpp
// 第1次：搬运块0、块1
DataCopyPad<T, PaddingMode::Compact>(xLocal, xGm, dataCopyParams, dataCopyPadParams);
// 第2次：搬运块2、块3（偏移24个float）
DataCopyPad<T, PaddingMode::Compact>(xLocal[32], xGm[24], dataCopyParams, dataCopyPadParams);
// 第3次：搬运块4、块5（偏移72个float = 24+48）
DataCopyPad<T, PaddingMode::Compact>(xLocal[72], xGm[48], dataCopyParams, dataCopyPadParams);
// 第4次：搬运块6、块7（偏移104个float = 72+32）
DataCopyPad<T, PaddingMode::Compact>(xLocal[104], xGm[72], dataCopyParams, dataCopyPadParams);
```

4次指令，每次搬运2个块，指令开销大。

### 4.3 使用Loop模式（正例）

通过 `SetLoopModePara` 设置二层循环参数，**1条指令完成全部8个块的搬运**：

```cpp
// optimize_datacopy_loop_mode 样例 CopyIn函数
__aicore__ inline void CopyIn()
{
    // 1. 配置Loop模式参数
    AscendC::LoopModeParams loopModeParams;
    loopModeParams.loop1Size = 2;          // 内层循环次数：2（每行2个块）
    loopModeParams.loop2Size = 2;          // 外层循环次数：2（共2组行）
    loopModeParams.loop1SrcStride = 96;    // 内层循环源间隔：96字节（24个float）
    loopModeParams.loop1DstStride = 128;   // 内层循环目的间隔：128字节（32个float）
    loopModeParams.loop2SrcStride = 192;   // 外层循环源间隔：192字节（48个float）
    loopModeParams.loop2DstStride = 288;   // 外层循环目的间隔：288字节（72个float）

    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::Duplicate<T>(xLocal, 0, count);

    // 2. 配置DataCopyPad基础参数
    AscendC::DataCopyParams dataCopyParams;
    dataCopyParams.blockCount = 2;   // 每次循环搬运2个连续块
    dataCopyParams.blockLen = 48;    // 每块48字节
    dataCopyParams.srcStride = 0;    // 连续块之间无间隔
    dataCopyParams.dstStride = 0;    // 连续块之间无间隔

    AscendC::DataCopyPadParams dataCopyPadParams;
    dataCopyPadParams.isPad = 0;
    dataCopyPadParams.leftPadding = 0;
    dataCopyPadParams.rightPadding = 0;
    dataCopyPadParams.paddingValue = 0;

    // 3. 使能Loop模式并执行搬运
    AscendC::SetLoopModePara(loopModeParams, AscendC::DataCopyMVType::OUT_TO_UB);
    AscendC::DataCopyPad<T, AscendC::PaddingMode::Compact>(
        xLocal, xGm, dataCopyParams, dataCopyPadParams);
    AscendC::ResetLoopModePara(AscendC::DataCopyMVType::OUT_TO_UB);

    inQueueX.EnQue<T>(xLocal);
}
```

### 4.4 Loop模式参数详解

Loop模式通过二层循环描述数据的搬运规律：

```
DataCopyPad基础搬运: blockCount=2, blockLen=48B
    ↓ 这构成了一个"基础搬运单元"（2个连续48B块 = 96B）

内层循环 (loop1): 循环2次，每次间隔 loop1SrcStride/loop1DstStride
    → 第1次: 基础搬运
    → 第2次: 偏移loop1SrcStride(GM)/loop1DstStride(UB)后再搬运

外层循环 (loop2): 循环2次，每次间隔 loop2SrcStride/loop2DstStride
    → 第1组: 内层循环完整执行一遍
    → 第2组: 偏移loop2SrcStride(GM)/loop2DstStride(UB)后再执行内层循环
```

具体到本样例：

```
GM侧：
  块0(0-47B) 块1(0-47B)  --间隔96B--  块2(96-143B) 块3(96-143B)   ← loop1循环
       |
  ----间隔192B----
       |
  块4(192-239B) 块5(192-239B) --间隔96B-- 块6(288-335B) 块7(288-335B) ← loop2+loop1

UB侧：
  块0 块1  --间隔128B--  块2 块3   ← loop1循环
       |
  ----间隔288B----
       |
  块4 块5  --间隔128B--  块6 块7   ← loop2+loop1
```

| 参数 | 值 | 含义 |
|------|-----|------|
| `loop1Size` | 2 | 内层循环2次（每行搬运2组块） |
| `loop2Size` | 2 | 外层循环2次（共搬运2行） |
| `loop1SrcStride` | 96 | 内层循环GM间隔96B（24个float × 4B） |
| `loop1DstStride` | 128 | 内层循环UB间隔128B（32个float × 4B） |
| `loop2SrcStride` | 192 | 外层循环GM间隔192B（48个float × 4B） |
| `loop2DstStride` | 288 | 外层循环UB间隔288B（72个float × 4B） |

### 4.5 为什么能优化

- **减少指令条数**：从4条DataCopyPad指令减少到1条，减少了指令发射和同步开销
- **硬件自动迭代**：Loop模式的循环迭代由DMA硬件自动完成，无需CPU参与
- **注意事项**：使用完Loop模式后，必须调用 `ResetLoopModePara` 重置寄存器，否则会影响后续搬运操作

> 参考文档：[非连续搬运场景减少搬运次数-Loop模式](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00018.html)、[SetLoopModePara API](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00242.html)

---

## 5. 优化手段三：NDDMA多维搬运减少指令条数

### 5.1 问题场景

NDDMA（N-Dimensional DMA）多维数据搬运接口适用于需要**多维度自由配置stride和padding**的场景。与Loop模式相比，NDDMA支持更灵活的1~5维搬运配置，可以处理更复杂的数据排布。

假设需要搬运2个32字节（8个float）的数据块，GM上的数据排布和UB上的目标排布存在多维度的间隔差异：

```
GM源数据（2D排列，shape=[2,8]）：
Row0: [d0 d1 d2 d3 d4 d5 d6 d7]    ← 8个float
Row1: [d8 d9 d10 d11 d12 d13 d14 d15]  ← 8个float

UB目的数据：
[d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 d10 d11 d12 d13 d14 d15]  ← 紧凑排列
```

### 5.2 不使用NDDMA（反例）

需要使用多次DataCopyPad指令分别处理不同维度的数据：

```cpp
// 需要多次搬运，每次处理一个维度的数据块
DataCopyPad<T, PaddingMode::Normal>(xLocal, xGm, dataCopyParams1, padParams1);
DataCopyPad<T, PaddingMode::Normal>(xLocal[offset], xGm[offset], dataCopyParams2, padParams2);
```

### 5.3 使用NDDMA（正例）

```cpp
// optimize_datacopy_nddma 样例 CopyIn函数
__aicore__ inline void CopyIn()
{
    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::Duplicate<T>(xLocal, 0, count);

    // 1. 配置2维搬运参数
    AscendC::NdDmaLoopInfo<2> loopInfo{
        {1, 32},    // loopSize: 每个维度的元素个数 [dim0=1, dim1=32]
        {1, 32},    // loopDstStride: 目的每维度stride [dim0=1, dim1=32]
        {32, 32},   // loopSrcStride: 源每维度stride [dim0=32, dim1=32]
        {0, 0},     // loopLpSize: 每维度左侧padding [dim0=0, dim1=0]
        {0, 0}      // loopRpSize: 每维度右侧padding [dim0=0, dim1=0]
    };
    AscendC::NdDmaParams<T, 2> params = {loopInfo, 0};  // constantValue=0

    // 2. 刷新NDDMA Cache
    AscendC::NdDmaDci();

    // 3. 配置搬运选项
    static constexpr AscendC::NdDmaConfig config = {false};

    // 4. 执行多维搬运
    AscendC::DataCopy<T, 2, config>(xLocal, xGm, params);

    inQueueX.EnQue<T>(xLocal);
}
```

### 5.4 NDDMA参数详解

`NdDmaLoopInfo<dim>` 的各参数含义（以2D为例）：

| 参数 | 含义 | 本样例值 | 说明 |
|------|------|---------|------|
| `loopSize[dim]` | 每个维度处理的元素个数 | {1, 32} | dim0搬运1个元素（行方向），dim1搬运32个元素（列方向） |
| `loopDstStride[dim]` | 目的每维stride | {1, 32} | 目的dim0方向stride=1元素，dim1方向stride=32元素 |
| `loopSrcStride[dim]` | 源每维stride | {32, 32} | 源dim0方向stride=32元素（一行），dim1方向stride=32元素 |
| `loopLpSize[dim]` | 左侧padding | {0, 0} | 无左侧padding |
| `loopRpSize[dim]` | 右侧padding | {0, 0} | 无右侧padding |

**维度索引说明**：数组索引0对应第1维，索引1对应第2维，依此类推。

NDDMA的数据搬运逻辑：

```
for i in range(loopSize[0]):          // dim0循环
    for j in range(loopSize[1]):      // dim1循环
        dst[dst_base + i*loopDstStride[0] + j*loopDstStride[1]] 
        = src[src_base + i*loopSrcStride[0] + j*loopSrcStride[1]]
```

### 5.5 NDDMA的典型应用场景

NDDMA不仅支持简单的2D搬运，还支持以下场景：

| 场景 | 说明 |
|------|------|
| **2D Padding** | 在每个维度上配置左右padding，自动填充常数或最近值 |
| **2D Transpose** | 通过配置srcStride和dstStride实现矩阵转置 |
| **2D BroadCast** | 将某一维度的数据广播到更大的空间 |
| **2D Slice** | 从大矩阵中提取子矩阵 |

### 5.6 为什么能优化

- **一条指令完成多维搬运**：将多次搬运合并为一条NDDMA指令，减少指令开销
- **灵活的维度配置**：支持1~5维的自由配置，适应各种数据排布
- **内置Padding支持**：无需额外指令处理padding
- **Cache一致性**：使用 `NdDmaDci()` 刷新NDDMA Cache，保证多核场景数据一致性

### 5.7 注意事项

1. NDDMA的Cache大小为32KB，搬运前必须调用 `NdDmaDci()` 刷新缓存
2. 一条指令所能获取的所有数据的地址范围宽度不能超过40位（1TB）
3. 当每层循环的dstStride为升序序列时，不同循环间的地址空间不能交织或重叠

> 参考文档：[非连续搬运场景减少搬运次数-NDDMA](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00018.html)、[多维数据搬运ISASI API](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0265.html)

---

## 6. 三种优化手段对比与选型

### 6.1 对比总结

| 优化手段 | 解决的问题 | 适用场景 | 指令条数优化 | 带宽优化 |
|---------|-----------|---------|------------|---------|
| Compact模式 | 非对齐搬运中padding浪费带宽 | 多块非32B对齐数据搬运 | 不变 | 减少无效数据搬运 |
| Loop模式 | 二维非连续搬运指令过多 | 数据块之间存在二层规律性间隔 | 多条→1条 | 不变 |
| NDDMA多维搬运 | 复杂多维搬运指令过多 | 1~5维自由配置stride/padding | 多条→1条 | 可配合padding减少无效搬运 |

### 6.2 选型建议

```
数据是否32B对齐？
├── 否 → 使用 DataCopyPad + Compact模式（减少无效搬运）
│         数据块间是否有规律间隔？
│         ├── 是，且是二维规律 → 使用 Loop模式（减少指令条数）
│         └── 是，且是多维规律 → 使用 NDDMA多维搬运（减少指令条数）
└── 是 → 使用基础 DataCopy
          数据块间是否有间隔？
          ├── 是，固定间隔 → 配置srcStride/dstStride一次搬运
          └── 否，连续数据 → 最简调用
```

### 6.3 组合使用

三种优化手段可以组合使用：

- **Compact + Loop**：Loop模式样例中同时使用了 `PaddingMode::Compact`，既减少了指令条数，又减少了无效数据搬运
- **Compact + NDDMA**：NDDMA本身支持per-dimension padding，可以实现类似Compact的紧凑效果

---

## 7. 参考文档

| 序号 | 文档名称 | 链接 |
|------|---------|------|
| 1 | CANN社区版9.0.0-beta.2 文档首页 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/index/index.html) |
| 2 | 尽量一次搬运较大的数据块 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0013.html) |
| 3 | GM地址尽量512B对齐 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0014.html) |
| 4 | 高效的使用搬运API | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0015.html) |
| 5 | 非对齐场景减少无效数据的搬运 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00017.html) |
| 6 | 非连续搬运场景减少搬运次数 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_00018.html) |
| 7 | DataCopyPad API (ISASI) | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00131.html) |
| 8 | 多维数据搬运 ISASI API | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0265.html) |
| 9 | SetLoopModePara API | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00242.html) |
