# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

**asc-devkit** is the core SDK for Ascend C, Huawei's C/C++ programming language for Ascend NPU operators (part of the CANN framework). It contains the API headers, implementations, CMake build infrastructure, operator examples, and unit tests.

## Build Commands

Requires: CANN toolkit installed and environment sourced (`source /usr/local/Ascend/cann/set_env.sh` or `source $HOME/Ascend/cann/set_env.sh`), gcc >= 7.3.0, cmake >= 3.16, Python >= 3.9.

```bash
# Build and package (produces build_out/cann-asc-devkit_*_linux-*.run)
bash build.sh --pkg

# Build with specific thread count
bash build.sh --pkg -j 8

# Clean build artifacts
bash build.sh --make_clean

# Build with ASAN
bash build.sh --pkg --asan
```

### Install the built package
```bash
cd build_out
./cann-asc-devkit_<version>_linux-<arch>.run --full
# or to a specific path:
./cann-asc-devkit_<version>_linux-<arch>.run --full --install-path=${install_path}
```

## Running Tests

Unit tests require additional deps: googletest (release-1.14.0), lcov >= 1.16, pytest >= 8.0, coverage >= 4.5.4.

```bash
bash build.sh --adv_test           # High-level API (adv_api) tests
bash build.sh --basic_test_one     # Basic API part 1
bash build.sh --basic_test_two     # Basic API part 2
bash build.sh --basic_test_three   # Basic API part 3
bash build.sh -t                   # All unit tests
bash build.sh -t --cov             # All tests with coverage
```

Test cases live in `tests/`, split by module. The test runner script is `tests/test_parts.sh`.

## Architecture

The repo separates **headers** (`include/`) from **implementations** (`impl/`) in parallel subdirectory trees:

```
include/          # Public API declarations (.h files)
  adv_api/        # High-level API (math, matmul, activations, etc.)
  basic_api/      # Low-level single-instruction abstractions (Tensor-based)
  c_api/          # Pure-C language extension API (pointer-based, SIMD)
  aicpu_api/      # AI CPU operator API
  utils/          # Shared utilities

impl/             # API implementations
  adv_api/        # High-level API impl (calls basic_api internally)
  basic_api/      # Basic API impl (wraps hardware instructions)
  c_api/          # C API impl
  aicpu_api/      # AI CPU impl
  micro_api/      # Micro-level API
  simt_api/       # SIMT programming API
  experimental/   # Experimental APIs
  utils/          # Utility implementations
```

**API layering** (bottom-up): `c_api` / `basic_api` → `adv_api`. High-level APIs compose basic APIs.

**CMake structure**: `cmake/` contains reusable modules for building, packaging, and dependency management. The root `CMakeLists.txt` orchestrates `impl/`, `tools/`, `cmake/`, and optionally `tests/` (when `ENABLE_TEST=ON`).

**Key CMake options**:
- `BUILD_OPEN_PROJECT` (default ON): builds as standalone open-source project
- `BUILD_WITH_INSTALLED_DEPENDENCY_CANN_PKG` (auto): links against installed CANN vs. source
- `ENABLE_TEST`: enables test subdirectory
- `PACKAGE_OPEN_PROJECT`: triggers packaging logic

**Heterogeneous compilation**: Files with `.asc` extension or compiled with `-x asc` flag use the heterogeneous (CPU+NPU twin) compilation path, enabling `<<<>>>` kernel launch syntax.

**Examples** (`examples/`): Organized by complexity — `00_introduction`, `01_utilities`, `02_features`, `03_libraries` (high-level API), `04_best_practices`. Each has its own CMakeLists.txt using the CMake module interfaces provided by this repo.

## Contributing API

Three contribution tracks, each with its own guide in `docs/`:
- **High-level API** (`adv_api`): See `docs/asc_adv_api_contributing.md`
- **Basic API** (`basic_api`): See `docs/asc_basic_api_contributing.md`
- **C API** (`c_api`): See `docs/asc_c_api_contributing.md`

Non-trivial changes (new APIs, interface changes, new config params) require an Issue discussion before submitting a PR.
