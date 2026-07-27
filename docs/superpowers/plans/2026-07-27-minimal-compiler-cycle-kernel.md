# Minimal Compiler Cycle Kernel Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build and run a minimal AscendC Kernel that stores the compiler-reported Kernel entry cycle and current cycle into two GM outputs for Host-side retrieval.

**Architecture:** A single `.asc` translation unit contains a two-store Vector Kernel and the ACL Host launcher. A small CMake target uses the locally installed devkit headers, targets the machine's `dav-2201` architecture, and deliberately omits `ASCENDC_TIME_STAMP_ON`.

**Tech Stack:** AscendC ASC language, bisheng compiler, CMake ASC language support, ACL Runtime, 910B4 NPU.

---

### Task 1: Establish The Failing Output Contract

**Files:**
- Expected later: `compiler_cycle_minimal/cycle_run.log`

- [x] **Step 1: Run the output contract before implementation**

Run:

```bash
test -f compiler_cycle_minimal/cycle_run.log && \
rg -n '^entry_cycle: [0-9]+$|^current_cycle: [0-9]+$|^delta: [0-9]+$' compiler_cycle_minimal/cycle_run.log
```

Expected: FAIL because `compiler_cycle_minimal/cycle_run.log` does not exist.

### Task 2: Add The Minimal Kernel And Host Launcher

**Files:**
- Create: `compiler_cycle_minimal/CMakeLists.txt`
- Create: `compiler_cycle_minimal/read_cycles.asc`

- [x] **Step 1: Add the CMake target**

Create `compiler_cycle_minimal/CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture")

find_package(ASC REQUIRED)

project(compiler_cycle_minimal LANGUAGES ASC CXX)

add_executable(demo
    read_cycles.asc
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<COMPILE_LANGUAGE:ASC>:-g>
    $<$<COMPILE_LANGUAGE:ASC>:-O2>
    $<$<COMPILE_LANGUAGE:ASC>:-save-temps=obj>
)

target_include_directories(demo BEFORE PRIVATE
    /tmp/asc-devkit-timestamp-install/cann/aarch64-linux/tikcpp/tikcfw
    /tmp/asc-devkit-timestamp-install/cann/aarch64-linux/asc/include
    /usr/local/Ascend/cann-9.1.0/include
)

target_link_directories(demo PRIVATE
    /usr/local/Ascend/cann-9.1.0/lib64
)
```

- [x] **Step 2: Add the Device Kernel and Host launcher**

Create `compiler_cycle_minimal/read_cycles.asc`:

```cpp
#include <cstdint>
#include <iostream>

#include "acl/acl.h"
#include "cce_aicore_intrinsics.h"

namespace {
void PreserveFirstError(aclError candidate, aclError& result)
{
    if (result == ACL_SUCCESS && candidate != ACL_SUCCESS) {
        result = candidate;
    }
}
} // namespace

extern "C" __global__ __vector__ void read_compiler_cycles(
    __gm__ uint64_t* entryCycle, __gm__ uint64_t* currentCycle)
{
    *entryCycle = static_cast<uint64_t>(__get_entry_sys_cnt());
    *currentCycle = static_cast<uint64_t>(get_sys_cnt());
}

int main()
{
    constexpr int32_t deviceId = 0;
    constexpr size_t cycleBytes = sizeof(uint64_t);
    aclrtStream stream = nullptr;
    uint64_t* entryDevice = nullptr;
    uint64_t* currentDevice = nullptr;
    uint64_t entryCycle = 0;
    uint64_t currentCycle = 0;
    bool deviceSet = false;

    aclError ret = aclInit(nullptr);
    if (ret != ACL_SUCCESS) {
        std::cerr << "aclInit failed: " << ret << std::endl;
        return 1;
    }

    ret = aclrtSetDevice(deviceId);
    deviceSet = (ret == ACL_SUCCESS);
    ret = (ret == ACL_SUCCESS) ? aclrtCreateStream(&stream) : ret;
    ret = (ret == ACL_SUCCESS)
              ? aclrtMalloc(reinterpret_cast<void**>(&entryDevice), cycleBytes, ACL_MEM_MALLOC_HUGE_FIRST)
              : ret;
    ret = (ret == ACL_SUCCESS)
              ? aclrtMalloc(reinterpret_cast<void**>(&currentDevice), cycleBytes, ACL_MEM_MALLOC_HUGE_FIRST)
              : ret;

    if (ret == ACL_SUCCESS) {
        read_compiler_cycles<<<1, 0, stream>>>(entryDevice, currentDevice);
        ret = aclrtSynchronizeStream(stream);
    }
    ret = (ret == ACL_SUCCESS)
              ? aclrtMemcpy(&entryCycle, cycleBytes, entryDevice, cycleBytes, ACL_MEMCPY_DEVICE_TO_HOST)
              : ret;
    ret = (ret == ACL_SUCCESS)
              ? aclrtMemcpy(&currentCycle, cycleBytes, currentDevice, cycleBytes, ACL_MEMCPY_DEVICE_TO_HOST)
              : ret;

    if (entryDevice != nullptr) {
        PreserveFirstError(aclrtFree(entryDevice), ret);
    }
    if (currentDevice != nullptr) {
        PreserveFirstError(aclrtFree(currentDevice), ret);
    }
    if (stream != nullptr) {
        PreserveFirstError(aclrtDestroyStream(stream), ret);
    }
    if (deviceSet) {
        PreserveFirstError(aclrtResetDevice(deviceId), ret);
    }
    PreserveFirstError(aclFinalize(), ret);

    if (ret != ACL_SUCCESS) {
        std::cerr << "runtime execution failed: " << ret << std::endl;
        return 1;
    }
    if (entryCycle == 0 || currentCycle == 0 || currentCycle < entryCycle) {
        std::cerr << "invalid cycle values" << std::endl;
        return 1;
    }

    std::cout << "entry_cycle: " << entryCycle << std::endl;
    std::cout << "current_cycle: " << currentCycle << std::endl;
    std::cout << "delta: " << currentCycle - entryCycle << std::endl;
    return 0;
}
```

- [x] **Step 3: Configure against the local devkit package**

Run:

```bash
ASCEND_HOME_PATH=/tmp/asc-devkit-timestamp-install/cann \
cmake -S compiler_cycle_minimal -B /tmp/compiler-cycle-minimal-build \
  -DCMAKE_ASC_RUN_MODE=npu \
  -DCMAKE_ASC_ARCHITECTURES=dav-2201
```

Expected: configuration succeeds and reports the local package as `ASCEND_CANN_PACKAGE_LINUX_PATH`.

- [x] **Step 4: Build and inspect the compile command**

Run:

```bash
ASCEND_HOME_PATH=/tmp/asc-devkit-timestamp-install/cann \
cmake --build /tmp/compiler-cycle-minimal-build --verbose -j2
```

Expected: build succeeds, uses `--npu-arch=dav-2201`, and the command does not contain `ASCENDC_TIME_STAMP_ON`.

- [x] **Step 5: Confirm the generated Device source uses only the requested compiler interfaces**

Run:

```bash
sed -n '/void read_compiler_cycles/,/^}/p' \
  /tmp/compiler-cycle-minimal-build/CMakeFiles/demo.dir/*-vec.asci | \
rg -n '__get_entry_sys_cnt|get_sys_cnt|InitSocState|DataCopy|TPipe|TQue|TBuf|PrintTimeStamp'
```

Expected: the Kernel body contains `__get_entry_sys_cnt()` and `get_sys_cnt()`; none of the excluded API names occur in the Kernel body.

### Task 3: Run On NPU And Turn The Contract Green

**Files:**
- Create: `compiler_cycle_minimal/cycle_run.log`

- [x] **Step 1: Run the binary on the real NPU**

Run:

```bash
source /usr/local/Ascend/cann-9.1.0/set_env.sh
ASCEND_HOME_PATH=/tmp/asc-devkit-timestamp-install/cann \
timeout 30 /tmp/compiler-cycle-minimal-build/demo \
  > compiler_cycle_minimal/cycle_run.log 2>&1
```

Expected: process exits with code 0.

- [x] **Step 2: Re-run the output contract**

Run:

```bash
test "$(rg -c '^entry_cycle: [0-9]+$|^current_cycle: [0-9]+$|^delta: [0-9]+$' \
  compiler_cycle_minimal/cycle_run.log)" -eq 3
```

Expected: PASS with exactly three output records.

- [x] **Step 3: Inspect the captured values**

Run:

```bash
sed -n '1,20p' compiler_cycle_minimal/cycle_run.log
```

Expected: both cycles are nonzero, current cycle is not less than entry cycle, and delta is printed.

- [x] **Step 4: Commit the isolated sample**

```bash
git add compiler_cycle_minimal/CMakeLists.txt \
  compiler_cycle_minimal/read_cycles.asc \
  compiler_cycle_minimal/cycle_run.log \
  docs/superpowers/plans/2026-07-27-minimal-compiler-cycle-kernel.md
git commit -m "test: add minimal compiler cycle NPU sample"
```
