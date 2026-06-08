# SetNextTaskStart<a name="ZH-CN_TOPIC_0000002337763421"></a>

> [!NOTE]说明 
>本接口为试验接口，在后续版本中可能会调整或改进，不保证后续兼容性。请开发者在使用过程中关注后续版本更新。

## 产品支持情况<a name="section17196114513104"></a>

| 产品 | 是否支持 | 备注 |
| --- | --- | --- |
| Ascend 950PR/Ascend 950DT | √ | 该接口生效 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ | 该接口生效 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ | 仅保证编译兼容，实际功能不生效。 |
| Atlas 200I/500 A2 推理产品 | √ | 仅保证编译兼容，实际功能不生效。 |
| Atlas 推理系列产品AI Core | √ | 仅保证编译兼容，实际功能不生效。 |
| Atlas 推理系列产品Vector Core | √ | 仅保证编译兼容，实际功能不生效。 |
| Atlas 训练系列产品 | √ | 仅保证编译兼容，实际功能不生效。 |
| Kirin X90 | √ | 仅保证编译兼容，实际功能不生效。 |
| Kirin 9030 | √ | 仅保证编译兼容，实际功能不生效。 |

## 功能说明<a name="section618mcpsimp"></a>

在[SuperKernel](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97/%E9%AB%98%E7%BA%A7%E7%BC%96%E7%A8%8B/%E9%AB%98%E7%BA%A7%E7%89%B9%E6%80%A7/%E7%AE%97%E5%AD%90%E5%85%A5%E5%9B%BE%E5%BC%80%E5%8F%91/SuperKernel%E5%BC%80%E5%8F%91.md)的子Kernel中调用，调用后的指令可以和后续其他的子Kernel实现并行，提升整体性能。如[图1](#fig37581010773)所示，SuperKernel按序调用子Kernel，为保证子Kernel之间数据互不干扰，会在子Kernel间插入算子间同步进行保序，子Kernel<sub>N-1</sub>调用该接口后，之后的指令会和后续子Kernel<sub>N</sub>实现并行。

SuperKernel是一种算子的二进制融合技术，与源码融合不同，它聚焦于内核函数 \(Kernel\) 的二进制的调度方案，展开深度优化，于已编译的二进制代码基础上融合创建一个超级Kernel函数（SuperKernel），以调用子函数的方式调用多个其他内核函数，也就是子Kernel。相对于单算子下发，SuperKernel技术可以减少任务调度等待时间和调度开销，同时利用Task间隙资源进一步优化算子头开销。

**开发者需要自行保证调用此接口后的指令不会与后序算子互相干扰而导致精度问题，推荐在整个算子最后一条搬运指令后调用此接口。**

**图 1**  通过SetNextTaskStart实现并行示意图<a name="fig37581010773"></a>  
![](../../../../figures/通过SetNextTaskStart实现并行示意图.png "通过SetNextTaskStart实现并行示意图")

## 函数原型<a name="section620mcpsimp"></a>

-   该原型在如下产品型号支持：

    Ascend 950PR/Ascend 950DT

    Atlas A3 训练系列产品/Atlas A3 推理系列产品

    Atlas A2 训练系列产品/Atlas A2 推理系列产品

    Atlas 200I/500 A2 推理产品

    ```
    template<pipe_t AIV_PIPE = PIPE_MTE3, pipe_t AIC_PIPE = PIPE_FIX>
    __aicore__ inline void SetNextTaskStart()
    ```

-   该原型在如下产品型号支持：

    Atlas 推理系列产品AI Core

    Atlas 训练系列产品

    ```
    template<pipe_t AIV_PIPE = PIPE_MTE3, pipe_t AIC_PIPE = PIPE_MTE3>
    __aicore__ inline void SetNextTaskStart()
    ```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| AIV_PIPE | SetNextTaskStart之后运行的指令，如果位于AIV上的AIV_PIPE流水，可以与后序算子并行。AIV_PIPE的取值范围为PIPE_MTE2、PIPE_MTE3、PIPE_S、PIPE_V，流水类型介绍可参考<a href="../核内同步/核内同步能力概述.md#section1272612276459">硬件流水类型</a>。 |
| AIC_PIPE | SetNextTaskStart之后运行的指令，如果位于AIC上的AIC_PIPE流水，可以与后序算子并行。AIC_PIPE的取值范围为PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX、PIPE_M，流水类型介绍可参考<a href="../核内同步/核内同步能力概述.md#section1272612276459">硬件流水类型</a>。 |

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   该接口适用于TorchAir图模式开发场景，且需在启用SuperKernel特性后方可生效。相关信息可参考[《PyTorch图模式使用指南\(TorchAir\)》中的“图内标定SuperKernel范围”](https://www.hiascend.com/document/detail/zh/Pytorch/2600/modthirdparty/torchairuseguide/docs/zh/ascend_ir/features/advanced/super_kernel_scope.md)章节。
-   在算子运行过程中，需要保证此接口在每个核上都被调用，且每个核上仅被调用一次。
-   若子Kernel某个TilingKey分支调用了此接口，则开发者需要保证当前算子可能会运行的所有TilingKey均调用了此接口，否则会出现因同步指令数量不匹配而卡住的现象。

## 调用示例<a name="section837496171220"></a>

详细示例请参考[TaskSync样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/06_sync_control/task_sync)。

```
#include "kernel_operator.h"

AscendC::LocalTensor<half> dstLocal = outQueueDst.DeQue<half>();
AscendC::DataCopy(dstGlobal, dstLocal, 512);
// 在最后一条搬运指令后调用，使之后的指令可与后续子kernel并行执行
AscendC::SetNextTaskStart();
outQueueDst.FreeTensor(dstLocal);
```
