# Basic API样例介绍

## overview

本样例介绍了Ascend C Basic API的使用方法，包括data_move、内存向量计算、Reg向量计算、cube_compute、resource_management、sync_control、atomic_operations、TPipe/TQue和工具类API等。

## 样例列表

| 目录名称  | 功能描述 |
| --------- | --------- |
| [00_data_movement](./00_data_movement) | 本路径下包含了与data_move相关的多个API的样例，展示了data_move接口的使用。|
| [01_memory_vector_compute](./01_memory_vector_compute) | 本样例路径以reduce、sort、transpose等算子为示例，展示了内存向量计算接口的使用。|
| [02_reg_vector_compute](./02_reg_vector_compute) | 本路径下包含了与向量计算相关的多个API的样例，基于Reg编程接口实现。|
| [03_matrix_compute](./03_matrix_compute) | 本样例路径以batch_matmul等算子为示例，展示了cube_compute接口的使用。|
| [04_memory_management](./04_memory_management) | 本路径下包含了与resource_management相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。         |
| [05_sync_control](./05_sync_control) | 本路径下包含了与sync_control相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。         |
| [06_atomic](./06_atomic) | 本路径下包含了与atomic_operations相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。         |
| [07_tpipe_tque](./07_tpipe_tque) | 本路径下包含了与tpipe和tque相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。         |
| [09_utils](./09_utils) | 本路径下包含了与工具相关API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。         |
