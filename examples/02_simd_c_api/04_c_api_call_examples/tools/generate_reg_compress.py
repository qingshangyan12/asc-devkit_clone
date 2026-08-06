#!/usr/bin/env python3

from pathlib import Path
from typing import Optional


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "data_compress"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def pack_source(file_name: str, low_api: Optional[str], high_api: Optional[str]) -> str:
    rel = f"reg/data_compress/{file_name}"
    kernel = "".join(x.capitalize() for x in file_name.split("_")) + "Kernel"
    low_call = f"{low_api}(low_reg, src_reg);" if low_api else ""
    high_call = f"{high_api}(high_reg, src_reg);" if high_api else ""
    low_store = "asc_storealign(low, low_reg, dst_mask);" if low_api else ""
    high_store = "asc_storealign(high, high_reg, dst_mask);" if high_api else ""
    low_golden = "golden_low[i] = static_cast<uint8_t>(src[i]);" if low_api else ""
    high_golden = "golden_high[SRC_COUNT + i] = static_cast<uint8_t>(src[i]);" if high_api else ""
    checks = []
    if low_api:
        checks.append("CApiExample::CompareRange(low, golden_low, 0, SRC_COUNT)")
    if high_api:
        checks.append("CApiExample::CompareRange(high, golden_high, SRC_COUNT, DST_COUNT)")
    check = " && ".join(checks)
    names = "/".join(x for x in [low_api, high_api] if x)
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t SRC_COUNT = 128;
constexpr uint32_t DST_COUNT = 256;

__simd_vf__ inline void Pack(__ubuf__ uint8_t* low, __ubuf__ uint8_t* high, __ubuf__ uint16_t* src)
{{
    vector_uint8_t low_reg;
    vector_uint8_t high_reg;
    vector_uint16_t src_reg;
    uint32_t count = DST_COUNT;
    vector_bool dst_mask = asc_update_mask_b8(count);
    asc_loadalign(src_reg, src);
    asc_duplicate_scalar(low_reg, static_cast<uint8_t>(0), dst_mask);
    asc_duplicate_scalar(high_reg, static_cast<uint8_t>(0), dst_mask);
    {low_call}
    {high_call}
    {low_store}
    {high_store}
}}

__global__ __vector__ void {kernel}(__gm__ uint8_t* low, __gm__ uint8_t* high, __gm__ uint16_t* src)
{{
    asc_init();
    __ubuf__ uint8_t low_local[DST_COUNT];
    __ubuf__ uint8_t high_local[DST_COUNT];
    __ubuf__ uint16_t src_local[SRC_COUNT];
    asc_copy_gm2ub_align(src_local, src, SRC_COUNT * sizeof(uint16_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Pack(low_local, high_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(low, low_local, DST_COUNT);
    asc_copy_ub2gm_align(high, high_local, DST_COUNT);
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint16_t> src(SRC_COUNT);
    std::vector<uint8_t> low(DST_COUNT, 0);
    std::vector<uint8_t> high(DST_COUNT, 0);
    std::vector<uint8_t> golden_low(DST_COUNT, 0);
    std::vector<uint8_t> golden_high(DST_COUNT, 0);
    for (uint32_t i = 0; i < SRC_COUNT; ++i) {{
        src[i] = static_cast<uint16_t>(0x1200U + i);
        {low_golden}
        {high_golden}
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint16_t> src_device(SRC_COUNT);
    CApiExample::DeviceBuffer<uint8_t> low_device(DST_COUNT);
    CApiExample::DeviceBuffer<uint8_t> high_device(DST_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !low_device.Allocate("aclrtMalloc(low)") ||
        !high_device.Allocate("aclrtMalloc(high)") || !src_device.CopyFrom(src)) return 1;
    {kernel}<<<1, 0>>>(low_device.Get(), high_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !low_device.CopyTo(low) || !high_device.CopyTo(high)) return 1;
    CApiExample::Print("Input", src);
    CApiExample::Print("Low output", low);
    CApiExample::Print("Low Golden", golden_low);
    CApiExample::Print("High output", high);
    CApiExample::Print("High Golden", golden_high);
    const bool passed = {check};
    std::cout << (passed ? "[Success] {names} passed." : "[Failed] {names} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def unpack_source() -> str:
    rel = "reg/data_compress/asc_unpack"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t SRC_COUNT = 256;
constexpr uint32_t DST_COUNT = 128;
__simd_vf__ inline void Unpack(__ubuf__ uint16_t* lower, __ubuf__ uint16_t* upper, __ubuf__ uint8_t* src)
{{
    vector_uint16_t lower_reg;
    vector_uint16_t upper_reg;
    vector_uint8_t src_reg;
    uint32_t count = DST_COUNT;
    vector_bool mask = asc_update_mask_b16(count);
    asc_loadalign(src_reg, src);
    asc_unpack_lower(lower_reg, src_reg);
    asc_unpack_upper(upper_reg, src_reg);
    asc_storealign(lower, lower_reg, mask);
    asc_storealign(upper, upper_reg, mask);
}}
__global__ __vector__ void AscUnpackKernel(__gm__ uint16_t* lower, __gm__ uint16_t* upper, __gm__ uint8_t* src)
{{
    asc_init();
    __ubuf__ uint16_t lower_local[DST_COUNT];
    __ubuf__ uint16_t upper_local[DST_COUNT];
    __ubuf__ uint8_t src_local[SRC_COUNT];
    asc_copy_gm2ub_align(src_local, src, SRC_COUNT);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Unpack(lower_local, upper_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(lower, lower_local, DST_COUNT * sizeof(uint16_t));
    asc_copy_ub2gm_align(upper, upper_local, DST_COUNT * sizeof(uint16_t));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<uint8_t> src(SRC_COUNT);
    std::vector<uint16_t> lower(DST_COUNT, 0), upper(DST_COUNT, 0);
    std::vector<uint16_t> golden_lower(DST_COUNT), golden_upper(DST_COUNT);
    for (uint32_t i = 0; i < SRC_COUNT; ++i) src[i] = static_cast<uint8_t>(i);
    for (uint32_t i = 0; i < DST_COUNT; ++i) {{ golden_lower[i] = src[i]; golden_upper[i] = src[DST_COUNT + i]; }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> src_device(SRC_COUNT);
    CApiExample::DeviceBuffer<uint16_t> lower_device(DST_COUNT), upper_device(DST_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !lower_device.Allocate("aclrtMalloc(lower)") ||
        !upper_device.Allocate("aclrtMalloc(upper)") || !src_device.CopyFrom(src)) return 1;
    AscUnpackKernel<<<1, 0>>>(lower_device.Get(), upper_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !lower_device.CopyTo(lower) || !upper_device.CopyTo(upper)) return 1;
    CApiExample::Print("Input", src);
    CApiExample::Print("Lower output", lower);
    CApiExample::Print("Lower Golden", golden_lower);
    CApiExample::Print("Upper output", upper);
    CApiExample::Print("Upper Golden", golden_upper);
    const bool passed = CApiExample::Compare(lower, golden_lower) && CApiExample::Compare(upper, golden_upper);
    std::cout << (passed ? "[Success] asc_unpack_lower/asc_unpack_upper passed."
                         : "[Failed] asc_unpack_lower/asc_unpack_upper failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def unsqueeze_source() -> str:
    rel = "reg/data_compress/asc_unsqueeze"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void Unsqueeze(__ubuf__ uint32_t* dst)
{{
    vector_uint32_t dst_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_unsqueeze(dst_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}}
__global__ __vector__ void AscUnsqueezeKernel(__gm__ uint32_t* dst)
{{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT];
    Unsqueeze(dst_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<uint32_t> input = {{ELEMENT_COUNT}};
    std::vector<uint32_t> output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) golden[i] = i;
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint32_t> dst_device(ELEMENT_COUNT);
    if (!dst_device.Allocate("aclrtMalloc(dst)")) return 1;
    AscUnsqueezeKernel<<<1, 0>>>(dst_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Input mask count", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] asc_unsqueeze passed." : "[Failed] asc_unsqueeze failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "asc_pack_deprecated.asc").write_text(pack_source("asc_pack_deprecated", "asc_pack", "asc_pack_v2"))
    (OUT / "asc_pack_to_low.asc").write_text(pack_source("asc_pack_to_low", "asc_pack_to_low", None))
    (OUT / "asc_pack_to_high.asc").write_text(pack_source("asc_pack_to_high", None, "asc_pack_to_high"))
    (OUT / "asc_unpack.asc").write_text(unpack_source())
    (OUT / "asc_unsqueeze.asc").write_text(unsqueeze_source())


if __name__ == "__main__":
    main()
