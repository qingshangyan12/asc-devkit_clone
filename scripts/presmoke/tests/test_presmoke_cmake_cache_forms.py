#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import shlex
import pytest
from presmoke.planner import rewrite_cmake


@pytest.mark.parametrize(
    "definition", ["-DCMAKE_ASC_RUN_MODE:STRING=cpu", "-D CMAKE_ASC_RUN_MODE=cpu", "-D CMAKE_ASC_RUN_MODE:STRING=cpu"]
)
def test_npu_rewrite_removes_old_mode_in_all_cache_forms(definition):
    command = rewrite_cmake(f"cmake {definition} -DOTHER:STRING=keep ..", "dav-2201", "npu")
    assert "CMAKE_ASC_RUN_MODE" not in command
    assert "-DOTHER:STRING=keep" in shlex.split(command)


@pytest.mark.parametrize("definition", ["-DCMAKE_ASC_ARCHITECTURES:STRING=dav-2201", "-D NPU_ARCH:STRING=2201"])
def test_architecture_rewrite_does_not_leave_conflicting_cache_definitions(definition):
    command = rewrite_cmake(f"cmake {definition} ..", "dav-3510", "sim")
    assert "2201" not in command
    assert "-DCMAKE_ASC_ARCHITECTURES=dav-3510" in shlex.split(command)
    assert "-DCMAKE_ASC_RUN_MODE=sim" in shlex.split(command)
