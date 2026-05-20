# Data Movement with On-the-fly Atomic Operation for Deterministic Computation Example

## Overview

This example first introduces the necessity and specific implementation approach for deterministic computation in data movement with on-the-fly atomic operation scenarios. It then demonstrates how to apply this approach in single AIV core, multi-AIV core, and multi-AIC core scenarios.

## Supported Products
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure
```
├── set_atomic_determine_compute
│   ├── figures                      // README image resources directory
│   ├── scripts
│   │   ├── gen_data.py         // Input data and golden data generation script
│   │   └── verify_result.py    // Verification script for comparing output data with golden data
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── set_atomic_add_multi_aic.h    // Multi-AIC core synchronization implementation
│   ├── set_atomic_add_multi_aiv.h    // Multi-AIV core synchronization implementation
│   ├── set_atomic_add_single_aiv.h   // Single AIV core synchronization implementation
│   ├── set_atomic_determine_compute.asc  // Ascend C example implementation & invocation example
```

## 1. Deterministic Computation

### 1.1 Necessity of Deterministic Computation

Floating-point accumulation does not satisfy mathematical associativity and commutativity. Different accumulation orders may produce different computation results. You can verify this with the following data:

```python
x = 1.0
y = 1e16
z = -1e16

# Different accumulation orders produce different results
left = (x + y) + z    # First add x and y, then add with z
right = x + (y + z)   # First add y and z, then add with x
```

Due to floating-point precision limitations, `(1.0 + 1e16) + (-1e16)` and `1.0 + (1e16 + (-1e16))` produce different results:
- The former: `1.0 + 1e16` loses precision, the result is approximately `1e16`, then adding `-1e16` gives `0.0`
- The latter: `1e16 + (-1e16)` equals exactly `0.0`, then adding `1.0` gives `1.0`

Therefore, in parallel computing environments, you must use synchronization mechanisms to ensure a fixed accumulation order and guarantee result determinism. Deterministic computation refers to a computation process that always produces completely consistent output results under the same input conditions, regardless of execution count or execution environment changes. Deterministic computation guarantees system stability and experimental verifiability.

### 1.2 Overview of Deterministic Computation

To introduce the problem of non-deterministic computation in atomic operation scenarios, we construct the following common deterministic computation scenario: First, complete GM initialization through single-group floating-point data movement. Then, start atomic accumulation operation. Finally, accumulate multiple groups of floating-point data on GM through multiple data movements. Specific pseudocode:

```
①Move data data0 to GM;    // Data movement, overwrites GM original random value, expected GM data is data0 
②SetAtomicAdd();         // Enable atomic accumulation, subsequent movements from UB/L0C/L1 to GM execute atomic accumulation 
③Move data1 to GM;    // Data movement with on-the-fly atomic operation, expected GM data is data0 + data1 
④Move data2 to GM;    // Data movement with on-the-fly atomic operation, expected GM data is data0 + data1 + data2 
⑤Move data3 to GM;    // Data movement with on-the-fly atomic operation, expected GM data is data0 + data1 + data2 + data3
```

As shown in the figure below, developer expected result: instruction emission order strictly corresponds to actual instruction execution order. Execute this code segment multiple times, no matter how many times executed, the final GM data is always data0 + data1 + data2 + data3, the result is completely consistent, achieving deterministic computation.

![Deterministic computation scenario](figures/确定性计算场景，GM上数据变化过程.png)

Figure 1: Deterministic computation scenario, data change process on GM

However, the actual situation is that without developer intervention, the instruction execution order may change each time the program runs, eventually causing GM data to be inconsistent with expected results. Below lists two possible instruction execution orders and their corresponding execution processes.

#### 1.2.1 Non-Deterministic Computation, Result 1

![Non-deterministic computation scenario 1](figures/非确定性计算场景1，GM上数据变化过程.png)

Figure 2: Non-deterministic computation scenario 1, data change process on GM

As shown in the figure, the instruction execution process in this scenario:
1. Initial state, GM data is: random value;
2. Move data0 to GM, GM data is initialized to: data0;
3. Execute SetAtomicAdd, enable atomic accumulation for subsequent movement instructions, GM data is: data0;
4. Three movement instructions with on-the-fly atomic operations are reordered, actual execution order is "move data2 → move data3 → move data1", final GM data is: data0 + data2 + data3 + data1.

**Cause of Non-Deterministic Computation 1**:
Movement instructions with on-the-fly atomic operations are reordered. Since floating-point addition does not satisfy associativity, that is (a+b)+c != a+(b+c), the final GM data data0 + data2 + data3 + data1 deviates from expected data0 + data1 + data2 + data3.

Reordering of movement instructions with on-the-fly atomic operations causes final result deviation under the following three prerequisite conditions:
- Atomic operation type is atomic accumulation (max and min operations satisfy associativity)
- Atomic operation data type is floating-point (integer addition satisfies associativity)
- Movement instructions with on-the-fly atomic operations reach 3 or more (floating-point addition satisfies commutativity)

#### 1.2.2 Non-Deterministic Computation, Result 2

![Non-deterministic computation scenario 2](figures/非确定性计算场景2，GM上数据变化过程.png)

Figure 3: Non-deterministic computation scenario 2, data change process on GM

As shown in the figure, the instruction execution process in this scenario:
1. Initial state, GM data is: random value;
2. Execute SetAtomicAdd, enable atomic accumulation for subsequent movement instructions, GM data is: random value;
3. Execute two movement instructions with on-the-fly atomic operations sequentially, execution order is "move data1 → move data2", GM data is: random value + data1 + data2;
4. Move data0 to GM, accumulated result on GM is overwritten by data0, GM data is: data0;
5. Finally execute data3 movement, final GM data is: data0 + data3.

**Cause of Non-Deterministic Computation 2**:
Reordering between normal movement instructions before enabling atomic accumulation and movement instructions with atomic operations causes data on GM that has completed atomic operations to be incorrectly overwritten by data0, producing non-deterministic computation results.

This type of reordering causes result deviation without any prerequisite conditions. Developers do not need to distinguish atomic operation type, atomic operation data type, or consider whether the number of movement instructions with on-the-fly atomic operations reaches 3 or more.

### 1.3 Deterministic Computation Implementation Approach

Based on the two root causes of non-deterministic computation, the deterministic computation implementation approach addresses these two aspects. The core idea is to insert appropriate synchronization between instructions, so that relevant instructions execute in the expected determined order each time the program runs, guaranteeing that each program execution produces the same result. Specifically, this includes the following two aspects:

- Insert synchronization between movement instruction before enabling atomic accumulation and instruction enabling atomic operation
  As shown in pseudocode below, insert synchronization between instruction ① and ② to ensure GM initial value meets expectations before starting atomic operation.
- Insert synchronization between multiple movement instructions after enabling atomic accumulation operation
  Insert synchronization between instruction ③ and ④, and between ④ and ⑤ to ensure floating-point accumulation order meets expectations.

No developer-inserted synchronization is needed between the instruction enabling atomic operation and subsequent movement instructions.

#### 1.3.1 Intra-Core Synchronization Implementation

```cpp
// Entire atomic accumulation executes in same core, control 5 instructions execution order as "①→②→③→④→⑤" 
①Move data data0 to GM;    // Data movement, overwrites GM original random value, expected GM data is data0 
Intra-core synchronization 
②SetAtomicAdd();         // Enable atomic accumulation, subsequent movements from UB/L0C/L1 to GM execute atomic accumulation 
// No synchronization needed between instruction ② and ③ 
③Move data1 to GM;    // Data movement after enabling atomic accumulation, expected GM data is data0 + data1 
Intra-core synchronization 
④Move data2 to GM;    // Data movement after enabling atomic accumulation, expected GM data is data0 + data1 + data2 
Intra-core synchronization 
⑤Move data3 to GM;    // Data movement after enabling atomic accumulation, expected GM data is data0 + data1 + data2 + data3
```
Movement instructions and instruction enabling atomic operation have pipeline types shown in the table below. When the above instructions execute in the same core, developers insert [single pipeline synchronization](https://www.hiascend.com/document/detail/en/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0271.html) or [multi-pipeline synchronization](https://www.hiascend.com/document/detail/en/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0270.html) as needed. Refer to SCENARIO_NUM=1 branch in the example.
<table border="1" style="text-align: left;">
  <tr>
    <th style="padding: 8px;">Instruction Name</th>
    <th style="padding: 8px;">Pipeline Type</th>
  </tr>
  <tr>
    <td style="padding: 8px;">DataCopy</td>
    <td style="padding: 8px;">PIPE_MTE3</td>
  </tr>
  <tr>
    <td style="padding: 8px;">Fixpipe</td>
    <td style="padding: 8px;">PIPE_FIX</td>
  </tr>
  <tr>
    <td style="padding: 8px;">SetAtomicAdd/SetAtomicMax/SetAtomicMin</td>
    <td style="padding: 8px;">PIPE_S</td>
  </tr>
</table>

#### 1.3.2 Inter-Core Synchronization Implementation

As shown in pseudocode below, when the above instructions execute in different cores, you must replace intra-core synchronization with inter-core synchronization.

```cpp
// Entire atomic accumulation executes in 4 different cores, control 4 cores execution order as "core0→core1→core2→core3" 
if (GetBlockIdx == 0) { 
   Move data data0 to GM; 
   Inter-core synchronization 
} else if (GetBlockIdx == 1) { 
   Inter-core synchronization     
   SetAtomicAdd();          
   Move data1 to GM; 
   Inter-core synchronization    
} else if (GetBlockIdx == 2) { 
   Inter-core synchronization 
   SetAtomicAdd();          
   Move data2 to GM;   
   Inter-core synchronization  
} else if (GetBlockIdx == 3) { 
   Inter-core synchronization 
   SetAtomicAdd();          
   Move data3 to GM;    
}
```
Since no hardware synchronization interface exists for controlling execution order between different cores, inter-core synchronization in deterministic computation scenarios must be implemented through software. The software synchronization approaches differ for pure Vector examples, pure Cube examples, and Mix examples (containing both Vector and Cube computation). See the table below.
<table border="1" style="text-align: left;">
  <tr>
    <th style="padding: 8px;">Example Type</th>
    <th style="padding: 8px;">Software Synchronization Approach</th>
    <th style="padding: 8px;">Description</th>
  </tr>
  <tr>
    <td style="padding: 8px;" rowspan="2">Pure Vector Example</td>
    <td style="padding: 8px;">Approach 1: Use multiple pairs of IBSet and IBWait interfaces to achieve synchronization among multiple AIV cores. Refer to SCENARIO_NUM=2 branch in the example.</td>
    <td style="padding: 8px;">Approach 1 supports specifying some AIV cores to participate in synchronization and controlling each AIV execution order.</td>
  </tr>
  <tr>
    <td style="padding: 8px;">Approach 2: Use InitDetermineComputeWorkspace, NotifyNextBlock, and WaitPreBlock interfaces together to ensure all AIV cores execute in blockIdx ascending order. Refer to SCENARIO_NUM=4 branch in the example (to be supported).</td>
    <td style="padding: 8px;">Approach 2 requires all AIV cores to participate in synchronization with fixed execution order as blockIdx ascending.</td>
  </tr>
  <tr>
    <td style="padding: 8px;">Pure Cube Example</td>
    <td style="padding: 8px;">Implement inter-core synchronization through semaphores in GM. First establish synchronization between one pair of cores, then extend to synchronization among multiple cores. Refer to SCENARIO_NUM=3 branch in the example.</td>
    <td style="padding: 8px;">Consider multi-core data consistency when Scalar unit accesses GM.<br>The "core" here can be AIV or AIC.</td>
  </tr>
</table>
The figure below shows how two cores perform inter-core synchronization through semaphores in GM:

![One pair of cores software synchronization approach flowchart](figures/一对核之间软件同步方案流程图.png)

Figure 4: One pair of cores software synchronization approach flowchart

- After the previous core completes data movement or enables atomic operation, it writes value 1 to the semaphore in inter-core shared GM through Scalar unit, indicating its task is complete. The previous core also needs intra-core synchronization:
  - When multiple movement instructions exist in the previous core, insert intra-core synchronization 1 between them.
  - Before Scalar unit writes data to GM, ensure all preceding movement instructions have completed execution, so insert intra-core synchronization 2 between them.
- Before executing movement tasks, the current core continuously reads the semaphore value through Scalar unit. If the semaphore is not equal to 1, the current core enters blocking wait state. When detecting semaphore equals 1, the current core unblocks and starts executing its data movement or atomic operation. To ensure the current core does not execute movement instructions before semaphore equals 1, insert intra-core synchronization 3 before movement instructions.

#### 1.3.3 Scalar Unit Access to Semaphores on GM

Scalar unit access to semaphores on GM has two access methods:

1. **Access through DCache**
   Use GlobalTensor member functions GetValue and SetValue for operations. In this case, developers must manually call [DataCacheCleanAndInvalid](https://www.hiascend.com/document/detail/en/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0177.html) interface to ensure multi-core data consistency.

2. **Access without DCache**
   Use [WriteGmByPassDCache](https://www.hiascend.com/document/detail/en/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00089.html) and ReadGmByPassDCache. This approach guarantees multi-core data consistency without additional operations.

Performance difference between the two approaches: Access without DCache has poorer performance, but if GM data reads and writes are few, consider using the method without DCache.

Inter-core synchronization approach also needs to work with intra-core synchronization. The three intra-core synchronizations are explained below:

- **Intra-core synchronization 1 (optional)**: When multiple data movement instructions exist in the core, use this synchronization to ensure each movement operation executes strictly in order.
- **Intra-core synchronization 2 (required)**: Wait for all tasks in the previous core to complete before allowing Scalar unit to write 1 to the global memory semaphore.
- **Intra-core synchronization 3 (required)**: Wait for Scalar unit to detect semaphore updated to 1 before the current core starts subsequent task execution.

## 2. Example Description

### 2.1 Scenario Configuration Description
<table border="1" style="text-align: left;">
  <tr>
    <td>SCENARIO_NUM Value</td>
    <td>Business Scenario</td>
    <td>Kernel Function</td>
    <td>Synchronization Mode Used</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Deterministic computation within single AIV core</td>
    <td>set_atomic_add_single_aiv_custom</td>
    <td>PipeBarrier ensures order</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Deterministic computation among multiple AIV cores</td>
    <td>set_atomic_add_multi_aiv_custom</td>
    <td>SetFlag/WaitFlag AIV inter-core synchronization</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Deterministic computation among multiple AIC cores</td>
    <td>set_atomic_add_multi_aic_custom</td>
    <td>SetFlag/WaitFlag AIC inter-core synchronization</td>
  </tr>
</table>

### 2.2 Computation Formula and Example Specifications

All three scenarios use the same computation formula:

$$
z = src0 + src2 + src3 + src1
$$

Where:
- `src0` is the GM initial value vector (all zeros)
- `src1`, `src2`, `src3` are three input vectors participating in atomic accumulation
- Accumulation order is fixed: `src0` (initial) → +`src2` → +`src3` → +`src1`
- `z` is the final accumulated result

#### 2.2.1 SCENARIO_NUM=1 (Deterministic Computation within Single AIV Core)
- **Computation Method**: Execute atomic accumulation operations sequentially in fixed order within single AIV core
- **Synchronization Mechanism**: Use PipeBarrier to ensure each atomic operation completes before executing the next

- **Example Specifications**:
  <table border="1">
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">SetAtomicAddSingleAiv</td></tr>
  <tr><td rowspan="5" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src0</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">src1</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">src2</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">src3</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Core Count(numBlocks)</td><td colspan="4" align="center">1</td></tr>
  </table>

#### 2.2.2 SCENARIO_NUM=2 and SCENARIO_NUM=3 (Deterministic Computation among Multiple Cores)
The core logic of these two scenarios is identical. The difference is that accumulated data is distributed across multiple AIV cores or AIC cores. Compared to single core scenarios, multi-core scenarios require synchronization between AIV cores or AIC cores. Therefore, the input adds sync_buf parameter for storing inter-core synchronization semaphore on GM. Its initial value must be 0.

- **Example Specifications**:
  <table border="1">
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">SetAtomicAddMultiCore</td></tr>
  <tr><td rowspan="6" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src0</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">src1</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">src2</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">src3</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td align="center">sync_buf</td><td align="center">[256]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[8]</td><td align="center">float32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Core Count(numBlocks)</td><td colspan="4" align="center">4</td></tr>
  </table>

### 2.3 Notes

#### (1) When `SCENARIO_NUM=3`, the example does not support Ascend 950PR/Ascend 950DT.
When `SCENARIO_NUM=1` and `SCENARIO_NUM=2`, the example supports Ascend 950PR/Ascend 950DT.
When `SCENARIO_NUM=3`, the example calls `DataCopy` to move data from L1 to GM. However, under Ascend 950PR/Ascend 950DT architecture, the DataCopy interface does not support L1 Buffer to GM path. Therefore, when `SCENARIO_NUM=3`, the example does not support Ascend 950PR/Ascend 950DT. To support Ascend 950PR/Ascend 950DT, refer to the compatibility approach in [Basic API Migration Guide](https://www.hiascend.com/document/detail/en/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_compatibility_10_00005.html).

#### (2) When `SCENARIO_NUM=2`, the example does not support static tensor programming mode.
When SCENARIO_NUM=2, the example calls `IBSet` and `IBWait` for inter-core synchronization. However, these two interfaces internally require the TPipe framework for intra-core synchronization. Therefore, when SCENARIO_NUM=2, the example does not support static tensor programming mode.

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
  SCENARIO_NUM=1 # Default demonstration of deterministic computation within single AIV core
  mkdir -p build && cd build;   # Create and enter build directory
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=${SCENARIO_NUM};make -j;  # Build project
  python3 ../scripts/gen_data.py
  ./demo                        # Execute the compiled executable program
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output correctness, confirm algorithm logic correctness
  ```
  The following output indicates successful accuracy verification.
  ```bash
  test pass!
  ```