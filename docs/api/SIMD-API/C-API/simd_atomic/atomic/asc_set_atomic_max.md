# asc_set_atomic_max

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
- 核心功能：设置对后续的从Unified Buffer/L0C Buffer/L1 Buffer到Global Memory的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、int32_t、bfloat16_t、half、float。
- 实现原理/公式：开启原子比较取大后，后续执行搬运操作从Unified Buffer/L0C Buffer/L1 Buffer到Global Memory时，将待拷贝的内容和Global Memory已有内容进行比较，将最大值写入Global Memory。
<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：不支持L1 Buffer到Global Memory的通路。
<!-- end id4 -->

## 函数原型

<!-- npu="950" id5 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_set_atomic_max_int8()
__aicore__ inline void asc_set_atomic_max_int16()
__aicore__ inline void asc_set_atomic_max_int()
__aicore__ inline void asc_set_atomic_max_bfloat()
__aicore__ inline void asc_set_atomic_max_float16()
__aicore__ inline void asc_set_atomic_max_float()
```

<!-- end id5 -->
<!-- npu="A3" id6 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_set_atomic_max_int8()
__aicore__ inline void asc_set_atomic_max_int16()
__aicore__ inline void asc_set_atomic_max_int()
__aicore__ inline void asc_set_atomic_max_bfloat()
__aicore__ inline void asc_set_atomic_max_float16()
__aicore__ inline void asc_set_atomic_max_float()
```

<!-- end id6 -->
<!-- npu="910b" id7 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_set_atomic_max_int8()
__aicore__ inline void asc_set_atomic_max_int16()
__aicore__ inline void asc_set_atomic_max_int()
__aicore__ inline void asc_set_atomic_max_bfloat()
__aicore__ inline void asc_set_atomic_max_float16()
__aicore__ inline void asc_set_atomic_max_float()
```

<!-- end id7 -->

## 参数说明

无

## 数据类型支持

<!-- npu="950" id8 -->
针对Ascend 950PR/Ascend 950DT：

支持对int8_t、int16_t、int32_t、bfloat16_t、half、float类型的数据进行原子比较取大操作。

<!-- end id8 -->
<!-- npu="A3" id9 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

支持对int8_t、int16_t、int32_t、bfloat16_t、half、float类型的数据进行原子比较取大操作。

<!-- end id9 -->
<!-- npu="910b" id10 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

支持对int8_t、int16_t、int32_t、bfloat16_t、half、float类型的数据进行原子比较取大操作。

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

<!-- npu="950" id11 -->
- 针对Ascend 950PR/Ascend 950DT：支持的数据通路为UB/L0C Buffer->GM。
<!-- end id11 -->
<!-- npu="A3" id12 -->
- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持的数据通路为UB/L0C Buffer/L1 Buffer->GM。
<!-- end id12 -->
<!-- npu="910b" id13 -->
- 针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持的数据通路为UB/L0C Buffer/L1 Buffer->GM。
<!-- end id13 -->
- 使用结束后，建议通过[asc_set_atomic_none](./asc_set_atomic_none.md)关闭原子最大操作，以免影响后续相关指令功能。
- 该指令执行前不会对GM的数据做清零操作，开发者需在需要时手动添加清零操作。

## 调用示例

<!-- npu="950" id14 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// total_length指参与计算的数据长度，dst是外部输入的int8_t类型的GM内存地址。
constexpr uint32_t total_length = 256;
__ubuf__ int8_t src0[total_length];
__ubuf__ int8_t src1[total_length];

asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_max_int8();
asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int8_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int8_t类型
输出数据dst：[2, 2, 2, ..., 2]   // int8_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id14 -->
<!-- npu="A3" id15 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// total_length指参与计算的数据长度，dst是外部输入的int8_t类型的GM内存地址。
constexpr uint32_t total_length = 256;
__ubuf__ int8_t src0[total_length];
__ubuf__ int8_t src1[total_length];

asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_max_int8();
asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int8_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int8_t类型
输出数据dst：[2, 2, 2, ..., 2]   // int8_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id15 -->
<!-- npu="910b" id16 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// total_length指参与计算的数据长度，dst是外部输入的int8_t类型的GM内存地址。
constexpr uint32_t total_length = 256;
__ubuf__ int8_t src0[total_length];
__ubuf__ int8_t src1[total_length];

asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_max_int8();
asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
asc_set_atomic_none();
```

结果示例：

```
输入数据src0：[1, 1, 1, ..., 1]  // int8_t类型
输入数据src1：[2, 2, 2, ..., 2]  // int8_t类型
输出数据dst：[2, 2, 2, ..., 2]   // int8_t类型
```

- 完整样例链接：`<TODO>`

<!-- end id16 -->
