# CV融合算子使用SSBuffer

本目录介绍在Ascend 950PR/Ascend 950DT的CV融合算子中使用SSBuffer的基础知识和算子实现方法。

- [基础知识](./basics.md)：介绍SSBuffer的适用场景、规格约束、使用方式和编程范式。
- [算子实现](./operator_impl.md)：以AIV/AIC环形消息队列通信算子为例，介绍SSBuffer空间规划、通信规则以及Device侧和Host侧实现。
- [需求分析](./ssbuffer_aiv_aic_comm_requirement_analysis.md)：整理基于SSBuffer进行AIV/AIC通信时需要满足的功能、同步和验收要求。

配套的可执行样例如下：

- [AIV/AIC环形消息队列通信](../../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ssbuf_aiv_aic_comm)：参考KFC消息状态设计，实现两条AIV到AIC的消息队列。
- [AIV/AIC环形消息队列通信C API样例](../../../../../../../../examples/02_simd_c_api/03_c_api/05_sync_control/ssbuf_aiv_aic_comm)：使用C API实现两条AIV到AIC的消息队列，也是算子实现章节采用的完整样例。
- [SSBuffer边界场景](../../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ssbuf_boundary_cases)：验证首尾32B对齐地址、AIV分区和多次任务复用。
