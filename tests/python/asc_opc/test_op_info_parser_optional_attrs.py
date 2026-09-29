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

import importlib
import sys
import types
from pathlib import Path
from types import SimpleNamespace

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
ASC_OPC_PATH = REPO_ROOT / "tools/build/asc_opc/python/asc_opc_tool"


@pytest.fixture
def op_info_parser(monkeypatch):
    monkeypatch.syspath_prepend(str(ASC_OPC_PATH))

    log = types.ModuleType("asc_op_compile_base.common.utils.log")
    for method_name in ("debug", "info", "warn", "warning", "error"):
        setattr(log, method_name, lambda *_args, **_kwargs: None)
    utils = types.ModuleType("asc_op_compile_base.common.utils")
    utils.log = log
    common = types.ModuleType("asc_op_compile_base.common")
    common.utils = utils
    package = types.ModuleType("asc_op_compile_base")
    package.common = common
    monkeypatch.setitem(sys.modules, "asc_op_compile_base", package)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common", common)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.utils", utils)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.utils.log", log)

    opc_common = types.ModuleType("opc_common")
    opc_common.LogLevel = SimpleNamespace(DEBUG=0)
    opc_common.opc_log_full = lambda *_args, **_kwargs: None
    monkeypatch.setitem(sys.modules, "opc_common", opc_common)

    op_manager = types.ModuleType("op_manager")
    op_manager.get_attr_info_from_opstore = lambda _op_type: [
        SimpleNamespace(name="axis", type="int", param_type="optional", default_value=1)
    ]
    monkeypatch.setitem(sys.modules, "op_manager", op_manager)
    monkeypatch.delitem(sys.modules, "op_info_parser", raising=False)

    return importlib.import_module("op_info_parser")


def test_empty_attr_list_still_uses_optional_defaults(op_info_parser):
    op_info = {"op_type": "Example"}
    parser = op_info_parser.OpInfoParser({"attrs": []}, op_info, {})

    parser.get_op_info("Example")

    assert op_info["attrs"] == [{"name": "axis", "dtype": "int", "value": 1}]
