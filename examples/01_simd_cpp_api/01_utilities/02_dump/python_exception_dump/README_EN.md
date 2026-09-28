# NPU Operator Exception Info Auto-Dump

> When an NPU kernel hits a runtime exception, automatically dump the exception context and kernel args of the failing operator for offline analysis (inputs/outputs can be reconstructed from the operator's args layout); a pure-Python input/output tensor dump is also provided and can be cross-checked against the device pointers in the exception products. Plugin-library design, zero changes to operator source code.
>
> 中文版：[README.md](README.md)

## 1. Capabilities

| Capability | How to enable | Products |
|---|---|---|
| Exception auto-dump (C++ plugin-library callback) | `enable_exception_dump(["kernel_prefix"])` | `{kernel}_dev{id}_task{id}_{ts}_info.txt` (exception common info) + same-prefix `_args.bin` (raw kernel args: device pointers of each input/output tensor + tiling; reconstruct offline using the operator's args layout) |
| Input/output tensor dump (pure Python, no .so, no build) | `run_with_tensor_dump(op, tag=...)` (automatic at call boundary) or `dump_tensors(..., stage=...)` (manual) | `input_{tag}_{name}_dev{idx}_{ts}.bin/.json` (data + meta; `data_ptr` in json matches the corresponding pointer in `_args.bin` for cross-checking); `output_*` likewise, produced only after a normal return |

Key points: kernel-name prefix filtering (a mismatch only logs one skip line); idempotent re-enabling; callback registration adapts to the CANN version at runtime (interfaces differ before/after 9.2.0, one .so works for both); on a kernel exception the output tensor is never written back — `output_*` is produced only after the operator returns normally, while the output-pointer scene is provided by `_args.bin`.

Configuration (environment variables): `XPUOPS_DUMP_DIR` (dump directory, shared by both capabilities, default `./exception_dump/`) ｜ `XPUOPS_DUMP_LEVEL` (0 = common info only, 1 = common info + args, default 1, exception dump only) ｜ `XPUOPS_DUMP_LIB_DIR` (directory of the .so, default `lib/` inside the project).

## 2. Quick Start (reproduce the demo)

Environment: Atlas A2 training series products / Atlas A2 inference series products ｜ CANN installed with its environment configured (effective after sourcing `set_env.sh`) ｜ PyTorch / torch_npu matching the installed CANN version ｜ pip package `ascend_ops` (provides the IFA operator called by the demo).

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh     # CANN environment
cd xpuops_exception_dump && bash build.sh && cd ..      # build the plugin .so (output to lib/, required)
python3 examples/demo_ifa.py
```

Demo flow: normal IFA call (input/output dumped automatically at the call boundary) → fault injection corrupts block_table to trigger an AICore kernel exception → the plugin-library callback auto-dumps exception info and args → the script self-checks the products; exit code 0 when all pass. Key expected output:

```text
[SIZE-OK] args.bin: 1264 bytes == expected 1264
[INFO-OK] kernel_name / error_code / args_dumped (3 items)
[INPUT-OK] / [OUTPUT-OK] / query size / corrupted block_table read-back
[PASS] IFA exception dump OK
```

Products are written to `./exception_dump_ifa/` at the package root. The specific error-code value is CANN-version dependent (it may differ across versions; judge by kernel-name hit + args on disk). The plugin .so is an environment-specific binary and is not committed to the repo — run `bash build.sh` first (see the command above; tensor dump is pure Python and has no such requirement).

## 3. Integrate with Your Own Operator

```python
from xpuops.debug import enable_exception_dump, run_with_tensor_dump, dump_tensors

# 1) Enable exception dump: prefix = demangled kernel function name (template
#    parameters stripped); when unsure, use the Kernel Name printed in exception
#    logs; pass nothing to cover all kernels; idempotent
enable_exception_dump(["my_kernel"])

# 2) (optional) auto-dump input/output tensors: on exception, inputs are already
#    on disk; outputs are unavailable (kernel never wrote back)
out = run_with_tensor_dump(torch.ops.my_lib.my_op, tag="my_op", *args, **kwargs)

# 3) (optional) manually dump specific tensors
dump_tensors({"query": q, "block_table": bt}, stage="input")
```

No C++ changes and no rebuild of your project; only exception dump loads the .so, tensor dump is pure Python. To verify: after triggering a kernel exception, confirm `_args.bin` and `_info.txt` appear in the dump directory (error_code / kernel_name consistent with the runtime log); if steps 2/3 were used, also confirm `input_*` / `output_*` products.

## 4. FAQ

| Symptom | Fix |
|---|---|
| Log `kernel '...' not in enabled prefix list, skip dump` | Correct the prefix per the actual kernel name in logs, or pass nothing (all kernels) |
| No `_args.bin` in the dump directory | `XPUOPS_DUMP_LEVEL` was set to 0 (common info only); set it back to 1 |
| No `output_*` after an exceptional call | Expected: outputs are not written back on exception; dump output baselines on the normal path (see Section 1 key points) |
| `args_dumped : no` in `_info.txt` / log `args unavailable` | CANN exception info lacks args; check the CANN version |
| `FileNotFoundError: libxpuops_exception_dump.so not found` | Build first: `cd xpuops_exception_dump && bash build.sh` (the .so binary is not committed); or set `XPUOPS_DUMP_LIB_DIR` to its directory (only exception dump needs the .so; tensor dump is unaffected) |
| `xpuops` module not found | Add `xpuops_exception_dump/python` to `PYTHONPATH` (the demo script handles this automatically) |
| Read back `input_*` / `output_*` tensors offline | Any environment with torch (no NPU needed): restore via the `.json` meta + `torch.frombuffer` byte view |

## 5. Directory Layout

```text
README.md / README_en.md      This document (Chinese / English)
examples/demo_ifa.py          Demo: normal call + fault injection + product self-check (runnable)
xpuops_exception_dump/        Plugin library project: src/ (C++ callback), python/xpuops/ (Python API),
                              build.sh (output to lib/; build the .so yourself, binaries not committed)
```
