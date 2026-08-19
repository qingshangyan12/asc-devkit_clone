#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_DIR = ROOT / "reg" / "compare_and_select"
EXAMPLE_PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"

OPERATORS = {
    "asc_eq": "==",
    "asc_ge": ">=",
    "asc_gt": ">",
    "asc_le": "<=",
    "asc_lt": "<",
    "asc_ne": "!=",
}


def class_name(api: str) -> str:
    return "".join(part.capitalize() for part in api.split("_"))


def header(api: str) -> str:
    rel = f"reg/compare_and_select/{api}"
    return (
        f"// Build/run ({EXAMPLE_PREFIX}/{rel}.asc): "
        f"bash {EXAMPLE_PREFIX}/run_example.sh {rel}\n"
        "// Architecture: dav-3510\n\n"
    )


def source(api: str, operator: str, scalar: bool) -> str:
    kernel = class_name(api) + "Kernel"
    input_args = "__ubuf__ float* src" if scalar else "__ubuf__ float* src0, __ubuf__ float* src1"
    reg_decls = "vector_float src_reg;" if scalar else "vector_float src0_reg;\n    vector_float src1_reg;"
    loads = "asc_loadalign(src_reg, src);" if scalar else "asc_loadalign(src0_reg, src0);\n    asc_loadalign(src1_reg, src1);"
    call_args = "predicate, src_reg, COMPARE_VALUE, mask" if scalar else "predicate, src0_reg, src1_reg, mask"
    kernel_args = "__gm__ float* dst, __gm__ float* src" if scalar else "__gm__ float* dst, __gm__ float* src0, __gm__ float* src1"
    local_decls = "__ubuf__ float src_local[ELEMENT_COUNT];" if scalar else "__ubuf__ float src0_local[ELEMENT_COUNT];\n    __ubuf__ float src1_local[ELEMENT_COUNT];"
    copies = "asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(float));" if scalar else "asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));\n    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));"
    compute_args = "dst_local, src_local" if scalar else "dst_local, src0_local, src1_local"
    vectors = "std::vector<float> src(ELEMENT_COUNT);" if scalar else "std::vector<float> src0(ELEMENT_COUNT);\n    std::vector<float> src1(ELEMENT_COUNT);"
    init = (
        "src[i] = static_cast<float>(i % 8);\n        golden[i] = src[i] " + operator + " COMPARE_VALUE ? 1.0f : 0.0f;"
        if scalar else
        "src0[i] = static_cast<float>(i % 8);\n        src1[i] = static_cast<float>((i / 2) % 8);\n        golden[i] = src0[i] " + operator + " src1[i] ? 1.0f : 0.0f;"
    )
    device_decls = "CApiExample::DeviceBuffer<float> src_device(ELEMENT_COUNT);" if scalar else "CApiExample::DeviceBuffer<float> src0_device(ELEMENT_COUNT);\n    CApiExample::DeviceBuffer<float> src1_device(ELEMENT_COUNT);"
    alloc = (
        '!src_device.Allocate("aclrtMalloc(src)") || !dst_device.Allocate("aclrtMalloc(dst)") || !src_device.CopyFrom(src)'
        if scalar else
        '!src0_device.Allocate("aclrtMalloc(src0)") || !src1_device.Allocate("aclrtMalloc(src1)") ||\n        !dst_device.Allocate("aclrtMalloc(dst)") || !src0_device.CopyFrom(src0) || !src1_device.CopyFrom(src1)'
    )
    launch_args = "dst_device.Get(), src_device.Get()" if scalar else "dst_device.Get(), src0_device.Get(), src1_device.Get()"
    prints = 'CApiExample::Print("Input", src);' if scalar else 'CApiExample::Print("Input src0", src0);\n    CApiExample::Print("Input src1", src1);'
    return header(api) + f'''#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr float COMPARE_VALUE = 3.0f;

__simd_vf__ inline void Compare(__ubuf__ float* dst, {input_args})
{{
    vector_float dst_reg;
    vector_float ones;
    vector_float zeros;
    {reg_decls}
    vector_bool predicate;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    {loads}
    {api}({call_args});
    asc_duplicate_scalar(ones, 1.0f, mask);
    asc_duplicate_scalar(zeros, 0.0f, mask);
    asc_select(dst_reg, ones, zeros, predicate);
    asc_storealign(dst, dst_reg, mask);
}}

__global__ __vector__ void {kernel}({kernel_args})
{{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    {local_decls}
    {copies}
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compare({compute_args});
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}}
}} // namespace

int main()
{{
    {vectors}
    std::vector<float> output(ELEMENT_COUNT, 0.0f);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        {init}
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    {device_decls}
    CApiExample::DeviceBuffer<float> dst_device(ELEMENT_COUNT);
    if ({alloc}) return 1;
    {kernel}<<<1, 0>>>({launch_args});
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !dst_device.CopyTo(output)) return 1;
    {prints}
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    for api, operator in OPERATORS.items():
        (OUTPUT_DIR / f"{api}.asc").write_text(source(api, operator, False))
        scalar_api = f"{api}_scalar"
        (OUTPUT_DIR / f"{scalar_api}.asc").write_text(source(scalar_api, operator, True))


if __name__ == "__main__":
    main()
