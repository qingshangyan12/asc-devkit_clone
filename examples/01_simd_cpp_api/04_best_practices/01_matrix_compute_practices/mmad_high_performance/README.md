# MMAD 最佳实践样例

## 概述

本样例基于静态Tensor编程范式实现高性能矩阵乘法运算，通过 L1/L0 双缓冲 Ping-Pong、大包搬运、细粒度流水同步、UnitFlag 等多种优化手段，实现接近 Cube 峰值的计算性能。与基于高阶 API Matmul 的实现不同，本样例手动管理数据搬运和流水同步，展示从 GM → L1 → L0 → L0C → GM 全流水线的调优方法。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── mmad_high_performance
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── mmad.asc              // Ascend C样例实现
```

## 样例描述

- 计算公式：C = A * B
  - A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]
  - C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]

- 样例规格：

  <table border="2" align="center">
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="5" align="center">Matmul</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
  <tr><td align="center">A</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">B</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">true</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">-</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">mmad_custom</td></tr>
  </table>

## 样例实现

### 性能指标说明

| 指标 | 说明 |
|------|------|
| Task Duration(μs) | 整个任务执行的总时间，算子执行时间以该参数为准 |
| Block Num | 使用的核数（Block数量） |
| aicore_time(μs) | AI Core 的平均执行时间 |
| aic_mac_time(μs) | Cube 计算单元的执行时间 |
| aic_mac_ratio | Cube 计算单元的时间占比，反映计算单元利用率 |
| aic_mte1_time(μs) | MTE1（L1 到 L0A/L0B 搬运）的执行时间 |
| aic_mte1_ratio | MTE1 的时间占比，反映 L1 到 L0 的数据搬运压力 |
| aic_mte2_time(μs) | MTE2（GM 到 L1 搬运）的执行时间 |
| aic_mte2_ratio | MTE2 的时间占比，反映 GM 到 L1 的数据加载压力 |
| aic_fixpipe_time(μs) | FixPipe（L0C 到 GM 搬运）的执行时间 |
| aic_fixpipe_ratio | FixPipe 的时间占比，反映结果写回的访存压力 |


### 指令队列与流水并行原理

理解本样例的优化手段，首先需要了解 AI Core 内部的指令队列机制。

AI Core 内部包含多个独立的执行单元：Scalar（标量）、Cube（矩阵）、MTE1/MTE2/MTE3（搬运）等。Scalar 单元读取指令序列，将各类指令**发射**到对应执行单元的**指令队列**中，各单元从自己的队列中取指令**异步并行**执行，不同指令队列之间的独立性是实现流水并行的基础。


核心数据流路径：

```
GM ──(MTE2, DataCopy)──> L1 ──(MTE1, LoadData)──> L0A/L0B ──(Cube, Mmad)──> L0C ──(Fixpipe)──> GM
       DataCopyInA/B             DataLoadA/B                   Compute             CopyOut
```

### 核心特性

#### 1. L1/L0 双缓冲 Ping-Pong 布局

L1 和 L0 均采用 Ping-Pong 双缓冲，使得 DataCopyIn（GM→L1）、DataLoad（L1→L0）与 Compute 三个阶段形成三级流水，每个阶段处理不同缓冲区的数据，互不阻塞。

```
时间 ──────────────────────────────────────────────────────────────>

MTE2:  |─ A1 Ping ──|─ A1 Pong ──|─ A1 Ping ──| ...
MTE1:               |─ A2 Ping ──|─ A2 Pong ──|─ A2 Ping ──| ...
Cube:                            |─ Mmad ─────|─ Mmad ─────| ...
Fixpipe:                           |─ CopyOut ──|
```


**L1 双缓冲布局**：A1 占 L1 前半（0~256KB），B1 占 L1 后半（256~512KB），各自再分为 Ping/Pong 两块：

```
L1 (512KB):
├── A1 Ping: [0, 128KB)
├── A1 Pong: [128KB, 256KB)
├── B1 Ping: [256KB, 384KB)
└── B1 Pong: [384KB, 512KB)
```

**L0 双缓冲布局**：A2/B2 各占 L0 前后 32KB，各自再分为 Ping/Pong：

```
L0A/L0B (64KB):
├── A2 Ping: [0, 16Kb)
├── A2 Pong: [32KB, 48KB)
├── B2 Ping: [0, 32KB)
└── B2 Pong: [32KB, 64KB)
```

```cpp
// A1: L1 Ping/Pong
AscendC::LocalTensor<half> a1LocalPing(AscendC::TPosition::A1, 0, a1PingpongSize);
AscendC::LocalTensor<half> a1LocalPong(AscendC::TPosition::A1, a1PingpongSize * sizeof(half), a1PingpongSize);
// A2: L0 Ping/Pong
AscendC::LocalTensor<half> a2LocalPing(AscendC::TPosition::A2, 0, a2PingpongSize);
AscendC::LocalTensor<half> a2LocalPong(AscendC::TPosition::A2, L0_PINGPONG_BYTES, a2PingpongSize);
```

#### 2. 大包搬运

通过 `stepKa`/`stepKb` 参数将多个 baseK 块打包为一次 DataCopyIn 操作（称为"大包"），减少 MTE2 搬运次数。例如 `stepKa=8` 表示一次将 8 个 baseK 块从 GM 搬入 L1。


```cpp
// DataCopyInA: 一次搬入 stepKa 个 baseK 块
AscendC::Nd2NzParams nd2nzParams;
nd2nzParams.nValue = curM;
nd2nzParams.dValue = baseK * stepKa;  // 大包包含 stepKa 个 baseK
```

#### 3. 细粒度流水同步

使用四类硬件事件标志实现精确的流水线同步，分为正向同步（数据就绪通知）和反向同步（缓冲区释放通知）：

| 事件类型 | 方向 | 用途 | flag 编号 |
|---------|------|------|----------|
| MTE2_MTE1 | 正向 | L1 数据就绪通知，DataCopyIn 通知 DataLoad 可读取 | 0/1: A1 Ping/Pong; 2/3: B1 Ping/Pong |
| MTE1_MTE2 | 反向 | L1 缓冲区释放通知，DataLoad 通知 DataCopyIn 可覆盖 | 同上 |
| MTE1_M | 正向 | L0 数据就绪通知，DataLoad 通知 Compute 可计算 | mte1DBFlag (0/1 交替) |
| M_MTE1 | 反向 | L0 缓冲区释放通知，Compute 通知 DataLoad 可覆盖 | mte1DBFlag (0/1 交替) |

**反向同步需预置**：由于反向同步是"消费方 SetFlag → 生产方 WaitFlag"，首次使用前必须预置 SetFlag，否则首次 WaitFlag 会死锁：

```cpp
// 初始化：预置反向同步 flag，防止首次 WaitFlag 死锁
AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(0);  // A1 Ping 可写
AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(1);  // A1 Pong 可写
AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(2);  // B1 Ping 可写
AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(3);  // B1 Pong 可写
AscendC::SetFlag<AscendC::HardEvent::M_MTE1>(0);     // L0 Ping 可写
AscendC::SetFlag<AscendC::HardEvent::M_MTE1>(1);     // L0 Pong 可写
```

**大包粒度的正向同步**：DataLoad 从 L1 大包中逐个 baseK 读取数据，只需在读取大包首个 baseK 时等待数据就绪，后续 baseK 与首个在同一个大包内，无需重复等待：

```cpp
// 正向同步：仅大包首元素需等待数据就绪
if (kOffsetInChunkA == 0) {
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(a1ReadIdx);
}
// 反向同步：仅大包末元素消费完后通知 DataCopyIn 可覆盖
if ((kOffsetInChunkA + 1) == stepKa) {
    AscendC::SetFlag<AscendC::HardEvent::MTE1_MTE2>(a1ReadIdx);
}
```

#### 4. LoadData3D 替代 LoadData2D——减少指令队列占用

在 Atlas A2/A3 架构上，本样例使用 `LoadData3DParamsV2`（即 LoadData3D）替代 `LoadData2DParams`（即 LoadData2D）完成 L1→L0 的数据搬运。这是一个关键的指令队列优化。

**问题背景**：MTE1 指令队列深度为 32。使用 LoadData2D 时，由于单条 LoadData2D 指令搬运粒度有限，搬运一个 baseM×baseK 的切片需要用 for 循环发射多条 LoadData2D 指令。例如 baseM=128、baseK=64 时，最少需要发射 `baseK/16 = 4` 条 LoadData2D 指令。

> **注意**：Atlas A5 芯片提供了 `LoadData2DParamsV2` 接口，单条指令即可完成搬运，无需 LoadData3D。因此本样例通过 `__NPU_ARCH__` 条件编译区分两种架构的 LoadData 实现。

#### 5. 多核并行切分

按 M/N 方向均匀切分矩阵到多核并行计算，4×6 切分策略（M 方向 4 块、N 方向 6 块，共 24 核）满足地址 512B 对齐，并减少同地址访问冲突：

```cpp
constexpr uint32_t mIter = AscendC::DivCeil(M, singleCoreM);
uint32_t mIterIdx = get_block_idx() % mIter;
uint32_t nIterIdx = get_block_idx() / mIter;
```

#### 6. 常量 Tiling

所有 Tiling 参数（baseM/baseK/baseN、singleCoreM/K/N、stepKa/stepKb）通过模板参数在编译期确定，运行时无需 Scalar 动态计算，减少 Scalar 开销：

```cpp
template <uint32_t M, uint32_t K, uint32_t N, uint32_t baseM, uint32_t baseK, uint32_t baseN,
          uint32_t singleCoreM, uint32_t singleCoreK, uint32_t singleCoreN,
          uint32_t stepKa, uint32_t stepKb>
class KernelMmad { ... };
```

#### 7. UnitFlag 优化

开启 UnitFlag 后，MMAD 和 FIXPIPE 实现细粒度（512B）流水并行，而非指令级同步。每当 Cube 完成一个 512B 数据结果的计算，FIXPIPE 立即搬出该数据，Cube 计算与结果写回流水重叠：

```cpp
mmadParams.unitFlag = (kBlockIdx != kLoopCount - 1) ? 2 : 3;  // 开启 UnitFlag
```

- `unitFlag = 2`：中间 K 块，MMAD 计算结果不立即写回，而是等待下一个 512B 完成后流水搬出
- `unitFlag = 3`：最后一个 K 块，通知 FIXPIPE 将所有剩余结果写回 GM

#### 8. DataCopyIn 优先搬运 B 矩阵

当 `stepKa > stepKb` 时，B 矩阵每 `stepKb` 个 baseK 就需要切换到下一个 L1 缓冲区（Pong），而 A 矩阵需要 `stepKa` 个 baseK 才切换。因此 B 的数据需求更紧迫。如果先搬运 A，MTE2 流水被 A 占用，B 的搬运要等 A 搬完才能开始，导致需要 B 数据时 B 尚未就绪。

本样例在 Compute 之后触发 DataCopyIn 时，**先 B 后 A**，优先搬运更紧迫的 B 数据：

```
k=0:  Compute → DataCopyIn(B1 Pong) → DataCopyIn(A1 Pong)
                 ↑ MTE2 先搬 B        ↑ 再搬 A
k=stepKb:       需要 B1 Pong → 已就绪 ✓（B 已有 stepKb 轮时间搬运）
k=stepKa:       需要 A1 Pong → 已就绪 ✓（A 有 stepKa 轮时间搬运，更充裕）
```

#### 9. K 方向主循环完整流程

以 stepKa=8、stepKb=4 为例，展示一个完整 (mBlockIdx, nBlockIdx) 子块内 K 方向循环的详细执行流程：

```
预处理：
  SetFlag(MTE1_MTE2, 0/1/2/3)  // 预置反向同步：L1 Ping/Pong 均可写
  SetFlag(M_MTE1, 0/1)         // 预置反向同步：L0 Ping/Pong 均可写
  DataCopyIn(A1 Ping, k=0)     // 搬入首个 A 大包到 Ping
  DataCopyIn(B1 Ping, k=0)     // 搬入首个 B 大包到 Ping
  SetFlag(MTE2_MTE1, 0/2)      // 通知 A1/B1 Ping 数据就绪

K循环 kBlockIdx = 0, 1, ..., kLoopCount-1:
  ┌─ a1ReadIdx = (kBlockIdx / stepKa) % 2        // 当前读 L1 A 的 Ping/Pong
  │  b1ReadIdx = (kBlockIdx / stepKb) % 2        // 当前读 L1 B 的 Ping/Pong
  │  kOffsetInChunkA = kBlockIdx % stepKa        // 当前 baseK 在 A 大包内的偏移
  │  kOffsetInChunkB = kBlockIdx % stepKb        // 当前 baseK 在 B 大包内的偏移
  │
  │  WaitFlag(M_MTE1, mte1DBFlag)                // 等待上一轮 Compute 释放 L0
  │  if (kOffsetInChunkA == 0)
  │      WaitFlag(MTE2_MTE1, a1ReadIdx)          // 等待 L1 A 大包数据就绪（仅首元素）
  │  if (kOffsetInChunkB == 0)
  │      WaitFlag(MTE2_MTE1, b1ReadIdx + 2)      // 等待 L1 B 大包数据就绪（仅首元素）
  │
  │  DataLoadA(A1 → A2)                          // L1 → L0
  │  DataLoadB(B1 → B2)                          // L1 → L0
  │
  │  if (kOffsetInChunkA + 1 == stepKa)
  │      SetFlag(MTE1_MTE2, a1ReadIdx)           // A 大包末元素：通知 L1 A 可覆盖
  │  if (kOffsetInChunkB + 1 == stepKb)
  │      SetFlag(MTE1_MTE2, b1ReadIdx + 2)       // B 大包末元素：通知 L1 B 可覆盖
  │
  │  Compute(Mmad)                               // Cube 计算（M 指令）
  │  SetFlag(M_MTE1, mte1DBFlag)                 // 通知 L0 可覆盖
  │  mte1DBFlag ^= 1                             // 切换 L0 Ping/Pong
  │
  │  // DataCopyIn 放在 Compute 之后，先 B 后 A
  │  if (B大包末元素 && 还有B数据):
  │      WaitFlag(MTE1_MTE2, b1WriteIdx + 2)     // 等待 L1 B 缓冲区可写
  │      DataCopyInB(下一个 B 大包)               // GM → L1（MTE2 指令）
  │      SetFlag(MTE2_MTE1, b1WriteIdx + 2)      // 通知 L1 B 数据就绪
  │  if (A大包末元素 && 还有A数据):
  │      WaitFlag(MTE1_MTE2, a1WriteIdx)         // 等待 L1 A 缓冲区可写
  │      DataCopyInA(下一个 A 大包)               // GM → L1（MTE2 指令）
  │      SetFlag(MTE2_MTE1, a1WriteIdx)          // 通知 L1 A 数据就绪
  └─
```

**时序示意**（stepKa=8, stepKb=4）：

```
预取: DataCopyIn(A1 Ping) + DataCopyIn(B1 Ping)                     ← K循环前

k=0:  WaitFlag(A1Ping, B1Ping) → DataLoad → Compute → DataCopyIn(B1Pong) → DataCopyIn(A1Pong)
k=1:  DataLoad → Compute
k=2:  DataLoad → Compute
k=3:  DataLoad(释放B1Ping) → Compute → DataCopyIn(搬运B1Ping)

k=4:  WaitFlag(B1Pong) → DataLoad(B1Pong) → Compute
k=5:  DataLoad → Compute
k=6:  DataLoad → Compute
k=7:  DataLoad(释放A1Ping B1Pong) → Compute → DataCopyIn(B1Pong) → DataCopyIn(A1Ping)
k=8:  WaitFlag(A1Pong已就绪✓) → WaitFlag(B1Ping已就绪✓) → DataLoad → Compute 
...
```


### 性能数据分析

#### Atlas A2训练系列芯片性能数据

| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4160.64 | 24 | 3696.74 | 3086.294 | 0.835 | 318.442 | 0.087 | 2546.154 | 0.689 | 3555.022 | 0.962 | 163.7 | 0.045 |

除去启动开销，已达成该芯片 83.5% 的峰值算力。


#### Ascend 950PR芯片性能数据

| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 2572.047 | 32 | 2571.44 | 2564.813 | 0.997 | 144.604 | 0.056 | 828.001 | 0.322 | 1874.267 | 0.729 | 221.997 | 0.086 |

已达成该芯片 99.7% 的峰值算力。

### 理论性能分析

#### Cube 计算性能分析

**Atlas A2 训练系列芯片**：样例参数 M=N=K=8192，baseM=128，baseN=256，baseK=64，该计算芯片主频为 1.85GHz，每 cycle 处理 16×16×16 次乘加运算。

$$cube\_time = \frac{M \times N \times K}{16 \times 16 \times 16 \times core\_num \times cube\_freq} = \frac{8192 \times 8192 \times 8192}{16 \times 16 \times 16 \times 24 \times 1850} = 3022.92\mu s$$

Cube 计算耗时误差：

$$误差 = \frac{aic\_mac\_time - cube\_time}{cube\_time} = \frac{3086.294 - 3022.92}{3022.92} = 2.10\%$$


**Ascend 950PR 芯片**：样例参数 M=N=K=8192，baseM=256，baseN=256，baseK=64，该处理器主频为 1.65GHz，每 cycle 处理 16×16×16 次乘加运算。

$$cube\_time = \frac{M \times N \times K}{16 \times 16 \times 16 \times core\_num \times cube\_freq} = \frac{8192 \times 8192 \times 8192}{16 \times 16 \times 16 \times 32 \times 1650} = 2542.00\mu s$$

Cube 计算耗时误差：

$$误差 = \frac{aic\_mac\_time - cube\_time}{cube\_time} = \frac{2564.813 - 2542.00}{2542.00} = 0.90\%$$


#### MTE2 带宽分析

**读入数据总量**：

Atlas A2 训练系列芯片（baseM=128，baseN=256）：

$$读入数据总量 = \left(\frac{N}{baseN} \times M \times K + \frac{M}{baseM} \times K \times N\right) \times sizeof(half) = (32 \times 8192 \times 8192 + 64 \times 8192 \times 8192) \times 2B = 12GB$$

Ascend 950PR 芯片（baseM=256，baseN=256）：

$$读入数据总量 = \left(\frac{N}{baseN} \times M \times K + \frac{M}{baseM} \times K \times N\right) \times sizeof(half) = (32 \times 8192 \times 8192 + 32 \times 8192 \times 8192) \times 2B = 8GB$$

**MTE2 理论耗时**：

Atlas A2 训练系列芯片：L2Cache 峰值带宽约 5TB/s，HBM 带宽约 1.8TB/s。首次从 HBM 读入数据，后续从 L2Cache 读取。

$$第一次从HBM读入的数据总量 = M \times K \times sizeof(half) + K \times N \times sizeof(half) = 256MB$$

$$MTE2理论耗时 = \frac{HBM读入数据总量}{1.8TB/s} + \frac{L2Cache读入数据总量}{5TB/s}$$

MTE2 耗时误差：

$$MTE2耗时误差 = \frac{3555.022 - 2672.44}{2672.44} = 33.01\%$$

当前 MTE2 耗时与理论值相差较大，原因包括：实际芯片 L2Cache 大小为 192MB，L2Cache 切分策略影响命中率；用户可通过 L2Cache 切分策略以提高 MTE2 带宽。

Ascend 950PR 芯片：L2Cache 峰值带宽约 5TB/s，HBM 带宽约 1.6TB/s。

$$MTE2理论耗时 = \frac{HBM读入数据总量}{1.6TB/s} + \frac{L2Cache读入数据总量}{5TB/s}$$

MTE2 耗时误差：

$$MTE2耗时误差 = \frac{1874.267 - 1832.10}{1832.10} = 2.30\%$$

相比于 Atlas A2 训练系列芯片，Ascend 950PR 芯片数据搬运更为高效，MTE2 带宽利用率更高。



## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

- 默认路径，root 用户安装 CANN 软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非 root 用户安装 CANN 软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径 install_path，安装 CANN 软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行

```bash
mkdir -p build && cd build;                                          # 创建并进入 build 目录
cmake -DNPU_ARCH=dav-2201 ..;make -j;                                # 编译（默认 NPU 模式）
python3 ../scripts/gen_data.py                                       # 生成测试输入数据
./demo                                                               # 执行编译生成的可执行程序
python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # 验证输出结果是否正确
```

使用 NPU 仿真模式时，添加 `-DRUN_MODE=sim` 参数即可。

示例：
```bash
cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..;make -j;  # NPU 仿真模式
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

### 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU 仿真 |
| `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

执行结果如下，说明精度对比成功。
```bash
test pass!
```

### 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo   # 分析样例性能
```

当前目录下会生成 PROF_ 前缀的文件夹，`mindstudio_profiler_output` 目录保存 Host 和各个 Device 的性能数据汇总，性能数据分析推荐查看该目录下文件：
```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存 Host 和各个 Device 的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
