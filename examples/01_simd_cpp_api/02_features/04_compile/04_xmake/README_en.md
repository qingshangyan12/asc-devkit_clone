# Build the Add Operator Example with xmake

## Overview

This example demonstrates how to build and run the Add operator using xmake's Ascend C toolchain. The example declares the `ascendc` toolchain directly in `xmake.lua`, uses `xmake` to complete the build, and uses `xmake run xmake_add` to run the executable.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

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

  ```python
  z = x + y
  ```

- Example implementation:

  - Kernel implementation
    Moves the input data from Global Memory to Local Memory, performs the Add computation, and then moves the result back to Global Memory.

  - Invocation implementation
    The Host side generates fixed input data, invokes the kernel function using the kernel launch operator `<<<>>>`, and performs accuracy verification after the result is copied back.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Configure environment variables

  Make sure xmake (version >= 3.0.9) is installed in the current environment and that xmake can use the `ascendc` toolchain. Configure environment variables according to the installation method of the CANN development kit in the current environment, so that xmake can locate the CANN SDK through `ASCEND_HOME_PATH` or `ASCEND_TOOLKIT_HOME`.

  When the CANN package is installed by the root user at the default path, run:

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  When the CANN package is installed by a non-root user at the default path, run:

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  When the CANN package is installed at a custom path `install_path`, run:

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

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

  ```bash
  test pass!
  ```
