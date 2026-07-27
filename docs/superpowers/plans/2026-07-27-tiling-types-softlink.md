# Tiling Types Softlink Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `../utils/types.h` resolvable for tiling headers accessed through `asc/include/tiling`.

**Architecture:** Extend the existing install-time CMake script with one directory creation and one relative symbolic link. Validate the generated install script in an isolated temporary prefix.

**Tech Stack:** CMake install scripts, POSIX symbolic links

---

### Task 1: Add the compatibility link

**Files:**
- Modify: `impl/adv_api/cmake/tiling_headers.cmake:22`
- Test: temporary CMake script under `/tmp`

- [ ] **Step 1: Write and run the failing installation test**

Configure `impl/adv_api/cmake/tiling_headers.cmake` with
`INSTALL_LIBRARY_DIR=.` into a temporary script, run it with a temporary
`CMAKE_INSTALL_PREFIX`, and assert that
`asc/include/tiling/utils/types.h` is a symbolic link whose stored target is
`../../adv_api/utils/types.h`.

Expected before the fix: FAIL because `asc/include/tiling/utils/types.h` does not exist.

- [ ] **Step 2: Add the minimal CMake rules**

Add immediately after creation of `asc/include/tiling`:

```cmake
file(MAKE_DIRECTORY
     ${ASCENDC_INSTALL_BASE_PATH}/asc/include/tiling/utils)
file(CREATE_LINK ../../adv_api/utils/types.h
     ${ASCENDC_INSTALL_BASE_PATH}/asc/include/tiling/utils/types.h
     SYMBOLIC)
```

- [ ] **Step 3: Run the installation test again**

Expected after the fix: PASS; the stored target is exactly
`../../adv_api/utils/types.h`, the target exists, and
`asc/include/tiling/math/../utils/types.h` is readable.

- [ ] **Step 4: Run static checks**

Run:

```bash
git diff --check
cmake -P /tmp/tiling-types-softlink-test.cmake
```

Expected: both commands exit with status 0.

- [ ] **Step 5: Commit the implementation**

```bash
git add impl/adv_api/cmake/tiling_headers.cmake
git commit -m "fix: add tiling types compatibility link"
```
