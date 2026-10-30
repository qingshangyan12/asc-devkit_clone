# asc_copy_l12l0b_sync_arch_2201 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:12:32.395Z pushedAt=2026-10-28T12:55:18.848Z nl=2 -->

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

**The `asc_copy_l12l0b_sync` API is deprecated. Use the [corresponding non-synchronous API](../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_2201.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Copies a 512B matrix stored in the L1 Buffer to the L0B Buffer. Supports 2D format transfer and 3D format transfer.

## Prototype

- Synchronous high-dimensional split transfer in 2D format

    ```cpp
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

- Synchronous high-dimensional split transfer in 3D format

    ```cpp
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    ```

## Parameters

**Table 1** 2D format parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination L0B Buffer address. |
| src | Input | Source L1 Buffer address. |
| start_index | Input | Fractal cube ID, indicating that the copy starts from the specified fractal in the source operand (0 indicates the first fractal cube in the source operand). Value range: [0, 65535]. Unit: 512 bytes. |
| repeat | Input | Number of iterations, where each iteration processes 512B data. Value range: [1, 255]. |
| src_stride | Input | Interval between the start addresses of the previous fractal and the next fractal in the source operand across adjacent iterations. Value range: [0, 65535]. Unit: 512 bytes. |
| dst_gap | Input | Interval between adjacent contiguous data blocks in the destination operand (the interval between the end of the preceding data block and the start of the following data block). Value range: [0, 65535]. Unit: 512 bytes. |

**Table 2** 3D format parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination L0B Buffer address. |
| src | Input | Source L1 Buffer address. |
| k_extension | Input | Transfer length of this instruction in the width dimension of the destination operand. If not overriding the rightmost fractal, it should be a multiple of 16 for the half type and a multiple of 32 for the int8_t/uint8_t type; if overriding the rightmost fractal, then no multiple requirement applies. Value range: [1, 65535]. |
| m_extension | Input | Transfer length of this instruction in the height dimension of the destination operand. If not overriding the bottommost fractal, it should be a multiple of 16 for the half/int8_t/uint8_t type; if overriding the bottommost fractal, then no multiple requirement applies. Value range: [1, 65535]. |
| k_start_pt | Input | Start point of this instruction in the width dimension of the destination operand. It should be a multiple of 16 for the half type and a multiple of 32 for the int8_t/uint8_t type. Value range: [0, 65535] |
| m_start_pt | Input | Start point of this instruction in the height dimension of the destination operand. If not overriding the bottommost fractal, it should be a multiple of 16 for the half/int8_t/uint8_t type; if overriding the bottommost fractal, then no multiple requirement applies. Value range: [0, 65535].|
| stride_w | Input | Step by which the convolution kernel slides in the width dimension of the source operand. Value range: [1, 63]. |
| stride_h | Input | Step by which the convolution kernel slides in the height dimension of the source operand. Value range: [1, 63]. |
| filter_w | Input | Width of the convolution kernel. Value range: [1, 255]. |
| filter_h | Input | Height of the convolution kernel. Value range: [1, 255]. |
| dilation_filter_w | Input | Dilation factor of the convolution kernel width. Value range: [1, 255]. |
| dilation_filter_h | Input | Dilation factor of the convolution kernel height. Value range: [1, 255]. |
| filter_size_w | Input | Whether to increase the convolution kernel width by 256 elements on the basis of filter_w. true indicates increase; false indicates no increase. |
| filter_size_h | Input | Whether to increase the convolution kernel height by 256 elements on the basis of filter_h. true indicates increase; false indicates no increase. |
| transpose | Input | Whether to enable the transpose function, which transposes the entire target matrix. It is valid only when the source operand is of the half type. true indicates enabled; false indicates disabled. |
| f_matrix_ctrl | Input | Indicates whether the asc_copy_l12l0b instruction obtains the FeatureMap attribute description from the left matrix or the right matrix. Currently only false is supported. |
| channel_size | Input | Number of channels of the source operand. Value range: [1, 63]. For uint32_t/int32_t/float, channelSize can take values 4, N * 8, and N * 8 + 4; for half/bfloat16, channelSize can take values 4, 8, N * 16, N * 16 + 4, and N * 16 + 8; for int8_t/uint8_t, channelSize can take values 4, 8, 16, 32 * N, N * 32 + 4, N * 32 + 8, and N * 32 + 16; for int4b_t, ChannelSize can take values 8, 16, 32, N * 64, N * 64 + 8, N * 64 + 16, and N * 64 + 32. N is a positive integer.|

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- This API does not support the transpose scenario of 2D format transfer. To implement it, see [asc_copy_l12l0b_trans](../cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_2201.md).

## 3D Data Format Description

The input feature map and filter must be in the NC1HWC0 format, where C0 is the lowest dimension and is fixed at 16 (32 for the u8/s8 type), and C1 = C/C0.

To simplify the scenario, the following scenario assumes that the input feature map has 4 channels, that is, Ci = 4. The shape of the input feature maps in the L1 Buffer is (Hi, Wi, Ci). After load3dv1 processing, the data shape in the L0B Buffer is (Wo\*Ho, Hk\*Wk\*Ci). Here, Wo and Ho are the output shape after convolution, and Hk and Wk are the filter shape.

Intuitively, the img2col process is the process in which the filter sweeps over the feature map and expands the corresponding feature map data into each row of the output data. The filter first slides Wo steps in the W direction, then moves one step in the H direction and repeats the above process, finally outputting Wo * Ho rows of data. In the following figure, the red and yellow data represent the first row and the second row, respectively. The numbers indicate the association among the original input data, the filter, and the output data. As can be seen, load3dv1 first transfers the 4 numbers corresponding to 00 in the Ci dimension of the input data, then transfers the 4 numbers corresponding to 01, and finally the size of this row is Hk\*Wk\*Ci, that is, 3\*3\*4=36 elements.

- The corresponding feature map format is shown in the following figure:

![ ](../../../figures/load3d_01.png)

- The corresponding filter format is shown in the following figure:

![ ](../../../figures/load3d_02.png)

Here, n is the number of filters. It can be seen that the dimension layout is (Hk, Wk, Ci, n). However, note that the format in the following figure also needs to be converted according to the format of the B matrix in Mmad.

In actual operation, due to storage space or computing capability limitations, we usually divide the entire convolution computation into blocks and transfer and compute only a small block of data at a time.

![ ](../../../figures/load3d_03.png)

For the feature map in the L0B Buffer, there are two schemes: horizontal blocking and vertical blocking, corresponding to repeatMode values 0 and 1, respectively.

Note: The fractal matrix in the following figures is 4x4, but it should actually be 16x16 (16x32 for the u8/s8 types).

When repeatMode = 0, each repeat changes the position where data points are read in the filter window, and then jumps to the next C0 position.

![ ](../../../figures/load3d_04.png)

When repeatMode = 1, the position where data is read in the filter window remains unchanged, and each repeat advances by C0 elements in the feature map.

![ ](../../../figures/load3d_05.png)



## Examples

```cpp
__cbuf__ half src[256];
__cb__ half dst[256];
asc_copy_l12l0b_sync(dst, src, 1, 1, 1, 0);
```
