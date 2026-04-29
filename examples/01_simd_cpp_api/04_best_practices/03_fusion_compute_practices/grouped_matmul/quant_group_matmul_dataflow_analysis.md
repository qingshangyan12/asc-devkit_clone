# Quant Group Matmul 数据流向与内存排布全解

> 本文档聚焦于**数据如何被切分、搬运、计算和整合**，不涉及具体 API 调用细节。

---

## 一、全局参数与原始 Shape

```
M = 1024,  K = 1024,  N = 8192
groupNum = 8
groupList = [128, 128, 128, 128, 128, 128, 128, 128]  (每组 M=128)
numBlocks = 8  (8个核)
cvParallNum = 4  (Cube-Vector流水深度)
baseM = 128, baseN = 256, baseK = 128  (Cube计算基块大小)
ubCalSize = 24 × 256 = 6144  (Vector单次处理元素数，24行×256列)
```

---

## 二、Shape 拆分总览

```
完整问题:  x[1024, 1024] × weight[8, 1024, 8192]  →  y[1024, 8192]

┌─────────────────────────────────────────────────────────────────┐
│ 第1层拆分: 按 Group 拆分 (M轴)                                   │
│                                                                   │
│   Group 0:  x[  0: 128, :] × weight[0]  →  y[  0: 128, :]     │
│   Group 1:  x[128: 256, :] × weight[1]  →  y[128: 256, :]     │
│   Group 2:  x[256: 384, :] × weight[2]  →  y[256: 384, :]     │
│   ...                                                             │
│   Group 7:  x[896:1024, :] × weight[7]  →  y[896:1024, :]     │
│                                                                   │
│ 第2层拆分: 每个 Group 按 (M, N) 二维分块                          │
│                                                                   │
│   每个 Group: M=128, N=8192                                      │
│   M方向: ceil(128/128) = 1 块   →  mIdx ∈ {0}                  │
│   N方向: ceil(8192/256) = 32 块  →  nIdx ∈ {0,1,...,31}        │
│   每个 Group 总共 1×32 = 32 个 (mIdx, nIdx) 子块                │
│                                                                   │
│ 第3层拆分: Vector 侧按 M 轴进一步细分到 UB                       │
│                                                                   │
│   Cube 输出: [baseM=128, baseN=256] = 32768 个 int32             │
│   UB 容量: ubCalSize = 6144 个元素                               │
│   vecBaseM = 6144 / 256 = 24 行                                  │
│   128行拆为: 24+24+24+24+24+8 = 6 次 Vector 迭代               │
└─────────────────────────────────────────────────────────────────┘
```

---

## 三、GM 全局内存排布与大小

### 3.1 各张量 GM 排布

| 张量 | 逻辑 Shape | 数据类型 | 物理大小 | GM 地址偏移公式 |
|------|-----------|---------|---------|---------------|
| `x` | [1024, 1024] | int8 | 1,048,576 B (1MB) | `row * K + col` |
| `weight` | [8, 1024, 8192] | int8 (NZ格式) | 67,108,864 B (64MB) | `groupIdx * N * K + nIdx * baseN * K` |
| `scale` | [8, 8192] | float32 | 262,144 B (256KB) | `groupIdx * N + col` |
| `perTokenScale` | [1024] | float32 | 4,096 B (4KB) | `row` |
| `groupList` | [8] | int64 | 64 B | `groupIdx` |
| `y` | [1024, 8192] | float16 | 16,777,216 B (16MB) | `row * N + col` |
| `workspace` | 见下文 | int32 | 4,194,304 B (4MB) | 见下文 |

### 3.2 Workspace 内存排布

Workspace 是 Cube 和 Vector 之间的中转站，采用**环形分槽设计**：

```
workspace 总大小 = parallNum × numBlocks × baseM × baseN × sizeof(int32)
                 = 4 × 8 × 128 × 256 × 4
                 = 4,194,304 Bytes (4MB)

workspace 逻辑布局 (int32 元素索引):

Slot 0 (cubeCount%4=0):   ┌──────────────────────────────────────────────┐
                          │ Core0: [0..32767]     → 128×256=32768 int32 │
                          │ Core1: [32768..65535]  → 128×256=32768 int32 │
                          │ Core2: [65536..98303]  → ...                 │
                          │ ...                                          │
                          │ Core7: [229376..262143] → ...                │
                          └──────────────────────────────────────────────┘
Slot 1 (cubeCount%4=1):   [262144 .. 524287]
Slot 2 (cubeCount%4=2):   [524288 .. 786431]
Slot 3 (cubeCount%4=3):   [786432 .. 1048575]

workSpaceOffset 计算公式:
  workSpaceOffset = baseN × baseM × (coreIdx + (cubeCount % parallNum) × coreNum)
                  = 256 × 128 × (coreIdx + (cubeCount % 4) × 8)
                  = 32768 × (coreIdx + cubeCount_mod4 × 8)
```

**具体偏移示例（Core 0）**：

| cubeCount | cubeCount%4 | Slot | workSpaceOffset (int32索引) | 字节偏移 |
|-----------|------------|------|---------------------------|---------|
| 0 | 0 | Slot 0 | 32768 × (0+0) = 0 | 0 |
| 1 | 1 | Slot 1 | 32768 × (0+8) = 262144 | 1,048,576 |
| 2 | 2 | Slot 2 | 32768 × (0+16) = 524288 | 2,097,152 |
| 3 | 3 | Slot 3 | 32768 × (0+24) = 786432 | 3,145,728 |
| 4 | 0 | Slot 0 | 32768 × (0+0) = 0 | 0 (复用!) |

**关键**：当 `cubeCount >= parallNum` 时，Cube 会等待 Vector 处理完对应 Slot 的数据后，才复用该 Slot 空间。

---

## 四、完整数据流向追踪

以 **Core 0** 为例，追踪其在所有 Group 中的完整执行过程。

### 4.1 多核任务分配

#### 4.1.1 分配核心思想

Process 函数采用**跨 Group 连续轮询**的策略将子块分配给多个 Core。核心逻辑如下：

1. 每个 Group 按 `(mIdx, nIdx)` 二维分块，总块数 = `numBlocksM × numBlocksN`
2. 所有子块在逻辑上从 0 开始编号（全局递增），每个 Core 按 `curBlock += coreNum` 步长轮询取块
3. 用 `preCount` 变量记录上一个 Group 末尾未被 8 整除的余数，实现**跨 Group 的连续分配**，避免负载不均

#### 4.1.2 关键变量解析

```cpp
// Process 函数中的多核分配逻辑（简化伪代码）
uint32_t numBlocksN = Ceil(n, baseN);       // N方向分块数, 本例 = Ceil(8192, 256) = 32

for (groupIdx = 0, preCount = 0; groupIdx < groupNum; ++groupIdx) {
    m = groupList[groupIdx];                 // 本组行数, 本例每组 = 128
    numBlocksM = Ceil(m, baseM);             // M方向分块数, 本例 = Ceil(128, 128) = 1
    curCount = preCount + numBlocksM * numBlocksN;  // 含上一组余数的连续块编号上界

    // ★ 核心分配公式：确定当前 Core 在本 Group 中的第一个块编号
    curBlock = coreIdx >= preCount ? coreIdx : coreIdx + coreNum;

    while (curBlock < curCount) {
        mIdx = (curBlock - preCount) / numBlocksN;   // 在本组内的 M 方向索引
        nIdx = (curBlock - preCount) % numBlocksN;   // 在本组内的 N 方向索引
        MMCompute(groupIdx, mnConfig);
        VectorCompute(groupIdx, mnConfig);
        curBlock += coreNum;                          // 轮询步进
    }

    preCount = curCount % coreNum;    // ★ 余数传递给下一组
    offsetM += m;                     // ★ 全局行偏移累积
}
```

#### 4.1.3 `preCount` 跨组连续分配机制

`preCount` 是实现跨 Group 连续轮询的关键。当某 Group 的子块数不被 `coreNum` 整除时，末尾若干块的分配会"溢出"，需要下一组补偿：

```
curBlock 初始值公式:
  curBlock = coreIdx >= preCount ? coreIdx : coreIdx + coreNum

含义:
  - 若 coreIdx >= preCount: 当前 Core 在上组末尾没有被"多分配"一个块，
    直接从 coreIdx 开始取本组的块
  - 若 coreIdx < preCount:  当前 Core 在上组末尾已经多处理了一个块，
    需要跳过本组前 preCount 个块（它们属于其他 Core），从 coreIdx+coreNum 开始

preCount 传递:
  preCount_next = curCount % coreNum
  即本组（含上组余数）的总块数对 coreNum 取模，余数传递给下一组
```

#### 4.1.4 本样例的逐 Group 逐步追踪

本样例参数：`coreNum=8`, `numBlocksN=32`, 每组 `m=128`, `numBlocksM=1`

每组块数 = 1 × 32 = 32，32 % 8 = 0，因此 `preCount` 始终为 0，分配非常规律。

**Group 0 (groupIdx=0, offsetM=0, m=128, 32块)**

```
preCount = 0
curCount = 0 + 32 = 32

各 Core 的 curBlock 起始值:
  coreIdx >= preCount(0) 恒成立 → curBlock = coreIdx

  Core 0: curBlock = 0,  8, 16, 24  → nIdx = 0, 8, 16, 24  (4块)
  Core 1: curBlock = 1,  9, 17, 25  → nIdx = 1, 9, 17, 25  (4块)
  Core 2: curBlock = 2, 10, 18, 26  → nIdx = 2,10, 18, 26  (4块)
  Core 3: curBlock = 3, 11, 19, 27  → nIdx = 3,11, 19, 27  (4块)
  Core 4: curBlock = 4, 12, 20, 28  → nIdx = 4,12, 20, 28  (4块)
  Core 5: curBlock = 5, 13, 21, 29  → nIdx = 5,13, 21, 29  (4块)
  Core 6: curBlock = 6, 14, 22, 30  → nIdx = 6,14, 22, 30  (4块)
  Core 7: curBlock = 7, 15, 23, 31  → nIdx = 7,15, 23, 31  (4块)

preCount = 32 % 8 = 0  → 传递给 Group 1
offsetM += 128  →  offsetM = 128
```

**Group 1 ~ Group 7**: 由于 `preCount=0`，分配方式完全相同，仅 `offsetM` 递增。

```
Group 1: offsetM = 128, preCount_in = 0, 各Core分配同上, preCount_out = 0
Group 2: offsetM = 256, preCount_in = 0, 各Core分配同上, preCount_out = 0
Group 3: offsetM = 384, preCount_in = 0, 各Core分配同上, preCount_out = 0
Group 4: offsetM = 512, preCount_in = 0, 各Core分配同上, preCount_out = 0
Group 5: offsetM = 640, preCount_in = 0, 各Core分配同上, preCount_out = 0
Group 6: offsetM = 768, preCount_in = 0, 各Core分配同上, preCount_out = 0
Group 7: offsetM = 896, preCount_in = 0, 各Core分配同上, preCount_out = 0
```

#### 4.1.5 `preCount` 非零场景举例

为说明 `preCount` 的作用，假设 Group 0 的 `m=200`，`numBlocksM = Ceil(200/128) = 2`：

```
Group 0 (假设 m=200):
  块数 = 2 × 32 = 64
  preCount = 0, curCount = 64
  64 % 8 = 0  → preCount_out = 0  (恰好整除，无溢出)

若改为 m=200 且 numBlocksN=5 (假设 N 更小):
  块数 = 2 × 5 = 10
  preCount = 0, curCount = 10

  Core 0: curBlock = 0, 8       → (mIdx=0,nIdx=0), (mIdx=1,nIdx=3)  共2块
  Core 1: curBlock = 1, 9       → (mIdx=0,nIdx=1), (mIdx=1,nIdx=4)  共2块
  Core 2: curBlock = 2          → (mIdx=0,nIdx=2)                    共1块
  Core 3: curBlock = 3          → (mIdx=0,nIdx=3)                    共1块
  Core 4: curBlock = 4          → (mIdx=0,nIdx=4)                    共1块
  Core 5: curBlock = 5          → (mIdx=1,nIdx=0)                    共1块
  Core 6: curBlock = 6          → (mIdx=1,nIdx=1)                    共1块
  Core 7: curBlock = 7          → (mIdx=1,nIdx=2)                    共1块

  preCount_out = 10 % 8 = 2

Group 1 (m=128, numBlocksM=1, numBlocksN=5, 块数=5):
  preCount = 2, curCount = 2 + 5 = 7

  Core 0: coreIdx(0) < preCount(2) → curBlock = 0+8 = 8 ≥ curCount(7), 跳过!  共0块
  Core 1: coreIdx(1) < preCount(2) → curBlock = 1+8 = 9 ≥ curCount(7), 跳过!  共0块
  Core 2: coreIdx(2) >= preCount(2) → curBlock = 2     → (mIdx=0,nIdx=0)  共1块
  Core 3: coreIdx(3) >= preCount(2) → curBlock = 3     → (mIdx=0,nIdx=1)  共1块
  Core 4: coreIdx(4) >= preCount(2) → curBlock = 4     → (mIdx=0,nIdx=2)  共1块
  Core 5: coreIdx(5) >= preCount(2) → curBlock = 5     → (mIdx=0,nIdx=3)  共1块
  Core 6: coreIdx(6) >= preCount(2) → curBlock = 6     → (mIdx=0,nIdx=4)  共1块
  Core 7: coreIdx(7) >= preCount(2) → curBlock = 7 ≥ curCount(7), 跳过!   共0块

  效果: Core 0,1 在上组多处理了1块，本组少处理1块; Core 2~6 补上，整体均衡
  preCount_out = 7 % 8 = 7
```

**核心思想**: `preCount` 机制确保了跨 Group 的连续轮询——上组末尾多分配的 Core，在下组开头少分配，从而保证所有 Core 的总负载始终均衡。

#### 4.1.6 mIdx / nIdx 的计算

从 `curBlock` 反推当前子块在 Group 内的二维坐标：

```
groupLocalIdx = curBlock - preCount       // 去除上组余数偏移后的组内线性索引
mIdx = groupLocalIdx / numBlocksN          // 整除得到 M 方向块号
nIdx = groupLocalIdx % numBlocksN          // 取模得到 N 方向块号
```

本样例中 `numBlocksM=1`，因此 `mIdx` 恒为 0，`nIdx` 即为组内线性索引。

#### 4.1.7 本样例总结

```
总 Group 数:                  8
每组块数:                     1 × 32 = 32
总块数:                       8 × 32 = 256
每核处理块数:                 256 / 8 = 32 (均匀分配)
preCount:                     始终为 0 (32 % 8 = 0)

Core 0 共处理: 8 Group × 4 块/Group = 32 个子块
  每个 Group 中: nIdx = 0, 8, 16, 24

Core k 共处理: 32 个子块
  每个 Group 中: nIdx = k, k+8, k+16, k+24
```

### 4.2 单个子块的完整数据流

以 **Group 0, (mIdx=0, nIdx=0)** 为例，追踪一个子块从输入到输出的全过程。本样例中 `curSingleM=128, curSingleN=256`。

#### 4.2.1 MMCompute：Cube 侧矩阵乘

##### 执行流程概要

MMCompute 由  AIC（Cube 核）执行矩阵乘法，将 GM 上的 int8 输入相乘，输出 int32 结果写入 workspace。

##### 关键变量计算

```cpp
// ① 计算当前子块的实际 M/N 大小（处理尾部块）
tailN = nIdx * singleN;                    // N方向起始偏移
curSingleN = (nIdx == numBlocksN - 1) ? (n - tailN) : singleN;
curSingleM = (mIdx == numBlocksM - 1) ? (m - mIdx * singleM) : singleM;

// 本例: nIdx=0, numBlocksN=32, 不是最后一个nIdx → curSingleN = 256
//       mIdx=0, numBlocksM=1,  是最后一个mIdx  → curSingleM = 128-0 = 128

// ② 计算输入偏移
xOffset = (offsetM + mIdx * singleM) * k;
//       = (0 + 0 * 128) * 1024 = 0
//       含义: x矩阵中第 offsetM+mIdx*128 行开始的连续 curSingleM 行

weightOffset = groupIdx * n * k + tailN * k;
//            = 0 * 8192 * 1024 + 0 * 1024 = 0
//            含义: weight[groupIdx] 中第 tailN 列开始的连续 curSingleN 列
//            注意: NZ格式下偏移以K为步进单位

// ③ 计算 workspace 写入偏移（★ 环形分槽核心公式）
workSpaceOffset = singleN * singleM * (coreIdx + (cubeCount % parallNum) * coreNum);
//               = 256 * 128 * (0 + (0 % 4) * 8) = 0
//               含义: 当前 Cube 输出写入 workspace 的起始位置
//               公式拆解: baseM*baseN 是一个Slot/Core的大小
//                         coreIdx 决定同Slot内不同Core的偏移
//                         (cubeCount % parallNum) * coreNum 决定不同Slot的偏移
```

##### 同步等待逻辑

```cpp
// ④ Cube-Vector 流水同步
if (cubeCount >= parallNum) {     // parallNum = 4
    CrossCoreWaitFlag(SYNC_AIV_TO_AIC);  // 等待 Vector 释放最早占用的 Slot
}
// 第一次 cubeCount=0: 不等待
// 第四次 cubeCount=3: 不等待
// 第五次 cubeCount=4: 等待! 等 Vector 处理完 Slot 0 后才能复用
```

**关键**: `cubeCount` 在 MMCompute 结尾无条件 `++`，因此前 `parallNum` 次无需等待，之后每次 Cube 计算前都要等 Vector 释放最早占用的 Slot。

##### Matmul 配置与执行

```cpp
// ⑤ 设置 Matmul 参数
mm.SetOrgShape(m, n, k);              // 整体逻辑 shape: (128, 8192, 1024)
mm.SetSingleShape(curSingleM, curSingleN, k);  // 本次实际计算: (128, 256, 1024)
mm.SetTensorA(xGm[xOffset]);          // 左矩阵起始地址
mm.SetTensorB(weightSlice);           // 右矩阵起始地址, NZ格式

// ⑥ L2 Cache 优化
if (numBlocksM == 1) {
    weightSlice.SetL2CacheHint(CACHE_MODE_DISABLE);
}
// 当 M 方向只有1个块时, weight 在 L2 中不会被其他 M 块复用,
// 关闭 L2 Cache 避免缓存污染

// ⑦ Iterate 循环
uint64_t worskspaceOffset = workSpaceOffset;
while (mm.Iterate()) {    // M方向1块×N方向1块 → Iterate返回1次
    mm.GetTensorC(mmOutGm[worskspaceOffset], 0, true);
    //   ↑ 将 Cube 计算结果写入 workspace
    //   参数 0: 不偏移, true: 使能 ND→NZ 格式转换
    //   写入大小: baseM × baseN = 128×256 = 32768 个 int32 = 131,072 Bytes

    CrossCoreSetFlag<2, PIPE_FIX>(SYNC_AIC_TO_AIV);
    //   ↑ 通知 Vector: 当前 Iterate 的数据已就绪
    //   mode=2: group 内同步, PIPE_FIX: Cube 侧管道

    worskspaceOffset += (baseM * baseN);
    //   ↑ 若 Iterate 多次, workspace 偏移递增
    //   本例只 Iterate 1次, 此递增无实际效果
}

// ⑧ cubeCount 递增（★ 无论 AIC 还是 AIV 都递增）
cubeCount++;
```

##### 本例 MMCompute 数据流图

```
xGm[0]                weightGm[0]                Cube (AIC)
 x[0:128, 0:1024]      weight[0, :, 0:256]         │
     │                      │                       │
     │  SetTensorA          │  SetTensorB            │
     └──────────────────────┴───────────────────────►│
                                                    │ int8 × int8 → int32
                                                    │ K轴累加: 8次 (1024/128)
                                                    │ Iterate: 1次 (M=1块, N=1块)
                                                    │
                      workspace                     │
                   mmOutGm[0]                       │
                   ┌──────────┐                     │
                   │128×256   │◄────────────────────┘
                   │int32     │  GetTensorC + SetFlag
                   │131,072 B │  → 通知 Vector 数据就绪
                   └──────────┘
```

#### 4.2.2 VectorCompute：Vector 侧反量化与激活

##### 执行流程概要

VectorCompute 由 AIV（Vector 核）执行。它从 workspace 读取 Cube 输出的 int32 结果，经过 per-channel 反量化、per-token 反量化、激活、类型转换，最终写出 half 结果到 y。

**AIC 侧直接 return**：`if ASCEND_IS_AIC { return; }`，AIC 核不执行 VectorCompute。

##### 关键变量计算

```cpp
// ① 重新计算 curCubeSingleN / curCubeSingleM（与 Cube 侧一致）
curCubeSingleN = (nIdx == numBlocksN - 1) ? (n - nIdx * singleN) : singleN;
curCubeSingleM = (mIdx == numBlocksM - 1) ? (m - mIdx * singleM) : singleM;
// 本例: curCubeSingleN = 256, curCubeSingleM = 128

// ② 计算 vecBaseM（UB 容量决定的每次 Vector 迭代行数）
vecBaseM = ubCalSize / (Ceil(baseN, 8) * 8);
//        = 6144 / (Ceil(256, 8) * 8)
//        = 6144 / 256 = 24
vecBaseM = min(vecBaseM, curCubeSingleM);
//        = min(24, 128) = 24

// ③ 计算 scale / out 偏移
scaleOffset = groupIdx * n + nIdx * singleN;
//           = 0 * 8192 + 0 * 256 = 0

outOffset = (offsetM + mIdx * singleM) * n + nIdx * singleN;
//         = (0 + 0 * 128) * 8192 + 0 * 256 = 0
//         含义: y 矩阵中当前子块的起始写入位置

// ④ taskRation: AIV 内部子任务分配（__mix__ 模式下多个 AIV 共享同一 coreIdx）
taskRation = GetTaskRation();  // __mix__(1,1) 下为 1
```

##### 外层循环：offsetN 迭代

```cpp
for (offsetN = 0; offsetN < curCubeSingleN; offsetN += baseN) {
    // 本例: curCubeSingleN=256, baseN=256 → 仅 1 次迭代, offsetN=0

    // ⑤ 尾部 N 处理
    if (offsetN + baseN >= curCubeSingleN) {
        curVecBaseN = curCubeSingleN - offsetN;  // 本例: 256 - 0 = 256
    }

    // ⑥ 对齐到 8 的倍数（32B UB 块对齐，8个int32=32B）
    alignBaseN = Ceil(curVecBaseN, 8) * 8;  // Ceil(256, 8) * 8 = 256

    // ⑦ 拷贝 scale 到 UB
    DataCopyScale(curVecBaseN, alignBaseN, scaleOffset + offsetN);

    // ⑧ 计算 workspace 读取偏移
    mmOutOffset = workSpaceOffset + offsetN * baseM;
    //            = 0 + 0 * 128 = 0

    // ⑨ ★ 等待 Cube 完成当前 Iterate 的数据写入
    CrossCoreWaitFlag(SYNC_AIC_TO_AIV);

    // ⑩ 内层 offsetM 循环
    for (offsetM = 0; offsetM < curCubeSingleM; offsetM += vecBaseM) {
        // ...见下文
    }

    // ⑪ 释放 scale UB 缓存
    scaleInQueue.FreeTensor(scaleInUb);
}

// ⑫ 通知 Cube: Vector 已完成当前子块, 可以复用对应 Slot
CrossCoreSetFlag<2, PIPE_MTE2>(SYNC_AIV_TO_AIC);
```

**外层 offsetN 循环的作用**：当 Cube 输出的 N 方向大于 baseN 时，Vector 需要分多次沿 N 轴处理。每次 offsetN 迭代读取一组 scale，然后在内层 offsetM 循环中处理该 N 切片的所有行。

##### 内层循环：offsetM 迭代

```cpp
for (offsetM = 0; offsetM < curCubeSingleM; offsetM += vecBaseM) {
    vecCount++;

    // ★ AIV 子任务过滤（__mix__ 多 AIV 场景）
    if (taskRation != 0 && vecCount % taskRation != subBlockIdx) {
        continue;  // 本子块不由当前 AIV 负责, 跳过
    }

    // ⑬ 尾部 M 处理
    if (offsetM + vecBaseM >= curCubeSingleM) {
        curVecBaseM = curCubeSingleM - offsetM;
    }

    // ⑭ workspace → UB (vecInQueue)
    mmOutLocal = vecInQueue.AllocTensor<cT::T>();
    DataCopyPad2D(mmOutLocal, mmOutGm[mmOutOffset + offsetM * curVecBaseN],
                  curVecBaseM, curVecBaseN, curVecBaseN);
    //   源: workspace 中从 mmOutOffset 起, 跳过 offsetM 行, 读取 curVecBaseM 行
    //   大小: curVecBaseM × curVecBaseN 个 int32
    //   srcDim0 = curVecBaseN (workspace 中每行连续存储)
    vecInQueue.EnQue(mmOutLocal);

    // ⑮ 反量化 + 激活 + 类型转换
    ComputeDequantAndActivate(mnConfig, curVecBaseM, alignBaseN, curVecBaseN, offsetM);

    // ⑯ UB → y(GM)
    yLocal = vecOutQueue.DeQue<half>();
    DataCopyPad2D(yGm[outOffset + offsetM * n + offsetN], yLocal,
                  curVecBaseM, curVecBaseN, alignBaseN, n);
    //   目标: yGm 中 outOffset + offsetM*n + offsetN 处
    //   二维写出: dim1=curVecBaseM行, dim0=curVecBaseN列
    //   srcDim0=alignBaseN (UB 中对齐后的行宽)
    //   dstDim0=n=8192 (GM 中 y 的行宽)
    vecOutQueue.FreeTensor(yLocal);
}
```

##### 内层循环的 6 次迭代详细追踪

```
curCubeSingleM = 128, vecBaseM = 24, curVecBaseN = 256

┌───────┬────────┬─────────────┬──────────────┬───────────────────────────────────────┐
│迭代 # │offsetM │curVecBaseM  │ 读取行范围    │ workspace 读取偏移                     │
│       │        │             │ (workspace内) │ mmOutOffset + offsetM * curVecBaseN    │
├───────┼────────┼─────────────┼──────────────┼───────────────────────────────────────┤
│  #0   │   0    │    24       │ [0, 24)      │ 0 + 0 * 256 = 0                       │
│  #1   │  24    │    24       │ [24, 48)     │ 0 + 24 * 256 = 6144                   │
│  #2   │  48    │    24       │ [48, 72)     │ 0 + 48 * 256 = 12288                  │
│  #3   │  72    │    24       │ [72, 96)     │ 0 + 72 * 256 = 18432                  │
│  #4   │  96    │    24       │ [96, 120)    │ 0 + 96 * 256 = 24576                  │
│  #5   │ 120    │     8       │ [120, 128)   │ 0 + 120 * 256 = 30720                 │
│       │        │  (尾部)     │              │  (offsetM+vecBaseM=144 ≥ 128 触发尾部) │
└───────┴────────┴─────────────┴──────────────┴───────────────────────────────────────┘

┌───────┬────────────────────────────────────────────────────────────────────────────┐
│迭代 # │ y 写出偏移 outOffset + offsetM * n + offsetN                               │
│       │ = 0 + offsetM * 8192 + 0                                                  │
├───────┼────────────────────────────────────────────────────────────────────────────┤
│  #0   │ yGm[0]            → y[0:24,   0:256]                                     │
│  #1   │ yGm[196608]       → y[24:48,  0:256]    (24 * 8192 = 196608)             │
│  #2   │ yGm[393216]       → y[48:72,  0:256]    (48 * 8192 = 393216)             │
│  #3   │ yGm[589824]       → y[72:96,  0:256]    (72 * 8192 = 589824)             │
│  #4   │ yGm[786432]       → y[96:120, 0:256]    (96 * 8192 = 786432)             │
│  #5   │ yGm[983040]       → y[120:128,0:256]    (120 * 8192 = 983040)            │
└───────┴────────────────────────────────────────────────────────────────────────────┘
```

#### 4.2.3 ComputeDequantAndActivate：单次 offsetM 迭代的计算流水

每次 offsetM 迭代调用 `ComputeDequantAndActivate`，执行 5 步计算：

```
Step 1: DataCopyPerTokenScaleAndBrcb — 读取 per-token scale 并 Broadcast
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  perTokenScaleOffset = offsetM + mIdx * singleM + offsetM_vec
                      = 0 + 0 * 128 + 0 = 0    (第#0次迭代)
                      = 0 + 0 * 128 + 24 = 24   (第#1次迭代)
                      = 0 + 0 * 128 + 48 = 48   (第#2次迭代)
                      ...

  拷贝: perTokenScaleGm[perTokenScaleOffset] → UB perTokenScaleInQueue
        curVecBaseM 个 float (如 24 个 float = 96 Bytes)

  Broadcast: [curVecBaseM, 1] → [curVecBaseM, alignBaseN]
           如 [24, 1] → [24, 256]
  输出: pertokenBrcbLocal, curVecBaseM × alignBaseN 个 float

Step 2: AscendDequant — per-channel 反量化
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  输入: mmOutInUb[curVecBaseM, curVecBaseN] (int32) + scaleInUb[alignBaseN] (float)
  输出: dequantMiddleResult[curVecBaseM, alignBaseN] (float)
  计算: result[i][j] = mmOut[i][j] * scale[j]   (每列一个 scale, 逐元素乘)
  sharedTmpLocal 作为 API 内部临时空间

Step 3: Mul — per-token 反量化
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  输入: dequantMiddleResult[curVecBaseM, alignBaseN] + pertokenBrcbLocal[curVecBaseM, alignBaseN]
  输出: mulsResultLocal[curVecBaseM, alignBaseN] (float)
  计算: result[i][j] = dequant[i][j] * pertokenScale[i]  (每行一个 scale, 逐元素乘)
  computeSize = curVecBaseM * alignBaseN (如 24 * 256 = 6144)

Step 4: FasterGelu — 激活函数
━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  输入: mulsResultLocal[computeSize]
  输出: actResultLocal[computeSize] (float)
  临时空间: actTmpLocal = tmpBuff 的前 2*ubCalSize*4 字节
           (与 dequantMiddleResult + pertokenBrcbLocal 空间复用, 时序不冲突)

Step 5: Cast — float32 → float16
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  输入: actResultLocal[computeSize] (float)
  输出: yLocalInUb[computeSize] (half)
  写入 vecOutQueue
```

#### 4.2.4 DataCopyScale：scale 参数搬运

```cpp
// 从 GM 拷贝 scale 到 UB scaleInQueue
DataCopyPadExtParams<float> padParams;  // 默认 padding 参数
DataCopyExtParams scaleParams{
    1,                    // blockCount = 1 (一维拷贝)
    curBaseN * sizeof(float),  // blockLen = 256 * 4 = 1024 Bytes
    1,                    // srcStride (不使用)
    1,                    // dstStride (不使用)
    0                     // dstStride2 (不使用)
};
scaleLocal = scaleInQueue.AllocTensor<float>();
DataCopyPad(scaleLocal, scaleGm[scaleOffset], scaleParams, padParams);
//   源: scaleGm[groupIdx * N + nIdx * baseN + offsetN]
//   大小: curBaseN 个 float (如 256 个 = 1024 Bytes)

scaleInUb = scaleInQueue.DeQue<float>();
scaleInUb.SetSize(alignBaseN);  // 设置逻辑大小为对齐后的值
```

**scale 是一维参数**：每组每列一个 scale 值，shape 为 `[groupNum, N]`，offsetN 循环内每次拷贝 `curVecBaseN` 个连续元素。

#### 4.2.5 PostCompute：流水收尾

```cpp
// PostCompute 在 Process 函数的 Group 循环结束后调用
if ASCEND_IS_AIC {
    if (cubeCount > 0) {  // 至少有一次 Cube 计算
        // ★ 需要等待最后 min(cubeCount, parallNum) 个 Vector 完成
        // 因为 Cube 的最后几次计算可能还在 workspace 中被 Vector 读取
        uint32_t loop = min(cubeCount, parallNum);
        for (idx = 0; idx < loop; ++idx) {
            CrossCoreWaitFlag(SYNC_AIV_TO_AIC);
        }
    }
}
```

**为什么是 `min(cubeCount, parallNum)`**：
- Cube 每次执行 MMCompute 后 `cubeCount++`
- 当 `cubeCount >= parallNum` 时，Cube 每次都会 Wait 一个 Vector 的完成信号
- 所以 Process 结束时，最多还有 `parallNum` 个未等待的 Vector 任务
- 但如果总 cubeCount < parallNum，则实际只发了 cubeCount 个 Vector 任务

#### 4.2.6 完整数据流总图（单子块视角）

```
┌─────────────────────────── GM 输入 ───────────────────────────┐
│                                                                 │
│  xGm[xOffset=0]:                                               │
│  ┌────────────────────┐                                        │
│  │ x[0:128, 0:1024]   │  int8, 128×1024=131,072 Bytes         │
│  └────────────────────┘                                        │
│                                                                 │
│  weightGm[weightOffset=0]:                                     │
│  ┌────────────────────┐                                        │
│  │ weight[0, :, 0:256]│  int8 NZ格式, 1024×256=262,144 Bytes  │
│  └────────────────────┘                                        │
│                                                                 │
│  scaleGm[scaleOffset=0]:                                       │
│  ┌──────┐                                                      │
│  │scale[0, 0:256] │  float32, 256×4=1,024 Bytes               │
│  └──────┘                                                      │
│                                                                 │
│  perTokenScaleGm[offsetM + mIdx*singleM + offsetM_vec]:       │
│  ┌──────┐                                                      │
│  │pts[offsetM_vec : offsetM_vec+curVecBaseM]                   │
│  │  float32, curVecBaseM×4 Bytes (随 offsetM 迭代变化)          │
│  └──────┘                                                      │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌───────────────────── MMCompute (AIC) ─────────────────────────┐
│                                                                 │
│  ① 计算偏移: xOffset, weightOffset, workSpaceOffset            │
│  ② 若 cubeCount >= 4: WaitFlag(SYNC_AIV_TO_AIC)              │
│  ③ SetOrgShape(128, 8192, 1024)                                │
│     SetSingleShape(128, 256, 1024)                              │
│  ④ SetTensorA + SetTensorB                                     │
│  ⑤ Iterate → GetTensorC → workspace                           │
│     每次写入: baseM×baseN = 32,768 个 int32 = 131,072 Bytes     │
│  ⑥ SetFlag(SYNC_AIC_TO_AIV) → 通知 Vector                     │
│  ⑦ cubeCount++                                                  │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌────────────────── workspace (中转) ──────────────────────────┐
│                                                                 │
│  mmOutGm[workSpaceOffset] = mmOutGm[0]                         │
│  ┌────────────────────┐                                        │
│  │ mmOut[0:128, 0:256]│  int32, 128×256×4 = 131,072 Bytes     │
│  │ Slot 0, Core 0     │                                        │
│  └────────────────────┘                                        │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────── VectorCompute (AIV) ──────────────────────────┐
│                                                                 │
│  外层 offsetN=0 (仅1次迭代):                                    │
│                                                                 │
│  ⑧ DataCopyScale: scaleGm[0] → UB scaleInQueue (256 floats)   │
│                                                                 │
│  ⑨ WaitFlag(SYNC_AIC_TO_AIV) — 等待 Cube 完成写入              │
│                                                                 │
│  内层 offsetM 循环 (6次):                                       │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ #0: offsetM=0,  curVecBaseM=24                          │   │
│  │   workspace[0+0*256] → UB vecInQueue (24×256 int32)     │   │
│  │   perTokenScale[0:24] → UB → Broadcast [24,1]→[24,256] │   │
│  │   Dequant(int32×scale → float)                          │   │
│  │   Mul(float×pertokenScale → float)                      │   │
│  │   FasterGelu(float → float)                             │   │
│  │   Cast(float → half) → vecOutQueue                      │   │
│  │   UB → yGm[0+0*8192+0] = y[0:24, 0:256]               │   │
│  ├─────────────────────────────────────────────────────────┤   │
│  │ #1: offsetM=24, curVecBaseM=24                          │   │
│  │   workspace[0+24*256] → UB                              │   │
│  │   perTokenScale[24:48] → UB → Broadcast                 │   │
│  │   Dequant → Mul → FasterGelu → Cast                     │   │
│  │   UB → yGm[0+24*8192+0] = y[24:48, 0:256]             │   │
│  ├─────────────────────────────────────────────────────────┤   │
│  │ #2~#4: 同理, offsetM=48,72,96                           │   │
│  ├─────────────────────────────────────────────────────────┤   │
│  │ #5: offsetM=120, curVecBaseM=8 (尾部!)                  │   │
│  │   workspace[0+120*256] → UB (8×256 int32)               │   │
│  │   perTokenScale[120:128] → UB → Broadcast [8,1]→[8,256]│   │
│  │   Dequant → Mul → FasterGelu → Cast                     │   │
│  │   UB → yGm[0+120*8192+0] = y[120:128, 0:256]          │   │
│  └─────────────────────────────────────────────────────────┘   │
│                                                                 │
│  ⑩ SetFlag(SYNC_AIV_TO_AIC) → 通知 Cube 可复用当前 Slot       │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│  y[0:128, 0:256]  half, 128×256×2 = 65,536 Bytes               │
│  由 6 次 Vector 迭代拼接完成                                     │
└─────────────────────────────────────────────────────────────────┘
```

#### 4.2.7 MMCompute 与 VectorCompute 的调用时序

在 Process 函数中，MMCompute 和 VectorCompute 是**顺序调用**的：

```cpp
while (curBlock < curCount) {
    MMCompute(groupIdx, mnConfig);       // Cube 执行, AIV 跳过
    VectorCompute(groupIdx, mnConfig);   // Vector 执行, AIC 跳过
    curBlock += coreNum;
}
```

但由于 `__mix__(1, 1)` 核函数模式下 AIC 和 AIV 并行运行在同一个物理核上：
- **AIC 执行路径**: 进入 MMCompute → 执行矩阵乘 → SetFlag → 跳过 VectorCompute → 进入下一次 MMCompute
- **AIV 执行路径**: 跳过 MMCompute → 进入 VectorCompute → WaitFlag → 执行反量化 → SetFlag → 进入下一次 VectorCompute

两者的并行通过 SetFlag/WaitFlag 同步实现，形成了 Cube-Vector 流水。

#### 4.2.8 同步标志总结

```
┌──────────────┬────────────────────┬────────────────────┬───────────────────────┐
│ 标志常量      │ 方向               │ 触发时机            │ 含义                   │
├──────────────┼────────────────────┼────────────────────┼───────────────────────┤
│ SYNC_AIC_TO_ │ Cube → Vector      │ 每次 Iterate 完成后 │ Cube 数据已写入        │
│ AIV (=5)     │ (SetFlag<PIPE_FIX>)│                    │ workspace, Vector     │
│              │                    │                    │ 可以读取              │
├──────────────┼────────────────────┼────────────────────┼───────────────────────┤
│ SYNC_AIV_TO_ │ Vector → Cube      │ 每次子块处理完成后  │ Vector 已读完当前     │
│ AIC (=3)     │ (SetFlag<PIPE_MTE2>)│                   │ workspace Slot,      │
│              │                    │                    │ Cube 可以复用该 Slot  │
└──────────────┴────────────────────┴────────────────────┴───────────────────────┘

等待规则:
  Cube:  cubeCount >= parallNum 时, 每次 MMCompute 开头 Wait(SYNC_AIV_TO_AIC)
  Vector: 每个 offsetN 循环开头 Wait(SYNC_AIC_TO_AIV)
  PostCompute: Cube 等待最后 min(cubeCount, parallNum) 个 Vector 完成
```

---

## 五、UB 内存分配详解

### 5.1 TQue 队列分配

| 队列 | 位置 | 单Buffer大小 | Buffer数 | 总大小 | 用途 |
|------|------|------------|---------|-------|------|
| `scaleInQueue` | VECIN | baseN × 4 = 1024 B | 2 | 2,048 B | scale 参数 |
| `perTokenScaleInQueue` | VECIN | baseM × 4 = 512 B | 2 | 1,024 B | perTokenScale 参数 |
| `vecInQueue` | VECIN | ubCalSize × 4 = 24,576 B | 2 | 49,152 B | Cube int32 结果 |
| `vecOutQueue` | VECOUT | ubCalSize × 2 = 12,288 B | 2 | 24,576 B | 最终 half 输出 |

**TQue 合计**: 2,048 + 1,024 + 49,152 + 24,576 = **76,800 Bytes**

### 5.2 TBuf (tmpBuff) 分配

tmpBuff 总大小 = **118,784 Bytes**，内部分区：

```
tmpBuff (118,784 Bytes) 偏移布局:

偏移 0        偏移 24,576    偏移 49,152    偏移 73,728
  │              │              │              │
  ▼              ▼              ▼              ▼
┌──────────────┬──────────────┬──────────────┬──────────────┐
│              │              │              │              │
│ dequantMid   │ pertokenBrcb │ mulsResult / │ actResult    │
│   Result     │    Local     │ sharedTmp    │   Local      │
│              │              │   Local      │              │
│ 6144 floats  │ 6144 floats  │ 6144 floats  │ 6144 floats  │
│ = 24,576 B   │ = 24,576 B   │ = 24,576 B   │ = 24,576 B   │
│              │              │ +24,576 B    │              │
│              │              │ (tmp空间)    │              │
│              │              │              │              │
└──────────────┴──────────────┴──────────────┴──────────────┘
                              ↑                              
                    sharedTmpLocal 起始偏移 = 49,152
                    sharedTmpLocal 大小 = 49,152 B (2×ubCalSize×4)
                    覆盖 mulsResultLocal 和 actResultLocal 空间
                    
说明: sharedTmpLocal 作为 AscendDequant / BroadCast 的内部临时空间，
      与 mulsResultLocal / actResultLocal 空间复用（时序上不冲突）
```

**UB 总占用**: 76,800 + 118,784 = **195,584 Bytes**

---

## 六、各阶段偏移量计算汇总

### 6.1 Cube 读取输入偏移

| 参数 | 公式 | Group0 nIdx=0 | Group0 nIdx=8 | Group1 nIdx=0 |
|------|------|--------------|--------------|--------------|
| `xOffset` | `(offsetM + mIdx × baseM) × K` | (0+0)×1024 = 0 | (0+0)×1024 = 0 | (128+0)×1024 = 131072 |
| `weightOffset` | `groupIdx × N × K + nIdx × baseN × K` | 0×8192×1024 + 0 = 0 | 0 + 8×256×1024 = 2,097,152 | 1×8192×1024 + 0 = 8,388,608 |

### 6.2 Workspace 写入偏移

| 参数 | 公式 | Core0 cubeCount=0 | Core0 cubeCount=1 | Core3 cubeCount=0 |
|------|------|-------------------|-------------------|-------------------|
| `workSpaceOffset` | `baseM×baseN × (coreIdx + (cubeCount%4)×8)` | 32768×(0+0)=0 | 32768×(0+8)=262144 | 32768×(3+0)=98304 |

### 6.3 Vector 读取偏移

| 参数 | 公式 | offsetN=0 | 说明 |
|------|------|-----------|------|
| `scaleOffset` | `groupIdx×N + nIdx×baseN + offsetN` | 0×8192+0+0=0 | scale 一维索引 |
| `mmOutOffset` | `workSpaceOffset + offsetN×baseM` | 0+0=0 | workspace 内偏移 |
| `perTokenScaleOffset` | `offsetM_global + offsetM_vec` | 0+0=0 | 全局行号 |

### 6.4 Vector 写出偏移

| 参数 | 公式 | Group0 nIdx=0 offsetM=0 | Group0 nIdx=0 offsetM=24 |
|------|------|------------------------|-------------------------|
| `outOffset` | `(offsetM_global + mIdx×baseM)×N + nIdx×baseN` | (0+0)×8192+0=0 | 同左 |
| 写出目标 | `yGm[outOffset + offsetM_vec×N + offsetN]` | yGm[0] | yGm[24×8192]=yGm[196608] |

---

## 七、不同 nIdx 的数据流差异

以 Group 0、Core 0 处理的 4 个 nIdx 块为例：

```
Core 0 在 Group 0 处理: nIdx = 0, 8, 16, 24

┌──────────┬───────────┬──────────────────┬───────────────────┬────────────────────┐
│ nIdx     │ weight偏移  │ scale偏移         │ workspace偏移       │ y输出偏移           │
│          │ (int8元素) │ (float元素)       │ (int32元素)        │ (half元素)         │
├──────────┼───────────┼──────────────────┼───────────────────┼────────────────────┤
│ 0        │ 0         │ 0                │ 0 (Slot0)         │ 0                  │
│ 8        │ 2,097,152 │ 2,048            │ 262,144 (Slot1)   │ 2,048              │
│ 16       │ 4,194,304 │ 4,096            │ 524,288 (Slot2)   │ 4,096              │
│ 24       │ 6,291,456 │ 6,144            │ 786,432 (Slot3)   │ 6,144              │
└──────────┴───────────┴──────────────────┴───────────────────┴────────────────────┘

注: y偏移 = nIdx × baseN = nIdx × 256 (写出到 y[:, nIdx×256 : (nIdx+1)×256])
```

---

## 八、跨 Group 的 offsetM 累积

offsetM 记录了当前组在全局 x 矩阵中的行偏移起点，每处理完一个 Group 递增：

```
Group 0:  offsetM_start = 0    offsetM_end = 128   → x[0:128, :]
Group 1:  offsetM_start = 128  offsetM_end = 256   → x[128:256, :]
Group 2:  offsetM_start = 256  offsetM_end = 384   → x[256:384, :]
...
Group 7:  offsetM_start = 896  offsetM_end = 1024  → x[896:1024, :]

offsetM 影响:
  ① xOffset  = (offsetM + mIdx × baseM) × K     → 决定 x 读取位置
  ② perTokenScaleOffset = offsetM + mIdx × baseM + offsetM_vec  → 决定 perTokenScale 读取位置
  ③ outOffset = (offsetM + mIdx × baseM) × N + nIdx × baseN     → 决定 y 写出位置
```

---

## 九、Cube-Vector 流水时序与 Workspace 复用

### 9.1 流水时序图（Core 0 视角）

```
时间 ──────────────────────────────────────────────────────────────────────>

Cube:   [nIdx=0]    [nIdx=8]    [nIdx=16]   [nIdx=24]   [G1 nIdx=0]  ...
         ↓Set0       ↓Set1       ↓Set2       ↓Set3        ↓Set0
         Slot0       Slot1       Slot2       Slot3        Slot0(复用)
                                              ↑Wait↑
Vector:      [nIdx=0]    [nIdx=8]    [nIdx=16]   [nIdx=24]  ...
              ↑Wait0      ↑Wait1      ↑Wait2      ↑Wait3
              ↓Set0       ↓Set1       ↓Set2       ↓Set3

标志同步:
  Cube → Vector: SetFlag(SYNC_AIC_TO_AIV)  每次 Iterate 后
  Vector → Cube: SetFlag(SYNC_AIV_TO_AIC)  每个 offsetN 循环结束后
  Cube 等待: 当 cubeCount >= parallNum(4) 时 WaitFlag(SYNC_AIV_TO_AIC)
```

### 9.2 Workspace Slot 生命周期

```
Slot 0 (offset 0):
  Cube 写入: cubeCount=0 (nIdx=0 of Group0)
  Vector 读取: 紧随其后
  Cube 可复用: cubeCount=4 (nIdx=0 of Group1), 需等 Vector 完成 Slot0

Slot 1 (offset 262144):
  Cube 写入: cubeCount=1 (nIdx=8 of Group0)
  Vector 读取: 紧随其后
  Cube 可复用: cubeCount=5 (nIdx=8 of Group1)

Slot 2 (offset 524288):
  Cube 写入: cubeCount=2
  复用: cubeCount=6

Slot 3 (offset 786432):
  Cube 写入: cubeCount=3
  复用: cubeCount=7
```

**PostCompute 阶段**：所有 Group 处理完毕后，Cube 需要等待最后 `min(cubeCount, parallNum)` 个未完成的 Vector 任务。

---

## 十、端到端数据流总图

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          GM (Global Memory)                              │
│                                                                           │
│  x[1024,1024]    weight[8,1024,8192]   scale[8,8192]   perTokenScale[1024]  y[1024,8192]
│  int8, 1MB       int8 NZ, 64MB        float, 256KB     float, 4KB       half, 16MB
│       │               │                    │                │               ↑
│       │    ┌──────────┘                    │                │               │
│       │    │                               │                │               │
└───────┼────┼───────────────────────────────┼────────────────┼───────────────┘
        │    │                               │                │
        ▼    ▼                               │                │
┌──────────────────────┐                     │                │
│  Cube (AIC) 矩阵乘   │                     │                │
│                      │                     │                │
│  逐Group逐Block:     │                     │                │
│  x[128,1024]         │                     │                │
│    × weight[1024,256]│                     │                │
│  → mmOut[128,256]    │                     │                │
│     int32            │                     │                │
│                      │                     │                │
│  每次Iterate写:      │                     │                │
│  32768 int32         │                     │                │
│  = 131,072 Bytes     │                     │                │
└──────────┬───────────┘                     │                │
           │                                 │                │
           ▼                                 │                │
┌──────────────────────┐                     │                │
│  Workspace (GM中转)   │                     │                │
│                      │                     │                │
│  4个Slot × 8个Core   │                     │                │
│  每个Slot/Core:      │                     │                │
│  128×256×4=131KB     │                     │                │
│  总计: 4MB           │                     │                │
│                      │                     │                │
│  环形复用,深度=4     │                     │                │
└──────────┬───────────┘                     │                │
           │                                 │                │
           ▼                                 ▼                ▼
┌──────────────────────────────────────────────────────────────────────────┐
│  Vector (AIV) — 每次 offsetM 迭代处理 vecBaseM=24 行                    │
│                                                                          │
│  ┌─────────────────────────────────────────────────────────────────┐    │
│  │  UB (195,584 Bytes 总计)                                        │    │
│  │                                                                  │    │
│  │  TQue:                                                           │    │
│  │   scaleInQueue:     2×1024B    ← scale[256] float               │    │
│  │   perTokenScaleQ:   2×512B     ← perTokenScale[128] float       │    │
│  │   vecInQueue:       2×24576B   ← mmOut[24,256] int32            │    │
│  │   vecOutQueue:      2×12288B   → y[24,256] half                 │    │
│  │                                                                  │    │
│  │  TBuf (tmpBuff, 118,784B):                                      │    │
│  │   dequantMiddleResult[6144] float  ← AscendDequant 输出          │    │
│  │   pertokenBrcbLocal[6144] float    ← BroadCast 输出              │    │
│  │   mulsResultLocal[6144] float      ← Mul 输出                    │    │
│  │   actResultLocal[6144] float       ← FasterGelu 输出             │    │
│  │   sharedTmpLocal[12288] float      ← API 临时空间(复用)          │    │
│  │                                                                  │    │
│  │  计算流水:                                                        │    │
│  │  int32 ──Dequant──> float ──Mul──> float ──Gelu──> float ──Cast──> half │
│  │  (6144)  ×scale    (6144)  ×pts   (6144)        (6144)      (6144)│    │
│  └─────────────────────────────────────────────────────────────────┘    │
└──────────────────────────────────────────────────────────────────────────┘
           │
           ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  y[1024, 8192] half, 16MB                                              │
│                                                                          │
│  逐块写入:                                                              │
│  Group i, (mIdx, nIdx), offsetM_vec:                                    │
│    yGm[(offsetM + mIdx×128 + offsetM_vec) × 8192 + nIdx×256 + offsetN] │
│                                                                          │
│  每个 Vector 迭代写入: 24行×256列 (或尾部8行×256列)                      │
│  每个 Cube 块写入: 128行×256列 (经6次Vector迭代)                        │
│  每个Group写入: 128行×8192列 (经32个nIdx块)                             │
│  全部8个Group: 1024行×8192列 = 完整输出                                  │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 十一、切分策略总结表

| 切分层级 | 维度 | 切分粒度 | 切分次数 | 说明 |
|---------|------|---------|---------|------|
| **Group** | M轴 | groupList[i]=128 | 8组 | 每组使用不同weight |
| **Cube Block** | M×N | baseM×baseN=128×256 | 每组1×32=32块 | M方向1块(128/128), N方向32块(8192/256) |
| **Vector offsetN** | N轴 | baseN=256 | 每块1次 | 与Cube N分块对齐 |
| **Vector offsetM** | M轴 | vecBaseM=24 | 每块6次 | UB容量决定: 6144/256=24行 |

### 一次完整传递的数据量

| 传递路径 | 单次大小 | 单次元素数 | 传递次数/块 | 说明 |
|---------|---------|-----------|-----------|------|
| x → Cube | 128×1024 int8 = 128KB | 131,072 | 1 | Cube按SetSingleShape读取 |
| weight → Cube | 1024×256 int8 = 256KB | 262,144 | 1 | NZ格式, Cube内部搬运 |
| Cube → workspace | 128×256 int32 = 128KB | 32,768 | 1 | Iterate+GetTensorC |
| workspace → UB | 24×256 int32 = 24KB | 6,144 | 6 | DataCopyPad2D |
| scale → UB | 256 float = 1KB | 256 | 1/offsetN | 每个offsetN循环1次 |
| perTokenScale → UB | 24 float = 96B | 24 | 6/offsetM | 每个offsetM循环1次 |
| UB → y(GM) | 24×256 half = 12KB | 6,144 | 6 | DataCopyPad2D |

---

## 十二、特殊场景：尾部块处理

当 M 或 N 不被 baseM/baseN 整除时，尾部块需要特殊处理：

### 12.1 N 方向尾部

若 N ≠ 32 × 256 = 8192（如 N=8000）：

```
nIdx=0..30: curSingleN = 256 (满块)
nIdx=31:   curSingleN = 8000 - 31×256 = 8000 - 7936 = 64 (尾部)

Vector 侧:
  curVecBaseN = 64 (实际列数)
  alignBaseN = Ceil(64, 8) × 8 = 64 (已对齐)

  DataCopyScale: 拷贝 64 个 float
  DataCopyPad2D: 拷贝 24×64 的 int32 (自动padding到64列对齐)
```

### 12.2 M 方向尾部

若某组 m=200（不被 baseM=128 整除）：

```
mIdx=0: curSingleM = 128 (满块), numBlocksM = ceil(200/128) = 2
mIdx=1: curSingleM = 200 - 128 = 72 (尾部)

Vector 侧:
  vecBaseM = 24
  offsetM=0:  curVecBaseM=24, 72/24=3次满迭代
  offsetM=72: 不存在(72刚好整除24)

  若 m=130:
  offsetM=0:  24行
  offsetM=24: 24行
  offsetM=48: 24行
  offsetM=72: 24行
  offsetM=96: 24行, 但 96+24=120 < 128, 继续
  offsetM=120: curVecBaseM=128-120=8 (尾部, 120+24≥128触发)
  offsetM=128: curSingleM=2, 128行已处理完
  ...
  offsetM=128: mIdx=1开始, curSingleM=2
  vecBaseM=2 < 24, 所以 vecBaseM=2
  offsetM=0: curVecBaseM=2, 仅1次迭代
```

---

## 十三、完整执行计数（本样例参数）

```
总 Group 数:                8
每组 M:                     128
每组 MN 分块数:              1 × 32 = 32
总 MN 分块数:                8 × 32 = 256
每核处理分块数:              256 / 8 = 32

每个分块:
  Cube Iterate 次数:         1  (128×256 在 baseM×baseN 内)
  Vector offsetN 循环:       1  (256 = baseN)
  Vector offsetM 循环:       6  (24+24+24+24+24+8 = 128行)

每核总 Vector 迭代:          32 × 1 × 6 = 192 次
每核总数据搬运量:
  workspace→UB: 192 × 24,576 B = 4.5 MB (int32)
  UB→y(GM):     192 × 12,288 B = 2.25 MB (half)
  scale→UB:     32 × 1,024 B   = 32 KB
  pts→UB+brc:   192 × 96 B     ≈ 18 KB
```
