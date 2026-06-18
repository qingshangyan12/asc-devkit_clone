# SIMD and SIMT Hybrid Programming Integer Fast Division Example

## Overview

This example uses fixed-divisor integer division as a case study to introduce integer division optimization methods in the SIMD and SIMT hybrid programming scenario. The example demonstrates the performance difference between using standard division directly within SIMT threads and using multiplication and shift to replace standard division after precomputing parameters.

**Optimization Path**:

| Case | SCENARIO_NUM | Implementation | Description |
|:---|:---:|:---|:---|
| Case 0 | 0 | Standard Division | Uses `/` directly within SIMT threads for integer division, serving as the baseline before fixed-divisor optimization. |
| Case 1 | 1 | Fast Division | Precomputes `magic` and `shift` for fixed divisors, uses multiplication and shift within SIMT threads to replace standard division. |

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Versions

- \>= CANN 9.1.0

## Directory Structure

```
├── simt_and_simd_integer_fast_div
│   ├── CMakeLists.txt          // Build project file
│   ├── integer_div.asc         // Integer division example implementation
│   └── README.md
```

## Example Description

**Example Function**:

The example implements fixed-divisor uint32 integer division and compares the performance of standard division and fast division computation methods within SIMT threads. The computation formula is:

```cpp
output[i] = input[i] / divisor
```

- input is the input data, shape [N], data type uint32, value range [0, INT32_MAX]
- divisor is the fixed divisor, data type uint32
- output is the output data, shape [N], data type uint32

**Example Specifications**:

<table>
<tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">integer_div</td></tr>
<tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">input</td><td align="center">[8388608]</td><td align="center">uint32</td><td align="center">ND</td></tr>
<tr><td align="center">divisor</td><td align="center">scalar</td><td align="center">uint32</td><td align="center">-</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[8388608]</td><td align="center">uint32</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center"><code>integer_div_kernel</code></td></tr>
</table>

## Example Implementation

Both cases use the same data transfer flow: first transfer the data assigned to the current thread block from GM to UB, SIMT VF completes the division computation on UB and writes the result to UB, and finally writes the result from UB back to GM. This avoids data transfer differences affecting performance comparison, so that performance differences mainly come from standard division vs. fast division themselves.

The shared data transfer and SIMT invocation flow for both cases is as follows:

```cpp
asc_copy_gm2ub_align(input_buf, input + block_offset, 1, blk_length, 0, 0, false, 0, 0, 0);

if ASC_IS_AIV {
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
}

if constexpr (scenario == 0) {
    asc_vf_call<simt_normal_div>(dim3(THREAD_COUNT), input_buf, output_buf, divisor, total_length);
} else {
    asc_vf_call<simt_fast_div>(dim3(THREAD_COUNT), input_buf, output_buf, magic, shift, total_length);
}

if ASC_IS_AIV {
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
}

asc_copy_ub2gm_align(output + block_offset, output_buf, 1, blk_length, 0, 0, 0);
```

### Performance Metric Description

| Field Name | Field Meaning |
|:---|:---|
| Task Duration(μs) | Total Task duration, including scheduling time to the accelerator, execution time on the accelerator, and completion response time. |
| aiv_time(μs) | Task execution time on AI Vector Core. |
| aiv_vec_time(μs) | Computation instruction duration. Thread computation in SIMT VF calls is also reflected in this metric. |
| aiv_vec_ratio | Ratio of computation instruction cycle count to total cycle count. |
| aiv_scalar_time(μs) | Scalar type instruction duration. |
| aiv_scalar_ratio | Ratio of scalar type instruction cycle count to total cycle count. |
| aiv_mte2_time(μs) | MTE2 type instruction duration, primarily corresponding to GM to UB transfers. |
| aiv_mte2_ratio | Ratio of MTE2 type instruction cycle count to total cycle count. |
| aiv_mte3_time(μs) | MTE3 type instruction duration, primarily corresponding to UB to GM transfers. |
| aiv_mte3_ratio | Ratio of MTE3 type instruction cycle count to total cycle count. |

### Case 0: Standard Division Version

**Example Objective**: Implement basic integer division functionality as the duration comparison baseline for the fast division version

**Core Implementation**:

- Each SIMT thread reads 1 input element from UB and uses `/` directly for integer division.

**Key Code**:

The `simt_normal_div()` function performs standard division on the input element `value` processed by the current thread:

```cpp
uint32_t result = value / divisor;
```

**Performance Data**:

| Case | Implementation | Data Size | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Case 0 | Standard Division | 8388608 | 110.167 | 109.110 | 40.685 | 0.373 | 10.701 | 0.098 | 36.229 | 0.332 | 18.678 | 0.171 |

**Performance Data Analysis**:

- Case 0 `Task Duration` is 110.167μs, serving as the comparison baseline for the fast division version.
- Case 0 `aiv_time` is 109.110μs, with `aiv_vec_time` at 40.685μs, serving as the baseline for standard division execution time on AI Vector Core and computation instruction duration.

---

### Case 1: Fast Division Version

**Optimization Objective**: For fixed-divisor integer division scenarios, use multiplication and shift to replace standard division, reducing division computation overhead within SIMT threads

**Core Implementation**:

- On the SIMD side, compute `magic` and `shift` needed for fast division based on the fixed divisor; the results can be reused by SIMT threads.
- Each SIMT thread reads 1 input element from UB and uses `__umulhi` and right shift within the thread to complete equivalent division computation.

#### Fast Division Conversion Principle

For uint32 integer division, the following transformation can convert division to multiplication and shift operations:

$$
\left\lfloor \frac{n}{d} \right\rfloor =
\left\lfloor \frac{n}{d} \times \frac{2^s}{2^s} \right\rfloor =
\left\lfloor \frac{2^s}{d} \times \frac{n}{2^s} \right\rfloor =
\left\lfloor {m} \times \frac{n}{2^s} \right\rfloor
$$

Where $n$ is the dividend, $d$ is the divisor, $m = \frac{2^s}{d}$. Division of $n$ by $2^s$ can be implemented through right shift by $s$ bits. To avoid $m \times n$ multiplication overflow, $m$ is decomposed as:

$$
m = (m - 2^{32}) + 2^{32}
$$

Let $magic = m - 2^{32}$, substituting into the formula:

$$
\left\lfloor \frac{n}{d} \right\rfloor =
\left\lfloor \frac{n \times {magic} + n \times 2^{32}}{2^s} \right\rfloor =
\left\lfloor \left(\frac{n \times magic}{2^{32}} + n\right) >> (s-32) \right\rfloor =
\left\lfloor \left(\frac{n \times magic}{2^{32}} + n\right) >> shift \right\rfloor
$$

Where `magic` and `shift` represent the multiplication magic number and shift bits needed for fast division.
In this example, based on the above division conversion approach, the SIMD part uses Scalar computation unit `clz()` and `bcnt1()` scalar computation interfaces to obtain `magic` and `shift` for fast division, for SIMT thread reuse.

**Key Code**:

The `calc_magic_shift()` function computes `magic` and `shift` for fast division:

```cpp
int64_t pos = BIT_64_LEN - CountLeadingZero(divisor);
int64_t cnt1 = GetBitCount1(divisor);
uint32_t shift = (cnt1 == 1) ? (pos - 1) : pos;
uint32_t magic = (1l << BIT_32_LEN) * ((1l << shift) - divisor) / divisor + 1;
```

The `simt_fast_div()` function performs fast division on the input element `value` processed by the current thread:

```cpp
uint32_t q = __umulhi(value, magic);
uint32_t result = (value + q) >> shift;
```

**Performance Data**:

| Case | Implementation | Data Size | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Case 1 | Fast Division | 8388608 | 98.235 | 97.350 | 22.497 | 0.231 | 12.707 | 0.131 | 40.026 | 0.411 | 19.513 | 0.200 |

**Performance Data Analysis**:

- Compared to Case 0 standard division version, Case 1 `Task Duration` decreases from 110.167μs to 98.235μs, `aiv_time` decreases from 109.110μs to 97.350μs, overall execution duration decreases.
- `aiv_vec_time` decreases from 40.685μs to 22.497μs, indicating that computation instruction duration significantly decreases after replacing standard division with multiplication and shift.
- `aiv_scalar_time` increases from Case 0's 10.701μs to 12.707μs, related to the additional scalar operations for computing `magic` and `shift` in the fast division scenario; however, overall `Task Duration` still decreases, indicating that the benefits from fast division can cover this overhead.

---

## Performance Comparison Summary

### Ascend 950PR Performance Data

**Comprehensive Optimization Effect**:

- Through the fast division optimization from Case 0 to Case 1, the example `Task Duration` decreases from 110.167μs to 98.235μs, a duration reduction of approximately 10.8%.
- Case 1 relative to Case 0 performance improvement is approximately 1.12x, indicating that in fixed-divisor scenarios, using multiplication and shift to replace standard division can reduce end-to-end duration.

| Case | Implementation | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---|:---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Case 0 | Standard Division | 110.167 | 109.110 | 40.685 | 0.373 | 10.701 | 0.098 | 36.229 | 0.332 | 18.678 | 0.171 |
| Case 1 | Fast Division | 98.235 | 97.350 | 22.497 | 0.231 | 12.707 | 0.131 | 40.026 | 0.411 | 19.513 | 0.200 |

### Optimization Summary

| Optimization Method | Core Principle | Example Demonstration |
|:---|:---|:---|
| Fixed divisor precomputation | When the divisor is fixed during kernel function execution, `magic` and `shift` can be precomputed, and SIMT threads reuse the result for fast division. | Case 1 calls `calc_magic_shift()` in the kernel function, and SIMT threads no longer execute `/` directly. |
| Multiplication and shift replacing standard division | Standard integer division instructions have high overhead; using multiplication and shift operations reduces per-element computation overhead. | Case 1 compared to Case 0, `Task Duration` decreases from 110.167μs to 98.235μs. |

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Switch Case

  Specify the Case to build through `-DSCENARIO_NUM=N` during cmake build. Case descriptions:

  - `0`: Standard division
  - `1`: Fast division

  Example:

  ```bash
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1
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
  SCENARIO_NUM=0                                                           # Select execution scenario
  mkdir -p build && cd build;                                          # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # Build the project
  ./integer_div                                                        # Run the example
  ```

- Build option description

  | Option | Values | Description |
  |:---|:---|:---|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture, corresponding to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `0`, `1` | Case number: 0=Standard division version, 1=Fast division version |

- Execution results

  The following execution result indicates that the accuracy comparison succeeded.

  ```text
  [Success] Case accuracy is verification passed.
  ```

### Performance Analysis

Use the `msprof` tool to obtain detailed performance data.

```bash
msprof ./integer_div   # Analyze performance
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
