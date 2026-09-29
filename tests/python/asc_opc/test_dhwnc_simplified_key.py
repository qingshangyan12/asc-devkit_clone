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

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
ASC_OPC_PATH = REPO_ROOT / "tools/build/asc_opc/python/asc_opc_tool"


@pytest.fixture
def simplified_key_utils(monkeypatch):
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
    for name, module in (
        ("asc_op_compile_base", package),
        ("asc_op_compile_base.common", common),
        ("asc_op_compile_base.common.utils", utils),
        ("asc_op_compile_base.common.utils.log", log),
    ):
        monkeypatch.setitem(sys.modules, name, module)

    opc_common = types.ModuleType("opc_common")
    opc_common.check_input_and_output_for_simplified_key = lambda _op: (False, False, False, False)
    opc_common.check_attr_for_simpilified_key = lambda _op: False
    monkeypatch.setitem(sys.modules, "opc_common", opc_common)
    monkeypatch.delitem(sys.modules, "simplified_key_utils", raising=False)

    return importlib.import_module("simplified_key_utils")


def test_dhwnc_format_name_resolves_to_format_value(simplified_key_utils):
    assert simplified_key_utils.get_format_value({"format": "DHWNC"}, 0) == 36
