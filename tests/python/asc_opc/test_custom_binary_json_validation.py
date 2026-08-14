#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
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
import types

import pytest


FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
FRAMEWORK_PATH = os.path.join(TOP_PATH, "tools/build/")
ASC_OPC_PATH = os.path.join(TOP_PATH, "tools/build/asc_opc/python/asc_opc_tool/")
OPC_STUB_PATH = os.path.join(TOP_PATH, "tests/python/asc_opc/stub/")
UTIL_PATH = os.path.join(TOP_PATH, "cmake/asc/fwk_modules/util/")
sys.path.insert(0, FRAMEWORK_PATH)
sys.path.insert(0, ASC_OPC_PATH)
sys.path.insert(0, OPC_STUB_PATH)
sys.path.insert(0, UTIL_PATH)

log_stub = types.ModuleType("asc_op_compile_base.common.utils.log")
for method_name in ("debug", "info", "warn", "warning", "error"):
    setattr(log_stub, method_name, lambda *_args, **_kwargs: None)
utils_stub = types.ModuleType("asc_op_compile_base.common.utils")
utils_stub.log = log_stub
common_stub = types.ModuleType("asc_op_compile_base.common")
common_stub.utils = utils_stub
package_stub = types.ModuleType("asc_op_compile_base")
package_stub.common = common_stub
sys.modules["asc_op_compile_base"] = package_stub
sys.modules["asc_op_compile_base.common"] = common_stub
sys.modules["asc_op_compile_base.common.utils"] = utils_stub
sys.modules["asc_op_compile_base.common.utils.log"] = log_stub

op_manager_stub = types.ModuleType("op_manager")
op_manager_stub.get_inout_info_from_opstore = lambda _op_type: (None, None)
sys.modules["op_manager"] = op_manager_stub

from constant import OpcOptions
import op_compile_info_check as CHECK_MODULE
from split_opc_json_with_op_list import split_json_files


check_op_compilation_json = CHECK_MODULE.check_op_compilation_json


class RecordingLogger:
    def __init__(self):
        self.errors = []
        self.warnings = []

    def error(self, message, *args, **kwargs):
        self.errors.append(message % args if args else message)

    def debug(self, _message, *_args, **_kwargs):
        pass

    def warn(self, message, *args, **kwargs):
        self.warnings.append(message % args if args else message)


def _valid_config():
    return {
        "op_type": "AddCustomTemplate",
        "op_list": [
            {
                "bin_filename": "AddCustomTemplate_FP16",
                "simplified_key": "custom_fp16",
                "inputs": [
                    {
                        "name": "x",
                        "index": 0,
                        "dtype": "float16",
                        "format": "ND",
                        "shape": [-2],
                    },
                    {},
                ],
                "outputs": [
                    {
                        "name": "z",
                        "index": 0,
                        "dtype": "float16",
                        "format": "ND",
                        "shape": [-2],
                    }
                ],
            }
        ],
    }


def _check(tmp_path, content, strict=True):
    path = tmp_path / "binary.json"
    path.write_text(json.dumps(content), encoding="utf-8")
    if strict:
        output_dir = tmp_path / "split"
        output_dir.mkdir()
        try:
            generated_files = split_json_files(
                str(path),
                str(output_dir),
                expected_op_type="AddCustomTemplate",
                require_simplified_key=True,
            )
        except ValueError:
            return False
        path = generated_files[0]
    return check_op_compilation_json(
        OpcOptions.INPUT_PARAM, {OpcOptions.INPUT_PARAM: str(path)}
    )[0]


def test_accepts_valid_custom_binary_dtype_and_format(tmp_path):
    assert _check(tmp_path, _valid_config()) is True


def test_accepts_format_stored_as_tuple_constant(tmp_path):
    content = _valid_config()
    content["op_list"][0]["inputs"][0]["format"] = "DHWNC"

    assert _check(tmp_path, content) is True


@pytest.mark.parametrize(
    ("field", "value"),
    [
        ("dtype", "fp17"),
        ("dtype", ["float16"]),
        ("format", "NOT_A_FORMAT"),
        ("format", {"name": "ND"}),
    ],
)
def test_rejects_invalid_custom_binary_dtype_or_format(tmp_path, field, value):
    content = _valid_config()
    content["op_list"][0]["inputs"][0][field] = value

    assert _check(tmp_path, content) is False


def test_rejects_invalid_dynamic_tensor_dtype(tmp_path):
    content = _valid_config()
    content["op_list"][0]["inputs"] = [
        [
            {
                "name": "x",
                "index": 0,
                "dtype": "fp17",
                "format": "ND",
                "shape": [-2],
            }
        ]
    ]

    assert _check(tmp_path, content) is False


def test_invalid_value_message_contains_file_and_field_path(tmp_path, monkeypatch):
    content = _valid_config()
    content["op_list"][0]["outputs"][0]["format"] = "NOT_A_FORMAT"
    path = tmp_path / "input.json"
    path.write_text(json.dumps(content), encoding="utf-8")
    output_dir = tmp_path / "split"
    output_dir.mkdir()
    generated_files = split_json_files(
        str(path),
        str(output_dir),
        expected_op_type="AddCustomTemplate",
        require_simplified_key=True,
    )
    logger = RecordingLogger()
    monkeypatch.setattr(CHECK_MODULE, "logger", logger)

    result, _ = check_op_compilation_json(
        OpcOptions.INPUT_PARAM, {OpcOptions.INPUT_PARAM: generated_files[0]}
    )

    assert result is False
    error = logger.errors[-1]
    assert str(path) in error
    assert "op_list[0].outputs[0].format" in error
    assert "NOT_A_FORMAT" in error


def test_invalid_value_message_keeps_original_op_index(tmp_path, monkeypatch):
    content = _valid_config()
    second_op = json.loads(json.dumps(content["op_list"][0]))
    second_op["bin_filename"] = "AddCustomTemplate_FP32"
    second_op["inputs"][0]["dtype"] = "fp17"
    content["op_list"].append(second_op)
    path = tmp_path / "binary.json"
    path.write_text(json.dumps(content), encoding="utf-8")
    output_dir = tmp_path / "split"
    output_dir.mkdir()
    generated_files = split_json_files(
        str(path),
        str(output_dir),
        expected_op_type="AddCustomTemplate",
        require_simplified_key=True,
    )
    logger = RecordingLogger()
    monkeypatch.setattr(CHECK_MODULE, "logger", logger)

    result, _ = check_op_compilation_json(
        OpcOptions.INPUT_PARAM, {OpcOptions.INPUT_PARAM: generated_files[1]}
    )

    assert result is False
    assert str(path) in logger.errors[-1]
    assert "op_list[1].inputs[0].dtype" in logger.errors[-1]


def test_standard_mode_keeps_existing_validation_behavior(tmp_path):
    content = _valid_config()
    del content["op_list"][0]["simplified_key"]
    content["op_list"][0]["inputs"][0]["dtype"] = "legacy_custom_dtype"

    assert _check(tmp_path, content, strict=False) is True


@pytest.mark.parametrize(
    ("content", "expected_error"),
    [
        ([], "root: expected object"),
        ({"op_type": "AddCustomTemplate", "op_list": []}, "op_list: expected non-empty array"),
        ({"op_type": "AddCustomTemplate", "op_list": [None]}, "op_list[0]: expected object"),
    ],
)
def test_rejects_invalid_binary_json_structure(tmp_path, content, expected_error):
    path = tmp_path / "binary.json"
    path.write_text(json.dumps(content), encoding="utf-8")

    with pytest.raises(ValueError, match=expected_error.replace("[", r"\[").replace("]", r"\]")):
        split_json_files(
            str(path),
            str(tmp_path),
            expected_op_type="AddCustomTemplate",
            require_simplified_key=True,
        )


def test_rejects_duplicate_binary_filename(tmp_path):
    content = _valid_config()
    content["op_list"].append(dict(content["op_list"][0]))
    path = tmp_path / "binary.json"
    path.write_text(json.dumps(content), encoding="utf-8")

    with pytest.raises(ValueError, match="duplicate output"):
        split_json_files(
            str(path),
            str(tmp_path),
            expected_op_type="AddCustomTemplate",
            require_simplified_key=True,
        )


def test_rejects_binary_filenames_with_same_output_name(tmp_path):
    content = _valid_config()
    second_op = json.loads(json.dumps(content["op_list"][0]))
    second_op["bin_filename"] = "AddCustomTemplate_FP16.json"
    content["op_list"].append(second_op)
    path = tmp_path / "binary.json"
    path.write_text(json.dumps(content), encoding="utf-8")

    with pytest.raises(ValueError, match="duplicate output"):
        split_json_files(
            str(path),
            str(tmp_path),
            expected_op_type="AddCustomTemplate",
            require_simplified_key=True,
        )
