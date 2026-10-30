# asc_load_l2_cache_mode

<!-- md-trans-meta sourceCommit=38d8dc73ccb5198f19afb76cd3933136d51881f5 translatedAt=2026-10-23T00:02:51.337Z pushedAt=2026-10-28T12:47:03.095Z nl=2 -->

asc_load_l2_cache_mode specifies the L2 cache management policy for moving data from GM to Unified Buffer (UB). It applies to the following:

- Vector data movement APIs, such as [asc_copy_gm2ub_align](../../vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md) and [asc_ndim_copy_gm2ub](../../vector_datamove/asc_ndim_copy_gm2ub.md).
- Load policy configuration and query on the scalar memory access path, such as [asc_set_scalar_cache_mode](../../cache_ctrl/asc_set_scalar_cache_mode.md) and [asc_get_scalar_load_cache_mode](../../cache_ctrl/asc_get_scalar_load_cache_mode.md).

For the impact of the L2 cache management policy, see [L2 Cache Mode Best Practices](../../../../../../../examples/02_simd_c_api/02_features/00_data_movement/00_set_l2_cache_mode/README.md).

## Enum Class Definition

```cpp
enum class asc_load_l2_cache_mode : uint8_t {
    NORMAL_FIRST_VICTIM = 0,
    NORMAL_LAST_VICTIM = 1,
    NORMAL_PERSISTENT = 2,
    NOTALLOC_KEEP = 4,
    NOTALLOC_CLEAN = 5,
    NOTALLOC_DROP = 6
};
```

## Enum Value Details

**Table 1** Enum value description

|Enum Value|L2 Cache Management Policy|
|------|-----------|
| NORMAL_FIRST_VICTIM | Enables the L2 cache and marks the allocated cache lines with high replacement priority. |
| NORMAL_LAST_VICTIM | Enables the L2 cache and marks the allocated cache lines with low replacement priority. |
| NORMAL_PERSISTENT | Enables the L2 cache. Data already stored in the L2 cache may be replaced. To ensure that specific data always remains in the L2 cache, use the persistent mode.<br>&bull; Note that a cache line marked as persistent can only be replaced by another cache line that is also marked as persistent. |
| NOTALLOC_KEEP | Disables the L2 cache, reads directly from GM each time, and keeps the state of existing cache lines unchanged. |
| NOTALLOC_CLEAN | Disables the L2 cache. If a cache entry with the same address already exists in the L2 cache, it is retained and marked as Clean. A cache line marked as Clean is discarded directly when evicted from the L2 cache. |
| NOTALLOC_DROP | Disables the L2 cache. If a cache entry with the same address already exists in the L2 cache, it is discarded directly. |
