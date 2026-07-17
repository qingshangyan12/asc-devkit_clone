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


class TestStaticCompileResourceGenerator(unittest.TestCase):
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
