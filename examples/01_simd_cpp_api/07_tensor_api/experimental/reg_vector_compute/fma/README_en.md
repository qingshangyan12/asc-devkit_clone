# fma Interface Sample

## Overview

This sample demonstrates how to use the experimental Tensor API `asc::te::experimental::fma` to perform element-wise fused multiply-add on three `float` Tensors.

```text
dst[i] = src0[i] * src1[i] + src2[i]
```

## Supported Products

- Ascend 950PR&950DT series

## Directory Structure

```text
├── CMakeLists.txt                 // Build project
├── README.md                      // Chinese sample description
├── README_en.md                   // English sample description
└── fma.asc                        // fma interface sample implementation
```

## Sample Description

- Sample function:

  The sample moves three input Tensors from GM to UB, loads them into registers using `load`, performs element-wise fused multiply-add using `fma`, and writes the result back to UB and GM using `store`. The host generates the inputs and verifies the output.

- Sample scenarios:

  | SCENARIO_NUM | Tensor Layout | Description |
  | --- | --- | --- |
  | 1 | One-dimensional, shape `[128]` | Processes two register blocks by one-dimensional offsets. |
  | 2 | Two-dimensional, shape `[2, 64]` | Processes each row by two-dimensional coordinates. |

- Sample implementation:

  - Key kernel steps

    1. Construct one-dimensional or two-dimensional GM and UB Tensors according to `SCENARIO_NUM`.
    2. Move data between GM and UB using `copy(dst, src)`.
    3. Use `update_mask`, `load`, `fma`, and `store` to perform element-wise fused multiply-add.

  The core computation is as follows:

  ```cpp
  template <typename Src0Tensor, typename Src1Tensor, typename Src2Tensor, typename DstTensor>
  __simd_vf__ inline void fma_vf(
      const Src0Tensor src0, const Src1Tensor src1, const Src2Tensor src2, DstTensor dst, uint16_t repeat_times,
      uint32_t total, uint32_t one_repeat_size)
  {
      using data_type = typename Src0Tensor::data_type;
      uint32_t remain = total;
      for (uint16_t i = 0; i < repeat_times; ++i) {
          const uint32_t offset = i * one_repeat_size;
          const auto coord = asc::te::make_coord(offset);
          auto mask = asc::te::experimental::update_mask<data_type>(remain);
          auto src0_reg = asc::te::experimental::load(src0, coord).with_mask(mask);
          auto src1_reg = asc::te::experimental::load(src1, coord).with_mask(mask);
          auto src2_reg = asc::te::experimental::load(src2, coord).with_mask(mask);
          auto dst_reg = asc::te::experimental::fma(src0_reg, src1_reg, src2_reg);
          asc::te::experimental::store(dst, coord, dst_reg);
      }
  }
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

  For NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command. When changing the run mode, chip model, or scenario, clear the CMake cache in the build directory before configuring the project again.

- Result

  Successful verification prints:

  ```text
  test pass!
  ```
