# asc_copy_l12bt

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

将MMAD指令的Bias数据从L1 Buffer搬运到BiasTable Buffer中。

## 函数原型

- 前n个数据搬运
    ```cpp
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
    ```

- 高维切分搬运
    ```cpp
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap)
    ```

- 同步搬运
    ```cpp
    __aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size)
    __aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的操作数起始地址。 |
| src | 输入 | 源操作数起始地址。 |
| size| 输入 | 搬运数据大小（字节）。|
| conv_control | 输入 | 是否开启随路转化。|
| n_burst | 输入 | 待搬运的连续传输数据块个数。取值范围：[1, 4095]。 |
| len_burst | 输入 | 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。取值范围：[1, 65535]。 |
| source_gap | 输入 | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |
| dst_gap | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需64字节对齐（BiasTable Buffer端口对齐要求），src起始地址需32字节对齐（L1 Buffer端口对齐要求），否则触发异常。
- BiasTable Buffer容量上限4KB，dst偏移+写入大小不可越界，否则触发写溢出异常。
- L1 Buffer容量上限512KB，src偏移+读取大小不可越界，否则触发异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- dtype仅支持`void`、`bfloat16_t`、`half`、`float`、`int32_t`，其他取值触发指令未定义异常。

### 高维切分搬运模式约束

- n_burst不可为0，否则触发异常。
- len_burst不可为0，否则触发异常。
- dtype为`float`或`int32_t`时，len_burst与dst_gap均必须2对齐（即取值为偶数），否则触发异常。
- conv_control使能（非0）时dtype必须为`half`，否则触发异常。
- 写入BT的数据大小与格式由dtype与conv_control共同决定：`bfloat16_t`源逐元素转换为`float`后写入（写入大小为读取大小的2倍）；`half`源使能转换时逐元素转换为`float`后写入（写入大小为读取大小的2倍），未使能转换时每个`half`扩展为32bit后写入（写入大小为读取大小的2倍，但每元素后跟占位数据）；`float`与`int32_t`源直拷，写入大小等于读取大小。
- n_burst传入>4095的值会按位宽截断处理（如4096→0、4097→1），硬件不报错。截断后实际生效值偏小，导致实际搬运的连续数据块数小于预期，部分数据未被搬运。截断后若值为0会同时触发n_burst不可为0的异常。
- conv_control传入非0/1的值仅最低位生效（如2→0即转换关闭、3→1即转换开启），硬件不报错。截断后实际生效的转换使能状态与预期不符，可能导致BT中数据格式与预期不一致。

### 连续数据搬运模式约束

- size取值需为32的整数倍，否则实现层按`len_burst=size/32`向下取整，末尾不足32字节的数据被丢弃。
- dtype为`float`或`int32_t`时，size/32需为偶数（等价于高维切分搬运模式下len_burst 2对齐要求），否则触发异常。

## 调用示例

```cpp
constexpr uint16_t n_burst = 1;
constexpr uint16_t len_burst = 1;
constexpr uint16_t source_gap = 0;
constexpr uint16_t dst_gap = 1;
constexpr uint16_t conv_control = 0;
__cbuf__ half src[256];
__biasbuf__ half dst[256];
uint64_t dst_addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 16;
asc_copy_l12bt(dst_addr, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
```
