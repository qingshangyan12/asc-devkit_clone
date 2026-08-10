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

本接口用于设置[asc_ndim_copy_gm2ub](./asc_ndim_copy_gm2ub.md)进行NDDMA多维数据搬运时，`loop0`至`loop4`各循环维度的源端步长和目的端步长。

本接口包含5个函数，分别用于配置`loop0`至`loop4`的步长，其中`loop0`为最内层循环，`loop4`为最外层循环。

源端步长和目的端步长分别表示同一循环维度内，源操作数和目的操作数中当前元素与下一元素之间的间隔，单位为元素个数。

`loop1`至`loop4`各维的填充元素数通过[asc_set_ndim_pad_count](./asc_set_ndim_pad_count.md)配置；使用常数填充模式时，还需通过[asc_set_ndim_pad_value](./asc_set_ndim_pad_value.md)设置填充值。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

```cpp
__aicore__ inline void asc_set_ndim_loop0_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
__aicore__ inline void asc_set_ndim_loop1_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
__aicore__ inline void asc_set_ndim_loop2_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
__aicore__ inline void asc_set_ndim_loop3_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
__aicore__ inline void asc_set_ndim_loop4_stride(uint64_t dst_stride,
                                                  uint64_t src_stride)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst_stride` | 输入 | 同一循环维度内，目的操作数中当前元素与下一元素之间的间隔，单位为元素个数。取值范围为[0，2^20−1]。 |
| `src_stride` | 输入 | 同一循环维度内，源操作数中当前元素与下一元素之间的间隔，单位为元素个数。取值范围为[0，2^40−1]。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 调用`asc_ndim_copy_gm2ub`前，必须先调用本接口完成`loop0`至`loop4`各循环维度的源端步长和目的端步长配置。
- 各循环维度的步长配置在对应的步长设置函数再次调用前持续生效。不同搬运任务需要不同步长时，应在调用`asc_ndim_copy_gm2ub`前重新配置。

## 调用示例

```cpp
uint64_t dst_stride = 8;
uint64_t src_stride = 8;
asc_set_ndim_loop0_stride(dst_stride, src_stride);
```
