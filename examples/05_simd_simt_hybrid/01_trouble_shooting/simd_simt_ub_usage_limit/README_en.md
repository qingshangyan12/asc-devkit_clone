# UB Out-of-Bounds Troubleshooting Sample for SIMD and SIMT Hybrid Programming

## Overview

This sample uses an output accuracy issue caused by a UB out-of-bounds access in SIMD and SIMT hybrid programming to demonstrate how UB capacity, memory layout, and actual access ranges can be used to locate and fix the invalid UB access. It then uses multiple Cases to explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the runtime and accuracy results of out-of-bounds accesses to different UB regions.

The main content of this sample is listed below.

| Topic | Case | Main Content |
| --- | --- | --- |
| Troubleshooting | Case 0 | Start from an output accuracy issue, locate the UB out-of-bounds write, and show the fix |
| UB capacity boundary | Case 1 | Explain usable UB limits under different VF-call, reserved-UB, and Data Cache configurations |
| UB layout relationship | Case 2 | Explain how static UB declarations at different execution locations jointly occupy the static memory region and how dynamic UB is shared |
| Out-of-bounds result | Case 3 | Explain runtime and accuracy results when writes go beyond the allocated memory range into following UB regions |

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Versions

- \>= CANN 9.1.0

## Directory Structure

```text
|-- simd_simt_ub_usage_limit
    |-- CMakeLists.txt         // Example build script
    |-- ub_usage_limit_common.h// Common declarations and helper functions
    |-- case0_ub_out_of_bounds.asc       // Case 0 implementation
    |-- case1_ub_limit.asc     // Case 1 implementation
    |-- case2_ub_layout.asc    // Case 2 implementation
    |-- case3_out_of_bounds_result.asc   // Case 3 implementation
    |-- figures                // Figures referenced by the README files
    |-- README.md              // Chinese documentation
    `-- README_en.md           // English documentation
```

## Example Description

This sample contains four Cases covering UB out-of-bounds troubleshooting, capacity boundaries, and memory layout.

**Case description**:

| Case | Scenario | Main Content |
| --- | --- | --- |
| Case 0 | UB out-of-bounds access | Present the accuracy issue, then locate and fix the out-of-bounds write from the UB layout and access offset |
| Case 1 | Usable UB limits in different VF scenarios | Determine usable UB limits under different VF-call, reserved-UB, and Data Cache configurations |
| Case 2 | Static and Dynamic UB Layout Across Execution Locations | Explain the static and dynamic UB layout relationship across execution locations |
| Case 3 | Static UB array writes out of bounds into different regions | Explain runtime and accuracy results when writes go beyond the allocated memory range into following UB regions |

**Example specifications**:

<table>
<tr><td align="center">Example Type (OpType)</td><td colspan="4" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="2" align="center">Example Input</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0, Case 3</td><td align="center">input</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Example Output</td><td align="center">Case 0, Case 3</td><td align="center">output</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td rowspan="4" align="center">Kernel Name</td><td align="center">Case 0</td><td colspan="3" align="center"><code>static_ub_out_of_bounds_kernel</code></td></tr>
<tr><td align="center">Case 1</td><td colspan="3" align="center"><code>ub_limit_kernel</code> / <code>simt_ub_limit_kernel</code></td></tr>
<tr><td align="center">Case 2</td><td colspan="3" align="center"><code>ub_layout_kernel</code></td></tr>
<tr><td align="center">Case 3</td><td colspan="3" align="center"><code>static_ub_out_of_bounds_kernel</code></td></tr>
</table>

Cases 1 and 2 validate UB capacity and layout rather than operator input and output, so they are not listed separately in the example input and output.

## Example Implementation

The Cases follow an issue presentation and fix, UB-limit determination, memory-layout explanation, and out-of-bounds result analysis sequence.

### Case 0: UB Out-of-Bounds Access

**Objective**:

Present an issue in a block-wise data calibration flow where an incorrect static UB offset causes the kernel to report no runtime error but fail output accuracy verification. Locate the cause by checking UB capacity, memory layout, and actual access ranges, and verify the fix.

**Core implementation**:

Case 0 implements a simplified data calibration flow. The `input` is the data to be calibrated. The kernel generates the calibration bias `bias` from the lower 8 bits of `input`, and writes `input + bias` to `output`:

```text
bias[i] = input[i] & 0xff
output[i] = input[i] + bias[i]
```

The kernel uses a 216 KB static UB array as temporary storage for block-wise computation and divides it into four regions by `TILE_COUNT`; `TILE_WORDS` is the number of `uint32_t` elements in each region. During processing, `tile_id` selects the current region. The SIMT VF stores `input` moved from GM and `bias` generated from `input` in that region. The kernel then calculates `bias_word_offset` from `tile_id` and `lane_id`, and writes data to the selected `bias` location. Finally, the SIMT VF reads `input` and `bias` from the same region, computes `output`, and writes it back to GM. The static UB regions and the out-of-bounds write position in Case 0 are shown below:

![Case 0 static UB regions and out-of-bounds write](./figures/case0_static_ub_layout_en.png "Case 0 static UB regions and out-of-bounds write")

The faulty code includes the 8 KB reserved UB in the usable static UB range when calculating the tile stride. As a result, `bias_word_offset` goes beyond the declared 216 KB static UB range and reaches Data Cache.

The key code is as follows:

```cpp
__simt_vf__ inline void simt_prepare_input_ub(
    __gm__ const uint32_t* input, __ubuf__ uint32_t* static_ub, uint32_t tile_id)
{
    const uint32_t tile_base = (tile_id & (TILE_COUNT - 1)) * TILE_WORDS;
    ...
    static_ub[tile_base + INPUT_OFFSET + lane] = input[lane];
    static_ub[tile_base + BIAS_OFFSET + lane] = input[lane] & 0xff;
}

__simt_vf__ inline void simt_store_calibrated_output(
    __gm__ const uint32_t* input, __ubuf__ uint32_t* static_ub, __gm__ uint32_t* output,
    uint32_t word_count, uint32_t tile_id)
{
    const uint32_t tile_base = (tile_id & (TILE_COUNT - 1)) * TILE_WORDS;
    ...
    const uint32_t bias = static_ub[tile_base + BIAS_OFFSET + index];
    output[index] = input[index] + bias;
}

__aicore__ inline uint32_t calc_bias_word_offset(uint32_t tile_id, uint32_t lane_id)
{
    const uint32_t tile_stride_words =
        (UB_BYTES + RESERVED_UB_BYTES) / TILE_COUNT / sizeof(uint32_t);
    return (tile_id & (TILE_COUNT - 1)) * tile_stride_words + tile_stride_words + lane_id;
}

__global__ __vector__ void static_ub_out_of_bounds_kernel(...)
{
    __ubuf__ uint32_t static_ub[216 * 1024 / sizeof(uint32_t)];
    uint8_t mutex_id = 0;

    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simt_prepare_input_ub>(dim3(256), input, static_ub, tile_id);
    asc_unlock(PIPE_V, mutex_id);

    asc_lock(PIPE_S, mutex_id);
    const uint32_t bias_word_offset = calc_bias_word_offset(tile_id, lane_id);
    static_ub[bias_word_offset] = CORRUPTED_VALUE;
    asc_unlock(PIPE_S, mutex_id);

    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simt_store_calibrated_output>(
        dim3(THREAD_COUNT), input, static_ub, output, BUFFER_WORDS, tile_id);
    asc_unlock(PIPE_V, mutex_id);
}
```

**Execution result**:

After the offset calculation writes beyond static UB and reaches Data Cache, the kernel reports no runtime error, but output accuracy verification fails.

```text
[Case0] mode=baseline, access_byte_offset=229376
[Case0] corrupted_output: expected=0xc0000000, actual=0xdeadbeef
[Case0] precision_check=FAILED (fault reproduced)
```

**Troubleshooting**:

Accuracy issues are usually located in the following order: execution status, calculation logic, and memory corruption.

1. Check whether the kernel executes normally. Configure the Runtime/ACL log file path and set whether logs are printed to stdout as needed. Then check the log files for typical kernel launch, synchronization, or AICore execution errors such as `Ascend Error`, `EZ9999`, `vector core exception`, and `ASSERT`. The log path must point to an existing directory. For log environment variables, see the [Ascend log example](../../../01_simd_cpp_api/01_utilities/09_log/README.md).

    ```bash
    export ASCEND_PROCESS_LOG_PATH=./log       # Set the Runtime/ACL log path.
    export ASCEND_SLOG_PRINT_TO_STDOUT=0       # Control whether logs are printed to stdout.
    export ASCEND_GLOBAL_LOG_LEVEL=3           # Print only ERROR-level Runtime/ACL logs.
    ./ub_usage_limit                           # Run the Case 0 baseline scenario.
    ```

    After execution, Runtime/ACL log files are generated under `./log`. In this scenario, the log files do not show kernel launch, synchronization, or AICore execution errors, so the accuracy issue shown above is not directly caused by kernel execution failure. The next step is to check why the output data does not match the expected result.

2. Check the calculation logic inside the kernel. First check whether the Host-side Golden formula matches the kernel output formula. The Host initializes input as `INPUT_BASE_VALUE + index` and calculates Golden as `input + (input & 0xff)`:

    ```cpp
    input[index] = INPUT_BASE_VALUE + index;
    const uint32_t expected = input + (input & 0xff);
    ```

    Then check how the kernel generates `bias` and writes the final output. The SIMT VF writes `bias` as `input & 0xff`, and the final output is written as `input + bias`:

    ```cpp
    static_ub[tile_base + BIAS_OFFSET + lane] = input[lane] & 0xff;
    const uint32_t bias = static_ub[tile_base + BIAS_OFFSET + index];
    output[index] = input[index] + bias;
    ```

    The Host-side Golden formula and the kernel output formula are consistent, and no obvious issue is found in the input data or calculation flow. If the output is still incorrect, the next step is to check whether memory has been overwritten and data has been corrupted.

3. Inspect memory accesses that may corrupt data. The incorrect output is `0xDEADBEEF`, which is the same value written into the static UB array by the kernel. Therefore, focus on the runtime offset used to write the storage region for `bias`. The offset calculation treats `216 KB static UB + 8 KB reserved region` as usable static UB space and calculates offsets as four tile regions. Each region is `(216 KB + 8 KB) / 4 = 56 KB`. When `tile_id = 3` and `lane_id = 0`, the target byte offset is:

    ```text
    block_id = tile_id & 3 = 3
    bias_word_offset = block_id * 56 KB / 4 B + 56 KB / 4 B + lane_id
                     = 57344
    byte_offset = bias_word_offset * 4 B = 229376 B = 224 KB
    ```

4. Confirm the target region from the UB layout. Case 0 does not add compiler options that disable reserved UB, uses 216 KB of static UB, and configures zero dynamic UB. The remaining space contains an 8 KB reserved region and 32 KB of Data Cache. Data Cache is SIMT-specific, caches GM data accessed by SIMT threads, and ranges from 32 KB to 128 KB.

    ```text
    Low address 0
    ├── Static UB        216 KB
    ├── Reserved region    8 KB
    └── Data Cache        32 KB
    High address 256 KB
    ```

    The Data Cache size is `min(256 KB - 216 KB - 0 KB - 8 KB, 128 KB) = 32 KB`, so it starts at `256 KB - 32 KB = 224 KB`. The 224 KB byte offset calculated above is outside the declared 216 KB static UB array and is exactly the start of Data Cache. The earlier SIMT VF stage reads GM input data and populates Data Cache, and the out-of-bounds write then corrupts the cached data. The later SIMT VF continues to read the same input data and generate `output` using `bias` in the static UB array, so an accuracy error appears.

**Solution**:

- Check the calculated access range before writing the static UB array. Perform the write only when `bias_word_offset` is less than the static UB array element count `UB_WORDS`, preventing access beyond the declared range.

    ```cpp
    if (bias_word_offset < UB_WORDS) {
        static_ub[bias_word_offset] = CORRUPTED_VALUE;
    }
    ```

- After the index check is added, the out-of-bounds write is blocked and output accuracy verification passes:

    ```text
    [Case0] mode=fixed, access_byte_offset=229376
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

**Conclusion**:

The fixed mode passes its accuracy check. A UB out-of-bounds access may not cause a kernel launch or runtime error. When it reaches Data Cache, it can corrupt data cached for SIMT GM access and appear as an accuracy error. Troubleshooting and fixing the issue requires checking not only total UB allocation, but also the actual access range calculated from runtime offsets.

Case 0 shows that a UB out-of-bounds access may appear as an accuracy failure. Mixed programming involves different execution locations such as MainScalar, SIMD VFs, and SIMT VFs. UB is used both for static or dynamic memory allocated at these locations and for data communication between VFs and MainScalar. To help developers avoid out-of-bounds UB access, Cases 1 through 3 explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the results of out-of-bounds writes into different regions.

### Case 1: Usable UB Limits in Different VF Scenarios

**Objective**:

Determine usable UB limits under different VF-call and reserved-UB configurations.

**Core implementation**:

This Case covers three mutually exclusive paths: no VF call, only a SIMD VF call, and only a SIMT VF call. When a VF is called, it verifies the usable UB limit under the default configuration, with the VF reserve disabled, and with all reserved UB disabled.

Each Vector Core has 256 KB of UB. When a VF is called, the default configuration reserves 8 KB of UB (6 KB for the VF reserve and 2 KB for Ascend C APIs). A scenario without a VF call is not calculated with this reservation. `--cce-disable-vf-stack-reserved-ubuf` disables the 6 KB VF reserve, and `--cce-disable-asc-reserved-ubuf` disables the 2 KB Ascend C API reserve. According to the [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制), Data Cache is SIMT-specific, caches GM data accessed by SIMT threads, and ranges from 32 KB to 128 KB. In SIMD and SIMT hybrid programming, part of UB must be allocated as Data Cache. Therefore, the usable UB limit in the SIMT-VF-only scenario is obtained by reserving at least 32 KB for Data Cache and then calculating the maximum space available to static and dynamic UB. The actual Data Cache size is calculated from the remaining UB space after subtracting static memory, dynamic memory, and reserved UB.

| Scenario | VF Call | Data Cache Constraint | Usable UB in Default Configuration (8 KB Reserved) | Usable UB with VF Reserve Disabled (2 KB Reserved) | Usable UB with All Reserved UB Disabled (0 KB Reserved) |
| --- | --- | ---: | ---: | ---: | ---: |
| 1 | No VF call | None | 256 KB | - | - |
| 2 | SIMD VF only | None | 248 KB | 254 KB | 256 KB |
| 3 | SIMT VF only | At least 32 KB | 216 KB | 222 KB | 224 KB |

CMake calculates the usable UB size from the VF-call path and reserved-UB configuration. This Case declares a static UB array named `static_ub` with that size and uses it to check whether the UB range is available under the current configuration. Depending on the scenario, one of MainScalar, a SIMD VF, or a SIMT VF writes `static_ub`. The kernel writes two elements from the array back to GM so that the Host can confirm that the write result is correct at the limit configuration:

```cpp
#if !defined(CASE1_USE_SIMT_VF)
__global__ __vector__ void ub_limit_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    uint8_t mutex_id = 0;

#if defined(CASE1_NO_VF)
    // Scenario 1: no VF call; MainScalar writes static_ub.
    for (uint32_t index = 0; index < UB_WORDS; ++index) {
        static_ub[index] = FILL_VALUE_BASE + index;
    }
#else
    // Scenario 2: call only a SIMD VF to write static_ub.
    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simd_fill_static_ub>(static_ub, UB_WORDS);
    asc_unlock(PIPE_V, mutex_id);
    asc_lock(PIPE_S, mutex_id);
#endif

    // Write back two elements so that the Host can confirm the write result.
    output[0] = static_ub[0];
    output[1] = static_ub[UB_WORDS - 1];
#if !defined(CASE1_NO_VF)
    asc_unlock(PIPE_S, mutex_id);
#endif
}
#endif

#if defined(CASE1_USE_SIMT_VF)
__global__ __vector__ void simt_ub_limit_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    uint8_t mutex_id = 0;

    // Scenario 3: call only a SIMT VF to write static_ub.
    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simt_fill_static_ub>(dim3(THREAD_COUNT), static_ub, UB_WORDS, FILL_VALUE_BASE);
    asc_unlock(PIPE_V, mutex_id);

    asc_lock(PIPE_S, mutex_id);
    output[0] = static_ub[0];
    output[1] = static_ub[UB_WORDS - 1];
    asc_unlock(PIPE_S, mutex_id);
}
#endif
```

**Execution result**:

This Case first allocates `static_ub` at each usable UB limit in the table above, and then allocates one more `uint32_t` element (4 bytes). The former launches and runs successfully. The latter still compiles, but kernel launch fails at runtime.

| Scenario | VF Call | Result at Limit | Result After Adding 4 B Beyond the Limit |
| --- | --- | --- | --- |
| 1 | No VF call | Passed | Kernel launch failed |
| 2 | SIMD VF only | All configurations passed | Kernel launch failed for all configurations |
| 3 | SIMT VF only | All configurations passed | Kernel launch failed for all configurations |

When a VF is called under the default configuration, the 6 KB VF reserve and the 2 KB Ascend C API reserve are excluded from usable UB. After a reserved region is disabled, the kernel should not depend on the released reserved space. [Selected Ascend C APIs](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api) that depend on the 2 KB reserved region are unavailable and cause a compilation error when called. See the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) for the restrictions of both options.

**Conclusion**:

VF calls and reserved UB affect the usable UB limit. In SIMD and SIMT hybrid programming, enough space must also be reserved for Data Cache. This limit constrains the sum of static and dynamic UB usage in a kernel, regardless of which UB type this Case uses for boundary validation. When applying this method to an actual kernel, count all UB declarations, check whether the kernel contains VF calls, and confirm whether it uses Ascend C APIs that depend on the 2 KB reserved region.

### Case 2: Static and Dynamic UB Layout Across Execution Locations

**Objective**:

Explain that static UB declarations in MainScalar, a SIMD VF, and a SIMT VF jointly occupy the current kernel's static memory region and must be counted according to the actual layout. Also explain that the three differently named dynamic UB declarations have the same start address and share the region configured at kernel launch.

**Core implementation**:

Case 2 declares static UB separately in MainScalar, a SIMD VF, and a SIMT VF. According to the [SIMD and SIMT hybrid programming memory hierarchy](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md#ub划分), static memory can be allocated in MainScalar, SIMD VF, and SIMT VF in mixed programming. Multiple static memory allocations are laid out in UB according to start-address alignment rules, and dynamic memory follows static memory. MainScalar declares a 1 KB `main_static_ub` array, the SIMD VF declares 2 KB, and the SIMT VF declares 4 KB. In this Case, all three declarations satisfy 32-byte alignment, so the static memory region occupies 7168 bytes.

MainScalar, the SIMD VF, and the SIMT VF each access their static UB array so that these static UB declarations participate in the current kernel layout. The following code shows the static and dynamic UB declarations in MainScalar, the SIMD VF, and the SIMT VF:

```cpp
// MainScalar
extern __ubuf__ uint32_t main_dynamic_ub[];
__ubuf__ uint32_t main_static_ub[MAIN_STATIC_WORDS];
// SIMD VF
extern __ubuf__ uint32_t simd_dynamic_ub[];
__ubuf__ uint32_t simd_static_ub[SIMD_STATIC_WORDS];
// SIMT VF
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t simt_static_ub[SIMT_STATIC_WORDS];
```

MainScalar, the SIMD VF, and the SIMT VF convert the start addresses of their static and dynamic UB arrays to integers and write them back to GM for the Host to print and compare. The printed addresses belong to the AICore UB address space and are used only for the layout comparison in this Case.

**Execution result**:

The expected static UB start addresses in MainScalar, the SIMD VF, and the SIMT VF are `0x0`, `0x400`, and `0xc00`, respectively. The static memory region occupies 7168 bytes, which is `0x1c00` in hexadecimal. Dynamic UB follows static memory, so its expected start address is `0x1c00`. The result is as follows:

```text
[Case2] static_total_bytes=7168
[Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
[Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
[Case2] result=PASSED
```

The static UB start addresses in MainScalar, the SIMD VF, and the SIMT VF accumulate according to the declaration sizes. The dynamic UB start address is `0x1c00` in all three locations, matching the address corresponding to the static memory region in this Case.

**Conclusion**:

Static UB declarations in MainScalar, the SIMD VF, and the SIMT VF jointly occupy the current kernel's static memory region. Count this usage according to the actual layout and alignment rules. Dynamic UB follows static memory. The three dynamic arrays have different names but the same start address and share the region configured by the Host at kernel launch.

### Case 3: Static UB Array Writes Out of Bounds Into Different Regions

**Objective**:

Starting from a static UB array, access its valid range, unoccupied ordinary UB space, reserved space, and Data Cache. Determine whether each access is out of bounds and whether it causes a runtime error or an accuracy error.

**Core implementation**:

This Case reuses the data calibration calculation from Case 0, whose output formula is `input + (input & 0xff)`. The kernel first runs the calculation so that the GM input enters Data Cache. It then writes to the target UB region using the runtime index passed from the Host. Finally, it runs the same calculation again. The Host uses the kernel launch/synchronization result and the second calculation output to determine whether the access causes a runtime error or an accuracy error.

Case 3 does not add compiler options that disable reserved UB, declares a 4 KB static UB array named `static_ub`, and configures zero dynamic UB. In this configuration, Data Cache reaches its 128 KB maximum and occupies the high-address half of UB. According to the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) and [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md), an 8 KB reserved region is placed before Data Cache. The resulting layout is:

```text
Data Cache size = min(256 KB - 4 KB - 0 KB - 8 KB, 128 KB) = 128 KB
Data Cache start = 256 KB - 128 KB = 128 KB
Reserved-region start = 128 KB - 8 KB = 120 KB
Unoccupied ordinary UB space size = 120 KB - 4 KB = 116 KB
```

Only the first 4 KB is valid for the static array. The sample defines an access offset for each of five target regions. The Host converts the byte offset to an index and passes it to the kernel:

| Target Region | Offset From the Static Array Start |
| --- | ---: |
| Last `uint32_t` element inside static UB | 4092 B |
| Middle of unoccupied ordinary UB space | 63488 B |
| Middle of the 6 KB VF reserved range | 125952 B |
| Middle of the 2 KB Ascend C API reserved range | 130048 B |
| Start of Data Cache | 131072 B |

The kernel writes to the selected target region by using the runtime index. The key code is as follows:

```cpp
__ubuf__ uint32_t static_ub[1024];
__ubuf__ volatile uint32_t* static_ub_ptr = static_ub;
static_ub_ptr[access_index] = 0xDEADBEEF;
```

**Execution result**:

The results for the five access configurations are as follows:

| Target Region | Launch/Sync | Accuracy | Result Description |
| --- | --- | --- | --- |
| Valid static UB | Succeeded | Passed | Valid access |
| Unoccupied ordinary UB space | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| 6 KB VF reserved range | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| 2 KB Ascend C API reserved range | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| Data Cache | Succeeded | Failed | Cached data was corrupted without a runtime error |

Key output from all five targets follows, showing the observed result for each target region:

```text
# Valid static UB
[Case3] region=static_ub, access_offset_bytes=4092
[Case3] precision_check=PASSED

# Unoccupied ordinary UB space
[Case3] region=unallocated_ub, access_offset_bytes=63488
[Case3] precision_check=PASSED

# 6 KB VF reserved range
[Case3] region=vf_reserved, access_offset_bytes=125952
[Case3] precision_check=PASSED

# 2 KB Ascend C API reserved range
[Case3] region=asc_reserved, access_offset_bytes=130048
[Case3] precision_check=PASSED

# Data Cache
[Case3] region=data_cache, access_offset_bytes=131072
[Case3] corrupted_output: expected=0xc0030000, actual=0xdeadbfde
[Case3] precision_check=FAILED
```

**Analysis**:

Passing the accuracy check only means that no accuracy error is observed in this run. It does not make unoccupied ordinary UB space or reserved space valid to access. The [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制) states that an access beyond an allocated static or dynamic UB range may not immediately cause an allocation failure, but it can corrupt Data Cache and lead to unstable results or accuracy errors. The table is only an observation from this run and must not be used as evidence that an out-of-bounds access is valid or that the target region is unused.

**Conclusion**:

In Case 3, only the access within the valid static UB range is in bounds. The other four scenarios all cause static-array out-of-bounds accesses. Such accesses may not cause a visible failure in the current run, but any access outside the declared static array remains invalid.

## Build and Run

Run the following steps in the sample root directory to build and execute the sample.

- Configure environment variables.

    Configure the environment according to the [CANN development kit installation method](../../../../docs/zh/quick_start.md#prepare&install).

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

  > **Note**: `${install_path}` is the CANN package installation directory. If no installation directory is specified, the default path is `/usr/local/Ascend`.

- Build and run the sample.

    ```bash
    SCENARIO_NUM=1                              # Select a Case: 0, 1, 2, or 3
    mkdir -p build && cd build                  # Create and enter the build directory
    cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
      -DSCENARIO_NUM=${SCENARIO_NUM} ..         # Configure; append Case-specific options as needed
    make -j                                     # Build the project
    ./ub_usage_limit                            # Run the sample
    ```

**Build options**:

Use a separate build directory for each scenario so that cached CMake options from a previous build do not affect the current scenario. In this sample, the default configuration means that no compiler options are added to disable reserved UB. The sample uses separate Ascend C files for different Cases. Except for `SCENARIO_NUM`, the other options switch comparison scenarios inside a single Case. For Case 1, select the scenario with `CASE1_VF_MODE` and `CASE1_RESERVED_MODE`; CMake calculates the corresponding UB size automatically.

| Option | Valid Value | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. This sample supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
| `SCENARIO_NUM` | `0`-`3` | Case number. The default value is `1`. CMake compiles only the Ascend C file of the selected Case |
| `CASE0_ENABLE_FIX` | `ON`, `OFF` | Whether Case 0 checks the runtime static-array index. The default is `OFF` |
| `CASE1_VF_MODE` | `none`, `simd`, `simt` | VF-call path for Case 1. The default is `simd` |
| `CASE1_RESERVED_MODE` | `default`, `disable_vf`, `disable_all` | Reserved-UB mode for Case 1. The default is `default`. `disable_vf` adds the compiler option `--cce-disable-vf-stack-reserved-ubuf`; `disable_all` adds both `--cce-disable-vf-stack-reserved-ubuf` and `--cce-disable-asc-reserved-ubuf` |
| `CASE1_OVER_LIMIT` | `ON`, `OFF` | Whether Case 1 allocates 4 B more than the usable UB limit. The default is `OFF` |
| `CASE3_ACCESS_REGION` | `0`-`4` | Case 3 target. Default: `0`. `0`: static UB; `1`: unoccupied ordinary UB space; `2`: 6 KB VF reserve; `3`: 2 KB Ascend C API reserve; `4`: Data Cache |

By default, Case 0 runs the scenario in which a static UB out-of-bounds write reaches Data Cache:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCASE0_ENABLE_FIX=OFF ..
make -j
./ub_usage_limit
```

Add `CASE0_ENABLE_FIX=ON` to enable the index check and build the fixed scenario:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCASE0_ENABLE_FIX=ON ..
make -j
./ub_usage_limit
```

The Case 1 compiler-option combinations are listed below:

| VF Call | Reserved-UB Configuration | Additional CMake Options |
| --- | --- | --- |
| No VF call | Default configuration | `-DCASE1_VF_MODE=none` |
| SIMD VF only | Default configuration | `-DCASE1_VF_MODE=simd -DCASE1_RESERVED_MODE=default` |
| SIMD VF only | VF reservation disabled | `-DCASE1_VF_MODE=simd -DCASE1_RESERVED_MODE=disable_vf` |
| SIMD VF only | All reserved UB disabled | `-DCASE1_VF_MODE=simd -DCASE1_RESERVED_MODE=disable_all` |
| SIMT VF only | Default configuration | `-DCASE1_VF_MODE=simt -DCASE1_RESERVED_MODE=default` |
| SIMT VF only | VF reservation disabled | `-DCASE1_VF_MODE=simt -DCASE1_RESERVED_MODE=disable_vf` |
| SIMT VF only | All reserved UB disabled | `-DCASE1_VF_MODE=simt -DCASE1_RESERVED_MODE=disable_all` |

For example, to build and run Case 1 when only calling a SIMT VF with all reserved UB disabled, use the following commands:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=1 \
  -DCASE1_VF_MODE=simt \
  -DCASE1_RESERVED_MODE=disable_all ..
make -j
./ub_usage_limit
```

**Execution result examples**:

The following examples show key output for each Case. Case 1 and Case 3 contain multiple scenarios, and comments in the code blocks indicate the corresponding scenario.

- Case 0 contains the issue scenario and the fixed scenario. Key output:

    ```text
    # Issue scenario: CASE0_ENABLE_FIX=OFF, out-of-bounds write into Data Cache
    [Case0] mode=baseline, access_byte_offset=229376
    [Case0] corrupted_output: expected=0xc0000000, actual=0xdeadbeef
    [Case0] precision_check=FAILED (fault reproduced)

    # Fixed scenario: CASE0_ENABLE_FIX=ON, runtime index check enabled
    [Case0] mode=fixed, access_byte_offset=229376
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

- Case 1 allocates `static_ub` at the usable UB limit. Key output for each scenario:

    ```text
    # Scenario 1: No VF call, usable UB limit is 262144 B
    [Case1] mode=without SIMD VF, ub_limit_bytes=262144
    [Case1] result=PASSED

    # Scenario 2: SIMD VF only, default configuration, usable UB limit is 253952 B
    [Case1] mode=with SIMD VF, ub_limit_bytes=253952
    [Case1] result=PASSED

    # Scenario 2: SIMD VF only, VF reservation disabled, usable UB limit is 260096 B
    [Case1] mode=with SIMD VF, ub_limit_bytes=260096
    [Case1] result=PASSED

    # Scenario 2: SIMD VF only, all reserved UB disabled, usable UB limit is 262144 B
    [Case1] mode=with SIMD VF, ub_limit_bytes=262144
    [Case1] result=PASSED

    # Scenario 3: SIMT VF only, default configuration, usable UB limit is 221184 B
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184
    [Case1] result=PASSED

    # Scenario 3: SIMT VF only, VF reservation disabled, usable UB limit is 227328 B
    [Case1] mode=with SIMT VF, ub_limit_bytes=227328
    [Case1] result=PASSED

    # Scenario 3: SIMT VF only, all reserved UB disabled, usable UB limit is 229376 B
    [Case1] mode=with SIMT VF, ub_limit_bytes=229376
    [Case1] result=PASSED
    ```

    Add `-DCASE1_OVER_LIMIT=ON` to make CMake allocate 4 B more than the usable UB limit for `static_ub`. In the default configuration, this causes kernel launch failure:

    ```text
    # SIMD VF only: 253952 B + 4 B
    [Case1] mode=with SIMD VF, ub_limit_bytes=253956
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED

    # SIMT VF only: 221184 B + 4 B
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184, configured_ub_bytes=221188
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED
    ```

- Key output for Case 2:

    ```text
    [Case2] static_total_bytes=7168
    [Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
    [Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
    [Case2] result=PASSED
    ```

- Case 3 accesses different UB regions. Key output:

    ```text
    # Valid static UB range
    [Case3] region=static_ub, access_offset_bytes=4092
    [Case3] precision_check=PASSED

    # Unallocated normal UB range
    [Case3] region=unallocated_ub, access_offset_bytes=63488
    [Case3] precision_check=PASSED

    # 6 KB VF reserved range
    [Case3] region=vf_reserved, access_offset_bytes=125952
    [Case3] precision_check=PASSED

    # 2 KB Ascend C API reserved range
    [Case3] region=asc_reserved, access_offset_bytes=130048
    [Case3] precision_check=PASSED

    # Data Cache
    [Case3] region=data_cache, access_offset_bytes=131072
    [Case3] corrupted_output: expected=0xc0030000, actual=0xdeadbfde
    [Case3] precision_check=FAILED
    ```
