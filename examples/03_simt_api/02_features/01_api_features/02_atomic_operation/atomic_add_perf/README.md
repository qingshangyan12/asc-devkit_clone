# asc_atomic_add原子加性能样例

## 概述

本样例以`asc_atomic_add()`原子加为例，通过五组对照实验，展示内存层级（Global Memory / Unified Buffer）、原子操作的组织方式（直接原子加 / 两级归约）、是否使用返回值、以及数据类型（int32 / int64）对原子加性能的影响，并给出相应的优化指导。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── atomic_add_perf
│   ├── CMakeLists.txt         // 样例构建脚本
│   ├── atomic_add_perf.asc    // 样例代码
│   ├── README.md
│   └── README_en.md
```

## 样例描述

本样例通过`SCENARIO_NUM`选择一个待测场景，每次运行仅启动一个目标Kernel。12个场景归为5组对照实验（Case 1~5），每组只改变一个属性、其余保持一致，从而单独观察该属性对原子加性能的影响。

每个线程执行一次`asc_atomic_add()`，因此原子加总次数等于线程总数：

```text
TOTAL_ATOMIC_ADDS = BLOCK_NUM * THREADS_PER_BLOCK
```

**场景清单**：

| 场景编号 | 对照 | 内存位置 | 数据类型 | 返回值 | 规模（Block×Thread） |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | Case 1 | Global Memory | int32_t | 否 | 1×2048 |
| 2 | Case 1 | Unified Buffer | int32_t | 否 | 1×2048 |
| 3 | Case 2 | Global Memory | int32_t | 否 | 8×256 |
| 4 | Case 2 | Unified Buffer | int32_t | 否 | 8×256 |
| 5 | Case 3 | Global Memory | int32_t | 是 | 1×2048 |
| 6 | Case 3 | Global Memory | int32_t | 否 | 1×2048 |
| 7 | Case 3 | Unified Buffer | int32_t | 是 | 1×2048 |
| 8 | Case 3 | Unified Buffer | int32_t | 否 | 1×2048 |
| 9 | Case 4 | Global Memory | int32_t | 否 | 48×256 |
| 10 | Case 4 | Global Memory | int64_t | 否 | 48×256 |
| 11 | Case 5 | Global Memory | int32_t | 是 | 48×256 |
| 12 | Case 5 | Global Memory | int64_t | 是 | 48×256 |

**5组对照**：

| 对照维度 | 参与场景 | 唯一变化的属性 | 对应下文 |
|---|---|---|:---:|
| 单线程块竞争 | 场景1 ↔ 场景2 | Global Memory → Unified Buffer | Case 1 |
| 多线程块竞争 | 场景3 ↔ 场景4 | Global Memory → Unified Buffer | Case 2 |
| 是否使用返回值 | 场景5 ↔ 场景6（GM）<br>场景7 ↔ 场景8（UB） | 使用返回值 → 不使用返回值 | Case 3 |
| 数据类型（无返回值） | 场景9 ↔ 场景10 | int32_t → int64_t | Case 4 |
| 数据类型（有返回值） | 场景11 ↔ 场景12 | int32_t → int64_t | Case 5 |

## 样例实现

本节先说明性能指标，再以五组对照（Case 1~5）逐一分析内存层级、原子组织方式、返回值、数据类型对原子加性能的影响。每组对照只改变一个属性、其余保持一致，并给出对应的`msopprof`实测数据与优化建议。

### 性能指标说明

| 指标 | 说明 |
| --- | --- |
| Task Duration（μs） | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |


---

### Case 1：单线程块竞争下的GM原子加 vs UB两级归约

**样例目标**：在所有线程竞争同一个地址的场景下，对比原子加落在Global Memory与Unified Buffer上的性能差异。

**核心实现**：单线程块启动2048个线程。GM场景下2048个线程对同一个GM地址执行原子加；UB场景下2048个线程先在本线程块的一个UB累加器上执行原子加，线程块内累加完成后由一个线程写回GM。

```cpp
// 场景1：GM单地址，所有线程竞争同一个GM计数器
__global__ void atomic_add_gm_single_no_return(int32_t* counter)
{
    asc_atomic_add(counter, 1);  // 2048个线程访问同一地址
}

// 场景2：UB两级归约，竞争收敛到线程块内UB
__global__ void atomic_add_ub_reduce_no_return(int32_t* gm_result)
{
    __ubuf__ int32_t local;
    if (threadIdx.x == 0) {
        local = 0;
    }
    asc_syncthreads();

    asc_atomic_add(&local, 1);   // 竞争发生在UB上
    asc_syncthreads();

    if (threadIdx.x == 0) {
        asc_atomic_add(gm_result, local);  // 每个线程块仅1次GM原子加
    }
}
```

**性能数据**：

| 场景 | 内存 | Task Duration（μs） |
|:---:|:---:|---:|
| 1 | Global Memory | 144.36 |
| 2 | Unified Buffer | 3.13 |

**分析**：

- 相同竞争线程数下，GM场景约144μs，UB场景约3μs，相差约 **46倍**。在Unified Buffer上执行原子操作，耗时远低于在Global Memory上执行原子操作。

**结论**：可在UB完成的原子累加，不宜在GM上执行。

---

### Case 2：多线程块竞争下的GM直接原子加 vs UB两级归约

**样例目标**：在多个线程块同时竞争一个地址的场景下，对比“所有线程直接原子加到同一个GM地址”与“先在各线程块UB内归约、再写回GM”两种组织方式的性能差异。

**核心实现**：启动8个线程块、每个线程块256个线程（共2048个线程）。GM场景下2048个线程直接对同一个GM地址执行原子加；UB场景下每个线程块先在本线程块UB累加器上归约，再由每个线程块各做一次GM原子加写回，GM上的原子操作从2048次降为8次。两个场景复用Case 1的两个Kernel，仅改变线程块数与每个线程块的线程数。

**性能数据**：

| 场景 | 组织方式 | Task Duration（μs） |
|:---:|:---:|---:|
| 3 | GM直接原子加 | 87.59 |
| 4 | UB两级归约 | 1.47 |

**分析**：

- 跨线程块竞争同一个GM地址时，所有原子操作在GM上排队，约88μs；改为两级归约后，竞争收敛到UB内、GM上仅保留少量原子操作，约1.5μs，相差约 **60倍**。
- 两级归约大幅减少了落在GM上的原子操作次数（2048 → 8）。

**结论**：多线程块向GM地址累加时，优先采用“线程块内UB归约 + 每个线程块一次GM写回”的两级归约，避免所有线程在GM同地址上的排队等待开销。

---

### Case 3：返回值开销

**样例目标**：对比`asc_atomic_add()`使用返回值与不使用返回值的性能差异。

**核心实现**：单线程块启动2048个线程，复用Case 1，仅改变“是否使用返回值”。GM与UB各测使用返回值、不使用返回值两种。使用返回值时将旧值写入线程本地`volatile`变量，避免返回值路径被编译器优化消除。

**性能数据**：

| 内存 | 使用返回值（μs） | 不使用返回值（μs） | 使用返回值/不使用返回值比值 |
|:---:|---:|---:|:---:|
| GM int32（场景5 vs 6） | 380.50 | 148.06 | 2.57× |
| UB int32（场景7 vs 8） | 3.41 | 3.11 | 1.10× |

**分析**：

- **GM int32**：GM上不使用返回值时硬件会执行指令优化，要比使用返回值快约2.6倍。
- **UB int32**：UB上优化前和优化后的原子操作指令耗时相差较小，不使用返回值仅为使用返回值的1.1倍。

**结论**：非必要业务场景，不应使用接口的返回值。

---

### Case 4：数据类型（无返回值）

**样例目标**：在多个线程通过取模映射到一组紧密排布的地址上、存在真实同地址竞争、且不使用返回值的场景下，对比int32与int64原子加的性能。

**核心实现**：启动48个线程块、每个线程块256个线程（共12288个线程），线程通过`地址 = base + (全局线程号 % TARGET_COUNT)`映射到一组紧密排布的地址上，`TARGET_COUNT=1024`，即约12个线程竞争同一个地址，存在同地址竞争。GM场景分别用int32与int64，均不使用返回值。

```cpp
// 多个线程通过取模落到同一个地址，存在同地址竞争
__global__ void atomic_add_gm_dense_i32_no_return(int32_t* counters)
{
    // 不使用返回值
    asc_atomic_add(counters + ((blockIdx.x * blockDim.x + threadIdx.x) % TARGET_COUNT), 1);
}
```

**性能数据**：

| 数据类型 | Task Duration（μs） |
|:---:|---:|
| int32（场景9） | 1.89 |
| int64（场景10） | 58.46 |

**分析**：

- int32不使用返回值时触发硬件优化，即使约12个线程竞争同一地址，耗时仍低至1.89μs。
- int64没有该优化，同等竞争下耗时高达58.46μs。
- 存在同地址竞争时，int32约比int64快 **31倍**。

**结论**：纯计数等非必要读回旧值的场景，优先使用int32而非int64。

---

### Case 5：数据类型（有返回值）

**样例目标**：在与Case 4相同的紧凑地址布局、使用返回值的场景下，说明同一条CacheLine上的原子操作会产生排队开销。

**核心实现**：与Case 4相同的映射布局（48个线程块×256线程，`地址 = base + (全局线程号 % TARGET_COUNT)`，`TARGET_COUNT=1024`），但使用返回值。GM场景分别用int32与int64数据类型进行对比。

对同一CacheLine上的原子操作排队示意图如下：

<img src="figures/case5.png" alt="local_his" style="width: 70%; height: auto;">

**性能数据**：

| 数据类型 | Task Duration（μs） | 每128B CacheLine容纳地址数 |
|:---:|---:|:---:|
| int32（场景11） | 110.95 | 128B/4B = 32 |
| int64（场景12） | 54.28 | 128B/8B = 16 |

**分析**：

- 与直觉相悖：窄类型int32比宽类型int64慢约 **2倍**（110.95 vs 54.28）。
- 根因在于 **同一条CacheLine上的原子操作需要排队**：GM原子操作以一条**128B CacheLine**为最小串行化单位，落在同一条CacheLine上的原子操作必须排队逐笔执行，即使它们修改的是各自独立、彼此不冲突的地址。相同的原子加总数下，1024个int32地址仅占32条CacheLine、1024个int64地址占64条，int32每条CacheLine上排队的原子操作数约为int64的2倍，耗时也近似翻倍。
- 该现象与int32类型本身无关，仅源于紧密排布下每条CacheLine容纳的地址数量更多。

**结论**：不应凭“类型更窄则更快”的直觉为紧密排布的GM地址选择int32。即使各线程访问的地址互不冲突，窄类型每条CacheLine容纳的原子累加的地址更多，同一条CacheLine上排队的原子操作也更多，反而更慢。可通过为数据添加间距（padding），使每条CacheLine仅放置更少的原子累加地址以减少排队开销。

---

## 性能对比总结

**全场景Task Duration汇总**：

| 场景 | 对照 | 内存 | 数据类型 | 返回值 | 规模 | Task Duration（μs） |
|:---:|:---:|:---:|:---:|:---:|:---:|---:|
| 1 | Case 1 | GM |   int32 | 否 | 1×2048 | 144.36 |
| 2 | Case 1 | UB |  int32 | 否 | 1×2048 | 3.13 |
| 3 | Case 2 | GM |   int32 | 否 | 8×256 | 87.59 |
| 4 | Case 2 | UB |  int32 | 否 | 8×256 | 1.47 |
| 5 | Case 3 | GM |   int32 | 是 | 1×2048 | 380.50 |
| 6 | Case 3 | GM |   int32 | 否 | 1×2048 | 148.06 |
| 7 | Case 3 | UB |  int32 | 是 | 1×2048 | 3.41 |
| 8 | Case 3 | UB |  int32 | 否 | 1×2048 | 3.11 |
| 9 | Case 4 | GM |   int32 | 否 | 48×256 | 1.89 |
| 10 | Case 4 | GM |   int64 | 否 | 48×256 | 58.46 |
| 11 | Case 5 | GM |   int32 | 是 | 48×256 | 110.95 |
| 12 | Case 5 | GM |  int64 | 是 | 48×256 | 54.28 |

**五组对照收益**：

| 优化维度 | 关键对照 | 收益量级 | 建议 |
|---|---|---|---|
| 内存层级 | UB（3.13μs） vs GM（144.36μs） | ~46× | 原子累加在UB上执行 |
| 原子组织方式 | UB两级归约（1.47μs） vs GM直接原子加（87.59μs） | ~60× | 多线程块向同一地址累加时采用两级归约 |
| 返回值 | GM不使用返回值（148.06μs） vs使用返回值（380.50μs） | ~2.6× | 非业务需要，不使用返回值 |
| 数据类型（有优化） | int32（1.89μs） vs int64（58.46μs） | ~31× | 不使用返回值时优先int32；其优化对同地址竞争稳健 |
| 数据类型（无优化） | int32（110.95μs） vs int64（54.28μs） | ~0.5× | 紧密布局下窄类型同CacheLine排队更多，不应仅按类型宽窄推断性能 |

## 调优建议

1. **优先降低原子操作的内存层级**：可在UB中完成的原子累加不宜在GM上执行。
2. **多线程块向同一地址累加时采用两级归约**：先在各线程块UB内归约，再由每个线程块做一次GM原子写回，避免所有线程在GM上排队，同时减少落在GM上的原子操作次数。
3. **纯计数场景不使用返回值**：非必要业务场景，不应使用接口的返回值；并且优先使用int32而非int64，因为int32在不使用返回值时会触发硬件优化，int64则不具备此优化。
4. **不应以数据类型宽窄推断原子性能**：性能取决于每条CacheLine容纳的地址个数，而非类型本身。即使各线程访问的地址互不冲突，紧密布局下窄类型每条CacheLine容纳更多原子操作的地址，同一条CacheLine上排队的原子操作也更多，反而更慢。高竞争或紧密布局场景可为数据添加间距（padding），使每条CacheLine放置少数原子操作的地址。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  编译并运行场景1：

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
  make -j
  ./atomic_add_perf
  ```

  编译选项说明：

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `1`-`12` | 待测场景编号 |
  | `BLOCK_NUM` | 正整数 | ThreadBlock数量；留空则使用该场景的默认值 |
  | `THREADS_PER_BLOCK` | 正整数 | 每个ThreadBlock中的线程数；留空则使用该场景的默认值 |
  | `TARGET_COUNT` | 正整数 | 仅Case 4/5（场景9~12）生效：地址数组大小，线程按`全局线程号 % TARGET_COUNT`映射到地址。留空使用默认值`1024`（约12个线程竞争一个地址）；减小可加剧竞争、增大可减弱竞争 |
  | `SKIP_VALIDATION` | `ON`/`OFF` | 是否跳过结果校验，默认`OFF`。使用`msopprof`采集GM场景时建议设为`ON` |

  执行结果如下，说明正确性校验通过。

  ```text
  Scenario 1: Case1 GM int32 single-address, no return value
  Workload: blocks=1 threads_per_block=2048 total_atomic_adds=2048
  Validation passed
  ```

## 性能分析

使用`msopprof`工具获取详细性能数据：

```bash
msprof op ./atomic_add_perf
```

命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹，性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```

> **关于GM场景的Validation failed：** GM场景的计数器仅在host侧分配时清零一次，kernel仅累加、不在入口清零。`msopprof`的warmup+replay会在同一块GM内存上重复执行kernel，计数器被累加多次，因此严格校验模式下会报`Validation failed`（计数为期望值的整数倍）。这是replay机制与校验逻辑的固有冲突，**不影响profiling数据采集**，metric仍正常落盘。采集GM场景时建议通过`-DSKIP_VALIDATION=ON`跳过校验。UB场景的计数器在kernel入口自动清零，对replay幂等，校验仍通过。