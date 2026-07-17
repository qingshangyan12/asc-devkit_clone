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

- 接口声明文件：`"c_api/scalar_compute/scalar_compute.h"`
- 核心功能：对Global Memory中address指向的元素执行原子交换操作。读取address指向的GM地址上的旧值（old_value），并将输入的标量值val替换旧值存储回同一地址，返回该地址修改前的值（old_value）。
- 实现原理/公式：

$$new\_value = val$$

## 函数原型

<!-- npu="950" id8 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline float asc_atomic_exch(__gm__ float *address, float val)

__aicore__ inline int32_t asc_atomic_exch(__gm__ int32_t *address, int32_t val)

__aicore__ inline uint32_t asc_atomic_exch(__gm__ uint32_t *address, uint32_t val)

__aicore__ inline int64_t asc_atomic_exch(__gm__ int64_t *address, int64_t val)

__aicore__ inline uint64_t asc_atomic_exch(__gm__ uint64_t *address, uint64_t val)
```

<!-- end id8 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id9 -->
针对Ascend 950PR/Ascend 950DT：

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Global Memory的地址。 |
| val | 输入 | 源操作数，用于交换写入的值，支持数据类型和address指向的数据类型一致。 |

<!-- end id9 -->

## 数据类型支持

<!-- npu="950" id10 -->
针对Ascend 950PR/Ascend 950DT：

address、val、返回值的数据类型相同，支持的数据类型为float、int32_t、uint32_t、int64_t、uint64_t。

<!-- end id10 -->

## 返回值说明

address地址中计算前的原始数据。

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 在开启编译器自动同步功能的前提下，编译器能够自动在PIPE_MTE2/PIPE_MTE3与PIPE_S之间插入同步。但是，asc_atomic_exch为标量计算，在读写GM时如果与搬运单元（MTE2/MTE3）存在数据依赖，编译器却无法自动插入同步，开发者需要根据实际情况手动插入同步。
- Scalar原子操作会绕过DCache，需要调用[asc_dcci](../cache_ctrl/asc_dcci.md)接口确保GM与DCache的一致性。

## 调用示例

<!-- npu="950" id11 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 多核场景下，各核分别对GM首个元素执行原子交换。
// dst是外部输入的int32_t类型的GM内存地址，初始值为10。
__gm__ int32_t *dst = ...;

asc_dcci_entire_all();                            // 手动同步，确保GM与DCache一致性

// 将dst的值从10替换为20，返回修改前的旧值10。
int32_t old = asc_atomic_exch(dst, 20);           // dst = 20, old = 10

// 原子操作后，若需通过搬运单元读取GM数据，需手动插入同步。
asc_sync_pipe(PIPE_S);
```

结果示例：

```
输入数据dst：10   // int32_t类型
输入数据val：20   // int32_t类型
输出数据dst：20   // int32_t类型（被替换为val）
返回值old：10     // int32_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id11 -->
