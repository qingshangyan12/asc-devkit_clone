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
def op_info_store(monkeypatch):
    monkeypatch.syspath_prepend(str(ASC_OPC_PATH))

    platform_info = types.ModuleType("asc_op_compile_base.common.platform.platform_info")
    platform = types.ModuleType("asc_op_compile_base.common.platform")
    platform.platform_info = platform_info

    log = types.ModuleType("asc_op_compile_base.common.utils.log")
    for method_name in ("debug", "info", "warn", "warning", "error"):
        setattr(log, method_name, lambda *_args, **_kwargs: None)
    utils = types.ModuleType("asc_op_compile_base.common.utils")
    utils.log = log
    common = types.ModuleType("asc_op_compile_base.common")
    common.platform = platform
    common.utils = utils
    package = types.ModuleType("asc_op_compile_base")
    package.common = common

    monkeypatch.setitem(sys.modules, "asc_op_compile_base", package)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common", common)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.platform", platform)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.platform.platform_info", platform_info)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.utils", utils)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.utils.log", log)
    monkeypatch.delitem(sys.modules, "op_info_store", raising=False)

    return importlib.import_module("op_info_store")


def test_memory_continuity_keys_are_not_treated_as_tensor_entries(op_info_store):
    op_content = {
        "input0": {"paramType": "required", "name": "x"},
        "output0": {"paramType": "required", "name": "y"},
        "inputMemContinues": {"flag": "true"},
        "outputMemContinues": {"value": "true"},
    }
    op_info = op_info_store.OpKernelInfo("Example")

    assert op_info.initialize_op_kernel_info("Example", op_content, op_info) is True
    assert [item.name for item in op_info.input_infos_] == ["x"]
    assert [item.name for item in op_info.output_infos_] == ["y"]
    assert op_info.input_mem_continues is True
    assert op_info.output_mem_continues is True
