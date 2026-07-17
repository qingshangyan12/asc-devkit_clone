# asc_atomic_inc

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
- 核心功能：对Global Memory中address指向的计数器执行原子递增操作。读取address指向的GM地址上的旧值（old_value），如果旧值大于等于指定数值val，则将0写入GM地址；否则将旧值加1后写回GM地址。返回该地址修改前的值（old_value）。
- 实现原理/公式：

$$new\_value = (old\_value >= val\ ?\ 0\ :\ old\_value + 1)$$

## 函数原型

<!-- npu="950" id8 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline uint32_t asc_atomic_inc(__gm__ uint32_t *address, uint32_t val)

__aicore__ inline uint64_t asc_atomic_inc(__gm__ uint64_t *address, uint64_t val)
```

<!-- end id8 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id9 -->
针对Ascend 950PR/Ascend 950DT：

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Global Memory的地址。 |
| val | 输入 | 源操作数，作为递增计数器的回绕阈值，支持数据类型和address指向的数据类型一致。 |

<!-- end id9 -->

## 数据类型支持

<!-- npu="950" id10 -->
针对Ascend 950PR/Ascend 950DT：

address、val、返回值的数据类型相同，支持的数据类型为uint32_t、uint64_t。

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

- 在开启编译器自动同步功能的前提下，编译器能够自动在PIPE_MTE2/PIPE_MTE3与PIPE_S之间插入同步。但是，asc_atomic_inc为标量计算，在读写GM时如果与搬运单元（MTE2/MTE3）存在数据依赖，编译器却无法自动插入同步，开发者需要根据实际情况手动插入同步。
- Scalar原子操作会绕过DCache，需要调用[asc_dcci](../cache_ctrl/asc_dcci.md)接口确保GM与DCache的一致性。

## 调用示例

<!-- npu="950" id11 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// dst是外部输入的uint32_t类型的GM内存地址，初始值为0。
__gm__ uint32_t *dst = ...;

asc_dcci_entire_all();                            // 手动同步，确保GM与DCache一致性

// 递增场景：dst(0)小于val(9)，所以dst加1变为1。
uint32_t old = asc_atomic_inc(dst, 9);            // dst = 1, old = 0

// 原子操作后，若需通过搬运单元读取GM数据，需手动插入同步。
asc_sync_pipe(PIPE_S);
```

结果示例：

```
输入数据dst：0   // uint32_t类型
输入数据val：9   // uint32_t类型
输出数据dst：1   // uint32_t类型（递增）
返回值old：0     // uint32_t类型
```

```cpp
// 回绕场景：dst(9)大于等于val(9)，所以dst被赋值为0。
asc_dcci_entire_all();                            // 手动同步
uint32_t old2 = asc_atomic_inc(dst, 9);           // dst = 0, old2 = 9
asc_sync_pipe(PIPE_S);
```

结果示例：

```
输入数据dst：9   // uint32_t类型
输入数据val：9   // uint32_t类型
输出数据dst：0   // uint32_t类型（回绕为0）
返回值old2：9    // uint32_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id11 -->
