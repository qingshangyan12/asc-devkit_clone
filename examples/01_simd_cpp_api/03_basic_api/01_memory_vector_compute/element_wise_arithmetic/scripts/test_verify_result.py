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


import struct
import subprocess
import sys
from pathlib import Path

import pytest


SCRIPT = Path(__file__).with_name("verify_result.py")


def fp16(*values):
    return struct.pack("=" + "e" * len(values), *values)


@pytest.mark.parametrize(
    "output,golden,expected",
    [
        pytest.param(fp16(1, 2), fp16(1, 2), 0, id="matching-values"),
        pytest.param(fp16(1.0009765625), fp16(1), 0, id="within-tolerance"),
        pytest.param(fp16(2), fp16(1), 1, id="wrong-value"),
        pytest.param(fp16(1), fp16(*([1] * 32)), 1, id="truncated-output"),
        pytest.param(fp16(*([1] * 32)), fp16(1), 1, id="extra-elements"),
        pytest.param(fp16(1) + b"x", fp16(1), 1, id="trailing-byte"),
        pytest.param(fp16(1), fp16(1) + b"x", 1, id="incomplete-reference"),
        pytest.param(fp16(1) + b"x", fp16(1) + b"x", 1, id="both-incomplete"),
        pytest.param(b"", fp16(1), 1, id="empty-output"),
        pytest.param(fp16(1), b"", 1, id="empty-reference"),
        pytest.param(b"", b"", 1, id="both-empty"),
    ],
)
def test_verifier_exit_status(tmp_path, output, golden, expected):
    output_path = tmp_path / "output.bin"
    golden_path = tmp_path / "golden.bin"
    output_path.write_bytes(output)
    golden_path.write_bytes(golden)
    result = subprocess.run(
        [sys.executable, str(SCRIPT), str(output_path), str(golden_path)], capture_output=True, text=True, check=False
    )
    assert result.returncode == expected, result.stdout + result.stderr
    assert ("test pass!" in result.stdout) == (expected == 0)
