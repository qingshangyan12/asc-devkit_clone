# MTE2支持Pre-Allocation

## 概述

Ascend 960PR/Ascend 960DT的带宽较大，单核算力不足，当访存请求发送量不足，导致outstanding（未完成请求并行上限）不能充满时，会出现请求间隙，最终算子不能打满带宽。  
为解决这个问题，芯片支持在与前序指令存在数据依赖的情况下，指令仍然可以提前发出，硬件可以保证数据的返回仍然维持依赖关系，这个功能通过对指令配置Pre-Allocation使能。

## 原理分析

Ascend 960PR/Ascend 960DT中HBM带宽增大，同时增加了相应数量的outstanding，由于存在总线乱序返回等原因会存在一定的lantency（延迟），导致在double buffer的场景出现bubble（间隙）。
如[图一](#fig_pre_allocation_1)所示。

**图1**  double buffer场景出现bubble<a id="fig_pre_allocation_1"></a>  
![](../../figures/pre-allocation1.png)

使能PreAllocation：为解决上述场景问题，芯片支持MTE2相关指令，可以提前发出从GM读取数据到总线上，等到同步关系解除时，再从总线上返回到LocalMem，用于减少总线的lantency，如[图二](#fig_pre_allocation_2)所示。

**图2**  pre-allocation消除bubble<a id="fig_pre_allocation_2"></a>  
![](../../figures/pre-allocation2.png)

### 完整示例

一个完整的Pre-Allocation优化MTE2 bubble的示例可参考 [data_copy_pre_allocation](../../../../../examples/01_simd_cpp_api/05_best_practices/04_memory_access/data_copy_pre_allocation/)。该样例在double buffer的DataCopy搬运场景下，演示MTE2 Pre-Allocation特性。

### 注意事项
- 该特性仅支持Ascend 960PR/Ascend 960DT，且仅对MTE2搬运有效。
- 如果该搬运指令读取的数据是前一条指令的输出，则应设置同步依赖关系，并且**禁用**Pre-Allocation功能。
- 该特性适用于存在MTE2前置屏障（如MTE3→MTE2、V→MTE2等）且该屏障会阻塞MTE2发出GM读请求的场景；若MTE2无前置屏障约束，使能该特性不会带来额外收益。
