# SIMD与SIMT混合编程稳定流压缩性能调优样例

## 概述

本样例以稳定流压缩（stable stream compaction）算子为例，介绍混合编程场景下使用Warp原语替代UB数组遍历，从而加速计算的优化方法。

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
  Stream Compaction 指的是给定一个输入序列以及目标元素要满足的判断条件，输出满足条件的元素集合，并且元素的相对顺序和在原序列一致。此算子的重难点在于每个线程无法如何知道命中元素在输出中的位置。

- 计算公式：

  设输入为 `input[0..N)`，选择谓词为 `P(x)`。输出包含所有满足 `P(input[i])` 的元素，并保持它们在输入中的相对顺序。每个命中元素的输出位置rank计算方式为：

  ```text
  rank(i) = count { j | 0 <= j < i, P(input[j]) }
  ```

  >样例中使用的是筛选条件是选择input中的偶数，即：x % 2 == 0。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">StreamCompaction</td></tr>
  <tr><td align="center">标题</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">input</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">input_count</td><td align="center">标量</td><td align="center">uint32</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">output</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">selected_count</td><td align="center">[1]</td><td align="center">uint32</td><td align="center">-</td></tr>
  </table>

- Case概览：

    本样例通过5个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

    | Case | 核函数 | 线程块数量 | 每个线程块的线程数 | 本步引入的变化 |
    | ---- | ------ | ---------- | ------------------ | -------------- |
    | 0 | `compact_thread_prefix_kernel` | 64 | 1024 | 使用UB缓存线程命中标志，线程级串行累加前缀 |
    | 1 | `compact_warp_shared_flags_kernel` | 64 | 1024 | 分Warp和Lane两级归约，串行累加范围从线程数收敛到warp数 |
    | 2 | `compact_prescanned_ballot_kernel` | 64 | 1024 | 使用Warp原语替代Case 2的显式遍历，进一步减少UB访问 |
    | 3 | `compact_prescanned_ballot_short_vector_kernel` | 64 | 1024 | 每线程批处理4个元素，减少循环次数 |

## 样例实现

该样例采用多核分块并行计算，涉及核间的数据交互：后面的block在计算rank时需要知道前面的block命中元素的数量。因此所有的kernel通过两次VF函数调用实现，第一个VF函数用来统计block_counts及相关数据，第二个VF函数用来计算output。

### Case 0：使用UB缓存线程命中状态

**优化目标**：观察各线程通过串行累加计算rank时的耗时，建立性能基线。

**核心实现**：第一个VF函数在核内共享内存UB中申请一个数组（thread_selected_flags）用来缓存所有线程的命中状态（命中写1，否则写0），命中元素的线程通过将之前所有的命中标志求和作为输出中的rank。如下图所示
![](./figures/case_0.drawio.png)

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
        output[block_base + tile_offsets[tile] + thread_prefix] = value;
    }
}
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 0 |  |

**性能数据分析**：
TODO

### Case 1：分级归约代替串行累加

**优化目标**：Case 0的问题在于排在后面的线程串行累加thread_selected_flags时的成本较高，可以线程串行调整为分级归约，从而降低读取UB的成本。

**核心实现**：分warp和线程两级归约，分别统计每个warp各自命中元素的数量，以及命中线程元素在当前warp内的位置，则rank等于前面所有warp命中元素之和加上在当前warp内的位置。如下图所示:
![](./figures/case_1.drawio.png)
其中`thread_selected_flags`和Case 1一致，`warp_selected_counts`和`warp_selected_offsets`均位于UB上，方便核内线程共享。

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
        uint32_t stable_rank = block_base + tile_offsets[tile] + warp_bases[tile * WARP_COUNT + warp_index] +
                                lane_offset;
        output[stable_rank] = value;
    }
}
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 1 | 83.921 |

**性能数据分析**：
Case 1相比于Case 0串行扫描范围从1024线程缩小到32lane，一方面计算量减少，另一方面Case 1单词循环内thread_selected_flags的访问次数约为1024*(1024-1)/2 ≈ 52 万次，Case 2单词循环内thread_selected_flags的访问次数约为1024*32+ 2\*(32-1)/2 ≈ 3.3 万次，UB访问量大大降低，两者结合带来了性能的提升。

### Case 2：使用warp接口替代显式遍历

**优化目标**：进一步消除Case 2中针对thread_selected_flags的UB访问和循环遍历。

**核心实现**：使用[asc_ballot](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_vote_functions/asc_ballot.md)将一个warp内线程的命中状态组成掩码selected_mask，再用[__popc](../../../../docs/zh/api/SIMT-API/math_functions/integer_math_functions/__popc.md)搭配[lanemask_lt](../../../../docs/zh/api/SIMT-API/Warp_functions/lane_id_functions/lanemask_lt.md)计算得到warp_selected_count及lane_offset。

**关键代码**：

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t idx = tile * THREAD_COUNT + thread_index;
    uint32_t hit = is_selected_device(input[idx]) ? 1U : 0U;
    uint32_t warp_selected_count = (uint32_t)asc_reduce_add(hit);
    if (lane_index == 0U) {
        tile_warp_offsets[tile * WARP_COUNT + warp_index] = warp_selected_count;
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
        output[block_base + tile_warp_offsets[tile * WARP_COUNT + warp_index] + lane_offset] = value;
    }
}
```

**性能数据**：
| 实现 | Task Duration(μs) |
|:---|---:|
| Case 2 | XXX |

**性能数据分析**：
Case 2利用这些 warp 接口可以直接在寄存器层面交换不同线程间的数据，不仅比访问UB更快，也不需要维护和访问thread_selected_flags，相比Case 2进一步减少了计算耗时。

### Case 3：使用短向量

**优化目标**：提升读取的带宽，降低数据搬运耗时。

**核心实现**：之前是每个线程一次处理一个元素，需要多次循环才能处理完毕。现在在一次循环中，每个线程处理连续的4个元素，进一步提升UB访问的性能。同时在第一个VF函数中按[tile, warp]计算好每个warp的偏移量放在UB中，方便在第二个VF函数中快速计算rank。如下图所示：
![](./figures/case_3.drawio.png)

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
| Case 3 | TODO |

**性能数据分析**：
TODO

## 性能对比总结

下表给出各个Case处理的

| Case | 实现方式 | Task Duration(μs) | 耗时相对于Case 0 |
|:---|:---|---:|---:|
| 1 | 使用UB缓存线程命中状态 | TODO | 1X |
| 2 | 分级归约代替串行累加 | TODO | TODO |
| 3 | 使用Warp原语替代显式遍历 | TODO | TODO |
| 4 | 使用短向量 | TODO | TODO |

## 调优建议

- 线程间协作优先使用warp原语在寄存器层面交换数据，而非通过UB数组加显式循环。
- 短向量
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
  | `SCENARIO_NUM`            | `0`-`3`  | 样例类型，默认为 3          |

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