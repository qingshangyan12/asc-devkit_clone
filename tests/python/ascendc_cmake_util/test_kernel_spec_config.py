#!/usr/bin/env python3
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

import subprocess
import sys
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
UTIL_DIR = REPO_ROOT / "cmake/asc/fwk_modules/util"
sys.path.insert(0, str(UTIL_DIR))

import ascendc_gen_options


@pytest.mark.parametrize("op_type,chip", [("ALL", ""), ("AddCustom", "ascend910b")])
def test_kernel_options_entry_rejects_kernel_spec_flags(tmp_path, op_type, chip):
    config_file = tmp_path / "custom_compile_options.ini"
    with pytest.raises(RuntimeError, match="KERNEL_SPEC_MODE"):
        ascendc_gen_options.gen_compile_options(
            str(config_file), op_type, chip, ["--kernel-spec=Normal", f"--kernel-spec-dir={tmp_path}/resources"]
        )
    assert not config_file.exists()
    assert not (tmp_path / "custom_opc_options.ini").exists()


def test_scoped_ordinary_device_options_are_unchanged(tmp_path):
    config_file = tmp_path / "custom_compile_options.ini"
    ascendc_gen_options.gen_compile_options(str(config_file), "AddCustom", "ascend910b", ["-O2", "--save-temp-files"])
    assert config_file.read_text() == "AddCustom,ascend910b,-O2\n"
    assert (tmp_path / "custom_opc_options.ini").read_text() == "AddCustom@ascend910b@--op_debug_config=dump_cce\n"


@pytest.mark.parametrize("mode", ["Normal", "Invalid"])
def test_cmake_package_kernel_spec_mode(tmp_path, mode):
    func_cmake = REPO_ROOT / "cmake/asc/fwk_modules/func.cmake"
    (tmp_path / "CMakeLists.txt").write_text(
        f"""cmake_minimum_required(VERSION 3.16)
project(kernel_spec_config NONE)
include("{func_cmake}")
npu_op_package(demo TYPE RUN CONFIG KERNEL_SPEC_MODE {mode})
get_property(mode GLOBAL PROPERTY _ASC_PKG_demo_KERNEL_SPEC_MODE)
if(NOT mode STREQUAL "{mode}")
    message(FATAL_ERROR "unexpected Kernel Spec mode: ${{mode}}")
endif()
""",
        encoding="utf-8",
    )
    result = subprocess.run(
        ["cmake", "-S", str(tmp_path), "-B", str(tmp_path / "build")], text=True, capture_output=True
    )
    if mode == "Invalid":
        assert result.returncode != 0
        assert "KERNEL_SPEC_MODE must be None, Normal or SK" in result.stderr
    else:
        assert result.returncode == 0, result.stdout + result.stderr
