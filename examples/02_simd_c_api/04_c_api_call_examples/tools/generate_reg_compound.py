#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "compound_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(api: str, call: str, golden: str, tolerance: str = "1e-6") -> str:
    rel = f"reg/compound_compute/{api}"
    kernel = "".join(part.capitalize() for part in api.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr float SCALAR_VALUE = 0.5f;

__simd_vf__ inline void Compute(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1)
{{
    vector_float dst_reg;
    vector_float src0_reg;
    vector_float src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(dst_reg, dst);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    {call}
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT];
    __ubuf__ float src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(dst_local, dst, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compute(dst_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<float> initial(ELEMENT_COUNT);
    std::vector<float> src0(ELEMENT_COUNT);
    std::vector<float> src1(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        initial[i] = 1.0f + static_cast<float>(i) * 0.125f;
        src0[i] = static_cast<float>(i % 8) * 0.25f;
        src1[i] = static_cast<float>((i + 3) % 8) * 0.25f;
        output[i] = initial[i];
        golden[i] = {golden};
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> src0_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> src1_device(ELEMENT_COUNT);
    if (!dst_device.Allocate("aclrtMalloc(dst)") || !src0_device.Allocate("aclrtMalloc(src0)") ||
        !src1_device.Allocate("aclrtMalloc(src1)") || !dst_device.CopyFrom(initial) ||
        !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)) return 1;
    {kernel}<<<1, 0>>>(dst_device.Get(), src0_device.Get(), src1_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Initial dst", initial);
    CApiExample::Print("Input src0", src0);
    CApiExample::Print("Input src1", src1);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden, {tolerance});
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def exp_sub_source() -> str:
    api = "asc_exp_sub_deprecated"
    rel = f"reg/compound_compute/{api}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Compute(__ubuf__ float* even, __ubuf__ float* odd,
    __ubuf__ float* src0, __ubuf__ float* src1)
{{
    vector_float even_reg;
    vector_float odd_reg;
    vector_float src0_reg;
    vector_float src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    asc_exp_sub(even_reg, src0_reg, src1_reg, mask);
    asc_exp_sub_v2(odd_reg, src0_reg, src1_reg, mask);
    asc_storealign(even, even_reg, mask);
    asc_storealign(odd, odd_reg, mask);
}}

__global__ __vector__ void AscExpSubDeprecatedKernel(__gm__ float* even, __gm__ float* odd,
    __gm__ float* src0, __gm__ float* src1)
{{
    asc_init();
    __ubuf__ float even_local[ELEMENT_COUNT];
    __ubuf__ float odd_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT];
    __ubuf__ float src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compute(even_local, odd_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(even, even_local, ELEMENT_COUNT * sizeof(float));
    asc_copy_ub2gm_align(odd, odd_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<float> src0(ELEMENT_COUNT, 0.5f);
    std::vector<float> src1(ELEMENT_COUNT, 0.25f);
    std::vector<float> even(ELEMENT_COUNT, 0.0f);
    std::vector<float> odd(ELEMENT_COUNT, 0.0f);
    std::vector<float> golden(ELEMENT_COUNT, std::exp(0.25f));
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> src0_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> src1_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> even_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> odd_device(ELEMENT_COUNT);
    if (!src0_device.Allocate("aclrtMalloc(src0)") || !src1_device.Allocate("aclrtMalloc(src1)") ||
        !even_device.Allocate("aclrtMalloc(even)") || !odd_device.Allocate("aclrtMalloc(odd)") ||
        !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)) return 1;
    AscExpSubDeprecatedKernel<<<1, 0>>>(
        even_device.Get(), odd_device.Get(), src0_device.Get(), src1_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !even_device.CopyTo(even) || !odd_device.CopyTo(odd)) return 1;
    CApiExample::Print("asc_exp_sub output", even);
    CApiExample::Print("asc_exp_sub_v2 output", odd);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(even, golden, 1e-3) && CApiExample::Compare(odd, golden, 1e-3);
    std::cout << (passed ? "[Success] asc_exp_sub/asc_exp_sub_v2 passed."
                         : "[Failed] asc_exp_sub/asc_exp_sub_v2 failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_abs_sub": ("asc_abs_sub(dst_reg, src0_reg, src1_reg, mask);", "std::fabs(src0[i] - src1[i])"),
        "asc_axpy": ("asc_axpy(dst_reg, src0_reg, SCALAR_VALUE, mask);", "src0[i] * SCALAR_VALUE + initial[i]"),
        "asc_madd": ("asc_madd(dst_reg, src0_reg, src1_reg, mask);", "initial[i] * src0[i] + src1[i]"),
    }
    for api, (call, golden) in cases.items():
        (OUTPUT_DIR / f"{api}.asc").write_text(source(api, call, golden))
    (OUTPUT_DIR / "asc_exp_sub_deprecated.asc").write_text(exp_sub_source())


if __name__ == "__main__":
    main()
