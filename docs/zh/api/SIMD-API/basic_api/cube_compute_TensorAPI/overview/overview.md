# overview

## cube_compute_unit
 	 
cube_compute_unit的详细描述请参考[LINK](../../cube_compute_ISASI/overview/cube_compute_unit.md)。

## cube_compute_flow

Tensor API针对cube_computeprogramming_model提供了两类接口，分别承载Cube核中各个通路的搬运能力和计算能力，如下图所示：

**图1**  普通矩阵基础计算流程图<a name="zh-cn_topic_0000002535567224_fig135639216483"></a>

![普通矩阵基础计算流程图](../../../../figures/matrix_computation_workflow_tensor_api.png)

1. 通过`Copy`接口将A、B原始矩阵的Global Memorydata_move到L1 Buffer中（如果存在Bias/accompanying_quantization，则同样通过`Copy`搬运到L1 Buffer中）。详细内容请参考[cube_compute_load](../cube_compute_load/cube_compute_load.md)。

2. 通过`Copy`接口将A、B矩阵分别加载到L0A Buffer和L0B Buffer上准备计算（如果存在Bias/accompanying_quantization，则通过`Copy`将L1 Buffer中的Bias数据/量化系数data_move到BiasTable Buffer/Fixpipe Buffer上）。详细内容请参考[cube_compute_load](../cube_compute_load/cube_compute_load.md)。

3. 通过`Mmad`接口对L0A Buffer、L0B Buffer、BiasTable Buffer上面的数据进行cube_compute，并输出结果到L0C Buffer上。详细内容请参考[mmad_compute](../mmad_compute/mmad_compute.md)。

4. 通过`Copy`接口将L0C Buffer的数据进行处理并搬出到Global Memory，`Copy`接口可以利用Fixpipe Buffer数据进行如accompanying_quantization、Relu等操作。详细内容请参考[cube_compute_store](../cube_compute_store/cube_compute_store.md)。

