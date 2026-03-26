# Matmul性能调优样例
## 概述
本样例介绍Matmul样例的性能调优方法，通过10个递进式的优化case，展示从基础实现到高性能优化的完整调优路径。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
- Atlas 推理系列产品AI Core

## 目录结构介绍
```
├── matmul
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── matmul.h                // 所有优化case的头文件定义
│   └── matmul.asc              // Ascend C样例实现 & 调用样例
```

## 样例描述
- 样例功能：本样例在矩阵乘法场景下，基于Matmul API实现矩阵乘法运算，支持float16类型矩阵相乘得到float16类型结果，通过10个递进式的优化case展示从基础实现到高性能优化的完整调优路径，包括单核基础版本、Tiling优化、多核并行切分、MDL模式、L1Cache/L2Cache优化、常量Tiling、UnitFlag优化等多种优化手段。

- 样例规格：

  <table border="2" align="center">
  <caption>表1：样例规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="5" align="center">MatmulCustom</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
  <tr><td align="center">a</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">b</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">true</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">-</td></tr>
  </tr>
  </table>

- 计算公式：C = A * B
  - A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]
  - C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]

## 性能调优递进路径

本样例通过10个递进式的优化case，展示Matmul样例的完整性能调优路径。每个case在前一个case的基础上，引入新的优化手段，逐步提升性能。通过对比各case的性能数据，可以直观地看到不同优化手段对Task Duration、aicore_time、aic_mac_ratio、aic_scalar_ratio、aic_mte1_ratio、aic_mte2_ratio等关键指标的影响。

### 性能指标说明

| 指标 | 说明 |
|------|------|
| Task Duration(μs) | 整个任务执行的总时间，包括所有核的执行时间 |
| Block Num | 使用的核数（Block数量） |
| aicore_time(μs) | AI Core的总执行时间 |
| aic_mac_time(μs) | Cube计算单元的执行时间 |
| aic_mac_ratio | Cube计算单元的时间占比，反映计算单元利用率 |
| aic_scalar_time(μs) | Scalar标量计算单元的执行时间 |
| aic_scalar_ratio | Scalar标量计算单元的时间占比 |
| aic_mte1_time(μs) | MTE1（L1到L0A/L0B搬运）的执行时间 |
| aic_mte1_ratio | MTE1的时间占比，反映L1到L0的数据搬运压力 |
| aic_mte2_time(μs) | MTE2（GM到L1搬运）的执行时间 |
| aic_mte2_ratio | MTE2的时间占比，反映GM到L1的数据加载压力 |
| aic_fixpipe_time(μs) | FixPipe（L0C到GM搬运）的执行时间 |
| aic_fixpipe_ratio | FixPipe的时间占比，反映结果写回的访存压力 |
| Task Duration(μs) | 整个任务执行的总时间，包括所有核的执行时间 |
| Block Num | 使用的核数（Block数量） |
| aicore_time(μs) | AI Core的总执行时间 |
| aic_mac_time(μs) | Cube计算单元的执行时间 |
| aic_mac_ratio | Cube计算单元的时间占比，反映计算单元利用率 |
| aic_scalar_time(μs) | Scalar标量计算单元的执行时间 |
| aic_scalar_ratio | Scalar标量计算单元的时间占比 |
| aic_mte1_time(μs) | MTE1（L1到L0A/L0B搬运）的执行时间 |
| aic_mte1_ratio | MTE1的时间占比，反映L1到L0的数据搬运压力 |
| aic_mte2_time(μs) | MTE2（GM到L1搬运）的执行时间 |
| aic_mte2_ratio | MTE2的时间占比，反映GM到L1的数据加载压力 |
| aic_fixpipe_time(μs) | FixPipe（L0C到GM搬运）的执行时间 |
| aic_fixpipe_ratio | FixPipe的时间占比，反映结果写回的访存压力 |

### Case 0: 单核基础版本 (SINGLE_CORE_BASIC)

**样例目标**：实现基础的Matmul功能，确保计算正确性

**核心实现**：
- 使用单核计算，numBlocks=1
- 基础Tiling配置，baseM=baseN=baseK=64
- 使用Norm模板，关闭L0 DoubleBUffer、关闭L1多块缓存功能

**关键代码**：
```cpp
tilingApi.SetShape(M, N, K);
tilingApi.SetFixSplit(64, 64, 64);
tilingData.set_baseK(64);
```

**性能数据**：

<table border="2" align="center">
<caption>表2：Case 0性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>760379.06</td><td>1</td><td>760378.5</td><td>141699.459</td><td>0.186</td><td>508995.9</td><td>0.669</td><td>162104.2</td><td>0.213</td><td>583530.4</td><td>0.767</td><td>34210.83</td><td>0.045</td></tr>
</table>

**分析**：
- 单核运算时间很长(760379.06us)
- 标量运算占比66.9%，影响计算效率
- aicore计算利用率和访存利用率很低，cube利用率仅有18.6%，有很大优化空间
---

### Case 1: 单核Tiling优化 (SINGLE_CORE_TILING)

**优化目标**：优化Tiling参数，提升单核计算效率

**核心实现**：
- 使用单核计算，numBlocks=1
- 采用优化的Tiling配置（baseM=128, baseN=256, baseK=64）
- 使用Norm模板，关闭L0 DoubleBUffer、关闭L1多块缓存功能

**关键代码**：
```cpp
tilingApi.SetShape(M, N, K);
tilingApi.SetFixSplit(128, 256, 64);
```

**优化手段**：
- 优化Tiling参数，提升计算/访存比
  - 计算量：baseM × baseN × baseK = 128 × 256 × 64 = 2,097,152
  - 访存量：输入A（baseM × baseK）+ 输入B（baseK × baseN）+ 输出C（baseM × baseN）= 128 × 64 + 64 × 256 + 128 × 256 = 8,192 + 16,384 + 32,768 = 57,344
  - 计算访存比：2,097,152 / 57,344 ≈ 36.6
  - 相比Case 0的配置（64×64×64），计算访存比从16.7提升到36.6，大幅提升了数据重用率

**性能数据**：

<table border="2" align="center">
<caption>表3：Case 1性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>249703.8</td><td>1</td><td>249703.24</td><td>81477.188</td><td>0.326</td><td>63635.758</td><td>0.255</td><td>52003.704</td><td>0.208</td><td>195849.889</td><td>0.784</td><td>11309.081</td><td>0.045</td></tr>
</table>

**分析**：
- 相比Case 0，Task Duration从760379.06us降低到249703.8us，性能提升约3.05倍
- aic_mac_ratio从0.186提升到0.326，Cube计算单元利用率提升明显
- aic_scalar_ratio从0.669降低到0.255，标量计算占比大幅减少
- 后续优化方向：引入多核并行计算，充分利用多核资源提升整体吞吐量

---

### Case 2: 多核切分 2x12 (MULTI_CORE_SPLIT_2_12)

**优化目标**：引入多核并行计算，提升整体吞吐量

**核心实现**：
- 使用24核并行计算
- 切分策略：M方向2块，N方向12块
- 单核处理：对M、N均匀切分，singleM=4096，singleN=683，尾块tailN=679
- 使用Norm模板，关闭L0 DoubleBUffer、关闭L1多块缓存功能

**关键代码**：
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(4096, 683, 8192);
tilingApi.SetFixSplit(128, 256, 64);
SetUnnorm(tilingData);  // 关闭L0 DoubleBuffer和L1多块缓存
```

**优化手段**：
- 多核并行计算，将8192×8192的矩阵乘法切分到24个核上并行执行
- 切分策略：M方向2块，N方向12块，单核处理4096×683的子矩阵
- 均衡负载分配，尽量让每个核的计算量相近

**性能数据**：

<table border="2" align="center">
<caption>表4：Case 2性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>12709.62</td><td>24</td><td>12706.94</td><td>3465.529</td><td>0.273</td><td>2988.996</td><td>0.235</td><td>2260.553</td><td>0.178</td><td>10294.421</td><td>0.81</td><td>866.736</td><td>0.068</td></tr>
</table>

**分析**：
- 相比Case 1，Task Duration从249703.8us降低到12709.62us，多核并行计算显著提升了整体吞吐量
- aic_mac_ratio仅0.273，Cube计算单元利用率仍然较低
- 切分策略导致地址不对齐，影响访存效率
- 后续优化方向：优化切分策略，提升地址对齐，改善访存效率

---

### Case 3: 多核切分 4x6 (MULTI_CORE_SPLIT_4_6)

**优化目标**：优化多核切分策略

**核心实现**：
- 使用24核并行计算
- 切分策略：M方向4块，N方向6块
- 单核处理：对M、N均匀切分，singleM=2048, singleN=1536，尾块tailN=512
- 使用Norm模板，关闭L0 DoubleBUffer、关闭L1多块缓存功能

**关键代码**：
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(2048, 1536, 8192);
tilingApi.SetFixSplit(128, 256, 64);
SetUnnorm(tilingData);  // 关闭L0 DoubleBuffer和L1多块缓存
```

**优化手段**：
- 优化切分策略为4x6，M方向4块，N方向6块
- 单核处理2048×1536的子矩阵，尾块tailN=512（512字节对齐）
- 地址对齐优化：512对齐可以提升访存效率，减少内存访问开销，相比Case 2的683不对齐，512对齐的访存效率更高

**性能数据**：

<table border="2" align="center">
<caption>表5：Case 3性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>12311.96</td><td>24</td><td>10853.87</td><td>3394.884</td><td>0.313</td><td>2657</td><td>0.245</td><td>2166.824</td><td>0.2</td><td>8599.433</td><td>0.792</td><td>489.819</td><td>0.045</td></tr>
</table>

**分析**：
- 相比Case 2，Task Duration从12709.62us降低到12311.96us，性能提升约3.2%
- aic_mte1_time从2260.553us降低到2166.824us，降低了93.729us
- aic_mte2_time从10294.421us降低到8599.433us，降低了1694.988us，访存效率显著提高
- 地址对齐优化：512对齐相比683不对齐，访存效率更高，性能提升明显
- aic_mac_ratio从0.273提升到0.313，Cube计算单元利用率略有提升
- 后续优化方向：启用L0 DoubleBuffer和L1多块缓存功能，隐藏数据搬运延迟，提升流水线效率

---

### Case 4: 多核使用Norm模板 (MULTI_CORE_NORM)

**优化目标**：启用L0 DoubleBuffer和L1多块缓存功能，隐藏数据搬运延迟

**核心实现**：
- 使用24核并行计算，切分策略同上（4x6）
- 启用L0 DoubleBuffer：在计算当前数据的同时，预加载下一批数据到L0缓冲区
- 启用L1多块缓存：在L1缓存中多个基本块，提前启动MTE1流水（搬1份基本块就可以做后续运算）

**关键代码**：
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(2048, 1536, 8192);
tilingApi.SetFixSplit(128, 256, 64);
// Norm模板自动优化，不调用SetUnnorm，默认启用L0 DoubleBuffer和L1多块缓存

AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_NORM>
    matmulObj;

```

**优化手段**：
- L0 DoubleBuffer：在Cube计算单元处理当前批次数据时，MTE2的搬运可以并行进行下一批数据的准备，隐藏数据搬运延迟
- L1多块缓存：L1缓存可以同时保存多个基本块，提前启动MTE1流水
- 自动tiling优化：Norm模板根据硬件特性和数据规模自动选择tiling参数，根据Tiling调优经验，部分TCubeTiling参数值推荐如下：
  - base块推荐：(baseM, baseN, baseK): (128, 256, 64)
  - dbL0A/dbL0B=2
  - depthA1/(stepM*stepKa)=2
  - depthB1/(stepN*stepKb)=2

**性能数据**：

<table border="2" align="center">
<caption>表6：Case 4性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>9149.06</td><td>24</td><td>8145.94</td><td>3383.213</td><td>0.415</td><td>2623.722</td><td>0.322</td><td>2219.853</td><td>0.273</td><td>8115.658</td><td>0.996</td><td>389.662</td><td>0.048</td></tr>
</table>

**分析**：
- 相比Case 3，Task Duration从12311.96us降低到9149.06us，性能提升约34.6%
- aicore_time从10853.87us降低到8145.94us，说明流水线效率显著提升
- aic_mac_ratio从0.313提升到0.415，Cube计算单元利用率提升明显
- aic_mte2_ratio从0.792提升到0.996，GM到L1的数据搬运占比接近1，说明MTE2仍然是主要瓶颈
- 后续优化方向：启用MDL模式，优化GM到L1的数据搬运策略，进一步降低MTE2 bound

---

### Case 5: 多核使用MDL模板 (MULTI_CORE_MDL)

**优化目标**：使用MDL模板，开启"大包"搬运，减少MTE2循环搬运次数，降低MTE2 bound

**核心实现**：
- 使用24核并行计算，切分策略同上（4x6）
- 使用MDL模板，启用"大包"搬运功能

**关键代码**：
```cpp
AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
    matmulObj;
// MDL模板自动优化，启用大包搬运功能
```

**优化手段**：
- 启用MDL模式，支持"大包"搬运
- 大包搬运：MTE2从GM到L1的搬运不再是每次只搬运一个基本块，而是一次性搬运多个基本块到L1缓存，本例中depthA1=4，代表一次性搬入4份A矩阵基本块到L1，MTE1进行1次搬运，显著减少GM到L1的搬运次数

**性能数据**：

<table border="2" align="center">
<caption>表7：Case 5性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>5067.58</td><td>24</td><td>4505.94</td><td>3116.627</td><td>0.692</td><td>2077.537</td><td>0.461</td><td>2335.761</td><td>0.518</td><td>4083.578</td><td>0.906</td><td>45.689</td><td>0.01</td></tr>
</table>

**分析**：
- 相比Case 4，Task Duration从9149.06us降低到5067.58us，性能提升约80.5%
- aic_mac_ratio从0.415提升到0.692，Cube计算单元利用率大幅提升
- aic_mte2_time从8115.658us降低到4083.578us，降低了5032.08us，MTE2搬运时间大幅减少
- 大包搬运效果显著：通过减少MTE2搬运次数，大幅提升了整体性能
- 后续优化方向：手动调整MDL模板的L1缓存参数（depthA1、stepKa），进一步充分利用L1缓存空间

---

### Case 6: 多核MDL + L1Cache优化 (MULTI_CORE_MDL_L1CACHE)

**优化目标**：手动调整MDL模板的L1缓存参数，充分利用L1缓存空间

**核心实现**：
- 使用24核并行计算，切分策略同上（4x6）
- 使用MDL模板，启用大包搬运功能
- 手动优化L1 Cache参数（depthA1=16, stepKa=8），充分利用L1缓存空间

**关键代码**：
```cpp
AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
    matmulObj;
tilingData.set_depthA1(16);  // 增大depthA1，增加L1缓存中A矩阵的块数
tilingData.set_stepKa(8);
```

**优化手段**：
- L1 Cache参数优化：手动调整depthA1和stepKa参数，充分利用L1缓存
  - 当前配置：depthA1=16, depthB1=8, baseM=128, baseN=256, baseK=64, sizeof(half)=2
  - L1缓存占用计算：depthA1 × baseM × baseK × sizeof(half) + depthB1 × baseN × baseK × sizeof(half) = 16 × 128 × 64 × 2 + 8 × 256 × 64 × 2 = 262,144 + 262,144 = 524,288 bytes = 512 KB

**性能数据**：

<table border="2" align="center">
<caption>表8：Case 6性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>4151.66</td><td>24</td><td>3756.78</td><td>3163.693</td><td>0.842</td><td>1386.015</td><td>0.369</td><td>2573.438</td><td>0.685</td><td>3674.569</td><td>0.978</td><td>47.084</td><td>0.013</td></tr>
</table>

**分析**：
- 相比Case 5，Task Duration从5067.58us降低到4151.66us，性能提升约22.0%
- aic_mac_ratio从0.692提升到0.842，Cube计算单元利用率显著提升
- aic_mte2_time从4083.578us降低到3674.569us，降低了409.009us，MTE2搬运时间进一步减少
- aic_scalar_ratio从0.461降低到0.369，标量计算占比减少
- L1缓存优化效果显著：通过手动调整depthA1和stepKa，充分利用L1缓存空间，大幅提升了数据重用度
- 后续优化方向：使用常量Tiling，减少运行时Scalar计算开销
---

### Case 7: 多核MDL + L1Cache + 常量Tiling (MULTI_CORE_MDL_L1CACHE_CONSTANTS)

**优化目标**：使用常量Tiling，减少运行时Scalar计算开销

**核心实现**：
- 使用24核并行计算，切分策略同上（4x6）
- 使用MDL模板并优化L1 Cache参数同上
- 使能Tiling全量常量化，将编译期常量用于tiling配置

**关键代码**：
```cpp
constexpr MatmulShapeParams shapeParams = {SINGLE_M, SINGLE_N, SINGLE_K, BASE_M, BASE_N, BASE_K};

constexpr static auto CONSTANT_CFG = GetCustomConstantCFG<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE>();
AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CONSTANT_CFG> matmulObj;
// CONSTANT_CFG在编译期计算，减少运行时Scalar计算
```

**优化手段**：
- 编译期常量配置：使用constexpr关键字将tiling参数定义为编译期常量
  - 静态Tiling减少运行时开销：tiling参数在编译期就已经计算好，运行时不需要通过Scalar单元动态计算
  - 提升流水线效率：减少Scalar运行时的计算开销，可以避免流水线停顿，让Cube计算单元持续工作

**性能数据**：

<table border="2" align="center">
<caption>表9：Case 7性能数据</caption>
<tr><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>4114.4</td><td>24</td><td>3721.04</td><td>3163.843</td><td>0.85</td><td>984.525</td><td>0.265</td><td>2580.314</td><td>0.693</td><td>3635.407</td><td>0.977</td><td>45.873</td><td>0.012</td></tr>
</table>

**分析**：
- 相比Case 6，Task Duration从4151.66us降低到4114.4us，性能提升约0.9%
- aic_mac_ratio从0.842提升到0.85，Cube计算单元利用率略有提升
- aic_scalar_ratio从0.369降低到0.265，aic_scalar_time从1386.015us降低到984.525us，标量计算占比显著降低，说明常量Tiling有效减少了运行时Scalar计算
- 后续优化方向：启用UnitFlag优化，并行化计算与搬运流水

---

### Case 8: 多核MDL + L1Cache + 常量Tiling + UnitFlag (MULTI_CORE_MDL_L1CACHE_CONSTANTS_UNITFLAG)

**优化目标**：启用UnitFlag优化，并行化计算与搬运流水

**核心实现**：
- 使用24核并行计算，切分策略同上（4x6）
- 使用MDL模板并优化L1 Cache参数同上
- 使能Tiling全量常量化同上
- 启用UnitFlag，优化计算与搬运的并行度

**关键代码**：
```cpp
// 使用constexpr定义编译期常量，并启用UnitFlag
MatmulConfig mmCFG = GetMMConfig<MatmulConfigMode::CONFIG_MDL>(shapeParams);
mmCFG.enUnitFlag = true;
```

**优化手段**：
- 启用UnitFlag优化：
  - 优化计算与搬运的并行度：未开启UnitFlag功能时，AIC核的MMAD计算指令和FIXPIPE数据搬运指令是指令级别的同步，FIXPIPE指令需要等MMAD指令执行完才进行结果搬出，MMAD和FIXPIPE之间流水串行；开启UnitFlag功能后，MMAD和FIXPIPE是512B大小的细粒度同步，在一条MMAD指令执行过程中，每当完成一个512B数据结果的计算，FIXPIPE立即搬出该512B的数据，从而实现Cube计算单元和FIXPIPE搬运单元流水并行



**性能数据**：
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4051.78 | 24 | 3775.56 | 3253.61 | 0.862 | 1082.696 | 0.287 | 2662.844 | 0.705 | 3680.253 | 0.975 | 286.867 | 0.076 |

**分析**：
- 相比Case 7，Task Duration从4114.4us降低到4051.78us，性能提升约1.5%
- aic_mac_ratio从0.85提升到0.862，Cube计算单元利用率略有提升
- aic_fixpipe_ratio提高，运行时间略有降低：UnitFlag优化提升了计算与搬运的并行度
- 后续优化方向：启用L2Cache优化，进一步提升数据访问效率

---

### Case 9: 多核MDL + L1Cache + 常量Tiling + UnitFlag + L2Cache (MULTI_CORE_MDL_L1CACHE_CONSTANTS_UNITFLAG_L2CACHE)

**优化目标**：启用L2Cache优化，进一步提升数据访问效率

**核心实现**：
- 使用24核并行计算，切分策略同上（4x6）
- 使用MDL模板并优化L1 Cache参数同上
- 使能Tiling全量常量化和UnitFlag同上
- 启用L2Cache


**关键代码**：
```cpp
// 使用constexpr定义编译期常量，启用L2Cache，对A矩阵M轴进行切分
for (int i = 0; i < 2; i++) {
    matmulObj.SetTensorA(aGlobal[offsetA + i * (M >> 1) * K], IS_TRANS_A);
    matmulObj.SetTensorB(bGlobal[offsetB], IS_TRANS_B);
    if (shapes.isBias) {
        matmulObj.SetBias(biasGlobal);
    }
    matmulObj.IterateAll(cGlobal[offsetC + i * (M >> 1) * N]);
}
```

**优化手段**：
- 启用L2Cache优化：L2Cache是比L1Cache更大的片上缓存，可以缓存更多数据
  - L2Cache特点：L2Cache是AI Core共享的缓存，容量比L1Cache更大，L2Cache的纯读带宽约为GM的3到4倍，在搬入或搬出相同数据量的情况下，访问L2Cache内的数据比GM更快
  - 缓存命中优化：若数据无法命中L2Cache，导致需要访问GM进行读写，带宽利用效率较低，导致MTE2可能成为样例整个运行过程中的性能瓶颈
  - 分块计算适配L2Cache：当前L2Cache大小为192MB，A、B矩阵大小均为8192×8192×2=128MB，因此可以将A矩阵在M轴切分成2份，其中区域1与B矩阵完成完整运算，区域2再与B矩阵完成完整运算，B矩阵会始终驻留在L2Cache中

```
分块计算适配L2Cache示意图：

xxxx
计算过程：
Step 1: C1 = A1 × B  (A1从GM加载到L2Cache，B从GM加载到L2Cache并驻留)
Step 2: C2 = A2 × B  (A2从GM加载到L2Cache，B已在L2Cache中，无需重新加载)

L2Cache利用：
- B矩阵（128MB）在Step 1加载后驻留在L2Cache
- Step 2中B矩阵直接从L2Cache读取，避免GM访问
- 单次L2Cache访问带宽约为GM的3到4倍，显著提升访存效率
```

**性能数据**：
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4033.06 | 24 | 3669.19 | 3162.239 | 0.862 | 1058.095 | 0.288 | 2601.479 | 0.709 | 3536.342 | 0.964 | 281.184 | 0.077 |

**分析**：
- 相比Case 8，aicore_time从3775.56us降低到3669.19us，性能提升约2.9%
- 综合优化效果：通过Case 0到Case 9的递进优化，aicore_time从760378.5us提升到3669.19us，性能提升约207.2倍
---

## 性能对比总结

<table border="2" align="center">
<caption>表12：性能对比总结</caption>
<tr><td>Case</td><td>Task Duration(μs)</td><td>Block Num</td><td>aicore_time(μs)</td><td>aic_mac_time(μs)</td><td>aic_mac_ratio</td><td>aic_scalar_time(μs)</td><td>aic_scalar_ratio</td><td>aic_mte1_time(μs)</td><td>aic_mte1_ratio</td><td>aic_mte2_time(μs)</td><td>aic_mte2_ratio</td><td>aic_fixpipe_time(μs)</td><td>aic_fixpipe_ratio</td></tr>
<tr><td>Case 0</td><td>760379.06</td><td>1</td><td>760378.5</td><td>141699.459</td><td>0.186</td><td>508995.9</td><td>0.669</td><td>162104.2</td><td>0.213</td><td>583530.4</td><td>0.767</td><td>34210.83</td><td>0.045</td></tr>
<tr><td>Case 1</td><td>249703.8</td><td>1</td><td>249703.24</td><td>81477.188</td><td>0.326</td><td>63635.758</td><td>0.255</td><td>52003.704</td><td>0.208</td><td>195849.889</td><td>0.784</td><td>11309.081</td><td>0.045</td></tr>
<tr><td>Case 2</td><td>12709.62</td><td>24</td><td>12706.94</td><td>3465.529</td><td>0.273</td><td>2988.996</td><td>0.235</td><td>2260.553</td><td>0.178</td><td>10294.421</td><td>0.81</td><td>866.736</td><td>0.068</td></tr>
<tr><td>Case 3</td><td>12311.96</td><td>24</td><td>10853.87</td><td>3394.884</td><td>0.313</td><td>2657</td><td>0.245</td><td>2166.824</td><td>0.2</td><td>8599.433</td><td>0.792</td><td>489.819</td><td>0.045</td></tr>
<tr><td>Case 4</td><td>9149.06</td><td>24</td><td>8145.94</td><td>3383.213</td><td>0.415</td><td>2623.722</td><td>0.322</td><td>2219.853</td><td>0.273</td><td>8115.658</td><td>0.996</td><td>389.662</td><td>0.048</td></tr>
<tr><td>Case 5</td><td>5067.58</td><td>24</td><td>4505.94</td><td>3116.627</td><td>0.692</td><td>2077.537</td><td>0.461</td><td>2335.761</td><td>0.518</td><td>4083.578</td><td>0.906</td><td>45.689</td><td>0.01</td与其他>
<tr><td>Case 6</td><td>4151.66</td><td>24</td><td>3756.78</td><td>3163.693</td><td>0.842</td><td>1386.015</td><td>0.369</td><td>2573.438</td><td>0.685</td><td>3674.569</td><td>0.978</td><td>47.084</td><td>0.013</td></tr>
<tr><td>Case 7</td><td>4114.4</td><td>24</td><td>3721.04</td><td>3163.843</td><td>0.85</td><td>984.525</td><td>0.265</td><td>2580.314</td><td>0.693</td><td>3635.407</td><td>0.977</td><td>45.873</td><td>0.012</td></tr>
<tr><td>Case 8</td><td>4051.78</td><td>24</td><td>3775.56</td><td>3253.61</td><td>0.862</td><td>1082.696</td><td>0.287</td><td>2662.844</td><td>0.705</td><td>3680.253</td><td>0.975</td><td>286.867</td><td>0.076</td></tr>
<tr><td>Case 9</td><td>4033.06</td><td>24</td><td>3669.19</td><td>3162.239</td><td>0.862</td><td>1058.095</td><td>0.288</td><td>2601.479</td><td>0.709</td><td>3536.342</td><td>0.964</td><td>281.184</td><td>0.077</td></tr>
</table>

## 调优建议

1. **从小规模开始**：先使用单核基础版本验证功能正确性
2. **逐步优化**：按照case顺序逐步引入优化手段，观察性能提升
3. **关注数据局部性**：合理设置切分策略，提升Cache命中率
4. **利用MDL模式**：MDL模式提供了高度优化的实现，优先使用
5. **常量Tiling**：对于固定shape的场景，使用常量Tiling减少运行时开销
6. **UnitFlag优化**：启用UnitFlag可以提升计算单元利用率
7. **L2Cache优化**：对于大规模矩阵，合理利用L2Cache可以显著提升性能

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量
请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

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

### 样例执行
```bash
mkdir -p build && cd build;    # 创建并进入build目录
cmake ..;make -j;              # 编译工程
python3 ../scripts/gen_data.py # 生成测试输入数据

# 执行不同的优化case（通过参数指定case编号）
./demo 0   # Case 0: 单核基础版本
./demo 1   # Case 1: 单核Tiling优化
./demo 2   # Case 2: 多核切分 2x12
./demo 3   # Case 3: 多核切分 4x6
./demo 4   # Case 4: 多核规范版本
./demo 5   # Case 5: 多核MDL版本
./demo 6   # Case 6: 多核MDL + L1Cache
./demo 7   # Case 7: 多核MDL + L1Cache + 常量Tiling
./demo 8   # Case 8: 多核MDL + L1Cache + 常量Tiling + UnitFlag
./demo 9   # Case 9: 多核MDL + L1Cache + 常量Tiling + UnitFlag + L2Cache

python3 ../scripts/verify_result.py output/output.bin output/goldenver.bin # 验证输出结果是否正确
```

执行结果如下，说明精度对比成功。
```bash
test 0: Single Core Basic
test pass!
```
