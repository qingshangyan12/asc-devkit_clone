# reduce_sum/reduce_max/reduce_min Interface Sample

## Overview

This sample demonstrates how to use the experimental Tensor APIs `asc::te::experimental::reduce_sum`, `reduce_max`, and `reduce_min` to reduce a `float` Reg Tensor with element scope. Input `src` is a one-dimensional Tensor with shape `[64]`. The three reduction results are written to three 64-element regions in the output Tensor.

The computation is as follows:

```text
dst[0]   = sum(src[0:64])
dst[64]  = max(src[0:64])
dst[128] = min(src[0:64])
```

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── CMakeLists.txt                 // Build project
├── README.md                      // Chinese sample description
├── README_en.md                   // English sample description
└── reduction_compute.asc          // Reduction interface sample implementation
```

## Sample Description

- Sample function:

  The sample moves the input Tensor from GM to UB, loads it into a register using `load`, calls the experimental Reg Tensor reduction APIs to calculate the sum, maximum, and minimum, and writes the three results back to UB and GM using `store`. The default template argument selects `reduce_scope::element`, and each reduction result is stored in the first element of its output region.

- Sample implementation:

  - Key kernel steps

    1. Use `asc::te::make_tensor`, `asc::te::make_mem_ptr`, and `asc::te::make_frame_layout` to construct one-dimensional Tensors in GM and UB.
    2. Use `asc_lock/asc_unlock` around the MTE2 pipeline and `asc::te::copy` to move the input from GM to UB.
    3. Use `all_mask` and `load` to load 64 `float` values into a Reg Tensor.
    4. Call `reduce_sum<float>`, `reduce_max<float>`, and `reduce_min<float>` to perform element-scope reductions.
    5. Use `store` and `asc::te::copy` to write the results back to UB and GM, and verify the three valid results on the host.

  The core computation is as follows:

  ```cpp
  const auto coord = asc::te::make_coord(0);
  auto mask = asc::te::experimental::all_mask<float>();
  auto src = asc::te::experimental::load(src_tensor, coord).with_mask(mask);

  auto sum = asc::te::experimental::reduce_sum<float>(src);
  auto max = asc::te::experimental::reduce_max<float>(src);
  auto min = asc::te::experimental::reduce_min<float>(src);

  asc::te::experimental::store(sum_tensor, coord, sum);
  asc::te::experimental::store(max_tensor, coord, max);
  asc::te::experimental::store(min_tensor, coord, min);
  ```

  `reduce_sum` also supports `reduce_scope::datablock` and `reduce_scope::pair`. `reduce_max` and `reduce_min` also support `reduce_scope::datablock`. Specify the reduction scope explicitly with the second template argument.

## Build and Run

Perform the following steps in the sample root directory to build and run the sample.

- Configure environment variables

  Configure the CANN package environment variables by following the [environment configuration guide](https://gitcode.com/cann/asc-devkit/blob/master/docs/en/quick_start.md).

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

  To use NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` option.

  Example:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..;make -j; # NPU simulation mode
  ```

  > **Note:** Clear the CMake cache before switching build modes. Run `rm CMakeCache.txt` in the `build` directory, and then run CMake again.

- Result

  Successful verification prints:

  ```text
  test pass!
  ```
