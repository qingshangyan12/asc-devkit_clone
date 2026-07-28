# SIMD and SIMT Hybrid Programming

This context defines the domain terms used by SIMD and SIMT hybrid programming samples.

## Language

**SIMT**:
A thread-level, CUDA-like single-instruction multiple-thread programming model.
_Avoid_: SIMD

**SIMD**:
A vector-level single-instruction multiple-data programming model.
_Avoid_: SIMT

**VF**:
A Vector Function marked as either `__simt_vf__` or `__simd_vf__`.
_Avoid_: kernel entry

**SIMT VF**:
A thread-level Vector Function marked with `__simt_vf__` and invoked through `asc_vf_call`.
_Avoid_: SIMD VF, warp

**SIMD VF**:
A vector-level Vector Function marked with `__simd_vf__` and invoked through `asc_vf_call`.
_Avoid_: SIMT VF

**MainScalar**:
The scalar control context of a `__global__ __vector__` mixed-programming kernel entry.
_Avoid_: Host, SIMT VF, SIMD VF

**Warp**:
A group of 32 SIMT threads executing on the same AIV core.
_Avoid_: VF, slot

## Relationships

- A mixed-programming kernel's **MainScalar** context invokes **SIMT VF** and **SIMD VF** through `asc_vf_call`.
- A **SIMT VF** executes work across one or more **Warp** groups.

## Flagged ambiguities

- "VF" can refer to either **SIMT VF** or **SIMD VF**; use the qualified term when the distinction matters.
