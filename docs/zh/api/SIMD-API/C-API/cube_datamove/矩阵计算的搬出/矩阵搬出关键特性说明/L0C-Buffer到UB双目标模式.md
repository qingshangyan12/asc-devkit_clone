# L0C Buffer到UB双目标模式

同一AI Core包含一个Cube Core和两个Vector Core时，[asc_copy_l0c2ub](../../asc_copy_l0c2ub.md)可通过`dual_dst_ctrl`将L0C Buffer中的矩阵拆分后写入两个Vector Core各自的UB。

| `dual_dst_ctrl` | 行为 |
| --- | --- |
| `0` | 单目标模式，通过`sub_blockid`选择目标UB。 |
| `1` | 按M方向拆分为两个`M / 2 × N`矩阵。 |
| `2` | 按N方向拆分为两个`M × N / 2`矩阵。 |

## 约束

- 双目标模式仅用于L0C Buffer->UB通路。
- 仅支持普通NZ2NZ或单接口文档指定的NZ2ND场景，不支持其他随路能力。
- 按M方向拆分时，M必须为2的倍数。
- 按N方向拆分时，N必须满足单接口文档规定的对齐倍数。
- `n_size`、`m_size`、`dst_stride`和`src_stride`均使用C API函数原型中的参数及单位。

完整参数和调用示例参见[asc_copy_l0c2ub](../../asc_copy_l0c2ub.md)。
