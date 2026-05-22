# Tensor API
Tensor API文档目录，整体使用时可以引入tensor_api/tensor.h，Tensor API列表如下：

## 数据结构

|结构名|说明|
|--------|--------|
|Shape|张量各维度大小描述。|
|Stride|张量各维度步长描述。|
|Tile|张量切分或分块描述。|
|Coord|张量坐标描述。|
|Layout|张量布局对象，包含形状、步长和布局属性。|
|Tensor|张量对象，由内存指针和布局组成。|
|CopyAtom|数据搬运原子对象。|
|MmadAtom|矩阵乘加原子对象。|
|LayoutTraitDefault|默认布局属性配置。|
|CopyTraits|数据搬运特征配置，包含Operation和Trait。|
|MmadTraits|矩阵计算特征配置，包含Operation和Trait。|
|[CopyL0C2GMTrait](data_movement/l0c_to_gm_data_movement.md)|L0C到GM搬运的编译期特征配置。|
|[CopyL0C2UBTrait](data_movement/l0c_to_ub_data_movement.md)|L0C到UB搬运的编译期特征配置。|
|[FixpipeParams](data_movement/l0c_to_gm_data_movement.md)|L0C搬出时的运行时搬运参数。|
|[MmadParams](matrix_computation/mmad计算.md)|矩阵乘加计算的运行时参数。|

## 基础构造

基础构造类API，用于构造Tensor API中的形状、步长、坐标、布局、内存指针和张量对象，此类API列表如下：

|API名称|说明|
|--------|--------|
|MakeShape|构造张量各维度大小描述。|
|MakeStride|构造张量各维度步长描述。|
|MakeTile|构造切分或分块信息。|
|MakeCoord|构造张量坐标。|
|MakeLayout|构造通用布局对象。|
|Crd2Idx|根据坐标、形状和步长计算线性索引。|
|Get|获取Tuple或Layout中的指定维度信息。|
|Select|从Layout中选择指定维度。|
|Size|获取Layout指定维度的大小。|
|Capacity|获取Layout指定维度的容量。|
|Cosize|获取Layout覆盖的元素范围大小。|
|Rank|获取Layout维度数。|
|Slice|从Tensor中按坐标和范围切分子Tensor。|
|MakeFrameLayout|构造GM、L1、L0A、L0B、L0C、UB等存储层级使用的数据布局。|
|MakeMemPtr|构造Tensor API内存指针适配器。|
|MakeTensor|将内存指针和布局组合为Tensor对象。|

## 数据搬运

数据搬运类API，用于在GM、L1、L0A、L0B、L0ScaleA、L0ScaleB、BiasTable Buffer、Fixpipe Buffer、L0C、UB等存储层级之间搬运数据，此类API列表如下：

|API名称|说明|
|--------|--------|
|[MakeCopy](data_movement/data_movement_overview.md)|构造数据搬运原子对象。|
|[Copy](data_movement/data_movement_overview.md)|执行数据搬运。|
|[CopyGM2L1](data_movement/gm_to_l1_data_movement.md)|将数据从Global Memory搬运到L1 Buffer。|
|[CopyL12L0A](data_movement/l1_to_l0a_data_movement.md)|将左矩阵数据从L1 Buffer搬运到L0A Buffer。|
|[CopyL12L0B](data_movement/l1_to_l0b_data_movement.md)|将右矩阵数据从L1 Buffer搬运到L0B Buffer。|
|[CopyL12L0ScaleA](data_movement/l1_to_l0scalea_data_movement.md)|将scaleA数据从L1 Buffer搬运到L0ScaleA Buffer。|
|[CopyL12L0ScaleB](data_movement/l1_to_l0scaleb_data_movement.md)|将scaleB数据从L1 Buffer搬运到L0ScaleB Buffer。|
|[CopyL12BT](data_movement/l1_to_bias_table_buffer_data_movement.md)|将Bias数据从L1 Buffer搬运到BiasTable Buffer。|
|[CopyL12FB](data_movement/l1_to_fixpipe_buffer_data_movement.md)|将量化参数等数据从L1 Buffer搬运到Fixpipe Buffer。|
|[CopyL12UB](data_movement/l1_to_ub_data_movement.md)|将数据从L1 Buffer搬运到UB。|
|[CopyL0C2GM](data_movement/l0c_to_gm_data_movement.md)|将矩阵计算结果从L0C Buffer搬运到Global Memory。|
|[CopyL0C2UB](data_movement/l0c_to_ub_data_movement.md)|将矩阵计算结果从L0C Buffer搬运到UB。|

## 矩阵计算

矩阵计算类API，用于执行普通矩阵乘加和MX矩阵乘加，此类API列表如下：

|API名称|说明|
|--------|--------|
|[MakeMmad](matrix_computation/mmad计算.md)|构造矩阵乘加原子对象。|
|[Mmad](matrix_computation/mmad计算.md)|执行矩阵乘加计算。|
|[MmadOperation](matrix_computation/mmad计算.md)|矩阵乘加操作。|
