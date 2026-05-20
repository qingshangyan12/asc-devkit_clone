# Matmul Gelu Best Practice Example

## Overview

This example demonstrates Cube-Vector (CV) fusion implementation based on the static Tensor programming paradigm. It fuses Matmul matrix multiplication and GELU activation function to execute in parallel within the same AI Core. After the AIC side completes Matmul computation, it outputs results to GM or UB through Fixpipe. The AIV side reads data from GM or UB and completes GELU vector computation. For Matmul optimization details, refer to [Matmul Basic API Best Practice Example](../../01_matrix_compute_practices/matmul_basic_api_high_performance/README_en.md). For GELU optimization details, refer to [Gelu Performance Tuning Example](../../02_reg_vector_compute_practices/gelu_high_performance/README_en.md).

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── matmul_gelu_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // Input data and ground truth data generation script
│   │   └── verify_result.py    // Ground truth comparison file
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read and write functions
│   └── mmad_gelu.asc           // Ascend C example implementation
```

## Example Description

Matmul + GELU computation formula:

$$
C = GELU(A * B)
$$

$$
GELU(x) \approx \frac{x}{1 + e^{-1.595769 \cdot (x + 0.044715 \cdot x^3)}}
$$

- A is the left matrix with shape [M, K] and data type float16. B is the right matrix with shape [K, N] and data type float16. Matrix B is stored in transposed format.
- C is the destination operand storing Matmul+GELU computation results with shape [M, N] and data type float32.

- Example specifications:

<table>
<tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="5" align="center">Matmul + GELU</td></tr>
<tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
<tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
<tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">float16</td><td align="center">ND</td><td align="center">true</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">float32</td><td align="center">ND</td><td align="center">-</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="5" align="center">mmad_vector_custom</td></tr>
</table>

## Example Implementation

### Performance Metrics Description

| Metric                    | Description                                                                 |
|------------------------|----------------------------------------------------------------------|
| Task Duration(μs)      | Total execution time of the task. Use this parameter as the operator execution time.                         |
| Block Num              | Number of cores used (Block count)                                              |
| aicore_time(μs)        | Average execution time of AI Core                                                |
| aic_mac_time(μs)       | Elapsed time for cube type instructions (matrix computation instructions)                                    |
| aic_mac_ratio          | Ratio of cube type instruction cycles to total cycles    |
| aic_mte1_time(μs)      | Elapsed time for mte1 type instructions (L1 -> L0A/L0B transfer)                               |
| aic_mte1_ratio         | Ratio of mte1 type instruction cycles to total cycles |
| aic_mte2_time(μs)      | Elapsed time for mte2 type instructions (GM -> L1 transfer)                                    |
| aic_mte2_ratio         | Ratio of mte2 type instruction cycles to total cycles    |
| aic_fixpipe_time(μs)   | Elapsed time for fixpipe type instructions (L0C -> GM/UB transfer)                             |
| aic_fixpipe_ratio      | Ratio of fixpipe type instruction cycles to total cycles |
| aiv_time(μs)           | Task execution time on AI Vector Core                               |
| aiv_vec_time(μs)       | Elapsed time for vec type instructions (vector computation instructions)                                    |
| aiv_vec_ratio          | Ratio of vec type instruction cycles to total cycles    |
| aiv_scalar_time(μs)    | Elapsed time for scalar type instructions (scalar computation instructions)                                 |
| aiv_scalar_ratio       | Ratio of scalar type instruction cycles to total cycles |
| aiv_mte2_time(μs)      | Elapsed time for mte2 type instructions (GM -> UB transfer instructions)                              |
| aiv_mte2_ratio         | Ratio of mte2 type instruction cycles to total cycles |
| aiv_mte3_time(μs)      | Elapsed time for mte3 type instructions (UB -> GM transfer instructions)                              |
| aiv_mte3_ratio         | Ratio of mte3 type instruction cycles to total cycles |

### Data Flow Path

This example provides two CV fusion scenarios:

**Scenario 1: CV Fusion-GM Transfer** (Supported on A2/A3/Ascend 950PR)

```
GM ──(MTE2)──> L1 ──(MTE1)──> L0A/L0B ──(Cube)──> L0C ──(Fixpipe)──> GM ──(MTE2)──> UB ──(Gelu)──> UB ──(MTE3)──> GM
```

**Scenario 2: CV Fusion-UB Direct** (Only supported on Ascend 950PR)

Ascend 950PR adds the L0C->UB pathway, which is not supported on A2/A3. With enhanced chip capabilities, the Matmul results can be output directly to UB for GELU computation without going through GM.

```
GM ──(MTE2)──> L1 ──(MTE1)──> L0A/L0B ──(Cube)──> L0C ──(Fixpipe)──> UB ──(Gelu)──> UB ──(MTE3)──> GM
```

### Core Features

The Matmul part of this example uses the same optimization techniques as [Matmul Basic API Best Practice Example](../../01_matrix_compute_practices/matmul_basic_api_high_performance/README_en.md) (L1/L0 double buffering, large packet transfer, fine-grained pipeline synchronization, UnitFlag, and so on). The GELU part uses RegBase method on Ascend 950PR architecture and MemBase method on A2/A3 architecture. The following focuses on the CV fusion implementation mechanism.

The following highlights the AIC↔AIV CV fusion mechanism.

#### 1. CV Fusion Architecture: `__mix__(1,2)` Kernel Function

This example uses `__global__ __mix__(1, 2)` to declare the kernel function, indicating that each logical core consists of 1 AIC and 2 AIVs. The AIC side handles Matmul computation, and the AIV side handles GELU vector computation. Both execute in pipeline parallel at (baseM × baseN) block granularity within the same AI Core.

```cpp
__global__ __mix__(1, 2) void mmad_vector_custom(__gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* c)
{
    AscendC::InitSocState();
    KernelMmadVector<...> op;
    op.Init(a, b, c);
    op.Process();
    AscendC::PipeBarrier<PIPE_ALL>();
}
```

In `Process()`, use `ASCEND_IS_AIC` and `ASCEND_IS_AIV` macros to distinguish AIC and AIV execution paths:

```cpp
if ASCEND_IS_AIC {
    // AIC side: Buffer allocation + compute loop + Fixpipe output
    ProcessLoopAic(...);
}
if ASCEND_IS_AIV {
    // AIV side: GELU vector computation + output to GM
    ProcessLoopAiv();
}
```

#### 2. Cross-Core Synchronization: AIC→AIV Data Ready Notification

After the AIC side completes Fixpipe output for each (baseM × baseN) sub-block, it notifies the AIV side that data is ready through `CrossCoreSetFlag`. The AIV side waits for this signal through `CrossCoreWaitFlag` and then starts GELU computation:

```cpp
// AIC side CopyOutAic():
AscendC::CrossCoreSetFlag<0x2, PIPE_FIX>(0x8);  // Notify AIV data is ready

// AIV side GeluAndCopyOutAivSeparate() / GeluAndCopyOutAiv():
AscendC::CrossCoreWaitFlag(0x8);  // Wait for AIC side Fixpipe to complete
```

#### 3. Scenario 1: CV Fusion-GM Transfer

The AIC side writes L0C computation results to GM through Fixpipe. The AIV side reads data from GM to UB, performs GELU computation, and writes results back to GM. This method is supported on all architectures.

**AIC side**: Fixpipe L0C → GM

```cpp
// A2/A3 architecture
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = curN;
fixpipeParams.mSize = curM;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = N;
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_GM>(cGM[...], cLocal, fixpipeParams);

// Ascend 950PR architecture
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
fixpipeParams.mSize = curM;
fixpipeParams.nSize = curN;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = N;
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_GM>(cGM[...], cLocal, fixpipeParams);
```

**AIV side**: GM → xUB → GELU → geluOutUB → GM

In `__mix__(1,2)` mode, each logical core has 2 AIVs. The M rows of data output to GM by Fixpipe are split by M/2 rows for two AIVs to process. Each AIV obtains its own sub-block index through `GetSubBlockIdx() % 2` and independently completes GELU computation for its M/2 rows:

```cpp
uint32_t localSubIdx = AscendC::GetSubBlockIdx() % 2;
uint32_t gmOffset = mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * baseM / 2 * N;

// Step 1: DataCopy GM → xUB
AscendC::DataCopyPad<float>(xUB, cGM[gmOffset], copyInParams, padParams);

// Step 2: GELU computation
// A2/A3: GeluMemBaseCompute(xUB, geluOutUB, computeLen);
// Ascend 950PR:    GeluRegBaseCompute(xUB, geluOutUB, computeLen);

// Step 3: DataCopy geluOutUB → GM
AscendC::DataCopyPad<float>(cGM[gmOffset], geluOutUB, copyOutParams);
```

#### 4. Scenario 2: CV Fusion-UB Direct (Only supported on Ascend 950PR)

The AIC side writes L0C computation results directly to UB through Fixpipe. The AIV side does not need to read data from GM. It directly performs GELU computation on UB and writes results back to GM. This method eliminates the GM transfer read/write overhead but only supports Fixpipe from L0C to UB on Ascend 950PR architecture.

**AIC side**: Fixpipe L0C → UB (Enable dual-target mode through `dualDstCtl=0b01`, split by M dimension to 2 AIVs)

```cpp
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
fixpipeParams.mSize = AscendC::DivCeil(curM, 2) * 2;
fixpipeParams.nSize = curN;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = curN;
fixpipeParams.dualDstCtl = 0b01;  // Split by M dimension to 2 AIVs' UB
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_UB>(xUB, cLocal, fixpipeParams);
```

**AIV side**: UB → GELU → geluOutUB → GM

The AIV side does not need DataCopy GM→UB and directly performs GELU computation on xUB:

```cpp
// GELU computation: xUB as input, geluOutUB as output
GeluRegBaseCompute(xUB, geluOutUB, computeLen);

// DataCopy: geluOutUB → GM
uint32_t localSubIdx = AscendC::GetSubBlockIdx() % 2;
uint32_t offset = baseM / 2 * N;
AscendC::DataCopyPad<float>(cGM[mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * offset], geluOutUB, copyParams);
```

#### 5. GELU Computation Implementation

**A2/A3 architecture**: Uses MemBase method (vector basic API + PipeBarrier), no additional intermediate buffers required, directly reuses output buffer:

```cpp
__aicore__ inline void GeluMemBaseCompute(
    const AscendC::LocalTensor<float>& xLocal, const AscendC::LocalTensor<float>& yLocal, uint32_t n)
{
    AscendC::Mul(yLocal, xLocal, xLocal, n);        // x²
    AscendC::PipeBarrier<PIPE_V>();
    AscendC::Mul(yLocal, yLocal, xLocal, n);        // x³
    ……
}
```

**Ascend 950PR architecture**: Uses RegBase method (VF function), intermediate computation completes in registers without additional UB buffers, reducing data interaction between UB and Reg:

```cpp
__simd_vf__ inline void GeluVf(__ubuf__ float* xAddr, __ubuf__ float* yAddr, uint32_t n, uint32_t loopNum)
{
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(float);
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<float> xReg, yReg;

    for (uint16_t i = 0; i < loopNum; ++i) {
        mask = AscendC::Reg::UpdateMask<float>(n);
        AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);
        AscendC::Reg::Mul(yReg, xReg, xReg, mask);       // x²
        ……
        AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);
    }
}
```

For detailed optimization analysis of GELU formula simplification and RegBase API with VF fusion, refer to [Gelu Performance Tuning Example](../../02_reg_vector_compute_practices/gelu_high_performance/README_en.md).

#### 6. Multi-Core Tiling under `__mix__(1,2)` Mode

In `__mix__(1,2)` mode, `GetBlockIdx()` is independently numbered on AIC and AIV sides: AIC side returns 0~numAICores-1, AIV side returns 0~numAIVCores-1. Since each logical core corresponds to 2 AIVs, the AIV side needs to map `GetBlockIdx() / 2` back to the logical core ID to use the same multi-core tiling strategy as the AIC side:

```cpp
uint32_t logicCoreId;
if ASCEND_IS_AIC {
    logicCoreId = AscendC::GetBlockIdx();
} else {
    logicCoreId = AscendC::GetBlockIdx() / 2;  // __mix__(1,2): Each logical core corresponds to 2 AIVs
}
uint32_t mIterIdx = logicCoreId % mIter;
uint32_t nIterIdx = logicCoreId / mIter;
```

### Performance Data Analysis

#### Atlas A2 Training Series Chip Performance Data

- Scenario 1: CV Fusion-GM Transfer, singleCoreM=2048, singleCoreN=1536, 24 cores

| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|
|Gelu computation | AI_VECTOR_CORE | 369.827 | 48 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 358.91 | 236.506 | 0.659 | 8.293 | 0.023 | 316.672 | 0.882 | 150.033 | 0.418 |
| Matmul computation | AI_CORE | 4227.605 | 24 | 0 | 3760.55 | 3083.197 | 0.82 | 338.636 | 0.09 | 2516.081 | 0.669 | 3685.713 | 0.98 | 193.704 | 0.052 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Scenario 1 | MIX_AIC | 4236.944 | 24 | 48 | 3770.16 | 3083.235 | 0.818 | 531.971 | 0.141 | 2520.852 | 0.669 | 3687.839 | 0.978 | 193.933 | 0.051 | 3773.6 | 174.327 | 0.046 | 2.016 | 0.001 | 90.315 | 0.024 | 66.373 | 0.018 |

#### Ascend 950PR Chip Performance Data

- Scenario 1: CV Fusion-GM Transfer, singleCoreM=2048, singleCoreN=1024, 32 cores
- Scenario 2: CV Fusion-UB Direct, singleCoreM=2048, singleCoreN=1024, 32 cores

| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|
| Gelu computation(RegBase) | AI_VECTOR_CORE | 348.868 | 64 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 347.99 | 66.277 | 0.19 | 3.03 | 0.009 | 320.543 | 0.921 | 314.547 | 0.904 |
| Matmul computation | AI_CORE | 2601.311 | 32 | 0 | 2600.55 | 2593.264 | 0.997 | 104.582 | 0.04 | 816.91 | 0.314 | 1875.057 | 0.721 | 252.163 | 0.097 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Scenario 1 | MIX_AIC | 2606.91 | 32 | 64 | 2605.9 | 2597.856 | 0.997 | 105.131 | 0.04 | 818.781 | 0.314 | 1917.737 | 0.736 | 252.231 | 0.097 | 2605.93 | 59.897 | 0.023 | 1.783 | 0.001 | 57.145 | 0.022 | 50.516 | 0.019 |
| Scenario 2 | MIX_AIC | 2573.584 | 32 | 64 | 2572.67 | 2565.251 | 0.997 | 104.669 | 0.041 | 819.934 | 0.319 | 2096.38 | 0.815 | 212.403 | 0.083 | 2572.69 | 60.438 | 0.023 | 1.138 | 0 | 0.005 | 0 | 68.76 | 0.027 |

#### Performance Data Interpretation

**Scenario 1: CV Fusion-GM Transfer**

Taking Ascend 950PR chip data as an example:
- Standalone Gelu task takes 348.868μs, standalone Matmul task takes 2601.311μs
- Scenario 1 fusion total time is 2606.91μs, basically consistent with Matmul standalone execution time

This indicates that when using CV fusion, the AIV Vector computation time is completely covered by the AIC Cube computation time. Since Cube and Vector can execute in parallel, the overall task execution time is not a simple C+V addition, but essentially equals the Cube computation time (that is, Matmul time), achieving efficient pipeline parallelism.

The figure below shows the pipeline parallel execution process of AIC and AIV in Scenario 1 (CV Fusion-GM Transfer). After completing Matmul computation on the AIC side, it writes results to GM through Fixpipe. The AIV side reads data from GM and completes GELU computation. Both execute in pipeline parallel at (baseM × baseN) block granularity:

![Scenario1 CV Fusion-GM Transfer Pipeline Parallelism](figure/CVParallell_L0C_GM_UB.png)

> **Note:** The pipeline diagram is obtained from the product line CANN package msopprof tool on the board. Command: `msprof op --aic-metrics=PipeUtilization`

**Reason for Significant MTE2 Time Reduction:**

Comparing the data shows:
- Standalone Gelu computation aiv_mte2_time is 320.543μs
- Scenario 1 aiv_mte2_time is only 57.145μs, reduced by approximately 82.2%

Reason analysis: Although in Scenario 1, AIV also needs to read data from GM to UB, after AIC's Fixpipe writes Matmul computation results to GM, the data will be cached in L2 Cache. When AIV initiates MTE2 operation to read data from GM, since the data is already in L2 Cache, it actually reads from L2 Cache instead of GM. Therefore, MTE2 time is significantly reduced. This is also an implicit performance optimization point in CV fusion scenarios.

**Scenario 2: CV Fusion-UB Direct**

Taking Ascend 950PR chip data as an example:
- Scenario 2 fusion total time is 2573.584μs, further reduced by approximately 33μs compared to Scenario 1's 2606.91μs
- Scenario 2 aiv_mte2_time is only 0.005μs, almost reduced to zero compared to Scenario 1's 57.145μs

The figure below shows the pipeline parallel execution process of AIC and AIV in Scenario 2 (CV Fusion-UB Direct). After completing Matmul computation on the AIC side, it writes results directly to UB through Fixpipe (without GM transfer). The AIV side directly performs GELU computation on UB, eliminating the GM→UB MTE2 transfer overhead:

![Scenario2 CV Fusion-UB Direct Pipeline Parallelism](figure/CVParallell_L0C_UB.png)


## Build and Run

Execute the following steps in the root directory of this example to build and run the example.

### Configure Environment Variables

Select the appropriate command to configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.

- Default path, root user installed CANN package

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- Default path, non-root user installed CANN package

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- Specified path install_path, installed CANN package

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### Example Execution

```bash
SCENARIO=1                                                           # Select execution scenario (1 for CV Fusion-GM Transfer, 2 for CV Fusion-UB Direct, only supported on Ascend 950PR)
mkdir -p build && cd build;                                          # Create and enter build directory
cmake -DSCENARIO_NUM=$SCENARIO -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # Build project (default npu mode)
python3 ../scripts/gen_data.py                                       # Generate test input data
./demo                                                               # Execute the compiled executable program
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

For NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

Example:

```bash
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;   # NPU simulation mode
```

> **Note:**
> - Before switching build mode, clean cmake cache. Execute `rm CMakeCache.txt` in the build directory and re-run cmake.
> - A2/A3 architecture only supports Scenario 1 (GM Transfer). Selecting Scenario 2 will trigger a compile-time `static_assert` error.

### Build Options Description

| Parameter | Description | Options | Default Value |
|------|------|---------|--------|
| `SCENARIO_NUM` | CV fusion scenario | 1: CV Fusion-GM Transfer; 2: CV Fusion-UB Direct (only supported on Ascend 950PR) | `1` |
| `CMAKE_ASC_RUN_MODE` | Run mode | `npu`, `sim` | `npu` |
| `CMAKE_ASC_ARCHITECTURES` | NPU hardware architecture | `dav-2201`, `dav-3510` | `dav-3510` |

The following result indicates successful precision comparison:

```bash
test pass!
```

### Performance Analysis

Use the `msprof` tool to obtain detailed performance data:

```bash
msprof ./demo   # Analyze example performance
```

A folder with PROF_ prefix will be generated in the current directory. The `mindstudio_profiler_output` directory stores Host and Device performance data summary. For performance data analysis, view the files in this directory:

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Stores Host and Device performance data summary
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```