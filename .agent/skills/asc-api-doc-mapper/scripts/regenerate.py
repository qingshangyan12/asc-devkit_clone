#!/usr/bin/env python3
"""Regenerate api-doc-mapping.json by scanning include/ headers and docs/api/ pages.

Covers 7 tiers: basic_api, adv_api, c_api, simt_api, tensor_api, aicpu_api, utils.

Two-tier extraction:
  Tier 1 (HIGH): 头文件路径为：xxx.h and 包含"xxx.h"头文件 in docs (all tiers)
  Tier 2 (MEDIUM): function-name matching against header declarations (all tiers)

Output: .agent/skills/asc-api-doc-mapper/references/api-doc-mapping.json
"""

import json
import os
import re
from collections import defaultdict
from datetime import datetime
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[4]

TIERS = [
    {
        "name": "basic_api",
        "headers": "include/basic_api",
        "docs": "docs/api/SIMD-API/基础API",
    },
    {
        "name": "adv_api",
        "headers": "include/adv_api",
        "docs": "docs/api/SIMD-API/高阶API",
    },
    {
        "name": "c_api",
        "headers": "include/c_api",
        "docs": "docs/api/SIMD-API/C-API",
    },
    {
        "name": "simt_api",
        "headers": "include/simt_api",
        "docs": "docs/api/SIMT-API",
    },
    {
        "name": "tensor_api",
        "headers": "include/tensor_api",
        "docs": "docs/api/tensor_api",
    },
    {
        "name": "aicpu_api",
        "headers": "include/aicpu_api",
        "docs": "docs/api/AI-CPU-API",
    },
    {
        "name": "utils",
        "headers": "include/utils",
        "docs": "docs/api/Utils-API",
    },
]

# Regex to find function declarations in headers.
# Matches: [attrs] [inline] return_type FuncName(
# Handles AscendC patterns like: __aicore__ inline void FuncName(
#                                template<...> void FuncName(
#                                __simd_callee__ inline const LocalTensor<float> FuncName(
FUNC_DECL_RE = re.compile(
    r"(?:__\w+__\s+)?(?:inline\s+|constexpr\s+)*"
    r"(?:[\w:]+(?:<[^>]*>)?[\s*&]*\s+)+?"  # return type
    r"(\w+)\s*\([^)]*\)",  # FuncName(
    re.MULTILINE,
)

# Words that are never function names (C++ keywords, common macros)
NON_FUNC_NAMES = {
    "if", "else", "while", "for", "switch", "case", "return", "sizeof",
    "typeof", "decltype", "static", "const", "volatile", "typedef",
    "class", "struct", "enum", "union", "namespace", "using", "template",
    "typename", "explicit", "friend", "virtual", "override", "final",
    "noexcept", "constexpr", "thread_local", "register", "extern",
    "public", "private", "protected", "operator", "new", "delete",
    "catch", "throw", "try", "static_cast", "dynamic_cast", "const_cast",
    "reinterpret_cast", "alignof", "alignas", "static_assert",
}

# Doc files to skip (not function docs)
SKIP_DOC_PATTERNS = [
    "README.md", "概述", "列表.md", "简介",
    "通用说明", "使用说明", "使用约束",
]


def _is_skip_doc(doc_path: str) -> bool:
    for p in SKIP_DOC_PATTERNS:
        if p in doc_path:
            return True
    return False


def _unescape_heading(text: str) -> str:
    r"""Unescape markdown escapes like \_ -> _ and \* -> *."""
    return text.replace("\\_", "_").replace("\\*", "*").replace("\\#", "#")


def _clean_func_name(name: str) -> str:
    """Strip anchors, (ISASI) suffix, and whitespace from a function name."""
    name = re.sub(r"<a\s+name=.*", "", name, flags=re.IGNORECASE)
    name = re.sub(r"\s*\(ISASI\)\s*$", "", name)
    return name.strip()


def build_header_index(tier: dict) -> dict[str, set[str]]:
    """Scan header files and return {func_name: {header_paths}}."""
    headers_dir = REPO_ROOT / tier["headers"]
    if not headers_dir.is_dir():
        return {}

    index: dict[str, set[str]] = defaultdict(set)
    for hfile in headers_dir.rglob("*.h"):
        try:
            content = hfile.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue
        rel = str(hfile.relative_to(REPO_ROOT / "include"))
        for m in FUNC_DECL_RE.finditer(content):
            name = m.group(1)
            if name not in NON_FUNC_NAMES and not name.startswith("_"):
                index[name].add(rel)
                # Also index by lower-case for case-insensitive lookup
                index[name.lower()].add(rel)
    return index


def extract_tier1(tier: dict) -> list[dict]:
    """Extract HIGH-confidence mappings from explicit header references.

    Matches two patterns:
      1. basic_api: 头文件路径为："basic_api/kernel_xxx.h"
      2. all tiers:  使用该接口需要包含"simt_api/xxx.h"头文件
    """
    results = []
    docs_dir = REPO_ROOT / tier["docs"]
    if not docs_dir.is_dir():
        return results

    # Pattern 1: 头文件路径为：`"basic_api/xxx.h"`
    header_ref_re1 = re.compile(
        r"头文件路径为[：:]\s*[`\"]*"
        r"([a-zA-Z_][a-zA-Z0-9_\\/]*\.h)"
    )
    # Pattern 2: 包含"tier/xxx.h"头文件 (handles escaped underscores like simt\_api)
    header_ref_re2 = re.compile(
        r"包含[`\"]"
        r"([a-zA-Z_][a-zA-Z0-9_\\/]*\.h)"
        r"[`\"][^。]*头文件"
    )

    for mdfile in docs_dir.rglob("*.md"):
        if _is_skip_doc(mdfile.name) or _is_skip_doc(str(mdfile)):
            continue
        try:
            content = mdfile.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue

        hdr_match = header_ref_re1.search(content)
        if not hdr_match:
            hdr_match = header_ref_re2.search(content)
        if not hdr_match:
            continue

        header_file = hdr_match.group(1).replace("\\_", "_")

        # Extract function name from first H1
        func_match = re.search(r"^#\s+(.+)$", content, re.MULTILINE)
        if not func_match:
            continue
        func_name = _clean_func_name(_unescape_heading(func_match.group(1)))
        if not func_name or len(func_name) > 80:
            continue

        doc_rel = str(mdfile.relative_to(REPO_ROOT / "docs/api"))
        results.append({
            "name": func_name,
            "header": header_file,
            "doc_path": doc_rel,
            "tier": tier["name"],
            "confidence": "HIGH",
        })
    return results


def extract_tier2(
    tier: dict, header_index: dict[str, set[str]], tier1_doc_paths: set[str]
) -> list[dict]:
    """Extract MEDIUM-confidence mappings via function-name matching.

    Skips docs already handled by Tier 1.
    """
    results = []
    docs_dir = REPO_ROOT / tier["docs"]
    if not docs_dir.is_dir():
        return results

    for mdfile in docs_dir.rglob("*.md"):
        if _is_skip_doc(mdfile.name) or _is_skip_doc(str(mdfile)):
            continue

        doc_rel = str(mdfile.relative_to(REPO_ROOT / "docs/api"))
        if doc_rel in tier1_doc_paths:
            continue

        try:
            content = mdfile.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue

        # Only process docs with function prototypes
        if "## 函数原型" not in content and "函数原型" not in content:
            continue

        # Extract function name from first H1
        func_match = re.search(r"^#\s+(.+)$", content, re.MULTILINE)
        if not func_match:
            continue
        func_name = _clean_func_name(_unescape_heading(func_match.group(1)))
        if not func_name or len(func_name) > 80:
            continue

        # Try exact match first, then case-insensitive
        candidates = header_index.get(func_name, set())
        if not candidates:
            candidates = header_index.get(func_name.lower(), set())

        if len(candidates) == 1:
            results.append({
                "name": func_name,
                "header": list(candidates)[0],
                "doc_path": doc_rel,
                "tier": tier["name"],
                "confidence": "MEDIUM",
            })
        elif len(candidates) > 1:
            results.append({
                "name": func_name,
                "header": " | ".join(sorted(candidates)),
                "doc_path": doc_rel,
                "tier": tier["name"],
                "confidence": "LOW",
            })
        else:
            results.append({
                "name": func_name,
                "header": None,
                "doc_path": doc_rel,
                "tier": tier["name"],
                "confidence": "NONE",
            })

    return results


def cross_reference(
    tier: dict,
    header_index: dict[str, set[str]],
    tier1_results: list[dict],
    tier2_results: list[dict],
) -> tuple[list[dict], list[dict]]:
    """Find unreferenced headers and unmapped docs."""
    # All unique headers on disk
    headers_dir = REPO_ROOT / tier["headers"]
    all_headers: set[str] = set()
    if headers_dir.is_dir():
        for hfile in headers_dir.rglob("*.h"):
            all_headers.add(str(hfile.relative_to(REPO_ROOT / "include")))

    # Headers referenced in results
    referenced: set[str] = set()
    for r in tier1_results + tier2_results:
        h = r.get("header")
        if h and " | " not in h:
            referenced.add(h)

    unreferenced = []
    for h in sorted(all_headers - referenced):
        reason = ""
        if "kernel_base_types" in h or "kernel_type" in h:
            reason = "类型定义头文件"
        elif "kernel_struct_" in h:
            reason = "结构体定义"
        unreferenced.append({"header": h, "tier": tier["name"], "reason": reason})

    unmapped = [
        r for r in tier2_results if r["confidence"] in ("LOW", "NONE")
    ]

    return unreferenced, unmapped


def main():
    all_functions = []
    all_unreferenced = []
    all_unmapped = []

    for tier in TIERS:
        print(f"Processing tier: {tier['name']} ...")

        # Phase 1: Build header index
        header_index = build_header_index(tier)
        print(f"  Headers indexed: {len(header_index)} unique function names")

        # Phase 2: Tier 1 (HIGH) - explicit header references
        tier1 = extract_tier1(tier)
        tier1_doc_paths = {r["doc_path"] for r in tier1}
        print(f"  Tier 1 (HIGH): {len(tier1)} mappings")

        # Phase 3: Tier 2 (MEDIUM) - function name matching
        tier2 = extract_tier2(tier, header_index, tier1_doc_paths)
        med = sum(1 for r in tier2 if r["confidence"] == "MEDIUM")
        low = sum(1 for r in tier2 if r["confidence"] == "LOW")
        none_ = sum(1 for r in tier2 if r["confidence"] == "NONE")
        print(f"  Tier 2: {med} MEDIUM, {low} LOW, {none_} NONE")

        # Phase 4: Cross-reference
        unreferenced, unmapped = cross_reference(tier, header_index, tier1, tier2)
        print(f"  Unreferenced headers: {len(unreferenced)}")
        print(f"  Unmapped docs: {len(unmapped)}")

        # Collect HIGH + MEDIUM into functions, LOW/NONE into unmapped
        all_functions.extend(r for r in tier1 if r["confidence"] == "HIGH")
        all_functions.extend(r for r in tier2 if r["confidence"] == "MEDIUM")
        all_unreferenced.extend(unreferenced)
        all_unmapped.extend(unmapped)

    # Deduplicate functions by (name, header, doc_path, tier)
    seen = set()
    deduped = []
    for r in all_functions:
        key = (r["name"], r["header"], r["doc_path"], r["tier"])
        if key not in seen:
            seen.add(key)
            deduped.append(r)
    all_functions = deduped

    # Build output
    output = {
        "functions": sorted(all_functions, key=lambda r: (r["tier"], r["name"])),
        "unreferenced_headers": all_unreferenced,
        "unmapped_docs": all_unmapped,
        "metadata": {
            "generated_at": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "total_functions": len(all_functions),
            "high_confidence": sum(1 for r in all_functions if r["confidence"] == "HIGH"),
            "medium_confidence": sum(1 for r in all_functions if r["confidence"] == "MEDIUM"),
            "unreferenced_headers": len(all_unreferenced),
            "unmapped_docs": len(all_unmapped),
        },
    }

    out_path = REPO_ROOT / ".agent/skills/asc-api-doc-mapper/references/api-doc-mapping.json"
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(output, f, ensure_ascii=False, indent=2)

    print(f"\nDone. Output: {out_path}")
    print(f"  Total HIGH: {output['metadata']['high_confidence']}")
    print(f"  Total MEDIUM: {output['metadata']['medium_confidence']}")
    print(f"  Unreferenced headers: {output['metadata']['unreferenced_headers']}")
    print(f"  Unmapped docs: {output['metadata']['unmapped_docs']}")


if __name__ == "__main__":
    main()
