# fma Interface Sample

## Overview

This sample demonstrates how to use the experimental Tensor API `asc::te::experimental::fma` to perform element-wise fused multiply-add on three `float` Tensors.

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

- Sample scenarios:

  | SCENARIO_NUM | Tensor Layout | Description |
  | --- | --- | --- |
  | 1 | One-dimensional, shape `[128]` | Loads and stores register blocks by one-dimensional offsets. |
  | 2 | Two-dimensional, shape `[2, 64]` | Loads and stores each row by two-dimensional coordinates. |

- Sample implementation:

  - Key kernel steps

    1. Construct one-dimensional or two-dimensional GM and UB Tensors according to `SCENARIO_NUM`.
    2. Move complete data in the one-dimensional scenario, or move rows using coordinates and `copy_shape(1, 64)` in the two-dimensional scenario.
    3. Use `all_mask`, `load`, `fma`, and `store` to perform element-wise fused multiply-add.

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
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  ./demo
  ```

  For NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command. When changing the run mode, chip model, or scenario, clear the CMake cache in the `build` directory before configuring the project again.

- Result

  Successful verification prints:

  ```text
  test pass!
  ```
