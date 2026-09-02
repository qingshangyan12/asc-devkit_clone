# fma Interface Sample

## Overview

This sample demonstrates how to use the experimental Tensor API `asc::te::experimental::fma` to perform element-wise fused multiply-add on three one-dimensional `float` Tensors with shape `[64]`.

```text
dst[i] = src0[i] * src1[i] + src2[i]
```

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── CMakeLists.txt                 // Build project
├── README.md                      // Sample description
└── fma.asc                        // fma interface sample implementation
```

## Sample Description

- Sample function:

  The sample moves three input Tensors from GM to UB, loads them into registers using `load`, performs element-wise fused multiply-add using `fma`, and writes the result back to UB and GM using `store`. The host generates the inputs and verifies the output directly.

- Sample implementation:

  - Key kernel steps

    1. Use `asc::te::make_tensor`, `asc::te::make_mem_ptr`, and `asc::te::make_frame_layout` to construct one-dimensional Tensors in GM and UB.
    2. Use `asc::te::copy` to move the three inputs from GM to UB.
    3. Use `all_mask` and `load` to load the UB data into Reg Tensors.
    4. Call `fma` to perform element-wise fused multiply-add.
    5. Use `store` and `asc::te::copy` to write the result back to UB and GM.

  The core computation is as follows:

  ```cpp
  const auto coord = asc::te::make_coord(0);
  auto mask = asc::te::experimental::all_mask<float>();
  auto src0 = asc::te::experimental::load(src0_tensor, coord).with_mask(mask);
  auto src1 = asc::te::experimental::load(src1_tensor, coord).with_mask(mask);
  auto src2 = asc::te::experimental::load(src2_tensor, coord).with_mask(mask);
  auto dst = asc::te::experimental::fma(src0, src1, src2);
  asc::te::experimental::store(dst_tensor, coord, dst);
  ```

## Build and Run

Perform the following steps in the sample root directory to build and run the sample.

- Configure environment variables

  Configure the CANN package environment variables by following the [environment configuration guide](../../../../../../docs/en/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/set_env.sh
  ```

  Replace `${install_path}` with the CANN package installation directory.

- Run the sample

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  ./demo
  ```

  For NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command. When changing the run mode or chip model, clear the CMake cache in the `build` directory before configuring the project again.

- Result

  Successful verification prints:

  ```text
  test pass!
  ```
