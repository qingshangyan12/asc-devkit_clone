# SimdVF Dump API Description

## Overview

This example uses `asc_dump_ubuf`, `asc_dump_reg`, and `asc_dump` to print UB and register data from vector functions. The host generates input and expected values in the same `.asc` file; the kernel reads from GM, performs C API addition, and writes back to GM. The host checks the result.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|----------------------|
| Ascend 950PR&950DT products | >= CANN 9.2.0 |

## Directory Structure

```
├── simd_vf_dump
│   ├── CMakeLists.txt          // Build project file
│   ├── simd_vf_dump.asc        // Kernel, host invocation, and result verification
│   └── README.md               // Example description document
```

## Example Description

- Example features:

  In vector programming mode, the host generates input, and the kernel uses `asc_vf_call` to print the original and modified data and C API calls for addition. The host compares the output with the expected values.

  1. `asc_dump_ubuf<T>` - Print UB data
  2. `asc_dump_reg<T>` - Print register data
  3. `asc_dump<T>` - Print UB/Reg data (generic API)

- simd_vf dump API usage:

  | API | Address Type | Example |
  |-----|-------------|---------|
  | `asc_dump_ubuf<T>` | `__ubuf__` | `asc_dump_ubuf<float>(input, desc, dump_size);` |
  | `asc_dump_reg<T>` | Reg | `asc_dump_reg<float>(input, desc, dump_size);` |
  | `asc_dump<T>` | `__ubuf__` / Reg | `asc_dump<float>(input, desc, dump_size);` |

  Parameter description:
  - Template parameter `T`: Specifies the data type (float/half, and so on)
  - `input`: UB data address or Reg register object
  - `desc`: Print control descriptor
  - `dump_size`: Number of data elements per single dump

- Invocation implementation

  Use `asc_vf_call` inside the `__global__ __vector__` kernel to print the original UB and register data. Compute `x + 1.0f` with the C API `asc_add_scalar` in the VF, then dump the modified UB and write the result to GM. `asc_dump` is a debugging utility shared by the C and C++ API examples; its template parameter specifies the printed data type. The host generates input and verifies output directly.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.
  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build -j
  ./build/demo
  ```

- Build options

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: Ascend 950PR&950DT products |

- Execution result
  The final execution result is as follows, indicating successful accuracy comparison.
  ```bash
  test pass!
  ```
