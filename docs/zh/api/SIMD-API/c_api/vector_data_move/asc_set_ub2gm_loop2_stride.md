# asc_set_ub2gm_loop2_stride

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

设置从Unified Buffer搬运到Global Memory的循环搬运接口中loop2维度的源端步长与目的端步长，写入配置后供后续调用的UB到GM循环搬运接口在循环模式下计算loop2维度各次迭代之间的源端地址偏移与目的端地址偏移使用。本接口将两个步长参数按位拼装为一个64位配置值后写入硬件配置寄存器，源端步长与目的端步长分别占据配置值的相应位段。

loop模式下，循环搬运次数需通过`asc_set_ub2gm_loop_size`预先配置，loop1维度的源端与目的端步长需通过`asc_set_ub2gm_loop1_stride`预先配置，本接口配置loop2维度的源端与目的端步长。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

以源操作数搬运场景为例，如下图所示。

![源操作数搬运场景示例](../figures/source_operand_move_example.png)

## 函数原型

```c
__aicore__ inline void asc_set_ub2gm_loop2_stride(uint64_t loop2_src_stride,
                                                  uint64_t loop2_dst_stride)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `loop2_src_stride` | 输入 | loop2维度相邻两次迭代在源端（Unified Buffer）的步长，单位字节，表示下一次迭代读取起始地址相对于当前迭代读取起始地址的偏移量。取值范围为[0，2^21−1]，需32字节对齐。参数类型为`uint64_t`（0~2^64−1），传入>2^21−1的值会按位宽截断处理（如2^21→0、2^21+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的源端地址偏移量小于预期，后一次迭代读到错误位置，取到非预期数据。 |
| `loop2_dst_stride` | 输入 | loop2维度相邻两次迭代在目的端（Global Memory）的步长，单位字节，表示下一次迭代写入起始地址相对于当前迭代写入起始地址的偏移量。取值范围为[0，2^40−1]。参数类型为`uint64_t`（0~2^64−1），传入>2^40−1的值会按位宽截断处理（如2^40→0、2^40+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的目的端地址偏移量小于预期，后一次迭代写到错误位置，与前一次迭代数据重叠或错位。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口仅写入loop2步长配置，实际循环搬运由后续调用的UB到GM循环搬运接口在执行循环模式时取用配置寄存器完成，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续所有UB到GM循环搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同loop2步长，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置循环次数的setter接口，再调用配置loop1步长的setter接口，再调用本接口配置loop2步长，最后调用UB到GM循环搬运接口执行搬运（配套setter名见功能说明章节）。

### 步长配置约束

- `loop2_src_stride`需32字节对齐（源端Unified Buffer端口对齐要求），否则触发异常；该对齐要求即使循环次数为1仍需满足。
- `loop2_src_stride`传入>2^21−1的值会按位宽截断处理（如2^21→0、2^21+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的源端地址偏移量小于预期，后一次迭代读到错误位置，取到非预期数据。参数类型为`uint64_t`（0~2^64−1）。
- `loop2_dst_stride`传入>2^40−1的值会按位宽截断处理（如2^40→0、2^40+1→1），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的目的端地址偏移量小于预期，后一次迭代写到错误位置，与前一次迭代数据重叠或错位。参数类型为`uint64_t`（0~2^64−1）。
- 后续UB到GM循环搬运接口在循环模式下执行时，目的数据（含loop1各次迭代之间、loop2各次迭代之间，以及不同本接口配置的多次搬运之间）不可存在地址重叠，硬件不报错但不保证重叠数据的正确性；loop1与loop2的目的步长允许交叉排列（如loop2目的步长可小于burst目的步长）。
- 完成当前循环搬运任务后，需调用`asc_set_ub2gm_loop_size(1, 1)`将两层循环次数复位为1，否则可能影响下一次搬运。

## 调用示例

```cpp
uint32_t loop1_size = 2;
uint32_t loop2_size = 2;
uint64_t loop1_src_stride = 96;
uint64_t loop1_dst_stride = 128;
uint64_t loop2_src_stride = 192;
uint64_t loop2_dst_stride = 288;
asc_set_ub2gm_loop_size(loop1_size, loop2_size);
asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride);
asc_set_ub2gm_loop2_stride(loop2_src_stride, loop2_dst_stride);
asc_copy_ub2gm_align(dst, src, 2, 48 * sizeof(int8_t), asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, 48, 48);
asc_set_ub2gm_loop_size(1, 1);
```
