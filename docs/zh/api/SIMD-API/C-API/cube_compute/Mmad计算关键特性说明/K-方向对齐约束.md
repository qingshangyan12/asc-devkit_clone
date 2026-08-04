# K方向对齐约束

2201架构的[asc_mmad](../asc_mmad.md)重载提供`k_direction_align`参数，用于控制特定`float`矩阵场景下K方向的数据读取和对齐行为。启用前应核对源矩阵排布、K方向有效长度和L0A Buffer/L0B Buffer中的分形数量。

3510架构的对应参数位置为`disable_gemv`，含义不同。因此，包含`k_direction_align`的调用示例只能用于2201架构，不能直接迁移到3510架构。

具体对齐计算和支持数据类型以[asc_mmad](../asc_mmad.md)的参数说明及约束为准。
