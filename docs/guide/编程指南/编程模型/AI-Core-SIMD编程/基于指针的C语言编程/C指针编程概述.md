# C指针编程概述

Ascend C为AI Core提供了SIMD编程路径，**C指针编程**是该路径的底层核心接口，直接映射NPU硬件指令，提供对向量处理单元、矩阵运算单元、本地存储等硬件资源的精细控制能力。对于追求极致性能、习惯C语言编程的开发者，C指针编程是解锁NPU硬件潜能的关键技术路径。本章旨在帮助开发者建立对C指针编程的系统化认知，理解其底层技术定位、核心编程范式与开发方法论。全文遵循「背景动因—核心概念—编程范式—落地实践」的递进逻辑展开，完整覆盖理论根基与实操要义，帮助开发者快速掌握底层开发能力。

## 背景动因：C指针编程为何不可或缺？

早期算子开发以C语言裸指针编程为核心。依托C语言底层内存操控能力，开发者可直接通过指针寻址操作设备内存，并使用与硬件底层指令精准映射的原生接口，支持向量计算、矩阵乘、数据搬运等硬件原语调用。该模式可实现硬件缓存、寄存器等资源的精细化管理，是底层极致性能优化的核心方案，广泛应用于离散类、归约类、矩阵类等核心算子的底层实现。C指针编程精准适配C开发者的编程习惯，实现对底层硬件的精细化控制。

### C指针编程的核心特征与本质定义

- **C语言函数封装**，接口清晰简洁，符合C编程习惯。
- 使用数组分配片上内存，显式插入同步语句，**精细化控制硬件资源**。
- 函数与硬件指令一一对应，调用即执行。接口设计简洁轻量，无冗余封装层，**直接映射硬件能力**。

## 编程范式: 面向C指针的SPMD编程范式

### Kernel函数

核函数（Kernel Function）是Ascend C算子设备侧实现的入口。Ascend C允许用户使用C/C++函数的语法扩展来编写设备端的运行代码，用户在核函数中进行数据访问和计算操作，由此实现该算子的所有功能。区别于普通的C++函数调用时仅执行一次，当核函数被调用时，多个核都执行相同的核函数代码，具有相同的函数入参，并行执行。

核函数定义时需要使用函数类型限定符__global__和__aicore__；其指针入参变量需要增加变量类型限定符__gm__，表明该指针变量指向Global Memory上某处内存地址；使用<<<...>>>内核调用符调用执行核函数，并指定调用时的执行核数。
```c
// 实现核函数
__vector__ __global__ __aicore__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint32_t blockLength = TILE_LENGTH * NUM_BLOCKS / asc_get_block_num();
}

// 调用核函数
void add_custom_do(uint32_t numBlocks, void* l2ctrl, void* stream, uint8_t* x, uint8_t* y, uint8_t* z)
{
    add_custom<<<numBlocks, l2ctrl, stream>>>(x, y, z);
}
```
**详细方法**：参见 [核函数](../核函数.md)

### 算子Kernel通过开发
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
**详细方法**：参见 [数据搬运与计算](./数据搬运与计算.md)、[同步机制](./同步机制.md)

- **数据计算**：根据算子类型调度Cube计算单元或Vector计算单元，依托片内高速缓存数据，完成矩阵、向量或混合逻辑计算；
```c
asc_add(z_local, x_local, y_local, tileLength);   // 向量加法
asc_sync_pipe(PIPE_V);                            // 计算完成后同步
asc_mul(z_local, z_local, y_local, tileLength);   // 向量乘法（可连续）
```
**详细方法**：参见 [数据搬运与计算](./数据搬运与计算.md)

- **数据搬出**：计算完成后，将片内存储中的结果数据写回全局内存，完成单次算子计算流程。
```c
asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);  // V通知MTE3
asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);    // MTE3等待V
asc_copy_ub2gm(z_gm + i * tileLength, z_local, tileLength);  // 搬出结果数据
```
**详细方法**：参见 [数据搬运与计算](./数据搬运与计算.md)

本章系统阐述了C指针编程的核心定位与技术体系。作为SIMD编程的底层接口，C指针编程直接映射NPU硬件指令，提供指令级透明可控能力，帮助开发者实现硬件峰值性能与精准问题定位。编程范式遵循SPMD模型的四步法，核心要点在于显式同步机制的正确应用。

后续章节将详细展开内存层级、同步机制、数据搬运与计算接口、编程示例等内容，帮助开发者掌握完整的实操能力。