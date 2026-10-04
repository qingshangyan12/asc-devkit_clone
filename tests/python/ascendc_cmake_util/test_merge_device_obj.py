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

import sys
from pathlib import Path

import pytest


UTIL_PATH = Path(__file__).resolve().parents[3] / "cmake" / "asc" / "legacy_modules" / "util"
if str(UTIL_PATH) not in sys.path:
    sys.path.insert(0, str(UTIL_PATH))

from merge_device_obj import CompareError, get_mix_obj_info  # noqa: E402


def _write_flags(root, aic_contents, aiv_contents):
    aic_dir = root / "aic"
    aiv_dir = root / "aiv"
    aic_dir.mkdir()
    aiv_dir.mkdir()
    (aic_dir / "mix_build.flag").write_text(aic_contents, encoding="utf-8")
    (aiv_dir / "mix_build.flag").write_text(aiv_contents, encoding="utf-8")
    return str(aic_dir), str(aiv_dir), "mix_build.flag", str(root / "output")


@pytest.mark.parametrize(
    "aic_contents,aiv_contents",
    [
        ("/aic/auto_gen_first.o\n/aic/auto_gen_second.o\n", "/aiv/auto_gen_first.o\n"),
        ("/aic/auto_gen_first.o\n", "/aiv/auto_gen_first.o\n/aiv/auto_gen_second.o\n"),
        ("/aic/auto_gen_first.o\n", "\n"),
        ("\n", "/aiv/auto_gen_first.o\n"),
        ("/aic/auto_gen_first.o\n", "/aiv/auto_gen_other.o\n"),
    ],
)
def test_get_mix_obj_info_rejects_unpaired_objects(tmp_path, aic_contents, aiv_contents):
    args = _write_flags(tmp_path, aic_contents, aiv_contents)

    with pytest.raises(CompareError):
        get_mix_obj_info(*args)


@pytest.mark.parametrize("blank_line", ["", "\n"])
def test_get_mix_obj_info_keeps_paired_objects(tmp_path, blank_line):
    args = _write_flags(tmp_path, "/aic/auto_gen_first.o\n" + blank_line, "/aiv/auto_gen_first.o\n" + blank_line)

    obj_list, merge_list = get_mix_obj_info(*args)

    assert obj_list == ["/aic/auto_gen_first.o", "/aiv/auto_gen_first.o"]
    assert merge_list == [(str(tmp_path / "output" / "first.o"), "/aic/auto_gen_first.o", "/aiv/auto_gen_first.o")]
