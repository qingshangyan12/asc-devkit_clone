# OOM Non-Contiguous Tensor Support Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make an OOM-enabled AscendC binary advertise and consume the version-1 trailing storage-shape extension while retaining its legacy OOM argument interpretation when the extension is absent.

**Architecture:** The compiler writes `oom.version` and `oom.tensor_version` into OOM-enabled single-operator JSON and generates registration calls that pass the legacy OOM-tail cursor. Device helpers inspect the optional header at that cursor; `magic == 1` selects separately parsed single-Tensor or TensorList descriptors, otherwise the generated kernel retains the existing length and second-level-pointer logic. Runtime/`aclnn` construction of the extension is out of scope for this repository.

**Tech Stack:** Python AscendC code generation, generated C++17 AscendC device code, gtest, Python unittest.

---

## File Structure

- Modify `tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py`: emit capability-aware Tensor and TensorList registration while preserving legacy fallback.
- Modify `tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py`: mirror primary generator behavior.
- Modify `tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py`: publish the OOM capability in single-operator JSON.
- Modify `tools/build/asc_op_compile_base/adapter/compile_op.py`: mirror JSON behavior.
- Modify `impl/basic_api/utils/kernel_utils_macros.h`: define the version-1 device ABI constants.
- Modify `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`: parse ordinary Tensor and TensorList descriptors independently.
- Modify `tests/python/asc_op_compiler/test_compile_op.py` and `tests/python/adapter/test_compile_op.py`: test generated source and JSON.
- Modify `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp`: test device parsing and legacy fallback.

### Task 1: Add failing compiler metadata and source tests

**Files:**
- Modify: `tests/python/asc_op_compiler/test_compile_op.py`
- Modify: `tests/python/adapter/test_compile_op.py`

- [ ] **Step 1: Test the OOM JSON capability**

Add an OOM case asserting:

```python
self.assertEqual(js["oom"], {"version": 1, "tensor_version": 1})
```

Add a non-OOM case asserting:

```python
self.assertNotIn("oom", js)
```

- [ ] **Step 2: Test generated ordinary-Tensor and TensorList source**

Extend an operation fixture to contain one ordinary Tensor and one dynamic TensorList. Assert the generated source contains all four calls:

```python
self.assertIn("OOMTryRegisterTensorWithStorageShape", source)
self.assertIn("OOMTryRegisterTensorListWithStorageShape", source)
self.assertIn("OOMCheckAddrRange", source)
self.assertIn("OOMCheckTensorListRange", source)
```

The last two calls prove the legacy fallback remains in the same binary.

- [ ] **Step 3: Mirror the tests for the adapter package**

Use adapter imports and the same assertions so both generators expose an identical ABI.

- [ ] **Step 4: Run the focused tests and verify red state**

```bash
python3 -m pytest tests/python/asc_op_compiler/test_compile_op.py -k 'oom or add_op_param_to_workspace' -q
python3 -m pytest tests/python/adapter/test_compile_op.py -k 'oom or add_op_param_to_workspace' -q
```

Expected: FAIL because the capability and storage-shape helper calls are absent.

- [ ] **Step 5: Commit the red tests**

```bash
git add tests/python/asc_op_compiler/test_compile_op.py tests/python/adapter/test_compile_op.py
git commit -m "test: cover OOM storage-shape generation"
```

### Task 2: Define and parse the device-side extension ABI

**Files:**
- Modify: `impl/basic_api/utils/kernel_utils_macros.h`
- Modify: `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`
- Modify: `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp`

- [ ] **Step 1: Add a failing single-Tensor descriptor test**

Construct this synthetic GM buffer:

```text
[u64 magic=1][u32 version=1][u32 reserved]
[u64 size=40][u32 kind=1][u32 reserved]
[u32 tensor_version=1][u32 nDim=2][u64 2][u64 8]
```

Parse it for a float32 Tensor and assert one range of `2 * 8 * 4` bytes is registered. Test a buffer without magic and assert parsing returns false without changing `g_oomAddrArange`.

- [ ] **Step 2: Add failing TensorList U/F tests**

Construct a `kind=2`, `count=2` block with shapes `{2, 8}` and `{3, 4}` and verify the two derived byte lengths. Repeat with both supported `flag` values for U and F mode. Do not alter or inspect the existing second-level-pointer storage in this test.

- [ ] **Step 3: Add malformed extension tests**

Assert rejection without range registration for unsupported header version, undersized block, invalid kind, unsupported TensorView version, shape bytes beyond block size, multiplication overflow, and TensorList count beyond remaining block bytes.

- [ ] **Step 4: Run the OOM gtest target and verify red state**

Use the repository's basic-api test build command for the target containing `test_dma_oom.cpp`, then execute that test binary.

Expected: compile failure because the new parser entry points do not exist.

- [ ] **Step 5: Add ABI constants**

Under `ASCENDC_OOM` in `kernel_utils_macros.h`, add:

```cpp
constexpr uint64_t OOM_STORAGE_SHAPE_MAGIC = 1UL;
constexpr uint32_t OOM_STORAGE_SHAPE_VERSION = 1U;
constexpr uint32_t OOM_TENSOR_VIEW_VERSION = 1U;
constexpr uint32_t OOM_DESC_KIND_TENSOR = 1U;
constexpr uint32_t OOM_DESC_KIND_TENSOR_LIST = 2U;
```

- [ ] **Step 6: Implement bounded parser helpers**

In `kernel_utils_ceil_oom_que.h`, add fixed-width reads and checked multiplication, then expose:

```cpp
__aicore__ static inline bool OOMHasStorageShapeHeader(__gm__ uint8_t* tail);

template <typename T>
__aicore__ static inline bool OOMTryRegisterTensorWithStorageShape(
    __gm__ uint8_t*& cursor, __gm__ T* tensorAddr);

template <typename T>
__aicore__ static inline bool OOMTryRegisterTensorListWithStorageShape(
    __gm__ uint8_t*& cursor, __gm__ T* tensorListAddr, const int inputSize);
```

The ordinary-Tensor helper validates `kind=1`, computes `product(storage_shape) * element_size`, calls `OOMCheckAddrRange`, and advances by block `size`. The TensorList helper validates `kind=2`, reads `flag` and `count`, uses the unchanged second-level-pointer block only to obtain each Tensor address, applies each appended `TensorViewDesc` storage shape, registers each range, and advances by block `size`.

- [ ] **Step 7: Run the device tests and verify green state**

Run the same target from Step 4.

Expected: existing OOM tests plus single Tensor, TensorList U/F, no-magic fallback, and malformed-descriptor tests pass.

- [ ] **Step 8: Commit the device ABI/parser**

```bash
git add impl/basic_api/utils/kernel_utils_macros.h impl/basic_api/utils/kernel_utils_ceil_oom_que.h tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp
git commit -m "feat: parse OOM storage-shape descriptors"
```

### Task 3: Generate magic-selected OOM registration

**Files:**
- Modify: `tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py`
- Modify: `tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py`
- Modify: `tests/python/asc_op_compiler/test_compile_op.py`
- Modify: `tests/python/adapter/test_compile_op.py`

- [ ] **Step 1: Preserve the legacy OOM cursor and locate the new extension**

Refactor generation so `tmpTilingSizeForOOM` still walks every existing input/output/output-shape/workspace length field. After consuming the existing workspace field, emit:

```cpp
__gm__ uint8_t* oomStorageShapeCursor =
    (__gm__ uint8_t*)tiling + tmpTilingSizeForOOM + 8;
bool hasOomStorageShape = AscendC::OOMHasStorageShapeHeader(oomStorageShapeCursor);
if (hasOomStorageShape) {
    oomStorageShapeCursor += 2;
}
```

The additional `8` accounts for the legacy workspace-length field, which the current generator reads without advancing afterward. Emit this only for dynamic kernels with `GM_ADDR tiling`; static kernels keep their current registrations.

- [ ] **Step 2: Emit ordinary-Tensor dynamic selection**

For every non-dynamic input/output in registration order, generate:

```cpp
if (!hasOomStorageShape ||
    !AscendC::OOMTryRegisterTensorWithStorageShape(oomStorageShapeCursor, tensor)) {
    AscendC::OOMCheckAddrRange(tensor, legacyLength);
}
```

Preserve the exact existing `legacyLength` expression read from `tiling + tmpTilingSizeForOOM`.

- [ ] **Step 3: Emit TensorList dynamic selection**

For every `param_type_list` entry equal to `dynamic`, generate:

```cpp
if (!hasOomStorageShape ||
    !AscendC::OOMTryRegisterTensorListWithStorageShape(
        oomStorageShapeCursor, tensorList, packedElementSize)) {
    AscendC::OOMCheckTensorListRange(tensorList, packedElementSize);
}
```

This preserves the current TensorList parser as the fallback and leaves its second-level-pointer block unchanged.

- [ ] **Step 4: Mirror generator changes in adapter**

Keep emitted helper names, header skip, parameter order, and fallback branches identical.

- [ ] **Step 5: Run focused Python tests**

```bash
python3 -m pytest tests/python/asc_op_compiler/test_compile_op.py -k 'oom or add_op_param_to_workspace' -q
python3 -m pytest tests/python/adapter/test_compile_op.py -k 'oom or add_op_param_to_workspace' -q
```

Expected: PASS.

- [ ] **Step 6: Commit generated-code changes**

```bash
git add tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py tests/python/asc_op_compiler/test_compile_op.py tests/python/adapter/test_compile_op.py
git commit -m "feat: generate OOM storage-shape fallback logic"
```

### Task 4: Publish capability in single-operator JSON

**Files:**
- Modify: `tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py`
- Modify: `tools/build/asc_op_compile_base/adapter/compile_op.py`
- Modify: `tests/python/asc_op_compiler/test_compile_op.py`
- Modify: `tests/python/adapter/test_compile_op.py`

- [ ] **Step 1: Add the OOM capability only for OOM builds**

In the same branch that extends `opParaSize`, add:

```python
js["oom"] = {"version": 1, "tensor_version": 1}
```

Increase OOM `opParaSize` by the fixed 2-byte extension header. This makes the
magic/version probe safe for a new binary invoked by a legacy runtime. Runtime
owns any additional descriptor bytes and must extend its allocation by their
actual total size. The existing bytes and their ordering remain unchanged.

- [ ] **Step 2: Mirror the capability in adapter**

Use the exact same property names and integer values.

- [ ] **Step 3: Run JSON tests**

```bash
python3 -m pytest tests/python/asc_op_compiler/test_compile_op.py -k oom -q
python3 -m pytest tests/python/adapter/test_compile_op.py -k oom -q
```

Expected: OOM output contains the capability; non-OOM output does not.

- [ ] **Step 4: Commit the JSON change**

```bash
git add tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py tools/build/asc_op_compile_base/adapter/compile_op.py tests/python/asc_op_compiler/test_compile_op.py tests/python/adapter/test_compile_op.py
git commit -m "feat: advertise OOM storage-shape capability"
```

### Task 5: Regression and compatibility verification

**Files:**
- Test: all files modified above.

- [ ] **Step 1: Run full primary and adapter compiler test files**

```bash
python3 -m pytest tests/python/asc_op_compiler/test_compile_op.py -q
python3 -m pytest tests/python/adapter/test_compile_op.py -q
```

Expected: PASS.

- [ ] **Step 2: Run the basic-api OOM device tests**

Build and run the target containing `test_dma_oom.cpp`.

Expected: legacy registration, no-magic fallback, ordinary Tensor storage shape, TensorList U/F storage shapes, and invalid metadata behavior all pass.

- [ ] **Step 3: Verify the generated binary contract**

Compile one dynamic OOM example and inspect its retained generated kernel source or saved temporary source. Verify it contains both new storage-shape helpers and old `OOMCheckAddrRange`/`OOMCheckTensorListRange` fallbacks. Inspect its JSON and verify:

```json
"oom": {"version": 1, "tensor_version": 1}
```

- [ ] **Step 4: Run final repository checks**

```bash
git diff --check
git status --short
```

Expected: no whitespace errors; only intentional changes are present.

- [ ] **Step 5: Commit final verification adjustments if needed**

```bash
git add tests/python/asc_op_compiler/test_compile_op.py tests/python/adapter/test_compile_op.py tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp
git commit -m "test: verify OOM storage-shape compatibility"
```

## External Integration Contract

The external runtime must treat the JSON `oom` property as optional, preserve all existing args bytes, and append the approved header and descriptor blocks after the existing OOM payload. It must allocate at least the JSON `opParaSize`, which includes the 2-byte extension header, and add the actual descriptor size when emitting the extension. This runtime implementation is not part of the devkit-only change.
