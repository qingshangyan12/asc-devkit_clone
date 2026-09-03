# Matrix Compute Tensor API Samples

## Overview

This directory contains Tensor API samples for matrix computation. They demonstrate data movement among GM, L1, L0A, L0B, L0C, and UB, as well as `mmad` matrix multiply-accumulate, convolution, and quantization workflows.

## Sample List

| Directory | Description | Supported Products |
| --- | --- | --- |
| [matmul](./matmul) | Implements matrix multiplication with the Tensor API static programming model. | Ascend 950PR/Ascend 950DT |
| [copy_in](./copy_in) | Implements dynamic-shape matrix multiplication with Bias and demonstrates GM-to-L1 and L1-to-L0 copy-in interfaces together with `mmad`. | Ascend 950PR/Ascend 950DT |
| [copy_out](./copy_out) | Implements dynamic-shape matrix multiplication with Bias and demonstrates L0C-to-GM and L0C-to-UB copy-out interfaces together with `mmad`. | Ascend 950PR/Ascend 950DT |
| [batch_matmul](./batch_matmul) | Implements Batch Matmul with Bias by using the Tensor API. | Ascend 950PR/Ascend 950DT |
| [conv2d_forward](./conv2d_forward) | Implements Conv2D Forward with the Tensor API and supports NC1HWC0, NCHW, and NHWC GM input/output formats. | Ascend 950PR/Ascend 950DT |
| [mmad](./mmad) | Implements dynamic-shape matrix multiplication and on-the-fly quantization with the Tensor API. | Ascend 950PR/Ascend 950DT |
| [matmul_mxfp4_high_performance](./matmul_mxfp4_high_performance) | Implements MxFP4 Matmul with static Tensor programming and demonstrates multi-core partitioning, bulk data movement, and double-buffered pipelines. | Ascend 950PR/Ascend 950DT |
