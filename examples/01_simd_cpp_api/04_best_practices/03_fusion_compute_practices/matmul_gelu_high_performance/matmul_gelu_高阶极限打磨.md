# 高阶极限打磨：CV融合-UB直通

## 场景说明

**Scenario 2：CV融合-UB直通**（仅Ascend 950PR支持）

AIC侧通过Fixpipe将L0C计算结果直接写入UB，AIV侧无需从GM读取数据，直接在UB上进行GELU计算后将结果写回GM。这种方式省去了GM中转的读写开销，但仅Ascend 950PR架构支持Fixpipe从L0C到UB。

Ascend 950PR新增L0C->UB的通路，A2/A3不支持，芯片能力增强，因此在Ascend 950PR架构中可以不经过GM直接将矩阵乘结果输出到UB进行GELU计算。

数据流路径：
```
GM ──(MTE2)──> L1 ──(MTE1)──> L0A/L0B ──(Cube)──> L0C ──(Fixpipe)──> UB ──(Gelu)──> UB ──(MTE3)──> GM
```

## AIC侧实现

Fixpipe L0C → UB（通过 `dualDstCtl=0b01` 开启双目标模式，按M维度拆分给2个AIV）

```cpp
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
fixpipeParams.mSize = AscendC::DivCeil(curM, 2) * 2;
fixpipeParams.nSize = curN;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = curN;
fixpipeParams.dualDstCtl = 0b01;  // 按M维度拆分到2个AIV的UB
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_UB>(xUB, cLocal, fixpipeParams);
```

## AIV侧实现

UB → GELU → geluOutUB → GM

AIV侧无需DataCopy GM→UB，直接对xUB进行GELU计算：

```cpp
// GELU计算：xUB作为输入，geluOutUB作为输出
GeluRegBaseCompute(xUB, geluOutUB, computeLen);

// DataCopy: geluOutUB → GM
uint32_t localSubIdx = AscendC::GetSubBlockIdx() % 2;
uint32_t offset = baseM / 2 * N;
AscendC::DataCopyPad<float>(cGM[mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * offset], geluOutUB, copyParams);
```

## 性能数据

> 以下数据基于msprof工具测量，取5次中位数。矩阵规格M=4096, K=1024, N=1024，输入数据类型float16，输出数据类型float32。

### Ascend 950PR芯片

- singleCoreM=2048, singleCoreN=1024，32核

| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|
| Scenario 2 | MIX_AIC | 2573.584 | 32 | 64 | 2572.67 | 2565.251 | 0.997 | 104.669 | 0.041 | 819.934 | 0.319 | 2096.38 | 0.815 | 212.403 | 0.083 | 2572.69 | 60.438 | 0.023 | 1.138 | 0 | 0.005 | 0 | 68.76 | 0.027 |

### 性能数据解读

以Ascend 950PR芯片数据为例：
- Scenario 2融合后总耗时2573.584μs，相比Scenario 1的2606.91μs进一步减少约33μs
- Scenario 2中aiv_mte2_time仅为0.005μs，相比Scenario 1的57.145μs几乎降为零

下图展示了Scenario 2（CV融合-UB直通）场景下AIC和AIV的流水并行执行过程。AIC侧完成Matmul计算后通过Fixpipe将结果直接写入UB（不经过GM中转），AIV侧直接在UB上进行GELU计算，省去了GM→UB的MTE2搬运开销：

![Scenario2 CV融合-UB直通流水并行](figures/CVParallell_L0C_UB.png)
