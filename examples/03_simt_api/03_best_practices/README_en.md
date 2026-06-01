# BestPractices Example Introduction

## Overview

Operator development practices based on Ascend C SIMT, demonstrating the application of SIMT features in actual operator development.

## Example List

| Directory Name                                              | Description                                                                                        |
| ------------------------------------------------------ | ----------------------------------------------------------------------------------------------- |
| [00_memory_optimizations](./00_memory_optimizations)   | Introduce memory access optimization methods based on SIMT programming, including global memory access coalescing, on-chip cache reuse, bank conflict avoidance, and short vector tuning. |
| [02_simt_grid_config](./02_simt_grid_config)   | The sample uses Gather+Adds fusion computation as an example to introduce gridDim configuration and vf function call optimization strategies under the Ascend C SIMD and SIMT hybrid programming model. |