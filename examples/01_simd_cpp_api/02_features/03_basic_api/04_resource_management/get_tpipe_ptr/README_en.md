# GetTPipePtr Example

## Overview

This example uses the GetTPipePtr interface to obtain a global TPipe pointer and perform TPipe-related operations through that pointer.

> **Note:** This example applies only to programming models based on TPipe and TQue.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── get_tpipe_ptr
│   ├── CMakeLists.txt          // Build project file
│   └── get_tpipe_ptr.asc       // Ascend C example implementation & invocation example
```

## Example Description

- Example Functionality

  When creating a TPipe object, the object initialization sets a globally unique TPipe pointer. This example calls the GetTPipePtr interface to obtain that pointer. The kernel function can perform TPipe-related operations without explicitly passing the TPipe pointer. The following shows code snippets with and without calling the GetTPipePtr interface.

  **Calling GetTPipePtr Interface**

  ```cpp
  class KernelAdd {
      // No TPipe member variable

      __aicore__ inline void Init(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z, uint32_t totalLength, uint32_t tileNum)
      {
          // Call GetTPipePtr to obtain TPipe pointer and use it
          GetTPipePtr()->InitBuffer(inQueueX, BUFFER_NUM, this->tileLength * sizeof(float));
      }
  };

  __global__ __vector__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z, AddCustomTilingData tiling)
  {
      // No need to explicitly pass TPipe pointer
      KernelAdd op;
      op.Init(x, y, z, tiling.totalLength, tiling.tileNum);
      op.Process();
  }
  ```

  **Not Calling GetTPipePtr Interface (Explicitly Passing TPipe Pointer to Kernel Function)**

  ```cpp
  class KernelAdd {
      AscendC::TPipe* pipe;  // Requires TPipe pointer member variable

      __aicore__ inline void Init(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z, uint32_t totalLength, uint32_t tileNum, AscendC::TPipe* pipeIn)
      {
          pipe = pipeIn;
          pipe->InitBuffer(inQueueX, BUFFER_NUM, this->tileLength * sizeof(float));
      }
  };

  __global__ __vector__ void add_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z, AddCustomTilingData tiling)
  {
      // Must explicitly pass TPipe pointer
      AscendC::TPipe pipe;
      KernelAdd op;
      op.Init(x, y, z, tiling.totalLength, tiling.tileNum, &pipe);
      op.Process();
  }
  ```

- Example Specifications

  <table>
    <tr>
      <td align="center">Category</td>
      <td align="center">name</td>
      <td align="center">shape</td>
      <td align="center">data type</td>
      <td align="center">format</td>
    </tr>
    <tr>
      <td rowspan="2" align="center">Example Input</td>
      <td align="center">x</td>
      <td align="center">[8, 2048]</td>
      <td align="center">float</td>
      <td align="center">ND</td>
    </tr>
    <tr>
      <td align="center">y</td>
      <td align="center">[8, 2048]</td>
      <td align="center">float</td>
      <td align="center">ND</td>
    </tr>
    <tr>
      <td align="center">Example Output</td>
      <td align="center">z</td>
      <td align="center">[8, 2048]</td>
      <td align="center">float</td>
      <td align="center">ND</td>
    </tr>
    <tr>
      <td align="center">Kernel Function Name</td>
      <td colspan="4" align="center">get_tpipe_ptr_custom</td>
    </tr>
  </table>

- Example Implementation

  - Kernel Implementation

    - Call the GetTPipePtr interface to obtain the global TPipe pointer.

    - Call the TPipe::InitBuffer interface to allocate memory space for TQue.

    - Call the DataCopy basic API to move data from GM (Global Memory) to UB (Unified Buffer).

    - Call the Add interface to perform addition on two input tensors.

    - Call the DataCopy basic API to move computation results from UB (Unified Buffer) to GM (Global Memory).

  - Invocation Implementation

    Use the kernel invocation operator `<<<>>>` to call the kernel function.

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
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # Build project, default npu mode
  ./demo                           # Execute the compiled executable program
  ```

  For CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.
  
  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Clear the cmake cache before switching build modes. Execute `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build Option Description

  | Parameter | Description | Options | Default |
  |------|------|---------|--------|
  | CMAKE_ASC_RUN_MODE | Run mode | npu, cpu, sim | npu |
  | CMAKE_ASC_ARCHITECTURES | NPU hardware architecture | dav-2201, dav-3510 | dav-2201 |

- Execution Result

  The following output indicates successful accuracy verification:
  ```bash
  [Success] Case accuracy is verification passed.
  ```