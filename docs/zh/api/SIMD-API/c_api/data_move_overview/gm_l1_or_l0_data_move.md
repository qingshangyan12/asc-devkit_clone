# GM与L1/L0数据搬运

DAV_3510在GM与Cube侧片上存储之间提供GM到L1和L0C到GM通路。GM数据进入L0A/L0B时，必须先搬到L1，再通过L1到L0A/L0B接口装载；L1不能直接写回GM。

## 接口选择

| 方向与场景 | 推荐接口 | 流水 |
| --- | --- | --- |
| GM到L1，连续或高维切分搬运 | [asc_copy_gm2l1](../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md) | PIPE_MTE2 |
| GM到L1，二维分形搬运 | [asc_copy_gm2l1](../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_2d_arch_3510.md) | PIPE_MTE2 |
| GM到L1，ND2NZ转换 | [asc_copy_gm2l1_nd2nz](../cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md) | PIPE_MTE2 |
| GM到L1，DN2NZ转换 | [asc_copy_gm2l1_dn2nz](../cube_datamove/asc_copy_gm2l1_dn2nz.md) | PIPE_MTE2 |
| GM到L1，非对齐搬运 | [asc_copy_gm2l1_align](../cube_datamove/asc_copy_gm2l1_align.md) | PIPE_MTE2 |
| L0C到GM，支持矩阵结果转换或随路处理 | [asc_copy_l0c2gm](../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md) | PIPE_FIX |

## 使用要点

- GM到L1的高维、Padding和NZ参数由接口入参及[asc_set_gm2l1_padding](../cube_datamove/asc_set_gm2l1_padding.md)、[asc_set_gm2l1_pad](../cube_datamove/asc_set_gm2l1_pad.md)、[asc_set_gm2l1_nz_para](../cube_datamove/asc_set_gm2l1_nz_para.md)等配置接口共同决定。
- L0C到GM可结合量化、类型转换、ReLU和格式转换能力。启用能力前应按照接口文档配置L0C搬出参数，并按转换后的目的排布检查GM空间。
- GM到L1属于PIPE_MTE2，后续L1到L0A/L0B属于PIPE_MTE1；两段存在数据依赖时必须显式同步。
- `asc_copy_gm2l0a`、`asc_copy_gm2l0b`和`asc_copy_l12gm`不支持DAV_3510，不应在Ascend 950PR/Ascend 950DT代码路径中调用。
