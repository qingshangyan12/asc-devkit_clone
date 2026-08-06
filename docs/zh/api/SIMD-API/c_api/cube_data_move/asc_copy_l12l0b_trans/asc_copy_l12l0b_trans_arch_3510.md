# asc_copy_l12l0b_trans

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

将数据从L1 Buffer搬运到L0B Buffer，搬运过程中以大小为512字节的数据分形为单位进行二维分形矩阵转置。本接口按分形索引、重复次数、源侧步长、目的侧间隔与分形间间隔等参数组织搬运，适用于普通矩阵计算所需2D格式数据在L1 Buffer与L0B Buffer之间伴随转置的搬运场景。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ <dtype>* dst,
                                             __cbuf__ <dtype>* src,
                                             uint16_t index_id,
                                             uint8_t repeat,
                                             uint16_t src_stride,
                                             uint16_t dst_gap,
                                             uint16_t dst_frac_gap,
                                             uint16_t src_frac_gap)
__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ <dtype>* dst,
                                                  __cbuf__ <dtype>* src,
                                                  uint16_t index_id,
                                                  uint8_t repeat,
                                                  uint16_t src_stride,
                                                  uint16_t dst_gap,
                                                  uint16_t dst_frac_gap,
                                                  uint16_t src_frac_gap)
```

### dtype支持的数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```cpp
// 示例：half类型的转置搬运。
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst,
                                             __cbuf__ half* src,
                                             uint16_t index_id,
                                             uint8_t repeat,
                                             uint16_t src_stride,
                                             uint16_t dst_gap,
                                             uint16_t dst_frac_gap,
                                             uint16_t src_frac_gap)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的缓冲（L0B Buffer）的起始地址。dtype由重载决定，src与dst同类型。起始地址需512字节对齐。 |
| src | 输入 | 源缓冲（L1 Buffer）的起始地址。dtype与dst一致。起始地址需32字节对齐。 |
| index_id | 输入 | 源侧起始分形索引，即源矩阵中起始分形矩阵的序号（从0计起）。取值范围 [0, 2^16−1]。 |
| repeat | 输入 | 重复搬运次数。取值范围 [1, 255]，不可为0（取0时指令不执行，被跳过）。 |
| src_stride | 输入 | 相邻repeat之间源侧（L1 Buffer）的分形步长，单位512字节分形。取值范围 [0, 2^16−1]。 |
| dst_gap | 输入 | 相邻repeat之间目的侧（L0B Buffer）的分形间隔，单位512字节分形。底层解码时实际生效步长为传入值加1（即传入0表1个分形步长）。取值范围 [0, 2^16−1]。 |
| dst_frac_gap | 输入 | 同一repeat内转置后相邻分形之间目的侧（L0B Buffer）的分形间隔，单位512字节分形。底层解码时实际生效步长为传入值加1。仅b4/b8/b32 dtype下参与地址计算；b16/half/bfloat16_t等分形数为1的dtype下不参与地址计算。取值范围 [0, 2^16−1]。 |
| src_frac_gap | 输入 | 同一repeat内源侧（L1 Buffer）相邻分形之间的分形间隔，单位512字节分形。底层解码时实际生效步长为传入值加1。仅b4/b8/b32 dtype下参与地址计算；b16/half/bfloat16_t等分形数为1的dtype下不参与地址计算。取值范围 [0, 2^16−1]。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 开发者需要保证目的操作数转置后的分形没有重叠。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则触发地址对齐异常。
- dst起始地址需512字节对齐（L0B Buffer对齐要求），否则触发地址对齐异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+搬运大小不可越界，否则触发地址溢出异常。
- L0B Buffer容量上限：L0B Buffer总容量64KB，dst偏移+搬运大小不可越界，否则触发地址溢出异常。
- repeat为0时不执行搬运，本接口被视为NOP（空操作）。
- index_id、src_stride等参数任意迭代组合后的源侧分形索引不可小于0，否则触发异常。
- dtype仅支持`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`，其他dtype触发异常。

## 调用示例

```cpp
//设置源操作数和目的操作数，total_length指参与计算的数据长度
constexpr uint64_t total_length = 512;
__cbuf__ int32_t src[total_length];
__cb__ int32_t dst[total_length];

// 设置搬运过程中的配置
uint8_t n = 64;
uint8_t nBlockSize = 32;
uint8_t repeat = n / nBlockSize;
uint16_t index_id = 0;
uint16_t src_stride = 1;
uint16_t dst_gap = 1;
uint16_t dst_frac_gap = 15;
uint16_t src_frac_gap = 8;

asc_copy_l12l0b_trans(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
```
