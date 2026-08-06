# asc_copy_gm2l1_nd2nz

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

将数据从Global Memory搬运到L1 Buffer，并在搬运过程中将源端按N维连续排布的ND格式数据转换为L1 Buffer侧的Nz分形排布，使其满足后续矩阵计算对分形列连续性的要求。该接口也适用于卷积格式NHWC（ND）转换为NC1HWC0格式等转换场景。

进行ND2NZ格式搬运前，需先通过[asc_set_gm2l1_nz_para](../asc_set_gm2l1_nz_para.md)接口配置ND2NZ搬运的目的Nz矩阵的stride与ND矩阵搬运个数。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ <dtype>* dst,
                                            __gm__ <dtype>* src,
                                            uint64_t loop1_src_stride,
                                            uint8_t l2_cache_ctl,
                                            uint16_t n_value,
                                            uint32_t d_value,
                                            uint64_t loop4_src_stride,
                                            bool smallc0_en)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ <dtype>* dst,
                                                 __gm__ <dtype>* src,
                                                 uint64_t loop1_src_stride,
                                                 uint8_t l2_cache_ctl,
                                                 uint16_t n_value,
                                                 uint32_t d_value,
                                                 uint64_t loop4_src_stride,
                                                 bool smallc0_en)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```cpp
// 示例：half类型的ND到Nz格式搬运。
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ half* dst,
                                            __gm__ half* src,
                                            uint64_t loop1_src_stride,
                                            uint8_t l2_cache_ctl,
                                            uint16_t n_value,
                                            uint32_t d_value,
                                            uint64_t loop4_src_stride,
                                            bool smallc0_en)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数，存储位置为L1 Buffer，作为Nz排布写入起点。目的地址需32字节对齐。 |
| `src` | 输入 | 源操作数，存储位置为GM，作为ND（NHWC）连续排布数据起点。源地址需1字节对齐。 |
| `loop1_src_stride` | 输入 | N维相邻元素间的源端步长，单位字节，即源端单个ND矩阵中沿D方向一行的总字节数（src_D × sizeof(dtype)）。取值范围[0, 2^40−1]。 |
| `l2_cache_ctl` | 输入 | 配置数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 cache策略异常。取值说明见[表2](#table2)。 |
| `n_value` | 输入 | N维元素个数，即一次搬运的N维长度，取值范围[1, 65535]。 |
| `d_value` | 输入 | D维元素个数（C通道方向），取值范围[1, 2^21−1]。搬运过程中D维不足32字节对齐部分由硬件自动补零。|
| `loop4_src_stride` | 输入 | ND维（最外层H迭代）相邻矩阵间的源端步长，单位字节，即相邻两个源ND矩阵起始地址的偏移量。取值范围[0, 2^40−1]。 |
| `smallc0_en` | 输入 | 小C0模式使能，控制D维≤4时是否启用4通道对齐的小C0排布。<br>&nbsp;&nbsp;&bull; `false`：不开启小C0模式，按标准C0（32字节）排布搬运；<br>&nbsp;&nbsp;&bull; `true`：开启小C0模式，每个burst按4通道对齐填充，目的侧步长由硬件自动配置，仅当`d_value`≤4时合法，否则结果错误。 |

**表2** `l2_cache_ctl`取值说明 <a id="table2"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0 | NORMAL模式 | 启用L2 Cache，并将分配的Cache Line标记为高替换优先级。 |
| 1 | LAST模式 | 启用L2 Cache，并将分配的Cache Line标记为低替换优先级。 |
| 2 | 驻留模式 | 启用L2 Cache驻留模式，已存入L2 Cache中的数据可能被替换，若需确保特定数据始终保留可采用驻留模式。被标记为驻留模式的Cache Line只能被其他同样标记为驻留模式的Cache Line替换。 |
| 4 | 禁用模式 | 不启用L2 Cache，每次都直接从GM中读取，并保持已有Cache Line的状态不变。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- `dst`起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- `src`起始地址需1字节对齐（GM对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，`dst`偏移+搬运大小不可越界，否则触发异常。
- `nd_num`（通过配套接口设置）、`n_value`或`d_value`为0时不执行搬运，本接口被视为NOP（空操作）。
- b4数据类型按b8数据类型设置。
- `l2_cache_ctl`支持0、1、2、4，未定义值会导致结果异常。

### ND2NZ搬运约束

- `smallc0_en`传入`true`时`d_value`必须≤4。小C0模式仅支持D维不超过4个元素。
- `loop1_src_stride`取值范围为[0, 2^40−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `d_value`传入>2^21−1的值会截断处理（如2^21→0、2^21+1→1）。截断后实际生效值偏小，导致D维单次搬运元素数不足；截断后若值为0，本接口被视为NOP（空操作）。
- `loop4_src_stride`取值范围为[0, 2^40−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。

## 调用示例

```cpp
//搬运关于最内层循环的步长为4个字节
constexpr uint64_t loop1_src_stride = 4;
//L2 Cache采用NORMAL模式
constexpr uint8_t l2_cache_ctl = 1;
//N值为3
constexpr uint16_t n_value = 3;
//D值为3
constexpr uint32_t d_value = 3;
//搬运关于最外层循环的步长为2个字节
constexpr uint64_t loop4_src_stride = 2;
//每一个C0数据块都会被pad至32个字节
constexpr bool smallc0_en = true;
__cbuf__ half dst[256];
// src表示源操作数的起始地址
asc_copy_gm2l1_nd2nz(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
```
