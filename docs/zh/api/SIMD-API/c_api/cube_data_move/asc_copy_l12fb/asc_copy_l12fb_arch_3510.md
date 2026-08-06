# asc_copy_l12fb

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

将L1 Buffer中的数据搬运到Fixpipe Buffer，供后续Fixpipe流水的随路量化与随路ReLU等处理消费。搬运的数据为随路量化和随路ReLU参数，数据搬运时格式和内容保持不变。源数据类型为`void`，无类型重载。

提供两种调用形式：
- **高维切分搬运**：指定`n_burst`、`len_burst`、`src_gap_size`、`dst_gap_size`，通过配置数据块个数、单个数据块长度以及源/目间隔实现连续与非连续搬运。
- **前n个数据搬运**：仅指定`size`，实现层等价于`n_burst=1`、`len_burst=size/ASC_C_API_ONE_DATABLOCK_SIZE`、`src_gap_size=0`、`dst_gap_size=0`，即按一段连续数据块搬运。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

- 前n个数据搬运
    ```cpp
    __aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
    ```

- 高维切分搬运
    ```cpp
    __aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size, uint16_t dst_gap_size)
    ```

- 同步搬运
    ```cpp
    __aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
    ```

## 参数说明

**表1** 参数说明（6参数高维切分搬运）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数在Fixpipe Buffer的起始地址。地址高4 bit为mem_block区段选择：0=Quant PRE区（量化前参数）、1=ReLU PRE区（ReLU前参数）、6=同0。区内偏移按mem_block重算。 |
| src | 输入 | 源操作数在L1 Buffer的起始地址。须32字节对齐。 |
| n_burst | 输入 | 待搬运的连续数据块个数。取值范围[1, 4095]，不可为0。 |
| len_burst | 输入 | 每个连续数据块的长度，单位64字节。取值范围[1, 65535]，不可为0。与参数类型一致，不会截断。 |
| src_gap_size | 输入 | 源端（L1 Buffer）相邻两次搬运之间的步长系数，从上一次搬运结束位置到下一次搬运起始位置。取值范围[0, 65535]。与参数类型一致，不会截断。 |
| dst_gap_size | 输入 | 目的端（Fixpipe Buffer）相邻两次搬运之间的步长系数，从上一次搬运结束位置到下一次搬运起始位置。取值范围[0, 65535]。与参数类型一致，不会截断。mem_block为Quant PRE区时须2对齐。 |

**表2** 参数说明（3参数前n个数据搬运）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数在Fixpipe Buffer的起始地址。 |
| src | 输入 | 源操作数在L1 Buffer的起始地址。须32字节对齐。 |
| size | 输入 | 搬运数据大小，单位字节。实现层转换为`len_burst = size / ASC_C_API_ONE_DATABLOCK_SIZE`（`ASC_C_API_ONE_DATABLOCK_SIZE`为32字节常量），等价为一段连续数据块搬运。实际搬运字节数为`len_burst × 64字节`。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- src起始地址需32字节对齐（L1 Buffer对齐要求），否则触发异常。
- dst的mem_block区段取值仅允许0（Quant PRE区）、1（ReLU PRE区）、6（同0），其他值触发异常。mem_block同时决定dst区段：0/6→Quant PRE区，1→ReLU PRE区。
- Fixpipe Buffer地址对齐：mem_block为Quant PRE区时dst起始地址需128字节对齐；mem_block为ReLU PRE区时需64字节对齐，否则触发异常。
- mem_block为Quant PRE区时，`len_burst`与`dst_gap_size`均须2对齐，否则触发异常。
- dst起始地址不得超过6144（Fixpipe Buffer各区段寻址上限），否则触发Fixpipe Buffer写越界异常。
- Fixpipe Buffer总容量上限为6KB。dst偏移与搬运大小之和越界时会截断处理，数据可能不完整，调用方需确保不越界。
- L1 Buffer总容量512KB，src偏移+搬运大小不可越界，否则触发地址越界异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。

### 高维切分搬运模式约束

- n_burst为0时不执行搬运，本接口被视为NOP（空操作）并报告警告。
- n_burst取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。

### 前n个数据搬运模式约束

- size为0时不执行搬运，本接口被视为NOP（空操作）并报告警告。
- 实际搬运字节数为`len_burst×64字节`，当size非64字节整数倍时实际搬运字节数大于size，调用方需确保dst与src在按64字节对齐的范围内可写可读。

## 调用示例

```cpp
constexpr uint16_t n_burst = 1;
constexpr uint16_t len_burst = 1;
constexpr uint16_t src_gap = 0;
constexpr uint16_t dst_gap = 1;
__cbuf__ half src[256];
__fbuf__ half dst[256];
asc_copy_l12fb(dst, src, n_burst, len_burst, src_gap, dst_gap);
```
