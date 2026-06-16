# C语言编程概述

C语言编程是Ascend C SIMD编程路径的底层核心接口，直接映射NPU硬件指令，提供对向量处理单元、矩阵运算单元、本地存储等硬件资源的精细控制能力。开发者可手动规划本地内存布局、控制数据流转、插入硬件流水同步、调整搬运与计算的流水并行，直接面向计算单元、存储层次和流水同步组织代码。该路径适用于对性能和可控性要求较高的算子开发场景，是追求极致性能、习惯C语言编程的开发者解锁NPU硬件潜能的关键技术路径。本章旨在帮助开发者建立对C语言编程的系统化认知，理解其底层技术定位、核心编程范式与开发方法论。

## Kernel函数

[核函数](../核函数.md)在前序章节已作介绍，C语言编程在核函数定义与调用上沿用SIMD编程通用规范，无额外语法约束。

## 内存层级

AI Core采用分级存储架构，不同计算单元的编程视角有所差异：

**Cube矩阵计算**：采用「Global Memory → L1/L0系列Buffer」两级层级，其中L1 Buffer、L0A/L0B Buffer、L0C Buffer均服务于矩阵计算。

**Vector矢量计算**：传统架构采用「Global Memory → UB」两级层级；Ascend 950PR/Ascend 950DT引入寄存器后构建「Global Memory → Unified Buffer → Register」三级层级。Global Memory存放输入输出数据，Unified Buffer作为矢量计算数据中间缓存，Register位于Vector计算单元最内层，直接与矢量计算执行单元交互。

<img src="../../../../figures/AI_Core存储层次结构_c.png" alt="AI Core存储层次结构" width="800" />


### 外部存储（Global Memory）

Global Memory是昇腾NPU的设备内存，位于AI Core外部，容量大、带宽高但访问延迟相对较长。用于存储算子的输入数据、输出结果和中间计算数据。运行在Device侧的核函数可直接访问Global Memory。Global Memory具有持久性：通过全局内存分配的空间，其中存储的数据将持续保留，直到该内存空间被释放或应用程序终止。用户通过Runtime API完成Device侧全局内存的管理。

结合下述示例, Host使用`aclrtMalloc`分配Device侧的全局内存，并通过`aclrtMemcpy`将数据从Host拷贝到Device侧的全局内存，或将数据从Device的全局内存拷贝回Host内存；然后通过<<<>>>拉齐Device侧核函数进行运算；通过`aclrtMalloc`分配的Device全局内存需使用`aclrtFree`接口释放。有关Runtime API的更多信息与细节，可以参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。
此外，核函数需使用__global__函数前缀进行声明。 该前缀用于标识函数在Host侧调用、Device侧执行，是Ascend C核函数的标准定义方式，确保编译器正确识别核函数入口并生成相应的硬件执行代码。
```c
// Kernel implementation, decorated with __global__ to mark kernel entry point
__global__ __aicore__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
{
    // Kernel internal logic implementation, customized based on operator scenario
}

// Host function: allocate Device memory, copy data via aclrtMemcpy, launch kernel via <<<>>>
std::vector<float> kernel_add(std::vector<float> &x, std::vector<float> &y) {
    // Get host-side buffer pointers
    uint8_t *xHost = reinterpret_cast<uint8_t *>(x.data());
    uint8_t *yHost = reinterpret_cast<uint8_t *>(y.data());
    uint8_t *zHost = nullptr;
    // Allocate Device memory
    aclrtMalloc((void **)&xDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&yDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&zDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    // Copy data from Host memory to Device memory via aclrtMemcpy
    aclrtMemcpy((uint8_t*)xDevice, totalByteSize, xHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy((uint8_t*)yDevice, totalByteSize, yHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    // Launch kernel via <<<>>>, passing Device memory pointers xDevice, yDevice, zDevice
    add_custom<<<numBlocks, nullptr, stream>>>(xDevice, yDevice, zDevice);
    aclrtSynchronizeStream(stream);
    // Release Device memory
    aclrtFree(xDevice);
    aclrtFree(yDevice);
    aclrtFree(zDevice);
}
```

### 内部存储（Local Memory）

AI Core包含三类基础计算单元：Cube负责矩阵计算、Vector负责矢量计算、Scalar负责标量计算。计算前需将数据从外部存储加载到内部存储，不同存储单元服务于不同计算单元：Unified Buffer支撑矢量计算, L1 Buffer和L0 Buffer支撑矩阵计算。

#### AIV内部存储之Unified Buffer

Unified Buffer是AI Core内主要用于矢量计算单元的内部存储单元，位于每个Vector Core（AIV）内部。与外部存储空间相比，Unified Buffer的容量较小，但具有更高的带宽和更低的访问延迟，可视为内核执行期间由用户管理的高速缓存资源。

在C指针编程中，用户可通过数组定义的方式动态或静态申请Unified Buffer。

**静态申请**：分配一段指定大小的内存空间，空间大小在编译时确定，不可动态修改。

```c
__global__ __vector__ void add_custom(...)
{
    constexpr uint32_t ub_size = 256;
    __ubuf__ half static_buffer[ub_size];  // Compiler automatically aligns to 32 bytes
}
```

**动态申请**：用户需要通过<<<>>>中参数dynamic_ub_size指定动态内存的空间大小，其大小在运行期确定。

```c
// Device-side: declare dynamic shared memory
__global__ __vector__ void add_custom(...)
{
    extern __ubuf__ char dynamic_buf[];
    // ...
}

// Host-side: specify dynamic shared memory size via third parameter in <<<>>>
int32_t main(int argc, char const *argv[])
{
    // ...
    uint32_t dyn_ubuf_size = 1024 * sizeof(char);
    add_custom<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(...);
    // ...
}
```

另外，开发者也可以通过定义结构体的方式来定义内存结构体, 这样可以简化算子中内存定义的代码部分。
```c
// Define src0 and src1 memory blocks unified via struct
__ubuf__ struct src_ubuf {
    half src0[128];
    half src1[128];
};

__global__ __vector__ void add_custom(...) {
    src_ubuf struct local_buf{};
    __ubuf__ src_ubuf* src_buffer = (__ubuf__ src_ubuf*)&local_buf;
    ...
    asc_add(dst, src_buffer.src0, src_buffer.src1, 128);
}
```

> 📌 Unified Buffer地址必须32字节对齐，以匹配硬件总线粒度和SIMD计算单元并行度，非对齐访问将导致运行时错误。

#### AIV内部存储之Register File

Ascend 950PR/Ascend 950DT新一代架构引入寄存器可编程能力，Vector计算单元的所有计算均需经「GM → UB → Register」三级数据流转，在寄存器上完成矢量计算。C语言编程通过直接定义寄存器变量方式进行寄存器编程。可参考[Reg矢量计算编程](./Reg矢量计算编程.md)中，有关寄存器类型等描述。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。


#### AIC内部存储L1 Buffer & L0 Buffer

Cube计算单元是AI Core中专用于执行矩阵运算的单元，Cube计算单元直接访问的专用缓存为：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer用于存储初始化累加值和矩阵计算结果。

在C语言编程中，用户可通过数组定义方式申请`L1 Buffer`和`L0 A Buffer`等, 不同的存储空间使用不同的地址限定符标识指定, AIC内部主要地址限定符如下:

**存储单元及地址限定符说明**：

| 存储单元 | 作用 | 地址限定符 | 示例 |
|---------|------|-----------|------|
| L1 Buffer | 缓存矩阵计算输入数据 | `__cbuf__` | `__cbuf__ half l1_buffer[512];` |
| L0A Buffer | 存储左矩阵 | `__ca__` | `__ca__ half l0a_buffer[256];` |
| L0B Buffer | 存储右矩阵 | `__cb__` | `__cb__ half l0b_buffer[256];` |
| L0C Buffer | 存储计算结果 | `__cc__` | `__cc__ half l0c_buffer[256];` |
| Fixpipe Buffer | 存储量化参数 | `__fbuf__` | `__fbuf__ half fixpipe_buffer[128];` |
| BiasTable Buffer | 存储Bias数据 | `__biasbuf__` | `__biasbuf__ half bias_buffer[64];` |

结合地址限定符，开发者可以直接通过数组定义的方式申请一块内存块，如下方代码所示：

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

同时为了对矩阵乘法等计算密集型任务进行优化，提升Cube计算效率，AI Core对于L1 Buffer、L0 Buffer都采用了分形格式。不同产品型号的分形格式存在差异，具体对比如下：

**表 1** 不同产品型号的分形格式对比

| 存储单元 | Atlas A2/A3训练和推理系列产品 | Ascend 950PR/Ascend 950DT |
|---------|----------------------------|---------------------------|
| L1 Buffer | Nz | Nz |
| L0A Buffer | Zz | Nz |
| L0B Buffer | Zn | Zn |
| L0C Buffer | Nz | Nz |

以`Nz`为例，内存排布如下图所示，整块内存通过大N外部column major（列主序）+ 小z内部row major（行主序）存放。

![Nz内存排布示意图](../../../../figures/FRACTAL_NZ内存排布示意图.png)

#### Bank冲突

为了提高数据访问的效率和吞吐量，内部存储采用了bank（大小相等的内存模块）结构设计。以Unified Buffer为例, Ascend 950PR/Ascend 950DT产品上，Unified Buffer总大小为256KB（256 × 1024字节），包含8个bank group，每个bank group包含2个bank。每个bank大小为16KB，由512行组成，每行长度为32B，采用低位地址交织。Atlas A2/A3训练和推理系列产品的UB容量规格请参考相应硬件架构文档。

<img src="../../../../figures/UB_Bank冲突示意图.png" alt="UB Bank冲突示意图" width="800" />

该架构地址使用低位编址, 地址排布如下图所示, 每个bank可以独立地进行数据的读写操作，允许多个数据请求同时进行。然而，当多个读写操作试图同时访问同一个bank，由于硬件资源的限制，这些操作必须排队等待，会导致bank冲突，引起性能下降。

![低位交织地址排布](../../../../figures/低位交织地址排布.png)

每个bank一拍只能完成一读或者一写，每个bank group最多只允许2读0写或者1读1写。根据内存结构，bank冲突主要可以分为以下三种类型：
- 读写冲突：读操作和写操作同时尝试访问同一个bank，或者两个读操作和一个写操作同时尝试访问同一个bank group。
- 写写冲突：多个写操作同时尝试访问同一个bank group。
- 读读冲突：两个读操作访问同一个bank，或两个以上操作同时尝试访问同一个bank group。

以读写冲突为例，当进行一组`dst = src0 + src1`的矢量加法计算时, 如果`src0`的起始地址在0x0000, `dst`的起始地址为0x4000, 结合Bank的排布, 可以看到当前`src0`和`dst`的地址分别属于同一个bank和同一个bank group, 则存在读写冲突。

当遇到Bank冲突的时候, 会引起算子性能的下降, 通常可以通过优化计算逻辑和优化分配地址方式来避免冲突的产生, 详情请参考[避免Bank冲突](../../../../算子实践参考/SIMD算子性能优化/内存访问/避免UB的bank冲突/避免bank冲突（NPU架构版本3510）.md)。

## 同步机制

在编程中，同步是指协调多个执行单元（如线程、进程等）对共享资源的访问顺序和时机来确保程序的正确性。如果没有同步来严格控制执行顺序，则会出现结果不一致、行为不可预测等多种问题。

AI Core的同步主要分为**核内同步**和**核间同步**。


### 核内同步

AI Core内部的执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行的方式运行，在读写同一存储资源时可能存在数据依赖关系。为确保数据一致性及计算正确性，需通过同步控制协调操作时序。

下图示例描述了一个常见的Vector计算数据流：

1. 通过DMA将数据从Global Memory搬入到Local Memory
2. 进行矢量计算
3. 通过DMA将数据从Local Memory搬出到Global Memory

<img src="../../../../figures/Vector计算数据流.png" alt="Vector计算数据流" width="800" />

四个执行单元Scalar、Vector、DMA（MTE2）、DMA(MTE3)并行执行，若访问同一片Local Memory，需要同步机制来控制它们的访问时序：保证先搬入Local Memory后再计算，计算完成后再搬出。

<img src="../../../../figures/同步顺序.png" alt="同步顺序示意图" width="800" />

> 📌 提示：上例描述为主要路径，当`Scalar`执行单元读写GM或者UB时，也需要考虑`Scalar`流水与其他流水的同步。

AI Core上的执行单元分别属于不同的执行流水, 同步即是保证不同执行流水间能够正确执行，如下为AI Core内的主要执行流水。
| 流水类型 | 含义 |
|---------|------|
| **PIPE_S** | 标量流水线，为此流水 |
| **PIPE_V** | 矢量计算流水及部分硬件架构下的L0C Buffer→Unified Buffer数据搬运流水 |
| **PIPE_M** | 矩阵计算流水 |
| **PIPE_MTE1** | L1 Buffer→L0A Buffer、L1 Buffer→L0B Buffer数据搬运流水 |
| **PIPE_MTE2** | GM→L1 Buffer、GM→UB等数据搬运流水 |
| **PIPE_MTE3** | UB→GM等数据搬运流水 |
| **PIPE_FIX** | L0C Buffer→GM、L0C Buffer→L1等数据搬运流水 |

为确保流水线间正确执行，AI Core针对不同应用场景提供了多种同步机制，开发者可根据实际需求灵活选择合适的同步方式来实现算子功能。

<table>
<thead>
<tr>
<th>同步类型</th>
<th>核心作用及场景说明</th>
<th>关键接口</th>
<th>实现代码示例</th>
</tr>
</thead>
<tbody>
<tr>
<td rowspan="2"><strong>多流水同步</strong></td>
<td><p><strong>机制一</strong>: 通过锁定指定流水再释放流水来完成流水间的同步依赖。用于核内异步流水之间的同步处理，其功能类似于传统CPU中的锁机制。该同步机制为Ascend 950PR/Ascend 950DT新引入机制，相较于notify/wait方式使用更简便。</p></td>
<td><code>asc_lock</code> & <code>asc_unlock</code></td>
<td>
        <code>uint8_t mutex_id = 1;</code><br>
        <code></code><br>
        <code>asc_lock(PIPE_MTE2, mutex_id);</code><br>
        <code>asc_copy_gm2ub((__ubuf__ void*)src0, ...);</code><br>
        <code>asc_unlock(PIPE_MTE2, mutex_id);</code><br>
        <code></code><br>
        <code>asc_lock(PIPE_V, mutex_id);</code><br>
        <code>asc_add(shared_buffer, x_local, y_local);</code><br>
        <code>asc_unlock(PIPE_V, mutex_id);</code>
      </td>
</tr>
<tr>
<td><p><strong>机制二</strong>: 通过notify与wait机制实现两条流水间的同步等待，用于核内异步流水的同步协调。Ascend 950PR/Ascend 950DT引入mutex机制后，仍保持对该接口的兼容支持，但优先推荐使用mutex机制进行多流水同步。</p></td>
<td><code>asc_sync_notify</code> & <code>asc_sync_wait</code></td>
<td>
        <code>asc_copy_gm2ub((__ubuf__ void*)src0, ...);</code><br>
        <code>asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);</code><br>
        <code>asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);</code><br>
        <code>asc_add(dst, src1, src0, total_length);</code>
      </td>
</tr>
<tr>
<td><strong>单流水同步</strong></td>
<td><p>等待同一流水内前序指令完成，用于同一流水内存在前后数据依赖。</p></td>
<td><code>asc_sync_pipe</code> & <code>asc_sync_mte2等</code></td>
<td>
        <code>asc_add(dst, src1, src0, total_length);</code><br>
        <code>asc_sync_pipe(PIPE_V);</code><br>
        <code>asc_sub(dst, dst, src0, total_length);</code>
      </td>
</tr>
<tr>
<td><strong>内存访问顺序控制</strong></td>
<td><p>等待所有前序内存访问完成，用于存在数据依赖或内存依赖的场景。</p></td>
<td><code>asc_sync_data_barrier</code></td>
<td>
        <code>asc_sync_data_barrier(mem_dsb_t::DSB_ALL);</code>
      </td>
</tr>
</tbody>
</table>

### 核间同步

当一个核计算所依赖的数据，需要等待其他相关核的计算全部完成后才能执行时，则需要通过核间同步来确保多核间的时序正确性。

以下图为例，AIC需要依赖于AIV的ReduceSum计算结果。由于整体矢量较大，必须拆分为多个部分，由每个AIV分别完成部分计算。每个AIV将其部分计算结果通过原子累加写入GM。AIC需要读取的必须是所有AIV均完成累加后的最终结果。

<img src="../../../../figures/核间同步业务场景示例.png" alt="核间同步业务场景示例" width="800" />

从上图可以看到，当前AI Core是由AIC核和AIV核组成，其中AIC核主要用于Cube计算，AIV核负责Vector计算。AIC/AIV按group进行划分。一个group内细分block及subblock，block与subblock比例为1:N（N>=1），block表示多少个"主核"，subblock表示一个"主核"带多少个"从核"。

<img src="../../../../figures/block和subblock之间关系.png" alt="block和subblock之间关系" width="800" />

算子按计算特征可划分为三类：Cube算子（矩阵计算）、Vector算子（矢量计算）和Mix算子（矩阵与矢量混合计算）。算子类型决定了其核间同步方式与group配置模式的选择。针对不同算子场景，C语言编程提供了相应的编程接口，以满足多样化的算子开发需求。

<table>
  <thead>
    <tr>
      <th>核间同步类型</th>
      <th>场景说明</th>
      <th>主要接口</th>
      <th>实现代码示例</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><strong>所有AIC核同步</strong></td>
      <td><p>同步所有AIC核，通常用于对Cube算子计算结果进行多核累加操作。</p></td>
      <td><pre><code>asc_sync_inter_arrive
                     asc_sync_inter_wait</code></pre></td>
      <td>
        <code>if ASCEND_IS_AIC {</code><br>
        <code>    int64_t flagID = 1;</code><br>
        <code>    asc_sync_inter_arrive(PIPE_S, flagID);</code><br>
        <code>    asc_sync_inter_wait(PIPE_S, flagID);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>所有AIV核同步</strong></td>
      <td><p>同步所有AIV核，通常用于对Vector算子计算结果进行多核累加操作。</p></td>
      <td><code>asc_sync_inter_arrive</code><br><code>asc_sync_inter_wait</code></td>
      <td>
        <code>if ASCEND_IS_AIV {</code><br>
        <code>    int64_t flagID = 1;</code><br>
        <code>    asc_sync_inter_arrive(PIPE_S, flagID);</code><br>
        <code>    asc_sync_inter_wait(PIPE_S, flagID);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>单个AI Core内AIC与AIV同步</strong></td>
      <td><p>同步单个AI Core内AIC与AIV核，通常用于Mix算子中Cube与Vector间的交互操作。</p></td>
      <td><code>asc_sync_intra_arrive</code><br><code>asc_sync_intra_wait</code></td>
      <td>
        <code>// Scenario: Synchronization between two AIVs and AIC. Atlas A2/A3 training/inference products support two AIVs triggering AIC wait simultaneously, Ascend 950PR/Ascend 950DT and later support AIV0 and AIV1 triggering AIC wait separately</code><br>
        <code>if ASCEND_IS_AIC {</code><br>
        <code>    // Atlas A2/A3 training/inference products: AIC sends one arrive, two AIVs wait simultaneously</code><br>
        <code>    asc_sync_intra_arrive(PIPE_FIX, 0);</code><br>
        <code>    // Ascend 950PR/Ascend 950DT products: AIC sends two arrives, two AIVs wait separately</code><br>
        <code>    asc_sync_intra_arrive(PIPE_FIX, 0);</code><br>
        <code>    asc_sync_intra_arrive(PIPE_FIX, 16);</code><br>
        <code>}</code><br>
        <code>if ASCEND_IS_AIV {</code><br>
        <code>    asc_sync_intra_wait(PIPE_MTE3, 0);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>单个AI Core内两个AIV间同步</strong></td>
      <td><p>同步单个AI Core内两个AIV子核，通常用于Vector算子内AIV核间的协调操作。</p></td>
      <td> <code>该功能当前C语言编程路径暂不支持，后续版本将提供支持。</code> </td>
      <td> <code>该功能当前C语言编程路径暂不支持，后续版本将提供支持。</code></td>
    </tr>
  </tbody>
</table>

> 📌 基于分离模式（AIC和AIV分离）开发融合算子时，算子逻辑中通常同时包含AIV和AIC的处理逻辑，此时需要使用Ascend C提供的宏ASCEND_IS_AIV/ASCEND_IS_AIC来对AIV和AIC的代码进行隔离;

## 算子类型

前序章节抽象硬件架构已阐述，AI Core包含Vector和Cube两类计算单元，分别用于向量计算和矩阵计算。Cube计算单元部署于AIC核，Vector计算单元部署于AIV核，每个核均配备独立的Scalar单元，可独立加载各自的代码段。基于AI Core的硬件能力，开发者可实现Vector矢量算子、Cube矩阵算子及Mix CV融合算子三类算子。

### Vector矢量计算

Vector矢量计算主要包含基础算术、归约计算、数据类型转换等矢量计算。

在Atlas A2/A3训练和推理系列产品上，Vector矢量计算的数据来自于Unified Buffer（要求32B对齐），C语言编程在Vector计算单元上提供了通过UB指针直接操作计算的接口能力。此类直接操作UB的编程方式称为Memory矢量计算编程。

以Add计算为例，当开发者需要计算4096长度的向量加法, 可以通过`asc_add(dst, src0, src1, count)`的接口直接进行计算。更多内容可参考[Memory矢量计算编程](./Memory矢量计算编程.md)中有关Memory矢量计算编程的描述。

```c
// Define Vector operator, marked with __vector__ to execute on Device Vector core, suitable for operators with only Vector computation
__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    __ubuf__ float x[4096];
    __ubuf__ float y[4096];
    __ubuf__ float z[4096];

    // 1. Data transfer in: transfer GM data to UB, this implementation is omitted
    // ...
    // 2. Perform calculation for the first 4096 elements via asc_add
    asc_add(z, x, y, 4096);
}

```

Ascend 950PR/Ascend 950DT新一代架构, 引入寄存器可编程能力，在传统UB缓存体系的基础上，开放寄存器（Register）可编程能力，构建出「Global Memory → UB → Register」的三级内存层级。新一代架构下的矢量计算都需要从Unified Buffer加载到Register寄存器后，再利用Register寄存器进行计算操作。可参考[Reg矢量计算编程](./Reg矢量计算编程.md)中有关Reg矢量计算编程的描述。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。

### Cube矩阵计算

Cube矩阵计算主要涵盖矩阵乘计算与卷积计算，其数据主要来源于L1 Buffer与L0A/B Buffer。L1 Buffer用于矩阵计算数据缓存，L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer用于存储初始累加值及矩阵计算结果。此类基于Cube计算单元直接操作L1/L0A/L0B/L0C Buffer的编程范式称为[Cube矩阵计算编程](./Cube矩阵计算编程.md)。

以Matmul矩阵乘计算为例, 开发者通过操作Cube矩阵计算能力可以完成如下图所示的c = a * b + bias的能力。
- A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]。
- C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]。
- bias为矩阵乘偏置，形状为[1, N]。对A*B结果矩阵的每一行都采用该bias进行偏置。

![矩阵乘示意图](../../../../figures/矩阵乘示意图.png)

```c
// Define Cube matrix operator, marked with __cube__ to execute on Device Cube core, suitable for operators with only Cube computation
__global__ __cube__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    __cbuf__ half a_l1_buf[BLOCK_M * BLOCK_K];   // L1 staging area
    __cbuf__ half b_l1_buf[BLOCK_K * BLOCK_N];   // L1 staging area
    __ca__ half l0a_buf[BLOCK_M * BLOCK_K];    // L0A input
    __cb__ half l0b_buf[BLOCK_K * BLOCK_N];    // L0B input
    __cc__ float l0c_buf[BLOCK_M * BLOCK_N];   // L0C output

    // GM2L1 and L12L0 data transfer operations
    // ...
    // Execute Mmad, computation size is (BLOCK_M, BLOCK_K, BLOCK_N)
    asc_mmad(l0c_buf, l0a_buf, l0b_buf, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
    // Matrix data transfer out
}
```

### Mix CV融合计算

CV融合计算程序由多个独立的小算子融合而成，其功能与多个小算子的功能等价，性能方面通常优于独立的小算子。用户可以根据实际业务场景诉求，按照具体算法自由融合向量（Vector）、矩阵（Cube）算子以达到性能上的收益。融合了Cube计算、Vector计算的算子统称为CV融合算子。

比如对于LLM大模型中最核心的一个融合算子Flash Attention， 其核心实现如下图。图中的Matmul算子（Cube）、Scale算子（Vector）、Mask算子（Vector）、SoftMax算子（Vector）融合为一个大的算子Flash Attention。

![Flash Attention核心实现](../../../../figures/Flash-Attention核心实现.png)

```c
// Define CV fusion operator, marked with __mix__ to execute on Device, suitable for operators combining Cube and Vector computation
__global__ __mix__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    if ASCEND_IS_AIV {
        // ...
        asc_add(z, x, y, 4096);
        // ...
    }

    if ASCEND_IS_AIC {
        // ...
        asc_mmad(l0c_buf, l0a_buf, l0b_buf, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
        // ...
    }
}

```

融合算子在提升性能与充分发挥算力的基础上，通过合并算子减少计算量与内存占用、优化数据流传输、简化代码实现，实现计算效率、资源利用率与代码质量的同步提升。

## 总结

本章系统阐述了C语言编程的核心定位与技术体系。作为SIMD编程的底层接口，C语言编程直接映射NPU硬件指令，提供指令级透明可控能力，帮助开发者实现硬件峰值性能与精准问题定位。进一步学习可以参考[Cube矩阵计算编程](./Cube矩阵计算编程.md)、[Memory矢量计算编程](./Memory矢量计算编程.md)和[Reg矢量计算编程](./Reg矢量计算编程.md)。
