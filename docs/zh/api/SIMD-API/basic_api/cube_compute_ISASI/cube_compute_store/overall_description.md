# overall_description<a name="ZH-CN_TOPIC_0000002538231192"></a>

cube_compute_store是Ascend C编程框架中用于data_move的一类核心接口，主要实现L0C Buffer与Global Memory、L1 Buffer、Unified Buffer（UB）之间的数据高效传输。该接口系列提供了多种data_move模式，能够满足cube_compute中的data_move需求，通常在Mmad接口后配合使用。

cube_compute_store接口支持多种随路搬运场景，包括**accompanying_quantization、accompanying_relu、随路格式转换（NZ2ND、NZ2DN）、随路通道拆分（Channel Split）、随路通道合并（Channel Merge）、UB双目标模式**。通过灵活配置不同的参数结构体，开发者可以精确控制数据的搬运路径、起始位置、搬运长度、随路搬运能力等关键信息，充分利用硬件的data_move能力，最大化算子的整体执行性能。

<!-- npu="950,A3,910b" id1 -->
cube_compute_store整体流程如下图所示：
<!-- end id1 -->

<!-- npu="A3,910b" id2 -->
**图1** cube_compute_store整体流程图（[NPU架构版本2201](../../../../../guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)）

![](../../../../figures/fixpipe_execution_flow_a2a3.png)
<!-- end id2 -->

<!-- npu="950" id3 -->
**图2** cube_compute_store整体流程图（[NPU架构版本3510](../../../../../guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)）

![](../../../../figures/fixpipe_execution_flow_a5.png)
<!-- end id3 -->

其中，Fixpipe Buffer不同的量化模式对应的内存排布如下：

**图3** Fixpipe Buffer内存排布图

![](../../../../figures/fixpipe_buffer.png)
