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

将数据从Unified Buffer (UB)搬运到L1 Buffer。

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

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L1 Buffer地址。 |
| src | 输入 | 源UB地址。 |
| size | 输入 | 搬运数据大小（字节）。 |
| n_burst | 输入 | 待搬运的连续传输数据块个数。取值范围：[1, 4095]。 |
| len_burst | 输入 | 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。 |
| src_gap | 输入 | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |
| dst_gap | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |

## 返回值说明

无

## 流水类型

PIPE_MTE3

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 同步计算包含同步等待。

### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- dst起始地址需32字节对齐（L1 Buffer端口对齐要求），否则搬运异常。
- src起始地址需32字节对齐（UB端口对齐要求），否则搬运异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发异常。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。src偏移+搬运大小不可超过实际可用容量，否则触发读溢出异常。

### burst搬运模式约束

- n_burst不可为0，否则触发异常。
- len_burst不可为0，否则触发异常。
- n_burst传入>4095的值会按位宽截断处理（如4096→0、4097→1），硬件不报错。截断后实际生效值偏小，导致实际搬运的连续数据块数小于预期，部分数据未被搬运。截断后若值为0会同时触发n_burst不可为0的异常。
- len_burst、src_gap、dst_gap解码后均为32字节的整数倍，物理上不存在非32字节对齐的步长/长度取值。

### 连续搬运模式约束

- size需为32的整数倍，否则size/32截断后实际搬运字节数不足，多出非整32字节部分被丢弃。
- size不可为0，否则无实际搬运（硬件不报错但无数据搬移）。
- size/32不可超过4095（即size不可超过4095×32字节），否则等效len_burst传入>4095的值会按位宽截断处理，硬件不报错。截断后实际生效值偏小，导致单次搬运字节数不足，需多次搬运才能搬完同一块数据。

## 调用示例

```cpp
// total_length指参与搬运的数据总长度。dst是外部输入的L1 Buffer内存。
constexpr uint64_t total_length = 128;
__ubuf__ half src[total_length];
asc_copy_ub2l1(dst, src, total_length * sizeof(half));
```
