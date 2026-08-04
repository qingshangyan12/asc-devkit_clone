# GEMV

GEMV用于矩阵与向量相乘。3510架构的[asc_mmad](../asc_mmad.md)重载提供`disable_gemv`参数：当`left_height`为1且`disable_gemv`为`false`时启用GEMV模式，L0A Buffer按该模式要求读取。

2201架构的对应参数位置是`k_direction_align`，不提供`disable_gemv`参数。代码不能在两个架构间直接复用同一布尔实参含义，应依据`asc_mmad`文档中按架构分组的原型选择调用。

GEMV场景还应满足L0A Buffer、L0B Buffer和L0C Buffer的排布、地址对齐及数据类型约束。完整要求以[asc_mmad](../asc_mmad.md)为准。
