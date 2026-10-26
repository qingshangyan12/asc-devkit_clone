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
import shutil
import subprocess
import pytest
from presmoke.werror import append_werror


def test_werror_preserves_quoted_header_directory(tmp_path):
    compiler = shutil.which("cc")
    if not compiler:
        pytest.skip("Host C compiler unavailable")
    headers = tmp_path / "two  spaces"
    headers.mkdir()
    (headers / "value.h").write_text("#define VALUE 7\n")
    source = tmp_path / "main.c"
    source.write_text('#include "value.h"\nint main(void) { return VALUE == 7 ? 0 : 1; }\n')
    flags = append_werror(f'-I"{headers}"')
    output = tmp_path / "program"
    compiled = subprocess.run(
        [compiler, *shlex.split(flags), str(source), "-o", str(output)], capture_output=True, text=True
    )
    assert compiled.returncode == 0, compiled.stderr
    assert subprocess.run([str(output)]).returncode == 0


def test_existing_werror_keeps_original_whitespace():
    flags = '-DNAME="two  spaces" -Werror'
    assert append_werror(flags) == flags
