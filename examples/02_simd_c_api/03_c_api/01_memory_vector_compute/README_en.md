# Memory Vector Compute C API Examples

## Overview

This directory contains Memory vector compute C API examples. Each example uses Ascend C `<<<>>>` direct invocation, with the host function and kernel function implemented in one `.asc` file, and follows the GM-UB-compute-GM data flow.

Unlike `02_reg_vector_compute`, which uses Reg C APIs with register operands, this directory uses Memory vector compute C APIs. The examples support Atlas A2 Training/Inference and Atlas A3 Training/Inference products (`dav-2201`) only; `dav-3510` is unsupported.

## Operator Development Examples

| Directory Name | Description |
| --- | --- |
| [cast](./cast) | This example implements half-to-packed-int4 or int32 conversion with Memory vector compute C APIs, primarily calling `asc_half2int4` and `asc_half2int32` |
| [compare](./compare) | This example performs vector and scalar comparison with Memory vector compute C APIs, primarily calling `asc_lt`, `asc_lt_scalar`, `asc_gt_scalar`, and `asc_get_cmp_mask` |
| [element_wise_arithmetic](./element_wise_arithmetic) | This example implements Leaky ReLU with the Memory vector compute C API, primarily calling `asc_leakyrelu` |
| [reduce](./reduce) | This example implements vector sum reduction with the Memory vector compute C API, primarily calling `asc_repeat_reduce_sum` |
