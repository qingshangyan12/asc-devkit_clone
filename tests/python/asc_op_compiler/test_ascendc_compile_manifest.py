#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# ----------------------------------------------------------------------------------------------------------

import json
import os
import sys
import unittest
from tempfile import TemporaryDirectory
from types import SimpleNamespace


FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
sys.path.insert(0, os.path.join(TOP_PATH, "tools/build/"))

from asc_op_compile_base.asc_op_compiler.ascendc_compile_manifest import (  # noqa: E402
    OpCompileManifestRecorder,
    STATIC_TILING_TOKEN,
)


class TestAscendcCompileManifest(unittest.TestCase):
    @staticmethod
    def _write(path, content):
        with open(path, "w") as output:
            output.write(content)

    @staticmethod
    def _tiling_header():
        return """#ifndef __TILINGDATA_HEADER__
#define __TILINGDATA_HEADER__
struct TilingDataLong { unsigned char value[96]; };
struct TilingDataShort { unsigned char value[8]; };

#if defined(ASCENDC_CPU_DEBUG)
inline void InitTilingData() {}
#endif
#ifdef ASCENDC_TIME_STAMP_ON
#define GET_TILING_DATA(tiling_data, tiling_arg)
#endif
#endif // __TILINGDATA_HEADER__
"""

    @staticmethod
    def _compile_info(source, wrapper, log):
        return SimpleNamespace(
            src_file=source,
            gen_kernel_func_file=wrapper,
            kernel_name="AddCustom_ab1b6750d7f510985325b603cb06dc8b",
            op_type="AddCustom",
            tiling_key_list=["1", "2"],
            tiling_key_struct_map={"1": "TilingDataLong", "2": "TilingDataShort"},
            tiling_struct_size_map={"TilingDataLong": 96, "TilingDataShort": 8},
            compile_log_path=log,
        )

    @staticmethod
    def _tiling_info():
        return SimpleNamespace(
            tiling_key_struct_map={},
            tiling_key_data_size={},
            default_tiling_struct="",
            default_tiling_size=0,
        )

    @staticmethod
    def _options():
        return SimpleNamespace(compile_options=["-O2"], mllvm_options=[])

    def test_writes_only_schema_4_resources_and_preserves_normal_wrapper(self):
        with TemporaryDirectory() as temp_dir:
            source = os.path.join(temp_dir, "add_custom.cpp")
            wrapper = os.path.join(temp_dir, "add_custom_kernel.cpp")
            sk_wrapper = os.path.join(temp_dir, "add_custom_sk_kernel.cpp")
            sk_bind = os.path.join(temp_dir, "sk_bind.cpp")
            tiling_header = os.path.join(temp_dir, "add_custom_tiling_data.h")
            compile_log = os.path.join(temp_dir, "compile.log")
            normal_body = '#include "{}"\nextern "C" void normal_wrapper() {{}}\n'.format(source)
            self._write(source, "void add_custom() {}\n")
            self._write(wrapper, normal_body)
            self._write(sk_wrapper, "void super_kernel_wrapper() {}\n")
            self._write(sk_bind, "void sk_bind() {}\n")
            self._write(tiling_header, self._tiling_header())
            self._write(compile_log, """// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {wrapper} -I/opt/ascend/include -I/opt/ascend/impl -I/opt/ascend/include -include {header} -DTILING_KEY_VAR=1UL -o {temp}/key1.o
// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {wrapper} -I/opt/ascend/include -I/opt/ascend/impl -I/opt/ascend/include -include {header} -DTILING_KEY_VAR=2UL -o {temp}/key2.o
// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {sk_wrapper} -I/opt/ascend/include -I/opt/ascend/impl -I/opt/ascend/include -include {header} -DTILING_KEY_VAR=1UL -o {temp}/sk_key1.o
// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {sk_wrapper} -I/opt/ascend/include -I/opt/ascend/impl -I/opt/ascend/include -include {header} -DTILING_KEY_VAR=2UL -o {temp}/sk_key2.o
// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {sk_bind} -DTILING_KEY_VAR=0 -o {temp}/sk_bind.o
// Stage: CompileStage.FATBIN
/opt/ascend/bin/ld.lld -r {temp}/key1.o {temp}/key2.o -o {temp}/normal.o
// Stage: CompileStage.LINKRELOCATE
/opt/ascend/bin/ld.lld {temp}/normal.o -o {temp}/normal.o
// Stage: CompileStage.FATBIN
/opt/ascend/bin/ld.lld -r {temp}/sk_key1.o {temp}/sk_key2.o {temp}/sk_bind.o -o {temp}/sk.o
""".format(wrapper=wrapper, sk_wrapper=sk_wrapper, sk_bind=sk_bind,
           header=tiling_header, temp=temp_dir))
            with open(compile_log, "r") as log_file:
                compile_commands = log_file.read()
            compile_commands = compile_commands.replace(
                "-c {}".format(wrapper),
                "-c {} -I{}/build_out/op_kernel/AddCustom/dynamic/../ascendc/common".format(
                    wrapper, temp_dir))
            self._write(compile_log, compile_commands)

            compile_info = self._compile_info(source, wrapper, compile_log)
            recorder = OpCompileManifestRecorder()
            recorder.start(
                SimpleNamespace(op_type="AddCustom", kernel_name=compile_info.kernel_name),
                "add_custom", source, temp_dir)
            recorder.set_tiling_header(tiling_header)
            recorder.prepare_binary_identity(compile_info, self._options())
            recorder.record_wrapper_body(compile_info)
            recorder.record_wrapper_body(
                SimpleNamespace(gen_kernel_func_file=sk_wrapper, kernel_name=compile_info.kernel_name),
                "super_kernel")
            recorder.record_sk_bind(sk_bind, compile_info.kernel_name)
            final_normal_body = normal_body + "// final DFX section\n"
            self._write(wrapper, final_normal_body)
            recorder.write(compile_info, self._tiling_info(), 4)

            manifest_name = "AddCustom_ab1b6750d7f510985325b603cb06dc8b_manifest.json"
            with open(os.path.join(temp_dir, manifest_name), "r") as manifest_file:
                manifest = json.load(manifest_file)
            self.assertEqual(
                set(manifest),
                {"schema_version", "binId", "soc_version", "source_file", "options", "files", "kernels"})
            self.assertEqual(manifest["schema_version"], "4.0")
            self.assertTrue(manifest["soc_version"])
            self.assertEqual(manifest["source_file"], "add_custom.cpp")
            self.assertRegex(manifest["binId"], r"^[0-9a-f]{64}$")
            self.assertEqual(manifest["options"], {"common_include": [
                "-I${env:ASCEND_HOME_PATH}/include",
                "-I${env:ASCEND_HOME_PATH}/impl",
                "-I${env:ASCEND_HOME_PATH}/include",
            ]})
            self.assertEqual([item["kernel_name"] for item in manifest["kernels"]], [
                "AddCustom_ab1b6750d7f510985325b603cb06dc8b_1",
                "AddCustom_ab1b6750d7f510985325b603cb06dc8b_2",
            ])
            self.assertTrue(all(set(item) == {"file_name", "file_path"}
                                for item in manifest["files"]))
            self.assertEqual(
                {item["file_path"] for item in manifest["files"]},
                {"/manifests/AddCustom_ab1b6750d7f510985325b603cb06dc8b/resources/{}".format(
                    item["file_name"]) for item in manifest["files"]})
            self.assertTrue(any(item["file_name"].endswith("_sk_bind.cpp")
                                for item in manifest["files"]))

            resources = os.path.join(temp_dir, "resources")
            with open(os.path.join(resources, "AddCustom_ab1b6750d7f510985325b603cb06dc8b_kernel.cpp")) as wrapper_file:
                self.assertEqual(
                    wrapper_file.read(), final_normal_body.replace(source, os.path.basename(source)))
            merged_name = "AddCustom_ab1b6750d7f510985325b603cb06dc8b_tiling_data.h"
            with open(os.path.join(resources, merged_name)) as header_file:
                merged = header_file.read()
            self.assertIn("#if TILING_KEY_VAR == 1UL", merged)
            self.assertIn("#elif TILING_KEY_VAR == 2UL", merged)
            self.assertIn("#define TILING_DATA_TYPE TilingDataLong", merged)
            self.assertIn("#define TILING_DATA_TYPE TilingDataShort", merged)
            self.assertIn(STATIC_TILING_TOKEN, merged)
            self.assertNotIn("InitTilingData", merged)

            for kernel, expected_struct, expected_size in zip(
                    manifest["kernels"], ["TilingDataLong", "TilingDataShort"], [96, 8]):
                static_input = kernel["constants_info"][0]
                self.assertEqual(static_input["name"], expected_struct)
                self.assertEqual(static_input["parameter_index"], 4)
                self.assertEqual(static_input["byte_size"], expected_size)
                self.assertEqual(static_input["file"], merged_name)
                self.assertEqual(static_input["template"], STATIC_TILING_TOKEN)
                self.assertEqual(set(kernel), {"kernel_name", "constants_info", "commands"})
                self.assertEqual(set(kernel["commands"][0]), {"type", "mode", "options", "output"})
                self.assertEqual({command["mode"] for command in kernel["commands"]},
                                 {"normal", "sk"})
                self.assertEqual([command["type"] for command in kernel["commands"]].count("ld"), 2)
                self.assertTrue(all("-r" in command["options"]
                                    for command in kernel["commands"] if command["type"] == "ld"))
                self.assertTrue(all(
                    "-I${source_dir}" in command["options"]
                    for command in kernel["commands"] if command["type"] == "bisheng"))
                self.assertFalse(any(
                    "build_out" in value or "/ascendc/common" in value
                    for command in kernel["commands"] for value in command["options"]))
                self.assertTrue(any(value.startswith("${resource:resources/")
                                    for command in kernel["commands"]
                                    for value in command["options"]))
                self.assertTrue(any(
                    "${options:common_include}" in command["options"]
                    for command in kernel["commands"] if command["type"] == "bisheng"))
                self.assertFalse(any(
                    value.startswith("-I${env:ASCEND_HOME_PATH}")
                    for command in kernel["commands"] for value in command["options"]))

    def test_keeps_real_mixed_core_compile_and_link_chain(self):
        with TemporaryDirectory() as temp_dir:
            source = os.path.join(temp_dir, "matmul.cpp")
            wrapper = os.path.join(temp_dir, "matmul_kernel.cpp")
            tiling_header = os.path.join(temp_dir, "matmul_tiling_data.h")
            compile_log = os.path.join(temp_dir, "compile.log")
            self._write(source, "void matmul() {}\n")
            self._write(wrapper, '#include "{}"\n'.format(source))
            self._write(tiling_header, self._tiling_header())
            self._write(compile_log, """// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {wrapper} -DTILING_KEY_VAR=1UL -o {temp}/mix_aic.o
// Stage: CompileStage.COMPILE
/opt/ascend/bin/bisheng -c {wrapper} -DTILING_KEY_VAR=1UL -o {temp}/mix_aiv.o
// Stage: CompileStage.FATBIN
/opt/ascend/bin/ld.lld {temp}/mix_aic.o {temp}/mix_aiv.o -o {temp}/mixed.o
""".format(wrapper=wrapper, temp=temp_dir))
            compile_info = self._compile_info(source, wrapper, compile_log)
            compile_info.tiling_key_list = ["1"]

            recorder = OpCompileManifestRecorder()
            recorder.start(
                SimpleNamespace(op_type="Matmul", kernel_name="Matmul_0123456789abcdef"),
                "matmul", source, temp_dir)
            recorder.set_tiling_header(tiling_header)
            recorder.prepare_binary_identity(compile_info, self._options())
            recorder.record_wrapper_body(compile_info)
            recorder.write(compile_info, self._tiling_info(), 6)

            with open(os.path.join(temp_dir, "Matmul_0123456789abcdef_manifest.json"), "r") as manifest_file:
                commands = json.load(manifest_file)["kernels"][0]["commands"]
            self.assertEqual([command["type"] for command in commands],
                             ["bisheng", "bisheng", "ld"])
            self.assertTrue(all(
                "-I${source_dir}" in command["options"]
                for command in commands if command["type"] == "bisheng"))
            self.assertIn("${output:artifacts/Matmul_0123456789abcdef_1.o}",
                          commands[-1]["output"])


if __name__ == "__main__":
    unittest.main()
