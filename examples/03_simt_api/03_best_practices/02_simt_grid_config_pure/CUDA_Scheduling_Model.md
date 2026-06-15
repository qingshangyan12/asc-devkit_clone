# CUDA 调度模型详解 ── GigaThread / Warp Scheduler / 指令发射

> 本文整理 CUDA 在硬件层面如何把 grid → block → warp 这套层级落到物理调度单元上，并对比 Ascend AIV 的 1 核 1 block 模型。所有引用均来自 NVIDIA 公开文档，链接放在文末。

## 目录

1. [两级调度架构总览](#一两级调度架构总览)
2. [第一层：GigaThread Engine（全局 block 调度器）](#二第一层gigathread-engine全局-block-调度器)
3. [第二层：Warp Scheduler（SM 内部 warp 调度器）](#三第二层warp-schedulersm-内部-warp-调度器)
4. [同 SM 多 block 驻留是谁决定的（occupancy）](#四同-sm-多-block-驻留是谁决定的occupancy)
5. [Warp 与"发射指令"是什么意思](#五warp-与发射指令是什么意思)
6. [发射动作的硬件分解](#五一发射动作的硬件分解)
7. [与 Ascend AIV 调度模型的差异](#六与-ascend-aiv-调度模型的差异)
8. [案例：Case 5 vs Case 6 的延迟掩盖](#七案例case-5-vs-case-6-的延迟掩盖)
9. [参考文档](#八参考文档)

---

## 一、两级调度架构总览

CUDA 的调度由两层硬件配合完成，**粒度完全不同**：

```
                    Kernel Launch (gridDim, blockDim)
                              │
                              ▼
                ┌──────────────────────────────┐
                │   GigaThread Engine (GTE)    │   ← 全局 block 调度器
                │   "compute work distributor" │     整张 GPU 一个
                └──────────────────────────────┘
                  │            │            │
                  ▼            ▼            ▼
                SM 0         SM 1   ...   SM N-1
            ┌─────────┐  ┌─────────┐  ┌─────────┐
            │ Warp     │  │ Warp     │  │ Warp     │  ← 每个 SM 4 个
            │Scheduler │  │Scheduler │  │Scheduler │     warp scheduler
            │   ×4     │  │   ×4     │  │   ×4     │     (Volta 起)
            └─────────┘  └─────────┘  └─────────┘
                  │            │            │
                  ▼            ▼            ▼
              已驻留的多个 block 的 warp 池
```

| 层 | 单元 | 粒度 | 频次 |
| --- | --- | --- | --- |
| 第一层 | GigaThread Engine（整张卡 1 个） | block | block 完成后才动一次 |
| 第二层 | Warp Scheduler（每 SM 4 个） | warp（32 线程） | 每个 cycle 都在选 |

---

## 二、第一层：GigaThread Engine（全局 block 调度器）

### 别名

NVIDIA 文档/白皮书里能看到几个等价名字：
- **GigaThread Engine (GTE)** — Fermi 之后的市场术语
- **Compute Work Distributor (CWD)** — 架构白皮书里的偏正式叫法
- **block scheduler** — 程序员侧的口语叫法

### 物理位置

是芯片级硬件单元，**不在任何一个 SM 里**，整张 GPU 共一个。

### 做什么

维护当前 grid 中所有 **未启动 block 的全局队列**，把 block 分发到具备容纳能力的 SM。

CUDA Programming Guide 原文：

> "the scheduler assigns thread blocks to SMs"
>
> "As long as SMs have sufficient hardware resources unoccupied by other thread blocks, the scheduler will continue assigning thread blocks to SMs."
>
> "If at some point no SM has the capacity to accept another thread block, the scheduler will wait. Once this happens, SMs are free to accept more work, and the scheduler assigns thread blocks to them."
>
> "no ordering guarantees are made by the scheduler"

### 触发与决策

- **触发条件**：某个 SM 报告"我还有资源空位" → GTE 立刻发一个 block 过去
- **不是 wave 同步**：不存在"等所有 SM 完成当前批 → 一起换下一批"的硬件屏障
- **per-SM 独立**：哪个 SM 先空，哪个先领新 block；block 的执行顺序与 `blockIdx` 无固定对应

### 重要推论：wave 是分析模型而非硬件机制

- "wave" = 一组同时驻留的 block 集合
- 程序员/profiler 用 wave 数估算端到端耗时是 OK 的
- 但 wave 之间没有屏障，所以 **实测 ≤ wave 数 × 单 block 耗时**（先空的 SM 会提前领下一波）

---

## 三、第二层：Warp Scheduler（SM 内部 warp 调度器）

### 物理位置

每个 SM 内部，**Volta（V100）开始固定为 4 个 warp scheduler**，分别管 SM 内部 4 个 sub-partition。

### 做什么

在 SM **已经驻留** 的所有 block 的 warp 集合里，**每个 cycle** 挑一个 ready 的 warp 把指令发射到执行单元。

### 调度粒度对比

| 项目 | GigaThread Engine | Warp Scheduler |
| --- | --- | --- |
| 处理对象 | block | warp（32 线程） |
| 频次 | block 完成后 | 每 cycle |
| 粒度 | 微秒级 | 时钟周期级 |
| 触发 | SM 资源释放 | warp ready |

### 选 warp 的依据（"warp eligibility"）

一个 warp 在某 cycle **eligible**（可被发射）的条件：

1. 操作数已就绪（不在等内存）
2. 不在等同步（`__syncthreads()`、`__syncwarp()` 等）
3. 不在等 mem barrier（global / shared memory fence）
4. 没有结构性资源冲突（执行单元当前 cycle 可用）

如果同一个 sub-partition 里有多个 eligible warp，warp scheduler 用 **round-robin 类的策略**（具体策略 NVIDIA 不公开）选一个发射。

### 这就是 latency hiding 的本质

CUDA 的核心设计哲学：**靠"切到另一个 warp"来藏内存/计算 latency**。

```
warp A: load → [stall 200 cycles] → use
warp B: load → [stall 200 cycles] → use
warp C: ...

每 cycle: scheduler 总能找到至少一个不在 stall 的 warp → pipe 永不空载
```

要做到这件事，SM 上必须 **同时驻留足够多的 warp** —— 这个比例就叫 **occupancy**（见下节）。

---

## 三·一、Sub-partition 与 Warp 池组织

前面提到"每个 SM 4 个 warp scheduler"。这一节解释这 4 个 scheduler 物理上是怎么组织的，以及多 block 的 warp 在 SM 内是怎么放置和共享的。

### Sub-partition 是什么

从 **Volta（V100）开始**，每个 SM 内部被硬件切成 4 个对等子单元。NVIDIA 文档里有几个等价名字：
- **sub-partition**（最常用）
- **processing block**（白皮书）
- **SM partition**

每个 sub-partition 是一个 **微缩版的 SM**，自己持有一整套调度+执行链路：

```
┌──── SM (整个) ─────────────────────────────────────────────────┐
│                                                                │
│  ┌─ Sub-partition 0 ──┐  ┌─ Sub-partition 1 ──┐                │
│  │ Warp Scheduler  ×1 │  │ Warp Scheduler  ×1 │                │
│  │ Dispatch Unit   ×1 │  │ Dispatch Unit   ×1 │                │
│  │ Register File 16K  │  │ Register File 16K  │                │
│  │ L0 I-Cache         │  │ L0 I-Cache         │                │
│  │ FP32 cores (32)    │  │ FP32 cores (32)    │                │
│  │ INT32 cores (16)   │  │ INT32 cores (16)   │                │
│  │ Tensor Core ×1     │  │ Tensor Core ×1     │                │
│  │ Warp Pool (16个)   │  │ Warp Pool (16个)   │                │
│  └────────────────────┘  └────────────────────┘                │
│                                                                │
│  ┌─ Sub-partition 2 ──┐  ┌─ Sub-partition 3 ──┐                │
│  │ ... 同上结构  ...   │  │ ... 同上结构  ...   │                │
│  └────────────────────┘  └────────────────────┘                │
│                                                                │
│  ┌────────────── 4 个 sub-partition 共享 ──────────────────┐    │
│  │  L1 Cache / Shared Memory（统一配置，H100 ~228 KB）        │    │
│  │  Texture Units                                            │    │
│  └───────────────────────────────────────────────────────────┘    │
└────────────────────────────────────────────────────────────────┘
```

### Per-sub-partition 私有的资源（4 份独立）

| 资源              | H100 每 sub-partition 持有 |
| ----------------- | -------------------------- |
| Warp Scheduler    | 1                          |
| Dispatch Unit     | 1                          |
| Register File     | 16,384 × 32-bit (= 16K)    |
| L0 I-Cache        | 1                          |
| FP32 Cores        | 32                         |
| INT32 Cores       | 16                         |
| Tensor Core       | 1                          |
| Warp Pool 容量    | 最多 16 warp               |

### 整个 SM 共享的资源

| 资源              | H100 整个 SM 持有 |
| ----------------- | ----------------- |
| L1 / Shared Memory | 228 KB            |
| Texture Units     | 4                 |

把整个 SM 加起来：
- 64 warp 上限 = 4 sub × 16 warp
- 65,536 × 32-bit 寄存器 = 4 sub × 16K
- 4 个 warp scheduler，**每 cycle 最多发射 4 条指令到 4 个不同执行单元**

### Warp 池：不是一个，是 4 个独立池

block 一旦进 SM，硬件会把它的 warp **静态分配** 到 4 个 sub-partition，一直绑死到 block 结束 —— **warp 不会在 sub-partition 间迁移**。

```
block N 进 SM     blockDim = 256 → 8 个 warp（W0~W7）
       │
       ▼
       静态分发（按 warp_id % 4 之类的策略）
       │
       ├── W0, W4 → Sub-partition 0
       ├── W1, W5 → Sub-partition 1
       ├── W2, W6 → Sub-partition 2
       └── W3, W7 → Sub-partition 3
```

> 具体分发策略 NVIDIA 不公开（可能是 `warp_id % 4` 或更复杂的 hash），但 **静态绑定** 这件事是确定的。

### 同 sub-partition 内，多 block 的 warp 共池

假设 occupancy 让 SM 上同时驻留 8 个 block，每个 block 8 warp：

```
Sub-partition 0 的 warp 池（最多 16 warp）：
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│B0W0 │B0W4 │B1W0 │B1W4 │B2W0 │B2W4 │B3W0 │B3W4 │
├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
│B4W0 │B4W4 │B5W0 │B5W4 │B6W0 │B6W4 │B7W0 │B7W4 │
└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
              ↑
     16 个 warp，来自 8 个不同 block
     Warp Scheduler 0 每 cycle 从这里挑一个 eligible warp 发射
```

**核心结论**：

- ✗ 整个 SM 的所有 warp **不**共享一个大池
- ✓ 每个 sub-partition 的 warp 池里 **多 block 的 warp 共存**
- ✓ Warp Scheduler 每 cycle 选 warp 时 **不区分 warp 来自哪个 block** —— 只看是否 eligible
- ✓ 4 个 sub-partition 互不干扰，并行选 warp，整 SM 峰值 **4 issue / cycle**

### 为什么这样设计

1. **解耦**：4 个 warp scheduler 互不干扰，每 cycle 各自独立选 warp
2. **决策空间小**：sub-partition 内只有 16 warp，扫一遍找 eligible 在 1 cycle 内能搞定；如果一个池子放 64 warp，调度逻辑会复杂得多
3. **寄存器文件分片访问**：4 份各 16K 的寄存器文件，每个只服务自己 sub-partition 的 warp，避免 4 路 read port 竞争
4. **隔离故障域**：某个 sub-partition 卡住不影响其他 3 个

### 重要推论：`__syncthreads()` 必须跨 sub-partition

block 的 warp 散落在 4 个 sub-partition 里，所以 `__syncthreads()` 不是 sub-partition 内的事，而是 **SM 级别的 barrier 硬件**做的（PTX 里的 `bar.sync` 指令对应一个 SM 共享的 barrier 计数器）。这就是为什么 `__syncthreads()` 比 `__syncwarp()` 贵 —— 前者要等 4 个 sub-partition 的所有相关 warp 都到齐，后者只在 1 个 warp 内同步。

### 对比 Ascend AIV

| 维度                       | CUDA SM                          | Ascend AIV |
| -------------------------- | -------------------------------- | ---------- |
| 物理切分子单元             | 4 个 sub-partition               | 无         |
| Warp Scheduler 数量        | 4 个（每 sub-partition 1 个）      | 无         |
| Warp Pool 数量             | 4 个独立池                       | 无         |
| 单 cycle 最多发射指令数    | 4（4 sub × 1）                    | pipe 内静态调度 |
| Warp 跨 sub-partition 迁移 | 不允许，静态绑定                 | N/A        |

Ascend 没有 sub-partition —— 1 核 1 block，没有"多 block warp 共池"需求；指令并行靠 vector + MTE2 pipe 在编译期排好，硬件不做 cycle 级 warp 选择。

---

## 四、同 SM 多 block 驻留是谁决定的（occupancy）

不是程序员主动配置，而是 **资源除法的结果**。

### SM 硬件容量上限（H100 SXM 举例）

| 资源 | 上限 |
| --- | --- |
| 最大 block 数 / SM | 32 |
| 最大 warp 数 / SM | 64 |
| 寄存器文件 / SM | 65,536 个 32-bit |
| Shared Memory / SM | 228 KB（可配置上限）|

### 实际驻留的 block 数 = 4 项里最严格的瓶颈

```
blocks_per_SM = min(
    max_blocks_per_SM_arch,                      // 32
    max_warps_per_SM      / warps_per_block,     // 64 / N
    max_regs_per_SM       / regs_per_block,      // 65536 / R
    max_shmem_per_SM      / shmem_per_block      // 228KB / S
)
```

### 例子

| 配置 (block 大小, 每 thread 寄存器, shmem/block) | 4 项各能放                                                | 实际 |
| ---------------------------------------------- | --------------------------------------------------------- | ---- |
| 256 thr, 32 reg, 0 shmem                       | arch:32, warp:64/8=8, reg:65536/(256·32)=8, shm:∞         | **8** |
| 1024 thr, 64 reg, 16 KB shmem                  | arch:32, warp:64/32=2, reg:65536/(1024·64)=1, shm:228/16=14 | **1** |
| 128 thr, 24 reg, 0 shmem                       | arch:32, warp:64/4=16, reg:65536/(128·24)=21, shm:∞       | **16** |

### Occupancy 的意义

**occupancy = 实际驻留 warp 数 / SM 最大 warp 数**

- occupancy 高 → warp scheduler 选择多 → latency hiding 强
- 但 100% occupancy ≠ 性能最优；有时 *较低* occupancy 配 *更多寄存器* 会更快（少 spill）

NVIDIA 提供了 [Occupancy Calculator](https://docs.nvidia.com/cuda/cuda-occupancy-calculator/index.html) 帮助评估。

### 程序员的杠杆

- `__launch_bounds__(maxThreadsPerBlock, minBlocksPerSM)` 给编译器寄存器分配的 hint
- `-maxrregcount=N` 编译选项强制限制单线程寄存器数
- 调整 blockDim、shmem 大小

---

## 五、Warp 与"发射指令"是什么意思

### 什么是 warp

> "The multiprocessor creates, manages, schedules, and executes threads in groups of 32 parallel threads called *warps*."  ── CUDA Programming Guide

- **32 个连续的线程被打包成 1 个 warp**（这是硬件常量，自 G80 起从未变过）
- 同一个 warp 里的 32 条线程 **总是一起被调度** —— 共享同一个程序计数器（PC）
- Warp 是 SIMT（Single-Instruction Multiple-Thread）执行模型的基本单元

### 32 个线程怎么共享一条指令？

CUDA 的 **SIMT 执行模型**：

```
warp 内 32 个线程 = 32 条 lane = 32 套寄存器/状态
                     ↓
              共享同一个 PC、同一条 fetch 出来的指令
                     ↓
        每条 lane 拿这条指令在自己的寄存器上独立执行
```

### "发射指令"是什么意思

每个 cycle，warp scheduler 选中一个 ready 的 warp 后做的事：

1. **Fetch**：根据该 warp 的 PC 取一条指令
2. **Decode**：解码成微操作
3. **Issue（发射）**：把这条指令推到执行单元，**让 32 条 lane 同时跑**
4. PC 推进到下一条指令

所以 "warp 发射一条指令" = "32 条 lane 在同一 cycle 跑了同一条指令"。

### 一个例子

```cpp
c[tid] = a[tid] + b[tid];   // tid = threadIdx.x，假设是 warp 内 lane id
```

编译后大致是：

```
LDG.E R1, [a + tid*4]     // load a[tid] from global memory
LDG.E R2, [b + tid*4]     // load b[tid]
IADD R3, R1, R2           // add
STG.E [c + tid*4], R3     // store c[tid]
```

发射时：

| Cycle | 发射的指令 | 32 条 lane 同时做的事 |
| ----- | ---------- | --------------------- |
| t     | `LDG.E R1, [a + tid*4]` | 32 条 lane 各自从 `a[0]`...`a[31]` 加载到自己的 R1 |
| t+1   | `LDG.E R2, ...` | 32 条 lane 各自加载 `b[0..31]` 到 R2 |
| t+200 | `IADD R3, R1, R2` | （等到 load 回来）32 条 lane 各自做加法 |
| t+201 | `STG.E ...` | 32 条 lane 各自存回 `c[0..31]` |

注意：**`LDG` 之后的 ~200 cycle stall 期间**，warp scheduler 会切到其他 warp 发射 —— 这就是 latency hiding。

### Warp Divergence（分支发散）

如果 warp 内 32 条 lane 执行不同分支：

```cpp
if (tid % 2 == 0) {
    A();
} else {
    B();
}
```

硬件会 **串行化执行**：

```
Cycle t:    发射 A() 的指令，偶数 lane active，奇数 lane mask 掉
Cycle t+k:  发射 B() 的指令，奇数 lane active，偶数 lane mask 掉
```

- Volta 之前：warp 必须整体走完一条分支再走另一条 → 分支严重时性能砍半
- Volta 起引入 **Independent Thread Scheduling**：每条 lane 有自己的 PC，能在两条分支间细粒度交错（但仍是同 warp 串行执行不同 lane mask 的指令）

### Warp 与 block / SM 的关系

```
1 block = ⌈ blockDim / 32 ⌉ 个 warp（不足 32 的最后一个 warp 仍占 32 lane，多余 lane mask 掉）
1 SM   = 同时驻留多个 block 的 warp（最多 64 warp / SM, H100）
4 warp scheduler / SM, 各管 SM 的一个 sub-partition
```

举例：blockDim = 256 → 1 block = 8 warp；如果 occupancy 让 SM 上同时驻留 8 个 block → SM 共 64 warp ready，4 个 warp scheduler 每 cycle 各选一个发射，吞吐 = 4 warp/cycle = 128 ops/cycle。

---

## 五·一、"发射"动作的硬件分解

前面把"发射指令"当成一个抽象动作。这一节把它落到具体硬件单元和 cycle 级时序上。

### 谁在发射？—— Dispatch Unit

每个 SM 的 4 个 sub-partition 里都有这套硬件：

```
┌─── SM Sub-partition (×4 per SM) ─────────────────────────────┐
│                                                              │
│   ┌───────────────┐   ┌───────────────┐                      │
│   │ L0 I-Cache    │──▶│ Warp Scheduler│  ← 选哪个 warp        │
│   │ (指令缓存)     │   │ (选 eligible) │                      │
│   └───────────────┘   └───────┬───────┘                      │
│                               │                              │
│                               ▼                              │
│                       ┌───────────────┐                      │
│                       │ Dispatch Unit │  ← "发射器"           │
│                       │ (issue 1/cyc) │                      │
│                       └───────┬───────┘                      │
│                               │                              │
│                ┌──────────────┼──────────────┐               │
│                ▼              ▼              ▼               │
│         ┌────────────┐ ┌──────────┐ ┌──────────────┐         │
│         │Register    │ │Operand   │ │Active Mask   │         │
│         │File (16K)  │ │Collector │ │(per-warp)    │         │
│         └─────┬──────┘ └────┬─────┘ └──────┬───────┘         │
│               └─────────────┼──────────────┘                 │
│                             ▼                                │
│   ┌──────┬──────┬──────┬──────┬──────┬──────┐                │
│   │ FP32 │ INT32│ FP64 │Tensor│ LSU  │ SFU  │ ← 执行单元      │
│   │ Cores│ Cores│ Cores│ Core │      │      │                │
│   └──────┴──────┴──────┴──────┴──────┴──────┘                │
└──────────────────────────────────────────────────────────────┘
```

**关键人物分工**（就这一个 cycle 的事）：

| 步骤 | 执行者                      | 时长       | 做的事                                          |
| ---- | --------------------------- | ---------- | ----------------------------------------------- |
| 1    | **Warp Scheduler**          | 1 cycle    | 从 sub-partition 的 warp 池里选一个 eligible warp  |
| 2    | **Dispatch Unit**（发射器） | 1 cycle    | 拿到该 warp 解码好的指令，**路由到对应执行单元**   |
| 3    | **Operand Collector**       | 1~几 cycle | 从 16K 寄存器文件读出 32 lane 的操作数              |
| 4    | **执行单元（FP32/MMA/LSU…）** | 视指令而定 | 32 lane 同步在 functional unit 上执行             |
| 5    | **Writeback**               | 1 cycle    | 32 lane 的结果写回寄存器文件                       |

**所以："发射" = Dispatch Unit 在 cycle T 把一条指令推给某个执行单元的动作。**

### 发射这个 cycle 具体做了什么 — 6 步

以 `IADD R3, R1, R2`（warp 内 32 lane 各自做 R3 = R1 + R2）为例：

**Step 1 — 选 warp（Warp Scheduler）**

sub-partition 持有的 warp 池里（H100 上每个 sub-partition 最多 16 warp），扫一遍找出 eligible warp（操作数 ready、不在 stall），按内部策略（round-robin + age）选一个。**这一步选出的是 warp ID，不是指令本身。**

**Step 2 — 取已解码的指令**

被选中的 warp 有它自己的 PC。指令在前置流水（Fetch + Decode）里已经准备好放在 instruction buffer。Dispatch Unit 从 buffer 拿出 `IADD R3, R1, R2`。

**Step 3 — 路由到执行单元（Dispatch Unit 核心动作）**

Dispatch Unit 看到这是个整数加法指令，按 opcode 决定送给哪个执行单元：

| 指令类型           | 路由目标                |
| ------------------ | ----------------------- |
| `FADD/FMUL/FFMA`   | FP32 cores              |
| `IADD/IMUL`        | INT32 cores             |
| `DADD/DFMA`        | FP64 cores              |
| `HMMA/IMMA`        | Tensor Core             |
| `LDG/STG/LDS/STS`  | Load/Store Unit (LSU)   |
| `RSQRT/EXP/LOG`    | Special Function Unit (SFU) |

每个 sub-partition 这一 cycle 只能 **发射 1 条指令到 1 个执行单元**（"1 issue / cycle"）。但 4 个 sub-partition 各自独立发射，整个 SM 一 cycle 可发射 4 条指令到不同执行单元。

**Step 4 — 读操作数（Operand Collector）**

Operand Collector 并行读 32 lane 的 R1、R2 寄存器：
- R1 → 32 个值（lane 0 的 R1, lane 1 的 R1, …, lane 31 的 R1）
- R2 → 32 个值
- 寄存器文件按 bank 组织，operand collector 处理 bank conflict、把 3 个源/目标操作数排到不同 cycle

**Step 5 — 32 lane 在执行单元同步执行**

执行单元（这里是 INT32 core 阵列）有 16 个物理 ALU。warp 的 32 lane 分两个 half-warp 各跑一 cycle：

```
cycle T  : lane  0..15 在 16 个 INT ALU 上同步执行 ADD
cycle T+1: lane 16..31 在 16 个 INT ALU 上同步执行 ADD
```

具体 issue rate / 跑完 cycle 数依架构和指令而定（Hopper 在 FP32 上一 cycle 就把 32 lane 全发，因为 FP32 core 数量足够）。

**Step 6 — Active Mask 控制哪些 lane 真生效**

每个 warp 有一个 32-bit active mask，标记此 cycle 哪些 lane 该 commit 结果：

- 正常情况：mask = 0xFFFFFFFF，32 lane 全 active
- divergence：if/else 分支只 active 一半 lane，另一半 mask 掉，但 ALU 资源照样占
- predicate 指令（`@P0 IADD ...`）：mask 是 P0 寄存器的 32 lane 值

mask 掉的 lane 结果丢弃、不写回寄存器，但占用执行单元那一 cycle —— 这就是 warp divergence 性能损失的来源。

### 关键观察

1. **"发射"是 1 cycle 的动作**，但指令的完整执行可能要几十~几百 cycle（特别是 LDG/STG）
2. **发射后 warp scheduler 立刻可以选下一个 warp**，不必等指令完成 —— 这就是 latency hiding 的实现
3. **每个 sub-partition 独立发射**，互不干扰；4 个 sub-partition × 1 issue/cycle = SM 峰值 4 issue/cycle
4. **stall 的 warp 不参与选择**，只要 sub-partition 至少有 1 个 eligible warp，发射器就不空载

### 一个完整的时序图

假设 sub-partition 有 4 个 warp：W0、W1、W2、W3。

```
cycle  W0          W1            W2           W3          Dispatch到
─────────────────────────────────────────────────────────────────────
  0    LDG R1 ─┐                                          LSU (W0)
  1    [stall]  FADD R5 ─┐                                FP32 (W1)
  2    [stall]  [stall]   IMAD R8 ─┐                      INT32 (W2)
  3    [stall]  [stall]   [stall]   FFMA R10 ─┐           FP32 (W3)
  4    [stall]  FADD R6 ─┐ [stall]   [stall]              FP32 (W1)
  5    [stall]  [stall]   IMAD R9 ─┐ [stall]              INT32 (W2)
  ...
 200   R1 ready, IADD R2─┐ ...                            INT32 (W0)
                                                          ↑
                                       Dispatch Unit 每 cycle
                                       挑一个 ready warp 发射
```

LDG 在 cycle 0 发射后 stall 200 cycle，但这 200 cycle 里 Dispatch Unit 不闲着 —— 一直在发射 W1/W2/W3 的指令。**这就是 GPU 把内存延迟"藏"起来的物理过程。**

---

## 六、与 Ascend AIV 调度模型的差异

| 维度 | CUDA SM | Ascend AIV |
| --- | --- | --- |
| 全局 block 调度器 | GigaThread Engine | 类似，但官方文档未做对等命名 |
| **每物理单元驻留 block 数** | 1~32（资源决定，叫 occupancy） | **恒为 1** |
| 单元内细粒度调度 | 4 个 warp scheduler，每 cycle 选 warp | 不需要（只有 1 block） |
| 隐藏延迟方式 | 多 warp 切换（thread-level parallelism） | vector pipe + MTE2 流水（instruction-level parallelism） |
| occupancy 概念 | 关键调优指标 | 不存在 |
| 单 wave 容量 | `SM 数 × 每 SM 最大并发 block`（依赖 kernel 资源用量） | **物理核数**（恒为常量，本平台 64） |
| 调优方向 | gridDim **远大于** SM 数（让 GTE 有充足队列填补 tail） | gridDim **等于** 物理核数（避免多 wave 串行） |

### 设计哲学的差异

- **CUDA**：让 SM 驻留多个 block，**A block 等内存的时候 SM 切到 B block 的 warp 去发射** → 用 *thread-level parallelism* 藏延迟。代价是硬件复杂、occupancy 调优负担落到程序员。
- **Ascend AIV**：1 核 1 block，没有 occupancy 概念。靠 **pipe 内并行**（vector + MTE2 在同 cycle 跑不同 stage）藏延迟。控制路径简单，但要求 block 内的 stride loop 能填满 pipe → 这就是为什么我们样例里要把每核工作集做大。

### 为什么 Ascend 没有 warp 调度器

因为 1 AIV 1 block，没有"多 block 切换"的需求，warp 调度退化为单 block 内的指令发射，由编译器静态调度（结合 vector + MTE2 的 pipe 并行）即可。这其实是与 CUDA 截然不同的简化路径。

---

## 七、案例：Case 5 vs Case 6 的延迟掩盖

把前面所有概念落到本目录 `grid_config.asc` 里的 Case 5（1024 核，每核 1024 元素）vs Case 6（64 核，每核 16384 元素）上。两个 case 的核函数完全一样，GM 读写总量也完全一样（都是 1M 次 gather + 1M 次 store），但性能差 ~3.18×。差异来自两层：调度开销 + 访存延迟掩盖。本节专门讲后者。

### 先把概念分清

| | 含义 | 是否可减少 |
| --- | --- | --- |
| **延迟 (latency)** | 一次 GM load 从发出到数据返回的 cycle 数 | 不可，物理决定，几百 cycle |
| **吞吐 (throughput)** | 单位时间能完成多少次 GM load | 可，只要不停往 GM 通道塞请求，每 cycle 都能有 load 完成 |

**掩盖延迟的本质** = 让线程在等数据回来的同时去做别的事（包括"发出下一个 load"），让多个 load 的延迟之间互相重叠，最终只有第 1 个 load 的延迟被完整暴露在关键路径上。GM 延迟一个 cycle 都没减少，**减少的是"延迟在关键路径上出现的次数"**。

### 谁在发指令？回顾 SIMT 模型

回顾本文 §五：

- **warp 内 lockstep**：32（或 64，依架构）条线程共享一个 PC，warp scheduler 发一条指令，warp 内所有 lane 同时各自执行——`load input[gather_idx]` 这一条指令立刻产生几十个并发 GM 请求
- **warp 间异步**：每个 warp 有自己的 PC、自己的寄存器、自己的 stall 状态；scheduler 每 cycle 在 sub-partition 的 warp 池里挑一个 ready 的发指令
- **stall 不会卡住整个 SM**：当一个 warp 因为等 GM 数据而 stall 时，scheduler 立刻切到另一个 ready warp 继续发指令，硬件单元不空转

所以"thread 0 在等 load(0) 时能不能继续发 load(1024)"这个问题要分两层回答：

1. **同 warp 内**：不能。thread 0 不是自己决定发什么指令的，warp 内 32 lane 共享 PC，由 warp scheduler 统一发射。
2. **跨循环迭代（编译器视角）**：可以。如果循环迭代之间无依赖，编译器可以做软件流水/循环展开，把多个迭代的 load 提前发出去，进入稳态后多个 load 同时挂在 GM 通道上。这就是 ILP。
3. **跨 warp（scheduler 视角）**：当然可以，而且这才是主力。当 warp 0 stall 时，scheduler 切到 warp 1 发它的 load——同一时刻 GM 上就有两个不同 warp 的 load 在飞。这就是 TLP。

### Case 5 的时间轴（每线程 1 次循环）

约定符号：`L` = 发 GM load，`·` = warp 在等数据，`+` = add 1.0，`S` = 写 UB。只画 4 个 warp（实际一个 block 16 个 warp，规律相同）。

```
time →
                    ←────── GM latency (~几百 cycle) ──────→
warp 0:  L · · · · · · · · · · · · · · · · · · · · · · · + S  done
warp 1:    L · · · · · · · · · · · · · · · · · · · · · · + S  done
warp 2:      L · · · · · · · · · · · · · · · · · · · · · + S  done
warp 3:        L · · · · · · · · · · · · · · · · · · · · + S  done
         ↑↑↑↑                                          ↑
         scheduler 把 4 个 warp 的 L 全发出去             数据一起回来
         之后所有 warp 都在 stall                       → 一起算 → 一起退出
         scheduler 没有 ready warp 可挑，硬件空转
```

整个 kernel 时间 ≈ **GM latency + 一点点 compute**。延迟完全裸露在关键路径上。

### Case 6 的时间轴（每线程 16 次循环）

```
time →
         ← cold start：和 Case 5 一样 →     ← 进入稳态：GM 通道一直在飞 load →
warp 0:  L₀ · · · · · · · · · · · · · · +₀S₀ L₁ · · · · +₁S₁ L₂ · · +₂S₂ L₃ · ...
warp 1:    L₀ · · · · · · · · · · · · · · +₀S₀ L₁ · · · · +₁S₁ L₂ · · +₂S₂ L₃ ...
warp 2:      L₀ · · · · · · · · · · · · · · +₀S₀ L₁ · · · · +₁S₁ L₂ · · +₂S₂ ...
warp 3:        L₀ · · · · · · · · · · · · · · +₀S₀ L₁ · · · · +₁S₁ L₂ · · +₂ ...
         └────── 第一次仍要等 ──────┘   ↑
                                       到这里 warp 0 数据回来
                                       scheduler 立刻发 +₀S₀ → L₁
                                       L₁ 在飞的同时，+₀S₀ 在 vec pipe 上算
                                       L₂ 在飞的同时，+₁S₁ 在算
                                       scheduler 永远能找到 ready 的 warp
```

关键点在右侧稳态那段：

1. **同一 warp 内（ILP）**：warp 0 发完 L₁ 后又开始等 GM，但它后面会接着处理 +₁S₁、L₂——L₁ 的等待时间被自己后续迭代里"非访存的 compute 部分"覆盖
2. **warp 之间（TLP，主力）**：warp 0 stall 等 L₁ 时，scheduler 切到 warp 1 发 +₀S₀；warp 1 stall 等 L₁ 时切到 warp 2……scheduler 始终有事可发
3. **GM 通道**：稳态下永远有几十上百个 load 同时在飞，吞吐打满，每个 load 的延迟之间互相重叠

### 数据上的直接验证

| | aiv_total_cycles | 元素数 | cycle/元素 |
| --- | --- | --- | --- |
| Case 5 | 4310.9 | 1024 | **4.21** |
| Case 6 | 51750.9 | 16384 | **3.16** |

工作量是 16×，但 cycles 只涨 12×。多出来的 4× 没有花掉，就是因为每个元素的"等 GM"那段时间被压缩了，单元素摊销成本下降约 25%。如果延迟完全掩盖不住，cycles 应该严格按 16× 等比放大；现在只放大 12×，说明 stride 循环把 GM 延迟"埋"进了流水线里。

### 一句话对照

| | 关键路径上 GM 等待次数 | 摊销延迟成本 |
| --- | --- | --- |
| Case 5 | 1 次（每条线程的唯一一次 load） | 1 × latency / 1 元素 = 完整暴露 |
| Case 6 | 1 次（只有 cold start 那一次裸露） | 1 × latency / 16 元素 ≈ 1/16 摊到每元素 |

### 与 CUDA 模型的差异

CUDA SM 上有 4 个 warp scheduler、最多 64 warp 共池供调度切换，TLP 是延迟掩盖的主力机制。Ascend AIV 是 1 核 1 block，没有跨 block 的 warp scheduler，但 block 内的 1024 线程仍然按 SIMT 组织、依然有 warp 概念，scheduler 在同一 block 内的 warp 之间切换以掩盖延迟。**所以 Case 6 的优化思路对应到 CUDA 语境就是"扩大 work-per-thread 让循环窗口足够长，给 scheduler 留出足够多 ready warp 来切"**——和 CUDA 上"提高 occupancy + 增大 stride loop"的优化经验是一回事。

---

## 八、参考文档

### 官方文档

- [CUDA Programming Guide §2.3.7 — Writing CUDA SIMT Kernels](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html)（block 调度行为原文）
- [CUDA Programming Guide §3.2 — Advanced Kernel Programming](https://docs.nvidia.com/cuda/cuda-programming-guide/03-advanced/advanced-kernel-programming.html)
- [CUDA C Programming Guide — Hardware Implementation](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html)（warp / SIMT 模型）
- [GPU Performance Background User's Guide — Wave & Wave Quantization](https://docs.nvidia.com/deeplearning/performance/dl-performance-gpu-background/index.html)
- [Nsight Compute Profiling Guide — Occupancy & Warp States](https://docs.nvidia.com/nsight-compute/ProfilingGuide/index.html)

### 架构白皮书（含 GigaThread / Warp Scheduler 图）

- [Volta Tuning Guide](https://docs.nvidia.com/cuda/volta-tuning-guide/index.html)
- [Ada Tuning Guide](https://docs.nvidia.com/cuda/ada-tuning-guide/index.html)
- [Hopper Tuning Guide](https://docs.nvidia.com/cuda/hopper-tuning-guide/index.html)

### 工具

- [CUDA Occupancy Calculator](https://docs.nvidia.com/cuda/cuda-occupancy-calculator/index.html)
