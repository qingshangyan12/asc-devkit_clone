#!/usr/bin/python3
# coding=utf-8

import hashlib
import importlib.util
import json
import os
import sys
import tempfile
import unittest

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
MODULE_PATH = os.path.join(
    TOP_PATH,
    "tools/build/asc_op_compile_base/asc_op_compiler/static_compile_resource_generator.py",
)
MODULE_SPEC = importlib.util.spec_from_file_location(
    "asc_devkit_static_compile_resource_generator", MODULE_PATH
)
MODULE = importlib.util.module_from_spec(MODULE_SPEC)
MODULE_SPEC.loader.exec_module(MODULE)
generate_static_compile_resources = MODULE.generate_static_compile_resources
collect_kernel_symbols = MODULE.collect_kernel_symbols
collect_local_include_files = MODULE.collect_local_include_files
parse_compile_option_macros = MODULE.parse_compile_option_macros
portable_wrapper_text = MODULE.portable_wrapper_text
resolve_symbol_tiling_sizes = MODULE.resolve_symbol_tiling_sizes
static_tiling_template_text = MODULE.static_tiling_template_text
super_kernel_build_extension = MODULE.super_kernel_build_extension


class TestStaticCompileResourceGenerator(unittest.TestCase):
    def test_collects_recursive_local_quoted_include_files(self):
        with tempfile.TemporaryDirectory() as root:
            os.makedirs(os.path.join(root, "detail"))
            source = os.path.join(root, "kernel.cpp")
            header = os.path.join(root, "tiling.h")
            detail = os.path.join(root, "detail", "types.h")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write('#include "tiling.h"\n#include "kernel_operator.h"\n')
            with open(header, "w", encoding="utf-8") as file_obj:
                file_obj.write('#include "detail/types.h"\n')
            with open(detail, "w", encoding="utf-8") as file_obj:
                file_obj.write("types\n")
            self.assertEqual(
                collect_local_include_files(source),
                [(header, "tiling.h"), (detail, "detail/types.h")],
            )

    def test_builds_optional_super_kernel_extension_from_launch_context(self):
        extension = super_kernel_build_extension(
            normal_output="artifacts/normal.o",
            sk_symbol="add_sk",
            sk_argv=["-c", "-D__SUPER_KERNEL_STATIC_BLOCK_NUM__=${launch.block_dim}"],
            sk_inputs=["resources/src/sk.cpp", "resources/include/tiling.h"],
            bind_argv=["-c", "resources/src/bind.cpp"],
            bind_input="resources/src/bind.cpp",
            output_prefix="artifacts/add",
        )
        steps = extension["super_kernel"]["steps"]
        self.assertEqual(
            [step["kind"] for step in steps],
            ["compile", "objcopy", "objcopy", "objcopy", "archive", "compile", "link", "link"],
        )
        self.assertIn("${launch.block_dim}", " ".join(steps[0]["argv"]))
        self.assertEqual(steps[-1]["outputs"], ["artifacts/add.static_with_sk.o"])

    def test_rewrites_only_the_original_source_include_for_portability(self):
        wrapper = '#include "/tmp/build/add.cpp"\n#include "kernel_common.h"\n'
        self.assertEqual(
            portable_wrapper_text(wrapper, "/tmp/build/add.cpp"),
            '#include "add.cpp"\n#include "kernel_common.h"\n',
        )

    def test_adapts_dynamic_tiling_header_with_one_static_value_token(self):
        dynamic_header = """#ifndef TILING_H\n#define TILING_H\nclass AddTilingData {};\n#define GET_TILING_DATA(name, arg) InitTilingData<AddTilingData>(arg, &name)\n#endif\n"""
        result = static_tiling_template_text(dynamic_header)
        self.assertIn("InitTilingData<AddTilingData>", result)
        self.assertIn("#ifdef CONST_TILING", result)
        self.assertIn("convert_from_bytes<AddTilingData>", result)
        self.assertEqual(result.count("@@STATIC_VALUE_tiling_data@@"), 1)
        self.assertNotIn("\\\n\n", result)

    def test_extracts_compile_macros_with_stable_kinds(self):
        self.assertEqual(
            parse_compile_option_macros([
                "-O3", "-DDTYPE_X=half", "-DORIG_DTYPE_X=DT_FLOAT16",
                "-DFORMAT_X=FORMAT_ND", "-DASCENDC_DUMP=1",
                "-DL2_CACHE_HINT", "-DDTYPE_X=half",
            ]),
            [
                {"name": "DTYPE_X", "value": "half", "kind": "dtype"},
                {"name": "ORIG_DTYPE_X", "value": "DT_FLOAT16", "kind": "dtype"},
                {"name": "FORMAT_X", "value": "FORMAT_ND", "kind": "format"},
                {"name": "ASCENDC_DUMP", "value": "1", "kind": "compile"},
                {"name": "L2_CACHE_HINT", "value": "1", "kind": "compile"},
            ],
        )

    def test_collects_actual_symbols_from_final_metadata(self):
        metadata = {
            "kernelName": "dispatch_entry",
            "kernelList": [{"kernelName": "actual_7"}, {"kernelName": "actual_9"}],
            "sub_operator_kernel_name": {
                "AiCore": {"func_name": "sk_actual"},
                "dynamic_func_names": {
                    "7": {"func_name": "actual_7"},
                    "9": {"func_name": "actual_9"},
                },
            },
        }
        self.assertEqual(
            collect_kernel_symbols(metadata, ["linked_actual", "actual_7"]),
            ["linked_actual", "actual_7", "dispatch_entry", "actual_9", "sk_actual"],
        )

    def test_resolves_exact_tiling_size_without_using_tiling_key_as_index(self):
        self.assertEqual(
            resolve_symbol_tiling_sizes(
                ["add_7", "add_9", "dispatch_entry"],
                ["7", "9"], {"7": 32, "9": 48}, 64,
            ),
            {"add_7": 32, "add_9": 48, "dispatch_entry": 64},
        )

    def test_generates_symbol_indexed_resource_directory(self):
        with tempfile.TemporaryDirectory() as root:
            binary = os.path.join(root, "AddCustom.o")
            source = os.path.join(root, "kernel.cpp")
            with open(binary, "wb") as file_obj:
                file_obj.write(b"dynamic-binary")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("kernel source\n")

            result = generate_static_compile_resources(
                output_root=root,
                kernel_base="AddCustom_xxx",
                op_type="AddCustom",
                binary_path=binary,
                resources=[
                    {"source": source, "path": "resources/src/kernel.cpp"}
                ],
                include_dirs=["resources/include"],
                kernels=[
                    {
                        "kernel_name": "add_custom_0",
                        "kernel_type": "KERNEL_TYPE_AIV_ONLY",
                        "architecture": "dav-c220-vec",
                        "macros": [
                            {"name": "DTYPE_X", "value": "half", "kind": "dtype"},
                            {"name": "FORMAT_X", "value": "FORMAT_ND", "kind": "format"},
                        ],
                        "static_targets": [
                            {
                                "name": "tiling_data",
                                "value_type": "bytes",
                                "required": True,
                                "parameter_index": 4,
                                "byte_size": 8,
                            }
                        ],
                        "actions": [],
                        "build": {
                            "base": {"steps": []},
                            "extensions": {"super_kernel": {"steps": []}},
                        },
                    }
                ],
            )

            manifest_path = os.path.join(result, "op_resource.json")
            with open(manifest_path, "r", encoding="utf-8") as file_obj:
                manifest = json.load(file_obj)
            self.assertEqual(manifest["contract_version"], "3.0")
            self.assertEqual(
                manifest["op"]["binary"]["sha256"],
                hashlib.sha256(b"dynamic-binary").hexdigest(),
            )
            kernel = manifest["op"]["kernels"][0]
            self.assertEqual(kernel["kernel_name"], "add_custom_0")
            self.assertEqual(kernel["static_targets"][0]["parameter_index"], 4)
            self.assertEqual(kernel["static_targets"][0]["byte_size"], 8)
            self.assertIn("super_kernel", kernel["build"]["extensions"])
            self.assertNotIn("dfx", json.dumps(manifest).lower())
            self.assertNotIn("oom", json.dumps(manifest).lower())
            copied = os.path.join(result, "resources/src/kernel.cpp")
            self.assertTrue(os.path.isfile(copied))
            self.assertEqual(
                manifest["op"]["resources"]["files"][0]["sha256"],
                hashlib.sha256(b"kernel source\n").hexdigest(),
            )

    def test_cleans_staging_directory_when_resource_copy_fails(self):
        with tempfile.TemporaryDirectory() as root:
            binary = os.path.join(root, "AddCustom.o")
            with open(binary, "wb") as file_obj:
                file_obj.write(b"dynamic-binary")
            with self.assertRaises(FileNotFoundError):
                generate_static_compile_resources(
                    output_root=root,
                    kernel_base="AddCustom_xxx",
                    op_type="AddCustom",
                    binary_path=binary,
                    resources=[
                        {"source": os.path.join(root, "missing.cpp"),
                         "path": "resources/src/kernel.cpp"}
                    ],
                    include_dirs=[],
                    kernels=[],
                )
            static_root = os.path.join(root, "static_compile")
            self.assertFalse(os.path.exists(os.path.join(static_root, "AddCustom_xxx")))
            if os.path.isdir(static_root):
                self.assertEqual(os.listdir(static_root), [])


if __name__ == "__main__":
    unittest.main()
