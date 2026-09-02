# CV融合算子使用SSBuffer

本目录介绍在Ascend 950PR/Ascend 950DT的CV融合算子中使用SSBuffer的约束、适用场景和编程方法。

- [SSBuffer使用说明](./ssbuffer_usage.md)：说明存储规格、与`ENABLE_CV_COMM_VIA_SSBUF`的关系、AIV/AIC通信协议和调用示例。

配套的可执行样例如下：

- [AIV/AIC环形队列通信](../../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ssbuf_aiv_aic_comm)：参考KFC消息状态机实现两条AIV到AIC的队列。
- [SSBuffer边界场景](../../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ssbuf_boundary_cases)：验证首尾32B对齐地址、AIV分区和多次任务复用。
