# Reg Tensor operator+ Sample

## Overview

This sample implements element-wise addition of two float vectors with the experimental Tensor API Reg Tensor `operator+` interface. It moves the inputs from GM to UB, loads them into registers, performs the addition, and stores the result back to UB and GM.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | CANN master |

> **Note:** This sample uses experimental interfaces. Enable `CANN_ASC_USE_EXPERIMENTAL` when building it.

## Directory Structure

```text
reg_vector_add_tensor_api
├── scripts
│   ├── gen_data.py                # Generates input and golden data
│   └── verify_result.py           # Compares the result with golden data
├── CMakeLists.txt                 # Build configuration
├── data_utils.h                   # Data input and output helpers
├── reg_vector_add_tensor_api.asc  # Ascend C sample implementation
├── README.md                      # Chinese documentation
└── README_en.md                   # English documentation
```

## Sample Description

The sample performs the following computation:

$$
dst_i = src0_i + src1_i
$$

Both inputs and the output are float vectors with shape `[64]`. The sample runs on one Vector core and performs these steps:

1. Move both input vectors from GM to UB with `copy_gm_to_ub`.
2. Load the UB data as Reg Tensors with `all_mask` and `load`.
3. Perform element-wise addition with the experimental `operator+`.
4. Store the result with `store` and move it back to GM with `copy_ub_to_gm`.

## Build and Run

Configure the environment according to the CANN development kit [installation guide](../../../../../docs/en/quick_start.md#prepare&install), then run the following commands in this sample directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py
```

For NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the cmake command.

A successful verification prints:

```text
test pass! verified float Reg Tensor operator+ results.
```
