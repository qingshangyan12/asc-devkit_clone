#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "logic_compute"
EXAMPLE_PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"

BINARY_CASES = {
    "asc_and": "src0[i] & src1[i]",
    "asc_or": "src0[i] | src1[i]",
    "asc_shiftleft": "src0[i] << src1[i]",
    "asc_shiftright": "src0[i] >> src1[i]",
    "asc_xor": "src0[i] ^ src1[i]",
}

SCALAR_CASES = {
    "asc_shiftleft_scalar": "src[i] << SHIFT",
    "asc_shiftright_scalar": "src[i] >> SHIFT",
}


def class_name(api: str) -> str:
    return "".join(part.capitalize() for part in api.split("_"))


def header(api: str) -> str:
    rel = f"reg/logic_compute/{api}"
    return (
        f"// Build/run ({EXAMPLE_PREFIX}/{rel}.asc): "
        f"bash {EXAMPLE_PREFIX}/run_example.sh {rel}\n"
        "// Architecture: dav-3510\n\n"
    )


def binary_source(api: str, golden: str) -> str:
    kernel = class_name(api) + "Kernel"
    shift = "shift" in api
    src1_init = "static_cast<int32_t>(i % 4)" if shift else "static_cast<int32_t>(0x0f0f0000U + i)"
    return header(api) + f'''#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Compute(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1)
{{
    vector_int32_t dst_reg;
    vector_int32_t src0_reg;
    vector_int32_t src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    {api}(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ int32_t* dst, __gm__ int32_t* src0, __gm__ int32_t* src1)
{{
    asc_init();
    __ubuf__ int32_t dst_local[ELEMENT_COUNT];
    __ubuf__ int32_t src0_local[ELEMENT_COUNT];
    __ubuf__ int32_t src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(int32_t));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compute(dst_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<int32_t> src0(ELEMENT_COUNT);
    std::vector<int32_t> src1(ELEMENT_COUNT);
    std::vector<int32_t> output(ELEMENT_COUNT, 0);
    std::vector<int32_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src0[i] = static_cast<int32_t>(0x101010U + i);
        src1[i] = {src1_init};
        golden[i] = {golden};
    }}

    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<int32_t> src0_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<int32_t> src1_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<int32_t> dst_device(ELEMENT_COUNT);
    if (!src0_device.Allocate("aclrtMalloc(src0)") || !src1_device.Allocate("aclrtMalloc(src1)") ||
        !dst_device.Allocate("aclrtMalloc(dst)") || !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)) return 1;

    {kernel}<<<1, 0>>>(dst_device.Get(), src0_device.Get(), src1_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Input src0", src0);
    CApiExample::Print("Input src1", src1);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def unary_source() -> str:
    api = "asc_not"
    kernel = class_name(api) + "Kernel"
    return header(api) + f'''#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void Compute(__ubuf__ int32_t* dst, __ubuf__ int32_t* src)
{{
    vector_int32_t dst_reg;
    vector_int32_t src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    {api}(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ int32_t* dst, __gm__ int32_t* src)
{{
    asc_init();
    __ubuf__ int32_t dst_local[ELEMENT_COUNT];
    __ubuf__ int32_t src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compute(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<int32_t> src(ELEMENT_COUNT);
    std::vector<int32_t> output(ELEMENT_COUNT, 0);
    std::vector<int32_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src[i] = static_cast<int32_t>(0x0f0f0000U + i);
        golden[i] = ~src[i];
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<int32_t> src_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<int32_t> dst_device(ELEMENT_COUNT);
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


def scalar_source(api: str, golden: str) -> str:
    kernel = class_name(api) + "Kernel"
    return header(api) + f'''#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr int16_t SHIFT = 2;

__simd_vf__ inline void Compute(__ubuf__ int32_t* dst, __ubuf__ int32_t* src)
{{
    vector_int32_t dst_reg;
    vector_int32_t src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    {api}(dst_reg, src_reg, SHIFT, mask);
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ int32_t* dst, __gm__ int32_t* src)
{{
    asc_init();
    __ubuf__ int32_t dst_local[ELEMENT_COUNT];
    __ubuf__ int32_t src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compute(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(int32_t));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<int32_t> src(ELEMENT_COUNT);
    std::vector<int32_t> output(ELEMENT_COUNT, 0);
    std::vector<int32_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src[i] = static_cast<int32_t>(i + 1);
        golden[i] = {golden};
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<int32_t> src_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<int32_t> dst_device(ELEMENT_COUNT);
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


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    for api, golden in BINARY_CASES.items():
        (OUTPUT_DIR / f"{api}.asc").write_text(binary_source(api, golden))
    (OUTPUT_DIR / "asc_not.asc").write_text(unary_source())
    for api, golden in SCALAR_CASES.items():
        (OUTPUT_DIR / f"{api}.asc").write_text(scalar_source(api, golden))


if __name__ == "__main__":
    main()
