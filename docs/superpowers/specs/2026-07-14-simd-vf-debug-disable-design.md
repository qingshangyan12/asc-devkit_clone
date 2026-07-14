# SIMD VF Debug Disable Design

## Goal

When `ASCENDC_DUMP` is `0`, keep the SIMD VF debug APIs source-compatible while removing their debug setup, teardown, reserved-UB requirements, and actual dump/printf processing at preprocessing time.

Strict removal of argument evaluation is out of scope. Disabled API definitions remain empty inline functions.

## Design

Treat `ASCENDC_DUMP` as a numeric `0` or `1` switch and preserve the existing behavior that an undefined switch enables debugging by default.

Guard the `SimdVfDebugTraits<SimdVfDebugTag>` enabling specialization with `#if ASCENDC_DUMP`. With the switch disabled, the primary trait remains `false`, so `AscVFCallImpl` does not instantiate `AscVFDebugInitUb` or `AscVFDebugTransferUb`.

In the SIMD VF dump and printf implementations, use an outer `#if ASCENDC_DUMP` branch:

- The enabled branch contains the current reserved-UB attributes, debug utility dependencies, TLV construction, data movement, diagnostics metadata, and real API implementations.
- The disabled branch contains only API-compatible empty inline definitions, without reserved-UB attributes or debug utility dependencies.

Declarations remain available in both modes so existing source code continues to compile.

## Compatibility

- `ASCENDC_DUMP=1`: existing dump and printf behavior is unchanged.
- `ASCENDC_DUMP=0`: SIMD VF debug calls compile as empty inline functions.
- Undefined `ASCENDC_DUMP`: behavior remains enabled for compatibility.
- Legacy empty definitions of `ASCENDC_DUMP` must be changed to the numeric value `1` where they are in scope for the new `#if ASCENDC_DUMP` checks.

## Verification

Extend the SIMD VF header-checker coverage to verify both modes:

- Enabled mode keeps `SimdVfDebugTraits` enabled and instantiates the real dump and printf APIs.
- Disabled mode keeps the trait disabled while the empty dump and printf APIs remain instantiable.
- Disabled mode compiles without requiring reserved UB.

Run the focused header-checker target for the 3510 architecture and inspect generated code or symbols as available to confirm that disabled builds contain no debug init/transfer or TLV/printf processing calls.
