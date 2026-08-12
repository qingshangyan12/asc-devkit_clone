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

from contextlib import ExitStack
import json
import os
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock


MODULE_DIR = os.path.realpath(
    os.path.join(
        os.path.dirname(__file__),
        "../../../tools/build/asc_op_compile_base/asc_op_compiler",
    )
)
sys.path.insert(0, MODULE_DIR)
import static_compile_resource_generator as generator


class TestStaticCompileResourceGenerator(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp_dir.cleanup)
        self.root = self.temp_dir.name
        self.input_root = os.path.join(self.root, "inputs")
        self.output_root = os.path.join(self.root, "output")
        self.cann_root = os.path.join(self.root, "cann")
        os.makedirs(os.path.join(self.cann_root, "bin"))
        os.makedirs(os.path.join(self.cann_root, "include"))
        self.bisheng = os.path.join(self.cann_root, "bin", "bisheng")
        Path(self.bisheng).touch()
        self.env_patch = mock.patch.dict(
            os.environ, {"ASCEND_HOME_PATH": self.cann_root}, clear=False
        )
        self.env_patch.start()
        self.addCleanup(self.env_patch.stop)

        os.makedirs(self.input_root)
        self.source_path = os.path.join(self.input_root, "add_custom.cpp")
        Path(self.source_path).write_text(
            'extern "C" void add_custom() {}\n', encoding="utf-8"
        )
        self.wrapper_path = os.path.join(self.input_root, "add_custom_kernel.cpp")
        Path(self.wrapper_path).write_text(
            "#ifndef ADD_CUSTOM_KERNEL\n"
            "#define ADD_CUSTOM_KERNEL\n"
            f'#include "{self.source_path}"\n'
            'extern "C" void auto_gen_add_custom_kernel() { add_custom(); }\n'
            "#endif\n",
            encoding="utf-8",
        )
        self.sk_wrapper_path = os.path.join(self.input_root, "add_custom_sk_kernel.cpp")
        Path(self.sk_wrapper_path).write_text(
            "#ifndef ADD_CUSTOM_SK_KERNEL\n"
            "#define ADD_CUSTOM_SK_KERNEL\n"
            f'#include "{self.source_path}"\n'
            'extern "C" void auto_gen_add_custom_kernel() { add_custom(); }\n'
            "#endif\n",
            encoding="utf-8",
        )
        self.tiling_path = os.path.join(self.input_root, "add_custom_tiling_data.h")
        Path(self.tiling_path).write_text(
            "#ifndef ADD_CUSTOM_TILING_DATA_H\n"
            "#define ADD_CUSTOM_TILING_DATA_H\n"
            "#define GET_TILING_DATA(tiling_data, tiling_arg) int tiling_data = 0\n"
            "#endif\n",
            encoding="utf-8",
        )
        self.dynamic_object = os.path.join(self.input_root, "AddCustom.o")
        Path(self.dynamic_object).write_bytes(b"\x7fELFtest-object")
        self.metadata_path = os.path.join(self.input_root, "AddCustom.json")
        Path(self.metadata_path).write_text(
            json.dumps({"kernelList": [{"kernelName": "add_custom_100000"}]}),
            encoding="utf-8",
        )
        self.resource_id = "a" * 64

    def make_compile_context(
        self,
        *,
        kernel_name="add_custom",
        code_channel=2,
        kernel_types=None,
        wrapper_path=None,
        symbols=None,
    ):
        return SimpleNamespace(
            kernel_name=kernel_name,
            origin_func_name="add_custom",
            code_channel=code_channel,
            hard_sync=False,
            no_set_kernel_type=kernel_types is None,
            tiling_key_list=["100000"],
            tiling_key_kernel_type=kernel_types or {},
            raw_tiling_key_kernel_type=kernel_types or {},
            tiling_key_group_map={},
            gen_kernel_func_file=wrapper_path or self.wrapper_path,
            dst_file=self.dynamic_object,
            global_kernel_symbols=list(symbols or []),
            global_kernel_attribute="__global__ [aicore]",
        )

    def fake_command_builder(self, compile_info, options, tiling_path, chip_version):
        if compile_info.no_set_kernel_type:
            core_types = (
                ("cube", "vec")
                if compile_info.code_channel == 0
                else ("cube" if compile_info.code_channel == 1 else "vec",)
            )
        else:
            kernel_type = compile_info.tiling_key_kernel_type["100000"].name
            core_types = ("cube",) if "AIC_ONLY" in kernel_type else ("vec",)
        commands = []
        for index, core_type in enumerate(core_types):
            suffix = ""
            if len(core_types) > 1:
                suffix = "_mix_aic" if core_type == "cube" else "_mix_aiv"
            compiled_symbol = (
                compile_info.global_kernel_symbols[index]
                if compile_info.global_kernel_symbols
                else f"{compile_info.kernel_name}_100000{suffix}"
            )
            output_path = os.path.join(self.input_root, f"static_{core_type}.o")
            argv = [
                self.bisheng,
                "-c",
                "-O3",
                *options.compile_options,
                "-DCONST_TILING",
                compile_info.gen_kernel_func_file,
                f"--cce-aicore-arch=dav-{chip_version}-{core_type}",
                "--cce-aicore-only",
                "-include",
                tiling_path,
                f"-Dauto_gen_add_custom_kernel={compiled_symbol}",
                "-DTILING_KEY_VAR=100000UL",
                "-Dadd_custom=add_custom_100000_tilingkey",
                "-o",
                output_path,
            ]
            raw_kernel_type = compile_info.raw_tiling_key_kernel_type.get(
                "100000",
                compile_info.tiling_key_kernel_type.get("100000"),
            )
            if (
                chip_version == "c310"
                and getattr(raw_kernel_type, "name", "") == "KERNEL_TYPE_AIC_ONLY"
            ):
                argv.append("-DRAW_AIC_ONLY_DUMP_TENSOR")
            commands.append(
                SimpleNamespace(
                    tiling_key="100000",
                    compiled_symbol=compiled_symbol,
                    core_type=core_type,
                    source_path=os.path.realpath(compile_info.gen_kernel_func_file),
                    tiling_path=os.path.realpath(tiling_path),
                    argv=tuple(argv),
                    output_path=output_path,
                )
            )
        return tuple(commands)

    def make_options(self):
        return SimpleNamespace(
            compile_options=["-I" + os.path.join(self.cann_root, "include")],
            mllvm_options=[],
        )

    def make_records(
        self,
        *,
        basic_context=None,
        basic_options=None,
        sk_context=None,
        sk_options=None,
        chip_version="c220",
        tiling_key_data_size=None,
        default_tiling_size=16,
        tiling_key_group_map=None,
    ):
        return generator.build_kernel_compile_records(
            basic_compile_info=basic_context or self.make_compile_context(),
            basic_compile_option_tuple=basic_options or self.make_options(),
            sk_compile_info=sk_context,
            sk_compile_option_tuple=sk_options,
            command_builder=self.fake_command_builder,
            chip_version=chip_version,
            metadata_json_path=self.metadata_path,
            parameter_index=4,
            tiling_template_path=self.tiling_path,
            tiling_key_data_size=(
                {"100000": 8} if tiling_key_data_size is None else tiling_key_data_size
            ),
            default_tiling_size=default_tiling_size,
            tiling_key_group_map=tiling_key_group_map or {},
        )

    def make_request(
        self,
        records,
        *,
        sk_cap_bitmap=None,
        sk_kernel_attribute="",
        basic_kernel_attribute="",
        sk_bind_compile_options=(),
    ):
        return generator.ManifestGenerationRequest(
            soc_version="Ascend910B",
            dynamic_object_path=self.dynamic_object,
            resource_id=self.resource_id,
            metadata_json_path=self.metadata_path,
            source_file_path=self.source_path,
            origin_func_name="add_custom",
            output_root=self.output_root,
            kernels=tuple(records),
            chip_version="c220",
            sk_cap_bitmap=sk_cap_bitmap,
            sk_kernel_attribute=sk_kernel_attribute,
            basic_kernel_attribute=basic_kernel_attribute,
            sk_bind_compile_options=tuple(sk_bind_compile_options),
        )

    def generate(self, request, exported_symbols):
        with ExitStack() as stack:
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "read_resource_id_section",
                    return_value=self.resource_id,
                )
            )
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "_read_exported_symbols",
                    return_value=set(exported_symbols),
                )
            )
            return generator.generate_manifest(request)

    def test_manifest_generation_does_not_probe_dependencies(self):
        request = self.make_request(self.make_records())

        with ExitStack() as stack:
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "read_resource_id_section",
                    return_value=self.resource_id,
                )
            )
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "_read_exported_symbols",
                    return_value={"add_custom_100000"},
                )
            )
            run = stack.enter_context(mock.patch.object(generator.subprocess, "run"))
            generator.generate_manifest(request)

        run.assert_not_called()

    def test_static_commands_are_rebuilt_from_compile_context(self):
        mix_records = self.make_records(
            basic_context=self.make_compile_context(code_channel=0)
        )
        self.assertEqual(
            [command.core_type for command in mix_records[0].commands],
            ["cube", "vec"],
        )
        for kernel_type_name, expected_core in (
            ("KERNEL_TYPE_AIC_ONLY", "cube"),
            ("KERNEL_TYPE_AIV_ONLY", "vec"),
        ):
            kernel_type = SimpleNamespace(name=kernel_type_name)
            records = self.make_records(
                basic_context=self.make_compile_context(
                    kernel_types={"100000": kernel_type}
                )
            )
            self.assertEqual(
                [command.core_type for command in records[0].commands],
                [expected_core],
            )
        c310_context = self.make_compile_context(
            kernel_types={"100000": SimpleNamespace(name="KERNEL_TYPE_AIC_ONLY")}
        )
        c310_context.raw_tiling_key_kernel_type = {}
        c310_records = self.make_records(
            basic_context=c310_context, chip_version="c310"
        )
        self.assertIn("-DRAW_AIC_ONLY_DUMP_TENSOR", c310_records[0].commands[0].argv)
        self.assertFalse(hasattr(generator, "DynamicCompileRecorder"))
        self.assertFalse(hasattr(generator, "capture_compile_command"))

    def test_basic_mix_manifest_uses_static_wrapper_and_tiling_template(self):
        records = self.make_records(
            basic_context=self.make_compile_context(code_channel=0),
        )
        Path(self.wrapper_path).write_text(
            Path(self.wrapper_path).read_text(encoding="utf-8")
            + "// wrapper content read during Manifest generation\n",
            encoding="utf-8",
        )
        result = self.generate(
            self.make_request(records),
            {"add_custom_100000_mix_aic", "add_custom_100000_mix_aiv"},
        )

        manifest = json.loads(Path(result.manifest_path).read_text(encoding="utf-8"))
        self.assertEqual(
            set(manifest),
            {
                "schema_version",
                "soc_version",
                "resource_id",
                "source_file",
                "resource_path",
                "option",
                "kernels",
            },
        )
        self.assertEqual(manifest["schema_version"], "1.0")
        self.assertEqual(manifest["source_file"], "add_custom.cpp")
        self.assertEqual(manifest["resource_path"], "resources")
        self.assertEqual(manifest["option"]["common_compile"][:2], ["-c", "-O3"])

        kernel = manifest["kernels"][0]
        static_input = kernel["static_inputs"][0]
        self.assertEqual(static_input["token"], "@@STATIC_VALUE_tiling_data@@")
        template_path = os.path.join(
            self.output_root, "manifest", "resources", static_input["file"]
        )
        template_text = Path(template_path).read_text(encoding="utf-8")
        self.assertEqual(template_text.count("@@STATIC_VALUE_tiling_data@@"), 1)

        basic = kernel["objects"][0]
        self.assertEqual(basic["obj_name"], "basic")
        self.assertEqual(basic["obj_type"], "basic")
        self.assertEqual(
            basic["outputs"],
            [
                "${output:add_custom_100000.aic.o}",
                "${output:add_custom_100000.aiv.o}",
            ],
        )
        serialized = json.dumps(manifest)
        self.assertNotIn("intermediates", serialized)
        self.assertNotIn(self.root, serialized)
        resource_root = os.path.join(self.output_root, "manifest", "resources")
        self.assertEqual(
            {
                os.path.relpath(path, resource_root)
                for path in Path(resource_root).rglob("*")
                if path.is_file()
            },
            {
                "src/add_custom.cpp",
                "src/add_custom_kernel.cpp",
                "include/add_custom_tiling_data.h",
            },
        )

        for command in basic["commands"]:
            self.assertEqual(command["type"], "compile")
            self.assertEqual(command["stage"], 0)
            expanded_cmd = list(command["cmd"])
            option_index = expanded_cmd.index("${option:common_compile}")
            expanded_cmd[option_index : option_index + 1] = manifest["option"][
                "common_compile"
            ]
            self.assertIn("-DCONST_TILING", expanded_cmd)
            include_index = expanded_cmd.index("-include")
            self.assertEqual(
                expanded_cmd[include_index + 1],
                "${resource:" + static_input["file"] + "}",
            )

        wrapper_reference = next(
            arg
            for arg in basic["commands"][0]["cmd"]
            if arg.startswith("${resource:src/")
        )
        wrapper_path = os.path.join(
            self.output_root,
            "manifest",
            "resources",
            wrapper_reference[len("${resource:") : -1],
        )
        wrapper_text = Path(wrapper_path).read_text(encoding="utf-8")
        self.assertNotIn("@@STATIC_VALUE_tiling_data@@", wrapper_text)
        self.assertNotIn(self.source_path, wrapper_text)
        self.assertIn('#include "add_custom.cpp"', wrapper_text)
        self.assertIn("wrapper content read during Manifest generation", wrapper_text)

    def test_build_kernel_records_reuses_master_recipe_for_group_slaves(self):
        Path(self.metadata_path).write_text(
            json.dumps(
                {
                    "kernelList": [
                        {"tilingKey": 100000, "kernelName": "add_custom_100000"},
                        {"tilingKey": 100001, "kernelName": "add_custom_100001"},
                    ]
                }
            ),
            encoding="utf-8",
        )

        records = self.make_records(
            tiling_key_group_map={"100000": ["100001"]},
        )

        self.assertEqual(
            [(item.kernel_name, item.tiling_key) for item in records],
            [
                ("add_custom_100000", "100000"),
                ("add_custom_100001", "100001"),
            ],
        )
        self.assertEqual([item.compile_tiling_key for item in records], ["100000"] * 2)
        self.assertEqual([item.byte_size for item in records], [8, 8])
        self.assertEqual(records[0].commands, records[1].commands)

        no_tiling_records = self.make_records(
            tiling_key_data_size={},
            default_tiling_size=0,
            tiling_key_group_map={"100000": ["100001"]},
        )
        self.assertEqual([item.byte_size for item in no_tiling_records], [0, 0])
        self.assertEqual(
            generator.static_tiling_template_text("").count(
                "@@STATIC_VALUE_tiling_data@@"
            ),
            1,
        )

    def test_sk_object_uses_static_symbol_bind_and_output_namespace(self):
        dynamic_sk_symbol = "add_custom_abcd1234_100000_tilingkey"
        basic_context = self.make_compile_context(symbols=["add_custom_100000"])
        sk_context = self.make_compile_context(
            kernel_name="add_custom_abcd1234",
            wrapper_path=self.sk_wrapper_path,
            symbols=[dynamic_sk_symbol],
        )
        options = self.make_options()
        records = self.make_records(
            basic_context=basic_context,
            basic_options=options,
            sk_context=sk_context,
            sk_options=options,
        )

        result = self.generate(
            self.make_request(
                records,
                sk_cap_bitmap=3,
                sk_kernel_attribute="__global__ [aicore]",
                basic_kernel_attribute="__global__ [aicore]",
                sk_bind_compile_options=options.compile_options,
            ),
            {"add_custom_100000", dynamic_sk_symbol},
        )

        manifest = json.loads(Path(result.manifest_path).read_text(encoding="utf-8"))
        sk_object = manifest["kernels"][0]["objects"][1]
        self.assertEqual(sk_object["obj_name"], "sk")
        self.assertEqual(
            [command["stage"] for command in sk_object["commands"]],
            [0, 0, 1, 1, 1],
        )
        self.assertEqual(
            sk_object["outputs"],
            [
                "${output:add_custom_100000.sk.o}",
                "${output:add_custom_100000.sk_split1.o}",
                "${output:add_custom_100000.sk_split2.o}",
                "${output:add_custom_100000.sk_split3.o}",
                "${output:add_custom_100000.sk_bind.o}",
            ],
        )
        sk_compile = sk_object["commands"][0]["cmd"]
        self.assertIn(
            "-Dauto_gen_add_custom_kernel=add_custom_100000_static_sk",
            sk_compile,
        )
        self.assertIn(
            "-Dadd_custom=add_custom_100000_tilingkey",
            sk_compile,
        )
        self.assertEqual(
            sk_object["commands"][2]["cmd"][0],
            "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy",
        )
        bind_reference = next(
            value
            for value in sk_object["commands"][1]["cmd"]
            if value.startswith("${resource:src/sk_bind_")
        )
        bind_path = os.path.join(
            self.output_root,
            "manifest",
            "resources",
            bind_reference[len("${resource:") : -1],
        )
        bind_text = Path(bind_path).read_text(encoding="utf-8")
        self.assertIn("add_custom_100000_static_sk", bind_text)
        self.assertNotIn(dynamic_sk_symbol, bind_text)

    def test_publish_failure_restores_previous_manifest(self):
        request = self.make_request(self.make_records())
        self.generate(request, {"add_custom_100000"})
        marker = os.path.join(self.output_root, "manifest", "previous.txt")
        Path(marker).write_text("previous\n", encoding="utf-8")
        real_replace = os.replace
        replace_count = 0

        def fail_stage_publish(source, destination):
            nonlocal replace_count
            replace_count += 1
            if replace_count == 2:
                raise OSError("publish failed")
            return real_replace(source, destination)

        with ExitStack() as stack:
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "read_resource_id_section",
                    return_value=self.resource_id,
                )
            )
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "_read_exported_symbols",
                    return_value={"add_custom_100000"},
                )
            )
            stack.enter_context(
                mock.patch.object(
                    generator.os, "replace", side_effect=fail_stage_publish
                )
            )
            with self.assertRaisesRegex(
                generator.ManifestPublishError, "failed to publish"
            ):
                generator.generate_manifest(request)

        self.assertEqual(Path(marker).read_text(encoding="utf-8"), "previous\n")
        self.assertFalse(
            any(
                name.startswith(".manifest.stage.")
                for name in os.listdir(self.output_root)
            )
        )

        with ExitStack() as stack:
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "read_resource_id_section",
                    return_value=self.resource_id,
                )
            )
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "_read_exported_symbols",
                    return_value={"add_custom_100000"},
                )
            )
            stack.enter_context(
                mock.patch.object(
                    generator,
                    "_fsync_directory",
                    side_effect=OSError("fsync failed"),
                )
            )
            with self.assertRaisesRegex(
                generator.ManifestPublishError, "failed to publish"
            ):
                generator.generate_manifest(request)

        self.assertEqual(Path(marker).read_text(encoding="utf-8"), "previous\n")


if __name__ == "__main__":
    unittest.main()
