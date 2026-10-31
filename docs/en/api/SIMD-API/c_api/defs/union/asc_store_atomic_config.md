# asc_store_atomic_config

<!-- md-trans-meta sourceCommit=462c9a712209ad24f2d57d4b5f5dd0d416050b06 translatedAt=2026-10-23T00:08:58.570Z pushedAt=2026-10-28T12:47:03.120Z nl=2 -->

The asc_store_atomic_config parameter contains the atomic operation enable bit and the atomic operation type, and is used by the [asc_get_store_atomic_config](../../atomic/scalar_atomic/asc_get_store_atomic_config.md) API.

## Struct Definition

```cpp
constexpr uint64_t ASC_STORE_ATOMIC_DEFAULT_VALUE = 0x0000000000000000;
union asc_store_atomic_config {
    uint64_t config = ASC_STORE_ATOMIC_DEFAULT_VALUE;
    struct {
        uint64_t atomic_type : 3;
        uint64_t atomic_op : 2;
        uint64_t reserved1 : 59;
    };
};
```

## Field Details

**Table 1** Field description

|field name|field meaning|
|----------|----------|
| atomic_type | Atomic operation enable bit. Default value: 0.<br>0: No atomic operation.<br>1: Enable the atomic operation, with the data type float.<br>2: Enable the atomic operation, with the data type half.<br>3: Enable the atomic operation, with the data type int16_t.<br>4: Enable the atomic operation, with the data type int32_t.<br>5: Enable the atomic operation, with the data type int8_t.<br>6: Enable the atomic operation, with the data type bfloat16_t.<br>Other values have no specific meaning.|
| atomic_op | Atomic operation type. Default value: 0.<br>0: Sum operation.<br>Other values have no specific meaning. |
