# Matrix Computation Optimization Sample Introduction

## Overview

This directory contains optimization samples based on Cube matrix computation pipelines.

## Sample List

| Directory Name | Description |
| --- | --- |
| [00_mmad_double_buffer](./00_mmad_double_buffer) | A Matmul C-API performance sample that compares a single-buffer baseline with L1 double buffering and demonstrates cross-K-iteration overlap among MTE2, MTE1, and Cube. |
| [01_mmad_direction](./01_mmad_direction) | An Mmad computation direction optimization sample that demonstrates how the Mmad direction affects performance when UnitFlag is enabled and compares performance under different direction settings across multiple scenarios. |
| [03_matmul_mx](./03_matmul_mx) | This sample implements a high-performance MXFP8 Matmul kernel using the C API, L1/L0 double buffering, large-block data transfers, and pipeline synchronization. |
