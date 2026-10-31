# L1 and Fixpipe Buffer Data Migration

<!-- md-trans-meta sourceCommit=7f211052f814e6d340cc6bf2dfc44c8393b5424f translatedAt=2026-09-23T14:01:57.917Z pushedAt=2026-10-26T07:41:44.264Z nl=2 -->

## DataCopy (L1ToFixpipe Buffer Continuous Data Migration)<a name="ZH-CN_TOPIC_0000002543262934"></a>

DataCopy enables continuous data movement from the L1 Buffer to the Fixpipe Buffer, with the format and content remaining unchanged during movement.

The supported data paths are as follows (represented by [logical position TPosition](../../basic_api/aux_data_structures/TPosition.md)):

- L1 Buffer -> Fixpipe Buffer
    - C1 -> C2PIPE2GM

The migrated data consists of [on-the-fly quantization](../cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md) and [on-the-fly ReLU](../cube_compute_ISASI/cube_store_key_features/accompanying_relu.md) parameters. For details about the API, see [DataCopy (L1ToFixpipe Buffer Data Migration)](../cube_compute_ISASI/cube_compute_load/DataCopy_L1ToFixpipe.md).

**src** and **dst** are the source operand and destination operand, respectively; **count** is the number of elements to be transferred continuously.

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count);
```

## DataCopy (L1ToFixpipe Buffer High-Dimensional Slicing Data Migration)<a name="ZH-CN_TOPIC_0000002573902863"></a>

DataCopy enables continuous and non-continuous data movement from the L1 Buffer to the Fixpipe Buffer, with the format and content remaining unchanged during movement.

High-dimensional slicing refers to the ability to implement non-continuous data movement by configuring data movement parameters such as the number of data blocks, the length of a single data block, and address offsets.

The specifically supported data paths are as follows (represented by [logical position TPosition](../../basic_api/aux_data_structures/TPosition.md)):

- L1 Buffer -> Fixpipe Buffer
    - C1 -> C2PIPE2GM

The data consists of [on-the-fly quantization](../cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md) and [on-the-fly ReLU](../cube_compute_ISASI/cube_store_key_features/accompanying_relu.md) parameters. For details about the API, see [DataCopy (L1ToFixpipe Buffer Data Migration)](../cube_compute_ISASI/cube_compute_load/DataCopy_L1ToFixpipe.md).

**src** and **dst** are the source operand and destination operand, respectively; **repeatParams** is the movement parameter.

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```
