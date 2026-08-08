# asc_atomic_exch

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

对Global Memory中`address`指向的单个元素执行原子交换操作。读取该元素的旧值`old_value`，将`val`写回该地址以替换旧值，并返回`old_value`。整个读取和写回过程为原子操作。

## 函数原型

```c
__aicore__ inline <dtype> asc_atomic_exch(__gm__ <dtype>* address,
                                          <dtype> val)
```

dtype支持的数据类型为`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。

```c
// 示例：int32_t类型原子交换。
__aicore__ inline int32_t asc_atomic_exch(__gm__ int32_t* address,
                                          int32_t val)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Global Memory的地址。 |
| val | 输入 | 源操作数。 |

## 返回值说明

address地址中计算前的原始数据。

## 流水类型

PIPE_S

## 约束说明

- 在开启编译器自动同步功能的前提下，编译器能够自动在PIPE_MTE2/PIPE_MTE3与PIPE_S之间插入同步。但是，asc_atomic_exch为标量计算，在读写GM时如果与搬运单元（MTE2/MTE3）存在数据依赖，编译器却无法自动插入同步，开发者需要根据实际情况手动插入同步。
- Scalar原子操作会绕过DCache，需要调用[asc_dcci](../cache_ctrl/asc_dcci.md)接口确保GM与DCache的一致性。

## 调用示例

```cpp
// dst为外部输入的int32_t类型的GM内存，此示例的初始值为10
asc_dcci_entire_all();            // 手动同步
int32_t old = asc_atomic_exch(dst, 20);  // 计算后dst = 20, old = 10
```
