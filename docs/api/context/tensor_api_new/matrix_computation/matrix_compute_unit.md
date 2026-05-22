# 矩阵计算单元

针对Ascend 950PR/Ascend 950DT产品，矩阵计算单元架构图如下：

**图1**矩阵计算单元架构图<a name="zh-cn_topic_0000002513373312_fig1082103513519"></a>
![](figures/矩阵计算单元架构图-26.png "矩阵计算单元架构图-26")

矩阵计算关联的存储单元为：

<a name="zh-cn_topic_0000002513373312_table187608336169"></a>
|L0A Buffer|AI Core内部物理存储单元，通常用于存储矩阵计算的左矩阵。|
|--------|--------|
|L0B Buffer|AI Core内部物理存储单元，通常用于存储矩阵计算的右矩阵。|
|L0C Buffer|AI Core内部物理存储单元，通常用于存储矩阵计算的结果，以及矩阵累加初始化值。|
|L1 Buffer|AI Core内部物理存储单元，空间相对较大，通常用于缓存矩阵计算的输入数据。 矩阵计算的输入一般需要从Global Memory搬运到L1 Buffer，然后分别搬运到L0A Buffer和L0B Buffer。|
|Fixpipe Buffer|AI Core内部物理存储单元，通常用于存储Fixpipe搬运过程中所需的量化参数等数据。|
|BiasTable Buffer|偏置存储，AI Core内部物理存储单元，通常用于存储矩阵计算所需的Bias（偏置）数据。|
