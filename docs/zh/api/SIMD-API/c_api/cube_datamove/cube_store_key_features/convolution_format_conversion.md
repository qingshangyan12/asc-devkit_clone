# 卷积格式转换

在卷积运算中，L0C Buffer上存放的数据格式为NC1HWC0格式，Fixpipe指令通过[NZ2ND](NZ2ND.md)能力，支持将L0C Buffer上数据转为NHWC格式输出。转换示意图如下所示：

**图1** NC1HWC0转换为NHWC格式<a name="zh-cn_topic_0000002515830464_fig63620361414"></a>  

![](../../../../figures/Fixpipe_NC1HWC0_NHWC.png)

<!-- npu="950" id1 -->
特别的，针对Ascend 950PR/Ascend 950DT产品，还支持Fixpipe指令通过[NZ2DN](NZ2DN.md)能力，将L0C Buffer上数据转为NCHW格式输出。转换示意图如下所示：

**图2**  NC1HWC0转换为NCHW格式<a name="fig188671630134414"></a>  

![](../../../../figures/Fixpipe_NC1HWC0_NCHW.png)
<!-- end id1 -->

## C API维度映射

将卷积结果视作矩阵`[M, N]`，其中`M=H×W`（一个批次/分片的空间位置数）、`N=C`（输出通道数），L0C小分形固定为16×16。

- NHWC输出：开启`enable_nz2nd`，不带额外行Padding时`dst_stride=C`，每个空间位置的C个通道连续。
- 3510 NCHW输出：开启`enable_nz2dn`，不带额外行Padding时`dst_stride=H×W`，每个通道的空间位置连续，并调用`asc_set_l0c_copy_channel_para(1)`。
- 多批次使用对应架构的批次配置接口。源批次步长由L0C的实际分形分配推导，目标批次步长通常为`H×W×C`个元素；含Padding或分片时按真实跨度计算。

量化系数和Vector激活系数按源矩阵的N列（输出通道）索引；转为DN之后，不能改为按输出空间位置索引。对于L1/UB目标，维度与步长还须满足片上搬出对齐约束。
