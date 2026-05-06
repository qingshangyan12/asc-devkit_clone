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

## 样例实现与性能分析

本章按照数据搬运优化中最常见的几个问题展开：先说明性能指标含义，再分别比较分块粒度、尾块、L2Cache复用和多核同地址访问冲突。每个优化点都包含实现方式、对比方式、性能数据和结论，便于从代码行为对应到性能变化。

### 核心特性总览

| 优化点 | 主要观察对象 | 对比方法 |
|:---|:---|:---|
| 分块粒度 | 单次DataCopy搬运大小对MTE2效率的影响 | 保持矩阵规模不变，调整`TILE_M/TILE_N` |
| 尾块搬运 | 非整除shape对搬运开销的影响 | 保持分块大小不变，将`N=12288`改为`N=12287` |
| L2Cache复用 | 重复访问相同GM数据时的cache收益 | 比较整块重复搬运和N方向分片后重复搬运 |
| 同地址冲突规避 | 多核同时访问相同GM地址段的冲突影响 | 比较所有核同序访问和按核错开访问 |

### 性能指标说明

**表1 MTE2 性能指标字段说明表**

| 字段名 | 字段含义 |
|:---|:---|
|Task Duration(us)|Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。|
|*_total_cycles|Task执行总cycle数。|
|*_mte2_time(us)|mte2类型指令（DDR->AICORE搬运类指令）耗时，单位us。|
|*_mte2_ratio|mte2类型指令（DDR->AICORE搬运类指令）的cycle数在total cycle数中的占用比。|

**表2 L2Cache 性能指标字段说明表**

| 字段名 | 字段含义 |
|:---|:---|
|Task Duration(us)|Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。|
|*_total_cycles|Task执行总cycle数。|
|*_time(us)|Task在对应AI Core上的理论执行时间，单位us。|
|*_write_cache_hit|写cache命中的次数。Atlas 350加速卡不支持该字段。|
|*_write_cache_miss_allocate|写cache缺失后重新分配缓存的次数。Atlas 350加速卡不支持该字段。|
|*_r*_read_cache_hit|读r*通道cache命中次数，r0/r1为硬件读通道，分析总读命中次数时需将两个通道累加。Atlas 350加速卡不支持该字段。|
|*_r*_read_cache_miss_allocate|读r*通道cache缺失后重新分配的次数，r0/r1为硬件读通道，分析总读缺失次数时需将两个通道累加。Atlas 350加速卡不支持该字段。|
|*_read_local_l2_hit|读Cache命中的次数。仅Atlas 350加速卡支持该字段。|
|*_read_local_l2_miss|读Cache缺失次数。仅Atlas 350加速卡支持该字段。|
|*_read_local_l2_victim|读Cache未命中并触发Cache中数据被换出的次数。仅Atlas 350加速卡支持该字段。|
|*_write_local_l2_hit|写Cache命中的次数。仅Atlas 350加速卡支持该字段。|
|*_write_local_l2_miss|写Cache缺失次数。仅Atlas 350加速卡支持该字段。|
|*_write_local_l2_victim|写Cache未命中并触发Cache中数据被换出的次数。仅Atlas 350加速卡支持该字段。|

### 优化点1：分块粒度对搬运效率的影响

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
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(us)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="3">A2</td><td>1</td><td>M=12288, N=12288, TILE=1/64, Block Num=48</td><td>564.84</td><td>49222116</td><td>548.161</td><td>0.989</td><td>小分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/64, Block Num=48</td><td>233.54</td><td>20167322</td><td>220.772</td><td>0.972</td><td>中等分块搬运</td></tr>
<tr><td>3</td><td>M=12288, N=12288, TILE=64/1024, Block Num=48</td><td>215.82</td><td>18563430</td><td>202.657</td><td>0.969</td><td>大分块搬运</td></tr>
<tr><td rowspan="3">A5</td><td>1</td><td>M=12288, N=12288, TILE=1/64, Block Num=64</td><td>884.06</td><td>91585425</td><td>881.13</td><td>1</td><td>小分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/64, Block Num=64</td><td>208.64</td><td>31489587</td><td>205.99</td><td>0.99</td><td>中等分块搬运</td></tr>
<tr><td>3</td><td>M=12288, N=12288, TILE=64/1024, Block Num=64</td><td>188.52</td><td>19710997</td><td>185.66</td><td>0.99</td><td>大分块搬运</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aic_total_cycles</th><th>aic_mte2_time(us)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>1</td><td>M=12288, N=12288, TILE=64/64, Block Num=24</td><td>300.7</td><td>12870910</td><td>283.508</td><td>0.978</td><td>中等分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/256, Block Num=24</td><td>230.24</td><td>9795512</td><td>214.519</td><td>0.972</td><td>大分块搬运</td></tr>
<tr><td rowspan="2">A5</td><td>1</td><td>M=12288, N=12288, TILE=64/64, Block Num=32</td><td>245.1</td><td>12480180</td><td>241.56</td><td>0.99</td><td>中等分块搬运</td></tr>
<tr><td>2</td><td>M=12288, N=12288, TILE=64/256, Block Num=32</td><td>190.52</td><td>9853588</td><td>187.19</td><td>0.99</td><td>大分块搬运</td></tr>
</table>

**优化效果分析**：
- 增大分块可以显著降低搬运指令发射开销。GM 到 UB 场景中，大分块相对小分块在A2上端到端耗时下降约 61.8%，在A5上下降约 78.7%。
- GM 到 L1 场景中，增大 `TILE_N` 后A2/A5端到端耗时均下降约 22% 以上，说明减少搬运粒度过小带来的循环与指令开销后，MTE2搬运效率更高。

**MTE2 带宽理论分析**：

本组基础场景只做单次GM读入，不考虑L2Cache复用收益。输入矩阵为 `M=12288, N=12288`，数据类型为half，读入数据总量为：

$$读入数据总量 = M \times N \times sizeof(half) = 12288 \times 12288 \times 2B = 288MB$$

按HBM峰值带宽粗略估算MTE2理论耗时：

$$MTE2理论耗时 = \frac{读入数据总量}{HBM峰值带宽}$$

Atlas A2 训练系列产品按HBM带宽约 1.8TB/s 估算，理论耗时约 167.77us。大分块场景下，GM到UB的 `aiv_mte2_time` 为 202.657us，GM到L1的 `aic_mte2_time` 为 214.519us，实测相对理论值分别高约 20.8% 和 27.9%。

Ascend 950PR 芯片按HBM带宽约 1.6TB/s 估算，理论耗时约 188.74us。大分块场景下，GM到UB的 `aiv_mte2_time` 为 185.66us，GM到L1的 `aic_mte2_time` 为 187.19us，实测与理论估算基本接近。该估算只用于判断搬运效率量级，实际性能会受到指令发射、地址连续性、DataCopyPad/ND2NZ处理等因素影响。

### 优化点2：尾块搬运影响

**实现方式**：尾块场景复用与对齐场景相同的搬运流程，仅改变矩阵 `N` 维，使最后一个搬运块无法按完整列宽覆盖。

**对比方式**：保持分块大小不变，将 `N` 从 `12288` 改为 `12287`，观察尾块处理对端到端耗时和MTE2耗时的影响。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(us)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>3</td><td>M=12288, N=12288, TILE=64/1024, Block Num=48</td><td>215.82</td><td>18563430</td><td>202.657</td><td>0.969</td><td>对齐大分块</td></tr>
<tr><td>4</td><td>M=12288, N=12287, TILE=64/1024, Block Num=48</td><td>275.3</td><td>23648813</td><td>259.047</td><td>0.973</td><td>尾块大分块</td></tr>
<tr><td rowspan="2">A5</td><td>3</td><td>M=12288, N=12288, TILE=64/1024, Block Num=64</td><td>188.52</td><td>19710997</td><td>185.66</td><td>0.99</td><td>对齐大分块</td></tr>
<tr><td>4</td><td>M=12288, N=12287, TILE=64/1024, Block Num=64</td><td>192.07</td><td>19739629</td><td>189.16</td><td>0.99</td><td>尾块大分块</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aic_total_cycles</th><th>aic_mte2_time(us)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>2</td><td>M=12288, N=12288, TILE=64/256, Block Num=24</td><td>230.24</td><td>9795512</td><td>214.519</td><td>0.972</td><td>对齐大分块</td></tr>
<tr><td>3</td><td>M=12288, N=12287, TILE=64/256, Block Num=24</td><td>438.76</td><td>19020328</td><td>422.515</td><td>0.986</td><td>尾块大分块</td></tr>
<tr><td rowspan="2">A5</td><td>2</td><td>M=12288, N=12288, TILE=64/256, Block Num=32</td><td>190.52</td><td>9853588</td><td>187.19</td><td>0.99</td><td>对齐大分块</td></tr>
<tr><td>3</td><td>M=12288, N=12287, TILE=64/256, Block Num=32</td><td>206.61</td><td>10589826</td><td>202.97</td><td>0.99</td><td>尾块大分块</td></tr>
</table>

**优化效果分析**：
- 尾块会引入额外边界处理，A2上影响更明显，尤其GM到L1场景端到端耗时上升约 90.6%。
- A5上尾块影响相对较小，但仍会带来额外开销。选择分块时应优先保证主搬运维度对齐。

### 优化点3：重复搬运与L2Cache复用

**实现方式**：重复搬运场景在同一数据规模下执行多轮GM读取，使用 `msprof --ai-core=on --aic-metrics=L2Cache` 采集L2Cache读命中和miss allocate数据。

**对比方式**：比较整块矩阵沿相同路径连续搬4次，与N方向切4份后每份连续搬4次。后者在每个分片内连续重复访问，更容易观察L2Cache复用收益。

**GM 到 UB L2Cache 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aiv_total_cycles</th><th>aiv_time(us)</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>5</td><td>M=12288, N=12288, TILE=64/1024, Block Num=48</td><td>828.06</td><td>72465129</td><td>816.05</td><td>212</td><td>4718595</td><td>217</td><td>4718592</td><td>整块矩阵按相同路径连续搬4次</td></tr>
<tr><td>6</td><td>M=12288, N=12288, TILE=64/1024, Block Num=48</td><td>365.74</td><td>31484525</td><td>354.56</td><td>3539159</td><td>1179644</td><td>3539158</td><td>1179655</td><td>N方向切4份，每份连续搬4次</td></tr>
</table>

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aiv_total_cycles</th><th>aiv_time(us)</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>说明</th></tr>
<tr><td rowspan="2">A5</td><td>5</td><td>M=12288, N=12288, TILE=64/1024, Block Num=64</td><td>741.58</td><td>77700635</td><td>740.75</td><td>31346</td><td>529720</td><td>8358412</td><td>整块矩阵按相同路径连续搬4次</td></tr>
<tr><td>6</td><td>M=12288, N=12288, TILE=64/1024, Block Num=64</td><td>354.95</td><td>36964347</td><td>354.19</td><td>5943026</td><td>528797</td><td>2446732</td><td>N方向切4份，每份连续搬4次</td></tr>
</table>

**GM 到 L1 L2Cache 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aic_total_cycles</th><th>aicore_time(us)</th><th>aic_r0_read_cache_hit</th><th>aic_r0_read_cache_miss_allocate</th><th>aic_r1_read_cache_hit</th><th>aic_r1_read_cache_miss_allocate</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>4</td><td>M=12288, N=12288, TILE=64/1024, Block Num=24</td><td>899.34</td><td>38725327</td><td>872.19</td><td>305</td><td>4718591</td><td>287</td><td>4718601</td><td>整块矩阵按相同路径连续ND2NZ搬4次</td></tr>
<tr><td>5</td><td>M=12288, N=12288, TILE=64/1024, Block Num=24</td><td>414.96</td><td>17662155</td><td>397.8</td><td>3539061</td><td>1179684</td><td>3539133</td><td>1179616</td><td>N方向切4份，每份连续ND2NZ搬4次</td></tr>
</table>

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aic_total_cycles</th><th>aicore_time(us)</th><th>aic_read_local_l2_hit</th><th>aic_read_local_l2_miss</th><th>aic_read_local_l2_victim</th><th>说明</th></tr>
<tr><td rowspan="2">A5</td><td>4</td><td>M=12288, N=12288, TILE=64/1024, Block Num=32</td><td>732.809</td><td>37950511</td><td>732.11</td><td>269472</td><td>2557819</td><td>4060060</td><td>整块矩阵按相同路径连续ND2NZ搬4次</td></tr>
<tr><td>5</td><td>M=12288, N=12288, TILE=64/1024, Block Num=32</td><td>390.347</td><td>19858177</td><td>389.64</td><td>5948502</td><td>1148258</td><td>1220548</td><td>N方向切4份，每份连续ND2NZ搬4次</td></tr>
</table>

**优化效果分析**：
- N方向切分后在分片内重复访问，L2Cache复用更充分，GM到UB和GM到L1的端到端耗时在A2/A5上均有明显下降。
- A2需按R0+R1两个硬件读通道累加观察，总读cache命中明显增加，miss allocate明显降低；A5对应表现为 `*_read_local_l2_hit` 增加，miss/victim降低。

### 优化点4：多核同地址访问冲突规避

**实现方式**：same addr场景中所有核按相同 `mBlockIdx` 顺序访问输入矩阵；offset addr场景中每个核按 `(mBlockIdx + blockIdx) % numBlocks` 错开访问顺序。

**关键代码**：

```cpp
uint32_t blockGroupStart = (mBlockIdx / numBlocks) * numBlocks;
uint32_t curMBlockIdx = offsetAddr ? blockGroupStart + (mBlockIdx + blockIdx) % numBlocks : mBlockIdx;
```

**对比方式**：所有核完整载入同一输入矩阵，比较相同顺序访问与按核错开访问顺序。错开访问通过调整并行分片访问顺序，降低同一时刻多核访问同地址的冲突概率。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(us)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>7</td><td>M=6144, N=512, TILE=128/64, Block Num=48</td><td>539.42</td><td>42823246</td><td>474.717</td><td>0.984</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>8</td><td>M=6144, N=512, TILE=128/64, Block Num=48</td><td>328.88</td><td>28054500</td><td>307.38</td><td>0.973</td><td>所有核错开分片顺序完整载入</td></tr>
<tr><td rowspan="2">A5</td><td>7</td><td>M=8192, N=512, TILE=128/64, Block Num=64</td><td>342.29</td><td>33624098</td><td>339.38</td><td>0.99</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>8</td><td>M=8192, N=512, TILE=128/64, Block Num=64</td><td>335.64</td><td>35298771</td><td>333.08</td><td>0.99</td><td>所有核错开分片顺序完整载入</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(us)</th><th>aic_total_cycles</th><th>aic_mte2_time(us)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>6</td><td>M=6144, N=512, TILE=256/64, Block Num=24</td><td>278.56</td><td>10880150</td><td>240.198</td><td>0.98</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>7</td><td>M=6144, N=512, TILE=256/64, Block Num=24</td><td>221.34</td><td>9532153</td><td>209.8</td><td>0.977</td><td>所有核错开分片顺序完整载入</td></tr>
<tr><td rowspan="2">A5</td><td>6</td><td>M=8192, N=512, TILE=256/64, Block Num=32</td><td>369.99</td><td>13312673</td><td>366.24</td><td>0.99</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>7</td><td>M=8192, N=512, TILE=256/64, Block Num=32</td><td>187.35</td><td>9713703</td><td>185.03</td><td>0.99</td><td>所有核错开分片顺序完整载入</td></tr>
</table>

**优化效果分析**：
- offset addr通过错开多核访问顺序，降低同一时刻访问同一GM地址段的冲突概率。A2的UB/L1场景和A5的L1场景收益更明显。
- 从端到端耗时看，A2的UB场景下降约 39.0%，A5的L1场景下降约 49.4%；A5的UB场景收益较小。

### 优化要点总结

| 优化手段 | 核心原理 | 使用建议 |
|:---|:---|:---|
| 增大搬运分块 | 减少DataCopy指令次数和循环控制开销，提高MTE2有效搬运效率 | 在片上空间允许时，优先选择较大的`TILE_M/TILE_N` |
| 保持主维度对齐 | 避免尾块带来的边界处理和非完整搬运开销 | 设计shape或切分策略时，尽量让主搬运维度被`TILE_N`整除 |
| 分片后重复访问 | 将重复访问限制在更小的数据范围内，提高L2Cache命中机会 | 需要多次读取同一批GM数据时，优先考虑先分片再在片内重复处理 |
| 错开多核访问顺序 | 降低多个核同一时刻访问相同GM地址段的概率 | 多核读取同一大块数据时，可按`blockIdx`轮转访问分片顺序 |


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
