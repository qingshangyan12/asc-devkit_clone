# C-API样例介绍
## overview
本样例展示了使用C_API构建Vector算子样例的编译流程，支持main函数和kernel函数在同一个cpp文件中实现。

## 样例列表
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [00_sync_add](./00_sync_add) | 本样例展示了使用C_API 基于同步搬运/计算接口构建Add算子样例的编译流程，支持main函数和kernel函数在同一个cpp文件中实现 |
| [01_async_add](./01_async_add) | 本样例展示了使用C_API 基于异步搬运/计算和统一sync_interface构建Add算子样例的编译流程，支持main函数和kernel函数在同一个cpp文件中实现 |
| [02_c_api_delicacy_async_add](./02_c_api_delicacy_async_add) | 本样例采用C_API接口编写Add算子样例，基于异步搬运、计算接口和精细化sync_interface实现 |