# Simt样例介绍

## 概述
基于SIMT实现的算子实现样例。

## 算子开发样例

|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [simt_gather](./simt_gather)                              | 本样例以Gather算子为示例，展示了在新同构模式下实现一个SIMT算子 |
| [simt_gather_and_simd_adds](./simt_gather_and_simd_adds)  | 本样例以gather和adds算子为示例，展示了在新同构模式下实现SIMT和SIMD混合的算子 |
| [simt_and_simd_floor_mod](./simt_and_simd_floor_mod)  | 本样例以floor算子为示例，展示了通过不同的数据类型场景使用SIMT/SIMD方式完成计算功能的混合编译方法|
| [pure_simt_gather](./pure_simt_gather)  | 本样例以gather算子为示例，展示了在纯SIMT场景下实现动态算子功能 |