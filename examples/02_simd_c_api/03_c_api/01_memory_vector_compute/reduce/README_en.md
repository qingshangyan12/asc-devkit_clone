# reduce Example

## Overview

This Memory vector compute sample uses [asc_repeat_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md) to sum float data within one repeat.

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Atlas A2 Training/Inference and Atlas A3 Training/Inference | >= CANN 9.2.0 |

## Directory Structure

```text
reduce/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── reduce.asc
├── README.md
└── README_en.md
```

## Sample Description

The input contains 64 float values. `asc_repeat_reduce_sum` writes the sum to output element 0. The generator creates deterministic input and one float golden value; the host checks it with an absolute tolerance of `1e-4`.

## Build and Run

Run the following steps in the root directory of this example.

- Configure environment variables

  Configure environment variables according to the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install).

```bash
source ${install_path}/cann/set_env.sh
```

  > **Note:** `${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for a non-root user.

- NPU execution

```bash
mkdir -p build && cd build;                                                                       # Create and enter the build directory
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # Build the project (default NPU mode)
python3 ../scripts/gen_data.py                                                                    # Generate test input and golden data
./demo                                                                                           # Run the compiled executable
```

- Build options

| Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU architecture: Atlas A2 Training/Inference and Atlas A3 Training/Inference |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
