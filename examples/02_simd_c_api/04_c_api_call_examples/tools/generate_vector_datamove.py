#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "vector_datamove"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(rel: str) -> str:
    binary = "/tmp/c_api_vector_datamove_" + rel.replace("/", "_")
    return f'''// Build/run ({PREFIX}/vector_datamove/{rel}.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/vector_datamove/{rel}.asc -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
'''


def vector_source(
    rel: str, body: str, input_bytes: int, output_bytes: int, golden: str, label: str) -> str:
    kernel = "".join(part.capitalize() for part in Path(rel).stem.split("_")) + "Kernel"
    return header(rel) + f'''
constexpr uint32_t INPUT_BYTES = {input_bytes};
constexpr uint32_t OUTPUT_BYTES = {output_bytes};

__global__ __vector__ void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input)
{{
    asc_init();
{body}
}}

void PrintData(const char* name, const std::vector<uint8_t>& data)
{{
    std::cout << name << ":";
    const uint32_t count = data.size() < 32 ? data.size() : 32;
    for (uint32_t i = 0; i < count; ++i) std::cout << ' ' << +data[i];
    if (data.size() > count) std::cout << " ...";
    std::cout << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES, 0), golden(OUTPUT_BYTES, 0);
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i + 1);
{golden}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {label} passed." : "[Failed] {label} failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def duplicate_golden(bytes_count: int = 256) -> str:
    return f'''    for (uint32_t copy = 0; copy < 2; ++copy) {{
        for (uint32_t i = 0; i < {bytes_count}; ++i) golden[copy * {bytes_count} + i] = input[i];
    }}'''


def continuous_copy(rel: str, direction: str, aligned: bool) -> str:
    api = f"asc_copy_{direction}" + ("_align" if aligned else "")
    sync_api = api + "_sync"
    if direction == "gm2ub":
        body = f'''    __ubuf__ uint8_t local[OUTPUT_BYTES];
    {api}(local, input, INPUT_BYTES);
    asc_sync_mte2(0);
    {sync_api}(local + INPUT_BYTES, input, INPUT_BYTES);
    asc_sync();
    asc_copy_ub2gm_align(output, local, OUTPUT_BYTES);
    asc_sync_mte3(0);'''
    else:
        body = f'''    __ubuf__ uint8_t local[INPUT_BYTES];
    asc_copy_gm2ub_align(local, input, INPUT_BYTES);
    asc_sync_mte2(0);
    {api}(output, local, INPUT_BYTES);
    {sync_api}(output + INPUT_BYTES, local, INPUT_BYTES);
    asc_sync_mte3(0);'''
    return vector_source(rel, body, 256, 512, duplicate_golden(), f"{api}/{sync_api}")


GM_LOOP_BODY = '''    __ubuf__ uint8_t local[OUTPUT_BYTES];
    asc_set_gm2ub_loop_size(2, 2);
    asc_set_gm2ub_loop1_stride(64, 32);
    asc_set_gm2ub_loop2_stride(128, 64);
    asc_copy_gm2ub_align(local, input, 1, 32, 0, 0, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
    asc_set_gm2ub_loop_size(1, 1);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, OUTPUT_BYTES);
    asc_sync_mte3(0);'''

UB_LOOP_BODY = '''    __ubuf__ uint8_t local[INPUT_BYTES];
    asc_copy_gm2ub_align(local, input, INPUT_BYTES);
    asc_sync_mte2(0);
    asc_set_ub2gm_loop_size(2, 2);
    asc_set_ub2gm_loop1_stride(64, 32);
    asc_set_ub2gm_loop2_stride(128, 64);
    asc_copy_ub2gm_align(output, local, 1, 32, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
    asc_set_ub2gm_loop_size(1, 1);
    asc_sync_mte3(0);'''

LOOP_GOLDEN = '''    for (uint32_t block = 0; block < 4; ++block) {
        const uint32_t source = (block / 2) * 128 + (block % 2) * 64;
        for (uint32_t i = 0; i < 32; ++i) golden[block * 32 + i] = input[source + i];
    }'''


def padding_source(rel: str, deprecated: bool) -> str:
    setter = "asc_set_gm2ub_pad(0x2a2a2a2aU);" if deprecated else "asc_set_copy_pad_val(static_cast<uint8_t>(42));"
    body = f'''    __ubuf__ uint8_t local[OUTPUT_BYTES];
    {setter}
    asc_copy_gm2ub_align(local, input, 2, 24, 4, 4, true,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 24, 32);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, OUTPUT_BYTES);
    asc_sync_mte3(0);'''
    golden = '''    for (uint32_t block = 0; block < 2; ++block) {
        for (uint32_t i = 0; i < 4; ++i) golden[block * 32 + i] = 42;
        for (uint32_t i = 0; i < 24; ++i) golden[block * 32 + 4 + i] = input[block * 24 + i];
        for (uint32_t i = 28; i < 32; ++i) golden[block * 32 + i] = 42;
    }'''
    label = "asc_set_gm2ub_pad" if deprecated else "asc_set_copy_pad_val"
    return vector_source(rel, body, 48, 64, golden, label)


NDIM_TRANSPOSE_BODY = '''    __ubuf__ uint8_t local[32];
    asc_set_ndim_loop0_stride(4, 1);
    asc_set_ndim_loop1_stride(1, 3);
    asc_set_ndim_loop2_stride(0, 0);
    asc_set_ndim_loop3_stride(0, 0);
    asc_set_ndim_loop4_stride(0, 0);
    asc_ndim_pad_count_config config;
    asc_set_ndim_pad_count(config);
    asc_ndim_copy_gm2ub(local, input, 3, 4, 1, 1, 1, 0, 0, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, OUTPUT_BYTES);
    asc_sync_mte3(0);'''

NDIM_TRANSPOSE_GOLDEN = '''    for (uint32_t column = 0; column < 3; ++column) {
        for (uint32_t row = 0; row < 4; ++row) golden[column * 4 + row] = input[row * 3 + column];
    }'''

NDIM_PADDING_BODY = '''    __ubuf__ uint8_t local[32];
    asc_set_ndim_loop0_stride(1, 1);
    asc_set_ndim_loop1_stride(5, 3);
    asc_set_ndim_loop2_stride(0, 0);
    asc_set_ndim_loop3_stride(0, 0);
    asc_set_ndim_loop4_stride(0, 0);
    asc_set_ndim_pad_value(static_cast<uint8_t>(42));
    asc_ndim_pad_count_config config;
    config.loop1_lp_count = 1;
    config.loop1_rp_count = 1;
    asc_set_ndim_pad_count(config);
    asc_ndim_copy_gm2ub(local, input, 3, 2, 1, 1, 1, 1, 1, true,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, OUTPUT_BYTES);
    asc_sync_mte3(0);'''

NDIM_PADDING_GOLDEN = '''    for (uint32_t i = 0; i < OUTPUT_BYTES; ++i) golden[i] = 42;
    for (uint32_t row = 0; row < 2; ++row) {
        for (uint32_t column = 0; column < 3; ++column) golden[(row + 1) * 5 + column + 1] = input[row * 3 + column];
    }'''


def ub2l1_source() -> str:
    rel = "asc_copy_ub2l1"
    return header(rel) + r'''
constexpr uint32_t BYTES = 256;

__global__ __mix__(1, 2) void AscCopyUb2l1Kernel(
    __gm__ uint8_t* output, __gm__ uint8_t* input, uint32_t mode)
{
    asc_init();
    __ubuf__ uint8_t source[BYTES], readback[BYTES];
    __cbuf__ uint8_t l1[BYTES];
    constexpr int64_t block_flag = 2;
    constexpr uint64_t intra_id = 2;
    if ASC_IS_AIV {
        if (asc_get_sub_block_id() == 0) {
            asc_copy_gm2ub_align(source, input, BYTES);
            asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
            asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
            if (mode == 0) asc_copy_ub2l1(l1, source, BYTES);
            else asc_copy_ub2l1_sync(l1, source, BYTES);
        }
        asc_sync_block_arrive(PIPE_MTE3, block_flag);
        if (asc_get_sub_block_id() == 0) {
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output + mode * BYTES, readback, BYTES);
            asc_sync_mte3(0);
        }
    }
    if ASC_IS_AIC {
        asc_sync_block_wait(PIPE_MTE1, block_flag);
        asc_copy_l12ub(readback, l1, false, 1, BYTES / 32, 0, 0);
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);
    }
}

void PrintData(const char* name, const std::vector<uint8_t>& data)
{
    std::cout << name << ":";
    for (uint32_t i = 0; i < 16; ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint8_t> input(BYTES), output(BYTES * 2, 0), golden(BYTES * 2);
    for (uint32_t i = 0; i < BYTES; ++i) {
        input[i] = static_cast<uint8_t>(i + 1);
        golden[i] = golden[BYTES + i] = input[i];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES * 2, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BYTES, input.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    for (uint32_t mode = 0; mode < 2; ++mode) {
        AscCopyUb2l1Kernel<<<1, 0>>>(output_device, input_device, mode);
        aclrtSynchronizeDevice();
    }
    aclrtMemcpy(output.data(), BYTES * 2, output_device, BYTES * 2, ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
    PrintData("L1 round-trip output", output);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_ub2l1/asc_copy_ub2l1_sync passed."
                         : "[Failed] asc_copy_ub2l1/asc_copy_ub2l1_sync failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def main() -> None:
    cases = {
        "asc_copy_gm2ub/asc_copy_gm2ub_arch_3510": continuous_copy(
            "asc_copy_gm2ub/asc_copy_gm2ub_arch_3510", "gm2ub", False),
        "asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510": continuous_copy(
            "asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510", "gm2ub", True),
        "asc_copy_ub2gm/asc_copy_ub2gm_arch_3510": continuous_copy(
            "asc_copy_ub2gm/asc_copy_ub2gm_arch_3510", "ub2gm", False),
        "asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510": continuous_copy(
            "asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510", "ub2gm", True),
        "asc_copy_ub2ub": vector_source(
            "asc_copy_ub2ub",
            '''    __ubuf__ uint8_t source[INPUT_BYTES], first[INPUT_BYTES], second[INPUT_BYTES];
    asc_copy_gm2ub_align(source, input, INPUT_BYTES);
    asc_sync_mte2(0);
    asc_copy_ub2ub(first, source, INPUT_BYTES);
    asc_sync();
    asc_copy_ub2ub_sync(second, source, INPUT_BYTES);
    asc_copy_ub2gm_align(output, first, INPUT_BYTES);
    asc_copy_ub2gm_align(output + INPUT_BYTES, second, INPUT_BYTES);
    asc_sync_mte3(0);''',
            256, 512, duplicate_golden(), "asc_copy_ub2ub/asc_copy_ub2ub_sync"),
        "asc_copy_ub2l1": ub2l1_source(),
        "asc_ndim_copy_gm2ub": vector_source(
            "asc_ndim_copy_gm2ub", NDIM_TRANSPOSE_BODY, 12, 12, NDIM_TRANSPOSE_GOLDEN, "asc_ndim_copy_gm2ub"),
        "asc_set_copy_pad_val": padding_source("asc_set_copy_pad_val", False),
        "asc_set_gm2ub_loop1_stride": vector_source(
            "asc_set_gm2ub_loop1_stride", GM_LOOP_BODY, 256, 128, LOOP_GOLDEN, "asc_set_gm2ub_loop1_stride"),
        "asc_set_gm2ub_loop2_stride": vector_source(
            "asc_set_gm2ub_loop2_stride", GM_LOOP_BODY, 256, 128, LOOP_GOLDEN, "asc_set_gm2ub_loop2_stride"),
        "asc_set_gm2ub_loop_size": vector_source(
            "asc_set_gm2ub_loop_size", GM_LOOP_BODY, 256, 128, LOOP_GOLDEN, "asc_set_gm2ub_loop_size"),
        "asc_set_gm2ub_pad_deprecated": padding_source("asc_set_gm2ub_pad_deprecated", True),
        "asc_set_ndim_loop_stride": vector_source(
            "asc_set_ndim_loop_stride", NDIM_TRANSPOSE_BODY, 12, 12, NDIM_TRANSPOSE_GOLDEN,
            "asc_set_ndim_loop0_stride/.../asc_set_ndim_loop4_stride"),
        "asc_set_ndim_pad_count": vector_source(
            "asc_set_ndim_pad_count", NDIM_PADDING_BODY, 6, 20, NDIM_PADDING_GOLDEN, "asc_set_ndim_pad_count"),
        "asc_set_ndim_pad_value": vector_source(
            "asc_set_ndim_pad_value", NDIM_PADDING_BODY, 6, 20, NDIM_PADDING_GOLDEN, "asc_set_ndim_pad_value"),
        "asc_set_ub2gm_loop1_stride": vector_source(
            "asc_set_ub2gm_loop1_stride", UB_LOOP_BODY, 256, 128, LOOP_GOLDEN, "asc_set_ub2gm_loop1_stride"),
        "asc_set_ub2gm_loop2_stride": vector_source(
            "asc_set_ub2gm_loop2_stride", UB_LOOP_BODY, 256, 128, LOOP_GOLDEN, "asc_set_ub2gm_loop2_stride"),
        "asc_set_ub2gm_loop_size": vector_source(
            "asc_set_ub2gm_loop_size", UB_LOOP_BODY, 256, 128, LOOP_GOLDEN, "asc_set_ub2gm_loop_size"),
    }
    for rel, text in cases.items():
        path = OUT / f"{rel}.asc"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} functional vector data movement examples in {OUT}")


if __name__ == "__main__":
    main()
