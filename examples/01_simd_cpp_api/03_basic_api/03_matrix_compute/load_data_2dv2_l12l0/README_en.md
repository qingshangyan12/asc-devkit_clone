# load_data_2dv2_l12l0 Example

## Overview

This example demonstrates how to call the `LoadData` interface and configure `LoadData2DParamsV2` structure parameters (abbreviated as `Load2Dv2` interface in this example) to transfer A / B matrices from L1 to L0A / L0B.

The example covers three input data types: int8_t, half, and float, as well as A / B matrix transposed and non-transposed input scenarios. L0A data layout is Nz format, L0B data layout is Zn format.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── load_data_2dv2_l12l0
│   ├── figures                          // Illustrations
│   ├── scripts
│   │   ├── gen_data.py                  // Script for generating input data and ground truth data
│   │   └── verify_result.py             // Script for verifying whether output data matches ground truth data
│   ├── CMakeLists.txt                   // Build project file
│   ├── data_utils.h                     // Data read/write functions
│   ├── load_data_2dv2_l12l0.asc         // Ascend C example implementation & invocation example
│   └── README.md                        // Example documentation
```

## Example Description

### 1. Overall Flow

The overall flow of the example is as follows:

```text
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
       │         │                  │          │
    DataCopy  Load2Dv2            Mmad        Fixpipe
```

**Step details**:

1. **GM -> L1**: Use `DataCopy` to implement ND to Nz format conversion.
2. **L1 -> L0A / L0B**: Use `Load2Dv2` interface with `LoadData2DParamsV2` structure parameters, controlling transpose via `ifTranspose`.
3. **Matrix multiplication**: Use `Mmad` interface to perform matrix multiplication.
4. **L0C -> GM**: Use `Fixpipe` interface to transfer results out.

This example uses a fixed matrix multiplication specification of `[m, n, k] = [40, 50, 70]`, with output C matrix `[40, 50]` in ND format. The kernel function selects different scenarios through `scenarioNum`.

### 2. Key Scenario Differences

The input shape, transpose flags, and L1 -> L0 transfer methods for each scenario are shown in [Table 1](#table1).

<a name="table1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 1: Meaning of Different scenarioNum Values</span></caption>
  <tr>
    <td><span style="font-weight: bold;">scenarioNum</span></td>
    <td><span style="font-weight: bold;">Input Data Type</span></td>
    <td><span style="font-weight: bold;">Output Data Type</span></td>
    <td><span style="font-weight: bold;">Input Shape</span></td>
    <td><span style="font-weight: bold;">isAtranspose</span></td>
    <td><span style="font-weight: bold;">isBtranspose</span></td>
    <td><span style="font-weight: bold">Extra Dirty Fractals</span></td>
    <td><span style="font-weight: bold;">L1 -> L0 Transfer Method</span></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">1</span></td>
    <td rowspan="2">int8_t</td>
    <td rowspan="2">int32_t</td>
    <td>A [40, 70]<br>B [50, 70]</td>
    <td>false</td>
    <td>true</td>
    <td>No</td>
    <td><code>Load2Dv2</code></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">2</span></td>
    <td>A [70, 40]<br>B [70, 50]</td>
    <td>true</td>
    <td>false</td>
    <td>Yes</td>
    <td><code>Load2Dv2</code></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">3</span></td>
    <td rowspan="2">half</td>
    <td rowspan="2">float</td>
    <td>A [40, 70]<br>B [50, 70]</td>
    <td>false</td>
    <td>true</td>
    <td>No</td>
    <td><code>Load2Dv2</code></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">4</span></td>
    <td>A [70, 40]<br>B [70, 50]</td>
    <td>true</td>
    <td>false</td>
    <td>No</td>
    <td><code>Load2Dv2</code></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">5</span></td>
    <td rowspan="2">float</td>
    <td rowspan="2">float</td>
    <td>A [40, 70]<br>B [50, 70]</td>
    <td>false</td>
    <td>true</td>
    <td>No</td>
    <td><code>Load2Dv2</code></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">6</span></td>
    <td>A [70, 40]<br>B [70, 50]</td>
    <td>true</td>
    <td>false</td>
    <td>No</td>
    <td><code>Load2Dv2</code></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">7</span></td>
    <td rowspan="1">int8_t</td>
    <td rowspan="1">int32_t</td>
    <td>A [70, 40]<br>B [70, 50]</td>
    <td>true</td>
    <td>false</td>
    <td>No</td>
    <td>for loop + <code>Load2Dv2</code></td>
  </tr>
</table>

Key differences by transfer mode:

**`Load2Dv2` non-transpose transfer: Scenarios 1 / 3 / 5**

- A matrix non-transposed input `[m, k]`, B matrix transposed input `[n, k]`.
- L1 -> L0A / L0B both do not need transpose, `loadDataParams.ifTranspose = false`.

**`Load2Dv2` transpose transfer, single call: Scenarios 2 / 4 / 6**

- A matrix transposed input `[k, m]`, B matrix non-transposed input `[k, n]`.
- L1 -> L0A / L0B both need transpose, `loadDataParams.ifTranspose = true`.
- In scenario 2, int8_t transpose single call causes extra dirty data fractals in m direction; `Fixpipe` transfers out only the valid region via `fixpipeParams.mSize = m`.

**`Load2Dv2` transpose transfer, for loop call: Scenario 7**

- Scenario 7 is also int8_t transposed input, same as scenario 2.
- A matrix uses for loop to call `Load2Dv2`, skipping dirty data fractals at the tail of m direction when writing to L0A, so no extra dirty data fractals participate in matrix computation.

### 3. Parameter Description

The subsequent code and parameter descriptions repeatedly use fractal and alignment related variables. This section defines these concepts first.

- `fractalShape`: Small fractal shape. B8 / B16 / B32 input data types have shape `[16, 32 / sizeof(T)]`, where `T` represents the input data type. Fractal information for data types involved in this example is shown in [Table 2](#table2).
- `fractalSize`: Number of elements in 1 small fractal, see [Table 2](#table2).
- `fractalNum`: When L1 -> L0A / L0B requires transpose, `Load2Dv2` transposes by square matrix. When multiple consecutive small fractals need to merge into a square matrix, `fractalNum` represents the number of small fractals in that square matrix, see [Table 2](#table2).

<a name="table2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 2: Fractal Information for Different Data Types</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">fractalShape</span></td>
    <td align="center"><span style="font-weight: bold;">fractalSize</span></td>
    <td align="center"><span style="font-weight: bold;">fractalNum</span></td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">B8</span></td>
    <td align="center">[16, 32]</td>
    <td align="center">512</td>
    <td align="center">2</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">B16</span></td>
    <td align="center">[16, 16]</td>
    <td align="center">256</td>
    <td align="center">1</td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">B32</span></td>
    <td align="center">[16, 8]</td>
    <td align="center">128</td>
    <td align="center">2</td>
  </tr>
</table>

- `CeilAlign`: Ceiling alignment operation. For example, when `m = 30`, `CeilAlign(30, 16) = 32`.

  ```cpp
  __aicore__ inline uint16_t CeilAlign(uint16_t size, uint16_t alignValue) {
      return (size + alignValue - 1) / alignValue * alignValue;
  }
  ```

- `CeilDivision`: Ceiling division, generally used to compute loop counts after ceiling alignment.
- `mAlignValue`: m axis aligns to `mAlignValue`. Similarly `nAlignValue`, `kaAlignValue`, `kbAlignValue`.
- `mAlignL1` and `mAlignL0`: Aligned values of m axis when A matrix is on L1 and L0A respectively. Similarly `nAlignL1`, `nAlignL0`, `kaAlignL1`, `kaAlignL0`, `kbAlignL1`, `kbAlignL0`.

### 4. Alignment Requirements

A / B matrices have different alignment requirements on each axis on L1 and L0. These values are used when configuring `Load2Dv2`, `Mmad`, and `Fixpipe` parameters. [Table 3](#table3) and [Table 4](#table4) summarize alignment requirements for A / B matrices on L1 and L0 when scenarioNum is 1-6; [Table 5](#table5) and [Table 6](#table6) summarize alignment requirements when scenarioNum is 7.

For the complete alignment tables (Tables 3-6), please refer to the Chinese README.

When scenarioNum=7, A matrix uses for loop + `Load2Dv2` for L1 -> L0A transfer, and L0A only writes the region aligned by valid data fractals.

### 5. `Load2Dv2`

A / B matrices on L1 are all in Nz layout, on L0A and L0B they are in Nz and Zn layouts respectively. The L1 -> L0 phase calls the `LoadData` interface with `LoadData2DParamsV2` structure parameters, abbreviated as `Load2Dv2` in this document.

When reading this section, first determine the scenario from [Table 1](#table1), then select alignment values from [Table 3](#table3)-[Table 6](#table6). Key parameters when configuring `LoadData2DParamsV2`:

- `ifTranspose`: Controls whether to transpose during L1 -> L0 transfer. Set to `false` when A matrix non-transposed input `[m, k]` and B matrix transposed input `[n, k]`; set to `true` when A matrix transposed input `[k, m]` and B matrix non-transposed input `[k, n]`.
- `mStep`, `kStep`: Represent the number of small fractals covered in row and col directions respectively. During transpose transfer, B8 / B32 need to transfer in square fractal combinations, related step sizes must satisfy `fractalNum` requirements.
- `srcStride`, `dstStride`: Represent intervals between adjacent small fractals in row direction of L1 source matrix and L0 destination matrix respectively.
- `mStartPosition`, `kStartPosition`: Represent the starting small fractal position in the L1 source matrix for this transfer. In scenario 7, `mStartPosition` is modified in the loop to transfer A matrix in segments.

For detailed parameter configurations and code examples for each sub-scenario (A matrix L1 -> L0A non-transpose, A matrix L1 -> L0A transpose for B8/B16/B32, B matrix L1 -> L0B non-transpose, B matrix L1 -> L0B transpose for B8/B16/B32), please refer to the Chinese README for the complete code examples and figures.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.
- Configure environment variables
  Configure environment variables according to the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.
- Run the example

  Run the following commands in the example directory.
  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin
  ```
  To use CPU debug or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Examples:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;   # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;   # NPU simulation mode
  ```
  > **Notice:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and then re-run cmake.

- Build option description

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`-`7` | Scenario number: different data types and transpose combinations |

- Execution result

  The following execution result indicates that the accuracy comparison is successful.

  ```bash
  test pass!
  ```
