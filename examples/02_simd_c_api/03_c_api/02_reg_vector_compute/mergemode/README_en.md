# mergemode Example

## Overview
This example verifies the behavior characteristics of the MaskMergeMode::MERGING mode based on the C API programming interface, demonstrating the mechanism where inactive mask bits preserve the original value of dst vector data register. The asc_copy API is used for verification.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure
```
├── mergemode
│   ├── scripts
│   │   ├── gen_data.py                // Input data and ground truth generation script
│   ├── CMakeLists.txt                 // Build configuration file
│   ├── data_utils.h                   // Data read/write functions
│   ├── mergemode.asc                  // Ascend C example implementation & invocation example
│   └── README.md                      // Example description
```

## Example Description
- Example functionality:
  Verifies the `MERGING` mode: when the mask is inactive, the corresponding bits of dst vector data register preserve their original values rather than participating in computation. The input is 200 negative numbers (not a multiple of VL), and the output is 2 (the original dst vector data register value preserved at inactive bits).

  **Verification principle**
  - Input: 200 float negative numbers (-100 to -1), VL=256Byte, 4 repeats in total
  - repeat 0-3: Before each repeat, Duplicate initializes dst vector data register=2
  - Max MERGING: active bits=max(negative, negative)=negative, inactive bits=preserve dst vector data register original value=2
  - Repeat 3 (200 elements, the last repeat has only 8 active bits):
    - yAddr[192:200] = negative numbers (8 active bits)
    - yAddr[200:256] = 2 (56 inactive bits, MERGING preserves dst vector data register original value)
  - ReduceMax(yAddr[0:256]) = 2, verifying that the MERGING mode indeed preserves the dst vector data register original value at inactive bits

  - Example specifications:
    | Item | name | shape | data type |
    | --- | --- | --- | --- |
    | Example Type (OpType) | AIV Example |  |  |
    | Example Input | x | [1, 128] | float |
    | Example Input | y | [1, 128] | float |
    | Example Output | z | [1, 128] | float |
    | Kernel Function Name | kernel_mergemode |  |  |

  - Example implementation:
    1. **MaxMergeModeVF**:
       - Each loop iteration first calls `Duplicate(dst vector data register, 2)` to initialize dst vector data register
       - Uses `UpdateMask` to handle data that is not a multiple of VL
       - Uses `Max`: active bits compute the max value, inactive bits preserve dst vector data register original value=2
       - Uses `StoreAlign(allMask)` to write the entire VL, verifying that the inactive bits of dst vector data register are indeed 2
    2. **ReduceMaxVF**: Performs ReduceMax reduction on yAddr[0:256]. The result should be 2 (the value preserved at inactive bits)
    3. Output is 8 floats aligned to 32B. The ReduceMax result is in the first element, with an expected value of 2

    - Invocation implementation
      Uses the kernel invocation syntax `<<<>>>` to call the kernel function, launching 1 core.

## Build and Run
Run the following steps in the root directory of this example to build and run it.
- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.
  ```bash
  mkdir -p build && cd build;                                                    # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                           # Build the project (default npu mode)
  python3 ../scripts/gen_data.py                                                 # Generate test input data
  ./demo                                                                         # Run the compiled executable to execute the example
  ```

  To use NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Examples:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU simulation mode
  ```

  > **Notice:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build option description

  | Option | Values | Description |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution result

  The following execution result indicates that the precision comparison is successful.
  ```bash
  test pass!
  ```
