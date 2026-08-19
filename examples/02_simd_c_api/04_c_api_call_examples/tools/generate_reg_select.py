#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "compare_and_select"
EXAMPLE_PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(file_name: str) -> str:
    rel = f"reg/compare_and_select/{file_name}"
    return (
        f"// Build/run ({EXAMPLE_PREFIX}/{rel}.asc): bash {EXAMPLE_PREFIX}/run_example.sh {rel}\n"
        "// Architecture: dav-3510\n\n"
    )


def squeeze_source(file_name: str, api: str) -> str:
    kernel = "".join(part.capitalize() for part in file_name.split("_")) + "Kernel"
    return header(file_name) + f'''#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Squeeze(__ubuf__ float* dst, __ubuf__ float* src)
{{
    vector_float dst_reg;
    vector_float src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    {api}(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ float* dst, __gm__ float* src)
{{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Squeeze(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT, 0.0f);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = golden[i] = static_cast<float>(i) * 0.25f;
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> src_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !dst_device.Allocate("aclrtMalloc(dst)") || !src_device.CopyFrom(src)) return 1;
    {kernel}<<<1, 0>>>(dst_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", src);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def select_source() -> str:
    return header("asc_select") + '''#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1)
{
    vector_float dst_reg;
    vector_float src0_reg;
    vector_float src1_reg;
    vector_bool predicate;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    asc_gt(predicate, src0_reg, src1_reg, mask);
    asc_select(dst_reg, src0_reg, src1_reg, predicate);
    asc_storealign(dst, dst_reg, mask);
}

__global__ __vector__ void AscSelectKernel(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT];
    __ubuf__ float src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Select(dst_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> src0(ELEMENT_COUNT);
    std::vector<float> src1(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT, 0.0f);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src0[i] = static_cast<float>(i);
        src1[i] = static_cast<float>(ELEMENT_COUNT - i);
        golden[i] = src0[i] > src1[i] ? src0[i] : src1[i];
    }
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> src0_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> src1_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if (!src0_device.Allocate("aclrtMalloc(src0)") || !src1_device.Allocate("aclrtMalloc(src1)") ||
        !dst_device.Allocate("aclrtMalloc(dst)") || !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)) return 1;
    AscSelectKernel<<<1, 0>>>(dst_device.Get(), src0_device.Get(), src1_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Input src0", src0);
    CApiExample::Print("Input src1", src1);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] asc_select passed." : "[Failed] asc_select failed.") << std::endl;
    return passed ? 0 : 1;
}
'''


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    (OUTPUT_DIR / "asc_select.asc").write_text(select_source())
    for file_name, api in {
        "asc_squeeze": "asc_squeeze",
        "asc_squeeze_deprecated": "asc_squeeze_v2",
        "asc_squeeze_with_status": "asc_squeeze_with_status",
    }.items():
        (OUTPUT_DIR / f"{file_name}.asc").write_text(squeeze_source(file_name, api))


if __name__ == "__main__":
    main()
