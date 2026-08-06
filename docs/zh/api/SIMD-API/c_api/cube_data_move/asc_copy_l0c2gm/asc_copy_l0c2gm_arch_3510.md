# asc_copy_l0c2gm

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

将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步执行随路量化、随路激活、随路格式转换（NZ2ND/NZ2DN）等能力组合。源数据位于L0C Buffer，目的地址位于Global Memory，通过L0C Buffer到Global Memory的数据通路完成搬运。

本接口支持多种随路能力的组合，需通过配套setter接口预先配置量化参数、激活参数、通道参数等寄存器，再调用本接口完成搬运。随路能力是否生效由本接口的`enable_clip_relu_pre`、`quant_pre_mode`、`relu_pre_mode`、`enable_channel_split`、`enable_nz2nd`、`enable_nz2dn`、`broadcast_en`等使能参数与配套setter配置共同决定。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

本接口的随路能力需与配套setter接口按功能分组配合使用：
- 随路量化模式下，需通过`asc_set_l0c_copy_prequant`、`asc_set_l0c2gm_quant_post`预先配置前/后级量化参数；
- 随路激活模式下，需通过`asc_set_l0c2gm_relu_alpha`、`asc_set_l0c2gm_lrelu_alpha`预先配置ReLU/Leaky ReLU激活参数；
- NZ2ND/NZ2DN格式转换场景下，需通过`asc_set_l0c2gm_nz2nd`、`asc_set_l0c2gm_channel_para`预先配置通道与格式转换参数；
- 基础搬运配置需通过`asc_set_l0c2gm_config`预先配置。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ <dst_dtype>* dst, __cc__ <src_dtype>* src, uint16_t n_size, uint16_t m_size,
    uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre,
    uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ <dst_dtype>* dst, __cc__ <src_dtype>* src, uint16_t n_size, uint16_t m_size,
    uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre,
    uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
```

### dtype支持的数据类型

src dtype与dst dtype支持以下组合：

- `src_dtype`为`float`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`float`。
- `src_dtype`为`int32_t`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`half`、`bfloat16_t`、`int32_t`。

### 典型示例

```cpp
// 示例：将float类型数据转换为bfloat16_t类型后搬运。
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size,
    uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre,
    uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的地址（Global Memory）的起始地址。起始地址需1字节对齐。dtype由重载决定。 |
| src | 输入 | 源地址（L0C Buffer）的起始地址。起始地址需按源dtype字节对齐：src为`float`或`int32_t`时需64字节对齐，src为`half`时需32字节对齐。 |
| n_size | 输入 | 从L0C Buffer搬运的矩阵在N方向（通道方向）的元素个数。取值范围 [0, 2^12−1]，n_size的合法取值还需满足与输出dtype、`enable_channel_split`、`enable_nz2nd`的组合约束（见约束说明）。 |
| m_size | 输入 | 从L0C Buffer搬运的矩阵在M方向（行方向）的元素个数。取值范围 [0, 2^16−1]，可为任意大于0的值。当m_size不是16的倍数时，硬件会读取额外哑数据并在写回目的地址时丢弃。 |
| dst_stride | 输入 | 相邻两次loop1迭代之间目的地址（Global Memory）的步长，单位为元素。取值范围 [0, 2^32−1]。未开启NZ2ND/NZ2DN时为loop1的目的步长，loop2的目的步长由硬件按C0通道大小自动确定。 |
| src_stride | 输入 | 相邻两次loop1迭代之间源地址（L0C Buffer）的步长，单位为C0_size（= 16×sizeof(src dtype)，src为`float`/`int32_t`时为64字节，src为`half`时为32字节）。取值范围 [0, 2^16−1]。 |
| l2_cache_mode | 输入 | 配置数据在L2 Cache中的管理策略。取值范围 [0, 2^4−1]，取值说明请参见[表2](#table2)。 |
| enable_clip_relu_pre | 输入 | 前处理阶段Clip ReLU模式控制。取值0：不开启Clip ReLU；取值1：标量模式Clip ReLU，需搭配`relu_pre_mode`与量化功能一起使用。取值范围 [0, 1]。取值2、3为保留值，传入触发异常。 |
| unit_flag_mode | 输入 | UnitFlag细粒度同步机制控制位。取值0：关闭UnitFlag机制，硬件忽略unit-flag；取值2：开启UnitFlag机制，硬件检查但不置位unit-flag，读阻塞直至unit-flag为1；取值3：开启UnitFlag机制，硬件检查并置位unit-flag。取值1为保留值。取值范围 [0, 3]。 |
| quant_pre_mode | 输入 | 前处理阶段量化模式控制位，决定源数据在搬运过程中的量化转换方式（如`float`->`half`、`float`->`bfloat16_t`、`int32_t`->`int8_t`等）。具体取值含义需通过配套setter接口配置量化参数后使用。取值范围 [0, 2^5−1]，部分取值属于保留值或非法值，传入触发异常。 |
| relu_pre_mode | 输入 | 前处理阶段ReLU模式控制。取值0：不开启ReLU；取值1：normal ReLU；取值2：Scalar ReLU；取值3：Vector ReLU。取值范围 [0, 3]。 |
| enable_channel_split | 输入 | `float`输出通道拆分使能。• false：关闭`float`通道拆分；• true：开启`float`通道拆分。仅当输出dtype为`float`时可设为true，其他输出dtype设为true触发异常。 |
| enable_nz2nd | 输入 | NZ2ND格式转换使能（Feature Map NC1HWC0->NHWC，Weight C1HWC0->NHWC）。• false：关闭NZ2ND转换，按普通DMA搬运；• true：开启NZ2ND转换。 |
| quant_post | 输入 | 保留参数，填0即可。 |
| relu_post | 输入 | 保留参数，填0即可。 |
| clip_relu_post | 输入 | 保留参数，填0即可。 |
| eltwise_op | 输入 | 保留参数，填0即可。 |
| eltwise_antq_en | 输入 | 保留参数，填false即可。 |
| c0_pad_en | 输入 | 保留参数，填false即可。 |
| broadcast_en | 输入 | Element-wise操作中源2数据在M方向的广播使能。• false：不广播源2数据；• true：广播源2数据。 |
| enable_nz2dn | 输入 | NZ2DN格式转换使能（Feature Map NC1HWC0->NCHW，Weight C1HWC0->NCHW）。• false：关闭NZ2DN转换；• true：开启NZ2DN转换。开启NZ2DN时需通过配套setter接口预先配置loop0 src stride等通道参数。 |

**表2** l2_cache_mode取值说明 <a id="table2"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为默认替换优先级。 |
| 1 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为默认替换优先级。 |
| 2 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为默认替换优先级。 |
| 4 | not-alloc模式 | 不在L2 Cache中分配Cache Line，保持已有Cache Line状态不变。 |
| 8 | write-back-home模式 | 写回归属策略。 |
| 9 | write-back-home模式 | 写回归属策略。 |
| 10 | write-back-home模式 | 写回归属策略。 |
| 12 | write-through-share模式 | 写透共享策略。 |
| 13 | write-through-share模式 | 写透共享策略。 |
| 14 | write-through-share模式 | 写透共享策略。 |

取值3、5、6、7、11、15为保留值或未定义值，传入触发L2 cache策略异常。

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- src起始地址需按源dtype字节对齐：src为`float`或`int32_t`时需64字节对齐，src为`half`时需32字节对齐，否则触发异常。
- dst起始地址需1字节对齐，否则触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L0C Buffer容量上限：L0C Buffer总容量256KB，src偏移+搬运大小不可超过L0C Buffer容量，否则触发源地址越界异常。
- 量化与激活模式中使用的M1/M2/M3/M4缩放系数不可为INF/NaN或非规格化数，否则结果错误。
- `enable_channel_split`仅在输出dtype为`float`时可设为true，其他输出dtype设为true触发异常。
- 开启NZ2DN转换时，需通过配套setter接口预先配置loop0 src stride，且loop0 src stride不可为0，否则触发异常。
- `unit_flag_mode`传入>3的值会截断处理。

### 随路量化激活参数约束

- `quant_pre_mode`取值不可为保留值或非法值（含6、7、[17, 20]、[27, 30]、[35, 2^5−1] 等区间），否则触发异常。
- `relu_pre_mode`取值不可为保留值（[4, 7]），否则触发异常。
- `enable_clip_relu_pre`取值不可为保留值（[2, 3]），否则触发异常。
- `enable_clip_relu_pre`设为标量模式时需搭配`relu_pre_mode`与量化功能一起使用。
- `enable_clip_relu_pre`传入>3的值会截断处理（如4→0、5→1）。截断后若值落在2、3仍会触发保留值异常。
- `quant_pre_mode`传入>2^5−1的值会截断处理（如2^5→0、2^5+1→1）。截断后实际生效值偏小，导致实际生效的量化模式与预期不符，输出数据类型与数值错误。截断后若值落在保留值或非法值区间仍会触发异常。
- `relu_pre_mode`传入>7的值会截断处理。截断后若值落在4~7仍会触发保留值异常。

### n_size组合约束

- `n_size`传入>2^12−1的值会截断处理（如2^12→0、2^12+1→1）。截断后实际生效值偏小，导致N方向实际搬运元素数小于预期，部分元素未被搬运。
- 未开启`enable_channel_split`、未开启`enable_nz2nd`、未开启`enable_nz2dn`时：输出dtype为`bfloat16_t`/`half`/`int8_t`/`uint8_t`/`fp8_e4m3fn_t`/`fp8_e5m2_t`/`hifloat8_t`时n_size需为16的倍数；输出dtype为`int32_t`/`float`时n_size需为16的倍数。
- 开启`enable_channel_split`、未开启`enable_nz2nd`、未开启`enable_nz2dn`时：输出dtype为`float`时n_size需为8的倍数。
- 未开启`enable_channel_split`、开启`enable_nz2nd`时：输出dtype为`bfloat16_t`/`half`/`int8_t`/`uint8_t`/`fp8_e4m3fn_t`/`fp8_e5m2_t`/`hifloat8_t`/`int32_t`/`float`时n_size可为任意大于0的值。
- 未开启`enable_channel_split`、未开启`enable_nz2nd`、开启`enable_nz2dn`时：输出dtype为`bfloat16_t`/`half`/`int8_t`/`uint8_t`/`fp8_e4m3fn_t`/`fp8_e5m2_t`/`hifloat8_t`/`int32_t`/`float`时n_size可为任意大于0的值。

## 调用示例

```cpp
// dst是外部输入的float类型的GM地址。
constexpr uint64_t total_length = 128 * 64;
__cc__ float src[total_length];
uint16_t n_size = 64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t l2_cache_mode = 0;
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
asc_copy_l0c2gm(dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
    unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post,
    relu_post, clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
```
