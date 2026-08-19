#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "reduce_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def uint_source(api: str, golden_code: str) -> str:
    rel = f"reg/reduce_compute/{api}"
    kernel = "".join(x.capitalize() for x in api.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void Reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src)
{{
    vector_uint32_t dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    {api}(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}
__global__ __vector__ void {kernel}(__gm__ uint32_t* dst, __gm__ uint32_t* src)
{{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT], src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Reduce(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<uint32_t> src(ELEMENT_COUNT), output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = i + 1;
    {golden_code}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint32_t> src_device(ELEMENT_COUNT), dst_device(ELEMENT_COUNT);
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


def pair_source() -> str:
    api = "asc_pair_reduce_sum"
    rel = f"reg/reduce_compute/{api}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void Reduce(__ubuf__ float* dst, __ubuf__ float* src)
{{
    vector_float dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    asc_pair_reduce_sum(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}
__global__ __vector__ void AscPairReduceSumKernel(__gm__ float* dst, __gm__ float* src)
{{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT], src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Reduce(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<float> src(ELEMENT_COUNT), output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = static_cast<float>(i);
    for (uint32_t i = 0; i < ELEMENT_COUNT / 2; ++i) golden[i] = src[2 * i] + src[2 * i + 1];
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> src_device(ELEMENT_COUNT), dst_device(ELEMENT_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !dst_device.Allocate("aclrtMalloc(dst)") || !src_device.CopyFrom(src)) return 1;
    AscPairReduceSumKernel<<<1, 0>>>(dst_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", src);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] asc_pair_reduce_sum passed." : "[Failed] asc_pair_reduce_sum failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    codes = {
        "asc_reduce_max": "golden[0] = src.back(); golden[1] = ELEMENT_COUNT - 1;",
        "asc_reduce_min": "golden[0] = src.front(); golden[1] = 0;",
        "asc_reduce_sum": "golden[0] = ELEMENT_COUNT * (ELEMENT_COUNT + 1) / 2;",
        "asc_reduce_max_datablock": "for (uint32_t block = 0; block < 8; ++block) golden[block] = src[block * 8 + 7];",
        "asc_reduce_min_datablock": "for (uint32_t block = 0; block < 8; ++block) golden[block] = src[block * 8];",
        "asc_reduce_sum_datablock": "for (uint32_t block = 0; block < 8; ++block) for (uint32_t i = 0; i < 8; ++i) golden[block] += src[block * 8 + i];",
    }
    for api, code in codes.items():
        (OUT / f"{api}.asc").write_text(uint_source(api, code))
    (OUT / "asc_pair_reduce_sum.asc").write_text(pair_source())


if __name__ == "__main__":
    main()
