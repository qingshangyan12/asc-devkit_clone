# asc_copy_gm2l1

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

将数据从Global Memory搬运到L1 Buffer。本接口提供两种参数列表完全不同的功能模式，用户按需选择对应重载：

- **高维切分搬运**：通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运，并支持在搬运过程中按通道padding模式对32字节粒度的数据插入或移除padding。数据搬运时格式和内容保持不变。
- **2D矩阵搬运模式**：以512字节的分形为单位，从Global Memory中按M维（行方向）与K维（通道方向）的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，用于普通矩阵计算所需的2D格式数据加载。

两种模式参数列表完全不同，用户必须按使用场景选择对应重载系列，不可混用。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

### 高维切分搬运模式

```cpp
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst,
                                      uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride,
                                      uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst,
                                           uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride,
                                           uint32_t dst_stride)
```

### 2D矩阵搬运模式

#### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_gm2l1(__cbuf__ <dtype>* dst,
                                      __gm__ <dtype>* src,
                                      uint32_t m_start_position,
                                      uint32_t k_start_position,
                                      uint16_t dst_stride,
                                      uint16_t m_step,
                                      uint16_t k_step,
                                      uint8_t decomp_mode,
                                      uint8_t l2_cache_ctl)
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ <dtype>* dst,
                                           __gm__ <dtype>* src,
                                           uint32_t m_start_position,
                                           uint32_t k_start_position,
                                           uint16_t dst_stride,
                                           uint16_t m_step,
                                           uint16_t k_step,
                                           uint8_t decomp_mode,
                                           uint8_t l2_cache_ctl)
```

#### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`void`。

#### 典型示例

```cpp
// 示例：bfloat16_t类型的2D矩阵搬运。
__aicore__ inline void asc_copy_gm2l1(__cbuf__ bfloat16_t* dst,
                                      __gm__ bfloat16_t* src,
                                      uint32_t m_start_position,
                                      uint32_t k_start_position,
                                      uint16_t dst_stride,
                                      uint16_t m_step,
                                      uint16_t k_step,
                                      uint8_t decomp_mode,
                                      uint8_t l2_cache_ctl)
```

## 参数说明

### 高维切分搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为GM。源地址需按dtype字节对齐。 |
| n_burst | 输入 | 待搬运的连续数据块个数。取值范围[1, 2^17-1]。 |
| len_burst | 输入 | 每个连续数据块的长度，单位32字节。取值范围 [1, 2^17-1]。 |
| `pad_func_mode` | 输入 | 4 bit值，指示通道上的padding功能，在32字节数据中插入padding值，或从每个32字节的读取数据中移除padding值。取值范围 [0, 8]：<br>&nbsp;&nbsp;&bull; `0`：不做padding；<br>&nbsp;&nbsp;&bull; `1`：每有1个字节的数据，插入31个padding值；<br>&nbsp;&nbsp;&bull; `2`：每有2个字节的数据，插入15个padding值；<br>&nbsp;&nbsp;&bull; `3`：每有4个字节的数据，插入14个padding值；<br>&nbsp;&nbsp;&bull; `4`：每有8个字节的数据，插入12个padding值；<br>&nbsp;&nbsp;&bull; `5`：每有16个字节的数据，插入8个padding值；<br>&nbsp;&nbsp;&bull; `6`：每有32个字节的数据，移除28个最高有效位数据，只保留4个最低有效位元素；<br>&nbsp;&nbsp;&bull; `7`：每有32个字节的数据，移除24个最高有效位数据，只保留8个最低有效位元素；<br>&nbsp;&nbsp;&bull; `8`：每有32个字节的数据，移除16个最高有效位数据，只保留16个最低有效位元素。 |
| src_stride | 输入 | 相邻连续数据块在源端（GM）的步长，单位32字节。取值范围 [0, 2^36−1]。 |
| dst_stride | 输入 | 相邻连续数据块在目的端（L1 Buffer）的步长，单位32字节。取值范围 [0, 2^17-1]。 |

### 2D矩阵搬运模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为GM。源地址需1字节对齐。 |
| m_start_position | 输入 | 源矩阵中首个分形在M维（行方向）的起始位置，单位为16个元素。取值范围 [0, 2^32−1]。 |
| k_start_position | 输入 | 源矩阵中首个分形在K维（通道方向）的起始位置，单位32字节。取值范围 [0, 2^32−1]。 |
| dst_stride | 输入 | 目的端（L1 Buffer）在K维相邻分形的步长，单位512字节。取值范围 [0, 4095]。 |
| m_step | 输入 | M维（行方向）加载步长，即一次加载的分形行数，单位为16个元素。取值范围 [1, 4095]。 |
| k_step | 输入 | K维（通道方向）加载步长，即一次加载的分形列数，单位32字节。取值范围 [1, 4095]。 |
| decomp_mode | 输入 | 保留参数，填0即可。 |
| l2_cache_ctl | 输入 | 配置数据在L2 Cache中的管理策略。取值说明请参见[表3](#table3)。 |

**表3** l2_cache_ctl取值说明 <a id="table3"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0 | NORMAL模式 | 启用L2 Cache，并且将分配的Cache Line标记为高替换优先级。 |
| 1 | LAST模式 | <br>&nbsp;&nbsp;&bull; 启用L2 Cache，并且将分配的Cache Line标记为低替换优先级；<br>&nbsp;&nbsp;&bull; **LAST模式功能，暂不支持。** |
| 2 | 驻留模式 | <br>&nbsp;&nbsp;&bull; 启用L2 Cache。已存入L2 Cache中的数据可能被替换，若需确保特定GM的数据始终保留在L2 Cache中，可采用驻留模式；<br>&nbsp;&nbsp;&bull; 注意，被标记为驻留模式的Cache Line只能被其他同样被标记为驻留模式的Cache Line替换；<br>&nbsp;&nbsp;&bull; **目前该驻留模式功能尚在开发中，暂不支持，计划于Ascend 950PR/Ascend 950DT上提供支持。** |
| 4 | 禁用模式 | 不启用L2 Cache，每次都直接从GM中读取，并且保持已有Cache Line的状态不变。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- `dst`起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- `src`起始地址需1字节对齐（Global Memory对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发异常。

### 高维切分搬运模式约束

- 对于插入padding值的场景：
    - `len_burst`的单位为32个字节的指的是写入L1 Buffer的实际数据数，它必须设置为1，此时一次仅从GM里读取1/2/4/8/16个字节的数据。
    - 每一次burst都是从GM中连续读取，因此不需要设置`src_stride`。
    - `dst_stride`以32个字节为单位。
- 对于移除padding值的场景：
    - 源操作数中一次burst操作的数据为`len_burst * 32`个字节，目的操作数中一次burst的实际数据为`len_burst * 4/8/16`个字节。
    - 每一次`burst`后都连续地往L1 Buffer中写入，因此不需要设置`dst_stride`。
    - `src_stride`以32个字节为单位。
- `pad_func_mode`取值范围为[0, 8]，未定义值会导致结果异常。
- `n_burst`取值范围为[1, 2^17-1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `len_burst`取值范围为[1, 2^17-1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `src_stride`取值范围为[0, 2^36−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `dst_stride`取值范围为[0, 2^17-1]，超出取值范围的值会被截断，导致搬运结果不符合预期。

### 2D矩阵搬运模式约束

- `m_step`或`k_step`为0时不执行搬运，本接口被视为NOP（空操作）。
- `decomp_mode`为保留参数，填0即可。
- `l2_cache_ctl`支持0、1、2、4，未定义值会导致结果异常。
- `dst_stride`取值范围为[0, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `m_step`取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `k_step`取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。

## 调用示例

```cpp
//待搬运的连续传输数据块个数为1
constexpr uint32_t n_burst = 1;
//待搬运的每个连续传输数据块的长度为128个字节
constexpr uint32_t len_burst = 4;
//不对数据做padding
constexpr uint8_t pad_func_mode = 0;
//输入输出数据中两个相邻连续数据块之间的距离为0
constexpr uint64_t src_stride = 0;
constexpr uint32_t dst_stride = 0;
// src表示源操作数的起始地址
__cbuf__ half dst[256];
asc_copy_gm2l1(dst, src, n_burst, len_burst, pad_func_mode, src_stride, dst_stride);
```
