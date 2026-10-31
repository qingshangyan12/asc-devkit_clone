# asc_load_image_to_cbuf_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:17:01.743Z pushedAt=2026-10-28T12:55:35.709Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/composite/cube_datamove_composite.h"`.

**The `asc_load_image_to_cbuf_sync` API is deprecated. Use the [corresponding asynchronous API](../cube_datamove/asc_load_image_to_cbuf.md) and the synchronization API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Moves image data from Global Memory to the L1 Buffer.

## Prototype

- Synchronous move

    ```cpp
    __aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
    __aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Start address of the destination operand (vector). |
| hor_size | Input | Horizontal width of the image loaded from the source image, in pixels. Value range: [1, 4094]. |
| ver_size | Input | Vertical height of the image loaded from the source image, in pixels. Value range: [1, 4094]. |
| hor_start_pos | Input | Horizontal start address of the loaded image on the source image, in pixels. Value range: [0, 4095]. |
| ver_start_pos | Input | Vertical start address of the loaded image on the source image, in pixels. Value range: [0, 4095]. |
| src_hor_size | Input | Horizontal width of the source image, in pixels. Value range: [1, 4094]. |
| top_pad_size | Input | Number of pixels padded at the top of the destination image. Value range: [0, 32]. |
| bot_pad_size | Input | Number of pixels padded at the bottom of the destination image. Value range: [0, 32]. |
| left_pad_size | Input | Number of pixels padded on the left of the destination image. Value range: [0, 32]. |
| right_pad_size | Input | Number of pixels padded on the right of the destination image. Value range: [0, 32]. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).

## Examples

```cpp
constexpr uint64_t total_length = 128;    // total_length is the total length of the data to be moved.
__cbuf__ half dst[total_length];
// When loading the image, both the height and width are 1, the start address is (0,0), the horizontal width of the source image is 1, and no padding is applied.
asc_load_image_to_cbuf_sync(dst, 1, 1, 0, 0, 1, 0, 0, 0, 0);
```
