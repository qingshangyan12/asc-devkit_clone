# copy (Movement from Global Memory to Unified Buffer)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-27T10:53:52.088Z pushedAt=2026-10-27T13:04:46.153Z nl=2 -->

> [!NOTE]Description
> This is an experimental API and may be adjusted or improved in later versions. Developers should pay attention to updates in later versions when using it.

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- AI Core in Atlas inference products: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Vector Core in Atlas inference products: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `tensor_api/tensor.h`, and the namespace is `asc::te`.

The Tensor API uses the `copy` API to uniformly perform movement over different paths. This API copies data from Global Memory to the Unified Buffer (UB). Based on the storage locations of the source tensor and destination tensor, the API automatically selects the `copy_gm_to_ub` path, and derives the number of movement blocks, single-block length, source stride, and destination stride according to the layout, implementing contiguous or non-contiguous high-dimensional sliced movement.

Currently, ND2ND, DN2DN, NZ2NZ, and ZN2ZN copies are supported. The source and destination layout patterns must be the same. No format conversion is performed during the movement, and the data format and content remain unchanged.

In addition to whole-tensor movement, the API also supports specifying a local region through the destination coordinates, source coordinates, and movement shape. When left and right padding needs to be added on the destination side, it can be configured through `gm_to_ub_params` and bound to the movement atomic object using `copy_atom::with`.

## Function Prototype

- Automatically derive the path based on the storage locations of the source and destination tensors.

    ```cpp
    template <typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src)
    ```

- Use the movement atomic object to perform movement.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const copy_atom<Atom>& atom,
        const DstTensor& dst, const SrcTensor& src)
    ```

- Copy a specified region.

    ```cpp
    template <typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src,
        const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
    ```

- Construct the movement atomic object.

    ```cpp
    template <typename CopyOperation>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation)

    template <typename CopyOperation, typename CopyTrait>
    __aicore__ inline constexpr auto make_copy(
        const CopyOperation& operation, const CopyTrait& trait)
    ```

The Global Memory-to-UB path uses `copy_gm_to_ub{}` as the movement operation object, with the default trait being `gm_to_ub_trait_default{}`.

## Parameters

**Table 1** `copy` API parameters

| Parameter | Input/Output | Description |
| :--- | :---: | :--- |
| atom | Input | Movement atomic object, which can be constructed through `make_copy(copy_gm_to_ub{})` or `make_copy(copy_gm_to_ub{}, gm_to_ub_trait_default{})`. |
| dst | Output | Destination tensor, whose storage location must be `location::ub`. |
| src | Input | Source tensor, whose storage location must be `location::gm`. |
| dst_coord | Input | Start coordinate of the destination region. The structure of the explicit coordinate must be consistent with `copy_shape`, and `zero_coord` can also be used. |
| src_coord | Input | Start coordinate of the source region. The structure of the explicit coordinate must be consistent with `copy_shape`, and `zero_coord` can also be used. |
| copy_shape | Input | Logical shape of the movement region. The API derives the address and movement parameters based on this shape and the source/destination layout. |

### Parameter Derivation

`copy` automatically derives the underlying multi-block movement parameters based on the source and destination Tensor layouts. In the following table, the single-block length and stride are all in bytes; the source and destination strides respectively represent the gap between the start addresses of adjacent data blocks.

| Layout Mode | Block Count Derivation | Single-Block Length Derivation | Source/Destination Stride Derivation |
| :--- | :--- | :--- | :--- |
| ND | Total number of rows in the Layout | Total number of columns in the Layout × element bytes | Row stride of the corresponding Tensor × element bytes |
| DN | Total number of columns in the Layout | Total number of rows in the Layout × element bytes | Column stride of the corresponding Tensor × element bytes |
| NZ | Outer column count of the fractal format | Total number of rows × inner column count × element bytes | Outer column stride of the corresponding Tensor × element bytes |
| ZN | Outer row count of the fractal format | Total column count × inner row count × element bytes | Outer row stride of the corresponding Tensor × element bytes |

- When the source and destination strides are both equal to the single-block length, the data blocks are arranged contiguously in the corresponding storage space.
- When the source or destination stride is greater than the single-block length, the API performs a non-contiguous movement; the gap is described by the Layout stride of the corresponding Tensor and does not need to be configured separately.

### gm_to_ub_params Description

```cpp
struct gm_to_ub_params {
    uint8_t left_padding_count = 0;
    uint8_t right_padding_count = 0;
    bool enable_constant_pad = true;
};
```

| Member | Default Value | Description |
| :--- | :---: | :--- |
| left_padding_count | `0` | Number of padding elements on the left side of each movement block. |
| right_padding_count | `0` | Number of padding elements on the right side of each movement block. |
| enable_constant_pad | `true` | Whether to enable constant padding control. |

Bind the runtime movement parameters in the following way:

```cpp
asc::te::gm_to_ub_params params{left_padding, right_padding, true};
auto atom = asc::te::make_copy(asc::te::copy_gm_to_ub{}).with(params);
asc::te::copy(atom, dst, src);
```

## Data Type

The data types of the source tensor and the destination tensor must be the same.

| Source/Destination Layout | Supported Data Types |
| :--- | :--- |
| ND, DN, NZ, ZN | `fp4x2_e1m2_t`, `fp4x2_e2m1_t`, `int8_t`, `uint8_t`, `hifloat8_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`, `int64_t` |

## Return Value

`copy` has no return value. `make_copy` returns a `copy_atom` object.

## Constraints

- The source tensor must reside in Global Memory, and the destination tensor must reside in UB.
- The source address supports 1-byte alignment, and the destination UB address must meet the 32-byte alignment requirement.
- The source and destination layout patterns must be the same. ND2ND, DN2DN, NZ2NZ, and ZN2ZN are supported, while on-the-fly format conversion is not supported.
- The capacity of the destination tensor or destination region must be no less than the space required by the data to be copied and the padding.
- The number of blocks, block length, and stride derived from the layout must satisfy the field range and alignment requirements of the underlying `asc_copy_gm2ub_align` instruction. The number of valid movement blocks ranges from [1, 4095], and the single-block length ranges from [1, 2097151] bytes. Unlike the `DataCopyParams` API on some products, the Tensor API requires that the source and destination layouts and the regional movement shape be valid, and it cannot rely on a block count or single-block length of 0 to represent a no-op.
- For multiple valid data blocks, the destination stride must be equal to the single-block length or be a positive integer multiple of 32 bytes. Both the source stride and the destination stride represent the gap between the start addresses of adjacent data blocks, rather than the inter-block gap in `DataCopyParams`.
- The left and right single-side Padding must not exceed 32 bytes. For the 8-byte element type, the Padding count is distributed at a 4-byte granularity.
- When the destination stride is equal to the single-block movement length, that is, in compact movement mode, both the left and right Padding must be 0.
- For the b4 type, the block length and stride are adjusted at the b8 movement granularity.

## Key Features

### Layout Automatic Derivation

The ND path generates movement blocks along the row direction, and the DN path generates movement blocks along the column direction. The NZ and ZN paths derive the number of blocks, block length, and stride based on the inner and outer axes of the fractal layout. The corresponding strides of the source and destination layouts determine whether the data blocks are arranged contiguously or with gaps. Users only need to ensure that the layouts of the source and destination tensors match correctly.

### Contiguous and Non-Contiguous Movement

When the number of blocks is 1, the source and destination strides do not affect the movement result. When the number of blocks is greater than 1, if both the source and destination strides are equal to the single-block length, adjacent data blocks are contiguous end to end, which is a contiguous movement; if either stride is greater than the single-block length, there is a gap between data blocks in the corresponding storage space, which is a non-contiguous movement.

Take an ND Tensor with element type `half` and movement shape `(2, 32)` as an example. The single-block length is `32 × sizeof(half) = 64` bytes.

| Scenario | Source Tensor Row Stride | Destination Tensor Row Stride | Derivation Result |
| :--- | :--- | :--- | :--- |
| Contiguous Movement | 32 elements | 32 elements | The number of blocks is 2, the single-block length is 64 bytes, and both the source and destination strides are 64 bytes. |
| Non-contiguous movement | 48 elements | 64 elements | The number of blocks is 2, the single-block length is 64 bytes, the source stride is 96 bytes, and the destination stride is 128 bytes. Between each pair of data blocks, the source data skips 32 bytes, and the destination data retains a 64-byte gap. |

During the copy, `copy` writes only the destination address range covered by each data block; the gaps produced by the destination tensor stride are not overwritten. The padding parameters also take effect per data block, and the destination tensor layout stride should include the space occupied by the padding.

### Partial Regional Movement

Region reload resolves the start address based on `dst_coord`, `src_coord`, and `copy_shape`. This method does not require constructing a Tensor slice first, making it suitable for copying a rectangular partial region from a large Tensor.

## Example

The following example copies ND data from Global Memory to the UB:

```cpp
#include "tensor_api/tensor.h"

constexpr uint32_t m = 8;
constexpr uint32_t n = 64;

__aicore__ inline void copy_gm_to_ub_example(__gm__ half* gm_addr)
{
    __ubuf__ half ub_buf[m * n];

    auto layout = asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn,
        asc::te::layout_trait_default<half>>(m, n);
    auto src = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::gm>(gm_addr), layout);
    auto dst = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(ub_buf), layout);

    asc::te::copy(dst, src);
}
```

The ND layout in this example contains 8 rows, each with 64 `half` elements. Therefore, the API derives 8 blocks and a single-block length of 128 bytes, with both the source stride and destination stride being 128 bytes. The data blocks are contiguous end to end, which is a contiguous movement.

The following example copies a partial region and explicitly constructs a movement atomic object:

```cpp
auto atom = asc::te::make_copy(asc::te::copy_gm_to_ub{}, asc::te::gm_to_ub_trait_default{});
asc::te::copy(atom, dst, src, asc::te::make_coord(1, 0), asc::te::make_coord(2, 0), asc::te::make_shape(2, 32));
```

This region contains 2 rows, with 32 `half` elements copied per row. Therefore, the API derives 2 blocks and a single-block length of 64 bytes. The row stride of both the source and destination Tensors is 128 bytes, so the source stride and destination stride are both 128 bytes, leaving a 64-byte gap between adjacent data blocks.
