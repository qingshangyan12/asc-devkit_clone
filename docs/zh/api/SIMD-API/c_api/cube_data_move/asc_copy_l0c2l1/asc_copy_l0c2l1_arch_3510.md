# asc_copy_l0c2l1

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

将Cube矩阵乘L0C Buffer中的中间结果搬运到L1 Buffer，并在传输路径上执行Fixpipe随路处理：随路量化（`quant_pre_mode`反量化/量化前处理）、随路ReLU/Clip ReLU激活（`relu_pre_mode`/`enable_clip_relu_pre`）、通道拆分（`enable_channel_split`，`float`输出拆分）、ND2NZ/NZ2DN随路格式转换（`enable_nz2nd`/`enable_nz2dn`）。本接口提供两种参数列表完全不同的功能模式，用户按需选择对应重载：

- **分形搬运模式**：dst类型为具体dtype（`int8_t`、`uint8_t`、`half`、`bfloat16_t`、`int32_t`、`float`），src类型为`int32_t`或`float`，按dst/src dtype组合提供9个重载，不配置L2 Cache策略，不启用广播与NZ2DN转换。
- **通用搬运模式**：dst类型为`void*`（dtype由src决定），src类型为`int32_t`或`float`，提供2个重载，支持配置L2 Cache策略（`l2_cache_mode`）、M维广播（`broadcast_en`）与NZ2DN随路格式转换（`enable_nz2dn`）。

两种模式参数列表完全不同，用户必须按使用场景选择对应重载系列，不可混用。本接口仅在AIC上执行有效，在AIV上调用为空操作。使用随路量化/反量化（量化前处理）前，需通过[asc_set_l0c2gm_config](../../cube_compute/asc_set_l0c2gm_config.md)、[asc_set_l0c_copy_prequant](../asc_set_l0c_copy_prequant.md)预先配置；使用随路ReLU/Leaky ReLU激活前，需通过[asc_set_l0c2gm_relu_alpha](../asc_set_l0c2gm_relu_alpha.md)、[asc_set_l0c2gm_lrelu_alpha](../asc_set_l0c2gm_lrelu_alpha.md)预先配置；使用通道拆分前，需通过[asc_set_l0c2gm_channel_para](../asc_set_l0c2gm_channel_para.md)预先配置；使用ND2NZ/NZ2DN格式转换前，需通过[asc_set_l0c_copy_params](../asc_set_l0c_copy_params.md)预先配置。

## 函数原型

### 分形搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       uint8_t eltwise_antq_en,
                                       bool c0_pad_en)
```

#### dtype支持的数据类型

src dtype与dst dtype支持以下组合：

- `src_dtype`为`int32_t`时，`dst_dtype`支持`int8_t`、`uint8_t`、`half`、`int32_t`。
- `src_dtype`为`float`时，`dst_dtype`支持`int8_t`、`uint8_t`、`half`、`bfloat16_t`、`float`。

#### 典型示例

```cpp
// 示例：将float类型数据转换为half类型后搬运。
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ half* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       uint8_t eltwise_antq_en,
                                       bool c0_pad_en)
```

### 通用搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t l2_cache_mode,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       bool eltwise_antq_en,
                                       bool c0_pad_en,
                                       bool broadcast_en,
                                       bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ <dst_dtype>* dst,
                                            __cc__ <src_dtype>* src,
                                            uint16_t n_size,
                                            uint16_t m_size,
                                            uint32_t dst_stride,
                                            uint16_t src_stride,
                                            uint8_t l2_cache_mode,
                                            uint8_t enable_clip_relu_pre,
                                            uint8_t unit_flag_mode,
                                            uint64_t quant_pre_mode,
                                            uint8_t relu_pre_mode,
                                            bool enable_channel_split,
                                            bool enable_nz2nd,
                                            uint64_t quant_post,
                                            uint8_t relu_post,
                                            bool clip_relu_post,
                                            uint8_t eltwise_op,
                                            bool eltwise_antq_en,
                                            bool c0_pad_en,
                                            bool broadcast_en,
                                            bool enable_nz2dn)
```

#### dtype支持的数据类型

`dst_dtype`仅支持`int4b_t`，`src_dtype`支持`int32_t`、`float`。

#### 典型示例

```cpp
// 示例：将float类型数据转换为int4b_t类型后同步搬运。
__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ int4b_t* dst,
                                            __cc__ float* src,
                                            uint16_t n_size,
                                            uint16_t m_size,
                                            uint32_t dst_stride,
                                            uint16_t src_stride,
                                            uint8_t l2_cache_mode,
                                            uint8_t enable_clip_relu_pre,
                                            uint8_t unit_flag_mode,
                                            uint64_t quant_pre_mode,
                                            uint8_t relu_pre_mode,
                                            bool enable_channel_split,
                                            bool enable_nz2nd,
                                            uint64_t quant_post,
                                            uint8_t relu_post,
                                            bool clip_relu_post,
                                            uint8_t eltwise_op,
                                            bool eltwise_antq_en,
                                            bool c0_pad_en,
                                            bool broadcast_en,
                                            bool enable_nz2dn)
```

## 参数说明

### 分形搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的缓冲（L1 Buffer）的起始地址，dtype由重载决定。起始地址需32字节对齐。 |
| src | 输入 | 源缓冲（L0C Buffer）的起始地址。src为`int32_t`或`float`时均需64字节对齐。 |
| n_size | 输入 | N维（通道方向）搬运大小，单位元素个数。取值范围 [1, 4095]，不可为0。N维大小需按dst dtype满足对齐约束：非NZ2ND模式下，`b8`和`b16`需为16的倍数，`b32`需为8的倍数；NZ2ND模式下，`b8`需为32的倍数，`b16`需为16的倍数，`b32`需为8的倍数。 |
| m_size | 输入 | M维（行方向）搬运大小，单位元素个数。取值范围 [1, 2^16−1]，不可为0。NZ2DN模式下需按dst dtype满足对齐约束：`b8`需为32的倍数，`b16`需为16的倍数，`b32`需为8的倍数；非NZ2DN模式下可为任意大于0的值。 |
| dst_stride | 输入 | 相邻分形行在目的端（L1 Buffer）的步长，单位元素个数。取值范围 [0, 2^32−1]。需32字节对齐（按dst dtype倍数：`b8`需为32的倍数，`b16`需为16的倍数，`b32`需为8的倍数），且即使循环次数为0也不可为0。 |
| src_stride | 输入 | 相邻分形行在源端（L0C Buffer）的步长，单位C0_size（=16×sizeof(src dtype)，src为`int32_t`或`float`时为64字节）。取值范围 [0, 2^16−1]。若不同C0（N方向）数据属于同一分形，需关闭unit-flag机制。 |
| `enable_clip_relu_pre` | 输入 | 预处理阶段Clip ReLU模式控制。取值范围 [0, 3]。<br>&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU；<br>&nbsp;&nbsp;&bull; `1`：Scalar模式Clip ReLU，需搭配`relu_pre_mode`与`quant_pre_mode`一起使用；<br>&nbsp;&nbsp;&bull; `2/3`：保留值，不可传入，否则触发异常。 |
| `unit_flag_mode` | 输入 | unit-flag机制控制位，用于基于内存块的细粒度数据同步。取值范围 [0, 3]。<br>&nbsp;&nbsp;&bull; `0`：关闭unit-flag机制，硬件忽略unit-flag；<br>&nbsp;&nbsp;&bull; `1`：保留值，不可传入；<br>&nbsp;&nbsp;&bull; `2`：开启unit-flag机制，硬件检查但不设置unit-flag，若unit-flag为0则读阻塞直至为1；<br>&nbsp;&nbsp;&bull; `3`：开启unit-flag机制，硬件检查并设置unit-flag，读后将其清零。NZ2DN模式且src_stride不为1时不可开启unit-flag。 |
| quant_pre_mode | 输入 | 预处理阶段量化/反量化模式控制。取值范围 [0, 63]。具体取值对应反量化/量化前处理模式（如`int32_t`反量化、`float`量化为`int8_t`、`uint8_t`、`float`反量化为`half`等），需与src dtype匹配，否则触发异常。部分取值为保留值（如6、7、17~20、27~30、35~63等），不可传入。 |
| `relu_pre_mode` | 输入 | 预处理阶段ReLU模式控制。取值范围 [0, 7]。<br>&nbsp;&nbsp;&bull; `0`：不开启ReLU；<br>&nbsp;&nbsp;&bull; `1`：normal ReLU；<br>&nbsp;&nbsp;&bull; `2`：Scalar ReLU；<br>&nbsp;&nbsp;&bull; `3`：Vector ReLU；<br>&nbsp;&nbsp;&bull; `4~7`：保留值，不可传入，否则触发异常。 |
| `enable_channel_split` | 输入 | `float`输出通道拆分控制。<br>&nbsp;&nbsp;&bull; `false`：不开启通道拆分；<br>&nbsp;&nbsp;&bull; `true`：开启`float`通道拆分，仅dst dtype为`float`且`quant_pre_mode`取值与`float`输出匹配（不开启量化或`float`->`float`量化前处理模式）时合法，否则触发异常。 |
| `enable_nz2nd` | 输入 | NZ2ND随路格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：不开启NZ2ND转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2ND转换，将Feature Map NC1HWC0转NHWC、Weight C1HWC0转NHWC。与`enable_nz2dn`互斥，同时使能触发异常。 |
| quant_post | 输入 | 保留参数，填0即可。 |
| relu_post | 输入 | 保留参数，填0即可。 |
| clip_relu_post | 输入 | 保留参数，填0即可。 |
| eltwise_op | 输入 | 保留参数，填0即可。 |
| eltwise_antq_en | 输入 | 保留参数，填0即可。 |
| c0_pad_en | 输入 | 保留参数，填0即可。 |

### 通用搬运模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的缓冲（L1 Buffer）的起始地址，dtype由src重载决定（`int32_t`或`float`）。起始地址需32字节对齐。 |
| src | 输入 | 源缓冲（L0C Buffer）的起始地址。src为`int32_t`或`float`时均需64字节对齐。 |
| n_size | 输入 | N维（通道方向）搬运大小，单位元素个数。取值范围 [1, 4095]，不可为0。N维大小需按dst dtype满足对齐约束：`b32`需为8的倍数（NZ2ND模式下同样需为8的倍数）。 |
| m_size | 输入 | M维（行方向）搬运大小，单位元素个数。取值范围 [1, 2^16−1]，不可为0。NZ2DN模式下需为8的倍数；非NZ2DN模式下可为任意大于0的值。 |
| dst_stride | 输入 | 相邻分形行在目的端（L1 Buffer）的步长，单位元素个数。取值范围 [0, 2^32−1]。需32字节对齐（`b32`需为8的倍数），且即使循环次数为0也不可为0。 |
| src_stride | 输入 | 相邻分形行在源端（L0C Buffer）的步长，单位C0_size（=16×sizeof(src dtype)，src为`int32_t`或`float`时为64字节）。取值范围 [0, 2^16−1]。若不同C0（N方向）数据属于同一分形，需关闭unit-flag机制。 |
| l2_cache_mode | 输入 | 配置数据在L2 Cache中的管理策略。取值说明请参见[表3](#table3)。 |
| `enable_clip_relu_pre` | 输入 | 预处理阶段Clip ReLU模式控制。取值范围 [0, 3]。<br>&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU；<br>&nbsp;&nbsp;&bull; `1`：Scalar模式Clip ReLU，需搭配`relu_pre_mode`与`quant_pre_mode`一起使用；<br>&nbsp;&nbsp;&bull; `2/3`：保留值，不可传入，否则触发异常。 |
| `unit_flag_mode` | 输入 | unit-flag机制控制位，用于基于内存块的细粒度数据同步。取值范围 [0, 3]。<br>&nbsp;&nbsp;&bull; `0`：关闭unit-flag机制，硬件忽略unit-flag；<br>&nbsp;&nbsp;&bull; `1`：保留值，不可传入；<br>&nbsp;&nbsp;&bull; `2`：开启unit-flag机制，硬件检查但不设置unit-flag，若unit-flag为0则读阻塞直至为1；<br>&nbsp;&nbsp;&bull; `3`：开启unit-flag机制，硬件检查并设置unit-flag，读后将其清零。NZ2DN模式且src_stride不为1时不可开启unit-flag。 |
| quant_pre_mode | 输入 | 预处理阶段量化/反量化模式控制。取值范围 [0, 63]。具体取值对应反量化/量化前处理模式（如`int32_t`反量化、`float`量化为`int8_t`、`uint8_t`、`float`反量化为`half`等），需与src dtype匹配，否则触发异常。部分取值为保留值（如6、7、17~20、27~30、35~63等），不可传入。 |
| `relu_pre_mode` | 输入 | 预处理阶段ReLU模式控制。取值范围 [0, 7]。<br>&nbsp;&nbsp;&bull; `0`：不开启ReLU；<br>&nbsp;&nbsp;&bull; `1`：normal ReLU；<br>&nbsp;&nbsp;&bull; `2`：Scalar ReLU；<br>&nbsp;&nbsp;&bull; `3`：Vector ReLU；<br>&nbsp;&nbsp;&bull; `4~7`：保留值，不可传入，否则触发异常。 |
| `enable_channel_split` | 输入 | `float`输出通道拆分控制。<br>&nbsp;&nbsp;&bull; `false`：不开启通道拆分；<br>&nbsp;&nbsp;&bull; `true`：开启`float`通道拆分，仅dst dtype为`float`且`quant_pre_mode`取值与`float`输出匹配（不开启量化或`float`->`float`量化前处理模式）时合法，否则触发异常。 |
| `enable_nz2nd` | 输入 | NZ2ND随路格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：不开启NZ2ND转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2ND转换，将Feature Map NC1HWC0转NHWC、Weight C1HWC0转NHWC。与`enable_nz2dn`互斥，同时使能触发异常。 |
| quant_post | 输入 | 保留参数，填0即可。 |
| relu_post | 输入 | 保留参数，填0即可。 |
| clip_relu_post | 输入 | 保留参数，填0即可。 |
| eltwise_op | 输入 | 保留参数，填0即可。 |
| eltwise_antq_en | 输入 | 保留参数，填0即可。 |
| c0_pad_en | 输入 | 保留参数，填0即可。 |
| `broadcast_en` | 输入 | element-wise操作前M维广播使能（广播源2数据）。<br>&nbsp;&nbsp;&bull; `false`：不开启M维广播；<br>&nbsp;&nbsp;&bull; `true`：开启M维广播。本芯片变体下element-wise操作为保留功能，此参数实际不生效。 |
| `enable_nz2dn` | 输入 | NZ2DN随路格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：不开启NZ2DN转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2DN转换，将Feature Map NC1HWC0转NCHW、Weight C1HWC0转NCHW。与`enable_nz2nd`互斥，同时使能触发异常。 |

**表3** l2_cache_mode取值说明 <a id="table3"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0 | NORMAL模式 | 启用L2 Cache，并且将分配的Cache Line标记为高替换优先级。|
| 1 | LAST模式 | <br>&nbsp;&nbsp;&bull; 启用L2 Cache，并且将分配的Cache Line标记为低替换优先级；<br>&nbsp;&nbsp;&bull; **LAST模式功能，暂不支持。**|
| 2 | 驻留模式 | <br>&nbsp;&nbsp;&bull; 启用L2 Cache。已存入L2 Cache中的数据可能被替换，若需确保特定GM的数据始终保留在L2 Cache中，可采用驻留模式；<br>&nbsp;&nbsp;&bull; 注意，被标记为驻留模式的Cache Line只能被其他同样被标记为驻留模式的Cache Line替换；<br>&nbsp;&nbsp;&bull; **目前该驻留模式功能尚在开发中，暂不支持，计划于Ascend 950PR/Ascend 950DT上提供支持。**|
| 4 | 禁用模式 | 不启用L2 Cache，每次都直接从GM中读取，并且保持已有Cache Line的状态不变。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- src起始地址需64字节对齐（L0C Buffer对齐要求，src为`int32_t`或`float`时），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- 接口使用随路量化、随路ReLU/Clip ReLU、通道拆分、NZ2ND/NZ2DN等随路能力前，须先调用配套setter接口预先配置对应参数（量化系数、ReLU alpha、Clip ReLU上限、通道步长等），再调用本搬运接口，否则随路处理使用未初始化的配置值产生错误结果。
- L0C Buffer容量上限：L0C Buffer总容量256KB，src偏移+搬运大小不可越界，否则触发异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发异常。

### 分形搬运模式约束

- n_size不可为0，否则触发异常。
- m_size不可为0，否则触发异常。
- dst_stride即使循环次数为0也不可为0，否则触发异常；同时需按dst dtype满足32字节对齐：`b8`需为32的倍数，`b16`需为16的倍数，`b32`需为8的倍数。
- n_size需按dst dtype与`enable_nz2nd`状态满足对齐约束：非NZ2ND模式下`b8`和`b16`需为16的倍数、`b32`需为8的倍数；NZ2ND模式下`b8`需为32的倍数、`b16`需为16的倍数、`b32`需为8的倍数，否则触发异常。
- m_size在`enable_nz2dn`为true时需按dst dtype满足对齐约束：`b8`需为32的倍数，`b16`需为16的倍数，`b32`需为8的倍数，否则触发异常。
- `enable_channel_split`为true时仅dst dtype为`float`且`quant_pre_mode`取值与`float`输出匹配（不开启量化或`float`→`float`量化前处理模式）时合法，否则触发异常。
- `quant_pre_mode`取值需与src dtype匹配，且不可取保留值（如6、7、17~20、27~30、35~63等），否则触发异常。
- `relu_pre_mode`不可取4~7保留值，否则触发异常。
- `enable_clip_relu_pre`不可取2/3保留值，否则触发异常。
- `quant_post`、`relu_post`、`clip_relu_post`、`eltwise_op`、`eltwise_antq_en`、`c0_pad_en`为保留参数，填0即可，传入非0值不生效。
- n_size传入>4095的值会截断处理（如4096→0、4097→1）。截断后实际生效值偏小，导致该维单次迭代搬运元素数不足，多出元素被丢弃且后续维度迭代起始地址错位。参数类型为`uint16_t`（0~2^16−1），截断后若值为0会同时触发n_size不可为0的异常。
- enable_clip_relu_pre传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理Clip ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在2/3仍会触发enable_clip_relu_pre不可取保留值的异常。
- unit_flag_mode传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的unit-flag模式与预期不符。参数类型为`uint8_t`（0~255），截断后若值为1仍会触发unit_flag_mode保留值异常。
- quant_pre_mode传入>63的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理量化模式与预期不符，可能产生错误的量化结果。参数类型为`uint64_t`（0~2^64−1），截断后若落在保留值仍会触发quant_pre_mode保留值异常。
- relu_pre_mode传入>7的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在4~7仍会触发relu_pre_mode保留值异常。

### 通用搬运模式约束

- n_size不可为0，否则触发异常。
- m_size不可为0，否则触发异常。
- dst_stride即使循环次数为0也不可为0，否则触发异常；同时需按dst dtype满足32字节对齐（`b32`需为8的倍数）。
- n_size需按dst dtype与`enable_nz2nd`状态满足对齐约束：`b32`需为8的倍数（非NZ2ND与NZ2ND模式均同），否则触发异常。
- m_size在`enable_nz2dn`为true时需为8的倍数（dst dtype为`b32`），否则触发异常。
- `enable_channel_split`为true时仅dst dtype为`float`且`quant_pre_mode`取值与`float`输出匹配（不开启量化或`float`→`float`量化前处理模式）时合法，否则触发异常。
- `quant_pre_mode`取值需与src dtype匹配，且不可取保留值（如6、7、17~20、27~30、35~63等），否则触发异常。
- `relu_pre_mode`不可取4~7保留值，否则触发异常。
- `enable_clip_relu_pre`不可取2/3保留值，否则触发异常。
- `enable_nz2nd`与`enable_nz2dn`互斥，同时使能触发异常。
- `quant_post`、`relu_post`、`clip_relu_post`、`eltwise_op`、`eltwise_antq_en`、`c0_pad_en`为保留参数，填0即可，传入非0值不生效。
- l2_cache_mode支持0、1、2、4，未定义值会导致结果异常。其中取值1（LAST模式）暂不支持，取值2（驻留模式）尚在开发中暂不支持、计划于Ascend 950PR/Ascend 950DT上提供支持。
- n_size传入>4095的值会截断处理（如4096→0、4097→1）。截断后实际生效值偏小，导致该维单次迭代搬运元素数不足，多出元素被丢弃且后续维度迭代起始地址错位。参数类型为`uint16_t`（0~2^16−1），截断后若值为0会同时触发n_size不可为0的异常。
- enable_clip_relu_pre传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理Clip ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在2/3仍会触发enable_clip_relu_pre不可取保留值的异常。
- unit_flag_mode传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的unit-flag模式与预期不符。参数类型为`uint8_t`（0~255），截断后若值为1仍会触发unit_flag_mode保留值异常。
- quant_pre_mode传入>63的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理量化模式与预期不符，可能产生错误的量化结果。参数类型为`uint64_t`（0~2^64−1），截断后若落在保留值仍会触发quant_pre_mode保留值异常。
- relu_pre_mode传入>7的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在4~7仍会触发relu_pre_mode保留值异常。

## 调用示例

```cpp
// dst和src分别对应目的操作数的输出地址和源操作数的输入地址
constexpr uint64_t total_length = 128 * 64;
__cbuf__ int32_t dst[total_length];
__cc__ int32_t src[total_length];
// 其余入参均已默认数值传入
uint16_t n_size = 64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t enable_clip_relu_pre = 0;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre_mode = QuantMode_t::NoQuant;
uint8_t relu_pre_mode = 0;
bool enable_channel_split = false;
bool enable_nz2nd = false;
uint64_t quant_post = 0;  // 无效参数
uint8_t relu_post = 0;  // 无效参数
bool clip_relu_post = false;  // 无效参数
uint8_t eltwise_op = 0;  // 无效参数
uint8_t eltwise_antq_en = 0;  // 无效参数
bool c0_pad_en = false;  // 无效参数
asc_copy_l0c2l1(
    dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode,
    quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
    clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en);
```
