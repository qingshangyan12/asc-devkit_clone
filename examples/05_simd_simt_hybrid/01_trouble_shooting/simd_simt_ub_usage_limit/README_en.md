# UB Usage Limit Sample for SIMD and SIMT Hybrid Programming

## Overview

This sample demonstrates Unified Buffer (UB) usage patterns, capacity calculation, and out-of-bounds troubleshooting in Ascend C SIMD and SIMT hybrid programming. The sample uses a `__global__ __vector__` kernel entry and invokes `__simt_vf__` and `__simd_vf__` functions through `asc_vf_call`. Four positive cases run by default. The out-of-bounds case is documented as a README-only anti-pattern and is not executed by default.

Besides the 4 default positive cases, `ub_usage_limit.asc` also contains 4 built-in **hardware probes** (`probe1` / `case3_sweep` / `case5_probe` / `platform_query`) used to empirically pin down, word-by-word, on real NPU hardware:

1. Without any SIMT VF, can SIMD actually use the full 256KB of UB?
2. When both a SIMT VF and a SIMD VF declare static UB, is the dynamic UB limit really `total UB - static1 - static2`?
3. When an out-of-bounds dynamic UB access grows more severe, does it fail silently (data overwritten, no error) or trigger an aicore runtime error?
4. What UB size does the Host-side `platform_ascendc::PlatformAscendC::GetCoreMemSize(CoreMemType::UB, ...)` API actually return under no-VF, SIMD-VF, and SIMT-VF kernel scenarios — does it change with the VF usage pattern of the kernel that ran?

The exact test procedure, commands, and outputs for these 4 probes are recorded in the **Empirical Findings and Troubleshooting** section below (see `README.md` for the full Chinese write-up), and can be reproduced directly with the commands shown there.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Version

- > CANN 9.0.0

## Directory Structure

```text
├── simd_simt_ub_usage_limit
│   ├── CMakeLists.txt
│   ├── ub_usage_limit.asc
│   ├── README.md
│   └── README_en.md
```

## UB Capacity Model

In hybrid programming, SIMD/MainScalar, SIMD VF, and SIMT VF share the same UB resource on a Vector Core. This sample uses the following capacity model:

| Item | Value | Description |
| --- | ---: | --- |
| Total UB | 256KB | UB capacity of one Vector Core |
| Reserved space | 8KB | Reserved by the system and should not be used as user UB |
| Minimum SIMT Data Cache reservation | 32KB | Must be reserved when SIMT VF exists |
| User UB without SIMT VF | 248KB | `256KB - 8KB` |
| User UB with SIMT VF | 216KB | `256KB - 8KB - 32KB` |

When a kernel contains SIMT VF, static UB, dynamic UB, UB used by SIMD VF, UB used by SIMT VF, and UB used by MainScalar must be counted together against the user-available UB limit.

## Cases

| Case | Purpose |
| --- | --- |
| Case 1 | Uses static UB in a `__global__ __vector__` kernel without SIMT VF. |
| Case 2 | Uses dynamic UB through SIMT VF and counts the Data Cache reservation. |
| Case 3 | Declares static UB in both SIMT VF and SIMD VF to show shared UB capacity. |
| Case 4 | Configures dynamic UB through the kernel launch parameter and checks its access range. |
| Case 5 | Documents an out-of-bounds anti-pattern and troubleshooting method. It is not run by default. |

Case 1, Case 3, and Case 5 each have a measured nuance beyond the table above — see **Empirical Findings and Troubleshooting** below (details in `README.md`):

- Case 1: whether the 8KB reserved space is deducted depends on whether the kernel ever calls `asc_vf_call`, not merely on the absence of a *SIMT* VF.
- Case 3: the dynamic UB limit does equal `216KB - static1 - static2`, but "static1 + static2" must also include MainScalar's own static UB, not just the two VFs.
- Case 5: out-of-bounds writes within roughly 8KB past the configured allocation are tolerated silently; beyond that, they trigger a catchable aicore runtime error — but this boundary is implementation-dependent and must not be assumed safe in general.

## Empirical Findings and Troubleshooting

Four hardware probes were used to answer the questions above on real Ascend 950PR (`dav-3510`) hardware. The full step-by-step commands, raw outputs, and analysis (in Chinese) are in `README.md`'s "实测结论与定位方法" section. Summary:

1. **`probe1`** (needs `-DENABLE_UB_PROBE1=ON` and `-DUB_PROBE1_STATIC_BYTES=<bytes>`, optional `-DUB_PROBE1_NO_VF=ON`): confirmed the static UB boundary is exactly 253920 bytes (`256KB - 8KB - 32B`) when the kernel calls a SIMD VF, and exactly 262112 bytes (`256KB - 32B`) when the kernel calls no VF function at all — i.e. the 8KB reservation is tied to VF usage, not to SIMT specifically.
2. **`case3_sweep <words>`**: confirmed the dynamic UB limit is exactly 90016 bytes for a kernel with 64KB SIMT VF static UB + 64KB SIMD VF static UB + 96 bytes of MainScalar static UB, matching `216KB - 128KB - 96B` precisely — the MainScalar static UB must be included in the subtraction.
3. **`case5_probe <words>`**: with a 4096-byte configured dynamic UB, out-of-bounds access up to 12384 bytes (8288 bytes past the configured size) still runs without error; 12388 bytes (8292 bytes past) triggers a runtime error caught via `aclrtGetLastError`/`aclrtSynchronizeStream`. The boundary sits close to, but not exactly at, the 8KB reserved-space size, and should not be treated as a reliable constant — always keep actual access within the configured/allocated range.
4. **`platform_query`**: `GetCoreMemSize(CoreMemType::UB, ...)` returned exactly the same value — 253952 bytes (`256KB - 8KB - 32B`) — before running any kernel, and after running a no-VF kernel, a SIMT-VF kernel, and a SIMT+SIMD-VF kernel in turn. The API is a Host-side static hardware-spec lookup (from the platform info table loaded once per process for the current SoC) and does not reflect, or change with, any specific kernel's VF usage pattern or actual UB occupancy.

Note: the documented bisheng flag `--cce-disable-vf-stack-reserved-ubuf` (meant to disable the 8KB VF-stack reservation) was tested via the `UB_PROBE1_DISABLE_RESERVED` CMake option and found **not supported** by the installed CANN 9.0.0 toolchain (`bisheng: error: unsupported option`). This is a known limitation of the current environment, not of this sample.

## Build and Run

Run the following commands in this sample directory:

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build
cmake --build build
./build/ub_usage_limit
```

The default output prints `used_bytes`, `limit_bytes`, and verification status for the four positive cases. The sample passes when all cases are `PASSED` and the final output is:

```text
[Success] All UB usage limit cases are verification passed.
```
