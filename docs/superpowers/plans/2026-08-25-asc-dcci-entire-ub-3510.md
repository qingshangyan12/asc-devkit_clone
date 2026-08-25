# Ascend 950 asc_dcci_entire_ub Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add Vector Core support for `asc_dcci_entire_ub()` on Ascend 950/DAV 3510 and prove its UB/DCache consistency behavior on NPU.

**Architecture:** Mirror the existing DAV 2201 DCCI intrinsic mapping in the DAV 3510 implementation and cover it with the existing gTest/MockCPP C API target. Validate real behavior with a single AIV kernel that creates stale Scalar DCache copies for two UB cache lines, observes old values before DCCI, invalidates the entire UB cache target, and observes new values after DCCI plus DSB.

**Tech Stack:** AscendC C API, Bisheng `dav-3510`, ACL runtime, CMake, gTest/MockCPP, Bash, Markdown.

---

### Task 1: Record API evidence and establish the failing test

**Files:**
- Modify: `tests/api/c_api/npu_arch_3510/cache_ctrl/test_dcci.cpp`
- Reference: `docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md`
- Reference: `include/c_api/cache_ctrl/cache_ctrl.h`
- Reference: `impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl/asc_dcci_impl.h`

- [ ] **Step 1: Record the selected API evidence in working notes**

Use these verified mappings:

| API | Documentation | Declaration | Selected use | Target and constraints |
| --- | --- | --- | --- | --- |
| `asc_dcci_entire_ub` | `docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md` | `include/c_api/cache_ctrl/cache_ctrl.h` | no-argument entire UB DCCI | DAV 3510 AIV only; `CTRL[49]=1`; follow with DSB |
| `asc_get_ctrl` / `asc_set_ctrl` | `docs/zh/api/SIMD-API/c_api/sys_var/asc_get_ctrl.md`, `asc_set_ctrl.md` | `include/c_api/spr/spr.h` | preserve all bits and set bit 49 | DAV 3510; Scalar pipeline |
| `asc_duplicate_scalar` | `docs/zh/api/SIMD-API/c_api/reg/data_fill/asc_duplicate_scalar.md` | `include/c_api/reg_compute/compute/reg_broadcast.h` | `vector_int32_t`, scalar value, b32 mask | DAV 3510 AIV; call only in `__simd_vf__` |
| `asc_update_mask_b32` | `docs/zh/api/SIMD-API/c_api/reg/reg_data_types/asc_update_mask.md` | `include/c_api/reg_compute/compute/reg_mask.h` | 64 int32 elements | DAV 3510 AIV; call only in `__simd_vf__` |
| `asc_storealign` | `docs/zh/api/SIMD-API/c_api/reg/reg_store/asc_storealign.md` | `include/c_api/reg_compute/store/storealign.h` | aligned int32 vector store to UB | DAV 3510 AIV; destination 32-byte aligned |
| `asc_sync_notify` / `asc_sync_wait` | `docs/zh/api/SIMD-API/c_api/sync/asc_sync_notify.md`, `asc_sync_wait.md` | `include/c_api/sync/sync.h` | `PIPE_V -> PIPE_S`, paired `EVENT_ID0` | DAV 3510 AIV; paired arguments must match |
| `asc_sync_data_barrier` | `docs/zh/api/SIMD-API/c_api/sync/asc_sync_data_barrier.md` | `include/c_api/sync/sync.h` | literal `mem_dsb_t::DSB_UB` and `DSB_ALL` | DAV 3510; literal enum required |
| `asc_store_dev` | `docs/zh/api/SIMD-API/c_api/scalar_compute/asc_store_dev.md` | `include/c_api/scalar_compute/scalar_store.h` | int32 observations to GM | DAV 3510; aligned GM; bypasses DCache |
| `asc_init` | `docs/zh/api/SIMD-API/c_api/misc/asc_init.md` | `include/c_api/utils/sys_init.h` | first kernel operation | DAV 3510 |

- [ ] **Step 2: Add the failing 3510 unit test**

Append this behavior to `test_dcci.cpp`, following existing naming style:

```cpp
namespace {
void dcci_entire_ub_stub(__ubuf__ void* dstPtr, uint64_t entire, uint64_t type)
{
    EXPECT_EQ((__ubuf__ void*)0, dstPtr);
    EXPECT_EQ(cache_line_t::ENTIRE_DATA_CACHE, entire);
    EXPECT_EQ(dcci_dst_t::CACHELINE_UB, type);
}
} // namespace

TEST_F(TestAscCApiSimdDcci, dcci_ub_entire)
{
    MOCKER(dcci, void(__ubuf__ void*, uint64_t, uint64_t)).times(1).will(invoke(dcci_entire_ub_stub));
    asc_dcci_entire_ub();
    GlobalMockObject::verify();
}
```

- [ ] **Step 3: Build the DAV 3510 AIV C API target and verify RED**

Run:

```bash
cmake -S . -B build \
  -DENABLE_TEST=ON \
  -DTEST_MOD=ascendc_ut_c_api_ascend950pr_9599_AIV \
  -DASCEND_CANN_PACKAGE_PATH=/usr/local/Ascend/cann-9.2.0-beta.1
cmake --build build --target ascendc_ut_c_api_ascend950pr_9599_AIV -j16
```

Expected: compilation or linking fails because DAV 3510 declares `asc_dcci_entire_ub()` but does not define its public forwarding function or intrinsic mapping. Confirm the failure names `asc_dcci_entire_ub` rather than an unrelated dependency.

### Task 2: Implement the DAV 3510 mapping

**Files:**
- Modify: `impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dcci_impl.h`
- Modify: `impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`
- Test: `tests/api/c_api/npu_arch_3510/cache_ctrl/test_dcci.cpp`

- [ ] **Step 1: Add the minimal intrinsic wrapper**

Insert after `asc_dcci_entire_all_impl()`:

```cpp
__aicore__ inline void asc_dcci_entire_ub_impl()
{
    dcci((__ubuf__ int64_t*)0, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_UB);
}
```

- [ ] **Step 2: Add the public forwarding function**

Insert after `asc_dcci_entire_all()`:

```cpp
__aicore__ inline void asc_dcci_entire_ub() { asc_dcci_entire_ub_impl(); }
```

Do not add `ASC_IS_AIV`; callers must restrict use to Vector Core.

- [ ] **Step 3: Rebuild and verify GREEN**

Run:

```bash
cmake --build build --target ascendc_ut_c_api_ascend950pr_9599_AIV -j16
```

Expected: target builds and all DAV 3510 AIV C API tests, including `dcci_ub_entire`, pass.

### Task 3: Create the AIV functional sample and manual test driver

**Files:**
- Create: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/dcci_entire_ub.asc`
- Create: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/CMakeLists.txt`
- Create: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/test.sh`

- [ ] **Step 1: Implement the device-side cache experiment**

Use these constants and output layout:

```cpp
constexpr uint32_t ELEMENTS_PER_VECTOR = 64;
constexpr uint32_t SECOND_LINE_OFFSET = 32;
constexpr uint32_t OUTPUT_COUNT = 7;
constexpr uint64_t CTRL_UB_DCACHE_MASK = 1ULL << 49;
constexpr int32_t OLD_VALUE = 11;
constexpr int32_t NEW_VALUE = 101;

enum OutputIndex : uint32_t {
    CTRL_BIT = 0,
    WARM_LINE0,
    WARM_LINE1,
    BEFORE_LINE0,
    BEFORE_LINE1,
    AFTER_LINE0,
    AFTER_LINE1
};
```

Create a `__simd_vf__` helper that fills 64 aligned int32 elements with the Reg API:

```cpp
__simd_vf__ inline void fill_ub(__ubuf__ int32_t* dst, int32_t value)
{
    vector_int32_t data;
    uint32_t count = ELEMENTS_PER_VECTOR;
    vector_bool mask = asc_update_mask_b32(count);
    asc_duplicate_scalar(data, value, mask);
    asc_storealign(dst, data, mask);
}
```

Create a `__vector__ __global__` kernel that:

```cpp
asc_init();
__ubuf__ int32_t data[ELEMENTS_PER_VECTOR];
fill_ub(data, OLD_VALUE);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);

const uint64_t originalCtrl = static_cast<uint64_t>(asc_get_ctrl());
asc_set_ctrl(originalCtrl | CTRL_UB_DCACHE_MASK);
auto volatileData = reinterpret_cast<__ubuf__ volatile int32_t*>(data);
const int32_t warm0 = volatileData[0];
const int32_t warm1 = volatileData[SECOND_LINE_OFFSET];

fill_ub(data, NEW_VALUE);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
const int32_t before0 = volatileData[0];
const int32_t before1 = volatileData[SECOND_LINE_OFFSET];

asc_dcci_entire_ub();
asc_sync_data_barrier(mem_dsb_t::DSB_UB);
const int32_t after0 = volatileData[0];
const int32_t after1 = volatileData[SECOND_LINE_OFFSET];
asc_set_ctrl(originalCtrl);

asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
asc_store_dev(output + CTRL_BIT, static_cast<int32_t>(((originalCtrl | CTRL_UB_DCACHE_MASK) >> 49) & 1));
asc_store_dev(output + WARM_LINE0, warm0);
asc_store_dev(output + WARM_LINE1, warm1);
asc_store_dev(output + BEFORE_LINE0, before0);
asc_store_dev(output + BEFORE_LINE1, before1);
asc_store_dev(output + AFTER_LINE0, after0);
asc_store_dev(output + AFTER_LINE1, after1);
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
```

If the compiler requires explicit volatile pointer declarations or a different accepted cast form, retain the same observable loads and confirm generated code still contains separate UB loads before and after DCCI.

- [ ] **Step 2: Implement checked ACL host execution**

The host must initialize ACL, set device 0, allocate `OUTPUT_COUNT * sizeof(int32_t)`, initialize it to a sentinel, launch one block, synchronize, copy back, free memory, reset the device, and finalize ACL. Check every return code and print the failing operation.

Verify the exact expected vector:

```cpp
const std::array<int32_t, OUTPUT_COUNT> expected = {
    1, OLD_VALUE, OLD_VALUE, OLD_VALUE, OLD_VALUE, NEW_VALUE, NEW_VALUE
};
```

Print every observation. Return zero and print `test pass!` only when all seven values match; otherwise return nonzero and print `test failed!`.

- [ ] **Step 3: Add the CMake project**

Use the repository sample pattern:

```cmake
cmake_minimum_required(VERSION 3.16)
set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
find_package(ASC REQUIRED)
project(dcci_entire_ub_sample LANGUAGES ASC CXX)
add_executable(demo dcci_entire_ub.asc)
target_link_libraries(demo PRIVATE tiling_api register platform m)
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
```

- [ ] **Step 4: Add `test.sh`**

The script must use `set -euo pipefail`, resolve the sample and repository paths, default CANN to `/usr/local/Ascend/cann-9.2.0-beta.1`, and verify:

```bash
expected_include="${repo_dir}/include/c_api"
expected_impl="${repo_dir}/impl/c_api"
cann_asc="${cann_path}/x86_64-linux/asc"
[[ -L "${cann_asc}/include/c_api" ]]
[[ -L "${cann_asc}/impl/c_api" ]]
[[ "$(readlink -f "${cann_asc}/include/c_api")" == "${expected_include}" ]]
[[ "$(readlink -f "${cann_asc}/impl/c_api")" == "${expected_impl}" ]]
```

Then recreate only `${sample_dir}/build`, configure `dav-3510` NPU mode, build, run `demo` through `tee`, and require both exit code zero and a line exactly equal to `test pass!`.

### Task 4: Switch the CANN C API subtrees

**Files outside Git:**
- Move: `/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/include/c_api`
- Move: `/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/impl/c_api`
- Create symlinks at the two original paths

- [ ] **Step 1: Verify exact sources and backup targets**

Require both sources to be real directories, neither source to be a symlink, and neither backup to exist:

```text
include/c_api.cann-9.2.0-beta.1.backup-before-asc-dcci-entire-ub
impl/c_api.cann-9.2.0-beta.1.backup-before-asc-dcci-entire-ub
```

- [ ] **Step 2: Move the directories and create absolute links**

Move each source to its backup, then link:

```text
/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/include/c_api
  -> /home/g00954335/asc-devkit_GRJ/include/c_api
/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/impl/c_api
  -> /home/g00954335/asc-devkit_GRJ/impl/c_api
```

- [ ] **Step 3: Verify both links**

Run `readlink -f` for both paths and require exact matches before compiling the sample.

### Task 5: Build, run, and stabilize the functional test

**Files:**
- Test: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/test.sh`

- [ ] **Step 1: Run the manual verification script**

Run:

```bash
bash examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/test.sh
```

Expected output includes both stale pre-DCCI values (`11`) and both fresh post-DCCI values (`101`), followed by `test pass!`.

- [ ] **Step 2: Repeat the NPU test**

Run the script at least three times from clean builds. Every run must meet the strict before/after criteria. A run where pre-DCCI already reads `101` is a failed proof and must be investigated rather than accepted.

- [ ] **Step 3: Inspect the compiler output if behavior is ambiguous**

Use the available Bisheng/LLVM disassembly tooling to confirm two Scalar UB reads occur on each side of `CACHELINE_UB` DCCI and that DSB follows DCCI. Do not weaken the host expected values to make the test pass.

### Task 6: Document support, reproduction, and merge conclusion

**Files:**
- Modify: `docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sys_var/asc_set_ctrl.md`
- Create: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/README.md`
- Create: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/README.md`
- Create: `examples/02_simd_c_api/03_c_api/04_cache_ctrl/dcci_entire_ub/README_en.md`
- Modify: `examples/02_simd_c_api/03_c_api/README.md`

- [ ] **Step 1: Update API constraints**

Replace the 950 “not supported” statement with: Ascend 950 supports `asc_dcci_entire_ub()` only on Vector Core because AIC has no UB. Add the requirement to follow UB DCCI with `asc_sync_data_barrier(mem_dsb_t::DSB_UB)` before dependent Scalar reads.

Correct `asc_set_ctrl.md` so the constraint list includes `CTRL[49]`, matching its own parameter table and this supported use case.

- [ ] **Step 2: Write sample documentation**

Document the two-cache-line sequence, exact expected observations, CANN 9.2.0-beta.1, `dav-3510`, the C API symlink prerequisite, and `bash test.sh` reproduction command in Chinese and English.

- [ ] **Step 3: Record the merge decision**

After fresh verification, state “可以合入” only if the unit test, compile, three NPU runs, and compatibility regression all pass. Include the actual observed before/after values and commands. Otherwise state “暂不可合入” and name the failing evidence.

- [ ] **Step 4: Update sample indexes**

Add `04_cache_ctrl` to the C API root README and `dcci_entire_ub` to the cache control README.

### Task 7: Final regression and repository checks

**Files:**
- Verify all changed implementation, test, example, and documentation files

- [ ] **Step 1: Re-run the DAV 3510 AIV C API target**

```bash
cmake --build build --target ascendc_ut_c_api_ascend950pr_9599_AIV -j16
```

- [ ] **Step 2: Compile an existing DAV 3510 C API sample**

Configure and build `examples/02_simd_c_api/03_c_api/02_reg_vector_compute/abs` against the linked C API subtree. Compilation must complete without new errors.

- [ ] **Step 3: Run documentation and shell checks**

Run the repository Markdown checker through the `doc-checker` skill, `bash -n` on `test.sh`, and `git diff --check`.

- [ ] **Step 4: Review the final diff and worktree**

Confirm no generated `build` files are tracked, no user-owned requirement file was modified or committed, public include has no source diff, and only the requested CANN paths were replaced by links.

- [ ] **Step 5: Commit implementation after all verification succeeds**

```bash
git add impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h \
  impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl/asc_dcci_impl.h \
  tests/api/c_api/npu_arch_3510/cache_ctrl/test_dcci.cpp \
  docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md \
  docs/zh/api/SIMD-API/c_api/sys_var/asc_set_ctrl.md \
  examples/02_simd_c_api/03_c_api/README.md \
  examples/02_simd_c_api/03_c_api/04_cache_ctrl
git commit -m "feat: support asc_dcci_entire_ub on Ascend 950"
```
