# L1或L0C与UB数据搬运

| 基础API场景 | C API落点 | 状态 |
|---|---|---|
| UB->L1 Buffer连续/高维 | [asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md) | 支持接口形态以文档为准。 |
| UB->L1 Buffer ND2NZ、非对齐/Padding | 无直接同语义C API | 不将Basic API路径伪映射。 |
| L0C Buffer->UB量化/激活 | [asc_copy_l0c2ub](../../cube_datamove/asc_copy_l0c2ub.md)及[asc_set_l0c2gm_config](../../cube_compute/asc_set_l0c2gm_config.md) | 配置与数据搬出共同生效。 |
| L1 Buffer->UB连续/高维 | [asc_copy_l12ub](../../cube_datamove/asc_copy_l12ub.md) | 支持；注意接口的核类型限制。 |
