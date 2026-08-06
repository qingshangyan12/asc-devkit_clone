#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "data_fill"
EXAMPLE_PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(file_name: str, api: str, scalar: bool) -> str:
    rel = f"reg/data_fill/{file_name}"
    kernel = "".join(part.capitalize() for part in file_name.split("_")) + "Kernel"
    call = f"{api}(dst_reg, FILL_VALUE, mask);" if scalar else f"{api}(dst_reg, src_reg, mask);"
    return f'''// Build/run ({EXAMPLE_PREFIX}/{rel}.asc): bash {EXAMPLE_PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr float FILL_VALUE = 3.5f;

__simd_vf__ inline void Fill(__ubuf__ float* dst, __ubuf__ float* src)
{{
    vector_float dst_reg;
    vector_float src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(dst_reg, dst);
    asc_loadalign(src_reg, src);
    {call}
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}(__gm__ float* dst, __gm__ float* src)
{{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(dst_local, dst, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Fill(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT, -1.0f);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        src[i] = static_cast<float>(i) * 0.25f + 1.0f;
        golden[i] = {'FILL_VALUE' if scalar else 'src[0]'};
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<float> src_device(ELEMENT_COUNT);
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if (!src_device.Allocate("aclrtMalloc(src)") || !dst_device.Allocate("aclrtMalloc(dst)") ||
        !src_device.CopyFrom(src) || !dst_device.CopyFrom(output)) return 1;
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
    cases = {
        "asc_duplicate": ("asc_duplicate", False),
        "asc_duplicate_merge": ("asc_duplicate_merge", False),
        "asc_duplicate_scalar": ("asc_duplicate_scalar", True),
        "asc_duplicate_scalar_merge": ("asc_duplicate_scalar_merge", True),
    }
    for file_name, (api, scalar) in cases.items():
        (OUTPUT_DIR / f"{file_name}.asc").write_text(source(file_name, api, scalar))


if __name__ == "__main__":
    main()
