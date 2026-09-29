# hifloat8_t SwiGLU Static Tensor Sample

## Overview

This sample implements a SwiGLU activation operator with hifloat8_t inputs and outputs using static Tensor programming. It demonstrates the dequantize-compute-quantize (DQ-Compute-Q) pattern: sigmoid and element-wise multiplication are computed in float, and the result is converted back to hifloat8_t with CAST_ROUND.

## Supported Products and CANN Version

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```
├── CMakeLists.txt       // Build configuration
├── data_utils.h         // Host file I/O helpers
├── hif8_swiglu.asc      // Ascend C kernel and host invocation
├── scripts
│   ├── gen_data.py      // Generate hifloat8_t inputs and reference output
│   ├── hif8.py          // hifloat8_t CAST_ROUND encoder/decoder
│   └── verify_result.py // Decode and verify the output
└── README_en.md
```

## Operator Specification

The operator processes 2048 elements in one core. All tensors use ND format.

| Name | Input/Output | Shape | Data type |
| --- | --- | --- | --- |
| g | Input | (1, 2048) | hifloat8_t |
| u | Input | (1, 2048) | hifloat8_t |
| z | Output | (1, 2048) | hifloat8_t |

The computation is:

$$z_i = Q(D(g_i) \cdot \sigma(D(g_i)) \cdot D(u_i))$$

Here, $D$ is a lossless hifloat8_t-to-float conversion and $Q$ is a float-to-hifloat8_t CAST_ROUND conversion.

## Implementation

- `LocalTensor` constructors are used to plan UB byte offsets directly without TPipe/TQue.
- `DataCopy` moves hifloat8_t data between Global Memory and UB.
- RegBase `Cast` converts between hifloat8_t and float, and the SwiGLU computation runs in registers.
- `CAST_ROUND` quantizes the result back to hifloat8_t without storing intermediate float data in UB.
- `SetFlag/WaitFlag` synchronizes the MTE2, Vector, and MTE3 pipelines.

## Build and Run

Run the following steps in the root directory of this sample to build and run it.

- Configure environment variables

  Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the sample

  Run the following commands in this sample directory.

  ```bash
  mkdir -p build && cd build                         # Create and enter the build directory
  cmake -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j                                             # Build the project
  python3 ../scripts/gen_data.py                     # Generate test input data
  ./demo                                              # Run the sample
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # Verify the output
  ```

  For NPU simulation, set `CMAKE_ASC_RUN_MODE` to `sim`:

  ```bash
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  > **Note:** Clear the CMake cache in the build directory or recreate the build directory before switching run modes.

- Build option description

  | Option | Values | Description |
  |------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture |
  | `CMAKE_VF_MODE` | `true`, `false` (default) | Enable SIMD VF fusion |

- Execution results

  The following output indicates that the accuracy comparison succeeded.

  ```text
  test pass!
  ```

The verification script decodes the output to float and compares it with the CAST_ROUND reference.
