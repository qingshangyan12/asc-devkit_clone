#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "scalar_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(name: str, body: str, initial, golden, APIs: str) -> str:
    rel = f"scalar_compute/{name}"
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    input_values = ", ".join(str(x) for x in initial)
    golden_values = ", ".join(str(x) for x in golden)
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENTS = 8;

__global__ __vector__ void {kernel}(__gm__ int64_t* output)
{{
    asc_init();
{body}
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<int64_t> input = {{{input_values}}};
    std::vector<int64_t> golden = {{{golden_values}}};
    input.resize(ELEMENTS, 0);
    golden.resize(ELEMENTS, 0);
    std::vector<int64_t> output(ELEMENTS, -1);
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<int64_t> output_device(ELEMENTS);
    if (!output_device.Allocate("aclrtMalloc(output)") || !output_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {APIs} passed." : "[Failed] {APIs} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def atomic(name: str, call: str, initial: int, updated: int) -> str:
    body = f'''    __gm__ uint32_t* address = reinterpret_cast<__gm__ uint32_t*>(output);
    asc_dcci_entire_all();
    const uint32_t old_value = {call};
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dcci_entire_all();
    output[1] = static_cast<int64_t>(old_value);'''
    return source(name, body, [initial, 0], [updated, initial], name)


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_atomic_add": atomic("asc_atomic_add", "asc_atomic_add(address, 5U)", 10, 15),
        "asc_atomic_and": atomic("asc_atomic_and", "asc_atomic_and(address, 5U)", 15, 5),
        "asc_atomic_cas": atomic("asc_atomic_cas", "asc_atomic_cas(address, 10U, 20U)", 10, 20),
        "asc_atomic_dec": atomic("asc_atomic_dec", "asc_atomic_dec(address, 20U)", 10, 9),
        "asc_atomic_exch": atomic("asc_atomic_exch", "asc_atomic_exch(address, 5U)", 10, 5),
        "asc_atomic_inc": atomic("asc_atomic_inc", "asc_atomic_inc(address, 20U)", 10, 11),
        "asc_atomic_max": atomic("asc_atomic_max", "asc_atomic_max(address, 15U)", 10, 15),
        "asc_atomic_min": atomic("asc_atomic_min", "asc_atomic_min(address, 5U)", 10, 5),
        "asc_atomic_or": atomic("asc_atomic_or", "asc_atomic_or(address, 5U)", 10, 15),
        "asc_atomic_sub": atomic("asc_atomic_sub", "asc_atomic_sub(address, 3U)", 10, 7),
        "asc_atomic_xor": atomic("asc_atomic_xor", "asc_atomic_xor(address, 15U)", 10, 5),
        "asc_clear_nthbit": source(
            "asc_clear_nthbit", "    output[0] = static_cast<int64_t>(asc_clear_nthbit(15, 1));",
            [0], [13], "asc_clear_nthbit"),
        "asc_clz": source("asc_clz", "    output[0] = asc_clz(1);", [0], [63], "asc_clz"),
        "asc_ffs": source("asc_ffs", "    output[0] = asc_ffs(8);", [0], [3], "asc_ffs"),
        "asc_ffz": source("asc_ffz", "    output[0] = asc_ffz(3);", [0], [2], "asc_ffz"),
        "asc_float2int32": source(
            "asc_float2int32",
            '''    constexpr float value = 1.25f;
    output[0] = asc_float2int32_rn(value);
    output[1] = asc_float2int32_rna(value);
    output[2] = asc_float2int32_rd(value);
    output[3] = asc_float2int32_ru(value);''',
            [0, 0, 0, 0], [1, 1, 1, 2],
            "asc_float2int32_rn/asc_float2int32_rna/asc_float2int32_rd/asc_float2int32_ru"),
        "asc_popc": source("asc_popc", "    output[0] = asc_popc(15);", [0], [4], "asc_popc"),
        "asc_set_nthbit": source(
            "asc_set_nthbit", "    output[0] = static_cast<int64_t>(asc_set_nthbit(8, 1));",
            [0], [10], "asc_set_nthbit"),
        "asc_sflbits": source("asc_sflbits", "    output[0] = asc_sflbits(0);", [0], [-1], "asc_sflbits"),
        "asc_store_dev": source(
            "asc_store_dev",
            "    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);\n    asc_store_dev(output, static_cast<int64_t>(42));",
            [0], [42], "asc_store_dev"),
        "asc_zero_bits_cnt": source(
            "asc_zero_bits_cnt", "    output[0] = asc_zero_bits_cnt(15);", [0], [60], "asc_zero_bits_cnt"),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} scalar compute examples in {OUT}")


if __name__ == "__main__":
    main()
