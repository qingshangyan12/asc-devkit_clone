# C语言编程概述

Ascend C为AI Core提供了SIMD编程路径，**C语言编程**是该路径的底层核心接口，直接映射NPU硬件指令，提供对向量处理单元、矩阵运算单元、本地存储等硬件资源的精细控制能力。

对于追求极致性能、习惯C语言编程的开发者，C语言编程是解锁NPU硬件潜能的关键技术路径。本章旨在帮助开发者建立对C语言编程的系统化认知，理解其底层技术定位、核心编程范式与开发方法论。

---

## 一、背景动因

早期算子开发以C语言裸指针编程为核心，其背景动因源于GPU硬件架构的复杂性——缓存、寄存器、共享内存等片上资源需精确调度。该模式依托C语言底层内存操控能力，通过裸指针直接寻址操作设备内存，绕过抽象层实现零开销访问；手动分配片上内存、插入显式同步语句，精确编排硬件执行时序；原生函数与硬件指令精准映射、调用即执行，无中间转换损耗。

支持向量计算、矩阵乘、数据搬运等核心原语调用，是底层极致性能优化的核心方案，广泛应用于离散、归约、矩阵类等算子的底层实现。

---

## 二、编程范式

C语言编程提供完备的编程能力，是SIMD编程路径中最贴近硬件的底层接口。开发者通过声明静态数组管理本地内存，感知并手动控制同步细节，直接调用C风格的Vector矢量计算和Cube矩阵计算接口，实现指令级精细控制。

### 2.1 Kernel函数

[核函数](../核函数.md)在前序章节已作介绍，C语言编程在核函数定义与调用上沿用SIMD编程通用规范，无额外语法约束。


### 2.2 内存管理

AI Core采用分级存储架构，不同计算单元的编程视角有所差异：

**Cube矩阵计算**：采用「Global Memory → L1/L0系列Buffer」两级层级，其中L1 Buffer、L0A/L0B Buffer、L0C Buffer均服务于矩阵计算。

**Vector矢量计算**：传统架构采用「Global Memory → UB」两级层级；Ascend 950PR/950DT引入寄存器后构建「Global Memory → UB → Register」三级层级。**Global Memory**存放输入输出数据，**UB**作为矢量计算数据枢纽，**Register**位于Vector计算单元最内层，具备最低访存延迟与最高带宽。

<img src="../../../../figures/AI%20Core存储层次结构.png" alt="AI Core存储层次结构" width="800" />

---

### 2.3 外部存储（Global Memory）

Global Memory是昇腾NPU的设备内存，位于AI Core外部，容量大、带宽高但访问延迟相对较长。用于存储算子的输入数据、输出结果和中间计算数据。运行在Device侧的核函数可直接访问Global Memory。

Global Memory具有持久性：通过全局内存分配的空间，其中存储的数据将持续保留，直到该内存空间被释放或应用程序终止。用户通过Runtime API完成Device侧全局内存的管理。

Host使用aclrtMalloc分配Device侧的全局内存，并通过aclrtMemcpy将数据从Host拷贝到Device侧的全局内存，或将数据从Device的全局内存拷贝回Host内存；通过aclrtMalloc分配的Device全局内存需使用aclrtFree接口释放。有关Runtime API的更多信息与细节，可以参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。

```c
__global__ __aicore__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
{
    // 核内实现
}
```

---

### 2.4 内部存储（Local Memory）

AI Core包含三类基础计算单元：Cube负责矩阵计算、Vector负责矢量计算、Scalar负责标量计算。计算前需将数据从外部存储加载到内部存储，不同存储单元服务于不同计算单元：L1 Buffer和L0 Buffer支撑矩阵计算，Unified Buffer支撑矢量计算。

#### 2.4.1 Unified Buffer

Unified Buffer是AI Core内主要用于矢量计算单元的内部存储单元，位于每个Vector Core（AIV）内部。与外部存储空间相比，Unified Buffer的容量较小，但具有更高的带宽和更低的访问延迟，可视为内核执行期间由用户管理的高速缓存资源。

在C指针编程中，用户可通过数组定义的方式动态或静态申请Unified Buffer。

**静态申请**：分配一段指定大小的内存空间，空间大小在编译时确定，不可动态修改。

```c
constexpr uint32_t ub_size = 256;
__ubuf__ half static_buffer[ub_size];  // 编译器自动32字节对齐
```

**动态申请**：用户需要通过<<<>>>中参数dynamic_ub_size指定动态内存的空间大小，其大小在运行期确定。

```c
extern __ubuf__ char dynamic_buffer[];
```

> 📌 Unified Buffer地址必须32字节对齐，以匹配硬件总线粒度和SIMD计算单元并行度，非对齐访问将导致运行时错误。

---

为了提高数据访问的效率和吞吐量，Unified Buffer采用了bank（大小相等的内存模块）结构设计。以NPU架构版本2201为例，Unified Buffer总大小为192K，划分为16个bank。每个bank由512行组成，每行长度为32B。这16个bank进一步组织为8个bank group，每个bank group包含2个bank。

<img src="UB Bank冲突示意图.png" alt="UB Bank冲突示意图" width="800" />

每个bank可以独立地进行数据的读写操作，允许多个数据请求同时进行。然而，当多个读写操作试图同时访问同一个bank，由于硬件资源的限制，这些操作必须排队等待，会导致bank冲突，引起性能下降。

**Bank冲突类型**：

| 冲突类型 | 说明 |
|---------|------|
| 读写冲突 | 读操作和写操作同时尝试访问同一个bank |
| 写写冲突 | 多个写操作同时尝试访问同一个bank group |
| 读读冲突 | 两个读操作同时尝试访问同一个bank，或者两个以上读操作同时尝试访问同一个bank group |

以读写冲突为例，结合下图所示，0x10000地址在bank16上，0x10020在bank17上，0x20020在bank33上。当要进行矢量加法计算时：

- 如果src0的地址为0x10000，dst的地址为0x10E20，则src0和dst都会在bank16上，产生读写冲突。如果dst的地址为0x10020，则dst分配在bank17，不会产生读写冲突。
- 如果src0的地址为0x10020，src1的地址为0x20020，则src0分配在bank17，src1分配在bank33，从图上可以看到，这两个bank在同一个bank group内，产生读读冲突。
- 如果dst的地址为0x10020，每次加法计算8个data block，如果每个data block的间隔stride为16，则dst的每个data block都在一个bank group内，执行该条指令时将会产生写写冲突。
  
![Unified_Buffer地址分配示意图](Unified_Buffer地址分配示意图.png)

---

**优化方法1：优化计算逻辑**

对于特定的算法计算，可通过调整内存读写顺序或方式来避免Bank冲突。以shape为(8, 16, 16)的输入执行(1, 0, 2)的transpose操作为例，通过将计算逻辑由"跳读、连续写"调整为"连续读、跳写"，可避免冲突。

<img src="../../../../figures/读读冲突.png" alt="读读冲突示意图" width="800" />


如上图所示，如果我们对src进行"跳读、连续写"的方式操作，这样src的8个data block都会分配在同一个bank group内，会产生读读冲突。
```c
for (uint16_t i = 0; i < 16; ++i) {
    // 按列进行计算，跳读、连续写
    asc_add_scalar(dst + i * 128, src + i * 16, 1, repeat, 1, 16, 8, 8);
}
```

<img src="../../../../figures/读读冲突优化.png" alt="读读冲突优化示意图" width="800" />

为了避免类似的读读冲突，通常是避免读操作时访问到同一个bank group。该场景是一个读单元，因为操作时跳着读，造成8个data block都分配到了同一个bank group内，导致读读冲突。所以这种情况，通过改写算法逻辑，将跳读调整为连续读，这样8个data block会分配到不同的8个bank group内，从而避免读读冲突。（该样例说明了如何通过调整计算逻辑优化读读冲突问题，同理写操作也会遇到类似问题）
```c
for (uint16_t i = 0; i < 8; ++i) {
    // 按列进行计算，连续读、跳写
    asc_add_scalar(dst + i * 16, src + i * 256, 1, repeat, 8, 1, 8, 8);
}
```

---

**优化方法2：优化地址分配**

相较于优化计算逻辑来避免Bank冲突，通过调整地址分配的方式避免Bank冲突更为通用。
以连续4096个float元素的加法`z = x + y`为例，通过在分配内存时适当扩大申请空间，保证在一个Repeat内，x和y不会同时位于同一个bank group，且x或y不会与z同时位于同一个bank。

<img src="../../../../figures/地址分配优化前示意图.png" alt="地址分配优化前示意图" width="800" />

如上图所示，如果我们按照要操作的内存空间大小顺序分配内存，将会发现x、y、z的地址都会分配在同一个bank/bank group内，这种情况将会产生读读冲突（x和y在同一个bank group内），读写冲突（x/y跟z在同一个bank内）。
```c
__ubuf__ float x[4096];  // 编译器统一分配，起始地址0x00000
__ubuf__ float y[4096];  // 起始地址0x04000
__ubuf__ float z[4096];  // 起始地址0x08000
asc_add(z, x, y, 4096);
```

<img src="../../../../figures/地址分配优化后示意图.png" alt="地址分配优化后示意图" width="800" />

C语言编程采用静态数组声明方式分配内存，内存地址按声明顺序连续分配。开发者可通过调整数组大小控制地址分配，实现地址错开，避免Bank冲突。

**避免读读冲突**：如上图所示，将x和y的首地址错开8个bank group空间（256字节），确保一次repeat内的8个data block分配在不同bank group。
**避免读写冲突**：确保dst（z）的地址不与x、y分配在同一bank。通过增加y的地址空间，将x/y所在的bank区域内存分配满，使z的地址从bank16开始分配。
```c
__ubuf__ float x[4096 + 64];  // 多申请256字节
__ubuf__ float y[128 * 128 - (4096 + 64)];  // 确保z不在同一Bank
__ubuf__ float z[4096];
asc_add(z, x, y, 4096);
```

---

#### 2.4.2 L1 Buffer & L0 Buffer

Cube计算单元是AI Core中专用于执行矩阵运算的单元，高亮部分展示了Cube计算单元及其直接访问的专用缓存：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer用于存储初始化累加值和矩阵计算结果。

<img src="../../../../figures/矩阵计算单元架构图.png" alt="矩阵计算单元架构图" width="800" />

**存储单元说明**：

| 存储单元 | 作用 | 地址限定符 | 示例 |
|---------|------|-----------|------|
| L1 Buffer | 缓存矩阵计算输入数据 | `__cbuf__` | `__cbuf__ half l1_buffer[512];` |
| L0A Buffer | 存储左矩阵 | `__ca__` | `__ca__ half l0a_buffer[256];` |
| L0B Buffer | 存储右矩阵 | `__cb__` | `__cb__ half l0b_buffer[256];` |
| L0C Buffer | 存储计算结果 | `__cc__` | `__cc__ half l0c_buffer[256];` |
| Fixpipe Buffer | 存储量化参数 | `__fbuf__` | `__fbuf__ half fixpipe_buffer[128];` |
| BiasTable Buffer | 存储Bias数据 | `__biasbuf__` | `__biasbuf__ half bias_buffer[64];` |

```c
constexpr uint32_t m_size = 256;
constexpr uint32_t n_size = 128;
constexpr uint32_t k_size = 256;

__cbuf__ half l1a_buffer[m_size * k_size];  // L1 Buffer，32字节对齐
__cbuf__ half l1b_buffer[n_size * k_size];
__ca__ half l0a_buffer[m_size * k_size];    // L0A Buffer，512字节对齐
__cb__ half l0b_buffer[n_size * k_size];    // L0B Buffer，512字节对齐
__cc__ half l0c_buffer[m_size * n_size];    // L0C Buffer，64字节对齐
```

同时为了对矩阵乘法等计算密集型任务进行优化，提升Cube计算效率，AI Core对于L1 Buffer、L0 Buffer都采用了分形格式：
- L1 Buffer  : FRACTAL_NZ
- L0A Buffer : FRACTAL_NZ (Ascend 950PR/Ascend 950DT架构该内存分形为NZ, Ascend 910B为ZZ分形)
- L0A Buffer : FRACTAL_ZN
- L0C Buffer : FRACTAL_NZ

以`FRACTAL_NZ`为例，内存排布如下图所示，整块内存通过大N外部column major（列主序）+ 小z内部row major（行主序）存放。

![FRACTAL_NZ内存排布示意图](FRACTAL_NZ内存排布示意图.png)
---

### 2.5 寄存器（Register）

Ascend 950PR/Ascend 950DT新一代架构引入寄存器可编程能力，Vector计算单元的所有计算均需经「GM → UB → Register」三级数据流转，在寄存器上完成矢量计算。C语言编程通过直接定义寄存器变量方式进行寄存器编程。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。

**寄存器类型说明**：

| 寄存器名称 | 寄存器类型 | 寄存器描述 | 使用场景 |
|-----------|-----------|-----------|----------|
| 矢量数据寄存器 | vector_half、vector_float、vector_int8_t、vector_bfloat16_t | 用于矢量计算，位宽为VL，可存储VL/sizeof(T)个数据元素 | 矢量计算的基本存储单元 |
| 掩码寄存器 | vector_bool | 指示计算过程中哪些元素参与计算，宽度为VL/8 | 控制矢量计算中的元素参与情况 |
| 地址寄存器 | iter_reg | 存储地址偏移量，循环中根据stride自动自增 | 循环迭代中自动管理地址偏移 |
| 非对齐寄存器 | vector_load_unalign、vector_store_unalign | 处理起始地址未满足32字节对齐要求的数据搬运 | UB和寄存器之间不对齐地址访问 |

---

## 三、同步机制

在编程中，同步是指协调多个执行单元（如线程、进程等）对共享资源的访问顺序和时机来确保程序的正确性。如果没有同步来严格控制执行顺序，则会出现结果不一致、行为不可预测等多种问题。

AI Core的同步主要分为**核内同步**和**核间同步**。

---

### 3.1 核内同步

#### 为什么需要核内同步

AI Core内部的执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行的方式运行，在读写同一存储资源时可能存在数据依赖关系。为确保数据一致性及计算正确性，需通过同步控制协调操作时序。

**2201硬件架构图**：

<img src="../../../../figures/2201_硬件架构图.png" alt="2201硬件架构图" width="800" />

**3510硬件架构图**：

<img src="../../../../figures/3510_硬件架构图.png" alt="3510硬件架构图" width="800" />

---

**Vector矢量计算流程**：

当我们描述一个常见的Vector矢量计算时，需要执行以下三个步骤：

1. 通过DMA将数据从Global Memory搬入到Local Memory
2. 进行矢量计算
3. 通过DMA将数据从Local Memory搬出到Global Memory

<img src="../../../../figures/Vector计算数据流.png" alt="Vector计算数据流" width="800" />

从上图可以看到，执行一个Vector矢量计算时，四个执行单元`Scalar`、`Vector`、`DMA(MTE2)`、`DMA(MTE3)`并行执行，如果访问同一片Local Memory，需要同步机制来控制访问时序。

<img src="../../../../figures/同步顺序.png" alt="同步顺序示意图" width="800" />

---

#### 流水类型说明

| 流水类型 | 含义 |
|---------|------|
| **PIPE_S** | 标量流水线，使用GlobalTensor/LocalTensor的GetValue函数时为此流水 |
| **PIPE_V** | 矢量计算流水及部分硬件架构下的L0C Buffer→UB数据搬运流水 |
| **PIPE_M** | 矩阵计算流水 |
| **PIPE_MTE1** | L1 Buffer→L0A Buffer、L1 Buffer→L0B Buffer数据搬运流水 |
| **PIPE_MTE2** | GM→L1 Buffer、GM→UB等数据搬运流水 |
| **PIPE_MTE3** | UB→GM等数据搬运流水 |
| **PIPE_FIX** | L0C Buffer→GM、L0C Buffer→L1等数据搬运流水 |

---

#### 核内同步关键特性

为了保证不同计算单元的执行流水能够正确执行，AI Core根据不同的应用场景提供了多种同步能力。

**核内同步机制说明**：

| 同步范围 | 同步类型 | C语言编程API | 适用场景 |
|---------|---------|-------------|----------|
| 核内同步 | 流水互斥同步  | `asc_lock`<br>`asc_unlock` | 通过锁定指定流水再释放流水来完成流水间的同步依赖 |
| 核内同步 | 核内多流水同步 | `asc_sync_notify`<br>`asc_sync_wait`<br>`asc_sync`<br>`asc_sync_pipe` | 多条流水线间数据依赖同步 |
| 核内同步 | 同一流水内同步 | `asc_sync_pipe` | 同一流水线内部约束执行顺序 |
| 核内同步 | 内存顺序访问控制 | `asc_sync_data_barrier` | 后续执行指令对前序的内存访问存在数据依赖时控制同步 |


> 📌 流水互斥同步自Ascend 950PR/950DT引入

---

#### 流水互斥同步

该同步能力从Ascend 950PR/950DT开始引入并使用，AI Core提供了类似传统CPU中锁机制的同步能力。通过锁定指定流水再释放流水来完成流水间的同步依赖。

- `asc_lock`：根据`mutex_id`获取Mutex，并阻塞当前流水指令队列，直到对应的`mutex_id`被Unlock
- `asc_unlock`：等待当前流水的前置指令退出后，根据`mutex_id`释放对应的Mutex
```c
// 锁定 MTE2 流水线互斥锁
uint8_t mutex_id = 1;
asc_lock(PIPE_MTE2, mutex_id);
asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));
asc_unlock(PIPE_MTE2, mutex_id);

// 锁定 Vector 流水线互斥锁
asc_lock(PIPE_V, mutex_id);
asc_add(shared_buffer, x_local, y_local);
asc_unlock(PIPE_V, mutex_id);
```

---

#### 核内多流水同步

该同步主要用于核内多流水间的同步，通过`asc_sync_notify/asc_sync_wait`对两条流水进行`notify`和`wait`处理。

- `asc_sync_notify`：当源流水的前序指令的所有读写操作都完成之后，当前指令开始执行，并将硬件中的对应标志位设置为1
- `asc_sync_wait`：当目的流水执行到该指令时，如果发现硬件中对应标志位为0，目的流水的后续指令将一直被阻塞；如果发现硬件中对应标志位为1，则将硬件中对应标志位设置为0，同时目的流水的后续指令开始执行

```c
// 通过DMA(MTE2)进行GM2UB的搬入处理
asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));

// 通过流水间同步保证PIPE_MTE2和PIPE_V间的同步
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

// 进行矢量计算 Add，执行流水为PIPE_V
asc_add(dst, src1, src0, total_length);
```

---

#### 同一流水内同步

同一流水线中虽然指令是顺序执行，但并不意味着后一条指令开始执行时前一条指令执行结束。所以对于同一条流水线执行，如果存在前后数据依赖，也需要进行同步处理。

```c
asc_add(dst, src1, src0, total_length);
// 后续asc_sub指令对dst存在依赖，插入asc_sync_pipe(PIPE_V)进行单流水的同步
asc_sync_pipe(PIPE_V);
asc_sub(dst, dst, src0, total_length);
```

可以直接通过`asc_sync_vector`或者`asc_sync_mte2`这些接口来直接等待某一条流水。

```c
// 通过DMA(MTE2)进行GM2UB的搬入处理
asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));
// 等待MTE2流水执行完成
asc_sync_mte2();
// 进行矢量计算 Add，执行流水为PIPE_V
asc_add(dst, src1, src0, total_length);
```

---

#### 内存访问顺序控制

当后续执行指令对前序的内存访问存在数据依赖时，可以通过`asc_sync_data_barrier`来进行内存访问顺序同步，该接口会阻塞后续的指令执行，直到所有之前的内存访问指令都执行结束。

```c
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);  // 等待所有内存访问完成
```

---

### 3.2 核间同步

当一个核计算所依赖的数据，需要等待其他相关核的计算全部完成后才能执行时，则需要通过核间同步来确保多核间的时序正确性。

以下图为例，AIC需要依赖于AIV的ReduceSum计算结果。由于整体矢量较大，必须拆分为多个部分，由每个AIV分别完成部分计算。每个AIV将其部分计算结果通过原子累加写入GM。AIC需要读取的必须是所有AIV均完成累加后的最终结果。

<img src="../../../../figures/核间同步业务场景示例.png" alt="核间同步业务场景示例" width="800" />

从上图可以看到，当前AI Core是由AIC核和AIV核组成，其中AIC核主要用于Cube计算，AIV核负责Vector计算。AIC/AIV按group进行划分。一个group内细分block及subblock，block与subblock比例为1:N（N>=1），block表示多少个"主核"，subblock表示一个"主核"带多少个"从核"。

<img src="../../../../figures/block和subblock之间关系.png" alt="block和subblock之间关系" width="800" />

算子按计算特征可分为三类：Cube算子（矩阵计算）、Vector算子（矢量计算）和Mix算子（同时包含矩阵和矢量计算）。算子类型决定了其所需的核间同步方式和可选的group配置模式。

---

#### 同步等待全部AIC或AIV执行结束

该能力为多个AIC或AIV之间的同步控制，通过`asc_sync_inter_wait`和`asc_sync_inter_arrive`的接口能力实现全部AIC或者全部AIV的同步能力。

```c
if ASCEND_IS_AIV {
    // 进行ReduceSum并搬运到聚合的GM中

    // 进行全V核的同步，等待所有核将ReduceSum的计算结果都写入到GM中
    int64_t flagID = 1;
    asc_sync_inter_arrive(PIPE_S, flagID);
    asc_sync_inter_wait(PIPE_S, flagID);
}
```

---

#### 单个AI Core内AIC与AIV之间的同步

该能力为单个AI Core内，一个group内的block跟subblock之间的同步能力。该能力为Ascend 950PR/Ascend 950DT架构新增的能力，支持AIV0与AIV1可单独触发AIC等待。

```c
// 当PIPE_FIX流水执行完毕后，AIC分别给AIV0和AIV1发起arrive的能力
if ASCEND_IS_AIC {
    asc_sync_intra_arrive(PIPE_FIX, 0);
    asc_sync_intra_arrive(PIPE_FIX, 16);
}

// AIV侧代码进行同步等待，等待AIC的同步信号后才执行PIPE_MTE3流水
if ASCEND_IS_AIV {
    asc_sync_intra_wait(PIPE_MTE3, 0);
}
```

---

## 四、计算单元

AI Core分为AIC和AIV两个独立的核，分别用于矩阵计算和向量计算。以下图为例(NPU架构版本3510), AIC核与AIV核的配比为1:2。

![硬件架构图3510](硬件架构图3510.png)

Cube计算单元和Vector计算单元分别部署在AIC核和AIV核上，每个核都有自己的Scalar单元，能独立加载自己的代码段。

### Vector计算单元

Vector计算单元主要用作向量计算，譬如Add、Sub等基础算术计算等。

在NPU架构版本2201上，Vector计算单元的数据来自于Unified Buffer（要求32B对齐），C语言编程在Vector计算单元上提供了通过UB指针直接操作计算的接口能力。

以Add计算为例，当开发者需要计算4096长度的向量加法, 可以通过`asc_add(dst, src0, src1, count)`的接口直接进行计算。
```c
__ubuf__ float x[4096 + 64];  // 多申请256字节
__ubuf__ float y[128 * 128 - (4096 + 64)];  // 确保z不在同一Bank
__ubuf__ float z[4096];

// 1. 数据搬入: 将GM的数据搬运到UB上, 该段实现忽略
// 2. 通过asc_add进行前4096个数的计算
asc_add(z, x, y, 4096);
```

如果用户计算一段非连续的向量加法，类似需要进行每间隔64个数计算64个数(总长度为4096)，开发者可以指定stride信息进行计算。
```c
__ubuf__ float x[4096 + 64];  // 多申请256字节
__ubuf__ float y[128 * 128 - (4096 + 64)];  // 确保z不在同一Bank
__ubuf__ float z[4096];

// 1. 数据搬入: 将GM的数据搬运到UB上, 该段实现忽略
// 2. 通过asc_add进行高维切分搬运，repeat_stride指定成16，每次搬运16个数据块
asc_add(z, x, y, 32, 1, 1, 1, 16, 16, 16);
```

新一代架构在传统UB缓存体系的基础上，开放寄存器（Register）可编程能力，构建出「Global Memory → UB → Register」的三级内存层级。新一代架构下的矢量计算都需要从Unified Buffer加载到Register寄存器后，再利用Register寄存器进行计算操作。

如下图所示，核函数（__global__ __aicore__）是Device侧入口函数，可调用__aicore__函数；__simd_vf__函数可被核函数调用，内部只能调用__simd_callee__子函数和constexpr aicore函数。

<img src="../../../../figures/Reg矢量计算调用层次.png" alt="Reg矢量计算调用层次" width="800" />


Reg矢量计算接口为C语言编程提供的矢量计算接口，接口定义都使用__simd_callee__作为限定符，依托寄存器低延迟、高带宽的硬件优势，专为极致性能优化场景设计了一套直接操作寄存器变量的矢量计算接口， 主要接口包括加载&存储、算术运算、比较运算、类型转换等矢量常用计算接口。
```c
// Load function: load data from UB to vector compute register variable
__simd_callee__ inline void asc_load(vector_int8_t& dst, __ubuf__  int8_t* src);
// Compute function: use vector compute register for vector computation, mainly provides arithmetic, comparison, type conversion and other common vector compute interfaces
__simd_callee__ inline void asc_add(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);
// Store function: store Reg vector compute result to UB
__simd_callee__ inline void asc_store(__ubuf__  int8_t* dst, vector_int8_t src);
```

上述矢量计算接口只能在simd vf函数中调用， 并且该函数只能通过`asc_vf_call`的方式被核函数/__aicore__函数调用
```c
__simd_vf__ inline void add_custom(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1) {
// Internal call to Reg vector compute interface for vector computation
vector_half dst_reg, src0_reg, src1_reg;
asc_load(src0_reg, src0);
// ...
}
```

核函数(使用__global__ __aicore__标识的为核函数)或者普通Device侧函数(使用__aicore__函数)通过`asc_vf_call`函数调用`simd vf`完成最终的计算
```c
__vector__ __global__ __aicore__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
__ubuf__ float x_local[TILE_LENGTH];
__ubuf__ float y_local[TILE_LENGTH];
__ubuf__ float z_local[TILE_LENGTH];
// Load GM data to UB
asc_copy_gm2ub_align((__ubuf__ float*)x_local, x, BLK_NUM, burst_length, 0, 0, true, cache_mode, src_stride, dst_stride);
asc_copy_gm2ub_align((__ubuf__ float*)y_local, y, BLK_NUM, burst_length, 0, 0, true, cache_mode, src_stride, dst_stride);
// ...
// Call simd vf function through asc_vf_call
asc_vf_call<add_custom>(z_local, x_local, y_local);

// Store computation result to result GM
asc_copy_ub2gm_align(z, (__ubuf__ float*)z_local, BLK_NUM, burst_length, cache_mode, src_stride, dst_stride);
}
```

### Cube计算单元

Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。

C语言编程针对矩阵计算编程模型提供了四类接口，分别承载CUBE核中各个通路的搬运和计算能力，如下图所示：

![普通矩阵基础计算流程图](普通矩阵基础计算流程图.png)

首先通过数组的方式申请L1 Buffer、L0A Buffer、L0B Buffer等资源。
```c
__cbuf__ half a_l1[BLOCK_M * BLOCK_K];   // L1 staging area
__cbuf__ half b_l1[BLOCK_K * BLOCK_N];   // L1 staging area
__ca__ half a_l0a[BLOCK_M * BLOCK_K];    // L0A input
__cb__ half b_l0b[BLOCK_K * BLOCK_N];    // L0B input
__cc__ float c_l0c[BLOCK_M * BLOCK_N];   // L0C output

```

1. 通过`asc_copy_gm2l1_nd2nz`接口将A、B原始矩阵的GM数据搬运到L1 Buffer中（如果存在Bias/随路量化，则同样通过DataCopy搬运到L1 Buffer中, L1 Buffer必须为NZ分形）。
```c
asc_copy_gm2l1(a_l1, a_gm, BLOCK_M * BLOCK_K);  // GM to L1
asc_copy_gm2l1(b_l1, b_gm, BLOCK_K * BLOCK_N);  // GM to L1
```

2. 通过`asc_copy_l12l0a`接口将A、B矩阵分别搬运到L0A Buffer和L0B Buffer上准备计算(如果存在Bias/随路量化，则通过`asc_copy`接口将L1 Buffer中的bias数据/量化系数数据搬运到BT Buffer/Fixpipe Buffer上)。
```c
asc_copy_l12l0a(a_l0a, a_l1, 0, 1, 1, 0);  // L1 to L0A
asc_copy_l12l0b(b_l0b, b_l1, 0, 1, 1, 0);  // L1 to L0B
```

3. 通过`asc_mmad`接口对L0A Buffer、L0B Buffer(BT Buffer)上面的数据进行矩阵计算，并输出结果到L0C Buffer上。
```c
asc_mmad(c_l0c, a_l0a, b_l0b, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
```

4. 通过`asc_copy_l0c2gm`接口将L0C Buffer的数据进行处理并搬出到GM中，该接口可以利用Fixpipe Buffer数据进行随路量化、Relu等随路操作。
```c
asc_copy_l0c2gm(c_gm, c_l0c, BLOCK_M * BLOCK_N);
```

## 五、总结

本章系统阐述了C语言编程的核心定位与技术体系。作为SIMD编程的底层接口，C语言编程直接映射NPU硬件指令，提供指令级透明可控能力，帮助开发者实现硬件峰值性能与精准问题定位。