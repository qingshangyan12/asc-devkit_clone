# L1 Buffer与BiasTable Buffer数据搬运

[asc_copy_l12bt](../asc_copy_l12bt/asc_copy_l12bt.md)用于将矩阵计算所需的Bias数据从L1 Buffer搬运到BiasTable Buffer。

接口在2201和3510架构上提供不同重载。调用时应核对对应架构文档中的`dst`、`src`、搬运长度、分块数量和间隔参数。

BiasTable Buffer的容量、地址对齐、支持数据类型以及与`asc_mmad`的同步关系以单接口文档为准。
