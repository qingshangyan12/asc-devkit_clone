# 矩阵计算的搬入总体说明

矩阵计算的搬入是AscendC编程框架中用于数据搬运的核心接口系列，主要实现Global Memory到L1 Buffer、L1 Buffer到L0A Buffer/L0B Buffer之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左矩阵、右矩阵、MX缩放系数以及辅助数据的搬运需求，通常与Mmad接口配合使用。

矩阵计算的搬入接口系列支持多种数据搬运场景，包括**二维格式数据搬运、矩阵转置搬运、ND/DN/NZ/ZN布局转换、MX Scale数据搬运以及BiasTable Buffer/Fixpipe Buffer/Unified Buffer辅助数据搬运**等。通过灵活配置不同的张量布局、存储位置和搬运原子对象，开发者可以精确控制数据的搬运路径、起始位置、搬运长度等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。

矩阵计算的搬入接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

针对Ascend 950PR/Ascend 950DT：

**表1** **数据通路和存储层级**

<a name="tensor_api_matrix_data_in_overview_table_data_path"></a>

|源位置|源地址对齐要求|源数据布局|目的位置|目的地址对齐要求|目的数据布局|格式转换|典型应用场景|
|--------|--------|--------|--------|--------|--------|--------|--------|
|Global Memory|1字节|`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`|L1 Buffer(A1)|32字节|`NDExtLayoutPtn`、`NZLayoutPtn`|ND2ND、ND2NZ、DN2NZ、NZ2NZ|从全局内存加载左矩阵数据到L1 Buffer|
|Global Memory|1字节|`NDExtLayoutPtn`、`DNExtLayoutPtn`、`ZNLayoutPtn`|L1 Buffer(B1)|32字节|`NDExtLayoutPtn`、`ZNLayoutPtn`|ND2ND、ND2ZN、DN2ZN、ZN2ZN|从全局内存加载右矩阵数据到L1 Buffer|
|Global Memory|1字节|`ScaleANDLayoutPtn`、`ScaleADNLayoutPtn`、`ZZLayoutPtn`|L1 Buffer(ScaleA)|32字节|`ZZLayoutPtn`|ScaleA ND2ZZ、ScaleA DN2ZZ、ZZ2ZZ|从全局内存加载左矩阵缩放系数到L1 Buffer|
|Global Memory|1字节|`ScaleBNDLayoutPtn`、`ScaleBDNLayoutPtn`、`NNLayoutPtn`|L1 Buffer(ScaleB)|32字节|`NNLayoutPtn`|ScaleB ND2NN、ScaleB DN2NN、NN2NN|从全局内存加载右矩阵缩放系数到L1 Buffer|
|L1 Buffer(A1)|32字节|`NZLayoutPtn`、`ZNLayoutPtn`|L0A Buffer(A2)|512字节|`NZLayoutPtn`|NZ2NZ、ZN2NZ|从L1 Buffer加载左矩阵数据到L0A Buffer|
|L1 Buffer(B1)|32字节|`ZNLayoutPtn`、`NZLayoutPtn`|L0B Buffer(B2)|512字节|`ZNLayoutPtn`|ZN2ZN、NZ2ZN|从L1 Buffer加载右矩阵数据到L0B Buffer|
|L1 Buffer(ScaleA)|32字节|`ZZLayoutPtn`|L0ScaleA Buffer|512字节|`ZZLayoutPtn`|ZZ2ZZ|从L1 Buffer加载左矩阵缩放系数到L0ScaleA Buffer|
|L1 Buffer(ScaleB)|32字节|`NNLayoutPtn`|L0ScaleB Buffer|512字节|`NNLayoutPtn`|NN2NN|从L1 Buffer加载右矩阵缩放系数到L0ScaleB Buffer|

**表2** **接口分类与使用场景**

<a name="tensor_api_matrix_data_in_overview_table_interface_category"></a>

|接口名称|主要功能|数据通路|源数据布局|目的数据布局|格式转换|典型应用场景|
|--------|--------|--------|--------|--------|--------|--------|
|[CopyGM2L1](<data_movement/gm_to_l1.md>)|Global Memory到L1 Buffer的数据搬运|Global Memory到L1 Buffer|`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn`、`ScaleANDLayoutPtn`、`ScaleADNLayoutPtn`、`ScaleBNDLayoutPtn`、`ScaleBDNLayoutPtn`、`ZZLayoutPtn`、`NNLayoutPtn`|`NDExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn`、`ZZLayoutPtn`、`NNLayoutPtn`|ND2ND、ND/DN2NZ、ND/DN2ZN、NZ2NZ、ZN2ZN、ScaleA ND/DN2ZZ、ScaleB ND/DN2NN、ZZ2ZZ、NN2NN|左矩阵/右矩阵搬入L1 Buffer，支持ND、DN、NZ、ZN以及MX Scale相关布局搬运|
|[CopyL12L0A](l1_to_l0a.md)|L1 Buffer到L0A Buffer的数据搬运|L1 Buffer到L0A Buffer|`NZLayoutPtn`、`ZNLayoutPtn`|`NZLayoutPtn`|NZ2NZ、ZN2NZ|左矩阵搬入L0A Buffer，支持普通搬运和转置搬运|
|[CopyL12L0B](l1_to_l0b.md)|L1 Buffer到L0B Buffer的数据搬运|L1 Buffer到L0B Buffer|`ZNLayoutPtn`、`NZLayoutPtn`|`ZNLayoutPtn`|ZN2ZN、NZ2ZN|右矩阵搬入L0B Buffer，支持普通搬运和转置搬运|
|[CopyL12L0ScaleA](l1_to_l0scalea.md)|L1 Buffer到L0ScaleA Buffer的数据搬运|L1 Buffer到L0ScaleA Buffer|`ZZLayoutPtn`|`ZZLayoutPtn`|ZZ2ZZ|MX矩阵计算中左矩阵缩放系数搬入|
|[CopyL12L0ScaleB](l1_to_l0scaleb.md)|L1 Buffer到L0ScaleB Buffer的数据搬运|L1 Buffer到L0ScaleB Buffer|`NNLayoutPtn`|`NNLayoutPtn`|NN2NN|MX矩阵计算中右矩阵缩放系数搬入|
|[CopyL12BT](l1_to_bias_table.md)|将L1 Buffer中的偏置数据搬运到BiasTable Buffer|L1 Buffer到BiasTable Buffer|`NDExtLayoutPtn`|`NDExtLayoutPtn`|ND2ND|矩阵计算或搬出阶段需要偏置表数据的场景|
|[CopyL12FB](l1_to_fixpipe.md)|将L1 Buffer中的辅助控制数据搬运到Fixpipe Buffer|L1 Buffer到Fixpipe Buffer|`NDExtLayoutPtn`|`NDExtLayoutPtn`|ND2ND|Fixpipe相关搬出配置需要辅助数据的场景|
|[CopyL12UB](l1_to_ub.md)|将L1 Buffer中的数据搬运到Unified Buffer|L1 Buffer到Unified Buffer|`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`|与源数据布局一致|ND2ND、DN2DN、NZ2NZ|L1 Buffer与Unified Buffer之间需要中间数据交换的场景|

**表3** **Ascend 950PR/Ascend 950DT产品接口支持情况**

<a name="tensor_api_matrix_data_in_overview_table_product_support"></a>

|产品系列|CopyGM2L1|CopyL12L0A|CopyL12L0B|CopyL12L0ScaleA|CopyL12L0ScaleB|
|--------|--------|--------|--------|--------|--------|
|Ascend 950PR/Ascend 950DT|√|√|√|√|√|

## API映射关系

`Copy`接口在`impl/tensor_api/arch/cube`路径下的`instruction.h`中完成到底层`asc_`函数的封装映射，映射关系如下：

|接口名称|对应`instruction.h`路径|底层`asc_`函数|
|--------|--------|--------|
|[CopyGM2L1](<data_movement/gm_to_l1.md>)|`impl/tensor_api/arch/cube/gm_to_l1/npu_arch_3510/instruction.h`|`asc_copy_gm2l1_align`、`asc_copy_gm2l1_nd2nz`、`asc_copy_gm2l1_dn2nz`|
|[CopyL12L0A](l1_to_l0a.md)|`impl/tensor_api/arch/cube/l1_to_l0a/npu_arch_3510/instruction.h`|`asc_copy_l12l0a`、`asc_copy_l12l0a_transpose`|
|[CopyL12L0B](l1_to_l0b.md)|`impl/tensor_api/arch/cube/l1_to_l0b/npu_arch_3510/instruction.h`|`asc_copy_l12l0b`、`asc_copy_l12l0b_transpose`|
|[CopyL12L0ScaleA](l1_to_l0scalea.md)|`impl/tensor_api/arch/cube/l1_to_l0scalea/npu_arch_3510/instruction.h`|`asc_copy_l12l0a_mx`|
|[CopyL12L0ScaleB](l1_to_l0scaleb.md)|`impl/tensor_api/arch/cube/l1_to_l0scaleb/npu_arch_3510/instruction.h`|`asc_copy_l12l0b_mx`|
|[CopyL12BT](l1_to_bias_table.md)|`impl/tensor_api/arch/cube/l1_to_bt/npu_arch_3510/instruction.h`|`asc_copy_l12bt`|
|[CopyL12FB](l1_to_fixpipe.md)|`impl/tensor_api/arch/cube/l1_to_fb/npu_arch_3510/instruction.h`|`asc_copy_l12fb`|
|[CopyL12UB](l1_to_ub.md)|`impl/tensor_api/arch/cube/l1_to_ub/npu_arch_3510/instruction.h`|`asc_copy_l12ub`|
