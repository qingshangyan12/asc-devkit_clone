# UB Out-of-Bounds Troubleshooting Sample for SIMD and SIMT Hybrid Programming

## Overview

This sample uses an output accuracy issue caused by a UB out-of-bounds access in SIMD and SIMT hybrid programming to demonstrate how UB capacity, memory layout, and actual access ranges can be used to locate and fix the invalid UB access. It then uses multiple Cases to explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the runtime and accuracy results of out-of-bounds accesses to different UB regions.

The main content of this sample is listed below.

| Topic | Case | Main Content |
| --- | --- | --- |
| Troubleshooting | Case 0 | Start from an output accuracy issue, locate the UB out-of-bounds write caused by complex workspace offset calculation, and show the fix |
| UB capacity boundary | Case 1 | Explain usable UB limits under different VF-call, reserved-UB, and Data Cache configurations |
| UB layout relationship | Case 2 | Explain how static UB declarations at different execution locations jointly occupy the static memory region and how dynamic UB is shared |
| Out-of-bounds result | Case 3 | Explain runtime and accuracy results when writes go beyond the allocated memory range into following UB regions |

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
|-- simd_simt_ub_usage_limit
    |-- CMakeLists.txt         // Example build script
    |-- ub_usage_limit_common.h// Common declarations and helper functions
    |-- case0_ub_oob.asc       // Case 0 implementation
    |-- case1_ub_limit.asc     // Case 1 implementation
    |-- case2_ub_layout.asc    // Case 2 implementation
    |-- case3_oob_result.asc   // Case 3 implementation
    |-- figures                // Figures referenced by the README files
    |-- README.md              // Chinese documentation
    `-- README_en.md           // English documentation
```

## Example Description

This sample contains four Cases covering UB out-of-bounds troubleshooting, capacity boundaries, and memory layout.

**Case description**:

| Case | Scenario | Main Content |
| --- | --- | --- |
| Case 0 | Static UB out-of-bounds access caused by complex workspace offset calculation | Present the accuracy issue, then locate and fix the out-of-bounds write from the UB layout and access offset |
| Case 1 | Usable UB limits in different VF scenarios | Determine usable UB limits under different VF-call, reserved-UB, and Data Cache configurations |
| Case 2 | Static and Dynamic UB Layout Across Execution Locations | Explain the static and dynamic UB layout relationship across execution locations |
| Case 3 | Static UB array writes out of bounds into different regions | Explain runtime and accuracy results when writes go beyond the allocated memory range into following UB regions |

**Example specifications**:

<table>
<tr><td align="center">Example Type (OpType)</td><td colspan="4" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="2" align="center">Example Input</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0, Case 3</td><td align="center">input</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td rowspan="2" align="center">Example Output</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0, Case 3</td><td align="center">output</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Kernel Name</td><td colspan="4" align="center">case0_static_ub_oob_kernel/case3_static_ub_oob_kernel</td></tr>
</table>

Cases 1 and 2 validate UB capacity and layout rather than operator input and output, so they are not listed separately in the example specifications.

## Example Implementation

The Cases follow an issue presentation and fix, UB-limit determination, memory-layout explanation, and out-of-bounds result analysis sequence.

### Case 0: Static UB Out-of-Bounds Access Caused by Complex Workspace Offset Calculation

**Objective**:

Present a practical operator-style issue where complex workspace offset calculation causes the kernel to report no runtime error but fail output accuracy verification. Locate the cause by checking UB capacity, memory layout, and actual access ranges, and verify the fix.

**Core implementation**:

Case 0 implements a simplified compressed-tile processing flow. The kernel divides the static UB workspace into four reusable blocks. Each tile uses one block to store `value`, `score`, and intermediate results. A SIMT VF first loads GM input into the workspace block of the current tile and generates intermediate results. MainScalar then calculates a write offset in the `score` region from `tile_id` and `lane_id`. Finally, another SIMT VF continues to read GM input and the `score` data from the workspace to generate output.

```text
output[i] = input[i] + score[i]
```

The kernel allocates 216 KB of static UB as the workspace. Different stages calculate workspace offsets from fixed base offsets, `tile_id`, and `lane_id`. In the faulty version, the workspace block size calculation mistakenly includes the 8 KB reserved region as usable workspace. As a result, the MainScalar write to the `score` region goes beyond the declared static UB range.

The key code is as follows:

```cpp
__ubuf__ uint32_t workspace[216 * 1024 / sizeof(uint32_t)];

asc_vf_call<simt_case0_prepare_workspace>(dim3(256), input, workspace, tile_id);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
asc_vf_call<simt_case0_update_result>(dim3(128), workspace, tile_id);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);

uint32_t block_id = tile_id & 3;
uint32_t score_word_offset = block_id * (56 * 1024 / sizeof(uint32_t)) +
                             (56 * 1024 / sizeof(uint32_t)) + lane_id;
workspace[score_word_offset] = CORRUPTED_VALUE;

asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID0);
asc_vf_call<simt_case0_store_compressed_tile>(dim3(THREAD_COUNT), input, workspace, output,
                                              BUFFER_WORDS, tile_id);
```

**Execution result**:

After the offset calculation writes beyond static UB and reaches Data Cache, the kernel reports no runtime error, but output accuracy verification fails.

```text
[Case0] mode=fault, faulty_byte_offset=229376
[Case0] first_mismatch_index=0, expected=0xc0000000, actual=0xdeadbeef
[Case0] precision_check=FAILED (fault reproduced)
```

**Troubleshooting**:

1. Confirm the symptom first. The faulty version does not report a kernel launch or synchronization failure. The abnormal behavior appears in the output data, so the next step is to inspect the UB access range inside the kernel.
2. Count UB usage. Case 0 uses the default compilation configuration, 216 KB of static UB, and zero dynamic UB. The remaining space contains an 8 KB reserved region and 32 KB of Data Cache. Data Cache is SIMT-specific, caches GM data accessed by SIMT threads, and ranges from 32 KB to 128 KB.

    ```text
    Low address 0
    ├── Static UB        216 KB
    ├── Reserved region    8 KB
    └── Data Cache        32 KB
    High address 256 KB
    ```

3. Calculate the Data Cache location. Its size is `min(256 KB - 216 KB - 0 KB - 8 KB, 128 KB) = 32 KB`, so it starts at `256 KB - 32 KB = 224 KB`.
4. Inspect the workspace offset calculation. The code mistakenly treats `216 KB static UB + 8 KB reserved region` as usable workspace and divides it into four workspace blocks. Each block is `(216 KB + 8 KB) / 4 = 56 KB`. When `tile_id = 3` and `lane_id = 0`, the target byte offset is:

    ```text
    block_id = tile_id & 3 = 3
    score_word_offset = block_id * 56 KB / 4 B + 56 KB / 4 B + lane_id
                      = 57344
    byte_offset = score_word_offset * 4 B = 229376 B = 224 KB
    ```

    This location is outside the declared 216 KB static workspace and is exactly the start of Data Cache.
5. Check the mismatched value. The earlier SIMT VF stage reads GM input and populates Data Cache, and the out-of-bounds write then corrupts the cached data. When the later SIMT VF continues to read the same input and generate output using the `score` data in the workspace, the incorrect output is `0xDEADBEEF`, the same as the out-of-bounds write value. Combining the fault offset and the mismatched value locates the issue to an out-of-bounds write into Data Cache.

**Solution**:

- Check the calculated access range before writing the workspace. Perform the write only when `score_word_offset` is less than the static workspace element count `CASE0_WORKSPACE_WORDS`, preventing access beyond the declared range of `workspace`.

    ```cpp
    if (score_word_offset < CASE0_WORKSPACE_WORDS) {
        workspace[score_word_offset] = CORRUPTED_VALUE;
    }
    ```

- After the index check is added, the out-of-bounds write is blocked and output accuracy verification passes:

    ```text
    [Case0] mode=fixed, faulty_byte_offset=229376
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

**Conclusion**:

The fixed mode passes its accuracy check. A UB out-of-bounds access may not cause a kernel launch or runtime error. When it reaches Data Cache, it can corrupt data cached for SIMT GM access and appear as an accuracy error. Troubleshooting and fixing the issue requires checking not only total UB allocation, but also the actual access range calculated from workspace blocks.

Case 0 shows that UB out-of-bounds access caused by complex workspace offset calculation may appear as an accuracy failure. Further diagnosis requires understanding UB capacity and layout in mixed programming: MainScalar, SIMD VFs, and SIMT VFs can declare static or dynamic UB; static UB declarations at different execution locations jointly occupy the static memory region, while dynamic UB declarations share the space configured at kernel launch; VF calls also affect reserved space and UB limits. Cases 1 through 3 explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the results of out-of-bounds writes into different regions.

### Case 1: Usable UB Limits in Different VF Scenarios

**Objective**:

Determine usable UB limits under different VF-call and reserved-UB configurations.

**Core implementation**:

This Case covers three scenarios: no VF call, a SIMD VF call, and a SIMT VF call. When a VF is called, it verifies the usable UB limit under the default configuration, with the VF reserve disabled, and with all reserved UB disabled.

Each Vector Core has 256 KB of UB. When a VF is called, the default configuration reserves 8 KB of UB (6 KB for the VF reserve and 2 KB for Ascend C APIs). A scenario without a VF call is not calculated with this reservation. According to the [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制), Data Cache is SIMT-specific, caches GM data accessed by SIMT threads, and ranges from 32 KB to 128 KB. In SIMD and SIMT hybrid programming, part of UB must be allocated as Data Cache. Therefore, the usable UB limit in a SIMT VF scenario is obtained by reserving at least 32 KB for Data Cache and then calculating the maximum space available to static and dynamic UB. The actual Data Cache size is calculated from the remaining UB space after subtracting static memory, dynamic memory, and reserved UB.

| Scenario | VF Call | Data Cache Constraint | Default (8 KB Reserved) | VF Reservation Disabled (2 KB Reserved) | All Reserved UB Disabled (0 KB Reserved) |
| --- | --- | ---: | ---: | ---: | ---: |
| 1 | No VF call | None | 256 KB | - | - |
| 2 | SIMD VF | None | 248 KB | 254 KB | 256 KB |
| 3 | SIMT VF | At least 32 KB | 216 KB | 222 KB | 224 KB |

This Case declares a static UB array named `ub_buffer` with size `UB_LIMIT_BYTES` and uses it to check whether that UB range is available under the current configuration. Depending on the scenario, MainScalar, a SIMD VF, or a SIMT VF writes `ub_buffer`. The kernel writes two elements from the array back to GM so that the Host can confirm that the write result is correct at the limit configuration:

```cpp
__ubuf__ uint32_t ub_buffer[UB_LIMIT_WORDS];

#if defined(UB_LIMIT_WITH_SIMT_VF)
// Scenario 3: call a SIMT VF to write ub_buffer.
asc_vf_call<simt_fill_ub_limit>(dim3(THREAD_COUNT), ub_buffer, UB_LIMIT_WORDS, CASE1_MARKER);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
#elif defined(UB_LIMIT_NO_SIMD_VF)
// Scenario 1: no VF call; MainScalar writes ub_buffer.
for (uint32_t index = 0; index < UB_LIMIT_WORDS; ++index) {
    ub_buffer[index] = CASE1_MARKER + index;
}
#else
// Scenario 2: call a SIMD VF to write ub_buffer.
asc_vf_call<simd_fill_ub_limit>(ub_buffer, UB_LIMIT_WORDS);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
#endif

// Write back two elements so that the Host can confirm the write result.
output[0] = ub_buffer[0];
output[1] = ub_buffer[UB_LIMIT_WORDS - 1];
```

**Execution result**:

This Case first allocates `ub_buffer` at each usable UB limit in the table above, and then allocates one more `uint32_t` element (4 bytes). The former launches and runs successfully. The latter still compiles, but kernel launch fails at runtime.

| Scenario | VF Call | Result at Limit | Result After Adding 4 B Beyond the Limit |
| --- | --- | --- | --- |
| 1 | No VF call | Passed | Kernel launch failed |
| 2 | SIMD VF | All configurations passed | Kernel launch failed for all configurations |
| 3 | SIMT VF | All configurations passed | Kernel launch failed for all configurations |

When a VF is called under the default configuration, the 6 KB VF reserve and the 2 KB Ascend C API reserve are excluded from usable UB. `--cce-disable-vf-stack-reserved-ubuf` releases the 6 KB VF reserve; after enabling it, the kernel should not depend on this reserved region. `--cce-disable-asc-reserved-ubuf` releases the other 2 KB reserved for [selected Ascend C APIs](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api); after enabling it, APIs that depend on this region are unavailable and cause a compilation error when called. See the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) for the restrictions of both options.

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
__ubuf__ uint32_t main_static_ub[CASE2_MAIN_STATIC_WORDS];
// SIMD VF
extern __ubuf__ uint32_t simd_dynamic_ub[];
__ubuf__ uint32_t simd_static_ub[CASE2_SIMD_STATIC_WORDS];
// SIMT VF
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t simt_static_ub[CASE2_SIMT_STATIC_WORDS];
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

Starting from a static UB array, access its valid range, unallocated UB, the reserved region, and Data Cache. Determine whether each access is out of bounds, and observe whether runtime errors or accuracy errors occur.

**Core implementation**:

Case 3 uses the default compilation configuration, declares static UB, and configures zero dynamic UB. In this configuration, Data Cache reaches its 128 KB maximum and occupies the high-address half of UB. According to the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) and [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md), an 8 KB reserved region is placed before Data Cache. The resulting layout is:

```text
Data Cache size = min(256 KB - 4 KB - 0 KB - 8 KB, 128 KB) = 128 KB
Data Cache start = 256 KB - 128 KB = 128 KB
Reserved-region start = 128 KB - 8 KB = 120 KB
Unallocated UB size = 120 KB - 4 KB = 116 KB
```

![Case 3 UB layout and access positions](./figures/case3_ub_layout_en.png "Case 3 UB layout and access positions")

Only the first 4 KB is valid for the static array. The sample defines an access offset for each of five target regions. The Host converts the byte offset to an index and passes it to the kernel, which writes to the selected region through a `volatile` pointer and a runtime index:

| Target Region | Offset From the Static Array Start |
| --- | ---: |
| Last `uint32_t` element inside static UB | 4092 B |
| Middle of unallocated ordinary UB | 63488 B |
| Middle of the 6 KB VF reserved range | 125952 B |
| Middle of the 2 KB Ascend C API reserved range | 130048 B |
| Start of Data Cache | 131072 B |

The kernel first calls a SIMT VF to read GM input and write element-wise add-one output, populating Data Cache. MainScalar then performs the selected write through the start of the 4 KB static array and a runtime index. The same SIMT VF reads GM and computes again. The Host checks kernel launch and synchronization, then compares the second output with Golden data to determine whether output accuracy is affected:

```cpp
__ubuf__ uint32_t static_ub[1024];
__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[access_index] = 0xDEADBEEF;
```

**Execution result**:

The results for the five access configurations are as follows:

| Target Region | Launch/Sync | Accuracy | Result Description |
| --- | --- | --- | --- |
| Valid static UB | Succeeded | Passed | Valid access |
| Unallocated ordinary UB | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| 6 KB VF reserved range | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| 2 KB Ascend C API reserved range | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| Data Cache | Succeeded | Failed | Cached data was corrupted without a runtime error |

Key output from all five targets follows, showing the observed result for each target region:

```text
# Valid static UB
[Case3] region=static_ub, access_offset_bytes=4092
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# Unallocated ordinary UB
[Case3] region=unallocated_ub, access_offset_bytes=63488
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# 6 KB VF reserved range
[Case3] region=vf_reserved, access_offset_bytes=125952
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# 2 KB Ascend C API reserved range
[Case3] region=asc_reserved, access_offset_bytes=130048
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# Data Cache
[Case3] region=data_cache, access_offset_bytes=131072
[Case3] first_mismatch_index=896, expected=0xc0030381, actual=0xdeadbef0
[Case3] precision_check=FAILED
[Case3] runtime_check=PASSED
```

**Analysis**:

Passing the accuracy check does not make the unallocated or reserved ranges valid to access. The [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制) states that an access beyond an allocated static or dynamic UB range may not immediately cause an allocation failure, but it can corrupt Data Cache and lead to unstable results or accuracy errors. The table is only an observation from this run and must not be used as evidence that an out-of-bounds access is valid or that the target region is unused.

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

Use a separate build directory for each scenario so that cached CMake options from a previous build do not affect the current scenario.

| Option | Valid Value | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. This sample supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
| `SCENARIO_NUM` | `0`-`3` | Case number. The default value is `1`. CMake compiles only the Ascend C file of the selected Case |
| `CASE0_ENABLE_FIX` | `ON`, `OFF` | Whether Case 0 checks the runtime static-array index. The default is `OFF` |
| `UB_LIMIT_BYTES` | Positive multiple of 4 B | Case 1 static UB array size in bytes. If omitted, the default is `253952` with a SIMD VF and `221184` with a SIMT VF |
| `UB_LIMIT_WITH_SIMT_VF` | `ON`, `OFF` | Whether Case 1 includes a SIMT VF. The default is `OFF` |
| `UB_LIMIT_NO_SIMD_VF` | `ON`, `OFF` | Whether the non-SIMT scenario omits the SIMD VF call. The default is `OFF` |
| `UB_LIMIT_DISABLE_VF_RESERVED` | `ON`, `OFF` | Whether to pass `--cce-disable-vf-stack-reserved-ubuf`. The default is `OFF` |
| `UB_LIMIT_DISABLE_ASC_RESERVED` | `ON`, `OFF` | Whether to pass `--cce-disable-asc-reserved-ubuf`. The default is `OFF` |
| `CASE3_ACCESS_REGION` | `0`-`4` | Case 3 target. Default: `0`. `0`: static UB; `1`: unallocated UB; `2`: 6 KB VF reserve; `3`: 2 KB Ascend C API reserve; `4`: Data Cache |

The Case 1 base scenarios and compiler-option combinations are listed below:

| Scenario | Additional CMake Options |
| --- | --- |
| Scenario 1: No VF call | `-DUB_LIMIT_NO_SIMD_VF=ON -DUB_LIMIT_BYTES=262144` |
| Scenario 2: SIMD VF | `-DUB_LIMIT_BYTES=253952` |
| Scenario 3: SIMT VF | `-DUB_LIMIT_WITH_SIMT_VF=ON -DUB_LIMIT_BYTES=221184` |
| SIMD VF, VF reservation disabled | `-DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_BYTES=260096` |
| SIMD VF, all reserved UB disabled | `-DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_DISABLE_ASC_RESERVED=ON -DUB_LIMIT_BYTES=262144` |
| SIMT VF, VF reservation disabled | `-DUB_LIMIT_WITH_SIMT_VF=ON -DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_BYTES=227328` |
| SIMT VF, all reserved UB disabled | `-DUB_LIMIT_WITH_SIMT_VF=ON -DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_DISABLE_ASC_RESERVED=ON -DUB_LIMIT_BYTES=229376` |

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

**Execution result examples**:

- Case 0 uses the default `CASE0_ENABLE_FIX=OFF`. Key output:

    ```text
    [Case0] precision_check=FAILED (fault reproduced)
    # Accuracy issue observed
    ```

- Case 1 calls a SIMD VF by default and uses a 253952-byte `ub_buffer`. Key output:

    ```text
    [Case1] mode=with SIMD VF, ub_limit_bytes=253952
    [Case1] result=PASSED
    ```

- Case 1 with a SIMT VF and 221184 bytes of static UB produces the following key output:

    ```text
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184
    [Case1] result=PASSED
    ```

- Key output for Case 2:

    ```text
    [Case2] static_total_bytes=7168
    [Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
    [Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
    [Case2] result=PASSED
    ```

- Case 3 uses the default `CASE3_ACCESS_REGION=0`. Key output:

    ```text
    [Case3] region=static_ub, access_offset_bytes=4092
    [Case3] precision_check=PASSED
    [Case3] runtime_check=PASSED
    ```

**Boundary and error result examples**:

- After disabling the 6 KB VF reservation and the 2 KB Ascend C API reservation in Case 1, a 262144-byte `ub_buffer` uses all 256 KB and passes:

    ```text
    [Case1] mode=with SIMD VF, ub_limit_bytes=262144
    [Case1] result=PASSED
    ```

- In Case 1 with the default SIMD VF configuration, increasing `ub_buffer` from 253952 to 253956 bytes makes total UB usage exceed the 248 KB limit by 4 bytes, and kernel launch fails:

    ```text
    [Case1] mode=with SIMD VF, ub_limit_bytes=253956
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED
    ```

- In Case 1 with a SIMT VF, increasing the static UB array from 221184 to 221188 bytes makes total UB usage exceed the 216 KB limit by 4 bytes, and kernel launch fails:

    ```text
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184, configured_ub_bytes=221188
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED
    ```

- In Case 3, set `CASE3_ACCESS_REGION=4` to write out of bounds into Data Cache. Kernel launch and synchronization succeed, but the output has an accuracy error:

    ```text
    [Case3] region=data_cache, access_offset_bytes=131072
    [Case3] precision_check=FAILED
    [Case3] runtime_check=PASSED
    ```
