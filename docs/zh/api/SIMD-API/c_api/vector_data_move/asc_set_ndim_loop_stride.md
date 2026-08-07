# asc_set_ndim_loop_stride

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

头文件路径为：`"c_api/vector_datamove/vector_datamove.h"`。

设置NDDMA高维切分搬运模式下第0～4维各维的源端步长与目的端步长。本接口包含5个独立setter，分别对应loop0～loop4五个嵌套循环维度，每次调用配置其中一维的源端和目的端步长。配置写入后作用于后续调用的高维切分搬运接口，决定对应维度每次迭代在源端与目的端的地址偏移量，单位为元素个数。须先调用本接口写入步长配置，再调用对应高维切分搬运接口，搬运接口在执行时按本接口设置的步长计算每次迭代的源端和目的端地址。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

在高维切分搬运模式下，本接口负责配置各维步长，需与`asc_set_ndim_pad_count`（配置各维padding计数值）、`asc_set_ndim_pad_value`（配置padding填充值）配合使用，共同完成搬运参数的完整配置。

## 函数原型

### loop0～loop4步长setter原型

5个setter参数列表相同，仅函数名按loopN递增，每个参数单独一行：

```c
__aicore__ inline void asc_set_ndim_loop0_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
```

```c
__aicore__ inline void asc_set_ndim_loop1_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
```

```c
__aicore__ inline void asc_set_ndim_loop2_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
```

```c
__aicore__ inline void asc_set_ndim_loop3_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
```

```c
__aicore__ inline void asc_set_ndim_loop4_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst_stride` | 输入 | 对应维度每次迭代在目的端（Unified Buffer）的步长，单位为元素个数，即相邻两次迭代目的端起始地址之间间隔的元素数，默认值为0。取值范围为[0，2^20−1]。参数类型为`uint64_t`（0~2^64−1），传入>2^20−1的值会按位宽截断处理（如2^20→0、2^20+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的目的端地址偏移量小于预期，后一次迭代写到错误位置，与前一迭代数据重叠或错位。 |
| `src_stride` | 输入 | 对应维度每次迭代在源端（Global Memory）的步长，单位为元素个数，即相邻两次迭代源端起始地址之间间隔的元素数，默认值为0。取值范围为[0，2^40−1]。参数类型为`uint64_t`（0~2^64−1），传入>2^40−1的值会按位宽截断处理（如2^40→0、2^40+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的源端地址偏移量小于预期，后一次迭代读到错误位置，取到非预期数据。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口仅设置对应维度的源端和目的端步长配置，实际搬运由后续调用的高维切分搬运接口执行，须先调用本接口写入配置，再调用对应高维切分搬运接口。
- 本接口与配套setter接口协同配置高维切分搬运的完整参数集，调用高维切分搬运接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- 配置写入后对后续所有高维切分搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同步长，应在调用高维切分搬运接口前重新调用本接口设置。
- 各维目的步长间无大小关系约束（如loop0目的步长可大于loop4目的步长），但若所有维目的步长按维度顺序升序排列，则不同维度的地址空间不可交错或重叠，否则触发写覆盖异常。

### 步长字段截断约束

- `dst_stride`传入>2^20−1的值会按位宽截断处理（如2^20→0、2^20+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的目的端地址偏移量小于预期，后一次迭代写到错误位置，与前一迭代数据重叠或错位。参数类型为`uint64_t`（0~2^64−1）。
- `src_stride`传入>2^40−1的值会按位宽截断处理（如2^40→0、2^40+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的源端地址偏移量小于预期，后一次迭代读到错误位置，取到非预期数据。参数类型为`uint64_t`（0~2^64−1）。

## 调用示例

```cpp
uint64_t dst_stride = 8;
uint64_t src_stride = 8;
asc_set_ndim_loop0_stride(dst_stride, src_stride);
```
