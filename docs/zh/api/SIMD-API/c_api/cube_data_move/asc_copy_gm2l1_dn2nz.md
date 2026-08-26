# asc_copy_gm2l1_dn2nz

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

将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换。源数据在Global Memory中以D维连续存储（DN格式），本接口按D维最外层、N维居中的顺序展开，以C0分块（32字节）为单位重排写入L1 Buffer（Nz格式），用于后续矩阵计算。当D维元素个数不超过4时可开启小C0模式，每个burst按4元素分组写入并对齐到4通道。

进行DN2NZ格式搬运前，需先通过[asc_set_gm2l1_nz_para](asc_set_gm2l1_nz_para.md)接口配置DN2Nz搬运的目的Nz矩阵的stride与DN矩阵搬运个数。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ <dtype>* dst,
                                            __gm__ <dtype>* src,
                                            uint64_t loop1_src_stride,
                                            uint8_t l2_cache_ctl,
                                            uint16_t n_value,
                                            uint32_t d_value,
                                            uint64_t loop4_src_stride,
                                            bool smallc0_en)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ <dtype>* dst,
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
// 示例：half类型的DN到Nz格式搬运。
__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ half* dst,
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
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为GM。源地址需按dtype字节对齐（b8：1字节、b16：2字节、b32：4字节）。 |
| loop1_src_stride | 输入 | D维相邻元素在源端（GM）的步长，单位字节。取值范围 [0, 2^40−1]。 |
| l2_cache_ctl | 输入 | 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。取值范围 [0, 15]。 |
| n_value | 输入 | N维元素个数。取值范围 [1, 2^16−1]。 |
| d_value | 输入 | D维元素个数。取值范围 [1, 2^21−1]。 |
| loop4_src_stride | 输入 | 源端第4维（最外层DN矩阵间）的步长，单位字节。取值范围 [0, 2^40−1]。 |
| smallc0_en | 输入 | 是否开启小C0模式。小C0模式下每个burst按4元素分组写入并对齐到4通道。<br>&nbsp;&nbsp;&bull; `false`：不开启小C0模式，按C0分块（32字节）写入；<br>&nbsp;&nbsp;&bull; `true`：开启小C0模式，仅当`d_value`≤4时有效，否则结果错误。 |

**表2** `l2_cache_ctl`取值说明 <a id="l2_cache_ctl_values"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为高替换优先级。 |
| 1 | Last模式 | &bull; 启用L2 Cache，并将分配的Cache Line标记为低替换优先级。<br>&bull; **Last模式暂不支持。**|
| 2 | Persistent模式 | &bull; 启用L2 Cache。已存入L2 Cache中的数据可能被替换，若需确保特定GM的数据始终保留在L2 Cache中，可采用驻留模式。<br>&bull; 被标记为驻留模式的Cache Line只能被其他同样标记为驻留模式的Cache Line替换。<br>&bull; **Persistent模式暂不支持。**|
| 4 | Disable模式 | 不启用L2 Cache，每次都直接从GM中读取，并保持已有Cache Line的状态不变。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- `dst`的起始地址要求32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- `src`的起始地址要求1字节对齐（Global Memory对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，`dst`偏移+搬运大小不可越界，否则触发异常。

### DN2NZ约束
- `loop1_src_stride`取值范围为[0, 2^40−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `l2_cache_ctl`支持0、1、2、4，未定义值会导致结果异常。
- `d_value`取值范围为[1, 2^21−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `n_value`取值范围为[1, 2^16-1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `loop4_src_stride`取值范围为[0, 2^40−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- 调用本接口前须先通过[asc_set_gm2l1_nz_para](asc_set_gm2l1_nz_para.md)接口配置DN2NZ搬运的目的Nz矩阵的stride与DN矩阵搬运个数。
- `smallc0_en`传入`true`时`d_value`必须≤4。小C0模式仅支持D维不超过4个元素。

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
//搬运关于最外层循环的步长为4个字节
constexpr uint64_t loop4_src_stride = 2;
//每一个C0数据块都会被pad至32个字节
constexpr bool smallc0_en = true;
// src表示源操作数的起始地址
__cbuf__ half dst[256];
asc_copy_gm2l1_dn2nz(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
```
