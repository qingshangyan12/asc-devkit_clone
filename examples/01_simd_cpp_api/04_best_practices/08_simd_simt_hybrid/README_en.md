# SIMD and SIMT Hybrid Programming Samples

## Overview

This directory provides best-practice samples for SIMD and SIMT hybrid programming, covering flexible branch logic implementation using SIMT, memory coalescing optimization, and performance optimization using UB to improve discrete memory access efficiency.

## Sample List

| Directory Name | Description | Supported Products |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [matrix_transpose](./matrix_transpose) |  This sample uses matrix transpose as an example to introduce memory coalescing optimization in Ascend C SIMD and SIMT hybrid programming scenarios, demonstrating how UB staging optimizes GM read and write access patterns. | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_floor_mod](./simt_and_simd_floor_mod) |  This sample uses the FloorMod operator as an example to demonstrate SIMT and SIMD hybrid operator development: SIMD DataCopy is used for data movement, SIMD is used for float computation, and SIMT is used for int32_t computation. | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_high_performance](./simt_and_simd_high_performance) |  This sample uses FloorMod as an example to introduce performance tuning in SIMD and SIMT hybrid programming scenarios, showing the performance differences among direct SIMT access to GM, SIMD RegBase computation, SIMT access to UB, and memory coalescing optimization within a warp. | Ascend 950PR/Ascend 950DT |
| [simt_gather_with_ub](./simt_gather_with_ub) |  This sample uses Gather as an example to demonstrate how UB improves discrete memory access efficiency in SIMD and SIMT hybrid programming, comparing direct SIMT access to GM with accessing data after preloading it to UB. | Ascend 950PR/Ascend 950DT |
