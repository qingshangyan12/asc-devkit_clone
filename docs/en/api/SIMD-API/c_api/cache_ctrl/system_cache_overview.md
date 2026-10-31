# System Cache Overview

<!-- md-trans-meta sourceCommit=38d8dc73ccb5198f19afb76cd3933136d51881f5 translatedAt=2026-10-21T03:00:41.823Z pushedAt=2026-10-28T12:21:43.281Z nl=2 -->

## Cache Type

The main purpose of Cache is to provide a high-speed buffer between the move unit or Scalar unit and external storage, reducing data access latency and improving bandwidth utilization. Typically, frequently accessed data is written to Cache, and the move unit or Scalar unit prefers to read data from Cache during execution. On a Cache miss, data is loaded from external storage and updated into Cache.

[Table 1](#table_cache_type) shows the functions of different Cache types, where Data Cache (DCache) and Instruction Cache (ICache) are used for caching data and instructions, respectively. The Cache types supported vary by product, as follows:
<!-- npu="950" id1 -->
Taking [NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch) as an example, Figure 1 shows the positional relationships of the five Cache types supported in the AI Core (L2 Cache, DCache, ICache, SIMT DCache, and NDDMA Cache) within the hardware architecture.

**Figure 1**  Positional relationships of the five Cache types in the AI Core    
![](../../../figures/npu_3510_hw_arch_cache.png "atlas_950_cache_architecture_diagram")
<!-- end id1 -->
<!-- npu="A3,910b" id2 -->
Taking [NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch) as an example, Figure 2 shows the positional relationships of the three Cache types supported in the AI Core (L2 Cache, DCache, and ICache) within the hardware architecture.

**Figure 2**  Positional relationships of the three Cache types in the AI Core    
![](../../../figures/atlas_a2_a3_cache_architecture_diagram.png "atlas_a2_a3_cache_architecture_diagram")
<!-- end id2 -->

**Table 1**  Cache types and function descriptions<a name="table_cache_type"></a>

| Cache Type | Description |
|------------|------|
| L2 Cache | As a level-2 cache, L2 Cache is dedicated to storing frequently accessed data and instructions to reduce reads and writes to GM.<br>&bull;When reading GM through the MTE2 unit, data is preferentially read from L2 Cache. On an L2 Cache miss, data is loaded from GM and updated into L2 Cache.<br>&bull;When reading GM through the Scalar unit, data is preferentially read from DCache. On a DCache miss, data is read from L2 Cache. When L2 Cache also misses, data is loaded from GM and updated into both L2 Cache and DCache.<br>&bull;When reading GM instructions through the Scalar unit, instructions are preferentially read from ICache. On an ICache miss, instructions are read from L2 Cache. When L2 Cache also misses, instructions are loaded from GM and updated into both L2 Cache and ICache. |
| DCache | DCache is used for caching data segments that the Scalar unit may repeatedly access in the near future.<br>When reading GM through the Scalar unit, data is preferentially read from DCache. On a DCache miss, data is read from L2 Cache. When L2 Cache also misses, data is loaded from GM and updated into both L2 Cache and DCache. |
| ICache | ICache is used for caching instructions most recently or frequently used by the Scalar unit.<br>When reading GM instructions through the Scalar unit, instructions are preferentially read from ICache. On an ICache miss, instructions are read from L2 Cache. When L2 Cache also misses, instructions are loaded from GM and updated into both L2 Cache and ICache. |
| SIMT DCache | SIMT access to GM must be relayed through SIMT DCache. The data flow goes from GM to SIMT DCache, and then from SIMT DCache to SIMT registers. SIMT DCache acts as an intermediate buffer layer, reducing the number of direct accesses to GM. |
| NDDMA Cache | NDDMA Cache is used for caching data that is recently or about to be moved by the [asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md) API. |

For hardware specifications such as the Cache Line size, Cache size, and location of each Cache, see [Cache Coherence Cache Unit Information](../../../../guide/programming_guide/advanced_programming/memory_model/cache_coherence.md#cache_unit_info).

## Cache Operations and Usage Scenarios

Table 2 describes the four basic cache operations and their core functions.

**Table 2**  Cache operations and function descriptions

| Operation | Description |
|---------|---------|
| Prefetch | The hardware automatically loads data that is expected to be accessed into the cache in advance based on the access pattern, improving the speed of subsequent accesses. |
| Preload | The software proactively loads specified data into the cache through explicit instructions, preparing for upcoming read and write operations. |
| Invalid | &bull;Marks the Cache Line (the smallest unit of the cache operation) in the specified address range as "invalid", removing it from the cache.<br>&bull;Ensures that the next access to these memory addresses reloads data from GM instead of using potentially stale cached data.<br>&bull;Note: The Invalid operation does not check whether a cache line is "dirty" (dirty, indicating that the data has been modified but not yet written back to GM). If dirty data exists, it is deprecated directly. |
| Clean | Writes back the modified data (dirty data) in the cache to GM to prevent data loss. The Clean operation does not mark the cache line as "invalid"; the cache line remains valid. |

## Cache Write Policies and Cache Consistency Issues

When data is written to GM with Cache enabled, there are two Cache write policies: Write-Back and Write-Through.

When data is written to GM using the Write-Back policy, the working principle is as follows:

- You expect to write data to GM (that is, expect the data to be written directly to GM), but the actual hardware behavior is:
    - When data is modified, it is updated only in the Cache.
    - The data in the Cache is marked as "dirty".
    - Only when the Cache Line is replaced (or when a Clean operation is explicitly performed by software) is the data written back to GM. When the Cache Line is replaced is determined by the hardware.

- When replacing Cache Line data:
    - When Cache space is insufficient and data needs to be replaced, check whether the data to be replaced is "dirty".
    - If it is "dirty" data, write it back to GM first, and then replace the Cache Line out of the Cache.

For data writing to GM, if the write-through strategy is adopted, its working principle is as follows:

- You expect to write data to GM (that is, the data is expected to be written directly to GM), but the actual hardware behavior is:
    - When data is modified, the data is updated in both the cache and GM immediately.
    - The cache line does not need to be marked as "dirty".

- When replacing cache line data:

    Since every write synchronously updates GM, the data in the cache does not have a "dirty" state, and there is no need to additionally write back to GM during replacement.

Under the write-back strategy, modified data is updated only in the local cache and is not immediately synchronized to GM. When other cores read the same address on GM, they obtain stale data, resulting in inconsistent cache data across different cores.
Cache coherence is the core mechanism for ensuring data correctness in multi-core systems. In simple terms, when multiple cores each have their own independent cache, they may cache the same data simultaneously. If one core modifies the data in its cache, the cache copies in other cores become "stale". If they are not synchronized in time, data incoherence across multiple cores will occur, which in turn leads to computation errors.

<!-- npu="950,A3,910b" id3 -->
The following describes whether each type of cache in different products needs to consider inter-core data incoherence:
<!-- npu="950" id4 -->
- For [NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch), the five types of caches supported by this product are as follows in terms of inter-core data coherence:
    - L2 Cache: A cache shared among multiple cores, so there is no need to consider inter-core data incoherence.
    - DCache: A multi-core independent cache, so data coherence across multiple cores must be considered.
    - ICache: Read-only, so data incoherence across multiple cores does not need to be considered.
    - NDDMA Cache: A multi-core independent cache, so data coherence across multiple cores must be considered.
    - SIMT DCache: In SIMT, when data is written, it is immediately written to GM, so there is no coherence issue; when data is read from GM, the multi-core independent cache requires consideration of data coherence across multiple cores.
<!-- end id4 -->
<!-- npu="A3,910b" id5 -->
- For [NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch), the data coherence across multiple cores for the three supported Cache types is as follows:
    - L2 Cache: A cache shared across multiple cores, so data incoherence across multiple cores does not need to be considered.
    - DCache: A multi-core independent cache, so data coherence across multiple cores must be considered.
    - ICache: Read-only, so data incoherence across multiple cores does not need to be considered.
<!-- end id5 -->
<!-- end id3 -->

For a more complete introduction to Cache write policies and Cache coherence, see [Cache Coherence](../../../../guide/programming_guide/advanced_programming/memory_model/cache_coherence.md).

## Cache Control API Summary

Table 3 summarizes the APIs related to cache control and their function descriptions by Cache type (L2 Cache, DCache, ICache, and NDDMA Cache).

**Table 3**  Cache control API summary

| Cache Type | API | Function Description |
|-----------|---------|---------|
| L2 Cache | [asc_copy_gm2ub_align](../vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md) (data movement, read GM) | Taking this API as an example, it is a data movement API that reads data from GM. You can use the [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md) enum as an input parameter to control the L2 Cache mode. Some APIs in [cube data movement](../cube_datamove/cube_datamove.md) and [vector data movement](../vector_datamove/vector_datamove.md) support this input parameter. For details, see the parameter description of each data movement API. |
| L2 Cache | [asc_copy_ub2gm_align](../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md) (data movement, write GM) | Taking this API as an example, it is a data movement API that writes data to GM. You can use the [asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md) enum as an input parameter to control the L2 Cache mode. Some APIs in [cube data movement](../cube_datamove/cube_datamove.md) and [vector data movement](../vector_datamove/vector_datamove.md) support this input parameter. For details, see the parameter description of each data movement API. |
| DCache | [asc_datacache_preload](asc_datacache_preload.md) | The `asc_datacache_preload` API preloads data from the specific GM address where the source address resides into the DCache. Each call preloads only one Cache Line of data. |
| DCache | [asc_dcci](asc_dcci.md) | When the Scalar unit accesses GM, the Cache is refreshed according to the API operation type to ensure coherence. When reading data that may be modified by other cores, or when Scalar writes must be immediately visible to others, you need to select clean, invalidate, or clean-and-invalidate operations based on the access order. |
| DCache | [asc_load_dev](../scalar_compute/scalar_load/asc_load_dev.md)/[asc_store_dev](../scalar_compute/scalar_store/asc_store_dev.md) | `asc_load_dev` reads data from a GM address without going through the DCache; `asc_store_dev` writes data to a GM address without going through the DCache. When multiple cores operate on a GM address, if the data cannot be aligned to a Cache Line, reads and writes through the DCache are performed at Cache Line granularity, which may cause random data overwriting across cores. Using these two APIs to directly read from and write to GM without going through the DCache avoids this issue. |
| ICache | [asc_icache_preload](asc_icache_preload.md) | You can manually call `asc_icache_preload` to preload instructions from the GM address where the instructions reside into the ICache. |
| ICache | [asc_get_icache_preload_status](asc_get_icache_preload_status.md) | `asc_get_icache_preload_status` is a debugging API. Call it after `asc_icache_preload` to obtain the ICache Preload state. A return value of 0 indicates that the ICache Preload is complete; a return value of 1 indicates that the ICache Preload is not complete. |
| NDDMA Cache | `asc_ndim_copy_dci` | Before using the [asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md) API for data movement, refresh the NDDMA Cache according to the API constraints to ensure that the Cache is in the latest state. |
