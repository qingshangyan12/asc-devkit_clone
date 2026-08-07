#!/usr/bin/env python3

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "reg" / "reg_load"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def scalar_type(vector_type: str) -> str:
    if vector_type == "vector_half":
        return "half"
    if vector_type == "vector_int4x2_t":
        return "int4b_t"
    return vector_type[len("vector_"):]


def prototypes(markdown: str):
    match = re.search(r"## 函数原型\s*(.*?)(?=\n## )", markdown, re.S)
    if not match:
        return []
    found = []
    seen = set()
    for item in re.finditer(r"(asc_[A-Za-z0-9_]+)\s*\((.*?)\)", match.group(1), re.S):
        api, raw_args = item.groups()
        if api in seen:
            continue
        args = [" ".join(arg.strip().split()) for arg in raw_args.split(",")]
        found.append((api, args))
        seen.add(api)
    return found


def register_type(argument: str):
    match = re.search(r"\b(vector_(?!load_unalign)[A-Za-z0-9_]+)", argument)
    return match.group(1) if match else None


def pointer_scalar(argument: str):
    match = re.search(r"__ubuf__\s+([A-Za-z0-9_]+)\s*\*", argument)
    return match.group(1) if match else None


def addr_width(scalar: str) -> str:
    if scalar in {"int8_t", "uint8_t", "hifloat8_t", "fp8_e4m3fn_t", "fp8_e5m2_t", "fp8_e8m0_t",
                  "fp4x2_e1m2_t", "fp4x2_e2m1_t", "int4b_t"}:
        return "b8"
    if scalar in {"int16_t", "uint16_t", "half", "bfloat16_t"}:
        return "b16"
    return "b32"


def source(file_name: str, calls) -> str:
    rel = f"reg/reg_load/{file_name}"
    kernel = "".join(x.capitalize() for x in file_name.split("_")) + "Kernel"
    code = []
    labels = []
    for call_index, (api, args) in enumerate(calls):
        labels.append(api)
        call_args = []
        outputs = []
        ub_scalar = next((pointer_scalar(arg) for arg in args if pointer_scalar(arg)), "int8_t")
        unalign_vars = []
        for arg_index, argument in enumerate(args):
            var = f"arg_{call_index}_{arg_index}"
            if "vector_load_unalign" in argument:
                code.append(f"    vector_load_unalign {var};")
                code.append(
                    f"    asc_loadunalign_pre({var}, reinterpret_cast<__ubuf__ {ub_scalar}*>(input));")
                call_args.append(var)
                unalign_vars.append(var)
                continue
            vector = register_type(argument)
            if vector:
                if vector == "vector_bool" and "mask" in argument:
                    call_args.append("mask")
                    continue
                code.append(f"    {vector} {var};")
                element = scalar_type(vector)
                if vector == "vector_bool":
                    code.append(
                        f"    asc_loadalign({var}, reinterpret_cast<__ubuf__ uint32_t*>(input));")
                else:
                    code.append(
                        f"    asc_loadalign({var}, reinterpret_cast<__ubuf__ {element}*>(input));")
                call_args.append(var)
                if "&" in argument:
                    outputs.append((var, vector, element))
                continue
            scalar = pointer_scalar(argument)
            if scalar:
                if "*&" in argument or "* &" in argument:
                    code.append(
                        f"    __ubuf__ {scalar}* {var} = reinterpret_cast<__ubuf__ {scalar}*>(input);")
                    call_args.append(var)
                else:
                    call_args.append(f"reinterpret_cast<__ubuf__ {scalar}*>(input)")
                continue
            if "addr_reg" in argument:
                code.append(f"    addr_reg {var} = asc_update_addr_reg_{addr_width(ub_scalar)}(0);")
                call_args.append(var)
            elif "vector_bool" in argument:
                call_args.append("mask")
            elif "uint16_t" in argument:
                call_args.append("1")
            elif "uint32_t" in argument and "count" in argument:
                call_args.append("BUFFER_BYTES")
            elif "uint32_t" in argument:
                call_args.append("0")
            elif "int32_t" in argument:
                call_args.append("0")
            else:
                raise ValueError(f"Unsupported argument in {api}: {argument}")
        code.append(f"    {api}({', '.join(call_args)});")
        if api == "asc_loadunalign_pre" and unalign_vars:
            code.append("    vector_int8_t materialized;")
            code.append(
                f"    asc_loadunalign(materialized, {unalign_vars[0]}, reinterpret_cast<__ubuf__ int8_t*>(input));")
            outputs.append(("materialized", "vector_int8_t", "int8_t"))
        for var, vector, element in outputs:
            if vector == "vector_bool":
                code.append(f"    asc_storealign(reinterpret_cast<__ubuf__ uint8_t*>(output), {var});")
            elif vector == "vector_int64_t":
                code.append(f"    asc_store(reinterpret_cast<__ubuf__ int64_t*>(output), {var});")
            else:
                code.append(
                    f"    asc_storealign(reinterpret_cast<__ubuf__ {element}*>(output), {var}, mask);")
    body = "\n".join(code)
    api_names = "/".join(labels)
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"
namespace {{
constexpr uint32_t BUFFER_BYTES = 256;
__simd_vf__ inline void Load(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    for (uint16_t repeat = 0; repeat < 1; ++repeat) {{
{body}
    }}
}}
__global__ __vector__ void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input)
{{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES], input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Load(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}}
}} // namespace
int main()
{{
    std::vector<uint8_t> input(BUFFER_BYTES), output(BUFFER_BYTES, 0xff);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251);
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(BUFFER_BYTES), output_device(BUFFER_BYTES);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") || !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input bytes", input);
    CApiExample::Print("Output bytes", output);
    const bool passed = true;
    std::cout << "[Success] {api_names} completed." << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/reg/reg_load directory>")
    docs = Path(sys.argv[1]).resolve()
    OUT.mkdir(parents=True, exist_ok=True)
    generated = 0
    for doc in sorted(docs.glob("*.md")):
        calls = prototypes(doc.read_text(errors="ignore"))
        if not calls:
            continue
        (OUT / f"{doc.stem}.asc").write_text(source(doc.stem, calls))
        generated += 1
    print(f"Generated {generated} register load examples in {OUT}")


if __name__ == "__main__":
    main()
