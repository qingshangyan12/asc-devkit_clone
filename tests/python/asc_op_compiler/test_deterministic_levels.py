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
                get_addition=lambda name: self.selected
                if name == "deterministic_level_filter"
                else self.additions.get(name),
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

    def test_offline_entry_expansion_and_legacy_dispatch(self):
        # Run production dispatch, discovery, grouping and ordinary reuse with real
        # descriptors; only the expensive device compilation is replaced.
        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        constants = load("deterministic_test_constants", opc / "constant.py")
        tree = ast.parse((opc / "op_compilation.py").read_text())
        cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "OpCompilation")
        methods = {
            "__op_compile_by_json_info",
            "__auto_split_levels",
            "__reuse_level_zero_as_ordinary",
            "__single_op_compile_all_deterministic",
        }
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
        for declared, requested, explicit in [
            (True, "all", None),
            (True, None, None),
            (True, "true", None),
            (True, "false", None),
            (True, "all", "1,2"),
            (False, "all", None),
            (False, None, None),
        ]:
            with self.subTest(
                declared=declared, requested=requested, explicit=explicit
            ), tempfile.TemporaryDirectory() as d:
                calls = []

                def compile_device(compiler, op, info, idx):
                    name = op["bin_filename"]
                    info["kernel_name"] = name
                    calls.append((name, copy.deepcopy(info)))
                    data = {"binFileName": name, "supportInfo": {"deterministic": info.get("deterministic", "true")}}
                    if declared:
                        data["deterministicLevelGroups"] = [[0], [1, 2], [3]]
                    if info.get("deterministic_level"):
                        data["supportInfo"]["deterministicLevels"] = list(
                            map(int, info["deterministic_level"].split(","))
                        )
                    (Path(d) / (name + ".json")).write_text(json.dumps(data))
                    (Path(d) / (name + ".o")).write_bytes(b"device")

                compilation._OpCompilation__single_op_compile = compile_device
                compiler = compilation()
                compiler._OpCompilation__compile_res = True
                compiler._OpCompilation__opc_compile_args = {"deterministic": requested, "output": d}
                if explicit:
                    compiler._OpCompilation__opc_compile_args["deterministic_level"] = explicit
                compiler._OpCompilation__op_compile_by_json_info(config)
                self.assertTrue(compiler._OpCompilation__compile_res)
                if explicit:
                    self.assertEqual(len(calls), 1)
                    self.assertEqual(calls[0][1]["deterministic_level"], explicit)
                elif declared:
                    self.assertEqual(
                        [c[0] for c in calls[1:4]],
                        ["AddCustom_test_level0", "AddCustom_test_level1_2", "AddCustom_test_level3"],
                    )
                    self.assertEqual([c[1]["deterministic_level"] for c in calls[1:4]], ["0", "1,2", "3"])
                    self.assertEqual(len(calls), 4)
                    if requested in {None, "all", "false"}:
                        self.assertEqual(
                            (Path(d) / "AddCustom_test.o").read_bytes(),
                            (Path(d) / "AddCustom_test_level0.o").read_bytes(),
                        )
                    if requested != "false":
                        self.assertTrue((Path(d) / "AddCustom_test_deterministic.o").exists())
                else:
                    self.assertEqual(len(calls), 2 if requested == "all" else 1)
                self.assertEqual(config["op_list"][0]["bin_filename"], "AddCustom_test")

    def test_ordinary_alias_reuses_exact_object_and_removes_level_keys(self):
        opc = ROOT / "tools/build/asc_opc/python/asc_opc_tool"
        constants = load("deterministic_alias_constants", opc / "constant.py")
        tree = ast.parse((opc / "op_compilation.py").read_text())
        cls = next(node for node in tree.body if isinstance(node, ast.ClassDef) and node.name == "OpCompilation")
        cls.body = [
            node
            for node in cls.body
            if isinstance(node, ast.FunctionDef) and node.name == "__reuse_level_zero_as_ordinary"
        ]
        namespace = {
            "OpcOptions": constants.OpcOptions,
            "shutil": shutil,
            "json": json,
            "hashlib": hashlib,
            "get_file_real_path": lambda output, name, suffix: str(Path(output) / (name + "." + suffix)),
            "read_json_file": lambda path: json.loads(Path(path).read_text()),
            "logger": types.SimpleNamespace(info=lambda *_: None),
        }
        exec(compile(ast.Module(body=[cls], type_ignores=[]), str(opc / "op_compilation.py"), "exec"), namespace)
        compiler = namespace["OpCompilation"]()
        with tempfile.TemporaryDirectory() as directory:
            compiler._OpCompilation__opc_compile_args = {"output": directory}
            object_bytes = b"ELF-device-implementation\x00\x01"
            (Path(directory) / "Add_level0.o").write_bytes(object_bytes)
            descriptor = {
                "binFileName": "Add_level0",
                "kernelName": "actual_device_symbol_level0",
                "deterministicLevels": [0],
                "supportInfo": {"deterministicLevels": [0], "simplifiedKey": ["AddCustom/d=0,p=1,dt=0/0,2/0,2/attrs"]},
            }
            (Path(directory) / "Add_level0.json").write_text(json.dumps(descriptor))
            compiler._OpCompilation__reuse_level_zero_as_ordinary(
                {"bin_filename": "Add"}, {"kernel_name": "Add_level0"}
            )
            self.assertEqual((Path(directory) / "Add.o").read_bytes(), object_bytes)
            legacy = json.loads((Path(directory) / "Add.json").read_text())
            self.assertEqual(legacy["kernelName"], descriptor["kernelName"])
            self.assertEqual(legacy["binFileName"], "Add")
            self.assertNotIn("deterministicLevels", legacy)
            self.assertNotIn("deterministicLevels", legacy["supportInfo"])
            self.assertEqual(legacy["supportInfo"]["simplifiedKey"], ["AddCustom/d=0,p=1/0,2/0,2/attrs"])
            self.assertEqual(json.loads((Path(directory) / "Add_level0.json").read_text()), descriptor)

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
