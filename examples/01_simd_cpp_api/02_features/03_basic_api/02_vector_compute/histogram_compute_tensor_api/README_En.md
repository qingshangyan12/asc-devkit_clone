# Histogram Compute Sample Implemented with Tensor API

## Overview

This sample demonstrates register-based histogram compute with Tensor API. A `uint8_t` register represents input values from 0 to 255. The histogram API divides this range into two 128-bin regions, `low` and `high`, and supports frequency and cumulative modes.

The same input is evaluated with four template-argument combinations. This shows how the initial destination register contributes to the result and how template arguments select the underlying compute API at compile time.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
histogram_compute_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── histogram_compute_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## Sample Description

The input contains 256 `uint8_t` elements. The output contains four rows of 128 `uint16_t` elements:

| Output Row | Range and Mode |
| --- | --- |
| `frequency/low` | Frequencies for values 0 to 127 |
| `frequency/high` | Frequencies for values 128 to 255 |
| `cumulative/low` | Cumulative frequencies for values 0 to 127 |
| `cumulative/high` | Cumulative frequencies for values 128 to 255 |

Every destination lane is initialized to 3, and each histogram operation accumulates from this value. The kernel copies the input from GM to UB, loads it as `reg_tensor<uint8_t>`, calls `histograms`, and writes the `reg_tensor<uint16_t>` results back to GM. Verification requires every output element to match the NumPy result exactly.

## Build and Run

Run the following steps in the sample directory.

- Configure the environment for the installed CANN toolkit.

  ```bash
  # Default path for the root user
  source /usr/local/Ascend/cann/set_env.sh

  # Default path for a non-root user
  source $HOME/Ascend/cann/set_env.sh

  # Custom installation path
  source ${install_path}/cann/set_env.sh
  ```

- Build and run the sample.

  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build -j
  cd build
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

A successful run prints:

```text
histogram_compute_tensor_api executed successfully.
test pass!
```
