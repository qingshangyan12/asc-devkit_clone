# list_tensor_desc_input Example

## Overview

This example implements an AddN example based on the static Tensor programming model. It uses the ListTensorDesc structure to handle dynamic input parameters and combines static memory allocation with event synchronization mechanisms to coordinate data movement and computation tasks.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── list_tensor_desc_input
│   ├── CMakeLists.txt              // Build project file
│   └── list_tensor_desc_input.asc  // Ascend C example implementation & invocation example
```

## Example Description

- Example Functionality

  This example demonstrates the dynamic Tensor programming model using Add computation. It applies to the following scenarios:
  1. Multi-input parameter dynamic processing: Supports dynamic combination operations of multiple input tensors in the model (for example, multi-branch network structures).
  2. Memory pipeline optimization: Uses static double buffering and event synchronization mechanisms to implement pipeline parallelism of data movement and computation, reducing memory access latency.
  3. Multi-core parallel computation: Adapts to the multi-core architecture of AI processors and supports efficient distribution of large-scale tensor operations.

- Example Specifications

  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">AddN</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x (dynamic input parameter srcList[0])</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y (dynamic input parameter srcList[1])</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">list_tensor_desc_input_custom</td></tr>
  </table>

- Example Implementation

  - Kernel Implementation

    The dynamic input feature uses the ListTensorDesc structure to store input data information in kernel function parameters. Construct the TensorList data structure as shown below.
    ```cpp
    constexpr uint32_t SHAPE_DIM = 2;
    struct TensorDesc {
      uint32_t dim{SHAPE_DIM};
      uint32_t index;
      uint64_t shape[SHAPE_DIM] = {8, 2048};
    };

    constexpr uint32_t TENSOR_DESC_NUM = 2;
    struct ListTensorDesc {
      uint64_t ptrOffset;
      TensorDesc tensorDesc[TENSOR_DESC_NUM];
      uintptr_t dataPtr[TENSOR_DESC_NUM];
    } inputDesc;
    ```
    Combine the allocated Tensor parameters into a ListTensorDesc data structure as shown below.
    ```cpp
    inputDesc = {(1 + (1 + SHAPE_DIM) * TENSOR_DESC_NUM) * sizeof(uint64_t),
                {xDesc, yDesc},
                {(uintptr_t)xDevice, (uintptr_t)yDevice}};
    ``` 
    Parse the corresponding input parameters according to the passed data format as shown below.

    ```cpp
    AscendC::ListTensorDesc keyListTensorDescInit((__gm__ void*)srcList);
    __gm__ uint8_t* x = (__gm__ uint8_t*)keyListTensorDescInit.GetDataPtr<__gm__ uint8_t>(0);
    __gm__ uint8_t* y = (__gm__ uint8_t*)keyListTensorDescInit.GetDataPtr<__gm__ uint8_t>(1);
    ```

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