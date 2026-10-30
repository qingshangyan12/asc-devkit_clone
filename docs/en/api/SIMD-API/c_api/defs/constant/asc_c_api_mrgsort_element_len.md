# ASC_C_API_MRGSORT_ELEMENT_LEN

<!-- md-trans-meta sourceCommit=462c9a712209ad24f2d57d4b5f5dd0d416050b06 translatedAt=2026-10-23T00:01:22.263Z pushedAt=2026-10-28T12:47:03.088Z nl=2 -->

ASC_C_API_MRGSORT_ELEMENT_LEN indicates the number of queues supported by the MrgSort4 operation, with a value of 4. This constant is used for the length of the src array in the [asc_mrgsort4](../../vector_compute/vector_sort/asc_mrgsort4.md) API and the length of the sorted_num array in the [asc_get_vms4_sr](../../vector_compute/vector_sort/asc_get_vms4_sr.md) API.

Header file path: `c_api/defs/constant.h`.

## Constant Definition

```cpp
constexpr uint8_t ASC_C_API_MRGSORT_ELEMENT_LEN = 4;
```
