# OOM Compile Artifact Check Sample

## Overview

This sample checks compatibility contracts in generated source and single-operator
JSON when OOM compilation is enabled. It does not launch a kernel and does not
require an NPU device or a specific SoC.

The sample covers these scenarios:

- A dynamic Tensor emits both storage-shape registration and the legacy address-range fallback.
- A dynamic TensorList emits both storage-shape registration and the legacy second-level-pointer fallback.
- A static shape keeps the existing fixed-length OOM address-range registration.
- Non-OOM artifacts remain isolated by the existing compile-time guard and omit the OOM JSON capability.
- OOM JSON contains `version=1` and `tensor_version=1`, and `opParaSize` reserves the two-byte extension header.

## Requirements

- Python 3;
- an AscendC devkit source checkout;
- a CANN installation containing `python/site-packages/asc_op_compile_base` and its native dependencies.

`SOC_ARCH` is not required. The script derives the devkit root from its own path.
Pass the CANN path with `--cann-path` or set `ASCEND_HOME_PATH`.

## Run the Sample

From the devkit repository root:

```bash
python3 examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py \
  --cann-path /path/to/Ascend/cann
```

When running from another location, specify both roots explicitly:

```bash
python3 check_oom_artifacts.py \
  --devkit-path /path/to/asc-devkit \
  --cann-path /path/to/Ascend/cann
```

A successful run prints:

```text
PASS dynamic tensor keeps legacy fallback
PASS dynamic tensor emits storage-shape registration
PASS dynamic TensorList keeps legacy fallback
PASS dynamic TensorList emits storage-shape registration
PASS static shape keeps legacy OOM generation
PASS non-OOM artifacts remain unchanged
PASS OOM JSON capability and opParaSize
All OOM compile artifact checks passed.
```

On a host without a device driver, the CANN platform library may also print a
driver log-level probe message while exiting. It does not change the checks or
their exit status.

## Exit Codes

- `0`: all artifact contracts match expectations;
- `1`: one or more artifact contracts failed;
- `2`: argument, path, import, or dependency environment error.

For exit code `2`, check `--devkit-path`, `--cann-path`, and the Python packages
and shared libraries in the CANN installation.
