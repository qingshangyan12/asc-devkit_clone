# GM与UB数据搬运

GM与UB数据搬运是Vector侧输入、输出的主要通路。GM到UB运行在PIPE_MTE2，UB到GM运行在PIPE_MTE3，接口仅在AIV侧生效。

## 接口选择

| 方向与场景 | 推荐接口 | 说明 |
| --- | --- | --- |
| GM到UB，连续或高维切分搬运 | [asc_copy_gm2ub](../vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md) | 地址和长度满足普通搬运约束时使用。 |
| GM到UB，非对齐搬运 | [asc_copy_gm2ub_align](../vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md) | 支持非对齐有效数据和边界填充。 |
| UB到GM，连续或高维切分搬运 | [asc_copy_ub2gm](../vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md) | 结果地址和长度满足普通搬运约束时使用。 |
| UB到GM，非对齐搬运 | [asc_copy_ub2gm_align](../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md) | 用于尾块或非对齐目的范围。 |
| GM到UB，最多五维搬运及Padding | [asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md) | 使用NDDMA描述多维循环。 |

## 使用要点

- 普通接口的连续模式通常以字节描述总长度；高维切分模式的长度和间隔单位以具体原型为准。
- 使用GM到UB非对齐搬运的Padding能力前，通过[asc_set_copy_pad_val](../vector_datamove/asc_set_copy_pad_val.md)设置填充值。
- NDDMA的各维步长、Padding数量和Padding值分别通过[asc_set_ndim_loop_stride](../vector_datamove/asc_set_ndim_loop_stride.md)、[asc_set_ndim_pad_count](../vector_datamove/asc_set_ndim_pad_count.md)和[asc_set_ndim_pad_value](../vector_datamove/asc_set_ndim_pad_value.md)配置。
- 多核可能读取同一块GM且该数据可能被其他核更新时，在使用NDDMA前按[asc_ndim_copy_dci](../vector_datamove/asc_ndim_copy_dci.md)说明执行DCI操作。
- PIPE_MTE2或PIPE_MTE3与PIPE_V之间存在数据依赖时，应在搬运与矢量计算之间插入匹配的同步操作。
