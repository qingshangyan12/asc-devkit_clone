# Algorithm

Tensor API的Algorithm层通过`copy`和`mmad`提供数据搬运和矩阵计算的统一调用入口。调用前通常需要先选择Arch层的Operation和Trait，并通过`make_copy`或`make_mmad`构造Atom；需要配置运行时参数时，使用Atom的`with`成员绑定Params。

- [Copy Operation、Atom和Trait](copy_operation_atom_trait.md)
- [Mmad Operation、Atom和Trait](../matrix_compute/mmad_compute/mmad_operation_atom_trait.md)
- [数据搬运接口](../data_move/data_move.md)
- [mmad接口](../matrix_compute/mmad_compute/mmad.md)
