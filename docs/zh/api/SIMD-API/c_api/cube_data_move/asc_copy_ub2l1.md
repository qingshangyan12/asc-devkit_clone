# asc_copy_ub2l1

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

将数据从Unified Buffer（UB）搬运到L1 Buffer，按数据块个数、单个数据块长度以及源/目间隔实现连续与非连续搬运。数据搬运时格式和内容保持不变。本接口提供两种参数列表不同的功能模式，用户按需选择对应重载：

- **burst搬运模式**：通过配置数据块个数、单个数据块长度、源/目间隔实现UB到L1 Buffer的非连续搬运，支持多burst重复搬运。
- **连续搬运模式**：将一段连续数据从UB搬运到L1 Buffer，等价于burst搬运模式取n_burst=1、src_gap=0、dst_gap=0、len_burst=size/32的简化形式。

两种模式参数列表完全不同，用户必须按使用场景选择对应重载系列，不可混用。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

- 前n个数据搬运

```cpp
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
```

- 高维切分搬运

```cpp
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
```

- 同步搬运

```cpp
__aicore__ inline void asc_copy_ub2l1_sync(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
```

## 参数说明

### burst搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为UB。源地址需32字节对齐。 |
| n_burst | 输入 | 待搬运的连续数据块个数。取值范围 [1, 4095]。 |
| len_burst | 输入 | 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。 |
| src_gap | 输入 | 源端（UB）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。 |
| dst_gap | 输入 | 目的端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。 |

### 连续搬运模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为UB。源地址需32字节对齐。 |
| size | 输入 | 待搬运的连续数据总字节数，单位字节，需为32的整数倍。内部按size/32计算等效len_burst，单次搬运完成。取值范围 [32, 4095×32]，且需为32的整数倍。 |

## 返回值说明

无

## 流水类型

PIPE_MTE3

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 同步计算包含同步等待。

### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- dst起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- src起始地址需32字节对齐（UB对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发异常。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。src偏移+搬运大小不可超过实际可用容量，否则触发读溢出异常。

### burst搬运模式约束

- n_burst或len_burst为0时不执行搬运，本接口被视为NOP（空操作）。
- n_burst取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- len_burst、src_gap、dst_gap解码后均为32字节的整数倍，物理上不存在非32字节对齐的步长/长度取值。

### 连续搬运模式约束

- size需为32的整数倍，否则size/32截断后实际搬运字节数不足，多出非整32字节部分被丢弃。
- size为0时不执行搬运，本接口被视为NOP（空操作）。
- size/32取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。

## 调用示例

```cpp
// total_length指参与搬运的数据总长度。dst是外部输入的L1 Buffer内存。
constexpr uint64_t total_length = 128;
__ubuf__ half src[total_length];
asc_copy_ub2l1(dst, src, total_length * sizeof(half));
```
