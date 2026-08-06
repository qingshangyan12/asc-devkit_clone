# Matrix Compute Practices Sample Introduction

## Overview

This directory provides Matrix computation performance optimization samples based on the Ascend C C API, focusing on pipelining GM, L1, L0, and Cube computation.

## Sample List

| Directory Name | Description | Supported Products |
| --- | --- | --- |
| [matmul_basic_api_high_performance](./matmul_c_api_high_performance) | A Matmul C-API performance sample that compares a single-buffer baseline with L1/L0 Ping-Pong double buffering under identical matrix specifications to demonstrate MTE2, MTE1, and Cube pipeline overlap. | Ascend 950PR/Ascend 950DT |
