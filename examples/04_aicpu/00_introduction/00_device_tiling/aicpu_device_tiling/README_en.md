# AI CPU Operator Tiling Offload Sample Introduction
## Overview

This sample introduces how to use AI CPU operators for tiling offload computation.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure
```
├── aicpu_device_tiling
│   ├── CMakeLists.txt                     // Build project file
│   ├── aicore_kernel.asc                  // AI Core operator implementation
│   ├── kernel_args.h                      // Tiling structure header file
│   ├── main.asc                           // AI CPU operator and AI Core operator invocation
│   ├── aicpu_tiling.aicpu                 // AI CPU operator implementation
│   └── README.md                          // Sample documentation
```

## Sample Description
- In main.asc, both AI CPU operators and AI Core operators use the kernel launch operator <<<...>>> for invocation. The AI CPU operator passes the tiling computation result to the AI Core operator.
- The AI CPU operator and AI Core operator launch on different streams. In this sample, they are aicpu_stream and aicore_stream respectively. Events record the tasks submitted on the streams. Use aclrtRecordEvent to record an event in a specified stream, and use aclrtStreamWaitEvent to block the specified stream until the specified event completes.

## Build and Run
Execute the following steps in the root directory of this sample.
- Configure environment variables  
  Configure environment variables according to the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development toolkit package on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. If no installation directory is specified, the default installation is under `/usr/local/Ascend`.

- Sample execution

  Execute the following commands in this sample directory.
  ```bash
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # Build project
  ./demo                           # Execute the compiled executable program to run the sample
  ```

- Build option description

  | Option　　　　　 | Available Values　　　　　　　　　　　| Description　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution result

  The execution result is as follows, indicating successful execution:
  Among them, `__mix__(1, 2)` starts 1 Cube execution unit and 2 Vector execution units, so the `Hello World` log prints 3 times.

  ```bash
  MyAicpuKernel inited
  MyAicpuKernel inited type 1 mode 2 len 4 end!
  Hello World: int mode 2 len 4 m 10.
  Hello World: int mode 2 len 4 m 10.
  Hello World: int mode 2 len 4 m 10.
  ```