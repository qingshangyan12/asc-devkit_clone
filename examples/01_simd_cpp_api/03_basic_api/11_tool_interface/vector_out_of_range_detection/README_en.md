# Vector Out-of-Range Detection

## Overview

This sample uses static Tensor programming with [NumericLimits](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/intro.md), [Compare](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compare.md), and [Compares](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compares.md) to detect NaN, positive infinity, and negative infinity in vector addition results.

## Supported Products and CANN Version

| Product | CANN version |
| --- | --- |
| Ascend 950PR&950DT系列产品 | Validated with CANN 9.3.0 weekly.20261009.01 |

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

The kernel computes `z = x + y` and produces three comparison masks:

| Mask | Condition | Implementation |
| --- | --- | --- |
| NaN | `z[i] != z[i]` | Compare with NE |
| Positive infinity | `z[i] == +Inf` | NumericLimits<float>::Infinity vector interface, Compare with EQ |
| Negative infinity | `z[i] == -Inf` | NumericLimits<float>::NegativeInfinity scalar interface, Compares with EQ |

NaN is unequal to itself; equality comparison with QuietNaN cannot detect NaN. Maximum and lowest finite values are not marked.

### Sample Specifications

| Direction | Name | Shape | Data type | Format |
| --- | --- | --- | --- | --- |
| Input | x, y | [256] each | float | ND |
| Output | z | [256] | float | ND |
| Output | masks | [3, 32] | uint8_t | ND |

Each comparison occupies one bit (1 for true, 0 for false). The three mask rows represent NaN, positive infinity, and negative infinity, respectively. Element `i` maps to bit `i % 8` of byte `i / 8`, starting from the least significant bit.

Inputs cover ordinary finite values, zeros, maximum/lowest finite values, infinities, and NaN. Addition also produces infinities from finite overflow and NaN from opposite infinities.

### Sample Implementation

**Kernel implementation**

1. Call [InitSocState](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/system_init/InitSocState.md) to initialize global status registers.
2. Use [LocalMemAllocator](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/LocalMemAllocator/LocalMemAllocator_intro.md) to allocate UB memory statically for the input, result, comparison value, and comparison masks. Four float buffers occupy 1024 bytes each, and the mask buffer occupies 96 bytes (4192 bytes in total). Buffer addresses meet the 32-byte alignment requirement.
3. Copy x and y from GM to UB using DataCopy. Use [SetFlag/WaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md) with MTE2_V to wait for the input transfers before vector computation.
4. Compute `z = x + y` with Add. Obtain infinities using NumericLimits and detect NaN and signed infinities using Compare and Compares.
5. Use V_MTE3 to wait for vector computation before copying z and masks to GM. Call [PipeBarrier](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md) before the kernel returns to wait for pipeline tasks to complete.

**Host invocation**

Read the inputs, allocate device memory, and copy inputs to the device. Invoke one Vector core using `<<<>>>`, synchronize the stream, and copy results back to the host for file output.

**Data generation and verification**

The generator uses a fixed random seed and NumPy reference results. The verifier checks NaN and signed infinity positions, finite results, and every comparison byte. Verification failures return a nonzero exit code.

## Build and Run

Run the following steps from the sample directory. Python 3 and NumPy are required for data generation and verification.

- Configure environment variables

  Configure the environment according to the CANN [installation instructions](../../../../../docs/zh/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  `${install_path}` is the CANN installation directory. For a separate weekly installation, use its installation path.

- Run the sample

  ```bash
  MODE=npu
  cmake -S . -B build_${MODE} -DCMAKE_ASC_RUN_MODE=${MODE} -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build_${MODE} -j
  (
      cd build_${MODE} || exit 1
      python3 ../scripts/gen_data.py &&
      ./demo &&
      python3 ../scripts/verify_result.py output
  )
  ```

  Set `MODE=cpu` for CPU debugging or `MODE=sim` for NPU simulation and repeat the commands. Separate build directories avoid sharing CMake caches.

- Build options

  | Option | Values | Description |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | NPU execution, CPU debugging, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | Ascend 950PR&950DT系列产品 |

- Execution results

  The following output indicates that computation results and comparison masks passed verification:

  ```text
  NaN: 3, +Inf: 3, -Inf: 3
  test pass!
  ```

  During NPU simulation, NaN/Inf test inputs may produce a `vec_err_idata_inf_nan_t0` model diagnostic. In validation, simulation completed normally and all element checks passed. Check both the program exit code and verification result.
