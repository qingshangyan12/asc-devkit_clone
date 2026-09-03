# operator+ Interface Sample

## Overview

This sample demonstrates how to use the experimental Tensor API `asc::te::experimental::operator+` to perform element-wise addition of two `float` Tensors. Inputs `src0` and `src1` have shape `[2, 64]`. Output `dst` also has shape `[2, 64]`. All three Tensors use the `float` data type, and each output element is the sum of the corresponding input elements.

The computation is as follows:

```text
dst[i] = src0[i] + src1[i]
```

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── CMakeLists.txt                 // Build project
├── README.md                      // Sample description
└── add.asc                        // operator+ interface sample implementation
```

## Sample Description

- Sample scenarios:

  | SCENARIO_NUM | Reg Tensor Layout | Description |
  | --- | --- | --- |
  | 1 | One-dimensional, shape `[128]` | Loads and stores each register block by a one-dimensional offset. |
  | 2 | Two-dimensional, shape `[2, 64]` | Loads and stores each row by a two-dimensional coordinate. |

- Sample function:

  Both scenarios move the input Tensors from GM to UB one row at a time, load the data into registers using `load`, add them with the experimental Tensor API Reg Tensor `operator+`, and write the results back to UB and GM using `store`.

- Sample implementation:

  - Key kernel steps

    1. Use `asc::te::make_tensor`, `asc::te::make_mem_ptr`, and `asc::te::make_frame_layout` to construct two-dimensional copy Tensors with shape `[2, 64]` in both GM and UB.
    2. Use `asc_lock/asc_unlock` around the MTE2 pipeline and `asc::te::copy` with coordinates and `copy_shape(1, 64)` to move GM data to UB one row at a time.
    3. Create one-dimensional and two-dimensional compute Tensors over the same UB memory, then select `compute_add_1d` or `compute_add_2d` according to `SCENARIO_NUM`.
    4. In the compute function, loop over `all_mask`, `load`, the experimental `operator+`, and `store` to perform element-wise addition.
    5. Use `asc::te::copy` to write the results from UB back to the corresponding two-dimensional GM regions one row at a time.

  The core computation is as follows:

  ```cpp
  const auto coord = asc::te::make_coord(row, 0);
  auto mask = asc::te::experimental::all_mask<float>();
  auto src0 = asc::te::experimental::load(src0_tensor, coord).with_mask(mask);
  auto src1 = asc::te::experimental::load(src1_tensor, coord).with_mask(mask);
  auto dst = src0 + src1;
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
