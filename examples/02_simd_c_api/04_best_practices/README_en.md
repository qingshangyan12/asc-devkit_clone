# BestPractices Sample Introduction

## Overview

This directory provides performance optimization practices based on the Ascend C C API. Each sample includes baseline and optimized scenarios with identical computation specifications to demonstrate how buffer design and pipeline synchronization affect performance.

## Sample List

| Directory Name | Description |
| --- | --- |
| [00_vector_compute](./00_vector_compute) | Vector computation optimization samples that demonstrate double-buffer pipelining across GM/UB data movement and Vector computation. |
| [01_matrix_compute](./01_matrix_compute) | Matrix computation optimization samples that demonstrate double-buffer pipelining across GM, L1, L0, and Cube computation. |
