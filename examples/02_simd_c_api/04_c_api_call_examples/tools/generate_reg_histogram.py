#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "histogram_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(file_name: str, stem: str, cumulative: bool) -> str:
    rel = f"reg/histogram_compute/{file_name}"
    kernel = "".join(x.capitalize() for x in file_name.split("_")) + "Kernel"
    golden = "i + 1" if cumulative else "1"
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
__simd_vf__ inline void Histogram(__ubuf__ uint16_t* low, __ubuf__ uint16_t* high, __ubuf__ uint8_t* src)
{{
    vector_uint16_t low_reg, high_reg;
    vector_uint8_t src_reg;
    uint32_t src_count = SRC_COUNT;
    vector_bool src_mask = asc_update_mask_b8(src_count);
    uint32_t dst_count = DST_COUNT;
    vector_bool dst_mask = asc_update_mask_b16(dst_count);
    asc_duplicate_scalar(low_reg, static_cast<uint16_t>(0), dst_mask);
    asc_duplicate_scalar(high_reg, static_cast<uint16_t>(0), dst_mask);
    asc_loadalign(src_reg, src);
    {stem}_bin0(low_reg, src_reg, src_mask);
    {stem}_bin1(high_reg, src_reg, src_mask);
    asc_storealign(low, low_reg, dst_mask);
    asc_storealign(high, high_reg, dst_mask);
}}
__global__ __vector__ void {kernel}(__gm__ uint16_t* low, __gm__ uint16_t* high, __gm__ uint8_t* src)
{{
    asc_init();
    __ubuf__ uint16_t low_local[DST_COUNT], high_local[DST_COUNT];
    __ubuf__ uint8_t src_local[SRC_COUNT];
    asc_copy_gm2ub_align(src_local, src, SRC_COUNT);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Histogram(low_local, high_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(low, low_local, DST_COUNT * sizeof(uint16_t));
    asc_copy_ub2gm_align(high, high_local, DST_COUNT * sizeof(uint16_t));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<uint8_t> src(SRC_COUNT);
    std::vector<uint16_t> low(DST_COUNT, 0), high(DST_COUNT, 0);
    std::vector<uint16_t> golden_low(DST_COUNT), golden_high(DST_COUNT);
    for (uint32_t i = 0; i < SRC_COUNT; ++i) src[i] = static_cast<uint8_t>(i);
    for (uint32_t i = 0; i < DST_COUNT; ++i) {{
        golden_low[i] = {golden};
        golden_high[i] = {'DST_COUNT + i + 1' if cumulative else golden};
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> src_device(SRC_COUNT);
    CApiExample::DeviceBuffer<uint16_t> low_device(DST_COUNT), high_device(DST_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !low_device.Allocate("aclrtMalloc(low)") ||
        !high_device.Allocate("aclrtMalloc(high)") || !src_device.CopyFrom(src)) return 1;
    {kernel}<<<1, 0>>>(low_device.Get(), high_device.Get(), src_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !low_device.CopyTo(low) || !high_device.CopyTo(high)) return 1;
    CApiExample::Print("Input", src);
    CApiExample::Print("BIN0 output", low);
    CApiExample::Print("BIN1 output", high);
    CApiExample::Print("BIN0 golden", golden_low);
    CApiExample::Print("BIN1 golden", golden_high);
    const bool passed = CApiExample::Compare(low, golden_low) && CApiExample::Compare(high, golden_high);
    std::cout << (passed ? "[Success] {stem}_bin0/bin1 passed." : "[Failed] {stem}_bin0/bin1 failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "asc_frequency_histogram.asc").write_text(
        source("asc_frequency_histogram", "asc_frequency_histogram", False))
    (OUT / "asc_cumulative_histogram.asc").write_text(
        source("asc_cumulative_histogram", "asc_cumulative_histogram", True))


if __name__ == "__main__":
    main()
