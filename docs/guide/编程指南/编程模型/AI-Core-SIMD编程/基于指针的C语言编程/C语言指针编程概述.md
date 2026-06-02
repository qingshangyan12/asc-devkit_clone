# C语言指针编程概述

Ascend C为AI Core提供了SIMD编程路径，**C语言指针编程**是该路径的底层核心接口，直接映射NPU硬件指令，提供对向量处理单元、矩阵运算单元、本地存储等硬件资源的精细控制能力。对于追求极致性能、习惯C语言编程的开发者，C语言指针编程是解锁NPU硬件潜能的关键技术路径。本章旨在帮助开发者建立对C语言指针编程的系统化认知，理解其底层技术定位、核心编程范式与开发方法论。全文遵循「背景动因—核心概念—编程范式—落地实践」的递进逻辑展开，完整覆盖理论根基与实操要义，帮助开发者快速掌握底层开发能力。

## 背景动因：C语言指针编程为何不可或缺？

早期算子开发以C语言裸指针编程为核心，其背景动因源于GPU硬件架构的复杂性——缓存、寄存器、共享内存等片上资源需精确调度。该模式依托C语言底层内存操控能力，通过裸指针直接寻址操作设备内存，绕过抽象层实现零开销访问；手动分配片上内存、插入显式同步语句，精确编排硬件执行时序；原生函数与硬件指令精准映射、调用即执行，无中间转换损耗。支持向量计算、矩阵乘、数据搬运等核心原语调用，是底层极致性能优化的核心方案，广泛应用于离散、归约、矩阵类等算子的底层实现。

## 编程范式：面向C指针的SPMD编程范式

C指针编程提供C完备编程能力，通过声明静态数组管理本地内存，由开发者自主管理同步。

### Kernel函数

[核函数](../核函数.md)在前序章节已作介绍，C语言指针编程在核函数定义与调用上沿用SIMD编程通用规范，无额外语法约束。

### 内存管理

AI Core采用分级存储架构，不同计算单元的编程视角有所差异：
  
  - **Cube矩阵计算**采用「Global Memory → L1/L0系列Buffer」两级层级, 其中L1 Buffer、L0A/L0B Buffer、L0C Buffer均服务于矩阵计算。
  
  - **Vector矢量计算**传统架构采用「Global Memory → UB」两级层级，Ascend 950PR/950DT引入寄存器后构建「Global Memory → UB → Register」三级层级，**Global Memory**存放输入输出数据，**UB**作为矢量计算数据枢纽，**Register**位于Vector计算单元最内层，具备最低访存延迟与最高带宽。
  
  ![AI Core存储层次结构](<../../../../figures/AI Core存储层次结构.png>)

#### 外部存储(Global Memory)

Global Memory是昇腾NPU的设备内存，位于AI Core外部，容量大、带宽高但访问延迟相对较长。用于存储算子的输入数据、输出结果和中间计算数据。 运行在Device侧的核函数可直接访问Global Memory。

Global Memory具有持久性：通过全局内存分配的空间、其中存储的数据将持续保留，直到该内存空间被释放或应用程序终止。用户通过Runtime API完成Device侧全局内存的管理。Host使用aclrtMalloc分配Device侧的全局内存，并通过aclrtMemcpy将数据从Host拷贝到Device侧的全局内存，或将数据从Device的全局内存拷贝回Host内存；通过aclrtMalloc分配的Device全局内存需使用aclrtFree接口释放。有关Runtime API的更多信息与细节，可以参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。

下述代码为Global Memory的使用提供了简易的示例。数据x、y、z均存储于Global Memory中，通过以下核函数实现对Global Memory的访问和存储。
```c
__global__ __aicore__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
{
    // 核内实现
}
```

#### 内部存储(Local Memory) 

AI Core包含三类基础计算单元：Cube负责矩阵计算、Vector负责矢量计算、Scalar负责标量计算。计算前需将数据从外部存储加载到内部存储，不同存储单元服务于不同计算单元：L1 Buffer和L0 Buffer支撑矩阵计算，Unified Buffer支撑矢量计算。

- Unified Buffer
  - **Unified Buffer内存申请**
  
  Unified Buffer是AI Core内主要用于矢量计算单元的内部存储单元，位于每个Vector Core(AIV)内部。与外部存储空间相比，Unified Buffer的容量较小，但具有更高的带宽和更低的访问延迟，可视为内核执行期间由用户管理的高速缓存资源。

  在C指针编程中，用户可直接通过数组定义的方式动态或者静态方式申请Unified Buffer。
  1. 静态申请：分配一段指定大小的内存空间，空间大小在编译时确定，不可动态修改，开发者通过数组分配申请。
    ```c
    constexpr uint32_t ub_size = 256;
    __ubuf__ half static_buffer[ub_size];  // Allocate UB, compiler automatically 32-byte aligns
    ```

  2. 动态申请：用户需要通过<<<>>>中参数dynamic_ub_size指定动态内存的空间大小，其大小在运行期确定。
    ```c
    extern __ubuf__ char dynamic_buffer[];
    ```
  > 📌 Unified Buffer地址必须32字节对齐，以匹配硬件总线粒度和SIMD计算单元并行度，非对齐访问将导致运行时错误。
  
  - Unified Buffer Bank冲突优化
  
  为了提高数据访问的效率和吞吐量，Unified Buffer采用了bank（大小相等的内存模块）结构设计。Unified Buffer总大小为192K(NPU架构版本2201)，划分为16个bank。每个bank由512行组成，每行长度为32B。这16个bank进一步组织为8个bank group，每个bank group包含2个bank，例如bank7和bank15组成一个bank group。

  ![UB Bank冲突示意图](UB Bank冲突示意图.png)

  每个bank可以独立地进行数据的读写操作，允许多个数据请求同时进行。然而，当多个读写操作试图同时访问同一个bank，由于硬件资源的限制，这些操作必须排队等待，会导致bank冲突，引起性能下降。
  bank冲突主要可以分为以下三种场景：
  1. **读写冲突**：读操作和写操作同时尝试访问同一个bank。
  2. **写写冲突**：多个写操作同时尝试访问同一个bank group。
  3. **读读冲突**：两个读操作同时尝试访问同一个bank，或者两个以上读操作同时尝试访问同一个bank group。 
   
  以**读写冲突**为例，根据Unified Buffer的Bank结构，当执行一条矢量加法计算时，产生和不产生读写冲突的场景分别如下：
  1. 假设src0的地址为0x10020, 如果dst的地址为0x10000, dst和src0的地址相差0x00020, src0位于Bank1，dst位于Bank0，不会产生冲突。
  2. 假设src0的地址为0x10020, 如果dst的地址为0x10120, dst和src0的地址相差0x00100, dst的地址正好位于src0的Bank内，故存在冲突。

  ![Unified Buffer 地址分配示意图](Unified Buffer 地址分配示意图.png)

  当前主要通过**优化计算逻辑**和**优化地址分配**的方式来避免Bank冲突的产生, 提升算子性能。
  - **优化计算逻辑**
  有一些特定的计算场景，会产生Bank冲突，譬如以下场景：对shape为(8, 16, 16)的输入执行(1, 0, 2)的transpose操作，输出shape为(16, 8, 16)。通过将计算逻辑由“跳读、连续写”调整为“连续读、跳写”，可避免冲突。
  
  ![读读冲突](../../../../figures/读读冲突.png)

  上图展示了“跳读、连续写”的计算逻辑，示例代码如下。该逻辑会导致同一Repeat内输入的8个DataBlock位于同一个bank group，从而发生读读冲突。
  ```c
  for (uint16_t i = 0; i < 16; ++i) {
      // 按列进行计算，block_stride设置为8，repeat_stride设置为1
      asc_add_scalar(dst + i * 128, src + i * 16, 1, repeat, 1, 16, 8, 8);
  }
  ```

  对于类似的算法实现场景，结合Unified Buffer的Bank组织结构特性, 通过将“跳读、连续写”修改成“连续读、跳写”，从而避免了读读冲突。

  ![读读冲突优化](../../../../figures/读读冲突优化.png)

  ```c
  for (uint16_t i = 0; i < 8; ++i) {
      // 按列进行计算，block_stride设置为1，repeat_stride设置为8
      asc_add_scalar(dst + i * 16, src + i * 256, 1, repeat, 8, 1, 8, 8);
  }
  ```
  
  - **优化地址分配**
  对于Bank冲突的优化，**优化计算逻辑**只能应用在特定场景，开发者在设计算子计算逻辑的时候将Bank冲突考虑进去即可避免类似问题，但是更多的避免Bank冲突的方法是通过优化地址分配来进行。
  以连续4096个float元素的加法`z = x + y`为例，通过在分配内存时适当扩大申请空间，保证在一个Repeat内，x和y不会同时位于同一个bank group，且x或y不会与z同时位于同一个bank。
  
  ![地址分配优化前示意图](../../../../figures/地址分配优化前示意图.png)

  ```c
  __ubuf__ float x[4096];  // 编译器统一分配，起始地址0x00000
  __ubuf__ float y[4096];  // 起始地址0x04000
  __ubuf__ float z[4096];  // 起始地址0x08000
  // 在一个Repeat内，x和y同时读同一个Bank Group，x/y和z同时读写同一个Bank。
  asc_add(z, x, y, 4096);
  ```
  为了保证`x`、`y`、`z`的地址分配在不同的Bank和Bank Group内，将`x`多申请256字节，避免一个Repeat内`x`和`y`同时读取同一个Bank Group，避免读读冲突。将`y`多申请空间，确保`z`不会跟`x`或者`y`落入到同一个Bank内，避免读写冲突。
  
  ![地址分配优化后示意图](../../../../figures/地址分配优化后示意图.png)
  
  ```c
  __ubuf__ float x[4096 + 64];  // 多申请256字节, 起始地址0x00000
  // 128 * 512是16个bank的空间，4096 * sizeof(float)+ 256是x所占的空间。 
  // 所以为了让z跟x/y不在一个Bank，需要将y的大小设置为 128 * 512 / sizeof(float) - (4096 + 256 / sizeof(float))
  __ubuf__ float y[128 * 128 - (4096 + 64)];  // 起始地址0x04000
  __ubuf__ float z[4096];  // 起始地址0x08000
  // 在一个Repeat内，x和y同时读同一个Bank Group，x/y和z同时读写同一个Bank。
  asc_add(z, x, y, 4096);
  ```

- L1 Buffer & L0 Buffer
  Cube计算单元是AI Core中专用于执行矩阵运算的单元，如下图所示，高亮部分展示了Cube计算单元及其直接访问的专用缓存：其中L0ABuffer用于存储左矩阵，L0BBuffer用于存储右矩阵，L0CBuffer则用于存储初始化累加值和矩阵计算结果。

  ![矩阵计算单元架构图(Ascend 950PR/Ascend 950DT)](../../../../figures/矩阵计算单元架构图.png)
  
  矩阵计算关联的存储单元主要为:
    - `L1 Buffer` : AI Core内部物理存储单元，空间相对较大，通常用于缓存矩阵计算的输入数据。矩阵计算的输入一般需要从GM搬运到L1Buffer，然后分别搬运到L0A Buffer和L0B Buffer。定义申请时使用地址限定符`__cbuf__`修饰，譬如: `__cbuf__ half l1_buffer[512];`。
    - `L0A Buffer` : AI Core内部物理存储单元，通常用于存储矩阵计算的左矩阵, 定义申请时使用地址限定符`__ca__`修饰。
    - `L0B Buffer` : AI Core内部物理存储单元，通常用于存储矩阵计算的右矩阵, 定义申请时使用地址限定符`__cb__`修饰。
    - `L0C Buffer` : AI Core内部物理存储单元，通常用于存储矩阵计算的结果，以及矩阵累加初始化值, 定义申请时使用地址限定符`__cc__`修饰。
    - `Fixpipe Buffer` : AI Core内部物理存储单元，通常用于存储Fixpipe搬运过程中所需的量化参数等数据, 定义申请时使用地址限定符`__fbuf__`修饰。
    - `BiasTable Buffer` : 偏置存储，AICore内部物理存储单元，通常用于存储矩阵计算所需的Bias（偏置）数据, 定义申请时使用地址限定符`__biasbuf__`修饰。
    
  在C指针编程中，同Unified Buffer, 用户也是直接通过数组定义的方式静态方式申请`L1 Buffer`、`L0A Buffer`等Cube计算单元的Buffer。
    ```c
    constexpr uint32_t m_size = 256;
    constexpr uint32_t n_size = 128;
    constexpr uint32_t k_size = 256;
    __cbuf__ half l1a_buffer[m_size * k_size];  // Allocate L1 Buffer, the allocation size must be 32-byte aligned.
    __cbuf__ half l1b_buffer[n_size * k_size];  // Allocate L1 Buffer, the allocation size must be 32-byte aligned.
    __ca__ half l0a_buffer[m_size * k_size];  // Allocate L0A Buffer, the allocation size must be 512-byte aligned.
    __cb__ half l0b_buffer[n_size * k_size];  // Allocate L0B Buffer, the allocation size must be 512-byte aligned.
    __cc__ half l0c_buffer[m_size * n_size];  // Allocate L0C Buffer, the allocation size must be 64-byte aligned.
    ```

#### 寄存器(Register)

Ascend 950PR/Ascend 950DT新一代架构引入寄存器可编程能力，Vector计算单元的所有计算均需经「GM → UB → Register」三级数据流转，在寄存器上完成矢量计算。C语言指针编程通过直接定义寄存器变量方式进行寄存器编程，根据架构设计，当前包含四类专用寄存器：

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入。

**表 2**  寄存器类型说明

<a name="table_reg_types"></a>
<table><thead align="left"><tr id="row_reg_header"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.4.1.1"><p id="p_reg_name"><a name="p_reg_name"></a><a name="p_reg_name"></a>寄存器名称</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.4.1.2"><p id="p_reg_type"><a name="p_reg_type"></a><a name="p_reg_type"></a>寄存器类型</p>
</th>
<th class="cellrowborder" valign="top" width="35%" id="mcps1.2.4.1.3"><p id="p_reg_desc"><a name="p_reg_desc"></a><a name="p_reg_desc"></a>寄存器描述</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.4.1.4"><p id="p_reg_scene"><a name="p_reg_scene"></a><a name="p_reg_scene"></a>使用场景</p>
</th>
</tr>
</thead>
<tbody><tr id="row_vector_data"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p_vector_data_name"><a name="p_vector_data_name"></a><a name="p_vector_data_name"></a><span id="ph_vector_data"><a name="ph_vector_data"></a><a name="ph_vector_data"></a>矢量数据寄存器</span></p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.2 "><p id="p_vector_data_type"><a name="p_vector_data_type"></a><a name="p_vector_data_type"></a>vector_half、vector_float、vector_int8_t、vector_bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.2.4.1.3 "><p id="p_vector_data_desc"><a name="p_vector_data_desc"></a><a name="p_vector_data_desc"></a>用于矢量计算，位宽为VL（Vector Length），可存储VL/sizeof(T)个数据元素。vector_half存储128个half，vector_float存储64个float。</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.4.1.4 "><p id="p_vector_data_scene"><a name="p_vector_data_scene"></a><a name="p_vector_data_scene"></a>矢量计算的基本存储单元，存储计算数据。</p>
</td>
</tr>
<tr id="row_mask"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p_mask_name"><a name="p_mask_name"></a><a name="p_mask_name"></a><span id="ph_mask"><a name="ph_mask"></a><a name="ph_mask"></a>掩码寄存器</span></p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.2 "><p id="p_mask_type"><a name="p_mask_type"></a><a name="p_mask_type"></a>vector_bool</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.2.4.1.3 "><p id="p_mask_desc"><a name="p_mask_desc"></a><a name="p_mask_desc"></a>指示计算过程中哪些元素参与计算，宽度为VL/8。支持静态模式（PAT_ALL/PAT_H/PAT_Q等）和动态创建。</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.4.1.4 "><p id="p_mask_scene"><a name="p_mask_scene"></a><a name="p_mask_scene"></a>控制矢量计算中的元素参与情况，处理尾部数据。</p>
</td>
</tr>
<tr id="row_addr"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p_addr_name"><a name="p_addr_name"></a><a name="p_addr_name"></a><span id="ph_addr"><a name="ph_addr"></a><a name="ph_addr"></a>地址寄存器</span></p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.2 "><p id="p_addr_type"><a name="p_addr_type"></a><a name="p_addr_type"></a>iter_reg</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.2.4.1.3 "><p id="p_addr_desc"><a name="p_addr_desc"></a><a name="p_addr_desc"></a>存储地址偏移量，循环中根据stride自动自增。通过asc_create_iter_reg_b16初始化。</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.4.1.4 "><p id="p_addr_scene"><a name="p_addr_scene"></a><a name="p_addr_scene"></a>循环迭代中自动管理地址偏移，简化地址计算。</p>
</td>
</tr>
<tr id="row_unalign"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.1 "><p id="p_unalign_name"><a name="p_unalign_name"></a><a name="p_unalign_name"></a><span id="ph_unalign"><a name="ph_unalign"></a><a name="ph_unalign"></a>非对齐寄存器</span></p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.4.1.2 "><p id="p_unalign_type"><a name="p_unalign_type"></a><a name="p_unalign_type"></a>vector_load_unalign、vector_store_unalign</p>
</td>
<td class="cellrowborder" valign="top" width="35%" headers="mcps1.2.4.1.3 "><p id="p_unalign_desc"><a name="p_unalign_desc"></a><a name="p_unalign_desc"></a>处理起始地址未32字节对齐的数据搬运。用作缓冲区优化UB和寄存器之间不对齐地址访问。需配合asc_loadunalign_pre、asc_storeunalign_post使用。</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.4.1.4 "><p id="p_unalign_scene"><a name="p_unalign_scene"></a><a name="p_unalign_scene"></a>UB和寄存器之间连续不对齐地址访问场景。</p>
</td>
</tr>
</tbody>
</table>

### 同步机制

在编程中，同步是指协调多个执行单元（如线程、进程等）对共享资源的访问顺序和时机来确保程序的正确性。如果没有同步来严格控制执行顺序，则会出现结果不一致、行为不可预测等多种问题。AI Core的同步，主要分为核内同步和核间同步。

- 核内同步
  - 为什么需要核内同步
    AI Core内部的执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行的方式运行，在读写同一存储资源时可能存在数据依赖关系。为确保数据一致性及计算正确性，需通过同步控制协调操作时序。

    针对NPU架构2201，硬件架构图如下，高亮部分展示了并行执行的计算单元和搬运单元。

    ![2201 硬件架构图](../../../../figures/2201_硬件架构图.png)

    针对NPU架构3510，硬件架构图如下，高亮部分展示了并行执行的计算单元和搬运单元。

    ![3510 硬件架构图](../../../../figures/3510_硬件架构图.png)

    当我们描述一个常见的Vector矢量计算时，需要执行下面三个步骤：1. 通过DMA将数据从Global Memory搬入到Local Memory。2. 进行矢量计算。3. 通过DMA将数据从Local Memory搬出到Global Memory。

    ![Vector计算数据流](../../../../figures/Vector计算数据流.png)

    从上图可以看到, 执行一个Vector矢量计算时，四个执行单元`Scalar`, `Vector`, `DMA(MTE2)`, `DMA(MTE3)`四个执行单元并行执行，如果访问同一片Local Memory, 需要同步机制来控制访问时序，保证先搬入Local Memory后在进行计算，然后计算完成后在搬出， 否则将会出现计算出错。

    ![同步顺序](../../../../figures/同步顺序.png)

    如上图所示, Vector矢量计算是由4个执行单元并行执行的，这些并行的执行单元在AI Core上映射到不同的执行数据流, 通常DMA数据搬入为`MTE2`数据流， DMA数据搬出为`MTE3`数据流, 同步即是控制这些数据流的访问顺序，保证执行正常。
    <table>
    <thead>
    <tr>
    <th><strong>流水类型</strong></th>
    <th><strong>含义</strong></th>
    </tr>
    </thead>
    <tbody>
    <tr>
    <td><strong>PIPE_S</strong></td>
    <td>标量流水线，使用GlobalTensor/LocalTensor的GetValue函数时为此流水</td>
    </tr>
    <tr>
    <td><strong>PIPE_V</strong></td>
    <td>矢量计算流水及部分硬件架构下的L0C Buffer->UB数据搬运流水</td>
    </tr>
    <tr>
    <td><strong>PIPE_M</strong></td>
    <td>矩阵计算流水</td>
    </tr>
    <tr>
    <td><strong>PIPE_MTE1</strong></td>
    <td>L1 Buffer->L0A Buffer、L1 Buffer->L0B Buffer数据搬运流水</td>
    </tr>
    <tr>
    <td><strong>PIPE_MTE2</strong></td>
    <td>GM->L1 Buffer、GM->UB等数据搬运流水</td>
    </tr>
    <tr>
    <td><strong>PIPE_MTE3</strong></td>
    <td>UB->GM等数据搬运流水</td>
    </tr>
    <tr>
    <td><strong>PIPE_FIX</strong></td>
    <td>L0C Buffer->GM、L0C Buffer->L1等数据搬运流水</td>
    </tr>
    </tbody>
    </table>
  - 核内同步关键特性
    为了保证不同计算单元的执行流水能够正确执行，AI Core根据不同的应用场景提供了多种同步能力。

    <a name="table_sync_mechanism"></a>
    <table><thead align="left"><tr id="row_sync_header"><th class="cellrowborder" valign="top" width="12%" id="mcps1.2.5.1.1"><p id="p_sync_scope">同步范围</p>
    </th>
    <th class="cellrowborder" valign="top" width="18%" id="mcps1.2.5.1.2"><p id="p_sync_type">同步类型</p>
    </th>
    <th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p_sync_c_api">C指针编程API</p>
    </th>
    <th class="cellrowborder" valign="top" width="20%" id="mcps1.2.5.1.5"><p id="p_sync_scene">适用场景</p>
    </th>
    </tr>
    </thead><tbody><tr id="row_mutex"><td class="cellrowborder" valign="top" rowspan="3" width="12%" headers="mcps1.2.5.1.1 "><p id="p_intra_scope"><span id="ph_intra_sync">核内同步</span></p>
    </td>
    <td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.5.1.2 "><p id="p_mutex_type">流水互斥同步<br><small>📌 Ascend 950PR/950DT引入</small></p>
    </td>
    <td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p_mutex_c_api"><code>asc_lock(pipe, mutex_id)</code><br><code>asc_unlock(pipe, mutex_id)</code></p>
    </td>
    <td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.5.1.5 "><p id="p_mutex_scene">通过锁定指定流水再释放流水来完成流水间的同步依赖</p>
    </td>
    </tr>
    <tr id="row_notify"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.5.1.2 "><p id="p_notify_type"> 核内多流水同步 </p>
    </td>
    <td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p_notify_c_api">全局同步：<br><code>asc_sync_notify</code><br><code>asc_sync_wait</code><br>单流水同步：<br><code>asc_sync</code><br><code>asc_sync_pipe</code></p>
    </td>
    <td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.5.1.5 "><p id="p_notify_scene">多条流水线间数据依赖同步，确保搬入完成后计算开始、计算完成后搬出开始</p>
    </td>
    </tr>
    <tr id="row_barrier"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.5.1.2 "><p id="p_barrier_type"> 同一流水内同步 </p>
    </td>
    <td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p_barrier_c_api"><code>asc_sync_data_barrier(arg)</code></p>
    </td>
    <td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.5.1.5 "><p id="p_barrier_scene">同一流水线内部约束执行顺序，避免"写未完成时读已执行"</p>
    </td>
    </tr>
    </tbody>
    </table>

    1. **流水互斥同步**
   
    该同步能力从Ascend 950PR/950DT开始引入并使用, AI Core提供了类似传统CPU中锁机制的同步能力。通过锁定指定流水再释放流水来完成流水间的同步依赖。每个锁有固定的一个MutexID，该ID可通过用户自定义（范围为0-27）并通过`asc_lock/asc_unlock`接口对指定流水进行阻塞或者释放。

    `asc_lock`：根据`mutex_id`获取Mutex, 并阻塞当前流水指令队列，直到对应的`mutex_id`被Unlock。
    `asc_unlock`：等待当前流水的前置指令退出后，根据`mutex_id`释放对应的Mutex。

    ```c
    // 锁定 MTE2 流水线互斥锁，确保当前线程独占 MTE2 资源进行数据搬运
    uint8_t mutex_id = 1;
    asc_lock(PIPE_MTE2, mutex_id);               
    asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
    asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));
    // 解锁 MTE2 流水线，允许其他线程使用 MTE2
    asc_unlock(PIPE_MTE2, mutex_id);       

    // 锁定 Vector 流水线互斥锁，确保当前线程独占向量计算资源
    asc_lock(PIPE_V, mutex_id);               
    asc_add(shared_buffer, x_local, y_local);
    // 解锁 Vector 流水线，释放计算资源供其他线程使用
    asc_unlock(PIPE_V, mutex_id);           
    ```

    2. **核内多流水同步**
    该同步主要用于核内多流水间的同步, 通过`asc_sync_notify/asc_sync_wait`对两条流水进行`notify`和`wait`处理，当Vector计算依赖DMA搬入时，需要插入`PIPE_V`等待`PIPE_MTE2`的同步，保证Global Memory的数据成功搬入到Local Memory后再进行Vector计算。

    `asc_sync_notify`: 当源流水的前序指令的所有读写操作都完成之后，当前指令开始执行，并将硬件中的对应标志位设置为1。`asc_sync_notify`只是设置硬件中的对应标志位，并不会阻塞源流水中的下一个指令。
    `asc_sync_wait`: 当目的流水执行到该指令时，如果发现硬件中对应标志位为0，目的流水的后续指令将一直被阻塞；如果发现硬件中对应标志位为1，则将硬件中对应标志位设置为0，同时目的流水的后续指令开始执行。
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
    
    3. **同一流水内同步**
    同一流水线中虽然指令是顺序执行，但并不意味着后一条指令开始执行时前一条指令执行结束。所以对于同一条流水线执行，如果存在前后数据依赖，也需要进行同步处理，C指针编程通过`asc_sync_pipe`接口来进行单流水线的同步。譬如前后两条矢量计算存在对Local Memory的依赖，则插入一条对于`PIPE_V`单流水的依赖。
    ```c
    asc_add(dst, src1, src0, total_length);
    // 后续asc_sub指令对dst存在依赖，因为asc_add和asc_sub都属于PIPE_V流水，则插入asc_sync_pipe(PIPE_V)进行单流水的同步
    asc_sync_pipe(PIPE_V);
    asc_sub(dst, dst, src0, total_length);
    ```
    
    另外可以直接通过`asc_sync_vector`或者`asc_sync_mte2`这些接口来直接等待某一条流水。
    ```c
    // 通过DMA(MTE2)进行GM2UB的搬入处理
    asc_copy_gm2ub((__ubuf__ void*)src0, (__gm__ void*)src0_gm, total_length * sizeof(float));
    asc_copy_gm2ub((__ubuf__ void*)src1, (__gm__ void*)src1_gm, total_length * sizeof(float));
    // 通过asc_sync_mte2等待MTE2流水执行完成
    asc_sync_mte2();
    // 进行矢量计算 Add，执行流水为PIPE_V
    asc_add(dst, src1, src0, total_length);
    ```

    4. **内存访问顺序控制**
    当后续执行指令对前序的内存访问存在数据依赖时，可以通过`asc_sync_data_barrier`来进行内存访问顺序同步，该接口会阻塞后续的指令执行，直到所有之前的内存访问指令都执行结束。
    ```c
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);  // Wait for all memory accesses to complete    
    ```

- 核间同步

    当一个核计算所依赖的数据，需要等待其他相关核的计算全部完成后才能执行时，则需要通过核间同步来确保多核间的时序正确性。
    以下图为例，AIC需要依赖于AIV的ReduceSum计算结果。由于整体矢量较大，必须拆分为多个部分，由每个AIV分别完成部分计算。每个AIV将其部分计算结果通过原子累加写入GM。AIC需要读取的必须是所有AIV均完成累加后的最终结果，因此需要通过核间同步来保证AIC读取ReduceSum结果时的时序正确性。
    
    ![核间同步业务场景示例](../../../../figures/核间同步业务场景示例.png)
    
    从上图可以看到，当前AI Core是由AIC核和AIV核组成，其中AIC核主要用于Cube计算, AIV核负责Vector计算。AIC/AIV按group进行划分。一个group内细分block及subblock, block与subblock比列为1:N(N>=1), block表示多少个“主核”，subblock表示一个"主核"带多少个"从核"。
    算子按计算特征可分为三类：Cube算子（矩阵计算）、Vector算子（矢量计算）和Mix算子（同时包含矩阵和矢量计算）。如表1所示，算子类型决定了其所需的核间同步方式和可选的group配置模式。其中Mix算子通过函数修饰符指定AIC与AIV的block/subblock关系；Cube算子和Vector算子为单类核执行，不涉及block/subblock划分。AIV为block且AIC为subblock的配置（即Vector核为主、Cube核为辅的模式）当前不支持。

    ![block和subblock之间关系（灰色部分表示一个group，即1个block和N个subblock）](../../../../figures/block和subblock之间关系.png)

    针对AI Core 多核之间的关系和以及AI Core的能力， C指针根据不同的应用场景分别提供了对应的核间同步的接口。
    • 同步等待全部AIC或者同步等待全部AIV执行结束。
    该能力为多个AIC或AIV之间的同步控制，通过`asc_sync_inter_wait`和`asc_sync_inter_arrive`的接口能力实现全部AIC或者全部AIV的同步能力。如上述场景，当要进行一个ReduceSum多核累加时, 则需要通过该接口实现一个全AIV核的同步能力。
    ```c
    if ASCEND_IS_AIV {
        int64_t flagID = 1;
        asc_sync_inter_arrive(PIPE_S, flagID);
        asc_sync_inter_wait(PIPE_S, flagID);
        
    }
    ```
        
    • 单个AI Core内，AIC与全部AIV之间的同步。
    该能力为单个AI Core内, 一个group内的block跟subblock之间的同步能力, 该能力为Ascend 950PR/Ascend 950DT架构新增的能力, 支持AIV0与AIV1可单独触发AIC等待。
    ```c
    // 当PIPE_FIX流水执行完毕后, AIC分别给AIV0和AIV1发起arrive的能力
    if ASCEND_IS_AIC {
        asc_sync_intra_arrive(PIPE_FIX, 0);
        asc_sync_intra_arrive(PIPE_FIX, 16);
    }
    // AIV侧代码进行同步等待，等待AIC的同步信号后才执行PIPE_MTE3流水
    if ASCEND_IS_AIV {
        asc_sync_intra_wait(PIPE_MTE3, 0);
    }
    ```

本章系统阐述了C语言指针编程的核心定位与技术体系。作为SIMD编程的底层接口，C语言指针编程直接映射NPU硬件指令，提供指令级透明可控能力，帮助开发者实现硬件峰值性能与精准问题定位。编程范式遵循SPMD模型的四步法，核心要点在于显式同步机制的正确应用。