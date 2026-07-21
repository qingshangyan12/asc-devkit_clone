# SIMD-VF Dump CPU Fallback Design

## Context

`utils/debug/asc_dump.h` declares the four SIMD-VF dump overloads in all
build modes. Their definitions in `asc_simd_dump_impl.h`, however, are wholly
excluded when `ASCENDC_CPU_DEBUG` is defined. A CPU Debug caller therefore
compiles but fails to link with an undefined `__asc_simd_vf::asc_dump` symbol.

The AICore dump implementation already handles this case differently: its CPU
branch supplies inline fallback definitions that assert that dump is not
supported. SIMD-VF dump should provide the same source, link, and runtime
behavior.

## Goals

- Make all four SIMD-VF dump overloads link successfully in CPU Debug builds.
- Match the existing AICore CPU behavior: calling an overload fails with a
  clear `not supported in cpu mode` assertion.
- Preserve existing NPU and simulator implementations and behavior.
- Add regression coverage at the link and runtime-behavior seams.

## Non-Goals

- Do not implement real SIMD-VF dump output in CPU Debug mode.
- Do not change `ASCENDC_DUMP` defaults or CMake run-mode behavior.
- Do not change the public declarations in `asc_dump.h`.
- Do not refactor unrelated debug APIs.

## Implementation

Extend the existing `#ifndef ASCENDC_CPU_DEBUG` conditional in
`impl/utils/debug/asc_simd_dump_impl.h` with an `#else` branch. In namespace
`__asc_simd_vf`, define CPU fallbacks for:

- `asc_dump_reg<T, U>(U&, uint32_t, uint32_t)`
- `asc_dump_ubuf<T>(__ubuf__ T*, uint32_t, uint32_t)`
- `asc_dump<T, U>(U&, uint32_t, uint32_t)`
- `asc_dump<T>(__ubuf__ T*, uint32_t, uint32_t)`

Keep each function's signature and `__simd_callee__ inline` qualifiers aligned
with its public declaration. Each body uses the same assertion style as the
AICore CPU fallback and identifies the invoked API in the message, for example:

```cpp
assert(false && "asc_dump_reg is not supported in cpu mode.");
```

The CPU branch does not include or invoke NPU dump helpers, reserve UB space,
or write TLV records.

## Error Behavior

CPU Debug compilation and linking succeed because every public overload has a
definition. Executing any of the four functions terminates through the existing
assertion mechanism. In builds where standard assertions are disabled, behavior
matches the existing AICore fallback and the function returns without dumping.

## Testing

Add a focused CPU unit test source under the Ascend 950PR/9599 AIV basic test
suite. Instantiate and call all four overloads so the test executable provides
link-time regression coverage. When assertions are enabled, use death tests to
verify that each overload fails with `not supported in cpu mode`.

Verification also includes:

1. Build and run the focused Ascend 950PR/9599 AIV basic CPU tests.
2. Rebuild the standalone `/home/czj/ZJ/0720` reproducer in CPU mode and verify
   that the former undefined symbol is gone.
3. Run the existing SIMD-VF dump unit coverage to ensure NPU-side TLV behavior
   is unchanged.
4. Run the relevant header checker targets for CPU and NPU configurations.

## Expected Outcome

SIMD-VF dump has the same CPU contract as AICore dump: the API remains
available and linkable, but an actual CPU Debug invocation reports that the
operation is unsupported.
