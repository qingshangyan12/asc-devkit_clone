# Memory矢量计算C API样例介绍

## 概述

本路径包含 Memory 矢量计算 C API 样例。每个样例均基于 Ascend C 的 `<<<>>>` 直调方式，主机函数和核函数在同一个 `.asc` 文件中实现，完成 GM-UB-计算-GM 数据流。

本目录与 `02_reg_vector_compute` 不同：后者使用寄存器操作数的 Reg C API；本目录使用 Memory 矢量计算 C API。样例仅支持 Atlas A2训练/推理系列和 Atlas A3训练/推理系列产品（`dav-2201`），不支持 `dav-3510`。

## 算子开发样例

| 目录名称 | 功能描述 |
| --- | --- |
| [cast](./cast) | 本样例基于 Memory 矢量计算 C API 实现 half 到打包 int4 或 int32 的类型转换，主要调用 `asc_half2int4` 和 `asc_half2int32` 接口 |
| [compare](./compare) | 本样例基于 Memory 矢量计算 C API 实现向量和标量比较，主要调用 `asc_lt`、`asc_lt_scalar`、`asc_gt_scalar` 和 `asc_get_cmp_mask` 接口 |
| [element_wise_arithmetic](./element_wise_arithmetic) | 本样例基于 Memory 矢量计算 C API 实现 Leaky ReLU，主要调用 `asc_leakyrelu` 接口 |
| [reduce](./reduce) | 本样例基于 Memory 矢量计算 C API 实现向量求和归约，主要调用 `asc_repeat_reduce_sum` 接口 |
