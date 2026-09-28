# <<<>>> Operator Exception Dump Plug-in Python Model Sample

## Overview

When an operator fails an AICore task on the NPU (for example, an out-of-bounds access triggering a hardware trap), the default behavior is to propagate the task failure and exit the process, losing the on-device evidence of "what data was actually fed to this execution" and making offline reproduction difficult. This sample demonstrates a **Runtime-level plug-in Exception Dump capability**: without modifying operator code, an exception callback is enabled through a Python API; after an AICore exception occurs in the operator, the callback automatically rescues the kernel args region and all registered inputs from device memory to host disk, forming an exception scene for offline analysis.

The operator under test is `incre_flash_attention` (IFA) and its `incre_flash_attention_meta` predecessor from the open-source ops-transformer repository (a real, complex `<<<>>>` direct-launch operator), with **zero modifications to operator code**.

| Feature | Description |
|---------|-------------|
| Non-intrusive | Works without modifying operator code; the exception callback is registered via `aclrtExceptionInfoCallbackRegister`, which supports coexistence of multiple callbacks and does not overwrite callbacks registered by the host framework |
| Zero overhead | The normal execution path goes through no dump logic at all; the injection-free baseline shows zero artifacts and zero callback triggers |
| Trustworthy | Dumped data is bit-exact with the constructed inputs; injected corrupt values are preserved as-is |
| Public APIs | Depends only on public CANN APIs (`-lacl_rt -lascendcl` + public headers), stable across version upgrades |

Registration chain (the only chain): Python calls `enable_exception_dump(kernel_name)`, and the callback library registers the exception callback via `aclrtExceptionInfoCallbackRegister` (the legacy `aclrtSetExceptionInfoCallback` is deprecated); when the first exception of the target kernel is matched, the callback obtains the function handle and binary handle and arms a per-binary callback, so both the current and subsequent exceptions are captured.

```
enable_exception_dump(kernel_name)
  -> aclrtExceptionInfoCallbackRegister
  -> obtain funcHandle and binHandle on the first exception
  -> aclrtBinarySetExceptionCallback (automatically arm the per-binary callback)
  -> dump
```

**Difference from in-kernel `asc_dump_*`**: the `asc_dump_*` series (see simple_dump and other samples) are data-printing interfaces written inside kernel functions for visualizing the normal execution path; this sample is a Runtime-level automatic exception-scene saving capability that adds no instrumentation inside the kernel. The two serve different purposes and do not replace each other.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|--------------|
| Ascend 910B4 | >= CANN 9.2.0 (tested baseline) |

Exception callback registration relies on the public interfaces `aclrtExceptionInfoCallbackRegister` / `aclrtExceptionInfoCallbackUnregister` (older CANN versions only provide the deprecated `aclrtSetExceptionInfoCallback` and are not supported); other product and CANN combinations must first confirm interface availability via `scripts/setup.sh check` and be re-verified.

End-to-end cases support **physical NPU only** (CPU / simulator modes are not supported); the exception scope covers AICore task failures (the chain does not differentiate exception types), while AICPU operator exceptions, CPU_DEBUG, and unified graph-mode exception collection are out of scope.

## Directory Structure

```
├── python_model_exception_dump
│   ├── runtime                                 // reusable generic plug-in core (C ABI callback library)
│   │   ├── exception_dump_public.cpp           //   callback library source (operator-agnostic, reusable as a whole)
│   │   └── exception_dump_public.h             //   sample-owned C ABI header (3 interface declarations)
│   ├── python                                  // Python layer
│   │   ├── op_exception_dump.py                //   Python API wrapper (primary user-facing interface)
│   │   ├── ifa_exception_dump.py               //   IFA model (input construction + scenario execution)
│   │   ├── verify_dump.py                      //   offline bit-exact verification of dumped data
│   │   └── parse_tiling.py                     //   offline parsing of the args pointer region + tiling
│   ├── test                                    // IFA real-operator guard cases
│   │   ├── test_public_dump.py                 //   test entry (scene1/scene2/baseline/all)
│   │   └── patches                             //   build adaptation patches (unrelated to the dump capability)
│   ├── scripts
│   │   ├── setup.sh                            // environment check, callback build, external IFA wheel build
│   │   └── run.sh                              // scenarios, full regression, artifact cleanup
│   ├── CMakeLists.txt                          // build project file (callback library)
│   ├── README.md                               // sample documentation (Chinese)
│   └── README_en.md                            // sample documentation (English)
```

Tested baseline: Ascend 910B4 + CANN 9.2.0; Python 3.11 + torch + torch_npu + the ascend_ops wheel (built from a fixed ops-transformer commit, baseline `ecfc11431`, via `scripts/setup.sh ifa`). Static checks only (syntax / callback library compilation) do not require an NPU.

## Usage

```bash
# 1. Environment preparation (run each time you enter the environment)
source /usr/local/Ascend/ascend-toolkit/set_env.sh       # set ASCEND_HOME_PATH / headers / library paths
export NPU_SOC_VERSION=ascend910_93                      # the build system matches <op>/<SOC>/ directories
./scripts/setup.sh check                                 # callback API, NPU, Python stack

# 2. Compile the callback library (mandatory in a new environment / after a CANN upgrade; the .so is not committed)
./scripts/setup.sh callback
#   Expected: nm -D exports exactly AddInputInfo / ClearInputInfo / RegisterPublicExceptionDump

# 3. Build and install the external IFA wheel (before running end-to-end cases; zero operator change)
export OPS_TRANSFORMER_ROOT=<ops-transformer repository root>
./scripts/setup.sh ifa

# 4. Run cases (artifacts are written under test/; run.sh accepts injected exit code 2 and returns 0)
./scripts/run.sh scene1        # block-table corruption injection -> exception callback dump
./scripts/run.sh full          # full regression: scene1 + verify + parse + all + baseline
./scripts/run.sh clean         # remove dumps and CANN crash logs
```

Offline verification can also run standalone: `python3 python/verify_dump.py` (bit-exact verification of dumped data) and `python3 python/parse_tiling.py` (args pointer-region and tiling parsing). The 8-input args slot table (ptrIndex) of the IFA cases is `INPUT_PTR_INDEX` in `python/ifa_exception_dump.py`; the tiling is inlined in the args region starting at offset 256.

## Onboarding a New Operator

Any operator launched as an AICore kernel via `<<<>>>` can adopt this capability with zero modification. Onboarding involves three parts: the operator side (no modification needed), the Python-side invocation, and the plug-in library execution logic. The `runtime/` callback library is operator-agnostic and reusable as a whole.

### How to Write the Operator (No Modification Needed)

The operator side requires **no code change and no rebuild**. The reason: exception callback registration, kernel-name filtering, and args-region reading all happen at the Runtime layer, independent of the operator implementation; a `<<<>>>` template kernel reports an Itanium mangled name (e.g. `_Z21incre_flash_attentionILh0ELh1ELh0EE...`), and the callback library has built-in substring matching, so passing the plain function name at registration already matches.

Two preconditions must be confirmed before onboarding:

- The target is an **AICore kernel** launched via `<<<>>>` (or an equivalent Runtime launch path); AICPU kernels are outside the exception callback coverage;
- After the kernel fails, the args region and the input tensors' device memory are not immediately reclaimed — which is the common case (the exception breaks the kernel's internal computation but does not free allocated memory), so the callback can rescue the scene before process exit.

The only homework for onboarding is **calibrating the args layout table**: the callback takes each input's device address from "the N-th 8-byte slot of the args region", and the slot order must strictly follow the target kernel's launch parameter order (copy the parameter order from the operator's host-side launch code; optional inputs occupy null slots that are not skipped). Calibration is empirical, not guessed: after the first run, compare the callback's `probe args[N] = 0x...` log against each tensor's `data_ptr()` on the Python side; when every slot matches, the layout table is correct, and bit-exact verification provides the third confirmation.

### How to Invoke from Python

Three steps on the Python side (`python/op_exception_dump.py` provides the wrapper, which loads the callback library via ctypes internally; no header files needed):

```python
from op_exception_dump import enable_exception_dump, register_input, clear_inputs

# 1. Enable + register the operator name (pass the kernel function name itself;
#    substring matching automatically handles mangled template-kernel names)
enable_exception_dump("your_kernel_name")

# 2. Register input handles: name + tensor + ptr_index (args slot, calibrated to the launch parameter order)
#    size/shape/dtype are read automatically from the tensor; clear before each new round of calls
clear_inputs()
register_input("input_a", tensor_a, ptr_index=0)
register_input("input_b", tensor_b, ptr_index=1)

# 3. Inputs generated at runtime (e.g. predecessor-operator outputs) are registered after they are produced
metadata = torch.ops.xxx.your_meta_op(...)
register_input("metadata", metadata, ptr_index=8)

# Then invoke the operator as usual; on exception the callback automatically writes task_fail_dump/dev*_ts*/
```

Notes: details such as byte-encoded arguments are handled by the wrapper; the callback library path defaults to `<sample>/runtime/exception_dump_public.so` and can be overridden via the `lib_path` argument or the `EXC_DUMP_LIB` environment variable; the registered size/shape/dtype must come from the current invocation and must not rely on fixed device addresses.

### Plug-in Library Execution Logic (How to Write the Plug-in Library)

The callback library (`runtime/exception_dump_public.cpp`) is a pure plug-in implementation. Core flow:

1. **Registration**: `RegisterPublicExceptionDump(kernelName)` records the filter name and registers the global exception callback via `aclrtExceptionInfoCallbackRegister` (the interface supports coexistence of multiple callbacks identified by function pointer and does not overwrite callbacks registered by the host framework; `aclrtExceptionInfoCallbackUnregister` unregisters).
2. **Exception trigger**: the Runtime invokes the callback when a kernel task fails. The callback first extracts `deviceId / taskId / streamId / errorCode` via the accessor family, obtains the reported kernel name via `aclrtGetFuncHandleFromExceptionInfo` + `aclrtGetFunctionName` and applies substring-match filtering, skipping non-matches.
3. **Automatic per-binary arming**: on the first match, it obtains the binary handle via `aclrtFunctionGetBinary` and arms a per-binary callback via `aclrtBinarySetExceptionCallback` (armed only once); subsequent exceptions of the same kernel go directly through the per-binary chain.
4. **Scene rescue**: it obtains the args-region location and length via `aclrtGetArgsFromExceptionInfo`, establishes the device context with `aclrtSetDevice`, and copies the region to the host via `aclrtMemcpy` (D2H); it resolves each input's device pointer from the args region by the registered ptr_index, copies each to host, and writes them to `task_fail_dump/dev{d}_task{t}_ts{ms}/` (`<name>.bin` × N + `args_raw.bin` + `inputs_info.txt`), then frees the buffers and calls `aclrtResetDevice`.
5. **Concurrency and timing constraints**: input registration and dump state are serialized by a mutex; the dump must complete **synchronously** inside the callback (the process exits along with the exception, so asynchronous writing would lose the scene); the callback runs on a Runtime thread where Python-level callbacks are unavailable (no GIL), so the callback library must be implemented in C/C++ while Python only registers and enrolls.

## Execution Notes (Read Before Automating Verdicts)

1. **Exit-code semantics**: after a kernel exception is caught, `test_public_dump.py` exits with 2; `run.sh` accepts only that code for an injection scenario and returns 0 itself. An injection that exits with 0 is treated as a failure. The baseline exits normally with 0. End-to-end results also require callback logs and dump artifacts.
2. **verify's reference is pinned to scene1**: `verify_dump.py` only back-fills `block_table[0,0]=0x7FFFFFFF` and verifies the latest `dev*` directory by default. To verify scene2 artifacts, pass the directory explicitly and manually check `actual_seq_kvlen[0]=0x7FFFFFFF`.
3. **Run scenarios in separate processes**: after an AICore error, the device stream state is not restored, and subsequent launches in the same process fail host-side (observed as 507057, ERR00100, etc., varying with the CANN version; exceptions in later segments of `all` mode are this phenomenon, not false callback triggers). Run scene1/baseline in separate processes; verifying per-binary takeover is the exception and uses `all` mode in a single process, judged by the "double dump by both callbacks" criterion; host-side errors in its trailing baseline are not evidence for the baseline scenario.
4. **Double dump by two callbacks**: after the per-binary chain is automatically armed, the same exception may be dumped once each by global-cb and binary-cb (identical content, distinguished by timestamp directories); this is expected.
5. **Artifact directory is script-relative**: run `./scripts/run.sh clean` before re-running; the baseline "zero artifacts" criterion is **no new `dev*_ts*` directories in the current round** (compare timestamps), not an empty directory.
6. **`extra-info/` directory (CANN crash logs)**: CANN generates it in the CWD on AICore errors; it is a normal by-product, not a dump deliverable; when generated by root inside a container, deletion from the host may fail due to permissions and must be done inside the container.
7. **The .so is environment-bound**: the `.so` under `runtime/` is a build artifact and not committed; in a new environment or after a CANN upgrade, `scripts/setup.sh callback` must be re-run.
8. **Exception-code layers**: the device-side callback `retcode=0x7bc87` and the Python-side `error code 507057` are error representations of different layers and are not converted numerically; exception codes may change across CANN versions — **gate on callback triggering (kernel-name match) and whether the dump lands on disk**.

## CANN Version Upgrade Re-check List

| # | Re-check item | Reason | Action |
|---|---------------|--------|--------|
| 1 | **The callback .so must be recompiled** | The .so links against the old CANN libraries; the ABI may be incompatible | Run `scripts/setup.sh callback` before running the cases |
| 2 | **Args layout table / tiling offset** | Changes in the launch parameter order or `BaseParams` field order shift ptrIndex/tiling parsing wholesale | After running scene1, compare the callback probe log `probe args[N]` with the printed `data_ptr()`; recalibrate the layout table if inconsistent |
| 3 | **Kernel mangled-name matching** | Operator renames / template-argument changes may break the match | The log line `Kernel name mismatch (got=..., expected=...)` is directly diagnostic; on mismatch, update the registered kernel name |
| 4 | **Callback registration API availability** | The sample depends on `aclrtExceptionInfoCallbackRegister`/`Unregister`; older versions only provide the deprecated `aclrtSetExceptionInfoCallback` | `scripts/setup.sh check` inspects the header automatically; on undefined-symbol errors during build/registration, confirm interface availability with the CANN side |
| 5 | **Exception-triggering behavior itself** | Fault injection relies on out-of-bounds access being trapped by hardware; if a new version front-loads validation, scene1 may no longer trigger an AICore error | This is an expected behavior change; record it. Capability validity is judged by the "exception occurs -> dump complete" chain |
| 6 | **Exception-code values** | 0x7bc87 / 507057 are measured values and may differ across versions | Gate on callback triggering (kernel-name match) and dump landing, not on exception-code values |
