#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "simd_atomic"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(name: str, calls: str, APIs: str, arch: str = "dav-3510") -> str:
    rel = f"simd_atomic/{name}"
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENTS = 8;

__global__ __vector__ void {kernel}(__gm__ uint64_t* output)
{{
    asc_init();
{calls}
    asc_store_atomic_config config;
    asc_get_store_atomic_config(config);
    output[1] = config.config;
    asc_set_atomic_none();
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
    CApiExample::Print("Output (marker, raw config)", output);
    CApiExample::Print("Golden marker", golden);
    const bool passed = CApiExample::CompareRange(output, golden, 0, 1);
    std::cout << (passed ? "[Success] {APIs} passed." : "[Failed] {APIs} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def typed_calls(operation: str) -> str:
    return "\n".join(
        f"    asc_set_atomic_{operation}_{suffix}();"
        for suffix in ("int8", "int16", "int32", "float16", "bfloat", "float")
    )


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_get_store_atomic_config": source(
            "asc_get_store_atomic_config", "    asc_set_atomic_add_float();",
            "asc_get_store_atomic_config"),
        "asc_set_atomic_add_int_deprecated": source(
            "asc_set_atomic_add_int_deprecated", "    asc_set_atomic_add_int();",
            "asc_set_atomic_add_int"),
        "asc_set_atomic_add": source(
            "asc_set_atomic_add", typed_calls("add"),
            "asc_set_atomic_add_int8/asc_set_atomic_add_int16/asc_set_atomic_add_int32/asc_set_atomic_add_float16/asc_set_atomic_add_bfloat/asc_set_atomic_add_float"),
        "asc_set_atomic_max_int_deprecated": source(
            "asc_set_atomic_max_int_deprecated", "    asc_set_atomic_max_int();",
            "asc_set_atomic_max_int"),
        "asc_set_atomic_max": source(
            "asc_set_atomic_max", typed_calls("max"),
            "asc_set_atomic_max_int8/asc_set_atomic_max_int16/asc_set_atomic_max_int32/asc_set_atomic_max_float16/asc_set_atomic_max_bfloat/asc_set_atomic_max_float"),
        "asc_set_atomic_min_int_deprecated": source(
            "asc_set_atomic_min_int_deprecated", "    asc_set_atomic_min_int();",
            "asc_set_atomic_min_int"),
        "asc_set_atomic_min": source(
            "asc_set_atomic_min", typed_calls("min"),
            "asc_set_atomic_min_int8/asc_set_atomic_min_int16/asc_set_atomic_min_int32/asc_set_atomic_min_float16/asc_set_atomic_min_bfloat/asc_set_atomic_min_float"),
        "asc_set_atomic_none": source(
            "asc_set_atomic_none", "    asc_set_atomic_add_int32();\n    asc_set_atomic_none();",
            "asc_set_atomic_none"),
        "asc_set_store_atomic_config_v1": source(
            "asc_set_store_atomic_config_v1", "    asc_set_store_atomic_config_v1(1, 0);",
            "asc_set_store_atomic_config_v1", "dav-2201"),
        "asc_set_store_atomic_config_v2_deprecated": source(
            "asc_set_store_atomic_config_v2_deprecated", "    asc_set_store_atomic_config_v2(1, 2);",
            "asc_set_store_atomic_config_v2"),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} SIMD atomic configuration examples in {OUT}")


if __name__ == "__main__":
    main()
