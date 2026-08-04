# UB Out-of-Bounds Troubleshooting Sample for SIMD and SIMT Hybrid Programming

## Overview

This sample starts with a simplified element-wise operator whose static UB access crosses into dynamic UB. Four positive cases then explain usable static UB limits under different VF configurations, dynamic UB, and UB usage in hybrid programming.

The compile option `SCENARIO_NUM` selects the Case. Each build compiles and runs exactly one Case, with no shared kernel, dynamic UB, output buffer, or runtime state between Cases. Case 0 intentionally causes a precision failure; Cases 1 through 4 are independent positive or boundary-study scenarios.

| Case | Scenario | What It Verifies | Execution |
| --- | --- | --- | --- |
| Case 0 | Static UB overwrites dynamic UB | An out-of-bounds write corrupts operator input and causes a precision failure | `SCENARIO_NUM=0` |
| Case 1 | MainScalar static UB without a VF | Basic static UB usage when no VF is called | `SCENARIO_NUM=1` |
| Case 2 | Usable static UB limit study | Effects of a SIMD VF and the compiler option on the usable static UB limit | `SCENARIO_NUM=2` |
| Case 3 | Dynamic UB with a SIMT VF | Configured dynamic UB must cover the actual access range | `SCENARIO_NUM=3` |
| Case 4 | UB shared by MainScalar, SIMD VF, and SIMT VF | Accumulated static UB and the single global dynamic UB allocation | `SCENARIO_NUM=4` |

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Case 0: Precision Failure Caused by a Static UB Out-of-Bounds Write

Case 0 models an incorrect static-array index in an operator. The kernel allocates 4 KB of static UB and correctly configures 8 KB of dynamic UB for two 4 KB buffers:

```text
Low address
|-- static_ub 4 KB, valid indices [0, 1023]
|-- input     4 KB, start of dynamic UB
`-- output    4 KB
High address

Configured dynamic UB: 8 KB
```

A SIMT VF first initializes `input`. MainScalar then writes four bytes beyond the static array through a runtime index and a volatile UB pointer:

```cpp
uint32_t faulty_index = 1024;
__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[faulty_index] = 0xDEADBEEF;
```

The runtime index models an operator index-calculation error, while `volatile` prevents the intentional fault write from being eliminated by the compiler. The write overwrites `input[0]` at the start of dynamic UB. A second SIMT VF computes `output[index] = input[index] + 1`. The first output changes from the Golden value `0xC0000001` to `0xDEADBEF0`, while the remaining output stays correct.

Build and run Case 0 with:

```bash
cmake -S . -B build_case0 -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0
cmake --build build_case0 -j
./build_case0/ub_usage_limit
```

Expected output:

```text
[Case0] static_bytes=4096, dynamic_bytes=8192, overflow_bytes=4
[Case0] golden_first=0xc0000001, actual_first=0xdeadbef0
[Case0] mismatch_count=1
[Case0] precision_check=FAILED (fault reproduced)
```

This demonstrates that correct total UB allocation does not prevent a single static-array out-of-bounds access from corrupting adjacent dynamic data and causing a silent precision failure.

## Positive Cases

### Case 1: Static UB Without a VF

`NoVfStaticUbKernel` calls neither a SIMD VF nor a SIMT VF. MainScalar declares and accesses a 4 KB static UB array, and the kernel launch configures zero bytes of dynamic UB. The Host verifies the first and last elements.

#### Result

The 4 KB static UB was read and written correctly with zero dynamic UB:

```text
Case 1 (no VF static UB): PASSED
```

This case uses 4096 bytes of static UB only to demonstrate basic MainScalar static UB usage. Case 2 owns the usable static UB limit study.

### Case 2: Usable Static UB Limit With and Without a SIMD VF

Case 2 compares the usable static UB limit of the current kernel under three compile-time configurations: no SIMD VF call, a SIMD VF call with default compilation, and a SIMD VF call with the VF stack reservation disabled. The `static_ub` size is adjusted until the listed value passes and adding four bytes causes kernel launch to fail. This case configures no dynamic UB, so its conclusions apply only to static UB usage.

The probe size, the presence of the SIMD VF call, and the compiler option are compile-time choices, so each comparison requires a separate build with `SCENARIO_NUM=2`.

The kernel declares a configurable static UB array and a 32-byte result array. `UB_BOUNDARY_STATIC_BYTES` must be a multiple of four bytes.

Without a SIMD VF:

```bash
cmake -S . -B build_boundary_no_vf \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=2 \
  -DUB_BOUNDARY_NO_SIMD_VF=ON \
  -DUB_BOUNDARY_STATIC_BYTES=262112
cmake --build build_boundary_no_vf -j
./build_boundary_no_vf/ub_usage_limit
```

With a SIMD VF and default compilation:

```bash
cmake -S . -B build_boundary_simd \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=2 \
  -DUB_BOUNDARY_STATIC_BYTES=253920
cmake --build build_boundary_simd -j
./build_boundary_simd/ub_usage_limit
```

With a SIMD VF and the reservation disabled:

```bash
cmake -S . -B build_boundary_flag \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=2 \
  -DUB_BOUNDARY_DISABLE_RESERVED=ON \
  -DUB_BOUNDARY_STATIC_BYTES=260064
cmake --build build_boundary_flag -j
./build_boundary_flag/ub_usage_limit
```

#### Result

Results on a real `dav-3510` NPU with CANN 9.2.0:

The table lists both the largest `static_ub` size that runs successfully for each configuration and the failure observed after adding one `uint32_t` (4 B):

| Configuration | `static_ub` Size | Result |
| --- | ---: | --- |
| No SIMD VF | 262112 B | Passed; 256 KB including the 32-byte result array |
| No SIMD VF | 262116 B | Kernel launch failed |
| SIMD VF, default | 253920 B | Passed |
| SIMD VF, default | 253924 B | Kernel launch failed |
| SIMD VF, reservation disabled | 260064 B | Passed |
| SIMD VF, reservation disabled | 260068 B | Kernel launch failed |

These results establish the usable limit of the `static_ub` array in the current kernel layout. `static_ub` is not the only static UB object in the kernel: the fixed 32-byte result array is also static UB and must be included in the total static UB usage. Each row therefore follows `total static UB = usable static_ub limit + 32-byte result array`:

| Configuration | Usable `static_ub` Limit | Total Static UB Including the 32-byte Result Array |
| --- | --- | --- |
| No SIMD VF | 262112 B | 262144 B (256 KB) |
| SIMD VF, default | 253920 B | 253952 B (248 KB) |
| SIMD VF, reservation disabled | 260064 B | 260096 B |

All three upper limits were verified at four-byte granularity: the listed value passed and adding four bytes failed. Because this case configures no dynamic UB, these results do not directly establish the limit for a combined static-and-dynamic UB layout.

Key output from the three passing configurations:

```text
# No SIMD VF, 262112-byte static array
[Case2] mode=without SIMD VF, static_bytes=262112
[Case2] launch_ret=0, sync_ret=0, result=PASSED

# SIMD VF with default compilation, 253920-byte static array
[Case2] mode=with SIMD VF, static_bytes=253920
[Case2] launch_ret=0, sync_ret=0, result=PASSED

# SIMD VF with the reservation disabled, 260064-byte static array
[Case2] mode=with SIMD VF, static_bytes=260064
[Case2] launch_ret=0, sync_ret=0, result=PASSED
```

Key output when the corresponding boundaries are exceeded:

```text
# No SIMD VF, 262116-byte static array
[Case2] mode=without SIMD VF, static_bytes=262116
[Case2] launch_ret=107000, sync_ret=107000, result=FAILED

# SIMD VF with default compilation, 253924-byte static array
[Case2] mode=with SIMD VF, static_bytes=253924
[Case2] launch_ret=107000, sync_ret=107000, result=FAILED

# SIMD VF with the reservation disabled, 260068-byte static array
[Case2] mode=with SIMD VF, static_bytes=260068
[Case2] launch_ret=107000, sync_ret=107000, result=FAILED
```

The option changes the usable boundary, but it does not imply that a kernel calling a SIMD VF can use the entire 256 KB. Compiler-generated UB usage and the VF implementation still matter. When the reservation is disabled, confirm that the VF does not spill registers into that region.

### Case 3: Dynamic UB with a SIMT VF

Case 3 verifies the dynamic UB limit when a SIMT VF is present. The kernel has a 32-byte static result array, so the calculated limit is `216 KB - 32 B = 221152 B`. `CASE3_DYNAMIC_BYTES` controls the configured size. SIMT threads fill the complete dynamic region, and the Host verifies its first and last words.

#### Result

The calculated limit passes:

```text
[Case3] max_dynamic_bytes=221152, configured_dynamic_bytes=221152
Case 3 (SIMT VF dynamic UB): PASSED
```

Adding one word fails at kernel launch:

```text
[Case3] max_dynamic_bytes=221152, configured_dynamic_bytes=221156
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 3 (SIMT VF dynamic UB): FAILED
```

The measured dynamic UB upper limit for this kernel is therefore 221152 bytes.

### Case 4: Shared Static and Dynamic UB Capacity

Case 4 declares static UB separately in MainScalar, a SIMD VF, and a SIMT VF. MainScalar uses a 1 KB `main_static_ub` array and three 32-byte result arrays named `simt_result`, `simd_result`, and `result`, for a total of 1120 bytes. The SIMD VF declares 2 KB and the SIMT VF declares 4 KB. Static UB declarations at different execution positions occupy separate space and accumulate in the current kernel. Each position accesses the end of its static array so that the compiler cannot eliminate the declared storage. In the SIMT VF, the thread responsible for the last element reads and reports that value itself, avoiding an unsynchronized cross-thread dependency. The total is 7264 bytes, giving a calculated dynamic limit of `216 KB - 7264 B = 213920 B`.

Dynamic UB is different: `CASE4_DYNAMIC_BYTES` configures its size once when the kernel is launched. MainScalar, the SIMD VF, and the SIMT VF each declare an external dynamic UB array with a different name at their respective code locations: `main_dynamic_ub`, `simd_dynamic_ub`, and `simt_dynamic_ub`. MainScalar writes A through element 0 of its array, the SIMD VF reads A and writes B through element 0 of its array, the SIMT VF reads B and writes C through element 0 of its array, and MainScalar finally reads C. All three contexts also observe the same last configured element. The successful handoff proves that these separately declared arrays have the same starting address and cover the same launch-configured dynamic UB range.

Case 4 therefore verifies two rules: static UB from different execution positions must be accumulated, while dynamic UB arrays declared separately in MainScalar, a SIMD VF, and a SIMT VF have the same starting address and correspond to the same dynamic UB region configured at kernel launch.

#### Result

The calculated limit passes:

```text
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
Case 4 usage: static_total_bytes=7264, dynamic_bytes=213920, dynamic_handoff=PASSED
Case 4 (mixed UB): PASSED
```

Adding one word fails at kernel launch:

```text
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213924
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 4 (mixed UB): FAILED
```

The measured dynamic UB upper limit after subtracting 7264 bytes of static UB is therefore 213920 bytes.

## Capacity Rules

With a SIMT VF in the default compilation mode, the 256 KB UB contains static memory, dynamic memory, an 8 KB reserved region, and Data Cache:

```text
Data Cache = min(256 KB - static UB - dynamic UB - 8 KB, 128 KB)
static UB + dynamic UB <= 216 KB
actual dynamic UB access range <= configured dynamic UB size
```

The static total includes declarations in MainScalar, SIMD VFs, and SIMT VFs. Both the total capacity rule and the direct dynamic access boundary must hold.

## Build and Run

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=<0-4>
cmake --build build -j
./build/ub_usage_limit
```

`${install_path}` is the CANN development kit installation directory. This sample requires CANN 9.2.0 or later and supports only the `dav-3510` architecture.

For example, with `SCENARIO_NUM=4`, the expected output is:

```text
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
Case 4 usage: static_total_bytes=7264, dynamic_bytes=213920, dynamic_handoff=PASSED
Case 4 (mixed UB): PASSED
```

Case-specific build options:

| Option | Default | Description |
| --- | --- | --- |
| `UB_BOUNDARY_STATIC_BYTES` | `253920` | Static UB array size for Case 2, in bytes |
| `UB_BOUNDARY_NO_SIMD_VF` | `OFF` | Uses MainScalar instead of a SIMD VF to fill static UB |
| `UB_BOUNDARY_DISABLE_RESERVED` | `OFF` | Passes `--cce-disable-vf-stack-reserved-ubuf` |
| `CASE3_DYNAMIC_BYTES` | `221152` | Dynamic UB size configured and accessed by Case 3, in bytes |
| `CASE4_DYNAMIC_BYTES` | `213920` | Dynamic UB size configured and accessed by Case 4, in bytes |

General build option:

| Option | Default | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture for Ascend 950PR/Ascend 950DT |
| `SCENARIO_NUM` | `1` | Case number to compile and run; valid values are `0` through `4` |

## Troubleshooting Procedure

1. Calculate the actual dynamic UB access range from each pointer offset and maximum index.
2. Compare that range with the dynamic UB byte count in the kernel launch.
3. Count every static UB declaration in MainScalar, SIMD VFs, and SIMT VFs.
4. When a SIMT VF is present, keep static plus dynamic UB within 216 KB.
5. Check both `aclrtGetLastError` after launch and `aclrtSynchronizeStream`.
6. Fix every access outside the configured range even when no runtime error is reported.
