# C指针编程概述

Ascend C为AI Core提供了SIMD编程路径，**C指针编程**是该路径的底层核心接口，直接映射NPU硬件指令，提供对向量处理单元、矩阵运算单元、本地存储等硬件资源的精细控制能力。对于追求极致性能、习惯C语言编程的开发者，C指针编程是解锁NPU硬件潜能的关键技术路径。本章旨在帮助开发者建立对C指针编程的系统化认知，理解其底层技术定位、核心编程范式与开发方法论。全文遵循「背景动因—核心概念—编程范式—落地实践」的递进逻辑展开，完整覆盖理论根基与实操要义，帮助开发者快速掌握底层开发能力。

## 背景动因：C指针编程为何不可或缺？

早期算子开发以C语言裸指针编程为核心。依托C语言底层内存操控能力，开发者可直接通过指针寻址操作设备内存，并使用与硬件底层指令精准映射的原生接口，支持向量计算、矩阵乘、数据搬运等硬件原语调用。该模式可实现硬件缓存、寄存器等资源的精细化管理，是底层极致性能优化的核心方案，广泛应用于离散类、归约类、矩阵类等核心算子的底层实现。C指针编程精准适配C开发者的编程习惯，实现对底层硬件的精细化控制。

### C指针编程的核心特征与本质定义

- **C语言函数封装**，接口清晰简洁，符合C编程习惯。
- 使用数组分配片上内存，显式插入同步语句，**精细化控制硬件资源**。
- 函数与硬件指令一一对应，调用即执行。接口设计简洁轻量，无冗余封装层，**直接映射硬件能力**。

## 编程范式: 面向C指针的SPMD编程范式

### Kernel函数及函数修饰符

核函数（Kernel Function）是Ascend C算子设备侧实现的入口。Ascend C允许用户使用C/C++函数的语法扩展来编写设备端的运行代码，用户在核函数中进行数据访问和计算操作，由此实现该算子的所有功能。区别于普通的C++函数调用时仅执行一次，当核函数被调用时，多个核都执行相同的核函数代码，具有相同的函数入参，并行执行。

使用__global__函数类型限定符来标识它是一个核函数；使用__vector__限定符标识该核函数在Vector核执行；其指针入参变量需要增加变量类型限定符__gm__，表明该指针变量指向Global Memory上某处内存地址；使用<<<...>>>内核调用符调用执行核函数，并指定调用时的执行核数。
- `__global_`: 执行空间限定符，声明一个Kernel函数。
- `__vector__`: 标识该核函数仅在Vector核执行。针对耦合模式的硬件架构，该修饰符不生效。
- `__cube__`: 标识该核函数仅在Cube核执行。针对耦合模式的硬件架构，该修饰符不生效。
- `__mix__`: 标识该核函数同时在Cube核和Vector核上执行。(cube, vec)分别表示核函数启动的Cube核和Vector核的配比，支持的配比为(1, 0)，(0, 1)，(1, 1)， (1, 2)。针对耦合模式的硬件架构，该修饰符不生效。
```c
// 实现核函数
__vector__ __global__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint32_t blockLength = TILE_LENGTH * NUM_BLOCKS / asc_get_block_num();
}

// 调用核函数
void add_custom_do(uint32_t numBlocks, void* l2ctrl, void* stream, uint8_t* x, uint8_t* y, uint8_t* z)
{
    add_custom<<<numBlocks, l2ctrl, stream>>>(x, y, z);
}
```

另外，通过`__aicore__`和`__host__`限定符来声明函数执行空间：
- **__aicore__**: 执行空间限定符，声明一个在Device侧执行的函数, 该函数只能被__global__函数，或者其他__aicore__函数调用。
```c
// 定义一个在Device侧执行的函数
__aicore__ void bar() {}

// 定义核函数, 当前不通过__vector__、__cube__、__mix__等指定具体的执行核, 标识__global__ __aicore__告知编译框架自行推导
__global__ __aicore__ void foo() {
  bar();
}
```
- **__host__**: 执行空间限定符, 声明一个在Host侧执行的函数，__global__ 和__host__不能一起使用，__host__限定符是可选项，无函数执行空间限定符定义的函数，默认是host函数。
```c
// 不指定执行空间限定符, 默认定义Host侧执行函数
int foo() {}

// 指定__host__空间限定符, 定义Host侧执行函数
__host__ int bar() {
  foo();
}

// __global__ 和__host__不能一起使用
__global__ __host__ void kfunc() {}
```

**学习路径**：详见 [核函数](../核函数.md) [SIMD Builtin关键字](../../../语言扩展层/SIMD-BuiltIn关键字.md)

### 内存层级
AI Core采用三级存储架构，数据搬运遵循层级流转原则。
- **外部存储（Global Memory）**容量大但延迟高，存放算子输入输出数据，需通过MTE2/MTE3流水线搬运到片上存储后方可参与计算。
- **片上存储（Local Memory）**包括Unified Buffer（UB）、L1 Buffer、L0A/L0B/L0C Buffer：UB作为Vector算子的数据枢纽，数据需从Global Memory搬运至UB后才能参与矢量计算；L1 Buffer、L0A/L0B Buffer、L0C Buffer服务于矩阵计算，输入数据从GM搬运至L1 Buffer再分发至L0A/L0B Buffer供Cube单元计算，结果暂存于L0C Buffer。
- **寄存器（Ascend 950PR/950DT引入）**位于Vector计算单元最内层，具备最低访存延迟与最高带宽，开放寄存器可编程能力可充分释放硬件峰值性能，Vector计算单元仅访问寄存器，数据需从UB加载至寄存器后方可参与计算。

Ascend C指针编程通过地址限定符声明数组，直接映射物理内存空间：
- `__gm__`限定符声明Global Memory变量，开发者可直接操作该指针进行数据搬入搬出。
- `__ubuf__/__cbuf__/__ca__/__cb__`限定符分别声明UB/L1/L0A/L0B等片上存储空间。片上存储包括Unified Buffer、L1 Buffer、L0A/L0B/L0C Buffer，分别服务于矢量计算和矩阵计算。
- `vector_half`是矢量数据寄存器类型（Ascend 950PR/950DT引入），作为矢量计算的基本存储单元。寄存器变量位宽为VL（Vector Length），可存储VL/sizeof(T)个数据元素（T为数据类型）。Vector计算单元采用Load-Compute-Store模式与UB协作：从UB加载数据至寄存器、执行计算、将结果存回UB。

**学习路径**：详见[内存层级](./内存层级.md)

### 同步机制
AI Core采用多流水线异步并行架构。MTE2流水线负责数据搬入（GM→片上存储），MTE3流水线负责数据搬出（片上存储→GM），Vector流水线执行矢量计算，Cube流水线执行矩阵乘运算。流水线异步执行带来显著性能收益，但缺乏自动协调，需通过同步机制确保数据依赖关系满足。

Ascend C C++Tensor编程针对AI Core同步机制分别提供了核内同步和核间同接口。核内同步包括：Mutex互斥锁保护共享资源并发访问(Ascend 950PR/Ascend 950DT引入)，SetFlag/WaitFlag实现多流水线间同步（生产者发信号、消费者等待），PipeBarrier约束单流水线内执行顺序。核间同步采用计数器机制，通过CrossCoreSetFlag/WaitFlag协调多核执行进度。
- **Mutex（共享资源访问保护）**用于核内异步流水指令之间的同步处理，其功能类似于传统CPU中的锁机制。通过锁定指定流水线再释放流水线来完成流水线间的同步。
- **Notify/Wait（流水线间数据依赖同步）**采用**信号通知-阻塞等待**模式，通过流水线间的主动通知与被动等待实现执行顺序的严格约束。
- **数据屏障（内存访问顺序控制）**采用**阻塞等待**模式，保证前序指令中所有数据的读写工作全部完成，后序指令才能执行。用于在同一流水线内部约束执行顺序。
- **核间同步（多AI Core协作）**采用**计数器递增/递减**模式，通过flag_id对应的计数器协调多核执行进度。每个flag_id对应一个初始值为0的计数器，arrive指令递增计数器表示"当前核到达同步点"，wait指令检测计数器大于0则递减并继续执行、等于0则阻塞等待。
- **核间Block内同步**采用**ID映射机制**，通过映射后的sync_id实现Vector Core与Cube Core间的信号传递与等待。

**学习路径**：详见[同步机制](./同步机制.md)

### 数据搬运与计算
AI Core采用「标量控制单元+Vector矢量计算单元+Cube矩阵计算单元+分级本地存储（UB/L1/L0/Reg）」的异构架构，各组件分工明确、协同联动，硬件能力边界直接决定算子的数据搬运路径与计算形态。数据搬运与计算是AI Core算子开发的两大核心操作，共同构成算子执行的数据流转与计算处理闭环——数据搬运解决「数据从哪里来、到哪里去」的问题，计算解决「数据如何处理」的问题，二者协同配合完成算子功能实现。

Ascend C C指针编程针对AI Core的架构，开放芯片完备编程能力，支持以数组形式分配内存，一般基于指针编程。提供与业界一致的C语言编程体验。提供了基于指针操作的搬运和计算接口，搬运接口分为矢量搬运（GM↔UB）和矩阵搬运（GM→L1→L0A/L0B→L0C→GM）。计算接口分为矢量计算和矩阵计算两大类，矢量计算覆盖算术、比较、转换等场景，Ascend 950PR/950DT引入寄存器，矢量计算接口拆分为Memory矢量计算接口和Reg矢量计算接口，矩阵计算以Mmad为核心执行矩阵乘加运算。

**学习路径**：详见[数据搬运与计算](./数据搬运与计算.md)

### 基于C指针的算子Kernel通用开发流程

基于SIMD的AI Core算子开发一般包含四个主要步骤，适配绝大多数算子开发场景：
- **Tiling（分块）设计**: 对全局超大张量数据进行均匀切分，为各AI Core分配大小均衡的独立数据分片，精准适配SPMD多核并行架构，规避单核算力瓶颈，实现全域负载均衡。
```c
constexpr uint32_t C_API_TOTAL_LENGTH = 16384;
constexpr uint32_t C_API_TILE_NUM = 8;
uint32_t blockLength = C_API_TOTAL_LENGTH / asc_get_block_num();    // 单核内计算数据量
uint32_t tileLength = blockLength / C_API_TILE_NUM;                 // 单次循环计算数据量
```

- **数据搬入**: 调用SIMD编程接口，将全局内存中的待计算数据，批量搬运至L1 Buffer、UB等AI Core片内高速本地内存。
```c
asc_copy_gm2ub(x_local, x_gm + i * tileLength, tileLength);  // 搬入输入数据1
asc_copy_gm2ub(y_local, y_gm + i * tileLength, tileLength);  // 搬入输入数据2
```

- **数据计算**：根据算子类型调度Cube计算单元或Vector计算单元，依托片内高速缓存数据，完成矩阵、向量或混合逻辑计算；
```c
asc_add(z_local, x_local, y_local, tileLength);   // 向量加法
asc_sync_pipe(PIPE_V);                            // 计算完成后同步
asc_mul(z_local, z_local, y_local, tileLength);   // 向量乘法（可连续）
```

- **数据搬出**：计算完成后，将片内存储中的结果数据写回全局内存，完成单次算子计算流程。
```c
asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);  // V通知MTE3
asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);    // MTE3等待V
asc_copy_ub2gm(z_gm + i * tileLength, z_local, tileLength);  // 搬出结果数据
```
**学习路径**：详见[编程示例](./编程示例.md)

本章系统阐述了C指针编程的核心定位与技术体系。作为SIMD编程的底层接口，C指针编程直接映射NPU硬件指令，提供指令级透明可控能力，帮助开发者实现硬件峰值性能与精准问题定位。编程范式遵循SPMD模型的四步法，核心要点在于显式同步机制的正确应用。

后续章节将详细展开内存层级、同步机制、数据搬运与计算接口、编程示例等内容，帮助开发者掌握完整的实操能力。