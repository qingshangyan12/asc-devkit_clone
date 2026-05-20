# CrossCoreSetFlag and CrossCoreWaitFlag Inter-Core Synchronization Example

## Overview

This example first introduces the three synchronization modes supported by the inter-core synchronization interfaces CrossCoreSetFlag and CrossCoreWaitFlag (see the table below). It then demonstrates the specific usage of these three synchronization modes in pure Vector computation scenarios and Cube-Vector fusion computation scenarios.
<table border="1" align="center">
  <tr bgcolor="lightgray">
    <td>Synchronization Control Mode</td>
    <td align="center">Description</td>
  </tr>
  <tr>
    <td rowspan="2">mode 0</td>
    <td>For AIC scenarios, synchronize all AIC cores. Instructions after CrossCoreWaitFlag execute only when all AIC cores have reached CrossCoreSetFlag.</td>
  </tr>
  <tr>
    <td>For AIV scenarios, synchronize all AIV cores. Instructions after CrossCoreWaitFlag execute only when all AIV cores have reached CrossCoreSetFlag.</td>
  </tr>
  <tr>
    <td>mode 1</td>
    <td>Synchronization control between AIV cores within a single AI Core. Instructions after CrossCoreWaitFlag execute only when both AIV cores have run CrossCoreSetFlag.</td>
  </tr>
  <tr>
    <td rowspan="2">mode 2</td>
    <td>After an AIC core executes CrossCoreSetFlag, instructions after CrossCoreWaitFlag on two AIV cores continue to execute.</td>
  </tr>
  <tr>
    <td>After both AIV cores execute CrossCoreSetFlag, instructions after CrossCoreWaitFlag on the AIC core can execute.</td>
  </tr>
</table>

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── cross_core_set_wait_flag
│   ├── scripts
│   │   ├── gen_data.py         // Input data and golden data generation script
│   │   └── verify_result.py    // Verification script for comparing output data with golden data
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── figures                 // Diagrams
│   ├── cross_core_set_wait_flag.h   // Ascend C example implementation
│   └── cross_core_set_wait_flag.asc // Invocation example and result verification
```

## Example Description
<table border="1" style="text-align: center;">
  <tr>
    <td>SCENARIO_NUM Value</td>
    <td>Business Scenario</td>
    <td>Synchronization Mode Used</td>
  </tr>
  <tr>
    <td>0</td>
    <td>Pure Vector computation scenario (16 AIV cores)</td>
    <td>mode 0 (AIV full-core synchronization)</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Pure Vector computation scenario (2 AIV cores)</td>
    <td>mode 1</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Cube-Vector fusion computation scenario</td>
    <td>mode 2 (AIC waits for AIV), mode 2 (AIV waits for AIC), mode 0 (AIC full-core synchronization)</td>
  </tr>
</table>
This example uses SCENARIO_NUM to control execution branches. Different values of this variable correspond to different business scenarios and synchronization modes. As shown in the table above, different SCENARIO_NUM values demonstrate the specific usage of three synchronization modes in pure Vector computation and Cube-Vector fusion computation scenarios.

### Computation Formula and Example Specifications

#### SCENARIO_NUM=0 (Pure Vector Computation Scenario, Mode 0)
- Computation Formula:  
  $$
  z = \sum_{i=0}^{15} (x \times i)
  $$
  - x is the input vector, all values are 1
  - i is the BlockIdx of each AIV (range 0-15)
  - z is the accumulated result from all AIV cores

- Example Specifications:
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">CrossCoreSetFlagMode0</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[32]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[32]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">cross_core_set_wait_flag_custom</td></tr>
  </table>

#### SCENARIO_NUM=1 (Pure Vector Computation Scenario, Mode 1)
- Computation Formula:  
  $$
  z = (x \times 2) + (x \times 3)
  $$
  - x is the input vector, all values are 1
  - Only AIV cores with BlockIdx=2 and 3 participate in computation
  - z is the accumulated result from these two AIV cores

- Example Specifications:
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">CrossCoreSetFlagMode1</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[32]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[32]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">cross_core_set_wait_flag_custom</td></tr>
  </table>

#### SCENARIO_NUM=2 (Cube-Vector Fusion Computation Scenario)
- Computation Formula:  
  $$
  C = \text{LeakyRelu}(Cast(A) \times Cast(B))
  $$
  - A is the left matrix, shape [M, K], data type uint8
  - B is the right matrix, shape [K, N], data type uint8
  - First convert A and B data types from uint8 to half
  - Then execute matrix multiplication: A × B
  - Finally execute LeakyRelu operation on the result
  - C is the final result, shape [M, N], data type float32

- Example Specifications:
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="5" align="center">CrossCoreSetFlagMode2</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
  <tr><td align="center">a</td><td align="center">[32, 32]</td><td align="center">uint8</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td align="center">b</td><td align="center">[32, 64]</td><td align="center">uint8</td><td align="center">ND</td><td align="center">false</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">c</td><td align="center">[32, 64]</td><td align="center">float32</td><td align="center">ND</td><td align="center">-</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="5" align="center">mmad_custom</td></tr>
  </table>

## Example Implementation

### 1. Cube-Vector Fusion Computation Scenario

#### 1.1 Overall Logic
<p align="center">
  <img src="figures/融合场景_示意图.png" width="100%">
</p>
<p align="center">
Figure 1: Cube-Vector fusion computation scenario, overall computation logic diagram
</p>

This example addresses the fusion operator scenario (configured with __mix__(1,2)). Each AI Core contains 1 AIC and 2 AIV cores. Configure logical core count numBlocks = 8, corresponding to 8 AIC cores and 16 AIV cores. Figure 1 shows that the overall computation logic consists of three core phases: precision conversion phase, block matrix multiplication and atomic accumulation phase, and LeakyRelu operation and result write-back phase.

#### 1.2 Precision Conversion Phase (Mode 2, AIC waits for AIV within single AI Core)
Since the left and right matrices on GM have uint8 data type, which does not meet the mmad instruction requirements for input data type, you must first move data from GM to AIV for precision conversion before performing block matrix multiplication in AIC. Therefore, within each AI Core, one AIC must wait for the other two AIV cores in the same AI Core to complete data precision conversion before starting block matrix multiplication.

Specifically: Split the left matrix (A matrix) data in GM along the K axis into 8 parts, distribute to AIV cores with even BlockIdx for uint8 to half precision conversion. Split the right matrix (B matrix) data in GM along the K axis into 8 parts, distribute to AIV cores with odd BlockIdx for uint8 to half precision conversion. As shown in Figure 2, use inter-core synchronization mode 2 according to the above description.

The corresponding code segment:

        if (blockIdx % 2 == 0) {
            ...
            AscendC::Cast(castALocal, aLocal, AscendC::RoundMode::CAST_NONE, A_BLOCKS_LENGTH);
            ...
            AscendC::DataCopy(AVectorGM, castALocal, A_BLOCKS_LENGTH);
        } else {
            ...
            AscendC::Cast(castBLocal, bLocal, AscendC::RoundMode::CAST_NONE, B_BLOCKS_LENGTH);
            ...
            AscendC::DataCopy(BVectorGM, castBLocal, B_BLOCKS_LENGTH);
        }
        // Mode 2, within each AICore, one AIC waits for 2 AIV cores
        AscendC::CrossCoreSetFlag<2, PIPE_MTE3>(SYNC_AIV_AIC_FLAG);

<p align="center">
  <img src="figures/融合场景_精度转换阶段.png" width="100%">
</p>
<p align="center">
Figure 2: Precision conversion phase, mode 2 diagram
</p>

#### 1.3 Block Matrix Multiplication and Atomic Accumulation Phase (Mode 0, AIC Full-Core Synchronization)
Each AIC executes block matrix multiplication, then enables atomic accumulation mechanism. Move computation results to the same GM region, accumulate results from 8 AIC cores' block matrix multiplication on GM, and finally obtain the complete C matrix. To obtain the correct C matrix, you must wait for all 8 AIC cores to complete block matrix multiplication and move results to GM through FixPipe. As shown in Figure 3, use inter-core synchronization mode 0 (AIC full-core synchronization) according to the above description.

The corresponding code segment:
$$
C = \sum_{i=1}^{8} A_i \cdot B_i
$$

        // Mode 2, within each AICore, AIC waits for 2 AIV cores
        AscendC::CrossCoreWaitFlag(SYNC_AIV_AIC_FLAG);

        CopyIn(a1Local, b1Local);
        SplitA(a1Local, a2Local);
        SplitBTranspose(b1Local, b2Local);
        Compute(a2Local, b2Local, c1Local);
        CopyOut(c1Local);
        // Mode 0, synchronize 8 AIC cores within 8 AICores
        AscendC::CrossCoreSetFlag<0, PIPE_FIX>(SYNC_AIC_FLAG);  
        AscendC::CrossCoreWaitFlag(SYNC_AIC_FLAG);  

        // Mode 2, within each AICore, 2 AIV cores wait for AIC
        AscendC::CrossCoreSetFlag<2, PIPE_FIX>(SYNC_AIC_AIV_FLAG);  

<p align="center">
  <img src="figures/融合场景_分块矩阵乘与原子累加阶段.png" width="100%">
</p>
<p align="center">
Figure 3: Block matrix multiplication and atomic accumulation phase, mode 0 diagram
</p>

#### 1.4 LeakyRelu and Result Write-back Phase (Mode 2, AIV waits for AIC)
Within each AI Core, the 2 AIV cores must wait for the AIC to complete block matrix multiplication and atomic accumulation before performing LeakyRelu operation on the C matrix blocks.

Specifically, split the accumulated C matrix along the M axis into 16 parts, distribute to 16 AIV cores for LeakyRelu operation respectively. As shown in Figure 4, use inter-core synchronization mode 2 (2 AIV cores wait for one AIC within a single AI Core) according to the above description.

The corresponding code segment:

        // Mode 2, within each AICore, 2 AIV cores wait for AIC
        AscendC::CrossCoreWaitFlag(SYNC_AIC_AIV_FLAG);

        // Execute LeakyRelu operation
        float alpha = 0.001;
        ...
        AscendC::LeakyRelu(reluCLocal, cLocal, alpha, C_AIV_BLOCKS_LENGTH);
        ...

<p align="center">
  <img src="figures/融合场景_LeakyRelu运算与结果回写阶段.png" width="100%">
</p>
<p align="center">
Figure 4: LeakyRelu operation and result write-back phase, mode 2 diagram
</p>

### 2. Pure Vector Computation Scenario

#### 2.1 Comparison between Mode 0 and Mode 1
This example sets NUM_BLOCKS to 8 (8 AI Cores). Each AI Core has AIC:AIV ratio of 2, meaning the example runs 8 AIC cores and 16 AIV cores total. AIV BlockIdx ranges from 0 to 15.

As shown in Figure 5, the computation logic for mode 0 and mode 1 is almost identical. The difference lies in the number of AIV cores participating in synchronization: for mode 0, all 16 AIV cores participate in synchronization; for mode 1, only 2 AIV cores (BlockIdx=2 and 3) in the second AI Core participate in synchronization. Therefore, the next section describes the overall logic of mode 0 in detail, and mode 1 is not described in detail.

<p align="center">
  <img src="figures/纯aiv_模式1和模式0的区别.png" width="100%">
</p>
<p align="center">
Figure 5: Pure AIV scenario, mode 0 computation logic diagram
</p>

#### 2.2 Overall Logic of Mode 0
The GM used in this example consists of two blocks: one for storing input data (initialDataGm), and one for storing accumulated results from all AIV cores (atomicResultGm).

As shown in Figure 6, the overall logic of mode 0 includes the following steps:

(1) Each AIV moves data with all values of 1 from initialDataGm to UB. This is the PIPE_MTE2 pipeline operation.

(2) Each AIV performs vector computation on UB data: multiply by the corresponding BlockIdx of each core through Muls instruction. This is the PIPE_V pipeline operation.

(3) Step 3 corresponding code segment is as follows. atomicResultGm stores the accumulated result after all 16 AIV cores complete data movement. Call CrossCoreSetFlag and CrossCoreWaitFlag interfaces to implement synchronization control: ensure all 16 AIV cores complete PIPE_MTE3 movement instructions before executing instructions after CrossCoreWaitFlag.

         // Enable atomic accumulation for UB to GM movement: moved data to atomicResult accumulates with original value and overwrites
        AscendC::SetAtomicAdd<float>(); 
        // DataCopy is PIPE_MTE3 pipeline operation
        AscendC::DataCopy(atomicResultGm, xLocal, this->blockLength);   
        // When this AIV completes preceding PIPE_MTE3 (DataCopy) pipeline operation, notify other AIV cores that this AIV has completed
        AscendC::CrossCoreSetFlag<0, PIPE_MTE3>(0);  
        // Block this AIV from continuing to execute instructions until all other AIV cores complete PIPE_MTE3 pipeline operation, then unblock and continue
        AscendC::CrossCoreWaitFlag(0); 

After the above synchronization completes, atomicResultGm now contains the accumulated result from all 16 AIV cores' vector computation. At this point, move data from atomicResultGm to a certain AIV, use DumpTensor to print data, check whether the accumulated result meets expectations, and finally move the result from this AIV back to atomicResultGm.

If the synchronization insertion in the previous step is incorrect, the data moved from atomicResultGm to AIV may be the accumulated result from only some AIV cores' vector computation, causing inaccurate printed results and results written back to atomicResultGm.

        if (AscendC::GetBlockIdx() == 0) {
            AscendC::DataCopy(yLocal, atomicResultGm, this->blockLength);   // PIPE_MTE2
            AscendC::printf("============== In PrintTensor Process AIV %d ==============", AscendC::GetBlockIdx());
            AscendC::DumpTensor(yLocal, AscendC::GetBlockIdx(), this->blockLength);
            AscendC::DataCopy(atomicResultGm, yLocal, this->blockLength);
            return;
        }
<p align="center">
  <img src="figures/纯aiv_模式0示意图.png" width="100%">
</p>
<p align="center">
Figure 6: Pure AIV scenario, mode 0 computation logic diagram
</p>

### 3. Notes

#### 3.1 Cube-Vector Fusion Computation Scenario
(1) In the Cube-Vector fusion computation scenario, the fusion operator (configured with __mix__(1,2)) requires code isolation between AIV and AIC cores using ASCEND_IS_AIV/ASCEND_IS_AIC.
```
KernelMmad op;
if ASCEND_IS_AIC {
    op.InitAIC(A, B, c);
    op.ProcessAIC();
} 
if ASCEND_IS_AIV {
    op.InitAIV(a, b, A, B, c);
    op.ProcessAIV();
}
```
(2) GetBlockIdx (obtain current core index) has different value ranges for AIC and AIV. Its values depend on the logical core count set for the operator and the AIC-to-AIV ratio in one AI Core. In this example, NUM_BLOCKS=8 and AIC-to-AIV ratio is 1:2, so GetBlockIdx ranges are 0-7 for AIC and 0-15 for AIV.

(3) The example uses static tensor programming paradigm and requires manual insertion of intra-core synchronization. Additionally, in static Tensor programming mode, developers must manually call the InitSocState() interface to initialize global state registers.

#### 3.2 Pure Vector Computation Scenario
(1) When using CrossCoreSetFlag and CrossCoreWaitFlag inter-core synchronization interfaces, even in pure Vector computation scenarios, the kernel function cannot use the __vector__ modifier.

This example kernel function uses __mix__(1,2) modifier. However, in pure Vector scenarios, since only vector computation executes, you must use the ASCEND_IS_AIV macro to ensure the program runs only on AIV cores. Otherwise, the program will hang.
```
if ASCEND_IS_AIV {
        op.Init(x, z, dataLength);
        op.Process();
}
```
(2) Mode 1 requires that the two AIV cores participating in synchronization must belong to the same AI Core. Otherwise, the program will hang. In this example, the two AIV cores participating in synchronization have GetBlockIdx=2 and 3, both belonging to the second AI Core (index starting from 1). If you change GetBlockIdx to 3 and 4 (belonging to two different AI Cores), the program will hang.

(3) The example uses static tensor programming paradigm and requires manual insertion of intra-core synchronization. Additionally, in static Tensor programming mode, developers must manually call the InitSocState() interface to initialize global state registers.

## Build and Run

Execute the following steps in the example root directory to build and run the example.

- Configure Environment Variables  
  Select the appropriate command based on the [installation method](../../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on your current environment.
  - Default path, root user installs CANN package
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installs CANN package
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, install CANN package
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
    
- Example Execution
  ```bash
  SCENARIO_NUM=0  # Set scenario number (values: 0, 1, 2)
  mkdir -p build && cd build;      # Create and enter build directory
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # Build project
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM   # Generate test input data
  ./demo                           # Execute the compiled executable program
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # Verify output correctness
  ```

  For CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.
  
  Example:
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # CPU debug mode
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU simulation mode
  ```
  > **Note:** Clear the cmake cache before switching build modes. Execute `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build Option Description

  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU run, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `0` (default), `1`, `2` | Scenario number: 0 (pure Vector mode 0), 1 (pure Vector mode 1), 2 (Cube+Vector fusion) |

- Execution Result

  The following output indicates successful accuracy verification.
  ```bash
  test pass!
  ```