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

import json
import os
import subprocess
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[3]
SAMPLE_ROOT = (
    REPO_ROOT
    / "examples/01_simd_cpp_api/02_features/99_acl_based/00_acl_compilation/custom_op"
)
ASC_CMAKE_DIR = REPO_ROOT / "cmake/asc"
SOC = "ascend910b"
OP_TYPE = "AddCustomTemplate"
OP_DIR = "add_custom_template"


def _run(command, cwd=None):
    result = subprocess.run(
        command,
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
        env=os.environ.copy(),
        timeout=600,
    )
    if result.stdout:
        print(result.stdout, end="")
    assert result.returncode == 0, (
        f"command failed with exit code {result.returncode}: {' '.join(map(str, command))}\n"
        f"{result.stdout}"
    )
    return result.stdout


def _load_json(path):
    assert path.is_file(), f"missing JSON file: {path}"
    return json.loads(path.read_text(encoding="utf-8"))


def test_custom_simplified_key_compile_smoke(tmp_path):
    build_dir = tmp_path / "build"
    _run(
        [
            "cmake",
            "-S",
            str(SAMPLE_ROOT),
            "-B",
            str(build_dir),
            f"-DASC_DIR={ASC_CMAKE_DIR}",
            f"-DCUSTOM_ASC_CMAKE_PATH={ASC_CMAKE_DIR}",
        ]
    )

    _run(
        [
            "cmake",
            "--build",
            str(build_dir),
            "--target",
            f"{OP_TYPE}_{SOC}",
            "--parallel",
            "2",
        ]
    )

    binary_root = build_dir / "op_kernel/ascendc_kernels/binary"
    op_binary_dir = binary_root / SOC / OP_DIR
    expected_keys = {
        "FP16": [
            "AddCustomTemplate/d=0,p=1/custom_fp16",
            "AddCustomTemplate/d=0,p=0/custom_fp16",
            "AddCustomTemplate/d=1,p=1/custom_fp16",
            "AddCustomTemplate/d=1,p=0/custom_fp16",
        ],
        "FP32": [
            "AddCustomTemplate/d=0,p=1/custom_fp32",
            "AddCustomTemplate/d=0,p=0/custom_fp32",
            "AddCustomTemplate/d=1,p=1/custom_fp32",
            "AddCustomTemplate/d=1,p=0/custom_fp32",
        ],
    }

    for dtype, simplified_keys in expected_keys.items():
        stem = f"AddCustomTemplate_{dtype}"
        binary_path = op_binary_dir / f"{stem}.o"
        assert binary_path.is_file(), f"missing kernel binary: {binary_path}"
        assert binary_path.stat().st_size > 0

        binary_json = _load_json(op_binary_dir / f"{stem}.json")
        assert binary_json["binFileName"] == stem
        assert binary_json["binFileSuffix"] == ".o"
        assert binary_json["supportInfo"]["simplifiedKeyMode"] == 2
        assert binary_json["supportInfo"]["simplifiedKey"] == simplified_keys

    _run(
        [
            "cmake",
            "--build",
            str(build_dir),
            "--target",
            "ascendc_kernels_ascendc_bin_ascend910b_gen_ops_config",
            "--parallel",
            "2",
        ]
    )

    config_path = binary_root / "config" / SOC / "binary_info_config.json"
    config = _load_json(config_path)[OP_TYPE]
    assert config["dynamicRankSupport"] is True
    assert config["simplifiedKeyMode"] == 2

    assert len(config["binaryList"]) == len(expected_keys)
    actual_binary_list = {entry["binPath"]: entry for entry in config["binaryList"]}
    assert set(actual_binary_list) == {
        "ascend910b/add_custom_template/AddCustomTemplate_FP16.o",
        "ascend910b/add_custom_template/AddCustomTemplate_FP32.o",
    }

    for dtype, simplified_keys in expected_keys.items():
        stem = f"AddCustomTemplate_{dtype}"
        bin_path = f"ascend910b/add_custom_template/{stem}.o"
        entry = actual_binary_list[bin_path]
        assert entry["coreType"] == 2
        assert entry["simplifiedKey"] == simplified_keys
        assert entry["jsonPath"] == f"ascend910b/add_custom_template/{stem}.json"
