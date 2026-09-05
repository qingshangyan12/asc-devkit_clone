# Memory Vector Compute C API Examples

## Overview

This directory contains multiple C API examples for Memory vector compute. Each example uses Ascend C `<<<>>>` direct invocation and supports implementing the `main` function and `kernel` function in the same `.asc` file.

## Operator Development Examples

| Directory Name | Description |
| --- | --- |
| [cast](./cast) | This example implements half-to-packed-int4 or int32 conversion with Memory vector compute interfaces, primarily calling `asc_half2int4` and `asc_half2int32` |
| [compare](./compare) | This example performs vector and scalar comparison with Memory vector compute interfaces, primarily calling `asc_lt`, `asc_gt_scalar`, and `asc_get_cmp_mask` |
| [element_wise_arithmetic](./element_wise_arithmetic) | This example implements Leaky ReLU with Memory vector compute interfaces, primarily calling `asc_leakyrelu` |
| [reduce](./reduce) | This example implements vector sum reduction with Memory vector compute interfaces, primarily calling `asc_repeat_reduce_sum` and `asc_datablock_reduce_sum` |
