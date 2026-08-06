# asc_copy_l12l0a

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

将L1 Buffer中以分形（512字节为单位）组织的Cube矩阵数据搬运到L0A Buffer，供后续Cube矩阵乘使用。本接口提供两种参数列表完全不同的功能模式，用户按需选择对应重载：

- **2D分形搬运模式**：按M维（行方向）与K维（通道方向）的起点与步长，从源矩阵中提取若干512字节分形写入L0A Buffer，可选开启转置。
- **3D img2col搬运模式**：对NC1HWC0格式的feature map执行image to column操作，按stride/filter/dilation/padding配置展开为img2col分形矩阵后写入L0A Buffer，可选开启转置，支持repeat模式。

两种模式参数列表完全不同，用户必须按使用场景选择对应重载系列，不可混用。本接口仅在AIC上执行有效，在AIV上调用为空操作。3D img2col搬运模式下，feature map描述（fm_w、fm_h及四周padding）需通过[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)预先配置，填充值与填充模式需通过[asc_set_l13d_padding](../asc_set_l13d_padding.md)预先配置，L1 Buffer容量上限需通过[asc_set_l13d_size](../asc_set_l13d_size.md)预先配置；如需使用repeat模式，repeat方向、次数与步长需通过[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)预先配置。

## 函数原型

### 2D搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l12l0a(__ca__ <dtype>* dst,
                                       __cbuf__ <dtype>* src,
                                       uint16_t m_start_position,
                                       uint16_t k_start_position,
                                       uint8_t m_step,
                                       uint8_t k_step,
                                       int16_t src_stride,
                                       uint16_t dst_stride)
__aicore__ inline void asc_copy_l12l0a_sync(__ca__ <dtype>* dst,
                                            __cbuf__ <dtype>* src,
                                            uint16_t m_start_position,
                                            uint16_t k_start_position,
                                            uint8_t m_step,
                                            uint8_t k_step,
                                            int16_t src_stride,
                                            uint16_t dst_stride)
```

#### dtype支持的数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 典型示例

```cpp
// 示例：half类型的2D搬运。
__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst,
                                       __cbuf__ half* src,
                                       uint16_t m_start_position,
                                       uint16_t k_start_position,
                                       uint8_t m_step,
                                       uint8_t k_step,
                                       int16_t src_stride,
                                       uint16_t dst_stride)
```

### 2D转置搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ <dtype>* dst,
                                                 __cbuf__ <dtype>* src,
                                                 uint16_t m_start_position,
                                                 uint16_t k_start_position,
                                                 uint8_t m_step,
                                                 uint8_t k_step,
                                                 int16_t src_stride,
                                                 uint16_t dst_stride)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ <dtype>* dst,
                                                      __cbuf__ <dtype>* src,
                                                      uint16_t m_start_position,
                                                      uint16_t k_start_position,
                                                      uint8_t m_step,
                                                      uint8_t k_step,
                                                      int16_t src_stride,
                                                      uint16_t dst_stride)
```

#### dtype支持的数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 典型示例

```cpp
// 示例：half类型的2D同步转置搬运。
__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ half* dst,
                                                      __cbuf__ half* src,
                                                      uint16_t m_start_position,
                                                      uint16_t k_start_position,
                                                      uint8_t m_step,
                                                      uint8_t k_step,
                                                      int16_t src_stride,
                                                      uint16_t dst_stride)
```

### 3D搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l12l0a(__ca__ <dtype>* dst,
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
__aicore__ inline void asc_copy_l12l0a_sync(__ca__ <dtype>* dst,
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
__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst,
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

### 2D分形搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的缓冲（L0A Buffer）的起始地址，dtype由重载决定。起始地址需512字节对齐。 |
| src | 输入 | 源缓冲（L1 Buffer）的起始地址，dtype与dst一致。起始地址需32字节对齐。 |
| m_start_position | 输入 | M维（行方向）起始分形位置，单位16（即一个分形的M维高度）。取值范围 [0, 2^16−1]。 |
| k_start_position | 输入 | K维（通道方向）起始分形位置，单位32字节（即一个分形的K维字节宽度）。取值范围 [0, 2^16−1]。 |
| m_step | 输入 | M维分形步长，即一次提取的分形行数，单位16。取值范围 [1, 255]，不可为0。 |
| k_step | 输入 | K维分形步长，即一次提取的分形列数，单位32字节。取值范围 [1, 255]，不可为0。 |
| src_stride | 输入 | 相邻K步在源端（L1 Buffer）的步长，单位分形（512字节），可为负值（用于反向读取）。取值范围 [−2^15, 2^15−1]。 |
| dst_stride | 输入 | 相邻K步在目的端（L0A Buffer）的步长，单位分形（512字节），不可为负值。取值范围 [0, 2^16−1]，即使k_step为1也不可为0。 |

### 3D img2col搬运模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的缓冲（L0A Buffer）的起始地址，dtype由重载决定。起始地址需512字节对齐。 |
| src | 输入 | 源缓冲（L1 Buffer）的起始地址，dtype与dst一致。起始地址按channel_size与dtype字节对齐（channel_size × sizeof(dtype) ≥ 32时需32字节对齐，否则按channel_size × sizeof(dtype) 对齐）。 |
| k_extension | 输入 | K维（通道方向）扩展步长，即img2col后K维提取的扩展长度。取值范围 [1, 2^16−1]，不可为0。若不覆盖最右侧分形，需按dtype位宽对齐：`b8`需为32的倍数、`b16`需为16的倍数、`b32`需为8的倍数。 |
| m_extension | 输入 | M维（行方向）扩展步长，即img2col后M维提取的扩展长度，单位16。取值范围 [1, 2^16−1]，不可为0。若不覆盖最下侧分形，需为16的倍数。 |
| k_start_pt | 输入 | img2col后矩阵K维起始位置。取值范围 [0, 2^16−1]。需按dtype位宽对齐：`b8`需为32的倍数、`b16`需为16的倍数、`b32`需为8的倍数。 |
| m_start_pt | 输入 | img2col后矩阵M维起始位置，单位16。取值范围 [0, 32767]。 |
| stride_w | 输入 | W维（宽方向）卷积步长。取值范围 [1, 63]，不可为0（传入0硬件按1处理）。 |
| stride_h | 输入 | H维（高方向）卷积步长。取值范围 [1, 63]，不可为0（传入0硬件按1处理）。 |
| filter_w | 输入 | W维卷积核宽度（dilation前的尺寸）。取值范围 [1, 255]，传入0硬件按1处理。 |
| filter_h | 输入 | H维卷积核高度（dilation前的尺寸）。取值范围 [1, 255]，传入0硬件按1处理。 |
| dilation_filter_w | 输入 | W维空洞（dilation）大小。取值范围 [0, 255]，传入0硬件按1处理。 |
| dilation_filter_h | 输入 | H维空洞（dilation）大小。取值范围 [0, 255]，传入0硬件按1处理。 |
| `filter_size_w` | 输入 | W维卷积核尺寸扩展控制位，用于9 bit卷积核宽的高位。<br>&nbsp;&nbsp;&bull; `false`：卷积核宽不扩展；<br>&nbsp;&nbsp;&bull; `true`：卷积核宽按9 bit解码（实际卷积核宽 = {filter_size_w, filter_w}）。 |
| `filter_size_h` | 输入 | H维卷积核尺寸扩展控制位，用于9 bit卷积核高的高位。<br>&nbsp;&nbsp;&bull; `false`：卷积核高不扩展；<br>&nbsp;&nbsp;&bull; `true`：卷积核高按9 bit解码（实际卷积核高 = {filter_size_h, filter_h}）。 |
| `transpose` | 输入 | 转置使能。<br>&nbsp;&nbsp;&bull; `false`：不开启转置；<br>&nbsp;&nbsp;&bull; `true`：开启转置，将img2col后的整张矩阵转置后再写入L0A Buffer。仅`b8`/`b16`/`b32`系列dtype解码使能，其他dtype时被忽略。 |
| `f_matrix_ctrl` | 输入 | feature map描述寄存器组选择位，用于选择使用第一组或第二组feature map描述。<br>&nbsp;&nbsp;&bull; `false`：使用第一组feature map描述（fm_w、fm_h、padding、repeat等配置）；<br>&nbsp;&nbsp;&bull; `true`：使用第二组feature map描述。两组描述须分别通过配套setter接口预先配置。 |
| channel_size | 输入 | 通道数N，受dtype位宽约束。`b8`取值 {0, 4, 8, 16, N×32, N×32+4, N×32+8, N×32+16}，上限65520；`b16`取值 {0, 4, 8, N×16, N×16+4, N×16+8}，上限65528；`b32`取值 {0, 4, N×8, N×8+4}，上限65532。不可为0。需按dtype满足对齐约束（与k_start_pt同基准）。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需512字节对齐（L0A Buffer对齐要求），否则会导致搬运异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L0A Buffer容量上限：L0A Buffer总容量64KB，dst偏移+搬运大小不可越界，否则触发异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发异常。

### 2D分形搬运模式约束

- m_step或k_step为0时不执行搬运，本接口被视为NOP（空操作）。
- dst_stride即使k_step为1也不可为0，否则触发异常。
- 开启transpose时需按dtype满足分形对齐约束：`int4b_t`需m_step为4的倍数；`int8_t`、`uint8_t`需m_step为2的倍数；`fp4x2_e2m1_t`、`fp4x2_e1m2_t`需m_step为1的倍数；`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`需m_step为2的倍数；`int16_t`、`uint16_t`、`half`、`bfloat16_t`需m_step为1的倍数；`int32_t`、`uint32_t`、`float`需k_step为2的倍数，否则触发地址对齐异常。
- 开启transpose时仅支持`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`，其他dtype触发异常。

### 3D img2col搬运模式约束

- k_extension、m_extension或channel_size为0时不执行搬运，本接口被视为NOP（空操作）。
- filter_w为0且filter_size_w为false，或filter_h为0且filter_size_h为false时不执行搬运，本接口被视为NOP（空操作）。
- stride_w与stride_h不可为0，传入0硬件按1处理。
- m_start_pt不可超过32767，否则触发异常。
- 须先调用配套setter接口预先配置feature map描述（fm_w、fm_h、pad_l、pad_r、pad_t、pad_b）、填充值与填充模式、L1 Buffer容量上限，再调用本接口，否则feature map描述使用未初始化的配置值产生错误结果。如需使用repeat模式，须先调用配套setter接口配置repeat方向、次数与步长。
- 调用本模式前，必须通过[asc_set_l3d_rpt_b](../asc_set_l3d_rpt_b.md)设置目的端步长。
- feature map尺寸fm_w与fm_h须>0且≤32767，否则指令被置为NOP不执行。
- feature map（含padding）须≥卷积核（含dilation）尺寸，即fm_w+pad_l+pad_r≥ (filter_w − 1) ×dilation_filter_w+1，fm_h+pad_t+pad_b≥ (filter_h − 1) ×dilation_filter_h+1，否则触发异常。
- feature map（不含padding）与通道数须满足fm_w×fm_h×channel_size×sizeof(dtype) ≤L1 Buffer容量，否则触发异常。
- k_start_pt+k_extension不可超过img2col后K维总长（=filter_h×filter_w×channel_size），m_start_pt+m_extension不可超过img2col后M维总长（=Hstep×Wstep，其中Hstep=floor((fm_h+pad_t+pad_b − (filter_h − 1) ×dilation_filter_h − 1) /stride_h) +1，Wstep同理），否则触发异常。
- channel_size需按dtype位宽满足对齐约束：`b8`需为32的倍数、或4、或8、或16、或32的倍数+4、或32的倍数+8、或32的倍数+16；`b16`需为16的倍数、或4、或8、或16的倍数+4、或16的倍数+8；`b32`需为8的倍数或8的倍数+4。k_start_pt需按同基准对齐，否则触发异常。
- channel_size不可超过dtype上限：`b8`系列≤65520、`b16`系列≤65528、`b32`系列≤65532，否则触发异常。
- img2col后矩阵尺寸受K/M维起点位宽限制：fm_w×fm_h须<2^16，Wstep×Hstep须<2^15，否则触发异常。
- transpose仅在`b8`/`b16`/`b32`系列dtype时解码使能，其他dtype时transpose字段被忽略恒为false。
- 若需同时使用本接口的3D img2col模式与历史3D模式，须在两者之间插入同步指令保证串行化，防止feature map配置串扰。
- `b4`系列dtype（`int4b_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`）不支持，本模式重载未提供这些dtype，传入会触发异常。
- stride_w传入>63的值会截断处理（如64→0、65→1）。截断后实际生效值偏小，导致W维卷积步长与预期不符，img2col后的窗口位置错位，提取的feature map数据与预期不一致。参数类型为`uint8_t`（0~255），截断后若值为0会被硬件按1处理。
- stride_h传入>63的值会截断处理（如64→0、65→1）。截断后实际生效值偏小，导致H维卷积步长与预期不符，img2col后的窗口位置错位，提取的feature map数据与预期不一致。参数类型为`uint8_t`（0~255），截断后若值为0会被硬件按1处理。

## 3D数据格式说明

要求输入的feature map和filter的格式是NC1HWC0，其中C0是最低维度而且C0是固定值为16（对于u8/s8类型为32），C1=C/C0。

为了简化场景，以下场景假设输入的feature map的channel为4，即Ci=4。输入feature maps在L1 Buffer中的形状为(Hi,Wi,Ci)，经过load3dv1处理后在L0A Buffer的数据形状为(Wo\*Ho, Hk\*Wk\*Ci)。其中Wo和Ho是卷积后输出的shape，Hk和Wk是filter的shape。

直观的来看，img2col的过程就是filter在feature map上扫过，将对应feature map的数据展开成输出数据的每一行的过程。filter首先在W方向上滑动Wo步，然后在H方向上走一步然后重复以上过程，最终输出Wo * Ho行数据。下图中红色和黄色的数据分别代表第一行和第二行。数字表示原始输入数据，filter和输出数据三者之间的关联关系。可以看到，load3dv1首先在输入数据的Ci维度搬运对应于00的4个数，然后搬运对应于01的四个数，最终这一行的大小为Hk\*Wk\*Ci即3\*3\*4=36个数。

- 对应的feature map格式如下图：

![ ](../../figures/load3d_01.png)

- 对应的filter的格式如下图：

![ ](../../figures/load3d_02.png)

其中n为filter的个数，可以看出维度排布为(Hk,Wk,Ci,n)，但是需要注意的是下图的格式还需要根据Mmad中B矩阵的格式转换。

实际操作中，由于存储空间或者计算能力限制，我们通常会将整个卷积计算分块，一次只搬运并计算一小块数据。

![ ](../../figures/load3d_03.png)

对于L0A Buffer中的feature map来说有两种方案，水平分块和垂直分块。分别对应参数中repeatMode的0和1。

注：下图中的分形矩阵大小为4x4，实际应该为16x16 (对于u8/s8类型为16x32)

repeatMode =0时，每次repeat会改变在filter窗口中读取数据点的位置，然后跳到下一个C0的位置。

![ ](../../figures/load3d_04.png)

repeatMode =1的时候filter窗口中读取数据的位置保持不变，每个repeat在feature map中前进C0个元素。

![ ](../../figures/load3d_05.png)

## 调用示例

```cpp
__ca__ bfloat16_t dst[256];
__cbuf__ bfloat16_t src[256];
uint16_t m_start_position = 8;
uint16_t k_start_position = 2;
uint8_t m_step = 4;
uint8_t k_step = 4;
int16_t src_stride = 1;
uint16_t dst_stride = 1;
asc_copy_l12l0a(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```
