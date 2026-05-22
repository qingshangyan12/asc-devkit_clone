# 矩阵计算的搬入总体说明

矩阵计算的搬入是AscendC编程框架中用于数据搬运的核心接口系列，主要实现Global Memory到L1、L1到L0A/L0B之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左矩阵、右矩阵、MX缩放系数以及辅助数据的搬运需求，通常与Mmad接口配合使用。

矩阵计算的搬入接口系列支持多种数据搬运场景，包括**二维格式数据搬运、矩阵转置搬运、ND/DN/NZ/ZN布局转换、MX Scale数据搬运以及BiasTable Buffer/Fixpipe Buffer/UB辅助数据搬运**等。通过灵活配置不同的张量布局、存储位置和搬运原子对象，开发者可以精确控制数据的搬运路径、起始位置、搬运长度等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。

矩阵计算的搬入接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

针对Ascend 950PR/Ascend 950DT：

**表1****数据通路和存储层级**

<a name="tensor_api_matrix_data_in_overview_table_data_path"></a>
|源位置|源地址对齐要求|目的位置|目的地址对齐要求|典型应用场景|
|--------|--------|--------|--------|--------|
|Global Memory|1字节|L1(A1)|32字节|从全局内存加载左矩阵数据到L1|
|Global Memory|1字节|L1(B1)|32字节|从全局内存加载右矩阵数据到L1|
|L1(A1)|32字节|L0A(A2)|512字节|从L1加载左矩阵数据到L0A|
|L1(B1)|32字节|L0B(B2)|512字节|从L1加载右矩阵数据到L0B|

**表2****接口分类与使用场景**

<a name="tensor_api_matrix_data_in_overview_table_interface_category"></a>
|接口名称|主要功能|数据通路|典型应用场景|
|--------|--------|--------|--------|
|[CopyGM2L1](gm_to_l1_data_movement.md)|Global Memory到L1的数据搬运|GM到L1|左矩阵/右矩阵搬入L1，支持ND、DN、NZ、ZN以及MX Scale相关布局搬运|
|[CopyL12L0A](l1_to_l0a_data_movement.md)|L1到L0A的数据搬运|L1到L0A|左矩阵搬入L0A，支持普通搬运和转置搬运|
|[CopyL12L0B](l1_to_l0b_data_movement.md)|L1到L0B的数据搬运|L1到L0B|右矩阵搬入L0B，支持普通搬运和转置搬运|
|[CopyL12L0ScaleA](l1_to_l0scalea_data_movement.md)|L1到L0ScaleA的数据搬运|L1到L0ScaleA|MX矩阵计算中左矩阵缩放系数搬入|
|[CopyL12L0ScaleB](l1_to_l0scaleb_data_movement.md)|L1到L0ScaleB的数据搬运|L1到L0ScaleB|MX矩阵计算中右矩阵缩放系数搬入|

**表3****辅助数据搬运接口说明**

<a name="tensor_api_matrix_data_in_overview_table_auxiliary_interface"></a>
|接口名称|功能说明|配合使用的场景|
|--------|--------|--------|
|[CopyL12BT](l1_to_bias_table_buffer_data_movement.md)|将L1中的偏置数据搬运到BiasTable Buffer|矩阵计算或搬出阶段需要偏置表数据的场景|
|[CopyL12FB](l1_to_fixpipe_buffer_data_movement.md)|将L1中的辅助控制数据搬运到Fixpipe Buffer|Fixpipe相关搬出配置需要辅助数据的场景|
|[CopyL12UB](l1_to_ub_data_movement.md)|将L1中的数据搬运到UB|L1与UB之间需要中间数据交换的场景|

**表4****Ascend 950PR/Ascend 950DT产品接口支持情况**

<a name="tensor_api_matrix_data_in_overview_table_product_support"></a>
|产品系列|CopyGM2L1|CopyL12L0A|CopyL12L0B|CopyL12L0ScaleA|CopyL12L0ScaleB|
|--------|--------|--------|--------|--------|--------|
|Ascend 950PR/Ascend 950DT|√|√|√|√|√|
