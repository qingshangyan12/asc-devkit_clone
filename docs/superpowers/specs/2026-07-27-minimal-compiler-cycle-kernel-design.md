# Minimal Compiler Cycle Kernel Design

## Goal

Create a minimal AscendC NPU executable that reads the compiler-provided Kernel entry cycle and the current system cycle, writes both values directly to two GM addresses, copies them back on the Host, and prints the values and their difference.

## Scope

- Target the local `910B4` device with `dav-2201`.
- Build against the devkit package installed at `/tmp/asc-devkit-timestamp-install/cann` and the compiler/runtime at `/usr/local/Ascend/cann-9.1.0`.
- Add an isolated sample directory named `compiler_cycle_minimal`.
- Do not enable `ASCENDC_TIME_STAMP_ON`.
- Do not use `InitSocState`, Tensor APIs, `DataCopy`, `TPipe`, `TQue`, `TBuf`, `PrintTimeStamp`, or other device helpers.

## Device Design

The Kernel takes two independent `__gm__ uint64_t*` output parameters. It performs exactly two cycle reads and two direct GM stores:

```cpp
__global__ __vector__ void read_compiler_cycles(
    __gm__ uint64_t* entryCycle,
    __gm__ uint64_t* currentCycle)
{
    *entryCycle = static_cast<uint64_t>(__get_entry_sys_cnt());
    *currentCycle = static_cast<uint64_t>(get_sys_cnt());
}
```

`__get_entry_sys_cnt()` is used by the current repository's timestamp implementation but has no public devkit API document or declaration. `get_sys_cnt()` is declared as a compiler intrinsic in `cce_aicore_intrinsics.h`. This sample is therefore a compiler-behavior experiment, not a stable public API example.

## Host Design

The Host performs the following sequence:

1. Initialize ACL and select device 0.
2. Create one stream.
3. Allocate two 8-byte Device buffers.
4. Launch one Vector Core with zero dynamic UB.
5. Synchronize the stream.
6. Copy both values to Host variables.
7. Print `entry_cycle`, `current_cycle`, and `delta`.
8. Release resources and finalize ACL.

Every ACL call is checked. Cleanup runs for resources that were successfully created.

## Output

Runtime stdout and stderr are redirected to `compiler_cycle_minimal/cycle_run.log`. Expected output shape:

```text
entry_cycle: <uint64>
current_cycle: <uint64>
delta: <current_cycle - entry_cycle>
```

The run succeeds only if both cycles are nonzero and `current_cycle >= entry_cycle`.

## Verification

- Confirm the verbose compile command targets `dav-2201` and does not contain `ASCENDC_TIME_STAMP_ON`.
- Confirm the preprocessed Kernel contains direct calls to `__get_entry_sys_cnt()` and `get_sys_cnt()`.
- Run on the real NPU, not simulator/model mode.
- Confirm process exit code 0 and inspect the redirected output file.
- Confirm the Device Kernel source contains none of the excluded framework APIs.
