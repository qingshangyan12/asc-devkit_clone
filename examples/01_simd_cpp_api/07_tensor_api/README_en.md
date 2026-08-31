# Tensor API Sample Introduction

## Overview

This directory contains samples that directly include `tensor_api/tensor.h` and use the Tensor API programming model. The samples cover data movement, matrix computation, convolution, and high-performance matrix multiplication practices.

## Sample List

| Directory Name | Description | Supported Products |
| --- | --- | --- |
| [matmul](./matrix_compute/matmul) | Implements matrix multiplication using the Tensor API static programming model | Ascend 950PR/Ascend 950DT |
| [copy_in](./matrix_compute/copy_in) | Implements copy-in interfaces and dynamic-shape matrix multiplication with Bias using the Tensor API, demonstrating GM-to-L1 and L1-to-L0 data movement and mmad computation | Ascend 950PR/Ascend 950DT |
| [copy_out](./matrix_compute/copy_out) | Implements copy-out interfaces and dynamic-shape matrix multiplication with Bias using the Tensor API, demonstrating L0C-to-GM and L0C-to-UB data movement and mmad computation | Ascend 950PR/Ascend 950DT |
| [batch_matmul](./matrix_compute/batch_matmul) | Implements Batch Matmul with Bias using the Tensor API programming model | Ascend 950PR/Ascend 950DT |
| [conv2d_forward](./matrix_compute/conv2d_forward) | Implements Conv2D Forward using the Tensor API and supports NC1HWC0, NCHW, and NHWC GM input/output formats | Ascend 950PR/Ascend 950DT |
| [mmad](./matrix_compute/mmad) | Implements dynamic-shape matrix multiplication and in-path quantization using the Tensor API | Ascend 950PR/Ascend 950DT |
| [matmul_mxfp4_high_performance](./matrix_compute/matmul_mxfp4_high_performance) | High-performance MxFP4 Matmul Tensor API sample based on the static Tensor programming model | Ascend 950PR/Ascend 950DT |
