# asc_copy_l0c2ub

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

头文件路径：`"c_api/cube_datamove/cube_datamove.h"`。

将矩阵计算结果从L0C Buffer搬运到Unified Buffer，并在搬运路径上执行随路量化与激活处理。本接口在搬运过程中可执行前处理阶段的量化（`quant_pre_mode`）、ReLU/Clip ReLU激活（`relu_pre_mode`、`enable_clip_relu_pre`）、`float` dtype通道拆分（`enable_channel_split`）、NZ2ND/NZ2DN格式转换（`enable_nz2nd`、`enable_nz2dn`）等随路能力组合；源操作数位于L0C Buffer，目的操作数位于Unified Buffer，dtype由重载决定，src与dst可为不同类型。

本接口仅AIC上执行有效，在AIV上调用为空操作。随路量化模式下，需通过[asc_set_l0c_copy_prequant](asc_set_l0c_copy_prequant.md)预先配置前处理量化参数；ReLU/LReLU激活模式下，需通过[asc_set_l0c2gm_relu_alpha](asc_set_l0c2gm_relu_alpha.md)、[asc_set_l0c2gm_lrelu_alpha](asc_set_l0c2gm_lrelu_alpha.md)预先配置激活参数；NZ2ND/NZ2DN循环搬运模式下，需通过[asc_set_l0c_copy_params](asc_set_l0c_copy_params.md)预先配置循环参数。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ <dst_dtype>* dst, __cc__ <src_dtype>* src, uint16_t n_size, uint16_t m_size,
    uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en,
    bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ <dst_dtype>* dst, __cc__ <src_dtype>* src, uint16_t n_size, uint16_t m_size,
    uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en,
    bool broadcast_en, bool enable_nz2dn)
```

### dtype支持的数据类型

src dtype与dst dtype支持以下组合：

- `src_dtype`为`int32_t`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`half`、`bfloat16_t`、`int32_t`。
- `src_dtype`为`float`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`float`。

### 典型示例

```cpp
// 示例：将float类型数据转换为half类型后搬运。
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
    uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en,
    bool broadcast_en, bool enable_nz2dn)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的缓冲（UB）的起始地址。dtype由重载决定。起始地址需32字节对齐。 |
| `src` | 输入 | 源缓冲（L0C Buffer）的起始地址。dtype由重载决定。起始地址需64字节对齐（`int32_t`或`float`源）。 |
| `n_size` | 输入 | 待搬运矩阵的N方向（通道方向）大小，单位元素个数。取值范围 [0, 4095]。N方向大小需满足按dtype与功能模式的对齐约束：`b8`输出在NZ2ND开启时需为32的倍数、其余场景需为16的倍数；`b16`输出需为16的倍数；`b32`输出在`enable_channel_split`关闭且NZ2ND开启时需为8的倍数、在`enable_channel_split`开启时需为8的倍数。 |
| `m_size` | 输入 | 待搬运矩阵的M方向（行方向）大小，单位元素个数。取值范围 [0, 2^16−1]。在普通DMA模式下可为任意大于0的值；当M不是16的倍数时，硬件会读取额外dummy数据并在写目的端时丢弃。NZ2DN开启时需满足按dtype的对齐约束：`b8`输出需为32的倍数、`b16`输出需为16的倍数、`b32`输出需为8的倍数。 |
| `dst_stride` | 输入 | 目的端（UB）相邻两次迭代间的步长，单位元素个数。取值范围 [0, 2^32−1]。需32字节对齐。当NZ2ND或NZ2DN关闭时为loop1的目的端步长，loop2的目的端步长由硬件按C0大小自动决定；当NZ2ND或NZ2DN开启时为loop2的目的端步长，loop1的目的端步长由硬件按C0大小自动决定。步长需满足按dtype的对齐约束：`b8`输出需为32的倍数且不可为0，`b16`输出需为16的倍数且不可为0，`b32`输出需为8的倍数且不可为0。 |
| `src_stride` | 输入 | 源端（L0C Buffer）相邻两次迭代间的步长，单位C0大小（`half`源时C0 = 32字节，`int32_t`或`float`源时C0 = 64字节）。取值范围 [0, 2^16−1]。若同一分形内不同C0的数据处于同一N方向分形，须关闭unit-flag机制。 |
| `dual_dst_ctrl` | 输入 | 保留参数，填0即可。 |
| `sub_blockid` | 输入 | 保留参数，填false即可。 |
| `enable_clip_relu_pre` | 输入 | 前处理阶段Clip ReLU模式控制。取值范围 [0, 3]。<br>&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU；<br>&nbsp;&nbsp;&bull; `1`：scalar模式Clip ReLU，需搭配normal relu一起使用且需要开启量化功能；<br>&nbsp;&nbsp;&bull; `2/3`：保留值，触发异常。开启时需通过配套setter接口预先配置Clip ReLU最大值。 |
| `unit_flag_mode` | 输入 | unit-flag同步机制控制。取值范围 [0, 3]。<br>&nbsp;&nbsp;&bull; `0`：禁用unit-flag，硬件忽略unit-flag；<br>&nbsp;&nbsp;&bull; `1`：保留值；<br>&nbsp;&nbsp;&bull; `2`：使能unit-flag，硬件检查但不设置unit-flag；<br>&nbsp;&nbsp;&bull; `3`：使能unit-flag，硬件检查并设置unit-flag。在LoopEnhance模式或NZ2DN开启且loop0 src stride不为1时须关闭。 |
| `quant_pre_mode` | 输入 | 前处理阶段量化模式控制，定义L0C Buffer源数据在搬运过程中的量化/反量化转换类型。取值范围 [0, 63]。具体取值与src/dst dtype组合的对应关系见CANN文档；非法取值（含6、7、[17, 20]、[27, 30]、[35, 63] 等）触发异常。开启时需通过配套setter接口预先配置量化参数。 |
| `relu_pre_mode` | 输入 | 前处理阶段ReLU模式控制。取值范围 [0, 7]。<br>&nbsp;&nbsp;&bull; `0`：不开启ReLU；<br>&nbsp;&nbsp;&bull; `1`：normal ReLU；<br>&nbsp;&nbsp;&bull; `2`：scalar ReLU；<br>&nbsp;&nbsp;&bull; `3`：vector ReLU；<br>&nbsp;&nbsp;&bull; `4`：PWL模式；<br>&nbsp;&nbsp;&bull; `5~7`：保留值，触发异常。开启scalar/vector ReLU时需通过配套setter接口预先配置ReLU alpha参数。 |
| `enable_channel_split` | 输入 | `float` dtype通道拆分使能。<br>&nbsp;&nbsp;&bull; `false`：关闭`float` dtype通道拆分；<br>&nbsp;&nbsp;&bull; `true`：开启`float` dtype通道拆分，仅dst dtype为`float`且`quant_pre_mode`取值为无转换（0）或`float`->`float`量化模式（14/15）时合法，其他组合开启触发异常。 |
| `enable_nz2nd` | 输入 | NZ2ND格式转换使能，将L0C Buffer中的NZ分形数据搬运到UB时转换为ND排布。<br>&nbsp;&nbsp;&bull; `false`：关闭NZ2ND转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2ND转换。开启时N方向大小需满足对应dtype的对齐约束。与`enable_nz2dn`互斥，不可同时开启。 |
| `quant_post` | 输入 | 保留参数，填0即可。 |
| `relu_post` | 输入 | 保留参数，填0即可。 |
| `clip_relu_post` | 输入 | 保留参数，填false即可。 |
| `eltwise_op` | 输入 | 保留参数，填0即可。 |
| `eltwise_antq_en` | 输入 | 保留参数，填false即可。 |
| `c0_pad_en` | 输入 | 保留参数，填false即可。 |
| `broadcast_en` | 输入 | element-wise运算时M维度广播使能。<br>&nbsp;&nbsp;&bull; `false`：关闭M维度广播；<br>&nbsp;&nbsp;&bull; `true`：开启M维度广播。本芯片变体下element-wise运算未启用，该参数无实际效果。 |
| `enable_nz2dn` | 输入 | NZ2DN格式转换使能，将L0C Buffer中的NZ分形数据搬运到UB时转换为NCHW排布。<br>&nbsp;&nbsp;&bull; `false`：关闭NZ2DN转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2DN转换。开启时M方向大小与loop0 src stride需满足对应dtype的对齐约束，loop0 src stride不可为0。与`enable_nz2nd`互斥，不可同时开启。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（UB对齐要求），否则触发异常。
- src起始地址需64字节对齐（L0C Buffer对齐要求，`int32_t`或`float`源），否则触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- **UB容量上限**：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst偏移+搬运大小不可超过实际可用容量，否则触发写溢出异常。
- **L0C Buffer容量上限**：L0C Buffer总容量256KB，src偏移+搬运大小不可超过L0C Buffer容量，否则触发读溢出异常。
- 量化与激活模式中的M1/M2/M3/M4 scale参数不可为INF/NaN或非规格化数，否则结果错误。
- dtype仅支持源为`int32_t`或`float`（L0C Buffer端`half`源在本芯片的Cube单元中不支持），dst dtype由重载决定，src与dst dtype组合需与`quant_pre_mode`量化模式匹配，否则触发异常。

### 随路量化激活搬运模式约束

- `n_size`、`m_size`、`dst_stride`需满足按dtype与功能模式（NZ2ND/NZ2DN/enable_channel_split等）的对齐约束（详见参数说明），违反约束触发异常。
- `dst_stride`不可为0（即使对应loop size为0），否则触发异常。
- `enable_clip_relu_pre`取值需为已定义模式（0/1之一），取值2/3触发异常。
- `unit_flag_mode`取值1为保留值，触发异常；取值需为0/2/3之一。
- `quant_pre_mode`取值需为已定义的合法量化模式，非法取值（含6、7、[17, 20]、[27, 30]、[35, 63] 等）触发异常。
- `relu_pre_mode`取值需为已定义模式（0/1/2/3/4之一），取值5~7触发异常。
- `enable_channel_split`仅在dst dtype为`float`且`quant_pre_mode`取值为无转换（0）或`float`→`float`量化模式（14/15）时合法，dst dtype非`float`或`quant_pre_mode`取其他值时开启触发异常。
- `enable_nz2nd`与`enable_nz2dn`互斥，不可同时开启。
- `enable_nz2nd`或`enable_nz2dn`开启时，须先通过配套setter接口预先配置loop3 size（大于0）与loop3 stride等循环参数，再调用本接口，否则触发异常。
- `enable_nz2dn`开启时loop0 src stride不可为0，否则触发异常。
- `enable_nz2dn`开启且loop0 src stride不为1时，`unit_flag_mode`须为0（关闭unit-flag）。
- `quant_post`、`relu_post`、`clip_relu_post`、`eltwise_op`、`eltwise_antq_en`、`c0_pad_en`、`dual_dst_ctrl`、`sub_blockid`为保留参数，须填0/false，传入非0值行为未定义。
- `n_size`传入>4095的值会截断处理（如4096→0、4097→1）。截断后实际生效值偏小，导致该维单次迭代搬运元素数不足，多出元素被丢弃且后续维度迭代起始地址错位。参数类型为`uint16_t`（0~2^16−1），截断后若值不满足按dtype的对齐约束仍会触发对齐异常。
- `enable_clip_relu_pre`传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的Clip ReLU模式与预期不符，前处理激活行为错误。参数类型为`uint8_t`（0~255），截断后若值为2/3仍会触发保留值非法配置异常。
- `unit_flag_mode`传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的同步机制与预期不符。参数类型为`uint8_t`（0~255），截断后若值为1仍会触发保留值非法配置异常。
- `quant_pre_mode`传入>63的值会截断处理。截断后实际生效值偏小，导致实际生效的量化模式与预期不符，转换后的数据与预期不一致。参数类型为`uint64_t`（0~2^64−1），截断后若值落在非法取值集合仍会触发异常。
- `relu_pre_mode`传入>7的值会截断处理。截断后实际生效值偏小，导致实际生效的ReLU模式与预期不符，前处理激活行为错误。参数类型为`uint8_t`（0~255），截断后若值为5~7仍会触发保留值非法配置异常。
- 开启`quant_pre_mode`、`relu_pre_mode`（scalar/vector模式）、`enable_clip_relu_pre`等随路能力时，须先调用配套setter接口预先配置量化参数、ReLU alpha、Clip ReLU最大值等，再调用本接口，否则硬件读取未配置的参数导致结果错误。

## 调用示例

```cpp
// dst和src分别对应目的操作数的输出地址和源操作数的输入地址
constexpr uint64_t total_length = 128 * 64;
__ubuf__ float dst[total_length];
__cc__ float src[total_length];
uint16_t n_size =   64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t dual_dst_ctrl = 0;
bool sub_blockid = false;
uint8_t enable_clip_relu_pre = 0;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre_mode = QuantMode_t::NoQuant;
uint8_t relu_pre_mode = 0;
bool enable_channel_split = false;
bool enable_nz2nd = false;
bool enable_nz2dn = false;
uint64_t quant_post = 0;  // 无效参数
uint8_t relu_post = 0;  // 无效参数
bool clip_relu_post = false;  // 无效参数
uint8_t eltwise_op = 0;  // 无效参数
bool eltwise_antq_en = false;  // 无效参数
bool c0_pad_en = false;  // 无效参数
bool broadcast_en = false;  // 无效参数
asc_copy_l0c2ub(dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
    enable_clip_relu_pre, unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split,
    enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op, eltwise_antq_en,
    c0_pad_en, broadcast_en, enable_nz2dn);
```
