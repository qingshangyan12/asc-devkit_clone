# 随路ReLU

矩阵结果搬出时，可在支持的产品和数据类型组合上执行Normal ReLU、Scalar ReLU、Vector ReLU或Clip ReLU。主要入口为[asc_copy_l0c2gm](../../asc_copy_l0c2gm/asc_copy_l0c2gm.md)。

3510架构的重载使用`relu_pre_mode`选择ReLU模式，并通过`enable_clip_relu_pre`控制Clip ReLU；2201架构的参数名称和能力集合不同，应进入对应架构文档核对。

Vector ReLU所需矢量地址通过[asc_set_l0c2gm_config](../../../cube_compute/asc_set_l0c2gm_config.md)配置，参数数据通过[asc_copy_l12fb](../../asc_copy_l12fb/asc_copy_l12fb.md)搬入Fixpipe Buffer。

Clip ReLU必须与Normal ReLU及目标架构要求的量化模式配合使用。与随路量化组合时，参见[随路量化与随路ReLU场景组合](随路量化与随路ReLU场景组合.md)。
