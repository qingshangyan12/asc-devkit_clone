# Matmul Best Practice Example

## Overview

This example implements matrix multiplication using Matmul high-level API. It demonstrates the complete optimization path from basic implementation to high-performance optimization through 9 progressive optimization cases, including single-core basic version, Tiling optimization, multi-core parallel partitioning, MDL mode, L1Cache/L2Cache optimization, constant Tiling, UnitFlag optimization and various other optimization methods.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series/Atlas A3 Inference Series
- Atlas A2 Training Series/Atlas A2 Inference Series

## Directory Structure

```
├── matmul_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // Input data and golden data generation script
│   │   └── verify_result.py    // Golden data comparison file
│   ├── CMakeLists.txt          // Build configuration file
│   ├── data_utils.h            // Data read/write functions
│   ├── figures                 // Diagrams
│   ├── matmul.h                // Header file definitions for all optimization cases
│   └── matmul.asc              // Ascend C example implementation
```

## Example Description

- Computation Formula: C = A * B
  - A and B are source operands. A is left matrix with shape [M, K]. B is right matrix with shape [K, N]
  - C is destination operand, matrix storing matrix multiplication result with shape [M, N]

- Example Specification:

<table>
<tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="5" align="center">Matmul</td></tr>
<tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
<tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td><td align="center">false</td></tr>
<tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td><td align="center">true</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">half</td><td align="center">ND</td><td align="center">-</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="5" align="center">matmul_custom</td></tr>
</table>


## Example Implementation

### Class Implementation Explanation

This example implements different optimization strategies through three independent classes. Each class corresponds to specific Case version.

| Class Name | Corresponding Case | Implementation Feature | Kernel Function Used | Optimization Feature |
|------|---------|---------|-------------|---------|
| **MatmulKernel** | Case 0-5 | Basic implementation, runtime Tiling | matmul_custom (isMdl=false)<br>matmul_custom_mdl (isMdl=true) | - Case 0: Single-core basic version<br>- Case 1: Single-core Tiling optimization<br>- Case 2: Multi-core partitioning 2x12<br>- Case 3: Multi-core partitioning 4x6<br>- Case 4: MDL mode<br>- Case 5: L1Cache optimization in MDL mode |
| **MatmulKernelL2Cache** | Case 6 | L2Cache optimization, runtime Tiling | matmul_custom_mdl_l2cache | - MDL mode<br>- L1Cache optimization<br>- L2Cache optimization (A matrix M axis partitioning) |
| **MatmulKernelMdlL2CacheConstant** | Case 7-8 | Constant Tiling, compile-time calculation | matmul_custom_mdl_l2cache_constant (useUnitFlag=false, Case7)<br>matmul_custom_mdl_l2cache_constant_unitflag (useUnitFlag=true, Case8) | - MDL mode<br>- L1Cache optimization<br>- L2Cache optimization<br>- Constant Tiling (compile-time calculation)<br>- Case 8: UnitFlag optimization |

#### 1. Tiling Mechanism Feature

- `MatmulKernel` and `MatmulKernelL2Cache` use `TCubeTiling` type, need to copy complete Tiling data structure from GM memory. Tiling parameters are calculated by Scalar unit at runtime.
- `MatmulKernelMdlL2CacheConstant` uses custom `MatmulProblemShape` structure, only contains shape information (M, N, K, singleCoreM and so on). Tiling parameters are calculated at compile-time through `CONSTANT_CFG`, no need for Scalar calculation at runtime.

#### 2. Process Method Computation Flow Feature
- **Computation Flow**: `MatmulKernel` single iteration, `MatmulKernelL2Cache` and `MatmulKernelMdlL2CacheConstant` loop 2 times (L2Cache optimization, A matrix M axis partitioning)

---
### Performance Indicator Description

| Indicator | Description |
|------|------|
| Task Duration(μs) | Total task execution time, operator execution time is based on this parameter |
| Block Num | Number of cores used (Block count) |
| aicore_time(μs) | AI Core average execution time |
| aic_mac_time(μs) | Cube computation unit execution time |
| aic_mac_ratio | Cube computation unit time ratio, reflects computation unit utilization |
| aic_scalar_time(μs) | Scalar computation unit execution time |
| aic_scalar_ratio | Scalar computation unit time ratio |
| aic_mte1_time(μs) | MTE1 (L1 to L0A/L0B transfer) execution time |
| aic_mte1_ratio | MTE1 time ratio, reflects L1 to L0 data transfer pressure |
| aic_mte2_time(μs) | MTE2 (GM to L1 transfer) execution time |
| aic_mte2_ratio | MTE2 time ratio, reflects GM to L1 data loading pressure |
| aic_fixpipe_time(μs) | FixPipe (L0C to GM transfer) execution time |
| aic_fixpipe_ratio | FixPipe time ratio, reflects result write-back memory pressure |

---

**Note**: The following Case performance change analysis uses A2 chip (Ascend 910B1) performance data as example. For Ascend 950PR performance tuning data, refer to [below](#ascend-950pr-chip-performance-data).

### Case 0: Single-core Basic Version (SINGLE_CORE_BASIC)

**Example Objective**: Implement basic Matmul function, ensure computation correctness

**Core Implementation**:
- Use single-core computation, **numBlocks=1**
- Basic Tiling configuration, **baseM=baseN=baseK=64**. Base block is basic block participating in one matrix multiplication instruction. baseM, baseN, baseK represent M, N, K axis length on L0 during Matmul computation respectively, in element units.
- Transfer strategy: Input A and B matrices on GM are transferred to L1 by baseM×baseK and baseK×baseN blocks sequentially, then from L1 to L0A/L0B. Cube unit executes one baseM×baseN×baseK Matmul computation for each transferred base block.

**Key Code**:
```cpp
tilingApi.SetShape(M, N, K);
tilingApi.SetFixSplit(64, 64, 64);
tilingData.set_baseK(64);
```

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 759363.98 | 1 | 759363.46 | 141699.459 | 0.187 | 508787.509 | 0.67 | 162104.184 | 0.213 | 582515.828 | 0.767 | 34303.063 | 0.045 |

**Analysis**:
- Single-core computation time **759363.98μs**
- Computation unit utilization aic_mac_ratio only **18.7%**, this scenario serves only as Matmul computation performance comparison example, not recommended for user use
---

### Case 1: Single-core Tiling Optimization (SINGLE_CORE_TILING)

**Optimization Objective**: Optimize Tiling base block parameters, improve single-core computation efficiency

**Core Implementation**:
- Use single-core computation, **numBlocks=1**
- Use optimized Tiling configuration, **baseM=128, baseN=256, baseK=64**
- Transfer strategy: Input A and B matrices on GM are transferred to L1 by baseM×baseK and baseK×baseN blocks sequentially, then from L1 to L0A/L0B. Cube unit executes one baseM×baseN×baseK Matmul computation for each transferred base block.

**Key Code**:
```cpp
tilingApi.SetShape(M, N, K);
tilingApi.SetFixSplit(128, 256, 64);
```

**Optimization Method**:
- **Base Block Selection Principle**: Case0 Tiling sets base block as [baseM, baseN, baseK] = [64, 64, 64], with high **memory-computation ratio** (that is data amount needed per cycle for computation). For current large shape scenario, base block selection principle is minimum memory-computation ratio, that is minimum data amount needed for memory access under same Cube computation amount.

- **Memory-Computation Ratio Analysis**: For fp16 input type, Cube execution unit can complete 16×16×16 multiply-add operations per cycle. Setting base block as [baseM, baseN, baseK] = [128, 256, 64] can achieve minimum memory-computation ratio while satisfying 512Byte alignment for GM address during transfer. Cube computation cycle count is (128 × 64 × 256) / (16 × 16 × 16) = 512cycle, memory-computation ratio is (128 × 64 × 2 + 256 × 64 × 2) / 512cycle = **96(byte / cycle)**; Setting base block as [baseM, baseN, baseK] = [64, 64, 64], Cube computation cycle count is (64 × 64 × 64) / (16 × 16 × 16) = 64cycle, memory-computation ratio is (64 × 64 × 2 + 64 × 64 * 2) / 64cycle = **256(byte / cycle)**. [128, 256, 64] base block scheme has **lower memory-computation ratio, same computation amount needs less data, thus lower bandwidth pressure**

- 💡**Recommended Base Block Setting**: A2/A3 chip L0A size equals L0B size, both 64KB, L0C size is 128KB. [baseM, baseN, baseK] = [128, 256, 64] can maximize memory space utilization. `For b16 input data type, recommend base block [baseM, baseN, baseK] = [128, 256, 64], for b8 input data type, recommend base block [baseM, baseN, baseK] = [128, 256, 128].`



**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 249467.08 | 1 | 249466.54 | 81477.188 | 0.327 | 63608.818 | 0.255 | 52003.703 | 0.208 | 195613.432 | 0.784 | 11313.746 | 0.045 |

**Analysis**:
- Compared to Case 0, Task Duration from 759363.98μs to 249467.08μs, reduced **509896.90μs**, performance improvement **3.04x**
- MTE2 data transfer duration from 582515.828μs to 195613.432μs, reduced **66.42%**
- Next optimization direction: Introduce multi-core parallel computation, fully utilize multi-core resources to improve overall throughput

---

### Case 2: Multi-core Partitioning 2x12 (MULTI_CORE_SPLIT_2_12)

**Optimization Objective**: Introduce multi-core parallel computation, improve overall throughput

**Core Implementation**:
- Multi-core parallel computation, partition 8192×8192 matrix multiplication to 24 cores for parallel execution
- Partitioning strategy: M direction 2 blocks, N direction 12 blocks, **singleM=4096, singleN=683, tail block tailN=679**
- Transfer strategy: Input A and B matrices on GM are transferred to L1 by baseM×baseK and baseK×baseN blocks sequentially, then from L1 to L0A/L0B. Cube unit executes one baseM×baseN×baseK Matmul computation for each transferred base block.
<img src="figures/2_12_split_core.png">

**Key Code**:
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(4096, 683, 8192);
tilingApi.SetFixSplit(128, 256, 64);
SetL1(tilingData);
```

**Optimization Method**:
- Balanced load distribution,尽量让每个核的计算量相近

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 12541.22 | 24 | 12537.56 | 3462.802 | 0.276 | 2987.47 | 0.238 | 2260.551 | 0.18 | 10177.798 | 0.812 | 875.439 | 0.07 |

**Analysis**:
- Compared to Case 1, Task Duration from 249467.08μs to 12541.22μs, performance improvement **19.89x**, multi-core parallel computation significantly improves overall throughput
- aic_mte2_time duration **10177.798μs**, ratio **81.2%**, becomes performance improvement bottleneck
- Next optimization direction: Current multi-core partitioning strategy does not satisfy 512B address alignment, and does not uniformly partition M and N. Next step will optimize partitioning strategy, improve address alignment, increase memory access efficiency

---

### Case 3: Multi-core Partitioning 4x6 (MULTI_CORE_SPLIT_4_6)

**Optimization Objective**: Optimize multi-core partitioning strategy

**Core Implementation**:
- Multi-core parallel computation, partition 8192×8192 matrix multiplication to 24 cores for parallel execution
- Partitioning strategy: Uniformly partition M and N, M direction 4 blocks, N direction 6 blocks
- Transfer strategy: Input A and B matrices on GM are transferred to L1 by baseM×baseK and baseK×baseN blocks sequentially, then from L1 to L0A/L0B. Cube unit executes one baseM×baseN×baseK Matmul computation for each transferred base block.

<img src="figures/4_6_split_core.png">

**Key Code**:
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(2048, 1536, 8192);
tilingApi.SetFixSplit(128, 256, 64);
SetL1(tilingData);
```

**Optimization Method**:
- Address satisfies **512B alignment**, set **singleM=2048, singleN=1536, tail block tailN=512**, improve effective bandwidth utilization
- **Avoid Same Address Access**: Same address access means when multiple cores simultaneously read same row data of A matrix, need to access same memory address. Hardware needs to serialize multiple accesses to same address. More cores with same address access causes more serious performance degradation due to serialization. Compared to 2×12 partitioning strategy in case2, 4×6 partitioning in case3 has smaller same address conflict delay
- **When multi-core partitioning, should uniformly partition M and N while satisfying 512B address alignment.**


**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 12283.84 | 24 | 10870.96 | 3394.884 | 0.312 | 2656.33 | 0.244 | 2166.824 | 0.199 | 8616.671 | 0.793 | 488.829 | 0.045 |

**Analysis**:
- Compared to Case 2, Task Duration from 12541.22μs to 12283.84μs, reduced 2.05%.
- aic_mte2_time from 10177.798μs to 8616.671μs, reduced **15.33%**, reduction **1561.127μs**
- Next optimization direction: Current aicore duration is still mainly MTE2. Next step will enable L1 multi-block cache function, hide data transfer latency, improve MTE2 pipeline efficiency

---

### Case 4: Multi-core Using MDL Template (MULTI_CORE_MDL)

**Optimization Objective**: Use MDL template, enable L1 multi-block cache function, enable "large package" transfer, reduce MTE2 loop transfer count

**Core Implementation**:
- Use 24-core parallel computation, partitioning strategy same as above (4x6)
- Use MDL template, enable "large package" transfer function

**Key Code**:
```cpp
AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
    matmulObj;
// MDL template automatic optimization, enable large package transfer function
```

**Optimization Method**:
- Enable MDL mode, support "large package" transfer
- **Large Package Transfer**: MTE2 transfer from GM to L1 no longer transfers only one basic block at a time, but caches multiple basic blocks in L1, significantly reducing GM to L1 transfer count. In this example scenario depthA1=4 and enables L1 transfer double buffer, meaning L1 caches 4 copies of baseM × baseK data blocks, transferring two blocks into ping and pong buffers respectively during transfer.
- 💡**L1 Multi-block Cache Tuning Parameters Should Satisfy**:
  - dbL0A / dbL0B=2
  - depthA1 / (stepM * stepKa)=2,
  - depthB1 / (stepN * stepKb)=2
  - **Parameter Meaning**:
    - dbL0A and dbL0B: Represent whether MTE1 of A matrix and B matrix enables double buffer respectively (value 1 or 2, 2 means enabling double buffer)
    - depthA1 and depthB1: Represent number of fully loaded basic blocks in L1 respectively
    - stepM and stepKa: Represent multiple of baseM on M direction and multiple of baseK on Ka direction (that is A matrix K direction) for A matrix data blocks cached in L1 respectively
    - stepN and stepKb: Represent multiple of baseN on N direction and multiple of baseK on Kb direction (that is B matrix K direction) for B matrix data blocks cached in L1 respectively

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 5039.86 | 24 | 4487.89 | 3116.472 | 0.694 | 2073.97 | 0.462 | 2332.914 | 0.52 | 4051.458 | 0.903 | 44.749 | 0.01 |

**Analysis**:
- Compared to Case 3, Task Duration from 12283.84μs to 5039.86μs, duration reduced **58.98%**
- aic_mte2_time from 8616.671μs to 4051.458μs, MTE2 transfer time reduced **52.97%**, reduction **4565.213μs**
- aic_mac_ratio from **31.2%** to **69.4%**
- Next optimization direction: MDL template automatically tuned Tiling does not fully utilize L1 space. Can manually adjust Tiling parameters to further improve MTE2 transfer intensity

---

### Case 5: Multi-core MDL + L1Cache Optimization (MULTI_CORE_MDL_L1CACHE)

**Optimization Objective**: Improve MTE2 transfer intensity, fully utilize L1 cache space

**Core Implementation**:
- Use 24-core parallel computation, partitioning strategy same as above (4x6)
- Use MDL template, enable "large package" transfer function
- Manually optimize Tiling parameters to **depthA1=16, stepKa=8**, fully utilize L1 cache space

**Key Code**:
```cpp
AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
    matmulObj;
tilingData.set_depthA1(16);  // Increase depthA1, increase A matrix block count in L1 cache
tilingData.set_stepKa(8);
```

**Optimization Method**:
- **L1 Cache Tiling Parameter Optimization**: Manually adjust depthA1 and stepKa parameters, fully utilize L1 cache
  - Current configuration: depthA1=16, depthB1=8, baseM=128, baseN=256, baseK=64
  - L1 Cache Usage Calculation:

    A Matrix:
      depthA1 × baseM × baseK × sizeof(half) = 16 × 128 × 64 × 2B = 262,144B = 256 KB

    B Matrix:
      depthB1 × baseN × baseK × sizeof(half) = 8 × 256 × 64 × 2B = 262,144B = 256 KB

    Total 256 KB + 256 KB = 512 KB

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4156.76 | 24 | 3925.61 | 3352.087 | 0.854 | 1464.492 | 0.373 | 2750.454 | 0.701 | 3778.555 | 0.963 | 47.853 | 0.012 |

**Analysis**:
- Compared to Case 4, Task Duration from 5039.86μs to 4156.76μs, duration reduced **17.52%**
- aic_mte2_time from 4051.458μs to 3778.555μs, MTE2 transfer time reduced **6.47%**
- aic_mac_ratio from 69.4% to 85.4%, cube utilization improved **23.05%**
- Next optimization direction: aic_mte2_ratio reaches **96.3%**, reaching MTE2 bound, becoming performance improvement bottleneck. Next step will optimize L2Cache to relieve MTE2 bound
---

### Case 6: Multi-core MDL + L1Cache + L2Cache (MULTI_CORE_MDL_L1CACHE_L2CACHE)

**Optimization Objective**: Enable L2Cache optimization, relieve MTE2 bound

**Core Implementation**:
- Use 24-core parallel computation, partitioning strategy same as above (4x6)
- Use MDL template and optimize L1 Cache parameters same as above
- Enable `L2Cache`, partition A matrix on M axis

**Key Code**:
```cpp
for (int i = 0; i < 2; i++) {
    matmulObj.SetTensorA(aGlobal[offsetA + i * (M >> 1) * K], IS_TRANS_A);
    matmulObj.SetTensorB(bGlobal[offsetB], IS_TRANS_B);
    if (shapes.isBias) {
        matmulObj.SetBias(biasGlobal);
    }
    matmulObj.IterateAll(cGlobal[offsetC + i * (M >> 1) * N]);
}
```

**Optimization Method**:
- **Enable L2Cache Optimization**:
  - L2Cache Feature: L2Cache is AI Core shared external cache. L2Cache pure read bandwidth is approximately 3 to 4 times GM. Under same data transfer amount, accessing data in L2Cache is faster than GM
  - Cache Hit Optimization: If data cannot hit L2Cache, need to access GM for read/write, bandwidth utilization efficiency is low, causing MTE2 possibly become performance bottleneck in entire example execution process
  - **Block Computation Adapting to L2Cache**: Current L2Cache size is **192MB**, matrix computation total data amount is **384MB** (A matrix 128MB + B matrix 128MB + C matrix 128MB). Since L2Cache capacity is less than matrix computation data total amount, can partition A matrix into 2 parts on M axis. Region 1 completes full computation with B matrix, then region 2 completes full computation with B matrix. Improve L2cache hit rate through partitioning

Block computation adapting to L2Cache diagram:

<img src="figures/L2Cache.png">

```
Computation Process:
Step 1: C1 = A1 × B  (A1 loads from GM to L2Cache, B loads from GM to L2Cache and stays)
Step 2: C2 = A2 × B  (A2 loads from GM to L2Cache, B already in L2Cache, no need to reload)

L2Cache Utilization:
- B matrix (128MB) stays in L2Cache after Step 1 loading
- Step 2 B matrix reads directly from L2Cache, avoid GM access
- Single L2Cache access bandwidth is approximately 3 to 4 times GM, significantly improves memory access efficiency
```

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4088.36 | 24 | 3786.26 | 3254.31 | 0.86 | 1753.463 | 0.463 | 2680.849 | 0.708 | 3625.42 | 0.958 | 47.398 | 0.013 |

**Analysis**:
- Compared to Case 5, Task Duration from 4156.76μs to 4088.36μs, reduced **68.40μs**
- aic_mte2_time from 3778.555μs to 3625.42μs, MTE2 transfer time reduced **4.05%**
- aic_mac_ratio from **85.4%** to **86%**
- Next optimization direction: Current performance constraint is still MTE2 bound. Developers can further optimize MTE2 by optimizing L2Cache partitioning strategy. This example will next show Scalar and Fixpipe pipeline optimization

---

### Case 7: Multi-core MDL + L1Cache + L2Cache + Constants Tiling (MULTI_CORE_MDL_L1CACHE_L2CACHE_CONSTANTS)

**Optimization Objective**: Use constant Tiling, reduce runtime Scalar computation overhead

**Core Implementation**:
- Use 24-core parallel computation, partitioning strategy same as above (4x6)
- Use MDL template and optimize L1 Cache parameters same as above
- L2 Cache strategy same as above
- Enable Tiling full constantization

**Key Code**:
```cpp
constexpr MatmulShapeParams shapeParams = {SINGLE_M_L2CACHE, SINGLE_N, SINGLE_K, BASE_M, BASE_N, BASE_K};

constexpr static auto CONSTANT_CFG = GetCustomConstantCFG<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, true, true>();
AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CONSTANT_CFG> matmulObj;
// CONSTANT_CFG calculates at compile-time, reduces runtime Scalar computation
```

**Optimization Method**:
- **Tiling Constantization**:
  - AI Core hardware advantage lies in vector/matrix parallel computation, scalar operation cannot leverage hardware capability. **Matmul API has large amount of Scalar computation during initialization and iteration. Scalar computation during Matmul initialization affects instruction header overhead. Scalar computation between Matmul iterations may block MTE2 pipeline**
  - Static Tiling Reduces Runtime Overhead: Use **MatmulApiStaticTiling** parameter to replace TCubeTiling variable parameter. Move Scalar computation to compile-time. Runtime does not need dynamic calculation through Scalar unit, reducing Scalar performance overhead

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4053.44 | 24 | 3665.12 | 3163.682 | 0.863 | 968.616 | 0.264 | 2609.617 | 0.712 | 3513.806 | 0.959 | 45.76 | 0.012 |

**Analysis**:
- Compared to Case 6, Task Duration from 4088.36μs to 4053.44μs
- aic_scalar_time from 1753.463μs to 968.616μs, reduced **44.76%**
- Constant Tiling effectively reduces Scalar unit performance overhead. When example performs poorly due to Scalar blocking, users can reduce Scalar time ratio through this method
- Next optimization direction: Enable UnitFlag optimization, demonstrate parallel computation and transfer pipeline

---

### Case 8: Multi-core MDL + L1Cache + L2Cache + Constants Tiling + UnitFlag (MULTI_CORE_MDL_L1CACHE_L2CACHE_CONSTANTS_UNITFLAG)

**Optimization Objective**: Enable UnitFlag optimization, parallelize computation and transfer pipeline

**Core Implementation**:
- Use 24-core parallel computation, partitioning strategy same as above (4x6)
- Use MDL template and optimize L1 Cache parameters same as above
- L2 Cache strategy same as above
- Enable Tiling full constantization same as above
- Enable UnitFlag, optimize computation and transfer parallelism

**Key Code**:
```cpp
// Use constexpr to define compile-time constant, enable UnitFlag
MatmulConfig mmCFG = GetMMConfig<MatmulConfigMode::CONFIG_MDL>(shapeParams);
mmCFG.enUnitFlag = true;
```

**Optimization Method**:
- **Enable UnitFlag Optimization**:
  - Optimize Computation and Transfer Parallelism: When UnitFlag function is not enabled, AIC core MMAD computation instruction and FIXPIPE data transfer instruction are instruction-level synchronous. FIXPIPE instruction needs to wait for MMAD instruction execution completion before performing result transfer. MMAD and FIXPIPE pipeline are serial. After enabling UnitFlag function, MMAD and FIXPIPE are **512B size fine-grained synchronous**. During one MMAD instruction execution, whenever 512B data result computation completes, FIXPIPE immediately transfers that 512B data, achieving Cube computation unit and FIXPIPE transfer unit pipeline parallel

UnitFlag function diagram:

<img src="figures/unitflag_close.png">
<img src="figures/unitflag_open.png">



**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4012.44 | 24 | 3559.06 | 3076.396 | 0.864 | 1026.069 | 0.288 | 2533.512 | 0.712 | 3435.584 | 0.965 | 272.576 | 0.077 |

**Analysis**:
- Compared to Case 7, Task Duration from 4053.44μs to 4012.44μs
- aic_fixpipe_time from 45.76μs to 272.576μs. Reason is after enabling Unitflag, aic_fixpipe_time includes FIXPIPE instruction wait time, not actual pipeline duration. Users can focus on whether end-to-end performance improves
- Current performance benefit is small because performance constraint is still MTE2 bound. Operator MMAD and FIXPIPE pipeline are masked by MTE2 bound. When example performs poorly due to FIXPIPE blocking, users can enable UnitFlag function

---

## Performance Comparison Summary

### Atlas A2 Training Series Chip Performance Data

**Comprehensive Optimization Effect**:
- This example cube utilization improved **67.7%** (18.7% → 86.4%), already reaches **86.4%** of chip peak computing power
- Through progressive optimization from Case 0 to Case 8, example duration reduced **99.47%** (759363.98μs → 4012.44μs)

| Case version | Task Duration(μs) | End-to-end Duration Relative to Case 0 | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------|------------------|----------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| Case 0 | 759363.98 | **1x** | 1 | 759363.46 | 141699.459 | 0.187 | 508787.509 | 0.67 | 162104.184 | 0.213 | 582515.828 | 0.767 | 34303.063 | 0.045 |
| Case 1 | 249467.08 | **3.04x** | 1 | 249466.54 | 81477.188 | 0.327 | 63608.818 | 0.255 | 52003.703 | 0.208 | 195613.432 | 0.784 | 11313.746 | 0.045 |
| Case 2 | 12541.22 | **60.55x** | 24 | 12537.56 | 3462.802 | 0.276 | 2987.47 | 0.238 | 2260.551 | 0.18 | 10177.798 | 0.812 | 875.439 | 0.07 |
| Case 3 | 12283.84 | **61.82x** | 24 | 10870.96 | 3394.884 | 0.312 | 2656.33 | 0.244 | 2166.824 | 0.199 | 8616.671 | 0.793 | 488.829 | 0.045 |
| Case 4 | 5039.86 | **150.67x** | 24 | 4487.89 | 3116.472 | 0.694 | 2073.97 | 0.462 | 2332.914 | 0.52 | 4051.458 | 0.903 | 44.749 | 0.01 |
| Case 5 | 4156.76 | **182.68x** | 24 | 3925.61 | 3352.087 | 0.854 | 1464.492 | 0.373 | 2750.454 | 0.701 | 3778.555 | 0.963 | 47.853 | 0.012 |
| Case 6 | 4088.36 | **185.74x** | 24 | 3786.26 | 3254.31 | 0.86 | 1753.463 | 0.463 | 2680.849 | 0.708 | 3625.42 | 0.958 | 47.398 | 0.013 |
| Case 7 | 4053.44 | **187.34x** | 24 | 3665.12 | 3163.682 | 0.863 | 968.616 | 0.264 | 2609.617 | 0.712 | 3513.806 | 0.959 | 45.76 | 0.012 |
| Case 8 | 4012.44 | **189.25x** | 24 | 3559.06 | 3076.396 | 0.864 | 1026.069 | 0.288 | 2533.512 | 0.712 | 3435.584 | 0.965 | 272.576 | 0.077 |

**Theoretical Performance Comparison**:

Matmul theoretical performance evaluation key parameters are: Cube computation performance and MTE2 bandwidth.

#### Cube Computation Performance Analysis

Example parameters: M=N=K=8192, baseM=128, baseN=256, baseK=64. This example performance data tested on Atlas A2 Training Series products. This computation chip main frequency is 1.85GHz, processes 16×16×16 multiply-add operations per cycle.

Cube theoretical computation time:
$$cube\_time = \frac{M \times N \times K}{16 \times 16 \times 16 \times core\_num \times cube\_freq} = \frac{8192 \times 8192 \times 8192}{16 \times 16 \times 16 \times 24 \times 1850} = 3022.92\mu s$$

Case 8 Cube computation duration error:
$$Error = \frac{aic\_mac\_time - cube\_time}{cube\_time} = \frac{{3076.396\mu s} - {3022.92\mu s}}{{3022.92\mu s}} = 1.77\%$$

Excluding startup overhead, has achieved 86% peak computing power of this chip

#### MTE2 Bandwidth Analysis

Read-in data total amount:
$$Read-in data total amount = \left[\frac{N}{baseN} \times M \times K\right] + \left[\frac{M}{baseM} \times K \times N\right] \times dataType = (32 \times 8192 \times 8192) + (64 \times 8192 \times 8192) \times 2B = 12GB$$

Ideally assuming L2Cache capacity is large enough, first load data from HBM, subsequent data all read from L2Cache. L2Cache peak bandwidth is approximately 5TB/s, HBM bandwidth is approximately 1.8TB/s.
$$First time data amount read from HBM = M \times K \times dataType + K \times N \times dataType = 256MB$$

MTE2 theoretical duration:
$$MTE2 theoretical duration =\frac{HBM read-in data amount}{1.8TB/s} +\frac{L2Cache read-in data amount}{5TB/s} = 2672.44\mu s$$

Case 8 MTE2 duration error:
$$MTE2 duration error = \frac{{3435.584\mu s} - {2672.44\mu s}}{{2672.44\mu s}} = 28.55\%$$

Current MTE2 duration differs significantly from theoretical value because actual chip L2Cache size is 192MB, current L2Cache partitioning strategy is relatively simple. Meanwhile when MTE2 transfer scenario is ND2NZ (GM data Layout is ND, transfer to L1 needs ND→NZ format conversion), L2Cache bandwidth decreases. Users can further optimize L2Cache partitioning strategy to improve MTE2 bandwidth.



### Ascend 950PR Chip Performance Data

| Case version | Task Duration(μs) | End-to-end Duration Relative to Case 0 | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------|------------------|----------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| Case 0 | 1096626.431 | **1x** | 1 | 1096625.66 | 198351.65 | 0.181 | 583195.222 | 0.532 | 115705.132 | 0.106 | 960571.993 | 0.876 | 28615.988 | 0.026 |
| Case 1 | 130560.475 | **8.40x** | 1 | 130559.56 | 88685.142 | 0.679 | 36462.067 | 0.279 | 22489.156 | 0.172 | 106793.342 | 0.818 | 4200.385 | 0.032 |
| Case 2 | 4294.619 | **255.35x** | 32 | 4293.9 | 2788.781 | 0.649 | 1149.24 | 0.268 | 707.592 | 0.165 | 3540.645 | 0.825 | 141.876 | 0.033 |
| Case 3 | 4332.557 | **253.11x** | 32 | 4331.82 | 2774.213 | 0.64 | 1143.276 | 0.264 | 703.896 | 0.162 | 3582.246 | 0.827 | 144.525 | 0.033 |
| Case 4 | 2668.224 | **410.99x** | 32 | 2667.49 | 2571.074 | 0.964 | 1377.736 | 0.516 | 799.378 | 0.3 | 2531.912 | 0.949 | 33.864 | 0.013 |
| Case 5 | 2591.366 | **423.18x** | 32 | 2590.51 | 2547.046 | 0.983 | 612.956 | 0.237 | 834.311 | 0.322 | 1926.358 | 0.744 | 35.44 | 0.014 |
| Case 6 | 2589.888 | **423.43x** | 32 | 2589.18 | 2547.518 | 0.984 | 765.125 | 0.296 | 826.429 | 0.319 | 1879.029 | 0.726 | 33.261 | 0.013 |
| Case 7 | 2589.09 | **423.55x** | 32 | 2588.38 | 2547.049 | 0.984 | 426.398 | 0.165 | 827.939 | 0.32 | 1895.165 | 0.732 | 33.648 | 0.013 |
| Case 8 | 2558.155 | **428.68x** | 32 | 2557.49 | 2549.657 | 0.997 | 412.29 | 0.161 | 835.579 | 0.327 | 1900.322 | 0.743 | 213.789 | 0.084 |

**Theoretical Performance Comparison**:
#### Cube Computation Performance Analysis

Example parameters: M=N=K=8192, baseM=256, baseN=256, baseK=64. This example performance data tested on Ascend 950PR chip. This processor main frequency is 1.65GHz, processes 16×16×16 multiply-add operations per cycle.

Cube theoretical computation time:
$$cube\_time = \frac{M \times N \times K}{16 \times 16 \times 16 \times core\_num \times cube\_freq} = \frac{8192 \times 8192 \times 8192}{16 \times 16 \times 16 \times 32 \times 1650} = 2542\mu s$$

Case 8 Cube computation duration error:
$$Error = \frac{aic\_mac\_time - cube\_time}{cube\_time} = \frac{{2549.657\mu s} - {2542\mu s}}{{2542\mu s}} = 0.30\%$$

Has achieved 99.7% peak computing power of this chip
#### MTE2 Bandwidth Analysis

Read-in data total amount:
$$Read-in data total amount = \left[\frac{N}{baseN} \times M \times K\right] + \left[\frac{M}{baseM} \times K \times N\right] \times dataType = (32 \times 8192 \times 8192) + (32 \times 8192 \times 8192) \times 2B = 8GB$$

Ideally assuming L2Cache capacity is large enough, first load data from HBM, subsequent data all read from L2Cache. L2Cache peak bandwidth is approximately 5TB/s, HBM bandwidth is approximately 1.6TB/s.
$$First time data amount read from HBM = M \times K \times dataType + K \times N \times dataType = 256MB$$

MTE2 theoretical duration:
$$MTE2 theoretical duration =\frac{HBM read-in data amount}{1.6TB/s} +\frac{L2Cache read-in data amount}{5TB/s} = 1832.1\mu s$$

Case 8 MTE2 duration error:
$$MTE2 duration error = \frac{{1900.322\mu s} - {1832.1\mu s}}{{1832.1\mu s}} = 3.72\%$$
Compared to Atlas A2 Training Series chip, Ascend 950PR chip upgrade, data transfer is more efficient

## Tuning Recommendations

1. **Start from Small Scale**: First use single-core basic version to verify function correctness
2. **Progressive Optimization**: Introduce optimization methods sequentially according to case order, observe performance improvement
3. **Multi-core Partitioning Strategy**: Reasonably set multi-core partitioning strategy, avoid same address access
4. **Utilize MDL Mode**: MDL mode provides highly optimized implementation, prioritize use
4. **L2Cache**: L2Cache can further relieve MTE2 bound, recommend use for data requiring repeated reading
6. **Constant Tiling**: For fixed shape scenarios, use constant Tiling to reduce Scalar runtime overhead
7. **UnitFlag Optimization**: Enable UnitFlag to parallelize computation and transfer pipeline

## Build and Run

Execute the following steps in the example root directory to build and run the example.
- Configure Environment Variables
  Choose the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
  - Default path, root user installation
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installation
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, CANN software package installation
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Example Execution
  ```bash
  SCENARIO_NUM=0                       # Select execution scenario
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # Build project (default npu mode)
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                           # Execute
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output result correctness, confirm algorithm logic correctness
  ```

  For NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;   # NPU simulation mode
  ```

  > **Note:** Clear cmake cache before switching build modes. Execute `rm CMakeCache.txt` in build directory and re-run cmake.

- Build Option Description

  | Option | Available Values | Description |
  |------|--------|------|
  | `SCENARIO_NUM` | `0`-`8` | Example type (0-8), default 0 |
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series/Atlas A2 Inference Series and Atlas A3 Training Series/Atlas A3 Inference Series, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

  The following execution result indicates successful precision comparison.
  ```bash
  test pass!
  ```

## Performance Analysis

Use `msprof` tool to obtain detailed performance data:

```bash
msprof ./demo   # Analyze case performance
```

Current directory generates PROF_-prefix folder. `mindstudio_profiler_output` directory saves Host and each Device performance data summary. Performance data analysis recommends viewing files in this directory.
```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Saves Host and each Device performance data summary
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```