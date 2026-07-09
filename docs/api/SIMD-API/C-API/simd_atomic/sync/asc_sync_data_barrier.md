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

- 接口声明文件：`c_api/sync/sync.h`
- 核心功能：用于阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可以通过参数控制）执行结束，用于解决Scalar的内存依赖。
- 实现原理/公式：对应DataSyncBarrier的用法，通过硬件内存屏障机制，确保指定内存位置的所有前序访问指令完成后，才放行后续指令。

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

<!-- npu="950" id7 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| arg | 输入 | 表示需要等待的内存位置，类型为mem_dsb_t，可取值为：<br>&bull; DSB_ALL：等待所有内存访问指令。<br>&bull; DSB_DDR：等待GM访问指令。<br>&bull; DSB_UB：等待UB访问指令。<br>&bull; DSB_SEQ：预留参数，暂未启动，为后续的功能扩展做保留。 |

<!-- end id7 -->
<!-- npu="A3" id8 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| arg | 输入 | 表示需要等待的内存位置，类型为mem_dsb_t，可取值为：<br>&bull; DSB_ALL：等待所有内存访问指令。<br>&bull; DSB_DDR：等待GM访问指令。<br>&bull; DSB_UB：等待UB访问指令。<br>&bull; DSB_SEQ：预留参数，暂未启动，为后续的功能扩展做保留。 |

<!-- end id8 -->
<!-- npu="910b" id9 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| arg | 输入 | 表示需要等待的内存位置，类型为mem_dsb_t，可取值为：<br>&bull; DSB_ALL：等待所有内存访问指令。<br>&bull; DSB_DDR：等待GM访问指令。<br>&bull; DSB_UB：等待UB访问指令。<br>&bull; DSB_SEQ：预留参数，暂未启动，为后续的功能扩展做保留。 |

<!-- end id9 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用额外的同步资源（如flag_id、event_id等）。
- 资源占用与参数/数据量关系：资源占用与数据量无关，与arg参数取值有关。

## 约束说明

- mem_dsb_t类型的入参必须直接传递枚举值字面量。
- 等待的内存位置取值为DSB_ALL时，只能等待GM和UB的访问指令，不包括其他内存位置，例如L0C Buffer、L1 Buffer。

## 调用示例

<!-- npu="950" id10 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/05_sync_control/data_sync_barrier`

<!-- end id10 -->
<!-- npu="A3" id11 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/05_sync_control/data_sync_barrier`

<!-- end id11 -->
<!-- npu="910b" id12 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/05_sync_control/data_sync_barrier`

<!-- end id12 -->
