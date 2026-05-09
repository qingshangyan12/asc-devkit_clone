# bank冲突算子直调样例
## 概述
本目录提供多种 bank 冲突调优样例，覆盖 Add 算子中的 UB 访存冲突，以及 ND2NZ 数据重排中 scatter 写引发的 bank 冲突，并提供核函数直调方法。

## 样例列表
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [bank_conflict_nd2nz](./bank_conflict_nd2nz) | 本样例介绍 Atlas A2/A3 系列产品和 Ascend 950PR/Ascend 950DT 上，ND 矩阵转换为 NZ 布局时通过调整 UB 写步长规避 bank 冲突的实现，并提供核函数直调方法 |
| [bank_conflict](./bank_conflict) | 本样例介绍 Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品上基于 Add 算子优化 bank 冲突的实现，并提供核函数直调方法 |
| [01_bank_conflict_950](./01_bank_conflict_950)| 本样例介绍 Ascend 950PR/Ascend 950DT 上基于 Add 算子优化 bank 冲突的实现，并提供核函数直调方法 |
