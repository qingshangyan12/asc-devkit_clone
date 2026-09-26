# Build the Add Operator Example with xmake

## Overview

This example demonstrates how to build and run the Add operator using xmake's Ascend C toolchain. The kernel is implemented with the **C_API**. The example declares the `ascendc` toolchain and NPU architecture in `xmake.lua`, uses `xmake` to complete the build, and uses `xmake run xmake_add` to run the executable.

## Supported Products and CANN Versions

| Product | CANN Version |
| ------- | ------------ |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

> **Note:** This example uses membase sync C_API (`asc_copy_*_sync` / `asc_add_sync`). The supported products match the [C_API sync Add example](../../../../02_simd_c_api/00_introduction/01_add/c_api_sync_add). For Ascend 950PR/Ascend 950DT, see the RegBase C_API examples.

## Directory Structure

```text
├── 04_xmake
│   ├── xmake.lua      // xmake project file
│   ├── add.asc        // Ascend C C_API Add operator implementation and invocation example
│   └── README.md      // Example description
```

## Example Description

- Example function:

  Add computation formula:

  ```text
  z = x + y
  ```

  The example uses 8 cores. Each core processes 2048 `float` elements, so the total data size is `8 × 2048 = 16384`.

- Example implementation:

  - Kernel implementation:

    With the C_API, input data must first be moved into on-chip storage, then computed, and finally moved back to external storage. The flow has three steps:

    1. Move inputs `x` and `y` from Global Memory (`__gm__`) to Unified Buffer (`__ubuf__`) as `xLocal` and `yLocal`;
    2. Perform addition on `xLocal` and `yLocal`, and store the result in `zLocal`;
    3. Move `zLocal` back to output `z` in Global Memory.

    The kernel entry calls `asc_init()` to initialize the runtime state. Data movement and computation use sync APIs (`asc_copy_gm2ub_sync` / `asc_add_sync` / `asc_copy_ub2gm_sync`) to ensure pipeline completion before the next step. The implementation style follows the [C_API sync Add example](../../../../02_simd_c_api/00_introduction/01_add/c_api_sync_add).

  - Invocation implementation:

    In the same `add.asc` file, the Host side generates input data, invokes the kernel with the launch operator `<<<>>>`, throws an exception when an ACL call fails, and performs accuracy verification after the result is copied back.

## How to Write the xmake Project

The core build script is `xmake.lua`:

```lua
-- Expose the NPU architecture via an option instead of hardcoding it
option("npu_arch")
    set_default("dav-2201")
    set_showmenu(true)
    set_description("NPU architecture for this sample (dav-2201 for Atlas A2/A3)")
option_end()

add_rules("mode.debug", "mode.release")

target("xmake_add")
    set_kind("binary")              -- build an executable
    set_toolchains("ascendc")       -- use the Ascend C toolchain (bisheng)
    add_files("add.asc")            -- add source files
    add_ascnpuarchs(get_config("npu_arch"))  -- pass the NPU architecture
```

Compared with CMake examples in the same directory:

| Item | CMake example (for example, `00_basic_compile`) | This example (xmake) |
| ---- | ----------------------------------------------- | -------------------- |
| Project file | `CMakeLists.txt` | `xmake.lua` |
| Toolchain setup | `find_package(ASC)` + `project(... LANGUAGES ASC CXX)` | `set_toolchains("ascendc")` |
| Architecture config | `-DCMAKE_ASC_ARCHITECTURES=dav-xxxx` | `xmake f --npu_arch=dav-xxxx` |
| Source declaration | `add_executable(demo ...)` | `target(...)` + `add_files(...)` |
| Build command | `cmake .. && make -j` | `xmake` |
| Run command | `./demo` | `xmake run xmake_add` |

For `option` / `get_config` usage, see the [xmake option documentation](https://xmake.io/guide/project-configuration/define-options.html).

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Install xmake

  This example requires xmake 3.0.9 or later. On Linux/macOS, run one of the following commands. For Windows and other installation methods, see the [xmake installation guide](https://xmake.io/guide/quick-start.html#installation).

  ```bash
  # One-line curl install (recommended)
  curl -fsSL https://xmake.io/shget.text | bash

  # Or use the official install script
  bash <(curl -fsSL https://xmake.io/shget.text)
  ```

  After installation, run `xmake --version` to confirm the version, and ensure `~/.local/bin` (or the install directory) is in your `PATH`.

- Configure environment variables

  Configure environment variables according to the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development toolkit in the current environment, so that xmake can locate the toolkit through `ASCEND_HOME_PATH` or `ASCEND_TOOLKIT_HOME`.

  - Default path, CANN package installed by the root user

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, CANN package installed by a non-root user

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Custom path `install_path`

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Run the example

  > **Note:** The commands below are prefixed with `XMAKE_ROOT=y`. For safety, xmake refuses to run build scripts as root by default, and this variable explicitly allows it — so it is **only required when running as root**. **Regular (non-root) users do not need it**: run `xmake f --npu_arch=dav-2201`, `xmake` and `xmake run xmake_add` directly.

  ```bash
  # Configure the NPU architecture (optional; default is dav-2201)
  XMAKE_ROOT=y xmake f --npu_arch=dav-2201

  # Build
  XMAKE_ROOT=y xmake

  # Run
  XMAKE_ROOT=y xmake run xmake_add
  ```

  For the mapping between product models and architecture versions, see the [architecture mapping table](../../../../../docs/zh/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md#npu-arch). This example currently supports `dav-2201` (Atlas A2/A3 series products).

- Build option description

  | Option | Values | Description |
  | ------ | ------ | ----------- |
  | `npu_arch` | `dav-2201` (default) | NPU architecture, passed from the command line to avoid hardcoding it in `xmake.lua` |

  To list all configuration options:

  ```bash
  xmake f --help
  ```

- Execution result

  The following execution result indicates that the accuracy comparison is successful.

  ```text
  test pass!
  ```
