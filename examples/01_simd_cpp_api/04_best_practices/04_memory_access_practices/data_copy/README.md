# DataCopy 最佳实践样例
## 概述

本样例展示 Global Memory 到 UB、Global Memory 到 L1 的数据搬运实践。本样例不包含计算逻辑，重点用于观察 MTE2 搬运行为、分块粒度、非对齐数据搬运、L2Cache 复用以及同地址访问冲突规避方式对数据搬运性能的影响，对比 DataCopy/DataCopyPad 在不同访问模式下的性能表现。

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

## 样例实现与性能分析

本章按照数据搬运优化中最常见的几个问题展开：先说明性能指标含义，再分别比较分块粒度、非对齐数据、L2Cache复用和多核同地址访问冲突。每个优化点都包含实现方式、对比方式、性能数据和结论，便于从代码行为对应到性能变化。

### 核心特性总览

<table>
<tr><th>优化点</th><th>主要观察对象</th><th>对比方法</th></tr>
<tr><td>分块粒度</td><td>单次DataCopy搬运大小对MTE2效率的影响</td><td>保持矩阵规模不变，调整<code>TILE_M/TILE_N</code></td></tr>
<tr><td>非对齐数据搬运</td><td>非整除shape对搬运开销的影响</td><td>保持分块大小不变，将<code>N=12288</code>改为<code>N=12287</code></td></tr>
<tr><td>L2Cache复用</td><td>重复访问相同GM数据时的cache收益</td><td>比较整块重复搬运和N方向分片后重复搬运</td></tr>
<tr><td>同地址冲突规避</td><td>多核同时访问相同GM地址段的冲突影响</td><td>比较所有核同序访问和按核错开访问</td></tr>
</table>

### 性能指标说明

普通MTE2性能表主要观察搬运耗时和搬运指令占比，字段含义如下。

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间，算子执行时间以该参数为准</td></tr>
<tr><td>*_total_cycles</td><td>Task执行总cycle数。</td></tr>
<tr><td>*_mte2_time(μs)</td><td>mte2类型指令（DDR->AICORE搬运类指令）耗时，单位μs。</td></tr>
<tr><td>*_mte2_ratio</td><td>mte2类型指令（DDR->AICORE搬运类指令）的cycle数在total cycle数中的占用比。</td></tr>
</table>

L2Cache性能表在MTE2耗时之外，还会展示cache命中、缺失和换出相关计数，字段含义如下。

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间，算子执行时间以该参数为准</td></tr>
<tr><td>*_total_cycles</td><td>Task执行总cycle数。</td></tr>
<tr><td>*_time(μs)</td><td>Task在对应AI Core上的理论执行时间，单位μs。</td></tr>
<tr><td>*_write_cache_hit</td><td>写cache命中的次数。</td></tr>
<tr><td>*_write_cache_miss_allocate</td><td>写cache缺失后重新分配缓存的次数。</td></tr>
<tr><td>*_r*_read_cache_hit</td><td>读r*通道cache命中次数，r0/r1为硬件读通道，分析总读命中次数时需将两个通道累加。</td></tr>
<tr><td>*_r*_read_cache_miss_allocate</td><td>读r*通道cache缺失后重新分配的次数，r0/r1为硬件读通道，分析总读缺失次数时需将两个通道累加。</td></tr>
<tr><td>*_read_local_l2_hit</td><td>读Cache命中的次数。</td></tr>
<tr><td>*_read_local_l2_miss</td><td>读Cache缺失次数。</td></tr>
<tr><td>*_read_local_l2_victim</td><td>读Cache未命中并触发Cache中数据被换出的次数。</td></tr>
</table>

### 优化点1：分块粒度对搬运效率的影响

**实现方式**：参考 `data_copy_ub.h` 中 GM 到 UB 的 `DataCopyPad` 搬运，以及 `data_copy_l1.h` 中 GM 到 L1 的 `DataCopy` 搬运。通过编译期场景参数切换不同 `TILE_M/TILE_N` 组合。保持输入矩阵规模不变，仅改变 `TILE_M/TILE_N`。小分块会增加搬运指令数量，大分块减少指令发射次数，但需要更大的片上临时空间。

先看单次DataCopy搬运的形状：UB路径按ND布局直接搬入UB，L1路径在搬入L1时完成ND到NZ的布局转换。两条路径使用的参数不同，但都由 `TILE_M/TILE_N` 决定单次搬运的二维块大小。

<table>
<tr><th>路径</th><th>参数字段</th><th>本样例取值</th><th>含义</th></tr>
<tr><td rowspan="4">GM到UB</td><td><code>DataCopyParams.blockCount</code></td><td><code>tileM</code></td><td>一次搬运的行数</td></tr>
<tr><td><code>DataCopyParams.blockLen</code></td><td><code>curCols * sizeof(half)</code></td><td>每行连续搬运的字节数</td></tr>
<tr><td><code>DataCopyParams.srcStride</code></td><td><code>(n - curCols) * sizeof(half)</code></td><td>源端相邻两行之间跳过的字节数</td></tr>
<tr><td><code>DataCopyParams.dstStride</code></td><td><code>0</code></td><td>UB中连续存放，不额外跳行</td></tr>
<tr><td rowspan="4">GM到L1</td><td><code>Nd2NzParams.nValue</code></td><td><code>tileM</code></td><td>ND源矩阵中本次搬运的行数</td></tr>
<tr><td><code>Nd2NzParams.dValue</code></td><td><code>curCols</code> 或 <code>tileN</code></td><td>ND源矩阵中本次搬运的列数</td></tr>
<tr><td><code>Nd2NzParams.srcDValue</code></td><td><code>n</code></td><td>GM中原矩阵的行宽</td></tr>
<tr><td><code>Nd2NzParams.dstNzC0Stride</code></td><td><code>AlignUp(tileM, 16)</code></td><td>L1中NZ布局的C0方向跨度</td></tr>
</table>

**分核与数据载入模式**：

```text
输入矩阵 GM: [M, N]

M方向按核切分：
┌────────────── M ──────────────┐
│ core0: singleCoreM 行         │
│ core1: singleCoreM 行         │
│ ...                           │  NUM_BLOCKS 个核并行
│ last core: singleCoreM 行     │
└───────────────────────────────┘

单核内部按 tile 搬运：
singleCoreM 行
┌──── tileN ────┬──── tileN ────┬──── tileN ────┐
│ tileM 行      │ tileM 行      │ tileM 行      │
├───────────────┼───────────────┼───────────────┤
│ tileM 行      │ tileM 行      │ tileM 行      │
└───────────────┴───────────────┴───────────────┘

GM -> UB: DataCopyPad，AIV 核执行
GM -> L1: DataCopy(ND2NZ)，AIC 核执行
```

**MTE2 带宽理论分析**：

本组基础场景只做单次GM读入，不考虑L2Cache复用收益。输入矩阵为 `M=12288, N=12288`，数据类型为half，读入数据总量为：

$$读入数据总量 = M \times N \times sizeof(half) = 12288 \times 12288 \times 2B = 288MB$$

按HBM峰值带宽粗略估算MTE2理论耗时：

$$MTE2理论耗时 = \frac{288 \times 2^{20}B}{HBM峰值带宽}$$

Atlas A2 训练系列产品按HBM带宽约 1.8TB/s 估算，理论耗时约 167.77μs。大分块场景下，GM到UB的 `aiv_mte2_time` 为 202.657μs，GM到L1的 `aic_mte2_time` 为 214.519μs，实测相对理论值分别高约 20.8% 和 27.9%。

Ascend 950PR 芯片按HBM带宽约 1.6TB/s 估算，理论耗时约 188.74μs。大分块场景下，GM到UB的 `aiv_mte2_time` 为 185.66μs，GM到L1的 `aic_mte2_time` 为 187.19μs，实测与理论估算基本接近。该估算只用于判断搬运效率量级，实际性能会受到指令发射、地址连续性、DataCopyPad/ND2NZ处理等因素影响。


**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>相对基线MTE2耗时变化</th><th>说明</th></tr>
<tr><td rowspan="3">A2</td><td>1</td><td>Tensor=[12288,12288]<br>Tile=[1,64]<br>DataCopyParams={blockCount=1, blockLen=128B, srcStride=24448B}<br>Block Num=48</td><td>564.84</td><td>49222116</td><td>548.161</td><td>0.989</td><td>基线</td><td>小分块搬运</td></tr>
<tr><td>2</td><td>Tensor=[12288,12288]<br>Tile=[64,64]<br>DataCopyParams={blockCount=64, blockLen=128B, srcStride=24448B}<br>Block Num=48</td><td>233.54</td><td>20167322</td><td>220.772</td><td>0.972</td><td>-59.7%</td><td>中等分块搬运</td></tr>
<tr><td>3</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>DataCopyParams={blockCount=64, blockLen=2048B, srcStride=22528B}<br>Block Num=48</td><td>215.82</td><td>18563430</td><td>202.657</td><td>0.969</td><td>-63.0%</td><td>大分块搬运</td></tr>
<tr><td rowspan="3">A5</td><td>1</td><td>Tensor=[12288,12288]<br>Tile=[1,64]<br>DataCopyParams={blockCount=1, blockLen=128B, srcStride=24448B}<br>Block Num=64</td><td>884.06</td><td>91585425</td><td>881.13</td><td>1</td><td>基线</td><td>小分块搬运</td></tr>
<tr><td>2</td><td>Tensor=[12288,12288]<br>Tile=[64,64]<br>DataCopyParams={blockCount=64, blockLen=128B, srcStride=24448B}<br>Block Num=64</td><td>208.64</td><td>31489587</td><td>205.99</td><td>0.99</td><td>-76.6%</td><td>中等分块搬运</td></tr>
<tr><td>3</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>DataCopyParams={blockCount=64, blockLen=2048B, srcStride=22528B}<br>Block Num=64</td><td>188.52</td><td>19710997</td><td>185.66</td><td>0.99</td><td>-78.9%</td><td>大分块搬运</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aic_total_cycles</th><th>aic_mte2_time(μs)</th><th>aic_mte2_ratio</th><th>相对基线MTE2耗时变化</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>1</td><td>Tensor=[12288,12288]<br>Tile=[64,64]<br>Nd2NzParams={nValue=64, dValue=64, srcDValue=12288}<br>Block Num=24</td><td>300.7</td><td>12870910</td><td>283.508</td><td>0.978</td><td>基线</td><td>中等分块搬运</td></tr>
<tr><td>2</td><td>Tensor=[12288,12288]<br>Tile=[64,256]<br>Nd2NzParams={nValue=64, dValue=256, srcDValue=12288}<br>Block Num=24</td><td>230.24</td><td>9795512</td><td>214.519</td><td>0.972</td><td>-24.3%</td><td>大分块搬运</td></tr>
<tr><td rowspan="2">A5</td><td>1</td><td>Tensor=[12288,12288]<br>Tile=[64,64]<br>Nd2NzParams={nValue=64, dValue=64, srcDValue=12288}<br>Block Num=32</td><td>245.1</td><td>12480180</td><td>241.56</td><td>0.99</td><td>基线</td><td>中等分块搬运</td></tr>
<tr><td>2</td><td>Tensor=[12288,12288]<br>Tile=[64,256]<br>Nd2NzParams={nValue=64, dValue=256, srcDValue=12288}<br>Block Num=32</td><td>190.52</td><td>9853588</td><td>187.19</td><td>0.99</td><td>-22.5%</td><td>大分块搬运</td></tr>
</table>


**优化效果分析**：
- 增大分块可以显著降低搬运指令发射开销。GM 到 UB 场景中，大分块相对小分块在A2上端到端耗时下降约 61.8%，在A5上下降约 78.7%。
- GM 到 L1 场景中，增大 `TILE_N` 后A2/A5端到端耗时均下降约 22% 以上，说明减少搬运粒度过小带来的循环与指令开销后，MTE2搬运效率更高。
- 实际配置时建议在片上空间允许的前提下优先增大单次搬运大小，同时保持分块不超过UB/L1可用缓存。单次搬运字节数越大、`mLoopCount/nLoopCount` 越少，DataCopy指令数量和循环控制开销越低。

### 优化点2：非对齐数据搬运影响

**实现方式**：非对齐数据场景复用与对齐场景相同的搬运流程，仅改变矩阵 `N` 维，使最后一个搬运块无法按完整列宽覆盖。

非对齐时，最后一个N方向tile的 `curCols` 小于 `TILE_N`。UB路径中，`DataCopyParams.blockLen` 会从完整的 `TILE_N * sizeof(half)` 变成 `curCols * sizeof(half)`；L1路径中，`Nd2NzParams.dValue` 会从完整的 `TILE_N` 变成 `curCols`。

**对比方式**：保持分块大小不变，将 `N` 从 `12288` 改为 `12287`，观察非对齐数据处理对端到端耗时和MTE2耗时的影响。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>相对对齐场景耗时变化</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>3</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>blockLen=2048B<br>srcStride=22528B<br>Block Num=48</td><td>215.82</td><td>18563430</td><td>202.657</td><td>0.969</td><td>基线</td><td>对齐大分块</td></tr>
<tr><td>4</td><td>Tensor=[12288,12287]<br>Tile=[64,1024]<br>full/tail curCols=1024,1023<br>blockLen=2048B,2046B<br>srcStride=22526B,22528B<br>Block Num=48</td><td>275.3</td><td>23648813</td><td>259.047</td><td>0.973</td><td>+27.6%</td><td>非对齐数据大分块</td></tr>
<tr><td rowspan="2">A5</td><td>3</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>blockLen=2048B<br>srcStride=22528B<br>Block Num=64</td><td>188.52</td><td>19710997</td><td>185.66</td><td>0.99</td><td>基线</td><td>对齐大分块</td></tr>
<tr><td>4</td><td>Tensor=[12288,12287]<br>Tile=[64,1024]<br>full/tail curCols=1024,1023<br>blockLen=2048B,2046B<br>srcStride=22526B,22528B<br>Block Num=64</td><td>192.07</td><td>19739629</td><td>189.16</td><td>0.99</td><td>+1.9%</td><td>非对齐数据大分块</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aic_total_cycles</th><th>aic_mte2_time(μs)</th><th>aic_mte2_ratio</th><th>相对对齐场景耗时变化</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>2</td><td>Tensor=[12288,12288]<br>Tile=[64,256]<br>Nd2NzParams={nValue=64, dValue=256, srcDValue=12288}<br>Block Num=24</td><td>230.24</td><td>9795512</td><td>214.519</td><td>0.972</td><td>基线</td><td>对齐大分块</td></tr>
<tr><td>3</td><td>Tensor=[12288,12287]<br>Tile=[64,256]<br>Nd2NzParams={nValue=64, dValue=256(full),255(tail), srcDValue=12287}<br>Block Num=24</td><td>438.76</td><td>19020328</td><td>422.515</td><td>0.986</td><td>+90.6%</td><td>非对齐数据大分块</td></tr>
<tr><td rowspan="2">A5</td><td>2</td><td>Tensor=[12288,12288]<br>Tile=[64,256]<br>Nd2NzParams={nValue=64, dValue=256, srcDValue=12288}<br>Block Num=32</td><td>190.52</td><td>9853588</td><td>187.19</td><td>0.99</td><td>基线</td><td>对齐大分块</td></tr>
<tr><td>3</td><td>Tensor=[12288,12287]<br>Tile=[64,256]<br>Nd2NzParams={nValue=64, dValue=256(full),255(tail), srcDValue=12287}<br>Block Num=32</td><td>206.61</td><td>10589826</td><td>202.97</td><td>0.99</td><td>+8.4%</td><td>非对齐数据大分块</td></tr>
</table>

**优化效果分析**：
- 非对齐数据会引入额外边界处理，A2上影响更明显，尤其GM到L1场景端到端耗时上升约 90.6%。
- A5上非对齐数据影响相对较小，但仍会带来额外开销。选择分块时应优先保证主搬运维度对齐。
- 建议用户在设计矩阵shape和切分策略时尽量采用对齐矩阵。按本样例half数据类型计算，A2建议主搬运维度对应的连续字节数满足512B对齐；A5建议满足128B对齐。

### 优化点3：重复搬运与L2Cache复用

**实现方式**：重复搬运场景在同一数据规模下执行多轮GM读取，使用 `msprof --ai-core=on --aic-metrics=L2Cache` 采集L2Cache读命中和miss allocate数据。

整块重复搬运时，`RepeatCopy(0, n, 4)` 表示 `nStart=0`、`nCount=N`、`repeatTimes=4`，每轮都按完整N方向读取同一块数据。分片重复搬运时，先令 `quarterN=N/4`，再对每个 `splitIdx` 调用 `RepeatCopy(splitIdx * quarterN, quarterN, 4)`，即每次只在N方向的1/4分片内连续重复4次。两种写法搬运总数据量相同，但分片模式的单次工作集更小，更容易提高L2Cache命中率。

**对比方式**：比较整块矩阵沿相同路径连续搬4次，与N方向切4份后每份连续搬4次。后者在每个分片内连续重复访问，更容易观察L2Cache复用收益。

**L2Cache复用模式**：

```text
场景5(UB) / 场景4(L1)：整块矩阵连续重复搬运

GM矩阵: [M, N]
┌─────────────────────────────── N ───────────────────────────────┐
│                         全部列一次搬完                           │
└──────────────────────────────────────────────────────────────────┘

第1次读取: GM/HBM -> UB或L1
第2次读取: GM/HBM -> UB或L1   工作集为整块矩阵，L2Cache难以完整保留
第3次读取: GM/HBM -> UB或L1
第4次读取: GM/HBM -> UB或L1


场景6(UB) / 场景5(L1)：N方向切4份后，每份连续重复搬运

GM矩阵: [M, N]
┌──────── N/4 ────────┬──────── N/4 ────────┬──────── N/4 ────────┬──────── N/4 ────────┐
│       分片0          │       分片1          │       分片2          │       分片3          │
└─────────────────────┴─────────────────────┴─────────────────────┴─────────────────────┘

每个分片内:
第1次读取: GM/HBM -> UB或L1
第2次读取: L2Cache -> UB或L1
第3次读取: L2Cache -> UB或L1
第4次读取: L2Cache -> UB或L1
```

**L2Cache 理论性能分析**：

本组场景输入矩阵为 `M=12288, N=12288`，数据类型为half，单次完整读入数据量为：

$$单次读入数据量 = M \times N \times sizeof(half) = 12288 \times 12288 \times 2B = 288MB$$

整块矩阵连续搬4次时，总读入数据量为：

$$整块重复读入数据量 = 288MB \times 4 = 1152MB$$

该访问方式单次工作集为288MB，难以完整保留在L2Cache中，因此可近似按主要从HBM读取估算：

$$整块重复理论耗时 = \frac{1152 \times 2^{20}B}{HBM带宽}$$

N方向切4份后，每份数据量为72MB。每份连续搬4次时，理想情况下第一次从HBM读入，后3次从L2Cache读取：

$$HBM读入数据量 = 72MB \times 4 = 288MB$$

$$L2Cache读入数据量 = 72MB \times 3 \times 4 = 864MB$$

$$分片重复理论耗时 = \frac{288 \times 2^{20}B}{HBM带宽} + \frac{864 \times 2^{20}B}{L2Cache带宽}$$

Atlas A2训练系列产品按HBM带宽约1.8TB/s、L2Cache峰值带宽约5TB/s估算；Ascend 950PR芯片按HBM带宽约1.6TB/s、L2Cache峰值带宽约5TB/s估算。

<table>
<tr><th>架构</th><th>搬运方式</th><th>理论耗时(μs)</th><th>GM到UB实测核心耗时(μs)</th><th>GM到L1实测核心耗时(μs)</th></tr>
<tr><td>A2</td><td>整块重复搬运</td><td>671.09</td><td>816.05</td><td>872.19</td></tr>
<tr><td>A2</td><td>N方向分片重复搬运</td><td>348.97</td><td>354.56</td><td>397.8</td></tr>
<tr><td>A5</td><td>整块重复搬运</td><td>754.97</td><td>740.75</td><td>732.11</td></tr>
<tr><td>A5</td><td>N方向分片重复搬运</td><td>369.94</td><td>354.19</td><td>389.64</td></tr>
</table>

从理论模型和实测结果看，N方向分片后重复搬运的耗时更接近“首次HBM + 后续L2Cache”的理想模型；整块重复搬运由于工作集更大，L2Cache复用不足，耗时更接近多次从HBM读取。GM到L1场景包含ND2NZ搬运，实际耗时还会受到格式转换和L1写入布局影响，因此通常会比纯GM到UB搬运更高。

**GM 到 UB L2Cache 性能数据**：

A2和A5的profiler输出字段不同，命中率计算方式也不同：A2使用 `l2cache_hit_ratio = (r0_hit + r1_hit) / (r0_hit + r1_hit + r0_miss_allocate + r1_miss_allocate)`；A5使用 `l2cache_hit_ratio = hit / (hit + miss + victim)`。

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_time(μs)</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>说明</th><th>L2Cache命中率</th></tr>
<tr><td rowspan="2">A2</td><td>5</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=48</td><td>828.06</td><td>72465129</td><td>816.05</td><td>212</td><td>4718595</td><td>217</td><td>4718592</td><td>整块矩阵按相同路径连续搬4次</td><td>0.005%</td></tr>
<tr><td>6</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=48</td><td>365.74</td><td>31484525</td><td>354.56</td><td>3539159</td><td>1179644</td><td>3539158</td><td>1179655</td><td>N方向切4份，每份连续搬4次</td><td>75.00%</td></tr>
</table>

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_time(μs)</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>说明</th><th>L2Cache命中率</th></tr>
<tr><td rowspan="2">A5</td><td>5</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=64</td><td>741.58</td><td>77700635</td><td>740.75</td><td>31346</td><td>529720</td><td>8358412</td><td>整块矩阵按相同路径连续搬4次</td><td>0.35%</td></tr>
<tr><td>6</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=64</td><td>354.95</td><td>36964347</td><td>354.19</td><td>5943026</td><td>528797</td><td>2446732</td><td>N方向切4份，每份连续搬4次</td><td>66.64%</td></tr>
</table>

**GM 到 L1 L2Cache 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aic_total_cycles</th><th>aicore_time(μs)</th><th>aic_r0_read_cache_hit</th><th>aic_r0_read_cache_miss_allocate</th><th>aic_r1_read_cache_hit</th><th>aic_r1_read_cache_miss_allocate</th><th>说明</th><th>L2Cache命中率</th></tr>
<tr><td rowspan="2">A2</td><td>4</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=24</td><td>899.34</td><td>38725327</td><td>872.19</td><td>305</td><td>4718591</td><td>287</td><td>4718601</td><td>整块矩阵按相同路径连续ND2NZ搬4次</td><td>0.006%</td></tr>
<tr><td>5</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=24</td><td>414.96</td><td>17662155</td><td>397.8</td><td>3539061</td><td>1179684</td><td>3539133</td><td>1179616</td><td>N方向切4份，每份连续ND2NZ搬4次</td><td>75.00%</td></tr>
</table>

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aic_total_cycles</th><th>aicore_time(μs)</th><th>aic_read_local_l2_hit</th><th>aic_read_local_l2_miss</th><th>aic_read_local_l2_victim</th><th>说明</th><th>L2Cache命中率</th></tr>
<tr><td rowspan="2">A5</td><td>4</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=32</td><td>732.809</td><td>37950511</td><td>732.11</td><td>269472</td><td>2557819</td><td>4060060</td><td>整块矩阵按相同路径连续ND2NZ搬4次</td><td>3.91%</td></tr>
<tr><td>5</td><td>Tensor=[12288,12288]<br>Tile=[64,1024]<br>Block Num=32</td><td>390.347</td><td>19858177</td><td>389.64</td><td>5948502</td><td>1148258</td><td>1220548</td><td>N方向切4份，每份连续ND2NZ搬4次</td><td>71.52%</td></tr>
</table>

**优化效果分析**：
- N方向切分后在分片内重复访问，L2Cache复用更充分，GM到UB和GM到L1的端到端耗时在A2/A5上均有明显下降。
- A2需按R0+R1两个硬件读通道累加观察，总读cache命中明显增加，miss allocate明显降低；A5对应表现为 `*_read_local_l2_hit` 增加，miss/victim降低。

### 优化点4：多核同地址访问冲突规避

**实现方式**：same addr场景中所有核按相同 `mBlockIdx` 顺序访问输入矩阵；offset addr场景中每个核按 `(mBlockIdx + blockIdx) % numBlocks` 错开访问顺序。

该优化点不改变单次DataCopy的搬运形状，只改变不同核访问GM分片的顺序。same addr模式下，所有核在同一个 `mBlockIdx` 上同步访问相同地址段；offset addr模式下，`curMBlockIdx` 会按 `blockIdx` 在每组 `numBlocks` 个M块内轮转，降低同一时刻多核访问同一GM地址段的概率。

**关键代码**：

```cpp
uint32_t blockGroupStart = (mBlockIdx / numBlocks) * numBlocks;
uint32_t curMBlockIdx = offsetAddr ? blockGroupStart + (mBlockIdx + blockIdx) % numBlocks : mBlockIdx;
```

**对比方式**：所有核完整载入同一输入矩阵，比较相同顺序访问与按核错开访问顺序。错开访问通过调整并行分片访问顺序，降低同一时刻多核访问同地址的冲突概率。

**GM 到 UB 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>7</td><td>Tensor=[6144,512]<br>Tile=[128,64]<br>Block Num=48</td><td>539.42</td><td>42823246</td><td>474.717</td><td>0.984</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>8</td><td>Tensor=[6144,512]<br>Tile=[128,64]<br>Block Num=48</td><td>328.88</td><td>28054500</td><td>307.38</td><td>0.973</td><td>所有核错开分片顺序完整载入</td></tr>
<tr><td rowspan="2">A5</td><td>7</td><td>Tensor=[8192,512]<br>Tile=[128,64]<br>Block Num=64</td><td>342.29</td><td>33624098</td><td>339.38</td><td>0.99</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>8</td><td>Tensor=[8192,512]<br>Tile=[128,64]<br>Block Num=64</td><td>335.64</td><td>35298771</td><td>333.08</td><td>0.99</td><td>所有核错开分片顺序完整载入</td></tr>
</table>

**GM 到 L1 性能数据**：

<table>
<tr><th>架构</th><th>Scenario</th><th>配置</th><th>Task Duration(μs)</th><th>aic_total_cycles</th><th>aic_mte2_time(μs)</th><th>aic_mte2_ratio</th><th>说明</th></tr>
<tr><td rowspan="2">A2</td><td>6</td><td>Tensor=[6144,512]<br>Tile=[256,64]<br>Block Num=24</td><td>278.56</td><td>10880150</td><td>240.198</td><td>0.98</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>7</td><td>Tensor=[6144,512]<br>Tile=[256,64]<br>Block Num=24</td><td>221.34</td><td>9532153</td><td>209.8</td><td>0.977</td><td>所有核错开分片顺序完整载入</td></tr>
<tr><td rowspan="2">A5</td><td>6</td><td>Tensor=[8192,512]<br>Tile=[256,64]<br>Block Num=32</td><td>369.99</td><td>13312673</td><td>366.24</td><td>0.99</td><td>所有核按相同顺序完整载入</td></tr>
<tr><td>7</td><td>Tensor=[8192,512]<br>Tile=[256,64]<br>Block Num=32</td><td>187.35</td><td>9713703</td><td>185.03</td><td>0.99</td><td>所有核错开分片顺序完整载入</td></tr>
</table>

**优化效果分析**：
- offset addr通过错开多核访问顺序，降低同一时刻访问同一GM地址段的冲突概率。A2的UB/L1场景和A5的L1场景收益更明显。
- 从端到端耗时看，A2的UB场景下降约 39.0%，A5的L1场景下降约 49.4%；A5的UB场景收益较小。

### 优化要点总结

| 优化手段 | 核心原理 | 使用建议 |
|:---|:---|:---|
| 增大搬运分块 | 减少DataCopy指令次数和循环控制开销，提高MTE2有效搬运效率 | 在片上空间允许时，优先选择较大的`TILE_M/TILE_N` |
| 保持主维度对齐 | 避免非对齐数据带来的边界处理和非完整搬运开销 | 设计shape或切分策略时，尽量让主搬运维度被`TILE_N`整除，并使连续搬运字节数满足A2 512B、A5 128B对齐 |
| 分片后重复访问 | 将重复访问限制在更小的数据范围内，提高L2Cache命中机会 | 需要多次读取同一批GM数据时，优先考虑先分片再在片内重复处理 |
| 错开多核访问顺序 | 降低多个核同一时刻访问相同GM地址段的概率 | 多核读取同一大块数据时，可按`blockIdx`轮转访问分片顺序 |


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

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
  | `SCENARIO_NUM` | 场景编号 | `COPY_DST=UB`: 1-8; `COPY_DST=L1`: 1-7 | 1 |
  | `COPY_DST` | 搬运目标 | `UB`、`L1` | `UB` |
  | `CMAKE_ASC_RUN_MODE` | 运行模式 | `npu`、`sim` | `npu` |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

- 性能采集

  使用 `msprof` 工具获取详细性能数据；L2Cache 相关场景使用 `--aic-metrics=L2Cache`。

  ```bash
  msprof ./demo
  msprof --ai-core=on --aic-metrics=L2Cache ./demo
  ```

  采集后会在当前目录生成 `PROF_` 前缀目录，性能汇总文件位于 `mindstudio_profiler_output` 目录下。

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
