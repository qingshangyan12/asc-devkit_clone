#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(category: str, name: str, body: str, APIs: str, arch: str = "dav-3510") -> str:
    rel = f"{category}/{name}"
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENTS = 16;

__global__ __vector__ void {kernel}(__gm__ uint64_t* output)
{{
    asc_init();
{body}
    output[0] = 1;
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint64_t> input(ELEMENTS, 0), output(ELEMENTS, ~0ULL), golden(ELEMENTS, 0);
    golden[0] = 1;
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint64_t> output_device(ELEMENTS);
    if (!output_device.Allocate("aclrtMalloc(output)") || !output_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::CompareRange(output, golden, 0, 1);
    std::cout << (passed ? "[Success] {APIs} passed." : "[Failed] {APIs} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    cache = ROOT / "cache_ctrl"
    misc = ROOT / "misc"
    cache.mkdir(parents=True, exist_ok=True)
    misc.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_datacache_preload": source(
            "cache_ctrl", "asc_datacache_preload",
            "    asc_datacache_preload(output, 0);",
            "asc_datacache_preload"),
        "asc_dcci": source(
            "cache_ctrl", "asc_dcci",
            '''    __ubuf__ uint8_t scratch[256];
    asc_dcci_single(output);
    asc_ub_dcci_single(scratch);
    asc_dcci_entire_ub();
    asc_dcci_entire_out();
    asc_dcci_entire_all();
    asc_dcci_entire_atomic();''',
            "asc_dcci_single/asc_ub_dcci_single/asc_dcci_entire_ub/asc_dcci_entire_out/asc_dcci_entire_all/asc_dcci_entire_atomic",
            "dav-2201"),
        "asc_dci": source(
            "cache_ctrl", "asc_dci",
            "    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);\n    asc_dci();",
            "asc_dci"),
        "asc_get_icache_preload_status": source(
            "cache_ctrl", "asc_get_icache_preload_status",
            "    const int64_t status = asc_get_icache_preload_status();\n    output[1] = static_cast<uint64_t>(status);",
            "asc_get_icache_preload_status"),
        "asc_icache_preload": source(
            "cache_ctrl", "asc_icache_preload",
            '''    const int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
    asc_icache_preload(reinterpret_cast<const void*>(pc), 32);''',
            "asc_icache_preload"),
    }
    for name, text in cases.items():
        (cache / f"{name}.asc").write_text(text, encoding="utf-8")
    (misc / "asc_init.asc").write_text(
        source("misc", "asc_init", "    // asc_init is called at kernel entry above.", "asc_init"), encoding="utf-8")
    print(f"Generated {len(cases) + 1} cache and miscellaneous examples")


if __name__ == "__main__":
    main()
