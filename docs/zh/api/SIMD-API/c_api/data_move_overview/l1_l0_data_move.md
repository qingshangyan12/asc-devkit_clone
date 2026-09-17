# L1与L0数据搬运

L1到L0A/L0B用于为矩阵计算装载A、B输入，运行在PIPE_MTE1；矩阵计算结果位于L0C，可通过PIPE_FIX搬到L1。DAV_3510的L1与L0通路具有方向性，不提供L0A/L0B到L1或L1到L0C的通用复制。

## 接口选择

| 方向与场景 | 推荐接口 | 流水 |
| --- | --- | --- |
| L1到L0A，二维搬运 | [asc_copy_l12l0a](../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_2d_arch_3510.md) | PIPE_MTE1 |
| L1到L0A，三维搬运 | [asc_copy_l12l0a](../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_3d_arch_3510.md) | PIPE_MTE1 |
| L1到L0A，MX数据 | [asc_copy_l12l0a_mx](../cube_datamove/asc_copy_l12l0a_mx.md) | PIPE_MTE1 |
| L1到L0B，二维搬运 | [asc_copy_l12l0b](../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_2d_arch_3510.md) | PIPE_MTE1 |
| L1到L0B，三维搬运 | [asc_copy_l12l0b](../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_3d_arch_3510.md) | PIPE_MTE1 |
| L1到L0B，MX数据 | [asc_copy_l12l0b_mx](../cube_datamove/asc_copy_l12l0b_mx.md) | PIPE_MTE1 |
| L1到L0B，转置搬运 | [asc_copy_l12l0b_trans](../cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md) | PIPE_MTE1 |
| L0C到L1 | [asc_copy_l0c2l1](../cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md) | PIPE_FIX |

## 使用要点

- L0A/L0B以分形为主要搬运粒度，起始地址、矩阵尺寸、步长和Padding必须同时满足接口约束。
- 三维搬运需要预先配置Feature Map、Padding、重复次数和尺寸等状态。A矩阵与B矩阵使用各自对应的配置接口。
- PIPE_MTE1向L0A/L0B写入后，矩阵计算读取这些Buffer前需要建立同步；PIPE_M读取L0输入、写入L0C，与后续PIPE_FIX搬出之间同样需要同步。
- `asc_copy_l12l0a_trans`、`asc_copy_l12l0b_sparse`和`asc_copy_l12l0c`不支持DAV_3510。
