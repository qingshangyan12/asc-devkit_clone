#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import json
import os
import sys
import unittest
from types import SimpleNamespace
from tempfile import TemporaryDirectory


FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
sys.path.insert(0, os.path.join(TOP_PATH, "tools/build/"))

from asc_op_compile_base.asc_op_compiler.ascendc_common_utility import CompileInfo
from asc_op_compile_base.asc_op_compiler.ascendc_compile_manifest import OpCompileManifestRecorder
from asc_op_compile_base.asc_op_compiler.ascendc_constants import CompileOptionTuple
from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage


class TestAscendcCompileManifest(unittest.TestCase):
    def setUp(self):
        global_var_storage.set_variable("ascendc_short_soc_version", "Ascend910B")

    @staticmethod
    def _compile_info(kernel_name, source_path, wrapper_path, log_path):
        compile_info = CompileInfo()
        compile_info.src_file = source_path
        compile_info.gen_kernel_func_file = wrapper_path
        compile_info.kernel_name = kernel_name
        compile_info.origin_func_name = "test_op"
        compile_info.op_type = "TestOp"
        compile_info.code_channel = 0
        compile_info.tiling_key_list = ["20", "21"]
        compile_info.tiling_key_group_map = {}
        compile_info.compile_log_path = log_path
        compile_info.no_set_kernel_type = True
        return compile_info

    @staticmethod
    def _write(path, content):
        with open(path, 'w') as fd:
            fd.write(content)

    @staticmethod
    def _compile_command(source, arch, symbol, tiling_key, extra=""):
        return " ".join(filter(None, [
            "bisheng -c -DBASE=1", extra, source,
            "--cce-aicore-arch={}".format(arch), "--cce-aicore-only",
            "-o {}.o".format(symbol), "-Dauto_gen_test_op_kernel={}".format(symbol),
            "-DTILING_KEY_VAR={}UL".format(tiling_key)
        ]))

    @staticmethod
    def _write_compile_log(path, commands):
        lines = []
        for command in commands:
            lines.extend(["// Stage: CompileStage.COMPILE\n", command + "\n"])
        TestAscendcCompileManifest._write(path, "".join(lines))

    @staticmethod
    def _op_info(kernel_name):
        return SimpleNamespace(op_type="TestOp", kernel_name=kernel_name)

    @staticmethod
    def _tiling_info():
        return SimpleNamespace(static_shape_flag=True, tiling_key=20, block_num=20)

    def test_normal_compile_keeps_existing_manifest_shape(self):
        with TemporaryDirectory() as temp_dir:
            kernel_name = "TestOp_normal"
            source_path = os.path.join(temp_dir, "test_op.cpp")
            wrapper_path = os.path.join(temp_dir, "test_op_kernel.cpp")
            log_path = os.path.join(temp_dir, "compile.log")
            self._write(source_path, "void test_op() {}\n")
            self._write(wrapper_path, """#include \"{}\"
extern \"C\" __global__ [aicore] void auto_gen_test_op_kernel() {{}}
""".format(source_path))
            self._write_compile_log(log_path, [
                self._compile_command(wrapper_path, "dav-c220-vec", "normal_symbol", "20")
            ])

            recorder = OpCompileManifestRecorder()
            recorder.start(self._op_info(kernel_name), "test_op", source_path, temp_dir)
            compile_info = self._compile_info(kernel_name, source_path, wrapper_path, log_path)
            compile_info.tiling_key_list = ["20"]
            recorder.record_wrapper_body(compile_info)
            recorder.write(compile_info, self._tiling_info(), CompileOptionTuple(["-DBASE=1"], []))

            with open(recorder.manifest_path, 'r') as fd:
                manifest = json.load(fd)
            self.assertNotIn("workflow", manifest)
            self.assertNotIn("features", manifest)
            self.assertNotIn("super_kernel_sub_combine", manifest)
            self.assertNotIn('"raw_options"', json.dumps(manifest))
            self.assertEqual(set(manifest["artifacts"]), {"wrapper_body"})
            with open(os.path.join(temp_dir, manifest["artifacts"]["wrapper_body"]), 'r') as fd:
                wrapper_body = fd.read()
            self.assertIn("{SRC_FILE_PLACEHOLDER}", wrapper_body)
            self.assertIn("{kernel_type_placeholder}", wrapper_body)

    def test_super_kernel_sub_combine_records_delta_artifacts_and_bindings(self):
        with TemporaryDirectory() as temp_dir:
            kernel_name = "TestOp_combine"
            source_path = os.path.join(temp_dir, "test_op.cpp")
            super_wrapper = os.path.join(temp_dir, "test_op_100_kernel.cpp")
            normal_wrapper = os.path.join(temp_dir, "test_op_100_norm_kernel.cpp")
            sk_bind_path = os.path.join(temp_dir, "sk_bind.cpp")
            log_path = os.path.join(temp_dir, "compile.log")
            self._write(source_path, "void test_op() {}\n")
            self._write(super_wrapper, """#include \"{}\"
extern \"C\" __attribute__((aligned(512))) [aicore] void auto_gen_test_op_kernel() {{}}
""".format(source_path))
            self._write(normal_wrapper, """#include \"{}\"
extern \"C\" __global__ [aicore] void auto_gen_test_op_kernel() {{}}
""".format(source_path))

            sk_bind_source = """#include \"kernel_operator.h\"
SK_BIND(norm_aiv_21, 7, sk_aiv_21, sk_aiv_21_split1, sk_aiv_21_split2, sk_aiv_21_split3);
SK_BIND(norm_aic_20, 7, sk_aic_20, sk_aic_20_split1, sk_aic_20_split2, sk_aic_20_split3);
SK_BIND(norm_aiv_20, 7, sk_aiv_20, sk_aiv_20_split1, sk_aiv_20_split2, sk_aiv_20_split3);
"""
            self._write(sk_bind_path, sk_bind_source)

            commands = [
                self._compile_command(super_wrapper, "dav-c220-cube", "sk_aic_20", "20",
                                      "-D__ASCENDC_ENABLE_SUPER_KERNEL__"),
                self._compile_command(super_wrapper, "dav-c220-vec", "sk_aiv_20", "20",
                                      "-D__ASCENDC_ENABLE_SUPER_KERNEL__"),
                self._compile_command(super_wrapper, "dav-c220-vec", "sk_aiv_21", "21",
                                      "-D__ASCENDC_ENABLE_SUPER_KERNEL__"),
                self._compile_command(normal_wrapper, "dav-c220-cube", "norm_aic_20", "20"),
                self._compile_command(normal_wrapper, "dav-c220-vec", "norm_aiv_20", "20"),
                self._compile_command(normal_wrapper, "dav-c220-vec", "norm_aiv_21", "21"),
                "bisheng -c -DBASE=1 -D__ASCENDC_ENABLE_SUPER_KERNEL__ {} "
                "--cce-aicore-arch=dav-c220-cube -DTILING_KEY_VAR=0 -o sk_bind.o".format(sk_bind_path)
            ]
            self._write_compile_log(log_path, commands)

            recorder = OpCompileManifestRecorder()
            recorder.start(self._op_info(kernel_name), "test_op", source_path, temp_dir)
            recorder.enable_super_kernel_sub_combine()
            super_info = self._compile_info(kernel_name, source_path, super_wrapper, log_path)
            super_info.global_kernel_attribute = "__attribute__((aligned(512))) [aicore]"
            normal_info = self._compile_info(kernel_name, source_path, normal_wrapper, log_path)
            normal_info.global_kernel_attribute = "__global__ [aicore]"
            recorder.record_wrapper_body(super_info, "super_kernel")
            recorder.record_wrapper_body(normal_info, "normal")

            super_options = CompileOptionTuple(
                ["-DBASE=1", "-D__ASCENDC_ENABLE_SUPER_KERNEL__", "-DONE_CORE_DUMP_SIZE=1024"],
                ["-super-option"])
            normal_options = CompileOptionTuple(
                ["-DBASE=1", "-DWORKSPACE_PARAM_OFFSET=10", "-DONE_CORE_DUMP_SIZE=1024"],
                ["-normal-option"])
            recorder.record_super_kernel_sub_combine(
                super_info, normal_info, super_options, normal_options, sk_bind_path)
            recorder.write(normal_info, self._tiling_info(), normal_options)

            with open(recorder.manifest_path, 'r') as fd:
                manifest = json.load(fd)
            self.assertNotIn("workflow", manifest)
            self.assertNotIn("super_kernel_sub_combine", manifest)
            self.assertNotIn('"raw_options"', json.dumps(manifest))
            super_kernel = manifest["features"]["super_kernel"]
            self.assertTrue(super_kernel["enable_super_kernel"])
            self.assertEqual(manifest["compile_options"]["compile_count"], 3)
            self.assertNotIn("0", manifest["compile_options"]["by_tiling_key"])
            self.assertEqual(set(manifest["compile_options"]["by_tiling_key"]), {"20", "21"})
            self.assertEqual(set(manifest["compile_options"]["by_tiling_key"]["20"]["cores"]), {"aic", "aiv"})
            aic_macros = manifest["compile_options"]["by_tiling_key"]["20"]["cores"]["aic"]["macros"]
            self.assertIn("auto_gen_test_op_kernel=norm_aic_20", aic_macros)
            self.assertNotIn("__ASCENDC_ENABLE_SUPER_KERNEL__", aic_macros)
            self.assertEqual(manifest["kernel_type_info"]["by_tiling_key"]["20"],
                             "KERNEL_TYPE_MIX_AIC_1_2")
            self.assertEqual(manifest["kernel_type_info"]["by_tiling_key"]["21"],
                             "KERNEL_TYPE_AIV_ONLY")

            artifacts = manifest["artifacts"]
            self.assertEqual(set(artifacts), {"wrapper_body", "super_kernel_wrapper_body", "sk_bind"})
            self.assertNotEqual(artifacts["wrapper_body"], artifacts["super_kernel_wrapper_body"])
            self.assertEqual(artifacts["sk_bind"], "TestOp_combine_sk_bind.cpp")
            with open(os.path.join(temp_dir, artifacts["wrapper_body"]), 'r') as fd:
                normal_artifact = fd.read()
            with open(os.path.join(temp_dir, artifacts["super_kernel_wrapper_body"]), 'r') as fd:
                super_artifact = fd.read()
            self.assertIn("{kernel_type_placeholder}", normal_artifact)
            self.assertNotIn("{kernel_type_placeholder}", super_artifact)
            self.assertIn("{SRC_FILE_PLACEHOLDER}", super_artifact)
            with open(os.path.join(temp_dir, artifacts["sk_bind"]), 'r') as fd:
                self.assertEqual(fd.read(), sk_bind_source)

            combine = super_kernel["super_kernel_sub_combine"]
            self.assertEqual(combine["compile_options_delta"]["add"]["macros"],
                             ["__ASCENDC_ENABLE_SUPER_KERNEL__"])
            self.assertEqual(combine["compile_options_delta"]["remove"]["macros"],
                             ["WORKSPACE_PARAM_OFFSET=10"])
            self.assertEqual(combine["compile_options_delta"]["add"]["mllvm_options"], ["-super-option"])
            self.assertEqual(combine["compile_options_delta"]["remove"]["mllvm_options"], ["-normal-option"])
            self.assertEqual(combine["kernel_attributes"]["normal"], "__global__ [aicore]")
            self.assertEqual(combine["kernel_attributes"]["super_kernel"],
                             "__attribute__((aligned(512))) [aicore]")
            self.assertEqual(combine["split_count"], 3)
            self.assertNotIn("capability_bitmap", combine)

            bindings = combine["bindings"]["tiling_key"]
            self.assertEqual(list(bindings), ["20", "21"])
            self.assertEqual([item["core_type"] for item in bindings["20"]], ["aic", "aiv"])
            self.assertEqual(bindings["21"][0]["core_type"], "aiv")
            self.assertEqual(bindings["20"][0]["normal_symbol"], "norm_aic_20")
            self.assertEqual(bindings["20"][0]["super_kernel_symbol"], "sk_aic_20")
            self.assertEqual(bindings["20"][0]["split_symbols"],
                             ["sk_aic_20_split1", "sk_aic_20_split2", "sk_aic_20_split3"])

    def test_super_kernel_record_only_uses_normal_compile_log(self):
        with TemporaryDirectory() as temp_dir:
            kernel_name = "TestOp_record_only"
            source_path = os.path.join(temp_dir, "test_op.cpp")
            normal_wrapper = os.path.join(temp_dir, "test_op_100_kernel.cpp")
            super_wrapper = os.path.join(temp_dir, "test_op_100_super_kernel.cpp")
            log_path = os.path.join(temp_dir, "compile.log")
            self._write(source_path, "void test_op() {}\n")
            self._write(normal_wrapper, """#include \"{}\"
extern \"C\" __global__ [aicore] void auto_gen_test_op_kernel() {{}}
""".format(source_path))
            self._write(super_wrapper, """#include \"{}\"
extern \"C\" __attribute__((aligned(512))) [aicore] void auto_gen_test_op_kernel() {{}}
""".format(source_path))
            self._write_compile_log(log_path, [
                self._compile_command(normal_wrapper, "dav-c220-vec", "normal_symbol", "20")
            ])

            recorder = OpCompileManifestRecorder()
            recorder.start(self._op_info(kernel_name), "test_op", source_path, temp_dir)
            recorder.enable_super_kernel_sub_combine(record_only=True)
            normal_info = self._compile_info(kernel_name, source_path, normal_wrapper, log_path)
            normal_info.tiling_key_list = ["20"]
            normal_info.global_kernel_attribute = "__global__ [aicore]"
            super_info = self._compile_info(kernel_name, source_path, super_wrapper, log_path)
            super_info.global_kernel_attribute = "__attribute__((aligned(512))) [aicore]"
            recorder.record_wrapper_body(normal_info)
            recorder.record_wrapper_body(super_info, "super_kernel")
            normal_options = CompileOptionTuple(["-DBASE=1"], [])
            super_options = CompileOptionTuple(
                ["-DBASE=1", "-D__ASCENDC_ENABLE_SUPER_KERNEL__"], [])
            recorder.record_super_kernel_sub_combine(
                super_info, normal_info, super_options, normal_options, None)
            recorder.write(normal_info, self._tiling_info(), normal_options)

            with open(recorder.manifest_path, 'r') as fd:
                manifest = json.load(fd)
            combine = manifest["features"]["super_kernel"]["super_kernel_sub_combine"]
            self.assertTrue(combine["record_only"])
            self.assertEqual(manifest["compile_options"]["compile_count"], 1)
            self.assertEqual(combine["split_count"], 3)
            self.assertEqual(combine["bindings"], {
                "tiling_key": {
                    "20": [{
                        "core_type": "aiv",
                        "arch": "dav-c220-vec",
                        "normal_symbol": "normal_symbol",
                        "super_kernel_symbol": "normal_symbol_middle",
                        "split_symbols": [
                            "normal_symbol_middle_split1",
                            "normal_symbol_middle_split2",
                            "normal_symbol_middle_split3"
                        ]
                    }]
                }
            })
            sk_bind_path = os.path.join(temp_dir, manifest["artifacts"]["sk_bind"])
            with open(sk_bind_path, 'r') as fd:
                sk_bind_source = fd.read()
            self.assertIn("SK_BIND(normal_symbol, 0, normal_symbol_middle", sk_bind_source)


if __name__ == '__main__':
    unittest.main()
