#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "arithmetic_compute"
EXAMPLE_PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


UNARY_CASES = {
    "asc_abs": ("std::fabs(src[i])", "(static_cast<int>(i) - 32) * 0.25f", "1e-6"),
    "asc_exp": ("std::exp(src[i])", "(static_cast<int>(i % 5) - 2) * 0.25f", "1e-3"),
    "asc_ln": ("std::log(src[i])", "0.5f + static_cast<float>(i % 8) * 0.25f", "1e-3"),
    "asc_neg": ("-src[i]", "(static_cast<int>(i) - 32) * 0.25f", "1e-6"),
    "asc_relu": ("std::max(src[i], 0.0f)", "(static_cast<int>(i) - 32) * 0.25f", "1e-6"),
    "asc_sqrt": ("std::sqrt(src[i])", "static_cast<float>((i % 8) * (i % 8))", "1e-3"),
}

BINARY_CASES = {
    "asc_div": ("src0[i] / src1[i]", "static_cast<float>(i + 1)", "static_cast<float>(i % 8 + 1)", "1e-3"),
    "asc_max": ("std::max(src0[i], src1[i])", "static_cast<float>(i) - 24.0f", "16.0f - static_cast<float>(i) * 0.5f", "1e-6"),
    "asc_min": ("std::min(src0[i], src1[i])", "static_cast<float>(i) - 24.0f", "16.0f - static_cast<float>(i) * 0.5f", "1e-6"),
    "asc_mul": ("src0[i] * src1[i]", "static_cast<float>(i) * 0.25f", "static_cast<float>(i % 8) * 0.5f", "1e-6"),
    "asc_prelu": ("src0[i] >= 0.0f ? src0[i] : src0[i] * src1[i]", "(static_cast<int>(i) - 32) * 0.25f", "0.125f", "1e-6"),
    "asc_sub": ("src0[i] - src1[i]", "static_cast<float>(i) * 0.25f", "static_cast<float>(64 - i) * 0.5f", "1e-6"),
}

SCALAR_CASES = {
    "asc_add_scalar": ("src[i] + SCALAR_VALUE", "(static_cast<int>(i) - 16) * 0.25f", "2.0f", "1e-6"),
    "asc_leakyrelu": ("src[i] >= 0.0f ? src[i] : src[i] * SCALAR_VALUE", "(static_cast<int>(i) - 32) * 0.25f", "0.125f", "1e-6"),
    "asc_max_scalar": ("std::max(src[i], SCALAR_VALUE)", "static_cast<float>(i) - 24.0f", "3.0f", "1e-6"),
    "asc_min_scalar": ("std::min(src[i], SCALAR_VALUE)", "static_cast<float>(i) - 24.0f", "3.0f", "1e-6"),
    "asc_mul_scalar": ("src[i] * SCALAR_VALUE", "(static_cast<int>(i) - 16) * 0.25f", "2.0f", "1e-6"),
}


def class_name(api: str) -> str:
    return "".join(part.capitalize() for part in api.split("_"))


def header(api: str) -> str:
    rel = f"reg/arithmetic_compute/{api}"
    return (
        f"// Build/run ({EXAMPLE_PREFIX}/{rel}.asc): "
        f"bash {EXAMPLE_PREFIX}/run_example.sh {rel}\n"
        "// Architecture: dav-3510\n\n"
    )


def unary_source(api: str, golden: str, init: str, tolerance: str) -> str:
    kernel = class_name(api) + "Kernel"
    return header(api) + f'''#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{

constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Compute(__ubuf__ float* dst, __ubuf__ float* src)
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
    Compute(dst_local, src_local);
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
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src[i] = {init};
        golden[i] = {golden};
    }}

    CApiExample::Runtime runtime;
    if (!runtime.Init()) {{
        return 1;
    }}
    CApiExample::DeviceBuffer<float> src_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !dst_device.Allocate("aclrtMalloc(dst)") ||
        !src_device.CopyFrom(src)) {{
        return 1;
    }}

    {kernel}<<<1, 0>>>(dst_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) {{
        return 1;
    }}

    CApiExample::Print("Input", src);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden, {tolerance});
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def binary_source(api: str, golden: str, init0: str, init1: str, tolerance: str) -> str:
    kernel = class_name(api) + "Kernel"
    return header(api) + f'''#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{

constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Compute(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1)
{{
    vector_float dst_reg;
    vector_float src0_reg;
    vector_float src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    {api}(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT];
    __ubuf__ float src1_local[ELEMENT_COUNT];
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
    std::vector<float> src0(ELEMENT_COUNT);
    std::vector<float> src1(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT, 0.0f);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src0[i] = {init0};
        src1[i] = {init1};
        golden[i] = {golden};
    }}

    CApiExample::Runtime runtime;
    if (!runtime.Init()) {{
        return 1;
    }}
    CApiExample::DeviceBuffer<float> src0_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> src1_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if (!src0_device.Allocate("aclrtMalloc(src0)") || !src1_device.Allocate("aclrtMalloc(src1)") ||
        !dst_device.Allocate("aclrtMalloc(dst)") || !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)) {{
        return 1;
    }}

    {kernel}<<<1, 0>>>(dst_device.Get(), src0_device.Get(), src1_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) {{
        return 1;
    }}

    CApiExample::Print("Input src0", src0);
    CApiExample::Print("Input src1", src1);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden, {tolerance});
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def scalar_source(api: str, golden: str, init: str, value: str, tolerance: str) -> str:
    kernel = class_name(api) + "Kernel"
    return header(api) + f'''#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{

constexpr uint32_t ELEMENT_COUNT = 64;
constexpr float SCALAR_VALUE = {value};

__simd_vf__ inline void Compute(__ubuf__ float* dst, __ubuf__ float* src)
{{
    vector_float dst_reg;
    vector_float src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    {api}(dst_reg, src_reg, SCALAR_VALUE, mask);
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
    Compute(dst_local, src_local);
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
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src[i] = {init};
        golden[i] = {golden};
    }}

    CApiExample::Runtime runtime;
    if (!runtime.Init()) {{
        return 1;
    }}
    CApiExample::DeviceBuffer<float> src_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !dst_device.Allocate("aclrtMalloc(dst)") ||
        !src_device.CopyFrom(src)) {{
        return 1;
    }}

    {kernel}<<<1, 0>>>(dst_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) {{
        return 1;
    }}

    CApiExample::Print("Input", src);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden, {tolerance});
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    for api, args in UNARY_CASES.items():
        (OUTPUT_DIR / f"{api}.asc").write_text(unary_source(api, *args))
    for api, args in BINARY_CASES.items():
        if api != "asc_add":
            (OUTPUT_DIR / f"{api}.asc").write_text(binary_source(api, *args))
    for api, args in SCALAR_CASES.items():
        (OUTPUT_DIR / f"{api}.asc").write_text(scalar_source(api, *args))


if __name__ == "__main__":
    main()
