# overview

矢量计算通过Vector计算单元完成，矢量计算的源操作数和目的操作数均通过Unified Buffer（UB）来进行存储。

memory_vector_compute提供[continuous_compute](continuous_compute.md#continuous_compute)和[high_dim_split](high_dim_split.md#high_dim_split)两种计算模式，并可通过[mask](mask.md#mask)实现数据选择性参与计算。

以Add接口为例，两种计算模式的调用方式示意图如下。其中，continuous_compute简单直观，适合一维Tensor连续数据处理；high_dim_split计算灵活可控，支持迭代执行和地址间隔配置。开发者可根据实际业务需求选择合适的计算方式。

![](../../../../figures/multi_level_vector_api.png)
