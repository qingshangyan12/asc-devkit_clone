# Matmul IBShareAB Feature Example

## Overview

This example calls the Matmul high-level API to enable IBShare functionality, reusing the same A matrix or B matrix data on L1 Buffer. This example demonstrates the scenario where both A matrix and B matrix are reused. Enabling this feature reduces data movement overhead. Two scenarios are supported, selected through environment variable.
    <table>
 	<tr>
 		<td>scenarioNum</td>
 		<td>Scenario Type</td>
 	</tr>
 	<tr>
 		<td>1</td>
 		<td>Enable AB matrix IBShare (A and B matrices not split)</td>
 	</tr>
 	<tr>
 		<td>2</td>
 		<td>Disable AB matrix IBShare (A and B matrices split along K axis)</td>
 	</tr>
 </table>

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products
- Atlas Inference Series Products AI Core

## Directory Structure

```
├── matmul_ibshareAB
│   └── scripts
│       ├── gen_data.py         // Input data and golden data generation script file
│       └── verify_result.py    // Golden value comparison file
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── figures                 // Diagrams
│   └── matmul_ibshareAB.asc    // Ascend C example implementation & invocation example
```

## Example Description

- Example Functionality:  
  Enable A and B matrix IBShare functionality when calling Matmul high-level API. During computation, the A and B matrix data on L1 Buffer used by the two AIV cores corresponding to the same AIC in each iteration are identical.

  When both A matrix and B matrix enable IBShare, no splitting along k column for computation. When neither enables IBSHARE, splitting along k column for computation. Comparing runtime of both scenarios demonstrates the performance improvement of this feature.

  Data processing diagram for enabled AB matrix ibshare scenario (A and B matrices not split):![alt text](./figures/matmul_ABshare.png)  
  Data processing diagram for disabled AB matrix ibshareAB scenario (A and B matrices split):![alt text](./figures/matmul_noABshare.png)  


- Example Specifications:  
  In this example: M = 128, N = 256, K = 384.
  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">matmul_ABshare_custom</td></tr>
  </table>

- Example Implementation: 

  - Kernel Key Steps  
    - Create Matmul object:  
        Configure A and B matrix IBSHARE parameters as true or false based on SCENARIO_NUM build option in CMakeLists.
        ```cpp
        #if SCENARIO_NUM == 1
        constexpr bool isABshare = true;
        #else
        constexpr bool isABshare = false;
        #endif

        Matmul<
            MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType, false, LayoutMode::NONE, isABshare>,
            MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType, false, LayoutMode::NONE, isABshare>,
            MatmulType<AscendC::TPosition::VECIN, CubeFormat::ND, CType>> matmulObj;
        ```


  - Invocation Implementation  
    Use the kernel invocation operator `<<<>>>` to call the kernel function.

## Build and Run

Execute the following steps in the example root directory to build and run the example.
- Configure Environment Variables  
  Select the appropriate command based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on your current environment.
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
  SCENARIO=1                                                                    # Set scenario number
  mkdir -p build && cd build;                                                   # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO ..;make -j; # Build project, default npu mode
  python3 ../scripts/gen_data.py                                                # Generate test input data
  ./demo                                                                        # Execute the compiled executable program
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin       # Verify output correctness, confirm algorithm logic correctness
  ```

  For CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=1 ..;make -j; # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=1 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Clear the cmake cache before switching build modes or scenarios. Execute `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build Option Description

  | Option　　　　　 | Available Values　　　　　　　　　　　| Description　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU run, CPU debug, NPU simulation　　　　　　　　　　　　　　　　　　　　　　　　 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`, `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1` (default), `2` | Scenario number: 1=enable AB matrix IBShare, 2=disable AB matrix IBShare |

- Execution Result

  The following output indicates successful accuracy verification:
  ```bash
  test pass!
  ```