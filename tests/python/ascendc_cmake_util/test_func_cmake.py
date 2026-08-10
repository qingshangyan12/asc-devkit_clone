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
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
FWK_MODULES_DIR = REPO_ROOT / "cmake/asc/fwk_modules"


def _configure(tmp_path, kernel_args):
    source_dir = tmp_path / "source"
    build_dir = tmp_path / "build"
    source_dir.mkdir()
    (source_dir / "kernel.cpp").write_text("", encoding="utf-8")
    (source_dir / "binary.json").write_text("{}", encoding="utf-8")
    (source_dir / "CMakeLists.txt").write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(binary_json_test NONE)
set(ASCEND_PYTHON_EXECUTABLE "python3")
set(ASCENDC_CMAKE_SCRIPTS_PATH "{FWK_MODULES_DIR.as_posix()}")
set(CUSTOM_OPC_OPTIONS "custom_opc_options.ini")
file(MAKE_DIRECTORY "${{CMAKE_BINARY_DIR}}/autogen")
file(WRITE "${{CMAKE_BINARY_DIR}}/autogen/custom_opc_options.ini" "")
set_property(GLOBAL PROPERTY ASCENDC_AUTO_GEN_PATH "${{CMAKE_BINARY_DIR}}/autogen")
include("{(FWK_MODULES_DIR / 'func.cmake').as_posix()}")
npu_op_kernel_sources(ascendc_kernels {kernel_args})
''',
        encoding="utf-8",
    )
    result = subprocess.run(
        ["cmake", "-S", str(source_dir), "-B", str(build_dir)],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    return result, source_dir, build_dir


def test_binary_json_writes_opc_kernel_config(tmp_path):
    result, source_dir, build_dir = _configure(
        tmp_path,
        "OP_TYPE AddCustomTemplate KERNEL_FILE kernel.cpp BINARY_JSON binary.json",
    )

    assert result.returncode == 0, result.stdout
    assert (build_dir / "autogen/custom_opc_options.ini").read_text() == (
        f"AddCustomTemplate@ALL@--kernel-json-file={source_dir / 'binary.json'}\n"
        "AddCustomTemplate@ALL@--simplified_key_mode=None\n"
    )


@pytest.mark.parametrize(
    ("kernel_args", "message"),
    [
        (
            "OP_TYPE AddCustomTemplate KERNEL_FILE kernel.cpp BINARY_JSON",
            "BINARY_JSON requires a file path",
        ),
        (
            "OP_TYPE AddCustomTemplate KERNEL_FILE kernel.cpp BINARY_JSON missing.json",
            "BINARY_JSON must be an existing file",
        ),
        (
            "KERNEL_FILE kernel.cpp BINARY_JSON binary.json",
            "OP_TYPE must be provided when BINARY_JSON is used",
        ),
        (
            "OP_TYPE AddCustomTemplate KERNEL_FILE kernel.cpp BINARY_JSON .",
            "BINARY_JSON must be an existing file",
        ),
    ],
)
def test_binary_json_rejects_invalid_configuration(tmp_path, kernel_args, message):
    result, _, _ = _configure(tmp_path, kernel_args)

    assert result.returncode != 0
    assert message in result.stdout
