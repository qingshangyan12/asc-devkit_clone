# 中级性能优化：CV融合-GM中转

## 场景说明

**Scenario 1：CV融合-GM中转**（A2/A3/Ascend 950PR均支持）

AIC侧通过Fixpipe将L0C计算结果写入GM，AIV侧从GM读取数据到UB后进行GELU计算，再将结果写回GM。这种方式所有架构均支持。

数据流路径：
```
GM ──(MTE2)──> L1 ──(MTE1)──> L0A/L0B ──(Cube)──> L0C ──(Fixpipe)──> GM ──(MTE2)──> UB ──(Gelu)──> UB ──(MTE3)──> GM
```

## AIC侧实现

Fixpipe L0C → GM

```cpp
// A2/A3架构
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = curN;
fixpipeParams.mSize = curM;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = N;
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_GM>(cGM[...], cLocal, fixpipeParams);

// Ascend 950PR架构
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
fixpipeParams.mSize = curM;
fixpipeParams.nSize = curN;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = N;
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_GM>(cGM[...], cLocal, fixpipeParams);
```

## AIV侧实现

GM → xUB → GELU → geluOutUB → GM

`__mix__(1,2)` 模式下每个逻辑核有2个AIV，Fixpipe输出到GM的M行数据按M/2行拆分给两个AIV处理。每个AIV通过 `GetSubBlockIdx() % 2` 获取自己的子块索引，独立完成各自M/2行的GELU计算：

```cpp
uint32_t localSubIdx = AscendC::GetSubBlockIdx() % 2;
uint32_t gmOffset = mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * baseM / 2 * N;

// Step 1: DataCopy GM → xUB
AscendC::DataCopyPad<float>(xUB, cGM[gmOffset], copyInParams, padParams);

// Step 2: GELU计算
// A2/A3: GeluMemBaseCompute(xUB, geluOutUB, computeLen);
// Ascend 950PR:    GeluRegBaseCompute(xUB, geluOutUB, computeLen);

// Step 3: DataCopy geluOutUB → GM
AscendC::DataCopyPad<float>(cGM[gmOffset], geluOutUB, copyOutParams);
```

## 性能数据

> 以下数据基于msprof工具测量，取5次中位数。矩阵规格M=4096, K=1024, N=1536(A2)/N=1024(950PR)，输入数据类型float16，输出数据类型float32。

### Atlas A2训练系列芯片

- singleCoreM=2048, singleCoreN=1536，24核

| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|
| Gelu计算 | AI_VECTOR_CORE | 369.827 | 48 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 358.91 | 236.506 | 0.659 | 8.293 | 0.023 | 316.672 | 0.882 | 150.033 | 0.418 |
| Matmul计算 | AI_CORE | 4227.605 | 24 | 0 | 3760.55 | 3083.197 | 0.82 | 338.636 | 0.09 | 2516.081 | 0.669 | 3685.713 | 0.98 | 193.704 | 0.052 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Scenario 1 | MIX_AIC | 4236.944 | 24 | 48 | 3770.16 | 3083.235 | 0.818 | 531.971 | 0.141 | 2520.852 | 0.669 | 3687.839 | 0.978 | 193.933 | 0.051 | 3773.6 | 174.327 | 0.046 | 2.016 | 0.001 | 90.315 | 0.024 | 66.373 | 0.018 |

### Ascend 950PR芯片

- singleCoreM=2048, singleCoreN=1024，32核

| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|
| Gelu计算(RegBase) | AI_VECTOR_CORE | 348.868 | 64 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 347.99 | 66.277 | 0.19 | 3.03 | 0.009 | 320.543 | 0.921 | 314.547 | 0.904 |
| Matmul计算 | AI_CORE | 2601.311 | 32 | 0 | 2600.55 | 2593.264 | 0.997 | 104.582 | 0.04 | 816.91 | 0.314 | 1875.057 | 0.721 | 252.163 | 0.097 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Scenario 1 | MIX_AIC | 2606.91 | 32 | 64 | 2605.9 | 2597.856 | 0.997 | 105.131 | 0.04 | 818.781 | 0.314 | 1917.737 | 0.736 | 252.231 | 0.097 | 2605.93 | 59.897 | 0.023 | 1.783 | 0.001 | 57.145 | 0.022 | 50.516 | 0.019 |

### 性能数据解读

以Ascend 950PR芯片数据为例：
- 单独Gelu任务耗时348.868μs，单独Matmul任务耗时2601.311μs
- Scenario 1融合后总耗时2606.91μs，与Matmul单独执行时间基本一致

这表明采用CV融合方式编写时，AIV的Vector计算时间基本被AIC的Cube计算时间完全覆盖。由于Cube和Vector可以并行执行，整体任务执行时间不是简单的C+V叠加，而是基本等于Cube计算时间（即Matmul时间），实现了高效的流水并行。

下图展示了Scenario 1（CV融合-GM中转）场景下AIC和AIV的流水并行执行过程。AIC侧完成Matmul计算后通过Fixpipe将结果写入GM，AIV侧从GM读取数据完成GELU计算，两者按(baseM × baseN)块粒度流水并行：

![Scenario1 CV融合-GM中转流水并行](figures/CVParallell_L0C_GM_UB.png)
> 💡 使用 msProf 工具需安装 CANN 商用/社区版，详细信息可参考[msOpProf工具安装指南](https://www.hiascend.com/document/detail/zh/canncommercial/900/devaids/optool/docs/zh/install_guide/msopprof_install_guide.md)。流水图基于msprof工具上板获取，获取命令：`msprof op --aic-metrics=PipeTimeline ./demo`

**MTE2时间显著减少的原因：**

对比数据可以发现：
- 单独Gelu计算的aiv_mte2_time为320.543μs
- Scenario 1中aiv_mte2_time仅为57.145μs，减少约82.2%

原因分析：虽然Scenario 1中AIV也需要从GM将数据读取到UB，但AIC的Fixpipe将Matmul计算结果写入GM后，数据会被缓存在L2 Cache中。当AIV发起MTE2操作从GM读取数据时，由于数据已在L2 Cache中命中，实际是从L2 Cache读取而非从GM读取，因此MTE2时间显著减少。这也是CV融合场景下的一个隐式性能优化点。
