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

import contextvars
import copy
import importlib
import sys
import types
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
FRAMEWORK_PATH = REPO_ROOT / "tools/build"


@pytest.fixture
def buildcfg(monkeypatch):
    monkeypatch.syspath_prepend(str(FRAMEWORK_PATH))
    package = types.ModuleType("asc_op_compile_base")
    package.__path__ = [str(FRAMEWORK_PATH / "asc_op_compile_base")]
    common = types.ModuleType("asc_op_compile_base.common")
    common.__path__ = [str(FRAMEWORK_PATH / "asc_op_compile_base/common")]
    platform = types.ModuleType("asc_op_compile_base.common.platform")
    platform.scope_cbuf = "scope_cbuf"
    common.platform = platform
    package.common = common
    monkeypatch.setitem(sys.modules, "asc_op_compile_base", package)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common", common)
    monkeypatch.setitem(sys.modules, "asc_op_compile_base.common.platform", platform)
    monkeypatch.delitem(sys.modules, "asc_op_compile_base.common.buildcfg.buildcfg", raising=False)

    return importlib.import_module("asc_op_compile_base.common.buildcfg.buildcfg")


def _reset_build_config(buildcfg):
    buildcfg._build_cfg.set(copy.deepcopy(buildcfg.cce_default_static_build_config))


def test_nested_values_do_not_leak_between_contexts(buildcfg):
    _reset_build_config(buildcfg)
    buildcfg.set_current_build_config("limit_ir_lines", ["parent"])

    with buildcfg.build_config() as config:
        config.current()["tir.limit_ir_lines"].append("child")
        assert config.current()["tir.limit_ir_lines"] == ["parent", "child"]

    assert buildcfg.get_current_build_config("tir.limit_ir_lines") == ["parent"]


def test_new_context_starts_from_unmodified_defaults(buildcfg):
    _reset_build_config(buildcfg)
    buildcfg.set_current_build_config("limit_ir_lines", ["changed"])

    fresh_context = contextvars.Context()

    assert fresh_context.run(buildcfg.get_current_build_config, "tir.limit_ir_lines") == []
