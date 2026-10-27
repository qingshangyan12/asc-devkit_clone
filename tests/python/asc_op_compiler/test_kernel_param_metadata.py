# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Regression tests for metadata generation during the existing CCE compilation."""

from contextlib import ExitStack
import importlib
import re
import struct
from pathlib import Path
import sys
from tempfile import TemporaryDirectory
from types import SimpleNamespace
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "tools/build"))
from asc_op_compile_base.asc_op_compiler import ascendc_compile_dfx as metadata
from asc_op_compile_base.asc_op_compiler import ascendc_compile_v220 as v220
from asc_op_compile_base.asc_op_compiler.ascendc_common_utility import (
    CommonUtility,
    CompileInfo,
    CompileCommandMode,
    CompileCommandSession,
)
from asc_op_compile_base.asc_op_compiler.ascendc_constants import CORE_TYPE_CUBE, CompileOptionTuple, KernelMetaType
from asc_op_compile_base.asc_op_compiler.ascendc_compile_dfx import DFXSectionGenerator
from asc_op_compile_base.asc_op_compiler.get_op_tiling import TilingInfo
from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage

compile_op = importlib.import_module("asc_op_compile_base.asc_op_compiler.compile_op")


def decode_declaration(source):
    # Read the emitted initializer, independently of the encoder's internal helper.
    initializer = source.split("= {", 1)[1].split("};", 1)[0]
    return bytes(int(value, 16) for value in re.findall(r"0x[0-9a-f]{2}", initializer))


class TestKernelParamMetadata(unittest.TestCase):
    def test_pointer_layout_is_emitted_without_a_second_compile_switch(self):
        info = CompileInfo()
        metadata.prepare_kernel_param_metadata(info, ("GM_ADDR x", "GM_ADDR tiling"))
        source = metadata.generate_param_metadata("add", info.kernel_param_metadata)
        self.assertIn('section(".ascend.meta." ASCENDC_PARAM_STRINGIFY(add))', source)
        encoded = decode_declaration(source)
        # Protocol bytes also observed in the native <<<>>> two-pointer ELF.
        expected = bytes.fromhex(
            "10000c00 02000000 1000000000000000 "
            "11002400 00000000 00000000 00000000 08000000 08000200 00000000 00000000 00000000 00000000 "
            "11002400 00000000 01000000 08000000 08000000 08000200 00000000 00000000 00000000 00000000"
        )
        self.assertEqual(encoded, expected)
        self.assertIn("static const unsigned char", source)
        self.assertIn("aligned(4)", source)
        self.assertNotIn("struct AscendCParam", source)
        self.assertNotIn("#ifdef ASCENDC_EMIT", source)
        self.assertNotIn("static_assert", source)

    def test_zero_parameters_does_not_emit_a_zero_length_array(self):
        info = CompileInfo()
        metadata.prepare_kernel_param_metadata(info, ())
        source = metadata.generate_param_metadata("empty", info.kernel_param_metadata)
        self.assertEqual(decode_declaration(source), bytes.fromhex("10000c00 00000000 0000000000000000"))
        self.assertNotIn("parameters[0]", source)

    def test_compilation_descriptions_are_independent_and_large_offsets_are_not_truncated(self):
        first, second = CompileInfo(), CompileInfo()
        metadata.prepare_kernel_param_metadata(first, tuple(f"GM_ADDR p{i}" for i in range(64)))
        metadata.prepare_kernel_param_metadata(second, ("GM_ADDR x",))
        first_bytes = decode_declaration(metadata.generate_param_metadata("first", first.kernel_param_metadata))
        second_bytes = decode_declaration(metadata.generate_param_metadata("second", second.kernel_param_metadata))
        self.assertEqual(len(first_bytes), 16 + 64 * 40)
        self.assertEqual(struct.unpack_from("<IQ", first_bytes, 4), (64, 512))
        self.assertEqual(
            struct.unpack_from("<IIIIHHIIII", first_bytes, 16 + 63 * 40 + 4), (0, 63, 504, 8, 8, 2, 0, 0, 0, 0)
        )
        self.assertEqual(struct.unpack_from("<IQ", second_bytes, 4), (1, 8))

    def test_unsupported_description_leaves_original_source_usable(self):
        for entry, parameters in (('bad"entry', ()), ("add", None)):
            with self.subTest(entry=entry, parameters=parameters):
                info = CompileInfo()
                metadata.prepare_kernel_param_metadata(info, parameters)
                self.assertEqual(metadata.generate_param_metadata(entry, info.kernel_param_metadata), "")

    def test_prepared_description_is_independent_of_the_formal_list(self):
        parameters = ["GM_ADDR x", "GM_ADDR tiling"]
        info = CompileInfo()
        metadata.prepare_kernel_param_metadata(info, parameters)
        description = info.kernel_param_metadata
        parameters.clear()
        self.assertEqual(description.count, 2)
        self.assertEqual(description.total_size, 16)
        self.assertEqual(description.infos, ((0, 0, 0, 8, 8, 2, 0, 0, 0, 0), (0, 1, 8, 8, 8, 2, 0, 0, 0, 0)))
        with self.assertRaises(AttributeError):
            description.count = 3
        first = metadata.generate_param_metadata("first", description)
        second = metadata.generate_param_metadata("second", description)
        self.assertEqual(first.replace("first", "second"), second)
        self.assertNotIn("GM_ADDR", first)

    def test_failed_preparation_clears_previous_description_even_if_logging_fails(self):
        info = CompileInfo()
        metadata.prepare_kernel_param_metadata(info, ("GM_ADDR x",))
        self.assertIsNotNone(info.kernel_param_metadata)
        parameters = mock.MagicMock()
        parameters.__len__.return_value = 0x20000000
        with mock.patch.object(metadata.logging.Logger, "warning", side_effect=RuntimeError("log failure")):
            metadata.prepare_kernel_param_metadata(info, parameters)
        self.assertIsNone(info.kernel_param_metadata)


class TestGeneratedKernelParamSections(unittest.TestCase):
    def setUp(self):
        self.stack = ExitStack()
        self.addCleanup(self.stack.close)
        self.folder = self.stack.enter_context(TemporaryDirectory())
        self.info = CompileInfo()
        self.info.src_file = "add.cpp"
        self.info.kernel_name = "add"
        self.info.origin_func_name = "add"
        self.info.gen_kernel_func_file = str(Path(self.folder) / "add_kernel.cpp")
        self.info.tiling_key_list = ["7"]
        self.tiling = TilingInfo()
        self.tiling.static_shape_flag = False
        self.op = compile_op.OpInfo(
            kernel_name="add",
            op_type="Add",
            inputs=[{"param_name": "x"}],
            outputs=[{"param_name": "y"}],
            origin_inputs=[],
            mc2_ctx=[],
        )
        DFXSectionGenerator().dfx_info_reset(self.op)
        for name, value in (
            ("ascendc_enable_super_kernel", False),
            ("ascendc_enable_sanitizer", True),
            ("ascendc_tiling_no_register", False),
            ("ascendc_enable_aicore_exception_restart", False),
        ):
            old = global_var_storage.get_variable(name)
            global_var_storage.set_variable(name, value)
            self.addCleanup(global_var_storage.set_variable, name, old)
        self.stack.enter_context(mock.patch.object(CommonUtility, "is_v220", return_value=True))
        self.stack.enter_context(mock.patch.object(CommonUtility, "is_v100", return_value=False))
        self.stack.enter_context(mock.patch.object(CommonUtility, "is_v200", return_value=False))
        self.stack.enter_context(mock.patch.object(CommonUtility, "is_c310", return_value=False))
        self.stack.enter_context(mock.patch.object(CommonUtility, "get_chip_version", return_value="c220"))
        self.stack.enter_context(mock.patch.object(compile_op, "get_current_build_config", return_value=[]))
        self.stack.enter_context(
            mock.patch.object(compile_op, "get_v220_kernel_type_mix_flag", return_value=(False, False))
        )

    def generate_source(self):
        compile_op.gen_kernel_fun(self.info, "add", self.op, self.tiling, CompileOptionTuple([], []))
        return Path(self.info.gen_kernel_func_file).read_text()

    def test_common_generator_places_metadata_after_main_entry(self):
        source = self.generate_source()
        entry = source.index('extern "C" __global__ [aicore] void auto_gen_add_kernel(')
        self.assertGreater(source.index("static const unsigned char g_ascendc_param_metadata_"), entry)
        self.assertIn('section(".ascend.meta." ASCENDC_PARAM_STRINGIFY(auto_gen_add_kernel))', source)
        self.assertEqual(struct.unpack_from("<IQ", decode_declaration(source), 4), (4, 32))

    def test_group_metadata_stays_with_each_slave_entry(self):
        self.info.tiling_key_group_map = {"7": ["8"]}
        self.info.tiling_key_kernel_type = {
            "7": KernelMetaType.KERNEL_TYPE_MIX_AIC_1_2,
            "8": KernelMetaType.KERNEL_TYPE_MIX_AIC_1_2,
        }
        source = self.generate_source()
        for core, entry in (("CUBE", "add_8_mix_aic"), ("VEC", "add_8_mix_aiv")):
            guard = f"#if TILING_KEY_VAR == 7UL && defined(__DAV_C220_{core}__)"
            block = source.split(guard, 1)[1].split("\n#if TILING_KEY_VAR", 1)[0]
            self.assertIn(f"void {entry}(", block)
            self.assertIn(f"ASCENDC_PARAM_STRINGIFY({entry})", block)

    def test_encoding_failure_preserves_generated_kernel(self):
        with mock.patch.object(metadata, "_serialize_param_metadata", side_effect=RuntimeError("encoding failure")):
            source = self.generate_source()
        self.assertIn("void auto_gen_add_kernel(", source)
        self.assertNotIn("g_ascendc_param_metadata_", source)

    def test_sk_generation_clears_ordinary_description_and_emits_no_metadata(self):
        self.generate_source()
        self.info.super_kernel_info = {"sp_options": {}}
        global_var_storage.set_variable("ascendc_enable_super_kernel", True)
        with mock.patch.object(compile_op, "get_context", return_value=None):
            source = self.generate_source()
        self.assertIn("void auto_gen_add_kernel(uint64_t args_offset)", source)
        self.assertIsNone(self.info.kernel_param_metadata)
        self.assertNotIn("g_ascendc_param_metadata_", source)

    def test_static_and_non_v220_generation_include_parameter_metadata(self):
        self.tiling.static_shape_flag = True
        with mock.patch.object(CommonUtility, "is_v220", return_value=False):
            source = self.generate_source()
        self.assertIn("ASCENDC_PARAM_STRINGIFY(auto_gen_add_kernel)", source)
        self.assertEqual(struct.unpack_from("<IQ", decode_declaration(source), 4), (3, 24))


class TestCompileCommandCompatibility(unittest.TestCase):
    def setUp(self):
        self.info = CompileInfo()
        self.info.kernel_name = "add"
        self.info.origin_func_name = "add"
        self.info.gen_kernel_func_file = "/tmp/metadata_wrapper.cpp"
        self.info.dst_file = "/tmp/metadata.o"
        self.info.code_channel = CORE_TYPE_CUBE
        metadata.prepare_kernel_param_metadata(self.info, ("GM_ADDR x", "GM_ADDR tiling"))
        self.info.tiling_key_list = ["7"]
        self.info.no_set_kernel_type = True
        self.tiling = TilingInfo()
        self.tiling.static_shape_flag = False
        self.stack = ExitStack()
        self.addCleanup(self.stack.close)
        for name, value in (("ascendc_enable_super_kernel", False), ("ascendc_meta_info", "")):
            old = global_var_storage.get_variable(name)
            global_var_storage.set_variable(name, value)
            self.addCleanup(global_var_storage.set_variable, name, old)
        self.stack.enter_context(mock.patch.object(CommonUtility, "is_v220", return_value=True))
        self.stack.enter_context(mock.patch.object(CommonUtility, "is_c310", return_value=False))
        self.stack.enter_context(mock.patch.object(DFXSectionGenerator(), "generate_dfx_section", return_value=""))

    def run_default_compile(self, mode):
        self.info.compile_command_session = CompileCommandSession(mode)
        command = ["bisheng", "-c", "wrapper.cpp", "-o", "/tmp/metadata_7.o"]
        self.original_source = (
            "#if 1\n"
            + metadata.generate_param_metadata("auto_gen_add_kernel", self.info.kernel_param_metadata)
            + "#endif\n"
        )
        self.stack.enter_context(
            mock.patch.object(CommonUtility, "ascendc_read_file", return_value=self.original_source.splitlines(True))
        )
        self.write = self.stack.enter_context(mock.patch.object(CommonUtility, "ascendc_write_file"))
        self.stack.enter_context(mock.patch.object(v220, "compile_single_tiling_v220", return_value=(command, "add_7")))
        self.batch = self.stack.enter_context(mock.patch.object(v220, "compile_multi_tilingkey"))
        self.fatbin = self.stack.enter_context(mock.patch.object(v220, "fatbin_objs"))
        return command

    def test_record_only_keeps_original_argv_and_return_with_metadata_source(self):
        command = self.run_default_compile(CompileCommandMode.RECORD_ONLY)
        result = v220.call_bisheng_v220(self.info, None, self.tiling, "dav-c220-cube", CORE_TYPE_CUBE)
        self.assertEqual(result, ["7"])
        self.assertEqual(self.info.compile_command_session.records[0].argv, tuple(command))
        source = "".join(self.write.call_args.args[1])
        self.assertEqual(source, self.original_source)
        self.batch.assert_not_called()
        self.fatbin.assert_not_called()

    def test_success_uses_original_batch_and_return(self):
        command = self.run_default_compile(CompileCommandMode.EXECUTE_AND_RECORD)
        result = v220.call_bisheng_v220(self.info, None, self.tiling, "dav-c220-cube", CORE_TYPE_CUBE)
        self.assertEqual(result, ["7"])
        self.assertEqual(self.info.compile_command_session.records[0].argv, tuple(command))
        self.batch.assert_called_once()
        self.assertEqual(self.batch.call_args.args[1], [command])
        self.fatbin.assert_called_once()

    def test_final_declaration_is_the_parameter_source_and_sk_clears_it(self):
        op = SimpleNamespace(
            mc2_ctx=["ctx"],
            inputs=[None, {"param_name": "x"}],
            outputs=[{"param_name": "y"}],
            output_shape_depend_on_compute=[0],
            param_type_list=None,
        )
        DFXSectionGenerator().dfx_info_reset(op)
        source, _, _, _ = compile_op._gen_kernel_func_declare_head(True, False, op, self.tiling, self.info)
        expected = (
            "GM_ADDR ffts_addr",
            "GM_ADDR ctx",
            "GM_ADDR x",
            "GM_ADDR y",
            "GM_ADDR __ascendc_output_shape",
            "GM_ADDR workspace",
            "GM_ADDR tiling",
        )
        description = self.info.kernel_param_metadata
        self.assertIsNotNone(description)
        self.assertEqual((description.count, description.total_size), (7, 56))
        self.assertEqual(description.infos, tuple((0, i, i * 8, 8, 8, 2, 0, 0, 0, 0) for i in range(7)))
        self.assertTrue(source.startswith(", ".join(expected) + ") {"))
        global_var_storage.set_variable("ascendc_enable_super_kernel", True)
        with mock.patch.object(compile_op, "get_context", return_value=None):
            source, _, _, _ = compile_op._gen_kernel_func_declare_head(True, False, op, self.tiling, self.info)
        self.assertTrue(source.startswith("uint64_t args_offset) {"))
        self.assertIsNone(self.info.kernel_param_metadata)

    def test_qualified_tiling_pointer_keeps_all_parameter_records(self):
        op = SimpleNamespace(
            mc2_ctx=[],
            inputs=[{"param_name": "x"}],
            outputs=[{"param_name": "y"}],
            output_shape_depend_on_compute=[],
            param_type_list=None,
        )
        DFXSectionGenerator().dfx_info_reset(op)
        with mock.patch.object(CommonUtility, "is_920r1", return_value=True):
            source, _, _, _ = compile_op._gen_kernel_func_declare_head(False, False, op, self.tiling, self.info)
        self.assertIn("GM_ADDR __early_read_before_pre_task_done__ tiling", source)
        self.assertIsNotNone(self.info.kernel_param_metadata)
        encoded = decode_declaration(metadata.generate_param_metadata("add", self.info.kernel_param_metadata))
        self.assertEqual(struct.unpack_from("<IQ", encoded, 4), (4, 32))
        self.assertEqual(len(encoded), 16 + 4 * 40)
        for ordinal in range(4):
            self.assertEqual(
                struct.unpack_from("<IIIIHHIIII", encoded, 16 + ordinal * 40 + 4),
                (0, ordinal, ordinal * 8, 8, 8, 2, 0, 0, 0, 0),
            )

    def test_existing_compile_exception_and_preexecution_record_are_preserved(self):
        command = self.run_default_compile(CompileCommandMode.EXECUTE_AND_RECORD)
        failure = RuntimeError("original compiler failure")
        self.batch.side_effect = failure
        with self.assertRaises(RuntimeError) as raised:
            v220.call_bisheng_v220(self.info, None, self.tiling, "dav-c220-cube", CORE_TYPE_CUBE)
        self.assertIs(raised.exception, failure)
        self.assertEqual(self.info.compile_command_session.records[0].argv, tuple(command))
        self.batch.assert_called_once()
        self.fatbin.assert_not_called()

    def test_static_compile_keeps_original_command_and_return(self):
        self.run_default_compile(CompileCommandMode.EXECUTE)
        self.tiling.static_shape_flag = True
        self.tiling.tiling_key = 7
        command = ["bisheng", "-o", "/tmp/metadata.o"]
        with ExitStack() as stack:
            stack.enter_context(mock.patch.object(v220, "gen_compile_cmd_v220", return_value=list(command)))
            stack.enter_context(mock.patch.object(v220, "gen_current_kernel_name", return_value="add_static"))
            stack.enter_context(mock.patch.object(v220, "get_compile_cmd_for_kernel_name", return_value=[]))
            compile_once = stack.enter_context(mock.patch.object(CommonUtility, "run_cmd_inner"))
            result = v220.call_bisheng_v220(self.info, None, self.tiling, "dav-c220-cube", CORE_TYPE_CUBE)
        self.assertEqual(result, ["7"])
        compile_once.assert_called_once()
        self.assertEqual(
            compile_once.call_args.args[0], command + ["-DTILING_KEY_VAR=7UL", "-D__ASCENDC_DEFINE_OP_SYSTEM_RUN_CFG__"]
        )
        self.assertEqual("".join(self.write.call_args.args[1]), self.original_source)
        self.batch.assert_not_called()
        self.fatbin.assert_not_called()

    def test_sk_entry_does_not_use_stale_ordinary_parameters(self):
        global_var_storage.set_variable("ascendc_enable_super_kernel", True)
        result = v220._generate_section_content(
            "sub_kernel", "7", KernelMetaType.KERNEL_TYPE_AIC_ONLY, self.tiling, self.info
        )
        self.assertEqual(result, "")


if __name__ == "__main__":
    unittest.main()
