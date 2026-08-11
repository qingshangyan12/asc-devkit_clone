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

将数据从L1 Buffer搬运到L0B Buffer。本接口提供两种参数列表完全不同的功能模式，用户按需选择对应重载：

- **2D矩阵搬运模式**：以512字节的分形为单位，从L1 Buffer中按M维（行方向）与K维（通道方向）的起始位置、步长切分提取多个分形矩阵搬运到L0B Buffer，用于普通矩阵计算所需的2D格式数据加载。
- **3D格式img2col搬运模式**：对L1 Buffer中NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、空洞卷积配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。

两种模式参数列表完全不同，用户必须按使用场景选择对应重载系列，不可混用。

3D格式img2col搬运模式下，Feature Map的尺寸描述需通过[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)预先配置，双源拼接场景下第二个Feature Map的尺寸描述需通过[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)预先配置；padding填充值需通过[asc_set_l13d_padding](../asc_set_l13d_padding.md)预先配置；repeat控制需通过[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)预先配置；L1 Buffer容量上限需通过[asc_set_l13d_size](../asc_set_l13d_size.md)预先配置。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

### 2D搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l12l0b(__cb__ <dtype>* dst,
                                       __cbuf__ <dtype>* src,
                                       uint16_t m_start_position,
                                       uint16_t k_start_position,
                                       uint8_t m_step,
                                       uint8_t k_step,
                                       int16_t src_stride,
                                       uint16_t dst_stride)
__aicore__ inline void asc_copy_l12l0b_sync(__cb__ <dtype>* dst,
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
__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst,
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
__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ <dtype>* dst,
                                                 __cbuf__ <dtype>* src,
                                                 uint16_t m_start_position,
                                                 uint16_t k_start_position,
                                                 uint8_t m_step,
                                                 uint8_t k_step,
                                                 int16_t src_stride,
                                                 uint16_t dst_stride)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ <dtype>* dst,
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
__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ half* dst,
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

### 2D矩阵搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。 |
| m_start_position | 输入 | 源矩阵中首个分形在M维（行方向）的起始位置，单位为16个元素。取值范围 [0, 2^16−1]。 |
| k_start_position | 输入 | 源矩阵中首个分形在K维（通道方向）的起始位置，单位32字节。取值范围 [0, 2^16−1]。 |
| m_step | 输入 | M维（行方向）加载步长，即一次加载的分形行数，单位为16个元素。取值范围 [1, 255]。 |
| k_step | 输入 | K维（通道方向）加载步长，即一次加载的分形列数，单位32字节。取值范围 [1, 255]。 |
| src_stride | 输入 | 源端（L1 Buffer）相邻K步之间的步长，单位512字节。取值范围 [−2^15, 2^15−1]，可为负值（负值表示源端反向取数）。 |
| dst_stride | 输入 | 目的端（L0B Buffer）相邻K步之间的步长，单位512字节。取值范围 [0, 2^16−1]，不可为负值。 |

### 3D格式img2col搬运模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer，数据为Feature Map。源地址按channel_size × sizeof(dtype) 对齐（channel_size较小时按对应字节数对齐，channel_size ≥ dtype阈值时32字节对齐）。 |
| k_extension | 输入 | 从起始位置起K维（通道方向）的扩展步长，即img2col展开后目的矩阵在K方向的提取范围。未覆盖最右侧分形时需为dtype对应分形列数的整数倍：b32为8的倍数、b16为16的倍数、b8为32的倍数。取值范围 [0, 2^16−1]，不可为0。 |
| m_extension | 输入 | 从起始位置起M维（行方向）的扩展步长，即img2col展开后目的矩阵在M方向的提取范围。未覆盖最下侧分形时需为16的倍数。取值范围 [0, 2^16−1]，不可为0。 |
| k_start_pt | 输入 | 目的矩阵在K维的起始位置。需为dtype对应分形列数的整数倍：b32为8的倍数、b16为16的倍数、b8为32的倍数。取值范围 [0, 2^16−1]。 |
| m_start_pt | 输入 | 目的矩阵在M维的起始位置。取值范围 [0, 32767]，传入 > 32767的值触发异常。 |
| stride_w | 输入 | 卷积核在W维（宽方向）的滑动步长。取值范围 [1, 63]。 |
| stride_h | 输入 | 卷积核在H维（高方向）的滑动步长。取值范围 [1, 63]。 |
| filter_w | 输入 | 卷积核在W维的尺寸（空洞前）。取值范围 [1, 255]。配合filter_size_w共同表示9 bit完整尺寸。 |
| filter_h | 输入 | 卷积核在H维的尺寸（空洞前）。取值范围 [1, 255]。配合filter_size_h共同表示9 bit完整尺寸。 |
| dilation_filter_w | 输入 | W维空洞卷积扩张率。取值范围 [1, 255]。 |
| dilation_filter_h | 输入 | H维空洞卷积扩张率。取值范围 [1, 255]。 |
| `filter_size_w` | 输入 | 卷积核W维尺寸的第9 bit高位。<br>&nbsp;&nbsp;&bull; `false`：卷积核W维尺寸的高位为0，filter_w即为完整尺寸；<br>&nbsp;&nbsp;&bull; `true`：卷积核W维尺寸的高位为1，完整尺寸为256 + filter_w。 |
| `filter_size_h` | 输入 | 卷积核H维尺寸的第9 bit高位。<br>&nbsp;&nbsp;&bull; `false`：卷积核H维尺寸的高位为0，filter_h即为完整尺寸；<br>&nbsp;&nbsp;&bull; `true`：卷积核H维尺寸的高位为1，完整尺寸为256 + filter_h。 |
| transpose | 输入 | 转置控制位。本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。 |
| `f_matrix_ctrl` | 输入 | Feature Map描述寄存器选择位。<br>&nbsp;&nbsp;&bull; `false`：使用第一组Feature Map描述寄存器（描述第一个Feature Map的尺寸、padding值与repeat控制）；<br>&nbsp;&nbsp;&bull; `true`：使用第二组Feature Map描述寄存器（描述第二个Feature Map的尺寸、padding值与repeat控制，用于双源拼接场景）。 |
| channel_size | 输入 | Feature Map的通道数N。取值需按dtype对齐：b32为0、4或8的整数倍；b16为0、4、8或16的整数倍；b8为0、4、8、16或32的整数倍。取值范围 [0, 2^16−1]，不可为0。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- dst起始地址需512字节对齐（L0B Buffer对齐要求），否则会导致搬运异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），2D矩阵搬运模式下需32字节对齐；3D格式img2col搬运模式下按channel_size×sizeof(dtype) 对齐（channel_size≥dtype阈值时32字节对齐，channel_size较小时按对应字节数对齐）。不满足触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L0B Buffer容量上限：L0B Buffer总容量64KB，dst偏移+搬运大小不可越界，否则触发写溢出异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发异常。

### 2D矩阵搬运模式约束

- m_step或k_step为0时不执行搬运，本接口被视为NOP（空操作）。
- 2D矩阵搬运模式下不执行转置。如需转置，请使用对应的转置搬运接口。
- src_stride为有符号数，可为负值；负值表示源端沿K维反向取数。dst_stride不可为负值。
- 源矩阵在L1 Buffer中的占用不可超过L1 Buffer容量（含地址下溢），目的矩阵在L0B Buffer中的占用不可超过L0B Buffer容量，否则触发地址溢出异常。

### 3D格式img2col搬运模式约束

- 调用本模式前须先通过配套setter接口预先配置Feature Map尺寸、padding填充值、repeat控制与L1 Buffer容量上限等参数，再调用本接口，否则搬运行为未定义。
- 3D格式搬运到L0B Buffer时会自动进行转置，transpose参数无效，不支持非转置场景。
- k_extension、m_extension或channel_size为0时不执行搬运，本接口被视为NOP（空操作）。
- filter_w为0且filter_size_w为false，或filter_h为0且filter_size_h为false时不执行搬运，本接口被视为NOP（空操作）。
- 调用本模式前，必须通过[asc_set_l3d_rpt_b](../asc_set_l3d_rpt_b.md)设置目的端步长。
- stride_w不可为0，stride_h不可为0，否则触发异常。
- channel_size需按dtype对齐基准对齐：b32须为8的倍数或8的倍数加4、b16须为16的倍数或等于4、8、b8须为32的倍数或等于4、8、16，否则触发对齐异常。
- k_start_pt需按dtype对应分形列数对齐：b32为8的倍数、b16为16的倍数、b8为32的倍数，否则触发对齐异常。
- m_start_pt取值范围 [0, 32767]，传入>32767的值触发异常。
- b4 dtype在3D格式img2col搬运模式下不被支持。
- Feature Map尺寸（含padding）须≥卷积核尺寸（含dilation），即fm_w+pad_l+pad_r≥ (filter_w − 1) ×dilation_filter_w+1，fm_h+pad_t+pad_b≥ (filter_h − 1) ×dilation_filter_h+1，否则触发异常。
- fm_w×fm_h×channel_size×sizeof(dtype) 不可超过L1 Buffer容量，否则触发L1 Buffer容量溢出异常。
- 第一个卷积核窗口位置须≤Feature Map尺寸（含padding）− 卷积核尺寸（含dilation），否则触发异常。
- k_start_pt+k_extension不可超过img2col展开后矩阵的K维总尺寸，m_start_pt+m_extension不可超过M维总尺寸，否则触发异常。
- `stride_w`取值范围为[1, 63]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `stride_h`取值范围为[1, 63]，超出取值范围的值会被截断，导致搬运结果不符合预期。

## 3D数据格式说明

要求输入的feature map和filter的格式是NC1HWC0，其中C0是最低维度而且C0是固定值为16（对于u8/s8类型为32），C1=C/C0。

为了简化场景，以下场景假设输入的feature map的channel为4，即Ci=4。输入feature maps在L1 Buffer中的形状为(Hi,Wi,Ci)，经过load3dv1处理后在L0B Buffer的数据形状为(Wo\*Ho, Hk\*Wk\*Ci)。其中Wo和Ho是卷积后输出的shape，Hk和Wk是filter的shape。

直观的来看，img2col的过程就是filter在feature map上扫过，将对应feature map的数据展开成输出数据的每一行的过程。filter首先在W方向上滑动Wo步，然后在H方向上走一步然后重复以上过程，最终输出Wo * Ho行数据。下图中红色和黄色的数据分别代表第一行和第二行。数字表示原始输入数据，filter和输出数据三者之间的关联关系。可以看到，load3dv1首先在输入数据的Ci维度搬运对应于00的4个数，然后搬运对应于01的四个数，最终这一行的大小为Hk\*Wk\*Ci即3\*3\*4=36个数。

- 对应的feature map格式如下图：

![ ](../../figures/load3d_01.png)

- 对应的filter的格式如下图：

![ ](../../figures/load3d_02.png)

其中n为filter的个数，可以看出维度排布为(Hk,Wk,Ci,n)，但是需要注意的是下图的格式还需要根据Mmad中B矩阵的格式转换。

实际操作中，由于存储空间或者计算能力限制，我们通常会将整个卷积计算分块，一次只搬运并计算一小块数据。

![ ](../../figures/load3d_03.png)

对于L0B Buffer中的feature map来说有两种方案，水平分块和垂直分块。分别对应参数中repeatMode的0和1。

注：下图中的分形矩阵大小为4x4，实际应该为16x16 (对于u8/s8类型为16x32)

repeatMode =0时，每次repeat会改变在filter窗口中读取数据点的位置，然后跳到下一个C0的位置。

![ ](../../figures/load3d_04.png)

repeatMode =1的时候filter窗口中读取数据的位置保持不变，每个repeat在feature map中前进C0个元素。

![ ](../../figures/load3d_05.png)

## 调用示例

```cpp
__cb__ bfloat16_t dst[256];
__cbuf__ bfloat16_t src[256];
uint16_t m_start_position = 8;
uint16_t k_start_position = 2;
uint8_t m_step = 4;
uint8_t k_step = 4;
int16_t src_stride = 1;
uint16_t dst_stride = 1;
asc_copy_l12l0b(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```
