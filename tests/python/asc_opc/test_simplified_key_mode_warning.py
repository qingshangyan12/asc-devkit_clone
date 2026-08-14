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

import sys
import types
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
ASC_OPC_PATH = REPO_ROOT / "tools/build/asc_opc/python/asc_opc_tool"
sys.path.insert(0, str(ASC_OPC_PATH))

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

from constant import OpcOptions
import simplified_key_utils as SIMPLIFIED_KEY_MODULE


class RecordingLogger:
    def __init__(self):
        self.warnings = []

    def warn(self, message, *args, **kwargs):
        self.warnings.append(message % args if args else message)


def _custom_op():
    return {
        "simplified_key": "custom_fp16",
        "inputs": [{"dtype": "float16", "format": "ND"}],
        "outputs": [{"dtype": "float16", "format": "ND"}],
    }


@pytest.mark.parametrize("requested_mode", [0, 1, None])
def test_custom_key_warns_when_mode_2_is_selected(monkeypatch, requested_mode):
    logger = RecordingLogger()
    monkeypatch.setattr(SIMPLIFIED_KEY_MODULE, "logger", logger)
    compile_args = (
        {}
        if requested_mode is None
        else {OpcOptions.SIMPLE_KEY_MODE: requested_mode}
    )

    result = SIMPLIFIED_KEY_MODULE.infer_simplified_key_mode(
        _custom_op(), compile_args
    )

    assert result[0] == 2
    assert logger.warnings == [
        "Custom simplified_key is configured; custom mode 2 will be used."
    ]
