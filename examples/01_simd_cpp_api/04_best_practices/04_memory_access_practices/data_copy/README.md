# DataCopy 最佳实践样例
## 概述

本样例展示 Global Memory 到 UB、Global Memory 到 L1 的数据搬运实践。本样例不包含计算逻辑，重点用于观察 MTE2 搬运行为、分块粒度、尾块搬运、L2Cache 复用以及同地址访问冲突规避方式对数据搬运性能的影响，对比 DataCopy/DataCopyPad 在不同访问模式下的性能表现。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── data_copy
│   ├── scripts
│   │   └── gen_data.py        // 输入数据生成脚本
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── data_copy.asc          // Ascend C 样例入口与 Kernel 调用
│   ├── data_copy_l1.h         // GM 到 L1 搬运实现
│   ├── data_copy_ub.h         // GM 到 UB 搬运实现
│   └── data_utils.h           // 数据读入写出函数
```

## 样例描述

本样例输入为half类型二维矩阵，格式为ND。通过编译选项 `COPY_DST` 选择目标存储位置，通过 `SCENARIO_NUM` 选择不同搬运场景。

- `COPY_DST=UB`：使用AIV核执行GM到UB搬运，Kernel名为`kernel_data_copy_pad_gm2ub`
- `COPY_DST=L1`：使用AIC核执行GM到L1搬运，Kernel名为`kernel_data_copy_gm2l1`

### 数据流路径

```
GM ──(MTE2, DataCopyPad)──> UB
GM ──(MTE2, DataCopy)────> L1
```

## 样例实现

### 性能指标说明

**表1 MTE2 性能指标字段说明表**

| 字段名 | 字段含义 |
|:---|:---|
|Task Duration(μs)|Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。|
|Block Num|Task执行使用的核数。|
|aiv_mte2_time(μs)|AIV mte2类型指令（GM->UB搬运类指令）耗时。|
|aiv_mte2_ratio|AIV mte2类型指令（GM->UB搬运类指令）的cycle数在total cycle数中的占用比。|
|aic_mte2_time(μs)|AIC mte2类型指令（GM->L1搬运类指令）耗时。|
|aic_mte2_ratio|AIC mte2类型指令（GM->L1搬运类指令）的cycle数在total cycle数中的占用比。|

**表2 L2Cache 性能指标字段说明表**

| 字段名 | 字段含义 |
|:---|:---|
|Task Duration(μs)|Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。|
|Block Num|Task执行使用的核数。|
|aiv_time(μs)|Task在AI Vector Core上的理论执行时间。|
|aiv_r0_read_cache_hit|AIV R0通道读L2Cache命中次数。|
|aiv_r0_read_cache_miss_allocate|AIV R0通道读L2Cache miss allocate次数。|
|aiv_r1_read_cache_hit|AIV R1通道读L2Cache命中次数。|
|aiv_r1_read_cache_miss_allocate|AIV R1通道读L2Cache miss allocate次数。|
|aicore_time(μs)|Task在AI Core上的理论执行时间。|
|aic_r0_read_cache_hit|AIC R0通道读L2Cache命中次数。|
|aic_r0_read_cache_miss_allocate|AIC R0通道读L2Cache miss allocate次数。|
|aic_r1_read_cache_hit|AIC R1通道读L2Cache命中次数。|
|aic_r1_read_cache_miss_allocate|AIC R1通道读L2Cache miss allocate次数。|

### 1. 分块粒度对搬运效率的影响

**实现方式**：参考 `data_copy_ub.h` 中 GM 到 UB 的 `DataCopyPad` 搬运，以及 `data_copy_l1.h` 中 GM 到 L1 的 `DataCopy` 搬运。通过编译期场景参数切换不同 `TILE_M/TILE_N` 组合。

**关键代码**：

```cpp
AscendC::DataCopyParams copyInParams{
    static_cast<uint16_t>(tileM),
    static_cast<uint16_t>(curCols * sizeof(half)),
    static_cast<uint16_t>((n - curCols) * sizeof(half)),
    0};
AscendC::DataCopyPadParams padParams{false, 0, 0, 0};
AscendC::DataCopyPad(ubLocal, srcGlobal[mIdx * n + nIdx], copyInParams, padParams);
```

**对比方式**：保持输入矩阵规模不变，仅改变 `TILE_M/TILE_N`。小分块会增加搬运指令数量，大分块减少指令发射次数，但需要更大的片上临时空间。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="3">A2</td><td>1</td><td>M=12288, N=12288, TILE=1/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>小分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>中等分块搬运</td></tr>
<tr><td>3</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>大分块搬运</td></tr>
<tr><td rowspan="3">A5</td><td>1</td><td>M=12288, N=12288, TILE=1/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>小分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>中等分块搬运</td></tr>
<tr><td>3</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>大分块搬运</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aic_mte2_time(μs)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>1</td><td>M=12288, N=12288, TILE=64/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>中等分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/256</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>大分块搬运</td></tr>
<tr><td rowspan="2">A5</td><td>1</td><td>M=12288, N=12288, TILE=64/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>中等分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/256</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>大分块搬运</td></tr>
</table>

**优化效果分析**：
- GM 到 UB 重点对比 `aiv_mte2_time` 和 `aiv_mte2_ratio`，GM 到 L1 重点对比 `aic_mte2_time` 和 `aic_mte2_ratio`。
- 大分块通常减少循环和搬运指令数量，但需要结合UB/L1空间约束选择。

### 2. 尾块搬运影响

**实现方式**：尾块场景复用与对齐场景相同的搬运流程，仅改变矩阵 `N` 维，使最后一个搬运块无法按完整列宽覆盖。

**对比方式**：保持分块大小不变，将 `N` 从 `12288` 改为 `12287`，观察尾块处理对端到端耗时和MTE2耗时的影响。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>3</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>对齐大分块</td></tr>
<tr><td>4</td><td>M=12288, N=12287, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>尾块大分块</td></tr>
<tr><td rowspan="2">A5</td><td>3</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>对齐大分块</td></tr>
<tr><td>4</td><td>M=12288, N=12287, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>尾块大分块</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aic_mte2_time(μs)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>2</td><td>M=12288, N=12288, TILE=64/256</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>对齐大分块</td></tr>
<tr><td>3</td><td>M=12288, N=12287, TILE=64/256</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>尾块大分块</td></tr>
<tr><td rowspan="2">A5</td><td>2</td><td>M=12288, N=12288, TILE=64/256</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>对齐大分块</td></tr>
<tr><td>3</td><td>M=12288, N=12287, TILE=64/256</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>尾块大分块</td></tr>
</table>

**优化效果分析**：
- 尾块场景会触发边界处理路径。
- 若 `Task Duration` 或对应的 `*_mte2_time` 明显升高，说明尾块搬运带来的额外处理对当前分块策略有影响。

### 3. 重复搬运与L2Cache复用

**实现方式**：重复搬运场景在同一数据规模下执行多轮GM读取，使用 `msprof --ai-core=on --aic-metrics=L2Cache` 采集L2Cache读命中和miss allocate数据。

**对比方式**：比较整块矩阵沿相同路径连续搬4次，与N方向切4份后每份连续搬4次。后者在每个分片内连续重复访问，更容易观察L2Cache复用收益。

**GM 到 UB L2Cache 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aiv_time(μs)</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>5</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>整块矩阵按相同路径连续搬4次</td></tr>
<tr><td>6</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>N方向切4份，每份连续搬4次</td></tr>
<tr><td rowspan="2">A5</td><td>5</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>整块矩阵按相同路径连续搬4次</td></tr>
<tr><td>6</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>N方向切4份，每份连续搬4次</td></tr>
</table>

**GM 到 L1 L2Cache 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aicore_time(μs)</th><th>aic_r0_read_cache_hit</th><th>aic_r0_read_cache_miss_allocate</th><th>aic_r1_read_cache_hit</th><th>aic_r1_read_cache_miss_allocate</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>4</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>整块矩阵按相同路径连续ND2NZ搬4次</td></tr>
<tr><td>5</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>N方向切4份，每份连续ND2NZ搬4次</td></tr>
<tr><td rowspan="2">A5</td><td>4</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>整块矩阵按相同路径连续ND2NZ搬4次</td></tr>
<tr><td>5</td><td>M=12288, N=12288, TILE=64/1024</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>N方向切4份，每份连续ND2NZ搬4次</td></tr>
</table>

**优化效果分析**：
- 重点比较 `*_read_cache_hit` 与 `*_read_cache_miss_allocate`。
- 相同路径重复访问若命中次数更高、miss allocate更低，说明L2Cache复用更充分。

### 4. 多核同地址访问冲突规避

**实现方式**：same addr场景中所有核按相同 `mBlockIdx` 顺序访问输入矩阵；offset addr场景中每个核按 `(mBlockIdx + blockIdx) % numBlocks` 错开访问顺序。

**关键代码**：

```cpp
uint32_t blockGroupStart = (mBlockIdx / numBlocks) * numBlocks;
uint32_t curMBlockIdx = offsetAddr ? blockGroupStart + (mBlockIdx + blockIdx) % numBlocks : mBlockIdx;
```

**对比方式**：所有核完整载入同一输入矩阵，比较相同顺序访问与按核错开访问顺序。错开访问通过调整并行分片访问顺序，降低同一时刻多核访问同地址的冲突概率。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>7</td><td>M=6144, N=512, TILE=128/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>8</td><td>M=6144, N=512, TILE=128/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核错开分片顺序完整载入</td></tr>
<tr><td rowspan="2">A5</td><td>7</td><td>M=8192, N=512, TILE=128/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>8</td><td>M=8192, N=512, TILE=128/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核错开分片顺序完整载入</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>Block Num</th><th>aic_mte2_time(μs)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>6</td><td>M=6144, N=512, TILE=256/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>7</td><td>M=6144, N=512, TILE=256/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核错开分片顺序完整载入</td></tr>
<tr><td rowspan="2">A5</td><td>6</td><td>M=8192, N=512, TILE=256/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>7</td><td>M=8192, N=512, TILE=256/64</td><td>待补充</td><td>待补充</td><td>待补充</td><td>待补充</td><td>所有核错开分片顺序完整载入</td></tr>
</table>

**优化效果分析**：
- GM 到 UB 重点比较same addr与offset addr的 `Task Duration` 和 `aiv_mte2_time`。
- GM 到 L1 重点比较same addr与offset addr的 `Task Duration` 和 `aic_mte2_time`。
- 若错开访问后耗时下降，说明多核同地址访问冲突得到缓解。


## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  SCENARIO_NUM=1
  COPY_DST=UB
  ASC_ARCH=dav-3510
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCOPY_DST=$COPY_DST -DCMAKE_ASC_ARCHITECTURES=$ASC_ARCH ..
  make -j
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM -copyDst $COPY_DST -arch $ASC_ARCH
  ./demo
  ```

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | 场景编号 | `COPY_DST=UB`: 1-8; `COPY_DST=L1`: 1-7 | `1` |
  | `COPY_DST` | 搬运目标 | `UB`、`L1` | `UB` |
  | `CMAKE_ASC_RUN_MODE` | 运行模式 | `npu`、`sim` | `npu` |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

- 性能采集

  使用 `msprof` 工具获取详细性能数据；L2Cache 相关场景使用 `--aic-metrics=L2Cache`。

  ```bash
  msprof ./demo
  msprof --ai-core=on --aic-metrics=L2Cache ./demo
  ```

  采集后会在当前目录生成 `PROF_` 前缀目录，性能汇总文件位于 `mindstudio_profiler_output` 目录下。性能数据待补充。

  ```
  PROF_xxxx_XXXXXX
  ├── device_{id}
  ├── host
  ├── mindstudio_profiler_log
  └── mindstudio_profiler_output
      ├── msprof_*.json
      ├── op_summary_*.csv
      └── README.txt
  ```

  查看具体的性能分析结果：
  ```bash
  # 查看Task Duration 以及各项数据
  cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
  ```
