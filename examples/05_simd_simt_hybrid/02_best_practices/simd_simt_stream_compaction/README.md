# SIMD与SIMT混合编程稳定流压缩性能调优样例

## 概述

本样例以稳定流压缩（stable stream compaction）算子为例，介绍混合编程场景下使用Warp函数替代显式遍历UB，从而加速计算的优化方法。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── simd_simt_stream_compaction
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── stream_compaction.asc       // 样例实现
│   ├── figures                     // README中的图片资源
│   ├── README.md                   // 样例说明文档
│   └── README_en.md                // 英文样例说明文档
```

## 样例描述

- 算子功能：

  Stream Compaction指的是给定一个输入序列以及目标元素要满足的判断条件，输出满足条件的元素集合，并且元素的相对顺序和在原序列一致。此算子的重难点在于：每个线程事先无法知道命中元素在输出中的位置，输出位置必须由线程间协同计算得到。

- 计算公式：

  设输入为 `input[0..N)`，选择谓词为 `P(x)`。输出包含所有满足 `P(input[i])` 的元素，并保持它们在输入中的相对顺序。每个命中元素的输出位置rank计算方式为：

  ```text
  rank(i) = count { j | 0 <= j < i, P(input[j]) }
  ```

  >样例中使用的筛选条件是选择input中的偶数，即：x % 2 = 0。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">StreamCompaction</td></tr>
  <tr><td align="center">标题</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">input</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">input_count</td><td align="center">标量</td><td align="center">uint32</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">output</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">selected_count</td><td align="center">[1]</td><td align="center">uint32</td><td align="center">-</td></tr>
  </table>

## 样例实现

该样例采用多核分块并行计算：64个线程块各负责一段8192元素的region（总输入524288个int32，命中约50%），每个线程块内1024个线程分8轮tile处理（8192/1024）。由于后面的线程块在计算rank时需要知道前面的线程块命中元素的数量，所以所有case的kernel通过两次VF函数调用实现：第一个VF函数用来统计block_counts（位于Global Memory，方便核间共享）及相关数据，第二个VF函数用来计算output，中间需要通过添加核间同步控制。

### Case实现说明

本样例通过3个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

| Case | 核函数 | 线程块数量 | 每个线程块的线程数 | 本步引入的变化 |
| ---- | ------ | ---------- | ------------------ | -------------- |
| 0 | compact_thread_prefix_kernel | 64 | 1024 | 使用UB缓存线程命中标志，线程级串行累加前缀 |
| 1 | compact_warp_shared_flags_kernel | 64 | 1024 | 分Warp和Lane两级归约，串行累加范围从线程数收敛到warpSize |
| 2 | compact_prescanned_ballot_kernel | 64 | 1024 | 使用Warp函数替代Case 1的显式遍历，进一步减少UB访问 |

### 性能指标说明

| 指标                 | 说明                                                                        |
| -------------------- | --------------------------------------------------------------------------- |
| Task Duration(μs)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间           |
| aiv_time(μs)        | Task在AI Vector Core上的理论执行时间，单位为μs                              |
| aiv_total_cycles    | Task被分配到每个AI Vector Core计算单元上后的执行cycle总数                   |
| aiv_vec_time(μs)    | vec类型指令（向量类运算指令）耗时，SIMT VF调用中的线程计算也体现在该类统计中 |
| aiv_vec_ratio       | vec类型指令的cycle数在total cycle数中的占用比                               |
| aiv_scalar_time(μs) | scalar类型指令（标量类运算指令）耗时，单位为μs                              |
| aiv_scalar_ratio    | scalar类型指令的cycle数在total cycle数中的占用比                            |
| aiv_mte2_time(μs)   | MTE2搬入指令耗时，主要对应GM到UB的搬运                                      |
| aiv_mte2_ratio      | MTE2搬入指令的cycle数在total cycle数中的占用比                            |
| aiv_ub_read_bw_vector(GB/s) | Vector指令读取UB的带宽速率，反映vec流水线从UB取数据的吞吐            |

除Task Duration外，其余指标均为所有Thread Block上的平均值。

### Case 0：使用UB缓存线程命中状态

**优化目标**：观察各线程通过串行累加计算rank时的耗时，建立性能基线。

**核心实现**：第一个VF函数在核内共享内存UB中申请一个数组（thread_selected_flags）用来缓存所有线程的命中状态（命中写1，否则写0），第二个VF函数里命中元素的线程通过将之前所有的命中标志求和作为输出中的rank。如下图所示：

![](./figures/case_0.png)

**关键代码**：

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t input_index = tile * THREAD_COUNT + thread_index;
    int32_t value = input[input_index];
    uint32_t hit = thread_selected_flags[input_index];

    uint32_t thread_prefix = 0U;
    for (uint32_t earlier = 0; earlier < thread_index; ++earlier) {
        thread_prefix += thread_selected_flags[tile * THREAD_COUNT + earlier];
    }

    if (hit == 1U) {
        output[block_base + tile_base[tile] + thread_prefix] = value;
    }
}
```

**性能数据**：
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_ub_read_bw_vector(GB/s) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 380.3 | 377.1 | 622290 | 373.6 | 0.9907 | 0.44 | 0.0012 | 1.37 | 0.0036 | 91.7 |

**性能数据分析**：
Case 0的耗时几乎全部消耗在第二个VF函数的串行前缀累加上：每个线程计算rank时需要逐一累加它前面所有线程的命中标志，该过程是"读标志-累加"的纯串行依赖链。实测aiv_vec_ratio高达0.9906，说明vec流水线被该串行链完全占满；单轮tile循环内thread_selected_flags的访问次数约为1024×1023/2 ≈ 52万次，8轮tile共约420万次UB读，实测Vector到UB的读带宽约91.7GB/s。三个Case中该实现耗时最长，作为后续优化的性能基线。

### Case 1：分级归约代替串行累加

**优化目标**：Case 0的问题在于排在后面的线程在计算rank时，串行累加thread_selected_flags的成本较高，可以通过分级归约减少计算和内存搬运的次数。

**核心实现**：将之前的串行累加thread_selected_flags分成warp和lane两级归约，分别统计每个warp各自命中元素的数量，以及线程在当前warp内的位置，则计算rank时等于前面所有warp命中元素之和加上在当前warp内的位置。如下图所示:

![](./figures/case_1.png)

其中thread_selected_flags和Case 0一致，warp_selected_counts和tile_base均位于UB上，方便核内线程共享。

**关键代码**：

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t input_index = tile * THREAD_COUNT + thread_index;
    int32_t value = input[input_index];
    uint32_t hit = thread_selected_flags[input_index];

    const uint32_t warp_base = warp_index * warpSize;
    const uint32_t flag_base = tile * THREAD_COUNT + warp_base;
    uint32_t lane_offset = 0U;
    for (uint32_t lane = 0U; lane < warpSize; ++lane) {
        if (lane < (uint32_t)laneid()) {
            lane_offset += thread_selected_flags[flag_base + lane];
        }
    }

    if (hit == 1U) {
        uint32_t stable_rank =
            block_base + tile_base[tile] + warp_bases[tile * WARP_COUNT + warp_index] + lane_offset;
        output[stable_rank] = value;
    }
}
```

**性能数据**：
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_ub_read_bw_vector(GB/s) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 25.0 | 24.0 | 39553 | 20.5 | 0.8538 | 0.45 | 0.0187 | 1.38 | 0.0574 | 179.2 |

**性能数据分析**：
Case 1相比于Case 0，串行扫描范围从1024线程缩小到32lane，一方面计算量减少，另一方面单轮tile循环内thread_selected_flags的访问次数从Case 0的约52万次（1024×1023/2）降低到约3.3万次（1024×32），UB访问量降低约16倍，且每次访问集中于本warp内连续的32个标志，实测Vector到UB的读带宽从91.7GB/s提升至179.2GB/s。两者结合使vec耗时从373.6μs降至20.5μs，Task Duration从380.3μs降至25.0μs，约15.2倍。

### Case 2：使用warp接口替代显式遍历

**优化目标**：进一步消除Case 1中针对thread_selected_flags的UB访问和循环遍历。

**核心实现**：Case 1计算warp_selected_count（VF1）和lane_offset（VF2）是通过遍历UB数组得到的，实际上关于warp内数据的计算，存在高性能接口可以直接使用。本例中可以使用[asc_ballot](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_vote_functions/asc_ballot.md)、[lanemask_lt](../../../../docs/zh/api/SIMT-API/Warp_functions/lane_id_functions/lanemask_lt.md)、[asc_reduce_add](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_reduce_functions/asc_reduce_add.md)高效计算得到warp_selected_count和lane_offset，结合预先计算好的每个[tile, warp]的输出基地址tile_warp_base，进一步减少计算rank的耗时。如下图所示：

![](./figures/case_2.png)

**关键代码**：

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t idx = tile * THREAD_COUNT + thread_index;
    uint32_t hit = is_selected_device(input[idx]) ? 1U : 0U;
    uint32_t warp_selected_count = (uint32_t)asc_reduce_add(hit);
    if (lane_index == 0U) {
        tile_warp_base[tile * WARP_COUNT + warp_index] = warp_selected_count;
    }
}

//...
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t idx = tile * THREAD_COUNT + thread_index;
    int32_t value = input[idx];
    bool selected = is_selected_device(value);
    uint32_t selected_mask = asc_ballot(selected ? 1 : 0);
    if (selected) {
        uint32_t lane_offset = (uint32_t)__popc(selected_mask & (uint32_t)lanemask_lt());
        output[block_base + tile_warp_base[tile * WARP_COUNT + warp_index] + lane_offset] = value;
    }
}
```

**性能数据**：
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_ub_read_bw_vector(GB/s) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 9.48 | 8.2 | 13547 | 4.70 | 0.5724 | 0.43 | 0.0529 | 1.37 | 0.1670 | 31.5 |

**性能数据分析**：
Case 2利用这些warp接口直接在寄存器层面交换不同线程间的数据，不仅比访问UB更快，也不再需要维护和访问thread_selected_flags：第二个VF函数中每轮tile仅需一次asc_ballot和一次__popc即可得到lane内偏移，第一个VF函数中warp命中计数改由asc_reduce_add单指令完成。相比Case 1，Vector到UB的读带宽从179.2GB/s降至31.5GB/s（标志数组流量基本消失），vec耗时从20.5μs降至4.70μs，Task Duration从25.0μs降至9.48μs，约2.6倍。

## 性能对比总结


下表给出各个Case在相同输入规模下的Task Duration对比：

| Case | 实现方式 | Task Duration(μs) | 性能相对于Case 0 |
|:---|:---|---:|---:|
| 0 | 使用UB缓存线程命中状态 | 380.3 | 1X |
| 1 | 分级归约代替串行累加 | 25.0 | 15.2X |
| 2 | 使用Warp函数替代显式遍历 | 9.48 | 40.1X |

三个Case的优化是逐步递进的：Case 1将串行累加从Thread Block级收敛到Warp级，主要削减了总的UB读次数；Case 2进一步用warp函数把warp内协作搬到寄存器层，主要削减了vec指令耗时。从指标变化看，性能提升的来源清晰可分：aiv_vec_time从373.6μs（Case 0）降至20.5μs（Case 1）再降至4.70μs（Case 2），共下降约80倍，是总耗时下降的主体；aiv_mte2_time在三个Case中基本不变（约1.4μs），因此占比反而从0.4%升至16.7%。综合来看，最终9.48μs的Task Duration中，vec计算约4.7μs、mte2搬入约1.4μs，其余为调度与核间同步等开销。

## 调优建议

- 在SIMT编程模型中，要充分发挥多线程并行的优势，不出现或少出现线程内执行大量的循环。
- 线程间协作优先使用[Warp函数](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_functions.md)在寄存器层面交换数据，而非通过UB数组加显式循环。


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
  SCENARIO_NUM=2                                                                     # 选择执行场景，可选0-2
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # 编译工程
  ./stream_compaction                                                                # 执行样例
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR&950DT系列产品） |
  | `SCENARIO_NUM`            | `0`-`2`  | 样例类型，默认为2 |

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
