# __mix__ 融合算子开发踩坑指南

## 1. `GetBlockIdx()` 在 AIC/AIV 侧独立编号问题

### 1.1 问题描述

在 `__mix__(1, 2)` 模式下，使用 `GetBlockIdx()` 计算多核数据偏移时，AIC 侧和 AIV 侧的返回值属于**各自独立的编号空间**，直接使用会导致 AIV 侧计算出错误的 GM 地址偏移，最终输出数据写入错误位置。

### 1.2 根因分析

`__mix__(1, 2)` 表示每个逻辑 AI Core 由 **1 个 AIC + 2 个 AIV** 组成。关键点在于：**AIC 侧和 AIV 侧的 `GetBlockIdx()` 返回值是各自独立编号的**。

- **AIC 侧**：`GetBlockIdx()` 返回 AIC 自己的核编号，范围为 `0 ~ numAICores - 1`
- **AIV 侧**：`GetBlockIdx()` 返回 AIV 自己的核编号，范围为 `0 ~ numAIVCores - 1`

以 `numBlocks=1`（1 个逻辑核）为例：

| 子核 | `GetBlockIdx()` 返回值 | 说明 |
|------|------------------------|------|
| AIC0 | **0** | AIC 的第 0 个核 |
| AIV0 | **0** | AIV 的第 0 个核 |
| AIV1 | **1** | AIV 的第 1 个核 |

以 `numBlocks=2`（2 个逻辑核）为例：

| 子核 | `GetBlockIdx()` 返回值 | 所属逻辑核 |
|------|------------------------|-----------|
| AIC0 | **0** | 逻辑核 0 |
| AIC1 | **1** | 逻辑核 1 |
| AIV0 | **0** | 逻辑核 0 |
| AIV1 | **1** | 逻辑核 0 |
| AIV2 | **2** | 逻辑核 1 |
| AIV3 | **3** | 逻辑核 1 |

可以看到：**AIV0 和 AIV1 属于逻辑核 0，但 `GetBlockIdx()` 返回 0 和 1；AIC0 也属于逻辑核 0，但 `GetBlockIdx()` 返回 0**。两者的编号空间完全独立，不能混用。

### 1.3 问题复现

以下代码在非 mix 模式下工作正常，但在 `__mix__(1,2)` 模式下会导致 AIV 侧 GM 地址错误：

```cpp
// ❌ 错误用法：直接用GetBlockIdx()计算多核偏移，AIV侧编号与AIC侧不同
__aicore__ inline void InitComputeParams()
{
    constexpr uint32_t mIter = DivCeil(M, singleCoreM);  // = 1
    uint32_t mIterIdx = GetBlockIdx() % mIter;   // AIC0:0, AIV0:0, AIV1:0 (碰巧相同)
    uint32_t nIterIdx = GetBlockIdx() / mIter;   // AIC0:0, AIV0:0, AIV1:1 ← AIV1错误！

    uint64_t gmOffsetC = mIterIdx * singleCoreM * N + nIterIdx * singleCoreN;
    // AIC0:  gmOffsetC = 0          ✅
    // AIV0:  gmOffsetC = 0          ✅ (碰巧正确)
    // AIV1:  gmOffsetC = 256        ❌ 应该也是0，但nIterIdx=1导致偏移错误
    cGM = cGMOri[gmOffsetC];
}
```

**实际效果**：AIV1 的 `cGM` 指向了错误的 GM 地址（偏移了 256 个 float），DataCopy 将数据写入错误位置。而正确位置（与 AIV0 同一个逻辑核的输出区域）的后半部分未被写入，最终输出结果部分或全部错误。

多核场景下问题更严重——AIV0 也可能出错，因为 AIV 的 `GetBlockIdx()` 返回的是 AIV 全局编号，而不是逻辑核编号。

### 1.4 解决方案

**核心思路**：AIC 侧的 `GetBlockIdx()` 天然等于逻辑核 ID（因为 1 个逻辑核对应 1 个 AIC）；AIV 侧需要将 `GetBlockIdx()` 除以每个逻辑核对应的 AIV 数量（`__mix__(1,2)` 下为 2），才能得到逻辑核 ID。

**方案一（推荐）：AIC/AIV 分支分别计算逻辑核 ID**

```cpp
// ✅ 正确用法：AIC侧直接用GetBlockIdx()，AIV侧用GetBlockIdx()/2
__aicore__ inline void InitComputeParams()
{
    constexpr uint32_t mIter = DivCeil(M, singleCoreM);
    uint32_t logicCoreId;
    if ASCEND_IS_AIC {
        logicCoreId = GetBlockIdx();                    // AIC: 1核1AIC，直接就是逻辑核ID
    } else {
        logicCoreId = GetBlockIdx() / 2;               // __mix__(1,2): 每个逻辑核对应2个AIV
    }
    uint32_t mIterIdx = logicCoreId % mIter;
    uint32_t nIterIdx = logicCoreId / mIter;
    // ...
}
```

单核场景验证（`numBlocks=1`）：

| 子核 | `GetBlockIdx()` | `logicCoreId` | `nIterIdx` |
|------|-----------------|---------------|------------|
| AIC0 | 0 | 0（直接用） | 0 ✅ |
| AIV0 | 0 | 0/2=0 | 0 ✅ |
| AIV1 | 1 | 1/2=0 | 0 ✅ |

双核场景验证（`numBlocks=2`）：

| 子核 | `GetBlockIdx()` | `logicCoreId` | `nIterIdx` |
|------|-----------------|---------------|------------|
| AIC0 | 0 | 0 | 0 ✅ |
| AIC1 | 1 | 1 | 1 ✅ |
| AIV0 | 0 | 0/2=0 | 0 ✅ |
| AIV1 | 1 | 1/2=0 | 0 ✅ |
| AIV2 | 2 | 2/2=1 | 1 ✅ |
| AIV3 | 3 | 3/2=1 | 1 ✅ |

**方案二：AIC 侧独占多核偏移计算，AIV 侧不参与**

将 `InitComputeParams()` 中的多核偏移计算移到 `if ASCEND_IS_AIC` 分支中，AIV 侧在 `if ASCEND_IS_AIV` 分支中单独设置 `cGM`（如直接指向输出矩阵起始地址，偏移由 AIC 通过 CrossCore 通信或其他机制传递）。

### 1.5 通用公式

对于 `__mix__(a, b)` 模式（a 个 AIC + b 个 AIV）：

- AIC 侧：`logicCoreId = GetBlockIdx()`
- AIV 侧：`logicCoreId = GetBlockIdx() / b`

### 1.6 参考文档

> 当启用 KERNEL_TYPE_MIX_VECTOR_CORE 时，算子会同时运行在 AI Core 和 Vector Core 上。此时，block_idx 在这两种核心上都是从 0 开始计数，用户无法直接通过 block_idx 来切分数据和控制多核逻辑。而 GetBlockIdx 在 Vector Core 上对 block_idx 增加偏移量（AI Core 的 block_num），从而保证返回的值能够正确反映多核环境下的实际逻辑。
>
> —— SIMD-BuiltIn 关键字文档

---

## 2. AIV 侧不可触碰 Cube 侧资源

### 2.1 问题描述

在 `__mix__` 模式下，AIC 和 AIV 执行相同的代码路径（除了 `ASCEND_IS_AIC`/`ASCEND_IS_AIV` 条件分支内的代码）。如果 AIV 侧执行了 Cube 侧的硬件操作（如 L1/L0 Buffer 分配、Cube 侧的 `SetFlag`/`WaitFlag`），会导致未定义行为或硬件异常。

### 2.2 受影响的操作

以下操作**只能在 AIC 侧执行**：

| 类别 | 操作 | 说明 |
|------|------|------|
| Buffer 分配 | `LocalTensor<TPosition::A1/B1/A2/B2/CO1>` | Cube 侧 Buffer 在 AIV 侧不存在 |
| 硬件事件 | `SetFlag/WaitFlag<HardEvent::MTE1_MTE2>` | Cube 侧 DMA 同步事件 |
| 硬件事件 | `SetFlag/WaitFlag<HardEvent::M_MTE1>` | Cube 侧计算同步事件 |
| 硬件事件 | `SetFlag/WaitFlag<HardEvent::M_FIX>` | Cube 侧 Fixpipe 同步事件 |
| 硬件事件 | `SetFlag/WaitFlag<HardEvent::MTE2_MTE1>` | Cube 侧 LoadData 同步事件 |
| 矩阵计算 | `Mmad` | Cube 矩阵乘指令 |
| 数据搬入 | `LoadData` | L1 → L0 搬运指令 |
| Fixpipe | `Fixpipe`（AIC 侧发起） | CO1 → UB 搬运，由 AIC 发起 |

以下操作**AIV 侧可以执行**：

| 类别 | 操作 | 说明 |
|------|------|------|
| Buffer 分配 | `LocalTensor<TPosition::VECOUT>` | UB 是 AIV 侧的存储空间 |
| 数据搬出 | `DataCopy`/`DataCopyPad`（UB → GM） | Vector 侧 DMA 操作 |
| 硬件事件 | `SetFlag/WaitFlag<HardEvent::V_MTE3>` 等 | Vector 侧同步事件 |
| 跨核同步 | `CrossCoreWaitFlag` | AIV 等待 AIC 通知 |
| 向量计算 | `Add`/`Mul`/`Cast` 等 | Vector 计算指令 |

### 2.3 正确的代码结构

```cpp
__aicore__ inline void Process()
{
    InitComputeParams();  // 仅做标量计算和GM地址设置，AIC/AIV都可执行

    if ASCEND_IS_AIC {
        // Cube侧Buffer分配
        LocalTensor<half> a1Local(TPosition::A1, ...);
        LocalTensor<float> cLocal(TPosition::CO1, ...);
        // Cube侧硬件事件初始化
        SetFlag<HardEvent::MTE1_MTE2>(0);
        // Cube侧计算循环
        ProcessLoopAic(...);
    }
    if ASCEND_IS_AIV {
        // 仅Vector侧操作
        ProcessLoopAiv();
    }
}
```

---

## 3. `dualDstCtl` 双目标模式的 AIV 数据搬出

### 3.1 问题描述

使用 Fixpipe 的 `dualDstCtl=0b01`（按 M 维度拆分到两个 AIV 的 UB）时，AIV 侧的 DataCopy 参数和 GM 偏移计算需要特别注意。

### 3.2 dualDstCtl=0b01 的行为

- AIC 侧 Fixpipe 将 L0C 中的数据按 M 维度拆分：前 M/2 行写入 AIV0 的 UB，后 M/2 行写入 AIV1 的 UB
- 两个 AIV 各自从自己的 UB 读取数据，写入 GM 的不同区域

### 3.3 AIV 侧 DataCopy 的正确写法

```cpp
// AIV侧CopyOut
__aicore__ inline void CopyOutAiv(uint32_t mBlockIdx, uint32_t nBlockIdx)
{
    CrossCoreWaitFlag(0x8);  // 等待AIC侧Fixpipe完成

    uint32_t curM = ...;
    uint32_t curN = ...;

    // GetSubBlockIdx()返回全局AIV编号，%2取组内局部编号
    uint32_t localSubIdx = GetSubBlockIdx() % 2;

    // GM偏移：localSubIdx * (M/2 * N) 实现前后半矩阵的分离
    uint32_t offset = baseM / 2 * N;
    DataCopyParams copyParams;
    copyParams.blockCount = curM / 2;           // 每个AIV只搬M/2行
    copyParams.blockLen = curN * sizeof(float);  // 每行N个float
    copyParams.srcStride = 0;                   // UB中数据连续排列
    copyParams.dstStride = (N - curN) * sizeof(float);  // GM行间距
    DataCopyPad<float>(cGM[mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * offset],
                       cUB, copyParams);
}
```

### 3.4 关键注意事项

1. **`GetSubBlockIdx()` vs `GetBlockIdx()`**：`GetSubBlockIdx()` 返回同一逻辑核内的 AIV 编号（0 或 1），用于区分 dualDstCtl 的数据来源；`GetBlockIdx()` 返回 AIV 全局编号，用于多核逻辑，二者用途不同，不可混用。

2. **`cUB` 地址**：每个 AIV 的 `cUB = LocalTensor<float>(TPosition::VECOUT, 0, ...)` 地址偏移为 0，但 `dualDstCtl=0b01` 模式下硬件会自动将数据分发到各自独立的 UB 空间，所以两个 AIV 的 `cUB` 虽然偏移相同但指向不同的物理存储。

3. **`CrossCoreSetFlag`/`CrossCoreWaitFlag` 配对**：AIC 侧 Fixpipe 完成后通过 `CrossCoreSetFlag<0x2, PIPE_FIX>(0x8)` 通知两个 AIV；两个 AIV 都调用 `CrossCoreWaitFlag(0x8)` 等待，模式 2 下 AIC 的 Set 会同时唤醒两个 AIV。

---

## 4. `InitSocState()` 调用位置

`InitSocState()` 必须在任何 AscendC API 调用之前执行。在 `__mix__` 模式下，它在全局函数中、Kernel 对象构造之前调用：

```cpp
__global__ __mix__(1, 2) void kernel_entry(GM_ADDR a, GM_ADDR b, GM_ADDR c)
{
    InitSocState();       // 必须最先调用
    KernelClass op;       // 构造Kernel对象
    op.Init(a, b, c);    // 初始化
    op.Process();         // 执行
    PipeBarrier<PIPE_ALL>();  // 全局同步
}
```

也可以放在类的构造函数中（参考 `fixpipe_co12ub` 样例），效果相同。
