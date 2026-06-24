# Softmax 性能调优样例

## 概述

本样例以单核Softmax为例，展示从MemBase到RegBase、从基础循环到融合展开的完整性能调优路径，包含Case 0-6共七个逐步优化的版本。

**优化路径**：
- Case 0 → Case 1：循环拆分（MemBase）
- Case 1 → Case 2：MemBase → RegBase API（寄存器级计算）
- Case 2 → Case 3：循环融合 + ExpSub融合指令 + UpdateMask尾块处理
- Case 3 → Case 4：外层循环展开（UpdateMask模式）
- Case 3 → Case 5：主尾块模式（main-tail block）
- Case 5 → Case 6：主尾块模式 + 外层展开 + ExpSub融合指令

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── softmax_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── build.sh                // 一键编译&验证脚本（支持单版本和批量）
│   ├── data_utils.h            // 数据读入写出函数
│   ├── softmax.asc             // Ascend C样例实现（Case 0-6七个版本）
│   └── README.md               // 样例说明文档
```

## 理论背景

本节介绍Softmax性能调优涉及的核心硬件原理与编程模型，为后续各Case的优化策略提供理论基础。

### 为什么选择RegBase — Memory矢量计算的瓶颈

在传统的SIMD矢量编程（MemBase）中，矢量计算单元的操作数直接落在LocalMemory（Unified Buffer，UB）上。每一条矢量计算的执行都隐含"从UB读 → 在执行单元算 → 写回UB"三个步骤。当一个算子由多个矢量计算串联组成时，每一步的中间结果都必须先写回UB，再被下一条指令重新读出。

这种模式存在三个典型瓶颈：

- **UB读写带宽被反复占用**：中间结果在UB上往返，访存压力随矢量计算数量增加而线性放大。
- **UB Bank冲突严重**：对UB的反复读写，极大增加了UB Bank冲突的概率。
- **关键路径被访存拉长**：当计算指令执行时间很短，整体耗时会被UB读写时间占据。

Softmax的每一行需要依次执行ReduceMax、Duplicate、Sub、Exp、ReduceSum、Duplicate、Div共7步操作，在MemBase模式下每行产生14次UB访问（7次Load + 7次Store），128行合计产生1792次UB访问，UB带宽成为性能天花板。

NPU架构版本3510在硬件中新增了**SIMD Register File**（寄存器）这一级存储。RegBase的核心思想是：将一段连续的矢量计算保留在寄存器中完成，仅在进入和退出这段计算时与UB交互一次，中间结果可不再回写UB。

| 维度 | Memory矢量计算 | Reg矢量计算 |
| --- | --- | --- |
| 数据位置 | UB | SIMD Registers |
| 中间结果 | 必须回写UB | 可在寄存器中连续消费 |
| 使用成本 | 较低 | 较高（需理解寄存器操作） |
| 适用目标 | 快速实现 | 追求极致性能 |

### 内存层级与寄存器类型

AIV核内的内存层级为 **GM → UB → Register**。寄存器不支持从GM直接加载数据或将数据直接写出到GM，必须经过UB中转。寄存器无法按索引或偏移访问单个元素，根据功能分为五类：

| 寄存器类型 | C API类型 | 基础API类型 | 宽度 (3510) | 用途 |
| --- | --- | --- | --- | --- |
| 矢量数据寄存器 | `vector_float` / `vector_half` 等 | `RegTensor<T>` | VL = 256B | 参与矢量计算的主要寄存器 |
| 掩码寄存器 | `vector_bool` | `MaskReg` | VL/8 = 32B | 控制有效元素，承载比较结果 |
| 地址寄存器 | `iter_reg` | `AddrReg` | 32bits | 保存UB地址偏移，支持最多四维寻址 |
| 搬入非对齐寄存器 | `vector_load_unalign` | `UnalignRegForLoad` | DataBlock = 32B | 非对齐搬入临时缓存 |
| 搬出非对齐寄存器 | `vector_store_unalign` | `UnalignRegForStore` | DataBlock = 32B | 非对齐搬出临时缓存 |

Vector Length (VL) = 256B，对于 `float` 类型单寄存器可容纳 64 个元素，对于 `half` 类型可容纳 128 个元素。

### VF函数执行域

Reg矢量计算通过三个执行域标签实现编译期隔离：

| 标签 | 角色 | 调用方 | 可调用对象 |
| --- | --- | --- | --- |
| `__aicore__` | Device侧普通函数 | 核函数或其他 `__aicore__` | `__aicore__` 或通过 `asc_vf_call` 调用VF |
| `__simd_vf__` | VF函数 | `__aicore__` 通过 `asc_vf_call` | `__simd_callee__` 修饰的API或子函数 |
| `__simd_callee__` | VF内部子函数 | `__simd_vf__` 或其他 `__simd_callee__` | `__simd_callee__` |

编译期按标签校验调用合法性。`__aicore__` 负责外层GM数据搬运和地址计算，`__simd_vf__` 专注寄存器级 Load→Compute→Store。VF函数内的标量计算在Aux Scalar单元执行，VF函数外的标量计算在Main Scalar单元执行，两者硬件独立。

### Hardware Loop编码规范

VF中的循环如果满足硬件约束，编译器可将其转换为Hardware Loop（硬件级循环），消除软件循环中的计数、比较和跳转开销。无法识别的循环退化为Software Loop，性能显著降低。

Hardware Loop编码规范：

| 规范项 | 要求 |
| --- | --- |
| 迭代变量类型 | 必须为 `uint16_t` |
| 起始值 | 从 0 开始 |
| 步长 | 递增 1 |
| 循环体内跳转 | 不允许 `if/else`、三元运算符 `?:` 等条件跳转 |
| 循环边界 | 执行过程中不被修改 |
| 嵌套层数 | 最多四层嵌套循环 |

对于必须出现的条件分支，使用编译时分支 `if constexpr`（无运行时开销）或 `for(1)` 模式替代运行时 `if`：

```cpp
// 尾块场景：用 for(1) 替代 if
uint16_t tail = count % oneRepeatSize;
uint16_t hasTail = !!tail;
for (uint16_t i = 0; i < hasTail; ++i) {
    // 尾块处理
}
```

### VF融合原理

VF融合是Reg矢量计算最重要的优化手段，编译器自动将多个VF函数及其循环合并。分为三个阶段：

1. **VF浅度融合**：将控制流等价（Control-Flow-Equivalent）的多个VF合并为一个VF函数，将VF外部的Software Loop硬化成Hardware Loop，消除VF调用开销。

2. **VF深度融合**：在浅度融合基础上，将多个Hardware Loop进行融合，减少Loop启动开销，消除冗余的Load/Store操作，使中间结果充分复用在寄存器中。例如两个VF各自Load同一UB地址的数据，融合后可共享一次Load。

3. **VF内自动同步**：编译器精准插入必要的 `asc_mem_bar` 同步指令，删除冗余的同步指令，释放硬件乱序执行（OOO）能力。

融合粒度并非越大越好：融合过大可能导致寄存器溢出（spill），编译器会插入换入换出指令，反而降低性能。

### 指令双发机制

指令双发（Dual Issue）指处理器在同一时钟周期内能够同时发射两条无数据依赖的指令到执行单元。关键条件：

- 两条指令之间无数据依赖（后一条不需要前一条的结果）。
- 硬件拥有足够的执行资源。

Reg矢量执行单元、DMA单元和Aux Scalar虽属于不同硬件单元，但实际执行时都归属PIPE_V流水。同一PIPE内，没有依赖关系的指令可以同时发射、并行执行。开发者的优化目标是**为执行队列提供足够多且无依赖的并发指令**，避免长依赖链阻塞硬件并行。

指令双发的典型优化手段：
- **拆分VF循环**：将过长的依赖链切分为多个独立循环，阶段性写回UB以减少寄存器间数据依赖。
- **手动循环展开**：使用多套寄存器组（如 `srcReg0`/`srcReg1`/`srcReg2`/`srcReg3`）交错执行不同数据流，打破单数据流内的依赖链，为执行队列填充更多无依赖指令。

### 流水线同步

Reg矢量执行单元与DMA单元可以并行工作。判断是否需要显式同步的关键在于：不同硬件流水之间是否访问同一UB地址且存在数据依赖。

| 场景 | 是否需要同步 | 原因 |
| --- | --- | --- |
| 同一寄存器内的写后读 | 不需要 | 寄存器依赖由硬件按指令顺序保证 |
| 不同寄存器访问同一UB地址，存在写后读 | 需要 | 搬运和计算执行单元无自动顺序约束 |
| 不同寄存器访问同一UB地址，存在写后写 | 需要 | 需保证写入顺序 |
| 地址无重叠、寄存器无依赖 | 不需要 | 不存在数据竞争 |

显式同步通过 `LocalMemBar`（C++基础API）或 `asc_mem_bar`（C API）实现，方向包括 `VEC_STORE → VEC_LOAD`、`VEC_LOAD → VEC_STORE`、`VEC_STORE → VEC_STORE` 等。

### 寄存器资源限制

在同一个VF中，硬件可同时使用的寄存器数量受严格限制，超出后编译器会插入换入换出（spill/fill）和额外同步指令，严重拖慢执行效率：

| 寄存器类型 | 最大可用数量 |
| --- | --- |
| `RegTensor<T>`（矢量数据寄存器） | 32 |
| `MaskReg`（掩码寄存器） | 8 |
| `UnalignRegForLoad`（搬入非对齐寄存器） | 4 |
| `UnalignRegForStore`（搬出非对齐寄存器） | 4 |

优化建议：使用布尔代数化简减少MaskReg使用，适当调整指令顺序以复用寄存器，控制展开因子避免寄存器溢出。

## 样例描述

**样例功能**：

样例实现的是固定shape(m, n)为(128,128)的Softmax运算。

Softmax的计算公式为：

$$
\text{Softmax}(x_{i,j}) = \frac{e^{x_{i,j} - \max_i}}{\sum_{j=0}^{n-1} e^{x_{i,j} - \max_i}}
$$

- x：输入，形状为[128, 128]，数据类型为float；
- z：输出，形状为[128, 128]，数据类型为float；

**样例规格**：

<table border="2">
<tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Softmax</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">[128, 128]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[128, 128]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">softmax_custom</td></tr>
</table>

## 版本概览

| 版本 | API类型 | 特点 | 性能 (仅统计vector部分) |
|:---:|:---|:---|:---:|
| **Case 0** | MemBase | 基础版本，单循环内完成所有计算 | 48572 cycles |
| **Case 1** | MemBase | 循环拆分为3个阶段（ReduceMax / Sub+Exp+ReduceSum / Div） | 48412 cycles |
| **Case 2** | RegBase | 首次使用Reg矢量API，未融合，含冗余Load/Store/sync | 31894 cycles |
| **Case 3** | RegBase | UpdateMask模式 + 循环融合 + ExpSub融合指令 | 3413 cycles |
| **Case 4** | RegBase | UpdateMask模式 + 外层循环展开，提升指令双发能力 | 2197 cycles |
| **Case 5** | RegBase | 主尾块模式（main-tail block），替代UpdateMask | 3424 cycles |
| **Case 6** | RegBase | 主尾块模式 + 外层展开 + ExpSub融合指令（最优） | 1785 cycles |

## 优化策略分析

本节详细介绍从Case 0到Case 6的渐进优化策略。

### 性能指标说明

**表1 AI Core 性能指标字段说明表**

| 字段名 | 字段含义 |
|:---|:---|
|aiv_vec_time(μs)|vec类型指令（向量类运算指令）耗时，单位μs。|
|折算aiv_vec_cycle|vec类型指令（向量类运算指令）的cycle数。|

---

## 第一阶段：MemBase优化 (Case 0 → Case 1)

### Case 0: MemBase基础版本 — 单循环内完成所有计算

**实现方式**：使用Ascend C基础API，在128行的for循环内依次完成ReduceMax、Duplicate、Sub、Exp、ReduceSum、Duplicate、Div全部7步操作。

**理论分析 — 为什么MemBase慢**：

Case 0使用Memory矢量计算API，每条指令的语义是"从UB读源操作数 → 在执行单元计算 → 写回UB目的操作数"的完整流程。Softmax每行7步操作产生14次UB访问（7次Load + 7次Store），128行合计产生1792次UB访问。这意味着：

1. **每步中间结果必须落地UB**：ReduceMax的结果写入UB后，Duplicate需要从UB重新读出；Duplicate写入UB后，Sub需要从UB重新读出……以此类推。中间结果在UB上反复读写，UB带宽压力随操作数线性放大。
2. **UB Bank冲突风险高**：连续的UB读写访问极易触发Bank冲突，导致访问延迟增加。
3. **单循环内异构操作混杂**：7种不同操作（规约、逐元素计算、超越函数Exp、广播Duplicate）在同一循环内顺序执行，每种操作有不同的流水线特性，编译器难以对跨操作类型的指令序列做有效调度。

**关键代码**：
```cpp
for (uint32_t i = 0; i < aDim; i++) {
    AscendC::ReduceMax(expTensor[i * rDimAlign], srcTensor[i * rDimAlign], sumTensor[i * rDimAlign], rDim);
    AscendC::Duplicate(expTensor[i * rDimAlign], expTensor[i * rDimAlign], rDim);
    AscendC::Sub(expTensor[i * rDimAlign], srcTensor[i * rDimAlign], expTensor[i * rDimAlign], rDim);
    AscendC::Exp(expTensor[i * rDimAlign], expTensor[i * rDimAlign], rDim);
    AscendC::ReduceSum(sumTensor[i * rDimAlign], expTensor[i * rDimAlign], dstTensor[i * rDimAlign].ReinterpretCast<float>(), rDim);
    AscendC::Duplicate(sumTensor[i * rDimAlign], sumTensor[i * rDimAlign], rDim);
    AscendC::Div(dstTensor[i * rDimAlign], expTensor[i * rDimAlign], sumTensor[i * rDimAlign], rDim);
}
```

**性能**：48572 cycles（vector部分）

### Case 0 → Case 1: 循环拆分

**优化动机**：Case 0中每行7步操作在同一循环中顺序执行，不同操作之间存在数据依赖，限制了编译器优化空间。

**理论分析 — 循环拆分为3个Phase**：

循环拆分的核心思想是**将异构操作解耦为同构操作的批量执行**，这在理论上带来三个收益：

1. **消除操作切换开销**：Case 0中每行在ReduceMax→Duplicate→Sub→Exp→ReduceSum→Duplicate→Div之间频繁切换。拆分后Phase 1只做ReduceMax（128行），Phase 2只做Sub+Exp+ReduceSum（128行），Phase 3只做Div（128行）。每Phase内指令序列高度同构，编译器可更有效地优化循环内流水线。

2. **提升缓存/流水线局部性**：相同操作集中执行时，计算单元的流水线状态可以跨行复用，减少流水线stall。例如Phase 1的ReduceMax规约电路可以流水处理连续128行的最大池化。

3. **为后续融合提供基础**：拆分为三个独立Phase后，每个Phase内部的操作类型单一，后续RegBase迁移时更容易识别融合机会（例如Phase 2内部Sub+Exp可融合为ExpSub）。

**优化措施**：将单循环拆分为3个独立的双层循环：
- Phase 1（所有行）：ReduceMax → 求每行最大值
- Phase 2（所有行）：Duplicate + Sub + Exp + ReduceSum → 数值稳定计算及求和
- Phase 3（所有行）：Duplicate + Div → 归一化

**关键代码变化**：
```cpp
// Case 0: 单循环，mix all operations
for (uint32_t i = 0; i < aDim; i++) {
    ReduceMax(...);
    Duplicate(...);
    Sub(...);
    // ...
}

// Case 1: 拆分为3个独立Phase
// Phase 1: ReduceMax (all rows)
for (uint32_t i = 0; i < aDim; i++) {
    ReduceMax(...);
}
// Phase 2: Duplicate + Sub + Exp + ReduceSum (all rows)
for (uint32_t i = 0; i < aDim; i++) {
    Duplicate(...); Sub(...); Exp(...); ReduceSum(...);
}
// Phase 3: Duplicate + Div (all rows)
for (uint32_t i = 0; i < aDim; i++) {
    Duplicate(...); Div(...);
}
```

**效果**：48572 → 48412 cycles。循环拆分使相同操作集中执行，提升缓存利用率和流水线效率。MemBase阶段收益有限（仅0.3%），但拆分后的三阶段结构为后续RegBase改造奠定了基础。

---

## 第二阶段：MemBase → RegBase (Case 1 → Case 2)

### Case 2: Reg矢量API初版 — 未融合，含冗余操作

**优化动机**：MemBase API每次向量计算内部隐式执行 Load→Compute→Store，129行 × 7次操作 = 大量UB↔寄存器交互。RegBase API可将数据加载到寄存器后常驻，中间结果暂存寄存器，仅需最终Store回UB。

**理论分析 — MemBase → RegBase 的关键收益**：

1. **Register File降低UB带宽压力**：
   在MemBase中，Softmax每行7步操作的中间结果（max值、scalar max for broadcast、sub结果、exp结果、sum值、scalar sum for broadcast）全部回写UB。Case 1中128行 × 4个中间张量 = 512次冗余UB读写。RegBase引入SIMD Register File后，这些中间值可以保留在矢量数据寄存器中连续消费：
   ```
   MemBase: UB → Load → Compute → Store → UB → Load → Compute → Store → UB ...
   RegBase: UB → Load → Compute(reg) → Compute(reg) → ... → Store → UB
   ```
   仅在进入和退出整段计算时各访问一次UB，中间结果全部在寄存器链内部传递。

2. **显式Load/Store控制搬运时机**：
   MemBase中搬运动作由API隐式完成，开发者无法控制。RegBase通过 `LoadAlign` / `StoreAlign`（对齐搬运）或 `LoadUnAlign` / `StoreUnAlign`（非对齐搬运）显式控制搬运粒度与时机，可以精确调度数据流。

3. **Hardware Loop使能**：
   MemBase循环在 `__aicore__` 域执行，不满足VF内Hardware Loop规范。RegBase计算在 `__simd_vf__` 函数内执行，循环使用 `uint16_t` 类型、从0开始递增1、无if/else跳转，编译器可将其识别为Hardware Loop，消除软件循环的计数/比较/跳转开销。

**关键变化**：

1. 使用 `Reg::LoadAlign` / `Reg::StoreAlign` 显式控制搬运
2. 使用 `Reg::Max` / `Reg::Sub` / `Reg::Exp` / `Reg::ReduceMax` / `Reg::ReduceSum` 等寄存器级API
3. 使用 `UpdateMask` 处理非对齐尾块
4. 通过 `LocalMemBar` 保证数据一致性

**Case 2存在的问题**：
- 多个循环未融合，存在冗余 Load/Store 指令
- 每次计算后插入 `LocalMemBar`，引入冗余同步
- 未使用 ExpSub 融合指令，Sub 和 Exp 分开执行

**性能**：31894 cycles（相比 Case 1 提升 34%）。RegBase带来的寄存器级计算消除了大量UB往返开销。

---

## 第三阶段：RegBase深度优化 (Case 2 → Case 3)

### Case 3: 循环融合 + ExpSub融合指令 + UpdateMask尾块处理

**理论分析 — 三项优化的深层原理**：

**1. VF深融合消除冗余Load/Store**：

Case 2中ReduceMax循环和ExpSub循环各自独立从UB Load源数据，本质上是两次相同的UB→寄存器搬运。VF深融合将两个循环的Load共享，原理如下：

```
Case 2 (未融合):
  for i: for j: Load(src) → Max → ReduceMax     // Load src 第1次
  mem_bar()
  for i: for j: Load(src) → Sub+Exp → Store      // Load src 第2次 (冗余!)
  mem_bar()
  for i: for j: Load(exp) → Add → ReduceSum      // Load exp 第1次
  mem_bar()
  for i: for j: Load(exp) → Duplicate+Div → Store // Load exp 第2次 (冗余!)

Case 3 (融合):
  for i:
      for j: Load(src) → Max → ReduceMax          // Load src
      for j: Load(src) → ExpSub → Store           // 复用已Load的src (无需重复搬入)
  mem_bar()
  for i:
      for j: Load(exp) → Add → ReduceSum          // Load exp
      for j: Load(exp) → Duplicate+Div → Store    // 复用已Load的exp
```

Case 3只需2次Load（src和exp各一次），Case 2需要4次Load，融合后消除2次冗余UB搬运。

**2. 指令融合减少依赖链长度**：

Case 2中 `Sub` 与 `Exp` 为两条独立指令：
```cpp
// Case 2: Sub → Exp 顺序执行，Exp必须等待Sub结果写入dstReg
AscendC::Reg::Sub(dstReg, srcReg, maxReg, mask);
AscendC::Reg::Exp(dstReg, dstReg, mask);
```
ExpSub融合后为单条指令，Sub的中间结果直接在硬件内部传递给Exp，消除了一条指令的发射延迟和一条寄存器写入：
```cpp
// Case 3: 一步融合，Sub→Exp在硬件内部完成
AscendC::Reg::ExpSub(expReg, srcReg, maxReg, mask);
```
在指令双发的执行队列中，减少一条依赖指令意味着后续独立指令可以更早被发射。

**3. UpdateMask vs 主尾块**：

UpdateMask模式在每次内层循环迭代中调用 `UpdateMask` 更新掩码，虽然满足Hardware Loop（无if/else），但每次调用仍产生标量计算开销。主尾块模式将数据切分为"主块（全Mask）"和"尾块（部分Mask）"分别处理，主块循环内无需任何掩码更新操作，完全消除UpdateMask的累加开销。

**优化手段**：

#### 1. 融合指令：ExpSub

Sub + Exp 两步操作融合为单条 ExpSub 指令，减少指令数和执行延迟：
```cpp
// Case 2: 两步
AscendC::Reg::Sub(dstReg, srcReg, maxReg, mask);
AscendC::Reg::Exp(dstReg, dstReg, mask);
// Case 3: 一步融合
AscendC::Reg::ExpSub(expReg, srcReg, maxReg, mask);
```

#### 2. 循环融合

将ReduceMax的Load与ExpSub的Load共享，将ReduceSum与Div融合：

```cpp
// Case 2: 未融合 — ReduceMax和ExpSub各独立Load
for (i) {
    for (j) { Load(src); Max(max, src); }    // Load #1
    ReduceMax(max);
    for (j) { Load(src); Sub+Exp+Store; }      // Load #2 (冗余!)
}
for (i) {
    for (j) { Load(exp); Add(sum, exp); }      // Load #3
    ReduceSum(sum);
    for (j) { Load(exp); Div+Store; }          // Load #4 (冗余!)
}

// Case 3: 融合后 — 共享Load，减少2次冗余搬入
for (i) {
    for (j) { Load(src); Max(max, src); }      // Load #1
    ReduceMax(max);
    for (j) { Load(src); ExpSub+Store; }       // Load #2 (复用同一个src)
}
mem_bar();
for (i) {
    for (j) { Load(exp); Add(sum, exp); }      // Load #3
    ReduceSum(sum);
    for (j) { Load(exp); Div+Store; }          // Load #4 (复用同一个exp)
}
```

#### 3. UpdateMask尾块处理

使用 `UpdateMask` 在每次内层循环中自动更新掩码，代码简洁：
```cpp
uint32_t count = rDim;
for (uint16_t j = 0; j < repeatTimes; j++) {
    mask = AscendC::Reg::UpdateMask<float>(count);
    LoadWithCastIfNeed(srcAddr, srcReg, mask, ...);
    AscendC::Reg::Max(maxReg, maxReg, srcReg, mask);
}
```

**效果**：31894 → 3413 cycles（提升 9.3x），融合收益显著。

---

## 第四阶段：循环展开优化

### Case 3 → Case 4: 外层循环展开（UpdateMask模式）

**优化动机**：Case 3中128行依次串行处理，编译器难以跨行调度指令，限制了VF双发效率。

**理论分析 — 外层展开如何提升指令双发**：

Case 3的执行模式为：行0的Load→Max→ReduceMax→Load→ExpSub→Store 全部执行完，再执行行1。此时执行队列中仅有一行（行i）的指令序列，而行i内部存在明显的依赖链：

```
Load(src_i) → Max(max_i, src_i) → ... → ReduceMax(max_i) → ... → Load(src_i) → ExpSub(...) → Store
```

依赖链上的指令必须等待前序指令完成，无法与任何其他指令并行发射。这就导致执行队列深度虽然可达64条指令，但由于行i内部所有指令彼此依赖，实际可同时发射的无依赖指令很少。

**外层展开（展开因子=2）** 使用双套寄存器组（`*Reg` 和 `*Reg1`），每次处理两行：

```
行0: Load(src0) → Max(max0, src0) → ... → ReduceMax(max0) → Load(src0) → ExpSub(exp0) → Store
行1:   Load(src1) → Max(max1, src1) → ... → ReduceMax(max1) → Load(src1) → ExpSub(exp1) → Store
```

行0与行1之间**无任何数据依赖**，两条流水线可以交叉发射：行0的Load发出后、等待数据返回期间，可以发射行1的Load；行0的Max执行期间可以发射行1的Max。执行队列中同时填充两行无依赖的指令，双发率显著提升。

这一机制对应"指令双发优化"中的核心原则：为执行队列提供足够多且无依赖的并发指令。当前shape为128行，每个展开组处理2行，共64组，寄存器使用量为单行模式的约2倍，仍在硬件限制内（展开后约6-8个RegTensor，远未达到32上限）。

**优化措施**：手动展开最外层循环，每次处理两行数据，使用双套寄存器组（`*Reg` 和 `*Reg1`）：

```cpp
// Case 3: 串行处理
for (uint16_t i = 0; i < aDim; i++) {
    // 单行 ReduceMax → ExpSub
}
for (uint16_t i = 0; i < aDim; i++) {
    // 单行 ReduceSum → Div
}

// Case 4: 外层展开，每次处理两行
uint16_t halfA = aDim >> 1;
for (uint16_t i = 0; i < halfA; i++) {
    Duplicate(maxReg, ...);  Duplicate(maxReg1, ...);
    for (j) {
        Load(srcRow0, srcRow0+halfA_offset);  // 两行交错Load
        Max(maxReg, srcRow0);  Max(maxReg1, srcRow1);
    }
    ReduceMax(maxReg);  ReduceMax(maxReg1);
    for (j) {
        Load(srcRow0, srcRow0+halfA_offset);
        ExpSub(expReg);  ExpSub(expReg1);
        Store(...);
    }
}
```

**效果**：3413 → 2197 cycles（提升 35.6%），外层展开有效提升指令双发能力。

### Case 3 → Case 5: 主尾块模式

**优化动机**：UpdateMask每次内层循环更新掩码引入标量开销；主尾块切分后主块可全部使用全Mask，消除UpdateMask重复计算。

**理论分析 — 主尾块模式与Hardware Loop**：

Hardware Loop的编码规范明确要求循环体内不允许 `if/else` 等运行时条件跳转。尾块处理是一个"运行时才能知道是否需要尾块"的场景，在Hardware Loop中不能直接使用 `if (hasTail) { ... }`。

主尾块模式的解决方案是：
1. 编译期计算尾块大小 `tail = rDim % VL_size`（当前shape下tail=0）。
2. 主块循环次数 `repeatTimesMain = rDim / VL_size`，全部使用编译期创建的 `CreateMask<ALL>()`（全Mask），循环内**无需任何条件判断或掩码更新**。
3. 尾块用 `for(1)` 模式替代 `if`：`for (uint16_t t = 0; t < hasTail; ++t) { /* 尾块处理 */ }`。`for(1)` 可被识别为Hardware Loop，而 `if` 会被认为分支跳转。

UpdateMask虽然有掩码自动更新特性，但每次调用UpdateMask仍引入标量计算（更新计数值、生成掩码）。主尾块模式下，主块128行的全部内层循环均使用固定全Mask，完全消除UpdateMask的累加开销。当前shape下 `rDim=128`，每个float占4B，`VL=256B` 对应64元素，恰好整除 `128 / 64 = 2`，尾块为0，主尾块模式达到收益上限。

**关键代码变化**：
```cpp
// 先算出主块次数和尾块大小
uint16_t tail = rDim % (256 / sizeof(float));
uint16_t repeatTimesMain = rDim / (256 / sizeof(float));
uint16_t hasTail = (tail != 0);

// Case 5: 内层循环全部使用全Mask（maskFull），无需UpdateMask
for (uint16_t i = 0; i < aDim; i++) {
    for (uint16_t j = 0; j < repeatTimesMain; j++) {
        LoadWithCastIfNeed(srcAddr, srcReg, maskFull, ...);
        AscendC::Reg::Max(maxReg, maxReg, srcReg, maskFull);
    }
    ReduceMax(maxReg, maskFull);
    Duplicate(maxReg, maxReg, maskFull);
    for (uint16_t j = 0; j < repeatTimesMain; j++) {
        LoadWithCastIfNeed(srcAddr, srcReg, maskFull, ...);
        AscendC::Reg::ExpSub(expReg, srcReg, maxReg, maskFull);
    }
}
```

**尾块处理方式性能比对**：

| 模式 | 折算aiv_vec_cycle | 说明 |
|:---|:---:|:---|
| UpdateMask模式（Case 4，带展开） | 2197 | 代码简洁，UpdateMask每次更新有开销 |
| 主尾块模式-带尾块处理 | 4030 | 尾块For(0)依赖Hardware Loop，当前性能不及预期 |
| 主尾块模式-不带尾块处理（Case 5） | 3424 | 主块全Mask，当前shape尾块=0，actual=3424 |
| 主尾块+展开+ExpSub（Case 6） | 1785 | 最优组合，展开+全Mask无更新开销 |

> 结论：当前shape下last轴仅128，尾块为0，主尾块模式展开后性能最优（Case 6）。若last轴更大且不可整除，UpdateMask模式代码更简洁，需根据场景权衡。

### Case 5 → Case 6: 最优组合 — 主尾块 + 外层展开 + ExpSub

**理论分析 — 最大化双发效率的最优组合**：

Case 6同时应用了三个维度上互不冲突的优化手段，形成"1 + 1 + 1 > 3"的叠加效果：

| 优化手段 | 核心收益 | 对寄存器的影响 | 是否相互干扰 |
| --- | --- | --- | --- |
| 主尾块切分 | 主块全Mask，消除UpdateMask开销 | 无额外寄存器 | 不干扰 |
| 外层循环展开(×2) | 两行数据交错发射，填充执行队列 | ×2 RegTensor用量 | 不干扰 |
| ExpSub融合指令 | Sub+Exp合并为单指令，减依赖链 | 减少1个临时RegTensor | 不干扰 |

三项优化的组合机制：

1. **寄存器资源利用**：当前shape下单行需要约4-6个RegTensor（src、max、exp、sum、临时寄存器），展开2倍后约8-12个RegTensor，MaskReg使用量约2-3个，均在硬件限制内（≤32 RegTensor、≤8 MaskReg）。寄存器资源充足是三重优化可以叠加的前提。

2. **执行队列深度最大化**：外层展开向执行队列中填充两行无依赖的指令；主尾块确保主块循环内无条件分支（全Mask无UpdateMask调用），指令序列连续无跳转；ExpSub将Sub+Exp的两条有依赖指令合并为单条，使执行队列中指令总数减少、依赖关系更简短。

3. **最终结果**：1785 cycles相比Case 0的48572 cycles达到27.2x提升，比单独应用外层展开（2197 cycles）或单独应用主尾块（3424 cycles）均有显著改善。

**优化措施**：在主尾块模式（Case 5）基础上增加外层循环展开，形成最优组合：
- 主尾块切分：主块全部使用全Mask，消除UpdateMask开销
- 外层循环展开：双套寄存器组，每次处理两行，提升双发效率
- ExpSub融合指令：Sub+Exp单指令执行

**性能**：1785 cycles（相比Case 0提升 27.2x），为本样例最优性能。

---

## 性能对比总结

| 版本 | 优化策略 | 折算aiv_vec_cycle | 相对Case 0 | 优化主题 |
|:---:|:---|:---:|:---:|:---|
| Case 0 | MemBase，单循环 | 48572 | 1.00x | 基准 |
| Case 1 | MemBase，循环拆分 | 48412 | 1.00x | 循环拆分 |
| Case 2 | RegBase，未融合 | 31894 | 1.52x | API升级 |
| Case 3 | RegBase，UpdateMask，ExpSub，循环融合 | 3413 | 14.23x | 融合优化 |
| Case 4 | RegBase，UpdateMask，外层展开 | 2197 | 22.11x | 指令双发 |
| Case 5 | RegBase，主尾块模式 | 3424 | 14.18x | 尾块处理 |
| Case 6 | RegBase，主尾块+展开+ExpSub | 1785 | 27.21x | 最优组合 |

### 优化要点总结

| 优化手段 | 核心原理 | 适用版本 |
|:---|:---|:---|
| 循环拆分 | 相同操作集中执行，提升缓存利用 | Case 0→Case 1 |
| MemBase→RegBase | 寄存器级计算减少UB↔寄存器往返 | Case 1→Case 2 |
| ExpSub融合指令 | 单条指令替代Sub+Exp，减少延迟 | Case 2→Case 3 |
| 循环融合 | 共享Load/Store，消除冗余搬运和同步 | Case 2→Case 3 |
| UpdateMask | 自动更新掩码，简洁尾块处理 | Case 3,Case 4 |
| 主尾块模式 | 主块全Mask无更新开销，尾块独立处理 | Case 5,Case 6 |
| 外层循环展开 | 多套寄存器组交错执行，提升指令双发 | Case 3→Case 4, Case 5→Case 6 |

---

## 理论分析总结

### 各Case应用的理论

| Case | API层级 | 循环拆分 | Hardware<br>Loop | VF融合 | ExpSub<br>融合 | 指令双发<br>(展开) | 主尾块 | 寄存器<br>压力 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| Case 0 | MemBase | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | 无寄存 |
| Case 1 | MemBase | ✓ (3Phase) | ✗ | ✗ | ✗ | ✗ | ✗ | 无寄存 |
| Case 2 | RegBase | ✓ (3Phase) | ✓ | ✗ | ✗ | ✗ | ✗ | 低 |
| Case 3 | RegBase | ✓ (融合后) | ✓ | ✓ (深融合) | ✓ | ✗ | ✗ | 低 |
| Case 4 | RegBase | ✓ (融合后) | ✓ | ✓ (深融合) | ✓ | ✓ (×2) | ✗ | 中 |
| Case 5 | RegBase | ✓ (主尾块) | ✓ | ✓ (深融合) | ✓ | ✗ | ✓ | 低 |
| Case 6 | RegBase | ✓ (主尾块) | ✓ | ✓ (深融合) | ✓ | ✓ (×2) | ✓ | 中 |

### 寄存器压力分析

以下为各Case在VF内同时使用的近似寄存器数量估算（单VF函数内最大同时存活量）：

| Case | RegTensor 数 | MaskReg 数 | 硬件上限 | 是否安全 |
|:---:|:---:|:---:|:---|:---:|
| Case 0 | — (MemBase, 无寄存器) | — | 32 / 8 | — |
| Case 1 | — (MemBase, 无寄存器) | — | 32 / 8 | — |
| Case 2 | 4~6 | 2 | 32 / 8 | ✓ |
| Case 3 | 4~6 | 2 | 32 / 8 | ✓ |
| Case 4 | 8~12 (×2展开) | 2~3 | 32 / 8 | ✓ |
| Case 5 | 4~6 | 2 | 32 / 8 | ✓ |
| Case 6 | 8~12 (×2展开) | 2~3 | 32 / 8 | ✓ |

所有Case的寄存器用量均在硬件安全范围内。Case 4和Case 6因外层展开(×2)导致RegTensor数约翻倍但仍远低于32的上限，未触发spill。若进一步增加展开因子（如×4），需谨慎评估寄存器是否溢出。

### IPC（指令发射率）对比

指令双发（Dual Issue）的收益可通过跨版本性能对比间接观察。假设Case 3至Case 6中单cycle近似可完成的工作量随优化叠加递增：

| 版本 | 折算cycle数 | 相对Case 3 | 双发贡献因素 |
|:---:|:---:|:---:|:---|
| Case 3 | 3413 | 1.00x（基线） | 融合+ExpSub，执行队列依赖链已缩短 |
| Case 4 | 2197 | 1.55x | +外层展开(×2)，执行队列中独立指令数翻倍 |
| Case 5 | 3424 | ~1.00x | 主尾块减少标量开销，但不增加并行度 |
| Case 6 | 1785 | 1.91x | +外层展开+主尾块+ExpSub，三重叠加最大化 |

Case 6相对Case 3的1.91x增益主要来源于外层展开驱动的双发效率提升（填充执行队列无依赖指令）和主尾块免除UpdateMask标量开销的叠加效果。

---

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 切换版本

  通过 `-DSCENARIO_NUM=N` (N=0~6) 指定要编译的版本：

  | SCENARIO_NUM | 版本 | 说明 | CMAKE_VF_MODE |
  |:---:|:---:|:---|:---:|
  | 0 | Case 0 | MemBase，单循环 | false |
  | 1 | Case 1 | MemBase，循环拆分 | false |
  | 2 | Case 2 | RegBase，未融合 | false |
  | 3 | Case 3 | RegBase，UpdateMask+ExpSub+融合 | true |
  | 4 | Case 4 | RegBase，UpdateMask+展开 | true |
  | 5 | Case 5 | RegBase，主尾块模式 | true |
  | 6 | Case 6 | RegBase，主尾块+展开+ExpSub | true |

  > **注意：** Case 0-2为MemBase或不使用VF融合，需设置 `CMAKE_VF_MODE=false`。Case 3-6需设置 `CMAKE_VF_MODE=true`。

- 配置环境变量
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 单版本执行
  ```bash
  mkdir -p build && cd build
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=6 -DCMAKE_VF_MODE=true
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

- 使用build.sh快速执行
  ```bash
  ./build.sh -c 6              # 编译并运行Case 6
  ./build.sh batch             # 批量编译并验证Case 0-6全部版本
  ```

- 执行结果

  ```bash
  error ratio: 0.0000, tolerance:0.0001
  test pass!
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构 |
  | `SCENARIO_NUM` | `0`~`6` | 版本编号 Case 0-6 |
  | `CMAKE_VF_MODE` | `true`、`false` | VF融合模式：Case 0-2=false，Case 3-6=true |

### 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
