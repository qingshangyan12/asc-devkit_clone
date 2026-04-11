# SIMD与SIMT混合编程<a name="ZH-CN_TOPIC_0000002477823926"></a>

AI Core上的SIMD（Single Instruction Multiple Data，单指令多数据）与SIMT（Single Instruction Multiple Thread，单指令多线程）混合编程当前仅支持Ascend 950PR/Ascend 950DT。该架构通过统一的计算资源和内存层级，实现向量级并行与线程级并行的高效协同，支持开发者根据算子特征进行精细化映射，在性能、能效与开发效率之间取得更优平衡。

本章将系统介绍抽象硬件架构、Vector Function的基本概念、SIMD VF与SIMT VF的工作机制，以及混合编程模式下的关键特性与编程实践。

## 抽象硬件架构<a name="section3163536195018"></a>

### 硬件架构概述<a name="section_hardware_overview"></a>

SIMD与SIMT混合编程架构通过统一的计算资源和内存层级，实现向量级并行与线程级并行的高效协同。整个硬件架构围绕AIV（AI Vector）核展开，每个AIV核内部包含多种计算单元和存储单元，通过Vector Function（VF）作为基本调度单位实现SIMD与SIMT两种编程模式的协同工作。

**图 1** SIMD与SIMT混合编程硬件架构图<a name="fig17744131364413"></a>  
![](../../../figures/SIMD与SIMT混合编程硬件架构图.png "SIMD与SIMT混合编程硬件架构图")

### 核心硬件组件<a name="section_hardware_components"></a>

#### 计算单元<a name="section_compute_units"></a>

| 单元类型 | 功能说明 | 在混合编程中的作用 |
|---------|---------|-------------------|
| Scalar计算单元 | 执行地址计算、循环控制等标量计算工作，负责向量计算、矩阵计算、数据搬运、同步指令的发射 | 将VF发射到Vector Function Queue，协调整个执行流程 |
| Vector计算单元 | 负责执行向量运算，支持多种数据类型的运算 | SIMD和SIMT共享，执行具体的向量计算任务 |
| Cube计算单元 | 负责执行矩阵运算 | 执行矩阵乘法等矩阵运算任务 |

#### 存储单元<a name="section_storage_units"></a>

| 存储类型 | 位置 | 容量/特点 | 在混合编程中的作用 |
|---------|-----|----------|-------------------|
| Global Memory | AIV核外部 | 大容量、高延迟 | 存储核函数处理所需的输入数据和计算结果，所有核共享 |
| Unified Buffer | AIV核内部 | 256KB高速片上内存 | 分为静态内存、动态内存、预留空间、Data Cache四个区域 |
| Register File (RF) | AIV核内部 | 快速访问 | SIMD VF使用向量寄存器存储计算数据 |
| 寄存器 | Vector Core内部 | 最快存储资源 | SIMT VF中每个线程拥有独立寄存器 |
| 堆栈空间 | Device侧 | 独立存储区域 | SIMT VF中每个线程独立的堆栈空间 |

#### DMA搬运单元<a name="section_dma_unit"></a>

DMA（Direct Memory Access）搬运单元负责数据搬运，包括：
- Global Memory和Unified Buffer之间的数据搬运
- 不同层级Local Memory之间的数据搬运
- SIMD模式下从Unified Buffer到向量寄存器的数据搬运

### Vector Function工作机制<a name="section_vf_mechanism"></a>

#### VF的基本概念<a name="section_vf_basic_concept"></a>

Vector Function（VF，向量函数）是SIMD与SIMT混合编程的基本调度单位，代表一个独立的计算任务片段。在混合编程架构中，整个执行过程以VF为粒度进行任务调度和切换。

**核心特性：**

- **基本调度单位**：VF是硬件调度的最小单元，Scalar计算单元将VF发射到Vector Function Queue中等待执行
- **类型可选**：每个VF可以选择使用SIMD或SIMT方式进行编程，开发者可根据算子特征灵活选择
- **快速切换**：不同类型VF之间可以快速切换，切换粒度为单个VF
- **独立执行**：同一时刻，一个AIV核只能执行SIMD或SIMT任务，VF之间串序执行

#### VF的执行流程<a name="section_vf_execution_flow"></a>

SIMD和SIMT的内部执行流程遵循以下步骤：

```
┌─────────────────────────────────────────────────────────────┐
│                    VF执行流程                                 │
├─────────────────────────────────────────────────────────────┤
│  1. Scalar计算单元将VF发射到Vector Function Queue            │
│  2. SIMD与SIMT混合编程以VF为粒度进行工作模式切换              │
│  3. 执行上下文（UB Data Cache）在VF切换时保留                 │
│  4. SIMD和SIMT之间的VF串序执行                                │
│  5. 同一时刻，一个AIV核仅能执行SIMD或SIMT任务                 │
│  6. VF执行完成后，结果数据被写回Unified Buffer或Global Memory │
└─────────────────────────────────────────────────────────────┘
```

**VF调度规则：**

- 一个核函数中可包含多个VF
- 每个VF可以选择使用SIMD或SIMT方式进行编程
- 不同类型VF之间可以快速切换，切换粒度为单个VF
- 在同一时刻，一个AIV核只能执行SIMT或SIMD任务

#### VF的设计理念<a name="section_vf_design_philosophy"></a>

VF的设计旨在充分发挥SIMD和SIMT各自的优势，实现精细化映射：

| 编程模式 | 核心优势 | 典型场景 |
|---------|---------|---------|
| SIMD VF | 基于向量寄存器与指令，实现高效的数据并行处理，单指令处理多数据，提升每周期的吞吐量 | 规则、连续的逐元素操作（elementwise），如卷积、矩阵乘法、向量操作等 |
| SIMT VF | 以线程为单位并行执行，能够简化复杂算子与不规则控制流的开发，有效缓解包含分支等的发散计算 | 不规则、含分支、动态访问等复杂逻辑，如注意力机制、稀疏操作、Gather/Scatter等 |

通过VF机制，开发者可以在同一算子中：
- 利用SIMD VF处理规则的逐元素操作，获得高带宽和高算力利用率
- 利用SIMT VF处理离散访问、分支控制等复杂任务，缓解发散和控制复杂度
- 通过算子融合和数据复用，减少数据搬运开销，提高整体性能

### SIMD与SIMT核心差异<a name="section_core_difference"></a>

**表 1** SIMD与SIMT核心差异点

<a name="table1651312912417"></a>
<table><thead align="left"><tr id="row151314292249"><th class="cellrowborder" valign="top" width="15.981598159815983%" id="mcps1.2.4.1.1"><p id="p17513182916244"><a name="p17513182916244"></a><a name="p17513182916244"></a>维度</p>
</th>
<th class="cellrowborder" valign="top" width="42.22422242224223%" id="mcps1.2.4.1.2"><p id="p1851392911242"><a name="p1851392911242"></a><a name="p1851392911242"></a>SIMD</p>
</th>
<th class="cellrowborder" valign="top" width="41.7941794179418%" id="mcps1.2.4.1.3"><p id="p451382915241"><a name="p451382915241"></a><a name="p451382915241"></a>SIMT</p>
</th>
</tr>
</thead>
<tbody><tr id="row5513182914244"><td class="cellrowborder" valign="top" width="15.981598159815983%" headers="mcps1.2.4.1.1 "><p id="p14513029132412"><a name="p14513029132412"></a><a name="p14513029132412"></a>编程模型</p>
</td>
<td class="cellrowborder" valign="top" width="42.22422242224223%" headers="mcps1.2.4.1.2 "><p id="p35132294246"><a name="p35132294246"></a><a name="p35132294246"></a>单指令多数据（SIMD），基于向量寄存器与向量指令。</p>
</td>
<td class="cellrowborder" valign="top" width="41.7941794179418%" headers="mcps1.2.4.1.3 "><p id="p175147295243"><a name="p175147295243"></a><a name="p175147295243"></a>单指令多线程（SIMT），以线程为单位并行执行。</p>
</td>
</tr>
<tr id="row16514172912412"><td class="cellrowborder" valign="top" width="15.981598159815983%" headers="mcps1.2.4.1.1 "><p id="p15514102917245"><a name="p15514102917245"></a><a name="p15514102917245"></a>数据搬运方式</p>
</td>
<td class="cellrowborder" valign="top" width="42.22422242224223%" headers="mcps1.2.4.1.2 "><p id="p194616581337"><a name="p194616581337"></a><a name="p194616581337"></a>通过显式Load/Store将数据从<span id="ph43501137162513"><a name="ph43501137162513"></a><a name="ph43501137162513"></a>Unified Buffer</span>搬运到向量寄存器。</p>
<p id="p1998118518347"><a name="p1998118518347"></a><a name="p1998118518347"></a>不支持直接从<span id="ph721809124812"><a name="ph721809124812"></a><a name="ph721809124812"></a>Global Memory</span>搬运数据到SIMD的向量寄存器。</p>
</td>
<td class="cellrowborder" valign="top" width="41.7941794179418%" headers="mcps1.2.4.1.3 "><p id="p55141929162410"><a name="p55141929162410"></a><a name="p55141929162410"></a>支持直接读写<span id="ph1410121110487"><a name="ph1410121110487"></a><a name="ph1410121110487"></a>Global Memory</span>或<span id="ph587461554814"><a name="ph587461554814"></a><a name="ph587461554814"></a>Unified Buffer</span>中的数据。</p>
</td>
</tr>
<tr id="row1216192522716"><td class="cellrowborder" valign="top" width="15.981598159815983%" headers="mcps1.2.4.1.1 "><p id="p141612253271"><a name="p141612253271"></a><a name="p141612253271"></a>适用场景</p>
</td>
<td class="cellrowborder" valign="top" width="42.22422242224223%" headers="mcps1.2.4.1.2 "><p id="p101611725162712"><a name="p101611725162712"></a><a name="p101611725162712"></a>规则、连续的逐元素操作（elementwise），如卷积、矩阵乘法、向量操作等。</p>
</td>
<td class="cellrowborder" valign="top" width="41.7941794179418%" headers="mcps1.2.4.1.3 "><p id="p111612025142713"><a name="p111612025142713"></a><a name="p111612025142713"></a>不规则、含分支、动态访问等复杂逻辑，如注意力机制、稀疏操作等。</p>
</td>
</tr>
</tbody>
</table>

### 共享硬件资源<a name="section_shared_resources"></a>

尽管SIMD与SIMT在编程模型和执行机制上有显著差异，但在硬件层面上共享以下关键资源：

- **ICache共享**：SIMT VF与SIMD VF共享Instruction Cache（指令缓存），提升指令预取效率
- **Vector ALU共享**：SIMT与SIMD共享Vector ALU单元，基于该单元执行的功能和性能基本相同
- **Unified Buffer共享**：Unified Buffer内存空间中一部分为SIMT与SIMD共享空间，另一部分作为SIMT的Data Cache

**资源分配示意图：**

```
Unified Buffer (256KB)
├── 静态内存（SIMD/SIMT共享）
├── 动态内存（SIMD/SIMT共享）
├── 预留空间（编译器和Ascend C预留）
└── Data Cache（SIMT专用）
```

## Vector Function基本概念<a name="section_vf_concept"></a>

### 什么是Vector Function<a name="section_vf_definition"></a>

Vector Function（VF，向量函数）是SIMD与SIMT混合编程的基本调度单位，代表一个独立的计算任务片段。在混合编程架构中，整个执行过程以VF为粒度进行任务调度和切换，每个VF通常对应算子中的一段可并行处理的逻辑。

**VF的核心价值：**

VF机制使得开发者可以在同一算子中灵活切换SIMD与SIMT执行方式，两种不同类型的VF可以快速切换，从而在性能、能效与开发效率之间取得更优平衡。

### VF在混合编程中的角色<a name="section_vf_role"></a>

在SIMD与SIMT混合编程中，核函数作为Device侧的入口函数，可以包含多个VF。核函数负责协调整个算子的执行流程，而每个VF承担具体的计算任务。

**VF组织结构示意：**

```
核函数（Kernel Function）
├── SIMT VF 1 → 执行不规则访问任务（如Gather）
├── SIMD VF 1 → 执行规则向量计算（如Adds）
├── SIMT VF 2 → 执行分支处理任务
└── SIMD VF 2 → 执行数据规约任务
```

**VF协作机制：**

这种设计使得开发者可以在同一算子中：
- 利用SIMD VF处理规则的逐元素操作，获得高带宽和高算力利用率
- 利用SIMT VF处理离散访问、分支控制等复杂任务，缓解发散和控制复杂度
- 通过算子融合和数据复用，减少数据搬运开销，提高整体性能

## SIMD VF工作机制<a name="section_simd_vf_mechanism"></a>

### SIMD VF概述<a name="section_simd_vf_overview"></a>

SIMD（Single Instruction Multiple Data）VF基于向量寄存器与向量指令实现数据并行计算。SIMD模式下，单条指令同时处理多个数据元素，通过显式的向量操作实现高效的批量数据处理。

**核心特点：**

- **向量寄存器架构**：使用Register File（RF）中的各类向量寄存器（Reg）存储数据
- **显式数据搬运**：通过Load/Store指令将数据从Unified Buffer搬运到向量寄存器
- **批量计算模式**：一条向量指令可同时处理多个数据元素，提高计算吞吐量
- **确定性执行**：所有数据元素执行相同操作，无分支发散问题

### SIMD VF执行流程<a name="section_simd_vf_execution"></a>

SIMD VF的执行遵循以下流程：

```
┌─────────────────────────────────────────────────────────────┐
│                    SIMD VF执行流程                            │
├─────────────────────────────────────────────────────────────┤
│  1. Scalar单元发射SIMD VF到Vector Function Queue            │
│  2. 从Unified Buffer加载数据到向量寄存器（Reg）               │
│  3. Vector计算单元执行向量运算                                │
│  4. 将计算结果从向量寄存器写回Unified Buffer                  │
│  5. 完成VF执行，准备切换到下一个VF                            │
└─────────────────────────────────────────────────────────────┘
```

**数据流路径：**

```
Global Memory → Unified Buffer → 向量寄存器（Reg） → Vector ALU → 向量寄存器 → Unified Buffer → Global Memory
```

**注意事项：**

- SIMD VF不支持直接从Global Memory搬运数据到向量寄存器，需先搬运到Unified Buffer
- 向量寄存器的类型和数量有限，需合理规划数据加载策略
- 执行过程中UB Data Cache作为执行上下文会被保留

### SIMD VF编程约束<a name="section_simd_vf_constraints"></a>

**函数定义：**

```cpp
__simd_vf__ inline void simd_vector_function(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z);
```

**约束规则：**

- 使用`__simd_vf__`修饰符标识SIMD VF函数
- 入参仅支持PoD（Plain Old Data）数据类型
- SIMD VF内只能调用`__simd_callee__`函数和constexpr函数
- 函数必须具有void返回类型

**PoD数据类型说明：**

PoD数据类型包括基础数据类型（int32_t、float等）以及这些基本数据类型组成的数组和结构体；不包括构造函数、析构函数、复制构造函数、复制赋值操作符、非静态成员函数或虚函数的类或结构体。

## SIMT VF工作机制<a name="section_simt_vf_mechanism"></a>

### SIMT VF概述<a name="section_simt_vf_overview"></a>

SIMT（Single Instruction Multiple Thread）VF以线程为基本执行单位，实现线程级并行计算。SIMT模式下，多个线程并行执行相同的代码，但每个线程可以维护独立的状态和控制流，特别适合处理不规则访问和复杂控制逻辑。

**核心特点：**

- **线程并行执行**：以Thread为最小执行单位，多个线程同时执行相同代码
- **独立状态维护**：每个线程拥有独立的寄存器和栈空间
- **灵活内存访问**：支持直接读写Global Memory或Unified Buffer中的数据
- **分支处理能力**：可处理数据相关的条件分支，适应复杂控制流

### SIMT VF执行流程<a name="section_simt_vf_execution"></a>

SIMT VF的执行基于Warp调度机制：

```
┌─────────────────────────────────────────────────────────────┐
│                    SIMT VF执行流程                            │
├─────────────────────────────────────────────────────────────┤
│  1. Scalar单元发射SIMT VF到Vector Function Queue            │
│  2. Thread Block被切分为多个Warp（每Warp含32线程）            │
│  3. Warp调度器轮流调度各Warp执行                              │
│  4. Warp内线程通过Data Cache访问Global Memory                │
│  5. 或直接访问Share Memory（Unified Buffer共享区域）         │
│  6. 完成VF执行，准备切换到下一个VF                            │
└─────────────────────────────────────────────────────────────┘
```

**数据流路径对比：**

| 内存类型 | 访问路径 | 特点 |
|---------|---------|------|
| Global Memory | Global Memory → Data Cache → 寄存器 | 延迟高，容量大 |
| Share Memory | Share Memory ↔ 寄存器 | 延迟低，带宽高 |

### Warp执行模型<a name="section_warp_model"></a>

Warp是SIMT架构的基本调度和执行单位，每个Warp包含32个线程。

**Warp调度机制：**

- 每个Vector Core（AIV）包含4个Warp调度器
- 调度器编号：scheduler_id = warp_id % 4
- 同一时刻，一个Vector Core只能执行一个Thread Block任务
- 不同Warp相互独立执行，无论执行相同或不同的代码路径

**Thread Block到Warp的划分：**

```
Thread Block（128线程）
├── Warp 0（线程0~31）   → 调度器0
├── Warp 1（线程32~63）  → 调度器1
├── Warp 2（线程64~95）  → 调度器2
└── Warp 3（线程96~127） → 调度器3
```

**分支发散（Branch Divergence）：**

当Warp中的线程通过数据相关的条件分支进入不同执行路径时，发生分支发散：
- 硬件串行执行每个分支路径，禁用不在当前路径上的线程
- 分支发散仅在同一Warp内发生，不同Warp之间相互独立
- 分支发散会导致Warp执行效率下降

**最佳执行效率：** 当Warp中的32个线程都执行相同的执行路径时，硬件利用率最高。

### SIMT VF编程约束<a name="section_simt_vf_constraints"></a>

**函数定义：**

```cpp
__simt_vf__ __launch_bounds__(thread_num) inline void simt_vector_function(__ubuf__ float* input, …)
```

**约束规则：**

- 使用`__simt_vf__`修饰符标识SIMT VF函数
- `__launch_bounds__(thread_num)`为可选配置，用于指定最大线程数（默认1024）
- `__gm__`修饰符用于标识指向Global Memory的指针
- `__ubuf__`修饰符用于标识指向Unified Buffer的指针
- 入参仅支持PoD数据类型
- SIMT VF内只能调用`__simt_callee__`函数和constexpr函数

## 混合编程下的SIMT机制<a name="section_simt_mechanism"></a>

### 线程架构<a name="section_thread_architecture"></a>

在SIMD与SIMT混合编程中，SIMT VF采用分层的线程组织结构，从顶层到底层依次为：Grid（线程块网格）、Thread Block（线程块）、Warp（线程束）和Thread（线程）。

#### Grid（线程块网格）<a name="section_grid"></a>

Grid是SIMT线程层次结构的最顶层，由多个Thread Block组成。

**特点：**
- 使用内置变量`gridDim`表示Grid中启用的线程块数量
- Grid可以是一维、二维或三维结构
- Grid中的所有Thread Block具有相同的尺寸和维度配置
- 同一Grid中的Thread Block相互独立，可按任意顺序执行

**限制：**
- Grid中Thread Block的总数不超过65535

#### Thread Block（线程块）<a name="section_thread_block"></a>

Thread Block是Grid的组成单元，由若干线程组成。

**特点：**
- 使用内置变量`blockDim`表示一个Thread Block启用的线程数量
- Thread Block可以是一维、二维或三维结构
- 同一Thread Block内的线程可以访问共享内存（Share Memory）
- Thread Block内的线程可通过同步机制进行协调
- 同一Thread Block内的所有线程在同一个Vector Core上执行

**限制：**
- 一个Thread Block最多可启用2048个线程（blockDim.x × blockDim.y × blockDim.z ≤ 2048）
- Thread Block内的线程数量影响每个线程可用的寄存器数量

#### 线程索引<a name="section_thread_index"></a>

| 内置变量 | 说明 | 维度表示 |
|---------|------|---------|
| gridDim | Grid的维度大小 | gridDim.x, gridDim.y, gridDim.z |
| blockDim | Thread Block的维度大小 | blockDim.x, blockDim.y, blockDim.z |
| blockIdx | 当前Thread Block在Grid中的索引 | blockIdx.x, blockIdx.y, blockIdx.z |
| threadIdx | 当前线程在Thread Block内的索引 | threadIdx.x, threadIdx.y, threadIdx.z |

**线程线性化规则：**

Thread Block中的线程按可预测的方式线性化排列：
- x维度索引变化最快，其次是y维度，最后是z维度
- 连续的threadIdx.x值表示连续的线程
- threadIdx.y的步长为blockDim.x
- threadIdx.z的步长为blockDim.x × blockDim.y

**一维Grid和Block的索引计算：**

```cpp
全局线程索引 = blockIdx.x * blockDim.x + threadIdx.x
```

**多维Grid和Block的索引计算：**

```cpp
全局线程索引 = blockIdx.y * gridDim.x * blockDim.x * blockDim.y 
             + blockIdx.x * blockDim.x * blockDim.y
             + threadIdx.y * blockDim.x 
             + threadIdx.x
```

#### 线程块执行规则<a name="section_block_execution_rules"></a>

**独立性原则：**

- Thread Block之间没有数据依赖关系
- Thread Block可以按任意顺序执行，可并行或串行执行
- 一个Thread Block不应依赖其他Thread Block的计算结果
- Thread Block之间无法直接进行数据交换和同步

**资源分配：**

- Thread Block的所有线程在同一个Vector Core上执行直至完成
- Thread Block所需的寄存器和共享内存在执行期间持续占用
- Vector Core可容纳的Thread Block数量受限于每个Block的资源占用

### 同步机制<a name="section_sync_mechanism"></a>

SIMT编程模型中，线程作为执行计算或操作内存的最小抽象单位，其操作相互独立。在某些场景中，需要支持线程间的同步或防止数据竞争。Ascend C提供了以下同步接口：

| 接口名 | 功能说明 | 使用场景 |
|-------|---------|---------|
| asc_syncthreads | 等待当前线程块内所有线程代码都执行到该函数位置 | 线程块内线程同步，确保所有线程完成前一阶段任务 |
| asc_threadfence | 保证不同线程对同一份全局、共享内存的访问过程中，写入操作的时序性 | 确保内存操作的可见性顺序，不阻塞线程 |
| asc_threadfence_block | 协调同一线程块内线程之间的内存操作顺序，确保某一线程在该接口之前的所有内存操作对同一线程块内的其他线程是可见的 | 线程块内内存操作同步 |

**同步机制使用建议：**

- 使用`asc_syncthreads`实现线程块内的栅栏同步，确保所有线程完成共享数据的写入后再进行读取
- 使用`asc_threadfence`在需要与其他Vector Core同步时，确保数据实时更新到Global Memory
- 合理安排同步位置，避免不必要的同步操作影响性能

### 寄存器资源分配<a name="section_register_allocation"></a>

每个Vector Core的寄存器资源总量固定，需要在线程块内的所有线程之间分配。

| blockDim大小 | 每个Thread可用寄存器个数 |
|-------------|------------------------|
| 1025~2048 | 16 |
| 513~1024 | 32 |
| 257~512 | 64 |
| 1~256 | 127 |

**资源分配原则：**

- 线程块内的线程数量越多，每个线程可使用的寄存器数量越少
- 当线程数量过大且计算复杂度较高时，可能导致寄存器溢出（Stack Spill），影响性能
- 应根据算子复杂度合理配置blockDim，平衡线程数量与寄存器资源

### 内存合并访问优化<a name="section_coalesced_access"></a>

Global Memory作为容量最大但延迟最高的存储资源，其访问效率直接影响程序性能。

**访问路径：**

```
Global Memory ↔ Data Cache ↔ 寄存器
```

**合并访问（Coalesced Access）：**

当同一Warp中的线程访问连续且对齐的内存地址时，多个访问请求可合并为一个内存事务，显著提升带宽利用率。

**优化策略：**

- **内存合并访问**：同一Warp中的线程应尽量访问相邻的内存地址
- **数据重用**：尽量让同一Vector Core上的线程重复使用已加载到Data Cache中的数据
- **避免随机访问**：非连续的内存访问模式会降低Cache利用率
- **使用Share Memory缓存**：将频繁访问的数据从Global Memory加载到Share Memory
- **合理划分内存空间**：平衡Share Memory和Data Cache空间，避免资源争用

## 内存层级<a name="section1676401713532"></a>

### SIMT内存层次结构<a name="section_simt_memory"></a>

- **寄存器**：每个线程独立的寄存器，用于存储局部变量，生命周期与核函数一致。可用寄存器数量与线程块中线程数有关。
- **栈空间**：每个线程独立的堆栈空间，用于函数调用时的上下文保存和局部数据存储。
- **Share Memory**：线程块内所有线程共享的本地内存（Unified Buffer），生命周期与线程块一致。
- **Global Memory**：所有线程均可通过Data Cache访问的全局内存。

### SIMD内存层次结构<a name="section_simd_memory"></a>

- **Register File（RF）**：SIMD的向量寄存器文件，包含多种类型的Reg。
- **Unified Buffer**：RF中所有Reg共享的本地内存。
- **Global Memory**：所有核共享的全局内存。

### 内存层级示意图<a name="section_memory_diagram"></a>

**图 2** SIMD与SIMT混合编程内存模型示意图<a name="fig237411381089"></a>  
![](../../../figures/SIMD与SIMT混合编程内存模型示意图.png "SIMD与SIMT混合编程内存模型示意图")

## UB内存分配<a name="section3725125414229"></a>

UB（Unified Buffer）内存空间总大小为256KB，按功能划分为四个主要区域，从低地址向高地址依次为静态内存、动态内存、预留空间、Data Cache。

**图 3** UB内存分配图<a name="fig184031623113510"></a>  
![](../../../figures/UB内存分配图.png "UB内存分配图")

### 内存区域说明<a name="section_memory_regions"></a>

1. **静态内存**：从内存起始地址分配，大小在编译时确定，不可动态修改。

    ```cpp
    // 静态内存通过数组分配，例如：
    __ubuf__ char staticBuf[1024];
    ```

2. **动态内存**：位于静态内存之后，通过执行配置中参数dynUBufSize指定大小，可通过以下方式申请：
    - 通过TPipe的相关接口申请
    - 通过LocalMemAllocator的Alloc接口申请
    - 使用动态数组分配（后续版本支持）

    ```cpp
    // 动态内存通过动态数组分配，例如：
    extern __ubuf__ char dynamicBuf[];
    ```

    >[!NOTE]说明 
    >上述两种方法申请动态内存时均从静态内存结束位置之后开始分配，如果同时使用可能导致地址空间重叠，引发未定义行为，因此只能选择其中一种方法。

3. **预留空间**：编译器和Ascend C预留空间，大小固定为8KB。

4. **Data Cache**：SIMT专有的Data Cache空间，内存大小必须大于或等于32KB。

### 内存计算规则<a name="section_memory_calculation"></a>

```
DataCache = UB总大小（256KB） – 静态内存 – 动态内存 – 预留空间（8KB）
```

>[!NOTE]说明 
>- 若DataCache小于32KB，会出现校验报错。
>- 在SIMD与SIMT混合编程场景下，算子内部不能使用全部Unified Buffer空间，需为SIMT预留至少32KB的Data Cache空间。
>- 动态内存的动态数组分配方式目前开发中，将在后续版本支持。

## 函数层级与定义<a name="section13964105005215"></a>

SIMD与SIMT混合编程涉及多种类型的函数，它们之间遵循严格的调用关系和层级约束。本章采用总分结构，首先介绍整体函数类型和调用关系，再详细展开每种函数的定义、调用语法及约束。

### 函数类型总览<a name="section_function_overview"></a>

SIMD与SIMT混合编程中涉及的函数类型如下表所示：

| 函数类型 | 修饰符 | 执行位置 | 角色定位 | 调用方式 |
|---------|-------|---------|---------|---------|
| 混合核函数 | `__global__` `__aicore__` | Device侧（AI Core） | 算子入口，协调VF执行 | Host侧通过<<<...\>\>\>调用 |
| `__aicore__`函数 | `__aicore__` | Device侧（AI Core） | Device侧辅助函数 | 核函数或同级函数调用 |
| SIMT VF函数 | `__simt_vf__` | Device侧（AI Core） | 线程级并行计算任务 | 通过asc_vf_call接口调用 |
| SIMD VF函数 | `__simd_vf__` | Device侧（AI Core） | 向量级并行计算任务 | 通过asc_vf_call接口调用 |
| `__simt_callee__`子函数 | `__simt_callee__` | Device侧（AI Core） | SIMT VF的辅助子函数 | SIMT VF内部调用 |
| `__simd_callee__`子函数 | `__simd_callee__` | Device侧（AI Core） | SIMD VF的辅助子函数 | SIMD VF内部调用 |

**函数层级架构：**

```
Host侧
  └─ 核函数调用<<<...>>>（入口）
      └─ Device侧
          ├─ 混合核函数（__global__ __aicore__）
          │   ├─ 调用 __aicore__函数
          │   ├─ 调用 SIMT VF函数（通过asc_vf_call）
          │   └─ 调用 SIMD VF函数（通过asc_vf_call）
          │
          ├─ SIMT VF函数（__simt_vf__）
          │   └─ 调用 __simt_callee__子函数
          │
          ├─ SIMD VF函数（__simd_vf__）
          │   └─ 调用 __simd_callee__子函数
          │
          └─ __aicore__函数
              └─ 调用同级 __aicore__函数或VF函数
```

### 函数调用关系图<a name="section_call_diagram"></a>

**图 4** 函数调用关系图<a name="fig2064152983610"></a>  
![](../../../figures/函数调用关系图.png "函数调用关系图")

**调用关系规则：**

| 函数类型 | 可调用函数类型 | 不可调用函数类型 |
|---------|--------------|----------------|
| 混合核函数 | `__aicore__`函数、SIMT VF函数、SIMD VF函数 | `__simt_callee__`、`__simd_callee__` |
| `__aicore__`函数 | `__aicore__`函数、SIMT VF函数、SIMD VF函数 | `__simt_callee__`、`__simd_callee__` |
| SIMT VF函数 | `__simt_callee__`函数、constexpr函数 | `__aicore__`函数、SIMD VF函数、`__simd_callee__` |
| SIMD VF函数 | `__simd_callee__`函数、constexpr函数 | `__aicore__`函数、SIMT VF函数、`__simt_callee__` |
| `__simt_callee__`子函数 | `__simt_callee__`函数、constexpr函数 | 其他所有函数 |
| `__simd_callee__`子函数 | `__simd_callee__`函数、constexpr函数 | 其他所有函数 |

### 混合核函数<a name="section_hybrid_kernel"></a>

混合核函数是SIMD与SIMT混合编程的Device侧入口函数，负责协调整个算子的执行流程，包括VF的调度和调用。

#### 定义语法<a name="section_hybrid_kernel_syntax"></a>

```cpp
__global__ __aicore__ void kernel_name(__gm__ type* param1, __gm__ type* param2, ...);
```

**修饰符说明：**

| 修饰符 | 作用 | 必需性 |
|-------|------|-------|
| `__global__` | 标识核函数，表明可在Host侧通过<<<...\>\>\>调用 | 必需 |
| `__aicore__` | 标识函数在Device侧AI Core上执行 | 必需 |
| `__gm__` | 标识指针参数指向Global Memory | 指针参数必需 |

**入参约束：**

- 入参支持指针类型（需使用`__gm__`修饰）和C/C++内置数据类型
- 指针参数指向Global Memory上的内存地址
- 建议使用GM_ADDR宏统一表达：`#define GM_ADDR __gm__ uint8_t*`

**返回值：**

- 必须具有void返回类型

#### 调用方式<a name="section_hybrid_kernel_call"></a>

混合核函数通过<<<...\>\>\>内核调用符在Host侧调用：

```cpp
kernel_name<<<numBlocks, dynUBufSize, stream>>>(args...);
```

**执行配置参数：**

| 参数 | 类型 | 说明 | 约束 |
|------|------|------|------|
| numBlocks | int32_t或dim3 | 设置核函数启用的核数 | 取值范围[1, 65535]，建议设置为物理核数或其倍数 |
| dynUBufSize | uint32_t | 指定动态内存大小 | 不超过最大可配置值：256KB - 8KB - 32KB - 静态内存 |
| stream | aclrtStream | 用于维护异步操作执行顺序 | 由Runtime API创建 |

**调用约束：**

- 核函数调用是异步的，调用后控制权立即返回Host侧
- 可通过`aclrtSynchronizeStream`等待核函数执行完毕
- 开发者需保证核函数内使用的动态内存不超过dynUBufSize

#### 编程示例<a name="section_hybrid_kernel_example"></a>

```cpp
// 定义混合核函数
__global__ __aicore__ void gather_and_adds_kernel(
    __gm__ float* input, 
    __gm__ uint32_t* index, 
    __gm__ float* output,
    uint32_t input_total_length, 
    uint32_t index_total_length)
{
    KERNEL_TASK_TYPE_DEFAULT(KERNEL_TYPE_AIV_ONLY);
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> ub_allocator;
    
    // 协调SIMT VF和SIMD VF执行
    uint32_t length_per_block = index_total_length / AscendC::GetBlockNum();
    AscendC::LocalTensor<float> gather_output = ub_allocator.Alloc<float>(length_per_block);
    
    // 调用SIMT VF
    asc_vf_call<simt_gather>(dim3(THREAD_COUNT), input, index, 
                             (__ubuf__ float*)gather_output.GetPhyAddr(), ...);
    
    // 调用SIMD VF
    asc_vf_call<simd_adds>((__ubuf__ float*)adds_output.GetPhyAddr(), ...);
    
    // 数据搬出
    AscendC::DataCopy(output_global_tensor, adds_output, length_per_block);
}

// Host侧调用
int main() {
    gather_and_adds_kernel<<<numBlocks, dynUBufSize, stream>>>(input, index, output, ...);
    aclrtSynchronizeStream(stream);
}
```

### SIMT VF函数<a name="section_simt_vf_detail"></a>

SIMT VF函数用于实现线程级并行计算任务，处理不规则访问和复杂控制逻辑。

#### 定义语法<a name="section_simt_vf_syntax"></a>

```cpp
__simt_vf__ __launch_bounds__(thread_num) inline void function_name(
    __gm__ type* gm_param, 
    __ubuf__ type* ubuf_param, 
    type scalar_param, ...);
```

**修饰符说明：**

| 修饰符 | 作用 | 必需性 |
|-------|------|-------|
| `__simt_vf__` | 标识SIMT VF函数 | 必需 |
| `__launch_bounds__(N)` | 指定最大线程数 | 可选，默认1024 |
| `inline` | 建议内联 | 建议添加 |
| `__gm__` | 标识指针指向Global Memory | 指针参数可选 |
| `__ubuf__` | 标识指针指向Unified Buffer | 指针参数可选 |

**线程数配置：**

- `__launch_bounds__(thread_num)`用于编译期指定核函数启动的最大线程数
- 不配置时默认为1024
- 线程数影响每个线程可用的寄存器数量（参见寄存器资源分配表）

#### 调用方式<a name="section_simt_vf_call"></a>

通过`asc_vf_call`接口在核函数或`__aicore__`函数中调用：

```cpp
asc_vf_call<function_name>(dim3(blockDim), arg1, arg2, ...);
```

**调用参数：**

| 参数 | 说明 |
|------|------|
| function_name | SIMT VF函数名（模板参数） |
| dim3(blockDim) | 线程块维度配置 |
| arg1, arg2, ... | 函数入参 |

#### 编程约束<a name="section_simt_vf_constraints_detail"></a>

- 入参仅支持PoD（Plain Old Data）数据类型
- 函数必须具有void返回类型
- SIMT VF内只能调用`__simt_callee__`函数和constexpr函数
- 可使用SIMT内置变量：threadIdx、blockIdx、blockDim、gridDim
- 可使用SIMT同步接口：asc_syncthreads、asc_threadfence、asc_threadfence_block

#### 编程示例<a name="section_simt_vf_example"></a>

```cpp
// SIMT VF函数定义
__simt_vf__ __launch_bounds__(128) inline void simt_gather(
    __gm__ float* input,
    __gm__ uint32_t* index,
    __ubuf__ float* output,
    uint32_t total_length)
{
    // 使用线程索引计算数据位置
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_length) return;
    
    // 直接访问Global Memory和Unified Buffer
    uint32_t gather_idx = index[idx];
    output[threadIdx.x] = input[gather_idx];
}

// 核函数中调用
asc_vf_call<simt_gather>(dim3(128), input, index, output, total_length);
```

### SIMD VF函数<a name="section_simd_vf_detail"></a>

SIMD VF函数用于实现向量级并行计算任务，处理规则的批量数据处理。

#### 定义语法<a name="section_simd_vf_syntax"></a>

```cpp
__simd_vf__ inline void function_name(
    __gm__ type* gm_param, 
    __ubuf__ type* ubuf_param, 
    type scalar_param, ...);
```

**修饰符说明：**

| 修饰符 | 作用 | 必需性 |
|-------|------|-------|
| `__simd_vf__` | 标识SIMD VF函数 | 必需 |
| `inline` | 建议内联 | 建议添加 |
| `__gm__` | 标识指针指向Global Memory | 指针参数可选 |
| `__ubuf__` | 标识指针指向Unified Buffer | 指针参数可选 |

#### 调用方式<a name="section_simd_vf_call"></a>

通过`asc_vf_call`接口在核函数或`__aicore__`函数中调用：

```cpp
asc_vf_call<function_name>(arg1, arg2, ...);
```

**注意：** SIMD VF调用不需要dim3参数。

#### 编程约束<a name="section_simd_vf_constraints_detail"></a>

- 入参仅支持PoD（Plain Old Data）数据类型
- 函数必须具有void返回类型
- SIMD VF内只能调用`__simd_callee__`函数和constexpr函数
- 不支持直接从Global Memory搬运数据到向量寄存器，需先搬运到Unified Buffer
- 使用向量寄存器API（如RegTensor、LoadAlign、StoreAlign等）进行计算

#### 编程示例<a name="section_simd_vf_example"></a>

```cpp
// SIMD VF函数定义
__simd_vf__ inline void simd_adds(
    __ubuf__ float* output, 
    __ubuf__ float* input,
    uint32_t count, 
    uint32_t repeat_size, 
    uint16_t repeat_times)
{
    AscendC::Reg::RegTensor<float> src_reg, dst_reg;
    AscendC::Reg::MaskReg mask_reg;
    
    for (uint16_t i = 0; i < repeat_times; i++) {
        mask_reg = AscendC::Reg::UpdateMask<float>(count);
        AscendC::Reg::LoadAlign(src_reg, input + i * repeat_size);
        AscendC::Reg::Adds(dst_reg, src_reg, 1.0f, mask_reg);
        AscendC::Reg::StoreAlign(output + i * repeat_size, dst_reg, mask_reg);
    }
}

// 核函数中调用
asc_vf_call<simd_adds>(output, input, count, repeat_size, repeat_times);
```

### `__aicore__`函数<a name="section_aicore_function"></a>

`__aicore__`函数是在Device侧AI Core上执行的辅助函数，用于封装可复用的逻辑。

#### 定义语法<a name="section_aicore_syntax"></a>

```cpp
__aicore__ void function_name(type param1, type param2, ...);
```

**修饰符说明：**

| 修饰符 | 作用 | 必需性 |
|-------|------|-------|
| `__aicore__` | 标识函数在AI Core上执行 | 必需 |

#### 调用方式<a name="section_aicore_call"></a>

在核函数或其他`__aicore__`函数中直接调用：

```cpp
function_name(arg1, arg2, ...);
```

#### 编程约束<a name="section_aicore_constraints"></a>

- 可以调用同级`__aicore__`函数、SIMT VF函数、SIMD VF函数
- 可以访问Global Memory和Unified Buffer
- 可使用Ascend C API接口

### `__simt_callee__`子函数<a name="section_simt_callee"></a>

`__simt_callee__`子函数是SIMT VF函数内部调用的辅助函数。

#### 定义语法<a name="section_simt_callee_syntax"></a>

```cpp
__simt_callee__ void/return_type function_name(type param1, type param2, ...);
```

**修饰符说明：**

| 修饰符 | 作用 | 必需性 |
|-------|------|-------|
| `__simt_callee__` | 标识SIMT VF内部可调用的子函数 | 必需 |

#### 调用方式<a name="section_simt_callee_call"></a>

在SIMT VF函数内部直接调用：

```cpp
return_type result = function_name(arg1, arg2, ...);
```

#### 编程约束<a name="section_simt_callee_constraints"></a>

- 仅能在SIMT VF函数内部调用
- 只能调用同级`__simt_callee__`函数和constexpr函数
- 可有返回值或通过引用传参
- 可使用SIMT内置变量和同步接口

### `__simd_callee__`子函数<a name="section_simd_callee"></a>

`__simd_callee__`子函数是SIMD VF函数内部调用的辅助函数。

#### 定义语法<a name="section_simd_callee_syntax"></a>

```cpp
__simd_callee__ void/return_type function_name(type param1, type param2, ...);
```

**修饰符说明：**

| 修饰符 | 作用 | 必需性 |
|-------|------|-------|
| `__simd_callee__` | 标识SIMD VF内部可调用的子函数 | 必需 |

#### 调用方式<a name="section_simd_callee_call"></a>

在SIMD VF函数内部直接调用：

```cpp
return_type result = function_name(arg1, arg2, ...);
```

#### 编程约束<a name="section_simd_callee_constraints"></a>

- 仅能在SIMD VF函数内部调用
- 只能调用同级`__simd_callee__`函数和constexpr函数
- 可有返回值或通过引用传参
- 可使用向量寄存器API

### PoD数据类型说明<a name="section_pod_type"></a>

SIMD VF和SIMT VF函数的入参仅支持PoD（Plain Old Data）数据类型。

**支持的类型：**

- 基础数据类型：int32_t、uint32_t、float、half等
- 基础数据类型的指针类型
- 基础数据类型组成的数组
- 基础数据类型组成的结构体（不含特殊成员函数）

**不支持的类型：**

- 含构造函数、析构函数的类或结构体
- 含复制构造函数、复制赋值操作符的类或结构体
- 含非静态成员函数或虚函数的类或结构体

### 函数定义完整示例<a name="section_complete_example"></a>

以下示例展示SIMD与SIMT混合编程中各类函数的完整定义和调用关系：

```cpp
// 1. SIMT VF函数定义
__simt_vf__ __launch_bounds__(128) inline void simt_gather(
    __gm__ float* input, __gm__ uint32_t* index, __ubuf__ float* output, 
    uint32_t total_length)
{
    // 可调用__simt_callee__子函数
    gather_impl(input, index, output, total_length);
}

// 2. __simt_callee__子函数定义
__simt_callee__ void gather_impl(
    __gm__ float* input, __gm__ uint32_t* index, __ubuf__ float* output,
    uint32_t total_length)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < total_length) output[threadIdx.x] = input[index[idx]];
}

// 3. SIMD VF函数定义
__simd_vf__ inline void simd_adds(
    __ubuf__ float* output, __ubuf__ float* input, 
    uint32_t count, uint16_t repeat_times)
{
    // 可调用__simd_callee__子函数
    adds_impl(output, input, count, repeat_times);
}

// 4. __simd_callee__子函数定义
__simd_callee__ void adds_impl(
    __ubuf__ float* output, __ubuf__ float* input,
    uint32_t count, uint16_t repeat_times)
{
    AscendC::Reg::RegTensor<float> src_reg, dst_reg;
    AscendC::Reg::MaskReg mask_reg;
    
    for (uint16_t i = 0; i < repeat_times; i++) {
        mask_reg = AscendC::Reg::UpdateMask<float>(count);
        AscendC::Reg::LoadAlign(src_reg, input + i * count);
        AscendC::Reg::Adds(dst_reg, src_reg, 1.0f, mask_reg);
        AscendC::Reg::StoreAlign(output + i * count, dst_reg, mask_reg);
    }
}

// 5. __aicore__函数定义（可选）
__aicore__ void init_allocator(AscendC::LocalMemAllocator<AscendC::Hardware::UB>& allocator)
{
    // Device侧辅助逻辑
}

// 6. 混合核函数定义
__global__ __aicore__ void gather_adds_kernel(
    __gm__ float* input, __gm__ uint32_t* index, __gm__ float* output,
    uint32_t input_len, uint32_t index_len)
{
    KERNEL_TASK_TYPE_DEFAULT(KERNEL_TYPE_AIV_ONLY);
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> ub_allocator;
    
    // 可调用__aicore__函数
    init_allocator(ub_allocator);
    
    uint32_t len_per_block = index_len / AscendC::GetBlockNum();
    AscendC::LocalTensor<float> temp = ub_allocator.Alloc<float>(len_per_block);
    
    // 调用SIMT VF
    asc_vf_call<simt_gather>(dim3(128), input, index, 
                             (__ubuf__ float*)temp.GetPhyAddr(), len_per_block);
    
    // 调用SIMD VF
    asc_vf_call<simd_adds>((__ubuf__ float*)output, (__ubuf__ float*)temp.GetPhyAddr(),
                           len_per_block, 1);
}

// 7. Host侧调用
int main() {
    aclrtStream stream;
    aclrtCreateStream(&stream);
    
    gather_adds_kernel<<<1, 64KB, stream>>>(input, index, output, 1000, 100);
    aclrtSynchronizeStream(stream);
}
```

## 编程示例<a name="section776244992018"></a>

以下示例展示SIMD与SIMT混合编程实现Gather与Adds算子，完整代码请参见[SIMD与SIMT混合编程实现gather&adds算子样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/00_introduction/03_fusion_operation/simt_gather_and_simd_adds)。

### 示例解析<a name="section_example_analysis"></a>

本示例实现两个计算任务：
1. **SIMT VF（simt_gather）**：根据索引数组从输入数据中收集元素，处理不规则访问
2. **SIMD VF（simd_adds）**：对收集的元素执行向量加法操作，处理规则计算

```cpp
// SIMT VF：处理不规则Gather操作
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void simt_gather(
    __gm__ float* input,
    __gm__ uint32_t* index,
    __ubuf__ float* gather_output,
    uint32_t input_total_length,
    uint32_t index_total_length,
    uint32_t output_total_length)
{
    if (threadIdx.x >= output_total_length) {
        return;
    }
    // blockIdx will be supported later.
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= index_total_length) {
        return;
    }

    uint32_t gather_idx = index[idx];
    if (gather_idx >= input_total_length) {
        return;
    }

    gather_output[threadIdx.x] = input[gather_idx];
}

// SIMD VF：处理规则向量加法
__simd_vf__ inline void simd_adds(__ubuf__ float* output, __ubuf__ float* input,
    uint32_t count, uint32_t one_repeat_size, uint16_t repeat_times)
{
    AscendC::Reg::RegTensor<float> src_reg0;
    AscendC::Reg::RegTensor<float> dst_reg0;
    AscendC::Reg::MaskReg mask_reg; 

    for (uint16_t i = 0; i < repeat_times; i++) {
        mask_reg = AscendC::Reg::UpdateMask<float>(count);
        AscendC::Reg::LoadAlign(src_reg0, input + i * one_repeat_size);
        AscendC::Reg::Adds(dst_reg0, src_reg0, ADDS_ADDEND, mask_reg);
        AscendC::Reg::StoreAlign(output + i * one_repeat_size, dst_reg0, mask_reg);
    }
}

// 混合编程核函数：协调SIMT VF和SIMD VF执行
__global__ __aicore__ void gather_and_adds_kernel(__gm__ float* input, __gm__ uint32_t* index, __gm__ float* output,
    uint32_t input_total_length, uint32_t index_total_length)
{
    KERNEL_TASK_TYPE_DEFAULT(KERNEL_TYPE_AIV_ONLY);
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> ub_allocator;
    // 1. gather numbers from input.
    uint32_t index_total_length_per_block = index_total_length / AscendC::GetBlockNum();
    AscendC::LocalTensor<float> gather_output = ub_allocator.Alloc<float>(index_total_length_per_block);
    asc_vf_call<simt_gather>(dim3(THREAD_COUNT),input, index,
                             (__ubuf__ float *)gather_output.GetPhyAddr(),
                             input_total_length,
                             index_total_length,
                             index_total_length_per_block);

    // 2. use reg compute api to do addition.
    AscendC::LocalTensor<float> adds_output = ub_allocator.Alloc<float>(index_total_length_per_block);
    constexpr uint32_t one_repeat_size = AscendC::GetVecLen() / sizeof(float);
    uint16_t repeat_times = (index_total_length_per_block + one_repeat_size - 1) / one_repeat_size;
    asc_vf_call<simd_adds>((__ubuf__ float *)adds_output.GetPhyAddr(),
        (__ubuf__ float *)gather_output.GetPhyAddr(), index_total_length_per_block, one_repeat_size, repeat_times);

    AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(0);
    AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(0);

    // 3. copy data to global memory.
    AscendC::GlobalTensor<float> output_global_tensor;
    output_global_tensor.SetGlobalBuffer(output + index_total_length_per_block * AscendC::GetBlockIdx());
    AscendC::DataCopy(output_global_tensor, adds_output, index_total_length_per_block);
}

int main(int argc, char *argv[])
{
     …
     //numBlocks only supports one dimension currently.
     gather_and_adds_kernel<<<numBlocks, dynUBufSize, stream>>>(input_device, index_device, output_device, input_total_length, index_total_length);
     …
}
```

### 关键代码说明<a name="section_key_code"></a>

1. **SIMT VF（simt_gather）**：
   - 使用`__simt_vf__`修饰符定义
   - 通过`threadIdx.x`和`blockIdx.x`计算线程索引
   - 直接访问Global Memory（`input[gather_idx]`）和Unified Buffer（`gather_output[threadIdx.x]`）
   - 处理索引边界检查和Gather逻辑

2. **SIMD VF（simd_adds）**：
   - 使用`__simd_vf__`修饰符定义
   - 通过向量寄存器（RegTensor）进行批量数据加载和计算
   - 使用LoadAlign、Adds、StoreAlign等向量操作接口
   - 处理规则的向量加法操作

3. **核函数（gather_and_adds_kernel）**：
   - 使用`__global__` `__aicore__`修饰符定义
   - 通过`asc_vf_call`接口分别调用SIMT VF和SIMD VF
   - 使用LocalMemAllocator分配Unified Buffer内存
   - 协调两种VF的执行顺序和数据传递

## 编程建议<a name="section_programming_guidelines"></a>

### VF类型选择<a name="section_vf_selection"></a>

| 场景特征 | 推荐VF类型 | 原因 |
|-------|-----------|------|
| 规则、连续的数据访问 | SIMD VF | 充分利用向量寄存器，提高计算吞吐量 |
| 离散、不规则的访问模式 | SIMT VF | 线程可独立寻址，简化复杂逻辑 |
| 包含大量分支判断 | SIMT VF | 可处理分支发散，控制复杂度低 |
| 简单的逐元素运算 | SIMD VF | 无分支开销，执行效率高 |
| 数据依赖复杂 | SIMT VF | 线程独立执行，便于处理依赖关系 |

### Thread Block配置建议<a name="section_block_config"></a>

- **Warp对齐**：blockDim应为32的整数倍（如32、64、128、256、512、1024）
- **资源平衡**：根据算子复杂度合理配置线程数量，避免寄存器溢出
- **占用率优化**：合理的blockDim可提高硬件占用率

### 分支优化建议<a name="section_branch_optimization"></a>

- **分支对齐**：组织数据使得同一Warp内的线程尽可能执行相同路径
- **分支重构**：使用条件优化技术减少Warp内的分支数量
- **边界处理**：始终进行边界检查，防止数组越界访问

### 内存优化建议<a name="section_memory_optimization"></a>

- **合并访问**：同一Warp中的线程应尽量访问相邻的内存地址
- **数据复用**：将频繁访问的数据缓存到Share Memory
- **合理划分内存**：平衡Share Memory和Data Cache空间
- **避免全部占用UB**：为SIMT预留足够的Data Cache空间（至少32KB）

## 相关参考<a name="section_reference"></a>

- [SIMT编程模型详解](../AI-Core-SIMT编程/抽象硬件架构.md)：了解SIMT架构的详细硬件抽象
- [线程架构](../AI-Core-SIMT编程/线程架构.md)：深入了解SIMT线程层次结构和Warp机制
- [同步机制](../AI-Core-SIMT编程/同步机制.md)：了解SIMT线程同步接口的使用方法
- [SIMD编程模型](抽象硬件架构.md)：了解SIMD架构的详细硬件抽象
- [核函数定义](核函数.md)：了解核函数的定义和调用规则