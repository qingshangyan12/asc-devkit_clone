#!/usr/bin/env python3

import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "reg_data_types"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(file_name: str, body: str, golden: str, api_names: str) -> str:
    rel = f"reg/reg_data_types/{file_name}"
    kernel = "".join(part.capitalize() for part in file_name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t BUFFER_BYTES = 768;

__simd_vf__ inline void Exercise(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{{
{body}
}}

__global__ __vector__ void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input)
{{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Exercise(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(BUFFER_BYTES), output(BUFFER_BYTES, 0xff), golden(BUFFER_BYTES);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251);
{golden}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(BUFFER_BYTES), output_device(BUFFER_BYTES);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input bytes", input);
    CApiExample::Print("Output bytes", output);
    CApiExample::Print("Golden bytes", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {api_names} passed." : "[Failed] {api_names} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


MASK_BODY = '''    vector_uint8_t value8;
    vector_uint16_t value16;
    vector_uint32_t value32;
    vector_bool mask8 = {mask8};
    vector_bool mask16 = {mask16};
    vector_bool mask32 = {mask32};
    asc_duplicate_scalar(value8, static_cast<uint8_t>(1), mask8);
    asc_duplicate_scalar(value16, static_cast<uint16_t>(1), mask16);
    asc_duplicate_scalar(value32, static_cast<uint32_t>(1), mask32);
    asc_storealign(output, value8, mask8);
    asc_storealign(reinterpret_cast<__ubuf__ uint16_t*>(output + 256), value16, mask16);
    asc_storealign(reinterpret_cast<__ubuf__ uint32_t*>(output + 512), value32, mask32);'''


def generate() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_create_mask": source(
            "asc_create_mask",
            MASK_BODY.format(
                mask8="asc_create_mask_b8(PAT_ALL)",
                mask16="asc_create_mask_b16(PAT_ALL)",
                mask32="asc_create_mask_b32(PAT_ALL)"),
            "    std::fill(golden.begin(), golden.end(), static_cast<uint8_t>(1));",
            "asc_create_mask_b8/asc_create_mask_b16/asc_create_mask_b32"),
        "asc_update_mask": source(
            "asc_update_mask",
            '''    uint32_t count8 = 256, count16 = 128, count32 = 64;
''' + MASK_BODY.format(
                mask8="asc_update_mask_b8(count8)",
                mask16="asc_update_mask_b16(count16)",
                mask32="asc_update_mask_b32(count32)"),
            "    std::fill(golden.begin(), golden.end(), static_cast<uint8_t>(1));",
            "asc_update_mask_b8/asc_update_mask_b16/asc_update_mask_b32"),
        "asc_get_mask_spr": source(
            "asc_get_mask_spr",
            '''    uint32_t count16 = 128;
    vector_bool seed16 = asc_update_mask_b16(count16);
    vector_bool mask16 = asc_get_mask_spr_b16();
    vector_uint16_t value16;
    asc_duplicate_scalar(value16, static_cast<uint16_t>(1), seed16);
    asc_storealign(reinterpret_cast<__ubuf__ uint16_t*>(output), value16, mask16);
    uint32_t count32 = 64;
    vector_bool seed32 = asc_update_mask_b32(count32);
    vector_bool mask32 = asc_get_mask_spr_b32();
    vector_uint32_t value32;
    asc_duplicate_scalar(value32, static_cast<uint32_t>(1), seed32);
    asc_storealign(reinterpret_cast<__ubuf__ uint32_t*>(output + 256), value32, mask32);''',
            '''    std::fill(golden.begin(), golden.begin() + 512, static_cast<uint8_t>(1));
    std::copy(input.begin() + 512, input.end(), golden.begin() + 512);''',
            "asc_get_mask_spr_b16/asc_get_mask_spr_b32"),
        "asc_update_addr_reg": source(
            "asc_update_addr_reg",
            '''    for (uint16_t repeat = 0; repeat < 1; ++repeat) {
    vector_bool mask8 = asc_create_mask_b8(PAT_ALL);
    vector_bool mask16 = asc_create_mask_b16(PAT_ALL);
    vector_bool mask32 = asc_create_mask_b32(PAT_ALL);
    vector_uint8_t value8;
    vector_uint16_t value16;
    vector_uint32_t value32;
    addr_reg offset8 = asc_update_addr_reg_b8(0);
    addr_reg offset16 = asc_update_addr_reg_b16(0);
    addr_reg offset32 = asc_update_addr_reg_b32(0);
    asc_loadalign(value8, input, offset8);
    asc_loadalign(value16, reinterpret_cast<__ubuf__ uint16_t*>(input + 256), offset16);
    asc_loadalign(value32, reinterpret_cast<__ubuf__ uint32_t*>(input + 512), offset32);
    asc_storealign(output, value8, mask8);
    asc_storealign(reinterpret_cast<__ubuf__ uint16_t*>(output + 256), value16, mask16);
    asc_storealign(reinterpret_cast<__ubuf__ uint32_t*>(output + 512), value32, mask32);
    }''',
            "    golden = input;",
            "asc_update_addr_reg_b8/asc_update_addr_reg_b16/asc_update_addr_reg_b32"),
        "asc_create_iter_reg_deprecated": source(
            "asc_create_iter_reg_deprecated",
            '''    for (uint16_t repeat = 0; repeat < 1; ++repeat) {
    vector_bool mask8 = asc_create_mask_b8(PAT_ALL);
    vector_bool mask16 = asc_create_mask_b16(PAT_ALL);
    vector_bool mask32 = asc_create_mask_b32(PAT_ALL);
    vector_uint8_t value8;
    vector_uint16_t value16;
    vector_uint32_t value32;
    iter_reg offset8 = asc_create_iter_reg_b8(0);
    iter_reg offset16 = asc_create_iter_reg_b16(0);
    iter_reg offset32 = asc_create_iter_reg_b32(0);
    asc_loadalign(value8, input, offset8);
    asc_loadalign(value16, reinterpret_cast<__ubuf__ uint16_t*>(input + 256), offset16);
    asc_loadalign(value32, reinterpret_cast<__ubuf__ uint32_t*>(input + 512), offset32);
    asc_storealign(output, value8, mask8);
    asc_storealign(reinterpret_cast<__ubuf__ uint16_t*>(output + 256), value16, mask16);
    asc_storealign(reinterpret_cast<__ubuf__ uint32_t*>(output + 512), value32, mask32);
    }''',
            "    golden = input;",
            "asc_create_iter_reg_b8/asc_create_iter_reg_b16/asc_create_iter_reg_b32"),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} register data type examples in {OUT}")


if __name__ == "__main__":
    if len(sys.argv) != 1:
        raise SystemExit(f"Usage: {sys.argv[0]}")
    generate()
