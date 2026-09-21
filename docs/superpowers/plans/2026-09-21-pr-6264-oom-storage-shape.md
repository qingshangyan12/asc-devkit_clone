# PR 6264 OOM Storage-Shape Adaptation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Apply PR 6264's final OOM storage-shape behavior to `asc-devkit-5` without disturbing later target-branch work.

**Architecture:** Cherry-pick the PR merge commit relative to its upstream parent so the repository receives only the reviewed 12-file feature delta. Validate device-side descriptor parsing, compiler-generated registration code, backward-compatible fallback, and both Python and Ascend910B1 C++ test surfaces.

**Tech Stack:** Git, AscendC C++, Python 3, pytest, CMake, GoogleTest, CANN 9.2.0, Ascend910B1 (`dav-2201`).

---

### Task 1: Apply The Reviewed PR Delta

**Files:**
- Modify: `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`
- Modify: `impl/basic_api/utils/kernel_utils_macros.h`
- Modify: `tools/build/asc_op_compile_base/adapter/ascendc_compile_dfx.py`
- Modify: `tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py`
- Modify: `tools/build/asc_op_compile_base/adapter/compile_op.py`
- Modify: `tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_dfx.py`
- Modify: `tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py`
- Modify: `tools/build/asc_op_compile_base/asc_op_compiler/cce_runtime.py`
- Modify: `tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py`
- Test: `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp`
- Test: `tests/python/adapter/test_compile_op.py`
- Test: `tests/python/asc_op_compiler/test_compile_op.py`

- [ ] **Step 1: Verify the target and PR ancestry**

Run:

```bash
git status --short --branch
git merge-base master pr/6264
git show -s --format='%H %P' pr/6264
```

Expected: clean `master`; merge base `3577641c0feec70af534dd61f2e11d7f116b3edf`; PR second parent equals that merge base.

- [ ] **Step 2: Apply the PR's final delta**

Run:

```bash
git cherry-pick -m 2 3c5e9d6f60cb19c4d2b0aa29d7ad61330d009e35
```

Expected: one new commit containing the 12 files listed above. If Git reports a conflict, preserve the target branch outside those files and resolve the listed files to the PR head's final behavior.

- [ ] **Step 3: Verify scope and conflict cleanliness**

Run:

```bash
git diff --name-status HEAD^ HEAD
git diff --check HEAD^ HEAD -- impl/basic_api/utils/kernel_utils_ceil_oom_que.h impl/basic_api/utils/kernel_utils_macros.h tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp tests/python/adapter/test_compile_op.py tests/python/asc_op_compiler/test_compile_op.py tools/build/asc_op_compile_base/adapter/ascendc_compile_dfx.py tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py tools/build/asc_op_compile_base/adapter/compile_op.py tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_dfx.py tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py tools/build/asc_op_compile_base/asc_op_compiler/cce_runtime.py tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py
rg -n '^(<<<<<<<|=======|>>>>>>>)' impl/basic_api/utils/kernel_utils_ceil_oom_que.h impl/basic_api/utils/kernel_utils_macros.h tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp tests/python/adapter/test_compile_op.py tests/python/asc_op_compiler/test_compile_op.py tools/build/asc_op_compile_base
```

Expected: exactly 12 changed files, no diff-check output, and no conflict markers.

### Task 2: Verify Compiler-Side Behavior

**Files:**
- Test: `tests/python/adapter/test_compile_op.py`
- Test: `tests/python/asc_op_compiler/test_compile_op.py`
- Verify: the nine modified Python implementation and test files

- [ ] **Step 1: Compile-check modified Python files**

Run:

```bash
python3 -m py_compile tests/python/adapter/test_compile_op.py tests/python/asc_op_compiler/test_compile_op.py tools/build/asc_op_compile_base/adapter/ascendc_compile_dfx.py tools/build/asc_op_compile_base/adapter/ascendc_compile_gen_code.py tools/build/asc_op_compile_base/adapter/compile_op.py tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_dfx.py tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py tools/build/asc_op_compile_base/asc_op_compiler/cce_runtime.py tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py
```

Expected: exit code 0 with no output.

- [ ] **Step 2: Provide the local test library**

Run:

```bash
cp /data/chu/Ascend/cann-9.2.0/python/site-packages/asc_op_compile_base/c_api/libasc_platform.so tools/build/asc_op_compile_base/c_api/libasc_platform.so
```

Expected: the test-only shared library exists at the local package path and remains untracked.

- [ ] **Step 3: Run adapter OOM storage-shape tests**

Run:

```bash
source /data/chu/Ascend/cann-9.2.0/set_env.sh
python3 -m pytest -q -p no:cacheprovider tests/python/adapter/test_compile_op.py::TestCompileOp::test_oom_storage_shape_patterns tests/python/adapter/test_compile_op.py::TestCompileOp::test_add_op_param_to_workspace
```

Expected: `2 passed`.

- [ ] **Step 4: Run asc_op_compiler OOM storage-shape tests in a separate process**

Run:

```bash
source /data/chu/Ascend/cann-9.2.0/set_env.sh
python3 -m pytest -q -p no:cacheprovider tests/python/asc_op_compiler/test_compile_op.py::TestCompileOp::test_oom_storage_shape_patterns tests/python/asc_op_compiler/test_compile_op.py::TestCompileOp::test_add_op_param_to_workspace
```

Expected: `2 passed`. Separate execution avoids pytest's duplicate `test_compile_op` module-name collision.

- [ ] **Step 5: Remove the test-only library**

Run:

```bash
unlink tools/build/asc_op_compile_base/c_api/libasc_platform.so
```

Expected: the copied binary is absent and `git status` shows no generated Python test artifacts.

### Task 3: Verify Ascend910B1 Device-Side Behavior

**Files:**
- Test: `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp`
- Verify: `impl/basic_api/utils/kernel_utils_ceil_oom_que.h`
- Verify: `impl/basic_api/utils/kernel_utils_macros.h`

- [ ] **Step 1: Configure and build the Ascend910B1 AIV Basic API target**

Run:

```bash
bash build.sh --basic_test_two --cann_path=/data/chu/Ascend/cann-9.2.0 -j 8
```

Expected: CMake configuration succeeds and creates `build/tests/api/basic_api/ascendc_ut_basic_api_ascend910B1_AIV`.

- [ ] **Step 2: Run the storage-shape tests**

Run:

```bash
build/tests/api/basic_api/ascendc_ut_basic_api_ascend910B1_AIV --gtest_filter=TestDMAOom.TestOOMStorageShape*
```

Expected: both `TestOOMStorageShapeTensorDescriptor` and `TestOOMStorageShapeTensorListDescriptor` pass.

- [ ] **Step 3: Run the complete OOM fixture**

Run:

```bash
build/tests/api/basic_api/ascendc_ut_basic_api_ascend910B1_AIV --gtest_filter=TestDMAOom.*
```

Expected: all `TestDMAOom` tests pass, including legacy range and TensorList behavior.

### Task 4: Final Repository Verification

**Files:**
- Verify: all files changed by the adaptation commit
- Verify: `docs/superpowers/specs/2026-09-21-pr-6264-oom-storage-shape-design.md`
- Verify: `docs/superpowers/plans/2026-09-21-pr-6264-oom-storage-shape.md`

- [ ] **Step 1: Confirm the PR behavior is present**

Run:

```bash
rg -n 'OOMHasStorageShapeHeader|OOMTryRegisterTensorWithStorageShape|OOMTryRegisterTensorListWithStorageShape|tensor_version' impl tests tools
```

Expected: implementations, generated-code patterns, JSON metadata, and tests are all found.

- [ ] **Step 2: Confirm later target-branch work remains in history**

Run:

```bash
git merge-base --is-ancestor 4f3a01a62 HEAD
```

Expected: exit code 0.

- [ ] **Step 3: Inspect final state**

Run:

```bash
git status --short --branch
git log -4 --oneline --decorate
```

Expected: clean `master`, with the plan commit and PR adaptation commit above `4f3a01a62`.
