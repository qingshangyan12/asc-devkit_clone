#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "data_fill"
EXAMPLE_PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(file_name: str) -> str:
    rel = f"reg/data_fill/{file_name}.asc"
    binary = f"/tmp/c_api_reg_data_fill_{file_name}"
    return f'''// Build/run ({EXAMPLE_PREFIX}/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {EXAMPLE_PREFIX}/{rel} -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
template <typename T>
void PrintData(const char* label, const std::vector<T>& values, uint32_t offset = 0)
{{
    std::cout << label << ":";
    const size_t remaining = values.size() - offset;
    const size_t count = remaining < 8 ? remaining : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +values[offset + i];
    if (remaining > count) std::cout << " ...";
    std::cout << std::endl;
}}
'''


def scalar_source() -> str:
    name = "asc_duplicate_scalar"
    return header(name) + '''
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr float FILL_VALUE = 3.5f;

__simd_vf__ inline void Fill(__ubuf__ float* dst, __ubuf__ float* src)
{
    vector_float dst_reg;
    vector_float src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(dst_reg, dst);
    asc_loadalign(src_reg, src);
    asc_duplicate_scalar(dst_reg, FILL_VALUE, mask);
    asc_storealign(dst, dst_reg, mask);
}

__global__ __vector__ void AscDuplicateScalarKernel(__gm__ float* dst, __gm__ float* src)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(dst_local, dst, sizeof(dst_local));
    asc_copy_gm2ub_align(src_local, src, sizeof(src_local));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Fill(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, sizeof(dst_local));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT, -1.0f);
    std::vector<float> golden(ELEMENT_COUNT, FILL_VALUE);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = static_cast<float>(i) * 0.25f + 1.0f;

    aclInit(nullptr);
    aclrtSetDevice(0);
    float *src_device = nullptr, *dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), src.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(float), src.data(), src.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(dst_device, output.size() * sizeof(float), output.data(), output.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscDuplicateScalarKernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    PrintData("Input", src);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_duplicate_scalar passed."
                        : "[Failed] asc_duplicate_scalar failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def highest_source() -> str:
    name = "asc_duplicate"
    return header(name) + '''
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t ACTIVE_COUNT = 4;
constexpr uint32_t RESULT_COUNT = 3;
constexpr uint32_t LOWEST_ZERO_OFFSET = 0;
constexpr uint32_t HIGHEST_ZERO_OFFSET = ELEMENT_COUNT;
constexpr uint32_t HIGHEST_MERGE_OFFSET = ELEMENT_COUNT * 2;
constexpr float INITIAL_VALUE = -7.0f;

__simd_vf__ inline void DuplicateModes(__ubuf__ float* dst, __ubuf__ float* src)
{
    vector_float src_reg;
    vector_float lowest_zero_reg;
    vector_float highest_zero_reg;
    vector_float highest_merge_reg;
    uint32_t active_count = ACTIVE_COUNT;
    uint32_t full_count = ELEMENT_COUNT;
    vector_bool active_mask = asc_update_mask_b32(active_count);
    vector_bool full_mask = asc_update_mask_b32(full_count);

    asc_loadalign(src_reg, src);
    asc_loadalign(lowest_zero_reg, dst + LOWEST_ZERO_OFFSET);
    asc_loadalign(highest_zero_reg, dst + HIGHEST_ZERO_OFFSET);
    asc_loadalign(highest_merge_reg, dst + HIGHEST_MERGE_OFFSET);

    asc_duplicate(lowest_zero_reg, src_reg, active_mask);
    vdup(highest_zero_reg, src_reg, active_mask, POS_HIGHEST, MODE_ZEROING);
    vdup(highest_merge_reg, src_reg, active_mask, POS_HIGHEST, MODE_MERGING);

    asc_storealign(dst + LOWEST_ZERO_OFFSET, lowest_zero_reg, full_mask);
    asc_storealign(dst + HIGHEST_ZERO_OFFSET, highest_zero_reg, full_mask);
    asc_storealign(dst + HIGHEST_MERGE_OFFSET, highest_merge_reg, full_mask);
}

__global__ __vector__ void AscDuplicateKernel(__gm__ float* dst, __gm__ float* src)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT * RESULT_COUNT];
    __ubuf__ float src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(dst_local, dst, sizeof(dst_local));
    asc_copy_gm2ub_align(src_local, src, sizeof(src_local));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    DuplicateModes(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, sizeof(dst_local));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT * RESULT_COUNT, INITIAL_VALUE);
    std::vector<float> golden(output.size());
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = static_cast<float>(i + 1);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        const bool active = i < ACTIVE_COUNT;
        golden[LOWEST_ZERO_OFFSET + i] = active ? src.front() : 0.0f;
        golden[HIGHEST_ZERO_OFFSET + i] = active ? src.back() : 0.0f;
        golden[HIGHEST_MERGE_OFFSET + i] = active ? src.back() : INITIAL_VALUE;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    float *src_device = nullptr, *dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), src.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(float), src.data(), src.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(dst_device, output.size() * sizeof(float), output.data(), output.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscDuplicateKernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    std::cout << "Input lowest/highest: " << src.front() << ' ' << src.back() << std::endl;
    PrintData("Lowest + zero", output, LOWEST_ZERO_OFFSET);
    PrintData("Highest + zero", output, HIGHEST_ZERO_OFFSET);
    PrintData("Highest + merge", output, HIGHEST_MERGE_OFFSET);
    PrintData("Golden lowest + zero", golden, LOWEST_ZERO_OFFSET);
    PrintData("Golden highest + zero", golden, HIGHEST_ZERO_OFFSET);
    PrintData("Golden highest + merge", golden, HIGHEST_MERGE_OFFSET);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_duplicate highest modes passed."
                        : "[Failed] asc_duplicate highest modes failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    (OUTPUT_DIR / "asc_duplicate.asc").write_text(highest_source(), encoding="utf-8")
    (OUTPUT_DIR / "asc_duplicate_scalar.asc").write_text(scalar_source(), encoding="utf-8")
    print(f"Generated 2 dav-3510 reg data-fill examples in {OUTPUT_DIR}")


if __name__ == "__main__":
    main()
