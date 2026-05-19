# Reg Vector Compute Practices样例介绍

## 概述

基于VF函数的性能优化样例，通过<<<>>>直调的实现方式，介绍了VF融合优化、Loop拆分优化、循环展开优化等方法。

## 样例列表
|  目录名称  |  功能描述  | 支持的产品 |
| -------------------------------------------------- | ---------------------------------------------------- | --- |
| [gelu_eltwise_high_performance](./gelu_eltwise_high_performance) | 本样例以Gelu+Element-wise计算为例，介绍RegBase的向量性能调优方法，样例展示并行度调整、Loop拆分和循环展开之后的性能收益情况。 | Ascend 950PR/Ascend 950DT |
| [gelu_high_performance](./gelu_high_performance) | 本样例以Gelu计算为例，介绍RegBase的向量性能调优方法，样例展示使能VF融合和循环展开之后的性能收益情况。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_continious_align](./optimize_vf_continious_align) | 本样例演示了SIMD场景下，基于连续非对齐搬运接口LoadUnAlign/StoreUnAlign进行搬运优化的算子实现。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_dual_instr](./optimize_vf_dual_instr) | 本样例演示了SIMD场景下，基于Reg编程接口下VF指令双发优化的样例，通过合理拆分VF循环，适当搬出中间结果到UB，减少数据依赖。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_fusion](./optimize_vf_fusion) | 本样例演示了SIMD场景下，基于Reg编程接口，通过VF融合优化算子代码实现。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_loop](./optimize_vf_loop) | 通过循环内成员变量访问优化、循环内指令分布优化、循环内地址管理优化等手段优化VF循环。 | Ascend 950PR/Ascend 950DT |
