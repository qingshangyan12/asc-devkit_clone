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

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将L1 Buffer中的偏置数据搬运到BiasTable Buffer，供矩阵计算单元（Mmad）作为bias输入读取。搬运过程按dtype做随路格式转换：`bfloat16_t`自动转换为`float`；`half`在使能转换时转换为`float`，未使能时每个`half`元素扩展为32 bit（高16 bit为占位数据）后写入；`float`与`int32_t`直拷，不做转换。源数据类型支持`void`、`bfloat16_t`、`half`、`float`、`int32_t`。

本接口提供两种参数列表完全不同的功能模式，用户按需选择对应重载：

- **高维切分搬运**：通过配置数据块个数、单个数据块长度、源/目间隔等搬运参数实现连续与非连续搬运。
- **连续数据搬运**：仅指定搬运字节数，实现层等价于固定`n_burst=1`、`len_burst=size/32`、源/目间隔为0的连续搬运。

本接口仅在AIC上执行有效。

## 函数原型

### 高维切分搬运模式

```c
// 占位符形式
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ <dtype>* src,
                                      uint16_t conv_control,
                                      uint16_t n_burst,
                                      uint16_t len_burst,
                                      uint16_t source_gap,
                                      uint16_t dst_gap)
```

#### dtype支持的数据类型

dtype支持的数据类型为`half`、`bfloat16_t`、`int32_t`、`float`、`void`。

#### 函数原型典型示例

```c
// 示例：bfloat16_t类型的高维切分搬运。
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ bfloat16_t* src,
                                      uint16_t conv_control,
                                      uint16_t n_burst,
                                      uint16_t len_burst,
                                      uint16_t source_gap,
                                      uint16_t dst_gap)
```

### 连续数据搬运模式

```c
// 占位符形式
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ <dtype>* src,
                                      uint32_t size)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst,
                                           __cbuf__ <dtype>* src,
                                           uint32_t size)
```

#### dtype支持的数据类型

dtype支持的数据类型为`half`、`bfloat16_t`、`int32_t`、`float`、`void`。

#### 函数原型典型示例

```c
// 示例：float类型的连续数据搬运。
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ float* src,
                                      uint32_t size)
```

## 参数说明

### 高维切分搬运模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。 |
| `conv_control` | 输入 | `half`源的随路格式转换使能控制。<br>&nbsp;&nbsp;&bull; `0`：不使能转换，每个`half`元素扩展为32 bit（高16 bit为占位数据）后写入BT；<br>&nbsp;&nbsp;&bull; `非0`：使能转换，每个`half`元素转换为`float`后写入BT。仅dtype为`half`时合法，其他dtype传入非0触发异常。传入非0/1的值仅最低位生效（如2->0即转换关闭、3->1即转换开启）。 |
| n_burst | 输入 | 待搬运的连续数据块个数。取值范围 [1, 4095]。 |
| len_burst | 输入 | 每个连续数据块的长度，单位32字节。取值范围 [1, 65535]。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。 |
| source_gap | 输入 | 源端（L1 Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。 |
| dst_gap | 输入 | 目的端（BiasTable Buffer）相邻两次搬运之间的间隔，单位32字节，从上一次搬运结束位置到下一次搬运起始位置。取值范围 [0, 65535]。`n_burst=1`时无意义，置0即可。dtype为`float`或`int32_t`时需2对齐（即取值为偶数），否则触发异常。 |

### 连续数据搬运模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为BiasTable Buffer。目的地址需64字节对齐。 |
| src | 输入 | 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。 |
| size | 输入 | 搬运数据大小，单位字节。实现层等价于固定`n_burst=1`、`len_burst=size/32`、源/目间隔为0的高维切分搬运。取值需为32的整数倍，否则向下取整导致末尾不足32字节的数据被丢弃。 |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

### 通用约束

- 本接口非AIC调用直接返回。
- dst起始地址需64字节对齐（BiasTable Buffer对齐要求），src起始地址需32字节对齐（L1 Buffer对齐要求），否则触发异常。
- BiasTable Buffer容量上限4KB，dst偏移+写入大小不可越界，否则触发写溢出异常。
- L1 Buffer容量上限512KB，src偏移+读取大小不可越界，否则触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。

### 高维切分搬运模式约束

- n_burst为0时不执行搬运，本接口被视为NOP（空操作）。
- src_gap和dst_gap取值范围为[0, 2^16−1]。
- dtype为`float`或`int32_t`时，len_burst与dst_gap均必须2对齐（即取值为偶数），否则触发异常。
- conv_control使能（非0）时dtype必须为`half`，否则触发异常。
- 写入BT的数据大小与格式由dtype与conv_control共同决定：`bfloat16_t`源逐元素转换为`float`后写入（写入大小为读取大小的2倍）；`half`源使能转换时逐元素转换为`float`后写入（写入大小为读取大小的2倍），未使能转换时每个`half`扩展为32bit后写入（写入大小为读取大小的2倍，但每元素后跟占位数据）；`float`与`int32_t`源直拷，写入大小等于读取大小。
- n_burst取值范围为[1, 4095]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- conv_control传入非0/1的值仅最低位生效（如2→0即转换关闭、3→1即转换开启）。截断后实际生效的转换使能状态与预期不符，可能导致BT中数据格式与预期不一致。

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
