# asc_copy_l12l0b

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含3D格式搬运。

## 函数原型

- 高维切分搬运 3D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w,uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    ```

- 同步高维切分搬运 3D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w,uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w,uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w,uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t step_k, uint16_t step_m, uint16_t pos_k, uint16_t pos_m, uint8_t stride_w, uint8_t stride_h, uint8_t w_k, uint8_t h_k, uint8_t dilation_w, uint8_t dilation_h, bool filter_w, bool filter_h, bool transpose, bool fmatrix_ctrl, uint16_t size_channel)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0A Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| step_k | 输入 | k方向上自起始点开始的沿伸步长。如果覆盖最下侧的分形，则无倍数要求。对32位类型为8的倍数，对16位类型为16的倍数，对8位类型为32的倍数，对4位类型为64的倍数。 |
| step_m | 输入 | m方向上自起始点开始的沿伸步长。如果覆盖最下侧的分形，则为16的倍数。 |
| pos_k | 输入 | 搬运的子矩阵在k方向上的起始位置。对32位类型为8的倍数，对16位类型为16的倍数，对8位类型为32的倍数，对4位类型为64的倍数。 |
| pos_m | 输入 | 搬运的子矩阵在m方向上的起始位置。取值范围：[0, 32767] |
| stride_w | 输入 | 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。 |
| stride_h | 输入 | 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。 |
| w_k | 输入 | 在膨胀处理前，width维度上9位卷积核尺寸的低8位。 |
| h_k | 输入 | 在膨胀处理前，height维度上9位卷积核尺寸的低8位。 |
| dilation_w | 输入 | width维度上的膨胀尺寸，0被视作为1。 |
| dilation_h | 输入 | height维度上的膨胀尺寸，0被视作为1。 |
| filter_w | 输入 | 在膨胀处理前，width维度上9位卷积核尺寸的第9位。 |
| filter_h | 输入 | 在膨胀处理前，height维度上9位卷积核尺寸的第9位。|
| transpose | 输入 | 转置控制。若为true，整个二维的特征图会被转置。 |
| fmatrix_ctrl | 输入 | SPR控制位。若为true，则采用SPR.FMATRIX,SPR.PADDING,SPR.L3D_RPT描述特征图；反之则采用SPR.FMATRIX_B,SPR.PADDING_B,SPR.L3D_RPT_B描述特征图。 |
| channel_size | 输入 | 源操作数的通道数，对于uint32_t/int32_t/float，channelSize可取值为4，N * 8，N * 8 + 4；对于half/bfloat16，channelSize可取值为4，8，N * 16，N * 16 + 4，N * 16 + 8；对于int8_t/uint8_t，channelSize可取值为4，8，16， 32 * N，N * 32 + 4，N * 32 + 8，N * 32 + 16；对于int4b_t，ChannelSize可取值为8，16，32，N * 64，N * 64 + 8，N * 64 + 16，N * 64 + 32。N为正整数。|

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- dst的起始地址需要512字节对齐，src的起始地址需要32字节对齐。
- 本接口不支持2D格式搬运的转置场景，若要实现请参考[asc_copy_l12l0b_trans](./asc_copy_l12l0b_trans.md)。

## 3D数据格式说明

要求输入的feature map和filter的格式是 NC1HWC0，其中 C0 是最低维度而且 C0 是固定值为 16（对于u8/s8类型为32），C1=C/C0。

为了简化场景，以下场景假设输入的 feature map 的 channel 为4，即 Ci=4。输入 feature maps 在 A1 中的形状为 (Hi,Wi,Ci)，经过 load3dv1 处理后在 A2 的数据形状为(Wo*Ho, Hk*Wk*Ci)。其中 Wo 和 Ho 是卷积后输出的shape，Hk 和 Wk 是 filter 的 shape。

直观的来看，img2col 的过程就是 filter 在 feature map 上扫过，将对应 feature map 的数据展开成输出数据的每一行的过程。filter 首先在W方向上滑动 Wo 步，然后在 H 方向上走一步然后重复以上过程，最终输出 Wo * Ho 行数据。下图中红色和黄色的数据分别代表第一行和第二行。数字表示原始输入数据，filter 和输出数据三者之间的关联关系。可以看到，load3dv1 首先在输入数据的 Ci 维度搬运对应于 00 的 4 个数，然后搬运对应于 01 的四个数，最终这一行的大小为 Hk*Wk*Ci 即 3*3*4=36 个数。

- 对应的feature map格式如下图：

![ ](../figures/load3d_01.png)

- 对应的 filter 的格式如下图：

![ ](../figures/load3d_02.png)

其中 n 为 filter 的个数，可以看出维度排布为 (Hk,Wk,Ci,n)，但是需要注意的是下图的格式还需要根据Mmad中B矩阵的格式转换。

实际操作中，由于存储空间或者计算能力限制，我们通常会将整个卷积计算分块，一次只搬运并计算一小块数据。

![ ](../figures/load3d_03.png)

对于 A2 的 feature map 来说有两种方案，水平分块和垂直分块。分别对应参数中 repeatMode 的 0 和 1。

注：下图中的分形矩阵大小为 4x4，实际应该为 16x16 (对于 u8/s8 类型为 16x32)

repeatMode =0 时，每次 repeat 会改变在 filter 窗口中读取数据点的位置，然后跳到下一个 C0 的位置。

![ ](../figures/load3d_04.png)

repeatMode =1 的时候 filter 窗口中读取数据的位置保持不变，每个 repeat 在 feature map 中前进 C0 个元素。

![ ](../figures/load3d_05.png)



## 调用示例

```cpp
__cbuf__ half src[256];
__cb__ half dst[256];
asc_copy_l12l0b(dst, src, 8, 16, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, false, true, 0)
```
