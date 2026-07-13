# HiFloat8 Quantized Matrix Multiplication Sample

## Overview

This sample implements the `QuantMatmulHifp8` operator. Input matrices A and B use HiFloat8. MMAD performs
matrix multiplication with a float bias, and Fixpipe converts the result to bfloat16 using a per-channel scale.
The operator computes `C = (A * B + bias) * scale`. During execution, `DumpTensor` inspects the bias and scale
in L1 Buffer, Bias Table Buffer, and Fixpipe Buffer, focusing on special-buffer data printing on dav-3510.

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
│   │   ├── gen_data.py          // Generates input and golden data
│   │   └── verify_result.py     // Verifies operator output and dump records
│   ├── CMakeLists.txt           // Build project file
│   ├── data_utils.h             // Binary file read and write helpers
│   ├── special_buffer_dump.asc  // Quantized MatMul, DumpTensor calls, and host launch
│   ├── README.md                // Chinese sample document
│   └── README_en.md             // English sample document
```

## Sample Description

- Function:

  The sample uses Cube programming to implement HiFloat8 quantized matrix multiplication with a bias and a
  per-channel scale. Each output element `C[m, n]` is calculated as follows:

  ```text
  C[m, n] = (sum(A[m, k] * B[k, n]) + bias[n]) * scale[n]
  ```

  MMAD initializes the C matrix with the float bias stored in Bias Table Buffer and performs HiFloat8 matrix
  multiplication and accumulation. Fixpipe reads the per-channel scale from Fixpipe Buffer, converts the float
  accumulation result to bfloat16, and writes it to Global Memory. `DumpTensor` observes the actual bias and
  scale data along this computation path.

- Specifications:

  The sample uses fixed dimensions `M=16, K=32, N=16`. The tensor specifications are listed below:

  <table>
  <tr><td rowspan="1" align="center">OpType</td><td colspan="4" align="center">QuantMatmulHifp8</td></tr>
  <tr><td rowspan="5" align="center">Inputs</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">hifloat8</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">hifloat8</td><td align="center">ND</td></tr>
  <tr><td align="center">bias</td><td align="center">[N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">scale</td><td align="center">[N]</td><td align="center">uint64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Output</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">bfloat16</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel function</td><td colspan="4" align="center">QuantMatmulHifp8</td></tr>
  </table>

- Implementation:

  One AI Core executes the operator as follows:

  1. Move HiFloat8 matrices A and B from Global Memory to L1, then to L0A and L0B.
  2. Move the float bias from Global Memory to L1, then to Bias Table Buffer. MMAD uses Bias Table Buffer as
     the initial C matrix and computes `A * B + bias`.
  3. Move the per-channel `uint64_t` scale from Global Memory to L1, then to Fixpipe Buffer.
  4. Fixpipe reads the scale, converts the float L0C result to bfloat16, and writes it to Global Memory.
  5. The host reads inputs from binary files, executes the operator, and writes `output/output.bin`.

  A and B contain the HiFloat8 value `1.0`, the bias contains `0` through `15`, even columns use scale `1.0`,
  and odd columns use scale `0.5`. The expected value in column `n` of every output row is therefore
  `(32 + bias[n]) * scale[n]`.

- DumpTensor usage:

  Before the bias and scale enter their compute units, the operator dumps data from L1 Buffer and the target
  special buffer. Comparing the same data before and after each transfer verifies the data path. `desc`
  identifies a dump record, and `dumpSize` is the number of elements to print. The sample uses these points:

  | desc | Position | Operator usage |
  |------|----------|----------------|
  | 101 | L1 Buffer | Float bias before it is moved to Bias Table Buffer |
  | 201 | Bias Table Buffer | Float bias used by MMAD to initialize the C matrix |
  | 102 | L1 Buffer | Per-channel `uint64_t` scale before it is moved to Fixpipe Buffer |
  | 301 | Fixpipe Buffer | `uint64_t` scale read by Fixpipe during output quantization |

  `scripts/gen_data.py` generates all four input files and `output/golden.bin`. `scripts/verify_result.py`
  verifies both the operator output and all four `DumpTensor` records. Dump expectations are read directly from
  the Python-generated bias and scale files.

## Fixpipe Buffer Data Rule

On dav-3510, data visible after a quantization parameter is written from L1 to Fixpipe Buffer follows this rule:

For data types, address spaces, and transfer constraints of the L1-to-Fixpipe path, see
[L1-to-Fixpipe Buffer DataCopy](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/L1ToFixpipe-Buffer数据搬运（DataCopy）.md).
For the data flow that uses quantization parameters from Fixpipe Buffer, see
[Efficient On-the-fly Quantization with FP Buffer](../../../../../docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过FP-Buffer存放量化参数实现高效随路量化.md).

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
python3 ../scripts/gen_data.py
./demo 2>&1 | tee run.log
python3 ../scripts/verify_result.py output/output.bin output/golden.bin run.log \
    input/bias_gm.bin input/scale_gm.bin
```

`${install_path}` is the CANN installation directory. The default installation root is `/usr/local/Ascend`.

A successful verification prints:

```text
Output result check: PASS
Bias L1 raw input check: PASS
Bias Table raw data check: PASS
Fixpipe L1 raw input check: PASS
Fixpipe quant visible-mask check: PASS
test pass!
```
