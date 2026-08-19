#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def mem_bar_source() -> str:
    rel = "reg/sync_control/asc_mem_bar"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void StoreThenLoad(
    __ubuf__ uint32_t* output, __ubuf__ uint32_t* scratch, __ubuf__ uint32_t* input)
{{
    vector_uint32_t input_reg;
    vector_uint32_t stored_reg;
    vector_uint32_t loaded_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(input_reg, input);
    asc_add(stored_reg, input_reg, input_reg, mask);
    asc_storealign(scratch, stored_reg, mask);
    asc_mem_bar(VST_VLD);
    asc_loadalign(loaded_reg, scratch);
    asc_storealign(output, loaded_reg, mask);
}}

__global__ __vector__ void AscMemBarKernel(__gm__ uint32_t* output, __gm__ uint32_t* input)
{{
    asc_init();
    __ubuf__ uint32_t input_local[ELEMENT_COUNT];
    __ubuf__ uint32_t scratch[ELEMENT_COUNT];
    __ubuf__ uint32_t output_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    StoreThenLoad(output_local, scratch, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint32_t> input(ELEMENT_COUNT), output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {{
        input[i] = i + 1;
        golden[i] = input[i] * 2;
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint32_t> input_device(ELEMENT_COUNT), output_device(ELEMENT_COUNT);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input)) return 1;
    AscMemBarKernel<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output after store/barrier/load", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] asc_mem_bar VST_VLD dependency passed."
                         : "[Failed] asc_mem_bar VST_VLD dependency failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def clear_ar_source() -> str:
    rel = "reg/system_variables_access/asc_clear_ar_spr"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENTS_PER_REGISTER = 128;
constexpr uint32_t REPEAT_COUNT = 2;
constexpr uint32_t INPUT_COUNT = ELEMENTS_PER_REGISTER * REPEAT_COUNT;
constexpr uint32_t OUTPUT_COUNT = INPUT_COUNT / 4;

__simd_vf__ inline void PackSelected(__ubuf__ uint16_t* output, __ubuf__ uint16_t* input)
{{
    __ubuf__ uint16_t* dst = output;
    vector_uint16_t src;
    vector_uint16_t squeezed;
    vector_store_unalign unalign;
    vector_bool every_fourth = asc_create_mask_b16(PAT_M4);
    asc_clear_ar_spr();
    for (uint32_t repeat = 0; repeat < REPEAT_COUNT; ++repeat) {{
        asc_loadalign(src, input + repeat * ELEMENTS_PER_REGISTER);
        asc_squeeze_with_status(squeezed, src, every_fourth);
        asc_storeunalign_postupdate(dst, unalign, squeezed);
    }}
    asc_storeunalign_post(dst, unalign);
}}

__global__ __vector__ void AscClearArSprKernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{{
    asc_init();
    __ubuf__ uint16_t input_local[INPUT_COUNT];
    __ubuf__ uint16_t output_local[OUTPUT_COUNT];
    asc_copy_gm2ub_align(input_local, input, INPUT_COUNT * sizeof(uint16_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    PackSelected(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_COUNT * sizeof(uint16_t));
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint16_t> input(INPUT_COUNT), output(OUTPUT_COUNT, 0), golden;
    for (uint32_t i = 0; i < INPUT_COUNT; ++i) input[i] = i + 1;
    golden.reserve(OUTPUT_COUNT);
    for (uint32_t repeat = 0; repeat < REPEAT_COUNT; ++repeat) {{
        for (uint32_t i = 0; i < ELEMENTS_PER_REGISTER; i += 4) {{
            golden.push_back(input[repeat * ELEMENTS_PER_REGISTER + i]);
        }}
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint16_t> input_device(INPUT_COUNT), output_device(OUTPUT_COUNT);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input)) return 1;
    AscClearArSprKernel<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Packed output after AR reset", output);
    CApiExample::Print("Golden every-fourth values", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] asc_clear_ar_spr started contiguous squeeze storage at offset zero."
                         : "[Failed] asc_clear_ar_spr squeeze storage failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    sync_dir = ROOT / "reg" / "sync_control"
    system_dir = ROOT / "reg" / "system_variables_access"
    sync_dir.mkdir(parents=True, exist_ok=True)
    system_dir.mkdir(parents=True, exist_ok=True)
    (sync_dir / "asc_mem_bar.asc").write_text(mem_bar_source())
    (system_dir / "asc_clear_ar_spr.asc").write_text(clear_ar_source())


if __name__ == "__main__":
    main()
