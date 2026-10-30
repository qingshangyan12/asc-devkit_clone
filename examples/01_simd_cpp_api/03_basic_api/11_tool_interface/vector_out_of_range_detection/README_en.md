# Vector Out-of-Range Detection

## Overview

This sample uses static Tensor programming with [NumericLimits](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/intro.md), [Compare](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compare.md), and [Compares](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compares.md) to detect positive infinity and negative infinity in vector addition results.

## Supported Products and CANN Version

| Product | CANN version |
| --- | --- |
| Ascend 950PR&950DT系列产品 | >= CANN 9.3.0 |

## Directory Structure

```text
vector_out_of_range_detection
├── scripts
│   ├── gen_data.py                    # Generate input and reference data
│   └── verify_result.py               # Verify results and comparison masks
├── CMakeLists.txt
├── data_utils.h
├── vector_out_of_range_detection.asc  # Kernel and host invocation
├── README.md
└── README_en.md
```

## Sample Description

### Sample Function

The kernel computes `z = x + y` and produces two comparison masks:

| Mask | Condition | Implementation |
| --- | --- | --- |
| Positive infinity | `z[i] == +Inf` | `NumericLimits<float>::Infinity` vector interface, Compare with EQ |
| Negative infinity | `z[i] == -Inf` | `NumericLimits<float>::NegativeInfinity` scalar interface, Compares with EQ |

Maximum and lowest finite values are not marked.

### Sample Specifications

| Direction | Name | Shape | Data type | Format |
| --- | --- | --- | --- | --- |
| Input | x, y | [256] each | float | ND |
| Output | z | [256] | float | ND |
| Output | masks | [2, 32] | uint8_t | ND |

Each comparison occupies one bit (1 for true, 0 for false). The two mask rows represent positive infinity and negative infinity, respectively. Element `i` maps to bit `i % 8` of byte `i / 8`, starting from the least significant bit.

Inputs cover ordinary finite values, zeros, maximum/lowest finite values, and infinities. Addition also produces infinities from finite overflow.

### Sample Implementation

**Kernel implementation**

1. Call [InitSocState](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/system_init/InitSocState.md) to initialize global status registers.
2. Use [LocalMemAllocator](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/LocalMemAllocator/LocalMemAllocator_intro.md) to allocate UB memory statically for the input, result, comparison value, and comparison masks. Four float buffers occupy 1024 bytes each, and the mask buffer occupies 64 bytes (4160 bytes in total). Buffer addresses meet the 32-byte alignment requirement.
3. Copy x and y from GM to UB using DataCopy. Use [SetFlag/WaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md) with MTE2_V to wait for the input transfers before vector computation.
4. Compute `z = x + y` with Add. Obtain infinities using NumericLimits and detect signed infinities using Compare and Compares.
5. Use V_MTE3 to wait for vector computation before copying z and masks to GM. Call [PipeBarrier](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md) before the kernel returns to wait for pipeline tasks to complete.

**Host invocation**

Read the inputs, allocate device memory, and copy inputs to the device. Invoke one Vector core using `<<<>>>`, synchronize the stream, and copy results back to the host for file output.

**Data generation and verification**

The generator uses a fixed random seed and NumPy reference results. The verifier checks signed infinity positions, finite results, and every comparison byte. Verification failures return a nonzero exit code.

The output file `output/output.bin` and reference file `output/golden.bin` share the same layout: the first 1024 bytes contain 256 float addition results, followed by 64 bytes of uint8_t comparison masks (positive infinity first, negative infinity second). The verifier accepts the output and reference file paths and checks file sizes, signed infinity positions, finite results, and every comparison bit.

## Build and Run

Run the following steps from the sample directory. Python 3 and NumPy are required for data generation and verification.

- Configure environment variables

  Configure the environment according to the CANN [installation instructions](../../../../../docs/zh/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  `${install_path}` is the CANN installation directory, which defaults to `/usr/local/Ascend` if no directory is specified.

- Run the sample

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  For CPU debugging or NPU simulation, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim`, respectively:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ```

  Clear the CMake cache before switching modes: run `rm CMakeCache.txt` in the build directory, then configure again.

- Build options

  | Option | Values | Description |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | NPU execution, CPU debugging, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Ascend 950PR&950DT系列产品 |

- Execution results

  The following output indicates that verification passed:

  ```text
  test pass!
  ```
