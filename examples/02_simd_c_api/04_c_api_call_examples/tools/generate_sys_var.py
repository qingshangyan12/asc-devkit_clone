#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "sys_var"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(name: str) -> str:
    rel = f"sys_var/{name}"
    binary = f"/tmp/c_api_sys_var_{name}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/{rel}.asc -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iomanip>
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
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}}

void PrintHex(const char* label, const std::vector<uint64_t>& data)
{{
    std::cout << label << ":" << std::hex;
    for (uint32_t i = 0; i < 4; ++i) std::cout << " 0x" << data[i];
    std::cout << std::dec << std::endl;
}}
'''


def host(name: str, kernel: str, validation: str, blocks: int = 1,
         input_decl: str = "", kernel_arg: str = "", input_setup: str = "",
         input_alloc: str = "", input_free: str = "") -> str:
    return f'''
}} // namespace

int main()
{{
    std::vector<uint64_t> output(ELEMENTS, ~0ULL);
{input_decl}{input_setup}    aclInit(nullptr);
    aclrtSetDevice(0);
    uint64_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(output_device, BYTES, output.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
{input_alloc}    {kernel}<<<{blocks}, 0>>>(output_device{kernel_arg});
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Observed", output);
    PrintHex("Observed hex", output);
    const bool passed = {validation};
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
{input_free}    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def ordinary(name: str, body: str, validation: str, blocks: int = 1) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return header(name) + f'''
__global__ __vector__ void {kernel}(__gm__ uint64_t* output)
{{
    asc_init();
{body}
}}
''' + host(name, kernel, validation, blocks)


def stable_getter(name: str, api: str) -> str:
    return ordinary(name, f'''    output[0] = static_cast<uint64_t>({api}());
    output[1] = static_cast<uint64_t>({api}());
    asc_dcci_single(output);''', "output[0] == output[1]")


def arch_version() -> str:
    return ordinary("asc_get_arch_ver", '''    uint32_t version = 0xffffffffU;
    asc_get_arch_ver(version);
    output[0] = version;
    asc_dcci_single(output);''', "output[0] != 0xffffffffULL")


def block_value(name: str, expression: str, validation: str) -> str:
    return ordinary(name, f'''    const uint64_t index = static_cast<uint64_t>(asc_get_block_idx());
    output[index * 8] = static_cast<uint64_t>({expression});
    asc_dcci_single(output + index * 8);''', validation, 2)


def ctrl(name: str, bit: int) -> str:
    return ordinary(name, f'''    const uint64_t before = static_cast<uint64_t>(asc_get_ctrl());
    const uint64_t changed = before ^ (1ULL << {bit});
    asc_set_ctrl(changed);
    const uint64_t after = static_cast<uint64_t>(asc_get_ctrl());
    asc_set_ctrl(before);
    const uint64_t restored = static_cast<uint64_t>(asc_get_ctrl());
    output[0] = before;
    output[1] = after;
    output[2] = restored;
    asc_dcci_single(output);''',
        f"((output[0] ^ output[1]) & (1ULL << {bit})) != 0 && output[2] == output[0]")


def phy_buf_addr() -> str:
    return ordinary("asc_get_phy_buf_addr", '''    const uint64_t base = asc_get_phy_buf_addr(0);
    const uint64_t offset = asc_get_phy_buf_addr(32);
    output[0] = base;
    output[1] = offset;
    output[2] = offset - base;
    asc_dcci_single(output);''', "output[2] == 32")


def program_counter() -> str:
    return ordinary("asc_get_program_counter", '''    output[0] = static_cast<uint64_t>(asc_get_program_counter());
    output[1] = static_cast<uint64_t>(asc_get_program_counter());
    asc_dcci_single(output);''', "output[0] != 0 && output[1] != 0 && output[0] != output[1]")


def sub_block(name: str, target: str) -> str:
    return ordinary(name, f'''    const uint64_t sub_id = static_cast<uint64_t>(asc_get_sub_block_id());
    const uint64_t sub_num = static_cast<uint64_t>(asc_get_sub_block_num());
    output[0] = {target};
    output[1] = sub_id;
    output[2] = sub_num;
    asc_dcci_single(output);''', "output[2] > 0 && output[1] < output[2]")


def system_cycle() -> str:
    return ordinary("asc_get_system_cycle", '''    const uint64_t before = static_cast<uint64_t>(asc_get_system_cycle());
    for (volatile uint32_t i = 0; i < 256; ++i) {}
    const uint64_t after = static_cast<uint64_t>(asc_get_system_cycle());
    output[0] = before;
    output[1] = after;
    output[2] = after - before;
    asc_dcci_single(output);''', "output[1] > output[0] && output[2] > 0")


def vf_len() -> str:
    return ordinary("asc_get_vf_len", '''    constexpr uint64_t vf_bytes = asc_get_vf_len();
    output[0] = vf_bytes;
    output[1] = vf_bytes / sizeof(float);
    asc_dcci_single(output);''', "output[0] == 256 && output[1] == 64")


def squeeze_status(name: str, getter: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return header(name) + f'''
__simd_vf__ inline void MakeSqueezeStatus(__ubuf__ uint16_t* input, __ubuf__ uint16_t* packed)
{{
    vector_uint16_t src, squeezed;
    vector_store_unalign unalign;
    __ubuf__ uint16_t* destination = packed;
    asc_loadalign(src, input);
    asc_clear_ar_spr();
    asc_squeeze_with_status(squeezed, src, asc_create_mask_b16(PAT_M4));
    asc_storeunalign_postupdate(destination, unalign, squeezed);
    asc_storeunalign_post(destination, unalign);
}}

__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint16_t* input)
{{
    asc_init();
    __ubuf__ uint16_t local[128];
    __ubuf__ uint16_t packed[32];
    asc_copy_gm2ub_align(local, input, 128 * sizeof(uint16_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID1);
    asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID1);
    MakeSqueezeStatus(local, packed);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID1);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID1);
    output[0] = static_cast<uint64_t>({getter}());
    asc_dcci_single(output);
}}
''' + host(
        name, kernel, "output[0] == 64", input_decl="    std::vector<uint16_t> input(128);\n",
        kernel_arg=", input_device",
        input_setup="    for (uint32_t i = 0; i < input.size(); ++i) input[i] = i + 1;\n",
        input_alloc='''    uint16_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), input.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint16_t), input.data(), input.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
''', input_free="    aclrtFree(input_device);\n")


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_get_arch_ver": arch_version(),
        "asc_get_ar_spr_deprecated": squeeze_status("asc_get_ar_spr_deprecated", "asc_get_ar_spr"),
        "asc_get_block_idx": block_value("asc_get_block_idx", "index", "output[0] == 0 && output[8] == 1"),
        "asc_get_block_num": block_value("asc_get_block_num", "asc_get_block_num()", "output[0] == 2 && output[8] == 2"),
        "asc_get_core_id": block_value("asc_get_core_id", "asc_get_core_id()", "output[0] != ~0ULL && output[8] != ~0ULL"),
        "asc_get_ctrl": ctrl("asc_get_ctrl", 53),
        "asc_get_phy_buf_addr": phy_buf_addr(),
        "asc_get_phy_stack_base": stable_getter("asc_get_phy_stack_base", "asc_get_phy_stack_base"),
        "asc_get_program_counter": program_counter(),
        "asc_get_smmu_tag_version": stable_getter("asc_get_smmu_tag_version", "asc_get_smmu_tag_version"),
        "asc_get_squeeze_status": squeeze_status("asc_get_squeeze_status", "asc_get_squeeze_status"),
        "asc_get_status": stable_getter("asc_get_status", "asc_get_status"),
        "asc_get_sub_block_id": sub_block("asc_get_sub_block_id", "sub_id"),
        "asc_get_sub_block_num": sub_block("asc_get_sub_block_num", "sub_num"),
        "asc_get_system_cycle": system_cycle(),
        "asc_get_sys_virtual_base": stable_getter("asc_get_sys_virtual_base", "asc_get_sys_virtual_base"),
        "asc_get_vf_len": vf_len(),
        "asc_set_ctrl": ctrl("asc_set_ctrl", 56),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    for unsupported in ("asc_get_ffts_base_addr.asc", "asc_set_ffts_base_addr.asc"):
        path = OUT / unsupported
        if path.exists():
            path.unlink()
    print(f"Generated {len(cases)} observable system variable examples; preserved 6 cube setter examples")


if __name__ == "__main__":
    main()
