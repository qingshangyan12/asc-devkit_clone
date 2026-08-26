# asc_copy_l12ub

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

将数据从L1 Buffer搬运到Unified Buffer，按数据块个数、单个数据块长度以及源/目间隔实现连续与非连续搬运。数据搬运时格式和内容保持不变。

本接口仅适用于AIC:AIV为1:2的Mix场景，通过`sub_blockid`选择目的Vector Core。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

- 高维切分搬运
    ```cpp
    __aicore__ inline void asc_copy_l12ub(__ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

- 同步搬运
    ```cpp
    __aicore__ inline void asc_copy_l12ub_sync(__ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst_addr | 输出 | 目的操作数，存储位置为UB。目的地址需32字节对齐。 |
| src_addr | 输入 | 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。 |
| `sub_blockid` | 输入 | 选择目的Vector Core。<br>&nbsp;&nbsp;&bull; `false`：AIV0；<br>&nbsp;&nbsp;&bull; `true`：AIV1。本接口仅适用于AIC:AIV为1:2的Mix场景。 |
| n_burst | 输入 | 待搬运的连续数据块个数。取值范围 [1, 4095]。 |
| len_burst | 输入 | 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。 |
| src_gap | 输入 | 源端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。 |
| dst_gap | 输入 | 目的端（UB）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_description_and_constraints.md#通用地址重叠约束)。
- 由于此接口实现了Cube核与Vector核间的数据搬运，所以如果核函数调用了该接口，则不能使用__cube__或__vector__的执行空间修饰符，否则会导致执行失败。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst_addr起始地址需32字节对齐（UB对齐要求），否则会导致搬运异常。
- src_addr起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+搬运大小不可越界，否则触发异常。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst偏移+搬运大小不可超过实际可用容量，否则触发写溢出异常。

### 搬运约束

- n_burst不可为0，否则触发异常。
- len_burst不可为0，否则触发异常。
- n_burst取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。

## 调用示例

```cpp
constexpr uint16_t n_burst = 1;
constexpr uint16_t len_burst = 1;
constexpr uint16_t src_gap = 0;
constexpr uint16_t dst_gap = 1;
__cbuf__ half src[256];
__ubuf__ half dst[256];
asc_copy_l12ub(dst, src, false, n_burst, len_burst, src_gap, dst_gap);
```
