# SIMD与SIMT混合编程稳定流压缩性能调优样例

## 概述

本样例以稳定流压缩（stable stream compaction）算子为例，介绍混合编程场景下使用Warp原语替代显式循环与UB数组访问，从而加速计算的优化方法。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── simd_simt_stream_compaction
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── stream_compaction.asc       // 样例实现
│   ├── README.md                   // 样例说明文档
│   └── README_en.md                // 英文样例说明文档
```

## 样例描述

- 算子功能：
  Stream Compaction 指的是给定一个输入序列以及目标元素要满足的判断条件，输出满足条件的元素集合，并且元素的相对顺序和在原序列一致。此算子的特点在于每个线程无法提前知道自己的元素在输出中的位置，输出位置必须由线程协作计算得出。

- 计算公式：

  设输入为 `input[0..N)`，选择谓词为 `P(x)`。输出包含所有满足 `P(input[i])` 的元素，并保持它们在输入中的相对顺序。每个命中元素的输出位置rank计算方式为：

  ```text
  rank(i) = count { j | 0 <= j < i, P(input[j]) }
  ```

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">StreamCompaction</td></tr>
  <tr><td align="center">标题</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">input</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">input_count</td><td align="center">标量</td><td align="center">uint32</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">output</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">selected_count</td><td align="center">标量</td><td align="center">uint32</td><td align="center">-</td></tr>
  </table>

- Case概览：

    本样例通过5个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

    | Case | 核函数 | 线程块数量 | 每个线程块的线程数 | 本步引入的变化 |
    | ---- | ------ | ---------- | ------------------ | -------------- |
    | 0 | `compact_atomic_rank_kernel` | 8 | 1024 | 各线程独立计算，通过重扫前缀计算rank |
    | 1 | `compact_thread_prefix_kernel` | 64 | 1024 | 使用UB缓存线程命中标志，线程级串行累加前缀，消除重复扫描与原子竞争 |
    | 2 | `compact_warp_shared_flags_kernel` | 64 | 1024 | 分Warp和Lane两级归约，串行累加范围从线程数收敛到warp数 |
    | 3 | `compact_warp_ballot_kernel` | 64 | 1024 | 使用Warp原语替代Case 2的显式flag遍历，进一步减少UB访问 |
    | 4 | `compact_warp_ballot_batched_kernel` | 64 | 1024 | 每线程批处理4个元素，减少循环次数从而减少总的核内同步开销 |

## 样例实现

各Case的处理数据量不同：Case 0仅处理8192个元素，其余Case处理全量524288个元素（64个block各负责一个8192元素的区域）。原因在于Case 0通过重扫当前位置之前的全部元素计算rank，计算量随输入规模平方增长，全量输入时耗时急剧增加，如果处理全量元素会导致耗时过久。尽管如此，Case 0耗时依旧是所有场景中最多的。

Case 1~4的每个block启用1024个线程，但却需要处理8192个元素，因此需要chunk循环：Case 1~3每个block将8192元素的区域按每1024个元素一个chunk分8轮处理，每轮1024个线程各负责1个元素并协作计算chunk内rank，轮末将本轮命中总数累加进running_offset作为后续chunk的基址，轮间通过asc_syncthreads保证块内线程步调一致；Case 4每线程一次处理4个连续元素，每轮循环覆盖4096个元素，每个blocK只需2轮循环。

### Case 0：原子计数与GM前缀重扫描

**优化目标**：观察各线程完全独立并行计算时的耗时，建立性能基线。

**核心实现**：8个block以grid-stride协作遍历前8192个元素；命中线程通过原子加更新总数`selected_count`，并调用`count_selected_before`方法重扫当前位置之前的元素，得到输出rank。

**关键代码**：

```cpp
for (uint32_t input_index = grid_offset; input_index < input_count; input_index += thread_stride) {
    int32_t value = input[input_index];
    if (!is_selected_device(value)) {
        continue;
    }

    (void)asc_atomic_add(selected_count, 1U);
    uint32_t stable_rank = count_selected_before(input, input_index);
    output[stable_rank] = value;
}
```

**性能数据**：

| 实现 | Task Duration(μs) |
|:---|---:|
| Case 0 | 892.728 |

**性能数据分析**：

耗时来自于两方面：

- 重复扫描：线程命中第i个元素计算输出rank时，需要重扫input[0..i)。本例中，前8192个元素中命中1487个，重扫总量约1487×8192/2≈610万次。
- 原子操作全局竞争：多个线程对同一GM地址selected_count执行asc_atomic_add串行化排队。

Case 0表面上使用了多线程，实际上是假并行，其耗时比启用单线程串行计算还要高。

### Case 1：使用UB缓存线程命中状态

**优化目标**：消除Case 0的重复计算和原子竞争。

**核心实现**：首先各线程将命中标志写入thread_selected_flags，命中写1，否则写0。然后命中线程分别串行累加此前所有线程的标志得到thread_prefix，也即在输出中的rank。

**关键代码**：

```cpp
__ubuf__ uint32_t thread_selected_flags[THREAD_COUNT];
thread_selected_flags[thread_index] = selected ? 1U : 0U;
asc_syncthreads();

uint32_t thread_prefix = 0U;
for (uint32_t earlier = 0; earlier < thread_index; ++earlier) {
    thread_prefix += thread_selected_flags[earlier];
}
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 1 | 353.022 |

**性能数据分析**：
Case 1 相比于 Case 0 耗时从892.728μs降至353.022μs（约2.5倍）：消除了约610万次的重复前缀扫描与同地址原子竞争，每个元素仅读取一次，稳定rank改由UB中的命中标志增量维护。剩余开销为线程级串行累加（每chunk内每线程最多读1024次UB）与每chunk 4次块级同步，由Case 2继续优化。

### Case 2：两级归约计算前缀和

**优化目标**：将Case 1的线程串行调整为分级归约，从而减少总的UB访问次数。

**核心实现**：
和Case 1一样先获取线程的命中状态thread_selected_flags，然后
1. 每个命中线程计算得到在所属warp内的位置偏移`lane_offset`，以及warp内所有线程命中数量之和warp_selected_count。
2. 根据warp_selected_counts[]可以得到，每个warp在所有warp间的位置偏移warp_selected_offsets[]。

则线程命中元素在chunk内的rank等于warp_selected_offset和lane_offset之和。

**关键代码**：

```cpp
uint32_t warp_base = warp_index * WARP_SIZE;
uint32_t lane_offset = 0U;
uint32_t warp_selected_count = 0U;
for (uint32_t lane = 0U; lane < WARP_SIZE; ++lane) {
    uint32_t flag = thread_selected_flags[warp_base + lane];
    warp_selected_count += flag;
    if (lane < lane_index) {
        lane_offset += flag;
    }
}
if (lane_index == 0) {
    warp_selected_counts[warp_index] = warp_selected_count;
}

if (thread_index < WARP_COUNT) {
    const uint32_t lane_index = (uint32_t)laneid();
    uint32_t own_count = warp_selected_counts[thread_index];
    uint32_t inclusive = own_count;
    for (uint32_t d = 1; d < WARP_SIZE; d <<= 1) {
        uint32_t n = (uint32_t)asc_shfl_up((int32_t)inclusive, d);
        if (lane_index >= d) {
            inclusive += n;
        }
    }
    warp_selected_offsets[thread_index] = inclusive - own_count;
}
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 2 | 83.921 |

**性能数据分析**：
Case 1单chunk内thread_selected_flags的访问次数约为1024*(1024-1)/2 ≈ 52 万次，Case 2单chunk内thread_selected_flags的访问次数约为1024*32+ 2\*(32-1)/2 ≈ 3.3 万次，UB访问量大大降低，从而带来了性能的提升。

### Case 3：使用Warp原语替代显式flag遍历

**优化目标**：消除Case 2中针对thread_selected_flags的UB访问和循环遍历。

**核心实现**：使用[asc_ballot](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_vote_functions/asc_ballot.md)将一个warp内线程的命中状态组成掩码selected_mask，再用[__popc](../../../../docs/zh/api/SIMT-API/math_functions/integer_math_functions/__popc.md)搭配[lanemask_lt](../../../../docs/zh/api/SIMT-API/Warp_functions/lane_id_functions/lanemask_lt.md)计算得到warp_selected_count及lane_offset。

**关键代码**：

```cpp
uint32_t selected_mask = asc_ballot(selected ? 1 : 0);
uint32_t warp_selected_count = (uint32_t)__popc(selected_mask);
uint32_t lane_offset = (uint32_t)__popc(selected_mask & (uint32_t)lanemask_lt());
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 3 | 20.087 |

**性能数据分析**：
Case 3利用这些 warp 接口可以直接在寄存器层面交换不同线程间的数据，不仅比访问UB更快，也不需要维护和访问thread_selected_flags，相比Case 2进一步减少了计算耗时。

### Case 4：批处理压缩，每线程一次处理4个元素

**优化目标**：减少Case 3中随chunk循环次数增加而线性增长的核内同步开销。

**核心实现**：每线程一次读入连续的4个元素，命中状态压入4bit位图`valid_bits`，一个批次覆盖4096个输入，整个区域只需2批（Case 3为8个chunk）。批内rank由四部分拼装：running_offset（chunk间累计）+ warp_selected_offset + warp内lane前缀 + sub_offset(线程内子偏移)，相当于在warp间、warp内归约之外，又多了一层线程内归约。

**关键代码**：

```cpp
// 每个线程在一次chunk循环中处理ITEMS_PER_THREAD个元素
for (uint32_t k = 0; k < ITEMS_PER_THREAD; ++k) {
    uint32_t idx = my_base + k;
    bool valid = idx < region_count;
    int32_t value = valid ? input[idx] : 0;
    values[k] = value;
    valid_bits |= (valid && is_selected_device(value)) ? (1U << k) : 0U;
}
uint32_t thread_hits = (uint32_t)__popc(valid_bits);

// 在计算rank时，还需要获取命中元素在线程内的偏移sub_offset
uint32_t inclusive_prefix = warp_inclusive_scan(thread_hits);

uint32_t my_base_rank = running_offsets[0] + warp_selected_offsets[warp_index] + (inclusive_prefix - (uint32_t)__popc(valid_bits));
for (uint32_t k = 0; k < ITEMS_PER_THREAD; ++k) {
    if ((valid_bits & (1U << k)) != 0U) {
        uint32_t sub_offset = (uint32_t)__popc(valid_bits & ((1U << k) - 1U));
        output[my_base_rank + sub_offset] = values[k];
    }
}
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 4 | 16.026 |

**性能数据分析**：
Case 3中，每次chunk循环需要5次核内同步，在总耗时较低的情况下，同步的开销占比就会变大。Case 4相比于Case 3的从5×8约40次同步降到5×2约10次同步，减少了大量的同步调用。虽然批处理后会带来warp内相邻线程起始地址间隔16字节，访存不再连续的代价，但是减少同步次数带来的收益更高。

## 性能对比总结

下表给出各个Case处理524288个int32元素（Case 0仅压缩前8192个）时，在Ascend 950PR（`dav-3510`，1650 MHz，CANN 9.3.0）上使用`msOpProf`采集的性能数据。实际性能会受到芯片频率、CANN版本和输入规模影响，性能数据应在相同设备、频率、CANN版本和输入规模下比较。

| Case | 实现方式 | Task Duration(μs) | 耗时相对于Case 0 |
|:---|:---|---:|---:|
| 0 | 原子计数与GM前缀重扫描 | 892.728 | 1X |
| 1 | 使用UB缓存线程命中状态 | 353.022 | 0.395X |
| 2 | 两级归约计算前缀和 | 83.921 | 0.094x |
| 3 | 使用Warp原语替代显式flag遍历 | 20.087 | 0.023X |
| 4 | 批处理压缩 | 16.026 | 0.018X |

## 调优建议

- **线程间协作优先使用warp原语，而非UB数组加显式循环**：同一warp内的32个线程优先通过现有接口直接在寄存器层面交换数据，避免将标志写入UB再逐线程遍历累加
- **总体耗时较低时，核内同步开销占比变大，应减少同步次数**：块级同步的开销基本固定，不随计算量缩减，当核函数总耗时降到较低水平后，同步占比会显著上升，此时可考虑减少同步次数。

## 编译运行

在本样例根目录执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=4                                                                     # 选择执行场景，可选0-4
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # 编译工程
  ./stream_compaction                                                                # 执行样例
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。若复用已有构建目录切换回NPU模式，需显式传入 `-DCMAKE_ASC_RUN_MODE=npu`，因为CMake会保留旧缓存。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`            | `0`-`4`  | 样例类型，默认为4                                          |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能调试

### msOpProf工具介绍

msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

通过上板性能采集，可以直接测定算子在昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

使用`msOpProf`工具获取详细性能数据：

```bash
msopprof ./stream_compaction
```

命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的性能数据文件夹，文件夹结构示例如下：

```bash
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息（含Task Duration）
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```

查看具体的性能分析结果：

```bash
# 查看Task Duration 以及各项数据
cat ./OPPROF_*/PipeUtilization.csv
```