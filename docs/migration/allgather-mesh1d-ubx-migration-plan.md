# AllGather Mesh1D UBX Migration Plan

## Scope

- Source: hccl master at ef2c77e73f0a809c32c57e163eb087ee7c70520f.
- Target: asc-devkit_InsAllGatherMesh1D_UBX_Optimise master at c78224733f267ff223e56b4c871b2f6aad16bb94.
- Op: HCCL_CMD_ALLGATHER.
- Runtime: AICPU, UBX, MESH_1D_CLOS, single level, no PCIe mix, mesh count equal to CLOS count, 4P.
- Target algorithm mapping: InsAllGatherMesh1D -> AicpuAllGatherSoleMesh.

## Performance Goal

Improve the forced Mesh1D path for large-data UBX benchmarks by using all valid peer channels and reducing per-repeat communication submission overhead. Default selector behavior must be measured separately: for MESH_1D_CLOS 4P, HCCL selects AicpuAllGatherConcurMeshNHR above 512 KB and AicpuAllGatherSoleMesh at or below 512 KB.

## Approved Changes

1. Rename the Mesh1D registration, selector return, algorithm-specific symmetric-memory checks, and in-repository tests to AicpuAllGatherSoleMesh. Preserve an old-name alias only if an external forced-alg contract is demonstrated.
2. Port the HCCL multi-channel Mesh execution: calculate min/max channels, allocate resources for the maximum, use the minimum uniformly at runtime, and dispatch one worker per active channel.
3. Port Port Group proportional slicing, including connected-rank tail-size selection, input-HCCL-buffer scratch stride handling, and per-channel offsets.
4. Port RX-only SendRecvBatchRead. Keep the existing target wrapper fallback to SendRecvRead when batch transfer is unavailable.
5. Restrict symmetric-memory loop expansion to AicpuAllGatherSoleMesh, keep using the target OpParam support flag (do not add the HCCL-only TemplateDataParams field), and use the remote output window only in the Mesh read path.
6. Propagate LocalCopy errors with CHK_RET.

## Target Compatibility Constraints

- Keep the target KernelRun signature using const TemplateResource&; do not widen the shared ABI for the HCCL cost-model implementation.
- Preserve target outputSliceStride handling for strideCount_ and non-contiguous output.
- Do not import HCCL cost-model/TemplateProp infrastructure, global selector changes, or shared channel.cc die de-duplication.
- Do not change skipCclCopy semantics in the first pass; it requires separate input/scratch validation.

## Correctness Gates

- Verify 4P rank order and channel order agreement across all peers.
- Verify 1/2/3/4 active channels, uneven peer channel counts, portGroupSize ratios, aligned and tail blocks, repeatNum greater than one, and zero-sized slices.
- Verify user input/output, HCCL scratch input/output, remote-memory access on/off, symmetric memory on/off, and strideCount_ paths.
- Verify batch-transfer unsupported fallback produces the old SendRecvRead behavior.
- Verify registration reverse lookup, selector boundary at 512 KB, forced Mesh above 512 KB, and no unregistered algorithm name.

## Performance Gates

- Record actual algName, engine, topology, channel counts, portGroupSize, batch-transfer capability, loop count, and per-channel transfer time.
- Compare target before/after and HCCL with identical forced algorithm and identical data layout, then separately compare default selector results.
- Reject the change if the forced Mesh path regresses on one-channel UBX or if multi-channel throughput does not improve when multiple valid channels are present.

## Status

User confirmed migration of the P0/P1 list. Code migration is now authorized; post-migration audit and hardware performance verification remain required.

## Implementation Results

- Updated the target Mesh1D registration and selector name to `AicpuAllGatherSoleMesh`, matching the current HCCL name.
- Ported min/max channel accounting: resources are requested for the maximum channel count, while communication uses the minimum count shared by every peer.
- Ported per-channel dispatch and proportional Port Group slicing, including tail blocks, HCCL-buffer input stride handling, and `strideCount` output layout.
- Ported RX-only `SendRecvBatchRead`; the target wrapper retains its `SendRecvRead` fallback when batch transfer is unavailable.
- Kept the target `KernelRun(..., const TemplateResource&)` ABI and target-specific output stride behavior.
- Kept the symmetric-memory fast path restricted to `AicpuAllGatherSoleMesh`.
- Added `CHK_RET` propagation for Mesh local copies.
- Kept the rank-size-1 path valid when the executor has no remote channel map; the
  executor only refreshes channel counts when a non-empty map is available.

## Verification

- `git diff --check`: passed.
- Registration and selector reverse lookup: statically checked for `AicpuAllGatherSoleMesh`; no stale `InsAllGatherMesh1D` remains in target source or tests.
- CMake source wiring: Mesh1D executor/template sources and the AICPU algorithm test target are already connected.
- Full CANN compilation and Ascend 4P UBX correctness/performance runs are pending. The current Windows environment has no CANN headers/toolchain; the attempted syntax check stops at the missing `ccu_datatype_v1.h` include.

Hardware validation must compare the old and migrated forced Mesh path with identical 4P UBX topology, data layout, algorithm name, channel map, and batch-transfer capability. At minimum, measure one-channel and multi-channel cases separately and reject the migration if the one-channel path regresses or multi-channel throughput does not improve.
