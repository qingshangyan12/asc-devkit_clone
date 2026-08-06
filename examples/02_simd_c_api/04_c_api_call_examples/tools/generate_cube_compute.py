#!/usr/bin/env python3

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "cube_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"
BUFFER_BYTES = 4096


def architecture(path: Path, markdown: str) -> str:
    if path.stem in {"asc_mmad", "asc_mmad_sparse"}:
        return "dav-2201"
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
        if api in seen:
            continue
        args = [" ".join(arg.strip().split()) for arg in raw_args.split(",") if arg.strip()]
        found.append((api, args))
        seen.add(api)
    return found


def variable_name(argument: str, fallback: str) -> str:
    match = re.search(r"([A-Za-z_][A-Za-z0-9_]*)\s*$", argument.strip())
    return match.group(1) if match else fallback


def argument_value(argument: str, index: int, declarations, buffers, unsupported):
    name = variable_name(argument, f"arg_{index}")
    pointer = re.search(r"(__ca__|__cb__|__cc__|__cbuf__|__fbuf__)\s+([A-Za-z0-9_:]+)\s*\*", argument)
    if pointer:
        domain, scalar = pointer.groups()
        buffer_names = {
            "__ca__": "a_l0_bytes",
            "__cb__": "b_l0_bytes",
            "__cc__": "c_l0_bytes",
            "__cbuf__": "l1_bytes",
            "__fbuf__": "fixpipe_bytes",
        }
        buffer_name = buffer_names[domain]
        buffers[buffer_name] = domain
        return f"reinterpret_cast<{domain} {scalar}*>({buffer_name})"
    reference = re.search(r"\b(asc_[A-Za-z0-9_]+)\s*&", argument)
    if reference:
        kind = reference.group(1)
        var = f"config_{index}_{name}"
        declarations.append(f"    {kind} {var}{{}};")
        return var
    if re.search(r"\bbool\b", argument):
        if name in {"c_matrix_init_val", "disable_gemv", "k_direction_align"}:
            return "true"
        return "false"
    if re.search(r"\b(half|bfloat16_t)\b", argument):
        kind = "bfloat16_t" if "bfloat16_t" in argument else "half"
        return f"static_cast<{kind}>(0)"
    if re.search(r"\bfloat\b", argument):
        return "0.0f"
    if re.search(r"\bdouble\b", argument):
        return "0.0"
    if re.search(r"\b(u?int|int)(8|16|32|64)_t\b", argument):
        if any(word in name for word in ("height", "width", "dim")) or name in {"m", "n", "k"}:
            return "16"
        if "stride" in name or "num" in name:
            return "1"
        return "0"
    enum_match = re.search(r"\b(asc_[A-Za-z0-9_]+)\b", argument)
    if enum_match:
        return f"static_cast<{enum_match.group(1)}>(0)"
    unsupported.append(argument)
    return "0"


def calls_for(items):
    lines = []
    unsupported = []
    buffers = {}
    for call_index, (api, args) in enumerate(items):
        declarations = []
        values = [argument_value(arg, call_index * 20 + i, declarations, buffers, unsupported)
                  for i, arg in enumerate(args)]
        lines.extend(declarations)
        call = f"{api}({', '.join(values)})"
        if api.startswith("asc_get_"):
            lines.append(f"    const uint64_t result_{call_index} = {call};")
            lines.append(f"    (void)result_{call_index};")
        else:
            lines.append(f"    {call};")
    buffer_lines = [f"    {domain} uint8_t {name}[LOCAL_BUFFER_BYTES];" for name, domain in buffers.items()]
    return "\n".join(buffer_lines + lines), unsupported


def source(name: str, body: str, apis: str, arch: str) -> str:
    rel = f"cube_compute/{name}"
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t LOCAL_BUFFER_BYTES = {BUFFER_BYTES};
constexpr uint32_t OUTPUT_ELEMENTS = 8;

__global__ __cube__ void {kernel}(__gm__ uint64_t* output, __gm__ uint8_t* input)
{{
    asc_init();
{body}
    asc_sync_pipe(PIPE_ALL);
    output[0] = static_cast<uint64_t>(input[0]);
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(32, 1);
    std::vector<uint64_t> output(OUTPUT_ELEMENTS, ~0ULL), golden(OUTPUT_ELEMENTS, 0);
    golden[0] = input[0];
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(input.size());
    CApiExample::DeviceBuffer<uint64_t> output_device(OUTPUT_ELEMENTS);
    if (!input_device.Allocate("aclrtMalloc(input)") || !output_device.Allocate("aclrtMalloc(output)") ||
        !input_device.CopyFrom(input) || !output_device.CopyFrom(golden)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get(), input_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::CompareRange(output, golden, 0, 1);
    std::cout << (passed ? "[Success] {apis} passed." : "[Failed] {apis} failed.") << std::endl;
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
        if unsupported:
            pending.append((path.name, sorted(set(unsupported))))
            continue
        apis = "/".join(api for api, _ in items)
        rendered.append((path.stem, source(path.stem, body, apis, architecture(path, markdown))))
    if pending:
        for name, arguments in pending:
            print(f"UNSUPPORTED {name}")
            for argument in arguments:
                print(f"  {argument}")
        raise SystemExit(f"Unsupported parameters in {len(pending)} documents")
    OUT.mkdir(parents=True, exist_ok=True)
    for name, text in rendered:
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(rendered)} cube compute examples in {OUT}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/cube_compute directory>")
    generate(Path(sys.argv[1]).resolve())
