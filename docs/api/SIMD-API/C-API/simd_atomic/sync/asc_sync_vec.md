# asc_sync_vec

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
- 核心功能：针对所有流水线执行同步操作。用于阻塞后续指令的执行，直到所有前序流水线操作全部完成。
- 实现原理/公式：对应PipeBarrier&lt;PIPE_ALL&gt;()的用法，对硬件中所有流水线执行屏障同步，保证前序指令中所有数据的读写工作全部完成后，后序指令才能执行。与asc_sync功能相近，为同一基础功能的不同C-API封装。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_sync_vec()
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_sync_vec()
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_sync_vec()
```

<!-- end id6 -->

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用额外的同步资源（如flag_id、event_id等）。
- 资源占用与参数/数据量关系：该接口无参数，资源占用与数据量无关。

## 约束说明

- asc_sync_vec()会等待所有流水线中所有先前提交的指令完成，这会对性能产生影响。若仅需阻塞单条流水线即可解决问题，应避免随意调用asc_sync_vec()，可使用asc_sync_pipe指定具体流水线进行同步。

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
asc_sync_vec();

asc_add(dst, src1, src0, total_length);

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync_vec();

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));
```

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
asc_sync_vec();

asc_add(dst, src1, src0, total_length);

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync_vec();

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));
```

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
asc_sync_vec();

asc_add(dst, src1, src0, total_length);

// 同步操作：前置操作完成后才能启动后续操作。
asc_sync_vec();

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));
```

<!-- end id9 -->
