# Basic Compute Sample Implemented with Tensor API

## Overview

This sample demonstrates the register basic compute API `mull`. It uses one
representative data type, `uint32_t`, to multiply two 64-element inputs and produce the low and high
32-bit halves of each product.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```plain
├── basic_arithmetic
│   ├── scripts
│   │   ├── gen_data.py          // Generates input and golden data.
│   │   └── verify_result.py     // Verifies output data.
│   ├── CMakeLists.txt           // Build project file.
│   ├── data_utils.h             // Data input and output utilities.
│   ├── README.md
│   ├── README_En.md
│   └── basic_arithmetic_tensor_api.asc  // Ascend C operator implementation and invocation sample.
```

## Sample Description

- Sample function:

  ```text
  product_i = uint64(src0_i) * uint64(src1_i)
  low_i = product_i & 0xffffffff
  high_i = product_i >> 32
  ```

- Sample specification:

  | Type | Name | Shape | Data Type | Format |
  | --- | --- | --- | --- | --- |
  | Input | `src0` | `[1, 64]` | `uint32_t` | ND |
  | Input | `src1` | `[1, 64]` | `uint32_t` | ND |
  | Output | `low` | `[1, 64]` | `uint32_t` | ND |
  | Output | `high` | `[1, 64]` | `uint32_t` | ND |

- Sample implementation:

  1. Create two input Tensors and two output Tensors on both GM and UB.
  2. Move `src0` and `src1` from GM to UB and load them into `reg_tensor<uint32_t>` objects.
  3. Call `mull` with a 64-element mask and obtain the low and high results through structured binding.
  4. Store both results in UB and move them back to GM.
  5. Save the output files on the host and verify both 32-bit halves against the golden data.

## Core Interface

### mull

```cpp
template <typename T>
__simd_callee__ inline reg_pair<T> mull(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1);
```

The API supports `int32_t` and `uint32_t`. The returned `first` member contains the low 32 bits, and
the `second` member contains the high 32 bits.

## Build and Run

Run the following commands in the sample root directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py
```

Build options:

| Option | Value | Description |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | NPU execution or NPU simulation |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture |

A successful run prints:

```text
basic_arithmetic_tensor_api executed successfully.
test pass! verified low and high uint32 mull results.
```
