# OOM Packed Descriptor Revision Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the version-1 OOM extension with the confirmed nibble-width header and TensorViewDesc layouts.

**Architecture:** Keep the capability JSON and legacy fallback unchanged. Encode the extension header in 2 bytes and each TensorViewDesc in 9 bytes, using explicit masks and shifts instead of compiler bit-fields. Tensor and TensorList block headers remain 10 and 12 bytes respectively.

**Tech Stack:** C++17 AscendC device helpers, gtest, Python code-generation checks.

---

### Task 1: Specify the packed ABI in tests

**Files:**
- Modify: `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp`

- [ ] **Step 1: Change the Tensor fixture to the 19-byte block**

Encode `u64 size=19`, `u8 type=1`, `u8 reserved`, followed immediately at offset 10 by `{u4 version, u4 reserved, u64 storageShapeSize}`. Use `storageShapeSize=16`, float32 element size 4, and expect a 64-byte registered range and cursor advancement by 19 bytes.

- [ ] **Step 2: Change the TensorList fixture to the packed list block**

Encode `u64 size=30`, `u8 type=2`, `u8 flag`, `u16 count=2`, followed at offset 12 by two 9-byte TensorViewDesc records with element counts 16 and 12. Verify U and F modes register 64-byte and 48-byte ranges.

- [ ] **Step 3: Run the focused device test in red state**

Run the basic-api target containing `test_dma_oom.cpp`. Expected: the new packed-layout assertions fail against the current parser.

### Task 2: Parse the packed ABI

**Files:**
- Modify: `impl/basic_api/utils/kernel_utils_macros.h`
- Modify: `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`

- [ ] **Step 1: Update fixed sizes and field types**

Set the extension header size to 2, Tensor header size to 10, TensorList header size to 12, and TensorViewDesc size to 9. Treat descriptor type and flag as `u8`, and count as `u16`.

- [ ] **Step 2: Replace dimension-array parsing**

Read TensorViewDesc version from bits 3:0 at offset 0 and `storageShapeSize` at offset 1. Reject unsupported versions, zero element counts, incorrect containing block sizes, and `storageShapeSize * elementSize` overflow.

- [ ] **Step 3: Preserve Tensor and TensorList registration behavior**

For Tensor, require exact size 19 and register `storageShapeSize * elementSize`. For TensorList, require exact size `12 + count * 9`, retrieve tensor addresses through the unchanged second-level-pointer block, apply the existing packed element-size scale, and advance by the declared block size only after successful parsing.

- [ ] **Step 4: Run device tests in green state**

Run the same target as Task 1. Expected: packed Tensor, TensorList U/F, legacy no-magic, and existing OOM tests pass.

### Task 3: Verify generated-code compatibility

**Files:**
- Verify: `tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py`
- Verify: `tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py`
- Verify: `tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py`

- [ ] **Step 1: Confirm unchanged boundaries**

Verify generated code skips the 2-byte extension header and invokes the same Tensor/TensorList helper APIs. Confirm the JSON remains `{"oom": {"version": 1, "tensor_version": 1}}` and OOM `opParaSize` reserves the fixed 2-byte extension header.

- [ ] **Step 2: Run static and focused checks**

Run `git diff --check`, parse modified Python files with `ast.parse`, run the focused Python OOM tests, and configure/run the basic-api test target. Record missing repository build artifacts or SDK packages if the environment blocks a test before execution.

- [ ] **Step 3: Commit directly to master**

Commit the specification, plan, packed parser, and regression test changes without modifying the deferred aclnn repository.
