# C++ Tensor编程概述

C++ Tensor编程是Ascend C SIMD编程路径的核心高级接口，基于Tensor对象抽象封装内存地址，提供类型安全、编译期检查的高层次编程能力。开发者通过LocalTensor、GlobalTensor、RegTensor等Tensor对象管理内存，无需直接操作指针地址，搬运计算接口直接操作Tensor对象即可完成数据流转。该路径适用于追求开发效率、类型安全、代码可维护性的算子开发场景，是习惯C++面向对象编程的开发者快速上手NPU算子开发的关键技术路径。本章旨在帮助开发者建立对C++ Tensor编程的系统化认知，理解其Tensor抽象编程范式、核心编程模型与开发方法论。

## Kernel函数

[核函数](../核函数.md)在前序章节已作介绍，C++ Tensor编程在核函数定义与调用上沿用SIMD编程通用规范，无额外语法约束。

## 内存层级

AI Core采用分级存储架构，Cube矩阵计算采用「Global Memory → L1/L0系列Buffer」两级层级服务于矩阵计算，Vector矢量计算传统架构为「Global Memory → UB」两级层级，而Ascend 950PR/Ascend 950DT引入寄存器后构建「Global Memory → Unified Buffer → Register」三级层级，其中Global Memory存放输入输出数据、Unified Buffer作为矢量计算中间缓存、Register位于Vector计算单元最内层直接与执行单元交互。

<img src="../../../../figures/AI%20Core存储层次结构.png" alt="AI Core存储层次结构" width="800" />

### Tensor内存抽象

Tensor本质是对内存地址的抽象封装，通过数组方式管理内存并执行计算，更匹配SIMD硬件执行模型。其核心思想是将原始内存地址抽象为Tensor对象，实现从"地址操作"到"对象管理"的编程层次提升。

根据芯片分级存储架构，Tensor分为两类：**GlobalTensor**对应Global Memory，存放算子输入/输出数据；**LocalTensor**对应片上内存（UB、L1、L0A/L0B/L0C等缓冲区），承载计算过程数据。Tensor体系持续演进：基础Tensor仅携带数据指针和大小信息，需手动传递操作参数；扩展Tensor引入Layout概念，携带Shape和Stride信息，接口可自动推导搬运长度、计算元素数等参数，大幅降低编程复杂度。

#### 基础Tensor与扩展Tensor

**基础Tensor（不带Layout）**仅封装数据指针与大小信息，无法表达数据的物理排布方式。开发者在执行搬运和计算时必须手动传入size/count参数、自行计算偏移与步长，面对高维张量时易出错且代码冗长。

```cpp
__global__ __vector__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z) {
  // 定义GlobalTensor并设置缓冲区
  AscendC::GlobalTensor<float> xGm, yGm, zGm;
  xGm.SetGlobalBuffer(x + block_idx * blockLength, blockLength);
  yGm.SetGlobalBuffer(y + block_idx * blockLength, blockLength);
  zGm.SetGlobalBuffer(z + block_idx * blockLength, blockLength);

  // 通过Allocator分配Local Memory(Unified Buffer)
  AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
  AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, blockLength>();
  AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, blockLength>();
  AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, blockLength>();

  // 搬运和计算操作需手动传入size参数
  AscendC::DataCopy(xLocal, xGm, blockLength);
  AscendC::Add(zLocal, xLocal, yLocal, blockLength);
}
```

**扩展Tensor（带有Layout）**封装Shape和Stride信息，接口自动推导操作参数，实现编译期类型安全。开发者无需关注内存布局细节，一行代码即可完成切片、搬运与计算。当前能力使能于Cube矩阵计算，后续将逐步支持Vector矢量计算。

```cpp
__global__ __cube__ void matmul_custom(__gm__ half* a, __gm__ half* b, __gm__ half* c) {
  // 定义带有Layout的GlobalTensor
  auto gmA = MakeTensor(MakeMemPtr(a), MakeFrameLayout<NDExtLayoutPtn>(M, K));
  auto gmB = MakeTensor(MakeMemPtr(b), MakeFrameLayout<NDExtLayoutPtn>(K, N));
  auto gmC = MakeTensor(MakeMemPtr(c), MakeFrameLayout<NDExtLayoutPtn>(M, N));

  // 构造带有Layout的LocalTensor
  __cbuf__ half l1ABuf[baseM * K];
  __cbuf__ half l1BBuf[K * baseN];
  auto l1ATensor = MakeTensor(MakeMemPtr(l1ABuf), l1ALayout);
  auto l1BTensor = MakeTensor(MakeMemPtr(l1BBuf), l1BLayout);

  // Copy自动推导参数，无需手动传入size
  Copy(l1ATensor, globalA);
  Copy(l1BTensor, globalB);
}
```

### 外部存储

Global Memory是昇腾NPU的设备内存，位于AI Core外部，容量大、带宽高但访问延迟较长。用于存储算子的输入数据、输出结果和中间计算数据。Global Memory具有持久性：其中存储的数据持续保留，直到内存空间被释放或应用程序终止。

用户通过Runtime API管理Device侧全局内存。Host侧使用`aclrtMalloc`分配Device内存，通过`aclrtMemcpy`完成Host与Device间的数据拷贝，通过`<<<>>>`拉起核函数执行，最后使用`aclrtFree`释放内存。有关Runtime API的详细信息，可参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。

```cpp
// 核函数定义（使用__global__前缀标识Host调用、Device执行）
__global__ __aicore__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
{
    AscendC::GlobalTensor<float> xGm;
    xGm.SetGlobalBuffer(x + block_idx * blockLength, blockLength);
}

// Host侧：分配Device内存、拷贝数据、拉起核函数
std::vector<float> kernel_add(std::vector<float> &x, std::vector<float> &y) {
    uint8_t *xHost = reinterpret_cast<uint8_t *>(x.data());
    uint8_t *yHost = reinterpret_cast<uint8_t *>(y.data());

    // 分配Device内存
    aclrtMalloc((void **)&xDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&yDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&zDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);

    // Host到Device数据拷贝
    aclrtMemcpy(xDevice, totalByteSize, xHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(yDevice, totalByteSize, yHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);

    // 拉起核函数
    add_custom<<<numBlocks, nullptr, stream>>>(xDevice, yDevice, zDevice);
    aclrtSynchronizeStream(stream);

    // 释放内存
    aclrtFree(xDevice);
    aclrtFree(yDevice);
    aclrtFree(zDevice);
}
```

### 内部存储

AI Core包含三类计算单元：Cube负责矩阵计算、Vector负责矢量计算、Scalar负责标量计算。计算前需将数据从外部存储加载到内部存储，不同存储单元服务不同计算单元：Unified Buffer支撑矢量计算，L1 Buffer和L0 Buffer支撑矩阵计算。

#### Unified Buffer

Unified Buffer是AI Core内主要用于矢量计算的内部存储单元，位于每个Vector Core（AIV）内部。容量较小但带宽高、延迟低，可视为内核执行期间由用户管理的高速缓存。

在C++ Tensor编程中，通过`LocalTensor`对象管理Unified Buffer内存：

- **基础Tensor**：通过`LocalMemAllocator`分配器创建，指定物理存储位置和内存大小
- **扩展Tensor**：通过`MakeTensor`创建，携带Shape和Stride信息

```cpp
// 基础Tensor：通过Allocator分配
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
AscendC::LocalTensor<half> ubTensor = ubAllocator.Alloc<half, 256>();

// 扩展Tensor：通过MakeTensor创建
auto ubTensor = MakeTensor(ubPointer, MakeShape(128, 64));
```

> 📌 Unified Buffer地址必须32字节对齐，以匹配硬件总线粒度和SIMD计算单元并行度，非对齐访问将导致运行时错误。

#### Register File

Ascend 950PR/Ascend 950DT引入寄存器可编程能力，Vector计算单元的所有计算均需经「GM → UB → Register」三级数据流转，在寄存器上完成矢量计算。C++ Tensor编程通过定义RegTensor方式进行寄存器编程。详见[Reg矢量计算编程](./Reg矢量计算编程.md)。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。

#### L1 Buffer & L0 Buffer

Cube计算单元专用的缓存为：L0A Buffer存储左矩阵，L0B Buffer存储右矩阵，L0C Buffer存储累加值和计算结果。L1 Buffer用于缓存矩阵计算输入数据。

通过`LocalTensor`对象管理这些存储单元：

| 存储单元 | 作用 | 示例 |
|---------|------|------|
| L1 Buffer | 缓存矩阵计算输入数据 | `LocalTensor<half> l1_buffer;` |
| L0A Buffer | 存储左矩阵 | `LocalTensor<half> l0a_buffer;` |
| L0B Buffer | 存储右矩阵 | `LocalTensor<half> l0b_buffer;` |
| L0C Buffer | 存储计算结果 | `LocalTensor<half> l0c_buffer;` |
| Fixpipe Buffer | 存储量化参数 | `LocalTensor<half> fixpipe_buffer;` |
| BiasTable Buffer | 存储Bias数据 | `LocalTensor<half> bias_buffer;` |

为提升Cube计算效率，L1 Buffer和L0 Buffer采用分形格式。不同产品型号的分形格式存在差异：

| 存储单元 | Atlas A2/A3训练和推理系列产品 | Ascend 950PR/Ascend 950DT |
|---------|----------------------------|---------------------------|
| L1 Buffer | Nz | Nz |
| L0A Buffer | Zz | Nz |
| L0B Buffer | Zn | Zn |
| L0C Buffer | Nz | Nz |

以`FRACTAL_Nz`为例，内存排布采用大N外部column major（列主序）+ 小z内部row major（行主序）存放：

![FRACTAL_Nz内存排布示意图](../../../../figures/FRACTAL_NZ内存排布示意图.png)

#### Bank冲突

为了提高数据访问的效率和吞吐量，内部存储采用了bank（大小相等的内存模块）结构设计。以Unified Buffer为例，Ascend 950PR/Ascend 950DT产品上，Unified Buffer总大小为256KB（256 × 1024字节），包含8个bank group，每个bank group包含2个bank。每个bank大小为16KB，由512行组成，每行长度为32B，采用低位地址交织。Atlas A2/A3训练和推理系列产品的UB容量规格请参考相应硬件架构文档。

<img src="../../../../figures/UB_Bank冲突示意图.png" alt="UB Bank冲突示意图" width="800" />

该架构地址使用低位编址，地址排布如下图所示，每个bank可以独立地进行数据的读写操作，允许多个数据请求同时进行。然而，当多个读写操作试图同时访问同一个bank，由于硬件资源的限制，这些操作必须排队等待，会导致bank冲突，引起性能下降。

![低位交织地址排布](../../../../figures/低位交织地址排布.png)

每个bank一拍只能完成一读或者一写，每个bank group最多只允许2读0写或者1读1写。根据内存结构，bank冲突主要可以分为以下三种类型：
- 读写冲突：读操作和写操作同时尝试访问同一个bank，或者两个读操作和一个写操作同时尝试访问同一个bank group。
- 写写冲突：多个写操作同时尝试访问同一个bank group。
- 读读冲突：两个读操作访问同一个bank，或两个以上操作同时尝试访问同一个bank group。

以读写冲突为例，当进行一组`dst = src0 + src1`的矢量加法计算时，如果`src0`的起始地址在0x0000，`dst`的起始地址为0x4000，结合Bank的排布，可以看到当前`src0`和`dst`的地址分别属于同一个bank和同一个bank group，则存在读写冲突。

当遇到Bank冲突时，会引起算子性能的下降，通常可以通过优化计算逻辑和优化分配地址方式来避免冲突的产生，详情请参考[避免Bank冲突](../../../../算子实践参考/SIMD算子性能优化/内存访问/避免UB的bank冲突/避免bank冲突（NPU架构版本3510）.md)。

## 同步机制

在编程中，同步是指协调多个执行单元（如线程、进程等）对共享资源的访问顺序和时机来确保程序的正确性。如果没有同步来严格控制执行顺序，则会出现结果不一致、行为不可预测等多种问题。

AI Core的同步主要分为**核内同步**和**核间同步**。

---

### 核内同步

AI Core内部的执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行的方式运行，在读写同一存储资源时可能存在数据依赖关系。为确保数据一致性及计算正确性，需通过同步控制协调操作时序。

下图示例描述了一个常见的Vector计算数据流：

1. 通过DMA将数据从Global Memory搬入到Local Memory
2. 进行矢量计算
3. 通过DMA将数据从Local Memory搬出到Global Memory

<img src="../../../../figures/Vector计算数据流.png" alt="Vector计算数据流" width="800" />

四个执行单元Scalar、Vector、DMA（MTE2）、DMA(MTE3)并行执行，若访问同一片Local Memory，需要同步机制来控制它们的访问时序：保证先搬入Local Memory后再计算，计算完成后再搬出。

<img src="../../../../figures/同步顺序.png" alt="同步顺序示意图" width="800" />

> 📌 提示：上例描述的是主要路径，当`Scalar`执行单元读写GM或者UB时，也需要考虑`Scalar`流水与其他流水的同步。

AI Core上的执行单元分别属于不同的执行流水，同步即是保证不同执行流水间能够正确执行，如下为AI Core内的主要执行流水。

| 流水类型 | 含义 |
|---------|------|
| **PIPE_S** | 标量流水线 |
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
<td><p><strong>机制一</strong>: Mutex互斥锁用于核内异步流水指令之间的同步处理，其功能类似于传统CPU中的锁机制。该同步机制为Ascend 950PR/Ascend 950DT新引入机制，相较于SetFlag/WaitFlag方式使用更简便。</p></td>
<td><code>Mutex::Lock</code> & <code>Mutex::Unlock</code></td>
<td>
        <code>AscendC::Mutex::Lock&lt;PIPE_MTE2&gt;(mutexId);</code><br>
        <code>AscendC::DataCopy(srcLocal, srcGlobal, dataSize);</code><br>
        <code>AscendC::Mutex::Unlock&lt;PIPE_MTE2&gt;(mutexId);</code><br>
        <code></code><br>
        <code>AscendC::Mutex::Lock&lt;PIPE_V&gt;(mutexId);</code><br>
        <code>AscendC::Add(dstLocal, src0Local, src1Local, dataSize);</code><br>
        <code>AscendC::Mutex::Unlock&lt;PIPE_V&gt;(mutexId);</code>
      </td>
</tr>
<tr>
<td><p><strong>机制二</strong>: SetFlag与WaitFlag机制实现两条流水间的同步等待，用于核内异步流水的同步协调。Ascend 950PR/Ascend 950DT引入Mutex机制后，仍保持对该接口的兼容支持，但优先推荐使用Mutex机制进行多流水同步。</p></td>
<td><code>SetFlag</code> & <code>WaitFlag</code></td>
<td>
        <code>AscendC::DataCopy(srcLocal, srcGlobal, dataSize);</code><br>
        <code>AscendC::SetFlag&lt;HardEvent::MTE2_V&gt;(eventId);</code><br>
        <code>AscendC::WaitFlag&lt;HardEvent::MTE2_V&gt;(eventId);</code><br>
        <code>AscendC::Add(dstLocal, src0Local, src1Local, count);</code>
      </td>
</tr>
<tr>
<td><strong>单流水同步</strong></td>
<td><p>PipeBarrier等待同一流水内前序指令完成，用于同一流水内存在前后数据依赖。</p></td>
<td><code>PipeBarrier</code></td>
<td>
        <code>AscendC::Add(dstLocal, src0Local, src1Local, count);</code><br>
        <code>AscendC::PipeBarrier&lt;PIPE_V&gt;();</code><br>
        <code>AscendC::Sub(dstLocal, dstLocal, src0Local, count);</code>
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

算子按计算特征可划分为三类：Cube算子（矩阵计算）、Vector算子（矢量计算）和Mix算子（矩阵与矢量混合计算）。算子类型决定了其核间同步方式与group配置模式的选择。针对不同算子场景，C++ Tensor编程提供了相应的编程接口，以满足多样化的算子开发需求。

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
      <td><pre><code>CrossCoreSetFlag
                      CrossCoreWaitFlag</code></pre></td>
      <td>
        <code>if ASCEND_IS_AIC {</code><br>
        <code>    AscendC::CrossCoreSetFlag&lt;0x0, PIPE_FIX&gt;(flagID);</code><br>
        <code>    AscendC::CrossCoreWaitFlag(flagID);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>所有AIV核同步</strong></td>
      <td><p>同步所有AIV核，通常用于对Vector算子计算结果进行多核累加操作。</p></td>
      <td><code>CrossCoreSetFlag</code><br><code>CrossCoreWaitFlag</code></td>
      <td>
        <code>if ASCEND_IS_AIV {</code><br>
        <code>    AscendC::CrossCoreSetFlag&lt;0x0, PIPE_MTE3&gt;(flagID);</code><br>
        <code>    AscendC::CrossCoreWaitFlag(flagID);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>单个AI Core内两个AIV子核之间同步</strong></td>
      <td><p>同步单个AI Core内两个AIV子核，通常用于Vector算子内需要分组进行同步的场景。</p></td>
      <td><code>CrossCoreSetFlag</code><br><code>CrossCoreWaitFlag</code></td>
      <td>
        <code>if ASCEND_IS_AIV {</code><br>
        <code>    AscendC::CrossCoreSetFlag&lt;0x1, PIPE_MTE3&gt;(flagID);</code><br>
        <code>    AscendC::CrossCoreWaitFlag(flagID);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>单个AI Core内AIC与AIV同步</strong></td>
      <td><p>同步单个AI Core内AIC与AIV核，通常用于Mix算子中Cube与Vector间的交互操作。两个AIV同时触发AIC等待。</p></td>
      <td><code>CrossCoreSetFlag</code><br><code>CrossCoreWaitFlag</code></td>
      <td>
        <code>if ASCEND_IS_AIC {</code><br>
        <code>    AscendC::CrossCoreSetFlag&lt;0x2, PIPE_FIX&gt;(0);</code><br>
        <code>}</code><br>
        <code>if ASCEND_IS_AIV {</code><br>
        <code>    AscendC::CrossCoreWaitFlag(0);</code><br>
        <code>}</code>
      </td>
    </tr>
    <tr>
      <td><strong>单个AI Core内AIC与AIV同步(Ascend 950PR/Ascend 950DT新引入)</strong></td>
      <td><p>同步单个AI Core内AIC与AIV核，通常用于Mix算子中Cube与Vector间的交互操作。AIV0与AIV1可单独触发AIC等待。</p></td>
      <td><code>CrossCoreSetFlag</code><br><code>CrossCoreWaitFlag</code></td>
      <td>
        <code>if ASCEND_IS_AIC {</code><br>
        <code>    AscendC::CrossCoreSetFlag&lt;0x4, PIPE_FIX&gt;(0);</code><br>
        <code>    AscendC::CrossCoreSetFlag&lt;0x4, PIPE_FIX&gt;(16);</code><br>
        <code>}</code><br>
        <code>if ASCEND_IS_AIV {</code><br>
        <code>    AscendC::CrossCoreWaitFlag(0);</code><br>
        <code>}</code>
      </td>
    </tr>

  </tbody>
</table>

> 📌 基于分离模式（AIC和AIV分离）开发融合算子时，算子逻辑中通常同时包含AIV和AIC的处理逻辑，此时需要使用Ascend C提供的宏ASCEND_IS_AIV/ASCEND_IS_AIC来对AIV和AIC的代码进行隔离;

## 算子类型

前序章节抽象硬件架构已阐述，AI Core包含Vector和Cube两类计算单元，分别用于向量计算和矩阵计算。Cube计算单元部署于AIC核，Vector计算单元部署于AIV核，每个核均配备独立的Scalar单元，可独立加载各自的代码段。基于AI Core的硬件能力，开发者可实现Vector矢量算子、Cube矩阵算子及Mix CV融合算子三类算子。

### Vector矢量计算

Vector矢量计算主要包含基础算术、归约计算、数据类型转换等矢量计算。

在Atlas A2/A3训练和推理系列产品上，Vector矢量计算的数据来自于Unified Buffer（要求32B对齐），C++ Tensor编程在Vector计算单元上提供了通过LocalTensor直接操作计算的接口能力。此类直接操作UB的编程方式称为Memory矢量计算编程。

以Add计算为例，当开发者需要计算4096长度的向量加法，可以通过`Add(dstTensor, src0Tensor, src1Tensor, count)`的接口直接进行计算。更多内容可参考[MemoryBase矢量计算编程](./MemoryBase矢量计算编程.md)中有关Memory矢量计算编程的描述。

```cpp
// Define Vector operator, marked with __aicore__ to execute on Device, suitable for operators with only Vector computation
__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
    AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, 4096>();
    AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, 4096>();
    AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, 4096>();

    // 1. Data transfer in: transfer GM data to UB, this implementation is omitted
    // ...
    // 2. Perform calculation for the first 4096 elements via Add interface
    AscendC::Add(zLocal, xLocal, yLocal, 4096);
}
```

Ascend 950PR/Ascend 950DT新一代架构，引入寄存器可编程能力，在传统UB缓存体系的基础上，开放寄存器（Register）可编程能力，构建出「Global Memory → UB → Register」的三级内存层级。新一代架构下的矢量计算都需要从Unified Buffer加载到Register寄存器后，再利用Register寄存器进行计算操作。可参考[Reg矢量计算编程](./Reg矢量计算编程.md)中有关Reg矢量计算编程的描述。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。

### Cube矩阵计算

Cube矩阵计算主要涵盖矩阵乘计算与卷积计算，其数据主要来源于L1 Buffer与L0A/B Buffer。L1 Buffer用于矩阵计算数据缓存，L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer用于存储初始累加值及矩阵计算结果。此类基于Cube计算单元直接操作L1/L0A/L0B/L0C Buffer的编程范式称为[Cube矩阵计算编程](./Cube矩阵计算编程.md)。

以Matmul矩阵乘计算为例，开发者通过操作Cube矩阵计算能力可以完成如下图所示的c = a * b + bias的能力。
- A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]。
- C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]。
- bias为矩阵乘偏置，形状为[1, N]。对A*B结果矩阵的每一行都采用该bias进行偏置。

![矩阵乘示意图](../../../../figures/矩阵乘示意图.png)

```cpp
// Define Cube matrix operator, marked with __cube__ to execute on Device Cube core, suitable for operators with only Cube computation
__global__ __cube__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
    AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0aAllocator;
    AscendC::LocalMemAllocator<AscendC::Hardware::L0B> l0bAllocator;
    AscendC::LocalMemAllocator<AscendC::Hardware::L0C> l0cAllocator;
    
    LocalTensor<half> a_l1_buf = l1Allocator.Alloc<half, BLOCK_M * BLOCK_K>();
    LocalTensor<half> b_l1_buf = l1Allocator.Alloc<half, BLOCK_K * BLOCK_N>();
    LocalTensor<half> l0a_buf = l0aAllocator.Alloc<half, BLOCK_M * BLOCK_K>();
    LocalTensor<half> l0b_buf = l0bAllocator.Alloc<half, BLOCK_K * BLOCK_N>();
    LocalTensor<float> l0c_buf = l0cAllocator.Alloc<float, BLOCK_M * BLOCK_N>();

    // GM2L1 and L12L0 data transfer operations
    // ...
    // Execute Mmad, computation size is (BLOCK_M, BLOCK_K, BLOCK_N)
    AscendC::Mmad(l0c_buf, l0a_buf, l0b_buf, {BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false});
    // Matrix data transfer out
}
```

### Mix CV融合计算

CV融合计算程序由多个独立的小算子融合而成，其功能与多个小算子的功能等价，性能方面通常优于独立的小算子。用户可以根据实际业务场景诉求，按照具体算法自由融合向量（Vector）、矩阵（Cube）算子以达到性能上的收益。融合了Cube计算、Vector计算的算子统称为CV融合算子。

比如对于LLM大模型中最核心的一个融合算子Flash Attention，其核心实现如下图。图中的Matmul算子（Cube）、Scale算子（Vector）、Mask算子（Vector）、SoftMax算子（Vector）融合为一个大的算子Flash Attention。

![Flash Attention核心实现](../../../../figures/Flash-Attention核心实现.png)

```cpp
// Define CV fusion operator, marked with __mix__ to execute on Device, suitable for operators combining Cube and Vector computation
__global__ __mix__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    if ASCEND_IS_AIV {
        // ...
        AscendC::Add(zLocal, xLocal, yLocal, 4096);
        // ...
    }

    if ASCEND_IS_AIC {
        // ...
        AscendC::Mmad(l0c_buf, l0a_buf, l0b_buf, {BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false});
        // ...
    }
}
```

融合算子在提升性能与充分发挥算力的基础上，通过合并算子减少计算量与内存占用、优化数据流传输、简化代码实现，实现计算效率、资源利用率与代码质量的同步提升。

## 总结

本章系统阐述了C++ Tensor编程的核心定位与技术体系。作为SIMD编程的高级接口，C++ Tensor编程基于Tensor对象抽象封装内存地址，提供类型安全、编译期检查的高层次编程能力，帮助开发者提升开发效率、降低编程复杂度。进一步学习可以参考[Cube矩阵计算编程](./Cube矩阵计算编程.md)、[MemoryBase矢量计算编程](./MemoryBase矢量计算编程.md)和[Reg矢量计算编程](./Reg矢量计算编程.md)。
