# NZ2ND

NZ2ND用于在矩阵结果搬出时，将L0C Buffer中的NZ分形排布转换为普通ND排布。主要通过[asc_copy_l0c2gm](../../asc_copy_l0c2gm/asc_copy_l0c2gm.md)或[asc_copy_l0c2ub](../../asc_copy_l0c2ub.md)中支持NZ2ND的重载实现。

## 主要参数

- `n_size`、`m_size`：源NZ矩阵在N、M方向上的有效大小。
- `src_stride`：源NZ矩阵中相邻Z排布的地址间隔。
- `dst_stride`：目的ND矩阵每行的元素数或对应接口定义的目的间隔。
- `enable_nz2nd`：3510架构`asc_copy_l0c2gm`重载中的NZ2ND开关。

不同目的存储位置和NPU架构的参数集合、stride单位和维度上限不同，应以对应单接口文档为准。

## 约束

- 源、目的地址和矩阵尺寸必须满足所选重载的对齐要求。
- 目的地址区域不能互相重叠。
- 与Channel Split、Channel Merge、量化或ReLU组合时，必须使用单接口文档明确支持的组合。
- 多矩阵场景的矩阵数量和矩阵间stride只能使用对应重载公开的参数，不能引用其他编程接口中的字段名。
