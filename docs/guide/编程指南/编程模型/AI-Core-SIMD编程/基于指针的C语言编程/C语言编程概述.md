# C语言编程概述

Ascend C为AI Core提供了SIMD编程路径，**C语言编程**是该路径的底层核心接口，直接映射NPU硬件指令，提供对向量处理单元、矩阵运算单元、本地存储等硬件资源的精细控制能力。

对于追求极致性能、习惯C语言编程的开发者，C语言编程是解锁NPU硬件潜能的关键技术路径。本章旨在帮助开发者建立对C语言编程的系统化认知，理解其底层技术定位、核心编程范式与开发方法论。

---

## 一、背景动因

C语言编程的动因在于让开发者掌握NPU硬件执行细节。满足开发者手动规划本地内存布局，控制数据在内存间流转，插入硬件流水同步，调整搬运和计算流水并行等直接面向计算单元、存储层次和流水同步组织代码的要求。适用于对性能和可控性要求较高的算子开发场景。

---

## 二、编程范式

C语言编程提供完备的编程能力，是SIMD编程路径中最贴近硬件的底层接口。保留了裸指针、数组、地址空间限定符和显式函数调用等底层表达能力。开发者通过声明静态数组管理本地内存，感知并手动控制同步细节，直接调用C API的Vector矢量计算或Cube矩阵计算接口，实现指令级精细控制。

### 2.1 Kernel函数

[核函数](../核函数.md)在前序章节已作介绍，C语言编程在核函数定义与调用上沿用SIMD编程通用规范，无额外语法约束。


### 2.2 内存层级

AI Core采用分级存储架构，不同计算单元的编程视角有所差异：

**Cube矩阵计算**：采用「Global Memory → L1/L0系列Buffer」两级层级，其中L1 Buffer、L0A/L0B Buffer、L0C Buffer均服务于矩阵计算。

**Vector矢量计算**：传统架构采用「Global Memory → UB」两级层级；Ascend 950PR/Ascend 950DT引入寄存器后构建「Global Memory → Unified Buffer → Register」三级层级。**Global Memory**存放输入输出数据，**Unified Buffer**作为矢量计算数据中间缓存，**Register**位于Vector计算单元最内层，直接与矢量计算执行单元交互。

<img src="../../../../figures/AI%20Core存储层次结构.png" alt="AI Core存储层次结构" width="800" />


### 2.3 外部存储（Global Memory）

Global Memory是昇腾NPU的设备内存，位于AI Core外部，容量大、带宽高但访问延迟相对较长。用于存储算子的输入数据、输出结果和中间计算数据。运行在Device侧的核函数可直接访问Global Memory。

Global Memory具有持久性：通过全局内存分配的空间，其中存储的数据将持续保留，直到该内存空间被释放或应用程序终止。用户通过Runtime API完成Device侧全局内存的管理。

Host使用aclrtMalloc分配Device侧的全局内存，并通过aclrtMemcpy将数据从Host拷贝到Device侧的全局内存，或将数据从Device的全局内存拷贝回Host内存；通过aclrtMalloc分配的Device全局内存需使用aclrtFree接口释放。有关Runtime API的更多信息与细节，可以参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。

```c
__global__ __aicore__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
{
    // Kernel implementation
}
```

---

### 2.4 内部存储（Local Memory）

AI Core包含三类基础计算单元：Cube负责矩阵计算、Vector负责矢量计算、Scalar负责标量计算。计算前需将数据从外部存储加载到内部存储，不同存储单元服务于不同计算单元：L1 Unified Buffer支撑矢量计算, Buffer和L0 Buffer支撑矩阵计算。

#### 2.4.1 Unified Buffer

Unified Buffer是AI Core内主要用于矢量计算单元的内部存储单元，位于每个Vector Core（AIV）内部。与外部存储空间相比，Unified Buffer的容量较小，但具有更高的带宽和更低的访问延迟，可视为内核执行期间由用户管理的高速缓存资源。

在C指针编程中，用户可通过数组定义的方式动态或静态申请Unified Buffer。

**静态申请**：分配一段指定大小的内存空间，空间大小在编译时确定，不可动态修改。

```c
constexpr uint32_t ub_size = 256;
__ubuf__ half static_buffer[ub_size];  // Compiler automatically aligns to 32 bytes
```

**动态申请**：用户需要通过<<<>>>中参数dynamic_ub_size指定动态内存的空间大小，其大小在运行期确定。

```c
extern __ubuf__ char dynamic_buffer[];
```

> 📌 Unified Buffer地址必须32字节对齐，以匹配硬件总线粒度和SIMD计算单元并行度，非对齐访问将导致运行时错误。

#### 2.4.2 Bank冲突优化

为了提高数据访问的效率和吞吐量，Unified Buffer采用了bank（大小相等的内存模块）结构设计。以Ascend 950PR/Ascend 950DT产品为例，Unified Buffer总大小为256K，包含8个bank group，每个bank group包含2个bank。每个bank大小为16KB，由512行组成，每行长度为32B，采用低位地址交织。

<img src="../../../../figures/UB_Bank冲突示意图.png" alt="UB Bank冲突示意图" width="800" />

该架构地址使用低位编址, 地址排布如下图所示, 每个bank可以独立地进行数据的读写操作，允许多个数据请求同时进行。然而，当多个读写操作试图同时访问同一个bank，由于硬件资源的限制，这些操作必须排队等待，会导致bank冲突，引起性能下降。

![低位交织地址排布](../../../../figures/低位交织地址排布.png)

每个bank一拍只能完成一读或者一写，每个bank group最多只允许2读0写或者1读1写。根据内存结构，bank冲突主要可以分为以下三种类型：
- 读写冲突：读操作和写操作同时尝试访问同一个bank，或者两个读操作和一个写操作同时尝试访问同一个bank group。
- 写写冲突：多个写操作同时尝试访问同一个bank group。
- 读读冲突：两个读操作访问同一个bank，或两个以上操作同时尝试访问同一个bank group。

以读写冲突为例，当进行一组`dst = src0 + src1`的矢量加法计算时, 如果`src0`的起始地址在0x0000, `dst`的起始地址为0x4000, 结合Bank的排布, 可以看到当前`src0`和`dst`的地址分别属于同一个bank和同一个bank group, 则存在读写冲突。

避免bank冲突的方法如下：
- 优化计算逻辑

对于特定的算法计算，可通过调整内存读写顺序或方式来避免Bank冲突。以shape为(8, 16, 16)的输入执行(1, 0, 2)的transpose操作为例，通过将计算逻辑由"跳读、连续写"调整为"连续读、跳写"，可避免冲突。

<img src="../../../../figures/读读冲突.png" alt="读读冲突示意图" width="800" />

如上图所示，如果我们对src进行"跳读、连续写"的方式操作，这样src的8个data block都会分配在同一个bank group内，会产生读读冲突。
```c
for (uint16_t i = 0; i < 16; ++i) {
    // Calculate by column, stride read, continuous write
    asc_add_scalar(dst + i * 128, src + i * 16, 1, repeat, 1, 16, 8, 8);
}
```

<img src="../../../../figures/读读冲突优化.png" alt="读读冲突优化示意图" width="800" />

为了避免类似的读读冲突，通常是避免读操作时访问到同一个bank group。该场景是一个读单元，因为操作时跳着读，造成8个data block都分配到了同一个bank group内，导致读读冲突。所以这种情况，通过改写算法逻辑，将跳读调整为连续读，这样8个data block会分配到不同的8个bank group内，从而避免读读冲突。（该样例说明了如何通过调整计算逻辑优化读读冲突问题，同理写操作也会遇到类似问题）
```c
for (uint16_t i = 0; i < 8; ++i) {
    // Calculate by column, continuous read, stride write
    asc_add_scalar(dst + i * 16, src + i * 256, 1, repeat, 8, 1, 8, 8);
}
```

- 优化地址分配

实现连续4096个float元素的加法z = x + y，通过在内存分配时适当扩大内存，保证在一个Repeat内，x/y和z不会同时出现同一个bank内，实现方案对比如下：

不做地址优化，直接使用数组分配内存，各个计算单元的地址分别为：
- x：起始地址0x00000，x的长度为4096 * sizeof(float)字节
- y：起始地址0x04000，y的长度为4096 * sizeof(float)字节
- z：起始地址0x08000，z的长度为4096 * sizeof(float)字节
在一个Repeat内，x和y同时读同一个bank group，x/y和z同时读写同一个bank。
```c
__ubuf__ float x[4096];
__ubuf__ float y[4096];
__ubuf__ float z[4096];
```
<img src="../../../../figures/地址优化前内存排布.png" alt="地址优化前内存排布" width="500" />

通过优化地址，使用数组分配内存时适当扩大内存申请，各个操作数据的地址分别为：
- x：起始地址0x00000，x长度为4096 * sizeof(float) 字节
- y：起始地址0x04000，y长度为(8 * 16 * 1024 - (4096 * sizeof(float)))字节
- z：起始地址0x20000，z长度为4096 * sizeof(float) 字节
y多申请空间，确保z不会和x/y落入同一个bank。
```c
__ubuf__ float x[4096];
__ubuf__ float y[(8 * 16 * 1024 - (4096 * sizeof(float))) / sizeof(float)];
__ubuf__ float z[4096];
```

<img src="../../../../figures/地址优化后内存排布.png" alt="地址优化后内存排布" width="500" />

#### 2.4.2 L1 Buffer & L0 Buffer

Cube计算单元是AI Core中专用于执行矩阵运算的单元，高亮部分展示了Cube计算单元及其直接访问的专用缓存：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer用于存储初始化累加值和矩阵计算结果。

<img src="../../../../figures/矩阵计算单元架构图.png" alt="矩阵计算单元架构图" width="800" />

**存储单元及地址限定符说明**：

| 存储单元 | 作用 | 地址限定符 | 示例 |
|---------|------|-----------|------|
| L1 Buffer | 缓存矩阵计算输入数据 | `__cbuf__` | `__cbuf__ half l1_buffer[512];` |
| L0A Buffer | 存储左矩阵 | `__ca__` | `__ca__ half l0a_buffer[256];` |
| L0B Buffer | 存储右矩阵 | `__cb__` | `__cb__ half l0b_buffer[256];` |
| L0C Buffer | 存储计算结果 | `__cc__` | `__cc__ half l0c_buffer[256];` |
| Fixpipe Buffer | 存储量化参数 | `__fbuf__` | `__fbuf__ half fixpipe_buffer[128];` |
| BiasTable Buffer | 存储Bias数据 | `__biasbuf__` | `__biasbuf__ half bias_buffer[64];` |

在C指针编程中，用户可通过数组定义方式申请`L1 Buffer`和`L0 A Buffer`等, 不同的存储空间使用不同的地址限定符标识指定, 如下面例子所示:

```c
constexpr uint32_t m_size = 256;
constexpr uint32_t n_size = 128;
constexpr uint32_t k_size = 256;

__cbuf__ half l1a_buffer[m_size * k_size];  // L1 Buffer, 32 bytes aligned
__cbuf__ half l1b_buffer[n_size * k_size];
__ca__ half l0a_buffer[m_size * k_size];    // L0A Buffer, 512 bytes aligned
__cb__ half l0b_buffer[n_size * k_size];    // L0B Buffer, 512 bytes aligned
__cc__ half l0c_buffer[m_size * n_size];    // L0C Buffer, 64 bytes aligned
```

同时为了对矩阵乘法等计算密集型任务进行优化，提升Cube计算效率，AI Core对于L1 Buffer、L0 Buffer都采用了分形格式：
- L1 Buffer  : FRACTAL_Nz
- L0A Buffer : FRACTAL_Nz (Ascend 950PR/Ascend 950DT架构该内存分形为Nz, Atlas A2/A3训练和推理系列产品为Zz分形)
- L0B Buffer : FRACTAL_Zn
- L0C Buffer : FRACTAL_Nz

以`FRACTAL_Nz`为例，内存排布如下图所示，整块内存通过大N外部column major（列主序）+ 小z内部row major（行主序）存放。

![FRACTAL_Nz内存排布示意图](../../../../figures/FRACTAL_Nz内存排布示意图.png)



### 2.5 寄存器（Register）

Ascend 950PR/Ascend 950DT新一代架构引入寄存器可编程能力，Vector计算单元的所有计算均需经「GM → UB → Register」三级数据流转，在寄存器上完成矢量计算。C语言编程通过直接定义寄存器变量方式进行寄存器编程。可参考[Reg矢量计算编程](./Reg矢量计算编程.md)中，有关寄存器类型等描述。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。

---

## 三、同步机制

在编程中，同步是指协调多个执行单元（如线程、进程等）对共享资源的访问顺序和时机来确保程序的正确性。如果没有同步来严格控制执行顺序，则会出现结果不一致、行为不可预测等多种问题。

AI Core的同步主要分为**核内同步**和**核间同步**。

---

### 3.1 核内同步

#### 为什么需要核内同步

AI Core内部的执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行的方式运行，在读写同一存储资源时可能存在数据依赖关系。为确保数据一致性及计算正确性，需通过同步控制协调操作时序。

**Atlas A2/A3训练和推理系列产品硬件架构图**：

<img src="../../../../figures/2201_硬件架构图.png" alt="2201硬件架构图" width="800" />

**Ascend 950PR/Ascend 950DT产品硬件架构图**：

<img src="../../../../figures/3510_硬件架构图.png" alt="3510硬件架构图" width="800" />

---

例如：在Ascend 950PR/Ascend 950DT硬件上**Vector矢量计算流程**，需要执行以下三个主要步骤：

1. 通过DMA将数据从Global Memory搬入到Local Memory
2. 进行矢量计算
3. 通过DMA将数据从Local Memory搬出到Global Memory

<img src="../../../../figures/Vector计算数据流.png" alt="Vector计算数据流" width="800" />

从上图可以看到，执行一个Vector矢量计算时，主要由三个执行单元`Vector`、`DMA(MTE2)`、`DMA(MTE3)`并行执行，如果访问同一片Local Memory，需要同步机制来控制访问时序。

<img src="../../../../figures/同步顺序.png" alt="同步顺序示意图" width="800" />

> 📌 提示：上例描述为主要路径，当`Scalar`执行单元读写GM或者UB时，也需要考虑`Scalar`流水与其他流水的同步。

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

**核内同步机制对比**：

| 同步类型 | 核心作用 | 适用场景 | 关键接口 |
|---------|---------|---------|---------|
| **流水互斥同步** | 锁定指定流水，防止并发冲突 | 多流水并发访问同一UB区域时 | `asc_lock` / `asc_unlock` |
| **多流水同步** | 建立两条流水间的依赖关系 | 源流水完成后目的流水才能执行 | `asc_sync_notify` / `asc_sync_wait` |
| **单流水同步** | 等待同一流水内前序指令完成 | 同一流水内存在前后数据依赖 | `asc_sync_pipe` / `asc_sync_*` |
| **内存屏障** | 等待所有前序内存访问完成 | 存在数据依赖或内存依赖的场景 | `asc_sync_data_barrier` |

> 📌 流水互斥同步自Ascend 950PR/Ascend 950DT引入


#### 流水互斥同步

该同步能力从Ascend 950PR/Ascend 950DT开始引入并使用，AI Core提供了类似传统CPU中锁机制的同步能力。通过锁定指定流水再释放流水来完成流水间的同步依赖。

- `asc_lock`：根据`mutex_id`获取Mutex，并阻塞当前流水指令队列，直到对应的`mutex_id`被Unlock
- `asc_unlock`：等待当前流水的前置指令退出后，根据`mutex_id`释放对应的Mutex
```c
// Lock MTE2 pipeline mutex
uint8_t mutex_id = 1;
asc_lock(PIPE_MTE2, mutex_id);
asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));
asc_unlock(PIPE_MTE2, mutex_id);

// Lock Vector pipeline mutex
asc_lock(PIPE_V, mutex_id);
asc_add(shared_buffer, x_local, y_local);
asc_unlock(PIPE_V, mutex_id);
```


#### 核内多流水同步

该同步主要用于核内多流水间的同步，通过`asc_sync_notify/asc_sync_wait`对两条流水进行`notify`和`wait`处理。

- `asc_sync_notify`：当源流水的前序指令的所有读写操作都完成之后，当前指令开始执行，并将硬件中的对应标志位设置为1
- `asc_sync_wait`：当目的流水执行到该指令时，如果发现硬件中对应标志位为0，目的流水的后续指令将一直被阻塞；如果发现硬件中对应标志位为1，则将硬件中对应标志位设置为0，同时目的流水的后续指令开始执行

```c
// Transfer data from GM to UB via DMA(MTE2)
asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));

// Synchronize between PIPE_MTE2 and PIPE_V via inter-pipeline sync
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

// Perform vector Add computation, execution pipeline is PIPE_V
asc_add(dst, src1, src0, total_length);
```


#### 同一流水内同步

同一流水线中虽然指令是顺序执行，但并不意味着后一条指令开始执行时前一条指令执行结束。所以对于同一条流水线执行，如果存在前后数据依赖，也需要进行同步处理。

```c
asc_add(dst, src1, src0, total_length);
// Subsequent asc_sub instruction depends on dst, insert asc_sync_pipe(PIPE_V) for single pipeline synchronization
asc_sync_pipe(PIPE_V);
asc_sub(dst, dst, src0, total_length);
```

可以直接通过`asc_sync_vector`或者`asc_sync_mte2`这些接口来直接等待某一条流水。

```c
// Transfer data from GM to UB via DMA(MTE2)
asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));
// Wait for MTE2 pipeline to complete
asc_sync_mte2();
// Perform vector Add computation, execution pipeline is PIPE_V
asc_add(dst, src1, src0, total_length);
```

---

#### 内存访问顺序控制

当后续执行指令对前序的内存访问存在数据依赖或者内存依赖时，可以通过`asc_sync_data_barrier`来进行内存访问顺序同步，该接口会阻塞后续的指令执行，直到所有之前的内存访问指令都执行结束。

```c
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);  // Wait for all memory access to complete
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
    // Perform ReduceSum and transfer to aggregated GM
    // Perform full V-core synchronization, wait for all cores to write ReduceSum results to GM
    int64_t flagID = 1;
    asc_sync_inter_arrive(PIPE_S, flagID);
    asc_sync_inter_wait(PIPE_S, flagID);
}
```

---

#### 单个AI Core内AIC与AIV之间的同步

该能力为单个AI Core内，一个group内的block跟subblock之间的同步能力。该能力为Ascend 950PR/Ascend 950DT架构新增的能力，支持AIV0与AIV1可单独触发AIC等待。

```c
// When PIPE_FIX pipeline execution completes, AIC sends arrive capability to AIV0 and AIV1 respectively
if ASCEND_IS_AIC {
    asc_sync_intra_arrive(PIPE_FIX, 0);
    asc_sync_intra_arrive(PIPE_FIX, 16);
}

// AIV side code performs synchronization wait, waits for AIC sync signal before executing PIPE_MTE3 pipeline
if ASCEND_IS_AIV {
    asc_sync_intra_wait(PIPE_MTE3, 0);
}
```

---

## 四、计算单元

AI Core分为AIC和AIV两个独立的核，分别用于矩阵计算和向量计算。以下图为例(Ascend 950PR/Ascend 950DT), AIC核与AIV核的配比为1:2。

![硬件架构图3510](../../../../figures/硬件架构图3510.png)

Cube计算单元和Vector计算单元分别部署在AIC核和AIV核上，每个核都有自己的Scalar单元，能独立加载自己的代码段。

### Vector计算单元

Vector计算单元主要用作向量计算，譬如Add、Sub等基础算术计算等。

在Atlas A2/A3训练和推理系列产品上，Vector计算单元的数据来自于Unified Buffer（要求32B对齐），C语言编程在Vector计算单元上提供了通过UB指针直接操作计算的接口能力。此类直接操作UB的编程方式称为Memory矢量计算编程。

以Add计算为例，当开发者需要计算4096长度的向量加法, 可以通过`asc_add(dst, src0, src1, count)`的接口直接进行计算。更多内容可参考[Memory矢量计算编程](./Memory矢量计算编程.md)中有关Memory矢量计算编程的描述。
```c
__ubuf__ float x[4096 + 64];  // Allocate extra 256 bytes
__ubuf__ float y[128 * 128 - (4096 + 64)];  // Ensure z is not in the same bank
__ubuf__ float z[4096];

// 1. Data transfer in: transfer GM data to UB, this implementation is omitted
// 2. Perform calculation for the first 4096 elements via asc_add
asc_add(z, x, y, 4096);
```

Ascend 950PR/Ascend 950DT新一代架构, 引入寄存器可编程能力，在传统UB缓存体系的基础上，开放寄存器（Register）可编程能力，构建出「Global Memory → UB → Register」的三级内存层级。新一代架构下的矢量计算都需要从Unified Buffer加载到Register寄存器后，再利用Register寄存器进行计算操作。可参考[Reg矢量计算编程](./Reg矢量计算编程.md)中有关Reg矢量计算编程的描述。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。


### Cube计算单元

Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L1 Buffer用于矩阵计算缓存，L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。此类直接操作L1/L0A/L0B/L0C Buffer的编程方式称为[Cube矩阵计算编程](./Cube矩阵计算编程.md)。

C语言编程针对矩阵计算编程模型提供了四类接口，分别承载CUBE核中各个通路的搬运和计算能力，如下图所示：

![普通矩阵基础计算流程图](../../../../figures/普通矩阵基础计算流程图.png)

首先通过数组的方式申请L1 Buffer、L0A Buffer、L0B Buffer等资源。
```c
__cbuf__ half a_l1[BLOCK_M * BLOCK_K];   // L1 staging area
__cbuf__ half b_l1[BLOCK_K * BLOCK_N];   // L1 staging area
__ca__ half a_l0a[BLOCK_M * BLOCK_K];    // L0A input
__cb__ half b_l0b[BLOCK_K * BLOCK_N];    // L0B input
__cc__ float c_l0c[BLOCK_M * BLOCK_N];   // L0C output

```

1. 通过`asc_copy_gm2l1_nd2nz`接口将A、B原始矩阵的GM数据搬运到L1 Buffer中（如果存在Bias/随路量化，则同样通过DataCopy搬运到L1 Buffer中, L1 Buffer必须为Nz分形）。
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

本章系统阐述了C语言编程的核心定位与技术体系。作为SIMD编程的底层接口，C语言编程直接映射NPU硬件指令，提供指令级透明可控能力，帮助开发者实现硬件峰值性能与精准问题定位。进一步学习可以参考[Cube矩阵计算编程](./Cube矩阵计算编程.md)、[Memory矢量计算编程](./Memory矢量计算编程.md)和[Reg矢量计算编程](./Reg矢量计算编程.md)。
