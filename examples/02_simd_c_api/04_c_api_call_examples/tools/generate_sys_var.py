#!/usr/bin/env python3

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "sys_var"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def architecture(markdown: str) -> str:
    line = next((line for line in markdown.splitlines() if "Ascend 950PR/Ascend 950DT" in line), "")
    return "dav-3510" if "：支持" in line else "dav-2201"


def function_name(markdown: str) -> str:
    section = re.search(r"## 函数原型\s*(.*?)(?=\n## )", markdown, re.S)
    if not section:
        raise ValueError("missing prototype section")
    match = re.search(r"(asc_[A-Za-z0-9_]+)\s*\(", section.group(1))
    if not match:
        raise ValueError("missing API name")
    return match.group(1)


def invocation(api: str) -> str:
    if api == "asc_get_arch_ver":
        return "    uint32_t core_version = 0;\n    asc_get_arch_ver(core_version);\n    const uint64_t observed = core_version;"
    if api == "asc_get_phy_buf_addr":
        return "    const uint64_t observed = asc_get_phy_buf_addr(0);"
    if api == "asc_set_ctrl":
        return "    const uint64_t observed = static_cast<uint64_t>(asc_get_ctrl());\n    asc_set_ctrl(observed);"
    if api == "asc_set_ffts_base_addr":
        return "    const uint64_t observed = static_cast<uint64_t>(asc_get_ffts_base_addr());\n    asc_set_ffts_base_addr(observed);"
    if api.startswith("asc_set_"):
        return f"    const uint64_t observed = 0;\n    {api}(observed);"
    return f"    const uint64_t observed = static_cast<uint64_t>({api}());"


def source(file_name: str, api: str, arch: str) -> str:
    rel = f"sys_var/{file_name}"
    kernel = "".join(part.capitalize() for part in file_name.split("_")) + "Kernel"
    call = invocation(api)
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: {arch}

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENTS = 8;

__global__ __vector__ void {kernel}(__gm__ uint32_t* output)
{{
    asc_init();
{call}
    output[0] = 1;
    output[1] = static_cast<uint32_t>(observed);
    output[2] = static_cast<uint32_t>(observed >> 32);
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint32_t> input(ELEMENTS, 0), output(ELEMENTS, 0xffffffffU), golden(ELEMENTS, 0);
    golden[0] = 1;
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint32_t> output_device(ELEMENTS);
    if (!output_device.Allocate("aclrtMalloc(output)") || !output_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output (marker, observed low/high)", output);
    CApiExample::Print("Golden marker", golden);
    const bool passed = CApiExample::CompareRange(output, golden, 0, 1);
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def generate(doc_dir: Path) -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    count = 0
    for path in sorted(doc_dir.glob("*.md")):
        markdown = path.read_text(encoding="utf-8")
        if "## 函数原型" not in markdown:
            continue
        api = function_name(markdown)
        (OUT / f"{path.stem}.asc").write_text(source(path.stem, api, architecture(markdown)), encoding="utf-8")
        count += 1
    print(f"Generated {count} system variable examples in {OUT}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/sys_var directory>")
    generate(Path(sys.argv[1]).resolve())
