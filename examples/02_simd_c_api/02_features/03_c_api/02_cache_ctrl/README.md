# C-API样例介绍
## 概述
本样例展示了使用C_API构建缓存控制（Cache Control）算子样例的编译流程，支持main函数和kernel函数在同一个cpp文件中实现。

## 样例列表
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [00_scalar_cache_mode](./00_scalar_cache_mode) | 本样例验证asc_set_scalar_cache_mode和asc_get_scalar_cache_mode接口的正确性，采用set→get回环一致性+no-op行为检查策略 |
