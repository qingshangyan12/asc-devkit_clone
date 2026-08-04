# 随路量化与随路ReLU场景组合

矩阵搬出可在部分量化模式下组合Normal ReLU、Scalar ReLU、Vector ReLU或Clip ReLU。组合能力由目标架构的[asc_copy_l0c2gm](../../asc_copy_l0c2gm/asc_copy_l0c2gm.md)重载决定。

## 配置原则

- Scalar量化和Scalar ReLU所需参数由搬出接口的显式配置参数提供。
- Vector量化或Vector ReLU使用Fixpipe Buffer中的参数，相关地址通过[asc_set_l0c2gm_config](../../../cube_compute/asc_set_l0c2gm_config.md)配置。
- Clip ReLU需要与Normal ReLU及目标架构要求的量化模式组合，不能单独启用。
- 量化模式、ReLU模式和输出数据类型必须构成单接口文档明确支持的组合。

2201与3510架构的量化模式集合、参数位域和ReLU组合不同。概览不复制硬件位域表，避免将一个架构的编码用于另一架构。完整枚举、参数编码和调用示例以对应架构的`asc_copy_l0c2gm`文档为准。
