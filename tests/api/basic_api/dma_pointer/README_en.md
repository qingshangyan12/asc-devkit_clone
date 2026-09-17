# DMA Pointer Tests

[中文](README.md)

The compile driver instantiates public DMA APIs using the ASC compiler's real GM,
UB, L1, L0C, Bias and Fixpipe address spaces. These compile checks do not execute
kernels. Separate runtime executables below check selected device scenarios.

Run from the repository root with the CANN environment activated:

```bash
python3 tests/api/basic_api/dma_pointer/check_dma_pointer.py \
    --compiler bisheng --extras --output /tmp/dma-pointer-results
```

The output directory must be new or empty. It contains each generated translation unit, its compiler
log including the header trace, and `results.json` with exact commands and source
hashes. Before/after hashes of Basic API headers and the driver detect source
changes during a run and invalidate the aggregate result. `--rows 21,31 --forms TT,PP`
selects a smaller diagnostic set; `--jobs 4` opts into compiler concurrency.
The default uses one compiler worker, and every case has an isolated directory.

Rows 1 through 44 correspond to the official DMA inventory in order:
DataCacheCleanAndInvalid (1-3), DataCachePreload (4), DataCopy (5-31),
DataCopyL1ToUB (32-33), DataCopyPad (34-44). TT is Tensor/Tensor, PP is
Pointer/Pointer, TP and PT are mixed destination/source forms. Cache APIs use
T/P. Row 34 uses TensorTrait/primitive padding and tests its TT/TP/PT forms;
pure pointers erase Trait identity and use the ordinary row 35 entry instead.
The extra `LP-C05-explicit-trait-pointer` case separately supplies
`<TensorTrait<float>, float>` to exercise the bridge with two pointers.

The test selects 2201 for historical rows 5, 19, 39-42 and 3510 for other
rows. A compile result on 2201 does not establish 950 support. In particular,
the Enhanced routes in rows 6-11 remain unsupported by the 3510 implementation;
their 3510 tests establish signature binding only.
Each row tests one selected route and type pair; this is declaration coverage,
not exhaustive dtype, layout, parameter-boundary or device-route coverage.

The extra cases check array decay, automatic type deduction, const sources,
rejection of const destinations, wrong address spaces, ordinary Host pointers,
mixed element types, SmallC0 template options and invalid Preload offset types.
A negative case passes only with a constraint/overload diagnostic and a passing
positive control. Include failures are not accepted as successful negatives.

Each case includes its corresponding standalone public DMA header and uses the supported
`ASCENDC_DUMP=0` build setting and verifies their worktree paths in `-H` output.
It rejects installed-SDK Basic API headers in the trace. Add `--umbrella` to
check the full `kernel_operator.h` graph. The default does not replace the
official Header Checker, debug-enabled builds or repository test parts.

The CPU test `test_dma_pointer_cache_compat.cpp` is discovered by the existing
950 AIV_BASIC source glob (`basic_test_four`). It checks the old public Tensor
Preload wrapper's offset type and pointer forwarding with mockcpp, including
indexed tensors and const handles. Pointer overloads are excluded in CPU Debug
because Host address spaces are erased. CPU mocks do not validate NPU cache
effects.

## Runtime Smoke Test

Build the runtime test with the CANN ASC CMake package available in the active
environment:

```bash
cmake -S tests/api/basic_api/dma_pointer -B /tmp/dma-pointer-runtime-build
cmake --build /tmp/dma-pointer-runtime-build -j2
/tmp/dma-pointer-runtime-build/dma_pointer_runtime --device DEVICE_ID
/tmp/dma-pointer-runtime-build/dma_pointer_cache --device DEVICE_ID
/tmp/dma-pointer-runtime-build/dma_pointer_l1toub --device DEVICE_ID
```

Replace `DEVICE_ID` with an explicitly selected available device. If CMake
cannot locate ASC, pass `-DASC_DIR=<CANN package>/lib64/cmake` using the active
installation's actual path. The target defaults to `dav-3510`, uses standalone
public DMA/synchronization headers, and prioritizes this worktree's headers
with `-iquote`. It uses `ASCENDC_DUMP=0`; this does not validate debug-enabled
builds or replace the complete Header Checker.

For a `dav-2201` device, select the architecture explicitly and use a separate
build directory:

```bash
cmake -S tests/api/basic_api/dma_pointer -B /tmp/dma-pointer-runtime-2201 \
    -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build /tmp/dma-pointer-runtime-2201 -j2
/tmp/dma-pointer-runtime-2201/dma_pointer_runtime --device DEVICE_ID
```

The selected CANN compiler must support the worktree baseline and target.
Results on `dav-2201` do not establish 950 behavior; record the actual device,
compiler and executable hash with each run. The runtime scenarios use Normal
padding only and do not require the 950-only Compact mode.

The transfer runner executes 240 cases: lengths 1, 5, 8, 16, 20, 31, 32, 59,
1024 and 2048, three rounds with changed deterministic inputs, count and
nonaligned Pad transfers, and TT/PP/TP/PT forms. It compares every
output uint32 exactly with a host oracle and the corresponding Tensor result.
UB/GM canaries and defined padding are checked. ACL initialization, allocation,
copies, synchronization, and cleanup errors cause a nonzero exit status.
The kernel initializes SoC state explicitly and serializes guard initialization
and input DMA with a MTE2 barrier because their destinations overlap. It also
completes its pending pipelines before its local array expires;
the API adapters themselves retain asynchronous semantics.

When no device is available, the same executable can be attempted with the
950 simulator. Use the `dav-3510` build; the simulator requires device 0:

```bash
cannsim record /tmp/dma-pointer-runtime-build/dma_pointer_runtime \
    -s Ascend950 -o /tmp/dma-pointer-simulator -u '--device 0'
```

Use fresh build/evidence directories for independent runs. A successful build
does not establish runtime correctness. Runtime success requires process exit
0 and `ALL_PASS cases=240 comparison=exact_u32_with_canaries_and_tensor_diff`; interrupted or
incomplete simulator output must not be counted as passing. This smoke test
does not establish full 44-overload device coverage or performance.

## Cache Runtime Test

`dma_pointer_cache` uses an explicitly Vector kernel and runs 48 cases across
three changed-input rounds. It checks the GM three-template and two-template
DCCI overloads, UB DCCI, and Preload with int16/int64 offsets 0/64, using
Tensor/Pointer forms and SINGLE/ENTIRE cache modes where applicable.
Scalar writes are flushed before observation; the host checks uint64 payload,
prefix/suffix guards and Tensor/Pointer equality. Success requires exit 0 and
`ALL_PASS cases=48 comparison=exact_u64_cache_visibility`.

The UB case first initializes backing memory to a different payload, so data from
a preceding test cannot mask a missing writeback. This suite checks the stated
cache visibility/use scenarios, not cache hit rates or performance.

## L1ToUB Runtime Test

`dma_pointer_l1toub` runs a Mix1:2 kernel using the documented AIC MTE1 producer and
mode2 cross-core completion flag. Both AIVs consume the notification, and only the
selected AIV copies its UB data back. There are 48 cases: three input rounds,
subBlockId 0/1, count/strided Params, and TT/PP/TP/PT forms.

Before each tested transfer, the old Tensor path fills the selected peer UB with
canary data and completes MTE1. The test then observes the whole UB payload,
including strided holes, plus GM guards and the unselected peer's output slot.
Success requires exit 0 and `L1TOUB_RUNTIME_PASS cases=48`.

The Cache and L1ToUB runtime targets are built only for dav-3510. These scenarios
do not replace the complete DMA route/dtype/layout matrix or performance testing.
