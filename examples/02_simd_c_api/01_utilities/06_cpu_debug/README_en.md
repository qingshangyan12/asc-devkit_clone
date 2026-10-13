# CPU Debug Direct Invocation Example Description

## Overview

This example uses Add computation as the carrier to demonstrate the Ascend C CPU Debug workflow, including CPU domain compilation, executable program execution, entering debug mode through GDB, and setting breakpoints, single-stepping, and viewing variables in kernel functions. This example helps developers debug Ascend C kernel function logic and locate issues locally without relying on NPU execution.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|----------------------|
| Ascend 950PR&950DT products | >= CANN 9.1.0 |

## Directory Structure

```
├── 06_cpu_debug
│   ├── CMakeLists.txt          // Build project file
│   ├── cpu_debug.asc           // Ascend C example implementation & invocation example
│   └── README.md               // Example description document
```

## Example Description

- CPU Debug introduction:
  The CPU Debug feature supports debugging the runtime state during CPU execution, primarily through the GDB tool. GDB debugging supports common debugging operations such as setting breakpoints, viewing registers and memory state, single-stepping, and viewing the call stack.

- Example introduction:
  This example uses the C API for a `float` elementwise Add. Each block handles `2048` elements: `asc_copy_gm2ub_align` loads the inputs, a SIMD VF computes with `asc_add` after synchronization, and `asc_copy_ub2gm_align` writes the result to GM.


## Build and Run

Run the following steps in the root directory of this example to build and run the example.
- Configure environment variables
  Configure environment variables based on the [installation method](../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.
  ```bash
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ./cpu_debug
  ```
  The command above passed accuracy verification on a `dav-3510` NPU. For CPU debugging, configure a separate `build_cpu` directory with `-DCMAKE_ASC_RUN_MODE=cpu`. In the CANN 9.3.0 container used for this port, CPU mode reported a UB address overflow and failed accuracy verification; it is not a validated execution path.

- Build options

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu`, `cpu` | `npu` was verified; `cpu` enables CPU compilation but failed accuracy in this container |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: Ascend 950PR&950DT products |

- Execution result
  The execution result is as follows, indicating successful execution.
  ```bash
  [Success] Case accuracy is verification passed.
  ```
- Enter GDB debug mode
  > **Note:** GDB version 12.1 or later is required.

  A CPU-domain executable supports GDB breakpoints, single-stepping, and variable inspection. Configure it separately with `cmake -S . -B build_cpu -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510`, build it, and run from `build_cpu`:
  ```bash
  gdb --args ./cpu_debug
  ```
  CPU Debug simulates NPU execution logic by launching a separate child process for each kernel function. Therefore, when debugging with GDB, set `follow-fork-mode` to let GDB track the child process in order to debug at breakpoints inside kernel functions. After entering GDB, first set the child process tracking mode:

  ```text
  (gdb) set follow-fork-mode child
  ```

  Then debug as needed. Common operations:

  ```text
  # Set a breakpoint at the kernel function entry point
  (gdb) break add_custom

  # Run the program
  (gdb) run

  # Single-step execution
  (gdb) next

  # Continue execution to the next breakpoint
  (gdb) continue
  ```
