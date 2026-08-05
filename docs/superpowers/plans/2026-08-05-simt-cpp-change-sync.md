# SIMT C++ Change Sync Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Synchronize eight missed C-side behavior changes into the existing SIMT C++ API without adding new API families.

**Architecture:** Keep the public C++ interfaces unchanged and update their existing DAV 3510 implementations to match the final C-side behavior. Extend existing CPU-debug unit tests for behavioral regressions and the existing ASC compiler target for device-only shuffle signature/type coverage.

**Tech Stack:** C++17, Ascend C (`.asc`), GoogleTest, CMake, CANN/Ascend compiler toolchain.

---

### Task 1: Add math and Bessel regression tests

**Files:**
- Modify: `tests/api/simt_api/ascendc_case_ascend950pr_9599_simt/test_operator_simt_math.cpp`

- [ ] **Step 1: Add failing boundary tests**

Add GoogleTest cases that verify:

```cpp
EXPECT_FALSE(std::signbit(Simt::CopySign(1.0f, 0.0f)));
float next = Simt::NextAfter(0.0f, 1.0f);
uint32_t nextBits = 0;
std::memcpy(&nextBits, &next, sizeof(nextBits));
EXPECT_EQ(nextBits, 1U);
EXPECT_TRUE(std::isnan(Simt::NextAfter(NAN, 1.0f)));
EXPECT_TRUE(std::isnan(Simt::Yn(-1, 0.0f)));
```

Also compare `Simt::Norm` and `Simt::Yn` against the corresponding C API for representative arrays and recurrence inputs that execute the four-way Norm loop and the three-way Yn recurrence.

- [ ] **Step 2: Run the focused test and verify RED**

Run:

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -S . -B build-cpp-sync -DENABLE_TEST=ON -DSIMT_PRODUCT_TYPE_LIST=ascend950pr_9599
cmake --build build-cpp-sync --target ascendc_ut_simt_api_ascend950pr_9599 -j
ctest --test-dir build-cpp-sync -R ascendc_ut_simt_api_ascend950pr_9599 --output-on-failure
```

Expected: the focused SIMT test fails on the copied pre-fix C++ behavior.

### Task 2: Synchronize math and Bessel behavior

**Files:**
- Modify: `impl/simt_api/cpp/dav_3510/kernel_simt_math_impl.h`
- Modify: `impl/simt_api/cpp/dav_3510/kernel_simt_transcendental_impl.h`
- Modify: `impl/simt_api/cpp/dav_3510/kernel_simt_bessel_impl.h`
- Test: `tests/api/simt_api/ascendc_case_ascend950pr_9599_simt/test_operator_simt_math.cpp`

- [ ] **Step 1: Fix CopySign and NextAfter**

Change `CopySignImpl` to treat positive zero as a non-negative sign source. Update `NextAfterImpl` to return NaN when either input is NaN and to emit the smallest positive or negative subnormal when `x` is zero.

- [ ] **Step 2: Synchronize final Norm implementation**

Update `NormImpl` to process four values per main-loop iteration, preserve infinity handling, process the remainder, and use division by the maximum magnitude as required by C-side follow-up commit `1ea17d35b`.

- [ ] **Step 3: Synchronize final Yn implementation**

Check invalid `n` and `x` before the zero-input special case. Replace `YnCase3` with the C-side three-step recurrence and scaling logic from `f89c96ca0`.

- [ ] **Step 4: Run the focused test and verify GREEN**

Run the Task 1 build and CTest commands again.

Expected: all focused SIMT math tests pass.

- [ ] **Step 5: Commit math synchronization**

```bash
git add impl/simt_api/cpp/dav_3510/kernel_simt_math_impl.h \
        impl/simt_api/cpp/dav_3510/kernel_simt_transcendental_impl.h \
        impl/simt_api/cpp/dav_3510/kernel_simt_bessel_impl.h \
        tests/api/simt_api/ascendc_case_ascend950pr_9599_simt/test_operator_simt_math.cpp
git commit -m "fix: sync SIMT C++ math behavior with C API"
```

### Task 3: Add shuffle compiler regression coverage

**Files:**
- Modify: `tests/api/simt_api/math_cpp/math_cpp.asc`

- [ ] **Step 1: Add failing integer shuffle compile coverage**

Add a device helper that instantiates `WarpShflSync`, `WarpShflUpSync`, `WarpShflDownSync`, and `WarpShflXorSync` with `int32_t` and a non-default width.

- [ ] **Step 2: Build and verify RED for the old compiler invocation**

Run:

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -S tests/api/simt_api/math_cpp -B build-warp-cpp-sync
cmake --build build-warp-cpp-sync -j
```

Expected: ASC compilation fails because the C++ implementation still calls the obsolete two-argument shuffle built-ins.

### Task 4: Synchronize shuffle compiler invocation

**Files:**
- Modify: `impl/simt_api/cpp/dav_3510/kernel_simt_warp_level_impl.h`
- Test: `tests/api/simt_api/math_cpp/math_cpp.asc`

- [ ] **Step 1: Replace packed control arguments**

Change the four device branches to call `__shfl*` with `(value, lane_or_delta, width)`, matching C-side commit `9673fc23f`. Remove constants that become unused.

- [ ] **Step 2: Build and verify GREEN**

Run the Task 3 CMake build again.

Expected: the integer shuffle compile coverage succeeds.

### Task 5: Add and implement BF16 shuffle support

**Files:**
- Modify: `tests/api/simt_api/math_cpp/math_cpp.asc`
- Modify: `impl/simt_api/cpp/dav_3510/kernel_simt_warp_level_impl.h`

- [ ] **Step 1: Add failing BF16 compile coverage**

Instantiate all four shuffle families with `bfloat16_t` and `bfloat16x2_t` in the device helper.

- [ ] **Step 2: Build and verify RED**

Run the Task 3 CMake build.

Expected: compilation fails at the C++ shuffle type constraint.

- [ ] **Step 3: Extend existing type constraints**

Add `bfloat16_t` and `bfloat16x2_t` to the supported types and diagnostics for all four shuffle implementations, matching C-side commit `7718de278`.

- [ ] **Step 4: Build and verify GREEN**

Run the Task 3 CMake build again.

Expected: all integer and BF16 shuffle instantiations compile.

- [ ] **Step 5: Commit shuffle synchronization**

```bash
git add impl/simt_api/cpp/dav_3510/kernel_simt_warp_level_impl.h tests/api/simt_api/math_cpp/math_cpp.asc
git commit -m "fix: sync SIMT C++ shuffle updates"
```

### Task 6: Verify and deliver

**Files:**
- Verify all files changed since `upstream/master`

- [ ] **Step 1: Run formatting and diff checks**

```bash
git diff --check upstream/master...HEAD
git status --short
```

Expected: no whitespace errors and only intended files are present.

- [ ] **Step 2: Run full relevant verification**

Rebuild and run the SIMT unit target for `ascend950pr_9599`, then rebuild the standalone `math_cpp` ASC target from clean build directories.

Expected: both builds succeed and the SIMT unit test reports zero failures.

- [ ] **Step 3: Push and open the PR**

Push `cpp_sync` to `origin`, compose a PR body containing the six source commits and eight missed logical items, and create the PR against `cann/asc-devkit:master`.
