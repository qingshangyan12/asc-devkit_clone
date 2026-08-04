# Unit Flag

unit flag用于协调[asc_mmad](../asc_mmad.md)写L0C Buffer与[asc_copy_l0c2gm](../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm.md)读取L0C Buffer，实现分形粒度的计算和搬出并行。

`asc_mmad`参数名为`unit_flag`，3510架构`asc_copy_l0c2gm`对应参数名为`unit_flag_mode`。两端必须针对同一L0C Buffer区域一致启用。

| 值 | 含义 |
| --- | --- |
| `0` | 关闭unit flag。 |
| `1` | 保留值，不应作为普通配置使用。 |
| `2` | 启用unit flag，指令结束后保持当前占用状态。 |
| `3` | 启用unit flag，指令结束后切换占用状态。 |

典型的K方向多次累加场景中，前若干次`asc_mmad`使用值`2`，最后一次使用值`3`，随后搬出接口使用值`3`释放对应L0C Buffer区域。实际配置还必须结合`c_matrix_source`和`c_matrix_init_val`，避免在每次K循环中错误地重新初始化C矩阵。

## 约束

- 计算和搬出的分形访问顺序必须一致。需要时使用[asc_set_mmad_direction_m](../asc_set_mmad_direction_m.md)或[asc_set_mmad_direction_n](../asc_set_mmad_direction_n.md)配置方向。
- 计算覆盖范围和搬出覆盖范围必须匹配；部分搬出不能用[asc_set_l0c2gm_config](../asc_set_l0c2gm_config.md)重置unit flag状态。
- `asc_set_l0c2gm_config`仅配置预处理矢量地址和是否启用unit flag。
- `asc_mmad`在2201和3510架构上的布尔参数不同，调用时应使用对应架构的完整原型。
