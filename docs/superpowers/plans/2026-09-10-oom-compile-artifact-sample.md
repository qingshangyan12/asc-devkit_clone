# OOM Compile Artifact Sample Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a device-independent example that checks legacy OOM generation remains compatible and storage-shape-aware OOM source and JSON artifacts are generated as specified.

**Architecture:** A standalone Python checker loads repository generator files while using the supplied CANN package for native dependencies. It creates minimal compiler fixtures, invokes the real source generator and JSON post-processor, and reports named checks. A subprocess-level unittest verifies user-visible output and exit codes.

**Tech Stack:** Python 3, `unittest`, `unittest.mock`, AscendC `asc_op_compile_base`, Markdown.

---

## File Structure

- Create `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py`: environment bootstrap, fixtures, artifact checks, and CLI.
- Create `tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py`: success and invalid-environment subprocess tests.
- Create `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/README.md`: Chinese guide.
- Create `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/README_en.md`: English guide.
- Modify `examples/01_simd_cpp_api/02_features/04_compile/README.md`: Chinese index entry.
- Modify `examples/01_simd_cpp_api/02_features/04_compile/README_en.md`: English index entry.

### Task 1: Establish the CLI Contract

**Files:**
- Create: `tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py`
- Create: `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py`

- [ ] **Step 1: Write the failing subprocess tests**

Create a `unittest.TestCase` that resolves `REPO_ROOT = Path(__file__).resolve().parents[3]` and launches the sample with `sys.executable`, `--devkit-path`, and `--cann-path /data/chu/Ascend/cann`. The success test must assert exit 0 and these lines:

```python
expected_lines = (
    "PASS dynamic tensor keeps legacy fallback",
    "PASS dynamic tensor emits storage-shape registration",
    "PASS dynamic TensorList keeps legacy fallback",
    "PASS dynamic TensorList emits storage-shape registration",
    "PASS static shape keeps legacy OOM generation",
    "PASS non-OOM artifacts remain unchanged",
    "PASS OOM JSON capability and opParaSize",
    "All OOM compile artifact checks passed.",
)
```

Add a second test using `--devkit-path /tmp/not-a-devkit`; require exit 2 and an `ERROR` line.

- [ ] **Step 2: Run the test and verify RED**

```bash
python3 -m unittest tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py -v
```

Expected: both tests fail because the sample file does not exist.

- [ ] **Step 3: Add the minimal CLI skeleton**

Implement `argparse`, default devkit root from `Path(__file__).resolve().parents[6]`, and CANN resolution from `--cann-path` then `ASCEND_HOME_PATH`. Validate these files:

```python
repo_generator = devkit_path / "tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py"
repo_compile_op = devkit_path / "tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py"
cann_platform = cann_path / "python/site-packages/asc_op_compile_base/c_api/libasc_platform.so"
```

Use exit codes 0 for success, 1 for failed checks, and 2 for arguments/import/environment errors. At this stage return 1 with `FAIL checks are not implemented` after valid path checks.

- [ ] **Step 4: Verify the partial state**

Run the test again. Expected: invalid-path test passes; success test fails with `FAIL checks are not implemented`.

- [ ] **Step 5: Commit**

```bash
git add tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py \
  examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py
git commit -m "test: define OOM artifact sample contract"
```

### Task 2: Generate and Check Source Artifacts

**Files:**
- Modify: `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py`

- [ ] **Step 1: Re-run the success test to retain RED**

```bash
python3 -m unittest tests.python.asc_op_compiler.test_oom_compile_artifact_sample.TestOomCompileArtifactSample.test_all_artifact_contracts_pass -v
```

Expected: FAIL because source checks are absent.

- [ ] **Step 2: Bootstrap compiler dependencies before import**

If `ASCENDC_OOM_SAMPLE_BOOTSTRAPPED` is absent, re-execute the script with `os.execve`. Prepend `${CANN_PATH}/python/site-packages` to `PYTHONPATH` and these directories to `LD_LIBRARY_PATH`:

```python
(
    cann_path / "aarch64-linux/lib64",
    cann_path / "aarch64-linux/devlib/linux/x86_64",
)
```

After bootstrap, import the CANN package dependencies, then load the repository generator with `importlib.util.spec_from_file_location` under a unique name within `asc_op_compile_base.asc_op_compiler`. This ensures functions under test come from `DEVKIT_PATH`.

- [ ] **Step 3: Build minimal real compiler fixtures**

Construct a dynamic `OpInfo` with a required float32 input `x`, a dynamic float32 TensorList input `xs`, and a required float32 output `z`. Set:

```python
op_info.param_type_list == ["required", "dynamic", "required"]
tiling_info.tiling_data_size = 64
tiling_info.tiling_key_list = ["1"]
tiling_info.tiling_key_data_size = {}
tiling_info.static_shape_flag = False
compile_info.tiling_key_struct_map = {}
```

Use `buildcfg.build_config()` and set `cfg.current()["tir.op_debug_config"]` to `["oom"]` or `[]` before calling the repository `add_op_param_to_workspace`. Create dynamic OOM, static OOM, and dynamic non-OOM outputs.

- [ ] **Step 4: Implement named source checks**

Use:

```python
def check(name, condition, detail):
    if condition:
        print(f"PASS {name}")
        return True
    print(f"FAIL {name}: {detail}")
    return False
```

Require the dynamic OOM source to contain `OOMHasStorageShapeHeader`, `oomStorageShapeCursor += 2`, `OOMTryRegisterTensorWithStorageShape`, `OOMTryRegisterTensorListWithStorageShape`, `OOMCheckAddrRange`, and `OOMCheckTensorListRange`. Check the two new registration calls follow parameter order. Require static OOM to contain fixed-length `OOMCheckAddrRange` and no storage-shape symbols. Require non-OOM output to contain no OOM symbols.

- [ ] **Step 5: Run the focused test**

Run the command from Step 1. Expected: source `PASS` lines appear; only the missing JSON check keeps the test red.

- [ ] **Step 6: Commit**

```bash
git add examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py
git commit -m "feat: check OOM generated source compatibility"
```

### Task 3: Check JSON Capability and Parameter Size

**Files:**
- Modify: `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py`

- [ ] **Step 1: Verify RED for JSON output**

Run the success test. Expected: `PASS OOM JSON capability and opParaSize` is absent.

- [ ] **Step 2: Load and invoke the repository JSON post-processor**

Load repository `compile_op.py` with the same import mechanism. In a `TemporaryDirectory`, write:

```python
base_json = {
    "kernelName": compile_info.kernel_name,
    "workspace": {"num": 1, "size": [0], "type": [0]},
    "compileInfo": {},
}
```

Patch platform-only collaborators with `unittest.mock`: `CommonUtility.get_kernel_meta_dir` returns the temporary directory; `CommonUtility.is_v220` and `CommonUtility.is_c310` return `False`; `add_sub_super_kernel_info` returns its JSON input; `_init_param_value` is a no-op; binary/hash and log post-processing helpers are no-ops. Invoke `_json_post_process` once under OOM build config and once under non-OOM config, restoring the base JSON between calls.

- [ ] **Step 3: Assert the real JSON artifacts**

For tiling size 64, calculate:

```python
legacy_oom_size = 64 + 8 + 8 * DFXSectionGenerator().param_placeholder_num
expected_oom_size = legacy_oom_size + 2
expected_non_oom_size = 64 + 8
```

Require:

```python
oom_json["oom"] == {"version": 1, "tensor_version": 1}
oom_json["opParaSize"] == expected_oom_size
"oom" not in non_oom_json
non_oom_json["opParaSize"] == expected_non_oom_size
```

Print `PASS OOM JSON capability and opParaSize` only when all conditions hold. Return 0 only when every named check passes; otherwise summarize failures and return 1.

- [ ] **Step 4: Run GREEN tests and the sample**

```bash
python3 -m unittest tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py -v
python3 examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py \
  --devkit-path /data/chu/01_OOM/asc-devkit-3 \
  --cann-path /data/chu/Ascend/cann
```

Expected: two tests pass; direct execution prints seven named `PASS` lines and returns 0.

- [ ] **Step 5: Commit**

```bash
git add examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py
git commit -m "feat: validate OOM JSON compile artifacts"
```

### Task 4: Document, Index, and Verify

**Files:**
- Create: `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/README.md`
- Create: `examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/README_en.md`
- Modify: `examples/01_simd_cpp_api/02_features/04_compile/README.md`
- Modify: `examples/01_simd_cpp_api/02_features/04_compile/README_en.md`

- [ ] **Step 1: Write both guides**

State that the sample checks generated source and JSON and does not launch a kernel. Document Python, devkit, and CANN prerequisites; the direct command above; dynamic Tensor, TensorList, static OOM, non-OOM, and JSON/size scenarios; exit codes 0/1/2; and common import/path failures.

- [ ] **Step 2: Update both compile indexes**

Add:

```markdown
| [04_oom_compile_artifact_check](./04_oom_compile_artifact_check) | 检查 OOM 原有场景兼容性以及非连续 Tensor storage-shape 编译产物。 |
```

and:

```markdown
| [04_oom_compile_artifact_check](./04_oom_compile_artifact_check) | Checks legacy OOM compatibility and non-contiguous Tensor storage-shape compile artifacts. |
```

- [ ] **Step 3: Run final verification**

```bash
python3 -m py_compile \
  examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py \
  tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py
python3 -m unittest tests/python/asc_op_compiler/test_oom_compile_artifact_sample.py -v
python3 examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py \
  --devkit-path /data/chu/01_OOM/asc-devkit-3 \
  --cann-path /data/chu/Ascend/cann
git diff --check
```

Expected: syntax succeeds, two tests pass, direct execution prints all seven `PASS` lines, and `git diff --check` is empty.

- [ ] **Step 4: Run the existing compiler regression target when available**

```bash
cmake --build build --target ascendc_pyut_asc_op_compiler
```

Expected: target passes. If no configured build tree exists, record that limitation; the standalone sample tests remain mandatory.

- [ ] **Step 5: Commit docs**

```bash
git add examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/README.md \
  examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/README_en.md \
  examples/01_simd_cpp_api/02_features/04_compile/README.md \
  examples/01_simd_cpp_api/02_features/04_compile/README_en.md
git commit -m "docs: add OOM artifact sample guide"
```

## Final Checks

- [ ] `git status --short` has no uncommitted sample changes.
- [ ] The direct sample retains seven named `PASS` results for handoff.
- [ ] No test or sample depends on an NPU device or a requested `SOC_ARCH`.
