#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "data_rearrange"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(api: str) -> str:
    rel = f"reg/data_rearrange/{api}"
    kernel = "".join(x.capitalize() for x in api.split("_")) + "Kernel"
    golden = (
        "for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {\n"
        "        const uint32_t combined_index = 2 * i;\n"
        "        golden0[i] = combined_index < ELEMENT_COUNT ? src0[combined_index] : src1[combined_index - ELEMENT_COUNT];\n"
        "        golden1[i] = combined_index + 1 < ELEMENT_COUNT ? src0[combined_index + 1] : src1[combined_index + 1 - ELEMENT_COUNT];\n"
        "    }"
        if api == "asc_deintlv" else
        "for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {\n"
        "        const uint32_t combined_index = i;\n"
        "        const uint32_t source_index = combined_index / 2;\n"
        "        golden0[i] = (combined_index % 2 == 0) ? src0[source_index] : src1[source_index];\n"
        "        const uint32_t second_index = ELEMENT_COUNT + i;\n"
        "        const uint32_t second_source = second_index / 2;\n"
        "        golden1[i] = (second_index % 2 == 0) ? src0[second_source] : src1[second_source];\n"
        "    }"
    )
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void Rearrange(__ubuf__ float* dst0, __ubuf__ float* dst1,
    __ubuf__ float* src0, __ubuf__ float* src1)
{{
    vector_float dst0_reg, dst1_reg, src0_reg, src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    {api}(dst0_reg, dst1_reg, src0_reg, src1_reg);
    asc_storealign(dst0, dst0_reg, mask);
    asc_storealign(dst1, dst1_reg, mask);
}}
__global__ __vector__ void {kernel}(__gm__ float* dst0, __gm__ float* dst1,
    __gm__ float* src0, __gm__ float* src1)
{{
    asc_init();
    __ubuf__ float dst0_local[ELEMENT_COUNT], dst1_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT], src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Rearrange(dst0_local, dst1_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst0, dst0_local, ELEMENT_COUNT * sizeof(float));
    asc_copy_ub2gm_align(dst1, dst1_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<float> src0(ELEMENT_COUNT), src1(ELEMENT_COUNT);
    std::vector<float> output0(ELEMENT_COUNT, 0), output1(ELEMENT_COUNT, 0);
    std::vector<float> golden0(ELEMENT_COUNT), golden1(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{ src0[i] = static_cast<float>(i); src1[i] = 1000.0f + i; }}
    {golden}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> src0_device(ELEMENT_COUNT), src1_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst0_device(ELEMENT_COUNT), dst1_device(ELEMENT_COUNT);
    if (!src0_device.Allocate("aclrtMalloc(src0)") || !src1_device.Allocate("aclrtMalloc(src1)") ||
        !dst0_device.Allocate("aclrtMalloc(dst0)") || !dst1_device.Allocate("aclrtMalloc(dst1)") ||
        !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)) return 1;
    {kernel}<<<1, 0>>>(dst0_device.Get(), dst1_device.Get(), src0_device.Get(), src1_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !dst0_device.CopyTo(output0) || !dst1_device.CopyTo(output1)) return 1;
    CApiExample::Print("Output 0", output0);
    CApiExample::Print("Golden 0", golden0);
    CApiExample::Print("Output 1", output1);
    CApiExample::Print("Golden 1", golden1);
    const bool passed = CApiExample::Compare(output0, golden0) && CApiExample::Compare(output1, golden1);
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    for api in ("asc_intlv", "asc_deintlv"):
        (OUT / f"{api}.asc").write_text(source(api))


if __name__ == "__main__":
    main()
