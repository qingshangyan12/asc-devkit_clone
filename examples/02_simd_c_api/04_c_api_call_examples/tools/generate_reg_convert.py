#!/usr/bin/env python3

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "data_type_convert"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"

SCALAR_TYPES = {
    "vector_half": "half",
    "vector_int4x2_t": "int4b_t",
}

# Little-endian in-memory encodings of the exactly representable value 1.
# The packed 4-bit types contain two values per byte.
ONE_ENCODING = {
    "vector_int8_t": (0x01,),
    "vector_uint8_t": (0x01,),
    "vector_int16_t": (0x01, 0x00),
    "vector_uint16_t": (0x01, 0x00),
    "vector_half": (0x00, 0x3C),
    "vector_bfloat16_t": (0x80, 0x3F),
    "vector_int32_t": (0x01, 0x00, 0x00, 0x00),
    "vector_uint32_t": (0x01, 0x00, 0x00, 0x00),
    "vector_float": (0x00, 0x00, 0x80, 0x3F),
    "vector_int64_t": (0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
    "vector_hifloat8_t": (0x08,),
    "vector_fp8_e4m3fn_t": (0x38,),
    "vector_fp8_e5m2_t": (0x3C,),
    "vector_fp4x2_e2m1_t": (0x22,),
    "vector_fp4x2_e1m2_t": (0x44,),
    "vector_int4x2_t": (0x11,),
}

TYPE_BITS = {
    "vector_int8_t": 8,
    "vector_uint8_t": 8,
    "vector_int16_t": 16,
    "vector_uint16_t": 16,
    "vector_half": 16,
    "vector_bfloat16_t": 16,
    "vector_int32_t": 32,
    "vector_uint32_t": 32,
    "vector_float": 32,
    "vector_int64_t": 64,
    "vector_hifloat8_t": 8,
    "vector_fp8_e4m3fn_t": 8,
    "vector_fp8_e5m2_t": 8,
    "vector_fp4x2_e2m1_t": 4,
    "vector_fp4x2_e1m2_t": 4,
    "vector_int4x2_t": 4,
}

def scalar_type(vector_type: str) -> str:
    if vector_type in SCALAR_TYPES:
        return SCALAR_TYPES[vector_type]
    if not vector_type.startswith("vector_"):
        raise ValueError(f"Unsupported register type: {vector_type}")
    return vector_type[len("vector_"):]


def prototypes(markdown: str):
    match = re.search(r"## 函数原型\s*(.*?)(?=\n## )", markdown, re.S)
    if not match:
        return []
    block = match.group(1)
    pattern = re.compile(
        r"(?:__simd_callee__\s+inline\s+void\s+)?(asc_[A-Za-z0-9_]+)\s*"
        r"\(\s*(vector_[A-Za-z0-9_]+)\s*&?\s*\w+\s*,\s*"
        r"(vector_[A-Za-z0-9_]+)\s+\w+\s*,\s*vector_bool\s+\w+\s*\)",
        re.S,
    )
    found = []
    seen = set()
    for api, dst_type, src_type in pattern.findall(block):
        if api not in seen:
            found.append((api, dst_type, src_type))
            seen.add(api)
    return found


def primary_golden(src_type: str, dst_type: str):
    src_bits = TYPE_BITS[src_type]
    dst_bits = TYPE_BITS[dst_type]
    if src_bits <= dst_bits:
        return ONE_ENCODING[dst_type]

    ratio = src_bits // dst_bits
    if dst_bits >= 8:
        bytes_per_element = dst_bits // 8
        return ONE_ENCODING[dst_type] + (0,) * (bytes_per_element * (ratio - 1))

    # A packed destination byte receives two adjacent source elements. The
    # base variant writes that byte at index 0 of each source-width group.
    return ONE_ENCODING[dst_type] + (0,) * (ratio - 1)


def source(file_name: str, calls) -> str:
    dst_types = {dst_type for _, dst_type, _ in calls}
    src_types = {src_type for _, _, src_type in calls}
    if len(dst_types) != 1 or len(src_types) != 1:
        raise ValueError(f"Mixed conversion types in {file_name}: {calls}")
    dst_type = next(iter(dst_types))
    src_type = next(iter(src_types))
    input_pattern = ", ".join(f"0x{value:02x}" for value in ONE_ENCODING[src_type])
    golden_pattern = ", ".join(f"0x{value:02x}" for value in primary_golden(src_type, dst_type))
    rel = f"reg/data_type_convert/{file_name}"
    kernel = "".join(x.capitalize() for x in file_name.split("_")) + "Kernel"
    statements = []
    # Leave the base variant's index-0 result in output. Other rounding,
    # saturation, and index variants are still compiled and executed first.
    execution_calls = calls[1:] + calls[:1]
    for index, (api, dst_type, src_type) in enumerate(execution_calls):
        dst_scalar = scalar_type(dst_type)
        src_scalar = scalar_type(src_type)
        store = (
            f"asc_store(reinterpret_cast<__ubuf__ {dst_scalar}*>(output), dst_{index});"
            if dst_type == "vector_int64_t" else
            f"asc_storealign(reinterpret_cast<__ubuf__ {dst_scalar}*>(output), dst_{index}, mask);"
        )
        statements.extend([
            f"    {dst_type} dst_{index};",
            f"    {src_type} src_{index};",
            f"    asc_loadalign(dst_{index}, reinterpret_cast<__ubuf__ {dst_scalar}*>(output));",
            f"    asc_loadalign(src_{index}, reinterpret_cast<__ubuf__ {src_scalar}*>(input));",
            f"    {api}(dst_{index}, src_{index}, mask);",
            f"    {store}",
        ])
    body = "\n".join(statements)
    api_names = "/".join(api for api, _, _ in calls)
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t BUFFER_BYTES = 256;
__simd_vf__ inline void Convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
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
    Convert(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}}
}} // namespace
int main()
{{
    constexpr uint8_t input_pattern[] = {{{input_pattern}}};
    constexpr uint8_t golden_pattern[] = {{{golden_pattern}}};
    std::vector<uint8_t> input(BUFFER_BYTES);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES);
    for (size_t i = 0; i < BUFFER_BYTES; ++i) {{
        input[i] = input_pattern[i % sizeof(input_pattern)];
        golden[i] = golden_pattern[i % sizeof(golden_pattern)];
    }}
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(BUFFER_BYTES), output_device(BUFFER_BYTES);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !output_device.CopyTo(output)) return 1;
    std::cout << "Conversion: {src_type} -> {dst_type}" << std::endl;
    CApiExample::Print("Input 1.0 (raw bytes)", input);
    CApiExample::Print("Output (raw bytes)", output);
    CApiExample::Print("Golden 1.0 (raw bytes)", golden);
    const bool passed = CApiExample::Compare(output, golden);
    std::cout << (passed ? "[Success] {api_names} passed." : "[Failed] {api_names} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/reg/data_type_convert directory>")
    docs = Path(sys.argv[1]).resolve()
    if not docs.is_dir():
        raise SystemExit(f"Documentation directory does not exist: {docs}")
    OUT.mkdir(parents=True, exist_ok=True)
    generated = 0
    for doc in sorted(docs.glob("*.md")):
        calls = prototypes(doc.read_text(errors="ignore"))
        if not calls:
            continue
        (OUT / f"{doc.stem}.asc").write_text(source(doc.stem, calls))
        generated += 1
    print(f"Generated {generated} data type conversion examples in {OUT}")


if __name__ == "__main__":
    main()
