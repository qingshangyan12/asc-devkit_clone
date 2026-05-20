# Aclrtc Runtime Compilation + Static Tensor Programming Add Example

## Overview

This example demonstrates using Ascend C **Aclrtc (runtime compilation)** mode to dynamically compile kernel function source code on the Host side and execute it. The kernel function uses **static Tensor programming paradigm** (LocalMemAllocator + SetFlag/WaitFlag event synchronization). The kernel function source code is embedded in Host code as a string and compiled and executed through aclrtc API at runtime.

Unlike traditional custom operator projects, Aclrtc does not require pre-compiling operator packages (.run), making it suitable for quick verification and prototype development.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── rtc_template_add
│   ├── CMakeLists.txt                   // Build project file
│   ├── data_utils.h                     // Data generation, accuracy verification utility functions + CHECK_ACL macro
│   └── rtc_template_add.cpp      // Host side code (containing RTC kernel function source code string)
```

## Example Description

- Example Functionality:  
  Add computation formula:

  ```
  z = x + y
  ```

- Example Specifications:
  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">Kernel::add_custom&ltfloat&gt</td></tr>
  </table>

- Example Implementation:

  - Kernel Implementation  
    The kernel function source code is embedded in Host code as a string. Complete memory management through LocalMemAllocator interface, complete event synchronization through SetFlag/WaitFlag interface, and implement the complete flow of data movement, computation, and result write-out.

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
      9. `aclrtKernelArgsAppend` — Append parameters one by one (kernel function is `GM_ADDR x, GM_ADDR y, GM_ADDR z`, corresponding to three Device memory pointers)
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
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # Build project, supports dav-3510, dav-2201
  ./main                                                                    # Execute the compiled executable program
  ```

- Build Option Description

  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

  > [!WARNING] Note
  Aclrtc supports only NPU run mode, not CPU debug or NPU simulation mode.

- Execution Result  
  The following output indicates successful accuracy verification.
  ```bash
  test pass!
  ```