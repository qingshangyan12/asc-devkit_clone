# 数据搬运（Tensor API，试验特性）

Tensor API通过`copy`统一执行不同存储位置之间的数据搬运。调用方使用Tensor的Pointer表达源和目的存储位置，使用Layout表达Shape、Stride和数据排布，并使用Operation、Trait及可选Params配置具体通路。

- [数据搬运概述](overview.md)
- [Global Memory与Unified Buffer数据搬运](gm_ub_data_move.md)
- [Global Memory与L1/L0 Buffer数据搬运](gm_l1_or_l0_data_move.md)
- [L1 Buffer与L0 Buffer数据搬运](l1_l0_data_move.md)
- [L1/L0C Buffer与Unified Buffer数据搬运](l1_or_l0c_ub_data_move.md)
- [L1 Buffer与Fixpipe Buffer数据搬运](l1_fixpipe_data_move.md)
- [L1 Buffer与BiasTable Buffer数据搬运](l1_bias_table_data_move.md)
- [Unified Buffer内部数据搬运](ub_ub_data_move.md)

`copy`、`make_copy`、Copy Operation、Trait和Atom的统一说明参见[Copy Operation、Atom和Trait](../algorithm/copy_operation_atom_trait.md)。
