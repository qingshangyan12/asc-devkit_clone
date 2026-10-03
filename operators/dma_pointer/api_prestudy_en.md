# API Pre-study Report

## Chip Generation

- Target generation: DAV_3510 (`__NPU_ARCH__ == 3510`), corresponding to Ascend 950PR / Ascend 950DT (Atlas A5).
- The new declarations and implementations are guarded by the DAV_3510 architecture condition. The related tests are under `ascendc_case_ascend950pr_9599`.
- DAV_3510 provides 248 KB UB, 256 KB L0C, 4 KB BT, and a CubeCore:VectorCore ratio of 1:2. This change covers native address-space pointers (`__gm__`, `__ubuf__`, `__cbuf__`, `__cc__`), UB/L1 transfers, L0C transfers, NDDMA, and cache APIs.
- DAV_3510 does not provide the L1-to-GM hardware path. The L1-to-GM overloads remain a compatibility surface and must not be considered supported only because they can be instantiated.

## APIs Found in the Code

- Data movement: `DataCopy`, `DataCopyPad`, `DataCopyL1ToUB`.
- Cache and control APIs: `DataCachePreload`, `DataCacheCleanAndInvalid`, `ICachePreLoad`, `SetLoopModePara`, `ResetLoopModePara`, and `NdDmaDci`.
- Internal implementation helpers include `DataCopy*Impl`, `DataCopyPad*Impl`, `DataCopyCheck`, `DataCopyGetOffsetList`, `DcciGMImpl`, `DcciUBImpl`, and `DataCachePreloadImpl`. These are implementation details and should not be called directly by applications.
- The change does not introduce vector math, reduction, tensor allocation, or queue management APIs.

## DataCopy

- DAV_3510 supports the implemented GM/UB/L1 transfer, ND2NZ, DN2NZ, NZ2ND, Slice, and NDDMA paths according to the corresponding architecture documentation.
- The change adds native pointer overloads alongside the existing Tensor overloads. Supported forms include GM↔UB, GM/UB↔L1, L0C paths, ND2NZ/DN2NZ/NZ2ND, Slice, enhanced parameters, CO1 destination parameters, and NDDMA.
- GM addresses must satisfy the natural alignment requirements of the element type. UB and L1 addresses generally require 32-byte alignment. L0C NZ sources commonly require 64-byte alignment.
- Continuous `count` transfers require `count * sizeof(T)` to satisfy the hardware DataBlock alignment rules.
- `DataCopyParams` fields use the architecture-defined block and gap units and ranges. A zero block count or block length is treated as a no-op on DAV_3510.
- Pointer Slice calls must explicitly provide `SliceInfo::shapeValue`, because a raw pointer does not carry Tensor shape metadata. The implementation validates dimensions, shapes, ranges, strides, burst length, integer bounds, and source/destination Slice count equality before generating offset lists.
- ND2NZ/DN2NZ/NZ2ND fields must follow the official architecture ranges. NDDMA users must respect the 40-bit address span and call `NdDmaDci()` before sharing updated GM data across cores when required.
- DataCopy does not replace queue or pipeline synchronization. Overlapping destinations require the appropriate pipeline barrier and callers remain responsible for address-space, capacity, and lifetime correctness.

## DataCopyPad

- Ascend 950PR/DT supports the documented GM→UB, UB→GM, GM→L1, and UB→L1 paths.
- The pointer overloads preserve the Tensor API parameter forms, including extended parameters and `PaddingMode`.
- GM and UB alignment, padding width, block length, stride ranges, and the DAV_3510 40-bit stride rules must follow the architecture documentation.
- A zero block count or block length is a no-op. Compact mode ignores the fields documented as inactive for that mode.

## DataCopyL1ToUB

- This public API is supported for the Ascend 950PR/DT Mix configuration with a 1:2 Cube:Vector ratio.
- L1 and UB addresses follow the 32-byte alignment requirement. `subBlockId` is limited to the documented values.

## Cache Control

### DataCachePreload

- Ascend 950PR/DT supports preloading one Cache Line into DCache.
- The pointer overload accepts a `__gm__ uint64_t*` source and a byte offset while preserving the existing Tensor form.
- The source type and offset type must follow the documented restrictions. Excessive consecutive preloads may congest the reservation station.

### DataCacheCleanAndInvalid

- Ascend 950PR/DT supports the documented cache-line modes, including `CACHELINE_ATOMIC`.
- GM and UB pointer overloads route to the corresponding DCCI implementation. `SINGLE_CACHE_LINE` operates on a 64-byte line; full-cache mode has a larger latency cost.
- Cache maintenance is not a cross-core synchronization primitive. Callers must avoid conflicting writes to the same cache line.

## Memory and Synchronization

- DAV_3510 supports the standard TPipe/TBufPool allocation model. The validation kernels also use `LocalTensor` address helpers to construct test pointers; those helpers do not provide production queue lifetime semantics.
- The pointer overloads do not carry Tensor position, queue event, capacity, or lifetime metadata. Callers must provide valid addresses and synchronization.
- `NdDmaDci()` must precede NDDMA accesses when the documented cache invalidation sequence is required.

## Generation-specific APIs

- NDDMA and `NdDmaDci` are generation-specific interfaces and should be guarded by the appropriate architecture condition.
- The native address-space qualifiers are part of overload resolution. A normal C++ pointer or an arbitrary cast does not prove that the underlying buffer belongs to the required physical memory space.
- This change does not use RegTensor, MaskReg, SIMT, or SIMD-RegBase APIs.

## Sunset API Check

The CANN sunset API scan did not find `aclrt*`, `aclnn*`, `acl.op.*`, deprecated `op_proto/inc` paths, or `libopapi.so` links in the changed files.

## Reference Sources

- AscendC DataCopy, DataCopyPad, cache-control, and resource-management documents in the repository.
- DAV_3510 hardware capability and generation mapping references.
- The CANN sunset API list used by the AscendC code-review tooling.
