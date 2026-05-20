# Aclrtc Runtime Compilation + printf Print Example

## Overview

This example demonstrates using **Aclrtc (runtime compilation)** interfaces to compile kernel function source code on the Host side at runtime and execute it. The kernel function prints output results through printf.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── rtc_hello_world
│   ├── CMakeLists.txt           // Build project file
│   ├── rtc_hello_world.cpp      // Host side code (containing RTC kernel function source code string)
```

## Example Description

- Example Implementation:
  - Kernel Implementation  
    The kernel function source code is embedded in Host code as a string and prints output results through printf.

  - Invocation Implementation  
    Call aclrtc interface series to compile and execute kernel function at runtime. The complete chain:

    - **Compilation Phase**
      1. `aclrtcCreateProg` — Create compilation program object, pass kernel function source code string
      2. `aclrtcAddNameExpr` — Register kernel function name to export (with template parameters, for example `Kernel::add_custom<float>`)
      3. `aclrtcCompileProg` — Execute runtime compilation, pass `--npu-arch` through options to specify NPU architecture
      4. `aclrtcGetBinDataSize` / `aclrtcGetBinData` — Get binary size and data of compilation artifact (device ELF)
      5. `aclrtcGetLoweredName` — Get mangled name of compiled kernel function for subsequent lookup

    - **Loading Phase**
      6. `aclrtBinaryLoadFromData` — Load compilation artifact binary to device (marked as AI Core executable through `ACL_RT_BINARY_MAGIC_ELF_AICORE`)
      7. `aclrtBinaryGetFunction` — Get kernel function handle from loaded binary (`funcHandle`)

    - **Parameter Configuration Phase**
      8. `aclrtKernelArgsInit` — Initialize kernel function parameter handle
      9. `aclrtKernelArgsAppend` — Append parameters one by one (kernel function is `__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z`, corresponding to three Device memory pointers)
      10. `aclrtKernelArgsFinalize` — Complete parameter configuration

    - **Execution Phase**
      11. `aclrtLaunchKernelWithConfig` — Launch kernel function, specify block count, stream and other execution configurations

    - **Resource Cleanup**
      12. `aclrtcDestroyProg` — Destroy compilation program object

    Data generation and accuracy verification are completed in Host side C++ without external script dependencies.

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
  mkdir -p build && cd build;                                               # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # Build project
  ./demo                                                                    # Execute the compiled executable program
  ```

  > [!WARNING] Note
  Aclrtc supports only NPU run mode, not CPU debug or NPU simulation mode.

- Execution Result  
  The following output indicates successful execution.
  
  ```bash
  [AIV Block 0/8] Hello World!!!
  [AIV Block 1/8] Hello World!!!
  [AIV Block 2/8] Hello World!!!
  [AIV Block 3/8] Hello World!!!
  [AIV Block 4/8] Hello World!!!
  [AIV Block 5/8] Hello World!!!
  [AIV Block 6/8] Hello World!!!
  [AIV Block 7/8] Hello World!!!
  ```