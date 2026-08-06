#!/usr/bin/env python3

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "vector_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def architecture(markdown: str) -> str:
    line = next((line for line in markdown.splitlines() if "Ascend 950PR/Ascend 950DT" in line), "")
    return "dav-3510" if "：支持" in line else "dav-2201"


def prototypes(markdown: str):
    section = re.search(r"## 函数原型\s*(.*?)(?=\n## )", markdown, re.S)
    if not section:
        return []
    found = []
    seen = set()
    for match in re.finditer(r"\b(asc_[A-Za-z0-9_]+)\s*\((.*?)\)", section.group(1), re.S):
        api, raw_args = match.groups()
        args = [" ".join(arg.strip().split()) for arg in raw_args.split(",") if arg.strip()]
        if api in seen:
            if api == "asc_set_deq_scale" and len(args) == 3 and all("__ubuf__" not in arg for arg in args):
                found = [(name, args if name == api else existing) for name, existing in found]
            continue
        found.append((api, args))
        seen.add(api)
    return found


def variable_name(argument: str, fallback: str) -> str:
    clean = re.sub(r"\[[^]]*\]", "", argument).strip()
    match = re.search(r"([A-Za-z_][A-Za-z0-9_]*)\s*$", clean)
    return match.group(1) if match else fallback


def pointer_offset(name: str) -> int:
    if "dst" in name or "output" in name:
        return 0
    if "offset" in name or "index" in name:
        return 3072
    if "src1" in name or name.endswith("1"):
        return 2048
    return 1024


def argument_value(argument: str, index: int, declarations, unsupported):
    name = variable_name(argument, f"arg_{index}")
    pointer_array = re.search(
        r"__ubuf__\s+([A-Za-z0-9_:]+)\s*\*(?:\s*\*|\s*[A-Za-z_][A-Za-z0-9_]*\s*\[)", argument)
    if pointer_array:
        scalar = pointer_array.group(1)
        var = f"address_list_{index}"
        offset = pointer_offset(name)
        declarations.append(f"    __ubuf__ {scalar}* {var}[16];")
        declarations.append(
            f"    for (uint32_t i = 0; i < 16; ++i) {var}[i] = "
            f"reinterpret_cast<__ubuf__ {scalar}*>(workspace + {offset} + i * 32);")
        return var
    scalar_array = re.search(
        r"\b(half|bfloat16_t|float|double|u?int(?:8|16|32|64)_t|bool)\s+"
        r"[A-Za-z_][A-Za-z0-9_]*\s*\[",
        argument)
    if scalar_array:
        scalar = scalar_array.group(1)
        var = f"value_array_{index}_{name}"
        declarations.append(f"    {scalar} {var}[16] = {{}};")
        return var
    pointer = re.search(r"__ubuf__\s+([A-Za-z0-9_:]+)\s*\*", argument)
    if pointer:
        scalar = pointer.group(1)
        offset = pointer_offset(name)
        return f"reinterpret_cast<__ubuf__ {scalar}*>(workspace + {offset})"
    gm_pointer = re.search(r"__gm__\s+([A-Za-z0-9_:]+)\s*\*", argument)
    if gm_pointer:
        scalar = gm_pointer.group(1)
        return f"reinterpret_cast<__gm__ {scalar}*>(output)"
    reference = re.search(r"\b(half|float|double|u?int(?:8|16|32|64)_t|int(?:8|16|32|64)_t)\s*&", argument)
    if reference:
        kind = reference.group(1)
        var = f"ref_{index}_{name}"
        value = "0.0f" if kind in {"float", "double"} else ("static_cast<half>(0)" if kind == "half" else "0")
        declarations.append(f"    {kind} {var} = {value};")
        return var
    config_ref = re.search(r"\b(asc_[A-Za-z0-9_]+)\s*&", argument)
    if config_ref:
        kind = config_ref.group(1)
        var = f"config_{index}_{name}"
        declarations.append(f"    {kind} {var}{{}};")
        return var
    if re.search(r"\bbool\b", argument):
        return "false"
    if re.search(r"\bhalf\b", argument) and "*" not in argument:
        return "static_cast<half>(1.0f)"
    if "bfloat16_t" in argument and "*" not in argument:
        return "static_cast<bfloat16_t>(1.0f)"
    if re.search(r"\bfloat\b", argument):
        return "1.0f"
    if re.search(r"\bdouble\b", argument):
        return "1.0"
    if "ub_addr8_t" in argument:
        return "VA2" if "src" in name else "VA0"
    if re.search(r"\b(u?int|int)(8|16|32|64)_t\b", argument):
        if any(word in name for word in ("count", "length", "size")):
            return "16"
        if "repeat" in name:
            return "1"
        if "stride" in name:
            return "1"
        if "mode" in name or "pattern" in name:
            return "0"
        return "1"
    if re.search(r"\b(asc_[A-Za-z0-9_]+|cmp_mode_t|round_mode_t)\b", argument):
        kind_match = re.search(r"\b(asc_[A-Za-z0-9_]+|cmp_mode_t|round_mode_t)\b", argument)
        kind = kind_match.group(1)
        return f"static_cast<{kind}>(0)"
    unsupported.append(argument)
    return "0"


def calls_for(items):
    lines = []
    unsupported = []
    for call_index, (api, args) in enumerate(items):
        declarations = []
        values = [argument_value(arg, call_index * 20 + i, declarations, unsupported) for i, arg in enumerate(args)]
        lines.extend(declarations)
        lines.append(f"    {api}({', '.join(values)});")
    return "\n".join(lines), unsupported


def source(name: str, body: str, APIs: str, arch: str) -> str:
    rel = f"vector_compute/{name}"
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t WORKSPACE_BYTES = 4096;
constexpr uint32_t OUTPUT_ELEMENTS = 8;

__global__ __vector__ void {kernel}(__gm__ uint64_t* output, __gm__ uint8_t* input)
{{
    asc_init();
    __ubuf__ uint8_t workspace[WORKSPACE_BYTES];
    asc_copy_gm2ub_align(workspace, input, WORKSPACE_BYTES);
    asc_sync_mte2(0);
{body}
    asc_sync();
    output[0] = 1;
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(WORKSPACE_BYTES, 1);
    std::vector<uint64_t> output(OUTPUT_ELEMENTS, ~0ULL), golden(OUTPUT_ELEMENTS, 0);
    golden[0] = 1;
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(WORKSPACE_BYTES);
    CApiExample::DeviceBuffer<uint64_t> output_device(OUTPUT_ELEMENTS);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input) || !output_device.CopyFrom(golden)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input bytes", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::CompareRange(output, golden, 0, 1);
    std::cout << (passed ? "[Success] {APIs} passed." : "[Failed] {APIs} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def generate(doc_dir: Path) -> None:
    pending = []
    rendered = []
    for path in sorted(doc_dir.glob("*.md")):
        markdown = path.read_text(encoding="utf-8")
        items = prototypes(markdown)
        if not items:
            continue
        body, unsupported = calls_for(items)
        if path.stem == "asc_transto5hd":
            body = '''    __ubuf__ uint8_t* destination_list[16];
    __ubuf__ uint8_t* source_list[16];
    for (uint32_t i = 0; i < 16; ++i) {
        destination_list[i] = reinterpret_cast<__ubuf__ uint8_t*>(workspace + i * 32);
        source_list[i] = reinterpret_cast<__ubuf__ uint8_t*>(workspace + 1024 + i * 32);
    }
    asc_set_va_reg(VA0, destination_list);
    asc_set_va_reg(VA2, source_list);
''' + body
        if unsupported:
            pending.append((path.name, sorted(set(unsupported))))
            continue
        APIs = "/".join(api for api, _ in items)
        rendered.append((path.stem, source(path.stem, body, APIs, architecture(markdown))))
    if pending:
        for name, arguments in pending:
            print(f"UNSUPPORTED {name}")
            for argument in arguments:
                print(f"  {argument}")
        raise SystemExit(f"Unsupported parameters in {len(pending)} documents")
    OUT.mkdir(parents=True, exist_ok=True)
    for name, text in rendered:
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(rendered)} vector compute examples in {OUT}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/vector_compute directory>")
    generate(Path(sys.argv[1]).resolve())
