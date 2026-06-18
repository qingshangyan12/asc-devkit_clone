# SIMT and SIMD Hybrid Programming High-Performance Optimization Example

## Overview

This example uses FloorMod computation to introduce performance tuning methods in the SIMD and SIMT hybrid programming scenario. The example demonstrates the performance differences between directly using SIMT to access GM, using SIMD RegBase computation, using SIMT to access UB with non-continuous thread mapping, and adjusting thread mapping so that adjacent threads within a Warp access UB continuously.

**Optimization Path**:

| Case | SCENARIO_NUM | Implementation | Description |
|:---|:---:|:---|:---|
| Case 0 | 0 | SIMT Direct GM Access | Both data access and computation are implemented through SIMT, with SIMT threads directly accessing GM, serving as the pure SIMT implementation control group. |
| Case 1 | 1 | SIMD RegBase | Both data access and computation are implemented through SIMD, implementing FloorMod computation based on RegBase, serving as the pure SIMD implementation control group. |
| Case 2 | 2 | SIMT Non-Continuous UB Access | Based on SIMD DataCopy interface for continuous GM to UB transfer, each SIMT thread processes a segment of continuous elements, but adjacent threads within a Warp access non-continuously. |
| Case 3 | 3 | SIMT Continuous UB Access | Based on SIMD DataCopy interface for continuous GM to UB transfer, adjusts SIMT thread mapping so that adjacent threads within a Warp access continuous addresses. |

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | > CANN 9.0.0 |

## Directory Structure

```
├── simt_and_simd_high_performance
│   ├── CMakeLists.txt        // Build project file
│   ├── data_utils.h          // Data read/write functions
│   ├── figures               // Illustrations
│   ├── floor_mod.asc         // Ascend C example implementation
│   ├── README.md             // Example documentation
│   └── scripts
│       ├── gen_data.py       // Input data and ground truth generation script
│       └── verify_result.py  // Ground truth comparison script
```

## Example Description

**Example Function**:

The example implements FloorMod computation. FloorMod is commonly used in tensor computation for periodic indexing, group mapping, and other scenarios that require maintaining consistent negative modulo semantics. The computation formula is:

$$
r = x \bmod y
$$

$$
z =
\begin{cases}
r + y, & r \ne 0 \land ((r < 0) \ne (y < 0)) \\
r, & \text{otherwise}
\end{cases}
$$

**Example Specifications**:

<table>
<tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">FloorMod</td></tr>
<tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">[8192, 8192]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td align="center">y</td><td align="center">[8192, 8192]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[8192, 8192]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="4" align="center">Kernel Function Name</td><td align="center">SCENARIO_NUM=0</td><td colspan="3" align="center"><code>floor_mod_gm_simt_custom</code></td></tr>
<tr><td align="center">SCENARIO_NUM=1</td><td colspan="3" align="center"><code>floor_mod_simd_custom</code></td></tr>
<tr><td align="center">SCENARIO_NUM=2</td><td colspan="3" align="center"><code>floor_mod_simt_non_contiguous_custom</code></td></tr>
<tr><td align="center">SCENARIO_NUM=3</td><td colspan="3" align="center"><code>floor_mod_simt_contiguous_custom</code></td></tr>
</table>

## Example Implementation

### Performance Metric Description

| Field Name | Field Meaning |
|:---|:---|
| Task Duration(μs) | Total Task duration, including scheduling time to the accelerator, execution time on the accelerator, and completion response time. |
| aiv_time(μs) | Task execution time on AI Vector Core. |
| aiv_vec_time(μs) | vec type instruction duration. Thread computation in SIMT VF calls is also reflected in this metric. |
| aiv_vec_ratio | Ratio of vec type instruction cycle count to total cycle count. |
| aiv_scalar_time(μs) | Scalar type instruction duration. |
| aiv_scalar_ratio | Ratio of scalar type instruction cycle count to total cycle count. |
| aiv_mte2_time(μs) | MTE2 type instruction duration, primarily corresponding to GM to UB transfers. |
| aiv_mte2_ratio | Ratio of MTE2 type instruction cycle count to total cycle count. |
| aiv_mte3_time(μs) | MTE3 type instruction duration, primarily corresponding to UB to GM transfers. |
| aiv_mte3_ratio | Ratio of MTE3 type instruction cycle count to total cycle count. |

### Case 0: SIMT Direct GM Access

**Implementation**: Refer to the `floor_mod_simt_gm_contiguous()` function implementation.

This implementation does not use MTE2/MTE3 for data transfer between GM and UB. Instead, SIMT threads directly read `x` and `y` from GM, compute, and write back directly to GM.

**Key Code**:

```cpp
__aicore__ inline void ProcessGmSimt()
{
    asc_vf_call<floor_mod_simt_gm_contiguous<T>>(
        dim3(THREAD_COUNT), const_cast<__gm__ T*>(xGm.GetPhyAddr()),
        const_cast<__gm__ T*>(yGm.GetPhyAddr()), const_cast<__gm__ T*>(zGm.GetPhyAddr()),
        DataLenPerCore);
}

__simt_vf__ inline void floor_mod_simt_gm_contiguous(
    __gm__ T* x, __gm__ T* y, __gm__ T* z, uint32_t inputTotalLength)
{
    for (uint32_t index = static_cast<uint32_t>(threadIdx.x); index < inputTotalLength;
         index += static_cast<uint32_t>(blockDim.x)) {
        T yValue = y[index];
        const auto rem = x[index] % yValue;
        bool signsDiffer = ((rem < 0) != (yValue < 0));
        z[index] = (signsDiffer && (rem != 0)) ? rem + yValue : rem;
    }
}
```

**Performance Data**:

| Case | Implementation | Cores | Task Duration(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | SIMT Direct GM Access | 64 | 867.222 | 863.175 | 0.996 | 0.005 | 0.000 | 0.003 | 0.000 |

**Performance Data Analysis**:

- This Case's `aiv_mte2_time` and `aiv_mte3_time` are close to 0, indicating that data reads and writes do not go through the MTE2/MTE3 GM->UB/UB->GM transfer path.
- `Task Duration` is **867.222μs**, with `aiv_vec_time` at **863.175μs**, `aiv_vec_ratio` reaching **0.996**, with duration primarily concentrated on SIMT internal GM reads/writes and FloorMod computation.

**Principle Description**:

When SIMT threads directly access GM, each thread needs to read `x`, `y` from GM during computation and write results back to GM, resulting in low access efficiency. Data transfer and computation are coupled in the same SIMT thread segment, preventing parallel transfer and computation pipelining.

The input and output in this example are sequentially arranged 2D ND data on GM, suitable for continuous block transfer. Subsequent implementations introduce the DataCopy interface from the SIMD programming paradigm. GM->UB and UB->GM transfers use DataCopy, performed by MTE, which can transfer GM data into UB in larger continuous data blocks, then continuously write UB results back to GM. Compared to SIMT threads accessing GM element by element, this approach has higher memory access efficiency and can parallelize data transfer and computation pipelining.

**Next Optimization Direction**:

The current Case serves as the baseline for pure SIMT direct GM access, used to display performance data for the pure SIMT implementation example; the next step will introduce SIMD/RegBase implementation, using continuous DataCopy for GM->UB and UB->GM transfers, and implementing FloorMod computation based on RegBase to display performance data for the pure SIMD implementation.

### Case 1: SIMD RegBase Computation

**Implementation**: Refer to the `floor_mod_simd()` function implementation.

This implementation uses DataCopy for data transfer between GM and UB, and in the RegBase VF function loads UB data to registers through `LoadAlign`, executes vector instructions such as `Div`, `Mul`, `Sub`, `Compare`, `Select`, and finally writes back to UB through `StoreAlign`. Both computation and transfer processes are implemented through SIMD.

**Key Code**:

```cpp
__aicore__ inline void CopyIn(
    uint32_t tileIdx, uint32_t count, AscendC::LocalTensor<T>& xLocal, AscendC::LocalTensor<T>& yLocal)
{
    uint32_t tileOffset = tileIdx * TileLength;
    AscendC::DataCopy(xLocal, xGm[tileOffset], count);
    AscendC::DataCopy(yLocal, yGm[tileOffset], count);
}
// SIMD Compute(UB)
template <typename T>
__simd_vf__ inline void floor_mod_simd(
    __ubuf__ T* zAddr, __ubuf__ T* xAddr, __ubuf__ T* yAddr, const uint32_t count)
{
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
    uint16_t loopTimes = AscendC::CeilDivision(count, oneRepeatSize);
    AscendC::Reg::RegTensor<T> xValue, yValue, modValue, tempValue, defaultValue, signValue;
    AscendC::Reg::MaskReg mask, selectMask, adjustMask;
    uint32_t maskCount = count;

    AscendC::Reg::Duplicate(defaultValue, T(-1));
    AscendC::Reg::Duplicate(signValue, FMOD_B32_SIGN);

    for (uint16_t i = 0; i < loopTimes; i++) {
        mask = AscendC::Reg::UpdateMask<T>(maskCount);
        AscendC::Reg::LoadAlign(xValue, xAddr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(yValue, yAddr + i * oneRepeatSize);
        AscendC::Reg::Div(tempValue, xValue, yValue, mask);
        AscendC::Reg::Mul(tempValue, yValue, tempValue, mask);
        AscendC::Reg::Sub(modValue, xValue, tempValue, mask);
        AscendC::Reg::Compares<T, AscendC::CMPMODE::NE>(selectMask, yValue, T(0), mask);
        AscendC::Reg::Select(tempValue, modValue, defaultValue, selectMask);
        AscendC::Reg::Add(modValue, tempValue, yValue, mask);
        AscendC::Reg::Compares<T, AscendC::CMPMODE::NE>(adjustMask, tempValue, T(0), mask);
        AscendC::Reg::And(xValue, tempValue, signValue, mask);
        AscendC::Reg::And(yValue, yValue, signValue, mask);
        AscendC::Reg::Compare<T, AscendC::CMPMODE::NE>(selectMask, xValue, yValue, mask);
        AscendC::Reg::MaskAnd(adjustMask, selectMask, adjustMask, mask);
        AscendC::Reg::Select(modValue, modValue, tempValue, adjustMask);
        AscendC::Reg::StoreAlign(zAddr + i * oneRepeatSize, modValue, mask);
    }
}

__aicore__ inline void CopyOut(uint32_t tileIdx, uint32_t count, AscendC::LocalTensor<T>& zLocal)
{
    uint32_t tileOffset = tileIdx * TileLength;
    AscendC::DataCopy(zGm[tileOffset], zLocal, count);
}
```

**Performance Data**:

| Case | Implementation | Cores | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 1 | SIMD RegBase | 64 | 532.144 | 531.270 | 523.082 | 0.985 | 3.544 | 0.007 | 237.127 | 0.446 | 55.417 | 0.104 |

**Performance Data Analysis**:

- Compared to Case 0, Case 1 introduces DataCopy for GM->UB/UB->GM transfer, `Task Duration` decreases from **867.222μs** to **532.144μs**, end-to-end duration reduction of **38.6%**.
- `Task Duration` is **532.144μs**, with `aiv_vec_time` at **523.082μs**, `aiv_vec_ratio` reaching **98.5%**, indicating duration is primarily concentrated on the RegBase vector computation path.
- `aiv_mte2_ratio` is **44.6%**, `aiv_mte3_ratio` is **10.4%**, transfer duration can overlap with the computation process to some extent; the current main bottleneck is not the GM transfer process but the FloorMod computation vector instruction execution itself.

**Principle Description**:

FloorMod computation includes not only remainder calculation but also conditional correction based on the sign relationship between remainder and divisor. When implementing with SIMD RegBase, branch judgment cannot be written directly as standard `if/else`; it needs to be decomposed into multiple vector instructions such as `Compare`, `And`, `MaskAnd`, `Select` to express conditional logic. Additionally, `Div`, `Mul`, `Sub`, sign judgment, and final selection have long data dependency chains, with subsequent instructions needing to wait for preceding results, resulting in high `aiv_vec_ratio` and manifesting as Vec bound.

Compared to SIMD, SIMT is more suitable for handling per-element computation with branch judgment, as it can directly use `%` and conditional statements within threads to complete FloorMod correction, reducing the extra computation chain from assembling branch logic with vector mask instructions.

**Next Optimization Direction**:

The current Case serves as the baseline for SIMD RegBase computation, used to display performance data for the pure SIMD implementation; the next step will optimize the Vector computation part using SIMT while retaining the continuous DataCopy transfer method.

### Case 2: SIMT Non-Continuous UB Access

**Implementation**: Refer to the `floor_mod_simt_non_contiguous()` function implementation.

This implementation first transfers `x` and `y` from GM to UB continuously through DataCopy, then uses SIMT programming for the computation process, completing branch judgment through if-else statements. Each thread processes a continuous segment of 8 elements, but adjacent threads within the same Warp access non-continuous addresses:

**Key Code**:

```cpp
// Single tile core flow: CopyIn(GM->UB) -> SIMT Compute(UB) -> CopyOut(UB->GM)
__aicore__ inline void CopyIn(
    uint32_t tileIdx, uint32_t count, AscendC::LocalTensor<T>& xLocal, AscendC::LocalTensor<T>& yLocal)
{
    uint32_t tileOffset = tileIdx * TileLength;
    AscendC::DataCopy(xLocal, xGm[tileOffset], count);
    AscendC::DataCopy(yLocal, yGm[tileOffset], count);
}

// SIMT Compute(UB)
template <typename T>
__simt_vf__ inline void floor_mod_simt_non_contiguous(
    __ubuf__ T* x, __ubuf__ T* y, __ubuf__ T* z, uint32_t inputTotalLength)
{
    constexpr uint32_t elems_per_thread = TileLength / THREAD_COUNT;
    uint32_t tid = threadIdx.x;
    for (uint32_t i = 0; i < elems_per_thread; i++) {
        uint32_t index = tid * elems_per_thread + i;
        if (index >= inputTotalLength) {
            break;
        }
        T yValue = y[index];
        auto rem = x[index] % yValue;
        bool signsDiffer = ((rem < 0) != (yValue < 0));
        if (signsDiffer && (rem != 0)) {
            z[index] = rem + yValue;
        } else {
            z[index] = rem;
        }
    }
}

__aicore__ inline void CopyOut(uint32_t tileIdx, uint32_t count, AscendC::LocalTensor<T>& zLocal)
{
    uint32_t tileOffset = tileIdx * TileLength;
    AscendC::DataCopy(zGm[tileOffset], zLocal, count);
}
```

**Performance Data**:

| Case | Implementation | Cores | Task Duration(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---:|---:|---:|---:|---:|---:|---:|---:|
| 2 | SIMT Non-Continuous UB Access | 64 | 542.492 | 507.571 | 0.937 | 534.293 | 0.986 | 41.851 | 0.077 |

**Performance Data Analysis**:

- Compared to Case 0, Case 2 uses DataCopy for continuous GM->UB/UB->GM transfer, `Task Duration` decreases from **867.222μs** to **542.492μs**, end-to-end duration reduction of **37.4%**, indicating that after introducing UB relay, the overhead from direct GM access is significantly mitigated.
- Compared to Case 1, Case 2 `aiv_vec_time` decreases from **523.082μs** to **507.571μs**, a **3.0%** decrease, indicating some benefit on the computation side after using SIMT to express FloorMod per-element computation.
- However, Case 2 `Task Duration` increases by **1.9%** compared to Case 1, with no end-to-end optimization benefit. The main reason is `aiv_mte2_time` increases from Case 1's **237.127μs** to **534.293μs**, a **125.3%** increase; `aiv_mte2_ratio` also reaches **0.986**.

**Principle Description**:

Case 2's indexing method is `index = tid * elems_per_thread + i`, where `elems_per_thread = 8`. From a single thread perspective, it processes 8 continuous elements;

Ascend 950PR/950DT UB is divided into 16 banks, organized into 8 bank groups, with each bank row being 32B (every 32B falls on the next bank consecutively). The vector computation unit can read/write one row (32B) from each bank group per beat. The 32 threads within a Warp execute the same instruction in the same beat, each with its own addressing.

In Case 2, adjacent threads `tid` and `tid+1` in the same iteration `i` access elements that differ by `elems_per_thread = 8`, meaning byte addresses differ by `8 * sizeof(int32) = 32B`, exactly one bank row span. This means adjacent threads fall on **different bank rows**, and the hardware cannot serve multiple threads simultaneously with one row read (32B). The same amount of data requires more beats to fetch, elongating UB access.

<div align="center">
  <img src="./figures/ubBank.png" alt="Ascend 950PR UB Banks Diagram" width="600">
</div>

**Next Optimization Direction**:

Although Case 2 improves FloorMod computation expression with SIMT, non-continuous UB access significantly elongates MTE2 transfer time, resulting in worse end-to-end performance than the pure SIMD RegBase implementation. The next step will adjust the thread-to-data mapping while retaining continuous DataCopy transfer and SIMT computation expression, changing from "each thread processes one continuous data segment" to "adjacent threads within the same Warp access continuous elements in the same iteration", so that adjacent threads fall on the same 32B bank row, allowing one row read to serve multiple threads simultaneously, improving UB access efficiency.

### Case 3: SIMT Continuous UB Access

**Implementation**: Refer to the `floor_mod_simt_contiguous()` function implementation.

This implementation also transfers `x` and `y` from GM to UB continuously through DataCopy, but changes the SIMT thread mapping method so that adjacent threads within the same Warp access adjacent elements:

**Key Code**:

```cpp
// Single tile core flow: CopyIn(GM->UB) -> SIMT Compute(UB) -> CopyOut(UB->GM)
__aicore__ inline void CopyIn(
    uint32_t tileIdx, uint32_t count, AscendC::LocalTensor<T>& xLocal, AscendC::LocalTensor<T>& yLocal)
{
    uint32_t tileOffset = tileIdx * TileLength;
    AscendC::DataCopy(xLocal, xGm[tileOffset], count);
    AscendC::DataCopy(yLocal, yGm[tileOffset], count);
}
// SIMT Compute(UB)
template <typename T>
__simt_vf__ inline void floor_mod_simt_contiguous(
    __ubuf__ T* x, __ubuf__ T* y, __ubuf__ T* z, uint32_t inputTotalLength)
{
    for (uint32_t index = static_cast<uint32_t>(threadIdx.x); index < inputTotalLength;
         index += static_cast<uint32_t>(blockDim.x)) {
        T yValue = y[index];
        const auto rem = x[index] % yValue;
        bool signsDiffer = ((rem < 0) != (yValue < 0));
        if (signsDiffer && (rem != 0)) {
            z[index] = rem + yValue;
        } else {
            z[index] = rem;
        }
    }
}

__aicore__ inline void CopyOut(uint32_t tileIdx, uint32_t count, AscendC::LocalTensor<T>& zLocal)
{
    uint32_t tileOffset = tileIdx * TileLength;
    AscendC::DataCopy(zGm[tileOffset], zLocal, count);
}
```

**Performance Data**:

| Case | Implementation | Cores | Task Duration(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|:---:|---:|---:|---:|---:|---:|---:|---:|
| 3 | SIMT Continuous UB Access | 64 | **457.402** | **319.948** | 0.701 | 437.758 | 0.959 | 110.623 | 0.242 |

**Performance Data Analysis**:

- Compared to Case 2, Case 3 still uses DataCopy for continuous GM->UB/UB->GM transfer, but adjusts thread mapping so adjacent threads within a Warp access adjacent elements, `Task Duration` decreases from **542.492μs** to **457.402μs**, end-to-end duration reduction of **15.7%**.
- Compared to Case 1, Case 3 `Task Duration` decreases by **14.0%**.
- `aiv_vec_time` decreases from Case 2's **507.571μs** to **319.948μs**, a **37.0%** decrease. Case 3 makes adjacent threads within a Warp access adjacent elements, with 32 adjacent threads falling on several continuous 32B bank rows, each row serving multiple threads simultaneously with one read, merging UB access and reducing required beats;
- `aiv_mte2_time` decreases from Case 2's **534.293μs** to **437.758μs**, a **18.1%** decrease, indicating that continuous access has already mitigated SIMT computation stage interference with MTE2 UB resources; however, `aiv_mte2_ratio` still reaches **0.959**, indicating the current main bottleneck has shifted to the MTE2 transfer path.

## Performance Comparison Summary

### Ascend 950PR Performance Comparison

The table below shows performance data comparison for this example running on Ascend 950 Series Products:

| Case | Implementation | Cores | Task Duration(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio | Main Bottleneck |
|:---|:---|:---:|---:|---:|---:|---:|---:|---:|---:|:---|
| 0 | SIMT Direct GM Access | 64 | 867.222 | 863.175 | 0.996 | 0.005 | 0.000 | 0.003 | 0.000 | SIMT Direct GM Access |
| 1 | SIMD RegBase | 64 | 532.144 | 523.082 | 0.985 | 237.127 | 0.446 | 55.417 | 0.104 | Vec bound |
| 2 | SIMT Non-Continuous UB Access | 64 | 542.492 | 507.571 | 0.937 | 534.293 | 0.986 | 41.851 | 0.077 | Cross-bank-row UB Access |
| 3 | SIMT Continuous UB Access | 64 | **457.402** | **319.948** | 0.701 | 437.758 | 0.959 | 110.623 | 0.242 | MTE2 bound |

### Optimization Summary

| Optimization Method | Core Principle | Example Demonstration |
|:---|:---|:---|
| Use SIMD for continuous data transfer | When SIMT directly accesses GM, access granularity and cache paths may result in low bandwidth utilization; first transfer continuously into UB with DataCopy, then have SIMT access UB to improve access efficiency | Case 3 compared to Case 0 end-to-end duration reduction of 47.3% |
| Use SIMT for branch judgment | For per-element computation with conditional correction, SIMD RegBase requires multiple Compare/Select/Mask instructions to express branches, with long dependency chains; SIMT can directly express `%` and conditional judgment within threads, reducing the overhead of assembling branch logic with vector masks | Case 2 compared to Case 1 `aiv_vec_time` decrease of 3.0%; after continuous access, Case 3 compared to Case 2 `aiv_vec_time` decrease of 37.0% |
| Adjust thread mapping for continuous UB access within Warp | UB is organized as 16 banks/8 bank groups, with 32B per bank row, hardware reads/writes one row per bank group per beat; making adjacent threads within the same Warp access adjacent elements, falling on the same 32B bank row, allows one row read to serve multiple threads simultaneously, more efficient than "continuous within single thread, adjacent threads crossing 32B rows" | Case 3 changes `tid*8+i` to `tid+i*1024` pattern, compared to Case 2 end-to-end duration reduction of 15.7% |

---

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Switch Case

  Specify the Case to build through `-DSCENARIO_NUM=N` during cmake build. Case descriptions:
  - `0`: SIMT direct GM access
  - `1`: SIMD RegBase
  - `2`: SIMT non-continuous access
  - `3`: SIMT continuous access

  Example:
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=3
  ```

- Configure environment variables

  Configure environment variables based on the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in this example directory.
  ```bash
  SCENARIO_NUM=1                                                       # Select execution scenario
  mkdir -p build && cd build;                                          # Create and enter the build directory
  cmake -DCMAKE_ASC_RUN_MODE=npu -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # Build the project
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify whether the output result is correct and confirm algorithm logic correctness
  ```

 To use NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

- Build option description

  | Option | Values | Description |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture, corresponding to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `0`, `1`, `2`, `3` | Case number: 0=SIMT direct GM access, 1=SIMD RegBase, 2=SIMT non-continuous access, 3=SIMT continuous access |

- Execution results

  The following execution result indicates that the accuracy comparison succeeded.
  ```bash
  test pass!
  ```

### Performance Analysis

Use the `msprof` tool to obtain detailed performance data.

```bash
msprof ./demo   # Analyze performance
```

A PROF_ prefixed folder is generated in the current directory. The `mindstudio_profiler_output` directory stores Host and Device performance data summaries. Performance data analysis is recommended to view files in this directory.

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Stores Host and Device performance data summaries
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

View the specific performance analysis results:
```bash
# View Task Duration and various data
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
