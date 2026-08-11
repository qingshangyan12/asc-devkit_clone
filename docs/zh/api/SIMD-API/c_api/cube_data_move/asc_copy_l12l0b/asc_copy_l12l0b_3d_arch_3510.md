# asc_copy_l12l0b

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。

搬运过程中是以512字节的数据分形为单位进行搬运的，此接口也可以用于普通矩阵计算所需的2D格式数据的搬运。如何使用此接口进行2D格式数据的搬运可以参考[关键特性说明](#zh-cn_topic_00123546_4)。支持从L1 Buffer到L0B Buffer的数据搬运。

3D img2col搬运模式下，Feature Map的属性描述（fm_w、fm_h及四周padding）需通过[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)预先配置，填充值与填充模式需通过[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)预先配置；如需使用repeat模式，repeat方向、次数与步长需通过`asc_set_l13d_rpt_b`预先配置。

本接口仅在AIC上执行有效，非AIC调用直接返回。

## 函数原型

```cpp
__aicore__ inline void asc_copy_l12l0b(__cb__ <dtype>* dst,
                                       __cbuf__ <dtype>* src,
                                       uint16_t k_extension,
                                       uint16_t m_extension,
                                       uint16_t k_start_pt,
                                       uint16_t m_start_pt,
                                       uint8_t stride_w,
                                       uint8_t stride_h,
                                       uint8_t filter_w,
                                       uint8_t filter_h,
                                       uint8_t dilation_filter_w,
                                       uint8_t dilation_filter_h,
                                       bool filter_size_w,
                                       bool filter_size_h,
                                       bool transpose,
                                       bool f_matrix_ctrl,
                                       uint16_t channel_size)
__aicore__ inline void asc_copy_l12l0b_sync(__cb__ <dtype>* dst,
                                            __cbuf__ <dtype>* src,
                                            uint16_t k_extension,
                                            uint16_t m_extension,
                                            uint16_t k_start_pt,
                                            uint16_t m_start_pt,
                                            uint8_t stride_w,
                                            uint8_t stride_h,
                                            uint8_t filter_w,
                                            uint8_t filter_h,
                                            uint8_t dilation_filter_w,
                                            uint8_t dilation_filter_h,
                                            bool filter_size_w,
                                            bool filter_size_h,
                                            bool transpose,
                                            bool f_matrix_ctrl,
                                            uint16_t channel_size)
```

#### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 典型示例

```cpp
// 示例：half类型的3D搬运。
__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst,
                                       __cbuf__ half* src,
                                       uint16_t k_extension,
                                       uint16_t m_extension,
                                       uint16_t k_start_pt,
                                       uint16_t m_start_pt,
                                       uint8_t stride_w,
                                       uint8_t stride_h,
                                       uint8_t filter_w,
                                       uint8_t filter_h,
                                       uint8_t dilation_filter_w,
                                       uint8_t dilation_filter_h,
                                       bool filter_size_w,
                                       bool filter_size_h,
                                       bool transpose,
                                       bool f_matrix_ctrl,
                                       uint16_t channel_size)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。 |
| `src` | 输入 | 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。 |
| `k_extension` | 输入 | 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。 |
| `m_extension` | 输入 | 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。 |
| `k_start_pt` | 输入 | 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数，对于b4类型，应为64的倍数；取值范围：[0, 65535]。 |
| `m_start_pt` | 输入 | 目的操作数height维度的起点，取值范围：[0, 32767]。 |
| `stride_w` | 输入 | 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。 |
| `stride_h` | 输入 | 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。 |
| `filter_w` | 输入 | 卷积核width，取值范围：[1, 255]。 |
| `filter_h` | 输入 | 卷积核height，取值范围：[1, 255]。 |
| `dilation_filter_w` | 输入 | 卷积核width膨胀系数，取值范围：[1, 255]。 |
| `dilation_filter_h` | 输入 | 卷积核height膨胀系数，取值范围：[1, 255]。 |
| `filter_size_w` | 输入 | 是否在`filter_w`的基础上将卷积核width增加256个元素。<br>&nbsp;&nbsp;&bull; `true`：增加；<br>&nbsp;&nbsp;&bull; `false`：不增加。 |
| `filter_size_h` | 输入 | 是否在`filter_h`的基础上将卷积核height增加256个元素。<br>&nbsp;&nbsp;&bull; `true`：增加；<br>&nbsp;&nbsp;&bull; `false`：不增加。 |
| `transpose` | 输入 | 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。 |
| `f_matrix_ctrl` | 输入 | 3D数据搬运Feature Map的属性描述描述寄存器组选择位，一般设置为`false`。<br>&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；<br>&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l13d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。 |
| `channel_size` | 输入 | 源操作数的通道数N。<br>对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。<br>对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。<br>对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需512字节对齐（L0B Buffer对齐要求），否则会导致搬运异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L0B Buffer容量上限：L0B Buffer总容量64KB，dst偏移+搬运大小不可越界，否则触发异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发异常。

### 3D img2col搬运模式约束

- 此接口需要先调用配套的寄存器设置接口[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)与[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)预先配置feature map描述、填充值与填充模式，否则会导致搬运接口不符合预期。如需使用repeat模式，须先调用`asc_set_l13d_rpt_b`接口配置repeat方向、次数与步长。
- 3D格式搬运到L0B Buffer时会自动进行转置，transpose参数无效，不支持非转置场景。
- `k_extension`、`m_extension`或`channel_size`为`0`，`filter_w`为`0`且`filter_size_w`为`false`，或`filter_h`为`0`且`filter_size_h`为`false`时不执行搬运，本接口被视为NOP（空操作）。
- 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。否则会导致搬运结果不符合预期。
- `stride_w`与`stride_h`不可为0，传入0硬件按1处理。
- `stride_w`取值范围为[1, 63]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `stride_h`取值范围为[1, 63]，超出取值范围的值会被截断，导致搬运结果不符合预期。

## 关键特性说明<a id="zh-cn_topic_00123546_4"></a>

### 使用3D卷积数据搬运完成矩阵计算所需2D格式数据的搬运

3D数据搬运在b8/b16/b32数据类型下均支持转置，L1 Buffer->L0B Buffer通路自动开启转置搬运，下面按照类型详细说明。

- 对于b8场景：每个分形大小是16×32，搬运示意图如下：

    ![](../../../../figures/load3d_l1tol0b_b8_trans_950.png)

- 对于b16场景：每个分形大小是16×16，搬运示意图如下：

    ![](../../../../figures/load3d_l1tol0b_b16_trans_950.png)

- 对于b32场景：每个分形大小是16×8，搬运示意图如下：

    ![](../../../../figures/load3d_l1tol0b_b32_trans_950.png)

## 调用示例

无
