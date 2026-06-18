# load_data_2dmx_l12l0 Example

## Overview

This example demonstrates how to use the basic API `LoadData` to complete L1 to L0 data transfer in MX quantized matrix multiplication.

The example takes FP4 / FP8 type A / B matrices and fp8_e8m0_t type scaleA / scaleB matrices as input, covering A / B matrix transposed and non-transposed input scenarios. It mainly demonstrates:

1. Using `LoadData2DParamsV2` to control A / B matrix transfer to L0A / L0B;
2. Using `LoadData2DMxParams` to control scaleA / scaleB matrix transfer to L0A_MX / L0B_MX;
3. Combining with `MmadMx` to complete MX matrix multiplication, and transferring results out to GM through `Fixpipe`.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── load_data_2dmx_l12l0
│   ├── scripts
│   │   ├── gen_data.py             // Script for generating input data and ground truth data
│   │   └── verify_result.py        // Script for verifying whether output data matches ground truth data
│   ├── CMakeLists.txt              // Build project file
│   ├── data_utils.h                // Data read/write functions
│   ├── figures                     // Illustrations
│   ├── load_data_2dmx_l12l0.asc    // Ascend C example implementation & invocation example
│   └── README.md                   // Example documentation
```

## Example Description

### 1. Overall Flow

The overall MX matrix multiplication flow is as follows:

```
GM(ND) --DataCopy--> L1(Nz/Zz/Nn) --LoadData(LoadData2DParamsV2, LoadData2DMxParams)--> L0A(Nz)/L0B(Zn)/L0A_MX(Zz)/L0B_MX(Nn) --MmadMx--> L0C(Nz) --Fixpipe--> GM(ND)
```

**Step details**:

1. **GM -> L1**:
   - Call `DataCopy` instruction with `Nd2NzParams` structure parameters to implement A / B matrix format conversion from ND to Nz.
   - Call `DataCopy` instruction with `Nd2NzParams` or `Dn2NzParams` structure parameters, using B16 view to implement scaleA / scaleB matrix format conversion from ND to Zz / Nn.
   - Use `Fill` to zero out invalid data to prevent dirty data from participating in `MmadMx` computation.
2. **L1 -> L0**:
   - Call `LoadData` instruction with `LoadData2DParamsV2` and `LoadData2DMxParams` structure parameters.
   - `LoadData2DParamsV2` controls A / B matrix transfer, `LoadData2DMxParams` controls scaleA / scaleB matrix transfer.
3. **Matrix multiplication**: Use `MmadMx` interface to perform MX matrix multiplication.
4. **L0C -> GM**: Use `Fixpipe` interface to transfer results out.

Note that `LoadData2DMxParams` does not contain transpose control parameters like `ifTranspose`. Scale matrices transfer directly in `x` / `y` direction with stride during the L1 -> L0A_MX / L0B_MX phase. Therefore, scaleA / scaleB need to be pre-arranged in the GM -> L1 phase to the layout expected by L0A_MX / L0B_MX.

A / B matrices and scaleA / scaleB matrices have different data layout formats in different storage units, as shown in [Table 1](#table1):

<a name="table1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 1: Data Layout Formats in Different Storage Units</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">GM</span></td>
    <td align="center"><span style="font-weight: bold;">L1</span></td>
    <td align="center"><span style="font-weight: bold;">L0A</span></td>
    <td align="center"><span style="font-weight: bold;">L0A_MX</span></td>
    <td align="center"><span style="font-weight: bold;">L0B</span></td>
    <td align="center"><span style="font-weight: bold;">L0B_MX</span></td>
    <td align="center"><span style="font-weight: bold;">L0C</span></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">A Matrix</span></td>
    <td align="center">ND</td>
    <td align="center">Nz</td>
    <td align="center">Nz</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">B Matrix</span></td>
    <td align="center">ND</td>
    <td align="center">Nz</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">Zn</td>
    <td align="center">-</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">scaleA Matrix</span></td>
    <td align="center">ND</td>
    <td align="center">Zz</td>
    <td align="center">-</td>
    <td align="center">Zz</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">scaleB Matrix</span></td>
    <td align="center">ND</td>
    <td align="center">Nn</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">Nn</td>
    <td align="center">-</td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">C Matrix</span></td>
    <td align="center">ND</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">-</td>
    <td align="center">Nz</td>
  </tr>
</table>

This example uses a fixed matrix multiplication specification of `[m, n, k] = [40, 50, 70]`, with output C matrix `[40, 50]`, float type, ND format. The kernel function selects different scenarios through `scenarioNum`. See [Table 2](#table2) for specific input shapes and L1 -> L0 transfer methods.

6 scenarios are derived from FP4 / FP8 data types, A / B transpose input, and whether for loop transfer is used when A is transposed. `isAtranspose` simultaneously indicates whether A matrix and scaleA matrix are transposed input; `isBtranspose` simultaneously indicates whether B matrix and scaleB matrix are transposed input. Scenarios 5 / 6 use for loop transfer for A matrix to avoid dirty data exceeding 1 fractal in single `LoadData` transfer when A is transposed.

<a name="table2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 2: Different scenarioNum Values, Input Shapes, and L1 -> L0 Transfer Methods</span></caption>
  <tr>
    <td><span style="font-weight: bold;">scenarioNum</span></td>
    <td><span style="font-weight: bold;">A / B Data Type</span></td>
    <td><span style="font-weight: bold;">A Matrix Type</span></td>
    <td><span style="font-weight: bold;">B Matrix Type</span></td>
    <td><span style="font-weight: bold;">Input Shape</span></td>
    <td><span style="font-weight: bold;">isAtranspose</span></td>
    <td><span style="font-weight: bold;">isBtranspose</span></td>
    <td><span style="font-weight: bold;">A Matrix Transfer</span></td>
    <td><span style="font-weight: bold;">B Matrix Transfer</span></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">1</span></td>
    <td>FP4</td>
    <td>fp4x2_e1m2_t</td>
    <td>fp4x2_e2m1_t</td>
    <td>A [40, 70]<br>B [50, 70]<br>scaleA [40, 4]<br>scaleB [50, 4]</td>
    <td align="center">false</td>
    <td align="center">true</td>
    <td>LoadData2DParamsV2 (no transpose)</td>
    <td>LoadData2DParamsV2 (no transpose)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">2</span></td>
    <td>FP4</td>
    <td>fp4x2_e2m1_t</td>
    <td>fp4x2_e1m2_t</td>
    <td>A [70, 40]<br>B [70, 50]<br>scaleA [4, 40, 2]<br>scaleB [4, 50, 2]</td>
    <td align="center">true</td>
    <td align="center">false</td>
    <td>LoadData2DParamsV2 (transpose)</td>
    <td>LoadData2DParamsV2 (transpose)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">3</span></td>
    <td>FP8</td>
    <td>fp8_e4m3fn_t</td>
    <td>fp8_e5m2_t</td>
    <td>A [40, 70]<br>B [50, 70]<br>scaleA [40, 4]<br>scaleB [50, 4]</td>
    <td align="center">false</td>
    <td align="center">true</td>
    <td>LoadData2DParamsV2 (no transpose)</td>
    <td>LoadData2DParamsV2 (no transpose)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">4</span></td>
    <td>FP8</td>
    <td>fp8_e5m2_t</td>
    <td>fp8_e4m3fn_t</td>
    <td>A [70, 40]<br>B [70, 50]<br>scaleA [4, 40, 2]<br>scaleB [4, 50, 2]</td>
    <td align="center">true</td>
    <td align="center">false</td>
    <td>LoadData2DParamsV2 (transpose)</td>
    <td>LoadData2DParamsV2 (transpose)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">5</span></td>
    <td>FP4</td>
    <td>fp4x2_e2m1_t</td>
    <td>fp4x2_e1m2_t</td>
    <td>A [70, 40]<br>B [70, 50]<br>scaleA [4, 40, 2]<br>scaleB [4, 50, 2]</td>
    <td align="center">true</td>
    <td align="center">false</td>
    <td>for loop + LoadData2DParamsV2 (transpose)</td>
    <td>LoadData2DParamsV2 (transpose)</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">6</span></td>
    <td>FP8</td>
    <td>fp8_e5m2_t</td>
    <td>fp8_e4m3fn_t</td>
    <td>A [70, 40]<br>B [70, 50]<br>scaleA [4, 40, 2]<br>scaleB [4, 50, 2]</td>
    <td align="center">true</td>
    <td align="center">false</td>
    <td>for loop + LoadData2DParamsV2 (transpose)</td>
    <td>LoadData2DParamsV2 (transpose)</td>
  </tr>
</table>

### 2. Scenario Description

The input shapes for each scenario are shown in [Table 2](#table2). This section only describes the differences in L1 -> L0 transfer, zero padding, and subsequent computation parameters between scenarios.

**Scenario 1: FP4 data type input, isAtranspose=false, isBtranspose=true**

- A matrix non-transposed input `[m, k]`, L1 -> L0A does not need transpose, `LoadData2DParamsV2` `ifTranspose = false`.
- B matrix transposed input `[n, k]`, L1 -> L0B does not need transpose, `LoadData2DParamsV2` `ifTranspose = false`.
- scaleA input shape is `[m, scaleK]`, scaleB input shape is `[n, scaleK]`.

<div align="center">
  <img src="figures/whole_process/B4_A_scaleA_.png" width="1000"><br>
  Figure 1: Scenario 1 MX matrix multiplication GM -> L1 -> L0A / L0A_MX flow diagram
</div>

<div align="center">
  <img src="figures/whole_process/B4_B_scaleB_NK.png" width="1000"><br>
  Figure 2: Scenario 1 MX matrix multiplication GM -> L1 -> L0B / L0B_MX flow diagram
</div>

**Scenario 2: FP4 data type input, isAtranspose=true, isBtranspose=false**

- A matrix transposed input `[k, m]`, L1 -> L0A needs transpose, `LoadData2DParamsV2` `ifTranspose = true`.
- B matrix non-transposed input `[k, n]`, L1 -> L0B needs transpose, `LoadData2DParamsV2` `ifTranspose = true`.
- scaleA input shape is `[scaleK/2, m, 2]`, scaleB input shape is `[scaleK/2, n, 2]`.
- A matrix m direction transfers extra dirty data exceeding 1 fractal, `MmadMx` needs to set `mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum)`, `Fixpipe` skips results from invalid fractals during transfer out.

**Scenario 3: FP8 data type input, isAtranspose=false, isBtranspose=true**

Similar to scenario 1, but data type is FP8.

- A and B matrix L1 -> L0 transfers both do not need transpose, `LoadData2DParamsV2` `ifTranspose = false`.
- scaleA input shape is `[m, scaleK]`, scaleB input shape is `[n, scaleK]`.
- In FP8 data type, when A matrix non-transposed input `[m, k]` and B matrix transposed input `[n, k]`, k is in col direction. `DataCopy` only aligns k direction to 32B. Need to flush k direction tail data to 0 on L1 to prevent dirty data from participating in computation.

**Scenario 4: FP8 data type input, isAtranspose=true, isBtranspose=false**

Similar to scenario 2, but data type is FP8.

- A matrix transposed input `[k, m]`, B matrix non-transposed input `[k, n]`, L1 -> L0 transfers both need transpose, `LoadData2DParamsV2` `ifTranspose = true`.
- scaleA input shape is `[scaleK/2, m, 2]`, scaleB input shape is `[scaleK/2, n, 2]`.
- A matrix m direction transfers extra dirty data exceeding 1 fractal, `MmadMx` needs to set `mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum)`.
- When A matrix transposed input `[k, m]` and B matrix non-transposed input `[k, n]`, both need to flush remaining dirty data in k direction to 0 on L1.

**Scenario 5: FP4 data type input, isAtranspose=true, isBtranspose=false**

- A matrix transposed input `[k, m]`, L1 -> L0A needs transpose, uses for loop to call `LoadData` for transfer.
- For loop skips dirty data fractals at the tail of m direction on L0A each time, keeping extra transferred data in m direction within 1 fractal.
- B matrix non-transposed input `[k, n]`, L1 -> L0B needs transpose, single call `LoadData` for transfer, `LoadData2DParamsV2` `ifTranspose = true`.
- scaleA input shape is `[scaleK/2, m, 2]`, scaleB input shape is `[scaleK/2, n, 2]`.
- Both A and B matrices need to flush remaining dirty data in k direction to 0 on L1.

**Scenario 6: FP8 data type input, isAtranspose=true, isBtranspose=false**

Similar to scenario 5, but data type is FP8.

- A matrix transposed input `[k, m]`, L1 -> L0A needs transpose, uses for loop to call `LoadData` for transfer, avoiding dirty data exceeding 1 fractal in m direction.
- B matrix non-transposed input `[k, n]`, L1 -> L0B needs transpose, single call `LoadData` for transfer, `LoadData2DParamsV2` `ifTranspose = true`.
- scaleA input shape is `[scaleK/2, m, 2]`, scaleB input shape is `[scaleK/2, n, 2]`.
- Both A and B matrices need to flush remaining dirty data in k direction to 0 on L1.

### 3. Parameter Description

The subsequent code and parameter descriptions repeatedly use fractal, alignment, and scale related variables. This section defines these concepts first.

- `fractalShape`: Small fractal shape. This example involves FP4 and FP8 for A / B matrix input, scale matrices transferred using B16 view. Fractal information is shown in [Table 3](#table3).
- `fractalSize`: Number of elements in 1 small fractal, see [Table 3](#table3).
- `fractalNum`: When L1 -> L0A / L0B requires transpose, `LoadData` transposes by square matrix. `fractalNum` represents the number of small fractals in the square matrix, see [Table 3](#table3).
- `packedK`: Equivalent column count in `DataCopy` parameters when transferring A / B matrix k direction using B8 view. FP4 input still has k elements, but during ND -> Nz transfer it is processed as B8 view, every 2 FP4 elements correspond to 1 B8 view element, so `packedK = CeilDivision(k, 2)`; FP8 input uses original element count, `packedK = k`.
- `scaleK`: Aligned length of scale matrix k axis. `scaleK = CeilDivision(k, SCALE_BASE_FACTOR) * SCALE_EVEN_NUMBER`, where `SCALE_BASE_FACTOR = 64`, `SCALE_EVEN_NUMBER = 2`. In this example when `k = 70`, `scaleK = CeilDivision(70, 64) * 2 = 4`.
- `alignK`: Aligned length of A / B matrix k axis. The `MmadMx` interface in MX matrix multiplication requires k direction alignment to `SCALE_BASE_FACTOR = 64`, so `alignK = CeilAlign(k, SCALE_BASE_FACTOR) = CeilAlign(k, 64)`. In this example when `k = 70`, `alignK = CeilAlign(70, 64) = 128`. This 64-alignment requirement only applies to k direction; m / n directions still align to the corresponding data type's small fractal or square transpose granularity.

<a name="table3"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">Table 3: Fractal Information for Different Data Types</span></caption>
  <tr>
    <td></td>
    <td align="center"><code>Usage in This Example</code></td>
    <td align="center"><code>fractalShape</code></td>
    <td align="center"><code>fractalSize</code></td>
    <td align="center"><code>fractalNum</code></td>
    <td align="center"><code>packedK</code></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">FP4</span></td>
    <td align="center">A / B matrix FP4 input</td>
    <td align="center">[16, 64]</td>
    <td align="center">1024</td>
    <td align="center">4</td>
    <td align="center"><code>CeilDivision(k, 2)</code></td>
  </tr>
    <tr>
    <td align="center"><span style="font-weight: bold;">FP8</span></td>
    <td align="center">A / B matrix FP8 input</td>
    <td align="center">[16, 32]</td>
    <td align="center">512</td>
    <td align="center">2</td>
    <td align="center"><code>k</code></td>
  </tr>
  <tr>
    <td align="center"><span style="font-weight: bold;">B16</span></td>
    <td align="center">scale matrix transferred using B16 view</td>
    <td align="center">[16, 16]</td>
    <td align="center">256</td>
    <td align="center">1</td>
    <td align="center">-</td>
  </tr>
</table>

- `CeilAlign`: Ceiling alignment operation. For example, when `m = 30`, `CeilAlign(30, 16) = 32`.

```cpp
__aicore__ inline uint16_t CeilAlign(uint16_t size, uint16_t alignValue) {
    return (size + alignValue - 1) / alignValue * alignValue;
}
```

- `CeilDivision`: Ceiling division, generally used to compute loop counts after ceiling alignment.
- `mAlignValue`: m axis aligns to `mAlignValue`. Similarly `nAlignValue`, `kAlignValue`.
- `mAlignL1` and `mAlignL0`: Aligned values of m axis when A matrix is on L1 and L0A respectively. Similarly `kaAlignL1`, `kaAlignL0`, `nAlignL1`, `nAlignL0`, `kbAlignL1`, `kbAlignL0`.


### 4. Alignment Requirements

A / B / scaleA / scaleB matrices have different alignment requirements on each axis on L1 and L0. These values are used when configuring `DataCopy`, `LoadData`, `MmadMx`, and `Fixpipe` parameters. A / B matrix k axis in MX scenarios all align to `SCALE_BASE_FACTOR = 64`, that is `kAlignValue = 64`; [Table 4](#table4) and [Table 5](#table5) only list m axis / n axis alignment requirements that vary with input layout and transfer method.

- [Table 4](#table4) is used to determine the shape of A / B matrices on L1 after GM -> L1, and the subsequent `Fill` zero-padding range.
- [Table 5](#table5) is used to determine L0A / L0B alignment values after `LoadData` transfer in the L1 -> L0 phase, affecting subsequent `MmadMx` and `Fixpipe` parameter configuration.
- [Table 6](#table6) is used to determine alignment shapes of scaleA / scaleB matrices on L1, L0A_MX, and L0B_MX.

For the complete alignment tables (Tables 4-6), please refer to the Chinese README.

### 5. GM to L1 (`DataCopy` and `Fill`)

This subsection describes `DataCopy` parameter configuration for the GM -> L1 phase for A matrix, B matrix, scaleA matrix, and scaleB matrix separately, as well as zero-padding operations needed on L1. Two types of constraints require focus:

1. scaleA / scaleB do not support transpose in the L1 -> L0A_MX / L0B_MX phase, so they need to be pre-converted to the layout format required by L0 in the GM -> L1 phase.
2. The `MmadMx` interface in MX matrix multiplication requires **k direction alignment to 64**, while `DataCopy` alignment behavior in the GM -> L1 phase depends on which axis k is on. Remaining k direction data needs to be flushed to 0 per scenario to prevent dirty data from participating in computation:
   - When k is in col direction, in FP8 data type `DataCopy` only aligns k direction to 32B (32 elements), need to continue padding to 64 element alignment.
   - When k is in row direction, `DataCopy` aligns k direction to 16, need to continue padding to 64 alignment.

For detailed parameter configurations and code examples for A matrix GM -> L1 (both [m, k] and [k, m] inputs), B matrix GM -> L1 (both [k, n] and [n, k] inputs), scaleA matrix GM -> L1 (both [m, scaleK] and [scaleK, m, 2] inputs), and scaleB matrix GM -> L1 (both [scaleK, n, 2] and [n, scaleK] inputs), please refer to the Chinese README for the complete code examples and figures.

### 6. L1 to L0 (`LoadData`)

This subsection describes how to call the `LoadData` instruction and complete data transfer and format transformation through `LoadData2DParamsV2` and `LoadData2DMxParams` structure parameters when transferring A / B matrices from L1 to L0A / L0B and scaleA / scaleB matrices from L1 to L0A_MX / L0B_MX.

#### 6.1. `LoadData2DParamsV2` Structure Parameter Description

`LoadData2DParamsV2` structure parameters control A / B matrix data transfer from L1 to L0A / L0B (transpose can occur during this process), including:

- **sid**: Source matrix identifier, default 0
- **mStartPosition**: Source matrix row direction start position, unit is 16 elements
- **kStartPosition**: Source matrix col direction start position, unit is 32B
- **mStep**: Source matrix row direction transfer length, unit is 16 elements
- **kStep**: Source matrix col direction transfer length, unit is 32B
- **srcStride**: Source matrix col direction adjacent fractal start address interval, unit is 512B
- **dstStride**: Destination matrix col direction adjacent fractal start address interval, unit is 512B
- **ifTranspose**: Whether to enable transpose function, transposing each fractal matrix, default false. When set to true, completes small fractal transpose and synchronously adjusts large fractal layout.

Note: A / B matrix data fractal size is 512B

#### 6.2. `LoadData2DMxParams` Structure Parameter Description

`LoadData2DMxParams` structure parameters control scale matrix data transfer from L1 to L0A_MX / L0B_MX (pure transfer, no layout format change), including:

- **xStartPosition**: Source matrix row direction start position, unit is 1 32B fractal
- **yStartPosition**: Source matrix col direction start position, unit is 32B
- **xStep**: Source matrix row direction transfer length, unit is 1 32B fractal
- **yStep**: Source matrix col direction transfer length, unit is 32B
- **srcStride**: Source matrix row direction adjacent fractal (16*2) start address interval, unit is 32B
- **dstStride**: Destination matrix row direction adjacent fractal (16*2) start address interval, unit is 32B

Note: scale matrix data fractal size is 16\*2\*1=32B

Calling `LoadData` once with both `LoadData2DParamsV2` and `LoadData2DMxParams` structure parameters simultaneously completes A matrix transfer to L0A and corresponding scale matrix transfer to L0A_MX. L0A_MX Buffer and L0A addresses have a fixed ratio relationship; the `LoadData` instruction derives L0A_MX address from L0A address automatically, no user configuration needed. B matrix and scaleB work similarly:

```cpp
AscendC::LoadData(a2Local, a1Local, scaleA1Local, loadDataParams, loadMxDataParams);
AscendC::LoadData(b2Local, b1Local, scaleB1Local, loadDataParams, loadMxDataParams);
```

For detailed parameter configurations and code examples for A matrix L1 -> L0A (non-transpose, single call transpose, for loop transpose) and scaleA matrix L1 -> L0A_MX, please refer to the Chinese README for the complete code examples and figures.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.
- Configure environment variables
  Configure environment variables according to the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.
- Install Python dependencies

  The `gen_data.py` script in this example requires `ml_dtypes` to generate FP8 input data and `en_dtypes` to generate FP4 input data. The following versions are recommended:

  ```bash
  python3 -m pip install ml_dtypes==0.2.0 en_dtypes==0.0.4
  ```

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
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | NPU architecture: Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`-`6` | Scenario number |

- Execution result

  The following execution result indicates that the accuracy comparison is successful.

  ```bash
  test pass!
  ```
