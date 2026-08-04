# ASC CPU Architecture Mapping Refactor Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the redundant flat CPU architecture map and index arithmetic with an explicit behavior-equivalent mapping.

**Architecture:** Keep the CPU-mode configuration in `CMakeASCInformation.cmake`, but select `_SOC_DIR` and `_CPU_MODEL_LIB` together in a three-branch `if`/`elseif`. Preserve all flags, paths, libraries, and diagnostics outside that selection logic.

**Tech Stack:** CMake 3.16+, ASC custom language modules, Ninja, Bisheng compiler

---

### Task 1: Simplify CPU architecture selection

**Files:**
- Modify: `cmake/asc/asc_modules/CMakeASCInformation.cmake:197`
- Test: `asc-devkit/examples/01_simd_cpp_api/02_features/04_compile/02_dynamic_library_compile/CMakeLists.txt`
- Test: `asc-devkit/examples/01_simd_cpp_api/02_features/04_compile/03_static_library_compile/CMakeLists.txt`

- [x] **Step 1: Capture the current generated link commands**

Configure a minimal CPU build for `dav-2002`, `dav-2201`, and `dav-3510` using the current local module, then save `ninja -t commands` output under `/tmp`. The expected model libraries are `_pvmodel`, `pem_davinci`, and `pem_davinci`, respectively; the expected CPU directories are `Ascend310P1`, `Ascend910B1`, and `Ascend950PR_9599`.

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -S ../ascendc_smoke/asc_e2e/e2e_bbit_51/cmake_tests/default_suffix \
  -B /tmp/asc-map-before-2002 -G Ninja \
  -DCMAKE_MODULE_PATH=$PWD/cmake/asc/asc_modules \
  -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2002
ninja -C /tmp/asc-map-before-2002 -t commands > /tmp/asc-map-before-2002.commands
```

Repeat with build suffixes and architecture values `2201`/`dav-2201` and `3510`/`dav-3510`.

- [x] **Step 2: Replace the flat mapping with explicit selection**

Replace `_ARCH_TO_SOC_DIR_MAP`, list lookup, modulo validation, and `_SHORT_NPU_ARCH` extraction with:

```cmake
if(CMAKE_ASC_ARCHITECTURES STREQUAL "dav-2002")
    set(_SOC_DIR "Ascend310P1")
    set(_CPU_MODEL_LIB _pvmodel)
elseif(CMAKE_ASC_ARCHITECTURES STREQUAL "dav-2201")
    set(_SOC_DIR "Ascend910B1")
    set(_CPU_MODEL_LIB pem_davinci)
elseif(CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    set(_SOC_DIR "Ascend950PR_9599")
    set(_CPU_MODEL_LIB pem_davinci)
else()
    message(FATAL_ERROR
        "Unsupported ASC architecture for CPU mode: ${CMAKE_ASC_ARCHITECTURES}, should be dav-2002, dav-2201, dav-3510")
endif()
```

Keep the existing CPU configuration after the conditional and replace the two architecture generator expressions in `link_libraries()` with:

```cmake
${_CPU_MODEL_LIB}
```

- [x] **Step 3: Verify generated commands remain equivalent**

Configure new `/tmp/asc-map-after-*` build directories for all three supported architectures using the Step 1 commands. Compare the relevant link commands after normalizing build-directory names.

```bash
ninja -C /tmp/asc-map-after-2002 -t commands > /tmp/asc-map-after-2002.commands
diff -u \
  <(sed 's/asc-map-before/asc-map-after/g' /tmp/asc-map-before-2002.commands) \
  /tmp/asc-map-after-2002.commands
```

Expected: no semantic differences in CPU directory paths or selected model libraries.

- [x] **Step 4: Verify unsupported architectures still fail**

```bash
cmake -S ../ascendc_smoke/asc_e2e/e2e_bbit_51/cmake_tests/default_suffix \
  -B /tmp/asc-map-invalid -G Ninja \
  -DCMAKE_MODULE_PATH=$PWD/cmake/asc/asc_modules \
  -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-invalid
```

Expected: configuration fails with `Unsupported ASC architecture for CPU mode`.

- [x] **Step 5: Run CPU library regressions**

Build and run the official static-library and shared-library CPU examples sequentially with `dav-2201`.

```bash
cmake --build /tmp/cann_installed_static_cpu -j2
cmake -E chdir /tmp/cann_installed_static_cpu ./demo
cmake --build /tmp/cann_installed_shared_cpu -j2
cmake -E chdir /tmp/cann_installed_shared_cpu ./demo
```

Expected: both programs print `test pass!` and exit zero.

- [x] **Step 6: Check and commit the implementation**

```bash
git diff --check
git add cmake/asc/asc_modules/CMakeASCInformation.cmake
git commit -m "refactor: simplify ASC CPU architecture mapping"
```

Expected: one implementation file is committed with no whitespace errors.
