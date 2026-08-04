# UB与UB数据搬运

[asc_copy_ub2ub](../asc_copy_ub2ub.md)用于UB内部数据搬运，当前公开接口包含以下形式：

| 形式 | 主要参数 | 说明 |
| --- | --- | --- |
| 连续搬运 | `dst`、`src`、`size` | `size`表示搬运字节数。 |
| 分块搬运 | `dst`、`src`、`n_burst`、`len_burst`、`src_gap`、`dst_gap` | 用于多段非连续数据，参数单位和范围以单接口文档为准。 |
| 同步搬运 | `asc_copy_ub2ub_sync` | 在接口内部完成该次搬运所需的同步，适用条件以单接口文档为准。 |

UB内部搬运直接使用公开原型中的标量参数。需要基于mask执行矢量数据选择或重排时，应从Reg或Memory矢量计算分类选择对应计算接口，不能将其作为`asc_copy_ub2ub`重载。

源、目的区域重叠时，应遵循[asc_copy_ub2ub](../asc_copy_ub2ub.md)的约束；未明确支持的重叠场景应使用独立缓冲区。
