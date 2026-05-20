# load_data_2dv2_l12l0 Sample

## Overview

This sample demonstrates how to use the `LoadData` interface with the LoadData2DParamsV2 structure (referred to as `Load2Dv2` in this sample) to move matrix data from L1 to L0A/L0B. The input uses ND format with B8/B16/B32 data types (using int8_t/half/float as examples). The sample covers 7 matrix multiplication scenarios with different combinations of left/right matrix transpose operations.

- Uses `LoadData2DParamsV2` parameter structure
- L0A data layout is **Nz format**
- L0B data layout is **Zn format**
- Supports transpose functionality controlled by the `ifTranspose` parameter
- Supports int8_t, half, and float data types

## Supported Products
- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── load_data_2dv2_l12l0
│   ├── figures                     // Illustrations
│   ├── scripts
│   │   ├── gen_data.py             // Input and golden data generation script
│   │   └── verify_result.py        // Verification script for output and golden data
│   ├── CMakeLists.txt              // Build project file
│   ├── data_utils.h                // Data read/write functions
│   └── load_data_2dv2_l12l0.asc    // Ascend C sample implementation & invocation
```

## Sample Description

### Sample Functionality

The overall workflow follows this pattern:

```
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
        │         │                  │          │       
     DataCopy  Load2Dv2            Mmad        Fixpipe 
```

**Step Details**:

1. **GM → L1**: Use `DataCopy` for ND to Nz format conversion
2. **L1 → L0A/L0B**: Use `Load2Dv2` interface for data movement, control transpose via `ifTranspose`
3. **Matrix Multiplication**: Use `Mmad` interface to execute matrix multiplication
4. **L0C → GM**: Use `Fixpipe` interface to move out results

All scenarios use the same matrix multiplication dimensions: [m, n, k] = [40, 50, 70]. The kernel function name is "KernelLoadDataL12L0Load2Dv2". The parameter scenarioNum represents the 7 scenarios described in [Table 1](#table-1):

<a name="table-1"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 1: scenarioNum Values and Their Meanings</span>
  <tr>
    <td ><span style="font-weight: bold;">scenarioNum</span></td>
    <td><span style="font-weight: bold;">Input Data Type</span></td>
    <td><span style="font-weight: bold;">Output Data Type</span></td>
    <td><span style="font-weight: bold;">isAtranspose</span></td>
    <td><span style="font-weight: bold;">isBtranspose</span></td>
    <td><span style="font-weight: bold;">Extra Data Movement</span></td>
    <td><span style="font-weight: bold;">L1->L0 Interface</span></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">1</span></td>
    <td rowspan="2" >int8_t</td>
    <td rowspan="2" >int32_t</td>
    <td>false</td>
    <td>true</td>
    <td>No</td>
    <td>Load2Dv2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">2</span></td>
    <td>true</td>
    <td>false</td>
    <td>Yes</td>
    <td>Load2Dv2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">3</span></td>
    <td rowspan="2" >half</td>
    <td rowspan="2" >float</td>
    <td>false</td>
    <td>true</td>
    <td>No</td>
    <td>Load2Dv2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">4</span></td>
    <td>true</td>
    <td>false</td>
    <td>No</td>
    <td>Load2Dv2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">5</span></td>
    <td rowspan="2" >float</td>
    <td rowspan="2" >float</td>
    <td>false</td>
    <td>true</td>
    <td>No</td>
    <td>Load2Dv2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">6</span></td>
    <td>true</td>
    <td>false</td>
    <td>No</td>
    <td>Load2Dv2</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">7</span></td>
    <td rowspan="1" >int8_t</td>
    <td rowspan="1" >int32_t</td>
    <td>true</td>
    <td>false</td>
    <td>No</td>
    <td>for loop + Load2Dv2</td>
  </tr>
</table>

Note: When scenarioNum equals 7, L1->L0A data movement uses a for loop with Load2Dv2. All other scenarios invoke Load2Dv2 once.

**Scenario 1: int8_t input, isAtranspose=False, isBtranspose=True**
- Input A [40, 70], int8_t type, ND format; B [50, 70], int8_t type, ND format
- Output C [40, 50], int32_t type, ND format
- Implementation: Use one Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: L1->L0A requires no transpose. Configure mStep, kStep, srcStride parameters to complete A matrix L1->L0A movement. Similarly, L1->L0B requires no transpose. B matrix completes L1->L0B movement and large fractal layout transformation by configuring mStep, kStep, srcStride parameters.

**Scenario 2: int8_t input, isAtranspose=True, isBtranspose=False**
- Input A [70, 40], int8_t type, ND format; B [70, 50], int8_t type, ND format
- Output C [40, 50], int32_t type, ND format
- Implementation: Use one Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: A matrix invokes one Load2DV2 instruction. Configure mStep, kStep, srcStride with ifTranspose=true to complete A matrix L1->L0A movement with transpose during the process. B matrix invokes one Load2DV2 instruction. Configure mStep, kStep, srcStride with ifTranspose=true to complete B matrix L1->L0B movement with transpose. This scenario has extra dirty data fractals in m direction. Set fixpipeParams.mSize = m during Fixpipe to prevent invalid data from being moved out.

**Scenario 3: half input, isAtranspose=False, isBtranspose=True**
- Input A [40, 70], half type, ND format; B [50, 70], half type, ND format
- Output C [40, 50], float type, ND format
- Implementation: Use one Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: L1->L0A requires no transpose. Configure mStep, kStep, srcStride to complete A matrix L1->L0A movement. Similarly, L1->L0B requires no transpose. B matrix completes L1->L0B movement and large fractal layout transformation.

**Scenario 4: half input, isAtranspose=True, isBtranspose=False**
- Input A [70, 40], half type, ND format; B [70, 50], half type, ND format
- Output C [40, 50], float type, ND format
- Implementation: Use one Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: A matrix invokes one Load2DV2 instruction. Configure mStep, kStep, srcStride with ifTranspose=true to complete A matrix L1->L0A movement with transpose. B matrix invokes one Load2DV2 instruction with ifTranspose=true to complete B matrix L1->L0B movement with transpose.

**Scenario 5: float input, isAtranspose=False, isBtranspose=True**
- Input A [40, 70], float type, ND format; B [50, 70], float type, ND format
- Output C [40, 50], float type, ND format
- Implementation: Use one Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: L1->L0A requires no transpose. Configure mStep, kStep, srcStride to complete A matrix L1->L0A movement. Similarly, L1->L0B requires no transpose. B matrix completes L1->L0B movement and large fractal layout transformation.

**Scenario 6: float input, isAtranspose=True, isBtranspose=False**
- Input A [70, 40], float type, ND format; B [70, 50], float type, ND format
- Output C [40, 50], float type, ND format
- Implementation: Use one Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: A matrix invokes one Load2DV2 instruction. Configure mStep, kStep, srcStride with ifTranspose=true to complete A matrix L1->L0A movement with transpose. B matrix invokes one Load2DV2 instruction with ifTranspose=true to complete B matrix L1->L0B movement with transpose.

**Scenario 7: int8_t input, isAtranspose=True, isBtranspose=False**
- Input A [70, 40], int8_t type, ND format; B [70, 50], int8_t type, ND format
- Output C [40, 50], int32_t type, ND format
- Implementation: Use for loop + Load2DV2 to move A matrix from L1 to L0A, and one Load2DV2 to move B matrix from L1 to L0B
- Explanation: A matrix uses for loop along k direction, invoking Load2DV2 multiple times. Each iteration moves 2 fractals along k axis and CeilDivision(k, fractalShape[1]) fractals along m axis from L1. Configure mStep, kStep, srcStride with ifTranspose=true to complete A matrix L1->L0A movement with transpose. B matrix invokes one Load2DV2 instruction with ifTranspose=true to complete B matrix L1->L0B movement with transpose. The for loop with dstStride parameter skips dirty data fractals in m direction when writing to L0A.

For clarity, the following concepts are defined:

(1) fractalShape: Small fractal shape is [16, 32 / sizeof(T)], where T represents the input data type. Fractal information for data types in this sample is shown in [Table 2](#table-2).

(2) fractalSize: Number of elements in one small fractal. See [Table 2](#table-2).

(3) fractalNum: When L1->L0A/L0B requires transpose, Load2DV2 internally transposes small block matrices. For B8 and B32 data types, fractalShape is [16,32] and [16,8] respectively. Two consecutive small fractals merge into one block for transpose. This parameter indicates how many small fractals one block contains. See [Table 2](#table-2).

<a name="table-2"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 2: Fractal Information for Different Data Types</span></caption>
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

(4) CeilAlign: Ceiling alignment operation. For example, when m=30, CeilAlign(30, 16)=32, meaning align m axis to 16, resulting in m axis length of 32.

      __aicore__ inline uint16_t CeilAlign(uint16_t size, uint16_t alignValue) {
          return (size + alignValue - 1) / alignValue * alignValue;
      }

(5) CeilDivision: Ceiling division, typically used to calculate loop counts after alignment.

(6) mAlignValue: m axis aligned to mAlignValue. For example, mAlignValue=32 means m axis aligned to 32. Similarly, nAlignValue means n axis aligned to nAlignValue, kaAlignValue means A matrix k axis aligned to kaAlignValue, and kbAlignValue means B matrix k axis aligned to kbAlignValue.

(7) mAlignL0 and mAlignL1: The aligned m axis values for A matrix in L1 and L0A respectively. Similarly for nAlignL0, nAlignL1, kaAlignL0, kaAlignL1, kbAlignL0, kbAlignL1.

Additionally, A and B matrices have different alignment requirements in L1 and L0 along row and col directions. Scenarios 1-6 in [Table 1](#table-1) (L1 layout is Nz) alignment requirements are summarized in [Table 3](#table-3) and [Table 4](#table-4):

<a name="table-3"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 3: A and B Matrix Alignment Requirements in L1 (L1 Layout is Nz)</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">B8 (fractalNum=2)</span></td>
    <td align="center"><span style="font-weight: bold;">B16 (fractalNum=1)</span></td>
    <td align="center"><span style="font-weight: bold;">B32 (fractalNum=2)</span></td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A matrix non-transpose input [m, k]</span></td>
    <td colspan="3" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kaAlignValue = fractalShape[1]</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A matrix transpose input [k, m]</span></td>
    <td colspan="2" align="center">kaAlignValue = fractalShape[0] * fractalNum</td>
    <td colspan="1" align="center">kaAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >mAlignValue = fractalShape[1]</td>
    <td colspan="1" align="center" >mAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
    <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B matrix non-transpose input [k, n]</span></td>
    <td colspan="2" align="center">kbAlignValue = fractalShape[0] * fractalNum</td>
    <td colspan="1" align="center">kbAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >nAlignValue = fractalShape[1]</td>
    <td colspan="1" align="center" >nAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
 <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B matrix transpose input [n, k]</span></td>
    <td colspan="3" align="center">nAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kbAlignValue = fractalShape[1]</td>
  </tr>
</table>

<a name="table-4"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 4: A and B Matrix Alignment Requirements in L0</span></caption>
  <tr>
    <td></td>
    <td align="center"><span style="font-weight: bold;">B8 (fractalNum=2)</span></td>
    <td align="center"><span style="font-weight: bold;">B16 (fractalNum=1)</span></td>
    <td align="center"><span style="font-weight: bold;">B32 (fractalNum=2)</span></td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A matrix non-transpose input [m, k], L1->L0A no transpose</span></td>
    <td colspan="3" align="center">mAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kaAlignValue = fractalShape[1]</td>
  </tr>
  <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">A matrix transpose input [k, m], L1->L0A requires transpose</span></td>
    <td colspan="2" align="center">kaAlignValue = fractalShape[1]</td>
    <td >kaAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
  <tr>
    <td colspan="2" align="center" >mAlignValue = fractalShape[0] * fractalNum</td>
    <td align="center" >mAlignValue = fractalShape[0]</td>
  </tr>
    <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B matrix non-transpose input [k, n], L1->L0B requires transpose</span></td>
    <td colspan="2" align="center">kbAlignValue = fractalShape[1]</td>
      <td align="center">kbAlignValue = fractalShape[1] * fractalNum</td>
  </tr>
  <tr>
    <td colspan="2" align="center">nAlignValue = fractalShape[0] * fractalNum</td>
    <td align="center" >nAlignValue = fractalShape[0]</td>
  </tr>
 <tr>
    <td rowspan="2" align="center"><span style="font-weight: bold;">B matrix transpose input [n, k], L1->L0B no transpose</span></td>
    <td colspan="3" align="center">nAlignValue = fractalShape[0]</td>
  </tr>
  <tr>
    <td colspan="3" align="center" >kbAlignValue = fractalShape[1]</td>
  </tr>
</table>

Specifically, when scenarioNum=7, A matrix uses for loop + Load2DV2 for L1->L0A movement. L0A only writes data aligned to valid data fractals. A and B matrix alignment requirements in L1 and L0 are shown in [Table 5](#table-5) and [Table 6](#table-6):

<a name="table-5"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 5: scenarioNum=7, A and B Matrix Alignment Requirements in L1</span></caption>
  <tr>
    <td align="center" ></td>
    <td align="center" ><span style="font-weight: bold;">int8_t (fractalNum=2)</span></td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">A matrix transpose input [k, m]</span></td>
    <td align="center" >kaAlignValue = fractalShape[0] * fractalNum</td>
  </tr>
    <tr>
    <td align="center" >mAlignValue = fractalShape[1]</td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">B matrix non-transpose input [k, n]</span></td>
    <td align="center" >kbAlignValue = fractalShape[0] * fractalNum</td>
  </tr>
    <tr>
    <td align="center" >nAlignValue = fractalShape[1]</td>
  </tr>
</table>

<a name="table-6"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 Table 6: scenarioNum=7, A and B Matrix Alignment Requirements in L0</span></caption>
  <tr>
    <td align="center" ></td>
    <td align="center" ><span style="font-weight: bold;">int8_t (fractalNum=2)</span></td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">A matrix transpose input [k, m], L1->L0A requires transpose</span></td>
    <td align="center" >mAlignValue = fractalShape[0]</td>
  </tr>
    <tr>
    <td align="center" >kaAlignValue = fractalShape[1]</td>
  </tr>
   <tr>
    <td rowspan="2"><span style="font-weight: bold;">B matrix non-transpose input [k, n], L1->L0B requires transpose</span></td>
    <td align="center" >kbAlignValue = fractalShape[1]</td>
  </tr>
    <tr>
    <td align="center" >nAlignValue = fractalShape[0] * fractalNum</td>
  </tr>
</table>

### Sample Implementation

A/B matrices have Nz data layout in L1, and Nz/Zn layouts in L0A/L0B respectively. The L1->L0 process invokes Load2DV2 interface to complete data movement and format transformation.

#### A Matrix L1->L0A Non-Transpose

L1->L0A non-transpose data movement has no large fractal and small fractal layout transformation. For B8/B16/B32 data types, Load2DV2 interface parameter configurations are basically the same, with only fractalShape differences. See [Table 2](#table-2). Using int8_t as an example for illustration:

<p align="center">
  <img src="figures/B8_A_l1_l0A_Load2dv2.png">
</p>

<p align="center">
Figure 1: int8_t data type, L1->L0A non-transpose, Load2DV2 data layout diagram
</p>

As shown in Figure 1, start moving from A matrix starting address in L1. m is row, configure mstep parameter; k is col, configure kStep parameter. Invoke one instruction to complete A matrix L1->L0A movement.

```cpp
mAlignL1 = CeilAlign(m, fractalShape[0]);
kaAlignL1 = CeilAlign(k, fractalShape[1]);
mAlignL0 = CeilAlign(m, fractalShape[0]);
kaAlignL0 = CeilAlign(k, fractalShape[1]);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStartPosition = 0;
loadDataParams.kStartPosition = 0;
loadDataParams.mStep = CeilDivision(mAlignL1, fractalShape[0]); // row
loadDataParams.kStep = CeilDivision(kaAlignL1, fractalShape[1]); // col
loadDataParams.srcStride = CeilDivision(mAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = false;
loadDataParams.sid = 0;
AscendC::LoadData(a2Local, a1Local, loadDataParams);
```

#### A Matrix L1->L0A Transpose

L1->L0A requires large fractal layout transformation and small fractal transpose. Using int8_t, half, float as examples, the following sections describe B8/B16/B32 transpose scenarios separately.

##### B8 Input Data Type

B8 input data type fractal is 16*32. L1->L0 transpose combines 2 fractals of 16*32 in row direction into one 32*32 block for transpose. Using int8_t as example, m is 40. If one Load2DV2 instruction completes L1->L0A movement with transpose, L0A writes one extra invalid fractal in m direction. Mmad calculation also computes one extra invalid fractal in m direction. Fixpipe only moves out valid data. If using for loop + Load2DV2, you can skip invalid fractals in m direction when writing to L0A. Both scenarios are described below.

**Invoke One Load2DV2**

<p align="center">
  <img src="figures/B8_A_l1_l0A_trans_load2dv2.png">
</p>

<p align="center">
Figure 2: int8_t data type, L1->L0A transpose, one Load2DV2 data layout diagram
</p>

In this sample, m=40. As shown in Figure 2, in L1, mAlignL1 = CeilAlign(m, fractalShape[1])=64. mAlignL1 - m = 24 > 16. When one Load2DV2 completes L1->L0A movement, m direction moves one invalid fractal, shown in red box. Mmad calculation also computes one extra invalid fractal in m direction. That is mmadParams.m = CeilAlign(m, fractalShape[0] * fractalNum). Fixpipe only moves out valid data (fixpipeParams.mSize = m).

```cpp
kaAlignL1 = CeilAlign(k, fractalShape[0] * fractalNum);
mAlignL1 = CeilAlign(m, fractalShape[1]);
mAlignL0 = CeilAlign(m, fractalShape[0] * fractalNum);
kaAlignL0 = CeilAlign(k, fractalShape[1]);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStep = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(mAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
loadDataParams.sid = 0;
AscendC::LoadData(a2Local, a1Local, loadDataParams);
```

**for loop + Load2Dv2**

<p align="center">
  <img src="figures/B8_A_l1_l0A_trans_for_load2dv2.png">
</p>

<p align="center">
Figure 3: int8_t data type, L1->L0A transpose, for loop with multiple Load2DV2 data layout diagram
</p>

In this sample, m=40. As shown in Figure 3, in L1, mAlignL1 = CeilAlign(m, fractalShape[1])=64. mAlignL1 - m = 24 > 16. Use for loop along k direction, invoking Load2DV2 multiple times. Each iteration moves 2 fractals along k axis and CeilDivision(k, fractalShape[1]) fractals along m axis from L1, shown in red box. Configure dstStride as m direction valid data aligned to small fractal fractalShape[0]. When writing to L0A, skip dirty data fractals in m direction read during transpose.

```cpp
kaAlignL1 = CeilAlign(k, fractalShape[0] * fractalNum);
mAlignL1 = CeilAlign(m, fractalShape[1]);
mAlignL0 = CeilAlign(m, fractalShape[0]);
kaAlignL0 = CeilAlign(k, fractalShape[1]);
// int8 input, A matrix [k,m] transpose input, L1->L0A requires transpose
// for loop invokes Load2DV2, loop along k axis. Each iteration moves 2 fractals in k direction from L1. Skip m direction tail dirty data fractals when writing to L0A. m direction extra data does not exceed 1 fractal.
uint16_t L0ALoopNum = CeilDivision(kaAlignL0, fractalShape[0] * fractalNum);
loadDataParams.mStep = INT8_M_STEP_ALIGN;
loadDataParams.kStep = CeilDivision(mAlignL0, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
uint32_t dstOffset = 0;
for (uint16_t loopIdx = 0; loopIdx < L0ALoopNum; ++loopIdx) {
    loadDataParams.mStartPosition = INT8_M_STEP_ALIGN * loopIdx;
    AscendC::LoadData(a2Local[dstOffset], a1Local, loadDataParams);
    dstOffset += CeilAlign(mAlignL0, fractalShape[0]) * fractalShape[1];
}
```

##### B16 Input Data Type

B16 input data type fractal is 16*16. One fractal is one block. L1->L0 transpose transposes by small fractal. One Load2DV2 completes L1->L0A data movement and transpose. Using half as example:

<p align="center">
  <img src="figures/B16_A_l1_l0A_trans_load2dv2.png">
</p>

<p align="center">
Figure 4: half data type, L1->L0A transpose, one Load2DV2 data layout diagram
</p>

As shown in Figure 4, start moving from A matrix starting address in L1. k is row, configure mstep parameter; m is col, configure kStep parameter. Set ifTranspose=true. Invoke one instruction to complete A matrix L1->L0A movement with transpose.

```cpp
kaAlignL1 = CeilAlign(k, fractalShape[0] * fractalNum);
mAlignL1 = CeilAlign(m, fractalShape[1]);
mAlignL0 = CeilAlign(m, fractalShape[0] * fractalNum);
kaAlignL0 = CeilAlign(k, fractalShape[1]);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStep = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(mAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
loadDataParams.sid = 0;
AscendC::LoadData(a2Local, a1Local, loadDataParams);
```

##### B32 Input Data Type

B32 input data type fractal is 16*8. L1->L0 transpose combines 2 fractals of 16*8 in col direction into one 16*16 block for transpose. Using float as example:

<p align="center">
  <img src="figures/B32_A_l1_l0A_trans_load2dv2.png">
</p>

<p align="center">
Figure 5: float data type, L1->L0A transpose, one Load2DV2 data layout diagram
</p>

In this sample, m is 40. Since transpose combines 2 fractals in col direction into a block (kStep must be multiple of 2), L1 col direction (m direction) has one extra invalid fractal to satisfy instruction requirement. One Load2DV2 instruction completes L1->L0A movement with transpose. L0A writes one extra invalid fractal in k direction. Since L0A layout is Nz, extra invalid fractal data is at tail. Configure mmadParams.k = k during Mmad calculation to only let valid data participate.

```cpp
kaAlignL1 = CeilAlign(k, fractalShape[0]);
mAlignL1 = CeilAlign(m, fractalShape[1] * fractalNum);
mAlignL0 = CeilAlign(m, fractalShape[0]);
kaAlignL0 = CeilAlign(k, fractalShape[1] * fractalNum);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStep = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(mAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kaAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
loadDataParams.sid = 0;
AscendC::LoadData(a2Local, a1Local, loadDataParams);
```

#### B Matrix L1->L0B Non-Transpose

L1->L0B non-transpose data movement only has large fractal layout transformation. For B8/B16/B32 data types, Load2DV2 interface parameter configurations are basically the same, with only fractalShape differences. See [Table 2](#table-2). Using float as example:

<p align="center">
  <img src="figures/B32_B_l1_l0B_load2dv2.png">
</p>

<p align="center">
Figure 6: float data type, L1->L0B non-transpose, one Load2DV2 data layout diagram
</p>

As shown in Figure 6, start moving from B matrix starting address in L1. n is row, configure mstep parameter; k is col, configure kStep parameter. Invoke one instruction to complete B matrix L1->L0B movement.

```cpp
nAlignL1 = CeilAlign(n, fractalShape[0]);
kbAlignL1 = CeilAlign(k, fractalShape[1]);
kbAlignL0 = CeilAlign(k, fractalShape[1]);
nAlignL0 = CeilAlign(n, fractalShape[0]);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStartPosition = 0;
loadDataParams.kStartPosition = 0;
loadDataParams.mStep = CeilDivision(nAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(kbAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(nAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(nAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = false;
loadDataParams.sid = 0;
AscendC::LoadData(b2Local, b1Local, loadDataParams);
```

#### B Matrix L1->L0B Transpose

L1->L0B requires large fractal layout transformation and small fractal transpose. Using int8_t, half, float as examples, the following sections describe B8/B16/B32 transpose scenarios.

##### B8 Input Data Type

B8 input data type fractal is 16*32. L1->L0 transpose combines 2 fractals of 16*32 in row direction into one 32*32 block for transpose. Using int8_t as example:

<p align="center">
  <img src="figures/B8_B_l1_l0B_trans_load2dv2.png">
</p>

<p align="center">
Figure 7: int8_t data type, L1->L0B transpose, one Load2DV2 data layout diagram
</p>

In this sample, k=70. Since transpose combines 2 fractals in row direction into a block (mStep must be multiple of 2), L1 row direction (k direction) has one extra invalid fractal. One Load2DV2 instruction completes L1->L0B movement with transpose.

```cpp
kbAlignL1 = CeilAlign(k, fractalShape[0] * fractalNum);
nAlignL1 = CeilAlign(n, fractalShape[1]);
kbAlignL0 = CeilAlign(k, fractalShape[1]);
nAlignL0 = CeilAlign(n, fractalShape[0] * fractalNum);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStep = CeilDivision(kbAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(nAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kbAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(nAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
AscendC::LoadData(b2Local, b1Local, loadDataParams);
```

##### B16 Input Data Type

B16 input data type fractal is 16*16. One fractal is one block. L1->L0 transpose transposes by small fractal. One Load2DV2 completes L1->L0B data movement and transpose. Using half as example:

<p align="center">
  <img src="figures/B16_B_l1_l0B_trans_load2dv2.png">
</p>

<p align="center">
Figure 8: half data type, L1->L0B transpose, one Load2DV2 data layout diagram
</p>

As shown in Figure 8, start moving from B matrix starting address in L1. k is row, configure mstep parameter; N is col, configure kStep parameter. Set ifTranspose=true. Invoke one instruction to complete B matrix L1->L0B movement with transpose.

```cpp
kbAlignL1 = CeilAlign(k, fractalShape[0] * fractalNum);
nAlignL1 = CeilAlign(n, fractalShape[1]);
kbAlignL0 = CeilAlign(k, fractalShape[1]);
nAlignL0 = CeilAlign(n, fractalShape[0] * fractalNum);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStep = CeilDivision(kbAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(nAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kbAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(nAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
AscendC::LoadData(b2Local, b1Local, loadDataParams);
```

##### B32 Input Data Type

B32 input data type fractal is 16*8. L1->L0 transpose combines 2 fractals of 16*8 in col direction into one 16*16 block for transpose. Using float as example:

<p align="center">
  <img src="figures/B32_B_l1_l0B_trans_load2dv2.png">
</p>

<p align="center">
Figure 9: float data type, L1->L0B transpose, one Load2DV2 data layout diagram
</p>

In this sample, n=50. Since transpose combines 2 fractals in col direction into a block (kStep must be multiple of 2), L1 col direction (n direction) has one extra invalid fractal. One Load2DV2 instruction completes L1->L0B movement with transpose. L0B writes one extra invalid fractal in k direction. Since L0B layout is Zn, extra invalid fractal data is at tail. Configure mmadParams.k = k during Mmad calculation to only let valid data participate.

```cpp
kbAlignL1 = CeilAlign(k, fractalShape[0]);
nAlignL1 = CeilAlign(n, fractalShape[1] * fractalNum);
kbAlignL0 = CeilAlign(k, fractalShape[1] * fractalNum);
nAlignL0 = CeilAlign(n, fractalShape[0]);
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStep = CeilDivision(kbAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(nAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(kbAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(nAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = true;
AscendC::LoadData(b2Local, b1Local, loadDataParams);
```

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.

- Configure environment variables
  Select the appropriate command based on the CANN development kit package [installation method](../../../../../../docs/en/quick_start.md#prepare&install) on your environment.
  - Default path, root user installed CANN package
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installed CANN package
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Custom path install_path, installed CANN package
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Sample execution
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin
  ```

  For CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;   # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;   # NPU simulation mode
  ```

  > **Note:** Clear cmake cache before switching build mode. Execute `rm CMakeCache.txt` in build directory before re-running cmake.

- Build option description

  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU run, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`-`7` | Scenario number: different data types and transpose combinations |

- Execution result

  The following output indicates successful precision comparison:
  ```bash
  test pass!
  ```