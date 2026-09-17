#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""Compile generated bindings to check specialization option semantics."""

import importlib.util
from itertools import product
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


class TestSkBindOptions(unittest.TestCase):
    @unittest.skipUnless(shutil.which("g++"), "requires a host C++ compiler")
    def test_dcci_override_preserves_other_capabilities(self):
        root = Path(__file__).resolve().parents[3]
        for variant in ("asc_op_compiler", "adapter"):
            module_dir = root / "tools/build/asc_op_compile_base" / variant
            module_name = "sk_bind_options_" + variant
            spec = importlib.util.spec_from_file_location(
                module_name, module_dir / "static_compile_resource_generator.py"
            )
            module = importlib.util.module_from_spec(spec)
            sys.modules[module_name] = module
            sys.path.insert(0, str(module_dir))
            try:
                spec.loader.exec_module(module)
            finally:
                sys.path.pop(0)
                del sys.modules[module_name]
            with self.subTest(variant=variant), tempfile.TemporaryDirectory() as directory:
                temp = Path(directory)
                # Match the real binding's integral template argument contract.
                (temp / "kernel_operator.h").write_text(
                    "#pragma once\n#include <cstdint>\n"
                    "template<auto GF, uint64_t cap, auto... SK> struct Binding {\n"
                    "  static constexpr uint64_t capability = cap;\n};\n"
                    "#define SK_BIND(...) "
                    "static_assert(Binding<__VA_ARGS__>::capability == EXPECTED)\n"
                )
                cases = []
                for bitmap, override, gm_dcci in product(
                    range(16), (None, 0, 1), (False, True)
                ):
                    cases.extend(
                        [
                            "#undef __ASCENDC_SUPER_KERNEL_DISABLE_DCCI__",
                            "#undef __ASCENDC_SUPER_KERNEL_ENABLE_GM_GET_SET_VALUE_DCCI__",
                            "#undef EXPECTED",
                        ]
                    )
                    expected = bitmap
                    if override is not None:
                        cases.append(
                            f"#define __ASCENDC_SUPER_KERNEL_DISABLE_DCCI__ {override}"
                        )
                        expected = (bitmap & ~4) | (4 if override else 0)
                    if gm_dcci:
                        cases.append(
                            "#define __ASCENDC_SUPER_KERNEL_ENABLE_GM_GET_SET_VALUE_DCCI__"
                        )
                    cases.append(f"#define EXPECTED {expected}")
                    cases.append(
                        module.gen_sk_bind_source(
                            [("basic_a", "sk_a"), ("basic_b", "sk_b")], "", "", bitmap
                        ).decode("utf-8")
                    )
                source = temp / "binding.cpp"
                source.write_text("\n".join(cases))
                result = subprocess.run(
                    [shutil.which("g++"), "-std=c++17", "-fsyntax-only", str(source)],
                    capture_output=True,
                    text=True,
                    check=False,
                )
                self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
