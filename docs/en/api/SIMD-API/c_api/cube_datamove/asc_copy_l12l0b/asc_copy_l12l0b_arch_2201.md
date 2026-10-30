# asc_copy_l12l0b

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-28T13:34:56.862Z pushedAt=2026-10-29T02:01:15.750Z nl=2 -->

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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Copies a 512B cube stored in the L1 Buffer to the L0B Buffer. Supports both 2D format transfer and 3D format transfer.

## Prototype

- High-dimensional split transfer in 2D format
     ```cpp
    __aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

- High-dimensional split transfer in 3D format

    ```cpp
    __aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    ```

## Parameters

**Table 1** 2D format parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination L0B Buffer address. |
| src | Input | Source L1 Buffer address. |
| start_index | Input | Fractal cube ID, indicating that the copy starts from the specified fractal in the source operand (0 indicates the first fractal cube in the source operand). Value range: [0, 65535]. Unit: 512 bytes. |
| repeat | Input | Number of iterations, where each iteration processes 512B of data. Value range: [1, 255]. |
| src_stride | Input | Gap between the start addresses of the previous fractal and the next fractal in the source operand across adjacent iterations. Value range: [0, 65535]. Unit: 512 bytes. |
| dst_gap | Input | Gap between adjacent contiguous data blocks in the destination operand (the gap between the end of the previous data block and the start of the next data block). Value range: [0, 65535]. Unit: 512 bytes. |

**Table 2** 3D format parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination L0B Buffer address. |
| src | Input | Source L1 Buffer address. |
| k_extension | Input | Transfer length of this instruction in the width dimension of the destination operand. If the rightmost fractal is not covered, it must be a multiple of 16 for the half type and a multiple of 32 for the int8_t/uint8_t type; if the rightmost fractal is covered, there is no multiple requirement. Value range: [1, 65535]. |
| m_extension | Input | Transfer length of this instruction in the height dimension of the destination operand. If the bottommost fractal is not covered, it must be a multiple of 16 for the half/int8_t/uint8_t type; if the bottommost fractal is covered, there is no multiple requirement. Value range: [1, 65535]. |
| k_start_pt | Input | Start point of this instruction in the width dimension of the destination operand. It must be a multiple of 16 for the half type and a multiple of 32 for the int8_t/uint8_t type. Value range: [0, 65535] |
| m_start_pt | Input | Start point of this instruction in the height dimension of the destination operand. If the bottommost fractal is not covered, it must be a multiple of 16 for the half/int8_t/uint8_t type; if the bottommost fractal is covered, there is no multiple requirement. Value range: [0, 65535].|
| stride_w | Input | Stride of the convolution kernel sliding in the width dimension of the source operand. Value range: [1, 63]. |
| stride_h | Input | Stride of the convolution kernel sliding in the height dimension of the source operand. Value range: [1, 63]. |
| filter_w | Input | Width of the convolution kernel. Value range: [1, 255]. |
| filter_h | Input | Height of the convolution kernel. Value range: [1, 255]. |
| dilation_filter_w | Input | Dilation factor of the convolution kernel width. Value range: [1, 255]. |
| dilation_filter_h | Input | Dilation factor of the convolution kernel height. Value range: [1, 255]. |
| filter_size_w | Input | Whether to increase the convolution kernel width by 256 elements on top of filter_w. true means increase; false means no increase. |
| filter_size_h | Input | Whether to increase the convolution kernel height by 256 elements on top of filter_h. true means increase; false means no increase. |
| transpose | Input | Whether to enable the transpose function, which transposes the entire target cube. This is valid only when the source operand is of the half type. true means enable; false means disable. |
| f_matrix_ctrl | Input | Attribute description indicating whether the asc_copy_l12l0b instruction obtains the FeatureMap from the left cube or the right cube. Currently only false is supported. |
| channel_size | Input | Number of channels of the source operand. Value range: [1, 63]. For uint32_t/int32_t/float, channelSize can be 4, N * 8, or N * 8 + 4; for half/bfloat16, channelSize can be 4, 8, N * 16, N * 16 + 4, or N * 16 + 8; for int8_t/uint8_t, channelSize can be 4, 8, 16, 32 * N, N * 32 + 4, N * 32 + 8, or N * 32 + 16; for int4b_t, ChannelSize can be 8, 16, 32, N * 64, N * 64 + 8, N * 64 + 16, or N * 64 + 32. N is a positive integer.|

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- This API does not support the transpose scenario of 2D format transfer. To implement it, see [asc_copy_l12l0b_trans](../asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md).

## 3D Data Format Description

The input feature map and filter must be in NC1HWC0 format, where C0 is the lowest dimension and is fixed at 16 (32 for u8/s8 types), and C1 = C/C0.

To simplify the scenario, the following assumes that the input feature map has 4 channels, that is, Ci = 4. The shape of the input feature maps in the L1 Buffer is (Hi, Wi, Ci). After load3dv1 processing, the data shape in the L0B Buffer is (Wo\*Ho, Hk\*Wk\*Ci), where Wo and Ho are the output shapes after convolution, and Hk and Wk are the filter shapes.

Intuitively, the img2col process is the process in which the filter sweeps over the feature map and expands the corresponding feature map data into each row of the output data. The filter first slides Wo steps in the W direction, then moves one step in the H direction and repeats the above process, finally producing Wo * Ho rows of data. In the following figure, the red and yellow data represent the first row and the second row, respectively. The numbers indicate the association among the original input data, the filter, and the output data. As can be seen, load3dv1 first transfers the four numbers corresponding to 00 in the Ci dimension of the input data, then transfers the four numbers corresponding to 01, and finally the size of this row is Hk\*Wk\*Ci, that is, 3\*3\*4=36 numbers.

- The corresponding feature map format is shown in the following figure:

![ ](../../../../figures/load3d_01.png)

- The corresponding filter format is shown in the following figure:

![ ](../../../../figures/load3d_02.png)

Here, n is the number of filters. It can be seen that the dimension layout is (Hk, Wk, Ci, n), but note that the format in the following figure still needs to be converted according to the B cube format in Mmad.

In actual operation, due to storage space or computing capability limitations, the entire convolution computation is usually divided into blocks, and only a small block of data is transferred and computed at a time.

![ ](../../../../figures/load3d_03.png)

For the feature map in the L0B Buffer, there are two schemes: horizontal tiling and vertical tiling, corresponding to repeatMode values 0 and 1, respectively.

Note: The fractal cube in the following figures is 4x4, but it should actually be 16x16 (16x32 for the u8/s8 type).

When repeatMode = 0, each repeat changes the position of the data point read in the filter window, and then jumps to the next C0 position.

![ ](../../../../figures/load3d_04.png)

When repeatMode = 1, the position of the data read in the filter window remains unchanged, and each repeat advances by C0 elements in the feature map.

![ ](../../../../figures/load3d_05.png)



## Examples

```cpp
__cbuf__ half src[256];
__cb__ half dst[256];
asc_copy_l12l0b(dst, src, 1, 1, 1, 0);
```
