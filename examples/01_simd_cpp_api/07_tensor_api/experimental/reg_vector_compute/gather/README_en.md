# gather API Sample

## Overview

This sample combines register loading, indexed gathering, and storage capabilities of the experimental Tensor API to demonstrate `asc::te::experimental::gather`. Its test case design follows the basic API sample `03_basic_api/02_reg_vector_compute/gather`. A single vector core processes a one-dimensional `half` Tensor with shape `[128]`, and the index Tensor has element type `uint16_t`.

The computation is defined as follows:

```text
dst[i] = src[index[i]], 0 <= i < 128
```

Indexes range from 0 to 127 and represent element positions within the source register, not byte offsets or global GM indexes. The 128 `half` elements exactly fill one 256-byte register.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── CMakeLists.txt          // Build configuration
├── README.md               // Sample description in Chinese
├── README_en.md            // Sample description in English
├── gather.asc              // Operator kernel, host invocation, and result verification
└── scripts
    ├── gen_data.py         // Input and golden data generation script
    └── verify_result.py    // Bit-for-bit output verification script
```

## Sample Description

- Sample functionality:

  The sample reads source data and indexes from `input/input_x.bin` and `input/input_y.bin`, transfers the input Tensors from GM to UB, loads them into registers with `load`, and calls the experimental Reg Tensor `gather` API to gather elements by index. It then uses `store` to write the result back to UB and transfers it to GM. The host saves the output to `output/output.bin` and verifies all 128 elements bit-for-bit against the golden data in `output/golden.bin`.

- Sample specifications:

  | File | Contents | Size |
  | --- | --- | --- |
  | `input/input_x.bin` | 128 `half` source elements | 256 B |
  | `input/input_y.bin` | 128 `uint16_t` index elements | 256 B |
  | `output/output.bin` | 128 `half` output elements | 256 B |
  | `output/golden.bin` | 128 `half` golden elements | 256 B |

- Sample implementation:

  - Key kernel steps

    1. Construct GM and UB Tensors using `asc::te::make_tensor`, `asc::te::make_mem_ptr`, and a layout.
    2. Enclose the MTE2 pipeline operations with `asc::te::mutex`, and transfer source data and indexes from GM to UB using `make_copy(copy_gm_to_ub{})` and `copy`.
    3. Use `update_mask<half>` to create a mask with 128 active elements, load source data and indexes with `load`, and set the active elements with `with_mask`.
    4. Call `gather(source_reg, index_reg)` to gather elements by index. The result inherits the index register mask.
    5. Use `store` to write to the UB output Tensor, then transfer the result to GM using `make_copy(copy_ub_to_gm{})` and `copy`.

  The core computation code is as follows:

  ```cpp
  const auto coord = asc::te::make_coord(0);
  uint32_t remaining = total_length;
  auto mask = asc::te::experimental::update_mask<half>(remaining);
  auto source_reg = asc::te::experimental::load(source, coord).with_mask(mask);
  auto index_reg = asc::te::experimental::load(indexes, coord).with_mask(mask);
  auto destination_reg = asc::te::experimental::gather(source_reg, index_reg);
  asc::te::experimental::store(destination, coord, destination_reg);
  ```

## Build and Run

Perform the following steps from the sample root directory to build and run the sample. The data generation and verification scripts require Python 3 and NumPy.

- Configure environment variables

  ```bash
  source ${install_path}/set_env.sh
  ```

  `${install_path}` is the CANN package installation directory, for example, `/usr/local/Ascend/cann`.

- Build and run

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py \
    --source input/input_x.bin --indexes input/input_y.bin --golden output/golden.bin
  ./demo
  ```

  The data generation script requires explicit paths for the source, indexes, and golden data; it does not embed file names. The optional `--case` accepts `random`, `identity`, `reverse`, or `repeated` and defaults to `random`. The host program uses a fixed sample directory layout, so use the paths above when running `./demo`.

  When running in a supported NPU environment, `-DCMAKE_ASC_RUN_MODE=sim` can be omitted; the default run mode is `npu`. For simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command above. When switching run modes or chip models, it is recommended to clear the CMake cache in the `build` directory first.

- Build options

  | Option | Description |
  | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | Operator execution mode. Options are `npu` and `sim`; the default is `npu`. |
  | `CMAKE_ASC_ARCHITECTURES` | NPU chip model. This sample supports only `dav-3510`. |

- Execution result

  Successful verification produces the following output:

  ```text
  PASS: 128/128 elements match golden bit-for-bit
  ```

  The host program already includes accuracy verification. You can also run the following command from the `build` directory to independently verify the output file using the Python script:

  ```bash
  python3 ../scripts/verify_result.py --output output/output.bin --golden output/golden.bin
  ```
