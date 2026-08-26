# asc_store_l2_cache_mode

asc_store_l2_cache_mode用于表示数据从Unified Buffer（UB）搬运到GM时的L2 cache策略，在调用[asc_copy_ub2gm_align](../vector_data_move/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md)时使用。

## 枚举类具体定义

```cpp
enum class asc_store_l2_cache_mode : uint8_t {
    NORMAL_FIRST_VICTIM = 0,
    NORMAL_LAST_VICTIM = 1,
    NORMAL_PERSISTENT = 2,
    NOTALLOC_CLEAN = 4
};
```

> **类型说明**：`asc_store_l2_cache_mode` 的底层类型为 `uint8_t`（枚举值即 `uint8_t` 数值）。在 CANN 9.1.0 / dav-3510 平台下，使用本枚举的接口（如 `asc_copy_ub2gm_align`）中 `l2_cache_mode` 参数的实际类型为 `uint8_t`，请按 `uint8_t` 传参。可传入本枚举的枚举值（底层为 `uint8_t`，值一致），也可直接传入枚举值对应的 `uint8_t` 数值（0、1、2、4）。

## 枚举值详解

**表1** 枚举值说明

|枚举值|L2 cache策略|
|------|-----------|
| NORMAL_FIRST_VICTIM | 启用L2 Cache，并且将分配的Cache Line标记为高替换优先级。 |
| NORMAL_LAST_VICTIM | 启用L2 Cache，并且将分配的Cache Line标记为低替换优先级。 |
| NORMAL_PERSISTENT | 启用L2 Cache。已存入L2 Cache中的数据可能被替换，若需确保特定数据始终保留在L2 Cache中，可采用驻留模式。<br>&bull; 注意，被标记为驻留模式的Cache Line只能被其他同样被标记为驻留模式的Cache Line替换。 |
| NOTALLOC_CLEAN | 不启用L2 Cache，若L2 Cache中已有同地址缓存会被保留并标记为Clean，标记为Clean的Cache Line被移出L2 Cache时会被直接丢弃。 |
