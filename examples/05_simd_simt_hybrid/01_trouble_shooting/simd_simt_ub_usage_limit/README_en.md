# UB Out-of-Bounds Troubleshooting Sample for SIMD and SIMT Hybrid Programming

## Overview

This sample uses an output accuracy issue caused by a UB out-of-bounds access in SIMD and SIMT hybrid programming to demonstrate how UB capacity, memory layout, and actual access ranges can be used to locate and fix the invalid UB access. It then uses multiple Cases to explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the runtime and accuracy results of out-of-bounds accesses to different UB regions.

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
| Case 0 | Output accuracy troubleshooting | Present the output accuracy issue, locate the cause by checking execution status, calculation logic, and access ranges, and verify the fix |
| Case 1 | Usable UB limits in different VF scenarios | Determine usable UB limits under different VF-call, reserved-UB, and Data Cache configurations |
| Case 2 | Static and Dynamic UB Layout Across Execution Locations | Explain the static and dynamic UB layout relationship across execution locations |
| Case 3 | Static UB array writes out of bounds into different regions | Explain runtime and accuracy results when writes go beyond the allocated memory range into following UB regions |

**Example specifications**:

<table>
<tr><td align="center">Example Type (OpType)</td><td colspan="3" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">input</td><td align="center">[10240]/[12800]/[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Example Output</td><td align="center">output</td><td align="center">[10240]/[12800]/[1024]</td><td align="center">uint32_t</td></tr>
<tr><td rowspan="4" align="center">Kernel Name</td><td align="center">Case 0</td><td colspan="2" align="center"><code>static_ub_out_of_bounds_kernel</code> / <code>static_ub_bias_offset_fixed_kernel</code></td></tr>
<tr><td align="center">Case 1</td><td colspan="2" align="center"><code>ub_limit_main_scalar_kernel</code> / <code>ub_limit_simd_vf_kernel</code> / <code>ub_limit_simt_vf_kernel</code> / <code>ub_limit_simd_simt_vf_kernel</code></td></tr>
<tr><td align="center">Case 2</td><td colspan="2" align="center"><code>ub_layout_kernel</code></td></tr>
<tr><td align="center">Case 3</td><td colspan="2" align="center"><code>ub_out_of_bounds_result_kernel</code></td></tr>
</table>

Case 0 can use `[10240]` or `[12800]` input and output. Case 3 uses `[1024]` input and output. Cases 1 and 2 verify UB capacity and layout and do not perform GM input/output data checks.

## Example Implementation

The Cases follow an issue presentation and fix, UB-limit determination, memory-layout explanation, and out-of-bounds result analysis sequence.

### Case 0: Output Accuracy Troubleshooting

**Objective**:

Present an issue in a data calibration flow where the kernel reports no runtime error but fails output accuracy verification. Locate the cause by checking execution status, calculation logic, and actual access ranges.

**Core implementation**:

Case 0 implements a simplified calibration calculation for input data. The `input` is the data to be calibrated. The kernel generates the calibration value `bias` from the lower 8 bits of `input` and adds it to the original input to obtain the calibrated output data `output`. The final calculation is:

```text
bias[i] = input[i] & 0xff
output[i] = input[i] + bias[i]
```

The kernel splits the input into 10 KB data tiles and divides static UB into four UB tiles to store `input` and `bias` during calibration. For each data tile, the SIMT VF calculates the storage position in static UB based on the data tile ID and moves `input` from GM. The kernel then generates the corresponding `bias`. Finally, the SIMT VF reads `input` and `bias`, generates `output`, and writes it back to GM.

The static UB tiles and data layout in Case 0 are shown below:

![Case 0 static UB tiles and data layout](./figures/case0_static_ub_layout_en.png "Case 0 static UB tiles and data layout")

In this Case, `tile_id` indicates the data tile currently processed by the kernel. The key code is as follows:

```cpp
// In the SIMT scenario, 216 KB of static UB is available after subtracting
// 8 KB of reserved UB and 32 KB of Data Cache from the 256 KB UB.
constexpr uint32_t UB_BYTES = 216 * 1024;
constexpr uint32_t UB_WORDS = UB_BYTES / sizeof(uint32_t);
constexpr uint32_t UB_TILE_COUNT = 4;
constexpr uint32_t UB_TILE_WORDS = UB_WORDS / UB_TILE_COUNT;
constexpr uint32_t DATA_TILE_WORDS = 2560; // Each data tile has 2560 uint32_t elements, that is, 10 KB.
constexpr uint32_t BIAS_OFFSET = DATA_TILE_WORDS;

__aicore__ inline uint32_t calc_bias_base_words(uint32_t ub_tile_id)
{
    return ub_tile_id * UB_TILE_WORDS + BIAS_OFFSET;
}

__aicore__ inline void generate_bias_values(__ubuf__ uint32_t* static_ub, uint32_t ub_tile_id)
{
    const uint32_t tile_base_words = ub_tile_id * UB_TILE_WORDS;
    const uint32_t bias_base_words = calc_bias_base_words(ub_tile_id);
    for (uint32_t index = 0; index < DATA_TILE_WORDS; ++index) {
        const uint32_t input = static_ub[tile_base_words + index];
        static_ub[bias_base_words + index] = input & 0xff;
    }
}

__global__ __vector__ void static_ub_out_of_bounds_kernel(
    __gm__ const uint32_t* input, __gm__ uint32_t* output, uint32_t input_words)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    uint8_t mutex_id = 0;

    const uint32_t input_tile_count = input_words / DATA_TILE_WORDS;
    for (uint32_t tile_id = 0; tile_id < input_tile_count; ++tile_id) {
        asc_lock(PIPE_V, mutex_id);
        asc_vf_call<simt_prepare_input_ub>(dim3(THREAD_COUNT), input, static_ub, tile_id);
        asc_unlock(PIPE_V, mutex_id);

        asc_lock(PIPE_S, mutex_id);
        generate_bias_values(static_ub, tile_id);
        asc_unlock(PIPE_S, mutex_id);

        ...
    }
}
```

**Execution result**:

When the input size is 40 KB, the kernel processes four data tiles and prints the following information:

```text
test pass!
```

When the input size is 50 KB, the kernel processes five data tiles and output accuracy verification fails:

```text
[Failed] Case accuracy verification failed!
```

**Troubleshooting**:

The following troubleshooting process focuses on the accuracy issue that occurs with a 50 KB input size. Accuracy issues are usually located in the following order: execution status, calculation logic, and memory corruption.

1. Check whether the kernel executes normally. Configure the log file path and set whether logs are printed to stdout as needed. Then check the log files for typical kernel launch, synchronization, or AICore execution errors such as `Ascend Error`, `EZ9999`, `vector core exception`, and `ASSERT`. The log path must point to an existing directory. For log environment variables, see the [Ascend log example](../../../01_simd_cpp_api/01_utilities/09_log/README.md).

    ```bash
    export ASCEND_PROCESS_LOG_PATH=./log       # Set the log path.
    export ASCEND_SLOG_PRINT_TO_STDOUT=0       # Control whether logs are printed to stdout.
    ./ub_usage_limit 12800                     # Run the scenario with a 50 KB input size.
    ```

    After execution, log files are generated under `./log`. In this scenario, the log files do not show kernel launch, synchronization, or AICore execution errors, so the accuracy issue shown above is not directly caused by kernel execution failure. The next step is to check why the output data does not match the expected result.

2. Check the calculation logic inside the kernel. First check whether the Host-side Golden formula matches the kernel output formula. The Host initializes input by global element index and calculates Golden as `input + (input & 0xff)`, which matches the calibration calculation in the kernel:

    ```cpp
    input[gm_base + index] = INPUT_BASE_VALUE + gm_base + index;
    uint32_t expected = input + (input & 0xff);
    ```

    Then check how the kernel generates `bias` and writes the final output. The kernel writes `bias` as `input & 0xff`. In the final output stage, it reads `input` from GM again and writes `input + bias` back to GM:

    ```cpp
    static_ub[bias_base_words + index] = input & 0xff;
    const uint32_t bias = static_ub[tile_base + BIAS_OFFSET + index];
    output[gm_base + index] = input + bias;
    ```

    The Host-side Golden and the kernel calculation intent are consistent, and no obvious issue is found in the input data or calculation flow. If the output is still incorrect, the next step is to check whether memory has been overwritten and data has been corrupted.

3. Print intermediate calculation variables. The previous checks show that the kernel formula itself is consistent, but the output is still incorrect. Therefore, temporary prints can be added at intermediate calculation positions to observe related runtime variables and calculation results.

    For example, add the following print before calculating the `bias` access position. `bias_base_words` indicates the start element index of the `bias` region calculated from the current `tile_id`:

    ```cpp
    const uint32_t bias_base_words = calc_bias_base_words(tile_id);
    printf("tile_id=%u, bias_base_words=%u\n", tile_id, bias_base_words);
    ```

    Printed values are as follows:

    ```text
    tile_id=4, bias_base_words=57856
    ```

    The printed values show that when the kernel reaches `tile_id = 4`, the current implementation still directly uses the data tile ID to calculate the `bias` access position. The first four data tiles use the four UB tiles respectively, so the issue is not exposed. When the input size reaches 50 KB, the fifth data tile should map the data tile ID back to UB tile 0, but the current implementation continues to calculate based on UB tile 4. The converted target start byte offset is:

    ```text
    Target start byte offset = bias_base_words * 4 B = 57856 * 4 B = 231424 B = 226 KB
    ```

    After the target start byte offset is obtained, compare it with the UB layout in this Case to determine the actual access range corresponding to 226 KB.

4. Confirm the actual access range from the UB layout. Case 0 uses 216 KB of static UB and configures zero dynamic UB. By default, the remaining space contains an 8 KB reserved region and 32 KB of Data Cache. Data Cache is SIMT-specific, caches GM data accessed by SIMT threads, and ranges from 32 KB to 128 KB.

    ![Case 0 actual UB access range](./figures/case0_access_range_en.png "Case 0 actual UB access range")

    The Data Cache size is `min(256 KB - 216 KB - 0 KB - 8 KB, 128 KB) = 32 KB`, so it starts at `256 KB - 32 KB = 224 KB`. The 226 KB byte offset calculated above is outside the declared 216 KB static UB array and enters the Data Cache range. This access may corrupt data cached for SIMT GM access. When the later SIMT VF continues to read GM input and calculate output, an accuracy error may appear.

**Solution**:

- Map the data tile ID `tile_id` to a UB tile ID before accessing static UB to avoid accessing a nonexistent UB tile with the data tile ID.

    ```cpp
    const uint32_t ub_tile_id = calc_ub_tile_id(tile_id);
    generate_bias_values(static_ub, ub_tile_id);
    ```

- After the `tile_id` mapping is fixed, the intermediate access is inside the declared static UB range and output accuracy verification passes:

    ```text
    [Case0] mode=fixed
    test pass!
    ```

**Conclusion**:

When a UB out-of-bounds access occurs, it may not cause a kernel launch or runtime error, and the logs may not contain explicit exception information. If the out-of-bounds access reaches the Data Cache region, it can corrupt data cached for SIMT GM access and appear as an accuracy error. Therefore, when locating an accuracy issue caused by UB out-of-bounds access, check not only the log result but also the calculation logic and the actual access range calculated from runtime parameters.

Case 0 shows that a UB out-of-bounds access may appear as an accuracy failure. Mixed programming involves different execution locations such as MainScalar, SIMD VFs, and SIMT VFs. UB is used both for static or dynamic memory allocated at these locations and for data communication between VFs and MainScalar. To help developers avoid out-of-bounds UB access, Cases 1 through 3 explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the results of out-of-bounds writes into different regions.

### Case 1: Usable UB Limits in Different VF Scenarios

**Objective**:

Determine the maximum usable UB limits under different VF-call and reserved-UB configurations.

**Core implementation**:

This Case covers four paths: no VF call, SIMD VF only, SIMT VF only, and both SIMD VF and SIMT VF. When a VF is called, it verifies the maximum usable UB limit under the default configuration, with the VF reserve disabled, and with all reserved UB disabled.

Each Vector Core has 256 KB of UB. When a VF is called, the default configuration reserves 8 KB of UB (6 KB for the VF reserve and 2 KB for Ascend C APIs). A scenario without a VF call is not calculated with this reservation. `--cce-disable-vf-stack-reserved-ubuf` disables the 6 KB VF reserve, and `--cce-disable-asc-reserved-ubuf` disables the 2 KB Ascend C API reserve. According to the [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制), Data Cache is SIMT-specific, caches GM data accessed by SIMT threads, and ranges from 32 KB to 128 KB. When the hybrid programming scenario contains a SIMT VF, part of UB must be allocated as Data Cache. Therefore, the maximum space available to static and dynamic UB is obtained by reserving at least 32 KB for Data Cache. The actual Data Cache size is calculated from the remaining UB space after subtracting static memory, dynamic memory, and reserved UB.

| Scenario | VF Call | Data Cache Constraint | Usable UB in Default Configuration (8 KB Reserved) | Usable UB with VF Reserve Disabled (2 KB Reserved) | Usable UB with All Reserved UB Disabled (0 KB Reserved) |
| --- | --- | ---: | ---: | ---: | ---: |
| 1 | No VF call | None | Max. 256 KB | - | - |
| 2 | SIMD VF only | None | Max. 248 KB | Max. 254 KB | Max. 256 KB |
| 3 | SIMT VF only | At least 32 KB | Max. 216 KB | Max. 222 KB | Max. 224 KB |
| 4 | Both SIMD VF and SIMT VF | At least 32 KB | Max. 216 KB | Max. 222 KB | Max. 224 KB |

CMake passes the reserved-UB and Data Cache space selected by the VF-call path and reserved-UB configuration. The Ascend C code calculates the maximum usable UB size from the UB total size, reserved UB, and Data Cache space, and then declares a static UB array named `static_ub` with that size. Different paths use separate kernels and write `static_ub` from MainScalar, a SIMD VF, a SIMT VF, or both SIMD VF and SIMT VF. The kernel accesses the tail element of `static_ub` and writes it to one GM location so that the limit position participates in the current kernel access. The Host determines whether the current UB size exceeds the limit based on the kernel launch/synchronization result and error information:

```cpp
// CASE1_VF_RESERVED_BYTES: 0 or 6KB; CASE1_ASC_RESERVED_BYTES: 0 or 2KB;
// CASE1_DATA_CACHE_BYTES: 0 or 32KB; CASE1_EXTRA_BYTES is 4B when checking the over-limit result.
constexpr uint32_t UB_BYTES = UB_TOTAL_BYTES - CASE1_VF_RESERVED_BYTES -
                              CASE1_ASC_RESERVED_BYTES - CASE1_DATA_CACHE_BYTES +
                              CASE1_EXTRA_BYTES;
constexpr uint32_t UB_WORDS = UB_BYTES / sizeof(uint32_t);

// Scenario 1: no VF call.
__global__ __vector__ void ub_limit_main_scalar_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    ...
    output[0] = static_ub[UB_WORDS - 1];
}

// Scenario 2: SIMD VF only.
__global__ __vector__ void ub_limit_simd_vf_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    ...
    asc_vf_call<simd_fill_static_ub>(static_ub, UB_WORDS);
    ...
    output[0] = static_ub[UB_WORDS - 1];
}

// Scenario 3: SIMT VF only.
__global__ __vector__ void ub_limit_simt_vf_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    ...
    asc_vf_call<simt_fill_static_ub>(dim3(THREAD_COUNT), static_ub, UB_WORDS, FILL_VALUE_BASE);
    ...
    output[0] = static_ub[UB_WORDS - 1];
}

// Scenario 4: both SIMD VF and SIMT VF.
__global__ __vector__ void ub_limit_simd_simt_vf_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    ...
    asc_vf_call<simd_fill_static_ub>(static_ub, UB_WORDS);
    asc_vf_call<simt_fill_static_ub>(dim3(THREAD_COUNT), static_ub, UB_WORDS, FILL_VALUE_BASE);
    ...
    output[0] = static_ub[UB_WORDS - 1];
}
```

**Execution result**:

This Case first allocates `static_ub` at the theoretical usable UB limit of the current scenario to confirm that the kernel can launch and run successfully. It then increases the `static_ub` allocation by 4 bytes, that is, by one more `uint32_t` element, to observe the runtime result after exceeding the UB limit. In this case, compilation still succeeds, but kernel launch fails and the error indicates that the shared memory size exceeds the limit.

| Scenario | VF Call | Result at Limit | Result After Adding 4 B Beyond the Limit |
| --- | --- | --- | --- |
| 1 | No VF call | Passed | Kernel launch failed |
| 2 | SIMD VF only | All configurations passed | Kernel launch failed for all configurations |
| 3 | SIMT VF only | All configurations passed | Kernel launch failed for all configurations |
| 4 | Both SIMD VF and SIMT VF | All configurations passed | Kernel launch failed for all configurations |

When the limit configuration runs successfully, the output is as follows:

```text
[Case1] result=PASSED
```

After increasing the `static_ub` allocation from the theoretical usable UB limit of the current scenario to "theoretical usable UB limit + 4 B", kernel launch fails and prints the following error:

```text
[Host] kernel error: ... the sum of dynamic shared memory and kernel shared memory exceeds the maximum limit.
[ERROR] [Bisheng] Launch kernel failure! ret 107000
```

**Conclusion**:

In SIMD and SIMT hybrid programming, whether a VF is called, whether a SIMT VF is included, and whether reserved UB is disabled affect the usable UB limit. When a VF is called, the default configuration subtracts the 6 KB VF reserve and the 2 KB Ascend C API reserve. After the corresponding reserved space is disabled by compiler options, the usable UB limit is recalculated with the released space. When a SIMT VF is included, UB space must also be reserved for Data Cache. In actual development, determine the usable UB limit of the current kernel based on these factors.

> **Note:** After [--cce-disable-vf-stack-reserved-ubuf](../../../../docs/zh/guide/编程指南/编译与运行/算子编译/AI-Core算子编译基本用法.md#ZH-CN_TOPIC_0000002462746461) is used, the reserved 6 KB SIMD VF stack space can be used as normal UB space, but the compiler can no longer use this space to cache register spill data. Developers need to ensure that register spills do not occur. After [--cce-disable-asc-reserved-ubuf](../../../../docs/zh/guide/编程指南/编译与运行/算子编译/AI-Core算子编译基本用法.md#ZH-CN_TOPIC_0000002462746461) is used, the 2 KB UB space originally reserved for [selected Ascend C APIs](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api) can be used as normal UB space, but these APIs become unavailable and cause a compilation error when called. See the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) for the restrictions of both options.

### Case 2: Static and Dynamic UB Layout Across Execution Locations

**Objective**:

Explain that static UB declarations in MainScalar, a SIMD VF, and a SIMT VF jointly occupy the current kernel's static memory region and must be counted together. Also explain that the three differently named dynamic UB declarations have the same start address and share the region configured at kernel launch.

**Core implementation**:

Case 2 declares static UB separately in MainScalar, a SIMD VF, and a SIMT VF. According to the [SIMD and SIMT hybrid programming memory hierarchy](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md#ub划分), static memory can be allocated in MainScalar, SIMD VF, and SIMT VF in mixed programming, and dynamic memory follows static memory. For the actual static UB layout, see that document. MainScalar declares a 1 KB `main_static_ub` array, the SIMD VF declares 2 KB, and the SIMT VF declares 4 KB. In this Case, all three declarations satisfy the default requirements, so the static memory region occupies 7168 bytes.

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

The static UB start addresses in MainScalar, the SIMD VF, and the SIMT VF are `0x0`, `0x400`, and `0xc00`, respectively. The static memory region occupies 7168 bytes, which is `0x1c00` in hexadecimal. Dynamic UB follows static memory, so its start address is `0x1c00`. The result is as follows:

```text
[Case2] static_total_bytes=7168
[Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
[Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
[Case2] result=PASSED
```

In this Case, the three static UB start addresses accumulate according to the declaration sizes. The dynamic UB start address is `0x1c00` in all three locations, matching the address corresponding to the static memory region in this Case.

**Conclusion**:

Static UB declarations in MainScalar, the SIMD VF, and the SIMT VF jointly occupy the current kernel's static memory region and must be counted together. Dynamic UB follows static memory. The three dynamic arrays have different names but the same start address and share the region configured at kernel launch. For more UB layout information, see [SIMD and SIMT hybrid programming memory hierarchy](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md#ub划分).

### Case 3: Static UB Array Writes Out of Bounds Into Different Regions

**Objective**:

Starting from a static UB array, access its valid range, unoccupied ordinary UB space, reserved space, and Data Cache. Determine whether each access is out of bounds and whether it causes a runtime error or an accuracy error.

**Core implementation**:

This Case uses a simplified data calibration calculation, whose output formula is `input + (input & 0xff)`. The kernel first runs the calculation so that the GM input enters Data Cache. It then writes to the target UB region using the runtime index passed from the Host. Finally, it runs the same calculation again. The Host uses the kernel launch/synchronization result and the second calculation output to determine whether the access causes a runtime error or an accuracy error.

Case 3 does not add compiler options that disable reserved UB, declares a 4 KB static UB array named `static_ub`, and configures zero dynamic UB. In this configuration, Data Cache reaches its 128 KB maximum and occupies the high-address half of UB. According to the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) and [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md), an 8 KB reserved region is placed before Data Cache. The resulting layout is:

```text
Data Cache size = min(256 KB - 4 KB - 0 KB - 8 KB, 128 KB) = 128 KB
Data Cache start = 256 KB - 128 KB = 128 KB
Reserved-region start = 128 KB - 8 KB = 120 KB
Unoccupied ordinary UB space size = 120 KB - 4 KB = 116 KB
```

Only the first 4 KB is valid for the static array. The sample defines an access offset for each of five target regions. The Host converts the byte offset to an index and passes it to the kernel:

![Case 3 UB layout and access positions](./figures/case3_ub_layout_en.png "Case 3 UB layout and access positions")

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

When accessing the valid static UB range, unoccupied ordinary UB space, the 6 KB VF reserved range, and the 2 KB Ascend C API reserved range, no accuracy error is observed, and the log prints the following information:

```text
test pass!
```

When accessing Data Cache, an accuracy error is observed, and the log prints the following information:

```text
[Failed] Case accuracy verification failed!
```

**Analysis**:

Passing the accuracy check only means that no accuracy error is observed in this run. It does not mean that regions after the allocated static UB can be accessed out of bounds. The [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制) states that an access beyond an allocated static or dynamic UB range may not immediately cause an allocation failure, but it can corrupt Data Cache and lead to unstable results or accuracy errors. The table only shows the observed results of different UB out-of-bounds locations in this run and must not be used as evidence that an out-of-bounds access is valid or that the target region is unused.

**Conclusion**:

UB out-of-bounds access to unoccupied ordinary UB space or reserved space is undefined behavior and may show no visible failure in this run, while out-of-bounds access to Data Cache may appear as an accuracy error.

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

Use a separate build directory for each scenario so that cached CMake options from a previous build do not affect the current scenario. In this sample, the default configuration means that no compiler options are added to disable reserved UB. The sample uses separate Ascend C files for different Cases. Except for `SCENARIO_NUM`, the other options switch comparison scenarios inside a single Case. For Case 1, select the scenario with `CASE1_VF_MODE` and `CASE1_RESERVED_MODE`; CMake passes the corresponding reserved-UB and Data Cache space, and the Ascend C code calculates the UB size.

| Option | Valid Value | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. This sample supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
| `SCENARIO_NUM` | `0`-`3` | Case number. The default value is `1`. CMake compiles only the Ascend C file of the selected Case |
| `CASE0_ENABLE_FIX` | `ON`, `OFF` | Whether Case 0 accesses static UB with the mapped UB tile ID. The default is `OFF` |
| `CASE1_VF_MODE` | `none`, `simd`, `simt`, `both` | VF-call path for Case 1. The default is `simd` |
| `CASE1_RESERVED_MODE` | `default`, `disable_vf`, `disable_all` | Reserved-UB mode for Case 1. The default is `default`. `disable_vf` adds the compiler option `--cce-disable-vf-stack-reserved-ubuf`; `disable_all` adds both `--cce-disable-vf-stack-reserved-ubuf` and `--cce-disable-asc-reserved-ubuf` |
| `CASE1_OVER_LIMIT` | `ON`, `OFF` | Whether Case 1 allocates 4 B more than the usable UB limit. The default is `OFF` |
| `CASE3_ACCESS_REGION` | `0`-`4` | Case 3 target. Default: `0`. `0`: static UB; `1`: unoccupied ordinary UB space; `2`: 6 KB VF reserve; `3`: 2 KB Ascend C API reserve; `4`: Data Cache |

Case 0 runs the issue scenario with a 50 KB input size:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCASE0_ENABLE_FIX=OFF ..
make -j
./ub_usage_limit 12800
```

When a smaller input is specified, only four data tiles are processed, so the issue corresponding to `tile_id = 4` is not triggered:

```bash
./ub_usage_limit 10240
```

Add `CASE0_ENABLE_FIX=ON` to access static UB with the mapped UB tile ID and build the fixed scenario:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCASE0_ENABLE_FIX=ON ..
make -j
./ub_usage_limit 12800
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
| Both SIMD VF and SIMT VF | Default configuration | `-DCASE1_VF_MODE=both -DCASE1_RESERVED_MODE=default` |
| Both SIMD VF and SIMT VF | VF reservation disabled | `-DCASE1_VF_MODE=both -DCASE1_RESERVED_MODE=disable_vf` |
| Both SIMD VF and SIMT VF | All reserved UB disabled | `-DCASE1_VF_MODE=both -DCASE1_RESERVED_MODE=disable_all` |

For example, to build and run Case 1 when calling both a SIMD VF and a SIMT VF with all reserved UB disabled, use the following commands:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=1 \
  -DCASE1_VF_MODE=both \
  -DCASE1_RESERVED_MODE=disable_all ..
make -j
./ub_usage_limit
```

**Execution result examples**:

The following examples show execution results for each Case.

- For the issue scenario of Case 0, the following information is displayed after execution:

    ```text
    [Case0] mode=baseline
    [Failed] Case accuracy verification failed!
    ```

    For the fixed scenario, the following information is displayed after successful execution:

    ```text
    [Case0] mode=fixed
    test pass!
    ```

- For Case 1, the run succeeds within the theoretical UB limit, and the log prints the following information:

    ```text
    [Case1] result=PASSED
    ```

    After Case 1 allocates one more `uint32_t` element than the theoretical UB limit, kernel launch fails and the log prints the following information:

    ```text
    [Host] kernel error: ... the sum of dynamic shared memory and kernel shared memory exceeds the maximum limit.
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    ```

- For Case 2, the following information is displayed after execution:

    ```text
    [Case2] static_total_bytes=7168
    [Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
    [Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
    [Case2] result=PASSED
    ```

- When Case 3 accesses the valid static UB range, unallocated normal UB range, 6 KB VF reserved range, and 2 KB Ascend C API reserved range, no accuracy error is observed, and the log prints the following information:

    ```text
    [Case3] region=static_ub
    test pass!

    [Case3] region=unallocated_ub
    test pass!

    [Case3] region=vf_reserved
    test pass!

    [Case3] region=asc_reserved
    test pass!
    ```

    When Case 3 accesses Data Cache, an accuracy error is observed, and the log prints the following information:

    ```text
    [Case3] region=data_cache
    [Failed] Case accuracy verification failed!
    ```
