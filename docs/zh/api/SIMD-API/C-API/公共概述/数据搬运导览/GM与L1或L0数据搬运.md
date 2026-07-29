# GM与L1或L0数据搬运

| 基础API场景 | C API落点 | `dav-3510`状态 |
|---|---|---|
| GM->L1 Buffer连续/高维 | [asc_copy_gm2l1](../../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1.md)、[asc_copy_gm2l1_align](../../cube_datamove/asc_copy_gm2l1_align.md) | 支持形式以文档为准。 |
| GM->L1 Buffer ND2NZ/DN2NZ | [asc_copy_gm2l1_nd2nz](../../cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md)、[asc_copy_gm2l1_dn2nz](../../cube_datamove/asc_copy_gm2l1_dn2nz.md)、[asc_set_gm2l1_nz_para](../../sys_var/asc_set_gm2l1_nz_para.md) | 支持。 |
| GM->L1 Buffer非对齐/Padding | [asc_copy_gm2l1_align](../../cube_datamove/asc_copy_gm2l1_align.md) | 支持受接口约束。 |
| GM->L1 Buffer NZ矩阵 | [asc_copy_gm2l1](../../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1.md) | 支持。 |
| GM->L0A Buffer/L0B Buffer | [asc_copy_gm2l0a](../../cube_datamove/asc_copy_gm2l0a.md)、[asc_copy_gm2l0b](../../cube_datamove/asc_copy_gm2l0b.md) | 3510通路不支持；不得作为3510迁移目标。 |
| L0C Buffer->GM量化/激活 | [asc_copy_l0c2gm](../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm.md)、[asc_set_l0c2gm_config](../../cube_compute/asc_set_l0c2gm_config.md) | 支持，属于复合配置路径。 |
