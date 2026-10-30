# asc_store_l2_cache_mode

<!-- md-trans-meta sourceCommit=38d8dc73ccb5198f19afb76cd3933136d51881f5 translatedAt=2026-10-23T00:04:57.294Z pushedAt=2026-10-28T12:47:03.106Z nl=2 -->

asc_store_l2_cache_mode indicates the L2 cache policy used when data is copied from the Unified Buffer (UB) to GM. It can be used for:

- Vector copy APIs, for example, [asc_copy_ub2gm_align](../../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md).
- Store policy configuration and query on the scalar memory access path, for example, [asc_set_scalar_cache_mode](../../cache_ctrl/asc_set_scalar_cache_mode.md) and [asc_get_scalar_store_cache_mode](../../cache_ctrl/asc_get_scalar_store_cache_mode.md).

## Enum Class Definition

```cpp
enum class asc_store_l2_cache_mode : uint8_t {
    NORMAL_FIRST_VICTIM = 0,
    NORMAL_LAST_VICTIM = 1,
    NORMAL_PERSISTENT = 2,
    NOTALLOC_CLEAN = 4
};
```

## Enum Value Details

**Table 1** Enum value description

|Enum Value|L2 Cache Policy|
|------|-----------|
| NORMAL_FIRST_VICTIM | Enables the L2 cache and marks the allocated cache lines with high replacement priority. |
| NORMAL_LAST_VICTIM | Enables the L2 cache and marks the allocated cache lines with low replacement priority. |
| NORMAL_PERSISTENT | Enables the L2 cache. Data already stored in the L2 cache may be replaced. To ensure that specific data always remains in the L2 cache, use the resident mode.<br>&bull; Note that a cache line marked as resident can only be replaced by another cache line that is also marked as resident. |
| NOTALLOC_CLEAN | Does not enable the L2 cache. If a cache entry with the same address already exists in the L2 cache, it is retained and marked as clean. A cache line marked as clean is discarded directly when evicted from the L2 cache. |
