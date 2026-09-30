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


@pytest.mark.parametrize(
    ("op_type", "soc", "expected_mode", "expected_root"),
    [
        ("AddCustom", "Ascend910B1", "Normal", "op_soc"),
        ("AddCustom", "Ascend910C1", "SK", "op_all_soc"),
        ("OtherOp", "Ascend910B1", "SK", "all_op_soc"),
        ("OtherOp", "Ascend910C1", "Normal", "global"),
    ],
)
def test_shell_options_selects_most_specific_scope(tmp_path, op_type, soc, expected_mode, expected_root):
    config_file = tmp_path / "custom_opc_options.ini"
    config_file.write_text(
        f"ALL@@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/global\n"
        f"ALL@ascend910b@--kernel-spec=SK@--kernel-spec-dir={tmp_path}/all_op_soc\n"
        f"AddCustom@@--kernel-spec=SK@--kernel-spec-dir={tmp_path}/op_all_soc\n"
        f"AddCustom@ascend910b@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/op_soc\n",
        encoding="utf-8",
    )

    options = shlex.split(kernel_spec_options.shell_options(str(config_file), op_type, soc))

    assert options == [
        f"--kernel-spec={expected_mode}",
        f"--kernel-spec-dir={tmp_path}/{expected_root}/{kernel_spec_options._normalize_soc(soc)}",
    ]


def test_shell_options_allows_exact_op_to_disable_global_mode(tmp_path):
    config_file = tmp_path / "custom_opc_options.ini"
    config_file.write_text(
        f"ALL@@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/global\nAddCustom@@--kernel-spec=None\n",
        encoding="utf-8",
    )

    assert kernel_spec_options.shell_options(str(config_file), "AddCustom", "Ascend910B1") == ""


def test_shell_options_prefers_exact_op_over_exact_soc(tmp_path):
    config_file = tmp_path / "custom_opc_options.ini"
    config_file.write_text(
        f"ALL@ascend910b@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/all_op_soc\n"
        f"AddCustom@ALL@--kernel-spec=SK@--kernel-spec-dir={tmp_path}/op_all_soc\n",
        encoding="utf-8",
    )

    options = shlex.split(kernel_spec_options.shell_options(str(config_file), "AddCustom", "Ascend910B1"))

    assert options == ["--kernel-spec=SK", f"--kernel-spec-dir={tmp_path}/op_all_soc/ascend910b"]


def test_shell_options_rejects_conflicting_rules_at_same_scope(tmp_path):
    config_file = tmp_path / "custom_opc_options.ini"
    config_file.write_text(
        f"AddCustom@@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/normal\n"
        f"AddCustom@@--kernel-spec=SK@--kernel-spec-dir={tmp_path}/sk\n",
        encoding="utf-8",
    )

    with pytest.raises(ValueError, match="Conflicting kernel specialization scope"):
        kernel_spec_options.shell_options(str(config_file), "AddCustom", "Ascend910B1")


def test_shell_options_does_not_borrow_directory_from_lower_scope(tmp_path):
    config_file = tmp_path / "custom_opc_options.ini"
    config_file.write_text(
        f"ALL@@--kernel-spec=Normal@--kernel-spec-dir={tmp_path}/global\nAddCustom@@--kernel-spec=SK\n",
        encoding="utf-8",
    )

    with pytest.raises(ValueError, match="kernel-spec-dir is required"):
        kernel_spec_options.shell_options(str(config_file), "AddCustom", "Ascend910B1")


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
