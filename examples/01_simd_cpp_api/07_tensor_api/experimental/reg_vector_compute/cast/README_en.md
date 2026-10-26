# Cast Tensor API Sample

## Overview

This sample combines the experimental Tensor API register load/store operations with current C API conversion interfaces to perform element-wise data type conversion. All scenarios use one-dimensional input and output Tensors with shape `[512]` in logical elements.

The sample covers seven conversion scenarios (numbered 1–7) between floating-point types and between integer and floating-point types. It also demonstrates the `unpack` and `unpack4` memory access modes together with `deinterleave` data reordering used for conversions between types of different widths. The computation is as follows:

```text
y[i] = convert<To>(x[i])
```

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── CMakeLists.txt          // Build configuration
├── README.md               // Sample description
├── cast.asc                   // Operator kernel, host invocation, and result verification
└── scripts
    └── gen_data.py         // Script for generating input and golden data
```

## Sample Description

- Sample Function:

  `SCENARIO_NUM` selects the data type conversion scenario to compile and run. The input type, output type, layout, rounding mode, and saturation mode for each scenario are listed below.

  | SCENARIO_NUM | Input Type | Output Type | Layout | Round | Sat | Description |
  | --- | --- | --- | --- | --- | --- | --- |
  | 1 | `half` | `int32_t` | `zero` | `floor` | `not_sat` | Rounds down and converts the result to a 32-bit signed integer. |
  | 2 | `float` | `int16_t` | `zero` | `rint` | `sat` | Rounds to the nearest integer and saturates the result to the `int16_t` range. |
  | 3 | `int8_t` | `int32_t` | `zero` | `rint` | `not_sat` | Extends an 8-bit signed integer to a 32-bit signed integer. |
  | 4 | `int32_t` | `uint8_t` | `zero` | `rint` | `sat` | Saturates the value to `[0, 255]` and converts it to an 8-bit unsigned integer. |
  | 5 | `bfloat16_t` | `float` | `zero` | `rint` | `not_sat` | Extends `bfloat16_t` to `float`. |
  | 6 | `float` | `bfloat16_t` | `zero` | `rint` | `not_sat` | Rounds `float` to `bfloat16_t`. |
  | 7 | UB: `int4b_t`; register: `int4x2_t` | `half` | `zero/one/two/three` | `rint` | `not_sat` | Directly loads 512 packed INT4 values and converts all four quarters. |

- Sample Specifications:

  <table>
    <tr>
      <td align="center">Sample Type (OpType)</td>
      <td colspan="4" align="center">cast</td>
    </tr>
    <tr>
      <td rowspan="2" align="center">Input</td>
      <td align="center">name</td>
      <td align="center">shape</td>
      <td align="center">data type</td>
      <td align="center">layout</td>
    </tr>
    <tr>
      <td align="center">x</td>
      <td align="center">[512]</td>
      <td align="center">half/float/int8_t/int32_t/bfloat16_t/int4b_t</td>
      <td align="center">One-dimensional</td>
    </tr>
    <tr>
      <td rowspan="2" align="center">Output</td>
      <td align="center">name</td>
      <td align="center">shape</td>
      <td align="center">data type</td>
      <td align="center">layout</td>
    </tr>
    <tr>
      <td align="center">y</td>
      <td align="center">[512]</td>
      <td align="center">int32_t/int16_t/uint8_t/float/bfloat16_t/half</td>
      <td align="center">One-dimensional</td>
    </tr>
    <tr>
      <td align="center">Kernel Name</td>
      <td colspan="4" align="center">cast</td>
    </tr>
  </table>

  | SCENARIO_NUM | `input/input_x.bin` | Input Size | `output/output.bin` / `output/golden.bin` | Output Size |
  | --- | --- | --- | --- | --- |
  | 1 | 512 `half` elements | 1024 B | 512 `int32_t` elements | 2048 B |
  | 2 | 512 `float` elements | 2048 B | 512 `int16_t` elements | 1024 B |
  | 3 | 512 `int8_t` elements | 512 B | 512 `int32_t` elements | 2048 B |
  | 4 | 512 `int32_t` elements | 2048 B | 512 `uint8_t` elements | 512 B |
  | 5 | 512 `bfloat16_t` | 1024 B | 512 `float` elements | 2048 B |
  | 6 | 512 `float` elements | 2048 B | 512 `bfloat16_t` | 1024 B |
  | 7 | 512 packed INT4 values (256 bytes) | 256 B | Four quarters of 128 `half` elements, 512 in total | 1024 B |

  Scenario 7 uses a direct load with source register type `reg_tensor<int4x2_t>`.
  Layout `q` converts logical input elements `8 * i + 2 * q` and `8 * i + 2 * q + 1`,
  the two INT4 values in byte q of each four-byte group. A byte-granularity full mask activates
  all source positions; results are stored in quarter order.
  Set both `SCENARIO_NUM` and `scenario_num` to 7 in the simulator commands below to run it.

  Scenario 7 constructs `int4b_t` Tensors directly in GM and UB, both with layouts
  describing 512 logical elements. A normal `copy(x_ub, x_gm)` uses the shared B4
  copy implementation to transfer 256 packed bytes without a `uint8_t` view.
  Low nibbles represent even elements; high nibbles represent odd elements.
  A direct `load` loads all 512 logical elements. Each of the four layouts converts
  128 elements, and normal `store` calls write four contiguous output sections. The input covers all
  256 packed-byte combinations, and every output half is compared bit-for-bit.
  The load result uses `auto` to infer the register type as `reg_tensor<int4x2_t>`.
  The shared Tensor pointer implementation converts
  logical offsets to byte addresses; the sample does not repeat that conversion.
  Compilation uses the headers installed in the CANN package without local repository overrides.

- Sample Implementation:

  - Key Kernel Steps

    1. Use `asc::te::make_tensor`, `asc::te::make_mem_ptr`, and a one-dimensional layout with `make_shape(512)` and `make_stride(1)` to construct the GM and UB tensors.
    2. Wrap the MTE2 pipeline with `asc_lock/asc_unlock`, and use `asc::te::copy` to move the input from GM to UB.
    3. Calculate the number of elements processed by each register operation based on the larger width of the source and destination types, and use `asc::te::experimental::update_mask` to set the valid-element mask.
    4. Use normal `load/store` for equal-width conversions, `unpack` or `unpack4` for widening loads, and use `deinterleave` to compact narrowing results before a normal `store`.
    5. Call the current C API conversion overloads with `ASC_POSITION_EVEN` or `ASC_DISPERSE_FIRST_QUARTER`.
    6. Lock the Vector and MTE3 pipelines in sequence with the same mutex, and then move the result from UB to GM.

    The core conversion code is as follows:

    ```cpp
    auto src_reg = asc::te::experimental::load(src, coord);
    src_reg = src_reg.with_mask(asc::te::experimental::update_mask<WiderT>(remain));

    auto dst_reg = asc::te::experimental::cast<int32_t>(src_reg);
    asc::te::experimental::store(dst, coord, dst_reg);
    ```

    A narrowing conversion uses layout zero for the register result. A 2x narrowing conversion applies one `deinterleave` operation to extract even positions, while a 4x narrowing conversion applies two to extract every fourth element before writing contiguous destination data with a normal `store`.

## Build and Run

Run the following steps in the sample root directory to build and execute the operator.

- Configure environment variables
  Configure environment variables based on the [installation method](https://gitcode.com/cann/asc-devkit/blob/master/docs/en/quick_start.md) of the CANN development kit on the current environment. **Currently only [CANN master](https://gitcode.com/cann/asc-devkit/blob/master/docs/en/quick_start.md) is supported.**

  ```bash
  source ${install_path}/set_env.sh
  ```

  Replace `${install_path}` with the actual CANN software package installation directory.

- Run the Sample

  The following example runs scenario 1:

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py --scenario_num=${SCENARIO_NUM}
  ./demo
  ```

  All scenarios use the same build and execution steps; only `SCENARIO_NUM` changes. Valid numbers are 1–7; set `SCENARIO_NUM=7` to run scenario 7. The `scenario_num` passed to the data generation script must match the `SCENARIO_NUM` used during compilation.

  To use NPU simulation, add the following build option:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=${SCENARIO_NUM} ..
  ```

  The build uses the Tensor API and C API headers installed in CANN. Synchronize the repository code to the corresponding package directories before validating local changes.

  When switching the run mode, chip model, or scenario, clear the CMake cache in the `build` directory before reconfiguring the project.

- Build Options

  | Option | Description |
  | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | Operator execution mode. Available values are `npu` and `sim`. The default is `npu`. |
  | `CMAKE_ASC_ARCHITECTURES` | NPU chip model. The default is `dav-3510`. |
  | `SCENARIO_NUM` | Required data type conversion scenario number. Valid numbers are 1–7. |

- Expected Result

  The following output indicates successful execution:

  ```text
  test pass!
  ```
