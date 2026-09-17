# 数据通路

DAV_3510包含GM、L1、L0A、L0B、L0C、UB、BiasTable、Fixpipe Buffer和SIMD Register File。不同存储单元由不同搬运流水连接，不能仅通过地址类型推断某一方向是否存在直达通路。

**图1** DAV_3510硬件架构与同步关系

![DAV_3510硬件架构与同步关系](../../../../figures/npu_3510_hw_arch_sync.png "DAV_3510硬件架构与同步关系")

## 存储单元用途

| 存储单元 | 主要用途 |
| --- | --- |
| GM | 保存Kernel输入、输出以及跨核共享数据。 |
| L1 | Cube侧输入缓存，也是GM与L0A/L0B之间的中转存储。 |
| L0A/L0B | 保存矩阵计算的A、B输入。 |
| L0C | 保存矩阵计算累加结果。 |
| UB | Vector侧片上数据缓存。 |
| BiasTable | 保存Mmad使用的bias。 |
| Fixpipe Buffer | 保存Fixpipe随路量化和随路ReLU参数。 |
| SIMD Register File | 保存Vector Function使用的矢量数据、掩码和地址状态。 |

## 支持的通路

| 数据通路 | 流水 | 执行范围 | 详细说明 |
| --- | --- | --- | --- |
| GM到L1、L0C到GM | PIPE_MTE2、PIPE_FIX | AIC | [GM与L1/L0数据搬运](../gm_l1_or_l0_data_move.md) |
| GM到UB、UB到GM | PIPE_MTE2、PIPE_MTE3 | AIV | [GM与UB数据搬运](../gm_ub_data_move.md) |
| L1到L0A/L0B、L0C到L1 | PIPE_MTE1、PIPE_FIX | AIC | [L1与L0数据搬运](../l1_l0_data_move.md) |
| L1到UB、UB到L1、L0C到UB | PIPE_MTE1、PIPE_MTE3、PIPE_FIX | AIC或AIV，随方向而定 | [L1/L0C与UB数据搬运](../l1_or_l0c_ub_data_move.md) |
| L1到BiasTable | PIPE_MTE1 | AIC | [L1到BiasTable数据搬运](../l1_bias_table_data_move.md) |
| L1到Fixpipe Buffer | PIPE_FIX | AIC | [L1到Fixpipe Buffer数据搬运](../l1_fixpipe_data_move.md) |
| UB到UB | PIPE_V | AIV | [UB内部数据搬运](../ub_ub_data_move.md) |
| Reg与UB | Vector Function | AIV | [Reg与UB数据搬运](../reg_ub_data_move.md) |

DAV_3510不支持GM到L0A/L0B直达搬运，数据需要经过GM到L1、L1到L0两段通路；也不支持L1到GM直达搬运。与其他NPU架构共用的接口名不代表DAV_3510一定支持该接口，产品支持情况应以接口文档中的芯片标签为准。

硬件存储结构的完整说明请参见[DAV_3510架构规格](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)。
