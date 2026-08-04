# L1 Buffer或L0C Buffer与UB数据搬运

| 数据通路 | 接口 | 说明 |
| --- | --- | --- |
| L1 Buffer->UB | [asc_copy_l12ub](../asc_copy_l12ub.md) | 用于将L1 Buffer数据搬运到UB。 |
| UB->L1 Buffer | [asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md) | 用于将UB数据搬运到L1 Buffer。 |
| L0C Buffer->UB | [asc_copy_l0c2ub](../asc_copy_l0c2ub.md) | 可根据重载配置数据类型转换、量化或双目标模式。 |

上述接口直接使用公开函数原型中的标量参数。`asc_copy_l0c2ub`不同产品上的数据类型转换、量化参数来源和同步要求不同，调用前应核对其完整原型及约束。
