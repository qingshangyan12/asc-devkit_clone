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

- 接口声明文件：`c_api/sync/sync.h`
- 核心功能：针对PIPE_MTE3流水线执行同步操作。
- 实现原理/公式：对应SetFlag/WaitFlag中MTE3相关的HardEvent同步组合，是SetFlag+WaitFlag的简化封装。通过设置和等待硬件事件标志位，实现PIPE_MTE3流水线与其他流水线之间的同步。

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

<!-- npu="950" id7 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| id | 输入 | 同步ID。取值范围为[0, 7]。 |

<!-- end id7 -->
<!-- npu="A3" id8 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| id | 输入 | 同步ID。取值范围为[0, 7]。 |

<!-- end id8 -->
<!-- npu="910b" id9 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| id | 输入 | 同步ID。取值范围为[0, 7]。 |

<!-- end id9 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：占用硬件事件标志位资源，每个id对应一个事件标志位，取值范围为[0, 7]。
- 资源占用与参数/数据量关系：资源占用与数据量无关，与id参数取值有关。

## 约束说明

- id的取值范围为0-7，建议使用事件ID 0-5，事件ID 6用于系统内部规划（当前未使用），事件ID 7用于TPipe编程中的自动同步功能，暂不建议直接使用事件ID 6-7。

## 调用示例

<!-- npu="950" id10 -->
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

<!-- end id10 -->
<!-- npu="A3" id11 -->
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

<!-- end id11 -->
<!-- npu="910b" id12 -->
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

<!-- end id12 -->
