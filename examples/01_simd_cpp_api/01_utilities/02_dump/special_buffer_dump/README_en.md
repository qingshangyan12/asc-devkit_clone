# 3510 Quantized MatMul Special Buffer Dump Sample

## Overview

This sample uses a HiFloat8 quantized matrix multiplication operator to demonstrate `DumpTensor` on L1 Buffer,
Bias Table Buffer, and Fixpipe Buffer on Ascend 950PR/Ascend 950DT. The operator computes
`C = (A * B + bias) * scale`: A and B use HiFloat8, accumulation and bias use float, and Fixpipe applies a
per-channel scale while converting the result to bfloat16.

## Supported Products and CANN Software Versions

| Product | CANN Software Version | HDK Version |
|---------|-----------------------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 | >= 25.7.0 |

The repository documentation for `DumpTensor` and `asc_dump` requires HDK 25.7.0 or later when printing L1
Buffer data on Ascend 950PR/Ascend 950DT. This is therefore the minimum HDK version for the complete sample.
The L1, Bias Table Buffer, and Fixpipe Buffer dumps all use the dav-3510 debug-bus path.

## Directory Structure

```text
├── special_buffer_dump
│   ├── scripts
│   │   └── verify_dump.py       // Parses and verifies the dump log
│   ├── CMakeLists.txt           // Build project file
│   ├── special_buffer_dump.asc  // Quantized MatMul, DumpTensor calls, and host verification
│   ├── README.md                // Chinese sample document
│   └── README_en.md             // English sample document
```

## Operator Flow

The sample uses fixed dimensions `M=16, K=32, N=16` and runs the following flow on one AI Core:

1. Move HiFloat8 matrices A and B from Global Memory to L1, then to L0A and L0B.
2. Move the float bias from Global Memory to L1 and dump it with `desc=101`. Move it to Bias Table Buffer and
   dump it with `desc=201`. MMAD uses Bias Table Buffer as the initial C matrix and computes `A * B + bias`.
3. Move the per-channel `uint64_t` scale from Global Memory to L1 and dump it with `desc=102`. Move it to
   Fixpipe Buffer and dump it with `desc=301`.
4. Fixpipe reads the scale, quantizes the float L0C result to bfloat16, and writes it to Global Memory.
5. The host verifies the matrix output, while `scripts/verify_dump.py` verifies all four DumpTensor records.

A and B contain the HiFloat8 value `1.0`, the bias contains `0` through `15`, even columns use scale `1.0`, and
odd columns use scale `0.5`. The expected value in column `n` of every output row is therefore
`(32 + bias[n]) * scale[n]`.

## Dump Identifiers

| desc | Position | Operator usage |
|------|----------|----------------|
| 101 | L1 Buffer | Float bias before it is moved to Bias Table Buffer |
| 201 | Bias Table Buffer | Float bias used by MMAD to initialize the C matrix |
| 102 | L1 Buffer | Per-channel `uint64_t` scale before it is moved to Fixpipe Buffer |
| 301 | Fixpipe Buffer | `uint64_t` scale read by Fixpipe during output quantization |

## Fixpipe Buffer Data Rule

On dav-3510, data visible after a quantization parameter is written from L1 to Fixpipe Buffer follows this rule:

```text
fixpipe_value = l1_value & 0x00007fe0ffffe0ff
```

The mask retains bits `[7:0]`, `[31:13]`, and `[46:37]`, and clears bits `[12:8]`, `[36:32]`, and `[63:47]`.
This sample creates valid per-channel parameters: the low 32 bits contain a truncated float scale and bit 46 is
the Sign flag. The `desc=301` result must therefore equal the `desc=102` value after applying the mask.

The high 16 bits of a logical Fixpipe Buffer address select the parameter area: `0` selects quantization
parameters and `1` selects ReLU parameters. The low 16 bits select an offset in that area. This sample uses the
quantization parameter area at address `0`.

## Build and Run

Run the following commands in the sample root directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
./demo 2>&1 | tee run.log
python3 ../scripts/verify_dump.py run.log
```

`${install_path}` is the CANN installation directory. The default installation root is `/usr/local/Ascend`.

A successful verification prints:

```text
Bias L1 raw input check: PASS
Bias Table raw data check: PASS
Fixpipe L1 raw input check: PASS
Fixpipe quant visible-mask check: PASS
QuantMatmul result check: PASS
test pass!
```
