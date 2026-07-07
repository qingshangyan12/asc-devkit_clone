# asc_atomic_add原子加性能样例

## 概述

本样例以[asc_atomic_add()](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/原子操作/asc_atomic_add.md)原子加为例，通过五组对照实验，展示内存层级（Global Memory / Unified Buffer）、原子操作的组织方式（直接原子加 / 线程块内先累加再写回GM）、是否使用返回值、以及数据类型（int32 / int64）对原子加性能的影响，并给出相应的优化指导。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── atomic_add_perf
│   ├── figures                // README中的图片资源
│   ├── CMakeLists.txt         // 样例构建脚本
│   ├── atomic_add_perf.asc    // 样例代码
│   ├── README.md
│   └── README_en.md
```

## 样例描述

本样例通过`SCENARIO_NUM`选择一个待测场景，每次运行仅启动一个目标Kernel。12个场景归为5组对照实验（Case 1~5），每组只改变一个属性、其余保持一致，从而单独观察该属性对原子加性能的影响。

每个线程执行一次`asc_atomic_add()`，因此最终累加值等于线程总数：

```text
TOTAL_ACCUMULATED_VALUE = BLOCK_NUM * THREADS_PER_BLOCK
```

**场景清单**：

| 场景编号 | 对照Case编号 | 内存位置 | 数据类型 | 返回值 | 规模（Block×Thread） |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | Case 1 | Global Memory | int32_t | 否 | 1×1024 |
| 2 | Case 1 | Unified Buffer | int32_t | 否 | 1×1024 |
| 3 | Case 2 | Global Memory | int32_t | 否 | 8×1024 |
| 4 | Case 2 | Unified Buffer | int32_t | 否 | 8×1024 |
| 5 | Case 3 | Global Memory | int32_t | 是 | 1×2048 |
| 6 | Case 3 | Global Memory | int32_t | 否 | 1×2048 |
| 7 | Case 3 | Unified Buffer | int32_t | 是 | 1×2048 |
| 8 | Case 3 | Unified Buffer | int32_t | 否 | 1×2048 |
| 9 | Case 4 | Global Memory | int32_t | 否 | 48×256 |
| 10 | Case 4 | Global Memory | int64_t | 否 | 48×256 |
| 11 | Case 5 | Global Memory | int32_t | 是 | 48×256 |
| 12 | Case 5 | Global Memory | int64_t | 是 | 48×256 |

**5组对照**：

| 对照维度 | 参与场景 | 对照变量 | 对照Case |
|---|---|---|:---:|
| 单线程块下的内存位置 | 场景1 ↔ 场景2 | Global Memory → Unified Buffer | Case 1 |
| 多线程块下的内存位置 | 场景3 ↔ 场景4 | Global Memory → Unified Buffer | Case 2 |
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

### Case 1：不同内存空间性能差异（单线程块场景）

**样例目标**：在所有线程竞争同一个地址的场景下，对比原子加落在Global Memory与Unified Buffer上的性能差异。

**核心实现**：单线程块启动1024个线程。GM场景下1024个线程对同一个GM地址执行原子加；UB场景下1024个线程对同一个UB地址执行原子加。Case 1只对比接口访问GM和访问UB的开销，UB场景不做正确性校验。

```cpp
// 场景1：GM单地址，所有线程竞争同一个GM计数器
__global__ void atomic_add_gm_single_no_return(int32_t* counter)
{
    asc_atomic_add(counter, 1);  // 1024个线程访问同一GM地址
}

// 场景2：UB单地址，所有线程调用同一个UB地址上的原子加接口
__global__ void atomic_add_ub_local_no_return()
{
    __ubuf__ int32_t local[1];
    asc_atomic_add(&local[0], 1);  // 1024个线程访问同一UB地址
}
```

**性能数据**：

| 场景 | 内存 | Task Duration（μs） |
|:---:|:---:|---:|
| 1 | Global Memory | 87.60 |
| 2 | Unified Buffer | 1.84 |

**分析**：

- 相同竞争线程数下，GM场景约87.60μs，UB场景约1.84μs，相差约**48倍**。UB场景的原子操作显著快于Global Memory上的原子操作。

**结论**：可在UB完成的原子累加，不宜在GM上执行。

---

### Case 2：不同内存空间性能差异（多线程块场景）

**样例目标**：在多个线程块同时竞争一个GM地址的场景下，对比“所有线程直接原子加到同一个GM地址”与“先在各线程块自己的UB中累加、再由每个线程块写回GM”两种方式的性能差异。

**核心实现**：启动8个线程块、每个线程块1024个线程（共8192个线程）。GM场景下8192个线程直接对同一个GM地址执行原子加；UB场景下每个线程块先在自己的UB数组`local[1]`上累加，再由每个线程块各做一次GM原子加写回，落在GM上的原子操作从8192次降为8次。

```cpp
// 场景3：GM单地址，8192个线程都直接竞争同一个GM计数器
__global__ void atomic_add_gm_single_no_return(int32_t* counter)
{
    asc_atomic_add(counter, 1);
}

// 场景4：每个线程块先在自己的UB中累加，再由线程0写回GM
__global__ void atomic_add_ub_block_accum_no_return(int32_t* gm_result)
{
    __ubuf__ int32_t local[1];
    if (threadIdx.x == 0) {
        local[0] = 0;
    }
    asc_syncthreads();

    asc_atomic_add(&local[0], 1);  // 1024个线程访问本线程块的UB地址
    asc_syncthreads();

    if (threadIdx.x == 0) {
        asc_atomic_add(gm_result, local[0]);  // 每个线程块仅1次GM原子加
    }
}
```

**性能数据**：

| 场景 | 组织方式 | Task Duration（μs） |
|:---:|:---:|---:|
| 3 | GM直接原子加 | 351.47 |
| 4 | UB块内累加后写回GM | 2.25 |

**分析**：

- GM是所有线程块的线程都可以直接访问的内存。场景3中，8个线程块上的8192个线程都直接对同一个GM地址做原子加，耗时约351.47μs。相比Case 1，Case 2中GM直接原子加的线程数从1024增加到8192，落在同一个GM地址上的原子操作次数同步增加，耗时显著上升。
- UB只有当前线程块内部的线程都可以访问，不同线程块使用各自的UB。场景4中，每个线程块先在自己的UB中得到块内累加结果，最后只向GM写回一次。这样落在GM上的原子操作次数从8192次降为8次，耗时约2.25μs。
- 采用UB块内累加后写回GM的方式只保留8次GM原子加，两种方式最终相差约**156倍**。

**结论**：当多个线程块要进行同地址原子累加时，不建议让每个线程都直接对GM做原子加。更推荐先在每个线程块的UB中得到一个块内累加结果，再把这个结果累加在GM上。

---

### Case 3：返回值开销

**样例目标**：分别在GM和UB上，对比`asc_atomic_add()`使用返回值与不使用返回值的性能差异。

**核心实现**：单线程块启动2048个线程，仅改变“是否使用返回值”。GM与UB各测使用返回值、不使用返回值两种。UB场景只调用UB地址上的原子加接口，不初始化、不写回GM，也不做结果校验。使用返回值时将旧值写入线程本地`volatile`变量，避免返回值路径被编译器优化消除。

**性能数据**：

| 内存 | 使用返回值（μs） | 不使用返回值（μs） | 耗时比值 |
|:---:|---:|---:|:---:|
| GM int32（场景5 vs 6） | 421.59 | 159.88 | 2.64× |
| UB int32（场景7 vs 8） | 3.17 | 2.76 | 1.15× |

**分析**：

- **GM int32**：GM上不使用返回值时硬件会执行优化后的指令，要比使用返回值快约2.6倍。
- **UB int32**：UB上优化前和优化后的原子操作指令耗时相差较小，使用返回值约为不使用返回值的1.15倍。

**结论**：非必要业务场景，不应使用接口的返回值。

---

### Case 4：数据类型（无返回值）

**样例目标**：在GM上，多个线程通过取模映射到一组紧密排布的地址上、存在真实同地址竞争、且不使用返回值的场景下，对比int32与int64原子加的性能。
> [!NOTE]说明
> 由于int64原子加仅支持GM，本样例的数据类型对照仅在GM场景下进行。

**核心实现**：启动48个线程块、每个线程块256个线程（共12288个线程），线程通过`地址 = base + (全局线程号 % TARGET_COUNT)`映射到一组紧密排布的地址上，`TARGET_COUNT=1024`，即约12个线程竞争同一个地址，存在同地址竞争。GM场景分别用int32与int64，均不使用返回值。

```cpp
// 多个线程通过取模落到同一个地址，存在同地址竞争
__global__ void atomic_add_gm_dense_i32_no_return(int32_t* counters, uint64_t target_count)
{
    // 不使用返回值
    asc_atomic_add(counters + ((blockIdx.x * blockDim.x + threadIdx.x) % target_count), 1);
}
```

**性能数据**：

| 数据类型 | Task Duration（μs） |
|:---:|---:|
| int32（场景9） | 1.89 |
| int64（场景10） | 58.46 |

**分析**：

- int32不使用返回值时会触发硬件指令优化，即使约12个线程竞争同一地址，耗时仍低至1.89μs。
- int64没有该优化，同等竞争下耗时高达58.46μs。
- 存在同地址竞争时，int32约比int64快**31倍**。

**结论**：纯计数等不使用返回值的场景，优先使用int32而非int64。

---

### Case 5：数据类型（有返回值）

**样例目标**：在GM上，使用与Case 4相同的紧凑地址布局、且使用返回值的场景下，对比int32与int64原子加的性能，说明不能仅凭“数据类型更窄就更快”的直觉选择原子加数据类型。

> [!NOTE]说明
> 由于int64原子加仅支持GM，本样例的数据类型对照仅在GM场景下进行。

**核心实现**：与Case 4使用相同的线程规模和地址映射方式（48个线程块×256线程，`地址 = base + (全局线程号 % TARGET_COUNT)`，`TARGET_COUNT=1024`），区别是Case 5使用`asc_atomic_add()`的返回值。GM场景分别用int32与int64数据类型进行对比。

```cpp
// 与Case 4相同的地址映射，但使用asc_atomic_add()返回值
__global__ void atomic_add_gm_dense_i32_return(int32_t* counters, uint64_t target_count)
{
    volatile int32_t old_value_sink =
        asc_atomic_add(counters + ((blockIdx.x * blockDim.x + threadIdx.x) % target_count), 1);
    (void)old_value_sink;
}
```

**性能数据**：

| 数据类型 | Task Duration（μs） |
|:---:|---:|
| int32（场景11） | 110.95 |
| int64（场景12） | 54.28 |

**分析**：

- 与直觉相悖：窄类型int32比宽类型int64慢约**2倍**（110.95 vs 54.28）。
- 原子操作访问GM地址时，会经过L2缓存层级，并由L2侧的原子处理单元完成数据处理。L2缓存不是按单个int32或int64单独处理，而是以一段连续内存为基本单位进行缓存和访问，这段连续内存称为CacheLine。当需要用到返回值时，如果多个线程同时更新同一地址，普通读写可能出现多个线程读到同一个旧值、后写回的结果覆盖先写回的结果，导致部分加法丢失；原子操作用于避免这种问题，因此“读出旧值、计算新值、写回新值”必须作为一个不可分割的整体完成；L2侧会按CacheLine对相关原子操作进行排队处理。因此，分析GM原子操作排队时，不能只看单个变量，还要看哪些操作落在同一条CacheLine上。

- 因此int32比int64慢的根因在于**同一条CacheLine上的原子操作需要排队**：CacheLine大小为128B，即使线程修改的是各自独立、彼此不冲突的地址，只要这些地址同属一条CacheLine，就会产生排队开销；落在不同CacheLine上的原子操作可以并行处理。相同的原子加总数下，每条CacheLine上可容纳32个int32地址或16个int64地址，int32每条CacheLine上排队的原子操作数约为int64的2倍，耗时也近似翻倍。

  | 数据类型 | 单个地址大小 | 每128B CacheLine容纳地址数 |
  |:---:|:---:|:---:|
  | int32 | 4B | 128B/4B = 32 |
  | int64 | 8B | 128B/8B = 16 |
- 该现象与int32类型本身无关，仅源于紧密排布下每条CacheLine容纳的地址数量更多。

对同一CacheLine上的原子操作排队示意图如下：

<img src="figures/case5.png" alt="local_his" style="width: 70%; height: auto;">

**结论**：不应凭“类型更窄则更快”的直觉为紧密排布的GM地址选择int32。即使各线程访问的地址互不冲突，窄类型每条CacheLine容纳的原子累加的地址更多，同一条CacheLine上排队的原子操作也更多，反而更慢。对于频繁原子更新的GM数组，可以通过padding或结构体对齐，降低L2 CacheLine粒度上的排队开销。

---

## 性能对比总结

**全场景Task Duration汇总**：

| 场景 | 对照Case编号 | 内存 | 数据类型 | 返回值 | 规模 | Task Duration（μs） |
|:---:|:---:|:---:|:---:|:---:|:---:|---:|
| 1 | Case 1 | GM |   int32 | 否 | 1×1024 | 87.60 |
| 2 | Case 1 | UB |  int32 | 否 | 1×1024 | 1.84 |
| 3 | Case 2 | GM |   int32 | 否 | 8×1024 | 351.47 |
| 4 | Case 2 | UB |  int32 | 否 | 8×1024 | 2.25 |
| 5 | Case 3 | GM |   int32 | 是 | 1×2048 | 421.59 |
| 6 | Case 3 | GM |   int32 | 否 | 1×2048 | 159.88 |
| 7 | Case 3 | UB |  int32 | 是 | 1×2048 | 3.17 |
| 8 | Case 3 | UB |  int32 | 否 | 1×2048 | 2.76 |
| 9 | Case 4 | GM |   int32 | 否 | 48×256 | 1.89 |
| 10 | Case 4 | GM |   int64 | 否 | 48×256 | 58.46 |
| 11 | Case 5 | GM |   int32 | 是 | 48×256 | 110.95 |
| 12 | Case 5 | GM |  int64 | 是 | 48×256 | 54.28 |

**五组对照收益**：

| 优化维度 | 关键对照 | 收益量级 | 建议 |
|---|---|---|---|
| 内存层级 | UB（1.84μs） vs GM（87.60μs） | ~48× | 可放在UB中的临时累加优先放在UB上执行 |
| 原子组织方式 | UB块内累加后写回GM（2.25μs） vs GM直接原子加（351.47μs） | ~156× | 多线程块向同一地址累加时，先在各线程块UB中累加，再少量写回GM |
| 返回值 | GM不使用返回值（159.88μs） vs 使用返回值（421.59μs） | ~2.6× | 非业务需要，不使用返回值 |
| 数据类型（有优化） | int32（1.89μs） vs int64（58.46μs） | ~31× | 不使用返回值时优先int32；其优化对同地址竞争稳健 |
| 数据类型（无优化） | int32（110.95μs） vs int64（54.28μs） | ~0.5× | 紧密布局下窄类型同CacheLine排队更多，不应仅按类型宽窄推断性能 |

## 调优建议

1. **优先降低原子操作的内存层级**：可在UB中完成的原子累加不宜在GM上执行。
2. **多线程块向同一地址累加时先在UB中累加**：先在各线程块自己的UB中累加，再由每个线程块做一次GM原子写回，避免所有线程直接在GM上排队，同时减少落在GM上的原子操作次数。
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
  | `SCENARIO_NUM` | `1`-`12` | 待测场景编号。每个场景的规模（Block×Thread）、数据类型、是否使用返回值等均由源码中`get_scenario_config()`统一配置 |
  | `TARGET_COUNT` | 正整数 | 仅Case 4/5（场景9~12）生效：地址数组大小，线程按`全局线程号 % TARGET_COUNT`映射到地址。留空使用默认值`1024`（约12个线程竞争一个地址）；减小可加剧竞争、增大可减弱竞争 |
  | `SKIP_VALIDATION` | `ON`/`OFF` | 是否跳过结果校验，默认`OFF`。Case 1/3的UB-only场景没有GM结果，会自动跳过校验；使用`msopprof`采集GM写回场景时建议设为`ON` |

  执行结果如下，说明正确性校验通过。

  ```text
  Scenario 1: Case1 GM int32 single-address, no return value
  Workload: blocks=1 threads_per_block=1024 total_atomic_adds=1024
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

> [!NOTE]说明
> **关于性能采集时的Validation failed：** 除Case 1/3的UB-only场景外，样例场景的最终累加结果会写回GM计数器，该计数器仅在host侧分配时清零一次，kernel仅累加、不在入口清零。`msopprof`的warmup+replay会在同一块GM内存上重复执行kernel，计数器被累加多次，因此严格校验模式下会报`Validation failed`（计数为期望值的整数倍）。
>
> Case 2的UB块内累加场景虽然UB上的中间累加器在kernel入口清零、对replay幂等，但最终仍通过`asc_atomic_add`写回GM计数器，该GM计数器在replay下同样被多次累加，因此校验同样会失败。
>
> 该现象是replay机制与校验逻辑的固有冲突。采集所有场景时均建议通过`-DSKIP_VALIDATION=ON`跳过校验。
