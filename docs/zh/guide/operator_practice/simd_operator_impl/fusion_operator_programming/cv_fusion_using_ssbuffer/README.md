# CV融合算子使用SSBuffer

本目录专门介绍在Ascend 950PR/Ascend 950DT的CV融合算子中使用SSBuffer的基础知识、通信规则和算子实现方法。

- [基础知识](./basics.md)：介绍SSBuffer的适用场景、规格约束、使用方式和编程范式。
- [算子实现](./operator_impl.md)：以AIV/AIC环形消息队列通信算子为例，介绍SSBuffer空间规划、通信规则以及Device侧和Host侧实现。

配套的可执行样例如下：

- [AIV/AIC环形消息队列通信C API样例](../../../../../../../examples/02_simd_c_api/05_best_practices/03_fusion_compute/ssbuf_aiv_aic_comm)：使用C API实现两条AIV到AIC的消息队列，也是算子实现章节采用的完整样例。
- [AIV/AIC环形消息队列通信](../../../../../../../examples/01_simd_cpp_api/05_best_practices/03_fusion_compute/ssbuf_aiv_aic_comm)：实现两条AIV到AIC的消息队列。
