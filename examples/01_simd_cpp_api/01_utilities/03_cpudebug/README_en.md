# CPU Debug Direct Invocation Sample

## Overview

This sample implements CPU Debug testing for the Add operator using Ascend C.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products

## Directory Structure

```
├── 03_cpudebug
│   ├── CMakeLists.txt          // Build project file
│   └── add.asc                 // Ascend C operator implementation and invocation sample
```

## Sample Description

- CPU Debug Introduction:
  CPU Debug enables debugging of runtime states during CPU execution, primarily through GDB. GDB supports common debugging operations such as setting breakpoints, viewing register and memory states, single-stepping, and viewing call stacks.

- Operator Introduction:
  For details about the Add operator, see the [Add Operator Details](../../00_introduction/01_vector/add_tpipe_tque/README_en.md) section.

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.

- Configure Environment Variables
  Select the appropriate command based on the CANN development kit [installation method](../../../../docs/en/quick_start.md#prepare&install) on your current environment.
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
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;
  ./add
  ```
  Select the appropriate `CMAKE_ASC_ARCHITECTURES` parameter based on the actual NPU hardware architecture.
  - Build Options
    | Option | Description |
    |------|------|
    | `CMAKE_ASC_RUN_MODE` | Set to `cpu` to enable CPU domain compilation |
    | `CMAKE_ASC_ARCHITECTURES` | Specify the NPU architecture version. CMake configures the corresponding CPU debug libraries based on this value.<br>`dav-2201` corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products. `dav-3510` corresponds to Ascend 950PR/Ascend 950DT |

- Execution Result
  The following output indicates successful execution:
  ```bash
  [Success] Case accuracy is verification passed.
  ```

- Enter GDB Debug Mode
  The generated CPU domain executable supports debugging through GDB. GDB supports setting breakpoints, viewing register and memory states, single-stepping, and viewing call stacks. Add `gdb --args` before `./add` in the command above to enter GDB mode.
  ```bash
  gdb --args ./add
  ```
  CPU Debug launches a separate subprocess for each kernel function to simulate NPU execution logic. When using GDB debugging, set `follow-fork-mode` to have GDB follow the child process for breakpoint debugging inside kernel functions. Set the child process tracking mode after entering GDB:

  ```text
  (gdb) set follow-fork-mode child
  ```

  Then perform debugging as needed. Common operations:

  ```text
  # Set breakpoint at kernel function entry
  (gdb) break Compute

  # Run program
  (gdb) run

  # Single step execution
  (gdb) next

  # Print variable value
  (gdb) print xLocal.GetValue(0)

  # Continue to next breakpoint
  (gdb) continue
  ```