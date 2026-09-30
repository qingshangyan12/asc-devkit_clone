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

import shlex
import subprocess
import sys
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
UTIL_DIR = REPO_ROOT / "cmake/asc/fwk_modules/util"
sys.path.insert(0, str(UTIL_DIR))

import kernel_spec_options
import ascendc_gen_options


def test_split_writes_global_kernel_spec_and_preserves_other_options(tmp_path):
    config_file = tmp_path / "custom_opc_options.ini"

    remaining = kernel_spec_options.split_and_write_options(
        ["--kernel-spec=Normal", f"--kernel-spec-dir={tmp_path}/kernel_spec", "-O2"], str(config_file), "ALL", ""
    )

    assert remaining == ["-O2"]
    assert config_file.read_text(encoding="utf-8") == (
        f"ALL@@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/kernel_spec\n"
    )


def test_shell_options_reads_global_rule_and_adds_soc_directory(tmp_path):
    config_file = tmp_path / "custom_opc_options.ini"
    config_file.write_text(f"ALL@@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/kernel_spec\n", encoding="utf-8")

    options = kernel_spec_options.shell_options(str(config_file), "AddCustom", "Ascend910B1")

    assert shlex.split(options) == ["--kernel-spec=Normal", f"--kernel-spec-dir={tmp_path}/kernel_spec/ascend910b"]


def test_gen_compile_options_keeps_device_options_separate(tmp_path):
    compile_file = tmp_path / "custom_compile_options.ini"

    ascendc_gen_options.gen_compile_options(
        str(compile_file), "ALL", "", ["--kernel-spec=Normal", f"--kernel-spec-dir={tmp_path}/kernel_spec", "-O2"]
    )

    assert "-O2" in compile_file.read_text(encoding="utf-8")
    assert "kernel-spec" not in compile_file.read_text(encoding="utf-8")
    assert "--kernel-spec=Normal" in (tmp_path / "custom_opc_options.ini").read_text(encoding="utf-8")


def test_cmake_package_rejects_invalid_kernel_spec_mode(tmp_path):
    func_cmake = REPO_ROOT / "cmake/asc/fwk_modules/func.cmake"
    project = tmp_path / "CMakeLists.txt"
    project.write_text(
        f"""cmake_minimum_required(VERSION 3.16)
project(kernel_spec_config NONE)
include(\"{func_cmake}\")
npu_op_package(demo TYPE RUN CONFIG KERNEL_SPEC_MODE Invalid)
""",
        encoding="utf-8",
    )

    result = subprocess.run(
        ["cmake", "-S", str(tmp_path), "-B", str(tmp_path / "build")], text=True, capture_output=True, check=False
    )

    assert result.returncode != 0
    assert "KERNEL_SPEC_MODE must be None, Normal or SK" in result.stderr


def test_cmake_package_stores_kernel_spec_mode(tmp_path):
    func_cmake = REPO_ROOT / "cmake/asc/fwk_modules/func.cmake"
    project = tmp_path / "CMakeLists.txt"
    project.write_text(
        f"""cmake_minimum_required(VERSION 3.16)
project(kernel_spec_config NONE)
include(\"{func_cmake}\")
npu_op_package(demo TYPE RUN CONFIG KERNEL_SPEC_MODE Normal)
get_property(mode GLOBAL PROPERTY _ASC_PKG_demo_KERNEL_SPEC_MODE)
if(NOT mode STREQUAL \"Normal\")
    message(FATAL_ERROR \"unexpected Kernel Spec mode: ${{mode}}\")
endif()
""",
        encoding="utf-8",
    )

    result = subprocess.run(
        ["cmake", "-S", str(tmp_path), "-B", str(tmp_path / "build")], text=True, capture_output=True, check=False
    )

    assert result.returncode == 0, result.stdout + result.stderr
