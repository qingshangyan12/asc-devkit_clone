# asc_sync_data_barrier

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
- 核心功能：用于阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可以通过参数控制）执行结束。
- 实现原理：通过内存同步屏障解决Scalar的内存依赖，确保指定内存位置的所有访问指令完成后才执行后续指令。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_sync_data_barrier(mem_dsb_t arg)
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_sync_data_barrier(mem_dsb_t arg)
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_sync_data_barrier(mem_dsb_t arg)
```

<!-- end id6 -->

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| arg | 输入 | 表示需要等待的内存位置，类型为mem_dsb_t，可取值为：<br>&bull; DSB_ALL：等待所有内存访问指令。<br>&bull; DSB_DDR：等待GM访问指令。<br>&bull; DSB_UB：等待UB访问指令。<br>&bull; DSB_SEQ：预留参数，暂未启动，为后续的功能扩展做保留。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- mem_dsb_t类型的入参必须直接传递枚举值字面量。
- 等待的内存位置取值为DSB_ALL时，只能等待GM和UB的访问指令，不包括其他内存位置，例如L0C Buffer、L1 Buffer。

## 调用示例

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

- 完整样例链接：`<TODO>`

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

- 完整样例链接：`<TODO>`

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

- 完整样例链接：`<TODO>`

<!-- end id9 -->
