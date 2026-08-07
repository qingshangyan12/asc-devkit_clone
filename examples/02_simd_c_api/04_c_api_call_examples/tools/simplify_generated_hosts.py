#!/usr/bin/env python3
"""Make generated C API examples self-contained and print-only on the host."""

from __future__ import annotations

import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PRINT_HELPERS = r'''
template <typename T>
void PrintData(const char* label, const std::vector<T>& values)
{
    std::cout << label << ":";
    const size_t count = values.size() < 8 ? values.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +values[i];
    if (values.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

void PrintHalfData(const char* label, const std::vector<aclFloat16>& values)
{
    std::cout << label << ":";
    const size_t count = values.size() < 8 ? values.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << aclFloat16ToFloat(values[i]);
    if (values.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

template <typename T>
class DeviceBuffer {
public:
    explicit DeviceBuffer(size_t count) : bytes_(count * sizeof(T))
    {
        aclrtMalloc(reinterpret_cast<void**>(&data_), bytes_, ACL_MEM_MALLOC_HUGE_FIRST);
    }

    ~DeviceBuffer() { Free(); }

    void CopyFrom(const std::vector<T>& host)
    {
        aclrtMemcpy(data_, bytes_, host.data(), bytes_, ACL_MEMCPY_HOST_TO_DEVICE);
    }

    void CopyTo(std::vector<T>& host)
    {
        aclrtMemcpy(host.data(), bytes_, data_, bytes_, ACL_MEMCPY_DEVICE_TO_HOST);
    }

    T* Get() const { return data_; }

    void Free()
    {
        if (data_ != nullptr) aclrtFree(data_);
        data_ = nullptr;
    }

private:
    T* data_ = nullptr;
    size_t bytes_ = 0;
};
'''.strip()


def split_top_level(text: str) -> list[str]:
    parts: list[str] = []
    start = 0
    depths = {"(": 0, "[": 0, "{": 0, "<": 0}
    pairs = {")": "(", "]": "[", "}": "{", ">": "<"}
    for index, char in enumerate(text):
        if char in depths:
            depths[char] += 1
        elif char in pairs and depths[pairs[char]]:
            depths[pairs[char]] -= 1
        elif char == "," and not any(depths.values()):
            parts.append(text[start:index].strip())
            start = index + 1
    parts.append(text[start:].strip())
    return parts


def remove_golden_declarations(source: str) -> str:
    pattern = re.compile(r"(?m)^(\s*)std::vector<([^>\n]+)>\s+([^;\n]+);$")

    def filter_declaration(match: re.Match[str]) -> str:
        indent, value_type, declarators = match.groups()
        kept = [item for item in split_top_level(declarators) if "golden" not in item.lower()]
        if not kept:
            return ""
        return f"{indent}std::vector<{value_type}> {', '.join(kept)};"

    return pattern.sub(filter_declaration, source)


def replace_status_if_blocks(source: str) -> str:
    patterns = [
        re.compile(r"(?ms)^    if \((.*?(?:Allocate|CopyFrom|CopyTo|CheckAcl).*?)\) return 1;\n"),
        re.compile(
            r"(?ms)^    if \((.*?(?:Allocate|CopyFrom|CopyTo|CheckAcl).*?)\) \{\n"
            r"        return 1;\n    \}\n"
        ),
    ]

    def replace_block(match: re.Match[str]) -> str:
        condition = match.group(1)
        calls: list[str] = []
        if "CheckAcl" in condition:
            calls.append("    aclrtSynchronizeDevice();")
        for variable, direction, host in re.findall(r"(\w+)\.Copy(From|To)\((\w+)\)", condition):
            if direction == "From" and "golden" in host.lower():
                host = variable[:-7] if variable.endswith("_device") else variable
            calls.append(f"    {variable}.Copy{direction}({host});")
        return "\n".join(calls) + ("\n" if calls else "")

    for pattern in patterns:
        source = pattern.sub(replace_block, source)
    return source


def replace_device_buffers(source: str) -> str:
    prefix, main_body = source.split("int main()", 1)
    value_types: dict[str, str] = {}
    declaration = re.compile(r"(?m)^    DeviceBuffer<([^>]+)>\s+([^;]+);$")

    def replace_declaration(match: re.Match[str]) -> str:
        value_type, declarators = match.groups()
        lines: list[str] = []
        for item in split_top_level(declarators):
            parsed = re.fullmatch(r"(\w+)\((.*)\)", item)
            if parsed is None:
                raise RuntimeError(f"unsupported device buffer declaration: {item}")
            name, count = parsed.groups()
            value_types[name] = value_type
            lines.extend(
                [
                    f"    {value_type}* {name} = nullptr;",
                    f"    aclrtMalloc(reinterpret_cast<void**>(&{name}), ({count}) * sizeof({value_type}),",
                    "        ACL_MEM_MALLOC_HUGE_FIRST);",
                ]
            )
        return "\n".join(lines)

    main_body = declaration.sub(replace_declaration, main_body)
    for name, value_type in value_types.items():
        main_body = re.sub(
            rf"^    {name}\.CopyFrom\((\w+)\);$",
            rf"    aclrtMemcpy({name}, \1.size() * sizeof({value_type}), \1.data(), \1.size() * sizeof({value_type}),\n"
            rf"        ACL_MEMCPY_HOST_TO_DEVICE);",
            main_body,
            flags=re.MULTILINE,
        )
        main_body = re.sub(
            rf"^    {name}\.CopyTo\((\w+)\);$",
            rf"    aclrtMemcpy(\1.data(), \1.size() * sizeof({value_type}), {name}, \1.size() * sizeof({value_type}),\n"
            rf"        ACL_MEMCPY_DEVICE_TO_HOST);",
            main_body,
            flags=re.MULTILINE,
        )
        main_body = main_body.replace(f"{name}.Get()", name)
        main_body = main_body.replace(f"    {name}.Free();", f"    aclrtFree({name});")

    prefix = re.sub(r"(?ms)\ntemplate <typename T>\nclass DeviceBuffer \{.*?\n\};\n", "\n", prefix)
    if "PrintHalfData(" not in main_body:
        prefix = re.sub(
            r"(?ms)\nvoid PrintHalfData\(const char\* label, const std::vector<aclFloat16>& values\)\n\{.*?\n\}\n",
            "\n",
            prefix,
        )
    return prefix + "int main()" + main_body


def simplify(path: Path) -> None:
    source = path.read_text()
    if "sample_host.h" not in source:
        if "DeviceBuffer<" in source:
            source = replace_device_buffers(source)
        source = re.sub(r"^\s*const .*\bexpected\b.*\n", "", source, flags=re.MULTILINE)
        path.write_text(source)
        return
    relative = path.relative_to(ROOT.parents[2]).as_posix()
    architecture = re.search(r"^// Architecture: (.+)$", source, re.MULTILINE).group(1)
    binary = "/tmp/c_api_" + "_".join(path.relative_to(ROOT).with_suffix("").parts)
    command = (
        f'// Build/run ({relative}): "$ASCEND_HOME_PATH/bin/bisheng" '
        f"--npu-arch={architecture} -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang "
        f"{relative} -lm -o {binary} && {binary}"
    )
    source = re.sub(r"^// Build/run .*$", command, source, count=1, flags=re.MULTILINE)
    source = source.replace('#include "common/sample_host.h"', '#include "acl/acl.h"')
    source = source.replace("namespace {\n", f"namespace {{\n{PRINT_HELPERS}\n\n", 1)
    source = source.replace("CApiExample::DeviceBuffer", "DeviceBuffer")
    source = re.sub(
        r"    CApiExample::Runtime runtime;\n"
        r"    if \(!runtime\.Init\(\)\) return 1;\n",
        "    aclInit(nullptr);\n    aclrtSetDevice(0);\n",
        source,
    )
    source = re.sub(
        r"    CApiExample::Runtime runtime;\n"
        r"    if \(!runtime\.Init\(\)\) \{\n        return 1;\n    \}\n",
        "    aclInit(nullptr);\n    aclrtSetDevice(0);\n",
        source,
    )
    prefix, main_body = source.split("int main()", 1)
    source = prefix + "int main()" + replace_status_if_blocks(main_body)
    source = source.replace("CApiExample::PrintHalf", "PrintHalfData")
    source = source.replace("CApiExample::Print", "PrintData")
    source = re.sub(r'^\s*Print(?:Half)?Data\("[^"]*[Gg]olden[^"]*".*\);\n', "", source, flags=re.MULTILINE)

    passed = source.find("    const bool passed =")
    if passed == -1:
        raise RuntimeError(f"missing comparison tail: {path}")
    close = source.rfind("}\n")
    buffers = sorted(set(re.findall(r"\b(\w+_device)\s*\(", source)))
    cleanup = "".join(f"    {name}.Free();\n" for name in buffers)
    source = (
        source[:passed]
        + cleanup
        + "    aclrtResetDevice(0);\n"
        + "    aclFinalize();\n"
        + "    return 0;\n"
        + source[close:]
    )

    # Preserve mixed input initialization while removing the corresponding golden assignment.
    source = re.sub(r"(\w+\[[^]]+\])\s*=\s*golden\[[^]]+\]\s*=", r"\1 =", source)
    source = remove_golden_declarations(source)
    source = re.sub(r"^.*golden.*\n", "", source, flags=re.MULTILINE | re.IGNORECASE)
    source = re.sub(r"^\s*const .*\bexpected\b.*\n", "", source, flags=re.MULTILINE)
    source = re.sub(r"(?ms)^\s*for \([^\n]+\) \{\n\s*\}\n", "", source)
    source = replace_device_buffers(source)

    if "CApiExample::" in source or "sample_host.h" in source or "golden" in source.lower():
        raise RuntimeError(f"incomplete simplification: {path}")
    path.write_text(source)


def main() -> None:
    examples = sorted(ROOT.rglob("*.asc"))
    for path in examples:
        simplify(path)
    print(f"Simplified {len(examples)} C API examples")


if __name__ == "__main__":
    main()
