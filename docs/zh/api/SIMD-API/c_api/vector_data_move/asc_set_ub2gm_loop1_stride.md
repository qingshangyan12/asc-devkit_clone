# asc_set_ub2gm_loop1_stride

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

本接口用于设置[asc_copy_ub2gm_align](./asc_copy_ub2gm_align/asc_copy_ub2gm_align.md)使用循环搬运模式时，内层循环`loop1`的源操作数步长和目的操作数步长。

源操作数步长和目的操作数步长分别表示`loop1`相邻两次迭代的源操作数数据块起始地址偏移量和目的操作数数据块起始地址偏移量，单位为字节。

使用循环搬运模式时，还需通过[asc_set_ub2gm_loop_size](./asc_set_ub2gm_loop_size.md)设置循环次数，并通过[asc_set_ub2gm_loop2_stride](./asc_set_ub2gm_loop2_stride.md)设置外层循环`loop2`的步长。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

以源操作数搬运场景为例，如下图所示。

![源操作数搬运场景示例](../figures/source_operand_move_example.png)

## 函数原型

```c
__aicore__ inline void asc_set_ub2gm_loop1_stride(uint64_t loop1_src_stride,
                                                  uint64_t loop1_dst_stride)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `loop1_src_stride` | 输入 | `loop1`相邻两次迭代的源操作数数据块起始地址偏移量，单位为字节。取值范围为[0，2^21−1]，且必须32字节对齐。 |
| `loop1_dst_stride` | 输入 | `loop1`相邻两次迭代的目的操作数数据块起始地址偏移量，单位为字节。取值范围为[0，2^40−1]。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 调用`asc_copy_ub2gm_align`前，必须通过本接口、`asc_set_ub2gm_loop_size`和`asc_set_ub2gm_loop2_stride`完成循环次数及两层循环步长的配置。
- `loop1`步长配置对后续的`asc_copy_ub2gm_align`调用生效，直至再次调用本接口覆盖配置。不同搬运任务需要不同`loop1`步长时，应在调用`asc_copy_ub2gm_align`前重新配置。

### 步长配置约束

- `loop1_src_stride`必须32字节对齐，即使`loop1_size`为1也必须满足该要求。
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
