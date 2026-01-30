# Add样例介绍
## 概述
基于Ascend C的Add算子的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。

## 算子开发样例
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [basic_api_memory_allocator_add](./basic_api_memory_allocator_add) | 样例基于静态Tensor方式编程实现Add样例，展示了LocalMemAllocator进行线性内存分配 |
| [basic_api_tque_add](./basic_api_tque_add) | 样例基于静态Tensor编程方法实现Add样例，展示了使用tque管理内存 |
| [micro_api_add](./micro_api_add) | 样例基于微指令API实现Add样例，展示了通过微指令API直接对芯片中涉及Vector计算的寄存器进行操作 |