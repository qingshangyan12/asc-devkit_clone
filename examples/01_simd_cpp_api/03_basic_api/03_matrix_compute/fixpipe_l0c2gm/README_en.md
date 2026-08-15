# fixpipe_l0c2gm Example

## Overview

This example introduces how to use Fixpipe to transfer matrix multiplication results from L0C Buffer to GM (Global Memory), supporting multiple output formats (Nz, ND, DN), data type conversion, inline quantization, ReLU, and ChannelSplit features. These interfaces efficiently transfer matrix multiplication computation results from L0C to global memory, with support for various data format conversions and preprocessing capabilities.

For Ascend 950PR/Ascend 950DT (dav-3510 architecture), this example covers all data type combinations supported by the Fixpipe L0C-to-GM path, including float and int32_t L0C source types, and output types including int8_t, uint8_t, half, bfloat16_t, fp8_e4m3fn_t, hifloat8_t, float, and int32_t. Each combination supports NoQuant, Cast, Scalar quantization, and Tensor quantization modes, tested with both ND and NZ output formats, totaling 59 scenarios.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure

```
├── fixpipe_l0c2gm
│   ├── figures                        // Illustrations
│   ├── scripts
│   │   ├── gen_data.py                // Script for generating input data and ground truth data
│   │   ├── gen_data_f322s8.py         // float→int8 quantization reference script (standalone)
│   │   ├── gen_data_s322s8.py         // int32→int8 quantization reference script (standalone)
│   │   ├── gen_data_s322f16.py        // int32→half quantization reference script (standalone)
│   │   └── verify_result.py           // Script for verifying whether output data matches ground truth data
│   ├── CMakeLists.txt                 // Build project file
│   ├── data_utils.h                   // Data read/write functions
│   ├── fixpipe_l0c2gm.asc             // Ascend C example implementation & invocation example
│   └── README.md                      // Example documentation
```

## FixpipeParamsV220 vs FixpipeParamsArch3510 Structure Comparison

Different products support different parameter structures:
- **Ascend 950PR/Ascend 950DT**: Supports both `FixpipeParamsV220` and `FixpipeParamsArch3510` parameter structures, `FixpipeParamsArch3510` is recommended
- **Atlas A3 Training/Inference Series, Atlas A2 Training/Inference Series**: Only supports `FixpipeParamsV220`

This example selects different architectures through the compilation parameter `CMAKE_ASC_ARCHITECTURES`, automatically selecting the corresponding parameter structure based on architecture:
- `dav-2201` architecture: Uses `FixpipeParamsV220`
- `dav-3510` architecture: Uses `FixpipeParamsArch3510`

<a name="table1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 1: Parameter Structure Comparison</span></caption>
<tr><td rowspan="1" align="center">Member Name</td><td align="center">FixpipeParamsV220</td><td align="center">FixpipeParamsArch3510</td><td align="center">Description</td></tr>
<tr><td align="center"><code>nSize</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Output matrix size in N direction</td></tr>
<tr><td align="center"><code>mSize</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Output matrix size in M direction</td></tr>
<tr><td align="center"><code>srcStride</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Start address offset of adjacent Z layouts in source Nz matrix</td></tr>
<tr><td align="center"><code>dstStride</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Start address offset of adjacent Z layouts in destination matrix (Nz format) or elements per row (ND/DN format)</td></tr>
<tr><td align="center"><code>quantPre</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Quantization mode control</td></tr>
<tr><td align="center"><code>deqScalar</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Scalar quantization parameter</td></tr>
<tr><td align="center"><code>reluEn</code></td><td align="center">Yes</td><td align="center">Yes</td><td>ReLU switch</td></tr>
<tr><td align="center"><code>unitFlag</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Mmad and Fixpipe fine-grained parallelism control</td></tr>
<tr><td align="center"><code>isChannelSplit</code></td><td align="center">Yes</td><td align="center">Yes</td><td>Channel split switch</td></tr>
<tr><td align="center"><code>ndNum</code> / <code>srcNdStride</code> / <code>dstNdStride</code></td><td align="center">Yes</td><td align="center">Yes (in <code>TransformParams</code>)</td><td>NZ2ND scenario multi-matrix transfer parameters, independent members in V220, integrated into <code>TransformParams</code> in Arch3510</td></tr>
<tr><td align="center"><code>dnNum</code> / <code>srcNzMatrixStride</code> / <code>dstDnMatrixStride</code> / <code>srcNzC0Stride</code></td><td align="center">No</td><td align="center">Yes (in <code>TransformParams</code>)</td><td>NZ2DN scenario multi-matrix transfer parameters, only Arch3510 supported</td></tr>
<tr><td align="center"><code>TransformParams</code></td><td align="center">No</td><td align="center">Yes</td><td>Template parameter-based type selector, auto-selects parameter type based on CO2Layout</td></tr>
<tr><td align="center"><code>dualDstCtrl</code></td><td align="center">No</td><td align="center">Yes</td><td>Dual-target mode control, supports M or N dimension splitting</td></tr>
<tr><td align="center"><code>subBlockId</code></td><td align="center">No</td><td align="center">Yes</td><td>Target UB number in single-target mode</td></tr>
</table>

## Scenario Description

This example selects different output scenarios through the compilation parameter `SCENARIO_NUM`. The meanings of different SCENARIO_NUM values are shown in the table below. All scenarios are based on the same matrix multiplication specification: [M, N, K] = [128, 256, 128], with kernel function name `fixpipe_l0c2gm`.

- Scenarios 1-7 support both `dav-2201` and `dav-3510` architectures (scenario 3 only `dav-3510`)
- Scenarios 8-59 only support `dav-3510` architecture (using `FixpipeParamsArch3510`-specific data types and quantization modes)

<a name="table2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 2: Meaning of Different scenarioNum Values</span></caption>
<tr><td align="center">scenarioNum</td><td align="center">Input Type</td><td align="center">L0C Data Type</td><td align="center">Output Data Type</td><td align="center">quantPre</td><td align="center">Quant Mode</td><td align="center">Output Format</td><td align="center">Description</td></tr>
<tr><td align="center">1</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">Nz</td><td align="center">Basic NZ output</td></tr>
<tr><td align="center">2</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">ND</td><td align="center">Basic ND output</td></tr>
<tr><td align="center">3</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">DN</td><td align="center">Basic DN output (3510 only)</td></tr>
<tr><td align="center">4</td><td align="center">half</td><td align="center">float</td><td align="center">int8_t</td><td align="center">QF322B8_PRE</td><td align="center">scalar</td><td align="center">ND</td><td align="center">float→int8 quant</td></tr>
<tr><td align="center">5</td><td align="center">half</td><td align="center">float</td><td align="center">int8_t</td><td align="center">VQF322B8_PRE</td><td align="center">tensor</td><td align="center">ND</td><td align="center">float→int8 quant</td></tr>
<tr><td align="center">6</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">ND</td><td align="center">ReLU enabled</td></tr>
<tr><td align="center">7</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">Nz</td><td align="center">ChannelSplit enabled</td></tr>
<tr><td align="center">8-9</td><td align="center">half</td><td align="center">float</td><td align="center">int8_t</td><td align="center">QF/VQF322B8_PRE</td><td align="center">scalar/tensor</td><td align="center">Nz</td><td align="center">float→int8 NZ output</td></tr>
<tr><td align="center">10-13</td><td align="center">half</td><td align="center">float</td><td align="center">uint8_t</td><td align="center">QF/VQF322B8_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→uint8 quant</td></tr>
<tr><td align="center">14-19</td><td align="center">half</td><td align="center">float</td><td align="center">half</td><td align="center">F322F16 / QF/VQF322F16_PRE</td><td align="center">cast/scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→half cast/quant</td></tr>
<tr><td align="center">20-25</td><td align="center">half</td><td align="center">float</td><td align="center">bfloat16_t</td><td align="center">F322BF16 / QF/VQF322BF16_PRE</td><td align="center">cast/scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→bf16 cast/quant</td></tr>
<tr><td align="center">26-29</td><td align="center">half</td><td align="center">float</td><td align="center">fp8_e4m3fn_t</td><td align="center">QF/VQF322FP8_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→fp8 quant</td></tr>
<tr><td align="center">30-37</td><td align="center">half</td><td align="center">float</td><td align="center">hifloat8_t</td><td align="center">QF/VQF322HIF8_PRE(_HYBRID)</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→hif8 quant (incl. HYBRID)</td></tr>
<tr><td align="center">38-41</td><td align="center">half</td><td align="center">float</td><td align="center">float</td><td align="center">QF/VQF322F32_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">float→float quant</td></tr>
<tr><td align="center">42-43</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">int32_t</td><td align="center">NoQuant</td><td align="center">-</td><td align="center">ND/Nz</td><td align="center">int32→int32 direct output</td></tr>
<tr><td align="center">44-47</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">int8_t</td><td align="center">REQ8 / VREQ8</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→int8 requant</td></tr>
<tr><td align="center">48-51</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">uint8_t</td><td align="center">REQ8 / VREQ8</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→uint8 requant</td></tr>
<tr><td align="center">52-55</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">half</td><td align="center">DEQF16 / VDEQF16</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→half dequant</td></tr>
<tr><td align="center">56-59</td><td align="center">int8_t</td><td align="center">int32_t</td><td align="center">bfloat16_t</td><td align="center">QS/VQS322BF16_PRE</td><td align="center">scalar/tensor</td><td align="center">ND/Nz</td><td align="center">int32→bf16 quant</td></tr>
</table>

### Data Type Combination Summary

For Ascend 950PR/Ascend 950DT (dav-3510 architecture), the complete data type combinations supported by the Fixpipe L0C→GM path are as follows:

| L0C Source Type | Input A/B Type | Output Type | quantPre Modes | Scenario Numbers |
|----------------|---------------|------------|-----------------|-----------------|
| float | half | float | NoQuant / QF322F32_PRE / VQF322F32_PRE | 1-3, 6-7, 38-41 |
| float | half | int8_t | QF322B8_PRE / VQF322B8_PRE | 4-5, 8-9 |
| float | half | uint8_t | QF322B8_PRE / VQF322B8_PRE | 10-13 |
| float | half | half | F322F16 / QF322F16_PRE / VQF322F16_PRE | 14-19 |
| float | half | bfloat16_t | F322BF16 / QF322BF16_PRE / VQF322BF16_PRE | 20-25 |
| float | half | fp8_e4m3fn_t | QF322FP8_PRE / VQF322FP8_PRE | 26-29 |
| float | half | hifloat8_t | QF322HIF8_PRE / QF322HIF8_PRE_HYBRID / V variants | 30-37 |
| int32_t | int8_t | int32_t | NoQuant | 42-43 |
| int32_t | int8_t | int8_t | REQ8 / VREQ8 | 44-47 |
| int32_t | int8_t | uint8_t | REQ8 / VREQ8 | 48-51 |
| int32_t | int8_t | half | DEQF16 / VDEQF16 | 52-55 |
| int32_t | int8_t | bfloat16_t | QS322BF16_PRE / VQS322BF16_PRE | 56-59 |

### quantPre Quantization Mode Description

| Quant Mode | quantPre Modes | Description | Requires cbufWorkspace |
|-----------|---------------|-------------|----------------------|
| No Quant | NoQuant | Direct output, no quantization | No |
| Cast | F322F16 / F322BF16 | float→half/bfloat16_t type conversion (CAST_RINT) | No |
| Scalar Quant | QF322B8_PRE / QF322F16_PRE / QF322BF16_PRE / QF322FP8_PRE / QF322HIF8_PRE / QF322HIF8_PRE_HYBRID / QF322F32_PRE / DEQF16 / REQ8 / QS322BF16_PRE | Entire matrix uses one quantization parameter (deqScalar) | No |
| Tensor Quant | VQF322B8_PRE / VQF322F16_PRE / VQF322BF16_PRE / VQF322FP8_PRE / VQF322HIF8_PRE / VQF322HIF8_PRE_HYBRID / VQF322F32_PRE / VDEQF16 / VREQ8 / VQS322BF16_PRE | Each column uses independent quantization parameter (cbufWorkspace) | Yes |

### Scenario 1-7 Detailed Description

All scenarios share the same matrix multiplication pipeline; the only difference is the Fixpipe configuration in the CopyOut stage. The common computation flow is described first, followed by per-scenario Fixpipe configurations.

#### Common Matrix Multiplication Pipeline

All scenarios use input matrix specification [M, N, K] = [128, 256, 128], with K axis split into 2 iterations (`K_ROUND=2`), processing `baseK = K / K_ROUND = 64` per iteration.

1. **CopyIn (GM → L1)**: Uses `DataCopy` with `Nd2NzParams` to convert ND format input matrices to NZ format in L1 Buffer
   - A matrix `[128, 64]` half ND → L1 A1 (NZ format), `dstNzC0Stride = CeilAlign(128, 16) = 128`
   - B matrix `[64, 256]` half ND → L1 B1 (NZ format), `dstNzC0Stride = CeilAlign(64, 16) = 64`

2. **SplitA (L1 → L0A)**: NZ → Zz format conversion
   - dav-3510: Uses `LoadData2DParamsV2`, single call for full transfer
   - dav-2201: Uses `LoadData2DParams`, loops `CeilDivision(128, 16) = 8` times per block

3. **SplitB (L1 → L0B)**: NZ → Zn format conversion (with transpose)
   - Uses `LoadData2DParams`, `ifTranspose = true`, loops `CeilDivision(64, 16) = 4` times

4. **Compute (Mmad)**: Matrix multiplication `C = A × B`, result stored in L0C Buffer (CO1)
   - `MmadParams`: `m = 128`, `n = CeilAlign(256, 32) = 256`, `k = 64`
   - First iteration: `cmatrixInitVal = true` (initialize L0C), subsequent: `cmatrixInitVal = false` (K accumulation)

5. **CopyOut (L0C → GM)**: Uses `Fixpipe` to transfer L0C results to GM, with scenario-specific configurations

#### Scenario 1: Output format Nz, output data type float

- **Template instantiation**: `KernelFixpipe<half, float, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [128, 256] float Nz format
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::NZ> fixpipeParams;
  fixpipeParams.mSize = 128;                          // M dimension size
  fixpipeParams.nSize = CeilAlign(256, 16) = 256;    // N dimension aligned to 16
  fixpipeParams.srcStride = CeilAlign(128, 16) = 128; // L0C NZ adjacent Z offset
  fixpipeParams.dstStride = 128 * 16 = 2048;           // GM NZ adjacent Z offset
  // quantPre = NoQuant (default)
  Fixpipe<float, float, CFG_NZ>(cGM, c, fixpipeParams);
  ```
- **Description**: L0C NZ format data is directly transferred to GM in NZ format, with no format conversion or quantization. The NZ format C0 size is 16 elements (float 4 bytes × 16 = 64 bytes), preserving the original fractal structure.
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2NZ.png" width="800">
</p>

#### Scenario 2: Output format ND, output data type float

- **Template instantiation**: `KernelFixpipe<half, float, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [128, 256] float ND format
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> fixpipeParams;
  fixpipeParams.mSize = 128;
  fixpipeParams.nSize = 256;       // ND format uses actual N size, no alignment needed
  fixpipeParams.srcStride = 128;   // L0C NZ adjacent Z offset
  fixpipeParams.dstStride = 256;   // ND format elements per row
  // quantPre = NoQuant (default)
  Fixpipe<float, float, CFG_ROW_MAJOR>(cGM, c, fixpipeParams);
  ```
- **Description**: Converts NZ format data in L0C to ND format output to GM. The NZ2ND conversion rearranges 16×16 fractal data into a contiguous row-major matrix. ND format has no alignment requirements; `nSize` and `dstStride` use actual matrix dimensions.
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2ND.png" width="800">
</p>

#### Scenario 3: Output format DN, output data type float (only supported on Ascend 950PR/Ascend 950DT)

- **Template instantiation**: `KernelFixpipe<half, float, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [256, 128] float DN format (transposed)
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::COLUMN_MAJOR> fixpipeParams;
  fixpipeParams.mSize = 128;
  fixpipeParams.nSize = 256;       // N dimension size
  fixpipeParams.srcStride = 128;   // L0C NZ adjacent Z offset
  fixpipeParams.dstStride = 128;   // DN format adjacent column gap
  fixpipeParams.params = {1, 0, 0, 1};  // {dnNum, srcNzMatrixStride, dstDnMatrixStride, srcNzC0Stride}
  Fixpipe<float, float, CFG_COLUMN_MAJOR>(cGM, c, fixpipeParams);
  ```
- **Description**: Converts NZ format data in L0C to DN (column-major) format output to GM. DN format is exclusive to Ascend 950PR/950DT. The `TransformParams` controls multi-matrix transfer: `dnNum=1` for single matrix, `srcNzC0Stride=1` for source NZ C0 step. The output matrix dimensions are transposed (M×N → N×M).
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2DN.png" width="800">
</p>

#### Scenario 4: Output format ND, output data type int8_t, Scalar quantization enabled

- **Template instantiation**: `KernelFixpipe<half, int8_t, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [128, 256] int8_t ND format
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> fixpipeParams;
  fixpipeParams.mSize = 128;
  fixpipeParams.nSize = 256;
  fixpipeParams.srcStride = 128;
  fixpipeParams.dstStride = 256;
  fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE;  // float→int8 scalar quant
  // deqScalar: float 2.0 bit representation, with sign bit set (bit46=1, signed)
  float quantScalar = 2.0;
  uint64_t deqScalar = *reinterpret_cast<int32_t*>(&quantScalar);
  deqScalar = (deqScalar & ~(1ULL << 46)) | (1ULL << 46);  // sign=1 for int8_t
  fixpipeParams.deqScalar = deqScalar;
  Fixpipe<int8_t, float, CFG_ROW_MAJOR>(cGM, c, fixpipeParams);
  ```
- **Description**: Quantizes L0C float data to int8_t using Scalar quantization. The entire C matrix uses a single quantization parameter (`deqScalar`), which encodes the scale factor (M1=2.0), offset (offset=0), and sign (sign=1, signed int8). The quantization formula is: `output = round(data * scale + offset)`, then saturated to `[-128, 127]`.

#### Scenario 5: Output format ND, output data type int8_t, Vector quantization enabled

- **Template instantiation**: `KernelFixpipe<half, int8_t, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [128, 256] int8_t ND format
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> fixpipeParams;
  fixpipeParams.mSize = 128;
  fixpipeParams.nSize = 256;
  fixpipeParams.srcStride = 128;
  fixpipeParams.dstStride = 256;
  fixpipeParams.quantPre = QuantMode_t::VQF322B8_PRE;  // float→int8 tensor quant
  // Quantization parameters copied from GM to L1 (C1 Buffer)
  LocalTensor<uint64_t> quantAlphaTensor(TPosition::C1, c1Addr, 256);
  CopyQuantAlphaGmToL1(quantAlphaTensor);  // GM → L1 DataCopy
  Fixpipe<int8_t, float, CFG_ROW_MAJOR>(cGM, c, quantAlphaTensor, fixpipeParams);
  ```
- **Description**: Quantizes L0C float data to int8_t using Tensor quantization. Unlike Scalar quantization (scenario 4), Tensor quantization provides independent quantization parameters for each column of the C matrix. The quantization parameters are stored in GM as `uint64_t` elements, each encoding one column's quantization coefficients. Before executing Fixpipe, the parameters must be copied from GM to L1 Buffer (C1) via `DataCopy`, then passed as `cbufWorkspace` to the Fixpipe interface.

#### Scenario 6: Output format ND, output data type float, ReLU enabled

- **Template instantiation**: `KernelFixpipe<half, float, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [128, 256] float ND format (negative values zeroed)
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> fixpipeParams;
  fixpipeParams.mSize = 128;
  fixpipeParams.nSize = 256;
  fixpipeParams.srcStride = 128;
  fixpipeParams.dstStride = 256;
  fixpipeParams.reluEn = true;  // Enable inline ReLU
  // quantPre = NoQuant (default)
  Fixpipe<float, float, CFG_ROW_MAJOR>(cGM, c, fixpipeParams);
  ```
- **Description**: Performs inline ReLU operation during data transfer from L0C to GM, setting all negative values to 0. ReLU is enabled via `reluEn = true` with no additional parameters required. ReLU can be used simultaneously with quantization, but this scenario only enables ReLU without quantization.

#### Scenario 7: Output format Nz, output data type float, ChannelSplit enabled

- **Template instantiation**: `KernelFixpipe<half, float, float, 128, 64, 256>`
- **Input**: A [128, 128] half ND format; B [128, 256] half ND format
- **Output**: C [128, 512] float Nz format (channel split enabled, C0 split from 16 to 8, output columns doubled)
- **Fixpipe configuration**:
  ```cpp
  FixpipeParamsArch3510<CO2Layout::NZ> fixpipeParams;
  fixpipeParams.mSize = 128;
  fixpipeParams.isChannelSplit = true;                    // Enable channel split
  fixpipeParams.nSize = CeilAlign(256, 8) = 256;          // C0 split to 8
  fixpipeParams.srcStride = CeilAlign(128, 16) = 128;     // L0C NZ adjacent Z offset
  fixpipeParams.dstStride = 128 * 8 = 1024;                // GM NZ adjacent Z offset (C0=8)
  Fixpipe<float, float, CFG_NZ>(cGM, c, fixpipeParams);
  ```
- **Description**: Enables channel split during data transfer from L0C to GM, splitting 16×16 small z fractal matrices into two independent 16×8 small z fractal matrices. Channel split only supports float input and output, and only Nz output format. After splitting, C0 changes from 16 to 8, and the output matrix N dimension data doubles (actual data unchanged, only layout differs).

### Scenario 8-41 Detailed Description (float L0C, dav-3510 only)

The following scenarios use half type as matrix multiplication input (A and B), with L0C result as float type. Fixpipe supports various output data types and quantization modes when transferring to GM. Each type combination is tested with both ND and NZ output formats, and both scalar and tensor quantization methods (where applicable).

**Scenario 8-9: float→int8_t, NZ format**
- Scenario 8: QF322B8_PRE (scalar quant), NZ output
- Scenario 9: VQF322B8_PRE (tensor quant), NZ output
- Description: Same quantization mode as scenario 4-5, but with NZ output format

**Scenario 10-13: float→uint8_t**
- Scenario 10-11: ND format, QF322B8_PRE / VQF322B8_PRE
- Scenario 12-13: NZ format, QF322B8_PRE / VQF322B8_PRE
- Description: Quantize float to uint8_t, same quantPre mode as int8_t but sign bit is 0 (unsigned)

**Scenario 14-19: float→half**
- Scenario 14-15: F322F16 (Cast mode), ND/NZ output
- Scenario 16-17: QF322F16_PRE (scalar/tensor), ND output
- Scenario 18-19: QF322F16_PRE (scalar/tensor), NZ output
- Description: Cast mode directly converts float to half (CAST_RINT); quantization mode performs quantized conversion through scale and offset

**Scenario 20-25: float→bfloat16_t**
- Scenario 20-21: F322BF16 (Cast mode), ND/NZ output
- Scenario 22-23: QF322BF16_PRE (scalar/tensor), ND output
- Scenario 24-25: QF322BF16_PRE (scalar/tensor), NZ output
- Description: Cast mode directly converts float to bfloat16_t; quantization mode performs quantized conversion through scale and offset

**Scenario 26-29: float→fp8_e4m3fn_t**
- Scenario 26-27: QF322FP8_PRE (scalar/tensor), ND output
- Scenario 28-29: QF322FP8_PRE (scalar/tensor), NZ output
- Description: Quantize float to fp8_e4m3fn_t format (1 sign + 4 exponent + 3 mantissa, range [-448, 448])

**Scenario 30-37: float→hifloat8_t**
- Scenario 30-31: QF322HIF8_PRE / VQF322HIF8_PRE, ND output (Half to Away Round)
- Scenario 32-33: QF322HIF8_PRE_HYBRID / VQF322HIF8_PRE_HYBRID, ND output (Hybrid Round)
- Scenario 34-37: NZ output, corresponding to the above 4 modes
- Description: Quantize float to hifloat8_t format, supporting two rounding modes

**Scenario 38-41: float→float (quantization mode)**
- Scenario 38-39: QF322F32_PRE / VQF322F32_PRE, ND output
- Scenario 40-41: QF322F32_PRE / VQF322F32_PRE, NZ output
- Description: L0C float data goes through quantization (scale and offset) and is output to GM as float

### Scenario 42-59 Detailed Description (int32_t L0C, dav-3510 only)

The following scenarios use int8_t type as matrix multiplication input (A and B). Mmad computation results are stored in L0C as int32_t type. Fixpipe supports various output data types and quantization modes when transferring to GM.

**Scenario 42-43: int32_t→int32_t, NoQuant**
- Scenario 42: ND output; Scenario 43: NZ output
- Description: Directly transfer int32_t L0C results to GM without quantization conversion

**Scenario 44-47: int32_t→int8_t, REQ8 / VREQ8**
- Scenario 44-45: ND output, REQ8 (scalar) / VREQ8 (tensor)
- Scenario 46-47: NZ output, REQ8 / VREQ8
- Description: Requantize int32_t to int8_t, n_shift is a right shift operation

**Scenario 48-51: int32_t→uint8_t, REQ8 / VREQ8**
- Scenario 48-49: ND output; Scenario 50-51: NZ output
- Description: Requantize int32_t to uint8_t, sign bit is 0

**Scenario 52-55: int32_t→half, DEQF16 / VDEQF16**
- Scenario 52-53: ND output; Scenario 54-55: NZ output
- Description: Dequantize int32_t to half through scale parameter for floating-point scaling

**Scenario 56-59: int32_t→bfloat16_t, QS322BF16_PRE / VQS322BF16_PRE**
- Scenario 56-57: ND output; Scenario 58-59: NZ output
- Description: Quantize int32_t to bfloat16_t

## Build and Run

Run the following steps in the root directory of this example to build and run the example.
- Configure environment variables  
  Configure environment variables according to the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.
- Run the example

  Run the following commands in the example directory.
  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # Build the project, default npu mode
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate test input data
  ./demo                           # Run the compiled executable to execute the example
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin -scenarioNum=$SCENARIO_NUM  # Verify whether the output result is correct
  ```

  > **Note:** Scenario 3 and scenarios 8-59 only support `dav-3510` architecture. Scenarios 1-7 support both `dav-2201` and `dav-3510`.

  To use CPU debug or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.
  
  Examples:

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # CPU debug mode
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU simulation mode
  ```
  > **Note:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and then re-run cmake.

- Build option description

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-59 | Scenario number (scenarios 3 and 8-59 only support dav-3510) |

  The following execution result indicates that the accuracy comparison is successful.

  ```bash
  test pass!
  ```
