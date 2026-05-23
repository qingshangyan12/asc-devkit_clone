# 矩阵计算的搬入总体说明

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

矩阵计算的搬入接口主要实现 Global Memory 到 L1 Buffer，以及 L1 Buffer 到 L0A Buffer、L0B Buffer、L0ScaleA Buffer、L0ScaleB Buffer、BiasTable Buffer、Fixpipe Buffer 的数据传输。接口通过张量的存储位置、Layout 和数据类型在编译期选择 `impl/tensor_api/arch/cube` 下对应的数据搬运实现。

## 数据通路和存储层级

<a name="tensor_api_matrix_data_in_overview_table_data_path"></a>

| 源位置 | 源地址对齐要求 | 目的位置 | 目的地址对齐要求 | 格式转换 | 典型应用场景 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Global Memory | 1字节 | L1 Buffer(A1) | 32字节 | ND2ND、ND2NZ、ND2ZN、DN2NZ、DN2ZN、NZ2NZ、ZN2ZN | 从全局内存加载左矩阵数据到 L1 Buffer |
| Global Memory | 1字节 | L1 Buffer(B1) | 32字节 | ND2ND、ND2NZ、ND2ZN、DN2NZ、DN2ZN、NZ2NZ、ZN2ZN | 从全局内存加载右矩阵数据到 L1 Buffer |
| Global Memory | 1字节 | L1 Buffer(ScaleA) | 32字节 | ScaleA ND2ZZ、ScaleA DN2ZZ、ZZ2ZZ | 从全局内存加载左矩阵缩放系数到 L1 Buffer |
| Global Memory | 1字节 | L1 Buffer(ScaleB) | 32字节 | ScaleB ND2NN、ScaleB DN2NN、NN2NN | 从全局内存加载右矩阵缩放系数到 L1 Buffer |
| L1 Buffer(A1) | 32字节 | L0A Buffer(A2) | 512字节 | NZ2NZ、ZN2NZ、ZN2NZB8B4 | 从 L1 Buffer 加载左矩阵数据到 L0A Buffer |
| L1 Buffer(B1) | 32字节 | L0B Buffer(B2) | 512字节 | ZN2ZN、NZ2ZN、NZ2ZNB8B4 | 从 L1 Buffer 加载右矩阵数据到 L0B Buffer |
| L1 Buffer(ScaleA) | 32字节 | L0ScaleA Buffer | 512字节 | ZZ2ZZ | 从 L1 Buffer 加载左矩阵缩放系数到 L0ScaleA Buffer |
| L1 Buffer(ScaleB) | 32字节 | L0ScaleB Buffer | 512字节 | NN2NN | 从 L1 Buffer 加载右矩阵缩放系数到 L0ScaleB Buffer |
| L1 Buffer | 32字节 | BiasTable Buffer | 64字节 | ND2ND | 从 L1 Buffer 加载 Bias 数据到 BiasTable Buffer |
| L1 Buffer | 32字节 | Fixpipe Buffer | 128字节 | ND2ND | 从 L1 Buffer 加载 Fixpipe 参数到 Fixpipe Buffer |

## 格式转换说明

- GM 到 L1 通路由 `CopyGM2L1Tensor2Tensor` 路由选择，支持 ND/DN 到 NZ/ZN 的随路格式转换，也支持 ND、NZ、ZN 格式保持搬运。
- ScaleA 从 GM 搬入 L1 时，`ScaleANDLayoutPtn`、`ScaleADNLayoutPtn` 转换为 `ZZLayoutPtn`，`ZZLayoutPtn` 保持为 `ZZLayoutPtn`。
- ScaleB 从 GM 搬入 L1 时，`ScaleBNDLayoutPtn`、`ScaleBDNLayoutPtn` 转换为 `NNLayoutPtn`，`NNLayoutPtn` 保持为 `NNLayoutPtn`。
- L1 到 L0A 通路支持 `NZLayoutPtn` 到 `NZLayoutPtn` 的普通搬运，以及 `ZNLayoutPtn` 到 `NZLayoutPtn` 的转置搬运；b8/b4 数据类型使用 `ZN2NZB8B4` 专用路径。
- L1 到 L0B 通路支持 `ZNLayoutPtn` 到 `ZNLayoutPtn` 的普通搬运，以及 `NZLayoutPtn` 到 `ZNLayoutPtn` 的转置搬运；b8/b4 数据类型使用 `NZ2ZNB8B4` 专用路径。
