# Memory Optimizations Example Introduction

## Overview

Using memory access optimization with direct `<<<>>>` invocation, this section introduces memory access tuning approaches based on  SIMT programming, including global memory access coalescing, on-chip cache reuse (e.g., UB), Bank conflict avoidance, and improving memory bandwidth utilization via short vector such as half2.

## Example List

| Directory Name                                           | Description |
| -------------------------------------------------------- | ----------- |
| [matrix_transpose_practice](./matrix_transpose_practice) | This example uses matrix transpose to demonstrate the memory access tuning path in SIMT programming, starting from direct-index transpose and progressively introducing UB staging, global memory coalescing, thread block mapping and thread count selection, UB bank conflict avoidance through padding, and double buffering to remove trailing synchronization. |
| [short_vector_add](./short_vector_add)                   | Taking vector addition as an example, this example compares element-wise addition of the basic half type with short-vectorized half2 addition, demonstrating the tuning method of improving memory bandwidth utilization based on short vectors in SIMT programming. |
| [cache_hint](./cache_hint)                               | Taking sin computation via table lookup as an example, this example specifies cache policies for different types of data so that the frequently accessed sin lookup table stays resident in the DCache and the number of Global Memory accesses is reduced, demonstrating the data cache optimization tuning path in SIMT programming. |
| [aligned_types](./aligned_types)                         | This example, based on Ascend C SIMT programming, demonstrates the impact of struct type alignment on Global Memory access throughput. |
| [hashtable_lookup](./hashtable_lookup)                   | This example implements a hash table lookup operator whose hash table resolves hash collisions based on open addressing. It looks up the hash table through different hash table structure designs and thread access patterns and compares their performance, demonstrating the impact of different optimization methods on performance. |
