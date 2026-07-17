# asc_dcci

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

- 接口声明文件：`"c_api/cache_ctrl/cache_ctrl.h"`
- 核心功能：在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。DMA搬运单元读写Global Memory，数据通过[asc_copy_ub2gm](../vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md)和[asc_copy_gm2ub](../vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md)等接口在UB等Local Memory和Global Memory间交互，没有Cache一致性问题；Scalar单元访问Global Memory，首先会访问每个核内的Data Cache，因此存在Data Cache与Global Memory的Cache一致性问题。该接口用来刷新Cache，保证Cache的一致性。
- 实现原理/公式：该接口的功能可以拆解为两部分：
  - Clean：将缓存中被修改过的数据（脏数据，Dirty Data）写回到Global Memory中，避免数据丢失。
  - Invalid：将指定地址范围的Cache Line标记为"无效"，使其从缓存中移除。保证下一次访问这些内存地址时，会从Global Memory重新加载数据，而不是使用可能过期的缓存数据。

  典型使用场景：
  - 读取Global Memory的数据，但该数据可能在外部被其余核修改，此时需要使用asc_dcci接口，直接访问Global Memory，获取最新数据。
  - 用户通过Scalar单元写Global Memory的数据，希望立刻写出，也需要使用asc_dcci接口。
  - Scalar单元访问UB数据时，该接口需配合[asc_set_ctrl()](../sys_var/asc_set_ctrl.md)接口使用，将CTRL[49]设置为1'b1，开启datacache模式。

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：不支持asc_dcci_entire_ub接口。
<!-- end id4 -->

## 函数原型

<!-- npu="950" id5 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 刷新传入地址所在的单个Cache Line
__aicore__ inline void asc_dcci_single(__gm__ void* dst)

// 刷新传入的UB地址空间地址所在的单个Cache Line
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)

// 刷新所有Cache Line
__aicore__ inline void asc_dcci_entire_all()
__aicore__ inline void asc_dcci_entire_out()
__aicore__ inline void asc_dcci_entire_atomic()
```

<!-- end id5 -->
<!-- npu="A3" id6 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 刷新传入地址所在的单个Cache Line
__aicore__ inline void asc_dcci_single(__gm__ void* dst)

// 刷新传入的UB地址空间地址所在的单个Cache Line
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)

// 刷新所有Cache Line
__aicore__ inline void asc_dcci_entire_all()
__aicore__ inline void asc_dcci_entire_ub()
__aicore__ inline void asc_dcci_entire_out()
__aicore__ inline void asc_dcci_entire_atomic()
```

<!-- end id6 -->
<!-- npu="910b" id7 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 刷新传入地址所在的单个Cache Line
__aicore__ inline void asc_dcci_single(__gm__ void* dst)

// 刷新传入的UB地址空间地址所在的单个Cache Line
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)

// 刷新所有Cache Line
__aicore__ inline void asc_dcci_entire_all()
__aicore__ inline void asc_dcci_entire_ub()
__aicore__ inline void asc_dcci_entire_out()
__aicore__ inline void asc_dcci_entire_atomic()
```

<!-- end id7 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id8 -->
针对Ascend 950PR/Ascend 950DT：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输入 | 需要刷新缓存的向量的起始地址。 |

<!-- end id8 -->
<!-- npu="A3" id9 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输入 | 需要刷新缓存的向量的起始地址。 |

<!-- end id9 -->
<!-- npu="910b" id10 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输入 | 需要刷新缓存的向量的起始地址。 |

<!-- end id10 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- SINGLE_CACHE_LINE模式下，如果传入地址非64B对齐，只会操作传入地址到64B对齐的部分。
- ENTIRE_DATA_CACHE模式下会刷新整个DCache，耗时较大，性能敏感的场景慎用。
- 多核操作同一条Cache Line时可能出现随机覆盖，与通用CPU的行为不同。

## 调用示例

<!-- npu="950" id11 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 示例：SINGLE_CACHE_LINE模式（x_gm 64B对齐场景）
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
for (int i = 0; i < 16; i++) {
    x_gm[i] = static_cast<float>(i + block_idx);
}
// 由于首地址64B对齐，调用asc_dcci_single指令后，数据会立刻刷新到Global Memory
asc_dcci_single(reinterpret_cast<__gm__ uint64_t*>(x_gm));
```

- 完整样例链接：`<TODO>`

<!-- end id11 -->
<!-- npu="A3" id12 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 示例：SINGLE_CACHE_LINE模式（x_gm 64B对齐场景）
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
for (int i = 0; i < 16; i++) {
    x_gm[i] = static_cast<float>(i + block_idx);
}
// 由于首地址64B对齐，调用asc_dcci_single指令后，数据会立刻刷新到Global Memory
asc_dcci_single(reinterpret_cast<__gm__ uint64_t*>(x_gm));
```

- 完整样例链接：`<TODO>`

<!-- end id12 -->
<!-- npu="910b" id13 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 示例：SINGLE_CACHE_LINE模式（x_gm 64B对齐场景）
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
for (int i = 0; i < 16; i++) {
    x_gm[i] = static_cast<float>(i + block_idx);
}
// 由于首地址64B对齐，调用asc_dcci_single指令后，数据会立刻刷新到Global Memory
asc_dcci_single(reinterpret_cast<__gm__ uint64_t*>(x_gm));
```

- 完整样例链接：`<TODO>`

<!-- end id13 -->
