# BestPractices样例介绍
## 概述
基于Ascend C的性能优化实践，聚焦于关键算子与内存访问的调优，旨在提升在Ascend平台上的运行效率。

## 样例列表
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
<<<<<<< HEAD
| [00_vector_compute_practices](./00_vector_compute_practices) | 基于Vector Compute API使用的优化样例，通过<<<>>>直调的实现方式，介绍基于静态Tensor方式编程的性能调优方法。 |
| [01_matrix_compute_practices](./01_matrix_compute_practices) | 基于Matrix Compute API使用的优化样例，介绍Matmul算子的性能调优方法。 |
| [02_reg_vector_compute_practices](./02_reg_vector_compute_practices) | 基于VF函数的性能优化样例，通过<<<>>>直调的实现方式，介绍了VF循环优化、VF指令双发优化、VF连续非对齐场景优化、VF融合优化的方法。 |
| [03_fusion_compute_practices](./03_fusion_compute_practices) | 融合计算相关的性能优化样例，介绍QuantGroupMatmul、Matmul+GELU等Cube-Vector融合算子的高性能实现，以及SIMD与SIMT混合编程高性能优化样例。 |
| [04_memory_access_practices](./04_memory_access_practices) | 基于搬运类API使用的优化样例，通过<<<>>>直调的实现方式，介绍了减少无效数据搬运、减少搬运指令数量等方法。 |
=======
| [00_vector_compute](./00_vector_compute) | 基于Vector Compute API使用的优化样例，通过<<<>>>直调的实现方式，介绍基于静态Tensor方式编程的性能调优方法。 |
| [01_matrix_compute](./01_matrix_compute) | 基于Matrix Compute API使用的优化样例，介绍Matmul算子的性能调优方法。 |
| [02_reg_compute](./02_reg_compute) | 基于VF函数的性能优化样例，通过<<<>>>直调的实现方式，介绍了VF循环优化、VF指令双发优化、VF连续非对齐场景优化、VF融合优化的方法。 |
| [03_fusion_compute](./03_fusion_compute) | 基于Matmul融合算子实现的高性能优化样例，介绍Cube-Vector融合实现方法。 |
| [04_memory_access](./04_memory_access) | 基于搬运类API使用的优化样例，通过<<<>>>直调的实现方式，介绍了减少无效数据搬运、减少搬运指令数量等方法。 |
| [08_simd_simt_hybrid](./08_simd_simt_hybrid) | 基于SIMT与SIMD混合编程方式实现的算子样例，介绍SIMT灵活实现分支判断逻辑，以及使用UB提升离散内存访问效率的性能优化方式。 |
>>>>>>> 1a2ae5575 (restructure 01_simd_cpp_api examples directory)
