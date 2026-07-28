# 基础 API 到 C API 的迁移样例

## 概述

本目录存放 `examples/01_simd_cpp_api/03_basic_api` 的 C API 迁移样例。迁移原则如下：

- 使用 `c_api/asc_simd.h`，以带地址空间限定符的数组分配片上内存；
- 优先使用当前仓库 C API 文档中已声明且支持 `dav-3510` 的接口；
- 当原基础 API 没有文档化的等价 C API 时，保留原始功能，并用显式数组和受支持的 C API 重写；
- 每个不具备逐指令等价关系的样例都在 [MIGRATION_NOTES.md](./MIGRATION_NOTES.md) 中说明。

## 目录结构

```text
04_basic_api_migration/
├── 01_memory_vector_compute/
│   └── element_wise_arithmetic/  # LeakyReLU
├── 02_reg_vector_compute/
│   └── vf_add/                   # dav-3510 VF Reg Add
├── 00_data_movement/
│   └── copy_ub2ub/               # UB 内数据搬运
│   └── data_copy_gm2l1/           # GM→L1、矩阵乘、Bias 与向量量化
├── MIGRATION_NOTES.md            # 映射、证据与待确认项
└── README.md
```

## 目标架构

样例的 CMake 工程固定使用 `dav-3510`。其中 `data_copy_gm2l1` 已完整覆盖原基础 API 的五个场景：Nz、ND、DN、ND+float Bias 与 ND+Vector Quant。本次仅进行了静态文档、声明和实现核对，未执行配置、编译或运行。
