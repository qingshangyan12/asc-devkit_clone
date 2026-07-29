# C-API样例介绍
## 概述
本样例展示了使用C_API构建系统变量（System Variable）配置样例的编译流程，支持main函数和kernel函数在同一个cpp文件中实现。

## 样例列表
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [00_saturation_flag](./00_saturation_flag) | 本样例验证asc_set_saturation_flag和asc_get_saturation_flag接口的正确性，采用set→get回环一致性策略，覆盖4种饱和模式×2种状态的极性验证 |
| [01_saturation_strategy](./01_saturation_strategy) | 本样例验证asc_set_saturation_strategy和asc_get_saturation_strategy接口的正确性，采用set→get回环一致性策略，覆盖USE_API和USE_GLOBAL两种策略 |
