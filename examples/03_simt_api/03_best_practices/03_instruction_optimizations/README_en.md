# Instruction Optimizations Sample Introduction

## Overview

Instruction optimization samples, implemented through direct `<<<>>>` invocation, introduce instruction tuning approaches based on SIMT programming. Currently, an atomic operation instruction optimization case is provided, demonstrating optimization methods such as hierarchical reduction and instruction cost control to improve instruction execution efficiency.

## Sample List

| Directory Name                                              | Description                                                                                        |
| ------------------------------------------------------ | ----------------------------------------------------------------------------------------------- |
| [atomic_histogram](./atomic_histogram)   | Using histogram counting as an example, this sample compares GM global atomic accumulation with UB block-local atomic accumulation followed by merge, demonstrating tuning methods and performance gains for atomic operation instructions. |
| [vector_muladd_atomic](./vector_muladd_atomic)   | Using vector multiply-add as an example, this sample compares in-kernel read-modify-write with atomic add, demonstrating the tuning method and performance gain of using atomic operations to offload accumulation to the memory side and eliminate one main-memory read. |