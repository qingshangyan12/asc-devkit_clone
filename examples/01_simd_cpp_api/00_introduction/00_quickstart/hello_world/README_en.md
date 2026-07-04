# HelloWorld Sample

## Overview

This sample demonstrates the basic process of running and verifying a kernel function on the NPU side using the <<<>>> kernel launch operator. The kernel function prints output results via printf.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|----------------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure

```
├── hello_world
│   ├── CMakeLists.txt      // Build project file
│   ├── hello_world.asc     // Ascend C sample implementation & sample call
│   └── README.md           // Sample documentation
```

## Build and Run

Execute the following steps in the root directory of this sample to build and run the sample.

- Configure Environment Variables

  Configure environment variables according to the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. If no installation directory is specified, it defaults to `/usr/local/Ascend`.

- Run the Sample

  Execute the following commands in this sample directory.
  ```bash
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # Build project (default npu mode)
  ./demo
  ```

  When using CPU debug or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  For example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  For detailed information about CPU debugging, refer to [06_cpu_debug sample](../../../01_utilities/06_cpu_debug).

  > **Note:** Clear the cmake cache before switching build modes. Execute `rm CMakeCache.txt` in the build directory, then run cmake again.

- Build Options

  | Option | Available Values | Description |
  |--------|------------------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution Results

  The following execution results indicate successful execution.
  



  
    ```bash
  cann 9.1.0 result：
  [AIV Block 0/8] Hello World!!!
  [AIV Block 1/8] Hello World!!!
  [AIV Block 2/8] Hello World!!!
  [AIV Block 3/8] Hello World!!!
  [AIV Block 4/8] Hello World!!!
  [AIV Block 5/8] Hello World!!!
  [AIV Block 6/8] Hello World!!!
  [AIV Block 7/8] Hello World!!!
  cann 9.0.0 result：
  Hello World!!!
  Hello World!!!
  Hello World!!!
  Hello World!!!
  Hello World!!!
  Hello World!!!
  Hello World!!!
  Hello World!!!
  ```

  The output consists of 8 lines, each corresponding to the print result of one AIV (AI Vector) Core. `[AIV Block X/8]` indicates the current core X (out of 8 cores total), showing that the kernel function has successfully executed on 8 AIV Cores.

- Note
  ```bash
  Installation steps for the Cann 9.1.0 environment
  1. wget https://ascend-cann.obs.cn-north-4.myhuaweicloud.com/CANN/20260701_newest/Ascend-cann-toolkit_9.1.0_linux-aarch64.run
  2. chmod +x Ascend-cann-toolkit_9.1.0_linux-aarch64.run
  3. ./Ascend-cann-toolkit_9.1.0_linux-aarch64.run --install --install-path=/home/developer/Ascend
  4. export cann_install_path=/home/developer/Ascend
  5. source ${cann_install_path}/cann/set_env.sh

  ```