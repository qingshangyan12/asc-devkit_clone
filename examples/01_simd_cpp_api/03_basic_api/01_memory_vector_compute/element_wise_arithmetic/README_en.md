# ElementWiseArithmetic Class Example

[简体中文](README.md)

## Overview

This example demonstrates the usage of basic arithmetic interfaces based on LeakyRelu.

This directory also provides eight VECTOR Basic API pointer examples. Each data operand can independently use a `LocalTensor` or a raw pointer. The examples compare host reference results, Tensor calls, and mixed Tensor/pointer calls.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Ascend 950PR&950DT products | >= CANN 9.1.0 |
| Atlas A3 products | >= CANN 9.0.0 |
| Atlas A2 products | >= CANN 9.0.0 |

The table applies to the original LeakyRelu example. The VECTOR pointer examples were validated on **Ascend 950PR, dav-3510, CANN 9.2.0-beta.2, Linux NPU mode**, with device 0 available. Their build entry supports `dav-3510` and `npu` only; CPU debug and simulation modes are not provided for these examples.

## Directory Structure

```text
├── element_wise_arithmetic
│   ├── scripts
│   │   ├── gen_data.py             // Script for generating input data and ground truth data
│   │   └── verify_result.py        // Script for verifying whether output data matches ground truth data
│   ├── CMakeLists.txt              // Build project file
│   ├── data_utils.h                // Data read/write functions
│   ├── element_wise_arithmetic.asc // Ascend C example implementation & invocation example
│   ├── vector_pointer_regression.asc            // Arithmetic, logic, reduction, and numeric limits
│   ├── vector_pointer_conversion_regression.asc // Conversion and compound operations with conversion
│   ├── vector_pointer_layout_regression.asc     // Interleave, DeInterleave, and Mull
│   ├── vector_pointer_compare_regression.asc    // Comparison, selection, and comparison masks
│   ├── vector_pointer_index_regression.asc      // Indexing, broadcasting, and block reduction
│   ├── vector_pointer_transpose_regression.asc  // Transpose and format conversion
│   ├── vector_pointer_sort_regression.asc       // Sorting and merge sorting
│   ├── vector_pointer_special_regression.asc    // L1 Fill and dequantization parameters
│   ├── README.md                   // Chinese documentation
│   └── README_en.md                // English documentation
```

## Example Description

### LeakyRelu Example

- Example function:  
  This example implements element-wise Leaky Relu (Leaky Rectified Linear Unit) operation on the input tensor and returns the computation result.

  The corresponding mathematical expression is:  
  $$
  out =
  \begin{cases}
  x, & \text{if } x \ge 0 \\
  \alpha x, & \text{if } x < 0
  \end{cases}
  $$

- Example specifications:
  <table>
  <tr><th align="center">Example Type (OpType)</th><th colspan="4" align="center">LeakyRelu</th></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[4, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">out</td><td align="center">[4, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">element_wise_arithmetic_kernel</td></tr>
  </table>

- Example implementation:  
  This example implements a LeakyRelu example with a fixed shape of [4, 128].
  - Kernel implementation  
    - Call the DataCopy basic API to transfer data from GM (Global Memory) to UB (Unified Buffer)
    - Call the LeakyRelu interface to perform Leaky Relu operation on the input Tensor
    - Call the DataCopy basic API to transfer computation results from UB (Unified Buffer) to GM (Global Memory)

  - Invocation implementation  
    Use the kernel invocation operator <<<>>> to call the kernel function.

### VECTOR Pointer Examples

Each `.asc` file contains its own host entry, kernels, deterministic input generation, and result checks. These examples do not require `scripts/gen_data.py`, `scripts/verify_result.py`, or external input data files.

| Group / CMake target | Main interfaces | Data types | Active element counts (`count`) |
| --- | --- | --- | --- |
| `vector_pointer_elementwise` | Add, Sub, Mul, Relu, Exp, LeakyRelu, ReduceSum, NumericLimits, etc. | float; logical operations use int16_t, int32_t, uint16_t, uint32_t | 1, 32, 1024, 2048 |
| `vector_pointer_conversion` | Cast, CastDeq, CastDequant, AddReluCast, SubReluCast, MulCast, AddDeqRelu, MulsCast | float/int32_t to half, half to int8_t | 1, 32, 1024, 2048 |
| `vector_pointer_layout` | Interleave, DeInterleave, Mull | float, int32_t | 32, 1024, 2048; Mull also uses 1 |
| `vector_pointer_compare` | Compare, Compares, Select, GetCmpMask, SetCmpMask | float; masks use the integer types required by the corresponding interfaces | 1, 32, 1024, 2048; GetCmpMask/SetCmpMask use only 1 and 32 |
| `vector_pointer_index` | Gather, Scatter, Gatherb, GatherMask, Brcb, ReduceDataBlock, ReducePairElem, ReduceRepeat | float, uint32_t indices/masks | 1, 32, 64, 1024, 2048; Gatherb/Brcb start at 64 |
| `vector_pointer_transpose` | Transpose, TransDataTo5HD | half | 256, 1024, 2048 |
| `vector_pointer_sort` | Sort, Sort32, MrgSort | float scores, uint32_t indices | 32, 128, 1024, 2048; MrgSort starts at 128 |
| `vector_pointer_special` | Fill, SetDeqScale | half for Fill, uint64_t output for SetDeqScale | 16, 32, 1024, 2048 for Fill; 16 for SetDeqScale |

Inputs and outputs use contiguous buffers with a base capacity of `CAPACITY=2048`. Output allocations are expanded for multiple operand combinations or multiple outputs. Sorting stores `(score, index)` pairs; transpose checks use 16×16 blocks or an extended matrix shape. The table describes the configurations selected by these examples, not every type, shape, or signature supported by the interfaces.

- Kernel implementation: DataCopy and event synchronization transfer data between GM and UB. `TPipe/TBuf` manages local storage, and the physical address of a `LocalTensor` is converted to an `__ubuf__` pointer before calling the same Basic API. Fill uses an L1 `__cbuf__` pointer and AIC/AIV synchronization.
- Host invocation: Initialize ACL, select device 0, create a stream, launch kernels with `<<<>>>`, synchronize, and retrieve results. Each group enumerates Tensor/pointer combinations using the same inputs and checks its own host reference computation.
- Verification: Integers and indices must match exactly. Floating-point computations use the mixed tolerance in the source, for example `abs(actual - expected) <= 1e-5 + 1e-4 * abs(expected)`. Each program also compares mixed calls with Tensor calls and checks valid outputs and untouched regions as appropriate for the interface. An ACL error or verification failure results in a nonzero exit code.

For example, Add in the arithmetic group includes these combinations. See the complete source for buffer allocation and synchronization:

```cpp
__ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
__ubuf__ float* srcPtr = (__ubuf__ float*)src0.GetPhyAddr();
AscendC::Add(dst, src0, src1, count);
AscendC::Add(dstPtr, src0, src1, count);
AscendC::Add(dst, srcPtr, src1, count);
AscendC::Add(dstPtr, srcPtr, src1, count);
```

## Build and Run

### LeakyRelu Example

Run the following steps in the root directory of this example to build and run the example.

- Configure environment variables  
  Configure environment variables according to the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.
  ```bash
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;             # Build the project, default npu mode
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                        # Run the compiled executable to execute the example
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify whether the output result is correct and confirm algorithm logic is correct
  ```

  To use CPU debug or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.
  
  Examples:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Notice:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and then re-run cmake.

- Build option description

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 products and Atlas A3 products, dav-3510 corresponds to Ascend 950PR&950DT products |

- Execution result

  The following execution result indicates that the accuracy comparison is successful.

  ```bash
  test pass!
  ```

### VECTOR Pointer Examples

Prerequisites are CMake 3.16 or later, a C++17 host toolchain, and the `ASC` CMake package, `bisheng`, and ACL runtime supplied by CANN. Load the CANN environment, then run these commands from this example directory:

```bash
source /path/to/cann/set_env.sh
cmake -S . -B build-vector \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    -DCMAKE_ASC_RUN_MODE=npu \
    -DBUILD_VECTOR_POINTER_SAMPLES=ON
cmake --build build-vector --target vector_pointer_samples -j2
(cd build-vector && ctest -R '^vector_pointer_' --output-on-failure -j1)
```

`BUILD_VECTOR_POINTER_SAMPLES` defaults to `OFF`. Enabling it adds the eight executable targets listed above, the aggregate build target `vector_pointer_samples`, and their CTest cases. These targets prioritize the Basic API headers from the current checkout. Build from a complete repository containing the pointer extensions. CTest runs the groups serially because they all use device 0.

To build and run only the comparison group:

```bash
cmake --build build-vector --target vector_pointer_compare -j2
./build-vector/vector_pointer_compare
```

Each program prints per-case results, for example:

```text
PASS Compare count=1 form=0 modes=16 tensor_equal=true tail_unchanged=true
```

When all CTest cases succeed, the summary is `100% tests passed, 0 tests failed out of 8`. Full per-case output is saved in `build-vector/Testing/Temporary/LastTest.log`; use `ctest -V` to display it. Preserve logs for any `FAIL`, `Tensor/pointer mismatch`, `ACL failure`, or nonzero exit code, and do not count such runs as passed.

To save compiler commands, source hashes, and per-group device logs separately, run the existing regression script from the repository root:

```bash
bash tests/api/basic_api/vector_pointer/run_target_regressions.sh /tmp/vector-runtime
```

This script directly invokes the compiler to build and run the same eight sources. See the [test README](../../../../../tests/api/basic_api/vector_pointer/README_en.md) for the broader interface scope, header compilation, invalid operand cases, and precision tests.
