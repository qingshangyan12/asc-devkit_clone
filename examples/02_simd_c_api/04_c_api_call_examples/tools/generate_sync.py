#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "sync"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(name: str) -> str:
    rel = f"sync/{name}"
    binary = f"/tmp/c_api_sync_{name}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/{rel}.asc -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
'''


def add_source(name: str, schedule: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return header(name) + f'''
constexpr uint32_t ELEMENTS = 64;
constexpr uint32_t BYTES = ELEMENTS * sizeof(float);

void PrintData(const char* label, const std::vector<float>& data)
{{
    std::cout << label << ":";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}}

__simd_vf__ inline void Add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1)
{{
    vector_float x, y, z;
    asc_loadalign(x, src0);
    asc_loadalign(y, src1);
    asc_add(z, x, y, asc_create_mask_b32(PAT_ALL));
    asc_storealign(dst, z, asc_create_mask_b32(PAT_ALL));
}}

__global__ __vector__ void {kernel}(__gm__ float* output, __gm__ float* src0, __gm__ float* src1)
{{
    asc_init();
    __ubuf__ float x[ELEMENTS], y[ELEMENTS], z[ELEMENTS];
{schedule}
}}
}} // namespace

int main()
{{
    std::vector<float> src0(ELEMENTS), src1(ELEMENTS), output(ELEMENTS, 0.0f), golden(ELEMENTS);
    for (uint32_t i = 0; i < ELEMENTS; ++i) {{
        src0[i] = static_cast<float>(i) * 0.25f;
        src1[i] = static_cast<float>(ELEMENTS - i) * 0.5f;
        golden[i] = src0[i] + src1[i];
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    float *src0_device = nullptr, *src1_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src0_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&src1_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src0_device, BYTES, src0.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src1_device, BYTES, src1.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, src0_device, src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input src0", src0);
    PrintData("Input src1", src1);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(src0_device);
    aclrtFree(src1_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def cross_source(name: str, scope: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    kernel_type = "__vector__" if scope == "inter" else "__mix__(1, 2)"
    branch_open = "" if scope == "inter" else "    if ASC_IS_AIV {\n"
    branch_close = "" if scope == "inter" else "    }\n"
    core_id = "asc_get_block_idx()" if scope == "inter" else "asc_get_sub_block_id()"
    launch_blocks = 2 if scope == "inter" else 1
    return header(name) + f'''
__global__ {kernel_type} void {kernel}(__gm__ uint32_t* output, __gm__ uint32_t* input)
{{
    asc_init();
{branch_open}    const uint32_t core_id = static_cast<uint32_t>({core_id});
    const uint32_t peer_id = 1U - core_id;
    __ubuf__ uint32_t local[8];
    asc_copy_gm2ub_align(local, input + core_id * 8, 32);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output + core_id * 8, local, 32);
    asc_sync_mte3(0);
    asc_sync_{scope}_arrive(PIPE_MTE3, static_cast<int64_t>(1));
    asc_sync_{scope}_wait(PIPE_MTE2, static_cast<int64_t>(1));
    asc_copy_gm2ub_align(local, output + peer_id * 8, 32);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output + 16 + core_id * 8, local, 32);
    asc_sync_mte3(0);
{branch_close}}}
}} // namespace

int main()
{{
    std::vector<uint32_t> input(16), output(32, 0), golden(32);
    for (uint32_t i = 0; i < 16; ++i) input[i] = i + 1;
    for (uint32_t i = 0; i < 16; ++i) {{
        golden[i] = input[i];
        golden[16 + i] = input[(i + 8) % 16];
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), 64, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), 128, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, 64, input.data(), 64, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<{launch_blocks}, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), 128, output_device, 128, ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input groups: ";
    for (uint32_t i = 0; i < 16; ++i) std::cout << input[i] << (i == 7 ? " | " : " ");
    std::cout << std::endl << "Peer data after sync: ";
    for (uint32_t i = 16; i < 32; ++i) std::cout << output[i] << (i == 23 ? " | " : " ");
    std::cout << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def block_source(name: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return header(name) + f'''
constexpr uint32_t ELEMENTS = 64;

__global__ __mix__(1, 2) void {kernel}(__gm__ uint32_t* output, __gm__ uint32_t* input)
{{
    asc_init();
    constexpr int64_t flag_id = 1;
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            output[0] = input[0];
            asc_dcci_single(output);
        }}
        asc_sync_pipe(PIPE_ALL);
        asc_sync_block_arrive(PIPE_V, flag_id);
    }}
    if ASC_IS_AIC {{
        asc_sync_block_wait(PIPE_M, flag_id);
        asc_sync_pipe(PIPE_ALL);
        asc_dci();
        output[1] = output[0];
        asc_dcci_single(output);
    }}
}}
}} // namespace

int main()
{{
    std::vector<uint32_t> input(ELEMENTS, 0), output(ELEMENTS, 0);
    input[0] = 0x12345678U;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), ELEMENTS * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), ELEMENTS * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, ELEMENTS * sizeof(uint32_t), input.data(), ELEMENTS * sizeof(uint32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), ELEMENTS * sizeof(uint32_t), output_device, ELEMENTS * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input: 0x" << std::hex << input[0] << std::dec << std::endl;
    std::cout << "AIV write / AIC read: 0x" << std::hex << output[0] << " / 0x" << output[1]
              << std::dec << std::endl;
    const bool passed = output[0] == input[0] && output[1] == input[0];
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def intra_source(name: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return header(name) + f'''
constexpr uint32_t ELEMENTS = 64;

__global__ __mix__(1, 2) void {kernel}(__gm__ uint32_t* output, __gm__ uint32_t* input)
{{
    asc_init();
    constexpr uint64_t sync_id = 1;
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            output[0] = input[0];
            asc_dcci_single(output);
            asc_sync_intra_arrive(PIPE_S, sync_id);
        }}
    }}
    if ASC_IS_AIC {{
        asc_sync_intra_wait(PIPE_S, sync_id);
        asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
        asc_dci();
        output[1] = output[0];
        asc_dcci_single(output);
    }}
}}
}} // namespace

int main()
{{
    std::vector<uint32_t> input(ELEMENTS, 0), output(ELEMENTS, 0);
    input[0] = 0x12345678U;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), ELEMENTS * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), ELEMENTS * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, ELEMENTS * sizeof(uint32_t), input.data(), ELEMENTS * sizeof(uint32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), ELEMENTS * sizeof(uint32_t), output_device, ELEMENTS * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input: 0x" << std::hex << input[0] << std::dec << std::endl;
    std::cout << "AIV write / AIC read: 0x" << std::hex << output[0] << " / 0x" << output[1]
              << std::dec << std::endl;
    const bool passed = output[0] == input[0] && output[1] == input[0];
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


LOADS = '''    asc_copy_gm2ub_align(x, src0, BYTES);
    asc_copy_gm2ub_align(y, src1, BYTES);'''
ADD = "    Add(z, x, y);"
STORE = "    asc_copy_ub2gm_align(output, z, BYTES);"


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    lock_schedule = f'''    asc_lock(PIPE_MTE2, 1);
{LOADS}
    asc_unlock(PIPE_MTE2, 1);
    asc_lock(PIPE_V, 1);
{ADD}
    asc_unlock(PIPE_V, 1);
    asc_lock(PIPE_MTE3, 1);
{STORE}
    asc_unlock(PIPE_MTE3, 1);
    asc_sync();'''
    notify_schedule = f'''{LOADS}
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
{ADD}
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
{STORE}
    asc_sync_mte3(0);'''
    cases = {
        "asc_lock": add_source("asc_lock", lock_schedule),
        "asc_unlock": add_source("asc_unlock", lock_schedule),
        "asc_sync": add_source("asc_sync", f'''{LOADS}\n    asc_sync();\n{ADD}\n    asc_sync();\n{STORE}\n    asc_sync();'''),
        "asc_sync_data_barrier": add_source("asc_sync_data_barrier", f'''{LOADS}\n    asc_sync();\n{ADD}\n    asc_sync();\n{STORE}\n    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);\n    asc_sync();'''),
        "asc_sync_mte2": add_source("asc_sync_mte2", f'''{LOADS}\n    asc_sync_mte2(0);\n{ADD}\n    asc_sync();\n{STORE}\n    asc_sync_mte3(0);'''),
        "asc_sync_mte3": add_source("asc_sync_mte3", f'''{LOADS}\n    asc_sync();\n{ADD}\n    asc_sync();\n{STORE}\n    asc_sync_mte3(0);'''),
        "asc_sync_notify": add_source("asc_sync_notify", notify_schedule),
        "asc_sync_wait": add_source("asc_sync_wait", notify_schedule),
        "asc_sync_pipe": add_source("asc_sync_pipe", f'''{LOADS}\n    asc_sync_pipe(PIPE_ALL);\n{ADD}\n    asc_sync_pipe(PIPE_ALL);\n{STORE}\n    asc_sync_pipe(PIPE_ALL);'''),
        "asc_sync_vec": add_source("asc_sync_vec", f'''{LOADS}\n    asc_sync();\n{ADD}\n    asc_sync_vec();\n{STORE}\n    asc_sync_mte3(0);'''),
        "asc_sync_block_arrive": block_source("asc_sync_block_arrive"),
        "asc_sync_block_wait": block_source("asc_sync_block_wait"),
        "asc_sync_inter_arrive": cross_source("asc_sync_inter_arrive", "inter"),
        "asc_sync_inter_wait": cross_source("asc_sync_inter_wait", "inter"),
        "asc_sync_subblock_arrive": cross_source("asc_sync_subblock_arrive", "subblock"),
        "asc_sync_subblock_wait": cross_source("asc_sync_subblock_wait", "subblock"),
        "asc_sync_intra_arrive": intra_source("asc_sync_intra_arrive"),
        "asc_sync_intra_wait": intra_source("asc_sync_intra_wait"),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} functional synchronization examples in {OUT}")


if __name__ == "__main__":
    main()
