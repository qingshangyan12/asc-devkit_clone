# NPU Operator Exception Info Auto-Dump Example

> 中文版：[README.md](README.md)

## Overview

This example demonstrates an NPU operator exception info auto-dump solution based on the AscendCL Runtime exception callback. When an NPU kernel hits a runtime exception, the plugin library automatically dumps the exception context of the failing operator (timestamp, device/task/stream/thread ID, error code, kernel name) and the kernel args for offline analysis (inputs/outputs can be reconstructed from the operator's args layout); a pure-Python input/output tensor dump is also provided, which automatically saves input/output tensors at the operator call boundary and can be cross-checked against the device pointers in the exception products. The solution requires zero changes to operator source code.

> **Note:** This example is experimental and currently implemented as an external plugin library. The capability is planned to be integrated into CANN (Python-side automatic exception info dump, operator input/output tensor dump, etc.), and this example will be simplified accordingly. Refer to the latest CANN version for updates.

## Supported Products and CANN Versions

| Products | CANN Versions |
|---|---|
| Ascend 950PR&950DT products | \>= CANN 9.1.0 |
| Atlas A3 products | \>= CANN 9.1.0 |
| Atlas A2 products | \>= CANN 9.1.0 |

## Directory Structure

```text
├── python_exception_dump
│   ├── examples
│   │   ├── demo_add.py                  // Add: normal call + fault injection + self-check
│   │   └── aclnn_add.py                 // ctypes bridge to the local AddCustom package
│   ├── npuops_exception_dump
│   │   ├── include
│   │   │   └── npuops_exception_dump.h  // Public C interface of the plugin library
│   │   ├── python/npuops                // Python API (debug.py, etc.)
│   │   ├── src                          // C++ exception callback implementation
│   │   ├── build.sh                     // One-shot build script (output to lib/)
│   │   └── CMakeLists.txt               // Build project file
│   ├── README_en.md
│   └── README.md
```

## Example Description

This example provides two dump capabilities:

| Capability | How to enable | Products |
|---|---|---|
| Exception auto-dump (C++ plugin-library callback) | `enable_exception_dump(["kernel_prefix"])` | `{kernel}_dev{id}_task{id}_{ts}_info.txt` (exception common info) + same-prefix `_args.bin` (raw kernel args: device pointers of each input/output tensor + tiling; reconstruct offline using the operator's args layout) |
| Input/output tensor dump (pure Python, no .so, no build) | `run_with_tensor_dump(op, tag=...)` (automatic at call boundary) or `dump_tensors(..., stage=...)` (manual) | `input_{tag}_{name}_dev{idx}_{ts}.bin/.json` (data + meta; `data_ptr` in json matches the corresponding pointer in `_args.bin` for cross-checking); `output_*` likewise, produced after the operator returns; note that in async-exception scenarios (exception detected after launch) the returned tensors may be saved, but the kernel never wrote back — such data is unreliable; use the normal-path dump as the output baseline |

Key points: kernel-name prefix filtering (a mismatch only logs one skip line); idempotent re-enabling; callback registration adapts to the CANN version at runtime (one .so works for both new and old interfaces); on a kernel exception the output tensor is never written back — `output_*` produced on an exceptional path is unreliable; use the normal-path dump as the output baseline, while the output-pointer scene is provided by `_args.bin`.

Configuration (environment variables):

| Environment variable | Description | Default |
|---|---|---|
| `NPUOPS_DUMP_DIR` | Dump directory (shared by both capabilities) | `./exception_dump/` |
| `NPUOPS_DUMP_LEVEL` | Dump level: 0 = common info only, 1 = common info + args (exception dump only) | `1` |
| `NPUOPS_DUMP_LIB_DIR` | Directory of the plugin-library .so | `lib/` inside the project |

## Build and Run

- Install PyTorch and the Ascend Extension for PyTorch plugin

  Refer to [pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch) or the [Ascend Extension for PyTorch community](https://hiascend.com/document/redirect/Pytorch-index) for installation instructions, and select a supported `Python` release to install `torch` and `torch-npu`.

- Install prerequisites

  The demo calls `AddCustom` built and installed from this repository's [custom operator project example](../../../../02_features/99_acl_based/00_acl_compilation/custom_op/README_en.md) through `ctypes`. The operator implementation is reused directly from the existing project.

- Configure environment variables

  Configure the environment variables according to how the CANN toolkit is installed on the current environment.

  ```bash
  source ${install_path}/ascend-toolkit/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. If not specified, CANN is installed under `/usr/local/Ascend` by default.

- Run the example

  In the root directory of this example, build the plugin library and run the demo.

  ```bash
  # Locate the existing custom_op project from this Dump example directory
  SAMPLE_DIR="$PWD"
  CUSTOM_OP_DIR="$(cd ../../../../02_features/99_acl_based/00_acl_compilation/custom_op && pwd)"
  cmake -S "$CUSTOM_OP_DIR" -B "$CUSTOM_OP_DIR/build"
  cmake --build "$CUSTOM_OP_DIR/build" --target binary package -j8
  bash "$CUSTOM_OP_DIR"/build/custom_opp_*.run --install-path="$SAMPLE_DIR/custom_opp"
  source "$SAMPLE_DIR/custom_opp/vendors/customize/bin/set_env.bash"
  export NPUOPS_ADD_LIB="$SAMPLE_DIR/custom_opp/vendors/customize/op_api/lib/libcust_opapi.so"

  cd npuops_exception_dump && bash build.sh && cd ..    # Build the plugin .so (output to lib/)
  python3 examples/demo_add.py                          # Run the demo
  ```

  > **Note:** The plugin .so is an environment-specific binary and is not committed to the repo — build it before running; tensor dump is pure Python and has no such requirement.

- Expected results

  Demo flow: call `aclnnAddCustom` normally with two contiguous float16 ND tensors of 16384 elements (`x = (arange(16384) % 1024).to(float16)`, `y = ones(16384, dtype=float16)`), dump both inputs and the output, and verify `x + y`. In a separate subprocess, call the same Add operator with its output address replaced by the deliberately invalid, non-null aligned device address `0x100000000000`, triggering a device memory-access exception. Both input tensors remain valid and are dumped before the failing call. The plugin callback dumps exception info and raw kernel args; the parent validates all products on the CPU. Input x is reduced modulo1024 so the inputs and sums are exactly representable in float16.

  ```text
  normal Add run OK
  caught expected device execution error: aclrtSynchronizeStream failed: ...
  [DATA-OK] normal/fault input tensors and normal x + y output
  [INFO-OK] <actual Add kernel name>, error=<actual nonzero error code>
  [SIZE-OK] args.bin: <actual size> bytes == callback args_bytes
  [DATA-OK] input and injected output pointers found in raw kernel args
  [PASS] Add exception dump OK
  ```

  Exit code 0 means that normal computation, device exception injection, and artifact validation all passed. A Python exception or host-side parameter validation error alone cannot pass the test.

  Each invocation creates a fresh `./exception_dump_add/run_*/` directory relative to the current working directory, preventing stale files from passing validation. Use `--dump-dir /path/to/dumps` to change the parent directory. Products include normal/fault `input_add_*` files, normal `output_add_normal_*` files, callback `*_info.txt` and `*_args.bin` files, and `fault.json`, which records the deliberately invalid output address.

  **Fault injection:** the Add kernel is unmodified. Only the output tensor descriptor in the fault call uses an invalid device pointer. This pointer does not represent a readable output tensor; output dumping is disabled for the fault call. The fault worker exits without reusing its failed NPU context, and the parent only reads files from the current run. Callbacks are enabled for all kernels; validation requires an actual kernel name containing `add`, a nonzero error code, an args size matching `args_bytes` in the paired info file, and both input pointers (from their tensor metadata) and the injected output pointer in raw args. No chip-specific kernel name, error code, or args length is hardcoded.

## FAQ

| Symptom | Fix |
|---|---|
| Log `kernel '...' not in enabled prefix list, skip dump` | Correct the prefix per the actual kernel name in logs, or pass nothing (all kernels) |
| No `_args.bin` in the dump directory | `NPUOPS_DUMP_LEVEL` was set to 0 (common info only); set it back to 1 |
| `output_*` missing or invalid after an exceptional call | Expected: outputs are not written back on exception (in async-detected cases some outputs may still be saved, but the data is unreliable); dump output baselines on the normal path (see the key points in Example Description) |
| `args_dumped : no` in `_info.txt` / log `args unavailable` | CANN exception info lacks args; check the CANN version |
| `FileNotFoundError: libnpuops_exception_dump.so not found` | Build first: `cd npuops_exception_dump && bash build.sh` (the .so binary is not committed); or set `NPUOPS_DUMP_LIB_DIR` to its directory (only exception dump needs the .so; tensor dump is unaffected) |
| `npuops` module not found | Add `npuops_exception_dump/python` to `PYTHONPATH` (the demo script handles this automatically) |
| Read back `input_*` / `output_*` tensors offline | Any environment with torch (no NPU needed): restore via the `.json` meta + `torch.frombuffer` byte view |

## Integrate with Your Own Operator

```python
from npuops.debug import enable_exception_dump, run_with_tensor_dump, dump_tensors

# 1) Enable exception dump: prefix = demangled kernel function name (template
#    parameters stripped); when unsure, use the Kernel Name printed in exception
#    logs; pass nothing to cover all kernels; idempotent
enable_exception_dump(["my_kernel"])

# 2) (optional) auto-dump input/output tensors: on exception, inputs are already
#    on disk; outputs are unavailable (kernel never wrote back)
out = run_with_tensor_dump(torch.ops.my_lib.my_op, tag="my_op", *args, **kwargs)

# 3) (optional) manually dump specific tensors
dump_tensors({"x": x, "y": y}, stage="input")
```

No C++ changes and no rebuild of your project; only exception dump loads the .so, tensor dump is pure Python. To verify: after triggering a kernel exception, confirm `_args.bin` and `_info.txt` appear in the dump directory (error_code / kernel_name consistent with the runtime log); if steps 2/3 were used, also confirm `input_*` / `output_*` products.
