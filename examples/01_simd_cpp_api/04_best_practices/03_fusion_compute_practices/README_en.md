# Fusion Compute Practices Sample Introduction

## Overview

This directory provides best-practice samples for fusion compute, covering QuantGroupMatmul grouped quantized matrix multiplication, Cube-Vector fusion for Matmul+GELU, and high-performance optimization based on SIMT and SIMD hybrid programming.

## Sample List

| Directory Name | Description |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [grouped_matmul](./grouped_matmul) | Introduces a high-performance QuantGroupMatmul implementation, supporting grouped quantized matrix multiplication and GELU activation. |
| [matmul_gelu_high_performance](./matmul_gelu_high_performance) | Demonstrates a high-performance Cube-Vector fusion implementation that runs Matmul and GELU activation in parallel on the same AI Core. |
| [simt_and_simd_high_performance](./simt_and_simd_high_performance) | Uses FloorMod as an example to demonstrate performance tuning in SIMT and SIMD hybrid programming scenarios. |
