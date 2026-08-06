#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(relative_dir: str, file_name: str, call: str, api_label: str) -> str:
    rel = f"reg/{relative_dir}/{file_name}"
    kernel = "".join(x.capitalize() for x in file_name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t MARKER = 0x3510U;
__simd_vf__ inline void Invoke(__ubuf__ uint32_t* dst)
{{
    vector_uint32_t marker;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_duplicate_scalar(marker, MARKER, mask);
    {call}
    asc_storealign(dst, marker, mask);
}}
__global__ __vector__ void {kernel}(__gm__ uint32_t* dst)
{{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT];
    Invoke(dst_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<uint32_t> output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT, MARKER);
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint32_t> dst_device(ELEMENT_COUNT);
    if (!dst_device.Allocate("aclrtMalloc(dst)")) return 1;
    {kernel}<<<1, 0>>>(dst_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    CApiExample::Print("Output marker", output);
    CApiExample::Print("Golden marker", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {api_label} invocation passed." : "[Failed] {api_label} invocation failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    cases = (
        ("sync_control", "asc_mem_bar", "asc_mem_bar(VV_ALL);", "asc_mem_bar"),
        ("system_variables_access", "asc_clear_ar_spr", "asc_clear_ar_spr();", "asc_clear_ar_spr"),
    )
    for directory, file_name, call, label in cases:
        output = ROOT / "reg" / directory
        output.mkdir(parents=True, exist_ok=True)
        (output / f"{file_name}.asc").write_text(source(directory, file_name, call, label))


if __name__ == "__main__":
    main()
