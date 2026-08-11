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

负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。

对于不同的数据类型，对应的分形矩阵如下：

- 对于b4数据类型，每个数据分形在L0B Buffer中为一个64x16的矩阵。
- 对于b8数据类型，每个数据分形在L0B Buffer中为一个32×16的矩阵。
- 对于b16数据类型，每个数据分形为一个16×16的矩阵。
- 对于b32数据类型，每个数据分形在L0B Buffer中为一个8×16的矩阵。

本接口仅在AIC上执行有效，非AIC调用直接返回。

## 函数原型

### 2D搬运模式

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

### 2D转置搬运模式

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

## 参数说明

### 2D矩阵搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矩阵，存储位置为L0B Buffer。目的地址需512字节对齐。 |
| src | 输入 | 源矩阵，存储位置为L1 Buffer。源地址需32字节对齐。 |
| m_start_position | 输入 | 源矩阵行方向起始分形位置，单位为16个元素。取值范围 [0, 2^16−1]。 |
| k_start_position | 输入 | 源矩阵列方向起始分形位置，单位32字节。取值范围 [0, 2^16−1]。 |
| m_step | 输入 | 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围 [0, 255]。<br>转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：<br>&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；<br>&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；<br>&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；<br>&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。<br>**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| k_step | 输入 | 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围 [0, 255]。<br>转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：<br>&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；<br>&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。<br>**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| src_stride | 输入 | 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围 [−2^15, 2^15−1]。 |
| dst_stride | 输入 | 目标矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围 [1, 2^16−1]。 |

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
