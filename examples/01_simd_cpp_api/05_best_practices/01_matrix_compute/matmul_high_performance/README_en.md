<<<<<<< HEAD
# Matmul Best Practice Example

## Overview

This example implements matrix multiplication based on the Matmul high-level API, demonstrating the complete tuning path from basic implementation to high-performance optimization through 9 progressive optimization cases, including single-core basic version, Tiling optimization, multi-core parallel splitting, MDL mode, L1Cache/L2Cache optimization, constant Tiling, UnitFlag optimization, and other optimization methods.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure
=======
# Matmul best practice examples

## Overview

This sample implements matrix multiplication using the Matmul high-level API. Nine progressive optimization cases demonstrate the complete tuning path from a basic implementation to high performance, including a basic single-core version, Tiling optimization, multi-core parallel splitting, MDL mode, L1Cache/L2Cache optimization, constant Tiling, and UnitFlag optimization.

## Products and CANN software versions supported by this example

| Products | CANN Software Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 training series products/Atlas A3 inference series products | >= CANN 9.0.0 |
| Atlas A2 training series products/Atlas A2 inference series products | >= CANN 9.0.0 |

## Introduction to directory structure
>>>>>>> upstream/master

```
├── matmul_high_performance
│   ├── scripts
<<<<<<< HEAD
│   │   ├── gen_data.py         // Input data and ground truth generation script
│   │   └── verify_result.py    // Ground truth comparison script
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── figures                 // Illustrations
│   ├── matmul.h                // Header file definitions for all optimization cases
│   ├── matmul.asc              // Ascend C example implementation
│   └── README.md               // Example documentation
```

## Example Description

- Computation formula: C = A * B
  - A, B are source operands. A is the left matrix, shape [M, K]; B is the right matrix, shape [K, N]
  - C is the destination operand, storing the matrix multiplication result matrix, shape [M, N]

- Example specifications:

<table>
<tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="5" align="center">Matmul</td></tr>
<tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
<tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td><td align="center">false</td></tr>
<tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td><td align="center">true</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">half</td><td align="center">ND</td><td align="center">-</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="5" align="center">matmul_custom</td></tr>
</table>

## Example Implementation

### Class Implementation Description

This example implements different optimization strategies through three independent classes, each corresponding to specific Case versions.

| Class Name | Corresponding Case | Implementation Characteristics | Kernel Function Used | Optimization Feature |
|------|---------|---------|-------------|---------|
| **MatmulKernel** | Case 0-5 | Basic implementation, runtime Tiling | matmul_custom (isMdl=false)<br>matmul_custom_mdl (isMdl=true) | - Case 0: Single-core basic version<br>- Case 1: Single-core Tiling optimization<br>- Case 2: Multi-core splitting 2x12<br>- Case 3: Multi-core splitting 4x6<br>- Case 4: MDL mode<br>- Case 5: MDL mode with L1Cache optimization |
| **MatmulKernelL2Cache** | Case 6 | L2Cache optimization, runtime Tiling | matmul_custom_mdl_l2cache | - MDL mode<br>- L1Cache optimization<br>- L2Cache optimization (A matrix M axis splitting) |
| **MatmulKernelMdlL2CacheConstant** | Case 7-8 | Constant Tiling, compile-time computation | matmul_custom_mdl_l2cache_constant (useUnitFlag=false, Case7)<br>matmul_custom_mdl_l2cache_constant_unitflag (useUnitFlag=true, Case8) | - MDL mode<br>- L1Cache optimization<br>- L2Cache optimization<br>- Constant Tiling (compile-time computation)<br>- Case 8: UnitFlag optimization |

#### 1. Tiling Mechanism Characteristics

- `MatmulKernel` and `MatmulKernelL2Cache` use the `TCubeTiling` type, requiring complete Tiling data structure copy from GM memory. Tiling parameters are computed by the Scalar unit at runtime
- `MatmulKernelMdlL2CacheConstant` uses a custom `MatmulProblemShape` structure containing only shape information (M, N, K, singleCoreM, etc.). Tiling parameters are already computed at compile time through `CONSTANT_CFG`, requiring no Scalar computation at runtime

#### 2. Process Method Computation Flow Characteristics

- **Computation Flow**: `MatmulKernel` single iteration, `MatmulKernelL2Cache` and `MatmulKernelMdlL2CacheConstant` loop 2 times (L2Cache optimization, A matrix M axis splitting)

### Performance Metric Description

| Metric | Description |
|------|------|
| Task Duration(μs) | Total execution time of the entire task, operator execution time is determined by this parameter |
| Block Num | Number of cores used (Block count) |
| aicore_time(μs) | Average AI Core execution time |
| aic_mac_time(μs) | Cube computation unit execution time |
| aic_mac_ratio | Cube computation unit time ratio, reflecting computation unit utilization |
| aic_scalar_time(μs) | Scalar computation unit execution time |
| aic_scalar_ratio | Scalar computation unit time ratio |
| aic_mte1_time(μs) | MTE1 (L1 to L0A/L0B transfer) execution time |
| aic_mte1_ratio | MTE1 time ratio, reflecting L1 to L0 data transfer pressure |
| aic_mte2_time(μs) | MTE2 (GM to L1 transfer) execution time |
| aic_mte2_ratio | MTE2 time ratio, reflecting GM to L1 data loading pressure |
| aic_fixpipe_time(μs) | FixPipe (L0C to GM transfer) execution time |
| aic_fixpipe_ratio | FixPipe time ratio, reflecting result write-back memory access pressure |

**Note**: The following Case performance change analysis uses A2 chip (Ascend 910B1) performance data as example. Ascend 950PR performance tuning data please refer to [below](#ascend-950pr-chip-performance-data).

### Case 0: Single-Core Basic Version (SINGLE_CORE_BASIC)

**Example Objective**: Implement basic Matmul functionality, ensuring computation correctness

**Core Implementation**:
- Uses single-core computation, **numBlocks=1**
- Basic Tiling configuration, **baseM=baseN=baseK=64**, base blocks are the basic blocks participating in one matrix multiplication instruction. baseM, baseN, baseK represent the M, N, K axis lengths on L0 during Matmul computation, in element units
- Transfer strategy: Input A, B matrices on GM are transferred in baseM×baseK, baseK×baseN blocks sequentially to L1, then from L1 to L0A/L0B. The Cube unit performs one baseM×baseN×baseK size Matmul computation for each transferred base block

**Key Code**:
=======
│   │   ├── gen_data.py         // Input data and ground truth data generation script file
│   │   └── verify_result.py    // truth comparison file
│   ├── CMakeLists.txt          // Compile project files
│   ├── data_utils.h            // Data reading and writing functions
│   ├── figures                 // Illustration
│   ├── matmul.h                // Header file definitions for all optimization cases
│   ├── matmul.asc              // Ascend C sample implementation
│   └── README.md               // Sample documentation
```

## Sample description

- Calculation formula: C = A * B
  - A and B are source operands, A is the left matrix with the shape [M, K]; B is the right matrix with the shape [K, N]
  - C is the destination operand, which stores the matrix of the matrix multiplication result. The shape is [M, N]

- Sample specifications:

<table>
<tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="5" align="center">Matmul</td></tr>
<tr><td rowspan="3" align="center"> sample input </td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
<tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td><td align="center">false</td></tr>
<tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td><td align="center">true</td></tr>
<tr><td rowspan="1" align="center"> sample output</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">half</td><td align="center">ND</td><td align="center">-</td></tr>
<tr><td rowspan="1" align="center">Kernel function name</td><td colspan="5" align="center">matmul_custom</td></tr>
</table>

## Sample implementation

### Class implementation description

This example implements different optimization strategies through three independent classes, each class corresponding to a specific Case version.

| Class name | Corresponding Case | Implementation features | Kernel function used | Optimization features |
|------|---------|---------|-------------|---------|
| **MatmulKernel** | Case 0-5 | Basic implementation, runtime Tiling | matmul_custom (isMdl=false)<br>matmul_custom_mdl (isMdl=true) | - Case 0: Single-core basic version <br>- Case 1: Single-core Tiling optimization <br>- Case 2: Multi-core segmentation 2x12<br>- Case 3: Multi-core segmentation 4x6<br>- Case 4: MDL mode <br>- Case 5: L1Cache optimization in MDL mode |
| **MatmulKernelL2Cache** | Case 6 | L2Cache optimization, runtime Tiling | matmul_custom_mdl_l2cache | - MDL mode <br>- L1Cache optimization <br>- L2Cache optimization (A matrix M-axis segmentation) |
| **MatmulKernelMdlL2CacheConstant** | Case 7-8 | Constant Tiling, calculated at compile time | matmul_custom_mdl_l2cache_constant (useUnitFlag=false, Case7)<br>matmul_custom_mdl_l2cache_constant_unitflag (useUnitFlag=true, Case8) | - MDL mode <br>- L1Cache optimization <br>- L2Cache optimization <br>- Constant Tiling (compile time calculation) <br>- Case 8: UnitFlag optimization |

#### 1. Characteristics of Tiling mechanism

- `MatmulKernel` and `MatmulKernelL2Cache` use the `TCubeTiling` type and need to copy the complete Tiling data structure from GM memory. The Tiling parameters are calculated by the Scalar unit at runtime.
- `MatmulKernelMdlL2CacheConstant` uses the custom `MatmulProblemShape` structure, which only contains shape information (M, N, K, singleCoreM, etc.). The Tiling parameters have been calculated by `CONSTANT_CFG` at compile time, and no Scalar calculation is required at runtime.

#### 2. Process method calculation process characteristics

- **Calculation process**: `MatmulKernel` single iteration, `MatmulKernelL2Cache` and `MatmulKernelMdlL2CacheConstant` cycle 2 times (L2Cache optimization, A matrix M-axis segmentation)

### Performance indicator description

| Indicator | Description |
|------|------|
| Task Duration(μs) | The total time of the entire task execution. The operator execution time is based on this parameter |
| Block Num | Number of cores used (number of blocks) |
| aicore_time(μs) | Average execution time of AI Core |
| aic_mac_time(μs) | Execution time of Cube computing unit |
| aic_mac_ratio | The time proportion of the Cube computing unit, reflecting the utilization of the computing unit |
| aic_scalar_time(μs) | Execution time of Scalar scalar calculation unit |
| aic_scalar_ratio | The time ratio of the Scalar scalar calculation unit |
| aic_mte1_time(μs) | Execution time of MTE1 (L1 to L0A/L0B transfer) |
| aic_mte1_ratio | The time ratio of MTE1, reflecting the data transfer pressure from L1 to L0 |
| aic_mte2_time(μs) | Execution time of MTE2 (GM to L1 transfer) |
| aic_mte2_ratio | The time ratio of MTE2, reflecting the data loading pressure from GM to L1 |
| aic_fixpipe_time(μs) | Execution time of FixPipe (L0C to GM transfer) |
| aic_fixpipe_ratio | The time ratio of FixPipe, reflecting the memory access pressure of result writeback |

**Note**: The following analysis uses performance data from an A2 chip (Ascend 910B1). For Ascend 950PR tuning data, see [Ascend 950PR Chip Performance Data](#ascend-950pr-chip-performance-data).

### Case 0: Single-core basic version (SINGLE_CORE_BASIC)

**Sample Goal**: Implement basic Matmul functions to ensure calculation accuracy

**Core implementation**:
- Use single core calculation, **numBlocks=1**
- Basic Tiling configuration, **baseM=baseN=baseK=64**, the base block is the basic block that participates in a matrix multiplication instruction. baseM, baseN, and baseK respectively represent the lengths of the M, N, and K axes on L0 during Matmul calculation, in units of elements.
- Transfer strategy: The A and B matrices input on the GM are sequentially transferred to L1 in blocks of baseM×baseK and baseK×baseN, and then transferred from L1 to L0A/L0B. The Cube unit performs a Matmul calculation of the size of baseM×baseN×baseK for each transferred base block.

**Key code**:
>>>>>>> upstream/master
```cpp
tilingApi.SetShape(M, N, K);
tilingApi.SetFixSplit(64, 64, 64);
tilingData.set_baseK(64);
```

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 759363.98 | 1 | 759363.46 | 141699.459 | 0.187 | 508787.509 | 0.67 | 162104.184 | 0.213 | 582515.828 | 0.767 | 34303.063 | 0.045 |

<<<<<<< HEAD
**Analysis**:
- Single-core computation time **759363.98μs**
- Computation unit utilization aic_mac_ratio only **18.7%**. This scenario is only used as a Matmul operation performance comparison example and is not recommended for users

### Case 1: Single-Core Tiling Optimization (SINGLE_CORE_TILING)

**Optimization Objective**: Optimize base block parameters in Tiling to improve single-core computation efficiency

**Core Implementation**:
- Uses single-core computation, **numBlocks=1**
- Uses optimized Tiling configuration, **baseM=128, baseN=256, baseK=64**

**Key Code**:
=======
**analyze**:
- Single core operation time **759363.98μs**
- The computing unit utilization aic_mac_ratio is only **18.7%**. This scenario is only used as a comparison example of Matmul computing performance and is not recommended for users.

### Case 1: Single-core Tiling optimization (SINGLE_CORE_TILING)

**Optimization Goal**: Optimize base block parameters in Tiling to improve single-core computing efficiency

**Core implementation**:
- Use single core calculation, **numBlocks=1**
- Adopt optimized Tiling configuration, **baseM=128, baseN=256, baseK=64**
- Transfer strategy: The A and B matrices input on the GM are sequentially transferred to L1 in blocks of baseM×baseK and baseK×baseN, and then transferred from L1 to L0A/L0B. The Cube unit performs a Matmul calculation of the size of baseM×baseN×baseK for each transferred base block.

**Key code**:
>>>>>>> upstream/master
```cpp
tilingApi.SetShape(M, N, K);
tilingApi.SetFixSplit(128, 256, 64);
```

<<<<<<< HEAD
**Optimization Method**:
- **Base block selection principle**: In Case 0, the base block set in Tiling is [baseM, baseN, baseK] = [64, 64, 64], with high **memory-to-computation ratio** (the data volume needed per computation cycle). For the current large shape scenario, the base block selection principle is to minimize the memory-to-computation ratio, meaning with the same Cube computation volume, the required data volume for memory access is minimized.
=======
**Optimization means**:
- **base block selection principle**: The base block set in Tiling in case0 is [baseM, baseN, baseK] = [64, 64, 64], and the **memory access calculation ratio** (that is, the amount of data required to calculate each cycle) is higher. For current scenarios with large shapes, the selection principle of basic blocks is to minimize the memory access and calculation ratio, that is, when the cube calculation amount is the same, the amount of data that needs to be accessed is the smallest.

- **Memory access calculation ratio analysis**: When the input is of fp16 type, the Cube execution unit can complete 16×16×16 multiplication and addition operations in 1 cycle. When the base block is set to [baseM, baseN, baseK] = [128, 256, 64], the minimum memory access calculation ratio can be achieved while meeting the GM address 512Byte alignment when moving out. The number of cycles calculated by Cube is (128 × 64 × 256) / (16 × 16 × 16) = 512cycle, and the memory access calculation ratio is (128 × 64 × 2 + 256 × 64 × 2) / 512cycle = **96(byte / cycle)**; set the base block to [baseM, baseN, baseK] = [64, 64, 64], the Cube calculation cycle number is (64 × 64 × 64) / (16 × 16 × 16) = 64cycle, and the memory access calculation ratio is (64 × 64 × 2 + 64 × 64 * 2) / 64cycle = **256 (byte / cycle)**. The base block scheme of [128, 256, 64] has a lower **memory access and calculation ratio. For the same amount of calculation, a smaller amount of data is required, and the required bandwidth pressure is also lower**

- 💡**Recommended base block settings**: On A2/A3 chips, L0A and L0B are both 64 KB and L0C is 128 KB. `[baseM, baseN, baseK] = [128, 256, 64]` maximizes memory utilization. For b16 input, the recommended base block is `[128, 256, 64]`; for b8 input, it is `[128, 256, 128]`.
>>>>>>> upstream/master

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 249467.08 | 1 | 249466.54 | 81477.188 | 0.327 | 63608.818 | 0.255 | 52003.703 | 0.208 | 195613.432 | 0.784 | 11313.746 | 0.045 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 0, Task Duration decreased from 759363.98μs to 249467.08μs, reduced by **509896.90μs**, performance improved **3.04x**
- MTE2 data transfer duration decreased from 582515.828μs to 195613.432μs, reduced by **66.42%**
- Next optimization direction: Introduce multi-core parallel computation to fully utilize multi-core resources for improving overall throughput

### Case 2: Multi-Core Splitting 2x12 (MULTI_CORE_SPLIT_2_12)

**Optimization Objective**: Introduce multi-core parallel computation to improve overall throughput

**Core Implementation**:
- Multi-core parallel computation, splitting 8192×8192 matrix multiplication across 24 cores
- Splitting strategy: M direction 2 blocks, N direction 12 blocks, **singleM=4096, singleN=683, tail block tailN=679**

<img src="figures/2_12_split_core.png">

=======
**analyze**:
- Compared with Case 0, Task Duration is reduced from 759363.98μs to 249467.08μs, a reduction of **509896.90μs**, and the performance is improved **3.04** times
- MTE2 data transfer time was reduced from 582515.828μs to 195613.432μs, a reduction of **66.42%**
- Follow-up optimization direction: introduce multi-core parallel computing and make full use of multi-core resources to improve overall throughput

### Case 2: Multi-core split 2x12 (MULTI_CORE_SPLIT_2_12)

**Optimization Goal**: Introduce multi-core parallel computing to improve overall throughput

**Core implementation**:
- Multi-core parallel computing, dividing the 8192×8192 matrix multiplication into 24 cores for parallel execution
- Segmentation strategy: 2 blocks in M ​​direction, 12 blocks in N direction, **singleM=4096, singleN=683, tail block tailN=679**
- Transfer strategy: The A and B matrices input on the GM are sequentially transferred to L1 in blocks of baseM×baseK and baseK×baseN, and then transferred from L1 to L0A/L0B. The Cube unit performs a Matmul calculation of the size of baseM×baseN×baseK for each transferred base block.
<img src="figures/2_12_split_core.png">

**Key code**:
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(4096, 683, 8192);
tilingApi.SetFixSplit(128, 256, 64);
SetL1(tilingData);
```

**Optimization means**:
- Balance load distribution and try to make the calculation amount of each core similar

>>>>>>> upstream/master
**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 12541.22 | 24 | 12537.56 | 3462.802 | 0.276 | 2987.47 | 0.238 | 2260.551 | 0.18 | 10177.798 | 0.812 | 875.439 | 0.07 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 1, Task Duration decreased from 249467.08μs to 12541.22μs, performance improved **19.89x**. Multi-core parallel computation significantly improved overall throughput
- aic_mte2_time duration **10177.798μs**, ratio **81.2%**, becoming the performance improvement bottleneck
- Next optimization direction: The current multi-core splitting strategy does not satisfy 512B address alignment and does not evenly split M, N across cores. The splitting strategy will be optimized to improve address alignment and memory access efficiency

### Case 3: Multi-Core Splitting 4x6 (MULTI_CORE_SPLIT_4_6)

**Optimization Objective**: Optimize multi-core splitting strategy

**Core Implementation**:
- Multi-core parallel computation, splitting 8192×8192 matrix multiplication across 24 cores
- Splitting strategy: Even splitting of M, N. M direction 4 blocks, N direction 6 blocks

<img src="figures/4_6_split_core.png">

**Optimization Method**:
- Addresses satisfy **512B alignment**, setting **singleM=2048, singleN=1536, tail block tailN=512**, improving effective bandwidth utilization
- **Avoid same-address access**: Same-address access means multiple cores simultaneously reading the same row of A matrix need to access the same memory address. Hardware must serialize multiple accesses to the same address. The more cores with same-address access, the more severe the performance degradation from serialization. Compared to the 2×12 core splitting strategy in Case 2, Case 3's 4×6 splitting has smaller same-address conflict delay
=======
**analyze**:
- Compared with Case 1, Task Duration was reduced from 249467.08μs to 12541.22μs, performance improved **19.89 times**, and multi-core parallel computing significantly improved the overall throughput.
- aic_mte2_time takes **10177.798μs**, accounting for **81.2%**, becoming a performance improvement bottleneck
- Follow-up optimization direction: The current multi-core segmentation strategy does not meet the address 512B alignment, and M and N are not evenly divided into cores. In the future, the segmentation strategy will be optimized to improve address alignment and memory access efficiency.

### Case 3: Multi-core split 4x6 (MULTI_CORE_SPLIT_4_6)

**Optimization Goal**: Optimize multi-core segmentation strategy

**Core implementation**:
- Multi-core parallel computing, dividing the 8192×8192 matrix multiplication into 24 cores for parallel execution
- Cutting strategy: Cut M and N evenly, 4 pieces in the M direction and 6 pieces in the N direction.
- Transfer strategy: The A and B matrices input on the GM are sequentially transferred to L1 in blocks of baseM×baseK and baseK×baseN, and then transferred from L1 to L0A/L0B. The Cube unit performs a Matmul calculation of the size of baseM×baseN×baseK for each transferred base block.

<img src="figures/4_6_split_core.png">

**Key code**:
```cpp
tilingApi.SetDim(24);
tilingApi.SetSingleShape(2048, 1536, 8192);
tilingApi.SetFixSplit(128, 256, 64);
SetL1(tilingData);
```

**Optimization means**:
- The address meets **512B alignment**, set **singleM=2048, singleN=1536, tail block tailN=512** to improve effective bandwidth utilization
- **Avoid same-address access**: Same-address access means that when multiple cores read the same row of data in the A matrix at the same time, they need to access the same memory address. Multiple accesses to the same address by hardware need to be serialized. The more cores are accessed at the same address, the more serious the performance degradation caused by serialization. Compared with the 2×12 core splitting strategy in case 2, the 4×6 core splitting strategy in case 3 has a smaller delay in conflicts with the same address.
- **When multi-core is split, M and N should be split evenly while meeting the alignment of address 512B. **
>>>>>>> upstream/master

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 12283.84 | 24 | 10870.96 | 3394.884 | 0.312 | 2656.33 | 0.244 | 2166.824 | 0.199 | 8616.671 | 0.793 | 488.829 | 0.045 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 2, Task Duration decreased from 12541.22μs to 12283.84μs, reduced by 2.05%
- aic_mte2_time decreased from 10177.798μs to 8616.671μs, reduced by **15.33%**, reduction of **1561.127μs**

### Case 4: Multi-Core Using MDL Template (MULTI_CORE_MDL)

**Optimization Objective**: Use MDL template, enable L1 multi-block cache functionality, enable "large packet" transfer, reduce MTE2 loop transfer count

**Core Implementation**:
- Uses 24-core parallel computation, splitting strategy same as above (4x6)
- Uses MDL template, enables "large packet" transfer functionality

**Optimization Method**:
- Enable MDL mode, supporting "large packet" transfer
- **Large packet transfer**: MTE2 GM to L1 transfer no longer transfers only one basic block at a time, but caches multiple basic blocks in L1, significantly reducing GM to L1 transfer count. In this example scenario, depthA1=4 with L1 transfer double buffer enabled, meaning L1 caches 4 baseM × baseK data blocks, with ping and pong buffers each loading two blocks during transfer
=======
**analyze**:
- Compared with Case 2, Task Duration decreased from 12541.22μs to 12283.84μs, a decrease of 2.05%.
- aic_mte2_time decreased from 10177.798μs to 8616.671μs, a decrease of **15.33%**, and a decrease of **1561.127μs**
- Follow-up optimization direction: The current aicore time consumption is still mainly for MTE2, and the L1 multi-block cache function will be enabled in the future to hide the data transfer delay and improve the efficiency of the MTE2 pipeline.

### Case 4: Multi-core using MDL template (MULTI_CORE_MDL)

**Optimization goal**: Use MDL template, enable L1 multi-block cache function, enable "big package" handling, and reduce the number of MTE2 cycle handling

**Core implementation**:
- Use 24 cores for parallel computing, and the segmentation strategy is the same as above (4x6)
- Use MDL template to enable "big package" handling function

**Key code**:
```cpp
AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
    matmulObj;
// MDL templates are automatically optimized and the large package handling function is enabled
```

**Optimization means**:
- Enable MDL mode to support "large package" handling
- **Large package transfer**: MTE2 transfers from GM to L1 no longer only transfers one basic block at a time, but caches multiple basic blocks in L1, which can significantly reduce the number of transfers from GM to L1. In this example scenario, depthA1=4 and the double buffer for L1 transfer is enabled, which means that 4 baseM × baseK data blocks are cached in L1, and two blocks each are moved into the ping and pong buffers during transfer.
- 💡**L1 multi-block cache tuning parameters should meet: **
  - dbL0A / dbL0B=2
  - depthA1 / (stepM * stepKa)=2，
  - depthB1 / (stepN * stepKb)=2
  - **Parameter meaning:**
    - dbL0A, dbL0B: respectively indicate whether MTE1 of A matrix and B matrix has double buffer enabled (value is 1 or 2, 2 indicates double buffer is enabled)
    - depthA1, depthB1: respectively represent the number of copies of the fully loaded basic block in L1
    - stepM, stepKa: respectively represent the multiples of baseM in the M direction of the data block cached in L1 of the A matrix, and the Ka direction is the multiple of baseK in the K direction of the A matrix.
    - stepN, stepKb: respectively represent the multiple of baseN in the N direction of the data block cached in L1 of the B matrix, and the Kb direction is the multiple of baseK in the K direction of the B matrix.
>>>>>>> upstream/master

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 5039.86 | 24 | 4487.89 | 3116.472 | 0.694 | 2073.97 | 0.462 | 2332.914 | 0.52 | 4051.458 | 0.903 | 44.749 | 0.01 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 3, Task Duration decreased from 12283.84μs to 5039.86μs, duration reduced by **58.98%**
- aic_mte2_time decreased from 8616.671μs to 4051.458μs, MTE2 transfer time reduced by **52.97%**
- aic_mac_ratio improved from **31.2%** to **69.4%**

### Case 5: Multi-Core MDL + L1Cache Optimization (MULTI_CORE_MDL_L1CACHE)

**Optimization Objective**: Increase MTE2 transfer intensity, fully utilize L1 cache space

**Core Implementation**:
- Uses 24-core parallel computation, splitting strategy same as above (4x6)
- Uses MDL template, enables "large packet" transfer functionality
- Manually optimizes Tiling parameters to **depthA1=16, stepKa=8**, fully utilizing L1 cache space
=======
**analyze**:
- Compared with Case 3, Task Duration was reduced from 12283.84μs to 5039.86μs, and the time consumption was reduced by **58.98%**
- aic_mte2_time was reduced from 8616.671μs to 4051.458μs, MTE2 transfer time was reduced by **52.97%**, and the decrease was **4565.213μs**
- aic_mac_ratio increased from **31.2%** to **69.4%**
- Follow-up optimization direction: The Tiling calculated by the MDL template automatic tuning does not fully utilize the L1 space. Tiling parameters can be manually adjusted to further improve the handling strength of MTE2.

### Case 5: Multi-core MDL + L1Cache optimization (MULTI_CORE_MDL_L1CACHE)

**Optimization Goal**: Improve the handling strength of MTE2 and make full use of L1 cache space

**Core implementation**:
- Use 24 cores for parallel computing, and the segmentation strategy is the same as above (4x6)
- Use MDL template to enable "big package" handling function
- Manually optimize the Tiling parameters to **depthA1=16, stepKa=8** to make full use of the L1 cache space

**Key code**:
```cpp
AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_MDL>
    matmulObj;
tilingData.set_depthA1(16);  // Increase depthA1 and increase the number of blocks of the A matrix in the L1 cache
tilingData.set_stepKa(8);
```

**Optimization means**:
- **L1 Cache Tiling parameter optimization**: Manually adjust the depthA1 and stepKa parameters to make full use of the L1 cache
  - Current configuration: depthA1=16, depthB1=8, baseM=128, baseN=256, baseK=64
  - L1 cache occupancy calculation:

    A matrix:
      depthA1 × baseM × baseK × sizeof(half) = 16 × 128 × 64 × 2B = 262,144B = 256 KB

    B matrix:
      depthB1 × baseN × baseK × sizeof(half) = 8 × 256 × 64 × 2B = 262,144B = 256 KB

    Total 256 KB + 256 KB = 512 KB
>>>>>>> upstream/master

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4156.76 | 24 | 3925.61 | 3352.087 | 0.854 | 1464.492 | 0.373 | 2750.454 | 0.701 | 3778.555 | 0.963 | 47.853 | 0.012 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 4, Task Duration decreased from 5039.86μs to 4156.76μs, duration reduced by **17.52%**
- aic_mac_ratio improved from 69.4% to 85.4%, Cube utilization improved by **23.05%**
- aic_mte2_ratio reaches **96.3%**, reaching MTE2 bound, becoming the performance improvement bottleneck

### Case 6: Multi-Core MDL + L1Cache + L2Cache (MULTI_CORE_MDL_L1CACHE_L2CACHE)

**Optimization Objective**: Enable L2Cache optimization to mitigate MTE2 bound

**Core Implementation**:
- Uses 24-core parallel computation, splitting strategy same as above (4x6)
- Uses MDL template with optimized L1 Cache parameters same as above
- Enables `L2Cache`, splitting A matrix M axis

**Optimization Method**:
- **Enable L2Cache optimization**:
  - L2Cache characteristics: L2Cache is the shared external cache for AI Cores. L2Cache pure read bandwidth is approximately 3 to 4 times that of GM. When transferring the same data volume in or out, accessing data in L2Cache is faster than GM
  - Cache hit optimization: If data cannot hit L2Cache, requiring GM access for read/write, bandwidth utilization efficiency is low, potentially making MTE2 the performance bottleneck during the entire example run
  - **Block computation adapted to L2Cache**: Current L2Cache size is **192MB**, total data needed for matrix computation is **384MB** (A matrix 128MB + B matrix 128MB + C matrix 128MB). Since L2Cache capacity is less than total matrix computation data, the A matrix can be split into 2 parts along M axis, with region 1 completing full computation with B matrix, then region 2 completing full computation with B matrix, improving L2Cache hit rate through splitting

Block computation adapted to L2Cache diagram:

<img src="figures/L2Cache.png">

=======
**analyze**:
- Compared with Case 4, Task Duration is reduced from 5039.86μs to 4156.76μs, and the time consumption is reduced by **17.52%**
- aic_mte2_time was reduced from 4051.458μs to 3778.555μs, and the MTE2 handling time was reduced by **6.74%**
- aic_mac_ratio increased from 69.4% to 85.4%, and cube utilization increased by **23.05%**
- Follow-up optimization direction: aic_mte2_ratio reaches **96.3%**, reaching MTE2 bound and becoming a performance improvement bottleneck. L2Cache will be optimized in the future to alleviate MTE2 bound.

### Case 6: Multi-core MDL + L1Cache + L2Cache (MULTI_CORE_MDL_L1CACHE_L2CACHE)

**Optimization Goal**: Enable L2Cache optimization and alleviate MTE2 bound

**Core implementation**:
- Use 24 cores for parallel computing, and the segmentation strategy is the same as above (4x6)
- Use MDL template and optimize L1 Cache parameters as above
- Enable `L2Cache` to segment the M axis of the A matrix

**Key code**:
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

**Optimization means**:
- **Enable L2Cache Optimization**:
  - L2Cache features: L2Cache is an external cache shared by AI Core. The pure read bandwidth of L2Cache is about 3 to 4 times that of GM. When the same amount of data is moved in or out, accessing data in L2Cache is faster than GM.
  - Cache hit optimization: If the data cannot hit the L2Cache, it will need to access GM for reading and writing, and the bandwidth utilization efficiency will be low, causing MTE2 to become a performance bottleneck during the entire running process of the sample.
  - **Blocked calculation adapted to L2Cache**: The current L2Cache size is **192MB**, and the total amount of data required for matrix calculation is **384MB** (A matrix 128MB + B matrix 128MB + C matrix 128MB), since the L2Cache capacity is smaller than the total amount of matrix calculation data, the A matrix can be divided into two parts on the M axis. Area 1 and B matrix complete the complete operation, and area 2 completes the complete operation with the B matrix. The hit rate of L2cache can be improved through splitting.

Schematic diagram of block computing adaptation to L2Cache:

<img src="figures/L2Cache.png">

```
Calculation process:
Step 1: C1 = A1 × B (A1 is loaded from GM to L2Cache, B is loaded from GM to L2Cache and resides)
Step 2: C2 = A2 × B (A2 is loaded from GM to L2Cache, B is already in L2Cache, no need to reload)

L2Cache utilization:
- B matrix (128MB) resides in L2Cache after loading in Step 1
- In Step 2, the B matrix is ​​read directly from L2Cache to avoid GM access.
- The bandwidth of a single L2Cache access is about 3 to 4 times that of GM, significantly improving memory access efficiency.
```

>>>>>>> upstream/master
**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4088.36 | 24 | 3786.26 | 3254.31 | 0.86 | 1753.463 | 0.463 | 2680.849 | 0.708 | 3625.42 | 0.958 | 47.398 | 0.013 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 5, Task Duration decreased from 4156.76μs to 4088.36μs, reduced by **68.40μs**
- aic_mte2_time decreased from 3778.555μs to 3625.42μs, MTE2 transfer time reduced by **4.05%**
- aic_mac_ratio improved from **85.4%** to **86%**

### Case 7: Multi-Core MDL + L1Cache + L2Cache + Constants Tiling (MULTI_CORE_MDL_L1CACHE_L2CACHE_CONSTANTS)

**Optimization Objective**: Use constant Tiling to reduce runtime Scalar computation overhead

**Core Implementation**:
- Uses 24-core parallel computation, splitting strategy same as above (4x6)
- Uses MDL template with optimized L1 Cache parameters same as above
- L2 Cache strategy same as above
- Enables full Tiling constantification
=======
**analyze**:
- Compared with Case 5, Task Duration was reduced from 4156.76μs to 4088.36μs, a decrease of **68.40μs**
- aic_mte2_time was reduced from 3778.555μs to 3625.42μs, and the MTE2 handling time was reduced by **4.05%**
- aic_mac_ratio increased from **85.4%** to **86%**
- Follow-up optimization direction: The current limitation on sample performance is still MTE2 bound. Developers can further optimize MTE2 by optimizing the L2Cache segmentation strategy. This sample will later show the optimization of Scalar and Fixpipe pipelines.

### Case 7: Multi-core MDL + L1Cache + L2Cache + Constants Tiling (MULTI_CORE_MDL_L1CACHE_L2CACHE_CONSTANTS)

**Optimization goal**: Use constant Tiling to reduce runtime Scalar calculation overhead

**Core implementation**:
- Use 24 cores for parallel computing, and the segmentation strategy is the same as above (4x6)
- Use MDL template and optimize L1 Cache parameters as above
- L2 Cache strategy is the same as above
- Enable Tiling full constantization

**Key code**:
```cpp
constexpr MatmulShapeParams shapeParams = {SINGLE_M_L2CACHE, SINGLE_N, SINGLE_K, BASE_M, BASE_N, BASE_K};

constexpr static auto CONSTANT_CFG = GetCustomConstantCFG<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, true, true>();
AscendC::Matmul<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CONSTANT_CFG> matmulObj;
// CONSTANT_CFG is calculated at compile time, reducing Scalar calculations at runtime
```

**Optimization means**:
- **Tiling constantization**:
  - The hardware advantage of AI Core lies in vector/matrix parallel computing, and scalar operations cannot take advantage of the hardware capabilities. **Matmul API has a large number of Scalar calculations during the initialization and iteration process. Scalar calculations during Matmul initialization affect the instruction header overhead. Scalar calculations between Matmul iterations may block the MTE2 pipeline**
  - Static Tiling reduces runtime overhead: Use the **MatmulApiStaticTiling** parameter instead of the TCubeTiling variable parameter to advance the Scalar calculation to the compile time. There is no need to dynamically calculate the Scalar unit during runtime, reducing Scalar performance overhead.
>>>>>>> upstream/master

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4053.44 | 24 | 3665.12 | 3163.682 | 0.863 | 968.616 | 0.264 | 2609.617 | 0.712 | 3513.806 | 0.959 | 45.76 | 0.012 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 6, Task Duration decreased from 4088.36μs to 4053.44μs
- aic_scalar_time decreased from 1753.463μs to 968.616μs, reduced by **44.76%**
- Constant Tiling effectively reduces Scalar unit performance overhead

### Case 8: Multi-Core MDL + L1Cache + L2Cache + Constants Tiling + UnitFlag (MULTI_CORE_MDL_L1CACHE_L2CACHE_CONSTANTS_UNITFLAG)

**Optimization Objective**: Enable UnitFlag optimization, parallelize computation and transfer pipeline

**Core Implementation**:
- Uses 24-core parallel computation, splitting strategy same as above (4x6)
- Uses MDL template with optimized L1 Cache parameters same as above
- L2 Cache strategy same as above
- Enables full Tiling constantification same as above
- Enables UnitFlag, optimizing computation and transfer parallelism

**Optimization Method**:
- **Enable UnitFlag optimization**:
  - Optimize computation and transfer parallelism: Without UnitFlag enabled, AIC core MMAD computation instructions and FIXPIPE data transfer instructions are instruction-level synchronized. FIXPIPE instructions must wait for MMAD instructions to complete before transferring results, with MMAD and FIXPIPE pipeline serial. After enabling UnitFlag, MMAD and FIXPIPE achieve **512B size fine-grained synchronization**. During one MMAD instruction execution, whenever one 512B data result computation completes, FIXPIPE immediately transfers that 512B data, achieving Cube computation unit and FIXPIPE transfer unit pipeline parallelism

UnitFlag functionality diagram:
=======
**analyze**:
- Compared with Case 6, Task Duration is reduced from 4088.36μs to 4053.44μs
- aic_scalar_time decreased from 1753.463μs to 968.616μs, a decrease of **44.76%**
- Constant Tiling effectively reduces the performance overhead of the Scalar unit. When the performance of the sample is poor due to Scalar blocking, the user can reduce the proportion of Scalar time through this method
- Follow-up optimization direction: enable UnitFlag optimization to demonstrate parallel computing and handling pipeline

### Case 8: Multi-core MDL + L1Cache + L2Cache + Constants Tiling + UnitFlag (MULTI_CORE_MDL_L1CACHE_L2CACHE_CONSTANTS_UNITFLAG)

**Optimization Goal**: Enable UnitFlag optimization to parallelize computing and handling pipelines

**Core implementation**:
- Use 24 cores for parallel computing, and the segmentation strategy is the same as above (4x6)
- Use MDL template and optimize L1 Cache parameters as above
- L2 Cache strategy is the same as above
- Enable Tiling full constantization as above
- Enable UnitFlag to optimize the parallelism of calculation and handling

**Key code**:
```cpp
// Use constexpr to define compile-time constants and enable UnitFlag
MatmulConfig mmCFG = GetMMConfig<MatmulConfigMode::CONFIG_MDL>(shapeParams);
mmCFG.enUnitFlag = true;
```

**Optimization means**:
- **Enable UnitFlag Optimization**:
  - Optimize the parallelism of calculation and transfer: When the UnitFlag function is not turned on, the MMAD calculation instructions and FIXPIPE data transfer instructions of the AIC core are synchronized at the instruction level. The FIXPIPE instruction needs to wait for the MMAD instruction to be executed before the results are moved out. The pipeline between MMAD and FIXPIPE is serial; turn on UnitF After the lag function, MMAD and FIXPIPE are **512B size fine-grained synchronization**. During the execution of an MMAD instruction, whenever a 512B data result calculation is completed, FIXPIPE immediately moves out the 512B data, thereby achieving parallel flow between the Cube calculation unit and the FIXPIPE handling unit.

UnitFlag function diagram:
>>>>>>> upstream/master

<img src="figures/unitflag_close.png">
<img src="figures/unitflag_open.png">

**Performance Data**:
| Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|-------------------|
| 4012.44 | 24 | 3559.06 | 3076.396 | 0.864 | 1026.069 | 0.288 | 2533.512 | 0.712 | 3435.584 | 0.965 | 272.576 | 0.077 |

<<<<<<< HEAD
**Analysis**:
- Compared to Case 7, Task Duration decreased from 4053.44μs to 4012.44μs
- Current performance benefit is small because the performance constraint remains MTE2 bound, with the operator's MMAD and FIXPIPE pipeline masked by MTE2 bound

## Performance Comparison Summary

### Atlas A2 Training Series Chip Performance Data

**Comprehensive Optimization Effect**:
- This example Cube utilization improved by **67.7%** (18.7% → 86.4%), reaching **86.4%** of chip peak compute
- Through progressive optimization from Case 0 to Case 8, example duration decreased by **99.47%** (759363.98μs → 4012.44μs)

| Case version | Task Duration(μs) | End-to-End Duration vs Case 0 | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
=======
**analyze**:
- Compared with Case 7, Task Duration is reduced from 4053.44μs to 4012.44μs
- aic_fixpipe_time has been increased from 45.76μs to 272.576μs. The reason is that after turning on Unitflag, aic_fixpipe_time will include the waiting time of the FIXPIPE instruction, which is not the actual pipeline time. Users can pay attention to whether the end-to-end performance is improved.
- The current performance gain is small because the performance constraint is still MTE2 bound, and the operator's MMAD and FIXPIPE pipelines are covered by MTE2 bound. When the sample performance is poor due to FIXPIPE blocking, the user can enable the UnitFlag function

## Performance comparison summary

### Atlas A2 training series chip performance data

**Comprehensive optimization effect**:
- In this sample, the cube utilization (the ratio of Cube computing unit time to AI Core time, i.e. `aic_mac_ratio`) increased by **67.7 percentage points** (18.7% → 86.4%)
- Through progressive optimization from Case 0 to Case 8, the sample time is reduced by **99.47%** (759363.98μs → 4012.44μs)

| Case version | Task Duration(μs) | End-to-end time consumption relative to Case 0 | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
>>>>>>> upstream/master
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

<<<<<<< HEAD
### Ascend 950PR Chip Performance Data

| Case version | Task Duration(μs) | End-to-End Duration vs Case 0 | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
=======
**Theoretical performance comparison**:

The key parameters for Matmul theoretical performance evaluation are: Cube computing performance and MTE2 bandwidth.

#### Cube computing performance analysis

Sample parameters: M=N=K=8192, baseM=128, baseN=256, baseK=64. The performance data of this sample was tested on the Atlas A2 training series product. The computing chip has a main frequency of 1.85GHz and processes 16×16×16 multiplication and addition operations per cycle.

Cube theoretical calculation time:
$$cube\_time = \frac{M \times N \times K}{16 \times 16 \times 16 \times core\_num \times cube\_freq} = \frac{8192 \times 8192 \times 8192}{16 \times 16 \times 16 \times 24 \times 1850} = 3022.92\mu s$$

Case 8 Cube calculation time-consuming error:
$$Error = \frac{aic\_mac\_time - cube\_time}{cube\_time} = \frac{{3076.396\mu s} - {3022.92\mu s}}{{3022.92\mu s}} = 1.77\%$$

Excluding startup overhead, 86% of the chip’s peak computing power has been achieved

#### MTE2 Bandwidth Analysis

Total amount of data read:
$$The total amount of data read = \left(\left[\frac{N}{baseN} \times M \times K\right] + \left[\frac{M}{baseM} \times K \times N\right]\right) \times dataType = (32 \times 8192 \times 8192 + 64 \times 8192 \times 8192) \times 2B = 12GB$$

Ideally, assuming that the L2Cache capacity is large enough, data is loaded from HBM for the first time, and subsequent data is read from L2Cache. The peak bandwidth of L2Cache is about 5TB/s, and the bandwidth of HBM is about 1.8TB/s.
$$The total amount of data read from HBM for the first time = M \times K \times dataType + K \times N \times dataType = 256MB$$

MTE2 theory time consumption:
$$Theoretical time consumption of MTE2 =\frac{Total amount of data read by HBM}{1.8TB/s} +\frac{Total amount of data read by L2Cache}{5TB/s}$$

$$=\frac{256 \times 1024 \times 1024B}{1.8 \times 10^{12}B/s} +\frac{12 \times 1024 \times 1024 \times 1024B - 256 \times 1024 \times 1024B}{5 \times 10^{12}B/s} = 149.13\mu s + 2523.29\mu s = 2672.42\mu s$$

Case 8 MTE2 time-consuming error:
$$MTE2 time-consuming error = \frac{{3435.584\mu s} - {2672.42\mu s}}{{2672.42\mu s}} = 28.56\%$$

The current MTE2 time consumption is quite different from the theoretical value because the actual chip L2Cache size is 192MB and the current L2Cache splitting strategy is relatively simple; on the other hand, when the MTE2 transfer scenario is ND2NZ (GM data Layout is ND, ND→NZ format conversion is required when transferring to L1), the L2Cache bandwidth will be reduced. Users can further optimize the L2Cache splitting strategy to increase MTE2 bandwidth.

### Ascend 950PR chip performance data

| Case version | Task Duration(μs) | End-to-end time consumption relative to Case 0 | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
>>>>>>> upstream/master
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

<<<<<<< HEAD
## Tuning Recommendations

1. **Start from small scale**: First use single-core basic version to verify functional correctness
2. **Optimize progressively**: Follow the case order to progressively introduce optimization methods and observe performance improvement
3. **Multi-core splitting strategy**: Set multi-core splitting strategies reasonably to avoid same-address access
4. **Utilize MDL mode**: MDL mode provides highly optimized implementation, prioritize its use
5. **L2Cache**: L2Cache can further mitigate MTE2 bound, recommended for data requiring repeated reads
6. **Constant Tiling**: For fixed shape scenarios, use constant Tiling to reduce Scalar runtime overhead
7. **UnitFlag optimization**: Enabling UnitFlag can parallelize computation and transfer pipeline

## Build and Run

Run the following steps in the root directory of this example to build and run the example.
- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit in the current environment.
=======
**Theoretical performance comparison**:

#### Cube computing performance analysis

Sample parameters: M=N=K=8192, baseM=256, baseN=256, baseK=64. The performance data of this sample was tested on the Ascend 950PR chip. The processor is clocked at 1.65GHz and processes 16×16×16 multiplication and addition operations per cycle.

Cube theoretical calculation time:
$$cube\_time = \frac{M \times N \times K}{16 \times 16 \times 16 \times core\_num \times cube\_freq} = \frac{8192 \times 8192 \times 8192}{16 \times 16 \times 16 \times 32 \times 1650} = 2542\mu s$$

Case 8 Cube calculation time-consuming error:
$$Error = \frac{aic\_mac\_time - cube\_time}{cube\_time} = \frac{{2549.657\mu s} - {2542\mu s}}{{2542\mu s}} = 0.30\%$$

99.7% of the chip’s peak computing power has been achieved

#### MTE2 Bandwidth Analysis

Total amount of data read:
$$The total amount of data read = \left(\left[\frac{N}{baseN} \times M \times K\right] + \left[\frac{M}{baseM} \times K \times N\right]\right) \times dataType = (32 \times 8192 \times 8192 + 32 \times 8192 \times 8192) \times 2B = 8GB$$

Ideally, assuming that the L2Cache capacity is large enough, data is loaded from HBM for the first time, and subsequent data is read from L2Cache. The peak bandwidth of L2Cache is about 5TB/s, and the bandwidth of HBM is about 1.6TB/s.
$$The total amount of data read from HBM for the first time = M \times K \times dataType + K \times N \times dataType = 256MB$$

MTE2 theory time consumption:
$$Theoretical time consumption of MTE2 =\frac{Total amount of data read by HBM}{1.6TB/s} +\frac{Total amount of data read by L2Cache}{5TB/s}$$

$$=\frac{256 \times 1024 \times 1024B}{1.6 \times 10^{12}B/s} +\frac{8 \times 1024 \times 1024 \times 1024B- 256 \times 1024 \times 1024B}{5 \times 10^{12}B/s} = 167.77\mu s + 1664.30\mu s = 1832.1\mu s$$

Case 8 MTE2 time-consuming error:
$$MTE2 time-consuming error = \frac{{1900.322\mu s} - {1832.1\mu s}}{{1832.1\mu s}} = 3.72\%$$
Compared with the Atlas A2 training series chip, the Ascend 950PR chip is upgraded and data transfer is more efficient.

## Tuning suggestions

1. **Start on a small scale**: First use the single-core basic version to verify functional correctness
2. **Gradual optimization**: Gradually introduce optimization methods in case order and observe performance improvements.
3. **Multi-core segmentation strategy**: Set up the multi-core segmentation strategy reasonably to avoid access from the same address
4. **Utilize MDL mode**: MDL mode provides a highly optimized implementation and should be used first.
4. **L2Cache**: L2Cache can further alleviate MTE2 bound and is recommended for data that needs to be read repeatedly.
6. **Constant Tiling**: For fixed shape scenarios, use constant Tiling to reduce Scalar runtime overhead.
7. **UnitFlag Optimization**: Enabling UnitFlag can parallelize computing and handling pipelines

## Compile and run

Perform the following steps in the root directory of this sample to compile and execute the sample.
- Configure environment variables
  Please configure the environment variables according to the [installation method ](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit package in the current environment.
>>>>>>> upstream/master
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

<<<<<<< HEAD
  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in this example directory.
  ```bash
  SCENARIO_NUM=0                       # Select execution scenario
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # Build the project (default npu mode)
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                           # Run
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify whether the output result is correct and confirm algorithm logic correctness
  ```

  To use NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;   # NPU simulation mode
  ```

  > **Notice:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and then re-run cmake.

- Build option description

  | Option | Values | Description |
  |------|--------|------|
  | `SCENARIO_NUM` | `0`-`8` | Example type (0-8), default is 0 |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

  The following execution result indicates that the accuracy comparison succeeded.
=======
  > **Note:** `${install_path}` is the CANN package installation directory. If the installation directory is not specified, it will be installed under `/usr/local/Ascend` by default.

- Sample execution

  Execute the following command in this sample directory.
  ```bash
  SCENARIO_NUM=0                       # Select execution scenario
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # Compile project (default npu mode)
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                           # implement
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify whether the output results are correct and confirm that the algorithm logic is correct
  ```

  When using NPU simulation mode, just add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;   # NPU emulation mode
  ```

  > **Note:** You need to clear the cmake cache before switching the compilation mode. You can execute `rm CMakeCache.txt` in the build directory and then re-cmake.

- Description of compilation options

  | options | optional values ​​| description |
  |------|--------|------|
  | `SCENARIO_NUM` | `0`-`8` | Sample type (0-8), default is 0 |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Operation mode: NPU operation, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 training series products/Atlas A2 inference series products and Atlas A3 training series products/Atlas A3 inference series products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

  The execution results are as follows, indicating that the accuracy comparison is successful.
>>>>>>> upstream/master
  ```bash
  test pass!
  ```

## Performance Analysis

<<<<<<< HEAD
Use the `msprof` tool to obtain detailed performance data:

```bash
msprof ./demo   # Analyze case performance
```

A PROF_ prefixed folder is generated in the current directory. The `mindstudio_profiler_output` directory stores Host and Device performance data summaries. Performance data analysis is recommended to view files in this directory.
```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Stores Host and Device performance data summaries
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
=======
### Introduction to the msOpProf Tool
msOpProf is a single-operator performance analysis tool with two usage modes: `msopprof` and `msopprof simulator`. It helps users identify anomalies in operator memory, code, and instructions for comprehensive operator tuning. It currently supports performance data collection and automatic parsing for different run modes (on-device or simulation) and file types (executables or operator binary `.o` files).

- On-device performance collection

    On-device performance collection directly measures the operator's execution time on the Ascend AI Processor. This method is suitable for quickly locating operator performance issues in an on-device environment.

    Run msopprof on the `demo` executable for operator tuning:
    ```
    msopprof ./demo
    ```

    - Performance data description
      After the command completes, a folder named "OPPROF_{timestamp}_XXX" will be generated in the default directory. The performance data folder structure is as follows:

      ```bash
      ├──dump                       # Raw performance data; users do not need to inspect it
      ├──ArithmeticUtilization.csv  # Cube/Vector instruction cycle proportions
      ├──L2Cache.csv                # L2 Cache hit rate; affects MTE2. Plan data transfer logic properly to increase the hit rate
      ├──Memory.csv                 # Read/write bandwidth rates of UB, L1, and main memory
      ├──MemoryL0.csv               # Read/write bandwidth rates of L0A, L0B, and L0C
      ├──MemoryUB.csv               # Read/write bandwidth rates from Vector and Scalar to UB
      ├──OpBasicInfo.csv            # Basic operator information
      ├──PipeUtilization.csv        # Durations and proportions of computation and data transfer units
      ├──ResourceConflictRatio.csv  # Proportions of UB bank groups, bank conflicts, and resource conflicts among all instructions
      └──visualize_data.bin         # MindStudio Insight presentation file
      ```
>>>>>>> upstream/master
