# OOM Storage-Shape Magic Number Update Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Change the OOM storage-shape extension-header magic byte from `1` to `0x4F` and lock the protocol value down with a regression test.

**Architecture:** Keep the existing two-byte extension-header layout and parsing flow unchanged. Update the single shared Device-side constant, then make the existing C++ unit test independently assert the specified protocol byte and reject the legacy value.

**Tech Stack:** AscendC/C++, GoogleTest, CMake

---

### Task 1: Update and verify the OOM storage-shape magic byte

**Files:**
- Modify: `tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp:97`
- Modify: `impl/basic_api/utils/kernel_utils_macros.h:481`

- [x] **Step 1: Write the failing protocol regression test**

In `TestDMAOom.TestOOMStorageShapeTensorDescriptor`, add a literal-value assertion before allocating the test buffers:

```cpp
EXPECT_EQ(g_oomStorageShapeMagic, 0x4FU);
```

Replace the existing generic invalid-magic check at the end of that test with a check for the legacy protocol value:

```cpp
OOMInit();
uint8_t* legacyMagicCursor = metadata;
WritePacked<uint8_t>(metadata, 1U);
EXPECT_FALSE(OOMHasStorageShapeHeader(legacyMagicCursor));
EXPECT_EQ(g_oomAddrArange.count, 0);
```

- [x] **Step 2: Build and run the test to verify RED**

Run:

```bash
cmake --build build --target ascendc_ut_basic_api_ascend910B1_AIV -j4
build/tests/api/basic_api/ascendc_ut_basic_api_ascend910B1_AIV \
  --gtest_filter=TestDMAOom.TestOOMStorageShapeTensorDescriptor
```

Expected: the build succeeds, then the test fails because `g_oomStorageShapeMagic` is still `1`; both the `0x4F` equality and legacy-value rejection assertions expose the old behavior.

- [x] **Step 3: Implement the minimal production change**

In `impl/basic_api/utils/kernel_utils_macros.h`, change only the storage-shape magic constant:

```cpp
constexpr uint8_t g_oomStorageShapeMagic = 0x4FU;
```

Do not modify the header size, versions, descriptor types, `ASCENDC_OOM` feature value, JSON metadata, or Python code generation.

- [x] **Step 4: Rebuild and verify GREEN**

Run:

```bash
cmake --build build --target ascendc_ut_basic_api_ascend910B1_AIV -j4
build/tests/api/basic_api/ascendc_ut_basic_api_ascend910B1_AIV \
  --gtest_filter='TestDMAOom.TestOOMStorageShape*'
```

Expected: both storage-shape tests pass.

- [x] **Step 5: Run the complete OOM fixture and static checks**

Run:

```bash
build/tests/api/basic_api/ascendc_ut_basic_api_ascend910B1_AIV \
  --gtest_filter='TestDMAOom.*'
git diff --check
git diff -- impl/basic_api/utils/kernel_utils_macros.h \
  tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp
```

Expected: all `TestDMAOom` tests pass, `git diff --check` reports no errors, and the source diff contains only the constant update plus the focused test assertions.

- [x] **Step 6: Commit the implementation**

```bash
git add impl/basic_api/utils/kernel_utils_macros.h \
  tests/api/basic_api/ascendc_case_ascend910b1/ascendc_case_ascend910b1_aiv/test_dma_oom.cpp
git commit -m "fix: update OOM storage shape magic"
```
