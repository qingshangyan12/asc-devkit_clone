# asc_sync_mte3

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

- 接口声明文件：`"c_api/sync/sync.h"`
- 核心功能：针对PIPE_MTE3流水线执行同步操作，用于在PIPE_MTE3流水线内部约束执行顺序。
- 实现原理：保证PIPE_MTE3流水线中前序指令的所有数据读写工作全部完成，后续PIPE_MTE3指令才能执行。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_sync_mte3(int id)
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_sync_mte3(int id)
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_sync_mte3(int id)
```

<!-- end id6 -->

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| id | 输入 | 同步ID。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 同一PIPE_MTE3流水线内，虽然指令是顺序执行，但并不意味着后一条指令开始执行时前一条指令执行结束。asc_sync_mte3可以保证前序指令中所有数据读写全部完成，后序指令才开始执行。

## 调用示例

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 本例中total_length指参与计算的数据总长度。src0_gm，src1_gm，dst_gm是外部输入的float类型的源操作数、目的操作数，指向GM内存空间。
constexpr uint32_t total_length = 128;
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
__ubuf__ float dst[total_length];

asc_copy_gm2ub(src0, src0_gm, total_length * sizeof(float));
asc_copy_gm2ub(src1, src1_gm, total_length * sizeof(float));

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync();

asc_add(dst, src1, src0, total_length);

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync();

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));

// 同步操作：数据搬运操作（UB到GM，PIPE_MTE3流水）完成后才能启动后续操作。
asc_sync_mte3(0);
```

- 完整样例链接：`<TODO>`

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 本例中total_length指参与计算的数据总长度。src0_gm，src1_gm，dst_gm是外部输入的float类型的源操作数、目的操作数，指向GM内存空间。
constexpr uint32_t total_length = 128;
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
__ubuf__ float dst[total_length];

asc_copy_gm2ub(src0, src0_gm, total_length * sizeof(float));
asc_copy_gm2ub(src1, src1_gm, total_length * sizeof(float));

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync();

asc_add(dst, src1, src0, total_length);

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync();

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));

// 同步操作：数据搬运操作（UB到GM，PIPE_MTE3流水）完成后才能启动后续操作。
asc_sync_mte3(0);
```

- 完整样例链接：`<TODO>`

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 本例中total_length指参与计算的数据总长度。src0_gm，src1_gm，dst_gm是外部输入的float类型的源操作数、目的操作数，指向GM内存空间。
constexpr uint32_t total_length = 128;
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
__ubuf__ float dst[total_length];

asc_copy_gm2ub(src0, src0_gm, total_length * sizeof(float));
asc_copy_gm2ub(src1, src1_gm, total_length * sizeof(float));

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync();

asc_add(dst, src1, src0, total_length);

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync();

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));

// 同步操作：数据搬运操作（UB到GM，PIPE_MTE3流水）完成后才能启动后续操作。
asc_sync_mte3(0);
```

- 完整样例链接：`<TODO>`

<!-- end id9 -->
