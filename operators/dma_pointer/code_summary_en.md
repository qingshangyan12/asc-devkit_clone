# Code Summary

Operator: DMA Pointer API extension  
Function: Add native address-space pointer overloads for `DataCopy`, `DataCopyPad`, `DataCopyL1ToUB`, and cache APIs on DAV_3510 while keeping the Tensor overloads unchanged.  
Side: Kernel

## Code Path

The user kernel calls `AscendC::DataCopy`, `DataCopyPad`, `DataCopyL1ToUB`, `DataCachePreload`, or `DataCacheCleanAndInvalid`. Public headers resolve the overload, and the DAV_3510 implementation forwards the native pointers to the architecture DMA or cache primitive.

The pointer data path is:

`__gm__` ↔ `__ubuf__` / `__cbuf__` / `__cc__` pointer → pointer overload → `DataCopy*Impl` / `DataCopyPad*Impl` / DCCI implementation → MTE1/MTE2/MTE3 or cache instruction.

The pointer path does not construct `LocalTensor` or `GlobalTensor`, so the address-space type determines the route. It does not carry Tensor position, queue event, capacity, or lifetime metadata.

## Main Branches

| Branch | Meaning | Handling |
|---|---|---|
| `__NPU_ARCH__ == 3510` | DAV_3510 build | Expose and compile pointer overloads |
| `dimValue` and `SliceInfo` validation | Pointer Slice input safety | Reject invalid dimensions, shapes, ranges, strides, burst length, or overflow |
| Source and destination Slice count comparison | Offset-list consistency | Return before DMA if counts differ |
| 32-byte alignment | Fast regular DMA path | Convert stride fields to the hardware block units |
| `PaddingMode` | Normal or Compact padding | Select the corresponding DataCopyPad implementation |
| `enableSmallC0` | ND2NZ/DN2NZ mode | Select the small-C0 implementation at compile time |
| `ASCEND_IS_AIC` | Core capability restriction | Keep unsupported AIC paths from issuing the instruction |

## Pointer Slice Validation

`CheckDataCopyPointerSliceInfo` is shared by GM→UB and UB→GM pointer Slice paths. It performs the following checks before offset-list generation:

1. `dimValue` is nonzero and does not exceed `K_MAX_SHAPE_DIM`.
2. `shapeValue` is nonzero, start and end indices are ordered, and end indices are within the shape.
3. Stride and span calculations remain within the supported integer range.
4. Shape-element and Slice-count multiplications cannot overflow the supported index range or `MAX_SLICE_SIZE`.
5. The first-dimension burst length and byte-stride calculations are valid.
6. Source and destination offset-list sizes are equal.

Invalid input triggers `ASCENDC_ASSERT` and returns before issuing a DMA operation.

## API Inventory

| API family | Pointer path |
|---|---|
| Basic DataCopy | GM↔UB, UB↔UB, GM/L1/L0C variants supported by DAV_3510 |
| Format conversion | ND2NZ, DN2NZ, NZ2ND |
| Slice | GM→UB and UB→GM with explicit `SliceInfo` arrays |
| Enhanced DataCopy | `DataCopyEnhancedParams` and CO1 destination parameters |
| DataCopyPad | GM→UB/L1, UB→GM/L1 with normal and compact modes |
| DataCopyL1ToUB | Count and `DataCopyParams` forms |
| NDDMA | `MultiCopyParams` / compatible NDDMA parameter forms |
| Cache | `DataCachePreload` and `DataCacheCleanAndInvalid` pointer forms |

## Files Changed

- `include/basic_api/kernel_operator_data_copy_intf.h`: public DataCopy declarations and architecture exposure.
- `impl/basic_api/kernel_operator_data_copy_intf_impl.h`: Tensor implementation conditions and DAV_3510 integration.
- `impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h`: pointer overload forwarding and Slice validation.
- `tests/api/basic_api/ascendc_header_checker/kernel_operator_data_copy_intf.cpp`: compile-time API coverage and corrected L0C calls.

## Test Coverage

The header checker instantiates the DataCopy, DataCopyPad, format-conversion, Slice, Nd2Nz, NDDMA, and cache pointer families. The Ascend950PR validation also executes pointer Pad, pointer Nd2Nz, and pointer Slice paths on a real DAV_3510 device and compares the results with golden data.

The official cache examples validate the runtime cache path. `data_cache_preload` and `gm_by_dcache` both report `Output: 20256` and `Golden: 20256`.

## Constraints and Caller Responsibilities

- Use the correct compiler address-space qualifier; arbitrary casts do not prove physical buffer placement.
- Follow the documented alignment, block, stride, padding, and address-span limits for each transfer family.
- Keep source and destination Slice arrays consistent with `dimValue` and populate `shapeValue` explicitly for pointer calls.
- Use pipeline barriers or queue synchronization when destinations overlap or when data is exchanged through a TQue pipeline.
- Call `NdDmaDci()` according to the NDDMA cache-coherency sequence.
- Do not treat cache maintenance as a cross-core synchronization primitive.

## Compatibility Notes

The pointer overloads are enabled for the DAV_3510 NPU compilation path. `ASCENDC_CPU_DEBUG` erases the hardware address-space distinctions, so pointer overload visibility is gated there to avoid duplicate or ambiguous overloads. Existing Tensor overloads remain available.
