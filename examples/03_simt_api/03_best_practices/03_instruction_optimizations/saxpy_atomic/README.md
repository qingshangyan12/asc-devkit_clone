# 原子操作融合更新样例

## 概述

在读-改-写（read-modify-write）场景下,如果各线程访问的地址互不重叠，不存在同地址竞争，可以用原子操作替代普通的"读旧值—计算—写回"三步操作。原子操作将读-改-写的三步融合在存储侧就地完成，核心侧无需显式读取旧值，从而节省一次核与内存之间的数据传输。

本样例以 `a[i] += b[i] * c[i]` 的向量更新为载体，对比普通读-改-写与 `asc_atomic_add` 的性能差异，展示融合更新的优化效果。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── saxpy_atomic
│   ├── CMakeLists.txt         // 样例构建脚本
│   ├── saxpy_atomic.asc       // Ascend C SIMT核函数实现 & Host调用样例
│   └── README.md
```

## 样例描述

本样例对三个 `float` 数组执行 `a[i] += b[i] * c[i]` 的逐元素更新操作。

- 计算公式：

  ```text
  a[i] = a[i] + b[i] * c[i],  i = 0, 1, ..., element_count - 1
  ```

  - `a`、`b`、`c` 均为 `float` 数组
  - 各线程访问的数组索引 `i` 互不重叠，不存在同地址竞争

- 样例规格：

  <table>
  <tr><td align="center">样例类型（OpType）</td><td colspan="4" align="center">Atomic Optimization</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">b</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">c</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输入输出</td><td align="center">a</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">vector_plain_update / vector_atomic_update</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过2个Case构成优化对比，各Case的核函数、实现方式如下表所示。

| Case | 核函数 | 实现方式 |
|---|---|---|
| 0 | `vector_plain_update` | 普通读-改-写 |
| 1 | `vector_atomic_update` | `asc_atomic_add` |

两个Case采用相同的线程块数量和线程数，访问模式与计算逻辑完全一致，仅更新 `a[i]` 的指令序列不同。

### 性能指标说明

| 指标 | 说明 |
| --- | --- |
| Task Duration（μs） | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |

---

### Case 0：普通读-改-写

**样例目标**：各线程按 grid-stride 模式遍历数组，对 `a[i]` 执行"读出旧值—计算新值—写回新值"的标准三步操作。

**场景配置**：

| Case | 实现方式 | 数据类型 | 规模（Block×Thread） | 数组元素数 |
|:---:|:---:|:---:|:---:|:---:|
| 0 | 普通读-改-写 | float | 64×2048 | 4194304 (2²²) |

**核心实现**：启动64个线程块，每个线程块2048个线程。各线程按固定步长遍历数组，每个元素执行如下操作：

1. 从 GM 读取 `b[i]`、`c[i]`、`a[i]` 三个值到核内寄存器
2. 在核内计算 `a[i] + b[i] * c[i]`
3. 将结果写回 GM 的 `a[i]`

对应的核函数实现如下。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_plain_update(
    float* a, const float* b, const float* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        a[index] += b[index] * c[index];
    }
}
```

**性能数据**：

| Case | 实现方式 | Task Duration（μs） |
|:---:|:---:|:---:|
| 0 | 普通读-改-写 | 51.96 |

Case 0 的耗时约 51.96μs。每个元素在核与内存之间产生 4 次事务：3 次读（`a`、`b`、`c`）+ 1 次写（`a`）。

---

### Case 1：asc_atomic_add 融合更新

**样例目标**：用 `asc_atomic_add` 替代普通读-改-写，将"读旧值—加—写回"的三步操作融合在存储侧完成，核心侧只需传递待累加的值。

**场景配置**：

| Case | 实现方式 | 数据类型 | 规模（Block×Thread） | 数组元素数 |
|:---:|:---:|:---:|:---:|:---:|
| 1 | `asc_atomic_add` | float | 64×2048 | 4194304 (2²²) |

**核心实现**：启动64个线程块，每个线程块2048个线程。各线程按固定步长遍历数组，每个元素执行如下操作：

1. 从 GM 读取 `b[i]`、`c[i]` 到核内寄存器
2. 在核内计算 `b[i] * c[i]`
3. 调用 [asc_atomic_add()](../../../../../docs/zh/api/SIMT-API/atomic_operations/asc_atomic_add.md) 将乘积累加到 `a[i]`

`a[i]` 的旧值不进入核内寄存器，读-改-写三步在存储侧（L2 Cache）就地完成。

对应的核函数实现如下。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_atomic_update(
    float* a, const float* b, const float* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        asc_atomic_add(&a[index], b[index] * c[index]);
    }
}
```

**性能数据**：

| Case | 实现方式 | Task Duration（μs） | 加速比 |
|:---:|:---:|:---:|:---:|
| 1 | `asc_atomic_add` | 46.82 | 1.110× |

Case 1 的耗时约 46.82μs，相比 Case 0 获得 **1.110×** 的性能提升。每个元素在核与内存之间产生 3 次事务：2 次读（`b`、`c`）+ 1 次原子请求（携带地址与待加值）。

---

## 性能对比

| Case | 实现方式 | Task Duration（μs） | 加速比 |
|:---:|:---:|:---:|:---:|
| 0 | 普通读-改-写 | 51.96 | - |
| 1 | `asc_atomic_add` | 46.82 | **1.110×** |

**结论**：在无同地址竞争的读-改-写场景下，用 `asc_atomic_add` 替代普通的三步操作，可获得约 **1.11×** 的性能收益。

---

## 优化原理

### 事务数优化

普通读-改-写要求核心显式读取 `a[i]` 的旧值到寄存器，而 `asc_atomic_add` 将读-改-写融合在存储侧完成，旧值不进入核心。每个元素的核与内存之间的事务数从 4 次降为 3 次。

**L2 Cache 事件计数器验证**（采集工具：msopprof，指标：L2Cache，规模：2²² 元素）：

| 实现方式 | L2 读事件 | L2 写事件 | L2 写 victim |
|---|---:|---:|---:|
| 普通读-改-写 | 307268 | 131072 | 0 |
| `asc_atomic_add` | 207958 | 105124 | 25948 |
| **比值** | **0.677× ≈ 2/3** | **(105124 + 25948) / 131072 = 1.000** | - |

- **读事件降至约 2/3**：三个数组的读流（`a`、`b`、`c`）减少为两个（`b`、`c`），符合预期。
- **写事件总数不变**：`asc_atomic_add` 的写事件数（105124）加上写 victim 数（25948）恰好等于普通读-改-写的写事件数（131072），说明原子操作在写侧不引入额外开销。

### 寄存器压力优化

编译日志显示，`vector_plain_update` 使用 16 个寄存器，而 `vector_atomic_update` 仅使用 14 个寄存器。少一个值（`a[i]` 的旧值）进入寄存器，降低了寄存器压力。

---

## 适用场景与注意事项

**适用场景**：

- 各线程访问的地址互不重叠，不存在同地址竞争
- 读-改-写操作可以表示为原子操作支持的形式（加、减、与、或、异或、最小值、最大值等）

**性能收益**：

- 本样例配置下（2²² 元素，64 blocks × 2048 threads）获得约 1.11× 的加速
- 在部分接近 L2 容量的数据规模下（如 96 MB），收益可能降至约 1.04×
- 规模远小于或远大于 L2 容量时，收益稳定在 1.05–1.19× 区间

**注意事项**：

- 本样例测量的是核与内存之间的事务优化，未测量 DRAM 侧的实际流量变化
- 若存在同地址竞争，原子操作会串行化，反而降低性能；此时应参考 [atomic_histogram](../atomic_histogram/README.md) 样例的分块累加策略

---

## 环境准备

请参考[环境准备](../../../../../README.md#环境准备)进行环境准备。

---

## 编译运行

1. 编译样例

    ```shell
    cd <源码目录>
    
    # 第一次编译前配置环境变量
    source <CANN软件安装目录>/set_env.sh
    
    cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=<芯片架构>
    cmake --build build
    ```

    其中 `<芯片架构>` 为实际部署的NPU硬件架构参数，如 `dav-3510`。

2. 运行样例

    ```shell
    # Case 0：普通读-改-写
    ./build/saxpy_atomic 0
    
    # Case 1：asc_atomic_add
    ./build/saxpy_atomic 1
    ```

3. 性能采集

    使用 msopprof 采集性能数据（`--replay-mode=kernel` 会重放 kernel，需关闭精度校验）：

    ```shell
    # 重新编译，关闭精度校验
    cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=<芯片架构> -DSKIP_VALIDATION=ON
    cmake --build build
    
    # 采集 Case 0
    msopprof --output=prof_case0 --aic-metrics=Memory --replay-mode=kernel --launch-count=1 ./build/saxpy_atomic 0
    
    # 采集 Case 1
    msopprof --output=prof_case1 --aic-metrics=Memory --replay-mode=kernel --launch-count=1 ./build/saxpy_atomic 1
    ```

    性能数据位于 `prof_case*/OPPROF_*/OpBasicInfo.csv` 的 `Task Duration(us)` 列。

    **注意**：编译与性能采集必须使用同一 CANN 版本。若编译使用 CANN 9.2.0，msopprof 使用 CANN 9.1.0，会导致采集结果为空。

---

## 参考资料

- [asc_atomic_add API 文档](../../../../../docs/zh/api/SIMT-API/atomic_operations/asc_atomic_add.md)
- [原子操作性能对比样例 (atomic_add_perf)](../../../02_features/01_api_features/02_atomic_operation/atomic_add_perf/README.md)
- [原子操作指令优化指导](../../../../../docs/zh/guide/operator_practice/simt_operator_optimization/instruction_optimization/atomic_instruction_optimization.md)
