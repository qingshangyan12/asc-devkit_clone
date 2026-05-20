# Gelu+Element-wise High Performance Optimization Sample

## Overview

This sample uses Gelu+Element-wise computation as an example (adding a series of Element-wise operations after Gelu to increase computation depth). It introduces RegBase vector performance tuning methods. The sample demonstrates performance gains from parallelism adjustment, loop splitting, and loop unrolling.

> **Recommendation**: Read the [Gelu Performance Optimization Sample](../gelu_high_performance/README_en.md) first to understand the basic optimization principles of VF fusion, then learn the more complex Loop splitting optimization methods in this sample.

**Optimization Path**:
- Case 0: Single loop computation, parallelism adjusted to 128 bytes
- Case 1: Single loop computation, parallelism increased to 256 bytes
- Case 2: Split VF loop into two loops, executing Gelu and Element-wise separately
- Case 3: Based on Case 2, enable unroll loop unrolling optimization

## Supported Products
- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── gelu_eltwise_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // Input data and golden data generation script
│   │   └── verify_result.py    // Golden data comparison file
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── gelu_eltwise.asc        // Ascend C sample implementation (containing 4 optimization cases)
```

## Sample Description

**Sample Function**:

This sample contains two computation steps: Gelu computation and Element-wise computation.

**Gelu Approximation Formula**:

$$
GELU(x) \approx \frac{x}{1 + e^{-1.595769 \cdot (x + 0.044715 \cdot x^3)}}
$$

Where $-2 \cdot \sqrt{\frac{2}{\pi}} \approx -1.595769$, and $0.044715$ is the coefficient of the Gelu approximation formula.

**Element-wise Computation Formula**:

Perform a series of Element-wise computations based on Gelu output:

$$
y = e^{-0.5 \cdot (GELU(x) + 1)^2 + 2}
$$

**Sample Specification**:

<table border="2">
<tr><td rowspan="1" align="center">Sample Type(OpType)</td><td colspan="4" align="center">Gelu+Element-wise</td></tr>
<tr><td rowspan="2" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">[8192, 8192]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Sample Output</td><td align="center">y</td><td align="center">[8192, 8192]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">gelu_eltwise_custom</td></tr>
</table>

## Sample Implementation

### Performance Metrics Description

**Table 1 AI Core Performance Metrics Field Description**

| Field Name | Field Meaning |
|:---|:---|
|Task Duration(μs)|Total task duration, including scheduling time to accelerator, execution time on accelerator, and response end time.|
|aiv_time(μs)|Theoretical execution time of Task on AI Vector Core.|
|aiv_total_cycles|Total cycle count computed by all vector cores.|
|aiv_vec_time(μs)|vec type instruction (vector operation instruction) duration.|
|aiv_vec_ratio|Ratio of vec type instruction (vector operation instruction) cycles to total cycle count.|
|aiv_scalar_time(μs)|scalar type instruction (scalar operation instruction) duration.|
|aiv_scalar_ratio|Ratio of scalar type instruction (scalar operation instruction) cycles to total cycle count.|
|aiv_mte2_time(μs)|mte2 type instruction (GM->UB transfer instruction) duration.|
|aiv_mte2_ratio|Ratio of mte2 type instruction (GM->UB transfer instruction) cycles to total cycle count.|
|aiv_mte3_time(μs)|mte3 type instruction (UB->GM transfer instruction) duration.|
|aiv_mte3_ratio|Ratio of mte3 type instruction (UB->GM transfer instruction) cycles to total cycle count.|

### Case 0: Single Loop Computation, Parallelism Adjusted to 128 Bytes

**Implementation**: Refer to `KernelGeluEltwise::GeluEltwiseBasic()` function

The baseline program uses RegBase API to implement Gelu and Element-wise computation. Following the principle of fusing computations whenever possible, both computations are placed in the same VF loop. For comparison, parallelism is set to 128 bytes per iteration (standard computation parallelism is 256 bytes).

**Key Code**:
```cpp
__simd_vf__ inline static void GeluEltwiseBasic(
    __ubuf__ float* xAddr, __ubuf__ float* yAddr, uint32_t n, uint32_t loopNum, uint32_t oneRepeatSize)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<float> xReg, yReg;

    for (uint16_t i = 0; i < loopNum; ++i) {
        mask = AscendC::Reg::UpdateMask<float>(n);
        // Gelu computation
        AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);
        AscendC::Reg::Mul(yReg, xReg, xReg, mask);
        AscendC::Reg::Mul(yReg, yReg, xReg, mask);
        AscendC::Reg::Muls(yReg, yReg, COEFF_A, mask);
        AscendC::Reg::Add(yReg, xReg, yReg, mask);
        AscendC::Reg::Muls(yReg, yReg, COEFF_B, mask);
        AscendC::Reg::Exp(yReg, yReg, mask);
        AscendC::Reg::Adds(yReg, yReg, 1.0f, mask);
        AscendC::Reg::Div(yReg, xReg, yReg, mask);
        // Element-wise computation
        AscendC::Reg::Adds(yReg, yReg, COEFF_C, mask);
        AscendC::Reg::Mul(yReg, yReg, yReg, mask);
        AscendC::Reg::Muls(yReg, yReg, COEFF_D, mask);
        AscendC::Reg::Adds(yReg, yReg, COEFF_E, mask);
        AscendC::Reg::Exp(yReg, yReg, mask);
        AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);
    }
}
```

**Sample Configuration**:
- Multi-core splitting: 32 parts in M direction, 2 parts in N direction, totaling 64 parts, distributed across 64 cores
- `tileLen = 8192` is the number of data elements transferred and computed each time
- `oneRepeatSize = 128 / sizeof(float) = 32` elements

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| 334.437 | 333.47 | 34582356 | 244.299 | 0.733 | 3.108 | 0.009 | 275.719 | 0.827 | 252.305 | 0.757 |

**Optimization Effect Analysis**:
- End-to-end duration: **334.437μs**
- Vector instruction duration: 244.299μs, accounting for **73.3%**
- Data transfer duration: 275.719μs (read) + 252.305μs (write), transfer ratio exceeds **80%**
- Total computation cycles: 34582356

**Next Optimization Direction**:
- Increase parallelism to 256 bytes to improve data processing efficiency

---

### Case 1: Single Loop Computation, Parallelism Increased to 256 Bytes

**Implementation**: Refer to `KernelGeluEltwise::GeluEltwiseBasic()` function

Same implementation as Case 0, but with parallelism increased to 256 bytes (default value).

**Sample Configuration**:
- Multi-core splitting: 32 parts in M direction, 2 parts in N direction, totaling 64 parts, distributed across 64 cores
- `tileLen = 8192` is the number of data elements transferred and computed each time
- `oneRepeatSize = 256 / sizeof(float) = 64` elements

**Optimization Method**:
- **Parallelism Increase**:
  - Increased from 128 bytes to 256 bytes, improving single-instruction computation throughput

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| 324.862 | 324.09 | 33758796 | 125.203 | 0.386 | 3.187 | 0.01 | 292.819 | 0.904 | 280.044 | 0.864 |

**Optimization Effect Analysis**:
- End-to-end duration: **324.862μs**, reduced by **2.9%** compared to Case 0
- Vector instruction duration: 125.203μs, reduced by **48.7%** compared to Case 0
- After parallelism increase, computation instruction throughput increased, vector computation efficiency significantly improved

**Next Optimization Direction**:
- Calculation shows IPC is significantly lower than the Gelu sample (detailed calculation in later sections). Split the loop to shorten instruction computation depth in single loop and improve instruction dual-issue efficiency

---

### Case 2: Split VF Loop into Two Loops

**Implementation**: Refer to `KernelGeluEltwise::GeluEltwiseLoopSplit()` function

Split Gelu and Element-wise computation into two independent loops, executing steps 1-8 and steps 9-13 separately.

**Key Code**:
```cpp
__simd_vf__ inline static void GeluEltwiseLoopSplit(
    __ubuf__ float* xAddr, __ubuf__ float* yAddr, uint32_t n, uint32_t loopNum, uint32_t oneRepeatSize)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<float> xReg, yReg;
    uint32_t n2 = n;

    // Loop 1: Gelu computation
    for (uint16_t i = 0; i < loopNum; ++i) {
        mask = AscendC::Reg::UpdateMask<float>(n);
        AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);
        AscendC::Reg::Mul(yReg, xReg, xReg, mask);
        AscendC::Reg::Mul(yReg, yReg, xReg, mask);
        AscendC::Reg::Muls(yReg, yReg, COEFF_A, mask);
        AscendC::Reg::Add(yReg, xReg, yReg, mask);
        AscendC::Reg::Muls(yReg, yReg, COEFF_B, mask);
        AscendC::Reg::Exp(yReg, yReg, mask);
        AscendC::Reg::Adds(yReg, yReg, 1.0f, mask);
        AscendC::Reg::Div(yReg, xReg, yReg, mask);
        AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);
    }

    // Loop 2: Element-wise computation
    for (uint16_t i = 0; i < loopNum; ++i) {
        mask = AscendC::Reg::UpdateMask<float>(n2);
        AscendC::Reg::LoadAlign(yReg, yAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(yReg, yReg, COEFF_C, mask);
        AscendC::Reg::Mul(yReg, yReg, yReg, mask);
        AscendC::Reg::Muls(yReg, yReg, COEFF_D, mask);
        AscendC::Reg::Adds(yReg, yReg, COEFF_E, mask);
        AscendC::Reg::Exp(yReg, yReg, mask);
        AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);
    }
}
```

**Sample Configuration**:
- Multi-core splitting: 32 parts in M direction, 2 parts in N direction, totaling 64 parts, distributed across 64 cores
- `tileLen = 8192` is the number of data elements transferred and computed each time
- `oneRepeatSize = 256 / sizeof(float) = 64` elements

**Optimization Method**:
- **Loop Splitting Principle**:
  - Split 13 steps of computation into two loops: Gelu (8 steps) and Element-wise (5 steps)
  - In VF functions, single loop instruction computation depth is large, but execution queue depth is limited. The execution queue cannot hold more instructions, limiting instruction dual-issue capability

- **Loop Splitting Benefit Analysis**:
  - Split point selection: Choose reduce operation end point or long-latency instruction (such as Exp, Div) end point
  - Gelu computation ends with Div, Element-wise ends with Exp, both are long-latency instructions
  - After splitting, each loop has shorter instruction dependency chain, beneficial for instruction dual-issue

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| 324.039 | 323.32 | 33794573 | 97.184 | 0.301 | 3.21 | 0.01 | 292.68 | 0.905 | 279.178 | 0.863 |

**Optimization Effect Analysis**:
- End-to-end duration: **324.039μs**, reduced by **0.25%** compared to Case 1
- Vector instruction duration: 97.184μs, reduced by **22.7%** compared to Case 1
- After loop splitting, instruction dependency chain shortened, instruction dual-issue efficiency improved, vector computation efficiency further increased

**Next Optimization Direction**:
- Enable loop unrolling optimization to further improve instruction dispatch parallelism

---

### Case 3: Enable Unroll Loop Unrolling Optimization

**Implementation**: Refer to `KernelGeluEltwise::GeluEltwiseLoopSplit()` function, add `#pragma unroll 6` loop unrolling optimization

Based on Case 2, add unroll optimization to both loops.

**Key Code**:
```cpp
__simd_vf__ inline static void GeluEltwiseLoopSplit(
    __ubuf__ float* xAddr, __ubuf__ float* yAddr, uint32_t n, uint32_t loopNum, uint32_t oneRepeatSize)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<float> xReg, yReg;
    uint32_t n2 = n;

    #pragma unroll 6
    // Loop 1: Gelu computation
    for (uint16_t i = 0; i < loopNum; ++i) {
        // ...
    }

    #pragma unroll 6
    // Loop 2: Element-wise computation
    for (uint16_t i = 0; i < loopNum; ++i) {
        // ...
    }
}
```

**Sample Configuration**:
- Multi-core splitting: 32 parts in M direction, 2 parts in N direction, totaling 64 parts, distributed across 64 cores
- `tileLen = 8192` is the number of data elements transferred and computed each time
- `oneRepeatSize = 256 / sizeof(float) = 64` elements

**Optimization Method**:
- **Loop Unrolling Optimization Principle**:
  - Use `#pragma unroll 6` to tell the compiler to unroll the loop, unrolling 6 iterations each time
  - Improve instruction-level parallelism, enabling more VF instructions to be dispatched consecutively

- **Loop Unrolling Benefit Analysis**:
  - Unroll factor selection: `unroll 6` is an empirical value that requires tuning based on actual scenarios
  - Excessive unrolling: May increase register pressure, leading to performance degradation, and code size also increases
  - Insufficient unrolling: Optimization effect is not obvious
  - Recommendation: Users can use trial-and-error method to find the optimal loop unrolling count

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|
| 324.748 | 323.97 | 33905446 | 94.339 | 0.291 | 4.932 | 0.015 | 288.144 | 0.889 | 279.082 | 0.861 |

**Optimization Effect Analysis**:
- End-to-end duration: **324.748μs**, increased by **0.22%** compared to Case 2
- Vector instruction duration: 94.339μs, reduced by **2.9%** compared to Case 2
- After loop unrolling, more instructions can execute in parallel, vector computation efficiency slightly improved

---

## Performance Comparison Summary

### Ascend 950PR Performance Comparison

The following table shows the performance data comparison of this sample running on Ascend 950 series products:

| Case | Optimization Strategy | Core Count | tileLen | Task Duration(μs) | aiv_vec_time(μs) | End-to-end Duration vs Case 0 | Vector Duration vs Case 0 |
|:---|:---|:---|:---|:---|:---|:---|:---|
| 0 | Single loop computation, parallelism 128 bytes (baseline) | 64 | 8192 | 334.437 | 244.299 | 1x | 1x |
| 1 | Single loop computation, parallelism 256 bytes | 64 | 8192 | 324.862 | 125.203 | 1.03x | 1.95x |
| 2 | Loop split (Gelu + Element-wise) | 64 | 8192 | 324.039 | 97.184 | 1.03x | 2.51x |
| 3 | Loop split + unroll loop unrolling | 64 | 8192 | 324.748 | 94.339 | 1.03x | 2.58x |

> **Note:** This sample is MTE2 bound, and the performance bottleneck is data transfer. End-to-end duration changes are small, but vector computation efficiency significantly improved.

### Optimization Key Points Summary

| Optimization Method | Core Principle | Usage Recommendation |
|:---|:---|:---|
| Parallelism Adjustment | Maximize instruction parallel processing capability | Use full instruction computation parallelism |
| Loop Splitting | Shorten instruction computation depth in single loop, improve instruction dual-issue efficiency | Choose reduce end or long-latency instruction end as split point |
| Loop Unrolling | Improve instruction dispatch parallelism, reduce loop control overhead | Use `#pragma unroll N`, N requires tuning based on actual scenario |

### Theoretical Performance Analysis

The following table shows the parallelism of vector computation instructions in this sample:

**Gelu Computation Instruction Parallelism**:

| No. | Instruction | Data Type | Computation Parallelism(bytes/cycle) |
|:---:|:---:|:---:|:---:|
| 1 | Mul | float | 256 |
| 2 | Mul | float | 256 |
| 3 | Muls | float | 256 |
| 4 | Add | float | 256 |
| 5 | Muls | float | 256 |
| 6 | Exp | float | 64 |
| 7 | Adds | float | 256 |
| 8 | Div | float | 64 |

**Element-wise Computation Instruction Parallelism**:

| No. | Instruction | Data Type | Computation Parallelism(bytes/cycle) |
|:---:|:---:|:---:|:---:|
| 9 | Adds | float | 256 |
| 10 | Mul | float | 256 |
| 11 | Muls | float | 256 |
| 12 | Adds | float | 256 |
| 13 | Exp | float | 64 |

**Theoretical Vector Duration Calculation (not considering VF fusion instruction dual-issue characteristic)**:

This sample contains 13 computation steps, with instruction parallelism as follows:
- 10 linear operation instructions (Mul×3, Muls×3, Add×1, Adds×3): parallelism 256 bytes/cycle
- 3 nonlinear operation instructions (Exp×2, Div×1): parallelism 64 bytes/cycle

Cycles needed to process 1 byte of data:

$$
Cycles_{\text{per\_byte}} = 10 \times \frac{1}{256} + 3 \times \frac{1}{64} = \frac{10}{256} + \frac{12}{256} = \frac{22}{256}
$$

Equivalent data processed per cycle:

$$
P_{\text{bytes}} = \frac{256}{22} \approx 11.64 \text{ bytes/cycle}
$$

This sample runs on Ascend 950PR series products with hardware parameters:
- Frequency $f = 1650 \text{ MHz}$
- AIV core count $N_{\text{core}} = 64$
- Data shape $M = N = 8192$, total data $D = M \times N \times 4 = 268435456 \text{ bytes}$

Theoretical vector duration calculation formula:

$$
T_{\text{theory}} = \frac{D}{P_{\text{bytes}} \times f \times N_{\text{core}}}
$$

Substituting values:

$$
T_{\text{theory}} = \frac{268435456}{\frac{256}{22} \times 1650 \times 10^6 \times 64} \approx 219.0 \text{ μs}
$$

**IPC Analysis in VF Fusion Scenario**:

In VF fusion scenario, vector instructions have dual-issue characteristic. Use IPC (Instructions Per Cycle, instruction dispatch count per cycle) to measure performance.

IPC calculation formula:

$$
IPC = \frac{N_{\text{instr}}}{Cycles_{\text{vec}}}
$$

Where:
- $N_{\text{instr}}$: Single-core vector computation instruction count
- $Cycles_{\text{vec}} = T_{\text{vec}} \times f$: Single-core vector computation cycle count
- $f = 1650 \text{ MHz}$: Hardware frequency

**Case 0: Single Loop Computation, Parallelism 128 Bytes**

Single-core vector computation instruction count:

$$
N_{\text{instr}} = N_{\text{VF}} \times N_{\text{loop}} \times N_{\text{op}} = 128 \times 256 \times 13 = 425984
$$

Where:
- $N_{\text{VF}} = 128$: VF function call count (Process outer loop)
- $N_{\text{loop}} = 256$: VF internal loop count (when oneRepeatSize=32)
- $N_{\text{op}} = 13$: VF internal computation instruction count (Gelu×8 + Element-wise×5)

Vector computation cycle count:

$$
Cycles_{\text{vec}} = T_{\text{vec}} \times f = 244.299 \text{ μs} \times 1650 \text{ MHz} = 401094
$$

IPC calculation:

$$
IPC = \frac{425984}{401094} \approx 1.06
$$

---

**Case 1: Single Loop Computation, Parallelism 256 Bytes**

Single-core vector computation instruction count:

$$
N_{\text{instr}} = N_{\text{VF}} \times N_{\text{loop}} \times N_{\text{op}} = 128 \times 128 \times 13 = 212992
$$

Where:
- $N_{\text{VF}} = 128$: VF function call count
- $N_{\text{loop}} = 128$: VF internal loop count (when oneRepeatSize=64)
- $N_{\text{op}} = 13$: VF internal computation instruction count

Vector computation cycle count:

$$
Cycles_{\text{vec}} = T_{\text{vec}} \times f = 125.203 \text{ μs} \times 1650 \text{ MHz} = 205835
$$

IPC calculation:

$$
IPC = \frac{212992}{205835} \approx 1.03
$$

**IPC Analysis**: Compared to Case 0, Case 1 has slightly lower IPC (1.03 vs 1.06), but vector duration significantly reduced (reduced by 48.7%). Note that while IPC is generally used to measure VF performance, the prerequisite is to maximize instruction parallelism usage.

---

**Case 2: Loop Splitting (Gelu + Element-wise)**

Single-core vector computation instruction count remains unchanged:

$$
N_{\text{instr}} = N_{\text{VF}} \times N_{\text{loop}} \times N_{\text{op}} = 128 \times 128 \times 13 = 212992
$$

Parameters are the same as Case 1. Loop splitting does not affect total instruction count.

Vector computation cycle count:

$$
Cycles_{\text{vec}} = T_{\text{vec}} \times f = 97.184 \text{ μs} \times 1650 \text{ MHz} = 159861
$$

IPC calculation:

$$
IPC = \frac{212992}{159861} \approx 1.33
$$

**IPC Analysis**: Compared to Case 1, Case 2 IPC increased from 1.03 to 1.33, an increase of **29.1%**. After loop splitting, instruction dependency chain shortened, and instruction dual-issue efficiency significantly improved.

---

**Case 3: Loop Splitting + Unroll Loop Unrolling**

Single-core vector computation instruction count remains unchanged:

$$
N_{\text{instr}} = N_{\text{VF}} \times N_{\text{loop}} \times N_{\text{op}} = 128 \times 128 \times 13 = 212992
$$

Parameters are the same as Case 2. Loop unrolling does not affect total instruction count.

Vector computation cycle count:

$$
Cycles_{\text{vec}} = T_{\text{vec}} \times f = 94.339 \text{ μs} \times 1650 \text{ MHz} = 155069
$$

IPC calculation:

$$
IPC = \frac{212992}{155069} \approx 1.38
$$

**IPC Analysis**: Compared to Case 2, Case 3 IPC increased from 1.33 to 1.38, an increase of **3.8%**. After loop unrolling, more instructions can execute in parallel, further improving instruction dispatch efficiency.

---

### IPC Comparison Summary

| Case | Optimization Strategy | Vector Duration(μs) | IPC | IPC vs Case 0 |
|:---:|:---|:---:|:---:|:---:|
| 0 | Single loop computation, parallelism 128 bytes (baseline) | 244.299 | 1.06 | 1x |
| 1 | Single loop computation, parallelism 256 bytes | 125.203 | 1.03 | 0.97x |
| 2 | Loop splitting (Gelu + Element-wise) | 97.184 | 1.33 | 1.25x |
| 3 | Loop splitting + unroll loop unrolling | 94.339 | 1.38 | 1.31x |

**IPC Optimization Recommendations**:
- Higher IPC is better. The theoretical limit approaches 2.0. In most cases, achieving 1.4~1.5 indicates good performance
- Case 3 IPC is 1.38, approaching ideal performance level
- Loop splitting is an optimization method to improve IPC. It requires analysis and experimentation based on actual VF computation depth

---

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.

- Switch Case

  Specify the case to compile through `-DSCENARIO_NUM=N` during cmake compilation. Case descriptions:
  - `0`: Single loop computation, parallelism 128 bytes
  - `1`: Single loop computation, parallelism 256 bytes
  - `2`: Loop splitting (Gelu + Element-wise)
  - `3`: Loop splitting + unroll loop unrolling

- Configure Environment Variables

  Select the corresponding environment variable configuration command based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  - Default path, root user installed CANN software package
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installed CANN software package
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, installed CANN software package
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Sample Execution

  ```bash
  SCENARIO_NUM=0
  mkdir -p build && cd build;      # Create and enter build directory
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # Build project, default npu mode
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                           # Execute compiled executable program, run sample
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output correctness, confirm algorithm logic is correct
  ```

  For NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU simulation mode
  ```

  > **Note:** Clear cmake cache before switching compilation mode or Case. Execute `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build Options Description

  | Option | Available Values | Description |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU run, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `0`, `1`, `2`, `3` | Case number: 0=single loop+128bytes, 1=single loop+256bytes, 2=loop split, 3=loop split+unroll |

- Execution Result

  The execution result is as follows, indicating successful accuracy comparison.
  ```bash
  error ratio: 0.0000, tolerance:0.0001
  test pass!
  ```

### Performance Analysis

Use the `msprof` tool to obtain detailed performance data:

```bash
msprof ./demo   # Analyze performance
```

A folder with PROF_ prefix will be generated in the current directory. The `mindstudio_profiler_output` directory contains the performance data summary of Host and each Device. For performance data analysis, check the files in this directory.

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Save Host and each Device performance data summary
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

View specific performance analysis results:
```bash
# View Task Duration and various data
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```