#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""Exercise real parsers with isolated context/logging dependencies, without an SDK."""

import ast
import copy
import hashlib
import json
import os
import importlib.util
import shutil
import subprocess
import sys
import tempfile
import types
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
BUILD = ROOT / "tools/build/asc_op_compile_base"


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


class DeterministicLevelsTest(unittest.TestCase):
    def setUp(self):
        modules = patch.dict(sys.modules)
        modules.start()
        self.addCleanup(modules.stop)
        self.selected = None
        self.flag = False
        self.additions = {}
        for name in (
            "asc_op_compile_base",
            "asc_op_compile_base.common",
            "tbe",
            "tbe.common",
            "asc_op_compile_base.asc_op_compiler",
            "asc_op_compile_base.adapter",
        ):
            module = types.ModuleType(name)
            module.__path__ = []
            sys.modules[name] = module
        self.helper = load("asc_op_compile_base.common.deterministic", BUILD / "common/deterministic.py")
        load("asc_op_compile_base.adapter.deterministic", BUILD / "adapter/deterministic.py")
        for package in ("asc_op_compile_base.common", "tbe.common"):
            context = types.ModuleType(package + ".context")
            context.get_context = lambda: types.SimpleNamespace(
                get_addition=lambda name: (
                    self.selected if name == "deterministic_level_filter" else self.additions.get(name)
                ),
                add_addition=lambda name, value: self.additions.update({name: value}),
            )
            sys.modules[context.__name__] = context
            config = types.ModuleType(package + ".buildcfg")
            config.get_current_build_config = lambda name: self.flag if name == "enable_deterministic_mode" else 0
            sys.modules[config.__name__] = config
        log = types.ModuleType("asc_op_compile_base.common.utils.log_utils")
        log.AscendCLogLevel = types.SimpleNamespace(LOG_INFO=0, LOG_WARNING=1)
        sys.modules[log.__name__] = log
        self.parsers = []
        for package in ("asc_op_compiler", "adapter"):
            prefix = "asc_op_compile_base." + package
            utility = types.ModuleType(prefix + ".ascendc_common_utility")
            utility.CommonUtility = types.SimpleNamespace(print_compile_log=lambda *args: None)
            sys.modules[utility.__name__] = utility
            info = types.ModuleType(prefix + ".get_op_tiling")
            info.OpInfo = object
            sys.modules[info.__name__] = info
            sys.modules[prefix + ".log_utils"] = log
            self.parsers.append(load(prefix + ".template_tiling", BUILD / package / "template_tiling.py"))

    def declaration(self):
        return "@@ASCENDC_TPL_ARGS_DECL_AddCustom@@ = {@@ASCENDC_TPL_BOOL_DECL_B@@ = {0,1}}"

    def group(self, bit, flag=None, levels=None):
        text = "@@{@@ASCENDC_TPL_BOOL_SEL_B@@ = {" + str(bit) + "}"
        if flag is not None:
            text += ",@@ASCENDC_TPL_DETERMINISTIC_SEL@@ = {" + str(flag).lower() + "}"
        if levels is not None:
            text += ",@@ASCENDC_TPL_DETERMINISTIC_LEVEL_SEL@@ = {" + levels + "}"
        return text + "}"

    def initialize(self, parser, *groups):
        parser.extract_template_tiling_info(self.declaration(), "@@ASCENDC_TPL_LISTS@@ = {" + ",".join(groups) + "}")

    def test_defaults_and_conflicts(self):
        normalize = self.helper.normalize_deterministic
        self.assertEqual(normalize(), (False, (0,)))
        self.assertEqual(normalize(True), (True, (1,)))
        self.assertEqual(normalize(None, "1,2"), (True, (1, 2)))
        for level in range(4):
            self.assertEqual(normalize(None, [level]), (level != 0, (level,)))
        for flag, levels in (
            (True, "0"),
            (False, "1"),
            (False, "2"),
            (False, "3"),
            (True, "0,3"),
            (True, "1,1"),
            (True, "4"),
        ):
            with self.subTest(flag=flag, levels=levels), self.assertRaises(ValueError):
                normalize(flag, levels)

    def test_dt_keys_and_unopted_identity(self):
        key = "AddCustom/d=1,p=1/0,2/0,2/attrs"
        self.assertEqual(self.helper.level_simplified_keys([key], None), [key])
        self.assertEqual(
            self.helper.level_simplified_keys([key], "1,2"),
            ["AddCustom/d=1,p=1,dt=1/0,2/0,2/attrs", "AddCustom/d=1,p=1,dt=2/0,2/0,2/attrs"],
        )
        with self.assertRaises(ValueError):
            self.helper.level_simplified_keys(["AddCustom/d=1,dt=1/x"], "1")

    def test_production_simplified_key_entry_normalizes_compiler_ignore(self):
        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        load("constant", opc / "constant.py")
        logger = types.SimpleNamespace(
            info=lambda *_: None, warn=lambda *_: None, error=lambda *_: None, debug=lambda *_: None
        )
        utils = types.ModuleType("asc_op_compile_base.common.utils")
        utils.log = logger
        sys.modules[utils.__name__] = utils
        common = types.ModuleType("opc_common")
        common.check_input_and_output_for_simplified_key = lambda _: (False, False, False, False)
        common.check_attr_for_simpilified_key = lambda _: (False, False, "")
        sys.modules[common.__name__] = common
        module = load("deterministic_production_simplified_keys", opc / "simplified_key_utils.py")
        op, info = (
            {"simplified_key": "0,2/0,2/attrs"},
            {"op_type": "AddCustom", "impl_mode": "high_performance", "deterministic_level": "1,2"},
        )
        keys, mode = module.generate_simplified_key({}, op, info, "ignore", None)
        self.assertEqual(mode, 2)
        self.assertEqual(keys, ["AddCustom/d=1,p=1,dt=1/0,2/0,2/attrs", "AddCustom/d=1,p=1,dt=2/0,2/0,2/attrs"])
        info["deterministic_level"] = "0,1,2"
        self.assertEqual(
            module.generate_simplified_key({}, op, info, "ignore", None)[0],
            [
                "AddCustom/d=0,p=1,dt=0/0,2/0,2/attrs",
                "AddCustom/d=1,p=1,dt=1/0,2/0,2/attrs",
                "AddCustom/d=1,p=1,dt=2/0,2/0,2/attrs",
            ],
        )
        info["optional_impl_mode_flag"] = True
        keys = module.generate_simplified_key({}, op, info, "ignore", None)[0]
        self.assertEqual(len(keys), 6)
        self.assertEqual(
            {key.split("/")[1] for key in keys},
            {"d=0,p=0,dt=0", "d=0,p=1,dt=0", "d=1,p=0,dt=1", "d=1,p=1,dt=1", "d=1,p=0,dt=2", "d=1,p=1,dt=2"},
        )
        info.pop("optional_impl_mode_flag")
        info["deterministic_level"] = "0"
        self.assertEqual(
            module.generate_simplified_key({}, op, info, "ignore", None)[0], ["AddCustom/d=0,p=1,dt=0/0,2/0,2/attrs"]
        )
        info["deterministic_level"] = "3"
        with self.assertRaises(ValueError):
            module.generate_simplified_key({}, op, info, "false", None)
        del info["deterministic_level"]
        self.assertEqual(
            module.generate_simplified_key({}, op, info, "ignore", None)[0],
            ["AddCustom/d=0,p=1/0,2/0,2/attrs", "AddCustom/d=1,p=1/0,2/0,2/attrs"],
        )
        with self.assertRaises(ValueError):
            self.helper.level_simplified_keys(["AddCustom/d=0,p=1/x"], "1,2")

    def test_three_groups_metadata_does_not_change_key_bits(self):
        for parser in self.parsers:
            self.initialize(parser, self.group(0, False, "0"), self.group(0, True, "1,2"), self.group(1, True, "3"))
            for flag, selected, expected in (
                (False, "0", {0}),
                (True, "1,2", {0}),
                (True, "3", {1}),
                (False, None, {0}),
                (True, None, {0, 1}),
            ):
                self.flag, self.selected = flag, selected
                decoded = parser.decode_tiling()
                self.assertEqual(self.additions["declared_deterministic_level_groups"], [[0], [1, 2], [3]])
                self.assertEqual(set(decoded), expected)
                for key, metadata in decoded.items():
                    self.assertEqual(metadata["paramArgs"], [str(key)])
                    self.assertIn("deterministicLevels", metadata)

    def test_two_groups_select_by_level_and_emit_actual_compile_metadata(self):
        for parser in self.parsers:
            for mixed_flag in (True, None):
                self.initialize(parser, self.group(0, mixed_flag, "0,1,2"), self.group(1, None, "3"))
                for flag, selected, expected in (
                    (True, "0,1,2", {0: [0, 1, 2]}),
                    (False, "0", {0: [0]}),
                    (True, "1", {0: [1]}),
                    (True, "2", {0: [2]}),
                    (True, "1,2", {0: [1, 2]}),
                    (True, "3", {1: [3]}),
                    (False, None, {0: [0]}),
                    (True, None, {0: [1, 2], 1: [3]}),
                ):
                    with self.subTest(parser=parser.__name__, flag=flag, selected=selected):
                        self.flag, self.selected = flag, selected
                        decoded = parser.decode_tiling()
                        self.assertEqual(self.additions["declared_deterministic_level_groups"], [[0, 1, 2], [3]])
                        self.assertEqual(set(decoded), set(expected))
                        for key, metadata in decoded.items():
                            self.assertEqual(metadata["deterministicLevels"], expected[key])
                            self.assertEqual(metadata["deterministic"], "true" if flag else "false")
                            self.assertEqual(metadata["paramArgs"], [str(key)])
            for flag, levels in ((False, "0,1,2"), (True, "0,1"), (True, "0,3")):
                with self.assertRaises(ValueError):
                    self.initialize(parser, self.group(0, flag, levels))

    def test_exact_request_boolean_conflicts_still_fail(self):
        for parser in self.parsers:
            self.initialize(parser, self.group(0, None, "0,1,2"), self.group(1, None, "3"))
            for flag, selected in ((True, "0"), (False, "1"), (False, "3"), (False, "0,1,2")):
                self.flag, self.selected = flag, selected
                with self.assertRaises(ValueError):
                    parser.decode_tiling()

    def test_mixed_levels_keys_keep_each_logical_boolean(self):
        self.assertEqual(self.helper.normalize_deterministic(None, "2,0,1"), (True, (0, 1, 2)))
        self.assertEqual(self.helper.normalize_deterministic("ignore", "0,1,2"), (True, (0, 1, 2)))
        with self.assertRaises(ValueError):
            self.helper.normalize_deterministic("ignore", "3")
        self.assertEqual(
            self.helper.level_simplified_keys(["AddCustom/d=1,p=1/x"], "0,1,2"),
            ["AddCustom/d=0,p=1,dt=0/x", "AddCustom/d=1,p=1,dt=1/x", "AddCustom/d=1,p=1,dt=2/x"],
        )
        with self.assertRaises(ValueError):
            self.helper.normalize_deterministic(False, "0,1,2")

    def test_missing_metadata_completed_across_mixed_groups(self):
        for parser in self.parsers:
            self.initialize(parser, self.group(0, True), self.group(1, None, "3"))
            self.flag, self.selected = True, "3"
            self.assertEqual(set(parser.decode_tiling()), {1})
            self.selected = "1,2"
            decoded = parser.decode_tiling()
            self.assertEqual(set(decoded), {0})
            self.assertEqual(decoded[0]["deterministicLevels"], [1])
            self.initialize(parser, self.group(0), self.group(1, None, "3"))
            self.flag, self.selected = False, "0"
            self.assertEqual(parser.decode_tiling()[0]["deterministicLevels"], [0])

    def test_unfiltered_legacy_keeps_exact_dictionary_shape(self):
        for parser in self.parsers:
            self.initialize(parser, self.group(0, True), self.group(1, False))
            self.flag, self.selected = True, None
            decoded = parser.decode_tiling()
            self.assertEqual(set(decoded), {0, 1})
            self.assertTrue(all("deterministicLevels" not in item for item in decoded.values()))
            self.selected = "3"
            self.assertEqual(parser.decode_tiling(), {})

    def test_modern_offline_context_reaches_adapter_wrapper(self):
        parser = self.parsers[1]
        self.initialize(parser, self.group(0, False, "0"), self.group(1, True, "3"))
        self.flag, self.selected = True, "3"
        with patch.object(parser, "get_context", return_value=None), patch.object(
            parser, "get_current_build_config", return_value=False
        ):
            self.assertEqual(set(parser.decode_tiling()), {1})
        self.selected = None
        helper = sys.modules["asc_op_compile_base.adapter.deterministic"]
        self.assertEqual(helper.offline_selection(None, False), (None, False))

    def test_conflicting_and_duplicate_template_metadata_rejected(self):
        for parser in self.parsers:
            for flag, level in ((True, "0"), (False, "1"), (False, "2"), (False, "3")):
                with self.subTest(parser=parser.__name__, flag=flag, level=level), self.assertRaises(ValueError):
                    self.initialize(parser, self.group(0, flag, level))
            duplicate = self.group(0, True, "1")[:-1] + ",@@ASCENDC_TPL_DETERMINISTIC_LEVEL_SEL@@ = {2}}"
            with self.assertRaises(ValueError):
                self.initialize(parser, duplicate)

    def test_actual_support_info_producer_mixed_ignore_does_not_change_task_boolean(self):
        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        constants = load("support_info_test_constants", opc / "constant.py")
        tree = ast.parse((opc / "single_op_post_compile.py").read_text())
        cls = next(node for node in tree.body if isinstance(node, ast.ClassDef) and node.name == "SingleOpPostCompile")
        cls.body = [
            node for node in cls.body if isinstance(node, ast.FunctionDef) and node.name == "__generate_op_support_info"
        ]
        cls.bases = [ast.Name(id="Base", ctx=ast.Load())]
        ast.fix_missing_locations(cls)

        class Base:
            @staticmethod
            def gen_notnone_param(target, key, value):
                if value is not None:
                    target[key] = value

        namespace = {
            "Base": Base,
            "copy": copy,
            "OpcOptions": constants.OpcOptions,
            "CompileParam": constants.CompileParam,
            "SupportInfo": constants.SupportInfo,
            "logger": types.SimpleNamespace(info=lambda *_: None),
            "opc_log_full": lambda *_: None,
            "LogLevel": types.SimpleNamespace(DEBUG=0),
        }
        exec(
            compile(ast.Module(body=[cls], type_ignores=[]), str(opc / "single_op_post_compile.py"), "exec"), namespace
        )
        producer = namespace["SingleOpPostCompile"]()
        producer._opc_compile_args = {}
        producer._simplified_key_mode = None
        producer._SingleOpPostCompile__generate_static_key = lambda *_: "static"
        producer._SingleOpPostCompile__generate_simplified_key = lambda *_: []
        producer._SingleOpPostCompile__change_inoroutputs = lambda value: value
        producer.adjust_support_info_inputoutput = lambda *_: None
        raw = ["true"]
        producer.parse_info_from_compiled_json = lambda _: (raw[0], None, None, None)
        info = {"deterministic_level": "0,1,2", "deterministic": "true"}
        generated = producer._SingleOpPostCompile__generate_op_support_info({}, info, "raw.json")
        self.assertEqual(generated["deterministic"], "ignore")
        self.assertEqual(generated["deterministicLevels"], [0, 1, 2])
        self.assertEqual(info["deterministic"], "true")
        info["deterministic_level"] = "3"
        self.assertEqual(
            producer._SingleOpPostCompile__generate_op_support_info({}, info, "raw.json")["deterministic"], "true"
        )
        info["deterministic_level"], raw[0] = "0", "false"
        self.assertEqual(
            producer._SingleOpPostCompile__generate_op_support_info({}, info, "raw.json")["deterministic"], "false"
        )
        info["deterministic_level"] = "0,1,2"
        with self.assertRaises(ValueError):
            producer._SingleOpPostCompile__generate_op_support_info({}, info, "raw.json")

    def test_preprocess_discovery_returns_groups_before_inference_or_native(self):
        context = sys.modules["asc_op_compile_base.common.context"].get_context()
        self.additions["deterministic_discovery_only"] = True
        for parser, package in zip(self.parsers, ("asc_op_compiler", "adapter")):
            groups = self.group(0, None, "0,1,2") + "," + self.group(1, None, "3")
            content = self.declaration() + "\n@@ASCENDC_TPL_LISTS@@ = {" + groups + "}\n"
            self.assertEqual(parser.discover_deterministic_groups(content), [[0, 1, 2], [3]])
            self.assertEqual(parser.discover_deterministic_groups("void old_kernel() {}"), [])
            path = BUILD / package / "kernel_info_infer.py"
            tree = ast.parse(path.read_text())
            method = next(
                node
                for node in ast.walk(tree)
                if isinstance(node, ast.FunctionDef) and node.name == "infer_info_from_ifile"
            )
            method.decorator_list = []
            namespace = {
                "OpInfo": object,
                "CORE_TYPE_MIX": 0,
                "CommonUtility": types.SimpleNamespace(
                    is_v220=lambda: False, is_c310=lambda: False, is_920r1=lambda: False, is_920r2=lambda: False
                ),
                "global_var_storage": types.SimpleNamespace(get_variable=lambda _: False),
                "get_level_discovery_context": lambda: context,
                "discover_deterministic_groups": parser.discover_deterministic_groups,
            }
            exec(compile(ast.Module(body=[method], type_ignores=[]), str(path), "exec"), namespace)
            with tempfile.TemporaryDirectory() as directory:
                preprocessed = Path(directory) / "kernel.i"
                preprocessed.write_text(content)
                result = namespace["infer_info_from_ifile"](None, str(preprocessed), None, "kernel.cpp", "kernel")
                self.assertIsNone(result)
                self.assertTrue(self.additions["deterministic_discovery_complete"])
                self.assertEqual(self.additions["declared_deterministic_level_groups"], [[0, 1, 2], [3]])
                self.assertEqual(list(Path(directory).glob("*.o")), [])

    def test_compile_boundary_skips_native_only_for_explicit_probe(self):
        for package in ("asc_op_compiler", "adapter"):
            path = BUILD / package / "compile_op.py"
            method = next(
                node
                for node in ast.parse(path.read_text()).body
                if isinstance(node, ast.FunctionDef) and node.name == "compile_op"
            )
            calls = []
            utility = types.SimpleNamespace(
                is_c310=lambda: False,
                is_920r1=lambda: False,
                is_920r2=lambda: False,
                is_m510=lambda: False,
                is_516r2=lambda: False,
                get_kernel_meta_dir=lambda: ".",
                get_distinct_filename_tag=lambda: "",
                print_compile_log=lambda *_: None,
            )
            namespace = {
                "__name__": "asc_op_compile_base." + package + ".compile_op",
                "__package__": "asc_op_compile_base." + package,
                "os": os,
                "OpInfo": object,
                "CommonUtility": utility,
                "CompileOptionTuple": lambda options, _: types.SimpleNamespace(compile_options=options),
                "LogUtil": types.SimpleNamespace(detail_log_print=lambda *_: None),
                "COMPILE_STAGE_MSG_INFO": {
                    key: key for key in ("compile_op_start", "preprocess_start", "preprocess_end")
                },
                "AscendCLogLevel": types.SimpleNamespace(LOG_INFO=1),
                "global_var_storage": types.SimpleNamespace(
                    global_storage_reset=lambda: None, get_variable=lambda _: False
                ),
                "_add_op_compile_options_by_customized_json": lambda *_: None,
                "compile_pre_process": lambda _, options: options,
                "DFXSectionGenerator": lambda: types.SimpleNamespace(dfx_info_reset=lambda _: None),
                "_update_compile_option": lambda *_: None,
                "_set_compile_info": lambda *_: None,
                "KernelInfoInfer": types.SimpleNamespace(
                    get_tiling_key_list_and_simple_infer_code_channel=lambda *_: None
                ),
                "get_context": sys.modules["asc_op_compile_base.common.context"].get_context,
                "compile_op_common_part": lambda *_: calls.append("native"),
            }
            exec(compile(ast.Module(body=[method], type_ignores=[]), str(path), "exec"), namespace)
            with tempfile.TemporaryDirectory() as directory:
                source = Path(directory) / "kernel.cpp"
                source.write_text("void kernel() {}")
                info = types.SimpleNamespace(kernel_name="AddCustom", impl_mode="")
                self.additions["deterministic_discovery_only"] = True
                namespace["compile_op"](str(source), "kernel", info)
                self.assertEqual(calls, [])
                self.additions["deterministic_discovery_only"] = False
                namespace["compile_op"](str(source), "kernel", info)
                self.assertEqual(calls, ["native"])

    def test_discovery_wrapper_paths_do_not_read_missing_output_json(self):
        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        constants = load("discovery_call_constants", opc / "constant.py")
        tree = ast.parse((opc / "single_op_compile.py").read_text())
        cls = next(node for node in tree.body if isinstance(node, ast.ClassDef) and node.name == "SingleOpCompile")
        cls.body = [node for node in cls.body if isinstance(node, ast.FunctionDef) and node.name == "__call_op"]
        additions = {}
        context = types.SimpleNamespace(
            add_addition=lambda key, value: additions.update({key: value}),
            get_addition=lambda key: additions.get(key),
            add_op_info=lambda _: None,
        )

        class Scope:
            def __init__(self, mode):
                pass

            def __enter__(self):
                return context

            def __exit__(self, *args):
                pass

        def forbidden(*args):
            raise AssertionError("discovery must not access a compiled JSON")

        mode = ["false"]
        namespace = {
            "copy": copy,
            "OpcOptions": constants.OpcOptions,
            "CompileParam": constants.CompileParam,
            "OpModeType": constants.OpModeType,
            "LogLevel": types.SimpleNamespace(INFO=1),
            "logger": types.SimpleNamespace(info=lambda *_: None, debug=lambda *_: None),
            "operator_info": types.SimpleNamespace(OpInfo=lambda *_: types.SimpleNamespace()),
            "get_new_attrs_for_op_compile": lambda *_: [],
            "get_dynamic_compile_static_from_opstore": lambda _: mode[0],
            "infer_simplified_key_mode": lambda *_: [None] * 4,
            "op_context": types.SimpleNamespace(OpContext=Scope, get_context=lambda: context),
            "parse_levels": self.helper.parse_levels,
            "opc_log_full": lambda *_: None,
            "get_json_file_path": forbidden,
            "update_compile_info": forbidden,
        }
        exec(compile(ast.Module(body=[cls], type_ignores=[]), str(opc / "single_op_compile.py"), "exec"), namespace)
        wrapper = namespace["SingleOpCompile"]()
        wrapper._SingleOpCompile__get_kwargs = lambda: {}
        wrapper.get_op_info_for_context = lambda *_: None
        wrapper.set_extra_settings_for_context = lambda *_: None
        wrapper.set_kernel_spec_for_context = lambda *_: None
        wrapper._SingleOpCompile__op = {}
        wrapper._SingleOpCompile__opc_compile_args = {}

        def operator(*args, **kwargs):
            self.assertTrue(additions["deterministic_discovery_only"])
            additions.update(
                deterministic_discovery_complete=True, declared_deterministic_level_groups=[[0, 1, 2], [3]]
            )

        for dynamic, static_mode in ((True, "false"), (False, "true"), (False, "false")):
            additions.clear()
            mode[0] = static_mode
            wrapper._SingleOpCompile__op_info = {
                "_deterministic_discovery_only": True,
                "op_func_attr": operator,
                "is_dynamic": dynamic,
                "inputs": [],
                "outputs": [],
                "op_type": "AddCustom",
            }
            self.assertEqual(wrapper._SingleOpCompile__call_op(), [[0, 1, 2], [3]])

    def test_offline_entry_expansion_and_legacy_dispatch(self):
        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        constants = load("deterministic_test_constants", opc / "constant.py")
        tree = ast.parse((opc / "op_compilation.py").read_text())
        cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "OpCompilation")
        methods = {"__op_compile_by_json_info", "__auto_split_levels", "__single_op_compile_all_deterministic"}
        cls.body = [n for n in cls.body if isinstance(n, ast.FunctionDef) and n.name in methods]

        def update(key, value, path):
            data = json.loads(Path(path).read_text())
            data[key] = value
            Path(path).write_text(json.dumps(data))

        namespace = {
            "copy": copy,
            "os": os,
            "shutil": shutil,
            "hashlib": hashlib,
            "json": json,
            "OpcOptions": constants.OpcOptions,
            "CompileParam": constants.CompileParam,
            "GraphDefParam": constants.GraphDefParam,
            "get_enable_vector_core_from_opstore": lambda _: False,
            "get_file_real_path": lambda output, name, suffix: str(Path(output) / (name + "." + suffix)),
            "read_json_file": lambda path: json.loads(Path(path).read_text()),
            "update_json_file": update,
            "logger": types.SimpleNamespace(
                info=lambda *_: None, warn=lambda *_: None, error=lambda *_: None, debug=lambda *_: None
            ),
        }
        exec(compile(ast.Module(body=[cls], type_ignores=[]), str(opc / "op_compilation.py"), "exec"), namespace)
        compilation = namespace["OpCompilation"]
        compilation.set_flag_before_compile = lambda *args: None
        compilation.set_l2_mode_flag = lambda *args: None
        compilation.get_l2_mode = lambda *args: "0"
        config = {"op_type": "AddCustom", "op_list": [{"bin_filename": "AddCustom_test"}]}
        for declared in (True, "two", False, "other"):
            for requested in ("all", None, "true", "false"):
                with self.subTest(declared=declared, requested=requested), tempfile.TemporaryDirectory() as output:
                    native, probes = [], []
                    groups = (
                        [[0], [1, 2], [3]]
                        if declared is True
                        else ([[0, 1, 2], [3]] if declared == "two" else [[3]] if declared == "other" else [])
                    )

                    def compile_device(compiler, op, info, idx):
                        name = op["bin_filename"]
                        info["kernel_name"] = name
                        if info.get("_deterministic_discovery_only"):
                            probes.append(name)
                            return groups
                        native.append((name, copy.deepcopy(info)))
                        data = {
                            "binFileName": name,
                            "supportInfo": {"deterministic": info.get("deterministic", "false")},
                        }
                        (Path(output) / (name + ".json")).write_text(json.dumps(data))
                        (Path(output) / (name + ".o")).write_bytes(b"device")

                    compilation._OpCompilation__single_op_compile = compile_device
                    compiler = compilation()
                    compiler._OpCompilation__compile_res = True
                    compiler._OpCompilation__is_ascendc = True
                    compiler._OpCompilation__opc_compile_args = {"deterministic": requested, "output": output}
                    compiler._OpCompilation__op_compile_by_json_info(config)
                    self.assertTrue(compiler._OpCompilation__compile_res)
                    self.assertEqual(probes, ["AddCustom_test_level_discovery"])
                    if declared is True or declared == "two":
                        expected = ["0", "1,2", "3"] if declared is True else ["0,1,2", "3"]
                        self.assertEqual([info["deterministic_level"] for _, info in native], expected)
                        self.assertEqual(
                            [name for name, _ in native],
                            ["AddCustom_test_level" + level.replace(",", "_") for level in expected],
                        )
                        self.assertEqual(len(list(Path(output).glob("*.o"))), len(expected))
                        self.assertFalse((Path(output) / "AddCustom_test.o").exists())
                        self.assertFalse((Path(output) / "AddCustom_test_deterministic.o").exists())
                    else:
                        self.assertEqual(len(native), 2 if requested == "all" else 1)
                    self.assertEqual(config["op_list"][0]["bin_filename"], "AddCustom_test")
        # An explicit grade is one task and does not run capability discovery.
        with tempfile.TemporaryDirectory() as output:
            calls = []
            compilation._OpCompilation__single_op_compile = lambda _, op, info, idx: calls.append(copy.deepcopy(info))
            compiler = compilation()
            compiler._OpCompilation__compile_res = True
            compiler._OpCompilation__is_ascendc = True
            compiler._OpCompilation__opc_compile_args = {
                "deterministic": "all",
                "deterministic_level": "0,1,2",
                "output": output,
            }
            compiler._OpCompilation__op_compile_by_json_info(config)
            self.assertEqual(len(calls), 1)
            self.assertEqual(calls[0]["deterministic_level"], "0,1,2")
            self.assertNotIn("_deterministic_discovery_only", calls[0])

    def test_real_elf_metadata_generator_uses_each_compile_boolean(self):
        for package in ("asc_op_compiler", "adapter"):
            path = BUILD / package / "ascendc_compile_dfx.py"
            tree = ast.parse(path.read_text())
            method = next(
                node
                for node in ast.walk(tree)
                if isinstance(node, ast.FunctionDef) and node.name == "generate_meta_info_func_section"
            )
            namespace = {"get_current_build_config": lambda _: self.flag}
            exec(compile(ast.Module(body=[method], type_ignores=[]), str(path), "exec"), namespace)
            for flag in (False, True):
                self.flag = flag
                section = namespace["generate_meta_info_func_section"](None, "134228013", None, "AddCustom")
                self.assertIn("{{F_TYPE_DETERMINISTIC_INFO, 4}, " + str(int(flag)) + "}", section)
                self.assertIn('section (".ascend.meta.AddCustom")', section)

    def test_impl_mode_normalization_keeps_optional_only_for_same_task(self):
        import inspect

        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        constants = load("optional_test_constants", opc / "constant.py")
        tree = ast.parse((opc / "op_compilation.py").read_text())
        cls = next(node for node in tree.body if isinstance(node, ast.ClassDef) and node.name == "OpCompilation")
        cls.body = [
            node
            for node in cls.body
            if isinstance(node, ast.FunctionDef) and node.name in {"__set_impl_mode", "__check_update_impl_mode"}
        ]
        logger = types.SimpleNamespace(info=lambda *_: None, debug=lambda *_: None)
        namespace = {
            "inspect": inspect,
            "OpcOptions": constants.OpcOptions,
            "OpImplType": constants.OpImplType,
            "CompileParam": constants.CompileParam,
            "logger": logger,
            "check_and_normalize_impl_mode": lambda mode: (True, mode),
            "normalize_optional_impl_mode": lambda mode: ",".join(
                item for item in mode.split(",") if item != "optional"
            ),
        }
        exec(compile(ast.Module(body=[cls], type_ignores=[]), str(opc / "op_compilation.py"), "exec"), namespace)
        compiler = namespace["OpCompilation"]()
        compiler._OpCompilation__opc_compile_args = {}
        compiler._OpCompilation__is_ascendc = True

        def operator(impl_mode="high_precision"):
            pass

        info = {"op_func_attr": operator, "impl_mode": "high_performance,optional"}
        compiler._OpCompilation__set_impl_mode(info)
        self.assertTrue(info["optional_impl_mode_flag"])
        self.assertEqual(info["impl_mode"], "high_performance")
        repeated = copy.deepcopy(info)
        compiler._OpCompilation__set_impl_mode(repeated)
        self.assertTrue(repeated["optional_impl_mode_flag"])
        repeated["impl_mode"] = "high_precision,high_performance"
        compiler._OpCompilation__set_impl_mode(repeated)
        self.assertFalse(repeated["optional_impl_mode_flag"])
        other = {"op_func_attr": operator, "impl_mode": "high_performance"}
        compiler._OpCompilation__set_impl_mode(other)
        self.assertFalse(other["optional_impl_mode_flag"])

    def test_native_host_encoder_ignores_metadata(self):
        compiler = shutil.which("clang++") or shutil.which("g++")
        if not compiler:
            self.skipTest("C++ compiler unavailable")
        source = r"""#define ASCENDC_DEBUG
#include "utils/tiling/template_argument.h"
ASCENDC_TPL_ARGS_DECL(AddCustom, ASCENDC_TPL_BOOL_DECL(B, 0, 1));
ASCENDC_TPL_SEL(ASCENDC_TPL_ARGS_SEL(ASCENDC_TPL_BOOL_SEL(B, 0, 1),
ASCENDC_TPL_DETERMINISTIC_SEL(true), ASCENDC_TPL_DETERMINISTIC_LEVEL_SEL(1,2)));
int main() { return GET_TPL_TILING_KEY(0) != 0 || GET_TPL_TILING_KEY(1) != 1; }
"""
        with tempfile.TemporaryDirectory() as directory:
            program = Path(directory) / "encoder.cpp"
            program.write_text(source)
            executable = Path(directory) / "encoder"
            subprocess.run(
                [
                    compiler,
                    "-std=c++17",
                    "-D_GLIBCXX_USE_CXX11_ABI=0",
                    "-I" + str(ROOT / "include"),
                    str(program),
                    str(ROOT / "impl/utils/tiling/template_argument.cpp"),
                    "-o",
                    str(executable),
                ],
                check=True,
                capture_output=True,
                text=True,
            )
            subprocess.run([str(executable)], check=True, capture_output=True, text=True)

    def test_framework_fallback_keys_keep_legacy_and_emit_canonical_dt(self):
        utility = load("deterministic_framework_insert", ROOT / "cmake/asc/fwk_modules/util/insert_simplified_keys.py")
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "bin.json"
            descriptor = {
                "binFileName": "AddCustom_FP32",
                "supportInfo": {
                    "staticKey": "old",
                    "deterministic": "true",
                    "implMode": "high_performance",
                    "deterministicLevels": [1, 2],
                    "inputs": [{"dtype": "float32", "format": "ND"}],
                },
            }
            path.write_text(json.dumps(descriptor, indent=2))
            utility.insert_simplified_keys(str(path))
            generated = json.loads(path.read_text())
            self.assertEqual(
                generated["supportInfo"]["simplifiedKey"],
                ["AddCustom/d=1,p=1,dt=1/0,2/", "AddCustom/d=1,p=1,dt=2/0,2/"],
            )
            utility.insert_simplified_keys(str(path))
            self.assertEqual(json.loads(path.read_text()), generated)
            descriptor["supportInfo"]["deterministicLevels"] = [0, 1, 2]
            path.write_text(json.dumps(descriptor, indent=2))
            utility.insert_simplified_keys(str(path))
            self.assertEqual(
                json.loads(path.read_text())["supportInfo"]["simplifiedKey"],
                ["AddCustom/d=0,p=1,dt=0/0,2/", "AddCustom/d=1,p=1,dt=1/0,2/", "AddCustom/d=1,p=1,dt=2/0,2/"],
            )
            descriptor["supportInfo"].pop("deterministicLevels")
            path.write_text(json.dumps(descriptor, indent=2))
            utility.insert_simplified_keys(str(path))
            self.assertEqual(json.loads(path.read_text())["supportInfo"]["simplifiedKey"], "AddCustom/d=1,p=1,o=0/0,2/")
            descriptor["supportInfo"]["deterministicLevels"] = [0]
            path.write_text(json.dumps(descriptor, indent=2))
            with self.assertRaises(ValueError):
                utility.insert_simplified_keys(str(path))

    def test_real_header_preprocessing(self):
        compiler = shutil.which("clang++") or shutil.which("g++")
        if not compiler:
            self.skipTest("C++ preprocessor unavailable")
        source = """#define ASCENDC_TPL_PRE
#include "template_argument.h"
ASCENDC_TPL_ARGS_DECL(AddCustom, ASCENDC_TPL_BOOL_DECL(B, 0, 1));
ASCENDC_TPL_SEL(ASCENDC_TPL_ARGS_SEL(ASCENDC_TPL_BOOL_SEL(B, 0),
ASCENDC_TPL_DETERMINISTIC_SEL(true), ASCENDC_TPL_DETERMINISTIC_LEVEL_SEL(1,2)));
"""
        result = subprocess.run(
            [compiler, "-std=c++17", "-E", "-P", "-x", "c++", "-I" + str(ROOT / "include/utils/tiling"), "-"],
            input=source,
            text=True,
            capture_output=True,
            check=True,
        )
        start = result.stdout.index("@@ASCENDC_TPL_ARGS_DECL_AddCustom@@")
        split = result.stdout.index("@@ASCENDC_TPL_LISTS@@", start)
        self.flag, self.selected = True, "1,2"
        for parser in self.parsers:
            parser.extract_template_tiling_info(result.stdout[start:split], result.stdout[split:])
            decoded = parser.decode_tiling()
            self.assertEqual(set(decoded), {0})
            self.assertEqual(decoded[0]["deterministicLevels"], [1, 2])
            self.assertEqual(decoded[0]["paramArgs"], ["0"])


if __name__ == "__main__":
    unittest.main()
