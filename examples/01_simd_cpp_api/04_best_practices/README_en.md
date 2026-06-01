# BestPractices Sample Introduction

## Overview
Performance optimization practices based on Ascend C, focusing on tuning key operators and memory access, aimed at improving execution efficiency on the Ascend platform.

## Sample List
| Directory Name | Description |
| --- | --- |
<<<<<<< HEAD
| [00_vector_compute_practices](./00_vector_compute_practices) | Optimization samples based on Vector Compute API usage, introducing performance tuning methods for static Tensor programming through the <<<>>> direct call implementation. |
| [01_matrix_compute_practices](./01_matrix_compute_practices) | Optimization samples based on Matrix Compute API usage, introducing performance tuning methods for the Matmul operator. |
| [02_reg_vector_compute_practices](./02_reg_vector_compute_practices) | Performance optimization samples based on VF functions, introducing VF loop optimization, VF instruction dual-issue optimization, VF continuous non-aligned scenario optimization, and VF fusion optimization through the <<<>>> direct call implementation. |
| [03_fusion_compute_practices](./03_fusion_compute_practices) | Performance optimization samples for fusion compute, introducing high-performance implementations of Cube-Vector fusion operators such as QuantGroupMatmul and Matmul+GELU, as well as high-performance optimization samples for SIMD and SIMT hybrid programming. |
| [04_memory_access_practices](./04_memory_access_practices) | Optimization samples based on data copy API usage, introducing methods to reduce invalid data copy and reduce data copy instruction count through the <<<>>> direct call implementation. |
=======
| [00_vector_compute](./00_vector_compute) | Optimization samples based on Vector Compute API usage, introducing performance tuning methods for static Tensor programming through the <<<>>> direct call implementation. |
| [01_matrix_compute](./01_matrix_compute) | Optimization samples based on Matrix Compute API usage, introducing performance tuning methods for the Matmul operator. |
| [02_reg_compute](./02_reg_compute) | Performance optimization samples based on VF functions, introducing VF loop optimization, VF instruction dual-issue optimization, VF continuous non-aligned scenario optimization, and VF fusion optimization through the <<<>>> direct call implementation. |
| [03_fusion_compute](./03_fusion_compute) | High-performance optimization samples based on Matmul fusion operator implementation, introducing Cube-Vector fusion implementation methods. |
| [04_memory_access](./04_memory_access) | Optimization samples based on data copy API usage, introducing methods to reduce invalid data copy and reduce data copy instruction count through the <<<>>> direct call implementation. |
| [08_simd_simt_hybrid](./08_simd_simt_hybrid) | Operator samples implemented based on SIMT and SIMD hybrid programming, introducing flexible branch logic implementation using SIMT and performance optimization methods using UB to improve discrete memory access efficiency. |
>>>>>>> 1a2ae5575 (restructure 01_simd_cpp_api examples directory)
