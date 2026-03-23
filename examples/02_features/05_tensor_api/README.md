# tensor_api样例介绍

## 概述

本样例展示了使用tensor_api构建Matmul算子样例的编译流程。

## 算子开发样例

|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [00_tensor_api_matmul](./00_tensor_api_matmul) | 本样例展示了使用Tensor_API 基于Layout和Tensor构造分形完成单核Matmul样例的编译流程，并展示了多样化的调用形式,支持main函数和kernel函数在同一个cpp文件中实现 |
| [01_tensor_api_matmul_multicore](./01_tensor_api_matmul_multicore) | 本样例展示了使用Tensor_API 基于Layout和Tensor构造分形完成多核场景Matmul样例的编译流程，并展示了多样化的调用形式,支持main函数和kernel函数在同一个cpp文件中实现 |
| [02_tensor_api_matmul_relu_quant](./02_tensor_api_matmul_relu_quant) | 本样例展示了使用Tensor_API 基于Layout和Tensor构造分形完成单核Matmul样例的编译流程，并使能Relu和随路量化将矩阵乘结果由float类型数据量化为int8_t类型，支持main函数和kernel函数在同一个cpp文件中实现 |