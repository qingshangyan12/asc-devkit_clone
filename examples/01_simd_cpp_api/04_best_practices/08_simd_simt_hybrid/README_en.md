# SIMD and SIMT Hybrid Programming Samples

## Overview

This directory provides best-practice samples for SIMD and SIMT hybrid programming, covering flexible branch logic implementation using SIMT, memory coalescing optimization, UB-based discrete memory access optimization, and gridDim configuration optimization.

## Sample List

| Directory Name | Description | Supported Products |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [matrix_transpose](./matrix_transpose) |  This sample uses matrix transpose as an example to introduce memory coalescing optimization in Ascend C SIMD and SIMT hybrid programming scenarios, demonstrating how UB staging optimizes GM read and write access patterns. | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_floor_mod](./simt_and_simd_floor_mod) |  This sample uses floor_mod as an example to demonstrate SIMT and SIMD hybrid programming, where float computation uses SIMD and int32_t computation uses SIMT for better performance. | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_grid_dim_config](./simt_and_simd_grid_dim_config) |  This sample uses Gather as an example to introduce gridDim configuration and vf function call optimization strategies in Ascend C SIMD and SIMT hybrid programming scenarios. | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_high_performance](./simt_and_simd_high_performance) |  This sample uses FloorMod as an example to introduce performance tuning in SIMD and SIMT hybrid programming scenarios, showing the performance differences among direct SIMT access to GM, SIMD RegBase computation, SIMT access to UB, and adjusting the thread mapping so that adjacent threads within a warp access UB contiguously. | Ascend 950PR/Ascend 950DT |
| [simt_gather_with_ub](./simt_gather_with_ub) |  This sample implements the gather operator using SIMD and SIMT hybrid programming, demonstrating performance optimization by using UB to improve discrete memory access efficiency. | Ascend 950PR/Ascend 950DT |
