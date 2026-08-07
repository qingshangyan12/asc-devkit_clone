#!/usr/bin/env python3

import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def raw_source(rel, kernel, api, aic_body, input_bytes, output_bytes, launches=2, golden=""):
    binary = "c_api_cube_datamove_" + rel.replace("/", "_").replace(".asc", "")
    if not golden:
        golden = f'''    for (uint32_t mode = 0; mode < {launches}; ++mode) {{
        for (uint32_t i = 0; i < OUTPUT_BYTES; ++i) golden[mode * OUTPUT_BYTES + i] = input[i];
    }}'''
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t INPUT_BYTES = {input_bytes};
constexpr uint32_t OUTPUT_BYTES = {output_bytes};

__global__ __mix__(1, 2) void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input, uint32_t mode)
{{
    asc_init();
    __cbuf__ uint8_t l1[256];
    __ubuf__ uint8_t readback[256];
    constexpr uint64_t intra_id = 2;
    if ASC_IS_AIC {{
{aic_body}
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);
    }}
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output + mode * OUTPUT_BYTES, readback, OUTPUT_BYTES);
            asc_sync_mte3(0);
        }}
    }}
}}

void PrintData(const char* label, const std::vector<uint8_t>& data)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 16 && i < data.size(); ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES * {launches}), golden(output.size());
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i + 1);
{golden}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    for (uint32_t mode = 0; mode < {launches}; ++mode) {{
        {kernel}<<<1, 0>>>(output_device, input_device, mode);
        aclrtSynchronizeDevice();
    }}
    aclrtMemcpy(output.data(), output.size(), output_device, output.size(), ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} demonstrated its data layout."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def fill_source():
    rel = "asc_fill_l1.asc"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/c_api_cube_datamove_asc_fill_l1 && /tmp/c_api_cube_datamove_asc_fill_l1
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t ELEMENTS = 64;

__global__ __mix__(1, 2) void AscFillL1Kernel(__gm__ float* output, uint32_t mode)
{{
    asc_init();
    __cbuf__ float l1[ELEMENTS];
    __ubuf__ float readback[ELEMENTS];
    constexpr uint64_t intra_id = 2;
    if ASC_IS_AIC {{
        asc_fill_value_config config{{}};
        config.repeat = 1;
        config.blk_num = ELEMENTS * sizeof(float) / 32;
        config.dst_gap = 0;
        if (mode == 0) asc_fill_l1(l1, 0x3f800000U, config);
        else asc_fill_l1_sync(l1, 0x3f800000U, config);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(readback, l1, false, 1, ELEMENTS * sizeof(float) / 32, 0, 0);
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);
    }}
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output + mode * ELEMENTS, readback, ELEMENTS * sizeof(float));
            asc_sync_mte3(0);
        }}
    }}
}}
}} // namespace

int main()
{{
    std::vector<float> output(ELEMENTS * 2), golden(ELEMENTS * 2, 1.0f);
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    for (uint32_t mode = 0; mode < 2; ++mode) {{
        AscFillL1Kernel<<<1, 0>>>(output_device, mode);
        aclrtSynchronizeDevice();
    }}
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Filled L1 values:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output[i];
    std::cout << " ..." << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_fill_l1/asc_fill_l1_sync filled L1 with 1.0."
                         : "[Failed] asc_fill_l1 result mismatch.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def sources():
    plain_readback = '''        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);'''
    items = {}
    body = '''        if (mode == 0) asc_copy_gm2l1(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
        else asc_copy_gm2l1_sync(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
''' + plain_readback
    items["asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.asc"] = raw_source(
        "asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.asc", "AscCopyGm2l1Kernel",
        "asc_copy_gm2l1/asc_copy_gm2l1_sync", body, 64, 64)

    body = '''        if (mode == 0) asc_copy_gm2l1_align(l1, input, 2, 32, 0, 0, false, 0, 32, 32);
        else asc_copy_gm2l1_align_sync(l1, input, 2, 32, 0, 0, false, 0, 32, 32);
''' + plain_readback
    items["asc_copy_gm2l1_align.asc"] = raw_source(
        "asc_copy_gm2l1_align.asc", "AscCopyGm2l1AlignKernel",
        "asc_copy_gm2l1_align/asc_copy_gm2l1_align_sync", body, 64, 64)

    body = '''        asc_copy_gm2l1(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        if (mode == 0) asc_copy_l12ub(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);
        else asc_copy_l12ub_sync(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);'''
    items["asc_copy_l12ub.asc"] = raw_source(
        "asc_copy_l12ub.asc", "AscCopyL12ubKernel", "asc_copy_l12ub/asc_copy_l12ub_sync", body, 64, 64)

    loop_cases = [
        ("asc_set_gm2l1_loop1_stride.asc", "AscSetGm2l1Loop1StrideKernel", "asc_set_gm2l1_loop1_stride",
         "asc_set_gm2l1_loop_size(2, 1);\n        asc_set_gm2l1_loop1_stride(64, 32);", 96, 64,
         '''    for (uint32_t i = 0; i < 32; ++i) { golden[i] = input[i]; golden[32 + i] = input[64 + i]; }'''),
        ("asc_set_gm2l1_loop2_stride.asc", "AscSetGm2l1Loop2StrideKernel", "asc_set_gm2l1_loop2_stride",
         "asc_set_gm2l1_loop_size(1, 2);\n        asc_set_gm2l1_loop2_stride(64, 32);", 96, 64,
         '''    for (uint32_t i = 0; i < 32; ++i) { golden[i] = input[i]; golden[32 + i] = input[64 + i]; }'''),
        ("asc_set_gm2l1_loop_size.asc", "AscSetGm2l1LoopSizeKernel", "asc_set_gm2l1_loop_size",
         "asc_set_gm2l1_loop_size(2, 2);\n        asc_set_gm2l1_loop1_stride(64, 32);\n        asc_set_gm2l1_loop2_stride(128, 64);", 224, 128,
         '''    for (uint32_t block = 0; block < 4; ++block) {
        for (uint32_t i = 0; i < 32; ++i) golden[block * 32 + i] = input[block * 64 + i];
    }'''),
    ]
    for rel, kernel, api, setup, inp, out, golden in loop_cases:
        body = f'''        {setup}
        asc_copy_gm2l1_align(l1, input, 1, 32, 0, 0, false, 0, 0, 0);
        asc_set_gm2l1_loop_size(1, 1);
''' + plain_readback
        items[rel] = raw_source(rel, kernel, api, body, inp, out, 1, golden)

    pad_golden = '''    for (uint32_t i = 0; i < 8; ++i) golden[i] = golden[24 + i] = 0xa5;
    for (uint32_t i = 0; i < 16; ++i) golden[8 + i] = input[i];'''
    body = '''        asc_set_gm2l1_pad(0xa5a5a5a5U);
        asc_copy_gm2l1_align(l1, input, 1, 16, 8, 8, false, 0, 0, 0);
''' + plain_readback
    items["asc_set_gm2l1_pad.asc"] = raw_source(
        "asc_set_gm2l1_pad.asc", "AscSetGm2l1PadKernel", "asc_set_gm2l1_pad", body, 16, 32, 1, pad_golden)
    items["asc_fill_l1.asc"] = fill_source()
    return items


def main():
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/cube_datamove directory>")
    doc_dir = Path(sys.argv[1]).resolve()
    if not doc_dir.is_dir():
        raise SystemExit(f"Documentation directory does not exist: {doc_dir}")
    rendered = sources()
    for rel, text in rendered.items():
        destination = ROOT / "cube_datamove" / rel
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text, encoding="utf-8")
    print(f"Generated {len(rendered)} functional Cube data movement examples")


if __name__ == "__main__":
    main()
