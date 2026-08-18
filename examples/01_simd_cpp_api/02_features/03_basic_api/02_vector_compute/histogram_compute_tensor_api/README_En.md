# Histogram Compute Sample Implemented with Tensor API

## Overview

This sample uses Tensor API to calculate histograms for 256 `uint8_t` elements and writes four result rows to GM. Python scripts generate golden data and require exact element-wise results.

## Supported Products

- Ascend 950PR
- Ascend 950DT

## Directory Structure

```text
histogram_compute_tensor_api
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── histogram_compute_tensor_api.asc
├── README.md
└── README_En.md
```

## Sample Description

| Input/Output | Specification |
| :--- | :--- |
| `input` | `[256]`, `uint8_t` |
| `output` | `[4, 128]`, `uint16_t` |

Every destination lane starts from 3 and the histogram result accumulates from that value.

| Output Row | Range and Mode |
| :--- | :--- |
| 0 | `low + frequency`, values 0 to 127 |
| 1 | `high + frequency`, values 128 to 255 |
| 2 | `low + cumulative`, values 0 to 127 |
| 3 | `high + cumulative`, values 128 to 255 |

## Build and Run

Run the following in this directory:

```bash
source /usr/local/Ascend/cann/set_env.sh
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
