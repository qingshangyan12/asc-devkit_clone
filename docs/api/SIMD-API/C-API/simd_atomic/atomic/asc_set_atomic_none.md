# asc_set_atomic_none

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->

## 功能说明

- 接口声明文件：`"c_api/atomic/atomic.h"`
- 核心功能：清空原子操作的状态，关闭数据搬运随路原子操作功能。后续执行数据搬运时，Global Memory中原始数据将被新搬运数据完全覆盖。一般和[asc_set_atomic_add](./asc_set_atomic_add.md)、[asc_set_atomic_max](./asc_set_atomic_max.md)、[asc_set_atomic_min](./asc_set_atomic_min.md)接口配合使用，用于在完成原子操作后关闭原子操作，避免影响后续功能。
- 实现原理/公式：将原子操作启用位设置为无原子操作状态，后续搬运指令将不再执行原子操作。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_set_atomic_none()
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_set_atomic_none()
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_set_atomic_none()
```

<!-- end id6 -->

## 参数说明

无

## 数据类型支持

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

本接口不涉及特定数据类型，用于清空原子操作状态。

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

本接口不涉及特定数据类型，用于清空原子操作状态。

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

本接口不涉及特定数据类型，用于清空原子操作状态。

<!-- end id9 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

无

## 调用示例

<!-- npu="950" id10 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
//total_length指参与搬运的数据总长度。dst是外部输入的int16_t类型的GM内存。
constexpr uint32_t total_length = 256;
__ubuf__ int16_t src0[total_length];
__ubuf__ int16_t src1[total_length];
asc_set_atomic_add_int16();
asc_copy_ub2gm_sync(dst, src0, total_length * sizeof(int16_t));
asc_copy_ub2gm_sync(dst, src1, total_length * sizeof(int16_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int16_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int16_t类型
输出数据dst：[3, 3, 3, ..., 3]   // int16_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id10 -->
<!-- npu="A3" id11 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
//total_length指参与搬运的数据总长度。dst是外部输入的int16_t类型的GM内存。
constexpr uint32_t total_length = 256;
__ubuf__ int16_t src0[total_length];
__ubuf__ int16_t src1[total_length];
asc_set_atomic_add_int16();
asc_copy_ub2gm_sync(dst, src0, total_length * sizeof(int16_t));
asc_copy_ub2gm_sync(dst, src1, total_length * sizeof(int16_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int16_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int16_t类型
输出数据dst：[3, 3, 3, ..., 3]   // int16_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id11 -->
<!-- npu="910b" id12 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
//total_length指参与搬运的数据总长度。dst是外部输入的int16_t类型的GM内存。
constexpr uint32_t total_length = 256;
__ubuf__ int16_t src0[total_length];
__ubuf__ int16_t src1[total_length];
asc_set_atomic_add_int16();
asc_copy_ub2gm_sync(dst, src0, total_length * sizeof(int16_t));
asc_copy_ub2gm_sync(dst, src1, total_length * sizeof(int16_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int16_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int16_t类型
输出数据dst：[3, 3, 3, ..., 3]   // int16_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id12 -->
