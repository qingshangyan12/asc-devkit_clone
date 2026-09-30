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

import sys
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(REPO_ROOT / "tools/build"))
sys.path.insert(0, str(REPO_ROOT / "cmake/asc/fwk_modules/util"))

import ascendc_gen_static_library  # noqa: E402


@pytest.mark.parametrize(
    "targets, basenames",
    [
        (("ops", "ops"), ("kernel.cpp.o", "kernel.cpp.o")),
        (("first", "second"), ("kernel.cpp.o", "kernel.cpp.o")),
        (("ops", "ops"), ("first.cpp.o", "second.cpp.o")),
        (("ops", "ops_0"), ("0_kernel.cpp.o", "kernel.cpp.o")),
    ],
)
def test_collect_preserves_each_object_file(tmp_path, targets, basenames):
    file_lists = tmp_path / "file_lists"
    file_lists.mkdir()
    expected_contents = []
    objects_by_target = {}
    for index, (target, basename) in enumerate(zip(targets, basenames)):
        source_dir = tmp_path / f"source_{index}"
        source_dir.mkdir()
        source = source_dir / basename
        content = f"object {index}".encode()
        source.write_bytes(content)
        expected_contents.append(content)
        objects_by_target.setdefault(target, []).append(str(source))
    for target, paths in objects_by_target.items():
        (file_lists / f"{target}.txt").write_text("\n".join(paths), encoding="utf-8")

    output_dir = tmp_path / "collected"
    objects = ascendc_gen_static_library.collect_object_from_files(str(file_lists), str(output_dir))

    assert len(objects) == len(set(objects)) == len(expected_contents)
    assert len(list(output_dir.iterdir())) == len(expected_contents)
    assert sorted((output_dir / name).read_bytes() for name in objects) == sorted(expected_contents)
