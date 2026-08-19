#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(category: str, name: str) -> str:
    rel = f"{category}/{name}"
    binary = f"/tmp/c_api_{category}_{name}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/{rel}.asc -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t ELEMENTS = 16;
constexpr uint32_t BYTES = ELEMENTS * sizeof(uint64_t);

void PrintData(const char* label, const std::vector<uint64_t>& data)
{{
    std::cout << label << ":";
    const size_t count = data.size() < 8 ? data.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << data[i];
    if (data.size() > count) std::cout << " ...";
    std::cout << std::endl;
}}
'''


def host(category: str, name: str, kernel: str, initialize: str, validation: str,
         labels: str = '    PrintData("Output", output);\n') -> str:
    return f'''
}} // namespace

int main()
{{
    std::vector<uint64_t> input(ELEMENTS), output(ELEMENTS, 0);
{initialize}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint64_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BYTES, input.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, BYTES, output.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
{labels}    const bool passed = {validation};
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def datacache_preload() -> str:
    name = "asc_datacache_preload"
    kernel = "AscDatacachePreloadKernel"
    return header("cache_ctrl", name) + f'''
__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint64_t* input)
{{
    asc_init();
    // Invalidate DCache so input[0] is a cold Scalar read.
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    uint64_t begin = asc_get_system_cycle();
    const uint64_t cold_value = input[0];
    const uint64_t cold_cycles = asc_get_system_cycle() - begin;

    // Start from another cold cache line, then preload its 64 bytes into DCache.
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    begin = asc_get_system_cycle();
    // offset=64 selects the cache line containing input[8].
    asc_datacache_preload(input, 64);
    const uint64_t preload_cycles = asc_get_system_cycle() - begin;
    for (volatile uint32_t i = 0; i < 128; ++i) {{}}
    begin = asc_get_system_cycle();
    const uint64_t prefetched_value = input[8];
    const uint64_t prefetched_cycles = asc_get_system_cycle() - begin;

    output[0] = cold_value;
    output[1] = prefetched_value;
    output[2] = cold_cycles;
    output[3] = preload_cycles;
    output[4] = prefetched_cycles;
    // Publish the Scalar result stores; this is separate from DCache preload.
    asc_dcci_single(output);
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
}}
''' + host(
        "cache_ctrl", name, kernel,
        "    for (uint32_t i = 0; i < ELEMENTS; ++i) input[i] = 100 + i;",
        "output[0] == input[0] && output[1] == input[8] && output[2] + output[3] + output[4] > 0",
        '''    std::cout << "Values (cold, prefetched): " << output[0] << ' ' << output[1] << std::endl;
    std::cout << "Cycles (cold read, preload, prefetched read): "
              << output[2] << ' ' << output[3] << ' ' << output[4] << std::endl;
''')


def dcci() -> str:
    name = "asc_dcci"
    kernel = "AscDcciKernel"
    return header("cache_ctrl", name) + f'''
__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint64_t* input)
{{
    asc_init();
    __ubuf__ uint8_t observed[64];
    // Scalar stores reach DCache first and can remain dirty instead of reaching GM.
    output[0] = input[0] + 1;
    // Clean/invalidate the 64B cache line containing output[0], then wait for writeback.
    asc_dcci_single(output);
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);

    // MTE2 bypasses DCache, so this readback observes the value now visible in GM.
    asc_copy_gm2ub_align(observed, reinterpret_cast<__gm__ uint8_t*>(output), 64);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(reinterpret_cast<__gm__ uint8_t*>(output + 8), observed, 64);
    asc_sync_mte3(0);
}}
''' + host(
        "cache_ctrl", name, kernel, "    input[0] = 41;",
        "output[0] == 42 && output[8] == 42",
        '''    std::cout << "Scalar value after DCCI: " << output[0] << std::endl;
    std::cout << "MTE2 value read from GM: " << output[8] << std::endl;
''')


def dci() -> str:
    name = "asc_dci"
    kernel = "AscDciKernel"
    return header("cache_ctrl", name) + f'''
__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint64_t* input)
{{
    asc_init();
    // Wait for prior memory accesses before discarding every DCache line.
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    const uint64_t begin = asc_get_system_cycle();
    // DCI invalidates the whole DCache without writing dirty lines back to GM.
    asc_dci();
    const uint64_t dci_cycles = asc_get_system_cycle() - begin;
    output[0] = *reinterpret_cast<__gm__ volatile uint64_t*>(input);
    output[1] = dci_cycles;
    asc_dcci_single(output);
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
}}
''' + host(
        "cache_ctrl", name, kernel, "    input[0] = 1234;",
        "output[0] == input[0]",
        '''    std::cout << "Value read after DCache invalidation: " << output[0] << std::endl;
    std::cout << "DCI issue cycles: " << output[1] << std::endl;
''')


def icache(name: str, read_status_twice: bool) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    target_comment = "// Query whether the ICache preload engine is busy (1) or idle (0)." if read_status_twice else \
        "// Preload one 2KB instruction region starting at the current program counter."
    return header("cache_ctrl", name) + f'''
__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint64_t* input)
{{
    asc_init();
    const uint64_t pc = static_cast<uint64_t>(asc_get_program_counter()) & 0xFFFFFFFFFFFFULL;
    const uint64_t begin = asc_get_system_cycle();
    {target_comment if not read_status_twice else '// Issue a 2KB instruction prefetch; this does not preload scalar GM data.'}
    asc_icache_preload(reinterpret_cast<const void*>(pc), 1);
    {target_comment if read_status_twice else '// Read the debug status immediately after issuing the preload.'}
    const uint64_t immediate_status = asc_get_icache_preload_status();
    uint64_t final_status = immediate_status;
    uint64_t polls = 0;
    while (final_status != 0 && polls < 1024) {{
        final_status = asc_get_icache_preload_status();
        ++polls;
    }}
    const uint64_t preload_cycles = asc_get_system_cycle() - begin;
    output[0] = immediate_status;
    output[1] = final_status;
    output[2] = preload_cycles;
    output[3] = pc;
    output[4] = polls;
    // Flush Scalar result stores to GM; this DCache step is unrelated to ICache.
    asc_dcci_single(output);
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
}}
''' + host(
        "cache_ctrl", name, kernel, "    input[0] = 1;",
        "output[0] <= 1 && output[1] == 0 && output[2] > 0 && output[3] != 0",
        '''    std::cout << "ICache status (immediate, final): " << output[0] << ' ' << output[1] << std::endl;
    std::cout << "Preload/status cycles and polls: " << output[2] << ' ' << output[4] << std::endl;
''')


def init_example() -> str:
    name = "asc_init"
    kernel = "AscInitKernel"
    return header("misc", name) + f'''
__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint64_t* input)
{{
    asc_init();
    __ubuf__ uint32_t local[BYTES / sizeof(uint32_t)];
    asc_copy_gm2ub_align(local, reinterpret_cast<__gm__ uint32_t*>(input), BYTES);
    asc_sync_mte2(0);
    asc_set_atomic_add_int32();
    // Reinitialize the core and clear persistent state such as the atomic mode.
    asc_init();
    // asc_init resets the atomic mode, so this is an ordinary UB-to-GM copy.
    asc_copy_ub2gm(reinterpret_cast<__gm__ uint32_t*>(output), local, BYTES);
    asc_sync_mte3(0);
}}
''' + host(
        "misc", name, kernel,
        "    for (uint32_t i = 0; i < ELEMENTS; ++i) { input[i] = i + 1; output[i] = 10; }",
        "output == input",
        '    PrintData("Output after asc_init reset", output);\n    PrintData("Golden ordinary copy", input);\n')


def main() -> None:
    cache = ROOT / "cache_ctrl"
    misc = ROOT / "misc"
    cache.mkdir(parents=True, exist_ok=True)
    misc.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_datacache_preload": datacache_preload(),
        "asc_dcci": dcci(),
        "asc_dci": dci(),
        "asc_get_icache_preload_status": icache("asc_get_icache_preload_status", True),
        "asc_icache_preload": icache("asc_icache_preload", False),
    }
    for name, text in cases.items():
        (cache / f"{name}.asc").write_text(text, encoding="utf-8")
    (misc / "asc_init.asc").write_text(init_example(), encoding="utf-8")
    print(f"Generated {len(cases) + 1} functional cache and miscellaneous examples")


if __name__ == "__main__":
    main()
