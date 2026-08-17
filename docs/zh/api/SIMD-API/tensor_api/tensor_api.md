# Tensor API（试验特性）

Tensor API是一套面向Ascend C算子开发的C++接口，通过Tensor及其layout统一描述数据的存储位置、数据类型、shape和stride，并在此基础上提供数据搬运与矩阵计算能力，降低开发者组织硬件指令参数和管理数据排布的复杂度。

Tensor API当前属于试验特性，提供部分常用数据结构和计算流程的抽象，并不覆盖全部硬件能力，也不用于替代C API。当前主要提供以下能力：

- 基础数据结构：提供tensor、pointer、layout、shape、stride和coord等数据结构，以及构造、查询、切片和排布处理接口。
- 矩阵计算搬运：支持矩阵数据在Global Memory、L1 Buffer、L0A Buffer、L0B Buffer、L0C Buffer及相关片上存储之间搬运，并支持部分数据排布转换和搬运关键特性。
- 矩阵计算：提供基于Tensor的`mmad`矩阵计算能力，以及GEMV、HF32和`unit_flag`等相关特性。

Tensor API和C API可以在同一个Ascend C核函数中配合使用。Tensor API用于组织tensor、layout及主要的数据搬运和矩阵计算流程；缓存控制、同步控制、系统变量配置等Tensor API暂未覆盖的底层能力，可通过C API补充。使用时需要引入Tensor API统一入口头文件，并根据所需能力引入对应的C API头文件，具体引入头文件请查看C API文档。

两类接口的产品支持范围和使用约束相互独立，混合使用时需要同时满足对应资料中的要求。C API操作涉及Tensor API所使用的同一存储区域时，需要正确传递地址和参数，并保证操作顺序、数据可见性和缓存一致性。具体接口支持的数据类型、数据排布、产品型号和约束以各接口资料为准。

- **[基础数据结构](basic_data_structure/basic_data_structure.md)**

- **[矩阵计算](matrix_computation/matrix_computation.md)**

- **[矩阵计算搬运](matrix_data_movement/matrix_data_movement.md)**

- **[缓存控制（C API）](cache_ctrl/cache_ctrl_overview.md)**

- **实验目录**（暂时为空）
