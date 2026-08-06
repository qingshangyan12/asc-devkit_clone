#!/usr/bin/env python3

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "cube_datamove"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"
BUFFER_BYTES = 8192


def architecture(path: Path, markdown: str) -> str:
    if "_arch_2201" in path.stem:
        return "dav-2201"
    if "_arch_3510" in path.stem:
        return "dav-3510"
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
            if api in {"asc_copy_l12bt", "asc_copy_l12bt_sync"} and len(args) == 3 and "__cbuf__ half*" in args[1]:
                found = [(name, args if name == api else existing) for name, existing in found]
            continue
        found.append((api, args))
        seen.add(api)
    return found


def variable_name(argument: str, fallback: str) -> str:
    clean = re.sub(r"\[[^]]*\]", "", argument).strip()
    match = re.search(r"([A-Za-z_][A-Za-z0-9_]*)\s*$", clean)
    return match.group(1) if match else fallback


def argument_value(argument: str, index: int, declarations, buffers, unsupported):
    name = variable_name(argument, f"arg_{index}")
    pointer = re.search(
        r"(__gm__|__ca__|__cb__|__cc__|__cbuf__|__fbuf__|__ubuf__)\s+(?:const\s+)?([A-Za-z0-9_:]+)\s*\*",
        argument)
    if pointer:
        domain, scalar = pointer.groups()
        if domain == "__gm__":
            host_buffer = "output" if any(word in name for word in ("dst", "output")) else "input"
            return f"reinterpret_cast<__gm__ {scalar}*>({host_buffer})"
        buffer_names = {
            "__ca__": "a_l0_bytes",
            "__cb__": "b_l0_bytes",
            "__cc__": "c_l0_bytes",
            "__cbuf__": "l1_bytes",
            "__fbuf__": "fixpipe_bytes",
            "__ubuf__": "ub_bytes",
        }
        buffer_name = buffer_names[domain]
        buffers[buffer_name] = domain
        return f"reinterpret_cast<{domain} {scalar}*>({buffer_name})"
    config_ref = re.search(r"\b(asc_[A-Za-z0-9_]+)\s*&", argument)
    if config_ref:
        kind = config_ref.group(1)
        var = f"config_{index}_{name}"
        declarations.append(f"    {kind} {var}{{}};")
        return var
    scalar_ref = re.search(r"\b(half|float|double|u?int(?:8|16|32|64)_t)\s*&", argument)
    if scalar_ref:
        kind = scalar_ref.group(1)
        var = f"value_{index}_{name}"
        value = "0.0f" if kind in {"float", "double"} else ("static_cast<half>(0)" if kind == "half" else "0")
        declarations.append(f"    {kind} {var} = {value};")
        return var
    if re.search(r"\bbool\b", argument):
        return "false"
    if re.search(r"\bhalf\b", argument):
        return "static_cast<half>(0)"
    if re.search(r"\bbfloat16_t\b", argument):
        return "static_cast<bfloat16_t>(0)"
    if re.search(r"\bfloat\b", argument):
        return "0.0f"
    if re.search(r"\bdouble\b", argument):
        return "0.0"
    integer = re.search(r"\b(u?int(?:8|16|32|64)_t|int(?:8|16|32|64)_t)\b", argument)
    if integer:
        kind = integer.group(1)
        if name == "size" or name.endswith("_size"):
            value = "32"
        elif any(word in name for word in ("n_burst", "len_burst", "repeat", "step", "num", "value")):
            value = "1"
        elif "stride" in name or "gap" in name or "position" in name or "index" in name:
            value = "0"
        else:
            value = "0"
        return f"static_cast<{kind}>({value})"
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
        lines.append(f"    {api}({', '.join(values)});")
        lines.append("    asc_sync_pipe(PIPE_ALL);")
    buffer_lines = [f"    {domain} uint8_t {name}[LOCAL_BUFFER_BYTES];" for name, domain in buffers.items()]
    return "\n".join(buffer_lines + lines), unsupported


def source(rel: Path, body: str, apis: str, arch: str) -> str:
    rel_text = (Path("cube_datamove") / rel).as_posix()
    kernel = "".join(part.capitalize() for part in re.split(r"[_/]", rel.as_posix())) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel_text}.asc): bash {PREFIX}/run_example.sh {rel_text}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t LOCAL_BUFFER_BYTES = {BUFFER_BYTES};
constexpr uint32_t IO_BYTES = {BUFFER_BYTES};

__global__ __cube__ void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input)
{{
    asc_init();
{body}
    output[0] = input[0];
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(IO_BYTES, 1), output(IO_BYTES, 0), golden(IO_BYTES, 0);
    golden[0] = input[0];
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint8_t> input_device(IO_BYTES);
    CApiExample::DeviceBuffer<uint8_t> output_device(IO_BYTES);
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
    for path in sorted(doc_dir.rglob("*.md")):
        markdown = path.read_text(encoding="utf-8")
        items = prototypes(markdown)
        if not items:
            continue
        body, unsupported = calls_for(items)
        rel = path.relative_to(doc_dir).with_suffix("")
        if unsupported:
            pending.append((rel.as_posix(), sorted(set(unsupported))))
            continue
        apis = "/".join(api for api, _ in items)
        rendered.append((rel, source(rel, body, apis, architecture(path, markdown))))
    if pending:
        for name, arguments in pending:
            print(f"UNSUPPORTED {name}")
            for argument in arguments:
                print(f"  {argument}")
        raise SystemExit(f"Unsupported parameters in {len(pending)} documents")
    for rel, text in rendered:
        destination = OUT / rel.with_suffix(".asc")
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text, encoding="utf-8")
    print(f"Generated {len(rendered)} cube data movement examples in {OUT}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/cube_datamove directory>")
    generate(Path(sys.argv[1]).resolve())
