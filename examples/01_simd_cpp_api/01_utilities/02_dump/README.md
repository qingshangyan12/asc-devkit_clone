# Dump样例介绍
## 概述
本样例展示了Ascend C asc_dump系列接口和simd_vf侧asc_dump接口的基本使用方法。

## 样例列表
| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [00_simple_dump](./00_simple_dump) | 使用静态Tensor编程模式实现矩阵乘法，展示asc_dump_gm/asc_dump_l1buf/asc_dump_cbuf/asc_dump_ubuf接口的基本使用方法 | Ascend 950PR/Ascend 950DT<br>Atlas A3训练系列产品/Atlas A3推理系列产品<br>Atlas A2训练系列产品/Atlas A2推理系列产品 |
| [01_simd_vf_dump](./01_simd_vf_dump) | 使用vector编程模式，展示simd_vf侧__asc_simd_vf::asc_dump_ubuf/asc_dump/__asc_simd_vf::asc_dump_reg接口的基本使用方法 | Ascend 950PR/Ascend 950DT<br>Atlas A3训练系列产品/Atlas A3推理系列产品 |
