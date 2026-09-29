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
    platform_info.set_current_compile_soc_info = lambda _soc: None
    platform_info.get_soc_spec = lambda _key: "Ascend910"
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

    module = importlib.import_module("op_info_store")
    module.OpPathParse().custom_opp_path_list = []
    module.OpPathParse().vendors_opp_path_list = []
    module.SubOpInfoStore().set_op_custom([])
    module.SubOpInfoStore().set_op_vendors([])
    return module


def test_reloading_clears_removed_custom_opp_paths(tmp_path, monkeypatch, op_info_store):
    monkeypatch.setenv("ASCEND_OPP_PATH", str(tmp_path))
    monkeypatch.setenv("ASCEND_CUSTOM_OPP_PATH", "custom")
    monkeypatch.setattr(op_info_store, "read_json_file", lambda _path: {"CustomOp": {}})

    op_info_store.load_op_info_store("Ascend910")
    assert op_info_store.SubOpInfoStore().op_custom_info_list == [{"CustomOp": {}}]

    monkeypatch.delenv("ASCEND_CUSTOM_OPP_PATH")
    op_info_store.load_op_info_store("Ascend910")

    assert op_info_store.OpPathParse().get_custom_opp_path_list() == []
    assert op_info_store.SubOpInfoStore().op_custom_info_list == []


def test_reloading_replaces_vendor_priority_list(tmp_path, monkeypatch, op_info_store):
    config_dir = tmp_path / "vendors"
    config_dir.mkdir()
    config_path = config_dir / "config.ini"
    monkeypatch.setenv("ASCEND_OPP_PATH", str(tmp_path))

    config_path.write_text("load_priority=vendor_a\n", encoding="utf-8")
    op_info_store.OpPathParse().generate_vendor_opp_list()
    assert [path.replace("\\", "/") for path in op_info_store.OpPathParse().get_vendors_opp_path_list()] == [
        f"{config_dir.as_posix()}/vendor_a"
    ]

    config_path.write_text("load_priority=vendor_b\n", encoding="utf-8")
    op_info_store.OpPathParse().generate_vendor_opp_list()

    assert [path.replace("\\", "/") for path in op_info_store.OpPathParse().get_vendors_opp_path_list()] == [
        f"{config_dir.as_posix()}/vendor_b"
    ]
