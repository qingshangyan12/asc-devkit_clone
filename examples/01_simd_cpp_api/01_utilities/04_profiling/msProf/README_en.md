# Profiling Sample Based on MatmulLeakyRelu Operator

## Overview

This sample demonstrates how to collect on-board performance data using msProf based on the MatmulLeakyRelu operator. Users can quickly identify software and hardware performance bottlenecks based on the output performance data, improving operator performance analysis efficiency.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── matmul_leakyrelu
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── matmul_leakyrelu.asc    // Ascend C operator implementation and invocation sample
│   └── scripts
│       ├── gen_data.py         // Input data and golden data generation script
│       └── verify_result.py    // Golden data comparison file
```

## Operator Description
  The MatmulLeakyRelu operator implements matrix multiplication (Matmul) combined with LeakyRelu activation function. For details, see the [MatmulLeakyRelu Operator Details](../../../00_introduction/03_fusion_operation/matmul_leakyrelu_basic_api/README_en.md) section.

## msProf Tool Introduction
msProf is a single operator performance analysis tool. It includes two usage modes: msprof op and msprof op simulator. This tool helps users locate operator memory, operator code, and operator instruction anomalies, enabling comprehensive operator tuning. It currently supports performance data collection and automatic parsing based on different run modes (on-board or simulation) and different file formats (executable files or operator binary .o files).

- On-Board Performance Collection

  On-board performance collection directly measures the operator execution time on Ascend AI processors. This method is suitable for quickly identifying operator performance issues in board environments.

  Execute operator tuning using msprof op based on an executable demo:
  ```
  msprof op ./demo
  ```

  - Performance Data Description
    After the command completes, a folder named `OPPROF_{timestamp}_XXX` is generated in the default directory. The performance data folder structure is as follows:

    ```bash
    ├──dump                       # Raw performance data, users need not focus on this
    ├──ArithmeticUtilization.csv  # Cube/vector instruction cycle ratio
    ├──L2Cache.csv                # L2 Cache hit rate, affects MTE2, recommend planning data movement logic reasonably to increase hit rate
    ├──Memory.csv                 # UB, L1 and main memory read/write bandwidth rate
    ├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth rate
    ├──MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth rate
    ├──OpBasicInfo.csv            # Operator basic information
    ├──PipeUtilization.csv        # Collection of compute unit and transfer unit time and ratio
    ├──ResourceConflictRatio.csv  # Ratio of bank group, bank conflict and resource conflict on UB among all instructions
    └──visualize_data.bin         # MindStudio Insight presentation file
    ```

For more msProf tool usage, see the [MindStudio Tools](https://www.hiascend.com/document/redirect/CannCommercialToolOpDev) operator tuning (msProf) section.


## Build and Run

Execute the following steps in the sample root directory to build and run the operator.

- Configure Environment Variables

  Select the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
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

- Run Sample
  ```bash
  mkdir -p build && cd build;           # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                     # Build project
  python3 ../scripts/gen_data.py        # Generate test input data
  msprof op ./demo                      # Execute operator tuning via msprof op based on executable demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output result correctness, confirm algorithm logic correctness
  ```

- Build Options

| Option | Available Values | Description |
|------|--------|------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution Result
  The following output indicates successful accuracy comparison:
  ```bash
  test pass!
  ```