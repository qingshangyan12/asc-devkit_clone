# MatMul C API Sample

## Overview

This sample runs FP32 matrix multiplication with bias on one Cube Core:

```text
dst[i, j] = sum(src0[i, k] * src1[k, j]) + weight[j]
```

The fixed input shapes are `src0[30, 40]`, `src1[40, 70]`, and `weight[1, 70]`; the output shape is `dst[30, 70]`. All tensors use float32 in ND format and neither matrix is transposed.

The Device pipeline is `GM(ND) -> L1(Nz) -> L0A/L0B -> Cube Mmad(+BiasTable) -> L0C -> GM(ND)`. It launches one Cube Core and performs no element-wise Host computation or tiling.

The data generation and verification scripts use only the Python standard library; NumPy is not required.

## Supported Products and Versions

| Product | CANN Version |
|---------|--------------|
| Ascend 950PR / Ascend 950DT | >= 9.1.0 |

## Build and Run

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

Add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command to use NPU simulation. A successful precision check prints `test pass!`.

See [TEST_REPORT.md](TEST_REPORT.md) for the complete Ascend 950 test environment, precision data, end-to-end timing notes, and reproduction commands.
