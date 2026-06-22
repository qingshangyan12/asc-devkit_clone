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

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
SCRIPT = PROJECT_ROOT / "scripts" / "setup_clangd.py"


def create_cann_root(root: Path) -> None:
    for rel_path in (
        "asc/include",
        "asc/impl",
        "tools/cpudebug/lib/include",
        "include/ascendc/highlevel_api",
        "include/ascendc/host_api",
    ):
        (root / rel_path).mkdir(parents=True)


def run_setup_clangd(*args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [sys.executable, str(SCRIPT), *args],
        cwd=PROJECT_ROOT,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def test_generates_local_clangd_from_template(tmp_path: Path) -> None:
    cann_root = tmp_path / "cann"
    output = tmp_path / ".clangd.local"
    create_cann_root(cann_root)

    result = run_setup_clangd("--cann-root", str(cann_root), "--npu-arch", "3510", "--output", str(output))

    assert result.returncode == 0, result.stderr
    generated = output.read_text(encoding="utf-8")
    assert f"-D__NPU_ARCH__=3510" in generated
    assert str(cann_root / "asc/include") in generated
    assert "@CANN_ROOT@" not in generated
    assert "@NPU_ARCH@" not in generated


def test_rejects_missing_cann_include_directories(tmp_path: Path) -> None:
    cann_root = tmp_path / "cann"
    cann_root.mkdir()

    result = run_setup_clangd("--cann-root", str(cann_root), "--output", str(tmp_path / ".clangd.local"))

    assert result.returncode != 0
    assert "Missing required CANN directories" in result.stderr
    assert "asc/include" in result.stderr


def test_does_not_overwrite_existing_output_without_force(tmp_path: Path) -> None:
    cann_root = tmp_path / "cann"
    output = tmp_path / ".clangd.local"
    create_cann_root(cann_root)
    output.write_text("existing config\n", encoding="utf-8")

    result = run_setup_clangd("--cann-root", str(cann_root), "--output", str(output))

    assert result.returncode != 0
    assert "already exists" in result.stderr
    assert output.read_text(encoding="utf-8") == "existing config\n"


def test_force_overwrites_existing_output(tmp_path: Path) -> None:
    cann_root = tmp_path / "cann"
    output = tmp_path / ".clangd.local"
    create_cann_root(cann_root)
    output.write_text("existing config\n", encoding="utf-8")

    result = run_setup_clangd("--cann-root", str(cann_root), "--npu-arch", "3510", "--output", str(output), "--force")

    assert result.returncode == 0, result.stderr
    generated = output.read_text(encoding="utf-8")
    assert generated != "existing config\n"
    assert f"-D__NPU_ARCH__=3510" in generated
    assert str(cann_root / "asc/include") in generated
