#!/usr/bin/python3
# coding=utf-8

import importlib.util
import json
import os
import tempfile
import unittest

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
MODULE_PATH = os.path.join(
    TOP_PATH,
    "tools/build/asc_op_compile_base/adapter/static_compile_resource_generator.py",
)
MODULE_SPEC = importlib.util.spec_from_file_location(
    "adapter_static_compile_resource_generator", MODULE_PATH
)
MODULE = importlib.util.module_from_spec(MODULE_SPEC)
MODULE_SPEC.loader.exec_module(MODULE)
generate_static_compile_resources = MODULE.generate_static_compile_resources
collect_kernel_symbols = MODULE.collect_kernel_symbols
collect_local_include_files = MODULE.collect_local_include_files
normalize_static_command = MODULE.normalize_static_command
normalize_static_program = MODULE.normalize_static_program
portable_wrapper_text = MODULE.portable_wrapper_text
resolve_symbol_tiling_sizes = MODULE.resolve_symbol_tiling_sizes
static_tiling_template_text = MODULE.static_tiling_template_text


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

    def test_normalizes_programs_and_portable_command_paths(self):
        self.assertEqual(
            [normalize_static_program(kind) for kind in ("compile", "link", "objcopy", "archive")],
            [
                "${env:ASCEND_HOME_PATH}/bin/bisheng",
                "${env:ASCEND_HOME_PATH}/bin/ld.lld",
                "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy",
                "${env:ASCEND_HOME_PATH}/bin/llvm-ar",
            ],
        )
        argv = [
            "-I/usr/local/Ascend/cann-9.2.0/aarch64-linux/asc/include",
            "-I/usr/local/Ascend/cann-9.2.0/aarch64-linux/asc/../ascendc/act",
            "-I/tmp/generated/op_build/ascendc/common",
            "${resource.resources/src/k.cpp}",
            "-o",
            "${artifact.artifacts/k.o}",
        ]
        self.assertEqual(
            normalize_static_command(
                "compile", argv, "/usr/local/Ascend/cann-9.2.0", 0
            ),
            [
                "${env:ASCEND_HOME_PATH}/bin/bisheng",
                "-I${env:ASCEND_HOME_PATH}/aarch64-linux/asc/include",
                "-I${env:ASCEND_HOME_PATH}/aarch64-linux/ascendc/act",
                "${resource:resources/src/k.cpp}",
                "-o",
                "${output:artifacts/k.o}",
            ],
        )

    def test_normalized_commands_are_stable_across_cann_roots(self):
        suffix = "/aarch64-linux/asc/include"
        first = normalize_static_command(
            "compile", ["-I/opt/cann-a" + suffix, "-c"], "/opt/cann-a", 0
        )
        second = normalize_static_command(
            "compile", ["-I/srv/cann-b" + suffix, "-c"], "/srv/cann-b", 0
        )
        self.assertEqual(first, second)

    def test_rejects_unclassified_absolute_command_path(self):
        with self.assertRaisesRegex(
            ValueError, r"command\[3\]\[1\].*non-portable absolute path"
        ):
            normalize_static_command(
                "link", ["-o", "/tmp/generated/kernel.o"], "/opt/cann", 3
            )
        with self.assertRaisesRegex(ValueError, "non-portable absolute path"):
            normalize_static_command(
                "compile", ["--sysroot=/opt/cann-other"], "/opt/cann", 0
            )

    def test_rejects_dot_segments_in_logical_resource_paths(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("source\n")
            with self.assertRaisesRegex(ValueError, "stay inside"):
                generate_static_compile_resources(
                    output_root=root,
                    kernel_base="Kernel_x",
                    resource_id="kernel",
                    resources=[{
                        "source": source,
                        "path": "resources/src/./kernel.cpp",
                    }],
                    kernels=[{
                        "kernel_name": "kernel",
                        "recipes": {"normal": {
                            "commands": [["cp", "${resource:resources/src/kernel.cpp}",
                                          "${output:artifacts/kernel.o}"]],
                            "artifact": "artifacts/kernel.o",
                        }},
                    }],
                )

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
        self.assertIn("__ascendc_static_tiling_data", result)
        self.assertIn(
            "static constexpr uint8_t __ascendc_static_tiling_data[] = "
            "@@STATIC_VALUE_tiling_data@@;",
            result,
        )
        self.assertIn("#define GET_TILING_DATA_WITH_STRUCT", result)
        self.assertIn("#define GET_TILING_DATA_MEMBER", result)
        self.assertEqual(result.count("@@STATIC_VALUE_tiling_data@@"), 1)
        self.assertNotIn("\\\n\n", result)

    def test_adapts_struct_and_member_tiling_without_concrete_default_type(self):
        dynamic_header = """#ifndef TILING_H
#define TILING_H
#define GET_TILING_DATA(name, arg)
#define GET_TILING_DATA_WITH_STRUCT(type, name, arg) InitTilingData<type>(arg, &name)
#define GET_TILING_DATA_MEMBER(type, member, name, arg) InitTilingData<type>(arg, &name)
#endif
"""
        result = static_tiling_template_text(dynamic_header)
        self.assertEqual(result.count("@@STATIC_VALUE_tiling_data@@"), 1)
        self.assertIn("convert_from_bytes<tiling_struct>", result)
        self.assertIn("convert_from_bytes<tiling_type>", result)
        self.assertNotIn("static_assert(false", result)
        self.assertIn("#define GET_TILING_DATA(tiling_data, tiling_arg)", result)

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
            ["linked_actual", "actual_7"],
        )
        self.assertEqual(
            collect_kernel_symbols(metadata),
            ["actual_7", "actual_9"],
        )
        self.assertEqual(
            collect_kernel_symbols({"kernelName": "single_entry"}),
            ["single_entry"],
        )

    def test_resolves_exact_tiling_size_without_using_tiling_key_as_index(self):
        self.assertEqual(
            resolve_symbol_tiling_sizes(
                ["add_7", "add_9"],
                ["7", "9"], {"7": 32, "9": 48}, 64,
            ),
            {"add_7": 32, "add_9": 48},
        )
        self.assertEqual(
            resolve_symbol_tiling_sizes(["single_entry"], ["7"], {}, 64),
            {"single_entry": 64},
        )

    def test_rejects_missing_exact_size_for_multi_tiling_symbol(self):
        with self.assertRaisesRegex(ValueError, "exact tiling byte size"):
            resolve_symbol_tiling_sizes(
                ["add_7", "add_9"], ["7", "9"], {"7": 32}, 64
            )

    def test_generates_minimal_direct_v4_manifest(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("@@STATIC_VALUE_tiling_data@@\n")

            result = generate_static_compile_resources(
                output_root=root,
                kernel_base="AddCustom_xxx",
                resource_id="add_custom",
                resources=[
                    {"source": source, "path": "resources/src/kernel.cpp"}
                ],
                kernels=[
                    {
                        "kernel_name": "add_custom_0",
                        "targets": [
                            {
                                "name": "tiling_data",
                                "required": True,
                                "parameter_index": 4,
                                "byte_size": 8,
                            }
                        ],
                        "patches": [{
                            "file": "resources/src/kernel.cpp",
                            "token": "@@STATIC_VALUE_tiling_data@@",
                            "value": "tiling_data",
                            "format": "cpp_uint8_array",
                        }],
                        "recipes": {
                            "normal": {
                                "commands": [[
                                    "${env:ASCEND_HOME_PATH}/bin/bisheng",
                                    "-DDTYPE_X=half",
                                    "${resource:resources/src/kernel.cpp}",
                                    "-o", "${output:artifacts/add_custom_0.o}",
                                ]],
                                "artifact": "artifacts/add_custom_0.o",
                            }
                        },
                    }
                ],
            )

            manifest_path = os.path.join(result, "manifest.json")
            with open(manifest_path, "r", encoding="utf-8") as file_obj:
                manifest = json.load(file_obj)
            self.assertEqual(
                set(manifest), {"schema_version", "resource_id", "kernels"}
            )
            self.assertEqual(manifest["schema_version"], "4.0")
            self.assertEqual(manifest["resource_id"], "add_custom")
            kernel = manifest["kernels"][0]
            self.assertEqual(
                set(kernel), {"kernel_name", "targets", "patches", "recipes"}
            )
            self.assertEqual(kernel["kernel_name"], "add_custom_0")
            self.assertEqual(kernel["targets"][0]["parameter_index"], 4)
            self.assertEqual(kernel["targets"][0]["byte_size"], 8)
            self.assertNotIn("required", kernel["targets"][0])
            self.assertNotIn("format", kernel["patches"][0])
            serialized = json.dumps(manifest)
            for forbidden in (
                "macros", "architecture", "kernel_type", "build",
                "binary", "filePath", "contract_version", "sha256",
            ):
                self.assertNotIn(forbidden, serialized)
            copied = os.path.join(result, "resources/src/kernel.cpp")
            self.assertTrue(os.path.isfile(copied))

    def test_rejects_unknown_fields_and_missing_command_resources(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("source\n")
            kernel = {
                "kernel_name": "kernel",
                "recipes": {"normal": {
                    "commands": [["cp", "${resource:resources/src/missing.cpp}",
                                  "${output:artifacts/kernel.o}"]],
                    "artifact": "artifacts/kernel.o",
                }},
            }
            with self.assertRaisesRegex(ValueError, "missing resource"):
                generate_static_compile_resources(
                    output_root=root, kernel_base="Kernel_x", resource_id="kernel",
                    resources=[{"source": source, "path": "resources/src/kernel.cpp"}],
                    kernels=[kernel],
                )
            kernel["recipes"]["normal"]["commands"][0][1] = (
                "${resource:resources/src/kernel.cpp}"
            )
            kernel["recipes"]["normal"]["commands"][0].append(
                "-I${env:ASCEND_HOME_PATH}/../host/include"
            )
            with self.assertRaisesRegex(ValueError, "stay inside"):
                generate_static_compile_resources(
                    output_root=root, kernel_base="Kernel_x", resource_id="kernel",
                    resources=[{"source": source, "path": "resources/src/kernel.cpp"}],
                    kernels=[kernel],
                )
            kernel["recipes"]["normal"]["commands"][0].pop()
            missing_name = dict(kernel)
            missing_name.pop("kernel_name")
            with self.assertRaisesRegex(ValueError, "kernel_name"):
                generate_static_compile_resources(
                    output_root=root, kernel_base="Kernel_x", resource_id="kernel",
                    resources=[{"source": source, "path": "resources/src/kernel.cpp"}],
                    kernels=[missing_name],
                )
            kernel["architecture"] = "dav-c220-vec"
            with self.assertRaisesRegex(ValueError, "unknown fields"):
                generate_static_compile_resources(
                    output_root=root, kernel_base="Kernel_x", resource_id="kernel",
                    resources=[{"source": source, "path": "resources/src/kernel.cpp"}],
                    kernels=[kernel],
                )

    def test_cleans_staging_directory_when_resource_copy_fails(self):
        with tempfile.TemporaryDirectory() as root:
            with self.assertRaises(FileNotFoundError):
                generate_static_compile_resources(
                    output_root=root,
                    kernel_base="AddCustom_xxx",
                    resource_id="add_custom",
                    resources=[
                        {"source": os.path.join(root, "missing.cpp"),
                         "path": "resources/src/kernel.cpp"}
                    ],
                    kernels=[],
                )
            static_root = os.path.join(root, "static_compile")
            self.assertFalse(os.path.exists(os.path.join(static_root, "AddCustom_xxx")))
            if os.path.isdir(static_root):
                self.assertEqual(os.listdir(static_root), [])


if __name__ == "__main__":
    unittest.main()
