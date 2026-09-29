# hifloat8_t SwiGLU Static Tensor Sample

## Overview

This sample implements a SwiGLU activation operator with hifloat8_t inputs and outputs using static Tensor programming. It demonstrates the dequantize-compute-quantize (DQ-Compute-Q) pattern: sigmoid and element-wise multiplication are computed in float, and the result is converted back to hifloat8_t with CAST_ROUND.

## Supported Products and CANN Version

| Product | CANN version |
|------|-------------|
| Ascend 950PR&950DT series products | >= CANN 9.2.0 |

## Directory Structure

```
├── CMakeLists.txt       // Build configuration
├── data_utils.h         // Host file I/O helpers
├── hif8_swiglu.asc      // Ascend C kernel and host invocation
├── scripts
│   ├── gen_data.py      // Generate hifloat8_t inputs and reference output
│   ├── hif8.py          // hifloat8_t CAST_ROUND encoder/decoder
│   └── verify_result.py // Decode and verify the output
└── README_en.md
```

## Operator Specification

The operator processes 2048 elements in one core. All tensors use ND format.

| Name | Input/Output | Shape | Data type |
| --- | --- | --- | --- |
| g | Input | (1, 2048) | hifloat8_t |
| u | Input | (1, 2048) | hifloat8_t |
| z | Output | (1, 2048) | hifloat8_t |

The computation is:

$$z_i = Q(D(g_i) \cdot \sigma(D(g_i)) \cdot D(u_i))$$

Here, $D$ is a lossless hifloat8_t-to-float conversion and $Q$ is a float-to-hifloat8_t CAST_ROUND conversion.

## Implementation

- `LocalTensor` constructors are used to plan UB byte offsets directly without TPipe/TQue.
- `DataCopy` moves hifloat8_t data between Global Memory and UB.
- RegBase `Cast` converts between hifloat8_t and float, and the SwiGLU computation runs in registers.
- `CAST_ROUND` quantizes the result back to hifloat8_t without storing intermediate float data in UB.
- `SetFlag/WaitFlag` synchronizes the MTE2, Vector, and MTE3 pipelines.

## Build and Run

Configure the CANN environment first, then run the following commands from the sample directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..
python3 scripts/gen_data.py
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

For NPU simulation, configure CMake as follows:

```bash
cd build
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..
```

The verification script decodes the output to float and compares it with the CAST_ROUND reference.
