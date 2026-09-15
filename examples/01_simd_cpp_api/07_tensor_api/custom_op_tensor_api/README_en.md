# Tensor API Custom Op Sample

## Overview

This sample implements a fixed-shape QKV projection with the Tensor API and demonstrates how to define and invoke a custom Copy Operation. All input batches share one projection weight. The computation is distributed across Cube cores and tiled along K:

```text
input_flat = reshape(input, [B * S, H])
output = input_flat * weight + bias
```

While moving the input from Global Memory to Local Memory L1, the custom `copy_slice_gm_to_l1` Operation directly converts the batched `[B, S, H]` ND layout into a flattened `[B * S, H]` NZ layout. Standard Tensor APIs then move the operands to L0A, L0B, and BiasTable, perform matrix multiplication, and store the result.

## Supported Products

| Product | NPU Architecture |
|---------|------------------|
| Ascend 950PR/Ascend 950DT | dav-3510 |

## Directory Structure

```text
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── custom_op_tensor_api.asc
├── data_utils.h
├── README_en.md
└── README.md
```

## Sample Specification

| Input/Output | Data Type | Shape | Format |
|--------------|-----------|-------|--------|
| input | half | [16, 64, 256] | ND |
| weight | half | [256, 768] | ND |
| bias | half | [768] | ND |
| output | half | [1024, 768] | ND |

The output can also be interpreted logically as `[16, 64, 768]` without an additional data movement operation.

## Processing Flow

1. Each Cube core handles one `[256, 384]` output region and derives its batch, M, and N offsets from the block index.
2. `copy_slice_gm_to_l1` flattens an input `[2, 64, 64]` ND slice into `[128, 64]` NZ.
3. `copy_gm_to_l1` moves the current K/N weight tile and bias tile into L1.
4. `copy_l1_to_l0a`, `copy_l1_to_l0b`, and `copy_l1_to_biastable` prepare the matrix operands.
5. Each core uses nested M/N/K loops with `[128, 192, 64]` base tiles, performing `2 x 2 x 4` tiled computations. The first K tile of each M/N tile initializes the result and adds bias; later K tiles accumulate.
6. `copy_l0c_to_gm` converts the output tile to ND and writes it back to GM.

The matrix multiplication uses `M=1024`, `K=256`, and `N=768`. With `single_core_m=256` and `single_core_n=384`, the workload is distributed across eight Cube cores. The base tile is configured as `base_m=128`, `base_n=192`, and `base_k=64`, so `single_core_m` differs from `base_m`.

The custom Operation is connected to the Tensor API Atom mechanism through `copy_traits`. Its 3510-specific implementation calls the `asc_set_gm2l1_nz_para` and `asc_copy_gm2l1_nd2nz` C APIs to perform the batched-ND-to-flattened-NZ transfer.

## Build and Run

Run the following commands in this directory:

```bash
source ${ASCEND_HOME_PATH}/set_env.sh
rm -rf build
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
