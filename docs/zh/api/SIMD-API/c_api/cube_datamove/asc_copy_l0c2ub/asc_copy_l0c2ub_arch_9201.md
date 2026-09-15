# asc_copy_l0c2ub_arch_9201

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持。
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：不支持。
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持。
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持。
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持。
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持。
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持。
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持。
<!-- end id8 -->

## 功能说明

将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。

下图展示了随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径。下图中的F32-\>F16与F32-\>BF16为Cast，其余为随路scalar/tensor量化模式。

**图1** asc_copy_l0c2ub随路功能组合

![asc_copy_l0c2ub随路功能组合](../../../../figures/C_API_L0C2UB_Function_Combination_950.png)

本接口支持多种随路能力的组合，需通过配套接口预先配置量化参数、激活参数、通道参数等寄存器，再调用本接口完成搬运。

- Nz2ND格式转换场景下，需通过[asc_set_l0c_copy_nz_para](../asc_set_l0c_copy_nz_para.md)预先配置格式转换参数，并且需要搭配本接口`enable_nz2nd`使用；
- Nz2DN格式转换场景下，需通过[asc_set_l0c_copy_nz_para](../asc_set_l0c_copy_nz_para.md)、[asc_set_l0c_copy_channel_para](../asc_set_l0c_copy_channel_para.md)预先配置格式转换参数，并且需要搭配本接口`enable_nz2dn`使用；
- 随路scalar量化模式下，需通过[asc_set_l0c_copy_prequant](../asc_set_l0c_copy_prequant.md)设置随路scalar量化参数，并且需要搭配本接口`quant_pre_mode`使用；
- 随路tensor量化模式下，需通过[asc_set_l0c2gm_config](../asc_set_l0c2gm_config.md)设置随路tensor量化使用tensor的起始地址，其中量化tensor的每个元素都代表一个量化参数，并且需要搭配本接口`quant_pre_mode`使用；
- 随路激活模式下，需通过[asc_set_l0c2gm_relu_alpha](../asc_set_l0c2gm_relu_alpha.md)、[asc_set_l0c2gm_lrelu_alpha](../asc_set_l0c2gm_lrelu_alpha.md)预先配置ReLU/Leaky ReLU激活参数，并且需要搭配本接口`enable_clip_relu_pre`与`relu_pre_mode`使用；

`quant_pre_mode`量化模式参数支持的枚举值如下：

- NoQuant：不开启量化功能。
- DEQF16：int32_t量化成half，scalar量化。
- VDEQF16：int32_t量化成half，tensor量化。
- REQ4：int32_t量化成int4b_t，scalar量化。
- VREQ4：int32_t量化成int4b_t，tensor量化。
- REQ8：int32_t量化成int8_t/uint8_t，scalar量化。
- VREQ8：int32_t量化成int8_t/uint8_t，tensor量化。
- QS322BF16_PRE：int32_t量化成bfloat16_t，scalar量化。
- VQS322BF16_PRE：int32_t量化成bfloat16_t，tensor量化。
- QF322F16_PRE：float量化成half，scalar量化。
- VQF322F16_PRE：float量化成half，tensor量化。
- QF322BF16_PRE：float量化成bfloat16_t，scalar量化。
- VQF322BF16_PRE：float量化成bfloat16_t，tensor量化。
- F322F16：float cast成half，cast mode为CAST_RINT模式。
- F322BF16：float cast成bfloat16_t，cast mode为CAST_RINT模式。
- QF322S4_PRE：float量化成int4b_t，scalar量化。
- VQF322S4_PRE：float量化成int4b_t，tensor量化。
- QF322B8_PRE：float量化成int8_t/uint8_t，scalar量化。
- VQF322B8_PRE：float量化成int8_t/uint8_t，tensor量化。
- QF322FP8_PRE：float量化成fp8_e4m3fn_t，scalar量化。
- VQF322FP8_PRE：float量化成fp8_e4m3fn_t，tensor量化。
- QF322HIF8_PRE：float量化成hifloat8_t(Half to Away Round)，scalar量化。
- VQF322HIF8_PRE：float量化成hifloat8_t(Half to Away Round)，tensor量化。
- QF322HIF8_PRE_HYBRID：float量化成hifloat8_t(Hybrid Round)，scalar量化。
- VQF322HIF8_PRE_HYBRID：float量化成hifloat8_t(Hybrid Round)，tensor量化。
- QF322F32_PRE：float量化成float，scalar量化，精度可以达到双千分之一，无法达到双万分之一。
- VQF322F32_PRE：float量化成float，tensor量化，精度可以达到双千分之一，无法达到双万分之一。

本接口仅在AIC上执行有效。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       asc_unit_flag_mode unit_flag_mode,
                                       asc_quant_mode quant_pre_mode,
                                       asc_relu_pre_mode relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       bool enable_nz2dn,
                                       bool enable_clip_relu_pre)

__aicore__ inline void asc_copy_l0c2ub(__ubuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       bool quant_pre_rnd,
                                       asc_unit_flag_mode unit_flag_mode,
                                       asc_quant_mode quant_pre_mode,
                                       asc_relu_pre_mode relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       bool enable_nz2dn,
                                       bool enable_clip_relu_pre)
```

### dtype支持的数据类型

src dtype与dst dtype支持以下组合：

- `src_dtype`为`int32_t`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`half`、`bfloat16_t`、`int32_t`。
- `src_dtype`为`float`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`float`。

### 函数原型典型示例

```cpp
// 示例：将float类型数据转换为half类型后搬运。
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ half* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       asc_unit_flag_mode unit_flag_mode,
                                       asc_quant_mode quant_pre_mode,
                                       asc_relu_pre_mode relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       bool enable_nz2dn,
                                       bool enable_clip_relu_pre)

__aicore__ inline void asc_copy_l0c2ub(__ubuf__ half* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       bool quant_pre_rnd,
                                       asc_unit_flag_mode unit_flag_mode,
                                       asc_quant_mode quant_pre_mode,
                                       asc_relu_pre_mode relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       bool enable_nz2dn,
                                       bool enable_clip_relu_pre)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。 |
| src | 输入 | 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。 |
| n_size | 输入 | 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。<br>对于Nz输出场景：<br>&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。<br>&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。<br>对于ND输出场景：<br>&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。<br> |
| m_size | 输入 | 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。<br>对于DN输出场景：<br>&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。<br> |
| dst_stride | 输入 | 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。<br>&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。<br>&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。<br> |
| src_stride | 输入 | 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。 |
| quant_pre_rnd | 输入 | 预处理阶段量化舍入模式。该参数仅当`quant_pre_mode`取值为QF322F16_PRE、VQF322F16_PRE、QF322BF16_PRE、VQF322BF16_PRE、F322F16、F322BF16时起作用。取值说明如下：<br>&nbsp;&nbsp;&bull; `false`：向最近值舍入，遇到中间值时取偶数。<br>&nbsp;&nbsp;&bull; `true`：向最近值舍入，遇到中间值时取奇数。<br>|
| unit_flag_mode | 输入 | UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`：不开启UnitFlag。<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为`asc_unit_flag_mode::ENABLE_KEEP`或`asc_unit_flag_mode::ENABLE_UPDATE`。 |
| quant_pre_mode | 输入 | 预处理阶段量化模式，取值见[功能说明](#功能说明)。 |
| relu_pre_mode | 输入 | 预处理阶段ReLU模式控制，取值如下：<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`：不开启ReLU。<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`：开启Normal ReLU。<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`：开启Scalar ReLU。<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`：开启Vector ReLU。<br> |
| enable_channel_split | 输入 | 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。<br>&nbsp;&nbsp;&bull; `false`：不开启。<br>&nbsp;&nbsp;&bull; `true`：开启。<br> |
| enable_nz2nd | 输入 | Nz2ND格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。<br>&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。<br> |
| enable_nz2dn | 输入 | Nz2DN格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。<br>&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。<br> |
| enable_clip_relu_pre | 输入 | 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：<br>&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。<br>&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。<br> |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

### 通用约束

- 本接口非AIC调用直接返回。
- `dst`起始地址需要按照32字节对齐，否则触发异常。
- `src`起始地址需要按照64字节对齐，否则触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD与SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst偏移量与搬运大小之和不可超过实际可用容量，否则触发写溢出异常。
- L0C Buffer容量上限：L0C Buffer总容量256KB，src偏移量与搬运大小之和不可超过L0C Buffer容量，否则触发读溢出异常。
- Nz矩阵以16×16个元素为一个基本分形。边界分形中超出`n_size`或`m_size`指定范围的数据不属于有效搬运结果。

### 随路转换约束

- `n_size`、`m_size`、`dst_stride`需根据dtype与功能模式确定对齐约束，详见参数说明，不满足对齐约束会导致搬运结果不符合预期。
- src与dst dtype组合需与`quant_pre_mode`量化模式匹配，否则会导致搬运结果不符合预期。
- `enable_channel_split`仅在输出dtype为`float`且输出为Nz格式时可设为true。
- 量化与激活模式中使用的量化系数不可为INF/NAN和非规格化数，否则会导致量化激活结果错误。
- 开启Nz2DN转换时，需通过[asc_set_l0c_copy_channel_para](../asc_set_l0c_copy_channel_para.md)预先配置源矩阵步长，且源矩阵步长不可为0，否则会导致搬运异常。
- 开启Nz2DN转换时，仅当通过[asc_set_l0c_copy_channel_para](../asc_set_l0c_copy_channel_para.md)配置源矩阵步长为1时，可同时开启UnitFlag功能。
- `enable_clip_relu_pre`设为Clip ReLU（标量模式）时需搭配`relu_pre_mode`与量化功能一起使用。

## 调用示例

将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[`__NPU_ARCH__`](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

本示例占用32KB L1 Buffer、16KB L0A Buffer、16KB L0B Buffer、64KB L0C Buffer和64KB UB，并使用`EVENT_ID0`与同步资源编号`0x8`完成流水及核间同步。

<!-- npu="960" id9 -->
以Ascend 960PR/Ascend 960DT产品（对应NPU架构为`dav-9201`）为例，编译运行命令如下：

```bash
bisheng examples.asc -o main --npu-arch=dav-9201 && ./main
```
<!-- end id9 -->
