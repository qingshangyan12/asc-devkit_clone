# L1 Buffer与Fixpipe Buffer数据搬运

[asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb.md)用于将随路量化、激活等Fixpipe操作所需的参数从L1 Buffer搬运到Fixpipe Buffer。

接口在2201和3510架构上提供不同重载。调用时应核对对应架构文档中的`dst`、`src`、搬运长度、分块数量和间隔参数。

Fixpipe Buffer的内存排布、地址对齐和参数数据类型必须与后续`asc_copy_l0c2gm`或`asc_copy_l0c2l1`的配置一致。
