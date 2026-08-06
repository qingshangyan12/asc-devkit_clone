#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "vector_datamove"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def data_source(rel: str, body: str, APIs: str, arch: str, marker_only: bool = False) -> str:
    kernel = "".join(part.capitalize() for part in Path(rel).stem.split("_")) + "Kernel"
    golden = "    golden[0] = 1;" if marker_only else "    golden = input;"
    compare = "CApiExample::CompareRange(output, golden, 0, 1)" if marker_only else "CApiExample::Compare(output, golden)"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t BYTES = 256;

__global__ __vector__ void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input)
{{
    asc_init();
{body}
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(BYTES), output(BYTES, 0xff), golden(BYTES, 0);
    for (uint32_t i = 0; i < BYTES; ++i) input[i] = static_cast<uint8_t>(i);
{golden}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(BYTES), output_device(BYTES);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input) || !output_device.CopyFrom(golden)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = {compare};
    std::cout << (passed ? "[Success] {APIs} passed." : "[Failed] {APIs} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def marker_source(name: str, calls: str, APIs: str, arch: str = "dav-3510") -> str:
    return data_source(
        f"vector_datamove/{name}", f"{calls}\n    output[0] = 1;", APIs, arch, marker_only=True)


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    cases = {}
    for arch in ("2201", "3510"):
        path = f"vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_{arch}"
        cases[path] = data_source(
            path,
            '''    __ubuf__ uint8_t local[BYTES];
    asc_copy_gm2ub(local, input, BYTES);
    asc_sync_mte2(0);
    asc_copy_gm2ub_sync(local, input, BYTES);
    asc_copy_ub2gm_align(output, local, BYTES);''',
            "asc_copy_gm2ub/asc_copy_gm2ub_sync", f"dav-{arch}")

        path = f"vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_{arch}"
        cases[path] = data_source(
            path,
            '''    __ubuf__ uint8_t local[BYTES];
    asc_copy_gm2ub_align(local, input, BYTES);
    asc_sync_mte2(0);
    asc_copy_gm2ub_align_sync(local, input, BYTES);
    asc_copy_ub2gm_align(output, local, BYTES);''',
            "asc_copy_gm2ub_align/asc_copy_gm2ub_align_sync", f"dav-{arch}")

        path = f"vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_{arch}"
        cases[path] = data_source(
            path,
            '''    __ubuf__ uint8_t local[BYTES];
    asc_copy_gm2ub_align(local, input, BYTES);
    asc_sync_mte2(0);
    asc_copy_ub2gm(output, local, BYTES);
    asc_sync_mte3(0);
    asc_copy_ub2gm_sync(output, local, BYTES);''',
            "asc_copy_ub2gm/asc_copy_ub2gm_sync", f"dav-{arch}")

        path = f"vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_{arch}"
        cases[path] = data_source(
            path,
            '''    __ubuf__ uint8_t local[BYTES];
    asc_copy_gm2ub_align(local, input, BYTES);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, BYTES);
    asc_sync_mte3(0);
    asc_copy_ub2gm_align_sync(output, local, BYTES);''',
            "asc_copy_ub2gm_align/asc_copy_ub2gm_align_sync", f"dav-{arch}")

    cases["vector_datamove/asc_copy_ub2ub"] = data_source(
        "vector_datamove/asc_copy_ub2ub",
        '''    __ubuf__ uint8_t source[BYTES];
    __ubuf__ uint8_t destination[BYTES];
    asc_copy_gm2ub_align(source, input, BYTES);
    asc_sync_mte2(0);
    asc_copy_ub2ub(destination, source, BYTES);
    asc_sync();
    asc_copy_ub2ub_sync(destination, source, BYTES);
    asc_copy_ub2gm_align(output, destination, BYTES);''',
        "asc_copy_ub2ub/asc_copy_ub2ub_sync", "dav-3510")

    cases["vector_datamove/asc_copy_ub2l1"] = data_source(
        "vector_datamove/asc_copy_ub2l1",
        '''    __ubuf__ uint8_t source[BYTES];
    __cbuf__ uint8_t destination[BYTES];
    asc_copy_gm2ub_align(source, input, BYTES);
    asc_sync_mte2(0);
    asc_copy_ub2l1(destination, source, BYTES);
    asc_sync();
    asc_copy_ub2l1_sync(destination, source, BYTES);
    output[0] = 1;''',
        "asc_copy_ub2l1/asc_copy_ub2l1_sync", "dav-3510", marker_only=True)

    cases["vector_datamove/asc_ndim_copy_gm2ub"] = data_source(
        "vector_datamove/asc_ndim_copy_gm2ub",
        '''    __ubuf__ uint8_t local[BYTES];
    asc_set_ndim_loop0_stride(1, 1);
    asc_set_ndim_loop1_stride(0, 0);
    asc_set_ndim_loop2_stride(0, 0);
    asc_set_ndim_loop3_stride(0, 0);
    asc_set_ndim_loop4_stride(0, 0);
    asc_ndim_copy_gm2ub(local, input, BYTES, 1, 1, 1, 1, 0, 0, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
    asc_sync_mte2(0);
    asc_copy_ub2gm_align(output, local, BYTES);''',
        "asc_ndim_copy_gm2ub", "dav-3510")

    marker_cases = {
        "asc_set_copy_pad_val": (
            '''    asc_set_copy_pad_val(static_cast<int8_t>(0));
    asc_set_copy_pad_val(static_cast<uint8_t>(0));
    asc_set_copy_pad_val(static_cast<int16_t>(0));
    asc_set_copy_pad_val(static_cast<uint16_t>(0));
    asc_set_copy_pad_val(static_cast<int32_t>(0));
    asc_set_copy_pad_val(static_cast<uint32_t>(0));
    asc_set_copy_pad_val(0.0f);''',
            "asc_set_copy_pad_val", "dav-3510"),
        "asc_set_gm2ub_loop1_stride": ("    asc_set_gm2ub_loop1_stride(1, 1);", "asc_set_gm2ub_loop1_stride", "dav-3510"),
        "asc_set_gm2ub_loop2_stride": ("    asc_set_gm2ub_loop2_stride(1, 1);", "asc_set_gm2ub_loop2_stride", "dav-3510"),
        "asc_set_gm2ub_loop_size": ("    asc_set_gm2ub_loop_size(1, 1);", "asc_set_gm2ub_loop_size", "dav-3510"),
        "asc_set_gm2ub_pad_deprecated": ("    asc_set_gm2ub_pad(0);", "asc_set_gm2ub_pad", "dav-3510"),
        "asc_set_ndim_loop_stride": (
            '''    asc_set_ndim_loop0_stride(1, 1);
    asc_set_ndim_loop1_stride(1, 1);
    asc_set_ndim_loop2_stride(1, 1);
    asc_set_ndim_loop3_stride(1, 1);
    asc_set_ndim_loop4_stride(1, 1);''',
            "asc_set_ndim_loop0_stride/asc_set_ndim_loop1_stride/asc_set_ndim_loop2_stride/asc_set_ndim_loop3_stride/asc_set_ndim_loop4_stride", "dav-3510"),
        "asc_set_ndim_pad_count": (
            "    asc_ndim_pad_count_config config;\n    config.config = 0;\n    asc_set_ndim_pad_count(config);",
            "asc_set_ndim_pad_count", "dav-3510"),
        "asc_set_ndim_pad_value": (
            '''    asc_set_ndim_pad_value(static_cast<int8_t>(0));
    asc_set_ndim_pad_value(static_cast<uint8_t>(0));
    asc_set_ndim_pad_value(static_cast<int16_t>(0));
    asc_set_ndim_pad_value(static_cast<uint16_t>(0));
    asc_set_ndim_pad_value(static_cast<int32_t>(0));
    asc_set_ndim_pad_value(static_cast<uint32_t>(0));
    asc_set_ndim_pad_value(0.0f);''',
            "asc_set_ndim_pad_value", "dav-3510"),
        "asc_set_ub2gm_loop1_stride": ("    asc_set_ub2gm_loop1_stride(1, 1);", "asc_set_ub2gm_loop1_stride", "dav-3510"),
        "asc_set_ub2gm_loop2_stride": ("    asc_set_ub2gm_loop2_stride(1, 1);", "asc_set_ub2gm_loop2_stride", "dav-3510"),
        "asc_set_ub2gm_loop_size": ("    asc_set_ub2gm_loop_size(1, 1);", "asc_set_ub2gm_loop_size", "dav-3510"),
    }
    for name, (calls, APIs, arch) in marker_cases.items():
        cases[f"vector_datamove/{name}"] = marker_source(name, calls, APIs, arch)

    for rel, text in cases.items():
        path = ROOT / f"{rel}.asc"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} vector data movement examples in {OUT}")


if __name__ == "__main__":
    main()
