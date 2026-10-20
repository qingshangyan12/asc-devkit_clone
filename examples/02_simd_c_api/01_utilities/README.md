# Utilities功能
## 概述
介绍Ascend C常用调试接口和工具

本目录的内核基础计算、数据搬运和同步使用 C API，目标架构为 `dav-3510`。`printf`、`asc_dump`、`ascendc_assert` 等调试工具接口可由 C/C++ API 样例共用。

## 样例列表
|  目录名称                                                   |  功能描述                                              | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [00_printf](./00_printf) | Printf接口打印功能 | Ascend 950PR&950DT系列产品 |
| [01_assert](./01_assert) | Assert接口异常检测功能 | Ascend 950PR&950DT系列产品 |
| [02_dump](./02_dump) | Dump数据调测功能 | Ascend 950PR&950DT系列产品 |
| [03_clock](./03_clock) | Kernel函数执行耗时打印 | Ascend 950PR&950DT系列产品 |
| [04_profiling](./04_profiling) | 使用Profiling工具采集性能数据 | Ascend 950PR&950DT系列产品|
| [05_sanitizer](./05_sanitizer) | Sanitizer调测工具的使用方法 | Ascend 950PR&950DT系列产品 |
| [06_simulator](./06_simulator) | CAmodel仿真与问题分析 | Ascend 950PR&950DT系列产品 |
| [07_log](./07_log) | Kernel日志功能（日志打屏/落盘/级别控制） | Ascend 950PR&950DT系列产品 |
