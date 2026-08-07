#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "simd_atomic"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(name: str) -> str:
    rel = f"simd_atomic/{name}"
    binary = f"/tmp/c_api_simd_atomic_{name}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/{rel}.asc -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t ELEMENTS = 64;
constexpr uint32_t BYTES = ELEMENTS * sizeof(int32_t);

template <typename T>
void PrintData(const char* label, const std::vector<T>& data)
{{
    std::cout << label << ":";
    const size_t count = data.size() < 8 ? data.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +data[i];
    if (data.size() > count) std::cout << " ...";
    std::cout << std::endl;
}}
'''


def main_body(kernel: str, name: str, initial: str, golden_expr: str,
              extra_alloc: str = "", extra_arg: str = "", extra_copy: str = "",
              extra_print: str = "", extra_check: str = "true") -> str:
    return f'''
}} // namespace

int main()
{{
    std::vector<int32_t> input(ELEMENTS), output(ELEMENTS, {initial}), golden(ELEMENTS);
    for (uint32_t i = 0; i < ELEMENTS; ++i) {{
        input[i] = static_cast<int32_t>(i % 8 + 1);
        golden[i] = {golden_expr};
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    int32_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
{extra_alloc}    aclrtMemcpy(input_device, BYTES, input.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, BYTES, output.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input_device{extra_arg});
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
{extra_copy}    PrintData("Input", input);
    PrintData("Output", output);
    PrintData("Golden", golden);
{extra_print}    bool passed = output == golden && ({extra_check});
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def atomic_family(name: str, operation: str, initial: int, golden_expr: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    source = header(name) + f'''
__global__ __vector__ void {kernel}(__gm__ int32_t* output, __gm__ int32_t* input)
{{
    asc_init();
    __ubuf__ int32_t local[ELEMENTS];
    asc_copy_gm2ub_align_sync(local, input, BYTES);
    asc_set_atomic_{operation}_int32();
    asc_copy_ub2gm_sync(output, local, BYTES);
    asc_set_atomic_none();
}}
'''
    return source + main_body(kernel, name, str(initial), golden_expr)


def atomic_add_example() -> str:
    name = "asc_set_atomic_add"
    kernel = "AscSetAtomicAddKernel"
    return header(name) + f'''
__global__ __vector__ void {kernel}(__gm__ float* output, __gm__ float* input0, __gm__ float* input1)
{{
    asc_init();
    __ubuf__ float local0[ELEMENTS];
    __ubuf__ float local1[ELEMENTS];
    asc_copy_gm2ub_align_sync(local0, input0, BYTES);
    asc_copy_gm2ub_align_sync(local1, input1, BYTES);
    asc_copy_ub2gm(output, local0, BYTES);
    asc_sync_pipe(PIPE_MTE3);
    asc_set_atomic_add_float();
    asc_copy_ub2gm(output, local1, BYTES);
    asc_set_atomic_none();
}}

}} // namespace

int main()
{{
    std::vector<float> input0(ELEMENTS), input1(ELEMENTS), output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < ELEMENTS; ++i) {{
        input0[i] = static_cast<float>(i % 8 + 1);
        input1[i] = 2.0f;
        golden[i] = input0[i] + input1[i];
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    float *input0_device = nullptr, *input1_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input0_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&input1_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input0_device, BYTES, input0.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(input1_device, BYTES, input1.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input0_device, input1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input0", input0);
    PrintData("Input1", input1);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {name} passed." : "[Failed] {name} failed.") << std::endl;
    aclrtFree(input0_device);
    aclrtFree(input1_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def one_operation(name: str, setter: str, initial: int, golden_expr: str) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return header(name) + f'''
__global__ __vector__ void {kernel}(__gm__ int32_t* output, __gm__ int32_t* input)
{{
    asc_init();
    __ubuf__ int32_t local[ELEMENTS];
    asc_copy_gm2ub_align_sync(local, input, BYTES);
    {setter};
    asc_copy_ub2gm_sync(output, local, BYTES);
    asc_set_atomic_none();
}}
''' + main_body(kernel, name, str(initial), golden_expr)


def config_example(name: str, calls: str, expected_config: int) -> str:
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    source = header(name) + f'''
__global__ __vector__ void {kernel}(__gm__ int32_t* output, __gm__ int32_t* input,
    __gm__ uint64_t* raw_config)
{{
    asc_init();
{calls}
    asc_store_atomic_config config;
    asc_get_store_atomic_config(config);
    raw_config[0] = config.config;
    asc_set_atomic_none();
}}
'''
    return source + main_body(
        kernel, name, "10", "10",
        extra_alloc="    uint64_t* config_device = nullptr;\n    aclrtMalloc(reinterpret_cast<void**>(&config_device), sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST);\n",
        extra_arg=", config_device",
        extra_copy="    std::vector<uint64_t> raw_config(1);\n    aclrtMemcpy(raw_config.data(), sizeof(uint64_t), config_device, sizeof(uint64_t), ACL_MEMCPY_DEVICE_TO_HOST);\n",
        extra_print="    PrintData(\"Raw atomic config\", raw_config);\n    aclrtFree(config_device);\n",
        extra_check=f"raw_config[0] == {expected_config}")


def none_example() -> str:
    name = "asc_set_atomic_none"
    kernel = "AscSetAtomicNoneKernel"
    source = header(name) + f'''
__global__ __vector__ void {kernel}(__gm__ int32_t* output, __gm__ int32_t* input)
{{
    asc_init();
    __ubuf__ int32_t local[ELEMENTS];
    asc_copy_gm2ub_align_sync(local, input, BYTES);
    asc_set_atomic_add_int32();
    asc_copy_ub2gm_sync(output, local, BYTES);
    asc_set_atomic_none();
    asc_copy_ub2gm_sync(output, local, BYTES);
}}
'''
    return source + main_body(kernel, name, "10", "input[i]")


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    cases = {
        "asc_get_store_atomic_config": config_example(
            "asc_get_store_atomic_config", "    asc_set_store_atomic_config_v2(1, 2);", 33),
        "asc_set_atomic_add_int_deprecated": one_operation(
            "asc_set_atomic_add_int_deprecated", "asc_set_atomic_add_int()", 10, "10 + input[i]"),
        "asc_set_atomic_add": atomic_add_example(),
        "asc_set_atomic_max_int_deprecated": one_operation(
            "asc_set_atomic_max_int_deprecated", "asc_set_atomic_max_int()", 5, "input[i] > 5 ? input[i] : 5"),
        "asc_set_atomic_max": atomic_family(
            "asc_set_atomic_max", "max", 5, "input[i] > 5 ? input[i] : 5"),
        "asc_set_atomic_min_int_deprecated": one_operation(
            "asc_set_atomic_min_int_deprecated", "asc_set_atomic_min_int()", 5, "input[i] < 5 ? input[i] : 5"),
        "asc_set_atomic_min": atomic_family(
            "asc_set_atomic_min", "min", 5, "input[i] < 5 ? input[i] : 5"),
        "asc_set_atomic_none": none_example(),
        "asc_set_store_atomic_config_v2_deprecated": config_example(
            "asc_set_store_atomic_config_v2_deprecated",
            "    asc_set_store_atomic_config_v2(1, 2);", 33),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    unsupported = OUT / "asc_set_store_atomic_config_v1.asc"
    if unsupported.exists():
        unsupported.unlink()
    print(f"Generated {len(cases)} dav-3510 SIMD atomic examples in {OUT}")


if __name__ == "__main__":
    main()
