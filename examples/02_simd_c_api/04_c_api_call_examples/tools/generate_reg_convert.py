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


def source(file_name: str, calls) -> str:
    rel = f"reg/data_type_convert/{file_name}"
    kernel = "".join(x.capitalize() for x in file_name.split("_")) + "Kernel"
    statements = []
    for index, (api, dst_type, src_type) in enumerate(calls):
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
    std::vector<uint8_t> input(BUFFER_BYTES, 0);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES, 0);
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(BUFFER_BYTES), output_device(BUFFER_BYTES);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input bytes", input);
    CApiExample::Print("Output bytes", output);
    CApiExample::Print("Golden bytes", golden);
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
