# asc_copy_l12l0a

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。

## 函数原型

- 高维切分搬运 3D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    ```

- 同步高维切分搬运 3D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0A Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| step_k | 输入 | k方向上自起始点开始的沿伸步长。如果覆盖最下侧的分形，则该参数无倍数要求。对位宽为32的数据类型，该参数为8的倍数；对位宽为16的数据类型，该参数为16的倍数；对位宽为8的数据类型，该参数为32的倍数；对位宽为4的数据类型，该参数为64的倍数。 |
| step_m | 输入 | m方向上自起始点开始的沿伸步长。如果覆盖最下侧的分形，则该参数为16的倍数。 |
| pos_k | 输入 | 搬运的子矩阵在k方向上的起始位置。对位宽为32的数据类型，该参数为8的倍数；对位宽为16的数据类型，该参数为16的倍数；对位宽为8的数据类型，该参数为32的倍数；对位宽为4的数据类型，该参数为64的倍数。 |
| pos_m | 输入 | 搬运的子矩阵在m方向上的起始位置。取值范围：[0, 32767]。 |
| stride_w | 输入 | 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。 |
| stride_h | 输入 | 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。 |
| w_k | 输入 | 在膨胀处理前，width维度上9位卷积核尺寸的低8位。 |
| h_k | 输入 | 在膨胀处理前，height维度上9位卷积核尺寸的低8位。 |
| dilation_w | 输入 | width维度上的膨胀尺寸，0被视作为1。 |
| dilation_h | 输入 | height维度上的膨胀尺寸，0被视作为1。 |
| filter_w | 输入 | 在膨胀处理前，width维度上9位卷积核尺寸的第9位。 |
| filter_h | 输入 | 在膨胀处理前，height维度上9位卷积核尺寸的第9位。|
| transpose | 输入 | 转置控制。若为true，整个二维的特征图会被转置。 |
| fmatrix_ctrl | 输入 | SPR控制位。 <br>&bull; true：采用set_fmatrix, set_padding, set_l3d_rpt接口控制特征图。 <br>&bull; false：采用set_fmatrix_b, set_padding_b, set_l3d_rpt_b接口控制特征图。 |
| channel_size | 输入 | 源操作数的通道数，对于uint32_t/int32_t/float，该参数为4，N * 8，N * 8 + 4；对于half/bfloat16，该参数为4，8，N * 16，N * 16 + 4，N * 16 + 8；对于int8_t/uint8_t，该参数为4，8，16， 32 * N，N * 32 + 4，N * 32 + 8，N * 32 + 16；对于int4b_t，该参数为8，16，32，N * 64，N * 64 + 8，N * 64 + 16，N * 64 + 32。其中N为正整数。|

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- dst的起始地址需要512字节对齐，src的起始地址需要32字节对齐。
- 本接口不支持2D格式搬运场景。

## 调用示例

```cpp
//k方向上自起始点开始的沿伸步长为8
constexpr uint16_t step_k = 8;
//m方向上自起始点开始的沿伸步长为16
constexpr uint16_t step_m = 16;
//搬运的子矩阵在k方向上的起始位置为0
constexpr uint16_t pos_k = 0;
//搬运的子矩阵在m方向上的起始位置为0
constexpr uint16_t pos_m = 0;
//卷积核在源操作数width维度滑动的步长为1
constexpr uint8_t stride_w = 1;
//卷积核在源操作数height维度滑动的步长为1
constexpr uint8_t stride_h = 1;
//在膨胀处理前，width维度上9位卷积核尺寸为3，低8位为3
constexpr uint8_t w_k = 3;
//在膨胀处理前，height维度上9位卷积核尺寸为3，低8位为3
constexpr uint8_t h_k = 3;
//width维度上的膨胀尺寸为1
constexpr uint8_t dilation_w = 1;
//height维度上的膨胀尺寸为1
constexpr uint8_t dilation_h = 1;
//在膨胀处理前，height维度上9位卷积核尺寸为3，第9位为0
constexpr bool filter_w = false;
//在膨胀处理前，height维度上9位卷积核尺寸为3，第9位为0
constexpr bool filter_h = false;
//二维的特征图不需要转置
constexpr bool transpose = false;
//采用set_fmatrix, set_padding, set_l3d_rpt接口控制特征图
constexpr bool fmatrix_ctrl = true;
//源操作数的通道数为4
constexpr uint16_t size_channel = 4;
__cbuf__ half src[256];
__ca__ half dst[256];
asc_copy_l12l0a(dst, src, step_k, step_m, pos_k, pos_m, stride_w, stride_h, w_k, h_k, dilation_w, dilation_h, filter_w, filter_h, transpose, fmatrix_ctrl, size_channel);
```
