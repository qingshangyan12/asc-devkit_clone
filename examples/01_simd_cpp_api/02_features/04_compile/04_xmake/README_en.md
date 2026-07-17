# Build the Add Operator Example with xmake

## Overview

This example demonstrates how to build and run the Add operator using xmake's Ascend C toolchain. The example declares the `ascendc` toolchain directly in `xmake.lua`, uses `xmake` to complete the build, and uses `xmake run xmake_add` to run the executable.

## Supported Products and CANN Versions

| Product | CANN Version |
| ------- | ------------ |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure

```text
├── 04_xmake
│   ├── xmake.lua      // xmake project file
│   ├── add.asc        // Ascend C Add operator implementation and invocation example
│   └── README.md      // Example description
```

## Example Description

- Example function:

  Add computation formula:

  ```text
  z = x + y
  ```

- Example implementation:

  - Kernel implementation:

    Each Block moves input data from Global Memory to Unified Buffer, performs the Add computation, and then moves the result back to Global Memory. The kernel calls `asc_sync` between the data movement and computation stages to synchronize the pipelines.

  - Invocation implementation:

    In the same `add.asc` file, the Host side generates fixed input data, invokes the kernel function using the kernel launch operator `<<<>>>`, and performs accuracy verification after the result is copied back.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Install xmake

  This example requires xmake 3.0.9 or later. Run the following command to install xmake. For other installation methods, see the [xmake installation guide](https://xmake.io/guide/quick-start.html#installation).

  ```bash
  curl -fsSL https://xmake.io/shget.text | bash
  ```

- Configure environment variables

  Configure environment variables according to the installation method of CANN Toolkit in the current environment, so that xmake can locate CANN Toolkit through `ASCEND_HOME_PATH` or `ASCEND_TOOLKIT_HOME`.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN Toolkit installation directory. If no installation directory is specified, the default directory is `/usr/local/Ascend`.

- Run the example

  ```bash
  xmake
  xmake run xmake_add
  ```

- Build option description

  The `xmake.lua` specifies the Ascend C toolchain and NPU architecture through the following configuration:

  ```lua
  set_toolchains("ascendc")
  add_ascnpuarchs("dav-2201")
  ```

  To adapt to Ascend 950PR/Ascend 950DT, change `dav-2201` to `dav-3510`.

- Execution result

  The following execution result indicates that the accuracy comparison is successful.

  ```text
  test pass!
  ```
