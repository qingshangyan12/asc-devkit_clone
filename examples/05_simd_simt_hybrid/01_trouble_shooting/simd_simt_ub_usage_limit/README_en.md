# UB Out-of-Bounds Troubleshooting Sample for SIMD and SIMT Hybrid Programming

## Overview

This sample uses an output accuracy issue caused by a static UB out-of-bounds access in SIMD and SIMT hybrid programming to demonstrate how UB capacity, memory layout, and actual access ranges can be used to locate and fix the invalid static UB access. It then uses multiple Cases to explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the runtime and accuracy results of out-of-bounds accesses to different UB regions.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
|-- simd_simt_ub_usage_limit
    |-- CMakeLists.txt         // Example build script
    |-- ub_usage_limit.asc     // Ascend C sample implementation
    |-- figures                // Figures referenced by the README files
    |-- README.md              // Chinese documentation
    `-- README_en.md           // English documentation
```

## Example Description

This sample contains five Cases. Case 0 presents, locates, and fixes an accuracy issue caused by a static UB write into Data Cache. Case 1 determines UB limits for four configurations that do not include a SIMT VF. Case 2 determines UB limits for three reserved-UB configurations with a SIMT VF. Case 3 explains how static UB accumulates and how dynamic UB is shared. Case 4 identifies the runtime and accuracy results of writes to different out-of-bounds regions.

| Case | Scenario | Main Content |
| --- | --- | --- |
| Case 0 | Static UB overwrites Data Cache | Present the accuracy issue, then locate and fix the out-of-bounds write from the UB layout |
| Case 1 | UB limits without a SIMT VF | Determine the UB limit for four configurations: no SIMD VF, default SIMD VF, VF stack reservation disabled, and all reserved UB disabled |
| Case 2 | UB limits with a SIMT VF | Determine UB limits under default compilation, with the VF stack reservation disabled, and with all reserved UB disabled |
| Case 3 | UB used by MainScalar, a SIMD VF, and a SIMT VF | Explain the static and dynamic UB layout relationship across execution locations |
| Case 4 | Small static UB crosses into other UB regions | Explain the runtime and accuracy results of writes into unallocated UB, reserved regions, and Data Cache |

> **UB out-of-bounds access**
>
> Keeping the total UB allocation within the capacity limit does not make every access valid. A static array can only be accessed within its declared range, and dynamic UB can only be accessed within the size configured at kernel launch. An out-of-bounds static array or dynamic UB access may reach unallocated UB, the VF stack reserve, the Ascend C API reserve, or Data Cache. A UB out-of-bounds access may not immediately cause a runtime error, so troubleshoot it by checking address ranges, runtime logs, and output accuracy together.

**Example specifications**:

<table>
<tr><td align="center">Example Type (OpType)</td><td colspan="4" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="3" align="center">Example Input</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0, Case 4</td><td align="center">input</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Case 1 to Case 3</td><td colspan="3" align="center">No GM input; test data is generated inside the kernel</td></tr>
<tr><td rowspan="3" align="center">Example Output</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0, Case 4</td><td align="center">output</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Case 1 to Case 3</td><td align="center">result</td><td align="center">[8]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Kernel Name</td><td colspan="4" align="center">Case0StaticUbOobKernel/Case1StaticUbLimitKernel/SimtUbLimitKernel/MixedUbKernel/Case4StaticUbOobKernel</td></tr>
</table>

## Example Implementation

The Cases follow an issue presentation and fix, UB-limit determination, memory-layout explanation, and out-of-bounds result analysis sequence.

### Case 0: Static UB Out-of-Bounds Access and Troubleshooting

**Objective**:

Present an element-wise computation whose kernel reports no runtime error but whose output fails accuracy verification, then locate the cause by checking UB capacity, memory layout, and actual access ranges, and verify the fix.

**Core implementation**:

Case 0 adds one to each input element in GM:

```text
output[i] = input[i] + 1
```

The kernel starts 1024 SIMT threads for this computation and allocates 216 KB of static UB. Between two executions of the computation, MainScalar writes the static array through a runtime index. The Host compares the second output with the Golden data.

The key code is as follows:

```cpp
__ubuf__ uint32_t static_ub[CASE0_STATIC_WORDS];

asc_vf_call<SimtCase0AddOne>(dim3(THREAD_COUNT), input, output, BUFFER_WORDS);
WaitVectorToScalar();

__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[faulty_index] = CORRUPTED_VALUE;

WaitScalarToVector();
asc_vf_call<SimtCase0AddOne>(dim3(THREAD_COUNT), input, output, BUFFER_WORDS);
```

**Execution result**:

After the static UB out-of-bounds write reaches Data Cache, the kernel reports no runtime error, but output accuracy verification fails.

```text
[Case0] mode=fault, static_bytes=221184, dynamic_bytes=0, reserved_bytes=8192, dcache_bytes=32768, faulty_byte_offset=229376
[Case0] mismatch_count=1
[Case0] first_mismatch_index=896, expected=0xc0000381, actual=0xdeadbef0
[Case0] precision_check=FAILED (fault reproduced)
```

**Troubleshooting**:

1. Check kernel launch and synchronization. The Host performs accuracy verification only after kernel launch and synchronization succeed. This run reaches accuracy verification, and `mismatch_count=1` shows that the kernel completes but the output has an accuracy error.
2. Count UB usage. Case 0 uses the default compilation configuration, 216 KB of static UB, and zero dynamic UB. The remaining space contains an 8 KB reserved region (the default compilation configuration reserves this region when the kernel contains a VF call) and 32 KB of Data Cache (a kernel containing a SIMT VF requires at least 32 KB to cache data accessed by SIMT threads in GM):

    ```text
    Low address 0
    ├── Static UB        216 KB
    ├── Reserved region    8 KB
    └── Data Cache        32 KB
    High address 256 KB
    ```

3. Calculate the Data Cache location. Its size is `min(256 KB - 216 KB - 0 KB - 8 KB, 128 KB) = 32 KB`, so it starts at `256 KB - 32 KB = 224 KB`.
4. Inspect the UB access in the source code. Relative to the start of `static_ub`, the byte offset is `faulty_index * sizeof(uint32_t) = 229376 B = 224 KB`:

    ```cpp
    __ubuf__ uint32_t static_ub[216 * 1024 / sizeof(uint32_t)];
    uint32_t faulty_index = (216 * 1024 + 8 * 1024) / sizeof(uint32_t);
    __ubuf__ volatile uint32_t* static_data = static_ub;
    static_data[faulty_index] = 0xDEADBEEF;
    ```

    This location is outside the declared 216 KB static array and is exactly the start of Data Cache.
5. Check the mismatched value. The first SIMT VF read populates Data Cache, and the out-of-bounds write then corrupts the cached data. After the second SIMT VF reads the same input and adds one, the incorrect output is `0xDEADBEF0`, exactly `0xDEADBEEF + 1`. This confirms that the accuracy failure is caused by the out-of-bounds write into Data Cache. The sample checks whether any mismatch exists without relying on a fixed mismatch position.

**Solution**:

- Check the index range before writing the static array through a runtime index. Perform the write only when `faulty_index` is less than the static array element count `CASE0_STATIC_WORDS`, preventing access beyond the declared range of `static_ub`.

    ```cpp
    if (faulty_index < CASE0_STATIC_WORDS) {
        static_data[faulty_index] = CORRUPTED_VALUE;
    }
    ```

- After the index check is added, the out-of-bounds write is blocked and output accuracy verification passes:

    ```text
    [Case0] mode=fixed, static_bytes=221184, dynamic_bytes=0, reserved_bytes=8192, dcache_bytes=32768, faulty_byte_offset=229376
    [Case0] mismatch_count=0
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

**Conclusion**:

The fixed mode passes its accuracy check. This Case demonstrates that a UB out-of-bounds access can corrupt SIMT Data Cache without causing a kernel launch or runtime error. Troubleshooting and fixing the issue requires validating each actual access index, not only the total UB allocation.

Case 0 shows that a UB out-of-bounds access may appear as an accuracy failure. Further diagnosis requires understanding UB capacity and layout in mixed programming: MainScalar, SIMD VFs, and SIMT VFs can declare static or dynamic UB; static UB at different execution locations accumulates, while dynamic UB declarations share the space configured at kernel launch; VF calls also affect reserved space and UB limits. Cases 1 through 4 explain UB limits under different VF configurations, the layout relationship between static and dynamic UB, and the results of out-of-bounds writes into different regions.

### Case 1: UB Limits Without a SIMT VF

**Objective**:

Determine UB limits without a SIMT VF under different VF-call and reserved-UB configurations.

**Core implementation**:

Case 1 uses the static `static_ub` array to validate capacity boundaries in four scenarios. Without a SIMT VF, no Data Cache is allocated, so VF calls and reserved UB determine the usable UB limit:

| Scenario | Reserved UB | Usable UB Limit | `static_ub` Limit |
| --- | ---: | ---: | ---: |
| No SIMD VF call | 0 KB | 256 KB | 262112 B |
| SIMD VF with default compilation | 8 KB | 248 KB | 253920 B |
| 6 KB VF stack reservation disabled | 2 KB | 254 KB | 260064 B |
| 6 KB VF stack and 2 KB Ascend C API reservations disabled | 0 KB | 256 KB | 262112 B |

The `static_ub` limits in the table exclude the fixed 32-byte result array. The usable UB limits are calculated as follows:

- No SIMD VF call: `256 KB`.
- SIMD VF with default compilation: `256 KB - 8 KB = 248 KB`.
- 6 KB VF stack reservation disabled: `256 KB - 2 KB = 254 KB`.
- All 8 KB of reserved UB disabled: `256 KB`.

The sample adjusts `static_ub` until the current value runs successfully and adding 4 bytes causes kernel launch to fail. The static array validates capacity only; it does not compare static and dynamic UB capacity.

The kernel declares an adjustable static UB array and a fixed 32-byte result array. MainScalar fills `static_ub` when no SIMD VF is called; otherwise, the SIMD VF fills it. The kernel then reads the first and last elements for Host verification:

```cpp
__ubuf__ uint32_t static_ub[BOUNDARY_STATIC_WORDS];
__ubuf__ uint32_t result[8];

#if defined(UB_BOUNDARY_NO_SIMD_VF)
for (uint32_t index = 0; index < BOUNDARY_STATIC_WORDS; ++index) {
    static_ub[index] = CASE1_MARKER + index;
}
#else
asc_vf_call<SimdFillBoundaryUb>(static_ub, BOUNDARY_STATIC_WORDS);
WaitVectorToScalar();
#endif

result[2] = static_ub[0];
result[3] = static_ub[BOUNDARY_STATIC_WORDS - 1];
```

The `static_ub` size is adjusted in four-byte increments.

#### Scenario 1: Without a SIMD VF

Without a SIMD VF, the current kernel has a 256 KB UB limit. After subtracting the fixed 32-byte result array, the `static_ub` limit is 262112 bytes.

```text
static_ub limit = 256 KB - 32 B = 262112 B
```

**Execution result**:

| `static_ub` Size | Total UB Usage | Result |
| ---: | ---: | --- |
| 262112 B (256 KB - 32 B) | 256 KB | Passed |
| 262116 B | 256 KB + 4 B | Kernel launch failed |

```text
# 262112-byte static_ub
[Case1] mode=without SIMD VF, static_bytes=262112
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# 262116-byte static_ub
[Case1] mode=without SIMD VF, static_bytes=262116
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

#### Scenario 2: With a SIMD VF and Default Compilation

Call a SIMD VF with the default compiler configuration. According to the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description), the 6 KB VF stack reserve and the 2 KB Ascend C API reserve are excluded from usable UB, so the combined size of `static_ub` and the fixed 32-byte result array cannot exceed 248 KB:

```text
Usable UB limit = 256 KB - 8 KB = 248 KB
static_ub limit = 248 KB - 32 B = 253920 B
```

**Execution result**:

| `static_ub` Size | Total UB Usage | Result |
| ---: | ---: | --- |
| 253920 B (248 KB - 32 B) | 248 KB | Passed |
| 253924 B | 248 KB + 4 B | Kernel launch failed |

```text
# 253920-byte static_ub
[Case1] mode=with SIMD VF, static_bytes=253920
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# 253924-byte static_ub
[Case1] mode=with SIMD VF, static_bytes=253924
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

#### Scenario 3: With a SIMD VF and the VF Stack Reservation Disabled

After the 6 KB VF stack reservation is disabled for a kernel that calls a SIMD VF, the 2 KB Ascend C API reserve remains. Therefore, `static_ub` and the fixed 32-byte result array cannot exceed 254 KB in total.

```text
Usable UB limit = 256 KB - 2 KB = 254 KB
static_ub limit = 254 KB - 32 B = 260064 B
```

**Execution result**:

| `static_ub` Size | Total UB Usage | Result |
| ---: | ---: | --- |
| 260064 B (254 KB - 32 B) | 254 KB | Passed |
| 260068 B | 254 KB + 4 B | Kernel launch failed |

```text
# 260064-byte static_ub
[Case1] mode=with SIMD VF, static_bytes=260064
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# 260068-byte static_ub
[Case1] mode=with SIMD VF, static_bytes=260068
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

#### Scenario 4: With a SIMD VF and All Reserved UB Disabled

After both the 6 KB VF stack reservation and the 2 KB Ascend C API reservation are disabled for a kernel that calls a SIMD VF, the current kernel can use all 256 KB of UB. After subtracting the fixed 32-byte result array, the `static_ub` limit is 262112 bytes.

```text
Usable UB limit = 256 KB
static_ub limit = 256 KB - 32 B = 262112 B
```

**Execution result**:

| `static_ub` Size | Total UB Usage | Result |
| ---: | ---: | --- |
| 262112 B (256 KB - 32 B) | 256 KB | Passed |
| 262116 B | 256 KB + 4 B | Kernel launch failed |

```text
# 262112-byte static_ub
[Case1] mode=with SIMD VF, static_bytes=262112
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# 262116-byte static_ub
[Case1] mode=with SIMD VF, static_bytes=262116
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

**Analysis**:

With a SIMD VF under the default configuration, the 6 KB VF stack reserve and the 2 KB Ascend C API reserve are excluded from usable UB. `--cce-disable-vf-stack-reserved-ubuf` releases only the 6 KB VF stack reserve; after enabling it, the compiler can no longer use this region to cache register spills, so the kernel must not incur register spills. `--cce-disable-asc-reserved-ubuf` releases the other 2 KB reserved for [selected Ascend C APIs](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api); after enabling it, APIs that depend on this region are unavailable and cause a compilation error when called. See the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) for the restrictions of both options.

**Conclusion**:

For the current kernel, the UB limit is 256 KB without a SIMD VF, 248 KB with a SIMD VF under the default configuration, 254 KB after disabling the 6 KB VF stack reservation, and 256 KB after disabling both the 6 KB VF stack reservation and the 2 KB Ascend C API reservation. For other kernels, count all UB declarations and confirm whether the VFs and Ascend C APIs in use depend on reserved UB.

### Case 2: UB Limits with a SIMT VF

**Objective**:

Determine UB limits in the current kernel when a SIMT VF is present under three configurations: default compilation, the 6 KB VF stack reservation disabled, and both the 6 KB VF stack reservation and 2 KB Ascend C API reservation disabled. Each configuration validates its boundary with both static and dynamic UB arrays.

**Core implementation**:

According to the [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制), each Vector Core has 256 KB of UB, and a kernel containing a SIMT VF requires at least 32 KB of Data Cache. The combined static and dynamic UB limit is determined by reserved UB and Data Cache:

| Scenario | Reserved UB | Data Cache | Combined UB Limit | UB Array Limit |
| --- | ---: | ---: | ---: | ---: |
| Default | 8 KB | 32 KB | 216 KB | 221152 B |
| 6 KB VF stack reservation disabled | 2 KB | 32 KB | 222 KB | 227296 B |
| 6 KB VF stack and 2 KB Ascend C API reservations disabled | 0 KB | 32 KB | 224 KB | 229344 B |

The UB array limits in the table exclude the fixed 32-byte result array. The combined UB limits are calculated as follows:

- Default compilation: `256 KB - 8 KB - 32 KB = 216 KB`.
- 6 KB VF stack reservation disabled: `256 KB - 2 KB - 32 KB = 222 KB`.
- All 8 KB of reserved UB disabled: `256 KB - 32 KB = 224 KB`.

Case 2 validates the same capacity boundary with both static and dynamic UB arrays. Dynamic UB size is specified by the kernel launch argument, while static mode allocates no dynamic UB. In both implementations, the SIMT VF fills the complete array and MainScalar reads the first and last elements for Host verification, ensuring that the entire allocated range is accessed. The key code is as follows:

```cpp
#if defined(CASE2_USE_STATIC_UB)
__ubuf__ uint32_t case2_ub[CASE2_UB_WORDS];
#else
extern __ubuf__ uint32_t case2_ub[];
#endif
__ubuf__ uint32_t result[RESULT_WORDS_PER_CASE];

asc_vf_call<SimtFillCase2Ub>(dim3(THREAD_COUNT), case2_ub, CASE2_UB_WORDS, CASE2_MARKER);
WaitVectorToScalar();
result[3] = case2_ub[0];
result[4] = case2_ub[CASE2_UB_WORDS - 1];
```

**Execution result**:

Both static and dynamic UB arrays are validated at 4-byte granularity under all three reserved-UB configurations:

| Compilation Configuration | UB Type | UB Array Limit | Result at Limit | Result After Adding 4 B |
| --- | --- | ---: | --- | --- |
| Default | Static/Dynamic | 221152 B | Passed | Kernel launch failed |
| 6 KB VF stack reservation disabled | Static/Dynamic | 227296 B | Passed | Kernel launch failed |
| All 8 KB reserved UB disabled | Static/Dynamic | 229344 B | Passed | Kernel launch failed |

Key output under the default configuration is as follows:

```text
[Case2] ub_type=dynamic, ub_array_limit_bytes=221152, configured_ub_bytes=221152
Case 2 (SIMT VF usable UB): PASSED

[Case2] ub_type=static, ub_array_limit_bytes=221152, configured_ub_bytes=221152
Case 2 (SIMT VF usable UB): PASSED
```

Adding one `uint32_t` element increases the UB array size to 221156 bytes. Together with the 32-byte result array, the total exceeds 216 KB by 4 bytes and kernel launch fails in both modes:

```text
[Case2] ub_type=dynamic, ub_array_limit_bytes=221152, configured_ub_bytes=221156
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 2 (SIMT VF usable UB): FAILED

[Case2] ub_type=static, ub_array_limit_bytes=221152, configured_ub_bytes=221156
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 2 (SIMT VF usable UB): FAILED
```

After disabling the 6 KB VF stack reservation, both array types pass at 227296 bytes and kernel launch fails at 227300 bytes. After disabling all 8 KB of reserved UB, both array types pass at 229344 bytes and kernel launch fails at 229348 bytes.

**Conclusion**:

A kernel containing a SIMT VF requires at least 32 KB of Data Cache. Under default compilation, with the 6 KB VF stack reservation disabled, and with all 8 KB of reserved UB disabled, the current kernel has combined static and dynamic UB limits of 216 KB, 222 KB, and 224 KB, respectively. After subtracting the fixed 32-byte result array, the UB array limits are 221152, 227296, and 229344 bytes. Static and dynamic UB arrays pass at the same boundary under all three configurations.

### Case 3: Static and Dynamic UB Layout Across Execution Locations

**Objective**:

Explain that static UB declarations in MainScalar, a SIMD VF, and a SIMT VF accumulate, while the three differently named dynamic UB declarations have the same start address and share the region configured at kernel launch.

**Core implementation**:

Case 3 declares static UB separately in MainScalar, a SIMD VF, and a SIMT VF. MainScalar declares a 1 KB `main_static_ub` array and three 32-byte result arrays, the SIMD VF declares 2 KB, and the SIMT VF declares 4 KB. Static UB declarations at different execution locations occupy separate space and accumulate in the current kernel, for a total of 7264 bytes.

Each execution location writes its static array and returns the last element for Host verification. Dynamic UB space is configured only once at kernel launch. The following code shows the static and dynamic UB declarations at all three execution locations:

```cpp
// MainScalar
extern __ubuf__ uint32_t main_dynamic_ub[];
__ubuf__ uint32_t main_static_ub[MIXED_MAIN_STATIC_WORDS];
// SIMD VF
extern __ubuf__ uint32_t simd_dynamic_ub[];
__ubuf__ uint32_t simd_static_ub[MIXED_SIMD_STATIC_WORDS];
// SIMT VF
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t simt_static_ub[MIXED_SIMT_STATIC_WORDS];
```

Each context accesses the last element of its dynamic UB array so that the configured range participates in the layout. It then converts the array start address to an integer and returns it through the result area for the Host to print and compare. This access does not repeat the usable-UB limit validation already covered by Case 2. The printed addresses belong to the AICore UB address space and are used only for the layout comparison in this Case.

**Execution result**:

The static UB total is 7264 bytes, which is `0x1c60` in hexadecimal. Dynamic UB follows static UB, so its expected start address is `0x1c60`. The result is as follows:

```text
[Case3] static_total_bytes=7264
[Case3] main_dynamic_ub=0x1c60, simd_dynamic_ub=0x1c60, simt_dynamic_ub=0x1c60
Case 3 layout: static_total_bytes=7264, same_start_address=YES
Case 3 (mixed UB): PASSED
```

The dynamic UB start address is `0x1c60` in MainScalar, the SIMD VF, and the SIMT VF, matching the address corresponding to the static UB total.

**Conclusion**:

Static UB usage in MainScalar, the SIMD VF, and the SIMT VF accumulates, and dynamic UB follows these static UB regions. The three dynamic arrays have different names but the same start address and share the region configured by the Host at kernel launch.

### Case 4: Small Static UB Crosses Into Different UB Regions

**Objective**:

Starting from a 4 KB static UB array, access its valid range, unallocated UB, the 6 KB VF stack reserve, the 2 KB Ascend C API reserve, and Data Cache, then identify whether each access is out of bounds and whether it causes a runtime or accuracy error.

**Core implementation**:

Case 4 uses the default compilation configuration, declares only 4 KB of static UB, and configures zero dynamic UB. Because static and dynamic UB usage is small, Data Cache reaches its 128 KB maximum and occupies the high-address half of UB. According to the [3510 architecture specification](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description) and [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md), the 8 KB reserved region consists of a 6 KB VF stack reserve and a 2 KB Ascend C API reserve immediately before Data Cache. The resulting layout is:

```text
Data Cache size = min(256 KB - 4 KB - 0 KB - 8 KB, 128 KB) = 128 KB
Data Cache start = 256 KB - 128 KB = 128 KB
Reserved-region start = 128 KB - 8 KB = 120 KB
Unallocated UB size = 120 KB - 4 KB = 116 KB
```

![Case 4 UB layout and access positions](./figures/case4_ub_layout_en.png "Case 4 UB layout and access positions")

Only the first 4 KB is valid for the static array. The sample defines an access offset for each of five target regions. The Host converts the byte offset to an index and passes it to the kernel, which writes to the selected region through a `volatile` pointer and a runtime index:

| Target Region | Offset From the Static Array Start |
| --- | ---: |
| Last `uint32_t` element inside static UB | 4092 B |
| Middle of unallocated ordinary UB | 63488 B |
| Middle of the 6 KB VF stack reserve | 125952 B |
| Middle of the 2 KB Ascend C API reserve | 130048 B |
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
| 6 KB VF stack reserve | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| 2 KB Ascend C API reserve | Succeeded | Passed | Out of bounds, but no visible failure in this run |
| Data Cache | Succeeded | Failed with one mismatch | Cached data was corrupted without a runtime error |

Key output from all five targets follows. `launch_ret=0` and `sync_ret=0` mean that neither launch nor synchronization reported an error; `precision_check` only reports whether output matches the Golden data:

```text
# Valid static UB
[Case4] region=static_ub, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=4092
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# Unallocated ordinary UB
[Case4] region=unallocated_ub, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=63488
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# 6 KB VF stack reserve
[Case4] region=vf_stack_6kb, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=125952
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# 2 KB Ascend C API reserve
[Case4] region=asc_reserved_2kb, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=130048
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# Data Cache
[Case4] region=data_cache, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=131072
[Case4] mismatch_count=1
[Case4] first_mismatch_index=896, expected=0xc0040381, actual=0xdeadbef0
[Case4] precision_check=FAILED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED
```

**Analysis**:

Passing the accuracy check does not make the unallocated or reserved ranges valid to access. The [SIMT memory hierarchy](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制) states that an access beyond an allocated static or dynamic UB range may not immediately cause an allocation failure, but it can corrupt Data Cache and lead to unstable results or accuracy errors. The table therefore reports only what occurred in this run; it does not establish that an out-of-bounds access is valid or that the target region was unused.

**Conclusion**:

In Case 4, only the access within the valid static UB range is in bounds. The other four scenarios all cause static-array out-of-bounds accesses. An access outside the declared static array remains invalid even when the kernel completes and the current accuracy check passes.

## UB Capacity Rules

With a SIMT VF, the 256 KB UB of one Vector Core contains static UB, dynamic UB, the region retained by the current compilation configuration, and Data Cache. By default, 6 KB is reserved for the VF stack and 2 KB for [selected Ascend C APIs](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api). After a compiler option releases either region, only the region that remains reserved is subtracted:

```text
Data Cache = min(256 KB - static UB - dynamic UB - reserved UB, 128 KB)
Default reserved UB = 6 KB VF stack reserve + 2 KB Ascend C API reserve = 8 KB
actual dynamic UB access range <= configured dynamic UB size
```

Data Cache requires at least 32 KB, so the usable UB limit is `256 KB - reserved UB - 32 KB`. The combined static and dynamic UB limits are 216 KB under default compilation, 222 KB after disabling the 6 KB VF stack reservation, and 224 KB after disabling both reserved regions.

The static total includes declarations in MainScalar, SIMD VFs, and SIMT VFs. Both the total capacity rule and each array's direct access boundary must hold.

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
    SCENARIO_NUM=1                              # Select a Case: 0, 1, 2, 3, or 4
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
| `SCENARIO_NUM` | `0`-`4` | Case number. The default value is `1` |
| `CASE0_ENABLE_FIX` | `ON`, `OFF` | Whether Case 0 checks the runtime static-array index. The default is `OFF` |
| `UB_BOUNDARY_STATIC_BYTES` | Positive multiple of 4 B | Case 1 static UB array size in bytes. The default is `253920` |
| `UB_BOUNDARY_NO_SIMD_VF` | `ON`, `OFF` | Whether Case 1 omits the SIMD VF call. The default is `OFF` |
| `UB_BOUNDARY_DISABLE_RESERVED` | `ON`, `OFF` | Whether Case 1 passes `--cce-disable-vf-stack-reserved-ubuf`. The default is `OFF` |
| `UB_BOUNDARY_DISABLE_ASC_RESERVED` | `ON`, `OFF` | Whether Case 1 passes `--cce-disable-asc-reserved-ubuf`. The default is `OFF` |
| `CASE2_UB_BYTES` | Positive multiple of 4 B | Case 2 static or dynamic UB array size in bytes. The default is `221152` |
| `CASE2_USE_STATIC_UB` | `ON`, `OFF` | Whether Case 2 uses a static UB array. The default is `OFF`, which selects dynamic UB |
| `CASE2_DISABLE_VF_STACK_RESERVED` | `ON`, `OFF` | Whether Case 2 passes `--cce-disable-vf-stack-reserved-ubuf`. The default is `OFF` |
| `CASE2_DISABLE_ASC_RESERVED` | `ON`, `OFF` | Whether Case 2 passes `--cce-disable-asc-reserved-ubuf`. The default is `OFF` |
| `CASE4_ACCESS_REGION` | `0`-`4` | Case 4 target. Default: `0`. `0`: static UB; `1`: unallocated UB; `2`: 6 KB VF stack reserve; `3`: 2 KB Ascend C API reserve; `4`: Data Cache |

Use the following additional CMake options for the four Case 1 scenarios:

| Scenario | Additional CMake Options |
| --- | --- |
| No SIMD VF call | `-DUB_BOUNDARY_NO_SIMD_VF=ON -DUB_BOUNDARY_STATIC_BYTES=262112` |
| SIMD VF with default compilation | `-DUB_BOUNDARY_STATIC_BYTES=253920` |
| 6 KB VF stack reservation disabled | `-DUB_BOUNDARY_DISABLE_RESERVED=ON -DUB_BOUNDARY_STATIC_BYTES=260064` |
| All 8 KB of reserved UB disabled | `-DUB_BOUNDARY_DISABLE_RESERVED=ON -DUB_BOUNDARY_DISABLE_ASC_RESERVED=ON -DUB_BOUNDARY_STATIC_BYTES=262112` |

Use the following additional CMake options for the three Case 2 reservation configurations. These options select dynamic UB by default; add `-DCASE2_USE_STATIC_UB=ON` to validate the same boundary with static UB.

| Scenario | Additional CMake Options |
| --- | --- |
| Default compilation | `-DCASE2_UB_BYTES=221152` |
| 6 KB VF stack reservation disabled | `-DCASE2_DISABLE_VF_STACK_RESERVED=ON -DCASE2_UB_BYTES=227296` |
| All 8 KB of reserved UB disabled | `-DCASE2_DISABLE_VF_STACK_RESERVED=ON -DCASE2_DISABLE_ASC_RESERVED=ON -DCASE2_UB_BYTES=229344` |

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
    [Case0] mismatch_count=1
    [Case0] precision_check=FAILED (fault reproduced)
    # Accuracy issue observed
    ```

- Case 1 calls a SIMD VF by default and uses a 253920-byte `static_ub`. Key output:

    ```text
    [Case1] mode=with SIMD VF, static_bytes=253920
    [Case1] launch_ret=0, sync_ret=0, result=PASSED
    ```

- Case 2 uses 221152 bytes of dynamic UB by default. Key output:

    ```text
    [Case2] ub_type=dynamic, ub_array_limit_bytes=221152, configured_ub_bytes=221152
    Case 2 (SIMT VF usable UB): PASSED
    ```

- Key output for Case 3:

    ```text
    [Case3] static_total_bytes=7264
    [Case3] main_dynamic_ub=0x1c60, simd_dynamic_ub=0x1c60, simt_dynamic_ub=0x1c60
    Case 3 layout: static_total_bytes=7264, same_start_address=YES
    Case 3 (mixed UB): PASSED
    ```

- Case 4 uses the default `CASE4_ACCESS_REGION=0`. Key output:

    ```text
    [Case4] region=static_ub, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=4092
    [Case4] mismatch_count=0
    [Case4] precision_check=PASSED
    [Case4] launch_ret=0, sync_ret=0, observation=COMPLETED
    ```

**Boundary and error result examples**:

- After disabling the 6 KB VF stack reservation and the 2 KB Ascend C API reservation in Case 1, a 262112-byte `static_ub` and the 32-byte result array use all 256 KB and pass:

    ```text
    [Case1] mode=with SIMD VF, static_bytes=262112
    [Case1] launch_ret=0, sync_ret=0, result=PASSED
    ```

- In Case 1 with the default SIMD VF configuration, increasing `static_ub` from 253920 to 253924 bytes makes total UB usage exceed the 248 KB limit by 4 bytes, and kernel launch fails:

    ```text
    [Case1] mode=with SIMD VF, static_bytes=253924
    [Case1] launch_ret=107000, sync_ret=107000, result=FAILED
    ```

- In Case 2, increasing dynamic UB from 221152 to 221156 bytes makes total UB usage exceed the 216 KB limit by 4 bytes, and kernel launch fails:

    ```text
    [Case2] ub_type=dynamic, ub_array_limit_bytes=221152, configured_ub_bytes=221156
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    Case 2 (SIMT VF usable UB): FAILED
    ```

- In Case 4, set `CASE4_ACCESS_REGION=4` to write out of bounds into Data Cache. Kernel launch and synchronization succeed, but the output has an accuracy error:

    ```text
    [Case4] region=data_cache, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=131072
    [Case4] mismatch_count=1
    [Case4] precision_check=FAILED
    [Case4] launch_ret=0, sync_ret=0, observation=COMPLETED
    ```
