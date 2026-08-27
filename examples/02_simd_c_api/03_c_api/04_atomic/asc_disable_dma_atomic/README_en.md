# Disable DMA Atomic with C API

## Overview

This example verifies that `asc_disable_dma_atomic` disables pipelined atomic operations for data movement. It enables atomic add, performs one copy, disables atomic mode, and performs a normal copy.

## Supported Products

- Atlas A2 training series products/Atlas A2 inference series products

## Build and Run

```bash
./run.sh
```

The expected output is:

```text
[Success] asc_disable_dma_atomic verification passed.
```
