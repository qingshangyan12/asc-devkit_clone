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

    def test_rejects_quoted_includes_outside_source_root(self):
        with tempfile.TemporaryDirectory() as root:
            source_root = os.path.join(root, "src")
            os.makedirs(source_root)
            source = os.path.join(source_root, "kernel.cpp")
            external = os.path.join(root, "common.h")
            with open(external, "w", encoding="utf-8") as file_obj:
                file_obj.write("common\n")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write('#include "../common.h"\n')
            with self.assertRaisesRegex(ValueError, "outside source root"):
                collect_local_include_files(source)

    def test_rejects_quoted_include_symlink_escape(self):
        with tempfile.TemporaryDirectory() as root:
            source_root = os.path.join(root, "src")
            os.makedirs(source_root)
            source = os.path.join(source_root, "kernel.cpp")
            external = os.path.join(root, "common.h")
            with open(external, "w", encoding="utf-8") as file_obj:
                file_obj.write("common\n")
            os.symlink(external, os.path.join(source_root, "common.h"))
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write('#include "common.h"\n#include <../system.h>\n')
            with self.assertRaisesRegex(ValueError, "outside source root"):
                collect_local_include_files(source)

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

    def test_canonicalizes_cann_paths_before_classifying_ownership(self):
        with tempfile.TemporaryDirectory() as root:
            cann_root = os.path.join(root, "cann")
            os.makedirs(cann_root)
            parent_spelling = os.path.join(
                cann_root, "..", "cann", "missing", "include"
            )
            self.assertEqual(
                normalize_static_command(
                    "compile",
                    [
                        "-I" + parent_spelling,
                        "-include" + os.path.join(
                            cann_root, "..", "cann", "missing", "version.h"
                        ),
                        "--sysroot=" + os.path.join(
                            cann_root, "..", "cann", "sysroot"
                        ),
                    ],
                    cann_root,
                    2,
                ),
                [
                    "${env:ASCEND_HOME_PATH}/bin/bisheng",
                    "-I${env:ASCEND_HOME_PATH}/missing/include",
                    "-include${env:ASCEND_HOME_PATH}/missing/version.h",
                    "--sysroot=${env:ASCEND_HOME_PATH}/sysroot",
                ],
            )

    def test_canonicalizes_alternate_symlink_spelling_of_cann_root(self):
        with tempfile.TemporaryDirectory() as root:
            actual = os.path.join(root, "actual")
            first_alias = os.path.join(root, "cann-current")
            second_alias = os.path.join(root, "toolkit-current")
            os.makedirs(os.path.join(actual, "include"))
            os.symlink(actual, first_alias)
            os.symlink(actual, second_alias)
            self.assertEqual(
                normalize_static_command(
                    "compile",
                    ["-I" + os.path.join(second_alias, "include")],
                    first_alias,
                    0,
                ),
                [
                    "${env:ASCEND_HOME_PATH}/bin/bisheng",
                    "-I${env:ASCEND_HOME_PATH}/include",
                ],
            )

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

    def test_matches_cpp_validator_absolute_argument_forms(self):
        rejected = [
            "-isystem/tmp/include",
            "-idirafter/tmp/include",
            "-isysroot/tmp/sysroot",
            "-iquote/tmp/include",
            "-imacros/tmp/macros.h",
            "-iprefix/tmp/prefix",
            "-iwithprefix/tmp/include",
            "-iwithprefixbefore/tmp/include",
            "-MF/tmp/deps.d",
            "-B/tmp/toolchain",
            "--gcc-toolchain=/tmp/toolchain",
            "@/tmp/args.rsp",
            "-Wl,-rpath,/tmp/lib",
            "-Wa,/tmp/input.s",
        ]
        for argument in rejected:
            with self.subTest(argument=argument):
                with self.assertRaisesRegex(ValueError, "non-portable absolute path"):
                    normalize_static_command("compile", [argument], "/opt/cann", 0)
        ordinary_comma = "s,/tmp/source,relative/source,g"
        self.assertEqual(
            normalize_static_command("compile", [ordinary_comma], "/opt/cann", 0),
            ["${env:ASCEND_HOME_PATH}/bin/bisheng", ordinary_comma],
        )

    def test_normalizes_cpp_validator_path_forms_owned_by_cann(self):
        with tempfile.TemporaryDirectory() as root:
            cann_root = os.path.join(root, "cann")
            os.makedirs(cann_root)
            owned = os.path.join(cann_root, "missing", "path")
            command = normalize_static_command(
                "compile",
                [
                    "-isystem" + owned,
                    "-include", owned,
                    "--gcc-toolchain=" + owned,
                    "@" + owned,
                    "-Wl,-rpath," + owned,
                    "-Wa," + owned,
                ],
                cann_root,
                0,
            )
            placeholder = "${env:ASCEND_HOME_PATH}/missing/path"
            self.assertEqual(
                command,
                [
                    "${env:ASCEND_HOME_PATH}/bin/bisheng",
                    "-isystem" + placeholder,
                    "-include", placeholder,
                    "--gcc-toolchain=" + placeholder,
                    "@" + placeholder,
                    "-Wl,-rpath," + placeholder,
                    "-Wa," + placeholder,
                ],
            )

    def test_normalizes_compound_cann_paths_without_hiding_host_paths(self):
        with tempfile.TemporaryDirectory() as root:
            cann_root = os.path.join(root, "cann")
            host_root = os.path.join(root, "host")
            os.makedirs(cann_root)
            cann_lib = os.path.join(cann_root, "lib")
            cann_include = os.path.join(cann_root, "include")
            host_lib = os.path.join(host_root, "lib")
            normalized = normalize_static_command(
                "compile",
                [
                    f"-Wl,-rpath,{cann_lib}:{cann_include}",
                    f"-DSEARCH={cann_include}:{cann_lib}",
                ],
                cann_root,
                0,
            )
            placeholder = "${env:ASCEND_HOME_PATH}"
            self.assertEqual(
                normalized,
                [
                    placeholder + "/bin/bisheng",
                    f"-Wl,-rpath,{placeholder}/lib:{placeholder}/include",
                    f"-DSEARCH={placeholder}/include:{placeholder}/lib",
                ],
            )
            for argument in (
                f"-Wl,-rpath,{cann_lib}:{host_lib}",
                f"-DSEARCH={cann_include}:{host_lib}",
            ):
                with self.subTest(argument=argument):
                    with self.assertRaisesRegex(
                        ValueError, "non-portable absolute path"
                    ):
                        normalize_static_command(
                            "compile", [argument], cann_root, 0
                        )
            ordinary = "label:/tmp/host,relative:value"
            self.assertEqual(
                normalize_static_command("compile", [ordinary], cann_root, 0),
                [placeholder + "/bin/bisheng", ordinary],
            )

    def test_normalizes_compound_paths_after_environment_placeholders(self):
        with tempfile.TemporaryDirectory() as root:
            cann_root = os.path.join(root, "cann")
            host_root = os.path.join(root, "host")
            os.makedirs(cann_root)
            cann_include = os.path.join(cann_root, "include")
            host_include = os.path.join(host_root, "include")
            env_path = "${env:ASCEND_HOME_PATH}/include"
            alt_path = "${env:ALT_HOME}/include"
            placeholder = "${env:ASCEND_HOME_PATH}"
            self.assertEqual(
                normalize_static_command(
                    "compile",
                    [
                        f"-DSEARCH={env_path}:{cann_include}",
                        f"-I{env_path}:{cann_include}",
                        f"-DALL={env_path}:{alt_path}",
                        f"-I{env_path}:{alt_path}",
                    ],
                    cann_root,
                    0,
                ),
                [
                    placeholder + "/bin/bisheng",
                    f"-DSEARCH={env_path}:{placeholder}/include",
                    f"-I{env_path}:{placeholder}/include",
                    f"-DALL={env_path}:{alt_path}",
                    f"-I{env_path}:{alt_path}",
                ],
            )
            for argument in (
                f"-DSEARCH={env_path}:{host_include}",
                f"-I{env_path}:{host_include}",
                f"-Wl,-rpath,{env_path}:{host_include}",
                "-Wl,-rpath,https://example.com/lib",
            ):
                with self.subTest(argument=argument):
                    with self.assertRaisesRegex(
                        ValueError, "non-portable absolute path"
                    ):
                        normalize_static_command(
                            "compile", [argument], cann_root, 0
                        )
            accepted = [
                "-DURL=https://example.com/toolchain",
                "-Wl,-plugin-opt=https://example.com/toolchain",
                "label:/tmp/host,relative:value",
            ]
            self.assertEqual(
                normalize_static_command("compile", accepted, cann_root, 0),
                [placeholder + "/bin/bisheng"] + accepted,
            )

    def test_normalizes_nested_response_and_joined_comma_domain_paths(self):
        with tempfile.TemporaryDirectory() as root:
            cann_root = os.path.join(root, "cann")
            os.makedirs(cann_root)
            placeholder = "${env:ASCEND_HOME_PATH}"
            owned = os.path.join(cann_root, "owned")
            self.assertEqual(
                normalize_static_command(
                    "compile",
                    [
                        f"-Wl,@{owned}", f"-Wl,-T{owned}",
                        f"-Wl,-R{owned}", f"-Wa,-I{owned}",
                    ],
                    cann_root,
                    0,
                ),
                [
                    placeholder + "/bin/bisheng",
                    f"-Wl,@{placeholder}/owned",
                    f"-Wl,-T{placeholder}/owned",
                    f"-Wl,-R{placeholder}/owned",
                    f"-Wa,-I{placeholder}/owned",
                ],
            )
            for argument in (
                "-Wl,@/tmp/link.rsp",
                "-Wl,-T/tmp/script.ld",
                "-Wl,-R/tmp/lib",
                "-Wa,-I/tmp/include",
            ):
                with self.subTest(argument=argument):
                    with self.assertRaisesRegex(
                        ValueError, "non-portable absolute path"
                    ):
                        normalize_static_command(
                            "compile", [argument], cann_root, 0
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

    def test_rejects_boolean_tiling_sizes(self):
        with self.assertRaisesRegex(ValueError, "fallback tiling size"):
            resolve_symbol_tiling_sizes(["add"], ["0"], {}, True)
        with self.assertRaisesRegex(ValueError, "exact tiling byte size"):
            resolve_symbol_tiling_sizes(
                ["add_7", "add_9"], ["7", "9"], {"7": True, "9": 32}, 64
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

    def test_rejects_boolean_target_integer_fields(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("source\n")
            for field, value in (
                ("parameter_index", True),
                ("parameter_index", False),
                ("byte_size", True),
                ("byte_size", False),
            ):
                with self.subTest(field=field, value=value):
                    target = {
                        "name": "tiling_data",
                        "parameter_index": 4,
                        "byte_size": 8,
                    }
                    target[field] = value
                    with self.assertRaisesRegex(
                        ValueError, rf"targets\[0\]\.{field}"
                    ):
                        generate_static_compile_resources(
                            output_root=root,
                            kernel_base="Kernel_x",
                            resource_id="kernel",
                            resources=[{
                                "source": source,
                                "path": "resources/src/kernel.cpp",
                            }],
                            kernels=[{
                                "kernel_name": "kernel",
                                "targets": [target],
                                "recipes": {"normal": {
                                    "commands": [[
                                        "cp",
                                        "${resource:resources/src/kernel.cpp}",
                                        "${output:artifacts/kernel.o}",
                                    ]],
                                    "artifact": "artifacts/kernel.o",
                                }},
                            }],
                        )

    def test_enforces_cpp_schema_collection_limits(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("source\n")

            def generate(*, targets=None, commands=None, patches_marker=None):
                kernel = {
                    "kernel_name": "kernel",
                    "recipes": {"normal": {
                        "commands": commands or [["tool"]],
                        "artifact": "artifacts/kernel.o",
                    }},
                }
                if targets is not None:
                    kernel["targets"] = targets
                if patches_marker == "kernel":
                    kernel["patches"] = []
                elif patches_marker == "recipe":
                    kernel["recipes"]["normal"]["patches"] = []
                return generate_static_compile_resources(
                    output_root=root,
                    kernel_base="Kernel_x",
                    resource_id="kernel",
                    resources=[{
                        "source": source,
                        "path": "resources/src/kernel.cpp",
                    }],
                    kernels=[kernel],
                )

            targets = [
                {"name": f"target_{index}", "parameter_index": index, "byte_size": 1}
                for index in range(256)
            ]
            self.assertTrue(os.path.isdir(generate(targets=targets)))
            for invalid_targets in ([], targets + [{
                "name": "overflow", "parameter_index": 256, "byte_size": 1,
            }]):
                with self.subTest(target_count=len(invalid_targets)):
                    with self.assertRaisesRegex(ValueError, "targets.*bounded non-empty"):
                        generate(targets=invalid_targets)

            max_bytes = 16 * 1024 * 1024
            self.assertTrue(os.path.isdir(generate(targets=[{
                "name": "tiling_data", "parameter_index": 0,
                "byte_size": max_bytes,
            }])))
            with self.assertRaisesRegex(ValueError, "byte_size.*supported range"):
                generate(targets=[{
                    "name": "tiling_data", "parameter_index": 0,
                    "byte_size": max_bytes + 1,
                }])

            self.assertTrue(os.path.isdir(generate(commands=[["tool"]] * 64)))
            with self.assertRaisesRegex(ValueError, "commands.*bounded non-empty"):
                generate(commands=[["tool"]] * 65)
            self.assertTrue(os.path.isdir(generate(commands=[["arg"] * 256])))
            with self.assertRaisesRegex(ValueError, "bounded non-empty argv"):
                generate(commands=[["arg"] * 257])
            for owner in ("kernel", "recipe"):
                with self.subTest(empty_patches_owner=owner):
                    with self.assertRaisesRegex(ValueError, "patches.*non-empty"):
                        generate(patches_marker=owner)

    def test_rejects_integer_values_outside_cpp_int64(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("source\n")

            def generate(target):
                return generate_static_compile_resources(
                    output_root=root,
                    kernel_base="Kernel_x",
                    resource_id="kernel",
                    resources=[{
                        "source": source,
                        "path": "resources/src/kernel.cpp",
                    }],
                    kernels=[{
                        "kernel_name": "kernel",
                        "targets": [target],
                        "recipes": {"normal": {
                            "commands": [["tool"]],
                            "artifact": "artifacts/kernel.o",
                        }},
                    }],
                )

            self.assertTrue(os.path.isdir(generate({
                "name": "tiling_data",
                "parameter_index": (1 << 63) - 1,
                "byte_size": 1,
            })))
            for field in ("parameter_index", "byte_size"):
                target = {
                    "name": "tiling_data",
                    "parameter_index": 0,
                    "byte_size": 1,
                }
                target[field] = 2 ** 80
                with self.subTest(field=field):
                    with self.assertRaisesRegex(ValueError, field):
                        generate(target)

    def test_rejects_reserved_machine_manifest_references(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "manifest.json")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("{}\n")

            def generate(*, resource_path, argument="tool", patches=None):
                kernel = {
                    "kernel_name": "kernel",
                    "targets": [{
                        "name": "tiling_data",
                        "parameter_index": 0,
                        "byte_size": 1,
                    }],
                    "recipes": {"normal": {
                        "commands": [[argument]],
                        "artifact": "artifacts/kernel.o",
                    }},
                }
                if patches is not None:
                    kernel["patches"] = patches
                return generate_static_compile_resources(
                    output_root=root,
                    kernel_base="Kernel_x",
                    resource_id="kernel",
                    resources=[{
                        "source": source,
                        "path": resource_path,
                    }],
                    kernels=[kernel],
                )

            for resource_path in (".sc/manifest.json", "manifest.json"):
                with self.subTest(resource_path=resource_path, owner="command"):
                    with self.assertRaisesRegex(ValueError, "reserved.*manifest"):
                        generate(
                            resource_path=resource_path,
                            argument="${resource:" + resource_path + "}",
                        )
                with self.subTest(resource_path=resource_path, owner="patch"):
                    with self.assertRaisesRegex(ValueError, "reserved.*manifest"):
                        generate(resource_path=resource_path, patches=[{
                            "file": resource_path,
                            "token": "@@TILING@@",
                            "value": "tiling_data",
                        }])

    def test_rejects_any_suffix_after_resource_or_output_placeholder(self):
        with tempfile.TemporaryDirectory() as root:
            source = os.path.join(root, "kernel.cpp")
            with open(source, "w", encoding="utf-8") as file_obj:
                file_obj.write("source\n")
            for argument in (
                "${resource:resources/src/kernel.cpp}.bak",
                "${output:artifacts/kernel.o}-suffix",
            ):
                with self.subTest(argument=argument):
                    with self.assertRaisesRegex(ValueError, "path-changing suffix"):
                        generate_static_compile_resources(
                            output_root=root,
                            kernel_base="Kernel_x",
                            resource_id="kernel",
                            resources=[{
                                "source": source,
                                "path": "resources/src/kernel.cpp",
                            }],
                            kernels=[{
                                "kernel_name": "kernel",
                                "recipes": {"normal": {
                                    "commands": [["tool", argument]],
                                    "artifact": "artifacts/kernel.o",
                                }},
                            }],
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
