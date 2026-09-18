# 概览

DAV_3510的数据搬运接口按照执行核和硬件通路分为矢量数据搬运、矩阵数据搬运以及Reg加载/存储三类。接口由标量流水发射后，可在不同搬运流水上异步执行。

## C API分类

| 分类 | 主要执行范围 | 典型通路 | 接口目录 |
| --- | --- | --- | --- |
| 矢量数据搬运 | AIV | GM与UB、UB到L1、UB内部 | [矢量数据搬运](../../vector_datamove/vector_datamove.md) |
| 矩阵数据搬运 | AIC | GM到L1、L1到L0A/L0B、L0C搬出、L1到专用Buffer | [矩阵计算搬运](../../cube_datamove/cube_datamove.md) |
| Reg加载/存储 | AIV的Vector Function | UB与矢量数据寄存器、掩码寄存器之间的数据交换 | [Reg加载](../../reg_compute/load/reg_load_overview.md)、[Reg存储](../../reg_compute/store/reg_store_overview.md) |

## 硬件流水

| 流水 | DAV_3510中的主要搬运方向 |
| --- | --- |
| PIPE_MTE1 | L1到L0A/L0B、L1到UB、L1到BiasTable |
| PIPE_MTE2 | GM到L1、GM到UB |
| PIPE_MTE3 | UB到GM、UB到L1 |
| PIPE_FIX | L0C到GM/L1/UB、L1到Fixpipe Buffer |
| PIPE_V | UB内部复制和矢量计算 |

同一流水中的指令按该流水的执行规则排序；生产者和消费者位于不同流水时，需要根据数据依赖调用[同步控制](../../sync/system_sync_overview.md)接口。接口所属流水、支持的执行核和同步示例以具体接口文档为准。

## 延伸阅读

- [数据搬运概念](data_move_concept.md)
- [数据通路](data_path.md)
- [总体约束说明](overall_constraints.md)
