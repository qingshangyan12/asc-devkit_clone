# Memory Access Practices样例介绍
## 概述
基于搬运类API使用的优化样例，通过<<<>>>直调的实现方式，介绍了减少无效数据搬运、减少搬运指令数量等方法。
 
## 样例列表
| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [bank_conflict](./bank_conflict) |  基于Ascend C的bank冲突性能优化样例。 |  |
| [data_copy](./data_copy) |  本样例介绍Global Memory到UB、Global Memory到L1的数据搬运实践，对比分块粒度、非对齐数据搬运、L2Cache复用和同地址访问冲突规避对DataCopy/DataCopyPad性能的影响。 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
